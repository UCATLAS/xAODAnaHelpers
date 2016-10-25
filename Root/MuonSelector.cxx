// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <tuple>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/TrackParticlexAODHelpers.h"

// package include(s):
#include "xAODAnaHelpers/MuonSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include "PATCore/TAccept.h"
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "IsolationSelection/IsolationSelectionTool.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include "TriggerMatchingTool/MatchingTool.h"

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(MuonSelector)

MuonSelector :: MuonSelector (std::string className) :
    Algorithm(className),
    m_cutflowHist(nullptr),
    m_cutflowHistW(nullptr),
    m_mu_cutflowHist_1(nullptr),
    m_mu_cutflowHist_2(nullptr),
    m_isolationSelectionTool_handle("CP::IsolationSelectionTool/MuonIsoSelToolName", nullptr),
    m_muonSelectionTool_handle("CP::MuonSelectionTool/MuonSelToolName", nullptr),
    m_trigDecTool(nullptr),
    m_trigMuonMatchTool_handle("Trig::MatchingTool/TrigMuonMatchingName", nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonSelector()", "Calling constructor");

  m_debug                   = false;
  m_useCutFlow              = true;

  // checks if the algorithm has been used already
  //
  m_isUsedBefore            = false;

  // input container to be read from TEvent or TStore
  //
  m_inContainerName         = "";

  // Systematics stuff
  //
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "MuonSelector_Syst";

  // decorate selected objects that pass the cuts
  //
  m_decorateSelectedObjects = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::VIEW_ELEMENTS option
  //                            decorrating and output container should not be mutually exclusive
  m_createSelectedContainer = false;
  // if requested, a new container is made using the SG::VIEW_ELEMENTS option
  m_outContainerName        = "";

  // if only want to look at a subset of object
  //
  m_nToProcess              = -1;

  // configurable cuts
  //
  m_muonQualityStr          = "Medium";
  m_muonType                = "";
  m_pass_max                = -1;
  m_pass_min                = -1;
  m_pT_max                  = 1e8;
  m_pT_min                  = 1e8;
  m_eta_max                 = 1e8;
  m_d0_max                  = 1e8;
  m_d0sig_max     	        = 1e8;
  m_z0sintheta_max          = 1e8;

  m_removeCosmicMuon        = false;
  m_removeEventBadMuon      = true;

  // isolation stuff
  //
  m_MinIsoWPCut             = "";
  m_IsoWPList		    = "LooseTrackOnly,Loose,Tight,Gradient,GradientLoose";
  m_CaloIsoEff              = "0.1*x+90";
  m_TrackIsoEff             = "98";
  m_CaloBasedIsoType        = "topoetcone20";
  m_TrackBasedIsoType       = "ptvarcone30";

  // trigger matching stuff
  //
  m_singleMuTrigChains      = "";
  m_diMuTrigChains          = "";
  m_minDeltaR               = 0.1; // Recommended threshold for muon triggers: see https://svnweb.cern.ch/trac/atlasoff/browser/Trigger/TrigAnalysis/TriggerMatchingTool/trunk/src/TestMatchingToolAlg.cxx
  m_doTrigMatch             = true;

}

MuonSelector::~MuonSelector() {}

EL::StatusCode MuonSelector :: setupJob (EL::Job& job)
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
  xAOD::Init( "MuonSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "Calling histInitialize");
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing MuonSelector Interface... ");

  // Let's see if the algorithm has been already used before:
  // if yes, will write object cutflow in a different histogram!
  //
  // This is the case when the selector algorithm is used for
  // preselecting objects, and then again for the final selection
  //
  Info("initialize()", "Algorithm name: '%s' - of type '%s' ", (this->m_name).c_str(), (this->m_className).c_str() );
  if ( this->numInstances() > 0 ) {
    m_isUsedBefore = true;
    Info("initialize()", "\t An algorithm of the same type has been already used %i times", this->numInstances() );
  }

  if ( m_useCutFlow ) {

    // retrieve the file in which the cutflow hists are stored
    //
    TFile *file     = wk()->getOutputFile ("cutflow");

    // retrieve the event cutflows
    //
    m_cutflowHist  = (TH1D*)file->Get("cutflow");
    m_cutflowHistW = (TH1D*)file->Get("cutflow_weighted");
    m_cutflow_bin  = m_cutflowHist->GetXaxis()->FindBin(m_name.c_str());
    m_cutflowHistW->GetXaxis()->FindBin(m_name.c_str());

    // retrieve the object cutflow
    //
    m_mu_cutflowHist_1  = (TH1D*)file->Get("cutflow_muons_1");

    m_mu_cutflow_all                  = m_mu_cutflowHist_1->GetXaxis()->FindBin("all");
    m_mu_cutflow_eta_and_quaility_cut = m_mu_cutflowHist_1->GetXaxis()->FindBin("eta_and_quality_cut");
    m_mu_cutflow_ptmax_cut            = m_mu_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");
    m_mu_cutflow_ptmin_cut            = m_mu_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");
    m_mu_cutflow_type_cut             = m_mu_cutflowHist_1->GetXaxis()->FindBin("type_cut");
    m_mu_cutflow_z0sintheta_cut       = m_mu_cutflowHist_1->GetXaxis()->FindBin("z0sintheta_cut");
    m_mu_cutflow_d0_cut               = m_mu_cutflowHist_1->GetXaxis()->FindBin("d0_cut");
    m_mu_cutflow_d0sig_cut            = m_mu_cutflowHist_1->GetXaxis()->FindBin("d0sig_cut");
    m_mu_cutflow_iso_cut              = m_mu_cutflowHist_1->GetXaxis()->FindBin("iso_cut");
    if( m_removeCosmicMuon )
      m_mu_cutflow_cosmic_cut              = m_mu_cutflowHist_1->GetXaxis()->FindBin("cosmic_cut");


    if ( m_isUsedBefore ) {
      m_mu_cutflowHist_2 = (TH1D*)file->Get("cutflow_muons_2");

      m_mu_cutflow_all 		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("all");
      m_mu_cutflow_eta_and_quaility_cut = m_mu_cutflowHist_2->GetXaxis()->FindBin("eta_and_quality_cut");
      m_mu_cutflow_ptmax_cut		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("ptmax_cut");
      m_mu_cutflow_ptmin_cut		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("ptmin_cut");
      m_mu_cutflow_type_cut		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("type_cut");
      m_mu_cutflow_z0sintheta_cut	 = m_mu_cutflowHist_2->GetXaxis()->FindBin("z0sintheta_cut");
      m_mu_cutflow_d0_cut		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("d0_cut");
      m_mu_cutflow_d0sig_cut		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("d0sig_cut");
      m_mu_cutflow_iso_cut		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("iso_cut");
      if( m_removeCosmicMuon )
        m_mu_cutflow_cosmic_cut		 = m_mu_cutflowHist_2->GetXaxis()->FindBin("cosmic_cut");
    }

  }// if m_useCutFlow

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  HelperClasses::EnumParser<xAOD::Muon::Quality> muQualityParser;
  m_muonQuality             = static_cast<int>( muQualityParser.parseEnum(m_muonQualityStr) );


  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

  std::set<int> muonQualitySet;
  muonQualitySet.insert(0);
  muonQualitySet.insert(1);
  muonQualitySet.insert(2);
  muonQualitySet.insert(3);
  if ( muonQualitySet.find(m_muonQuality) == muonQualitySet.end() ) {
    Error("initialize()", "Unknown muon quality requested: %i!",m_muonQuality);
    return EL::StatusCode::FAILURE;
  }

  std::set<std::string> muonTypeSet;
  muonTypeSet.insert("");
  muonTypeSet.insert("Combined");
  muonTypeSet.insert("MuonStandAlone");
  muonTypeSet.insert("SegmentTagged");
  muonTypeSet.insert("CaloTagged");
  muonTypeSet.insert("SiliconAssociatedForwardMuon");
  if ( muonTypeSet.find(m_muonType) == muonTypeSet.end() ) {
    Error("initialize()", "Unknown muon type requested: %s!",m_muonType.c_str());
    return EL::StatusCode::FAILURE;
  }

  // Parse input isolation WP list, split by comma, and put into a vector for later use
  // Make sure it's not empty!
  //
  if ( m_IsoWPList.empty() ) {
    m_IsoWPList	= "LooseTrackOnly,Loose,Tight,Gradient,GradientLoose";
  }
  std::string token;
  std::istringstream ss(m_IsoWPList);
  while ( std::getline(ss, token, ',') ) {
    m_IsoKeys.push_back(token);
  }

  if ( m_inContainerName.empty() ){
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  // ********************************
  //
  // Initialise CP::MuonSelectionTool
  //
  // ********************************
  //
  // Set eta and quality requirements in order to accept the muon - ID tracks required by default
  //

  m_muonSelectionTool_name                  = "MuonSelectionTool_" + m_name;
  std::string muonSelectionTool_handle_name = "CP::MuonSelectionTool/" + m_muonSelectionTool_name;

  RETURN_CHECK("MuonSelector::initialize()", checkToolStore<CP::MuonSelectionTool>(m_muonSelectionTool_name), "" );
  //RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool_handle.makeNew<CP::MuonSelectionTool>(muonSelectionTool_handle_name), "Failed to create handle to CP::MuonSelectionTool");
  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool_handle.make(muonSelectionTool_handle_name), "Failed to create handle to CP::MuonSelectionTool");
  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool_handle.setProperty( "MaxEta", static_cast<double>(m_eta_max) ),"Failed to set Failed to set MaxEta property");
  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool_handle.setProperty( "MuQuality", m_muonQuality ),"Failed to set MuQuality property");
  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool_handle.initialize(), "Failed to properly initialize CP::MuonSelectionTool");

  // *************************************
  //
  // Initialise CP::IsolationSelectionTool
  //
  // *************************************

  m_isolationSelectionTool_name                  = "IsolationSelectionTool_" + m_name;
  std::string isolationSelectionTool_handle_name = "CP::IsolationSelectionTool/" + m_isolationSelectionTool_name;

  RETURN_CHECK("MuonSelector::initialize()", checkToolStore<CP::IsolationSelectionTool>(m_isolationSelectionTool_name), "" );
  //RETURN_CHECK("MuonSelector::initialize()", m_isolationSelectionTool_handle.makeNew<CP::IsolationSelectionTool>(isolationSelectionTool_handle_name), "Failed to create handle to CP::IsolationSelectionTool");
  RETURN_CHECK("MuonSelector::initialize()", m_isolationSelectionTool_handle.make(isolationSelectionTool_handle_name), "Failed to create handle to CP::IsolationSelectionTool");
  // Do this only for the first WP in the list
  //
  if ( m_debug ) { Info("initialize()", "Adding isolation WP %s to IsolationSelectionTool", (m_IsoKeys.at(0)).c_str() ); }
  RETURN_CHECK("MuonSelector::initialize()", m_isolationSelectionTool_handle.setProperty("MuonWP", (m_IsoKeys.at(0)).c_str()), "Failed to configure base WP" );
  RETURN_CHECK("MuonSelector::initialize()", m_isolationSelectionTool_handle.initialize(), "Failed to properly initialize IsolationSelectionTool." );

  // Add the remaining input WPs to the tool
  // (start from 2nd element)
  //
  for ( auto WP_itr = std::next(m_IsoKeys.begin()); WP_itr != m_IsoKeys.end(); ++WP_itr ) {

     if ( m_debug ) { Info("initialize()", "Adding extra isolation WP %s to IsolationSelectionTool", (*WP_itr).c_str() ); }

     if ( (*WP_itr).find("UserDefined") != std::string::npos ) {

       HelperClasses::EnumParser<xAOD::Iso::IsolationType> isoParser;

       std::vector< std::pair<xAOD::Iso::IsolationType, std::string> > myCuts;
       myCuts.push_back(std::make_pair<xAOD::Iso::IsolationType, std::string>(isoParser.parseEnum(m_TrackBasedIsoType), m_TrackIsoEff.c_str() ));
       myCuts.push_back(std::make_pair<xAOD::Iso::IsolationType, std::string>(isoParser.parseEnum(m_CaloBasedIsoType) , m_CaloIsoEff.c_str()  ));

       CP::IsolationSelectionTool::IsoWPType iso_type(CP::IsolationSelectionTool::Efficiency);
       if ( (*WP_itr).find("Cut") != std::string::npos ) { iso_type = CP::IsolationSelectionTool::Cut; }

       RETURN_CHECK( "MuonSelector::initialize()",  m_isolationSelectionTool_handle->addUserDefinedWP((*WP_itr).c_str(), xAOD::Type::Muon, myCuts, "", iso_type), "Failed to add user-defined isolation WP" );

     } else {

        RETURN_CHECK( "MuonSelector::initialize()", m_isolationSelectionTool_handle->addMuonWP( (*WP_itr).c_str() ), "Failed to add isolation WP" );

     }
  }

  //m_isolationSelectionTool_handle->msg().setLevel( MSG::ERROR); // ERROR, VERBOSE, DEBUG, INFO


  // **************************************
  //
  // Initialise Trig::MatchingTool
  //
  // **************************************

  // NB: need to retrieve the TrigDecisionTool from asg::ToolStore to configure the tool!
  //     do not initialise if there are no input trigger chains, or the TrigDecisionTool hasn't been configured yet
  //
  if( !( m_singleMuTrigChains.empty() && m_diMuTrigChains.empty() ) && asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) ) {

    m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
    ToolHandle<Trig::TrigDecisionTool> trigDecHandle( m_trigDecTool );

    //  everything went fine, let's initialise the tool!
    //
    m_trigMuonMatchTool_name                  = "MatchingTool_" + m_name;
    std::string trigMuonMatchTool_handle_name = "Trig::MatchingTool/" + m_trigMuonMatchTool_name;

    RETURN_CHECK("MuonSelector::initialize()", checkToolStore<Trig::MatchingTool>(m_trigMuonMatchTool_name), "" );
    //RETURN_CHECK("MuonSelector::initialize()", m_trigMuonMatchTool_handle.makeNew<Trig::MatchingTool>(trigMuonMatchTool_handle_name), "Failed to create handle to MatchingTool");
    RETURN_CHECK("MuonSelector::initialize()", m_trigMuonMatchTool_handle.make(trigMuonMatchTool_handle_name), "Failed to create handle to MatchingTool");
    RETURN_CHECK("MuonSelector::initialize()", m_trigMuonMatchTool_handle.setProperty( "TrigDecisionTool", trigDecHandle ), "Failed to pass TrigDecisionTool to MatchingTool" );
    RETURN_CHECK("MuonSelector::initialize()", m_trigMuonMatchTool_handle.initialize(), "Failed to properly initialize MatchingTool" );

  } else {

    m_doTrigMatch = false;

    std::cout << "***********************************************************" << std::endl;
    Warning("initialize()", "Will not perform any muon trigger matching at this stage b/c :");
    std::cout << "" << std::endl;
    std::cout << "\t -) could not find the TrigDecisionTool in asg::ToolStore" << std::endl;
    std::cout << "\t AND/OR" << std::endl;
    std::cout << "\t -) all input HLT trigger chain lists are empty" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "However, if you really didn't want to do the matching now, it's all good!" << std::endl;
    std::cout << "***********************************************************" << std::endl;
  }

  Info("initialize()", "MuonSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Muon Selection..." ); }

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  // MC event weight
  //
  float mcEvtWeight(1.0);
  static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
  if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
    Error("execute()  ", "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }
  mcEvtWeight = mcEvtWeightAcc( *eventInfo );

  m_numEvent++;

  // QUESTION: why this must be done in execute(), and does not work in initialize()?
  //
  if ( m_numEvent == 1 && m_trigDecTool ) {

    // parse input muon trigger chain list, split by comma and fill vector
    //
    std::string singlemu_trig;
    std::istringstream ss_singlemu_trig(m_singleMuTrigChains);

    while ( std::getline(ss_singlemu_trig, singlemu_trig, ',') ) {
   	m_singleMuTrigChainsList.push_back(singlemu_trig);
    }

    std::string dimu_trig;
    std::istringstream ss_dimu_trig(m_diMuTrigChains);

    while ( std::getline(ss_dimu_trig, dimu_trig, ',') ) {
   	m_diMuTrigChainsList.push_back(dimu_trig);
    }

    Info("execute()", "Input single muon trigger chains that will be considered for matching:\n");
    for ( auto const &chain : m_singleMuTrigChainsList ) { Info("execute()", "\t %s", chain.c_str()); }
    Info("execute()", "\n");

    Info("execute()", "Input di-muon trigger chains that will be considered for matching:\n");
    for ( auto const &chain : m_diMuTrigChainsList ) { Info("execute()", "\t %s", chain.c_str()); }
    Info("execute()", "\n");

  }

  // did any collection pass the cuts?
  //
  bool eventPass(false);
  bool countPass(true); // for cutflow: count for the 1st collection in the syst container - could be better as should only count for the nominal
  const xAOD::MuonContainer* inMuons(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  //
  if ( m_inputAlgoSystNames.empty() ) {

    // this will be the collection processed - no matter what!!
    //
    RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName, m_event, m_store, m_verbose) ,"");

    // create output container (if requested)
    //
    ConstDataVector<xAOD::MuonContainer>* selectedMuons(nullptr);
    if ( m_createSelectedContainer ) { selectedMuons = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS); }

    // find the selected muons, and return if event passes object selection
    //
    eventPass = executeSelection( inMuons, mcEvtWeight, countPass, selectedMuons );

    if ( m_createSelectedContainer ) {
      if ( eventPass ) {
        // add ConstDataVector to TStore
	//
        RETURN_CHECK( "MuonSelector::execute()", m_store->record( selectedMuons, m_outContainerName ), "Failed to store const data container");
      } else {
        // if the event does not pass the selection, CDV won't be ever recorded to TStore, so we have to delete it!
	//
        delete selectedMuons; selectedMuons = nullptr;
      }
    }

  } else { // get the list of systematics to run over

    // get vector of string giving the syst names of the upstream algo from TStore (rememeber: 1st element is a blank string: nominal case!)
    //
    std::vector< std::string >* systNames(nullptr);
    RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

    // prepare a vector of the names of CDV containers for usage by downstream algos
    // must be a pointer to be recorded in TStore
    //
    std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;
    if ( m_debug ) { Info("execute()", " input list of syst size: %i ", static_cast<int>(systNames->size()) ); }

    // loop over systematic sets
    //
    bool eventPassThisSyst(false);
    for ( auto systName : *systNames ) {

      if ( m_debug ) { Info("execute()", " syst name: %s  input container name: %s ", systName.c_str(), (m_inContainerName+systName).c_str() ); }

      RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName + systName, m_event, m_store, m_verbose) ,"");

      // create output container (if requested) - one for each systematic
      //
      ConstDataVector<xAOD::MuonContainer>* selectedMuons(nullptr);
      if ( m_createSelectedContainer ) { selectedMuons = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS); }

      // find the selected muons, and return if event passes object selection
      //
      eventPassThisSyst = executeSelection( inMuons, mcEvtWeight, countPass, selectedMuons );

      if ( countPass ) { countPass = false; } // only count objects/events for 1st syst collection in iteration (i.e., nominal)

      if ( eventPassThisSyst ) {
	// save the string of syst set under question if event is passing the selection
	//
	vecOutContainerNames->push_back( systName );
      }

      // if for at least one syst set the event passes selection, this will remain true!
      //
      eventPass = ( eventPass || eventPassThisSyst );

      if ( m_debug ) { Info("execute()", " syst name: %s  output container name: %s ", systName.c_str(), (m_outContainerName+systName).c_str() ); }

      if ( m_createSelectedContainer ) {
        if ( eventPassThisSyst ) {
          // add ConstDataVector to TStore
	  //
          RETURN_CHECK( "MuonSelector::execute()", m_store->record( selectedMuons, m_outContainerName+systName ), "Failed to store const data container");
        } else {
          // if the event does not pass the selection for this syst, CDV won't be ever recorded to TStore, so we have to delete it!
          delete selectedMuons; selectedMuons = nullptr;
        }
      }

    } // close loop over syst sets

    if ( m_debug ) {  Info("execute()", " output list of syst size: %i ", static_cast<int>(vecOutContainerNames->size()) ); }

    // record in TStore the list of systematics names that should be considered down stream
    //
    RETURN_CHECK( "MuonSelector::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  }

  // look what we have in TStore
  //
  if ( m_verbose ) { m_store->print(); }

  if( !eventPass ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  if ( m_debug ) { Info("execute()", "Left Muon Selection..." ); }
  return EL::StatusCode::SUCCESS;

}

bool MuonSelector :: executeSelection ( const xAOD::MuonContainer* inMuons, float mcEvtWeight, bool countPass,
					    ConstDataVector<xAOD::MuonContainer>* selectedMuons )
{

  if ( m_debug ) { Info("execute()", "In  executeSelection..." ); }
  const xAOD::VertexContainer* vertices(nullptr);
  RETURN_CHECK("MuonSelector::executeSelection()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);

  int nPass(0); int nObj(0);
  static SG::AuxElement::Decorator< char > passSelDecor( "passSel" );

  for ( auto mu_itr : *inMuons ) { // duplicated of basic loop

    // if only looking at a subset of muons make sure all are decorated
    //
    if ( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if ( m_decorateSelectedObjects ) {
        passSelDecor( *mu_itr ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    bool passSel = this->passCuts( mu_itr, pvx );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *mu_itr ) = passSel;
    }

    // Remove events with isBadMuon (poor q/p)
    if( m_removeEventBadMuon && passSel ){
      if( m_muonSelectionTool_handle->isBadMuon( *mu_itr ) ){
        if( m_debug )  Info("executeSelection()", "Rejecting event with bad muon (pt = %f)", mu_itr->pt() ); 
        return false;
      }
    }

    if ( passSel ) {
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedMuons->push_back( mu_itr );
      }
    }
  }

  // for cutflow: make sure to count passed objects only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  if ( m_debug ) { Info("executeSelection()", "Initial muons: %i - Selected muons: %i", nObj , nPass ); }

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  //
  if ( m_pass_min > 0 && nPass < m_pass_min ) {
    if ( m_debug ) { Info("executeSelection()", "Reject event: nSelectedMuons (%i) < nPassMin (%i)", nPass, m_pass_min  ); }
    return false;
  }
  if ( m_pass_max > 0 && nPass > m_pass_max ) {
    if ( m_debug ) { Info("executeSelection()", "Reject event: nSelectedMuons (%i) > nPassMax (%i)", nPass, m_pass_max  ); }
    return false;
  }

  // for cutflow: make sure to count passed events only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ){
    m_numEventPass++;
    m_weightNumEventPass += mcEvtWeight;
  }

  // Perform trigger matching on the "good" (selected) muons
  //
  // NB: this part will be skipped if:
  //
  //  1. the user didn't pass any trigger chains to the algo (see initialize(): in that case, the tool is not even initialised!)
  //  2. there are no selected muons in the event
  //
  if ( m_doTrigMatch && selectedMuons ) {

    unsigned int nSelectedMuons = selectedMuons->size();

    static SG::AuxElement::Decorator< std::map<std::string,char> > isTrigMatchedMapMuDecor( "isTrigMatchedMapMu" );

    if ( nSelectedMuons > 0 ) {

      if ( m_debug ) { Info("executeSelection()", "Doing single muon trigger matching..."); }

      for ( auto const &chain : m_singleMuTrigChainsList ) {

        if ( m_debug ) { Info("executeSelection()", "\t checking trigger chain %s", chain.c_str()); }

        for ( auto const muon : *selectedMuons ) {

          //  For each muon, decorate w/ a map<string,char> with the 'isMatched' info associated
          //  to each trigger chain in the input list.
          //  If decoration map doesn't exist for this muon yet, create it (will be done only for the 1st iteration on the chain names)
          //
          if ( !isTrigMatchedMapMuDecor.isAvailable( *muon ) ) {
            isTrigMatchedMapMuDecor( *muon ) = std::map<std::string,char>();
          }

          char matched = ( m_trigMuonMatchTool_handle->match( *muon, chain, m_minDeltaR ) );

          if ( m_debug ) { Info("executeSelection()", "\t\t is muon trigger matched? %i", matched); }

          ( isTrigMatchedMapMuDecor( *muon ) )[chain] = matched;
        }
      }

    }

    // If checking dilepton trigger, form lepton pairs and test matching for each one.
    // Save a:
    //
    // multimap< chain, pair< pair<idx_i, idx_j>, ismatched > >
    //
    // as *event* decoration to store which
    // pairs are matched (to a given chain) and which aren't.
    // A multimap is used b/c a given key (i.e., a chain) can be associated to more than one pair. This is the case for e.g., trilepton events.
    //
    // By retrieving this map later on, user can decide what to do with the event
    // (Generally one could just loop over the map and save a flag if there's at least one pair that matches a given chain)

    if ( nSelectedMuons > 1 && !m_diMuTrigChains.empty() ) {

      if ( m_debug ) { Info("executeSelection()", "Doing di-muon trigger matching..."); }

      const xAOD::EventInfo* eventInfo(nullptr);
      RETURN_CHECK("MuonSelector::executeSelection()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

      typedef std::pair< std::pair<unsigned int,unsigned int>, char> dimuon_trigmatch_pair;
      typedef std::multimap< std::string, dimuon_trigmatch_pair >    dimuon_trigmatch_pair_map;      
      static SG::AuxElement::Decorator< dimuon_trigmatch_pair_map >  diMuonTrigMatchPairMapDecor( "diMuonTrigMatchPairMap" );

      for ( auto const &chain : m_diMuTrigChainsList ) {

      	if ( m_debug ) { Info("executeSelection()", "\t checking trigger chain %s", chain.c_str()); }
      
      	//  If decoration map doesn't exist for this event yet, create it (will be done only for the 1st iteration on the chain names)
      	//
      	if ( !diMuonTrigMatchPairMapDecor.isAvailable( *eventInfo ) ) {
          diMuonTrigMatchPairMapDecor( *eventInfo ) = dimuon_trigmatch_pair_map();
      	}	
      
      	std::vector<const xAOD::IParticle*> myMuons;
      
      	for ( unsigned int imu = 0; imu < selectedMuons->size()-1; ++imu ) {
      
      	  for ( unsigned int jmu = imu+1; jmu < selectedMuons->size(); ++jmu ) {
      
            // test a new pair
            //
      	    myMuons.clear();
      	    myMuons.push_back( selectedMuons->at(imu) );
      	    myMuons.push_back( selectedMuons->at(jmu) );
      
            // check whether the pair is matched
            //
      	    char matched = m_trigMuonMatchTool_handle->match( myMuons, chain, m_minDeltaR );
      
      	    if ( m_debug ) { Info("executeSelection()", "\t\t is the muon pair (%i,%i) trigger matched? %i", imu, jmu, matched); }
      
      	    std::pair <unsigned int, unsigned int>  chain_idxs = std::make_pair(imu,jmu);
            dimuon_trigmatch_pair  chain_decision = std::make_pair(chain_idxs,matched);
            diMuonTrigMatchPairMapDecor( *eventInfo ).insert( std::pair< std::string, dimuon_trigmatch_pair >(chain,chain_decision) );
      	    
      	  }
      	}
      } //for m_diMuTrigChainsList
    } //if nSelectedMuons > 1 && !m_diMuTrigChains.empty()
  } //if m_doTrigMatch && selectedMuons

  if ( m_debug ) { Info("execute()", "Left  executeSelection..." ); }
  return true;
}


EL::StatusCode MuonSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: finalize ()
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

  if ( m_useCutFlow ) {
    Info("histFinalize()", "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: histFinalize ()
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

int MuonSelector :: passCuts( const xAOD::Muon* muon, const xAOD::Vertex *primaryVertex  ) {

  if ( m_debug ) { Info("execute()", "In  passCuts..." ); }
  // fill cutflow bin 'all' before any cut
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_all, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_all, 1 ); }
  if ( m_debug ) { Info("execute()", "In  passCuts2..." ); }
  // *********************************************************************************************************************************************************************
  //
  // MuonSelectorTool cut: quality & |eta| acceptance cut
  //

  // quality decorators
  static SG::AuxElement::Decorator< char > isVeryLooseQDecor("isVeryLooseQ");
  static SG::AuxElement::Decorator< char > isLooseQDecor("isLooseQ");
  static SG::AuxElement::Decorator< char > isMediumQDecor("isMediumQ");
  static SG::AuxElement::Decorator< char > isTightQDecor("isTightQ");

  if ( m_debug ) { Info("execute()", "Got the decors" ); }
  int this_quality = static_cast<int>( m_muonSelectionTool_handle->getQuality( *muon ) );
  if ( m_debug ) { Info("execute()", "Got quality" ); }

  isVeryLooseQDecor( *muon ) = ( this_quality == static_cast<int>(xAOD::Muon::VeryLoose) ) ? 1 : 0;
  isLooseQDecor( *muon )     = ( this_quality == static_cast<int>(xAOD::Muon::Loose) )     ? 1 : 0;
  isMediumQDecor( *muon )    = ( this_quality == static_cast<int>(xAOD::Muon::Medium) )    ? 1 : 0;
  isTightQDecor( *muon )     = ( this_quality == static_cast<int>(xAOD::Muon::Tight) )     ? 1 : 0;
  if ( m_debug ) { Info("execute()", "Doing muon quality" ); }
  // this will accept the muon based on the settings at initialization : eta, ID track info, muon quality
  if ( ! m_muonSelectionTool_handle->accept( *muon ) ) {
    if ( m_debug ) { Info("PassCuts()", "Muon failed requirements of MuonSelectionTool."); }
    return 0;
  }

  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_eta_and_quaility_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_eta_and_quaility_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  //
  // pT max cut
  //
  if ( m_debug ) { Info("execute()", "Doing pt cuts" ); }
  if ( m_pT_max != 1e8 ) {
    if (  muon->pt() > m_pT_max ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed pT max cut."); }
      return 0;
    }
  }
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_ptmax_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_ptmax_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  //
  // pT min cut
  //
  if ( m_pT_min != 1e8 ) {
    if ( muon->pt() < m_pT_min ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed pT min cut."); }
      return 0;
    }
  }
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_ptmin_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_ptmin_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  //
  // muon type cut
  //
  HelperClasses::EnumParser<xAOD::Muon::MuonType> muTypeParser;
  if ( !m_muonType.empty() ) {
    if ( muon->muonType() != static_cast<int>(muTypeParser.parseEnum(m_muonType))) {
      if ( m_debug ) { Info("PassCuts()", "Muon type: %d - required: %s . Failed", muon->muonType(), m_muonType.c_str()); }
      return 0;
    }
  }
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_type_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_type_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  //
  // impact parameter cuts
  //

  // Put tracking cuts here, after pt cuts, to be safe with derivations.

  // The following returns a pointer (which should not usually be NULL, but might be if the muon has been stripped of information) to the
  // primary TrackParticle corresponding to the MuonType of this muon.
  // This is determined in the following order:
  //  1. CombinedTrackParticle
  //  2. InnerDetectorTrackParticle
  //  3. (Extrapolated)MuonSpectrometerTrackParticle
  //
  const xAOD::TrackParticle* tp = muon->primaryTrackParticle();

  if ( !tp ) {
    if ( m_debug ) Info( "PassCuts()", "Muon has no TrackParticle. Won't be selected.");
    return 0;
  }

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  double d0_significance = fabs( xAOD::TrackingHelpers::d0significance( tp, eventInfo->beamPosSigmaX(), eventInfo->beamPosSigmaY(), eventInfo->beamPosSigmaXY() ) );

  // Take distance between z0 and zPV ( after referring the PV z coordinate to the beamspot position, given by vz() ), multiplied by sin(theta)
  // see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackingDC14 for further reference
  //
  float z0sintheta = 1e8;
  if (primaryVertex) z0sintheta = ( tp->z0() + tp->vz() - primaryVertex->z() ) * sin( tp->theta() );

  // z0*sin(theta) cut
  //
  if ( !( fabs(z0sintheta) < m_z0sintheta_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed z0*sin(theta) cut."); }
      return 0;
  }
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_z0sintheta_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_z0sintheta_cut, 1 ); }

  // decorate muon w/ z0*sin(theta) info
  static SG::AuxElement::Decorator< float > z0sinthetaDecor("z0sintheta");
  z0sinthetaDecor( *muon ) = z0sintheta;

  // d0 cut
  //
  if ( !( tp->d0() < m_d0_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed d0 cut."); }
      return 0;
  }
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_d0_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_d0_cut, 1 ); }

  // d0sig cut
  //
  if ( !( d0_significance < m_d0sig_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed d0 significance cut."); }
      return 0;
  }
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_d0sig_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_d0sig_cut, 1 ); }

  // decorate muon w/ d0sig info
  static SG::AuxElement::Decorator< float > d0SigDecor("d0sig");
  d0SigDecor( *muon ) = static_cast<float>(d0_significance);

  // *********************************************************************************************************************************************************************
  //
  // isolation cut
  //

  // Get the "list" of input WPs with the accept() decision from the tool
  //
  Root::TAccept accept_list = m_isolationSelectionTool_handle->accept( *muon );

  // Decorate w/ decision for all input WPs
  //
  std::string base_decor("isIsolated");
  for ( auto WP_itr : m_IsoKeys ) {

    std::string decorWP = base_decor + "_" + WP_itr;

    if ( m_debug ) { Info("PassCuts()", "Decorate muon with %s - accept() ? %i", decorWP.c_str(), accept_list.getCutResult( WP_itr.c_str()) ); }
    muon->auxdecor<char>(decorWP) = static_cast<char>( accept_list.getCutResult( WP_itr.c_str() ) );

  }

  // Apply the cut if needed
  //
  if ( !m_MinIsoWPCut.empty() && !accept_list.getCutResult( m_MinIsoWPCut.c_str() ) ) {
    if ( m_debug ) { Info("PassCuts()", "Muon failed isolation cut %s ",  m_MinIsoWPCut.c_str() ); }
    return 0;
  }
  if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_iso_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_iso_cut, 1 ); }

  if( m_removeCosmicMuon ){

    double muon_d0 = tp->d0();
    double pv_z = primaryVertex ? primaryVertex->z() : 0;
    double muon_z0_exPV = tp->z0() + tp->vz() - pv_z;

    if( fabs(muon_z0_exPV) >= 1.0 || fabs(muon_d0) >= 0.2 ){
      if ( m_debug )   Info("PassCuts()", "Muon failed cosmic cut" );
      return 0;
    }
    if(m_useCutFlow) m_mu_cutflowHist_1->Fill( m_mu_cutflow_cosmic_cut, 1 );
    if ( m_isUsedBefore && m_useCutFlow ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_cosmic_cut, 1 ); }

  }


  if ( m_debug ) { Info("execute()", "Leave passCuts... pass" ); }
  return 1;
}

