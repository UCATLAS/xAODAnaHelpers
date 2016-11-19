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

// ROOT includes:
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetCalibrator)

JetCalibrator :: JetCalibrator (std::string className) :
    Algorithm(className),
    m_runSysts(false),          // gets set later is syst applies to this tool
    m_JetCalibrationTool_handle("JetCalibrationTool/JetCalibrationTool_"+className),
    m_JetUncertaintiesTool_handle("JetUncertaintiesTool/JetUncertaintiesTool_"+className),
    m_JERTool_handle("JERTool/JERTool_"+className),
    m_JERSmearingTool_handle("JERSmearingTool/JERSmearingTool_"+className),
    m_JVTUpdateTool_handle("JetVertexTaggerTool/JVTUpdateTool_"+className),
    m_JetCleaningTool_handle("JetCleaningTool/JetCleaningTool_"+className)
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

  m_doCleaning              = true;
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
  m_systVal                 = 1.;
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

  // If there is no InputContainer we must stop
  if ( m_inContainerName.empty() ) {
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  if ( m_outputAlgo.empty() ) {
    m_outputAlgo = m_jetAlgo + "_Calib_Algo";
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
  m_JetCalibrationTool_handle.setTypeAndName("JetCalibrationTool/JetCalibrationTool_" + m_name);
  if( !m_JetCalibrationTool_handle.isUserConfigured() ){
    RETURN_CHECK("JetCalibrator::initialize()", ASG_MAKE_ANA_TOOL(m_JetCalibrationTool_handle, JetCalibrationTool), "Could not make JetCalibrationTool");

    RETURN_CHECK("JetCalibrator::initialize()", m_JetCalibrationTool_handle.setProperty("JetCollection",m_jetAlgo), "Failed to set JetCollection");
    RETURN_CHECK("JetCalibrator::initialize()", m_JetCalibrationTool_handle.setProperty("ConfigFile",m_calibConfig), "Failed to set ConfigFile");
    RETURN_CHECK("JetCalibrator::initialize()", m_JetCalibrationTool_handle.setProperty("CalibSequence",m_calibSequence), "Failed to set CalibSequence");
    RETURN_CHECK("JetCalibrator::initialize()", m_JetCalibrationTool_handle.setProperty("IsData",!m_isMC), "Failed to set IsData");

    RETURN_CHECK("JetCalibrator::initialize()", m_JetCalibrationTool_handle.retrieve(), "Failed to retrieve JetCalibrationTool");
  }

  if(m_doCleaning){
    // initialize and configure the jet cleaning tool
    //------------------------------------------------

    m_JetCleaningTool_handle.setTypeAndName("JetCleaningTool/JetCleaningTool_" + m_name);
    if( !m_JetCleaningTool_handle.isUserConfigured() ){
      RETURN_CHECK("JetCalibrator::initialize()", ASG_MAKE_ANA_TOOL(m_JetCleaningTool_handle, JetCleaningTool), "Could not make JetCleaningTool");

      RETURN_CHECK( "JetCalibrator::initialize()", m_JetCleaningTool_handle.setProperty( "CutLevel", m_jetCleanCutLevel), "Failed to set CutLevel");
      if (m_jetCleanUgly){
        RETURN_CHECK( "JetCalibrator::initialize()", m_JetCleaningTool_handle.setProperty( "DoUgly", true), "Failed to set DoUgly");
      }

      RETURN_CHECK("JetCalibrator::initialize()", m_JetCleaningTool_handle.retrieve(), "Failed to retrieve JetCleaningTool");
    }

    if( m_saveAllCleanDecisions ){
      m_decisionNames.push_back( "LooseBad" );
      m_decisionNames.push_back( "LooseBadUgly" );
      m_decisionNames.push_back( "TightBad" );
      m_decisionNames.push_back( "TightBadUgly" );

      for(unsigned int iD=0; iD < m_decisionNames.size() ; ++iD){
        asg::AnaToolHandle<IJetSelector> this_JetCleaningTool_handle;
        this_JetCleaningTool_handle.setTypeAndName("JetCleaningTool/JetCleaningTool_"+m_decisionNames.at(iD)+"_"+m_name);
        if( !this_JetCleaningTool_handle.isUserConfigured() ){
          RETURN_CHECK("JetCalibrator::initialize()", ASG_MAKE_ANA_TOOL(this_JetCleaningTool_handle, JetCleaningTool), "Could not make JetCleaningTool");

          if( m_decisionNames.at(iD).find("Ugly") != std::string::npos ){
            RETURN_CHECK( "JetCalibrator::initialize()", this_JetCleaningTool_handle.setProperty( "CutLevel", m_decisionNames.at(iD).substr(0,m_decisionNames.at(iD).size()-4) ), "");
            RETURN_CHECK( "JetCalibrator::initialize()", this_JetCleaningTool_handle.setProperty( "DoUgly", true), "");
          }else{
            RETURN_CHECK( "JetCalibrator::initialize()", this_JetCleaningTool_handle.setProperty( "CutLevel", m_decisionNames.at(iD) ), "Failed to set CutLevel");
          }

          RETURN_CHECK("JetCalibrator::initialize()", this_JetCleaningTool_handle.retrieve(), "Failed to retrieve JetCleaningTool");
        }
        m_AllJetCleaningTool_handles.push_back( this_JetCleaningTool_handle );
      }// For each cleaning decision
    }//If save all cleaning decisions
  }// if m_doCleaning

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

    m_JetUncertaintiesTool_handle.setTypeAndName("JetUncertaintiesTool/JetUncertaintiesTool_" + m_name);
    if( !m_JetUncertaintiesTool_handle.isUserConfigured() ){

      RETURN_CHECK("JetCalibrator::initialize()", ASG_MAKE_ANA_TOOL(m_JetUncertaintiesTool_handle, JetUncertaintiesTool), "Could not make JetUncertaintiesTool");

      m_JESUncertConfig = gSystem->ExpandPathName( m_JESUncertConfig.c_str() );
      Info("JetCalibrator::initialize()","Initialize JES UNCERT with %s", m_JESUncertConfig.c_str());
      RETURN_CHECK("JetCalibrator::initialize()", m_JetUncertaintiesTool_handle.setProperty("JetDefinition",m_jetAlgo), "Failed to set JetDefinition");
      RETURN_CHECK("JetCalibrator::initialize()", m_JetUncertaintiesTool_handle.setProperty("MCType",m_JESUncertMCType), "Failed to set MCType");
      RETURN_CHECK("JetCalibrator::initialize()", m_JetUncertaintiesTool_handle.setProperty("ConfigFile", m_JESUncertConfig), "Failed to set ConfigFile");

      RETURN_CHECK("JetCalibrator::initialize()", m_JetUncertaintiesTool_handle.retrieve(), "Failed to retrieve JetUncertaintiesTool");

//      m_JetUncertaintiesTool_handle->msg().setLevel( MSG::ERROR ); // VERBOSE, INFO, DEBUG
    }


    Info("initialize()"," Initializing Jet Systematics :");
    const CP::SystematicSet recSysts = m_JetUncertaintiesTool_handle->recommendedSystematics();

    //If just one systVal, then push it to the vector
    RETURN_CHECK("JetCalibrator::initialize()", this->parseSystValVector(), "Failed to parse vector of systematic sigma values.");
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
      if ( m_JetUncertaintiesTool_handle->applySystematicVariation(m_systList.at(0)) != CP::SystematicCode::Ok ) {
        Error("JetCalibrator:initialize()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
        return EL::StatusCode::FAILURE;
      }
    }
  } // running systematics
  else {
    Info("initialize()", "No JES Uncertainities considered");
  }

  // initialize and configure the JET Smearing tool
  if ( !m_JERUncertConfig.empty() ) {

    // Instantiate the JER tool
    m_JERTool_handle.setTypeAndName("JERTool/JERTool_" + m_name);
    if( !m_JERTool_handle.isUserConfigured() ){
      RETURN_CHECK("JetCalibrator::initialize()", ASG_MAKE_ANA_TOOL(m_JERTool_handle, JERTool), "Could not make JERTool");

      RETURN_CHECK("JetCalibrator::initialize()", m_JERTool_handle.setProperty("PlotFileName", m_JERUncertConfig.c_str()), "");
      RETURN_CHECK("JetCalibrator::initialize()", m_JERTool_handle.setProperty("CollectionName", m_jetAlgo), "");

      RETURN_CHECK("JetCalibrator::initialize()", m_JERTool_handle.retrieve(), "Failed to retrieve JERTool");
    }

    // Instantiate the JER Smearing tool
    m_JERSmearingTool_handle.setTypeAndName("JERSmearingTool/JERSmearingTool_" + m_name);
    if( !m_JERSmearingTool_handle.isUserConfigured() ){
      RETURN_CHECK("JetCalibrator::initialize()", ASG_MAKE_ANA_TOOL(m_JERSmearingTool_handle, JERSmearingTool), "Could not make JERSmearingTool");

      RETURN_CHECK( "JetCalibrator::initialize()", m_JERSmearingTool_handle.setProperty("JERTool", m_JERTool_handle.getHandle()), "Failed to set JERTool");
      RETURN_CHECK( "JetCalibrator::initialize()", m_JERSmearingTool_handle.setProperty("isMC", m_isMC), "Failed to set isMC");
      RETURN_CHECK( "JetCalibrator::initialize()", m_JERSmearingTool_handle.setProperty("ApplyNominalSmearing", m_JERApplyNominal), "Failed to set ApplyNominalSmearing");
      if( m_JERFullSys )
        RETURN_CHECK( "JetCalibrator::initialize()", m_JERSmearingTool_handle.setProperty("SystematicMode", "Full"), "Failed to set SystematicMode");
      else
        RETURN_CHECK( "JetCalibrator::initialize()", m_JERSmearingTool_handle.setProperty("SystematicMode", "Simple"), "Failed to set SystematicMode");

      RETURN_CHECK("JetCalibrator::initialize()", m_JERSmearingTool_handle.retrieve(), "Failed to retrieve JERSmearingTool");
    }



    const CP::SystematicSet recSysts = m_JERSmearingTool_handle->recommendedSystematics();
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
  }

  // initialize and configure the JVT correction tool

  m_JVTUpdateTool_handle.setTypeAndName("JetVertexTaggerTool/JVTUpdateTool_" + m_name);
  if( m_redoJVT && !m_JVTUpdateTool_handle.isUserConfigured() ){
    RETURN_CHECK( "JetCalibrator::initialize()", ASG_MAKE_ANA_TOOL(m_JVTUpdateTool_handle, JetVertexTaggerTool), "Could not make the tool");
    RETURN_CHECK("JetCalibrator::initialize()", m_JVTUpdateTool_handle.setProperty("JVTFileName","JetMomentTools/JVTlikelihood_20140805.root"), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_JVTUpdateTool_handle.retrieve(), "Failed to retrieve JVTUpdateTool");

  }

  std::vector< std::string >* SystJetsNames = new std::vector< std::string >;
  for ( const auto& syst_it : m_systList ) {
    if ( m_systName.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    SystJetsNames->push_back(syst_it.name());
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  RETURN_CHECK("JetCalibrator::initialize()",m_store->record(SystJetsNames, "jets_Syst"+m_name ), "Failed to record vector of jet systs names.");


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

  //
  // Perform nominal calibration
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > calibJetsSC = xAOD::shallowCopyContainer( *inJets );
  ConstDataVector<xAOD::JetContainer>* calibJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  calibJetsCDV->reserve( calibJetsSC.first->size() );

  std::string outSCContainerName=m_outContainerName+"ShallowCopy";
  std::string outSCAuxContainerName=m_outContainerName+"ShallowCopyAux.";
  RETURN_CHECK( "JetCalibrator::execute()", m_store->record( calibJetsSC.first,  outSCContainerName),    "Failed to record shallow copy container.");
  RETURN_CHECK( "JetCalibrator::execute()", m_store->record( calibJetsSC.second, outSCAuxContainerName), "Failed to record shallow copy aux container.");

  for ( auto jet_itr : *(calibJetsSC.first) ) {
    m_numObject++;

    //Set isBjet for JES Systematics
    if(m_isMC && m_runSysts){

      int this_TruthLabel = 0;

      static SG::AuxElement::ConstAccessor<int> TruthLabelID ("TruthLabelID");
      static SG::AuxElement::ConstAccessor<int> PartonTruthLabelID ("PartonTruthLabelID");

      if ( TruthLabelID.isAvailable( *jet_itr) ) {
  this_TruthLabel = TruthLabelID( *jet_itr );
  if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
      } else if(PartonTruthLabelID.isAvailable( *jet_itr) ) {
  this_TruthLabel = PartonTruthLabelID( *jet_itr );
  if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
      }

      bool isBjet = false; // decide whether or not the jet is a b-jet (truth-labelling + kinematic selections)
      if (this_TruthLabel == 5) isBjet = true;
      static SG::AuxElement::Decorator<char> accIsBjet("IsBjet"); // char due to limitations of ROOT I/O, still treat it as a bool
      accIsBjet(*jet_itr) = isBjet;
    }



    if ( m_JetCalibrationTool_handle->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
      Error("execute()", "JetCalibration tool reported a CP::CorrectionCode::Error");
      Error("execute()", "%s", m_name.c_str());
      return StatusCode::FAILURE;
    }
  }//for jets

  // loop over available systematics - remember syst == "Nominal" --> baseline
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

  //std::vector< int >
  for ( const auto& syst_it : m_systList ) {
    unsigned int sysIndex = (&syst_it - &m_systList[0]);
    int thisSysType = m_systType.at(sysIndex);

    // always append the name of the variation, including nominal which is an empty string
    outSCContainerName   =m_outContainerName+syst_it.name()+"ShallowCopy";
    outSCAuxContainerName=m_outContainerName+syst_it.name()+"ShallowCopyAux.";
    std::string outContainerName=m_outContainerName+syst_it.name();

    vecOutContainerNames->push_back( syst_it.name() );

    // create shallow copy;
    std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > uncertCalibJetsSC = (thisSysType==0)? calibJetsSC : xAOD::shallowCopyContainer( *calibJetsSC.first );
    ConstDataVector<xAOD::JetContainer>* uncertCalibJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
    uncertCalibJetsCDV->reserve( uncertCalibJetsSC.first->size() );

    //Apply Uncertainties
    if ( m_runSysts ) {
      if ( thisSysType == 1 ){
        // JES Uncertainty Systematic
        if( m_debug ) { std::cout << "Configure JES for systematic variation : " << syst_it.name() << std::endl; }
        if ( m_JetUncertaintiesTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
          Error("execute()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
          return EL::StatusCode::FAILURE;
        }

        for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {
          if (m_applyFatJetPreSel) {
            bool validForJES = (jet_itr->pt() >= 150e3 && jet_itr->pt() < 3000e3);
            validForJES &= (jet_itr->m()/jet_itr->pt() >= 0 && jet_itr->m()/jet_itr->pt() < 1);
            validForJES &= (fabs(jet_itr->eta()) < 2);
            if (!validForJES) continue;
          }

          if ( m_JetUncertaintiesTool_handle->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
            Error("execute()", "JetUncertaintiesTool reported a CP::CorrectionCode::Error");
            Error("execute()", "%s", m_name.c_str());
          }
        }//for jets
      }//JES

      if ( thisSysType == 2 || m_JERApplyNominal){
        if( m_debug ) { std::cout << "Configure JER for systematic variation : " << syst_it.name() << std::endl; }
        if( thisSysType == 2){ //apply this systematic
          if ( m_JERSmearingTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
            Error("execute()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
            return EL::StatusCode::FAILURE;
          }
        }else{ //apply nominal, which is always first element of m_systList
          if ( m_JERSmearingTool_handle->applySystematicVariation(m_systList.at(0)) != CP::SystematicCode::Ok ) {
            Error("execute()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
            return EL::StatusCode::FAILURE;
          }
        }
        // JER Uncertainty Systematic
        for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {
          if ( m_JERSmearingTool_handle->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
            Error("execute()", "JERSmearTool tool reported a CP::CorrectionCode::Error");
            Error("execute()", "%s", m_name.c_str());
          }
        }//for jets
      }//JER

    }// if m_runSysts

    if(m_doCleaning){
      // decorate with cleaning decision
      for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {

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

        isCleanDecor(*jet_itr) = m_JetCleaningTool_handle->keep(*jetToClean);

        if( m_saveAllCleanDecisions ){
          for(unsigned int i=0; i < m_AllJetCleaningTool_handles.size() ; ++i){
            jet_itr->auxdata< char >(("clean_pass"+m_decisionNames.at(i)).c_str()) = m_AllJetCleaningTool_handles.at(i)->keep(*jetToClean);
          }
        }
      } //end cleaning decision
    }

    if ( !xAOD::setOriginalObjectLink(*inJets, *(uncertCalibJetsSC.first)) ) {
      Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    // Recalculate JVT using calibrated Jets
    if(m_redoJVT){
      for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {
        jet_itr->auxdata< float >("Jvt") = m_JVTUpdateTool_handle->updateJvt(*jet_itr);
      }
    }

    // save pointers in ConstDataVector with same order
    for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {
      uncertCalibJetsCDV->push_back( jet_itr );
    }

    // can only sort the CDV - a bit no-no to sort the shallow copies
    if ( m_sort ) {
      std::sort( uncertCalibJetsCDV->begin(), uncertCalibJetsCDV->end(), HelperFunctions::sort_pt );
    }

    // add shallow copy to TStore
    if(thisSysType!=0) { // nominal is always saved outside of loop
      RETURN_CHECK( "JetCalibrator::execute()", m_store->record( uncertCalibJetsSC.first, outSCContainerName), "Failed to record shallow copy container.");
      RETURN_CHECK( "JetCalibrator::execute()", m_store->record( uncertCalibJetsSC.second, outSCAuxContainerName), "Failed to record shallow copy aux container.");
    }

    // add ConstDataVector to TStore
    RETURN_CHECK( "JetCalibrator::execute()", m_store->record( uncertCalibJetsCDV, outContainerName), "Failed to record const data container.");
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
