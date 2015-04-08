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

#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODRootAccess/TEvent.h"

#include "TTree.h"
#include "TFile.h"

class HelpTreeBase {

public:

#ifndef __CINT__  
  HelpTreeBase(xAOD::TEvent *event, TTree* tree, TFile* file, const float units = 1e3 );
#endif // not __CINT__ 
  virtual ~HelpTreeBase() {;}

  void AddEvent    (const std::string detailStr = "");
  void AddMuons    (const std::string detailStr = "");
  void AddElectrons(const std::string detailStr = "");
  void AddJets     (const std::string detailStr = "");
  void AddFatJets  (const std::string detailStr = "");

  // control which branches are filled
  HelperClasses::EventInfoSwitch*     m_eventInfoSwitch;
  HelperClasses::MuonInfoSwitch*      m_muInfoSwitch;
  HelperClasses::ElectronInfoSwitch*  m_elInfoSwitch;
  HelperClasses::JetInfoSwitch*       m_jetInfoSwitch;
  HelperClasses::JetInfoSwitch*       m_fatJetInfoSwitch;

#ifndef __CINT__  
  void FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event = 0 );
  void FillMuons( const xAOD::MuonContainer& muons );
  void FillElectrons( const xAOD::ElectronContainer& electrons );
  void FillJets( const xAOD::JetContainer& jets, int pvLocation = -1 );
  void FillFatJets( const xAOD::JetContainer& fatJets );
#endif // not __CINT__ 
  void Fill();
  void ClearEvent();
  void ClearMuons();
  void ClearElectrons();
  void ClearJets();

  bool writeTo( TFile *file );

  // User defined functions
  virtual void AddEventUser(const std::string detailStr = "")      { return; };
  virtual void AddMuonsUser(const std::string detailStr = "")      { return; };  
  virtual void AddElectronsUser(const std::string detailStr = "")  { return; };  
  virtual void AddJetsUser(const std::string detailStr = "")       { return; };   
  
  virtual void ClearEventUser()                                    { return; };
  virtual void ClearMuonsUser() 				   { return; };
  virtual void ClearElectronsUser() 				   { return; };
  virtual void ClearJetsUser() 					   { return; };
 
#ifndef __CINT__  
  virtual void FillEventUser( const xAOD::EventInfo* eventInfo )   { return; };
  virtual void FillMuonsUser( const xAOD::Muon* muon )             { return; };
  virtual void FillElectronsUser( const xAOD::Electron* electron ) { return; };
  virtual void FillJetsUser( const xAOD::Jet* jet )                { return; };
  virtual void FillFatJetsUser( const xAOD::Jet* fatJet )          { return; };
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
  // event pileup
  int m_npv;
  float m_actualMu;
  float m_averageMu;
  int m_lumiBlock;
  // event shapeEM
  double m_rhoEM;
  double m_rhoLC;


  // jets
  int m_njet;
  std::vector<float> m_jet_pt;
  std::vector<float> m_jet_eta;
  std::vector<float> m_jet_phi;
  std::vector<float> m_jet_E;

  // clean
  std::vector<float> m_jet_time;
  std::vector<float> m_jet_LArQuality;
  std::vector<float> m_jet_hecq;
  std::vector<float> m_jet_negE;
  std::vector<float> m_jet_avLArQF;
  std::vector<float> m_jet_bchCorrCell;
  std::vector<float> m_jet_N90Const;
  std::vector<float> m_jet_LArBadHVE;
  std::vector<float> m_jet_LArBadHVRatio;

  // energy
  std::vector<float> m_jet_HECf;
  std::vector<float> m_jet_EMf;
  std::vector<float> m_jet_centroidR;
  std::vector<float> m_jet_fracSampMax;
  std::vector<float> m_jet_fracSampMaxIdx;
  std::vector<float> m_jet_lowEtFrac;

  // layer
  std::vector< std::vector<float> > m_jet_ePerSamp;

  // tracksAll
  std::vector< std::vector<int> >   m_jet_NTrkPt1000;
  std::vector< std::vector<float> > m_jet_SumPtPt1000;
  std::vector< std::vector<float> > m_jet_TrkWPt1000;
  std::vector< std::vector<int> >   m_jet_NTrkPt500;
  std::vector< std::vector<float> > m_jet_SumPtPt500;
  std::vector< std::vector<float> > m_jet_TrkWPt500;
  std::vector< std::vector<float> > m_jet_jvf;
  std::vector< std::vector<float> > m_jet_jvfloose;

  // tracksPV
  std::vector<int>   m_jet_NTrkPt1000PV;
  std::vector<float> m_jet_SumPtPt1000PV;
  std::vector<float> m_jet_TrkWPt1000PV;
  std::vector<int>   m_jet_NTrkPt500PV;
  std::vector<float> m_jet_SumPtPt500PV;
  std::vector<float> m_jet_TrkWPt500PV;
  std::vector<float> m_jet_jvfPV;
  //std::vector<float> m_jet_jvfloosePV;

  // flavor tag
  std::vector<float> m_jet_mv1;
  std::vector<float> m_jet_sv1ip3d;

  // truth
  std::vector<int>   m_jet_truthLabelID;
  std::vector<int>   m_jet_truthCount;
  std::vector<float> m_jet_truthPt;
  std::vector<float> m_jet_truthDr_B;
  std::vector<float> m_jet_truthDr_C;
  std::vector<float> m_jet_truthDr_T;
  std::vector<float> m_jet_truth_pt;
  std::vector<float> m_jet_truth_eta;
  std::vector<float> m_jet_truth_phi;
  std::vector<float> m_jet_truth_E;

  // truth detail
  std::vector<int>   m_jet_truthCount_BhadFinal;
  std::vector<int>   m_jet_truthCount_BhadInit;
  std::vector<int>   m_jet_truthCount_BQFinal;
  std::vector<float> m_jet_truthPt_BhadFinal;
  std::vector<float> m_jet_truthPt_BhadInit;
  std::vector<float> m_jet_truthPt_BQFinal;
  std::vector<int>   m_jet_truthCount_ChadFinal;
  std::vector<int>   m_jet_truthCount_ChadInit;
  std::vector<int>   m_jet_truthCount_CQFinal;
  std::vector<float> m_jet_truthPt_ChadFinal;
  std::vector<float> m_jet_truthPt_ChadInit;
  std::vector<float> m_jet_truthPt_CQFinal;
  std::vector<int>   m_jet_truthCount_TausFinal;
  std::vector<float> m_jet_truthPt_TausFinal;

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
