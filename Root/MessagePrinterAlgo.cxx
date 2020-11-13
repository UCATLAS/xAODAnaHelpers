// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

#include <xAODAnaHelpers/MessagePrinterAlgo.h>

// this is needed to distribute the algorithm to the workers

MessagePrinterAlgo :: MessagePrinterAlgo (const std::string& name, ISvcLocator *pSvcLocator) :
    Algorithm(name, pSvcLocator, "MessagePrinterAlgo")
{
    declareProperty("sourceWidth", m_sourceWidth);
}


StatusCode MessagePrinterAlgo :: histInitialize ()
{
  ANA_CHECK( xAH::Algorithm::algInitialize());
  if(numInstances() != 1){
    ANA_MSG_FATAL( "More than one instance of MessagePrinterAlgo was created. Aborting.");
    return StatusCode::FAILURE;
  }

  // otherwise, we're fine, let's just set up the messaging correctly
  m_messagePrinter.reset(new asg::MessagePrinter(m_sourceWidth));
  m_messagePrinterOverlay.reset(new asg::MessagePrinterOverlay(m_messagePrinter.get()));

  return StatusCode::SUCCESS;
}

StatusCode MessagePrinterAlgo :: fileExecute () { return StatusCode::SUCCESS; }
StatusCode MessagePrinterAlgo :: changeInput (bool /*firstFile*/) { return StatusCode::SUCCESS; }
StatusCode MessagePrinterAlgo :: initialize () { return StatusCode::SUCCESS; }
StatusCode MessagePrinterAlgo :: execute () { return StatusCode::SUCCESS; }
StatusCode MessagePrinterAlgo :: finalize () { return StatusCode::SUCCESS; }

StatusCode MessagePrinterAlgo :: histFinalize ()
{
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return StatusCode::SUCCESS;
}
