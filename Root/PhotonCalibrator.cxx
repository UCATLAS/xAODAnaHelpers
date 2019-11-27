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

#include "ElectronPhotonFourMomentumCorrection/EgammaCalibrationAndSmearingTool.h"
#include "ElectronPhotonSelectorTools/AsgPhotonIsEMSelector.h"
#include "ElectronPhotonShowerShapeFudgeTool/ElectronPhotonShowerShapeFudgeTool.h"

#include <PATCore/PATCoreEnums.h>

// apparently needed for egammaPIDs but was included in HelperClasses too?
#include "ElectronPhotonSelectorTools/egammaPIDdefs.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(PhotonCalibrator)


PhotonCalibrator :: PhotonCalibrator () :
    Algorithm("PhotonCalibrator")
{
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

  ANA_MSG_INFO( "Calling setupJob");

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
  ANA_CHECK( xAH::Algorithm::algInitialize());

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

  ANA_MSG_INFO( "Initializing PhotonCalibrator Interface... ");
  

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

  // initialize the CP::EgammaCalibrationAndSmearingTool
  //
  const std::string CalibToolName = m_name + "_EgammaCalibrationAndSmearingTool_Photons";
  if ( asg::ToolStore::contains<CP::EgammaCalibrationAndSmearingTool>(CalibToolName.c_str()) ) {
    m_EgammaCalibrationAndSmearingTool = asg::ToolStore::get<CP::EgammaCalibrationAndSmearingTool>(CalibToolName.c_str());
  } else {
    m_EgammaCalibrationAndSmearingTool = new CP::EgammaCalibrationAndSmearingTool(CalibToolName.c_str());
  }

  ANA_CHECK( m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", m_esModel));
  ANA_CHECK( m_EgammaCalibrationAndSmearingTool->setProperty("decorrelationModel", m_decorrelationModel));
  if(m_randomRunNumber>0) ANA_CHECK( m_EgammaCalibrationAndSmearingTool->setProperty("randomRunNumber", m_randomRunNumber));

  //Backwards compatibility
  if (m_useAFII)
    m_forceFastSim = true;
  if ( isFastSim() )
    ANA_CHECK( m_EgammaCalibrationAndSmearingTool->setProperty("useAFII", 1));

  ANA_CHECK( m_EgammaCalibrationAndSmearingTool->initialize());
  m_EgammaCalibrationAndSmearingTool->msg().setLevel( msg().level() );

  // Get a list of recommended systematics for this tool
  //
  //const CP::SystematicRegistry& systReg = CP::SystematicRegistry::getInstance();
  const CP::SystematicSet& recSyst = m_EgammaCalibrationAndSmearingTool->recommendedSystematics();;
  ANA_MSG_INFO(" Initializing Photon Calibrator Systematics :");

  m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, msg() );

  ANA_MSG_INFO("Will be using EgammaCalibrationAndSmearingTool systematic:");

  auto SystPhotonsNames = std::make_unique< std::vector< std::string > >();
  for ( const auto& syst_it : m_systList ) {
    SystPhotonsNames->push_back(syst_it.name());
    ANA_MSG_INFO("\t " << syst_it.name());
  }
    ANA_CHECK(m_store->record(std::move(SystPhotonsNames), "photons_Syst"+m_name ));

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
  m_photonTightIsEMSelector->msg().setLevel( msg().level() );

  // Medium
  const std::string MediumSelectorName = m_name + "_PhotonMediumIsEMSelector";
  if ( asg::ToolStore::contains<AsgPhotonIsEMSelector>(MediumSelectorName.c_str()) ) {
    m_photonMediumIsEMSelector = asg::ToolStore::get<AsgPhotonIsEMSelector>(MediumSelectorName.c_str());
  } else {
    m_photonMediumIsEMSelector = new AsgPhotonIsEMSelector ( MediumSelectorName.c_str() );
  }
  m_photonMediumIsEMSelector->msg().setLevel( msg().level() );

  // Loose
  const std::string LooseSelectorName = m_name + "_PhotonLooseIsEMSelector";
  if ( asg::ToolStore::contains<AsgPhotonIsEMSelector>(LooseSelectorName.c_str()) ) {
    m_photonLooseIsEMSelector = asg::ToolStore::get<AsgPhotonIsEMSelector>(LooseSelectorName.c_str());
  } else {
    m_photonLooseIsEMSelector = new AsgPhotonIsEMSelector ( LooseSelectorName.c_str() );
  }
  m_photonLooseIsEMSelector->msg().setLevel( msg().level() );

  //set the type of selection
  ANA_CHECK( m_photonTightIsEMSelector->setProperty("isEMMask", egammaPID::PhotonTight));
  ANA_CHECK( m_photonMediumIsEMSelector->setProperty("isEMMask", egammaPID::PhotonMedium));
  ANA_CHECK( m_photonLooseIsEMSelector->setProperty("isEMMask", egammaPID::PhotonLoose));

  //set the configuration file
  // todo : monitor the config files!
  ANA_CHECK( m_photonTightIsEMSelector->setProperty("ConfigFile", m_tightIDConfigPath));
  ANA_CHECK( m_photonMediumIsEMSelector->setProperty("ConfigFile",m_mediumIDConfigPath));
  ANA_CHECK( m_photonLooseIsEMSelector->setProperty("ConfigFile", m_looseIDConfigPath));


  ANA_CHECK( m_photonTightIsEMSelector->initialize());
  ANA_CHECK( m_photonMediumIsEMSelector->initialize());
  ANA_CHECK( m_photonLooseIsEMSelector->initialize());

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
  m_photonFudgeMCTool->msg().setLevel( msg().level() );

  int FFset = 21; // for MC15 samples, which are based on a geometry derived from GEO-21 from 2015+2016 data
  m_photonFudgeMCTool->setProperty("Preselection",FFset);
  ANA_CHECK( m_photonFudgeMCTool->initialize());


  ////// Efficiency correction tools //////
  if (isMC()) {

    int dataType = PATCore::ParticleDataType::Data;
    if ( isFastSim() ) {
      dataType = PATCore::ParticleDataType::Fast;
    } else {
      dataType = PATCore::ParticleDataType::Full;
    }
    ANA_MSG_DEBUG("isSimulation=" << ( isMC() ? "Y" : "N") << " isAFII=" << ( isFastSim() ? "Y" : "N" ) << " selected dataType=" << dataType );


    // photon efficiency correction tool
    //----------------------------------
    //create the tools

    if( !m_overridePhotonCalibMap.empty() ){
      ANA_MSG_WARNING("Overriding photon calibration map to " << m_overridePhotonCalibMap);
      ANA_CHECK( m_photonTightEffTool_handle .setProperty("MapFilePath", m_overridePhotonCalibMap) );
      ANA_CHECK( m_photonMediumEffTool_handle.setProperty("MapFilePath", m_overridePhotonCalibMap) );
      ANA_CHECK( m_photonLooseEffTool_handle .setProperty("MapFilePath", m_overridePhotonCalibMap) );
    }

    // set data type
    ANA_CHECK( m_photonTightEffTool_handle. setProperty("ForceDataType", dataType));
    ANA_CHECK( m_photonMediumEffTool_handle.setProperty("ForceDataType", dataType));
    ANA_CHECK( m_photonLooseEffTool_handle. setProperty("ForceDataType", dataType));

    // set debug levels
    ANA_CHECK( m_photonTightEffTool_handle. setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_photonMediumEffTool_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_photonLooseEffTool_handle. setProperty("OutputLevel", msg().level()));

    //initialize
    ANA_CHECK( m_photonTightEffTool_handle.retrieve());
    ANA_CHECK( m_photonMediumEffTool_handle.retrieve());
    ANA_CHECK( m_photonLooseEffTool_handle.retrieve());
  }

  //IsolationCorrectionTool
  ANA_CHECK(m_isolationCorrectionTool_handle.setProperty("OutputLevel", msg().level()));
  ANA_CHECK(m_isolationCorrectionTool_handle.retrieve());

  ANA_MSG_INFO( "PhotonCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode PhotonCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG("Applying Photon Calibration ... ");

  // get the collection from TEvent or TStore
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  const xAOD::PhotonContainer* inPhotons(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(inPhotons, m_inContainerName, m_event, m_store, msg()) );

  ANA_MSG_DEBUG("Retrieve has been completed with container name = " << m_inContainerName);

  // loop over available systematics - remember syst == EMPTY_STRING --> baseline
  // prepare a vector of the names of CDV containers
  // must be a pointer to be recorded in TStore
  //
  auto vecOutContainerNames = std::make_unique< std::vector< std::string > >();

  for ( const auto& syst_it : m_systList ) {
    ANA_MSG_DEBUG("Systematic Loop for m_systList=" << syst_it.name() );
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
    ANA_MSG_DEBUG("syst_it.name()=" << syst_it.name());

    if ( m_EgammaCalibrationAndSmearingTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR( "Failed to configure EgammaCalibrationAndSmearingTool for systematic " << syst_it.name());
      return EL::StatusCode::FAILURE;
    }

    ANA_MSG_DEBUG("Systematics applied");
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
      ANA_MSG_DEBUG("Checking photon " << idx << " raw pt = " << phSC_itr->pt()*1e-3 << " GeV " );

      if ( phSC_itr->pt() > 7e3 && !(phSC_itr->caloCluster()) ){
        ANA_MSG_WARNING( "photon "<<idx<<", raw pt = "<<phSC_itr->pt()*1e-3<<" GeV, does not have caloCluster()! " );
      }

      // apply calibration (w/ syst)
      //
      if ( (phSC_itr->author() & xAOD::EgammaParameters::AuthorPhoton) || (phSC_itr->author() & xAOD::EgammaParameters::AuthorAmbiguous) ) {
        if ( m_EgammaCalibrationAndSmearingTool->applyCorrection( *phSC_itr ) != CP::CorrectionCode::Ok ) {
          ANA_MSG_WARNING( "Problem in CP::EgammaCalibrationAndSmearingTool::applyCorrection()");
        }

        if ( m_isolationCorrectionTool_handle->applyCorrection( *phSC_itr ) != CP::CorrectionCode::Ok ) {
          ANA_MSG_WARNING( "Problem in CP::IsolationCorrection::applyCorrection()");
        }
      }

      ANA_MSG_DEBUG("Calibrated pt with systematic: " << syst_it.name() << " , pt = " << phSC_itr->pt() * 1e-3 << " GeV");

      ANA_CHECK( decorate(phSC_itr));

      ++idx;

    } // close calibration loop

    if ( !xAOD::setOriginalObjectLink(*inPhotons, *(calibPhotonsSC.first)) ) {
      ANA_MSG_ERROR( "Failed to set original object links -- MET rebuilding cannot proceed.");
    }

    // save pointers in ConstDataVector with same order
    //
    ANA_CHECK( HelperFunctions::makeSubsetCont(calibPhotonsSC.first, calibPhotonsCDV, msg()));

    // Sort after copying to CDV.
    if ( m_sort ) {
      std::sort( calibPhotonsCDV->begin(), calibPhotonsCDV->end(), HelperFunctions::sort_pt );
    }

    // add SC container to TStore
    //
    ANA_CHECK( m_store->record( calibPhotonsSC.first,  outSCContainerName  ));
    ANA_CHECK( m_store->record( calibPhotonsSC.second, outSCAuxContainerName ));
    // add ConstDataVector to TStore
    //
    ANA_CHECK( m_store->record( calibPhotonsCDV, outContainerName));

  } // close loop on systematics

  // add vector<string container_names_syst> to TStore
  //
  ANA_CHECK( m_store->record( std::move(vecOutContainerNames), m_outputAlgoSystNames));

  // look what we have in TStore
  //
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

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

  ANA_MSG_INFO( "Deleting tool instances...");

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

  ANA_MSG_INFO( "Finalization done.");

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

  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonCalibrator :: decorate(xAOD::Photon* photon)
{
  // (1) apply fudge factors and (2) evaluate the ID quality
  bool isTight(false);
  bool isMedium(false);
  bool isLoose(false);
  if (m_readIDFlagsFromDerivation){
    static SG::AuxElement::ConstAccessor< char > LHDecisionTight(  "DFCommonPhotonsIsEMTight" );
    static SG::AuxElement::ConstAccessor< char > LHDecisionMedium( "DFCommonPhotonsIsEMMedium" );
    static SG::AuxElement::ConstAccessor< char > LHDecisionLoose(  "DFCommonPhotonsIsEMLoose" );
    if (LHDecisionTight.isAvailable( *photon ))
      isTight = LHDecisionTight( *photon );
    if (LHDecisionMedium.isAvailable( *photon ))
      isMedium = LHDecisionMedium( *photon );
    if (LHDecisionLoose.isAvailable( *photon ))
      isLoose =  LHDecisionLoose( *photon );
  } else {
    if( isMC() && !isFastSim() ){
      if(m_photonFudgeMCTool->applyCorrection(*photon) == CP::CorrectionCode::Error){
        ANA_MSG_ERROR( "photonFudgeMCTool->applyCorrection(*photon) returned CP::CorrectionCode::Error");
        return EL::StatusCode::FAILURE;
      }
    }
    isTight  = m_photonTightIsEMSelector->accept(photon);
    isMedium = m_photonMediumIsEMSelector->accept(photon);
    isLoose  = m_photonLooseIsEMSelector->accept(photon);
  }
  photon->auxdecor< bool >( "PhotonID_Tight"    ) = isTight;
  photon->auxdecor< bool >( "PhotonID_Medium"   ) = isMedium;
  photon->auxdecor< bool >( "PhotonID_Loose"    ) = isLoose;
  ANA_MSG_DEBUG("isTight="<<(isTight?"Y":"N")<<" isMedium="<<(isMedium?"Y":"N")<<" isLoose="<<(isLoose?"Y":"N") );

  // (3) set efficiency correction
  if (isMC()) {
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
    if(cluster_et > 10000. && fabs(cluster_eta) < 2.37 && !inCrack){
      // SF
      if(m_photonTightEffTool_handle->getEfficiencyScaleFactor(*photon, photonTightEffSF) == CP::CorrectionCode::Error){
        ANA_MSG_ERROR("getEfficiencyScaleFactor returned CP::CorrectionCode::Error");
        return EL::StatusCode::FAILURE;
      }
      if(m_photonMediumEffTool_handle->getEfficiencyScaleFactor(*photon, photonMediumEffSF) == CP::CorrectionCode::Error){
        ANA_MSG_ERROR("getEfficiencyScaleFactor returned CP::CorrectionCode::Error");
        return EL::StatusCode::FAILURE;
      }
      if(m_photonLooseEffTool_handle->getEfficiencyScaleFactor(*photon, photonLooseEffSF) == CP::CorrectionCode::Error){
        ANA_MSG_ERROR("getEfficiencyScaleFactor returned CP::CorrectionCode::Error");
        return EL::StatusCode::FAILURE;
      }
      // SF error
      if(m_photonTightEffTool_handle->getEfficiencyScaleFactorError(*photon, photonTightEffSFError) == CP::CorrectionCode::Error){
        ANA_MSG_ERROR("getEfficiencyScaleFactorError returned CP::CorrectionCode::Error");
        return EL::StatusCode::FAILURE;
      }
      if(m_photonMediumEffTool_handle->getEfficiencyScaleFactorError(*photon, photonMediumEffSFError) == CP::CorrectionCode::Error){
        ANA_MSG_ERROR("getEfficiencyScaleFactorError returned CP::CorrectionCode::Error");
        return EL::StatusCode::FAILURE;
      }
      if(m_photonLooseEffTool_handle->getEfficiencyScaleFactorError(*photon, photonLooseEffSFError) == CP::CorrectionCode::Error){
        ANA_MSG_ERROR("getEfficiencyScaleFactorError returned CP::CorrectionCode::Error");
        return EL::StatusCode::FAILURE;
      }
    }

    photon->auxdecor< float >( "PhotonID_Tight_EffSF"  ) = photonTightEffSF;
    photon->auxdecor< float >( "PhotonID_Medium_EffSF" ) = photonMediumEffSF;
    photon->auxdecor< float >( "PhotonID_Loose_EffSF"  ) = photonLooseEffSF;

    photon->auxdecor< float >( "PhotonID_Tight_EffSF_Error"  ) = photonTightEffSFError;
    photon->auxdecor< float >( "PhotonID_Medium_EffSF_Error" ) = photonMediumEffSFError;
    photon->auxdecor< float >( "PhotonID_Loose_EffSF_Error"  ) = photonLooseEffSFError;

    ANA_MSG_DEBUG("Tight=" << photonTightEffSF << "(" << photonTightEffSFError << ")"
                  "Medium=" << photonMediumEffSF << "(" << photonMediumEffSFError << ")"
                  "Loose=" << photonLooseEffSF << "(" << photonLooseEffSFError << ")");
  }

  return EL::StatusCode::SUCCESS;
}
