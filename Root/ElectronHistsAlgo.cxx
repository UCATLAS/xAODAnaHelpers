#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODEgamma/ElectronContainer.h>

#include <xAODAnaHelpers/ElectronHistsAlgo.h>
#include <xAODAnaHelpers/ElectronHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <AsgTools/MessageCheck.hh>

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronHistsAlgo)

ElectronHistsAlgo :: ElectronHistsAlgo () :
IParticleHistsAlgo("ElectronHistsAlgo")
{ }

EL::StatusCode ElectronHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("ElectronHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<ElectronHists>(name);
}

EL::StatusCode ElectronHistsAlgo :: execute () {
  return IParticleHistsAlgo::execute<ElectronHists, xAOD::ElectronContainer>();
}
