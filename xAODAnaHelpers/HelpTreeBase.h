/***************************************************
 * HelpTreeBase:
 *
 * This class is meant to help the user write out a tree.
 * Some branches are included by default while others
 * need to be added by the user
 *
 * Gabriel Facini ( gabriel.facini@cern.ch ), Marco Milesi (marco.milesi@cern.ch)
 * 
 *
 ***************************************************/

// Dear emacs, this is -*-c++-*-
#ifndef xAODAnaHelpers_HelpTreeBase_H
#define xAODAnaHelpers_HelpTreeBase_H

#include "xAODEventShape/EventShape.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODTau/TauJetContainer.h"

#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODRootAccess/TEvent.h"

// root includes
#include "TTree.h"
#include "TFile.h"

class HelpTreeBase {

public:

  HelpTreeBase(xAOD::TEvent *event, TTree* tree, TFile* file, const float units = 1e3, bool debug = false );
  virtual ~HelpTreeBase() {;}

 // virtual void setDebugMode(  bool debug ){
 //   m_debug = debug;
 // } 

  void AddEvent    (const std::string detailStr = "");
  void AddMuons    (const std::string detailStr = "");
  void AddElectrons(const std::string detailStr = "");
  void AddJets     (const std::string detailStr = "");
  void AddFatJets  (const std::string detailStr = "");
  void AddTaus     (const std::string detailStr = "");
  

  // control which branches are filled
  HelperClasses::EventInfoSwitch*     m_eventInfoSwitch;
  HelperClasses::MuonInfoSwitch*      m_muInfoSwitch;
  HelperClasses::ElectronInfoSwitch*  m_elInfoSwitch;
  HelperClasses::JetInfoSwitch*       m_jetInfoSwitch;
  HelperClasses::JetInfoSwitch*       m_fatJetInfoSwitch;
  HelperClasses::TauInfoSwitch*       m_tauInfoSwitch;
  

  void FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event = 0 );
  void FillMuons( const xAOD::MuonContainer* muons, const xAOD::Vertex* primaryVertex );
  void FillElectrons( const xAOD::ElectronContainer* electrons, const xAOD::Vertex* primaryVertex );
  void FillJets( const xAOD::JetContainer* jets, int pvLocation = -1 );
  void FillFatJets( const xAOD::JetContainer* fatJets );
  void FillTaus( const xAOD::TauJetContainer* taus );
  
  void Fill();
  void ClearEvent();
  void ClearMuons();
  void ClearElectrons();
  void ClearJets();
  void ClearTaus();

  bool writeTo( TFile *file );

  virtual void AddEventUser(const std::string detailStr = "")      { 
    Info("AddEventUser","Empty function called from HelpTreeBase %s",detailStr.c_str()); 
    return; 
  };
  virtual void AddMuonsUser(const std::string detailStr = "")      {
    Info("AddMuonsUser","Empty function called from HelpTreeBase %s",detailStr.c_str()); 
    return; 
  };
  virtual void AddElectronsUser(const std::string detailStr = "")  {
    Info("AddElectronsUser","Empty function called from HelpTreeBase %s",detailStr.c_str()); 
    return; 
  };
  virtual void AddJetsUser(const std::string detailStr = "")       {
    Info("AddJetsUser","Empty function called from HelpTreeBase %s",detailStr.c_str()); 
    return; 
  };
  virtual void AddTausUser(const std::string detailStr = "")       {
    Info("AddTausUser","Empty function called from HelpTreeBase %s",detailStr.c_str()); 
    return; 
  };
  
  virtual void ClearEventUser()     { return; };
  virtual void ClearMuonsUser()     { return; };
  virtual void ClearElectronsUser() { return; };
  virtual void ClearJetsUser() 	    { return; };
  virtual void ClearTausUser() 	    { return; };
  

  virtual void FillEventUser( const xAOD::EventInfo* /*eventInfo*/ )   { return; };
  virtual void FillMuonsUser( const xAOD::Muon* /*muon*/ )             { return; };
  virtual void FillElectronsUser( const xAOD::Electron* /*electron*/ ) { return; };
  virtual void FillJetsUser( const xAOD::Jet* /*jet*/ )                { return; };
  virtual void FillFatJetsUser( const xAOD::Jet* /*fatJet*/ )          { return; };
  virtual void FillTausUser( const xAOD::TauJet* /*tau*/ )             { return; };
  

protected:

  TTree* m_tree;

  int m_units; //For MeV to GeV conversion in output
  
  bool m_debug;

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
  // truth
  int m_pdgId1;
  int m_pdgId2;
  int m_pdfId1;
  int m_pdfId2;
  float m_x1;
  float m_x2;
  //float m_scale;
  float m_q;
  //float m_pdf1;
  //float m_pdf2;
  float m_xf1;
  float m_xf2;

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
  std::vector<float> m_jet_LArBadHVEFrac;
  std::vector<int> m_jet_LArBadHVNCell;
  std::vector<float> m_jet_OotFracClus5;
  std::vector<float> m_jet_OotFracClus10;

  // energy
  std::vector<float> m_jet_HECf;
  std::vector<float> m_jet_EMf;
  std::vector<float> m_jet_centroidR;
  std::vector<float> m_jet_fracSampMax;
  std::vector<float> m_jet_fracSampMaxIdx;
  std::vector<float> m_jet_lowEtFrac;
  std::vector<float> m_jet_muonSegCount;

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

  // allTrack
  std::vector<int> m_jet_GhostTrackCount;
  std::vector<float> m_jet_GhostTrackPt;
  std::vector< std::vector<float> > m_jet_GhostTrack_pt;
  std::vector< std::vector<float> > m_jet_GhostTrack_eta;
  std::vector< std::vector<float> > m_jet_GhostTrack_phi;
  std::vector< std::vector<float> > m_jet_GhostTrack_e;

  // flavor tag
  std::vector<float> m_jet_sv0;
  std::vector<float> m_jet_sv1;
  std::vector<float> m_jet_ip3d;
  std::vector<float> m_jet_sv1ip3d;
  std::vector<float> m_jet_mv1;
  std::vector<float> m_jet_mv2c00;
  std::vector<float> m_jet_mv2c20;

  // truth
  std::vector<int>   m_jet_truthConeLabelID;
  std::vector<int>   m_jet_truthCount;
  std::vector<float> m_jet_truthPt;
  std::vector<float> m_jet_truthDr_B;
  std::vector<float> m_jet_truthDr_C;
  std::vector<float> m_jet_truthDr_T;
  std::vector<int>   m_jet_partonTruthID;
  std::vector<float> m_jet_ghostTruthAssFrac;
  std::vector<float> m_jet_truth_pt;
  std::vector<float> m_jet_truth_eta;
  std::vector<float> m_jet_truth_phi;
  std::vector<float> m_jet_truth_E;
  std::vector<int> m_jet_truth_pdgId;
  std::vector<float> m_jet_truth_partonPt;
  std::vector<float> m_jet_truth_partonDR;

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
  
  // kinematics
  std::vector<float> m_muon_pt;
  std::vector<float> m_muon_eta;
  std::vector<float> m_muon_phi;
  std::vector<float> m_muon_m;
  
  // track parameters
  std::vector<float> m_muon_trkd0;
  std::vector<float> m_muon_trkd0sig;
  std::vector<float> m_muon_trkz0;
  std::vector<float> m_muon_trkz0sintheta;
  std::vector<float> m_muon_trkphi0;
  std::vector<float> m_muon_trktheta;
  std::vector<float> m_muon_trkcharge;
  std::vector<float> m_muon_trkqOverP;
  
  // track hit content
  std::vector<int> m_muon_trknSiHits;
  std::vector<int> m_muon_trknPixHits;
  std::vector<int> m_muon_trknPixHoles;
  std::vector<int> m_muon_trknSCTHits;
  std::vector<int> m_muon_trknSCTHoles;
  std::vector<int> m_muon_trknTRTHits;
  std::vector<int> m_muon_trknTRTHoles;
  std::vector<int> m_muon_trknBLayerHits; 
  //std::vector<int> m_muon_trknInnermostPixLayHits; // not available ?
  //std::vector<float> m_muon_trkPixdEdX; // not available ?

  // electrons
  int m_nel;
  
  // kinematics
  std::vector<float> m_el_pt;
  std::vector<float> m_el_phi;
  std::vector<float> m_el_eta;
  std::vector<float> m_el_m;
 
  // track parameters
  std::vector<float> m_el_trkd0;
  std::vector<float> m_el_trkd0sig;
  std::vector<float> m_el_trkz0;
  std::vector<float> m_el_trkz0sintheta;
  std::vector<float> m_el_trkphi0;
  std::vector<float> m_el_trktheta;
  std::vector<float> m_el_trkcharge;
  std::vector<float> m_el_trkqOverP;
  
  // track hit content
  std::vector<int> m_el_trknSiHits;
  std::vector<int> m_el_trknPixHits;
  std::vector<int> m_el_trknPixHoles;
  std::vector<int> m_el_trknSCTHits;
  std::vector<int> m_el_trknSCTHoles;
  std::vector<int> m_el_trknTRTHits;
  std::vector<int> m_el_trknTRTHoles;
  std::vector<int> m_el_trknBLayerHits; 
  //std::vector<int> m_el_trknInnermostPixLayHits; // not available ?
  //std::vector<float> m_el_trkPixdEdX; // not available ?

  // taus
  int m_ntau;
  
  // kinematics
  std::vector<float> m_tau_pt;
  std::vector<float> m_tau_eta;
  std::vector<float> m_tau_phi;
  std::vector<float> m_tau_m;
  std::vector<int>   m_tau_ntrk;
  std::vector<float> m_tau_charge;

};

#endif
