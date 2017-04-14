#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/ClusterHistsAlgo.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(ClusterHistsAlgo)

ClusterHistsAlgo :: ClusterHistsAlgo (std::string className) :
    Algorithm(className),
    m_plots(nullptr)
{
  m_inContainerName         = "";
  m_detailStr               = "";
  m_debug                   = false;

}

EL::StatusCode ClusterHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("ClusterHistsAlgo").ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ClusterHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  // needed here and not in initalize since this is called first
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    Error("histInitialize()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }


  // declare class and add histograms to output
  m_plots = new ClusterHists(m_name, m_detailStr);
  RETURN_CHECK("ClusterHistsAlgo::histInitialize()", m_plots -> initialize(), "");
  m_plots -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ClusterHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ClusterHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode ClusterHistsAlgo :: initialize ()
{
  Info("initialize()", "ClusterHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ClusterHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ClusterHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");


  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  const xAOD::CaloClusterContainer* ccls(nullptr);
  RETURN_CHECK("ClusterHistsAlgo::execute()", HelperFunctions::retrieve(ccls, m_inContainerName, m_event, m_store, m_verbose) ,"");

  RETURN_CHECK("ClusterHistsAlgo::execute()", m_plots->execute( ccls, eventWeight ), "");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ClusterHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ClusterHistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ClusterHistsAlgo :: histFinalize ()
{
  // clean up memory
  if(m_plots) delete m_plots;
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
