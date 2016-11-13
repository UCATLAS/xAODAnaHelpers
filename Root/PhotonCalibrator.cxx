/*******************************************************
 *
 * Interface to CP Photon calibration tool(s).
 *
 * The tool applies:
 *
 * -) scale corrections for DATA
 * -) smearing corrections for MC
 * (data VS. MC check is done by the CP tool internally)
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *******************************************************/

// c++ include(s):
#include <iostream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include <xAODEventInfo/EventInfo.h>
#include <xAODEgamma/PhotonContainer.h>
#include <xAODEgamma/Photon.h>
#include <xAODBase/IParticleHelpers.h>
#include <xAODBase/IParticleContainer.h>
#include <xAODBase/IParticle.h>
#include <AthContainers/ConstDataVector.h>
#include <AthContainers/DataVector.h>
#include <xAODCore/ShallowCopy.h>
#include <xAODEgamma/EgammaDefs.h>
#include <xAODEgamma/EgammaxAODHelpers.h>

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/PhotonCalibrator.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>


#include "ElectronPhotonFourMomentumCorrection/EgammaCalibrationAndSmearingTool.h"
#include "IsolationCorrections/IsolationCorrectionTool.h"
#include "ElectronPhotonSelectorTools/AsgPhotonIsEMSelector.h"
#include "ElectronPhotonShowerShapeFudgeTool/ElectronPhotonShowerShapeFudgeTool.h"
#include "PhotonEfficiencyCorrection/AsgPhotonEfficiencyCorrectionTool.h"
#include "PathResolver/PathResolver.h"

#include <PATCore/PATCoreEnums.h>

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(PhotonCalibrator)


PhotonCalibrator :: PhotonCalibrator (std::string className) :
    Algorithm(className),
    m_EgammaCalibrationAndSmearingTool(nullptr),
    m_photonFudgeMCTool(nullptr),
    m_photonTightIsEMSelector(nullptr),
    m_photonMediumIsEMSelector(nullptr),
    m_photonLooseIsEMSelector(nullptr),
    m_photonTightEffTool(nullptr),
    m_photonMediumEffTool(nullptr),
    m_photonLooseEffTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("PhotonCalibrator()", "Calling constructor");

  m_debug                   = false;

  // input container to be read from TEvent or TStore
  //
  m_inContainerName         = "";
  m_outContainerName        = "";

  m_sort                    = true;
  m_isMC                    = false;

  m_toolInitializationAtTheFirstEventDone = false;

  // Systematics stuff
  //
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "PhotonCalibrator_Syst";
  m_systName		    = "";
  m_systVal 		    = 0.;

  m_esModel                 = "";
  m_decorrelationModel      = "";

}


EL::StatusCode PhotonCalibrator :: setupJob (EL::Job& job)
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
  xAOD::Init( "PhotonCalibrator" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonCalibrator :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonCalibrator :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonCalibrator :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonCalibrator :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing PhotonCalibrator Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( m_inContainerName.empty() ) {
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  MSG::Level msgLevel = (m_debug) ? MSG::VERBOSE : MSG::INFO;

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the CP::EgammaCalibrationAndSmearingTool
  //
  const std::string CalibToolName = m_name + "_EgammaCalibrationAndSmearingTool_Photons";
  if ( asg::ToolStore::contains<CP::EgammaCalibrationAndSmearingTool>(CalibToolName.c_str()) ) {
    m_EgammaCalibrationAndSmearingTool = asg::ToolStore::get<CP::EgammaCalibrationAndSmearingTool>(CalibToolName.c_str());
  } else {
    m_EgammaCalibrationAndSmearingTool = new CP::EgammaCalibrationAndSmearingTool(CalibToolName.c_str());
  }

  RETURN_CHECK( "PhotonCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", m_esModel),"Failed to set property ESModel");
  RETURN_CHECK( "PhotonCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->setProperty("decorrelationModel", m_decorrelationModel),"Failed to set property decorrelationModel");
  if (m_useAFII) {
    RETURN_CHECK( "PhotonCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->setProperty("useAFII", 1), "Failed to set property useAFII");
  }
  RETURN_CHECK( "PhotonCalibrator::initialize()", m_EgammaCalibrationAndSmearingTool->initialize(), "Failed to properly initialize the EgammaCalibrationAndSmearingTool");
  m_EgammaCalibrationAndSmearingTool->msg().setLevel( msgLevel );

  // Get a list of recommended systematics for this tool
  //
  //const CP::SystematicRegistry& systReg = CP::SystematicRegistry::getInstance();
  const CP::SystematicSet& recSyst = m_EgammaCalibrationAndSmearingTool->recommendedSystematics();;
  Info("initialize()"," Initializing Photon Calibrator Systematics :");

  m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, m_debug );

  Info("initialize()","Will be using EgammaCalibrationAndSmearingTool systematic:");

  std::vector< std::string >* SystPhotonsNames = new std::vector< std::string >;
  for ( const auto& syst_it : m_systList ) {
    SystPhotonsNames->push_back(syst_it.name());
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }
    RETURN_CHECK("PhotonCalibrator::initialize()",m_store->record(SystPhotonsNames, "photons_Syst"+m_name ), "Failed to record vector of jet systs names.");

  //isEM selector tools
  //------------------
  //create the selectors
  // Tight
  const std::string TightSelectorName = m_name + "_PhotonTightIsEMSelector";
  if ( asg::ToolStore::contains<AsgPhotonIsEMSelector>(TightSelectorName.c_str()) ) {
    m_photonTightIsEMSelector = asg::ToolStore::get<AsgPhotonIsEMSelector>(TightSelectorName.c_str());
  } else {
    m_photonTightIsEMSelector = new AsgPhotonIsEMSelector ( TightSelectorName.c_str() );
  }
  m_photonTightIsEMSelector->msg().setLevel( msgLevel );

  // Medium
  const std::string MediumSelectorName = m_name + "_PhotonMediumIsEMSelector";
  if ( asg::ToolStore::contains<AsgPhotonIsEMSelector>(MediumSelectorName.c_str()) ) {
    m_photonMediumIsEMSelector = asg::ToolStore::get<AsgPhotonIsEMSelector>(MediumSelectorName.c_str());
  } else {
    m_photonMediumIsEMSelector = new AsgPhotonIsEMSelector ( MediumSelectorName.c_str() );
  }
  m_photonMediumIsEMSelector->msg().setLevel( msgLevel );

  // Loose
  const std::string LooseSelectorName = m_name + "_PhotonLooseIsEMSelector";
  if ( asg::ToolStore::contains<AsgPhotonIsEMSelector>(LooseSelectorName.c_str()) ) {
    m_photonLooseIsEMSelector = asg::ToolStore::get<AsgPhotonIsEMSelector>(LooseSelectorName.c_str());
  } else {
    m_photonLooseIsEMSelector = new AsgPhotonIsEMSelector ( LooseSelectorName.c_str() );
  }
  m_photonLooseIsEMSelector->msg().setLevel( msgLevel );

  //set the type of selection
  RETURN_CHECK("PhotonHandler::initializeTools()", m_photonTightIsEMSelector->setProperty("isEMMask", egammaPID::PhotonTight), "failed in initialization");
  RETURN_CHECK("PhotonHandler::initializeTools()", m_photonMediumIsEMSelector->setProperty("isEMMask", egammaPID::PhotonMedium), "failed in initialization");
  RETURN_CHECK("PhotonHandler::initializeTools()", m_photonLooseIsEMSelector->setProperty("isEMMask", egammaPID::PhotonLoose), "failed in initialization");

  //set the configuration file
  // todo : monitor the config files!
  RETURN_CHECK("PhotonHandler::initializeTools()",
	       m_photonTightIsEMSelector->setProperty("ConfigFile","ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMTightSelectorCutDefs.conf"),
	       "failed in setting property");
  RETURN_CHECK("PhotonHandler::initializeTools()",
	       m_photonMediumIsEMSelector->setProperty("ConfigFile","ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMMediumSelectorCutDefs.conf"),
	       "failed in setting property");
  RETURN_CHECK("PhotonHandler::initializeTools()",
	       m_photonLooseIsEMSelector->setProperty("ConfigFile","ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMLooseSelectorCutDefs.conf"),
	       "failed in setting property");


  RETURN_CHECK("PhotonHandler::initializeTools()", m_photonTightIsEMSelector->initialize(), "failed in initialization for Tight");
  RETURN_CHECK("PhotonHandler::initializeTools()", m_photonMediumIsEMSelector->initialize(), "failed in initialization for Medium");
  RETURN_CHECK("PhotonHandler::initializeTools()", m_photonLooseIsEMSelector->initialize(), "failed in initialization for Loose");

  // ***********************************************************

  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //

  // fudge MC tool
  //--------------
  const std::string FudgeMCToolName = m_name + "PhotonFudgeMCTool";
  if ( asg::ToolStore::contains<ElectronPhotonShowerShapeFudgeTool>(FudgeMCToolName.c_str()) ) {
    m_photonFudgeMCTool = asg::ToolStore::get<ElectronPhotonShowerShapeFudgeTool>(FudgeMCToolName.c_str());
  } else {
    m_photonFudgeMCTool = new ElectronPhotonShowerShapeFudgeTool(FudgeMCToolName.c_str());
  }
  m_photonFudgeMCTool->msg().setLevel( msgLevel );
  RETURN_CHECK("PhotonHandler::initializeTools()", m_photonFudgeMCTool->initialize(),
	       "failed in initialization of fudge MC tool");

  Info("initialize()", "PhotonCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode PhotonCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Photon Calibration ... "); }

  m_numEvent++;

  // get the collection from TEvent or TStore
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("PhotonCalibrator::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,
	       Form("Failed in retrieving %s in %s", m_inContainerName.c_str(), m_name.c_str() ));
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) ? true : false;

  if ( ! m_toolInitializationAtTheFirstEventDone ) {
    EL_RETURN_CHECK ("PhotonCalibrator::execute() ", toolInitializationAtTheFirstEvent (eventInfo));
  }



  const xAOD::PhotonContainer* inPhotons(nullptr);
  RETURN_CHECK("PhotonCalibrator::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName, m_event, m_store, m_verbose) ,
	       Form("Failed in retrieving %s in %s", m_inContainerName.c_str(), m_name.c_str() ));

  if ( m_debug ) { Info("execute()", "Retrieve has been completed with container name = %s", m_inContainerName.c_str()); }

  // loop over available systematics - remember syst == EMPTY_STRING --> baseline
  // prepare a vector of the names of CDV containers
  // must be a pointer to be recorded in TStore
  //
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

  for ( const auto& syst_it : m_systList ) {
    if ( m_debug ) { Info("execute()", "Systematic Loop for m_systList=%s", syst_it.name().c_str() ); }
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
    if (m_debug) { Info("execute()", "syst_it.name()=%s", syst_it.name().c_str()); }

    if ( m_EgammaCalibrationAndSmearingTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("execute()", "Failed to configure EgammaCalibrationAndSmearingTool for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }

    if ( m_debug ) { Info("execute()", "Systematics applied"); }
    // create shallow copy for calibration - one per syst
    //
    std::pair< xAOD::PhotonContainer*, xAOD::ShallowAuxContainer* > calibPhotonsSC = xAOD::shallowCopyContainer( *inPhotons );

    // create ConstDataVector to be eventually stored in TStore
    //
    ConstDataVector<xAOD::PhotonContainer>* calibPhotonsCDV = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
    calibPhotonsCDV->reserve( calibPhotonsSC.first->size() );

    // now calibrate!
    // four momentum calibration
    unsigned int idx(0);
    for ( auto phSC_itr : *(calibPhotonsSC.first) ) {

      // set smearing seeding if needed - no need for this after Base,2.1.26
      // m_EgammaCalibrationAndSmearingTool->setRandomSeed(eventInfo->eventNumber() + 100 * idx);
      //
      if ( m_debug ) {
        Info( "execute", "Checking photon %i, raw pt = %.2f GeV ", idx, (phSC_itr->pt() * 1e-3) );
      }

      if ( phSC_itr->pt() > 7e3 && !(phSC_itr->caloCluster()) ){
	Warning( "execute", "photon %i, raw pt = %.2f GeV, does not have caloCluster()! ", idx, (phSC_itr->pt() * 1e-3) );
      }

      // apply calibration (w/ syst)
      //
      if ( (phSC_itr->author() & xAOD::EgammaParameters::AuthorPhoton) || (phSC_itr->author() & xAOD::EgammaParameters::AuthorAmbiguous) ) {
	if ( m_EgammaCalibrationAndSmearingTool->applyCorrection( *phSC_itr ) != CP::CorrectionCode::Ok ) {
	  Warning("execute()", "Problem in CP::EgammaCalibrationAndSmearingTool::applyCorrection()");
	}

	//if ( m_IsolationCorrectionTool->applyCorrection( *phSC_itr ) != CP::CorrectionCode::Ok ) { /// indeed wrong
	if ( m_IsolationCorrectionTool->CorrectLeakage( *phSC_itr ) != CP::CorrectionCode::Ok ) {
	  Warning("execute()", "Problem in CP::IsolationCorrection::applyCorrection()");
	}
      }

      if ( m_debug ) { Info("execute()", "Calibrated pt with systematic: %s , pt = %.2f GeV", (syst_it).name().c_str(), (phSC_itr->pt() * 1e-3)); }

      EL_RETURN_CHECK ( "PhotonCalibrator::execute()", decorate(phSC_itr));

      ++idx;

    } // close calibration loop

    if ( !xAOD::setOriginalObjectLink(*inPhotons, *(calibPhotonsSC.first)) ) {
      Error("execute()  ", "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    // save pointers in ConstDataVector with same order
    //
    RETURN_CHECK( "PhotonCalibrator::execute()", HelperFunctions::makeSubsetCont(calibPhotonsSC.first, calibPhotonsCDV), "");

    // Sort after copying to CDV.
    if ( m_sort ) {
      std::sort( calibPhotonsCDV->begin(), calibPhotonsCDV->end(), HelperFunctions::sort_pt );
    }

    // add SC container to TStore
    //
    RETURN_CHECK( "PhotonCalibrator::execute()", m_store->record( calibPhotonsSC.first,  outSCContainerName  ), "Failed to store container.");
    RETURN_CHECK( "PhotonCalibrator::execute()", m_store->record( calibPhotonsSC.second, outSCAuxContainerName ), "Failed to store aux container.");
    // add ConstDataVector to TStore
    //
    RETURN_CHECK( "PhotonCalibrator::execute()", m_store->record( calibPhotonsCDV, outContainerName), "Failed to store const data container.");

  } // close loop on systematics

  // add vector<string container_names_syst> to TStore
  //
  RETURN_CHECK( "PhotonCalibrator::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");

  // look what we have in TStore
  //
  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonCalibrator :: finalize ()
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

  if ( m_EgammaCalibrationAndSmearingTool ) {
    delete m_EgammaCalibrationAndSmearingTool;
    m_EgammaCalibrationAndSmearingTool = nullptr;
  }
  if ( m_photonFudgeMCTool ) {
    delete m_photonFudgeMCTool;
    m_photonFudgeMCTool = nullptr;
  }

  if ( m_photonTightIsEMSelector ) {
    delete m_photonTightIsEMSelector;
    m_photonTightIsEMSelector = nullptr;
  }

  if (m_photonMediumIsEMSelector) {
    delete m_photonMediumIsEMSelector;
    m_photonMediumIsEMSelector = nullptr;
  }

  if (m_photonLooseIsEMSelector) {
    delete m_photonLooseIsEMSelector;
    m_photonLooseIsEMSelector = nullptr;
  }

  if (m_photonTightEffTool) {
    delete m_photonTightEffTool;
    m_photonTightEffTool = nullptr;
  }

  if (m_photonMediumEffTool) {
    delete m_photonMediumEffTool;
    m_photonMediumEffTool = nullptr;
  }

  if (m_photonLooseEffTool) {
    delete m_photonLooseEffTool;
    m_photonLooseEffTool = nullptr;
  }

  Info("finalize()", "Finalization done.");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonCalibrator :: histFinalize ()
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

EL::StatusCode PhotonCalibrator :: decorate(xAOD::Photon* photon)
{
  // (1) apply fudge factors
  if((!m_useAFII) && m_isMC){
    if(m_photonFudgeMCTool->applyCorrection(*photon) == CP::CorrectionCode::Error){
      Error("decorate()", "photonFudgeMCTool->applyCorrection(*photon) returned CP::CorrectionCode::Error");
      return EL::StatusCode::FAILURE;
    }
  }

  // (2) evaluate the ID quality
  const bool isTight  = m_photonTightIsEMSelector->accept(photon);
  const bool isMedium = m_photonMediumIsEMSelector->accept(photon);
  const bool isLoose  = m_photonLooseIsEMSelector->accept(photon);
  photon->auxdecor< bool >( "PhotonID_Tight"    ) = isTight;
  photon->auxdecor< bool >( "PhotonID_Medium"   ) = isMedium;
  photon->auxdecor< bool >( "PhotonID_Loose"    ) = isLoose;
  if (m_debug) {Info("decorate()", "isTight=%s isMedium=%s isLoose=%s", isTight ? "Y" : "N", isMedium ? "Y" : "N", isLoose ? "Y" : "N" ); }

  // (3) set efficiency correction
  if (m_isMC) {
    const xAOD::CaloCluster* cluster = photon->caloCluster();
    float cluster_eta = 10;
    float cluster_et = 0;
    if (cluster) {
      cluster_eta = cluster->etaBE(2);
      if (cluster_eta != 0.0) {
	cluster_et = cluster->e() / cosh(cluster_eta);
      }
    }

    bool inCrack = abs(cluster_eta)>1.37 && abs(cluster_eta)<1.52;

    // photon SF
    double photonTightEffSF(1.), photonMediumEffSF(1.), photonLooseEffSF(1.);
    double photonTightEffSFError(0.), photonMediumEffSFError(0.), photonLooseEffSFError(0.);

    // configuration files not yet available for 13 TeV :(
    //sf only available after basic kinematic selection
    if(cluster_et > 20000. && fabs(cluster_eta) < 2.37 && !inCrack){
      // SF
      if(m_photonTightEffTool->getEfficiencyScaleFactor(*photon, photonTightEffSF) == CP::CorrectionCode::Error){
	Error("PhotonHandler::decorate()", "getEfficiencyScaleFactor returned CP::CorrectionCode::Error");
	return EL::StatusCode::FAILURE;
      }
      if(m_photonMediumEffTool->getEfficiencyScaleFactor(*photon, photonMediumEffSF) == CP::CorrectionCode::Error){
	Error("PhotonHandler::decorate()", "getEfficiencyScaleFactor returned CP::CorrectionCode::Error");
	return EL::StatusCode::FAILURE;
      }
      if(m_photonLooseEffTool->getEfficiencyScaleFactor(*photon, photonLooseEffSF) == CP::CorrectionCode::Error){
	Error("PhotonHandler::decorate()", "getEfficiencyScaleFactor returned CP::CorrectionCode::Error");
	return EL::StatusCode::FAILURE;
      }
      // SF error
      if(m_photonTightEffTool->getEfficiencyScaleFactorError(*photon, photonTightEffSFError) == CP::CorrectionCode::Error){
	Error("PhotonHandler::decorate()", "getEfficiencyScaleFactorError returned CP::CorrectionCode::Error");
	return EL::StatusCode::FAILURE;
      }
      if(m_photonMediumEffTool->getEfficiencyScaleFactorError(*photon, photonMediumEffSFError) == CP::CorrectionCode::Error){
	Error("PhotonHandler::decorate()", "getEfficiencyScaleFactorError returned CP::CorrectionCode::Error");
	return EL::StatusCode::FAILURE;
      }
      if(m_photonLooseEffTool->getEfficiencyScaleFactorError(*photon, photonLooseEffSFError) == CP::CorrectionCode::Error){
	Error("PhotonHandler::decorate()", "getEfficiencyScaleFactorError returned CP::CorrectionCode::Error");
	return EL::StatusCode::FAILURE;
      }
    }

    photon->auxdecor< float >( "PhotonID_Tight_EffSF"  ) = photonTightEffSF;
    photon->auxdecor< float >( "PhotonID_Medium_EffSF" ) = photonMediumEffSF;
    photon->auxdecor< float >( "PhotonID_Loose_EffSF"  ) = photonLooseEffSF;

    photon->auxdecor< float >( "PhotonID_Tight_EffSF_Error"  ) = photonTightEffSFError;
    photon->auxdecor< float >( "PhotonID_Medium_EffSF_Error" ) = photonMediumEffSFError;
    photon->auxdecor< float >( "PhotonID_Loose_EffSF_Error"  ) = photonLooseEffSFError;

    if (m_debug) {Info("decorate()", "Tight=%f (%f) Medium=%f (%f) Loose=%f (%f)",
		       photonTightEffSF, photonTightEffSFError,
		       photonMediumEffSF, photonMediumEffSFError,
		       photonLooseEffSF, photonLooseEffSFError); }
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode  PhotonCalibrator :: toolInitializationAtTheFirstEvent ( const xAOD::EventInfo* eventInfo )
{
  MSG::Level msgLevel = (m_debug) ? MSG::VERBOSE : MSG::INFO;

  // Efficiency correction
  if (m_isMC) {
    // in order to use EventInfo
    // some tools are initialized after the first events are stored from file
    if (m_toolInitializationAtTheFirstEventDone) {
      Error("toolInitializationAtTheFirstEvent ()", "please call this function only when m_toolInitializationAtTheFirstEventDone = false");
      return EL::StatusCode::FAILURE;
    }

    int dataType = PATCore::ParticleDataType::Data;
    if (eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION )) {
      if (m_useAFII) {
	dataType = PATCore::ParticleDataType::Fast;
      } else {
	dataType = PATCore::ParticleDataType::Full;
      }
    }

    if (m_debug) { Info("toolInitializationAtTheFirstEvent()", "isSimulation=%s isAFII=%s selected dataType=%d",
			( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ? "Y" : "N"), (m_useAFII ? "Y" : "N" ), dataType ); }

    // photon efficiency correction tool
    //----------------------------------
    //create the tools
    // Tight
    const std::string TightCorrectorName = m_name + "_PhotonTightEfficiencyCorrectionTool";
    if ( asg::ToolStore::contains<AsgPhotonEfficiencyCorrectionTool>(TightCorrectorName.c_str()) ) {
      m_photonTightEffTool = asg::ToolStore::get<AsgPhotonEfficiencyCorrectionTool>(TightCorrectorName.c_str());
    } else {
      m_photonTightEffTool = new AsgPhotonEfficiencyCorrectionTool(TightCorrectorName.c_str());
    }
    m_photonTightEffTool->msg().setLevel( msgLevel );

    // Medium
    const std::string MediumCorrectorName = m_name + "_PhotonMediumEfficiencyCorrectionTool";
    if ( asg::ToolStore::contains<AsgPhotonEfficiencyCorrectionTool>(MediumCorrectorName.c_str()) ) {
      m_photonMediumEffTool = asg::ToolStore::get<AsgPhotonEfficiencyCorrectionTool>(MediumCorrectorName.c_str());
    } else {
      m_photonMediumEffTool = new AsgPhotonEfficiencyCorrectionTool(MediumCorrectorName.c_str());
    }
    m_photonMediumEffTool->msg().setLevel( msgLevel );

    // Loose
    const std::string LooseCorrectorName = m_name + "_PhotonLooseEfficiencyCorrectionTool";
    if ( asg::ToolStore::contains<AsgPhotonEfficiencyCorrectionTool>(LooseCorrectorName.c_str()) ) {
      m_photonLooseEffTool = asg::ToolStore::get<AsgPhotonEfficiencyCorrectionTool>(LooseCorrectorName.c_str());
    } else {
      m_photonLooseEffTool = new AsgPhotonEfficiencyCorrectionTool(LooseCorrectorName.c_str());
    }
    m_photonLooseEffTool->msg().setLevel( msgLevel );

    // todo : Set input files ** not yet available for 13 TeV ** Date : 13 May
    // recommended files here: https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PhotonEfficiencyRun2#Recommended_input_files
    std::string file_unc = PathResolverFindCalibFile("PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.unc.v01.root");
    std::string file_con = PathResolverFindCalibFile("PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.con.v01.root");
    if(m_useAFII){
      file_unc = PathResolverFindCalibFile("PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.AFII.unc.v01.root");
      file_con = PathResolverFindCalibFile("PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.AFII.con.v01.root");
    }

    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonTightEffTool->setProperty("CorrectionFileNameConv",file_con), "failed in setting property");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonTightEffTool->setProperty("CorrectionFileNameUnconv",file_unc), "failed in setting property");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonMediumEffTool->setProperty("CorrectionFileNameConv",file_con), "failed in setting property");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonMediumEffTool->setProperty("CorrectionFileNameUnconv",file_unc), "failed in setting property");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonLooseEffTool->setProperty("CorrectionFileNameConv",file_con), "failed in setting property");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonLooseEffTool->setProperty("CorrectionFileNameUnconv",file_unc), "failed in setting property");

    // set data type
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonTightEffTool->setProperty("ForceDataType", dataType), "failed in setting property");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonMediumEffTool->setProperty("ForceDataType", dataType), "failed in setting property");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonLooseEffTool->setProperty("ForceDataType", dataType), "failed in setting property");

    //initialize
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonTightEffTool->initialize(), "failed in initialization");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonMediumEffTool->initialize(), "failed in initialization");
    RETURN_CHECK("PhotonHandler::initializeTools()", m_photonLooseEffTool->initialize(), "failed in initialization");

  }

  //IsolationCorrectionTool
  const std::string IsoCorrToolName = m_name + "_IsolationCorrectiongTool_Photons";
  if ( asg::ToolStore::contains<CP::IsolationCorrectionTool>(IsoCorrToolName.c_str()) ) {
    m_IsolationCorrectionTool = asg::ToolStore::get<CP::IsolationCorrectionTool>(IsoCorrToolName.c_str());
  } else {
    m_IsolationCorrectionTool = new CP::IsolationCorrectionTool(IsoCorrToolName.c_str());
  }

  if (m_isMC) {
    RETURN_CHECK( "PhotonCalibrator::initializeTool()", m_IsolationCorrectionTool->setProperty("IsMC", true), "Failed in IsolationCorrectionTool setProperty");
  } else {
    RETURN_CHECK( "PhotonCalibrator::initializeTool()", m_IsolationCorrectionTool->setProperty("IsMC", false), "Failed in IsolationCorrectionTool setProperty");
  }
  RETURN_CHECK( "PhotonCalibrator::initializeTool()", m_IsolationCorrectionTool->initialize(), "Failed in IsolationCorrectionTool initialization");
  m_IsolationCorrectionTool->msg().setLevel( msgLevel );

  m_toolInitializationAtTheFirstEventDone = true;

  return EL::StatusCode::SUCCESS;
}
