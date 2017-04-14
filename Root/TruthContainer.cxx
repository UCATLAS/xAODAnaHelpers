#include "xAODAnaHelpers/TruthContainer.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <iostream>
#include <xAODTruth/TruthVertex.h>
#include <xAODTruth/TruthParticle.h>
//#include "xAODTruth/TruthEventContainer.h"

using namespace xAH;
using std::vector;  using std::endl;  using std::cout;


TruthContainer::TruthContainer(const std::string& name, const std::string& detailStr, float units)
  : ParticleContainer(name,detailStr,units,true)

{
  m_pdgId   = new vector<int>();
  m_status  = new vector<int>();
  m_barcode = new vector<int>();


  if(m_infoSwitch.m_type){
    m_is_higgs  = new vector<int>();
    m_is_bhad   = new vector<int>();
  }

  if(m_infoSwitch.m_bVtx){
    m_Bdecay_x  = new vector<float>();
    m_Bdecay_y  = new vector<float>();
    m_Bdecay_z  = new vector<float>();
  }

  if(m_infoSwitch.m_parents){
    m_nParents        = new vector<int>();
    m_parent_pdgId    = new vector< vector<int> >();
    m_parent_barcode  = new vector< vector<int> >();
    m_parent_status   = new vector< vector<int> >();
  }

  if(m_infoSwitch.m_children){
    m_nChildren      = new vector<int>();
    m_child_pdgId    = new vector< vector<int> >();
    m_child_barcode  = new vector< vector<int> >();
    m_child_status   = new vector< vector<int> >();
  }


}

TruthContainer::~TruthContainer()
{
  if(m_debug) cout << " Deleting TruthContainer "  << endl;  
  
  delete m_pdgId;
  delete m_status;
  delete m_barcode;


  if(m_infoSwitch.m_type){
    delete m_is_higgs;
    delete m_is_bhad ;
  }

  if(m_infoSwitch.m_bVtx){
    delete m_Bdecay_x;
    delete m_Bdecay_y;
    delete m_Bdecay_z;
  }

  if(m_infoSwitch.m_parents){
    delete m_nParents;
    delete m_parent_pdgId;
    delete m_parent_barcode;
    delete m_parent_status;
  }

  if(m_infoSwitch.m_children){
    delete m_nChildren;
    delete m_child_pdgId;
    delete m_child_barcode;
    delete m_child_status;
  }

}

void TruthContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  connectBranch<int>(tree,"pdgId",                      &m_pdgId);
  connectBranch<int>(tree,"status",                     &m_status);
  connectBranch<int>(tree,"barcode",                    &m_barcode);

  if(m_infoSwitch.m_type){
    connectBranch<int>(tree,"is_higgs",  &m_is_higgs);
    connectBranch<int>(tree,"is_bhad",   &m_is_bhad);
  }

  if(m_infoSwitch.m_bVtx){
    connectBranch<float>(tree,"Bdecay_x",   &m_Bdecay_x);
    connectBranch<float>(tree,"Bdecay_y",   &m_Bdecay_y);
    connectBranch<float>(tree,"Bdecay_z",   &m_Bdecay_z);
  }

  if(m_infoSwitch.m_parents){
    connectBranch<int>(tree,"nParents",   &m_nParents);
    connectBranch<vector<int> >(tree,"parent_pdgId",   &m_parent_pdgId);
    connectBranch<vector<int> >(tree,"parent_barcode", &m_parent_barcode);
    connectBranch<vector<int> >(tree,"parent_status",  &m_parent_status);
  }

  if(m_infoSwitch.m_children){
    connectBranch<int>         (tree,"nChildren",     &m_nChildren);
    connectBranch<vector<int> >(tree,"child_pdgId",   &m_child_pdgId);
    connectBranch<vector<int> >(tree,"child_barcode", &m_child_barcode);
    connectBranch<vector<int> >(tree,"child_status",  &m_child_status);
  }

}

void TruthContainer::updateParticle(uint idx, TruthPart& truth)
{
  if(m_debug) cout << "in TruthContainer::updateParticle " << endl;
  ParticleContainer::updateParticle(idx,truth);  

  truth.pdgId                    =m_pdgId                    ->at(idx);
  truth.status                   =m_status                   ->at(idx);
  truth.barcode                  =m_barcode                  ->at(idx);


  if(m_infoSwitch.m_type){
    truth.is_higgs = (bool)m_is_higgs->at(idx);
    truth.is_bhad  = (bool)m_is_bhad ->at(idx);
  }


  if(m_infoSwitch.m_bVtx){
    truth.Bdecay_x = m_Bdecay_x->at(idx);
    truth.Bdecay_y = m_Bdecay_y->at(idx);
    truth.Bdecay_z = m_Bdecay_z->at(idx);
  }

  if(m_infoSwitch.m_parents){
    truth.nParents       = m_nParents->at(idx);
    truth.parent_pdgId   = m_parent_pdgId->at(idx);
    truth.parent_barcode = m_parent_barcode->at(idx);
    truth.parent_status  = m_parent_status ->at(idx);
  }

  if(m_infoSwitch.m_children){
    truth.nChildren     = m_nChildren->at(idx);
    truth.child_pdgId   = m_child_pdgId->at(idx);
    truth.child_barcode = m_child_barcode->at(idx);
    truth.child_status  = m_child_status ->at(idx);
  }


  if(m_debug) cout << "leave TruthContainer::updateParticle " << endl;
  return;
}



void TruthContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  setBranch<int>(tree,"pdgId",                      m_pdgId              );
  setBranch<int>(tree,"status",                     m_status             );
  setBranch<int>(tree,"barcode",                    m_barcode             );

  if(m_infoSwitch.m_type){
    setBranch<int>(tree,"is_higgs",                      m_is_higgs              );
    setBranch<int>(tree,"is_bhad",                       m_is_bhad              );
  }


  if(m_infoSwitch.m_bVtx){
    setBranch<float>(tree,"Bdecay_x",                      m_Bdecay_x              );
    setBranch<float>(tree,"Bdecay_y",                      m_Bdecay_y              );
    setBranch<float>(tree,"Bdecay_z",                      m_Bdecay_z              );
  }

  if(m_infoSwitch.m_parents){
    setBranch<int>         (tree,"nParents",                      m_nParents              );
    setBranch<vector<int> >(tree,"parent_pdgId",                  m_parent_pdgId          );
    setBranch<vector<int> >(tree,"parent_barcode",                m_parent_barcode        );
    setBranch<vector<int> >(tree,"parent_status",                 m_parent_status         );
  }

  if(m_infoSwitch.m_children){
    setBranch<int>         (tree,"nChildren",                    m_nChildren            );
    setBranch<vector<int> >(tree,"child_pdgId",                  m_child_pdgId          );
    setBranch<vector<int> >(tree,"child_barcode",                m_child_barcode        );
    setBranch<vector<int> >(tree,"child_status",                 m_child_status         );
  }


  return;
}
    

void TruthContainer::clear()
{
  
  ParticleContainer::clear();

  m_pdgId ->clear();
  m_status->clear();
  m_barcode->clear();

  if(m_infoSwitch.m_type){
    m_is_higgs  ->clear();
    m_is_bhad   ->clear();
  }

  if(m_infoSwitch.m_bVtx){
    m_Bdecay_x->clear();
    m_Bdecay_y->clear();
    m_Bdecay_z->clear();
  }

  if(m_infoSwitch.m_parents){
    m_nParents->clear();
    m_parent_pdgId->clear();
    m_parent_barcode->clear();
    m_parent_status->clear();
  }

  if(m_infoSwitch.m_children){
    m_nChildren->clear();
    m_child_pdgId->clear();
    m_child_barcode->clear();
    m_child_status->clear();
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

  m_pdgId  ->push_back( truth->pdgId() );
  m_status ->push_back( truth->status() );
  m_barcode->push_back( truth->barcode() );
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

  if(m_infoSwitch.m_parents){
    int nParents = truth->nParents();
    m_nParents->push_back(nParents);

    m_parent_pdgId  ->push_back(vector<int>());
    m_parent_barcode->push_back(vector<int>());
    m_parent_status ->push_back(vector<int>());
    for(int iparent = 0; iparent < nParents; ++iparent){
      const xAOD::TruthParticle* parent = truth->parent(iparent);
      if(parent){
	m_parent_pdgId  ->back().push_back(parent->pdgId());
	m_parent_barcode->back().push_back(parent->barcode());
	m_parent_status ->back().push_back(parent->status());
      }else{			
	m_parent_pdgId  ->back().push_back(-99);
	m_parent_barcode->back().push_back(-99);
	m_parent_status ->back().push_back(-99);
      }
    }
  }


  if(m_infoSwitch.m_children){
    int nChildren = truth->nChildren();
    m_nChildren->push_back(nChildren);

    m_child_pdgId  ->push_back(vector<int>());
    m_child_barcode->push_back(vector<int>());
    m_child_status ->push_back(vector<int>());
    for(int ichild = 0; ichild < nChildren; ++ichild){
      const xAOD::TruthParticle* child = truth->child(ichild);
      if(child){
	m_child_pdgId  ->back().push_back(child->pdgId());
	m_child_barcode->back().push_back(child->barcode());
	m_child_status ->back().push_back(child->status());
      }else{			
	m_child_pdgId  ->back().push_back(-99);
	m_child_barcode->back().push_back(-99);
	m_child_status ->back().push_back(-99);
      }
    }
  }


  if(m_debug) std::cout << "Leave Fill Truth " << std::endl;
  return;
}

