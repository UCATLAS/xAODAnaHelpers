// c++ include(s):
#include <iostream>

// package include(s):
#include "xAODAnaHelpers/HelpTreeBase.h"

// needed? should it be here?
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

/* TODO: event */
HelpTreeBase::HelpTreeBase(xAOD::TEvent * /*event*/, TTree* tree, TFile* file, int units /*= 1e3 */)
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

void HelpTreeBase::AddEvent() {
  m_tree->Branch("runNumber",          &m_runNumber,      "runNumber/I");
  m_tree->Branch("eventNumber",        &m_eventNumber,    "eventNumber/I");
  m_tree->Branch("mcEventNumber",      &m_mcEventNumber,  "mcEventNumber/I");
  m_tree->Branch("mcChannelNumber",    &m_mcChannelNumber,"mcChannelNumber/I");
  m_tree->Branch("mcEventWeight",      &m_mcEventWeight,  "mcEventWeight/F");
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

void HelpTreeBase::AddMuons() {
  m_tree->Branch("nmuon",    &m_nmuon,"nmuon/I");
  m_tree->Branch("muon_pt",  &m_muon_pt);
  m_tree->Branch("muon_phi", &m_muon_phi);
  m_tree->Branch("muon_eta", &m_muon_eta);

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

void HelpTreeBase::AddElectrons() { 
  m_tree->Branch("nel",    &m_nel,"nel/I");
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

void HelpTreeBase::AddJets()
{

  m_tree->Branch("njet",    &m_njet,"njet/I");
  m_tree->Branch("jet_E",   &m_jet_E);
  m_tree->Branch("jet_pt",  &m_jet_pt);
  m_tree->Branch("jet_phi", &m_jet_phi);
  m_tree->Branch("jet_eta", &m_jet_eta);

}

void HelpTreeBase::FillJets( const xAOD::JetContainer& jets ) {
  xAOD::JetContainer::const_iterator jet_itr = jets.begin();
  xAOD::JetContainer::const_iterator jet_end = jets.end();
  m_njet = 0;
  for( ; jet_itr != jet_end; ++jet_itr ) {
    m_jet_pt.push_back ( (*jet_itr)->pt() / m_units );
    m_jet_eta.push_back( (*jet_itr)->eta() );
    m_jet_phi.push_back( (*jet_itr)->phi() );
    m_jet_E .push_back ( (*jet_itr)->e()   );
    this->FillJetsUser(*jet_itr);
    m_njet++;
  }
}

/*********************
 *
 *   FAT JETS
 *
 ********************/

void HelpTreeBase::AddFatJets() { }
/* TODO: fatJets */
void HelpTreeBase::FillFatJets( const xAOD::JetContainer& /*fatJets*/ ) { }

void HelpTreeBase::Clear() {

  m_njet = 0;
  m_jet_pt.clear();
  m_jet_eta.clear();
  m_jet_phi.clear();
  m_jet_E.clear();
    
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

