#include "AthContainers/ConstDataVector.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/TrackHistsAlgo.h>

// this is needed to distribute the algorithm to the workers

TrackHistsAlgo :: TrackHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator) :
    Algorithm(name, pSvcLocator, "TrackHistsAlgo")
{
    declareProperty("inContainerName", m_inContainerName);
    declareProperty("detailStr", m_detailStr);
}


StatusCode TrackHistsAlgo :: histInitialize ()
{

  ANA_MSG_INFO( m_name );
  ANA_CHECK( xAH::Algorithm::algInitialize());
  // needed here and not in initalize since this is called first
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    ANA_MSG_ERROR( "One or more required configuration values are empty");
    return StatusCode::FAILURE;
  }


  // declare class and add histograms to output
  m_plots = new TrackHists(m_name, m_detailStr);
  ANA_CHECK( m_plots -> initialize());

  return StatusCode::SUCCESS;
}

StatusCode TrackHistsAlgo :: fileExecute () { return StatusCode::SUCCESS; }
StatusCode TrackHistsAlgo :: changeInput (bool /*firstFile*/) { return StatusCode::SUCCESS; }

StatusCode TrackHistsAlgo :: initialize ()
{
  ANA_MSG_INFO( "TrackHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return StatusCode::SUCCESS;
}

StatusCode TrackHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( evtStore()->retrieve(eventInfo, m_eventInfoContainerName) );


  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  const xAOD::TrackParticleContainer* tracks(nullptr);
  ANA_CHECK( evtStore()->retrieve(tracks, m_inContainerName) );

  // get primary vertex
  const xAOD::VertexContainer *vertices(nullptr);
  ANA_CHECK( evtStore()->retrieve(vertices, m_vertexContainerName) );
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices, msg());

  ANA_CHECK( m_plots->execute( tracks, pvx, eventWeight, eventInfo ));

  return StatusCode::SUCCESS;
}

StatusCode TrackHistsAlgo :: finalize () { return StatusCode::SUCCESS; }
StatusCode TrackHistsAlgo :: histFinalize ()
{
  for( auto hist : m_plots->hists() ){
    ANA_CHECK(book(hist));
  }

  // clean up memory
  if(m_plots) delete m_plots;
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return StatusCode::SUCCESS;
}
