#include <xAODAnaHelpers/Algorithm.h>

// RCU include for throwing an exception+message
#include <RootCoreUtils/ThrowMsg.h>

//  for isMC()
#include <xAODAnaHelpers/HelperFunctions.h>
#include "xAODEventInfo/EventInfo.h"

#include "PATInterfaces/SystematicSet.h"

std::map<std::string, int> xAH::Algorithm::m_instanceRegistry = {};

// this is needed to distribute the algorithm to the workers

xAH::Algorithm::Algorithm(const std::string& name, ISvcLocator *pSvcLocator, const std::string& className) :
  EL::AnaAlgorithm(name, pSvcLocator),
  m_className(className)
{
    declareProperty("name", m_name);
    declareProperty("debug", m_debug);
    declareProperty("verbose", m_verbose);
    declareProperty("msgLevel", m_msgLevel);
    declareProperty("systName", m_systName);
    declareProperty("systVal", m_systVal);
    declareProperty("systValVectorString", m_systValVectorString);
    declareProperty("eventInfoContainerName", m_eventInfoContainerName);
    declareProperty("vertexContainerName", m_vertexContainerName);
    declareProperty("isMC", m_isMC);
    declareProperty("isFastSim", m_isFastSim);
    declareProperty("forceFastSim", m_forceFastSim);
    declareProperty("forceFullSim", m_forceFullSim);
    declareProperty("forceData", m_forceData);
    declareProperty("setAFII", m_setAFII);
    declareProperty("className", m_className);
    declareProperty("isDAOD", m_isDAOD);
    declareProperty("isPHYS", m_isPHYS);
    declareProperty("cutFlowHistName", m_cutFlowHistName);
    declareProperty("metaDataHistName", m_metaDataHistName);
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
    if(!evtStore()->retrieve(ei, m_eventInfoContainerName).isSuccess()){
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
    ANA_CHECK( inputMetaStore()->retrieve(fmd, "FileMetaData") );
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


void xAH::Algorithm::writeSystematicsListHist( const std::vector< CP::SystematicSet > &systs, const std::string& histName )
{
  if (!systs.size() || histName.empty()) {
    return;
  }
  std::string name = m_metaDataHistName + "/" + "systematics" + "/" + histName;

  ANA_CHECK(book(TH1D(name.c_str(), name.c_str(), systs.size(), 0.5, systs.size() + 0.5)));
  TH1D* hist = hist(name);
  for (size_t i = 0; i < systs.size(); i++) {
    if (systs[i].name().empty()) {
      hist->GetXaxis()->SetBinLabel(i + 1, "nominal");
    } else {
      hist->GetXaxis()->SetBinLabel(i + 1, systs[i].name().c_str());
    }
  }
  hist->Write();
}
