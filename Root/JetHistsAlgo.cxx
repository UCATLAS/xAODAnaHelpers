#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODJet/JetContainer.h>

#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/JetHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>

// this is needed to distribute the algorithm to the workers
ClassImp(JetHistsAlgo)

JetHistsAlgo :: JetHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator) :
IParticleHistsAlgo(name, pSvcLocator, "JetHistsAlgo")
{ }


StatusCode JetHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<JetHists>(name);
}

StatusCode JetHistsAlgo :: execute ()
{
  return IParticleHistsAlgo::execute<JetHists, xAOD::JetContainer>();
}
