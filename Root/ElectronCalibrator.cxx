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

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronCalibrator)


ElectronCalibrator :: ElectronCalibrator () {
}

ElectronCalibrator :: ElectronCalibrator (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_EgammaCalibrationAndSmearingTool(0)
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
  m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", "es2012c");
  m_EgammaCalibrationAndSmearingTool->setProperty("ResolutionType", "SigmaEff90");

  RETURN_CHECK( "ElectronCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->initialize(), "Failed to properly initialize the EgammaCalibrationAndSmearingTool");

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

  if(m_debug){
    for( auto Electron_itr = inElectrons->begin(); Electron_itr != inElectrons->end(); ++Electron_itr ){
      Info("execute()", "  original Electron pt = %.2f GeV", ((*Electron_itr)->pt() * 1e-3));
    }
  }

  // create shallow copy
  std::pair< xAOD::ElectronContainer*, xAOD::ShallowAuxContainer* > calibElectronsSC = xAOD::shallowCopyContainer( *inElectrons );
  ConstDataVector<xAOD::ElectronContainer>* calibElectronsCDV = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
  calibElectronsCDV->reserve( calibElectronsSC.first->size() );

  // calibrate
  for( auto elSC_itr : *(calibElectronsSC.first) ) {
     // set smearing seeding if needed (already done by default - check TWiki: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ElectronPhotonFourMomentumCorrection )
     // int i = std::distance((calibElectronsSC.first)->begin(), &elSC_itr);
     // m_EgammaCalibrationAndSmearingTool->setRandomSeed(eventInfo->eventNumber()+100*i);

     // apply correction
     m_EgammaCalibrationAndSmearingTool->applyCorrection( *elSC_itr );
     if(m_debug) Info("execute()", "  corrected Electron pt = %.2f GeV", (elSC_itr->pt() * 1e-3));
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
    delete m_EgammaCalibrationAndSmearingTool;
    m_EgammaCalibrationAndSmearingTool = 0;
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

