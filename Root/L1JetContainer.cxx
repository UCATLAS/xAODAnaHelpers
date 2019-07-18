#include "xAODAnaHelpers/L1JetContainer.h"
#include <iostream>

using namespace xAH;

L1JetContainer::L1JetContainer(const std::string& name, float units, bool mc)
  : ParticleContainer(name,"",units,mc)
{
  m_l1Jet_et8x8             =new std::vector<float>();
  m_l1Jet_eta               =new std::vector<float>();
  m_l1Jet_phi               =new std::vector<float>();
}

L1JetContainer::~L1JetContainer()
{
  if(m_debug) std::cout << " Deleting L1JetContainer "  << std::endl;
  delete m_l1Jet_et8x8;
  delete m_l1Jet_eta;
  delete m_l1Jet_phi;
}

void L1JetContainer::setTree(TTree *tree)
{
  ParticleContainer::setTree(tree);
  connectBranch<float>(tree,"et8x8",&m_l1Jet_et8x8);
  connectBranch<float>(tree,"eta",  &m_l1Jet_eta);
  connectBranch<float>(tree,"phi",  &m_l1Jet_phi);
}

void L1JetContainer::updateParticle(uint idx, Jet& jet)
{
  ParticleContainer::updateParticle(idx,jet);
}

void L1JetContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);
  setBranch<float>(tree,"et8x8",m_l1Jet_et8x8);
  setBranch<float>(tree,"eta",  m_l1Jet_eta);
  setBranch<float>(tree,"phi",  m_l1Jet_phi);
  return;
}

void L1JetContainer::clear()
{
  ParticleContainer::clear();
  m_l1Jet_et8x8->clear();
  m_l1Jet_eta  ->clear();
  m_l1Jet_phi  ->clear();
  return;
}

void L1JetContainer::FillL1Jets( const xAOD::JetRoIContainer* jets, bool sort){
  if(!sort) {
    for( auto jet_itr : *jets ) {
      m_l1Jet_et8x8->push_back ( jet_itr->et8x8() / m_units );
      m_l1Jet_eta->push_back( jet_itr->eta() );
      m_l1Jet_phi->push_back( jet_itr->phi() );
    }
  } else {
    std::vector< std::vector<float> > vec;
    for( auto jet_itr : *jets ) {
      std::vector<float> row;
      row.clear();
      row.push_back(jet_itr->et8x8());
      row.push_back(jet_itr->eta());
      row.push_back(jet_itr->phi());
      vec.push_back(row);
    }
    
    std::sort(vec.begin(), vec.end(), [&](const std::vector<float> a, const std::vector<float> b) { return a.at(0) < b.at(0);});
    for (int i = int(vec.size())-1; i >= 0; i--) {
      m_l1Jet_et8x8->push_back((vec.at(i)).at(0) / m_units);
      m_l1Jet_eta->push_back((vec.at(i)).at(1));
      m_l1Jet_phi->push_back((vec.at(i)).at(2));
    }
    vec.clear();
  }
}
