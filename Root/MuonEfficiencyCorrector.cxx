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
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/MuonEfficiencyCorrector.h"
#include "MuonEfficiencyCorrections/MuonEfficiencyScaleFactors.h"
#include "MuonEfficiencyCorrections/MuonTriggerScaleFactors.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(MuonEfficiencyCorrector)


MuonEfficiencyCorrector :: MuonEfficiencyCorrector () :
    Algorithm("MuonEfficiencyCorrector")
{
}


EL::StatusCode MuonEfficiencyCorrector :: setupJob (EL::Job& job)
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
  xAOD::Init( "MuonEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_INFO( "Initializing MuonEfficiencyCorrector Interface... ");

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

  // Create a ToolHandle of the PRW tool which is passed to the MuonEfficiencyScaleFactors class later
  //
  if( isMC() ){
    if(!m_pileup_tool_handle.isUserConfigured()){
      ANA_MSG_FATAL("A configured " << m_pileup_tool_handle.typeAndName() << " must have been previously created! Are you creating one in xAH::BasicEventSelection?" );
      return EL::StatusCode::FAILURE;
    }
    ANA_CHECK( m_pileup_tool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_pileup_tool_handle);
  }


  // 1.
  // initialize the CP::MuonEfficiencyScaleFactors Tool for reco efficiency SF
  //
  m_recoEffSF_tool_name = "MuonEfficiencyScaleFactors_effSF_Reco_" + m_WorkingPointReco;

  ANA_MSG_INFO( " Initialising CP::MuonEfficiencyScaleFactors for RECO efficiency SF..." );

  ANA_CHECK( checkToolStore<CP::MuonEfficiencyScaleFactors>(m_recoEffSF_tool_name));
  if ( asg::ToolStore::contains<CP::MuonEfficiencyScaleFactors>(m_recoEffSF_tool_name) ) {
    m_muRecoSF_tool = asg::ToolStore::get<CP::MuonEfficiencyScaleFactors>( m_recoEffSF_tool_name );
  } else {
    m_muRecoSF_tool = new CP::MuonEfficiencyScaleFactors( m_recoEffSF_tool_name );
    ANA_CHECK( m_muRecoSF_tool->setProperty("WorkingPoint", m_WorkingPointReco ));
    ANA_CHECK( m_muRecoSF_tool->setProperty("CalibrationRelease", m_calibRelease ));
    ANA_CHECK( m_muRecoSF_tool->initialize());

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesReco = m_outputSystNamesReco + "_Reco" + m_WorkingPointReco;

    CP::SystematicSet affectSystsReco = m_muRecoSF_tool->affectingSystematics();
    for ( const auto& syst_it : affectSystsReco ) { ANA_MSG_DEBUG("MuonEfficiencyScaleFactors tool can be affected by reco efficiency systematic: " << syst_it.name()); }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsReco = m_muRecoSF_tool->recommendedSystematics();
    m_systListReco = HelperFunctions::getListofSystematics( recSystsReco, m_systNameReco, m_systValReco, msg() );

    ANA_MSG_INFO("Will be using MuonEfficiencyScaleFactors tool reco efficiency systematic:");
    for ( const auto& syst_it : m_systListReco ) {
      if ( m_systNameReco.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  }

  // 2.
  // initialize the CP::MuonEfficiencyScaleFactors Tool for isolation efficiency SF
  //

  m_isoEffSF_tool_name = "MuonEfficiencyScaleFactors_effSF_Iso_" + m_WorkingPointIso;
  std::string iso_WP = m_WorkingPointIso + "Iso";

  ANA_MSG_INFO( " Initialising CP::MuonEfficiencyScaleFactors for ISO efficiency SF..." );

  ANA_CHECK( checkToolStore<CP::MuonEfficiencyScaleFactors>(m_isoEffSF_tool_name));
  if ( asg::ToolStore::contains<CP::MuonEfficiencyScaleFactors>(m_isoEffSF_tool_name) ) {
    m_muIsoSF_tool = asg::ToolStore::get<CP::MuonEfficiencyScaleFactors>( m_isoEffSF_tool_name );
  } else {
    m_muIsoSF_tool = new CP::MuonEfficiencyScaleFactors( m_isoEffSF_tool_name );
    ANA_CHECK( m_muIsoSF_tool->setProperty("WorkingPoint", iso_WP ));
    ANA_CHECK( m_muIsoSF_tool->setProperty("CalibrationRelease", m_calibRelease ));
    ANA_CHECK( m_muIsoSF_tool->initialize());

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesIso = m_outputSystNamesIso + "_Iso" + m_WorkingPointIso;

    CP::SystematicSet affectSystsIso = m_muIsoSF_tool->affectingSystematics();
    for ( const auto& syst_it : affectSystsIso ) { ANA_MSG_DEBUG("MuonEfficiencyScaleFactors tool can be affected by iso efficiency systematic: " << syst_it.name()); }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsIso = m_muIsoSF_tool->recommendedSystematics();
    m_systListIso = HelperFunctions::getListofSystematics( recSystsIso, m_systNameIso, m_systValIso, msg() );

    ANA_MSG_INFO("Will be using MuonEfficiencyScaleFactors tool iso efficiency systematic:");
    for ( const auto& syst_it : m_systListIso ) {
      if ( m_systNameIso.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  }

  // 3.
  // Initialise the CP::MuonTriggerScaleFactors tool
  //

  m_trigEffSF_tool_name = "MuonTriggerScaleFactors_effSF_Trig_Reco" + m_WorkingPointReco;

  ANA_MSG_INFO( " Initialising CP::MuonTriggerScaleFactors for TRIGGER efficiency SF..." );

  ANA_CHECK( checkToolStore<CP::MuonTriggerScaleFactors>(m_trigEffSF_tool_name));
  if ( asg::ToolStore::contains<CP::MuonTriggerScaleFactors>( m_trigEffSF_tool_name ) ) {
    m_muTrigSF_tool = asg::ToolStore::get<CP::MuonTriggerScaleFactors>( m_trigEffSF_tool_name );
  } else {
    m_muTrigSF_tool = new CP::MuonTriggerScaleFactors( m_trigEffSF_tool_name );

    if ( m_AllowZeroSF ) {
      ANA_MSG_WARNING( "m_AllowZeroSF is set to True. No errors will arise for runs missing required triggers!!!");
      ANA_CHECK( m_muTrigSF_tool->setProperty("AllowZeroSF", m_AllowZeroSF ));
    }

    ANA_CHECK( m_muTrigSF_tool->setProperty("MuonQuality", m_WorkingPointReco ));
    ANA_CHECK( m_muTrigSF_tool->initialize());
  }

  std::string token;
  std::istringstream ss(m_MuTrigLegs);
  while ( std::getline(ss, token, ',') ) {
    size_t pos = token.find(":");
    m_SingleMuTriggerMap[token.substr(0,pos)] = token.substr(pos+1);
  }

  // Remember base output syst. names container
  m_outputSystNamesTrigBase = m_outputSystNamesTrig;
  // Add the chosen WP to the string labelling the output syst. names container
  m_outputSystNamesTrig = m_outputSystNamesTrig + "_Reco" + m_WorkingPointReco;

  CP::SystematicSet affectSystsTrig = m_muTrigSF_tool->affectingSystematics();
  for ( const auto& syst_it : affectSystsTrig ) { ANA_MSG_DEBUG("MuonEfficiencyScaleFactors tool can be affected by trigger efficiency systematic: " << syst_it.name()); }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsTrig = m_muTrigSF_tool->recommendedSystematics();
  m_systListTrig = HelperFunctions::getListofSystematics( recSystsTrig, m_systNameTrig, m_systValTrig, msg() );

  ANA_MSG_INFO("Will be using MuonEfficiencyScaleFactors tool trigger efficiency systematic:");
  for ( const auto& syst_it : m_systListTrig ) {
    if ( m_systNameTrig.empty() ) {
  	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
  	break;
    }
    ANA_MSG_INFO("\t " << syst_it.name());
  }


  // 4.
  // initialize the CP::MuonEfficiencyScaleFactors Tool for track-to-vertex association (TTVA) SF
  //

  m_TTVAEffSF_tool_name = "MuonEfficiencyScaleFactors_effSF_" + m_WorkingPointTTVA;

  ANA_MSG_INFO( " Initialising CP::MuonEfficiencyScaleFactors for TTVA efficiency SF..." );

  ANA_CHECK( checkToolStore<CP::MuonEfficiencyScaleFactors>(m_TTVAEffSF_tool_name));
  if ( asg::ToolStore::contains<CP::MuonEfficiencyScaleFactors>(m_TTVAEffSF_tool_name) ) {
    m_muTTVASF_tool = asg::ToolStore::get<CP::MuonEfficiencyScaleFactors>( m_TTVAEffSF_tool_name );
  } else {
    m_muTTVASF_tool = new CP::MuonEfficiencyScaleFactors( m_TTVAEffSF_tool_name );
    ANA_CHECK( m_muTTVASF_tool->setProperty("WorkingPoint", m_WorkingPointTTVA ));
    ANA_CHECK( m_muTTVASF_tool->setProperty("CalibrationRelease", m_calibRelease ));
    ANA_CHECK( m_muTTVASF_tool->initialize());

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesTTVA = m_outputSystNamesTTVA + "_" + m_WorkingPointTTVA;

    CP::SystematicSet affectSystsTTVA = m_muTTVASF_tool->affectingSystematics();
    for ( const auto& syst_it : affectSystsTTVA ) { ANA_MSG_DEBUG("MuonEfficiencyScaleFactors tool can be affected by TTVA efficiency systematic: " << syst_it.name()); }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsTTVA = m_muTTVASF_tool->recommendedSystematics();
    m_systListTTVA = HelperFunctions::getListofSystematics( recSystsTTVA, m_systNameTTVA, m_systValTTVA, msg() );

    ANA_MSG_INFO("Will be using MuonEfficiencyScaleFactors tool TTVA efficiency systematic:");
    for ( const auto& syst_it : m_systListTTVA ) {
      if ( m_systNameTTVA.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  }

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systListReco, m_outputSystNamesReco, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListIso, m_outputSystNamesIso, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListTrig, m_outputSystNamesTrig, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListTTVA, m_outputSystNamesTTVA, fileMD);
  }

  // *********************************************************************************

  ANA_MSG_INFO( "MuonEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  m_numEvent++;

  if ( !isMC() ) {
    if ( m_numEvent == 1 ) { ANA_MSG_INFO( "Sample is Data! Do not apply any Muon Efficiency correction... "); }
    return EL::StatusCode::SUCCESS;
  }

  ANA_MSG_DEBUG( "Applying Muon Efficiency corrections... ");


  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // if m_inputSystNamesMuons = "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  std::vector<std::string>* systNames_ptr(nullptr);
  if ( !m_inputSystNamesMuons.empty() ) ANA_CHECK( HelperFunctions::retrieve(systNames_ptr, m_inputSystNamesMuons, 0, m_store, msg()) );

  std::vector<std::string> systNames{""};
  if (systNames_ptr) systNames = *systNames_ptr;

  // Declare a write status set to true
  // For the systematically varied input containers, we won't store again the vector with efficiency systs in TStore ( it will be always the same!)
  //
  bool writeSystNames(true);

  // loop over systematic sets available
  for ( auto systName : systNames ) {
    const xAOD::MuonContainer* inputMuons(nullptr);

    // some systematics might have rejected the event
    if ( m_store->contains<xAOD::MuonContainer>( m_inContainerName+systName ) ) {
      // retrieve input muons
      ANA_CHECK( HelperFunctions::retrieve(inputMuons, m_inContainerName+systName, m_event, m_store, msg()) );

      ANA_MSG_DEBUG( "Number of muons: " << static_cast<int>(inputMuons->size()) );
      ANA_MSG_DEBUG( "Input syst: " << systName );
      unsigned int idx(0);
      for ( auto mu : *(inputMuons) ) {
        ANA_MSG_DEBUG( "Input muon " << idx << ", pt = " << mu->pt() * 1e-3 << " GeV" );
        ++idx;
      }

      // decorate muons w/ SF - there will be a decoration w/ different name for each syst!
      this->executeSF( eventInfo, inputMuons, systName.empty(), writeSystNames );

      writeSystNames = false;

    } // check existence of container

  } // close loop on systematic sets available from upstream algo

  // look what we have in TStore
  //
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  return EL::StatusCode::SUCCESS;

}


EL::StatusCode MuonEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG( "Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: finalize ()
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


EL::StatusCode MuonEfficiencyCorrector :: histFinalize ()
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

EL::StatusCode MuonEfficiencyCorrector :: executeSF ( const xAOD::EventInfo* eventInfo, const xAOD::MuonContainer* inputMuons, bool nominal, bool writeSystNames )
{

  //
  // In the following, every muon gets decorated with 2 vector<double>'s (for reco/iso efficiency SFs),
  // and the event w/ 1 vector<double> (for trigger efficiency SFs)
  // Each vector contains the SFs, one SF for each syst (first component of each vector will be the nominal SF).
  //
  // Additionally, we create these vector<string> with the SF syst names, so that we know which component corresponds to.
  // ( there's a 1:1 correspondence with the vector<double> defined above )
  //
  // These vector<string> are eventually stored in TStore
  //

  // 1.
  // Reco efficiency SFs - this is a per-MUON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  std::unique_ptr< std::vector< std::string > > sysVariationNamesReco = nullptr;

  // Do it only if a tool with *this* name hasn't already been used
  //
  if ( !isToolAlreadyUsed(m_recoEffSF_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesReco = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    for ( const auto& syst_it : m_systListReco ) {
      if ( !syst_it.name().empty() && !nominal ) continue;

      // Create the name of the SF weight to be recorded
      std::string sfName = "MuRecoEff_SF_syst_Reco" + m_WorkingPointReco;

      ANA_MSG_DEBUG( "Muon reco efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
      if( writeSystNames ) sysVariationNamesReco->push_back(syst_it.name());

      // apply syst
      //
      if ( m_muRecoSF_tool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
        ANA_MSG_ERROR("Failed to configure MuonEfficiencyScaleFactors for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

      // and now apply reco efficiency SF!
      //
      unsigned int idx(0);
      for ( auto mu_itr : *(inputMuons) ) {

    	 ANA_MSG_DEBUG( "Applying reco efficiency SF" );

    	 // a)
    	 // decorate directly the muon with reco efficiency (useful at all?), and the corresponding SF
    	 //
    	 //if ( m_muRecoSF_tool->applyMCEfficiency( *mu_itr ) != CP::CorrectionCode::Ok ) {
    	 //  ANA_MSG_WARNING( "Problem in applyMCEfficiency for Reco");
    	 //}
    	 //if ( m_muRecoSF_tool->applyEfficiencyScaleFactor( *mu_itr ) != CP::CorrectionCode::Ok ) {
    	 //  ANA_MSG_WARNING( "Problem in applyEfficiencyScaleFactor for Reco");
    	 //}

    	 // b)
    	 // obtain reco efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* muon)
    	 //

    	 SG::AuxElement::Decorator< std::vector<float> > sfVecReco( sfName );
    	 if ( !sfVecReco.isAvailable( *mu_itr ) ) {
  	   sfVecReco( *mu_itr ) = std::vector<float>();
    	 }

    	 float recoEffSF(1.0);
    	 if ( m_muRecoSF_tool->getEfficiencyScaleFactor( *mu_itr, recoEffSF ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
    	   recoEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecReco( *mu_itr ).push_back( recoEffSF );

         // reco sys names are saved in a vector. Entries positions are preserved!
         //
    	 SG::AuxElement::Decorator< std::vector<std::string> > sfVecReco_sysNames( m_outputSystNamesReco + "_sysNames" );
    	 if ( !sfVecReco_sysNames.isAvailable( *mu_itr ) ) {
  	   sfVecReco_sysNames( *mu_itr ) = std::vector<std::string>();
         }
    	 sfVecReco_sysNames( *mu_itr ).push_back( syst_it.name().c_str() );

         ANA_MSG_DEBUG( "===>>>");
         ANA_MSG_DEBUG( "Muon " << idx << ", pt = " << mu_itr->pt()*1e-3 << " GeV" );
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

  }

  // 2.
  // Isolation efficiency SFs - this is a per-MUON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  std::unique_ptr< std::vector< std::string > > sysVariationNamesIso = nullptr;

  // Do it only if a tool with *this* name hasn't already been used
  //
  if ( !isToolAlreadyUsed(m_isoEffSF_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesIso = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    for ( const auto& syst_it : m_systListIso ) {
      if ( !syst_it.name().empty() && !nominal ) continue;

      // Create the name of the SF weight to be recorded
      std::string sfName = "MuIsoEff_SF_syst_Iso" + m_WorkingPointIso;

      ANA_MSG_DEBUG( "Muon iso efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
      if ( writeSystNames ) sysVariationNamesIso->push_back(syst_it.name());

      // apply syst
      //
      if ( m_muIsoSF_tool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ANA_MSG_ERROR("Failed to configure MuonEfficiencyScaleFactors for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

      // and now apply Iso efficiency SF!
      //
      unsigned int idx(0);
      for ( auto mu_itr : *(inputMuons) ) {

    	 ANA_MSG_DEBUG( "Applying iso efficiency SF" );

    	 // a)
    	 // decorate directly the muon with iso efficiency (useful at all?), and the corresponding SF
    	 //
    	 //if ( m_muIsoSF_tool->applyMCEfficiency( *mu_itr ) != CP::CorrectionCode::Ok ) {
    	 //  ANA_MSG_WARNING( "Problem in applyMCEfficiency for Iso");
    	 //}
    	 //if ( m_muIsoSF_tool->applyEfficiencyScaleFactor( *mu_itr ) != CP::CorrectionCode::Ok ) {
    	 //  ANA_MSG_WARNING( "Problem in applyEfficiencyScaleFactor for Iso");
    	 //}

    	 // b)
    	 // obtain iso efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* muon)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecIso( sfName );
    	 if ( !sfVecIso.isAvailable( *mu_itr ) ) {
  	   sfVecIso( *mu_itr ) = std::vector<float>();
    	 }

    	 float IsoEffSF(1.0);
    	 if ( m_muIsoSF_tool->getEfficiencyScaleFactor( *mu_itr, IsoEffSF ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	   IsoEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecIso( *mu_itr ).push_back(IsoEffSF);

         ANA_MSG_DEBUG( "===>>>");
         ANA_MSG_DEBUG( "Muon " << idx << ", pt = " << mu_itr->pt()*1e-3 << " GeV " );
         ANA_MSG_DEBUG( "Isolation SF decoration: " << sfName );
         ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
         ANA_MSG_DEBUG( "Isolation SF:");
         ANA_MSG_DEBUG( "\t " << IsoEffSF << " (from getEfficiencyScaleFactor())");
         ANA_MSG_DEBUG( "--------------------------------------");

    	 ++idx;

      } // close muon loop

    }  // close loop on isolation efficiency SF systematics

    // Add list of systematics names to TStore
    // We only do this once per event if the list does not exist yet
    if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesIso ) ) {
      ANA_CHECK( m_store->record( std::move(sysVariationNamesIso), m_outputSystNamesIso ));
    }

  }

  // 3.
  // Trigger efficiency SF - this is in principle given by the MCP tool as a per-EVENT weight
  //
  // To allow more freedom to the user, we calculate it as a per-muon weight with a trick
  // We store also the MC efficiency per-muon

  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //
  // NB: calculation of the event SF is up to the analyzer


  // Do it only if a tool with *this* name hasn't already been used
  //
  if ( !isToolAlreadyUsed(m_trigEffSF_tool_name) ) {

    static const SG::AuxElement::ConstAccessor<unsigned int> acc_rnd("RandomRunNumber");
    unsigned int run=0;
    if (acc_rnd.isAvailable(*eventInfo)){
      run = acc_rnd(*eventInfo);
    }

    for (auto const& trig : m_SingleMuTriggerMap) {

      auto trig_it = trig.second;
      if (trig.first.find("2015")!=std::string::npos && run>284484) continue;
      else if ((trig.first.find("2016")!=std::string::npos || trig.first.find("2017")!=std::string::npos) && run<=284484) continue;

      std::unique_ptr< std::vector< std::string > > sysVariationNamesTrig = nullptr;
      if ( writeSystNames ) sysVariationNamesTrig = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);
      // this is used to put the list of sys strings in the store.
      // The original string needs to be updated with the name of
      // the trigger for every item in the trigger loop.
      //
      //std::string m_fullname_outputSystNamesTrig;

      std::string sf_string = m_outputSystNamesTrig;
      std::string insfstr   = m_outputSystNamesTrigBase + "_";
      std::string outsfstr  = m_outputSystNamesTrigBase + "_" + trig_it + "_";

      for(std::string::size_type i = 0; (i = sf_string.find(insfstr, i)) != std::string::npos;) {
        sf_string.replace(i, insfstr.length(), outsfstr);
        i += outsfstr.length();
      }

      for ( const auto& syst_it : m_systListTrig ) {
        if ( !syst_it.name().empty() && !nominal ) continue;

        // Create the name of the SF weight to be recorded
        std::string sfName = "MuTrigEff_SF_syst_" + trig_it + "_Reco" + m_WorkingPointReco;
        std::string effName = "MuTrigMCEff_syst_" + trig_it + "_Reco" + m_WorkingPointReco;

        ANA_MSG_DEBUG( "Trigger efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
        if ( writeSystNames ) sysVariationNamesTrig->push_back(syst_it.name());

        // apply syst
        //
        if ( m_muTrigSF_tool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
          ANA_MSG_ERROR( "Failed to configure MuonTriggerScaleFactors for trigger " << trig_it << " systematic " << syst_it.name());
          return EL::StatusCode::FAILURE;
        }
        ANA_MSG_DEBUG( "Successfully applied systematic " << syst_it.name() << " for trigger " << trig_it);

        // and now apply trigger efficiency SF!
        //
        unsigned int idx(0);
        for ( auto mu_itr : *(inputMuons) ) {

           ANA_MSG_DEBUG( "Applying trigger efficiency SF and MC efficiency" );

           // Pass a container with only the muon in question to the tool
           // (use a view container to be light weight)
           //
           ConstDataVector<xAOD::MuonContainer> mySingleMuonCont(SG::VIEW_ELEMENTS);
           mySingleMuonCont.push_back( mu_itr );

           //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* muon)
           //
           SG::AuxElement::Decorator< std::vector<float> > effMC( effName );
           if ( !effMC.isAvailable( *mu_itr ) ) {
             effMC( *mu_itr ) = std::vector<float>();
           }

           SG::AuxElement::Decorator< std::vector<float> > sfVecTrig( sfName );
           if ( !sfVecTrig.isAvailable( *mu_itr ) ) {
             sfVecTrig( *mu_itr ) = std::vector<float>();
           }

           // ugly ass hardcoding
           //
           std::string full_scan_chain = "HLT_mu8noL1";

           double triggerMCEff(0.0); // tool wants a double
           if ( m_muTrigSF_tool->getTriggerEfficiency( *mu_itr, triggerMCEff, trig_it, !isMC() ) != CP::CorrectionCode::Ok ) {
             ANA_MSG_WARNING( "Problem in getTriggerEfficiency - single muon trigger(s)");
             triggerMCEff = 0.0;
           }
           // Add it to decoration vector
           //
           effMC( *mu_itr ).push_back(triggerMCEff);

           // retrieve MC efficiency for full scan chain
           //
           double triggerDataEff(0.0); // tool wants a double
           if ( trig_it == full_scan_chain ) {
             if ( m_muTrigSF_tool->getTriggerEfficiency( *mu_itr, triggerDataEff, trig_it, isMC() ) != CP::CorrectionCode::Ok ) {
               ANA_MSG_WARNING( "Problem in getTriggerEfficiency - single muon trigger(s)");
               triggerDataEff = 0.0;
             }
           }

           double triggerEffSF(1.0); // tool wants a double
           if ( trig_it != full_scan_chain ) {
             if ( m_muTrigSF_tool->getTriggerScaleFactor( *mySingleMuonCont.asDataVector(), triggerEffSF, trig_it ) != CP::CorrectionCode::Ok ) {
               ANA_MSG_WARNING( "Problem in getTriggerScaleFactor - single muon trigger(s)");
               triggerEffSF = 1.0;
             }
           } else {
             if ( triggerMCEff > 0.0 ) {
               triggerEffSF = triggerDataEff / triggerMCEff;
             }
           }

           // Add it to decoration vector
           //
           sfVecTrig( *mu_itr ).push_back(triggerEffSF);


           ANA_MSG_DEBUG( "===>>>");
           ANA_MSG_DEBUG( "Muon " << idx << ", pt = " << mu_itr->pt()*1e-3 << " GeV " );
           ANA_MSG_DEBUG( "Trigger efficiency SF decoration: " << sfName );
           ANA_MSG_DEBUG( "Trigger MC efficiency decoration: " << effName );
           ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
           ANA_MSG_DEBUG( "Trigger efficiency SF:");
           ANA_MSG_DEBUG( "\t " << triggerEffSF << " (from getTriggerScaleFactor())" );
           ANA_MSG_DEBUG( "Trigger MC efficiency:");
           ANA_MSG_DEBUG( "\t " << triggerDataEff << " (from getTriggerEfficiency())" );
           ANA_MSG_DEBUG( "--------------------------------------");

           ++idx;

        } // close muon loop
      }  // close loop on trigger efficiency SF systematics

      // Add list of systematics names to TStore
      // We only do this once per event if the list does not exist yet
      if ( writeSystNames && !m_store->contains<std::vector<std::string>>( sf_string ) ) {
        ANA_CHECK( m_store->record( std::move(sysVariationNamesTrig), sf_string ));
      }
    } // close  trigger loop

  }

  // 4.
  // TTVA efficiency SFs - this is a per-MUON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  std::unique_ptr< std::vector< std::string > > sysVariationNamesTTVA = nullptr;

  // Do it only if a tool with *this* name hasn't already been used
  //
  if ( !isToolAlreadyUsed(m_TTVAEffSF_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesTTVA = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    for ( const auto& syst_it : m_systListTTVA ) {
      if ( !syst_it.name().empty() && !nominal ) continue;

      // Create the name of the SF weight to be recorded
      std::string sfName = "MuTTVAEff_SF_syst_" + m_WorkingPointTTVA;

      ANA_MSG_DEBUG( "Muon iso efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name() );
      if ( writeSystNames ) sysVariationNamesTTVA->push_back(syst_it.name());

      // apply syst
      //
      //
      // TEMP! Commenting this out b/c stupid AnaToolHandle does not work as it should...
      //
      //if ( m_muTTVASF_tool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      if ( m_muTTVASF_tool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
       	ANA_MSG_ERROR("Failed to configure MuonEfficiencyScaleFactors for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematic: " << syst_it.name());

      // and now apply TTVA efficiency SF!
      //
      unsigned int idx(0);
      for ( auto mu_itr : *(inputMuons) ) {

    	 ANA_MSG_DEBUG( "Applying TTVA efficiency SF" );

    	 // a)
    	 // decorate directly the muon with TTVA efficiency (useful at all?), and the corresponding SF
    	 //
    	 //if ( m_muTTVASF_tool->applyMCEfficiency( *mu_itr ) != CP::CorrectionCode::Ok ) {
    	 //  ANA_MSG_WARNING( "Problem in applyMCEfficiency for TTVA");
    	 //}
    	 //if ( m_muTTVASF_tool->applyEfficiencyScaleFactor( *mu_itr ) != CP::CorrectionCode::Ok ) {
    	 //  ANA_MSG_WARNING( "Problem in applyEfficiencyScaleFactor for TTVA");
    	 //}

    	 // b)
    	 // obtain TTVA efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* muon)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecTTVA( sfName );
    	 if ( !sfVecTTVA.isAvailable( *mu_itr ) ) {
  	   sfVecTTVA( *mu_itr ) = std::vector<float>();
    	 }

    	 float TTVAEffSF(1.0);
    	 if ( m_muTTVASF_tool->getEfficiencyScaleFactor( *mu_itr, TTVAEffSF ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in getEfficiencyScaleFactor");
  	   TTVAEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecTTVA( *mu_itr ).push_back(TTVAEffSF);

         ANA_MSG_DEBUG( "===>>>");
         ANA_MSG_DEBUG( "Muon " << idx << ", pt = " << mu_itr->pt()*1e-3 << " GeV " );
         ANA_MSG_DEBUG( "TTVA SF decoration: " << sfName );
         ANA_MSG_DEBUG( "Systematic: " << syst_it.name());
         ANA_MSG_DEBUG( "TTVA SF:");
         ANA_MSG_DEBUG( "\t " << TTVAEffSF << " (from getEfficiencyScaleFactor())" );
         ANA_MSG_DEBUG( "--------------------------------------");

    	 ++idx;

      } // close muon loop

    }  // close loop on TTVA efficiency SF systematics

    // Add list of systematics names to TStore
    // We only do this once per event if the list does not exist yet
    if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesTTVA ) ) {
      ANA_CHECK( m_store->record( std::move(sysVariationNamesTTVA), m_outputSystNamesTTVA ));
    }
  }

  return EL::StatusCode::SUCCESS;
}
