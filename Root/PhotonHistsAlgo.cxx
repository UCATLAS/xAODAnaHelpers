#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <SampleHandler/MetaFields.h>

#include <xAODEgamma/PhotonContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODAnaHelpers/PhotonHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(PhotonHistsAlgo)

PhotonHistsAlgo :: PhotonHistsAlgo (std::string className) :
    Algorithm(className)
{
  m_inContainerName         = "";
  // which plots will be turned on
  m_detailStr               = "";
  // name of algo input container comes from - only if
  m_inputAlgo               = "";

  m_debug                   = false;

}

EL::StatusCode PhotonHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("PhotonHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonHistsAlgo::AddHists( std::string name ) {

  std::string fullname(m_name);
  fullname += name; // add systematic
  PhotonHists* photonHists = new PhotonHists( fullname, m_detailStr ); // add systematic
  RETURN_CHECK("PhotonHistsAlgo::AddHists", photonHists->initialize(), "");
  photonHists->record( wk() );
  m_plots[name] = photonHists;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode PhotonHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode PhotonHistsAlgo :: initialize ()
{
  Info("initialize()", m_name.c_str());

  // in case anything was missing or blank...
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    Error("initialize()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }


  // only running 1 collection
  if(m_inputAlgo.empty()) { AddHists( "" ); }
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("PhotonHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }
  double xs     =wk()->metaData()->castDouble(SH::MetaFields::crossSection    ,1);
  double eff    =wk()->metaData()->castDouble(SH::MetaFields::filterEfficiency,1);
  eventWeight *= xs * eff;

  // this will hold the collection processed
  const xAOD::PhotonContainer* inPhotons = 0;

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  if( m_inputAlgo.empty() ) {
    RETURN_CHECK("PhotonHistsAlgo::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName, m_event, m_store, m_verbose) ,("Failed to get "+m_inContainerName).c_str());

    /* two ways to fill */

    // 1. pass the photon collection
    RETURN_CHECK("PhotonHistsAlgo::execute()", m_plots[""]->execute( inPhotons, eventWeight ), "");

    /* 2. loop over the photons
       for( auto photon_itr : *inPhotons ) {
       m_plots[""]->execute( photon_itr, eventWeight );
       }
    */

  }
  else { // get the list of systematics to run over

    // get vector of string giving the names
    std::vector<std::string>* systNames(nullptr);
    RETURN_CHECK("PhotonHistsAlgo::execute()", HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, m_verbose) ,"");

    // loop over systematics
    for( auto systName : *systNames ) {
      RETURN_CHECK("PhotonHistsAlgo::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");
      if( m_plots.find( systName ) == m_plots.end() ) { this->AddHists( systName ); }
      RETURN_CHECK("PhotonHistsAlgo::execute()", m_plots[systName]->execute( inPhotons, eventWeight ), "");
    }

  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode PhotonHistsAlgo :: finalize () {
  Info("finalize()", m_name.c_str());
  if(!m_plots.empty()){
    for( auto plots : m_plots ) {
      if(plots.second) delete plots.second;
    }
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonHistsAlgo :: histFinalize () {
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
