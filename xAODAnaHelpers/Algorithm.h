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

// for StatusCode::isSuccess
#include "AsgTools/StatusCode.h"

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
        Algorithm(std::string className = "Algorithm");
        ~Algorithm();
        /// @cond
        ClassDef(Algorithm, 1);
        /// @endcond

        /**
            @brief Run any initializations commmon to all xAH Algorithms (such as registerInstance). Call this inside :code:`histInitialize` for best results.
         */
        StatusCode algInitialize();

        /**
            @brief Run any finalizations common to all xAH Algorithms (such as unregisterInstance). Call this inside :code:`histFinalize` for best results.
         */
        StatusCode algFinalize();

        /**
            @brief Set the name of this particular instance to something unique (used for ROOT's TObject name primarily)
            @param name         The name of the instance
         */
        Algorithm* setName(std::string name);

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

        /**
            @brief Enable and set the systematic with default value 0
            @param systName     The name of the systematic
         */
        Algorithm* setSyst(std::string systName);
        /**
            @brief Enable and set the systematic with the value
            @param systName     The name of the systematic
            @param systVal      The value to set the systematic to
            @rst
                .. note:: This will set the systematic to the value :math:`\pm x`.
            @endrst
         */
        Algorithm* setSyst(std::string systName, float systVal);
        /**
            @brief Enable and set the systemtatic with the vector of values
            @param systName             The name of the systematic
            @param systValVector        The values to set the systematic to
         */
        Algorithm* setSyst(std::string systName, std::vector<float> systValVector);

        /** All algorithms initialized should have a unique name, to differentiate them at the TObject level */
        std::string m_name;

        /** Enable debug output */
        bool m_debug,
        /** Enable verbose output */
             m_verbose;

        /** If running systematics, the name of the systematic */
        std::string m_systName;
        /** If running systematics, the value to set the systematic to
            @rst
                .. note:: This will set the systematic to the value :math:`\pm x`.
            @endrst
         */
        float m_systVal;
        /** If running systematics, you can run multiple points and store them in here */
        std::vector<float> m_systValVector;

        /** If the xAOD has a different EventInfo container name, set it here */
        std::string m_eventInfoContainerName;

        /**
            @rst
                This is an override at the algorithm level to force analyzing MC or not.

                ===== ========================================================
                Value Meaning
                ===== ========================================================
                -1    Default, use eventInfo object to determine if data or mc
                0     Treat the input as data
                1     Treat the input as MC
                ===== ========================================================

            @endrst
         */
        int m_isMC;

      protected:
        /** The TEvent object */
        xAOD::TEvent* m_event; //!
        /** The TStore object */
        xAOD::TStore* m_store; //!

        // will try to determine if data or if MC
        // returns: -1=unknown (could not determine), 0=data, 1=mc
        /**
            @rst
                Try to determine if we are running over data or MC.

                ============ =======
                Return Value Meaning
                ============ =======
                -1           Unknown
                0            Data
                1            MC
                ============ =======

            @endrst
         */
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

        /**
            @rst
                Map containing info about whether a CP Tool of a given name has been already used or not by this :cpp:`xAH::Algorithm`
		Its content must be set in `:cpp:EL::initialize()`, depending on whether the tool it's created from scratch, or retrieved from `:cpp:asg::ToolStore`
            @endrst
         */
        std::map<std::string, bool> m_toolAlreadyUsed; //!

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
