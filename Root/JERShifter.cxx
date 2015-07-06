/******************************************
 *
 * Interface to CP JER Shifter tool(s).
 *
 * G. Facini, M. Milesi (marco.milesi@cern.ch), J. Dandoy
 *
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
#include "xAODJet/JetContainer.h"
#include "xAODJet/Jet.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/JERShifter.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JERShifter)


JERShifter :: JERShifter ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  // input container to be read from TEvent or TStore
  m_inContainerName = "";

  // output container to be put into TStore
  m_outContainerName  	= "";

  m_jetAlgo             = "";
  m_JERConfig           = "JetResolution/Prerec2015_xCalib_2012JER_ReducedTo9NP_Plots.root";
  m_JERFullSys          = false;
  m_JERApplyNominal     = true;

}


EL::StatusCode JERShifter :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
  job.useXAOD ();
  xAOD::Init( "JERShifter" ).ignore(); // call before opening first file

  if(!getConfig().empty()){
    TEnv* config = new TEnv(getConfig(true).c_str());

    // input container to be read from TEvent or TStore
    m_inContainerName = config->GetValue("InputContainer",  m_inContainerName.c_str());

    // output container to be put into TStore
    m_outContainerName  	= config->GetValue("OutputContainer", m_outContainerName.c_str());

    m_JERConfig           = config->GetValue("JERConfig",       m_JETConfig.c_str());
    m_jetAlgo             = config->GetValue("JetAlgorithm",    m_jetAlgo.c_str());
    m_JERFullSys                = config->GetValue("JERFullSys" ,           m_JERFullSys);
    m_JERApplyNominal                = config->GetValue("JERApplyNominal" ,           m_JERApplyNominal);
    m_debug               = config->GetValue("Debug" ,          m_debug);

    delete config;
  }

  m_outAuxContainerName = m_outContainerName + "Aux."; // the period is very important!

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JERShifter :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JERShifter :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JERShifter :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JERShifter :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  //if(m_isMC){
  //  m_isFullSim = true;
  //  const std::string stringMeta = wk()->metaData()->castString("SimulationFlavour"); // NB: needs to be defined as sample metadata in job steering macro. Should be either "AFII" or "FullSim"
  //  if ( stringMeta.empty() ) {
  //    Warning("initialize()", "Could not access simulation flavour from EL::Worker. Treating MC as FullSim by default!" );
  //  } else {
  //    m_isFullSim = (stringMeta == "AFII") ? false : true;
  //  }

  //}

  Info("initialize()"," Number of events in file: %lld ", m_event->getEntries() );

  m_numEvent      = 0;

  // Instantiate the tools
  std::string JERTool_name  = std::string("JERTool_") + m_name;
  std::string JERSmearingTool_name = std::string("JERSmearingTool_") + m_name;
  m_JERTool     = new JERTool( JERTool_name.c_str() );
  m_JERSmearTool = new JERSmearingTool( JERSmearingTool_name.c_str() );

  // Configure the JERTool
  //m_JERTool->msg().setLevel(MSG::DEBUG);
  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->setProperty("PlotFileName", m_JERConfig.c_str()), "");
  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->setProperty("CollectionName", m_jetAlgo), "");

  //m_JERSmearTool->msg().setLevel(MSG::DEBUG);
  ToolHandle<IJERTool> JERToolHandle(m_JERTool->name());
  RETURN_CHECK( "JERShifter::initialize()", m_JERSmearTool->setProperty("JERTool", JERToolHandle), "");

  RETURN_CHECK( "JERShifter::initialize()", m_JERSmearTool->setProperty("isMC", m_isMC), "");

  RETURN_CHECK( "JERShifter::initialize()", m_JERSmearTool->setProperty("ApplyNominalSmearing", m_JERApplyNominal), "");

  if( m_JERFullSys )
    RETURN_CHECK( "JERShifter::initialize()", m_JERSmearTool->setProperty("SystematicMode", "Full"), "");
  else
    RETURN_CHECK( "JERShifter::initialize()", m_JERSmearTool->setProperty("SystematicMode", "Simple"), "");


  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->initialize(), "Failed to properly initialize the JER Tool");
  RETURN_CHECK( "JERShifter::initialize()", m_JERSmearTool->initialize(), "Failed to properly initialize the JERSmearTool Tool");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JERShifter :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::JetContainer* inJets(nullptr);
  RETURN_CHECK("JERShifter::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, m_verbose) ,"");

  // create shallow copy
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > smearedJets = xAOD::shallowCopyContainer( *inJets );

  // calibrate and decorate with cleaning decision
  xAOD::JetContainer::iterator jet_itr = (smearedJets.first)->begin();
  xAOD::JetContainer::iterator jet_end = (smearedJets.first)->end();
  for( ; jet_itr != jet_end; ++jet_itr ){

    if ( m_JERSmearTool->applyCorrection( **jet_itr ) == CP::CorrectionCode::Error ) {
      Error("JERSmearToolTool()", "JERSmearToolTool tool reported a CP::CorrectionCode::Error");
      Error("JERSmearToolTool()", "%s", m_name.c_str());
      return StatusCode::FAILURE;
    }

  }

  // add shallow copy to TStore
  RETURN_CHECK( "JERShifter::execute()", m_store->record( smearedJets.first, m_outContainerName ), "Failed to store container");
  RETURN_CHECK( "JERShifter::execute()", m_store->record( smearedJets.second, m_outAuxContainerName ), "Failed to store aux container");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JERShifter :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JERShifter :: finalize ()
{
  if(m_JERTool) delete m_JERTool;
  if(m_JERSmearTool) delete m_JERSmearTool;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JERShifter :: histFinalize ()
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
  return EL::StatusCode::SUCCESS;
}
