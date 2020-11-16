#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/ClusterHistsAlgo.h>

// this is needed to distribute the algorithm to the workers

ClusterHistsAlgo :: ClusterHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator) :
    Algorithm(name, pSvcLocator, "ClusterHistsAlgo")
{
    declareProperty("inContainerName", m_inContainerName);
    declareProperty("detailStr", m_detailStr);
}


StatusCode ClusterHistsAlgo :: histInitialize ()
{

  ANA_MSG_INFO( m_name );
  ANA_CHECK( xAH::Algorithm::algInitialize());
  // needed here and not in initalize since this is called first
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    ANA_MSG_ERROR( "One or more required configuration values are empty");
    return StatusCode::FAILURE;
  }


  // declare class and add histograms to output
  m_plots = new ClusterHists(m_name, m_detailStr);
  ANA_CHECK( m_plots -> initialize());

  return StatusCode::SUCCESS;
}

StatusCode ClusterHistsAlgo :: fileExecute () { return StatusCode::SUCCESS; }
StatusCode ClusterHistsAlgo :: changeInput (bool /*firstFile*/) { return StatusCode::SUCCESS; }

StatusCode ClusterHistsAlgo :: initialize ()
{
  ANA_MSG_INFO( "ClusterHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return StatusCode::SUCCESS;
}

StatusCode ClusterHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( evtStore()->retrieve(eventInfo, m_eventInfoContainerName) );


  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  const xAOD::CaloClusterContainer* ccls(nullptr);
  ANA_CHECK( evtStore()->retrieve(ccls, m_inContainerName) );

  ANA_CHECK( m_plots->execute( ccls, eventWeight ));

  return StatusCode::SUCCESS;
}

StatusCode ClusterHistsAlgo :: finalize () { return StatusCode::SUCCESS; }
StatusCode ClusterHistsAlgo :: histFinalize ()
{

  for( auto hist : m_plots->hists() ){
    ANA_CHECK(book(hist));
  }

  // clean up memory
  if(m_plots) delete m_plots;
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return StatusCode::SUCCESS;
}
