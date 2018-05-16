#include "xAODAnaHelpers/ClusterContainer.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <iostream>
#include "xAODTruth/TruthEventContainer.h"

using namespace xAH;

ClusterContainer::ClusterContainer(const std::string& name, const std::string& detailStr)
  : ParticleContainer(name,detailStr)
    
{
  // rapidity
  if(m_infoSwitch.m_rapidity) {
    m_rapidity                  =new std::vector<float>();
  }
  
  // clean
  if(m_infoSwitch.m_clean) {
    m_Timing                    =new std::vector<float>();
  }
}

ClusterContainer::~ClusterContainer()
{
  if(m_debug) std::cout << " Deleting ClusterContainer "  << std::endl;
  if(m_infoSwitch.m_rapidity) {
    delete m_rapidity;
  }
  
  // clean
  if(m_infoSwitch.m_clean) {
    delete m_Timing;
  }
  
}
 
 void ClusterContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if(m_infoSwitch.m_rapidity)
    {
      connectBranch<float>(tree,"rapidity",                      &m_rapidity);
    }


  if(m_infoSwitch.m_clean)
    {
      connectBranch<float>(tree, "Timing",                     &m_Timing);
    }
}

void ClusterContainer::updateParticle(uint idx, Cluster& cluster)
{
  if(m_debug) std::cout << "in ClusterContainer::updateParticle " << std::endl;
  ParticleContainer::updateParticle(idx,cluster);

  if(m_infoSwitch.m_rapidity)
    {
      cluster.rapidity                    =m_rapidity                    ->at(idx);
    }

  if(m_infoSwitch.m_clean)
    {
      cluster.Timing                    =m_Timing                    ->at(idx);
    }

  if(m_debug) std::cout << "leave ClusterContainer::updateParticle " << std::endl;
  return;
}



void ClusterContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_rapidity ) {
    setBranch<float>(tree,"rapidity",                      m_rapidity              );
  }

  if( m_infoSwitch.m_clean ){
    setBranch<float>(tree,"Timing",                        m_Timing               );
  }

  return;
}


void ClusterContainer::clear()
{

  ParticleContainer::clear();

  // rapidity
  if( m_infoSwitch.m_rapidity ) {
    m_rapidity->clear();
  }
    
  // clean
  if( m_infoSwitch.m_clean ){
    m_Timing                    ->clear();
  }

  return;
}

void ClusterContainer::FillCluster( const xAOD::CaloCluster* cluster ){
  return FillCluster(static_cast<const xAOD::IParticle*>(cluster));
}

void ClusterContainer::FillCluster( const xAOD::IParticle* particle ){
  if(m_debug) std::cout << "In ClusterContainer::FillCluster " << std::endl;

  ParticleContainer::FillParticle(particle);

  const xAOD::CaloCluster* cluster=dynamic_cast<const xAOD::CaloCluster*>(particle);

  // if( m_infoSwitch.m_rapidity ){
    // m_rapidity->push_back( cluster->rapidity() );
  // }


  // if (m_infoSwitch.m_clean){

    // static SG::AuxElement::ConstAccessor<float> clusterTime ("Timing");
    // safeFill<float, float, xAOD::CaloCluster>(cluster, clusterTime, m_Timing, -999);
  
  // }

  return;
}
