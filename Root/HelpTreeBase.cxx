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

HelpTreeBase::HelpTreeBase(xAOD::TEvent* event, TTree* tree, TFile* file, const float units, bool debug, bool DC14):
  m_eventInfoSwitch(nullptr),
  m_trigInfoSwitch(nullptr),
  m_muInfoSwitch(nullptr),
  m_elInfoSwitch(nullptr),
  m_jetInfoSwitch(nullptr),
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
  //m_store = store;
  Info("HelpTreeBase()", "HelpTreeBase setup");

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
  m_tree->Branch("eventNumber",        &m_eventNumber,    "eventNumber/I");
  m_tree->Branch("mcEventNumber",      &m_mcEventNumber,  "mcEventNumber/I");
  m_tree->Branch("mcChannelNumber",    &m_mcChannelNumber,"mcChannelNumber/I");
  m_tree->Branch("mcEventWeight",      &m_mcEventWeight,  "mcEventWeight/F");
  m_tree->Branch("weight_pileup",      &m_weight_pileup,  "weight_pileup/F");

  if ( m_eventInfoSwitch->m_pileup ) {
    m_tree->Branch("NPV",                &m_npv,            "NPV/I");
    m_tree->Branch("actualInteractionsPerCrossing",  &m_actualMu,  "actualInteractionsPerCrossing/F");
    m_tree->Branch("averageInteractionsPerCrossing", &m_averageMu, "averageInteractionsPerCrossing/F");
    m_tree->Branch("lumiBlock", &m_lumiBlock, "lumiBlock/I");
  }

  if ( m_eventInfoSwitch->m_shapeEM ) {
    m_tree->Branch("rhoEM",                &m_rhoEM,            "rhoEM/D");
  }

  if ( m_eventInfoSwitch->m_shapeLC ) {
    m_tree->Branch("rhoLC",                &m_rhoLC,            "rhoLC/D");
  }

  if( m_eventInfoSwitch->m_truth ) {
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

void HelpTreeBase::FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event ) {

  this->ClearEvent();
  this->ClearEventUser();

  m_runNumber             = eventInfo->runNumber();
  m_eventNumber           = eventInfo->eventNumber();
  if ( eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) ) {
    m_mcEventNumber         = eventInfo->mcEventNumber();
    m_mcChannelNumber       = eventInfo->mcChannelNumber();
    m_mcEventWeight         = eventInfo->mcEventWeight();
  } else {
    m_mcEventNumber         = -1;
    m_mcChannelNumber       = -1;
    m_mcEventWeight	    = 1.;
  }
  static SG::AuxElement::ConstAccessor< double > weight_pileup ("PileupWeight");
  if ( weight_pileup.isAvailable( *eventInfo ) ) {
    m_weight_pileup = weight_pileup( *eventInfo );
  } else { m_weight_pileup = 1.; }


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
    m_lumiBlock = eventInfo->lumiBlock();

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

  if( m_eventInfoSwitch->m_truth && m_event ) {
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
    m_tree->Branch("passAny",         &m_passAny,      "passAny/I"     );
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
    if( passAny.isAvailable( *eventInfo ) ) { m_passAny = passAny( *eventInfo ); }
    else { m_passAny = -999; }
    static SG::AuxElement::ConstAccessor< int > passL1("passL1");
    if( passL1.isAvailable( *eventInfo ) ) { m_passL1 = passL1( *eventInfo ); }
    else { m_passL1 = -999; }
    static SG::AuxElement::ConstAccessor< int > passHLT("passHLT");
    if( passHLT.isAvailable( *eventInfo ) ) { m_passHLT = passHLT( *eventInfo ); }
    else { m_passHLT = -999; }

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

    if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Switch: m_passTriggers"); }
    static SG::AuxElement::ConstAccessor< std::vector< std::string > > passTrigs("passTriggers");
    if( passTrigs.isAvailable( *eventInfo ) ) { m_passTriggers = passTrigs( *eventInfo ); }

  }
  
}

// Clear Trigger
void HelpTreeBase::ClearTrigger() {

  m_passAny = -999;
  m_passL1  = -999;
  m_passHLT = -999;

  m_masterKey = 0;
  m_L1PSKey   = 0;
  m_HLTPSKey  = 0;

  m_passTriggers.clear();

}

/*********************
 *
 *   JET TRIGGER
 *
 ********************/

/* TODO: jet trigger */
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
    m_tree->Branch("muon_isTrigMatched", &m_muon_isTrigMatched);
  }

  if ( m_muInfoSwitch->m_isolation ) {
    m_tree->Branch("muon_isIsolated",     &m_muon_isIsolated);
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

    static SG::AuxElement::Accessor<char> isTrigMatchedAcc("isTrigMatched");
    if ( m_muInfoSwitch->m_trigger ) {
      if ( isTrigMatchedAcc.isAvailable( *muon_itr ) ) { m_muon_isTrigMatched.push_back( isTrigMatchedAcc( *muon_itr ) ); } else { m_muon_isTrigMatched.push_back( -1 );}
    }

    if ( m_muInfoSwitch->m_isolation ) {
      static SG::AuxElement::Accessor<char> isIsoAcc ("isIsolated");
      if ( isIsoAcc.isAvailable( *muon_itr ) ) { m_muon_isIsolated.push_back( isIsoAcc( *muon_itr ) ); } else { m_muon_isIsolated.push_back( -1 ); }
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
	// The refernce  frame is chosen to be a system centered in the beamspot position, with z axis parallel to the beam line.
	// Remember that the beamspot size ( of O(10 micrometers) in the transverse plane) is << average vertex transverse position resolution ( O(60-80 micrometers) )
	// The coordinates of this r.f. wrt. the ATLAS system origin are returned by means of vx(), vy(), vz()
	//
        m_muon_trkd0.push_back( trk->d0() );
        static SG::AuxElement::Accessor<float> d0SigAcc ("d0sig");
        float d0_significance =  ( d0SigAcc.isAvailable( *muon_itr ) ) ? fabs( d0SigAcc( *muon_itr ) ) : -9999.0;
	m_muon_trkd0sig.push_back( d0_significance );
	float z0 =   trk->z0()  - ( primaryVertex->z() - trk->vz() ) ; // distance between z0 and zPV ( after referring the PV z coordinate to the beamspot position, given by vz() )
								       // see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackingDC14 for further reference
	float theta = trk->theta();
        m_muon_trkz0.push_back( z0 );
        m_muon_trkz0sintheta.push_back(  z0 * sin(theta) );
        m_muon_trkphi0.push_back( trk->phi0() );
        m_muon_trktheta.push_back( theta );
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
  }

  if ( m_muInfoSwitch->m_isolation ) {
    m_muon_isIsolated.clear();
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
  }

  if ( m_elInfoSwitch->m_trigger ){
    // a vector of trigger match decision for each electron trigger chain
    m_tree->Branch( "el_isTrigMatchedToChain", &m_el_isTrigMatchedToChain );
    // a vector of strings for each electron trigger chain - 1:1 correspondence w/ vector above
    m_tree->Branch( "el_listTrigChains", &m_el_listTrigChains );
  }

  if ( m_elInfoSwitch->m_isolation ) {
    m_tree->Branch("el_isIsolated",       &m_el_isIsolated);    
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
    m_tree->Branch("el_LHVeryLoose",  &m_el_LHVeryLoose);
    m_tree->Branch("el_LHLoose",      &m_el_LHLoose);
    m_tree->Branch("el_LHMedium",     &m_el_LHMedium);
    m_tree->Branch("el_LHTight",      &m_el_LHTight);
    m_tree->Branch("el_IsEMLoose",    &m_el_IsEMLoose);
    m_tree->Branch("el_IsEMMedium",   &m_el_IsEMMedium);
    m_tree->Branch("el_IsEMTight",    &m_el_IsEMTight);
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

  for ( auto el_itr : *(electrons) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillElectrons()", "Filling electron w/ pT = %2f", el_itr->pt() / m_units ); }

    const xAOD::TrackParticle* trk = el_itr->trackParticle();

    if ( m_elInfoSwitch->m_kinematic ) {
      m_el_pt.push_back ( (el_itr)->pt() / m_units );
      m_el_eta.push_back( (el_itr)->eta() );
      m_el_phi.push_back( (el_itr)->phi() );
      m_el_m.push_back  ( (el_itr)->m() / m_units );
    }

    if ( m_elInfoSwitch->m_trigger ) {

      // retrieve map<string,char> w/ chain,isMatched
      //
      static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapElAcc("isTrigMatchedMapEl");

      if ( isTrigMatchedMapElAcc.isAvailable( *el_itr ) ) {
	 // loop over map and fill branches
	 //
	 for ( auto const &it : (isTrigMatchedMapElAcc( *el_itr )) ) {
  	   m_el_isTrigMatchedToChain.push_back( static_cast<int>(it.second) );
	   m_el_listTrigChains.push_back( it.first );
	 }
       } else { 
	 m_el_isTrigMatchedToChain.push_back( -1 );
	 m_el_listTrigChains.push_back("NONE"); 
       }  
      
    }

    if ( m_elInfoSwitch->m_isolation ) {
      static SG::AuxElement::Accessor<char> isIsoAcc ("isIsolated");
      if ( isIsoAcc.isAvailable( *el_itr ) ) { m_el_isIsolated.push_back( isIsoAcc( *el_itr ) ); } else { m_el_isIsolated.push_back( -1 ); }
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

      static SG::AuxElement::Accessor<char> LHVeryLooseAcc ("LHVeryLoose");
      static SG::AuxElement::Accessor<char> LHLooseAcc ("LHLoose");
      static SG::AuxElement::Accessor<char> LHMediumAcc ("LHMedium");
      static SG::AuxElement::Accessor<char> LHTightAcc ("LHTight");

      static SG::AuxElement::Accessor<char> EMLooseAcc ("Loose");
      static SG::AuxElement::Accessor<char> EMMediumAcc ("Medium");
      static SG::AuxElement::Accessor<char> EMTightAcc ("Tight");

      if ( LHVeryLooseAcc.isAvailable( *el_itr ) ) { m_el_LHVeryLoose.push_back( LHVeryLooseAcc( *el_itr ) ); } else { m_el_LHVeryLoose.push_back( -1 ); }
      if ( LHLooseAcc.isAvailable( *el_itr ) )     { m_el_LHLoose.push_back( LHLooseAcc( *el_itr ) );         } else { m_el_LHLoose.push_back( -1 ); }
      if ( LHMediumAcc.isAvailable( *el_itr ) )    { m_el_LHMedium.push_back( LHMediumAcc( *el_itr ) );       } else { m_el_LHMedium.push_back( -1 ); }
      if ( LHTightAcc.isAvailable( *el_itr ) )     { m_el_LHTight.push_back( LHTightAcc( *el_itr ) );         } else { m_el_LHTight.push_back( -1 ); }

      if ( EMLooseAcc.isAvailable( *el_itr ) )         { m_el_IsEMLoose.push_back( EMLooseAcc( *el_itr ) );   } else { m_el_IsEMLoose.push_back( -1 ); }
      if ( EMMediumAcc.isAvailable( *el_itr ) )        { m_el_IsEMMedium.push_back( EMMediumAcc( *el_itr ) ); } else { m_el_IsEMMedium.push_back( -1 ); }
      if ( EMTightAcc.isAvailable( *el_itr ) )         { m_el_IsEMTight.push_back( EMTightAcc( *el_itr ) );   } else { m_el_IsEMTight.push_back( -1 ); }
    }

    if ( m_elInfoSwitch->m_trackparams ) {
      if ( trk ) {
        //
	// NB.:
	// All track parameters are calculated at the perigee, i.e., the point of closest approach to the origin of some r.f. (which in RunII is NOT the ATLAS detector r.f!).
	// The refernce  frame is chosen to be a system centered in the beamspot position, with z axis parallel to the beam line.
	// Remember that the beamspot size ( of O(10 micrometers) in the transverse plane) is << average vertex transverse position resolution ( O(60-80 micrometers) )
	// The coordinates of this r.f. wrt. the ATLAS system origin are returned by means of vx(), vy(), vz()
	//
        m_el_trkd0.push_back( trk->d0() );
        static SG::AuxElement::Accessor<float> d0SigAcc ("d0sig");
        float d0_significance =  ( d0SigAcc.isAvailable( *el_itr ) ) ? fabs( d0SigAcc( *el_itr ) ) : -9999.0;
        m_el_trkd0sig.push_back( d0_significance );
	float z0 =  trk->z0()  - ( primaryVertex->z() - trk->vz() ) ; // distance between z0 and zPV ( after referring the PV z coordinate to the beamspot position, given by vz() )
								      // see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackingDC14 for further reference
	float theta = trk->theta();
        m_el_trkz0.push_back( z0 );
        m_el_trkz0sintheta.push_back( z0 * sin(theta) );
        m_el_trkphi0.push_back( trk->phi0() );
        m_el_trktheta.push_back( theta );
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
  }

  if ( m_elInfoSwitch->m_trigger ) {   
    m_el_isTrigMatchedToChain.clear();
    m_el_listTrigChains.clear();
  }
  
  if ( m_elInfoSwitch->m_isolation ) {
    m_el_isIsolated.clear();
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
    m_el_LHVeryLoose.clear();
    m_el_LHLoose.clear();
    m_el_LHMedium.clear();
    m_el_LHTight.clear();
    m_el_IsEMLoose.clear();
    m_el_IsEMMedium.clear();
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

}

/*********************
 *
 *   JETS
 *
 ********************/

void HelpTreeBase::AddJets(const std::string detailStr)
{

  if(m_debug) Info("AddJets()", "Adding jet variables: %s", detailStr.c_str());

  m_jetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );

  // always
  m_tree->Branch("njets",    &m_njet,"njets/I");

  if ( m_jetInfoSwitch->m_kinematic ) {
    m_tree->Branch("jet_E",   &m_jet_E);
    m_tree->Branch("jet_pt",  &m_jet_pt);
    m_tree->Branch("jet_phi", &m_jet_phi);
    m_tree->Branch("jet_eta", &m_jet_eta);
  }

  if ( m_jetInfoSwitch->m_rapidity ) {
    m_tree->Branch("jet_rapidity", &m_jet_rapidity);
  }

  if( m_jetInfoSwitch->m_clean ) {
    m_tree->Branch("jet_Timing",              &m_jet_time               );
    m_tree->Branch("jet_LArQuality",          &m_jet_LArQuality         );
    m_tree->Branch("jet_HECQuality",          &m_jet_hecq               );
    m_tree->Branch("jet_NegativeE",           &m_jet_negE               );
    m_tree->Branch("jet_AverageLArQF",        &m_jet_avLArQF            );
    m_tree->Branch("jet_BchCorrCell",         &m_jet_bchCorrCell        );
    m_tree->Branch("jet_N90Constituents",     &m_jet_N90Const           );
    m_tree->Branch("jet_LArBadHVEFracnergyFrac", &m_jet_LArBadHVEFrac   );
    m_tree->Branch("jet_LArBadHVNCell",       &m_jet_LArBadHVNCell  	  );
    m_tree->Branch("jet_OotFracClusters5",    &m_jet_OotFracClus5  	    );
    m_tree->Branch("jet_OotFracClusters10",   &m_jet_OotFracClus10  	  );
    m_tree->Branch("jet_LeadingClusterPt",              &m_jet_LeadingClusterPt  	            );
    m_tree->Branch("jet_LeadingClusterSecondLambda",    &m_jet_LeadingClusterSecondLambda  	  );
    m_tree->Branch("jet_LeadingClusterCenterLambda",    &m_jet_LeadingClusterCenterLambda  	  );
    m_tree->Branch("jet_LeadingClusterSecondR",         &m_jet_LeadingClusterSecondR  	      );
    m_tree->Branch("jet_clean_passLooseBad",            &m_jet_clean_passLooseBad             );
    m_tree->Branch("jet_clean_passLooseBadUgly",        &m_jet_clean_passLooseBadUgly         );
    m_tree->Branch("jet_clean_passTightBad",            &m_jet_clean_passTightBad             );
    m_tree->Branch("jet_clean_passTightBadUgly",        &m_jet_clean_passTightBadUgly         );
  }

  if ( m_jetInfoSwitch->m_energy ) {
    m_tree->Branch("jet_HECFrac", 	            &m_jet_HECf	    );
    m_tree->Branch("jet_EMFrac",  	            &m_jet_EMf	    );
    m_tree->Branch("jet_CentroidR",	            &m_jet_centroidR      );
    m_tree->Branch("jet_FracSamplingMax",       &m_jet_fracSampMax    );
    m_tree->Branch("jet_FracSamplingMaxIndex",  &m_jet_fracSampMaxIdx );
    m_tree->Branch("jet_LowEtConstituentsFrac", &m_jet_lowEtFrac      );
    m_tree->Branch("jet_GhostMuonSegmentCount", &m_jet_muonSegCount   );
    m_tree->Branch("jet_Width",                 &m_jet_width          );
  }

  if ( m_jetInfoSwitch->m_scales ) {
    m_tree->Branch("jet_emScalePt",              &m_jet_emPt            );
    m_tree->Branch("jet_pileupScalePt",          &m_jet_pileupPt        );
    m_tree->Branch("jet_originConstitScalePt",   &m_jet_originConstitPt );
    m_tree->Branch("jet_etaJESScalePt",          &m_jet_etaJESPt        );
    m_tree->Branch("jet_gscScalePt",             &m_jet_gscPt           );
    m_tree->Branch("jet_insituScalePt",          &m_jet_insituPt        );
  }

  if ( m_jetInfoSwitch->m_layer ) {
    m_tree->Branch("jet_EnergyPerSampling",     &m_jet_ePerSamp   );
  }

  if ( m_jetInfoSwitch->m_trackAll ) {
    m_tree->Branch("jet_NumTrkPt1000",	    &m_jet_NTrkPt1000   );
    m_tree->Branch("jet_SumPtTrkPt1000",    &m_jet_SumPtPt1000  );
    m_tree->Branch("jet_TrackWidthPt1000",  &m_jet_TrkWPt1000   );
    m_tree->Branch("jet_NumTrkPt500",	      &m_jet_NTrkPt500    );
    m_tree->Branch("jet_SumPtTrkPt500",	    &m_jet_SumPtPt500   );
    m_tree->Branch("jet_TrackWidthPt500",   &m_jet_TrkWPt500    );
    m_tree->Branch("jet_JVF",		            &m_jet_jvf	        );
  }

  if ( m_jetInfoSwitch->m_trackPV ) {
    m_tree->Branch("jet_NumTrkPt1000PV",      &m_jet_NTrkPt1000PV   );
    m_tree->Branch("jet_SumPtTrkPt1000PV",    &m_jet_SumPtPt1000PV  );
    m_tree->Branch("jet_TrackWidthPt1000PV",  &m_jet_TrkWPt1000PV   );
    m_tree->Branch("jet_NumTrkPt500PV",	      &m_jet_NTrkPt500PV    );
    m_tree->Branch("jet_SumPtTrkPt500PV",     &m_jet_SumPtPt500PV   );
    m_tree->Branch("jet_TrackWidthPt500PV",   &m_jet_TrkWPt500PV    );
    m_tree->Branch("jet_JVFPV",		            &m_jet_jvfPV	        );
    m_tree->Branch("jet_Jvt",		              &m_jet_Jvt	          );
    m_tree->Branch("jet_JvtJvfcorr",		      &m_jet_JvtJvfcorr     );
    m_tree->Branch("jet_JvtRpt",              &m_jet_JvtRpt         );
    //m_tree->Branch("jet_GhostTrackAssociationFraction", &m_jet_ghostTrackAssFrac);
  }

  if ( m_jetInfoSwitch->m_allTrack ) {
    // if want to apply the selection of the PV then need to setup track selection tool
    // this applies the JVF/JVT selection cuts
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JvtManualRecalculation
    if( m_jetInfoSwitch->m_allTrackPVSel ) {
      m_trkSelTool = new InDet::InDetTrackSelectionTool( "JetTrackSelection" );
      m_trkSelTool->setCutLevel( InDet::CutLevel::Loose );
      m_trkSelTool->initialize();
      // to do this need to have AddJets return a status code
      //RETURN_CHECK( "HelpTreeBase::JetTrackSelection", m_trkSelTool->initialize(), "");
    }
    m_tree->Branch("jet_GhostTrackCount",  &m_jet_GhostTrackCount );
    m_tree->Branch("jet_GhostTrackPt",     &m_jet_GhostTrackPt    );
    m_tree->Branch("jet_GhostTrack_pt",    &m_jet_GhostTrack_pt   );
    m_tree->Branch("jet_GhostTrack_qOverP",&m_jet_GhostTrack_qOverP);
    m_tree->Branch("jet_GhostTrack_eta",   &m_jet_GhostTrack_eta  );
    m_tree->Branch("jet_GhostTrack_phi",   &m_jet_GhostTrack_phi  );
    m_tree->Branch("jet_GhostTrack_e",     &m_jet_GhostTrack_e    );
    m_tree->Branch("jet_GhostTrack_d0",    &m_jet_GhostTrack_d0   );
    m_tree->Branch("jet_GhostTrack_z0",    &m_jet_GhostTrack_z0   );
    if ( m_jetInfoSwitch->m_allTrackDetail ) {
      m_tree->Branch("jet_GhostTrack_nPixelHits", &m_jet_GhostTrack_nPixHits);
      m_tree->Branch("jet_GhostTrack_nSCTHits",   &m_jet_GhostTrack_nSCTHits);
      m_tree->Branch("jet_GhostTrack_nTRTHits",   &m_jet_GhostTrack_nTRTHits);
      m_tree->Branch("jet_GhostTrack_nPixelSharedHits", &m_jet_GhostTrack_nPixSharedHits);
      m_tree->Branch("jet_GhostTrack_nPixelSplitHits",  &m_jet_GhostTrack_nPixSplitHits);
      m_tree->Branch("jet_GhostTrack_nInnermostPixelLayerHits",       &m_jet_GhostTrack_nIMLPixHits);
      m_tree->Branch("jet_GhostTrack_nInnermostPixelLayerSharedHits", &m_jet_GhostTrack_nIMLPixSharedHits);
      m_tree->Branch("jet_GhostTrack_nInnermostPixelLayerSplitHits",  &m_jet_GhostTrack_nIMLPixSplitHits);
      m_tree->Branch("jet_GhostTrack_nNextToInnermostPixelLayerHits", &m_jet_GhostTrack_nNIMLPixHits);
      m_tree->Branch("jet_GhostTrack_nNextToInnermostPixelLayerSharedHits", &m_jet_GhostTrack_nNIMLPixSharedHits);
      m_tree->Branch("jet_GhostTrack_nNextToInnermostPixelLayerSplitHits",  &m_jet_GhostTrack_nNIMLPixSplitHits);
    }
  }

  if ( m_jetInfoSwitch->m_constituent ) {
    m_tree->Branch("jet_numConstituents" ,   &m_jet_numConstituents);
  }

  if ( m_jetInfoSwitch->m_constituentAll ) {
    m_tree->Branch("jet_constituentWeights", &m_jet_constitWeights);
    m_tree->Branch("jet_constituent_pt" ,    &m_jet_constit_pt    );
    m_tree->Branch("jet_constituent_eta",    &m_jet_constit_eta   );
    m_tree->Branch("jet_constituent_phi",    &m_jet_constit_phi   );
    m_tree->Branch("jet_constituent_e"  ,    &m_jet_constit_e     );
  }

  if( m_jetInfoSwitch->m_flavTag ) {
    if ( !m_DC14 ) {
      m_tree->Branch("jet_SV0",           &m_jet_sv0);
      m_tree->Branch("jet_SV1",           &m_jet_sv1);
      m_tree->Branch("jet_IP3D",          &m_jet_ip3d);
    }
    m_tree->Branch("jet_SV1IP3D",       &m_jet_sv1ip3d);
    m_tree->Branch("jet_MV1",           &m_jet_mv1);
    m_tree->Branch("jet_MV2c00",        &m_jet_mv2c00);
    m_tree->Branch("jet_MV2c20",        &m_jet_mv2c20);
  }

  if( m_jetInfoSwitch->m_area ) {
    m_tree->Branch("jet_GhostArea",     &m_jet_ghostArea);
    m_tree->Branch("jet_ActiveArea",    &m_jet_activeArea);
    m_tree->Branch("jet_VoronoiArea",   &m_jet_voronoiArea);
    m_tree->Branch("jet_ActiveArea4vec_pt", &m_jet_activeArea_pt);
    m_tree->Branch("jet_ActiveArea4vec_eta", &m_jet_activeArea_eta);
    m_tree->Branch("jet_ActiveArea4vec_phi", &m_jet_activeArea_phi);
    m_tree->Branch("jet_ActiveArea4vec_m",   &m_jet_activeArea_m);
  }

  if ( m_jetInfoSwitch->m_truth ) {
    m_tree->Branch("jet_ConeTruthLabelID",   &m_jet_truthConeLabelID );
    m_tree->Branch("jet_TruthCount",         &m_jet_truthCount     );
//    m_tree->Branch("jet_TruthPt",            &m_jet_truthPt        );
    m_tree->Branch("jet_TruthLabelDeltaR_B", &m_jet_truthDr_B      );
    m_tree->Branch("jet_TruthLabelDeltaR_C", &m_jet_truthDr_C      );
    m_tree->Branch("jet_TruthLabelDeltaR_T", &m_jet_truthDr_T      );
    m_tree->Branch("jet_PartonTruthLabelID", &m_jet_partonTruthID  );
    m_tree->Branch("jet_GhostTruthAssociationFraction", &m_jet_ghostTruthAssFrac);
    m_tree->Branch("jet_truth_E",   &m_jet_truth_E);
    m_tree->Branch("jet_truth_pt",  &m_jet_truth_pt);
    m_tree->Branch("jet_truth_phi", &m_jet_truth_phi);
    m_tree->Branch("jet_truth_eta", &m_jet_truth_eta);
  }

  if ( m_jetInfoSwitch->m_truthDetails ) {
    m_tree->Branch("jet_GhostBHadronsFinalCount",   &m_jet_truthCount_BhadFinal );
    m_tree->Branch("jet_GhostBHadronsInitialCount", &m_jet_truthCount_BhadInit  );
    m_tree->Branch("jet_GhostBQuarksFinalCount",    &m_jet_truthCount_BQFinal   );
    m_tree->Branch("jet_GhostBHadronsFinalPt",      &m_jet_truthPt_BhadFinal    );
    m_tree->Branch("jet_GhostBHadronsInitialPt",    &m_jet_truthPt_BhadInit     );
    m_tree->Branch("jet_GhostBQuarksFinalPt",       &m_jet_truthPt_BQFinal      );

    m_tree->Branch("jet_GhostCHadronsFinalCount",   &m_jet_truthCount_ChadFinal );
    m_tree->Branch("jet_GhostCHadronsInitialCount", &m_jet_truthCount_ChadInit  );
    m_tree->Branch("jet_GhostCQuarksFinalCount",    &m_jet_truthCount_CQFinal   );
    m_tree->Branch("jet_GhostCHadronsFinalPt",      &m_jet_truthPt_ChadFinal    );
    m_tree->Branch("jet_GhostCHadronsInitialPt",    &m_jet_truthPt_ChadInit     );
    m_tree->Branch("jet_GhostCQuarksFinalPt",       &m_jet_truthPt_CQFinal      );

    m_tree->Branch("jet_GhostTausFinalCount",       &m_jet_truthCount_TausFinal );
    m_tree->Branch("jet_GhostTausFinalPt",          &m_jet_truthPt_TausFinal    );

    m_tree->Branch("jet_truth_pdgId", &m_jet_truth_pdgId);
    m_tree->Branch("jet_truth_partonPt", &m_jet_truth_partonPt);
    m_tree->Branch("jet_truth_partonDR", &m_jet_truth_partonDR);
  }

  this->AddJetsUser();
}

void HelpTreeBase::FillJets( const xAOD::JetContainer* jets, int pvLocation ) {

  this->ClearJets();
  this->ClearJetsUser();

  const xAOD::VertexContainer* vertices(nullptr);
  const xAOD::Vertex *pv = 0;
  //int pvLocation(-1);
  if( m_jetInfoSwitch->m_trackPV || m_jetInfoSwitch->m_allTrack ) {
    HelperFunctions::retrieve( vertices, "PrimaryVertices", m_event, 0 );
    pvLocation = HelperFunctions::getPrimaryVertexLocation( vertices );
    pv = vertices->at( pvLocation );
  }

  for( auto jet_itr : *jets ) {

    if( m_jetInfoSwitch->m_kinematic ){
      m_jet_pt.push_back ( jet_itr->pt() / m_units );
      m_jet_eta.push_back( jet_itr->eta() );
      m_jet_phi.push_back( jet_itr->phi() );
      m_jet_E.push_back  ( jet_itr->e() / m_units );
    }

    if( m_jetInfoSwitch->m_rapidity ){
      m_jet_rapidity.push_back( jet_itr->rapidity() );
    }

    if (m_jetInfoSwitch->m_clean) {

      static SG::AuxElement::ConstAccessor<float> jetTime ("Timing");
      if ( jetTime.isAvailable( *jet_itr ) ) {
        m_jet_time.push_back( jetTime( *jet_itr ) );
      } else { m_jet_time.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> LArQuality ("LArQuality");
      if ( LArQuality.isAvailable( *jet_itr ) ) {
        m_jet_LArQuality.push_back( LArQuality( *jet_itr ) );
      } else { m_jet_LArQuality.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> hecq ("HECQuality");
      if ( hecq.isAvailable( *jet_itr ) ) {
        m_jet_hecq.push_back( hecq( *jet_itr ) );
      } else { m_jet_hecq.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> negE ("NegativeE");
      if ( negE.isAvailable( *jet_itr ) ) {
        m_jet_negE.push_back( negE( *jet_itr ) / m_units );
      } else { m_jet_negE.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> avLArQF ("AverageLArQF");
      if ( avLArQF.isAvailable( *jet_itr ) ) {
        m_jet_avLArQF.push_back( avLArQF( *jet_itr ) );
      } else { m_jet_avLArQF.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> bchCorrCell ("BchCorrCell");
      if ( bchCorrCell.isAvailable( *jet_itr ) ) {
        m_jet_bchCorrCell.push_back( bchCorrCell( *jet_itr ) );
      } else { m_jet_bchCorrCell.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> N90Const ("N90Constituents");
      if ( N90Const.isAvailable( *jet_itr ) ) {
        m_jet_N90Const.push_back( N90Const( *jet_itr ) );
      } else { m_jet_N90Const.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> LArBadHVEFrac ("LArBadHVEnergyFrac");
      if ( LArBadHVEFrac.isAvailable( *jet_itr ) ) {
        m_jet_LArBadHVEFrac.push_back( LArBadHVEFrac( *jet_itr ) );
      } else { m_jet_LArBadHVEFrac.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> LArBadHVNCell ("LArBadHVNCell");
      if ( LArBadHVNCell.isAvailable( *jet_itr ) ) {
        m_jet_LArBadHVNCell.push_back( LArBadHVNCell( *jet_itr ) );
      } else { m_jet_LArBadHVNCell.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> OotFracClus5 ("OotFracClusters5");
      if ( OotFracClus5.isAvailable( *jet_itr ) ) {
        m_jet_OotFracClus5.push_back( OotFracClus5( *jet_itr ) );
      } else { m_jet_OotFracClus5.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> OotFracClus10 ("OotFracClusters10");
      if ( OotFracClus10.isAvailable( *jet_itr ) ) {
        m_jet_OotFracClus10.push_back( OotFracClus10( *jet_itr ) );
      } else { m_jet_OotFracClus10.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> leadClusPt ("LeadingClusterPt");
      if ( leadClusPt.isAvailable( *jet_itr ) ) {
        m_jet_LeadingClusterPt.push_back( leadClusPt( *jet_itr ) );
      } else { m_jet_LeadingClusterPt.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> leadClusSecondLambda ("LeadingClusterSecondLambda");
      if ( leadClusSecondLambda.isAvailable( *jet_itr ) ) {
        m_jet_LeadingClusterSecondLambda.push_back( leadClusSecondLambda( *jet_itr ) );
      } else { m_jet_LeadingClusterSecondLambda.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> leadClusCenterLambda ("LeadingClusterCenterLambda");
      if ( leadClusCenterLambda.isAvailable( *jet_itr ) ) {
        m_jet_LeadingClusterCenterLambda.push_back( leadClusCenterLambda( *jet_itr ) );
      } else { m_jet_LeadingClusterCenterLambda.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> leadClusSecondR ("LeadingClusterSecondR");
      if ( leadClusSecondR.isAvailable( *jet_itr ) ) {
        m_jet_LeadingClusterSecondR.push_back( leadClusSecondR( *jet_itr ) );
      } else { m_jet_LeadingClusterSecondR.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<char> clean_passLooseBad ("clean_passLooseBad");
      if ( clean_passLooseBad.isAvailable( *jet_itr ) ) {
        m_jet_clean_passLooseBad.push_back( clean_passLooseBad( *jet_itr ) );
      } else { m_jet_clean_passLooseBad.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<char> clean_passLooseBadUgly ("clean_passLooseBadUgly");
      if ( clean_passLooseBadUgly.isAvailable( *jet_itr ) ) {
        m_jet_clean_passLooseBadUgly.push_back( clean_passLooseBadUgly( *jet_itr ) );
      } else { m_jet_clean_passLooseBadUgly.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<char> clean_passTightBad ("clean_passTightBad");
      if ( clean_passTightBad.isAvailable( *jet_itr ) ) {
        m_jet_clean_passTightBad.push_back( clean_passTightBad( *jet_itr ) );
      } else { m_jet_clean_passTightBad.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<char> clean_passTightBadUgly ("clean_passTightBadUgly");
      if ( clean_passTightBadUgly.isAvailable( *jet_itr ) ) {
        m_jet_clean_passTightBadUgly.push_back( clean_passTightBadUgly( *jet_itr ) );
      } else { m_jet_clean_passTightBadUgly.push_back( -999 ); }

    } // clean

    if ( m_jetInfoSwitch->m_energy ) {

      static SG::AuxElement::ConstAccessor<float> HECf ("HECFrac");
      if ( HECf.isAvailable( *jet_itr ) ) {
        m_jet_HECf.push_back( HECf( *jet_itr ) );
      } else { m_jet_HECf.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> EMf ("EMFrac");
      if ( EMf.isAvailable( *jet_itr ) ) {
        m_jet_EMf.push_back( EMf( *jet_itr ) );
      } else { m_jet_EMf.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> centroidR ("CentroidR");
      if ( centroidR.isAvailable( *jet_itr ) ) {
        m_jet_centroidR.push_back( centroidR( *jet_itr ) );
      } else { m_jet_centroidR.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> fracSampMax ("FracSamplingMax");
      if ( fracSampMax.isAvailable( *jet_itr ) ) {
        m_jet_fracSampMax.push_back( fracSampMax( *jet_itr ) );
      } else { m_jet_fracSampMax.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> fracSampMaxIdx ("FracSamplingMaxIndex");
      if ( fracSampMaxIdx.isAvailable( *jet_itr ) ) {
        m_jet_fracSampMaxIdx.push_back( fracSampMaxIdx( *jet_itr ) );
      } else { m_jet_fracSampMaxIdx.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> lowEtFrac ("LowEtConstituentsFrac");
      if ( lowEtFrac.isAvailable( *jet_itr ) ) {
        m_jet_lowEtFrac.push_back( lowEtFrac( *jet_itr ) );
      } else { m_jet_lowEtFrac.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> muonSegCount ("GhostMuonSegmentCount");
      if ( muonSegCount.isAvailable( *jet_itr ) ) {
        m_jet_muonSegCount.push_back( muonSegCount( *jet_itr ) );
      } else { m_jet_muonSegCount.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> width ("Width");
      if ( width.isAvailable( *jet_itr ) ) {
        m_jet_width.push_back( width( *jet_itr ) );
      } else { m_jet_width.push_back( -999 ); }

    } // energy

    // each step of the calibration sequence
    if ( m_jetInfoSwitch->m_scales ) {
      m_jet_emPt.push_back( jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetEMScaleMomentum" ).Pt() / m_units );
      m_jet_pileupPt.push_back( jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetPileupScaleMomentum" ).Pt() / m_units );
      m_jet_originConstitPt.push_back( jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetOriginConstitScaleMomentum" ).Pt() / m_units );
      m_jet_etaJESPt.push_back( jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetEtaJESScaleMomentum" ).Pt() / m_units );
      m_jet_gscPt.push_back( jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetGSCScaleMomentum" ).Pt() / m_units );
      // only available in data
      xAOD::JetFourMom_t insitu;
      bool status = jet_itr->getAttribute<xAOD::JetFourMom_t>( "JetInsituScaleMomentum", insitu );
      if(status) {
        m_jet_insituPt.push_back( insitu.Pt() / m_units );
      } else { m_jet_insituPt.push_back( -999 ); }
    }

    if ( m_jetInfoSwitch->m_layer ) {
      static SG::AuxElement::ConstAccessor< std::vector<float> > ePerSamp ("EnergyPerSampling");
      if ( ePerSamp.isAvailable( *jet_itr ) ) {
        m_jet_ePerSamp.push_back( ePerSamp( *jet_itr ) );
        m_jet_ePerSamp.back();
        std::transform((m_jet_ePerSamp.back()).begin(),
            (m_jet_ePerSamp.back()).end(),
            (m_jet_ePerSamp.back()).begin(),
            std::bind2nd(std::divides<float>(), m_units));
      } else {
        // could push back a vector of 24...
        // ... waste of space vs prevention of out of range down stream
        std::vector<float> junk(1,-999);
        m_jet_ePerSamp.push_back( junk );
      }
    }

    if ( m_jetInfoSwitch->m_trackAll || m_jetInfoSwitch->m_trackPV ) {


      // several moments calculated from all verticies
      // one accessor for each and just use appropiately in the following
      static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk1000("NumTrkPt1000");
      static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt1000("SumPtTrkPt1000");
      static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth1000("TrackWidthPt1000");
      static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk500 ("NumTrkPt500");
      static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt500 ("SumPtTrkPt500");
      static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth500 ("TrackWidthPt500");
      static SG::AuxElement::ConstAccessor< std::vector<float> > jvf("JVF");

      if ( m_jetInfoSwitch->m_trackAll ) {

        std::vector<int> junkInt(1,-999);
        std::vector<float> junkFlt(1,-999);

        if ( nTrk1000.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt1000.push_back( nTrk1000( *jet_itr ) );
        } else { m_jet_NTrkPt1000.push_back( junkInt ); }

        if ( sumPt1000.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt1000.push_back( sumPt1000( *jet_itr ) );
          std::transform((m_jet_SumPtPt1000.back()).begin(),
              (m_jet_SumPtPt1000.back()).end(),
              (m_jet_SumPtPt1000.back()).begin(),
              std::bind2nd(std::divides<float>(), m_units));
        } else { m_jet_SumPtPt1000.push_back( junkFlt ); }

        if ( trkWidth1000.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt1000.push_back( trkWidth1000( *jet_itr ) );
        } else { m_jet_TrkWPt1000.push_back( junkFlt ); }

        if ( nTrk500.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt500.push_back( nTrk500( *jet_itr ) );
        } else { m_jet_NTrkPt500.push_back( junkInt ); }

        if ( sumPt500.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt500.push_back( sumPt500( *jet_itr ) );
          std::transform((m_jet_SumPtPt500.back()).begin(),
              (m_jet_SumPtPt500.back()).end(),
              (m_jet_SumPtPt500.back()).begin(),
              std::bind2nd(std::divides<float>(), m_units));
        } else { m_jet_SumPtPt500.push_back( junkFlt ); }

        if ( trkWidth500.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt500.push_back( trkWidth500( *jet_itr ) );
        } else { m_jet_TrkWPt500.push_back( junkFlt ); }

        if ( jvf.isAvailable( *jet_itr ) ) {
          m_jet_jvf.push_back( jvf( *jet_itr ) );
        } else { m_jet_jvf.push_back( junkFlt ); }

      } // trackAll

      if ( m_jetInfoSwitch->m_trackPV && pvLocation >= 0 ) {

        if ( nTrk1000.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt1000PV.push_back( nTrk1000( *jet_itr )[pvLocation] );
        } else { m_jet_NTrkPt1000PV.push_back( -999 ); }

        if ( sumPt1000.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt1000PV.push_back( sumPt1000( *jet_itr )[pvLocation] / m_units );
        } else { m_jet_SumPtPt1000PV.push_back( -999 ); }

        if ( trkWidth1000.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt1000PV.push_back( trkWidth1000( *jet_itr )[pvLocation] );
        } else { m_jet_TrkWPt1000PV.push_back( -999 ); }

        if ( nTrk500.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt500PV.push_back( nTrk500( *jet_itr )[pvLocation] );
        } else { m_jet_NTrkPt500PV.push_back( -999 ); }

        if ( sumPt500.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt500PV.push_back( sumPt500( *jet_itr )[pvLocation] / m_units );
        } else { m_jet_SumPtPt500PV.push_back( -999 ); }

        if ( trkWidth500.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt500PV.push_back( trkWidth500( *jet_itr )[pvLocation] );
        } else { m_jet_TrkWPt500PV.push_back( -999 ); }

        if ( jvf.isAvailable( *jet_itr ) ) {
          m_jet_jvfPV.push_back( jvf( *jet_itr )[pvLocation] );
        } else { m_jet_jvfPV.push_back( -999 ); }

      } // trackPV

      static SG::AuxElement::ConstAccessor< float > jvt ("Jvt");
      if ( jvt.isAvailable( *jet_itr ) ) {
        m_jet_Jvt.push_back( jvt( *jet_itr ) );
      } else { m_jet_Jvt.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor< float > jvtJvfcorr ("JvtJvfcorr");
      if ( jvtJvfcorr.isAvailable( *jet_itr ) ) {
        m_jet_JvtJvfcorr.push_back( jvtJvfcorr( *jet_itr ) );
      } else { m_jet_JvtJvfcorr.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor< float > jvtRpt ("JvtRpt");
      if ( jvtRpt.isAvailable( *jet_itr ) ) {
        m_jet_JvtRpt.push_back( jvtRpt( *jet_itr ) );
      } else { m_jet_JvtRpt.push_back( -999 ); }

//      static SG::AuxElement::ConstAccessor<float> ghostTrackAssFrac("GhostTrackAssociationFraction");
//      if ( ghostTrackAssFrac.isAvailable( *jet_itr) ) {
//        m_jet_ghostTrackAssFrac.push_back( ghostTrackAssFrac( *jet_itr) );
//      } else { m_jet_ghostTrackAssFrac.push_back( -999 ) ; }

    }

    if ( m_jetInfoSwitch->m_allTrack ) {
      static SG::AuxElement::ConstAccessor< int > ghostTrackCount("GhostTrackCount");
      if ( ghostTrackCount.isAvailable( *jet_itr ) ) {
        m_jet_GhostTrackCount.push_back( ghostTrackCount( *jet_itr ) );
      } else { m_jet_GhostTrackCount.push_back( -999 ); }
      static SG::AuxElement::ConstAccessor< float > ghostTrackPt ("GhostTrackPt");
      if ( ghostTrackPt.isAvailable( *jet_itr ) ) {
        m_jet_GhostTrackPt.push_back( ghostTrackPt( *jet_itr ) / m_units );
      } else { m_jet_GhostTrackPt.push_back( -999 ); }
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
          if( m_jetInfoSwitch->m_allTrackPVSel ) {
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
          if( m_jetInfoSwitch->m_allTrackDetail ) {
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
      m_jet_GhostTrack_pt. push_back( pt  );
      m_jet_GhostTrack_qOverP. push_back( qOverP );
      m_jet_GhostTrack_eta.push_back( eta );
      m_jet_GhostTrack_phi.push_back( phi );
      m_jet_GhostTrack_e.  push_back( e   );
      m_jet_GhostTrack_d0. push_back( d0  );
      m_jet_GhostTrack_z0. push_back( z0  );
      if( m_jetInfoSwitch->m_allTrackDetail ) {
        m_jet_GhostTrack_nPixHits.push_back( nPixHits );
        m_jet_GhostTrack_nSCTHits.push_back( nSCTHits );
        m_jet_GhostTrack_nTRTHits.push_back( nTRTHits );
        m_jet_GhostTrack_nPixSharedHits.push_back( nPixSharedHits );
        m_jet_GhostTrack_nPixSplitHits.push_back( nPixSplitHits );
        m_jet_GhostTrack_nIMLPixHits.push_back( nIMLPixHits );
        m_jet_GhostTrack_nIMLPixSharedHits.push_back( nIMLPixSharedHits );
        m_jet_GhostTrack_nIMLPixSplitHits.push_back( nIMLPixSplitHits );
        m_jet_GhostTrack_nNIMLPixHits.push_back( nNIMLPixHits );
        m_jet_GhostTrack_nNIMLPixSharedHits.push_back( nNIMLPixSharedHits );
        m_jet_GhostTrack_nNIMLPixSplitHits.push_back( nNIMLPixSplitHits );
      }
    } // allTrack switch

    if( m_jetInfoSwitch->m_constituent ) {
      m_jet_numConstituents.push_back( jet_itr->numConstituents() );
    }

    if( m_jetInfoSwitch->m_constituentAll ) {
      m_jet_constitWeights.push_back( jet_itr->getAttribute< std::vector<float> >( "constituentWeights" ) );
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
      m_jet_constit_pt. push_back( pt  );
      m_jet_constit_eta.push_back( eta );
      m_jet_constit_phi.push_back( phi );
      m_jet_constit_e.  push_back( e   );
    }

    if ( m_jetInfoSwitch->m_flavTag) {
      const xAOD::BTagging * myBTag = jet_itr->btagging();
      if ( !m_DC14 ) {

        static SG::AuxElement::ConstAccessor<double> SV0_significance3DAcc ("SV0_significance3D");
        if ( SV0_significance3DAcc.isAvailable(*myBTag) ) { m_jet_sv0.push_back(  myBTag -> SV0_significance3D() ); }

	m_jet_sv1.push_back(     myBTag -> SV1_loglikelihoodratio()   );
        m_jet_ip3d.push_back(    myBTag -> IP3D_loglikelihoodratio()  );

      }
      m_jet_sv1ip3d.push_back( myBTag -> SV1plusIP3D_discriminant() );
      m_jet_mv1.push_back(     myBTag -> MV1_discriminant()         );

      //MV2c00 MV2c20 MV2c10 MV2c100 MV2m
      double val(-999);
      myBTag->variable<double>("MV2c00", "discriminant", val);
      m_jet_mv2c00.push_back( val );
      myBTag->variable<double>("MV2c20", "discriminant", val);
      m_jet_mv2c20.push_back( val );

    }

    if ( m_jetInfoSwitch->m_area ) {

      static SG::AuxElement::ConstAccessor<float> ghostArea("JetGhostArea");
      if ( ghostArea.isAvailable( *jet_itr) ) {
        m_jet_ghostArea.push_back( ghostArea( *jet_itr) );
      } else { m_jet_ghostArea.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> activeArea("ActiveArea");
      if ( activeArea.isAvailable( *jet_itr) ) {
        m_jet_activeArea.push_back( activeArea( *jet_itr) );
      } else { m_jet_activeArea.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> voronoiArea("VoronoiArea");
      if ( voronoiArea.isAvailable( *jet_itr) ) {
        m_jet_voronoiArea.push_back( voronoiArea( *jet_itr) );
      } else { m_jet_voronoiArea.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> activeArea_pt("ActiveArea4vec_pt");
      if ( activeArea_pt.isAvailable( *jet_itr) ) {
        m_jet_activeArea_pt.push_back( activeArea_pt( *jet_itr) );
      } else { m_jet_activeArea_pt.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> activeArea_eta("ActiveArea4vec_eta");
      if ( activeArea_eta.isAvailable( *jet_itr) ) {
        m_jet_activeArea_eta.push_back( activeArea_eta( *jet_itr) );
      } else { m_jet_activeArea_eta.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> activeArea_phi("ActiveArea4vec_phi");
      if ( activeArea_phi.isAvailable( *jet_itr) ) {
        m_jet_activeArea_phi.push_back( activeArea_phi( *jet_itr) );
      } else { m_jet_activeArea_phi.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> activeArea_m("ActiveArea4vec_m");
      if ( activeArea_m.isAvailable( *jet_itr) ) {
        m_jet_activeArea_m.push_back( activeArea_m( *jet_itr) );
      } else { m_jet_activeArea_m.push_back( -999 ); }

    }

    if ( m_jetInfoSwitch->m_truth ) {

      static SG::AuxElement::ConstAccessor<int> ConeTruthLabelID ("ConeTruthLabelID");
      if ( ConeTruthLabelID.isAvailable( *jet_itr) ) {
        m_jet_truthConeLabelID.push_back( ConeTruthLabelID( *jet_itr) );
      } else { m_jet_truthConeLabelID.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> TruthCount ("TruthCount");
      if ( TruthCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount.push_back( TruthCount( *jet_itr) );
      } else { m_jet_truthCount.push_back( -999 ); }

//    seems to be empty
//      static SG::AuxElement::ConstAccessor<float> TruthPt ("TruthPt");
//      if ( TruthPt.isAvailable( *jet_itr) ) {
//        m_jet_truthPt.push_back( TruthPt( *jet_itr)/1000 );
//      } else { m_jet_truthPt.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_B ("TruthLabelDeltaR_B");
      if ( TruthLabelDeltaR_B.isAvailable( *jet_itr) ) {
        m_jet_truthDr_B.push_back( TruthLabelDeltaR_B( *jet_itr) );
      } else { m_jet_truthDr_B.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_C ("TruthLabelDeltaR_C");
      if ( TruthLabelDeltaR_C.isAvailable( *jet_itr) ) {
        m_jet_truthDr_C.push_back( TruthLabelDeltaR_C( *jet_itr) );
      } else { m_jet_truthDr_C.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_T ("TruthLabelDeltaR_T");
      if ( TruthLabelDeltaR_T.isAvailable( *jet_itr) ) {
        m_jet_truthDr_T.push_back( TruthLabelDeltaR_T( *jet_itr) );
      } else { m_jet_truthDr_T.push_back( -999 ) ; }

      static SG::AuxElement::ConstAccessor<int> partonLabel("PartonTruthLabelID");
      if ( partonLabel.isAvailable( *jet_itr) ) {
        m_jet_partonTruthID.push_back( partonLabel( *jet_itr) );
      } else { m_jet_partonTruthID.push_back( -999 ) ; }

      static SG::AuxElement::ConstAccessor<float> ghostTruthAssFrac("GhostTruthAssociationFraction");
      if ( ghostTruthAssFrac.isAvailable( *jet_itr) ) {
        m_jet_ghostTruthAssFrac.push_back( ghostTruthAssFrac( *jet_itr) );
      } else { m_jet_ghostTruthAssFrac.push_back( -999 ) ; }

      const xAOD::Jet* truthJet = HelperFunctions::getLink<xAOD::Jet>( jet_itr, "GhostTruthAssociationLink" );
      if(truthJet) {
        m_jet_truth_pt.push_back ( truthJet->pt() / m_units );
        m_jet_truth_eta.push_back( truthJet->eta() );
        m_jet_truth_phi.push_back( truthJet->phi() );
        m_jet_truth_E.push_back  ( truthJet->e() / m_units );
      } else {
        m_jet_truth_pt.push_back ( -999 );
        m_jet_truth_eta.push_back( -999 );
        m_jet_truth_phi.push_back( -999 );
        m_jet_truth_E.push_back  ( -999 );
      }

    }

    if ( m_jetInfoSwitch->m_truthDetails ) {

      //
      // B-Hadron Details
      //
      static SG::AuxElement::ConstAccessor<int> GhostBHadronsFinalCount ("GhostBHadronsFinalCount");
      if ( GhostBHadronsFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_BhadFinal.push_back( GhostBHadronsFinalCount( *jet_itr) );
      } else { m_jet_truthCount_BhadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostBHadronsInitialCount ("GhostBHadronsInitialCount");
      if ( GhostBHadronsInitialCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_BhadInit.push_back( GhostBHadronsInitialCount( *jet_itr) );
      } else { m_jet_truthCount_BhadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostBQuarksFinalCount ("GhostBQuarksFinalCount");
      if ( GhostBQuarksFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_BQFinal.push_back( GhostBQuarksFinalCount( *jet_itr) );
      } else { m_jet_truthCount_BQFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostBHadronsFinalPt ("GhostBHadronsFinalPt");
      if ( GhostBHadronsFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_BhadFinal.push_back( GhostBHadronsFinalPt( *jet_itr) );
      } else { m_jet_truthPt_BhadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostBHadronsInitialPt ("GhostBHadronsInitialPt");
      if ( GhostBHadronsInitialPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_BhadInit.push_back( GhostBHadronsInitialPt( *jet_itr) );
      } else { m_jet_truthPt_BhadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostBQuarksFinalPt ("GhostBQuarksFinalPt");
      if ( GhostBQuarksFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_BQFinal.push_back( GhostBQuarksFinalPt( *jet_itr) );
      } else { m_jet_truthPt_BQFinal.push_back( -999 ); }


      //
      // C-Hadron Details
      //
      static SG::AuxElement::ConstAccessor<int> GhostCHadronsFinalCount ("GhostCHadronsFinalCount");
      if ( GhostCHadronsFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_ChadFinal.push_back( GhostCHadronsFinalCount( *jet_itr) );
      } else { m_jet_truthCount_ChadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostCHadronsInitialCount ("GhostCHadronsInitialCount");
      if ( GhostCHadronsInitialCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_ChadInit.push_back( GhostCHadronsInitialCount( *jet_itr) );
      } else { m_jet_truthCount_ChadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostCQuarksFinalCount ("GhostCQuarksFinalCount");
      if ( GhostCQuarksFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_CQFinal.push_back( GhostCQuarksFinalCount( *jet_itr) );
      } else { m_jet_truthCount_CQFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostCHadronsFinalPt ("GhostCHadronsFinalPt");
      if ( GhostCHadronsFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_ChadFinal.push_back( GhostCHadronsFinalPt( *jet_itr) );
      } else { m_jet_truthPt_ChadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostCHadronsInitialPt ("GhostCHadronsInitialPt");
      if ( GhostCHadronsInitialPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_ChadInit.push_back( GhostCHadronsInitialPt( *jet_itr) );
      } else { m_jet_truthPt_ChadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostCQuarksFinalPt ("GhostCQuarksFinalPt");
      if ( GhostCQuarksFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_CQFinal.push_back( GhostCQuarksFinalPt( *jet_itr) );
      } else { m_jet_truthPt_CQFinal.push_back( -999 ); }


      //
      // Tau Details
      //
      static SG::AuxElement::ConstAccessor<int> GhostTausFinalCount ("GhostTausFinalCount");
      if ( GhostTausFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_TausFinal.push_back( GhostTausFinalCount( *jet_itr) );
      } else { m_jet_truthCount_TausFinal.push_back( -999 ); }


      /* THE ONLY UN-OFFICIAL PIECE OF CODE HERE USE WITH CAUTION */
      static SG::AuxElement::ConstAccessor<float> GhostTausFinalPt ("GhostTausFinalPt");
      if ( GhostTausFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_TausFinal.push_back( GhostTausFinalPt( *jet_itr) );
      } else { m_jet_truthPt_TausFinal.push_back( -999 ); }


      // light quark(1,2,3) , gluon (21 or 9), charm(4) and b(5)
      // GhostPartons should select for these pdgIds only
//    static SG::AuxElement::ConstAccessor< std::vector<const xAOD::TruthParticle*> > ghostPartons("GhostPartons");
//    if( ghostPartons.isAvailable( *jet_itr )) {
//    std::vector<const xAOD::TruthParticle*> truthPartons = ghostPartons( *jet_itr );
      
      std::vector<const xAOD::TruthParticle*> truthPartons = jet_itr->getAssociatedObjects<xAOD::TruthParticle>("GhostPartons");

      if( truthPartons.size() == 0){
        m_jet_truth_pdgId.push_back(-999);
      } else {
        int iParent = 0;
        for(unsigned int i=1; i < truthPartons.size(); ++i){
          if( (truthPartons.at(i)->pt() > 0.001) && (truthPartons.at(i)->e() > truthPartons.at(iParent)->e()) )
            iParent = i;
        }
        m_jet_truth_pdgId.push_back(truthPartons.at(iParent)->pdgId());
        m_jet_truth_partonPt.push_back(truthPartons.at(iParent)->pt() / m_units);
        m_jet_truth_partonDR.push_back(truthPartons.at(iParent)->p4().DeltaR( jet_itr->p4() ));
      }

//    }

    }

    this->FillJetsUser(jet_itr);

    m_njet++;

  } // loop over jets
}

void HelpTreeBase::ClearJets() {

  m_njet = 0;
  if( m_jetInfoSwitch->m_kinematic ){
    m_jet_pt.clear();
    m_jet_eta.clear();
    m_jet_phi.clear();
    m_jet_E.clear();
  }

  // rapidity
  if( m_jetInfoSwitch->m_rapidity ) {
    m_jet_rapidity.clear();
  }

  // clean
  if( m_jetInfoSwitch->m_clean ) {
    m_jet_time.clear();
    m_jet_LArQuality.clear();
    m_jet_hecq.clear();
    m_jet_negE.clear();
    m_jet_avLArQF.clear();
    m_jet_bchCorrCell.clear();
    m_jet_N90Const.clear();
    m_jet_LArBadHVEFrac.clear();
    m_jet_LArBadHVNCell.clear();
    m_jet_OotFracClus5.clear();
    m_jet_OotFracClus10.clear();
    m_jet_LeadingClusterPt.clear();
    m_jet_LeadingClusterSecondLambda.clear();
    m_jet_LeadingClusterCenterLambda.clear();
    m_jet_LeadingClusterSecondR.clear();
    m_jet_clean_passLooseBad.clear();
    m_jet_clean_passLooseBadUgly.clear();
    m_jet_clean_passTightBad.clear();
    m_jet_clean_passTightBadUgly.clear();
  }

  // energy
  if ( m_jetInfoSwitch->m_energy ) {
    m_jet_HECf.clear();
    m_jet_EMf.clear();
    m_jet_centroidR.clear();
    m_jet_fracSampMax.clear();
    m_jet_fracSampMaxIdx.clear();
    m_jet_lowEtFrac.clear();
    m_jet_muonSegCount.clear();
    m_jet_width.clear();
  }

  // each step of the calibration sequence
  if ( m_jetInfoSwitch->m_scales ) {
    m_jet_emPt.clear();
    m_jet_pileupPt.clear();
    m_jet_originConstitPt.clear();
    m_jet_etaJESPt.clear();
    m_jet_gscPt.clear();
    m_jet_insituPt.clear();
  }

  // layer
  if ( m_jetInfoSwitch->m_layer ) {
    m_jet_ePerSamp.clear();
  }

  // trackAll
  if ( m_jetInfoSwitch->m_trackAll ) {
    m_jet_NTrkPt1000.clear();
    m_jet_SumPtPt1000.clear();
    m_jet_TrkWPt1000.clear();
    m_jet_NTrkPt500.clear();
    m_jet_SumPtPt500.clear();
    m_jet_TrkWPt500.clear();
    m_jet_jvf.clear();
    //m_jet_jvfloose.clear();
  }

  // trackPV
  if ( m_jetInfoSwitch->m_trackPV ) {
    m_jet_NTrkPt1000PV.clear();
    m_jet_SumPtPt1000PV.clear();
    m_jet_TrkWPt1000PV.clear();
    m_jet_NTrkPt500PV.clear();
    m_jet_SumPtPt500PV.clear();
    m_jet_TrkWPt500PV.clear();
    m_jet_jvfPV.clear();
  }

  if ( m_jetInfoSwitch->m_trackAll || m_jetInfoSwitch->m_trackPV ) {
    m_jet_Jvt.clear();
    m_jet_JvtJvfcorr.clear();
    m_jet_JvtRpt.clear();
    //m_jet_ghostTrackAssFrac.clear();
  }


  if ( m_jetInfoSwitch->m_allTrack ) {
    m_jet_GhostTrackCount.clear();
    m_jet_GhostTrackPt.clear();
    m_jet_GhostTrack_pt.clear();
    m_jet_GhostTrack_qOverP.clear();
    m_jet_GhostTrack_eta.clear();
    m_jet_GhostTrack_phi.clear();
    m_jet_GhostTrack_e.clear();
    m_jet_GhostTrack_d0.clear();
    m_jet_GhostTrack_z0.clear();
    if ( m_jetInfoSwitch->m_allTrackDetail ) {
      m_jet_GhostTrack_nPixHits.clear();
      m_jet_GhostTrack_nSCTHits.clear();
      m_jet_GhostTrack_nTRTHits.clear();
      m_jet_GhostTrack_nPixSharedHits.clear();
      m_jet_GhostTrack_nPixSplitHits.clear();
      m_jet_GhostTrack_nIMLPixHits.clear();
      m_jet_GhostTrack_nIMLPixSharedHits.clear();
      m_jet_GhostTrack_nIMLPixSplitHits.clear();
      m_jet_GhostTrack_nNIMLPixHits.clear();
      m_jet_GhostTrack_nNIMLPixSharedHits.clear();
      m_jet_GhostTrack_nNIMLPixSplitHits.clear();
    }
  }

  if( m_jetInfoSwitch->m_constituent ) {
    m_jet_numConstituents.clear();
  }

  if( m_jetInfoSwitch->m_constituentAll ) {
    m_jet_constitWeights.clear();
    m_jet_constit_pt.clear();
    m_jet_constit_eta.clear();
    m_jet_constit_phi.clear();
    m_jet_constit_e.clear();
  }

  // flavor tag
  if ( m_jetInfoSwitch->m_flavTag ) {
    m_jet_sv0.clear();
    m_jet_sv1.clear();
    m_jet_ip3d.clear();
    m_jet_sv1ip3d.clear();
    m_jet_mv1.clear();
    m_jet_mv2c00.clear();
    m_jet_mv2c20.clear();
  }

  if ( m_jetInfoSwitch->m_area ) {
    m_jet_ghostArea.clear();
    m_jet_activeArea.clear();
    m_jet_voronoiArea.clear();
    m_jet_activeArea_pt.clear();
    m_jet_activeArea_eta.clear();
    m_jet_activeArea_phi.clear();
    m_jet_activeArea_m.clear();
  }

  // truth
  if ( m_jetInfoSwitch->m_truth ) {
    m_jet_truthConeLabelID.clear();
    m_jet_truthCount.clear();
    m_jet_truthPt.clear();
    m_jet_truthDr_B.clear();
    m_jet_truthDr_C.clear();
    m_jet_truthDr_T.clear();
    m_jet_partonTruthID.clear();
    m_jet_ghostTruthAssFrac.clear();
    m_jet_truth_pt.clear();
    m_jet_truth_eta.clear();
    m_jet_truth_phi.clear();
    m_jet_truth_E.clear();
  }

  // truth_detail
  if ( m_jetInfoSwitch->m_truthDetails ) {
    m_jet_truthCount_BhadFinal.clear();
    m_jet_truthCount_BhadInit.clear();
    m_jet_truthCount_BQFinal.clear();
    m_jet_truthPt_BhadFinal.clear();
    m_jet_truthPt_BhadInit.clear();
    m_jet_truthPt_BQFinal.clear();
    m_jet_truthCount_ChadFinal.clear();
    m_jet_truthCount_ChadInit.clear();
    m_jet_truthCount_CQFinal.clear();
    m_jet_truthPt_ChadFinal.clear();
    m_jet_truthPt_ChadInit.clear();
    m_jet_truthPt_CQFinal.clear();
    m_jet_truthCount_TausFinal.clear();
    m_jet_truthPt_TausFinal.clear();
    m_jet_truth_pdgId.clear();
    m_jet_truth_partonPt.clear();
    m_jet_truth_partonDR.clear();
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
  m_runNumber = m_eventNumber = m_mcEventNumber = m_mcChannelNumber = -999;
  m_mcEventWeight = 1.;
  m_weight_pileup = 1.;
  // pileup
  m_npv = m_lumiBlock = -999;
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

  m_tree->Branch("metFinal",	     &m_metFinal,	  "metFinal/F");
  m_tree->Branch("metFinalPx",       &m_metFinalPx,	  "metFinalPx/F");
  m_tree->Branch("metFinalPy",       &m_metFinalPy,	  "metFinalPy/F");
  m_tree->Branch("metFinalSumEt",    &m_metFinalSumEt,    "metFinalSumEt/F");
  m_tree->Branch("metFinalPhi",      &m_metFinalPhi,	  "metFinalPhi/F");  

  if ( m_metInfoSwitch->m_refEle ) {
    m_tree->Branch("metEle",         &m_metEle,            "metEle/F"     );
    m_tree->Branch("metElePhi",      &m_metElePhi,         "metElePhi/F"  );
  }
  if ( m_metInfoSwitch->m_refGamma ) {
    m_tree->Branch("metGamma",       &m_metGamma,          "metGamma/F"   );
    m_tree->Branch("metGammaPhi",    &m_metGammaPhi,       "metGammaPhi/F");
  }
  if ( m_metInfoSwitch->m_refTau ) {
    m_tree->Branch("metTau",         &m_metTau,            "metTau/F"     );
    m_tree->Branch("metTauPhi",      &m_metTauPhi,         "metTauPhi/F"  );
  }
  if ( m_metInfoSwitch->m_muons ) {
    m_tree->Branch("metMuons",       &m_metMuons,          "metMuons/F"   );
    m_tree->Branch("metMuonsPhi",    &m_metMuonsPhi,       "metMuonsPhi/F");
  }
  if ( m_metInfoSwitch->m_softClus) {
    m_tree->Branch("metSoftCluss",   &m_metSoftCluss,      "metSoftCluss/F"   );
    m_tree->Branch("metSoftClussPhi",&m_metSoftClussPhi,   "metSoftClussPhi/F");
  }

  this->AddMETUser();
}

void HelpTreeBase::FillMET( const xAOD::MissingETContainer* met ) {

  // Clear previous events
  this->ClearMET();
  this->ClearMETUser();

  if ( m_debug ) { Info("HelpTreeBase::FillMET()", "Filling MET info"); }

  const xAOD::MissingET* final = *met->find("FinalClus"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
  m_metFinal	  = final->met() / m_units;
  m_metFinalPx    = final->mpx() / m_units;  
  m_metFinalPy    = final->mpy() / m_units;	
  m_metFinalSumEt = final->sumet() / m_units;	    
  m_metFinalPhi   = final->phi();

  /* add stuff... */

  this->FillMETUser(met);
}

void HelpTreeBase::ClearMET() {

  m_metFinal	  = -999;
  m_metFinalPx    = -999;
  m_metFinalPy    = -999;
  m_metFinalSumEt = -999;
  m_metFinalPhi   = -999;

  if ( m_metInfoSwitch->m_refEle ) {  
    m_metEle       = m_metElePhi       = -999;
  } 
  if ( m_metInfoSwitch->m_refGamma ) {
    m_metGamma     = m_metGammaPhi     = -999;
  }
  if ( m_metInfoSwitch->m_refTau ) {
    m_metTau       = m_metTauPhi       = -999;
  }
  if ( m_metInfoSwitch->m_muons ) {
    m_metMuons     = m_metMuonsPhi     = -999;
  }
  if ( m_metInfoSwitch->m_softClus) {
    m_metSoftCluss = m_metSoftClussPhi = -999;
  }
}



bool HelpTreeBase::writeTo( TFile* file ) {
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if ( status == 0 ) { return false; }
  return true;
}

