/******************************************
 *
 * Interface to Jet calibration tool(s).
 *
 * G.Facini (gabriel.facini@cern.ch), M. Milesi (marco.milesi@cern.ch), J. Dandoy (jeff.dandoy@cern.ch)
 *
 *
 ******************************************/

// c++ include(s):
#include <iostream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/Jet.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/JetCalibrator.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"


// this is needed to distribute the algorithm to the workers
ClassImp(JetCalibrator)

JetCalibrator :: JetCalibrator (std::string className) :
    Algorithm(className),
    m_runSysts(false),          // gets set later is syst applies to this tool
    m_jetCalibration(nullptr),  // JetCalibrationTool
    m_JESUncertTool(nullptr),   // JetUncertaintiesTool
    m_jetCleaning(nullptr)      // JetCleaningTool
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("JetCalibrator()", "Calling constructor");


  // read debug flag from .config file
  m_debug                   = false;

  m_sort                    = true;
  // input container to be read from TEvent or TStore
  m_inContainerName         = "";
  // shallow copies are made with this output container name
  m_outContainerName        = "";

  // CONFIG parameters for JetCalibrationTool
  m_jetAlgo                 = "";
  m_outputAlgo     = "";

  // when running data "_Insitu" is appended to this string
  m_calibSequence           = "JetArea_Residual_Origin_EtaJES_GSC";
  m_calibConfigFullSim      = "JES_MC15Prerecommendation_April2015.config";
  m_calibConfigAFII         = "JES_Prerecommendation2015_AFII_Apr2015.config";
  m_calibConfigData         = "JES_MC15Prerecommendation_April2015.config";
  m_calibConfig             = "";

  // CONFIG parameters for JetUncertaintiesTool
  m_JESUncertConfig         = "";
  m_JESUncertMCType         = "MC15";
  m_setAFII                 = false;

  // CONFIG parameters for JERSmearingTool
  m_JERUncertConfig         = "";
  m_JERFullSys              = false;
  m_JERApplyNominal         = false;

  // CONFIG parameters for JetCleaningTool
  m_jetCleanCutLevel        = "LooseBad";
  m_saveAllCleanDecisions   = false;
  m_jetCleanUgly            = false;
  m_cleanParent             = false;
  m_applyFatJetPreSel       = false;

  //recalculate JVT using calibrated jets
  m_redoJVT                 = false;

  // Initialize systematics variables
  m_systName                = "";
  m_systVal                 = 0.;
}

EL::StatusCode  JetCalibrator :: configure ()
{
  if ( !getConfig().empty() ) {

    Info("configure()", "Configuring JetCalibrator Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug                   = config->GetValue("Debug" , m_debug);
    m_sort                    = config->GetValue("Sort",            m_sort);
    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());
    // shallow copies are made with this output container name
    m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());

    // CONFIG parameters for JetCalibrationTool and systematic variations
    m_jetAlgo                 = config->GetValue("JetAlgorithm",        m_jetAlgo.c_str());
    m_outputAlgo     = config->GetValue("OutputAlgoSystNames", m_outputAlgo.c_str());
    m_outputAlgo     = config->GetValue("OutputAlgo", m_outputAlgo.c_str()); //legacy option

    // Provisional default, since "None" is assigned to this string if this default is not set here
    m_systName                = config->GetValue("SystName",            m_systName.c_str());
    m_systVal                 = config->GetValue("SystVal",             m_systVal);

    // when running data "_Insitu" is appended to this string
    m_calibSequence           = config->GetValue("CalibSequence",           m_calibSequence.c_str());
    m_calibConfigFullSim      = config->GetValue("configNameFullSim",       m_calibConfigFullSim.c_str());
    m_calibConfigAFII         = config->GetValue("configNameAFII",          m_calibConfigAFII.c_str());
    m_calibConfigData         = config->GetValue("configNameData",          m_calibConfigData.c_str());

    // CONFIG parameters for JetUncertaintiesTool
    m_JESUncertConfig         = config->GetValue("JESUncertConfig", m_JESUncertConfig.c_str());
    m_JESUncertMCType         = config->GetValue("JESUncertMCType", m_JESUncertMCType.c_str());
    m_setAFII                 = config->GetValue("SetAFII",  m_setAFII);

    // CONFIG parameters for JERSmearingTool
    m_JERUncertConfig         = config->GetValue("JERUncertConfig", m_JERUncertConfig.c_str());
    m_JERFullSys              = config->GetValue("JERFullSys",      m_JERFullSys);
    m_JERApplyNominal         = config->GetValue("JERApplyNominal", m_JERApplyNominal);

    // CONFIG parameters for JetCleaningTool
    m_jetCleanCutLevel        = config->GetValue("JetCleanCutLevel",        m_jetCleanCutLevel.c_str());
    m_jetCleanUgly            = config->GetValue("JetCleanUgly",            m_jetCleanUgly );
    m_saveAllCleanDecisions   = config->GetValue("SaveAllCleanDecisions",   m_saveAllCleanDecisions);
    m_cleanParent             = config->GetValue("CleanParent",             m_cleanParent);
    m_applyFatJetPreSel       = config->GetValue("ApplyFatJetPreSel",       m_applyFatJetPreSel);

    m_redoJVT                 = config->GetValue("RedoJVT",         m_redoJVT);

    config->Print();

    delete config; config = nullptr;
  }

  // If there is no InputContainer we must stop
  if ( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  if ( m_outputAlgo.empty() ) {
    m_outputAlgo = m_jetAlgo + "_Calib_Algo";
  }

  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  if ( !getConfig().empty() )
    Info("configure()", "JetCalibrator Interface succesfully configured! ");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetCalibrator :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "JetCalibrator" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing JetCalibrator Interface... ");
  m_runSysts = false; //Ensure this starts false

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  //Insitu should not be applied to the trimmed jets, per Jet/Etmiss recommendation
  if ( !m_isMC && m_calibSequence.find("Insitu") == std::string::npos && m_inContainerName.find("AntiKt10LCTopoTrimmedPtFrac5SmallR20") == std::string::npos) m_calibSequence += "_Insitu";

  if( m_isMC && m_calibSequence.find("Insitu") != std::string::npos){
    Error("initialize()", "Attempting to use an Insitu calibration sequence on MC.  Exiting.");
    return EL::StatusCode::FAILURE;
  }

  if ( !m_isMC ) {
    m_calibConfig = m_calibConfigData;
  }else{
    m_calibConfig = m_calibConfigFullSim;
    // treat as fullsim by default
    m_isFullSim = true;
    // Check simulation flavour for calibration config - cannot directly read metadata in xAOD otside of Athena!
    //
    // N.B.: With SampleHandler, you can define sample metadata in job steering macro!
    //
    //       They will be passed to the EL:;Worker automatically and can be retrieved anywhere in the EL::Algorithm
    //       I reasonably suppose everyone will use SH...
    //
    //       IMPORTANT! the metadata name set in SH *must* be "AFII" (if not set, name will be *empty_string*)

    //
    const std::string stringMeta = wk()->metaData()->castString("SimulationFlavour");
    if ( m_setAFII ) {
      Info("initialize()", "Setting simulation flavour to AFII");
      m_isFullSim = false;
    }else if ( stringMeta.empty() ) {
      Warning("initialize()", "Could not access simulation flavour from EL::Worker. Treating MC as FullSim by default!" );
    } else {
      m_isFullSim = (stringMeta == "AFII") ? false : true;
    }
    if ( !m_isFullSim ) {
      m_calibConfig = m_calibConfigAFII;
    }
  }

  // initialize jet calibration tool
  std::string jcal_tool_name = std::string("JetCorrectionTool_") + m_name;
  m_jetCalibration = new JetCalibrationTool(jcal_tool_name.c_str(),
      m_jetAlgo,
      m_calibConfig,
      m_calibSequence,
      !m_isMC);
  m_jetCalibration->msg().setLevel( MSG::INFO); // VERBOSE, INFO, DEBUG
  RETURN_CHECK( "JetCalibrator::initialize()", m_jetCalibration->initializeTool( jcal_tool_name.c_str() ), "JetCalibrator Interface succesfully initialized!");

  // initialize and configure the jet cleaning tool
  //------------------------------------------------
  std::string jc_tool_name = std::string("JetCleaning_") + m_name;
  m_jetCleaning = new JetCleaningTool( jc_tool_name.c_str() );
  RETURN_CHECK( "JetCalibrator::initialize()", m_jetCleaning->setProperty( "CutLevel", m_jetCleanCutLevel), "");
  if (m_jetCleanUgly){
    RETURN_CHECK( "JetCalibrator::initialize()", m_jetCleaning->setProperty( "DoUgly", true), "");
  }
  RETURN_CHECK( "JetCalibrator::initialize()", m_jetCleaning->initialize(), "JetCleaning Interface succesfully initialized!");

  if( m_saveAllCleanDecisions ){
    //std::string m_decisionNames[] = {"LooseBad", TightBad"};
    m_decisionNames.push_back( "LooseBad" );
    m_decisionNames.push_back( "LooseBadUgly" );
    m_decisionNames.push_back( "TightBad" );
    m_decisionNames.push_back( "TightBadUgly" );
    for(unsigned int i=0; i < m_decisionNames.size() ; ++i){
      m_allJetCleaningTools.push_back( new JetCleaningTool((jc_tool_name+"_pass"+m_decisionNames.at(i)).c_str()) );
      if( m_decisionNames.at(i).find("Ugly") != std::string::npos ){
        std::cout << "adding for " << m_decisionNames.at(i).substr(0,m_decisionNames.at(i).size()-4) << std::endl;
        RETURN_CHECK( "JetCalibrator::initialize()", m_allJetCleaningTools.at( i )->setProperty( "CutLevel", m_decisionNames.at(i).substr(0,m_decisionNames.at(i).size()-4) ), "");
        RETURN_CHECK( "JetCalibrator::initialize()", m_allJetCleaningTools.at( i )->setProperty( "DoUgly", true ), "");
      }else{
        RETURN_CHECK( "JetCalibrator::initialize()", m_allJetCleaningTools.at( i )->setProperty( "CutLevel", m_decisionNames.at(i)), "");
      }
      RETURN_CHECK( "JetCalibrator::initialize()", m_allJetCleaningTools.at( i )->initialize(), ("JetCleaning Interface "+m_decisionNames.at(i)+" succesfully initialized!").c_str());
    }
  }

  //
  // Get a list of recommended systematics for this tool
  //
  const CP::SystematicSet recSyst = CP::SystematicSet();
  Info("initialize()"," Initializing Jet Calibrator Systematics :");

  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, m_debug );
  for(unsigned int i=0; i<m_systList.size(); i++)
    m_systType.insert(m_systType.begin(), 0); // Push systType nominal for this case

  Info("initialize()","Will be using JetCalibrationTool systematic:");

  // initialize and configure the jet uncertainity tool
  // only initialize if a config file has been given
  //------------------------------------------------
  if ( !m_JESUncertConfig.empty() && !m_systName.empty()  && m_systName != "None" ) {
    m_JESUncertConfig = gSystem->ExpandPathName( m_JESUncertConfig.c_str() );
    Info("initialize()","Initialize JES UNCERT with %s", m_JESUncertConfig.c_str());
    std::string ju_tool_name = std::string("JESProvider_") + m_name;
    m_JESUncertTool = new JetUncertaintiesTool( ju_tool_name.c_str() );
    RETURN_CHECK("JetCalibrator::initialize()", m_JESUncertTool->setProperty("JetDefinition",m_jetAlgo), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_JESUncertTool->setProperty("MCType",m_JESUncertMCType), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_JESUncertTool->setProperty("ConfigFile", m_JESUncertConfig), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_JESUncertTool->initialize(), "");
    m_JESUncertTool->msg().setLevel( MSG::ERROR ); // VERBOSE, INFO, DEBUG
    const CP::SystematicSet recSysts = m_JESUncertTool->recommendedSystematics();

    Info("initialize()"," Initializing Jet Systematics :");

    //If just one systVal, then push it to the vector
    if( m_systValVector.size() == 0) {
      if ( m_debug ){ Info("initialize()", "Pushing the following systVal to m_systValVector: %f", m_systVal ); }
      m_systValVector.push_back(m_systVal);
    }

    for(unsigned int iSyst=0; iSyst < m_systValVector.size(); ++iSyst){
      m_systVal = m_systValVector.at(iSyst);
      std::vector<CP::SystematicSet> JESSysList = HelperFunctions::getListofSystematics( recSysts, m_systName, m_systVal, m_debug );

      for(unsigned int i=0; i < JESSysList.size(); ++i){
        // do not add another nominal syst to the list!!
        // CP::SystematicSet() creates an empty systematic set, compared to the set at index i
        if (JESSysList.at(i).empty() || JESSysList.at(i) == CP::SystematicSet() ) { Info("initialize()","JESSysList Empty at index %d.",i); continue; }
        m_systList.push_back(  JESSysList.at(i) );
        m_systType.push_back(1);
      }
    }

    // Setup the tool for the 1st systematic on the list
    // If running all, the tool will be setup for each syst on each event
    if ( !m_systList.empty() ) {
      m_runSysts = true;
      // setup uncertainity tool for systematic evaluation
      if ( m_JESUncertTool->applySystematicVariation(m_systList.at(0)) != CP::SystematicCode::Ok ) {
        Error("initialize()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
        return EL::StatusCode::FAILURE;
      }
    }
  } // running systematics
  else {
    Info("initialize()", "No JES Uncertainities considered");
    // m_JESUncertTool not streamed so have to do this
    m_JESUncertTool = nullptr;
  }

  // initialize and configure the JET Smearing tool
  if ( !m_JERUncertConfig.empty() ) {

    // Instantiate the tools
    std::string JERTool_name  = std::string("JERTool_") + m_name;
    std::string JERSmearingTool_name = std::string("JERSmearingTool_") + m_name;
    m_JERTool     = new JERTool( JERTool_name.c_str() );
    m_JERSmearTool = new JERSmearingTool( JERSmearingTool_name.c_str() );

    // Configure the JERTool
    //m_JERTool->msg().setLevel(MSG::DEBUG);
    RETURN_CHECK( "initialize()", m_JERTool->setProperty("PlotFileName", m_JERUncertConfig.c_str()), "");
    RETURN_CHECK( "initialize()", m_JERTool->setProperty("CollectionName", m_jetAlgo), "");

    //m_JERSmearTool->msg().setLevel(MSG::DEBUG);
    m_JERToolHandle = ToolHandle<IJERTool>(m_JERTool->name());
    RETURN_CHECK( "initialize()", m_JERSmearTool->setProperty("JERTool", m_JERToolHandle), "");

    RETURN_CHECK( "initialize()", m_JERSmearTool->setProperty("isMC", m_isMC), "");

    //m_JERApplyNominal = true;
    RETURN_CHECK( "initialize()", m_JERSmearTool->setProperty("ApplyNominalSmearing", m_JERApplyNominal), "");

    //m_JERFullSys = true;
    if( m_JERFullSys )
      RETURN_CHECK( "initialize()", m_JERSmearTool->setProperty("SystematicMode", "Full"), "");
    else
      RETURN_CHECK( "initialize()", m_JERSmearTool->setProperty("SystematicMode", "Simple"), "");


    RETURN_CHECK( "initialize()", m_JERTool->initialize(), "Failed to properly initialize the JER Tool");
    RETURN_CHECK( "initialize()", m_JERSmearTool->initialize(), "Failed to properly initialize the JERSmearTool Tool");

    const CP::SystematicSet recSysts = m_JERSmearTool->recommendedSystematics();
    Info("initialize()", " Initializing JER Systematics :");

    std::vector<CP::SystematicSet> JERSysList = HelperFunctions::getListofSystematics( recSysts, m_systName, 1, m_debug ); //Only 1 sys allowed
    for(unsigned int i=0; i < JERSysList.size(); ++i){
      // do not add another nominal syst to the list!!
      // CP::SystematicSet() creates an empty systematic set, compared to the set at index i
      if (JERSysList.at(i).empty() || JERSysList.at(i) == CP::SystematicSet() ) { Info("initialize()","JERSysList Empty at index %d.",i); continue; }
      m_systList.push_back(  JERSysList.at(i) );
      m_systType.push_back(2);
    }

    if (!m_systList.empty()){
      m_runSysts = true;
    }

  }
  else {
    Info("initialize()", "No JER Uncertainities considered");
    // m_JERSmearTool not streamed so have to do this
    m_JERSmearTool = nullptr;
  }

  // initialize and configure the JVT correction tool
  if(m_redoJVT){
    m_JVTTool = new JetVertexTaggerTool("jvtag");
    m_JVTToolHandle = ToolHandle<IJetUpdateJvt>("jvtag");
    RETURN_CHECK("JetCalibrator::initialize()", m_JVTTool->setProperty("JVTFileName","JetMomentTools/JVTlikelihood_20140805.root"), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_JVTTool->initialize(), "");
  }

  for ( const auto& syst_it : m_systList ) {
    if ( m_systName.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Jet Calibration and Cleaning... "); }

  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::JetContainer* inJets(nullptr);
  RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, m_verbose) ,"");

  // loop over available systematics - remember syst == "Nominal" --> baseline
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

  //std::vector< int >
  for ( const auto& syst_it : m_systList ) {
    unsigned int sysIndex = (&syst_it - &m_systList[0]);
    int thisSysType = m_systType.at(sysIndex);

    std::string outSCContainerName(m_outSCContainerName);
    std::string outSCAuxContainerName(m_outSCAuxContainerName);
    std::string outContainerName(m_outContainerName);

    // always append the name of the variation, including nominal which is an empty string
    outSCContainerName    += syst_it.name();
    outSCAuxContainerName += syst_it.name();
    outContainerName      += syst_it.name();
    vecOutContainerNames->push_back( syst_it.name() );

    // create shallow copy;
    std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > calibJetsSC = xAOD::shallowCopyContainer( *inJets );
    ConstDataVector<xAOD::JetContainer>* calibJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
    calibJetsCDV->reserve( calibJetsSC.first->size() );

    // Nominal calibration for all inputs
    for ( auto jet_itr : *(calibJetsSC.first) ) {
      m_numObject++;

      if ( m_jetCalibration->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
        Error("execute()", "JetCalibration tool reported a CP::CorrectionCode::Error");
        Error("execute()", "%s", m_name.c_str());
        return StatusCode::FAILURE;
      }
    }//for jets

    //Apply Uncertainties
    if ( m_runSysts ) {
      if ( thisSysType == 1 ){
        // JES Uncertainty Systematic
        if( m_debug ) { std::cout << "Configure JES for systematic variation : " << syst_it.name() << std::endl; }
        if ( m_JESUncertTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
          Error("execute()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
          return EL::StatusCode::FAILURE;
        }
        
	for ( auto jet_itr : *(calibJetsSC.first) ) {
          if ( m_runSysts ) {

	    if (m_applyFatJetPreSel) {
	      bool validForJES = (jet_itr->pt() >= 150e3 && jet_itr->pt() < 3000e3);
	      validForJES &= (jet_itr->m()/jet_itr->pt() >= 0 && jet_itr->m()/jet_itr->pt() < 1);
	      validForJES &= (fabs(jet_itr->eta()) < 2);
	      if (!validForJES) continue;
	    }	    

            if ( m_JESUncertTool->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
              Error("execute()", "JetUncertaintiesTool reported a CP::CorrectionCode::Error");
              Error("execute()", "%s", m_name.c_str());
            }
          }
        }//for jets
      }//JES

      if ( thisSysType == 2 || m_JERApplyNominal){
        if( m_debug ) { std::cout << "Configure JER for systematic variation : " << syst_it.name() << std::endl; }
        if( thisSysType == 2){ //apply this systematic
          if ( m_JERSmearTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
            Error("execute()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
            return EL::StatusCode::FAILURE;
          }
        }else{ //apply nominal, which is always first element of m_systList
          if ( m_JERSmearTool->applySystematicVariation(m_systList.at(0)) != CP::SystematicCode::Ok ) {
            Error("execute()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
            return EL::StatusCode::FAILURE;
          }
        }
        // JER Uncertainty Systematic
        for ( auto jet_itr : *(calibJetsSC.first) ) {
          if ( m_JERSmearTool->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
            Error("execute()", "JERSmearTool tool reported a CP::CorrectionCode::Error");
            Error("execute()", "%s", m_name.c_str());
          }
        }//for jets
      }//JER

    }// if m_runSysts

    // decorate with cleaning decision
    for ( auto jet_itr : *(calibJetsSC.first) ) {

      static SG::AuxElement::Decorator< char > isCleanDecor( "cleanJet" );
      const xAOD::Jet* jetToClean = jet_itr;

      if(m_cleanParent){
        ElementLink<xAOD::JetContainer> el_parent = jet_itr->auxdata<ElementLink<xAOD::JetContainer> >("Parent") ;
        if(!el_parent.isValid()){
          Error("jetDecision()", "Could not make jet cleaning decision on the parent! It doesn't exist.");
        } else {
          jetToClean = *el_parent;
        }
      }

      isCleanDecor(*jet_itr) = m_jetCleaning->accept(*jetToClean);

      if( m_saveAllCleanDecisions ){
        for(unsigned int i=0; i < m_allJetCleaningTools.size() ; ++i){
          jet_itr->auxdata< char >(("clean_pass"+m_decisionNames.at(i)).c_str()) = m_allJetCleaningTools.at(i)->accept(*jetToClean);
        }
      }
    } //end cleaning decision

    if ( !xAOD::setOriginalObjectLink(*inJets, *(calibJetsSC.first)) ) {
      Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    // Recalculate JVT using calibrated Jets
    if(m_redoJVT){
      for ( auto jet_itr : *(calibJetsSC.first) ) {
        jet_itr->auxdata< float >("Jvt") = m_JVTToolHandle->updateJvt(*jet_itr);
      }
    }

    // save pointers in ConstDataVector with same order
    for ( auto jet_itr : *(calibJetsSC.first) ) {
      calibJetsCDV->push_back( jet_itr );
    }

    // can only sort the CDV - a bit no-no to sort the shallow copies
    if ( m_sort ) {
      std::sort( calibJetsCDV->begin(), calibJetsCDV->end(), HelperFunctions::sort_pt );
    }

    // add shallow copy to TStore
    RETURN_CHECK( "JetCalibrator::execute()", m_store->record( calibJetsSC.first, outSCContainerName), "Failed to record shallow copy container.");
    RETURN_CHECK( "JetCalibrator::execute()", m_store->record( calibJetsSC.second, outSCAuxContainerName), "Failed to record shallow copy aux container.");

    // add ConstDataVector to TStore
    RETURN_CHECK( "JetCalibrator::execute()", m_store->record( calibJetsCDV, outContainerName), "Failed to record const data container.");
  }
  // add vector of systematic names to TStore
  RETURN_CHECK( "JetCalibrator::execute()", m_store->record( vecOutContainerNames, m_outputAlgo), "Failed to record vector of output container names.");

  // look what do we have in TStore

  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: finalize ()
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

  Info("finalize()", "Deleting tool instances...");

  if ( m_jetCalibration ) {
    delete m_jetCalibration; m_jetCalibration = nullptr;
  }
  if ( m_jetCleaning ) {
    delete m_jetCleaning; m_jetCleaning = nullptr;
  }
  if ( m_JESUncertTool ) {
    delete m_JESUncertTool; m_JESUncertTool = nullptr;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: histFinalize ()
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

  Info("histFinalize()", "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
