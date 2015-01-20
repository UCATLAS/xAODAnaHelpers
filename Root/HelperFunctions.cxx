#include "xAODTracking/VertexContainer.h"
#include "xAODJet/Jet.h"
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



/*
  // function to apply cp tools and make shallow copies of generic xAOD Containers
  //https://lost-contact.mit.edu/afs/cern.ch/atlas/software/builds/AthAnalysisBase/latest/Event/xAOD/xAODBase/doc/mainpage.h
  
  #include "xAODBase/IParticleContainer.h"
  #include "AsgTools/AsgTool.h"
  #include "xAODCore/ShallowCopy.h"  
  
  // before calling this function somewhere else, check that the tool you want to pass is an ASG tool!
  
  if( dynamic_cast<AsgTool*>(myTool) ){
      calibrateAndSC( myxAODContainer, xAOD::myType ,myTool );
  }
       
  
  
  void HelperFunctions::calibrateAndSC( xAOD::IParticleContainer* my_xAODContainer, xAOD::Type type , AsgTool* my_AsgTool ){
     

     std::pair< xAOD::IParticleContainer*, xAOD::ShallowAuxContainer* > sc_xAODContainer = xAOD::shallowCopyContainer( *my_xAODContainer );
     
     auto sc_itr =  (sc_xAODContainer.first)->begin();
     auto sc_end =  (sc_xAODContainer.first)->end();

     if (m_sort) {
       std::sort( sc_xAODContainer.first->begin(), sc_xAODContainer.first->end(), HelperFunctions::sort_pt );
     }  
     
     // add shallow copy to TStore
     if( !m_store->record( sc_xAODContainer.first, m_outContainerName.Data() ).isSuccess() ){
    	Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName.Data() );
    	return EL::StatusCode::FAILURE;
     }
     if( !m_store->record( sc_xAODContainer.second, m_outAuxContainerName.Data() ).isSuccess() ){
    	Error("execute()  ", "Failed to store aux container %s. Exiting.", m_outAuxContainerName.Data() );
    	return EL::StatusCode::FAILURE;
     }     
     
  }
  


*/



