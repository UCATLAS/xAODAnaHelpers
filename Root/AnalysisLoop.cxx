#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

#include <xAODAnaHelpers/AnalysisLoop.h>

// additional includes that do not go in the header
// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
// ROOT include(s):
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(AnalysisLoop)



AnalysisLoop :: AnalysisLoop ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}

EL::StatusCode AnalysisLoop :: configure ()
{
  Info("AnalysisLoop::configure()", "User configuration read from : %s", m_configFile.c_str());

  // read in user configuration from text file
  m_configFile = gSystem->ExpandPathName( m_configFile.c_str() );
  TEnv *env = new TEnv(m_configFile.c_str());
  if( !env ) {
    Error("AnalysisLoop()", "Failed to initialize reading of config file. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // basics
  m_debug         = env->GetValue("Debug"     ,     false       );
  m_maxEvent      = env->GetValue("MaxEvent"  ,     -1          );
  m_isMC          = env->GetValue("IsMC"      ,     true        ); // FIXME - needs to be better than this!!!

  // primary vertex
  m_vertexContainerName = env->GetValue("VertexContainer", "PrimaryVertices");

  // muons
  m_muonContainerName = env->GetValue("MuonContainer", "");
  if( m_muonContainerName.length() > 0 ) {
    Info("AnalysisLoop()", "Using Muon Container     : %s", m_muonContainerName.c_str() );
  }

  // electrons
  m_electronContainerName = env->GetValue("ElectronContainer", "");
  if( m_electronContainerName.length() > 0 ) {
    Info("AnalysisLoop()", "Using Electron Container : %s", m_electronContainerName.c_str() );
  }

  // jets
  m_jetContainerName = env->GetValue("JetContainer", "");
  if( m_jetContainerName.length() > 0 ) {
    Info("AnalysisLoop::configure()", "Using Jet Container      : %s", m_jetContainerName.c_str() );
  }

  m_fatJetContainerName = env->GetValue("FatJetContainer", "");
  if( m_fatJetContainerName.length() > 0 ) {
    Info("AnalysisLoop::configure()", "Using Fat Jet Container  : %s", m_fatJetContainerName.c_str() );
  }

  env->Print();
  Info("AnalysisLoop::configure()", "Complete! \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisLoop :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or maynot be of value to you.
  job.useXAOD();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("AnalysisLoop").ignore(); // call before opening first file

  // custom user job setup
  if( setupJobUser( job ) != EL::StatusCode::SUCCESS ) {
    Error("setupJob()", "User setupJob failed");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisLoop :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  //TODO this starts as true, and must be changed
//!!  if (m_histEventCount) {
//!!    // TODO why is histInitialize() called after fileExecute() ??
//!!    Warning("fileExecute()", "Histograms already initialized! Skipping histInitialize()...");
//!!    return EL::StatusCode::SUCCESS;
//!!  }

  // event counts from meta data
  m_histEventCount = new TH1D("MetaData_EventCount", "MetaData_EventCount", 6, 0.5, 6.5);
  m_histEventCount -> GetXaxis() -> SetBinLabel(1, "nEvents initial");
  m_histEventCount -> GetXaxis() -> SetBinLabel(2, "nEvents selected in");
  m_histEventCount -> GetXaxis() -> SetBinLabel(3, "nEvents selected out");
  m_histEventCount -> GetXaxis() -> SetBinLabel(4, "sumOfWeights initial");
  m_histEventCount -> GetXaxis() -> SetBinLabel(5, "sumOfWeights selected in");
  m_histEventCount -> GetXaxis() -> SetBinLabel(6, "sumOfWeights selected out");
  wk() -> addOutput(m_histEventCount);

  if( histInitializeUser() != EL::StatusCode::SUCCESS ) {
    Error("histInitialize()", "User histogram initialize failed");
    return EL::StatusCode::FAILURE;
  }

  Info("histInitialize()", "Histograms initialized.");
  return EL::StatusCode::SUCCESS;

}



EL::StatusCode AnalysisLoop :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  if (!m_histEventCount) {
    // TODO why is histInitialize() called after fileExecute() ??
    Warning("fileExecute()", "Histograms not initialized! Calling histInitialize()...");
    histInitialize();
  }

  //----------------------------
  // Meta data
  //---------------------------

  // get the MetaData tree once a new file is opened, with
  TTree *MetaData = dynamic_cast<TTree*>(wk()->inputFile()->Get("MetaData"));
  if (!MetaData) {
    // TODO: report Error and return FAILURE?
    Warning("fileExecute()", "MetaData not found!");
    return EL::StatusCode::SUCCESS;
  }
  MetaData->LoadTree(0);

  // extract the information from the EventBookkeepers branch
  TTreeFormula tfNevents("tf", "EventBookkeepers.m_nAcceptedEvents", MetaData);
  TTreeFormula tfSumW("tf", "EventBookkeepers.m_nWeightedAcceptedEvents", MetaData);
  tfNevents.UpdateFormulaLeaves();
  tfSumW.UpdateFormulaLeaves();
  if (tfNevents.GetNdata() < 2 || tfSumW.GetNdata() < 2) {
    // TODO: report Error and return FAILURE?
    Warning("fileExecute()", "Could not read sum of weights from MetaData!");
    return EL::StatusCode::SUCCESS;
  }

  // read meta data
  Info("fileExecute()", "Meta data from this file:");
  Info("fileExecute()", "Initial  events         = %f", tfNevents.EvalInstance(1));
  Info("fileExecute()", "Selected events         = %f", tfNevents.EvalInstance(0));
  Info("fileExecute()", "Initial  sum of weights = %f", tfSumW.EvalInstance(1));
  Info("fileExecute()", "Selected sum of weights = %f", tfSumW.EvalInstance(0));
  m_histEventCount -> Fill(1, tfNevents.EvalInstance(1)); // nEvents initial
  m_histEventCount -> Fill(2, tfNevents.EvalInstance(0)); // nEvents selected in
  m_histEventCount -> Fill(4, tfSumW.EvalInstance(1)); // sumOfWeights initial
  m_histEventCount -> Fill(5, tfSumW.EvalInstance(0)); // sumOfWeights selected in


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisLoop :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisLoop :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  // get TEvent
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  if ( configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  //https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/GoodRunListsForAnalysis
  vecStringGRL.push_back("/afs/cern.ch/user/a/atlasdqm/grlgen/All_Good/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml");
  m_grl->setProperty( "GoodRunsListVec", vecStringGRL);
  m_grl->setProperty("PassThrough", false); // if true (default) will ignore result of GRL and will just pass all events
  if (!m_grl->initialize().isSuccess()) { // check this isSuccess
    Error("initialize()", "Failed to properly initialize the GRL. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  //--------------------------------------------
  //  Get Containers Depending on Analysis Needs
  //--------------------------------------------

  // First open configuration file which holds all the analysis customization
  // get configuration from steerfile

  // as a check, let's see the number of events in our xAOD (long long int)
  Info("initialize()", "Number of events in file   = %lli", m_event->getEntries());
  if(m_maxEvent < 0) m_maxEvent = m_event->getEntries() ;
  Info("initialize()", "Number of events to run on = %i ",m_maxEvent);

  // count number of events
  m_eventCounter   = 0;
  m_numCleanEvents = 0;
  m_numPassEvents  = 0;

  // custom user initalizations
  if( initializeUser() != EL::StatusCode::SUCCESS ) {
    Error("initialize()", "User initialize failed");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode AnalysisLoop :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  // print every 100 events, so we know where we are:
  if ( (m_eventCounter % 100) == 0 ) {
    Info("execute()", "Event number = %i", m_eventCounter);
  }
  if( m_maxEvent < m_eventCounter)
    return EL::StatusCode::SUCCESS;

  //---------------------------------------------------------
  //  Get Containers which everyone needs
  //    xADO::EventInfo       - eventInfo ( const )
  //---------------------------------------------------------

  //----------------------------
  // Event information
  //---------------------------
  const xAOD::EventInfo* eventInfo = 0;
  if ( ! m_event->retrieve(eventInfo, "EventInfo").isSuccess() ) {
    Error("AnalysisLoop::execute()", "Failed to retrieve event info collection. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  if(m_isMC != eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION )) {
    Error("AnalysisLoop::execute()", "EventInfo and user disagree on event type. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  // if data check if event passes GRL and even cleaning
  if( !m_isMC ) { // it's data!

    // GRL
    if(!m_grl->passRunLB(*eventInfo)){
      return EL::StatusCode::SUCCESS; // go to next event
    }

    //------------------------------------------------------------
    // Apply event cleaning to remove events due to
    // problematic regions of the detector, and incomplete events.
    // Apply to data.
    //------------------------------------------------------------
    // reject event if:
    if( (eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) || (eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) || (eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) )  )
    {
      return EL::StatusCode::SUCCESS; // go to the next event
    } // end if event flags check
    m_numCleanEvents++;

  } // end if not MC


  ++m_eventCounter;
  if( passSelection() == EL::StatusCode::SUCCESS ) {
    m_numPassEvents++;
  } else {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisLoop :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  //--------------------------
  //save the event in the xAOD
  //--------------------------
//  if(m_event->fill() < 0){
//    Error("execute()", "Failed writing event to output file!");
//  };

  // GF TOFIX
  //m_histEventCount -> Fill(3); // nEvents selected out
  //m_histEventCount -> Fill(6, m_eventInfoHandler->get_MCEventWeight()); // sumOfWeights selected out

  // custom user execute
  if( postExecuteUser() != EL::StatusCode::SUCCESS ) {
    Error("postExecute()", "User postExecute failed");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisLoop :: finalize ()
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

  // custom user finalize
  if( finalizeUser() != EL::StatusCode::SUCCESS ) {
    Error("finalize()", "User finalize failed");
    return EL::StatusCode::FAILURE;
  }

  Info("finalize()", "Number of events          = %i", m_eventCounter);
  if(!m_isMC) {
    Info("finalize()", "Number of clean events    = %i", m_numCleanEvents);
  }
  Info("finalize()", "Number of selected events = %i", m_numPassEvents);

  if(m_grl)           { delete m_grl;           m_grl             = 0; }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisLoop :: histFinalize ()
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
  return EL::StatusCode::SUCCESS;
}
