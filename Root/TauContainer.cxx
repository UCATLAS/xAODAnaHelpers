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
    m_isJetRNNSigVeryLoose = new  std::vector<int>   ();
    m_isJetRNNSigLoose     = new  std::vector<int>   ();
    m_isJetRNNSigMedium    = new  std::vector<int>   ();
    m_isJetRNNSigTight     = new  std::vector<int>   ();
    
    m_JetRNNScore          = new  std::vector<float>   ();
    m_JetRNNScoreSigTrans  = new  std::vector<float>   ();
  }

  if( m_infoSwitch.m_EleVeto) {
    m_isEleRNNLoose  = new  std::vector<int>   ();
    m_isEleRNNMedium = new  std::vector<int>   ();
    m_isEleRNNTight  = new  std::vector<int>   ();
    
    m_EleRNNScore    = new  std::vector<float> ();
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
    delete m_isJetRNNSigVeryLoose;
    delete m_isJetRNNSigLoose;
    delete m_isJetRNNSigMedium;
    delete m_isJetRNNSigTight;
    
    delete m_JetRNNScore;
    delete m_JetRNNScoreSigTrans;
  }

  if( m_infoSwitch.m_EleVeto) {
    delete m_isEleRNNLoose;
    delete m_isEleRNNMedium;
    delete m_isEleRNNTight;
    
    delete m_EleRNNScore;

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
    connectBranch<int>    (tree, "isJetRNNSigVeryLoose",   &m_isJetRNNSigVeryLoose);
    connectBranch<int>    (tree, "isJetRNNSigLoose",       &m_isJetRNNSigLoose);
    connectBranch<int>    (tree, "isJetRNNSigMedium",      &m_isJetRNNSigMedium);
    connectBranch<int>    (tree, "isJetRNNSigTight",       &m_isJetRNNSigTight);
    
    connectBranch<float>  (tree, "JetRNNScore",         &m_JetRNNScore);
    connectBranch<float>  (tree, "JetRNNScoreSigTrans", &m_JetRNNScoreSigTrans);
  }

  if ( m_infoSwitch.m_EleVeto ){
    connectBranch<int>    (tree, "isEleRNNLoose",    &m_isEleRNNLoose);
    connectBranch<int>    (tree, "isEleRNNMedium",   &m_isEleRNNMedium);
    connectBranch<int>    (tree, "isEleRNNTight",    &m_isEleRNNTight);
    
    connectBranch<float>  (tree, "EleRNNScore",    &m_EleRNNScore);
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
    tau.isJetRNNSigVeryLoose   =   m_isJetRNNSigVeryLoose   ->at(idx);
    tau.isJetRNNSigLoose       =   m_isJetRNNSigLoose       ->at(idx);
    tau.isJetRNNSigMedium      =   m_isJetRNNSigMedium      ->at(idx);
    tau.isJetRNNSigTight       =   m_isJetRNNSigTight       ->at(idx);
    
    tau.JetRNNScore         =   m_JetRNNScore         ->at(idx);
    tau.JetRNNScoreSigTrans =   m_JetRNNScoreSigTrans ->at(idx);
  }

  if ( m_infoSwitch.m_EleVeto ) {
    tau.isEleRNNLoose   =  m_isEleRNNLoose   ->at(idx);
    tau.isEleRNNMedium  =  m_isEleRNNMedium  ->at(idx);
    tau.isEleRNNTight   =  m_isEleRNNTight   ->at(idx);
    
    tau.EleRNNScore     =  m_EleRNNScore     ->at(idx);
    
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
    setBranch<int>   (tree,"isJetRNNSigVeryLoose", m_isJetRNNSigVeryLoose);
    setBranch<int>   (tree,"isJetRNNSigLoose", m_isJetRNNSigLoose);
    setBranch<int>   (tree,"isJetRNNSigMedium", m_isJetRNNSigMedium);
    setBranch<int>   (tree,"isJetRNNSigTight", m_isJetRNNSigTight);
    
    setBranch<float> (tree,"JetRNNScore", m_JetRNNScore);
    setBranch<float> (tree,"JetRNNScoreSigTrans", m_JetRNNScoreSigTrans);

  }
  
  if ( m_infoSwitch.m_EleVeto ){
    setBranch<int>   (tree,"isEleRNNLoose", m_isEleRNNLoose);
    setBranch<int>   (tree,"isEleRNNMedium", m_isEleRNNMedium);
    setBranch<int>   (tree,"isEleRNNTight", m_isEleRNNTight);
    
    setBranch<float> (tree,"EleRNNScore", m_EleRNNScore);

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
    m_isJetRNNSigVeryLoose->clear();
    m_isJetRNNSigLoose->clear();
    m_isJetRNNSigMedium->clear();
    m_isJetRNNSigTight->clear();
    
    m_JetRNNScore->clear();
    m_JetRNNScoreSigTrans->clear();
  }

  if ( m_infoSwitch.m_EleVeto ) {
    m_isEleRNNLoose->clear();
    m_isEleRNNMedium->clear();
    m_isEleRNNTight->clear();
    
    m_EleRNNScore->clear();
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
    
    static SG::AuxElement::Accessor<int> isJetRNNSigVeryLooseAcc ("isJetRNNSigVeryLoose");
    safeFill<int, int, xAOD::TauJet>(tau, isJetRNNSigVeryLooseAcc, m_isJetRNNSigVeryLoose, -1);

    static SG::AuxElement::Accessor<int> isJetRNNSigLooseAcc ("isJetRNNSigLoose");
    safeFill<int, int, xAOD::TauJet>(tau, isJetRNNSigLooseAcc, m_isJetRNNSigLoose, -1);

    static SG::AuxElement::Accessor<int> isJetRNNSigMediumAcc ("isJetRNNSigMedium");
    safeFill<int, int, xAOD::TauJet>(tau, isJetRNNSigMediumAcc, m_isJetRNNSigMedium, -1);

    static SG::AuxElement::Accessor<int> isJetRNNSigTightAcc ("isJetRNNSigTight");
    safeFill<int, int, xAOD::TauJet>(tau, isJetRNNSigTightAcc, m_isJetRNNSigTight, -1);
    
    static SG::AuxElement::Accessor<float> JetRNNScoreAcc ("JetRNNScore");
    safeFill<float, float, xAOD::TauJet>(tau, JetRNNScoreAcc, m_JetRNNScore, -999.);

    static SG::AuxElement::Accessor<float> JetRNNScoreSigTransAcc ("JetRNNScoreSigTrans");
    safeFill<float, float, xAOD::TauJet>(tau, JetRNNScoreSigTransAcc, m_JetRNNScoreSigTrans, -999.);
  }

  if ( m_infoSwitch.m_EleVeto ) {
    
    static SG::AuxElement::Accessor<int> isEleRNNLooseAcc ("isEleRNNLoose");
    safeFill<int, int, xAOD::TauJet>(tau, isEleRNNLooseAcc, m_isEleRNNLoose, -1);

    static SG::AuxElement::Accessor<int> isEleRNNMediumAcc ("isEleRNNMedium");
    safeFill<int, int, xAOD::TauJet>(tau, isEleRNNMediumAcc, m_isEleRNNMedium, -1);

    static SG::AuxElement::Accessor<int> isEleRNNTightAcc ("isEleRNNTight");
    safeFill<int, int, xAOD::TauJet>(tau, isEleRNNTightAcc, m_isEleRNNTight, -1);

    static SG::AuxElement::Accessor<float> EleRNNScoreAcc ("EleRNNScore");
    safeFill<float, float, xAOD::TauJet>(tau, EleRNNScoreAcc, m_EleRNNScore, -999.);

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
