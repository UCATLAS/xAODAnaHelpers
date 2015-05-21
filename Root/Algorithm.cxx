#include <xAODAnaHelpers/Algorithm.h>

// ROOT includes
#include <TSystem.h>

// RCU include for throwing an exception+message
#include <RootCoreUtils/ThrowMsg.h>

// this is needed to distribute the algorithm to the workers
ClassImp(xAH::Algorithm)

xAH::Algorithm::Algorithm() :
  m_name(""),
  m_debug(false),
  m_systName(""),
  m_systVal(0),
  m_configName(""),
  m_event(nullptr),
  m_store(nullptr)
{}

xAH::Algorithm* xAH::Algorithm::setName(std::string name){
  m_name = name;
  // call the TNamed
  this->SetName(name.c_str());
  return this;
}

xAH::Algorithm* xAH::Algorithm::setConfig(std::string configName){
  m_configName = configName;
  if ( !m_configName.empty() ) {
    m_configName = gSystem->ExpandPathName( m_configName.c_str() );
    /* check if file exists
     https://root.cern.ch/root/roottalk/roottalk02/5332.html */
    FileStat_t fStats;
    int fSuccess = gSystem->GetPathInfo(m_configName.c_str(), fStats);
    if(fSuccess != 0){
      // could not find
      delete this;
      RCU_THROW_MSG("xAH::Algorithm::setConfig could not find the config file specified:\r\n\t\tConfig File: "+configName);
    }
  }
  return this;
}

xAH::Algorithm* xAH::Algorithm::setDebug(bool debug){
  m_debug = debug;
  return this;
}

xAH::Algorithm* xAH::Algorithm::setSyst(std::string systName){
  return this->setSyst(systName, 0);
}

xAH::Algorithm* xAH::Algorithm::setSyst(std::string systName, float systVal){
  m_systName = systName;
  m_systVal = systVal;
  return this;
}
