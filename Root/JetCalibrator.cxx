/******************************************
 *
 * Interface to Jet calibration tool(s).
 *
 * G.Facini (gabriel.facini@cern.ch), M. Milesi (marco.milesi@cern.ch)
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


JetCalibrator :: JetCalibrator () :
  m_runSysts(false),          // gets set later is syst applies to this tool
  m_jetCalibration(nullptr),  // JetCalibrationTool
  m_jetCleaning(nullptr),     // JetCleaningTool
  m_jetUncert(nullptr)        // JetUncertaintiesTool
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
  // input container to be read from TEvent or TStore
  m_inContainerName         = "";

  // DC14 switch for little things that need to happen to run
  // for those samples with the corresponding packages
  m_DC14                    = false;

  // CONFIG parameters for JetCalibrationTool
  m_jetAlgo                 = "";
  m_outputAlgo              = "";

  // when running data "_Insitu" is appended to this string
  m_calibSequence           = "JetArea_Residual_Origin_EtaJES_GSC";
  m_calibConfigFullSim      = "JES_MC15Prerecommendation_April2015.config";
  m_calibConfigAFII         = "JES_Prerecommendation2015_AFII_Apr2015.config";
  m_calibConfigData         = "JES_MC15Prerecommendation_April2015.config";

  // CONFIG parameters for JetCleaningTool
  m_jetCalibCutLevel        = "MediumBad";
  m_saveAllCleanDecisions   = false;

  // CONFIG parameters for JetUncertaintiesTool
  m_uncertConfig            = "";
  // calibrator uses TopoEM or TopoLC while the uncertainity tool uses EMTopo and LCTopo
  // calibrator should switch at some point
  // "fix" the name here so the user never knows the difference

  // shallow copies are made with this output container name
  m_outContainerName        = "";
  m_sort                    = true;

}

EL::StatusCode  JetCalibrator :: configure ()
{

  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing JetCalibrator Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug                   = config->GetValue("Debug" , m_debug);
    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());

    // DC14 switch for little things that need to happen to run
    // for those samples with the corresponding packages
    m_DC14                    = config->GetValue("DC14", m_DC14);

    // CONFIG parameters for JetCalibrationTool
    m_jetAlgo                 = config->GetValue("JetAlgorithm", m_jetAlgo.c_str());
    m_outputAlgo              = config->GetValue("OutputAlgo",   m_outputAlgo.c_str());

    // when running data "_Insitu" is appended to this string
    m_calibSequence           = config->GetValue("CalibSequence",           m_calibSequence.c_str());
    m_calibConfigFullSim      = config->GetValue("configNameFullSim",       m_calibConfigFullSim.c_str());
    m_calibConfigAFII         = config->GetValue("configNameAFII",          m_calibConfigAFII.c_str());
    m_calibConfigData         = config->GetValue("configNameData",          m_calibConfigData.c_str());

    // CONFIG parameters for JetCleaningTool
    m_jetCalibCutLevel        = config->GetValue("JetCalibCutLevel",        m_jetCalibCutLevel.c_str());
    m_saveAllCleanDecisions   = config->GetValue("SaveAllCleanDecisions",   m_saveAllCleanDecisions);

    // CONFIG parameters for JetUncertaintiesTool
    m_uncertConfig            = config->GetValue("JetUncertConfig", m_uncertConfig.c_str());
    // calibrator uses TopoEM or TopoLC while the uncertainity tool uses EMTopo and LCTopo
    // calibrator should switch at some point
    // "fix" the name here so the user never knows the difference

    // shallow copies are made with this output container name
    m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());
    m_sort                    = config->GetValue("Sort",            m_sort);

    config->Print();
    Info("configure()", "JetCalibrator Interface succesfully configured! ");

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

  m_jetUncertAlgo = m_jetAlgo;
  if(m_DC14) {
    m_jetUncertAlgo = HelperFunctions::replaceString(m_jetUncertAlgo, std::string("TopoEM"), std::string("EMTopo"));
    m_jetUncertAlgo = HelperFunctions::replaceString(m_jetUncertAlgo, std::string("TopoLC"), std::string("LCTopo"));
  }

  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

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

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_debug) ,"");
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  //For DC14 only when running data "_Insitu" is appended to the calibration string!
  if ( !m_isMC && m_calibSequence.find("Insitu") == std::string::npos) m_calibSequence += "_Insitu";

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
    // N.B. (Marco) : With SampleHandler, you can define sample metadata in job steering macro!
    //                They will be passed to the EL:;Worker automatically and can be retrieved anywhere in the EL::Algorithm
    //                I reasonably suppose everyone will use SH...
    //
    const std::string stringMeta = wk()->metaData()->castString("SimulationFlavour"); // NB: needs to be defined as sample metadata in job steering macro. Should be either "AFII" or "FullSim"
    if ( stringMeta.empty() ) {
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
  RETURN_CHECK( "JetCalibrator::initialize()", m_jetCleaning->setProperty( "CutLevel", m_jetCalibCutLevel), "");
  RETURN_CHECK( "JetCalibrator::initialize()", m_jetCleaning->initialize(), "JetCleaning Interface succesfully initialized!");

  if( m_saveAllCleanDecisions ){
    //std::string m_decisionNames[] = {"VeryLooseBad", "LooseBad", "MediumBad", "TightBad"};
    m_decisionNames.push_back( "VeryLooseBad" );  m_decisionNames.push_back( "LooseBad" );
    m_decisionNames.push_back( "MediumBad" );     m_decisionNames.push_back( "TightBad" );
    for(unsigned int i=0; i < m_decisionNames.size() ; ++i){
      m_allJetCleaningTools.push_back( new JetCleaningTool((jc_tool_name+"_"+m_decisionNames.at(i)).c_str()) );
      RETURN_CHECK( "JetCalibrator::initialize()", m_allJetCleaningTools.at(i)->setProperty( "CutLevel", m_decisionNames.at(i)), "");
      RETURN_CHECK( "JetCalibrator::initialize()", m_allJetCleaningTools.at(i)->initialize(), ("JetCleaning Interface "+m_decisionNames.at(i)+" succesfully initialized!").c_str());
    }
  }

  // initialize and configure the jet uncertainity tool
  // only initialize if a config file has been given
  //------------------------------------------------
  if ( !m_uncertConfig.empty() && !m_systName.empty() ) {
    m_uncertConfig = gSystem->ExpandPathName( m_uncertConfig.c_str() );
    Info("initialize()","Initialize JES UNCERT with %s", m_uncertConfig.c_str());
    std::string ju_tool_name = std::string("JESProvider_") + m_name;
    m_jetUncert = new JetUncertaintiesTool( ju_tool_name.c_str() );
    RETURN_CHECK("JetCalibrator::initialize()", m_jetUncert->setProperty("JetDefinition",m_jetUncertAlgo), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_jetUncert->setProperty("MCType","MC12"), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_jetUncert->setProperty("ConfigFile", m_uncertConfig), "");
    RETURN_CHECK("JetCalibrator::initialize()", m_jetUncert->initialize(), "");
    m_jetUncert->msg().setLevel( MSG::ERROR ); // VERBOSE, INFO, DEBUG
    const CP::SystematicSet recSysts = m_jetUncert->recommendedSystematics();

    Info("initialize()"," Initializing Jet Systematics :");
    m_systList = HelperFunctions::getListofSystematics( recSysts, m_systName, m_systVal );

    // Setup the tool for the 1st systematic on the list
    // If running all, the tool will be setup for each syst on each event
    if ( !m_systList.empty() ) {
      m_runSysts = true;
      // setup uncertainity tool for systematic evaluation
      if ( m_jetUncert->applySystematicVariation(m_systList.at(0)) != CP::SystematicCode::Ok ) {
        Error("initialize()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
        return EL::StatusCode::FAILURE;
      }
    }
  } // running systematics
  else {
    Info("initialize()", "No uncertainities considered");
    // m_jetUncert not streamed so have to do this
    m_runSysts = false; m_jetUncert = nullptr;
  }

  // if not running systematics, need the nominal
  // if running systematics, and running them all, need the nominal
  // add it to the front!
  if ( m_systList.empty() || (!m_systList.empty() && m_systName == "All") ) {
    m_systList.insert( m_systList.begin(), CP::SystematicSet() );
    const CP::SystematicVariation nullVar = CP::SystematicVariation(""); // blank = nominal
    m_systList.begin()->insert(nullVar);
  }

  for ( const auto& syst_it : m_systList ){
    Info("initialize()"," Running with systematic : %s", (syst_it.name()).c_str());
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
  RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, m_debug) ,"");

  // loop over available systematics - remember syst == "Nominal" --> baseline
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;
  for ( const auto& syst_it : m_systList ) {

    std::string outSCContainerName(m_outSCContainerName);
    std::string outSCAuxContainerName(m_outSCAuxContainerName);
    std::string outContainerName(m_outContainerName);

    // always append the name of the variation, including nominal which is an empty string
    outSCContainerName    += syst_it.name();
    outSCAuxContainerName += syst_it.name();
    outContainerName      += syst_it.name();
    vecOutContainerNames->push_back( syst_it.name() );

    if ( m_runSysts ) {
      if( m_debug ) { std::cout << "Configure for systematic variation : " << syst_it.name() << std::endl; }
      if ( m_jetUncert->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
        Error("execute()", "Cannot configure JetUncertaintiesTool for systematic %s", m_systName.c_str());
        return EL::StatusCode::FAILURE;
      }
    }

    // create shallow copy;
    std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > calibJetsSC = xAOD::shallowCopyContainer( *inJets );
    ConstDataVector<xAOD::JetContainer>* calibJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
    calibJetsCDV->reserve( calibJetsSC.first->size() );

    // calibrate and decorate with cleaning decision
    for ( auto jet_itr : *(calibJetsSC.first) ) {
      m_numObject++;

      if ( m_jetCalibration->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
        Error("execute()", "JetCalibration tool reported a CP::CorrectionCode::Error");
        Error("execute()", "%s", m_name.c_str());
        return StatusCode::FAILURE;
      }

      if ( m_runSysts ) {
        if ( m_jetUncert->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
          Error("execute()", "JetUncertaintiesTool reported a CP::CorrectionCode::Error");
          Error("execute()", "%s", m_name.c_str());
        }
      }

      // decorate with cleaning decision
      static SG::AuxElement::Decorator< char > isCleanDecor( "cleanJet" );
      isCleanDecor( *jet_itr) = jetDecision(jet_itr, m_jetCleaning);

      if( m_saveAllCleanDecisions ){
        for(unsigned int i=0; i < m_allJetCleaningTools.size() ; ++i){
          jet_itr->auxdata< char >(("clean_"+m_decisionNames.at(i)).c_str()) = jetDecision(jet_itr, m_allJetCleaningTools.at(i));
        }
      }

    }

    if ( !xAOD::setOriginalObjectLink(*inJets, *(calibJetsSC.first)) ) {
      Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
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
  if ( m_debug ) { m_store->print(); }

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
  if ( m_jetUncert ) {
    delete m_jetUncert; m_jetUncert = nullptr;
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

  return EL::StatusCode::SUCCESS;
}

bool JetCalibrator :: jetDecision(const xAOD::Jet* jet, JetCleaningTool* j_cleaner){
  bool decision = false;

  try{
    decision = j_cleaner->accept( *jet);
  }catch(...){
    if(jet->isAvailable<ElementLink< xAOD::JetContainer > >("Parent")){
      const xAOD::Jet* parent = dynamic_cast<const xAOD::Jet*>(*(jet->auxdata<ElementLink< xAOD::JetContainer > >("Parent")));
      decision = j_cleaner->accept( *parent);
    }else{
      Error("jetDecision()", "Could not make jet cleaning decision on the jet or it's parent!");
    }
  }

  return decision;
}
