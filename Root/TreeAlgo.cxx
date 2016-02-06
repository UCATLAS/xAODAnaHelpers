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

#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(TreeAlgo)

TreeAlgo :: TreeAlgo (std::string className) :
    Algorithm(className),
    m_helpTree(nullptr)
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

  this->treeInitialize();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: treeInitialize ()
{
  Info("treeInitialize()", "%s", m_name.c_str() );
  // needed here and not in initalize since this is called first

  TTree * outTree = new TTree(m_name.c_str(),m_name.c_str());
  if ( !outTree ) {
    Error("treeInitialize()","Failed to instantiate output tree!");
    return EL::StatusCode::FAILURE;
  }

  // get the file we created already
  TFile* treeFile = wk()->getOutputFile ("tree");

  m_helpTree = new HelpTreeBase( m_event, outTree, treeFile, m_units, m_debug, m_DC14 );

  // tell the tree to go into the file
  outTree->SetDirectory( treeFile );
  // choose if want to add tree to same directory as ouput histograms
  if ( m_outHistDir ) {
    wk()->addOutput( outTree );
  }

  m_helpTree->AddEvent( m_evtDetailStr );

  if ( !m_trigDetailStr.empty() )       {   m_helpTree->AddTrigger    (m_trigDetailStr);    }
  if ( !m_muContainerName.empty() )     {   m_helpTree->AddMuons      (m_muDetailStr);      }
  if ( !m_elContainerName.empty() )     {   m_helpTree->AddElectrons  (m_elDetailStr);      }
  if ( !m_jetContainerName.empty() )    {   m_helpTree->AddJets       (m_jetDetailStr, "jet");     }
  if ( !m_trigJetContainerName.empty() ){   m_helpTree->AddJets       (m_trigJetDetailStr, "trigJet");     }
  if ( !m_truthJetContainerName.empty() ){   m_helpTree->AddJets       (m_truthJetDetailStr, "truthJet");     }
  if ( !m_fatJetContainerName.empty() ) {   m_helpTree->AddFatJets    (m_fatJetDetailStr);  }
  if ( !m_tauContainerName.empty() )    {   m_helpTree->AddTaus       (m_tauDetailStr);     }
  if ( !m_METContainerName.empty() )    {   m_helpTree->AddMET        (m_METDetailStr);     }
  if ( !m_photonContainerName.empty() ) {   m_helpTree->AddPhotons    (m_photonDetailStr);  }

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

  m_helpTree->FillEvent( eventInfo, m_event );

  // Fill trigger information
  if ( !m_trigDetailStr.empty() )    {
    m_helpTree->FillTrigger( eventInfo );
  }

  // Fill jet trigger information - this can be used if with layer/cleaning info we need to turn off some variables?
  /*if ( !m_trigJetDetailStr.empty() ) {
    m_helpTree->FillJetTrigger();
  }*/

  // for the containers the were supplied, fill the appropriate vectors
  if ( !m_muContainerName.empty() ) {
    const xAOD::MuonContainer* inMuon(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inMuon, m_muContainerName, m_event, m_store, m_verbose) ,"");
    m_helpTree->FillMuons( inMuon, primaryVertex );
  }

  if ( !m_elContainerName.empty() ) {
    const xAOD::ElectronContainer* inElec(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inElec, m_elContainerName, m_event, m_store, m_verbose) ,"");
    m_helpTree->FillElectrons( inElec, primaryVertex );
  }
  if ( !m_jetContainerName.empty() ) {
    const xAOD::JetContainer* inJets(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inJets, m_jetContainerName, m_event, m_store, m_verbose) ,"");
    m_helpTree->FillJets( inJets, HelperFunctions::getPrimaryVertexLocation(vertices), "jet" );
  }
  if ( !m_trigJetContainerName.empty() ) {
    const xAOD::JetContainer* inTrigJets(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inTrigJets, m_trigJetContainerName, m_event, m_store, m_verbose) ,"");
    m_helpTree->FillJets( inTrigJets, HelperFunctions::getPrimaryVertexLocation(vertices), "trigJet" );
  }
  if ( !m_truthJetContainerName.empty() ) {
    const xAOD::JetContainer* inTruthJets(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inTruthJets, m_truthJetContainerName, m_event, m_store, m_verbose) ,"");
        m_helpTree->FillJets( inTruthJets, HelperFunctions::getPrimaryVertexLocation(vertices), "truthJet" );
  }
  if ( !m_fatJetContainerName.empty() ) {
    const xAOD::JetContainer* inFatJets(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inFatJets, m_fatJetContainerName, m_event, m_store, m_verbose) ,"");
    m_helpTree->FillFatJets( inFatJets );
  }
  if ( !m_tauContainerName.empty() ) {
    const xAOD::TauJetContainer* inTaus(nullptr);
    RETURN_CHECK("HTopMultilepTreeAlgo::execute()", HelperFunctions::retrieve(inTaus, m_tauContainerName, m_event, m_store, m_verbose) , "");
    m_helpTree->FillTaus( inTaus );
  }
  if ( !m_METContainerName.empty() ) {
    const xAOD::MissingETContainer* inMETCont(nullptr);
    RETURN_CHECK("HTopMultilepTreeAlgo::execute()", HelperFunctions::retrieve(inMETCont, m_METContainerName, m_event, m_store, m_debug) , "");
    m_helpTree->FillMET( inMETCont );
  }
  if ( !m_photonContainerName.empty() ) {
    const xAOD::PhotonContainer* inPhotons(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inPhotons, m_photonContainerName, m_event, m_store, m_verbose) ,"");
    m_helpTree->FillPhotons( inPhotons );
  }

  // fill the tree
  m_helpTree->Fill();

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode TreeAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TreeAlgo :: finalize () {

  Info("finalize()", "Deleting tree instances...");

  if ( m_helpTree ) { delete m_helpTree;   m_helpTree = nullptr; }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: treeFinalize () { return EL::StatusCode::SUCCESS; }
