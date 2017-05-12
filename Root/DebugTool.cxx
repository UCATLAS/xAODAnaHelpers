/************************************
 *
 * Debug tool
 *
 * J.Alison (john.alison@cern.ch)
 *
 ************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
//#include "PATInterfaces/SystematicVariation.h"
//#include "PATInterfaces/SystematicRegistry.h"
//#include "PATInterfaces/SystematicCode.h"

// package include(s):
//#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/DebugTool.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include <AsgTools/MessageCheck.hh>

// external tools include(s):

// ROOT include(s):
#include "TFile.h"

// this is needed to distribute the algorithm to the workers
ClassImp(DebugTool)


DebugTool :: DebugTool () :
    Algorithm("DebugTool")
{
}

EL::StatusCode DebugTool :: setupJob (EL::Job& job)
{
  ATH_MSG_INFO( "Calling setupJob");
  job.useXAOD ();
  xAOD::Init( "DebugTool" ).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: histInitialize ()
{
  ATH_MSG_INFO( "Calling histInitialize");
  ANA_CHECK( xAH::Algorithm::algInitialize());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: fileExecute ()
{
  ATH_MSG_INFO( "Calling fileExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: changeInput (bool /*firstFile*/)
{
  ATH_MSG_INFO( "Calling changeInput");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: initialize ()
{
  ATH_MSG_INFO( " ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ATH_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: execute ()
{
  ATH_MSG_INFO( m_name);

  //
  // look what we have in TStore
  //
  if ( m_printStore ) {
    m_store->print();
  }

  return EL::StatusCode::SUCCESS;

}


EL::StatusCode DebugTool :: postExecute ()
{
  ATH_MSG_DEBUG("Calling postExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: finalize ()
{
  ATH_MSG_INFO( m_name );
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: histFinalize ()
{
  ATH_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}



