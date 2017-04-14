#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODEgamma/PhotonContainer.h>

#include <xAODAnaHelpers/PhotonHistsAlgo.h>
#include <xAODAnaHelpers/PhotonHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(PhotonHistsAlgo)

PhotonHistsAlgo :: PhotonHistsAlgo (std::string className) :
IParticleHistsAlgo(className)
{ }

EL::StatusCode PhotonHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("PhotonHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<PhotonHists>(name);
}

EL::StatusCode PhotonHistsAlgo :: execute () {
  return IParticleHistsAlgo::execute<PhotonHists, xAOD::PhotonContainer>();
}
