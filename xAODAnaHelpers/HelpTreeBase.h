/********************************************************************************
 * HelpTreeBase:
 *
 * This class is meant to help the user write out a tree.
 * Some branches are included by default while others
 * need to be added by the user
 *
 * Gabriel Facini ( gabriel.facini@cern.ch ), Marco Milesi (marco.milesi@cern.ch)
 *
 *
 ********************************************************************************/

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
#include "xAODMissingET/MissingETContainer.h"

#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include "InDetTrackSelectionTool/InDetTrackSelectionTool.h"

// root includes
#include "TTree.h"
#include "TFile.h"

namespace TrigConf {
  class xAODConfigTool;
}

namespace Trig {
  class TrigDecisionTool;
}

class HelpTreeBase {

public:

  HelpTreeBase(xAOD::TEvent *event, TTree* tree, TFile* file, const float units = 1e3, bool debug = false, bool DC14 = false );
  virtual ~HelpTreeBase() {;}

  void AddEvent       (const std::string detailStr = "");
  void AddTrigger     (const std::string detailStr = "");
  void AddJetTrigger  (const std::string detailStr = "");
  void AddMuons       (const std::string detailStr = "");
  void AddElectrons   (const std::string detailStr = "");
  void AddJets        (const std::string detailStr = "");
  void AddFatJets     (const std::string detailStr = "");
  void AddTaus        (const std::string detailStr = "");
  void AddMET         (const std::string detailStr = "");

  xAOD::TEvent* m_event;
  xAOD::TStore* m_store; 

  // control which branches are filled
  HelperClasses::EventInfoSwitch*      m_eventInfoSwitch;
  HelperClasses::TriggerInfoSwitch*    m_trigInfoSwitch;
  HelperClasses::JetTriggerInfoSwitch* m_jetTrigInfoSwitch;
  HelperClasses::MuonInfoSwitch*       m_muInfoSwitch;
  HelperClasses::ElectronInfoSwitch*   m_elInfoSwitch;
  HelperClasses::JetInfoSwitch*        m_jetInfoSwitch;
  HelperClasses::JetInfoSwitch*        m_fatJetInfoSwitch;
  HelperClasses::TauInfoSwitch*        m_tauInfoSwitch;
  HelperClasses::METInfoSwitch*        m_metInfoSwitch;

  InDet::InDetTrackSelectionTool * m_trkSelTool;

  std::string                  m_triggerSelection;
  TrigConf::xAODConfigTool*    m_trigConfTool;
  Trig::TrigDecisionTool*      m_trigDecTool;

  void FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event );
  void FillTrigger( const xAOD::EventInfo* eventInfo );
  void FillJetTrigger();
  void FillMuons( const xAOD::MuonContainer* muons, const xAOD::Vertex* primaryVertex );
  void FillElectrons( const xAOD::ElectronContainer* electrons, const xAOD::Vertex* primaryVertex );
  void FillJets( const xAOD::JetContainer* jets, int pvLocation = -1 );
  void FillFatJets( const xAOD::JetContainer* fatJets );
  void FillTaus( const xAOD::TauJetContainer* taus );
  void FillMET( const xAOD::MissingETContainer* met );

  void Fill();
  void ClearEvent();
  void ClearTrigger();
  void ClearJetTrigger();
  void ClearMuons();
  void ClearElectrons();
  void ClearJets();
  void ClearFatJets();
  void ClearTaus();
  void ClearMET();

  bool writeTo( TFile *file );

  virtual void AddEventUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddEventUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddTriggerUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddTriggerUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddJetTriggerUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddJetTriggerUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddMuonsUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddMuonsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddElectronsUser(const std::string detailStr = "")  {
    if(m_debug) Info("AddElectronsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddJetsUser(const std::string detailStr = "")       {
    if(m_debug) Info("AddJetsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddFatJetsUser(const std::string detailStr = "")       {
    if(m_debug) Info("AddFatJetsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddTausUser(const std::string detailStr = "")       {
    if(m_debug) Info("AddTausUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddMETUser(const std::string detailStr = "")       {
    if(m_debug) Info("AddMETUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  
  virtual void ClearEventUser()     { return; };
  virtual void ClearTriggerUser()   { return; };
  virtual void ClearMuonsUser()     { return; };
  virtual void ClearElectronsUser() { return; };
  virtual void ClearJetsUser() 	    { return; };
  virtual void ClearFatJetsUser()   { return; };
  virtual void ClearTausUser() 	    { return; };
  virtual void ClearMETUser()       { return; };

  virtual void FillEventUser( const xAOD::EventInfo*  )        { return; };
  virtual void FillMuonsUser( const xAOD::Muon*  )             { return; };
  virtual void FillElectronsUser( const xAOD::Electron*  )     { return; };
  virtual void FillJetsUser( const xAOD::Jet*  )               { return; };
  virtual void FillFatJetsUser( const xAOD::Jet*  )            { return; };
  virtual void FillTausUser( const xAOD::TauJet*  )            { return; };
  virtual void FillMETUser( const xAOD::MissingETContainer*  ) { return; };
  virtual void FillTriggerUser( const xAOD::EventInfo*  )      { return; };
  virtual void FillJetTriggerUser()                            { return; };

protected:

  TTree* m_tree;

  int m_units; //For MeV to GeV conversion in output

  bool m_debug;
  bool m_DC14;

  // event
  int m_runNumber;
  int m_eventNumber;
  int m_mcEventNumber;
  int m_mcChannelNumber;
  float m_mcEventWeight;
  float m_weight_pileup;
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

  // CaloCluster
  std::vector<float> m_caloCluster_pt;
  std::vector<float> m_caloCluster_eta;
  std::vector<float> m_caloCluster_phi;
  std::vector<float> m_caloCluster_e;

  // trigger
  int m_passAny;
  int m_passL1;
  int m_passHLT;
  unsigned int m_masterKey;
  unsigned int m_L1PSKey;
  unsigned int m_HLTPSKey;
  std::vector<std::string>  m_elTrigForMatching;   /* each event can have a list of electron trigger chains to which each electron could be matched.
  						   / This list is created when configuring ElectronSelector.cxx, where the electron trigger matching is actually performed
						   */

  std::vector<std::string> m_passTriggers;

  // jet trigger

  // jets
  int m_njet;
  std::vector<float> m_jet_pt;
  std::vector<float> m_jet_eta;
  std::vector<float> m_jet_phi;
  std::vector<float> m_jet_E;

  // rapidity
  std::vector<float> m_jet_rapidity;

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
  std::vector<float> m_jet_LeadingClusterPt;
  std::vector<float> m_jet_LeadingClusterSecondLambda;
  std::vector<float> m_jet_LeadingClusterCenterLambda;
  std::vector<float> m_jet_LeadingClusterSecondR;
  std::vector<int> m_jet_clean_passLooseBad;
  std::vector<int> m_jet_clean_passLooseBadUgly;
  std::vector<int> m_jet_clean_passTightBad;
  std::vector<int> m_jet_clean_passTightBadUgly;

  // energy
  std::vector<float> m_jet_HECf;
  std::vector<float> m_jet_EMf;
  std::vector<float> m_jet_centroidR;
  std::vector<float> m_jet_fracSampMax;
  std::vector<float> m_jet_fracSampMaxIdx;
  std::vector<float> m_jet_lowEtFrac;
  std::vector<float> m_jet_muonSegCount;
  std::vector<float> m_jet_width;

  // scales
  std::vector<float> m_jet_emPt;
  std::vector<float> m_jet_pileupPt;
  std::vector<float> m_jet_originConstitPt;
  std::vector<float> m_jet_etaJESPt;
  std::vector<float> m_jet_gscPt;
  std::vector<float> m_jet_insituPt;

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

  // tracksAll or tracksPV
  std::vector<float> m_jet_Jvt;
  std::vector<float> m_jet_JvtJvfcorr;
  std::vector<float> m_jet_JvtRpt;
  //std::vector<float> m_jet_ghostTrackAssFrac;

  // allTrack
  std::vector<int> m_jet_GhostTrackCount;
  std::vector<float> m_jet_GhostTrackPt;
  std::vector< std::vector<float> > m_jet_GhostTrack_pt;
  std::vector< std::vector<float> > m_jet_GhostTrack_qOverP;
  std::vector< std::vector<float> > m_jet_GhostTrack_eta;
  std::vector< std::vector<float> > m_jet_GhostTrack_phi;
  std::vector< std::vector<float> > m_jet_GhostTrack_e;
  std::vector< std::vector<float> > m_jet_GhostTrack_d0;
  std::vector< std::vector<float> > m_jet_GhostTrack_z0;

  // allTrackDetail
  std::vector< std::vector<int> > m_jet_GhostTrack_nPixHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nSCTHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nTRTHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nPixSharedHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nPixSplitHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nIMLPixHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nIMLPixSharedHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nIMLPixSplitHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nNIMLPixHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nNIMLPixSharedHits;
  std::vector< std::vector<int> > m_jet_GhostTrack_nNIMLPixSplitHits;

  // constituent
  std::vector< int > m_jet_numConstituents;
  std::vector< std::vector<float> > m_jet_constitWeights;
  std::vector< std::vector<float> > m_jet_constit_pt;
  std::vector< std::vector<float> > m_jet_constit_eta;
  std::vector< std::vector<float> > m_jet_constit_phi;
  std::vector< std::vector<float> > m_jet_constit_e;

  // flavor tag
  std::vector<float> m_jet_sv0;
  std::vector<float> m_jet_sv1;
  std::vector<float> m_jet_ip3d;
  std::vector<float> m_jet_sv1ip3d;
  std::vector<float> m_jet_mv1;
  std::vector<float> m_jet_mv2c00;
  std::vector<float> m_jet_mv2c20;

  // area
  std::vector<float> m_jet_ghostArea;
  std::vector<float> m_jet_activeArea;
  std::vector<float> m_jet_voronoiArea;
  std::vector<float> m_jet_activeArea_pt;
  std::vector<float> m_jet_activeArea_eta;
  std::vector<float> m_jet_activeArea_phi;
  std::vector<float> m_jet_activeArea_m;

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

  // fat jets
  int m_nfatjet;

  // kinematics
  std::vector<float> m_fatjet_pt;
  std::vector<float> m_fatjet_eta;
  std::vector<float> m_fatjet_phi;
  std::vector<float> m_fatjet_m;
  std::vector<float> m_fatjet_E;

  // substructure
  std::vector<float> m_fatjet_tau32_wta;

  // muons
  int m_nmuon;

  // kinematics
  std::vector<float> m_muon_pt;
  std::vector<float> m_muon_eta;
  std::vector<float> m_muon_phi;
  std::vector<float> m_muon_m;

  // trigger
  std::vector<int>  m_muon_isTrigMatched;

  // isolation
  std::vector<int>   m_muon_isIsolated;
  std::vector<float> m_muon_ptcone20;
  std::vector<float> m_muon_ptcone30;
  std::vector<float> m_muon_ptcone40;
  std::vector<float> m_muon_ptvarcone20;
  std::vector<float> m_muon_ptvarcone30;
  std::vector<float> m_muon_ptvarcone40;  
  std::vector<float> m_muon_topoetcone20;
  std::vector<float> m_muon_topoetcone30;
  std::vector<float> m_muon_topoetcone40;  

  // quality
  std::vector<int>   m_muon_isVeryLoose;
  std::vector<int>   m_muon_isLoose;
  std::vector<int>   m_muon_isMedium;
  std::vector<int>   m_muon_isTight;

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
  std::vector<int> m_muon_trknInnermostPixLayHits; // not available in DC14
  std::vector<float> m_muon_trkPixdEdX;            // not available in DC14

  // electrons
  int m_nel;

  // kinematics
  std::vector<float> m_el_pt;
  std::vector<float> m_el_phi;
  std::vector<float> m_el_eta;
  std::vector<float> m_el_m;
  
  // trigger
  std::vector<int> m_el_isTrigMatchedToChain;
  std::vector<std::string> m_el_listTrigChains;
  
  // isolation
  std::vector<int>   m_el_isIsolated;
  std::vector<float> m_el_etcone20;
  std::vector<float> m_el_ptcone20;
  std::vector<float> m_el_ptcone30;
  std::vector<float> m_el_ptcone40;
  std::vector<float> m_el_ptvarcone20;
  std::vector<float> m_el_ptvarcone30;
  std::vector<float> m_el_ptvarcone40;  
  std::vector<float> m_el_topoetcone20;
  std::vector<float> m_el_topoetcone30;
  std::vector<float> m_el_topoetcone40;  
  
  // PID
  std::vector<int>   m_el_LHVeryLoose;
  std::vector<int>   m_el_LHLoose;
  std::vector<int>   m_el_LHMedium;
  std::vector<int>   m_el_LHTight;
  std::vector<int>   m_el_IsEMLoose;
  std::vector<int>   m_el_IsEMMedium;
  std::vector<int>   m_el_IsEMTight;

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
  std::vector<int> m_el_trknInnermostPixLayHits; // not available in DC14
  std::vector<float> m_el_trkPixdEdX;            // not available in DC14

  // taus
  int m_ntau;

  // kinematics
  std::vector<float> m_tau_pt;
  std::vector<float> m_tau_eta;
  std::vector<float> m_tau_phi;
  std::vector<float> m_tau_m;
  std::vector<int>   m_tau_ntrk;
  std::vector<float> m_tau_charge;

  // met
  float m_metFinal;
  float m_metFinalPx;
  float m_metFinalPy;
  float m_metFinalPhi; 
  float m_metFinalSumEt;
   
  float m_metEle;       float m_metElePhi;
  float m_metGamma;     float m_metGammaPhi;
  float m_metTau;       float m_metTauPhi;
  float m_metMuons;     float m_metMuonsPhi;
  float m_metSoftCluss; float m_metSoftClussPhi;

};

#endif
