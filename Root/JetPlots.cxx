#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "xAODJet/JetContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include <xAODAnaHelpers/JetPlots.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include "TEnv.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetPlots)

JetPlots :: JetPlots () {
}

JetPlots :: JetPlots (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_plots(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
}



EL::StatusCode JetPlots :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
  job.useXAOD();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("JetPlots").ignore(); // call before opening first file

  TEnv* config = new TEnv(m_configName.c_str());
  if( !config ) {
    Error("JetPlots::setupJob()", "Failed to read config file!");
    Error("JetPlots::setupJob()", "config name : %s",m_configName.c_str());
    return EL::StatusCode::FAILURE;
  }
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_detailLevel             = config->GetValue("DetailLevel",     0 ); // 0-9

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetPlots :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  // declare class and add histograms to output
  m_plots = new JetPlotsClass(m_name,m_detailLevel);
  m_plots -> BookHistograms( wk() );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetPlots :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetPlots :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetPlots :: initialize ()
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
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetPlots :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  const xAOD::EventInfo* eventInfo = 0;
  if ( ! m_event->retrieve(eventInfo, "EventInfo").isSuccess() ) {
    Error("AnalysisLoop::execute()", "Failed to retrieve event info collection. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  const xAOD::JetContainer* jets = 0;
  if ( !m_event->retrieve( jets, m_inContainerName.Data() ).isSuccess() ){
    if ( !m_store->retrieve( jets, m_inContainerName.Data() ).isSuccess() ){
      Error("JetPlots::execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  // get the highest sum pT^2 primary vertex location in the PV vector
  const xAOD::VertexContainer* vertices = 0;
  if ( !m_event->retrieve( vertices, "PrimaryVertices").isSuccess() ){
    return EL::StatusCode::FAILURE;
  }
  int pvLocation = HelperFunctions::getPrimaryVertexLocation(vertices);

  // two ways to fill
  // 1. pass the jet collection
  // 2. jet by jet

  // 1.
  m_plots->FillHistograms( jets, eventWeight );

  // 2.
//  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
//  xAOD::JetContainer::const_iterator jet_end = jets->end();
//  for( ; jet_itr != jet_end; ++jet_itr ) {
//    m_plots->FillHistograms( *jet_itr, eventWeight );
//  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetPlots :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetPlots :: finalize ()
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



EL::StatusCode JetPlots :: histFinalize ()
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

