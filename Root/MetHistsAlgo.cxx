#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/MetHistsAlgo.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(MetHistsAlgo)

MetHistsAlgo :: MetHistsAlgo (std::string className) :
    Algorithm(className),
    m_plots(nullptr)
{
  m_inContainerName         = "";
  m_detailStr               = "";
  m_debug                   = false;
}

EL::StatusCode MetHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("MetHistsAlgo").ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MetHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    Error("histInitialize()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }

  // declare class and add histograms to output
  m_plots = new MetHists(m_name, m_detailStr);
  RETURN_CHECK("MetHistsAlgo::histInitialize()", m_plots -> initialize(), "");
  m_plots -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MetHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MetHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode MetHistsAlgo :: initialize ()
{
  Info("initialize()", "MetHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MetHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MetHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");


  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  const xAOD::MissingETContainer* met(nullptr);
  RETURN_CHECK("MetHistsAlgo::execute()", HelperFunctions::retrieve(met, m_inContainerName, m_event, m_store, m_verbose) ,"");

  RETURN_CHECK("MetHistsAlgo::execute()", m_plots->execute( met, eventWeight ), "");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MetHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MetHistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MetHistsAlgo :: histFinalize ()
{
  // clean up memory
  if(m_plots) delete m_plots;

  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
