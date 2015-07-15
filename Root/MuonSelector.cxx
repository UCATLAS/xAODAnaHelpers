/*******************************************
 *
 * Interface to CP Muon selection tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *
 ******************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>

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
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TrigMuonMatching/TrigMuonMatching.h"

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(MuonSelector)


MuonSelector :: MuonSelector () :
  m_cutflowHist(nullptr),
  m_cutflowHistW(nullptr),
  m_mu_cutflowHist_1(nullptr),
  m_mu_cutflowHist_2(nullptr),
  m_IsolationSelectionTool(nullptr),
  m_muonSelectionTool(nullptr),
  m_trigDecTool(nullptr),
  m_trigMuonMatchTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonSelector()", "Calling constructor");

  // read debug flag from .config file
  m_debug                   = false;
  m_useCutFlow              = true;

  // checks if the algorithm has been used already
  m_isUsedBefore            = false;

  // input container to be read from TEvent or TStore
  m_inContainerName         = "";

  // Systematics stuff
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "MuonSelector_Syst";


  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::View_Element option
  //                            decorrating and output container should not be mutually exclusive
  m_createSelectedContainer = false;
  // if requested, a new container is made using the SG::View_Element option
  m_outContainerName        = "";

  // if only want to look at a subset of object
  m_nToProcess              = -1;

  // configurable cuts
  m_muonType                = "";
  m_pass_max                = -1;
  m_pass_min                = -1;
  m_pT_max                  = 1e8;
  m_pT_min                  = 1e8;
  m_eta_max                 = 1e8;
  m_d0_max                  = 1e8;
  m_d0sig_max     	    = 1e8;
  m_z0sintheta_max          = 1e8;

  // isolation stuff
  m_doIsolation             = false;
  m_IsoWP		    = "Tight";
  m_CaloIsoEff              = "0.1*x+90";  // only if isolation WP is "UserDefined"
  m_TrackIsoEff             = "98";        // only if isolation WP is "UserDefined"
  m_CaloBasedIsoType        = "topoetcone20";
  m_TrackBasedIsoType       = "ptvarcone30";

  // trigger matching stuff
  m_singleMuTrigChains      = "";
  m_diMuTrigChains          = "";
  m_minDeltaR               = 0.1;

  m_passAuxDecorKeys        = "";
  m_failAuxDecorKeys        = "";

}

MuonSelector::~MuonSelector() {}

EL::StatusCode  MuonSelector :: configure ()
{

  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing MuonSelector Interface. User configuration read from : %s ", m_configName.c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug                   = config->GetValue("Debug" ,      m_debug);
    m_useCutFlow              = config->GetValue("UseCutFlow",  m_useCutFlow);

    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());

    // Systematics stuff
    m_inputAlgoSystNames      = config->GetValue("InputAlgoSystNames",  m_inputAlgoSystNames.c_str());
    m_outputAlgoSystNames     = config->GetValue("OutputAlgoSystNames", m_outputAlgoSystNames.c_str());


    // decorate selected objects that pass the cuts
    m_decorateSelectedObjects = config->GetValue("DecorateSelectedObjects", m_decorateSelectedObjects);
    // additional functionality : create output container of selected objects
    //                            using the SG::View_Element option
    //                            decorrating and output container should not be mutually exclusive
    m_createSelectedContainer = config->GetValue("CreateSelectedContainer", m_createSelectedContainer);
    // if requested, a new container is made using the SG::View_Element option
    m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());

    // if only want to look at a subset of object
    m_nToProcess              = config->GetValue("NToProcess", m_nToProcess);

    // configurable cuts
    std::string muonQuality   = config->GetValue("MuonQuality", "Medium");
    HelperClasses::EnumParser<xAOD::Muon::Quality> muQualityParser;
    m_muonQuality             = static_cast<int>( muQualityParser.parseEnum(muonQuality) );
    m_muonType                = config->GetValue("MuonType", m_muonType.c_str());
    m_pass_max                = config->GetValue("PassMax", m_pass_max);
    m_pass_min                = config->GetValue("PassMin", m_pass_min);
    m_pT_max                  = config->GetValue("pTMax",  m_pT_max);
    m_pT_min                  = config->GetValue("pTMin",  m_pT_min);
    m_eta_max                 = config->GetValue("etaMax", m_eta_max);
    m_d0_max                  = config->GetValue("d0Max", m_d0_max);
    m_d0sig_max     	      = config->GetValue("d0sigMax", m_d0sig_max);
    m_z0sintheta_max          = config->GetValue("z0sinthetaMax", m_z0sintheta_max);

    // isolation 
    m_doIsolation             = config->GetValue("DoIsolationCut"    ,  m_doIsolation);
    m_IsoWP		      = config->GetValue("IsolationWP"       ,  m_IsoWP.c_str());
    m_CaloIsoEff              = config->GetValue("CaloIsoEfficiecny" ,  m_CaloIsoEff.c_str());  
    m_TrackIsoEff             = config->GetValue("TrackIsoEfficiency",  m_TrackIsoEff.c_str());
    m_CaloBasedIsoType        = config->GetValue("CaloBasedIsoType"  ,  m_CaloBasedIsoType.c_str());
    m_TrackBasedIsoType       = config->GetValue("TrackBasedIsoType" ,  m_TrackBasedIsoType.c_str());

    // trigger matching 
    m_singleMuTrigChains      = config->GetValue("SingleMuTrigChain" , m_singleMuTrigChains.c_str() );
    m_diMuTrigChains	      = config->GetValue("DiMuTrigChain"     , m_diMuTrigChains.c_str() );
    m_minDeltaR 	      = config->GetValue("MinDeltaR"         , m_minDeltaR );

    m_passAuxDecorKeys        = config->GetValue("PassDecorKeys", m_passAuxDecorKeys.c_str());
    m_failAuxDecorKeys        = config->GetValue("FailDecorKeys", m_failAuxDecorKeys.c_str());

    config->Print();

    Info("configure()", "MuonSelector Interface succesfully configured! ");

    delete config; config = nullptr;
  }

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

  std::set<int> muonQualitySet;
  muonQualitySet.insert(0);
  muonQualitySet.insert(1);
  muonQualitySet.insert(2);
  muonQualitySet.insert(3);
  if ( muonQualitySet.find(m_muonQuality) == muonQualitySet.end() ) {
    Error("configure()", "Unknown muon quality requested: %i!",m_muonQuality);
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
    Error("configure()", "Unknown muon type requested: %s!",m_muonType.c_str());
    return EL::StatusCode::FAILURE;
  }

  // parse and split by comma
  std::string token;

  std::istringstream ss(m_passAuxDecorKeys);
  while ( std::getline(ss, token, ',') ) {
    m_passKeys.push_back(token);
  }

  ss.clear();
  ss.str(m_failAuxDecorKeys);
  while ( std::getline(ss, token, ',') ) {
    m_failKeys.push_back(token);
  }

  if( m_inContainerName.empty() ){
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


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

  // let's see if the algorithm has been already used before:
  // if yes, will write object cutflow in a different histogram!
  //
  // This is the case when the selector algorithm is used for 
  // preselecting objects, and then again for the final selection
  //
  Info("initialize()", "Algorithm name: '%s' - of type '%s' ", (this->m_name).c_str(), (this->m_classname).c_str() );
  if ( this->countUsed() > 0 ) {
    m_isUsedBefore = true;
    Info("initialize()", "\t An algorithm of the same type has been already used %i times", this->countUsed() );
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
    }
    
  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;
  
  // ****************************
  //
  // Initialise MuonSelectionTool
  //
  // ****************************
  
  if ( asg::ToolStore::contains<CP::MuonSelectionTool>("MuonSelectionTool") ) {
    m_muonSelectionTool = asg::ToolStore::get<CP::MuonSelectionTool>("MuonSelectionTool");
  } else {
    m_muonSelectionTool = new CP::MuonSelectionTool( "MuonSelectionTool" );
  }
  m_muonSelectionTool->msg().setLevel( MSG::ERROR); // VERBOSE

  // set eta and quality requirements in order to accept the muon - ID tracks required by default
  //
  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool->setProperty("MaxEta",    static_cast<double>(m_eta_max)), "Failed to set MaxEta property");
  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool->setProperty("MuQuality", m_muonQuality), "Failed to set MuQuality property" );
  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool->initialize(), "Failed to properly initialize the Muon Selection Tool");
 
  // *************************
  //
  // Initialise isolation tool
  //
  // *************************

  if ( asg::ToolStore::contains<CP::IsolationSelectionTool>("IsolationSelectionTool_Muons") ) {
    m_IsolationSelectionTool = asg::ToolStore::get<CP::IsolationSelectionTool>("IsolationSelectionTool_Muons");
  } else {
    m_IsolationSelectionTool = new CP::IsolationSelectionTool( "IsolationSelectionTool_Muons" );
  }
  m_IsolationSelectionTool->msg().setLevel( MSG::ERROR); // ERROR, VERBOSE, DEBUG, INFO
  
  if ( m_IsoWP == "UserDefined" ) {
    
    HelperClasses::EnumParser<xAOD::Iso::IsolationType> isoParser;
    
    std::vector< std::pair<xAOD::Iso::IsolationType, std::string> > myCuts;
    myCuts.push_back(std::make_pair<xAOD::Iso::IsolationType, std::string>(isoParser.parseEnum(m_TrackBasedIsoType), m_TrackIsoEff.c_str() ));
    myCuts.push_back(std::make_pair<xAOD::Iso::IsolationType, std::string>(isoParser.parseEnum(m_CaloBasedIsoType) , m_CaloIsoEff.c_str()  ));
   
    RETURN_CHECK( "MuonSelector::initialize()", m_IsolationSelectionTool->addUserDefinedWP("myTestWP", xAOD::Type::Muon, myCuts), "Failed to configure user-defined WP" );

  } else {
    RETURN_CHECK( "MuonSelector::initialize()", m_IsolationSelectionTool->setProperty( "MuonWP", m_IsoWP.c_str() ), "Failed to configure WorkingPoint" );
  }

  RETURN_CHECK( "MuonSelector::initialize()", m_IsolationSelectionTool->initialize(), "Failed to properly initialize IsolationSelectionTool." );

  // ********************************
  //
  // Initialise trigger matching tool
  //
  // ********************************
  
  // NB: need to retrieve the TrigDecisionTool from asg::ToolStore to configure the tool!
  //     do not initialise if there are no input trigger chains
  //
  if( !( m_singleMuTrigChains.empty() && m_diMuTrigChains.empty() ) && asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) ) {

    m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
    ToolHandle<Trig::TrigDecisionTool> trigDecHandle( m_trigDecTool );

    //  everything went fine, let's initialise the tool!
    //
    if ( asg::ToolStore::contains<Trig::TrigMuonMatching>("TrigMuonMatcinghTool") ) {
      m_trigMuonMatchTool = asg::ToolStore::get<Trig::TrigMuonMatching>("TrigMuonMatchingTool");
    } else {
      m_trigMuonMatchTool = new Trig::TrigMuonMatching("TrigMuonMatchingTool");
    }
    
    RETURN_CHECK( "MuonSelector::initialize()", m_trigMuonMatchTool->setProperty( "TriggerTool", trigDecHandle ), "Failed to configure TrigDecisionTool" );
    RETURN_CHECK( "MuonSelector::initialize()", m_trigMuonMatchTool->initialize(), "Failed to properly initialize TrigMuonMatching." );

  } else {
    Warning("initialize()", "\n***********************************************************\n Will not perform any electron trigger matching at this stage b/c : \n ");
    Warning("initialize()", "\t -) could not find the TrigDecisionTool in asg::ToolStore" );
    Warning("initialize()", "\t AND/OR" );
    Warning("initialize()", "\t -) input HLT trigger chain list is empty \n" );
    Warning("initialize()", "\n*********************************************************** \n If you want to apply the matching now, please double check that!");
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

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  // MC event weight
  float mcEvtWeight(1.0);
  static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
  if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
    Error("execute()  ", "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }
  mcEvtWeight = mcEvtWeightAcc( *eventInfo );

  m_numEvent++;

  // did any collection pass the cuts?
  bool eventPass(false);
  bool countPass(true); // for cutflow: count for the 1st collection in the syst container - could be better as should only count for the nominal
  const xAOD::MuonContainer* inMuons(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  if ( m_inputAlgoSystNames.empty() ) {

    // this will be the collection processed - no matter what!!
    RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName, m_event, m_store, m_verbose) ,"");

    // create output container (if requested)
    ConstDataVector<xAOD::MuonContainer>* selectedMuons(nullptr);
    if ( m_createSelectedContainer ) { selectedMuons = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS); }

    // find the selected muons, and return if event passes object selection
    eventPass = executeSelection( inMuons, mcEvtWeight, countPass, selectedMuons );

    if ( m_createSelectedContainer ) {
      if ( eventPass ) {
        // add ConstDataVector to TStore
        RETURN_CHECK( "MuonSelector::execute()", m_store->record( selectedMuons, m_outContainerName ), "Failed to store const data container");
      } else {
        // if the event does not pass the selection, CDV won't be ever recorded to TStore, so we have to delete it!
        delete selectedMuons; selectedMuons = nullptr;
      }
    }

  } else { // get the list of systematics to run over

    // get vector of string giving the syst names of the upstream algo from TStore (rememeber: 1st element is a blank string: nominal case!)
    std::vector< std::string >* systNames(nullptr);
    RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

    // prepare a vector of the names of CDV containers for usage by downstream algos
    // must be a pointer to be recorded in TStore
    std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;
    if ( m_debug ) { Info("execute()", " input list of syst size: %i ", static_cast<int>(systNames->size()) ); }

    // loop over systematic sets
    bool eventPassThisSyst(false);
    for ( auto systName : *systNames ) {

      if ( m_debug ) { Info("execute()", " syst name: %s  input container name: %s ", systName.c_str(), (m_inContainerName+systName).c_str() ); }

      RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName + systName, m_event, m_store, m_verbose) ,"");

      // create output container (if requested) - one for each systematic
      ConstDataVector<xAOD::MuonContainer>* selectedMuons(nullptr);
      if ( m_createSelectedContainer ) { selectedMuons = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS); }

      // find the selected muons, and return if event passes object selection
      eventPassThisSyst = executeSelection( inMuons, mcEvtWeight, countPass, selectedMuons );

      if ( countPass ) { countPass = false; } // only count objects/events for 1st syst collection in iteration (i.e., nominal)

      if ( eventPassThisSyst ) {
	// save the string of syst set under question if event is passing the selection
	vecOutContainerNames->push_back( systName );
      }

      // if for at least one syst set the event passes selection, this will remain true!
      eventPass = ( eventPass || eventPassThisSyst );

      if ( m_debug ) { Info("execute()", " syst name: %s  output container name: %s ", systName.c_str(), (m_outContainerName+systName).c_str() ); }

      if ( m_createSelectedContainer ) {
        if ( eventPassThisSyst ) {
          // add ConstDataVector to TStore
          RETURN_CHECK( "MuonSelector::execute()", m_store->record( selectedMuons, m_outContainerName+systName ), "Failed to store const data container");
        } else {
          // if the event does not pass the selection for this syst, CDV won't be ever recorded to TStore, so we have to delete it!
          delete selectedMuons; selectedMuons = nullptr;
        }
      }

    } // close loop over syst sets

    if ( m_debug ) {  Info("execute()", " output list of syst size: %i ", static_cast<int>(vecOutContainerNames->size()) ); }

    // record in TStore the list of systematics names that should be considered down stream
    RETURN_CHECK( "MuonSelector::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  }

  // look what do we have in TStore
  if ( m_verbose ) { m_store->print(); }

  if( !eventPass ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  return EL::StatusCode::SUCCESS;

}

bool MuonSelector :: executeSelection ( const xAOD::MuonContainer* inMuons, float mcEvtWeight, bool countPass,
					    ConstDataVector<xAOD::MuonContainer>* selectedMuons )
{

  const xAOD::VertexContainer* vertices(nullptr);
  RETURN_CHECK("MuonSelector::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);

  int nPass(0); int nObj(0);
  static SG::AuxElement::Decorator< char > passSelDecor( "passSel" );

  for ( auto mu_itr : *inMuons ) { // duplicated of basic loop

    // if only looking at a subset of muons make sure all are decorated
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

    if ( passSel ) {
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedMuons->push_back( mu_itr );
      }
    }
  }

  // for cutflow: make sure to count passed objects only once (i.e., this flag will be true only for nominal)
  if ( countPass ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  if ( m_debug ) { Info("execute()", "Initial muons: %i - Selected muons: %i", nObj , nPass ); }

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  if ( m_pass_min > 0 && nPass < m_pass_min ) {
    if ( m_debug ) { Info("execute()", "Reject event: nSelectedMuons (%i) < nPassMin (%i)", nPass, m_pass_min  ); }
    return false;
  }
  if ( m_pass_max > 0 && nPass > m_pass_max ) {
    if ( m_debug ) { Info("execute()", "Reject event: nSelectedMuons (%i) > nPassMax (%i)", nPass, m_pass_max  ); }
    return false;
  }

  // for cutflow: make sure to count passed events only once (i.e., this flag will be true only for nominal)
  if ( countPass ){
    m_numEventPass++;
    m_weightNumEventPass += mcEvtWeight;
  }

  // perform trigger matching on the "good" (selected) muons
  //
  // NB: this part will be skipped if:
  //
  //  1. the user didn't pass any trigger chains to the algo (see initialize(): in that case, the tool is not even initialised!)
  //  2. there are no selected muons in the event
  //
  if ( m_trigMuonMatchTool && selectedMuons ) {
  
    unsigned int nSelectedMuons = selectedMuons->size();

    static SG::AuxElement::Decorator< char > isTrigMatchedDecor("isTrigMatched");

    if ( nSelectedMuons > 0 && !m_singleMuTrigChains.empty() ) {
      if ( m_debug ) { Info("execute()", "Single Muon Trigger Matching "); }

       for ( const auto muon : *selectedMuons ) {
    	 isTrigMatchedDecor( *muon ) = ( m_trigMuonMatchTool->match( muon, m_singleMuTrigChains, m_minDeltaR ) ) ? 1 : 0;
       }

    }
    if ( nSelectedMuons > 1 && !m_diMuTrigChains.empty() ) {
      if ( m_debug ) { Info("execute()", "DiMuon Trigger Matching "); }

      // take the first two muons in the selected container
      //
      const xAOD::Muon* mu1 = selectedMuons->at(0);
      const xAOD::Muon* mu2 = selectedMuons->at(1);

      std::pair<Bool_t,Bool_t> result1, result2;
      m_trigMuonMatchTool->matchDimuon( mu1, mu2, m_diMuTrigChains, result1, result2, m_minDeltaR );

      if ( result1.first && result2.first ) {
    	isTrigMatchedDecor( *mu1 ) = ( result1.first ) ? 1 : 0;
    	isTrigMatchedDecor( *mu2 ) = ( result2.first ) ? 1 : 0;
      }

    }
  }

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

  if ( m_muonSelectionTool )      { m_muonSelectionTool = nullptr; delete m_muonSelectionTool;  }
  if ( m_IsolationSelectionTool ) { m_IsolationSelectionTool = nullptr; delete m_IsolationSelectionTool; }
  if ( m_trigMuonMatchTool )      {  m_trigMuonMatchTool = nullptr; delete m_trigMuonMatchTool; }

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

  return EL::StatusCode::SUCCESS;
}

int MuonSelector :: passCuts( const xAOD::Muon* muon, const xAOD::Vertex *primaryVertex  ) {
 
  // fill cutflow bin 'all' before any cut
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_all, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_all, 1 ); }

  // *********************************************************************************************************************************************************************
  // 
  // MuonSelectorTool cut: quality & |eta| acceptance cut
  //
  
  // quality decorators
  static SG::AuxElement::Decorator< char > isVeryLooseQDecor("isVeryLooseQ");
  static SG::AuxElement::Decorator< char > isLooseQDecor("isLooseQ");
  static SG::AuxElement::Decorator< char > isMediumQDecor("isMediumQ");
  static SG::AuxElement::Decorator< char > isTightQDecor("isTightQ");
  // initialise
  isVeryLooseQDecor( *muon ) = -1;
  isLooseQDecor( *muon )     = -1;
  isMediumQDecor( *muon )    = -1;
  isTightQDecor( *muon )     = -1;

  int this_quality = static_cast<int>( m_muonSelectionTool->getQuality( *muon ) );

  // this will accept the muon based on the settings at initialization : eta, ID track info, muon quality
  if ( ! m_muonSelectionTool->accept( *muon ) ) {
    if ( m_debug ) { Info("PassCuts()", "Muon failed requirements of MuonSelectionTool."); }
    return 0;
  }

  isVeryLooseQDecor( *muon ) = ( this_quality == static_cast<int>(xAOD::Muon::VeryLoose) ) ? 1 : 0;
  isLooseQDecor( *muon )     = ( this_quality == static_cast<int>(xAOD::Muon::Loose) )     ? 1 : 0;
  isMediumQDecor( *muon )    = ( this_quality == static_cast<int>(xAOD::Muon::Medium) )    ? 1 : 0;
  isTightQDecor( *muon )     = ( this_quality == static_cast<int>(xAOD::Muon::Tight) )     ? 1 : 0;

  m_mu_cutflowHist_1->Fill( m_mu_cutflow_eta_and_quaility_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_eta_and_quaility_cut, 1 ); }
	   
  // *********************************************************************************************************************************************************************
  // 
  // pT max cut
  //
  if ( m_pT_max != 1e8 ) {
    if (  muon->pt() > m_pT_max ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed pT max cut."); }
      return 0;
    }
  }
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_ptmax_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_ptmax_cut, 1 ); }
  
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
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_ptmin_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_ptmin_cut, 1 ); }

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
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_type_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_type_cut, 1 ); }

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
  RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  
  double d0_significance = fabs( xAOD::TrackingHelpers::d0significance( tp, eventInfo->beamPosSigmaX(), eventInfo->beamPosSigmaY(), eventInfo->beamPosSigmaXY() ) );
  float z0sintheta	 = ( tp->z0() + tp->vz() - primaryVertex->z() ) * sin( tp->theta() );

  // z0*sin(theta) cut
  //
  if ( !( fabs(z0sintheta) < m_z0sintheta_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed z0*sin(theta) cut."); }
      return 0;
  }
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_z0sintheta_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_z0sintheta_cut, 1 ); }
  
  // d0 cut
  //
  if ( !( tp->d0() < m_d0_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed d0 cut."); }
      return 0;
  }
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_d0_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_d0_cut, 1 ); }
  
  // d0sig cut
  //
  if ( !( d0_significance < m_d0sig_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Muon failed d0 significance cut."); }
      return 0;
  }
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_d0sig_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_d0sig_cut, 1 ); }

  // decorate muon w/ d0sig info
  static SG::AuxElement::Decorator< float > d0SigDecor("d0sig");
  d0SigDecor( *muon ) = static_cast<float>(d0_significance);
  
  // *********************************************************************************************************************************************************************
  // 
  // isolation cut
  //
  static SG::AuxElement::Decorator< char > isIsoDecor("isIsolated");
  bool passIso(false);

  passIso = ( m_IsolationSelectionTool->accept( *muon ) );
  isIsoDecor( *muon ) = ( passIso ) ? 1 : 0;

  if ( m_doIsolation && !passIso ) {
    if ( m_debug ) { Info("PassCuts()", "Muon failed isolation cut "); }
    return 0;
  }
  m_mu_cutflowHist_1->Fill( m_mu_cutflow_iso_cut, 1 );
  if ( m_isUsedBefore ) { m_mu_cutflowHist_2->Fill( m_mu_cutflow_iso_cut, 1 ); }


  return 1;
}

