/******************************************
 *
 * Interface to CP Electron calibration tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 * Jan 28 15:29 AEST 2015
 *
 ******************************************/

// c++ include(s):
#include <iostream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/ElectronCalibrator.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):
#include "ElectronPhotonFourMomentumCorrection/EgammaCalibrationAndSmearingTool.h"
#include "ElectronEfficiencyCorrection/AsgElectronEfficiencyCorrectionTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronCalibrator)


ElectronCalibrator :: ElectronCalibrator () {
}

ElectronCalibrator :: ElectronCalibrator (std::string name, std::string configName ) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_EgammaCalibrationAndSmearingTool(0),
  m_asgElectronEfficiencyCorrectionTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronCalibrator()", "Calling constructor \n");

}


EL::StatusCode  ElectronCalibrator :: configure ()
{
  Info("configure()", "Configuing ElectronCalibrator Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "ElectronCalibrator::configure()", m_configName);

  TEnv* config = new TEnv(m_configName.c_str());

  // read debug flag from .config file
  m_debug                   = config->GetValue("Debug" , false );
  // input container to be read from TEvent or TStore
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_outContainerName        = config->GetValue("OutputContainer", "");
  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  // Systematics stuff
  m_doSyst                  = config->GetValue("DoSyst" , false );
  m_runSingleSyst           = config->GetValue("RunSingleSyst" , true );
  m_systName		    = config->GetValue("SystName" , "" );
  m_systSigma 		    = config->GetValue("SystSigma" , 0. );
  // file(s) containing corrections
  m_corrFileName1           = config->GetValue("CorrectionFileName1" , "" );
  //m_corrFileName2         = config->GetValue("CorrectionFileName2" , "" );

  m_sort                    = config->GetValue("Sort",          false);

  if( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "ElectronCalibrator Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronCalibrator :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob \n");

  job.useXAOD ();
  xAOD::Init( "ElectronCalibrator" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronCalibrator :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronCalibrator :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronCalibrator :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronCalibrator :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing ElectronCalibrator Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the Egamma calibration and smearing tool
  m_EgammaCalibrationAndSmearingTool = new CP::EgammaCalibrationAndSmearingTool( "EgammaCalibrationAndSmearingTool" );
  m_EgammaCalibrationAndSmearingTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", "es2012c"),"Failed to set property ESModel");
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->setProperty("ResolutionType", "SigmaEff90"),"Failed to set property ResolutionType");
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->initialize(), "Failed to properly initialize the EgammaCalibrationAndSmearingTool");

  // initialize the ElectronEfficiencyCorrectionTool
  m_asgElectronEfficiencyCorrectionTool = new AsgElectronEfficiencyCorrectionTool("ElectronEfficiencyCorrectionTool");
  m_asgElectronEfficiencyCorrectionTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  std::vector<std::string> inputFiles{ m_corrFileName1 } ; // pass all the files with the corrections
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_asgElectronEfficiencyCorrectionTool->setProperty("CorrectionFileNameList",inputFiles),"Failed to set property CorrectionFileNameList");
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_asgElectronEfficiencyCorrectionTool->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
  //RETURN_CHECK( "ElectronCalibrator::initialize()", m_asgElectronEfficiencyCorrectionTool->setProperty("ResultPrefix", <your prefix>),"Failed to set property ResultPrefix");
  //RETURN_CHECK( "ElectronCalibrator::initialize()", m_asgElectronEfficiencyCorrectionTool->setProperty("ResultName", <your name>),"Failed to set property ResultName");
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_asgElectronEfficiencyCorrectionTool->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool");


  if(m_doSyst){

    // Get a list of systematics
    CP::SystematicSet recSysts = m_asgElectronEfficiencyCorrectionTool->recommendedSystematics();
    // Convert into a simple list
    m_systList = CP::make_systematics_vector(recSysts);

    // if we are applying one systematic at a time...
    if(m_runSingleSyst){
      Info("initialize()", "Checking if single systematic %s is available", m_systName.c_str());
      // check if there's a match with the syst passed in constructor
      bool found_match(false);
      unsigned int idx(0), syst_idx(-1);
      for ( const auto& syst_it : m_systList ){
    	Info("initialize()"," systematic: %s", (syst_it.name()).c_str());
    	if( syst_it.name() == m_systName ){
  	  Info("initialize()","  is available!");
  	  syst_idx = idx;
  	  found_match = true;
    	}
	idx++;
      }
      if( !m_systList.empty() && found_match) {
    	// setup uncertainity tool for systematic evaluation
    	if (m_asgElectronEfficiencyCorrectionTool->applySystematicVariation(m_systList.at(syst_idx)) != CP::SystematicCode::Ok) {
    	  Error("initialize()", "Cannot configure AsgElectronEfficiencyCorrectionTool for systematic %s", m_systName.c_str());
    	  return EL::StatusCode::FAILURE;
    	}
    	Info("initialize()", "Successfully applied systematic: %s", (( m_asgElectronEfficiencyCorrectionTool->appliedSystematics() ).name()).c_str());
      }
    } else {
      Info("initialize()", "Running on all available systematics:");
      for ( const auto& syst_it : m_systList ){ Info("initialize()","  %s", (syst_it.name()).c_str()); }
    }

  } // end check m_doSyst

  Info("initialize()", "ElectronCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Electron Calibration and Cleaning... \n");

  m_numEvent++;

  const xAOD::EventInfo* eventInfo = HelperFunctions::getContainer<xAOD::EventInfo>("EventInfo", m_event, m_store);

  // get the collection from TEvent or TStore
  const xAOD::ElectronContainer* inElectrons = HelperFunctions::getContainer<xAOD::ElectronContainer>(m_inContainerName, m_event, m_store);

  // create shallow copy
  std::pair< xAOD::ElectronContainer*, xAOD::ShallowAuxContainer* > calibElectronsSC = xAOD::shallowCopyContainer( *inElectrons );
  ConstDataVector<xAOD::ElectronContainer>* calibElectronsCDV = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
  calibElectronsCDV->reserve( calibElectronsSC.first->size() );

  // if not running systematics (or running on one sys only), just calibrate
  if( !m_doSyst || m_runSingleSyst){
    this->calibrate( calibElectronsSC.first );
  } else {
    // loop over available systematics
    for(const auto& syst_it : m_systList){
      if (m_asgElectronEfficiencyCorrectionTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok) {
        Error("initialize()", "Failed to configure AsgElectronEfficiencyCorrectionTool for systematic %s", syst_it.name().c_str());
        return EL::StatusCode::FAILURE;
      }
      if (m_debug) Info("execute()", "Successfully applied systematic: %s", m_asgElectronEfficiencyCorrectionTool->appliedSystematics().name().c_str());
      // and now calibrate!
      this->calibrate( calibElectronsSC.first );
    }
  }

  if(!xAOD::setOriginalObjectLink(*inElectrons, *(calibElectronsSC.first))) {
    Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
  }

  if(m_sort) {
    std::sort( calibElectronsSC.first->begin(), calibElectronsSC.first->end(), HelperFunctions::sort_pt );
  }

  // save pointers in ConstDataVector with same order
  /*
  for( auto elSC_itr : *(calibElectronsSC.first) ) {
    calibElectronsCDV->push_back( elSC_itr );
  }
  */
  RETURN_CHECK( "ElectronCalibrator::execute()", HelperFunctions::makeSubsetCont(calibElectronsSC.first, calibElectronsCDV, "", ToolName::CALIBRATOR), "");

  // add shallow copy to TStore
  RETURN_CHECK( "ElectronCalibrator::execute()", m_store->record( calibElectronsSC.first, m_outSCContainerName ), "Failed to store container.");
  RETURN_CHECK( "ElectronCalibrator::execute()", m_store->record( calibElectronsSC.second, m_outSCAuxContainerName ), "Failed to store aux container.");
  // add ConstDataVector to TStore
  RETURN_CHECK( "ElectronCalibrator::execute()", m_store->record( calibElectronsCDV, m_outContainerName ), "Failed to store const data container.");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronCalibrator :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  Info("finalize()", "Deleting tool instances... \n");

  if(m_EgammaCalibrationAndSmearingTool){
    delete m_EgammaCalibrationAndSmearingTool; m_EgammaCalibrationAndSmearingTool = 0;
  }
  if(m_asgElectronEfficiencyCorrectionTool){
    delete m_asgElectronEfficiencyCorrectionTool; m_asgElectronEfficiencyCorrectionTool = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronCalibrator :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.

  Info("histFinalize()", "Calling histFinalize \n");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronCalibrator :: calibrate (xAOD::ElectronContainer* electrons)
{
  // calibrate and apply SF
  unsigned int idx(0);
  double SF(0);
  for( auto el_itr : *electrons ) {
    // set smearing seeding if needed (already done by default - check TWiki: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ElectronPhotonFourMomentumCorrection )
    //int idx = std::distance( calibElectronsSC.first.begin(), &el_itr );
    // m_EgammaCalibrationAndSmearingTool->setRandomSeed(eventInfo->eventNumber()+100*idx);

    if(m_debug) Info( "execute", "Checking electron %i, raw pt = %.2f GeV, eta = %.2f ", idx, (el_itr->pt() * 1e-3), el_itr->caloCluster()->eta());

    // apply calibration
    if(m_EgammaCalibrationAndSmearingTool->applyCorrection( *el_itr ) != CP::CorrectionCode::Ok){
      Error("execute()", "Problem in m_EgammaCalibrationAndSmearingTool->applyCorrection()");
      return EL::StatusCode::FAILURE;
    }
    if(m_debug) Info("execute()", "  corrected Electron pt = %.2f GeV", (el_itr->pt() * 1e-3));

    // apply SF
    if(el_itr->pt() < 7e3 || fabs( el_itr->caloCluster()->eta() ) > 2.47) {
      if(m_debug) Info( "execute", "Apply SF: Skipping electron %i, is outside acceptance", idx);
      continue; //skip electrons outside of recommendations
    }
    if(m_asgElectronEfficiencyCorrectionTool->getEfficiencyScaleFactor( *el_itr, SF ) != CP::CorrectionCode::Ok){
      Error( "execute()", "Problem in getEfficiencyScaleFactor");
      return EL::StatusCode::FAILURE;
    }

    if(m_asgElectronEfficiencyCorrectionTool->applyEfficiencyScaleFactor( *el_itr ) != CP::CorrectionCode::Ok){
      Error( "execute()", "Problem in applyEfficiencyScaleFactor");
      return EL::StatusCode::FAILURE;
    }

    if(m_debug) Info( "execute", "===>>> Resulting SF (from get function) %f, (from apply function) %f", SF, el_itr->auxdata< float >("SF"));

    ++idx;

  }
  return EL::StatusCode::SUCCESS;
}
