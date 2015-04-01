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

ElectronEfficiencyCorrector :: ElectronEfficiencyCorrector (std::string name, std::string configName, 
	std::string systName, float systVal) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_systName(systName),       // if running systs - the name of the systematic
  m_systVal(systVal),         // if running systs - the value ( +/- 1 )
  m_runSysts(false),          // gets set later is syst applies to this tool  
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

  // name of algo input container comes from - only if running on systematics
  m_inputAlgo               = config->GetValue("InputAlgo",  "ElectronCollection_Calib_Algo"); // default: container of calibrated electrons
  m_outputAlgo              = config->GetValue("OutputAlgo", "ElectronCollection_CalibCorr_Algo"); 
  									       
  // Systematics stuff
  m_systName		    = config->GetValue("SystName" , "" );      // default: no syst
  m_systVal 		    = config->GetValue("SystVal" , 0. );
  // file(s) containing corrections
  m_corrFileName1           = config->GetValue("CorrectionFileName1" , "" );
  //m_corrFileName2         = config->GetValue("CorrectionFileName2" , "" );


  if( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "ElectronEfficiencyCorrector Interface succesfully configured! \n");

  delete config;
  
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

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

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
  const CP::SystematicSet recSysts = m_asgElectronEfficiencyCorrectionTool->recommendedSystematics();
  Info("initialize()"," Initializing Electron Efficiency Corrector Systematics :");
  m_systList = HelperFunctions::getListofSystematics( recSysts, m_systName, m_systVal );
  // Convert into a simple list
  m_systList = CP::make_systematics_vector(recSysts);
    
  if( !m_systList.empty() ) { m_runSysts = true; }

  if(m_debug){
    for ( const auto& syst_it : m_systList ){
      Info("initialize()"," available systematic: %s", (syst_it.name()).c_str());
    }
  }
  for ( const auto& syst_it : m_systList ){
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

  if(m_debug) Info("execute()", "Applying Electron Efficiency Correction... \n");

  m_numEvent++;
 
  // initialise containers
  const xAOD::ElectronContainer* correctedElectrons = 0;
  ConstDataVector<xAOD::ElectronContainer>* correctedElectronsCDV = 0; 
  
  // if m_inputAlgo = "" --> input comes from xAOD, or just running one collection, 
  // then get the one collection and be done with it
  if ( m_inputAlgo.empty() ) { 

    	correctedElectrons = HelperFunctions::getContainer<xAOD::ElectronContainer>(m_inContainerName, m_event, m_store);
    	correctedElectronsCDV = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
    	correctedElectronsCDV->reserve( correctedElectrons->size() );
	
	// decorate electrons w/ SF
	this->executeSF( correctedElectrons );
    	
	// save pointers in ConstDataVector
    	RETURN_CHECK( "ElectronCalibrator::execute()", HelperFunctions::makeSubsetCont(correctedElectrons, correctedElectronsCDV, "", ToolName::CORRECTOR), "");
    	// add container to TStore
    	RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( correctedElectronsCDV, m_outContainerName), "Failed to store container.");

  } else {
  // if m_inputAlgo = NOT EMPTY --> you are retrieving syst varied containers from an upstream algo 
  
	// get vector of string giving the syst names of the upstream algo m_inputAlgo (rememeber: 1st element is a blank string: nominal case!)
        std::vector<std::string>* systNames = 0;
    	if ( m_store->contains< std::vector<std::string> >( m_inputAlgo ) ) { 
    	  if(!m_store->retrieve( systNames, m_inputAlgo ).isSuccess()) {
    	    Error("execute()", "Cannot find vector from %s algo. Aborting", m_inputAlgo.c_str());
    	    return StatusCode::FAILURE;
	  }
    	} else {
    	  Error("execute()", "TStore does not contain %s algo. Aborting", m_inputAlgo.c_str());
	  return StatusCode::FAILURE;
    	}
    	
    	// prepare a vector of the names of CDV containers 
    	// must be a pointer to be recorded in TStore
    	// for now just copy the one you just retrieved in it!
    	std::vector<std::string>* vecOutContainerNames = new std::vector< std::string >(*systNames);

    	// just to check everything is fine
    	if(m_debug){
    	  Info("execute()","output vector already contains systematics:" );
    	  for (auto it : *vecOutContainerNames){
    	    Info("execute()" ,"\t %s ", it.c_str());
    	  }
    	}

    	// loop over systematic sets available 
    	for( auto systName : *systNames ) {
    	
    	   correctedElectrons = HelperFunctions::getContainer<xAOD::ElectronContainer>(m_inContainerName+systName, m_event, m_store);
    	   // create ConstDataVector to be eventually stored in TStore
    	   correctedElectronsCDV = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
    	   correctedElectronsCDV->reserve( correctedElectrons->size() );

    	   if(m_debug){
    	     unsigned int idx(0);
    	     for( auto el : *(correctedElectrons) ) {
    	       Info( "execute", "Input electron %i, pt = %.2f GeV ", idx, (el->pt() * 1e-3) );
    	       ++idx;
    	     }
    	   }
           
	   // decorate electrons w/ SF- there will be a decoration w/ different name for each syst!
	   this->executeSF( correctedElectrons );

    	   // save pointers in ConstDataVector
    	   RETURN_CHECK( "ElectronCalibrator::execute()", HelperFunctions::makeSubsetCont(correctedElectrons, correctedElectronsCDV, "", ToolName::CORRECTOR), "");
    	   // add container to TStore
    	   RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_store->record( correctedElectronsCDV, m_outContainerName+systName), "Failed to store container.");
    	
    	} // close loop on systematic sets available from upstream algo
      
        // save list of systs that should be considered down stream
        RETURN_CHECK( "execute()", m_store->record( vecOutContainerNames, m_outputAlgo), "Failed to record vector of output container names.");
  }

  // look what do we have in TStore  
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

EL::StatusCode ElectronEfficiencyCorrector :: executeSF (  const xAOD::ElectronContainer* correctedElectrons  )
{ 

  // loop over available systematics for this tool - remember syst == EMPTY_STRING --> baseline  
  for(const auto& syst_it : m_systList){

    // appends syst name to decoration
    if( !syst_it.name().empty() ){
      RETURN_CHECK( "ElectronEfficiencyCorrector::execute()", m_asgElectronEfficiencyCorrectionTool->setProperty("ResultPrefix",syst_it.name()), "Failed to set property ResultPrefix" );
    }

    // if not running systematics (i.e., syst name is "") or running on one syst only, skip directly all other syst
    //if(!(syst_it.name() == "All")){
    //  if( syst_it.name() != m_systName ) { continue; }
    //}
 
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

       if(m_debug) Info( "execute", "Checking electron %i, pt = %.2f GeV ", idx, (el_itr->pt() * 1e-3) );
       ++idx;
 
       // NB: derivations might remove CC and tracks for low pt electrons
       if( !(el_itr->caloCluster() && el_itr->trackParticle()) ){			       
         if(m_debug) Info( "execute", "Apply SF: skipping electron %i, it has no caloCluster or trackParticle info", idx);
         continue;
       }

       // skip electron if outside acceptance for SF calculation	 
       if( el_itr->pt() < 7e3 ) {
         if(m_debug) Info( "execute", "Apply SF: skipping electron %i, is outside pT acceptance", idx);
         continue;
       }
       if( fabs( el_itr->caloCluster()->eta() ) > 2.47) {
         if(m_debug) Info( "execute", "Apply SF: skipping electron %i, is outside |eta| acceptance", idx);
         continue; 
       }

       if(m_asgElectronEfficiencyCorrectionTool->getEfficiencyScaleFactor( *el_itr, SF ) != CP::CorrectionCode::Ok){
         Error( "execute()", "Problem in getEfficiencyScaleFactor");
         return EL::StatusCode::FAILURE;
       }
       // apply SF as decoration for this electron
       if(m_asgElectronEfficiencyCorrectionTool->applyEfficiencyScaleFactor( *el_itr ) != CP::CorrectionCode::Ok){
         Error( "execute()", "Problem in applyEfficiencyScaleFactor");
         return EL::StatusCode::FAILURE;
       }

       if(m_debug) Info( "execute", "===>>> Resulting SF (from get function) %f, (from apply function) %f", SF, el_itr->auxdata< float >("SF"));

    } // close electron loop

  }  // close loop on systematics

  return EL::StatusCode::SUCCESS;
}
