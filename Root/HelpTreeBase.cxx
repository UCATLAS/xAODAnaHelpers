// c++ include(s):
#include <iostream>

// EDM include(s):
#include "xAODBTagging/BTagging.h"
#include "xAODTracking/TrackParticle.h"
#include "xAODTracking/TrackSummaryAccessors_v1.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODJet/JetConstituentVector.h"
#include "xAODCaloEvent/CaloClusterContainer.h"
#include "xAODPrimitives/IsolationType.h"


#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelpTreeBase.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>


#include "AsgTools/StatusCode.h"

using std::vector;

// needed? should it be here?
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

HelpTreeBase::HelpTreeBase(xAOD::TEvent* event, TTree* tree, TFile* file, const float units, bool debug, bool DC14, xAOD::TStore* store):
  m_eventInfoSwitch(nullptr),
  m_trigInfoSwitch(nullptr),
  m_muInfoSwitch(nullptr),
  m_elInfoSwitch(nullptr),
  m_phInfoSwitch(nullptr),
  m_truthInfoSwitch(nullptr),
  m_fatJetInfoSwitch(nullptr),
  m_tauInfoSwitch(nullptr),
  m_metInfoSwitch(nullptr),
  m_trigConfTool(nullptr),
  m_trigDecTool(nullptr)
{

  m_units = units;
  m_debug = debug;
  m_DC14  = DC14;
  m_tree = tree;
  m_tree->SetDirectory( file );
  m_event = event;
  m_store = store;
  Info("HelpTreeBase()", "HelpTreeBase setup");

  // turn things off it this is data...since TStore is not a needed input
  // default isMC to true so more is added to the tree than less
  const xAOD::EventInfo* eventInfo(nullptr);
  HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, false);
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

}

HelpTreeBase::~HelpTreeBase() {

    //delete all the info switches that have been built earlier on

    //event
    delete m_eventInfoSwitch;

    //trig
    delete m_trigInfoSwitch;

    //mu
    delete m_muInfoSwitch;

    //el
    delete m_elInfoSwitch;

    //ph
    delete m_phInfoSwitch;

    //truth
    delete m_truthInfoSwitch;

    //fatjet
    delete m_fatJetInfoSwitch;

    //tau
    delete m_tauInfoSwitch;

    //met
    delete m_metInfoSwitch;

    //jet

    for (auto jet: m_jets)
      delete jet.second;

}


HelpTreeBase::HelpTreeBase(TTree* tree, TFile* file, xAOD::TEvent* event, xAOD::TStore* store, const float units, bool debug, bool DC14):
  HelpTreeBase(event, tree, file, units, debug, DC14, store)
{
  // use the other constructor for everything
}


void HelpTreeBase::Fill() {
  m_tree->Fill();
}

/*********************
 *
 *   EVENT
 *
 ********************/

void HelpTreeBase::AddEvent( const std::string detailStr ) {

  if(m_debug)  Info("AddEvent()", "Adding event variables: %s", detailStr.c_str());

  m_eventInfoSwitch = new HelperClasses::EventInfoSwitch( detailStr );

  // always
  m_tree->Branch("runNumber",          &m_runNumber,      "runNumber/I");
  m_tree->Branch("eventNumber",        &m_eventNumber,    "eventNumber/LI");
  m_tree->Branch("lumiBlock",          &m_lumiBlock,      "lumiBlock/I");
  m_tree->Branch("coreFlags",          &m_coreFlags,      "coreFlags/i");
  if( m_isMC ) {
    m_tree->Branch("mcEventNumber",      &m_mcEventNumber,  "mcEventNumber/I");
    m_tree->Branch("mcChannelNumber",    &m_mcChannelNumber,"mcChannelNumber/I");
    m_tree->Branch("mcEventWeight",      &m_mcEventWeight,  "mcEventWeight/F");
  } else {
    m_tree->Branch("bcid",               &m_bcid,           "bcid/I");
  }

  if ( m_eventInfoSwitch->m_eventCleaning ) {
    m_tree->Branch("timeStamp",          &m_timeStamp,         "timeStamp/i");
    m_tree->Branch("timeStampNSOffset",  &m_timeStampNSOffset, "timeStampNSOffset/i");
    m_tree->Branch("TileError",          &m_TileError,         "TileError/O");
    m_tree->Branch("SCTError",           &m_SCTError,          "SCTError/O");
    m_tree->Branch("LArError",           &m_LArError,          "LArError/O");
    m_tree->Branch("TileFlags",          &m_TileFlags,         "TileFlags/i");
    m_tree->Branch("SCTFlags",           &m_SCTFlags,          "SCTFlags/i");
    m_tree->Branch("LArFlags",           &m_LArFlags,          "LArFlags/i");
  }

  if ( m_eventInfoSwitch->m_pileup ) {
    m_tree->Branch("weight_pileup",      &m_weight_pileup,  "weight_pileup/F");
    m_tree->Branch("NPV",                &m_npv,            "NPV/I");
    m_tree->Branch("actualInteractionsPerCrossing",  &m_actualMu,  "actualInteractionsPerCrossing/F");
    m_tree->Branch("averageInteractionsPerCrossing", &m_averageMu, "averageInteractionsPerCrossing/F");
  }

  if ( m_eventInfoSwitch->m_shapeEM ) {
    m_tree->Branch("rhoEM",                &m_rhoEM,            "rhoEM/D");
  }

  if ( m_eventInfoSwitch->m_shapeLC ) {
    m_tree->Branch("rhoLC",                &m_rhoLC,            "rhoLC/D");
  }

  if( m_eventInfoSwitch->m_truth && m_isMC ) {
    m_tree->Branch("pdgId1",            &m_pdgId1,        "pdgId1/I" );
    m_tree->Branch("pdgId2",            &m_pdgId2,        "pdgId2/I" );
    m_tree->Branch("pdfId1",            &m_pdfId1,        "pdfId1/I" );
    m_tree->Branch("pdfId2",            &m_pdfId2,        "pdfId2/I" );
    m_tree->Branch("x1",                &m_x1,            "x1/F"  );
    m_tree->Branch("x2",                &m_x2,            "x2/F"  );
    //m_tree->Branch("scale",             &m_scale,         "scale/F");
    //m_tree->Branch("q",                 &m_q,             "q/F");
    //m_tree->Branch("pdf1",              &m_pdf1,          "pdf1/F");
    //m_tree->Branch("pdf2",              &m_pdf2,          "pdf2/F");
    m_tree->Branch("xf1",               &m_xf1,           "xf1/F");
    m_tree->Branch("xf2",               &m_xf2,           "xf2/F");
  }

  if ( m_eventInfoSwitch->m_caloClus ) {
    m_tree->Branch("caloCluster_pt",  &m_caloCluster_pt);
    m_tree->Branch("caloCluster_phi", &m_caloCluster_phi);
    m_tree->Branch("caloCluster_eta", &m_caloCluster_eta);
    m_tree->Branch("caloCluster_e",   &m_caloCluster_e);
  }

  this->AddEventUser();
}

void HelpTreeBase::FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* /*event*/ ) {

  this->ClearEvent();
  this->ClearEventUser();

  m_runNumber             = eventInfo->runNumber();
  m_eventNumber           = eventInfo->eventNumber();
  m_lumiBlock             = eventInfo->lumiBlock();
  m_coreFlags             = eventInfo->eventFlags(xAOD::EventInfo::Core);
  if ( m_isMC ) {
    m_mcEventNumber         = eventInfo->mcEventNumber();
    m_mcChannelNumber       = eventInfo->mcChannelNumber();
    m_mcEventWeight         = eventInfo->mcEventWeight();
  } else {
    m_bcid                  = eventInfo->bcid();
  }

  if ( m_eventInfoSwitch->m_eventCleaning ) {

    if ( eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) m_LArError = true;
    else m_LArError = false;
    m_LArFlags = eventInfo->eventFlags(xAOD::EventInfo::LAr);

    if ( eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) m_TileError = true;
    else m_TileError = false;
    m_TileFlags = eventInfo->eventFlags(xAOD::EventInfo::Tile);

    if ( eventInfo->errorState(xAOD::EventInfo::SCT)==xAOD::EventInfo::Error ) m_SCTError = true;
    else m_SCTError = false;
    m_SCTFlags = eventInfo->eventFlags(xAOD::EventInfo::SCT);

    m_timeStamp = eventInfo->timeStamp();
    m_timeStampNSOffset = eventInfo->timeStampNSOffset();

  }

  if ( m_eventInfoSwitch->m_pileup ) {

    if ( m_event ) {
      const xAOD::VertexContainer* vertices(nullptr);
      HelperFunctions::retrieve( vertices, "PrimaryVertices", m_event, 0 );
      m_npv = HelperFunctions::countPrimaryVertices(vertices, 2);
    } else {
      m_npv = -1;
    }

    m_actualMu  = eventInfo->actualInteractionsPerCrossing();
    m_averageMu = eventInfo->averageInteractionsPerCrossing();

    if ( m_isMC ) {

      static SG::AuxElement::ConstAccessor< float > weight_pileup ("PileupWeight");
      static SG::AuxElement::ConstAccessor< float >  correct_mu("corrected_averageInteractionsPerCrossing");
      static SG::AuxElement::ConstAccessor< unsigned int > rand_run_nr("RandomRunNumber");
      static SG::AuxElement::ConstAccessor< unsigned int > rand_lumiblock_nr("RandomLumiBlockNumber");

      if ( weight_pileup.isAvailable( *eventInfo ) )	 { m_weight_pileup = weight_pileup( *eventInfo ); }	    else { m_weight_pileup = 1.0; }
      if ( correct_mu.isAvailable( *eventInfo ) )	 { m_correct_mu = correct_mu( *eventInfo ); }		    else { m_correct_mu = -1.0; }
      if ( rand_run_nr.isAvailable( *eventInfo ) )	 { m_rand_run_nr = rand_run_nr( *eventInfo ); } 	    else { m_rand_run_nr = 900000; }
      if ( rand_lumiblock_nr.isAvailable( *eventInfo ) ) { m_rand_lumiblock_nr = rand_lumiblock_nr( *eventInfo ); } else { m_rand_lumiblock_nr = 0; }

    }

  }

  if ( m_eventInfoSwitch->m_shapeLC && m_event ) {
    const xAOD::EventShape* evtShape(nullptr);
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", m_event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoLC ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoLC = -999;
    }
  }

  if ( m_eventInfoSwitch->m_shapeEM && m_event ) {
    const xAOD::EventShape* evtShape(nullptr);
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", m_event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoEM ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoEM = -999;
    }
  }

  if( m_eventInfoSwitch->m_caloClus && m_event ) {
    const xAOD::CaloClusterContainer* caloClusters = 0;
    HelperFunctions::retrieve( caloClusters, "CaloCalTopoClusters", m_event, 0);
    // save the clusters at the EM scale
    for( auto clus : * caloClusters ) {
      if ( clus->pt ( xAOD::CaloCluster::State::UNCALIBRATED ) < 2000 ) { continue; } // 2 GeV cut
      m_caloCluster_pt. push_back( clus->pt ( xAOD::CaloCluster::State::UNCALIBRATED ) / m_units );
      m_caloCluster_eta.push_back( clus->eta( xAOD::CaloCluster::State::UNCALIBRATED ) );
      m_caloCluster_phi.push_back( clus->phi( xAOD::CaloCluster::State::UNCALIBRATED ) );
      m_caloCluster_e.  push_back( clus->e  ( xAOD::CaloCluster::State::UNCALIBRATED ) / m_units );
    }
  }

  if( m_eventInfoSwitch->m_truth && m_event && m_isMC ) {
    //MC Truth
    const xAOD::TruthEventContainer* truthE = 0;
    HelperFunctions::retrieve( truthE, "TruthEvents", m_event, 0 );
    if( truthE ) {
      const xAOD::TruthEvent* truthEvent = truthE->at(0);
      truthEvent->pdfInfoParameter(m_pdgId1,   xAOD::TruthEvent::PDGID1);
      truthEvent->pdfInfoParameter(m_pdgId2,   xAOD::TruthEvent::PDGID2);
      truthEvent->pdfInfoParameter(m_pdfId1,   xAOD::TruthEvent::PDFID1);
      truthEvent->pdfInfoParameter(m_pdfId2,   xAOD::TruthEvent::PDFID2);
      truthEvent->pdfInfoParameter(m_x1,       xAOD::TruthEvent::X1);
      truthEvent->pdfInfoParameter(m_x2,       xAOD::TruthEvent::X2);
      //truthEvent->pdfInfoParameter(m_scale,    xAOD::TruthEvent::SCALE);
      //truthEvent->pdfInfoParameter(m_q,        xAOD::TruthEvent::Q);
      //truthEvent->pdfInfoParameter(m_pdf1,     xAOD::TruthEvent::PDF1);
      //truthEvent->pdfInfoParameter(m_pdf2,     xAOD::TruthEvent::PDF2);
      truthEvent->pdfInfoParameter(m_xf1,      xAOD::TruthEvent::XF1);
      truthEvent->pdfInfoParameter(m_xf2,      xAOD::TruthEvent::XF2);

//      // crashes because of q?`
//        const xAOD::TruthEvent::PdfInfo info = truthEvent->pdfInfo();
//        if( info.valid() ) {
//          m_pdgId1 = info.pdgId1;
//          m_pdgId2 = info.pdgId2;
//          m_pdfId1 = info.pdfId1;
//          m_pdfId2 = info.pdfId2;
//          m_x1     = info.x1;
//          m_x2     = info.x2;
//          //m_q      = info.Q;
//          m_xf1    = info.xf1;
//          m_xf2    = info.xf2;
//        }

    }

  }

  this->FillEventUser(eventInfo);
}

/*********************
 *
 *   TRIGGER
 *
 ********************/
void HelpTreeBase::AddTrigger( const std::string detailStr ) {

  if(m_debug) Info("AddTrigger()", "Adding trigger variables: %s", detailStr.c_str());

  m_trigInfoSwitch = new HelperClasses::TriggerInfoSwitch( detailStr );

  // Add these basic branches
  if ( m_trigInfoSwitch->m_basic ) {
    m_tree->Branch("passL1",          &m_passL1,       "passL1/I"      );
    m_tree->Branch("passHLT",         &m_passHLT,      "passHLT/I"     );
  }

  // Detailed trigger infoformation related to the menu (might be useful)
  // This is useful for using this database: https://atlas-trigconf.cern.ch/
  if ( m_trigInfoSwitch->m_menuKeys ) {
    m_tree->Branch("masterKey",       &m_masterKey,    "masterKey/I"         );
    m_tree->Branch("lvl1PrescaleKey", &m_L1PSKey,      "lvl1PrescaleKey/I"   );
    m_tree->Branch("hltPrescaleKey",  &m_HLTPSKey,     "hltPrescaleKey/I"    );
  }

  // Trigger Decision for each and every trigger in a vector
  if ( m_trigInfoSwitch->m_passTriggers ) {
    // vector of strings for trigger names which fired
    m_tree->Branch("passedTriggers",      &m_passTriggers    );
    m_tree->Branch("triggerPrescales",      &m_triggerPrescales    );
  }

  //this->AddTriggerUser();
}

// Fill the information in the trigger branches
void HelpTreeBase::FillTrigger( const xAOD::EventInfo* eventInfo ) {

  if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Filling trigger info"); }

  // Clear previous events
  this->ClearTrigger();
  this->ClearTriggerUser();

  // Grab the global pass information from the TrigDecisionTool
  if ( m_trigInfoSwitch->m_basic ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Switch: m_trigInfoSwitch->m_basic"); }

    static SG::AuxElement::ConstAccessor< int > passAny("passAny");
    static SG::AuxElement::ConstAccessor< int > passL1("passL1");
    if( passL1.isAvailable( *eventInfo ) ) { m_passL1 = passL1( *eventInfo ); }
    else { m_passL1 = -1; }
    static SG::AuxElement::ConstAccessor< int > passHLT("passHLT");
    if( passHLT.isAvailable( *eventInfo ) ) { m_passHLT = passHLT( *eventInfo ); }
    else { m_passHLT = -1; }

  }

  // If detailed menu information about the configuration keys, turn this on.
  // This is useful for using this database: https://atlas-trigconf.cern.ch/
  if ( m_trigInfoSwitch->m_menuKeys ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Switch: m_trigInfoSwitch->m_menuKeys"); }

    static SG::AuxElement::ConstAccessor< int > masterKey("masterKey");
    if( masterKey.isAvailable( *eventInfo ) ) { m_masterKey = masterKey( *eventInfo ); }
    else { m_masterKey = -999; }
    static SG::AuxElement::ConstAccessor< int > L1PSKey("L1PSKey");
    if( L1PSKey.isAvailable( *eventInfo ) ) { m_L1PSKey = L1PSKey( *eventInfo ); }
    else { m_L1PSKey = -999; }
    static SG::AuxElement::ConstAccessor< int > HLTPSKey("HLTPSKey");
    if( HLTPSKey.isAvailable( *eventInfo ) ) { m_HLTPSKey = HLTPSKey( *eventInfo ); }
    else { m_HLTPSKey = -999; }

  }

  // If detailed information about each and every trigger is desired
  // save a vector of strings holding passing decisions
  if ( m_trigInfoSwitch->m_passTriggers ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Switch: m_trigInfoSwitch->m_passTriggers"); }
    static SG::AuxElement::ConstAccessor< std::vector< std::string > > passTrigs("passTriggers");
    if( passTrigs.isAvailable( *eventInfo ) ) { m_passTriggers = passTrigs( *eventInfo ); }

    static SG::AuxElement::ConstAccessor< std::vector< float > > trigPrescales("triggerPrescales");
    if( trigPrescales.isAvailable( *eventInfo ) ) { m_triggerPrescales = trigPrescales( *eventInfo ); }
  }

}

// Clear Trigger
void HelpTreeBase::ClearTrigger() {

  m_passL1  = -999;
  m_passHLT = -999;

  m_masterKey = 0;
  m_L1PSKey   = 0;
  m_HLTPSKey  = 0;

  m_passTriggers.clear();
  m_triggerPrescales.clear();

}

/*********************
 *
 *   JET TRIGGER
 *
 ********************/

/* TODO: jet trigger */
//CD: is this useful at all?
void HelpTreeBase::AddJetTrigger( const std::string detailStr )
{
  if ( m_debug )  Info("AddJetTrigger()", "Adding jet trigger variables: %s", detailStr.c_str());
}
void HelpTreeBase::FillJetTrigger(  ) { }
void HelpTreeBase::ClearJetTrigger(  ) { }


/*********************
 *
 *   MUONS
 *
 ********************/

void HelpTreeBase::AddMuons(const std::string detailStr) {

  if ( m_debug )  Info("AddMuons()", "Adding muon variables: %s", detailStr.c_str());

  m_muInfoSwitch = new HelperClasses::MuonInfoSwitch( detailStr );
  // always
  m_tree->Branch("nmuon",   &m_nmuon, "nmuon/I");

  if ( m_muInfoSwitch->m_kinematic ){
    m_tree->Branch("muon_pt",  &m_muon_pt);
    m_tree->Branch("muon_phi", &m_muon_phi);
    m_tree->Branch("muon_eta", &m_muon_eta);
    m_tree->Branch("muon_m",   &m_muon_m);
  }

  if ( m_muInfoSwitch->m_trigger ){
    // this is true if there's a match for at least one trigger chain
    m_tree->Branch("muon_isTrigMatched", &m_muon_isTrigMatched);
    // a vector of trigger match decision for each muon trigger chain
    m_tree->Branch( "muon_isTrigMatchedToChain", &m_muon_isTrigMatchedToChain );
    // a vector of strings for each muon trigger chain - 1:1 correspondence w/ vector above
    m_tree->Branch( "muon_listTrigChains", &m_muon_listTrigChains );
  }

  if ( m_muInfoSwitch->m_isolation ) {
    m_tree->Branch("muon_isIsolated_LooseTrackOnly", &m_muon_isIsolated_LooseTrackOnly);
    m_tree->Branch("muon_isIsolated_Loose",	     &m_muon_isIsolated_Loose);
    m_tree->Branch("muon_isIsolated_Tight",	     &m_muon_isIsolated_Tight);
    m_tree->Branch("muon_isIsolated_Gradient",	     &m_muon_isIsolated_Gradient);
    m_tree->Branch("muon_isIsolated_GradientLoose",  &m_muon_isIsolated_GradientLoose);
    m_tree->Branch("muon_isIsolated_FixedCutLoose",	   &m_muon_isIsolated_FixedCutLoose);
    m_tree->Branch("muon_isIsolated_FixedCutTightTrackOnly", &m_muon_isIsolated_FixedCutTightTrackOnly);
    m_tree->Branch("muon_isIsolated_UserDefinedFixEfficiency",    &m_muon_isIsolated_UserDefinedFixEfficiency);
    m_tree->Branch("muon_isIsolated_UserDefinedCut",              &m_muon_isIsolated_UserDefinedCut);
    m_tree->Branch("muon_ptcone20",	  &m_muon_ptcone20);
    m_tree->Branch("muon_ptcone30",	  &m_muon_ptcone30);
    m_tree->Branch("muon_ptcone40",	  &m_muon_ptcone40);
    m_tree->Branch("muon_ptvarcone20",	  &m_muon_ptvarcone20);
    m_tree->Branch("muon_ptvarcone30",	  &m_muon_ptvarcone30);
    m_tree->Branch("muon_ptvarcone40",	  &m_muon_ptvarcone40);
    m_tree->Branch("muon_topoetcone20",   &m_muon_topoetcone20);
    m_tree->Branch("muon_topoetcone30",   &m_muon_topoetcone30);
    m_tree->Branch("muon_topoetcone40",   &m_muon_topoetcone40);
  }

  if ( m_muInfoSwitch->m_effSF && m_isMC ) {
    m_tree->Branch("muon_RecoEff_SF_Loose",         &m_muon_RecoEff_SF_Loose);
    m_tree->Branch("muon_TrigEff_SF_Loose_Loose",		    &m_muon_TrigEff_SF_Loose_Loose);
    m_tree->Branch("muon_TrigEff_SF_Loose_FixedCutTightTrackOnly",  &m_muon_TrigEff_SF_Loose_FixedCutTightTrackOnly);
    m_tree->Branch("muon_TrigMCEff_Loose_Loose",		    &m_muon_TrigMCEff_Loose_Loose);
    m_tree->Branch("muon_TrigMCEff_Loose_FixedCutTightTrackOnly",   &m_muon_TrigMCEff_Loose_FixedCutTightTrackOnly);
    m_tree->Branch("muon_IsoEff_SF_LooseTrackOnly", &m_muon_IsoEff_SF_LooseTrackOnly);
    m_tree->Branch("muon_IsoEff_SF_Loose",	    &m_muon_IsoEff_SF_Loose);
    m_tree->Branch("muon_IsoEff_SF_Tight",	    &m_muon_IsoEff_SF_Tight);
    m_tree->Branch("muon_IsoEff_SF_Gradient",	    &m_muon_IsoEff_SF_Gradient);
    m_tree->Branch("muon_IsoEff_SF_GradientLoose",  &m_muon_IsoEff_SF_GradientLoose);
    m_tree->Branch("muon_IsoEff_SF_FixedCutTightTrackOnly",  &m_muon_IsoEff_SF_FixedCutTightTrackOnly);
    m_tree->Branch("muon_TTVAEff_SF",  &m_muon_TTVAEff_SF);
  }

  if ( m_muInfoSwitch->m_quality ) {
    m_tree->Branch("muon_isVeryLoose",  &m_muon_isVeryLoose);
    m_tree->Branch("muon_isLoose",      &m_muon_isLoose);
    m_tree->Branch("muon_isMedium",     &m_muon_isMedium);
    m_tree->Branch("muon_isTight",      &m_muon_isTight);
  }

  if ( m_muInfoSwitch->m_trackparams ) {
    m_tree->Branch("muon_trkd0",          &m_muon_trkd0);
    m_tree->Branch("muon_trkd0sig",       &m_muon_trkd0sig);
    m_tree->Branch("muon_trkz0",          &m_muon_trkz0);
    m_tree->Branch("muon_trkz0sintheta",  &m_muon_trkz0sintheta);
    m_tree->Branch("muon_trkphi0",        &m_muon_trkphi0);
    m_tree->Branch("muon_trktheta",       &m_muon_trktheta);
    m_tree->Branch("muon_trkcharge",      &m_muon_trkcharge);
    m_tree->Branch("muon_trkqOverP",      &m_muon_trkqOverP);
  }

  if ( m_muInfoSwitch->m_trackhitcont ) {
    m_tree->Branch("muon_trknSiHits",    &m_muon_trknSiHits);
    m_tree->Branch("muon_trknPixHits",   &m_muon_trknPixHits);
    m_tree->Branch("muon_trknPixHoles",  &m_muon_trknPixHoles);
    m_tree->Branch("muon_trknSCTHits",   &m_muon_trknSCTHits);
    m_tree->Branch("muon_trknSCTHoles",  &m_muon_trknSCTHoles);
    m_tree->Branch("muon_trknTRTHits",   &m_muon_trknTRTHits);
    m_tree->Branch("muon_trknTRTHoles",  &m_muon_trknTRTHoles);
    m_tree->Branch("muon_trknBLayerHits",&m_muon_trknBLayerHits);
    if ( !m_DC14 ) {
      m_tree->Branch("muon_trknInnermostPixLayHits",  &m_muon_trknInnermostPixLayHits);
      m_tree->Branch("muon_trkPixdEdX",    &m_muon_trkPixdEdX);
    }
  }

  if( m_muInfoSwitch->m_energyLoss ) {
    m_tree->Branch("muon_EnergyLoss"                ,  &m_muon_EnergyLoss               );
    m_tree->Branch("muon_EnergyLossSigma"           ,  &m_muon_EnergyLossSigma          );
    m_tree->Branch("muon_energyLossType"            ,  &m_muon_energyLossType           );
    m_tree->Branch("muon_MeasEnergyLoss"            ,  &m_muon_MeasEnergyLoss           );
    m_tree->Branch("muon_MeasEnergyLossSigma"       ,  &m_muon_MeasEnergyLossSigma      );
    m_tree->Branch("muon_ParamEnergyLoss"           ,  &m_muon_ParamEnergyLoss          );
    m_tree->Branch("muon_ParamEnergyLossSigmaMinus" ,  &m_muon_ParamEnergyLossSigmaMinus);
    m_tree->Branch("muon_ParamEnergyLossSigmaPlus"  ,  &m_muon_ParamEnergyLossSigmaPlus );
  }

  this->AddMuonsUser();
}

void HelpTreeBase::FillMuons( const xAOD::MuonContainer* muons, const xAOD::Vertex* primaryVertex ) {

  this->ClearMuons();
  this->ClearMuonsUser();

  m_nmuon = 0;
  for ( auto muon_itr : *(muons) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillMuons()", "Filling muon w/ pT = %2f", muon_itr->pt() / m_units ); }
    if ( m_muInfoSwitch->m_kinematic ) {
      m_muon_pt.push_back ( muon_itr->pt() / m_units  );
      m_muon_eta.push_back( muon_itr->eta() );
      m_muon_phi.push_back( muon_itr->phi() );
      m_muon_m.push_back  ( muon_itr->m() / m_units  );
    }

    if ( m_muInfoSwitch->m_trigger ) {

      // retrieve map<string,char> w/ <chain,isMatched>
      //
      static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapMuAcc("isTrigMatchedMapMu");

      std::vector<int> matches;

      if ( isTrigMatchedMapMuAcc.isAvailable( *muon_itr ) ) {
	 // loop over map and fill branches
	 //
	 for ( auto const &it : (isTrigMatchedMapMuAcc( *muon_itr )) ) {
  	   matches.push_back( static_cast<int>(it.second) );
	   m_muon_listTrigChains.push_back( it.first );
	 }
       } else {
	 matches.push_back( -1 );
	 m_muon_listTrigChains.push_back("NONE");
       }

       m_muon_isTrigMatchedToChain.push_back(matches);

       // if at least one match among the chains is found, say this muon is trigger matched
       if ( std::find(matches.begin(), matches.end(), 1) != matches.end() ) { m_muon_isTrigMatched.push_back(1); }
       else { m_muon_isTrigMatched.push_back(0); }

    }


    if ( m_muInfoSwitch->m_isolation ) {

      static SG::AuxElement::Accessor<char> isIsoLooseTrackOnlyAcc ("isIsolated_LooseTrackOnly");
      static SG::AuxElement::Accessor<char> isIsoLooseAcc ("isIsolated_Loose");
      static SG::AuxElement::Accessor<char> isIsoTightAcc ("isIsolated_Tight");
      static SG::AuxElement::Accessor<char> isIsoGradientAcc ("isIsolated_Gradient");
      static SG::AuxElement::Accessor<char> isIsoGradientLooseAcc ("isIsolated_GradientLoose");
      static SG::AuxElement::Accessor<char> isIsoFixedCutLooseAcc ("isIsolated_FixedCutLoose");
      static SG::AuxElement::Accessor<char> isIsoFixedCutTightTrackOnlyAcc ("isIsolated_FixedCutTightTrackOnly");
      static SG::AuxElement::Accessor<char> isIsoUserDefinedFixEfficiencyAcc ("isIsolated_UserDefinedFixEfficiency");
      static SG::AuxElement::Accessor<char> isIsoUserDefinedCutAcc ("isIsolated_UserDefinedCut");

      if ( isIsoLooseTrackOnlyAcc.isAvailable( *muon_itr ) ) { m_muon_isIsolated_LooseTrackOnly.push_back( isIsoLooseTrackOnlyAcc( *muon_itr ) ); } else { m_muon_isIsolated_LooseTrackOnly.push_back( -1 ); }
      if ( isIsoLooseAcc.isAvailable( *muon_itr ) )          { m_muon_isIsolated_Loose.push_back( isIsoLooseAcc( *muon_itr ) ); } else { m_muon_isIsolated_Loose.push_back( -1 ); }
      if ( isIsoTightAcc.isAvailable( *muon_itr ) )          { m_muon_isIsolated_Tight.push_back( isIsoTightAcc( *muon_itr ) ); } else { m_muon_isIsolated_Tight.push_back( -1 ); }
      if ( isIsoGradientAcc.isAvailable( *muon_itr ) )       { m_muon_isIsolated_Gradient.push_back( isIsoGradientAcc( *muon_itr ) ); } else { m_muon_isIsolated_Gradient.push_back( -1 ); }
      if ( isIsoGradientLooseAcc.isAvailable( *muon_itr ) )  { m_muon_isIsolated_GradientLoose.push_back( isIsoGradientLooseAcc( *muon_itr ) ); } else { m_muon_isIsolated_GradientLoose.push_back( -1 ); }
      if ( isIsoFixedCutLooseAcc.isAvailable( *muon_itr ) )          { m_muon_isIsolated_FixedCutLoose.push_back( isIsoFixedCutLooseAcc( *muon_itr ) ); } else { m_muon_isIsolated_FixedCutLoose.push_back( -1 ); }
      if ( isIsoFixedCutTightTrackOnlyAcc.isAvailable( *muon_itr ) )   { m_muon_isIsolated_FixedCutTightTrackOnly.push_back( isIsoFixedCutTightTrackOnlyAcc( *muon_itr ) ); } else { m_muon_isIsolated_FixedCutTightTrackOnly.push_back( -1 ); }
      if ( isIsoUserDefinedFixEfficiencyAcc.isAvailable( *muon_itr ) ) { m_muon_isIsolated_UserDefinedFixEfficiency.push_back( isIsoUserDefinedFixEfficiencyAcc( *muon_itr ) ); } else { m_muon_isIsolated_UserDefinedFixEfficiency.push_back( -1 ); }
      if ( isIsoUserDefinedCutAcc.isAvailable( *muon_itr ) )           { m_muon_isIsolated_UserDefinedCut.push_back( isIsoUserDefinedCutAcc( *muon_itr ) ); } else { m_muon_isIsolated_UserDefinedCut.push_back( -1 ); }

      m_muon_ptcone20.push_back( muon_itr->isolation( xAOD::Iso::ptcone20 ) );
      m_muon_ptcone30.push_back( muon_itr->isolation( xAOD::Iso::ptcone30 ) );
      m_muon_ptcone40.push_back( muon_itr->isolation( xAOD::Iso::ptcone40 ) );
      m_muon_ptvarcone20.push_back( muon_itr->isolation( xAOD::Iso::ptvarcone20 ) );
      m_muon_ptvarcone30.push_back( muon_itr->isolation( xAOD::Iso::ptvarcone30 ) );
      m_muon_ptvarcone40.push_back( muon_itr->isolation( xAOD::Iso::ptvarcone40 ) );
      m_muon_topoetcone20.push_back( muon_itr->isolation( xAOD::Iso::topoetcone20 ) );
      m_muon_topoetcone30.push_back( muon_itr->isolation( xAOD::Iso::topoetcone30 ) );
      m_muon_topoetcone40.push_back( muon_itr->isolation( xAOD::Iso::topoetcone40 ) );

    }

    if ( m_muInfoSwitch->m_quality ) {
      static SG::AuxElement::Accessor<char> isVeryLooseQAcc ("isVeryLooseQ");
      static SG::AuxElement::Accessor<char> isLooseQAcc ("isLooseQ");
      static SG::AuxElement::Accessor<char> isMediumQAcc ("isMediumQ");
      static SG::AuxElement::Accessor<char> isTightQAcc ("isTightQ");

      if ( isVeryLooseQAcc.isAvailable( *muon_itr ) ) { m_muon_isVeryLoose.push_back( isVeryLooseQAcc( *muon_itr ) ); } else { m_muon_isVeryLoose.push_back( -1 ); }
      if ( isLooseQAcc.isAvailable( *muon_itr ) )     { m_muon_isLoose.push_back( isLooseQAcc( *muon_itr ) ); }         else { m_muon_isLoose.push_back( -1 ); }
      if ( isMediumQAcc.isAvailable( *muon_itr ) )    { m_muon_isMedium.push_back( isMediumQAcc( *muon_itr ) ); }       else { m_muon_isMedium.push_back( -1 ); }
      if ( isTightQAcc.isAvailable( *muon_itr ) )     { m_muon_isTight.push_back( isTightQAcc( *muon_itr ) ); }         else { m_muon_isTight.push_back( -1 ); }
    }

    const xAOD::TrackParticle* trk = muon_itr->primaryTrackParticle();

    if ( m_muInfoSwitch->m_trackparams ) {
      if ( trk ) {
        //
	// NB.:
	// All track parameters are calculated at the perigee, i.e., the point of closest approach to the origin of some r.f. (which in RunII is NOT the ATLAS detector r.f!).
	// The reference  frame is chosen to be a system centered in the beamspot position, with z axis parallel to the beam line.
	// Remember that the beamspot size ( of O(10 micrometers) in the transverse plane) is << average vertex transverse position resolution ( O(60-80 micrometers) )
	// The coordinates of this r.f. wrt. the ATLAS system origin are returned by means of vx(), vy(), vz()
	//
        m_muon_trkd0.push_back( trk->d0() );
        static SG::AuxElement::Accessor<float> d0SigAcc ("d0sig");
        float d0_significance =  ( d0SigAcc.isAvailable( *muon_itr ) ) ? fabs( d0SigAcc( *muon_itr ) ) : -1.0;
	m_muon_trkd0sig.push_back( d0_significance );
        m_muon_trkz0.push_back( trk->z0()  - ( primaryVertex->z() - trk->vz() ) );
        static SG::AuxElement::Accessor<float> z0sinthetaAcc("z0sintheta");
        float z0sintheta =  ( z0sinthetaAcc.isAvailable( *muon_itr ) ) ? z0sinthetaAcc( *muon_itr ) : -999.0;
        m_muon_trkz0sintheta.push_back( z0sintheta );
        m_muon_trkphi0.push_back( trk->phi0() );
        m_muon_trktheta.push_back( trk->theta() );
        m_muon_trkcharge.push_back( trk->charge() );
        m_muon_trkqOverP.push_back( trk->qOverP() );
      } else {
        m_muon_trkd0.push_back( -999.0 );
        m_muon_trkd0sig.push_back( -1.0 );
        m_muon_trkz0.push_back( -999.0 );
        m_muon_trkz0sintheta.push_back( -999.0 );
        m_muon_trkphi0.push_back( -999.0 );
        m_muon_trktheta.push_back( -999.0 );
        m_muon_trkcharge.push_back( -999.0 );
        m_muon_trkqOverP.push_back( -999.0 );
      }
    }

    if ( m_muInfoSwitch->m_trackhitcont ) {
      uint8_t nPixHits(-1), nPixHoles(-1), nSCTHits(-1), nSCTHoles(-1), nTRTHits(-1), nTRTHoles(-1), nBLayerHits(-1), nInnermostPixLayHits(-1);
      float pixdEdX(-1.0);
      if ( trk ) {
        trk->summaryValue( nPixHits,     xAOD::numberOfPixelHits );
        trk->summaryValue( nPixHoles,    xAOD::numberOfPixelHoles );
      	trk->summaryValue( nSCTHits,     xAOD::numberOfSCTHits );
      	trk->summaryValue( nSCTHoles,    xAOD::numberOfSCTHoles );
      	trk->summaryValue( nTRTHits,     xAOD::numberOfTRTHits );
      	trk->summaryValue( nTRTHoles,    xAOD::numberOfTRTHoles );
        trk->summaryValue( nBLayerHits,  xAOD::numberOfBLayerHits );
        if ( !m_DC14 ) {
	  trk->summaryValue( nInnermostPixLayHits, xAOD::numberOfInnermostPixelLayerHits );
          trk->summaryValue( pixdEdX,   xAOD::pixeldEdx);
        }
      }
      m_muon_trknSiHits.push_back( nPixHits + nSCTHits );
      m_muon_trknPixHits.push_back( nPixHits );
      m_muon_trknPixHoles.push_back( nPixHoles );
      m_muon_trknSCTHits.push_back( nSCTHits );
      m_muon_trknSCTHoles.push_back( nSCTHoles );
      m_muon_trknTRTHits.push_back( nTRTHits );
      m_muon_trknTRTHoles.push_back( nTRTHoles );
      m_muon_trknBLayerHits.push_back( nBLayerHits );
      if ( !m_DC14 ) {
        m_muon_trknInnermostPixLayHits.push_back( nInnermostPixLayHits );
        m_muon_trkPixdEdX.push_back( pixdEdX );
      }
    }

    if ( m_muInfoSwitch->m_effSF && m_isMC ) {

      static SG::AuxElement::Accessor< std::vector< float > > accRecoSF_Loose("MuonEfficiencyCorrector_RecoSyst_Loose");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigSF_Loose_Loose("MuonEfficiencyCorrector_TrigSyst_RecoLoose_IsoLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigSF_Loose_FixedCutTightTrackOnly("MuonEfficiencyCorrector_TrigSyst_RecoLoose_IsoFixedCutTightTrackOnly");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigMCEff_Loose_Loose("MuonEfficiencyCorrector_TrigMCEff_RecoLoose_IsoLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigMCEff_Loose_FixedCutTightTrackOnly("MuonEfficiencyCorrector_TrigMCEff_RecoLoose_IsoFixedCutTightTrackOnly");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_LooseTrackOnly("MuonEfficiencyCorrector_IsoSyst_LooseTrackOnly");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_Loose("MuonEfficiencyCorrector_IsoSyst_IsoLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_Tight("MuonEfficiencyCorrector_IsoSyst_IsoTight");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_Gradient("MuonEfficiencyCorrector_IsoSyst_IsoGradient");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_GradientLoose("MuonEfficiencyCorrector_IsoSyst_IsoGradientLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_FixedCutTightTrackOnly("MuonEfficiencyCorrector_IsoSyst_IsoFixedCutTightTrackOnly");
      static SG::AuxElement::Accessor< std::vector< float > > accTTVASF("MuonEfficiencyCorrector_TTVASyst_TTVA");

      std::vector<float> junkSF(1,1.0);
      std::vector<float> junkEff(1,0.0);

      if( accRecoSF_Loose.isAvailable( *muon_itr ) )         { m_muon_RecoEff_SF_Loose.push_back( accRecoSF_Loose( *muon_itr ) ); } else { m_muon_RecoEff_SF_Loose.push_back( junkSF ); }
      if ( accTrigSF_Loose_Loose.isAvailable( *muon_itr ) )                     { m_muon_TrigEff_SF_Loose_Loose.push_back( accTrigSF_Loose_Loose( *muon_itr ) ); } else { m_muon_TrigEff_SF_Loose_Loose.push_back( junkSF ); }
      if ( accTrigSF_Loose_FixedCutTightTrackOnly.isAvailable( *muon_itr ) )    { m_muon_TrigEff_SF_Loose_FixedCutTightTrackOnly.push_back( accTrigSF_Loose_FixedCutTightTrackOnly( *muon_itr ) ); } else { m_muon_TrigEff_SF_Loose_FixedCutTightTrackOnly.push_back( junkSF ); }
      if ( accTrigMCEff_Loose_Loose.isAvailable( *muon_itr ) )                  { m_muon_TrigMCEff_Loose_Loose.push_back( accTrigMCEff_Loose_Loose( *muon_itr ) ); } else { m_muon_TrigMCEff_Loose_Loose.push_back( junkEff ); }
      if ( accTrigMCEff_Loose_FixedCutTightTrackOnly.isAvailable( *muon_itr ) ) { m_muon_TrigMCEff_Loose_FixedCutTightTrackOnly.push_back( accTrigMCEff_Loose_FixedCutTightTrackOnly( *muon_itr ) ); } else { m_muon_TrigMCEff_Loose_FixedCutTightTrackOnly.push_back( junkEff ); }
      if( accIsoSF_LooseTrackOnly.isAvailable( *muon_itr ) ) { m_muon_IsoEff_SF_LooseTrackOnly.push_back( accIsoSF_LooseTrackOnly( *muon_itr ) ); } else { m_muon_IsoEff_SF_LooseTrackOnly.push_back( junkSF ); }
      if( accIsoSF_Loose.isAvailable( *muon_itr ) )          { m_muon_IsoEff_SF_Loose.push_back( accIsoSF_Loose( *muon_itr ) ); } else { m_muon_IsoEff_SF_Loose.push_back( junkSF ); }
      if( accIsoSF_Tight.isAvailable( *muon_itr ) )          { m_muon_IsoEff_SF_Tight.push_back( accIsoSF_Tight( *muon_itr ) ); } else { m_muon_IsoEff_SF_Tight.push_back( junkSF ); }
      if( accIsoSF_GradientLoose.isAvailable( *muon_itr ) )  { m_muon_IsoEff_SF_GradientLoose.push_back( accIsoSF_GradientLoose( *muon_itr ) ); } else {  m_muon_IsoEff_SF_GradientLoose.push_back( junkSF ); }
      if( accIsoSF_Gradient.isAvailable( *muon_itr ) )       { m_muon_IsoEff_SF_Gradient.push_back( accIsoSF_Gradient( *muon_itr ) ); } else { m_muon_IsoEff_SF_Gradient.push_back( junkSF ); }
      if( accIsoSF_FixedCutTightTrackOnly.isAvailable( *muon_itr ) )  { m_muon_IsoEff_SF_FixedCutTightTrackOnly.push_back( accIsoSF_FixedCutTightTrackOnly( *muon_itr ) ); } else {  m_muon_IsoEff_SF_FixedCutTightTrackOnly.push_back( junkSF ); }
      if( accTTVASF.isAvailable( *muon_itr ) )         { m_muon_TTVAEff_SF.push_back( accTTVASF( *muon_itr ) ); } else { m_muon_TTVAEff_SF.push_back( junkSF ); }

    }

    if(m_muInfoSwitch->m_energyLoss ) {
      static SG::AuxElement::Accessor< float >         accMuon_EnergyLoss                ("EnergyLoss");
      static SG::AuxElement::Accessor< float >         accMuon_EnergyLossSigma           ("EnergyLossSigma");
      static SG::AuxElement::Accessor< unsigned char > accMuon_energyLossType            ("energyLossType");
      static SG::AuxElement::Accessor< float >         accMuon_MeasEnergyLoss            ("MeasEnergyLoss");
      static SG::AuxElement::Accessor< float >         accMuon_MeasEnergyLossSigma       ("MeasEnergyLossSigma");
      static SG::AuxElement::Accessor< float >         accMuon_ParamEnergyLoss           ("ParamEnergyLoss");
      static SG::AuxElement::Accessor< float >         accMuon_ParamEnergyLossSigmaMinus ("ParamEnergyLossSigmaMinus");
      static SG::AuxElement::Accessor< float >         accMuon_ParamEnergyLossSigmaPlus  ("ParamEnergyLossSigmaPlus");

      if(accMuon_EnergyLoss                .isAvailable( *muon_itr)) m_muon_EnergyLoss               .push_back(accMuon_EnergyLoss                (*muon_itr)  );
      if(accMuon_EnergyLossSigma           .isAvailable( *muon_itr)) m_muon_EnergyLossSigma          .push_back(accMuon_EnergyLossSigma           (*muon_itr)  );
      if(accMuon_energyLossType            .isAvailable( *muon_itr)) m_muon_energyLossType           .push_back(accMuon_energyLossType            (*muon_itr)  );
      if(accMuon_MeasEnergyLoss            .isAvailable( *muon_itr)) m_muon_MeasEnergyLoss           .push_back(accMuon_MeasEnergyLoss            (*muon_itr)  );
      if(accMuon_MeasEnergyLossSigma       .isAvailable( *muon_itr)) m_muon_MeasEnergyLossSigma      .push_back(accMuon_MeasEnergyLossSigma       (*muon_itr)  );
      if(accMuon_ParamEnergyLoss           .isAvailable( *muon_itr)) m_muon_ParamEnergyLoss          .push_back(accMuon_ParamEnergyLoss           (*muon_itr)  );
      if(accMuon_ParamEnergyLossSigmaMinus .isAvailable( *muon_itr)) m_muon_ParamEnergyLossSigmaMinus.push_back(accMuon_ParamEnergyLossSigmaMinus (*muon_itr)  );
      if(accMuon_ParamEnergyLossSigmaPlus  .isAvailable( *muon_itr)) m_muon_ParamEnergyLossSigmaPlus .push_back(accMuon_ParamEnergyLossSigmaPlus  (*muon_itr)  );
    }

    this->FillMuonsUser(muon_itr);

    m_nmuon++;
  }
}

void HelpTreeBase::ClearMuons() {

  m_nmuon = 0;

  if ( m_muInfoSwitch->m_kinematic ) {
    m_muon_pt.clear();
    m_muon_eta.clear();
    m_muon_phi.clear();
    m_muon_m.clear();
  }

  if ( m_muInfoSwitch->m_trigger ) {
    m_muon_isTrigMatched.clear();
    m_muon_isTrigMatchedToChain.clear();
    m_muon_listTrigChains.clear();
  }

  if ( m_muInfoSwitch->m_isolation ) {
    m_muon_isIsolated_LooseTrackOnly.clear();
    m_muon_isIsolated_Loose.clear();
    m_muon_isIsolated_Tight.clear();
    m_muon_isIsolated_Gradient.clear();
    m_muon_isIsolated_GradientLoose.clear();
    m_muon_isIsolated_FixedCutLoose.clear();
    m_muon_isIsolated_FixedCutTightTrackOnly.clear();
    m_muon_isIsolated_UserDefinedFixEfficiency.clear();
    m_muon_isIsolated_UserDefinedCut.clear();
    m_muon_ptcone20.clear();
    m_muon_ptcone30.clear();
    m_muon_ptcone40.clear();
    m_muon_ptvarcone20.clear();
    m_muon_ptvarcone30.clear();
    m_muon_ptvarcone40.clear();
    m_muon_topoetcone20.clear();
    m_muon_topoetcone30.clear();
    m_muon_topoetcone40.clear();
  }

  if ( m_muInfoSwitch->m_quality ) {
    m_muon_isVeryLoose.clear();
    m_muon_isLoose.clear();
    m_muon_isMedium.clear();
    m_muon_isTight.clear();
  }

  if ( m_muInfoSwitch->m_trackparams ) {
    m_muon_trkd0.clear();
    m_muon_trkd0sig.clear();
    m_muon_trkz0.clear();
    m_muon_trkz0sintheta.clear();
    m_muon_trkphi0.clear();
    m_muon_trktheta.clear();
    m_muon_trkcharge.clear();
    m_muon_trkqOverP.clear();
  }

  if ( m_muInfoSwitch->m_trackhitcont ) {
    m_muon_trknSiHits.clear();
    m_muon_trknPixHits.clear();
    m_muon_trknPixHoles.clear();
    m_muon_trknSCTHits.clear();
    m_muon_trknSCTHoles.clear();
    m_muon_trknTRTHits.clear();
    m_muon_trknTRTHoles.clear();
    m_muon_trknBLayerHits.clear();
    if ( !m_DC14 ) {
      m_muon_trknInnermostPixLayHits.clear();
      m_muon_trkPixdEdX.clear();
    }
  }

  if ( m_muInfoSwitch->m_effSF && m_isMC ) {
    m_muon_RecoEff_SF_Loose.clear();
    m_muon_TrigEff_SF_Loose_Loose.clear();
    m_muon_TrigEff_SF_Loose_FixedCutTightTrackOnly.clear();
    m_muon_TrigMCEff_Loose_Loose.clear();
    m_muon_TrigMCEff_Loose_FixedCutTightTrackOnly.clear();
    m_muon_IsoEff_SF_LooseTrackOnly.clear();
    m_muon_IsoEff_SF_Loose.clear();
    m_muon_IsoEff_SF_Tight.clear();
    m_muon_IsoEff_SF_Gradient.clear();
    m_muon_IsoEff_SF_GradientLoose.clear();
    m_muon_IsoEff_SF_FixedCutTightTrackOnly.clear();
    m_muon_TTVAEff_SF.clear();
  }

  if ( m_muInfoSwitch->m_energyLoss ) {
    m_muon_EnergyLoss.clear();
    m_muon_EnergyLossSigma.clear();
    m_muon_energyLossType.clear();
    m_muon_MeasEnergyLoss.clear();
    m_muon_MeasEnergyLossSigma.clear();
    m_muon_ParamEnergyLoss.clear();
    m_muon_ParamEnergyLossSigmaMinus.clear();
    m_muon_ParamEnergyLossSigmaPlus.clear();

  }

}

/*********************
 *
 *   ELECTRONS
 *
 ********************/

void HelpTreeBase::AddElectrons(const std::string detailStr) {

  if(m_debug)  Info("AddElectrons()", "Adding electron variables: %s", detailStr.c_str());

  m_elInfoSwitch = new HelperClasses::ElectronInfoSwitch( detailStr );

  // always
  m_tree->Branch("nel",    &m_nel,"nel/I");

  if ( m_elInfoSwitch->m_kinematic ) {
    m_tree->Branch("el_pt",  &m_el_pt);
    m_tree->Branch("el_phi", &m_el_phi);
    m_tree->Branch("el_eta", &m_el_eta);
    m_tree->Branch("el_m",   &m_el_m);
    m_tree->Branch("el_caloCluster_eta", &m_el_caloCluster_eta);
  }

  if ( m_elInfoSwitch->m_trigger ){
    // this is true if there's a match for at least one trigger chain
    m_tree->Branch("el_isTrigMatched", &m_el_isTrigMatched);
    // a vector of trigger match decision for each electron trigger chain
    m_tree->Branch("el_isTrigMatchedToChain", &m_el_isTrigMatchedToChain);
    // a vector of strings with the electron trigger chain names - 1:1 correspondence w/ vector above
    m_tree->Branch("el_listTrigChains", &m_el_listTrigChains);
  }

  if ( m_elInfoSwitch->m_isolation ) {
    m_tree->Branch("el_isIsolated_LooseTrackOnly", &m_el_isIsolated_LooseTrackOnly);
    m_tree->Branch("el_isIsolated_Loose",	   &m_el_isIsolated_Loose);
    m_tree->Branch("el_isIsolated_Tight",	   &m_el_isIsolated_Tight);
    m_tree->Branch("el_isIsolated_Gradient",	   &m_el_isIsolated_Gradient);
    m_tree->Branch("el_isIsolated_GradientLoose",  &m_el_isIsolated_GradientLoose);
    m_tree->Branch("el_isIsolated_FixedCutLoose",	   &m_el_isIsolated_FixedCutLoose);
    m_tree->Branch("el_isIsolated_FixedCutTight",	   &m_el_isIsolated_FixedCutTight);
    m_tree->Branch("el_isIsolated_FixedCutTightTrackOnly", &m_el_isIsolated_FixedCutTightTrackOnly);
    m_tree->Branch("el_isIsolated_UserDefinedFixEfficiency",    &m_el_isIsolated_UserDefinedFixEfficiency);
    m_tree->Branch("el_isIsolated_UserDefinedCut",              &m_el_isIsolated_UserDefinedCut);
    m_tree->Branch("el_etcone20",	  &m_el_etcone20);
    m_tree->Branch("el_ptcone20",	  &m_el_ptcone20);
    m_tree->Branch("el_ptcone30",	  &m_el_ptcone30);
    m_tree->Branch("el_ptcone40",	  &m_el_ptcone40);
    m_tree->Branch("el_ptvarcone20",	  &m_el_ptvarcone20);
    m_tree->Branch("el_ptvarcone30",	  &m_el_ptvarcone30);
    m_tree->Branch("el_ptvarcone40",	  &m_el_ptvarcone40);
    m_tree->Branch("el_topoetcone20",     &m_el_topoetcone20);
    m_tree->Branch("el_topoetcone30",     &m_el_topoetcone30);
    m_tree->Branch("el_topoetcone40",     &m_el_topoetcone40);
  }

  if ( m_elInfoSwitch->m_PID ) {
    m_tree->Branch("nel_LHLoose",      &m_nel_LHLoose);
    m_tree->Branch("el_LHLoose",       &m_el_LHLoose);
    m_tree->Branch("nel_LHMedium",     &m_nel_LHMedium);
    m_tree->Branch("el_LHMedium",      &m_el_LHMedium);
    m_tree->Branch("nel_LHTight",      &m_nel_LHTight);
    m_tree->Branch("el_LHTight",       &m_el_LHTight);
    m_tree->Branch("nel_IsEMLoose",    &m_nel_IsEMLoose);
    m_tree->Branch("el_IsEMLoose",     &m_el_IsEMLoose);
    m_tree->Branch("nel_IsEMMedium",   &m_nel_IsEMMedium);
    m_tree->Branch("el_IsEMMedium",    &m_el_IsEMMedium);
    m_tree->Branch("nel_IsEMTight",    &m_nel_IsEMTight);
    m_tree->Branch("el_IsEMTight",     &m_el_IsEMTight);
  }

  if ( m_elInfoSwitch->m_effSF && m_isMC ) {
    m_tree->Branch("el_RecoEff_SF"  ,		 &m_el_RecoEff_SF  );
    m_tree->Branch("el_TrigEff_SF_LHLooseAndBLayer"  ,	 &m_el_TrigEff_SF_LHLooseAndBLayer  );
    m_tree->Branch("el_TrigEff_SF_LHTight"  ,&m_el_TrigEff_SF_LHTight  );
    m_tree->Branch("el_TrigMCEff_LHLooseAndBLayer"   ,&m_el_TrigMCEff_LHLooseAndBLayer  );
    m_tree->Branch("el_TrigMCEff_LHTight"   ,&m_el_TrigMCEff_LHTight  );
    m_tree->Branch("el_IsoEff_SF_Loose"  , &m_el_IsoEff_SF_Loose );
    m_tree->Branch("el_IsoEff_SF_FixedCutTight"  , &m_el_IsoEff_SF_FixedCutTight );
    m_tree->Branch("el_PIDEff_SF_LHLooseAndBLayer",  &m_el_PIDEff_SF_LHLooseAndBLayer);
    m_tree->Branch("el_PIDEff_SF_LHLoose",       &m_el_PIDEff_SF_LHLoose);
    m_tree->Branch("el_PIDEff_SF_LHMedium",      &m_el_PIDEff_SF_LHMedium);
    m_tree->Branch("el_PIDEff_SF_LHTight",       &m_el_PIDEff_SF_LHTight);
  }

  if ( m_elInfoSwitch->m_trackparams ) {
    m_tree->Branch("el_trkd0",      &m_el_trkd0);
    m_tree->Branch("el_trkd0sig",   &m_el_trkd0sig);
    m_tree->Branch("el_trkz0",      &m_el_trkz0);
    m_tree->Branch("el_trkz0sintheta",  &m_el_trkz0sintheta);
    m_tree->Branch("el_trkphi0",    &m_el_trkphi0);
    m_tree->Branch("el_trktheta",   &m_el_trktheta);
    m_tree->Branch("el_trkcharge",  &m_el_trkcharge);
    m_tree->Branch("el_trkqOverP",  &m_el_trkqOverP);
  }

  if ( m_elInfoSwitch->m_trackhitcont ) {
    m_tree->Branch("el_trknSiHits",    &m_el_trknSiHits);
    m_tree->Branch("el_trknPixHits",   &m_el_trknPixHits);
    m_tree->Branch("el_trknPixHoles",  &m_el_trknPixHoles);
    m_tree->Branch("el_trknSCTHits",   &m_el_trknSCTHits);
    m_tree->Branch("el_trknSCTHoles",  &m_el_trknSCTHoles);
    m_tree->Branch("el_trknTRTHits",   &m_el_trknTRTHits);
    m_tree->Branch("el_trknTRTHoles",  &m_el_trknTRTHoles);
    m_tree->Branch("el_trknBLayerHits",&m_el_trknBLayerHits);
    if ( !m_DC14 ) {
      m_tree->Branch("el_trknInnermostPixLayHits",  &m_el_trknInnermostPixLayHits);
      m_tree->Branch("el_trkPixdEdX",    &m_el_trkPixdEdX);
    }
  }

  this->AddElectronsUser();
}

void HelpTreeBase::FillElectrons( const xAOD::ElectronContainer* electrons, const xAOD::Vertex* primaryVertex ) {

  this->ClearElectrons();
  this->ClearElectronsUser();

  m_nel = 0;
  m_nel_LHLoose = 0;
  m_nel_LHMedium = 0;
  m_nel_LHTight = 0;
  m_nel_IsEMLoose = 0;
  m_nel_IsEMMedium = 0;
  m_nel_IsEMTight = 0;

  for ( auto el_itr : *(electrons) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillElectrons()", "Filling electron w/ pT = %2f", el_itr->pt() / m_units ); }

    const xAOD::TrackParticle* trk = el_itr->trackParticle();

    if ( m_elInfoSwitch->m_kinematic ) {
      m_el_pt.push_back ( (el_itr)->pt() / m_units );
      m_el_eta.push_back( (el_itr)->eta() );
      m_el_phi.push_back( (el_itr)->phi() );
      m_el_m.push_back  ( (el_itr)->m() / m_units );

      float calo_eta   = ( el_itr->caloCluster() ) ? el_itr->caloCluster()->etaBE(2) : -999.0;
      m_el_caloCluster_eta.push_back( calo_eta );

    }

    if ( m_elInfoSwitch->m_trigger ) {

      // retrieve map<string,char> w/ <chain,isMatched>
      //
      static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapElAcc("isTrigMatchedMapEl");

      std::vector<int> matches;

      if ( isTrigMatchedMapElAcc.isAvailable( *el_itr ) ) {
	 // loop over map and fill branches
	 //
	 for ( auto const &it : (isTrigMatchedMapElAcc( *el_itr )) ) {
  	   matches.push_back( static_cast<int>(it.second) );
	   m_el_listTrigChains.push_back( it.first );
	 }
       } else {
	 matches.push_back( -1 );
	 m_el_listTrigChains.push_back("NONE");
       }

       m_el_isTrigMatchedToChain.push_back(matches);

       // if at least one match among the chains is found, say this electron is trigger matched
       if ( std::find(matches.begin(), matches.end(), 1) != matches.end() ) { m_el_isTrigMatched.push_back(1); }
       else { m_el_isTrigMatched.push_back(0); }

    }

    if ( m_elInfoSwitch->m_isolation ) {

      static SG::AuxElement::Accessor<char> isIsoLooseTrackOnlyAcc ("isIsolated_LooseTrackOnly");
      static SG::AuxElement::Accessor<char> isIsoLooseAcc ("isIsolated_Loose");
      static SG::AuxElement::Accessor<char> isIsoTightAcc ("isIsolated_Tight");
      static SG::AuxElement::Accessor<char> isIsoGradientAcc ("isIsolated_Gradient");
      static SG::AuxElement::Accessor<char> isIsoGradientLooseAcc ("isIsolated_GradientLoose");
      static SG::AuxElement::Accessor<char> isIsoFixedCutLooseAcc ("isIsolated_FixedCutLoose");
      static SG::AuxElement::Accessor<char> isIsoFixedCutTightAcc ("isIsolated_FixedCutTight");
      static SG::AuxElement::Accessor<char> isIsoFixedCutTightTrackOnlyAcc ("isIsolated_FixedCutTightTrackOnly");
      static SG::AuxElement::Accessor<char> isIsoUserDefinedFixEfficiencyAcc ("isIsolated_UserDefinedFixEfficiency");
      static SG::AuxElement::Accessor<char> isIsoUserDefinedCutAcc ("isIsolated_UserDefinedCut");

      if ( isIsoLooseTrackOnlyAcc.isAvailable( *el_itr ) ) { m_el_isIsolated_LooseTrackOnly.push_back( isIsoLooseTrackOnlyAcc( *el_itr ) ); } else { m_el_isIsolated_LooseTrackOnly.push_back( -1 ); }
      if ( isIsoLooseAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_Loose.push_back( isIsoLooseAcc( *el_itr ) ); } else { m_el_isIsolated_Loose.push_back( -1 ); }
      if ( isIsoTightAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_Tight.push_back( isIsoTightAcc( *el_itr ) ); } else { m_el_isIsolated_Tight.push_back( -1 ); }
      if ( isIsoGradientAcc.isAvailable( *el_itr ) )       { m_el_isIsolated_Gradient.push_back( isIsoGradientAcc( *el_itr ) ); } else { m_el_isIsolated_Gradient.push_back( -1 ); }
      if ( isIsoGradientLooseAcc.isAvailable( *el_itr ) )  { m_el_isIsolated_GradientLoose.push_back( isIsoGradientLooseAcc( *el_itr ) ); } else { m_el_isIsolated_GradientLoose.push_back( -1 ); }
      if ( isIsoFixedCutLooseAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_FixedCutLoose.push_back( isIsoFixedCutLooseAcc( *el_itr ) ); } else { m_el_isIsolated_FixedCutLoose.push_back( -1 ); }
      if ( isIsoFixedCutTightAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_FixedCutTight.push_back( isIsoFixedCutTightAcc( *el_itr ) ); } else { m_el_isIsolated_FixedCutTight.push_back( -1 ); }
      if ( isIsoFixedCutTightTrackOnlyAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_FixedCutTightTrackOnly.push_back( isIsoFixedCutTightTrackOnlyAcc( *el_itr ) ); } else { m_el_isIsolated_FixedCutTightTrackOnly.push_back( -1 ); }
      if ( isIsoUserDefinedFixEfficiencyAcc.isAvailable( *el_itr ) ) { m_el_isIsolated_UserDefinedFixEfficiency.push_back( isIsoUserDefinedFixEfficiencyAcc( *el_itr ) ); } else { m_el_isIsolated_UserDefinedFixEfficiency.push_back( -1 ); }
      if ( isIsoUserDefinedCutAcc.isAvailable( *el_itr ) )           { m_el_isIsolated_UserDefinedCut.push_back( isIsoUserDefinedCutAcc( *el_itr ) ); } else { m_el_isIsolated_UserDefinedCut.push_back( -1 ); }

      m_el_etcone20.push_back( el_itr->isolation( xAOD::Iso::etcone20 ) );
      m_el_ptcone20.push_back( el_itr->isolation( xAOD::Iso::ptcone20 ) );
      m_el_ptcone30.push_back( el_itr->isolation( xAOD::Iso::ptcone30 ) );
      m_el_ptcone40.push_back( el_itr->isolation( xAOD::Iso::ptcone40 ) );
      m_el_ptvarcone20.push_back( el_itr->isolation( xAOD::Iso::ptvarcone20 ) );
      m_el_ptvarcone30.push_back( el_itr->isolation( xAOD::Iso::ptvarcone30 ) );
      m_el_ptvarcone40.push_back( el_itr->isolation( xAOD::Iso::ptvarcone40 ) );
      m_el_topoetcone20.push_back( el_itr->isolation( xAOD::Iso::topoetcone20 ) );
      m_el_topoetcone30.push_back( el_itr->isolation( xAOD::Iso::topoetcone30 ) );
      m_el_topoetcone40.push_back( el_itr->isolation( xAOD::Iso::topoetcone40 ) );
    }

    if ( m_elInfoSwitch->m_PID ) {

      static SG::AuxElement::Accessor<char> LHLooseAcc ("LHLoose");
      static SG::AuxElement::Accessor<char> LHMediumAcc ("LHMedium");
      static SG::AuxElement::Accessor<char> LHTightAcc ("LHTight");

      static SG::AuxElement::Accessor<char> EMLooseAcc ("Loose");
      static SG::AuxElement::Accessor<char> EMMediumAcc ("Medium");
      static SG::AuxElement::Accessor<char> EMTightAcc ("Tight");

      if ( LHLooseAcc.isAvailable( *el_itr ) ) {
        m_el_LHLoose.push_back( LHLooseAcc( *el_itr ) );
        if ( LHLooseAcc( *el_itr ) == 1 ) { ++m_nel_LHLoose; }
      }  else { m_el_LHLoose.push_back( -1 ); }
      if ( LHMediumAcc.isAvailable( *el_itr ) ) {
        m_el_LHMedium.push_back( LHMediumAcc( *el_itr ) );
        if ( LHMediumAcc( *el_itr ) == 1 ) { ++m_nel_LHMedium; }
      }  else { m_el_LHMedium.push_back( -1 ); }
      if ( LHTightAcc.isAvailable( *el_itr ) ) {
        m_el_LHTight.push_back( LHTightAcc( *el_itr ) );
        if ( LHTightAcc( *el_itr ) == 1 ) { ++m_nel_LHTight; }
      } else { m_el_LHTight.push_back( -1 ); }

      if ( EMLooseAcc.isAvailable( *el_itr ) ) {
        m_el_IsEMLoose.push_back( EMLooseAcc( *el_itr ) );
        if ( EMLooseAcc( *el_itr ) == 1 ) { ++m_nel_IsEMLoose; }
      } else { m_el_IsEMLoose.push_back( -1 ); }
      if ( EMMediumAcc.isAvailable( *el_itr ) ) {
        m_el_IsEMMedium.push_back( EMMediumAcc( *el_itr ) );
	if ( EMMediumAcc( *el_itr ) == 1 ) { ++m_nel_IsEMMedium; }
      } else { m_el_IsEMMedium.push_back( -1 ); }
      if ( EMTightAcc.isAvailable( *el_itr ) ) {
        m_el_IsEMTight.push_back( EMTightAcc( *el_itr ) );
	if ( EMTightAcc( *el_itr ) == 1 ) { ++m_nel_IsEMTight; }
      } else { m_el_IsEMTight.push_back( -1 ); }

    }

    if ( m_elInfoSwitch->m_trackparams ) {
      if ( trk ) {
        //
	// NB.:
	// All track parameters are calculated at the perigee, i.e., the point of closest approach to the origin of some r.f. (which in RunII is NOT the ATLAS detector r.f!).
	// The reference  frame is chosen to be a system centered in the beamspot position, with z axis parallel to the beam line.
	// Remember that the beamspot size ( of O(10 micrometers) in the transverse plane) is << average vertex transverse position resolution ( O(60-80 micrometers) )
	// The coordinates of this r.f. wrt. the ATLAS system origin are returned by means of vx(), vy(), vz()
	//
        m_el_trkd0.push_back( trk->d0() );
        static SG::AuxElement::Accessor<float> d0SigAcc ("d0sig");
        float d0_significance =  ( d0SigAcc.isAvailable( *el_itr ) ) ? fabs( d0SigAcc( *el_itr ) ) : -1.0;
	m_el_trkd0sig.push_back( d0_significance );
        m_el_trkz0.push_back( trk->z0()  - ( primaryVertex->z() - trk->vz() ) );
        static SG::AuxElement::Accessor<float> z0sinthetaAcc("z0sintheta");
        float z0sintheta =  ( z0sinthetaAcc.isAvailable( *el_itr ) ) ? z0sinthetaAcc( *el_itr ) : -999.0;
        m_el_trkz0sintheta.push_back( z0sintheta );
        m_el_trkphi0.push_back( trk->phi0() );
        m_el_trktheta.push_back( trk->theta() );
        m_el_trkcharge.push_back( trk->charge() );
        m_el_trkqOverP.push_back( trk->qOverP() );
      } else {
        m_el_trkd0.push_back( -999.0 );
        m_el_trkd0sig.push_back( -1.0 );
        m_el_trkz0.push_back( -999.0 );
        m_el_trkz0sintheta.push_back( -999.0 );
        m_el_trkphi0.push_back( -999.0 );
        m_el_trktheta.push_back( -999.0 );
        m_el_trkcharge.push_back( -999.0 );
        m_el_trkqOverP.push_back( -999.0 );
      }
    }

    if ( m_elInfoSwitch->m_trackhitcont ) {
      uint8_t nPixHits(-1), nPixHoles(-1), nSCTHits(-1), nSCTHoles(-1), nTRTHits(-1), nTRTHoles(-1), nBLayerHits(-1), nInnermostPixLayHits(-1);
      float pixdEdX(-1.0);
      if ( trk ) {
        trk->summaryValue( nPixHits,  xAOD::numberOfPixelHits );
        trk->summaryValue( nPixHoles, xAOD::numberOfPixelHoles );
        trk->summaryValue( nSCTHits,  xAOD::numberOfSCTHits );
        trk->summaryValue( nSCTHoles, xAOD::numberOfSCTHoles );
        trk->summaryValue( nTRTHits,  xAOD::numberOfTRTHits );
        trk->summaryValue( nTRTHoles, xAOD::numberOfTRTHoles );
        trk->summaryValue( nBLayerHits,  xAOD::numberOfBLayerHits );
	      if ( !m_DC14 ) {
          trk->summaryValue( nInnermostPixLayHits, xAOD::numberOfInnermostPixelLayerHits );
          trk->summaryValue( pixdEdX,   xAOD::pixeldEdx);
        }
      }
      m_el_trknSiHits.push_back( nPixHits + nSCTHits );
      m_el_trknPixHits.push_back( nPixHits );
      m_el_trknPixHoles.push_back( nPixHoles );
      m_el_trknSCTHits.push_back( nSCTHits );
      m_el_trknSCTHoles.push_back( nSCTHoles );
      m_el_trknTRTHits.push_back( nTRTHits );
      m_el_trknTRTHoles.push_back( nTRTHoles );
      m_el_trknBLayerHits.push_back( nBLayerHits );
      if ( !m_DC14 ) {
        m_el_trknInnermostPixLayHits.push_back( nInnermostPixLayHits );
        m_el_trkPixdEdX.push_back( pixdEdX );
      }
    }

    if ( m_elInfoSwitch->m_effSF && m_isMC ) {

      static SG::AuxElement::Accessor< std::vector< float > > accRecoSF("ElectronEfficiencyCorrector_RecoSyst");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigSF_LHLooseAndBLayer("ElectronEfficiencyCorrector_TrigSyst_LHLooseAndBLayer");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigSF_LHTight("ElectronEfficiencyCorrector_TrigSyst_LHTight");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigMCEff_LHLooseAndBLayer("ElectronEfficiencyCorrector_TrigMCEffSyst_LHLooseAndBLayer");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigMCEff_LHTight("ElectronEfficiencyCorrector_TrigMCEffSyst_LHTight");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_Loose("ElectronEfficiencyCorrector_IsoSyst_IsoLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_FixedCutTight("ElectronEfficiencyCorrector_IsoSyst_IsoFixedCutTight");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHLooseAndBLayer("ElectronEfficiencyCorrector_PIDSyst_LHLooseAndBLayer");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHLoose("ElectronEfficiencyCorrector_PIDSyst_LHLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHMedium("ElectronEfficiencyCorrector_PIDSyst_LHMedium");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHTight("ElectronEfficiencyCorrector_PIDSyst_LHTight");

      std::vector<float> junkSF(1,1.0);
      std::vector<float> junkEff(1,0.0);

      if( accRecoSF.isAvailable( *el_itr ) )                     { m_el_RecoEff_SF.push_back( accRecoSF( *el_itr ) ); } else { m_el_RecoEff_SF.push_back( junkSF ); }
      if( accTrigSF_LHLooseAndBLayer.isAvailable( *el_itr ) )    { m_el_TrigEff_SF_LHLooseAndBLayer.push_back( accTrigSF_LHLooseAndBLayer( *el_itr ) ); } else { m_el_TrigEff_SF_LHLooseAndBLayer.push_back( junkSF ); }
      if( accTrigSF_LHTight.isAvailable( *el_itr ) )             { m_el_TrigEff_SF_LHTight.push_back( accTrigSF_LHTight( *el_itr ) ); } else { m_el_TrigEff_SF_LHTight.push_back( junkSF ); }
      if( accTrigMCEff_LHLooseAndBLayer.isAvailable( *el_itr ) ) { m_el_TrigMCEff_LHLooseAndBLayer.push_back( accTrigMCEff_LHLooseAndBLayer( *el_itr ) ); } else { m_el_TrigMCEff_LHLooseAndBLayer.push_back( junkEff ); }
      if( accTrigMCEff_LHTight.isAvailable( *el_itr ) )          { m_el_TrigMCEff_LHTight.push_back( accTrigMCEff_LHTight( *el_itr ) ); } else { m_el_TrigMCEff_LHTight.push_back( junkEff ); }
      if( accIsoSF_Loose.isAvailable( *el_itr ) )                { m_el_IsoEff_SF_Loose.push_back( accIsoSF_Loose( *el_itr ) ); } else { m_el_IsoEff_SF_Loose.push_back( junkSF ); }
      if( accIsoSF_FixedCutTight.isAvailable( *el_itr ) )        { m_el_IsoEff_SF_FixedCutTight.push_back( accIsoSF_FixedCutTight( *el_itr ) ); } else { m_el_IsoEff_SF_FixedCutTight.push_back( junkSF ); }
      if( accPIDSF_LHLooseAndBLayer.isAvailable( *el_itr ) )     { m_el_PIDEff_SF_LHLooseAndBLayer.push_back( accPIDSF_LHLooseAndBLayer( *el_itr ) ); } else { m_el_PIDEff_SF_LHLooseAndBLayer.push_back( junkSF ); }
      if( accPIDSF_LHLoose.isAvailable( *el_itr ) )              { m_el_PIDEff_SF_LHLoose.push_back( accPIDSF_LHLoose( *el_itr ) ); } else { m_el_PIDEff_SF_LHLoose.push_back( junkSF ); }
      if( accPIDSF_LHMedium.isAvailable( *el_itr ) )             { m_el_PIDEff_SF_LHMedium.push_back( accPIDSF_LHMedium( *el_itr ) ); } else { m_el_PIDEff_SF_LHMedium.push_back( junkSF ); }
      if( accPIDSF_LHTight.isAvailable( *el_itr ) )              { m_el_PIDEff_SF_LHTight.push_back( accPIDSF_LHTight( *el_itr ) ); } else { m_el_PIDEff_SF_LHTight.push_back( junkSF ); }

    }

    this->FillElectronsUser(el_itr);

    m_nel++;
  }
}

void HelpTreeBase::ClearElectrons() {

  m_nel = 0;

  if ( m_elInfoSwitch->m_kinematic ){
    m_el_pt.clear();
    m_el_eta.clear();
    m_el_phi.clear();
    m_el_m.clear();
    m_el_caloCluster_eta.clear();
  }

  if ( m_elInfoSwitch->m_trigger ) {
    m_el_isTrigMatched.clear();
    m_el_isTrigMatchedToChain.clear();
    m_el_listTrigChains.clear();
  }

  if ( m_elInfoSwitch->m_isolation ) {
    m_el_isIsolated_LooseTrackOnly.clear();
    m_el_isIsolated_Loose.clear();
    m_el_isIsolated_Tight.clear();
    m_el_isIsolated_Gradient.clear();
    m_el_isIsolated_GradientLoose.clear();
    m_el_isIsolated_FixedCutLoose.clear();
    m_el_isIsolated_FixedCutTight.clear();
    m_el_isIsolated_FixedCutTightTrackOnly.clear();
    m_el_isIsolated_UserDefinedFixEfficiency.clear();
    m_el_isIsolated_UserDefinedCut.clear();
    m_el_etcone20.clear();
    m_el_ptcone20.clear();
    m_el_ptcone30.clear();
    m_el_ptcone40.clear();
    m_el_ptvarcone20.clear();
    m_el_ptvarcone30.clear();
    m_el_ptvarcone40.clear();
    m_el_topoetcone20.clear();
    m_el_topoetcone30.clear();
    m_el_topoetcone40.clear();
  }

  if ( m_elInfoSwitch->m_PID ) {
    m_nel_LHLoose = 0;
    m_el_LHLoose.clear();
    m_nel_LHMedium = 0;
    m_el_LHMedium.clear();
    m_nel_LHTight = 0;
    m_el_LHTight.clear();
    m_nel_IsEMLoose = 0;
    m_el_IsEMLoose.clear();
    m_nel_IsEMMedium = 0;
    m_el_IsEMMedium.clear();
    m_nel_IsEMTight = 0;
    m_el_IsEMTight.clear();
  }

  if ( m_elInfoSwitch->m_trackparams ) {
    m_el_trkd0.clear();
    m_el_trkd0sig.clear();
    m_el_trkz0.clear();
    m_el_trkz0sintheta.clear();
    m_el_trkphi0.clear();
    m_el_trktheta.clear();
    m_el_trkcharge.clear();
    m_el_trkqOverP.clear();
  }

  if ( m_elInfoSwitch->m_trackhitcont ) {
    m_el_trknSiHits.clear();
    m_el_trknPixHits.clear();
    m_el_trknPixHoles.clear();
    m_el_trknSCTHits.clear();
    m_el_trknSCTHoles.clear();
    m_el_trknTRTHits.clear();
    m_el_trknTRTHoles.clear();
    m_el_trknBLayerHits.clear();
    if ( !m_DC14 ) {
      m_el_trknInnermostPixLayHits.clear();
      m_el_trkPixdEdX.clear();
    }
  }

  if( m_elInfoSwitch->m_effSF && m_isMC ) {
    m_el_RecoEff_SF.clear();
    m_el_TrigEff_SF_LHLooseAndBLayer.clear();
    m_el_TrigEff_SF_LHTight.clear();
    m_el_TrigMCEff_LHLooseAndBLayer.clear();
    m_el_TrigMCEff_LHTight.clear();
    m_el_IsoEff_SF_Loose.clear();
    m_el_IsoEff_SF_FixedCutTight.clear();
    m_el_PIDEff_SF_LHLooseAndBLayer.clear();
    m_el_PIDEff_SF_LHLoose.clear();
    m_el_PIDEff_SF_LHMedium.clear();
    m_el_PIDEff_SF_LHTight.clear();
  }
}

/*********************
 *
 *   PHOTONS
 *
 ********************/

void HelpTreeBase::AddPhotons(const std::string detailStr) {

  if(m_debug)  Info("AddPhotons()", "Adding photon variables: %s", detailStr.c_str());

  m_phInfoSwitch = new HelperClasses::PhotonInfoSwitch( detailStr );

  // always
  m_tree->Branch("nph",    &m_nph,"nph/I");

  if ( m_phInfoSwitch->m_kinematic ) {
    m_tree->Branch("ph_pt",  &m_ph_pt);
    m_tree->Branch("ph_phi", &m_ph_phi);
    m_tree->Branch("ph_eta", &m_ph_eta);
    m_tree->Branch("ph_m",   &m_ph_m);
    m_tree->Branch("ph_E",   &m_ph_E);
  }

  if ( m_phInfoSwitch->m_isolation ) {

    m_tree->Branch("ph_isIsolated_Cone40CaloOnly", &m_ph_isIsolated_Cone40CaloOnly);
    m_tree->Branch("ph_isIsolated_Cone40",         &m_ph_isIsolated_Cone40);
    m_tree->Branch("ph_isIsolated_Cone20",         &m_ph_isIsolated_Cone20);

    //m_tree->Branch("ph_etcone20",         &m_ph_etcone20);
    m_tree->Branch("ph_ptcone20",         &m_ph_ptcone20);
    m_tree->Branch("ph_ptcone30",         &m_ph_ptcone30);
    m_tree->Branch("ph_ptcone40",         &m_ph_ptcone40);
    m_tree->Branch("ph_ptvarcone20",      &m_ph_ptvarcone20);
    m_tree->Branch("ph_ptvarcone30",      &m_ph_ptvarcone30);
    m_tree->Branch("ph_ptvarcone40",      &m_ph_ptvarcone40);
    m_tree->Branch("ph_topoetcone20",     &m_ph_topoetcone20);
    m_tree->Branch("ph_topoetcone30",     &m_ph_topoetcone30);
    m_tree->Branch("ph_topoetcone40",     &m_ph_topoetcone40);
  }

  if ( m_phInfoSwitch->m_PID ) {
    m_tree->Branch("nph_IsLoose",    &m_nph_IsLoose);
    m_tree->Branch("ph_IsLoose",     &m_ph_IsLoose);
    m_tree->Branch("nph_IsMedium",   &m_nph_IsMedium);
    m_tree->Branch("ph_IsMedium",    &m_ph_IsMedium);
    m_tree->Branch("nph_IsTight",    &m_nph_IsTight);
    m_tree->Branch("ph_IsTight",     &m_ph_IsTight);
  }

  this->AddPhotonsUser();
}

void HelpTreeBase::FillPhotons( const xAOD::PhotonContainer* photons ) {

  this->ClearPhotons();
  this->ClearPhotonsUser();

  m_nph = 0;

  for ( auto ph_itr : *(photons) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillPhotons()", "Filling photon w/ pT = %2f", ph_itr->pt() / m_units ); }

    if ( m_phInfoSwitch->m_kinematic ) {
      m_ph_pt.push_back ( (ph_itr)->pt() / m_units );
      m_ph_eta.push_back( (ph_itr)->eta() );
      m_ph_phi.push_back( (ph_itr)->phi() );
      m_ph_m.push_back  ( (ph_itr)->m()  / m_units );
      m_ph_E.push_back  ( (ph_itr)->e()  / m_units );
    }

    if ( m_phInfoSwitch->m_isolation ) {

      static SG::AuxElement::Accessor<char> isIsoCone40CaloOnlyAcc ("isIsolated_Cone40CaloOnly");
      static SG::AuxElement::Accessor<char> isIsoCone40Acc         ("isIsolated_Cone40");
      static SG::AuxElement::Accessor<char> isIsoCone20Acc         ("isIsolated_Cone20");

      if ( isIsoCone40CaloOnlyAcc.isAvailable( *ph_itr ) ) {
	m_ph_isIsolated_Cone40CaloOnly.push_back( isIsoCone40CaloOnlyAcc( *ph_itr ) );
      } else {
	m_ph_isIsolated_Cone40CaloOnly.push_back( -1 );
      }

      if ( isIsoCone40Acc.isAvailable( *ph_itr ) ) {
	m_ph_isIsolated_Cone40.push_back( isIsoCone40Acc( *ph_itr ) );
      } else {
	m_ph_isIsolated_Cone40.push_back( -1 );
      }

      if ( isIsoCone20Acc.isAvailable( *ph_itr ) ) {
	m_ph_isIsolated_Cone20.push_back( isIsoCone20Acc( *ph_itr ) );
      } else {
	m_ph_isIsolated_Cone20.push_back( -1 );
      }

      //m_ph_etcone20    .push_back( ph_itr->isolation( xAOD::Iso::etcone20    ) / m_units  );
      m_ph_ptcone20    .push_back( ph_itr->isolation( xAOD::Iso::ptcone20    ) / m_units  );
      m_ph_ptcone30    .push_back( ph_itr->isolation( xAOD::Iso::ptcone30    ) / m_units  );
      m_ph_ptcone40    .push_back( ph_itr->isolation( xAOD::Iso::ptcone40    ) / m_units  );
      m_ph_ptvarcone20 .push_back( ph_itr->isolation( xAOD::Iso::ptvarcone20 ) / m_units  );
      m_ph_ptvarcone30 .push_back( ph_itr->isolation( xAOD::Iso::ptvarcone30 ) / m_units  );
      m_ph_ptvarcone40 .push_back( ph_itr->isolation( xAOD::Iso::ptvarcone40 ) / m_units  );
      m_ph_topoetcone20.push_back( ph_itr->isolation( xAOD::Iso::topoetcone20) / m_units  );
      m_ph_topoetcone30.push_back( ph_itr->isolation( xAOD::Iso::topoetcone30) / m_units  );
      m_ph_topoetcone40.push_back( ph_itr->isolation( xAOD::Iso::topoetcone40) / m_units  );

    }

    if ( m_phInfoSwitch->m_PID ) {
      static SG::AuxElement::Accessor<bool> phLooseAcc  ("PhotonID_Loose");
      static SG::AuxElement::Accessor<bool> phMediumAcc ("PhotonID_Medium");
      static SG::AuxElement::Accessor<bool> phTightAcc  ("PhotonID_Tight");

      if ( phLooseAcc.isAvailable( *ph_itr ) ) {
        m_ph_IsLoose.push_back( phLooseAcc( *ph_itr ) );
	if ( phLooseAcc( *ph_itr ) == 1 ) { ++m_nph_IsLoose; }
      } else { m_ph_IsLoose.push_back( -1 ); }

      if ( phMediumAcc.isAvailable( *ph_itr ) ) {
        m_ph_IsMedium.push_back( phMediumAcc( *ph_itr ) );
	if ( phMediumAcc( *ph_itr ) == 1 ) { ++m_nph_IsMedium; }
      } else { m_ph_IsMedium.push_back( -1 ); }

      if ( phTightAcc.isAvailable( *ph_itr ) ) {
        m_ph_IsTight.push_back( phTightAcc( *ph_itr ) );
	if ( phTightAcc( *ph_itr ) == 1 ) { ++m_nph_IsTight; }
      } else { m_ph_IsTight.push_back( -1 ); }

    }

    this->FillPhotonsUser(ph_itr);

    m_nph++;
  }
}

void HelpTreeBase::ClearPhotons() {

  m_nph = 0;

  if ( m_phInfoSwitch->m_kinematic ){
    m_ph_pt.clear();
    m_ph_eta.clear();
    m_ph_phi.clear();
    m_ph_m.clear();
    m_ph_E.clear();
  }


  if ( m_phInfoSwitch->m_isolation ){
    m_ph_isIsolated_Cone40CaloOnly.clear();
    m_ph_isIsolated_Cone40.clear();
    m_ph_isIsolated_Cone20.clear();
    //m_ph_etcone20.clear();
    m_ph_ptcone20.clear();
    m_ph_ptcone30.clear();
    m_ph_ptcone40.clear();
    m_ph_ptvarcone20.clear();
    m_ph_ptvarcone30.clear();
    m_ph_ptvarcone40.clear();
    m_ph_topoetcone20.clear();
    m_ph_topoetcone30.clear();
    m_ph_topoetcone40.clear();
  }


  if ( m_phInfoSwitch->m_PID ) {
    m_nph_IsLoose = 0;
    m_ph_IsLoose.clear();
    m_nph_IsMedium = 0;
    m_ph_IsMedium.clear();
    m_nph_IsTight = 0;
    m_ph_IsTight.clear();
  }

}

/*********************
 *
 *   JETS
 *
 ********************/

void HelpTreeBase::AddJets(const std::string detailStr, const std::string jetName)
{

  if(m_debug) Info("AddJets()", "Adding jet %s with variables: %s", jetName.c_str(), detailStr.c_str());

  m_jets[jetName] = new xAH::JetContainer(jetName, detailStr, m_units);

  xAH::JetContainer* thisJet = m_jets[jetName];
  thisJet->setBranches(m_tree);
  this->AddJetsUser(detailStr, jetName);

}


void HelpTreeBase::FillJets( const xAOD::JetContainer* jets, int pvLocation, const std::string jetName ) {

  this->ClearJets(jetName);

  const xAOD::VertexContainer* vertices(nullptr);
  const xAOD::Vertex *pv = 0;

  xAH::JetContainer* thisJet = m_jets[jetName];

  if( thisJet->m_infoSwitch.m_trackPV || thisJet->m_infoSwitch.m_allTrack ) {
    HelperFunctions::retrieve( vertices, "PrimaryVertices", m_event, 0 );
    pvLocation = HelperFunctions::getPrimaryVertexLocation( vertices );
    if ( pvLocation >= 0 ) pv = vertices->at( pvLocation );
  }



  // Global event BTag SF weight (--> the product of each object's weight)
  //
  if ( m_isMC ) {
    const xAOD::EventInfo* eventInfo(nullptr);
    HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, false);

    /*
    if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFix.empty() ) {
      for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFix.size(); i++ ) {
    	switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFix.at(i) ) {
    	  case 30 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix30_GLOBAL("BTag_SF_FixedCutBEff_30_GLOBAL");
            if ( sfFix30_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix30 = sfFix30_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix30.push_back(-999.0); }
    	    break;
    	  case 50 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix50_GLOBAL("BTag_SF_FixedCutBEff_50_GLOBAL");
            if ( sfFix50_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix50 = sfFix50_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix50.push_back(-999.0); }
    	    break;
    	  case 60 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix60_GLOBAL("BTag_SF_FixedCutBEff_60_GLOBAL");
            if ( sfFix60_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix60 = sfFix60_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix60.push_back(-999.0); }
    	    break;
    	  case 70 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix70_GLOBAL("BTag_SF_FixedCutBEff_70_GLOBAL");
            if ( sfFix70_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix70 = sfFix70_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix70.push_back(-999.0); }
    	    break;
    	  case 77 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix77_GLOBAL("BTag_SF_FixedCutBEff_77_GLOBAL");
            if ( sfFix77_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix77 = sfFix77_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix77.push_back(-999.0); }
    	    break;
    	  case 80 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix80_GLOBAL("BTag_SF_FixedCutBEff_80_GLOBAL");
            if ( sfFix80_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix80 = sfFix80_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix80.push_back(-999.0); }
    	    break;
    	  case 85 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix85_GLOBAL("BTag_SF_FixedCutBEff_85_GLOBAL");
            if ( sfFix85_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix85 = sfFix85_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix85.push_back(-999.0); }
    	    break;
    	  case 90 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix90_GLOBAL("BTag_SF_FixedCutBEff_90_GLOBAL");
            if ( sfFix90_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFix90 = sfFix90_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFix90.push_back(-999.0); }
    	    break;
    	}
      }
    } // sfFTagFix

    if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.empty() ) {
      for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.size(); i++ ) {
    	switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.at(i) ) {
    	  case 30 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt30_GLOBAL("BTag_SF_FlatBEff_30_GLOBAL");
            if ( sfFlt30_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFlt30 = sfFlt30_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFlt30.push_back(-999.0); }
    	    break;
    	  case 40 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt40_GLOBAL("BTag_SF_FlatBEff_40_GLOBAL");
            if ( sfFlt40_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFlt40 = sfFlt40_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFlt40.push_back(-999.0); }
    	    break;
    	  case 50 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt50_GLOBAL("BTag_SF_FlatBEff_50_GLOBAL");
            if ( sfFlt50_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFlt50 = sfFlt50_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFlt50.push_back(-999.0); }
    	    break;
    	  case 60 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt60_GLOBAL("BTag_SF_FlatBEff_60_GLOBAL");
            if ( sfFlt60_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFlt60 = sfFlt60_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFlt60.push_back(-999.0); }
    	    break;
    	  case 70 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt70_GLOBAL("BTag_SF_FlatBEff_70_GLOBAL");
            if ( sfFlt70_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFlt70 = sfFlt70_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFlt70.push_back(-999.0); }
    	    break;
    	  case 77 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt77_GLOBAL("BTag_SF_FlatBEff_77_GLOBAL");
            if ( sfFlt77_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFlt77 = sfFlt77_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFlt77.push_back(-999.0); }
    	    break;
    	  case 85 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt85_GLOBAL("BTag_SF_FlatBEff_85_GLOBAL");
            if ( sfFlt85_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_sfFlt85 = sfFlt85_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_sfFlt85.push_back(-999.0); }
    	    break;
    	}
      }
    } // sfFTagFlt
    */
  }

  for( auto jet_itr : *jets ) {
    this->FillJet(jet_itr, pv, pvLocation, jetName);
  }

}



void HelpTreeBase::FillJet( const xAOD::Jet* jet_itr, const xAOD::Vertex* pv, int pvLocation, const std::string jetName ) {

  xAH::JetContainer* thisJet = m_jets[jetName];
  
  thisJet->FillJet(jet_itr, pv, pvLocation);

  this->FillJetsUser(jet_itr, jetName);

  return;
}

void HelpTreeBase::ClearJets(const std::string jetName) {

  xAH::JetContainer* thisJet = m_jets[jetName];
  thisJet->clear();

  this->ClearJetsUser(jetName);

}

/*********************
 *
 *   TRUTH
 *
 ********************/

void HelpTreeBase::AddTruthParts(const std::string truthName, const std::string detailStr)
{

  if(m_debug) Info("AddTruthParts()", "Adding truth particle %s with variables: %s", truthName.c_str(), detailStr.c_str());

  m_truthInfoSwitch = new HelperClasses::TruthInfoSwitch( detailStr );

  m_truth[truthName] = new truthInfo();

  // always
  m_tree->Branch(("n"+truthName).c_str(),    &m_truth[truthName]->N, ("n"+truthName+"/I").c_str());

  if ( m_truthInfoSwitch->m_kinematic ) {
    m_tree->Branch((truthName+"_E"  ).c_str(), &m_truth[truthName]->E);
    m_tree->Branch((truthName+"_pt" ).c_str(), &m_truth[truthName]->pt);
    m_tree->Branch((truthName+"_phi").c_str(), &m_truth[truthName]->phi);
    m_tree->Branch((truthName+"_eta").c_str(), &m_truth[truthName]->eta);
  }

  this->AddTruthUser(truthName);
}

void HelpTreeBase::FillTruth( const std::string truthName, const xAOD::TruthParticleContainer* truthParts ) {

  this->ClearTruth(truthName);
  this->ClearTruthUser(truthName);

  for( auto truth_itr : *truthParts ) {

    if( m_truthInfoSwitch->m_kinematic ){
      m_truth[truthName]->pt .push_back  ( truth_itr->pt() / m_units );
      m_truth[truthName]->eta.push_back  ( truth_itr->eta() );
      m_truth[truthName]->phi.push_back  ( truth_itr->phi() );
      m_truth[truthName]->E  .push_back  ( truth_itr->e() / m_units );
    }

    this->FillTruthUser(truthName, truth_itr);

    m_truth[truthName]->N++;

  } // loop over Truth

}

void HelpTreeBase::ClearTruth(const std::string truthName) {

  m_truth[truthName]->N = 0;
  if( m_truthInfoSwitch->m_kinematic ){
    m_truth[truthName]->pt.clear();
    m_truth[truthName]->eta.clear();
    m_truth[truthName]->phi.clear();
    m_truth[truthName]->E.clear();
  }

}


/*********************
 *
 *   FAT JETS
 *
 ********************/

void HelpTreeBase::AddFatJets(std::string detailStr) {

  if(m_debug) Info("AddFatJets()", "Adding fat jet variables: %s", detailStr.c_str());

  m_fatJetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );

  // always
  m_tree->Branch("nfatjets",    &m_nfatjet,"nfatjets/I");

  if ( m_fatJetInfoSwitch->m_kinematic ) {
    m_tree->Branch("fatjet_E",   &m_fatjet_E);
    m_tree->Branch("fatjet_m",   &m_fatjet_m);
    m_tree->Branch("fatjet_pt",  &m_fatjet_pt);
    m_tree->Branch("fatjet_phi", &m_fatjet_phi);
    m_tree->Branch("fatjet_eta", &m_fatjet_eta);
  }
  if ( m_fatJetInfoSwitch->m_substructure ) {

    m_tree->Branch("fatjet_Split12",&m_fatjet_Split12);
    m_tree->Branch("fatjet_Split23",&m_fatjet_Split23);
    m_tree->Branch("fatjet_Split34",&m_fatjet_Split34);
    m_tree->Branch("fatjet_tau1_wta",&m_fatjet_tau1_wta);
    m_tree->Branch("fatjet_tau2_wta",&m_fatjet_tau2_wta);
    m_tree->Branch("fatjet_tau3_wta",&m_fatjet_tau3_wta);
    m_tree->Branch("fatjet_tau21_wta",   &m_fatjet_tau21_wta);    
    m_tree->Branch("fatjet_tau32_wta",   &m_fatjet_tau32_wta);
    m_tree->Branch("fatjet_ECF1",&m_fatjet_ECF1);
    m_tree->Branch("fatjet_ECF2",&m_fatjet_ECF2);
    m_tree->Branch("fatjet_ECF3",&m_fatjet_ECF3);
    m_tree->Branch("fatjet_C2",&m_fatjet_C2);
    m_tree->Branch("fatjet_D2",&m_fatjet_D2);
    m_tree->Branch("fatjet_NTrimSubjets",&m_fatjet_NTrimSubjets);
  }

  this->AddFatJetsUser();
}

void HelpTreeBase::FillFatJets( const xAOD::JetContainer* fatJets ) {
  this->ClearFatJets();
  this->ClearFatJetsUser();

  for( auto fatjet_itr : *fatJets ) {

    if( m_fatJetInfoSwitch->m_kinematic ){
      m_fatjet_pt.push_back ( fatjet_itr->pt() / m_units );
      m_fatjet_m.push_back ( fatjet_itr->m() / m_units );
      m_fatjet_eta.push_back( fatjet_itr->eta() );
      m_fatjet_phi.push_back( fatjet_itr->phi() );
      m_fatjet_E.push_back  ( fatjet_itr->e() / m_units );
    }
    if( m_fatJetInfoSwitch->m_substructure ){
      static SG::AuxElement::ConstAccessor<float> Split12("Split12");
      static SG::AuxElement::ConstAccessor<float> Split23("Split23");
      static SG::AuxElement::ConstAccessor<float> Split34("Split34");      
      if ( Split12.isAvailable( *fatjet_itr ) ) {
        m_fatjet_Split12.push_back( Split12( *fatjet_itr ) / m_units );
      } else { m_fatjet_Split12.push_back( -999 ); }
      if ( Split23.isAvailable( *fatjet_itr ) ) {
	m_fatjet_Split23.push_back(Split23( *fatjet_itr ) / m_units );
      } else { m_fatjet_Split34.push_back( -999 ); }
      if ( Split34.isAvailable( *fatjet_itr ) ) {
	m_fatjet_Split34.push_back( Split34( *fatjet_itr ) / m_units );
      } else { m_fatjet_Split34.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> tau1_wta ("Tau1_wta");
      static SG::AuxElement::ConstAccessor<float> tau2_wta ("Tau2_wta");
      static SG::AuxElement::ConstAccessor<float> tau3_wta ("Tau3_wta");
      static SG::AuxElement::ConstAccessor<float> tau21_wta ("Tau21_wta");
      static SG::AuxElement::ConstAccessor<float> tau32_wta ("Tau32_wta");

      if ( tau1_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau1_wta.push_back( tau1_wta( *fatjet_itr ) );
      } else { m_fatjet_tau1_wta.push_back( -999 ); }
      if ( tau2_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau2_wta.push_back( tau2_wta( *fatjet_itr ) );
      } else { m_fatjet_tau2_wta.push_back( -999 ); }
      if ( tau3_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau3_wta.push_back( tau3_wta( *fatjet_itr ) );
      } else { m_fatjet_tau3_wta.push_back( -999 ); }
      if(tau21_wta.isAvailable( *fatjet_itr )){
        m_fatjet_tau21_wta.push_back( tau21_wta( *fatjet_itr ) );
      } else if ( tau1_wta.isAvailable( *fatjet_itr ) and tau2_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau21_wta.push_back( tau2_wta( *fatjet_itr ) / tau1_wta( *fatjet_itr ) );
      } else { m_fatjet_tau21_wta.push_back( -999 ); }
      if(tau32_wta.isAvailable( *fatjet_itr )){
	m_fatjet_tau32_wta.push_back( tau32_wta( *fatjet_itr ) );
      } else if ( tau2_wta.isAvailable( *fatjet_itr ) and tau3_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau32_wta.push_back( tau3_wta( *fatjet_itr ) / tau2_wta( *fatjet_itr ) );
      } else { m_fatjet_tau32_wta.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> ECF1 ("ECF1");
      static SG::AuxElement::ConstAccessor<float> ECF2("ECF2");
      static SG::AuxElement::ConstAccessor<float> ECF3 ("ECF3");
      static SG::AuxElement::ConstAccessor<float> C2("C2");
      static SG::AuxElement::ConstAccessor<float> D2 ("D2");

      if(C2.isAvailable(*fatjet_itr)){
	m_fatjet_C2.push_back(C2(*fatjet_itr));
      } else if( ECF1.isAvailable(*fatjet_itr) && ECF2.isAvailable(*fatjet_itr) && ECF3.isAvailable(*fatjet_itr)){
      	m_fatjet_C2.push_back( ECF3(*fatjet_itr)*ECF1(*fatjet_itr)/pow(ECF2(*fatjet_itr),2.0));
      } else{ m_fatjet_C2.push_back(-999); }

      if( D2.isAvailable( *fatjet_itr ) ) {
	m_fatjet_D2.push_back( D2( *fatjet_itr ));
      } else if (ECF1.isAvailable( *fatjet_itr ) && ECF2.isAvailable( *fatjet_itr ) && ECF3.isAvailable( *fatjet_itr )){
	float e2=(ECF2( *fatjet_itr )/(ECF1( *fatjet_itr )*ECF1( *fatjet_itr )));
	float e3=(ECF3( *fatjet_itr )/(ECF1( *fatjet_itr )*ECF1( *fatjet_itr )*ECF1( *fatjet_itr )));
	m_fatjet_D2.push_back( e3/(e2*e2*e2) );
      } else{ m_fatjet_D2.push_back(-999); }

      if ( ECF1.isAvailable( *fatjet_itr ) ) {
	m_fatjet_ECF1.push_back( ECF1( *fatjet_itr ) / m_units);
      } else { m_fatjet_ECF1.push_back( -999 ); }
      if ( ECF2.isAvailable( *fatjet_itr ) ) {
	m_fatjet_ECF2.push_back( ECF2( *fatjet_itr ) / m_units);
      } else { m_fatjet_ECF2.push_back( -999 ); }
      if ( ECF3.isAvailable( *fatjet_itr ) ) {
	m_fatjet_ECF3.push_back( ECF3( *fatjet_itr ) / m_units);
      } else { m_fatjet_ECF3.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> NTrimSubjets("NTrimSubjets");
      if( NTrimSubjets.isAvailable(*fatjet_itr) ){
	m_fatjet_NTrimSubjets.push_back(NTrimSubjets(*fatjet_itr));
      } else{ m_fatjet_NTrimSubjets.push_back(-999); }
    }
    this->FillFatJetsUser(fatjet_itr);

    m_nfatjet++;

  } // loop over fat jets

}

void HelpTreeBase::ClearFatJets() {

  m_nfatjet = 0;
  if( m_fatJetInfoSwitch->m_kinematic ){
    m_fatjet_pt.clear();
    m_fatjet_eta.clear();
    m_fatjet_phi.clear();
    m_fatjet_E.clear();
    m_fatjet_m.clear();
  }
  if( m_fatJetInfoSwitch->m_substructure ){
    m_fatjet_Split12.clear();
    m_fatjet_Split23.clear();
    m_fatjet_Split34.clear();
    m_fatjet_tau1_wta.clear();
    m_fatjet_tau2_wta.clear();
    m_fatjet_tau3_wta.clear();
    m_fatjet_tau21_wta.clear();
    m_fatjet_tau32_wta.clear();
    m_fatjet_ECF1.clear();
    m_fatjet_ECF2.clear();
    m_fatjet_ECF3.clear();
    m_fatjet_C2.clear();
    m_fatjet_D2.clear();
    m_fatjet_NTrimSubjets.clear();
  }

}

void HelpTreeBase::ClearEvent() {
  m_runNumber = m_eventNumber = m_mcEventNumber = m_mcChannelNumber = m_bcid = m_lumiBlock;
  m_coreFlags = 0;
  //eventCleaning
  m_LArError = false;
  m_TileError = false;
  m_SCTError = false;
  m_LArFlags = 0;
  m_TileFlags = 0;
  m_SCTFlags = 0;
  m_mcEventWeight = 1.;
  m_weight_pileup = 1.;
  m_timeStamp = -999;
  m_timeStampNSOffset = -999;
  // pileup
  m_npv = -999;
  m_actualMu = m_averageMu = -999;
  // shapeEM
  m_rhoEM = -999;
  // shapeLC
  m_rhoLC = -999;
  // truth
  m_pdgId1 = m_pdgId2 = m_pdfId1 = m_pdfId2 = -999;
  m_x1 = m_x2 = -999;
  m_xf1 = m_xf2 = -999;

  //m_scale = m_q = m_pdf1 = m_pdf2 = -999;

  // CaloCluster
  if( m_eventInfoSwitch->m_caloClus){
    m_caloCluster_pt.clear();
    m_caloCluster_eta.clear();
    m_caloCluster_phi.clear();
    m_caloCluster_e.clear();
  }

}


/*********************
 *
 *   TAUS
 *
 ********************/

void HelpTreeBase::AddTaus(const std::string detailStr) {

  m_tauInfoSwitch = new HelperClasses::TauInfoSwitch( detailStr );

  // always
  m_tree->Branch("ntau",   &m_ntau, "ntau/I");

  if ( m_tauInfoSwitch->m_kinematic ) {
    m_tree->Branch("tau_pt",     &m_tau_pt);
    m_tree->Branch("tau_phi",    &m_tau_phi);
    m_tree->Branch("tau_eta",    &m_tau_eta);
    m_tree->Branch("tau_m",      &m_tau_m);
    m_tree->Branch("tau_ntrk",   &m_tau_ntrk);
    m_tree->Branch("tau_charge", &m_tau_charge);
  }

  this->AddTausUser();
}

void HelpTreeBase::FillTaus( const xAOD::TauJetContainer* taus ) {

  this->ClearTaus();
  this->ClearTausUser();

  m_ntau = 0;
  for ( auto tau_itr : *(taus) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTaus()", "Filling tau w/ pT = %2f", tau_itr->pt() / m_units ); }

    if ( m_tauInfoSwitch->m_kinematic ) {
      m_tau_pt.push_back ( tau_itr->pt() / m_units  );
      m_tau_eta.push_back( tau_itr->eta() );
      m_tau_phi.push_back( tau_itr->phi() );
      m_tau_m.push_back  ( tau_itr->m() / m_units  );
      m_tau_charge.push_back( tau_itr->charge() );
      m_tau_ntrk.push_back( tau_itr->nTracks() );
    }

    this->FillTausUser(tau_itr);

    m_ntau++;
  }
}

void HelpTreeBase::ClearTaus() {

  m_ntau = 0;

  if ( m_tauInfoSwitch->m_kinematic ){
    m_tau_pt.clear();
    m_tau_eta.clear();
    m_tau_phi.clear();
    m_tau_m.clear();
    m_tau_charge.clear();
    m_tau_ntrk.clear();
  }

}



/*********************
 *
 *     MET
 *
 ********************/
void HelpTreeBase::AddMET( const std::string detailStr ) {

  if(m_debug) Info("AddMET()", "Adding MET variables: %s", detailStr.c_str());

  m_metInfoSwitch = new HelperClasses::METInfoSwitch( detailStr );

  m_tree->Branch("metFinalClus",	 &m_metFinalClus,      "metFinalClus/F");
  m_tree->Branch("metFinalClusPx",       &m_metFinalClusPx,    "metFinalClusPx/F");
  m_tree->Branch("metFinalClusPy",       &m_metFinalClusPy,    "metFinalClusPy/F");
  m_tree->Branch("metFinalClusSumEt",    &m_metFinalClusSumEt, "metFinalClusSumEt/F");
  m_tree->Branch("metFinalClusPhi",      &m_metFinalClusPhi,   "metFinalClusPhi/F");

  m_tree->Branch("metFinalTrk",	         &m_metFinalTrk,       "metFinalTrk/F");
  m_tree->Branch("metFinalTrkPx",        &m_metFinalTrkPx,     "metFinalTrkPx/F");
  m_tree->Branch("metFinalTrkPy",        &m_metFinalTrkPy,     "metFinalTrkPy/F");
  m_tree->Branch("metFinalTrkSumEt",     &m_metFinalTrkSumEt,  "metFinalTrkSumEt/F");
  m_tree->Branch("metFinalTrkPhi",       &m_metFinalTrkPhi,    "metFinalTrkPhi/F");

  if ( m_metInfoSwitch->m_refEle ) {
    m_tree->Branch("metEle",             &m_metEle,	       "metEle/F");
    m_tree->Branch("metEleSumEt",        &m_metEleSumEt,       "metEleSumEt/F");
    m_tree->Branch("metElePhi",          &m_metElePhi,         "metElePhi/F");
  }
  if ( m_metInfoSwitch->m_refGamma ) {
    m_tree->Branch("metGamma",       	 &m_metGamma,	       "metGamma/F");
    m_tree->Branch("metGammaSumEt",  	 &m_metGammaSumEt,     "metGammaSumEt/F");
    m_tree->Branch("metGammaPhi",    	 &m_metGammaPhi,       "metGammaPhi/F");
  }
  if ( m_metInfoSwitch->m_refTau ) {
    m_tree->Branch("metTau",             &m_metTau,	       "metTau/F");
    m_tree->Branch("metTauSumEt",        &m_metTauSumEt,       "metTauSumEt/F");
    m_tree->Branch("metTauPhi",          &m_metTauPhi,         "metTauPhi/F");
  }
  if ( m_metInfoSwitch->m_refMuons ) {
    m_tree->Branch("metMuons",           &m_metMuons,	       "metMuons/F");
    m_tree->Branch("metMuonsSumEt",      &m_metMuonsSumEt,     "metMuonsSumEt/F");
    m_tree->Branch("metMuonsPhi",        &m_metMuonsPhi,       "metMuonsPhi/F");
  }
  if ( m_metInfoSwitch->m_refJet ) {
    m_tree->Branch("metJet",             &m_metJet,	       "metJet/F");
    m_tree->Branch("metJetSumEt",        &m_metJetSumEt,       "metJetSumEt/F");
    m_tree->Branch("metJetPhi",          &m_metJetPhi,         "metJetPhi/F");
  }
  if ( m_metInfoSwitch->m_refJetTrk ) {
    m_tree->Branch("metJetTrk",          &m_metJetTrk,         "metJetTrk/F");
    m_tree->Branch("metJetTrkSumEt",     &m_metJetTrkSumEt,    "metJetTrkSumEt/F");
    m_tree->Branch("metJetTrkPhi",       &m_metJetTrkPhi,      "metJetTrkPhi/F");
  }
  if ( m_metInfoSwitch->m_softClus) {
    m_tree->Branch("metSoftClus",        &m_metSoftClus,       "metSoftClus/F");
    m_tree->Branch("metSoftClusSumEt",   &m_metSoftClusSumEt,  "metSoftClusSumEt/F");
    m_tree->Branch("metSoftClusPhi",     &m_metSoftClusPhi,    "metSoftClussPhi/F");
  }
  if ( m_metInfoSwitch->m_softTrk) {
    m_tree->Branch("metSoftTrk",         &m_metSoftTrk,        "metSoftTrk/F");
    m_tree->Branch("metSoftTrkSumEt",    &m_metSoftTrkSumEt,   "metSoftTrkSumEt/F");
    m_tree->Branch("metSoftTrkPhi",      &m_metSoftTrkPhi,     "metSoftTrksPhi/F");
  }

  this->AddMETUser();
}

void HelpTreeBase::FillMET( const xAOD::MissingETContainer* met ) {

  // Clear previous events
  this->ClearMET();
  this->ClearMETUser();

  if ( m_debug ) { Info("HelpTreeBase::FillMET()", "Filling MET info"); }

  const xAOD::MissingET* final_clus = *met->find("FinalClus"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
  m_metFinalClus      = final_clus->met() / m_units;
  m_metFinalClusPx    = final_clus->mpx() / m_units;
  m_metFinalClusPy    = final_clus->mpy() / m_units;
  m_metFinalClusSumEt = final_clus->sumet() / m_units;
  m_metFinalClusPhi   = final_clus->phi();

  const xAOD::MissingET* final_trk = *met->find("FinalTrk"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
  m_metFinalTrk	      = final_trk->met() / m_units;
  m_metFinalTrkPx     = final_trk->mpx() / m_units;
  m_metFinalTrkPy     = final_trk->mpy() / m_units;
  m_metFinalTrkSumEt  = final_trk->sumet() / m_units;
  m_metFinalTrkPhi    = final_trk->phi();

  if ( m_metInfoSwitch->m_refEle ) {
    const xAOD::MissingET* ref_ele       = *met->find("RefEle");
    m_metEle         		         = ref_ele->met() / m_units;
    m_metEleSumEt    		         = ref_ele->sumet() / m_units;
    m_metElePhi      		         = ref_ele->phi();
  }
  if ( m_metInfoSwitch->m_refGamma ) {
    const xAOD::MissingET* ref_gamma     = *met->find("RefGamma");
    m_metGamma             	         = ref_gamma->met() / m_units;
    m_metGammaSumEt        	         = ref_gamma->sumet() / m_units;
    m_metGammaPhi          	         = ref_gamma->phi();
  }
  if ( m_metInfoSwitch->m_refTau ) {
    const xAOD::MissingET* ref_tau  	 = *met->find("RefTau");
    m_metTau             	    	 = ref_tau->met() / m_units;
    m_metTauSumEt        	    	 = ref_tau->sumet() / m_units;
    m_metTauPhi          	    	 = ref_tau->phi();
  }
  if ( m_metInfoSwitch->m_refMuons ) {
    const xAOD::MissingET* ref_muon  	 = *met->find("Muons");
    m_metMuons             	     	 = ref_muon->met() / m_units;
    m_metMuonsSumEt        	     	 = ref_muon->sumet() / m_units;
    m_metMuonsPhi          	     	 = ref_muon->phi();
  }
  if ( m_metInfoSwitch->m_refJet ) {
    const xAOD::MissingET* ref_jet  	 = *met->find("RefJet");
    m_metJet             	     	 = ref_jet->met() / m_units;
    m_metJetSumEt        	     	 = ref_jet->sumet() / m_units;
    m_metJetPhi          	     	 = ref_jet->phi();
  }
  if ( m_metInfoSwitch->m_refJetTrk ) {
    const xAOD::MissingET* ref_jet_trk   = *met->find("RefJetTrk");
    m_metJetTrk             	     	 = ref_jet_trk->met() / m_units;
    m_metJetTrkSumEt        	     	 = ref_jet_trk->sumet() / m_units;
    m_metJetTrkPhi          	     	 = ref_jet_trk->phi();
  }
  if ( m_metInfoSwitch->m_softClus) {
    const xAOD::MissingET* ref_soft_clus = *met->find("SoftClus");
    m_metSoftClus            		 = ref_soft_clus->met() / m_units;
    m_metSoftClusSumEt       		 = ref_soft_clus->sumet() / m_units;
    m_metSoftClusPhi         		 = ref_soft_clus->phi();
  }
  if ( m_metInfoSwitch->m_softTrk) {
    const xAOD::MissingET* ref_soft_trk  = *met->find("PVSoftTrk");
    m_metSoftTrk             		 = ref_soft_trk->met() / m_units;
    m_metSoftTrkSumEt        		 = ref_soft_trk->sumet() / m_units;
    m_metSoftTrkPhi          		 = ref_soft_trk->phi();
  }

  this->FillMETUser(met);
}

void HelpTreeBase::ClearMET() {

  m_metFinalClus      = -999;
  m_metFinalClusPx    = -999;
  m_metFinalClusPy    = -999;
  m_metFinalClusSumEt = -999;
  m_metFinalClusPhi   = -999;

  m_metFinalTrk	      = -999;
  m_metFinalTrkPx     = -999;
  m_metFinalTrkPy     = -999;
  m_metFinalTrkSumEt  = -999;
  m_metFinalTrkPhi    = -999;

  if ( m_metInfoSwitch->m_refEle ) {
    m_metEle = m_metEleSumEt = m_metElePhi = -999;
  }
  if ( m_metInfoSwitch->m_refGamma ) {
    m_metGamma = m_metGammaSumEt = m_metGammaPhi = -999;
  }
  if ( m_metInfoSwitch->m_refTau ) {
    m_metTau = m_metTauSumEt = m_metTauPhi = -999;
  }
  if ( m_metInfoSwitch->m_refMuons ) {
    m_metMuons = m_metMuonsSumEt = m_metMuonsPhi = -999;
  }
  if ( m_metInfoSwitch->m_refJet ) {
    m_metJet = m_metJetSumEt = m_metJetPhi = -999;
  }
  if ( m_metInfoSwitch->m_refJetTrk ) {
    m_metJetTrk = m_metJetTrkSumEt = m_metJetTrkPhi = -999;
  }
	  if ( m_metInfoSwitch->m_softClus) {
    m_metSoftClus = m_metSoftClusSumEt = m_metSoftClusPhi = -999;
  }
}


bool HelpTreeBase::writeTo( TFile* file ) {
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if ( status == 0 ) { return false; }
  return true;
}


//////////////////////////////////////////////////////////////
///  hiding some of the explicit things for b-tagging here ///
///        it's ugly                                       ///
//////////////////////////////////////////////////////////////
/*
void HelpTreeBase::Fill_Fix30( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix30("BTag_FixedCutBEff_30");
  if( isFix30.isAvailable( *jet ) ) {
    if ( isFix30( *jet ) == 1 ) ++thisJet->m_njet_Fix30;
    thisJet->m_jet_isFix30.push_back( isFix30( *jet ) );
  } else { thisJet->m_jet_isFix30.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix30("BTag_SF_FixedCutBEff_30");
  if ( sfFix30.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix30.push_back( sfFix30( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix30.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix50( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix50("BTag_FixedCutBEff_50");
  if( isFix50.isAvailable( *jet ) ) {
    if ( isFix50( *jet ) == 1 ) ++thisJet->m_njet_Fix50;
    thisJet->m_jet_isFix50.push_back( isFix50( *jet ) );
  } else { thisJet->m_jet_isFix50.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix50("BTag_SF_FixedCutBEff_50");
  if ( sfFix50.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix50.push_back( sfFix50( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix50.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix60( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix60("BTag_FixedCutBEff_60");
  if( isFix60.isAvailable( *jet ) ) {
    if ( isFix60( *jet ) == 1 ) ++thisJet->m_njet_Fix60;
    thisJet->m_jet_isFix60.push_back( isFix60( *jet ) );
  } else { thisJet->m_jet_isFix60.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix60("BTag_SF_FixedCutBEff_60");
  if ( sfFix60.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix60.push_back( sfFix60( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix60.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix70( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix70("BTag_FixedCutBEff_70");
  if( isFix70.isAvailable( *jet ) ) {
    if ( isFix70( *jet ) == 1 ) ++thisJet->m_njet_Fix70;
    thisJet->m_jet_isFix70.push_back( isFix70( *jet ) );
  } else { thisJet->m_jet_isFix70.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix70("BTag_SF_FixedCutBEff_70");
  if ( sfFix70.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix70.push_back( sfFix70( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix70.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix77( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix77("BTag_FixedCutBEff_77");
  if( isFix77.isAvailable( *jet ) ) {
    if ( isFix77( *jet ) == 1 ) ++thisJet->m_njet_Fix77;
    thisJet->m_jet_isFix77.push_back( isFix77( *jet ) );
  } else { thisJet->m_jet_isFix77.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix77("BTag_SF_FixedCutBEff_77");
  if ( sfFix77.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix77.push_back( sfFix77( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix77.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix80( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix80("BTag_FixedCutBEff_80");
  if( isFix80.isAvailable( *jet ) ) {
    if ( isFix80( *jet ) == 1 ) ++thisJet->m_njet_Fix80;
    thisJet->m_jet_isFix80.push_back( isFix80( *jet ) );
  } else { thisJet->m_jet_isFix80.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix80("BTag_SF_FixedCutBEff_80");
  if ( sfFix80.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix80.push_back( sfFix80( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix80.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix85( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix85("BTag_FixedCutBEff_85");
  if( isFix85.isAvailable( *jet ) ) {
    if ( isFix85( *jet ) == 1 ) ++thisJet->m_njet_Fix85;
    thisJet->m_jet_isFix85.push_back( isFix85( *jet ) );
  } else { thisJet->m_jet_isFix85.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix85("BTag_SF_FixedCutBEff_85");
  if ( sfFix85.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix85.push_back( sfFix85( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix85.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix90( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix90("BTag_FixedCutBEff_90");
  if( isFix90.isAvailable( *jet ) ) {
    if ( isFix90( *jet ) == 1 ) ++thisJet->m_njet_Fix90;
    thisJet->m_jet_isFix90.push_back( isFix90( *jet ) );
  } else { thisJet->m_jet_isFix90.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix90("BTag_SF_FixedCutBEff_90");
  if ( sfFix90.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFix90.push_back( sfFix90( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFix90.push_back(junk);
  }
}


///// FLAT EFFICIENCY

void HelpTreeBase::Fill_Flt30( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt30("BTag_FlatBEff_30");
  if( isFlt30.isAvailable( *jet ) ) {
    if ( isFlt30( *jet ) == 1 ) ++thisJet->m_njet_Flt30;
    thisJet->m_jet_isFlt30.push_back( isFlt30( *jet ) );
  } else { thisJet->m_jet_isFlt30.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt30("BTag_SF_FlatBEff_30");
  if ( sfFlt30.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFlt30.push_back( sfFlt30( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFlt30.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt40( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt40("BTag_FlatBEff_40");
  if( isFlt40.isAvailable( *jet ) ) {
    if ( isFlt40( *jet ) == 1 ) ++thisJet->m_njet_Flt40;
    thisJet->m_jet_isFlt40.push_back( isFlt40( *jet ) );
  } else { thisJet->m_jet_isFlt40.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt40("BTag_SF_FlatBEff_40");
  if ( sfFlt40.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFlt40.push_back( sfFlt40( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFlt40.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt50( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt50("BTag_FlatBEff_50");
  if( isFlt50.isAvailable( *jet ) ) {
    if ( isFlt50( *jet ) == 1 )  ++thisJet->m_njet_Flt50;
    thisJet->m_jet_isFlt50.push_back( isFlt50( *jet ) );
  } else { thisJet->m_jet_isFlt50.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt50("BTag_SF_FlatBEff_50");
  if ( sfFlt50.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFlt50.push_back( sfFlt50( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFlt50.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt60( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt60("BTag_FlatBEff_60");
  if( isFlt60.isAvailable( *jet ) ) {
    if ( isFlt60( *jet ) == 1 )  ++thisJet->m_njet_Flt60;
    thisJet->m_jet_isFlt60.push_back( isFlt60( *jet ) );
  } else { thisJet->m_jet_isFlt60.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt60("BTag_SF_FlatBEff_60");
  if ( sfFlt60.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFlt60.push_back( sfFlt60( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFlt60.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt70( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt70("BTag_FlatBEff_70");
  if( isFlt70.isAvailable( *jet ) ) {
    if ( isFlt70( *jet ) == 1 ) ++thisJet->m_njet_Flt70;
    thisJet->m_jet_isFlt70.push_back( isFlt70( *jet ) );
  } else { thisJet->m_jet_isFlt70.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt70("BTag_SF_FlatBEff_70");
  if ( sfFlt70.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFlt70.push_back( sfFlt70( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFlt70.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt77( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt77("BTag_FlatBEff_77");
  if( isFlt77.isAvailable( *jet ) ) {
    if ( isFlt77( *jet ) == 1 ) ++thisJet->m_njet_Flt77;
    thisJet->m_jet_isFlt77.push_back( isFlt77( *jet ) );
  } else { thisJet->m_jet_isFlt77.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt77("BTag_SF_FlatBEff_77");
  if ( sfFlt77.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFlt77.push_back( sfFlt77( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFlt77.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt85( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt85("BTag_FlatBEff_85");
  if( isFlt85.isAvailable( *jet ) ) {
    if ( isFlt85( *jet ) == 1 ) ++thisJet->m_njet_Flt85;
    thisJet->m_jet_isFlt85.push_back( isFlt85( *jet ) );
  } else { thisJet->m_jet_isFlt85.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt85("BTag_SF_FlatBEff_85");
  if ( sfFlt85.isAvailable( *jet ) ) {
    thisJet->m_jet_sfFlt85.push_back( sfFlt85( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_sfFlt85.push_back(junk);
  }
}
*/
