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
    // set the name this way as duplicate names are handled automatically
    m_name = name();
    // names will be BasicEventSelection.baseEventSel for example
    msg().setName(m_className + "." + m_name);
    // deprecating m_debug, but this is around for backwards compatibility
    m_debug = msgLvl(MSG::DEBUG);
    // deprecating m_verbose, but this is around for backwards compatibility
    m_verbose = msgLvl(MSG::VERBOSE);

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
        ANA_MSG_ERROR("numInstances: we seem to have recorded zero instances of " << m_className << ". This should not happen.");
        return 0;
    }
    return m_instanceRegistry.at(m_className);
}

void xAH::Algorithm::unregisterInstance(){
    if(m_instanceRegistry.find(m_className) == m_instanceRegistry.end()){
        ANA_MSG_ERROR("unregisterInstance: we seem to have recorded zero instances of " << m_className << ". This should not happen.");
    }
    m_instanceRegistry[m_className]--;
}
