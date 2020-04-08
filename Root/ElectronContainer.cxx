#include "xAODAnaHelpers/ElectronContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

ElectronContainer::ElectronContainer(const std::string& name, const std::string& detailStr, float units, bool mc, bool storeSystSFs)
  : ParticleContainer(name, detailStr, units, mc, true, storeSystSFs)
{

  if ( m_infoSwitch.m_kinematic ) {
    m_caloCluster_eta = new std::vector<float> ();
    m_charge          = new std::vector<float> ();
  }

  if ( m_infoSwitch.m_trigger ){
    m_isTrigMatched               = new std::vector<int>               ();
    m_isTrigMatchedToChain        = new std::vector<std::vector<int> > ();
    m_listTrigChains              = new std::vector<std::string>       ();
  }

  if ( m_infoSwitch.m_isolation ) {
    m_isIsolated = new std::map< std::string, std::vector< int > >();
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    m_etcone20                               = new std::vector<float> ();
    m_ptcone20                               = new std::vector<float> ();
    m_ptcone30                               = new std::vector<float> ();
    m_ptcone40                               = new std::vector<float> ();
    m_ptvarcone20                            = new std::vector<float> ();
    m_ptvarcone30                            = new std::vector<float> ();
    m_ptvarcone40                            = new std::vector<float> ();
    m_topoetcone20                           = new std::vector<float> ();
    m_topoetcone30                           = new std::vector<float> ();
    m_topoetcone40                           = new std::vector<float> ();
  }

  if ( m_infoSwitch.m_PID ) {
    m_PID = new std::map< std::string, std::vector< int > >();
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    m_TrigEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
    m_TrigMCEff  = new std::map< std::string, std::vector< std::vector< float > > >();
    m_PIDEff_SF  = new std::map< std::string, std::vector< std::vector< float > > >();
    m_IsoEff_SF  = new std::map< std::string, std::vector< std::vector< float > > >();

    m_RecoEff_SF = new std::vector< std::vector< float > > ();
  }

  if ( m_infoSwitch.m_recoparams ) {
    m_author = new std::vector<int> ();
    m_OQ     = new std::vector<int> ();
  }

  if ( m_infoSwitch.m_trackparams ) {
    m_trkd0           = new std::vector<float> ();
    m_trkd0sig        = new std::vector<float> ();
    m_trkz0           = new std::vector<float> ();
    m_trkz0sintheta   = new std::vector<float> ();
    m_trkphi0         = new std::vector<float> ();
    m_trktheta        = new std::vector<float> ();
    m_trkcharge       = new std::vector<float> ();
    m_trkqOverP       = new std::vector<float> ();
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    m_trknSiHits                 = new std::vector<int>   ();
    m_trknPixHits                = new std::vector<int>   ();
    m_trknPixHoles               = new std::vector<int>   ();
    m_trknSCTHits                = new std::vector<int>   ();
    m_trknSCTHoles               = new std::vector<int>   ();
    m_trknTRTHits                = new std::vector<int>   ();
    m_trknTRTHoles               = new std::vector<int>   ();
    m_trknBLayerHits             = new std::vector<int>   ();
    m_trknInnermostPixLayHits    = new std::vector<int>   ();
    m_trkPixdEdX                 = new std::vector<float> ();
  }

  if ( m_infoSwitch.m_promptlepton ) {
    m_PromptLeptonInput_DL1mu           = new std::vector<float> ();
    m_PromptLeptonInput_DRlj            = new std::vector<float> ();
    m_PromptLeptonInput_LepJetPtFrac    = new std::vector<float> ();
    m_PromptLeptonInput_PtFrac          = new std::vector<float> ();
    m_PromptLeptonInput_PtRel           = new std::vector<float> ();
    m_PromptLeptonInput_TrackJetNTrack  = new std::vector<int>   ();
    m_PromptLeptonInput_ip2             = new std::vector<float> ();
    m_PromptLeptonInput_ip3             = new std::vector<float> ();
    m_PromptLeptonInput_rnnip           = new std::vector<float> ();
    m_PromptLeptonInput_sv1_jf_ntrkv    = new std::vector<int>   ();
    m_PromptLeptonIso                   = new std::vector<float> ();
    m_PromptLeptonVeto                  = new std::vector<float> ();
  }

}

ElectronContainer::~ElectronContainer()
{
  if ( m_infoSwitch.m_kinematic ) {
    delete m_caloCluster_eta;
    delete m_charge;
  }

  if ( m_infoSwitch.m_trigger ){
    delete m_isTrigMatched       ;
    delete m_isTrigMatchedToChain;
    delete m_listTrigChains      ;
  }

  if ( m_infoSwitch.m_isolation ) {
    delete m_isIsolated;
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    delete m_etcone20                               ;
    delete m_ptcone20                               ;
    delete m_ptcone30                               ;
    delete m_ptcone40                               ;
    delete m_ptvarcone20                            ;
    delete m_ptvarcone30                            ;
    delete m_ptvarcone40                            ;
    delete m_topoetcone20                           ;
    delete m_topoetcone30                           ;
    delete m_topoetcone40                           ;
  }

  if ( m_infoSwitch.m_PID ) {
    delete m_PID;
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    delete m_TrigEff_SF ;
    delete m_TrigMCEff  ;
    delete m_PIDEff_SF  ;
    delete m_IsoEff_SF  ;
    delete m_RecoEff_SF ;
  }

  if ( m_infoSwitch.m_recoparams ) {
    delete m_author   ;
    delete m_OQ       ;
  }

  if ( m_infoSwitch.m_trackparams ) {
    delete m_trkd0          ;
    delete m_trkd0sig       ;
    delete m_trkz0          ;
    delete m_trkz0sintheta  ;
    delete m_trkphi0        ;
    delete m_trktheta       ;
    delete m_trkcharge      ;
    delete m_trkqOverP      ;
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    delete m_trknSiHits              ;
    delete m_trknPixHits             ;
    delete m_trknPixHoles            ;
    delete m_trknSCTHits             ;
    delete m_trknSCTHoles            ;
    delete m_trknTRTHits             ;
    delete m_trknTRTHoles            ;
    delete m_trknBLayerHits          ;
    delete m_trknInnermostPixLayHits ;
    delete m_trkPixdEdX              ;
  }

  if ( m_infoSwitch.m_promptlepton ) {
    delete m_PromptLeptonInput_DL1mu           ;
    delete m_PromptLeptonInput_DRlj            ;
    delete m_PromptLeptonInput_LepJetPtFrac    ;
    delete m_PromptLeptonInput_PtFrac          ;
    delete m_PromptLeptonInput_PtRel           ;
    delete m_PromptLeptonInput_TrackJetNTrack  ;
    delete m_PromptLeptonInput_ip2             ;
    delete m_PromptLeptonInput_ip3             ;
    delete m_PromptLeptonInput_rnnip           ;
    delete m_PromptLeptonInput_sv1_jf_ntrkv    ;
    delete m_PromptLeptonIso                   ;
    delete m_PromptLeptonVeto                  ;
  }

}

void ElectronContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if ( m_infoSwitch.m_kinematic ) {
    connectBranch<float>(tree,"caloCluster_eta", &m_caloCluster_eta);
    connectBranch<float>(tree,"charge",          &m_charge);
  }

  if ( m_infoSwitch.m_trigger ){
    connectBranch<int>         (tree,"isTrigMatched",        &m_isTrigMatched);
    connectBranch<vector<int> >(tree,"isTrigMatchedToChain", &m_isTrigMatchedToChain);
    connectBranch<std::string> (tree,"listTrigChains",       &m_listTrigChains);
  }

  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty() && isol != "NONE") {
        tree->SetBranchStatus ( (m_name + "_isIsolated_" + isol).c_str() , 1);
        tree->SetBranchAddress( (m_name + "_isIsolated_" + isol).c_str() , & (*m_isIsolated)[ isol ] );
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    connectBranch<float>(tree, "etcone20",         &m_etcone20);
    connectBranch<float>(tree, "ptcone20",         &m_ptcone20);
    connectBranch<float>(tree, "ptcone30",         &m_ptcone30);
    connectBranch<float>(tree, "ptcone40",         &m_ptcone40);
    connectBranch<float>(tree, "ptvarcone20",      &m_ptvarcone20);
    connectBranch<float>(tree, "ptvarcone30",      &m_ptvarcone30);
    connectBranch<float>(tree, "ptvarcone40",      &m_ptvarcone40);
    connectBranch<float>(tree, "topoetcone20",     &m_topoetcone20);
    connectBranch<float>(tree, "topoetcone30",     &m_topoetcone30);
    connectBranch<float>(tree, "topoetcone40",     &m_topoetcone40);
  }

  if ( m_infoSwitch.m_PID ) {
    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      if (!PID.empty()) {
        tree->SetBranchStatus ( (m_name + PID).c_str() , 1);
        tree->SetBranchAddress( (m_name + PID).c_str() , & (*m_PID)[ PID ] );
      }
    }
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    for (auto& PID : m_infoSwitch.m_PIDSFWPs) {
      tree->SetBranchStatus ( (m_name+"_PIDEff_SF_" + PID).c_str() , 1);
      tree->SetBranchAddress( (m_name+"_PIDEff_SF_" + PID).c_str() , & (*m_PIDEff_SF)[ PID ] );

      for (auto& isol : m_infoSwitch.m_isolWPs) {
        if(!isol.empty()) {
          tree->SetBranchStatus ( (m_name+"_IsoEff_SF_" + PID + "_isol" + isol).c_str() , 1);
          tree->SetBranchAddress( (m_name+"_IsoEff_SF_" + PID + "_isol" + isol).c_str() , & (*m_IsoEff_SF)[ PID+isol ] );
        }
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          tree->SetBranchStatus ( (m_name+"_TrigEff_SF_" + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "")).c_str() , 1 );
          tree->SetBranchAddress( (m_name+"_TrigEff_SF_" + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "")).c_str() , & (*m_TrigEff_SF)[ trig+PID+isol ] );

          tree->SetBranchStatus ( (m_name+"_TrigMCEff_"  + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "")).c_str() , 1 );
          tree->SetBranchAddress( (m_name+"_TrigMCEff_"  + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "")).c_str() , & (*m_TrigMCEff) [ trig+PID+isol ] );
        }
      }
    }

    connectBranch<std::vector<float> >(tree, "RecoEff_SF"  ,                &m_RecoEff_SF  );
  }

  if ( m_infoSwitch.m_recoparams ) {
    connectBranch<int>(tree, "author",   &m_author);
    connectBranch<int>(tree, "OQ",       &m_OQ);
  }

  if ( m_infoSwitch.m_trackparams ) {
    connectBranch<float>(tree, "trkd0",          &m_trkd0);
    connectBranch<float>(tree, "trkd0sig",       &m_trkd0sig);
    connectBranch<float>(tree, "trkz0",          &m_trkz0);
    connectBranch<float>(tree, "trkz0sintheta",  &m_trkz0sintheta);
    connectBranch<float>(tree, "trkphi0",        &m_trkphi0);
    connectBranch<float>(tree, "trktheta",       &m_trktheta);
    connectBranch<float>(tree, "trkcharge",      &m_trkcharge);
    connectBranch<float>(tree, "trkqOverP",      &m_trkqOverP);
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    connectBranch<int>(tree, "trknSiHits",    &m_trknSiHits);
    connectBranch<int>(tree, "trknPixHits",   &m_trknPixHits);
    connectBranch<int>(tree, "trknPixHoles",  &m_trknPixHoles);
    connectBranch<int>(tree, "trknSCTHits",   &m_trknSCTHits);
    connectBranch<int>(tree, "trknSCTHoles",  &m_trknSCTHoles);
    connectBranch<int>(tree, "trknTRTHits",   &m_trknTRTHits);
    connectBranch<int>(tree, "trknTRTHoles",  &m_trknTRTHoles);
    connectBranch<int>(tree, "trknBLayerHits",&m_trknBLayerHits);
    connectBranch<int>(tree, "trknInnermostPixLayHits",  &m_trknInnermostPixLayHits);
    connectBranch<float>(tree, "trkPixdEdX",    &m_trkPixdEdX);
  }

  if ( m_infoSwitch.m_promptlepton ) {
    connectBranch<float>(tree, "PromptLeptonInput_DL1mu",          &m_PromptLeptonInput_DL1mu);
    connectBranch<float>(tree, "PromptLeptonInput_DRlj",           &m_PromptLeptonInput_DRlj);
    connectBranch<float>(tree, "PromptLeptonInput_LepJetPtFrac",   &m_PromptLeptonInput_LepJetPtFrac);
    connectBranch<float>(tree, "PromptLeptonInput_PtFrac",         &m_PromptLeptonInput_PtFrac);
    connectBranch<float>(tree, "PromptLeptonInput_PtRel",          &m_PromptLeptonInput_PtRel);
    connectBranch<int>  (tree, "PromptLeptonInput_TrackJetNTrack", &m_PromptLeptonInput_TrackJetNTrack);
    connectBranch<float>(tree, "PromptLeptonInput_ip2",            &m_PromptLeptonInput_ip2);
    connectBranch<float>(tree, "PromptLeptonInput_ip3",            &m_PromptLeptonInput_ip3);
    connectBranch<float>(tree, "PromptLeptonInput_rnnip",          &m_PromptLeptonInput_rnnip);
    connectBranch<int>  (tree, "PromptLeptonInput_sv1_jf_ntrkv",   &m_PromptLeptonInput_sv1_jf_ntrkv);
    connectBranch<float>(tree, "PromptLeptonIso",                  &m_PromptLeptonIso);
    connectBranch<float>(tree, "PromptLeptonVeto",                 &m_PromptLeptonVeto);
  }

}

void ElectronContainer::updateParticle(uint idx, Electron& elec)
{
  ParticleContainer::updateParticle(idx,elec);

  if ( m_infoSwitch.m_kinematic ) {
    elec.caloCluster_eta = m_caloCluster_eta -> at(idx);
    elec.charge          = m_charge          -> at(idx);
  }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    elec.isTrigMatched         =     m_isTrigMatched         ->at(idx);
    elec.isTrigMatchedToChain  =     m_isTrigMatchedToChain  ->at(idx);
    elec.listTrigChains        =     m_listTrigChains        ->at(idx);
  }

  // isolation
  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty() && isol != "NONE") {
        elec.isIsolated[isol] = (*m_isIsolated)[ isol ].at(idx);
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    elec.etcone20                                 =     m_etcone20                                   ->at(idx);
    elec.ptcone20                                 =     m_ptcone20                                   ->at(idx);
    elec.ptcone30                                 =     m_ptcone30                                   ->at(idx);
    elec.ptcone40                                 =     m_ptcone40                                   ->at(idx);
    elec.ptvarcone20                              =     m_ptvarcone20                                ->at(idx);
    elec.ptvarcone30                              =     m_ptvarcone30                                ->at(idx);
    elec.ptvarcone40                              =     m_ptvarcone40                                ->at(idx);
    elec.topoetcone20                             =     m_topoetcone20                               ->at(idx);
    elec.topoetcone30                             =     m_topoetcone30                               ->at(idx);
    elec.topoetcone40                             =     m_topoetcone40                               ->at(idx);
  }

  // quality
  if ( m_infoSwitch.m_PID ) {
    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      if (!PID.empty()) {
        elec.PID[PID] = (*m_PID)[ PID ].at(idx);
      }
    }
  }

  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {

    for (auto& PID : m_infoSwitch.m_PIDSFWPs) {
      elec.PIDEff_SF[ PID ] = (*m_PIDEff_SF) [ PID ].at(idx);
      for (auto& iso : m_infoSwitch.m_isolWPs) {
        if(!iso.empty())
          elec.IsoEff_SF[ PID+iso ] =  (*m_IsoEff_SF) [ PID+iso ].at(idx);
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          elec.TrigEff_SF[ trig+PID+iso ] = (*m_TrigEff_SF)[ trig+PID+iso ].at(idx);
          elec.TrigMCEff [ trig+PID+iso ] = (*m_TrigMCEff )[ trig+PID+iso ].at(idx);
        }
      }
    }

    elec.RecoEff_SF                               = m_RecoEff_SF                              ->at(idx);

  }

  // reco parameters
  if ( m_infoSwitch.m_recoparams ) {
    elec.author      = m_author     ->at(idx);
    elec.OQ          = m_OQ         ->at(idx);
  }

  // track parameters
  if ( m_infoSwitch.m_trackparams ) {
    elec.trkd0             = m_trkd0            ->at(idx);
    elec.trkd0sig          = m_trkd0sig         ->at(idx);
    elec.trkz0             = m_trkz0            ->at(idx);
    elec.trkz0sintheta     = m_trkz0sintheta    ->at(idx);
    elec.trkphi0           = m_trkphi0          ->at(idx);
    elec.trktheta          = m_trktheta         ->at(idx);
    elec.trkcharge         = m_trkcharge        ->at(idx);
    elec.trkqOverP         = m_trkqOverP        ->at(idx);
  }

  // track hit content
  if ( m_infoSwitch.m_trackhitcont ) {
    elec.trknSiHits                 = m_trknSiHits                ->at(idx);
    elec.trknPixHits                = m_trknPixHits               ->at(idx);
    elec.trknPixHoles               = m_trknPixHoles              ->at(idx);
    elec.trknSCTHits                = m_trknSCTHits               ->at(idx);
    elec.trknSCTHoles               = m_trknSCTHoles              ->at(idx);
    elec.trknTRTHits                = m_trknTRTHits               ->at(idx);
    elec.trknTRTHoles               = m_trknTRTHoles              ->at(idx);
    elec.trknBLayerHits             = m_trknBLayerHits            ->at(idx);
    elec.trknInnermostPixLayHits    = m_trknInnermostPixLayHits   ->at(idx);         // not available in DC14
    elec.trkPixdEdX                 = m_trkPixdEdX                ->at(idx);         // not available in DC14
  }

  // prompt lepton
  if ( m_infoSwitch.m_promptlepton ) {
    elec.PromptLeptonInput_DL1mu           = m_PromptLeptonInput_DL1mu           ->at(idx);
    elec.PromptLeptonInput_DRlj            = m_PromptLeptonInput_DRlj            ->at(idx);
    elec.PromptLeptonInput_LepJetPtFrac    = m_PromptLeptonInput_LepJetPtFrac    ->at(idx);
    elec.PromptLeptonInput_PtFrac          = m_PromptLeptonInput_PtFrac          ->at(idx);
    elec.PromptLeptonInput_PtRel           = m_PromptLeptonInput_PtRel           ->at(idx);
    elec.PromptLeptonInput_TrackJetNTrack  = m_PromptLeptonInput_TrackJetNTrack  ->at(idx);
    elec.PromptLeptonInput_ip2             = m_PromptLeptonInput_ip2             ->at(idx);
    elec.PromptLeptonInput_ip3             = m_PromptLeptonInput_ip3             ->at(idx);
    elec.PromptLeptonInput_rnnip           = m_PromptLeptonInput_rnnip           ->at(idx);
    elec.PromptLeptonInput_sv1_jf_ntrkv    = m_PromptLeptonInput_sv1_jf_ntrkv    ->at(idx);
    elec.PromptLeptonIso                   = m_PromptLeptonIso                   ->at(idx);
    elec.PromptLeptonVeto                  = m_PromptLeptonVeto                  ->at(idx);
  }

}


void ElectronContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_kinematic ) {
    setBranch<float>(tree,"caloCluster_eta", m_caloCluster_eta);
    setBranch<float>(tree,"charge",          m_charge);
  }

  if ( m_infoSwitch.m_trigger ){
    setBranch<int>         (tree,"isTrigMatched",        m_isTrigMatched);
    setBranch<vector<int> >(tree,"isTrigMatchedToChain", m_isTrigMatchedToChain);
    setBranch<std::string> (tree,"listTrigChains",       m_listTrigChains);
  }

  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty() && isol != "NONE") {
        setBranch<int>(tree, "isIsolated_" + isol, &(*m_isIsolated)[isol]);
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    setBranch<float>(tree, "etcone20",         m_etcone20);
    setBranch<float>(tree, "ptcone20",         m_ptcone20);
    setBranch<float>(tree, "ptcone30",         m_ptcone30);
    setBranch<float>(tree, "ptcone40",         m_ptcone40);
    setBranch<float>(tree, "ptvarcone20",      m_ptvarcone20);
    setBranch<float>(tree, "ptvarcone30",      m_ptvarcone30);
    setBranch<float>(tree, "ptvarcone40",      m_ptvarcone40);
    setBranch<float>(tree, "topoetcone20",     m_topoetcone20);
    setBranch<float>(tree, "topoetcone30",     m_topoetcone30);
    setBranch<float>(tree, "topoetcone40",     m_topoetcone40);
  }

  if ( m_infoSwitch.m_PID ) {
    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      if (!PID.empty()) {
        setBranch<int>(tree, PID, &(*m_PID)[PID]);
      }
    }
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    for (auto& PID : m_infoSwitch.m_PIDSFWPs) {
      tree->Branch( (m_name+"_PIDEff_SF_"  + PID).c_str() , & (*m_PIDEff_SF)[ PID ] );
      for (auto& isol : m_infoSwitch.m_isolWPs) {
        if(!isol.empty())
          tree->Branch( (m_name+"_IsoEff_SF_"  + PID + "_isol" + isol).c_str() , & (*m_IsoEff_SF)[ PID+isol ] );
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          tree->Branch( (m_name+"_TrigEff_SF_" + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "")).c_str() , & (*m_TrigEff_SF)[ trig+PID+isol ] );
          tree->Branch( (m_name+"_TrigMCEff_"  + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "")).c_str() , & (*m_TrigMCEff) [ trig+PID+isol ] );
        }
      }
    }

    setBranch<vector<float> >(tree, "RecoEff_SF"  ,                m_RecoEff_SF  );
  }

  if ( m_infoSwitch.m_recoparams ) {
    setBranch<int>(tree, "author",   m_author);
    setBranch<int>(tree, "OQ",       m_OQ);
  }

  if ( m_infoSwitch.m_trackparams ) {
    setBranch<float>(tree, "trkd0",          m_trkd0);
    setBranch<float>(tree, "trkd0sig",       m_trkd0sig);
    setBranch<float>(tree, "trkz0",          m_trkz0);
    setBranch<float>(tree, "trkz0sintheta",  m_trkz0sintheta);
    setBranch<float>(tree, "trkphi0",        m_trkphi0);
    setBranch<float>(tree, "trktheta",       m_trktheta);
    setBranch<float>(tree, "trkcharge",      m_trkcharge);
    setBranch<float>(tree, "trkqOverP",      m_trkqOverP);
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    setBranch<int>(tree, "trknSiHits",    m_trknSiHits);
    setBranch<int>(tree, "trknPixHits",   m_trknPixHits);
    setBranch<int>(tree, "trknPixHoles",  m_trknPixHoles);
    setBranch<int>(tree, "trknSCTHits",   m_trknSCTHits);
    setBranch<int>(tree, "trknSCTHoles",  m_trknSCTHoles);
    setBranch<int>(tree, "trknTRTHits",   m_trknTRTHits);
    setBranch<int>(tree, "trknTRTHoles",  m_trknTRTHoles);
    setBranch<int>(tree, "trknBLayerHits",m_trknBLayerHits);
    setBranch<int>(tree, "trknInnermostPixLayHits",  m_trknInnermostPixLayHits);
    setBranch<float>(tree, "trkPixdEdX",    m_trkPixdEdX);
  }

  if ( m_infoSwitch.m_promptlepton ) {
    setBranch<float>(tree, "PromptLeptonInput_DL1mu",           m_PromptLeptonInput_DL1mu);
    setBranch<float>(tree, "PromptLeptonInput_DRlj",            m_PromptLeptonInput_DRlj);
    setBranch<float>(tree, "PromptLeptonInput_LepJetPtFrac",    m_PromptLeptonInput_LepJetPtFrac);
    setBranch<float>(tree, "PromptLeptonInput_PtFrac",          m_PromptLeptonInput_PtFrac);
    setBranch<float>(tree, "PromptLeptonInput_PtRel",           m_PromptLeptonInput_PtRel);
    setBranch<int>  (tree, "PromptLeptonInput_TrackJetNTrack",  m_PromptLeptonInput_TrackJetNTrack);
    setBranch<float>(tree, "PromptLeptonInput_ip2",             m_PromptLeptonInput_ip2);
    setBranch<float>(tree, "PromptLeptonInput_ip3",             m_PromptLeptonInput_ip3);
    setBranch<float>(tree, "PromptLeptonInput_rnnip",           m_PromptLeptonInput_rnnip);
    setBranch<int>  (tree, "PromptLeptonInput_sv1_jf_ntrkv",    m_PromptLeptonInput_sv1_jf_ntrkv);
    setBranch<float>(tree, "PromptLeptonIso",                   m_PromptLeptonIso);
    setBranch<float>(tree, "PromptLeptonVeto",                  m_PromptLeptonVeto);
  }

  return;
}



void ElectronContainer::clear()
{

  ParticleContainer::clear();

  if ( m_infoSwitch.m_kinematic ) {
    m_caloCluster_eta ->clear();
    m_charge          ->clear();
  }

  if ( m_infoSwitch.m_trigger ){
    m_isTrigMatched               ->clear();
    m_isTrigMatchedToChain        ->clear();
    m_listTrigChains              ->clear();
  }

  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      (*m_isIsolated)[ isol ].clear();
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    m_etcone20                               ->clear();
    m_ptcone20                               ->clear();
    m_ptcone30                               ->clear();
    m_ptcone40                               ->clear();
    m_ptvarcone20                            ->clear();
    m_ptvarcone30                            ->clear();
    m_ptvarcone40                            ->clear();
    m_topoetcone20                           ->clear();
    m_topoetcone30                           ->clear();
    m_topoetcone40                           ->clear();
  }

  if ( m_infoSwitch.m_PID ) {
    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      (*m_PID)[ PID ].clear();
    }
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {

    for (auto& PID : m_infoSwitch.m_PIDSFWPs) {
      (*m_PIDEff_SF)[ PID ].clear();
      for (auto& isol : m_infoSwitch.m_isolWPs) {
        if(!isol.empty())
          (*m_IsoEff_SF)[ PID+isol ].clear();
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          (*m_TrigEff_SF)[ trig+PID+isol ].clear();
          (*m_TrigMCEff)[ trig+PID+isol ].clear();
        }
      }
    }

    m_RecoEff_SF->clear();

  }

  if ( m_infoSwitch.m_recoparams ) {
    m_author    -> clear();
    m_OQ        -> clear();
  }

  if ( m_infoSwitch.m_trackparams ) {
    m_trkd0           -> clear();
    m_trkd0sig        -> clear();
    m_trkz0           -> clear();
    m_trkz0sintheta   -> clear();
    m_trkphi0         -> clear();
    m_trktheta        -> clear();
    m_trkcharge       -> clear();
    m_trkqOverP       -> clear();
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    m_trknSiHits                 -> clear();
    m_trknPixHits                -> clear();
    m_trknPixHoles               -> clear();
    m_trknSCTHits                -> clear();
    m_trknSCTHoles               -> clear();
    m_trknTRTHits                -> clear();
    m_trknTRTHoles               -> clear();
    m_trknBLayerHits             -> clear();
    m_trknInnermostPixLayHits    -> clear();
    m_trkPixdEdX                 -> clear();
  }

  if ( m_infoSwitch.m_promptlepton ) {
    m_PromptLeptonInput_DL1mu            -> clear();
    m_PromptLeptonInput_DRlj             -> clear();
    m_PromptLeptonInput_LepJetPtFrac     -> clear();
    m_PromptLeptonInput_PtFrac           -> clear();
    m_PromptLeptonInput_PtRel            -> clear();
    m_PromptLeptonInput_TrackJetNTrack   -> clear();
    m_PromptLeptonInput_ip2              -> clear();
    m_PromptLeptonInput_ip3              -> clear();
    m_PromptLeptonInput_rnnip            -> clear();
    m_PromptLeptonInput_sv1_jf_ntrkv     -> clear();
    m_PromptLeptonIso                    -> clear();
    m_PromptLeptonVeto                   -> clear();
  }

}


void ElectronContainer::FillElectron( const xAOD::Electron* elec, const xAOD::Vertex* primaryVertex ){
  return FillElectron(static_cast<const xAOD::IParticle*>(elec), primaryVertex);
}

void ElectronContainer::FillElectron( const xAOD::IParticle* particle, const xAOD::Vertex* primaryVertex )
{

  ParticleContainer::FillParticle(particle);

  const xAOD::Electron* elec=dynamic_cast<const xAOD::Electron*>(particle);

  const xAOD::TrackParticle* trk = elec->trackParticle();

  if ( m_infoSwitch.m_kinematic ) {
    float calo_eta   = ( elec->caloCluster() ) ? elec->caloCluster()->etaBE(2) : -999.0;
    m_caloCluster_eta->push_back( calo_eta );

    m_charge->push_back( elec->charge() );
  }

  if ( m_infoSwitch.m_trigger ) {

    // retrieve map<string,char> w/ <chain,isMatched>
    //
    static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapElAcc("isTrigMatchedMapEl");

    std::vector<int> matches;

    if ( isTrigMatchedMapElAcc.isAvailable( *elec ) ) {
      // loop over map and fill branches
      //
      for ( auto const &it : (isTrigMatchedMapElAcc( *elec )) ) {
	matches.push_back( static_cast<int>(it.second) );
	m_listTrigChains->push_back( it.first );
      }
    } else {
      matches.push_back( -1 );
      m_listTrigChains->push_back("NONE");
    }

    m_isTrigMatchedToChain->push_back(matches);

    // if at least one match among the chains is found, say this electron is trigger matched
    if ( std::find(matches.begin(), matches.end(), 1) != matches.end() ) { m_isTrigMatched->push_back(1); }
    else { m_isTrigMatched->push_back(0); }

  }

  if ( m_infoSwitch.m_isolation ) {
    static std::map< std::string, SG::AuxElement::Accessor<char> > accIsol;

    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty() && isol != "NONE") {
        std::string isolWP = "isIsolated_" + isol;
        accIsol.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( isol , SG::AuxElement::Accessor<char>( isolWP ) ) );
        safeFill<char, int, xAOD::Electron>( elec, accIsol.at( isol ), &m_isIsolated->at( isol ), -1 );
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    m_etcone20    ->push_back( elec->isolation( xAOD::Iso::etcone20 )    /m_units );
    m_ptcone20    ->push_back( elec->isolation( xAOD::Iso::ptcone20 )    /m_units );
    m_ptcone30    ->push_back( elec->isolation( xAOD::Iso::ptcone30 )    /m_units );
    m_ptcone40    ->push_back( elec->isolation( xAOD::Iso::ptcone40 )    /m_units );
    m_ptvarcone20 ->push_back( elec->isolation( xAOD::Iso::ptvarcone20 ) /m_units );
    m_ptvarcone30 ->push_back( elec->isolation( xAOD::Iso::ptvarcone30 ) /m_units );
    m_ptvarcone40 ->push_back( elec->isolation( xAOD::Iso::ptvarcone40 ) /m_units );
    m_topoetcone20->push_back( elec->isolation( xAOD::Iso::topoetcone20 )/m_units );
    m_topoetcone30->push_back( elec->isolation( xAOD::Iso::topoetcone30 )/m_units );
    m_topoetcone40->push_back( elec->isolation( xAOD::Iso::topoetcone40 )/m_units );
  }

  if ( m_infoSwitch.m_PID ) {
    static std::map< std::string, SG::AuxElement::Accessor<char> > accPID;
    static SG::AuxElement::Accessor<bool> accBLayer( "bLayerPass" );

    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      if (!PID.empty()) {
        if (PID == "LHLooseBL") {
          accPID.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( PID , SG::AuxElement::Accessor<char>( "LHLoose" ) ) );
          if ( accPID.at( PID ).isAvailable( *elec ) && accBLayer.isAvailable( *elec ) ) {
            m_PID->at( PID ).push_back( accBLayer( *elec ) == 1 && (accPID.at( PID ))( *elec ) == 1 );
          } else {
            m_PID->at( PID ).push_back( -1 );
          }
        } else {
          accPID.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( PID , SG::AuxElement::Accessor<char>( PID ) ) );
          safeFill<char, int, xAOD::Electron>( elec, accPID.at( PID ), &m_PID->at( PID ), -1 );
        }
      }
    }
  }

  if ( m_infoSwitch.m_recoparams ) {
    m_author -> push_back(elec->author());
    m_OQ     -> push_back(elec->isGoodOQ(xAOD::EgammaParameters::BADCLUSELECTRON));
  }

  if ( m_infoSwitch.m_trackparams ) {
    if ( trk ) {

      //
      // NB.:
      // All track parameters are calculated at the perigee, i.e., the point of closest approach to the origin of some r.f. (which in RunII is NOT the ATLAS detector r.f!).
      // The reference  frame is chosen to be a system centered in the beamspot position, with z axis parallel to the beam line.
      // Remember that the beamspot size ( of O(10 micrometers) in the transverse plane) is << average vertex transverse position resolution ( O(60-80 micrometers) )
      // The coordinates of this r.f. wrt. the ATLAS system origin are returned by means of vx(), vy(), vz()
      //
      m_trkd0->push_back( trk->d0() );

      static SG::AuxElement::Accessor<float> d0SigAcc ("d0sig");
      float d0_significance =  ( d0SigAcc.isAvailable( *elec ) ) ? d0SigAcc( *elec ) : -1.0;
      m_trkd0sig->push_back( d0_significance );
      if (primaryVertex)
        m_trkz0->push_back( trk->z0()  - ( primaryVertex->z() - trk->vz() ) );
      else
        m_trkz0->push_back( -999.0 );


      static SG::AuxElement::Accessor<float> z0sinthetaAcc("z0sintheta");
      float z0sintheta =  ( z0sinthetaAcc.isAvailable( *elec ) ) ? z0sinthetaAcc( *elec ) : -999.0;

      m_trkz0sintheta->push_back( z0sintheta );
      m_trkphi0->push_back( trk->phi0() );
      m_trktheta->push_back( trk->theta() );
      m_trkcharge->push_back( trk->charge() );
      m_trkqOverP->push_back( trk->qOverP() );

    } else {

      m_trkd0->push_back( -999.0 );
      m_trkd0sig->push_back( -999.0 );
      m_trkz0->push_back( -999.0 );
      m_trkz0sintheta->push_back( -999.0 );
      m_trkphi0->push_back( -999.0 );
      m_trktheta->push_back( -999.0 );
      m_trkcharge->push_back( -999.0 );
      m_trkqOverP->push_back( -999.0 );
    }
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    uint8_t nPixHits(-1), nPixHoles(-1), nSCTHits(-1), nSCTHoles(-1), nTRTHits(-1), nTRTHoles(-1), nBLayerHits(-1), nInnermostPixLayHits(-1);
    float pixdEdX(-1.0);
    if ( trk ) {
      trk->summaryValue( nPixHits,  xAOD::numberOfPixelHits );
      trk->summaryValue( nPixHoles, xAOD::numberOfPixelHoles );
      trk->summaryValue( nSCTHits,  xAOD::numberOfSCTHits );
      trk->summaryValue( nSCTHoles, xAOD::numberOfSCTHoles );
      trk->summaryValue( nTRTHits,  xAOD::numberOfTRTHits );
      trk->summaryValue( nTRTHoles, xAOD::numberOfTRTHoles );
      trk->summaryValue( nBLayerHits,  xAOD::numberOfBLayerHits );
      trk->summaryValue( nInnermostPixLayHits, xAOD::numberOfInnermostPixelLayerHits );
      trk->summaryValue( pixdEdX,   xAOD::pixeldEdx);
    }
    m_trknSiHits->push_back( nPixHits + nSCTHits );
    m_trknPixHits->push_back( nPixHits );
    m_trknPixHoles->push_back( nPixHoles );
    m_trknSCTHits->push_back( nSCTHits );
    m_trknSCTHoles->push_back( nSCTHoles );
    m_trknTRTHits->push_back( nTRTHits );
    m_trknTRTHoles->push_back( nTRTHoles );
    m_trknBLayerHits->push_back( nBLayerHits );
    m_trknInnermostPixLayHits->push_back( nInnermostPixLayHits );
    m_trkPixdEdX->push_back( pixdEdX );
  }

  if ( m_infoSwitch.m_promptlepton ) {
    SG::AuxElement::ConstAccessor<float> acc_DL1mu          ("PromptLeptonInput_DL1mu");
    SG::AuxElement::ConstAccessor<float> acc_DRlj           ("PromptLeptonInput_DRlj");
    SG::AuxElement::ConstAccessor<float> acc_LepJetPtFrac   ("PromptLeptonInput_LepJetPtFrac");
    SG::AuxElement::ConstAccessor<float> acc_PtFrac         ("PromptLeptonInput_PtFrac");
    SG::AuxElement::ConstAccessor<float> acc_PtRel          ("PromptLeptonInput_PtRel");
    SG::AuxElement::ConstAccessor<short> acc_TrackJetNTrack ("PromptLeptonInput_TrackJetNTrack");
    SG::AuxElement::ConstAccessor<float> acc_ip2            ("PromptLeptonInput_ip2");
    SG::AuxElement::ConstAccessor<float> acc_ip3            ("PromptLeptonInput_ip3");
    SG::AuxElement::ConstAccessor<float> acc_rnnip          ("PromptLeptonInput_rnnip");
    SG::AuxElement::ConstAccessor<short> acc_sv1_jf_ntrkv   ("PromptLeptonInput_sv1_jf_ntrkv");
    SG::AuxElement::ConstAccessor<float> acc_Iso            ("PromptLeptonIso");
    SG::AuxElement::ConstAccessor<float> acc_Veto           ("PromptLeptonVeto");

    m_PromptLeptonInput_DL1mu          ->push_back( acc_DL1mu          .isAvailable(*elec) ? acc_DL1mu(*elec)          : -100);
    m_PromptLeptonInput_DRlj           ->push_back( acc_DRlj           .isAvailable(*elec) ? acc_DRlj(*elec)           : -100);
    m_PromptLeptonInput_LepJetPtFrac   ->push_back( acc_LepJetPtFrac   .isAvailable(*elec) ? acc_LepJetPtFrac(*elec)   : -100);
    m_PromptLeptonInput_PtFrac         ->push_back( acc_PtFrac         .isAvailable(*elec) ? acc_PtFrac(*elec)         : -100);
    m_PromptLeptonInput_PtRel          ->push_back( acc_PtRel          .isAvailable(*elec) ? acc_PtRel(*elec)          : -100);
    m_PromptLeptonInput_TrackJetNTrack ->push_back( acc_TrackJetNTrack .isAvailable(*elec) ? acc_TrackJetNTrack(*elec) : -100);
    m_PromptLeptonInput_ip2            ->push_back( acc_ip2            .isAvailable(*elec) ? acc_ip2(*elec)            : -100);
    m_PromptLeptonInput_ip3            ->push_back( acc_ip3            .isAvailable(*elec) ? acc_ip3(*elec)            : -100);
    m_PromptLeptonInput_rnnip          ->push_back( acc_rnnip          .isAvailable(*elec) ? acc_rnnip(*elec)          : -100);
    m_PromptLeptonInput_sv1_jf_ntrkv   ->push_back( acc_sv1_jf_ntrkv   .isAvailable(*elec) ? acc_sv1_jf_ntrkv(*elec)   : -100);
    m_PromptLeptonIso                  ->push_back( acc_Iso            .isAvailable(*elec) ? acc_Iso(*elec)            : -100);
    m_PromptLeptonVeto                 ->push_back( acc_Veto           .isAvailable(*elec) ? acc_Veto(*elec)           : -100);
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {

    std::vector<float> junkSF(1,-1.0);
    std::vector<float> junkEff(1,-1.0);

    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accPIDSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accIsoSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigEFF;

    for (auto& PID : m_infoSwitch.m_PIDSFWPs) {
      std::string PIDSF = "ElPIDEff_SF_syst_" + PID;
      accPIDSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( PID , SG::AuxElement::Accessor< std::vector< float > >( PIDSF ) ) );
      safeSFVecFill<float, xAOD::Electron>( elec, accPIDSF.at( PID ), &m_PIDEff_SF->at( PID ), junkSF );

      for (auto& isol : m_infoSwitch.m_isolWPs) {

        if(!isol.empty()) {
          std::string IsoSF = "ElIsoEff_SF_syst_" + PID + "_isol" + isol;
          accIsoSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( PID+isol , SG::AuxElement::Accessor< std::vector< float > >( IsoSF ) ) );
          safeSFVecFill<float, xAOD::Electron>( elec, accIsoSF.at( PID+isol ), &m_IsoEff_SF->at( PID+isol ), junkSF );
        }

        for (auto& trig : m_infoSwitch.m_trigWPs) {

          std::string TrigSF = "ElTrigEff_SF_syst_" + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "");
          accTrigSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig+PID+isol , SG::AuxElement::Accessor< std::vector< float > >( TrigSF ) ) );
          safeSFVecFill<float, xAOD::Electron>( elec, accTrigSF.at( trig+PID+isol ), &m_TrigEff_SF->at( trig+PID+isol ), junkSF );

          std::string TrigEFF = "ElTrigMCEff_syst_" + trig + "_" + PID + (!isol.empty() ? "_isol" + isol : "");
          accTrigEFF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig+PID+isol , SG::AuxElement::Accessor< std::vector< float > >( TrigEFF ) ) );
          safeSFVecFill<float, xAOD::Electron>( elec, accTrigEFF.at( trig+PID+isol ), &m_TrigMCEff->at( trig+PID+isol ), junkSF );

        }

      }
    }

   static SG::AuxElement::Accessor< std::vector< float > > accRecoSF("ElRecoEff_SF_syst_Reconstruction");
   safeSFVecFill<float, xAOD::Electron>( elec, accRecoSF, m_RecoEff_SF, junkSF );
 }

  return;
}
