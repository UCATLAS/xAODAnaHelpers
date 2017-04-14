#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODMuon/MuonContainer.h>

#include <xAODAnaHelpers/MuonHistsAlgo.h>
#include <xAODAnaHelpers/MuonHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(MuonHistsAlgo)

MuonHistsAlgo :: MuonHistsAlgo (std::string className) :
IParticleHistsAlgo(className)
{ }

EL::StatusCode MuonHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("MuonHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<MuonHists>(name);
}

EL::StatusCode MuonHistsAlgo :: execute () {
  return IParticleHistsAlgo::execute<MuonHists, xAOD::MuonContainer>();
}
