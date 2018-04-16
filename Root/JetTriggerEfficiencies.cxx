// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include <xAODJet/JetContainer.h>
#include <xAODJet/JetAuxContainer.h>
#include <xAODCore/AuxContainerBase.h>
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"

// package include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/JetTriggerEfficiencies.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

// external tools include(s):
#include "JetJvtEfficiency/JetJvtEfficiency.h"
#include "JetMomentTools/JetForwardJvtTool.h"
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"
#include "TriggerMatchingTool/MatchingTool.h"

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetTriggerEfficiencies)


JetTriggerEfficiencies :: JetTriggerEfficiencies () :
    Algorithm("JetTriggerEfficiencies")
{
}

EL::StatusCode JetTriggerEfficiencies :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_DEBUG( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "JetTriggerEfficiencies" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_DEBUG( "Calling histInitialize");
  ANA_CHECK( xAH::Algorithm::algInitialize());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  ANA_MSG_DEBUG( "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  ANA_MSG_DEBUG( "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_DEBUG( "Calling initialize");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  
  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  // Grab the TrigDecTool from the ToolStore
  if(!m_trigDecTool_handle.isUserConfigured()){
    ANA_MSG_FATAL("A configured " << m_trigDecTool_handle.typeAndName() << " must have been previously created! Double-check the name of the tool." );
    return EL::StatusCode::FAILURE;
  }
  ANA_CHECK( m_trigDecTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);
  
  ANA_MSG_DEBUG( "JetTriggerEfficiencies Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG( "Getting jet trigger efficiencies... " << m_name);

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // MC event weight
  float mcEvtWeight(1.0);
  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
    static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
    if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
      ANA_MSG_ERROR( "mcEventWeight is not available as decoration! Aborting" );
      return EL::StatusCode::FAILURE;
    }
    mcEvtWeight = mcEvtWeightAcc( *eventInfo );
  }

  const xAOD::JetContainer* inJets(nullptr);

  // this will be the collection processed - no matter what!!
  ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, msg()) );

  std::string refChainName = "HLT_j260";
  std::cout << refChainName << " info:" << std::endl;
  bool passedRefTrigger = m_trigDecTool_handle->isPassed(refChainName);
  const unsigned int refBits = m_trigDecTool_handle->isPassedBits(refChainName);
  bool L1_isPassedBeforePrescale = refBits & TrigDefs::L1_isPassedBeforePrescale;
  bool L1_isPassedAfterPrescale  = refBits & TrigDefs::L1_isPassedAfterPrescale;
  bool L1_isPassedAfterVeto      = refBits & TrigDefs::L1_isPassedAfterVeto;

  bool isPrescaledOut = refBits & TrigDefs::EF_prescaled;
  std::cout << "  passed? " << passedRefTrigger << std::endl;

  std::cout << "  L1 passed before prescale? " << L1_isPassedBeforePrescale << std::endl;
  std::cout << "  L1 passed after prescale? " << L1_isPassedAfterPrescale << std::endl;
  std::cout << "  L1 passed after veto? " << L1_isPassedAfterVeto << std::endl;

  std::cout << "  HLT prescaled out? " << isPrescaledOut << std::endl;

  // pass = executeSelection( inJets, mcEvtWeight, count, m_outContainerName, true );

  // look what we have in TStore
  if(msgLvl(MSG::VERBOSE)) m_store->print();


  ANA_MSG_DEBUG( "Leave jet trigger efficiencies... ");

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode JetTriggerEfficiencies :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: finalize ()
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

  ANA_MSG_DEBUG( m_name );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: histFinalize ()
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

  ANA_MSG_DEBUG( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}

