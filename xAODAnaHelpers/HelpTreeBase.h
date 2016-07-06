/********************************************************
 * HelpTreeBase:
 *
 * This class is meant to help the user write out a tree.
 * Some branches are included by default while others
 * need to be added by the user
 *
 * John Alison (john.alison@cern.ch)
 * Gabriel Facini (gabriel.facini@cern.ch)
 * Marco Milesi (marco.milesi@cern.ch)
 * Jeff Dandoy (jeff.dandoy@cern.ch)
 *
 ********************************************************/

// Dear emacs, this is -*-c++-*-
#ifndef xAODAnaHelpers_HelpTreeBase_H
#define xAODAnaHelpers_HelpTreeBase_H

#include "xAODEventShape/EventShape.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODMissingET/MissingETContainer.h"

#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/JetContainer.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"


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

  HelpTreeBase(xAOD::TEvent *event, TTree* tree, TFile* file, const float units = 1e3, bool debug = false, bool DC14 = false, xAOD::TStore* store = nullptr );
  HelpTreeBase(TTree* tree, TFile* file, xAOD::TEvent *event = nullptr, xAOD::TStore* store = nullptr, const float units = 1e3, bool debug = false, bool DC14 = false );
  virtual ~HelpTreeBase();

  void AddEvent       (const std::string detailStr = "");
  void AddTrigger     (const std::string detailStr = "");
  void AddJetTrigger  (const std::string detailStr = "");
  void AddMuons       (const std::string detailStr = "");
  void AddElectrons   (const std::string detailStr = "");
  void AddPhotons     (const std::string detailStr = "");
  void AddJets        (const std::string detailStr = "", const std::string jetName = "jet");
  void AddTruthParts  (const std::string truthName,      const std::string detailStr = "");
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
  HelperClasses::PhotonInfoSwitch*     m_phInfoSwitch;
  HelperClasses::TruthInfoSwitch*      m_truthInfoSwitch;
  HelperClasses::JetInfoSwitch*        m_fatJetInfoSwitch;
  HelperClasses::TauInfoSwitch*        m_tauInfoSwitch;
  HelperClasses::METInfoSwitch*        m_metInfoSwitch;


  std::string                  m_triggerSelection;
  TrigConf::xAODConfigTool*    m_trigConfTool;
  Trig::TrigDecisionTool*      m_trigDecTool;

  void FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event = nullptr );

  void FillTrigger( const xAOD::EventInfo* eventInfo );
  void FillJetTrigger();
  void FillMuons( const xAOD::MuonContainer* muons, const xAOD::Vertex* primaryVertex );
  void FillElectrons( const xAOD::ElectronContainer* electrons, const xAOD::Vertex* primaryVertex );
  void FillPhotons( const xAOD::PhotonContainer* photons );

  void FillJets( const xAOD::JetContainer* jets, int pvLocation = -1, const std::string jetName = "jet" );
  void FillJet( const xAOD::Jet* jet_itr, const xAOD::Vertex* pv, int pvLocation, const std::string jetName = "jet" );

  void FillTruth( const std::string truthName, const xAOD::TruthParticleContainer* truth);
  void FillFatJets( const xAOD::JetContainer* fatJets );
  void FillTaus( const xAOD::TauJetContainer* taus );
  void FillMET( const xAOD::MissingETContainer* met );

  void Fill();
  void ClearEvent();
  void ClearTrigger();
  void ClearJetTrigger();
  void ClearMuons();
  void ClearElectrons();
  void ClearPhotons();
  void ClearJets(const std::string jetName = "jet");
  void ClearTruth(const std::string truthName);
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
  virtual void AddPhotonsUser(const std::string detailStr = "")  {
    if(m_debug) Info("AddPhotonsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };
  virtual void AddJetsUser(const std::string detailStr = "", const std::string jetName = "jet")       {
    if(m_debug) Info("AddJetsUser","Empty function called from HelpTreeBase %s %s",detailStr.c_str(), jetName.c_str());
    return;
  };
  virtual void AddTruthUser(const std::string truthName, const std::string detailStr = "")       {
    if(m_debug) Info("AddTruthUser","Empty function called from HelpTreeBase %s %s",truthName.c_str(), detailStr.c_str());
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
  virtual void ClearPhotonsUser() { return; };
  virtual void ClearTruthUser(const std::string& /*truthName*/) 	    { return; };
  virtual void ClearJetsUser (const std::string /*jetName = "jet"*/ ) 	    { return; };
  virtual void ClearFatJetsUser()   { return; };
  virtual void ClearTausUser() 	    { return; };
  virtual void ClearMETUser()       { return; };

  virtual void FillEventUser( const xAOD::EventInfo*  )        { return; };
  virtual void FillMuonsUser( const xAOD::Muon*  )             { return; };
  virtual void FillElectronsUser( const xAOD::Electron*  )     { return; };
  virtual void FillPhotonsUser( const xAOD::Photon*  )     { return; };
  virtual void FillJetsUser( const xAOD::Jet*, const std::string /*jetName = "jet"*/  )               { return; };
  virtual void FillTruthUser( const std::string& /*truthName*/, const xAOD::TruthParticle*  )               { return; };
  virtual void FillFatJetsUser( const xAOD::Jet*  )            { return; };
  virtual void FillTausUser( const xAOD::TauJet*  )            { return; };
  virtual void FillMETUser( const xAOD::MissingETContainer*  ) { return; };
  virtual void FillTriggerUser( const xAOD::EventInfo*  )      { return; };
  virtual void FillJetTriggerUser()                            { return; };

 protected:

  template<typename T, typename U, typename V>
    void safeFill(const V* xAODObj, SG::AuxElement::ConstAccessor<T>& accessor, std::vector<U>& destination, U defaultValue, int m_units = 1);

  template<typename T, typename U, typename V>
    void safeVecFill(const V* xAODObj, SG::AuxElement::ConstAccessor<std::vector<T> >& accessor, std::vector<std::vector<U> >& destination, int m_units = 1);

  template<typename T>
    void setBranch(std::string prefix, std::string varName, std::vector<T>* localVectorPtr);

protected:

  TTree* m_tree;

  int m_units; //For MeV to GeV conversion in output

  bool m_debug;
  bool m_DC14;
  bool m_isMC;

  // event
  int m_runNumber;
  long int m_eventNumber;
  int m_lumiBlock;
  uint32_t m_coreFlags;
  uint32_t m_timeStamp;
  uint32_t m_timeStampNSOffset;
  bool m_TileError;
  bool m_LArError;
  bool m_SCTError;
  uint32_t m_TileFlags;
  uint32_t m_LArFlags;
  uint32_t m_SCTFlags;
  int m_mcEventNumber;
  int m_mcChannelNumber;
  float m_mcEventWeight;
  float m_weight_pileup;
  float m_correct_mu;
  int   m_rand_run_nr;
  int   m_rand_lumiblock_nr;
  int m_bcid;
  // event pileup
  int m_npv;
  float m_actualMu;
  float m_averageMu;
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
  int m_passL1;
  int m_passHLT;
  unsigned int m_masterKey;
  unsigned int m_L1PSKey;
  unsigned int m_HLTPSKey;
  std::vector<std::string>  m_elTrigForMatching;   /* each event can have a list of electron trigger chains to which each electron could be matched.
  						   / This list is created when configuring ElectronSelector.cxx, where the electron trigger matching is actually performed
						   */

  std::vector<std::string> m_passTriggers;
  std::vector<float> m_triggerPrescales;

  // jet trigger

  //
  // jet Info
  //
  struct jetInfo{

    int N;
    std::vector<float> m_jet_pt;
    std::vector<float> m_jet_eta;
    std::vector<float> m_jet_phi;
    std::vector<float> m_jet_E;

    // rapidity
    std::vector<float> m_jet_rapidity;


    jetInfo(){ }

  };
  std::map<std::string, xAH::JetContainer*> m_jets;

  //
  // Truth
  //
  struct truthInfo{

    int  N;
    std::vector<float> pt;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<float> E;

    truthInfo(){ }

  };
  std::map<std::string, truthInfo*> m_truth;

  // fat jets
  int m_nfatjet;

  // kinematics
  std::vector<float> m_fatjet_pt;
  std::vector<float> m_fatjet_eta;
  std::vector<float> m_fatjet_phi;
  std::vector<float> m_fatjet_m;
  std::vector<float> m_fatjet_E;

  // substructure
  std::vector<float> m_fatjet_Split12;
  std::vector<float> m_fatjet_Split23;
  std::vector<float> m_fatjet_Split34;
  std::vector<float> m_fatjet_tau1_wta;
  std::vector<float> m_fatjet_tau2_wta;
  std::vector<float> m_fatjet_tau3_wta;
  std::vector<float> m_fatjet_tau21_wta;
  std::vector<float> m_fatjet_tau32_wta;
  std::vector<float> m_fatjet_ECF1;
  std::vector<float> m_fatjet_ECF2;
  std::vector<float> m_fatjet_ECF3;
  std::vector<float> m_fatjet_C2;
  std::vector<float> m_fatjet_D2;
  std::vector<float> m_fatjet_NTrimSubjets;

  // muons
  int m_nmuon;

  // kinematics
  std::vector<float> m_muon_pt;
  std::vector<float> m_muon_eta;
  std::vector<float> m_muon_phi;
  std::vector<float> m_muon_m;

  // trigger
  std::vector<int> m_muon_isTrigMatched;
  std::vector<std::vector<int> > m_muon_isTrigMatchedToChain;
  std::vector<std::string> m_muon_listTrigChains;

  // isolation
  std::vector<int>   m_muon_isIsolated_LooseTrackOnly;
  std::vector<int>   m_muon_isIsolated_Loose;
  std::vector<int>   m_muon_isIsolated_Tight;
  std::vector<int>   m_muon_isIsolated_Gradient;
  std::vector<int>   m_muon_isIsolated_GradientLoose;
  std::vector<int>   m_muon_isIsolated_FixedCutLoose;
  std::vector<int>   m_muon_isIsolated_FixedCutTightTrackOnly;
  std::vector<int>   m_muon_isIsolated_UserDefinedFixEfficiency;
  std::vector<int>   m_muon_isIsolated_UserDefinedCut;
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

  // scale factors w/ sys
  // per object
  std::vector< std::vector< float > > m_muon_RecoEff_SF_Loose;
  std::vector< std::vector< float > > m_muon_TrigEff_SF_Loose_Loose;
  std::vector< std::vector< float > > m_muon_TrigEff_SF_Loose_FixedCutTightTrackOnly;
  std::vector< std::vector< float > > m_muon_TrigMCEff_Loose_Loose;
  std::vector< std::vector< float > > m_muon_TrigMCEff_Loose_FixedCutTightTrackOnly;
  std::vector< std::vector< float > > m_muon_IsoEff_SF_LooseTrackOnly;
  std::vector< std::vector< float > > m_muon_IsoEff_SF_Loose;
  std::vector< std::vector< float > > m_muon_IsoEff_SF_Tight;
  std::vector< std::vector< float > > m_muon_IsoEff_SF_Gradient;
  std::vector< std::vector< float > > m_muon_IsoEff_SF_GradientLoose;
  std::vector< std::vector< float > > m_muon_IsoEff_SF_FixedCutLoose;
  std::vector< std::vector< float > > m_muon_IsoEff_SF_FixedCutTightTrackOnly;
  std::vector< std::vector< float > > m_muon_TTVAEff_SF;

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

  std::vector<float>         m_muon_EnergyLoss;
  std::vector<float>         m_muon_EnergyLossSigma;
  std::vector<unsigned char> m_muon_energyLossType;
  std::vector<float>         m_muon_MeasEnergyLoss;
  std::vector<float>         m_muon_MeasEnergyLossSigma;
  std::vector<float>         m_muon_ParamEnergyLoss;
  std::vector<float>         m_muon_ParamEnergyLossSigmaMinus;
  std::vector<float>         m_muon_ParamEnergyLossSigmaPlus;

  //
  // electrons
  int m_nel;

  // kinematics
  std::vector<float> m_el_pt;
  std::vector<float> m_el_phi;
  std::vector<float> m_el_eta;
  std::vector<float> m_el_m;
  std::vector<float> m_el_caloCluster_eta;

  // trigger
  std::vector<int> m_el_isTrigMatched;
  std::vector<std::vector<int> > m_el_isTrigMatchedToChain;
  std::vector<std::string>       m_el_listTrigChains;

  // isolation
  std::vector<int>   m_el_isIsolated_LooseTrackOnly;
  std::vector<int>   m_el_isIsolated_Loose;
  std::vector<int>   m_el_isIsolated_Tight;
  std::vector<int>   m_el_isIsolated_Gradient;
  std::vector<int>   m_el_isIsolated_GradientLoose;
  std::vector<int>   m_el_isIsolated_FixedCutLoose;
  std::vector<int>   m_el_isIsolated_FixedCutTight;
  std::vector<int>   m_el_isIsolated_FixedCutTightTrackOnly;
  std::vector<int>   m_el_isIsolated_UserDefinedFixEfficiency;
  std::vector<int>   m_el_isIsolated_UserDefinedCut;
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
  int m_nel_LHLoose;
  std::vector<int>   m_el_LHLoose;
  int m_nel_LHMedium;
  std::vector<int>   m_el_LHMedium;
  int m_nel_LHTight;
  std::vector<int>   m_el_LHTight;
  int m_nel_IsEMLoose;
  std::vector<int>   m_el_IsEMLoose;
  int m_nel_IsEMMedium;
  std::vector<int>   m_el_IsEMMedium;
  int m_nel_IsEMTight;
  std::vector<int>   m_el_IsEMTight;

  // scale factors w/ sys
  // per object
  std::vector< std::vector< float > > m_el_RecoEff_SF;
  std::vector< std::vector< float > > m_el_TrigEff_SF_LHLooseAndBLayer;
  std::vector< std::vector< float > > m_el_TrigEff_SF_LHTight;
  std::vector< std::vector< float > > m_el_TrigMCEff_LHLooseAndBLayer;
  std::vector< std::vector< float > > m_el_TrigMCEff_LHTight;
  std::vector< std::vector< float > > m_el_IsoEff_SF_Loose;
  std::vector< std::vector< float > > m_el_IsoEff_SF_FixedCutTight;
  std::vector< std::vector< float > > m_el_PIDEff_SF_LHLooseAndBLayer;
  std::vector< std::vector< float > > m_el_PIDEff_SF_LHLoose;
  std::vector< std::vector< float > > m_el_PIDEff_SF_LHMedium;
  std::vector< std::vector< float > > m_el_PIDEff_SF_LHTight;

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

  //
  // photons
  int m_nph;

  // kinematics
  std::vector<float> m_ph_pt;
  std::vector<float> m_ph_phi;
  std::vector<float> m_ph_eta;
  std::vector<float> m_ph_m;
  std::vector<float> m_ph_E;

  // isolation
  std::vector<int>   m_ph_isIsolated_Cone40CaloOnly;
  std::vector<int>   m_ph_isIsolated_Cone40;
  std::vector<int>   m_ph_isIsolated_Cone20;
  //std::vector<float> m_ph_etcone20;
  std::vector<float> m_ph_ptcone20;
  std::vector<float> m_ph_ptcone30;
  std::vector<float> m_ph_ptcone40;
  std::vector<float> m_ph_ptvarcone20;
  std::vector<float> m_ph_ptvarcone30;
  std::vector<float> m_ph_ptvarcone40;
  std::vector<float> m_ph_topoetcone20;
  std::vector<float> m_ph_topoetcone30;
  std::vector<float> m_ph_topoetcone40;

  // PID
  int m_nph_IsLoose;
  std::vector<int>   m_ph_IsLoose;
  int m_nph_IsMedium;
  std::vector<int>   m_ph_IsMedium;
  int m_nph_IsTight;
  std::vector<int>   m_ph_IsTight;

  //
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
  float m_metFinalClus;
  float m_metFinalClusPx;
  float m_metFinalClusPy;
  float m_metFinalClusPhi;
  float m_metFinalClusSumEt;

  float m_metFinalTrk;
  float m_metFinalTrkPx;
  float m_metFinalTrkPy;
  float m_metFinalTrkPhi;
  float m_metFinalTrkSumEt;

  float m_metEle;       float m_metEleSumEt;      float m_metElePhi;
  float m_metGamma;     float m_metGammaSumEt;    float m_metGammaPhi;
  float m_metTau;       float m_metTauSumEt;      float m_metTauPhi;
  float m_metMuons;     float m_metMuonsSumEt;    float m_metMuonsPhi;
  float m_metJet;       float m_metJetSumEt;      float m_metJetPhi;
  float m_metJetTrk;    float m_metJetTrkSumEt;   float m_metJetTrkPhi;
  float m_metSoftClus;  float m_metSoftClusSumEt; float m_metSoftClusPhi;
  float m_metSoftTrk;   float m_metSoftTrkSumEt;  float m_metSoftTrkPhi;

 public:

  //void Fill_Fix30( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Fix50( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Fix60( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Fix70( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Fix77( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Fix80( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Fix85( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Fix90( const xAOD::Jet* jet, jetInfo* thisJet );
  //
  //void Fill_Flt30( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Flt40( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Flt50( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Flt60( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Flt70( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Flt77( const xAOD::Jet* jet, jetInfo* thisJet );
  //void Fill_Flt85( const xAOD::Jet* jet, jetInfo* thisJet );


};


template<typename T, typename U, typename V>
void HelpTreeBase::safeFill(const V* xAODObj, SG::AuxElement::ConstAccessor<T>& accessor, std::vector<U>& destination, U defaultValue, int m_units){
  if ( accessor.isAvailable( *xAODObj ) ) {
    destination.push_back( accessor( *xAODObj ) / m_units );
  } else {
    destination.push_back( defaultValue );
  }
  return;
}

template<typename T, typename U, typename V>
void HelpTreeBase::safeVecFill(const V* xAODObj, SG::AuxElement::ConstAccessor<std::vector<T> >& accessor, std::vector<std::vector<U> >& destination, int m_units){
  destination.push_back( std::vector<U>() );

  if ( accessor.isAvailable( *xAODObj ) ) {
    for(U itemInVec : accessor(*xAODObj))        destination.back().push_back(itemInVec / m_units);
  } 
  return;
}


template<typename T>
void HelpTreeBase::setBranch(std::string prefix, std::string varName, std::vector<T>* localVectorPtr){
  m_tree->Branch((prefix+"_"+varName).c_str(),        localVectorPtr);
}


#endif

