// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODJet/JetContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"

// package include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/JetSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// external tools include(s):

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetSelector)


JetSelector :: JetSelector (std::string className) :
    Algorithm(className),
    m_cutflowHist(nullptr),
    m_cutflowHistW(nullptr),
    m_jet_cutflowHist_1(nullptr),
    m_BJetSelectTool(nullptr),
    m_JVT_tool_handle("CP::JetJvtEfficiency/JVTToolName", nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  if(m_debug) Info("JetSelector()", "Calling constructor");

  // read debug flag from .config file
  m_debug         = false;
  m_useCutFlow    = true;

  // input container to be read from TEvent or TStore
  m_inContainerName         = "";
  m_jetScaleType            = "";

  // name of algo input container comes from - only if running on syst
  m_inputAlgo               = "";
  m_outputAlgo              = "";

  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects = true;
  m_decor                   = "passSel";

  // additional functionality : create output container of selected objects
  //                            using the SG::VIEW_ELEMENTS option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainer = false;
  // if requested, a new container is made using the SG::VIEW_ELEMENTS option
  m_outContainerName        = "";
  // if only want to look at a subset of object
  m_nToProcess              = -1;

  // cuts
  m_cleanJets               = true;
  m_cleanEvtLeadJets        = -1; // indepedent of previous switch
  m_cleanEvent              = false;
  m_pass_max                = -1;
  m_pass_min                = -1;
  m_pT_max                  = 1e8;
  m_pT_min                  = 1e8;
  m_eta_max                 = 1e8;
  m_eta_min                 = 1e8;
  m_detEta_max              = 1e8;
  m_detEta_min              = 1e8;
  m_mass_max                = 1e8;
  m_mass_min                = 1e8;
  m_rapidity_max            = 1e8;
  m_rapidity_min            = 1e8;
  m_truthLabel 	            = -1;
  m_useHadronConeExcl       = true;

  m_doJVF                   = false;
  m_pt_max_JVF              = 50e3;
  m_eta_max_JVF             = 2.4;
  m_JVFCut                  = 0.5;
  m_doJVT                   = false;

  m_pt_max_JVT              = 60e3;
  m_eta_max_JVT             = 2.4;
  m_JVTCut                  = -1.0;
  m_WorkingPointJVT         = "Medium";

  m_systValJVT 	            = 0.0;
  m_systNameJVT	            = "";
  m_outputSystNamesJVT      = "JetJvtEfficiency_JVTSyst";

  // Btag quality
  m_doBTagCut               = false;
  m_corrFileName            = "$ROOTCOREBIN/data/xAODBTaggingEfficiency/cutprofiles_22072015.root";
  m_taggerName              = "MV2c20";
  m_operatingPt             = "FixedCutBEff_70";
  m_jetAuthor               = "AntiKt4EMTopoJets";
  // for the b-tagging tool - these are the b-tagging groups minimums
  // users making tighter cuts can use the selector's parameters to keep
  // things consistent
  m_b_eta_max               = 2.5;
  m_b_pt_min                = 20000;

  // HLT Btag quality
  m_doHLTBTagCut            = false;
  m_HLTBTagTaggerName       = "MV2c20";
  m_HLTBTagCutValue         = -0.4434;
  m_requireHLTVtx           = false;
  m_requireNoHLTVtx         = false;

  m_passAuxDecorKeys        = "";
  m_failAuxDecorKeys        = "";

}

EL::StatusCode JetSelector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  if(m_debug) Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "JetSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  if(m_debug) Info("histInitialize()", "Calling histInitialize");
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  if(m_debug) Info("fileExecute()", "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  if(m_debug) Info("changeInput()", "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  if ( m_debug ) Info("initialize()", "Calling initialize");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetSelector::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

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
    m_jet_cutflowHist_1 = (TH1D*)file->Get("cutflow_jets_1");

    m_jet_cutflow_all             = m_jet_cutflowHist_1->GetXaxis()->FindBin("all");
    m_jet_cutflow_ptmax_cut       = m_jet_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");
    m_jet_cutflow_ptmin_cut       = m_jet_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");
    m_jet_cutflow_eta_cut         = m_jet_cutflowHist_1->GetXaxis()->FindBin("eta_cut");
    m_jet_cutflow_jvt_cut         = m_jet_cutflowHist_1->GetXaxis()->FindBin("JVT_cut");
    m_jet_cutflow_btag_cut        = m_jet_cutflowHist_1->GetXaxis()->FindBin("BTag_cut");
    m_jet_cutflow_cleaning_cut    = m_jet_cutflowHist_1->GetXaxis()->FindBin("cleaning_cut");

  }

  //If not set, find default from input container name
  if (m_jetScaleType.size() == 0){
    if( m_inContainerName.find("EMTopo") != std::string::npos){
      m_jetScaleType = "JetEMScaleMomentum";
    }else{
      m_jetScaleType = "JetConstitScaleMomentum";
    }
  }


  if ( m_outputAlgo.empty() ) {
    m_outputAlgo = m_inputAlgo + "_JetSelect";
  }

  m_isEMjet = m_inContainerName.find("EMTopoJets") != std::string::npos;
  m_isLCjet = m_inContainerName.find("LCTopoJets") != std::string::npos;

  // parse and split by comma
  std::string token;

  std::istringstream ss(m_passAuxDecorKeys);
  while ( std::getline(ss, token, ',') ) {
    m_passKeys.push_back(token);
  }

  ss.clear();
  ss.str(m_failAuxDecorKeys);
  while ( std::getline(ss, token, ',') ) {
    std::cout << token << std::endl;
    m_failKeys.push_back(token);
  }

  if ( m_inContainerName.empty() ) {
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  bool allOK(true);
  if (!m_operatingPt.empty() || m_doBTagCut ) { allOK = false; }
  if (m_operatingPt == "FixedCutBEff_30") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_50") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_60") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_70") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_77") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_80") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_85") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_90") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_30") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_40") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_50") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_60") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_70") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_77") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_85") { allOK = true; }

  if( !allOK ) {
    Error("initialize()", "Requested operating point is not known to xAH. Arrow v Indian? %s", m_operatingPt.c_str());
    return EL::StatusCode::FAILURE;
  }

  if ( m_decorateSelectedObjects ) {
    Info("initialize()"," Decorate Jets with %s", m_decor.c_str());
  }

  //
  // initialize the BJetSelectionTool
  //
  std::string sel_tool_name = std::string("BJetSelectionTool_") + m_name;
  if ( asg::ToolStore::contains<BTaggingSelectionTool>( sel_tool_name ) ) {
    m_BJetSelectTool = asg::ToolStore::get<BTaggingSelectionTool>( sel_tool_name );
  } else {
    m_BJetSelectTool = new BTaggingSelectionTool( sel_tool_name );
  }
  m_BJetSelectTool->msg().setLevel( MSG::INFO ); // DEBUG, VERBOSE, INFO, ERROR

  // if applying cut on nr. bjets, configure it
  //
  if ( m_doBTagCut ) {

    // A few which are not configurable as of yet....
    // is there a reason to have this configurable here??...I think no (GF to self)
    //
    RETURN_CHECK( "JetSelector::initialize()", m_BJetSelectTool->setProperty("MaxEta",m_b_eta_max),"Failed to set property:MaxEta");
    RETURN_CHECK( "JetSelector::initialize()", m_BJetSelectTool->setProperty("MinPt",m_b_pt_min),"Failed to set property:MinPt");
    RETURN_CHECK( "JetSelector::initialize()", m_BJetSelectTool->setProperty("FlvTagCutDefinitionsFileName", m_corrFileName),"Failed to set property:FlvTagCutDefinitionsFileName");

    // configurable parameters
    //
    RETURN_CHECK( "JetSelector::initialize()", m_BJetSelectTool->setProperty("TaggerName",	      m_taggerName),"Failed to set property: TaggerName");
    RETURN_CHECK( "JetSelector::initialize()", m_BJetSelectTool->setProperty("OperatingPoint",      m_operatingPt),"Failed to set property: OperatingPoint");
    RETURN_CHECK( "JetSelector::initialize()", m_BJetSelectTool->setProperty("JetAuthor",	      m_jetAuthor),"Failed to set property: JetAuthor");
    RETURN_CHECK( "JetSelector::initialize()", m_BJetSelectTool->initialize(), "Failed to properly initialize the BJetSelectionTool");

  }

  // initialize the CP::JetJvtEfficiency Tool
  //
  m_JVT_tool_name = "JetJvtEfficiency_effSF";
  std::string JVT_handle_name = "CP::JetJvtEfficiency/" + m_JVT_tool_name +"_"+m_name;
  RETURN_CHECK("JetSelector::initialize()", checkToolStore<CP::JetJvtEfficiency>(m_JVT_tool_name), "" );
  RETURN_CHECK("JetSelector::initialize()", m_JVT_tool_handle.makeNew<CP::JetJvtEfficiency>(JVT_handle_name), "Failed to create handle to CP::JetJvtEfficiency for JVT");
  RETURN_CHECK("JetSelector::initialize()", m_JVT_tool_handle.setProperty("WorkingPoint", m_WorkingPointJVT ),"Failed to set Working Point property of JetJvtEfficiency for JVT");
  RETURN_CHECK("JetSelector::initialize()", m_JVT_tool_handle.initialize(), "Failed to properly initialize CP::JetJvtEfficiency for JVT");

  //  Add the chosen WP to the string labelling the vector<SF> decoration
  //
  m_outputSystNamesJVT = m_outputSystNamesJVT + "_JVT_" + m_WorkingPointJVT;

  if ( m_debug ) {
    CP::SystematicSet affectSystsJVT = m_JVT_tool_handle->affectingSystematics();
    for ( const auto& syst_it : affectSystsJVT ) { Info("initialize()","JetJvtEfficiency tool can be affected by JVT efficiency systematic: %s", (syst_it.name()).c_str()); }
  }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsJVT = m_JVT_tool_handle->recommendedSystematics();
  m_systListJVT = HelperFunctions::getListofSystematics( recSystsJVT, m_systNameJVT, m_systValJVT, m_debug );

  Info("initialize()","Will be using JetJvtEfficiency tool JVT efficiency systematic:");
  for ( const auto& syst_it : m_systListJVT ) {
    if ( m_systNameJVT.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  if(m_debug) Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  if(m_debug) Info("initialize()", "JetSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Jet Selection... %s", m_name.c_str()); }

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  // MC event weight
  float mcEvtWeight(1.0);
  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
    static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
    if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
      Error("execute()  ", "mcEventWeight is not available as decoration! Aborting" );
      return EL::StatusCode::FAILURE;
    }
    mcEvtWeight = mcEvtWeightAcc( *eventInfo );
  }

  m_numEvent++;

  // did any collection pass the cuts?
  bool pass(false);
  bool count(true); // count for the 1st collection in the container - could be better as
                    // shoudl only count for the nominal
  const xAOD::JetContainer* inJets(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  if ( m_inputAlgo.empty() ) {

    // this will be the collection processed - no matter what!!
    RETURN_CHECK("JetSelector::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, m_verbose) ,"");

    pass = executeSelection( inJets, mcEvtWeight, count, m_outContainerName);

  }  else { // get the list of systematics to run over

    // get vector of string giving the names
    std::vector<std::string>* systNames(nullptr);
    RETURN_CHECK("JetSelector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, m_verbose) ,"");

    // loop over systematics
    std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;
    bool passOne(false);
    for ( auto systName : *systNames ) {

      RETURN_CHECK("JetSelector::execute()", HelperFunctions::retrieve(inJets, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");

      passOne = executeSelection( inJets, mcEvtWeight, count, m_outContainerName+systName );
      if ( count ) { count = false; } // only count for 1 collection
      // save the string if passing the selection
      if ( passOne ) {
        vecOutContainerNames->push_back( systName );
      }
      // the final decision - if at least one passes keep going!
      pass = pass || passOne;
    }

    // save list of systs that should be considered down stream
    RETURN_CHECK( "JetSelector::execute()", m_store->record( vecOutContainerNames, m_outputAlgo), "Failed to record vector of output container names.");
    //delete vecOutContainerNames;

  }

  // look what we have in TStore
  if ( m_verbose ) { m_store->print(); }

  if ( !pass ) {
    wk()->skipEvent();
  }

  if ( m_debug ) { Info("execute()", "Leave Jet Selection... "); }

  return EL::StatusCode::SUCCESS;

}

bool JetSelector :: executeSelection ( const xAOD::JetContainer* inJets,
    float mcEvtWeight,
    bool count,
    std::string outContainerName
    )
{
  if ( m_debug ) { Info("executeSelection()", "in executeSelection... %s", m_name.c_str()); }

  // create output container (if requested)
  ConstDataVector<xAOD::JetContainer>* selectedJets(nullptr);
  if ( m_createSelectedContainer ) {
    selectedJets = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  }

  // if doing JVF or JVT get PV location
  if ( m_doJVF ) {
    const xAOD::VertexContainer* vertices(nullptr);
    RETURN_CHECK("JetSelector::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
    m_pvLocation = HelperFunctions::getPrimaryVertexLocation( vertices );
  }


  int nPass(0); int nObj(0);
  bool passEventClean(true);

  static SG::AuxElement::Accessor< char > isCleanAcc("cleanJet");

  //
  // This cannot be static as multiple instance of Jet Selector would
  //   then share the same passSelDecor, including the m_decor name
  //
  SG::AuxElement::Decorator< char > passSelDecor( m_decor );

  for ( auto jet_itr : *inJets ) { // duplicated of basic loop

    // if only looking at a subset of jets make sure all are decorated
    if ( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if ( m_decorateSelectedObjects ) {
        passSelDecor( *jet_itr ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    // All selections but Cleaning
    int passSel = this->PassCuts( jet_itr );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *jet_itr ) = passSel;
    }

    // Cleaning Selection must come after kinematic and JVT selections
    if ( m_cleanJets && passSel && isCleanAcc.isAvailable( *jet_itr ) ) {
      if( !isCleanAcc( *jet_itr ) ) { 
        passSel = false;
        if ( m_decorateSelectedObjects )
          passSelDecor( *jet_itr ) = passSel;

        // If any of the passing jets fail the recommendation is to remove the jet (and MET is wrong)
        // If any of the N leading jets are not clean the event should be removed
        if( m_cleanEvent || nObj <= m_cleanEvtLeadJets ){
          passEventClean = false; 
          if (m_debug) Info("executeSelection()", "Remove event due to bad jet with pt %f", jet_itr->pt() );
        }// if cleaning the event 

      }// if jet is not clean
    }// if jet clean aux missing
    if( m_useCutFlow && passSel )
      m_jet_cutflowHist_1->Fill( m_jet_cutflow_cleaning_cut, 1 );



    if ( passSel ) {
      if ( m_debug ) { Info("executeSelection()", "passSel"); }
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedJets->push_back( jet_itr );
      }
    }
  }

  // Loop over selected jets and decorate with JVT efficiency SF
  // Do it only for MC
  //
  if ( m_isMC && m_doJVT ) {
    
    std::vector< std::string >* sysVariationNamesJVT  = new std::vector< std::string >;

    // Do it only if a tool with *this* name hasn't already been used
    //
    if ( !isToolAlreadyUsed(m_JVT_tool_name) ) {

      for ( const auto& syst_it : m_systListJVT ) {

	// Create the name of the SF weight to be recorded
        //   template:  SYSNAME_JVTEff_SF
        //
        std::string sfName = "JVTEff_SF_" + m_WorkingPointJVT;;
        if ( !syst_it.name().empty() ) {
           std::string prepend = syst_it.name() + "_";
           sfName.insert( 0, prepend );
        }
        if ( m_debug ) { Info("executeSelection()", "JVT SF sys name (to be recorded in xAOD::TStore) is: %s", sfName.c_str()); }
        sysVariationNamesJVT->push_back(sfName);

        // apply syst
        //
        if ( m_JVT_tool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
          Error("executeSelection()", "Failed to configure CP::JetJvtEfficiency for systematic %s", syst_it.name().c_str());
          return EL::StatusCode::FAILURE;
        }
        if ( m_debug ) { Info("executeSelection()", "Successfully applied systematic: %s", syst_it.name().c_str()); }

        // and now apply JVT SF!
        //
        unsigned int idx(0);
        for ( auto jet : *(selectedJets) ) {

           if ( m_debug ) { Info( "executeSelection()", "Applying JVT SF" ); }

           // obtain JVT SF as a float (to be stored away separately)
           //
           //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* jet)
           //
           SG::AuxElement::Decorator< std::vector<float> > sfVecJVT( m_outputSystNamesJVT );
           if ( !sfVecJVT.isAvailable( *jet ) ) {
             sfVecJVT( *jet ) = std::vector<float>();
           }

           float jvtSF(1.0);
	   if ( jet->pt() < m_pt_max_JVT && fabs(jet->eta()) < m_eta_max_JVT ) {
             if ( m_JVT_tool_handle->getEfficiencyScaleFactor( *jet, jvtSF ) != CP::CorrectionCode::Ok ) {
               Warning( "executeSelection()", "Problem in JVT Tool getEfficiencyScaleFactor");
               jvtSF = 1.0;
             }
	   }
           //
           // Add it to decoration vector
           //
           sfVecJVT( *jet ).push_back( jvtSF );

           if ( m_debug ) {
             Info( "executeSelection()", "===>>>");
             Info( "executeSelection()", " ");
             Info( "executeSelection()", "Jet %i, pt = %.2f GeV, |eta| = %.2f", idx, (jet->pt() * 1e-3), fabs(jet->eta()) );
             Info( "executeSelection()", " ");
             Info( "executeSelection()", "JVT SF decoration: %s", m_outputSystNamesJVT.c_str() );
             Info( "executeSelection()", " ");
             Info( "executeSelection()", "Systematic: %s", syst_it.name().c_str() );
             Info( "executeSelection()", " ");
             Info( "executeSelection()", "JVT SF:");
             Info( "executeSelection()", "\t %f (from getEfficiencyScaleFactor())", jvtSF );
             Info( "executeSelection()", "--------------------------------------");
           }

           ++idx;
	}

      }

    }

    // Add list of JVT systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSelection() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesJVT) ) { RETURN_CHECK( "JetSelector::executeSelection()", m_store->record( sysVariationNamesJVT, m_outputSystNamesJVT), "Failed to record vector of systematic names for JVT efficiency SF" ); }

  }

  // add ConstDataVector to TStore
  if ( m_createSelectedContainer ) {
    RETURN_CHECK("JetSelector::executeSelection()", m_store->record( selectedJets, outContainerName ), "Failed to store const data container.");
  }

  if ( count ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  if ( !passEventClean ) { return false; }
  if ( m_pass_min > 0 && nPass < m_pass_min ) {
    return false;
  }
  if ( m_pass_max > 0 && nPass > m_pass_max ) {
    return false;
  }

  if ( count ) {
    m_numEventPass++;
    m_weightNumEventPass += mcEvtWeight;
  }

  if ( m_debug ) { Info("executeSelection()", "leave executeSelection... "); }
  return true;
}


EL::StatusCode JetSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: finalize ()
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

  if(m_debug) Info("finalize()", "%s", m_name.c_str());

  if ( m_useCutFlow ) {
    if(m_debug) Info("histFinalize()", "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

  if ( m_BJetSelectTool ) { m_BJetSelectTool = nullptr; delete m_BJetSelectTool; }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: histFinalize ()
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

  if(m_debug) Info("histFinalize()", "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}

int JetSelector :: PassCuts( const xAOD::Jet* jet ) {
  if ( m_debug ) { Info("PassCuts()", "In pass cuts"); }

  // fill cutflow bin 'all' before any cut
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_all, 1 );

  // pT
  if ( m_pT_max != 1e8 ) {
    if ( jet->pt() > m_pT_max ) { return 0; }
  }
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_ptmax_cut, 1 );

  if ( m_pT_min != 1e8 ) {
    if ( jet->pt() < m_pT_min ) { return 0; }
  }
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_ptmin_cut, 1 );

  // eta
  if ( m_eta_max != 1e8 ) {
    if ( fabs(jet->eta()) > m_eta_max ) { return 0; }
  }
  if ( m_eta_min != 1e8 ) {
    if ( fabs(jet->eta()) < m_eta_min ) { return 0; }
  }
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_eta_cut, 1 );

  // detEta
  if ( m_detEta_max != 1e8 ) {
    if ( fabs( ( jet->getAttribute<xAOD::JetFourMom_t>(m_jetScaleType.c_str()) ).eta() ) > m_detEta_max ) { return 0; }
  }
  if ( m_detEta_min != 1e8 ) {
    if ( fabs( ( jet->getAttribute<xAOD::JetFourMom_t>(m_jetScaleType.c_str()) ).eta() ) < m_detEta_min ) { return 0; }
  }

  // mass
  if ( m_mass_max != 1e8 ) {
    if ( jet->m() > m_mass_max ) { return 0; }
  }
  if ( m_mass_min != 1e8 ) {
    if ( jet->m() < m_mass_min ) { return 0; }
  }

  // rapidity
  if ( m_rapidity_max != 1e8 ) {
    if ( jet->rapidity() > m_rapidity_max ) { return 0; }
  }
  if ( m_rapidity_min != 1e8 ) {
    if ( jet->rapidity() < m_rapidity_min ) { return 0; }
  }

  // JVF pileup cut
  if ( m_doJVF ){
    if ( m_debug ) { Info("PassCuts()", "Doing JVF"); }
    if ( m_debug ) { Info("PassCuts()", "Jet Pt %f" , jet->pt()); }
    if ( jet->pt() < m_pt_max_JVF ) {
      xAOD::JetFourMom_t jetScaleP4 = jet->getAttribute< xAOD::JetFourMom_t >( m_jetScaleType.c_str() );
      if ( fabs(jetScaleP4.eta()) < m_eta_max_JVF ){
        if ( jet->getAttribute< std::vector<float> >( "JVF" ).at( m_pvLocation ) < m_JVFCut ) {
          return 0;
        }
      }
    }
  } // m_doJVF

  // JVT pileup cut
  //
  if ( m_doJVT ) {
    if ( m_debug ) { Info("PassCuts()", "Checking JVT cut..."); }
    if ( m_debug ) {
      if ( m_JVTCut > 0 && jet->getAttribute< float >( "Jvt" ) < m_JVTCut ) { Info("passCuts()", " pt/eta = %2f/%2f ", jet->pt() , jet->eta() ); }
    }

    if ( jet->pt() < m_pt_max_JVT ) {
      if ( m_debug ) { Info("PassCuts()", "Pass JVT-pT Cut"); }
      xAOD::JetFourMom_t jetScaleP4 = jet->getAttribute< xAOD::JetFourMom_t >( m_jetScaleType.c_str() );
      if ( fabs(jetScaleP4.eta()) < m_eta_max_JVT ){
	if ( m_debug ) Info("passCuts()", " Pass JVT-Eta Cut " );

	// Old usage: check manually whether this jet passes JVT cut
	//
	if ( m_JVTCut > 0 ) {
	  if ( m_debug ) { Info("passCuts()", " JVT = %2f ", jet->getAttribute< float >( "Jvt" ) ); }
          if ( jet->getAttribute< float >( "Jvt" ) < m_JVTCut ) {
	    if ( m_debug ) { Info("passCuts()", " upper JVTCut is %2f - cutting this jet!!", m_JVTCut ); }
            return 0;
          } else {
	    if ( m_debug ) { Info("passCuts()", " upper JVTCut is %2f - jet passes JVT ", m_JVTCut ); }
	  }
	} else {
  	  if ( !m_JVT_tool_handle->passesJvtCut(*jet) ) { return 0; }
	}

      }
    }
  } // m_doJVT
  if ( m_useCutFlow ) m_jet_cutflowHist_1->Fill( m_jet_cutflow_jvt_cut, 1 );

  //
  //  BTagging
  //
  if ( m_doBTagCut ) {
    if ( m_debug ) { Info("PassCuts()", "Doing BTagging"); }
    if ( m_BJetSelectTool->accept( jet ) ) {
      if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_btag_cut, 1 );
    } else {
      return 0;
    }
  }


  //
  //  HLT BTagging
  //
  if ( m_doHLTBTagCut ) {
    const xAOD::BTagging *btag_info = jet->auxdata< const xAOD::BTagging* >("HLTBTag");

    double tagValue = -99;
    if(m_HLTBTagTaggerName=="MV2c20"){
      btag_info->MVx_discriminant("MV2c20", tagValue);
    }

    if(m_HLTBTagTaggerName=="COMB"){
      float wIP3D = btag_info->IP3D_loglikelihoodratio();
      float wSV1  = btag_info->SV1_loglikelihoodratio();
      tagValue = wIP3D + wSV1;
    }

    if(tagValue < m_HLTBTagCutValue){return 0;}
  }

  //
  // HLT Valid Vtx
  //
  if ( m_requireHLTVtx ) {
    const xAOD::Vertex *online_pvx   = jet->auxdata<const xAOD::Vertex*>("HLTBJetTracks_vtx");
    if(!online_pvx) {return 0;}
  }

  if ( m_requireNoHLTVtx ) {
    const xAOD::Vertex *online_pvx   = jet->auxdata<const xAOD::Vertex*>("HLTBJetTracks_vtx");
    if(online_pvx) {return 0;}
  }


  //
  //  Pass Keys
  //
  for ( auto& passKey : m_passKeys ) {
    if ( !(jet->auxdata< char >(passKey) == '1') ) { return 0;}
  }

  //
  //  Fail Keys
  //
  for ( auto& failKey : m_failKeys ){
    if ( !(jet->auxdata< char >(failKey) == '0') ) { return 0;}
  }

  //
  //  Truth Label
  //
  if ( m_truthLabel != -1 ) {
    if ( m_debug ) { Info("PassCuts()", "Doing Truth Label"); }
    int this_TruthLabel = 0;

    static SG::AuxElement::ConstAccessor<int> HadronConeExclTruthLabelID ("HadronConeExclTruthLabelID");
    static SG::AuxElement::ConstAccessor<int> TruthLabelID ("TruthLabelID");
    static SG::AuxElement::ConstAccessor<int> PartonTruthLabelID ("PartonTruthLabelID");

    if( m_useHadronConeExcl && HadronConeExclTruthLabelID.isAvailable( *jet) ){
      this_TruthLabel = HadronConeExclTruthLabelID(( *jet) );
    } else if ( TruthLabelID.isAvailable( *jet) ) {
      this_TruthLabel = TruthLabelID( *jet );
      if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
    } else {
      this_TruthLabel = PartonTruthLabelID( *jet );
      if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
    }

    if ( this_TruthLabel == -1 ) {return 0;}
    if ( (m_truthLabel == 5) && this_TruthLabel != 5 ) { return 0;}
    if ( (m_truthLabel == 4) && this_TruthLabel != 4 ) { return 0;}
    if ( (m_truthLabel == 0) && this_TruthLabel != 0 ) { return 0;}

  }

  if ( m_debug ) { Info("PassCuts()", "Passed Cuts"); }
  return 1;
}


