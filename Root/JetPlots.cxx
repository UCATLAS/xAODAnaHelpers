#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"

#include <xAODAnaHelpers/JetHists.h>
#include <xAODAnaHelpers/JetPlots.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include "TEnv.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetPlots)

JetPlots :: JetPlots () {
}

JetPlots :: JetPlots (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_plots(0)
{
}

EL::StatusCode JetPlots :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("JetPlots").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetPlots :: histInitialize ()
{

  // needed here and not in initalize since this is called first
  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("histInitialize()", "%s Failed to properly configure. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  }

  // declare class and add histograms to output
  m_plots = new JetHists(m_name, m_detailStr, m_delimiter);
  m_plots -> initialize( );
  m_plots -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetPlots :: configure ()
{
  TEnv* config = new TEnv(m_configName.c_str());
  if( !config ) {
    Error("JetPlots::configure()", "Failed to read config file!");
    Error("JetPlots::configure()", "config name : %s",m_configName.c_str());
    return EL::StatusCode::FAILURE;
  }
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_detailStr               = config->GetValue("DetailStr",       ""); 
  m_delimiter               = config->GetValue("Delimiter",       "/");

  config->Print();
  Info("configure()", "JetPlots Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetPlots :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetPlots :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetPlots :: initialize ()
{
  Info("initialize()", "JetPlots");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetPlots :: execute ()
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

  xAOD::JetContainer* jets = 0;
  if ( !m_event->retrieve( jets, m_inContainerName ).isSuccess() ){
    if ( !m_store->retrieve( jets, m_inContainerName ).isSuccess() ){
      Error("JetPlots::execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.c_str() );
      return EL::StatusCode::FAILURE;
    }
  }

  // get the highest sum pT^2 primary vertex location in the PV vector
  const xAOD::VertexContainer* vertices = 0;
  if ( !m_event->retrieve( vertices, "PrimaryVertices").isSuccess() ){
    return EL::StatusCode::FAILURE;
  }
  /*int pvLocation = HelperFunctions::getPrimaryVertexLocation(vertices);*/

  /* two ways to fill */

  // 1. pass the jet collection
  m_plots->execute( jets, eventWeight );

  /* 2. loop over the jets
  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();
  for( ; jet_itr != jet_end; ++jet_itr ) {
    m_plots->execute( *jet_itr, eventWeight );
  }
  */

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetPlots :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetPlots :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetPlots :: histFinalize () {
  // clean up memory
  if(m_plots){
    delete m_plots;
    m_plots = 0;
  }
  return EL::StatusCode::SUCCESS;
}
