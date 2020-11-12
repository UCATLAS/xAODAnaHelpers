#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODEgamma/PhotonContainer.h>

#include <xAODAnaHelpers/PhotonHistsAlgo.h>
#include <xAODAnaHelpers/PhotonHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>

// this is needed to distribute the algorithm to the workers
ClassImp(PhotonHistsAlgo)

PhotonHistsAlgo :: PhotonHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator) :
IParticleHistsAlgo(name, pSvcLocator, "PhotonHistsAlgo")
{ }


StatusCode PhotonHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<PhotonHists>(name);
}

StatusCode PhotonHistsAlgo :: execute () {
  return IParticleHistsAlgo::execute<PhotonHists, xAOD::PhotonContainer>();
}
