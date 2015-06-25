#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <xAODAnaHelpers/Writer.h>
#include <xAODAnaHelpers/util.h>

#include "EventLoop/OutputStream.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(Writer)



Writer :: Writer () {
  m_outputLabel               = "";

  m_jetContainerNamesStr      = "";
  m_electronContainerNamesStr = "";
  m_muonContainerNamesStr     = "";
  m_debug                   = false;

}

EL::StatusCode Writer :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
  // let's initialize the algorithm to use the xAODRootAccess package
  job.useXAOD ();
  xAOD::Init( "Writer" ).ignore(); // call before opening first file

  if(!getConfig().empty()){
    TEnv* config = new TEnv(getConfig(true).c_str());
    if( !config ) {
      Error("Writer::setupJob()", "Failed to read config file!");
      Error("Writer::setupJob()", "config name : %s",getConfig().c_str());
      return EL::StatusCode::FAILURE;
    }

    m_outputLabel               = config->GetValue("OutputLabel"            , m_outputLabel);

    m_jetContainerNamesStr      = config->GetValue("JetContainerNames"      , m_jetContainerNamesStr);
    m_electronContainerNamesStr = config->GetValue("ElectronContainerNames" , m_electronContainerNamesStr);
    m_muonContainerNamesStr     = config->GetValue("MuonContainerNames"     , m_muonContainerNamesStr);
    m_debug                   = config->GetValue("Debug",                m_debug);

    delete config;
  }


  m_jetContainerNames       = SplitString( m_jetContainerNamesStr,      ',' );
  m_electronContainerNames  = SplitString( m_electronContainerNamesStr, ',' );
  m_muonContainerNames      = SplitString( m_muonContainerNamesStr,     ',' );

  if ( m_outputLabel.Length() == 0 ) {
    Error("Writer::setupJob()", "No OutputLabel specified!");
    return EL::StatusCode::FAILURE;
  }

  // tell EventLoop about our output xAOD:
  EL::OutputStream out(m_outputLabel.Data());
  job.outputAdd (out);

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: initialize ()
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
  m_numEvent      = 0;

  // output xAOD
  TFile * file = wk()->getOutputFile (m_outputLabel.Data());
  RETURN_CHECK( "Writer::initialize()", m_event->writeTo(file), "Failed to write event to output file");

  //FIXME add this as well
// Set which variables not to write out:
// event->setAuxItemList( "AntiKt4LCTopoJetsAux.", "-NumTrkPt1000.-NumTrkPt500" );
// // Set which variable to do write out:
// event->setAuxItemList( "GoodJetsAux.", "JetGhostArea.TrackCount" );


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.
  m_numEvent++;

  // try to find the containers in m_event - if there then copy entire container directly
  // if not found in m_event, look in m_store - user created - write aux store as well
  for( auto contName : m_jetContainerNames ) {

    const xAOD::JetContainer* inJetsConst(nullptr);
    // look in event
    if ( HelperFunctions::retrieve(inJetsConst, contName.Data(), m_event, 0, m_verbose).isSuccess() ) {
      // without modifying the contents of it:
      Info("execute()", " Write a collection %s %lu", contName.Data(), inJetsConst->size() );
      m_event->copy( contName.Data() );
      Info("execute()", " Wrote a collection %s", contName.Data());
      continue;
    }

    // look in store
    xAOD::JetContainer* inJets(nullptr);
    if ( HelperFunctions::retrieve(inJets, contName.Data(), 0, m_store, m_verbose).isSuccess() ){
//      // FIXME add something like this
//      jets_shallowCopy.second->setShallowIO( false ); // true = shallow copy, false = deep copy
//      // if true should have something like this line somewhere:

      // Record the objects into the output xAOD:
      Info("execute()", " Write a collection %s %lu", contName.Data(), inJets->size() );
      if( ! m_event->record( inJets, contName.Data() ) ) {
        Error("execute()" ,"%s: Could not record %s", m_name.c_str(), contName.Data());
        return EL::StatusCode::FAILURE;
      }
      Info("execute()", " Wrote a collection %s", contName.Data());

      // get pointer to associated aux container
      xAOD::JetAuxContainer* inJetsAux = 0;
      Info("execute()", " Wrote a aux store %s", contName.Data());
      TString auxName( contName + "Aux." );
      if ( HelperFunctions::retrieve(inJetsAux, auxName.Data(), 0, m_store, m_verbose).isSuccess() ){
        Error("execute()" ,"%s: Could not get Aux data for %s", m_name.c_str(), contName.Data());
        return EL::StatusCode::FAILURE;
      }
      Info("execute()", " Wrote a aux store %s", contName.Data());

      if( ! m_event->record( inJetsAux, auxName.Data() ) ) {
        Error("execute()", "%s: Could not record aux store for %s", m_name.c_str(), contName.Data());
        return EL::StatusCode::FAILURE;
      }
    }
    // could not find the container - problems
    else {
      Error("execute()" ,"%s: Could not find %s", m_name.c_str(), contName.Data());
      return EL::StatusCode::FAILURE;
    }
  }

  // add similar loop for electron and muons

  m_event->fill();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: finalize ()
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

  // finalize and close our output xAOD file ( and write MetaData tree )
  TFile * file = wk()->getOutputFile(m_outputLabel.Data());
  RETURN_CHECK( "Writer::finalize()", m_event->finishWritingTo( file ), "Failed to finish writing event to output file");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Writer :: histFinalize ()
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
