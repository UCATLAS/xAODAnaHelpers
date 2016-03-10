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
//#include "AsgTools/StatusCode.h"

// ROOT include(s):
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TSystem.h"


// this is needed to distribute the algorithm to the workers
ClassImp(BasicEventSelection)

BasicEventSelection :: BasicEventSelection (std::string className) :
    Algorithm(className),
    m_PU_default_channel(0),
    m_grl(nullptr),
    m_pileup_tool_handle("CP::PileupReweightingTool/PileupToolName", nullptr),
    m_trigConfTool(nullptr),
    m_trigDecTool(nullptr),
    m_histEventCount(nullptr),
    m_cutflowHist(nullptr),
    m_cutflowHistW(nullptr),
    m_el_cutflowHist_1(nullptr),
    m_el_cutflowHist_2(nullptr),
    m_mu_cutflowHist_1(nullptr),
    m_mu_cutflowHist_2(nullptr),
    m_ph_cutflowHist_1(nullptr),
    m_tau_cutflowHist_1(nullptr),
    m_tau_cutflowHist_2(nullptr),
    m_jet_cutflowHist_1(nullptr),
    m_truth_cutflowHist_1(nullptr)
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

  // Output Stream Names
  m_metaDataStreamName = "metadata";
  m_cutFlowStreamName = "cutflow";

  // Check for duplicated events in Data and MC
  m_checkDuplicatesData = false;
  m_checkDuplicatesMC	= false;

  // GRL
  m_applyGRLCut = false;
  m_GRLxml = "$ROOTCOREBIN/data/xAODAnaHelpers/data15_13TeV.periodAllYear_HEAD_DQDefects-00-01-02_PHYS_StandardGRL_Atlas_Ready.xml";
  //https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/GoodRunListsForAnalysis
  m_GRLExcludeList = "";

  // Clean Powheg huge weight
  m_cleanPowheg = false;

  // Pileup Reweighting
  m_doPUreweighting    = false;
  m_lumiCalcFileNames  = "";
  m_PRWFileNames       = "";
  m_PU_default_channel = 0;

  // Primary Vertex
  m_vertexContainerName = "PrimaryVertices";
  m_applyPrimaryVertexCut = false;
  // number of tracks to require to count PVs
  m_PVNTrack = 2; // harmonized cut

  // Event Cleaning
  m_applyEventCleaningCut = false;
  m_applyCoreFlagsCut     = false;

  // Trigger
  m_triggerSelection = "";
  m_applyTriggerCut = false;
  m_storeTrigDecisions = false;
  m_storePassL1 = false;
  m_storePassHLT = false;
  m_storeTrigKeys = false;

  //CP::CorrectionCode::enableFailure();
  //StatusCode::enableFailure();
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


  EL::OutputStream outForCFlow(m_cutFlowStreamName);
  if(!job.outputHas(m_cutFlowStreamName) ){job.outputAdd ( outForCFlow );}

  EL::OutputStream outForMetadata(m_metaDataStreamName);
  if(!job.outputHas(m_metaDataStreamName) ){job.outputAdd ( outForMetadata );}

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "Calling histInitialize");
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");

  // write the metadata hist to this file so algos downstream can pick up the pointer
  TFile *fileMD = wk()->getOutputFile (m_metaDataStreamName);
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
  //
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //---------------------------
  // Meta data - CutBookkepers
  //---------------------------
  //
  // Metadata for intial N (weighted) events are used to correctly normalise MC
  // if running on a MC DAOD which had some skimming applied at the derivation stage

  // get the MetaData tree once a new file is opened, with
  //
  TTree* MetaData = dynamic_cast<TTree*>( wk()->inputFile()->Get("MetaData") );
  if ( !MetaData ) {
    Error("fileExecute()", "MetaData tree not found! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  MetaData->LoadTree(0);

  //check if file is from a DxAOD
  bool m_isDerivation = !MetaData->GetBranch("StreamAOD");

  if (  m_useMetaData ) {

      // check for corruption
      //
      // If there are some Incomplete CBK, throw a FAILURE,
      // unless ALL of them have inputStream == "unknownStream"
      //
      const xAOD::CutBookkeeperContainer* incompleteCBC(nullptr);
      if ( !m_event->retrieveMetaInput(incompleteCBC, "IncompleteCutBookkeepers").isSuccess() ) {
	  Error("fileExecute()","Failed to retrieve IncompleteCutBookkeepers from MetaData! Exiting.");
	  return EL::StatusCode::FAILURE;
      }
      bool allFromUnknownStream(true);
      if ( incompleteCBC->size() != 0 ) {

	  for ( auto cbk : *incompleteCBC ) {
	      if ( cbk->inputStream() != "unknownStream" ) {
		  allFromUnknownStream = false;
		  break;
	      }
	  }
	  if ( !allFromUnknownStream ) {
	      Error("fileExecute()","Found incomplete Bookkeepers! Check file for corruption.");
	      return EL::StatusCode::FAILURE;
	  }

      }

      // Now, let's find the actual information
      //
      const xAOD::CutBookkeeperContainer* completeCBC(nullptr);
      if ( !m_event->retrieveMetaInput(completeCBC, "CutBookkeepers").isSuccess() ) {
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

      if ( m_isDerivation ) { Info("fileExecute()","Looking at DAOD made by Derivation Algorithm: %s", derivationName.c_str()); }

      int maxCycle(-1);
      for ( const auto& cbk: *completeCBC ) {
	  if ( cbk->cycle() > maxCycle && cbk->name() == "AllExecutedEvents" && cbk->inputStream() == "StreamAOD" ) {
	      allEventsCBK = cbk;
	      maxCycle = cbk->cycle();
	  }
	  if ( m_isDerivation ) {
	      if ( cbk->name() == derivationName ) {
		  DxAODEventsCBK = cbk;
	      }
	  }
      }

      m_MD_initialNevents     = allEventsCBK->nAcceptedEvents();
      m_MD_initialSumW	      = allEventsCBK->sumOfEventWeights();
      m_MD_initialSumWSquared = allEventsCBK->sumOfEventWeightsSquared();

      m_MD_finalNevents	      = ( m_isDerivation ) ? DxAODEventsCBK->nAcceptedEvents() : m_MD_initialNevents;
      m_MD_finalSumW	      = ( m_isDerivation ) ? DxAODEventsCBK->sumOfEventWeights() : m_MD_initialSumW;
      m_MD_finalSumWSquared   = ( m_isDerivation ) ? DxAODEventsCBK->sumOfEventWeightsSquared() : m_MD_initialSumWSquared;

      // Write metadata event bookkeepers to histogram
      //
      Info("fileExecute()", "Meta data from this file:");
      Info("fileExecute()", "Initial  events  = %u",	     static_cast<unsigned int>(m_MD_initialNevents) );
      Info("fileExecute()", "Selected events  = %u",	     static_cast<unsigned int>(m_MD_finalNevents) );
      Info("fileExecute()", "Initial  sum of weights = %f",	 m_MD_initialSumW);
      Info("fileExecute()", "Selected sum of weights = %f",	 m_MD_finalSumW);
      Info("fileExecute()", "Initial  sum of weights squared = %f", m_MD_initialSumWSquared);
      Info("fileExecute()", "Selected sum of weights squared = %f", m_MD_finalSumWSquared);

      m_histEventCount -> Fill(1, m_MD_initialNevents);
      m_histEventCount -> Fill(2, m_MD_finalNevents);
      m_histEventCount -> Fill(3, m_MD_initialSumW);
      m_histEventCount -> Fill(4, m_MD_finalSumW);
      m_histEventCount -> Fill(5, m_MD_initialSumWSquared);
      m_histEventCount -> Fill(6, m_MD_finalSumWSquared);

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

  // if truth level make sure parameters are set properly
  if( m_truthLevelOnly ) {
    Info("initialize()", "Truth only! Turn off trigger stuff");
    m_triggerSelection = "";
    m_applyTriggerCut = m_storeTrigDecisions = m_storePassL1 = m_storePassHLT = m_storeTrigKeys = false;
    Info("initialize()", "Truth only! Turn off GRL");
    m_applyGRLCut = false;
    Info("initialize()", "Truth only! Turn off Pile-up Reweight");
    m_doPUreweighting = false;
  }

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("BasicEventSelection::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  m_isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );
  if ( m_debug ) { Info("initialize()", "Is MC? %i", static_cast<int>(m_isMC) ); }

  //Protection in case GRL does not apply to this run
  //
  if ( m_applyGRLCut ) {
    std::string runNumString = std::to_string(eventInfo->runNumber());
    if ( m_GRLExcludeList.find( runNumString ) != std::string::npos ) {
      Info("initialize()", "RunNumber is in GRLExclusion list, setting applyGRL to false");
      m_applyGRLCut = false;
    }
  }

  m_cleanPowheg = false;
  if ( eventInfo->runNumber() == 426005 ) { // Powheg+Pythia J5
    m_cleanPowheg = true;
    Info("initialize()", "This is J5 Powheg - cleaning that nasty huge weight event");
  }

  Info("initialize()", "Setting up histograms");

  // write the cutflows to this file so algos downstream can pick up the pointer
  //
  TFile *fileCF = wk()->getOutputFile (m_cutFlowStreamName);
  fileCF->cd();

  // Note: the following code is needed for anyone developing/running in ROOT 6.04.10+
  // Bin extension is not done anymore via TH1::SetBit(TH1::kCanRebin), but with TH1::SetCanExtend(TH1::kAllAxes)

  //initialise event cutflow, which will be picked ALSO by the algos downstream where an event selection is applied (or at least can be applied)
  //
  // use 1,1,2 so Fill(bin) and GetBinContent(bin) refer to the same bin
  //
  m_cutflowHist  = new TH1D("cutflow", "cutflow", 1, 1, 2);
  m_cutflowHist->SetCanExtend(TH1::kAllAxes);
  // use 1,1,2 so Fill(bin) and GetBinContent(bin) refer to the same bin
  //
  m_cutflowHistW = new TH1D("cutflow_weighted", "cutflow_weighted", 1, 1, 2);
  m_cutflowHistW->SetCanExtend(TH1::kAllAxes);

  // initialise object cutflows, which will be picked by the object selector algos downstream and filled.
  //
  m_el_cutflowHist_1     = new TH1D("cutflow_electrons_1", "cutflow_electrons_1", 1, 1, 2);
  m_el_cutflowHist_1->SetCanExtend(TH1::kAllAxes);
  m_el_cutflowHist_2     = new TH1D("cutflow_electrons_2", "cutflow_electrons_2", 1, 1, 2);
  m_el_cutflowHist_2->SetCanExtend(TH1::kAllAxes);
  m_mu_cutflowHist_1     = new TH1D("cutflow_muons_1", "cutflow_muons_1", 1, 1, 2);
  m_mu_cutflowHist_1->SetCanExtend(TH1::kAllAxes);
  m_mu_cutflowHist_2     = new TH1D("cutflow_muons_2", "cutflow_muons_2", 1, 1, 2);
  m_mu_cutflowHist_2->SetCanExtend(TH1::kAllAxes);
  m_ph_cutflowHist_1     = new TH1D("cutflow_photons_1", "cutflow_photons_1", 1, 1, 2);
  m_ph_cutflowHist_1->SetCanExtend(TH1::kAllAxes);
  m_tau_cutflowHist_1     = new TH1D("cutflow_taus_1", "cutflow_taus_1", 1, 1, 2);
  m_tau_cutflowHist_1->SetCanExtend(TH1::kAllAxes);
  m_tau_cutflowHist_2     = new TH1D("cutflow_taus_2", "cutflow_taus_2", 1, 1, 2);
  m_tau_cutflowHist_2->SetCanExtend(TH1::kAllAxes);
  m_jet_cutflowHist_1    = new TH1D("cutflow_jets_1", "cutflow_jets_1", 1, 1, 2);
  m_jet_cutflowHist_1->SetCanExtend(TH1::kAllAxes);
  m_truth_cutflowHist_1  = new TH1D("cutflow_truths_1", "cutflow_truths_1", 1, 1, 2);
  m_truth_cutflowHist_1->SetCanExtend(TH1::kAllAxes);

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
    m_cutflow_SCT = m_cutflowHist->GetXaxis()->FindBin("SCT");
    m_cutflowHistW->GetXaxis()->FindBin("SCT");
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


  // 1.
  // initialize the GoodRunsListSelectionTool
  //

  if(m_applyGRLCut){
    m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
    std::vector<std::string> vecStringGRL;
    m_GRLxml = gSystem->ExpandPathName( m_GRLxml.c_str() );
    vecStringGRL.push_back(m_GRLxml);
    RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty( "GoodRunsListVec", vecStringGRL), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_grl->setProperty("PassThrough", false), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_grl->initialize(), "");
  }

  // 2.
  // initialize the CP::PileupReweightingTool
  //

  if ( m_doPUreweighting ) {

    std::vector<std::string> PRWFiles;
    std::vector<std::string> lumiCalcFiles;

    std::string tmp_lumiCalcFileNames = m_lumiCalcFileNames;
    std::string tmp_PRWFileNames      = m_PRWFileNames;

    // Parse all comma seperated files
    //
    while ( tmp_PRWFileNames.size() > 0) {
      size_t pos = tmp_PRWFileNames.find_first_of(',');
      if ( pos == std::string::npos ) {
        pos = tmp_PRWFileNames.size();
        PRWFiles.push_back(tmp_PRWFileNames.substr(0, pos));
        tmp_PRWFileNames.erase(0, pos);
      } else {
        PRWFiles.push_back(tmp_PRWFileNames.substr(0, pos));
        tmp_PRWFileNames.erase(0, pos+1);
      }
    }
    while ( tmp_lumiCalcFileNames.size() > 0) {
      size_t pos = tmp_lumiCalcFileNames.find_first_of(',');
      if ( pos == std::string::npos ) {
        pos = tmp_lumiCalcFileNames.size();
        lumiCalcFiles.push_back(tmp_lumiCalcFileNames.substr(0, pos));
        tmp_lumiCalcFileNames.erase(0, pos);
      } else {
        lumiCalcFiles.push_back(tmp_lumiCalcFileNames.substr(0, pos));
        tmp_lumiCalcFileNames.erase(0, pos+1);
      }
    }
    Info("initialize()", "Adding Pileup files for CP::PileupReweightingTool:");
    for( unsigned int i=0; i < PRWFiles.size(); ++i){
      printf( "\t %s \n", PRWFiles.at(i).c_str() );
    }
    Info("initialize()", "Adding LumiCalc files for CP::PileupReweightingTool:");
    for( unsigned int i=0; i < lumiCalcFiles.size(); ++i){
      printf( "\t %s \n", lumiCalcFiles.at(i).c_str() );
    }

    RETURN_CHECK("BasicEventSelection::initialize()", checkToolStore<CP::PileupReweightingTool>("Pileup"), "" );
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.makeNew<CP::PileupReweightingTool>("CP::PileupReweightingTool/Pileup"), "Failed to create handle to CP::PileupReweightingTool");
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.setProperty("ConfigFiles", PRWFiles), "");
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.setProperty("LumiCalcFiles", lumiCalcFiles), "");
    if ( m_PU_default_channel ) {
      RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.setProperty("DefaultChannel", m_PU_default_channel), "");
    }
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.setProperty("DataScaleFactor", 1.0/1.16), "Failed to set pileup reweighting data scale factor");
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.setProperty("DataScaleFactorUP", 1.0), "Failed to set pileup reweighting data scale factor up");
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.setProperty("DataScaleFactorDOWN", 1.0/1.23), "Failed to set pileup reweighting data scale factor down");
    RETURN_CHECK("BasicEventSelection::initialize()", m_pileup_tool_handle.initialize(), "Failed to properly initialize CP::PileupReweightingTool");
    //m_pileup_tool_handle->EnableDebugging(true);

  }

  // 3.
  // initialize the Trig::TrigDecisionTool
  //

  if( !m_triggerSelection.empty() || m_applyTriggerCut || m_storeTrigDecisions || m_storePassL1 || m_storePassHLT || m_storeTrigKeys ) {

    //if it's there, it must be already configured
    if ( asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) && asg::ToolStore::contains<TrigConf::xAODConfigTool>("xAODConfigTool")) {

      m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
      m_trigConfTool = asg::ToolStore::get<TrigConf::xAODConfigTool>("xAODConfigTool");

    }

    else {

      m_trigConfTool = new TrigConf::xAODConfigTool( "xAODConfigTool" );
      RETURN_CHECK("BasicEventSelection::initialize()", m_trigConfTool->initialize(), "Failed to properly initialize TrigConf::xAODConfigTool");
      ToolHandle< TrigConf::ITrigConfigTool > configHandle( m_trigConfTool );

      m_trigDecTool = new Trig::TrigDecisionTool( "TrigDecisionTool" );
      RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "ConfigTool", configHandle ), "");
      RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "TrigDecisionKey", "xTrigDecision" ), "");
      RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "OutputLevel", MSG::ERROR), "");
      RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->initialize(), "Failed to properly initialize Trig::TrigDecisionTool");
      Info("initialize()", "Successfully configured Trig::TrigDecisionTool!");

    }

  }//end trigger configuration

  // As a check, let's see the number of events in our file (long long int)
  //
  Info("initialize()", "Number of events in file = %lli", m_event->getEntries());
  // count number of events
  m_eventCounter   = 0;

  Info("initialize()", "BasicEventSelection succesfully initialized!");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if( m_debug ) { Info("execute()", "Basic Event Selection"); }

  //------------------
  // Event information
  //------------------
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("BasicEventSelection::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  //--------------------------------------------------------------------------------------------------------
  // Before counting events, check  current event is not a duplicate
  // This is done by checking against the std::set of <runNumber,eventNumber> filled for all previous events
  //--------------------------------------------------------------------------------------------------------

  if ( ( !m_isMC && m_checkDuplicatesData ) || ( m_isMC && m_checkDuplicatesMC ) ) {
    std::pair<uint32_t,uint32_t> thispair = std::make_pair(eventInfo->runNumber(),eventInfo->eventNumber());
    if ( m_RunNr_VS_EvtNr.find(thispair) != m_RunNr_VS_EvtNr.end() ) {
      if ( m_debug ) { Warning("execute()","Found duplicated event! runNumber = %u, eventNumber = %u. Skipping this event", static_cast<uint32_t>(eventInfo->runNumber()),static_cast<uint32_t>(eventInfo->eventNumber()) ); }
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS; // go to next event
    }
    m_RunNr_VS_EvtNr.insert(thispair);
  }

  ++m_eventCounter;

  //-----------------------------------------
  //Print trigger's used for first event only
  //-----------------------------------------
  if ( m_eventCounter == 1 && !m_triggerSelection.empty() ) {
    Info("execute()", "*** Triggers used (in OR) are:\n");
    auto printingTriggerChainGroup = m_trigDecTool->getChainGroup(m_triggerSelection);
    std::vector<std::string> triggersUsed = printingTriggerChainGroup->getListOfTriggers();
    for ( unsigned int iTrigger = 0; iTrigger < triggersUsed.size(); ++iTrigger ) {
      printf("    %s\n", triggersUsed.at(iTrigger).c_str());
    }
    printf("\n");
  }

  //------------------------------------------------------------------------------------------
  // Update Pile-Up Reweighting
  //------------------------------------------------------------------------------------------
  if ( m_isMC && m_doPUreweighting ) {
      m_pileup_tool_handle->apply( *eventInfo ); // NB: this call automatically decorates eventInfo with:
                                                 //  1.) the PU weight ("PileupWeight")
                                                 //  2.) the corrected mu ("corrected_averageInteractionsPerCrossing")
                                                 //  3.) the random run number ("RandomRunNumber")
                                                 //  4.) the random lumiblock number ("RandomLumiBlockNumber")
  }

  //------------------------------------------------------------------------------------------
  // Declare an 'eventInfo' decorator with the *total* MC event weight
  // This will be the product of all the weights, SFs applied to MC downstream from this algo!
  //------------------------------------------------------------------------------------------
  static SG::AuxElement::Decorator< float > mcEvtWeightDecor("mcEventWeight");
  static SG::AuxElement::Accessor< float >  mcEvtWeightAcc("mcEventWeight");

  float mcEvtWeight(1.0);

  // Check if need to create xAH event weight
  if(!mcEvtWeightDecor.isAvailable(*eventInfo)) {
    if ( m_isMC ) {
      const std::vector< float > weights = eventInfo->mcEventWeights(); // The weight (and systs) of all the MC events used in the simulation
      if ( weights.size() > 0 ) mcEvtWeight = weights[0];

      //for ( auto& it : weights ) { Info("execute()", "event weight: %2f.", it ); }

      // kill the powheg event with a huge weight
      if( m_cleanPowheg ) {
	if( eventInfo->eventNumber() == 1652845 ) {
	  Info("execute()","Dropping huge weight event. Weight should be 352220000");
	  Info("execute()","WEIGHT : %f ", mcEvtWeight);
	  wk()->skipEvent();
	  return EL::StatusCode::SUCCESS; // go to next event
	}
      }
    }
    // Decorate event with the *total* MC event weight
    //
    mcEvtWeightDecor(*eventInfo) = mcEvtWeight;
  } else {
    mcEvtWeight=mcEvtWeightAcc(*eventInfo);
  }

  // print every 1000 events, so we know where we are:
  //
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

  //-----------------------------------------------------
  // If data, check if event passes GRL and even cleaning
  //-----------------------------------------------------
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

    if ( m_applyEventCleaningCut && (eventInfo->errorState(xAOD::EventInfo::SCT)==xAOD::EventInfo::Error) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_SCT, 1 );
    m_cutflowHistW->Fill( m_cutflow_SCT, mcEvtWeight);

    if ( m_applyCoreFlagsCut && (eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) ) ) {
      wk()->skipEvent();
      return EL::StatusCode::SUCCESS;
    }
    m_cutflowHist ->Fill( m_cutflow_core, 1 );
    m_cutflowHistW->Fill( m_cutflow_core, mcEvtWeight);

  }

  //-----------------------------
  // Primary Vertex 'quality' cut
  //-----------------------------

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

  //---------------------
  // Trigger decision cut
  //---------------------

  if ( !m_triggerSelection.empty() ) {

    auto triggerChainGroup = m_trigDecTool->getChainGroup(m_triggerSelection);

    if ( m_applyTriggerCut ) {

      if ( !triggerChainGroup->isPassed() ) {
        wk()->skipEvent();
        return EL::StatusCode::SUCCESS;
      }
      m_cutflowHist ->Fill( m_cutflow_trigger, 1 );
      m_cutflowHistW->Fill( m_cutflow_trigger, mcEvtWeight);

    }

    // save passed triggers in eventInfo
    //
    if ( m_storeTrigDecisions ) {

      std::vector<std::string> passTriggers;
      std::vector<float> triggerPrescales;

      for ( auto &trigName : triggerChainGroup->getListOfTriggers() ) {
        auto trigChain = m_trigDecTool->getChainGroup( trigName );
        if ( trigChain->isPassed() ) {
          passTriggers.push_back( trigName );
          triggerPrescales.push_back( trigChain->getPrescale() );
        }
      }
      static SG::AuxElement::Decorator< std::vector< std::string > > passTrigs("passTriggers");
      passTrigs( *eventInfo ) = passTriggers;
      static SG::AuxElement::Decorator< std::vector< float > > trigPrescales("triggerPrescales");
      trigPrescales( *eventInfo ) = triggerPrescales;

    }

    static SG::AuxElement::Decorator< float > weight_prescale("weight_prescale");
    weight_prescale(*eventInfo) = triggerChainGroup->getPrescale();

    if ( m_storePassL1 ) {
      static SG::AuxElement::Decorator< int > passL1("passL1");
      passL1(*eventInfo)  = ( m_triggerSelection.find("L1_") != std::string::npos )  ? (int)m_trigDecTool->isPassed(m_triggerSelection.c_str()) : -1;
    }
    if ( m_storePassHLT ) {
      static SG::AuxElement::Decorator< int > passHLT("passHLT");
      passHLT(*eventInfo) = ( m_triggerSelection.find("HLT_") != std::string::npos ) ? (int)m_trigDecTool->isPassed(m_triggerSelection.c_str()) : -1;
    }

  } // if giving a specific list of triggers to look at

  if ( m_storeTrigKeys ) {
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

  Info("finalize()", "Number of processed events \t= %i", m_eventCounter);

  m_RunNr_VS_EvtNr.clear();

  if ( m_grl )          { delete m_grl; m_grl = nullptr; }
  if ( m_trigDecTool )  { delete m_trigDecTool;  m_trigDecTool = nullptr; }
  if ( m_trigConfTool ) { delete m_trigConfTool;  m_trigConfTool = nullptr; }

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
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
