#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

#include <xAODJet/JetContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>
#include <xAODEgamma/PhotonContainer.h>

#include <xAODAnaHelpers/TreeAlgo.h>

#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(TreeAlgo)

TreeAlgo :: TreeAlgo (std::string className) :
    Algorithm(className),
    m_trees({}),
    m_muSystNames({}),
    m_elSystNames({}),
    m_jetSystNames({}),
    m_photonSystNames({})
{
  this->SetName("TreeAlgo"); // needed if you want to retrieve this algo with wk()->getAlg(ALG_NAME) downstream

  m_evtDetailStr            = "";
  m_trigDetailStr           = "";
  m_trigJetDetailStr        = "";
  m_truthJetDetailStr       = "";
  m_muDetailStr             = "";
  m_elDetailStr             = "";
  m_jetDetailStr            = "";
  m_fatJetDetailStr         = "";
  m_tauDetailStr            = "";
  m_METDetailStr            = "";
  m_photonDetailStr         = "";

  m_debug                   = false;

  m_outHistDir              = false;

  m_muContainerName         = "";
  m_elContainerName         = "";
  m_jetContainerName        = "";
  m_fatJetContainerName     = "";
  m_tauContainerName        = "";
  m_METContainerName        = "";
  m_photonContainerName     = "";

  m_muSystsVec              = "";
  m_elSystsVec              = "";
  m_jetSystsVec             = "";
  m_photonSystsVec          = "";

  // DC14 switch for little things that need to happen to run
  // for those samples with the corresponding packages
  m_DC14                    = false;

  //Units, defaulting to GeV
  m_units                   = 1e3;


}

EL::StatusCode TreeAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("TreeAlgo").ignore();

  EL::OutputStream outForTree("tree");
  job.outputAdd (outForTree);

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: initialize ()
{
  Info("initialize()", m_name.c_str());
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  return this->treeInitialize();
}

EL::StatusCode TreeAlgo :: treeInitialize ()
{
  Info("treeInitialize()", "%s", m_name.c_str() );
  // needed here and not in initalize since this is called first

  // make a tree for every systematic we need to process
  std::vector<std::string> all_systNames;
  all_systNames.push_back(""); // handle the nominal case (merge all) specially

  // temporarily hold vector retrieved
  std::vector<std::string>* systNames(nullptr);

  // note that the way we set this up, none of the below m_##SystNames vectors contain the nominal case
  if(!m_muSystsVec.empty()){
    RETURN_CHECK("TreeAlgo::treeInitialize()", HelperFunctions::retrieve(systNames, m_muSystsVec, 0, m_store, m_verbose) ,"");
    for(const auto& systName: *systNames){
      if (std::find(all_systNames.begin(), all_systNames.end(), systName) != all_systNames.end()) continue;
      all_systNames.push_back(systName);
      m_muSystNames.push_back(systName);
    }
  }

  if(!m_elSystsVec.empty()){
    RETURN_CHECK("TreeAlgo::treeInitialize()", HelperFunctions::retrieve(systNames, m_elSystsVec, 0, m_store, m_verbose) ,"");
    for(const auto& systName: *systNames){
      if (std::find(all_systNames.begin(), all_systNames.end(), systName) != all_systNames.end()) continue;
      all_systNames.push_back(systName);
      m_elSystNames.push_back(systName);
    }
  }

  if(!m_jetSystsVec.empty()){
    RETURN_CHECK("TreeAlgo::treeInitialize()", HelperFunctions::retrieve(systNames, m_jetSystsVec, 0, m_store, m_verbose) ,"");
    for(const auto& systName: *systNames){
      if (std::find(all_systNames.begin(), all_systNames.end(), systName) != all_systNames.end()) continue;
      all_systNames.push_back(systName);
      m_jetSystNames.push_back(systName);
    }
  }

  if(!m_photonSystsVec.empty()){
    RETURN_CHECK("TreeAlgo::treeInitialize()", HelperFunctions::retrieve(systNames, m_photonSystsVec, 0, m_store, m_verbose) ,"");
    for(const auto& systName: *systNames){
      if (std::find(all_systNames.begin(), all_systNames.end(), systName) != all_systNames.end()) continue;
      all_systNames.push_back(systName);
      m_photonSystNames.push_back(systName);
    }
  }

  // get the file we created already
  TFile* treeFile = wk()->getOutputFile ("tree");
  treeFile->mkdir(m_name.c_str());

  // let's make the tdirectory and ttrees
  for(const auto& systName: all_systNames){
    std::string treeName = systName;
    if(systName.empty()) treeName = "nominal";

    Info("treeInitialize()", "Making tree %s", treeName.c_str());
    TTree * outTree = new TTree(treeName.c_str(),treeName.c_str());
    if ( !outTree ) {
      Error("treeInitialize()","Failed to instantiate output tree!");
      return EL::StatusCode::FAILURE;
    }

    m_trees[systName] = new HelpTreeBase( m_event, outTree, treeFile, m_units, m_debug, m_DC14 );
    const auto& helpTree = m_trees[systName];

    // tell the tree to go into the file
    outTree->SetDirectory( treeFile->GetDirectory(m_name.c_str()) );
    // choose if want to add tree to same directory as ouput histograms
    if ( m_outHistDir ) {
      if(m_trees.size() > 1) Warning("treeInitialize()", "You're running systematics! You may find issues in writing all of the output TTrees to the output histogram file... Set `m_outHistDir = false` if you run into issues!");
      wk()->addOutput( outTree );
    }

    helpTree->AddEvent( m_evtDetailStr );
    if ( !m_trigDetailStr.empty() )       {   helpTree->AddTrigger    (m_trigDetailStr);    }
    if ( !m_muContainerName.empty() )     {   helpTree->AddMuons      (m_muDetailStr);      }
    if ( !m_elContainerName.empty() )     {   helpTree->AddElectrons  (m_elDetailStr);      }
    if ( !m_jetContainerName.empty() )    {   helpTree->AddJets       (m_jetDetailStr, "jet");     }
    if ( !m_trigJetContainerName.empty() ){   helpTree->AddJets       (m_trigJetDetailStr, "trigJet");     }
    if ( !m_truthJetContainerName.empty() ){  helpTree->AddJets       (m_truthJetDetailStr, "truthJet");     }
    if ( !m_fatJetContainerName.empty() ) {   helpTree->AddFatJets    (m_fatJetDetailStr);  }
    if ( !m_tauContainerName.empty() )    {   helpTree->AddTaus       (m_tauDetailStr);     }
    if ( !m_METContainerName.empty() )    {   helpTree->AddMET        (m_METDetailStr);     }
    if ( !m_photonContainerName.empty() ) {   helpTree->AddPhotons    (m_photonDetailStr);  }
  }

  Info("treeInitialize()", "Successfully initialized output tree");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TreeAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }


EL::StatusCode TreeAlgo :: execute ()
{

  // Get EventInfo and the PrimaryVertices
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");
  const xAOD::VertexContainer* vertices(nullptr);
  RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  // get the primaryVertex
  const xAOD::Vertex* primaryVertex = HelperFunctions::getPrimaryVertex( vertices );

  for(auto& item: m_trees){
    auto& systName = item.first;
    auto& helpTree = item.second;

    // assume the nominal container by default
    std::string muSuffix("");
    std::string elSuffix("");
    std::string jetSuffix("");
    std::string photonSuffix("");
    // if we find the systematic in the corresponding vector, we will use that container's systematic version instead of nominal version
    if (std::find(m_muSystNames.begin(), m_muSystNames.end(), systName) != m_muSystNames.end()) muSuffix = systName;
    if (std::find(m_elSystNames.begin(), m_elSystNames.end(), systName) != m_elSystNames.end()) elSuffix = systName;
    if (std::find(m_jetSystNames.begin(), m_jetSystNames.end(), systName) != m_jetSystNames.end()) jetSuffix = systName;
    if (std::find(m_photonSystNames.begin(), m_photonSystNames.end(), systName) != m_photonSystNames.end()) photonSuffix = systName;

    helpTree->FillEvent( eventInfo, m_event );

    // Fill trigger information
    if ( !m_trigDetailStr.empty() )    {
      helpTree->FillTrigger( eventInfo );
    }

    // Fill jet trigger information - this can be used if with layer/cleaning info we need to turn off some variables?
    /*if ( !m_trigJetDetailStr.empty() ) {
      helpTree->FillJetTrigger();
    }*/

    // for the containers the were supplied, fill the appropriate vectors
    if ( !m_muContainerName.empty() ) {
      const xAOD::MuonContainer* inMuon(nullptr);
      RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inMuon, m_muContainerName, m_event, m_store, m_verbose) ,"");
      helpTree->FillMuons( inMuon, primaryVertex );
    }

    if ( !m_elContainerName.empty() ) {
      const xAOD::ElectronContainer* inElec(nullptr);
      RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inElec, m_elContainerName, m_event, m_store, m_verbose) ,"");
      helpTree->FillElectrons( inElec, primaryVertex );
    }
    if ( !m_jetContainerName.empty() ) {
      const xAOD::JetContainer* inJets(nullptr);
      RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inJets, m_jetContainerName, m_event, m_store, m_verbose) ,"");
      helpTree->FillJets( inJets, HelperFunctions::getPrimaryVertexLocation(vertices), "jet" );
    }
    if ( !m_trigJetContainerName.empty() ) {
      const xAOD::JetContainer* inTrigJets(nullptr);
      RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inTrigJets, m_trigJetContainerName, m_event, m_store, m_verbose) ,"");
      helpTree->FillJets( inTrigJets, HelperFunctions::getPrimaryVertexLocation(vertices), "trigJet" );
    }
    if ( !m_truthJetContainerName.empty() ) {
      const xAOD::JetContainer* inTruthJets(nullptr);
      RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inTruthJets, m_truthJetContainerName, m_event, m_store, m_verbose) ,"");
          helpTree->FillJets( inTruthJets, HelperFunctions::getPrimaryVertexLocation(vertices), "truthJet" );
    }
    if ( !m_fatJetContainerName.empty() ) {
      const xAOD::JetContainer* inFatJets(nullptr);
      RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inFatJets, m_fatJetContainerName, m_event, m_store, m_verbose) ,"");
      helpTree->FillFatJets( inFatJets );
    }
    if ( !m_tauContainerName.empty() ) {
      const xAOD::TauJetContainer* inTaus(nullptr);
      RETURN_CHECK("HTopMultilepTreeAlgo::execute()", HelperFunctions::retrieve(inTaus, m_tauContainerName, m_event, m_store, m_verbose) , "");
      helpTree->FillTaus( inTaus );
    }
    if ( !m_METContainerName.empty() ) {
      const xAOD::MissingETContainer* inMETCont(nullptr);
      RETURN_CHECK("HTopMultilepTreeAlgo::execute()", HelperFunctions::retrieve(inMETCont, m_METContainerName, m_event, m_store, m_debug) , "");
      helpTree->FillMET( inMETCont );
    }
    if ( !m_photonContainerName.empty() ) {
      const xAOD::PhotonContainer* inPhotons(nullptr);
      RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inPhotons, m_photonContainerName, m_event, m_store, m_verbose) ,"");
      helpTree->FillPhotons( inPhotons );
    }

    // fill the tree
    helpTree->Fill();

  }

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode TreeAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TreeAlgo :: finalize () {

  Info("finalize()", "Deleting tree instances...");

  for(auto& item: m_trees){
    if(item.second) {delete item.second; item.second = nullptr; }
  }
  m_trees.clear();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: treeFinalize () { return EL::StatusCode::SUCCESS; }
