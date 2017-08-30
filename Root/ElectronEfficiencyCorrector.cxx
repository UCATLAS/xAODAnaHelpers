// c++ include(s):
#include <iostream>
#include <memory>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/ElectronAuxContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/ElectronEfficiencyCorrector.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronEfficiencyCorrector)


ElectronEfficiencyCorrector :: ElectronEfficiencyCorrector () :
    Algorithm("ElectronEfficiencyCorrector")
{
}


EL::StatusCode ElectronEfficiencyCorrector :: setupJob (EL::Job& job)
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
  xAOD::Init( "ElectronEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_INFO( "Initializing ElectronEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  m_numEvent      = 0;
  m_numObject     = 0;


  // *******************************************************

  int sim_flav(1); // default for FullSim
  if ( m_isMC ) {
    const std::string stringMeta = wk()->metaData()->castString("SimulationFlavour");
    if ( m_setAFII || ( !stringMeta.empty() && ( stringMeta.find("AFII") != std::string::npos ) ) ) {
      ANA_MSG_INFO( "Setting simulation flavour to AFII");
      sim_flav = 3;
    }
    else if ( !m_setAFII && stringMeta.empty() ) {
      ANA_MSG_WARNING( "No meta-data string found. Simulation flavour will be set to FullSim. Care if you are running on Fast-Sim MC.");
    }
  }

  // 1.
  // initialize the AsgElectronEfficiencyCorrectionTool for PID efficiency SF
  //
  if ( !m_corrFileNamePID.empty() ) {

    m_PID_WP = HelperFunctions::parse_wp( "ID", m_corrFileNamePID, msg() );

    if ( m_PID_WP.empty() ) {
      ANA_MSG_ERROR( "ID working point for electronID SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    ANA_MSG_INFO("Electron ID wp: " << m_PID_WP);

    m_pidEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_PID_" + m_PID_WP;

    ANA_CHECK( checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_pidEffSF_tool_name));

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_pidEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_PID = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_pidEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_PID = new AsgElectronEfficiencyCorrectionTool(m_pidEffSF_tool_name);
      m_asgElEffCorrTool_elSF_PID->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesPID{ m_corrFileNamePID } ; // initialise vector w/ all the files containing corrections
      ANA_CHECK( m_asgElEffCorrTool_elSF_PID->setProperty("CorrectionFileNameList",inputFilesPID));
      ANA_CHECK( m_asgElEffCorrTool_elSF_PID->setProperty("ForceDataType",sim_flav));
      ANA_CHECK( m_asgElEffCorrTool_elSF_PID->setProperty("CorrelationModel",m_correlationModel));
      ANA_CHECK( m_asgElEffCorrTool_elSF_PID->initialize());
    }

    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsPID = m_asgElEffCorrTool_elSF_PID->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsPID ) { ANA_MSG_DEBUG("AsgElectronEfficiencyCorrectionTool can be affected by PID efficiency systematic: " << syst_it.name()); }

    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsPID = m_asgElEffCorrTool_elSF_PID->recommendedSystematics();
    m_systListPID = HelperFunctions::getListofSystematics( recSystsPID, m_systNamePID, m_systValPID, msg() );

    ANA_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool PID efficiency systematic:");
    for ( const auto& syst_it : m_systListPID ) {
      if ( m_systNamePID.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesPID = m_outputSystNamesPID + "_" + m_PID_WP;

  }

  // 2.
  // initialize the AsgElectronEfficiencyCorrectionTool for isolation efficiency SF
  //
  if ( !m_corrFileNameIso.empty() ) {

    m_Iso_WP    = HelperFunctions::parse_wp( "ISO", m_corrFileNameIso, msg() );
    m_IsoPID_WP = HelperFunctions::parse_wp( "ID", m_corrFileNameIso, msg() );

    if ( m_Iso_WP.empty() ) {
      ANA_MSG_ERROR("ISO working point for isolation SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }
    if ( m_IsoPID_WP.empty() ) {
      ANA_MSG_ERROR( "ID working point for isolation SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    ANA_MSG_INFO("ISOLATION wp: " << m_Iso_WP << "\n ID wp (for isolation): " << m_IsoPID_WP);

    m_IsoEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_Iso_" + m_IsoPID_WP + "_isol" + m_Iso_WP;

    ANA_CHECK( checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_IsoEffSF_tool_name));

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_IsoEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_Iso = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_IsoEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_Iso = new AsgElectronEfficiencyCorrectionTool(m_IsoEffSF_tool_name);
      m_asgElEffCorrTool_elSF_Iso->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesIso{ m_corrFileNameIso } ; // initialise vector w/ all the files containing corrections
      ANA_CHECK( m_asgElEffCorrTool_elSF_Iso->setProperty("CorrectionFileNameList",inputFilesIso));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Iso->setProperty("ForceDataType",sim_flav));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Iso->setProperty("CorrelationModel",m_correlationModel));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Iso->initialize());
    }


    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsIso = m_asgElEffCorrTool_elSF_Iso->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsIso ) { ANA_MSG_DEBUG("AsgElectronEfficiencyCorrectionTool can be affected by Iso efficiency systematic: " << syst_it.name()); }

    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsIso = m_asgElEffCorrTool_elSF_Iso->recommendedSystematics();
    m_systListIso = HelperFunctions::getListofSystematics( recSystsIso, m_systNameIso, m_systValIso, msg() );

    ANA_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool Iso efficiency systematic:");
    for ( const auto& syst_it : m_systListIso ) {
      if ( m_systNameIso.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesIso = m_outputSystNamesIso + "_" + m_IsoPID_WP + "_isol" + m_Iso_WP;

  }

  // 3.
  // initialize the AsgElectronEfficiencyCorrectionTool for Reco Efficiency SF
  //
  if ( !m_corrFileNameReco.empty() ) {

    m_RecoEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_Reco";

    ANA_CHECK( checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_RecoEffSF_tool_name));

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_RecoEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_Reco = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_RecoEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_Reco = new AsgElectronEfficiencyCorrectionTool(m_RecoEffSF_tool_name);
      m_asgElEffCorrTool_elSF_Reco->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesReco{ m_corrFileNameReco } ; // initialise vector w/ all the files containing corrections
      ANA_CHECK( m_asgElEffCorrTool_elSF_Reco->setProperty("CorrectionFileNameList",inputFilesReco));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Reco->setProperty("ForceDataType",sim_flav));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Reco->setProperty("CorrelationModel",m_correlationModel));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Reco->initialize());
    }


    // Get a list of affecting systematics
    //
    CP::SystematicSet affectSystsReco = m_asgElEffCorrTool_elSF_Reco->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsReco ) { ANA_MSG_DEBUG("AsgElectronEfficiencyCorrectionTool can be affected by reco efficiency systematic: " << syst_it.name()); }

    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsReco = m_asgElEffCorrTool_elSF_Reco->recommendedSystematics();
    m_systListReco = HelperFunctions::getListofSystematics( recSystsReco, m_systNameReco, m_systValReco, msg() );

    ANA_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool reco efficiency systematic:");
    for ( const auto& syst_it : m_systListReco ) {
      if ( m_systNameReco.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  }

  // 4.
  // Initialise the AsgElectronEfficiencyCorrectionTool for Trigger Efficiency SF
  //
  if ( !m_corrFileNameTrig.empty() ) {

    m_WorkingPointIsoTrig = HelperFunctions::parse_wp( "ISO", m_corrFileNameTrig, msg() );
    m_WorkingPointIDTrig  = HelperFunctions::parse_wp( "ID", m_corrFileNameTrig, msg() );
    m_WorkingPointTrigTrig = HelperFunctions::parse_wp( "TRIG", m_corrFileNameTrig, msg() );

    if ( m_WorkingPointIDTrig.empty() ) {
      ANA_MSG_ERROR( "ID working point for trigger SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    ANA_MSG_INFO("Trigger ISOLATION wp: " << m_WorkingPointIsoTrig << "\n Trigger ID wp: " << m_WorkingPointIDTrig);

    m_TrigEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_Trig_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      m_TrigEffSF_tool_name += ( "_isol" + m_WorkingPointIsoTrig );
    }

    ANA_CHECK( checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_TrigEffSF_tool_name));

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_TrigEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_Trig = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_TrigEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_Trig = new AsgElectronEfficiencyCorrectionTool(m_TrigEffSF_tool_name);
      m_asgElEffCorrTool_elSF_Trig->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesTrig{ m_corrFileNameTrig } ; // initialise vector w/ all the files containing corrections
      ANA_CHECK( m_asgElEffCorrTool_elSF_Trig->setProperty("CorrectionFileNameList",inputFilesTrig));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Trig->setProperty("ForceDataType",sim_flav));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Trig->setProperty("CorrelationModel",m_correlationModel));
      ANA_CHECK( m_asgElEffCorrTool_elSF_Trig->initialize());
    }


    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsTrig = m_asgElEffCorrTool_elSF_Trig->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsTrig ) { ANA_MSG_DEBUG("AsgElectronEfficiencyCorrectionTool can be affected by Trig efficiency SF systematic: " << syst_it.name()); }

    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsTrig = m_asgElEffCorrTool_elSF_Trig->recommendedSystematics();
    m_systListTrig = HelperFunctions::getListofSystematics( recSystsTrig, m_systNameTrig, m_systValTrig, msg() );

    ANA_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool Trig efficiency SF systematic:");
    for ( const auto& syst_it : m_systListTrig ) {
      if ( m_systNameTrig.empty() ) {
    	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
    	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesTrig = m_outputSystNamesTrig + "_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      m_outputSystNamesTrig += ( "_isol" + m_WorkingPointIsoTrig );
    }

  }

  // 5.
  // Initialise the AsgElectronEfficiencyCorrectionTool for Trigger Efficiency (for MC)
  //
  if ( !m_corrFileNameTrigMCEff.empty() ) {

    m_TrigMCEff_tool_name = "ElectronEfficiencyCorrectionTool_effSF_TrigMCEff_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      m_TrigMCEff_tool_name += ( "_isol" + m_WorkingPointIsoTrig );
    }

    ANA_CHECK( checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_TrigMCEff_tool_name));

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_TrigMCEff_tool_name) ) {
      m_asgElEffCorrTool_elSF_TrigMCEff= asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_TrigMCEff_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_TrigMCEff = new AsgElectronEfficiencyCorrectionTool(m_TrigMCEff_tool_name);
      m_asgElEffCorrTool_elSF_TrigMCEff->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesTrigMCEff{ m_corrFileNameTrigMCEff } ; // initialise vector w/ all the files containing corrections
      ANA_CHECK( m_asgElEffCorrTool_elSF_TrigMCEff->setProperty("CorrectionFileNameList",inputFilesTrigMCEff));
      ANA_CHECK( m_asgElEffCorrTool_elSF_TrigMCEff->setProperty("ForceDataType",sim_flav));
      ANA_CHECK( m_asgElEffCorrTool_elSF_TrigMCEff->setProperty("CorrelationModel",m_correlationModel));
      ANA_CHECK( m_asgElEffCorrTool_elSF_TrigMCEff->initialize());
    }


    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsTrigMCEff = m_asgElEffCorrTool_elSF_TrigMCEff->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsTrigMCEff ) { ANA_MSG_DEBUG("AsgElectronEfficiencyCorrectionTool can be affected by TrigMCEff efficiency systematic: " << syst_it.name()); }

    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsTrigMCEff = m_asgElEffCorrTool_elSF_TrigMCEff->recommendedSystematics();
    m_systListTrigMCEff = HelperFunctions::getListofSystematics( recSystsTrigMCEff, m_systNameTrigMCEff, m_systValTrigMCEff, msg() );

    ANA_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool TrigMCEff efficiency systematic:");
    for ( const auto& syst_it : m_systListTrigMCEff ) {
      if ( m_systNameTrigMCEff.empty() ) {
  	ANA_MSG_INFO("\t Running w/ nominal configuration only!");
  	break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesTrigMCEff = m_outputSystNamesTrigMCEff + "_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      m_outputSystNamesTrigMCEff += ( "_isol" + m_WorkingPointIsoTrig );
    }

  }

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systListPID, m_outputSystNamesPID, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListIso, m_outputSystNamesIso, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListReco, m_outputSystNamesReco, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListTrig, m_outputSystNamesTrig, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListTrigMCEff, m_outputSystNamesTrigMCEff, fileMD);
  }

  // *********************************************************************************

  ANA_MSG_INFO( "ElectronEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  m_numEvent++;

  if ( !m_isMC ) {
    if ( m_numEvent == 1 ) { ANA_MSG_INFO( "Sample is Data! Do not apply any Electron Efficiency correction... "); }
    return EL::StatusCode::SUCCESS;
  }

  ANA_MSG_DEBUG( "Applying Electron Efficiency Correction... ");
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // if m_inputSystNamesElectrons = "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  std::vector<std::string>* systNames_ptr(nullptr);
  if ( !m_inputSystNamesElectrons.empty() ) ANA_CHECK( HelperFunctions::retrieve(systNames_ptr, m_inputSystNamesElectrons, 0, m_store, msg()) );

  std::vector<std::string> systNames{""};
  if(systNames_ptr) systNames = *systNames_ptr;

  // Declare a write status set to true
  // For the systematically varied input containers, we won't store again the vector with efficiency systs in TStore ( it will be always the same!)
  bool writeSystNames(true);

  // loop over systematic sets available
  for ( auto systName : systNames ) {

    const xAOD::ElectronContainer* inputElectrons(nullptr);

    // some systematics might have rejected the event
    if ( m_store->contains<xAOD::ElectronContainer>( m_inContainerName+systName ) ) {

      // retrieve input electrons
      ANA_CHECK( HelperFunctions::retrieve(inputElectrons, m_inContainerName+systName, m_event, m_store, msg()) );

      ANA_MSG_DEBUG( "Number of electrons: " << static_cast<int>(inputElectrons->size()) );
      ANA_MSG_DEBUG( "Input syst: " << systName );
      unsigned int idx(0);
      for ( auto el : *(inputElectrons) ) {
        ANA_MSG_DEBUG( "Input electron " << idx << ", pt = " << el->pt() * 1e-3 << " GeV " );
        ++idx;
      }

      // decorate electrons w/ SF - there will be a decoration w/ different name for each syst!
      this->executeSF( inputElectrons, systName.empty(), writeSystNames );

      writeSystNames = false;

    } // check existence of container

  } // close loop on systematic sets available from upstream algo

  // look what we have in TStore
  //
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG( "Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: finalize ()
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



EL::StatusCode ElectronEfficiencyCorrector :: histFinalize ()
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

EL::StatusCode ElectronEfficiencyCorrector :: executeSF ( const xAOD::ElectronContainer* inputElectrons, bool nominal, bool writeSystNames )
{

  // In the following, every electron gets decorated with several vector<double>'s (for various SFs),
  //
  // Each vector contains the SFs, one SF for each syst (first component of each vector will be the nominal SF).
  //
  // Additionally, we create these vector<string> with the SF syst names, so that we know which component corresponds to.
  // ( there's a 1:1 correspondence with the vector<double>'s defined above )
  //
  // These vector<string> are eventually stored in TStore
  //
  std::unique_ptr< std::vector< std::string > > sysVariationNamesPID       = nullptr;
  std::unique_ptr< std::vector< std::string > > sysVariationNamesReco      = nullptr;
  std::unique_ptr< std::vector< std::string > > sysVariationNamesIso       = nullptr;
  std::unique_ptr< std::vector< std::string > > sysVariationNamesTrig      = nullptr;
  std::unique_ptr< std::vector< std::string > > sysVariationNamesTrigMCEff = nullptr;

  // 1.
  // PID efficiency SFs - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Do it only if a tool with *this* name hasn't already been used, and has been previously initialised
  //
  if ( !m_corrFileNamePID.empty() && !isToolAlreadyUsed(m_pidEffSF_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesPID = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    // Create the names of the SF weights to be recorded
    std::string sfName = "ElPIDEff_SF_syst_" + m_PID_WP;

    for ( const auto& syst_it : m_systListPID ) {

      if ( !syst_it.name().empty() && !nominal ) continue;

      ANA_MSG_DEBUG("Electron PID efficiency sys name (to be recorded in xAOD::TStore) is: " << syst_it.name());
      if ( writeSystNames ) sysVariationNamesPID->push_back(syst_it.name());

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_PID->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ANA_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_PID for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_PID->appliedSystematics().name() );

      // and now apply PID efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 ANA_MSG_DEBUG( "Applying PID efficiency SF" );

    	 bool isBadElectron(false);

    	 //
    	 // obtain PID efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecPID ( sfName  );
    	 if ( !sfVecPID.isAvailable( *el_itr )  ) {
    	   sfVecPID ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info");
    	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )");
    	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance");
    	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF's for PID
    	 //
    	 double pidEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron &&  m_asgElEffCorrTool_elSF_PID->getEfficiencyScaleFactor( *el_itr, pidEffSF ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in PID getEfficiencyScaleFactor Tool");
  	   pidEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecPID( *el_itr ).push_back( pidEffSF );

       ANA_MSG_DEBUG( "===>>>");
       ANA_MSG_DEBUG( "Electron " << idx << ", pt = " << el_itr->pt()*1e-3 << " GeV ");
       ANA_MSG_DEBUG( "PID SF decoration: " << sfName );
       ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
       ANA_MSG_DEBUG( "PID efficiency SF:");
       ANA_MSG_DEBUG( "\t " << pidEffSF << " (from getEfficiencyScaleFactor())" );
       ANA_MSG_DEBUG( "--------------------------------------");

    	 ++idx;

      } // close electron loop

    }  // close loop on PID efficiency systematics

    // Add list of systematics names to TStore
    // We only do this once per event if the list does not exist yet
    if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesPID ) ) {
      ANA_CHECK( m_store->record( std::move(sysVariationNamesPID), m_outputSystNamesPID ));
    }

  }

  // 2.
  // Iso efficiency SFs - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Do it only if a tool with *this* name hasn't already been used, and has been previously initialised
  //
  if ( !m_corrFileNameIso.empty() && !isToolAlreadyUsed(m_IsoEffSF_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesIso = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    // Create the names of the SF weights to be recorded
    std::string sfName = "ElIsoEff_SF_syst_" + m_IsoPID_WP + "_isol" + m_Iso_WP;

    for ( const auto& syst_it : m_systListIso ) {

      if ( !syst_it.name().empty() && !nominal ) continue;

      ANA_MSG_DEBUG("Electron Iso efficiency sys name (to be recorded in xAOD::TStore) is: " << syst_it.name());
      if ( writeSystNames ) sysVariationNamesIso->push_back(syst_it.name());

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_Iso->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ANA_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_Iso for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_Iso->appliedSystematics().name() );

      // and now apply Iso efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 ANA_MSG_DEBUG( "Applying Iso efficiency SF" );

    	 bool isBadElectron(false);

    	 //
    	 // obtain Iso efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecIso ( sfName  );
    	 if ( !sfVecIso.isAvailable( *el_itr )  ) {
    	   sfVecIso ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info");
    	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )");
    	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance");
    	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF's for Iso
    	 //
    	 double IsoEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron &&  m_asgElEffCorrTool_elSF_Iso->getEfficiencyScaleFactor( *el_itr, IsoEffSF ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in Iso getEfficiencyScaleFactor Tool");
  	   IsoEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
       sfVecIso( *el_itr ).push_back( IsoEffSF );

       ANA_MSG_DEBUG( "===>>>");
       ANA_MSG_DEBUG( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
       ANA_MSG_DEBUG( "Iso SF decoration: " << sfName );
       ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
       ANA_MSG_DEBUG( "Iso efficiency SF:");
       ANA_MSG_DEBUG( "\t " << IsoEffSF << " (from getEfficiencyScaleFactor())" );
       ANA_MSG_DEBUG( "--------------------------------------");

    	 ++idx;

      } // close electron loop

    }  // close loop on Iso efficiency systematics

    // Add list of systematics names to TStore
    // We only do this once per event if the list does not exist yet
    if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesIso ) ) {
      ANA_CHECK( m_store->record( std::move(sysVariationNamesIso), m_outputSystNamesIso ));
    }

  }

  // 3.
  // Reco efficiency SFs - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Do it only if a tool with *this* name hasn't already been used, and has been previously initialised
  //
  if ( !m_corrFileNameReco.empty() && !isToolAlreadyUsed(m_RecoEffSF_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesReco = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    // Create the names of the SF weights to be recorded
    std::string sfName = "ElRecoEff_SF_syst";

    for ( const auto& syst_it : m_systListReco ) {

      if ( !syst_it.name().empty() && !nominal ) continue;

      ANA_MSG_DEBUG("Electron Reco efficiency sys name (to be recorded in xAOD::TStore) is: " << syst_it.name());
      if ( writeSystNames ) sysVariationNamesReco->push_back(syst_it.name());

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_Reco->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ANA_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_Reco for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_Reco->appliedSystematics().name() );

      // and now apply Reco efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 ANA_MSG_DEBUG( "Applying Reco efficiency SF" );

    	 bool isBadElectron(false);

    	 //
    	 // obtain Reco efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecReco ( sfName  );
    	 if ( !sfVecReco.isAvailable( *el_itr )  ) {
    	   sfVecReco ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info");
  	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )");
  	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance");
  	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF's for Reco
    	 //
    	 double recoEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron && m_asgElEffCorrTool_elSF_Reco->getEfficiencyScaleFactor( *el_itr, recoEffSF ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in Reco getEfficiencyScaleFactor Tool");
  	   recoEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
       sfVecReco( *el_itr ).push_back( recoEffSF );

       ANA_MSG_DEBUG( "===>>>");
       ANA_MSG_DEBUG( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
       ANA_MSG_DEBUG( "Reco SF decoration: " << sfName );
       ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
       ANA_MSG_DEBUG( "Reco efficiency SF:");
       ANA_MSG_DEBUG( "\t " << recoEffSF << " (from getEfficiencyScaleFactor())" );
       ANA_MSG_DEBUG( "--------------------------------------");

    	 ++idx;

      } // close electron loop

    }  // close loop on Reco efficiency systematics

    // Add list of systematics names to TStore
    // We only do this once per event if the list does not exist yet
    if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesReco ) ) {
      ANA_CHECK( m_store->record( std::move(sysVariationNamesReco), m_outputSystNamesReco ));
    }

  }

  // 4.
  // Trig efficiency SFs (eff_data/eff_MC) - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // NB: calculation of the event SF is up to the analyzer

  // Do it only if a tool with *this* name hasn't already been used, and has been previously initialised
  //

  if ( !m_corrFileNameTrig.empty() && !isToolAlreadyUsed(m_TrigEffSF_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesTrig = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    // Create the names of the SF weights to be recorded
    std::string sfName = "ElTrigEff_SF_syst_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      sfName += ( "_isol" + m_WorkingPointIsoTrig );
    }

    for ( const auto& syst_it : m_systListTrig ) {

      if ( !syst_it.name().empty() && !nominal ) continue;

      ANA_MSG_DEBUG("Electron Trig efficiency SF sys name (to be recorded in xAOD::TStore) is: " << syst_it.name());
      if ( writeSystNames ) sysVariationNamesTrig->push_back(syst_it.name());

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_Trig->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ANA_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_Trig for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_Trig->appliedSystematics().name() );

      // and now apply trigger efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 ANA_MSG_DEBUG( "Applying Trigger efficiency SF" );

    	 bool isBadElectron(false);

    	 //
    	 // obtain Trigger efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecTrig ( sfName  );
    	 if ( !sfVecTrig.isAvailable( *el_itr )  ) {
    	   sfVecTrig ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info");
  	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )");
  	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance");
  	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF for Trig
    	 //
    	 double trigEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron && m_asgElEffCorrTool_elSF_Trig->getEfficiencyScaleFactor( *el_itr, trigEffSF ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in Trig getEfficiencyScaleFactor Tool");
  	   isBadElectron = true;
  	   trigEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
       sfVecTrig( *el_itr ).push_back( trigEffSF );

       ANA_MSG_DEBUG( "===>>>");
       ANA_MSG_DEBUG( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
       ANA_MSG_DEBUG( "Trigger efficiency SF decoration: " << sfName );
       ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
       ANA_MSG_DEBUG( "Trigger efficiency SF:");
       ANA_MSG_DEBUG( "\t " << trigEffSF << "(from getEfficiencyScaleFactor())" );
       ANA_MSG_DEBUG( "--------------------------------------");

    	 ++idx;

      } // close electron loop

    }  // close loop on Trig efficiency SF systematics

    // Add list of systematics names to TStore
    // We only do this once per event if the list does not exist yet
    if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesTrig ) ) {
      ANA_CHECK( m_store->record( std::move(sysVariationNamesTrig), m_outputSystNamesTrig ));
    }

  }

  // 5.
  // Trig MC efficiency - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Do it only if a tool with *this* name hasn't already been used, and has been previously initialised
  //
  if ( !m_corrFileNameTrigMCEff.empty() && !isToolAlreadyUsed(m_TrigMCEff_tool_name) ) {

    if ( writeSystNames ) sysVariationNamesTrigMCEff = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>);

    // Create the names of the SF weights to be recorded
    std::string sfName = "ElTrigMCEff_syst_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      sfName += ( "_isol" + m_WorkingPointIsoTrig );
    }

    for ( const auto& syst_it : m_systListTrigMCEff ) {

      if ( !syst_it.name().empty() && !nominal ) continue;

      ANA_MSG_DEBUG("Electron Trig MC efficiency sys name (to be recorded in xAOD::TStore) is: " << syst_it.name());
      if ( writeSystNames ) sysVariationNamesTrigMCEff->push_back(syst_it.name());

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_TrigMCEff->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ANA_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_TrigMCEff for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_TrigMCEff->appliedSystematics().name() );

      // and now apply trigger MC efficiency!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 ANA_MSG_DEBUG( "Applying Trigger MC efficiency" );

    	 bool isBadElectron(false);

    	 //
    	 // obtain Trigger MC efficiency as a float (to be stored away separately)
    	 //
    	 //  If efficiency decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecTrigMCEff ( sfName );
    	 if ( !sfVecTrigMCEff.isAvailable( *el_itr )  ) {
    	   sfVecTrigMCEff ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info");
  	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )");
  	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   ANA_MSG_DEBUG( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance");
  	   isBadElectron = true;
    	 }

    	 //
    	 // obtain Trig MC efficiency
    	 //
    	 double trigMCEff(0.0); // tool wants a double
    	 if ( !isBadElectron && m_asgElEffCorrTool_elSF_TrigMCEff->getEfficiencyScaleFactor( *el_itr, trigMCEff ) != CP::CorrectionCode::Ok ) {
    	   ANA_MSG_WARNING( "Problem in TrigMCEff getEfficiencyScaleFactor Tool");
  	   isBadElectron = true;
  	   trigMCEff = 0.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
       sfVecTrigMCEff( *el_itr ).push_back( trigMCEff );

       ANA_MSG_DEBUG( "===>>>");
       ANA_MSG_DEBUG( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
       ANA_MSG_DEBUG( "Trigger efficiency decoration: " << sfName );
       ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
       ANA_MSG_DEBUG( "Trigger MC efficiency:");
       ANA_MSG_DEBUG( "\t " << trigMCEff << " (from getEfficiencyScaleFactor())" );
       ANA_MSG_DEBUG( "--------------------------------------");

    	 ++idx;

      } // close electron loop

    }  // close loop on Trig efficiency SF systematics

    // Add list of systematics names to TStore
    // We only do this once per event if the list does not exist yet
    if ( writeSystNames && !m_store->contains<std::vector<std::string>>( m_outputSystNamesTrigMCEff ) ) {
      ANA_CHECK( m_store->record( std::move(sysVariationNamesTrigMCEff), m_outputSystNamesTrigMCEff ));
    }

  }

  return EL::StatusCode::SUCCESS;
}
