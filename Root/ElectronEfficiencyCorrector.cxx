// c++ include(s):
#include <iostream>

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

#include <xAODAnaHelpers/tools/ReturnCheck.h>

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronEfficiencyCorrector)


ElectronEfficiencyCorrector :: ElectronEfficiencyCorrector (std::string className) :
    Algorithm(className),
    m_asgElEffCorrTool_elSF_PID(nullptr),
    m_asgElEffCorrTool_elSF_Iso(nullptr),
    m_asgElEffCorrTool_elSF_Reco(nullptr),
    m_asgElEffCorrTool_elSF_Trig(nullptr),
    m_asgElEffCorrTool_elSF_TrigMCEff(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  ATH_MSG_INFO( "Calling constructor");

  m_debug                   = false;

  // input container to be read from TEvent or TStore
  //
  m_inContainerName         = "";
  m_outContainerName        = "";

  m_setAFII                 = false;

  // Systematics stuff
  //
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "";
  m_systValPID              = 0.0;
  m_systValIso              = 0.0;
  m_systValReco             = 0.0;
  m_systValTrig             = 0.0;
  m_systValTrigMCEff        = 0.0;
  m_systNamePID             = "";
  m_systNameTrig            = "";
  m_systNameReco            = "";
  m_systNameTrigMCEff       = "";
  m_outputSystNamesPID      = "EleEffCorr_PIDSyst";
  m_outputSystNamesIso      = "EleEffCorr_IsoSyst";
  m_outputSystNamesReco     = "EleEffCorr_RecoSyst";
  m_outputSystNamesTrig     = "EleEffCorr_TrigSyst";
  m_outputSystNamesTrigMCEff = "EleEffCorr_TrigMCEffSyst";
  m_correlationModel = "FULL";

  // file(s) containing corrections
  //
  m_corrFileNamePID         = "";
  m_corrFileNameIso         = "";
  m_corrFileNameReco        = "";
  m_corrFileNameTrig        = "";
  m_corrFileNameTrigMCEff   = "";
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

  ATH_MSG_INFO( "Calling setupJob");

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
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
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

  ATH_MSG_INFO( "Initializing ElectronEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ATH_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  if ( m_inContainerName.empty() ) {
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronEfficiencyCorrector::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  m_numEvent      = 0;
  m_numObject     = 0;


  int sim_flav(1); // default for FullSim
  if ( m_isMC ) {
    const std::string stringMeta = wk()->metaData()->castString("SimulationFlavour");
    if ( m_setAFII || ( !stringMeta.empty() && ( stringMeta.find("AFII") != std::string::npos ) ) ) {
      ATH_MSG_INFO( "Setting simulation flavour to AFII");
      sim_flav = 3;
    }
    else if ( !m_setAFII && stringMeta.empty() ) {
      ATH_MSG_WARNING( "No meta-data string found. Simulation flavour will be set to FullSim. Care if you are running on Fast-Sim MC.");
    }
  }

  // 1.
  // initialize the AsgElectronEfficiencyCorrectionTool for PID efficiency SF
  //
  if ( !m_corrFileNamePID.empty() ) {

    m_PID_WP = HelperFunctions::parse_wp( "ID", m_corrFileNamePID );

    if ( m_PID_WP.empty() ) {
      Error("initialize()", "ID working point for electronID SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    std::cout << "\n\n Electron ID wp: " << m_PID_WP << "\n\n" << std::endl;

    m_pidEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_PID_" + m_PID_WP;

    RETURN_CHECK("ElectronEfficiencyCorrector::initialize()", checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_pidEffSF_tool_name), "" );

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_pidEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_PID = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_pidEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_PID = new AsgElectronEfficiencyCorrectionTool(m_pidEffSF_tool_name);
      m_asgElEffCorrTool_elSF_PID->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesPID{ m_corrFileNamePID } ; // initialise vector w/ all the files containing corrections
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("CorrectionFileNameList",inputFilesPID),"Failed to set property CorrectionFileNameList");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("ForceDataType",sim_flav),"Failed to set property ForceDataType");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("CorrelationModel",m_correlationModel),"Failed to set property CorrelationModel");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool PID");
    }

    if ( m_debug ) {

      // Get a list of affecting systematics
     //
      CP::SystematicSet affectSystsPID = m_asgElEffCorrTool_elSF_PID->affectingSystematics();
      //
      // Convert into a simple list
      //
      for ( const auto& syst_it : affectSystsPID ) { ATH_MSG_INFO("AsgElectronEfficiencyCorrectionTool can be affected by PID efficiency systematic: " << syst_it.name()); }
    }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsPID = m_asgElEffCorrTool_elSF_PID->recommendedSystematics();
    m_systListPID = HelperFunctions::getListofSystematics( recSystsPID, m_systNamePID, m_systValPID, m_debug );

    ATH_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool PID efficiency systematic:");
    for ( const auto& syst_it : m_systListPID ) {
      if ( m_systNamePID.empty() ) {
    	Info("initialize()","\t Running w/ nominal configuration only!");
    	break;
      }
      ATH_MSG_INFO("\t " << syst_it.name());
    }

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesPID = m_outputSystNamesPID + "_" + m_PID_WP;

  }

  // 2.
  // initialize the AsgElectronEfficiencyCorrectionTool for isolation efficiency SF
  //
  if ( !m_corrFileNameIso.empty() ) {

    m_Iso_WP    = HelperFunctions::parse_wp( "ISO", m_corrFileNameIso );
    m_IsoPID_WP = HelperFunctions::parse_wp( "ID", m_corrFileNameIso );

    if ( m_Iso_WP.empty() ) {
      Error("initialize()","ISO working point for isolation SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }
    if ( m_IsoPID_WP.empty() ) {
      Error("initialize()", "ID working point for isolation SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    std::cout << "\n\n ISOLATION wp: " << m_Iso_WP << "\n ID wp (for isolation): " << m_IsoPID_WP << "\n\n" << std::endl;

    m_IsoEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_Iso_" + m_IsoPID_WP + "_isol" + m_Iso_WP;

    RETURN_CHECK("ElectronEfficiencyCorrector::initialize()", checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_IsoEffSF_tool_name), "" );

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_IsoEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_Iso = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_IsoEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_Iso = new AsgElectronEfficiencyCorrectionTool(m_IsoEffSF_tool_name);
      m_asgElEffCorrTool_elSF_Iso->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesIso{ m_corrFileNameIso } ; // initialise vector w/ all the files containing corrections
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Iso->setProperty("CorrectionFileNameList",inputFilesIso),"Failed to set property CorrectionFileNameList");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Iso->setProperty("ForceDataType",sim_flav),"Failed to set property ForceDataType");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Iso->setProperty("CorrelationModel",m_correlationModel),"Failed to set property CorrelationModel");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Iso->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool Iso");
    }

    if ( m_debug ) {

      // Get a list of affecting systematics
     //
      CP::SystematicSet affectSystsIso = m_asgElEffCorrTool_elSF_Iso->affectingSystematics();
      //
      // Convert into a simple list
      //
      for ( const auto& syst_it : affectSystsIso ) { ATH_MSG_INFO("AsgElectronEfficiencyCorrectionTool can be affected by Iso efficiency systematic: " << syst_it.name()); }
    }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsIso = m_asgElEffCorrTool_elSF_Iso->recommendedSystematics();
    m_systListIso = HelperFunctions::getListofSystematics( recSystsIso, m_systNameIso, m_systValIso, m_debug );

    ATH_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool Iso efficiency systematic:");
    for ( const auto& syst_it : m_systListIso ) {
      if ( m_systNameIso.empty() ) {
    	Info("initialize()","\t Running w/ nominal configuration only!");
    	break;
      }
      ATH_MSG_INFO("\t " << syst_it.name());
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

    RETURN_CHECK("ElectronEfficiencyCorrector::initialize()", checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_RecoEffSF_tool_name), "" );

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_RecoEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_Reco = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_RecoEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_Reco = new AsgElectronEfficiencyCorrectionTool(m_RecoEffSF_tool_name);
      m_asgElEffCorrTool_elSF_Reco->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesReco{ m_corrFileNameReco } ; // initialise vector w/ all the files containing corrections
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->setProperty("CorrectionFileNameList",inputFilesReco),"Failed to set property CorrectionFileNameList");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->setProperty("ForceDataType",sim_flav),"Failed to set property ForceDataType");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->setProperty("CorrelationModel",m_correlationModel),"Failed to set property CorrelationModel");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool Reco");
    }

    if ( m_debug ) {

      // Get a list of affecting systematics
      //
      CP::SystematicSet affectSystsReco = m_asgElEffCorrTool_elSF_Reco->affectingSystematics();
      //
      // Convert into a simple list
      //
      for ( const auto& syst_it : affectSystsReco ) { ATH_MSG_INFO("AsgElectronEfficiencyCorrectionTool can be affected by reco efficiency systematic: " << syst_it.name()); }
    }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsReco = m_asgElEffCorrTool_elSF_Reco->recommendedSystematics();
    m_systListReco = HelperFunctions::getListofSystematics( recSystsReco, m_systNameReco, m_systValReco, m_debug );

    ATH_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool reco efficiency systematic:");
    for ( const auto& syst_it : m_systListReco ) {
      if ( m_systNameReco.empty() ) {
    	Info("initialize()","\t Running w/ nominal configuration only!");
    	break;
      }
      ATH_MSG_INFO("\t " << syst_it.name());
    }
  }

  // 4.
  // Initialise the AsgElectronEfficiencyCorrectionTool for Trigger Efficiency SF
  //
  if ( !m_corrFileNameTrig.empty() ) {

    m_WorkingPointIsoTrig = HelperFunctions::parse_wp( "ISO", m_corrFileNameTrig );
    m_WorkingPointIDTrig  = HelperFunctions::parse_wp( "ID", m_corrFileNameTrig );
    m_WorkingPointTrigTrig = HelperFunctions::parse_wp( "TRIG", m_corrFileNameTrigMCEff );

    if ( m_WorkingPointIDTrig.empty() ) {
      Error("initialize()", "ID working point for trigger SF not found in config file! This should not happen. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    std::cout << "\n\n Trigger ISOLATION wp: " << m_WorkingPointIsoTrig << "\n Trigger ID wp: " << m_WorkingPointIDTrig << "\n\n" << std::endl;

    m_TrigEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_Trig_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      m_TrigEffSF_tool_name += ( "_isol" + m_WorkingPointIsoTrig );
    }

    RETURN_CHECK("ElectronEfficiencyCorrector::initialize()", checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_TrigEffSF_tool_name), "" );

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_TrigEffSF_tool_name) ) {
      m_asgElEffCorrTool_elSF_Trig = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_TrigEffSF_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_Trig = new AsgElectronEfficiencyCorrectionTool(m_TrigEffSF_tool_name);
      m_asgElEffCorrTool_elSF_Trig->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesTrig{ m_corrFileNameTrig } ; // initialise vector w/ all the files containing corrections
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->setProperty("CorrectionFileNameList",inputFilesTrig),"Failed to set property CorrectionFileNameList");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->setProperty("ForceDataType",sim_flav),"Failed to set property ForceDataType");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->setProperty("CorrelationModel",m_correlationModel),"Failed to set property CorrelationModel");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool Trig");
    }

    if ( m_debug ) {

      // Get a list of affecting systematics
     //
      CP::SystematicSet affectSystsTrig = m_asgElEffCorrTool_elSF_Trig->affectingSystematics();
      //
      // Convert into a simple list
      //
      for ( const auto& syst_it : affectSystsTrig ) { ATH_MSG_INFO("AsgElectronEfficiencyCorrectionTool can be affected by Trig efficiency SF systematic: " << syst_it.name()); }
    }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsTrig = m_asgElEffCorrTool_elSF_Trig->recommendedSystematics();
    m_systListTrig = HelperFunctions::getListofSystematics( recSystsTrig, m_systNameTrig, m_systValTrig, m_debug );

    ATH_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool Trig efficiency SF systematic:");
    for ( const auto& syst_it : m_systListTrig ) {
      if ( m_systNameTrig.empty() ) {
    	Info("initialize()","\t Running w/ nominal configuration only!");
    	break;
      }
      ATH_MSG_INFO("\t " << syst_it.name());
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

    RETURN_CHECK("ElectronEfficiencyCorrector::initialize()", checkToolStore<AsgElectronEfficiencyCorrectionTool>(m_TrigMCEff_tool_name), "" );

    if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>(m_TrigMCEff_tool_name) ) {
      m_asgElEffCorrTool_elSF_TrigMCEff= asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>(m_TrigMCEff_tool_name);
    } else {
      m_asgElEffCorrTool_elSF_TrigMCEff = new AsgElectronEfficiencyCorrectionTool(m_TrigMCEff_tool_name);
      m_asgElEffCorrTool_elSF_TrigMCEff->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
      std::vector<std::string> inputFilesTrigMCEff{ m_corrFileNameTrigMCEff } ; // initialise vector w/ all the files containing corrections
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_TrigMCEff->setProperty("CorrectionFileNameList",inputFilesTrigMCEff),"Failed to set property CorrectionFileNameList");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_TrigMCEff->setProperty("ForceDataType",sim_flav),"Failed to set property ForceDataType");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_TrigMCEff->setProperty("CorrelationModel",m_correlationModel),"Failed to set property CorrelationModel");
      RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_TrigMCEff->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool TrigMCEff");
    }

    if ( m_debug ) {

      // Get a list of affecting systematics
     //
      CP::SystematicSet affectSystsTrigMCEff = m_asgElEffCorrTool_elSF_TrigMCEff->affectingSystematics();
      //
      // Convert into a simple list
      //
      for ( const auto& syst_it : affectSystsTrigMCEff ) { ATH_MSG_INFO("AsgElectronEfficiencyCorrectionTool can be affected by TrigMCEff efficiency systematic: " << syst_it.name()); }
    }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsTrigMCEff = m_asgElEffCorrTool_elSF_TrigMCEff->recommendedSystematics();
    m_systListTrigMCEff = HelperFunctions::getListofSystematics( recSystsTrigMCEff, m_systNameTrigMCEff, m_systValTrigMCEff, m_debug );

    ATH_MSG_INFO("Will be using AsgElectronEfficiencyCorrectionTool TrigMCEff efficiency systematic:");
    for ( const auto& syst_it : m_systListTrigMCEff ) {
      if ( m_systNameTrigMCEff.empty() ) {
  	Info("initialize()","\t Running w/ nominal configuration only!");
  	break;
      }
      ATH_MSG_INFO("\t " << syst_it.name());
    }

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    //
    m_outputSystNamesTrigMCEff = m_outputSystNamesTrigMCEff + "_" + m_WorkingPointTrigTrig + "_" + m_WorkingPointIDTrig;;
    if ( !m_WorkingPointIsoTrig.empty() ) {
      m_outputSystNamesTrigMCEff += ( "_isol" + m_WorkingPointIsoTrig );
    }

  }

  // *********************************************************************************

  ATH_MSG_INFO( "ElectronEfficiencyCorrector Interface succesfully initialized!" );

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
    if ( m_numEvent == 1 ) { ATH_MSG_INFO( "Sample is Data! Do not apply any Electron Efficiency correction... "); }
    return EL::StatusCode::SUCCESS;
  }

  if ( m_debug ) { ATH_MSG_INFO( "Applying Electron Efficiency Correction... "); }
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  // initialise containers
  //
  const xAOD::ElectronContainer* inputElectrons(nullptr);

  // if m_inputAlgoSystNames = "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it

  // Declare a counter, initialised to 0
  // For the systematically varied input containers, we won't store again the vector with efficiency systs in TStore ( it will be always the same!)
  //
  unsigned int countInputCont(0);

  if ( m_inputAlgoSystNames.empty() ) {

    // I might not want to decorate sys altered electrons but for some events the nominal container might not exist
    // if electrons are only allowed in systematic instances, hence, we have to check for the existence of the nominal container
    //
    if ( m_store->contains<xAOD::ElectronContainer>( m_inContainerName )  ) {
       RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(inputElectrons, m_inContainerName, m_event, m_store, m_verbose) ,"");

       if ( m_debug ) { ATH_MSG_INFO( "Number of electrons: " << static_cast<int>(inputElectrons->size()) ); }

       // decorate electrons w/ SF - there will be a decoration w/ different name for each syst!
       //
       this->executeSF( inputElectrons, countInputCont );
    }

  } else {
  // if m_inputAlgo = NOT EMPTY --> you are retrieving syst varied containers from an upstream algo.
  // This is the case of calibrators: one different SC for each calibration syst applied

      // get vector of string giving the syst names of the upstream algo m_inputAlgo (rememeber: 1st element is a blank string: nominal case!)
      //
      std::vector<std::string>* systNames(nullptr);
      RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

    	// loop over systematic sets available
	//
        std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

        for ( auto systName : *systNames ) {

          const xAOD::ElectronContainer* outputElectrons(nullptr);

          if ( m_store->contains<xAOD::ElectronContainer>( m_inContainerName+systName )  ) {

             RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(inputElectrons, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");

             if ( !m_store->contains<xAOD::ElectronContainer>( m_outContainerName+systName ) ) {
               RETURN_CHECK("ElectronEfficiencyCorrector::execute()", (HelperFunctions::makeDeepCopy<xAOD::ElectronContainer, xAOD::ElectronAuxContainer, xAOD::Electron>(m_store, m_outContainerName+systName, inputElectrons)), "");
             }

             RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(outputElectrons, m_outContainerName+systName, m_event, m_store, m_verbose) ,"");

             if ( m_debug ){
                 ATH_MSG_INFO( "Number of electrons: " << static_cast<int>(outputElectrons->size()) );
	         ATH_MSG_INFO( "Input syst: " << systName );
                 unsigned int idx(0);
                 for ( auto el : *(outputElectrons) ) {
                     ATH_MSG_INFO( "Input electron " << idx << ", pt = " << el->pt() * 1e-3 << " GeV " );
                     ++idx;
                 }
             }

             // decorate electrons w/ SF - there will be a decoration w/ different name for each syst!
	     //
             this->executeSF( outputElectrons, countInputCont );

             vecOutContainerNames->push_back( systName );
             // increment counter
	     //
             ++countInputCont;

          } // check existence of container

        } // close loop on systematic sets available from upstream algo

        if ( !m_outputAlgoSystNames.empty() && !m_store->contains< std::vector<std::string> >( m_outputAlgoSystNames ) ) { // might have already been stored by another execution of this algo
          RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");
        }

  }

  // look what we have in TStore
  //
  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { ATH_MSG_INFO( "Calling postExecute"); }

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

  ATH_MSG_INFO( "Deleting tool instances...");

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

  ATH_MSG_INFO( "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronEfficiencyCorrector :: executeSF ( const xAOD::ElectronContainer* inputElectrons, unsigned int countSyst )
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
  std::vector< std::string >* sysVariationNamesPID       = nullptr;
  std::vector< std::string >* sysVariationNamesReco      = nullptr;
  std::vector< std::string >* sysVariationNamesIso       = nullptr;
  std::vector< std::string >* sysVariationNamesTrig      = nullptr;
  std::vector< std::string >* sysVariationNamesTrigMCEff = nullptr;

  // 1.
  // PID efficiency SFs - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Do it only if a tool with *this* name hasn't already been used, and has been previously initialised
  //
  if ( !m_corrFileNamePID.empty() && !isToolAlreadyUsed(m_pidEffSF_tool_name) ) {

    if(countSyst == 0) sysVariationNamesPID = new std::vector< std::string >;

    for ( const auto& syst_it : m_systListPID ) {

      // Create the name of the SF weight to be recorded
      //   template:  SYSNAME_ElPIDEff_SF
      //
      std::string sfName  = "ElPIDEff_SF_" + m_PID_WP;

      if ( !syst_it.name().empty() ) {
    	 std::string prepend = syst_it.name() + "_";
    	 sfName.insert( 0, prepend );
      }
      if ( m_debug ) ATH_MSG_INFO( "Electron PID efficiency sys name (to be recorded in xAOD::TStore) is: " << sfName);
      if(countSyst == 0) sysVariationNamesPID->push_back(sfName);

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_PID->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ATH_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_PID for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      if ( m_debug ) { ATH_MSG_INFO( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_PID->appliedSystematics().name() ); }

      // and now apply PID efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 if ( m_debug ) { ATH_MSG_INFO( "Applying PID efficiency SF" ); }

    	 bool isBadElectron(false);

    	 //
    	 // obtain PID efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecPID ( m_outputSystNamesPID  );
    	 if ( !sfVecPID.isAvailable( *el_itr )  ) {
    	   sfVecPID ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info"); }
    	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )"); }
    	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance"); }
    	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF's for PID
    	 //
    	 double pidEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron &&  m_asgElEffCorrTool_elSF_PID->getEfficiencyScaleFactor( *el_itr, pidEffSF ) != CP::CorrectionCode::Ok ) {
    	   ATH_MSG_WARNING( "Problem in PID getEfficiencyScaleFactor Tool");
  	   pidEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecPID( *el_itr ).push_back( pidEffSF );

    	 if ( m_debug ) {
    	   ATH_MSG_INFO( "===>>>");
    	   ATH_MSG_INFO( " ");
  	   ATH_MSG_INFO( "Electron " << idx << ", pt = " << el_itr->pt()*1e-3 << " GeV ");
  	   ATH_MSG_INFO( " ");
  	   ATH_MSG_INFO( "PID SF decoration: " << m_outputSystNamesPID );
  	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Systematic: " << syst_it.name() );
    	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "PID efficiency SF:");
    	   ATH_MSG_INFO( "\t " << pidEffSF << " (from getEfficiencyScaleFactor())" );
    	   ATH_MSG_INFO( "--------------------------------------");
    	 }

    	 ++idx;

      } // close electron loop

    }  // close loop on PID efficiency systematics

    // Add list of systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSF() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    // Use the counter defined in execute() to check this is done only once per event
    //
    if ( countSyst == 0 ) { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesPID,  m_outputSystNamesPID ), "Failed to record vector of systematic names PID"  ); }

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

    if(countSyst == 0) sysVariationNamesIso = new std::vector< std::string >;

    for ( const auto& syst_it : m_systListIso ) {

      // Create the name of the SF weight to be recorded
      //   template:  SYSNAME_ElIsoEff_SF
      //
      std::string sfName  = "ElIsoEff_SF_" + m_IsoPID_WP + "_isol" + m_Iso_WP;

      if ( !syst_it.name().empty() ) {
    	 std::string prepend = syst_it.name() + "_";
    	 sfName.insert( 0, prepend );
      }
      if ( m_debug ) ATH_MSG_INFO( "Electron Iso efficiency sys name (to be recorded in xAOD::TStore) is: " << sfName);
      if(countSyst == 0) sysVariationNamesIso->push_back(sfName);

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_Iso->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ATH_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_Iso for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      if ( m_debug ) { ATH_MSG_INFO( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_Iso->appliedSystematics().name() ); }

      // and now apply Iso efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 if ( m_debug ) { ATH_MSG_INFO( "Applying Iso efficiency SF" ); }

    	 bool isBadElectron(false);

    	 //
    	 // obtain Iso efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecIso ( m_outputSystNamesIso  );
    	 if ( !sfVecIso.isAvailable( *el_itr )  ) {
    	   sfVecIso ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info"); }
    	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )"); }
    	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance"); }
    	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF's for Iso
    	 //
    	 double IsoEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron &&  m_asgElEffCorrTool_elSF_Iso->getEfficiencyScaleFactor( *el_itr, IsoEffSF ) != CP::CorrectionCode::Ok ) {
    	   ATH_MSG_WARNING( "Problem in Iso getEfficiencyScaleFactor Tool");
  	   IsoEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecIso( *el_itr ).push_back( IsoEffSF );

    	 if ( m_debug ) {
    	   ATH_MSG_INFO( "===>>>");
    	   ATH_MSG_INFO( " ");
  	   ATH_MSG_INFO( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
  	   ATH_MSG_INFO( " ");
  	   ATH_MSG_INFO( "Iso SF decoration: " << m_outputSystNamesIso );
  	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Systematic: " << syst_it.name() );
    	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Iso efficiency SF:");
    	   ATH_MSG_INFO( "\t " << IsoEffSF << " (from getEfficiencyScaleFactor())" );
    	   ATH_MSG_INFO( "--------------------------------------");
    	 }

    	 ++idx;

      } // close electron loop

    }  // close loop on Iso efficiency systematics

    // Add list of systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSF() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    // Use the counter defined in execute() to check this is done only once per event
    //
    if ( countSyst == 0 ) { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesIso,  m_outputSystNamesIso), "Failed to record vector of systematic names Iso" ); }

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

    if(countSyst == 0) sysVariationNamesReco = new std::vector< std::string >;

    for ( const auto& syst_it : m_systListReco ) {

      // Create the name of the SF weight to be recorded
      //   template:  SYSNAME_ElRecoEff_SF
      //
      std::string sfName  = "ElRecoEff_SF";

      if ( !syst_it.name().empty() ) {
    	 std::string prepend = syst_it.name() + "_";
    	 sfName.insert( 0, prepend );
      }
      if ( m_debug ) ATH_MSG_INFO( "Electron Reco efficiency sys name (to be recorded in xAOD::TStore) is: " << sfName);
      if(countSyst == 0) sysVariationNamesReco->push_back(sfName);

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_Reco->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ATH_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_Reco for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      if ( m_debug ) { ATH_MSG_INFO( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_Reco->appliedSystematics().name() ); }

      // and now apply Reco efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 if ( m_debug ) { ATH_MSG_INFO( "Applying Reco efficiency SF" ); }

    	 bool isBadElectron(false);

    	 //
    	 // obtain Reco efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecReco ( m_outputSystNamesReco  );
    	 if ( !sfVecReco.isAvailable( *el_itr )  ) {
    	   sfVecReco ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info"); }
  	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )"); }
  	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance"); }
  	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF's for Reco
    	 //
    	 double recoEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron && m_asgElEffCorrTool_elSF_Reco->getEfficiencyScaleFactor( *el_itr, recoEffSF ) != CP::CorrectionCode::Ok ) {
    	   ATH_MSG_WARNING( "Problem in Reco getEfficiencyScaleFactor Tool");
  	   recoEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecReco( *el_itr ).push_back( recoEffSF );

    	 if ( m_debug ) {
    	   ATH_MSG_INFO( "===>>>");
    	   ATH_MSG_INFO( " ");
  	   ATH_MSG_INFO( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
  	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Reco SF decoration: " << m_outputSystNamesReco );
    	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Systematic: " << syst_it.name() );
    	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Reco efficiency SF:");
    	   ATH_MSG_INFO( "\t " << recoEffSF << " (from getEfficiencyScaleFactor())" );
    	   ATH_MSG_INFO( "--------------------------------------");
    	 }

    	 ++idx;

      } // close electron loop

    }  // close loop on Reco efficiency systematics

    // Add list of systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSF() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    // Use the counter defined in execute() to check this is done only once per event
    //
    if ( countSyst == 0 ) { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesReco, m_outputSystNamesReco), "Failed to record vector of systematic names Reco" ); }

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

    if(countSyst == 0) sysVariationNamesTrig = new std::vector< std::string >;

    for ( const auto& syst_it : m_systListTrig ) {

      // Create the name of the SF weight to be recorded
      //   template:  SYSNAME_ElTrigEff_SF
      //
      std::string sfName  = "ElTrigEff_SF_" + m_WorkingPointIDTrig;
      if ( !m_WorkingPointIsoTrig.empty() ) {
        sfName += ( "_isol" + m_WorkingPointIsoTrig );
      }

      if ( !syst_it.name().empty() ) {
    	 std::string prepend = syst_it.name() + "_";
    	 sfName.insert( 0, prepend );
      }
      if ( m_debug ) ATH_MSG_INFO( "Electron Trig efficiency SF sys name (to be recorded in xAOD::TStore) is: " << sfName);
      if(countSyst == 0) sysVariationNamesTrig->push_back(sfName);

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_Trig->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ATH_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_Trig for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      if ( m_debug ) { ATH_MSG_INFO( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_Trig->appliedSystematics().name() ); }

      // and now apply trigger efficiency SF!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 if ( m_debug ) { ATH_MSG_INFO( "Applying Trigger efficiency SF" ); }

    	 bool isBadElectron(false);

    	 //
    	 // obtain Trigger efficiency SF as a float (to be stored away separately)
    	 //
    	 //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecTrig ( m_outputSystNamesTrig  );
    	 if ( !sfVecTrig.isAvailable( *el_itr )  ) {
    	   sfVecTrig ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info"); }
  	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )"); }
  	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance"); }
  	   isBadElectron = true;
    	 }

    	 //
    	 // obtain efficiency SF for Trig
    	 //
    	 double trigEffSF(1.0); // tool wants a double
    	 if ( !isBadElectron && m_asgElEffCorrTool_elSF_Trig->getEfficiencyScaleFactor( *el_itr, trigEffSF ) != CP::CorrectionCode::Ok ) {
    	   ATH_MSG_WARNING( "Problem in Trig getEfficiencyScaleFactor Tool");
  	   isBadElectron = true;
  	   trigEffSF = 1.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecTrig( *el_itr ).push_back( trigEffSF );

    	 if ( m_debug ) {
    	   ATH_MSG_INFO( "===>>>");
    	   ATH_MSG_INFO( " ");
  	   ATH_MSG_INFO( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
  	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Trigger efficiency SF decoration: " << m_outputSystNamesTrig );
	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Systematic: " << syst_it.name() );
    	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Trigger efficiency SF:");
    	   ATH_MSG_INFO( "\t " << trigEffSF << "(from getEfficiencyScaleFactor())" );
    	   ATH_MSG_INFO( "--------------------------------------");
    	 }

    	 ++idx;

      } // close electron loop

    }  // close loop on Trig efficiency SF systematics

    // Add list of systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSF() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    // Use the counter defined in execute() to check this is done only once per event
    //
    if ( countSyst == 0 ) { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesTrig, m_outputSystNamesTrig), "Failed to record vector of systematic names Trig" ); }

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

    if(countSyst == 0) sysVariationNamesTrigMCEff = new std::vector< std::string >;

    for ( const auto& syst_it : m_systListTrigMCEff ) {

      // Create the name of the SF weight to be recorded
      //   template:  SYSNAME_ElTrigEff_SF
      //
      std::string sfName  = "ElTrigMCEff_" + m_WorkingPointIDTrig;
      if ( !m_WorkingPointIsoTrig.empty() ) {
        sfName += ( "_isol" + m_WorkingPointIsoTrig );
      }

      if ( !syst_it.name().empty() ) {
    	 std::string prepend = syst_it.name() + "_";
    	 sfName.insert( 0, prepend );
      }
      if ( m_debug ) ATH_MSG_INFO( "Electron Trig MC efficiency sys name (to be recorded in xAOD::TStore) is: " << sfName);
      if(countSyst == 0) sysVariationNamesTrigMCEff->push_back(sfName);

      // apply syst
      //
      if ( m_asgElEffCorrTool_elSF_TrigMCEff->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
    	ATH_MSG_ERROR("Failed to configure AsgElectronEfficiencyCorrectionTool_TrigMCEff for systematic " << syst_it.name());
    	return EL::StatusCode::FAILURE;
      }
      if ( m_debug ) { ATH_MSG_INFO( "Successfully applied systematics: " << m_asgElEffCorrTool_elSF_TrigMCEff->appliedSystematics().name() ); }

      // and now apply trigger MC efficiency!
      //
      unsigned int idx(0);
      for ( auto el_itr : *(inputElectrons) ) {

    	 if ( m_debug ) { ATH_MSG_INFO( "Applying Trigger MC efficiency" ); }

    	 bool isBadElectron(false);

    	 //
    	 // obtain Trigger MC efficiency as a float (to be stored away separately)
    	 //
    	 //  If efficiency decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
    	 //
    	 SG::AuxElement::Decorator< std::vector<float> > sfVecTrigMCEff ( m_outputSystNamesTrigMCEff  );
    	 if ( !sfVecTrigMCEff.isAvailable( *el_itr )  ) {
    	   sfVecTrigMCEff ( *el_itr ) = std::vector<float>();
    	 }

    	 // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
    	 //
    	 if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", it has no caloCluster or trackParticle info"); }
  	   isBadElectron = true;
    	 }
    	 //
    	 // skip electron if outside acceptance for SF calculation
    	 //
    	 if ( el_itr->pt() < 15e3 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside pT acceptance ( currently SF available for pT > 15 GeV )"); }
  	   isBadElectron = true;
    	 }
    	 if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
    	   if ( m_debug ) { ATH_MSG_INFO( "Apply SF: skipping electron " << idx << ", is outside |eta| acceptance"); }
  	   isBadElectron = true;
    	 }

    	 //
    	 // obtain Trig MC efficiency
    	 //
    	 double trigMCEff(0.0); // tool wants a double
    	 if ( !isBadElectron && m_asgElEffCorrTool_elSF_TrigMCEff->getEfficiencyScaleFactor( *el_itr, trigMCEff ) != CP::CorrectionCode::Ok ) {
    	   ATH_MSG_WARNING( "Problem in TrigMCEff getEfficiencyScaleFactor Tool");
  	   isBadElectron = true;
  	   trigMCEff = 0.0;
    	 }
    	 //
    	 // Add it to decoration vector
    	 //
    	 sfVecTrigMCEff( *el_itr ).push_back( trigMCEff );

    	 if ( m_debug ) {
    	   ATH_MSG_INFO( "===>>>");
    	   ATH_MSG_INFO( " ");
  	   ATH_MSG_INFO( "Electron " << idx << ", pt = " << el_itr->pt() * 1e-3 << " GeV" );
  	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Trigger efficiency decoration: " << m_outputSystNamesTrigMCEff );
  	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Systematic: " << syst_it.name() );
    	   ATH_MSG_INFO( " ");
    	   ATH_MSG_INFO( "Trigger MC efficiency:");
    	   ATH_MSG_INFO( "\t " << trigMCEff << " (from getEfficiencyScaleFactor())" );
    	   ATH_MSG_INFO( "--------------------------------------");
    	 }

    	 ++idx;

      } // close electron loop

    }  // close loop on Trig efficiency SF systematics

    // Add list of systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSF() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    // Use the counter defined in execute() to check this is done only once per event
    //
    if ( countSyst == 0 ) { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesTrigMCEff, m_outputSystNamesTrigMCEff), "Failed to record vector of systematic names TrigMCEff" ); }

  }

  return EL::StatusCode::SUCCESS;
}
