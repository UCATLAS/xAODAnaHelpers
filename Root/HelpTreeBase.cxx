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
  m_trkSelTool(nullptr),
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

    for(auto jetInfoSwitch: m_thisJetInfoSwitch)
        delete jetInfoSwitch.second;

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

  m_thisJetInfoSwitch[jetName] = new HelperClasses::JetInfoSwitch( detailStr );

  m_jets[jetName] = new jetInfo();

  jetInfo* thisJet = m_jets[jetName];

  // always
  m_tree->Branch(("n"+jetName+"s").c_str(),    &thisJet->N,("n"+jetName+"s/I").c_str());

  if ( m_thisJetInfoSwitch[jetName]->m_kinematic ) {
    m_tree->Branch((jetName+"_E"  ).c_str(),  &thisJet->m_jet_E);
    m_tree->Branch((jetName+"_pt" ).c_str(),  &thisJet->m_jet_pt);
    m_tree->Branch((jetName+"_phi").c_str(),  &thisJet->m_jet_phi);
    m_tree->Branch((jetName+"_eta").c_str(),  &thisJet->m_jet_eta);
  }

  if ( m_thisJetInfoSwitch[jetName]->m_rapidity ) {
    m_tree->Branch((jetName+"_rapidity").c_str(), &thisJet->m_jet_rapidity);
  }

  if( m_thisJetInfoSwitch[jetName]->m_clean ) {
    m_tree->Branch((jetName+"_Timing").c_str(),                        &thisJet->m_jet_time               );
    m_tree->Branch((jetName+"_LArQuality").c_str(),                    &thisJet->m_jet_LArQuality         );
    m_tree->Branch((jetName+"_HECQuality").c_str(),                    &thisJet->m_jet_hecq               );
    m_tree->Branch((jetName+"_NegativeE").c_str(),                     &thisJet->m_jet_negE               );
    m_tree->Branch((jetName+"_AverageLArQF").c_str(),                  &thisJet->m_jet_avLArQF            );
    m_tree->Branch((jetName+"_BchCorrCell").c_str(),                   &thisJet->m_jet_bchCorrCell        );
    m_tree->Branch((jetName+"_N90Constituents").c_str(),               &thisJet->m_jet_N90Const           );
    m_tree->Branch((jetName+"_LArBadHVEFracnergyFrac").c_str(),        &thisJet->m_jet_LArBadHVEFrac   );
    m_tree->Branch((jetName+"_LArBadHVNCell").c_str(),                 &thisJet->m_jet_LArBadHVNCell  	  );
    m_tree->Branch((jetName+"_OotFracClusters5").c_str(),              &thisJet->m_jet_OotFracClus5  	    );
    m_tree->Branch((jetName+"_OotFracClusters10").c_str(),             &thisJet->m_jet_OotFracClus10  	  );
    m_tree->Branch((jetName+"_LeadingClusterPt").c_str(),              &thisJet->m_jet_LeadingClusterPt  	            );
    m_tree->Branch((jetName+"_LeadingClusterSecondLambda").c_str(),    &thisJet->m_jet_LeadingClusterSecondLambda  	  );
    m_tree->Branch((jetName+"_LeadingClusterCenterLambda").c_str(),    &thisJet->m_jet_LeadingClusterCenterLambda  	  );
    m_tree->Branch((jetName+"_LeadingClusterSecondR").c_str(),         &thisJet->m_jet_LeadingClusterSecondR  	      );
    m_tree->Branch((jetName+"_clean_passLooseBad").c_str(),            &thisJet->m_jet_clean_passLooseBad             );
    m_tree->Branch((jetName+"_clean_passLooseBadUgly").c_str(),        &thisJet->m_jet_clean_passLooseBadUgly         );
    m_tree->Branch((jetName+"_clean_passTightBad").c_str(),            &thisJet->m_jet_clean_passTightBad             );
    m_tree->Branch((jetName+"_clean_passTightBadUgly").c_str(),        &thisJet->m_jet_clean_passTightBadUgly         );
  }

  if ( m_thisJetInfoSwitch[jetName]->m_energy ) {
    m_tree->Branch((jetName+"_HECFrac").c_str(), 	            &thisJet->m_jet_HECf	    );
    m_tree->Branch((jetName+"_EMFrac").c_str(),  	            &thisJet->m_jet_EMf	    );
    m_tree->Branch((jetName+"_CentroidR").c_str(),	            &thisJet->m_jet_centroidR      );
    m_tree->Branch((jetName+"_FracSamplingMax").c_str(),           &thisJet->m_jet_fracSampMax    );
    m_tree->Branch((jetName+"_FracSamplingMaxIndex").c_str(),      &thisJet->m_jet_fracSampMaxIdx );
    m_tree->Branch((jetName+"_LowEtConstituentsFrac").c_str(),     &thisJet->m_jet_lowEtFrac      );
    m_tree->Branch((jetName+"_GhostMuonSegmentCount").c_str(),     &thisJet->m_jet_muonSegCount   );
    m_tree->Branch((jetName+"_Width").c_str(),                     &thisJet->m_jet_width          );
  }

  if ( m_thisJetInfoSwitch[jetName]->m_scales ) {
    m_tree->Branch((jetName+"_emScalePt").c_str(),              &thisJet->m_jet_emPt            );
    m_tree->Branch((jetName+"_constScalePt").c_str(),           &thisJet->m_jet_constPt         );
    m_tree->Branch((jetName+"_pileupScalePt").c_str(),          &thisJet->m_jet_pileupPt        );
    m_tree->Branch((jetName+"_originConstitScalePt").c_str(),   &thisJet->m_jet_originConstitPt );
    m_tree->Branch((jetName+"_etaJESScalePt").c_str(),          &thisJet->m_jet_etaJESPt        );
    m_tree->Branch((jetName+"_gscScalePt").c_str(),             &thisJet->m_jet_gscPt           );
    m_tree->Branch((jetName+"_insituScalePt").c_str(),          &thisJet->m_jet_insituPt        );
  }

  if ( m_thisJetInfoSwitch[jetName]->m_layer ) {
    m_tree->Branch((jetName+"_EnergyPerSampling").c_str(),     &thisJet->m_jet_ePerSamp   );
  }

  if ( m_thisJetInfoSwitch[jetName]->m_trackAll ) {
    m_tree->Branch((jetName+"_NumTrkPt1000").c_str(),	    &thisJet->m_jet_NTrkPt1000   );
    m_tree->Branch((jetName+"_SumPtTrkPt1000").c_str(),    &thisJet->m_jet_SumPtPt1000  );
    m_tree->Branch((jetName+"_TrackWidthPt1000").c_str(),  &thisJet->m_jet_TrkWPt1000   );
    m_tree->Branch((jetName+"_NumTrkPt500").c_str(),	    &thisJet->m_jet_NTrkPt500    );
    m_tree->Branch((jetName+"_SumPtTrkPt500").c_str(),	    &thisJet->m_jet_SumPtPt500   );
    m_tree->Branch((jetName+"_TrackWidthPt500").c_str(),   &thisJet->m_jet_TrkWPt500    );
    m_tree->Branch((jetName+"_JVF").c_str(),		    &thisJet->m_jet_jvf	        );
  }

  if ( m_thisJetInfoSwitch[jetName]->m_trackPV ) {
    m_tree->Branch((jetName+"_NumTrkPt1000PV").c_str(),      &thisJet->m_jet_NTrkPt1000PV   );
    m_tree->Branch((jetName+"_SumPtTrkPt1000PV").c_str(),    &thisJet->m_jet_SumPtPt1000PV  );
    m_tree->Branch((jetName+"_TrackWidthPt1000PV").c_str(),  &thisJet->m_jet_TrkWPt1000PV   );
    m_tree->Branch((jetName+"_NumTrkPt500PV").c_str(),	      &thisJet->m_jet_NTrkPt500PV    );
    m_tree->Branch((jetName+"_SumPtTrkPt500PV").c_str(),     &thisJet->m_jet_SumPtPt500PV   );
    m_tree->Branch((jetName+"_TrackWidthPt500PV").c_str(),   &thisJet->m_jet_TrkWPt500PV    );
    m_tree->Branch((jetName+"_JVFPV").c_str(),		      &thisJet->m_jet_jvfPV	        );
    m_tree->Branch((jetName+"_Jvt").c_str(),		      &thisJet->m_jet_Jvt	          );
    m_tree->Branch((jetName+"_JvtJvfcorr").c_str(),	      &thisJet->m_jet_JvtJvfcorr     );
    m_tree->Branch((jetName+"_JvtRpt").c_str(),              &thisJet->m_jet_JvtRpt         );
    //m_tree->Branch((jetName+"_GhostTrackAssociationFraction").c_str(), &thisJet->m_jet_ghostTrackAssFrac);
  }

  if ( m_thisJetInfoSwitch[jetName]->m_allTrack ) {
    // if want to apply the selection of the PV then need to setup track selection tool
    // this applies the JVF/JVT selection cuts
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JvtManualRecalculation
    if( m_thisJetInfoSwitch[jetName]->m_allTrackPVSel ) {
      m_trkSelTool = new InDet::InDetTrackSelectionTool( "JetTrackSelection", "Loose" );
      m_trkSelTool->initialize();
      // to do this need to have AddJets return a status code
      //RETURN_CHECK( "HelpTreeBase::JetTrackSelection", m_trkSelTool->initialize(), "");
    }
    m_tree->Branch((jetName+"_GhostTrackCount").c_str(),  &thisJet->m_jet_GhostTrackCount );
    m_tree->Branch((jetName+"_GhostTrackPt").c_str(),     &thisJet->m_jet_GhostTrackPt    );
    m_tree->Branch((jetName+"_GhostTrack_pt").c_str(),    &thisJet->m_jet_GhostTrack_pt   );
    m_tree->Branch((jetName+"_GhostTrack_qOverP").c_str(),&thisJet->m_jet_GhostTrack_qOverP);
    m_tree->Branch((jetName+"_GhostTrack_eta").c_str(),   &thisJet->m_jet_GhostTrack_eta  );
    m_tree->Branch((jetName+"_GhostTrack_phi").c_str(),   &thisJet->m_jet_GhostTrack_phi  );
    m_tree->Branch((jetName+"_GhostTrack_e").c_str(),     &thisJet->m_jet_GhostTrack_e    );
    m_tree->Branch((jetName+"_GhostTrack_d0").c_str(),    &thisJet->m_jet_GhostTrack_d0   );
    m_tree->Branch((jetName+"_GhostTrack_z0").c_str(),    &thisJet->m_jet_GhostTrack_z0   );
    if ( m_thisJetInfoSwitch[jetName]->m_allTrackDetail ) {
      m_tree->Branch((jetName+"_GhostTrack_nPixelHits").c_str(),                           &thisJet->m_jet_GhostTrack_nPixHits);
      m_tree->Branch((jetName+"_GhostTrack_nSCTHits").c_str(),                             &thisJet->m_jet_GhostTrack_nSCTHits);
      m_tree->Branch((jetName+"_GhostTrack_nTRTHits").c_str(),                             &thisJet->m_jet_GhostTrack_nTRTHits);
      m_tree->Branch((jetName+"_GhostTrack_nPixelSharedHits").c_str(),                     &thisJet->m_jet_GhostTrack_nPixSharedHits);
      m_tree->Branch((jetName+"_GhostTrack_nPixelSplitHits").c_str(),                      &thisJet->m_jet_GhostTrack_nPixSplitHits);
      m_tree->Branch((jetName+"_GhostTrack_nInnermostPixelLayerHits").c_str(),             &thisJet->m_jet_GhostTrack_nIMLPixHits);
      m_tree->Branch((jetName+"_GhostTrack_nInnermostPixelLayerSharedHits").c_str(),       &thisJet->m_jet_GhostTrack_nIMLPixSharedHits);
      m_tree->Branch((jetName+"_GhostTrack_nInnermostPixelLayerSplitHits").c_str(),        &thisJet->m_jet_GhostTrack_nIMLPixSplitHits);
      m_tree->Branch((jetName+"_GhostTrack_nNextToInnermostPixelLayerHits").c_str(),       &thisJet->m_jet_GhostTrack_nNIMLPixHits);
      m_tree->Branch((jetName+"_GhostTrack_nNextToInnermostPixelLayerSharedHits").c_str(), &thisJet->m_jet_GhostTrack_nNIMLPixSharedHits);
      m_tree->Branch((jetName+"_GhostTrack_nNextToInnermostPixelLayerSplitHits").c_str(),  &thisJet->m_jet_GhostTrack_nNIMLPixSplitHits);
    }
  }

  if ( m_thisJetInfoSwitch[jetName]->m_constituent ) {
    m_tree->Branch((jetName+"_numConstituents").c_str() ,   &thisJet->m_jet_numConstituents);
  }

  if ( m_thisJetInfoSwitch[jetName]->m_constituentAll ) {
    m_tree->Branch((jetName+"_constituentWeights").c_str(), &thisJet->m_jet_constitWeights);
    m_tree->Branch((jetName+"_constituent_pt").c_str(),    &thisJet->m_jet_constit_pt    );
    m_tree->Branch((jetName+"_constituent_eta").c_str(),    &thisJet->m_jet_constit_eta   );
    m_tree->Branch((jetName+"_constituent_phi").c_str(),    &thisJet->m_jet_constit_phi   );
    m_tree->Branch((jetName+"_constituent_e").c_str(),    &thisJet->m_jet_constit_e     );
  }

  if( m_thisJetInfoSwitch[jetName]->m_flavTag ) {
    if ( !m_DC14 ) {
      m_tree->Branch((jetName+"_SV0").c_str(),           &thisJet->m_jet_sv0);
      m_tree->Branch((jetName+"_SV1").c_str(),           &thisJet->m_jet_sv1);
      m_tree->Branch((jetName+"_IP3D").c_str(),          &thisJet->m_jet_ip3d);
    }
    m_tree->Branch((jetName+"_SV1IP3D").c_str(),       &thisJet->m_jet_sv1ip3d);
    m_tree->Branch((jetName+"_MV1").c_str(),           &thisJet->m_jet_mv1);
    m_tree->Branch((jetName+"_MV2c00").c_str(),        &thisJet->m_jet_mv2c00);
    m_tree->Branch((jetName+"_MV2c20").c_str(),        &thisJet->m_jet_mv2c20);
    m_tree->Branch((jetName+"_HadronConeExclTruthLabelID").c_str(), &thisJet->m_jet_hadConeExclTruthLabel);
  }

  if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFix.empty() ) {
    for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFix.size(); i++ ) {
      switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFix.at(i) ) {
        case 30 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix30").c_str(),   &thisJet->m_njet_mv2c20_Fix30,("n"+jetName+"s_mv2c20_Fix30/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix30").c_str(),   &thisJet->m_jet_mv2c20_isFix30);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix30").c_str(),   &thisJet->m_jet_mv2c20_sfFix30);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix30").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix30);
	  }
          break;
        case 50 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix50").c_str(),   &thisJet->m_njet_mv2c20_Fix50,("n"+jetName+"s_mv2c20_Fix50/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix50").c_str(),   &thisJet->m_jet_mv2c20_isFix50);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix50").c_str(),   &thisJet->m_jet_mv2c20_sfFix50);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix50").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix50);
	  }
          break;
        case 60 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix60").c_str(),   &thisJet->m_njet_mv2c20_Fix60,("n"+jetName+"s_mv2c20_Fix60/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix60").c_str(),   &thisJet->m_jet_mv2c20_isFix60);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix60").c_str(),   &thisJet->m_jet_mv2c20_sfFix60);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix60").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix60);
	  }
          break;
        case 70 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix70").c_str(),   &thisJet->m_njet_mv2c20_Fix70,("n"+jetName+"s_mv2c20_Fix70/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix70").c_str(),   &thisJet->m_jet_mv2c20_isFix70);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix70").c_str(),   &thisJet->m_jet_mv2c20_sfFix70);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix70").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix70);
	  }
          break;
        case 77 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix77").c_str(),   &thisJet->m_njet_mv2c20_Fix77,("n"+jetName+"s_mv2c20_Fix77/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix77").c_str(),   &thisJet->m_jet_mv2c20_isFix77);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix77").c_str(),   &thisJet->m_jet_mv2c20_sfFix77);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix77").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix77);
	  }
          break;
        case 80 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix80").c_str(),   &thisJet->m_njet_mv2c20_Fix80,("n"+jetName+"s_mv2c20_Fix80/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix80").c_str(),   &thisJet->m_jet_mv2c20_isFix80);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix80").c_str(),   &thisJet->m_jet_mv2c20_sfFix80);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix80").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix80);
	  }
          break;
        case 85 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix85").c_str(),   &thisJet->m_njet_mv2c20_Fix85,("n"+jetName+"s_mv2c20_Fix85/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix85").c_str(),   &thisJet->m_jet_mv2c20_isFix85);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix85").c_str(),   &thisJet->m_jet_mv2c20_sfFix85);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix85").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix85);
	  }
          break;
        case 90 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Fix90").c_str(),   &thisJet->m_njet_mv2c20_Fix90,("n"+jetName+"s_mv2c20_Fix90/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFix90").c_str(),   &thisJet->m_jet_mv2c20_isFix90);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFix90").c_str(),   &thisJet->m_jet_mv2c20_sfFix90);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFix90").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFix90);
	  }
          break;
        default:
          std::cout << "BTag Fixed operating point of " << m_thisJetInfoSwitch[jetName]->m_sfFTagFix.at(i)
            << " is unknown to xAH" << std::endl;
      }
    }
  } // sfFTagFix

  if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.empty() ) {
    for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.size(); i++ ) {
      switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.at(i) ) {
        case 30 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Flt30").c_str(),   &thisJet->m_njet_mv2c20_Flt30,("n"+jetName+"s_mv2c20_Flt30/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFlt30").c_str(),   &thisJet->m_jet_mv2c20_isFlt30);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFlt30").c_str(),   &thisJet->m_jet_mv2c20_sfFlt30);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFlt30").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFlt30);
	  }
          break;
        case 40 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Flt40").c_str(),   &thisJet->m_njet_mv2c20_Flt40,("n"+jetName+"s_mv2c20_Flt40/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFlt40").c_str(),   &thisJet->m_jet_mv2c20_isFlt40);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFlt40").c_str(),   &thisJet->m_jet_mv2c20_sfFlt40);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFlt40").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFlt40);
	  }
          break;
        case 50 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Flt50").c_str(),   &thisJet->m_njet_mv2c20_Flt50,("n"+jetName+"s_mv2c20_Flt50/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFlt50").c_str(),   &thisJet->m_jet_mv2c20_isFlt50);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFlt50").c_str(),   &thisJet->m_jet_mv2c20_sfFlt50);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFlt50").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFlt50);
	  }
          break;
        case 60 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Flt60").c_str(),   &thisJet->m_njet_mv2c20_Flt60,("n"+jetName+"s_mv2c20_Flt60/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFlt60").c_str(),   &thisJet->m_jet_mv2c20_isFlt60);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFlt60").c_str(),   &thisJet->m_jet_mv2c20_sfFlt60);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFlt60").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFlt60);
	  }
          break;
        case 70 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Flt70").c_str(),   &thisJet->m_njet_mv2c20_Flt70,("n"+jetName+"s_mv2c20_Flt70/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFlt70").c_str(),   &thisJet->m_jet_mv2c20_isFlt70);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFlt70").c_str(),   &thisJet->m_jet_mv2c20_sfFlt70);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFlt70").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFlt70);
	  }
          break;
        case 77 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Flt77").c_str(),   &thisJet->m_njet_mv2c20_Flt77,("n"+jetName+"s_mv2c20_Flt77/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFlt77").c_str(),   &thisJet->m_jet_mv2c20_isFlt77);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFlt77").c_str(),   &thisJet->m_jet_mv2c20_sfFlt77);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFlt77").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFlt77);
	  }
          break;
        case 85 :
	  m_tree->Branch(("n"+jetName+"s_mv2c20_Flt85").c_str(),   &thisJet->m_njet_mv2c20_Flt85,("n"+jetName+"s_mv2c20_Flt85/I").c_str());
          m_tree->Branch((jetName+"_MV2c20_isFlt85").c_str(),   &thisJet->m_jet_mv2c20_isFlt85);
          if ( m_isMC ) {
	    m_tree->Branch((jetName+"_MV2c20_SFFlt85").c_str(),   &thisJet->m_jet_mv2c20_sfFlt85);
            m_tree->Branch(("weight_"+jetName+"__MV2c20_SFFlt85").c_str(),   &thisJet->m_weight_jet_mv2c20_sfFlt85);
	  }
          break;
        default:
          std::cout << "BTag Flat operating point of " << m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.at(i)
            << " is unknown to xAH" << std::endl;
      }
    }
  } // sfFTagFlt

  if( m_thisJetInfoSwitch[jetName]->m_area ) {
    m_tree->Branch((jetName+"_GhostArea").c_str(),     &thisJet->m_jet_ghostArea);
    m_tree->Branch((jetName+"_ActiveArea").c_str(),    &thisJet->m_jet_activeArea);
    m_tree->Branch((jetName+"_VoronoiArea").c_str(),   &thisJet->m_jet_voronoiArea);
    m_tree->Branch((jetName+"_ActiveArea4vec_pt").c_str(), &thisJet->m_jet_activeArea_pt);
    m_tree->Branch((jetName+"_ActiveArea4vec_eta").c_str(), &thisJet->m_jet_activeArea_eta);
    m_tree->Branch((jetName+"_ActiveArea4vec_phi").c_str(), &thisJet->m_jet_activeArea_phi);
    m_tree->Branch((jetName+"_ActiveArea4vec_m").c_str(),   &thisJet->m_jet_activeArea_m);
  }

  if ( m_thisJetInfoSwitch[jetName]->m_truth && m_isMC ) {
    m_tree->Branch((jetName+"_ConeTruthLabelID").c_str(),   &thisJet->m_jet_truthConeLabelID );
    m_tree->Branch((jetName+"_TruthCount").c_str(),         &thisJet->m_jet_truthCount     );
//    m_tree->Branch((jetName+"_TruthPt").c_str(),            &thisJet->m_jet_truthPt        );
    m_tree->Branch((jetName+"_TruthLabelDeltaR_B").c_str(), &thisJet->m_jet_truthDr_B      );
    m_tree->Branch((jetName+"_TruthLabelDeltaR_C").c_str(), &thisJet->m_jet_truthDr_C      );
    m_tree->Branch((jetName+"_TruthLabelDeltaR_T").c_str(), &thisJet->m_jet_truthDr_T      );
    m_tree->Branch((jetName+"_PartonTruthLabelID").c_str(), &thisJet->m_jet_partonTruthID  );
    m_tree->Branch((jetName+"_GhostTruthAssociationFraction").c_str(), &thisJet->m_jet_ghostTruthAssFrac);
    m_tree->Branch((jetName+"_truth_E").c_str(),   &thisJet->m_jet_truth_E);
    m_tree->Branch((jetName+"_truth_pt").c_str(),  &thisJet->m_jet_truth_pt);
    m_tree->Branch((jetName+"_truth_phi").c_str(), &thisJet->m_jet_truth_phi);
    m_tree->Branch((jetName+"_truth_eta").c_str(), &thisJet->m_jet_truth_eta);
  }

  if ( m_thisJetInfoSwitch[jetName]->m_truthDetails ) {
    m_tree->Branch((jetName+"_GhostBHadronsFinalCount").c_str(),   &thisJet->m_jet_truthCount_BhadFinal );
    m_tree->Branch((jetName+"_GhostBHadronsInitialCount").c_str(), &thisJet->m_jet_truthCount_BhadInit  );
    m_tree->Branch((jetName+"_GhostBQuarksFinalCount").c_str(),    &thisJet->m_jet_truthCount_BQFinal   );
    m_tree->Branch((jetName+"_GhostBHadronsFinalPt").c_str(),      &thisJet->m_jet_truthPt_BhadFinal    );
    m_tree->Branch((jetName+"_GhostBHadronsInitialPt").c_str(),    &thisJet->m_jet_truthPt_BhadInit     );
    m_tree->Branch((jetName+"_GhostBQuarksFinalPt").c_str(),       &thisJet->m_jet_truthPt_BQFinal      );

    m_tree->Branch((jetName+"_GhostCHadronsFinalCount"  ).c_str(), &thisJet->m_jet_truthCount_ChadFinal );
    m_tree->Branch((jetName+"_GhostCHadronsInitialCount").c_str(), &thisJet->m_jet_truthCount_ChadInit  );
    m_tree->Branch((jetName+"_GhostCQuarksFinalCount"   ).c_str(), &thisJet->m_jet_truthCount_CQFinal   );
    m_tree->Branch((jetName+"_GhostCHadronsFinalPt"     ).c_str(), &thisJet->m_jet_truthPt_ChadFinal    );
    m_tree->Branch((jetName+"_GhostCHadronsInitialPt"   ).c_str(), &thisJet->m_jet_truthPt_ChadInit     );
    m_tree->Branch((jetName+"_GhostCQuarksFinalPt"      ).c_str(), &thisJet->m_jet_truthPt_CQFinal      );

    m_tree->Branch((jetName+"_GhostTausFinalCount").c_str(),       &thisJet->m_jet_truthCount_TausFinal );
    m_tree->Branch((jetName+"_GhostTausFinalPt"   ).c_str(),       &thisJet->m_jet_truthPt_TausFinal    );

    m_tree->Branch((jetName+"_truth_pdgId"   ).c_str(), &thisJet->m_jet_truth_pdgId);
    m_tree->Branch((jetName+"_truth_partonPt").c_str(), &thisJet->m_jet_truth_partonPt);
    m_tree->Branch((jetName+"_truth_partonDR").c_str(), &thisJet->m_jet_truth_partonDR);
  }


  this->AddJetsUser(detailStr, jetName);

}


void HelpTreeBase::FillJets( const xAOD::JetContainer* jets, int pvLocation, const std::string jetName ) {

  this->ClearJets(jetName);

  const xAOD::VertexContainer* vertices(nullptr);
  const xAOD::Vertex *pv = 0;

  if( m_thisJetInfoSwitch[jetName]->m_trackPV || m_thisJetInfoSwitch[jetName]->m_allTrack ) {
    HelperFunctions::retrieve( vertices, "PrimaryVertices", m_event, 0 );
    pvLocation = HelperFunctions::getPrimaryVertexLocation( vertices );
    pv = vertices->at( pvLocation );
  }

  jetInfo* thisJet = m_jets[jetName];

  // Global event BTag SF weight (--> the product of each object's weight)
  //
  if ( m_isMC ) {
    const xAOD::EventInfo* eventInfo(nullptr);
    HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, false);


    if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFix.empty() ) {
      for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFix.size(); i++ ) {
    	switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFix.at(i) ) {
    	  case 30 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix30_GLOBAL("BTag_SF_FixedCutBEff_30_GLOBAL");
            if ( sfFix30_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix30 = sfFix30_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix30.push_back(-999.0); }
    	    break;
    	  case 50 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix50_GLOBAL("BTag_SF_FixedCutBEff_50_GLOBAL");
            if ( sfFix50_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix50 = sfFix50_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix50.push_back(-999.0); }
    	    break;
    	  case 60 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix60_GLOBAL("BTag_SF_FixedCutBEff_60_GLOBAL");
            if ( sfFix60_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix60 = sfFix60_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix60.push_back(-999.0); }
    	    break;
    	  case 70 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix70_GLOBAL("BTag_SF_FixedCutBEff_70_GLOBAL");
            if ( sfFix70_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix70 = sfFix70_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix70.push_back(-999.0); }
    	    break;
    	  case 77 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix77_GLOBAL("BTag_SF_FixedCutBEff_77_GLOBAL");
            if ( sfFix77_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix77 = sfFix77_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix77.push_back(-999.0); }
    	    break;
    	  case 80 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix80_GLOBAL("BTag_SF_FixedCutBEff_80_GLOBAL");
            if ( sfFix80_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix80 = sfFix80_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix80.push_back(-999.0); }
    	    break;
    	  case 85 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix85_GLOBAL("BTag_SF_FixedCutBEff_85_GLOBAL");
            if ( sfFix85_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix85 = sfFix85_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix85.push_back(-999.0); }
    	    break;
    	  case 90 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix90_GLOBAL("BTag_SF_FixedCutBEff_90_GLOBAL");
            if ( sfFix90_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFix90 = sfFix90_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFix90.push_back(-999.0); }
    	    break;
    	}
      }
    } // sfFTagFix

    if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.empty() ) {
      for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.size(); i++ ) {
    	switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.at(i) ) {
    	  case 30 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt30_GLOBAL("BTag_SF_FlatBEff_30_GLOBAL");
            if ( sfFlt30_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFlt30 = sfFlt30_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFlt30.push_back(-999.0); }
    	    break;
    	  case 40 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt40_GLOBAL("BTag_SF_FlatBEff_40_GLOBAL");
            if ( sfFlt40_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFlt40 = sfFlt40_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFlt40.push_back(-999.0); }
    	    break;
    	  case 50 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt50_GLOBAL("BTag_SF_FlatBEff_50_GLOBAL");
            if ( sfFlt50_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFlt50 = sfFlt50_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFlt50.push_back(-999.0); }
    	    break;
    	  case 60 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt60_GLOBAL("BTag_SF_FlatBEff_60_GLOBAL");
            if ( sfFlt60_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFlt60 = sfFlt60_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFlt60.push_back(-999.0); }
    	    break;
    	  case 70 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt70_GLOBAL("BTag_SF_FlatBEff_70_GLOBAL");
            if ( sfFlt70_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFlt70 = sfFlt70_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFlt70.push_back(-999.0); }
    	    break;
    	  case 77 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt77_GLOBAL("BTag_SF_FlatBEff_77_GLOBAL");
            if ( sfFlt77_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFlt77 = sfFlt77_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFlt77.push_back(-999.0); }
    	    break;
    	  case 85 :
            static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt85_GLOBAL("BTag_SF_FlatBEff_85_GLOBAL");
            if ( sfFlt85_GLOBAL.isAvailable( *eventInfo ) ) { thisJet->m_weight_jet_mv2c20_sfFlt85 = sfFlt85_GLOBAL( *eventInfo ); } else { thisJet->m_weight_jet_mv2c20_sfFlt85.push_back(-999.0); }
    	    break;
    	}
      }
    } // sfFTagFlt
  }

  for( auto jet_itr : *jets ) {
    this->FillJet(jet_itr, pv, pvLocation, jetName);
  }

}



void HelpTreeBase::FillJet( const xAOD::Jet* jet_itr, const xAOD::Vertex* pv, int pvLocation, const std::string jetName ) {

  jetInfo* thisJet = m_jets[jetName];

  if( m_thisJetInfoSwitch[jetName]->m_kinematic ){
    thisJet->m_jet_pt.push_back ( jet_itr->pt() / m_units );
    thisJet->m_jet_eta.push_back( jet_itr->eta() );
    thisJet->m_jet_phi.push_back( jet_itr->phi() );
    thisJet->m_jet_E.push_back  ( jet_itr->e() / m_units );
  }


  if( m_thisJetInfoSwitch[jetName]->m_rapidity ){
    thisJet->m_jet_rapidity.push_back( jet_itr->rapidity() );
  }

  if (m_thisJetInfoSwitch[jetName]->m_clean) {

    static SG::AuxElement::ConstAccessor<float> jetTime ("Timing");
    safeFill<float, float>(jet_itr, jetTime, thisJet->m_jet_time, -999);

    static SG::AuxElement::ConstAccessor<float> LArQuality ("LArQuality");
    safeFill<float, float>(jet_itr, LArQuality, thisJet->m_jet_LArQuality, -999);

    static SG::AuxElement::ConstAccessor<float> hecq ("HECQuality");
    safeFill<float, float>(jet_itr, hecq, thisJet->m_jet_hecq, -999);

    static SG::AuxElement::ConstAccessor<float> negE ("NegativeE");
    safeFill<float, float>(jet_itr, negE, thisJet->m_jet_negE, -999, m_units);

    static SG::AuxElement::ConstAccessor<float> avLArQF ("AverageLArQF");
    safeFill<float, float>(jet_itr, avLArQF, thisJet->m_jet_avLArQF, -999);

    static SG::AuxElement::ConstAccessor<float> bchCorrCell ("BchCorrCell");
    safeFill<float, float>(jet_itr, bchCorrCell, thisJet->m_jet_bchCorrCell, -999);

    static SG::AuxElement::ConstAccessor<float> N90Const ("N90Constituents");
    safeFill<float, float>(jet_itr, N90Const, thisJet->m_jet_N90Const, -999);

    static SG::AuxElement::ConstAccessor<float> LArBadHVEFrac ("LArBadHVEnergyFrac");
    safeFill<float, float>(jet_itr, LArBadHVEFrac, thisJet->m_jet_LArBadHVEFrac, -999);

    static SG::AuxElement::ConstAccessor<int> LArBadHVNCell ("LArBadHVNCell");
    safeFill<int, int>(jet_itr, LArBadHVNCell, thisJet->m_jet_LArBadHVNCell, -999);

    static SG::AuxElement::ConstAccessor<float> OotFracClus5 ("OotFracClusters5");
    safeFill<float, float>(jet_itr, OotFracClus5, thisJet->m_jet_OotFracClus5, -999);

    static SG::AuxElement::ConstAccessor<float> OotFracClus10 ("OotFracClusters10");
    safeFill<float, float>(jet_itr, OotFracClus10, thisJet->m_jet_OotFracClus10, -999);

    static SG::AuxElement::ConstAccessor<float> leadClusPt ("LeadingClusterPt");
    safeFill<float, float>(jet_itr, leadClusPt, thisJet->m_jet_LeadingClusterPt, -999);

    static SG::AuxElement::ConstAccessor<float> leadClusSecondLambda ("LeadingClusterSecondLambda");
    safeFill<float, float>(jet_itr, leadClusSecondLambda, thisJet->m_jet_LeadingClusterSecondLambda, -999);

    static SG::AuxElement::ConstAccessor<float> leadClusCenterLambda ("LeadingClusterCenterLambda");
    safeFill<float, float>(jet_itr, leadClusCenterLambda, thisJet->m_jet_LeadingClusterCenterLambda, -999);

    static SG::AuxElement::ConstAccessor<float> leadClusSecondR ("LeadingClusterSecondR");
    safeFill<float, float>(jet_itr, leadClusSecondR, thisJet->m_jet_LeadingClusterSecondR, -999);

    static SG::AuxElement::ConstAccessor<char> clean_passLooseBad ("clean_passLooseBad");
    safeFill<char, int>(jet_itr, clean_passLooseBad, thisJet->m_jet_clean_passLooseBad, -999);

    static SG::AuxElement::ConstAccessor<char> clean_passLooseBadUgly ("clean_passLooseBadUgly");
    safeFill<char, int>(jet_itr, clean_passLooseBadUgly, thisJet->m_jet_clean_passLooseBadUgly, -999);

    static SG::AuxElement::ConstAccessor<char> clean_passTightBad ("clean_passTightBad");
    safeFill<char, int>(jet_itr, clean_passTightBad, thisJet->m_jet_clean_passTightBad, -999);

    static SG::AuxElement::ConstAccessor<char> clean_passTightBadUgly ("clean_passTightBadUgly");
    safeFill<char, int>(jet_itr, clean_passTightBadUgly, thisJet->m_jet_clean_passTightBadUgly, -999);

  } // clean

  if ( m_thisJetInfoSwitch[jetName]->m_energy ) {

    static SG::AuxElement::ConstAccessor<float> HECf ("HECFrac");
    safeFill<float, float>(jet_itr, HECf, thisJet->m_jet_HECf, -999);

    static SG::AuxElement::ConstAccessor<float> EMf ("EMFrac");
    safeFill<float, float>(jet_itr, EMf, thisJet->m_jet_EMf, -999);

    static SG::AuxElement::ConstAccessor<float> centroidR ("CentroidR");
    safeFill<float, float>(jet_itr, centroidR, thisJet->m_jet_centroidR, -999);

    static SG::AuxElement::ConstAccessor<float> fracSampMax ("FracSamplingMax");
    safeFill<float, float>(jet_itr, fracSampMax, thisJet->m_jet_fracSampMax, -999);

    static SG::AuxElement::ConstAccessor<int> fracSampMaxIdx ("FracSamplingMaxIndex");
    safeFill<int, float>(jet_itr, fracSampMaxIdx, thisJet->m_jet_fracSampMaxIdx, -999);

    static SG::AuxElement::ConstAccessor<float> lowEtFrac ("LowEtConstituentsFrac");
    safeFill<float, float>(jet_itr, lowEtFrac, thisJet->m_jet_lowEtFrac, -999);

    static SG::AuxElement::ConstAccessor<int> muonSegCount ("GhostMuonSegmentCount");
    safeFill<int, float>(jet_itr, muonSegCount, thisJet->m_jet_muonSegCount, -999);

    static SG::AuxElement::ConstAccessor<float> width ("Width");
    safeFill<float, float>(jet_itr, width, thisJet->m_jet_width, -999);

  } // energy


    // each step of the calibration sequence
  if ( m_thisJetInfoSwitch[jetName]->m_scales ) {
    xAOD::JetFourMom_t fourVec;
    bool status(false);
    // EM Scale
    status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetEMScaleMomentum", fourVec );
    if( status ) { thisJet->m_jet_emPt.push_back( fourVec.Pt() / m_units ); }
    else { thisJet->m_jet_emPt.push_back( -999 ); }
    // Constit Scale
    status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetConstitScaleMomentum", fourVec );
    if( status ) { thisJet->m_jet_constPt.push_back( fourVec.Pt() / m_units ); }
    else { thisJet->m_jet_constPt.push_back( -999 ); }
    // Pileup Scale
    status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetPileupScaleMomentum", fourVec );
    if( status ) { thisJet->m_jet_pileupPt.push_back( fourVec.Pt() / m_units ); }
    else { thisJet->m_jet_pileupPt.push_back( -999 ); }
    // OriginConstit Scale
    status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetOriginConstitScaleMomentum", fourVec );
    if( status ) { thisJet->m_jet_originConstitPt.push_back( fourVec.Pt() / m_units ); }
    else { thisJet->m_jet_originConstitPt.push_back( -999 ); }
    // EtaJES Scale
    status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetEtaJESScaleMomentum", fourVec );
    if( status ) { thisJet->m_jet_etaJESPt.push_back( fourVec.Pt() / m_units ); }
    else { thisJet->m_jet_etaJESPt.push_back( -999 ); }
    // GSC Scale
    status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetGSCScaleMomentum", fourVec );
    if( status ) { thisJet->m_jet_gscPt.push_back( fourVec.Pt() / m_units ); }
    else { thisJet->m_jet_gscPt.push_back( -999 ); }
    // only available in data
    status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetInsituScaleMomentum", fourVec );
    if(status) { thisJet->m_jet_insituPt.push_back( fourVec.Pt() / m_units ); }
    else { thisJet->m_jet_insituPt.push_back( -999 ); }
  }

  if ( m_thisJetInfoSwitch[jetName]->m_layer ) {
    static SG::AuxElement::ConstAccessor< std::vector<float> > ePerSamp ("EnergyPerSampling");
    if ( ePerSamp.isAvailable( *jet_itr ) ) {
      thisJet->m_jet_ePerSamp.push_back( ePerSamp( *jet_itr ) );
      thisJet->m_jet_ePerSamp.back();
      std::transform((thisJet->m_jet_ePerSamp.back()).begin(),
		     (thisJet->m_jet_ePerSamp.back()).end(),
		     (thisJet->m_jet_ePerSamp.back()).begin(),
		     std::bind2nd(std::divides<float>(), m_units));
    } else {
      // could push back a vector of 24...
      // ... waste of space vs prevention of out of range down stream
      std::vector<float> junk(1,-999);
      thisJet->m_jet_ePerSamp.push_back( junk );
    }
  }


  if ( m_thisJetInfoSwitch[jetName]->m_trackAll || m_thisJetInfoSwitch[jetName]->m_trackPV ) {

    // several moments calculated from all verticies
    // one accessor for each and just use appropiately in the following
    static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk1000("NumTrkPt1000");
    static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt1000("SumPtTrkPt1000");
    static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth1000("TrackWidthPt1000");
    static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk500 ("NumTrkPt500");
    static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt500 ("SumPtTrkPt500");
    static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth500 ("TrackWidthPt500");
    static SG::AuxElement::ConstAccessor< std::vector<float> > jvf("JVF");

    if ( m_thisJetInfoSwitch[jetName]->m_trackAll ) {

      std::vector<int> junkInt(1,-999);
      std::vector<float> junkFlt(1,-999);

      if ( nTrk1000.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_NTrkPt1000.push_back( nTrk1000( *jet_itr ) );
      } else { thisJet->m_jet_NTrkPt1000.push_back( junkInt ); }

      if ( sumPt1000.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_SumPtPt1000.push_back( sumPt1000( *jet_itr ) );
	std::transform((thisJet->m_jet_SumPtPt1000.back()).begin(),
		       (thisJet->m_jet_SumPtPt1000.back()).end(),
		       (thisJet->m_jet_SumPtPt1000.back()).begin(),
		       std::bind2nd(std::divides<float>(), m_units));
      } else { thisJet->m_jet_SumPtPt1000.push_back( junkFlt ); }

      if ( trkWidth1000.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_TrkWPt1000.push_back( trkWidth1000( *jet_itr ) );
      } else { thisJet->m_jet_TrkWPt1000.push_back( junkFlt ); }

      if ( nTrk500.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_NTrkPt500.push_back( nTrk500( *jet_itr ) );
      } else { thisJet->m_jet_NTrkPt500.push_back( junkInt ); }

      if ( sumPt500.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_SumPtPt500.push_back( sumPt500( *jet_itr ) );
	std::transform((thisJet->m_jet_SumPtPt500.back()).begin(),
		       (thisJet->m_jet_SumPtPt500.back()).end(),
		       (thisJet->m_jet_SumPtPt500.back()).begin(),
		       std::bind2nd(std::divides<float>(), m_units));
      } else { thisJet->m_jet_SumPtPt500.push_back( junkFlt ); }

      if ( trkWidth500.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_TrkWPt500.push_back( trkWidth500( *jet_itr ) );
      } else { thisJet->m_jet_TrkWPt500.push_back( junkFlt ); }

      if ( jvf.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_jvf.push_back( jvf( *jet_itr ) );
      } else { thisJet->m_jet_jvf.push_back( junkFlt ); }

    } // trackAll

    if ( m_thisJetInfoSwitch[jetName]->m_trackPV && pvLocation >= 0 ) {

      if ( nTrk1000.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_NTrkPt1000PV.push_back( nTrk1000( *jet_itr )[pvLocation] );
      } else { thisJet->m_jet_NTrkPt1000PV.push_back( -999 ); }

      if ( sumPt1000.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_SumPtPt1000PV.push_back( sumPt1000( *jet_itr )[pvLocation] / m_units );
      } else { thisJet->m_jet_SumPtPt1000PV.push_back( -999 ); }

      if ( trkWidth1000.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_TrkWPt1000PV.push_back( trkWidth1000( *jet_itr )[pvLocation] );
      } else { thisJet->m_jet_TrkWPt1000PV.push_back( -999 ); }

      if ( nTrk500.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_NTrkPt500PV.push_back( nTrk500( *jet_itr )[pvLocation] );
      } else { thisJet->m_jet_NTrkPt500PV.push_back( -999 ); }

      if ( sumPt500.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_SumPtPt500PV.push_back( sumPt500( *jet_itr )[pvLocation] / m_units );
      } else { thisJet->m_jet_SumPtPt500PV.push_back( -999 ); }

      if ( trkWidth500.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_TrkWPt500PV.push_back( trkWidth500( *jet_itr )[pvLocation] );
      } else { thisJet->m_jet_TrkWPt500PV.push_back( -999 ); }

      if ( jvf.isAvailable( *jet_itr ) ) {
	thisJet->m_jet_jvfPV.push_back( jvf( *jet_itr )[pvLocation] );
      } else { thisJet->m_jet_jvfPV.push_back( -999 ); }

    } // trackPV

    static SG::AuxElement::ConstAccessor< float > jvt ("Jvt");
    if ( jvt.isAvailable( *jet_itr ) ) {
      thisJet->m_jet_Jvt.push_back( jvt( *jet_itr ) );
    } else { thisJet->m_jet_Jvt.push_back( -999 ); }

    static SG::AuxElement::ConstAccessor< float > jvtJvfcorr ("JvtJvfcorr");
    if ( jvtJvfcorr.isAvailable( *jet_itr ) ) {
      thisJet->m_jet_JvtJvfcorr.push_back( jvtJvfcorr( *jet_itr ) );
    } else { thisJet->m_jet_JvtJvfcorr.push_back( -999 ); }

    static SG::AuxElement::ConstAccessor< float > jvtRpt ("JvtRpt");
    if ( jvtRpt.isAvailable( *jet_itr ) ) {
      thisJet->m_jet_JvtRpt.push_back( jvtRpt( *jet_itr ) );
    } else { thisJet->m_jet_JvtRpt.push_back( -999 ); }

    //      static SG::AuxElement::ConstAccessor<float> ghostTrackAssFrac("GhostTrackAssociationFraction");
    //      if ( ghostTrackAssFrac.isAvailable( *jet_itr) ) {
    //        thisJet->m_jet_ghostTrackAssFrac.push_back( ghostTrackAssFrac( *jet_itr) );
    //      } else { thisJet->m_jet_ghostTrackAssFrac.push_back( -999 ) ; }

  }

  if ( m_thisJetInfoSwitch[jetName]->m_allTrack ) {
    static SG::AuxElement::ConstAccessor< int > ghostTrackCount("GhostTrackCount");
    if ( ghostTrackCount.isAvailable( *jet_itr ) ) {
      thisJet->m_jet_GhostTrackCount.push_back( ghostTrackCount( *jet_itr ) );
    } else { thisJet->m_jet_GhostTrackCount.push_back( -999 ); }
    static SG::AuxElement::ConstAccessor< float > ghostTrackPt ("GhostTrackPt");
    if ( ghostTrackPt.isAvailable( *jet_itr ) ) {
      thisJet->m_jet_GhostTrackPt.push_back( ghostTrackPt( *jet_itr ) / m_units );
    } else { thisJet->m_jet_GhostTrackPt.push_back( -999 ); }
    std::vector<float> pt;
    std::vector<float> qOverP;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<float> e;
    std::vector<float> d0;
    std::vector<float> z0;
    std::vector<int> nPixHits;
    std::vector<int> nSCTHits;
    std::vector<int> nTRTHits;
    std::vector<int> nPixSharedHits;
    std::vector<int> nPixSplitHits;
    std::vector<int> nIMLPixHits;
    std::vector<int> nIMLPixSharedHits;
    std::vector<int> nIMLPixSplitHits;
    std::vector<int> nNIMLPixHits;
    std::vector<int> nNIMLPixSharedHits;
    std::vector<int> nNIMLPixSplitHits;
    static SG::AuxElement::ConstAccessor< std::vector<ElementLink<DataVector<xAOD::IParticle> > > >ghostTrack ("GhostTrack");
    if ( ghostTrack.isAvailable( *jet_itr ) ) {
      std::vector<ElementLink<DataVector<xAOD::IParticle> > > trackLinks = ghostTrack( *jet_itr );
      //std::vector<float> pt(trackLinks.size(),-999);
      for ( auto link_itr : trackLinks ) {
	if( !link_itr.isValid() ) { continue; }
	const xAOD::TrackParticle* track = dynamic_cast<const xAOD::TrackParticle*>( *link_itr );
	// if asking for tracks passing PV selection ( i.e. JVF JVT tracks )
	if( m_thisJetInfoSwitch[jetName]->m_allTrackPVSel ) {
	  // PV selection from
	  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JvtManualRecalculation
	  if( track->pt() < 500 )                { continue; } // pT cut
	  if( !m_trkSelTool->accept(*track,pv) ) { continue; } // ID quality cut
	  if( track->vertex() != pv ) {                        // if not in PV vertex fit
	    if( track->vertex() != 0 )           { continue; } // make sure in no vertex fits
	    if( fabs((track->z0()+track->vz()-pv->z())*sin(track->theta())) > 3.0 ) { continue; } // make sure close to PV in z
	  }
	}
	pt. push_back( track->pt() / m_units );
	qOverP.push_back( track->qOverP() * m_units );
	eta.push_back( track->eta() );
	phi.push_back( track->phi() );
	e.  push_back( track->e()  / m_units );
	d0. push_back( track->d0() );
	z0. push_back( track->z0() + track->vz() - pv->z() ); // store z0 wrt PV...most useful
	if( m_thisJetInfoSwitch[jetName]->m_allTrackDetail ) {
	  uint8_t getInt(0);
	  // n pix, sct, trt
	  track->summaryValue( getInt, xAOD::numberOfPixelHits );
	  nPixHits.push_back( getInt );
	  track->summaryValue( getInt, xAOD::numberOfSCTHits );
	  nSCTHits.push_back( getInt );
	  track->summaryValue( getInt, xAOD::numberOfTRTHits );
	  nTRTHits.push_back( getInt );
	  // pixel split shared
	  track->summaryValue( getInt, xAOD::numberOfPixelSharedHits );
	  nPixSharedHits.push_back( getInt );
	  track->summaryValue( getInt, xAOD::numberOfPixelSplitHits );
	  nPixSplitHits.push_back( getInt );
	  // n ibl, split, shared
	  track->summaryValue( getInt, xAOD::numberOfInnermostPixelLayerHits );
	  nIMLPixHits.push_back( getInt );
	  track->summaryValue( getInt, xAOD::numberOfInnermostPixelLayerSharedHits );
	  nIMLPixSharedHits.push_back( getInt );
	  track->summaryValue( getInt, xAOD::numberOfInnermostPixelLayerSplitHits );
	  nIMLPixSplitHits.push_back( getInt );
	  // n bl,  split, shared
	  track->summaryValue( getInt, xAOD::numberOfNextToInnermostPixelLayerHits );
	  nNIMLPixHits.push_back( getInt );
	  track->summaryValue( getInt, xAOD::numberOfNextToInnermostPixelLayerSharedHits );
	  nNIMLPixSharedHits.push_back( getInt );
	  track->summaryValue( getInt, xAOD::numberOfNextToInnermostPixelLayerSplitHits );
	  nNIMLPixSplitHits.push_back( getInt );
	}
      }
    } // if ghostTrack available
    thisJet->m_jet_GhostTrack_pt. push_back( pt  );
    thisJet->m_jet_GhostTrack_qOverP. push_back( qOverP );
    thisJet->m_jet_GhostTrack_eta.push_back( eta );
    thisJet->m_jet_GhostTrack_phi.push_back( phi );
    thisJet->m_jet_GhostTrack_e.  push_back( e   );
    thisJet->m_jet_GhostTrack_d0. push_back( d0  );
    thisJet->m_jet_GhostTrack_z0. push_back( z0  );
    if( m_thisJetInfoSwitch[jetName]->m_allTrackDetail ) {
      thisJet->m_jet_GhostTrack_nPixHits.push_back( nPixHits );
      thisJet->m_jet_GhostTrack_nSCTHits.push_back( nSCTHits );
      thisJet->m_jet_GhostTrack_nTRTHits.push_back( nTRTHits );
      thisJet->m_jet_GhostTrack_nPixSharedHits.push_back( nPixSharedHits );
      thisJet->m_jet_GhostTrack_nPixSplitHits.push_back( nPixSplitHits );
      thisJet->m_jet_GhostTrack_nIMLPixHits.push_back( nIMLPixHits );
      thisJet->m_jet_GhostTrack_nIMLPixSharedHits.push_back( nIMLPixSharedHits );
      thisJet->m_jet_GhostTrack_nIMLPixSplitHits.push_back( nIMLPixSplitHits );
      thisJet->m_jet_GhostTrack_nNIMLPixHits.push_back( nNIMLPixHits );
      thisJet->m_jet_GhostTrack_nNIMLPixSharedHits.push_back( nNIMLPixSharedHits );
      thisJet->m_jet_GhostTrack_nNIMLPixSplitHits.push_back( nNIMLPixSplitHits );
    }
  } // allTrack switch

  if( m_thisJetInfoSwitch[jetName]->m_constituent ) {
    thisJet->m_jet_numConstituents.push_back( jet_itr->numConstituents() );
  }

  if( m_thisJetInfoSwitch[jetName]->m_constituentAll ) {
    thisJet->m_jet_constitWeights.push_back( jet_itr->getAttribute< std::vector<float> >( "constituentWeights" ) );
    std::vector<float> pt;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<float> e;
    xAOD::JetConstituentVector consVec = jet_itr->getConstituents();
    if( consVec.isValid() ) {
      // don't use auto since iterator can also set the scale ...
      // not sure what that does with auto - probably default but just incase
      // use the example provided in
      // http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODJet/xAODJet/JetConstituentVector.h
      xAOD::JetConstituentVector::iterator constit = consVec.begin();
      xAOD::JetConstituentVector::iterator constitE = consVec.end();
      for( ; constit != constitE; constit++){
	pt. push_back( constit->pt() / m_units );
	eta.push_back( constit->eta() );
	phi.push_back( constit->phi() );
	e.  push_back( constit->e() / m_units  );
      }
    }
    thisJet->m_jet_constit_pt. push_back( pt  );
    thisJet->m_jet_constit_eta.push_back( eta );
    thisJet->m_jet_constit_phi.push_back( phi );
    thisJet->m_jet_constit_e.  push_back( e   );
  }

  if ( m_thisJetInfoSwitch[jetName]->m_flavTag) {
    const xAOD::BTagging * myBTag = jet_itr->btagging();
    if ( !m_DC14 ) {

      static SG::AuxElement::ConstAccessor<double> SV0_significance3DAcc ("SV0_significance3D");
      if ( SV0_significance3DAcc.isAvailable(*myBTag) ) { thisJet->m_jet_sv0.push_back(  myBTag -> SV0_significance3D() ); }

      thisJet->m_jet_sv1.push_back(     myBTag -> SV1_loglikelihoodratio()   );
      thisJet->m_jet_ip3d.push_back(    myBTag -> IP3D_loglikelihoodratio()  );

    }
    thisJet->m_jet_sv1ip3d.push_back( myBTag -> SV1plusIP3D_discriminant() );
    thisJet->m_jet_mv1.push_back(     myBTag -> MV1_discriminant()         );

    //MV2c00 MV2c20 MV2c10 MV2c100 MV2m
    double val(-999);
    myBTag->variable<double>("MV2c00", "discriminant", val);
    thisJet->m_jet_mv2c00.push_back( val );
    myBTag->variable<double>("MV2c20", "discriminant", val);
    thisJet->m_jet_mv2c20.push_back( val );

    // flavor groups truth definition
    static SG::AuxElement::ConstAccessor<int> hadConeExclTruthLabel("HadronConeExclTruthLabelID");
    safeFill<int, int>(jet_itr, hadConeExclTruthLabel, thisJet->m_jet_hadConeExclTruthLabel, -999);

  }

  if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFix.empty() ) {
    for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFix.size(); i++ ) {
      switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFix.at(i) ) {
      case 30 : this->Fill_Fix30( jet_itr, thisJet );
	break;
      case 50 : this->Fill_Fix50( jet_itr, thisJet );
	break;
      case 60 : this->Fill_Fix60( jet_itr, thisJet );
	break;
      case 70 : this->Fill_Fix70( jet_itr, thisJet );
	break;
      case 77 : this->Fill_Fix77( jet_itr, thisJet );
	break;
      case 80 : this->Fill_Fix80( jet_itr, thisJet );
	break;
      case 85 : this->Fill_Fix85( jet_itr, thisJet );
	break;
      case 90 : this->Fill_Fix90( jet_itr, thisJet );
	break;
      }
    }
  } // sfFTagFix

  if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.empty() ) {
    for( unsigned int i=0; i<m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.size(); i++ ) {
      switch( m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.at(i) ) {
      case 30 : this->Fill_Flt30( jet_itr, thisJet );
	break;
      case 40 : this->Fill_Flt40( jet_itr, thisJet );
	break;
      case 50 : this->Fill_Flt50( jet_itr, thisJet );
	break;
      case 60 : this->Fill_Flt60( jet_itr, thisJet );
	break;
      case 70 : this->Fill_Flt70( jet_itr, thisJet );
	break;
      case 77 : this->Fill_Flt77( jet_itr, thisJet );
	break;
      case 85 : this->Fill_Flt85( jet_itr, thisJet );
	break;
      }
    }
  } // sfFTagFlt

  if ( m_thisJetInfoSwitch[jetName]->m_area ) {

    static SG::AuxElement::ConstAccessor<float> ghostArea("JetGhostArea");
    safeFill<float, float>(jet_itr, ghostArea, thisJet->m_jet_ghostArea, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea("ActiveArea");
    safeFill<float, float>(jet_itr, activeArea, thisJet->m_jet_activeArea, -999);

    static SG::AuxElement::ConstAccessor<float> voronoiArea("VoronoiArea");
    safeFill<float, float>(jet_itr, voronoiArea, thisJet->m_jet_voronoiArea, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_pt("ActiveArea4vec_pt");
    safeFill<float, float>(jet_itr, activeArea_pt, thisJet->m_jet_activeArea_pt, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_eta("ActiveArea4vec_eta");
    safeFill<float, float>(jet_itr, activeArea_eta, thisJet->m_jet_activeArea_eta, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_phi("ActiveArea4vec_phi");
    safeFill<float, float>(jet_itr, activeArea_phi, thisJet->m_jet_activeArea_phi, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_m("ActiveArea4vec_m");
    safeFill<float, float>(jet_itr, activeArea_m, thisJet->m_jet_activeArea_m, -999);
  }

  if ( m_thisJetInfoSwitch[jetName]->m_truth && m_isMC ) {

    static SG::AuxElement::ConstAccessor<int> ConeTruthLabelID ("ConeTruthLabelID");
    safeFill<int, int>(jet_itr, ConeTruthLabelID, thisJet->m_jet_truthConeLabelID, -999);

    static SG::AuxElement::ConstAccessor<int> TruthCount ("TruthCount");
    safeFill<int, int>(jet_itr, TruthCount, thisJet->m_jet_truthCount, -999);

    //    seems to be empty
    //      static SG::AuxElement::ConstAccessor<float> TruthPt ("TruthPt");
    //      if ( TruthPt.isAvailable( *jet_itr) ) {
    //        m_jet_truthPt.push_back( TruthPt( *jet_itr)/1000 );
    //      } else { m_jet_truthPt.push_back( -999 ); }

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_B ("TruthLabelDeltaR_B");
    safeFill<float, float>(jet_itr, TruthLabelDeltaR_B, thisJet->m_jet_truthDr_B, -999);

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_C ("TruthLabelDeltaR_C");
    safeFill<float, float>(jet_itr, TruthLabelDeltaR_C, thisJet->m_jet_truthDr_C, -999);

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_T ("TruthLabelDeltaR_T");
    safeFill<float, float>(jet_itr, TruthLabelDeltaR_T, thisJet->m_jet_truthDr_T, -999);

    static SG::AuxElement::ConstAccessor<int> partonLabel("PartonTruthLabelID");
    safeFill<int, int>(jet_itr, partonLabel, thisJet->m_jet_partonTruthID, -999);

    static SG::AuxElement::ConstAccessor<float> ghostTruthAssFrac("GhostTruthAssociationFraction");
    safeFill<float, float>(jet_itr, ghostTruthAssFrac, thisJet->m_jet_ghostTruthAssFrac, -999);

    const xAOD::Jet* truthJet = HelperFunctions::getLink<xAOD::Jet>( jet_itr, "GhostTruthAssociationLink" );
    if(truthJet) {
      thisJet->m_jet_truth_pt.push_back ( truthJet->pt() / m_units );
      thisJet->m_jet_truth_eta.push_back( truthJet->eta() );
      thisJet->m_jet_truth_phi.push_back( truthJet->phi() );
      thisJet->m_jet_truth_E.push_back  ( truthJet->e() / m_units );
    } else {
      thisJet->m_jet_truth_pt.push_back ( -999 );
      thisJet->m_jet_truth_eta.push_back( -999 );
      thisJet->m_jet_truth_phi.push_back( -999 );
      thisJet->m_jet_truth_E.push_back  ( -999 );
    }

  }

  if ( m_thisJetInfoSwitch[jetName]->m_truthDetails ) {

    //
    // B-Hadron Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostBHadronsFinalCount ("GhostBHadronsFinalCount");
    safeFill<int, int>(jet_itr, GhostBHadronsFinalCount, thisJet->m_jet_truthCount_BhadFinal, -999);

    static SG::AuxElement::ConstAccessor<int> GhostBHadronsInitialCount ("GhostBHadronsInitialCount");
    safeFill<int, int>(jet_itr, GhostBHadronsInitialCount, thisJet->m_jet_truthCount_BhadInit, -999);

    static SG::AuxElement::ConstAccessor<int> GhostBQuarksFinalCount ("GhostBQuarksFinalCount");
    safeFill<int, int>(jet_itr, GhostBQuarksFinalCount, thisJet->m_jet_truthCount_BQFinal, -999);

    static SG::AuxElement::ConstAccessor<float> GhostBHadronsFinalPt ("GhostBHadronsFinalPt");
    safeFill<float, float>(jet_itr, GhostBHadronsFinalPt, thisJet->m_jet_truthPt_BhadFinal, -999);

    static SG::AuxElement::ConstAccessor<float> GhostBHadronsInitialPt ("GhostBHadronsInitialPt");
    safeFill<float, float>(jet_itr, GhostBHadronsInitialPt, thisJet->m_jet_truthPt_BhadInit, -999);

    static SG::AuxElement::ConstAccessor<float> GhostBQuarksFinalPt ("GhostBQuarksFinalPt");
    safeFill<float, float>(jet_itr, GhostBQuarksFinalPt, thisJet->m_jet_truthPt_BQFinal, -999);

    //
    // C-Hadron Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostCHadronsFinalCount ("GhostCHadronsFinalCount");
    safeFill<int, int>(jet_itr, GhostCHadronsFinalCount, thisJet->m_jet_truthCount_ChadFinal, -999);

    static SG::AuxElement::ConstAccessor<int> GhostCHadronsInitialCount ("GhostCHadronsInitialCount");
    safeFill<int, int>(jet_itr, GhostCHadronsInitialCount, thisJet->m_jet_truthCount_ChadInit, -999);

    static SG::AuxElement::ConstAccessor<int> GhostCQuarksFinalCount ("GhostCQuarksFinalCount");
    safeFill<int, int>(jet_itr, GhostCQuarksFinalCount, thisJet->m_jet_truthCount_CQFinal, -999);

    static SG::AuxElement::ConstAccessor<float> GhostCHadronsFinalPt ("GhostCHadronsFinalPt");
    safeFill<float, float>(jet_itr, GhostCHadronsFinalPt, thisJet->m_jet_truthPt_ChadFinal, -999);

    static SG::AuxElement::ConstAccessor<float> GhostCHadronsInitialPt ("GhostCHadronsInitialPt");
    safeFill<float, float>(jet_itr, GhostCHadronsInitialPt, thisJet->m_jet_truthPt_ChadInit, -999);

    static SG::AuxElement::ConstAccessor<float> GhostCQuarksFinalPt ("GhostCQuarksFinalPt");
    safeFill<float, float>(jet_itr, GhostCQuarksFinalPt, thisJet->m_jet_truthPt_CQFinal, -999);

    //
    // Tau Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostTausFinalCount ("GhostTausFinalCount");
    safeFill<int, int>(jet_itr, GhostTausFinalCount, thisJet->m_jet_truthCount_TausFinal, -999);

    // THE ONLY UN-OFFICIAL PIECE OF CODE HERE USE WITH CAUTION
    static SG::AuxElement::ConstAccessor<float> GhostTausFinalPt ("GhostTausFinalPt");
    safeFill<float, float>(jet_itr, GhostTausFinalPt, thisJet->m_jet_truthPt_TausFinal, -999);

    // light quark(1,2,3) , gluon (21 or 9), charm(4) and b(5)
    // GhostPartons should select for these pdgIds only
    //    static SG::AuxElement::ConstAccessor< std::vector<const xAOD::TruthParticle*> > ghostPartons("GhostPartons");
    //    if( ghostPartons.isAvailable( *jet_itr )) {
    //    std::vector<const xAOD::TruthParticle*> truthPartons = ghostPartons( *jet_itr );

    std::vector<const xAOD::TruthParticle*> truthPartons = jet_itr->getAssociatedObjects<xAOD::TruthParticle>("GhostPartons");

    if( truthPartons.size() == 0){
      thisJet->m_jet_truth_pdgId.push_back(-999);
    } else {
      int iParent = 0;
      for(unsigned int i=1; i < truthPartons.size(); ++i){
	if( (truthPartons.at(i)->pt() > 0.001) && (truthPartons.at(i)->e() > truthPartons.at(iParent)->e()) )
	  iParent = i;
      }
      thisJet->m_jet_truth_pdgId.push_back(truthPartons.at(iParent)->pdgId());
      thisJet->m_jet_truth_partonPt.push_back(truthPartons.at(iParent)->pt() / m_units);
      thisJet->m_jet_truth_partonDR.push_back(truthPartons.at(iParent)->p4().DeltaR( jet_itr->p4() ));
    }

  }

  this->FillJetsUser(jet_itr, jetName);
  thisJet->N++;


  return;
}

void HelpTreeBase::ClearJets(const std::string jetName) {

  jetInfo* thisJet = m_jets[jetName];

  thisJet->N = 0;
  if( m_thisJetInfoSwitch[jetName]->m_kinematic ){
    thisJet->m_jet_pt.clear();
    thisJet->m_jet_eta.clear();
    thisJet->m_jet_phi.clear();
    thisJet->m_jet_E.clear();
  }


  // rapidity
  if( m_thisJetInfoSwitch[jetName]->m_rapidity ) {
    thisJet->m_jet_rapidity.clear();
  }

  // clean
  if( m_thisJetInfoSwitch[jetName]->m_clean ) {
    thisJet->m_jet_time.clear();
    thisJet->m_jet_LArQuality.clear();
    thisJet->m_jet_hecq.clear();
    thisJet->m_jet_negE.clear();
    thisJet->m_jet_avLArQF.clear();
    thisJet->m_jet_bchCorrCell.clear();
    thisJet->m_jet_N90Const.clear();
    thisJet->m_jet_LArBadHVEFrac.clear();
    thisJet->m_jet_LArBadHVNCell.clear();
    thisJet->m_jet_OotFracClus5.clear();
    thisJet->m_jet_OotFracClus10.clear();
    thisJet->m_jet_LeadingClusterPt.clear();
    thisJet->m_jet_LeadingClusterSecondLambda.clear();
    thisJet->m_jet_LeadingClusterCenterLambda.clear();
    thisJet->m_jet_LeadingClusterSecondR.clear();
    thisJet->m_jet_clean_passLooseBad.clear();
    thisJet->m_jet_clean_passLooseBadUgly.clear();
    thisJet->m_jet_clean_passTightBad.clear();
    thisJet->m_jet_clean_passTightBadUgly.clear();
  }

  // energy
  if ( m_thisJetInfoSwitch[jetName]->m_energy ) {
    thisJet->m_jet_HECf.clear();
    thisJet->m_jet_EMf.clear();
    thisJet->m_jet_centroidR.clear();
    thisJet->m_jet_fracSampMax.clear();
    thisJet->m_jet_fracSampMaxIdx.clear();
    thisJet->m_jet_lowEtFrac.clear();
    thisJet->m_jet_muonSegCount.clear();
    thisJet->m_jet_width.clear();
  }

  // each step of the calibration sequence
  if ( m_thisJetInfoSwitch[jetName]->m_scales ) {
    thisJet->m_jet_emPt.clear();
    thisJet->m_jet_constPt.clear();
    thisJet->m_jet_pileupPt.clear();
    thisJet->m_jet_originConstitPt.clear();
    thisJet->m_jet_etaJESPt.clear();
    thisJet->m_jet_gscPt.clear();
    thisJet->m_jet_insituPt.clear();
  }

  // layer
  if ( m_thisJetInfoSwitch[jetName]->m_layer ) {
    thisJet->m_jet_ePerSamp.clear();
  }

  // trackAll
  if ( m_thisJetInfoSwitch[jetName]->m_trackAll ) {
    thisJet->m_jet_NTrkPt1000.clear();
    thisJet->m_jet_SumPtPt1000.clear();
    thisJet->m_jet_TrkWPt1000.clear();
    thisJet->m_jet_NTrkPt500.clear();
    thisJet->m_jet_SumPtPt500.clear();
    thisJet->m_jet_TrkWPt500.clear();
    thisJet->m_jet_jvf.clear();
    //thisJet->m_jet_jvfloose.clear();
  }

  // trackPV
  if ( m_thisJetInfoSwitch[jetName]->m_trackPV ) {
    thisJet->m_jet_NTrkPt1000PV.clear();
    thisJet->m_jet_SumPtPt1000PV.clear();
    thisJet->m_jet_TrkWPt1000PV.clear();
    thisJet->m_jet_NTrkPt500PV.clear();
    thisJet->m_jet_SumPtPt500PV.clear();
    thisJet->m_jet_TrkWPt500PV.clear();
    thisJet->m_jet_jvfPV.clear();
  }

  if ( m_thisJetInfoSwitch[jetName]->m_trackAll || m_thisJetInfoSwitch[jetName]->m_trackPV ) {
    thisJet->m_jet_Jvt.clear();
    thisJet->m_jet_JvtJvfcorr.clear();
    thisJet->m_jet_JvtRpt.clear();
    //thisJet->m_jet_ghostTrackAssFrac.clear();
  }


  if ( m_thisJetInfoSwitch[jetName]->m_allTrack ) {
    thisJet->m_jet_GhostTrackCount.clear();
    thisJet->m_jet_GhostTrackPt.clear();
    thisJet->m_jet_GhostTrack_pt.clear();
    thisJet->m_jet_GhostTrack_qOverP.clear();
    thisJet->m_jet_GhostTrack_eta.clear();
    thisJet->m_jet_GhostTrack_phi.clear();
    thisJet->m_jet_GhostTrack_e.clear();
    thisJet->m_jet_GhostTrack_d0.clear();
    thisJet->m_jet_GhostTrack_z0.clear();
    if ( m_thisJetInfoSwitch[jetName]->m_allTrackDetail ) {
      thisJet->m_jet_GhostTrack_nPixHits.clear();
      thisJet->m_jet_GhostTrack_nSCTHits.clear();
      thisJet->m_jet_GhostTrack_nTRTHits.clear();
      thisJet->m_jet_GhostTrack_nPixSharedHits.clear();
      thisJet->m_jet_GhostTrack_nPixSplitHits.clear();
      thisJet->m_jet_GhostTrack_nIMLPixHits.clear();
      thisJet->m_jet_GhostTrack_nIMLPixSharedHits.clear();
      thisJet->m_jet_GhostTrack_nIMLPixSplitHits.clear();
      thisJet->m_jet_GhostTrack_nNIMLPixHits.clear();
      thisJet->m_jet_GhostTrack_nNIMLPixSharedHits.clear();
      thisJet->m_jet_GhostTrack_nNIMLPixSplitHits.clear();
    }
  }

  if( m_thisJetInfoSwitch[jetName]->m_constituent ) {
    thisJet->m_jet_numConstituents.clear();
  }

  if( m_thisJetInfoSwitch[jetName]->m_constituentAll ) {
    thisJet->m_jet_constitWeights.clear();
    thisJet->m_jet_constit_pt.clear();
    thisJet->m_jet_constit_eta.clear();
    thisJet->m_jet_constit_phi.clear();
    thisJet->m_jet_constit_e.clear();
  }

  // flavor tag
  if ( m_thisJetInfoSwitch[jetName]->m_flavTag ) {
    thisJet->m_jet_sv0.clear();
    thisJet->m_jet_sv1.clear();
    thisJet->m_jet_ip3d.clear();
    thisJet->m_jet_sv1ip3d.clear();
    thisJet->m_jet_mv1.clear();
    thisJet->m_jet_mv2c00.clear();
    thisJet->m_jet_mv2c20.clear();
    thisJet->m_jet_hadConeExclTruthLabel.clear();
  }

  if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFix.empty() ) { // just clear them all....

    thisJet->m_njet_mv2c20_Fix30 = 0;
    thisJet->m_jet_mv2c20_isFix30.clear();
    thisJet->m_weight_jet_mv2c20_sfFix30.clear();
    thisJet->m_jet_mv2c20_sfFix30.clear();

    thisJet->m_njet_mv2c20_Fix50 = 0;
    thisJet->m_jet_mv2c20_isFix50.clear();
    thisJet->m_weight_jet_mv2c20_sfFix50.clear();
    thisJet->m_jet_mv2c20_sfFix50.clear();

    thisJet->m_njet_mv2c20_Fix60 = 0;
    thisJet->m_jet_mv2c20_isFix60.clear();
    thisJet->m_weight_jet_mv2c20_sfFix60.clear();
    thisJet->m_jet_mv2c20_sfFix60.clear();

    thisJet->m_njet_mv2c20_Fix70 = 0;
    thisJet->m_jet_mv2c20_isFix70.clear();
    thisJet->m_weight_jet_mv2c20_sfFix70.clear();
    thisJet->m_jet_mv2c20_sfFix70.clear();

    thisJet->m_njet_mv2c20_Fix77 = 0;
    thisJet->m_jet_mv2c20_isFix77.clear();
    thisJet->m_weight_jet_mv2c20_sfFix77.clear();
    thisJet->m_jet_mv2c20_sfFix77.clear();

    thisJet->m_njet_mv2c20_Fix80 = 0;
    thisJet->m_jet_mv2c20_isFix80.clear();
    thisJet->m_weight_jet_mv2c20_sfFix80.clear();
    thisJet->m_jet_mv2c20_sfFix80.clear();

    thisJet->m_njet_mv2c20_Fix85 = 0;
    thisJet->m_jet_mv2c20_isFix85.clear();
    thisJet->m_weight_jet_mv2c20_sfFix85.clear();
    thisJet->m_jet_mv2c20_sfFix85.clear();

    thisJet->m_njet_mv2c20_Fix90 = 0;
    thisJet->m_jet_mv2c20_isFix90.clear();
    thisJet->m_weight_jet_mv2c20_sfFix90.clear();
    thisJet->m_jet_mv2c20_sfFix90.clear();
  }

  if( !m_thisJetInfoSwitch[jetName]->m_sfFTagFlt.empty() ) { // just clear them all....

    thisJet->m_njet_mv2c20_Flt30 = 0;
    thisJet->m_jet_mv2c20_isFlt30.clear();
    thisJet->m_weight_jet_mv2c20_sfFlt30.clear();
    thisJet->m_jet_mv2c20_sfFlt30.clear();

    thisJet->m_njet_mv2c20_Flt40 = 0;
    thisJet->m_jet_mv2c20_isFlt40.clear();
    thisJet->m_weight_jet_mv2c20_sfFlt40.clear();
    thisJet->m_jet_mv2c20_sfFlt40.clear();

    thisJet->m_njet_mv2c20_Flt50 = 0;
    thisJet->m_jet_mv2c20_isFlt50.clear();
    thisJet->m_weight_jet_mv2c20_sfFlt50.clear();
    thisJet->m_jet_mv2c20_sfFlt50.clear();

    thisJet->m_njet_mv2c20_Flt60 = 0;
    thisJet->m_jet_mv2c20_isFlt60.clear();
    thisJet->m_weight_jet_mv2c20_sfFlt60.clear();
    thisJet->m_jet_mv2c20_sfFlt60.clear();

    thisJet->m_njet_mv2c20_Flt70 = 0;
    thisJet->m_jet_mv2c20_isFlt70.clear();
    thisJet->m_weight_jet_mv2c20_sfFlt70.clear();
    thisJet->m_jet_mv2c20_sfFlt70.clear();

    thisJet->m_njet_mv2c20_Flt77 = 0;
    thisJet->m_jet_mv2c20_isFlt77.clear();
    thisJet->m_weight_jet_mv2c20_sfFlt77.clear();
    thisJet->m_jet_mv2c20_sfFlt77.clear();

    thisJet->m_njet_mv2c20_Flt85 = 0;
    thisJet->m_jet_mv2c20_isFlt85.clear();
    thisJet->m_weight_jet_mv2c20_sfFlt85.clear();
    thisJet->m_jet_mv2c20_sfFlt85.clear();
  }

  if ( m_thisJetInfoSwitch[jetName]->m_area ) {
    thisJet->m_jet_ghostArea.clear();
    thisJet->m_jet_activeArea.clear();
    thisJet->m_jet_voronoiArea.clear();
    thisJet->m_jet_activeArea_pt.clear();
    thisJet->m_jet_activeArea_eta.clear();
    thisJet->m_jet_activeArea_phi.clear();
    thisJet->m_jet_activeArea_m.clear();
  }

  // truth
  if ( m_thisJetInfoSwitch[jetName]->m_truth && m_isMC ) {
    thisJet->m_jet_truthConeLabelID.clear();
    thisJet->m_jet_truthCount.clear();
    thisJet->m_jet_truthPt.clear();
    thisJet->m_jet_truthDr_B.clear();
    thisJet->m_jet_truthDr_C.clear();
    thisJet->m_jet_truthDr_T.clear();
    thisJet->m_jet_partonTruthID.clear();
    thisJet->m_jet_ghostTruthAssFrac.clear();
    thisJet->m_jet_truth_pt.clear();
    thisJet->m_jet_truth_eta.clear();
    thisJet->m_jet_truth_phi.clear();
    thisJet->m_jet_truth_E.clear();
  }

  // truth_detail
  if ( m_thisJetInfoSwitch[jetName]->m_truthDetails ) {
    thisJet->m_jet_truthCount_BhadFinal.clear();
    thisJet->m_jet_truthCount_BhadInit.clear();
    thisJet->m_jet_truthCount_BQFinal.clear();
    thisJet->m_jet_truthPt_BhadFinal.clear();
    thisJet->m_jet_truthPt_BhadInit.clear();
    thisJet->m_jet_truthPt_BQFinal.clear();
    thisJet->m_jet_truthCount_ChadFinal.clear();
    thisJet->m_jet_truthCount_ChadInit.clear();
    thisJet->m_jet_truthCount_CQFinal.clear();
    thisJet->m_jet_truthPt_ChadFinal.clear();
    thisJet->m_jet_truthPt_ChadInit.clear();
    thisJet->m_jet_truthPt_CQFinal.clear();
    thisJet->m_jet_truthCount_TausFinal.clear();
    thisJet->m_jet_truthPt_TausFinal.clear();
    thisJet->m_jet_truth_pdgId.clear();
    thisJet->m_jet_truth_partonPt.clear();
    thisJet->m_jet_truth_partonDR.clear();
  }

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
    m_tree->Branch("fatjet_tau32_wta",   &m_fatjet_tau32_wta);
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
      static SG::AuxElement::ConstAccessor<float> tau2_wta ("Tau2_wta");
      static SG::AuxElement::ConstAccessor<float> tau3_wta ("Tau3_wta");
      if ( tau2_wta.isAvailable( *fatjet_itr ) and tau3_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau32_wta.push_back( tau3_wta( *fatjet_itr ) / tau2_wta( *fatjet_itr ) );
      } else { m_fatjet_tau32_wta.push_back( -999 ); }

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
    m_fatjet_tau32_wta.clear();
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

void HelpTreeBase::Fill_Fix30( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix30("BTag_FixedCutBEff_30");
  if( isFix30.isAvailable( *jet ) ) {
    if ( isFix30( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix30;
    thisJet->m_jet_mv2c20_isFix30.push_back( isFix30( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix30.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix30("BTag_SF_FixedCutBEff_30");
  if ( sfFix30.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix30.push_back( sfFix30( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix30.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix50( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix50("BTag_FixedCutBEff_50");
  if( isFix50.isAvailable( *jet ) ) {
    if ( isFix50( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix50;
    thisJet->m_jet_mv2c20_isFix50.push_back( isFix50( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix50.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix50("BTag_SF_FixedCutBEff_50");
  if ( sfFix50.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix50.push_back( sfFix50( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix50.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix60( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix60("BTag_FixedCutBEff_60");
  if( isFix60.isAvailable( *jet ) ) {
    if ( isFix60( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix60;
    thisJet->m_jet_mv2c20_isFix60.push_back( isFix60( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix60.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix60("BTag_SF_FixedCutBEff_60");
  if ( sfFix60.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix60.push_back( sfFix60( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix60.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix70( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix70("BTag_FixedCutBEff_70");
  if( isFix70.isAvailable( *jet ) ) {
    if ( isFix70( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix70;
    thisJet->m_jet_mv2c20_isFix70.push_back( isFix70( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix70.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix70("BTag_SF_FixedCutBEff_70");
  if ( sfFix70.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix70.push_back( sfFix70( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix70.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix77( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix77("BTag_FixedCutBEff_77");
  if( isFix77.isAvailable( *jet ) ) {
    if ( isFix77( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix77;
    thisJet->m_jet_mv2c20_isFix77.push_back( isFix77( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix77.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix77("BTag_SF_FixedCutBEff_77");
  if ( sfFix77.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix77.push_back( sfFix77( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix77.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix80( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix80("BTag_FixedCutBEff_80");
  if( isFix80.isAvailable( *jet ) ) {
    if ( isFix80( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix80;
    thisJet->m_jet_mv2c20_isFix80.push_back( isFix80( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix80.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix80("BTag_SF_FixedCutBEff_80");
  if ( sfFix80.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix80.push_back( sfFix80( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix80.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix85( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix85("BTag_FixedCutBEff_85");
  if( isFix85.isAvailable( *jet ) ) {
    if ( isFix85( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix85;
    thisJet->m_jet_mv2c20_isFix85.push_back( isFix85( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix85.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix85("BTag_SF_FixedCutBEff_85");
  if ( sfFix85.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix85.push_back( sfFix85( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix85.push_back(junk);
  }
}

void HelpTreeBase::Fill_Fix90( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFix90("BTag_FixedCutBEff_90");
  if( isFix90.isAvailable( *jet ) ) {
    if ( isFix90( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Fix90;
    thisJet->m_jet_mv2c20_isFix90.push_back( isFix90( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFix90.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFix90("BTag_SF_FixedCutBEff_90");
  if ( sfFix90.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFix90.push_back( sfFix90( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFix90.push_back(junk);
  }
}


///// FLAT EFFICIENCY

void HelpTreeBase::Fill_Flt30( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt30("BTag_FlatBEff_30");
  if( isFlt30.isAvailable( *jet ) ) {
    if ( isFlt30( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Flt30;
    thisJet->m_jet_mv2c20_isFlt30.push_back( isFlt30( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFlt30.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt30("BTag_SF_FlatBEff_30");
  if ( sfFlt30.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFlt30.push_back( sfFlt30( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFlt30.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt40( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt40("BTag_FlatBEff_40");
  if( isFlt40.isAvailable( *jet ) ) {
    if ( isFlt40( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Flt40;
    thisJet->m_jet_mv2c20_isFlt40.push_back( isFlt40( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFlt40.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt40("BTag_SF_FlatBEff_40");
  if ( sfFlt40.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFlt40.push_back( sfFlt40( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFlt40.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt50( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt50("BTag_FlatBEff_50");
  if( isFlt50.isAvailable( *jet ) ) {
    if ( isFlt50( *jet ) == 1 )  ++thisJet->m_njet_mv2c20_Flt50;
    thisJet->m_jet_mv2c20_isFlt50.push_back( isFlt50( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFlt50.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt50("BTag_SF_FlatBEff_50");
  if ( sfFlt50.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFlt50.push_back( sfFlt50( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFlt50.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt60( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt60("BTag_FlatBEff_60");
  if( isFlt60.isAvailable( *jet ) ) {
    if ( isFlt60( *jet ) == 1 )  ++thisJet->m_njet_mv2c20_Flt60;
    thisJet->m_jet_mv2c20_isFlt60.push_back( isFlt60( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFlt60.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt60("BTag_SF_FlatBEff_60");
  if ( sfFlt60.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFlt60.push_back( sfFlt60( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFlt60.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt70( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt70("BTag_FlatBEff_70");
  if( isFlt70.isAvailable( *jet ) ) {
    if ( isFlt70( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Flt70;
    thisJet->m_jet_mv2c20_isFlt70.push_back( isFlt70( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFlt70.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt70("BTag_SF_FlatBEff_70");
  if ( sfFlt70.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFlt70.push_back( sfFlt70( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFlt70.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt77( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt77("BTag_FlatBEff_77");
  if( isFlt77.isAvailable( *jet ) ) {
    if ( isFlt77( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Flt77;
    thisJet->m_jet_mv2c20_isFlt77.push_back( isFlt77( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFlt77.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt77("BTag_SF_FlatBEff_77");
  if ( sfFlt77.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFlt77.push_back( sfFlt77( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFlt77.push_back(junk);
  }
}

void HelpTreeBase::Fill_Flt85( const xAOD::Jet* jet, jetInfo* thisJet ) {
  static SG::AuxElement::ConstAccessor< int > isFlt85("BTag_FlatBEff_85");
  if( isFlt85.isAvailable( *jet ) ) {
    if ( isFlt85( *jet ) == 1 ) ++thisJet->m_njet_mv2c20_Flt85;
    thisJet->m_jet_mv2c20_isFlt85.push_back( isFlt85( *jet ) );
  } else { thisJet->m_jet_mv2c20_isFlt85.push_back( -1 ); }
  if(!m_isMC) { return; }
  static SG::AuxElement::ConstAccessor< std::vector<float> > sfFlt85("BTag_SF_FlatBEff_85");
  if ( sfFlt85.isAvailable( *jet ) ) {
    thisJet->m_jet_mv2c20_sfFlt85.push_back( sfFlt85( *jet ) );
  } else {
    std::vector<float> junk(1,-999);
    thisJet->m_jet_mv2c20_sfFlt85.push_back(junk);
  }
}

