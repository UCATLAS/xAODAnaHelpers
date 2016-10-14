#include "xAODAnaHelpers/TauContainer.h"

#include <iostream>

using namespace xAH;
using std::vector;
using std::string;

TauContainer::TauContainer(const std::string& name, const std::string& detailStr, float units, bool mc)
  : ParticleContainer(name, detailStr, units, mc, true, false)
{

  if( m_infoSwitch.m_kinematic) {
    m_ntrk    = new  std::vector<int>   ();
    m_charge  = new  std::vector<float> ();
  }

}

TauContainer::~TauContainer()
{
  
  if (m_infoSwitch.m_kinematic){
    delete m_ntrk;
    delete m_charge;
  }


}

void TauContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if ( m_infoSwitch.m_kinematic ){
    connectBranch<int>    (tree, "ntrk",       &m_ntrk);
    connectBranch<float>  (tree, "charge",     &m_charge );
  }
}

void TauContainer::updateParticle(uint idx, Tau& tau)
{
  ParticleContainer::updateParticle(idx,tau);  

  // trigger
  if ( m_infoSwitch.m_kinematic ) {
    tau.ntrk    =     m_ntrk    ->at(idx);
    tau.charge  =     m_charge  ->at(idx);
  }

}


void TauContainer::setBranches(TTree *tree)
{

  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_kinematic ){
    setBranch<int>  (tree,"ntrk", m_ntrk);
    setBranch<float>(tree,"charge", m_charge );

  }
  
  return;
}



void TauContainer::clear()
{
  
  ParticleContainer::clear();

  if ( m_infoSwitch.m_kinematic ) {
    m_ntrk->clear();
    m_charge->clear();
  }
}


void TauContainer::FillTau( const xAOD::TauJet* tau ){
  return FillTau(static_cast<const xAOD::IParticle*>(tau));
}

void TauContainer::FillTau( const xAOD::IParticle* particle ) 
{

  ParticleContainer::FillParticle(particle);

  const xAOD::TauJet* tau=dynamic_cast<const xAOD::TauJet*>(particle);

  if ( m_infoSwitch.m_kinematic ) {

    m_charge->push_back( tau->charge()  );
    m_ntrk  ->push_back( tau->nTracks() );
  }
  
  return;
}
