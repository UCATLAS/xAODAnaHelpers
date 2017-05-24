#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "AthContainers/ConstDataVector.h"

#include <${package}/${name}Hists.h>
#include <${package}/${name}HistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>

#include <AsgTools/MessageCheck.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(${name}HistsAlgo)

${name}HistsAlgo :: ${name}HistsAlgo () {
}

${name}HistsAlgo :: ${name}HistsAlgo (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_plots(0)
{
}

EL::StatusCode ${name}HistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("${name}HistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}HistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );

  // declare class and add histograms to output
  m_plots = new ${name}Hists(m_name, m_detailStr);
  m_plots -> initialize( );
  m_plots -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}HistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ${name}HistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode ${name}HistsAlgo :: initialize ()
{
  Info("initialize()", "${name}HistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}HistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK(HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, m_verbose));

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  // this will be the collection processed - no matter what!!
  const xAOD::JetContainer* inJets(nullptr);
  ANA_CHECK(HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, m_verbose));

  /* two ways to fill */

  // 1. pass the jet collection
  m_plots->execute( inJets, eventWeight );

  /* 2. loop over the jets
  xAOD::${name}Container::const_iterator jet_itr = jets->begin();
  xAOD::${name}Container::const_iterator jet_end = jets->end();
  for( const auto jet: *inJets ) {
    m_plots->execute( jet, eventWeight );
  }
  */

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}HistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ${name}HistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode ${name}HistsAlgo :: histFinalize () {
  // clean up memory
  if(m_plots){
    delete m_plots;
    m_plots = 0;
  }
  return EL::StatusCode::SUCCESS;
}
