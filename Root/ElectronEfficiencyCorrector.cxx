/*****************************************************
 *
 * Interface to CP Electron Efficiency Correction Tool.
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *
 *****************************************************/

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


ElectronEfficiencyCorrector :: ElectronEfficiencyCorrector () :
  m_asgElEffCorrTool_elSF_PID(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronEfficiencyCorrector()", "Calling constructor");

  // read debug flag from .config file
  m_debug                   = false;

  // input container to be read from TEvent or TStore
  m_inContainerName         = "";

  // Systematics stuff
  m_inputAlgoSystNames      = "";
  m_systName		    = "";
  m_outputSystNames         = "ElectronEfficiencyCorrector_Syst";
  m_systVal 		    = 0.;

  // file(s) containing corrections
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
    m_debug                   = config->GetValue("Debug", m_debug);

    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());

    // Systematics stuff
    m_inputAlgoSystNames      = config->GetValue("InputAlgoSystNames",  m_inputAlgoSystNames.c_str());
    m_systName		      = config->GetValue("SystName" , m_systName.c_str());
    m_outputSystNames         = config->GetValue("OutputSystNames",  m_outputSystNames.c_str());
    m_systVal 		      = config->GetValue("SystVal" , m_systVal);
    m_runAllSyst              = (m_systName.find("All") != std::string::npos);
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

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the ElectronEfficiencyCorrectionTool
  m_asgElEffCorrTool_elSF_PID = new AsgElectronEfficiencyCorrectionTool( "ElectronEfficiencyCorrectionTool_effSF_PID" );
  m_asgElEffCorrTool_elSF_PID->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  std::vector<std::string> inputFiles{ m_corrFileNamePID } ; // initialise vector w/ all the files containing corrections
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("CorrectionFileNameList",inputFiles),"Failed to set property CorrectionFileNameList");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool");


  // Get a list of systematics
  const CP::SystematicSet recSysts = m_asgElEffCorrTool_elSF_PID->recommendedSystematics();
  Info("initialize()"," Initializing Electron Efficiency Corrector Systematics :");
  m_systList = HelperFunctions::getListofSystematics( recSysts, m_systName, m_systVal );
  // Convert into a simple list
  m_systList = CP::make_systematics_vector(recSysts);

  if ( m_debug ) {
    for ( const auto& syst_it : m_systList ) {
      Info("initialize()"," available systematic: %s", (syst_it.name()).c_str());
    }
  }
  for ( const auto& syst_it : m_systList ) {
    Info("initialize()"," Running with systematic : %s", (syst_it.name()).c_str());
  }

  Info("initialize()", "ElectronEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Electron Efficiency Correction... "); }

  m_numEvent++;

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  bool isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );
  if ( !isMC ) {
    if ( m_debug ) { Info("execute()", "Event is Data! Do not apply Electron Efficiency Correction... "); }
    return EL::StatusCode::SUCCESS;
  }

  // initialise containers
  const xAOD::ElectronContainer* inputElectrons(nullptr);

  // if m_inputAlgoSystNames = "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it

  // Declare a counter, initialised to 0
  // For the systematically varied input containers, we won't store again the vector with efficiency systs in TStore ( it will be always the same!)
  unsigned int countInputCont(0);

  if ( m_inputAlgoSystNames.empty() ) {

        RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(inputElectrons, m_inContainerName, m_event, m_store, m_verbose) ,"");

	// decorate electrons w/ SF - there will be a decoration w/ different name for each syst!
	this->executeSF( inputElectrons, countInputCont );

  } else {
  // if m_inputAlgo = NOT EMPTY --> you are retrieving syst varied containers from an upstream algo. This is the case of calibrators: one different SC
  // for each calibration syst applied

	// get vector of string giving the syst names of the upstream algo m_inputAlgo (rememeber: 1st element is a blank string: nominal case!)
        std::vector<std::string>* systNames(nullptr);
        RETURN_CHECK("ElectronEfficiencyCorrector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

    	// loop over systematic sets available
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
	   this->executeSF( inputElectrons, countInputCont );

	   // increment counter
	   ++countInputCont;

    	} // close loop on systematic sets available from upstream algo

   }

  // look what do we have in TStore
  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute");

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

  if ( m_asgElEffCorrTool_elSF_PID ) {
    delete m_asgElEffCorrTool_elSF_PID; m_asgElEffCorrTool_elSF_PID = nullptr;
  }

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

EL::StatusCode ElectronEfficiencyCorrector :: executeSF (  const xAOD::ElectronContainer* inputElectrons, unsigned int countSyst  )
{

  //
  // Every electron gets decorated with a vector<double> of SFs, one for each syst.
  // We create this vector<string> with the SF syst names, so that we know which component corresponds to.
  // This vector is eventually stored in TStore
  //
  std::vector< std::string >* sysVariationNames = new std::vector< std::string >;

  // loop over available systematics for this tool - remember: syst == EMPTY_STRING --> nominal
  for ( const auto& syst_it : m_systList ) {

    //
    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_ElEff_SF
    //
    std::string sfName = "ElEff_SF";
    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if(m_debug) Info("execute()", "SF decoration name is: %s", sfName.c_str());
    sysVariationNames->push_back(sfName);

    // apply syst
    if ( m_asgElEffCorrTool_elSF_PID->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("initialize()", "Failed to configure AsgElectronEfficiencyCorrectionTool for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if ( m_debug ) { Info("execute()", "Successfully applied systematic: %s", m_asgElEffCorrTool_elSF_PID->appliedSystematics().name().c_str()); }

    // and now apply efficiency SF!
    unsigned int idx(0);
    for ( auto el_itr : *(inputElectrons) ) {

       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
       //
       SG::AuxElement::Decorator< std::vector<double> > sfVec( m_outputSystNames );
       if ( !sfVec.isAvailable( *el_itr ) ) {
	 sfVec( *el_itr ) = std::vector<double>();
       }

       if ( m_debug ) { Info( "execute", "Checking electron %i, pt = %.2f GeV ", idx, (el_itr->pt() * 1e-3) ); }
       ++idx;

       // NB: derivations might remove CC and tracks for low pt electrons
       if ( !(el_itr->caloCluster() && el_itr->trackParticle()) ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, it has no caloCluster or trackParticle info", idx); }
         continue;
       }

       // skip electron if outside acceptance for SF calculation
       if ( el_itr->pt() < 15e3 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside pT acceptance ( currently > 15 GeV )", idx); }
         continue;
       }
       if ( fabs( el_itr->caloCluster()->eta() ) > 2.47 ) {
         if ( m_debug ) { Info( "execute", "Apply SF: skipping electron %i, is outside |eta| acceptance", idx); }
         continue;
       }

       //
       // obtain efficiency SF
       //
       double SF(0.0);
       if ( m_asgElEffCorrTool_elSF_PID->getEfficiencyScaleFactor( *el_itr, SF ) != CP::CorrectionCode::Ok ) {
         Error( "execute()", "Problem in getEfficiencyScaleFactor");
         return EL::StatusCode::FAILURE;
       }
       //
       // Add it to decoration vector
       //
       sfVec( *el_itr ).push_back(SF);

       if ( m_debug ) { Info( "execute", "===>>> Resulting SF: %f for systematic: %s ", SF, syst_it.name().c_str()); }

    } // close electron loop

  }  // close loop on systematics

  //
  // add list of efficiency systematics names to TStore
  //
  // NB: we need to make sure that this is not pushed more than once in TStore!
  // This will be the case when this executeSF() function gets called for every syst varied input container,
  // e.g. the different SC containers w/ calibration systematics upstream.
  //
  // Use the counter defined in execute() to check this is done only once
  //
  if ( countSyst == 0 ) { RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( sysVariationNames, m_outputSystNames), "Failed to record vector of systematic names" ); }

  return EL::StatusCode::SUCCESS;
}
