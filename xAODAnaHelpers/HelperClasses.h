#ifndef xAODAnaHelpers_HELPERCLASSES_H
#define xAODAnaHelpers_HELPERCLASSES_H

#include <map>
#include <iostream>

#include "TString.h"

/* stuff below is for templating getContainer */
#include <RootCoreUtils/ThrowMsg.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include <xAODEventInfo/EventInfo.h>

namespace HelperClasses {

  enum class ContainerType {
      UNKNOWN      = 0,
      CONSTDV      = 1,
      CONSTCONT    = 2,
  };

  enum class ToolName {
      MUONSELECTOR = 0,
      ELECTRONSELECTOR,
      JETSELECTOR,
      BJETSELECTOR,
      OVERLAPREMOVER,
  };

  /* template enum parser
  shamelessly copied from: http://stackoverflow.com/a/726681
  */
  template <typename T>
  class EnumParser
  {
     std::multimap <TString, T> enumMap;
   public:
     EnumParser();

     T parseEnum(const TString &value)
     {
	/*
	for (auto it = enumMap.begin(); it != enumMap.end(); ++it){
	   std::cout << "element: " << (*it).first << std::endl;
	}
        */
        typename std::multimap <TString, T>::const_iterator iValue = enumMap.find(value);
        if (iValue == enumMap.end()){
            std::cerr << "Could not find input TString in enum!" << std::endl;
	}
        return iValue->second;
     }
  };


  struct InfoSwitch {
    std::string m_configStr;
    InfoSwitch(std::string configStr) : m_configStr(configStr) { };
    bool parse(std::string flag);
  };

  struct JetInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_clean;
    bool m_energy;
    bool m_resolution;
    bool m_truth;
    bool m_truthDetails;
    bool m_layer;
    bool m_trackPV;
    bool m_trackAll;
    void initialize();
    JetInfoSwitch(std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };


  template <typename T>
  const T* getContainer(std::string name, xAOD::TEvent* event, xAOD::TStore* store) {
    const T* cont = 0;
    if ( store->contains< ConstDataVector<T> >(name)){
      ConstDataVector<T>* contCDV = 0;
      if ( !store->retrieve( contCDV, name ).isSuccess() ){
        Error("getContainer()  ", "Failed to retrieve %s ConstDataVector from Store. Exiting.", name.c_str() );
        RCU_THROW_MSG("Failure");
      }
      cont = contCDV->asDataVector();
    }
    else if ( event->contains<const T>(name)){
      if ( !event->retrieve( cont , name ).isSuccess() ){
        Error("getContainer()  ", "Failed to retrieve %s const DataVector from File. Exiting.", name.c_str() );
        RCU_THROW_MSG("Failure");
      }
    }
    else if ( store->contains<const T>(name)){
      Warning("getContainer()", "TEvent can't retrieve %s from TStore!", name.c_str());
      if( !store->retrieve( cont, name ).isSuccess() ){
        Error("getContainer()", "Failed to retrieve %s const DataVector from Store. Exiting.", name.c_str() );
        RCU_THROW_MSG("Failure");
      }
    }
    else {
      Error("getContainer()  ", "Failed to retrieve %s container from File or Store. Exiting.", name.c_str() );
      store->print();
      RCU_THROW_MSG("Failure");
    }

    return cont;
  }

  // PTS defined here
  /* EventInfo is special, why? ConstDataVector not used with it...
    https://gist.github.com/kratsg/84ae66cb37a9d858d829
  */
  template <>
  inline const xAOD::EventInfo* getContainer<xAOD::EventInfo>(std::string name, xAOD::TEvent* event, xAOD::TStore*) {
    const xAOD::EventInfo* eventInfo = 0;
    if ( !event->retrieve(eventInfo, name).isSuccess() ) {
      Error("getContainer()", "Failed to retrieve %s EventInfo from event. Exiting.", name.c_str());
      RCU_THROW_MSG("Failure");
    }

    return eventInfo;
  }

} // close namespace HelperClasses


# endif
