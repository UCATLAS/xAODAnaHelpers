// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
// output stream
#include "EventLoop/OutputStream.h"

// EDM include(s):
#include "xAODCore/AuxContainerBase.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/ElectronAuxContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODJet/Jet.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/MissingET.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/PhotonAuxContainer.h"
#include "xAODEgamma/Photon.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTau/TauJetAuxContainer.h"
#include "xAODTau/TauJet.h"

// package include(s):
#include "xAODAnaHelpers/MinixAOD.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(MinixAOD)

MinixAODCopyInfo::MinixAODCopyInfo(const std::string& copyString)
{
  size_t posOut = copyString.find_first_of('>');
  size_t posSys = copyString.find_first_of('|');
  this->inputContainerName =copyString.substr(0,std::min(posOut,posSys));
  this->outputContainerName=copyString.substr(posOut+1,posSys);
  if(this->outputContainerName.empty()) this->outputContainerName=this->inputContainerName; // Default is empty

  if(posSys!=std::string::npos) this->systAlgo =copyString.substr(posSys+1);

  this->shallowIO=this->inputContainerName.back()!='*';
  if(!this->shallowIO) this->inputContainerName.pop_back();

  //std::cout << "TEST " << copyString << " = " << this->inputContainerName << " " << this->outputContainerName << " " << this->systAlgo << " " << this->shallowIO << std::endl;
}

MinixAOD :: MinixAOD (std::string className) :
    Algorithm(className),
    m_simpleCopyKeys_vec(),
    m_shallowCopyKeys_vec(),
    m_deepCopyKeys_vec(),
    m_particleCopyKeys_vec(),
    m_fileMetaDataTool(nullptr),
    m_trigMetaDataTool(nullptr)
{
  Info("MinixAOD()", "Calling constructor");

  m_outputFileName = "out_miniXAOD";
  m_createOutputFile = true;
  m_copyFileMetaData = false;
  m_copyTriggerInfo = false;
  m_copyCutBookkeeper = false;
  m_simpleCopyKeys = "";
  m_storeCopyKeys = "";
  m_deepCopyKeys = "";
  m_particleCopyKeys = "";
}

EL::StatusCode MinixAOD :: setupJob (EL::Job& job)
{
  if(m_debug) Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "MinixAOD" ).ignore(); // call before opening first file

  // only create the output xaod if requested
  if(m_createOutputFile){
    EL::OutputStream out_xAOD (m_outputFileName, "xAOD");
    job.outputAdd (out_xAOD);
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MinixAOD :: histInitialize ()
{
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: changeInput (bool firstFile)
{
  //
  // Update CutBookkeeper
  if(m_copyCutBookkeeper)
    {
      // Retrieve the input container:
      const xAOD::CutBookkeeperContainer* inputCBKContainer(nullptr);
      RETURN_CHECK("MinixAOD::fileExecute()", wk()->xaodEvent()->retrieveMetaInput(inputCBKContainer, "CutBookkeepers"), "");

      if(firstFile)
	{
	  // Create an output container
	  m_outputCBKContainer     = new xAOD::CutBookkeeperContainer();
	  m_outputCBKContainer_aux = new xAOD::CutBookkeeperAuxContainer();
	  m_outputCBKContainer->setStore( m_outputCBKContainer_aux );

	  // Create an empty incomplete output container
	  m_outputInCBKContainer     = new xAOD::CutBookkeeperContainer();
	  m_outputInCBKContainer_aux = new xAOD::CutBookkeeperAuxContainer();
	  m_outputInCBKContainer->setStore( m_outputInCBKContainer_aux );

	  // Create our cutbookkeeper
	  m_outputCBK=new xAOD::CutBookkeeper();
	  m_outputCBK->setName("MinixAODKernel");    
	  m_outputCBK->setCycle(inputCBKContainer->maxCycle()+1);
	}

      // Merge
      m_outputCBKContainer->merge(inputCBKContainer);
    }

  return EL::StatusCode::SUCCESS; 
}

EL::StatusCode MinixAOD :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode MinixAOD :: initialize ()
{
  if(m_debug) Info("initialize()", "Calling initialize");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // always do this, obviously
  TFile *file_xAOD = wk()->getOutputFile(m_outputFileName);
  RETURN_CHECK("MinixAOD::initialize()", m_event->writeTo(file_xAOD), "Could not set output to file");

  if(m_copyFileMetaData){
    m_fileMetaDataTool = new xAODMaker::FileMetaDataTool();

    if(m_verbose)
      RETURN_CHECK("MinixAOD::initialize()", m_fileMetaDataTool->setProperty("OutputLevel", MSG::VERBOSE ), "Could not set verbosity on fileMenuMetaDataTool");


    RETURN_CHECK("MinixAOD::initialize()", m_fileMetaDataTool->initialize(), "Could not initialize FileMetaDataTool");
    if(m_debug) Info("initialize()", "FileMetaDataTool initialized...");
  }

  if(m_copyTriggerInfo){
    m_trigMetaDataTool = new xAODMaker::TriggerMenuMetaDataTool();

    if(m_verbose)
      RETURN_CHECK("MinixAOD::initialize()", m_trigMetaDataTool->setProperty("OutputLevel", MSG::VERBOSE ), "Could not set verbosity on TriggerMenuMetaDataTool");

    RETURN_CHECK("MinixAOD::initialize()", m_trigMetaDataTool->initialize(), "Could not initialize TriggerMenuMetaDataTool");
    if(m_debug) Info("initialize()", "TriggerMenuMetaDataTool initialized...");

    if(m_debug) std::cout << "Adding xTrigDecision and TrigConfKeys to the list of keys copied from the input file." << std::endl;
    m_simpleCopyKeys_vec.push_back(MinixAODCopyInfo("xTrigDecision"));
    m_simpleCopyKeys_vec.push_back(MinixAODCopyInfo("TrigConfKeys"));
  }

  // parse and split by comma
  std::string token;
  std::istringstream ss("");

  // simple copy
  ss.clear(); ss.str(m_simpleCopyKeys);
  while(std::getline(ss, token, ' '))
    m_simpleCopyKeys_vec.push_back(MinixAODCopyInfo(token));

  // store copy
  ss.clear(); ss.str(m_storeCopyKeys);
  while(std::getline(ss, token, ' '))
    m_storeCopyKeys_vec.push_back(MinixAODCopyInfo(token));

  // shallow copy
  ss.clear(); ss.str(m_shallowCopyKeys);
  while(std::getline(ss, token, ' '))
    m_shallowCopyKeys_vec.push_back(MinixAODCopyInfo(token));

  // A B C D ... Z -> {A, B, C, D, ..., Z}
  ss.clear(); ss.str(m_deepCopyKeys);
  while(std::getline(ss, token, ' '))
    m_deepCopyKeys_vec.push_back(MinixAODCopyInfo(token));

  // A1|A2 B1|B2 C1|C2 ... Z1|Z2 -> {(A1, A2), (B1, B2), ..., (Z1, Z2)}
  ss.clear(); ss.str(m_particleCopyKeys);
  while(std::getline(ss, token, ' '))
    m_particleCopyKeys_vec.push_back(MinixAODCopyInfo(token));

  if(m_debug) Info("initialize()", "MinixAOD Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: execute ()
{
  if(m_verbose) Info("execute()", "Dumping objects...");

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("BasicEventSelection::initialize()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, m_verbose) ,"");

  //
  // Fill cutbookkeeper
  if(m_copyCutBookkeeper)
    {
      float eventWeight(1);
      if ( eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) )
	eventWeight = eventInfo->mcEventWeight();

      m_outputCBK->addNAcceptedEvents(1);
      m_outputCBK->addSumOfEventWeights(eventWeight);
      m_outputCBK->addSumOfEventWeightsSquared(eventWeight*eventWeight);
    }

  //
  // Determine what should be copied how
  std::vector<MinixAODCopyInfo> simpleCopyKeys_vec =m_simpleCopyKeys_vec;
  std::vector<MinixAODCopyInfo> shallowCopyKeys_vec=m_shallowCopyKeys_vec;
  std::vector<MinixAODCopyInfo> deepCopyKeys_vec   =m_deepCopyKeys_vec;
  std::vector<MinixAODCopyInfo> storeCopyKeys_vec  =m_storeCopyKeys_vec;
  std::vector<std::string>      vectorCopyKeys_vec;
  
  for(const auto& copy: m_particleCopyKeys_vec){
    std::vector<std::string>* systNames(nullptr);
    if(copy.systAlgo.empty())
      {
	systNames=new std::vector<std::string>();
	systNames->push_back("");
      }
    else
      {
	vectorCopyKeys_vec.push_back(copy.systAlgo);
	RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(systNames, copy.systAlgo, m_event, m_store, m_verbose) ,"");
      }

    for(auto systName : *systNames)
      {
	// copy from container
	MinixAODCopyInfo newCopyInfo;
	newCopyInfo.inputContainerName =copy.inputContainerName +systName;
	newCopyInfo.outputContainerName=copy.outputContainerName+systName;

	if(m_store->contains<xAOD::IParticleContainer>(newCopyInfo.inputContainerName))
	  {
	    // Is this a shallow copy?
	    if(m_store->contains<xAOD::ShallowAuxContainer>(newCopyInfo.inputContainerName+"Aux."))
	      {
		newCopyInfo.shallowIO=copy.shallowIO;
		shallowCopyKeys_vec.push_back(newCopyInfo);
		continue;
	      }

	    // Is this a normal container?
	    if(m_store->contains<xAOD::AuxContainerBase>(newCopyInfo.inputContainerName+"Aux."))
	      {
		storeCopyKeys_vec.push_back(newCopyInfo);
		continue;
	      }

	    // This is a ConstDataVector
	    deepCopyKeys_vec.push_back(newCopyInfo);
	    continue;
	  }

	// simple copy
	simpleCopyKeys_vec.push_back(newCopyInfo);
      }

    if(copy.systAlgo.empty())
      delete systNames;
  }
    
  //
  // Copy code
  
  // simple copy is easiest - it's in the input, copy over, no need for types
  for(const auto& copy: simpleCopyKeys_vec){
    if(m_debug) std::cout << "Copying " << copy.inputContainerName << " from input file" << std::endl;
    RETURN_CHECK("MinixAOD::execute()", m_event->copy(copy.inputContainerName), std::string("Could not copy "+copy.inputContainerName+" from input file.").c_str());
  }

  // we need to make deep copies
  for(const auto& copy: deepCopyKeys_vec){
    const xAOD::IParticleContainer* cont(nullptr);
    RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(cont, copy.inputContainerName, nullptr, m_store, m_verbose), std::string("Could not retrieve container "+copy.inputContainerName+" from TStore. Enable m_verbose to find out why.").c_str());

    if(const xAOD::ElectronContainer* t_cont = dynamic_cast<const xAOD::ElectronContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::makeDeepCopy<xAOD::ElectronContainer,  xAOD::ElectronAuxContainer,  xAOD::Electron> (m_event, copy.outputContainerName, t_cont)), std::string("Could not deep copy "+copy.outputContainerName+".").c_str());
    } else if(const xAOD::JetContainer* t_cont = dynamic_cast<const xAOD::JetContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::makeDeepCopy<xAOD::JetContainer,       xAOD::JetAuxContainer,       xAOD::Jet>      (m_event, copy.outputContainerName, t_cont)), std::string("Could not deep copy "+copy.outputContainerName+".").c_str());
    } else if(const xAOD::MissingETContainer* t_cont = dynamic_cast<const xAOD::MissingETContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::makeDeepCopy<xAOD::MissingETContainer, xAOD::MissingETAuxContainer, xAOD::MissingET>(m_event, copy.outputContainerName, t_cont)), std::string("Could not deep copy "+copy.outputContainerName+".").c_str());
    } else if(const xAOD::MuonContainer* t_cont = dynamic_cast<const xAOD::MuonContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::makeDeepCopy<xAOD::MuonContainer,      xAOD::MuonAuxContainer,      xAOD::Muon>     (m_event, copy.outputContainerName, t_cont)), std::string("Could not deep copy "+copy.outputContainerName+".").c_str());
    } else if(const xAOD::PhotonContainer* t_cont = dynamic_cast<const xAOD::PhotonContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::makeDeepCopy<xAOD::PhotonContainer,    xAOD::PhotonAuxContainer,    xAOD::Photon>   (m_event, copy.outputContainerName, t_cont)), std::string("Could not deep copy "+copy.outputContainerName+".").c_str());
    } else if(const xAOD::TauJetContainer* t_cont = dynamic_cast<const xAOD::TauJetContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::makeDeepCopy<xAOD::TauJetContainer,    xAOD::TauJetAuxContainer,    xAOD::TauJet>   (m_event, copy.outputContainerName, t_cont)), std::string("Could not deep copy "+copy.outputContainerName+".").c_str());
    } else {
      std::cout << "Could not identify what container " << copy.inputContainerName << " corresponds to for deep-copying." << std::endl;
      return EL::StatusCode::FAILURE;
    }

    if(m_debug) std::cout << "Deep-Copied " << copy.inputContainerName << " to record to output file as " << copy.outputContainerName << std::endl;
  }

  // shallow IO handling 
  for(const auto& copy: shallowCopyKeys_vec){
    const xAOD::IParticleContainer* cont(nullptr);
    RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(cont, copy.inputContainerName, nullptr, m_store, m_verbose), std::string("Could not retrieve container "+copy.inputContainerName+" from TStore. Enable m_verbose to find out why.").c_str());

    if(dynamic_cast<const xAOD::ElectronContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::ElectronContainer> (m_event, m_store, copy.inputContainerName, copy.outputContainerName, copy.shallowIO)), std::string("Could not copy "+copy.outputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::JetContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::JetContainer>      (m_event, m_store, copy.inputContainerName, copy.outputContainerName, copy.shallowIO)), std::string("Could not copy "+copy.outputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::MissingETContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::MissingETContainer>(m_event, m_store, copy.inputContainerName, copy.outputContainerName, copy.shallowIO)), std::string("Could not copy "+copy.outputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::MuonContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::MuonContainer>     (m_event, m_store, copy.inputContainerName, copy.outputContainerName, copy.shallowIO)), std::string("Could not copy "+copy.outputContainerName+" from TStore to TEvent").c_str());
    } else if(dynamic_cast<const xAOD::PhotonContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::PhotonContainer>   (m_event, m_store, copy.inputContainerName, copy.outputContainerName, copy.shallowIO)), std::string("Could not copy "+copy.outputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::TauJetContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::TauJetContainer>   (m_event, m_store, copy.inputContainerName, copy.outputContainerName, copy.shallowIO)), std::string("Could not copy "+copy.outputContainerName+" from TStore to TEvent.").c_str());
    } else {
      std::cout << "Could not identify what container " << copy.inputContainerName << " corresponds to for copying from TStore to TEvent." << std::endl;
      return EL::StatusCode::FAILURE;
    }

  }

  // all we need to do is retrieve it and figure out what type it is to record it and we're done
  for(const auto& copy: storeCopyKeys_vec){
    const xAOD::IParticleContainer* cont(nullptr);
    RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(cont, copy.inputContainerName, nullptr, m_store, m_verbose), std::string("Could not retrieve container "+copy.inputContainerName+" from TStore. Enable m_verbose to find out why.").c_str());

    if(dynamic_cast<const xAOD::ElectronContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::ElectronContainer,  xAOD::ElectronAuxContainer> (m_event, m_store, copy.inputContainerName, copy.outputContainerName)), std::string("Could not copy "+copy.inputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::JetContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::JetContainer,       xAOD::JetAuxContainer>      (m_event, m_store, copy.inputContainerName, copy.outputContainerName)), std::string("Could not copy "+copy.inputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::MissingETContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::MissingETContainer, xAOD::MissingETAuxContainer>(m_event, m_store, copy.inputContainerName, copy.outputContainerName)), std::string("Could not copy "+copy.inputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::MuonContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::MuonContainer,      xAOD::MuonAuxContainer>     (m_event, m_store, copy.inputContainerName, copy.outputContainerName)), std::string("Could not copy "+copy.inputContainerName+" from TStore to TEvent").c_str());
    } else if(dynamic_cast<const xAOD::PhotonContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::PhotonContainer,    xAOD::PhotonAuxContainer>   (m_event, m_store, copy.inputContainerName, copy.outputContainerName)), std::string("Could not copy "+copy.inputContainerName+" from TStore to TEvent.").c_str());
    } else if(dynamic_cast<const xAOD::TauJetContainer*>(cont)){
      RETURN_CHECK("MinixAOD::execute()", (HelperFunctions::recordOutput<xAOD::TauJetContainer,    xAOD::TauJetAuxContainer>   (m_event, m_store, copy.inputContainerName, copy.outputContainerName)), std::string("Could not copy "+copy.inputContainerName+" from TStore to TEvent.").c_str());
    } else {
      std::cout << "Could not identify what container " << copy.inputContainerName << " corresponds to for copying from TStore to TEvent." << std::endl;
      return EL::StatusCode::FAILURE;
    }

    if(m_debug) std::cout << "Copied " << copy.inputContainerName << " and its auxiliary container from TStore to TEvent" << std::endl;
  }

  // remove duplicates from m_copyFromStoreToEventKeys_vec
  // - see http://stackoverflow.com/a/1041939/1532974
  std::set<std::string> s;
  unsigned size = vectorCopyKeys_vec.size();
  for( unsigned i = 0; i < size; ++i ) s.insert( vectorCopyKeys_vec[i] );
  vectorCopyKeys_vec.assign( s.begin(), s.end() );

  for(const auto& systAlgoName: vectorCopyKeys_vec){
    std::vector<std::string>* systNames(nullptr);
    RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(systNames, systAlgoName, m_event, m_store, m_verbose) ,"");
    m_event->record(systNames, systAlgoName);
  }

  m_event->fill();
  if(m_debug) Info("execute()", "Finished dumping objects...");

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MinixAOD :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode MinixAOD :: finalize () {
  //
  // Save cutbookkeeper
  if(m_copyCutBookkeeper)
    {
      RETURN_CHECK("MinixAOD::finalize()", wk()->xaodEvent()->recordMeta(m_outputCBKContainer      ,"CutBookkeepers")              , "");
      RETURN_CHECK("MinixAOD::finalize()", wk()->xaodEvent()->recordMeta(m_outputCBKContainer_aux  ,"CutBookkeepersAux.")          , "");
      RETURN_CHECK("MinixAOD::finalize()", wk()->xaodEvent()->recordMeta(m_outputInCBKContainer    ,"IncompleteCutBookkeepers")    , "");
      RETURN_CHECK("MinixAOD::finalize()", wk()->xaodEvent()->recordMeta(m_outputInCBKContainer_aux,"IncompleteCutBookkeepersAux."), "");

      m_outputCBKContainer->push_back(m_outputCBK);
    }

  //
  // Close file
  TFile *file_xAOD = wk()->getOutputFile(m_outputFileName);
  RETURN_CHECK("MinixAOD::finalize()", m_event->finishWritingTo(file_xAOD), "Could not finish writing to the output xAOD.");

  if(m_fileMetaDataTool) delete m_fileMetaDataTool;
  if(m_trigMetaDataTool) delete m_trigMetaDataTool;

  return EL::StatusCode::SUCCESS;
}
EL::StatusCode MinixAOD :: histFinalize ()
{
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
