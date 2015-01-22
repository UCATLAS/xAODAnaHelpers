#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include <xAODAnaHelpers/JetHistsAlgo.h>

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  // get the data path for xAODAnaHelpers/data
  std::string dataPath = gSystem->ExpandPathName("$ROOTCOREBIN/data");
  SH::DiskListLocal list (dataPath);
  // SH::scanDir (sh, list, "r20test_AOD.pool.root", "xAODAnaHelpers"); // specifying one particular sample
  SH::scanDir (sh, list, "r20test_AOD.pool.root"); // specifying one particular sample

   

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  // Add our analysis to the job:
  JetHistsAlgo* jk_AntiKt10LC = new JetHistsAlgo("AntiKt10/", "$ROOTCOREBIN/data/xAODAnaHelpers/test_jetPlotExample.config");

  // Attach algorithms
  job.algsAdd( jk_AntiKt10LC );


  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}

