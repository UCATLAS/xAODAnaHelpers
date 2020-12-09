
#include <xAODEgamma/ElectronContainer.h>

#include <xAODAnaHelpers/ElectronHistsAlgo.h>
#include <xAODAnaHelpers/ElectronHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>

// this is needed to distribute the algorithm to the workers

ElectronHistsAlgo :: ElectronHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator) :
IParticleHistsAlgo(name, pSvcLocator, "ElectronHistsAlgo")
{ }


StatusCode ElectronHistsAlgo::AddHists( std::string name ) {
  return IParticleHistsAlgo::AddHists<ElectronHists>(name);
}

StatusCode ElectronHistsAlgo :: execute () {
  return IParticleHistsAlgo::execute<ElectronHists, xAOD::ElectronContainer>();
}
