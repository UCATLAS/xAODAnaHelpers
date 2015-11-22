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

    /**
        @rst

            This is used by all algorithms within |xAH|.

            .. note:: The expectation is that the user does not directly use this class but rather inherits from it.

            The main goal of this algorithm class is to standardize how everyone defines an algorithm that plugs into |xAH|. A series of common utilities are provided such as :cpp:member:`~xAH::Algorithm::m_className` which defines the class name so we can manage a registry :cpp:member:`~xAH::Algorithm::m_instanceRegistry` to keep |xAH| as flexible as possible to our users.

            We expect the user to create a new algorithm, such as a selector for jets::

                class JetSelector : public xAH::Algorithm
                {
                    // ...
                };

            The above example is taken from our implementation in :cpp:class:`JetSelector`. Just remember that when you write your initializer, you will be expected to do something like::

                // this is needed to distribute the algorithm to the workers
                ClassImp(JetSelector)


                JetSelector :: JetSelector () :
                    Algorithm("JetSelector"),
                    ...
                {
                    // ...
                }

            which this class will automatically register all instances of for you. Each instance can have a different name :cpp:member:`~xAH::Algorithm::m_name` but will have the same :cpp:member:`~xAH::Algorithm::m_className` so we can track how many references have been made. This is useful for selectors to deal with cutflows, but can be useful for other algorithms that need to know how many times they've been instantiated in a single job.

        @endrst

     */
  class Algorithm : public EL::Algorithm {
      public:
        /**
            @brief Initialization
            @param className    This is the name of the class that inherits from :cpp:class:`~xAH::Algorithm`
         */
        Algorithm(std::string className);
        ~Algorithm();
        /// @cond
        ClassDef(Algorithm, 1);
        /// @endcond

        /**
            @brief Set the name of this particular instance to something unique (used for ROOT's TObject name primarily)
            @param name         The name of the instance
         */
        Algorithm* setName(std::string name);
        /**
            @brief Let the algorithm know you wish to configure using a ROOT::TEnv() file
            @param configName   The path to the config file you wish to use. Can expand the path for you automatically.
         */
        Algorithm* setConfig(std::string configName);
        /**
            @brief Retrieve the path to the config file
            @param expand       Whether to retrieve the raw string set or the expanded path version
         */
        std::string getConfig(bool expand=false);

        /**
            @rst
                Set the level of verbosity in algorithms.

                ===== ====== ===== =======
                Value Binary Debug Verbose
                ===== ====== ===== =======
                0     00
                1     01     x
                2     10           x
                3     11     x     x
                ===== ====== ===== =======

            @endrst
         */
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
                the moniker by which all instances are tracked in :cpp:member:`xAH::Algorithm::m_instanceRegistry`
            @endrst
         */
        std::string m_className;

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
                bookkeeps the number of times :cpp:member:`xAH::Algorithm::m_className` has been used in a variable shared among all classes/instances that inherit from me
            @endrst
         */
	static std::map<std::string, int> m_instanceRegistry;
  };

}
#endif
