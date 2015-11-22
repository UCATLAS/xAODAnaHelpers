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
        /// @cond
        ClassDef(AlgorithmRegistry, 1);
        /// @endcond

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
        Algorithm(std::string className);
        ~Algorithm();
        /// @cond
        ClassDef(Algorithm, 1);
        /// @endcond

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

        /**
            @rst
                Register the given instance under the moniker :cpp:member:`xAH::Algorithm::m_className`
                This will increase the reference count by 1.
            @endrst
         */
        void registerInstance();
        /**
            @rst
                Return number of instances registered under the moniker :cpp:member:`xAH::Algorithm::m_className`

                This will return the reference count.

                .. warning:: If for some reason the instance wasn't registered, we spit out a warning.
            @endrst
         */
        int numInstances();
        /**
            @rst
                Unregister the given instance under the moniker :cpp:member:`xAH::Algorithm::m_className`

                This will decrease the reference count by 1.

                .. warning:: If for some reason the instance wasn't registered, we spit out a warning.
            @endrst
         */
        void unregisterInstance();

      private:
        /**
            @rst
                the moniker by which all instances are tracked in :cpp:member:`xAH::Algorithm::m_instanceRegistry`
            @endrst
         */
        std::string m_className;
        /**
            @rst
                bookkeeps the number of times :cpp:member:`xAH::Algorithm::m_className` has been used in a variable shared among all classes/instances that inherit from me
            @endrst
         */
	static std::map<std::string, int> m_instanceRegistry;
  };

}
#endif
