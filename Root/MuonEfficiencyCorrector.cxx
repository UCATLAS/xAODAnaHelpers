/**************************************************
 *
 * Interface to CP Muon Efficiency Correction Tool.
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *************************************************/

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

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/MuonEfficiencyCorrector.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(MuonEfficiencyCorrector)


MuonEfficiencyCorrector :: MuonEfficiencyCorrector (std::string className) :
    Algorithm(className),
    m_asgMuonEffCorrTool_muSF_Reco(nullptr),
    m_asgMuonEffCorrTool_muSF_Iso(nullptr),
    m_asgMuonEffCorrTool_muSF_Trig(nullptr),
    m_pileuptool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonEfficiencyCorrector()", "Calling constructor");

  m_debug                      = false;

  // Input container to be read from TEvent or TStore
  //
  m_inContainerName            = "";

  // Reco efficiency SF
  //
  m_WorkingPointReco           = "Loose";

  // Iso efficiency SF
  //
  m_WorkingPointIso            = "LooseTrackOnly";

  // Trigger efficiency SF
  //
  m_runNumber                  = 900000; // do NOT change this default value!
  m_WorkingPointRecoTrig       = "Loose";
  m_WorkingPointIsoTrig        = "LooseTrackOnly";
  m_SingleMuTrig               = "HLT_mu20_iloose_L1MU15";
  m_DiMuTrig                   = "HLT_2mu14";

  // Systematics stuff
  //
  m_inputAlgoSystNames         = "";
  m_systValReco 	       = 0.0;
  m_systValIso 	               = 0.0;
  m_systValTrig 	       = 0.0;
  m_systNameReco	       = "";
  m_systNameIso	               = "";
  m_systNameTrig	       = "";
  m_outputSystNamesReco        = "MuonEfficiencyCorrector_RecoSyst";
  m_outputSystNamesIso         = "MuonEfficiencyCorrector_IsoSyst";
  m_outputSystNamesTrig        = "MuonEfficiencyCorrector_TrigSyst";

}


EL::StatusCode  MuonEfficiencyCorrector :: configure ()
{

  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing MuonEfficiencyCorrector Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // Read debug flag from .config file
    //
    m_debug                      = config->GetValue("Debug", m_debug);

    // Input container to be read from TEvent or TStore
    //
    m_inContainerName            = config->GetValue("InputContainer",  m_inContainerName.c_str());

    // Reco efficiency SF
    //
    m_WorkingPointReco           = config->GetValue("WorkingPointReco", m_WorkingPointReco.c_str());

    // Iso efficiency SF
    //
    m_WorkingPointIso            = config->GetValue("WorkingPointIso", m_WorkingPointIso.c_str());

    // Trigger efficiency SF
    //
    m_runNumber                  = config->GetValue("RunNumber", m_runNumber);
    m_WorkingPointRecoTrig       = config->GetValue("WorkingPointRecoTrig", m_WorkingPointRecoTrig.c_str());
    m_WorkingPointIsoTrig        = config->GetValue("WorkingPointIsoTrig", m_WorkingPointIsoTrig.c_str());
    m_SingleMuTrig               = config->GetValue("SingleMuTrig", m_SingleMuTrig.c_str());
    m_DiMuTrig                   = config->GetValue("DiMuTrig", m_DiMuTrig.c_str());

    // Systematics stuff
    m_inputAlgoSystNames         = config->GetValue("InputAlgoSystNames",  m_inputAlgoSystNames.c_str());
    m_systValReco 		 = config->GetValue("SystValReco" , m_systValReco);
    m_systValIso 		 = config->GetValue("SystValIso" , m_systValIso);
    m_systValTrig 		 = config->GetValue("SystValTrig" , m_systValTrig);
    m_systNameReco	         = config->GetValue("SystNameReco" , m_systNameReco.c_str());
    m_systNameIso	         = config->GetValue("SystNameIso" , m_systNameIso.c_str());
    m_systNameTrig		 = config->GetValue("SystNameTrig" , m_systNameTrig.c_str());
    m_outputSystNamesReco        = config->GetValue("OutputSystNamesReco", m_outputSystNamesReco.c_str());
    m_outputSystNamesIso         = config->GetValue("OutputSystNamesIso",  m_outputSystNamesIso.c_str());
    m_outputSystNamesTrig        = config->GetValue("OutputSystNamesTrig", m_outputSystNamesTrig.c_str());

    config->Print();

    Info("configure()", "MuonEfficiencyCorrector Interface succesfully configured! ");

    delete config; config = nullptr;
  }

  if ( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonEfficiencyCorrector :: setupJob (EL::Job& job)
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
  xAOD::Init( "MuonEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing MuonEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonEfficiencyCorrector::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  m_numEvent      = 0;
  m_numObject     = 0;


  // 1.
  // initialize the CP::MuonEfficiencyScaleFactors Tool for reco efficiency SF
  //

  if ( asg::ToolStore::contains<CP::MuonEfficiencyScaleFactors>("MuonEfficiencyScaleFactors_effSF_Reco") ) {
    m_asgMuonEffCorrTool_muSF_Reco = asg::ToolStore::get<CP::MuonEfficiencyScaleFactors>("MuonEfficiencyScaleFactors_effSF_Reco");
  } else {
    m_asgMuonEffCorrTool_muSF_Reco = new CP::MuonEfficiencyScaleFactors("MuonEfficiencyScaleFactors_effSF_Reco");
    RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Reco->setProperty("WorkingPoint", m_WorkingPointReco ),"Failed to set Working Point property of MuonEfficiencyScaleFactors for reco efficiency SF");
    RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Reco->setProperty("doAudit", false),"Failed to set doAudit property of MuonEfficiencyScaleFactors");
    RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Reco->initialize(), "Failed to properly initialize MuonEfficiencyScaleFactors for reco efficiency SF");
  }

  if ( m_debug ) {

    // Get a list of affecting systematics
    //
    CP::SystematicSet affectSystsReco = m_asgMuonEffCorrTool_muSF_Reco->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsReco ) { Info("initialize()","MuonEfficiencyScaleFactors tool can be affected by reco efficiency systematic: %s", (syst_it.name()).c_str()); }
  }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsReco = m_asgMuonEffCorrTool_muSF_Reco->recommendedSystematics();
  m_systListReco = HelperFunctions::getListofSystematics( recSystsReco, m_systNameReco, m_systValReco, m_debug );

  Info("initialize()","Will be using MuonEfficiencyScaleFactors tool reco efficiency systematic:");
  for ( const auto& syst_it : m_systListReco ) {
    if ( m_systNameReco.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  // 2.
  // initialize the CP::MuonEfficiencyScaleFactors Tool for isolation efficiency SF
  //

  //
  // Add an "Iso" suffix to the WP (required for tool configuration)
  //
  std::string tool_WP = m_WorkingPointIso + "Iso";

  std::string isoEffSF_tool_name = "MuonEfficiencyScaleFactors_effSF_Iso_" + m_WorkingPointIso;
  m_asgMuonEffCorrTool_muSF_Iso = new CP::MuonEfficiencyScaleFactors(isoEffSF_tool_name);
  RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Iso->setProperty("WorkingPoint", tool_WP ), "Failed to set Working Point property of MuonEfficiencyScaleFactors for iso efficiency SF");
  RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Iso->initialize(), "Failed to properly initialize MuonEfficiencyScaleFactors for iso efficiency SF");

  //
  //  Add the chosen WP to the string labelling the vector<SF> decoration
  //
  m_outputSystNamesIso = m_outputSystNamesIso + "_" + m_WorkingPointIso;

  if ( m_debug ) {

    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsIso = m_asgMuonEffCorrTool_muSF_Iso->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsIso ) { Info("initialize()","MuonEfficiencyScaleFactors tool can be affected by iso efficiency systematic: %s", (syst_it.name()).c_str()); }
  }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsIso = m_asgMuonEffCorrTool_muSF_Iso->recommendedSystematics();
  m_systListIso = HelperFunctions::getListofSystematics( recSystsIso, m_systNameIso, m_systValIso, m_debug );

  Info("initialize()","Will be using MuonEfficiencyScaleFactors tool iso efficiency systematic:");
  for ( const auto& syst_it : m_systListIso ) {
    if ( m_systNameIso.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  // 3.
  // Initialise the CP::MuonTriggerScaleFactors tool
  //

  if ( asg::ToolStore::contains<CP::MuonTriggerScaleFactors>("MuonTriggerScaleFactors_effSF_Trig") ) {
    m_asgMuonEffCorrTool_muSF_Trig = asg::ToolStore::get<CP::MuonTriggerScaleFactors>("MuonTriggerScaleFactors_effSF_Trig");
  } else {
    m_asgMuonEffCorrTool_muSF_Trig = new CP::MuonTriggerScaleFactors("MuonTriggerScaleFactors_effSF_Trig");

    int runNumber(m_runNumber);
    if ( asg::ToolStore::contains<CP::PileupReweightingTool>("Pileup") ) {
      m_pileuptool = asg::ToolStore::get<CP::PileupReweightingTool>("Pileup");
    }
    //
    // If PileupReweightingTool exists, and a specific runNumber hasn't been set by the user yet,
    // use the random runNumber weighted by integrated luminosity got from CP::PileupReweightingTool::getRandomRunNumber()
    // Source: // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ExtendedPileupReweighting#Generating_PRW_config_files
    //
    if ( m_runNumber == 900000 && m_pileuptool ) {
      runNumber = m_pileuptool->getRandomRunNumber( *eventInfo, false );
      Info("initialize()","CP::MuonTriggerScaleFactors - setting runNumber %i read from CP::PileupReweightingTool::getRandomRunNumber()", runNumber);
    } else {
      Warning("initialize()","CP::MuonTriggerScaleFactors - setting runNumber %i read from user's configuration - NOT RECOMMENDED", runNumber );
    }
    if( m_asgMuonEffCorrTool_muSF_Trig->setRunNumber( runNumber ) == CP::CorrectionCode::Error ) {
      Warning("initialize()","Cannot set RunNumber for MuonTriggerScaleFactors tool");
    }
    //
    // Add an "Iso" prefix to the WP (required for tool configuration)
    //
    std::string iso_trig_WP = "Iso" + m_WorkingPointIsoTrig;
    RETURN_CHECK("MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Trig->setProperty("Isolation", iso_trig_WP ),"Failed to set Isolation property of MuonTriggerScaleFactors");
    RETURN_CHECK("MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Trig->setProperty("MuonQuality", m_WorkingPointRecoTrig ),"Failed to set MuonQuality property of MuonTriggerScaleFactors");
    RETURN_CHECK("MuonEfficiencyCorrector::initialize()", m_asgMuonEffCorrTool_muSF_Trig->initialize(), "Failed to properly initialize MuonTriggerScaleFactors");
  }

  if ( m_debug ) {

    // Get a list of affecting systematics
   //
    CP::SystematicSet affectSystsTrig = m_asgMuonEffCorrTool_muSF_Trig->affectingSystematics();
    //
    // Convert into a simple list
    //
    for ( const auto& syst_it : affectSystsTrig ) { Info("initialize()","MuonEfficiencyScaleFactors tool can be affected by trigger efficiency systematic: %s", (syst_it.name()).c_str()); }
  }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsTrig = m_asgMuonEffCorrTool_muSF_Trig->recommendedSystematics();
  m_systListTrig = HelperFunctions::getListofSystematics( recSystsTrig, m_systNameTrig, m_systValTrig, m_debug );

  Info("initialize()","Will be using MuonEfficiencyScaleFactors tool trigger efficiency systematic:");
  for ( const auto& syst_it : m_systListTrig ) {
    if ( m_systNameTrig.empty() ) {
      Info("initialize()","\t Running w/ nominal configuration only!");
      break;
    }
    Info("initialize()","\t %s", (syst_it.name()).c_str());
  }

  // *********************************************************************************

  Info("initialize()", "MuonEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  m_numEvent++;

  if ( !m_isMC ) {
    if ( m_numEvent == 1 ) { Info("execute()", "Sample is Data! Do not apply any Muon Efficiency correction... "); }
    return EL::StatusCode::SUCCESS;
  }

  if ( m_debug ) { Info("execute()", "Applying Muon Efficiency corrections... "); }


  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MuonEfficiencyCorrector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  // initialise containers
  //
  const xAOD::MuonContainer* inputMuons(nullptr);

  // if m_inputAlgoSystNames = "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it

  // Declare a counter, initialised to 0
  // For the systematically varied input containers, we won't store again the vector with efficiency systs in TStore ( it will be always the same!)
  //
  unsigned int countInputCont(0);

  if ( m_inputAlgoSystNames.empty() ) {

    RETURN_CHECK("MuonEfficiencyCorrector::execute()", HelperFunctions::retrieve(inputMuons, m_inContainerName, m_event, m_store, m_verbose) ,"");

    // decorate muons w/ SF - there will be a decoration w/ different name for each syst!
    //
    this->executeSF( inputMuons, eventInfo, countInputCont );

  } else {
  // if m_inputAlgo = NOT EMPTY --> you are retrieving syst varied containers from an upstream algo. This is the case of calibrators: one different SC
  // for each calibration syst applied

	// get vector of string giving the syst names of the upstream algo m_inputAlgo (rememeber: 1st element is a blank string: nominal case!)
	//
        std::vector<std::string>* systNames(nullptr);
        RETURN_CHECK("MuonEfficiencyCorrector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

    	// loop over systematic sets available
	//
    	for ( auto systName : *systNames ) {

           RETURN_CHECK("MuonEfficiencyCorrector::execute()", HelperFunctions::retrieve(inputMuons, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");

    	   if ( m_debug ){
    	     unsigned int idx(0);
    	     for ( auto mu : *(inputMuons) ) {
    	       Info( "execute", "Input muon %i, pt = %.2f GeV ", idx, (mu->pt() * 1e-3) );
    	       ++idx;
    	     }
    	   }

	   // decorate muons w/ SF - there will be a decoration w/ different name for each syst!
	   //
	   this->executeSF( inputMuons, eventInfo, countInputCont );

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


EL::StatusCode MuonEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonEfficiencyCorrector :: finalize ()
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

  if ( m_asgMuonEffCorrTool_muSF_Reco )  { m_asgMuonEffCorrTool_muSF_Reco = nullptr; delete m_asgMuonEffCorrTool_muSF_Reco; }
  if ( m_asgMuonEffCorrTool_muSF_Iso )   { m_asgMuonEffCorrTool_muSF_Iso = nullptr;  delete m_asgMuonEffCorrTool_muSF_Iso;  }
  if ( m_asgMuonEffCorrTool_muSF_Trig )  { m_asgMuonEffCorrTool_muSF_Trig = nullptr; delete m_asgMuonEffCorrTool_muSF_Trig; }
  if ( m_pileuptool )                    { m_pileuptool = nullptr;                   delete m_pileuptool; }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonEfficiencyCorrector :: histFinalize ()
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

EL::StatusCode MuonEfficiencyCorrector :: executeSF (  const xAOD::MuonContainer* inputMuons, const xAOD::EventInfo* eventInfo, unsigned int countSyst  )
{

  //
  // In the following, every muon gets decorated with 2 vector<double>'s (for reco/iso efficiency SFs),
  // and the event w/ 1 vector<double> (for trigger efficiency SFs)
  // Each vector contains the SFs, one SF for each syst (first component of each vector will be the nominal SF).
  //
  // Additionally, we create these vector<string> with the SF syst names, so that we know which component corresponds to.
  // ( there's a 1:1 correspondence with the vector<double> defined above )
  //
  // These vector<string> are eventually stored in TStore
  //
  std::vector< std::string >* sysVariationNamesReco  = new std::vector< std::string >;
  std::vector< std::string >* sysVariationNamesIso   = new std::vector< std::string >;
  std::vector< std::string >* sysVariationNamesTrig  = new std::vector< std::string >;

  // 1.
  // Reco efficiency SFs - this is a per-MUON weight
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
    //   template:  SYSNAME_MuRecoEff_SF
    //
    std::string sfName = "MuRecoEff_SF";
    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if ( m_debug ) { Info("executeSF()", "Muon reco efficiency SF decoration name is: %s", sfName.c_str()); }
    sysVariationNamesReco->push_back(sfName);

    // apply syst
    //
    if ( m_asgMuonEffCorrTool_muSF_Reco->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("executeSF()", "Failed to configure MuonEfficiencyScaleFactors for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if ( m_debug ) { Info("executeSF()", "Successfully applied systematic: %s", syst_it.name().c_str()); }

    // and now apply reco efficiency SF!
    //
    unsigned int idx(0);
    for ( auto mu_itr : *(inputMuons) ) {

       if ( m_debug ) { Info( "executeSF()", "Applying reco efficiency SF" ); }

       // a)
       // decorate directly the muon with reco efficiency (useful at all?), and the corresponding SF
       //
       if ( m_asgMuonEffCorrTool_muSF_Reco->applyRecoEfficiency( *mu_itr ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in applyRecoEfficiency");
       }
       if ( m_asgMuonEffCorrTool_muSF_Reco->applyEfficiencyScaleFactor( *mu_itr ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in applyEfficiencyScaleFactor");
       }

       // b)
       // obtain reco efficiency SF as a float (to be stored away separately)
       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* muon)
       //
       SG::AuxElement::Decorator< std::vector<float> > sfVecReco( m_outputSystNamesReco );
       if ( !sfVecReco.isAvailable( *mu_itr ) ) {
	 sfVecReco( *mu_itr ) = std::vector<float>();
       }

       float recoEffSF(1.0);
       if ( m_asgMuonEffCorrTool_muSF_Reco->getEfficiencyScaleFactor( *mu_itr, recoEffSF ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in getEfficiencyScaleFactor");
         recoEffSF = 1.0;
       }
       //
       // Add it to decoration vector
       //
       sfVecReco( *mu_itr ).push_back( recoEffSF );

       recoEffSF_GLOBAL *= recoEffSF;

       if ( m_debug ) {
         Info( "executeSF()", "===>>>");
         Info( "executeSF()", " ");
	 Info( "executeSF()", "Muon %i, pt = %.2f GeV ", idx, (mu_itr->pt() * 1e-3) );
	 Info( "executeSF()", " ");
         Info( "executeSF()", "Systematic: %s", syst_it.name().c_str() );
         Info( "executeSF()", " ");
         Info( "executeSF()", "Reco efficiency:");
         Info( "executeSF()", "\t %f (from applyRecoEfficiency())", mu_itr->auxdataConst< float >( "Efficiency" ) );
         Info( "executeSF()", "and its SF:");
         Info( "executeSF()", "\t %f (from applyEfficiencyScaleFactor())", mu_itr->auxdataConst< float >( "EfficiencyScaleFactor" ) );
         Info( "executeSF()", "\t %f (from getEfficiencyScaleFactor())", recoEffSF );
         Info( "executeSF()", "--------------------------------------");
       }

       ++idx;

    } // close muon loop

    // For *this* systematic, store the global SF weight for the event
    //
    if ( m_debug ) {
       Info( "executeSF()", "--------------------------------------");
       Info( "executeSF()", "GLOBAL Reco efficiency SF for event:");
       Info( "executeSF()", "\t %f ", recoEffSF_GLOBAL );
       Info( "executeSF()", "--------------------------------------");
    }
    sfVecReco_GLOBAL( *eventInfo ).push_back( recoEffSF_GLOBAL );

  }  // close loop on reco efficiency SF systematics

  // 2.
  // Isolation efficiency SFs - this is a per-MUON weight
  //
  // Firstly, loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //

  // Define also an *event* weight, which is the product of all the iso eff. SFs for each object in the event
  //
  std::string ISO_SF_NAME_GLOBAL = m_outputSystNamesIso + "_GLOBAL";
  SG::AuxElement::Decorator< std::vector<float> > sfVecIso_GLOBAL ( ISO_SF_NAME_GLOBAL );

  for ( const auto& syst_it : m_systListIso ) {

    // Initialise product of SFs for *this* systematic
    //
    float isoEffSF_GLOBAL(1.0);

    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_MuIsoEff_SF_WP
    //
    std::string sfName = "MuIsoEff_SF_" + m_WorkingPointIso;
    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if ( m_debug ) { Info("executeSF()", "Muon iso efficiency SF decoration name is: %s", sfName.c_str()); }
    sysVariationNamesIso->push_back(sfName);

    // apply syst
    //
    if ( m_asgMuonEffCorrTool_muSF_Iso->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("executeSF()", "Failed to configure MuonEfficiencyScaleFactors for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if ( m_debug ) { Info("executeSF()", "Successfully applied systematic: %s", syst_it.name().c_str()); }

    // and now apply Iso efficiency SF!
    //
    unsigned int idx(0);
    for ( auto mu_itr : *(inputMuons) ) {

       if ( m_debug ) { Info( "executeSF()", "Applying iso efficiency SF" ); }

       // a)
       // decorate directly the muon with iso efficiency (useful at all?), and the corresponding SF
       //
       if ( m_asgMuonEffCorrTool_muSF_Iso->applyRecoEfficiency( *mu_itr ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in applyIsoEfficiency");
       }
       if ( m_asgMuonEffCorrTool_muSF_Iso->applyEfficiencyScaleFactor( *mu_itr ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in applyEfficiencyScaleFactor");
       }

       // b)
       // obtain iso efficiency SF as a float (to be stored away separately)
       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* muon)
       //
       SG::AuxElement::Decorator< std::vector<float> > sfVecIso( m_outputSystNamesIso );
       if ( !sfVecIso.isAvailable( *mu_itr ) ) {
	 sfVecIso( *mu_itr ) = std::vector<float>();
       }

       float IsoEffSF(1.0);
       if ( m_asgMuonEffCorrTool_muSF_Iso->getEfficiencyScaleFactor( *mu_itr, IsoEffSF ) != CP::CorrectionCode::Ok ) {
         Warning( "executeSF()", "Problem in getEfficiencyScaleFactor");
	 IsoEffSF = 1.0;
       }
       //
       // Add it to decoration vector
       //
       sfVecIso( *mu_itr ).push_back(IsoEffSF);

       isoEffSF_GLOBAL *= IsoEffSF;

       if ( m_debug ) {
         Info( "executeSF()", "===>>>");
         Info( "executeSF()", " ");
	 Info( "executeSF()", "Muon %i, pt = %.2f GeV ", idx, (mu_itr->pt() * 1e-3) );
	 Info( "executeSF()", " ");
	 Info( "executeSF()", "Isolation SF decoration: %s", m_outputSystNamesIso.c_str() );
	 Info( "executeSF()", " ");
         Info( "executeSF()", "Systematic: %s", syst_it.name().c_str() );
         Info( "executeSF()", " ");
         Info( "executeSF()", "Iso efficiency:");
         Info( "executeSF()", "\t %f (from applyIsoEfficiency())", mu_itr->auxdataConst< float >( "ISOEfficiency" ) );
         Info( "executeSF()", "and its SF:");
         Info( "executeSF()", "\t %f (from applyEfficiencyScaleFactor())", mu_itr->auxdataConst< float >( "ISOEfficiencyScaleFactor" ) );
         Info( "executeSF()", "\t %f (from getEfficiencyScaleFactor())", IsoEffSF );
         Info( "executeSF()", "--------------------------------------");
       }

       ++idx;

    } // close muon loop

    // For *this* systematic, store the global SF weight for the event
    //
    if ( m_debug ) {
       Info( "executeSF()", "--------------------------------------");
       Info( "executeSF()", "GLOBAL Iso efficiency SF for event:");
       Info( "executeSF()", "\t %f ", isoEffSF_GLOBAL );
       Info( "executeSF()", "--------------------------------------");
    }
    sfVecIso_GLOBAL( *eventInfo ).push_back( isoEffSF_GLOBAL );

  }  // close loop on isolation efficiency SF systematics

  // 3.
  // Trigger efficiency SF - this is a per-EVENT weight
  //
  SG::AuxElement::Decorator< std::vector<float> > sfVecTrig( m_outputSystNamesTrig );

  // Loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  // Every systematic will correspond to a different SF!
  //
  for ( const auto& syst_it : m_systListTrig ) {

    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_MuTrigEff_SF
    //
    std::string sfName = "MuTrigEff_SF";
    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if ( m_debug ) { Info("executeSF()", "Trigger efficiency SF decoration name is: %s", sfName.c_str()); }
    sysVariationNamesTrig->push_back(sfName);

    // apply syst
    //
    if ( m_asgMuonEffCorrTool_muSF_Trig->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("executeSF()", "Failed to configure MuonTriggerScaleFactors for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if ( m_debug ) { Info("executeSF()", "Successfully applied systematic: %s", syst_it.name().c_str()); }

    // and now apply trigger efficiency SF!
    //
    unsigned int nMuons = inputMuons->size();
    if ( m_debug ) { Info( "executeSF", "Applying trigger efficiency SF: \n Number of muons : %u", nMuons); }

    // obtain trigger efficiency SF
    //
    double triggerEffSF(1.0); // tool wants a double
    if ( nMuons > 0 ) {
      if ( !m_SingleMuTrig.empty() ) {
	if ( m_asgMuonEffCorrTool_muSF_Trig->getTriggerScaleFactor( *inputMuons, triggerEffSF, m_SingleMuTrig ) != CP::CorrectionCode::Ok ) {
	  Warning( "executeSF()", "Problem in getTriggerScaleFactor - single muon trigger(s)");
	  triggerEffSF = 1.0;
	}
        if ( m_debug ) {
          Info( "executeSF()", "===>>>");
          Info( "executeSF()", " ");
          Info( "executeSF()", "Systematic: %s", syst_it.name().c_str() );
          Info( "executeSF()", " ");
          Info( "executeSF()", "Trigger efficiency SF:");
          Info( "executeSF()", "\t %f (from getTriggerScaleFactor())", triggerEffSF );
          Info( "executeSF()", "--------------------------------------");
        }
      }
      /*
      if ( nMuons == 2 ) {
        if ( !m_DiMuTrig.empty() ) {
          if ( m_asgMuonEffCorrTool_muSF_Trig->getTriggerScaleFactor( *inputMuons, triggerEffSF, m_DiMuTrig ) != CP::CorrectionCode::Ok ) {
	    Warning( "executeSF()", "Problem in getTriggerScaleFactor - dimuon trigger(s)");
	    triggerEffSF = 1.0;
          }
          if ( m_debug ) {
            Info( "executeSF()", "===>>>");
            Info( "executeSF()", " ");
            Info( "executeSF()", "Systematic: %s", syst_it.name().c_str() );
            Info( "executeSF()", " ");
            Info( "executeSF()", "Trigger efficiency SF:");
            Info( "executeSF()", "\t %f (from getTriggerScaleFactor())", triggerEffSF );
            Info( "executeSF()", "--------------------------------------");
          }
        }
      }
      */
    }
    //
    // Add trigger SF to event decoration vector
    //
    sfVecTrig( *eventInfo ).push_back( triggerEffSF );

  }  // close loop on trigger efficiency SF systematics

  //
  // add list of reco/iso/trigger efficiency SF systematics names to TStore
  //
  // NB: we need to make sure that this is not pushed more than once in TStore!
  // This will be the case when this executeSF() function gets called for every syst varied input container,
  // e.g. the different SC containers w/ calibration systematics upstream.
  //
  // Use the counter defined in execute() to check this is done only once
  //
  if ( countSyst == 0 ) {
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesReco) ) { RETURN_CHECK( "MuonEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesReco, m_outputSystNamesReco), "Failed to record vector of systematic names for muon reco efficiency SF" ); }
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesIso) )  { RETURN_CHECK( "MuonEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesIso, m_outputSystNamesIso),   "Failed to record vector of systematic names for muon iso efficiency SF" ); }
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesTrig) ) { RETURN_CHECK( "MuonEfficiencyCorrector::executeSF()", m_store->record( sysVariationNamesTrig, m_outputSystNamesTrig), "Failed to record vector of systematic names for muon trigger efficiency  SF" ); }
  }

  return EL::StatusCode::SUCCESS;
}
