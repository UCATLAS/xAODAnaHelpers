/********************************************
 *
 * Interface to CP Electron selection tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *******************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/EgammaDefs.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/TrackParticlexAODHelpers.h"

// package include(s):
#include "xAODAnaHelpers/ElectronSelector.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/AsgElectronIsEMSelector.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TrigEgammaMatchingTool/TrigEgammaMatchingTool.h"

#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronSelector)

ElectronSelector :: ElectronSelector () :
  m_cutflowHist(nullptr),
  m_cutflowHistW(nullptr),
  m_el_cutflowHist_1(nullptr),
  m_el_cutflowHist_2(nullptr),
  m_IsolationSelectionTool(nullptr),
  m_el_LH_PIDManager(nullptr),
  m_el_CutBased_PIDManager(nullptr),
  m_trigDecTool(nullptr),
  m_trigElMatchTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronSelector()", "Calling constructor");

  // read debug flag from .config file
  m_debug                   = false;
  m_useCutFlow              = true;

  // checks if the algorithm has been used already
  m_isUsedBefore            = false;

  // input container to be read from TEvent or TStore
  m_inContainerName         = "";

  // Systematics stuff
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "ElectronSelector_Syst";


  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::VIEW_ELEMENT option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainer = false;
  // if requested, a new container is made using the SG::VIEW_ELEMENT option
  m_outContainerName        = "";

  // if only want to look at a subset of object
  m_nToProcess              = -1;

  // configurable cuts
  m_pass_max                = -1;
  m_pass_min                = -1;
  m_pT_max                  = 1e8;
  m_pT_min                  = 1e8;
  m_eta_max                 = 1e8;
  m_vetoCrack               = true;
  m_d0_max                  = 1e8;
  m_d0sig_max     	    = 1e8;
  m_z0sintheta_max          = 1e8;
  m_doAuthorCut             = true;
  m_doOQCut                 = true;

  m_confDirPID              = "mc15_20150224";
  // likelihood-based PID
  m_doLHPIDcut              = false;
  m_LHOperatingPoint        = "Loose";
  m_LHConfigYear            = "2015";

  // cut-based PID
  m_doCutBasedPIDcut        = false;
  m_CutBasedOperatingPoint  = "IsEMLoose";
  m_CutBasedConfigYear      = "2012";

  // isolation
  m_doIsolation             = false;
  m_IsoWP		    = "Tight";
  m_CaloIsoEff              = "0.1*x+90";  // only if isolation WP is "UserDefined"
  m_TrackIsoEff             = "98";        // only if isolation WP is "UserDefined"
  m_CaloBasedIsoType        = "topoetcone20";
  m_TrackBasedIsoType       = "ptvarcone20";

  // trigger matching
  m_ElTrigChains            = "";

}

ElectronSelector::~ElectronSelector() {}

EL::StatusCode  ElectronSelector :: configure ()
{
  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing ElectronSelector Interface. User configuration read from : %s ", getConfig().c_str());

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
    m_pass_max                = config->GetValue("PassMax", m_pass_max);
    m_pass_min                = config->GetValue("PassMin", m_pass_min);
    m_pT_max                  = config->GetValue("pTMax",  m_pT_max);
    m_pT_min                  = config->GetValue("pTMin",  m_pT_min);
    m_eta_max                 = config->GetValue("etaMax", m_eta_max);
    m_vetoCrack               = config->GetValue("VetoCrack", m_vetoCrack);
    m_d0_max                  = config->GetValue("d0Max", m_d0_max);
    m_d0sig_max     	      = config->GetValue("d0sigMax", m_d0sig_max);
    m_z0sintheta_max          = config->GetValue("z0sinthetaMax", m_z0sintheta_max);
    m_doAuthorCut             = config->GetValue("DoAuthorCut", m_doAuthorCut);
    m_doOQCut                 = config->GetValue("DoOQCut", m_doOQCut);

    m_confDirPID              = config->GetValue("ConfDirPID", m_confDirPID.c_str());
    // likelihood-based PID
    m_doLHPIDcut              = config->GetValue("DoLHPIDCut", m_doLHPIDcut);
    m_LHOperatingPoint        = config->GetValue("LHOperatingPoint", m_LHOperatingPoint.c_str());
    m_LHConfigYear            = config->GetValue("LHConfigYear", m_LHConfigYear.c_str());

    // cut-based PID
    m_doCutBasedPIDcut        = config->GetValue("DoCutBasedPIDCut", m_doCutBasedPIDcut);
    m_CutBasedOperatingPoint  = config->GetValue("CutBasedOperatingPoint", m_CutBasedOperatingPoint.c_str());
    m_CutBasedConfigYear      = config->GetValue("CutBasedConfigYear", m_CutBasedConfigYear.c_str());

    // isolation
    m_doIsolation             = config->GetValue("DoIsolationCut"    ,  m_doIsolation);
    m_IsoWP		      = config->GetValue("IsolationWP"       ,  m_IsoWP.c_str());
    m_CaloIsoEff              = config->GetValue("CaloIsoEfficiecny" ,  m_CaloIsoEff.c_str());  // only if isolation WP is "UserDefined"
    m_TrackIsoEff             = config->GetValue("TrackIsoEfficiency",  m_TrackIsoEff.c_str()); // only if isolation WP is "UserDefined"
    m_CaloBasedIsoType        = config->GetValue("CaloBasedIsoType"  ,  m_CaloBasedIsoType.c_str());
    m_TrackBasedIsoType       = config->GetValue("TrackBasedIsoType" ,  m_TrackBasedIsoType.c_str());

    // trigger matching
    m_ElTrigChains            = config->GetValue("ElTrigChains"      , m_ElTrigChains.c_str() );

    config->Print();

    Info("configure()", "ElectronSelector Interface succesfully configured! ");

    delete config; config = nullptr;
  }

  if ( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  if ( m_LHOperatingPoint != "VeryLoose" &&
       m_LHOperatingPoint != "Loose"     &&
       m_LHOperatingPoint != "Medium"    &&
       m_LHOperatingPoint != "Tight"     ) {
    Error("configure()", "Unknown electron likelihood PID requested %s!",m_LHOperatingPoint.c_str());
    return EL::StatusCode::FAILURE;
  }
  if ( m_CutBasedOperatingPoint != "IsEMLoose"  &&
       m_CutBasedOperatingPoint != "IsEMMedium" &&
       m_CutBasedOperatingPoint != "IsEMTight"  ) {
    Error("configure()", "Unknown electron cut-based PID requested %s!",m_CutBasedOperatingPoint.c_str());
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronSelector :: setupJob (EL::Job& job)
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
  xAOD::Init( "ElectronSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "Calling histInitialize");


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing ElectronSelector Interface... ");

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
    m_el_cutflowHist_1 = (TH1D*)file->Get("cutflow_electrons_1");
    
    m_el_cutflow_all             = m_el_cutflowHist_1->GetXaxis()->FindBin("all");
    m_el_cutflow_author_cut      = m_el_cutflowHist_1->GetXaxis()->FindBin("author_cut");     
    m_el_cutflow_OQ_cut          = m_el_cutflowHist_1->GetXaxis()->FindBin("OQ_cut");         
    m_el_cutflow_ptmax_cut       = m_el_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");      
    m_el_cutflow_ptmin_cut       = m_el_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");
    m_el_cutflow_eta_cut         = m_el_cutflowHist_1->GetXaxis()->FindBin("eta_cut"); // including crack veto, if applied
    m_el_cutflow_PID_cut         = m_el_cutflowHist_1->GetXaxis()->FindBin("PID_cut");
    m_el_cutflow_z0sintheta_cut  = m_el_cutflowHist_1->GetXaxis()->FindBin("z0sintheta_cut");
    m_el_cutflow_d0_cut          = m_el_cutflowHist_1->GetXaxis()->FindBin("d0_cut");
    m_el_cutflow_d0sig_cut       = m_el_cutflowHist_1->GetXaxis()->FindBin("d0sig_cut");
    m_el_cutflow_iso_cut         = m_el_cutflowHist_1->GetXaxis()->FindBin("iso_cut");
    
    if ( m_isUsedBefore ) {
       m_el_cutflowHist_2 = (TH1D*)file->Get("cutflow_electrons_2");
    
       m_el_cutflow_all 	    = m_el_cutflowHist_2->GetXaxis()->FindBin("all");
       m_el_cutflow_author_cut      = m_el_cutflowHist_2->GetXaxis()->FindBin("author_cut");	 
       m_el_cutflow_OQ_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("OQ_cut");	 
       m_el_cutflow_ptmax_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("ptmax_cut");	 
       m_el_cutflow_ptmin_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("ptmin_cut");
       m_el_cutflow_eta_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("eta_cut"); // including crack veto, if applied
       m_el_cutflow_PID_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("PID_cut");
       m_el_cutflow_z0sintheta_cut  = m_el_cutflowHist_2->GetXaxis()->FindBin("z0sintheta_cut");
       m_el_cutflow_d0_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("d0_cut");
       m_el_cutflow_d0sig_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("d0sig_cut");
       m_el_cutflow_iso_cut	    = m_el_cutflowHist_2->GetXaxis()->FindBin("iso_cut");
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
  // Initialise Electron ID tools
  //
  // ****************************

  // tell the selector tools where to find configuration files
  //
  std::string confDir = "ElectronPhotonSelectorTools/offline/" +  m_confDirPID + "/";

  // initialise PID tool(s) using classes defined in ParticlePIDManager.h
  //
  // if not using cut-based PID, make sure all the decorations will be set ... by choosing the loosest WP!
  //
  std::string cutbasedWP = ( m_doCutBasedPIDcut ) ? m_CutBasedOperatingPoint : "IsEMLoose";
  m_el_CutBased_PIDManager = new ElectronCutBasedPIDManager( cutbasedWP, m_debug );

  if  ( m_doCutBasedPIDcut ) {
    if ( m_debug ) {
      Info("initialize()", "Cutting on Electron Cut-Based PID! \n ********************" );
      Info("initialize()", "Selected cut-based WP: %s", (m_el_CutBased_PIDManager->getSelectedWP()).c_str() );
    }
  } else {
    if ( m_debug ) { Info("initialize()", "Will decorate each electron with all Electron Cut-Based PID WPs decison (pass/not pass)!" ); }
  }

  RETURN_CHECK( "ElectronSelector::initialize()", m_el_CutBased_PIDManager->setupTools( this->m_name, confDir, m_CutBasedConfigYear ), "Failed to properly setup ElectronCutBasedPIDManager." );

  // if not using LH PID, make sure all the decorations will be set ... by choosing the loosest WP!
  //
  std::string likelihoodWP = ( m_doLHPIDcut ) ? m_LHOperatingPoint : "VeryLoose";
  m_el_LH_PIDManager = new ElectronLHPIDManager( likelihoodWP, m_debug );

  if  ( m_doLHPIDcut ) {
    if ( m_debug ) {
       Info("initialize()", "Cutting on Electron Likelihood PID! \n ********************" );
       Info("initialize()", "\t Selected LH WP: %s", (m_el_LH_PIDManager->getSelectedWP()).c_str() );
    }
  } else {
    if ( m_debug ) { Info("initialize()", "Will decorate each electron with all Electron Likelihood PID WPs decison (pass/not pass)!" ); }
  }

  if ( m_debug ) { Info("initialize()", "Selected LH WP: %s", (m_el_LH_PIDManager->getSelectedWP()).c_str() ); }
  RETURN_CHECK( "ElectronSelector::initialize()", m_el_LH_PIDManager->setupTools( this->m_name, confDir, m_LHConfigYear ), "Failed to properly setup ElectronLHPIDManager." );

  // *************************
  //
  // Initialise isolation tool
  //
  // *************************

  if ( asg::ToolStore::contains<CP::IsolationSelectionTool>("IsolationSelectionTool_Electrons") ) {
    m_IsolationSelectionTool = asg::ToolStore::get<CP::IsolationSelectionTool>("IsolationSelectionTool_Electrons");
  } else {
    m_IsolationSelectionTool = new CP::IsolationSelectionTool( "IsolationSelectionTool_Electrons" );
  }
  m_IsolationSelectionTool->msg().setLevel( MSG::ERROR); // ERROR, VERBOSE, DEBUG, INFO

  if ( m_IsoWP == "UserDefined" ) {

    HelperClasses::EnumParser<xAOD::Iso::IsolationType> isoParser;
  
    std::vector< std::pair<xAOD::Iso::IsolationType, std::string> > myCuts;
    myCuts.push_back(std::make_pair<xAOD::Iso::IsolationType, std::string>(isoParser.parseEnum(m_TrackBasedIsoType), m_TrackIsoEff.c_str() ));
    myCuts.push_back(std::make_pair<xAOD::Iso::IsolationType, std::string>(isoParser.parseEnum(m_CaloBasedIsoType) , m_CaloIsoEff.c_str()  ));
  
    RETURN_CHECK( "ElectronSelector::initialize()", m_IsolationSelectionTool->addUserDefinedWP("myTestWP", xAOD::Type::Electron, myCuts), "Failed to configure user-defined WP" );
  
  } else {
    RETURN_CHECK( "ElectronSelector::initialize()", m_IsolationSelectionTool->setProperty( "ElectronWP", m_IsoWP.c_str() ), "Failed to configure WorkingPoint" );
  }

  RETURN_CHECK( "ElectronSelector::initialize()", m_IsolationSelectionTool->initialize(), "Failed to properly initialize IsolationSelectionTool." );

  // ********************************
  //
  // Initialise trigger matching tool
  //
  // ********************************

  //
  // NB: need to retrieve the TrigDecisionTool from asg::ToolStore to configure the tool!
  //     do not initialise if there are no input trigger chains
  //
  if( !m_ElTrigChains.empty() && asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) ) {

    m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
    ToolHandle<Trig::TrigDecisionTool> trigDecHandle( m_trigDecTool );

    //  everything went fine, let's initialise the tool!
    //
    if ( asg::ToolStore::contains<Trig::TrigEgammaMatchingTool>("TrigEgammaMatchingTool") ) {
      m_trigElMatchTool = asg::ToolStore::get<Trig::TrigEgammaMatchingTool>("TrigEgammaMatchingTool");
    } else {
      m_trigElMatchTool = new Trig::TrigEgammaMatchingTool("TrigEgammaMatchingTool");
      RETURN_CHECK( "ElectronSelector::initialize()", m_trigElMatchTool->setProperty( "TriggerTool", trigDecHandle ), "Failed to configure TrigDecisionTool" );
      RETURN_CHECK( "ElectronSelector::initialize()", m_trigElMatchTool->initialize(), "Failed to properly initialize TrigMuonMatching." );
    } 
    

  } else {
    Warning("initialize()", "\n***********************************************************\n Will not perform any electron trigger matching at this stage b/c : \n ");
    Warning("initialize()", "\t -) could not find the TrigDecisionTool in asg::ToolStore" );
    Warning("initialize()", "\t AND/OR" );
    Warning("initialize()", "\t -) input HLT trigger chain list is empty \n" );
    Warning("initialize()", "\n*********************************************************** \n If you want to apply the matching now, please double check that!");
  }

  // **********************************************************************************************
  
  Info("initialize()", "ElectronSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Electron Selection... "); }

  // retrieve event
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

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
    
    // store the trigger chains that will be considered for matching
    //
    if ( m_ElTrigChains.find("ALL") != std::string::npos ) {
      std::vector<std::string> list = (m_trigDecTool->getChainGroup("HLT_e.*"))->getListOfTriggers();
      for ( auto &trig : list ) { m_ElTrigChainsList.push_back(trig); }
    } else {
      // parse input electron trigger chain list, split by comma and fill vector
      //
      std::string trig;
      std::istringstream ss(m_ElTrigChains);
      
      while ( std::getline(ss, trig, ',') ) {
    	m_ElTrigChainsList.push_back(trig);
      }
    }	 

    Info("initialize()", "Input electron trigger chains for matching:");
    for ( auto const &chain : m_ElTrigChainsList ) { Info("initialize()", "\t %s", chain.c_str()); }
  
  }

  // did any collection pass the cuts?
  //
  bool eventPass(false);
  bool countPass(true); // for cutflow: count for the 1st collection in the syst container - could be better as should only count for the nominal
  const xAOD::ElectronContainer* inElectrons(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  //
  if ( m_inputAlgoSystNames.empty() ) {

    // this will be the collection processed - no matter what!!
    //
    RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName, m_event, m_store, m_verbose) ,"");

    // create output container (if requested)
    ConstDataVector<xAOD::ElectronContainer>* selectedElectrons(nullptr);
    if ( m_createSelectedContainer ) { selectedElectrons = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS); }

    // find the selected electrons, and return if event passes object selection
    //
    eventPass = executeSelection( inElectrons, mcEvtWeight, countPass, selectedElectrons );

    if ( m_createSelectedContainer) {
      if ( eventPass ) {
        // add ConstDataVector to TStore
	//
        RETURN_CHECK( "ElectronSelector::execute()", m_store->record( selectedElectrons, m_outContainerName ), "Failed to store const data container");
      } else {
        // if the event does not pass the selection, CDV won't be ever recorded to TStore, so we have to delete it!
        delete selectedElectrons; selectedElectrons = nullptr;
      }
    }

  } else { // get the list of systematics to run over

    // get vector of string giving the syst names of the upstream algo from TStore (rememeber: 1st element is a blank string: nominal case!)
    //
    std::vector< std::string >* systNames(nullptr);
    RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

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

      RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName + systName, m_event, m_store, m_verbose) ,"");

      // create output container (if requested) - one for each systematic
      //
      ConstDataVector<xAOD::ElectronContainer>* selectedElectrons(nullptr);
      if ( m_createSelectedContainer ) { selectedElectrons = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS); }

      // find the selected electrons, and return if event passes object selection
      //
      eventPassThisSyst = executeSelection( inElectrons, mcEvtWeight, countPass, selectedElectrons );

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
          RETURN_CHECK( "ElectronSelector::execute()", m_store->record( selectedElectrons, m_outContainerName+systName ), "Failed to store const data container");
        } else {
          // if the event does not pass the selection for this syst, CDV won't be ever recorded to TStore, so we have to delete it!
	  //
          delete selectedElectrons; selectedElectrons = nullptr;
        }
      }

    } // close loop over syst sets

    if ( m_debug ) {  Info("execute()", " output list of syst size: %i ", static_cast<int>(vecOutContainerNames->size()) ); }

    // record in TStore the list of systematics names that should be considered down stream
    //
    RETURN_CHECK( "ElectronSelector::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  }

  // look what do we have in TStore
  //
  if ( m_verbose ) { m_store->print(); }

  if( !eventPass ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  return EL::StatusCode::SUCCESS;

}

bool ElectronSelector :: executeSelection ( const xAOD::ElectronContainer* inElectrons, float mcEvtWeight, bool countPass,
					    ConstDataVector<xAOD::ElectronContainer>* selectedElectrons )
{

  const xAOD::VertexContainer* vertices(nullptr);
  RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);

  int nPass(0); int nObj(0);
  static SG::AuxElement::Decorator< char > passSelDecor( "passSel" );

  for ( auto el_itr : *inElectrons ) { // duplicated of basic loop

    // if only looking at a subset of electrons make sure all are decorated
    //
    if ( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if ( m_decorateSelectedObjects ) {
        passSelDecor( *el_itr ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    bool passSel = this->passCuts( el_itr, pvx );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *el_itr ) = passSel;
    }

    if ( passSel ) {
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedElectrons->push_back( el_itr );
      }
    }
  }

  // for cutflow: make sure to count passed objects only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  if ( m_debug ) { Info("execute()", "Initial electrons:%i - Selected electrons: %i", nObj , nPass ); }

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  //
  if ( m_pass_min > 0 && nPass < m_pass_min ) {
    return false;
  }
  if ( m_pass_max > 0 && nPass > m_pass_max ) {
    return false;
  }

  // for cutflow: make sure to count passed events only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ){
    m_numEventPass++;
    m_weightNumEventPass += mcEvtWeight;
  }

  // Perform trigger matching on the "good" (selected) electrons
  //
  // NB: this part will be skipped if:
  //
  //  1. the user didn't pass any trigger chains to the algo (see initialize(): in that case, the tool is not even initialised!)
  //  2. there are no selected electrons in the event
  //
  if ( m_trigElMatchTool && selectedElectrons ) {
    
    unsigned int nSelectedElectrons = selectedElectrons->size();

    if ( nSelectedElectrons > 0 ) {
      
      if ( m_debug ) { Info("execute()", "Now doing electron trigger matching..."); }
     
      for ( auto const &chain : m_ElTrigChainsList ) {
       
         if ( m_debug ) { Info("execute()", "\t checking trigger chain %s", chain.c_str()); }
       
         for ( auto const electron : *selectedElectrons ) {
    	   
           //  For each electron, decorate w/ a map<string,char> with the 'isMatched' info associated
	   //  to each trigger chain in the input list.
           //  If decoration map doesn't exist, create it (will be done only for the 1st iteration)
           //
           SG::AuxElement::Decorator< std::map<std::string,char> > isTrigMatchedMapElDecor( "isTrigMatchedMapEl" );
           if ( !isTrigMatchedMapElDecor.isAvailable( *electron ) ) {
	     isTrigMatchedMapElDecor( *electron ) = std::map<std::string,char>();
           }
	     
	   int matched = ( m_trigElMatchTool->matchHLT( electron, chain ) ) ? 1 : 0;  
	   
           if ( m_debug ) { Info("execute()", "\t\t is electron trigger matched? %i", matched); }
	   	   
	   ( isTrigMatchedMapElDecor( *electron ) )[chain] = static_cast<char>(matched);  
         }
      }

    }
  }

  return true;
}

EL::StatusCode ElectronSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: finalize ()
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

  if ( m_el_CutBased_PIDManager )         { m_el_CutBased_PIDManager = nullptr; delete m_el_CutBased_PIDManager;  }
  if ( m_el_LH_PIDManager )               { m_el_LH_PIDManager = nullptr;       delete m_el_LH_PIDManager;  }
  if ( m_IsolationSelectionTool )         { m_IsolationSelectionTool = nullptr; delete m_IsolationSelectionTool; }
  if ( m_trigElMatchTool )                { m_trigElMatchTool = nullptr;        delete m_trigElMatchTool; }
  
  if ( m_useCutFlow ) {
    Info("finalize()", "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: histFinalize ()
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

int ElectronSelector :: passCuts( const xAOD::Electron* electron, const xAOD::Vertex *primaryVertex ) {

  float et    = electron->pt();
  
  // all the eta cuts are done using the measurement of the cluster position with the 2nd layer cluster,
  // as for Egamma CP  recommendation
  //
  float eta   = ( electron->caloCluster() ) ? electron->caloCluster()->etaBE(2) : -999.0;

  int oq      = static_cast<int>( electron->auxdata<uint32_t>("OQ") & 1446 );

  // fill cutflow bin 'all' before any cut
  m_el_cutflowHist_1->Fill( m_el_cutflow_all, 1 );
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_all, 1 ); }

  // *********************************************************************************************************************************************************************
  // 
  // author cut
  //
  if ( m_doAuthorCut ) {
    if ( !( electron->author(xAOD::EgammaParameters::AuthorElectron) || electron->author(xAOD::EgammaParameters::AuthorAmbiguous) ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed author kinematic cut." ); }
      return 0;
    }
  }  
  m_el_cutflowHist_1->Fill( m_el_cutflow_author_cut, 1 );        
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_author_cut, 1 ); }
  
  // *********************************************************************************************************************************************************************
  // 
  // Object Quality cut
  //
  if ( m_doOQCut ) {
    if ( !(oq == 0) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed Object Quality cut." ); }
      return 0;
    }
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_OQ_cut, 1 );        
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_OQ_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  // 
  // pT max cut
  //
  if ( m_pT_max != 1e8 ) {
    if ( et > m_pT_max ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed pT max cut." ); }
      return 0;
    }
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_ptmax_cut, 1 );        
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_ptmax_cut, 1 ); }
  
  // *********************************************************************************************************************************************************************
  //   
  // pT min cut
  //
  if ( m_pT_min != 1e8 ) {
    if ( et < m_pT_min ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed pT min cut." ); }
      return 0;
    }
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_ptmin_cut, 1 );        
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_ptmin_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  // 
  // eta cuts
  //
   
  // |eta| max cut
  //
  if ( m_eta_max != 1e8 ) {
    if ( fabs(eta) > m_eta_max ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed |eta| max cut." ); }
      return 0;
    }
  }
  // |eta| crack veto
  //
  if ( m_vetoCrack ) {
    if ( fabs( eta ) > 1.37 && fabs( eta ) < 1.52 ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed |eta| crack veto cut." ); }
      return 0;
    }
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_eta_cut, 1 );
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_eta_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  // 
  // electron PID cuts
  //

  //
  // likelihood PID
  //

  // set default values for *this* electron decorations
  //
  m_el_LH_PIDManager->setDecorations( electron );

  // retrieve only tools with WP >= selected WP, cut electrons if not satisfying selected WP, and decorate w/ tool decision all the others
  //
  typedef std::multimap< std::string, AsgElectronLikelihoodTool* > LHToolsMap;
  LHToolsMap myLHTools = m_el_LH_PIDManager->getValidTools();

  if ( m_doLHPIDcut && !( ( myLHTools.find( m_LHOperatingPoint )->second )->accept( *electron ) ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed likelihood PID cut w/ operating point %s", m_LHOperatingPoint.c_str() ); }
      return 0;
  }

  for ( auto it : (myLHTools) ) {

    const std::string decorWP =  "LH" + it.first;
    if ( m_debug ) {
      Info("PassCuts()", "Decorating electron with decison for LH WP : %s ", ( decorWP ).c_str() );
      Info("PassCuts()", "\t does electron pass %s ? %i ", ( decorWP ).c_str(), static_cast<int>( it.second->accept( *electron ) ) );
    }
    electron->auxdecor<char>(decorWP) = static_cast<char>( it.second->accept( *electron ) );

  }

  //
  // cut-based PID
  //

  // set default values for *this* electron decorations
  //
  m_el_CutBased_PIDManager->setDecorations( electron );

  // retrieve only tools with WP >= selected WP, cut electrons if not satisfying selected WP, and decorate w/ tool decision all the others
  //
  typedef std::multimap< std::string, AsgElectronIsEMSelector* > CutBasedToolsMap;
  CutBasedToolsMap myCutBasedTools = m_el_CutBased_PIDManager->getValidTools();

  if ( m_doCutBasedPIDcut && !( ( myCutBasedTools.find( m_CutBasedOperatingPoint )->second )->accept( *electron ) ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed cut-based PID cut." ); }
      return 0;
  }

  for ( auto it : (myCutBasedTools) ) {

    const std::string decorWP = it.second->getOperatingPointName( );

    if ( m_debug ) {
      Info("PassCuts()", "Decorating electron with decison for cut-based WP : %s ", ( decorWP ).c_str() );
      Info("PassCuts()", "\t does electron pass %s ? %i ", ( decorWP ).c_str(), static_cast<int>( it.second->accept( *electron ) ) );
    }

    electron->auxdecor<char>(decorWP) = static_cast<char>( it.second->accept( *electron ) );
  }
  
  m_el_cutflowHist_1->Fill( m_el_cutflow_PID_cut, 1 );
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_PID_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  // 
  // impact parameter cuts
  //

  // Tracking cuts AFTER acceptance, in case of derivation reduction.

  const xAOD::TrackParticle* tp  = electron->trackParticle();

  if ( !tp ) {
    if ( m_debug ) Info( "PassCuts()", "Electron has no TrackParticle. Won't be selected.");
    return 0;
  }
  
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  
  double d0_significance = fabs( xAOD::TrackingHelpers::d0significance( tp, eventInfo->beamPosSigmaX(), eventInfo->beamPosSigmaY(), eventInfo->beamPosSigmaXY() ) );
  float z0sintheta       = ( tp->z0() + tp->vz() - primaryVertex->z() ) * sin( tp->theta() );

  // z0*sin(theta) cut
  //
  if ( m_z0sintheta_max != 1e8 ) {
    if ( !( fabs(z0sintheta) < m_z0sintheta_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed z0*sin(theta) cut." ); }
      return 0;
    }
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_z0sintheta_cut, 1 );
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_z0sintheta_cut, 1 ); }
  
  // d0 cut
  //
  if ( m_d0_max != 1e8 ) {
    if ( !( tp->d0() < m_d0_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed d0 cut."); }
      return 0;
    }  
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_d0_cut, 1 );
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_d0_cut, 1 ); }

  // d0sig cut
  //
  if ( m_d0sig_max != 1e8 ) {
    if ( !( d0_significance < m_d0sig_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed d0 significance cut."); }
      return 0;
    }
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_d0sig_cut, 1 );
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill( m_el_cutflow_d0sig_cut, 1 ); }

  // decorate electron w/ d0sig info
  static SG::AuxElement::Decorator< float > d0SigDecor("d0sig");
  d0SigDecor( *electron ) = static_cast<float>(d0_significance);

  // *********************************************************************************************************************************************************************
  // 
  // isolation cut
  //
  static SG::AuxElement::Decorator< char > isIsoDecor("isIsolated");
  
  bool passIso(false); 
  if ( electron->caloCluster() && electron->caloCluster()->eta() < 2.47 ) {
    passIso = ( m_IsolationSelectionTool->accept( *electron ) );
  }
  isIsoDecor( *electron ) = ( passIso );

  if ( m_doIsolation && !passIso ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed isolation cut." ); }
      return 0;
  }
  m_el_cutflowHist_1->Fill( m_el_cutflow_iso_cut, 1 );
  if ( m_isUsedBefore ) { m_el_cutflowHist_2->Fill(m_el_cutflow_iso_cut , 1 ); }

  return 1;
}


