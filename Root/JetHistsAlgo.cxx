#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "AthContainers/ConstDataVector.h"

#include <xAODAnaHelpers/JetHists.h>
#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetHistsAlgo)

JetHistsAlgo :: JetHistsAlgo () {
}

JetHistsAlgo :: JetHistsAlgo (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_type(0),
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

  Info("histInitialize()", "%s", m_name.c_str() );
  // needed here and not in initalize since this is called first
  Info("histInitialize()", "Attempting to configure using: %s", m_configName.c_str());
  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("histInitialize()", "%s failed to properly configure. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  } else {
    Info("histInitialize()", "Succesfully configured! \n");
  }

  // declare class and add histograms to output
  m_plots = new JetHists(m_name, m_detailStr);
  m_plots -> initialize( );
  m_plots -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo :: configure ()
{
  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  // check if file exists
  /* https://root.cern.ch/root/roottalk/roottalk02/5332.html */
  FileStat_t fStats;
  int fSuccess = gSystem->GetPathInfo(m_configName.c_str(), fStats);
  if(fSuccess != 0){
    Error("configure()", "Could not find the configuration file");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Found configuration file");

  // the file exists, use TEnv to read it off
  TEnv* config = new TEnv(m_configName.c_str());
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_detailStr               = config->GetValue("DetailStr",       "");

  // in case anything was missing or blank...
  if( m_inContainerName.IsNull() || m_detailStr.empty() ){
    Error("configure()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Loaded in configuration values");

  // everything seems preliminarily ok, let's print config and say we were successful
  config->Print();
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

  // this will be the collection processed - no matter what!!
  const xAOD::JetContainer* inJets = 0;

  // if type is not defined then we need to define it
  //  1 = get from TStore
  //  2 = get from TEvent
  if( m_type == 0 ) {
    if ( m_store->contains< ConstDataVector<xAOD::JetContainer> >(m_inContainerName.Data())){
      m_type = 1;  
    }
    else if ( m_event->contains<const xAOD::JetContainer>(m_inContainerName.Data())){
      m_type = 2;
    }
    else {
      Error("JetHistsAlgo::execute()  ", "Failed to retrieve %s container from File or Store. Exiting.", m_inContainerName.Data() );
      m_store->print();
      return EL::StatusCode::FAILURE;
    }
  }

  // Can retrieve collection from input file ( const )
  //           or collection from tstore ( ConstDataVector which then gives a const collection )
  // decide which on first pass
  // 
  // FIXME replace with enum
  if ( m_type == 1 ) {        // get ConstDataVector from TStore

    ConstDataVector<xAOD::JetContainer>* inJetsCDV = 0;
    if ( !m_store->retrieve( inJetsCDV, m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container from Store. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
    inJets = inJetsCDV->asDataVector();

  }  
  else if ( m_type == 2 ) {   // get const container from TEvent

    if ( !m_event->retrieve( inJets , m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container from File. Exiting.", m_inContainerName.Data() );
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
  m_plots->execute( inJets, eventWeight );

  /* 2. loop over the jets
  for( auto jet_itr : *inJets ) {
    m_plots->execute( jet_itr, eventWeight );
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
