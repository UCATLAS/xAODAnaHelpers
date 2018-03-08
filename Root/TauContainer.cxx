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
  
  // isolation
  if ( m_infoSwitch.m_identification ) {
    m_isIdentified = new std::map< std::string, std::vector< int > >();
  }
  
  // might need to delete these  
  if( m_infoSwitch.m_JetID) {
    m_isJetBDTSigVeryLoose = new  std::vector<int>   ();
    m_isJetBDTSigLoose     = new  std::vector<int>   ();
    m_isJetBDTSigMedium    = new  std::vector<int>   ();
    m_isJetBDTSigTight     = new  std::vector<int>   ();
    
    m_isJetBDTBkgLoose     = new  std::vector<int>   ();
    m_isJetBDTBkgMedium    = new  std::vector<int>   ();
    m_isJetBDTBkgTight     = new  std::vector<int>   ();
    
    m_JetBDTScore          = new  std::vector<float>   ();
    m_JetBDTScoreSigTrans  = new  std::vector<float>   ();
  }

  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    m_TauIDEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
    m_EleOLRElectronEff_SF  = new std::map< std::string, std::vector< std::vector< float > > >();
    m_TrigEff_SF = new std::map< std::string, std::vector< std::vector< float > > >();
  
    m_RecoEff_SF = new vector< vector< float > > ();
    m_EleOLRHadTauEff_SF = new vector< vector< float > > ();
  
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

  // tau identification
  if ( m_infoSwitch.m_identification ) {
    delete m_isIdentified;
  }

  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    delete m_RecoEff_SF;
    delete m_TauIDEff_SF;
    delete m_TrigEff_SF;
    
    delete m_EleOLRElectronEff_SF;
    delete m_EleOLRHadTauEff_SF;

  }

  // might need to delete these
  if( m_infoSwitch.m_JetID) {
    delete m_isJetBDTSigVeryLoose;
    delete m_isJetBDTSigLoose;
    delete m_isJetBDTSigMedium;
    delete m_isJetBDTSigTight;
    
    delete m_isJetBDTBkgLoose;
    delete m_isJetBDTBkgMedium;
    delete m_isJetBDTBkgTight;
    
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

  if ( m_infoSwitch.m_identification ) {
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      if (!tauid.empty()) {
        tree->SetBranchStatus ( (m_name + "_isIdentified_" + tauid).c_str() , 1);
        tree->SetBranchAddress( (m_name + "_isIdentified_" + tauid).c_str() , & (*m_isIdentified)[ tauid ] );
      }
    }
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      tree->SetBranchStatus ( (m_name + "_TauIDEff_SF_" + tauid).c_str() , 1);
      tree->SetBranchAddress( (m_name + "_TauIDEff_SF_" + tauid).c_str() , & (*m_TauIDEff_SF)[ tauid ] );

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tree->SetBranchStatus ( (m_name + "_TrigEff_SF_" + trig + "_TauID" + tauid).c_str() , 1 );
        tree->SetBranchAddress( (m_name + "_TrigEff_SF_" + trig + "_TauID" + tauid).c_str() , & (*m_TrigEff_SF)[ trig+tauid ] );

      }
    }
    
    for (auto& eolr : m_infoSwitch.m_eOLRWPs) {
      if (!eolr.empty()) {
        tree->SetBranchStatus ( (m_name + "_EleOLRElectronEff_SF_" + eolr).c_str() , 1);
        tree->SetBranchAddress( (m_name + "_EleOLRElectronEff_SF_" + eolr).c_str() , & (*m_EleOLRElectronEff_SF)[ eolr ] );
      }
    }

    connectBranch<vector<float> >(tree,"RecoEff_SF",  &m_RecoEff_SF);
    connectBranch<vector<float> >(tree,"EleOLRHadTauEff_SF",  &m_EleOLRHadTauEff_SF);
  
  }

  // might need to delete these 
  if ( m_infoSwitch.m_JetID ){
    connectBranch<int>    (tree, "isJetBDTSigVeryLoose",   &m_isJetBDTSigVeryLoose);
    connectBranch<int>    (tree, "isJetBDTSigLoose",       &m_isJetBDTSigLoose);
    connectBranch<int>    (tree, "isJetBDTSigMedium",      &m_isJetBDTSigMedium);
    connectBranch<int>    (tree, "isJetBDTSigTight",       &m_isJetBDTSigTight);
    
    connectBranch<int>    (tree, "isJetBDTBkgLoose",       &m_isJetBDTBkgLoose);
    connectBranch<int>    (tree, "isJetBDTBkgMedium",      &m_isJetBDTBkgMedium);
    connectBranch<int>    (tree, "isJetBDTBkgTight",       &m_isJetBDTBkgTight);
    
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
 
  // tau identification
  if ( m_infoSwitch.m_identification ) {
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      if (!tauid.empty()) {
        tau.isIdentified[tauid] = (*m_isIdentified)[ tauid ].at(idx);
      }
    }
  }

  // scale factors w/ sys
  // per object
  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      tau.TauIDEff_SF[ tauid ] = (*m_TauIDEff_SF)[ tauid ].at(idx);

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tau.TrigEff_SF[ trig+tauid ] = (*m_TrigEff_SF)[ trig+tauid ].at(idx);
      }
    }

    for (auto& eolr : m_infoSwitch.m_eOLRWPs) {
      tau.EleOLRElectronEff_SF[ eolr ] = (*m_EleOLRElectronEff_SF)[ eolr ].at(idx);
    }

    tau.RecoEff_SF = m_RecoEff_SF -> at(idx);
    tau.EleOLRHadTauEff_SF = m_EleOLRHadTauEff_SF -> at(idx);
  }

  //  might need to delete these
  if ( m_infoSwitch.m_JetID ) {
    tau.isJetBDTSigVeryLoose   =   m_isJetBDTSigVeryLoose   ->at(idx);
    tau.isJetBDTSigLoose       =   m_isJetBDTSigLoose       ->at(idx);
    tau.isJetBDTSigMedium      =   m_isJetBDTSigMedium      ->at(idx);
    tau.isJetBDTSigTight       =   m_isJetBDTSigTight       ->at(idx);
    
    tau.isJetBDTBkgLoose       =   m_isJetBDTBkgLoose       ->at(idx);
    tau.isJetBDTBkgMedium      =   m_isJetBDTBkgMedium      ->at(idx);
    tau.isJetBDTBkgTight       =   m_isJetBDTBkgTight       ->at(idx);
    
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
 
  if ( m_infoSwitch.m_identification ) {
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      if (!tauid.empty()) {
        setBranch<int>(tree, "isIdentified_" + tauid, &(*m_isIdentified)[tauid]);
      }
    }
  }


  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      tree->Branch( (m_name + "_TauIDEff_SF_" + tauid).c_str() , & (*m_TauIDEff_SF)[ tauid ] );
      
      for (auto& trig : m_infoSwitch.m_trigWPs) {
        tree->Branch( (m_name + "_TrigEff_SF_" + trig + "_TauID" + tauid).c_str() , & (*m_TrigEff_SF)[ trig+tauid ] );
      }
    }
    
    for (auto& eolr : m_infoSwitch.m_eOLRWPs) {
      tree->Branch( (m_name + "_EleOLRElectronEff_SF_" + eolr).c_str() , & (*m_EleOLRElectronEff_SF)[ eolr ] );
    }
    
    setBranch<vector<float> >(tree,"RecoEff_SF",  m_RecoEff_SF);
    setBranch<vector<float> >(tree,"EleOLRHadTauEff_SF",  m_EleOLRHadTauEff_SF);
    
  }

  // might need to delete these
  if ( m_infoSwitch.m_JetID ){
    setBranch<int>   (tree,"isJetBDTSigVeryLoose", m_isJetBDTSigVeryLoose);
    setBranch<int>   (tree,"isJetBDTSigLoose", m_isJetBDTSigLoose);
    setBranch<int>   (tree,"isJetBDTSigMedium", m_isJetBDTSigMedium);
    setBranch<int>   (tree,"isJetBDTSigTight", m_isJetBDTSigTight);
    
    setBranch<int>   (tree,"isJetBDTBkgLoose", m_isJetBDTBkgLoose);
    setBranch<int>   (tree,"isJetBDTBkgMedium", m_isJetBDTBkgMedium);
    setBranch<int>   (tree,"isJetBDTBkgTight", m_isJetBDTBkgTight);
    
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
 
  if ( m_infoSwitch.m_identification ) {
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      (*m_isIdentified)[ tauid ].clear();
    }
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {
    
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      (*m_TauIDEff_SF)[ tauid ].clear();

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        (*m_TrigEff_SF)[ trig+tauid ].clear();
      }
    }

    for (auto& eolr : m_infoSwitch.m_eOLRWPs) {
      (*m_EleOLRElectronEff_SF)[ eolr ].clear();
    }

    m_RecoEff_SF->clear();
    m_EleOLRHadTauEff_SF->clear();
  }

  // might need to delete these
  if ( m_infoSwitch.m_JetID ) {
    m_isJetBDTSigVeryLoose->clear();
    m_isJetBDTSigLoose->clear();
    m_isJetBDTSigMedium->clear();
    m_isJetBDTSigTight->clear();
    
    m_isJetBDTBkgLoose->clear();
    m_isJetBDTBkgMedium->clear();
    m_isJetBDTBkgTight->clear();
    
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

  if ( m_infoSwitch.m_identification ) {
    static std::map< std::string, SG::AuxElement::Accessor<char> > accTauID;

    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      if (!tauid.empty()) {
        std::string tauIDWP = "isIdentified_" + tauid;
        accTauID.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( tauid , SG::AuxElement::Accessor<char>( tauIDWP ) ) );
        safeFill<char, int, xAOD::TauJet>( tau, accTauID.at( tauid ), &m_isIdentified->at( tauid ), -1 );
      }
    }
  }

  if ( m_infoSwitch.m_effSF && m_mc ) {

    std::vector<float> junkSF(1,1.0);

    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTauIDSF;
    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accTrigSF;
    for (auto& tauid : m_infoSwitch.m_tauIDWPs) {
      std::string tauIDEffSF = "TauIDEff_SF_syst_TauID" + tauid;
      accTauIDSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( tauid , SG::AuxElement::Accessor< std::vector< float > >( tauIDEffSF ) ) );
      safeSFVecFill<float, xAOD::TauJet>( tau, accTauIDSF.at( tauid ), &m_TauIDEff_SF->at( tauid ), junkSF );

      for (auto& trig : m_infoSwitch.m_trigWPs) {
        std::string trigEffSF = "TauTrigEff_SF_syst_" + trig + "_TauID" + tauid;
        accTrigSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( trig+tauid , SG::AuxElement::Accessor< std::vector< float > >( trigEffSF ) ) );
        safeSFVecFill<float, xAOD::TauJet>( tau, accTrigSF.at( trig+tauid ), &m_TrigEff_SF->at( trig+tauid ), junkSF );

      }
    }

    static std::map< std::string, SG::AuxElement::Accessor< std::vector< float > > > accEleOLRElectronSF;
    for (auto& eolr : m_infoSwitch.m_eOLRWPs) {
      std::string eOLREffSF = "TauEleOLRElectronEff_SF_syst_" + eolr;
      accEleOLRElectronSF.insert( std::pair<std::string, SG::AuxElement::Accessor< std::vector< float > > > ( eolr , SG::AuxElement::Accessor< std::vector< float > >( eOLREffSF ) ) );
      safeSFVecFill<float, xAOD::TauJet>( tau, accEleOLRElectronSF.at( eolr ), &m_EleOLRElectronEff_SF->at( eolr ), junkSF );
    }

    static SG::AuxElement::Accessor< std::vector< float > > accRecoSF("TauRecoEff_SF_syst_Reco");
    safeSFVecFill<float, xAOD::TauJet>( tau, accRecoSF, m_RecoEff_SF, junkSF );

    static SG::AuxElement::Accessor< std::vector< float > > accEleOLRHadTauSF("TauEleOLRHadTauEff_SF_syst_EleOLRHadTau");
    safeSFVecFill<float, xAOD::TauJet>( tau, accEleOLRHadTauSF, m_EleOLRHadTauEff_SF, junkSF );

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
    
    static SG::AuxElement::Accessor<int> isJetBDTBkgLooseAcc ("isJetBDTBkgLoose");
    safeFill<int, int, xAOD::TauJet>(tau, isJetBDTBkgLooseAcc, m_isJetBDTBkgLoose, -1);
    
    static SG::AuxElement::Accessor<int> isJetBDTBkgMediumAcc ("isJetBDTBkgMedium");
    safeFill<int, int, xAOD::TauJet>(tau, isJetBDTBkgMediumAcc, m_isJetBDTBkgMedium, -1);
   
    static SG::AuxElement::Accessor<int> isJetBDTBkgTightAcc ("isJetBDTBkgTight");
    safeFill<int, int, xAOD::TauJet>(tau, isJetBDTBkgTightAcc, m_isJetBDTBkgTight, -1);

    static SG::AuxElement::Accessor<float> JetBDTScoreAcc ("JetBDTScore");
    safeFill<float, float, xAOD::TauJet>(tau, JetBDTScoreAcc, m_JetBDTScore, -999.);

    static SG::AuxElement::Accessor<float> JetBDTScoreSigTransAcc ("JetBDTScoreSigTrans");
    safeFill<float, float, xAOD::TauJet>(tau, JetBDTScoreSigTransAcc, m_JetBDTScoreSigTrans, -999.);
  }
  
  return;
}
