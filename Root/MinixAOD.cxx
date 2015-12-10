// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"

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
    m_shallowCopyKeys(""),
    m_deepCopyNames(""),
    m_shallowCopyKeys_vec(),
    m_deepCopyNames_vec(),
    m_fileMetaDataTool(nullptr),
    m_triggerMetaDataTool(nullptr)
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

    config->Print();
    Info("configure()", "MinixAOD Interface succesfully configured! ");

    delete config; config = nullptr;
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: setupJob (EL::Job& job)
{
  Info("setupJob()", "Calling setupJob");

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
  Info("initialize()", "Calling initialize");

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
    Info("initialize()", "FileMetaDataTool initialized...");
  }

  if(m_copyTriggerInfo){
    m_triggerMetaDataTool = new xAODMaker::TriggerMenuMetaDataTool();

    if(m_verbose)
      RETURN_CHECK("MinixAOD::initialize()", m_triggerMetaDataTool->setProperty("OutputLevel", MSG::VERBOSE ), "Could not set verbosity on TriggerMenuMetaDataTool");

    RETURN_CHECK("MinixAOD::initialize()", m_triggerMetaDataTool->initialize(), "Could not initialize TriggerMenuMetaDataTool");
    Info("initialize()", "TriggerMenuMetaDataTool initialized...");
  }

  // parse and split by comma
  std::string token;

  std::istringstream ss(m_shallowCopyKeys);
  while ( std::getline(ss, token, ',') ) {
    m_shallowCopyKeyNames.push_back(token);
  }

  ss.clear();
  ss.str(m_deepCopyKeys);
  while ( std::getline(ss, token, ',') ) {
    std::cout << token << std::endl;
    m_deepCopyKeyNames.push_back(token);
  }

  Info("initialize()", "MinixAOD Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: execute ()
{
  if ( m_debug ) { Info("execute()", "Dumping objects..."); }

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MinixAOD::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  if ( m_debug ) { Info("execute()", "Finished dumping objects..."); }

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MinixAOD :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: finalize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: histFinalize ()
{
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
