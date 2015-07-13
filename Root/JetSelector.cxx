/******************************************
 *
 * Jet selector tool
 *
 * G.Facini (gabriel.facini@cern.ch), M. Milesi (marco.milesi@cern.ch)
 * J.Alison (john.alison@cern.ch)
 *
 ******************************************/

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
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetSelector)


JetSelector :: JetSelector () :
  m_cutflowHist(nullptr),
  m_cutflowHistW(nullptr),
  m_jet_cutflowHist_1(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("JetSelector()", "Calling constructor");

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
  m_cleanEvtLeadJets        = 0; // indepedent of previous switch
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

  m_doJVF 		    = false;
  m_doBTagCut 		    = false;
  m_pt_max_JVF 	            = 50e3;
  m_eta_max_JVF 	    = 2.4;
  m_JVFCut 		    = 0.5;
  m_doJVT 		    = false;
  m_pt_max_JVT 	            = 50e3;
  m_eta_max_JVT 	    = 2.4;
  m_JVTCut 		    = 0.64;

  // Btag quality
  m_btag_veryloose          = false;
  m_btag_loose              = false;
  m_btag_medium             = false;
  m_btag_tight              = false;

  m_passAuxDecorKeys        = "";
  m_failAuxDecorKeys        = "";

}

EL::StatusCode  JetSelector :: configure ()
{
  if(!getConfig().empty()){
    Info("configure()", "Configuing JetSelector Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug         = config->GetValue("Debug" ,      m_debug);
    m_useCutFlow    = config->GetValue("UseCutFlow",  m_useCutFlow);

    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());
    m_jetScaleType            = config->GetValue("JetScaleType",  m_jetScaleType.c_str());
    //If not set, find default from input container name
    if (m_jetScaleType.size() == 0){
      if( m_inContainerName.find("EMTopo") != std::string::npos){
        m_jetScaleType = "JetEMScaleMomentum";
      }else{
        m_jetScaleType = "JetConstitScaleMomentum";
      }
    }

    // name of algo input container comes from - only if running on syst
    m_inputAlgo               = config->GetValue("InputAlgo",   m_inputAlgo.c_str());
    m_outputAlgo              = config->GetValue("OutputAlgo",  m_outputAlgo.c_str());

    // decorate selected objects that pass the cuts
    m_decorateSelectedObjects = config->GetValue("DecorateSelectedObjects", m_decorateSelectedObjects);
    // additional functionality : create output container of selected objects
    //                            using the SG::VIEW_ELEMENTS option
    //                            decorating and output container should not be mutually exclusive
    m_createSelectedContainer = config->GetValue("CreateSelectedContainer", m_createSelectedContainer);
    // if requested, a new container is made using the SG::VIEW_ELEMENTS option
    m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());
    // if only want to look at a subset of object
    m_nToProcess              = config->GetValue("NToProcess", m_nToProcess);

    // cuts
    m_cleanJets               = config->GetValue("CleanJets",  m_cleanJets);
    m_cleanEvtLeadJets        = config->GetValue("CleanEventWithLeadJets", m_cleanEvtLeadJets); // indepedent of previous switch
    m_pass_max                = config->GetValue("PassMax",      m_pass_max);
    m_pass_min                = config->GetValue("PassMin",      m_pass_min);
    m_pT_max                  = config->GetValue("pTMax",       m_pT_max);
    m_pT_min                  = config->GetValue("pTMin",       m_pT_min);
    m_eta_max                 = config->GetValue("etaMax",      m_eta_max);
    m_eta_min                 = config->GetValue("etaMin",      m_eta_min);
    m_detEta_max              = config->GetValue("detEtaMax",   m_detEta_max);
    m_detEta_min              = config->GetValue("detEtaMin",   m_detEta_min);
    m_mass_max                = config->GetValue("massMax",     m_mass_max);
    m_mass_min                = config->GetValue("massMin",     m_mass_min);
    m_rapidity_max            = config->GetValue("rapidityMax", m_rapidity_max);
    m_rapidity_min            = config->GetValue("rapidityMin", m_rapidity_min);
    m_truthLabel 	      = config->GetValue("TruthLabel",   m_truthLabel);

    m_doJVF 		      = config->GetValue("DoJVF",       m_doJVF);
    m_doBTagCut		      = config->GetValue("DoBTagCut",   m_doBTagCut);
    m_pt_max_JVF 	      = config->GetValue("pTMaxJVF",    m_pt_max_JVF);
    m_eta_max_JVF 	      = config->GetValue("etaMaxJVF",   m_eta_max_JVF);
    m_JVFCut 		      = config->GetValue("JVFCut",      m_JVFCut);

    m_doJVT 		      = config->GetValue("DoJVT",       m_doJVT);
    m_pt_max_JVT 	      = config->GetValue("pTMaxJVT",    m_pt_max_JVT);
    m_eta_max_JVT 	      = config->GetValue("etaMaxJVT",   m_eta_max_JVT);
    m_JVTCut 		      = config->GetValue("JVTCut",      m_JVTCut);

    // Btag quality
    m_btag_veryloose          = config->GetValue("BTagVeryLoose",   m_btag_veryloose);
    m_btag_loose              = config->GetValue("BTagLoose",       m_btag_loose);
    m_btag_medium             = config->GetValue("BTagMedium",      m_btag_medium);
    m_btag_tight              = config->GetValue("BTagTight",       m_btag_tight);

    m_passAuxDecorKeys        = config->GetValue("PassDecorKeys", m_passAuxDecorKeys.c_str());
    m_failAuxDecorKeys        = config->GetValue("FailDecorKeys", m_failAuxDecorKeys.c_str());

    config->Print();
    Info("configure()", "JetSelector Interface succesfully configured! ");

    delete config; config = nullptr;
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
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  //
  // Set the Btagging cut
  //
  m_btagCut = -100; // < -99 turns off btagging
  m_decor   = "passSel";
  //
  // MV2c20 cuts
  //
  // VeryLoose / 85% / -0.7682
  // Loose     / 77% / -0.3867
  // Medium    / 70% / 0.0314
  // Tight     / 60% / 0.5102

  //if ( m_isEMjet ) {

  m_btag_veryloose_cut = -0.7682;
  m_btag_loose_cut     = -0.3867;
  m_btag_medium_cut    =  0.0314;
  m_btag_tight_cut     =  0.5102;
  if ( m_btag_veryloose ) { m_btagCut = m_btag_veryloose_cut; }
  if ( m_btag_loose     ) { m_btagCut = m_btag_loose_cut;     }
  if ( m_btag_medium    ) { m_btagCut = m_btag_medium_cut;    }
  if ( m_btag_tight     ) { m_btagCut = m_btag_tight_cut;     }

  //} else if ( m_isLCjet ) {
  //if ( m_btag_veryloose ) { m_btagCut = 0.1340; }
  //if ( m_btag_loose     ) { m_btagCut = 0.3511; }
  //if ( m_btag_medium    ) { m_btagCut = 0.7892; }
  //if ( m_btag_tight     ) { m_btagCut = 0.9827; }
  //}

  if ( m_decorateSelectedObjects ) {
    Info(m_name.c_str()," Decorate Jets with %s", m_decor.c_str());
  }

  return EL::StatusCode::SUCCESS;
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

  Info("setupJob()", "Calling setupJob");

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

  Info("histInitialize()", "Calling histInitialize");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput");

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
  Info("initialize()", "Calling initialize");
  
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
    m_jet_cutflow_cleaning_cut    = m_jet_cutflowHist_1->GetXaxis()->FindBin("cleaning_cut");     
    m_jet_cutflow_ptmax_cut       = m_jet_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");
    m_jet_cutflow_ptmin_cut       = m_jet_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");      
    m_jet_cutflow_eta_cut         = m_jet_cutflowHist_1->GetXaxis()->FindBin("eta_cut"); 
    m_jet_cutflow_jvt_cut         = m_jet_cutflowHist_1->GetXaxis()->FindBin("JVT_cut");
    m_jet_cutflow_btag_cut        = m_jet_cutflowHist_1->GetXaxis()->FindBin("BTag_cut");
 
  }

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  Info("initialize()", "JetSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Jet Selection... "); }

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

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

  }
  else { // get the list of systematics to run over

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

    // save list of systs that shoudl be considered down stream
    RETURN_CHECK( "JetSelector::execute()", m_store->record( vecOutContainerNames, m_outputAlgo), "Failed to record vector of output container names.");
    //delete vecOutContainerNames;

  }

  // look what do we have in TStore
  if ( m_verbose ) { m_store->print(); }

  if ( !pass ) {
    wk()->skipEvent();
  }

  return EL::StatusCode::SUCCESS;

}

bool JetSelector :: executeSelection ( const xAOD::JetContainer* inJets,
    float mcEvtWeight,
    bool count,
    std::string outContainerName
    )
{

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
  static SG::AuxElement::Decorator< char > passSelDecor( m_decor );

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
    int passSel = this->PassCuts( jet_itr );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *jet_itr ) = passSel;
    }

    // event level cut if any of the N leading jets are not clean
    if ( m_cleanEvtLeadJets > 0 && nObj <= m_cleanEvtLeadJets ) {
      if ( isCleanAcc.isAvailable( *jet_itr ) ) {
        if( !isCleanAcc( *jet_itr ) ) { passEventClean = false; }
      }
    }

    if ( passSel ) {
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedJets->push_back( jet_itr );
      }
    }
  }

  if ( count ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  // add ConstDataVector to TStore
  if ( m_createSelectedContainer ) {
    RETURN_CHECK("JetSelector::execute()", m_store->record( selectedJets, outContainerName ), "Failed to store const data container.");
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

  Info("finalize()", "%s", m_name.c_str());
  if ( m_useCutFlow ) {
    Info("histFinalize()", "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

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

  Info("histFinalize()", "Calling histFinalize");

  return EL::StatusCode::SUCCESS;
}

int JetSelector :: PassCuts( const xAOD::Jet* jet ) {

  // fill cutflow bin 'all' before any cut
  m_jet_cutflowHist_1->Fill( m_jet_cutflow_all, 1 );

  // clean jets
  static SG::AuxElement::Accessor< char > isCleanAcc("cleanJet");
  if ( m_cleanJets ) {
    if ( isCleanAcc.isAvailable( *jet ) ) {
      if ( !isCleanAcc( *jet ) ) { return 0; }
    }
  }
  m_jet_cutflowHist_1->Fill( m_jet_cutflow_cleaning_cut, 1 );        

  // pT
  if ( m_pT_max != 1e8 ) {
    if ( jet->pt() > m_pT_max ) { return 0; }
  }
  m_jet_cutflowHist_1->Fill( m_jet_cutflow_ptmax_cut, 1 );        
  
  if ( m_pT_min != 1e8 ) {
    if ( jet->pt() < m_pT_min ) { return 0; }
  }
  m_jet_cutflowHist_1->Fill( m_jet_cutflow_ptmin_cut, 1 );        

  // eta
  if ( m_eta_max != 1e8 ) {
    if ( fabs(jet->eta()) > m_eta_max ) { return 0; }
  }
  if ( m_eta_min != 1e8 ) {
    if ( fabs(jet->eta()) < m_eta_min ) { return 0; }
  }
  m_jet_cutflowHist_1->Fill( m_jet_cutflow_eta_cut, 1 );        

  // detEta
  if ( m_detEta_max != 1e8 ) {
    if ( ( jet->getAttribute<xAOD::JetFourMom_t>(m_jetScaleType.c_str()) ).eta() > m_detEta_max ) { return 0; }
  }
  if ( m_detEta_min != 1e8 ) {
    if( ( jet->getAttribute<xAOD::JetFourMom_t>(m_jetScaleType.c_str()) ).eta() < m_detEta_min ) { return 0; }
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

    if ( m_debug ) {
      if ( jet->getAttribute< float >( "Jvt" ) < m_JVTCut ) { Info("passCuts()", " pt/eta = %2f/%2f ", jet->pt() , jet->eta() ); }
    }

    if ( jet->pt() < m_pt_max_JVT ) {
      xAOD::JetFourMom_t jetScaleP4 = jet->getAttribute< xAOD::JetFourMom_t >( m_jetScaleType.c_str() );
      if ( fabs(jetScaleP4.eta()) < m_eta_max_JVT ){
        if ( m_debug ) { Info("passCuts()", " JVT = %2f ", jet->getAttribute< float >( "Jvt" ) ); }
        if ( jet->getAttribute< float >( "Jvt" ) < m_JVTCut ) {
	  if ( m_debug ) { Info("passCuts()", " upper JVTCut is %2f - cutting this jet!!", m_JVTCut ); }
          return 0;
        }
      }
    }
  } // m_doJVT
  m_jet_cutflowHist_1->Fill( m_jet_cutflow_jvt_cut, 1 );        

  //
  //  BTagging
  //
  if ( m_btagCut >= -99 ) {

    //
    //  Decorate All Jets with default BTag decisions
    //
    jet->auxdecor<char>("BTagVeryLoose") = -1;
    jet->auxdecor<char>("BTagLoose")     = -1;
    jet->auxdecor<char>("BTagMedium")    = -1;
    jet->auxdecor<char>("BTagTight")     = -1;

    const xAOD::BTagging *myBTag = jet->btagging();
    if ( myBTag ) {

      //
      // Cut on Btagging weight
      //
      double discriminant = -99;
      myBTag->MVx_discriminant("MV2c20",discriminant);
      if ( m_doBTagCut && (discriminant < m_btagCut) ) {
	return 0;
      }
      m_jet_cutflowHist_1->Fill( m_jet_cutflow_btag_cut, 1 );        

      //
      // Decorate the Jets
      //
      jet->auxdecor<char>("BTagVeryLoose") = static_cast<char>( discriminant > m_btag_veryloose_cut );
      jet->auxdecor<char>("BTagLoose")     = static_cast<char>( discriminant > m_btag_loose_cut     );
      jet->auxdecor<char>("BTagMedium")    = static_cast<char>( discriminant > m_btag_medium_cut    );
      jet->auxdecor<char>("BTagTight")     = static_cast<char>( discriminant > m_btag_tight_cut     );

    }
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

    int this_TruthLabel = 0;
    static SG::AuxElement::ConstAccessor<int> TruthLabelID ("TruthLabelID");
    if ( TruthLabelID.isAvailable( *jet) ) {
      this_TruthLabel = TruthLabelID( *jet );
    } else {
      static SG::AuxElement::ConstAccessor<int> PartonTruthLabelID ("PartonTruthLabelID");
      this_TruthLabel = PartonTruthLabelID( *jet );
    }

    if ( (m_truthLabel == 5) && this_TruthLabel != 5 ) { return 0;}
    if ( (m_truthLabel == 4) && this_TruthLabel != 4 ) { return 0;}
    if ( (m_truthLabel == 0) && !(this_TruthLabel == 21 || this_TruthLabel<4) ) { return 0;}

  }

  return 1;
}


