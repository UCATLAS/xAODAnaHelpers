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

// this is needed to distribute the algorithm to the workers
ClassImp(TreeAlgo)

TreeAlgo :: TreeAlgo () :
    Algorithm("TreeAlgo")
{
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
  ANA_MSG_INFO( m_name );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // get the file we created already
  TFile* treeFile = wk()->getOutputFile ("tree");
  treeFile->mkdir(m_name.c_str());
  treeFile->cd(m_name.c_str());

  // to handle more than one jet collections (reco and truth)
  std::string token;
  std::istringstream ss_reco_containers(m_jetContainerName);
  while ( std::getline(ss_reco_containers, token, ' ') ){
    m_jetContainers.push_back(token);
  }
  std::istringstream ss_reco_names(m_jetBranchName);
  while ( std::getline(ss_reco_names, token, ' ') ){
    m_jetBranches.push_back(token);
  }
  if( !m_jetContainerName.empty() && m_jetContainers.size()!=m_jetBranches.size()){
    ANA_MSG_ERROR( "The number of jet containers must be equal to the number of jet name branches. Exiting");
    return EL::StatusCode::FAILURE;
  }
  std::istringstream ss_truth_containers(m_truthJetContainerName);
  while ( std::getline(ss_truth_containers, token, ' ') ){
    m_truthJetContainers.push_back(token);
  }
  std::istringstream ss_truth_names(m_truthJetBranchName);
  while ( std::getline(ss_truth_names, token, ' ') ){
    m_truthJetBranches.push_back(token);
  }
  if( !m_truthJetContainerName.empty() && m_truthJetContainers.size()!=m_truthJetBranches.size()){
    ANA_MSG_ERROR( "The number of truth jet containers must be equal to the number of truth jet name branches. Exiting");
    return EL::StatusCode::FAILURE;
  }

  // allow to store different variables for each jet collection (reco only, default: store the same)
  std::istringstream ss(m_jetDetailStr);
  while ( std::getline(ss, token, '|') ){
    m_jetDetails.push_back(token);
  }
  if( m_jetDetails.size()!=1  && m_jetContainers.size()!=m_jetDetails.size()){
    ANA_MSG_ERROR( "The size of m_jetContainers should be equal to the size of m_jetDetailStr. Exiting");
    return EL::StatusCode::FAILURE;
  }



  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: histInitialize ()
{
  ANA_MSG_INFO( m_name );
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TreeAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }


EL::StatusCode TreeAlgo :: execute ()
{

  // what systematics do we need to process for this event?
  // handle the nominal case (merge all) on every event, always
  std::vector<std::string> event_systNames({""});
  std::vector<std::string> muSystNames;
  std::vector<std::string> elSystNames;
  std::vector<std::string> jetSystNames;
  std::vector<std::string> photonSystNames;
  std::vector<std::string> fatJetSystNames;
  std::vector<std::string> metSystNames;

  // this is a temporary pointer that gets switched around to check each of the systematics
  std::vector<std::string>* systNames(nullptr);

  // note that the way we set this up, none of the below ##SystNames vectors contain the nominal case
  // TODO: do we really need to check for duplicates? Maybe, maybe not.
  if(!m_muSystsVec.empty()){
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_muSystsVec, 0, m_store, msg()) );
    for(const auto& systName: *systNames){
      muSystNames.push_back(systName);
      if (std::find(event_systNames.begin(), event_systNames.end(), systName) != event_systNames.end()) continue;
      event_systNames.push_back(systName);
    }
  }

  if(!m_elSystsVec.empty()){
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_elSystsVec, 0, m_store, msg()) );
    for(const auto& systName: *systNames){
      elSystNames.push_back(systName);
      if (std::find(event_systNames.begin(), event_systNames.end(), systName) != event_systNames.end()) continue;
      event_systNames.push_back(systName);
    }
  }

  if(!m_jetSystsVec.empty()){
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_jetSystsVec, 0, m_store, msg()) );
    for(const auto& systName: *systNames){
      jetSystNames.push_back(systName);
      if (std::find(event_systNames.begin(), event_systNames.end(), systName) != event_systNames.end()) continue;
      event_systNames.push_back(systName);
    }
  }
  if(!m_fatJetSystsVec.empty()){
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_fatJetSystsVec, 0, m_store, msg()) );
    for(const auto& systName: *systNames){
      fatJetSystNames.push_back(systName);
      if (std::find(event_systNames.begin(), event_systNames.end(), systName) != event_systNames.end()) continue;
      event_systNames.push_back(systName);
    }
  }
  if(!m_photonSystsVec.empty()){
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_photonSystsVec, 0, m_store, msg()) );
    for(const auto& systName: *systNames){
      photonSystNames.push_back(systName);
      if (std::find(event_systNames.begin(), event_systNames.end(), systName) != event_systNames.end()) continue;
      event_systNames.push_back(systName);
    }
  }
  if(!m_metSystsVec.empty()){
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_metSystsVec, 0, m_store, msg()) );
    for(const auto& systName: *systNames){
      metSystNames.push_back(systName);
      if (std::find(event_systNames.begin(), event_systNames.end(), systName) != event_systNames.end()) continue;
      event_systNames.push_back(systName);
    }
  }

  TFile* treeFile = wk()->getOutputFile ("tree");

  // let's make the tdirectory and ttrees
  for(const auto& systName: event_systNames){
    // check if we have already created the tree
    if(m_trees.find(systName) != m_trees.end()) continue;
    std::string treeName = systName;
    if(systName.empty()) treeName = "nominal";

    ANA_MSG_INFO( "Making tree " << m_name << "/" << treeName );
    TTree * outTree = new TTree(treeName.c_str(),treeName.c_str());
    if ( !outTree ) {
      ANA_MSG_ERROR("Failed to instantiate output tree!");
      return EL::StatusCode::FAILURE;
    }

    m_trees[systName] = new HelpTreeBase( m_event, outTree, treeFile, m_units, msgLvl(MSG::DEBUG), m_store );
    const auto& helpTree = m_trees[systName];

    // tell the tree to go into the file
    outTree->SetDirectory( treeFile->GetDirectory(m_name.c_str()) );
    // choose if want to add tree to same directory as ouput histograms
    if ( m_outHistDir ) {
      if(m_trees.size() > 1) ANA_MSG_WARNING( "You're running systematics! You may find issues in writing all of the output TTrees to the output histogram file... Set `m_outHistDir = false` if you run into issues!");
      wk()->addOutput( outTree );
    }

    // initialize all branch addresses since we just added this tree
    helpTree->AddEvent( m_evtDetailStr );
    if (!m_trigDetailStr.empty() )              { helpTree->AddTrigger(m_trigDetailStr);                           }
    if (!m_muContainerName.empty() )            { helpTree->AddMuons(m_muDetailStr);                               }
    if (!m_elContainerName.empty() )            { helpTree->AddElectrons(m_elDetailStr);                           }
    if (!m_jetContainerName.empty() )           {
      for(unsigned int ll=0; ll<m_jetContainers.size();++ll){
        if(m_jetDetails.size()==1) helpTree->AddJets       (m_jetDetailStr, m_jetBranches.at(ll).c_str());
	else{ helpTree->AddJets       (m_jetDetails.at(ll), m_jetBranches.at(ll).c_str()); }
      }
    }
    if (!m_l1JetContainerName.empty() )         { helpTree->AddL1Jets();                                           }
    if (!m_trigJetContainerName.empty() )       { helpTree->AddJets(m_trigJetDetailStr, "trigJet");                }
    if (!m_truthJetContainerName.empty() )      {
      for(unsigned int ll=0; ll<m_truthJetContainers.size();++ll){
        helpTree->AddJets       (m_truthJetDetailStr, m_truthJetBranches.at(ll).c_str());
      }
    }
    if ( !m_fatJetContainerName.empty() ) {
	    std::string token;
  	  std::istringstream ss(m_fatJetContainerName);
    	while ( std::getline(ss, token, ' ') ){
      	  helpTree->AddFatJets(m_fatJetDetailStr, token);
			}
		}
    if (!m_truthFatJetContainerName.empty() )   { helpTree->AddTruthFatJets(m_truthFatJetDetailStr);               }
    if (!m_tauContainerName.empty() )           { helpTree->AddTaus(m_tauDetailStr);                               }
    if (!m_METContainerName.empty() )           { helpTree->AddMET(m_METDetailStr);                                }
    if (!m_photonContainerName.empty() )        { helpTree->AddPhotons(m_photonDetailStr);                         }
    if (!m_truthParticlesContainerName.empty()) { helpTree->AddTruthParts("xAH_truth", m_truthParticlesDetailStr); }
    if (!m_trackParticlesContainerName.empty()) { helpTree->AddTrackParts(m_trackParticlesContainerName, m_trackParticlesDetailStr); }
  }

  /* THIS IS WHERE WE START PROCESSING THE EVENT AND PLOTTING THINGS */

  // Get EventInfo and the PrimaryVertices
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  const xAOD::VertexContainer* vertices(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, msg()) );
  // get the primaryVertex
  const xAOD::Vertex* primaryVertex = HelperFunctions::getPrimaryVertex( vertices , msg());

  for(const auto& systName: event_systNames){
    auto& helpTree = m_trees[systName];

    // assume the nominal container by default
    std::string muSuffix("");
    std::string elSuffix("");
    std::string jetSuffix("");
    std::string photonSuffix("");
    std::string fatJetSuffix("");
    std::string metSuffix("");
    /*
       if we find the systematic in the corresponding vector, we will use that container's systematic version instead of nominal version
        NB: since none of these contain the "" (nominal) case because of how I filter it, we handle the merging.. why?
        - in each loop to make the ##systNames vectors, we check to see if the systName exists in event_systNames which is initialized
        -   to {""} - the nominal case. If the systName exists, we do not add it to the corresponding ##systNames vector, otherwise, we do.
        -   This precludes the nominal case in all of the ##systNames vectors, which means the default will always be to run nominal.
    */
    if (std::find(muSystNames.begin(), muSystNames.end(), systName) != muSystNames.end()) muSuffix = systName;
    if (std::find(elSystNames.begin(), elSystNames.end(), systName) != elSystNames.end()) elSuffix = systName;
    if (std::find(jetSystNames.begin(), jetSystNames.end(), systName) != jetSystNames.end()) jetSuffix = systName;
    if (std::find(photonSystNames.begin(), photonSystNames.end(), systName) != photonSystNames.end()) photonSuffix = systName;
    if (std::find(fatJetSystNames.begin(), fatJetSystNames.end(), systName) != fatJetSystNames.end()) fatJetSuffix = systName;
    if (std::find(metSystNames.begin(), metSystNames.end(), systName) != metSystNames.end()) metSuffix = systName;

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
      if ( !m_store->contains<xAOD::MuonContainer>(m_muContainerName + muSuffix) ) continue;

      const xAOD::MuonContainer* inMuon(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inMuon, m_muContainerName+muSuffix, m_event, m_store, msg()) );
      helpTree->FillMuons( inMuon, primaryVertex );
    }

    if ( !m_elContainerName.empty() ) {
      if ( !m_store->contains<xAOD::ElectronContainer>(m_elContainerName + elSuffix) ) continue;

      const xAOD::ElectronContainer* inElec(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inElec, m_elContainerName+elSuffix, m_event, m_store, msg()) );
      helpTree->FillElectrons( inElec, primaryVertex );
    }

    if ( !m_jetContainerName.empty() ) {
      bool reject = false;
      for ( unsigned int ll = 0; ll < m_jetContainers.size(); ++ll ) { // Systs only for first jet container
        const xAOD::JetContainer* inJets(nullptr);
        if ( ll==0 ) {
          if ( !m_store->contains<xAOD::JetContainer>(m_jetContainers.at(ll)+jetSuffix) ) {
            reject = true;
            break;
          }
          ANA_CHECK( HelperFunctions::retrieve(inJets, m_jetContainers.at(ll)+jetSuffix, m_event, m_store, msg()) );
        } else {
          if ( !m_store->contains<xAOD::JetContainer>(m_jetContainers.at(ll)) ) {
            reject = true;
            break;
          }
          ANA_CHECK( HelperFunctions::retrieve(inJets, m_jetContainers.at(ll), m_event, m_store, msg()) );
        }

        helpTree->FillJets( inJets, HelperFunctions::getPrimaryVertexLocation(vertices, msg()), m_jetBranches.at(ll) );
      }
      
      if ( reject ) continue;
    }

    if ( !m_l1JetContainerName.empty() ){
      if ( !m_store->contains<xAOD::JetRoIContainer>(m_l1JetContainerName) ) continue;

      const xAOD::JetRoIContainer* inL1Jets(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inL1Jets, m_l1JetContainerName, m_event, m_store, msg()) );
      helpTree->FillL1Jets( inL1Jets);
    }

    if ( !m_trigJetContainerName.empty() ) {
      if ( !m_store->contains<xAOD::JetContainer>(m_trigJetContainerName) ) continue;

      const xAOD::JetContainer* inTrigJets(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inTrigJets, m_trigJetContainerName, m_event, m_store, msg()) );
      helpTree->FillJets( inTrigJets, HelperFunctions::getPrimaryVertexLocation(vertices, msg()), "trigJet" );
    }

    if ( !m_truthJetContainerName.empty() ) {
      bool reject = false;
      for ( unsigned int ll = 0; ll < m_truthJetContainers.size(); ++ll) {
        if ( !m_store->contains<xAOD::JetContainer>(m_truthJetContainers.at(ll)) ) {
          reject = true;
          break;
        }

        const xAOD::JetContainer* inTruthJets(nullptr);
        ANA_CHECK( HelperFunctions::retrieve(inTruthJets, m_truthJetContainers.at(ll), m_event, m_store, msg()) );
        helpTree->FillJets( inTruthJets, HelperFunctions::getPrimaryVertexLocation(vertices, msg()), m_truthJetBranches.at(ll) );
      }
      
      if ( reject ) continue;
    }

    if ( !m_fatJetContainerName.empty() ) {
      bool reject = false;
      std::string token;
      std::istringstream ss(m_fatJetContainerName);
      while ( std::getline(ss, token, ' ') ){
        if ( !m_store->contains<xAOD::JetContainer>(token+fatJetSuffix) ) {
          reject = true;
          break;
        }

      	const xAOD::JetContainer* inFatJets(nullptr);
	ANA_CHECK( HelperFunctions::retrieve(inFatJets, token+fatJetSuffix, m_event, m_store, msg()) );
      	helpTree->FillFatJets( inFatJets, token );
      }
      
      if ( reject ) continue;
    }

    if ( !m_truthFatJetContainerName.empty() ) {
      if ( !m_store->contains<xAOD::JetContainer>(m_truthFatJetContainerName) ) continue;

      const xAOD::JetContainer* inTruthFatJets(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inTruthFatJets, m_truthFatJetContainerName, m_event, m_store, msg()) );
      helpTree->FillTruthFatJets( inTruthFatJets );
    }

    if ( !m_tauContainerName.empty() ) {
      if ( !m_store->contains<xAOD::TauJetContainer>(m_tauContainerName) ) continue;

      const xAOD::TauJetContainer* inTaus(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inTaus, m_tauContainerName, m_event, m_store, msg()) );
      helpTree->FillTaus( inTaus );
    }

    if ( !m_METContainerName.empty() ) {
      if ( !m_store->contains<xAOD::MissingETContainer>(m_METContainerName + metSuffix) ) continue;

      const xAOD::MissingETContainer* inMETCont(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inMETCont, m_METContainerName, m_event, m_store, msg()) );
      helpTree->FillMET( inMETCont );
    }

    if ( !m_photonContainerName.empty() ) {
      if ( !m_store->contains<xAOD::PhotonContainer>(m_photonContainerName + photonSuffix) ) continue;

      const xAOD::PhotonContainer* inPhotons(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inPhotons, m_photonContainerName+photonSuffix, m_event, m_store, msg()) );
      helpTree->FillPhotons( inPhotons );
    }

    if ( !m_truthParticlesContainerName.empty() ) {
      if ( !m_store->contains<xAOD::TruthParticleContainer>(m_truthParticlesContainerName) ) continue;

      const xAOD::TruthParticleContainer* inTruthParticles(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inTruthParticles, m_truthParticlesContainerName, m_event, m_store, msg()));
      helpTree->FillTruth("xAH_truth", inTruthParticles);
    }

    if ( !m_trackParticlesContainerName.empty() ) {
      if ( !m_store->contains<xAOD::TrackParticleContainer>(m_trackParticlesContainerName) ) continue;

      const xAOD::TrackParticleContainer* inTrackParticles(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(inTrackParticles, m_trackParticlesContainerName, m_event, m_store, msg()));
      helpTree->FillTracks(m_trackParticlesContainerName, inTrackParticles);
    }

    // fill the tree
    helpTree->Fill();
  }

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode TreeAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TreeAlgo :: finalize () {

  ANA_MSG_INFO( "Deleting tree instances...");

  for(auto& item: m_trees){
    if(item.second) {delete item.second; item.second = nullptr; }
  }
  m_trees.clear();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
