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

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tree->SetBranchStatus ( (m_name + "_TauTrigEff_SF_" + trig + "_" + taueff).c_str() , 1 );
        tree->SetBranchAddress( (m_name + "_TauTrigEff_SF_" + trig + "_" + taueff).c_str() , & (*m_TauTrigEff_SF)[ trig+taueff ] );

      }
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

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tau.TauTrigEff_SF[ trig+taueff ] = (*m_TauTrigEff_SF)[ trig+taueff ].at(idx);
      }
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
      
      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tree->Branch( (m_name + "_TauTrigEff_SF_" + trig + "_" + taueff).c_str() , & (*m_TauTrigEff_SF)[ trig+taueff ] );
      }
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

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        (*m_TauTrigEff_SF)[ trig+taueff ].clear();
      }
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

    std::vector<float> junkSF(1,1.0);

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
  
  return;
}
