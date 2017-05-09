#include <xAODAnaHelpers/Algorithm.h>

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
  m_event(nullptr),
  m_store(nullptr),
  m_className(className),
  m_registered(false)
{
}

xAH::Algorithm::~Algorithm()
{
}

StatusCode xAH::Algorithm::algInitialize(){
    // register an instance of the the class
    registerInstance();
    return StatusCode::SUCCESS;
}

StatusCode xAH::Algorithm::algFinalize(){
    unregisterInstance();
    return StatusCode::SUCCESS;
}

xAH::Algorithm* xAH::Algorithm::SetName(std::string name){
  m_name = name;
  // call the TNamed
  //EL::Algorithm::SetName((m_className+"."+name).c_str());
  EL::Algorithm::SetName(name.c_str());
  return this;
}

xAH::Algorithm* xAH::Algorithm::setLevel(int level){
  m_debug = level & 1;
  m_verbose = (level >> 1)&1;
  return this;
}

StatusCode xAH::Algorithm::parseSystValVector(){

    std::stringstream ss(m_systValVectorString);
    float systVal;
    while( ss >> systVal ){
      m_systValVector.push_back(systVal);
      if (ss.peek() == ',')
        ss.ignore();
    }
    ss.str("");

    return StatusCode::SUCCESS;
}

int xAH::Algorithm::isMC(){
  // first override if need to
  if(m_isMC == 0 || m_isMC == 1) return m_isMC;

  const xAOD::EventInfo* ei(nullptr);
  // couldn't retrieve it
  if(!HelperFunctions::retrieve(ei, m_eventInfoContainerName, m_event, m_store, msg()).isSuccess()){
    if(m_debug) ATH_MSG_WARNING( "Could not retrieve eventInfo container: " << m_eventInfoContainerName);
    return -1;
  }

  static SG::AuxElement::ConstAccessor<uint32_t> eventType("eventTypeBitmask");
  if(!eventType.isAvailable(*ei)){
    if(m_debug) ATH_MSG_WARNING( "eventType is not available.");
    return -1;
  }

  // reached here, return 0 or 1 since we have all we need
  return (static_cast<uint32_t>(eventType(*ei)) & xAOD::EventInfo::IS_SIMULATION);
}

void xAH::Algorithm::registerInstance(){
    if(m_registered) return;
    m_instanceRegistry[m_className]++;
    m_registered = true;
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
