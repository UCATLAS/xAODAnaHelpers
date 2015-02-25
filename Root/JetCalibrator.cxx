/******************************************
 *
 * Interface to Jet calibration tool(s).
 *
 * G.Facini (gabriel.facini@cern.ch), M. Milesi (marco.milesi@cern.ch)
 * Jan 28 15:54 AEST 2015
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
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):
#include "JetCalibTools/JetCalibrationTool.h"
#include "JetSelectorTools/JetCleaningTool.h"
#include "JetUncertainties/JetUncertaintiesTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"


// this is needed to distribute the algorithm to the workers
ClassImp(JetCalibrator)


JetCalibrator :: JetCalibrator () {
}

JetCalibrator :: JetCalibrator (std::string name, std::string configName, 
    std::string systName, float systVal ) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_jetCalibration(0),
  m_jetCleaning(0),
  m_jetUncert(0),
  m_systName(systName),
  m_systVal(systVal),
  m_runSysts(false)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("JetCalibrator()", "Calling constructor \n");

}

EL::StatusCode  JetCalibrator :: configure ()
{
  Info("configure()", "Configuing JetCalibrator Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG("configure()", m_configName);

  TEnv* config = new TEnv(m_configName.c_str());

  // read debug flag from .config file
  m_debug                   = config->GetValue("Debug" , false );
  // input container to be read from TEvent or TStore
  m_inContainerName         = config->GetValue("InputContainer",  "");

  // CONFIG parameters for JetCalibrationTool
  m_jetAlgo                 = config->GetValue("JetAlgorithm",    "");

  // when running data "_Insitu" is appended to this string
  m_calibSequence           = config->GetValue("CalibSequence",           "EtaJES");
  m_calibConfigData	        = config->GetValue("configNameData",          "JES_Full2012dataset_Preliminary_MC14.config");
  m_calibConfigFullSim      = config->GetValue("configNameFullSim",       "JES_Full2012dataset_May2014.config");
  m_calibConfigAFII         = config->GetValue("configNameAFII",          "JES_Full2012dataset_AFII_January2014.config");

  // CONFIG parameters for JetCleaningTool
  m_jetCalibCutLevel        = config->GetValue("JetCalibCutLevel", "MediumBad");

  // CONFIG parameters for JetUncertaintiesTool
  m_uncertConfig            = config->GetValue("JetUncertConfig", "");
  // calibrator uses TopoEM or TopoLC while the uncertainity tool uses EMTopo and LCTopo
  // calibrator should switch at some point
  // "fix" the name here so the user never knows the difference 
  m_jetUncertAlgo = m_jetAlgo;
  m_jetUncertAlgo = HelperFunctions::replaceString(m_jetUncertAlgo, std::string("TopoEM"), std::string("EMTopo"));
  m_jetUncertAlgo = HelperFunctions::replaceString(m_jetUncertAlgo, std::string("TopoLC"), std::string("LCTopo"));
  
  // shallow copies are made with this output container name
  m_outContainerName        = config->GetValue("OutputContainer", "");
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!
  m_sort                    = config->GetValue("Sort",          false);

  if( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }
  // add more and add to jet selector

  config->Print();
  Info("configure()", "JetCalibrator Interface succesfully configured! \n");

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

  Info("setupJob()", "Calling setupJob \n");

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

  Info("initialize()", "Initializing JetCalibrator Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  const xAOD::EventInfo* eventInfo = HelperFunctions::getContainer<xAOD::EventInfo>("EventInfo", m_event, m_store);
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) ? true : false;

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  if( !m_isMC ) m_calibSequence += "_Insitu";

  if( m_isMC ){
    m_isFullSim = true;
//    // Check simulation flavour for calibration config
//    const std::string stringMeta = wk()->metaData()->getString("SimulationFlavour"); // NB: needs to be defined as sample metadata in job steering macro. Should be either "AFII" or "FullSim"
//    if (stringMeta.empty()){
//      Warning("initialize()", "Could not access simulation flavour from EL::Worker. Treating MC as FullSim by default!" );
//      m_isFullSim = true;
//    } else {
//      m_isFullSim = (stringMeta == "AFII") ? false : true;
//    }
    m_calibConfig = ( m_isFullSim ) ? m_calibConfigFullSim : m_calibConfigAFII;
  } else {
    m_calibConfig = m_calibConfigData;
  }

  if(m_debug){
  std::cout << " Parameters to JetCalibrationTool() : "  << "\n"
        << "\t m_inContainerName : "	    << m_inContainerName        <<  " of type " <<  typeid(m_inContainerName).name() << "\n"
        << "\t m_outContainerName: "	    << m_outContainerName       <<  " of type " <<  typeid(m_outContainerName).name() << "\n"
        << "\t m_outSCContainerName: "	    << m_outSCContainerName     <<  " of type " <<  typeid(m_outSCContainerName).name() << "\n"
        << "\t m_outSCAuxContainerName: "   << m_outSCAuxContainerName  <<  " of type " <<  typeid(m_outSCAuxContainerName).name() << "\n"
        << "\t m_debug: "		            << m_debug 		            <<  " of type " <<  typeid(m_debug).name() <<  "\n"
        << "\t m_isMC: "		            << m_isMC  		            <<  " of type " <<  typeid(m_isMC).name() << "\n"
        << "\t m_jetAlgo  : "		        << m_jetAlgo	            <<  " of type " <<  typeid(m_jetAlgo).name() <<  "\n"
        << "\t m_calibConfig	      : "   << m_calibConfig	        <<  " of type " <<  typeid(m_calibConfig).name() << "\n"
        << "\t m_calibSequence        : "   << m_calibSequence          <<  " of type " <<  typeid(m_calibSequence).name() << "\n"
        << "\t m_jetCalibCutLevel     : "   << m_jetCalibCutLevel	    <<  " of type " <<  typeid(m_jetCalibCutLevel).name() << "\n"
        << "\t m_jetUncertAlgo  : "		        << m_jetUncertAlgo	            <<  " of type " <<  typeid(m_jetUncertAlgo).name() <<  "\n"
        << "\t m_uncertConfig	      : "   << m_uncertConfig	        <<  " of type " <<  typeid(m_uncertConfig).name() << "\n"
  	<< std::endl;
  }

  // initialize jet calibration tool
  m_jetCalibration = new JetCalibrationTool("JetCorrectionTool",
      m_jetAlgo,
      m_calibConfig,
      m_calibSequence,
      !m_isMC);
  m_jetCalibration->msg().setLevel( MSG::ERROR); // VERBOSE, INFO, DEBUG
  RETURN_CHECK( "initialize()", m_jetCalibration->initializeTool("JetCorrectionTool"), "");

  // initialize and configure the jet cleaning tool
  //------------------------------------------------
  m_jetCleaning = new JetCleaningTool("JetCleaning");
  RETURN_CHECK( "initialize()", m_jetCleaning->setProperty( "CutLevel", m_jetCalibCutLevel), "");
  RETURN_CHECK( "initialize()", m_jetCleaning->initialize(), "");

  Info("initialize()", "JetCalibrator Interface succesfully initialized!" );

  // initialize and configure the jet uncertainity tool
  // only initialize if a config file has been given
  //------------------------------------------------
  if ( !m_uncertConfig.empty() && ! m_systName.empty()) {
    m_jetUncert = new JetUncertaintiesTool("JESProvider");
    RETURN_CHECK("initialize()", m_jetUncert->setProperty("JetDefinition",m_jetUncertAlgo), "");
    RETURN_CHECK("initialize()", m_jetUncert->setProperty("MCType","MC12"), "");
    RETURN_CHECK("initialize()", m_jetUncert->setProperty("ConfigFile", m_uncertConfig), "");
    RETURN_CHECK("initialize()", m_jetUncert->initialize(), "");
    CP::SystematicSet recSysts = m_jetUncert->recommendedSystematics();
    Info("initialize()"," The following systematics are recommended:");
    for( auto syst : recSysts ) {
      Info("initialize()","  %s", (syst.basename()).c_str());
      if( m_systName == syst.basename() ) {
        Info("initialize()","Found match! Applying systematic %s", syst.basename().c_str());
        // continuous systematics - can choose at what sigma to evaluate
        if (syst == CP::SystematicVariation (syst.basename(), CP::SystematicVariation::CONTINUOUS)) {
          m_sysList.push_back(CP::SystematicSet());
          if ( m_systVal == 0 ) { 
            Info("initialize()","Setting continuous systematic to 0 is nominal! Please check!");
            return EL::StatusCode::FAILURE;
          }
          m_sysList.back().insert(CP::SystematicVariation (syst.basename(), m_systVal));
        } 
        // not a continuous system
        else {
          m_sysList.push_back(CP::SystematicSet());
          m_sysList.back().insert(syst);
        }
      } // found match!
    } // loop over recommended systematics
    if( !m_sysList.empty() ) { m_runSysts = true; }
  } // running systematics

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Jet Calibration and Cleaning... \n");

  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::JetContainer* inJets = HelperFunctions::getContainer<xAOD::JetContainer>(m_inContainerName, m_event, m_store);

  // create shallow copytrue;
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > calibJetsSC = xAOD::shallowCopyContainer( *inJets );
  ConstDataVector<xAOD::JetContainer>* calibJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  calibJetsCDV->reserve( calibJetsSC.first->size() );

  // calibrate and decorate with cleaning decision
  for( auto jet_itr : *(calibJetsSC.first) ) {
    m_numObject++;

    if ( m_jetCalibration->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
      Error("execute()", "JetCalibration tool reported a CP::CorrectionCode::Error");
      Error("execute()", "%s", m_name.c_str());
      return StatusCode::FAILURE;
    }

    // decorate with cleaning decision
    bool cleanJet = m_jetCleaning->accept( *jet_itr );
    jet_itr->auxdata< char >( "cleanJet" ) = cleanJet;

  }

  if(m_sort) {
    std::sort( calibJetsSC.first->begin(), calibJetsSC.first->end(), HelperFunctions::sort_pt );
  }

  // save pointers in ConstDataVector with same order
  for( auto jet_itr : *(calibJetsSC.first) ) {
    calibJetsCDV->push_back( jet_itr );
  }

  // add shallow copy to TStore
  RETURN_CHECK( "execute()", m_store->record( calibJetsSC.first, m_outSCContainerName), "Failed to record shallow copy container.");
  RETURN_CHECK( "execute()", m_store->record( calibJetsSC.second, m_outSCAuxContainerName), "Failed to record shallow copy aux container.");

  // add ConstDataVector to TStore
  RETURN_CHECK( "execute()", m_store->record( calibJetsCDV, m_outContainerName), "Failed to record const data container.");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

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

  Info("finalize()", "Deleting tool instances... \n");

  if(m_jetCalibration){
    delete m_jetCalibration;
    m_jetCalibration = 0;
  }
  if(m_jetCleaning){
    delete m_jetCleaning;
    m_jetCleaning= 0;
  }
  if( m_jetUncert ) {
    delete m_jetUncert;
    m_jetUncert = 0;
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

  Info("histFinalize()", "Calling histFinalize \n");

  return EL::StatusCode::SUCCESS;
}
