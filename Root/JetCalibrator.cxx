/******************************************
 *
 * Interface to Jet calibration tool(s).
 *
 * G.Facini (gabriel.facini@cern.ch), M. Milesi (marco.milesi@cern.ch), J. Dandoy (jeff.dandoy@cern.ch)
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
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/Jet.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/JetCalibrator.h"

// ROOT includes:
#include "TSystem.h"

// tools
#include "JetCalibTools/JetCalibrationTool.h"
#include "JetUncertainties/JetUncertaintiesTool.h"
#include "JetSelectorTools/JetCleaningTool.h"
#include "JetMomentTools/JetVertexTaggerTool.h"
#include "JetTileCorrection/JetTileCorrectionTool.h"
#include "METUtilities/METHelpers.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetCalibrator)

JetCalibrator :: JetCalibrator () :
    Algorithm("JetCalibrator")
{
}

EL::StatusCode JetCalibrator :: setupJob (EL::Job& job)
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
  xAOD::Init( "JetCalibrator" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_INFO( "Initializing JetCalibrator Interface... ");
  m_runSysts = false; //Ensure this starts false

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  // If there is no InputContainer we must stop
  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  if ( m_outputAlgo.empty() ) {
    m_outputAlgo = m_jetAlgo + "_Calib_Algo";
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  if( isMC() && m_calibSequence.find("_Insitu") != std::string::npos){
    ANA_MSG_ERROR( "Attempting to use an Insitu calibration sequence on MC.  Exiting.");
    return EL::StatusCode::FAILURE;
  }

  if( !isMC() && m_calibSequence.find("_Smear") != std::string::npos){
    ANA_MSG_ERROR( "Attempting to use an Smear calibration sequence on data.  Exiting.");
    return EL::StatusCode::FAILURE;
  }

  if ( !isMC() ) {
    // Insitu should not be applied to the trimmed jets, per Jet/Etmiss recommendation
    if ( m_forceInsitu && m_calibSequence.find("Insitu") == std::string::npos) m_calibSequence += "_Insitu";

    m_calibConfig = m_calibConfigData;
  } else {

    if ( isFastSim() ) {
      m_calibConfig = m_calibConfigAFII;
    } else {
      m_calibConfig = m_calibConfigFullSim;
      // Insitu should not be applied to the trimmed jets, per Jet/Etmiss recommendation
      if ( m_forceSmear && m_calibSequence.find("Smear") == std::string::npos) m_calibSequence += "_Smear";
    }
  }

  if(m_uncertMCType.empty()) m_uncertMCType = isFastSim() ? "AFII" : "MC16";

  // Autoconfigure calibration sequence if the user didn't do it.
  // Recommended strings taken from ApplyJetCalibrationR21 Twiki.
  if(m_calibSequence.empty()){
    // Standard R=0.4 jets
    if(m_inContainerName.find("AntiKt4EM") != std::string::npos){
      if( !isMC() )           m_calibSequence = "JetArea_Residual_EtaJES_GSC_Insitu";
      else if( isFastSim() )  m_calibSequence = "JetArea_Residual_EtaJES_GSC";
      else /*FullSim*/        m_calibSequence = "JetArea_Residual_EtaJES_GSC_Smear";
    }
    // R-scan jets
    else if(m_inContainerName.find("AntiKt2LCTopo") != std::string::npos ||
            m_inContainerName.find("AntiKt6LCTopo") != std::string::npos)
      m_calibSequence = "JetArea_Residual_EtaJES_GSC";
    // R=1.0 jets
    else if(m_inContainerName.find("AntiKt10LCTopo")           != std::string::npos ||
            m_inContainerName.find("AntiKt10TrackCaloCluster") != std::string::npos)
      m_calibSequence = "EtaJES_JMS";
    // Anything else is unrecognized
    else{
      ANA_MSG_ERROR( "Cannot autoconfigure jet calibration sequence for collection " << m_systName);
      ANA_MSG_ERROR( "JetCalibrator::m_calibSequence needs to be set manually in configuration.");
      return EL::StatusCode::FAILURE;
    }
  }

  // Warn user if they're running standard jets without in-situ in data or smearing in MC
  if(m_inContainerName.find("AntiKt4EM") != std::string::npos){
    if(!isMC() && m_calibSequence.find("_Insitu") == std::string::npos)
      ANA_MSG_WARNING("Calibrating AntiKt4EM jets in data without the in-situ step. This is not recommended, make sure it's really what you want!");
    else if(!isFastSim() && m_calibSequence.find("_Smear") == std::string::npos)
      ANA_MSG_WARNING("Calibrating AntiKt4EM jets in fullsim without the smearing step. This is not recommended, make sure it's really what you want!");
  }

  // retrieve EventInfo to get sample information
  const xAOD::EventInfo* ei(nullptr);
  ANA_CHECK(HelperFunctions::retrieve(ei, m_eventInfoContainerName, m_event, m_store, msg()));

  // initialize jet calibration tool
  ANA_CHECK( ASG_MAKE_ANA_TOOL(m_JetCalibrationTool_handle, JetCalibrationTool));
  ANA_CHECK( m_JetCalibrationTool_handle.setProperty("JetCollection",m_jetAlgo));
  if(!m_calibConfigDir.empty()){
    ANA_CHECK( m_JetCalibrationTool_handle.setProperty("ConfigDir",m_calibConfigDir));
  }
  ANA_CHECK( m_JetCalibrationTool_handle.setProperty("ConfigFile",m_calibConfig));
  ANA_CHECK( m_JetCalibrationTool_handle.setProperty("CalibSequence",m_calibSequence));
  if ( !m_overrideCalibArea.empty() ) {
    ANA_MSG_WARNING("Overriding jet calibration area to " << m_overrideCalibArea);
    ANA_CHECK( m_JetCalibrationTool_handle.setProperty("CalibArea", m_overrideCalibArea));
  }
  ANA_CHECK( m_JetCalibrationTool_handle.setProperty("IsData",!isMC()));
  ANA_CHECK( m_JetCalibrationTool_handle.setProperty("OutputLevel", msg().level()));
  if ( m_jetCalibToolsDEV ) {
    ANA_CHECK( m_JetCalibrationTool_handle.setProperty("DEVmode", m_jetCalibToolsDEV));
  }
  ANA_CHECK( m_JetCalibrationTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_JetCalibrationTool_handle);

  // initialize jet tile correction tool
  if(m_doJetTileCorr && !isMC()){ // Jet Tile Correction should only be applied to data
    ANA_CHECK( ASG_MAKE_ANA_TOOL(m_JetTileCorrectionTool_handle, CP::JetTileCorrectionTool));
    ANA_CHECK( m_JetTileCorrectionTool_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_JetTileCorrectionTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_JetTileCorrectionTool_handle);
  }

  if(m_doCleaning){
    // initialize and configure the jet cleaning tool
    //------------------------------------------------

    ANA_CHECK( m_JetCleaningTool_handle.setProperty( "CutLevel", m_jetCleanCutLevel));
    ANA_CHECK( m_JetCleaningTool_handle.setProperty( "DoUgly", m_jetCleanUgly));
    ANA_CHECK( m_JetCleaningTool_handle.setProperty( "OutputLevel", msg().level() ));
    ANA_CHECK( m_JetCleaningTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_JetCleaningTool_handle);

    if( m_saveAllCleanDecisions ){
      m_decisionNames.push_back( "LooseBad" );
      m_decisionNames.push_back( "LooseBadUgly" );
      m_decisionNames.push_back( "LooseBadTrigger" );
      m_decisionNames.push_back( "TightBad" );
      m_decisionNames.push_back( "TightBadUgly" );

      for(unsigned int iD=0; iD < m_decisionNames.size() ; ++iD){
        asg::AnaToolHandle<IJetSelector> this_JetCleaningTool_handle("JetCleaningTool/JetCleaningTool_"+m_decisionNames.at(iD), this);
        if( m_decisionNames.at(iD).find("Ugly") != std::string::npos ){
          ANA_CHECK( this_JetCleaningTool_handle.setProperty( "CutLevel", m_decisionNames.at(iD).substr(0,m_decisionNames.at(iD).size()-4) ));
          ANA_CHECK( this_JetCleaningTool_handle.setProperty( "DoUgly", true));
        }else{
          ANA_CHECK( this_JetCleaningTool_handle.setProperty( "CutLevel", m_decisionNames.at(iD) ));
        }
        ANA_CHECK( this_JetCleaningTool_handle.setProperty( "OutputLevel", msg().level() ));
        ANA_CHECK( this_JetCleaningTool_handle.retrieve());
        ANA_MSG_DEBUG("Retrieved tool: " << this_JetCleaningTool_handle);
        m_AllJetCleaningTool_handles.push_back( this_JetCleaningTool_handle );
      }// For each cleaning decision
    }//If save all cleaning decisions
  }// if m_doCleaning

  // initialize largeR jet truth labelling tool
  if(isMC() && m_inContainerName.find("AntiKt10") != std::string::npos){
    // Truth labelling is required for systematics on largeR jets and is provided by the WZ tagger tool.
    // Since only the truth-labelling functionality is used, the tagger config is hard-coded as it does not matter.
    ANA_CHECK(m_SmoothedWZTagger_handle.setProperty("CalibArea" , "SmoothedWZTaggers/Rel21"));
    ANA_CHECK(m_SmoothedWZTagger_handle.setProperty("ConfigFile", "SmoothedContainedWTagger_AntiKt10LCTopoTrimmed_FixedSignalEfficiency50_MC16d_20190410.dat"));
    ANA_CHECK(m_SmoothedWZTagger_handle.setProperty("DSID", ei->mcChannelNumber()));
    ANA_CHECK(m_SmoothedWZTagger_handle.retrieve());
  }// if MC && largeR

  // Generate nominal systematic
  const CP::SystematicSet recSyst = CP::SystematicSet();;
  m_systList = HelperFunctions::getListofSystematics( recSyst, "", 0, msg() ); // Generate nominal

  // initialize and configure the jet uncertainity tool
  // only initialize if a config file has been given
  //------------------------------------------------
  if ( !m_uncertConfig.empty() && !m_systName.empty() && m_systName != "None" ) {

    if(m_mcAndPseudoData){
      ANA_MSG_INFO("Input treated as MC AND pseudo-data. JER uncertainties will be run twice.");
      initializeUncertaintiesTool(m_JetUncertaintiesTool_handle, false);
      // Need a second uncertainties tool to handle the pseudodata smearing
      initializeUncertaintiesTool(m_pseudodataJERTool_handle, true);
    }
    else if(m_pseudoData) {
      ANA_MSG_INFO("Input treated as pseudo-data");
      initializeUncertaintiesTool(m_JetUncertaintiesTool_handle, true);
    }
    else {
      initializeUncertaintiesTool(m_JetUncertaintiesTool_handle, !isMC());
    }

    //
    // Get a list of recommended systematics for this tool
    //
    ANA_MSG_INFO(" Initializing Jet Systematics :");
    const CP::SystematicSet recSysts = m_JetUncertaintiesTool_handle->recommendedSystematics();

    //If just one systVal, then push it to the vector
    ANA_CHECK( this->parseSystValVector());
    if( m_systValVector.size() == 0) {
      ANA_MSG_DEBUG("Pushing the following systVal to m_systValVector: " << m_systVal );
      m_systValVector.push_back(m_systVal);
    }

    for(unsigned int iSyst=0; iSyst < m_systValVector.size(); ++iSyst){
      std::vector<CP::SystematicSet> sysList = HelperFunctions::getListofSystematics( recSysts, m_systName, m_systValVector.at(iSyst), msg() );

      for(unsigned int i=0; i < sysList.size(); ++i){
        // do not add another nominal syst to the list!!
        // CP::SystematicSet() creates an empty systematic set, compared to the set at index i
        if (sysList.at(i).empty() || sysList.at(i) == CP::SystematicSet() ) { ANA_MSG_INFO("sysList Empty at index " << i); continue; }
        m_systList.push_back( sysList.at(i) );
      }
    }

    // Setup the tool for the 1st systematic on the list
    // If running all, the tool will be setup for each syst on each event
    if ( !m_systList.empty() ) {
      m_runSysts = true;
      // setup uncertainity tool for systematic evaluation
      if ( m_JetUncertaintiesTool_handle->applySystematicVariation(m_systList.at(0)) != CP::SystematicCode::Ok ) {
        ANA_MSG_ERROR( "Cannot configure JetUncertaintiesTool for systematic " << m_systName);
        return EL::StatusCode::FAILURE;
      }
    }
  } // running systematics
  else {
    ANA_MSG_INFO( "No Jet Uncertainities considered");
  }

  // initialize and configure the JVT correction tool

  if( m_redoJVT ){
    ANA_CHECK( m_JVTUpdateTool_handle.setProperty("JVTFileName", PathResolverFindCalibFile("JetMomentTools/JVTlikelihood_20140805.root")));

    if( ! m_JvtAuxName.empty() ){
      ANA_CHECK( m_JVTUpdateTool_handle.setProperty("JVFCorrName", m_JvtAuxName) )
    }
    ANA_CHECK( m_JVTUpdateTool_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_JVTUpdateTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_JVTUpdateTool_handle);
  }

  if ( m_calculatefJVT ) {
    ANA_CHECK(m_fJVTTool_handle.setProperty("CentralMaxPt", m_fJVTCentralMaxPt));
    if ( m_fJVTWorkingPoint == "Tight" ) {
      ANA_CHECK(m_fJVTTool_handle.setProperty("UseTightOP", true));
    }
    ANA_CHECK(m_fJVTTool_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK(m_fJVTTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_fJVTTool_handle);
  }

  auto SystJetsNames = std::make_unique< std::vector< std::string > >();
  for ( const auto& syst_it : m_systList ) {
    if ( m_systName.empty() && m_systName.empty() ) {
      ANA_MSG_INFO("\t Running w/ nominal configuration only!");
      break;
    }
    SystJetsNames->push_back(syst_it.name());
    ANA_MSG_INFO("\t " << syst_it.name());
  }

  ANA_CHECK(m_store->record(std::move(SystJetsNames), "jets_Syst"+m_name ));

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systList, m_name, fileMD);
  }

  ANA_MSG_INFO( "JetCalibrator Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetCalibrator :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG("Applying Jet Calibration and Cleaning... ");

  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::JetContainer* inJets(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, msg()) );

  //
  // Perform nominal calibration
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > calibJetsSC = xAOD::shallowCopyContainer( *inJets );
  // ConstDataVector<xAOD::JetContainer>* calibJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  // calibJetsCDV->reserve( calibJetsSC.first->size() );

  if ( m_addGhostMuonsToJets ) {
    ANA_MSG_VERBOSE("Run muon-to-jet ghost association");
    const xAOD::MuonContainer* muons(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(muons, "Muons", m_event, m_store, msg()) );
    met::addGhostMuonsToJets( *muons, *calibJetsSC.first );
  }

  std::string outSCContainerName=m_outContainerName+"ShallowCopy";
  std::string outSCAuxContainerName=m_outContainerName+"ShallowCopyAux.";
  ANA_CHECK( m_store->record( calibJetsSC.first,  outSCContainerName));
  ANA_CHECK( m_store->record( calibJetsSC.second, outSCAuxContainerName));

  for ( auto jet_itr : *(calibJetsSC.first) ) {
    m_numObject++;

    //
    // truth labelling for systematics
    if(isMC() && m_runSysts){

      // b-jet truth labelling
      int this_TruthLabel = 0;

      static SG::AuxElement::ConstAccessor<int> TruthLabelID ("TruthLabelID");
      static SG::AuxElement::ConstAccessor<int> PartonTruthLabelID ("PartonTruthLabelID");

      if ( TruthLabelID.isAvailable( *jet_itr) ) {
	this_TruthLabel = TruthLabelID( *jet_itr );
	if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
      } else if(PartonTruthLabelID.isAvailable( *jet_itr) ) {
	this_TruthLabel = PartonTruthLabelID( *jet_itr );
	if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
      }

      bool isBjet = false; // decide whether or not the jet is a b-jet (truth-labelling + kinematic selections)
      if (this_TruthLabel == 5) isBjet = true;
      static SG::AuxElement::Decorator<char> accIsBjet("IsBjet"); // char due to limitations of ROOT I/O, still treat it as a bool
      accIsBjet(*jet_itr) = isBjet;

      // largeR jet truth labelling
      if(m_SmoothedWZTagger_handle.isInitialized()) {
	m_SmoothedWZTagger_handle->decorateTruthLabel(*jet_itr);
      }
    }

    //
    // the calibration
    if ( m_JetCalibrationTool_handle->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
      ANA_MSG_ERROR( "JetCalibration tool reported a CP::CorrectionCode::Error");
      ANA_MSG_ERROR( m_name );
      return StatusCode::FAILURE;
    }

    if(m_doJetTileCorr && !isMC()){
      if( m_JetTileCorrectionTool_handle->applyCorrection(*jet_itr) == CP::CorrectionCode::Error ){
        ANA_MSG_ERROR( "JetTileCorrection tool reported a CP::CorrectionCode::Error");
      }
    }

  }//for jets

  // loop over available systematics - remember syst == "Nominal" --> baseline
  auto vecOutContainerNames = std::make_unique< std::vector< std::string > >();

  for ( const auto& syst_it : m_systList ) {

    executeSystematic(syst_it, inJets, calibJetsSC, *vecOutContainerNames, false);

    if(m_mcAndPseudoData && std::string(syst_it.name()).find("JER") != std::string::npos) {
      // This is a JER uncertainty that also needs a pseudodata copy done.
      executeSystematic(syst_it, inJets, calibJetsSC, *vecOutContainerNames, true);
    }
  }

  // add vector of systematic names to TStore
  ANA_CHECK( m_store->record( std::move(vecOutContainerNames), m_outputAlgo));

  // look what do we have in TStore

  if(msgLvl(MSG::VERBOSE)) m_store->print();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: finalize ()
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

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetCalibrator :: histFinalize ()
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

EL::StatusCode JetCalibrator::executeSystematic(const CP::SystematicSet& thisSyst, const xAOD::JetContainer* inJets,
                                                std::pair<xAOD::JetContainer*, xAOD::ShallowAuxContainer*>& calibJetsSC,
                                                std::vector<std::string>& vecOutContainerNames, bool isPDCopy){

  bool nominal = thisSyst.name().empty();

  std::string outSCContainerName, outSCAuxContainerName, outContainerName;
  asg::AnaToolHandle<ICPJetUncertaintiesTool>* jetUncTool(nullptr);

  // always append the name of the variation, including nominal which is an empty string
  if(isPDCopy){
    outSCContainerName    = m_outContainerName+thisSyst.name()+"_PDShallowCopy";
    outSCAuxContainerName = m_outContainerName+thisSyst.name()+"_PDShallowCopyAux.";
    outContainerName      = m_outContainerName+thisSyst.name()+"_PD";
    vecOutContainerNames.push_back(thisSyst.name()+"_PD");
    jetUncTool = &m_pseudodataJERTool_handle;
  }
  else{
    outSCContainerName    = m_outContainerName+thisSyst.name()+"ShallowCopy";
    outSCAuxContainerName = m_outContainerName+thisSyst.name()+"ShallowCopyAux.";
    outContainerName      = m_outContainerName+thisSyst.name();
    vecOutContainerNames.push_back(thisSyst.name());
    jetUncTool = &m_JetUncertaintiesTool_handle;
  }

  // create shallow copy;
  std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > uncertCalibJetsSC = nominal ? calibJetsSC : xAOD::shallowCopyContainer(*calibJetsSC.first);
  ConstDataVector<xAOD::JetContainer>* uncertCalibJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  uncertCalibJetsCDV->reserve( uncertCalibJetsSC.first->size() );

  //Apply Uncertainties
  if ( m_runSysts ) {
    // Jet Uncertainty Systematic
    ANA_MSG_DEBUG("Configure for systematic variation : " << thisSyst.name());
    if ( (*jetUncTool)->applySystematicVariation(thisSyst) != CP::SystematicCode::Ok ) {
      ANA_MSG_ERROR( "Cannot configure JetUncertaintiesTool for systematic " << m_systName);
      return EL::StatusCode::FAILURE;
    }

    for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {
      if (m_applyFatJetPreSel) {
        bool validForJES = (jet_itr->pt() >= 150e3 && jet_itr->pt() < 3000e3);
        validForJES &= (jet_itr->m()/jet_itr->pt() >= 0 && jet_itr->m()/jet_itr->pt() < 1);
        validForJES &= (fabs(jet_itr->eta()) < 2);
        if (!validForJES) continue;
      }

      if ( (*jetUncTool)->applyCorrection( *jet_itr ) == CP::CorrectionCode::Error ) {
        ANA_MSG_ERROR( "JetUncertaintiesTool reported a CP::CorrectionCode::Error");
        ANA_MSG_ERROR( m_name );
      }
    }

  }// if m_runSysts

  if(m_doCleaning){
    // decorate with cleaning decision
    for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {

      static SG::AuxElement::Decorator< int > isCleanDecor( "cleanJet" );
      const xAOD::Jet* jetToClean = jet_itr;

      if(m_cleanParent){
        ElementLink<xAOD::JetContainer> el_parent = jet_itr->auxdata<ElementLink<xAOD::JetContainer> >("Parent") ;
        if(!el_parent.isValid())
          ANA_MSG_ERROR( "Could not make jet cleaning decision on the parent! It doesn't exist.");
        else
          jetToClean = *el_parent;
      }

      isCleanDecor(*jet_itr) = m_JetCleaningTool_handle->keep(*jetToClean);

      if( m_saveAllCleanDecisions ){
        for(unsigned int i=0; i < m_AllJetCleaningTool_handles.size() ; ++i){
          jet_itr->auxdata< int >(("clean_pass"+m_decisionNames.at(i)).c_str()) = m_AllJetCleaningTool_handles.at(i)->keep(*jetToClean);
        }
      }
    } //end cleaning decision
  }

  if ( !xAOD::setOriginalObjectLink(*inJets, *(uncertCalibJetsSC.first)) ) {
    ANA_MSG_ERROR( "Failed to set original object links -- MET rebuilding cannot proceed.");
  }

  // Recalculate JVT using calibrated Jets
  if(m_redoJVT){
    for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {
      jet_itr->auxdata< float >("Jvt") = m_JVTUpdateTool_handle->updateJvt(*jet_itr);
    }
  }

  // Calculate fJVT using calibrated Jets
  if ( m_calculatefJVT ) {
    m_fJVTTool_handle->modify(*(uncertCalibJetsSC.first));
  }

  // save pointers in ConstDataVector with same order
  for ( auto jet_itr : *(uncertCalibJetsSC.first) ) {
    uncertCalibJetsCDV->push_back( jet_itr );
  }

  // can only sort the CDV - a bit no-no to sort the shallow copies
  if ( m_sort ) {
    std::sort( uncertCalibJetsCDV->begin(), uncertCalibJetsCDV->end(), HelperFunctions::sort_pt );
  }

  // add shallow copy to TStore
  if(!nominal){ // nominal is always saved outside of systematics loop
    ANA_CHECK( m_store->record( uncertCalibJetsSC.first, outSCContainerName));
    ANA_CHECK( m_store->record( uncertCalibJetsSC.second, outSCAuxContainerName));
  }
  // add ConstDataVector to TStore
  ANA_CHECK( m_store->record( uncertCalibJetsCDV, outContainerName));
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetCalibrator::initializeUncertaintiesTool(asg::AnaToolHandle<ICPJetUncertaintiesTool>& uncToolHandle, bool isData){

  ANA_MSG_INFO("Initialize Jet Uncertainties Tool with " << m_uncertConfig);
  ANA_CHECK( ASG_MAKE_ANA_TOOL(uncToolHandle, JetUncertaintiesTool));
  ANA_CHECK( uncToolHandle.setProperty("JetDefinition",m_jetAlgo));
  ANA_CHECK( uncToolHandle.setProperty("MCType",m_uncertMCType));
  ANA_CHECK( uncToolHandle.setProperty("IsData",isData));
  ANA_CHECK( uncToolHandle.setProperty("ConfigFile", m_uncertConfig));
  if ( !m_overrideUncertCalibArea.empty() ) {
    ANA_MSG_WARNING("Overriding jet uncertainties calibration area to " << m_overrideUncertCalibArea);
    ANA_CHECK( uncToolHandle.setProperty("CalibArea", m_overrideUncertCalibArea));
  }
  if( !m_overrideAnalysisFile.empty() ) {
     ANA_MSG_WARNING("Overriding jet uncertainties analysis file to " << m_overrideAnalysisFile);
     ANA_CHECK( uncToolHandle.setProperty("AnalysisFile", m_overrideAnalysisFile));
  }
  if( !m_overrideUncertPath.empty() ){
    std::string uncPath = PathResolverFindCalibDirectory(m_overrideUncertPath);
    ANA_MSG_WARNING("Overriding jet uncertainties path to " << uncPath);
    ANA_CHECK( uncToolHandle.setProperty("Path", uncPath));
  }
  ANA_CHECK( uncToolHandle.setProperty("OutputLevel", msg().level()));
  ANA_CHECK( uncToolHandle.retrieve());
  ANA_MSG_DEBUG("Retrieved JetUncertaintiesTool: " << uncToolHandle);

  return EL::StatusCode::SUCCESS;
}
