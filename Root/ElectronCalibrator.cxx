/**********************************************
 *
 * Interface to CP Electron calibration tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *
 *********************************************/

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

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronCalibrator)


ElectronCalibrator :: ElectronCalibrator () :
  m_EgammaCalibrationAndSmearingTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronCalibrator()", "Calling constructor");

  // read debug flag from .config file
  m_debug                   = false;

  // input container to be read from TEvent or TStore
  m_inContainerName         = "";
  m_outContainerName        = "";

  // Systematics stuff
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "ElectronCalibrator_Syst";
  m_runSysts                = false; // gets set later is syst applies to this tool
  m_systName		    = "";
  m_systVal 		    = 0.;

  m_esModel                 = "";
  m_decorrelationModel      = "";

  m_sort                    = true;

}


EL::StatusCode  ElectronCalibrator :: configure ()
{

  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing ElectronCalibrator Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug                   = config->GetValue("Debug", m_debug);
    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());
    m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());

   // Systematics stuff
    m_inputAlgoSystNames      = config->GetValue("InputAlgoSystNames",  m_inputAlgoSystNames.c_str());
    m_outputAlgoSystNames     = config->GetValue("OutputAlgoSystNames", m_outputAlgoSystNames.c_str());
    m_runSysts                = false; // gets set later is syst applies to this tool
    m_systName		      = config->GetValue("SystName" , m_systName.c_str() );
    m_systVal 		      = config->GetValue("SystVal" , m_systVal );
    m_runAllSyst              = (m_systName.find("All") != std::string::npos);

    m_esModel		      = config->GetValue("ESModel" , m_esModel.c_str() );
    m_decorrelationModel      = config->GetValue("DecorrelationModel" , m_decorrelationModel.c_str() );

    m_sort                    = config->GetValue("Sort", m_sort);

    config->Print();

    Info("configure()", "ElectronCalibrator Interface succesfully configured! ");

    delete config; config = nullptr;
  }

  if ( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

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

  Info("setupJob()", "Calling setupJob");

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

  Info("initialize()", "Initializing ElectronCalibrator Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the CP EgammaCalibrationAndSmearing tool
  //
  std::string egcas_tool_name = std::string("EgammaCalibrationAndSmearingTool_") + m_name;
  m_EgammaCalibrationAndSmearingTool = new CP::EgammaCalibrationAndSmearingTool( egcas_tool_name.c_str() );
  m_EgammaCalibrationAndSmearingTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", m_esModel),"Failed to set property ESModel");
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->setProperty("decorrelationModel", m_decorrelationModel),"Failed to set property decorrelationModel");
  RETURN_CHECK( "ElectronCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->initialize(), "Failed to properly initialize the EgammaCalibrationAndSmearingTool");

  // get a list of systematics
  //
  const CP::SystematicRegistry& systReg = CP::SystematicRegistry::getInstance();
  const CP::SystematicSet& recSyst = (systReg.recommendedSystematics());
  Info("initialize()"," Initializing Electron Calibrator Systematics :");
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

  Info("initialize()", "ElectronCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Electron Calibration ... "); }

  m_numEvent++;

  // get the collection from TEvent or TStore
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  const xAOD::ElectronContainer* inElectrons(nullptr);
  RETURN_CHECK("ElectronCalibrator::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName, m_event, m_store, m_verbose) ,"");

  // loop over available systematics - remember syst == EMPTY_STRING --> baseline
  // prepare a vector of the names of CDV containers
  // must be a pointer to be recorded in TStore
  //
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

  for ( const auto& syst_it : m_systList ) {

    // discard photon systematics
    //
    //if ( (syst_it.name()).find("PH_", 0) != std::string::npos ) { continue; }

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
    if ( m_runSysts ) {
      if ( m_EgammaCalibrationAndSmearingTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
        Error("initialize()", "Failed to configure EgammaCalibrationAndSmearingTool for systematic %s", syst_it.name().c_str());
        return EL::StatusCode::FAILURE;
      }
    }

    // create shallow copy for calibration - one per syst
    //
    std::pair< xAOD::ElectronContainer*, xAOD::ShallowAuxContainer* > calibElectronsSC = xAOD::shallowCopyContainer( *inElectrons );

    // create ConstDataVector to be eventually stored in TStore
    //
    ConstDataVector<xAOD::ElectronContainer>* calibElectronsCDV = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
    calibElectronsCDV->reserve( calibElectronsSC.first->size() );

    // now calibrate!
    //
    unsigned int idx(0);
    for ( auto elSC_itr : *(calibElectronsSC.first) ) {

      // set smearing seeding if needed - no need for this after Base,2.1.26
      // m_EgammaCalibrationAndSmearingTool->setRandomSeed(eventInfo->eventNumber() + 100 * idx);
      //
      if ( m_debug ) {
        Info( "execute", "Checking electron %i, raw pt = %.2f GeV ", idx, (elSC_itr->pt() * 1e-3) );
        if ( elSC_itr->pt() > 7e3 && !(elSC_itr->caloCluster()) ){
	  Warning( "execute", "electron %i, raw pt = %.2f GeV, does not have caloCluster()! ", idx, (elSC_itr->pt() * 1e-3) );
	}
      }

      // apply calibration (w/ syst)
      //
      if ( elSC_itr->caloCluster() && elSC_itr->trackParticle() ) {  // NB: derivations might remove CC and tracks for low pt electrons
        if ( m_EgammaCalibrationAndSmearingTool->applyCorrection( *elSC_itr ) != CP::CorrectionCode::Ok ) {
          Error("execute()", "Problem in m_EgammaCalibrationAndSmearingTool->applyCorrection()");
          return EL::StatusCode::FAILURE;
        }
      }

      if ( m_debug ) { Info("execute()", "Calibrated pt with systematic: %s , pt = %.2f GeV", (syst_it).name().c_str(), (elSC_itr->pt() * 1e-3)); }

      ++idx;
    } // close calibration loop

    if ( !xAOD::setOriginalObjectLink(*inElectrons, *(calibElectronsSC.first)) ) {
      Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    if ( m_sort ) {
      std::sort( calibElectronsCDV->begin(), calibElectronsCDV->end(), HelperFunctions::sort_pt );
    }

    // save pointers in ConstDataVector with same order
    RETURN_CHECK( "ElectronCalibrator::execute()", HelperFunctions::makeSubsetCont(calibElectronsSC.first, calibElectronsCDV, "", ToolName::CALIBRATOR), "");

    // add SC container to TStore
    RETURN_CHECK( "ElectronCalibrator::execute()", m_store->record( calibElectronsSC.first,  outSCContainerName  ), "Failed to store container.");
    RETURN_CHECK( "ElectronCalibrator::execute()", m_store->record( calibElectronsSC.second, outSCAuxContainerName ), "Failed to store aux container.");
    // add ConstDataVector to TStore
    RETURN_CHECK( "ElectronCalibrator::execute()", m_store->record( calibElectronsCDV, outContainerName), "Failed to store const data container.");

  } // close loop on systematics

  // add vector<string container_names_syst> to TStore
  RETURN_CHECK( "ElectronCalibrator::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  // look what do we have in TStore
  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

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

  Info("finalize()", "Deleting tool instances...");

  if ( m_EgammaCalibrationAndSmearingTool ) { delete m_EgammaCalibrationAndSmearingTool; m_EgammaCalibrationAndSmearingTool = nullptr; }

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

  Info("histFinalize()", "Calling histFinalize");

  return EL::StatusCode::SUCCESS;
}

