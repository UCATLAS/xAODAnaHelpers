// Dear emacs, this is -*-c++-*-
#ifndef xAODAnaHelpers_HelpTreeBase_H
#define xAODAnaHelpers_HelpTreeBase_H

#include "xAODRootAccess/TEvent.h"

namespace xAOD {
#ifndef XAODEVENTINFO_EVENTINFO_H
    class EventInfo;
#endif
#ifndef XAODMUON_MUONCONTAINER_H 
    class MuonContainer;
#endif
#ifndef XAODMUON_MUON_H 
      class Muon;
#endif
#ifndef XAODEGAMMA_ELECTRONCONTAINER_H 
  class ElectronContainer;
#endif
#ifndef XAODEGAMMA_ELECTRON_H 
    class Electron;
#endif
#ifndef XAODJET_JETCONTAINER_H 
    class JetContainer;
#endif
#ifndef XAODJET_JET_H 
      class Jet;
#endif
      class TFile;
}

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


class HelpTreeBase {

public:

  HelpTreeBase(xAOD::TEvent * event, TTree* tree, TFile* file);
  virtual ~HelpTreeBase() {;}

  void AddEvent();
  void AddMuons();
  void AddElectrons();
  void AddJets();
  void AddFatJets();

  void FillEvent( const xAOD::EventInfo* eventInfo );
  void FillMuons( const xAOD::MuonContainer& muons );
  void FillElectrons( const xAOD::ElectronContainer& electrons );
  void FillJets( const xAOD::JetContainer& jets );
  void FillFatJets( const xAOD::JetContainer& fatJets );

  void Fill();
  void Clear();

  bool writeTo( TFile *file );
  
  // User defined functions
  virtual void AddUser()                                           = 0;
  virtual void FillEventUser( const xAOD::EventInfo* eventInfo )   = 0;
  virtual void FillMuonsUser( const xAOD::Muon* muon )             = 0;
  virtual void FillElectronsUser( const xAOD::Electron* electron ) = 0;
  virtual void FillJetsUser( const xAOD::Jet* jet )                = 0;
  virtual void FillFatJetsUser( const xAOD::Jet* fatJet )          = 0;
  virtual void ClearUser()                                         = 0;

protected:

  TTree* m_tree;

  // event
  int m_runNumber;
  int m_eventNumber;
  int m_mcEventNumber;
  int m_mcChannelNumber;

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

};

#endif
