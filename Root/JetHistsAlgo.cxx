#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODJet/JetContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetHistsAlgo)

JetHistsAlgo :: JetHistsAlgo () {
  m_inContainerName         = "";
  // which plots will be turned on
  m_detailStr               = "";
  // name of algo input container comes from - only if
  m_inputAlgo               = "";

  m_debug                   = false;

}

EL::StatusCode JetHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("JetHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo::AddHists( std::string name ) {

  std::string fullname(m_name);
  fullname += name; // add systematic
  JetHists* jetHists = new JetHists( fullname, m_detailStr ); // add systematic
  RETURN_CHECK("JetHistsAlgo::AddHists", jetHists->initialize(), "");
  jetHists->record( wk() );
  m_plots[name] = jetHists;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: configure ()
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

EL::StatusCode JetHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetHistsAlgo :: initialize ()
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

EL::StatusCode JetHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  // get the highest sum pT^2 primary vertex location in the PV vector
  const xAOD::VertexContainer* vertices(nullptr);
  RETURN_CHECK("JetHistsAlgo::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  int pvLocation = HelperFunctions::getPrimaryVertexLocation(vertices);

  // this will hold the collection processed
  const xAOD::JetContainer* inJets = 0;

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  if( m_inputAlgo.empty() ) {
    RETURN_CHECK("JetHistsAlgo::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, m_verbose) ,("Failed to get "+m_inContainerName).c_str());

    /* two ways to fill */

    // 1. pass the jet collection
    RETURN_CHECK("JetHistsAlgo::execute()", m_plots[""]->execute( inJets, eventWeight, pvLocation ), "");

    /* 2. loop over the jets
       for( auto jet_itr : *inJets ) {
       m_plots[""]->execute( jet_itr, eventWeight, pvLocation );
       }
    */

  }
  else { // get the list of systematics to run over

    // get vector of string giving the names
    std::vector<std::string>* systNames(nullptr);
    RETURN_CHECK("JetHistsAlgo::execute()", HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, m_verbose) ,"");

    // loop over systematics
    for( auto systName : *systNames ) {
      RETURN_CHECK("JetHistsAlgo::execute()", HelperFunctions::retrieve(inJets, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");
      if( m_plots.find( systName ) == m_plots.end() ) { this->AddHists( systName ); }
      RETURN_CHECK("JetHistsAlgo::execute()", m_plots[systName]->execute( inJets, eventWeight, pvLocation ), "");
    }

  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetHistsAlgo :: finalize () {
  Info("finalize()", m_name.c_str());
  if(!m_plots.empty()){
    for( auto plots : m_plots ) {
      if(plots.second) delete plots.second;
    }
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
