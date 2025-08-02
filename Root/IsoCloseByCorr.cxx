/******************************************
 *
 * Interface to Isolation Close-by-Correction tool to correct variables used in the evaluation of
 * isolation working points for the presence of leptons close-by to the object of interest.
 * More information on the twiki:
 * https://twiki.cern.ch/twiki/bin/view/AtlasProtected/IsolationOverlapRemoval
 *
 * G. Frattari (guglielmo.frattari@SPAMNOT_CERN.CH), S. Addepalli (sagara17@SPAMNOT_CERN.CH)
 *
 ******************************************/
//
//
// c++ include(s):
#include <iostream>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/Photon.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/Muon.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/IsoCloseByCorr.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(IsoCloseByCorr)


IsoCloseByCorr :: IsoCloseByCorr () :
    Algorithm("IsoCloseByCorr")
{
}

EL::StatusCode IsoCloseByCorr :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_INFO( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "IsoCloseByCorr" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode IsoCloseByCorr :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode IsoCloseByCorr :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode IsoCloseByCorr :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode IsoCloseByCorr :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_INFO( "Initializing IsoCloseByCorr Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //Add a track selection tool to superseed the default in the isolation close-by-correction tool
  ANA_MSG_INFO( " Initialising InDet::InDetTrackSelectionTool..." );
  m_trackSelectionTool_name = "TrackParticleSelectionTool";
  ANA_CHECK(checkToolStore<InDet::InDetTrackSelectionTool>(m_trackSelectionTool_name));
  const bool TrackSelToolInstanceExists = asg::ToolStore::contains<InDet::InDetTrackSelectionTool>(m_trackSelectionTool_name);
  m_trackSelection_tool = asg::AnaToolHandle<InDet::InDetTrackSelectionTool>("InDet::InDetTrackSelectionTool/"+m_trackSelectionTool_name);
  ANA_CHECK(m_trackSelection_tool.setProperty("minPt", 500.));
  ANA_CHECK(m_trackSelection_tool.setProperty("CutLevel", "Loose"));
  ANA_CHECK(m_trackSelection_tool.retrieve());
  assert(m_trackSelection_tool.isInitialized());


  //Initialize isolation tool to be input to close-by-correction tool
  m_IsoSelToolCloseBy_name = "IsoSelToolCloseBy";
  ANA_MSG_INFO( " Initialising CP::IsolationSelectionTool..." );
  ANA_CHECK(checkToolStore<CP::IsolationSelectionTool>(m_IsoSelToolCloseBy_name));
  const bool IsoSelToolInstanceExists = asg::ToolStore::contains<CP::IsolationSelectionTool>(m_IsoSelToolCloseBy_name);
  m_IsoSelCloseBy_tool = asg::AnaToolHandle<CP::IsolationSelectionTool>("CP::IsolationSelectionTool/"+m_IsoSelToolCloseBy_name);
  ANA_CHECK( m_IsoSelCloseBy_tool.setProperty("ElectronWP" , m_el_iso_WP) );
  ANA_CHECK( m_IsoSelCloseBy_tool.setProperty("MuonWP" , m_mu_iso_WP) );
  ANA_CHECK( m_IsoSelCloseBy_tool.retrieve());
  assert(m_IsoSelCloseBy_tool.isInitialized());

  //Initialize the close-by-correction tool
  m_IsoCloseByCorr_tool_name = "IsoCloseByCorrTool";
  ANA_MSG_INFO( " Initialising CP::IsolationCloseByCorrectionTool..." );
  ANA_CHECK(checkToolStore<CP::IsolationCloseByCorrectionTool>(m_IsoCloseByCorr_tool_name));
  const bool IsoCloseByCorrToolInstanceExists = asg::ToolStore::contains<CP::IsolationCloseByCorrectionTool>(m_IsoCloseByCorr_tool_name);
  m_IsoCloseByCorr_tool = asg::AnaToolHandle<CP::IsolationCloseByCorrectionTool>("CP::IsolationCloseByCorrectionTool/"+m_IsoCloseByCorr_tool_name);
  ANA_CHECK( m_IsoCloseByCorr_tool.setProperty("IsolationSelectionTool", m_IsoSelToolCloseBy_name) );
  ANA_CHECK( m_IsoCloseByCorr_tool.setProperty("TrackSelectionTool", m_trackSelectionTool_name) );
  ANA_CHECK( m_IsoCloseByCorr_tool.setProperty("BackupPrefix" , "Vanilla") );
  ANA_CHECK( m_IsoCloseByCorr_tool.setProperty("IsolationSelectionDecorator", "CleanedUpIsolation") );
  ANA_CHECK( m_IsoCloseByCorr_tool.setProperty("CaloCorrectionModel", CP::IsolationCloseByCorrectionTool::UseAveragedDecorators) );
  ANA_CHECK( m_IsoCloseByCorr_tool.retrieve());
  assert(m_IsoCloseByCorr_tool.isInitialized());

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode IsoCloseByCorr :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG("Applying IsoCloseByCorrectionTool... ");

  // get the collections from TEvent or TStore
  const xAOD::ElectronContainer* inElectrons (nullptr);
  const xAOD::MuonContainer* inMuons         (nullptr);
  const xAOD::PhotonContainer* inPhotons     (nullptr);

  ANA_CHECK( HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, m_event, m_store, msg()) );
  ANA_CHECK( HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, m_event, m_store, msg()) );
  if(m_doPhotons){
    ANA_CHECK( HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, m_event, m_store, msg()) );
  }
  
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  if(m_IsoCloseByCorr_tool->getCloseByIsoCorrection(Gaudi::Hive::currentContext(),
                                                    (xAOD::ElectronContainer*)inElectrons,
                                                    (xAOD::MuonContainer*)inMuons,
                                                    (xAOD::PhotonContainer*)inPhotons).code()  == CP::CorrectionCode::Error) {
    ANA_MSG_ERROR("CloseByIsoCorrection returns Error code");
    return StatusCode::FAILURE;
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode IsoCloseByCorr :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode IsoCloseByCorr :: finalize ()
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

  ANA_MSG_INFO( "Deleting tool instances...");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode IsoCloseByCorr :: histFinalize ()
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

  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}
