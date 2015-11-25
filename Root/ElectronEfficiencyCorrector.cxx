/******************************************************
 *
 * Interface to CP Electron Efficiency Correction Tool.
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 ******************************************************/

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

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/ElectronEfficiencyCorrector.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronEfficiencyCorrector)


ElectronEfficiencyCorrector :: ElectronEfficiencyCorrector (std::string className) :
    Algorithm(className),
    m_asgElEffCorrTool_elSF_PID(nullptr),
    m_asgElEffCorrTool_elSF_Reco(nullptr),
    m_asgElEffCorrTool_elSF_Trig(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronEfficiencyCorrector()", "Calling constructor");

  m_debug                   = false;

  // input container to be read from TEvent or TStore
  //
  m_inContainerName         = "";

  // Systematics stuff
  //
  m_inputAlgoSystNames      = "";
  m_systValPID              = 0.0;
  m_systValReco             = 0.0;
  m_systValTrig             = 0.0;
  m_systNamePID             = "";
  m_systNameReco            = "";
  m_systNameTrig            = "";
  m_outputSystNamesPID      = "ElectronEfficiencyCorrector_PIDSyst";
  m_outputSystNamesReco     = "ElectronEfficiencyCorrector_RecoSyst";
  m_outputSystNamesTrig     = "ElectronEfficiencyCorrector_TrigSyst";

  // file(s) containing corrections
  //
  m_corrFileNamePID         = "";
  m_corrFileNameReco        = "";
  m_corrFileNameTrig        = "";
}


EL::StatusCode  ElectronEfficiencyCorrector :: configure ()
{

  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing ElectronEfficiencyCorrector Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    //
    m_debug                   = config->GetValue("Debug", m_debug);

    // input container to be read from TEvent or TStore
    //
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());

    // Systematics stuff
    //
    m_inputAlgoSystNames      = config->GetValue("InputAlgoSystNames",  m_inputAlgoSystNames.c_str());
    m_systNamePID             = config->GetValue("SystNamePID"  , m_systNamePID.c_str());
    m_systNameReco            = config->GetValue("SystNameReco" , m_systNameReco.c_str());
    m_systNameTrig            = config->GetValue("SystNameTrig" , m_systNameTrig.c_str());
    m_outputSystNamesPID      = config->GetValue("OutputSystNamesPID" ,  m_outputSystNamesPID.c_str());
    m_outputSystNamesReco     = config->GetValue("OutputSystNamesReco",  m_outputSystNamesReco.c_str());
    m_outputSystNamesTrig     = config->GetValue("OutputSystNamesTrig",  m_outputSystNamesTrig.c_str());
    m_systValPID              = config->GetValue("SystValPID"  , m_systValPID);
    m_systValReco             = config->GetValue("SystValReco" , m_systValReco);
    m_systValTrig             = config->GetValue("SystValTrig" , m_systValTrig);
    // file(s) containing corrections
    m_corrFileNamePID         = config->GetValue("CorrectionFileNamePID" , m_corrFileNamePID.c_str());
    m_corrFileNameReco        = config->GetValue("CorrectionFileNameReco" , m_corrFileNameReco.c_str());
    m_corrFileNameTrig        = config->GetValue("CorrectionFileNameTrig" , m_corrFileNameTrig.c_str());

    config->Print();

    Info("configure()", "ElectronEfficiencyCorrector Interface succesfully configured! ");

    delete config; config = nullptr;
  }

  if ( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: setupJob (EL::Job& job)
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
  xAOD::Init( "ElectronEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  RETURN_CHECK("xAH::Algorithm::initialize()", xAH::Algorithm::initialize(), "");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing ElectronEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronEfficiencyCorrector::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  m_numEvent      = 0;
  m_numObject     = 0;

  // 1.
  // initialize the AsgElectronEfficiencyCorrectionTool for PID efficiency SF
  //

  //
  // Parse the PID WP from m_corrFileNamePID (needs to be of the form "LH+WP")
  //
  std::size_t init_pos = m_corrFileNamePID.find("offline") + 8;
  std::size_t end_pos  = m_corrFileNamePID.find("LH");
  m_PID_WP = "LH" + m_corrFileNamePID.substr( init_pos, (end_pos - init_pos) );
  if ( m_PID_WP.empty() ) {
    Error("initialize()", "m_PID_WP should not be empty! Exiting." );
    return EL::StatusCode::FAILURE;
  }
  //
  //  Add the chosen WP to the string labelling the vector<SF> decoration
  //
  m_outputSystNamesPID = m_outputSystNamesPID + "_" + m_PID_WP;

  std::string pidEffSF_tool_name = "ElectronEfficiencyCorrectionTool_effSF_PID_" + m_PID_WP;
  m_asgElEffCorrTool_elSF_PID = new AsgElectronEfficiencyCorrectionTool(pidEffSF_tool_name);

  m_asgElEffCorrTool_elSF_PID->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO

  std::vector<std::string> inputFilesPID{ m_corrFileNamePID } ; // initialise vector w/ all the files containing corrections
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("CorrectionFileNameList",inputFilesPID),"Failed to set property CorrectionFileNameList");

  int sim_flav(1); // default for FullSim
  if ( m_isMC ) {
    const std::string stringMeta = wk()->metaData()->castString("SimulationFlavour");
    if ( !stringMeta.empty() && ( stringMeta.find("AFII") != std::string::npos ) ) {
      Info("initialize()", "Setting simulation flavour to AFII");
      sim_flav = 3;
    }
  }
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("ForceDataType",sim_flav),"Failed to set property ForceDataType");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool PID");

  if ( m_debug ) {

    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsPID = m_asgElEffCorrTool_elSF_PID->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsPID ) { Info("initialize()","AsgElectronEfficiencyCorrectionTool can be affected by PID efficiency systematic: %s", (syst_it.name()).c_str()); }
  }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsPID = m_asgElEffCorrTool_elSF_PID->recommendedSystematics();
  m_systListPID = HelperFunctions::getListofSystematics( recSystsPID, m_systNamePID, m_systValPID, m_debug );

  Info("initialize()","Will be using AsgElectronEfficiencyCorrectionTool PID efficiency systematic:");
  for ( const auto& syst_it : m_systListPID ) {
    if ( m_systNamePID.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  // 2.
  // initialize the AsgElectronEfficiencyCorrectionTool for Reco Efficiency SF
  //

  if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>("ElectronEfficiencyCorrectionTool_effSF_Reco") ) {
    m_asgElEffCorrTool_elSF_Reco = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>("ElectronEfficiencyCorrectionTool_effSF_Reco");
  } else {
    m_asgElEffCorrTool_elSF_Reco = new AsgElectronEfficiencyCorrectionTool("ElectronEfficiencyCorrectionTool_effSF_Reco");

    m_asgElEffCorrTool_elSF_Reco->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO

    std::vector<std::string> inputFilesReco{ m_corrFileNameReco } ; // initialise vector w/ all the files containing corrections
    RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->setProperty("CorrectionFileNameList",inputFilesReco),"Failed to set property CorrectionFileNameList");
    RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
    RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool Reco");
  }

  if ( m_debug ) {

    // Get a list of affecting systematics
    //
    CP::SystematicSet affectSystsReco = m_asgElEffCorrTool_elSF_Reco->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsReco ) { Info("initialize()","AsgElectronEfficiencyCorrectionTool can be affected by reco efficiency systematic: %s", (syst_it.name()).c_str()); }
  }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsReco = m_asgElEffCorrTool_elSF_Reco->recommendedSystematics();
  m_systListReco = HelperFunctions::getListofSystematics( recSystsReco, m_systNameReco, m_systValReco, m_debug );

  Info("initialize()","Will be using AsgElectronEfficiencyCorrectionTool reco efficiency systematic:");
  for ( const auto& syst_it : m_systListReco ) {
    if ( m_systNameReco.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  // 3.
  // Initialise the AsgElectronEfficiencyCorrectionTool for Trigger Efficiency SF
  //

  if ( asg::ToolStore::contains<AsgElectronEfficiencyCorrectionTool>("ElectronEfficiencyCorrectionTool_effSF_Trig") ) {
    m_asgElEffCorrTool_elSF_Trig = asg::ToolStore::get<AsgElectronEfficiencyCorrectionTool>("ElectronEfficiencyCorrectionTool_effSF_Trig");
  } else {
    m_asgElEffCorrTool_elSF_Trig = new AsgElectronEfficiencyCorrectionTool("ElectronEfficiencyCorrectionTool_effSF_Trig");

    m_asgElEffCorrTool_elSF_Trig->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO

    std::vector<std::string> inputFilesTrig{ m_corrFileNameTrig } ; // initialise vector w/ all the files containing corrections
    RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->setProperty("CorrectionFileNameList",inputFilesTrig),"Failed to set property CorrectionFileNameList");
    RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
    RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool Trig");
  }

  if ( m_debug ) {

    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsTrig = m_asgElEffCorrTool_elSF_Trig->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsTrig ) { Info("initialize()","AsgElectronEfficiencyCorrectionTool can be affected by Trig efficiency systematic: %s", (syst_it.name()).c_str()); }
  }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsTrig = m_asgElEffCorrTool_elSF_Trig->recommendedSystematics();
  m_systListTrig = HelperFunctions::getListofSystematics( recSystsTrig, m_systNameTrig, m_systValTrig, m_debug );

  Info("initialize()","Will be using AsgElectronEfficiencyCorrectionTool Trig efficiency systematic:");
  for ( const auto& syst_it : m_systListTrig ) {
    if ( m_systNameTrig.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  // *********************************************************************************

  Info("initialize()", "ElectronEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  m_numEvent++;

  if ( !m_isMC ) {
    if ( m_numEvent == 1 ) { Info("execute()", "Sample is Data! Do not apply any Electron Efficiency correction... "); }
    return EL::StatusCode::SUCCESS;
  }

  if ( m_debug ) { Info("execute()", "Applying Electron Efficiency Correction... "); }
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  // initialise containers
  //
  const xAOD::ElectronContainer* inputElectrons(nullptr);

  // if m_inputAlgoSystNames = "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it

  // Declare a counter, initialised to 0
  // For the systematically varied input containers, we won't store again the vector with efficiency systs in TStore ( it will be always the same!)
  //
  unsigned int countInputCont(0);

  if ( m_inputAlgoSystNames.empty() ) {

      RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(inputElectrons, m_inContainerName, m_event, m_store, m_verbose) ,"");

      // decorate electrons w/ SF - there will be a decoration w/ different name for each syst!
      //
      this->executeSF( inputElectrons, eventInfo, countInputCont );

  } else {
  // if m_inputAlgo = NOT EMPTY --> you are retrieving syst varied containers from an upstream algo.
  // This is the case of calibrators: one different SC for each calibration syst applied

      // get vector of string giving the syst names of the upstream algo m_inputAlgo (rememeber: 1st element is a blank string: nominal case!)
      //
      std::vector<std::string>* systNames(nullptr);
      RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

    	// loop over systematic sets available
	//
    	for ( auto systName : *systNames ) {

          RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(inputElectrons, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");

          if ( m_debug ){
              unsigned int idx(0);
              for ( auto el : *(inputElectrons) ) {
                  Info( "execute", "Input electron %i, pt = %.2f GeV ", idx, (el->pt() * 1e-3) );
                  ++idx;
              }
          }

          // decorate electrons w/ SF - there will be a decoration w/ different name for each syst!
	  //
          this->executeSF( inputElectrons, eventInfo, countInputCont );

          // increment counter
	  //
          ++countInputCont;

      } // close loop on systematic sets available from upstream algo

  }

  // look what we have in TStore
  //
  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: finalize ()
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

  if ( m_asgElEffCorrTool_elSF_PID )  { m_asgElEffCorrTool_elSF_PID = nullptr;   delete m_asgElEffCorrTool_elSF_PID;  }
  if ( m_asgElEffCorrTool_elSF_Reco ) {  m_asgElEffCorrTool_elSF_Reco = nullptr; delete m_asgElEffCorrTool_elSF_Reco; }
  if ( m_asgElEffCorrTool_elSF_Trig ) { m_asgElEffCorrTool_elSF_Trig = nullptr;  delete m_asgElEffCorrTool_elSF_Trig; }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronEfficiencyCorrector :: histFinalize ()
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

EL::StatusCode ElectronEfficiencyCorrector :: executeSF (  const xAOD::ElectronContainer* inputElectrons, const xAOD::EventInfo* eventInfo, unsigned int countSyst  )
{

  //
  // In the following, every electron gets decorated with 2 vector<double>'s (for reco/PID efficiency SFs),
  // and the event w/ 1 vector<double> (for trigger efficiency SFs)
  // Each vector contains the SFs, one SF for each syst (first component of each vector will be the nominal SF).
  //
  // Additionally, we create these vector<string> with the SF syst names, so that we know which component corresponds to.
  // ( there's a 1:1 correspondence with the vector<double> defined above )
  //
  // These vector<string> are eventually stored in TStore
  //
  std::vector< std::string >* sysVariationNamesPID  = new std::vector< std::string >;
  std::vector< std::string >* sysVariationNamesReco = new std::vector< std::string >;
  std::vector< std::string >* sysVariationNamesTrig = new std::vector< std::string >;

  // 1.
  // PID efficiency SFs - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Define also an *event* weight, which is the product of all the PID eff. SFs for each object in the event
  //
  std::string PID_SF_NAME_GLOBAL = m_outputSystNamesPID + "_GLOBAL";
  SG::AuxElement::Decorator< std::vector<float> > sfVecPID_GLOBAL ( PID_SF_NAME_GLOBAL );

  for ( const auto& syst_it : m_systListPID ) {

    // Initialise product of SFs for *this* systematic
    //
    float pidEffSF_GLOBAL(1.0);

    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_ElPIDEff_SF
    //
    std::string sfName  = "ElPIDEff_SF";

    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if(m_debug) Info("executeSF()", "Electron PID efficiency SF  name is: %s", sfName.c_str());
    sysVariationNamesPID->push_back(sfName);

    // apply syst
    //
    if ( m_asgElEffCorrTool_elSF_PID->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("executeSF()", "Failed to configure AsgElectronEfficiencyCorrectionTool_PID for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if ( m_debug ) { Info("executeSF()", "Successfully applied systematics: %s ", m_asgElEffCorrTool_elSF_PID->appliedSystematics().name().c_str() ); }

    // and now apply PID efficiency SF!
    //
    unsigned int idx(0);
    for ( auto el_itr : *(inputElectrons) ) {

       if ( m_debug ) { Info( "executeSF()", "Applying PID efficiency SF" ); }

       bool isBadElectron(false);

       //
       // obtain PID efficiency SF as a float (to be stored away separately)
       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
       //
       SG::AuxElement::Decorator< std::vector<float> > sfVecPID ( m_outputSystNamesPID  );
       if ( !sfVecPID.isAvailable( *el_itr )  ) {
         sfVecPID ( *el_itr ) = std::vector<float>();
       }

       // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
       //
       if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, it has no caloCluster or trackParticle info", idx); }
         isBadElectron = true;
       }
       //
       // skip electron if outside acceptance for SF calculation
       //
       if ( el_itr->pt() < 15e3 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside pT acceptance ( currently SF available for pT > 15 GeV )", idx); }
         isBadElectron = true;
       }
       if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside |eta| acceptance", idx); }
         isBadElectron = true;
       }

       //
       // obtain efficiency SF's for PID
       //
       double pidEffSF(1.0); // tool wants a double
       if ( !isBadElectron &&  m_asgElEffCorrTool_elSF_PID->getEfficiencyScaleFactor( *el_itr, pidEffSF ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in getEfficiencyScaleFactor");
	 pidEffSF = 1.0;
       }
       //
       // Add it to decoration vector
       //
       sfVecPID( *el_itr ).push_back( pidEffSF );

       pidEffSF_GLOBAL *= pidEffSF;

       if ( m_debug ) {
         Info( "executeSF()", "===>>>");
         Info( "executeSF()", " ");
	 Info( "executeSF()", "Electron %i, pt = %.2f GeV ", idx, (el_itr->pt() * 1e-3) );
	 Info( "executeSF()", " ");
	 Info( "executeSF()", "PID SF decoration: %s", m_outputSystNamesPID.c_str() );
	 Info( "executeSF()", " ");
         Info( "executeSF()", "Systematic: %s", syst_it.name().c_str() );
         Info( "executeSF()", " ");
         Info( "executeSF()", "PID efficiency SF:");
         Info( "executeSF()", "\t %f (from getEfficiencyScaleFactor())", pidEffSF );
         Info( "executeSF()", "--------------------------------------");
       }

       ++idx;

    } // close electron loop

    // For *this* systematic, store the global SF weight for the event
    //
    if ( m_debug ) {
       Info( "executeSF()", "--------------------------------------");
       Info( "executeSF()", "GLOBAL PID efficiency SF for event:");
       Info( "executeSF()", "\t %f ", pidEffSF_GLOBAL );
       Info( "executeSF()", "--------------------------------------");
    }
    sfVecPID_GLOBAL( *eventInfo ).push_back( pidEffSF_GLOBAL );

  }  // close loop on PID efficiency systematics


  // 2.
  // Reco efficiency SFs - this is a per-ELECTRON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Define also an *event* weight, which is the product of all the reco eff. SFs for each object in the event
  //
  std::string RECO_SF_NAME_GLOBAL = m_outputSystNamesReco + "_GLOBAL";
  SG::AuxElement::Decorator< std::vector<float> > sfVecReco_GLOBAL ( RECO_SF_NAME_GLOBAL );

  for ( const auto& syst_it : m_systListReco ) {

    // Initialise product of SFs for *this* systematic
    //
    float recoEffSF_GLOBAL(1.0);

    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_ElRecoEff_SF
    //
    std::string sfName  = "ElRecoEff_SF";

    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if(m_debug) Info("executeSF()", "Electron Reco efficiency SF decoration name is: %s", sfName.c_str());
    sysVariationNamesReco->push_back(sfName);

    // apply syst
    //
    if ( m_asgElEffCorrTool_elSF_Reco->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("executeSF()", "Failed to configure AsgElectronEfficiencyCorrectionTool_Reco for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if ( m_debug ) { Info("executeSF()", "Successfully applied systematics: %s ", m_asgElEffCorrTool_elSF_Reco->appliedSystematics().name().c_str() ); }

    // and now apply Reco efficiency SF!
    //
    unsigned int idx(0);
    for ( auto el_itr : *(inputElectrons) ) {

       if ( m_debug ) { Info( "executeSF()", "Applying Reco efficiency SF" ); }

       bool isBadElectron(false);

       //
       // obtain Reco efficiency SF as a float (to be stored away separately)
       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
       //
       SG::AuxElement::Decorator< std::vector<float> > sfVecReco ( m_outputSystNamesReco  );
       if ( !sfVecReco.isAvailable( *el_itr )  ) {
         sfVecReco ( *el_itr ) = std::vector<float>();
       }

       // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
       //
       if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, it has no caloCluster or trackParticle info", idx); }
	 isBadElectron = true;
       }
       //
       // skip electron if outside acceptance for SF calculation
       //
       if ( el_itr->pt() < 15e3 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside pT acceptance ( currently SF available for pT > 15 GeV )", idx); }
	 isBadElectron = true;
       }
       if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside |eta| acceptance", idx); }
	 isBadElectron = true;
       }

       //
       // obtain efficiency SF's for Reco
       //
       double recoEffSF(1.0); // tool wants a double
       if ( !isBadElectron && m_asgElEffCorrTool_elSF_Reco->getEfficiencyScaleFactor( *el_itr, recoEffSF ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in getEfficiencyScaleFactor");
	 recoEffSF = 1.0;
       }
       //
       // Add it to decoration vector
       //
       sfVecReco( *el_itr ).push_back( recoEffSF );

       recoEffSF_GLOBAL *= recoEffSF;

       if ( m_debug ) {
         Info( "executeSF()", "===>>>");
         Info( "executeSF()", " ");
	 Info( "executeSF()", "Electron %i, pt = %.2f GeV ", idx, (el_itr->pt() * 1e-3) );
	 Info( "executeSF()", " ");
         Info( "executeSF()", "Systematic: %s", syst_it.name().c_str() );
         Info( "executeSF()", " ");
         Info( "executeSF()", "Reco efficiency SF:");
         Info( "executeSF()", "\t %f (from getEfficiencyScaleFactor())", recoEffSF );
         Info( "executeSF()", "--------------------------------------");
       }

       ++idx;

    } // close electron loop

    // For *this* systematic, store the global SF weight for the event
    //
    if ( m_debug ) {
       Info( "executeSF()", "--------------------------------------");
       Info( "executeSF()", "GLOBAL Reco efficiency SF for event:");
       Info( "executeSF()", "\t %f ", recoEffSF_GLOBAL );
       Info( "executeSF()", "--------------------------------------");
    }
    sfVecReco_GLOBAL( *eventInfo ).push_back( recoEffSF_GLOBAL );

  }  // close loop on Reco efficiency systematics


  // 3.
  // Trig efficiency SFs - this is a per-EVENT weight
  //
  SG::AuxElement::Decorator< std::vector<float> > sfVecTrig ( m_outputSystNamesTrig  );

  // Loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //
  for ( const auto& syst_it : m_systListTrig ) {

    // Create the name of the SF weight to be Trigrded
    //   template:  SYSNAME_ElTrigEff_SF
    //
    std::string sfName  = "ElTrigEff_SF";

    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if(m_debug) Info("executeSF()", "Electron Trig efficiency SF decoration name is: %s", sfName.c_str());
    sysVariationNamesTrig->push_back(sfName);

    // apply syst
    //
    if ( m_asgElEffCorrTool_elSF_Trig->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("executeSF()", "Failed to configure AsgElectronEfficiencyCorrectionTool_Trig for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if ( m_debug ) { Info("executeSF()", "Successfully applied systematics: %s ", m_asgElEffCorrTool_elSF_Trig->appliedSystematics().name().c_str() ); }

    // and now apply trigger efficiency SF!
    //
    unsigned int idx(0);
    double trigSF(1.0); // tool wants a double
    for ( auto el_itr : *(inputElectrons) ) {

       // Retrieve the SF only from first electron (for any other selected electron, this info is just duplicated!)
       // This is a per-EVENT weight!
       // Do not increase counter until we find a "good" electron
       //
       if ( idx > 0 ) { break; }

       if ( m_debug ) { Info( "executeSF()", "Applying Trig efficiency SF" ); }

       bool isBadElectron(false);

       // NB: derivations might remove CC and tracks for low pt electrons: add a safety check!
       //
       if ( !( el_itr->caloCluster() && el_itr->trackParticle() ) ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, it has no caloCluster or trackParticle info", idx); }
	 isBadElectron = true;
       }
       //
       // skip electron if outside acceptance for SF calculation
       //
       if ( el_itr->pt() < 15e3 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside pT acceptance ( currently SF available for pT > 15 GeV )", idx); }
	 isBadElectron = true;
       }
       if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside |eta| acceptance", idx); }
	 isBadElectron = true;
       }

       //
       // obtain efficiency SF for Trig
       //
       if ( !isBadElectron && m_asgElEffCorrTool_elSF_Trig->getEfficiencyScaleFactor( *el_itr, trigSF ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in getEfficiencyScaleFactor");
	 isBadElectron = true;
	 trigSF = 1.0;
       }

       if ( m_debug ) {
         Info( "executeSF()", "===>>>");
         Info( "executeSF()", " ");
         Info( "executeSF()", "Systematic: %s", syst_it.name().c_str() );
         Info( "executeSF()", " ");
         Info( "executeSF()", "Trig efficiency SF:");
         Info( "executeSF()", "\t %f (from getEfficiencyScaleFactor())", trigSF );
         Info( "executeSF()", "--------------------------------------");
       }

       if ( !isBadElectron ) ++idx;

    } // close electron loop

    //
    // Add trigger SF to event decoration vector
    //
    sfVecTrig( *eventInfo ).push_back( trigSF );

  }  // close loop on Trig efficiency systematics


  //
  // add list of efficiency systematics names to TStore
  //
  // NB: we need to make sure that this is not pushed more than once in TStore!
  // This will be the case when this executeSF() function gets called for every syst varied input container,
  // e.g. the different SC containers w/ calibration systematics upstream.
  //
  // Use the counter defined in execute() to check this is done only once
  //
  // Also, make sure TStore does not contain the object yet (can happen if this module is instantiated more than once)
  //
  if ( countSyst == 0 ) {
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesPID) )  { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesPID, m_outputSystNamesPID ), "Failed to record vector of systematic names PID"  ); }
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesReco) ) { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesReco, m_outputSystNamesReco), "Failed to record vector of systematic names Reco" ); }
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesTrig) ) { RETURN_CHECK( "ElectronEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesTrig, m_outputSystNamesTrig), "Failed to record vector of systematic names Trig" ); }
  }

  return EL::StatusCode::SUCCESS;
}
