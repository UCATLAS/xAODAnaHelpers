#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// c++ includes
#include <iostream>

#include "xAODCore/ShallowCopy.h"

#include "xAODJet/JetContainer.h"
#include "AthContainers/ConstDataVector.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/ContainerRecording.h>

// this is needed to distribute the algorithm to the workers
ClassImp(ContainerRecording)

ContainerRecording :: ContainerRecording () {}

EL::StatusCode ContainerRecording :: setupJob (EL::Job& job) {
  job.useXAOD();
  xAOD::Init("ContainerRecording").ignore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ContainerRecording :: histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ContainerRecording :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ContainerRecording :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode ContainerRecording :: initialize ()
{
  Info("initialize()", "ContainerRecording");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ContainerRecording :: execute ()
{

    std::string inputContainer          = "AntiKt10LCTopoJets";
    std::string outputContainer_CDV     = inputContainer+"_CDV";
    std::string outputContainer_shallow = inputContainer+"_Shallow";

    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    Info("execute()", "Retrieving the input jet container %s.", inputContainer.c_str());
    // First: retrieve AntiKt10LCTopoJets from TEvent
    const xAOD::JetContainer* in_aktJets(0);
    // this should succeed
    RETURN_CHECK("ContainerRecording::execute()", HelperFunctions::retrieve(in_aktJets, inputContainer, m_event, m_store, true), "Failed to retrieve from Event or Store.");
    RETURN_CHECK("ContainerRecording::execute()", HelperFunctions::retrieve(in_aktJets, inputContainer, m_event, 0, true), "Failed to retrieve from Event.");
    // this should fail, we use "!" to switch
    RETURN_CHECK("ContainerRecording::execute()", !HelperFunctions::retrieve(in_aktJets, inputContainer, 0, m_store, true), "Found it in Store. How did it get there?");

    // Second: build a ConstDataVector containing an actual subset of in_aktJets
    //  eg: Pt > 100 GeV
    ConstDataVector<xAOD::JetContainer>* in_aktJets_subset(0);
    in_aktJets_subset = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
    for(const auto jet: *in_aktJets){
      if(jet->pt()/1000. > 100.) in_aktJets_subset->push_back(jet);
    }
    Info("execute()", "Applied Pt>100 GeV cut to %s jets. Only %d/%d jets passed.", inputContainer.c_str(), (int)in_aktJets_subset->size(), (int)in_aktJets->size());

    // ........ and build a shallow copy of the in_aktJets, but set all Pt to zero
    std::pair< xAOD::JetContainer*, xAOD::ShallowAuxContainer* > in_aktJets_shallow = xAOD::shallowCopyContainer( *in_aktJets );
    for(auto jet: *(in_aktJets_shallow.first)){
      xAOD::JetFourMom_t newp4(0.0, jet->eta(), jet->phi(), jet->m());
      jet->setJetP4(newp4);
    }
    Info("execute()", "Applied Pt=0 GeV to the shallow-copied jets.");

    // Third: store the ConstDataVector in TStore
    RETURN_CHECK("ContainerRecording::execute()", m_store->record(in_aktJets_subset, outputContainer_CDV), "Failed to store CDV subset in Store." );
    // ....... and the shallow copy in TStore
    RETURN_CHECK("ContainerRecording::execute()", m_store->record( in_aktJets_shallow.first, outputContainer_shallow ), "");
    RETURN_CHECK("ContainerRecording::execute()", m_store->record( in_aktJets_shallow.second, outputContainer_shallow+"Aux." ), "");
    std::cout << std::endl << "000000000000000000000000000000000000000000000000000" << std::endl;

    // Fourth: retrieve the object stored in multiple ways
    Info("execute()", "Attempting to retrieve %s as a ConstDataVector.", outputContainer_CDV.c_str());
    int cannotRetrieve = 0;
    // -- retrieve as a ConstDataVector
    ConstDataVector<xAOD::JetContainer>* out_aktJets_CDV_event(0);
    ConstDataVector<xAOD::JetContainer>* out_aktJets_CDV_store(0);
    ConstDataVector<xAOD::JetContainer>* out_aktJets_CDV_both(0);
    // ---- from TStore
    if( !HelperFunctions::retrieve(out_aktJets_CDV_store, outputContainer_CDV, 0, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store as ConstDataVector.", outputContainer_CDV.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store as ConstDataVector.", outputContainer_CDV.c_str());
    }
    // ---- from TEvent
    if( !HelperFunctions::retrieve(out_aktJets_CDV_event, outputContainer_CDV, m_event, 0, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Event as ConstDataVector.", outputContainer_CDV.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Event as ConstDataVector.", outputContainer_CDV.c_str());
    }
    // ---- from TStore, then TEvent (TStore should have priority)
    if( !HelperFunctions::retrieve(out_aktJets_CDV_both, outputContainer_CDV, m_event, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store/Event as ConstDataVector.", outputContainer_CDV.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store/Event as ConstDataVector.", outputContainer_CDV.c_str());
    }
    std::cout << std::endl << "111111111111111111111111111111111111111111111111111" << std::endl;

    Info("execute()", "Attempting to retrieve %s as a const DataVector.", outputContainer_CDV.c_str());
    // -- retrieve as a const DataVector
    const xAOD::JetContainer* out_aktJets_event(0);
    const xAOD::JetContainer* out_aktJets_store(0);
    const xAOD::JetContainer* out_aktJets_both(0);
    // ---- from TStore
    if( !HelperFunctions::retrieve(out_aktJets_store, outputContainer_CDV, 0, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store as const DataVector.", outputContainer_CDV.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store as const DataVector.", outputContainer_CDV.c_str());
    }
    // ---- from TEvent
    if( !HelperFunctions::retrieve(out_aktJets_event, outputContainer_CDV, m_event, 0, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Event as const DataVector.", outputContainer_CDV.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Event as const DataVector.", outputContainer_CDV.c_str());
    }
    // ---- from TStore, then TEvent (TStore should have priority)
    if( !HelperFunctions::retrieve(out_aktJets_both, outputContainer_CDV, m_event, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store/Event as const DataVector.", outputContainer_CDV.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store/Event as const DataVector.", outputContainer_CDV.c_str());
    }
    std::cout << std::endl << "222222222222222222222222222222222222222222222222222" << std::endl;

    Info("execute()", "Attempting to retrieve %s as a DataVector.", outputContainer_shallow.c_str());
    // -- retrieve as a DataVector
    xAOD::JetContainer* out_aktJets_shallow_event(0);
    xAOD::JetContainer* out_aktJets_shallow_store(0);
    xAOD::JetContainer* out_aktJets_shallow_both(0);
    // ---- from TStore
    if( !HelperFunctions::retrieve(out_aktJets_shallow_store, outputContainer_shallow, 0, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store as DataVector.", outputContainer_shallow.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store as DataVector.", outputContainer_shallow.c_str());
    }
    // ---- from TEvent
    if( !HelperFunctions::retrieve(out_aktJets_shallow_event, outputContainer_shallow, m_event, 0, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Event as DataVector.", outputContainer_shallow.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Event as DataVector.", outputContainer_shallow.c_str());
    }
    // ---- from TStore, then TEvent (TStore should have priority)
    if( !HelperFunctions::retrieve(out_aktJets_shallow_both, outputContainer_shallow, m_event, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store/Event as DataVector.", outputContainer_shallow.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store/Event as DataVector.", outputContainer_shallow.c_str());
    }
    std::cout << std::endl << "333333333333333333333333333333333333333333333333333" << std::endl;

    Info("execute()", "Attempting to retrieve %s as a csont DataVector.", outputContainer_shallow.c_str());
    // -- retrieve as a const DataVector
    const xAOD::JetContainer* out_aktJets_const_shallow_event(0);
    const xAOD::JetContainer* out_aktJets_const_shallow_store(0);
    const xAOD::JetContainer* out_aktJets_const_shallow_both(0);
    // ---- from TStore
    if( !HelperFunctions::retrieve(out_aktJets_const_shallow_store, outputContainer_shallow, 0, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store as const DataVector.", outputContainer_shallow.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store as const DataVector.", outputContainer_shallow.c_str());
    }
    // ---- from TEvent
    if( !HelperFunctions::retrieve(out_aktJets_const_shallow_event, outputContainer_shallow, m_event, 0, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Event as const DataVector.", outputContainer_shallow.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Event as const DataVector.", outputContainer_shallow.c_str());
    }
    // ---- from TStore, then TEvent (TStore should have priority)
    if( !HelperFunctions::retrieve(out_aktJets_const_shallow_both, outputContainer_shallow, m_event, m_store, true).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store/Event as const DataVector.", outputContainer_shallow.c_str());
      cannotRetrieve |= 1;
    } else {
      Info("execute()", "Successfully retrieved %s from Store/Event as const DataVector.", outputContainer_shallow.c_str());
    }
    std::cout << std::endl << "444444444444444444444444444444444444444444444444444" << std::endl;

    if(cannotRetrieve) return EL::StatusCode::FAILURE;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ContainerRecording :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ContainerRecording :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ContainerRecording :: histFinalize () { return EL::StatusCode::SUCCESS; }
