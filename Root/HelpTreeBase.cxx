// c++ include(s):
#include <iostream>

// EDM include(s):
#include "xAODBTagging/BTagging.h"
#include "xAODTracking/TrackParticle.h"
#include "xAODTracking/TrackSummaryAccessors_v1.h"
#include "xAODTruth/TruthEventContainer.h"

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelpTreeBase.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "AsgTools/StatusCode.h"

// needed? should it be here?
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

HelpTreeBase::HelpTreeBase(xAOD::TEvent* event, TTree* tree, TFile* file, const float units, bool debug):
  m_eventInfoSwitch(0),
  m_muInfoSwitch(0),
  m_elInfoSwitch(0),
  m_jetInfoSwitch(0),
  m_fatJetInfoSwitch(0),
  m_tauInfoSwitch(0)
{

  m_units = units;
  m_debug = debug;
  m_tree = tree;
  m_tree->SetDirectory( file );
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

  Info("AddEvent()", "Adding event variables: %s", detailStr.c_str());

  m_eventInfoSwitch = new HelperClasses::EventInfoSwitch( detailStr );

  // always
  m_tree->Branch("runNumber",          &m_runNumber,      "runNumber/I");
  m_tree->Branch("eventNumber",        &m_eventNumber,    "eventNumber/I");
  m_tree->Branch("mcEventNumber",      &m_mcEventNumber,  "mcEventNumber/I");
  m_tree->Branch("mcChannelNumber",    &m_mcChannelNumber,"mcChannelNumber/I");
  m_tree->Branch("mcEventWeight",      &m_mcEventWeight,  "mcEventWeight/F");

  if ( m_eventInfoSwitch->m_pileup ) {
    m_tree->Branch("NPV",                &m_npv,            "NPV/I");
    m_tree->Branch("actualInteractionsPerCrossing",  &m_actualMu,  "actualInteractionsPerCrossing/F");
    m_tree->Branch("averageInteractionsPerCrossing", &m_averageMu, "averageInteractionsPerCrossing/F");
    m_tree->Branch("lumiBlock", &m_lumiBlock, "lumiBlock/I");
  }

  if ( m_eventInfoSwitch->m_shapeEM ) {
    m_tree->Branch("rhoEM",                &m_rhoEM,            "rhoEM/F");
  }

  if ( m_eventInfoSwitch->m_shapeLC ) {
    m_tree->Branch("rhoLC",                &m_rhoLC,            "rhoLC/F");
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

  if ( m_eventInfoSwitch->m_pileup ) {
    if ( event ) {
      const xAOD::VertexContainer* vertices(nullptr);
      if(!HelperFunctions::retrieve( vertices, "PrimaryVertices", event, 0 ).isSuccess()){
        Error("HelpTreeBase::FillEvent()", "Could not retrieve primary vertex container.");
        return;
      }
      m_npv = HelperFunctions::countPrimaryVertices(vertices, 2);
      //Info("FillEvent()","Number of primary vertices w/ at least 2 tracks: %i", m_npv);
    } else {
      m_npv = -1;
    }

    m_actualMu  = eventInfo->actualInteractionsPerCrossing();
    m_averageMu = eventInfo->averageInteractionsPerCrossing();
    m_lumiBlock = eventInfo->lumiBlock();

  }

  if ( m_eventInfoSwitch->m_shapeLC && event ) {
    const xAOD::EventShape* evtShape(nullptr);
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoLC ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoLC = -999;
    }
  }

  if ( m_eventInfoSwitch->m_shapeEM && event ) {
    const xAOD::EventShape* evtShape(nullptr);
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoEM ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoEM = -999;
    }
  }

  if( m_eventInfoSwitch->m_truth && event ) {
    //MC Truth
    const xAOD::TruthEventContainer* truthE = 0;
    HelperFunctions::retrieve( truthE, "TruthEvents", event, 0 );
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
 *   MUONS
 *
 ********************/

void HelpTreeBase::AddMuons(const std::string detailStr) {

  m_muInfoSwitch = new HelperClasses::MuonInfoSwitch( detailStr );

  // always
  m_tree->Branch("nmuon",   &m_nmuon, "nmuon/I");

  if ( m_muInfoSwitch->m_kinematic ){
    m_tree->Branch("muon_pt",  &m_muon_pt);
    m_tree->Branch("muon_phi", &m_muon_phi);
    m_tree->Branch("muon_eta", &m_muon_eta);
    m_tree->Branch("muon_m",   &m_muon_m);
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
    //m_tree->Branch("muon_trknInnermostPixLayHits",  &m_muon_trknInnermostPixLayHits);
    //m_tree->Branch("muon_trkPixdEdX",  &m_muon_trkPixdEdX);
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
	float d0_significance = fabs( trk->d0() )  / sqrt(trk->definingParametersCovMatrix()(0,0) );
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
        //trk->summaryValue( nInnermostPixLayHits, xAOD::numberOfInnermostPixelLayerHits );
        //trk->summaryValue( pixdEdX,   xAOD::pixeldEdx);
      }
      m_muon_trknSiHits.push_back( nPixHits + nSCTHits );
      m_muon_trknPixHits.push_back( nPixHits );
      m_muon_trknPixHoles.push_back( nPixHoles );
      m_muon_trknSCTHits.push_back( nSCTHits );
      m_muon_trknSCTHoles.push_back( nSCTHoles );
      m_muon_trknTRTHits.push_back( nTRTHits );
      m_muon_trknTRTHoles.push_back( nTRTHoles );
      m_muon_trknBLayerHits.push_back( nBLayerHits );
      //m_muon_trknInnermostPixLayHits.push_back( nInnermostPixLayHits );
      //m_muon_trkPixdEdX.push_back( pixdEdX );
    }

    this->FillMuonsUser(muon_itr);

    m_nmuon++;
  }
}

/*********************
 *
 *   ELECTRONS
 *
 ********************/

void HelpTreeBase::AddElectrons(const std::string detailStr) {

  m_elInfoSwitch = new HelperClasses::ElectronInfoSwitch( detailStr );

  // always
  m_tree->Branch("nel",    &m_nel,"nel/I");

  if ( m_elInfoSwitch->m_kinematic ) {
    m_tree->Branch("el_pt",  &m_el_pt);
    m_tree->Branch("el_phi", &m_el_phi);
    m_tree->Branch("el_eta", &m_el_eta);
    m_tree->Branch("el_m",   &m_el_m);
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
    //m_tree->Branch("el_trknInnermostPixLayHits",  &m_el_trknInnermostPixLayHits);
    //m_tree->Branch("el_trkPixdEdX",    &m_el_trkPixdEdX);
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
	float d0_significance = fabs( trk->d0() )  / sqrt(trk->definingParametersCovMatrix()(0,0) );
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
        //trk->summaryValue( nInnermostPixLayHits, xAOD::numberOfInnermostPixelLayerHits );
        //trk->summaryValue( pixdEdX,   xAOD::pixeldEdx);
      }
      m_el_trknSiHits.push_back( nPixHits + nSCTHits );
      m_el_trknPixHits.push_back( nPixHits );
      m_el_trknPixHoles.push_back( nPixHoles );
      m_el_trknSCTHits.push_back( nSCTHits );
      m_el_trknSCTHoles.push_back( nSCTHoles );
      m_el_trknTRTHits.push_back( nTRTHits );
      m_el_trknTRTHoles.push_back( nTRTHoles );
      m_el_trknBLayerHits.push_back( nBLayerHits );
      //m_el_trknInnermostPixLayHits.push_back( nInnermostPixLayHits );
      //m_el_trkPixdEdX.push_back( pixdEdX );
    }

    this->FillElectronsUser(el_itr);

    m_nel++;
  }
}

/*********************
 *
 *   JETS
 *
 ********************/

void HelpTreeBase::AddJets(const std::string detailStr)
{

  Info("AddJets()", "Adding jet variables: %s", detailStr.c_str());

  m_jetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );

  // always
  m_tree->Branch("njets",    &m_njet,"njets/I");

  if ( m_jetInfoSwitch->m_kinematic ) {
    m_tree->Branch("jet_E",   &m_jet_E);
    m_tree->Branch("jet_pt",  &m_jet_pt);
    m_tree->Branch("jet_phi", &m_jet_phi);
    m_tree->Branch("jet_eta", &m_jet_eta);
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
    //m_tree->Branch("jet_JVFLoose",          &m_jet_jvfloose     );
    // HigestJVFLooseVtx  Vertex
    // JVT  Jvt, JvtRpt, JvtJvfcorr float JVT, etc., see Twiki
  }

  if ( m_jetInfoSwitch->m_trackPV ) {
    m_tree->Branch("jet_NumTrkPt1000PV",      &m_jet_NTrkPt1000PV   );
    m_tree->Branch("jet_SumPtTrkPt1000PV",    &m_jet_SumPtPt1000PV  );
    m_tree->Branch("jet_TrackWidthPt1000PV",  &m_jet_TrkWPt1000PV   );
    m_tree->Branch("jet_NumTrkPt500PV",	      &m_jet_NTrkPt500PV    );
    m_tree->Branch("jet_SumPtTrkPt500PV",     &m_jet_SumPtPt500PV   );
    m_tree->Branch("jet_TrackWidthPt500PV",   &m_jet_TrkWPt500PV    );
    m_tree->Branch("jet_JVFPV",		            &m_jet_jvfPV	        );
    //m_tree->Branch("jet_JVFLoosePV",          &m_jet_jvfloosePV     );
    // HigestJVFLooseVtx  Vertex
    // JVT  Jvt, JvtRpt, JvtJvfcorr float JVT, etc., see Twiki
  }

  if ( m_jetInfoSwitch->m_allTrack ) {
    m_tree->Branch("jet_GhostTrackCount",  &m_jet_GhostTrackCount );
    m_tree->Branch("jet_GhostTrackPt",     &m_jet_GhostTrackPt    );
    m_tree->Branch("jet_GhostTrack_pt",    &m_jet_GhostTrack_pt   );
    m_tree->Branch("jet_GhostTrack_eta",   &m_jet_GhostTrack_eta  );
    m_tree->Branch("jet_GhostTrack_phi",   &m_jet_GhostTrack_phi  );
    m_tree->Branch("jet_GhostTrack_e",     &m_jet_GhostTrack_e    );
  }

  if( m_jetInfoSwitch->m_flavTag ) {
    m_tree->Branch("jet_SV0",           &m_jet_sv0);
    m_tree->Branch("jet_SV1",           &m_jet_sv1);
    m_tree->Branch("jet_IP3D",          &m_jet_ip3d);
    m_tree->Branch("jet_SV1IP3D",       &m_jet_sv1ip3d);
    m_tree->Branch("jet_MV1",           &m_jet_mv1);
    m_tree->Branch("jet_MV2c00",        &m_jet_mv2c00);
    m_tree->Branch("jet_MV2c20",        &m_jet_mv2c20);
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

  for( auto jet_itr : *jets ) {

    if( m_jetInfoSwitch->m_kinematic ){
      m_jet_pt.push_back ( jet_itr->pt() / m_units );
      m_jet_eta.push_back( jet_itr->eta() );
      m_jet_phi.push_back( jet_itr->phi() );
      m_jet_E.push_back  ( jet_itr->e() / m_units );
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

      }

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

      }

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
      std::vector<float> eta;
      std::vector<float> phi;
      std::vector<float> e;
      static SG::AuxElement::ConstAccessor< std::vector<ElementLink<DataVector<xAOD::IParticle> > > >ghostTrack ("GhostTrack");
      if ( ghostTrack.isAvailable( *jet_itr ) ) {
        std::vector<ElementLink<DataVector<xAOD::IParticle> > > trackLinks = ghostTrack( *jet_itr );
        //std::vector<float> pt(trackLinks.size(),-999);
        for ( auto link_itr : trackLinks ) {
          if( !link_itr.isValid() ) { continue; }
          const xAOD::IParticle* trk = *link_itr;
          //std::cout << "\t" << trk->pt() << std::endl;
          pt. push_back( trk->pt() / m_units );
          eta.push_back( trk->eta() );
          phi.push_back( trk->phi() );
          e.  push_back( trk->e()  / m_units );
        }
      } // if ghostTrack available
      m_jet_GhostTrack_pt. push_back( pt  );
      m_jet_GhostTrack_eta.push_back( eta );
      m_jet_GhostTrack_phi.push_back( phi );
      m_jet_GhostTrack_e.  push_back( e   );
    } // allTrack switch

    if ( m_jetInfoSwitch->m_flavTag) {
      const xAOD::BTagging * myBTag = jet_itr->btagging();
      m_jet_sv0.push_back(     myBTag -> SV0_significance3D()       );
      m_jet_sv1.push_back(     myBTag -> SV1_loglikelihoodratio()   );
      m_jet_ip3d.push_back(    myBTag -> IP3D_loglikelihoodratio()  );
      m_jet_sv1ip3d.push_back( myBTag -> SV1plusIP3D_discriminant() );
      m_jet_mv1.push_back(     myBTag -> MV1_discriminant()         );

      //MV2c00 MV2c20 MV2c10 MV2c100 MV2m
      double val(-999);
      myBTag->variable<double>("MV2c00", "discriminant", val);
      m_jet_mv2c00.push_back( val );
      myBTag->variable<double>("MV2c20", "discriminant", val);
      m_jet_mv2c20.push_back( val );

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
      static SG::AuxElement::ConstAccessor< std::vector<const xAOD::TruthParticle*> > ghostPartons("GhostPartons");
      if( ghostPartons.isAvailable( *jet_itr )) { 
        std::vector<const xAOD::TruthParticle*> truthPartons = ghostPartons( *jet_itr );
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
      }

    }

    this->FillJetsUser(jet_itr);
    
    m_njet++;
    
  } // loop over jets
}

/*********************
 *
 *   FAT JETS
 *
 ********************/

void HelpTreeBase::AddFatJets(std::string detailStr) {
  m_fatJetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );
}
/* TODO: fatJets */
void HelpTreeBase::FillFatJets( const xAOD::JetContainer* /*fatJets*/ ) { }

void HelpTreeBase::ClearEvent() {
  m_runNumber = m_eventNumber = m_mcEventNumber = m_mcChannelNumber = -999;
  m_mcEventWeight = 1.;
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

void HelpTreeBase::ClearMuons() {

  m_nmuon = 0;

  if ( m_muInfoSwitch->m_kinematic ){
    m_muon_pt.clear();
    m_muon_eta.clear();
    m_muon_phi.clear();
    m_muon_m.clear();
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
    //m_muon_trknInnermostPixLayHits.clear();
    //m_muon_trkPixdEdX.clear();
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
    //m_el_trknInnermostPixLayHits.clear();
    //m_el_trkPixdEdX.clear();
  }

}


void HelpTreeBase::ClearJets() {

  m_njet = 0;
  m_jet_pt.clear();
  m_jet_eta.clear();
  m_jet_phi.clear();
  m_jet_E.clear();

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
    //m_jet_jvfloosePV.clear();
  }

  if ( m_jetInfoSwitch->m_allTrack ) {
    m_jet_GhostTrackCount.clear();
    m_jet_GhostTrackPt.clear();
    m_jet_GhostTrack_pt.clear();
    m_jet_GhostTrack_eta.clear();
    m_jet_GhostTrack_phi.clear();
    m_jet_GhostTrack_e.clear();
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


bool HelpTreeBase::writeTo( TFile* file ) {
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if ( status == 0 ) { return false; }
  return true;
}

