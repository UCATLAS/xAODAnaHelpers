#include "xAODAnaHelpers/TauContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

TauContainer::TauContainer(const std::string& name, const std::string& detailStr, float units, bool mc)
  : ParticleContainer(name, detailStr, units, mc, true)
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
  
  if( m_infoSwitch.m_JetID) {
    m_isJetBDTVeryLoose = new  std::vector<int>   ();
    m_isJetBDTLoose     = new  std::vector<int>   ();
    m_isJetBDTMedium    = new  std::vector<int>   ();
    m_isJetBDTTight     = new  std::vector<int>   ();
    
    m_JetBDTScore     = new  std::vector<float>   ();
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

  if( m_infoSwitch.m_JetID) {
    delete m_isJetBDTVeryLoose;
    delete m_isJetBDTLoose;
    delete m_isJetBDTMedium;
    delete m_isJetBDTTight;
    
    delete m_JetBDTScore;
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
  
  if ( m_infoSwitch.m_JetID ){
    connectBranch<int>    (tree, "isJetBDTVeryLoose",   &m_isJetBDTVeryLoose);
    connectBranch<int>    (tree, "isJetBDTLoose",       &m_isJetBDTLoose);
    connectBranch<int>    (tree, "isJetBDTMedium",      &m_isJetBDTMedium);
    connectBranch<int>    (tree, "isJetBDTTight",       &m_isJetBDTTight);
    
    connectBranch<float>  (tree, "JetBDTScore",       &m_JetBDTScore);
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
  
  // JetID
  if ( m_infoSwitch.m_JetID ) {
    tau.isJetBDTVeryLoose =   m_isJetBDTVeryLoose ->at(idx);
    tau.isJetBDTLoose     =   m_isJetBDTLoose     ->at(idx);
    tau.isJetBDTMedium    =   m_isJetBDTMedium    ->at(idx);
    tau.isJetBDTTight     =   m_isJetBDTTight     ->at(idx);
    tau.JetBDTScore    =   m_JetBDTScore    ->at(idx);
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
  
  if ( m_infoSwitch.m_JetID ){
    setBranch<int>   (tree,"isJetBDTVeryLoose", m_isJetBDTVeryLoose);
    setBranch<int>   (tree,"isJetBDTLoose", m_isJetBDTLoose);
    setBranch<int>   (tree,"isJetBDTMedium", m_isJetBDTMedium);
    setBranch<int>   (tree,"isJetBDTTight", m_isJetBDTTight);
    setBranch<float> (tree,"JetBDTScore", m_JetBDTScore);

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
  
  if ( m_infoSwitch.m_JetID ) {
    m_isJetBDTVeryLoose->clear();
    m_isJetBDTLoose->clear();
    m_isJetBDTMedium->clear();
    m_isJetBDTTight->clear();
    m_JetBDTScore->clear();
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


  if ( m_infoSwitch.m_JetID ) {
    
    static SG::AuxElement::Accessor<char> isJetBDTVeryLooseAcc ("isJetBDTVeryLoose");
    safeFill<char, int, xAOD::TauJet>(tau, isJetBDTVeryLooseAcc, m_isJetBDTVeryLoose, -1);

    static SG::AuxElement::Accessor<char> isJetBDTLooseAcc ("isJetBDTLoose");
    safeFill<char, int, xAOD::TauJet>(tau, isJetBDTLooseAcc, m_isJetBDTLoose, -1);

    static SG::AuxElement::Accessor<char> isJetBDTMediumAcc ("isJetBDTMedium");
    safeFill<char, int, xAOD::TauJet>(tau, isJetBDTMediumAcc, m_isJetBDTMedium, -1);

    static SG::AuxElement::Accessor<char> isJetBDTTightAcc ("isJetBDTTight");
    safeFill<char, int, xAOD::TauJet>(tau, isJetBDTTightAcc, m_isJetBDTTight, -1);
    
    static SG::AuxElement::Accessor<char> JetBDTScoreAcc ("JetBDTScore");
    safeFill<char, float, xAOD::TauJet>(tau, JetBDTScoreAcc, m_JetBDTScore, -999.);

  }
  
  return;
}
