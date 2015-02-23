/******************************************
 *
 * Interface to CP JER Shifter tool(s).
 *
 * G. Facini, M. Milesi (marco.milesi@cern.ch)
 * Jan 28 15:51 AEST 2015
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

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):
#include "JetResolution/JERTool.h"
#include "JetResolution/JERSmearingTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"



// this is needed to distribute the algorithm to the workers
ClassImp(JERShifter)


JERShifter :: JERShifter () {
}

JERShifter :: JERShifter (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
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

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "JERShifter::setupJob()", m_configName);

  TEnv* config = new TEnv(m_configName.c_str());

  // input container to be read from TEvent or TStore
  m_inContainerName = config->GetValue("InputContainer",  "");

  // output container to be put into TStore
  m_outContainerName  	= config->GetValue("OutputContainer", "");
  m_outAuxContainerName = m_outContainerName + "Aux."; // the period is very important!

  m_jetAlgo             = config->GetValue("JetAlgorithm",    "");

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

  std::cout << m_name << " Number of events = " << m_event->getEntries() << std::endl;

  m_numEvent      = 0;

  // Instantiate the tools
  m_JERTool     = new JERTool("JERTool");
  m_JERSmearing = new JERSmearingTool("JERSmearingTool");

  // Configure the JERTool
  //m_JERTool->msg().setLevel(MSG::DEBUG);
  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->setProperty("PlotFileName", "JetResolution/JERProviderPlots_2012.root"), "");
  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->setProperty("CollectionName", m_jetAlgo.Data()), "");
  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->setProperty("BeamEnergy", "8TeV"), "");
  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->setProperty("SimulationType", "FullSim"), "");

  // Configure the JERSmearingTool
  //m_JERSmearing->msg().setLevel(MSG::DEBUG);
  m_JERSmearing->setJERTool(m_JERTool);
  m_JERSmearing->setNominalSmearing(true);

  RETURN_CHECK( "JERShifter::initialize()", m_JERTool->initialize(), "Failed to properly initialize the JER Tool");
  RETURN_CHECK( "JERShifter::initialize()", m_JERSmearing->initialize(), "Failed to properly initialize the JERSmearing Tool");

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
  const xAOD::JetContainer* inJets = 0;
  if ( !m_event->retrieve( inJets , m_inContainerName.Data() ).isSuccess() ){
    if ( !m_store->retrieve( inJets , m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  // create shallow copy
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > smearedJets = xAOD::shallowCopyContainer( *inJets );

  // calibrate and decorate with cleaning decision
  xAOD::JetContainer::iterator jet_itr = (smearedJets.first)->begin();
  xAOD::JetContainer::iterator jet_end = (smearedJets.first)->end();
  for( ; jet_itr != jet_end; ++jet_itr ){

    if ( m_JERSmearing->applyCorrection( **jet_itr ) == CP::CorrectionCode::Error ) {
      Error("JERSmearingTool()", "JERSmearingTool tool reported a CP::CorrectionCode::Error");
      Error("JERSmearingTool()", "%s", m_name.c_str());
      return StatusCode::FAILURE;
    }

  }

  // add shallow copy to TStore
  RETURN_CHECK( "JERShifter::execute()", m_store->record( smearedJets.first, m_outContainerName.Data() ), "Failed to store container");
  RETURN_CHECK( "JERShifter::execute()", m_store->record( smearedJets.second, m_outAuxContainerName.Data() ), "Failed to store aux container");

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
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.
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
