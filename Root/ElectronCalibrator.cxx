/*******************************************************
 *
 * Interface to CP Electron calibration tool(s).
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

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronCalibrator)


ElectronCalibrator :: ElectronCalibrator () :
    Algorithm("ElectronCalibrator")
{
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

  ANA_MSG_INFO( "Calling setupJob");

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
  ANA_CHECK( xAH::Algorithm::algInitialize());
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

  ANA_MSG_INFO( "Initializing ElectronCalibrator Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!
  // shallow copies are made with this output container name
  m_outSCContainerName      = m_outContainerName + "ShallowCopy";
  m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize the CP::EgammaCalibrationAndSmearingTool
  //
  if ( asg::ToolStore::contains<CP::EgammaCalibrationAndSmearingTool>("EgammaCalibrationAndSmearingTool") ) {
    m_EgammaCalibrationAndSmearingTool = asg::ToolStore::get<CP::EgammaCalibrationAndSmearingTool>("EgammaCalibrationAndSmearingTool");
  } else {
    m_EgammaCalibrationAndSmearingTool = new CP::EgammaCalibrationAndSmearingTool("EgammaCalibrationAndSmearingTool");
  }
  m_EgammaCalibrationAndSmearingTool->msg().setLevel( MSG::ERROR ); // DEBUG, VERBOSE, INFO
  ANA_CHECK( m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", m_esModel));
  ANA_CHECK( m_EgammaCalibrationAndSmearingTool->setProperty("decorrelationModel", m_decorrelationModel));

  //
  // For AFII samples
  //
  if ( isFastSim() ){
    ANA_MSG_INFO( "Setting simulation flavour to AFII");
    ANA_CHECK( m_EgammaCalibrationAndSmearingTool->setProperty("useAFII", 1));
  }
  ANA_CHECK( m_EgammaCalibrationAndSmearingTool->initialize());

  // Get a list of recommended systematics for this tool
  //
  //const CP::SystematicSet recSyst = CP::SystematicSet();
  const CP::SystematicSet& recSyst = m_EgammaCalibrationAndSmearingTool->recommendedSystematics();

  ANA_MSG_INFO(" Initializing Electron Calibrator Systematics :");
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, msg() );

  ANA_MSG_INFO("Will be using EgammaCalibrationAndSmearingTool systematic:");
  auto SystElectronsNames = std::make_unique< std::vector< std::string > >();
  for ( const auto& syst_it : m_systList ) {
    if ( m_systName.empty() ) {
      ANA_MSG_INFO("\t Running w/ nominal configuration only!");
      break;
    }
    SystElectronsNames->push_back(syst_it.name());
    ANA_MSG_INFO("\t " << syst_it.name());
  }

  ANA_CHECK(m_store->record(std::move(SystElectronsNames), "ele_Syst"+m_name ));

  // ***********************************************************

  // initialize the CP::IsolationCorrectionTool
  //
  if ( m_applyIsolationCorrection ) {
    if ( asg::ToolStore::contains<CP::IsolationCorrectionTool>("IsolationCorrectionTool") ) {
      m_IsolationCorrectionTool = asg::ToolStore::get<CP::IsolationCorrectionTool>("IsolationCorrectionTool");
    } else {
      m_IsolationCorrectionTool = new CP::IsolationCorrectionTool("IsolationCorrectionTool");
    }
    m_IsolationCorrectionTool->msg().setLevel( MSG::INFO ); // DEBUG, VERBOSE, INFO
    ANA_CHECK( m_IsolationCorrectionTool->setProperty("IsMC", isMC() ));
    ANA_CHECK( m_IsolationCorrectionTool->initialize());
    ANA_MSG_INFO( "Applying electron isolation correction" );
  }

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systList, m_name, fileMD);
  }

  // ***********************************************************

  ANA_MSG_INFO( "ElectronCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG("Applying Electron Calibration ... ");

  m_numEvent++;

  // get the collection from TEvent or TStore
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  const xAOD::ElectronContainer* inElectrons(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(inElectrons, m_inContainerName, m_event, m_store, msg()) );

  // loop over available systematics - remember syst == EMPTY_STRING --> baseline
  // prepare a vector of the names of CDV containers
  // must be a pointer to be recorded in TStore
  //
  auto vecOutContainerNames = std::make_unique< std::vector< std::string > >();

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
    if ( m_EgammaCalibrationAndSmearingTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR( "Failed to configure EgammaCalibrationAndSmearingTool for systematic " << syst_it.name());
      return EL::StatusCode::FAILURE;
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
      ANA_MSG_DEBUG("Checking electron "<<idx<<", raw pt = "<<elSC_itr->pt()*1e-3<<" GeV ");
      if ( elSC_itr->pt() > 7e3 && !(elSC_itr->caloCluster()) ){
        ANA_MSG_DEBUG( "electron " << idx << ", raw pt = " << elSC_itr->pt() * 1e-3 << " GeV, does not have caloCluster()! " );
      }

      // apply calibration (w/ syst) and leakage correction to calo based iso vars
      //
      if ( elSC_itr->caloCluster() && elSC_itr->trackParticle() ) {  // NB: derivations might remove CC and tracks for low pt electrons
        if ( m_EgammaCalibrationAndSmearingTool->applyCorrection( *elSC_itr ) != CP::CorrectionCode::Ok ) {
          ANA_MSG_WARNING( "Problem in CP::EgammaCalibrationAndSmearingTool::applyCorrection()");
        }

        if ( m_applyIsolationCorrection ) {
          if ( elSC_itr->pt() > 7e3 && m_IsolationCorrectionTool->CorrectLeakage( *elSC_itr ) != CP::CorrectionCode::Ok ) {
            ANA_MSG_WARNING( "Problem in CP::IsolationCorrectionTool::CorrectLeakage()");
          }
        }
      }

      ANA_MSG_DEBUG( "Calibrated pt with systematic: " << syst_it.name() <<" , pt = " << elSC_itr->pt() * 1e-3 << " GeV");

      ++idx;

    } // close calibration loop

    if ( !xAOD::setOriginalObjectLink(*inElectrons, *(calibElectronsSC.first)) ) {
      ANA_MSG_ERROR( "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    // save pointers in ConstDataVector with same order
    //
    ANA_CHECK( HelperFunctions::makeSubsetCont(calibElectronsSC.first, calibElectronsCDV, msg()));

    // Sort after copying to CDV.
    if ( m_sort ) {
      std::sort( calibElectronsCDV->begin(), calibElectronsCDV->end(), HelperFunctions::sort_pt );
    }

    // add SC container to TStore
    //
    ANA_CHECK( m_store->record( calibElectronsSC.first,  outSCContainerName  ));
    ANA_CHECK( m_store->record( calibElectronsSC.second, outSCAuxContainerName ));
    // add ConstDataVector to TStore
    //
    ANA_CHECK( m_store->record( calibElectronsCDV, outContainerName));

  } // close loop on systematics

  // add vector<string container_names_syst> to TStore
  //
  ANA_CHECK( m_store->record( std::move(vecOutContainerNames), m_outputAlgoSystNames));

  // look what we have in TStore
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

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

  ANA_MSG_INFO( "Deleting tool instances...");

  if ( m_EgammaCalibrationAndSmearingTool ) { delete m_EgammaCalibrationAndSmearingTool; m_EgammaCalibrationAndSmearingTool = nullptr; }
  if ( m_IsolationCorrectionTool )          { delete m_IsolationCorrectionTool; m_IsolationCorrectionTool = nullptr; }

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

  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}
