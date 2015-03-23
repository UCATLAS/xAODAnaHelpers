/******************************************
 *
 * Interface to CP Electron Efficiency Correction Tool.
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *
 ******************************************/

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
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):
#include "ElectronEfficiencyCorrection/AsgElectronEfficiencyCorrectionTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronEfficiencyCorrector)


ElectronEfficiencyCorrector :: ElectronEfficiencyCorrector () {
}

ElectronEfficiencyCorrector :: ElectronEfficiencyCorrector (std::string name, std::string configName ) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_asgElectronEfficiencyCorrectionTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronEfficiencyCorrector()", "Calling constructor \n");

}


EL::StatusCode  ElectronEfficiencyCorrector :: configure ()
{
  Info("configure()", "Configuing ElectronEfficiencyCorrector Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "ElectronEfficiencyCorrector::configure()", m_configName);

  TEnv* config = new TEnv(m_configName.c_str());

  // read debug flag from .config file
  m_debug                   = config->GetValue("Debug" , false );
  // input container to be read from TEvent or TStore
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_outContainerName        = config->GetValue("OutputContainer", "");
  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

  // Systematics stuff
  m_runAllSyst              = config->GetValue("RunAllSyst" , false ); // default: false
  m_systName		    = config->GetValue("SystName" , "" );      // default: no syst
  m_systSigma 		    = config->GetValue("SystSigma" , 0. );
  // file(s) containing corrections
  m_corrFileName1           = config->GetValue("CorrectionFileName1" , "" );
  //m_corrFileName2         = config->GetValue("CorrectionFileName2" , "" );


  if( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "ElectronEfficiencyCorrector Interface succesfully configured! \n");

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

  Info("setupJob()", "Calling setupJob \n");

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

  Info("initialize()", "Initializing ElectronEfficiencyCorrector Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the ElectronEfficiencyCorrectionTool
  std::string eec_tool_name = std::string("ElectronEfficiencyCorrectionTool_") + m_name;
  m_asgElectronEfficiencyCorrectionTool = new AsgElectronEfficiencyCorrectionTool( eec_tool_name.c_str() );
  m_asgElectronEfficiencyCorrectionTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  std::vector<std::string> inputFiles{ m_corrFileName1 } ; // initialise vector w/ all the files containing corrections
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElectronEfficiencyCorrectionTool->setProperty("CorrectionFileNameList",inputFiles),"Failed to set property CorrectionFileNameList");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElectronEfficiencyCorrectionTool->setProperty("ForceDataType",1),"Failed to set property ForceDataType");
  RETURN_CHECK( "ElectronEfficiencyCorrector::initialize()", m_asgElectronEfficiencyCorrectionTool->initialize(), "Failed to properly initialize the AsgElectronEfficiencyCorrectionTool");


  // Get a list of systematics
  CP::SystematicSet recSysts = m_asgElectronEfficiencyCorrectionTool->recommendedSystematics();
  // Convert into a simple list
  m_systList = CP::make_systematics_vector(recSysts);

  for ( const auto& syst_it : m_systList ){
      Info("initialize()"," available systematic: %s", (syst_it.name()).c_str());
  }
  if( m_systName.empty() ){
      Info("initialize()"," Running w/ nominal configuration!");
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

  if(m_debug) Info("execute()", "Applying Electron Efficiency Correction... \n");

  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::ElectronContainer* correctedElectrons = HelperFunctions::getContainer<xAOD::ElectronContainer>(m_inContainerName, m_event, m_store);
  // create ConstDataVector to be eventually stored in TStore
  ConstDataVector<xAOD::ElectronContainer>* correctedElectronsCDV = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
  correctedElectronsCDV->reserve( correctedElectrons->size() );

  if(m_debug){
    unsigned int idx(0);
    for( auto el : *(correctedElectrons) ) {
      Info( "execute", "Input electron %i, pt = %.2f GeV ", idx, (el->pt() * 1e-3) );
      ++idx;
    }
  }

  // loop over available systematics
  for(const auto& syst_it : m_systList){

    // appends syst name to decoration
    if( !syst_it.name().empty() ){
      RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_asgElectronEfficiencyCorrectionTool->setProperty("ResultPrefix",syst_it.name()), "Failed to set property ResultPrefix" );
    }

    // if not running systematics (i.e., syst name is "") or running on one syst only, skip directly all other syst
    if(!m_runAllSyst){
      if( syst_it.name() != m_systName ) { continue; }
    }

    // apply syst
    if (m_asgElectronEfficiencyCorrectionTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok) {
      Error("initialize()", "Failed to configure AsgElectronEfficiencyCorrectionTool for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if (m_debug) Info("execute()", "Successfully applied systematic: %s", m_asgElectronEfficiencyCorrectionTool->appliedSystematics().name().c_str());

    // and now apply efficiency SF!
    unsigned int idx(0);
    double SF(0);
    for( auto el_itr : *(correctedElectrons) ) {

      if(m_debug) Info( "execute", "Checking electron %i, pt = %.2f GeV, eta = %.2f ", idx, (el_itr->pt() * 1e-3), el_itr->caloCluster()->eta());

      // skip electron if outside acceptance for SF calculation
      if(el_itr->pt() < 7e3 || fabs( el_itr->caloCluster()->eta() ) > 2.47) {
    	if(m_debug) Info( "execute", "Apply SF: skipping electron %i, is outside acceptance", idx);
    	continue;
      }
      if(m_asgElectronEfficiencyCorrectionTool->getEfficiencyScaleFactor( *el_itr, SF ) != CP::CorrectionCode::Ok){
    	Error( "execute()", "Problem in getEfficiencyScaleFactor");
    	return EL::StatusCode::FAILURE;
      }
      // apply SF as decoration for this electron
//      if(m_asgElectronEfficiencyCorrectionTool->applyEfficiencyScaleFactor( *el_itr ) != CP::CorrectionCode::Ok){
//    	Error( "execute()", "Problem in applyEfficiencyScaleFactor");
//    	return EL::StatusCode::FAILURE;
//      }

      if(m_debug) Info( "execute", "===>>> Resulting SF (from get function) %f, (from apply function) %f", SF, el_itr->auxdata< float >("SF"));

      ++idx;
    } // close calibration loop

  } // close loop on systematics

  // save pointers in ConstDataVector
  RETURN_CHECK( "ElectronCalibrator::execute()", HelperFunctions::makeSubsetCont(correctedElectrons, correctedElectronsCDV, "", ToolName::CORRECTOR), "");

  // add container to TStore
  RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( correctedElectronsCDV,  m_outContainerName ), "Failed to store container.");

  if(m_debug) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

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

  Info("finalize()", "Deleting tool instances... \n");

  if(m_asgElectronEfficiencyCorrectionTool){
    delete m_asgElectronEfficiencyCorrectionTool; m_asgElectronEfficiencyCorrectionTool = 0;
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

  Info("histFinalize()", "Calling histFinalize \n");

  return EL::StatusCode::SUCCESS;
}

