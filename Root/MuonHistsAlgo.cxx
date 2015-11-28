#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODMuon/MuonContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODAnaHelpers/MuonHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(MuonHistsAlgo)

MuonHistsAlgo :: MuonHistsAlgo (std::string className) :
    Algorithm(className)
{
  m_inContainerName         = "";
  // which plots will be turned on
  m_detailStr               = "";
  // name of algo input container comes from - only if
  m_inputAlgo               = "";

  m_debug                   = false;

}

EL::StatusCode MuonHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("MuonHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo::AddHists( std::string name ) {

  std::string fullname(m_name);
  fullname += name; // add systematic
  MuonHists* muonHists = new MuonHists( fullname, m_detailStr ); // add systematic
  RETURN_CHECK("MuonHistsAlgo::AddHists", muonHists->initialize(), "");
  muonHists->record( wk() );
  m_plots[name] = muonHists;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo :: configure ()
{
  if(!getConfig().empty()){
    // the file exists, use TEnv to read it off
    TEnv* config = new TEnv(getConfig(true).c_str());
    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());
    // which plots will be turned on
    m_detailStr               = config->GetValue("DetailStr",       m_detailStr.c_str());
    // name of algo input container comes from - only if
    m_inputAlgo               = config->GetValue("InputAlgo",       m_inputAlgo.c_str());

    m_debug                   = config->GetValue("Debug" ,           m_debug);

    Info("configure()", "Loaded in configuration values");

    // everything seems preliminarily ok, let's print config and say we were successful
    config->Print();
    delete config;
  }

  // in case anything was missing or blank...
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    Error("configure()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MuonHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode MuonHistsAlgo :: initialize ()
{
  Info("initialize()", m_name.c_str());

  // needed here and not in initalize since this is called first
  Info("histInitialize()", "Attempting to configure using: %s", m_configName.c_str());
  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("histInitialize()", "%s failed to properly configure. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  } else {
    Info("histInitialize()", "Succesfully configured! ");
  }

  // only running 1 collection
  if(m_inputAlgo.empty()) { AddHists( "" ); }
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  // this will hold the collection processed
  const xAOD::MuonContainer* inMuons = 0;

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  if( m_inputAlgo.empty() ) {
    RETURN_CHECK("MuonHistsAlgo::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName, m_event, m_store, m_verbose) ,("Failed to get "+m_inContainerName).c_str());

    /* two ways to fill */

    // 1. pass the muon collection
    RETURN_CHECK("MuonHistsAlgo::execute()", m_plots[""]->execute( inMuons, eventWeight ), "");

    /* 2. loop over the muons
       for( auto muon_itr : *inMuons ) {
       m_plots[""]->execute( muon_itr, eventWeight );
       }
    */

  }
  else { // get the list of systematics to run over

    // get vector of string giving the names
    std::vector<std::string>* systNames(nullptr);
    RETURN_CHECK("MuonHistsAlgo::execute()", HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, m_verbose) ,"");

    // loop over systematics
    for( auto systName : *systNames ) {
      RETURN_CHECK("MuonHistsAlgo::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");
      if( m_plots.find( systName ) == m_plots.end() ) { this->AddHists( systName ); }
      RETURN_CHECK("MuonHistsAlgo::execute()", m_plots[systName]->execute( inMuons, eventWeight ), "");
    }

  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode MuonHistsAlgo :: finalize () {
  Info("finalize()", m_name.c_str());
  if(!m_plots.empty()){
    for( auto plots : m_plots ) {
      if(plots.second) delete plots.second;
    }
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo :: histFinalize () {
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
