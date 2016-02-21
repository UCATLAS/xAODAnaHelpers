#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODJet/JetContainer.h>

#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/JetHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(JetHistsAlgo)

JetHistsAlgo :: JetHistsAlgo (std::string className) :
IParticleHistsAlgo(className)
{ }

EL::StatusCode JetHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("JetHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<JetHists>(name);
}

EL::StatusCode JetHistsAlgo :: execute ()
{
  return IParticleHistsAlgo::execute<JetHists, xAOD::JetContainer>();
}
