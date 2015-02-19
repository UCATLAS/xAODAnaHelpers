#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "xAODAnaHelpers/BasicEventSelection.h"
#include "xAODAnaHelpers/JetCalibrator.h"
#include "xAODAnaHelpers/JetSelector.h"
#include "xAODAnaHelpers/BJetSelector.h"
#include "xAODAnaHelpers/JetHistsAlgo.h"
#include "xAODAnaHelpers/MuonCalibrator.h"
#include "xAODAnaHelpers/MuonSelector.h"
#include "xAODAnaHelpers/ElectronCalibrator.h"
#include "xAODAnaHelpers/ElectronSelector.h"
#include "xAODAnaHelpers/Writer.h"
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
  SH::scanDir (sh, list, "r20test_AOD.pool.root", "xAODAnaHelpers"); // specifying one particular sample

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  BasicEventSelection* baseEventSel = new BasicEventSelection("baseEventSel","$ROOTCOREBIN/data/xAODAnaHelpers/baseEvent.config");

  // Add our analysis to the job:
  JetHistsAlgo* jk_AntiKt10LC = new JetHistsAlgo("AntiKt10/", "$ROOTCOREBIN/data/xAODAnaHelpers/test_jetPlotExample.config");



  /// RECO JETS - calibrate, select, then plot

  JetCalibrator* jetCalib = new JetCalibrator("jetCalib_AntiKt4TopoEM","$ROOTCOREBIN/data/xAODAnaHelpers/jetCalib_AntiKt4TopoEMCalib.config");

  JetSelector* jetSelect_signal = new JetSelector("jetSelect_signal","$ROOTCOREBIN/data/xAODAnaHelpers/jetSelect_signal.config");

  JetHistsAlgo* jetHistsAlgo_signal = new JetHistsAlgo("jetHistsAlgo_signal","$ROOTCOREBIN/data/xAODAnaHelpers/jetHistsAlgo_signal.config");

  /// TRUTH JETS - select, then plot
  JetSelector* jetSelect_truth = new JetSelector("jetSelect_truth","$ROOTCOREBIN/data/xAODAnaHelpers/jetSelect_truth.config");

  JetHistsAlgo* jetHistsAlgo_truth = new JetHistsAlgo("jetHistsAlgo_truth","$ROOTCOREBIN/data/xAODAnaHelpers/jetHistsAlgo_truth.config");

  // Attach algorithms
  job.algsAdd( baseEventSel ); 
  job.algsAdd( jetCalib );
  job.algsAdd( jetSelect_signal );
  job.algsAdd( jetHistsAlgo_signal );
  job.algsAdd( jetSelect_truth );
  job.algsAdd( jetHistsAlgo_truth );
  job.algsAdd( jk_AntiKt10LC );


  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}


