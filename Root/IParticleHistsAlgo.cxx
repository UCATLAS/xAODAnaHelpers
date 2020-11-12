#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODBase/IParticleContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODAnaHelpers/IParticleHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>

// this is needed to distribute the algorithm to the workers
ClassImp(IParticleHistsAlgo)

IParticleHistsAlgo :: IParticleHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator, std::string className) :
    Algorithm(className)
{
    declareProperty("inContainerName", m_inContainerName);
    declareProperty("detailStr", m_detailStr);
    declareProperty("inputAlgo", m_inputAlgo);
    declareProperty("histPrefix", m_histPrefix);
    declareProperty("histTitle", m_histTitle);
}


StatusCode IParticleHistsAlgo :: histInitialize ()
{

  ANA_MSG_INFO( m_name );
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return StatusCode::SUCCESS;
}

StatusCode IParticleHistsAlgo::AddHists( std::string name ) {
  std::string fullname(m_name);
  fullname += name; // add systematic
  IParticleHists* particleHists = new IParticleHists( fullname, m_detailStr, m_histPrefix, m_histTitle ); // add systematic
  particleHists->m_debug = msgLvl(MSG::DEBUG);
  ANA_CHECK( particleHists->initialize());
  particleHists->record( wk() );
  m_plots[name] = particleHists;

  return StatusCode::SUCCESS;
}

StatusCode IParticleHistsAlgo :: fileExecute () { return StatusCode::SUCCESS; }
StatusCode IParticleHistsAlgo :: changeInput (bool /*firstFile*/) { return StatusCode::SUCCESS; }

StatusCode IParticleHistsAlgo :: initialize ()
{
  ANA_MSG_DEBUG( m_name);

  // in case anything was missing or blank...
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    ANA_MSG_ERROR( "One or more required configuration values are empty");
    return StatusCode::FAILURE;
  }


  // only running 1 collection
  if(m_inputAlgo.empty()) { AddHists( "" ); }
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return StatusCode::SUCCESS;
}

StatusCode IParticleHistsAlgo :: execute ()
{
  return execute<IParticleHists, xAOD::IParticleContainer>();
}

StatusCode IParticleHistsAlgo :: postExecute () { return StatusCode::SUCCESS; }

StatusCode IParticleHistsAlgo :: finalize () {
  ANA_MSG_DEBUG( m_name );
  for( auto plots : m_plots ) {
    if(plots.second){
      plots.second->finalize();
      delete plots.second;
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode IParticleHistsAlgo :: histFinalize () {
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return StatusCode::SUCCESS;
}
