#include "xAODAnaHelpers/TruthVertexContainer.h"
#include <iostream>

using namespace xAH;

TruthVertexContainer::TruthVertexContainer(const std::string& detailStr, const std::string& name)
{
  m_name      = name;
  m_detailStr = detailStr;
  if(m_detailStr!=""){
    m_vertex_x  = new std::vector<float>();
    m_vertex_y  = new std::vector<float>();
    m_vertex_z  = new std::vector<float>();
  }
}

TruthVertexContainer::~TruthVertexContainer()
{
  if(m_detailStr!=""){
    delete m_vertex_x;
    delete m_vertex_y;
    delete m_vertex_z;
  }
}

void TruthVertexContainer::setTree(TTree *tree)
{
  if(m_detailStr!=""){
    connectBranch<float>(tree,"x",&m_vertex_x);
    connectBranch<float>(tree,"y",&m_vertex_y);
    connectBranch<float>(tree,"z",&m_vertex_z);
  }
}

void TruthVertexContainer::setBranches(TTree *tree)
{
  if(m_detailStr!=""){
    setBranch<float>(tree,"x",m_vertex_x);
    setBranch<float>(tree,"y",m_vertex_y);
    setBranch<float>(tree,"z",m_vertex_z);
  }
  return;
}

void TruthVertexContainer::clear()
{
  if(m_detailStr!=""){
    m_vertex_x->clear();
    m_vertex_y->clear();
    m_vertex_z->clear();
  }
  return;
}

void TruthVertexContainer::FillTruthVertices( const xAOD::TruthVertexContainer* truthVertices){
  if(m_detailStr == "primary"){ // hard-scatter vertex only
    int hsBarcode = -999;
    const xAOD::TruthVertex* hsTruthVertex(nullptr);
    for ( auto *truthVertex : *truthVertices ) {
      if ( truthVertex->barcode()<0 && truthVertex->barcode()>hsBarcode ) {
        hsBarcode = truthVertex->barcode();
        hsTruthVertex = truthVertex;
      }
    }
    m_vertex_x->push_back( hsTruthVertex->x() );
    m_vertex_y->push_back( hsTruthVertex->y() );
    m_vertex_z->push_back( hsTruthVertex->z() );
  } else if (m_detailStr == "all"){
    for( auto truthVertex : *truthVertices) {
      m_vertex_x->push_back( truthVertex->x() );
      m_vertex_y->push_back( truthVertex->y() );
      m_vertex_z->push_back( truthVertex->z() );
    }
  }
}
