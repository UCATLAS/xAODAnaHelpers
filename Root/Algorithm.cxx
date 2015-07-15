#include <xAODAnaHelpers/Algorithm.h>

// ROOT includes
#include <TSystem.h>

// RCU include for throwing an exception+message
#include <RootCoreUtils/ThrowMsg.h>

//  for isMC()
#include <xAODAnaHelpers/HelperFunctions.h>
#include "xAODEventInfo/EventInfo.h"

// this is needed to distribute the algorithm to the workers
ClassImp(xAH::AlgorithmRegistry)

int xAH::AlgorithmRegistry::countRegistered(std::string className){

  auto iter = m_registered_algos.find(className);

  if ( iter != m_registered_algos.end() ) {
    Info("countRegistered()","input class name: %s is already in the registry! Increase counter by 1 and return it", className.c_str() );
    m_registered_algos.at(className)++;
    return m_registered_algos.at(className);
  }

  Info("countRegistered()","input class name: %s is not registered yet. Returning 0", className.c_str() );

  return 0;

}

// this is needed to distribute the algorithm to the workers
ClassImp(xAH::Algorithm)

xAH::Algorithm::Algorithm() :
  m_name(""),
  m_classname(""),
  m_debug(false),
  m_verbose(false),
  m_systName(""),
  m_systVal(0),
  m_eventInfoContainerName("EventInfo"),
  m_isMC(-1),
  m_configName(""),
  m_event(nullptr),
  m_store(nullptr),
  m_count_used(0)
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
    /* check if file exists
     https://root.cern.ch/root/roottalk/roottalk02/5332.html */
    FileStat_t fStats;
    int fSuccess = gSystem->GetPathInfo(xAH::Algorithm::getConfig(true).c_str(), fStats);
    if(fSuccess != 0){
      // could not find
      delete this;
      RCU_THROW_MSG("xAH::Algorithm::setConfig could not find the config file specified:\r\n\t\tConfig File: "+configName);
    }
  }
  return this;
}

std::string xAH::Algorithm::getConfig(bool expand){
  if(expand) return gSystem->ExpandPathName( m_configName.c_str() );
  return m_configName;
}

xAH::Algorithm* xAH::Algorithm::setLevel(int level){
  m_debug = level & 1;
  m_verbose = (level >> 1)&1;
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

xAH::Algorithm* xAH::Algorithm::setSyst(std::string systName, std::vector<float> systValVector){
  m_systName = systName;
  m_systValVector = systValVector;
  return this;
}

int xAH::Algorithm::isMC(){
  // first override if need to
  if(m_isMC == 0 || m_isMC == 1) return m_isMC;

  const xAOD::EventInfo* ei(nullptr);
  // couldn't retrieve it
  if(!HelperFunctions::retrieve(ei, m_eventInfoContainerName, m_event, m_store).isSuccess()){
    if(m_debug) Warning("isMC()", "Could not retrieve eventInfo container: %s", m_eventInfoContainerName.c_str());
    return -1;
  }

  static SG::AuxElement::ConstAccessor<uint32_t> eventType("eventTypeBitmask");
  if(!eventType.isAvailable(*ei)){
    if(m_debug) Warning("isMC()", "eventType is not available.");
    return -1;
  }

  // reached here, return 0 or 1 since we have all we need
  return (static_cast<uint32_t>(eventType(*ei)) & xAOD::EventInfo::IS_SIMULATION);
}

xAH::Algorithm* xAH::Algorithm::registerClass(xAH::AlgorithmRegistry &reg, std::string className){

  Info("registerClass()","input class name: %s", className.c_str() );

  m_classname = className;
  
  // the function will return 0 if the algo 
  // isn't in the registry yet
  m_count_used = reg.countRegistered(className);

  // if not found already, set in the map in the registry the name of the algo
  // and assign a value of 0 to the counter
  if ( m_count_used == 0 ) { reg.m_registered_algos[className] = m_count_used; }

  return this;

}

