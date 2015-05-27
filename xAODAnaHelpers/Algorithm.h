#ifndef xAODAnaHelpers_Algorithm_H
#define xAODAnaHelpers_Algorithm_H

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// EL include(s):
#include <EventLoop/StatusCode.h>
#include <EventLoop/Algorithm.h>

#include <string>

namespace xAH {
  class Algorithm : public EL::Algorithm {
      public:
        Algorithm();
        ClassDef(Algorithm, 1);

        Algorithm* setName(std::string name);
        Algorithm* setConfig(std::string configName);
        std::string getConfig(bool expand=false);

        Algorithm* setDebug(bool debug);

        Algorithm* setSyst(std::string systName);
        Algorithm* setSyst(std::string systName, float systVal);

        // each algorithm should have a unique name for init, to differentiate them
        std::string m_name;

        // enable verbose debugging or not
        bool m_debug;

        // if running systs - the name of the systematic
        std::string m_systName;
        // if running systs - the value ( +/- 1 )
        float m_systVal;

        // custom EventInfo container name
        std::string m_eventInfoContainerName;

      protected:
        // name of a config file to load in, optional
        std::string m_configName;

        xAOD::TEvent* m_event; //!
        xAOD::TStore* m_store; //!

  };
}
#endif
