/***************************************************
 * HelpTreeBase:
 *
 * This class is meant to help the user write out a tree.
 * Some branches are included by default while others
 * need to be added by the user
 *
 * Gabriel Facini ( gabriel.facini@cern.ch )
 * Fri Nov 14 15:36:19 CET 2014
 *
 ***************************************************/

// Dear emacs, this is -*-c++-*-
#ifndef xAODAnaHelpers_HelpTreeBase_H
#define xAODAnaHelpers_HelpTreeBase_H

#ifndef __CINT__    
  #include "xAODEventInfo/EventInfo.h"
  #include "xAODEgamma/ElectronContainer.h"
  #include "xAODEgamma/Electron.h"
  #include "xAODEgamma/PhotonContainer.h"
  #include "xAODEgamma/Photon.h"
  #include "xAODMuon/MuonContainer.h"
  #include "xAODMuon/Muon.h"
  #include "xAODJet/JetContainer.h"
  #include "xAODJet/Jet.h"
  #include "xAODTau/TauJetContainer.h"
  #include "xAODTau/TauJet.h"
#endif // not __CINT__  

#include "xAODRootAccess/TEvent.h"

#include "TTree.h"
#include "TFile.h"

class HelpTreeBase {

public:

#ifndef __CINT__  
  HelpTreeBase(xAOD::TEvent * event, TTree* tree, TFile* file, int units=1e3 );
#endif // not __CINT__ 
  virtual ~HelpTreeBase() {;}

  void AddEvent();
  void AddMuons();
  void AddElectrons();
  void AddJets();
  void AddFatJets();
#ifndef __CINT__  
  void FillEvent( const xAOD::EventInfo* eventInfo );
  void FillMuons( const xAOD::MuonContainer& muons );
  void FillElectrons( const xAOD::ElectronContainer& electrons );
  void FillJets( const xAOD::JetContainer& jets );
  void FillFatJets( const xAOD::JetContainer& fatJets );
#endif // not __CINT__ 
  void Fill();
  void Clear();

  bool writeTo( TFile *file );

  // User defined functions
  virtual void AddUser()                                           = 0;
#ifndef __CINT__  
  virtual void FillEventUser( const xAOD::EventInfo* eventInfo )   = 0;
  virtual void FillMuonsUser( const xAOD::Muon* muon )             = 0;
  virtual void FillElectronsUser( const xAOD::Electron* electron ) = 0;
  virtual void FillJetsUser( const xAOD::Jet* jet )                = 0;
  virtual void FillFatJetsUser( const xAOD::Jet* fatJet )          = 0;
  virtual void ClearUser()                                         = 0;
#endif // not __CINT__ 

protected:

  TTree* m_tree;

  int m_units; //For MeV to GeV conversion in output

  // event
  int m_runNumber;
  int m_eventNumber;
  int m_mcEventNumber;
  int m_mcChannelNumber;
  float m_mcEventWeight;

  // jets
  int m_njet;
  std::vector<float> m_jet_pt;
  std::vector<float> m_jet_eta;
  std::vector<float> m_jet_phi;
  std::vector<float> m_jet_E;

  // muons
  int m_nmuon;
  std::vector<float> m_muon_pt;
  std::vector<float> m_muon_eta;
  std::vector<float> m_muon_phi;
  
  // electrons
  int m_nel;
  std::vector<float> m_el_pt;
  std::vector<float> m_el_phi;
  std::vector<float> m_el_eta;

};

#endif
