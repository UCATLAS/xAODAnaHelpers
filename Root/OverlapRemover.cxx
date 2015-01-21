#include <iostream>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM includes
#include "xAODCore/ShallowCopy.h"
#include "xAODAnaHelpers/OverlapRemover.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODEgamma/PhotonContainer.h"

// Local includes
#include "AssociationUtils/OverlapRemovalTool.h"

// ROOT includes
#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(OverlapRemover)


OverlapRemover :: OverlapRemover () {
}

OverlapRemover :: OverlapRemover (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_overlapRemovalTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("OverlapRemover()", "Calling constructor \n");

}

EL::StatusCode  OverlapRemover :: configure ()
{
  Info("configure()", "Configuing OverlapRemover Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  TEnv* config = new TEnv(m_configName.c_str());
  if( !config ) {
    Error("configure()", "Failed to read config file!");
    Error("configure()", "config name : %s",m_configName.c_str());
    return EL::StatusCode::FAILURE;
  }

  // read debug flag from .config file
  m_debug         = config->GetValue("Debug" , false );
  // input container(s) to be read from TEvent or TStore

  /* Muons */
  m_inContainerName_Muons         	     = config->GetValue("InputContainerMuons",  "");
  /* Electrons */
  m_inContainerName_Electrons                = config->GetValue("InputContainerElectrons",  "");
  /* Photons */
  m_inContainerName_Photons                  = config->GetValue("InputContainerPhotons",  "");
  /* Taus */
  m_inContainerName_Taus                     = config->GetValue("InputContainerTaus",  "");
  /* Jets */
  m_inContainerName_Jets                     = config->GetValue("InputContainerJets",  "");

  if( m_inContainerName_Muons.Length() == 0 ) {
    Error("configure()", "InputContainerMuons is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  if( m_inContainerName_Electrons.Length() == 0 ) {
    Error("configure()", "InputContainerElectrons is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  if( m_inContainerName_Jets.Length() == 0 ) {
    Error("configure()", "InputContainerJets is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "OverlapRemover Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: setupJob (EL::Job& job)
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
  xAOD::Init( "OverlapRemover" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing OverlapRemover Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize overlap removal tool
  m_overlapRemovalTool = new OverlapRemovalTool("OverlapRemovalTool");
  m_overlapRemovalTool->msg().setLevel( MSG::ERROR); // VERBOSE, INFO, DEBUG
  // set object decoration
  m_overlapRemovalTool->setProperty("InputLabel",   "");         // second string is 'selected' (default : use flag you already chose!) if passing only selected objects to the OR tool, otherwise passes all objects
  m_overlapRemovalTool->setProperty("OverlapLabel", "overlaps"); // decorates objects with 'overlaps' boolean if they overlap (based on harmonization task force predefined order)
  if( ! m_overlapRemovalTool->initialize().isSuccess() ) {
    Error("initialize()", "Failed to properly initialize the OverlapRemovalTool. %s. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  }

  Info("initialize()", "OverlapRemover Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Overlap Removal... \n");

  m_numEvent++;

  // get the collections from TEvent or TStore

  /*const*/ xAOD::MuonContainer* inMuons = 0;
  if ( !m_event->retrieve( inMuons , m_inContainerName_Muons.Data() ).isSuccess() ){
    if ( !m_store->retrieve( inMuons , m_inContainerName_Muons.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName_Muons.Data() );
      return EL::StatusCode::FAILURE;
    }
  }
  /*const*/ xAOD::ElectronContainer* inElectrons = 0;
  if ( !m_event->retrieve( inElectrons , m_inContainerName_Electrons.Data() ).isSuccess() ){
    if ( !m_store->retrieve( inElectrons , m_inContainerName_Electrons.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName_Electrons.Data() );
      return EL::StatusCode::FAILURE;
    }
  }
  /*const*/ xAOD::JetContainer* inJets = 0;
  if ( !m_event->retrieve( inJets , m_inContainerName_Jets.Data() ).isSuccess() ){
    if ( !m_store->retrieve( inJets , m_inContainerName_Jets.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName_Jets.Data() );
      return EL::StatusCode::FAILURE;
    }
  }
  /*const*/ xAOD::PhotonContainer* inPhotons = 0;
  // try to retrieve photon container only if passed in config file
  if( m_inContainerName_Photons.Length() != 0 ) {
    if ( !m_event->retrieve( inJets , m_inContainerName_Photons.Data() ).isSuccess() ){
      if ( !m_store->retrieve( inJets , m_inContainerName_Photons.Data() ).isSuccess() ){
        Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName_Photons.Data() );
        return EL::StatusCode::FAILURE;
      }
    }
  }
  /*const*/ xAOD::TauJetContainer* inTaus = 0;
  // try to retrieve tau container only if passed in config file
  if( m_inContainerName_Taus.Length() != 0 ) {
    if ( !m_event->retrieve( inJets , m_inContainerName_Taus.Data() ).isSuccess() ){
      if ( !m_store->retrieve( inJets , m_inContainerName_Taus.Data() ).isSuccess() ){
        Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName_Taus.Data() );
        return EL::StatusCode::FAILURE;
      }
    }
  }

  // remove overlaps
  /*
  =======================================================
  https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/AnalysisCommon/AssociationUtils/trunk/doc/README.rst
  ========================================================
  */
  m_overlapRemovalTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons);

  // debug : check that something has been done
  if(m_debug){
    auto jet_itr = inJets->begin();
    auto jet_end = inJets->end();
    std::cout << "Jets in container: " << inJets->size() << std::endl;
    for( ; jet_itr != jet_end; ++jet_itr ){
      int i = std::distance( inJets->begin(), jet_itr );
      if ( (*jet_itr)->isAvailable< bool >( "overlaps" ) ){
    	if ( (*jet_itr)->auxdata< bool >( "overlaps" ) ){
    	  std::cout <<"\t jet " << i << " overlaps with other object " << std::endl;
    	}
      }
    }
    std::cout << " " << std::endl;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: finalize ()
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

  if(m_overlapRemovalTool){
    delete m_overlapRemovalTool;
    m_overlapRemovalTool = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: histFinalize ()
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
