#include <iostream>

#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODEventInfo/EventInfo.h"

#include "TFile.h"
#include "TTree.h"

#include "xAODAnaHelpers/HelpTreeBase.h"

// needed? should it be here?
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

HelpTreeBase::HelpTreeBase(xAOD::TEvent * event, TTree* tree, TFile* file)
{

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
 *   MUONS
 *
 ********************/

void HelpTreeBase::AddEvent() { 
  m_tree->Branch("runNumber",          &m_runNumber,      "runNumber/I");
  m_tree->Branch("eventNumber",        &m_eventNumber,    "eventNumber/I");
  m_tree->Branch("mcEventNumber",      &m_mcEventNumber,  "mcEventNumber/I");
  m_tree->Branch("mcChannelNumber",    &m_mcChannelNumber,"mcChannelNumber/I");
}
void HelpTreeBase::FillEvent( const xAOD::EventInfo* eventInfo ) { 
  m_runNumber             = eventInfo->runNumber();
  m_eventNumber           = eventInfo->eventNumber();
  if(eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) {
    m_mcEventNumber         = eventInfo->mcEventNumber();
    m_mcChannelNumber       = eventInfo->mcChannelNumber();
  } else {
    m_mcEventNumber         = -1;
    m_mcChannelNumber       = -1;
  }
  FillEventUser(eventInfo);
}

/*********************
 *
 *   MUONS
 *
 ********************/

void HelpTreeBase::AddMuons() { 

  m_tree->Branch("muon_pt",  &m_muon_pt);
  m_tree->Branch("muon_phi", &m_muon_phi);
  m_tree->Branch("muon_eta", &m_muon_eta);

}
void HelpTreeBase::FillMuons( const xAOD::MuonContainer& muons ) { 
  xAOD::MuonContainer::const_iterator muon_itr = muons.begin();
  xAOD::MuonContainer::const_iterator muon_end = muons.end();
  m_nmuon = 0;
  for( ; muon_itr != muon_end; ++muon_itr ) {
    if( (*muon_itr)->isAvailable<int>("write") ) { 
      if( ! (*muon_itr)->auxdata< int >("write") ) { 
        continue;
      }
    }
    m_muon_pt.push_back ( (*muon_itr)->pt()  );
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

void HelpTreeBase::AddElectrons() { }
void HelpTreeBase::FillElectrons( const xAOD::ElectronContainer& electrons ) { }

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
    if( (*jet_itr)->isAvailable<int>("write") ) { 
      if( ! (*jet_itr)->auxdata< int >("write") ) { 
        continue;
      }
    }
    m_jet_pt.push_back ( (*jet_itr)->pt()  );
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
void HelpTreeBase::FillFatJets( const xAOD::JetContainer& fatJets ) { }

void HelpTreeBase::Clear() { 

  m_njet = 0;
  m_jet_pt.clear();
  m_jet_eta.clear();
  m_jet_phi.clear();
  m_jet_E.clear();

  this->ClearUser();
}

bool HelpTreeBase::writeTo( TFile* file ) { 
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if( status == 0 ) { return false; }
  return true;
}

