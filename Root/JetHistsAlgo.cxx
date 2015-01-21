#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"

#include <xAODAnaHelpers/JetHists.h>
#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include "TEnv.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetHistsAlgo)

JetHistsAlgo :: JetHistsAlgo () {
}

JetHistsAlgo :: JetHistsAlgo (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_plots(0)
{
}

EL::StatusCode JetHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("JetHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: histInitialize ()
{

  Error("histInitialize()", "%s", m_name.c_str() );
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

EL::StatusCode JetHistsAlgo :: configure ()
{
  TEnv* config = new TEnv(m_configName.c_str());
  if( !config ) {
    Error("JetHistsAlgo::configure()", "Failed to read config file!");
    Error("JetHistsAlgo::configure()", "config name : %s",m_configName.c_str());
    return EL::StatusCode::FAILURE;
  }
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_detailStr               = config->GetValue("DetailStr",       ""); 
  m_delimiter               = config->GetValue("Delimiter",       "/");

  config->Print();
  Info("configure()", "JetHistsAlgo Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetHistsAlgo :: initialize ()
{
  Info("initialize()", "JetHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: execute ()
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
      Error("JetHistsAlgo::execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.c_str() );
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

EL::StatusCode JetHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetHistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetHistsAlgo :: histFinalize () {
  // clean up memory
  if(m_plots){
    delete m_plots;
    m_plots = 0;
  }
  return EL::StatusCode::SUCCESS;
}
