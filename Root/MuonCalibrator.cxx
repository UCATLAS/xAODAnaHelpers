/********************************************
 *
 * Interface to CP Muon calibration tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 * F. Scutti (federico.scutti@cern.ch)
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
#include "PATInterfaces/CorrectionCode.h" // to check the return correction code status of tools

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(MuonCalibrator)

MuonCalibrator :: MuonCalibrator () :
    Algorithm("MuonCalibrator")
{
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

  ANA_MSG_INFO( "Calling setupJob");

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
  ANA_CHECK( xAH::Algorithm::algInitialize());
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

  ANA_MSG_INFO( "Initializing MuonCalibrator Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  // Check if is MC
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  m_isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );

  // Create a ToolHandle of the PRW tool which is used for the random generation
  // of run numbers. Depending on the outcome a specific initialization of the tool
  // will be used.
  //
  if( m_isMC ){
    if(!setToolName(m_pileup_tool_handle, "Pileup")){
      ANA_MSG_FATAL("A configured " << m_pileup_tool_handle.typeAndName() << " must have been previously created! Are you creating one in xAH::BasicEventSelection?" );
      return EL::StatusCode::FAILURE;
    }
    ANA_CHECK( m_pileup_tool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_pileup_tool_handle);
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  std::string tmp_years = m_Years;

  // Parse all comma seperated years
  //
  while ( tmp_years.size() > 0) {
    size_t pos = tmp_years.find_first_of(',');
    if ( pos == std::string::npos ) {
      pos = tmp_years.size();
      m_YearsList.push_back(tmp_years.substr(0, pos));
      tmp_years.erase(0, pos);
    } else {
      m_YearsList.push_back(tmp_years.substr(0, pos));
      tmp_years.erase(0, pos+1);
    }
  }

  // Initialize vector of names
  //
  for(auto yr : m_YearsList) {
    m_muonCalibrationAndSmearingTool_names[yr] = "MuonCalibrationAndSmearingTool_" + yr;
  }

  // Initialize the CP::MuonCalibrationAndSmearingTool
  //
  for(auto yr : m_YearsList) {
    ANA_CHECK( checkToolStore<CP::MuonCalibrationAndSmearingTool>(m_muonCalibrationAndSmearingTool_names[yr]));

    if ( asg::ToolStore::contains<CP::MuonCalibrationAndSmearingTool>( m_muonCalibrationAndSmearingTool_names[yr] ) ) {
      m_muonCalibrationAndSmearingTools[yr] = asg::ToolStore::get<CP::MuonCalibrationAndSmearingTool>( m_muonCalibrationAndSmearingTool_names[yr] );
    } else {
      m_muonCalibrationAndSmearingTools[yr] = new CP::MuonCalibrationAndSmearingTool( m_muonCalibrationAndSmearingTool_names[yr]);
      m_muonCalibrationAndSmearingTools[yr]->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO

      if ( yr == "Data16") {
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("Release", "Recs2016_15_07"));
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("SagittaCorr", m_do_sagittaCorr ));
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("doSagittaMCDistortion", m_do_sagittaMCDistortion ));
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("SagittaRelease", m_sagittaRelease ));
      } else if (yr == "Data15") {
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("Release", "Recs2016_08_07"));
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("SagittaCorr", false ));
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("doSagittaMCDistortion", false ));
      } else if ( !yr.empty() ) {
        ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("Year", yr ));
      }

      if ( !m_release.empty() ) { ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->setProperty("Release", m_release)); }

      ANA_CHECK( m_muonCalibrationAndSmearingTools[yr]->initialize());

    }
  }

  // ***********************************************************

  // Get a list of recommended systematics for this tool
  //
  //const CP::SystematicSet recSyst = CP::SystematicSet();
  const CP::SystematicSet& recSyst = m_muonCalibrationAndSmearingTools[m_YearsList.at(0)]->recommendedSystematics();

  ANA_MSG_INFO(" Initializing Muon Calibrator Systematics :");
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, msg() );

  ANA_MSG_INFO("Will be using MuonCalibrationAndSmearingTool systematic:");
  std::vector< std::string >* SystMuonsNames = new std::vector< std::string >;
  for ( const auto& syst_it : m_systList ) {
    if ( m_systName.empty() ) {
      ANA_MSG_INFO("\t Running w/ nominal configuration only!");
      break;
    }
    SystMuonsNames->push_back(syst_it.name());
    ANA_MSG_INFO("\t " << syst_it.name());
  }

  ANA_CHECK(m_store->record(SystMuonsNames, "muons_Syst"+m_name ));
  
  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systList, m_name, fileMD);
  }

  ANA_MSG_INFO( "MuonCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG( "Applying Muon Calibration And Smearing ... ");

  m_numEvent++;

  if ( !m_isMC && !m_forceDataCalib ) {
    if ( m_numEvent == 1 ) { ANA_MSG_INFO( "Sample is Data! Do not apply any Muon Calibration... "); }
  }

  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  std::string randYear = "Data16";

  // a default 2016 run
  int runNumber = 296939;

  if ( !m_isMC && m_forceDataCalib ) {
    runNumber = eventInfo->runNumber();
  } else if ( m_isMC ) {
    runNumber = m_pileup_tool_handle->getRandomRunNumber( *eventInfo, true );
  }

  if (runNumber >= 266904 && runNumber <= 284484 ) {
    randYear = "Data15";
    if( ! (std::find(m_YearsList.begin(), m_YearsList.end(), randYear) != m_YearsList.end()) ) {
      ANA_MSG_ERROR( "Random runNumber is 2015 but no corresponding MuonCalibrationAndSmearingTool tool has been initialized. Check ilumicalc config or extend m_Years!");
      return EL::StatusCode::FAILURE;
    }

  } else if (runNumber >= 296939 ) {

    randYear = "Data16";
    if( ! (std::find(m_YearsList.begin(), m_YearsList.end(), randYear) != m_YearsList.end()) ) {
     ANA_MSG_ERROR( "Random runNumber is 2016 but no corresponding MuonCalibrationAndSmearingTool tool has been initialized. Check ilumicalc config or extend m_Years!");
     return EL::StatusCode::FAILURE;
    }

  }



  // get the collections from TEvent or TStore
  //
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  const xAOD::MuonContainer* inMuons(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(inMuons, m_inContainerName, m_event, m_store, msg()) );

  // loop over available systematics - remember syst == EMPTY_STRING --> baseline
  // prepare a vector of the names of CDV containers
  // must be a pointer to be recorded in TStore
  //
  std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

  for ( const auto& syst_it : m_systList ) {

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
    if ( m_muonCalibrationAndSmearingTools[randYear]->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR( "Failed to configure MuonCalibrationAndSmearingTool for systematic " << syst_it.name());
      return EL::StatusCode::FAILURE;
    }

    // create shallow copy for calibration - one per syst
    //
    std::pair< xAOD::MuonContainer*, xAOD::ShallowAuxContainer* > calibMuonsSC = xAOD::shallowCopyContainer( *inMuons );
    // create ConstDataVector to be eventually stored in TStore
    //
    ConstDataVector<xAOD::MuonContainer>* calibMuonsCDV = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
    calibMuonsCDV->reserve( calibMuonsSC.first->size() );

    // now calibrate!
    //
    unsigned int idx(0);
    if ( m_isMC || m_forceDataCalib ) {

      for ( auto muSC_itr : *(calibMuonsSC.first) ) {

	ANA_MSG_DEBUG( "  uncailbrated muon " << idx << ", pt = " << muSC_itr->pt()*1e-3 << " GeV");
	if(muSC_itr-> primaryTrackParticle()){
	  if ( m_muonCalibrationAndSmearingTools[randYear]->applyCorrection(*muSC_itr) == CP::CorrectionCode::Error ) {  // Can have CorrectionCode values of Ok, OutOfValidityRange, or Error. Here only checking for Error.
	    ANA_MSG_WARNING( "MuonCalibrationAndSmearingTool returned Error CorrectionCode");		  // If OutOfValidityRange is returned no modification is made and the original muon values are taken.
	  }
	}

	ANA_MSG_DEBUG( "  corrected muon pt = " << muSC_itr->pt()*1e-3 << " GeV");

	++idx;

      } // close calibration loop
    }

    ANA_MSG_DEBUG( "setOriginalObjectLink");
    if ( !xAOD::setOriginalObjectLink(*inMuons, *(calibMuonsSC.first)) ) {
      ANA_MSG_ERROR( "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    // save pointers in ConstDataVector with same order
    //
    ANA_MSG_DEBUG( "makeSubsetCont");
    ANA_CHECK( HelperFunctions::makeSubsetCont(calibMuonsSC.first, calibMuonsCDV, msg()));
    ANA_MSG_DEBUG( "done makeSubsetCont");

    // sort after coping to CDV
    if ( m_sort ) {
      ANA_MSG_DEBUG( "sorting");
      std::sort( calibMuonsCDV->begin(), calibMuonsCDV->end(), HelperFunctions::sort_pt );
    }

    // add SC container to TStore
    //
    ANA_MSG_DEBUG( "recording calibMuonsSC");
    ANA_CHECK( m_store->record( calibMuonsSC.first,  outSCContainerName  ));
    ANA_CHECK( m_store->record( calibMuonsSC.second, outSCAuxContainerName ));

    //
    // add ConstDataVector to TStore
    //
    ANA_MSG_DEBUG( "record calibMuonsCDV");
    ANA_CHECK( m_store->record( calibMuonsCDV, outContainerName));

  } // close loop on systematics

  // add vector<string container_names_syst> to TStore
  //
  ANA_MSG_DEBUG( "record m_outputAlgoSystNames");
  ANA_CHECK( m_store->record( vecOutContainerNames, m_outputAlgoSystNames));

  // look what we have in TStore
  //
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  ANA_MSG_DEBUG( "Left ");
  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MuonCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG( "Calling postExecute");

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

  ANA_MSG_INFO( "Deleting tool instances...");

  for(auto yr : m_YearsList) {
    if ( m_muonCalibrationAndSmearingTools[yr] ) { m_muonCalibrationAndSmearingTools[yr] = nullptr; delete m_muonCalibrationAndSmearingTools[yr]; }
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

  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}
