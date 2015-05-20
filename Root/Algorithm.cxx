#include <xAODAnaHelpers/Algorithm.h>

// this is needed to distribute the algorithm to the workers
ClassImp(xAH::Algorithm)

xAH::Algorithm::Algorithm() :
  m_name(""),
  m_configName(""),
  m_debug(false),
  m_systName(""),
  m_systVal(0),
  m_event(nullptr),
  m_store(nullptr)
{}

xAH::Algorithm* xAH::Algorithm::setName(std::string name){
  m_name = name;
  return this;
}

xAH::Algorithm* xAH::Algorithm::setConfig(std::string configName){
  m_configName = configName;
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
