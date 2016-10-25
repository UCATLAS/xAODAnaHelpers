#include "xAODAnaHelpers/FatJetContainer.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <iostream>
#include "xAODTruth/TruthEventContainer.h"

using namespace xAH;
using std::vector;  using std::endl;  using std::cout;


FatJetContainer::FatJetContainer(const std::string& name, const std::string& detailStr, const std::string& suffix, 
				 float units, bool mc)
  : ParticleContainer(name,detailStr,units,mc, false, false, suffix), 
    m_trackJetPtCut(10e3),
    m_trackJetEtaCut(2.5)
{

  if (m_infoSwitch.m_scales) {
      m_JetConstitScaleMomentum_eta       = new std::vector<float>();
      m_JetConstitScaleMomentum_phi       = new std::vector<float>();
      m_JetConstitScaleMomentum_m       = new std::vector<float>();
      m_JetConstitScaleMomentum_pt        = new std::vector<float>();

      m_JetEMScaleMomentum_eta        = new std::vector<float>();
      m_JetEMScaleMomentum_phi        = new std::vector<float>();
      m_JetEMScaleMomentum_m        = new std::vector<float>();
      m_JetEMScaleMomentum_pt       = new std::vector<float>();
  }

  if (m_infoSwitch.m_area) {
    m_GhostArea = new std::vector<float>();
    m_ActiveArea = new std::vector<float>();
    m_VoronoiArea = new std::vector<float>();

    m_ActiveArea4vec_pt = new std::vector<float>();
    m_ActiveArea4vec_eta = new std::vector<float>();
    m_ActiveArea4vec_phi = new std::vector<float>();
    m_ActiveArea4vec_m = new std::vector<float>();
  }

  if ( m_infoSwitch.m_substructure ) {
    m_Split12           = new std::vector<float>();
    m_Split23           = new std::vector<float>();
    m_Split34           = new std::vector<float>();
    m_tau1_wta          = new std::vector<float>();
    m_tau2_wta          = new std::vector<float>();
    m_tau3_wta          = new std::vector<float>();
    m_tau21_wta         = new std::vector<float>();
    m_tau32_wta         = new std::vector<float>();
    m_ECF1              = new std::vector<float>();
    m_ECF2              = new std::vector<float>();
    m_ECF3              = new std::vector<float>();
    m_C2                = new std::vector<float>();
    m_D2                = new std::vector<float>();
    m_NTrimSubjets      = new std::vector<float>();
    m_NClusters         = new std::vector<int>  ();
    m_nTracks           = new std::vector<int>  ();

  }

  if ( m_infoSwitch.m_constituent) {
    m_numConstituents    = new std::vector< int >();
  }

  if ( m_infoSwitch.m_constituentAll) {
    m_constituentWeights  = new std::vector< std::vector<float> >();  
    m_constituent_pt      = new std::vector< std::vector<float> >();  
    m_constituent_eta     = new std::vector< std::vector<float> >();  
    m_constituent_phi     = new std::vector< std::vector<float> >();  
    m_constituent_e       = new std::vector< std::vector<float> >();  
  }

  if ( m_infoSwitch.m_bosonCount) {
    m_nTQuarks  = new std::vector< int > ();
    m_nHBosons  = new std::vector< int > ();      
    m_nWBosons  = new std::vector< int > ();            
    m_nZBosons  = new std::vector< int > ();      
  }

  if ( m_infoSwitch.m_VTags ) { 
    m_WbosonTaggerMedium = new JetSubStructureUtils::BosonTag("medium", "smooth", "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_Wtagging_MC15_Prerecommendations_20150809.dat", false, false);
    m_ZbosonTaggerMedium = new JetSubStructureUtils::BosonTag("medium", "smooth", "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_Ztagging_MC15_Prerecommendations_20150809.dat", false, false);

    m_WbosonTaggerTight = new JetSubStructureUtils::BosonTag("tight", "smooth", "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_Wtagging_MC15_Prerecommendations_20150809.dat", false, false);
    m_ZbosonTaggerTight = new JetSubStructureUtils::BosonTag("tight", "smooth", "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_Ztagging_MC15_Prerecommendations_20150809.dat", false, false);

    m_Wtag_medium  = new std::vector< int > ();            
    m_Ztag_medium  = new std::vector< int > ();            

    m_Wtag_tight  = new std::vector< int > ();            
    m_Ztag_tight  = new std::vector< int > ();            
  }

  if( m_infoSwitch.m_trackJets ){
    std::string trkJetName = name;
    if( !suffix.empty() ){ trkJetName += "_"+suffix; }
    trkJetName += "_"+m_infoSwitch.m_trackJetName;
    m_trkJets = new xAH::JetContainer(trkJetName, "kinematic flavorTag constituent sfFTagFix77", m_units, m_mc);

    m_trkJetsIdx  = new std::vector<std::vector<unsigned int> > ();
  }

}

FatJetContainer::~FatJetContainer()
{
  if(m_debug) cout << " Deleting FatJetContainer "  << endl;  

  if ( m_infoSwitch.m_scales ) {
      delete m_JetConstitScaleMomentum_eta ;
      delete m_JetConstitScaleMomentum_phi ;
      delete m_JetConstitScaleMomentum_m   ;
      delete m_JetConstitScaleMomentum_pt  ;

      delete m_JetEMScaleMomentum_eta      ;
      delete m_JetEMScaleMomentum_phi      ;
      delete m_JetEMScaleMomentum_m        ;
      delete m_JetEMScaleMomentum_pt       ;
  }

  if ( m_infoSwitch.m_area ) {
    delete m_GhostArea;
    delete m_ActiveArea;
    delete m_VoronoiArea;

    delete m_ActiveArea4vec_pt;
    delete m_ActiveArea4vec_eta;
    delete m_ActiveArea4vec_phi;
    delete m_ActiveArea4vec_m;
  }

  if ( m_infoSwitch.m_substructure ) {
    delete m_Split12     ;
    delete m_Split23     ;
    delete m_Split34     ;
    delete m_tau1_wta    ;
    delete m_tau2_wta    ;
    delete m_tau3_wta    ;
    delete m_tau21_wta   ;
    delete m_tau32_wta   ;
    delete m_ECF1        ;
    delete m_ECF2        ;
    delete m_ECF3        ;
    delete m_C2          ;
    delete m_D2          ;
    delete m_NTrimSubjets;
    delete m_NClusters   ;
    delete m_nTracks   ;
  }

  if ( m_infoSwitch.m_constituent) {
    delete m_numConstituents;
  }

  if ( m_infoSwitch.m_constituentAll) {
    delete m_constituentWeights;  
    delete m_constituent_pt    ;  
    delete m_constituent_eta   ;  
    delete m_constituent_phi   ;  
    delete m_constituent_e     ;  
  }

  if ( m_infoSwitch.m_bosonCount) {
    delete m_nTQuarks;
    delete m_nHBosons;
    delete m_nWBosons;
    delete m_nZBosons;
  }

  if ( m_infoSwitch.m_VTags ) { 
    delete m_WbosonTaggerMedium;
    delete m_ZbosonTaggerMedium;

    delete m_WbosonTaggerTight;
    delete m_ZbosonTaggerTight;

    delete m_Wtag_medium;
    delete m_Ztag_medium;

    delete m_Wtag_tight;
    delete m_Ztag_tight;
  }


  if( m_infoSwitch.m_trackJets ){
    delete m_trkJets;
    delete m_trkJetsIdx;
  }

}

void FatJetContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if( m_infoSwitch.m_scales ) {
    connectBranch<float>(tree, "JetConstitScaleMomentum_eta", &m_JetConstitScaleMomentum_eta);
    connectBranch<float>(tree, "JetConstitScaleMomentum_phi", &m_JetConstitScaleMomentum_phi);
    connectBranch<float>(tree, "JetConstitScaleMomentum_m", &m_JetConstitScaleMomentum_m);
    connectBranch<float>(tree, "JetConstitScaleMomentum_pt", &m_JetConstitScaleMomentum_pt);
    
    connectBranch<float>(tree, "JetEMScaleMomentum_eta", &m_JetEMScaleMomentum_eta);
    connectBranch<float>(tree, "JetEMScaleMomentum_phi", &m_JetEMScaleMomentum_phi);
    connectBranch<float>(tree, "JetEMScaleMomentum_m", &m_JetEMScaleMomentum_m);
    connectBranch<float>(tree, "JetEMScaleMomentum_pt", &m_JetEMScaleMomentum_pt);
  }

  if ( m_infoSwitch.m_area ) {
    connectBranch<float>(tree, "m_GhostArea", &m_GhostArea);
    connectBranch<float>(tree, "m_ActiveArea", &m_ActiveArea);
    connectBranch<float>(tree, "m_VoronoiArea", &m_VoronoiArea);

    connectBranch<float>(tree, "m_ActiveArea4vec_pt", &m_ActiveArea4vec_pt);
    connectBranch<float>(tree, "m_ActiveArea4vec_eta", &m_ActiveArea4vec_eta);
    connectBranch<float>(tree, "m_ActiveArea4vec_phi", &m_ActiveArea4vec_phi);
    connectBranch<float>(tree, "m_ActiveArea4vec_m", &m_ActiveArea4vec_m);
  }

  if ( m_infoSwitch.m_substructure ) {
    connectBranch<float>(tree, "Split12",      &m_Split12);
    connectBranch<float>(tree, "Split23",      &m_Split23);
    connectBranch<float>(tree, "Split34",      &m_Split34);
    connectBranch<float>(tree, "tau1_wta",     &m_tau1_wta);
    connectBranch<float>(tree, "tau2_wta",     &m_tau2_wta);
    connectBranch<float>(tree, "tau3_wta",     &m_tau3_wta);
    connectBranch<float>(tree, "tau21_wta",    &m_tau21_wta);
    connectBranch<float>(tree, "tau32_wta",    &m_tau32_wta);
    connectBranch<float>(tree, "ECF1",         &m_ECF1);
    connectBranch<float>(tree, "ECF2",         &m_ECF2);
    connectBranch<float>(tree, "ECF3",         &m_ECF3);
    connectBranch<float>(tree, "C2",           &m_C2);
    connectBranch<float>(tree, "D2",           &m_D2);
    connectBranch<float>(tree, "NTrimSubjets", &m_NTrimSubjets);
    connectBranch<int>  (tree, "Nclusters",    &m_NClusters);
    connectBranch<int>  (tree, "nTracks",      &m_nTracks);
  }

  if ( m_infoSwitch.m_constituent) {
    connectBranch<int>  (tree, "numConstituents",    &m_numConstituents);
  }

  if ( m_infoSwitch.m_constituentAll) {
    connectBranch< vector<float> >(tree, "constituentWeights",  &m_constituentWeights);
    connectBranch< vector<float> >(tree, "constituent_pt",      &m_constituent_pt);
    connectBranch< vector<float> >(tree, "constituent_eta",     &m_constituent_eta);
    connectBranch< vector<float> >(tree, "constituent_phi",     &m_constituent_phi);
    connectBranch< vector<float> >(tree, "constituent_e",       &m_constituent_e);
  }

  if ( m_infoSwitch.m_bosonCount) {
    connectBranch< int >(tree, "nTQuarks",  &m_nTQuarks);
    connectBranch< int >(tree, "nHBosons",  &m_nHBosons);
    connectBranch< int >(tree, "nWBosons",  &m_nWBosons);
    connectBranch< int >(tree, "nZBosons",  &m_nZBosons);
  }


  if ( m_infoSwitch.m_VTags) {
    connectBranch< int >(tree, "Wtag_medium",  &m_Wtag_medium);
    connectBranch< int >(tree, "Ztag_medium",  &m_Ztag_medium);

    connectBranch< int >(tree, "Wtag_tight",   &m_Wtag_tight);
    connectBranch< int >(tree, "Ztag_tight",   &m_Ztag_tight);
  }
  
  if( m_infoSwitch.m_trackJets ){
    m_trkJets->JetContainer::setTree(tree, "MV2c10");
    connectBranch< vector<unsigned int> >(tree, "trkJetsIdx",   &m_trkJetsIdx);
  }
}

void FatJetContainer::updateParticle(uint idx, FatJet& fatjet)
{
  if(m_debug) cout << "in FatJetContainer::updateParticle " << endl;
  ParticleContainer::updateParticle(idx,fatjet);  

  if ( m_infoSwitch.m_scales ) {
      fatjet.JetConstitScaleMomentum_eta = m_JetConstitScaleMomentum_eta ->at(idx);
      fatjet.JetConstitScaleMomentum_phi = m_JetConstitScaleMomentum_phi ->at(idx);
      fatjet.JetConstitScaleMomentum_m = m_JetConstitScaleMomentum_m ->at(idx);
      fatjet.JetConstitScaleMomentum_pt = m_JetConstitScaleMomentum_pt ->at(idx);

      fatjet.JetEMScaleMomentum_eta = m_JetEMScaleMomentum_eta ->at(idx);
      fatjet.JetEMScaleMomentum_phi = m_JetEMScaleMomentum_phi ->at(idx);
      fatjet.JetEMScaleMomentum_m = m_JetEMScaleMomentum_m ->at(idx);
      fatjet.JetEMScaleMomentum_pt = m_JetEMScaleMomentum_pt ->at(idx);
  }

  if ( m_infoSwitch.m_area ) {
    fatjet.GhostArea = m_GhostArea->at(idx);
    fatjet.ActiveArea = m_ActiveArea->at(idx);
    fatjet.VoronoiArea = m_VoronoiArea->at(idx);
    fatjet.ActiveArea4vec_pt = m_ActiveArea4vec_pt->at(idx);
    fatjet.ActiveArea4vec_eta = m_ActiveArea4vec_eta->at(idx);
    fatjet.ActiveArea4vec_phi = m_ActiveArea4vec_phi->at(idx);
    fatjet.ActiveArea4vec_m = m_ActiveArea4vec_m->at(idx);
  }

  if ( m_infoSwitch.m_substructure ) {
    fatjet.Split12      = m_Split12     ->at(idx);
    fatjet.Split23      = m_Split23     ->at(idx);
    fatjet.Split34      = m_Split34     ->at(idx);
    fatjet.tau1_wta     = m_tau1_wta    ->at(idx);
    fatjet.tau2_wta     = m_tau2_wta    ->at(idx);
    fatjet.tau3_wta     = m_tau3_wta    ->at(idx);
    fatjet.tau21_wta    = m_tau21_wta   ->at(idx);
    fatjet.tau32_wta    = m_tau32_wta   ->at(idx);
    fatjet.ECF1         = m_ECF1        ->at(idx);
    fatjet.ECF2         = m_ECF2        ->at(idx);
    fatjet.ECF3         = m_ECF3        ->at(idx);
    fatjet.C2           = m_C2          ->at(idx);
    fatjet.D2           = m_D2          ->at(idx);
    fatjet.NTrimSubjets = m_NTrimSubjets->at(idx);
    fatjet.NClusters    = m_NClusters   ->at(idx);
    fatjet.nTracks      = m_nTracks     ->at(idx);
  }

  if ( m_infoSwitch.m_constituent) {
    fatjet.numConstituents    = m_numConstituents   ->at(idx);
  }

  if ( m_infoSwitch.m_constituentAll) {
    fatjet.constituentWeights = m_constituentWeights  ->at(idx);  
    fatjet.constituent_pt     = m_constituent_pt      ->at(idx);  
    fatjet.constituent_eta    = m_constituent_eta     ->at(idx);  
    fatjet.constituent_phi    = m_constituent_phi     ->at(idx);  
    fatjet.constituent_e      = m_constituent_e       ->at(idx);  
  }

  if(m_infoSwitch.m_bosonCount){
    fatjet.nTQuarks = m_nTQuarks->at(idx);
    fatjet.nHBosons = m_nHBosons->at(idx);
    fatjet.nWBosons = m_nWBosons->at(idx);
    fatjet.nZBosons = m_nZBosons->at(idx);
  }

  if(m_infoSwitch.m_VTags){
    fatjet.Wtag_medium = m_Wtag_medium->at(idx);
    fatjet.Ztag_medium = m_Ztag_medium->at(idx);

    fatjet.Wtag_tight = m_Wtag_tight->at(idx);
    fatjet.Ztag_tight = m_Ztag_tight->at(idx);
  }


  if( m_infoSwitch.m_trackJets ){
    fatjet.trkJets.clear();

    for(unsigned int iTrkJet : m_trkJetsIdx->at(idx)){
      Jet thisTrkJet;
      m_trkJets->updateParticle(iTrkJet,  thisTrkJet);
      fatjet.trkJets.push_back(thisTrkJet);
    }

  }

  if(m_debug) cout << "leave FatJetContainer::updateParticle " << endl;
  return;
}



void FatJetContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_scales ) {
      setBranch<float>(tree, "JetConstitScaleMomentum_eta", m_JetConstitScaleMomentum_eta);
      setBranch<float>(tree, "JetConstitScaleMomentum_phi", m_JetConstitScaleMomentum_phi);
      setBranch<float>(tree, "JetConstitScaleMomentum_m", m_JetConstitScaleMomentum_m);
      setBranch<float>(tree, "JetConstitScaleMomentum_pt", m_JetConstitScaleMomentum_pt);

      setBranch<float>(tree, "JetEMScaleMomentum_eta", m_JetEMScaleMomentum_eta);
      setBranch<float>(tree, "JetEMScaleMomentum_phi", m_JetEMScaleMomentum_phi);
      setBranch<float>(tree, "JetEMScaleMomentum_m", m_JetEMScaleMomentum_m);
      setBranch<float>(tree, "JetEMScaleMomentum_pt", m_JetEMScaleMomentum_pt);
  }

  if ( m_infoSwitch.m_area ) {
    setBranch<float>(tree, "GhostArea", m_GhostArea); 
    setBranch<float>(tree, "ActiveArea", m_ActiveArea); 
    setBranch<float>(tree, "VoronoiArea", m_VoronoiArea); 
    setBranch<float>(tree, "ActiveArea4vec_pt", m_ActiveArea4vec_pt); 
    setBranch<float>(tree, "ActiveArea4vec_eta", m_ActiveArea4vec_eta); 
    setBranch<float>(tree, "ActiveArea4vec_phi", m_ActiveArea4vec_phi); 
    setBranch<float>(tree, "ActiveArea4vec_m", m_ActiveArea4vec_m); 

  }

  if ( m_infoSwitch.m_substructure ) {
    setBranch<float>(tree, "Split12",      m_Split12);
    setBranch<float>(tree, "Split23",      m_Split23);
    setBranch<float>(tree, "Split34",      m_Split34);
    setBranch<float>(tree, "tau1_wta",     m_tau1_wta);
    setBranch<float>(tree, "tau2_wta",     m_tau2_wta);
    setBranch<float>(tree, "tau3_wta",     m_tau3_wta);
    setBranch<float>(tree, "tau21_wta",    m_tau21_wta);
    setBranch<float>(tree, "tau32_wta",    m_tau32_wta);
    setBranch<float>(tree, "ECF1",         m_ECF1);
    setBranch<float>(tree, "ECF2",         m_ECF2);
    setBranch<float>(tree, "ECF3",         m_ECF3);
    setBranch<float>(tree, "C2",           m_C2);
    setBranch<float>(tree, "D2",           m_D2);
    setBranch<float>(tree, "NTrimSubjets", m_NTrimSubjets);
    setBranch<int>  (tree, "Nclusters",    m_NClusters);
    setBranch<int>  (tree, "nTracks",      m_nTracks);
  }

  if ( m_infoSwitch.m_constituent) {
    setBranch<int>  (tree, "numConstituents",    m_numConstituents);
  }

  if ( m_infoSwitch.m_constituentAll) {
    setBranch< vector<float> >(tree, "constituentWeights",  m_constituentWeights);
    setBranch< vector<float> >(tree, "constituent_pt",      m_constituent_pt);
    setBranch< vector<float> >(tree, "constituent_eta",     m_constituent_eta);
    setBranch< vector<float> >(tree, "constituent_phi",     m_constituent_phi);
    setBranch< vector<float> >(tree, "constituent_e",       m_constituent_e);
  }

  if(m_infoSwitch.m_bosonCount){
    setBranch< int >(tree, "nTQuarks",       m_nTQuarks);
    setBranch< int >(tree, "nHBosons",       m_nHBosons);
    setBranch< int >(tree, "nWBosons",       m_nWBosons);
    setBranch< int >(tree, "nZBosons",       m_nZBosons);
  }

  if(m_infoSwitch.m_VTags){
    setBranch< int >(tree, "Wtag_medium",       m_Wtag_medium);
    setBranch< int >(tree, "Ztag_medium",       m_Ztag_medium);

    setBranch< int >(tree, "Wtag_tight",       m_Wtag_tight);
    setBranch< int >(tree, "Ztag_tight",       m_Ztag_tight);
  }
 
  if( m_infoSwitch.m_trackJets ){
    m_trkJets->setBranches(tree);
    setBranch< vector<unsigned int> >(tree, "trkJetsIdx",       m_trkJetsIdx);
  }

  return;
}
    

void FatJetContainer::clear()
{
  
  ParticleContainer::clear();

  if ( m_infoSwitch.m_scales ) {
    m_JetConstitScaleMomentum_eta   ->clear();
    m_JetConstitScaleMomentum_phi   ->clear();
    m_JetConstitScaleMomentum_m   ->clear();
    m_JetConstitScaleMomentum_pt    ->clear();

    m_JetEMScaleMomentum_eta    ->clear();
    m_JetEMScaleMomentum_phi    ->clear();
    m_JetEMScaleMomentum_m    ->clear();
    m_JetEMScaleMomentum_pt   ->clear();
  }

  if ( m_infoSwitch.m_area ) {
    m_GhostArea->clear(); 
    m_ActiveArea->clear(); 
    m_VoronoiArea->clear(); 
    m_ActiveArea4vec_pt->clear(); 
    m_ActiveArea4vec_eta->clear(); 
    m_ActiveArea4vec_phi->clear(); 
    m_ActiveArea4vec_m->clear(); 
  }

  if ( m_infoSwitch.m_substructure ) {
    m_Split12     ->clear();
    m_Split23     ->clear();
    m_Split34     ->clear();
    m_tau1_wta    ->clear();
    m_tau2_wta    ->clear();
    m_tau3_wta    ->clear();
    m_tau21_wta   ->clear();
    m_tau32_wta   ->clear();
    m_ECF1        ->clear();
    m_ECF2        ->clear();
    m_ECF3        ->clear();
    m_C2          ->clear();
    m_D2          ->clear();
    m_NTrimSubjets->clear();
    m_NClusters   ->clear();
    m_nTracks     ->clear();
  }

  if ( m_infoSwitch.m_constituent) {
    m_numConstituents->clear();
  }

  if ( m_infoSwitch.m_constituentAll) {
    m_constituentWeights->clear();  
    m_constituent_pt    ->clear();  
    m_constituent_eta   ->clear();  
    m_constituent_phi   ->clear();  
    m_constituent_e     ->clear();  
  }

  if(m_infoSwitch.m_bosonCount){
    m_nTQuarks->clear();
    m_nHBosons->clear();
    m_nWBosons->clear();
    m_nZBosons->clear();
  }

  if(m_infoSwitch.m_VTags){
    m_Wtag_medium->clear();
    m_Ztag_medium->clear();

    m_Wtag_tight->clear();
    m_Ztag_tight->clear();
  }

  if( m_infoSwitch.m_trackJets ){
    m_trkJets->clear();
    m_trkJetsIdx->clear();
  }

  return;
}

void FatJetContainer::FillFatJet( const xAOD::Jet* jet ){
  return FillFatJet(static_cast<const xAOD::IParticle*>(jet));
}

void FatJetContainer::FillFatJet( const xAOD::IParticle* particle ){

  ParticleContainer::FillParticle(particle);

  const xAOD::Jet* fatjet=dynamic_cast<const xAOD::Jet*>(particle);

  if( m_infoSwitch.m_scales ){
    static SG::AuxElement::ConstAccessor<float> acc_JetConstitScaleMomentum_eta("JetConstitScaleMomentum_eta");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetConstitScaleMomentum_eta, m_JetConstitScaleMomentum_eta, -999);
    static SG::AuxElement::ConstAccessor<float> acc_JetConstitScaleMomentum_phi("JetConstitScaleMomentum_phi");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetConstitScaleMomentum_phi, m_JetConstitScaleMomentum_phi, -999);
    static SG::AuxElement::ConstAccessor<float> acc_JetConstitScaleMomentum_m("JetConstitScaleMomentum_m");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetConstitScaleMomentum_m, m_JetConstitScaleMomentum_m, -999, m_units);
    static SG::AuxElement::ConstAccessor<float> acc_JetConstitScaleMomentum_pt("JetConstitScaleMomentum_pt");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetConstitScaleMomentum_pt, m_JetConstitScaleMomentum_pt, -999, m_units);

    static SG::AuxElement::ConstAccessor<float> acc_JetEMScaleMomentum_eta("JetEMScaleMomentum_eta");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetEMScaleMomentum_eta, m_JetEMScaleMomentum_eta, -999);
    static SG::AuxElement::ConstAccessor<float> acc_JetEMScaleMomentum_phi("JetEMScaleMomentum_phi");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetEMScaleMomentum_phi, m_JetEMScaleMomentum_phi, -999);
    static SG::AuxElement::ConstAccessor<float> acc_JetEMScaleMomentum_m("JetEMScaleMomentum_m");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetEMScaleMomentum_m, m_JetEMScaleMomentum_m, -999, m_units);
    static SG::AuxElement::ConstAccessor<float> acc_JetEMScaleMomentum_pt("JetEMScaleMomentum_pt");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_JetEMScaleMomentum_pt, m_JetEMScaleMomentum_pt, -999, m_units);
  }

  if ( m_infoSwitch.m_area ) {
    static SG::AuxElement::ConstAccessor<float> acc_GhostArea("GhostArea");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_GhostArea, m_GhostArea, -999); 
    static SG::AuxElement::ConstAccessor<float> acc_ActiveArea("ActiveArea");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ActiveArea, m_ActiveArea, -999); 
    static SG::AuxElement::ConstAccessor<float> acc_VoronoiArea("VoronoiArea");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_VoronoiArea, m_VoronoiArea, -999); 
    static SG::AuxElement::ConstAccessor<float> acc_ActiveArea4vec_pt("ActiveArea4vec_pt");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ActiveArea4vec_pt, m_ActiveArea4vec_pt, -999, m_units); 
    static SG::AuxElement::ConstAccessor<float> acc_ActiveArea4vec_eta("ActiveArea4vec_eta");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ActiveArea4vec_eta, m_ActiveArea4vec_eta, -999); 
    static SG::AuxElement::ConstAccessor<float> acc_ActiveArea4vec_phi("ActiveArea4vec_phi");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ActiveArea4vec_phi, m_ActiveArea4vec_phi, -999); 
    static SG::AuxElement::ConstAccessor<float> acc_ActiveArea4vec_m("ActiveArea4vec_m");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ActiveArea4vec_m, m_ActiveArea4vec_m, -999, m_units); 
  }

  if( m_infoSwitch.m_substructure ){
    static SG::AuxElement::ConstAccessor<float> acc_Split12("Split12");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_Split12, m_Split12, -999, m_units);

    static SG::AuxElement::ConstAccessor<float> acc_Split23("Split23");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_Split23, m_Split23, -999, m_units);

    static SG::AuxElement::ConstAccessor<float> acc_Split34("Split34");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_Split34, m_Split34, -999, m_units);

    static SG::AuxElement::ConstAccessor<float> acc_tau1_wta ("Tau1_wta");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_tau1_wta, m_tau1_wta, -999);

    static SG::AuxElement::ConstAccessor<float> acc_tau2_wta ("Tau2_wta");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_tau2_wta, m_tau2_wta, -999);

    static SG::AuxElement::ConstAccessor<float> acc_tau3_wta ("Tau3_wta");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_tau3_wta, m_tau3_wta, -999);

    static SG::AuxElement::ConstAccessor<float> acc_tau21_wta ("Tau21_wta");
    if(acc_tau21_wta.isAvailable( *fatjet )){
      m_tau21_wta->push_back( acc_tau21_wta( *fatjet ) );
    } else if ( acc_tau1_wta.isAvailable( *fatjet ) && acc_tau2_wta.isAvailable( *fatjet ) ) {
      m_tau21_wta->push_back( acc_tau2_wta( *fatjet ) / acc_tau1_wta( *fatjet ) );
    } else { m_tau21_wta->push_back( -999 ); }

    static SG::AuxElement::ConstAccessor<float> acc_tau32_wta ("Tau32_wta");
    if(acc_tau32_wta.isAvailable( *fatjet )){
      m_tau32_wta->push_back( acc_tau32_wta( *fatjet ) );
    } else if ( acc_tau2_wta.isAvailable( *fatjet ) && acc_tau3_wta.isAvailable( *fatjet ) ) {
      m_tau32_wta->push_back( acc_tau3_wta( *fatjet ) / acc_tau2_wta( *fatjet ) );
    } else { m_tau32_wta->push_back( -999 ); }

    static SG::AuxElement::ConstAccessor<int> acc_NClusters ("MyNClusters");
    safeFill<int, int, xAOD::Jet>(fatjet, acc_NClusters, m_NClusters, -999);

    static SG::AuxElement::ConstAccessor<float> acc_ECF1 ("ECF1");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ECF1, m_ECF1, -999, m_units);

    static SG::AuxElement::ConstAccessor<float> acc_ECF2("ECF2");
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ECF2, m_ECF2, -999, m_units);

    static SG::AuxElement::ConstAccessor<float> acc_ECF3 ("ECF3");      
    safeFill<float, float, xAOD::Jet>(fatjet, acc_ECF3, m_ECF3, -999, m_units);

    static SG::AuxElement::ConstAccessor<int> NTrimSubjets("NTrimSubjets");
    safeFill<int, float, xAOD::Jet>(fatjet, NTrimSubjets, m_NTrimSubjets, -999);

    static SG::AuxElement::ConstAccessor<float> acc_D2 ("D2");
    if( acc_D2.isAvailable( *fatjet ) ) {
      m_D2->push_back( acc_D2( *fatjet ));
    } else if (acc_ECF1.isAvailable( *fatjet ) && acc_ECF2.isAvailable( *fatjet ) && acc_ECF3.isAvailable( *fatjet )){
      float e2=(acc_ECF2( *fatjet )/(acc_ECF1( *fatjet )*acc_ECF1( *fatjet )));
      float e3=(acc_ECF3( *fatjet )/(acc_ECF1( *fatjet )*acc_ECF1( *fatjet )*acc_ECF1( *fatjet )));
      m_D2->push_back( e3/(e2*e2*e2) );
    } else{ m_D2->push_back(-999); }

    static SG::AuxElement::ConstAccessor<float> acc_C2("C2");
    if(acc_C2.isAvailable(*fatjet)){
      m_C2->push_back(acc_C2(*fatjet));
    } else if( acc_ECF1.isAvailable(*fatjet) && acc_ECF2.isAvailable(*fatjet) && acc_ECF3.isAvailable(*fatjet)){
      m_C2->push_back( acc_ECF3(*fatjet)*acc_ECF1(*fatjet)/pow(acc_ECF2(*fatjet),2.0));
    } else{ m_C2->push_back(-999); }

    m_nTracks->push_back( fatjet->auxdata<int>("GhostTrackCount"));

  }

  if( m_infoSwitch.m_constituent ){
    m_numConstituents->push_back( fatjet->numConstituents() );
  }


  if( m_infoSwitch.m_constituentAll ){
    m_constituentWeights->push_back( fatjet->getAttribute< std::vector<float> >( "constituentWeights" ) );
    std::vector<float> pt;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<float> e;
    xAOD::JetConstituentVector consVec = fatjet->getConstituents();

    if( consVec.isValid() ) {
      
      // use the example provided in
      // http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODJet/xAODJet/JetConstituentVector.h
      xAOD::JetConstituentVector::iterator constit = consVec.begin();
      xAOD::JetConstituentVector::iterator constitE = consVec.end();
      for( ; constit != constitE; constit++){
	pt. push_back( constit->pt() / m_units );
	eta.push_back( constit->eta() );
	phi.push_back( constit->phi() );
	e.  push_back( constit->e() / m_units  );
      }
    }
    m_constituent_pt ->push_back( pt  );
    m_constituent_eta->push_back( eta );
    m_constituent_phi->push_back( phi );
    m_constituent_e  ->push_back( e   );
  }


  if(m_infoSwitch.m_bosonCount){

    const xAOD::Jet* fatJetParentJet = 0;
    
    try{
      auto el = fatjet->auxdata<ElementLink<xAOD::JetContainer> >("Parent");
      if(!el.isValid()){
	Warning("executeSingle()", "Invalid link to \"Parent\" from fat-jet.");
      }
      else{
	fatJetParentJet = (*el);
      }
    }catch(...){
      Warning("executeSingle()", "Unable to get parent jet of fat-jet for truth labeling. Trimmed jet area would be used!");
      fatJetParentJet = fatjet;
    }
    
    if(m_mc){
      static SG::AuxElement::ConstAccessor< int > truthfatjet_TQuarks("GhostTQuarksFinalCount");
      safeFill<int, int, xAOD::Jet>(fatJetParentJet, truthfatjet_TQuarks, m_nTQuarks, -999);

      static SG::AuxElement::ConstAccessor< int > truthfatjet_WBosons("GhostWBosonsCount");
      safeFill<int, int, xAOD::Jet>(fatJetParentJet, truthfatjet_WBosons, m_nWBosons, -999);

      static SG::AuxElement::ConstAccessor< int > truthfatjet_ZBosons("GhostZBosonsCount");
      safeFill<int, int, xAOD::Jet>(fatJetParentJet, truthfatjet_ZBosons, m_nZBosons, -999);

      static SG::AuxElement::ConstAccessor< int > truthfatjet_HBosons("GhostHBosonsCount");
      safeFill<int, int, xAOD::Jet>(fatJetParentJet, truthfatjet_HBosons, m_nHBosons, -999);
    }
  }
  
  if(m_infoSwitch.m_VTags){
    int Wtag_pass_medium = m_WbosonTaggerMedium->result(*fatjet);
    int Ztag_pass_medium = m_ZbosonTaggerMedium->result(*fatjet);
    m_Wtag_medium->push_back(Wtag_pass_medium);
    m_Ztag_medium->push_back(Ztag_pass_medium);

    int Wtag_pass_tight = m_WbosonTaggerTight->result(*fatjet);
    int Ztag_pass_tight = m_ZbosonTaggerTight->result(*fatjet);
    m_Wtag_tight->push_back(Wtag_pass_tight);
    m_Ztag_tight->push_back(Ztag_pass_tight);
  }


  //
  // Associated track jets
  //
  //if(
  if( m_infoSwitch.m_trackJets ){

    const xAOD::Jet* fatjet_parent = 0;
    
    try{
      auto el = fatjet->auxdata<ElementLink<xAOD::JetContainer> >("Parent");
      if(!el.isValid()){
	Warning("execute()", "Invalid link to \"Parent\" from leading calo-jet");
      }
      else{
	fatjet_parent = (*el);
      }
    }
    catch (...){
      Warning("execute()", "Unable to fetch \"Parent\" link from leading calo-jet");
    }

    if(fatjet_parent == 0){
      Warning("execute()", "Trimmed jet area will be used for leading calo-jet");
      fatjet_parent = fatjet;
    }

    std::vector<const xAOD::Jet*> assotrkjets;
    try{
      assotrkjets = fatjet_parent->getAssociatedObjects<xAOD::Jet>(m_infoSwitch.m_trackJetName);
    }
    catch (...){
      Warning("execute()", "Unable to fetch \"%s\" link from leading calo-jet", m_infoSwitch.m_trackJetName.data());
    }

    m_trkJetsIdx->push_back(vector<unsigned int>());
    for(auto TrackJet : assotrkjets){
      if(!SelectTrackJet(TrackJet)) continue;
      m_trkJetsIdx->back().push_back(m_trkJets->m_n);
      m_trkJets->FillJet(TrackJet, 0 , 0);
    }
  }

  return;
}


bool FatJetContainer::SelectTrackJet(const xAOD::Jet* TrackJet)
{
  if( TrackJet->pt() < m_trackJetPtCut )            return false;
  if( fabs(TrackJet->eta()) > m_trackJetEtaCut )    return false;
  if( TrackJet->numConstituents() < 2 )             return false;

  return true;
}

