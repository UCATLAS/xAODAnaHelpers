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
#include "xAODAnaHelpers/BJetEfficiencyCorrector.h"
#include "xAODAnaHelpers/JetHistsAlgo.h"
#include "xAODAnaHelpers/MuonCalibrator.h"
#include "xAODAnaHelpers/MuonEfficiencyCorrector.h"
#include "xAODAnaHelpers/MuonSelector.h"
#include "xAODAnaHelpers/ElectronCalibrator.h"
#include "xAODAnaHelpers/ElectronEfficiencyCorrector.h"
#include "xAODAnaHelpers/ElectronSelector.h"
#include "xAODAnaHelpers/Writer.h"
#include "xAODAnaHelpers/OverlapRemover.h"
#include "xAODAnaHelpers/METConstructor.h"
#include "xAODAnaHelpers/TreeAlgo.h"

#include "PATInterfaces/SystematicVariation.h"

#include <sstream>

// usage:
// test_multiAlgo [optional] outdir maxevts dataPath/ datasetname filename 
// NB: 
//    -) 'outdir' , 'maxevts' and 'dataPath/' must be all specified
//       --> use '-1' for maxevts if you want to run them all
//     


int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  // default
  std::string datasetname;
  std::string filename;
  std::string dataPath;

  if ( argc > 4 ) {
    dataPath = argv[3];
    datasetname = argv[4];

    SH::DiskListLocal list (dataPath);  // path to folder containing your datasets subfolders
    if( argc > 5 ){
      filename = argv[5];
      SH::scanDir (sh, list, filename); // running on a specific file
    } else {
      SH::scanDir (sh, list, "*.root*", datasetname ); // running on all files in a specific dataset
    }


  } else {
    // default
    std::string filename = "mc14_13TeV.110351.PowhegPythia_P2012_ttbar_allhad.merge.AOD.e3232_s1982_s2008_r5787_r5853_skim.root";
    // get the data path for xAODAnaHelpers/data
    std::string dataPath = gSystem->ExpandPathName("$ROOTCOREBIN/data");
    SH::DiskListLocal list (dataPath);
    SH::scanDir (sh, list, filename, "xAODAnaHelpers"); // specifying one particular sample
  }
  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );
  job.options()->setDouble(EL::Job::optRemoveSubmitDir, 1);

  // For Trigger
  job.options()->setString( EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_branch );

  // Select max number of events
  if ( argc > 2 ) {
    double maxevts;
    std::stringstream ss_maxevts; ss_maxevts << argv[2]; ss_maxevts >> maxevts;
    if ( maxevts > 0 ) { job.options()->setDouble(EL::Job::optMaxEvents, maxevts); }
  }

  std::string localDataDir = "$ROOTCOREBIN/data/xAODAnaHelpers/";

  // this class will hold the class name of the algorithms used below, and the number
  // of times an algo is used
  xAH::AlgorithmRegistry registry;

  BasicEventSelection* baseEventSel             = new BasicEventSelection();
  baseEventSel->setName("baseEventSel")->setConfig(localDataDir+"baseEvent.config");

//  JET_GroupedNP_1__continuous
//  JET_GroupedNP_2__continuous
//  JET_GroupedNP_3__continuous
//  JET_RelativeNonClosure_MC12__continuous
//  JetCalibrator* jetCalib                       = new JetCalibrator(        "jetCalib_AntiKt4TopoEM",   localDataDir+"jetCalib_AntiKt4TopoEMCalib.config", "JET_GroupedNP_1", -1);

  JetCalibrator* jetCalib                       = new JetCalibrator();
  jetCalib->setName("jetCalib_AntiKt4TopoEM")->setConfig(localDataDir+"jetCalib_AntiKt4TopoEMCalib.config")->registerClass(registry, "JetCalibrator");

  MuonCalibrator* muonCalib                     = new MuonCalibrator();
  muonCalib->setName("muonCalib")->setConfig(localDataDir+"muonCalib.config")->registerClass(registry, "MuonCalibrator");

  ElectronCalibrator* electronCalib             = new ElectronCalibrator();
  electronCalib->setName("electronCalib")->setConfig(localDataDir+"electronCalib.config")->registerClass(registry, "ElectronCalibrator");/*->setSysts("All");*/

  MuonEfficiencyCorrector*      muonEffCorr     = new MuonEfficiencyCorrector();
  muonEffCorr->setName("muonEfficiencyCorrector")->setConfig(localDataDir+"muonEffCorr.config")->registerClass(registry, "MuonEfficiencyCorrector");

  ElectronEfficiencyCorrector*  electronEffCorr = new ElectronEfficiencyCorrector();
  electronEffCorr->setName("electronEfficiencyCorrector")->setConfig(localDataDir+"electronEffCorr.config")->registerClass(registry, "ElectronEfficiencyCorrector");/*->setSysts("All");*/

  MuonSelector* muonSelect_signal               = new MuonSelector();
  muonSelect_signal->setName("muonSelect_signal")->setConfig(localDataDir+"muonSelect_signal.config")->registerClass(registry, "MuonSelector")->registerClass(registry, "MuonSelector");

  ElectronSelector* electronSelect_signal       = new ElectronSelector();
  electronSelect_signal->setName("electronSelect_signal")->setConfig(localDataDir+"electronSelect_signal.config")->registerClass(registry, "ElectronSelector");

  JetSelector* jetSelect_signal                 = new JetSelector();
  jetSelect_signal->setName("jetSelect_signal")->setConfig(localDataDir+"jetSelect_signal.config")->registerClass(registry, "JetSelector");

  JetSelector* bjetSelect_signal                = new JetSelector();
  bjetSelect_signal->setName("bjetSelect_signal")->setConfig(localDataDir+"bjetSelect_signal.config")->registerClass(registry, "JetSelector");

  BJetEfficiencyCorrector* bjetEffCorr_btag     = new BJetEfficiencyCorrector();
  bjetEffCorr_btag->setName("bjetEffCor_btag")->setConfig(localDataDir+"bjetEffCorr.config")->registerClass(registry, "BJetEfficiencyCorrector");

  JetHistsAlgo* jetHistsAlgo_signal             = new JetHistsAlgo();
  jetHistsAlgo_signal->setName("jetHistsAlgo_signal")->setConfig(localDataDir+"jetHistsAlgo_signal.config")->registerClass(registry, "JetHistsAlgo");

  JetHistsAlgo* jetHistsAlgo_btag               = new JetHistsAlgo();
  jetHistsAlgo_btag->setName("jetHistsAlgo_btag")->setConfig(localDataDir+"jetHistsAlgo_btagged.config")->registerClass(registry, "JetHistsAlgo");

  JetSelector* jetSelect_truth                  = new JetSelector();
  jetSelect_truth->setName("jetSelect_truth")->setConfig(localDataDir+"jetSelect_truth.config")->registerClass(registry, "JetSelector");

  JetHistsAlgo* jetHistsAlgo_truth              = new JetHistsAlgo();
  jetHistsAlgo_truth->setName("jetHistsAlgo_truth")->setConfig(localDataDir+"jetHistsAlgo_truth.config")->registerClass(registry, "JetHistsAlgo");

  OverlapRemover* overlapRemoval                = new OverlapRemover();
  overlapRemoval->setName("OverlapRemovalTool")->setConfig(localDataDir+"overlapRemoval.config")->registerClass(registry, "OverlapRemover");

  METConstructor* met = new METConstructor();
  met->setName("met")->setConfig(localDataDir+"MET_MC15.config")->registerClass(registry, "METConstructor");

  JetHistsAlgo* jk_AntiKt10LC                   = new JetHistsAlgo();
  jk_AntiKt10LC->setName("AntiKt10/")->setConfig(localDataDir+"test_jetPlotExample.config")->registerClass(registry, "JetHistsAlgo");

  TreeAlgo* out_tree                            = new TreeAlgo();
  out_tree->setName("physics")->setConfig(localDataDir+"tree.config")->registerClass(registry, "TreeAlgo");

  // Attach algorithms
  job.algsAdd( baseEventSel );
  job.algsAdd( jetCalib );
  job.algsAdd( muonCalib );
  job.algsAdd( muonSelect_signal );
  job.algsAdd( muonEffCorr );
  job.algsAdd( electronCalib );
  job.algsAdd( electronSelect_signal );
  job.algsAdd( electronEffCorr );
  job.algsAdd( jetSelect_signal );
  job.algsAdd( bjetSelect_signal );
  job.algsAdd( bjetEffCorr_btag );
  job.algsAdd( jetHistsAlgo_signal );
  job.algsAdd( jetHistsAlgo_btag );
  job.algsAdd( jetSelect_truth );
  job.algsAdd( jetHistsAlgo_truth );
  job.algsAdd( overlapRemoval );
  job.algsAdd( met );
  job.algsAdd( jk_AntiKt10LC );
  job.algsAdd( out_tree );

  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}


