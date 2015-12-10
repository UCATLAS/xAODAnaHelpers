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
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/MinixAOD.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(MinixAOD)

MinixAOD :: MinixAOD (std::string className) :
    Algorithm(className),
    m_outputFileName("out_miniXAOD"),
    m_createOutputFile(true),
    m_copyFileMetaData(false),
    m_copyTriggerInfo(false),
    m_simpleCopyKeys(""),
    m_storeCopyKeys(""),
    m_shallowCopyKeys(""),
    m_deepCopyKeys(""),
    m_simpleCopyKeys_vec(),
    m_shallowCopyKeys_vec(),
    m_deepCopyKeys_vec(),
    m_copyFromStoreToEventKeys_vec(),
    m_fileMetaDataTool(nullptr),
    m_trigMetaDataTool(nullptr)
{
  Info("MinixAOD()", "Calling constructor");
}

EL::StatusCode  MinixAOD :: configure ()
{
  if ( !getConfig().empty() ) {
    Info("configure()", "Configuing MinixAOD Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug         = config->GetValue("Debug" ,      m_debug);
    m_verbose       = config->GetValue("Verbose",     m_verbose);

    config->Print();
    Info("configure()", "MinixAOD Interface succesfully configured! ");

    delete config; config = nullptr;
  }
  return EL::StatusCode::SUCCESS;
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

EL::StatusCode MinixAOD :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode MinixAOD :: initialize ()
{
  if(m_debug) Info("initialize()", "Calling initialize");

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

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
  }

  // parse and split by comma
  std::string token;
  std::istringstream ss("");

  // A B C D ... Z -> {A, B, C, D, ..., Z}
  ss.clear(); ss.str(m_simpleCopyKeys);
  while(std::getline(ss, token, ' '))
    m_simpleCopyKeys_vec.push_back(token);

  // A B C D ... Z -> {A, B, C, D, ..., Z}
  ss.clear(); ss.str(m_storeCopyKeys);
  while(std::getline(ss, token, ' '))
    m_copyFromStoreToEventKeys_vec.push_back(token);

  // A1|A2 B1|B2 C1|C2 ... Z1|Z2 -> {(A1, A2), (B1, B2), ..., (Z1, Z2)}
  ss.clear(); ss.str(m_shallowCopyKeys);
  while(std::getline(ss, token, ' ')){
    int pos = token.find_first_of('|');
    m_shallowCopyKeys_vec.push_back(std::pair<std::string, std::string>(token.substr(0, pos), token.substr(pos+1)));
  }

  // A1|A2 B1|B2 C1|C2 ... Z1|Z2 -> {(A1, A2), (B1, B2), ..., (Z1, Z2)}
  ss.clear(); ss.str(m_deepCopyKeys);
  while(std::getline(ss, token, ' ')){
    int pos = token.find_first_of('|');
    m_deepCopyKeys_vec.push_back(std::pair<std::string, std::string>(token.substr(0, pos), token.substr(pos+1)));
  }

  if(m_debug) Info("initialize()", "MinixAOD Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: execute ()
{
  if(m_verbose) Info("execute()", "Dumping objects...");

  // simple copy is easiest - it's in the input, copy over, no need for types
  for(const auto& key: m_simpleCopyKeys_vec)
    RETURN_CHECK("MinixAOD::execute()", m_event->copy(key), std::string("Could not copy "+key+" from input file.").c_str());

  for(const auto& keypair: m_deepCopyKeys_vec){
    auto in_key = keypair.first;
    auto out_key = keypair.second;

    const xAOD::IParticleContainer* cont(nullptr);
    RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(cont, in_key, nullptr, m_store, m_verbose), std::string("Could not retrieve container "+in_key+" from TStore. Enable m_verbose to find out why.").c_str());

    if(const xAOD::JetContainer* t_cont = dynamic_cast<const xAOD::JetContainer*>(cont))
      HelperFunctions::makeDeepCopy<xAOD::JetContainer, xAOD::JetAuxContainer, xAOD::Jet>(m_store, out_key.c_str(), t_cont);

    m_copyFromStoreToEventKeys_vec.push_back(out_key);
  }

  // shallow IO handling (if no parent, assume deep copy)
  for(const auto& keypair: m_shallowCopyKeys_vec){
    auto key = keypair.first;
    auto parent = keypair.second;

    // only add the parent if it doesn't exist -- this could be if someone has multiple shallows to add from the same parent!
    if(!parent.empty())
      if(std::find(m_copyFromStoreToEventKeys_vec.begin(), m_copyFromStoreToEventKeys_vec.end(), parent) == m_copyFromStoreToEventKeys_vec.end())
        m_copyFromStoreToEventKeys_vec.push_back(parent);

    m_copyFromStoreToEventKeys_vec.push_back(key);
  }

  for(const auto& key: m_copyFromStoreToEventKeys_vec){
    // all we need to do is retrieve it and figure out what type it is to record it
    const xAOD::IParticleContainer* cont(nullptr);
    RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(cont, key, nullptr, m_store, m_verbose), std::string("Could not retrieve container "+key+" from TStore. Enable m_verbose to find out why.").c_str());

    if(dynamic_cast<const xAOD::JetContainer*>(cont))
      HelperFunctions::recordOutput<xAOD::JetContainer, xAOD::JetAuxContainer>(m_event, m_store, key);
  }


  if(m_verbose) Info("execute()", "Finished dumping objects...");
  m_event->fill();
  if(m_verbose) Info("execute()", "Called m_event->fill() successfully.");

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MinixAOD :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: finalize () {
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
