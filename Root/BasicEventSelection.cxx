/******************************************
 *
 * Basic event selection. Performs general simple cuts (GRL, Event Cleaning, Min nr. Tracks for PV candidate)
 *
 * G. Facini, M. Milesi (marco.milesi@cern.ch)
 *
 *
 ******************************************/

//#include "PATInterfaces/CorrectionCode.h"
//#include "AsgTools/StatusCode.h"

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

// additional includes that do not go in the header
// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/BasicEventSelection.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "PATInterfaces/CorrectionCode.h"

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
  m_configName(configName),
  m_grl(nullptr),
  m_pileuptool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("BasicEventSelection()", "Calling constructor");

  //CP::CorrectionCode::enableFailure();
  //StatusCode::enableFailure();

  m_cutflowHist  = 0;
  m_cutflowHistW = 0;
  m_histEventCount = 0;
}


EL::StatusCode BasicEventSelection :: configure ()
{

  if ( !m_configName.empty() ) {
    // read in user configuration from text file
    m_configName = gSystem->ExpandPathName( m_configName.c_str() );
    RETURN_CHECK_CONFIG( "BasicEventSelection::configure()", m_configName);

    TEnv *config = new TEnv(m_configName.c_str());
    if ( !config ) {
      Error("BasicEventSelection()", "Failed to initialize reading of config file. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    // basics
    m_debug             = config->GetValue("Debug"     ,     false);
    m_truthLevelOnly    = config->GetValue("TruthLevelOnly", false);

    // GRL
    m_GRLxml            = config->GetValue("GRL","$ROOTCOREBIN/data/xAODAnaHelpers/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml"  );  //https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/GoodRunListsForAnalysis

    // Pileup Reweighting
    m_doPUreweighting   = config->GetValue("DoPileupReweighting", false);

    if ( !m_truthLevelOnly ) {
      // primary vertex
      m_vertexContainerName = config->GetValue("VertexContainer", "PrimaryVertices");
      // number of tracks to require to count PVs
      m_PVNTrack            = config->GetValue("NTrackForPrimaryVertex",  2); // harmonized cut
    }

    // Trigger
    m_triggerSelection           = config->GetValue("Trigger", "");
    if( m_triggerSelection.size() > 0)
      Info("configure()", "Using Trigger %s", m_triggerSelection.c_str() );

    config->Print();

    Info("configure()", "BasicEventSelection succesfully configured! ");

    delete config; config = nullptr;
  }

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
  Info("setupJob()", "Calling setupJob");

  job.useXAOD();
  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("BasicEventSelection").ignore(); // call before opening first file

  EL::OutputStream outForCFlow("cutflow");
  job.outputAdd ( outForCFlow );
  EL::OutputStream outForMetadata("metadata");
  job.outputAdd ( outForMetadata );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  // TODO why is histInitialize() called after fileExecute() ??

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  Info("histInitialize()", "Calling histInitialize");

  // write the metadata hist to this file so algos downstream can pick up the pointer
  TFile *fileMD = wk()->getOutputFile ("metadata");
  fileMD->cd();
  // event counts from meta data
  m_histEventCount = new TH1D("MetaData_EventCount", "MetaData_EventCount", 6, 0.5, 6.5);
  m_histEventCount -> GetXaxis() -> SetBinLabel(1, "nEvents initial");
  m_histEventCount -> GetXaxis() -> SetBinLabel(2, "nEvents selected in");
  m_histEventCount -> GetXaxis() -> SetBinLabel(3, "nEvents selected out");
  m_histEventCount -> GetXaxis() -> SetBinLabel(4, "sumOfWeights initial");
  m_histEventCount -> GetXaxis() -> SetBinLabel(5, "sumOfWeights selected in");
  m_histEventCount -> GetXaxis() -> SetBinLabel(6, "sumOfWeights selected out");

  // write meta data to histogram
  // This is done here b/c histInitialize() is called after fileExecute()...
  // ... why ??
  Info("fileExecute()", "Meta data from this file:");
  Info("fileExecute()", "Initial  events	 = %i", m_MD_initialNevents);
  Info("fileExecute()", "Selected events	 = %i", m_MD_finalNevents);
  Info("fileExecute()", "Initial  sum of weights = %f", m_MD_initialSumW);
  Info("fileExecute()", "Selected sum of weights = %f", m_MD_finalSumW);
  m_histEventCount -> Fill(1, m_MD_initialNevents); // nEvents initial
  m_histEventCount -> Fill(2, m_MD_finalNevents);   // nEvents selected in
  m_histEventCount -> Fill(4, m_MD_initialSumW);    // sumOfWeights initial
  m_histEventCount -> Fill(5, m_MD_finalSumW);      // sumOfWeights selected in


  // write the cutflows to this file so algos downstream can pick up the pointer
  TFile *fileCF = wk()->getOutputFile ("cutflow");
  fileCF->cd();
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
  if( m_triggerSelection.size() > 0)
    m_cutflow_trigger  = m_cutflowHist->GetXaxis()->FindBin("Trigger");

  // do it again for the weighted cutflow hist
  m_cutflowHistW->GetXaxis()->FindBin("all");
  m_cutflowHistW->GetXaxis()->FindBin("GRL");
  m_cutflowHistW->GetXaxis()->FindBin("LAr");
  m_cutflowHistW->GetXaxis()->FindBin("tile");
  m_cutflowHistW->GetXaxis()->FindBin("core");
  m_cutflowHistW->GetXaxis()->FindBin("NPV");
  if( m_triggerSelection.size() > 0)
    m_cutflowHistW->GetXaxis()->FindBin("Trigger");


  Info("histInitialize()", "Histograms initialized!");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  Info("fileExecute()", "Calling fileExecute");

  if (!m_histEventCount) {
    // TODO why is histInitialize() called after fileExecute() ??
    Warning("fileExecute()", "Histograms for event counting not initialized! Calling histInitialize()...");
    histInitialize();
  }

  //----------------------------
  // Meta data
  //---------------------------

  // Marco
  // Metadata on intial N (weighted) events are used to correctly normalise MC if running on a MC DAOD which had some skimming applied at the derivation stage
  // NB: this is a just a hack, and will be replaced by something more official hopefully soon

  // get the MetaData tree once a new file is opened, with
  TTree *MetaData = dynamic_cast<TTree*>(wk()->inputFile()->Get("MetaData"));
  if (!MetaData) {
    Error("fileExecute()", "MetaData not found!");
    return EL::StatusCode::FAILURE;
  }

  TBranch *b = MetaData->GetBranch("EventBookkeepers");
  if(!b){
    Error("fileExecute()", "EventBookkeepers is not a branch of MetaData");
    //return EL::StatusCode::FAILURE;
  }

  // extract the information from the EventBookkeepers branch
  TTreeFormula tfNevents("tfNevents", "EventBookkeepers.m_nAcceptedEvents", MetaData);
  TTreeFormula tfSumW("tfSumW", "EventBookkeepers.m_nWeightedAcceptedEvents", MetaData);

  MetaData->LoadTree(0);

  tfNevents.UpdateFormulaLeaves();
  tfSumW.UpdateFormulaLeaves();

  tfNevents.GetNdata();
  tfSumW.GetNdata();

  if ( !tfNevents.GetNdim() ) {
    Warning("fileExecute()", "Could not read events from MetaData!");
    //return EL::StatusCode::FAILURE;
  }
  if ( !tfSumW.GetNdim() ) {
    Warning("fileExecute()", "Could not read sum of weights from MetaData!");
    //return EL::StatusCode::FAILURE;
  }

  // Marco:
  // NB: xAODs and DxAODs store Nevents initial/final in different entries.
  //    This is going to change soon though ...
  // Update: no one really knows what all the entries truly stand for. Therefore, do not trust this too much at this stage!

  if( HelperFunctions::isFilePrimaryxAOD( wk()->inputFile() ) ){
     // primary xAODs
    Info("fileExecute()", "Processing a primary xAOD file.");
    m_MD_initialNevents = tfNevents.EvalInstance(0);
    m_MD_finalNevents   = tfNevents.EvalInstance(1);
    m_MD_initialSumW = tfSumW.EvalInstance(0);
    m_MD_finalSumW   = tfSumW.EvalInstance(1);
  } else {
     // derived xAODs
    Info("fileExecute()", "Processing a derived xAOD file.");
    m_MD_initialNevents = tfNevents.EvalInstance(3);
    m_MD_finalNevents   = tfNevents.EvalInstance(4);
    m_MD_initialSumW = tfSumW.EvalInstance(3);
    m_MD_finalSumW   = tfSumW.EvalInstance(4);
  }

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

  Info("initialize()", "Initializing BasicEventSelection... ");

  // get TEvent and TStore
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

//  if ( this->configure() == EL::StatusCode::FAILURE ) {
//    Error("initialize()", "Failed to properly configure. Exiting." );
//    return EL::StatusCode::FAILURE;
//  }

  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  m_GRLxml = gSystem->ExpandPathName( m_GRLxml.c_str() );
  vecStringGRL.push_back(m_GRLxml);
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty( "GoodRunsListVec", vecStringGRL), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty("PassThrough", false), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->initialize(), "");

  m_pileuptool = new CP::PileupReweightingTool("Pileup");
  std::vector<std::string> confFiles;
  std::vector<std::string> lcalcFiles;
  //confFiles.push_back("blah"); // pass from config file
  //lcalcFiles.push_back("blah"); // pass from config file
  //RETURN_CHECK("BasicEventSelection::initialize()", m_pileuptool->setProperty("ConfigFiles", confFiles), "");
  //RETURN_CHECK("BasicEventSelection::initialize()", m_pileuptool->setProperty("LumiCalcFiles", lcalcFiles), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_pileuptool->initialize(), "");


  // Trigger //
  m_trigConfTool = new TrigConf::xAODConfigTool( "xAODConfigTool" );
  RETURN_CHECK("BasicEventSelection::initialize()", m_trigConfTool->initialize(), "");
  ToolHandle< TrigConf::ITrigConfigTool > configHandle( m_trigConfTool );

  m_trigDecTool = new Trig::TrigDecisionTool( "TrigDecisionTool" );
  RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "ConfigTool", configHandle ), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "TrigDecisionKey", "xTrigDecision" ), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "OutputLevel", MSG::ERROR), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->initialize(), "");


  //--------------------------------------------
  //  Get Containers Depending on Analysis Needs
  //--------------------------------------------

  // First open configuration file which holds all the analysis customization
  // get configuration from steerfile

  // as a check, let's see the number of events in our xAOD (long long int)
  Info("initialize()", "Number of events in file = %lli", m_event->getEntries());
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
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("BasicEventSelection::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, m_debug) ,"");

  ++m_eventCounter;

  bool isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) ? true : false;
  if(m_debug){
    Info("execute()", "Is MC? %i", static_cast<int>(isMC) );
  }

  float mcEvtWeight(1.0), pileupWeight(1.0);
  if( isMC ){
     const std::vector< float > weights = eventInfo->mcEventWeights(); // The weights of all the MC events used in the simulation
     if( weights.size() > 0 ) mcEvtWeight = weights[0];

     //for ( auto& it : weights ) { Info("execute()", "event weight: %2f.", it ); }

     if( m_doPUreweighting ){
       m_pileuptool->apply(eventInfo);
       static SG::AuxElement::ConstAccessor< double > pileupWeightAcc("PileupWeight");
       pileupWeight = pileupWeightAcc(*eventInfo) ;
     }
     mcEvtWeight *= pileupWeight;
  }
  // decorate with pileup corrected mc event weight
  static SG::AuxElement::Decorator< float > mcEvtWeightDecor("mcEventWeight");
  mcEvtWeightDecor(*eventInfo) = mcEvtWeight;


  // print every 1000 events, so we know where we are:
  m_cutflowHist ->Fill( m_cutflow_all, 1 );
  m_cutflowHistW->Fill( m_cutflow_all, mcEvtWeight);
  if ( (m_eventCounter % 1000) == 0 ) {
    Info("execute()", "Event number = %i", m_eventCounter);
  }

  if( m_debug && (m_eventCounter % 500) == 0 ) {
    Info(m_name.c_str(), "Store Content:");
    m_store->print();
    Info(m_name.c_str(), "End Content");
  }

  // if data check if event passes GRL and even cleaning
  if( !isMC ) {

    // if data event in Egamma stream is also in Muons stream, skip it - TO DO

    // Get the streams that the event was put in
    const std::vector<  xAOD::EventInfo::StreamTag > streams = eventInfo->streamTags();

    if(m_debug){
      for ( auto& it : streams ) {
	const std::string stream_name = it.name();
	Info("execute()", "event has fired stream: %s", stream_name.c_str() );
      }
    }

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

    // if event in Egamma stream was already in Muons stream, skip it



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
  const xAOD::VertexContainer* vertices(nullptr);
  if(!m_truthLevelOnly) {
    RETURN_CHECK("BasicEventSelection::execute()", HelperFunctions::retrieve(vertices, m_vertexContainerName, m_event, m_store, m_debug) ,"");

    if( !HelperFunctions::passPrimaryVertexSelection( vertices, m_PVNTrack ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
  }
  m_cutflowHist ->Fill( m_cutflow_npv, 1 );
  m_cutflowHistW->Fill( m_cutflow_npv, mcEvtWeight);

  // Trigger //
  if (m_triggerSelection.size() > 0){
    auto triggerChainGroup = m_trigDecTool->getChainGroup(m_triggerSelection);
//    std::cout << m_triggerSelection << " " << triggerChainGroup->isPassed() << " " << triggerChainGroup->getPrescale() << std::endl;
    if( !triggerChainGroup->isPassed() ){
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    static SG::AuxElement::Decorator< float > weight_prescale("weight_prescale");
    weight_prescale(*eventInfo) = triggerChainGroup->getPrescale();
    m_cutflowHist ->Fill( m_cutflow_trigger, 1 );
    m_cutflowHistW->Fill( m_cutflow_trigger, mcEvtWeight);
  }


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

  Info("finalize()", "Number of processed events      = %i", m_eventCounter);

  if(m_grl) delete m_grl;
  if(m_pileuptool) delete m_pileuptool;

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
//    TFile * file = wk()->getOutputFile (m_cutFlowFileName);
//    if(!m_myTree->writeTo( file )) {
//      Error("finalize()", "Failed to write tree to ouput file!");
//      return EL::StatusCode::FAILURE;
//    }
//  }

  return EL::StatusCode::SUCCESS;
}
