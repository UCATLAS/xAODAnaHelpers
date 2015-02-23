/******************************************
 *
 * Basic event selection. Performs general simple cuts (GRL, Event Cleaning, Min nr. Tracks for PV candidate)
 *
 * G. Facini, M. Milesi (marco.milesi@cern.ch)
 * Jan 28 16:44 AEST 2015
 *
 ******************************************/

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

// additional includes that do not go in the header
// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"

// rootcore includes
#include "GoodRunsLists/GoodRunsListSelectionTool.h"

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/BasicEventSelection.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(BasicEventSelection)


BasicEventSelection :: BasicEventSelection (){
}

BasicEventSelection :: BasicEventSelection (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("BasicEventSelection()", "Calling constructor \n");
  //StatusCode::enableFailure();
  m_cutflowHist  = 0;
  m_cutflowHistW = 0;
}


EL::StatusCode BasicEventSelection :: configure ()
{
  // read in user configuration from text file
  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  TEnv *env = new TEnv(m_configName.c_str());
  if( !env ) {
    Error("BasicEventSelection()", "Failed to initialize reading of config file. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // basics
  m_debug         = env->GetValue("Debug"     ,     0         );
  m_cleanTStore   = env->GetValue("CleanTStore",    true      );

  // GRL
  m_GRLxml        = env->GetValue("GRL"       ,     "/afs/cern.ch/user/a/atlasdqm/grlgen/All_Good/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml"  );    //https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/GoodRunListsForAnalysis

  // primary vertex
  m_vertexContainerName = env->GetValue("VertexContainer", "PrimaryVertices");
  // number of tracks to require to count PVs
  m_PVNTrack            = env->GetValue("NTrackForPrimaryVertex",  2); // harmonized cut

  env->Print();
  Info("configure()", "BasicEventSelection succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode BasicEventSelection :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
  //
  Info("setupJob()", "Calling setupJob \n");

  job.useXAOD();
  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("BasicEventSelection").ignore(); // call before opening first file

  EL::OutputStream outForCFlow("cutflow");
  job.outputAdd ( outForCFlow );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "Calling histInitialize \n");

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

  // write the cutflows to this file so algos downstream can pick up the pointer
  TFile *file = wk()->getOutputFile ("cutflow");
  file->cd();
  // use 1,1,2 so Fill(bin) and GetBinContent(bin) refer to the same bin
  m_cutflowHist  = new TH1D("cutflow", "cutflow", 1, 1, 2);
  m_cutflowHist->SetBit(TH1::kCanRebin);
  // use 1,1,2 so Fill(bin) and GetBinContent(bin) refer to the same bin
  m_cutflowHistW = new TH1D("cutflow_weighted", "cutflow_weighted", 1, 1, 2);
  m_cutflowHistW->SetBit(TH1::kCanRebin);

  // label the bins for the cutflow
  m_cutflow_all  = m_cutflowHist->GetXaxis()->FindBin("all");
  m_cutflow_grl  = m_cutflowHist->GetXaxis()->FindBin("GRL");
  m_cutflow_lar  = m_cutflowHist->GetXaxis()->FindBin("LAr");
  m_cutflow_tile = m_cutflowHist->GetXaxis()->FindBin("tile");
  m_cutflow_core = m_cutflowHist->GetXaxis()->FindBin("core");
  m_cutflow_npv  = m_cutflowHist->GetXaxis()->FindBin("NPV");

  // do it again for the weighted cutflow hist
  m_cutflowHistW->GetXaxis()->FindBin("all");
  m_cutflowHistW->GetXaxis()->FindBin("GRL");
  m_cutflowHistW->GetXaxis()->FindBin("LAr");
  m_cutflowHistW->GetXaxis()->FindBin("tile");
  m_cutflowHistW->GetXaxis()->FindBin("core");
  m_cutflowHistW->GetXaxis()->FindBin("NPV");

  Info("histInitialize()", "Histograms initialized!");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  Info("fileExecute()", "Calling fileExecute \n");

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
//  m_histEventCount -> Fill(1, tfNevents.EvalInstance(1)); // nEvents initial
//  m_histEventCount -> Fill(2, tfNevents.EvalInstance(0)); // nEvents selected in
//  m_histEventCount -> Fill(4, tfSumW.EvalInstance(1)); // sumOfWeights initial
//  m_histEventCount -> Fill(5, tfSumW.EvalInstance(0)); // sumOfWeights selected in


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing BasicEventSelection... \n");

  // get TEvent and TStore
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  m_GRLxml = gSystem->ExpandPathName( m_GRLxml.c_str() );
  vecStringGRL.push_back(m_GRLxml);
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty( "GoodRunsListVec", vecStringGRL), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty("PassThrough", false), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->initialize(), "");

  //--------------------------------------------
  //  Get Containers Depending on Analysis Needs
  //--------------------------------------------

  // First open configuration file which holds all the analysis customization
  // get configuration from steerfile

  // as a check, let's see the number of events in our xAOD (long long int)
  Info("initialize()", "Number of events in file   = %lli", m_event->getEntries());
  // count number of events
  m_eventCounter   = 0;

  Info("initialize()", "BasicEventSelection succesfully initilaized!");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  //----------------------------
  // Event information
  //---------------------------
  const xAOD::EventInfo* eventInfo = 0;
  RETURN_CHECK("BasicEventSelection::execute()", m_event->retrieve(eventInfo, "EventInfo"), "");

  bool isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) ? true : false;

  float mcEvtWeight(1.0), pileupWeight(1.0);
  if( isMC ){
     const std::vector< float > weights = eventInfo->mcEventWeights();
     if( weights.size() > 0 ) mcEvtWeight = weights[0];

     //if( m_doPUreweighting ){ // FIXME
     //  pileupWeight = eventInfo->auxdecor< double >( "PileupWeight" ); // this decoration added previously by PU reweighting tool
     //}
     mcEvtWeight *= pileupWeight;
  }
  // decorate with PU corrected mc event weight
  static SG::AuxElement::Decorator< float > mcEvtWeightDecor("mcEventWeight");
  mcEvtWeightDecor(*eventInfo) = mcEvtWeight;

  // print every 100 events, so we know where we are:
  ++m_eventCounter;
  m_cutflowHist ->Fill( m_cutflow_all, 1 );
  m_cutflowHistW->Fill( m_cutflow_all, mcEvtWeight);
  if ( (m_eventCounter % 1000) == 0 ) {
    Info("execute()", "Event number = %i", m_eventCounter);
  }

  if( m_cleanTStore ) {
    if(m_debug > 0 && m_eventCounter % 50 == 0){
      Info(m_name.c_str(), "Num Events Processed = %i", m_eventCounter);
      Info(m_name.c_str(), "Clearing the store. Content:");
      m_store->print();
      Info(m_name.c_str(), "End Content");
    }
    m_store->clear();
  }


  // if data check if event passes GRL and even cleaning
  if( !isMC ) {

    // GRL
    if(!m_grl->passRunLB(*eventInfo)){
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS; // go to next event
    }
    m_cutflowHist ->Fill( m_cutflow_grl, 1 );
    m_cutflowHistW->Fill( m_cutflow_grl, mcEvtWeight);

    //------------------------------------------------------------
    // Apply event cleaning to remove events due to
    // problematic regions of the detector, and incomplete events.
    // Apply to data.
    //------------------------------------------------------------
    // reject event if:
    if( (eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_lar, 1 );
    m_cutflowHistW->Fill( m_cutflow_lar, mcEvtWeight);

    if( (eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_tile, 1 );
    m_cutflowHistW->Fill( m_cutflow_tile, mcEvtWeight);


    if( (eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_core, 1 );
    m_cutflowHistW->Fill( m_cutflow_core, mcEvtWeight);

  } else { // is MC - fill cutflows just for consistency

    m_cutflowHist ->Fill( m_cutflow_grl, 1 );
    m_cutflowHistW->Fill( m_cutflow_grl, mcEvtWeight);
    m_cutflowHist ->Fill( m_cutflow_lar, 1 );
    m_cutflowHistW->Fill( m_cutflow_lar, mcEvtWeight);
    m_cutflowHist ->Fill( m_cutflow_tile, 1 );
    m_cutflowHistW->Fill( m_cutflow_tile, mcEvtWeight);
    m_cutflowHist ->Fill( m_cutflow_core, 1 );
    m_cutflowHistW->Fill( m_cutflow_core, mcEvtWeight);

  }

  const xAOD::VertexContainer* vertices = 0;
  RETURN_CHECK("BasicEventSelection::execute()", m_event->retrieve( vertices, m_vertexContainerName.Data()), "");

  if( !HelperFunctions::passPrimaryVertexSelection( vertices, m_PVNTrack ) ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }
  m_cutflowHist ->Fill( m_cutflow_npv, 1 );
  m_cutflowHistW->Fill( m_cutflow_npv, mcEvtWeight);


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  // GF TOFIX
  //m_histEventCount -> Fill(3); // nEvents selected out
  //m_histEventCount -> Fill(6, m_eventInfoHandler->get_MCEventWeight()); // sumOfWeights selected out
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: finalize ()
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

  Info("finalize()", "Number of events          = %i", m_eventCounter);

  if(m_grl) { delete m_grl; m_grl = 0; }
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: histFinalize ()
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

//  if(m_useCutFlow) {
//    TFile * file = wk()->getOutputFile (m_cutFlowFileName.Data());
//    std::cout << file->GetName() << std::endl;
//    if(!m_myTree->writeTo( file )) {
//      Error("finalize()", "Failed to write tree to ouput file!");
//      return EL::StatusCode::FAILURE;
//    }
//  }

  return EL::StatusCode::SUCCESS;
}
