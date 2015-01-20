#ifndef xAODAnaHelpers_HELPERFUNCTIONS_H
#define xAODAnaHelpers_HELPERFUNCTIONS_H


namespace xAOD {
#ifndef XAODTRACKING_VERTEX_H 
#ifndef XAODTRACKING_VERTEXFWD_H
    class Vertex;
#endif
#endif
#ifndef XAODTRACKING_VERTEXCONTAINER_H 
#ifndef XAODTRACKING_VERTEXCONTAINERFWD_H
    class VertexContainer;
#endif
#endif
#ifndef XAODJET_JET_H 
  class Jet;
#endif
}

namespace HelperFunctions {

  // primary vertex
  bool passPrimaryVertexSelection(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  int countPrimaryVertices(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  const xAOD::Vertex* getPrimaryVertex(const xAOD::VertexContainer* vertexContainer);
  int getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer);

  // miscellaneous
  bool sort_pt(xAOD::IParticle* partA, xAOD::IParticle* partB);

} // close namespace HelperFunctions

# endif

