#include "xAODAnaHelpers/VertexContainer.h"
#include <iostream>

using namespace xAH;

VertexContainer::VertexContainer(const std::string& detailStr, const std::string& name)
{
  m_name      = name;
  m_detailStr = detailStr;
  if(m_detailStr!=""){
    m_vertex_x  = new std::vector<float>();
    m_vertex_y  = new std::vector<float>();
    m_vertex_z  = new std::vector<float>();
  }
}

VertexContainer::~VertexContainer()
{
  if(m_detailStr!=""){
    delete m_vertex_x;
    delete m_vertex_y;
    delete m_vertex_z;
  }
}

void VertexContainer::setTree(TTree *tree)
{
  if(m_detailStr!=""){
    connectBranch<float>(tree,"x",&m_vertex_x);
    connectBranch<float>(tree,"y",&m_vertex_y);
    connectBranch<float>(tree,"z",&m_vertex_z);
  }
}

void VertexContainer::setBranches(TTree *tree)
{
  if(m_detailStr!=""){
    setBranch<float>(tree,"x",m_vertex_x);
    setBranch<float>(tree,"y",m_vertex_y);
    setBranch<float>(tree,"z",m_vertex_z);
  }
  return;
}

void VertexContainer::clear()
{
  if(m_detailStr!=""){
    m_vertex_x->clear();
    m_vertex_y->clear();
    m_vertex_z->clear();
  }
  return;
}

void VertexContainer::FillVertices( const xAOD::VertexContainer* vertices){
  if(m_detailStr == "primary"){ // hard-scatter vertex only
    int pvLocation = HelperFunctions::getPrimaryVertexLocation( vertices );
    m_vertex_x->push_back( vertices->at(pvLocation)->x() );
    m_vertex_y->push_back( vertices->at(pvLocation)->y() );
    m_vertex_z->push_back( vertices->at(pvLocation)->z() );
  } else if (m_detailStr == "all"){
    for( auto vertex : *vertices) {
      m_vertex_x->push_back( vertex->x() );
      m_vertex_y->push_back( vertex->y() );
      m_vertex_z->push_back( vertex->z() );
    }
  }
}
