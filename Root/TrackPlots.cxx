#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"

#include <xAODAnaHelpers/TrackHists.h>
#include <xAODAnaHelpers/TrackPlots.h>

#include "TEnv.h"

// this is needed to distribute the algorithm to the workers
ClassImp(TrackPlots)

TrackPlots :: TrackPlots (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_plots(0)
{
}

EL::StatusCode TrackPlots :: setupJob (EL::Job& job)
{
  job.useXAOD();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("TrackPlots").ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackPlots :: histInitialize ()
{

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("histInitialize()", "%s Failed to properly configure. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  }

  // declare class and add histograms to output
  m_plots = new TrackHists(m_name, m_detailStr, m_delimiter);
  m_plots -> initialize();
  m_plots -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackPlots :: configure ()
{
  TEnv* config = new TEnv(m_configName.c_str());
  if( !config ) {
    Error("TrackPlots::setupJob()", "Failed to read config file!");
    Error("TrackPlots::setupJob()", "config name : %s",m_configName.c_str());
    return EL::StatusCode::FAILURE;
  }
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_detailStr               = config->GetValue("DetailStr",       "");
  m_delimiter               = config->GetValue("Delimiter",      "/");

  config->Print();
  Info("configure()", "JetPlots Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackPlots :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TrackPlots :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackPlots :: initialize ()
{
  Info("initialize()", "TrackPlots");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackPlots :: execute ()
{
  const xAOD::EventInfo* eventInfo = 0;
  if ( ! m_event->retrieve(eventInfo, "EventInfo").isSuccess() ) {
    Error("AnalysisLoop::execute()", "Failed to retrieve event info collection. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  const xAOD::TrackParticleContainer* tracks = 0;
  if ( !m_event->retrieve( tracks, m_inContainerName ).isSuccess() ){
    if ( !m_store->retrieve( tracks, m_inContainerName ).isSuccess() ){
      Error("TrackPlots::execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.c_str() );
      return EL::StatusCode::FAILURE;
    }
  }

  m_plots->execute( tracks, eventWeight );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackPlots :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TrackPlots :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TrackPlots :: histFinalize ()
{
  // clean up memory
  if(m_plots){
    delete m_plots;
    m_plots = 0;
  }
  return EL::StatusCode::SUCCESS;
}
