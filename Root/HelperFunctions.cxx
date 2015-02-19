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
  xAOD::VertexContainer::const_iterator vtx_itr = vertexContainer->begin();
  xAOD::VertexContainer::const_iterator vtx_end = vertexContainer->end();
  for( ; vtx_itr != vtx_end; ++vtx_itr )
  {
    if((int)(*vtx_itr)->nTrackParticles() < Ntracks ) { continue; }
    NPV++;
  }

  return NPV;

}

int HelperFunctions::getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer)
{
  xAOD::VertexContainer::const_iterator vtx_itr = vertexContainer->begin();
  xAOD::VertexContainer::const_iterator vtx_end = vertexContainer->end();
  int location(0);
  for( ; vtx_itr != vtx_end; ++vtx_itr )
  {
    if((*vtx_itr)->vertexType() == xAOD::VxType::VertexType::PriVtx) { 
      return location;
    }
    location++;
  }
  return -1;
}


const xAOD::Vertex* HelperFunctions::getPrimaryVertex(const xAOD::VertexContainer* vertexContainer)
{
  // vertex types are listed on L328 of
  // https://svnweb.cern.ch/trac/atlasoff/browser/Event/xAOD/xAODTracking/trunk/xAODTracking/TrackingPrimitives.h
  xAOD::VertexContainer::const_iterator vtx_itr = vertexContainer->begin();
  xAOD::VertexContainer::const_iterator vtx_end = vertexContainer->end();
  for( ; vtx_itr != vtx_end; ++vtx_itr )
  {
    if((*vtx_itr)->vertexType() != xAOD::VxType::VertexType::PriVtx) { continue; }
    return (*vtx_itr);
  }

  return 0;
}

bool HelperFunctions::sort_pt(xAOD::IParticle* partA, xAOD::IParticle* partB){
  return partA->pt() > partB->pt();
}

template< typename T1, typename T2 >
bool HelperFunctions::makeSubsetCont( T1*& intCont, T2*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name ) 
{
  // iterate over input container
  for ( auto in_itr = intCont->begin();  in_itr != intCont->end(); ++in_itr ) {
    static SG::AuxElement::ConstAccessor<char> myAccessor(flagSelect);
    if(!myAccessor.isAvailable(*(*in_itr))){
      std::stringstream ss; ss << (*in_itr)->type();
      std::cout<< "HelperFunctions::makeSubsetCont() - flag " << flagSelect << " is missing for object of type " << (ss.str()).c_str() << " ! Will not make a subset of its container" << std::endl;
      return false;
    } 
    
    if (tool_name == HelperClasses::ToolName::OVERLAPREMOVER){ // this tool uses reverted logic
      if ( !myAccessor(*(*in_itr)) ){ outCont->push_back( *in_itr ); }  
    } else {
      if ( myAccessor(*(*in_itr)) ){ outCont->push_back( *in_itr ); }
    }
  }
  return true;
}
template bool HelperFunctions::makeSubsetCont< const xAOD::ElectronContainer, ConstDataVector<xAOD::ElectronContainer> >(const xAOD::ElectronContainer*& intCont, ConstDataVector<xAOD::ElectronContainer>*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont< const xAOD::MuonContainer, ConstDataVector<xAOD::MuonContainer> >(const xAOD::MuonContainer*& intCont, ConstDataVector<xAOD::MuonContainer>*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont< const xAOD::JetContainer, ConstDataVector<xAOD::JetContainer> >(const xAOD::JetContainer*& intCont, ConstDataVector<xAOD::JetContainer>*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont< const xAOD::PhotonContainer, ConstDataVector<xAOD::PhotonContainer> >(const xAOD::PhotonContainer*& intCont, ConstDataVector<xAOD::PhotonContainer>*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont< const xAOD::TauJetContainer, ConstDataVector<xAOD::TauJetContainer> >(const xAOD::TauJetContainer*& intCont, ConstDataVector<xAOD::TauJetContainer>*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );

template bool HelperFunctions::makeSubsetCont<xAOD::ElectronContainer , xAOD::ElectronContainer >( xAOD::ElectronContainer*& intCont ,xAOD::ElectronContainer*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont<xAOD::MuonContainer     , xAOD::MuonContainer     >( xAOD::MuonContainer*&     intCont ,xAOD::MuonContainer*&     outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont<xAOD::JetContainer      , xAOD::JetContainer      >( xAOD::JetContainer*&      intCont ,xAOD::JetContainer*&      outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont<xAOD::PhotonContainer   , xAOD::PhotonContainer   >( xAOD::PhotonContainer*&   intCont ,xAOD::PhotonContainer*&   outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name );
template bool HelperFunctions::makeSubsetCont<xAOD::TauJetContainer   , xAOD::TauJetContainer   >( xAOD::TauJetContainer*&   intCont ,xAOD::TauJetContainer*&   outCont, const std::string& flagSelect, HelperClasses::ToolName
 tool_name );
