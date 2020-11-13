#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/MetHistsAlgo.h>

// this is needed to distribute the algorithm to the workers

MetHistsAlgo :: MetHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator) :
    Algorithm(name, pSvcLocator, "MetHistsAlgo")
{
    declareProperty("inContainerName", m_inContainerName);
    declareProperty("detailStr", m_detailStr);
}


StatusCode MetHistsAlgo :: histInitialize ()
{

  ANA_MSG_INFO( m_name );
  ANA_CHECK( xAH::Algorithm::algInitialize());
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    ANA_MSG_ERROR( "One or more required configuration values are empty");
    return StatusCode::FAILURE;
  }

  // declare class and add histograms to output
  m_plots = new MetHists(m_name, m_detailStr);
  ANA_CHECK( m_plots -> initialize());

  return StatusCode::SUCCESS;
}

StatusCode MetHistsAlgo :: fileExecute () { return StatusCode::SUCCESS; }
StatusCode MetHistsAlgo :: changeInput (bool /*firstFile*/) { return StatusCode::SUCCESS; }

StatusCode MetHistsAlgo :: initialize ()
{
  ANA_MSG_INFO( "MetHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return StatusCode::SUCCESS;
}

StatusCode MetHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );


  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  const xAOD::MissingETContainer* met(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(met, m_inContainerName, m_event, m_store, msg()) );

  ANA_CHECK( m_plots->execute( met, eventWeight ));

  return StatusCode::SUCCESS;
}

StatusCode MetHistsAlgo :: finalize () { return StatusCode::SUCCESS; }
StatusCode MetHistsAlgo :: histFinalize ()
{

  for( auto hist : m_plots->hists() ){
    ANA_CHECK(book(hist));
  }

  // clean up memory
  if(m_plots) delete m_plots;

  ANA_CHECK( xAH::Algorithm::algFinalize());
  return StatusCode::SUCCESS;
}
