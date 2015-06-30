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

        // 00 = no verbosity
        // 01 = debug only
        // 10 = verbose only
        // 11 = debug and verbose
        Algorithm* setLevel(int level);

        Algorithm* setSyst(std::string systName);
        Algorithm* setSyst(std::string systName, float systVal);

        // each algorithm should have a unique name for init, to differentiate them
        std::string m_name;

        // enable verbosity, debugging or not
        bool m_debug,
             m_verbose;

        // if running systs - the name of the systematic
        std::string m_systName;
        // if running systs - the value ( +/- 1 )
        float m_systVal;

        // custom EventInfo container name
        std::string m_eventInfoContainerName;

        // override at algorithm level
        // default: -1, use eventInfo object to determine if data or mc
        // 0: this is data
        // 1: this is mc
        int m_isMC;

      protected:
        // name of a config file to load in, optional
        std::string m_configName;

        xAOD::TEvent* m_event; //!
        xAOD::TStore* m_store; //!

        // will try to determine if data or if MC
        // returns: -1=unknown (could not determine), 0=data, 1=mc
        int isMC();

  };
}
#endif
