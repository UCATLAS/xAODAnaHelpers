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
  
  if (!m_RecommendationTag.empty()) { 
    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("RecommendationTag",m_RecommendationTag)) 
  };
  
  // just the tool default settings
  // https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/TauID/TauAnalysisTools/trunk/doc/README-TauEfficiencyCorrectionsTool.rst 
  
  // initialise reco and EleOLRHadTau SF no matter what 
  std::vector<int> configVec;
  configVec.push_back({TauAnalysisTools::SFRecoHadTau});
  configVec.push_back({TauAnalysisTools::SFEleOLRHadTau});
  
  if ( !m_WorkingPointTauID.empty() ) {
     configVec.push_back({TauAnalysisTools::SFJetIDHadTau});
     
     if      ( m_WorkingPointTauID == "VeryLoose") { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTVERYLOOSE)); }
     else if ( m_WorkingPointTauID == "Loose")     { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTLOOSE));     }
     else if ( m_WorkingPointTauID == "Medium")    { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTMEDIUM));    }
     else if ( m_WorkingPointTauID == "Tight")     { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDBDTTIGHT));     }
     else { 
       ANA_MSG_ERROR("Failed to configure WorkingPointTauID with unknown " << m_WorkingPointTauID);
       return EL::StatusCode::FAILURE;
     }
  } else if ( m_WorkingPointTauID.empty() )      { 
    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("IDLevel", (int)TauAnalysisTools::JETIDNONE));     
    // still consider this a working point
    m_WorkingPointTauID = "None";
  }

  if (!m_WorkingPointEleOLRElectron.empty()) {
    configVec.push_back({TauAnalysisTools::SFEleOLRElectron});
    
    if      (m_WorkingPointEleOLRElectron == "TauEleOLR")            { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::TAUELEOLR));            }
    else if (m_WorkingPointEleOLRElectron == "EleBDTLoose")          { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTLOOSE));          }
    else if (m_WorkingPointEleOLRElectron == "EleBDTLoosePlusVeto")  { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTLOOSEPLUSVETO));  }
    else if (m_WorkingPointEleOLRElectron == "EleBDTMedium")         { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTMEDIUM));         }
    else if (m_WorkingPointEleOLRElectron == "EleBDTMediumPlusVeto") { ANA_CHECK(m_tauEffCorrTool_handle.setProperty("OLRLevel", (int)TauAnalysisTools::ELEBDTMEDIUMPLUSVETO)); }
    else { 
       ANA_MSG_ERROR("Failed to configure WorkingPointEleOLRElectron with unknown " << m_WorkingPointEleOLRElectron);
       return EL::StatusCode::FAILURE;
    }
  } else if ( m_WorkingPointEleOLRElectron.empty() )      { 
    // still consider this a working point
    m_WorkingPointEleOLRElectron = "None";
  }

  if (!m_TriggerName.empty()) {
    // We always want trigger to be individual
    configVec = {TauAnalysisTools::SFTriggerHadTau};
     
    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("TriggerName", m_TriggerName));
    ANA_CHECK(m_tauEffCorrTool_handle.setProperty("PileupReweightingTool",m_pileup_tool_handle));
    
  }           
  
  ANA_CHECK(m_tauEffCorrTool_handle.setProperty("EfficiencyCorrectionTypes",configVec));
  ANA_CHECK(m_tauEffCorrTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool for Tau Efficiency corrections: " << m_tauEffCorrTool_handle);

  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!  First fill a 
  // list of identically inclusive lists and then remove unwanted elements

  const CP::SystematicSet recSysts = m_tauEffCorrTool_handle->recommendedSystematics();
  m_systList = HelperFunctions::getListofSystematics( recSysts, m_systName, m_systVal, msg() );
  
  ANA_MSG_INFO("Will be using TauEfficiencyScaleFactors tool efficiency systematic:");
  for ( const auto& syst_it : m_systList ) {
    if ( m_systName.empty() ) {
  	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
  	break;
    } 
    // IMPORTANT: for the trigger iteration remove the sys if it does not
    // contain the word TRIGGER (?)
    //else if (syst_it.name().find("RECO") == std::string::npos) {
    //  m_systListReco.erase(std::remove(m_systListReco.begin(), m_systListReco.end(), syst_it), m_systListReco.end()); 
    //}
    ANA_MSG_INFO("\t " << syst_it.name());
  }

  
  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systList, m_outputSystNames, fileMD);
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
      // retrieve input taus
      ANA_CHECK( HelperFunctions::retrieve(inputTaus, m_inContainerName+systName, m_event, m_store, msg()) );

      ANA_MSG_DEBUG( "Number of taus: " << static_cast<int>(inputTaus->size()) );
      ANA_MSG_DEBUG( "Input syst: " << systName );
      unsigned int idx(0);
      for ( auto tau : *(inputTaus) ) {
        ANA_MSG_DEBUG( "Input tau " << idx << ", pt = " << tau->pt() * 1e-3 << " GeV" );
        ++idx;
      }

      // decorate taus w/ SF - there will be a decoration w/ different name for each syst!
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
  // Tau efficiency SFs - this is a per-TAU weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //
  
  std::unique_ptr< std::vector< std::string > > sysVariationNames = nullptr;
  
  if ( writeSystNames ) sysVariationNames = std::make_unique< std::vector< std::string > >();

  for ( const auto& syst_it : m_systList ) {
    if ( !syst_it.name().empty() && !nominal ) continue;

    // Create the name of the SF weight to be recorded
    std::string sfName = "TauEff_SF_syst" ;
    if ( !m_WorkingPointEleOLRElectron.empty() ) { sfName += "_EleOLRElectron" + m_WorkingPointEleOLRElectron; }
    if ( !m_WorkingPointTauID.empty() )          { sfName += "_TauID" + m_WorkingPointTauID; }
    if ( !m_TriggerName.empty() )                { sfName += "_Trig" + m_TriggerName; }

    ANA_MSG_DEBUG( "Tau efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
    if( writeSystNames ) sysVariationNames->push_back(syst_it.name());

    // apply syst
    //
    if ( m_tauEffCorrTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR("Failed to configure TauEfficiencyScaleFactors for systematic " << syst_it.name());
      return EL::StatusCode::FAILURE;
    }
    ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

    // and now apply tau efficiency SF!
    //
    unsigned int idx(0);
    for ( auto tau_itr : *(inputTaus) ) {

  	ANA_MSG_DEBUG( "Applying tau efficiency SF" );

  	// obtain tau efficiency SF as a float (to be stored away separately)
  	//
  	//  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* tau)
  	//

  	SG::AuxElement::Decorator< std::vector<float> > sfVec( sfName );
  	if ( !sfVec.isAvailable( *tau_itr ) ) {
          sfVec( *tau_itr ) = std::vector<float>();
  	}

  	double tauEffSF(-1.0);
  	if ( m_tauEffCorrTool_handle->getEfficiencyScaleFactor( *tau_itr, tauEffSF ) != CP::CorrectionCode::Ok ) {
  	  ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	  tauEffSF = -1.0;
  	}
  	//
  	// Add it to decoration vector
  	//
  	sfVec( *tau_itr ).push_back( tauEffSF );

        // tauEff sys names are saved in a vector. Entries positions are preserved!
        //
  	SG::AuxElement::Decorator< std::vector<std::string> > sfVec_sysNames( m_outputSystNames + "_sysNames" );
  	if ( !sfVec_sysNames.isAvailable( *tau_itr ) ) {
          sfVec_sysNames( *tau_itr ) = std::vector<std::string>();
        }
  	sfVec_sysNames( *tau_itr ).push_back( syst_it.name().c_str() );

        ANA_MSG_DEBUG( "===>>>");
        ANA_MSG_DEBUG( "Tau " << idx << ", pt = " << tau_itr->pt()*1e-3 << " GeV" );
        ANA_MSG_DEBUG( "Tau eff. SF decoration: " << sfName );
        ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
        ANA_MSG_DEBUG( "Tau eff. SF:");
        ANA_MSG_DEBUG( "\t " << tauEffSF << " (from getEfficiencyScaleFactor())" );
        ANA_MSG_DEBUG( "--------------------------------------");

  	++idx;

    } // close tau loop

  }  // close loop on reco efficiency SF systematics

  // Add list of systematics names to TStore
  // We only do this once per event if the list does not exist yet
  if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNames ) ) {
    ANA_CHECK( m_store->record( std::move(sysVariationNames), m_outputSystNames ));
  }
  
  return EL::StatusCode::SUCCESS;
}
