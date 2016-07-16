#include "xAODAnaHelpers/MuonContainer.h"

#include <iostream>

using namespace xAH;

MuonContainer::MuonContainer(const std::string& name, const std::string& detailStr)
  : ParticleContainer(name, detailStr, 1e3, true, false)
{

  m_isMedium   = new std::vector<int>();
  m_isTight    = new std::vector<int>();
  m_EnergyLoss = new std::vector<float>();
}

MuonContainer::~MuonContainer()
{
  delete m_isMedium;
  delete m_isTight;
  delete m_EnergyLoss;
}

void MuonContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if ( m_infoSwitch.m_quality ) {

    connectBranch<int  >     (tree, "isMedium"   ,      &m_isMedium    ); 
    connectBranch<int  >     (tree, "isTight"    ,      &m_isTight     ); 

  }

  if( m_infoSwitch.m_energyLoss ) {
    connectBranch<float>     (tree, "EnergyLoss" ,      &m_EnergyLoss  ); 
  }


}

void MuonContainer::updateParticle(uint idx, Muon& muon)
{
  ParticleContainer::updateParticle(idx,muon);  

  if( m_infoSwitch.m_quality ) {
    muon.isMedium                    =m_isMedium                    ->at(idx);
    muon.isTight                     =m_isTight                     ->at(idx);
  }

  if( m_infoSwitch.m_energyLoss ) {
    muon.EnergyLoss                  =m_EnergyLoss                  ->at(idx)/1000;
  }

}
