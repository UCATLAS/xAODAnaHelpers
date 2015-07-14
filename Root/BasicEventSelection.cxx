/********************************************************
 *
 * Basic event selection. Performs general simple cuts
 * (GRL, Event Cleaning, Min nr. Tracks for PV candidate)
 *
 * G. Facini, M. Milesi (marco.milesi@cern.ch)
 *
 *
 *******************************************************/

//#include "PATInterfaces/CorrectionCode.h"
//#include "AsgTools/StatusCode.h"

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODCutFlow/CutBookkeeper.h"
#include "xAODCutFlow/CutBookkeeperContainer.h"

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/BasicEventSelection.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "PATInterfaces/CorrectionCode.h"

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(BasicEventSelection)

BasicEventSelection :: BasicEventSelection () :
  m_grl(nullptr),
  m_pileuptool(nullptr),
  m_PU_default_channel(0),
  m_trigConfTool(nullptr),
  m_trigDecTool(nullptr),
  m_histEventCount(nullptr),
  m_cutflowHist(nullptr),
  m_cutflowHistW(nullptr),
  m_el_cutflowHist_1(nullptr),
  m_el_cutflowHist_2(nullptr),
  m_mu_cutflowHist_1(nullptr),
  m_mu_cutflowHist_2(nullptr),
  m_jet_cutflowHist_1(nullptr)  
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("BasicEventSelection()", "Calling constructor");

  // basics
  m_debug = false;
  m_truthLevelOnly = false;

  // derivation name
  m_derivationName = "";

  // Metadata
  m_useMetaData = true;

  // GRL
  m_applyGRLCut = true;
  m_GRLxml = "$ROOTCOREBIN/data/xAODAnaHelpers/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml";  //https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/GoodRunListsForAnalysis
  m_GRLExcludeList = "";

  // Clean Powheg huge weight
  m_cleanPowheg = false;

  // Pileup Reweighting
  m_doPUreweighting   = false;
  m_lumiCalcFileNames = "";
  m_PRWFileNames      = "";

  // Primary Vertex
  m_vertexContainerName = "PrimaryVertices";
  m_applyPrimaryVertexCut = true;
  // number of tracks to require to count PVs
  m_PVNTrack = 2; // harmonized cut

  // Event Cleaning
  m_applyEventCleaningCut = true;

  // Trigger
  m_triggerSelection = "";
  m_applyTriggerCut = true ;
  m_storeTrigDecisions = false;
  m_storePassAny = false;
  m_storePassL1 = false;
  m_storePassHLT = false;
  m_storeTrigKeys = false;

  //CP::CorrectionCode::enableFailure();
  //StatusCode::enableFailure();
}


EL::StatusCode BasicEventSelection :: configure ()
{

  if ( !getConfig().empty() ) {

    // read in user configuration from text file
    TEnv *config = new TEnv(getConfig(true).c_str());
    if ( !config ) {
      Error("BasicEventSelection()", "Failed to initialize reading of config file. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    // basics
    m_debug             = config->GetValue("Debug"     ,     m_debug);
    m_truthLevelOnly    = config->GetValue("TruthLevelOnly", m_truthLevelOnly);

    // derivation name
    m_derivationName    = config->GetValue("DerivationName", m_derivationName.c_str() );
    // temp flag for derivations with broken meta data
    m_useMetaData       = config->GetValue("UseMetaData", m_useMetaData);

    // GRL
    m_applyGRLCut       = config->GetValue("ApplyGRL",        m_applyGRLCut);
    m_applyGRLCut       = config->GetValue("ApplyGRLCut",        m_applyGRLCut);
    m_GRLxml            = config->GetValue("GRL", m_GRLxml.c_str());
    m_GRLExcludeList    = config->GetValue("GRLExclude", m_GRLExcludeList.c_str());

    // Pileup Reweighting
    m_doPUreweighting    = config->GetValue("DoPileupReweighting", m_doPUreweighting);
    m_lumiCalcFileNames  = config->GetValue("LumiCalcFiles",       m_lumiCalcFileNames.c_str());
    m_PRWFileNames       = config->GetValue("PRWFiles",            m_PRWFileNames.c_str());
    m_PU_default_channel = config->GetValue("PUDefaultChannel",    m_PU_default_channel);

    // Event Cleaning
    m_applyEventCleaningCut      = config->GetValue("ApplyEventCleaningCut",    m_applyEventCleaningCut);

    // Primary Vertex
    m_vertexContainerName        = config->GetValue("VertexContainer",       m_vertexContainerName.c_str());
    m_applyPrimaryVertexCut      = config->GetValue("ApplyPrimaryVertexCut", m_applyPrimaryVertexCut);
    // number of tracks to require to count PVs
    m_PVNTrack                   = config->GetValue("NTrackForPrimaryVertex",  m_PVNTrack);

    // Trigger
    m_triggerSelection           = config->GetValue("Trigger",            m_triggerSelection.c_str());
    m_applyTriggerCut            = config->GetValue("ApplyTriggerCut",    m_applyTriggerCut);
    m_storeTrigDecisions         = config->GetValue("StoreTrigDecision",  m_storeTrigDecisions);
    m_storePassAny               = config->GetValue("StorePassAny",       m_storePassAny);
    m_storePassL1                = config->GetValue("StorePassL1",        m_storePassL1);
    m_storePassHLT               = config->GetValue("StorePassHLT",       m_storePassHLT);
    m_storeTrigKeys              = config->GetValue("StoreTrigKeys",      m_storeTrigKeys);

    // if truth level make sure parameters are set properly
    if( m_truthLevelOnly ) {
      Info("configure()", "Truth only! Turn off trigger stuff");
      m_triggerSelection = "";
      m_applyTriggerCut = m_storeTrigDecisions = m_storePassAny = m_storePassL1 = m_storePassHLT = m_storeTrigKeys = false;
      Info("configure()", "Truth only! Turn off GRL");
      m_applyGRLCut = false;
      Info("configure()", "Truth only! Turn off Pile-up Reweight");
      m_doPUreweighting = false;
    }

    if( !m_triggerSelection.empty() )
      Info("configure()", "Using Trigger %s", m_triggerSelection.c_str() );
    if( !m_applyTriggerCut )
      Info("configure()", "WILL NOT CUT ON TRIGGER AS YOU REQUESTED!");

    if( m_doPUreweighting ){
      if( m_lumiCalcFileNames.size() == 0){
        Error("BasicEventSelection()", "Pileup Reweighting is requested but no LumiCalc file is specified. Exiting" );
        return EL::StatusCode::FAILURE;
      }
      if( m_PRWFileNames.size() == 0){
        Error("BasicEventSelection()", "Pileup Reweighting is requested but no PRW file is specified. Exiting" );
        return EL::StatusCode::FAILURE;
      }
    }

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

  Info("histInitialize()", "Calling histInitialize");

  // Make sure configuration variables have been configured
  if ( !getConfig().empty() && ( this->configure() == EL::StatusCode::FAILURE ) ) {
    Error("histInitialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // write the metadata hist to this file so algos downstream can pick up the pointer
  TFile *fileMD = wk()->getOutputFile ("metadata");
  fileMD->cd();

  // event counts from meta data
  if ( !m_histEventCount ) {
    m_histEventCount = new TH1D("MetaData_EventCount", "MetaData_EventCount", 6, 0.5, 6.5);
    m_histEventCount -> GetXaxis() -> SetBinLabel(1, "nEvents initial");
    m_histEventCount -> GetXaxis() -> SetBinLabel(2, "nEvents selected");
    m_histEventCount -> GetXaxis() -> SetBinLabel(3, "sumOfWeights initial");
    m_histEventCount -> GetXaxis() -> SetBinLabel(4, "sumOfWeights selected");
    m_histEventCount -> GetXaxis() -> SetBinLabel(5, "sumOfWeightsSquared initial");
    m_histEventCount -> GetXaxis() -> SetBinLabel(6, "sumOfWeightsSquared selected");
  }

  Info("histInitialize()", "Histograms initialized!");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  Info("fileExecute()", "Calling fileExecute");

  // get TEvent and TStore - must be done here b/c we need to retrieve CutBookkeepers container from TEvent!
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //---------------------------
  // Meta data - CutBookkepers
  //---------------------------
  //
  // Metadata for intial N (weighted) events are used to correctly normalise MC
  // if running on a mc DAOD which had some skimming applied at the derivation stage

  // get the MetaData tree once a new file is opened, with
  TTree *MetaData = dynamic_cast<TTree*>(wk()->inputFile()->Get("MetaData"));
  if ( !MetaData ) {
    Error("fileExecute()", "MetaData not found! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  MetaData->LoadTree(0);

  //check if file is from a DxAOD
  bool m_isDerivation = !MetaData->GetBranch("StreamAOD");

  if ( m_isDerivation && m_useMetaData ) {

    // check for corruption
    const xAOD::CutBookkeeperContainer* incompleteCBC(nullptr);
    if(!m_event->retrieveMetaInput(incompleteCBC, "IncompleteCutBookkeepers").isSuccess()){
      Error("initializeEvent()","Failed to retrieve IncompleteCutBookkeepers from MetaData! Exiting.");
      return EL::StatusCode::FAILURE;
    }

    // Now, let's find the actual information
    const xAOD::CutBookkeeperContainer* completeCBC(nullptr);
    if ( !m_event->retrieveMetaInput(completeCBC, "CutBookkeepers").isSuccess() ){
      Error("fileExecute()","Failed to retrieve CutBookkeepers from MetaData! Exiting.");
      return EL::StatusCode::FAILURE;
    }

    // Find the smallest cycle number, the original first processing step/cycle
    int minCycle(10000);
    for ( auto cbk : *completeCBC ) {
      if ( !( cbk->name().empty() )  && ( minCycle > cbk->cycle() ) ){ minCycle = cbk->cycle(); }
    }

    // Now, let's actually find the right one that contains all the needed info...
    const xAOD::CutBookkeeper* allEventsCBK(nullptr);
    const xAOD::CutBookkeeper* DxAODEventsCBK(nullptr);
    std::string derivationName = m_derivationName + "Kernel";

    if ( m_debug ) { Info("fileExecute()","Looking at DAOD made by Derivation Algorithm: %s", derivationName.c_str()); }

    for ( auto cbk :  *completeCBC ) {
      if ( minCycle == cbk->cycle() && cbk->name() == "AllExecutedEvents" ){
	allEventsCBK = cbk;
      }
      if ( cbk->name() == derivationName){
	DxAODEventsCBK = cbk;
      }
    }

    m_MD_initialNevents     = allEventsCBK->nAcceptedEvents();
    m_MD_initialSumW        = allEventsCBK->sumOfEventWeights();
    m_MD_initialSumWSquared = allEventsCBK->sumOfEventWeightsSquared();

    m_MD_finalNevents       = DxAODEventsCBK->nAcceptedEvents();
    m_MD_finalSumW          = DxAODEventsCBK->sumOfEventWeights();
    m_MD_finalSumWSquared   = DxAODEventsCBK->sumOfEventWeightsSquared();

  }

  //
  // Write meta data to histogram
  //
  Info("histInitialize()", "Meta data from this file:");
  Info("histInitialize()", "Initial  events	 = %u",            static_cast<unsigned int>(m_MD_initialNevents) );
  Info("histInitialize()", "Selected events	 = %u",            static_cast<unsigned int>(m_MD_finalNevents) );
  Info("histInitialize()", "Initial  sum of weights = %f",         m_MD_initialSumW);
  Info("histInitialize()", "Selected sum of weights = %f",         m_MD_finalSumW);
  Info("histInitialize()", "Initial  sum of weights squared = %f", m_MD_initialSumWSquared);
  Info("histInitialize()", "Selected sum of weights squared = %f", m_MD_finalSumWSquared);

  m_histEventCount -> Fill(1, m_MD_initialNevents);
  m_histEventCount -> Fill(2, m_MD_finalNevents);
  m_histEventCount -> Fill(3, m_MD_initialSumW);
  m_histEventCount -> Fill(4, m_MD_finalSumW);
  m_histEventCount -> Fill(5, m_MD_initialSumWSquared);
  m_histEventCount -> Fill(6, m_MD_finalSumWSquared);

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

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("BasicEventSelection::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  m_isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );
  if ( m_debug ) { Info("initialize()", "Is MC? %i", static_cast<int>(m_isMC) ); }


  //Protection in case GRL does not apply to this run
  if(m_applyGRLCut){
    std::string runNumString = std::to_string(eventInfo->runNumber());
    if (m_GRLExcludeList.find( runNumString ) != std::string::npos){
      Info("initialize()", "RunNumber is in GRLExclusion list, setting applyGRL to false");
      m_applyGRLCut = false;
    }
  }

  m_cleanPowheg = false;
  if( eventInfo->runNumber() == 426005 ) { // Powheg+Pythis J5
    m_cleanPowheg = true;
    Info("initialize()", "This is J5 Powheg - cleaning that nasty huge weight event");
  }


  Info("initialize()", "Setting up histograms");

  // write the cutflows to this file so algos downstream can pick up the pointer
  TFile *fileCF = wk()->getOutputFile ("cutflow");
  fileCF->cd();

  // initialise event cutflow, which will be picked ALSO by the algos downstream where an event selection is applied (or at least can be applied)
  //
  // use 1,1,2 so Fill(bin) and GetBinContent(bin) refer to the same bin
  m_cutflowHist  = new TH1D("cutflow", "cutflow", 1, 1, 2);
  m_cutflowHist->SetBit(TH1::kCanRebin);
  // use 1,1,2 so Fill(bin) and GetBinContent(bin) refer to the same bin
  m_cutflowHistW = new TH1D("cutflow_weighted", "cutflow_weighted", 1, 1, 2);
  m_cutflowHistW->SetBit(TH1::kCanRebin);

  // initialise object cutflows, which will be picked by the object selector algos downstream and filled.
  //
  m_el_cutflowHist_1  = new TH1D("cutflow_electrons_1", "cutflow_electrons_1", 1, 1, 2);
  m_el_cutflowHist_1->SetBit(TH1::kCanRebin);
  m_el_cutflowHist_2  = new TH1D("cutflow_electrons_2", "cutflow_electrons_2", 1, 1, 2);
  m_el_cutflowHist_2->SetBit(TH1::kCanRebin);
  m_mu_cutflowHist_1  = new TH1D("cutflow_muons_1", "cutflow_muons_1", 1, 1, 2);
  m_mu_cutflowHist_1->SetBit(TH1::kCanRebin);
  m_mu_cutflowHist_2  = new TH1D("cutflow_muons_2", "cutflow_muons_2", 1, 1, 2);
  m_mu_cutflowHist_2->SetBit(TH1::kCanRebin); 
  m_jet_cutflowHist_1  = new TH1D("cutflow_jets_1", "cutflow_jets_1", 1, 1, 2);
  m_jet_cutflowHist_1->SetBit(TH1::kCanRebin);   
  
  // start labelling the bins for the event cutflow
  //
  m_cutflow_all  = m_cutflowHist->GetXaxis()->FindBin("all");
  m_cutflowHistW->GetXaxis()->FindBin("all");


  if ( !m_isMC ) {
    if ( m_applyGRLCut ) {
      m_cutflow_grl  = m_cutflowHist->GetXaxis()->FindBin("GRL");
      m_cutflowHistW->GetXaxis()->FindBin("GRL");
    }
    m_cutflow_lar  = m_cutflowHist->GetXaxis()->FindBin("LAr");
    m_cutflowHistW->GetXaxis()->FindBin("LAr");
    m_cutflow_tile = m_cutflowHist->GetXaxis()->FindBin("tile");
    m_cutflowHistW->GetXaxis()->FindBin("tile");
    m_cutflow_core = m_cutflowHist->GetXaxis()->FindBin("core");
    m_cutflowHistW->GetXaxis()->FindBin("core");
  }
  m_cutflow_npv  = m_cutflowHist->GetXaxis()->FindBin("NPV");
  m_cutflowHistW->GetXaxis()->FindBin("NPV");
  if ( !m_triggerSelection.empty() > 0 && m_applyTriggerCut ) {
    m_cutflow_trigger  = m_cutflowHist->GetXaxis()->FindBin("Trigger");
    m_cutflowHistW->GetXaxis()->FindBin("Trigger");
  }

  Info("initialize()", "Setting Up Tools");

  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  m_GRLxml = gSystem->ExpandPathName( m_GRLxml.c_str() );
  vecStringGRL.push_back(m_GRLxml);
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty( "GoodRunsListVec", vecStringGRL), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty("PassThrough", false), "");
  RETURN_CHECK("BasicEventSelection::initialize()", m_grl->initialize(), "");

  // Pileup RW Tool //
  if ( m_doPUreweighting ) {
    m_pileuptool = new CP::PileupReweightingTool("Pileup");

    std::vector<std::string> PRWFiles;
    std::vector<std::string> lumiCalcFiles;

    std::string tmp_lumiCalcFileNames = m_lumiCalcFileNames;
    std::string tmp_PRWFileNames = m_PRWFileNames;

    // Parse all comma seperated files
    while( tmp_PRWFileNames.size() > 0){
      int pos = tmp_PRWFileNames.find_first_of(',');
      if( pos == std::string::npos){
        pos = tmp_PRWFileNames.size();
        PRWFiles.push_back(tmp_PRWFileNames.substr(0, pos));
        tmp_PRWFileNames.erase(0, pos);
      }else{
        PRWFiles.push_back(tmp_PRWFileNames.substr(0, pos));
        tmp_PRWFileNames.erase(0, pos+1);
      }
    }
    while( tmp_lumiCalcFileNames.size() > 0){
      int pos = tmp_lumiCalcFileNames.find_first_of(',');
      if( pos == std::string::npos){
        pos = tmp_lumiCalcFileNames.size();
        lumiCalcFiles.push_back(tmp_lumiCalcFileNames.substr(0, pos));
        tmp_lumiCalcFileNames.erase(0, pos);
      }else{
        lumiCalcFiles.push_back(tmp_lumiCalcFileNames.substr(0, pos));
        tmp_lumiCalcFileNames.erase(0, pos+1);
      }
    }

    std::cout << "PileupReweighting Tool is adding Pileup files:" << std::endl;
    for( unsigned int i=0; i < PRWFiles.size(); ++i){
      std::cout << "    " << PRWFiles.at(i) << std::endl;
    }
    std::cout << "PileupReweighting Tool is adding Lumi Calc files:" << std::endl;
    for( unsigned int i=0; i < lumiCalcFiles.size(); ++i){
      std::cout << "    " << lumiCalcFiles.at(i) << std::endl;
    }

    RETURN_CHECK("BasicEventSelection::initialize()", m_pileuptool->setProperty("ConfigFiles", PRWFiles), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileuptool->setProperty("LumiCalcFiles", lumiCalcFiles), "");
    if(m_PU_default_channel)
      RETURN_CHECK("BasicEventSelection::initialize()", m_pileuptool->setProperty("DefaultChannel", m_PU_default_channel), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileuptool->initialize(), "");
  }


  // Trigger //
  if( !m_triggerSelection.empty() || m_applyTriggerCut || m_storeTrigDecisions || m_storePassAny || m_storePassL1 || m_storePassHLT || m_storeTrigKeys ) {
    m_trigConfTool = new TrigConf::xAODConfigTool( "xAODConfigTool" );
    RETURN_CHECK("BasicEventSelection::initialize()", m_trigConfTool->initialize(), "");
    ToolHandle< TrigConf::ITrigConfigTool > configHandle( m_trigConfTool );

    m_trigDecTool = new Trig::TrigDecisionTool( "TrigDecisionTool" );
    RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "ConfigTool", configHandle ), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "TrigDecisionKey", "xTrigDecision" ), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "OutputLevel", MSG::ERROR), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->initialize(), "Failed to initialise TrigDecisionTool!");
  }


  // as a check, let's see the number of events in our file (long long int)
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

  if( m_debug ) { Info("execute()", "Basic Event Selection"); }

  //----------------------------
  // Event information
  //---------------------------
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("BasicEventSelection::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  ++m_eventCounter;

  //--------------------------
  //Print trigger's used for first event only
  if ( m_eventCounter == 1 && !m_triggerSelection.empty() ) {
    Info("execute()", "*** Triggers used (in OR) are:\n");
    auto printingTriggerChainGroup = m_trigDecTool->getChainGroup(m_triggerSelection);
    std::vector<std::string> triggersUsed = printingTriggerChainGroup->getListOfTriggers();
    for ( unsigned int iTrigger = 0; iTrigger < triggersUsed.size(); ++iTrigger ) {
      printf("    %s\n", triggersUsed.at(iTrigger).c_str());
    }
    printf("\n");
  }


  float mcEvtWeight(1.0), pileupWeight(1.0);
  if ( m_isMC ) {
    const std::vector< float > weights = eventInfo->mcEventWeights(); // The weights of all the MC events used in the simulation
    if ( weights.size() > 0 ) mcEvtWeight = weights[0];

    //for ( auto& it : weights ) { Info("execute()", "event weight: %2f.", it ); }

    // kill the powheg event with a huge weight
    if( m_cleanPowheg ) {
      if( eventInfo->eventNumber() == 1652845 ) {
        std::cout << "Dropping huge weight event. Weight should be 352220000" << std::endl;
        std::cout << "WEIGHT : " << mcEvtWeight << std::endl;
        wk()->skipEvent();
        return EL::StatusCode::SUCCESS; // go to next event
      }
    }

    if ( m_doPUreweighting ) {
      m_pileuptool->apply(*eventInfo);
      static SG::AuxElement::ConstAccessor< double > pileupWeightAcc("PileupWeight");
      pileupWeight = pileupWeightAcc(*eventInfo) ;
    }
  }

  // decorate with mc event weight
  static SG::AuxElement::Decorator< float > mcEvtWeightDecor("mcEventWeight");
  mcEvtWeightDecor(*eventInfo) = mcEvtWeight;


  // print every 1000 events, so we know where we are:
  m_cutflowHist ->Fill( m_cutflow_all, 1 );
  m_cutflowHistW->Fill( m_cutflow_all, mcEvtWeight);
  if ( (m_eventCounter % 1000) == 0 ) {
    Info("execute()", "Event number = %i", m_eventCounter);
  }

  if ( m_verbose && (m_eventCounter % 500) == 0 ) {
    Info(m_name.c_str(), "Store Content:");
    m_store->print();
    Info(m_name.c_str(), "End Content");
  }

  // if data check if event passes GRL and even cleaning
  if ( !m_isMC ) {


    if ( m_debug ) {

      // Get the streams that the event was put in
      const std::vector<  xAOD::EventInfo::StreamTag > streams = eventInfo->streamTags();

      for ( auto& it : streams ) {
	      const std::string stream_name = it.name();
	      Info("execute()", "event has fired stream: %s", stream_name.c_str() );
      }
    }

    // GRL
    if ( m_applyGRLCut ) {
      if ( !m_grl->passRunLB( *eventInfo ) ) {
        wk()->skipEvent();
        return EL::StatusCode::SUCCESS; // go to next event
      }
      m_cutflowHist ->Fill( m_cutflow_grl, 1 );
      m_cutflowHistW->Fill( m_cutflow_grl, mcEvtWeight);
    }

    //------------------------------------------------------------
    // Apply event cleaning to remove events due to
    // problematic regions of the detector, and incomplete events.
    // Apply to data.
    //------------------------------------------------------------

    if ( m_applyEventCleaningCut && (eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_lar, 1 );
    m_cutflowHistW->Fill( m_cutflow_lar, mcEvtWeight);

    if ( m_applyEventCleaningCut && (eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_tile, 1 );
    m_cutflowHistW->Fill( m_cutflow_tile, mcEvtWeight);


    if( m_applyEventCleaningCut && (eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_core, 1 );
    m_cutflowHistW->Fill( m_cutflow_core, mcEvtWeight);

  } //if !m_isMC

  // Primary Vertex
  const xAOD::VertexContainer* vertices(nullptr);
  if ( !m_truthLevelOnly && m_applyPrimaryVertexCut ) {
    RETURN_CHECK("BasicEventSelection::execute()", HelperFunctions::retrieve(vertices, m_vertexContainerName, m_event, m_store, m_verbose) ,"");

    if ( !HelperFunctions::passPrimaryVertexSelection( vertices, m_PVNTrack ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
  }
  m_cutflowHist ->Fill( m_cutflow_npv, 1 );
  m_cutflowHistW->Fill( m_cutflow_npv, mcEvtWeight);

  // Trigger
  if ( !m_triggerSelection.empty() ) {
    auto triggerChainGroup = m_trigDecTool->getChainGroup(m_triggerSelection);
    if ( m_applyTriggerCut ) {
      if ( !triggerChainGroup->isPassed() ) {
        wk()->skipEvent();
        return EL::StatusCode::SUCCESS;
      }
      m_cutflowHist ->Fill( m_cutflow_trigger, 1 );
      m_cutflowHistW->Fill( m_cutflow_trigger, mcEvtWeight);

    } // m_applyTriggerCut

    // save passed triggers in eventInfo
    if( m_storeTrigDecisions ) {
      std::vector<std::string> passTriggers;
      for (auto &trigName : triggerChainGroup->getListOfTriggers()) {
        auto trigChain = m_trigDecTool->getChainGroup( trigName );
        if (trigChain->isPassed()) {
          passTriggers.push_back( trigName );
        }
      }
      static SG::AuxElement::Decorator< std::vector< std::string > > passTrigs("passTriggers");
      passTrigs( *eventInfo ) = passTriggers;
    } // storeTrigDecision

    static SG::AuxElement::Decorator< float > weight_prescale("weight_prescale");
    weight_prescale(*eventInfo) = triggerChainGroup->getPrescale();
  } // if giving a specific list of triggers to look at

  if( m_storePassAny ) {
    static SG::AuxElement::Decorator< int > passAny("passAny");
    passAny(*eventInfo) = (int)m_trigDecTool->isPassed(".*");
  }
  if( m_storePassL1 ) {
    static SG::AuxElement::Decorator< int > passL1("passL1");
    passL1(*eventInfo) = (int)m_trigDecTool->isPassed("L1_.*");
  }
  if( m_storePassHLT ) {
    static SG::AuxElement::Decorator< int > passHLT("passHLT");
    passHLT(*eventInfo) = (int)m_trigDecTool->isPassed("HLT_.*");
  }


  if( m_storeTrigKeys ) {
    static SG::AuxElement::Decorator< int > masterKey("masterKey");
    masterKey(*eventInfo) = m_trigConfTool->masterKey();
    static SG::AuxElement::Decorator< int > L1PSKey("L1PSKey");
    L1PSKey(*eventInfo) = m_trigConfTool->lvl1PrescaleKey();
    static SG::AuxElement::Decorator< int > HLTPSKey("HLTPSKey");
    HLTPSKey(*eventInfo) = m_trigConfTool->hltPrescaleKey();
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

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
  if(m_trigDecTool) delete m_trigDecTool;
  if(m_trigConfTool) delete m_trigConfTool;

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

  return EL::StatusCode::SUCCESS;
}
