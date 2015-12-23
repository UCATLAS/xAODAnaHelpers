/******************************************
 *
 * This class gets HLT jets from the TDT and can be expanded to get other features
 *
 * Merlin Davies (merlin.davies@cern.ch)
 * Caterina Doglioni (caterina.doglioni@cern.ch)
 * John Alison (john.alison@cern.ch)
 *
 *
 ******************************************/

// c++ include(s):
#include <iostream>
#include <vector>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
//CD: do we need all this stuff?
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODJet/Jet.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HLTJetGetter.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using std::cout;  using std::endl;
using std::vector;

// this is needed to distribute the algorithm to the workers
ClassImp(HLTJetGetter)

HLTJetGetter :: HLTJetGetter (std::string className) :
    Algorithm(className),
    //m_trigItem(""),
    m_triggerList(".*"),
    m_inContainerName(""),
    m_outContainerName(""),
    m_trigDecTool(nullptr)
{
  Info("HLTJetGetter()", "Calling constructor");

  // read debug flag from .config file
  m_debug                   = false;
    
  //m_sort                    = true;

}


EL::StatusCode HLTJetGetter :: setupJob (EL::Job& job)
{
  Info("setupJob()", "Calling setupJob");
  job.useXAOD ();
  xAOD::Init( "HLTJetGetter" ).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: histInitialize ()
{
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: changeInput (bool /*firstFile*/)
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode  HLTJetGetter :: configure ()
{
    if ( !getConfig().empty() ) {
        
        Info("configure()", "Configuring JetCalibrator Interface. User configuration read from : %s ", getConfig().c_str());
        
        TEnv* config = new TEnv(getConfig(true).c_str());
        
        // read debug flag from .config file
        m_debug                   = config->GetValue("Debug" , m_debug);
        // input container to be read from TDT (will be stripped - TODO: put here stripped name already?)
        m_inContainerName         = config->GetValue("InputContainer",  m_inContainerName.c_str());
        // output container is passed on with this output container name
        m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());
        // list of triggers whose features have to be pulled from TDT: ".*" for all
        m_triggerList             = config->GetValue("TriggerList", m_triggerList.c_str());

        config->Print();
        
        delete config; config = nullptr;
    }
    
    // If there is no InputContainer we must stop
    if ( m_inContainerName.empty() ) {
        Error("configure()", "InputContainer is empty!");
        return EL::StatusCode::FAILURE;
    }
    
    //Needed for later?
    //m_outSCContainerName      = m_outContainerName + "ShallowCopy";
    //m_outSCAuxContainerName   = m_outSCContainerName + "Aux."; // the period is very important!
    
    if ( !getConfig().empty() )
        Info("configure()", "JetCalibrator Interface succesfully configured! ");
    
    return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: initialize ()
{

  Info("initialize()", "Initializing HLTJetGetter Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //
  // Grab the TrigDecTool from the ToolStore
  //
  //m_trigDecTool = dynamic_cast<Trig::TrigDecisionTool*>(asg::ToolStore::get("TrigDecisionTool"));

  if ( asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) ) {
    m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
  } else {
    Error("Initialize()", "the Trigger Decision Tool is not initialized.. [%s]", m_name.c_str());
    return EL::StatusCode::FAILURE;
  }
    
  // Configure
  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode HLTJetGetter :: execute ()
{
  if ( m_debug ) { Info("execute()", "Getting HLT jets... "); }

  //
  // Create the new container and its auxiliary store.
  //
  xAOD::JetContainer*     hltJets    = new xAOD::JetContainer();
  xAOD::JetAuxContainer*  hltJetsAux = new xAOD::JetAuxContainer();
  hltJets->setStore( hltJetsAux ); //< Connect the two

    std::cout << "1" << std::endl;
    //std::cout << m_trigDecTool->Get << std::endl;
    std::cout << "1" << std::endl;
  //Retrieving jets via trigger decision tool:
  const Trig::ChainGroup * chainGroup = m_trigDecTool->getChainGroup(m_triggerList.c_str()); //Trigger list:
    std::cout << "2" << std::endl;
  auto chainFeatures = chainGroup->features(); //Gets features associated to chain defined above
    std::cout << "3" << std::endl;
  std::string stripped_name = m_inContainerName;
  stripped_name.erase(0, 23); //Stripping 'HLT_xAOD__JetContainer_' from input container name
    std::cout << "4" << std::endl;
  auto JetFeatureContainers = chainFeatures.containerFeature<xAOD::JetContainer>(stripped_name.c_str());
    std::cout << "5" << std::endl;

  for( auto fContainer : JetFeatureContainers ) {
    for( auto trigJet : *fContainer.cptr() ) {
      xAOD::Jet *Jet = new xAOD::Jet();
      *Jet = *trigJet;
      hltJets->push_back( Jet );
    }//end trigJet loop
  }//end feature container loop
  
  RETURN_CHECK("HLTJetGetter::execute()", m_store->record( hltJets,    m_outContainerName),     "Failed to record selected hltJets");
  RETURN_CHECK("HLTJetGetter::execute()", m_store->record( hltJetsAux, m_outContainerName+"Aux."), "Failed to record selected hltJetsAux.");

  if ( m_debug ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: postExecute ()
{
  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: finalize ()
{
  Info("finalize()", "Deleting tool instances...");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: histFinalize ()
{
  Info("histFinalize()", "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
