#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "AthContainers/ConstDataVector.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/TrackHists.h>
#include <xAODAnaHelpers/TrackHistsAlgo.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(TrackHistsAlgo)

TrackHistsAlgo :: TrackHistsAlgo () {
}

TrackHistsAlgo :: TrackHistsAlgo (std::string name, std::string configName, std::string containerName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_inContainerName(containerName),
  m_plots(0)
{
}

EL::StatusCode TrackHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("TrackHistsAlgo").ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );
  // needed here and not in initalize since this is called first
  Info("histInitialize()", "Attempting to configure using: %s", m_configName.c_str());
  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("histInitialize()", "%s failed to properly configure. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  } else {
    Info("histInitialize()", "Successfully configured! \n");
  }

  // declare class and add histograms to output
  m_plots = new TrackHists(m_name, m_detailStr);
  m_plots -> initialize();
  m_plots -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackHistsAlgo :: configure ()
{
  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "TrackHistsAlgo::configure()", m_configName);

  // the file exists, use TEnv to read it off
  TEnv* config = new TEnv(m_configName.c_str());

  //
  //  If Container Name is already set dont read it from the config
  //   (Allows to pass as argument in setup script)
  //
  if(m_inContainerName.empty())
     m_inContainerName         = config->GetValue("InputContainer",  "");

  m_detailStr               = config->GetValue("DetailStr",       "");

  if( m_inContainerName.empty() || m_detailStr.empty() ){
    Error("configure()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Loaded in configuration values");

  // everything seems preliminarily ok, let's print config and say we were successful
  config->Print();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TrackHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackHistsAlgo :: initialize ()
{
  Info("initialize()", "TrackHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo = HelperFunctions::getContainer<xAOD::EventInfo>("EventInfo", m_event, m_store);;

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  const xAOD::TrackParticleContainer* tracks = HelperFunctions::getContainer<xAOD::TrackParticleContainer>(m_inContainerName, m_event, m_store);;

  // get primary vertex
  const xAOD::VertexContainer *vertices = HelperFunctions::getContainer<xAOD::VertexContainer>("PrimaryVertices", m_event, m_store);
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);

  m_plots->execute( tracks, pvx, eventWeight );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TrackHistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TrackHistsAlgo :: histFinalize ()
{
  // clean up memory
  if(m_plots){
    delete m_plots;
    m_plots = 0;
  }
  return EL::StatusCode::SUCCESS;
}
