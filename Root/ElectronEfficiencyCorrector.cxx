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

  // read debug flag from .config file
  m_debug                   = false;

  // input container to be read from TEvent or TStore
  m_inContainerName         = "";

  // Systematics stuff
  m_inputAlgoSystNames      = "";
  m_systNamePID             = "";
  m_systNameReco            = "";
  m_systNameTrig            = "";
  m_outputSystNamesPID      = "ElectronEfficiencyCorrector_PIDSyst";
  m_outputSystNamesReco      = "ElectronEfficiencyCorrector_RecoSyst";
  m_outputSystNamesTrig     = "ElectronEfficiencyCorrector_TrigSyst";
  m_systValPID              = 0.;
  m_systValReco             = 0.;
  m_systValTrig             = 0.;

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
    m_systNamePID             = config->GetValue("SystNamePID"  , m_systNamePID.c_str());
    m_systNameReco            = config->GetValue("SystNameReco" , m_systNameReco.c_str());
    m_systNameTrig            = config->GetValue("SystNameTrig" , m_systNameTrig.c_str());
    m_outputSystNamesPID      = config->GetValue("OutputSystNamesPID" ,  m_outputSystNamesPID.c_str());
    m_outputSystNamesReco     = config->GetValue("OutputSystNamesReco",  m_outputSystNamesReco.c_str());
    m_outputSystNamesTrig     = config->GetValue("OutputSystNamesTrig",  m_outputSystNamesTrig.c_str());
    m_systValPID              = config->GetValue("SystValPID"  , m_systValPID);
    m_systValReco             = config->GetValue("SystValReco" , m_systValReco);
    m_systValTrig             = config->GetValue("SystValTrig" , m_systValTrig);
    m_runAllSystPID           = (m_systNamePID.find("All")  != std::string::npos);
    m_runAllSystReco          = (m_systNameReco.find("All") != std::string::npos);
    m_runAllSystTrig          = (m_systNameTrig.find("All") != std::string::npos);
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

  // initialize the ElectronEfficiencyCorrectionTool for PID Efficiency
  m_asgElEffCorrTool_elSF_PID = new AsgElectronEfficiencyCorrectionTool( "ElectronEfficiencyCorrectionTool_effSF_PID" );
  m_asgElEffCorrTool_elSF_PID->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  std::vector<std::string> inputFilesPID{ m_corrFileNamePID } ; // initialise vector w/ all the files containing corrections
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("CorrectionFileNameList",inputFilesPID),"Failed to set property CorrectionFileNameList");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_PID->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool PID");

  // Get a list of systematics
  const CP::SystematicSet recSystsPID = m_asgElEffCorrTool_elSF_PID->recommendedSystematics();
  Info("initialize()"," Initializing Electron Efficiency Corrector Systematics :");
  m_systListPID = HelperFunctions::getListofSystematics( recSystsPID, m_systNamePID, m_systValPID );
  m_systListPID = CP::make_systematics_vector(recSystsPID);   // Convert into a simple list

  if ( m_debug ) {
    for ( const auto& syst_it : m_systListPID ) {
      Info("initialize()"," PID tool available systematic: %s", (syst_it.name()).c_str());
    }
  }
  for ( const auto& syst_it : m_systListPID ) {
    Info("initialize()"," PID tool running with systematic : %s", (syst_it.name()).c_str());
  }

  /// Initialize the ElectronEfficiencyCorrectionTool for Reco Efficiency
  m_asgElEffCorrTool_elSF_Reco = new AsgElectronEfficiencyCorrectionTool( "ElectronEfficiencyCorrectionTool_effSF_Reco" );
  m_asgElEffCorrTool_elSF_Reco->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  std::vector<std::string> inputFilesReco{ m_corrFileNameReco } ; // initialise vector w/ all the files containing corrections
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->setProperty("CorrectionFileNameList",inputFilesReco),"Failed to set property CorrectionFileNameList");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Reco->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool Reco");

  // Get a list of systematics
  const CP::SystematicSet recSystsReco = m_asgElEffCorrTool_elSF_Reco->recommendedSystematics();
  Info("initialize()"," Initializing Electron Efficiency Corrector Systematics :");
  m_systListReco = HelperFunctions::getListofSystematics( recSystsReco, m_systNameReco, m_systValReco );
  // Convert into a simple list
  m_systListReco = CP::make_systematics_vector(recSystsReco);

  if ( m_debug ) {
    for ( const auto& syst_it : m_systListReco ) {
      Info("initialize()"," available systematic: %s", (syst_it.name()).c_str());
    }
  }
  for ( const auto& syst_it : m_systListReco ) {
    Info("initialize()"," Running with systematic : %s", (syst_it.name()).c_str());
  }

  /// Initialize the ElectronEfficiencyCorrectionTool for Trigger Efficiency
  m_asgElEffCorrTool_elSF_Trig = new AsgElectronEfficiencyCorrectionTool( "ElectronEfficiencyCorrectionTool_effSF_Trig" );
  m_asgElEffCorrTool_elSF_Trig->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  std::vector<std::string> inputFilesTrig{ m_corrFileNameTrig } ; // initialise vector w/ all the files containing corrections
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->setProperty("CorrectionFileNameList",inputFilesTrig),"Failed to set property CorrectionFileNameList");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElEffCorrTool_elSF_Trig->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool Trig");


  // Get a list of systematics
  const CP::SystematicSet recSystsTrig = m_asgElEffCorrTool_elSF_Trig->recommendedSystematics();
  Info("initialize()"," Initializing Electron Efficiency Corrector Systematics :");
  m_systListTrig = HelperFunctions::getListofSystematics( recSystsTrig, m_systNameTrig, m_systValTrig );
  // Convert into a simple list
  m_systListTrig = CP::make_systematics_vector(recSystsTrig);

  if ( m_debug ) {
    for ( const auto& syst_it : m_systListTrig ) {
      Info("initialize()"," available systematic: %s", (syst_it.name()).c_str());
    }
  }
  for ( const auto& syst_it : m_systListTrig ) {
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
      // if m_inputAlgo = NOT EMPTY --> you are retrieving syst varied containers from an upstream algo.
      // This is the case of calibrators: one different SC for each calibration syst applied

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
  } // close else

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

  if ( m_asgElEffCorrTool_elSF_Reco ) {
    delete m_asgElEffCorrTool_elSF_Reco; m_asgElEffCorrTool_elSF_Reco = nullptr;
  }

  if ( m_asgElEffCorrTool_elSF_Trig ) {
    delete m_asgElEffCorrTool_elSF_Trig; m_asgElEffCorrTool_elSF_Trig = nullptr;
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
  for ( const auto& syst_it : m_systListPID ) {

    //
    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_ElEff_SF
    //
    std::string sfNamePID  = "ElPIDEff_SF";
  
    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfNamePID.insert( 0, prepend );
    }
    if(m_debug) Info("execute()", "SF decoration names are: %s", sfNamePID.c_str());

    sysVariationNames->push_back(sfNamePID);
  
    // apply systs
    if ( m_asgElEffCorrTool_elSF_PID->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("initialize()", "Failed to configure AsgElectronEfficiencyCorrectionTool_PID for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }

    if ( m_debug ) { Info("execute()", "Successfully applied systematics: %s ", m_asgElEffCorrTool_elSF_PID->appliedSystematics().name().c_str() ); }

    // and now apply efficiency SF!
    unsigned int idx(0);
    for ( auto el_itr : *(inputElectrons) ) {

       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
       //
       SG::AuxElement::Decorator< std::vector<double> > sfVecPID ( m_outputSystNamesPID  );
       
       if ( !sfVecPID.isAvailable( *el_itr )  ){
          sfVecPID ( *el_itr ) = std::vector<double>();}

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
       // obtain efficiency SF's for PID, Reco, and Trig
       //
       double pidSF(0.0); 
       if (  m_asgElEffCorrTool_elSF_PID->getEfficiencyScaleFactor( *el_itr, pidSF  ) != CP::CorrectionCode::Ok ) {
         Error( "execute()", "Problem in getEfficiencyScaleFactor");
         return EL::StatusCode::FAILURE;
       }
       //
       // Add it to decoration vector
       //
       sfVecPID(  *el_itr ).push_back( pidSF );
  
       if ( m_debug )
          Info( "execute", "===>>> Resulting PID  SF: %f for systematic: %s ", pidSF , syst_it.name().c_str()); 
        

    } // close electron loop
  }  // close loop on systematics

  for ( const auto& syst_it : m_systListReco ) {

    //
    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_ElEff_SF
    //
    std::string sfNameReco = "ElRecoEff_SF";
    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfNameReco.insert( 0, prepend );
    }

    if(m_debug) Info("execute()", "SF decoration names are: %s", sfNameReco.c_str());

    sysVariationNames->push_back(sfNameReco);

    // apply systs
    if ( m_asgElEffCorrTool_elSF_Reco->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("initialize()", "Failed to configure AsgElectronEfficiencyCorrectionTool_Reco for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }

    if ( m_debug ) { Info("execute()", "Successfully applied systematics: %s", m_asgElEffCorrTool_elSF_Reco->appliedSystematics().name().c_str() ); }

    // and now apply efficiency SF!
    unsigned int idx(0);
    for ( auto el_itr : *(inputElectrons) ) {

       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
       //
       SG::AuxElement::Decorator< std::vector<double> > sfVecReco( m_outputSystNamesReco );

       if ( !sfVecReco.isAvailable( *el_itr ) ){
          sfVecReco( *el_itr ) = std::vector<double>();}

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
       // obtain efficiency SF's for PID, Reco, and Trig
       //
       double recoSF(0.0);
 
       if ( m_asgElEffCorrTool_elSF_Reco->getEfficiencyScaleFactor( *el_itr, recoSF ) != CP::CorrectionCode::Ok ) {
         Error( "execute()", "Problem in getEfficiencyScaleFactor");
         return EL::StatusCode::FAILURE;
       }

       //
       // Add it to decoration vector
       //
       sfVecReco( *el_itr ).push_back( recoSF );

       if ( m_debug ) Info( "execute", "===>>> Resulting Reco SF: %f for systematic: %s ", recoSF, syst_it.name().c_str()); 

    } // close electron loop

  }  // close loop on systematics

  for ( const auto& syst_it : m_systListTrig ) {

    //
    // Create the name of the SF weight to be recorded
    //   template:  SYSNAME_ElEff_SF
    //
    std::string sfNameTrig = "ElTrigEff_SF";
    if ( !syst_it.name().empty() ) {
       std::string prepend = syst_it.name() + "_";
       sfNameTrig.insert( 0, prepend );
    }
    if(m_debug) Info("execute()", "SF decoration names are: %s", sfNameTrig.c_str());

    sysVariationNames->push_back(sfNameTrig);

    // apply systs
    if ( m_asgElEffCorrTool_elSF_Trig->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      Error("initialize()", "Failed to configure AsgElectronEfficiencyCorrectionTool_Trig for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }

    if ( m_debug ) Info("execute()", "Successfully applied systematics: %s", m_asgElEffCorrTool_elSF_Trig->appliedSystematics().name().c_str() ); 

    // and now apply efficiency SF!
    unsigned int idx(0);
    for ( auto el_itr : *(inputElectrons) ) {

       //
       //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* electron)
       //
       SG::AuxElement::Decorator< std::vector<double> > sfVecTrig( m_outputSystNamesTrig );

       if ( !sfVecTrig.isAvailable( *el_itr ) ){
          sfVecTrig( *el_itr ) = std::vector<double>();}

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
       // obtain efficiency SF's for PID, Reco, and Trig
       //
       double trigSF(0.0);
       if ( m_asgElEffCorrTool_elSF_Trig->getEfficiencyScaleFactor( *el_itr, trigSF ) != CP::CorrectionCode::Ok ) {
         Error( "execute()", "Problem in getEfficiencyScaleFactor");
         return EL::StatusCode::FAILURE;
       }
       //
       // Add it to decoration vector
       //
       sfVecTrig( *el_itr ).push_back( trigSF );

       if ( m_debug ) Info( "execute", "===>>> Resulting Trig SF: %f for systematic: %s ", trigSF, syst_it.name().c_str()); 

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
  if ( countSyst == 0 ) { 
    RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( sysVariationNames, m_outputSystNamesPID ), "Failed to record vector of systematic names PID"  ); 
    RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( sysVariationNames, m_outputSystNamesReco), "Failed to record vector of systematic names Reco" );
    RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( sysVariationNames, m_outputSystNamesTrig), "Failed to record vector of systematic names Trig" ); 
  }

  return EL::StatusCode::SUCCESS;
}
