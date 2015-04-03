/******************************************
 *
 * Interface to CP Muon Efficiency Correction Tool.
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
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(MuonEfficiencyCorrector)


MuonEfficiencyCorrector :: MuonEfficiencyCorrector () {
}

MuonEfficiencyCorrector :: MuonEfficiencyCorrector (std::string name, std::string configName ) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_MuonEffSFTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonEfficiencyCorrector()", "Calling constructor");

}


EL::StatusCode  MuonEfficiencyCorrector :: configure ()
{
  Info("configure()", "Configuing MuonEfficiencyCorrector Interface. User configuration read from : %s ", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "MuonEfficiencyCorrector::configure()", m_configName);

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
  Info("configure()", "MuonEfficiencyCorrector Interface succesfully configured! ");

  delete config;

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

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the MuonEfficiencyCorrectionTool
  std::string mefsf_tool_name = std::string("MuonEfficiencyCorrectionTool_") + m_name;
  m_MuonEffSFTool = new CP::MuonEfficiencyScaleFactors( mefsf_tool_name.c_str() );
  m_MuonEffSFTool->msg().setLevel( MSG::INFO ); // DEBUG, VERBOSE, INFO, ERROR

  RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_MuonEffSFTool->setProperty("WorkingPoint","CBandST"),"Failed to set property");  // set the Working Point you are using
  RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_MuonEffSFTool->setProperty("DataPeriod","2012"),"Failed to set property");
  // test audit trail
  RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_MuonEffSFTool->setProperty("doAudit",false),"Failed to set property"); // audit trail functionality.
  RETURN_CHECK( "MuonEfficiencyCorrector::initialize()", m_MuonEffSFTool->initialize(), "Failed to properly initialize the MuonEfficiencyCorrectionTool");


  // Get a list of affecting systematics
  CP::SystematicSet affectSysts = m_MuonEffSFTool->affectingSystematics();
  // Convert into a simple list
  std::vector<CP::SystematicSet> affectSystList = CP::make_systematics_vector(affectSysts);
  for ( const auto& syst_it : affectSystList ){
      Info("initialize()"," tool can be affected by systematic: %s", (syst_it.name()).c_str());
  }

  // Get a list of recommended systematics
  CP::SystematicSet recSysts = m_MuonEffSFTool->recommendedSystematics();
  // Convert into a simple list
  m_systList = CP::make_systematics_vector(recSysts);
  for ( const auto& syst_it : m_systList ){
      Info("initialize()"," available recommended systematic: %s", (syst_it.name()).c_str());
  }

  if( m_systName.empty() && !m_runAllSyst ){
      Info("initialize()"," Running w/ nominal configuration!");
  }

  Info("initialize()", "MuonEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Muon Efficiency Correction... ");

  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::MuonContainer* correctedMuons(nullptr);
  RETURN_CHECK("MuonEfficiencyCorrector::execute()", HelperFunctions::retrieve(correctedMuons, m_inContainerName, m_event, m_store, m_debug) ,"");

  // create ConstDataVector to be eventually stored in TStore
  ConstDataVector<xAOD::MuonContainer>* correctedMuonsCDV = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
  correctedMuonsCDV->reserve( correctedMuons->size() );

  if(m_debug){
    unsigned int idx(0);
    for( auto mu_itr : *(correctedMuons) ) {
      Info( "execute", "Input Muon %i, pt = %.2f GeV ", idx, (mu_itr->pt() * 1e-3) );
      ++idx;
    }
  }

  // loop over available systematics
  for(const auto& syst_it : m_systList){

    // prepends syst name to decoration
    std::string SFdecor = std::string("SF");
    if( !syst_it.name().empty() ){
       std::string prepend = syst_it.name() + "_";
       SFdecor.insert( 0, prepend );
    }
    RETURN_CHECK( "MuonEfficiencyCorrector::execute()", m_MuonEffSFTool->setProperty( "ScaleFactorDecorationName", SFdecor.c_str() ), "Failed to set property ScaleFactorDecorationName" );

    if(m_debug) Info("execute()", "SF decoration name is: %s", SFdecor.c_str());

    // if not running systematics (i.e., syst name is "") or running on one syst only, skip directly all other syst
    if(!m_runAllSyst){
      if( syst_it.name() != m_systName ) { continue; }
    }

    // apply syst
    if (m_MuonEffSFTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok) {
      Error("initialize()", "Failed to configure MuonEfficiencyCorrections for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if(m_debug) Info("execute()", "Successfully applied systematic: %s", syst_it.name().c_str());

    // prepare a vector to hold SF replicas
    // the size will tell the tool how many you want
    // -> we will try 50!
    std::vector<float> replicas(50);

    // and now apply data-driven efficiency and efficiency SF!
    float eff(0.0), SF(0.0);
    for( auto mu_itr : *(correctedMuons)) {

	// directly obtain reco efficiency
        if( m_MuonEffSFTool->getRecoEfficiency( *mu_itr, eff ) != CP::CorrectionCode::Ok){
    	  Error( "execute()", "Problem in getRecoEfficiency");
    	  return EL::StatusCode::FAILURE;
	}
        if(m_debug) Info( "execute", "\t reco efficiency = %g", eff );

        // directly obtain efficiency SF
        if( m_MuonEffSFTool->getEfficiencyScaleFactor( *mu_itr, SF ) != CP::CorrectionCode::Ok){
    	  Error( "execute()", "Problem in getEfficiencyScaleFactor");
    	  return EL::StatusCode::FAILURE;
        }
	if(m_debug) Info( "execute()", "\t efficiency SF = %g", SF );

        // apply reco efficiency as decoration for this muon
        if( m_MuonEffSFTool->applyRecoEfficiency( *mu_itr ) != CP::CorrectionCode::Ok){
    	  Error( "execute()", "Problem in applyRecoEfficiency");
    	  return EL::StatusCode::FAILURE;
        }

        // apply SF as decoration for this muon
        if( m_MuonEffSFTool->applyEfficiencyScaleFactor( *mu_itr ) != CP::CorrectionCode::Ok){
    	  Error( "execute()", "Problem in applyEfficiencyScaleFactor");
    	  return EL::StatusCode::FAILURE;
        }

        // uncomment to try out replica genration (commented as it produces a lot of text)
        //  if( m_MuonEffSFTool->getEfficiencyScaleFactorReplicas( *mu_itr, replicas ) != CP::CorrectionCode::Ok ){
    	//       Error( "execute()", "Problem in getEfficiencyScaleFactorReplicas");
    	//       return EL::StatusCode::FAILURE;
	//  }
        //
        //  for (size_t t =0; t < replicas.size();t++){
        //    Info( "execute()", "\t scaleFactor Replica %d = %.8f",static_cast<int>(t), replicas[t] );
        //  }

        if(m_debug){
	  Info( "execute", "===>>> Resulting reco efficiency (from get function) %f, (from apply function) %f", eff, mu_itr->auxdataConst< float >("Efficiency"));
          Info( "execute", "===>>> Resulting SF (from get function) %f, (from apply function) %f",              SF,  mu_itr->auxdataConst< float >(SFdecor.c_str()));
	}

	// if we run in audit trail mode, we get some info
        // Info( "execute", "\t Muon Audit info: MuonEfficiencyCorrections = %d, MuonEfficiencyCorrectionsVersion = %s, AppliedCorrections = %s",
        //											    mu_itr->auxdataConst< bool >( "MuonEfficiencyCorrections" ),
        //											    mu_itr->auxdataConst< std::string >( "MuonEfficiencyCorrectionsVersion" ).c_str(),
        //											    mu_itr->auxdataConst< std::string >( "AppliedCorrections" ).c_str());

    } // close muon loop

  } // close loop on systematics


  if(m_debug){
    unsigned int idx(0);
    for( auto mu_itr : *(correctedMuons) ) {
      Info( "execute", "Input Muon %i, pt = %.2f GeV ", idx, (mu_itr->pt() * 1e-3) );
      ++idx;
    }
  }


  // save pointers in ConstDataVector
  RETURN_CHECK( "MuonCalibrator::execute()", HelperFunctions::makeSubsetCont(correctedMuons, correctedMuonsCDV, "", ToolName::CORRECTOR), "");

  // add container to TStore
  RETURN_CHECK( "MuonEfficiencyCorrector::execute()", m_store->record( correctedMuonsCDV,  m_outContainerName ), "Failed to store container.");

  if(m_debug) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute");

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

  if(m_MuonEffSFTool){
    delete m_MuonEffSFTool; m_MuonEffSFTool = 0;
  }

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

