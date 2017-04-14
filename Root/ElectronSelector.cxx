// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <tuple>

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
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/AsgElectronIsEMSelector.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TriggerMatchingTool/MatchingTool.h"
#include "PATCore/TAccept.h"

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronSelector)

ElectronSelector :: ElectronSelector (std::string className) :
    Algorithm(className),
    m_cutflowHist(nullptr),
    m_cutflowHistW(nullptr),
    m_el_cutflowHist_1(nullptr),
    m_el_cutflowHist_2(nullptr),
    m_isolationSelectionTool_handle("CP::IsolationSelectionTool/ElectronIsoSelToolName", nullptr),
    m_el_LH_PIDManager(nullptr),
    m_el_CutBased_PIDManager(nullptr),
    m_trigDecTool(nullptr),
    m_trigElectronMatchTool_handle("Trig::MatchingTool/TrigElectronMatchingName", nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronSelector()", "Calling constructor");

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
  m_outputAlgoSystNames     = "ElectronSelector_Syst";


  // decorate selected objects that pass the cuts
  //
  m_decorateSelectedObjects = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::VIEW_ELEMENTS option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainer = false;
  // if requested, a new container is made using the SG::VIEW_ELEMENTS option
  m_outContainerName        = "";

  // if only want to look at a subset of object
  //
  m_nToProcess              = -1;

  // configurable cuts
  //
  m_pass_max                = -1;
  m_pass_min                = -1;
  m_pT_max                  = 1e8;
  m_pT_min                  = 1e8;
  m_eta_max                 = 1e8;
  m_vetoCrack               = true;
  m_d0_max                  = 1e8;
  m_d0sig_max           = 1e8;
  m_z0sintheta_max          = 1e8;
  m_doAuthorCut             = true;
  m_doOQCut                 = true;

  m_readIDFlagsFromDerivation = false;

  // likelihood-based PID
  m_doLHPID                 = true;
  m_doLHPIDcut              = false;
  m_LHOperatingPoint        = "Loose";

  // cut-based PID
  m_doCutBasedPID           = false;
  m_doCutBasedPIDcut        = false;
  m_CutBasedOperatingPoint  = "Loose";

  // isolation stuff
  //
  m_MinIsoWPCut             = "";
  m_IsoWPList       = "LooseTrackOnly,Loose,Tight,Gradient,GradientLoose";
  m_CaloIsoEff              = "0.1*x+90";
  m_TrackIsoEff             = "98";
  m_CaloBasedIsoType        = "topoetcone20";
  m_TrackBasedIsoType       = "ptvarcone20";

  // trigger matching stuff
  //
  m_singleElTrigChains      = "";
  m_diElTrigChains          = "";
  m_minDeltaR               = 0.07; // Recommended threshold for egamma triggers: see https://svnweb.cern.ch/trac/atlasoff/browser/Trigger/TrigAnalysis/TriggerMatchingTool/trunk/src/TestMatchingToolAlg.cxx
  m_doTrigMatch             = true;

}

ElectronSelector::~ElectronSelector() {}

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
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");

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
    m_el_cutflowHist_1 = (TH1D*)file->Get("cutflow_electrons_1");

    m_el_cutflow_all             = m_el_cutflowHist_1->GetXaxis()->FindBin("all");
    m_el_cutflow_author_cut      = m_el_cutflowHist_1->GetXaxis()->FindBin("author_cut");
    m_el_cutflow_OQ_cut          = m_el_cutflowHist_1->GetXaxis()->FindBin("OQ_cut");
    m_el_cutflow_ptmax_cut       = m_el_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");
    m_el_cutflow_ptmin_cut       = m_el_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");
    m_el_cutflow_eta_cut         = m_el_cutflowHist_1->GetXaxis()->FindBin("eta_cut"); // including crack veto, if applied
    m_el_cutflow_z0sintheta_cut  = m_el_cutflowHist_1->GetXaxis()->FindBin("z0sintheta_cut");
    m_el_cutflow_d0_cut          = m_el_cutflowHist_1->GetXaxis()->FindBin("d0_cut");
    m_el_cutflow_d0sig_cut       = m_el_cutflowHist_1->GetXaxis()->FindBin("d0sig_cut");
    m_el_cutflow_BL_cut          = m_el_cutflowHist_1->GetXaxis()->FindBin("BL_cut");
    m_el_cutflow_PID_cut         = m_el_cutflowHist_1->GetXaxis()->FindBin("PID_cut");
    m_el_cutflow_iso_cut         = m_el_cutflowHist_1->GetXaxis()->FindBin("iso_cut");

    if ( m_isUsedBefore ) {
      m_el_cutflowHist_2 = (TH1D*)file->Get("cutflow_electrons_2");

      m_el_cutflow_all       = m_el_cutflowHist_2->GetXaxis()->FindBin("all");
      m_el_cutflow_author_cut    = m_el_cutflowHist_2->GetXaxis()->FindBin("author_cut");
      m_el_cutflow_OQ_cut    = m_el_cutflowHist_2->GetXaxis()->FindBin("OQ_cut");
      m_el_cutflow_ptmax_cut     = m_el_cutflowHist_2->GetXaxis()->FindBin("ptmax_cut");
      m_el_cutflow_ptmin_cut     = m_el_cutflowHist_2->GetXaxis()->FindBin("ptmin_cut");
      m_el_cutflow_eta_cut     = m_el_cutflowHist_2->GetXaxis()->FindBin("eta_cut"); // including crack veto, if applied
      m_el_cutflow_z0sintheta_cut  = m_el_cutflowHist_2->GetXaxis()->FindBin("z0sintheta_cut");
      m_el_cutflow_d0_cut    = m_el_cutflowHist_2->GetXaxis()->FindBin("d0_cut");
      m_el_cutflow_d0sig_cut     = m_el_cutflowHist_2->GetXaxis()->FindBin("d0sig_cut");
      m_el_cutflow_BL_cut    = m_el_cutflowHist_2->GetXaxis()->FindBin("BL_cut");
      m_el_cutflow_PID_cut     = m_el_cutflowHist_2->GetXaxis()->FindBin("PID_cut");
      m_el_cutflow_iso_cut     = m_el_cutflowHist_2->GetXaxis()->FindBin("iso_cut");
    }

  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

  // Compatible with Efficiency Nomenclature
  if( m_LHOperatingPoint == "LooseAndBLayer" )
    m_LHOperatingPoint = "LooseBL";

  if ( // m_LHOperatingPoint != "VeryLoose"       &&
       m_LHOperatingPoint != "Loose"           &&
       m_LHOperatingPoint != "LooseBL"         &&
       m_LHOperatingPoint != "Medium"          &&
       m_LHOperatingPoint != "Tight"     ) {
    Error("initialize()", "Unknown electron likelihood PID requested %s!",m_LHOperatingPoint.c_str());
    return EL::StatusCode::FAILURE;
  }
  if ( m_CutBasedOperatingPoint != "Loose"  &&
       m_CutBasedOperatingPoint != "Medium" &&
       m_CutBasedOperatingPoint != "Tight"  ) {
    Error("initialize()", "Unknown electron cut-based PID requested %s!",m_CutBasedOperatingPoint.c_str());
    return EL::StatusCode::FAILURE;
  }

  // Parse input isolation WP list, split by comma, and put into a vector for later use
  // Make sure it's not empty!
  //
  if ( m_IsoWPList.empty() ) {
    m_IsoWPList = "LooseTrackOnly,Loose,Tight,Gradient,GradientLoose";
  }
  std::string token;
  std::istringstream ss(m_IsoWPList);
  while ( std::getline(ss, token, ',') ) {
    m_IsoKeys.push_back(token);
  }

  if ( m_inContainerName.empty() ) {
    Error("initialize()", "InputContainer is empty!");
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
  
  // Assume no local BLayer track quality cut unless doing LooseBL and reading decision from the derivation
  m_doBLTrackQualityCut     = false;

  // initialise PID tool(s) using classes defined in ParticlePIDManager.h
  //
  // if not using cut-based PID, make sure all the decorations will be set ... by choosing the loosest WP!
  if( m_doCutBasedPID ){
    std::string cutbasedWP = ( m_doCutBasedPIDcut ) ? m_CutBasedOperatingPoint : "Loose";
    m_el_CutBased_PIDManager = new ElectronCutBasedPIDManager( cutbasedWP, m_debug );

    if  ( m_doCutBasedPIDcut ) {
      Info("initialize()", "Cutting on Electron Cut-Based PID! \n ********************" );
      Info("initialize()", "Selected cut-based WP: %s", (m_el_CutBased_PIDManager->getSelectedWP()).c_str() );
    } else {
      Info("initialize()", "Will decorate each electron with all Electron Cut-Based PID WPs decision (pass/not pass)!" );
    }

    if ( m_readIDFlagsFromDerivation ) {
      Info("initialize()", "Reading Electron cut-based ID from DAODs ..." );
      RETURN_CHECK( "ElectronSelector::initialize()", m_el_CutBased_PIDManager->setupWPs( false ), "Failed to setup ElectronCutBasedPIDManager in Derivation mode." );
    } else {
      Info("initialize()", "Reading Electron cut-based ID from CP Tool ..." );
      RETURN_CHECK( "ElectronSelector::initialize()", m_el_CutBased_PIDManager->setupWPs( true, this->m_name ), "Failed to setup ElectronCutBasedPIDManager in tool mode." );
    }
  }// if m_doCutBasedPID

  if( m_doLHPID ){
    // if not using LH PID, make sure all the decorations will be set ... by choosing the loosest WP!
    std::string likelihoodWP = ( m_doLHPIDcut ) ? m_LHOperatingPoint : "Loose"; 
    m_el_LH_PIDManager = new ElectronLHPIDManager( likelihoodWP, m_debug );


    if  ( m_doLHPIDcut ) {
         Info("initialize()", "Cutting on Electron Likelihood PID! \n ********************" );
         Info("initialize()", "\t Input WP: %s corresponding to actual LikeEnum::Menu WP: %s", likelihoodWP.c_str(), (m_el_LH_PIDManager->getSelectedWP()).c_str() );
    } else {
         Info("initialize()", "Will decorate each electron with all Electron Likelihood PID WPs decision (pass/not pass)!" );
    }

    if ( m_readIDFlagsFromDerivation ) {
      // LooseBL is not in Derivations, so choose Loose and do BLayer cut locally
      if( m_LHOperatingPoint == "LooseBL" ){
        m_LHOperatingPoint = "Loose";
        m_doBLTrackQualityCut = true;
      }

      Info("initialize()", "Reading Electron LH ID from DAODs ..." );
      RETURN_CHECK( "ElectronSelector::initialize()", m_el_LH_PIDManager->setupWPs( false ), "Failed to setup ElectronLHPIDManager in Derivation mode." );
    } else {
      Info("initialize()", "Reading Electron LH ID from CP Tool ..." );
      RETURN_CHECK( "ElectronSelector::initialize()", m_el_LH_PIDManager->setupWPs( true, this->m_name), "Failed to setup ElectronLHPIDManager in tool mode." );
    }
  }// if m_doLHPID

  // *************************************
  //
  // Initialise CP::IsolationSelectionTool
  //
  // *************************************

  m_isolationSelectionTool_name                  = "IsolationSelectionTool_" + m_name;
  std::string isolationSelectionTool_handle_name = "CP::IsolationSelectionTool/" + m_isolationSelectionTool_name;

  RETURN_CHECK("ElectronSelector::initialize()", checkToolStore<CP::IsolationSelectionTool>(m_isolationSelectionTool_name), "" );
  RETURN_CHECK("ElectronSelector::initialize()", m_isolationSelectionTool_handle.makeNew<CP::IsolationSelectionTool>(isolationSelectionTool_handle_name), "Failed to create handle to CP::IsolationSelectionTool");
  // Do this only for the first WP in the list
  //
  if ( m_debug ) { Info("initialize()", "Adding isolation WP %s to IsolationSelectionTool", (m_IsoKeys.at(0)).c_str() ); }
  RETURN_CHECK("ElectronSelector::initialize()", m_isolationSelectionTool_handle.setProperty("ElectronWP", (m_IsoKeys.at(0)).c_str()), "Failed to configure base WP" );
  RETURN_CHECK("ElectronSelector::initialize()", m_isolationSelectionTool_handle.initialize(), "Failed to properly initialize IsolationSelectionTool." );

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

       RETURN_CHECK( "ElectronSelector::initialize()",  m_isolationSelectionTool_handle->addUserDefinedWP((*WP_itr).c_str(), xAOD::Type::Electron, myCuts, "", iso_type), "Failed to add user-defined isolation WP" );

     } else {

        RETURN_CHECK( "ElectronSelector::initialize()", m_isolationSelectionTool_handle->addElectronWP( (*WP_itr).c_str() ), "Failed to add isolation WP" );

     }
  }

  //m_isolationSelectionTool_handle->msg().setLevel(MSG::ERROR); // ERROR, VERBOSE, DEBUG, INFO

  // ***************************************
  //
  // Initialise Trig::MatchingTool
  //
  // ***************************************

  //
  // NB: need to retrieve the TrigDecisionTool from asg::ToolStore to configure the tool!
  //     do not initialise if there are no input trigger chains
  //
  if(  !( m_singleElTrigChains.empty() && m_diElTrigChains.empty() ) && asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) ) {

    m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
    ToolHandle<Trig::TrigDecisionTool> trigDecHandle( m_trigDecTool );

    //  everything went fine, let's initialise the tool!
    //
    m_trigElMatchTool_name                  = "MatchingTool_" + m_name;
    std::string trigElectronMatchTool_handle_name = "Trig::MatchingTool/" + m_trigElMatchTool_name;

    RETURN_CHECK("ElectronSelector::initialize()", checkToolStore<Trig::MatchingTool>(m_trigElMatchTool_name), "" );
    RETURN_CHECK("ElectronSelector::initialize()", m_trigElectronMatchTool_handle.makeNew<Trig::MatchingTool>(trigElectronMatchTool_handle_name), "Failed to create handle to MatchingTool");
    RETURN_CHECK("ElectronSelector::initialize()", m_trigElectronMatchTool_handle.setProperty( "TrigDecisionTool", trigDecHandle ), "Failed to pass TrigDecisionTool to MatchingTool" );
    RETURN_CHECK("ElectronSelector::initialize()", m_trigElectronMatchTool_handle.initialize(), "Failed to properly initialize MatchingTool" );

  } else {

    m_doTrigMatch = false;

    std::cout << "***********************************************************" << std::endl;
    Warning("initialize()", "Will not perform any electron trigger matching at this stage b/c :");
    std::cout << "" << std::endl;
    std::cout << "\t -) could not find the TrigDecisionTool in asg::ToolStore" << std::endl;
    std::cout << "\t AND/OR" << std::endl;
    std::cout << "\t -) all input HLT trigger chain lists are empty" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "However, if you really didn't want to do the matching now, it's all good!" << std::endl;
    std::cout << "***********************************************************" << std::endl;
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

    // parse input electron trigger chain list, split by comma and fill vector
    //
    std::string singleel_trig;
    std::istringstream ss_singleel_trig(m_singleElTrigChains);

    while ( std::getline(ss_singleel_trig, singleel_trig, ',') ) {
      m_singleElTrigChainsList.push_back(singleel_trig);
    }

    std::string diel_trig;
    std::istringstream ss_diel_trig(m_diElTrigChains);

    while ( std::getline(ss_diel_trig, diel_trig, ',') ) {
      m_diElTrigChainsList.push_back(diel_trig);
    }

    Info("execute()", "Input single electron trigger chains that will be considered for matching:\n");
    for ( auto const &chain : m_singleElTrigChainsList ) { Info("execute()", "\t %s", chain.c_str()); }
    Info("execute()", "\n");

    Info("execute()", "Input di-electron trigger chains that will be considered for matching:\n");
    for ( auto const &chain : m_diElTrigChainsList ) { Info("execute()", "\t %s", chain.c_str()); }
    Info("execute()", "\n");

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
        vecOutContainerNames->push_back( systName );
      }

      // if for at least one syst set the event passes selection, this will remain true!
      //
      eventPass = ( eventPass || eventPassThisSyst );

      if ( m_debug ) { Info("execute()", " syst name: %s  output container name: %s ", systName.c_str(), (m_outContainerName+systName).c_str() ); }

      if ( m_createSelectedContainer ) {
        if ( eventPassThisSyst ) {
          // add ConstDataVector to TStore
          RETURN_CHECK( "ElectronSelector::execute()", m_store->record( selectedElectrons, m_outContainerName+systName ), "Failed to store const data container");
        } else {
          // if the event does not pass the selection for this syst, CDV won't be ever recorded to TStore, so we have to delete it!
          delete selectedElectrons; selectedElectrons = nullptr;
        }
      }

    } // close loop over syst sets

    if ( m_debug ) {  Info("execute()", " output list of syst size: %i ", static_cast<int>(vecOutContainerNames->size()) ); }

    // record in TStore the list of systematics names that should be considered down stream
    //
    RETURN_CHECK( "ElectronSelector::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  }

  // look what we have in TStore
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
  RETURN_CHECK("ElectronSelector::executeSelection()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
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

  if ( m_debug ) { Info("executeSelection()", "Initial electrons:%i - Selected electrons: %i", nObj , nPass ); }

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
  //  2. there are no selected muons in the event
  //
  if ( m_doTrigMatch && selectedElectrons ) {

    unsigned int nSelectedElectrons = selectedElectrons->size();

    static SG::AuxElement::Decorator< std::map<std::string,char> > isTrigMatchedMapElDecor( "isTrigMatchedMapEl" );

    if ( nSelectedElectrons > 0 ) {

      if ( m_debug ) { Info("executeSelection()", "Doing single electron trigger matching..."); }

      for ( auto const &chain : m_singleElTrigChainsList ) {

        if ( m_debug ) { Info("executeSelection()", "\t checking trigger chain %s", chain.c_str()); }

        for ( auto const electron : *selectedElectrons ) {

          //  For each electron, decorate w/ a map<string,char> with the 'isMatched' info associated
          //  to each trigger chain in the input list.
          //  If decoration map doesn't exist for this electron yet, create it (will be done only for the 1st iteration on the chain names)
          //
          if ( !isTrigMatchedMapElDecor.isAvailable( *electron ) ) {
            isTrigMatchedMapElDecor( *electron ) = std::map<std::string,char>();
          }

          char matched = ( m_trigElectronMatchTool_handle->match( *electron, chain, m_minDeltaR ) );

          if ( m_debug ) { Info("executeSelection()", "\t\t is electron trigger matched? %i", matched); }

          ( isTrigMatchedMapElDecor( *electron ) )[chain] = matched;
        }
      }
    }// if nSelectedElectrons > 0

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

    if ( nSelectedElectrons > 1 && !m_diElTrigChains.empty() ) {

      if ( m_debug ) { Info("executeSelection()", "Doing di-electron trigger matching..."); }

      const xAOD::EventInfo* eventInfo(nullptr);
      RETURN_CHECK("ElectronSelector::executeSelection()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

      typedef std::pair< std::pair<unsigned int,unsigned int>, char>     dielectron_trigmatch_pair;
      typedef std::multimap< std::string, dielectron_trigmatch_pair >    dielectron_trigmatch_pair_map;      
      static SG::AuxElement::Decorator< dielectron_trigmatch_pair_map >  diElectronTrigMatchPairMapDecor( "diElectronTrigMatchPairMap" );

      for ( auto const &chain : m_diElTrigChainsList ) {

        if ( m_debug ) { Info("executeSelection()", "\t checking trigger chain %s", chain.c_str()); }

        //  If decoration map doesn't exist for this event yet, create it (will be done only for the 1st iteration on the chain names)
        if ( !diElectronTrigMatchPairMapDecor.isAvailable( *eventInfo ) ) {
          diElectronTrigMatchPairMapDecor( *eventInfo ) = dielectron_trigmatch_pair_map();
        } 

        std::vector<const xAOD::IParticle*> myElectrons;

        for ( unsigned int iel = 0; iel < selectedElectrons->size()-1; ++iel ) {

          for ( unsigned int jel = iel+1; jel < selectedElectrons->size(); ++jel ) {

            // test a new pair
            myElectrons.clear();
            myElectrons.push_back( selectedElectrons->at(iel) );
            myElectrons.push_back( selectedElectrons->at(jel) );

            // check whether the pair is matched
            char matched = m_trigElectronMatchTool_handle->match( myElectrons, chain, m_minDeltaR );

            if ( m_debug ) { Info("executeSelection()", "\t\t is the electron pair (%i,%i) trigger matched? %i", iel, jel, matched); }

            std::pair <unsigned int, unsigned int>  chain_idxs = std::make_pair(iel,jel);
            dielectron_trigmatch_pair                   chain_decision = std::make_pair(chain_idxs,matched);
            diElectronTrigMatchPairMapDecor( *eventInfo ).insert( std::pair< std::string, dielectron_trigmatch_pair >(chain,chain_decision) );
      
          }
        }
      }//for m_diElTrigChainsList
    } // if electrons
  }// if m_doTrigMatch

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

  if ( m_el_CutBased_PIDManager ) { delete m_el_CutBased_PIDManager;  m_el_CutBased_PIDManager = nullptr; }
  if ( m_el_LH_PIDManager )       { delete m_el_LH_PIDManager;        m_el_LH_PIDManager = nullptr;   }
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
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");

  return EL::StatusCode::SUCCESS;
}

int ElectronSelector :: passCuts( const xAOD::Electron* electron, const xAOD::Vertex *primaryVertex ) {

  float et    = electron->pt();

  // all the eta cuts are done using the measurement of the cluster position with the 2nd layer cluster,
  // as for Egamma CP  recommendation
  //
  float eta   = ( electron->caloCluster() ) ? electron->caloCluster()->etaBE(2) : -999.0;

  // fill cutflow bin 'all' before any cut
  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_all, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_all, 1 ); }

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
  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_author_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_author_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  //
  // Object Quality cut
  //
  if ( m_doOQCut ) {
    if( !electron->isGoodOQ(xAOD::EgammaParameters::BADCLUSELECTRON) ){
      if ( m_debug ) { Info("PassCuts()", "Electron failed Object Quality cut BADCLUSELECTRON." ); }
      return 0;
    }
  }
  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_OQ_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_OQ_cut, 1 ); }

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
  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_ptmax_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_ptmax_cut, 1 ); }

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
  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_ptmin_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_ptmin_cut, 1 ); }

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
  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_eta_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_eta_cut, 1 ); }


  // *********************************************************************************************************************************************************************
  //
  // Tracking cuts AFTER acceptance, in case of derivation reduction.
  //

  const xAOD::TrackParticle* tp  = electron->trackParticle();
  if ( !tp ) {
    if ( m_debug ) Info( "PassCuts()", "Electron has no TrackParticle. Won't be selected.");
    return 0;
  }

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  double d0_significance = fabs( xAOD::TrackingHelpers::d0significance( tp, eventInfo->beamPosSigmaX(), eventInfo->beamPosSigmaY(), eventInfo->beamPosSigmaXY() ) );

  // Take distance between z0 and zPV ( after referring the PV z coordinate to the beamspot position, given by vz() ), multiplied by sin(theta)
  // see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackingDC14 for further reference
  //
  float z0sintheta = 1e8;
  if (primaryVertex) z0sintheta = ( tp->z0() + tp->vz() - primaryVertex->z() ) * sin( tp->theta() );

  // z0*sin(theta) cut
  //
  if ( m_z0sintheta_max != 1e8 ) {
    if ( !( fabs(z0sintheta) < m_z0sintheta_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed z0*sin(theta) cut." ); }
      return 0;
    }
  }
  if ( m_useCutFlow ) m_el_cutflowHist_1->Fill( m_el_cutflow_z0sintheta_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_z0sintheta_cut, 1 ); }

  // decorate electron w/ z0*sin(theta) info
  static SG::AuxElement::Decorator< float > z0sinthetaDecor("z0sintheta");
  z0sinthetaDecor( *electron ) = z0sintheta;

  // d0 cut
  //
  if ( m_d0_max != 1e8 ) {
    if ( !( tp->d0() < m_d0_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed d0 cut."); }
      return 0;
    }
  }
  if ( m_useCutFlow ) m_el_cutflowHist_1->Fill( m_el_cutflow_d0_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_d0_cut, 1 ); }

  // d0sig cut
  //
  if ( m_d0sig_max != 1e8 ) {
    if ( !( d0_significance < m_d0sig_max ) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed d0 significance cut."); }
      return 0;
    }
  }
  if ( m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_d0sig_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_d0sig_cut, 1 ); }

  // decorate electron w/ d0sig info
  static SG::AuxElement::Decorator< float > d0SigDecor("d0sig");
  d0SigDecor( *electron ) = static_cast<float>(d0_significance);


  // BLayer track quality cut
  //
  if ( m_doBLTrackQualityCut ) {

    // this is taken from ElectronPhotonSelectorTools/Root/AsgElectronLikelihoodTool.cxx

    uint8_t expectBlayer(true);
    uint8_t nBlayerHits(0);
    uint8_t nBlayerOutliers(0);

    tp->summaryValue(expectBlayer,    xAOD::expectBLayerHit);
    tp->summaryValue(nBlayerHits,     xAOD::numberOfBLayerHits);
    tp->summaryValue(nBlayerOutliers, xAOD::numberOfBLayerOutliers);

    if ( expectBlayer && (nBlayerHits+nBlayerOutliers) < 1 ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed BL track quality cut."); }
      return 0;
    }
  }
  if ( m_useCutFlow ) m_el_cutflowHist_1->Fill( m_el_cutflow_BL_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_BL_cut, 1 ); }

  // *********************************************************************************************************************************************************************
  //
  // electron PID cuts
  //

  //
  // likelihood PID
  //

  // set default values for *this* electron decorations
  //
  if( m_doLHPID ) {
    m_el_LH_PIDManager->setDecorations( electron );

    if ( m_readIDFlagsFromDerivation ) {
  
      if ( m_doLHPIDcut ) {
  
        bool passSelID(false);
        static SG::AuxElement::ConstAccessor< char > LHDecision( "DFCommonElectronsLH" + m_LHOperatingPoint );
        if( LHDecision.isAvailable( *electron ) )
            passSelID = LHDecision( *electron );
  
        if ( !passSelID ) {
          if ( m_debug ) { Info("PassCuts()", "Electron failed likelihood PID cut w/ operating point %s", m_LHOperatingPoint.c_str() ); }
          return 0;
        }
      }
  
      const std::set<std::string> myLHWPs = m_el_LH_PIDManager->getValidWPs();
      for ( auto it : (myLHWPs) ) {
  
        const std::string decorWP =  "LH"+it;
  
        bool passThisID(false);
        SG::AuxElement::ConstAccessor< char > LHDecisionAll( "DFCommonElectrons" + decorWP );
        if( LHDecisionAll.isAvailable( *electron ) )
            passThisID = LHDecisionAll( *electron );
  
        if ( m_debug ) {
          Info("PassCuts()", "Decorating electron with decision for LH WP : %s ", ( decorWP ).c_str() );
          Info("PassCuts()", "\t does electron pass %s ? %i ", ( decorWP ).c_str(), passThisID );
        }
        electron->auxdecor<char>(decorWP) = static_cast<char>( passThisID );
  
      }
  
    } else {
  
      // retrieve only tools with WP >= selected WP, cut electrons if not satisfying selected WP, and decorate w/ tool decision all the others
      //
      typedef std::multimap< std::string, AsgElectronLikelihoodTool* > LHToolsMap;
      LHToolsMap myLHTools = m_el_LH_PIDManager->getValidWPTools();
  
      if ( m_doLHPIDcut && !( ( myLHTools.find( m_LHOperatingPoint )->second )->accept( *electron ) ) ) {
        if ( m_debug ) { Info("PassCuts()", "Electron failed likelihood PID cut w/ operating point %s", m_LHOperatingPoint.c_str() ); }
        return 0;
      }
  
      for ( auto it : (myLHTools) ) {
  
        const std::string decorWP =  "LH" + it.first;
        if ( m_debug ) {
          Info("PassCuts()", "Decorating electron with decision for LH WP : %s ", ( decorWP ).c_str() );
          Info("PassCuts()", "\t does electron pass %s ? %i ", ( decorWP ).c_str(), static_cast<int>( it.second->accept( *electron ) ) );
        }
        electron->auxdecor<char>(decorWP) = static_cast<char>( it.second->accept( *electron ) );
  
      }
  
    }
  }// if m_doLHPID

  //
  // cut-based PID
  //

  // set default values for *this* electron decorations
  //
  if( m_doCutBasedPID ) {
    m_el_CutBased_PIDManager->setDecorations( electron );

    if ( m_readIDFlagsFromDerivation ) {
  
      if ( m_doCutBasedPIDcut ) {
  
        bool passSelID(false);
        static SG::AuxElement::ConstAccessor< char > CutDecision( "DFCommonElectronsIsEM" + m_CutBasedOperatingPoint );
        if( CutDecision.isAvailable( *electron ) )
            passSelID = CutDecision( *electron );
  
        if ( !passSelID ) {
          if ( m_debug ) { Info("PassCuts()", "Electron failed cut-based PID cut w/ operating point %s", m_CutBasedOperatingPoint.c_str() ); }
          return 0;
        }
  
      }
  
      const std::set<std::string> myCutBasedWPs = m_el_CutBased_PIDManager->getValidWPs();
      for ( auto it : (myCutBasedWPs) ) {
  
        const std::string decorWP = "IsEM"+it;
  
        bool passThisID(false);
        SG::AuxElement::ConstAccessor< char > CutDecisionAll( "DFCommonElectrons" + decorWP );
        if( CutDecisionAll.isAvailable( *electron ) )
            passThisID = CutDecisionAll( *electron );
  
        if ( m_debug ) {
          Info("PassCuts()", "Decorating electron with decision for cut-based WP : %s ", ( decorWP ).c_str() );
          Info("PassCuts()", "\t does electron pass %s ? %i ", ( decorWP ).c_str(), passThisID );
        }
        electron->auxdecor<char>(decorWP) = static_cast<char>( passThisID );
  
      }
  
    } else {
  
      // retrieve only tools with WP >= selected WP, cut electrons if not satisfying selected WP, and decorate w/ tool decision all the others
      //
      typedef std::multimap< std::string, AsgElectronIsEMSelector* > CutBasedToolsMap;
      CutBasedToolsMap myCutBasedTools = m_el_CutBased_PIDManager->getValidWPTools();
  
      if ( m_doCutBasedPIDcut && !( ( myCutBasedTools.find( m_CutBasedOperatingPoint )->second )->accept( *electron ) ) ) {
        if ( m_debug ) { Info("PassCuts()", "Electron failed cut-based PID cut." ); }
        return 0;
      }
  
      for ( auto it : (myCutBasedTools) ) {
  
        const std::string decorWP = "IsEM"+it.second->getOperatingPointName( );
  
        if ( m_debug ) {
          Info("PassCuts()", "Decorating electron with decision for cut-based WP : %s ", ( decorWP ).c_str() );
          Info("PassCuts()", "\t does electron pass %s ? %i ", ( decorWP ).c_str(), static_cast<int>( it.second->accept( *electron ) ) );
        }
  
        electron->auxdecor<char>(decorWP) = static_cast<char>( it.second->accept( *electron ) );
      }
  
    }
  }// if m_doCutBasedPID

  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_PID_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_PID_cut, 1 ); }


  // *********************************************************************************************************************************************************************
  //
  // isolation cut
  //

  // Get the "list" of input WPs with the accept() decision from the tool
  //
  Root::TAccept accept_list = m_isolationSelectionTool_handle->accept( *electron );

  // Decorate w/ decision for all input WPs
  //
  std::string base_decor("isIsolated");
  for ( auto WP_itr : m_IsoKeys ) {

    std::string decorWP = base_decor + "_" + WP_itr;

    if ( m_debug ) { Info("PassCuts()", "Decorate electron with %s - accept() ? %i", decorWP.c_str(), accept_list.getCutResult( WP_itr.c_str()) ); }
    electron->auxdecor<char>(decorWP) = static_cast<char>( accept_list.getCutResult( WP_itr.c_str() ) );

  }

  // Apply the cut if needed
  //
  if ( !m_MinIsoWPCut.empty() && !accept_list.getCutResult( m_MinIsoWPCut.c_str() ) ) {
    if ( m_debug ) { Info("PassCuts()", "Electron failed isolation cut %s ",  m_MinIsoWPCut.c_str() ); }
    return 0;
  }
  if(m_useCutFlow) m_el_cutflowHist_1->Fill( m_el_cutflow_iso_cut, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_el_cutflowHist_2->Fill( m_el_cutflow_iso_cut, 1 ); }

  return 1;
}


