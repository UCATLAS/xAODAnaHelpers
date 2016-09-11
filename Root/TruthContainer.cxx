#include "xAODAnaHelpers/TruthContainer.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <iostream>
#include <xAODTruth/TruthVertex.h>
//#include "xAODTruth/TruthEventContainer.h"

using namespace xAH;
using std::vector;  using std::endl;  using std::cout;


TruthContainer::TruthContainer(const std::string& name, const std::string& detailStr, float units)
  : ParticleContainer(name,detailStr,units,true)

{
  m_pdgId   = new std::vector<int>();
  m_status  = new std::vector<int>();


  if(m_infoSwitch.m_type){
    m_is_higgs  = new std::vector<int>();
    m_is_bhad   = new std::vector<int>();
  }

  if(m_infoSwitch.m_bVtx){
    m_Bdecay_x  = new std::vector<float>();
    m_Bdecay_y  = new std::vector<float>();
    m_Bdecay_z  = new std::vector<float>();
  }

}

TruthContainer::~TruthContainer()
{
  if(m_debug) cout << " Deleting TruthContainer "  << endl;  
  
  delete m_pdgId;
  delete m_status;


  if(m_infoSwitch.m_type){
    delete m_is_higgs;
    delete m_is_bhad ;
  }

  if(m_infoSwitch.m_bVtx){
    delete m_Bdecay_x;
    delete m_Bdecay_y;
    delete m_Bdecay_z;
  }

}

void TruthContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  connectBranch<int>(tree,"pdgId",                      &m_pdgId);
  connectBranch<int>(tree,"status",                     &m_status);

  if(m_infoSwitch.m_type){
    connectBranch<int>(tree,"is_higgs",  &m_is_higgs);
    connectBranch<int>(tree,"is_bhad",   &m_is_bhad);
  }

  if(m_infoSwitch.m_bVtx){
    connectBranch<float>(tree,"Bdecay_x",   &m_Bdecay_x);
    connectBranch<float>(tree,"Bdecay_y",   &m_Bdecay_y);
    connectBranch<float>(tree,"Bdecay_z",   &m_Bdecay_z);
  }

}

void TruthContainer::updateParticle(uint idx, TruthPart& truth)
{
  if(m_debug) cout << "in TruthContainer::updateParticle " << endl;
  ParticleContainer::updateParticle(idx,truth);  

  truth.pdgId                    =m_pdgId                    ->at(idx);
  truth.status                   =m_status                   ->at(idx);


  if(m_infoSwitch.m_type){
    truth.is_higgs = (bool)m_is_higgs->at(idx);
    truth.is_bhad  = (bool)m_is_bhad ->at(idx);
  }


  if(m_infoSwitch.m_bVtx){
    truth.Bdecay_x = m_Bdecay_x->at(idx);
    truth.Bdecay_y = m_Bdecay_y->at(idx);
    truth.Bdecay_z = m_Bdecay_z->at(idx);
  }



  if(m_debug) cout << "leave TruthContainer::updateParticle " << endl;
  return;
}



void TruthContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  setBranch<int>(tree,"pdgId",                      m_pdgId              );
  setBranch<int>(tree,"status",                     m_status             );

  if(m_infoSwitch.m_type){
    setBranch<int>(tree,"is_higgs",                      m_is_higgs              );
    setBranch<int>(tree,"is_bhad",                       m_is_bhad              );
  }


  if(m_infoSwitch.m_bVtx){
    setBranch<float>(tree,"Bdecay_x",                      m_Bdecay_x              );
    setBranch<float>(tree,"Bdecay_y",                      m_Bdecay_y              );
    setBranch<float>(tree,"Bdecay_z",                      m_Bdecay_z              );
  }

  
  return;
}
    

void TruthContainer::clear()
{
  
  ParticleContainer::clear();

  m_pdgId ->clear();
  m_status->clear();

  if(m_infoSwitch.m_type){
    m_is_higgs  ->clear();
    m_is_bhad   ->clear();
  }

  if(m_infoSwitch.m_bVtx){
    m_Bdecay_x->clear();
    m_Bdecay_y->clear();
    m_Bdecay_z->clear();
  }


  return;
}

void TruthContainer::FillTruth( const xAOD::TruthParticle* truth ){
  return FillTruth(static_cast<const xAOD::IParticle*>(truth));
}

void TruthContainer::FillTruth( const xAOD::IParticle* particle ){
  if(m_debug) std::cout << "In Fill Truth " << std::endl;
  ParticleContainer::FillParticle(particle);

  const xAOD::TruthParticle* truth=dynamic_cast<const xAOD::TruthParticle*>(particle);
  if(m_debug) std::cout << "Got TP " << std::endl;

  m_pdgId ->push_back( truth->pdgId() );
  m_status->push_back( truth->status() );
  if(m_debug)   std::cout << "Filled status " << std::endl;

  if(m_infoSwitch.m_type){
    if(m_debug) std::cout << "Filling  type " << std::endl;
    m_is_higgs->push_back( (int)truth->isHiggs()        );
    m_is_bhad ->push_back( (int)truth->isBottomHadron() );
  }


  if(m_infoSwitch.m_bVtx){
    if(m_debug) std::cout << "Filling  bVtx " << std::endl;
    if(truth->isBottomHadron() && truth->hasDecayVtx()){
      const xAOD::TruthVertex* vtx = truth->decayVtx();
      m_Bdecay_x->push_back(vtx->x());
      m_Bdecay_y->push_back(vtx->y());
      m_Bdecay_z->push_back(vtx->z());
    }
    else{
      m_Bdecay_x->push_back(-999999.);
      m_Bdecay_y->push_back(-999999.);
      m_Bdecay_z->push_back(-999999.);
    }
  }
  if(m_debug) std::cout << "Leave Fill Truth " << std::endl;
  return;
}

