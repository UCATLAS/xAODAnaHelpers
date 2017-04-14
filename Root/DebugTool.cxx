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
  Info("DebugTool()", "Calling constructor");
}

EL::StatusCode DebugTool :: setupJob (EL::Job& job)
{
  Info("setupJob()", "Calling setupJob");
  job.useXAOD ();
  xAOD::Init( "DebugTool" ).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: histInitialize ()
{
  Info("histInitialize()", "Calling histInitialize");
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: fileExecute ()
{
  Info("fileExecute()", "Calling fileExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: changeInput (bool /*firstFile*/)
{
  Info("changeInput()", "Calling changeInput");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: initialize ()
{
  Info("initialize()", " ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: execute ()
{
  if ( m_debug ) { Info("execute()", " "); }

  Info("DebugTool", m_name.c_str());

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
  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: finalize ()
{
  Info("finalize()", "%s", m_name.c_str());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DebugTool :: histFinalize ()
{
  Info("histFinalize()", "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}



