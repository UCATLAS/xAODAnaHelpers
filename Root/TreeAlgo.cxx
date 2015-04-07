#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

#include <xAODJet/JetContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODAnaHelpers/TreeAlgo.h>

#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(TreeAlgo)

TreeAlgo :: TreeAlgo () {}

TreeAlgo :: TreeAlgo (const std::string name,const std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_helpTree(nullptr)
{
  this->SetName("TreeAlgo"); // needed if you want to retrieve this algo with wk()->getAlg(ALG_NAME) downstream
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
  Info("treeInitialize()", "Attempting to configure using: %s", m_configName.c_str());

  //if ( this->configure() == EL::StatusCode::FAILURE ) {
  //Error("treeInitialize()", "%s failed to properly configure. Exiting.", m_name.c_str() );
  //return EL::StatusCode::FAILURE;
  //} else {
  //Info("treeInitialize()", "Succesfully configured! ");
  //}

  TTree * outTree = new TTree(m_name.c_str(),m_name.c_str());
  if( !outTree ) {
    Error("treeInitialize()","Failed to instantiate output tree!");
    return EL::StatusCode::FAILURE;
  }

  // get the file we created already
  TFile* treeFile = wk()->getOutputFile ("tree");
  m_helpTree = new HelpTreeBase( m_event, outTree, treeFile );
  // tell the tree to go into the file
  outTree->SetDirectory( treeFile );
  // if want to add to same file as ouput histograms
  // wk()->addOutput( outTree );

// get the input from user which determines which branches are created!
  this->configure();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: configure ()
{
  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG("TreeAlgo::configure()", m_configName);

  // the file exists, use TEnv to read it off
  TEnv* config = new TEnv(m_configName.c_str());
  m_evtDetailStr            = config->GetValue("EventDetailStr",       "");
  m_muDetailStr             = config->GetValue("MuonDetailStr",        "");
  m_elDetailStr             = config->GetValue("ElectronDetailStr",    "");
  m_jetDetailStr            = config->GetValue("JetDetailStr",         "");
  m_fatJetDetailStr         = config->GetValue("FatJetDetailStr",      "");
  m_debug                   = config->GetValue("Debug" ,           false );

  m_muContainerName         = config->GetValue("MuonContainerName",       "");
  m_elContainerName         = config->GetValue("ElectronContainerName",   "");
  m_jetContainerName        = config->GetValue("JetContainerName",        "");
  m_fatJetContainerName     = config->GetValue("FatJetContainerName",     "");

  m_helpTree->AddEvent    (m_evtDetailStr);

  if( !m_muContainerName.empty() )     {   m_helpTree->AddMuons    (m_muDetailStr);      }
  if( !m_elContainerName.empty() )     {   m_helpTree->AddElectrons(m_elDetailStr);      }
  if( !m_jetContainerName.empty() )    {   m_helpTree->AddJets     (m_jetDetailStr);     }
  if( !m_fatJetContainerName.empty() ) {   m_helpTree->AddFatJets  (m_fatJetDetailStr);  }

  Info("configure()", "Loaded in configuration values");

  // everything seems preliminarily ok, let's print config and say we were successful
  config->Print();
  delete config;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode TreeAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }



EL::StatusCode TreeAlgo :: execute ()
{
  // Get EventInfo the PrimaryVertices
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, m_debug) ,"");
  const xAOD::VertexContainer* vertices(nullptr);
  RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_debug) ,"");

  m_helpTree->FillEvent( eventInfo );

  // for the containers the were supplied, fill the appropiate vectors
  if(!m_muContainerName.empty()) {
    const xAOD::MuonContainer* inMuon(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inMuon, m_muContainerName, m_event, m_store, m_debug) ,"");
    m_helpTree->FillMuons( *inMuon );
  }

  if(!m_elContainerName.empty()) {
    const xAOD::ElectronContainer* inElec(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inElec, m_elContainerName, m_event, m_store, m_debug) ,"");
    m_helpTree->FillElectrons( *inElec );
  }

  if(!m_jetContainerName.empty()) {
    const xAOD::JetContainer* inJets(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inJets, m_jetContainerName, m_event, m_store, m_debug) ,"");
    m_helpTree->FillJets( *inJets, HelperFunctions::getPrimaryVertexLocation(vertices) );
  }

  if(!m_fatJetContainerName.empty()) {
    const xAOD::JetContainer* inFatJets(nullptr);
    RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(inFatJets, m_fatJetContainerName, m_event, m_store, m_debug) ,"");
    m_helpTree->FillFatJets( *inFatJets );
  }

  // fill the tree
  m_helpTree->Fill();

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode TreeAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TreeAlgo :: finalize () {
  if(m_helpTree){
    delete  m_helpTree;
    m_helpTree = 0;
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TreeAlgo :: treeFinalize () { return EL::StatusCode::SUCCESS; }
