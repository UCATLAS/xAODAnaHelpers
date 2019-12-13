#include "xAODAnaHelpers/MuonContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

MuonContainer::MuonContainer(const std::string& name, const std::string& detailStr, float units, bool mc, bool storeSystSFs)
  : ParticleContainer(name, detailStr, units, mc, true, storeSystSFs)
{

  if ( m_infoSwitch.m_kinematic ) {
    m_charge          = new std::vector<float> ();
  }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched          = new     vector<int>               ();
    m_isTrigMatchedToChain   = new     vector<vector<int> >      ();
    m_listTrigChains         = new     vector<std::string>       ();
  }
    
  // isolation
  if ( m_infoSwitch.m_isolation ) {
    m_isIsolated = new std::map< std::string, std::vector< int > >();
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    m_ptcone20                                   = new  vector<float> ();
    m_ptcone30                                   = new  vector<float> ();
    m_ptcone40                                   = new  vector<float> ();
    m_ptvarcone20                                = new  vector<float> ();
    m_ptvarcone30                                = new  vector<float> ();
    m_ptvarcone40                                = new  vector<float> ();
    m_topoetcone20                               = new  vector<float> ();
    m_topoetcone30                               = new  vector<float> ();
    m_topoetcone40                               = new  vector<float> ();
  }

  // quality
  if ( m_infoSwitch.m_quality ) {
    m_quality = new std::map< std::string, std::vector< int > >();
  }

  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    m_RecoEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
    m_IsoEff_SF  = new std::map< std::string, std::vector< std::vector< float > > >();
    m_TrigEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
    m_TrigMCEff  = new std::map< std::string, std::vector< std::vector< float > > >();
  
    m_TTVAEff_SF = new vector< vector< float > > ();
  
  }
      // track parameters
  if ( m_infoSwitch.m_trackparams ) {
    m_trkd0            = new vector<float> ();
    m_trkd0sig         = new vector<float> ();
    m_trkz0            = new vector<float> ();
    m_trkz0sintheta    = new vector<float> ();
    m_trkphi0          = new vector<float> ();
    m_trktheta         = new vector<float> ();
    m_trkcharge        = new vector<float> ();
    m_trkqOverP        = new vector<float> ();
  }

      // track hit content
  if ( m_infoSwitch.m_trackhitcont ) {
    m_trknSiHits                = new vector<int>   ();
    m_trknPixHits               = new vector<int>   ();
    m_trknPixHoles              = new vector<int>   ();
    m_trknSCTHits               = new vector<int>   ();
    m_trknSCTHoles              = new vector<int>   ();
    m_trknTRTHits               = new vector<int>   ();
    m_trknTRTHoles              = new vector<int>   ();
    m_trknBLayerHits            = new vector<int>   ();
    m_trknInnermostPixLayHits   = new vector<int>   ();         // not available in DC14
    m_trkPixdEdX                = new vector<float> ();         // not available in DC14
  }

  if ( m_infoSwitch.m_energyLoss ) {
    m_EnergyLoss                   = new vector<float>         ();
    m_EnergyLossSigma              = new vector<float>         ();
    m_energyLossType               = new vector<unsigned char> ();
    m_MeasEnergyLoss               = new vector<float>         ();
    m_MeasEnergyLossSigma          = new vector<float>         ();
    m_ParamEnergyLoss              = new vector<float>         ();
    m_ParamEnergyLossSigmaMinus    = new vector<float>         ();
    m_ParamEnergyLossSigmaPlus     = new vector<float>         ();
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

MuonContainer::~MuonContainer()
{
  if ( m_infoSwitch.m_kinematic ) {
    delete m_charge;
  }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    delete m_isTrigMatched         ;
    delete m_isTrigMatchedToChain  ;
    delete m_listTrigChains        ;
  }
    
  // isolation
  if ( m_infoSwitch.m_isolation ) {
    delete m_isIsolated;
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    delete m_ptcone20                                   ;
    delete m_ptcone30                                   ;
    delete m_ptcone40                                   ;
    delete m_ptvarcone20                                ;
    delete m_ptvarcone30                                ;
    delete m_ptvarcone40                                ;
    delete m_topoetcone20                               ;
    delete m_topoetcone30                               ;
    delete m_topoetcone40                               ;
  }
  
  // quality
  if ( m_infoSwitch.m_quality ) {
    delete m_quality;  
  }
  
  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    delete m_RecoEff_SF ;
    delete m_IsoEff_SF  ;
    delete m_TrigEff_SF ;
    delete m_TrigMCEff  ;
    
    delete m_TTVAEff_SF ;

  }
      // track parameters
  if ( m_infoSwitch.m_trackparams ) {
    delete m_trkd0            ;
    delete m_trkd0sig         ;
    delete m_trkz0            ;
    delete m_trkz0sintheta    ;
    delete m_trkphi0          ;
    delete m_trktheta         ;
    delete m_trkcharge        ;
    delete m_trkqOverP        ;
  }

      // track hit content
  if ( m_infoSwitch.m_trackhitcont ) {
    delete m_trknSiHits                ;
    delete m_trknPixHits               ;
    delete m_trknPixHoles              ;
    delete m_trknSCTHits               ;
    delete m_trknSCTHoles              ;
    delete m_trknTRTHits               ;
    delete m_trknTRTHoles              ;
    delete m_trknBLayerHits            ;
    delete m_trknInnermostPixLayHits   ;         // not available in DC14
    delete m_trkPixdEdX                ;         // not available in DC14
  }

  if ( m_infoSwitch.m_energyLoss ) {
    delete m_EnergyLoss                   ;
    delete m_EnergyLossSigma              ;
    delete m_energyLossType               ;
    delete m_MeasEnergyLoss               ;
    delete m_MeasEnergyLossSigma          ;
    delete m_ParamEnergyLoss              ;
    delete m_ParamEnergyLossSigmaMinus    ;
    delete m_ParamEnergyLossSigmaPlus     ;
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

void MuonContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if ( m_infoSwitch.m_kinematic ) {
    connectBranch<float>(tree, "charge", &m_charge);
  }

  if ( m_infoSwitch.m_trigger ){
    connectBranch<int>         (tree, "isTrigMatched",        &m_isTrigMatched);
    connectBranch<vector<int> >(tree, "isTrigMatchedToChain", &m_isTrigMatchedToChain );
    connectBranch<string>      (tree, "listTrigChains",       &m_listTrigChains );
  }

  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty()) {
        tree->SetBranchStatus ( (m_name + "_isIsolated_" + isol).c_str() , 1);
        tree->SetBranchAddress( (m_name + "_isIsolated_" + isol).c_str() , & (*m_isIsolated)[ isol ] );
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    connectBranch<float>(tree,"ptcone20",	  &m_ptcone20);
    connectBranch<float>(tree,"ptcone30",	  &m_ptcone30);
    connectBranch<float>(tree,"ptcone40",	  &m_ptcone40);
    connectBranch<float>(tree,"ptvarcone20",	  &m_ptvarcone20);
    connectBranch<float>(tree,"ptvarcone30",	  &m_ptvarcone30);
    connectBranch<float>(tree,"ptvarcone40",	  &m_ptvarcone40);
    connectBranch<float>(tree,"topoetcone20",   &m_topoetcone20);
    connectBranch<float>(tree,"topoetcone30",   &m_topoetcone30);
    connectBranch<float>(tree,"topoetcone40",   &m_topoetcone40);
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    for (auto& reco : m_infoSwitch.m_recoWPs) {
      tree->SetBranchStatus ( (m_name + "_RecoEff_SF_Reco" + reco).c_str() , 1);
      tree->SetBranchAddress( (m_name + "_RecoEff_SF_Reco" + reco).c_str() , & (*m_RecoEff_SF)[ reco ] );

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tree->SetBranchStatus ( (m_name + "_TrigEff_SF_" + trig + "_Reco" + reco).c_str() , 1 );
        tree->SetBranchAddress( (m_name + "_TrigEff_SF_" + trig + "_Reco" + reco).c_str() , & (*m_TrigEff_SF)[ trig+reco ] );

        tree->SetBranchStatus ( (m_name + "_TrigMCEff_"  + trig + "_Reco" + reco).c_str() , 1 );
        tree->SetBranchAddress( (m_name + "_TrigMCEff_"  + trig + "_Reco" + reco).c_str() , & (*m_TrigMCEff) [ trig+reco ] );
      }
    }

    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty()) {
        tree->SetBranchStatus ( (m_name + "_IsoEff_SF_Iso" + isol).c_str() , 1);
        tree->SetBranchAddress( (m_name + "_IsoEff_SF_Iso" + isol).c_str() , & (*m_IsoEff_SF)[ isol ] );
      }
    }

    connectBranch<vector<float> >(tree,"TTVAEff_SF",  &m_TTVAEff_SF);
  }

  if ( m_infoSwitch.m_quality ) {
    for (auto& quality : m_infoSwitch.m_recoWPs) {
      if (!quality.empty()) {
        tree->SetBranchStatus ( (m_name + "is" + quality).c_str() , 1);
        tree->SetBranchAddress( (m_name + "is" + quality).c_str() , & (*m_quality)[ quality ] );
      }
    }
  }

  if ( m_infoSwitch.m_trackparams ) {
    connectBranch<float>(tree,"trkd0",          &m_trkd0);
    connectBranch<float>(tree,"trkd0sig",       &m_trkd0sig);
    connectBranch<float>(tree,"trkz0",          &m_trkz0);
    connectBranch<float>(tree,"trkz0sintheta",  &m_trkz0sintheta);
    connectBranch<float>(tree,"trkphi0",        &m_trkphi0);
    connectBranch<float>(tree,"trktheta",       &m_trktheta);
    connectBranch<float>(tree,"trkcharge",      &m_trkcharge);
    connectBranch<float>(tree,"trkqOverP",      &m_trkqOverP);
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    connectBranch<int>(tree,"trknSiHits",    &m_trknSiHits);
    connectBranch<int>(tree,"trknPixHits",   &m_trknPixHits);
    connectBranch<int>(tree,"trknPixHoles",  &m_trknPixHoles);
    connectBranch<int>(tree,"trknSCTHits",   &m_trknSCTHits);
    connectBranch<int>(tree,"trknSCTHoles",  &m_trknSCTHoles);
    connectBranch<int>(tree,"trknTRTHits",   &m_trknTRTHits);
    connectBranch<int>(tree,"trknTRTHoles",  &m_trknTRTHoles);
    connectBranch<int>(tree,"trknBLayerHits",&m_trknBLayerHits);
    connectBranch<int>(tree,"trknInnermostPixLayHits",  &m_trknInnermostPixLayHits);
    connectBranch<float>(tree,"trkPixdEdX",    &m_trkPixdEdX);
  }

  if(m_infoSwitch.m_energyLoss ) {
    connectBranch<float>(tree,"EnergyLoss"                ,  &m_EnergyLoss               );
    connectBranch<float>(tree,"EnergyLossSigma"           ,  &m_EnergyLossSigma          );
    connectBranch<unsigned char>(tree,"energyLossType"            ,  &m_energyLossType           );
    connectBranch<float>(tree,"MeasEnergyLoss"            ,  &m_MeasEnergyLoss           );
    connectBranch<float>(tree,"MeasEnergyLossSigma"       ,  &m_MeasEnergyLossSigma      );
    connectBranch<float>(tree,"ParamEnergyLoss"           ,  &m_ParamEnergyLoss          );
    connectBranch<float>(tree,"ParamEnergyLossSigmaMinus" ,  &m_ParamEnergyLossSigmaMinus);
    connectBranch<float>(tree,"ParamEnergyLossSigmaPlus"  ,  &m_ParamEnergyLossSigmaPlus );
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

void MuonContainer::updateParticle(uint idx, Muon& muon)
{
  ParticleContainer::updateParticle(idx,muon);  

  if ( m_infoSwitch.m_kinematic ) {
    muon.charge = m_charge->at(idx);
  }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    muon.isTrigMatched         =     m_isTrigMatched         ->at(idx);
    muon.isTrigMatchedToChain  =     m_isTrigMatchedToChain  ->at(idx);
    muon.listTrigChains        =     m_listTrigChains        ->at(idx);
  }
    
  // isolation
  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty()) {
        muon.isIsolated[isol] = (*m_isIsolated)[ isol ].at(idx);
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    muon.ptcone20                                 =     m_ptcone20                                   ->at(idx);
    muon.ptcone30                                 =     m_ptcone30                                   ->at(idx);
    muon.ptcone40                                 =     m_ptcone40                                   ->at(idx);
    muon.ptvarcone20                              =     m_ptvarcone20                                ->at(idx);
    muon.ptvarcone30                              =     m_ptvarcone30                                ->at(idx);
    muon.ptvarcone40                              =     m_ptvarcone40                                ->at(idx);
    muon.topoetcone20                             =     m_topoetcone20                               ->at(idx);
    muon.topoetcone30                             =     m_topoetcone30                               ->at(idx);
    muon.topoetcone40                             =     m_topoetcone40                               ->at(idx);
  }
  
  // quality
  if ( m_infoSwitch.m_quality ) {
    for (auto& quality : m_infoSwitch.m_recoWPs) {
      if (!quality.empty()) {
        muon.quality[quality] = (*m_quality)[ quality ].at(idx);
      }
    } 
  }
  
  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& reco : m_infoSwitch.m_recoWPs) {
      muon.RecoEff_SF[ reco ] = (*m_RecoEff_SF)[ reco ].at(idx);

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        muon.TrigEff_SF[ trig+reco ] = (*m_TrigEff_SF)[ trig+reco ].at(idx);
        muon.TrigMCEff [ trig+reco ] = (*m_TrigMCEff )[ trig+reco ].at(idx);
      }
    }

    for (auto& isol : m_infoSwitch.m_isolWPs) {
      muon.IsoEff_SF[ isol ] = (*m_IsoEff_SF)[ isol ].at(idx);
    }

    muon.TTVAEff_SF = m_TTVAEff_SF -> at(idx);
  }
      // track parameters
  if ( m_infoSwitch.m_trackparams ) {
    muon.trkd0             = m_trkd0            ->at(idx);
    muon.trkd0sig          = m_trkd0sig         ->at(idx);
    muon.trkz0             = m_trkz0            ->at(idx);
    muon.trkz0sintheta     = m_trkz0sintheta    ->at(idx);
    muon.trkphi0           = m_trkphi0          ->at(idx);
    muon.trktheta          = m_trktheta         ->at(idx);
    muon.trkcharge         = m_trkcharge        ->at(idx);
    muon.trkqOverP         = m_trkqOverP        ->at(idx);
  }

      // track hit content
  if ( m_infoSwitch.m_trackhitcont ) {
    muon.trknSiHits                 = m_trknSiHits                ->at(idx);
    muon.trknPixHits                = m_trknPixHits               ->at(idx);
    muon.trknPixHoles               = m_trknPixHoles              ->at(idx);
    muon.trknSCTHits                = m_trknSCTHits               ->at(idx);
    muon.trknSCTHoles               = m_trknSCTHoles              ->at(idx);
    muon.trknTRTHits                = m_trknTRTHits               ->at(idx);
    muon.trknTRTHoles               = m_trknTRTHoles              ->at(idx);
    muon.trknBLayerHits             = m_trknBLayerHits            ->at(idx);
    muon.trknInnermostPixLayHits    = m_trknInnermostPixLayHits   ->at(idx);         // not available in DC14
    muon.trkPixdEdX                 = m_trkPixdEdX                ->at(idx);         // not available in DC14
  }

  if ( m_infoSwitch.m_energyLoss ) {
    muon.EnergyLoss                    = m_EnergyLoss                   ->at(idx);
    muon.EnergyLossSigma               = m_EnergyLossSigma              ->at(idx);
    muon.energyLossType                = m_energyLossType               ->at(idx);
    muon.MeasEnergyLoss                = m_MeasEnergyLoss               ->at(idx);
    muon.MeasEnergyLossSigma           = m_MeasEnergyLossSigma          ->at(idx);
    muon.ParamEnergyLoss               = m_ParamEnergyLoss              ->at(idx);
    muon.ParamEnergyLossSigmaMinus     = m_ParamEnergyLossSigmaMinus    ->at(idx);
    muon.ParamEnergyLossSigmaPlus      = m_ParamEnergyLossSigmaPlus     ->at(idx);
  }

  // prompt lepton
  if ( m_infoSwitch.m_promptlepton ) {
    muon.PromptLeptonInput_DL1mu           = m_PromptLeptonInput_DL1mu           ->at(idx);
    muon.PromptLeptonInput_DRlj            = m_PromptLeptonInput_DRlj            ->at(idx);
    muon.PromptLeptonInput_LepJetPtFrac    = m_PromptLeptonInput_LepJetPtFrac    ->at(idx);
    muon.PromptLeptonInput_PtFrac          = m_PromptLeptonInput_PtFrac          ->at(idx);
    muon.PromptLeptonInput_PtRel           = m_PromptLeptonInput_PtRel           ->at(idx);
    muon.PromptLeptonInput_TrackJetNTrack  = m_PromptLeptonInput_TrackJetNTrack  ->at(idx);
    muon.PromptLeptonInput_ip2             = m_PromptLeptonInput_ip2             ->at(idx);
    muon.PromptLeptonInput_ip3             = m_PromptLeptonInput_ip3             ->at(idx);
    muon.PromptLeptonInput_rnnip           = m_PromptLeptonInput_rnnip           ->at(idx);
    muon.PromptLeptonInput_sv1_jf_ntrkv    = m_PromptLeptonInput_sv1_jf_ntrkv    ->at(idx);
    muon.PromptLeptonIso                   = m_PromptLeptonIso                   ->at(idx);
    muon.PromptLeptonVeto                  = m_PromptLeptonVeto                  ->at(idx);
  }

}


void MuonContainer::setBranches(TTree *tree)
{

  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_kinematic ) {
    setBranch<float>(tree, "charge", m_charge);
  }

  if ( m_infoSwitch.m_trigger ){
    // this is true if there's a match for at least one trigger chain
    setBranch<int>(tree,"isTrigMatched", m_isTrigMatched);
    // a vector of trigger match decision for each muon trigger chain
    setBranch<vector<int> >(tree,"isTrigMatchedToChain", m_isTrigMatchedToChain );
    // a vector of strings for each muon trigger chain - 1:1 correspondence w/ vector above
    setBranch<string>(tree, "listTrigChains", m_listTrigChains );
  }

  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty()) {
        setBranch<int>(tree, "isIsolated_" + isol, &(*m_isIsolated)[isol]);
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    setBranch<float>(tree,"ptcone20",	  m_ptcone20);
    setBranch<float>(tree,"ptcone30",	  m_ptcone30);
    setBranch<float>(tree,"ptcone40",	  m_ptcone40);
    setBranch<float>(tree,"ptvarcone20",	  m_ptvarcone20);
    setBranch<float>(tree,"ptvarcone30",	  m_ptvarcone30);
    setBranch<float>(tree,"ptvarcone40",	  m_ptvarcone40);
    setBranch<float>(tree,"topoetcone20",   m_topoetcone20);
    setBranch<float>(tree,"topoetcone30",   m_topoetcone30);
    setBranch<float>(tree,"topoetcone40",   m_topoetcone40);
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& reco : m_infoSwitch.m_recoWPs) {
      tree->Branch( (m_name + "_RecoEff_SF_Reco" + reco).c_str() , & (*m_RecoEff_SF)[ reco ] );
      
      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tree->Branch( (m_name + "_TrigEff_SF_" + trig + "_Reco" + reco).c_str() , & (*m_TrigEff_SF)[ trig+reco ] );
        tree->Branch( (m_name + "_TrigMCEff_" + trig + "_Reco" + reco).c_str() , & (*m_TrigMCEff)[ trig+reco ] );
      }
    }
    
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      tree->Branch( (m_name + "_IsoEff_SF_Iso" + isol).c_str() , & (*m_IsoEff_SF)[ isol ] );
    }
    
    setBranch<vector<float> >(tree,"TTVAEff_SF",  m_TTVAEff_SF);
    
  }

  if ( m_infoSwitch.m_quality ) {
    for (auto& quality : m_infoSwitch.m_recoWPs) {
      if (!quality.empty()) {
        setBranch<int>(tree, "is" + quality, &(*m_quality)[quality]);
      }
    }
  }

  if ( m_infoSwitch.m_trackparams ) {
    setBranch<float>(tree,"trkd0",          m_trkd0);
    setBranch<float>(tree,"trkd0sig",       m_trkd0sig);
    setBranch<float>(tree,"trkz0",          m_trkz0);
    setBranch<float>(tree,"trkz0sintheta",  m_trkz0sintheta);
    setBranch<float>(tree,"trkphi0",        m_trkphi0);
    setBranch<float>(tree,"trktheta",       m_trktheta);
    setBranch<float>(tree,"trkcharge",      m_trkcharge);
    setBranch<float>(tree,"trkqOverP",      m_trkqOverP);
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    setBranch<int>(tree,"trknSiHits",    m_trknSiHits);
    setBranch<int>(tree,"trknPixHits",   m_trknPixHits);
    setBranch<int>(tree,"trknPixHoles",  m_trknPixHoles);
    setBranch<int>(tree,"trknSCTHits",   m_trknSCTHits);
    setBranch<int>(tree,"trknSCTHoles",  m_trknSCTHoles);
    setBranch<int>(tree,"trknTRTHits",   m_trknTRTHits);
    setBranch<int>(tree,"trknTRTHoles",  m_trknTRTHoles);
    setBranch<int>(tree,"trknBLayerHits",m_trknBLayerHits);
    setBranch<int>(tree,"trknInnermostPixLayHits",  m_trknInnermostPixLayHits);
    setBranch<float>(tree,"trkPixdEdX",    m_trkPixdEdX);
  }

  if( m_infoSwitch.m_energyLoss ) {
    setBranch<float>(tree,"EnergyLoss"                ,  m_EnergyLoss               );
    setBranch<float>(tree,"EnergyLossSigma"           ,  m_EnergyLossSigma          );
    setBranch<unsigned char>(tree,"energyLossType"    ,  m_energyLossType           );
    setBranch<float>(tree,"MeasEnergyLoss"            ,  m_MeasEnergyLoss           );
    setBranch<float>(tree,"MeasEnergyLossSigma"       ,  m_MeasEnergyLossSigma      );
    setBranch<float>(tree,"ParamEnergyLoss"           ,  m_ParamEnergyLoss          );
    setBranch<float>(tree,"ParamEnergyLossSigmaMinus" ,  m_ParamEnergyLossSigmaMinus);
    setBranch<float>(tree,"ParamEnergyLossSigmaPlus"  ,  m_ParamEnergyLossSigmaPlus );
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



void MuonContainer::clear()
{
  
  ParticleContainer::clear();

  if ( m_infoSwitch.m_kinematic ) {
    m_charge->clear();
  }

  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched->clear();
    m_isTrigMatchedToChain->clear();
    m_listTrigChains->clear();
  }

  if ( m_infoSwitch.m_isolation ) {
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      (*m_isIsolated)[ isol ].clear();
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    m_ptcone20->clear();
    m_ptcone30->clear();
    m_ptcone40->clear();
    m_ptvarcone20->clear();
    m_ptvarcone30->clear();
    m_ptvarcone40->clear();
    m_topoetcone20->clear();
    m_topoetcone30->clear();
    m_topoetcone40->clear();
  }

  if ( m_infoSwitch.m_quality ) {
    for (auto& quality : m_infoSwitch.m_recoWPs) {
      (*m_quality)[ quality ].clear();
    }
  }

  if ( m_infoSwitch.m_trackparams ) {
    m_trkd0->clear();
    m_trkd0sig->clear();
    m_trkz0->clear();
    m_trkz0sintheta->clear();
    m_trkphi0->clear();
    m_trktheta->clear();
    m_trkcharge->clear();
    m_trkqOverP->clear();
  }

  if ( m_infoSwitch.m_trackhitcont ) {
    m_trknSiHits->clear();
    m_trknPixHits->clear();
    m_trknPixHoles->clear();
    m_trknSCTHits->clear();
    m_trknSCTHoles->clear();
    m_trknTRTHits->clear();
    m_trknTRTHoles->clear();
    m_trknBLayerHits->clear();
    m_trknInnermostPixLayHits->clear();
    m_trkPixdEdX->clear();
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

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& reco : m_infoSwitch.m_recoWPs) {
      (*m_RecoEff_SF)[ reco ].clear();

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        (*m_TrigEff_SF)[ trig+reco ].clear();
        (*m_TrigMCEff)[ trig+reco ].clear();
      }
    }

    for (auto& isol : m_infoSwitch.m_isolWPs) {
      (*m_IsoEff_SF)[ isol ].clear();
    }

    m_TTVAEff_SF->clear();
  }

  if ( m_infoSwitch.m_energyLoss ) {
    m_EnergyLoss->clear();
    m_EnergyLossSigma->clear();
    m_energyLossType->clear();
    m_MeasEnergyLoss->clear();
    m_MeasEnergyLossSigma->clear();
    m_ParamEnergyLoss->clear();
    m_ParamEnergyLossSigmaMinus->clear();
    m_ParamEnergyLossSigmaPlus->clear();

  }

}


void MuonContainer::FillMuon( const xAOD::Muon* muon, const xAOD::Vertex* primaryVertex ){
  return FillMuon(static_cast<const xAOD::IParticle*>(muon), primaryVertex);
}

void MuonContainer::FillMuon( const xAOD::IParticle* particle, const xAOD::Vertex* primaryVertex ) 
{

  ParticleContainer::FillParticle(particle);

  const xAOD::Muon* muon=dynamic_cast<const xAOD::Muon*>(particle);

  if ( m_infoSwitch.m_kinematic ) {
    m_charge->push_back( muon->charge() );
  }

  if ( m_infoSwitch.m_trigger ) {

    // retrieve map<string,char> w/ <chain,isMatched>
    //
    static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapMuAcc("isTrigMatchedMapMu");

    std::vector<int> matches;

    if ( isTrigMatchedMapMuAcc.isAvailable( *muon ) ) {
      // loop over map and fill branches
      //
      for ( auto const &it : (isTrigMatchedMapMuAcc( *muon )) ) {
	matches.push_back( static_cast<int>(it.second) );
	m_listTrigChains->push_back( it.first );
      }
    } else {
      matches.push_back( -1 );
      m_listTrigChains->push_back("NONE");
    }

    m_isTrigMatchedToChain->push_back(matches);
    
    // if at least one match among the chains is found, say this muon is trigger matched
    if ( std::find(matches.begin(), matches.end(), 1) != matches.end() ) { m_isTrigMatched->push_back(1); }
    else { m_isTrigMatched->push_back(0); }
    
  }
  
  
  if ( m_infoSwitch.m_isolation ) {
    static std::map< std::string, SG::AuxElement::Accessor<char> > accIsol;

    for (auto& isol : m_infoSwitch.m_isolWPs) {
      if (!isol.empty()) {
        std::string isolWP = "isIsolated_" + isol;
        accIsol.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( isol , SG::AuxElement::Accessor<char>( isolWP ) ) );
        safeFill<char, int, xAOD::Muon>( muon, accIsol.at( isol ), &m_isIsolated->at( isol ), -1 );
      }
    }
  }

  if ( m_infoSwitch.m_isolationKinematics ) {
    m_ptcone20    ->push_back( muon->isolation( xAOD::Iso::ptcone20 )    /m_units );
    m_ptcone30    ->push_back( muon->isolation( xAOD::Iso::ptcone30 )    /m_units );
    m_ptcone40    ->push_back( muon->isolation( xAOD::Iso::ptcone40 )    /m_units );
    m_ptvarcone20 ->push_back( muon->isolation( xAOD::Iso::ptvarcone20 ) /m_units );
    m_ptvarcone30 ->push_back( muon->isolation( xAOD::Iso::ptvarcone30 ) /m_units );
    m_ptvarcone40 ->push_back( muon->isolation( xAOD::Iso::ptvarcone40 ) /m_units );
    m_topoetcone20->push_back( muon->isolation( xAOD::Iso::topoetcone20 )/m_units );
    m_topoetcone30->push_back( muon->isolation( xAOD::Iso::topoetcone30 )/m_units );
    m_topoetcone40->push_back( muon->isolation( xAOD::Iso::topoetcone40 )/m_units );
    
  }

  if ( m_infoSwitch.m_quality ) {
    static std::map< std::string, SG::AuxElement::Accessor<char> > accQuality;

    for (auto& quality : m_infoSwitch.m_recoWPs) {
      if (!quality.empty()) {
        accQuality.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( quality , SG::AuxElement::Accessor<char>( "is" + quality + "Q" ) ) );
        safeFill<char, int, xAOD::Muon>( muon, accQuality.at( quality ), &m_quality->at( quality ), -1 );
      }
    } 
  }
  
  const xAOD::TrackParticle* trk = muon->primaryTrackParticle();

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
      float d0_significance =  ( d0SigAcc.isAvailable( *muon ) ) ? d0SigAcc( *muon ) : -1.0;
      m_trkd0sig->push_back( d0_significance );

      if (primaryVertex)
        m_trkz0->push_back( trk->z0()  - ( primaryVertex->z() - trk->vz() ) );
      else
        m_trkz0->push_back( -999.0 );
	    
      static SG::AuxElement::Accessor<float> z0sinthetaAcc("z0sintheta");
      float z0sintheta =  ( z0sinthetaAcc.isAvailable( *muon ) ) ? z0sinthetaAcc( *muon ) : -999.0;
      m_trkz0sintheta->push_back( z0sintheta );
      m_trkphi0  ->push_back( trk->phi0() );
      m_trktheta ->push_back( trk->theta() );
      m_trkcharge->push_back( trk->charge() );
      m_trkqOverP->push_back( trk->qOverP() );
    
    } else {
      m_trkd0          -> push_back( -999.0 );
      m_trkd0sig       -> push_back( -999.0 );
      m_trkz0          -> push_back( -999.0 );
      m_trkz0sintheta  -> push_back( -999.0 );
      m_trkphi0        -> push_back( -999.0 );
      m_trktheta       -> push_back( -999.0 );
      m_trkcharge      -> push_back( -999.0 );
      m_trkqOverP      -> push_back( -999.0 );
    }

  }

  if ( m_infoSwitch.m_trackhitcont ) {
    uint8_t nPixHits(-1), nPixHoles(-1), nSCTHits(-1), nSCTHoles(-1), nTRTHits(-1), nTRTHoles(-1), nBLayerHits(-1), nInnermostPixLayHits(-1);
    float pixdEdX(-1.0);
    
    if ( trk ) {
      trk->summaryValue( nPixHits,     xAOD::numberOfPixelHits );
      trk->summaryValue( nPixHoles,    xAOD::numberOfPixelHoles );
      trk->summaryValue( nSCTHits,     xAOD::numberOfSCTHits );
      trk->summaryValue( nSCTHoles,    xAOD::numberOfSCTHoles );
      trk->summaryValue( nTRTHits,     xAOD::numberOfTRTHits );
      trk->summaryValue( nTRTHoles,    xAOD::numberOfTRTHoles );
      trk->summaryValue( nBLayerHits,  xAOD::numberOfBLayerHits );
      trk->summaryValue( nInnermostPixLayHits, xAOD::numberOfInnermostPixelLayerHits );
      trk->summaryValue( pixdEdX,   xAOD::pixeldEdx);
    }
    
    m_trknSiHits               ->  push_back( nPixHits + nSCTHits );
    m_trknPixHits              ->  push_back( nPixHits );
    m_trknPixHoles             ->  push_back( nPixHoles );
    m_trknSCTHits              ->  push_back( nSCTHits );
    m_trknSCTHoles             ->  push_back( nSCTHoles );
    m_trknTRTHits              ->  push_back( nTRTHits );
    m_trknTRTHoles             ->  push_back( nTRTHoles );
    m_trknBLayerHits           ->  push_back( nBLayerHits );
    m_trknInnermostPixLayHits  ->  push_back( nInnermostPixLayHits );
    m_trkPixdEdX               ->  push_back( pixdEdX );
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

    m_PromptLeptonInput_DL1mu          ->push_back( acc_DL1mu          .isAvailable(*muon) ? acc_DL1mu(*muon)          : -100);
    m_PromptLeptonInput_DRlj           ->push_back( acc_DRlj           .isAvailable(*muon) ? acc_DRlj(*muon)           : -100);
    m_PromptLeptonInput_LepJetPtFrac   ->push_back( acc_LepJetPtFrac   .isAvailable(*muon) ? acc_LepJetPtFrac(*muon)   : -100);
    m_PromptLeptonInput_PtFrac         ->push_back( acc_PtFrac         .isAvailable(*muon) ? acc_PtFrac(*muon)         : -100);
    m_PromptLeptonInput_PtRel          ->push_back( acc_PtRel          .isAvailable(*muon) ? acc_PtRel(*muon)          : -100);
    m_PromptLeptonInput_TrackJetNTrack ->push_back( acc_TrackJetNTrack .isAvailable(*muon) ? acc_TrackJetNTrack(*muon) : -100);
    m_PromptLeptonInput_ip2            ->push_back( acc_ip2            .isAvailable(*muon) ? acc_ip2(*muon)            : -100);
    m_PromptLeptonInput_ip3            ->push_back( acc_ip3            .isAvailable(*muon) ? acc_ip3(*muon)            : -100);
    m_PromptLeptonInput_rnnip          ->push_back( acc_rnnip          .isAvailable(*muon) ? acc_rnnip(*muon)          : -100);
    m_PromptLeptonInput_sv1_jf_ntrkv   ->push_back( acc_sv1_jf_ntrkv   .isAvailable(*muon) ? acc_sv1_jf_ntrkv(*muon)   : -100);
    m_PromptLeptonIso                  ->push_back( acc_Iso            .isAvailable(*muon) ? acc_Iso(*muon)            : -100);
    m_PromptLeptonVeto                 ->push_back( acc_Veto           .isAvailable(*muon) ? acc_Veto(*muon)           : -100);
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {

    std::vector<float> junkSF(1,-1.0);
    std::vector<float> junkEff(1,-1.0);

    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accRecoSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigEFF;
    for (auto& reco : m_infoSwitch.m_recoWPs) {
      std::string recoEffSF = "MuRecoEff_SF_syst_Reco" + reco;
      accRecoSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( reco , SG::AuxElement::Accessor< std::vector< float > >( recoEffSF ) ) );
      safeSFVecFill<float, xAOD::Muon>( muon, accRecoSF.at( reco ), &m_RecoEff_SF->at( reco ), junkSF );

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        std::string trigEffSF = "MuTrigEff_SF_syst_" + trig + "_Reco" + reco;
        accTrigSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig+reco , SG::AuxElement::Accessor< std::vector< float > >( trigEffSF ) ) );
        safeSFVecFill<float, xAOD::Muon>( muon, accTrigSF.at( trig+reco ), &m_TrigEff_SF->at( trig+reco ), junkSF );

        std::string trigMCEff = "MuTrigMCEff_syst_" + trig + "_Reco" + reco;
        accTrigEFF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig+reco , SG::AuxElement::Accessor< std::vector< float > >( trigMCEff ) ) );
        safeSFVecFill<float, xAOD::Muon>( muon, accTrigEFF.at( trig+reco ), &m_TrigMCEff->at( trig+reco ), junkEff );
      }
    }

    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accIsoSF;
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      std::string isolEffSF = "MuIsoEff_SF_syst_Iso" + isol;
      accIsoSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( isol , SG::AuxElement::Accessor< std::vector< float > >( isolEffSF ) ) );
      safeSFVecFill<float, xAOD::Muon>( muon, accIsoSF.at( isol ), &m_IsoEff_SF->at( isol ), junkSF );
    }

    static SG::AuxElement::Accessor< std::vector< float > > accTTVASF("MuTTVAEff_SF_syst_TTVA");
    safeSFVecFill<float, xAOD::Muon>( muon, accTTVASF, m_TTVAEff_SF, junkSF );

  }

  if(m_infoSwitch.m_energyLoss ) {
    static SG::AuxElement::Accessor< float >         accMuon_EnergyLoss                ("EnergyLoss");
    safeFill<float, float, xAOD::Muon>(muon, accMuon_EnergyLoss, m_EnergyLoss, -1);

    static SG::AuxElement::Accessor< float >         accMuon_EnergyLossSigma           ("EnergyLossSigma");
    safeFill<float, float, xAOD::Muon>(muon, accMuon_EnergyLossSigma, m_EnergyLossSigma, -1);

    static SG::AuxElement::Accessor< unsigned char > accMuon_energyLossType            ("energyLossType");
    safeFill<unsigned char, unsigned char, xAOD::Muon>(muon, accMuon_energyLossType, m_energyLossType, -1);

    static SG::AuxElement::Accessor< float >         accMuon_MeasEnergyLoss            ("MeasEnergyLoss");
    safeFill<float, float, xAOD::Muon>(muon, accMuon_MeasEnergyLoss, m_MeasEnergyLoss, -1);

    static SG::AuxElement::Accessor< float >         accMuon_MeasEnergyLossSigma       ("MeasEnergyLossSigma");
    safeFill<float, float, xAOD::Muon>(muon, accMuon_MeasEnergyLossSigma, m_MeasEnergyLossSigma, -1);

    static SG::AuxElement::Accessor< float >         accMuon_ParamEnergyLoss           ("ParamEnergyLoss");
    safeFill<float, float, xAOD::Muon>(muon, accMuon_ParamEnergyLoss, m_ParamEnergyLoss, -1);

    static SG::AuxElement::Accessor< float >         accMuon_ParamEnergyLossSigmaMinus ("ParamEnergyLossSigmaMinus");
    safeFill<float, float, xAOD::Muon>(muon, accMuon_ParamEnergyLossSigmaMinus, m_ParamEnergyLossSigmaMinus, -1);

    static SG::AuxElement::Accessor< float >         accMuon_ParamEnergyLossSigmaPlus  ("ParamEnergyLossSigmaPlus");
    safeFill<float, float, xAOD::Muon>(muon, accMuon_ParamEnergyLossSigmaPlus, m_ParamEnergyLossSigmaPlus, -1);

  }
  
  return;
}
