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
#include "AsgTools/ToolStore.h"
#include "AsgTools/AnaToolHandle.h"

// for resolving paths of various calibration files
#include "PathResolver/PathResolver.h"

// messaging includes
#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>
#include <AsgTools/MessageCheck.h>

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

            which this class will automatically register all instances of for you. Each instance can have a different algorithm name but will have the same :cpp:member:`~xAH::Algorithm::m_className` so we can track how many references have been made. This is useful for selectors to deal with cutflows, but can be useful for other algorithms that need to know how many times they've been instantiated in a single job.

        @endrst

     */
  class Algorithm : public EL::Algorithm {
      public:
        /**
            @brief Initialization
            @param className    This is the name of the class that inherits from :cpp:namespace:`~xAH::Algorithm`
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
            @brief All algorithms initialized should have a unique name, to differentiate them at the TObject level.

            Note, :code:`GetName()` returns a :code:`char*` while this returns a :code:`std::string`.
        */
        std::string m_name = "UnnamedAlgorithm";

        /** m_debug is being deprecated */
        bool m_debug = false;
        /** m_verbose is being deprecated */
        bool m_verbose = false;

        /** debug level */
        MSG::Level m_msgLevel = MSG::INFO;

        /** If running systematics, the name of the systematic */
        std::string m_systName = "";
        /** If running systematics, the value to set the systematic to
            @rst
                .. note:: This will set the systematic to the value :math:`\pm x`.
            @endrst
         */
        float m_systVal = 0.0;

        /** If running systematics, you can run multiple points and store them in here.
            A comma separated list of working points should be given to m_systValVectorString,
            and then parsed by calling parseSystValVector.
         */
        std::string m_systValVectorString = "";

        std::vector<float> m_systValVector;

        /**
            @brief Parse string of systematic sigma levels in m_systValVectorString into m_systValVector
         */
        StatusCode parseSystValVector();

        /** If the xAOD has a different EventInfo container name, set it here */
        std::string m_eventInfoContainerName = "EventInfo";

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
        int m_isMC = -1;

      protected:
        /**
            @rst
                The moniker by which all instances are tracked in :cpp:member:`xAH::Algorithm::m_instanceRegistry`

            @endrst
         */
        std::string m_className = "Algorithm"; //!

        /** The TEvent object */
        xAOD::TEvent* m_event = nullptr; //!
        /** The TStore object */
        xAOD::TStore* m_store = nullptr; //!

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
                Check whether the input CP tool already exists with *this* name in the asg::ToolStore

		Depending on the outcome, the content of the map :cpp:member:`xAH::Algorithm::m_toolAlreadyUsed` wll be set accordingly.

            @endrst
         */
        template< typename T >
        StatusCode checkToolStore( const std::string& tool_name ) {

            if ( !asg::ToolStore::contains<T>(tool_name) ) {
              m_toolAlreadyUsed[tool_name] = false;
              ANA_MSG_INFO("Tool " << tool_name << " is being used for the first time!" );
            } else {
              m_toolAlreadyUsed[tool_name] = true;
              ANA_MSG_INFO("Tool " << tool_name << " has been already used!" );
            }

            return StatusCode::SUCCESS;
        }

        /**
            @rst
                Check whether the input CP tool has been already used by any :cpp:class:`xAH::Algorithm` in the current job by scanning :cpp:member:`xAH::Algorithm::m_toolAlreadyUsed`.

            @endrst
         */
        inline bool isToolAlreadyUsed( const std::string& tool_name ) {
           return ( m_toolAlreadyUsed.find(tool_name)->second );
        }


        /**
            @rst
                Sets the name of the tool and emits :code:`ANA_MSG_WARNING` if the tool of given type/name has been configured previously.

                The reason this exists is to unify setting the tool name correctly. |xAH| is choosing the convention that you always set the type of the tool in the header, but not the name. The name, if it needs to be configurable, will be set during algorithm execution, such as in :code:`histInitialize()`. If no name is needed, the tool will use the name of the algorithm plus a unique identifier (:cpp:func:`xAH::Algorithm::getAddress`) appended to ensure the tool is unique and effectively private.

                The tool will not be guaranteed unique if two tools of the same type are created without a name passed in. But this is, at this point, up to the user and a more complex scenario than what this function tries to simplify on its own.

            @endrst
         */
        template <typename T>
        bool setToolName(asg::AnaToolHandle<T>& handle, std::string name = "") const {
          if(name.empty()) name = handle.name() + "_" + m_name + "::" + getAddress();
          handle.setName(name);
          ANA_MSG_DEBUG("Trying to set-up tool: " << handle.typeAndName());
          bool res = handle.isUserConfigured();
          if (res) ANA_MSG_WARNING("note: handle " << handle.typeAndName() << " is user configured. If this is expected, ignore the message. If it is not expected, look into " << m_className + "::" << m_name << ", check documentation, or ask around.");
          return res;
        }

        /// @brief Return a :code:`std::string` representation of :code:`this`
        std::string getAddress() const {
          const void * address = static_cast<const void*>(this);
          std::stringstream ss;
          ss << address;
          return ss.str();
        }

      private:
        /**
            @rst
              A boolean to keep track of whether this instance was registered or not.

              Calling :cpp:func:`xAH::Algorithm::registerInstance` multiple times won't inflate the number of instances of a class made because of me.

            @endrst
        */
        bool m_registered = false; //!

        /**
            @rst
                Bookkeeps the number of times :cpp:member:`xAH::Algorithm::m_className` has been used in a variable shared among all classes/instances that inherit from me

            @endrst
         */
        static std::map<std::string, int> m_instanceRegistry;

        /**
            @rst
                Map containing info about whether a CP Tool of a given name has been already used or not by this :cpp:class:`xAH::Algorithm`.

                Its content gets set through :cpp:func:`xAH::Algorithm::checkToolStore`, depending on whether the tool it's created from scratch, or retrieved from :cpp:class:`asg::ToolStore`

            @endrst
         */
        std::map<std::string, bool> m_toolAlreadyUsed; //!
  };

}
#endif
