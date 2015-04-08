// c++ include(s):
#include <iostream>

#include "AsgTools/StatusCode.h"

#include "xAODBTagging/BTagging.h"

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelpTreeBase.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// needed? should it be here?
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

/* TODO: event */
HelpTreeBase::HelpTreeBase(xAOD::TEvent* event, TTree* tree, TFile* file, const float units):
  m_eventInfoSwitch(0),
  m_muInfoSwitch(0),
  m_elInfoSwitch(0),
  m_jetInfoSwitch(0),
  m_fatJetInfoSwitch(0)
{

  m_units = units;
  m_tree = tree;
  m_tree->SetDirectory( file );
  Info("HelpTreeBase()", "HelpTreeBase setup");

}

void HelpTreeBase::Fill() {
  m_tree->Fill();
  //this->Clear();
}

/*********************
 *
 *   EVENT
 *
 ********************/

void HelpTreeBase::AddEvent( const std::string detailStr) {

  this->ClearEvent();
  this->ClearEventUser();

  m_eventInfoSwitch = new HelperClasses::EventInfoSwitch( detailStr );

  // always
  m_tree->Branch("runNumber",          &m_runNumber,      "runNumber/I");
  m_tree->Branch("eventNumber",        &m_eventNumber,    "eventNumber/I");
  m_tree->Branch("mcEventNumber",      &m_mcEventNumber,  "mcEventNumber/I");
  m_tree->Branch("mcChannelNumber",    &m_mcChannelNumber,"mcChannelNumber/I");
  m_tree->Branch("mcEventWeight",      &m_mcEventWeight,  "mcEventWeight/F");

  if( m_eventInfoSwitch->m_pileup ) {
    m_tree->Branch("NPV",                &m_npv,            "NPV/I");
    m_tree->Branch("actualInteractionsPerCrossing",  &m_actualMu,  "actualInteractionsPerCrossing/F");
    m_tree->Branch("averageInteractionsPerCrossing", &m_averageMu, "averageInteractionsPerCrossing/F");
    m_tree->Branch("lumiBlock", &m_lumiBlock, "lumiBlock/I");
  } 

  if( m_eventInfoSwitch->m_shapeEM ) {
    m_tree->Branch("rhoEM",                &m_rhoEM,            "rhoEM/F");
  }

  if( m_eventInfoSwitch->m_shapeLC ) {
    m_tree->Branch("rhoLC",                &m_rhoLC,            "rhoLC/F");
  }

  this->AddEventUser();
}

void HelpTreeBase::FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event ) {
  m_runNumber             = eventInfo->runNumber();
  m_eventNumber           = eventInfo->eventNumber();
  if(eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) {
    m_mcEventNumber         = eventInfo->mcEventNumber();
    m_mcChannelNumber       = eventInfo->mcChannelNumber();
    m_mcEventWeight         = eventInfo->mcEventWeight();
  } else {
    m_mcEventNumber         = -1;
    m_mcChannelNumber       = -1;
    m_mcEventWeight	    = 1.;
  }

  if(m_eventInfoSwitch->m_pileup) {
    if( event ) {
      const xAOD::VertexContainer* vertices = 0;
      HelperFunctions::retrieve( vertices, "PrimaryVertices", event, 0 );
      m_npv = HelperFunctions::countPrimaryVertices(vertices, 2);
    } else { 
      m_npv = -1;
    }

    m_actualMu  = eventInfo->actualInteractionsPerCrossing();
    m_averageMu = eventInfo->averageInteractionsPerCrossing();
    m_lumiBlock = eventInfo->lumiBlock();

  }

  if( m_eventInfoSwitch->m_shapeLC && event ) {
    const xAOD::EventShape* evtShape = 0;
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoLC ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoLC = -999;
    }
  }

  if( m_eventInfoSwitch->m_shapeEM && event ) {
    const xAOD::EventShape* evtShape = 0;
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoEM ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoEM = -999;
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
  
  if( m_muInfoSwitch->m_kinematic ){
    m_tree->Branch("nmuon",   &m_nmuon, "nmuon/I");  
    m_tree->Branch("muon_pt",  &m_muon_pt);
    m_tree->Branch("muon_phi", &m_muon_phi);
    m_tree->Branch("muon_eta", &m_muon_eta);
  }
  
  this->AddMuonsUser();
}

void HelpTreeBase::FillMuons( const xAOD::MuonContainer& muons ) {  

  this->ClearMuons();
  this->ClearMuonsUser();

  m_nmuon = 0;
  for( auto muon_itr : muons ) {
    if( m_muInfoSwitch->m_kinematic ){
      m_muon_pt.push_back ( (muon_itr)->pt() / m_units  );
      m_muon_eta.push_back( (muon_itr)->eta() );
      m_muon_phi.push_back( (muon_itr)->phi() );
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

  m_tree->Branch("nel",    &m_nel,"nel/I");  
  
  if( m_elInfoSwitch->m_kinematic ){
    m_tree->Branch("el_pt",  &m_el_pt);
    m_tree->Branch("el_phi", &m_el_phi);
    m_tree->Branch("el_eta", &m_el_eta);
  }
  
  this->AddElectronsUser();
}

void HelpTreeBase::FillElectrons( const xAOD::ElectronContainer& electrons ) { 
  
  this->ClearElectrons();
  this->ClearElectronsUser();
  
  m_nel = 0;
  for( auto el_itr : electrons ) {
    if( m_elInfoSwitch->m_kinematic ){
      m_el_pt.push_back ( (el_itr)->pt() / m_units ); 
      m_el_eta.push_back( (el_itr)->eta() );
      m_el_phi.push_back( (el_itr)->phi() );
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

  m_jetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );

  // always
  m_tree->Branch("njets",    &m_njet,"njets/I");

  if( m_jetInfoSwitch->m_kinematic ){
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
    m_tree->Branch("jet_LArBadHVEnergy",      &m_jet_LArBadHVE          );
    m_tree->Branch("jet_LArBadHVRatio",       &m_jet_LArBadHVRatio  	  );
  }

  if( m_jetInfoSwitch->m_energy ) {
    m_tree->Branch("jet_HECFrac", 	            &m_jet_HECf	    );
    m_tree->Branch("jet_EMFrac",  	            &m_jet_EMf	    );
    m_tree->Branch("jet_CentroidR",	            &m_jet_centroidR      );
    m_tree->Branch("jet_FracSamplingMax",       &m_jet_fracSampMax    );
    m_tree->Branch("jet_FracSamplingMaxIndex",  &m_jet_fracSampMaxIdx );
    m_tree->Branch("jet_LowEtConstituentsFrac", &m_jet_lowEtFrac      );
  }		       

  if( m_jetInfoSwitch->m_layer ) { 
    m_tree->Branch("jet_EnergyPerSampling",     &m_jet_ePerSamp   );
  }

  if( m_jetInfoSwitch->m_trackAll ) { 
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

  if( m_jetInfoSwitch->m_trackPV ) { 
    m_tree->Branch("jet_NumTrkPt1000PV",      &m_jet_NTrkPt1000PV   );
    m_tree->Branch("jet_SumPtTrkPt1000PV",    &m_jet_SumPtPt1000PV  );
    m_tree->Branch("jet_TrackWidthPt1000PV",  &m_jet_TrkWPt1000PV   );
    m_tree->Branch("jet_NumTrkPt500PV",	      &m_jet_NTrkPt500PV    );
    m_tree->Branch("jet_SumPtTrkPt500PV",     &m_jet_SumPtPt500PV   );
    m_tree->Branch("jet_TrackWidthPt500PV",   &m_jet_TrkWPt500PV    );
    m_tree->Branch("jet_JVFPV",		      &m_jet_jvfPV	    );
    //m_tree->Branch("jet_JVFLoosePV",          &m_jet_jvfloosePV     );
    // HigestJVFLooseVtx  Vertex
    // JVT  Jvt, JvtRpt, JvtJvfcorr float JVT, etc., see Twiki
  }

  if( m_jetInfoSwitch->m_flavTag ) { 
    m_tree->Branch("jet_MV1Weight",     &m_jet_mv1);
    m_tree->Branch("jet_SV1IP3DWeight", &m_jet_sv1ip3d);
  }

  if( m_jetInfoSwitch->m_truth ) {
    m_tree->Branch("jet_TruthLabelID",       &m_jet_truthLabelID   );
    m_tree->Branch("jet_TruthCount",         &m_jet_truthCount     );
//    m_tree->Branch("jet_TruthPt",            &m_jet_truthPt        );
    m_tree->Branch("jet_TruthLabelDeltaR_B", &m_jet_truthDr_B      );
    m_tree->Branch("jet_TruthLabelDeltaR_C", &m_jet_truthDr_C      );
    m_tree->Branch("jet_TruthLabelDeltaR_T", &m_jet_truthDr_T      );
    m_tree->Branch("jet_truth_E",   &m_jet_truth_E);
    m_tree->Branch("jet_truth_pt",  &m_jet_truth_pt);
    m_tree->Branch("jet_truth_phi", &m_jet_truth_phi);
    m_tree->Branch("jet_truth_eta", &m_jet_truth_eta);
  }

  if( m_jetInfoSwitch->m_truthDetails ) {
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
  }

  this->AddJetsUser();
}

void HelpTreeBase::FillJets( const xAOD::JetContainer& jets, int pvLocation ) {
  
  this->ClearJets();
  this->ClearJetsUser();
  
  for( auto jet_itr : jets ) {  
  
    if( m_jetInfoSwitch->m_kinematic ){
      m_jet_pt.push_back ( jet_itr->pt() / m_units );
      m_jet_eta.push_back( jet_itr->eta() );
      m_jet_phi.push_back( jet_itr->phi() );
      m_jet_E.push_back  ( jet_itr->e() / m_units );
    }
    
    if(m_jetInfoSwitch->m_clean) {

      static SG::AuxElement::ConstAccessor<float> jetTime ("Timing");
      if( jetTime.isAvailable( *jet_itr ) ) {
        m_jet_time.push_back( jetTime( *jet_itr ) );
      } else { m_jet_time.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> LArQuality ("LArQuality");
      if( LArQuality.isAvailable( *jet_itr ) ) {
        m_jet_LArQuality.push_back( LArQuality( *jet_itr ) );
      } else { m_jet_LArQuality.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> hecq ("HECQuality");
      if( hecq.isAvailable( *jet_itr ) ) {
        m_jet_hecq.push_back( hecq( *jet_itr ) );
      } else { m_jet_hecq.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> negE ("NegativeE");
      if( negE.isAvailable( *jet_itr ) ) {
        m_jet_negE.push_back( negE( *jet_itr ) / m_units );
      } else { m_jet_negE.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> avLArQF ("AverageLArQF");
      if( avLArQF.isAvailable( *jet_itr ) ) {
        m_jet_avLArQF.push_back( avLArQF( *jet_itr ) );
      } else { m_jet_avLArQF.push_back( -999 ); }
    
      static SG::AuxElement::ConstAccessor<float> bchCorrCell ("BchCorrCell");
      if( bchCorrCell.isAvailable( *jet_itr ) ) {
        m_jet_bchCorrCell.push_back( bchCorrCell( *jet_itr ) );
      } else { m_jet_bchCorrCell.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> N90Const ("N90Constituents");
      if( N90Const.isAvailable( *jet_itr ) ) {
        m_jet_N90Const.push_back( N90Const( *jet_itr ) );
      } else { m_jet_N90Const.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> LArBadHVE ("LArBadHVEnergy");
      if( LArBadHVE.isAvailable( *jet_itr ) ) {
        m_jet_LArBadHVE.push_back( LArBadHVE( *jet_itr ) );
      } else { m_jet_LArBadHVE.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> LArBadHVRatio ("LArBadHVRatio");
      if( LArBadHVRatio.isAvailable( *jet_itr ) ) {
        m_jet_LArBadHVRatio.push_back( LArBadHVRatio( *jet_itr ) );
      } else { m_jet_LArBadHVRatio.push_back( -999 ); }

    } // clean

    if( m_jetInfoSwitch->m_energy ) {

      static SG::AuxElement::ConstAccessor<float> HECf ("HECFrac");
      if( HECf.isAvailable( *jet_itr ) ) {
        m_jet_HECf.push_back( HECf( *jet_itr ) );
      } else { m_jet_HECf.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> EMf ("EMFrac");
      if( EMf.isAvailable( *jet_itr ) ) {
        m_jet_EMf.push_back( EMf( *jet_itr ) );
      } else { m_jet_EMf.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> centroidR ("CentroidR");
      if( centroidR.isAvailable( *jet_itr ) ) {
        m_jet_centroidR.push_back( centroidR( *jet_itr ) );
      } else { m_jet_centroidR.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> fracSampMax ("FracSamplingMax");
      if( fracSampMax.isAvailable( *jet_itr ) ) {
        m_jet_fracSampMax.push_back( fracSampMax( *jet_itr ) );
      } else { m_jet_fracSampMax.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> fracSampMaxIdx ("FracSamplingMaxIndex");
      if( fracSampMaxIdx.isAvailable( *jet_itr ) ) {
        m_jet_fracSampMaxIdx.push_back( fracSampMaxIdx( *jet_itr ) );
      } else { m_jet_fracSampMaxIdx.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> lowEtFrac ("LowEtConstituentsFrac");
      if( lowEtFrac.isAvailable( *jet_itr ) ) {
        m_jet_lowEtFrac.push_back( lowEtFrac( *jet_itr ) );
      } else { m_jet_lowEtFrac.push_back( -999 ); }

    } // energy

    if( m_jetInfoSwitch->m_layer ) {
      static SG::AuxElement::ConstAccessor< std::vector<float> > ePerSamp ("EnergyPerSampling");
      if( ePerSamp.isAvailable( *jet_itr ) ) {
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

    if( m_jetInfoSwitch->m_trackAll || m_jetInfoSwitch->m_trackPV ) {
      static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk1000("NumTrkPt1000");
      static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt1000("SumPtTrkPt1000");
      static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth1000("TrackWidthPt1000");
      static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk500 ("NumTrkPt500");
      static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt500 ("SumPtTrkPt500");
      static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth500 ("TrackWidthPt500");
      static SG::AuxElement::ConstAccessor< std::vector<float> > jvf("JVF");

      if( m_jetInfoSwitch->m_trackAll ) {

        std::vector<int> junkInt(1,-999);
        std::vector<float> junkFlt(1,-999);

        if( nTrk1000.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt1000.push_back( nTrk1000( *jet_itr ) );
        } else { m_jet_NTrkPt1000.push_back( junkInt ); }

        if( sumPt1000.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt1000.push_back( sumPt1000( *jet_itr ) );
        } else { m_jet_SumPtPt1000.push_back( junkFlt ); }

        if( trkWidth1000.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt1000.push_back( trkWidth1000( *jet_itr ) );
        } else { m_jet_TrkWPt1000.push_back( junkFlt ); }

        if( nTrk500.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt500.push_back( nTrk500( *jet_itr ) );
        } else { m_jet_NTrkPt500.push_back( junkInt ); }

        if( sumPt500.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt500.push_back( sumPt500( *jet_itr ) );
        } else { m_jet_SumPtPt500.push_back( junkFlt ); }

        if( trkWidth500.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt500.push_back( trkWidth500( *jet_itr ) );
        } else { m_jet_TrkWPt500.push_back( junkFlt ); }

        if( jvf.isAvailable( *jet_itr ) ) {
          m_jet_jvf.push_back( jvf( *jet_itr ) );
        } else { m_jet_jvf.push_back( junkFlt ); }

      }

      if( m_jetInfoSwitch->m_trackPV && pvLocation >= 0 ) {

        if( nTrk1000.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt1000PV.push_back( nTrk1000( *jet_itr )[pvLocation] );
        } else { m_jet_NTrkPt1000PV.push_back( -999 ); }

        if( sumPt1000.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt1000PV.push_back( sumPt1000( *jet_itr )[pvLocation] );
        } else { m_jet_SumPtPt1000PV.push_back( -999 ); }

        if( trkWidth1000.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt1000PV.push_back( trkWidth1000( *jet_itr )[pvLocation] );
        } else { m_jet_TrkWPt1000PV.push_back( -999 ); }

        if( nTrk500.isAvailable( *jet_itr ) ) {
          m_jet_NTrkPt500PV.push_back( nTrk500( *jet_itr )[pvLocation] );
        } else { m_jet_NTrkPt500PV.push_back( -999 ); }

        if( sumPt500.isAvailable( *jet_itr ) ) {
          m_jet_SumPtPt500PV.push_back( sumPt500( *jet_itr )[pvLocation] );
        } else { m_jet_SumPtPt500PV.push_back( -999 ); }

        if( trkWidth500.isAvailable( *jet_itr ) ) {
          m_jet_TrkWPt500PV.push_back( trkWidth500( *jet_itr )[pvLocation] );
        } else { m_jet_TrkWPt500PV.push_back( -999 ); }

        if( jvf.isAvailable( *jet_itr ) ) {
          m_jet_jvfPV.push_back( jvf( *jet_itr )[pvLocation] );
        } else { m_jet_jvfPV.push_back( -999 ); }

      }

    }

    if( m_jetInfoSwitch->m_flavTag) {
      static SG::AuxElement::ConstAccessor< float > MV1WeightAcc("MV1Weight");
      if( MV1WeightAcc.isAvailable( *jet_itr ) ) {
        m_jet_mv1.push_back( MV1WeightAcc( *jet_itr ) );
      } else { m_jet_mv1.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor< float > SV1plusIP3DWeightAcc("SV1plusIP3DWeight");
      if( SV1plusIP3DWeightAcc.isAvailable( *jet_itr ) ) {
        m_jet_sv1ip3d.push_back( SV1plusIP3DWeightAcc( *jet_itr ) );
      } else { m_jet_sv1ip3d.push_back( -999 ); }
    }

    if( m_jetInfoSwitch->m_truth ) {

      static SG::AuxElement::ConstAccessor<int> TruthLabelID ("TruthLabelID");
      if( TruthLabelID.isAvailable( *jet_itr) ) {
        m_jet_truthLabelID.push_back( TruthLabelID( *jet_itr) );
      } else { m_jet_truthLabelID.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> TruthCount ("TruthCount");
      if( TruthCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount.push_back( TruthCount( *jet_itr) );
      } else { m_jet_truthCount.push_back( -999 ); }

//    seems to be empty
//      static SG::AuxElement::ConstAccessor<float> TruthPt ("TruthPt");
//      if( TruthPt.isAvailable( *jet_itr) ) {
//        m_jet_truthPt.push_back( TruthPt( *jet_itr)/1000 );
//      } else { m_jet_truthPt.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_B ("TruthLabelDeltaR_B");
      if( TruthLabelDeltaR_B.isAvailable( *jet_itr) ) {
        m_jet_truthDr_B.push_back( TruthLabelDeltaR_B( *jet_itr) );
      } else { m_jet_truthDr_B.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_C ("TruthLabelDeltaR_C");
      if( TruthLabelDeltaR_C.isAvailable( *jet_itr) ) {
        m_jet_truthDr_C.push_back( TruthLabelDeltaR_C( *jet_itr) );
      } else { m_jet_truthDr_C.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_T ("TruthLabelDeltaR_T");
      if( TruthLabelDeltaR_T.isAvailable( *jet_itr) ) {
        m_jet_truthDr_T.push_back( TruthLabelDeltaR_T( *jet_itr) );
      } else { m_jet_truthDr_T.push_back( -999 ) ; }

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


    if( m_jetInfoSwitch->m_truthDetails ) {

      //
      // B-Hadron Details
      //
      static SG::AuxElement::ConstAccessor<int> GhostBHadronsFinalCount ("GhostBHadronsFinalCount");
      if( GhostBHadronsFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_BhadFinal.push_back( GhostBHadronsFinalCount( *jet_itr) );
      } else { m_jet_truthCount_BhadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostBHadronsInitialCount ("GhostBHadronsInitialCount");
      if( GhostBHadronsInitialCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_BhadInit.push_back( GhostBHadronsInitialCount( *jet_itr) );
      } else { m_jet_truthCount_BhadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostBQuarksFinalCount ("GhostBQuarksFinalCount");
      if( GhostBQuarksFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_BQFinal.push_back( GhostBQuarksFinalCount( *jet_itr) );
      } else { m_jet_truthCount_BQFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostBHadronsFinalPt ("GhostBHadronsFinalPt");
      if( GhostBHadronsFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_BhadFinal.push_back( GhostBHadronsFinalPt( *jet_itr) );
      } else { m_jet_truthPt_BhadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostBHadronsInitialPt ("GhostBHadronsInitialPt");
      if( GhostBHadronsInitialPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_BhadInit.push_back( GhostBHadronsInitialPt( *jet_itr) );
      } else { m_jet_truthPt_BhadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostBQuarksFinalPt ("GhostBQuarksFinalPt");
      if( GhostBQuarksFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_BQFinal.push_back( GhostBQuarksFinalPt( *jet_itr) );
      } else { m_jet_truthPt_BQFinal.push_back( -999 ); }


      //
      // C-Hadron Details
      //
      static SG::AuxElement::ConstAccessor<int> GhostCHadronsFinalCount ("GhostCHadronsFinalCount");
      if( GhostCHadronsFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_ChadFinal.push_back( GhostCHadronsFinalCount( *jet_itr) );
      } else { m_jet_truthCount_ChadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostCHadronsInitialCount ("GhostCHadronsInitialCount");
      if( GhostCHadronsInitialCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_ChadInit.push_back( GhostCHadronsInitialCount( *jet_itr) );
      } else { m_jet_truthCount_ChadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> GhostCQuarksFinalCount ("GhostCQuarksFinalCount");
      if( GhostCQuarksFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_CQFinal.push_back( GhostCQuarksFinalCount( *jet_itr) );
      } else { m_jet_truthCount_CQFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostCHadronsFinalPt ("GhostCHadronsFinalPt");
      if( GhostCHadronsFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_ChadFinal.push_back( GhostCHadronsFinalPt( *jet_itr) );
      } else { m_jet_truthPt_ChadFinal.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostCHadronsInitialPt ("GhostCHadronsInitialPt");
      if( GhostCHadronsInitialPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_ChadInit.push_back( GhostCHadronsInitialPt( *jet_itr) );
      } else { m_jet_truthPt_ChadInit.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> GhostCQuarksFinalPt ("GhostCQuarksFinalPt");
      if( GhostCQuarksFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_CQFinal.push_back( GhostCQuarksFinalPt( *jet_itr) );
      } else { m_jet_truthPt_CQFinal.push_back( -999 ); }


      //
      // Tau Details
      //
      static SG::AuxElement::ConstAccessor<int> GhostTausFinalCount ("GhostTausFinalCount");
      if( GhostTausFinalCount.isAvailable( *jet_itr) ) {
        m_jet_truthCount_TausFinal.push_back( GhostTausFinalCount( *jet_itr) );
      } else { m_jet_truthCount_TausFinal.push_back( -999 ); }


      static SG::AuxElement::ConstAccessor<float> GhostTausFinalPt ("GhostTausFinalPt");
      if( GhostTausFinalPt.isAvailable( *jet_itr) ) {
        m_jet_truthPt_TausFinal.push_back( GhostTausFinalPt( *jet_itr) );
      } else { m_jet_truthPt_TausFinal.push_back( -999 ); }

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
void HelpTreeBase::FillFatJets( const xAOD::JetContainer& /*fatJets*/ ) { }

void HelpTreeBase::ClearEvent() {}

void HelpTreeBase::ClearMuons() {
  m_nmuon = 0;  
  m_muon_pt.clear();
  m_muon_eta.clear();
  m_muon_phi.clear(); 
}

void HelpTreeBase::ClearElectrons() {
  m_nel = 0;  
  m_el_pt.clear();
  m_el_eta.clear();
  m_el_phi.clear();
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
    m_jet_LArBadHVE.clear();
    m_jet_LArBadHVRatio.clear();
  }

  // energy
  if( m_jetInfoSwitch->m_energy ) {
    m_jet_HECf.clear();
    m_jet_EMf.clear();
    m_jet_centroidR.clear();
    m_jet_fracSampMax.clear();
    m_jet_fracSampMaxIdx.clear();
    m_jet_lowEtFrac.clear();
  }

  // layer
  if( m_jetInfoSwitch->m_layer ) {
    m_jet_ePerSamp.clear();
  }

  // trackAll
  if( m_jetInfoSwitch->m_trackAll ) {
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
  if( m_jetInfoSwitch->m_trackPV ) {
    m_jet_NTrkPt1000PV.clear();
    m_jet_SumPtPt1000PV.clear();
    m_jet_TrkWPt1000PV.clear();
    m_jet_NTrkPt500PV.clear();
    m_jet_SumPtPt500PV.clear();
    m_jet_TrkWPt500PV.clear();
    m_jet_jvfPV.clear();
    //m_jet_jvfloosePV.clear();
  }

  // flavor tag
  if( m_jetInfoSwitch->m_flavTag ) {
    m_jet_mv1.clear();
    m_jet_sv1ip3d.clear();
  }

  // truth
  if( m_jetInfoSwitch->m_truth ) {
    m_jet_truthLabelID.clear();
    m_jet_truthCount.clear();
    m_jet_truthPt.clear();
    m_jet_truthDr_B.clear();
    m_jet_truthDr_C.clear();
    m_jet_truthDr_T.clear();
    m_jet_truth_pt.clear();
    m_jet_truth_eta.clear();
    m_jet_truth_phi.clear();
    m_jet_truth_E.clear();
  }

  // truth_detail
  if( m_jetInfoSwitch->m_truthDetails ) {
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
  }

}

bool HelpTreeBase::writeTo( TFile* file ) {
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if( status == 0 ) { return false; }
  return true;
}

