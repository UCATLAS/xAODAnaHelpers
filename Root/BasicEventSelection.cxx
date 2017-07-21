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

#include "PATInterfaces/CorrectionCode.h"
//#include "AsgTools/StatusCode.h"

// tools
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "PileupReweighting/PileupReweightingTool.h"
#include "TrigConfxAOD/xAODConfigTool.h"
#include "PMGTools/PMGSherpa22VJetsWeightTool.h"

// ROOT include(s):
#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TSystem.h"
#include "xAODCore/tools/IOStats.h"
#include "xAODCore/tools/ReadStats.h"


// this is needed to distribute the algorithm to the workers
ClassImp(BasicEventSelection)

BasicEventSelection :: BasicEventSelection () :
    Algorithm("BasicEventSelection")
{
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
  ANA_MSG_INFO( "Calling setupJob");

  job.useXAOD();
  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("BasicEventSelection").ignore(); // call before opening first file

  EL::OutputStream outForCFlow(m_cutFlowStreamName);
  if(!job.outputHas(m_cutFlowStreamName) ){ job.outputAdd ( outForCFlow ); }

  EL::OutputStream outForMetadata(m_metaDataStreamName);
  if(!job.outputHas(m_metaDataStreamName) ){ job.outputAdd ( outForMetadata ); }

  EL::OutputStream outForDuplicates(m_duplicatesStreamName);
  if(!job.outputHas(m_duplicatesStreamName) ){ job.outputAdd ( outForDuplicates ); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_INFO( "Calling histInitialize");
  ANA_CHECK( xAH::Algorithm::algInitialize());

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

  ANA_MSG_INFO( "Histograms initialized!");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BasicEventSelection :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  ANA_MSG_INFO( "Calling fileExecute");

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
    ANA_MSG_ERROR( "MetaData tree not found! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  MetaData->LoadTree(0);

  //check if file is from a DxAOD
  bool m_isDerivation = !MetaData->GetBranch("StreamAOD");

  if (  m_useMetaData ) {

      // Check for potential file corruption
      //
      // If there are some Incomplete CBK, throw a WARNING,
      // unless ALL of them have inputStream == "unknownStream"
      //
      const xAOD::CutBookkeeperContainer* incompleteCBC(nullptr);
      if ( !m_event->retrieveMetaInput(incompleteCBC, "IncompleteCutBookkeepers").isSuccess() ) {
	  ANA_MSG_ERROR("Failed to retrieve IncompleteCutBookkeepers from MetaData! Exiting.");
	  return EL::StatusCode::FAILURE;
      }
      bool allFromUnknownStream(true);
      if ( incompleteCBC->size() != 0 ) {

	  std::string stream("");
	  for ( auto cbk : *incompleteCBC ) {
	      ANA_MSG_INFO("Incomplete cbk name: " << cbk->name() << " - stream: " << cbk->inputStream());
	      if ( cbk->inputStream() != "unknownStream" ) {
		  allFromUnknownStream = false;
		  stream = cbk->inputStream();
		  break;
	      }
	  }
	  if ( !allFromUnknownStream ) { ANA_MSG_WARNING("Found incomplete CBK from stream: " << stream << ". This is not necessarily a sign of file corruption (incomplete CBK appear when 'maxevents' is set in the AOD jo, for instance), but you may still want to check input file for potential corruption..." ); }

      }

      // Now, let's find the actual information
      //
      const xAOD::CutBookkeeperContainer* completeCBC(nullptr);
      if ( !m_event->retrieveMetaInput(completeCBC, "CutBookkeepers").isSuccess() ) {
	  ANA_MSG_ERROR("Failed to retrieve CutBookkeepers from MetaData! Exiting.");
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

      if ( m_isDerivation ) {
	if(m_derivationName != ""){
	  ANA_MSG_INFO("Override auto config to look at DAOD made by Derivation Algorithm: " << m_derivationName);
	}else{
	  ANA_MSG_INFO("Will autoconfig to look at DAOD made by Derivation Algorithm.");
	}
      }

      int maxCycle(-1);
      for ( const auto& cbk: *completeCBC ) {
	  ANA_MSG_INFO("Complete cbk name: " << cbk->name() << " - stream: " << cbk->inputStream() );
	  if ( cbk->cycle() > maxCycle && cbk->name() == "AllExecutedEvents" && cbk->inputStream() == "StreamAOD" ) {
	      allEventsCBK = cbk;
	      maxCycle = cbk->cycle();
	  }
	  if ( m_isDerivation ) {

	    if(m_derivationName != ""){

	      if ( cbk->name() == m_derivationName ) {
		DxAODEventsCBK = cbk;
	      }

	    } else if( cbk->name().find("Kernel") != std::string::npos ){
	      ANA_MSG_INFO("Auto config found DAOD made by Derivation Algorithm: " << cbk->name());
	      DxAODEventsCBK = cbk;
	    }

	  } // is derivation
      }

      m_MD_initialNevents     = allEventsCBK->nAcceptedEvents();
      m_MD_initialSumW	      = allEventsCBK->sumOfEventWeights();
      m_MD_initialSumWSquared = allEventsCBK->sumOfEventWeightsSquared();

      if ( m_isDerivation && !DxAODEventsCBK ) {
        ANA_MSG_ERROR( "No CutBookkeeper corresponds to the selected Derivation Framework algorithm name. Check it with your DF experts! Aborting.");
        return EL::StatusCode::FAILURE;
      }

      m_MD_finalNevents	      = ( m_isDerivation ) ? DxAODEventsCBK->nAcceptedEvents() : m_MD_initialNevents;
      m_MD_finalSumW	      = ( m_isDerivation ) ? DxAODEventsCBK->sumOfEventWeights() : m_MD_initialSumW;
      m_MD_finalSumWSquared   = ( m_isDerivation ) ? DxAODEventsCBK->sumOfEventWeightsSquared() : m_MD_initialSumWSquared;

      // Write metadata event bookkeepers to histogram
      //
      ANA_MSG_INFO( "Meta data from this file:");
      ANA_MSG_INFO( "Initial  events  = "                << static_cast<unsigned int>(m_MD_initialNevents) );
      ANA_MSG_INFO( "Selected events  = "                << static_cast<unsigned int>(m_MD_finalNevents) );
      ANA_MSG_INFO( "Initial  sum of weights = "         << m_MD_initialSumW);
      ANA_MSG_INFO( "Selected sum of weights = "         << m_MD_finalSumW);
      ANA_MSG_INFO( "Initial  sum of weights squared = " << m_MD_initialSumWSquared);
      ANA_MSG_INFO( "Selected sum of weights squared = " << m_MD_finalSumWSquared);

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

  ANA_MSG_INFO( "Initializing BasicEventSelection... ");

  // if truth level make sure parameters are set properly
  if( m_truthLevelOnly ) {
    ANA_MSG_INFO( "Truth only! Turn off trigger stuff");
    m_triggerSelection      = "";
    m_extraTriggerSelection = "";
    m_applyTriggerCut = m_storeTrigDecisions = m_storePassL1 = m_storePassHLT = m_storeTrigKeys = false;
    ANA_MSG_INFO( "Truth only! Turn off GRL");
    m_applyGRLCut = false;
    ANA_MSG_INFO( "Truth only! Turn off Pile-up Reweight");
    m_doPUreweighting = false;
    m_doPUreweightingSys = false;
  }

  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  m_isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );
  ANA_MSG_DEBUG( "Is MC? " << static_cast<int>(m_isMC) );

  //Protection in case GRL does not apply to this run
  //
  if ( m_applyGRLCut ) {
    std::string runNumString = std::to_string(eventInfo->runNumber());
    if ( m_GRLExcludeList.find( runNumString ) != std::string::npos ) {
      ANA_MSG_INFO( "RunNumber is in GRLExclusion list, setting applyGRL to false");
      m_applyGRLCut = false;
    }
  }

  m_cleanPowheg = false;
  if ( eventInfo->runNumber() == 426005 ) { // Powheg+Pythia J5
    m_cleanPowheg = true;
    ANA_MSG_INFO( "This is J5 Powheg - cleaning that nasty huge weight event");
  }

  //////// Initialize Tool for Sherpa 2.2 Reweighting ////////////
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/CentralMC15ProductionList#Sherpa_v2_2_0_V_jets_NJet_reweig
  m_reweightSherpa22 = false;
  if( m_isMC &&
      ( (eventInfo->mcChannelNumber() >= 363331 && eventInfo->mcChannelNumber() <= 363483 ) ||
        (eventInfo->mcChannelNumber() >= 363102 && eventInfo->mcChannelNumber() <= 363122 ) ||
        (eventInfo->mcChannelNumber() >= 363361 && eventInfo->mcChannelNumber() <= 363435 ) ) ){
    ANA_MSG_INFO( "This is Sherpa 2.2 dataset and should be reweighted.  An extra weight will be saved to EventInfo called \"weight_Sherpa22\".");
    m_reweightSherpa22 = true;

    //Choose Jet Truth container, WZ has more information and is favored by the tool
    std::string pmg_TruthJetContainer = "";
    if( m_event->contains<xAOD::JetContainer>("AntiKt4TruthWZJets") ){
      pmg_TruthJetContainer = "AntiKt4TruthWZJets";
    } else if( m_event->contains<xAOD::JetContainer>("AntiKt4TruthJets") ){
      pmg_TruthJetContainer = "AntiKt4TruthJets";
    } else {
      ANA_MSG_WARNING( "No Truth Jet Container found for Sherpa 22 reweighting, weight_Sherpa22 will not be set.");
      m_reweightSherpa22 = false;
    }

    //Initialize Tool
    if( m_reweightSherpa22 ){

      setToolName(m_reweightSherpa22_tool_handle);
      ANA_CHECK( m_reweightSherpa22_tool_handle.setProperty( "TruthJetContainer", pmg_TruthJetContainer ));
      ANA_CHECK( m_reweightSherpa22_tool_handle.setProperty( "OutputLevel", msg().level() ));
      ANA_CHECK( m_reweightSherpa22_tool_handle.retrieve());
      ANA_MSG_DEBUG("Retrieved tool: " << m_reweightSherpa22_tool_handle);

    }
  }//if isMC and a Sherpa 2.2 sample


  ANA_MSG_INFO( "Setting up histograms");

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
  m_trk_cutflowHist_1    = new TH1D("cutflow_trks_1", "cutflow_trks_1", 1, 1, 2);
  m_trk_cutflowHist_1->SetCanExtend(TH1::kAllAxes);
  m_truth_cutflowHist_1  = new TH1D("cutflow_truths_1", "cutflow_truths_1", 1, 1, 2);
  m_truth_cutflowHist_1->SetCanExtend(TH1::kAllAxes);

  // start labelling the bins for the event cutflow
  //
  m_cutflow_all  = m_cutflowHist->GetXaxis()->FindBin("all");
  m_cutflowHistW->GetXaxis()->FindBin("all");

  if ( ( !m_isMC && m_checkDuplicatesData ) || ( m_isMC && m_checkDuplicatesMC ) ) {
    m_cutflow_duplicates  = m_cutflowHist->GetXaxis()->FindBin("Duplicates");
    m_cutflowHistW->GetXaxis()->FindBin("Duplicates");
  }

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

  // -------------------------------------------------------------------------------------------------

  // Create TTree for bookeeeping duplicated events
  //
  TFile *fileDPL = wk()->getOutputFile (m_duplicatesStreamName);
  fileDPL->cd();

  m_duplicatesTree = new TTree("duplicates","Info on duplicated events");
  m_duplicatesTree->Branch("runNumber",    &m_duplRunNumber,      "runNumber/I");
  m_duplicatesTree->Branch("eventNumber",  &m_duplEventNumber,    "eventNumber/LI");

  // -------------------------------------------------------------------------------------------------

  ANA_MSG_INFO( "Setting Up Tools");

  // 1.
  // initialize the GoodRunsListSelectionTool
  //

  if(m_applyGRLCut){
    std::vector<std::string> vecStringGRL;

    std::string grl;
    std::istringstream ss(m_GRLxml);
    while ( std::getline(ss, grl, ',') ) {
        std::string file = PathResolverFindCalibFile(grl);
        ANA_MSG_DEBUG("Found GRL: " << file);
        vecStringGRL.push_back(file);
    }

    setToolName(m_grl_handle);
    ANA_CHECK( m_grl_handle.setProperty( "GoodRunsListVec", vecStringGRL));
    ANA_CHECK( m_grl_handle.setProperty("PassThrough", false));
    ANA_CHECK( m_grl_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_grl_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_grl_handle);
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
    ANA_MSG_INFO( "Adding Pileup files for CP::PileupReweightingTool:");
    for( unsigned int i=0; i < PRWFiles.size(); ++i){
      printf( "\t %s \n", PRWFiles.at(i).c_str() );
    }
    ANA_MSG_INFO( "Adding LumiCalc files for CP::PileupReweightingTool:");
    for( unsigned int i=0; i < lumiCalcFiles.size(); ++i){
      printf( "\t %s \n", lumiCalcFiles.at(i).c_str() );
    }

    setToolName(m_pileup_tool_handle, "Pileup");
    ANA_CHECK( m_pileup_tool_handle.setProperty("ConfigFiles", PRWFiles));
    ANA_CHECK( m_pileup_tool_handle.setProperty("LumiCalcFiles", lumiCalcFiles));
    ANA_CHECK( m_pileup_tool_handle.setProperty("DataScaleFactor", 1.0/1.09));
    ANA_CHECK( m_pileup_tool_handle.setProperty("DataScaleFactorUP", 1.0));
    ANA_CHECK( m_pileup_tool_handle.setProperty("DataScaleFactorDOWN", 1.0/1.18));
    ANA_CHECK( m_pileup_tool_handle.setProperty("OutputLevel", msg().level() ));
    ANA_CHECK( m_pileup_tool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_pileup_tool_handle);
  }

  // pileup reweighing tool is needed to get the data weight for unprescaling
  if ( m_savePrescaleDataWeight && !m_doPUreweighting) {

    ANA_MSG_ERROR( "m_savePrescaleDataWeight is true but m_doPUreweighting is false !!!");
    return EL::StatusCode::FAILURE;

  }

  // 3.
  // initialize the Trig::TrigDecisionTool
  //
  if( !m_triggerSelection.empty() || !m_extraTriggerSelection.empty() ||
      m_applyTriggerCut || m_storeTrigDecisions || m_storePassL1 || m_storePassHLT || m_storeTrigKeys ) {

    setToolName(m_trigConfTool_handle);
    ANA_CHECK( m_trigConfTool_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_trigConfTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigConfTool_handle);

    setToolName(m_trigDecTool_handle, m_trigDecTool_name);
    ANA_CHECK( m_trigDecTool_handle.setProperty( "ConfigTool", m_trigConfTool_handle ));
    ANA_CHECK( m_trigDecTool_handle.setProperty( "TrigDecisionKey", "xTrigDecision" ));
    ANA_CHECK( m_trigDecTool_handle.setProperty( "OutputLevel", msg().level() ));
    ANA_CHECK( m_trigDecTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);
  }//end trigger configuration

  // As a check, let's see the number of events in our file (long long int)
  //
  ANA_MSG_INFO( "Number of events in file = " << m_event->getEntries());

  // Initialize counter for number of entries
  m_eventCounter   = 0;

  ANA_MSG_INFO( "BasicEventSelection succesfully initialized!");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BasicEventSelection :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG( "Basic Event Selection");

  // Print every 1000 entries, so we know where we are:
  //
  if ( (m_eventCounter % 1000) == 0 ) {
    ANA_MSG_INFO( "Entry number = " << m_eventCounter);
    ANA_MSG_VERBOSE( "Store Content:");
    if(msgLvl(MSG::VERBOSE)) m_store->print();
    ANA_MSG_VERBOSE( "End Content");
  }

  //-----------------------------------------
  // Print triggers used for first entry only
  // and fill the trigger expression for
  // unprescaling data
  //-----------------------------------------

  std::string TriggerExpression = "";

  if ( !m_triggerSelection.empty() ) {
    if (m_eventCounter == 0 || m_savePrescaleDataWeight) {
      if (m_eventCounter == 0) ANA_MSG_INFO( "*** Triggers used (in OR) are:\n");
      auto printingTriggerChainGroup = m_trigDecTool_handle->getChainGroup(m_triggerSelection);
      std::vector<std::string> triggersUsed = printingTriggerChainGroup->getListOfTriggers();
      for ( unsigned int iTrigger = 0; iTrigger < triggersUsed.size(); ++iTrigger ) {
        if (m_eventCounter == 0) printf("    %s\n", triggersUsed.at(iTrigger).c_str());
        TriggerExpression.append(triggersUsed.at(iTrigger).c_str());
        if ( iTrigger != triggersUsed.size() - 1) TriggerExpression.append(" || ");
      }
    }
  }

  if ( m_eventCounter == 0 && !m_extraTriggerSelection.empty() ) {
    ANA_MSG_INFO( "*** Extra Trigger Info Saved are :\n");
    auto printingTriggerChainGroup = m_trigDecTool_handle->getChainGroup(m_extraTriggerSelection);
    std::vector<std::string> triggersUsed = printingTriggerChainGroup->getListOfTriggers();
    for ( unsigned int iTrigger = 0; iTrigger < triggersUsed.size(); ++iTrigger ) {
      printf("    %s\n", triggersUsed.at(iTrigger).c_str());
    }
    printf("\n");
  }



  ++m_eventCounter;

  //------------------
  // Grab event
  //------------------
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  //------------------------------------------------------------------------------------------
  // Declare an 'eventInfo' decorator with the MC event weight
  //------------------------------------------------------------------------------------------
  static SG::AuxElement::Decorator< float > mcEvtWeightDecor("mcEventWeight");
  static SG::AuxElement::Accessor< float >  mcEvtWeightAcc("mcEventWeight");

  float mcEvtWeight(1.0);

  // Check if need to create xAH event weight
  //
  if ( !mcEvtWeightDecor.isAvailable(*eventInfo) ) {
    if ( m_isMC ) {
      const std::vector< float > weights = eventInfo->mcEventWeights(); // The weight (and systs) of all the MC events used in the simulation
      if ( weights.size() > 0 ) mcEvtWeight = weights[0];

      //for ( auto& it : weights ) { ANA_MSG_INFO( "event weight: %2f.", it ); }

      // kill the powheg event with a huge weight
      if( m_cleanPowheg ) {
        if( eventInfo->eventNumber() == 1652845 ) {
          ANA_MSG_INFO("Dropping huge weight event. Weight should be 352220000");
          ANA_MSG_INFO("WEIGHT : " << mcEvtWeight);
          wk()->skipEvent();
          return EL::StatusCode::SUCCESS; // go to next event
        }
      }
    }
    // Decorate event with the *total* MC event weight
    //
    mcEvtWeightDecor(*eventInfo) = mcEvtWeight;
  } else {
    mcEvtWeight = mcEvtWeightAcc(*eventInfo);
  }

  //------------------------------------------------------------------------------------------
  // Declare an 'eventInfo' decorator with prescale weight for unprescaling data
  // https://cds.cern.ch/record/2014726/files/ATL-COM-SOFT-2015-119.pdf (line 130)
  //------------------------------------------------------------------------------------------

  static SG::AuxElement::Decorator< float > prsc_DataWeightDecor("prescale_DataWeight");
  static SG::AuxElement::Accessor< float >  prsc_DataWeightAcc("prescale_DataWeight");

  float prsc_DataWeight(1.0);

  // Check if need to create xAH event weight
  //
  if ( !prsc_DataWeightDecor.isAvailable(*eventInfo) ) {
    if ( !m_isMC && m_savePrescaleDataWeight ) {

      // get mu dependent data weight
      prsc_DataWeight = m_pileup_tool_handle->getDataWeight( *eventInfo, TriggerExpression, true );
    }

    // Decorate event with the *total* MC event weight
    //
    prsc_DataWeightDecor(*eventInfo) = prsc_DataWeight;
  } else {
    prsc_DataWeight = prsc_DataWeightAcc(*eventInfo);
  }


  //------------------------------------------------------------------------------------------
  // Declare an 'eventInfo' decorator with the Sherpa 2.2 reweight to multijet truth
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/CentralMC15ProductionList#Sherpa_v2_2_0_V_jets_NJet_reweig
  //------------------------------------------------------------------------------------------

  if ( m_reweightSherpa22 ){
    static SG::AuxElement::Decorator< float > weight_Sherpa22Decor("weight_Sherpa22");
    // Check if weight needs to be added
    if ( !weight_Sherpa22Decor.isAvailable(*eventInfo) ) {

      float weight_Sherpa22 = -999.;
      weight_Sherpa22 = m_reweightSherpa22_tool_handle->getWeight();
      weight_Sherpa22Decor( *eventInfo ) = weight_Sherpa22;
      ANA_MSG_DEBUG("Setting Sherpa 2.2 reweight to " << weight_Sherpa22);

    } // If not already decorated
  } // if m_reweightSherpa22


  //------------------------------------------------------------------------------------------
  // Fill initial bin of cutflow
  //------------------------------------------------------------------------------------------

  m_cutflowHist ->Fill( m_cutflow_all, 1 );
  m_cutflowHistW->Fill( m_cutflow_all, mcEvtWeight);

  if( !m_useMetaData )
    {
      m_histEventCount -> Fill(1, 1);
      m_histEventCount -> Fill(2, 1);
      m_histEventCount -> Fill(3, mcEvtWeight);
      m_histEventCount -> Fill(4, mcEvtWeight);
      m_histEventCount -> Fill(5, mcEvtWeight*mcEvtWeight);
      m_histEventCount -> Fill(6, mcEvtWeight*mcEvtWeight);
    }

  //--------------------------------------------------------------------------------------------------------
  // Check current event is not a duplicate
  // This is done by checking against the std::set of <runNumber,eventNumber> filled for all previous events
  //--------------------------------------------------------------------------------------------------------

  if ( ( !m_isMC && m_checkDuplicatesData ) || ( m_isMC && m_checkDuplicatesMC ) ) {

    std::pair<uint32_t,uint32_t> thispair = std::make_pair(eventInfo->runNumber(),eventInfo->eventNumber());

    if ( m_RunNr_VS_EvtNr.find(thispair) != m_RunNr_VS_EvtNr.end() ) {

      ANA_MSG_WARNING("Found duplicated event! runNumber = " << static_cast<uint32_t>(eventInfo->runNumber()) << ", eventNumber = " << static_cast<uint32_t>(eventInfo->eventNumber()) << ". Skipping this event");

      // Bookkeep info in duplicates TTree
      //
      m_duplRunNumber   = eventInfo->runNumber();
      m_duplEventNumber = eventInfo->eventNumber();

      m_duplicatesTree->Fill();

      wk()->skipEvent();
      return EL::StatusCode::SUCCESS; // go to next event
    }

    m_RunNr_VS_EvtNr.insert(thispair);

    m_cutflowHist ->Fill( m_cutflow_duplicates, 1 );
    m_cutflowHistW->Fill( m_cutflow_duplicates, mcEvtWeight);

  }

  //------------------------------------------------------------------------------------------
  // Update Pile-Up Reweighting
  //------------------------------------------------------------------------------------------
  if ( m_isMC && m_doPUreweighting ) {
    m_pileup_tool_handle->applySystematicVariation(CP::SystematicSet()).ignore();
    m_pileup_tool_handle->apply( *eventInfo ); // NB: this call automatically decorates eventInfo with:
                                                 //  1.) the PU weight ("PileupWeight")
                                                 //  2.) the corrected mu ("corrected_averageInteractionsPerCrossing")
                                                 //  3.) the random run number ("RandomRunNumber")
                                                 //  4.) the random lumiblock number ("RandomLumiBlockNumber")
      if ( m_doPUreweightingSys ) {
       	CP::SystematicSet tmpSet;tmpSet.insert(CP::SystematicVariation("PRW_DATASF",1));
      	m_pileup_tool_handle->applySystematicVariation( tmpSet ).ignore();
	eventInfo->auxdecor< float >( "PileupWeight_UP" )= m_pileup_tool_handle->getCombinedWeight( *eventInfo );
	tmpSet.clear();tmpSet.insert(CP::SystematicVariation("PRW_DATASF",-1));
	m_pileup_tool_handle->applySystematicVariation( tmpSet ).ignore();
	eventInfo->auxdecor< float >( "PileupWeight_DOWN")= m_pileup_tool_handle->getCombinedWeight( *eventInfo );
      }
  }


  //------------------------------------------------------
  // If data, check if event passes GRL and event cleaning
  //------------------------------------------------------
  if ( !m_isMC ) {

    // Get the streams that the event was put in
    const std::vector<  xAOD::EventInfo::StreamTag > streams = eventInfo->streamTags();

    for ( auto& stream : streams ) {
      ANA_MSG_DEBUG( "event has fired stream: " << stream.name() );
    }

    // GRL
    if ( m_applyGRLCut ) {
      if ( !m_grl_handle->passRunLB( *eventInfo ) ) {
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
    ANA_CHECK( HelperFunctions::retrieve(vertices, m_vertexContainerName, m_event, m_store, msg()) );

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

    auto triggerChainGroup = m_trigDecTool_handle->getChainGroup(m_triggerSelection);

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

      std::vector<std::string>  passTriggers;
      std::vector<float>        triggerPrescales;
      std::vector<std::string>  isPassedBitsNames;
      std::vector<unsigned int> isPassedBits;

      // Save info for the triggers used to skim events
      //
      for ( auto &trigName : triggerChainGroup->getListOfTriggers() ) {
        auto trigChain = m_trigDecTool_handle->getChainGroup( trigName );
        if ( trigChain->isPassed() ) {
          passTriggers.push_back( trigName );
          triggerPrescales.push_back( trigChain->getPrescale() );
        }
	isPassedBitsNames.push_back( trigName );
	isPassedBits     .push_back( m_trigDecTool_handle->isPassedBits(trigName) );
      }

      // Save info for extra triggers
      //
      if ( !m_extraTriggerSelection.empty() ) {

	auto extraTriggerChainGroup = m_trigDecTool_handle->getChainGroup(m_extraTriggerSelection);

	for ( auto &trigName : extraTriggerChainGroup->getListOfTriggers() ) {
	  auto trigChain = m_trigDecTool_handle->getChainGroup( trigName );
	  if ( trigChain->isPassed() ) {
	    passTriggers.push_back( trigName );
	    triggerPrescales.push_back( trigChain->getPrescale() );
	  }
	  isPassedBitsNames.push_back( trigName );
	  isPassedBits     .push_back( m_trigDecTool_handle->isPassedBits(trigName) );
	}
      }

      static SG::AuxElement::Decorator< std::vector< std::string > >  passTrigs("passTriggers");
      passTrigs( *eventInfo ) = passTriggers;
      static SG::AuxElement::Decorator< std::vector< float > >        trigPrescales("triggerPrescales");
      trigPrescales( *eventInfo ) = triggerPrescales;
      static SG::AuxElement::Decorator< std::vector< unsigned int > > isPassBits("isPassedBits");
      isPassBits( *eventInfo ) = isPassedBits;
      static SG::AuxElement::Decorator< std::vector< std::string > >  isPassBitsNames("isPassedBitsNames");
      isPassBitsNames( *eventInfo ) = isPassedBitsNames;

    }

    static SG::AuxElement::Decorator< float > weight_prescale("weight_prescale");
    weight_prescale(*eventInfo) = triggerChainGroup->getPrescale();

    if ( m_storePassL1 ) {
      static SG::AuxElement::Decorator< int > passL1("passL1");
      passL1(*eventInfo)  = ( m_triggerSelection.find("L1_") != std::string::npos )  ? (int)m_trigDecTool_handle->isPassed(m_triggerSelection.c_str()) : -1;
    }
    if ( m_storePassHLT ) {
      static SG::AuxElement::Decorator< int > passHLT("passHLT");
      passHLT(*eventInfo) = ( m_triggerSelection.find("HLT_") != std::string::npos ) ? (int)m_trigDecTool_handle->isPassed(m_triggerSelection.c_str()) : -1;
    }

  } // if giving a specific list of triggers to look at

  if ( m_storeTrigKeys ) {
    static SG::AuxElement::Decorator< int > masterKey("masterKey");
    masterKey(*eventInfo) = m_trigConfTool_handle->masterKey();
    static SG::AuxElement::Decorator< int > L1PSKey("L1PSKey");
    L1PSKey(*eventInfo) = m_trigConfTool_handle->lvl1PrescaleKey();
    static SG::AuxElement::Decorator< int > HLTPSKey("HLTPSKey");
    HLTPSKey(*eventInfo) = m_trigConfTool_handle->hltPrescaleKey();
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

  ANA_MSG_INFO( "Number of processed events \t= " << m_eventCounter);

  m_RunNr_VS_EvtNr.clear();

  if ( m_trigDecTool_handle.isInitialized() )  m_trigDecTool_handle->finalize();

  //after execution loop
  if(m_printBranchList){
    xAOD::IOStats::instance().stats().printSmartSlimmingBranchList();
  }

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
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}
