#include "xAODAnaHelpers/HelperFunctions.h"

// Get Number of Vertices with at least Ntracks
bool HelperFunctions::passPrimaryVertexSelection(const xAOD::VertexContainer* vertexContainer, int Ntracks)
{
  const xAOD::Vertex* primaryVertex = getPrimaryVertex( vertexContainer );
  if(!primaryVertex){ return false; }
  if((int)(primaryVertex)->nTrackParticles() < Ntracks ) {
    return false;
  }
  return true;
}

int HelperFunctions::countPrimaryVertices(const xAOD::VertexContainer* vertexContainer, int Ntracks)
{

  int NPV = 0;

  // Loop over vertices in the container
  for( auto vtx_itr : *vertexContainer )
  {
    if((int)vtx_itr->nTrackParticles() < Ntracks ) { continue; }
    NPV++;
  }

  return NPV;

}

int HelperFunctions::getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer)
{
  int location(0);
  for( auto vtx_itr : *vertexContainer )
  {
    if(vtx_itr->vertexType() == xAOD::VxType::VertexType::PriVtx) {
      return location;
    }
    location++;
  }
  return -1;
}

  std::string replaceString(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
      subject.replace(pos, search.length(), replace);
      pos += replace.length();
    }
    return subject;
  }


const xAOD::Vertex* HelperFunctions::getPrimaryVertex(const xAOD::VertexContainer* vertexContainer)
{
  // vertex types are listed on L328 of
  // https://svnweb.cern.ch/trac/atlasoff/browser/Event/xAOD/xAODTracking/trunk/xAODTracking/TrackingPrimitives.h
  for( auto vtx_itr : *vertexContainer )
  {
    if(vtx_itr->vertexType() != xAOD::VxType::VertexType::PriVtx) { continue; }
    return vtx_itr;
  }

  return 0;
}

bool HelperFunctions::sort_pt(xAOD::IParticle* partA, xAOD::IParticle* partB){
  return partA->pt() > partB->pt();
}
