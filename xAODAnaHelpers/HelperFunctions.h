#ifndef xAODAnaHelpers_HELPERFUNCTIONS_H
#define xAODAnaHelpers_HELPERFUNCTIONS_H

#ifndef __CINT__
  #include "xAODTracking/VertexContainer.h"
  #include "xAODEgamma/ElectronContainer.h"
  #include "xAODEgamma/Electron.h"
  #include "xAODEgamma/PhotonContainer.h"
  #include "xAODEgamma/Photon.h"
  #include "xAODMuon/MuonContainer.h"
  #include "xAODMuon/Muon.h"
  #include "xAODJet/JetContainer.h"
  #include "xAODJet/Jet.h"
  #include "xAODTau/TauJetContainer.h"
  #include "xAODTau/TauJet.h"  
  #include "xAODBase/IParticleHelpers.h"
  #include "xAODBase/IParticleContainer.h"
  #include "xAODBase/IParticle.h"
  
  #include "AthContainers/ConstDataVector.h"
#endif

#ifndef __CINT__
  #include "xAODAnaHelpers/HelperClasses.h"
#endif

namespace HelperFunctions {

  // primary vertex
  bool passPrimaryVertexSelection(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  int countPrimaryVertices(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  const xAOD::Vertex* getPrimaryVertex(const xAOD::VertexContainer* vertexContainer);
  int getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer);

  // miscellaneous
  bool sort_pt(xAOD::IParticle* partA, xAOD::IParticle* partB);

#ifndef __CINT__
  template< typename T1, typename T2 >
    bool makeSubsetCont( T1*& intCont, T2*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name ); // template function to copy a subset of intCont into outCont
# endif

} // close namespace HelperFunctions

# endif

