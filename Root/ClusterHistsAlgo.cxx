#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/ClusterHistsAlgo.h>

// this is needed to distribute the algorithm to the workers
ClassImp(ClusterHistsAlgo)

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
  m_plots -> record( wk() );

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
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );


  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  const xAOD::CaloClusterContainer* ccls(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(ccls, m_inContainerName, m_event, m_store, msg()) );

  ANA_CHECK( m_plots->execute( ccls, eventWeight ));

  return StatusCode::SUCCESS;
}

StatusCode ClusterHistsAlgo :: postExecute () { return StatusCode::SUCCESS; }
StatusCode ClusterHistsAlgo :: finalize () { return StatusCode::SUCCESS; }
StatusCode ClusterHistsAlgo :: histFinalize ()
{
  // clean up memory
  if(m_plots) delete m_plots;
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return StatusCode::SUCCESS;
}
