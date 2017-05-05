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
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// external tools include(s):

// ROOT include(s):
#include "TFile.h"

// this is needed to distribute the algorithm to the workers
ClassImp(DebugTool)


DebugTool :: DebugTool (std::string className) :
    Algorithm(className),
    m_printStore(false)
{
  ATH_MSG_INFO( "Calling constructor");
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
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");

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
  if ( m_debug ) { ATH_MSG_INFO( " "); }

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
  if ( m_debug ) { ATH_MSG_INFO( "Calling postExecute"); }
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
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}



