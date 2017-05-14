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
  m_className(className)
{
}

xAH::Algorithm::~Algorithm()
{
}

StatusCode xAH::Algorithm::algInitialize(){
    // register an instance of the the class
    registerInstance();
    SetName(m_name.c_str());
    setMsgLevel(m_debugLevel);
    m_debug = msgLvl(MSG::DEBUG);
    return StatusCode::SUCCESS;
}

StatusCode xAH::Algorithm::algFinalize(){
    unregisterInstance();
    return StatusCode::SUCCESS;
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
    ANA_MSG_DEBUG( "Could not retrieve eventInfo container: " << m_eventInfoContainerName);
    return -1;
  }

  static SG::AuxElement::ConstAccessor<uint32_t> eventType("eventTypeBitmask");
  if(!eventType.isAvailable(*ei)){
    ANA_MSG_DEBUG( "eventType is not available.");
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
        msg() << MSG::ERROR << "numInstances: we seem to have recorded zero instances of " << m_className << ". This should not happen." << endmsg;
        return 0;
    }
    return m_instanceRegistry.at(m_className);
}

void xAH::Algorithm::unregisterInstance(){
    if(m_instanceRegistry.find(m_className) == m_instanceRegistry.end()){
        msg() << MSG::ERROR << "unregisterInstance: we seem to have recorded zero instances of " << m_className << ". This should not happen." << endmsg;
    }
    m_instanceRegistry[m_className]--;
}
