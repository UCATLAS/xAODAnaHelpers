#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODTau/TauJetContainer.h>

#include <xAODAnaHelpers/TauHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(TauHistsAlgo)

TauHistsAlgo :: TauHistsAlgo (std::string className) :
IParticleHistsAlgo(className)
{ }

EL::StatusCode TauHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("TauHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TauHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<IParticleHists>(name);
}

EL::StatusCode TauHistsAlgo :: execute () {
  return IParticleHistsAlgo::execute<xAOD::TauJetContainer>();
}
