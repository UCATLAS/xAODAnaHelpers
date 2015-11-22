#include <xAODAnaHelpers/Algorithm.h>

// ROOT includes
#include <TSystem.h>

// RCU include for throwing an exception+message
#include <RootCoreUtils/ThrowMsg.h>

//  for isMC()
#include <xAODAnaHelpers/HelperFunctions.h>
#include "xAODEventInfo/EventInfo.h"

std::map<std::string, int> xAH::Algorithm::m_instanceRegistry = {};

// this is needed to distribute the algorithm to the workers
ClassImp(xAH::Algorithm)

xAH::Algorithm::Algorithm(std::string className) :
  m_name(""),
  m_debug(false),
  m_verbose(false),
  m_systName(""),
  m_systVal(0),
  m_eventInfoContainerName("EventInfo"),
  m_isMC(-1),
  m_configName(""),
  m_event(nullptr),
  m_store(nullptr),
  m_className(className)
{
    registerInstance();
}

xAH::Algorithm::~Algorithm() {
    unregisterInstance();
}

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

void xAH::Algorithm::registerInstance(){
    m_instanceRegistry[m_className]++;
}

int xAH::Algorithm::numInstances(){
    if(m_instanceRegistry.find(m_className) == m_instanceRegistry.end()){
        printf("numInstances: we seem to have recorded zero instances of %s. This should not happen.", m_className.c_str());
        return 0;
    }
    return m_instanceRegistry.at(m_className);
}

void xAH::Algorithm::unregisterInstance(){
    if(m_instanceRegistry.find(m_className) == m_instanceRegistry.end()){
        printf("unregisterInstance: we seem to have recorded zero instances of %s. This should not happen.", m_className.c_str());
    }
    m_instanceRegistry[m_className]--;
}
