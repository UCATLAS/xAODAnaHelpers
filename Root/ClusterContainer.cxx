#include "xAODAnaHelpers/ClusterContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

ClusterContainer::ClusterContainer(const std::string& name, const std::string& detailStr, float units, bool mc)
  : ParticleContainer(name, detailStr, units, mc, true)
{
}

ClusterContainer::~ClusterContainer()
{
}

void ClusterContainer::setTree(TTree *tree)
{
  // Connect branches
  ParticleContainer::setTree(tree);
  tree->SetBranchStatus  ("nclus" , 1);
  tree->SetBranchAddress ("nclus" , &m_n);
}

void ClusterContainer::updateParticle(uint idx, Cluster& cluster)
{
  ParticleContainer::updateParticle(idx,cluster);
}


void ClusterContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);
  return;
}



void ClusterContainer::clear()
{
  ParticleContainer::clear();
}


void ClusterContainer::FillCluster( const xAOD::CaloCluster* cluster ){
  return FillCluster(static_cast<const xAOD::IParticle*>(cluster));
}

void ClusterContainer::FillCluster( const xAOD::IParticle* particle )
{
  ParticleContainer::FillParticle(particle);
  return;
}
