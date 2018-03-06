// c++ include(s):
#include <iostream>
#include <map>
#include <random>
#include <algorithm>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>


// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTau/TauJetAuxContainer.h"
#include "xAODTau/TauJet.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/TauEfficiencyCorrector.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(TauEfficiencyCorrector)


TauEfficiencyCorrector :: TauEfficiencyCorrector () :
    Algorithm("TauEfficiencyCorrector")
{
}


EL::StatusCode TauEfficiencyCorrector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_INFO( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "TauEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauEfficiencyCorrector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauEfficiencyCorrector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauEfficiencyCorrector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_INFO( "Initializing TauEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;


  // *******************************************************

  // Create a ToolHandle of the PRW tool which is passed to the TauEfficiencyScaleFactors class later
  //
  if( isMC() ){
    if(!m_pileup_tool_handle.isUserConfigured()){
      ANA_MSG_FATAL("A configured " << m_pileup_tool_handle.typeAndName() << " must have been previously created! Are you creating one in xAH::BasicEventSelection?" );
      return EL::StatusCode::FAILURE;
    }
    ANA_CHECK( m_pileup_tool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_pileup_tool_handle);
  }

  if (!m_RecommendationTag.empty()) ANA_CHECK(m_tauEffCorrTool_handle.setProperty("RecommendationTag",m_RecommendationTag));
  
  // just the tool default settings
  // https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/TauID/TauAnalysisTools/trunk/doc/README-TauEfficiencyCorrectionsTool.rst 
  std::vector<int> defaultSetting;
  
  if (!m_WorkingPointReco.empty())           defaultSetting.push_back(TauAnalysisTools::SFRecoHadTau);
  if (!m_WorkingPointEleOLRHadTau.empty())   defaultSetting.push_back(TauAnalysisTools::SFEleOLRHadTau); 
  
  if (!m_WorkingPointEleOLRElectron.empty()) {
    defaultSetting.push_back(TauAnalysisTools::SFEleOLRElectron);

    if (m_WorkingPointEleOLRElectron == "TauEleOLR")            ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::TAUELEOLR));
    if (m_WorkingPointEleOLRElectron == "EleBDTLoose")          ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTLOOSE));
    if (m_WorkingPointEleOLRElectron == "EleBDTLoosePlusVeto")  ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTLOOSEPLUSVETO));
    if (m_WorkingPointEleOLRElectron == "EleBDTMedium")         ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTMEDIUM));
    if (m_WorkingPointEleOLRElectron == "EleBDTMediumPlusVeto") ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTMEDIUMPLUSVETO));
  }
  
  if (!m_WorkingPointTauID.empty()) {
    defaultSetting.push_back(TauAnalysisTools::SFJetIDHadTau);
  
    if (m_WorkingPointTauID == "VeryLoose") ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTVERYLOOSE));
    if (m_WorkingPointTauID == "Loose")     ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTLOOSE));
    if (m_WorkingPointTauID == "Medium")    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTMEDIUM));
    if (m_WorkingPointTauID == "Tight")     ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTTIGHT));
  }           

  if (!m_WorkingPointTrigger.empty() && !m_TriggerName.empty()) {
    defaultSetting.push_back(TauAnalysisTools::SFTriggerHadTau);

    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("TriggerName", m_TriggerName));
    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("PileupReweightingTool",m_pileup_tool_handle));
    
    if (m_WorkingPointTrigger == "VeryLoose") ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTVERYLOOSE));
    if (m_WorkingPointTrigger == "Loose")     ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTLOOSE));
    if (m_WorkingPointTrigger == "Medium")    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTMEDIUM));
    if (m_WorkingPointTrigger == "Tight")     ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTTIGHT));
  }           

  ANA_CHECK(m_tauEffCorrTool_handle.setProperty("EfficiencyCorrectionTypes",defaultSetting));
  ANA_CHECK(m_tauEffCorrTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_tauEffCorrTool_handle);


  CP::SystematicSet affectSysts = m_tauEffCorrTool_handle->affectingSystematics();
  for ( const auto& syst_it : affectSysts ) { ANA_MSG_DEBUG("TauEfficiencyScaleFactors tool can be affected by efficiency systematic: " << syst_it.name()); }
 


  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!  First fill a 
  // list of identically inclusive lists and then remove unwanted elements
 
  // --------------------------- 
  // building tau reco syst list 
  // ---------------------------
  //
  if (!m_WorkingPointReco.empty()) {
    const CP::SystematicSet recSystsReco = m_tauEffCorrTool_handle->recommendedSystematics();
    m_outputSystNamesReco = m_outputSystNamesReco + "_" + m_WorkingPointReco;
    m_systListReco = HelperFunctions::getListofSystematics( recSystsReco, m_systNameReco, m_systValReco, msg() );
    
    ANA_MSG_INFO("Will be using TauEfficiencyScaleFactors tool reco efficiency systematic:");
    for ( const auto& syst_it : m_systListReco ) {
      if ( m_systNameReco.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      } else if (syst_it.name().find("RECO") == std::string::npos) {
        m_systListReco.erase(std::remove(m_systListReco.begin(), m_systListReco.end(), syst_it), m_systListReco.end()); 
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  } 
  
  // -------------------------
  // building tau ID syst list 
  // -------------------------
  //
  if (!m_WorkingPointTauID.empty()) {
    const CP::SystematicSet recSystsTauID = m_tauEffCorrTool_handle->recommendedSystematics();
    m_outputSystNamesTauID = m_outputSystNamesTauID + "_TauID" + m_WorkingPointTauID;
    m_systListTauID = HelperFunctions::getListofSystematics( recSystsTauID, m_systNameTauID, m_systValTauID, msg() );
    
    ANA_MSG_INFO("Will be using TauEfficiencyScaleFactors tool tau ID efficiency systematic:");
    for ( const auto& syst_it : m_systListTauID ) {
      if ( m_systNameTauID.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      } else if (syst_it.name().find("JETID") == std::string::npos) {
        m_systListTauID.erase(std::remove(m_systListTauID.begin(), m_systListTauID.end(), syst_it), m_systListTauID.end()); 
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  } 

  // ----------------------------------
  // building true tauhad OLR syst list 
  // ----------------------------------
  //
  if (!m_WorkingPointEleOLRHadTau.empty()) {
    const CP::SystematicSet recSystsEleOLRHadTau = m_tauEffCorrTool_handle->recommendedSystematics();
    m_outputSystNamesEleOLRHadTau = m_outputSystNamesEleOLRHadTau + "_" + m_WorkingPointEleOLRHadTau;
    m_systListEleOLRHadTau = HelperFunctions::getListofSystematics( recSystsEleOLRHadTau, m_systNameEleOLRHadTau, m_systValEleOLRHadTau, msg() );
    
    ANA_MSG_INFO("Will be using TauEfficiencyScaleFactors tool tau ID efficiency systematic:");
    for ( const auto& syst_it : m_systListEleOLRHadTau ) {
      if ( m_systNameEleOLRHadTau.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      } else if (syst_it.name().find("OLR") == std::string::npos && syst_it.name().find("BDT") != std::string::npos) { // cross-check the full list of names
        m_systListEleOLRHadTau.erase(std::remove(m_systListEleOLRHadTau.begin(), m_systListEleOLRHadTau.end(), syst_it), m_systListEleOLRHadTau.end()); 
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  } 

  // ------------------------------------
  // building true electron OLR syst list 
  // ------------------------------------
  //
  if (!m_WorkingPointEleOLRElectron.empty()) {
    const CP::SystematicSet recSystsEleOLRElectron = m_tauEffCorrTool_handle->recommendedSystematics();
    m_outputSystNamesEleOLRElectron = m_outputSystNamesEleOLRElectron + "_EleOLRElectron" + m_WorkingPointEleOLRElectron;
    m_systListEleOLRElectron = HelperFunctions::getListofSystematics( recSystsEleOLRElectron, m_systNameEleOLRElectron, m_systValEleOLRElectron, msg() );
    
    ANA_MSG_INFO("Will be using TauEfficiencyScaleFactors tool tau ID efficiency systematic:");
    for ( const auto& syst_it : m_systListEleOLRElectron ) {
      if ( m_systNameEleOLRElectron.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      } else if (syst_it.name().find("OLR") == std::string::npos && syst_it.name().find("BDT") == std::string::npos) { // cross-check the full list of names
        m_systListEleOLRElectron.erase(std::remove(m_systListEleOLRElectron.begin(), m_systListEleOLRElectron.end(), syst_it), m_systListEleOLRElectron.end()); 
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  } 

  // ------------------------------
  // building tau trigger syst list
  // ------------------------------
  //
  if (!m_WorkingPointTrigger.empty() && !m_TriggerName.empty()) {
    const CP::SystematicSet recSystsTrigger = m_tauEffCorrTool_handle->recommendedSystematics();
    m_outputSystNamesTrigger = m_outputSystNamesTrigger + "_Trigger" + m_TriggerName + "_TauID" + m_WorkingPointTrigger;
    m_systListTrigger = HelperFunctions::getListofSystematics( recSystsTrigger, m_systNameTrigger, m_systValTrigger, msg() );
    ANA_MSG_INFO("Will be using TauEfficiencyScaleFactors tool tau trigger efficiency systematic:");
    for ( const auto& syst_it : m_systListTrigger ) {
      if ( m_systNameTrigger.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      } else if (syst_it.name().find("TRIGGER") == std::string::npos) {
        m_systListTrigger.erase(std::remove(m_systListTrigger.begin(), m_systListTrigger.end(), syst_it), m_systListTrigger.end()); 
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  } 

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systListReco, m_outputSystNamesReco, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListTauID, m_outputSystNamesTauID, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListEleOLRHadTau, m_outputSystNamesEleOLRHadTau, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListEleOLRElectron, m_outputSystNamesEleOLRElectron, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListTrigger, m_outputSystNamesTrigger, fileMD);
  }

  // *********************************************************************************

  ANA_MSG_INFO( "TauEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode TauEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  m_numEvent++;

  if ( !isMC() ) {
    if ( m_numEvent == 1 ) { ANA_MSG_INFO( "Sample is Data! Do not apply any Tau Efficiency correction... "); }
    return EL::StatusCode::SUCCESS;
  }

  ANA_MSG_DEBUG( "Applying Tau Efficiency corrections... ");
 
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // if m_inputSystNamesTaus = "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  std::vector<std::string>* systNames_ptr(nullptr);
  if ( !m_inputSystNamesTaus.empty() ) ANA_CHECK( HelperFunctions::retrieve(systNames_ptr, m_inputSystNamesTaus, 0, m_store, msg()) );

  std::vector<std::string> systNames{""};
  if (systNames_ptr) systNames = *systNames_ptr;

  // Declare a write status set to true
  // For the systematically varied input containers, we won't store again the vector with efficiency systs in TStore ( it will be always the same!)
  //
  bool writeSystNames(true);

  // loop over systematic sets available
  for ( auto systName : systNames ) {
    const xAOD::TauJetContainer* inputTaus(nullptr);

    // some systematics might have rejected the event
    if ( m_store->contains<xAOD::TauJetContainer>( m_inContainerName+systName ) ) {
      // retrieve input muons
      ANA_CHECK( HelperFunctions::retrieve(inputTaus, m_inContainerName+systName, m_event, m_store, msg()) );

      ANA_MSG_DEBUG( "Number of taus: " << static_cast<int>(inputTaus->size()) );
      ANA_MSG_DEBUG( "Input syst: " << systName );
      unsigned int idx(0);
      for ( auto mu : *(inputTaus) ) {
        ANA_MSG_DEBUG( "Input tau " << idx << ", pt = " << mu->pt() * 1e-3 << " GeV" );
        ++idx;
      }

      // decorate muons w/ SF - there will be a decoration w/ different name for each syst!
      this->executeSF( eventInfo, inputTaus, systName.empty(), writeSystNames );

      writeSystNames = false;

    } // check existence of container

  } // close loop on systematic sets available from upstream algo

  // look what we have in TStore
  //
  if(msgLvl(MSG::VERBOSE)) m_store->print();
  
  return EL::StatusCode::SUCCESS;

}


EL::StatusCode TauEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG( "Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauEfficiencyCorrector :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  ANA_MSG_INFO( "Deleting tool instances...");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode TauEfficiencyCorrector :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.

  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TauEfficiencyCorrector :: executeSF ( const xAOD::EventInfo* /*eventInfo*/, const xAOD::TauJetContainer* inputTaus, bool nominal, bool writeSystNames )
{
  //**************************** 
  
  //
  // In the following, every tau gets decorated with 2 vector<double>'s (for reco/iso efficiency SFs),
  // and the event w/ 1 vector<double> (for trigger efficiency SFs)
  // Each vector contains the SFs, one SF for each syst (first component of each vector will be the nominal SF).
  //
  // Additionally, we create these vector<string> with the SF syst names, so that we know which component corresponds to.
  // ( there's a 1:1 correspondence with the vector<double> defined above )
  //
  // These vector<string> are eventually stored in TStore
  //

  // 1.
  // Reco efficiency SFs - this is a per-TAU weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //


  // ----------------------------- 
  // applying tau reco corrections
  // -----------------------------
  //
  std::unique_ptr< std::vector< std::string > > sysVariationNamesReco = nullptr;
  
  if ( writeSystNames ) sysVariationNamesReco = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

  for ( const auto& syst_it : m_systListReco ) {
    if ( !syst_it.name().empty() && !nominal ) continue;

    // Create the name of the SF weight to be recorded
    std::string sfName = "TauRecoEff_SF_syst_" + m_WorkingPointReco;

    ANA_MSG_DEBUG( "Tau reco efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
    if( writeSystNames ) sysVariationNamesReco->push_back(syst_it.name());

    // apply syst
    //
    if ( m_tauEffCorrTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR("Failed to configure TauEfficiencyScaleFactors for systematic " << syst_it.name());
      return EL::StatusCode::FAILURE;
    }
    ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

    // and now apply reco efficiency SF!
    //
    unsigned int idx(0);
    for ( auto tau_itr : *(inputTaus) ) {

  	ANA_MSG_DEBUG( "Applying reco efficiency SF" );

  	// obtain reco efficiency SF as a float (to be stored away separately)
  	//
  	//  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* tau)
  	//

  	SG::AuxElement::Decorator< std::vector<float> > sfVecReco( sfName );
  	if ( !sfVecReco.isAvailable( *tau_itr ) ) {
          sfVecReco( *tau_itr ) = std::vector<float>();
  	}

  	double recoEffSF(1.0);
  	if ( m_tauEffCorrTool_handle->getEfficiencyScaleFactor( *tau_itr, recoEffSF ) != CP::CorrectionCode::Ok ) {
  	  ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	  recoEffSF = 1.0;
  	}
  	//
  	// Add it to decoration vector
  	//
  	sfVecReco( *tau_itr ).push_back( recoEffSF );

        // reco sys names are saved in a vector. Entries positions are preserved!
        //
  	SG::AuxElement::Decorator< std::vector<std::string> > sfVecReco_sysNames( m_outputSystNamesReco + "_sysNames" );
  	if ( !sfVecReco_sysNames.isAvailable( *tau_itr ) ) {
          sfVecReco_sysNames( *tau_itr ) = std::vector<std::string>();
        }
  	sfVecReco_sysNames( *tau_itr ).push_back( syst_it.name().c_str() );

        ANA_MSG_DEBUG( "===>>>");
        ANA_MSG_DEBUG( "Tau " << idx << ", pt = " << tau_itr->pt()*1e-3 << " GeV" );
        ANA_MSG_DEBUG( "Reco eff. SF decoration: " << sfName );
        ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
        ANA_MSG_DEBUG( "Reco eff. SF:");
        ANA_MSG_DEBUG( "\t " << recoEffSF << " (from getEfficiencyScaleFactor())" );
        ANA_MSG_DEBUG( "--------------------------------------");

  	++idx;

    } // close muon loop

  }  // close loop on reco efficiency SF systematics

  // Add list of systematics names to TStore
  // We only do this once per event if the list does not exist yet
  if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesReco ) ) {
    ANA_CHECK( m_store->record( std::move(sysVariationNamesReco), m_outputSystNamesReco ));
  }


  // ---------------------------
  // applying tau ID corrections
  // ---------------------------
  //
  std::unique_ptr< std::vector< std::string > > sysVariationNamesTauID = nullptr;
  
  if ( writeSystNames ) sysVariationNamesTauID = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

  for ( const auto& syst_it : m_systListTauID ) {
    if ( !syst_it.name().empty() && !nominal ) continue;

    // Create the name of the SF weight to be recorded
    std::string sfName = "TauTauIDEff_SF_syst_TauID" + m_WorkingPointTauID;

    ANA_MSG_DEBUG( "Tau reco efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
    if( writeSystNames ) sysVariationNamesTauID->push_back(syst_it.name());

    // apply syst
    //
    if ( m_tauEffCorrTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR("Failed to configure TauEfficiencyScaleFactors for systematic " << syst_it.name());
  	return EL::StatusCode::FAILURE;
    }
    ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

    // and now apply reco efficiency SF!
    //
    unsigned int idx(0);
    for ( auto tau_itr : *(inputTaus) ) {

  	ANA_MSG_DEBUG( "Applying TauID efficiency SF" );

  	// obtain reco efficiency SF as a float (to be stored away separately)
  	//
  	//  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* tau)
  	//

  	SG::AuxElement::Decorator< std::vector<float> > sfVecTauID( sfName );
  	if ( !sfVecTauID.isAvailable( *tau_itr ) ) {
          sfVecTauID( *tau_itr ) = std::vector<float>();
  	}

  	double tauIDEffSF(1.0);
  	if ( m_tauEffCorrTool_handle->getEfficiencyScaleFactor( *tau_itr, tauIDEffSF ) != CP::CorrectionCode::Ok ) {
  	  ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	  tauIDEffSF = 1.0;
  	}
  	//
  	// Add it to decoration vector
  	//
  	sfVecTauID( *tau_itr ).push_back( tauIDEffSF );

        // reco sys names are saved in a vector. Entries positions are preserved!
        //
  	SG::AuxElement::Decorator< std::vector<std::string> > sfVecTauID_sysNames( m_outputSystNamesTauID + "_sysNames" );
  	if ( !sfVecTauID_sysNames.isAvailable( *tau_itr ) ) {
          sfVecTauID_sysNames( *tau_itr ) = std::vector<std::string>();
        }
  	sfVecTauID_sysNames( *tau_itr ).push_back( syst_it.name().c_str() );

        ANA_MSG_DEBUG( "===>>>");
        ANA_MSG_DEBUG( "Tau " << idx << ", pt = " << tau_itr->pt()*1e-3 << " GeV" );
        ANA_MSG_DEBUG( "TauID eff. SF decoration: " << sfName );
        ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
        ANA_MSG_DEBUG( "TauID eff. SF:");
        ANA_MSG_DEBUG( "\t " << tauIDEffSF << " (from getEfficiencyScaleFactor())" );
        ANA_MSG_DEBUG( "--------------------------------------");

  	++idx;

    } // close muon loop

  }  // close loop on reco efficiency SF systematics

  // Add list of systematics names to TStore
  // We only do this once per event if the list does not exist yet
  if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesTauID ) ) {
    ANA_CHECK( m_store->record( std::move(sysVariationNamesTauID), m_outputSystNamesTauID ));
  }

  // ------------------------------------ 
  // applying true tauhad OLR corrections 
  // ------------------------------------
  //
  std::unique_ptr< std::vector< std::string > > sysVariationNamesEleOLRHadTau = nullptr;
  
  if ( writeSystNames ) sysVariationNamesEleOLRHadTau = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

  for ( const auto& syst_it : m_systListEleOLRHadTau ) {
    if ( !syst_it.name().empty() && !nominal ) continue;

    // Create the name of the SF weight to be recorded
    std::string sfName = "TauEleOLRHadTauEff_SF_syst_" + m_WorkingPointEleOLRHadTau;

    ANA_MSG_DEBUG( "Tau eleOLRHadTau efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
    if( writeSystNames ) sysVariationNamesEleOLRHadTau->push_back(syst_it.name());

    // apply syst
    //
    if ( m_tauEffCorrTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR("Failed to configure TauEfficiencyScaleFactors for systematic " << syst_it.name());
      return EL::StatusCode::FAILURE;
    }
    ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

    // and now apply eleOLRHadTau efficiency SF!
    //
    unsigned int idx(0);
    for ( auto tau_itr : *(inputTaus) ) {

  	ANA_MSG_DEBUG( "Applying eleOLRHadTau efficiency SF" );

  	// obtain eleOLRHadTau efficiency SF as a float (to be stored away separately)
  	//
  	//  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* tau)
  	//

  	SG::AuxElement::Decorator< std::vector<float> > sfVecEleOLRHadTau( sfName );
  	if ( !sfVecEleOLRHadTau.isAvailable( *tau_itr ) ) {
          sfVecEleOLRHadTau( *tau_itr ) = std::vector<float>();
  	}

  	double eleOLRHadTauEffSF(1.0);
  	if ( m_tauEffCorrTool_handle->getEfficiencyScaleFactor( *tau_itr, eleOLRHadTauEffSF ) != CP::CorrectionCode::Ok ) {
  	  ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	  eleOLRHadTauEffSF = 1.0;
  	}
  	//
  	// Add it to decoration vector
  	//
  	sfVecEleOLRHadTau( *tau_itr ).push_back( eleOLRHadTauEffSF );

        // eleOLRHadTau sys names are saved in a vector. Entries positions are preserved!
        //
  	SG::AuxElement::Decorator< std::vector<std::string> > sfVecEleOLRHadTau_sysNames( m_outputSystNamesEleOLRHadTau + "_sysNames" );
  	if ( !sfVecEleOLRHadTau_sysNames.isAvailable( *tau_itr ) ) {
          sfVecEleOLRHadTau_sysNames( *tau_itr ) = std::vector<std::string>();
        }
  	sfVecEleOLRHadTau_sysNames( *tau_itr ).push_back( syst_it.name().c_str() );

        ANA_MSG_DEBUG( "===>>>");
        ANA_MSG_DEBUG( "Tau " << idx << ", pt = " << tau_itr->pt()*1e-3 << " GeV" );
        ANA_MSG_DEBUG( "EleOLRHadTau eff. SF decoration: " << sfName );
        ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
        ANA_MSG_DEBUG( "EleOLRHadTau eff. SF:");
        ANA_MSG_DEBUG( "\t " << eleOLRHadTauEffSF << " (from getEfficiencyScaleFactor())" );
        ANA_MSG_DEBUG( "--------------------------------------");

  	++idx;

    } // close muon loop

  }  // close loop on eleOLRHadTau efficiency SF systematics

  // Add list of systematics names to TStore
  // We only do this once per event if the list does not exist yet
  if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesEleOLRHadTau ) ) {
    ANA_CHECK( m_store->record( std::move(sysVariationNamesEleOLRHadTau), m_outputSystNamesEleOLRHadTau ));
  }


  // --------------------------------------
  // applying true electron OLR corrections
  // --------------------------------------
  //
  std::unique_ptr< std::vector< std::string > > sysVariationNamesEleOLRElectron = nullptr;
  
  if ( writeSystNames ) sysVariationNamesEleOLRElectron = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

  for ( const auto& syst_it : m_systListEleOLRElectron ) {
    if ( !syst_it.name().empty() && !nominal ) continue;

    // Create the name of the SF weight to be recorded
    std::string sfName = "TauEleOLRElectronEff_SF_syst_EleOLRElectron" + m_WorkingPointEleOLRElectron;

    ANA_MSG_DEBUG( "Tau reco efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
    if( writeSystNames ) sysVariationNamesEleOLRElectron->push_back(syst_it.name());

    // apply syst
    //
    if ( m_tauEffCorrTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR("Failed to configure TauEfficiencyScaleFactors for systematic " << syst_it.name());
  	return EL::StatusCode::FAILURE;
    }
    ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

    // and now apply reco efficiency SF!
    //
    unsigned int idx(0);
    for ( auto tau_itr : *(inputTaus) ) {

  	ANA_MSG_DEBUG( "Applying EleOLRElectron efficiency SF" );

  	// obtain reco efficiency SF as a float (to be stored away separately)
  	//
  	//  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* tau)
  	//

  	SG::AuxElement::Decorator< std::vector<float> > sfVecEleOLRElectron( sfName );
  	if ( !sfVecEleOLRElectron.isAvailable( *tau_itr ) ) {
          sfVecEleOLRElectron( *tau_itr ) = std::vector<float>();
  	}

  	double eleOLRElectronEffSF(1.0);
  	if ( m_tauEffCorrTool_handle->getEfficiencyScaleFactor( *tau_itr, eleOLRElectronEffSF ) != CP::CorrectionCode::Ok ) {
  	  ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	  eleOLRElectronEffSF = 1.0;
  	}
  	//
  	// Add it to decoration vector
  	//
  	sfVecEleOLRElectron( *tau_itr ).push_back( eleOLRElectronEffSF );

        // reco sys names are saved in a vector. Entries positions are preserved!
        //
  	SG::AuxElement::Decorator< std::vector<std::string> > sfVecEleOLRElectron_sysNames( m_outputSystNamesEleOLRElectron + "_sysNames" );
  	if ( !sfVecEleOLRElectron_sysNames.isAvailable( *tau_itr ) ) {
          sfVecEleOLRElectron_sysNames( *tau_itr ) = std::vector<std::string>();
        }
  	sfVecEleOLRElectron_sysNames( *tau_itr ).push_back( syst_it.name().c_str() );

        ANA_MSG_DEBUG( "===>>>");
        ANA_MSG_DEBUG( "Tau " << idx << ", pt = " << tau_itr->pt()*1e-3 << " GeV" );
        ANA_MSG_DEBUG( "EleOLRElectron eff. SF decoration: " << sfName );
        ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
        ANA_MSG_DEBUG( "EleOLRElectron eff. SF:");
        ANA_MSG_DEBUG( "\t " << eleOLRElectronEffSF << " (from getEfficiencyScaleFactor())" );
        ANA_MSG_DEBUG( "--------------------------------------");

  	++idx;

    } // close muon loop

  }  // close loop on reco efficiency SF systematics

  // Add list of systematics names to TStore
  // We only do this once per event if the list does not exist yet
  if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesEleOLRElectron ) ) {
    ANA_CHECK( m_store->record( std::move(sysVariationNamesEleOLRElectron), m_outputSystNamesEleOLRElectron ));
  }


  // --------------------------------
  // applying tau trigger corrections
  // --------------------------------
  //
  std::unique_ptr< std::vector< std::string > > sysVariationNamesTrigger = nullptr;
  
  if ( writeSystNames ) sysVariationNamesTrigger = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

  for ( const auto& syst_it : m_systListTrigger ) {
    if ( !syst_it.name().empty() && !nominal ) continue;

    // Create the name of the SF weight to be recorded
    std::string sfName = "TauTriggerEff_SF_syst_Trigger" + m_TriggerName + "_TauID" + m_WorkingPointTrigger;
    
    ANA_MSG_DEBUG( "Tau reco efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
    if( writeSystNames ) sysVariationNamesTrigger->push_back(syst_it.name());

    // apply syst
    //
    if ( m_tauEffCorrTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR("Failed to configure TauEfficiencyScaleFactors for systematic " << syst_it.name());
  	return EL::StatusCode::FAILURE;
    }
    ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

    // and now apply reco efficiency SF!
    //
    unsigned int idx(0);
    for ( auto tau_itr : *(inputTaus) ) {

  	ANA_MSG_DEBUG( "Applying Trigger efficiency SF" );

  	// obtain reco efficiency SF as a float (to be stored away separately)
  	//
  	//  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* tau)
  	//

  	SG::AuxElement::Decorator< std::vector<float> > sfVecTrigger( sfName );
  	if ( !sfVecTrigger.isAvailable( *tau_itr ) ) {
          sfVecTrigger( *tau_itr ) = std::vector<float>();
  	}

  	double trigEffSF(1.0);
  	if ( m_tauEffCorrTool_handle->getEfficiencyScaleFactor( *tau_itr, trigEffSF ) != CP::CorrectionCode::Ok ) {
  	  ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	  trigEffSF = 1.0;
  	}
  	//
  	// Add it to decoration vector
  	//
  	sfVecTrigger( *tau_itr ).push_back( trigEffSF );

        // reco sys names are saved in a vector. Entries positions are preserved!
        //
  	SG::AuxElement::Decorator< std::vector<std::string> > sfVecTrigger_sysNames( m_outputSystNamesTrigger + "_sysNames" );
  	if ( !sfVecTrigger_sysNames.isAvailable( *tau_itr ) ) {
          sfVecTrigger_sysNames( *tau_itr ) = std::vector<std::string>();
        }
  	sfVecTrigger_sysNames( *tau_itr ).push_back( syst_it.name().c_str() );

        ANA_MSG_DEBUG( "===>>>");
        ANA_MSG_DEBUG( "Tau " << idx << ", pt = " << tau_itr->pt()*1e-3 << " GeV" );
        ANA_MSG_DEBUG( "Trigger eff. SF decoration: " << sfName );
        ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
        ANA_MSG_DEBUG( "Trigger eff. SF:");
        ANA_MSG_DEBUG( "\t " << trigEffSF << " (from getEfficiencyScaleFactor())" );
        ANA_MSG_DEBUG( "--------------------------------------");

  	++idx;

    } // close muon loop

  }  // close loop on reco efficiency SF systematics

  // Add list of systematics names to TStore
  // We only do this once per event if the list does not exist yet
  if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesTrigger ) ) {
    ANA_CHECK( m_store->record( std::move(sysVariationNamesTrigger), m_outputSystNamesTrigger ));
  }

  return EL::StatusCode::SUCCESS;
}
