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
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODJet/Jet.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HLTJetGetter.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// this is needed to distribute the algorithm to the workers
ClassImp(HLTJetGetter)

HLTJetGetter :: HLTJetGetter (std::string className) :
Algorithm(className),
m_trigDecTool(nullptr)
{
    Info("HLTJetGetter()", "Calling constructor");

    // regex list of triggers
    m_triggerList = ".*";
    // input container to be read from TEvent or TStore
    m_inContainerName = "";
    // shallow copies are made with this output container name
    m_outContainerName = "";
    // flag to own TDT and TCT
    m_ownTDTAndTCT = false;

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

EL::StatusCode HLTJetGetter :: initialize ()
{


    Info("initialize()", "Initializing HLTJetGetter Interface... ");

    m_event = wk()->xaodEvent();
    m_store = wk()->xaodStore();

    //
    // Grab the TrigDecTool from the ToolStore
    //

    if ( asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) ) {
        m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
    } else {
        Info ("Initialize()", "the Trigger Decision Tool is not yet initialized...[%s]. Doing so now.", m_name.c_str());
        m_ownTDTAndTCT = true;

        m_trigConfTool = new TrigConf::xAODConfigTool( "xAODConfigTool" );
        RETURN_CHECK("BasicEventSelection::initialize()", m_trigConfTool->initialize(), "Failed to properly initialize TrigConf::xAODConfigTool");
        ToolHandle< TrigConf::ITrigConfigTool > configHandle( m_trigConfTool );

        m_trigDecTool = new Trig::TrigDecisionTool( "TrigDecisionTool" );
        RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "ConfigTool", configHandle ), "");
        RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "TrigDecisionKey", "xTrigDecision" ), "");
        RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->setProperty( "OutputLevel", MSG::ERROR), "");
        RETURN_CHECK("BasicEventSelection::initialize()", m_trigDecTool->initialize(), "Failed to properly initialize Trig::TrigDecisionTool");
        Info("initialize()", "Successfully configured Trig::TrigDecisionTool!");
    }

    // If there is no InputContainer we must stop
    if ( m_inContainerName.empty() ) {
        Error("initialize()", "InputContainer is empty!");
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

    //Retrieving jets via trigger decision tool:
    const Trig::ChainGroup * chainGroup = m_trigDecTool->getChainGroup(m_triggerList.c_str()); //Trigger list:
    auto chainFeatures = chainGroup->features(); //Gets features associated to chain defined above
    auto JetFeatureContainers = chainFeatures.containerFeature<xAOD::JetContainer>(m_inContainerName.c_str());

    RETURN_CHECK("HLTJetGetter::execute()", m_store->record( hltJets,    m_outContainerName),     "Failed to record selected hltJets");
    RETURN_CHECK("HLTJetGetter::execute()", m_store->record( hltJetsAux, m_outContainerName+"Aux."), "Failed to record selected hltJetsAux.");

    for( auto fContainer : JetFeatureContainers ) {
        for( auto trigJet : *fContainer.cptr() ) {
            xAOD::Jet *Jet = new xAOD::Jet();
            *Jet = *trigJet;
            hltJets->push_back( Jet );
        }//end trigJet loop
    }//end feature container loop

    if ( m_verbose ) { m_store->print(); }

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

    // this is necessary because in most cases the pointer will be set to null
    // after deletion in BasicEventSelection, but it will not propagate here
    if ( m_ownTDTAndTCT ) {
      if ( m_trigDecTool )  { delete m_trigDecTool; m_trigDecTool = nullptr;  }
      if ( m_trigConfTool ) {  delete m_trigConfTool; m_trigConfTool = nullptr; }
    }

    return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetGetter :: histFinalize ()
{
    Info("histFinalize()", "Calling histFinalize");
    RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
    return EL::StatusCode::SUCCESS;
}
