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

  class AlgorithmRegistry {

      public:
        AlgorithmRegistry(){};
	virtual ~AlgorithmRegistry() {};
        ClassDef(AlgorithmRegistry, 1);

        // this maps bookkeeps the names of the algorithms
        // which are used, and how many times they have
        // been used before as well
        std::map<std::string, int> m_registered_algos;

        // returns how many times an algo has been
        // already used
        int countRegistered(std::string className);

  };

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
        Algorithm* setSyst(std::string systName, std::vector<float> systValVector);

        // each algorithm should have a unique name for init, to differentiate them
        std::string m_name;
	
        // each algorithm have a 'class' name (e.g., 'ElectronSelector', 'JetCalibrator' ...) to identify the type
	std::string m_classname;
       
        // enable verbosity, debugging or not
        bool m_debug,
             m_verbose;

        // if running systs - the name of the systematic
        std::string m_systName;
        // if running systs - the value ( +/- 1 )
        float m_systVal;
        // for running multiple syst points
        std::vector<float> m_systValVector;

        // custom EventInfo container name
        std::string m_eventInfoContainerName;

        // override at algorithm level
        // default: -1, use eventInfo object to determine if data or mc
        // 0: this is data
        // 1: this is mc
        int m_isMC;

	// register the name of the algorithms
	// in a record.
	// This can be used as a 'database' for other algos
	// to check if a class of the same type already exists
	Algorithm* registerClass(AlgorithmRegistry &reg, std::string className);

      protected:
        // name of a config file to load in, optional
        std::string m_configName;

        xAOD::TEvent* m_event; //!
        xAOD::TStore* m_store; //!

        // will try to determine if data or if MC
        // returns: -1=unknown (could not determine), 0=data, 1=mc
        int isMC();

	// returns how many times an algo of *this* type
	// has already been used
	int countUsed() { return m_count_used; };

      private:
        // bookkeeps the number of times an algo of *this* type has been used
	int m_count_used;

  };

}
#endif
