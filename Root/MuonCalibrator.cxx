#include <iostream>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODEventInfo/EventInfo.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODAnaHelpers/MuonCalibrator.h"
#include "xAODAnaHelpers/HelperFunctions.h"


#include "MuonMomentumCorrections/MuonCalibrationAndSmearingTool.h"
#include "PATInterfaces/CorrectionCode.h" // to check the return correction code status of tools

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(MuonCalibrator)


MuonCalibrator :: MuonCalibrator () {
}

MuonCalibrator :: MuonCalibrator (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_muonCalibrationAndSmearingTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonCalibrator()", "Calling constructor \n");

}

EL::StatusCode  MuonCalibrator :: configure ()
{
  Info("configure()", "Configuing MuonCalibrator Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  // check if file exists
  /* https://root.cern.ch/root/roottalk/roottalk02/5332.html */
  FileStat_t fStats;
  int fSuccess = gSystem->GetPathInfo(m_configName.c_str(), fStats);
  if(fSuccess != 0){
    Error("configure()", "Could not find the configuration file");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Found configuration file");
  
  TEnv* config = new TEnv(m_configName.c_str());

  // read debug flag from .config file
  m_debug         = config->GetValue("Debug" , false );
  // input container to be read from TEvent or TStore
  m_inContainerName         = config->GetValue("InputContainer",  "");

  m_outContainerName        = config->GetValue("OutputContainer", "");
  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  m_sort                    = config->GetValue("Sort",          false);

  if( m_inContainerName.Length() == 0 ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }
  // add more and add to Muon selector

  config->Print();
  Info("configure()", "MuonCalibrator Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonCalibrator :: setupJob (EL::Job& job)
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
  xAOD::Init( "MuonCalibrator" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing MuonCalibrator Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the muon calibration and smearing tool
  m_muonCalibrationAndSmearingTool = new CP::MuonCalibrationAndSmearingTool( "MuonCorrectionTool" );
  m_muonCalibrationAndSmearingTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE
  if (! m_muonCalibrationAndSmearingTool->initialize().isSuccess() ){
    Error("initialize()", "Failed to properly initialize the MonCalibrationAndSmearingTool Tool. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  Info("initialize()", "MuonCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Muon Calibration and Cleaning... \n");

  m_numEvent++;

  const xAOD::EventInfo* eventInfo = 0;
  if ( ! m_event->retrieve(eventInfo, "EventInfo").isSuccess() ) {
    Error("execute()", "Failed to retrieve event info collection. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  // get the collection from TEvent or TStore
  const xAOD::MuonContainer* inMuons = 0;
  if ( !m_event->retrieve( inMuons , m_inContainerName.Data() ).isSuccess() ){
    if ( !m_store->retrieve( inMuons , m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  if(m_debug){
    for( auto muon: *inMuons ){
      Info("execute()", "  original muon pt = %.2f GeV", (muon->pt() * 1e-3));
    }
  }

  // create shallow copy
  std::pair< xAOD::MuonContainer*, xAOD::ShallowAuxContainer* > calibMuonsSC = xAOD::shallowCopyContainer( *inMuons );
  ConstDataVector<xAOD::MuonContainer>* calibMuonsCDV = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
  calibMuonsCDV->reserve( calibMuonsSC.first->size() );

  // calibrate only MC
  if( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) {
    for( auto muonSC_itr : *(calibMuonsSC.first) ) {
      /* https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/SoftwareTutorialxAODAnalysisInROOT#Muons */
      if( m_muonCalibrationAndSmearingTool->applyCorrection(*muonSC_itr) == CP::CorrectionCode::Error ){ // apply correction and check return code
        // Can have CorrectionCode values of Ok, OutOfValidityRange, or Error. Here only checking for Error.
        // If OutOfValidityRange is returned no modification is made and the original muon values are taken.
        Error("execute()", "MuonCalibrationAndSmearingTool returns Error CorrectionCode");
      }
      if(m_debug) Info("execute()", "  corrected muon pt = %.2f GeV", (muonSC_itr->pt() * 1e-3));
    } 
  } 

  if(m_sort) {
    std::sort( calibMuonsSC.first->begin(), calibMuonsSC.first->end(), HelperFunctions::sort_pt );
  }

  // save pointers in ConstDataVector with same order
  for( auto mu_itr : *(calibMuonsSC.first) ) {
    calibMuonsCDV->push_back( mu_itr );
  }

  // add shallow copy to TStore
  if( !m_store->record( calibMuonsSC.first, m_outSCContainerName.Data() ).isSuccess() ){
    Error("execute()  ", "Failed to store container %s. Exiting.", m_outSCContainerName.Data() );
    return EL::StatusCode::FAILURE;
  }
  if( !m_store->record( calibMuonsSC.second, m_outSCAuxContainerName.Data() ).isSuccess() ){
    Error("execute()  ", "Failed to store aux container %s. Exiting.", m_outSCAuxContainerName.Data() );
    return EL::StatusCode::FAILURE;
  }
  // add ConstDataVector to TStore
  if( !m_store->record( calibMuonsCDV, m_outContainerName.Data() ).isSuccess() ){
    Error("execute()  ", "Failed to store const data container %s. Exiting.", m_outContainerName.Data() );
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: finalize ()
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

  if(m_muonCalibrationAndSmearingTool){
    delete m_muonCalibrationAndSmearingTool;
    m_muonCalibrationAndSmearingTool = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: histFinalize ()
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
