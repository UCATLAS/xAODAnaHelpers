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


    //Backwards compatibility
    m_forceFastSim = m_forceFastSim || m_setAFII;

    //Return a failure if there's contradictory flags (2 or more are true)
    if( m_forceData ? (m_forceFastSim || m_forceFullSim) : (m_forceFastSim && m_forceFullSim) ){
      ANA_MSG_ERROR("Multiple input-type flags are set, be sure only one of m_forceData(" << m_forceData << "), m_forceFastSim(" << m_forceFastSim << "), and m_forceFullSim(" << m_forceFullSim << ") are true.");
      return StatusCode::FAILURE;
    }
  
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

bool xAH::Algorithm::isMC(){
    
    // If decision is established, return the decision
    if(m_isMC == 0 || m_isMC == 1) return m_isMC;
    
    // If overriding decision by boolean flags
    if( m_forceData ){
      m_isMC = 0;
      return m_isMC;
    }else if ( m_forceFullSim || m_forceFastSim ){
      m_isMC = 1;
      return m_isMC;
    }

    const xAOD::EventInfo* ei(nullptr);
    // couldn't retrieve it
    if(!HelperFunctions::retrieve(ei, m_eventInfoContainerName, m_event, m_store, msg()).isSuccess()){
      RCU_THROW_MSG( "Could not retrieve eventInfo container (" + m_eventInfoContainerName+") for isMC() check.");
    }

    static SG::AuxElement::ConstAccessor<uint32_t> eventType("eventTypeBitmask");
    if(!eventType.isAvailable(*ei)){
      RCU_THROW_MSG( "eventType is not available for isMC() check.");
    }

    // reached here, return True or False since we have all we need
    m_isMC = (static_cast<uint32_t>(eventType(*ei)) & xAOD::EventInfo::IS_SIMULATION);
    return m_isMC;
}

bool xAH::Algorithm::isFastSim(){

    // If decision is established, return the decision
    if(m_isFastSim == 0 || m_isFastSim == 1) return m_isFastSim;
      
    // If overriding decision by boolean flags
    if( m_forceData || m_forceFullSim ){
      m_isFastSim = 0;
      return m_isFastSim;
    }else if ( m_forceFastSim ){
      m_isFastSim = 1;
      return m_isFastSim;
    }
   
    std::string SimulationFlavour; 
    const xAOD::FileMetaData* fmd = nullptr;
    ANA_CHECK( wk()->xaodEvent()->retrieveMetaInput(fmd, "FileMetaData") );
    fmd->value(xAOD::FileMetaData::simFlavour, SimulationFlavour);
  
    if( SimulationFlavour == "AtlfastII" ){
      m_isFastSim = 1;
    }else{
      m_isFastSim = 0;
    }
  
    return m_isFastSim;
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
