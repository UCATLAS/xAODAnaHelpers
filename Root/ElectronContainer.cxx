#include "xAODAnaHelpers/ElectronContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

ElectronContainer::ElectronContainer(const std::string& name, const std::string& detailStr, float units, bool mc)
  : ParticleContainer(name, detailStr, units, mc, true, false)
{

  if ( m_infoSwitch.m_kinematic ) {
    m_caloCluster_eta = new std::vector<float> ();
  }

  if ( m_infoSwitch.m_trigger ){
    m_isTrigMatched               = new std::vector<int>               ();
    m_isTrigMatchedToChain        = new std::vector<std::vector<int> > ();
    m_listTrigChains              = new std::vector<std::string>       ();
  }

  if ( m_infoSwitch.m_isolation ) {

    m_isIsolated_LooseTrackOnly              = new std::vector<int>   ();
    m_isIsolated_Loose                       = new std::vector<int>   ();
    m_isIsolated_Tight                       = new std::vector<int>   ();
    m_isIsolated_Gradient                    = new std::vector<int>   ();
    m_isIsolated_GradientLoose               = new std::vector<int>   ();
    m_isIsolated_FixedCutLoose               = new std::vector<int>   ();
    m_isIsolated_FixedCutTight               = new std::vector<int>   ();
    m_isIsolated_FixedCutTightTrackOnly      = new std::vector<int>   ();
    m_isIsolated_UserDefinedFixEfficiency    = new std::vector<int>   ();
    m_isIsolated_UserDefinedCut              = new std::vector<int>   ();
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
    m_n_LHLoose = 0;
    m_LHLoose = new std::vector<int>   ();

    m_n_LHLooseBL = 0;
    m_LHLooseBL = new std::vector<int>   ();

    m_n_LHMedium = 0;
    m_LHMedium = new std::vector<int>   ();
    
    m_n_LHTight = 0;
    m_LHTight = new std::vector<int>   ();

    m_n_IsEMLoose = 0;
    m_IsEMLoose = new std::vector<int>   ();

    m_n_IsEMMedium = 0;
    m_IsEMMedium = new std::vector<int>   ();

    m_n_IsEMTight = 0;
    m_IsEMTight = new std::vector<int>   ();

  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    m_TrigEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
    m_TrigMCEff  = new std::map< std::string, std::vector< std::vector< float > > >();
    m_IsoEff_SF  = new std::map< std::string, std::vector< std::vector< float > > >();
    
    m_RecoEff_SF                  = new std::vector< std::vector< float > > ();
    m_PIDEff_SF_LHLooseAndBLayer  = new std::vector< std::vector< float > > ();
    m_PIDEff_SF_LHLoose           = new std::vector< std::vector< float > > ();
    m_PIDEff_SF_LHMedium          = new std::vector< std::vector< float > > ();
    m_PIDEff_SF_LHTight           = new std::vector< std::vector< float > > ();
    
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

  if ( m_infoSwitch.m_effSF && m_mc ) {

    // default PID working points if no user input
    if ( !m_infoSwitch.m_PIDWPs.size() ) m_infoSwitch.m_PIDWPs = {"LooseAndBLayerLLH","MediumLLH","TightLLH"};

    // default isolation working points if no user input
    if ( !m_infoSwitch.m_isolWPs.size() ) m_infoSwitch.m_isolWPs = {"","isolGradient","isolLoose","isolTight"};

    // default trigger working points if no user input
    if ( !m_infoSwitch.m_trigWPs.size() ) m_infoSwitch.m_trigWPs = {"SINGLE_E_2015_e24_lhmedium_L1EM20VH_OR_e60_lhmedium_OR_e120_lhloose_2016_e24_lhtight_nod0_ivarloose_OR_e60_lhmedium_nod0_OR_e140_lhloose_nod0"};

    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      for (auto& isol : m_infoSwitch.m_isolWPs) {
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          Info("ElectronConainer()", "Used working points: %s_%s_%s", trig.c_str(), PID.c_str(), isol.c_str() );
        }
      }
    }
  }   


}

ElectronContainer::~ElectronContainer()
{
  if ( m_infoSwitch.m_kinematic ) {
    delete m_caloCluster_eta;
  }

  if ( m_infoSwitch.m_trigger ){
    delete m_isTrigMatched       ;
    delete m_isTrigMatchedToChain;
    delete m_listTrigChains      ;
  }

  if ( m_infoSwitch.m_isolation ) {
    delete m_isIsolated_LooseTrackOnly              ;
    delete m_isIsolated_Loose                       ;
    delete m_isIsolated_Tight                       ;
    delete m_isIsolated_Gradient                    ;
    delete m_isIsolated_GradientLoose               ;
    delete m_isIsolated_FixedCutLoose               ;
    delete m_isIsolated_FixedCutTight               ;
    delete m_isIsolated_FixedCutTightTrackOnly      ;
    delete m_isIsolated_UserDefinedFixEfficiency    ;
    delete m_isIsolated_UserDefinedCut              ;
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
    delete m_LHLoose    ;
    delete m_LHLooseBL  ;
    delete m_LHMedium   ;
    delete m_LHTight    ;
    delete m_IsEMLoose  ;
    delete m_IsEMMedium ;
    delete m_IsEMTight  ;
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    delete m_TrigEff_SF ;
    delete m_TrigMCEff  ;
    delete m_IsoEff_SF  ;
    delete m_RecoEff_SF                 ;
    delete m_PIDEff_SF_LHLooseAndBLayer ;
    delete m_PIDEff_SF_LHLoose          ;
    delete m_PIDEff_SF_LHMedium         ;
    delete m_PIDEff_SF_LHTight          ;
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

}

void ElectronContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if ( m_infoSwitch.m_kinematic ) {
    connectBranch<float>(tree,"caloCluster_eta", &m_caloCluster_eta);
  }

  if ( m_infoSwitch.m_trigger ){
    connectBranch<int>         (tree,"isTrigMatched",        &m_isTrigMatched);
    connectBranch<vector<int> >(tree,"isTrigMatchedToChain", &m_isTrigMatchedToChain);
    connectBranch<std::string> (tree,"listTrigChains",       &m_listTrigChains);
  }

  if ( m_infoSwitch.m_isolation ) {
    connectBranch<int>(tree, "isIsolated_LooseTrackOnly",              &m_isIsolated_LooseTrackOnly);
    connectBranch<int>(tree, "isIsolated_Loose",                       &m_isIsolated_Loose);
    connectBranch<int>(tree, "isIsolated_Tight",                       &m_isIsolated_Tight);
    connectBranch<int>(tree, "isIsolated_Gradient",                    &m_isIsolated_Gradient);
    connectBranch<int>(tree, "isIsolated_GradientLoose",               &m_isIsolated_GradientLoose);
    connectBranch<int>(tree, "isIsolated_FixedCutLoose",               &m_isIsolated_FixedCutLoose);
    connectBranch<int>(tree, "isIsolated_FixedCutTight",               &m_isIsolated_FixedCutTight);
    connectBranch<int>(tree, "isIsolated_FixedCutTightTrackOnly",      &m_isIsolated_FixedCutTightTrackOnly);
    connectBranch<int>(tree, "isIsolated_UserDefinedFixEfficiency",    &m_isIsolated_UserDefinedFixEfficiency);
    connectBranch<int>(tree, "isIsolated_UserDefinedCut",              &m_isIsolated_UserDefinedCut);

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
    tree->SetBranchStatus (("n"+m_name+"_LHLoose").c_str(),     1);
    tree->SetBranchAddress(("n"+m_name+"_LHLoose").c_str(),     &m_n_LHLoose);
    connectBranch<int>(tree, "LHLoose",         &m_LHLoose);

    tree->SetBranchStatus (("n"+m_name+"_LHLooseBL").c_str(),   1);
    tree->SetBranchAddress(("n"+m_name+"_LHLooseBL").c_str(),   &m_n_LHLooseBL);
    connectBranch<int>(tree, "LHLooseBL",         &m_LHLooseBL);

    tree->SetBranchStatus (("n"+m_name+"_LHMedium").c_str(),      1);
    tree->SetBranchAddress(("n"+m_name+"_LHMedium").c_str(),      &m_n_LHMedium);
    connectBranch<int>(tree, "LHMedium",      &m_LHMedium);

    tree->SetBranchStatus (("n"+m_name+"_LHTight").c_str(),      1);
    tree->SetBranchAddress(("n"+m_name+"_LHTight").c_str(),      &m_n_LHTight);
    connectBranch<int>(tree, "LHTight",       &m_LHTight);

    tree->SetBranchStatus (("n"+m_name+"_IsEMLoose").c_str(),      1);
    tree->SetBranchAddress(("n"+m_name+"_IsEMLoose").c_str(),      &m_n_IsEMLoose);
    connectBranch<int>(tree, "IsEMLoose",     &m_IsEMLoose);

    tree->SetBranchStatus (("n"+m_name+"_IsEMMedium").c_str(),      1);
    tree->SetBranchAddress(("n"+m_name+"_IsEMMedium").c_str(),      &m_n_IsEMMedium);
    connectBranch<int>(tree, "IsEMMedium",    &m_IsEMMedium);

    tree->SetBranchStatus (("n"+m_name+"_IsEMTight").c_str(),      1);
    tree->SetBranchAddress(("n"+m_name+"_IsEMTight").c_str(),      &m_n_IsEMTight);
    connectBranch<int>(tree, "IsEMTight",     &m_IsEMTight);
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      for (auto& isol : m_infoSwitch.m_isolWPs) {
        if(!isol.empty()) {
          tree->SetBranchStatus ( (m_name+"_IsoEff_SF_" + PID + "_" + isol).c_str() , 1);
          tree->SetBranchAddress( (m_name+"_IsoEff_SF_" + PID + "_" + isol).c_str() , & (*m_IsoEff_SF)[ PID+isol ] );
        }
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          tree->SetBranchStatus ( (m_name+"_TrigEff_SF_" + trig + "_" + PID + (!isol.empty() ? "_" + isol : "")).c_str() , 1 );
          tree->SetBranchAddress( (m_name+"_TrigEff_SF_" + trig + "_" + PID + (!isol.empty() ? "_" + isol : "")).c_str() , & (*m_TrigEff_SF)[ trig+PID+isol ] );

          tree->SetBranchStatus ( (m_name+"_TrigMCEff_"  + trig + "_" + PID + (!isol.empty() ? "_" + isol : "")).c_str() , 1 );
          tree->SetBranchAddress( (m_name+"_TrigMCEff_"  + trig + "_" + PID + (!isol.empty() ? "_" + isol : "")).c_str() , & (*m_TrigMCEff) [ trig+PID+isol ] );
        }
      }
    }
    
    connectBranch<std::vector<float> >(tree, "RecoEff_SF"  ,                &m_RecoEff_SF  );
    connectBranch<std::vector<float> >(tree, "PIDEff_SF_LHLooseAndBLayer",  &m_PIDEff_SF_LHLooseAndBLayer);
    connectBranch<std::vector<float> >(tree, "PIDEff_SF_LHLoose",           &m_PIDEff_SF_LHLoose);
    connectBranch<std::vector<float> >(tree, "PIDEff_SF_LHMedium",          &m_PIDEff_SF_LHMedium);
    connectBranch<std::vector<float> >(tree, "PIDEff_SF_LHTight",           &m_PIDEff_SF_LHTight);
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


}

void ElectronContainer::updateParticle(uint idx, Electron& elec)
{
  ParticleContainer::updateParticle(idx,elec);  

  if ( m_infoSwitch.m_kinematic ) {
    elec.caloCluster_eta = m_caloCluster_eta -> at(idx);
  }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    elec.isTrigMatched         =     m_isTrigMatched         ->at(idx);
    elec.isTrigMatchedToChain  =     m_isTrigMatchedToChain  ->at(idx);
    elec.listTrigChains        =     m_listTrigChains        ->at(idx);
  }
    
  // isolation
  if ( m_infoSwitch.m_isolation ) {
    elec.isIsolated_LooseTrackOnly                =     m_isIsolated_LooseTrackOnly                  ->at(idx);
    elec.isIsolated_Loose                         =     m_isIsolated_Loose                           ->at(idx);
    elec.isIsolated_Tight                         =     m_isIsolated_Tight                           ->at(idx);
    elec.isIsolated_Gradient                      =     m_isIsolated_Gradient                        ->at(idx);
    elec.isIsolated_GradientLoose                 =     m_isIsolated_GradientLoose                   ->at(idx);
    elec.isIsolated_FixedCutLoose                 =     m_isIsolated_FixedCutLoose                   ->at(idx);
    elec.isIsolated_FixedCutTight                 =     m_isIsolated_FixedCutTight                   ->at(idx);
    elec.isIsolated_FixedCutTightTrackOnly        =     m_isIsolated_FixedCutTightTrackOnly          ->at(idx);
    elec.isIsolated_UserDefinedFixEfficiency      =     m_isIsolated_UserDefinedFixEfficiency        ->at(idx);
    elec.isIsolated_UserDefinedCut                =     m_isIsolated_UserDefinedCut                  ->at(idx);
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
    elec.LHLoose       = m_LHLoose    ->at(idx);   
    elec.LHLooseBL     = m_LHLooseBL  ->at(idx);   
    elec.LHMedium      = m_LHMedium   ->at(idx);   
    elec.LHTight       = m_LHTight    ->at(idx);   
    elec.IsEMLoose     = m_IsEMLoose  ->at(idx);   
    elec.IsEMMedium    = m_IsEMMedium ->at(idx);   
    elec.IsEMTight     = m_IsEMTight  ->at(idx);   
  }
  
  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {

    for (auto& PID : m_infoSwitch.m_PIDWPs) {
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
    elec.PIDEff_SF_LHLooseAndBLayer               = m_PIDEff_SF_LHLooseAndBLayer              ->at(idx);
    elec.PIDEff_SF_LHLoose                        = m_PIDEff_SF_LHLoose                       ->at(idx);
    elec.PIDEff_SF_LHMedium                       = m_PIDEff_SF_LHMedium                      ->at(idx);
    elec.PIDEff_SF_LHTight                        = m_PIDEff_SF_LHTight                       ->at(idx);

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

}


void ElectronContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_kinematic ) {
    setBranch<float>(tree,"caloCluster_eta", m_caloCluster_eta);
  }

  if ( m_infoSwitch.m_trigger ){
    setBranch<int>         (tree,"isTrigMatched",        m_isTrigMatched);
    setBranch<vector<int> >(tree,"isTrigMatchedToChain", m_isTrigMatchedToChain);
    setBranch<std::string> (tree,"listTrigChains",       m_listTrigChains);
  }

  if ( m_infoSwitch.m_isolation ) {
    setBranch<int>(tree, "isIsolated_LooseTrackOnly",              m_isIsolated_LooseTrackOnly);
    setBranch<int>(tree, "isIsolated_Loose",                       m_isIsolated_Loose);
    setBranch<int>(tree, "isIsolated_Tight",                       m_isIsolated_Tight);
    setBranch<int>(tree, "isIsolated_Gradient",                    m_isIsolated_Gradient);
    setBranch<int>(tree, "isIsolated_GradientLoose",               m_isIsolated_GradientLoose);
    setBranch<int>(tree, "isIsolated_FixedCutLoose",               m_isIsolated_FixedCutLoose);
    setBranch<int>(tree, "isIsolated_FixedCutTight",               m_isIsolated_FixedCutTight);
    setBranch<int>(tree, "isIsolated_FixedCutTightTrackOnly",      m_isIsolated_FixedCutTightTrackOnly);
    setBranch<int>(tree, "isIsolated_UserDefinedFixEfficiency",    m_isIsolated_UserDefinedFixEfficiency);
    setBranch<int>(tree, "isIsolated_UserDefinedCut",              m_isIsolated_UserDefinedCut);

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
    tree->Branch(("n"+m_name+"_LHLoose").c_str(),      &m_n_LHLoose);
    setBranch<int>(tree, "LHLoose",         m_LHLoose);

    tree->Branch(("n"+m_name+"_LHLooseBL").c_str(),      &m_n_LHLooseBL);
    setBranch<int>(tree, "LHLooseBL",         m_LHLooseBL);

    tree->Branch(("n"+m_name+"_LHMedium").c_str(),      &m_n_LHMedium);
    setBranch<int>(tree, "LHMedium",      m_LHMedium);

    tree->Branch(("n"+m_name+"_LHTight").c_str(),      &m_n_LHTight);
    setBranch<int>(tree, "LHTight",       m_LHTight);

    tree->Branch(("n"+m_name+"_IsEMLoose").c_str(),      &m_n_IsEMLoose);
    setBranch<int>(tree, "IsEMLoose",     m_IsEMLoose);

    tree->Branch(("n"+m_name+"_IsEMMedium").c_str(),      &m_n_IsEMMedium);
    setBranch<int>(tree, "IsEMMedium",    m_IsEMMedium);

    tree->Branch(("n"+m_name+"_IsEMTight").c_str(),      &m_n_IsEMTight);
    setBranch<int>(tree, "IsEMTight",     m_IsEMTight);
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      for (auto& isol : m_infoSwitch.m_isolWPs) {
        if(!isol.empty())
          tree->Branch( (m_name+"_IsoEff_SF_"  + PID + isol).c_str() , & (*m_IsoEff_SF)[ PID+isol ] );
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          tree->Branch( (m_name+"_TrigEff_SF_" + trig + "_" + PID + (!isol.empty() ? "_" + isol : "")).c_str() , & (*m_TrigEff_SF)[ trig+PID+isol ] );
          tree->Branch( (m_name+"_TrigMCEff_"  + trig + "_" + PID + (!isol.empty() ? "_" + isol : "")).c_str() , & (*m_TrigMCEff) [ trig+PID+isol ] );
        }
      }
    }
    
    setBranch<vector<float> >(tree, "RecoEff_SF"  ,                m_RecoEff_SF  );
    setBranch<vector<float> >(tree, "PIDEff_SF_LHLooseAndBLayer",  m_PIDEff_SF_LHLooseAndBLayer);
    setBranch<vector<float> >(tree, "PIDEff_SF_LHLoose",           m_PIDEff_SF_LHLoose);
    setBranch<vector<float> >(tree, "PIDEff_SF_LHMedium",          m_PIDEff_SF_LHMedium);
    setBranch<vector<float> >(tree, "PIDEff_SF_LHTight",           m_PIDEff_SF_LHTight);
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

  return;
}



void ElectronContainer::clear()
{
  
  ParticleContainer::clear();

  if ( m_infoSwitch.m_kinematic ) {
    m_caloCluster_eta ->clear();
  }

  if ( m_infoSwitch.m_trigger ){
    m_isTrigMatched               ->clear();
    m_isTrigMatchedToChain        ->clear();
    m_listTrigChains              ->clear();
  }

  if ( m_infoSwitch.m_isolation ) {

    m_isIsolated_LooseTrackOnly              ->clear();
    m_isIsolated_Loose                       ->clear();
    m_isIsolated_Tight                       ->clear();
    m_isIsolated_Gradient                    ->clear();
    m_isIsolated_GradientLoose               ->clear();
    m_isIsolated_FixedCutLoose               ->clear();
    m_isIsolated_FixedCutTight               ->clear();
    m_isIsolated_FixedCutTightTrackOnly      ->clear();
    m_isIsolated_UserDefinedFixEfficiency    ->clear();
    m_isIsolated_UserDefinedCut              ->clear();
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
    m_n_LHLoose = 0;
    m_LHLoose   -> clear();

    m_n_LHLooseBL = 0;
    m_LHLooseBL   -> clear();

    m_n_LHMedium = 0;
    m_LHMedium   -> clear();
    
    m_n_LHTight = 0;
    m_LHTight   -> clear();

    m_n_IsEMLoose = 0;
    m_IsEMLoose   -> clear();

    m_n_IsEMMedium = 0;
    m_IsEMMedium   -> clear();

    m_n_IsEMTight = 0;
    m_IsEMTight   -> clear();

  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& PID : m_infoSwitch.m_PIDWPs) {
      for (auto& isol : m_infoSwitch.m_isolWPs) {
        if(!isol.empty())
          (*m_IsoEff_SF)[ PID+isol ].clear();
        for (auto& trig : m_infoSwitch.m_trigWPs) {
          (*m_TrigEff_SF)[ trig+PID+isol ].clear();
          (*m_TrigMCEff)[ trig+PID+isol ].clear();
        }
      }
    }

    m_RecoEff_SF                  -> clear();
    m_PIDEff_SF_LHLooseAndBLayer  -> clear();
    m_PIDEff_SF_LHLoose           -> clear();
    m_PIDEff_SF_LHMedium          -> clear();
    m_PIDEff_SF_LHTight           -> clear();
    
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

    static SG::AuxElement::Accessor<char> isIsoLooseTrackOnlyAcc ("isIsolated_LooseTrackOnly");
    safeFill<char, int, xAOD::Electron>(elec, isIsoLooseTrackOnlyAcc, m_isIsolated_LooseTrackOnly, -1);

    static SG::AuxElement::Accessor<char> isIsoLooseAcc ("isIsolated_Loose");
    safeFill<char, int, xAOD::Electron>(elec, isIsoLooseAcc, m_isIsolated_Loose, -1);

    static SG::AuxElement::Accessor<char> isIsoTightAcc ("isIsolated_Tight");
    safeFill<char, int, xAOD::Electron>(elec, isIsoTightAcc, m_isIsolated_Tight, -1);

    static SG::AuxElement::Accessor<char> isIsoGradientAcc ("isIsolated_Gradient");
    safeFill<char, int, xAOD::Electron>(elec, isIsoGradientAcc, m_isIsolated_Gradient, -1);

    static SG::AuxElement::Accessor<char> isIsoGradientLooseAcc ("isIsolated_GradientLoose");
    safeFill<char, int, xAOD::Electron>(elec, isIsoGradientLooseAcc, m_isIsolated_GradientLoose, -1);

    static SG::AuxElement::Accessor<char> isIsoFixedCutLooseAcc ("isIsolated_FixedCutLoose");
    safeFill<char, int, xAOD::Electron>(elec, isIsoFixedCutLooseAcc, m_isIsolated_FixedCutLoose, -1);

    static SG::AuxElement::Accessor<char> isIsoFixedCutTightAcc ("isIsolated_FixedCutTight");
    safeFill<char, int, xAOD::Electron>(elec, isIsoFixedCutTightAcc, m_isIsolated_FixedCutTight, -1);

    static SG::AuxElement::Accessor<char> isIsoFixedCutTightTrackOnlyAcc ("isIsolated_FixedCutTightTrackOnly");
    safeFill<char, int, xAOD::Electron>(elec, isIsoFixedCutTightTrackOnlyAcc, m_isIsolated_FixedCutTightTrackOnly, -1);

    static SG::AuxElement::Accessor<char> isIsoUserDefinedFixEfficiencyAcc ("isIsolated_UserDefinedFixEfficiency");
    safeFill<char, int, xAOD::Electron>(elec, isIsoUserDefinedFixEfficiencyAcc, m_isIsolated_UserDefinedFixEfficiency, -1);

    static SG::AuxElement::Accessor<char> isIsoUserDefinedCutAcc ("isIsolated_UserDefinedCut");
    safeFill<char, int, xAOD::Electron>(elec, isIsoUserDefinedCutAcc, m_isIsolated_UserDefinedCut, -1);

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

    static SG::AuxElement::Accessor<char> LHLooseAcc ("LHLoose");
    if ( LHLooseAcc.isAvailable( *elec ) ) {
      m_LHLoose->push_back( LHLooseAcc( *elec ) );
      if ( LHLooseAcc( *elec ) == 1 ) { ++m_n_LHLoose; }
    }  else {  m_LHLoose->push_back( -1 ); }

    static SG::AuxElement::Accessor<char> LHLooseBLAcc ("LHLooseBL");
    if ( LHLooseBLAcc.isAvailable( *elec ) ) {
      m_LHLooseBL->push_back( LHLooseBLAcc( *elec ) );
      if ( LHLooseBLAcc( *elec ) == 1 ) { ++m_n_LHLooseBL; }
    }  else { m_LHLooseBL->push_back( -1 ); }

    static SG::AuxElement::Accessor<char> LHMediumAcc ("LHMedium");
    if ( LHMediumAcc.isAvailable( *elec ) ) {
      m_LHMedium->push_back( LHMediumAcc( *elec ) );
      if ( LHMediumAcc( *elec ) == 1 ) { ++m_n_LHMedium; }
    }  else { m_LHMedium->push_back( -1 ); }

    static SG::AuxElement::Accessor<char> LHTightAcc ("LHTight");
    if ( LHTightAcc.isAvailable( *elec ) ) {
      m_LHTight->push_back( LHTightAcc( *elec ) );
      if ( LHTightAcc( *elec ) == 1 ) { ++m_n_LHTight; }
    } else { m_LHTight->push_back( -1 ); }


    static SG::AuxElement::Accessor<char> EMLooseAcc ("IsEMLoose");
    if ( EMLooseAcc.isAvailable( *elec ) ) {
      m_IsEMLoose->push_back( EMLooseAcc( *elec ) );
      if ( EMLooseAcc( *elec ) == 1 ) { ++m_n_IsEMLoose; }
    } else { m_IsEMLoose->push_back( -1 ); }

    static SG::AuxElement::Accessor<char> EMMediumAcc ("IsEMMedium");
    if ( EMMediumAcc.isAvailable( *elec ) ) {
      m_IsEMMedium->push_back( EMMediumAcc( *elec ) );
      if ( EMMediumAcc( *elec ) == 1 ) { ++m_n_IsEMMedium; }
    } else { m_IsEMMedium->push_back( -1 ); }

    static SG::AuxElement::Accessor<char> EMTightAcc ("IsEMTight");
    if ( EMTightAcc.isAvailable( *elec ) ) {
      m_IsEMTight->push_back( EMTightAcc( *elec ) );
      if ( EMTightAcc( *elec ) == 1 ) { ++m_n_IsEMTight; }
    } else { m_IsEMTight->push_back( -1 ); }

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
      float d0_significance =  ( d0SigAcc.isAvailable( *elec ) ) ? fabs( d0SigAcc( *elec ) ) : -1.0;
      m_trkd0sig->push_back( d0_significance );
      m_trkz0->push_back( trk->z0()  - ( primaryVertex->z() - trk->vz() ) );
      
      static SG::AuxElement::Accessor<float> z0sinthetaAcc("z0sintheta");
      float z0sintheta =  ( z0sinthetaAcc.isAvailable( *elec ) ) ? z0sinthetaAcc( *elec ) : -999.0;
    
      m_trkz0sintheta->push_back( z0sintheta );
      m_trkphi0->push_back( trk->phi0() );
      m_trktheta->push_back( trk->theta() );
      m_trkcharge->push_back( trk->charge() );
      m_trkqOverP->push_back( trk->qOverP() );

    } else {
        
      m_trkd0->push_back( -999.0 );
      m_trkd0sig->push_back( -1.0 );
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

  if ( m_infoSwitch.m_effSF && m_mc ) {

    std::vector<float> junkSF(1,1.0);
    std::vector<float> junkEff(1,0.0);

    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accIsoSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigEFF;

    for (auto& PID : m_infoSwitch.m_PIDWPs) {

      for (auto& isol : m_infoSwitch.m_isolWPs) {

        if(!isol.empty()) {
          std::string IsoSF = "EleEffCorr_IsoSyst_" + PID + "_" + isol;
          accIsoSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( PID+isol , SG::AuxElement::Accessor< std::vector< float > >( IsoSF ) ) );
          if( (accIsoSF.at( PID+isol )).isAvailable( *elec ) ) { 
            m_IsoEff_SF->at( PID+isol ).push_back( (accIsoSF.at( PID+isol ))( *elec ) ); 
          } else { 
            m_IsoEff_SF->at( PID+isol ).push_back( junkSF ); 
          }
        }

        for (auto& trig : m_infoSwitch.m_trigWPs) {

          std::string TrigSF = "EleEffCorr_TrigSyst_" + trig + "_" + PID + (!isol.empty() ? "_" + isol : "");
          accTrigSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig+PID+isol , SG::AuxElement::Accessor< std::vector< float > >( TrigSF ) ) );
          if( (accTrigSF.at( trig+PID+isol )).isAvailable( *elec ) ) { 
            m_TrigEff_SF->at( trig+PID+isol ).push_back( (accTrigSF.at( trig+PID+isol ))( *elec ) ); 
          }else { 
            m_TrigEff_SF->at( trig+PID+isol ).push_back( junkSF ); 
          }

          std::string TrigEFF = "EleEffCorr_TrigMCEffSyst_" + trig + "_" + PID + (!isol.empty() ? "_" + isol : "");
          accTrigEFF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig+PID+isol , SG::AuxElement::Accessor< std::vector< float > >( TrigEFF ) ) );
          if( (accTrigEFF.at( trig+PID+isol )).isAvailable( *elec ) ) { 
            m_TrigMCEff->at( trig+PID+isol ).push_back( (accTrigEFF.at( trig+PID+isol ))( *elec ) ); 
          } else { 
            m_TrigMCEff->at( trig+PID+isol ).push_back( junkEff ); 
          }

        }

      }
    }

   static SG::AuxElement::Accessor< std::vector< float > > accRecoSF("EleEffCorr_RecoSyst");
   static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHLooseAndBLayer("EleEffCorr_PIDSyst_LooseAndBLayerLLH");
   static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHLoose("EleEffCorr_PIDSyst_LooseLLh");
   static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHMedium("EleEffCorr_PIDSyst_MediumLLH");
   static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHTight("EleEffCorr_PIDSyst_TightLLH");

   if( accRecoSF.isAvailable( *elec ) )                     { m_RecoEff_SF->push_back( accRecoSF( *elec ) ); } else { m_RecoEff_SF->push_back( junkSF ); }
   if( accPIDSF_LHLooseAndBLayer.isAvailable( *elec ) )     { m_PIDEff_SF_LHLooseAndBLayer->push_back( accPIDSF_LHLooseAndBLayer( *elec ) ); } else { m_PIDEff_SF_LHLooseAndBLayer->push_back( junkSF ); }
   if( accPIDSF_LHLoose.isAvailable( *elec ) )              { m_PIDEff_SF_LHLoose->push_back( accPIDSF_LHLoose( *elec ) ); } else { m_PIDEff_SF_LHLoose->push_back( junkSF ); }
   if( accPIDSF_LHMedium.isAvailable( *elec ) )             { m_PIDEff_SF_LHMedium->push_back( accPIDSF_LHMedium( *elec ) ); } else { m_PIDEff_SF_LHMedium->push_back( junkSF ); }
   if( accPIDSF_LHTight.isAvailable( *elec ) )              { m_PIDEff_SF_LHTight->push_back( accPIDSF_LHTight( *elec ) ); } else { m_PIDEff_SF_LHTight->push_back( junkSF ); }

 }

  return;
}
