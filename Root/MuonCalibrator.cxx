/********************************************
 *
 * Interface to CP Muon calibration tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 ********************************************/

// c++ include(s):
#include <iostream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/MuonCalibrator.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"
#include "PATInterfaces/CorrectionCode.h" // to check the return correction code status of tools

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(MuonCalibrator)

MuonCalibrator :: MuonCalibrator (std::string className) :
    Algorithm(className),
    m_muonCalibrationAndSmearingTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonCalibrator()", "Calling constructor");

  m_debug                   = false;

  // input container to be read from TEvent or TStore
  //
  m_inContainerName         = "";
  m_outContainerName        = "";

  m_release                 = "PreRecs";

  m_sort                    = true;

  // Systematics stuff
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "MuonCalibrator_Syst";
  m_systName		    = "";
  m_systVal 		    = 0.;

}

EL::StatusCode MuonCalibrator :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "MuonCalibrator" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing MuonCalibrator Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  if ( m_inContainerName.empty() ) {
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  // Check if is MC
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonCalibrator::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_debug) ,"");
  m_isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );

  m_numEvent      = 0;
  m_numObject     = 0;

  // Initialize the CP::MuonCalibrationAndSmearingTool
  //
  if ( asg::ToolStore::contains<CP::MuonCalibrationAndSmearingTool>("MuonCalibrationAndSmearingTool") ) {
    m_muonCalibrationAndSmearingTool = asg::ToolStore::get<CP::MuonCalibrationAndSmearingTool>("MuonCalibrationAndSmearingTool");
  } else {
    m_muonCalibrationAndSmearingTool = new CP::MuonCalibrationAndSmearingTool("MuonCalibrationAndSmearingTool");
  }
  m_muonCalibrationAndSmearingTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  RETURN_CHECK("MuonCalibrator::initialize()", m_muonCalibrationAndSmearingTool->setProperty("Release", m_release),"Failed to set property Release");
  RETURN_CHECK("MuonCalibrator::initialize()", m_muonCalibrationAndSmearingTool->initialize(), "Failed to properly initialize the MuonCalibrationAndSmearingTool.");

  // ***********************************************************

  // Get a list of recommended systematics for this tool
  //
  const CP::SystematicSet recSyst = CP::SystematicSet();

  Info("initialize()"," Initializing Muon Calibrator Systematics :");
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, m_debug );

  Info("initialize()","Will be using MuonCalibrationAndSmearingTool systematic:");
  for ( const auto& syst_it : m_systList ) {
    if ( m_systName.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  Info("initialize()", "MuonCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Muon Calibration And Smearing ... "); }

  m_numEvent++;

  if ( !m_isMC ) {
    if ( m_numEvent == 1 ) { Info("execute()", "Sample is Data! Do not apply any Muon Calibration... "); }
  }

  // get the collections from TEvent or TStore
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  const xAOD::MuonContainer* inMuons(nullptr);
  RETURN_CHECK("MuonCalibrator::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName, m_event, m_store, m_verbose) ,"");

  // loop over available systematics - remember syst == EMPTY_STRING --> baseline
  // prepare a vector of the names of CDV containers
  // must be a pointer to be recorded in TStore
  //
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

  for ( const auto& syst_it : m_systList ) {

    std::string outSCContainerName(m_outSCContainerName);
    std::string outSCAuxContainerName(m_outSCAuxContainerName);
    std::string outContainerName(m_outContainerName);

    // always append the name of the variation, including nominal which is an empty string
    //
    outSCContainerName    += syst_it.name();
    outSCAuxContainerName += syst_it.name();
    outContainerName      += syst_it.name();
    vecOutContainerNames->push_back( syst_it.name() );

    // apply syst
    //
    if ( m_muonCalibrationAndSmearingTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("execute()", "Failed to configure MuonCalibrationAndSmearingTool for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }

    // create shallow copy for calibration - one per syst
    //
    std::pair< xAOD::MuonContainer*, xAOD::ShallowAuxContainer* > calibMuonsSC = xAOD::shallowCopyContainer( *inMuons );
    // create ConstDataVector to be eventually stored in TStore
    //
    ConstDataVector<xAOD::MuonContainer>* calibMuonsCDV = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
    calibMuonsCDV->reserve( calibMuonsSC.first->size() );

    // now calibrate!
    //
    unsigned int idx(0);
    if ( m_isMC ) {

      for ( auto muSC_itr : *(calibMuonsSC.first) ) {

	if ( m_debug ) { Info("execute()", "  uncailbrated muon %i, pt = %.2f GeV", idx, (muSC_itr->pt() * 1e-3)); }
	if(muSC_itr-> primaryTrackParticle()){
	  if ( m_muonCalibrationAndSmearingTool->applyCorrection(*muSC_itr) == CP::CorrectionCode::Error ) {  // Can have CorrectionCode values of Ok, OutOfValidityRange, or Error. Here only checking for Error.
	    Warning("execute()", "MuonCalibrationAndSmearingTool returned Error CorrectionCode");		  // If OutOfValidityRange is returned no modification is made and the original muon values are taken.
	  }
	}

	if ( m_debug ) { Info("execute()", "  corrected muon pt = %.2f GeV", (muSC_itr->pt() * 1e-3)); }

	++idx;

      } // close calibration loop
    }

    if ( m_debug ) { Info("execute()", "setOriginalObjectLink"); }
    if ( !xAOD::setOriginalObjectLink(*inMuons, *(calibMuonsSC.first)) ) {
      Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    // save pointers in ConstDataVector with same order
    //
    if ( m_debug ) { Info("execute()", "makeSubsetCont"); }
    RETURN_CHECK( "MuonCalibrator::execute()", HelperFunctions::makeSubsetCont(calibMuonsSC.first, calibMuonsCDV), "");
    if ( m_debug ) { Info("execute()", "done makeSubsetCont"); }

    // sort after coping to CDV
    if ( m_sort ) {
      if ( m_debug ) { Info("execute()", "sorting"); }
      std::sort( calibMuonsCDV->begin(), calibMuonsCDV->end(), HelperFunctions::sort_pt );
    }

    // add SC container to TStore
    //
    if ( m_debug ) { Info("execute()", "recording calibMuonsSC"); }
    RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( calibMuonsSC.first,  outSCContainerName  ), "Failed to store container.");
    RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( calibMuonsSC.second, outSCAuxContainerName ), "Failed to store aux container.");

    //
    // add ConstDataVector to TStore
    //
    if ( m_debug ) { Info("execute()", "record calibMuonsCDV"); }
    RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( calibMuonsCDV, outContainerName), "Failed to store const data container.");

  } // close loop on systematics

  // add vector<string container_names_syst> to TStore
  //
  if ( m_debug ) { Info("execute()", "record m_outputAlgoSystNames"); }
  RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  // look what we have in TStore
  //
  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MuonCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: finalize ()
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

  Info("finalize()", "Deleting tool instances...");

  if ( m_muonCalibrationAndSmearingTool ) { m_muonCalibrationAndSmearingTool = nullptr; delete m_muonCalibrationAndSmearingTool; }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonCalibrator :: histFinalize ()
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

  Info("histFinalize()", "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
