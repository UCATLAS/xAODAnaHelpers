#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <xAODAnaHelpers/Writer.h>
#include <xAODAnaHelpers/util.h>

#include "EventLoop/OutputStream.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(Writer)



Writer :: Writer () {
}

Writer :: Writer (std::string name, std::string configName) :
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

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  TEnv* config = new TEnv(m_configName.c_str());
  if( !config ) {
    Error("Writer::setupJob()", "Failed to read config file!");
    Error("Writer::setupJob()", "config name : %s",m_configName.c_str());
    return EL::StatusCode::FAILURE;
  }

  m_outputLabel               = config->GetValue("OutputLabel"            , "");

  m_jetContainerNamesStr      = config->GetValue("JetContainerNames"      , "");
  m_electronContainerNamesStr = config->GetValue("ElectronContainerNames" , "");
  m_muonContainerNamesStr     = config->GetValue("MuonContainerNames"     , "");
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
  if(!m_event->writeTo(file).isSuccess()){
    Error("initialize()", "Failed to write event to ouput file!");
    return EL::StatusCode::FAILURE;
  }

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

    const xAOD::JetContainer* inJetsConst = 0;
    // look in event
    if ( m_event->retrieve( inJetsConst , contName.Data() ).isSuccess() ){
      // without modifying the contents of it:
      std::cout << " Write a collection " << contName << " " << inJetsConst->size() << std::endl;
      m_event->copy( contName.Data() );
      std::cout << " Wrote a collection " << contName << std::endl;
      continue;
    }

    // look in store
    xAOD::JetContainer* inJets = 0;
    if ( m_store->retrieve( inJets , contName.Data() ).isSuccess() ){

//      // FIXME add something like this
//      jets_shallowCopy.second->setShallowIO( false ); // true = shallow copy, false = deep copy
//      // if true should have something like this line somewhere:

      // Record the objects into the output xAOD:
      std::cout << " Write a collection " << contName << " " << inJets->size() << std::endl;
      if( ! m_event->record( inJets, contName.Data() ) ) {
        Error(m_name.c_str() ,"Could not record %s", contName.Data());
        return EL::StatusCode::FAILURE;
      }
      std::cout << " Wrote a collection " << contName << std::endl;

      // get pointer to associated aux container
      xAOD::JetAuxContainer* inJetsAux = 0;
      std::cout << " Wrote a aux store " << contName << std::endl;
      TString auxName( contName + "Aux." );
      if ( m_store->retrieve( inJetsAux , auxName.Data() ).isSuccess() ){
        Error(m_name.c_str() ,"Could not get Aux data for %s", contName.Data());
        return EL::StatusCode::FAILURE;
      }
      std::cout << " Wrote a aux store " << contName << std::endl;

      if( ! m_event->record( inJetsAux, auxName.Data() ) ) {
        Error(m_name.c_str() ,"Could not record aux store for %s", contName.Data());
        return EL::StatusCode::FAILURE;
      }
    }
    // could not find the container - problems
    else {
      Error(m_name.c_str() ,"Could not find %s", contName.Data());
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
  if( ! m_event->finishWritingTo( file ).isSuccess() ) {
    Error(m_name.c_str(),"Failed to finish writing event to output file!");
    return EL::StatusCode::FAILURE;
  }

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
