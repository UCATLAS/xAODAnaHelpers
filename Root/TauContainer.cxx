#include "xAODAnaHelpers/TauContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

TauContainer::TauContainer(const std::string& name, const std::string& detailStr, float units, bool mc, bool storeSystSFs)
  : ParticleContainer(name, detailStr, units, mc, true, storeSystSFs)
{

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched          = new     vector<int>               ();
    m_isTrigMatchedToChain   = new     vector<vector<int> >      ();
    m_listTrigChains         = new     vector<std::string>       ();
  }
  
  if( m_infoSwitch.m_kinematic) {
    m_ntrk    = new  std::vector<int>   ();
    m_charge  = new  std::vector<float> ();
  }
  
  // might need to delete these  
  if( m_infoSwitch.m_JetID) {
    m_isJetBDTSigVeryLoose = new  std::vector<int>   ();
    m_isJetBDTSigLoose     = new  std::vector<int>   ();
    m_isJetBDTSigMedium    = new  std::vector<int>   ();
    m_isJetBDTSigTight     = new  std::vector<int>   ();
    
    m_JetBDTScore          = new  std::vector<float>   ();
    m_JetBDTScoreSigTrans  = new  std::vector<float>   ();
  }

  if( m_infoSwitch.m_EleVeto) {
    m_isEleBDTLoose  = new  std::vector<int>   ();
    m_isEleBDTMedium = new  std::vector<int>   ();
    m_isEleBDTTight  = new  std::vector<int>   ();
    
    m_EleBDTScore    = new  std::vector<float> ();
    m_passEleOLR     = new  std::vector<int>   ();
  }

  if( m_infoSwitch.m_xahTauJetMatching) {
    m_tau_matchedJetWidth = new  std::vector<float>   ();
    m_tau_matchedJetJvt   = new  std::vector<float>   ();
  }

  if( m_infoSwitch.m_trackAll) {
    m_tau_tracks_pt       = new  std::vector< std::vector<float> > ();
    m_tau_tracks_eta      = new  std::vector< std::vector<float> > ();
    m_tau_tracks_phi      = new  std::vector< std::vector<float> > ();

    m_tau_tracks_isCore          = new  std::vector< std::vector<int> > ();
    m_tau_tracks_isWide          = new  std::vector< std::vector<int> > ();
    m_tau_tracks_failTrackFilter = new  std::vector< std::vector<int> > ();
    m_tau_tracks_passTrkSel      = new  std::vector< std::vector<int> > ();
    m_tau_tracks_isClCharged     = new  std::vector< std::vector<int> > ();
    m_tau_tracks_isClIso         = new  std::vector< std::vector<int> > ();
    m_tau_tracks_isClConv        = new  std::vector< std::vector<int> > ();
    m_tau_tracks_isClFake        = new  std::vector< std::vector<int> > ();
  }

  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    m_TauEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
    m_TauTrigEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
  
  }

}

TauContainer::~TauContainer()
{
  
  // trigger
  if ( m_infoSwitch.m_trigger ) {
    delete m_isTrigMatched         ;
    delete m_isTrigMatchedToChain  ;
    delete m_listTrigChains        ;
  }
  
  if (m_infoSwitch.m_kinematic){
    delete m_ntrk;
    delete m_charge;
  }

  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    delete m_TauEff_SF;
    delete m_TauTrigEff_SF;

  }

  // might need to delete these
  if( m_infoSwitch.m_JetID) {
    delete m_isJetBDTSigVeryLoose;
    delete m_isJetBDTSigLoose;
    delete m_isJetBDTSigMedium;
    delete m_isJetBDTSigTight;
    
    delete m_JetBDTScore;
    delete m_JetBDTScoreSigTrans;
  }

  if( m_infoSwitch.m_EleVeto) {
    delete m_isEleBDTLoose;
    delete m_isEleBDTMedium;
    delete m_isEleBDTTight;
    
    delete m_EleBDTScore;

    delete m_passEleOLR;
  }

  if( m_infoSwitch.m_xahTauJetMatching) {
    delete m_tau_matchedJetWidth; 
    delete m_tau_matchedJetJvt; 
  }

  if( m_infoSwitch.m_trackAll) {
    delete m_tau_tracks_pt;
    delete m_tau_tracks_eta;
    delete m_tau_tracks_phi;

    delete m_tau_tracks_isCore;
    delete m_tau_tracks_isWide;
    delete m_tau_tracks_failTrackFilter;
    delete m_tau_tracks_passTrkSel;
    delete m_tau_tracks_isClCharged;
    delete m_tau_tracks_isClIso;
    delete m_tau_tracks_isClConv;
    delete m_tau_tracks_isClFake;
  }

}

void TauContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if ( m_infoSwitch.m_trigger ){
    connectBranch<int>         (tree, "isTrigMatched",        &m_isTrigMatched);
    connectBranch<vector<int> >(tree, "isTrigMatchedToChain", &m_isTrigMatchedToChain );
    connectBranch<string>      (tree, "listTrigChains",       &m_listTrigChains );
  }
  
  if ( m_infoSwitch.m_kinematic ){
    connectBranch<int>    (tree, "ntrk",       &m_ntrk);
    connectBranch<float>  (tree, "charge",     &m_charge );
  }
  
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& taueff : m_infoSwitch.m_tauEffWPs) {
      tree->SetBranchStatus ( (m_name + "_TauEff_SF_" + taueff).c_str() , 1);
      tree->SetBranchAddress( (m_name + "_TauEff_SF_" + taueff).c_str() , & (*m_TauEff_SF)[ taueff ] );

    }

    for (auto& trig : m_infoSwitch.m_trigWPs) {
      tree->SetBranchStatus ( (m_name + "_TauTrigEff_SF_" + trig).c_str() , 1 );
      tree->SetBranchAddress( (m_name + "_TauTrigEff_SF_" + trig).c_str() , & (*m_TauTrigEff_SF)[ trig ] );

    }
  }

  // might need to delete these 
  if ( m_infoSwitch.m_JetID ){
    connectBranch<int>    (tree, "isJetBDTSigVeryLoose",   &m_isJetBDTSigVeryLoose);
    connectBranch<int>    (tree, "isJetBDTSigLoose",       &m_isJetBDTSigLoose);
    connectBranch<int>    (tree, "isJetBDTSigMedium",      &m_isJetBDTSigMedium);
    connectBranch<int>    (tree, "isJetBDTSigTight",       &m_isJetBDTSigTight);
    
    connectBranch<float>  (tree, "JetBDTScore",         &m_JetBDTScore);
    connectBranch<float>  (tree, "JetBDTScoreSigTrans", &m_JetBDTScoreSigTrans);
  }

  if ( m_infoSwitch.m_EleVeto ){
    connectBranch<int>    (tree, "isEleBDTLoose",    &m_isEleBDTLoose);
    connectBranch<int>    (tree, "isEleBDTMedium",   &m_isEleBDTMedium);
    connectBranch<int>    (tree, "isEleBDTTight",    &m_isEleBDTTight);
    
    connectBranch<float>  (tree, "EleBDTScore",    &m_EleBDTScore);
    connectBranch<int>    (tree, "passEleOLR",     &m_passEleOLR);
  }

  if( m_infoSwitch.m_xahTauJetMatching) {
    connectBranch<float>  (tree, "matchedJetWidth",    &m_tau_matchedJetWidth);
    connectBranch<float>  (tree, "matchedJetJvt",    &m_tau_matchedJetJvt);
  }

  if( m_infoSwitch.m_trackAll) {
    tree->SetBranchStatus ( (m_name + "_tracks_pt").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_pt").c_str() , &m_tau_tracks_pt );

    tree->SetBranchStatus ( (m_name + "_tracks_eta").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_eta").c_str() , &m_tau_tracks_eta );

    tree->SetBranchStatus ( (m_name + "_tracks_phi").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_phi").c_str() , &m_tau_tracks_phi );

    tree->SetBranchStatus ( (m_name + "_tracks_isCore").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_isCore").c_str() , &m_tau_tracks_isCore );

    tree->SetBranchStatus ( (m_name + "_tracks_isWide").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_isWide").c_str() , &m_tau_tracks_isWide );

    tree->SetBranchStatus ( (m_name + "_tracks_failTrackFilter").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_failTrackFilter").c_str() , &m_tau_tracks_failTrackFilter );

    tree->SetBranchStatus ( (m_name + "_tracks_passTrkSel").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_passTrkSel").c_str() , &m_tau_tracks_passTrkSel );

    tree->SetBranchStatus ( (m_name + "_tracks_isClCharged").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_isClCharged").c_str() , &m_tau_tracks_isClCharged );

    tree->SetBranchStatus ( (m_name + "_tracks_isClIso").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_isClIso").c_str() , &m_tau_tracks_isClIso );

    tree->SetBranchStatus ( (m_name + "_tracks_isClConv").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_isClConv").c_str() , &m_tau_tracks_isClConv );

    tree->SetBranchStatus ( (m_name + "_tracks_isClFake").c_str() , 1 );
    tree->SetBranchAddress( (m_name + "_tracks_isClFake").c_str() , &m_tau_tracks_isClFake );
  }

}

void TauContainer::updateParticle(uint idx, Tau& tau)
{
  ParticleContainer::updateParticle(idx,tau);  

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    tau.isTrigMatched         =     m_isTrigMatched         ->at(idx);
    tau.isTrigMatchedToChain  =     m_isTrigMatchedToChain  ->at(idx);
    tau.listTrigChains        =     m_listTrigChains        ->at(idx);
  }
  
  if ( m_infoSwitch.m_kinematic ) {
    tau.ntrk    =     m_ntrk    ->at(idx);
    tau.charge  =     m_charge  ->at(idx);
  }
  
  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& taueff : m_infoSwitch.m_tauEffWPs) {
      tau.TauEff_SF[ taueff ] = (*m_TauEff_SF)[ taueff ].at(idx);
    }
    
    for (auto& trig : m_infoSwitch.m_trigWPs) {
      tau.TauTrigEff_SF[ trig ] = (*m_TauTrigEff_SF)[ trig ].at(idx);
    }
  }

  //  might need to delete these
  if ( m_infoSwitch.m_JetID ) {
    tau.isJetBDTSigVeryLoose   =   m_isJetBDTSigVeryLoose   ->at(idx);
    tau.isJetBDTSigLoose       =   m_isJetBDTSigLoose       ->at(idx);
    tau.isJetBDTSigMedium      =   m_isJetBDTSigMedium      ->at(idx);
    tau.isJetBDTSigTight       =   m_isJetBDTSigTight       ->at(idx);
    
    tau.JetBDTScore         =   m_JetBDTScore         ->at(idx);
    tau.JetBDTScoreSigTrans =   m_JetBDTScoreSigTrans ->at(idx);
  }

  if ( m_infoSwitch.m_EleVeto ) {
    tau.isEleBDTLoose   =  m_isEleBDTLoose   ->at(idx);
    tau.isEleBDTMedium  =  m_isEleBDTMedium  ->at(idx);
    tau.isEleBDTTight   =  m_isEleBDTTight   ->at(idx);
    
    tau.EleBDTScore     =  m_EleBDTScore     ->at(idx);
    
    tau.passEleOLR      =  m_passEleOLR      ->at(idx);
  }

  if( m_infoSwitch.m_xahTauJetMatching) {
    tau.matchedJetWidth  = m_tau_matchedJetWidth ->at(idx);
    tau.matchedJetJvt    = m_tau_matchedJetJvt ->at(idx);
  }

  if( m_infoSwitch.m_trackAll) {
    tau.tracks_pt  = m_tau_tracks_pt  ->at(idx);
    tau.tracks_eta = m_tau_tracks_eta ->at(idx);
    tau.tracks_phi = m_tau_tracks_phi ->at(idx);

    tau.tracks_isCore           = m_tau_tracks_isCore           ->at(idx);
    tau.tracks_isWide           = m_tau_tracks_isWide           ->at(idx);
    tau.tracks_failTrackFilter  = m_tau_tracks_failTrackFilter  ->at(idx);
    tau.tracks_passTrkSel       = m_tau_tracks_passTrkSel       ->at(idx);
    tau.tracks_isClCharged      = m_tau_tracks_isClCharged      ->at(idx);
    tau.tracks_isClIso          = m_tau_tracks_isClIso          ->at(idx);
    tau.tracks_isClConv         = m_tau_tracks_isClConv         ->at(idx);
    tau.tracks_isClFake         = m_tau_tracks_isClFake         ->at(idx);
  }

}


void TauContainer::setBranches(TTree *tree)
{

  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_trigger ){
    // this is true if there's a match for at least one trigger chain
    setBranch<int>(tree,"isTrigMatched", m_isTrigMatched);
    // a vector of trigger match decision for each tau trigger chain
    setBranch<vector<int> >(tree,"isTrigMatchedToChain", m_isTrigMatchedToChain );
    // a vector of strings for each tau trigger chain - 1:1 correspondence w/ vector above
    setBranch<string>(tree, "listTrigChains", m_listTrigChains );
  }

  if ( m_infoSwitch.m_kinematic ){
    setBranch<int>  (tree,"ntrk", m_ntrk);
    setBranch<float>(tree,"charge", m_charge );

  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& taueff : m_infoSwitch.m_tauEffWPs) {
      tree->Branch( (m_name + "_TauEff_SF_" + taueff).c_str() , & (*m_TauEff_SF)[ taueff ] );
    }
    
    for (auto& trig : m_infoSwitch.m_trigWPs) {
      tree->Branch( (m_name + "_TauTrigEff_SF_" + trig).c_str() , & (*m_TauTrigEff_SF)[ trig ] );
    }
  }

  // might need to delete these
  if ( m_infoSwitch.m_JetID ){
    setBranch<int>   (tree,"isJetBDTSigVeryLoose", m_isJetBDTSigVeryLoose);
    setBranch<int>   (tree,"isJetBDTSigLoose", m_isJetBDTSigLoose);
    setBranch<int>   (tree,"isJetBDTSigMedium", m_isJetBDTSigMedium);
    setBranch<int>   (tree,"isJetBDTSigTight", m_isJetBDTSigTight);
    
    setBranch<float> (tree,"JetBDTScore", m_JetBDTScore);
    setBranch<float> (tree,"JetBDTScoreSigTrans", m_JetBDTScoreSigTrans);

  }
  
  if ( m_infoSwitch.m_EleVeto ){
    setBranch<int>   (tree,"isEleBDTLoose", m_isEleBDTLoose);
    setBranch<int>   (tree,"isEleBDTMedium", m_isEleBDTMedium);
    setBranch<int>   (tree,"isEleBDTTight", m_isEleBDTTight);
    
    setBranch<float> (tree,"EleBDTScore", m_EleBDTScore);

    setBranch<int>   (tree,"passEleOLR", m_passEleOLR);
  }
 
  if( m_infoSwitch.m_xahTauJetMatching) {
    setBranch<float>  (tree, "matchedJetWidth",    m_tau_matchedJetWidth);
    setBranch<float>  (tree, "matchedJetJvt",    m_tau_matchedJetJvt);
  } 
  
  if( m_infoSwitch.m_trackAll) {
    tree->Branch( (m_name + "_tracks_pt").c_str() , &m_tau_tracks_pt );
    tree->Branch( (m_name + "_tracks_eta").c_str() , &m_tau_tracks_eta );
    tree->Branch( (m_name + "_tracks_phi").c_str() , &m_tau_tracks_phi );
    
    tree->Branch( (m_name + "_tracks_isCore").c_str() , &m_tau_tracks_isCore );
    tree->Branch( (m_name + "_tracks_isWide").c_str() , &m_tau_tracks_isWide );
    tree->Branch( (m_name + "_tracks_failTrackFilter").c_str() , &m_tau_tracks_failTrackFilter );
    tree->Branch( (m_name + "_tracks_passTrkSel").c_str() , &m_tau_tracks_passTrkSel );
    tree->Branch( (m_name + "_tracks_isClCharged").c_str() , &m_tau_tracks_isClCharged );
    tree->Branch( (m_name + "_tracks_isClIso").c_str() , &m_tau_tracks_isClIso );
    tree->Branch( (m_name + "_tracks_isClConv").c_str() , &m_tau_tracks_isClConv );
    tree->Branch( (m_name + "_tracks_isClFake").c_str() , &m_tau_tracks_isClFake );
  }

  return;
}



void TauContainer::clear()
{
  
  ParticleContainer::clear();

  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched->clear();
    m_isTrigMatchedToChain->clear();
    m_listTrigChains->clear();
  }

  if ( m_infoSwitch.m_kinematic ) {
    m_ntrk->clear();
    m_charge->clear();
  }
 
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& taueff : m_infoSwitch.m_tauEffWPs) {
      (*m_TauEff_SF)[ taueff ].clear();
    }

    for (auto& trig : m_infoSwitch.m_trigWPs) {
      (*m_TauTrigEff_SF)[ trig ].clear();
    }
  }

  // might need to delete these
  if ( m_infoSwitch.m_JetID ) {
    m_isJetBDTSigVeryLoose->clear();
    m_isJetBDTSigLoose->clear();
    m_isJetBDTSigMedium->clear();
    m_isJetBDTSigTight->clear();
    
    m_JetBDTScore->clear();
    m_JetBDTScoreSigTrans->clear();
  }

  if ( m_infoSwitch.m_EleVeto ) {
    m_isEleBDTLoose->clear();
    m_isEleBDTMedium->clear();
    m_isEleBDTTight->clear();
    
    m_EleBDTScore->clear();
    m_passEleOLR->clear();
  }

  if( m_infoSwitch.m_xahTauJetMatching) {
    m_tau_matchedJetWidth->clear();
    m_tau_matchedJetJvt->clear();
  }

  if( m_infoSwitch.m_trackAll) {
    m_tau_tracks_pt->clear();
    m_tau_tracks_eta->clear();
    m_tau_tracks_phi->clear();

    m_tau_tracks_isCore->clear();
    m_tau_tracks_isWide->clear();
    m_tau_tracks_failTrackFilter->clear();
    m_tau_tracks_passTrkSel->clear();
    m_tau_tracks_isClCharged->clear();
    m_tau_tracks_isClIso->clear();
    m_tau_tracks_isClConv->clear();
    m_tau_tracks_isClFake->clear();
  }

}


void TauContainer::FillTau( const xAOD::TauJet* tau ){
  return FillTau(static_cast<const xAOD::IParticle*>(tau));
}

void TauContainer::FillTau( const xAOD::IParticle* particle ) 
{

  ParticleContainer::FillParticle(particle);

  const xAOD::TauJet* tau=dynamic_cast<const xAOD::TauJet*>(particle);

  if ( m_infoSwitch.m_trigger ) {

    // retrieve map<string,char> w/ <chain,isMatched>
    //
    static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapTauAcc("isTrigMatchedMapTau");

    std::vector<int> matches;

    if ( isTrigMatchedMapTauAcc.isAvailable( *tau ) ) {
      // loop over map and fill branches
      //
      for ( auto const &it : (isTrigMatchedMapTauAcc( *tau )) ) {
	matches.push_back( static_cast<int>(it.second) );
	m_listTrigChains->push_back( it.first );
      }
    } else {
      matches.push_back( -1 );
      m_listTrigChains->push_back("NONE");
    }

    m_isTrigMatchedToChain->push_back(matches);
    
    // if at least one match among the chains is found, say this tau is trigger matched
    if ( std::find(matches.begin(), matches.end(), 1) != matches.end() ) { m_isTrigMatched->push_back(1); }
    else { m_isTrigMatched->push_back(0); }
    
  }

  if ( m_infoSwitch.m_kinematic ) {

    m_charge->push_back( tau->charge()  );
    m_ntrk  ->push_back( tau->nTracks() );
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {

    std::vector<float> junkSF(1,-1.0);

    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTauEffSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTauTrigSF;
    
    for (auto& taueff : m_infoSwitch.m_tauEffWPs) {
      std::string tauEffSF = "TauEff_SF_syst_" + taueff;
      accTauEffSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( taueff , SG::AuxElement::Accessor< std::vector< float > >( tauEffSF ) ) );
      safeSFVecFill<float, xAOD::TauJet>( tau, accTauEffSF.at( taueff ), &m_TauEff_SF->at( taueff ), junkSF );
    }

    for (auto& trig : m_infoSwitch.m_trigWPs) {
      std::string trigEffSF = "TauEff_SF_syst_" + trig;
      accTauTrigSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig , SG::AuxElement::Accessor< std::vector< float > >( trigEffSF ) ) );
      safeSFVecFill<float, xAOD::TauJet>( tau, accTauTrigSF.at( trig ), &m_TauTrigEff_SF->at( trig ), junkSF );
    }
  
  }

  // might need to delete these
  if ( m_infoSwitch.m_JetID ) {
    
    static SG::AuxElement::Accessor<int> isJetBDTSigVeryLooseAcc ("isJetBDTSigVeryLoose");
    safeFill<int, int, xAOD::TauJet>(tau, isJetBDTSigVeryLooseAcc, m_isJetBDTSigVeryLoose, -1);

    static SG::AuxElement::Accessor<int> isJetBDTSigLooseAcc ("isJetBDTSigLoose");
    safeFill<int, int, xAOD::TauJet>(tau, isJetBDTSigLooseAcc, m_isJetBDTSigLoose, -1);

    static SG::AuxElement::Accessor<int> isJetBDTSigMediumAcc ("isJetBDTSigMedium");
    safeFill<int, int, xAOD::TauJet>(tau, isJetBDTSigMediumAcc, m_isJetBDTSigMedium, -1);

    static SG::AuxElement::Accessor<int> isJetBDTSigTightAcc ("isJetBDTSigTight");
    safeFill<int, int, xAOD::TauJet>(tau, isJetBDTSigTightAcc, m_isJetBDTSigTight, -1);
    
    static SG::AuxElement::Accessor<float> JetBDTScoreAcc ("JetBDTScore");
    safeFill<float, float, xAOD::TauJet>(tau, JetBDTScoreAcc, m_JetBDTScore, -999.);

    static SG::AuxElement::Accessor<float> JetBDTScoreSigTransAcc ("JetBDTScoreSigTrans");
    safeFill<float, float, xAOD::TauJet>(tau, JetBDTScoreSigTransAcc, m_JetBDTScoreSigTrans, -999.);
  }

  if ( m_infoSwitch.m_EleVeto ) {
    
    static SG::AuxElement::Accessor<int> isEleBDTLooseAcc ("isEleBDTLoose");
    safeFill<int, int, xAOD::TauJet>(tau, isEleBDTLooseAcc, m_isEleBDTLoose, -1);

    static SG::AuxElement::Accessor<int> isEleBDTMediumAcc ("isEleBDTMedium");
    safeFill<int, int, xAOD::TauJet>(tau, isEleBDTMediumAcc, m_isEleBDTMedium, -1);

    static SG::AuxElement::Accessor<int> isEleBDTTightAcc ("isEleBDTTight");
    safeFill<int, int, xAOD::TauJet>(tau, isEleBDTTightAcc, m_isEleBDTTight, -1);

    static SG::AuxElement::Accessor<float> EleBDTScoreAcc ("EleBDTScore");
    safeFill<float, float, xAOD::TauJet>(tau, EleBDTScoreAcc, m_EleBDTScore, -999.);

    static SG::AuxElement::Accessor<int> passEleOLRAcc ("passEleOLR");
    safeFill<int, int, xAOD::TauJet>(tau, passEleOLRAcc, m_passEleOLR, -1);
  }

  if( m_infoSwitch.m_xahTauJetMatching) {
    static SG::AuxElement::Accessor< float > jetWidthAcc("JetWidth");
    safeFill<float, float, xAOD::TauJet>(tau, jetWidthAcc, m_tau_matchedJetWidth, -1.);
    static SG::AuxElement::Accessor< float > jetJvtAcc("JetJvt");
    safeFill<float, float, xAOD::TauJet>(tau, jetJvtAcc, m_tau_matchedJetJvt, -1.);
  }

  if( m_infoSwitch.m_trackAll) {
    static SG::AuxElement::ConstAccessor< std::vector<float>   >   tauTrackPtAcc("trackPt");
    safeVecFill<float, float, xAOD::TauJet>(tau, tauTrackPtAcc, m_tau_tracks_pt);
    
    static SG::AuxElement::ConstAccessor< std::vector<float>   >   tauTrackEtaAcc("trackEta");
    safeVecFill<float, float, xAOD::TauJet>(tau, tauTrackEtaAcc, m_tau_tracks_eta);
    
    static SG::AuxElement::ConstAccessor< std::vector<float>   >   tauTrackPhiAcc("trackPhi");
    safeVecFill<float, float, xAOD::TauJet>(tau, tauTrackPhiAcc, m_tau_tracks_phi);
    
    // track classification
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackIsCoreAcc("trackIsCore");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackIsCoreAcc, m_tau_tracks_isCore);
    
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackIsWideAcc("trackIsWide");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackIsWideAcc, m_tau_tracks_isWide);
    
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackFailTrackFilterAcc("trackFailTrackFilter");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackFailTrackFilterAcc, m_tau_tracks_failTrackFilter);
    
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackPassTrkSelAcc("trackPassTrkSel");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackPassTrkSelAcc, m_tau_tracks_passTrkSel);
    
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackIsClChargedAcc("trackIsClCharged");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackIsClChargedAcc, m_tau_tracks_isClCharged);
    
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackIsClIsoAcc("trackIsClIso");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackIsClIsoAcc, m_tau_tracks_isClIso);
    
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackIsClConvAcc("trackIsClConv");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackIsClConvAcc, m_tau_tracks_isClConv);
    
    static SG::AuxElement::ConstAccessor< std::vector<int>   >   tauTrackIsClFakeAcc("trackIsClFake");
    safeVecFill<int, int, xAOD::TauJet>(tau, tauTrackIsClFakeAcc, m_tau_tracks_isClFake);
  }

  return;
}
