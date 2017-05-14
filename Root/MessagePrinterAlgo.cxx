// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

#include <xAODAnaHelpers/MessagePrinterAlgo.h>

// this is needed to distribute the algorithm to the workers
ClassImp(MessagePrinterAlgo)

MessagePrinterAlgo :: MessagePrinterAlgo () :
    Algorithm("MessagePrinterAlgo")
{
}


EL::StatusCode MessagePrinterAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("MessagePrinterAlgo").ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MessagePrinterAlgo :: histInitialize ()
{
  ANA_MSG_INFO( "Calling histInitialize");
  if(numInstances() != 0){
    ANA_MSG_FATAL( "More than one instance of MessagePrinterAlgo was created. Aborting.");
    return EL::StatusCode::FAILURE;
  }
  ANA_CHECK( xAH::Algorithm::algInitialize());

  m_messagePrinter.reset(new asg::MessagePrinter(m_sourceWidth));
  m_messagePrinterOverlay.reset(new asg::MessagePrinterOverlay(m_messagePrinter.get()));

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MessagePrinterAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MessagePrinterAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }
EL::StatusCode MessagePrinterAlgo :: initialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MessagePrinterAlgo :: execute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MessagePrinterAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MessagePrinterAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode MessagePrinterAlgo :: histFinalize ()
{
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}
