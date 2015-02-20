#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "AthContainers/ConstDataVector.h"

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

    std::string inputContainer = "AntiKt10LCTopoJets";
    std::string outputContainer = inputContainer+"_CDV";

    // First: retrieve AntiKt10LCTopoJets from TEvent
    const xAOD::JetContainer* in_aktJets(0);
    if( !m_event->retrieve(in_aktJets, inputContainer).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Event. Exiting.", inputContainer.c_str());
      return EL::StatusCode::FAILURE;
    }
    Info("execute()", "Successfully retrieved %s from Event.", inputContainer.c_str());

    // Second: build a ConstDataVector containing an actual subset of in_aktJets
    //  eg: Pt > 100 GeV
    ConstDataVector<xAOD::JetContainer>* in_aktJets_subset(0);
    in_aktJets_subset = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
    for(const auto jet: *in_aktJets){
      if(jet->pt()/1000. > 100.) in_aktJets_subset->push_back(jet);
    }
    Info("execute()", "Applied Pt>100 GeV cut to %s jets. Only %d/%d jets passed.", inputContainer.c_str(), (int)in_aktJets_subset->size(), (int)in_aktJets->size());

    // Third: store the ConstDataVector in TStore
    if( !m_store->record(in_aktJets_subset, outputContainer).isSuccess() ){
      Error("execute()", "Failed to store CDV subset of %s in Store as %s. Exiting.", inputContainer.c_str(), outputContainer.c_str());
      return EL::StatusCode::FAILURE;
    }
    Info("execute()", "Successfully stored CDV subset of %s in Store as %s.", inputContainer.c_str(), outputContainer.c_str());
    std::cout << "\n";

    // Fourth: retrieve the object stored in multiple ways


    Info("execute()", "Attempting to retrieve %s as a ConstDataVector.", outputContainer.c_str());
    // -- retrieve as a ConstDataVector
    ConstDataVector<xAOD::JetContainer>* out_aktJets_CDV_event(0);
    ConstDataVector<xAOD::JetContainer>* out_aktJets_CDV_store(0);
    int cannotRetrieve = 0;

    // ---- from TStore
    if( !m_store->retrieve(out_aktJets_CDV_store, outputContainer).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store as ConstDataVector. Exiting.", outputContainer.c_str());
      cannotRetrieve |= 1;
    }
    Info("execute()", "Successfully retrieved %s from Store as ConstDataVector.", outputContainer.c_str());

    // ---- from TEvent
    if( !m_event->retrieve(out_aktJets_CDV_event, outputContainer).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Event as ConstDataVector. Exiting.", outputContainer.c_str());
      cannotRetrieve |= 1;
    }
    Info("execute()", "Successfully retrieved %s from Event as ConstDataVector.", outputContainer.c_str());
    std::cout << "\n";

    Info("execute()", "Attempting to retrieve %s as a const DataVector.", outputContainer.c_str());
    // -- retrieve as a const DataVector
    const xAOD::JetContainer* out_aktJets_event(0);
    const xAOD::JetContainer* out_aktJets_store(0);

    // ---- from TEvent
    if( !m_event->retrieve(out_aktJets_event, outputContainer).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Event as const DataVector. Exiting.", outputContainer.c_str());
      cannotRetrieve |= 1;
    }
    Info("execute()", "Successfully retrieved %s from Event as const DataVector.", outputContainer.c_str());

    // ---- from TStore
    if( !m_store->retrieve(out_aktJets_store, outputContainer).isSuccess() ){
      Error("execute()", "Failed to retrieve %s from Store as const DataVector. Exiting.", outputContainer.c_str());
      cannotRetrieve |= 1;
    }
    Info("execute()", "Successfully retrieved %s from Store as const DataVector.", outputContainer.c_str());
    std::cout << "\n";

    if(cannotRetrieve) return EL::StatusCode::FAILURE;

    //else if ( m_event->contains<const xAOD::JetContainer>(m_inContainerName)){

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ContainerRecording :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ContainerRecording :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode ContainerRecording :: histFinalize () { return EL::StatusCode::SUCCESS; }
