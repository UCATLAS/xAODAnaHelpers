#include "xAODAnaHelpers/MuonContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

MuonContainer::MuonContainer(const std::string& name, const std::string& detailStr, float units, bool mc)
  : ParticleContainer(name, detailStr, units, mc, true, false)
{

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched          = new     vector<int>               ();
    m_isTrigMatchedToChain   = new     vector<vector<int> >      ();
    m_listTrigChains         = new     vector<std::string>       ();
  }
    
  // isolation
  if ( m_infoSwitch.m_isolation ) {
    m_isIsolated_LooseTrackOnly                  = new  vector<int>   ();
    m_isIsolated_Loose                           = new  vector<int>   ();
    m_isIsolated_Tight                           = new  vector<int>   ();
    m_isIsolated_Gradient                        = new  vector<int>   ();
    m_isIsolated_GradientLoose                   = new  vector<int>   ();
    m_isIsolated_FixedCutLoose                   = new  vector<int>   ();
    m_isIsolated_FixedCutTightTrackOnly          = new  vector<int>   ();
    m_isIsolated_UserDefinedFixEfficiency        = new  vector<int>   ();
    m_isIsolated_UserDefinedCut                  = new  vector<int>   ();
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
    m_isVeryLoose  = new vector<int>();   
    m_isLoose      = new vector<int>();   
    m_isMedium     = new vector<int>();   
    m_isTight      = new vector<int>();   
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

}

MuonContainer::~MuonContainer()
{

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    delete m_isTrigMatched         ;
    delete m_isTrigMatchedToChain  ;
    delete m_listTrigChains        ;
  }
    
  // isolation
  if ( m_infoSwitch.m_isolation ) {
    delete m_isIsolated_LooseTrackOnly                  ;
    delete m_isIsolated_Loose                           ;
    delete m_isIsolated_Tight                           ;
    delete m_isIsolated_Gradient                        ;
    delete m_isIsolated_GradientLoose                   ;
    delete m_isIsolated_FixedCutLoose                   ;
    delete m_isIsolated_FixedCutTightTrackOnly          ;
    delete m_isIsolated_UserDefinedFixEfficiency        ;
    delete m_isIsolated_UserDefinedCut                  ;
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
    delete m_isVeryLoose  ;   
    delete m_isLoose      ;   
    delete m_isMedium     ;   
    delete m_isTight      ;   
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

}

void MuonContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if ( m_infoSwitch.m_trigger ){
    connectBranch<int>         (tree, "isTrigMatched",        &m_isTrigMatched);
    connectBranch<vector<int> >(tree, "isTrigMatchedToChain", &m_isTrigMatchedToChain );
    connectBranch<string>      (tree, "listTrigChains",       &m_listTrigChains );
  }

  if ( m_infoSwitch.m_isolation ) {
    connectBranch<int>(tree,"isIsolated_LooseTrackOnly", &m_isIsolated_LooseTrackOnly);
    connectBranch<int>(tree,"isIsolated_Loose",	     &m_isIsolated_Loose);
    connectBranch<int>(tree,"isIsolated_Tight",	     &m_isIsolated_Tight);
    connectBranch<int>(tree,"isIsolated_Gradient",	     &m_isIsolated_Gradient);
    connectBranch<int>(tree,"isIsolated_GradientLoose",  &m_isIsolated_GradientLoose);
    connectBranch<int>(tree,"isIsolated_FixedCutLoose",	   &m_isIsolated_FixedCutLoose);
    connectBranch<int>(tree,"isIsolated_FixedCutTightTrackOnly", &m_isIsolated_FixedCutTightTrackOnly);
    connectBranch<int>(tree,"isIsolated_UserDefinedFixEfficiency",    &m_isIsolated_UserDefinedFixEfficiency);
    connectBranch<int>(tree,"isIsolated_UserDefinedCut",              &m_isIsolated_UserDefinedCut);
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
      std::string recoEffSF = "muon_RecoEff_SF_" + reco; 
      tree->SetBranchAddress( recoEffSF.c_str() , & (*m_RecoEff_SF)[ reco ] );
    }
    
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      std::string isolEffSF = "muon_IsoEff_SF_" + isol; 
      tree->SetBranchAddress( isolEffSF.c_str() , & (*m_IsoEff_SF)[ isol ] );
    }
    
    for (auto& trig : m_infoSwitch.m_trigWPs) {
      std::string trigEffSF = "muon_TrigEff_SF_" + trig; 
      tree->SetBranchAddress( trigEffSF.c_str() , & (*m_TrigEff_SF)[ trig ] );
      
      
      std::string trigMCEff = "muon_TrigMCEff_" + trig;
      tree->SetBranchAddress( trigMCEff.c_str() , & (*m_TrigMCEff)[ trig ] );
    }
    
    connectBranch<vector<float> >(tree,"TTVAEff_SF",  &m_TTVAEff_SF);
  }

  if ( m_infoSwitch.m_quality ) {
    connectBranch<int>(tree,"isVeryLoose",  &m_isVeryLoose);
    connectBranch<int>(tree,"isLoose",      &m_isLoose);
    connectBranch<int>(tree,"isMedium",     &m_isMedium);
    connectBranch<int>(tree,"isTight",      &m_isTight);
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


}

void MuonContainer::updateParticle(uint idx, Muon& muon)
{
  ParticleContainer::updateParticle(idx,muon);  

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    muon.isTrigMatched         =     m_isTrigMatched         ->at(idx);
    muon.isTrigMatchedToChain  =     m_isTrigMatchedToChain  ->at(idx);
    muon.listTrigChains        =     m_listTrigChains        ->at(idx);
  }
    
  // isolation
  if ( m_infoSwitch.m_isolation ) {
    muon.isIsolated_LooseTrackOnly                =     m_isIsolated_LooseTrackOnly                  ->at(idx);
    muon.isIsolated_Loose                         =     m_isIsolated_Loose                           ->at(idx);
    muon.isIsolated_Tight                         =     m_isIsolated_Tight                           ->at(idx);
    muon.isIsolated_Gradient                      =     m_isIsolated_Gradient                        ->at(idx);
    muon.isIsolated_GradientLoose                 =     m_isIsolated_GradientLoose                   ->at(idx);
    muon.isIsolated_FixedCutLoose                 =     m_isIsolated_FixedCutLoose                   ->at(idx);
    muon.isIsolated_FixedCutTightTrackOnly        =     m_isIsolated_FixedCutTightTrackOnly          ->at(idx);
    muon.isIsolated_UserDefinedFixEfficiency      =     m_isIsolated_UserDefinedFixEfficiency        ->at(idx);
    muon.isIsolated_UserDefinedCut                =     m_isIsolated_UserDefinedCut                  ->at(idx);
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
    muon.isVeryLoose    = m_isVeryLoose  ->at(idx);   
    muon.isLoose        = m_isLoose      ->at(idx);   
    muon.isMedium       = m_isMedium     ->at(idx);   
    muon.isTight        = m_isTight      ->at(idx);   
  }
  
  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& reco : m_infoSwitch.m_recoWPs) {
      muon.RecoEff_SF[ reco ] = (*m_RecoEff_SF)[ reco ].at(idx);
    }
    
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      muon.IsoEff_SF[ isol ] = (*m_IsoEff_SF)[ isol ].at(idx);
    }
    
    for (auto& trig : m_infoSwitch.m_trigWPs) {
      muon.TrigEff_SF[ trig ] = (*m_TrigEff_SF)[ trig ].at(idx);
      muon.TrigMCEff [ trig ] = (*m_TrigMCEff )[ trig ].at(idx);
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

}


void MuonContainer::setBranches(TTree *tree)
{

  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_trigger ){
    // this is true if there's a match for at least one trigger chain
    setBranch<int>(tree,"isTrigMatched", m_isTrigMatched);
    // a vector of trigger match decision for each muon trigger chain
    setBranch<vector<int> >(tree,"isTrigMatchedToChain", m_isTrigMatchedToChain );
    // a vector of strings for each muon trigger chain - 1:1 correspondence w/ vector above
    setBranch<string>(tree, "listTrigChains", m_listTrigChains );
  }

  if ( m_infoSwitch.m_isolation ) {
    setBranch<int>(tree,"isIsolated_LooseTrackOnly", m_isIsolated_LooseTrackOnly);
    setBranch<int>(tree,"isIsolated_Loose",	     m_isIsolated_Loose);
    setBranch<int>(tree,"isIsolated_Tight",	     m_isIsolated_Tight);
    setBranch<int>(tree,"isIsolated_Gradient",	     m_isIsolated_Gradient);
    setBranch<int>(tree,"isIsolated_GradientLoose",  m_isIsolated_GradientLoose);
    setBranch<int>(tree,"isIsolated_FixedCutLoose",	   m_isIsolated_FixedCutLoose);
    setBranch<int>(tree,"isIsolated_FixedCutTightTrackOnly", m_isIsolated_FixedCutTightTrackOnly);
    setBranch<int>(tree,"isIsolated_UserDefinedFixEfficiency",    m_isIsolated_UserDefinedFixEfficiency);
    setBranch<int>(tree,"isIsolated_UserDefinedCut",              m_isIsolated_UserDefinedCut);
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
      std::string recoEffSF = "muon_RecoEff_SF_" + reco;
      tree->Branch( recoEffSF.c_str() , & (*m_RecoEff_SF)[ reco ] );

    }
    
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      std::string isolEffSF = "muon_IsoEff_SF_" + isol;
      tree->Branch( isolEffSF.c_str() , & (*m_IsoEff_SF)[ isol ] );
    }
    
    for (auto& trig : m_infoSwitch.m_trigWPs) {
      std::string trigEffSF = "muon_TrigEff_SF_" + trig;
      std::string trigMCEff = "muon_TrigMCEff_" + trig; 
      tree->Branch( trigEffSF.c_str() , & (*m_TrigEff_SF)[ trig ] );
      tree->Branch( trigMCEff.c_str() , & (*m_TrigMCEff)[ trig ] );
    }
    
    setBranch<vector<float> >(tree,"TTVAEff_SF",  m_TTVAEff_SF);
    
  }

  if ( m_infoSwitch.m_quality ) {
    setBranch<int>(tree,"isVeryLoose",  m_isVeryLoose);
    setBranch<int>(tree,"isLoose",      m_isLoose);
    setBranch<int>(tree,"isMedium",     m_isMedium);
    setBranch<int>(tree,"isTight",      m_isTight);
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
  
  return;
}



void MuonContainer::clear()
{
  
  ParticleContainer::clear();

  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched->clear();
    m_isTrigMatchedToChain->clear();
    m_listTrigChains->clear();
  }

  if ( m_infoSwitch.m_isolation ) {
    m_isIsolated_LooseTrackOnly->clear();
    m_isIsolated_Loose->clear();
    m_isIsolated_Tight->clear();
    m_isIsolated_Gradient->clear();
    m_isIsolated_GradientLoose->clear();
    m_isIsolated_FixedCutLoose->clear();
    m_isIsolated_FixedCutTightTrackOnly->clear();
    m_isIsolated_UserDefinedFixEfficiency->clear();
    m_isIsolated_UserDefinedCut->clear();
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
    m_isVeryLoose->clear();
    m_isLoose->clear();
    m_isMedium->clear();
    m_isTight->clear();
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

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& reco : m_infoSwitch.m_recoWPs) {
      (*m_RecoEff_SF)[ reco ].clear();
    }
    
    for (auto& isol : m_infoSwitch.m_isolWPs) {
      (*m_IsoEff_SF)[ isol ].clear();
    }
    
    for (auto& trig : m_infoSwitch.m_trigWPs) {
      (*m_TrigEff_SF)[ trig ].clear();
      (*m_TrigMCEff)[ trig ].clear();
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
    

    static SG::AuxElement::Accessor<char> isIsoLooseTrackOnlyAcc ("isIsolated_LooseTrackOnly");
    safeFill<char, int, xAOD::Muon>(muon, isIsoLooseTrackOnlyAcc, m_isIsolated_LooseTrackOnly, -1);

    static SG::AuxElement::Accessor<char> isIsoLooseAcc ("isIsolated_Loose");
    safeFill<char, int, xAOD::Muon>(muon, isIsoLooseAcc, m_isIsolated_Loose, -1);

    static SG::AuxElement::Accessor<char> isIsoTightAcc ("isIsolated_Tight");
    safeFill<char, int, xAOD::Muon>(muon, isIsoTightAcc, m_isIsolated_Tight, -1);

    static SG::AuxElement::Accessor<char> isIsoGradientAcc ("isIsolated_Gradient");
    safeFill<char, int, xAOD::Muon>(muon, isIsoGradientAcc, m_isIsolated_Gradient, -1);

    static SG::AuxElement::Accessor<char> isIsoGradientLooseAcc ("isIsolated_GradientLoose");
    safeFill<char, int, xAOD::Muon>(muon, isIsoGradientLooseAcc, m_isIsolated_GradientLoose, -1);

    static SG::AuxElement::Accessor<char> isIsoFixedCutLooseAcc ("isIsolated_FixedCutLoose");
    safeFill<char, int, xAOD::Muon>(muon, isIsoFixedCutLooseAcc, m_isIsolated_FixedCutLoose, -1);

    static SG::AuxElement::Accessor<char> isIsoFixedCutTightTrackOnlyAcc ("isIsolated_FixedCutTightTrackOnly");
    safeFill<char, int, xAOD::Muon>(muon, isIsoFixedCutTightTrackOnlyAcc, m_isIsolated_FixedCutTightTrackOnly, -1);

    static SG::AuxElement::Accessor<char> isIsoUserDefinedFixEfficiencyAcc ("isIsolated_UserDefinedFixEfficiency");
    safeFill<char, int, xAOD::Muon>(muon, isIsoUserDefinedFixEfficiencyAcc, m_isIsolated_UserDefinedFixEfficiency, -1);

    static SG::AuxElement::Accessor<char> isIsoUserDefinedCutAcc ("isIsolated_UserDefinedCut");
    safeFill<char, int, xAOD::Muon>(muon, isIsoUserDefinedCutAcc, m_isIsolated_UserDefinedCut, -1);

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
    static SG::AuxElement::Accessor<char> isVeryLooseQAcc ("isVeryLooseQ");
    safeFill<char, int, xAOD::Muon>(muon, isVeryLooseQAcc, m_isVeryLoose, -1);

    static SG::AuxElement::Accessor<char> isLooseQAcc ("isLooseQ");
    safeFill<char, int, xAOD::Muon>(muon, isLooseQAcc, m_isLoose, -1);

    static SG::AuxElement::Accessor<char> isMediumQAcc ("isMediumQ");
    safeFill<char, int, xAOD::Muon>(muon, isMediumQAcc, m_isMedium, -1);

    static SG::AuxElement::Accessor<char> isTightQAcc ("isTightQ");
    safeFill<char, int, xAOD::Muon>(muon, isTightQAcc, m_isTight, -1);    
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
      float d0_significance =  ( d0SigAcc.isAvailable( *muon ) ) ? fabs( d0SigAcc( *muon ) ) : -1.0;
      m_trkd0sig->push_back( d0_significance );

      m_trkz0->push_back( trk->z0()  - ( primaryVertex->z() - trk->vz() ) );

      static SG::AuxElement::Accessor<float> z0sinthetaAcc("z0sintheta");
      float z0sintheta =  ( z0sinthetaAcc.isAvailable( *muon ) ) ? z0sinthetaAcc( *muon ) : -999.0;
      m_trkz0sintheta->push_back( z0sintheta );
      m_trkphi0  ->push_back( trk->phi0() );
      m_trktheta ->push_back( trk->theta() );
      m_trkcharge->push_back( trk->charge() );
      m_trkqOverP->push_back( trk->qOverP() );
    
    } else {
      m_trkd0          -> push_back( -999.0 );
      m_trkd0sig       -> push_back( -1.0 );
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

  if ( m_infoSwitch.m_effSF && m_mc ) {

    std::vector<float> junkSF(1,1.0);
    std::vector<float> junkEff(1,0.0);
    
    static SG::AuxElement::Accessor< std::vector< float > > accTTVASF("MuonEfficiencyCorrector_TTVASyst_TTVA");
    
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accRecoSF;
    
    for (auto& reco : m_infoSwitch.m_recoWPs) {
            
      std::string recoEffSF = "MuonEfficiencyCorrector_RecoSyst_" + reco;
      accRecoSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( reco , SG::AuxElement::Accessor< std::vector< float > >( recoEffSF ) ) );
      if( (accRecoSF.at( reco  )).isAvailable( *muon ) ) { 
        m_RecoEff_SF->at( reco ).push_back( (accRecoSF.at( reco ))( *muon ) ); 
      }else { 
        m_RecoEff_SF->at( reco  ).push_back( junkSF ); 
      }
    }
    
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accIsoSF;
    
    for (auto& isol : m_infoSwitch.m_isolWPs) {
            
      std::string isolEffSF = "MuonEfficiencyCorrector_IsoSyst_" + isol;
      accIsoSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( isol , SG::AuxElement::Accessor< std::vector< float > >( isolEffSF ) ) );
      if( (accIsoSF.at( isol  )).isAvailable( *muon ) ) { 
        m_IsoEff_SF->at( isol ).push_back( (accIsoSF.at( isol ))( *muon ) ); 
      }else { 
        m_IsoEff_SF->at( isol  ).push_back( junkSF ); 
      }
    }
    
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigEFF;
    
    for (auto& trig : m_infoSwitch.m_trigWPs) {
            
      std::string trigEffSF = "MuonEfficiencyCorrector_TrigSyst_" + trig;
      accTrigSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig , SG::AuxElement::Accessor< std::vector< float > >( trigEffSF ) ) );
      if( (accTrigSF.at( trig  )).isAvailable( *muon ) ) { 
        m_TrigEff_SF->at( trig ).push_back( (accTrigSF.at( trig ))( *muon ) ); 
      }else { 
        m_TrigEff_SF->at( trig  ).push_back( junkSF ); 
      }
      
      std::string trigMCEff = "MuonEfficiencyCorrector_TrigMCEff_" + trig;
      accTrigEFF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig , SG::AuxElement::Accessor< std::vector< float > >( trigMCEff ) ) );
      if( (accTrigEFF.at( trig  )).isAvailable( *muon ) ) { 
        m_TrigMCEff->at( trig  ).push_back( (accTrigEFF.at( trig  ))( *muon ) ); 
      } else { 
        m_TrigMCEff->at( trig  ).push_back( junkEff ); 
      }
    }


    if( accTTVASF.isAvailable( *muon ) )         { m_TTVAEff_SF->push_back( accTTVASF( *muon ) ); } else { m_TTVAEff_SF->push_back( junkSF ); }

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
