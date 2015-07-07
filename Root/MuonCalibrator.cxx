/********************************************
 *
 * Interface to CP Muon calibration tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
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

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(MuonCalibrator)

MuonCalibrator :: MuonCalibrator () :
  m_muonCalibrationAndSmearingTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonCalibrator()", "Calling constructor");

  // read debug flag from .config file
  m_debug                   = false;
  // input container to be read from TEvent or TStore
  m_inContainerName         = "";
  m_outContainerName        = "";

  m_sort                    = true;

  // Systematics stuff
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "MuonCalibrator_Syst";
  m_systName		      = "";
  m_systVal 		      = 0.;

  m_sort                    = false;

}

EL::StatusCode  MuonCalibrator :: configure ()
{

  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing MuonCalibrator Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug                   = config->GetValue("Debug", m_debug);
    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());
    m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());

    m_sort                    = config->GetValue("Sort",  m_sort);

    // Systematics stuff
    m_inputAlgoSystNames      = config->GetValue("InputAlgoSystNames",  m_inputAlgoSystNames.c_str());
    m_outputAlgoSystNames     = config->GetValue("OutputAlgoSystNames", m_outputAlgoSystNames.c_str());
    m_runSysts                = false; // gets set later is syst applies to this tool
    m_systName		      = config->GetValue("SystName" , m_systName.c_str());
    m_systVal 		      = config->GetValue("SystVal" , m_systVal);
    m_runAllSyst              = (m_systName.find("All") != std::string::npos);

    m_sort                    = config->GetValue("Sort",  m_sort);

    config->Print();

    Info("configure()", "MuonCalibrator Interface succesfully configured! ");

    delete config; config = nullptr;
  }

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  if ( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  return EL::StatusCode::SUCCESS;
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

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  
  // see if is MC
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_debug) ,"");
  m_isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the muon calibration and smearing tool
  std::string mcas_tool_name = std::string("MuonCalibrationAndSmearingTool_") + m_name;
  m_muonCalibrationAndSmearingTool = new CP::MuonCalibrationAndSmearingTool( mcas_tool_name.c_str() );
  m_muonCalibrationAndSmearingTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE
  RETURN_CHECK("MuonCalibrator::initialize()", m_muonCalibrationAndSmearingTool->initialize(), "Failed to properly initialize the MuonCalibrationAndSmearingTool.");


  // ***********************************************************

  // get a list of systematics
  const CP::SystematicRegistry& systReg = CP::SystematicRegistry::getInstance();
  const CP::SystematicSet& recSyst = (systReg.recommendedSystematics());
  Info("initialize()"," Initializing Muon Calibrator Systematics :");
  m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal );

  if ( !m_systList.empty() ) { m_runSysts = true; }

  // ****************************************************************** //
  // *
  // *
  // * Marco: we need to manually add an "empty-string" syst variation (i.e., case "baseline") at top of vector
  // *        to apply same logic as in ElectronEfficiencyCorrector.
  // *        Whilst looping on available systematics, the empty string will be regarded as baseline case
  // *
  // *
  // ****************************************************************** //

  // if not running systematics, need the nominal
  // if running systematics, and running them all, need the nominal
  // add it to the front!

  if ( m_systList.empty() || ( !m_systList.empty() && m_systName == "All" ) ) {
    m_systList.insert( m_systList.begin(), CP::SystematicSet() );
    const CP::SystematicVariation nullVar = CP::SystematicVariation(""); // blank = nominal
    m_systList.back().insert(nullVar);
  }

  if ( m_debug ) {
    for ( const auto& syst_it : m_systList ) {
      Info("initialize()"," available systematic: %s", (syst_it.name()).c_str());
    }
  }

  for ( const auto& syst_it : m_systList ) {
    Info("initialize()"," Running with systematic : %s", (syst_it.name()).c_str());
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

  // get the collection from TEvent or TStore
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  const xAOD::MuonContainer* inMuons(nullptr);
  RETURN_CHECK("MuonCalibrator::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName, m_event, m_store, m_verbose) ,"");

  // loop over available systematics - remember syst == EMPTY_STRING --> baseline
  // prepare a vector of the names of CDV containers
  // must be a pointer to be recorded in TStore
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

  for ( const auto& syst_it : m_systList ) {

    std::string outSCContainerName(m_outSCContainerName);
    std::string outSCAuxContainerName(m_outSCAuxContainerName);
    std::string outContainerName(m_outContainerName);

    // always append the name of the variation, including nominal which is an empty string
    outSCContainerName    += syst_it.name();
    outSCAuxContainerName += syst_it.name();
    outContainerName      += syst_it.name();
    vecOutContainerNames->push_back( syst_it.name() );

    // apply syst
    if ( m_runSysts ) {
      if ( m_muonCalibrationAndSmearingTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
        Error("initialize()", "Failed to configure MuonCalibrationAndSmearingTool for systematic %s", syst_it.name().c_str());
        return EL::StatusCode::FAILURE;
      }
    }

    // create shallow copy for calibration - one per syst
    std::pair< xAOD::MuonContainer*, xAOD::ShallowAuxContainer* > calibMuonsSC = xAOD::shallowCopyContainer( *inMuons );
    // create ConstDataVector to be eventually stored in TStore
    ConstDataVector<xAOD::MuonContainer>* calibMuonsCDV = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
    calibMuonsCDV->reserve( calibMuonsSC.first->size() );

    // now calibrate!
    unsigned int idx(0);
    for ( auto muSC_itr : *(calibMuonsSC.first) ) {

      if ( m_debug ) { Info("execute()", "  uncailbrated muon %i, pt = %.2f GeV", idx, (muSC_itr->pt() * 1e-3)); }

      // calibrate only MC
      if ( m_isMC ) {

        if ( m_muonCalibrationAndSmearingTool->applyCorrection(*muSC_itr) == CP::CorrectionCode::Error ) {
          // Can have CorrectionCode values of Ok, OutOfValidityRange, or Error. Here only checking for Error.
          // If OutOfValidityRange is returned no modification is made and the original muon values are taken.
          Warning("execute()", "MuonCalibrationAndSmearingTool returns Error CorrectionCode");
        }
      }
      
      if ( m_debug ) { Info("execute()", "  corrected muon pt = %.2f GeV", (muSC_itr->pt() * 1e-3)); }

      ++idx;
    } // close calibration loop

    if ( !xAOD::setOriginalObjectLink(*inMuons, *(calibMuonsSC.first)) ) {
      Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    if ( m_sort ) {
      std::sort( calibMuonsCDV->begin(), calibMuonsCDV->end(), HelperFunctions::sort_pt );
    }

    // save pointers in ConstDataVector with same order
    RETURN_CHECK( "MuonCalibrator::execute()", HelperFunctions::makeSubsetCont(calibMuonsSC.first, calibMuonsCDV, "", ToolName::CALIBRATOR), "");

    // add SC container to TStore
    RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( calibMuonsSC.first,  outSCContainerName  ), "Failed to store container.");
    RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( calibMuonsSC.second, outSCAuxContainerName ), "Failed to store aux container.");
    // add ConstDataVector to TStore
    RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( calibMuonsCDV, outContainerName), "Failed to store const data container.");

  } // close loop on systematics

  // add vector<string container_names_syst> to TStore
  RETURN_CHECK( "MuonCalibrator::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  // look what do we have in TStore
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

  if ( m_muonCalibrationAndSmearingTool ) {
    delete m_muonCalibrationAndSmearingTool; m_muonCalibrationAndSmearingTool = nullptr;
  }

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

  return EL::StatusCode::SUCCESS;
}
