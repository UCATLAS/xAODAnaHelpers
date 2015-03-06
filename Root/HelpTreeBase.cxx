// c++ include(s):
#include <iostream>

// package include(s):
#include "xAODAnaHelpers/HelpTreeBase.h"

// needed? should it be here?
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

/* TODO: event */
HelpTreeBase::HelpTreeBase(xAOD::TEvent * /*event*/, TTree* tree, TFile* file, int units /*= 1e3 */):
  m_evtDetailStr(""),
  m_muDetailStr(""),
  m_elDetailStr(""),
  m_jetInfoSwitch(0),
  m_fatJetDetailStr("")
{

  m_units = units;
  m_tree = tree;
  m_tree->SetDirectory( file );
  std::cout << "HelpTreeBase::HelpTreeBase setup " << std::endl;

}

void HelpTreeBase::Fill() {
  m_tree->Fill();
  this->Clear();
}

/*********************
 *
 *   EVENT
 *
 ********************/

void HelpTreeBase::AddEvent(std::string detailStr) {
  m_tree->Branch("evtsel_run_number",          &m_runNumber,      "evtsel_run_number/I");
  m_tree->Branch("evtsel_event_number",        &m_eventNumber,    "evtsel_event_number/I");
  m_tree->Branch("evtsel_mc_event_number",      &m_mcEventNumber,  "evtsel_mc_event_number/I");
  m_tree->Branch("evtsel_mc_channel_number",    &m_mcChannelNumber,"evtsel_mc_channel_number/I");
  m_tree->Branch("evtsel_weight",      &m_mcEventWeight,  "evtsel_weight/F");
}
void HelpTreeBase::FillEvent( const xAOD::EventInfo* eventInfo ) {
  m_runNumber             = eventInfo->runNumber();
  m_eventNumber           = eventInfo->eventNumber();
  if(eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) {
    m_mcEventNumber         = eventInfo->mcEventNumber();
    m_mcChannelNumber       = eventInfo->mcChannelNumber();
    if( eventInfo->isAvailable< float >( "mcEventWeight" ) ){
      m_mcEventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
    } else {
      std::cout << "HelpTreeBase::FillEvent() - WARNING! Couldn't retrieve mcEventWeight! " << std::endl;
    }
  } else {
    m_mcEventNumber         = -1;
    m_mcChannelNumber       = -1;
    m_mcEventWeight	    = 1.;
  }
  FillEventUser(eventInfo);
}

/*********************
 *
 *   MUONS
 *
 ********************/

void HelpTreeBase::AddMuons(std::string detailStr) {
  m_tree->Branch("evtsel_mu_num",    &m_nmuon, "evtsel_mu_num/I");
  m_tree->Branch("mu_staco_n",   &m_nmuon, "mu_staco_n/I");  
  m_tree->Branch("mu_staco_pt",  &m_muon_pt);
  m_tree->Branch("mu_staco_phi", &m_muon_phi);
  m_tree->Branch("mu_staco_eta", &m_muon_eta);

}
void HelpTreeBase::FillMuons( const xAOD::MuonContainer& muons ) {
  xAOD::MuonContainer::const_iterator muon_itr = muons.begin();
  xAOD::MuonContainer::const_iterator muon_end = muons.end();
  m_nmuon = 0;
  for( ; muon_itr != muon_end; ++muon_itr ) {
    m_muon_pt.push_back ( (*muon_itr)->pt() / m_units  );
    m_muon_eta.push_back( (*muon_itr)->eta() );
    m_muon_phi.push_back( (*muon_itr)->phi() );
    this->FillMuonsUser(*muon_itr);
    m_nmuon++;
  }
}

/*********************
 *
 *   ELECTRONS
 *
 ********************/

void HelpTreeBase::AddElectrons(std::string detailStr) { 
  m_tree->Branch("evtsel_el_num",    &m_nel,"evtsel_el_num/I");
  m_tree->Branch("el_n",    &m_nel,"el_n/I");  
  m_tree->Branch("el_pt",  &m_el_pt);
  m_tree->Branch("el_phi", &m_el_phi);
  m_tree->Branch("el_eta", &m_el_eta);

}
void HelpTreeBase::FillElectrons( const xAOD::ElectronContainer& electrons ) { 
  xAOD::ElectronContainer::const_iterator el_itr = electrons.begin();
  xAOD::ElectronContainer::const_iterator el_end = electrons.end();
  m_nel = 0;
  for( ; el_itr != el_end; ++el_itr ) {
    m_el_pt.push_back ( (*el_itr)->pt()  ); 
    m_el_eta.push_back( (*el_itr)->eta() );
    m_el_phi.push_back( (*el_itr)->phi() );
    this->FillElectronsUser(*el_itr);
    m_nel++;
  }
}

/*********************
 *
 *   JETS
 *
 ********************/

void HelpTreeBase::AddJets(std::string detailStr)
{

  m_jetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );

  // always
  m_tree->Branch("evtsel_jets_num",    &m_njet,"evtsel_jets_num/I");

  // TLorentzVector
//  if( m_jetInfoSwitch->m_TLV ) {
//    // add TLorentzVector instead of flat variables
//  } else {
    m_tree->Branch("jet_AntiKt4LCTopo_E",   &m_jet_E);
    m_tree->Branch("jet_AntiKt4LCTopo_pt",  &m_jet_pt);
    m_tree->Branch("jet_AntiKt4LCTopo_phi", &m_jet_phi);
    m_tree->Branch("jet_AntiKt4LCTopo_eta", &m_jet_eta);
//  }

  if( m_jetInfoSwitch->m_clean ) { 
    m_tree->Branch("jet_AntiKt4LCTopo_Timing",              &m_jet_time               );
    m_tree->Branch("jet_AntiKt4LCTopo_LArQuality",          &m_jet_LArQuality         );
    m_tree->Branch("jet_AntiKt4LCTopo_HECQuality",          &m_jet_hecq               );
    m_tree->Branch("jet_AntiKt4LCTopo_NegativeE",           &m_jet_negE               );
    m_tree->Branch("jet_AntiKt4LCTopo_AverageLArQF",        &m_jet_avLArQF            );
    m_tree->Branch("jet_AntiKt4LCTopo_BchCorrCell",         &m_jet_bchCorrCell        );
    m_tree->Branch("jet_AntiKt4LCTopo_N90Constituents",     &m_jet_N90Const           );
    m_tree->Branch("jet_AntiKt4LCTopo_LArBadHVEnergyFrac",  &m_jet_LArBadHVEFrac      );
    m_tree->Branch("jet_AntiKt4LCTopo_LArBadHVNCellFrac",   &m_jet_LArBadHVNCellFrac  );
  }

  if( m_jetInfoSwitch->m_energy ) {
    m_tree->Branch("jet_AntiKt4LCTopo_HECFrac", 	      &m_jet_HECf	    );
    m_tree->Branch("jet_AntiKt4LCTopo_EMFrac",  	      &m_jet_EMf	    );
    m_tree->Branch("jet_AntiKt4LCTopo_CentroidR",	      &m_jet_centroidR      );
    m_tree->Branch("jet_AntiKt4LCTopo_FracSamplingMax",       &m_jet_fracSampMax    );
    m_tree->Branch("jet_AntiKt4LCTopo_FracSamplingMaxIndex",  &m_jet_fracSampMaxIdx );
    m_tree->Branch("jet_AntiKt4LCTopo_LowEtConstituentsFrac", &m_jet_lowEtFrac      );
  }		       

  if( m_jetInfoSwitch->m_layer ) { 
    m_tree->Branch("jet_AntiKt4LCTopo_EnergyPerSampling",     &m_jet_ePerSamp   );
  }

  if( m_jetInfoSwitch->m_trackAll ) { 
    m_tree->Branch("jet_AntiKt4LCTopo_NumTrkPt1000",	  &m_jet_NTrkPt1000   );
    m_tree->Branch("jet_AntiKt4LCTopo_SumPtTrkPt1000",    &m_jet_SumPtPt1000  );
    m_tree->Branch("jet_AntiKt4LCTopo_TrackWidthPt1000",  &m_jet_TrkWPt1000   );
    m_tree->Branch("jet_AntiKt4LCTopo_NumTrkPt500",	  &m_jet_NTrkPt500    );
    m_tree->Branch("jet_AntiKt4LCTopo_SumPtTrkPt500",	  &m_jet_SumPtPt500   );
    m_tree->Branch("jet_AntiKt4LCTopo_TrackWidthPt500",   &m_jet_TrkWPt500    );
    m_tree->Branch("jet_AntiKt4LCTopo_JVF",		  &m_jet_jvf	      );
    m_tree->Branch("jet_AntiKt4LCTopo_JVFLoose",          &m_jet_jvfloose     );
    // JVT needs to be added

    // NumTrkPt1000 vector<int> 
    // SumPtTrkPt1000, TrackWidthPt1000 vector<float>
    // NumTrkPt500  vector<int> Moments calculated from ghost-associated tracks. Track pT > 0.5 GeV.(1)
    // SumPtTrkPt500, TrackWidthPt500 vector<float>
    // Width  float $ \sum( \Delta R(jet,constit) P_T(constit) ) / \sum P_T(constit) $  Done
    //
    // JVF  JVF vector<float> Vector JVF: JVF with respect to all vertices (last index is to dummy vertex)  Done  JetVertexFractionTool jvf
    // JVF  JVFLoose  vector<float> As preceding adding posterior InDet loose track selection.  Done  JetVertexFractionTool jvfloose
    // HigestJVFLooseVtx  Vertex
    // JVT  Jvt, JvtRpt, JvtJvfcorr float JVT, etc., see Twiki
  }

  if( m_jetInfoSwitch->m_trackPV ) { 
    m_tree->Branch("jet_AntiKt4LCTopo_NumTrkPt1000PV",      &m_jet_NTrkPt1000PV   );
    m_tree->Branch("jet_AntiKt4LCTopo_SumPtTrkPt1000PV",    &m_jet_SumPtPt1000PV  );
    m_tree->Branch("jet_AntiKt4LCTopo_TrackWidthPt1000PV",  &m_jet_TrkWPt1000PV   );
    m_tree->Branch("jet_AntiKt4LCTopo_NumTrkPt500PV",	    &m_jet_NTrkPt500PV    );
    m_tree->Branch("jet_AntiKt4LCTopo_SumPtTrkPt500PV",     &m_jet_SumPtPt500PV   );
    m_tree->Branch("jet_AntiKt4LCTopo_TrackWidthPt500PV",   &m_jet_TrkWPt500PV    );
    m_tree->Branch("jet_AntiKt4LCTopo_JVFPV",		    &m_jet_jvfPV	  );
    m_tree->Branch("jet_AntiKt4LCTopo_JVFLoosePV",          &m_jet_jvfloosePV     );
  }

}

void HelpTreeBase::FillJets( const xAOD::JetContainer& jets ) {
  m_njet = 0;
  for( auto jet_itr : jets ) {
    m_jet_pt.push_back ( jet_itr->pt() / m_units );
    m_jet_eta.push_back( jet_itr->eta() );
    m_jet_phi.push_back( jet_itr->phi() );
    m_jet_E .push_back ( jet_itr->e() / m_units );

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

      static SG::AuxElement::ConstAccessor<float> LArBadHVEFrac ("LArBadHVEnergyFrac");
      if( LArBadHVEFrac.isAvailable( *jet_itr ) ) {
        m_jet_LArBadHVEFrac.push_back( LArBadHVEFrac( *jet_itr ) );
      } else { m_jet_LArBadHVEFrac.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> LArBadHVNCellFrac ("LArBadHVNCellFrac");
      if( LArBadHVNCellFrac.isAvailable( *jet_itr ) ) {
        m_jet_LArBadHVNCellFrac.push_back( LArBadHVNCellFrac( *jet_itr ) );
      } else { m_jet_LArBadHVNCellFrac.push_back( -999 ); }

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

      static SG::AuxElement::ConstAccessor<float> fracSampMaxIdx ("FracSamplingMaxIndex");
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

    if( m_jetInfoSwitch->m_trackAll ) {
    }

    if( m_jetInfoSwitch->m_trackPV ) {
    }

    this->FillJetsUser(jet_itr);
    m_njet++;
  }
}

/*********************
 *
 *   FAT JETS
 *
 ********************/

void HelpTreeBase::AddFatJets(std::string detailStr) { }
/* TODO: fatJets */
void HelpTreeBase::FillFatJets( const xAOD::JetContainer& /*fatJets*/ ) { }

void HelpTreeBase::Clear() {

  m_njet = 0;
  m_jet_pt.clear();
  m_jet_eta.clear();
  m_jet_phi.clear();
  m_jet_E.clear();

  // clean
  m_jet_time.clear();
  m_jet_LArQuality.clear();
  m_jet_hecq.clear();
  m_jet_negE.clear();
  m_jet_avLArQF.clear();
  m_jet_bchCorrCell.clear();
  m_jet_N90Const.clear();
  m_jet_LArBadHVEFrac.clear();
  m_jet_LArBadHVNCellFrac.clear();

  // energy
  m_jet_HECf.clear();
  m_jet_EMf.clear();
  m_jet_centroidR.clear();
  m_jet_fracSampMax.clear();
  m_jet_fracSampMaxIdx.clear();
  m_jet_lowEtFrac.clear();

  // layer
  m_jet_ePerSamp.clear();

  // tracksAll
  m_jet_NTrkPt1000.clear();
  m_jet_SumPtPt1000.clear();
  m_jet_TrkWPt1000.clear();
  m_jet_NTrkPt500.clear();
  m_jet_SumPtPt500.clear();
  m_jet_TrkWPt500.clear();
  m_jet_jvf.clear();
  m_jet_jvfloose.clear();

  // tracksPV
  m_jet_NTrkPt1000PV.clear();
  m_jet_SumPtPt1000PV.clear();
  m_jet_TrkWPt1000PV.clear();
  m_jet_NTrkPt500PV.clear();
  m_jet_SumPtPt500PV.clear();
  m_jet_TrkWPt500PV.clear();
  m_jet_jvfPV.clear();
  m_jet_jvfloosePV.clear();



  m_nmuon = 0;  
  m_muon_pt.clear();
  m_muon_eta.clear();
  m_muon_phi.clear();    
    
  m_nel = 0;  
  m_el_pt.clear();
  m_el_eta.clear();
  m_el_phi.clear();
    
  this->ClearUser();
}

bool HelpTreeBase::writeTo( TFile* file ) {
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if( status == 0 ) { return false; }
  return true;
}

