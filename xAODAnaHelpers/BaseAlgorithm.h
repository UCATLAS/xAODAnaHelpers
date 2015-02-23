#ifndef xAODAnaHelpers_BaseAlgorithm_H
#define xAODAnaHelpers_BaseAlgorithm_H

#include <RootCoreUtils/ThrowMsg.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include <xAODEventInfo/EventInfo.h>

class BaseAlgorithm {
  public:
    BaseAlgorithm() {};

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
      else {
        Error("getContainer()  ", "Failed to retrieve %s container from File or Store. Exiting.", name.c_str() );
        store->print();
        RCU_THROW_MSG("Failure");
      }

      return cont;
    }

};

// PTS defined here
/* EventInfo is special, why? ConstDataVector not used with it...
  https://gist.github.com/kratsg/84ae66cb37a9d858d829
*/
template <>
inline const xAOD::EventInfo* BaseAlgorithm::getContainer<xAOD::EventInfo>(std::string name, xAOD::TEvent* event, xAOD::TStore*) {
  const xAOD::EventInfo* eventInfo = 0;
  if ( !event->retrieve(eventInfo, name).isSuccess() ) {
    Error("getContainer()", "Failed to retrieve %s EventInfo from event. Exiting.", name.c_str());
    RCU_THROW_MSG("Failure");
  }

  return eventInfo;
}

#endif

