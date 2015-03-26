#ifndef xAODAnaHelpers_HELPERFUNCTIONS_H
#define xAODAnaHelpers_HELPERFUNCTIONS_H

// for typing in template
#include <typeinfo>
#include <cxxabi.h>
// Gaudi/Athena include(s):
#include "AthContainers/normalizedTypeinfoName.h"

// local includes
#include "AsgTools/StatusCode.h"

// jet reclustering and trimming
#include <fastjet/JetDefinition.hh>

#ifndef __CINT__
  #include "xAODTracking/VertexContainer.h"
  #include "xAODEventShape/EventShape.h"
  #include "xAODEgamma/ElectronContainer.h"
  #include "xAODEgamma/Electron.h"
  #include "xAODEgamma/PhotonContainer.h"
  #include "xAODEgamma/Photon.h"
  #include "xAODMuon/MuonContainer.h"
  #include "xAODMuon/Muon.h"
  #include "xAODJet/JetContainer.h"
  #include "xAODJet/Jet.h"
  #include "xAODTau/TauJetContainer.h"
  #include "xAODTau/TauJet.h"
  #include "xAODBase/IParticleHelpers.h"
  #include "xAODBase/IParticleContainer.h"
  #include "xAODBase/IParticle.h"

  #include "AthContainers/ConstDataVector.h"
#endif

#ifndef __CINT__
  #include "xAODAnaHelpers/HelperClasses.h"
#endif

// CP interface includes
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicVariation.h"

namespace HelperFunctions {

  // primary vertex
  bool passPrimaryVertexSelection(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  int countPrimaryVertices(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  const xAOD::Vertex* getPrimaryVertex(const xAOD::VertexContainer* vertexContainer);
  int getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer);
  std::string replaceString(std::string subjet, const std::string& search, const std::string& replace);

  /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*\
  |                                                                            |
  |   Author  : Giordon Stark                                                  |
  |   Email   : gstark@cern.ch                                                 |
  |   Thanks to Ben Nachman for inspiration                                    |
  |                                                                            |
  |   jetReclustering():                                                       |
  |       Takes a set of small-R jets and reclusters to large-R jets           |
  |                                                                            |
  |       @jets                                                                |
  |         jet container to recluster and trim                                |
  |       @radius   [1.0]                                                      |
  |         radius of large-R jet                                              |
  |       @fcut     [0.05]                                                     |
  |         trimming cut to apply                                              |
  |       @rc_alg   [fastjet::kt_algorithm]                                    |
  |         clustering algorithm                                               |
  |                                                                            |
  |                                                                            |
  |   jetTrimming():                                                           |
  |       Takes a jet or a set of jets and applies an fcut on subjets          |
  |         and reclusters into a TLorentzVector                               |
  |                                                                            |
  |       @jet(s)                                                              |
  |         jet (container to loop over and) to apply trimming                 |
  |       @radius   [0.3]                                                      |
  |         radius of subjets to form                                          |
  |       @fcut     [0.05]                                                     |
  |         trimming cut to apply on subjets                                   |
  |       @s_alg    [fastjet::kt_algorithm]                                    |
  |         algorithm for building subjets                                     |
  |                                                                            |
  \*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
  std::vector<TLorentzVector> jetReclustering(
    const xAOD::JetContainer* jets,
    double radius = 1.0,
    double fcut = 0.05,
    fastjet::JetAlgorithm rc_alg = fastjet::antikt_algorithm);
  /* http://www.lpthe.jussieu.fr/~salam/fastjet/repo/doxygen-3.0alpha2/classfastjet_1_1Filter.html#usage */
  std::vector<TLorentzVector> jetTrimming(
    const xAOD::JetContainer* jets,
    double radius = 0.3,
    double fcut = 0.05,
    fastjet::JetAlgorithm s_alg = fastjet::kt_algorithm);
  TLorentzVector jetTrimming(
    const xAOD::Jet* jet,
    double radius = 0.3,
    double fcut = 0.05,
    fastjet::JetAlgorithm s_alg = fastjet::kt_algorithm);


  // miscellaneous
  bool sort_pt(xAOD::IParticle* partA, xAOD::IParticle* partB);

  std::vector< CP::SystematicSet > getListofSystematics( CP::SystematicSet recSysts, 
      std::string systName, float systVal );

#ifndef __CINT__

  /* ****************** 
  / *
  / * Marco Milesi (marco.milesi@cern.ch)
  / *
  / * Template function to copy a subset of generic input container into generic output container 
  / *   
  / *   Arguments:
  / *
  / *    -) intCont: input container
  / *    -) outCont: ouput container
  / *    -) flagSelect: a string representing a 'selection' decoration for the objects in input container (e.g. "passSel", "overlaps" ... )
  / *         default: ""
  / *    -) tool_name: enum that specifies the tool which is calling this function (defined in HelperClasses.h)
  / *  
  / *  
  / *  
  / ***************** */ 
  template< typename T1, typename T2 >
    StatusCode makeSubsetCont( T1*& intCont, T2*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name ){ 

     for ( auto in_itr : *(intCont) ) {

       if (tool_name == HelperClasses::ToolName::CALIBRATOR || tool_name == HelperClasses::ToolName::CORRECTOR) /* copy full container */
       {
         outCont->push_back( in_itr );
	 continue;
       }
      
       static SG::AuxElement::ConstAccessor<char> myAccessor(flagSelect);
       
       if(!myAccessor.isAvailable(*(in_itr))){
         std::stringstream ss; ss << in_itr->type();
	 Error("HelperFunctions::makeSubsetCont()", "flag %s is missing for object of type %s ! Will not make a subset of its container", flagSelect.c_str(), (ss.str()).c_str() );
         return StatusCode::FAILURE;
       }
       if (tool_name == HelperClasses::ToolName::OVERLAPREMOVER){ /* this tool uses reverted logic for "flagSelect", that's why I put this check */
         if ( !myAccessor(*(in_itr)) ){ outCont->push_back( in_itr ); }
       } else {
         if ( myAccessor(*(in_itr)) ) { outCont->push_back( in_itr ); }
       }

     }

     return StatusCode::SUCCESS;
    
   }
   
  /*    type_name<T>()      The awesome type demangler!
          - normally, typeid(T).name() is gibberish with gcc. This decodes it. Fucking magic man.

          @ useXAOD [true]  If set to false, will use the standard demangling
                            tool. Otherwise, use built-in StoreGate tool

        Example Usage:
        template <typename T>
        void echoType(){
          Info("retrieve()", "This is type %s", HelperFunctions::type_name<T>().c_str());
        }
  */
  template <typename T>
  std::string type_name(bool useXAOD=true) {
    if(useXAOD) return SG::normalizedTypeinfoName( typeid(T) );

    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if(status == 0) {
        tname = demangled_name;
        std::free(demangled_name);
    }
    return tname;
  }

  /*  retrieve()    retrieve an arbitrary object from TStore / TEvent
        - tries to make your life simple by providing a one-stop container retrieval shop for all types
        @ cont  : pass in a pointer to the object to store the retrieved container in
        @ name  : the name of the object to look up
        @ event : the TEvent, usually wk()->xaodEvent(). Set to 0 to not search TEvent.
        @ store : the TStore, usually wk()->xaodStore(). Set to 0 to not search TStore.
        @ debug : turn on more verbose output, helpful for debugging

      Example Usage:
      const xAOD::JetContainer* jets(0);
      // look for "AntiKt10LCTopoJets" in both TEvent and TStore
      RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(jets, "AntiKt10LCTopoJets", m_event, m_store) ,"");
      // look for "AntiKt10LCTopoJets" in only TStore
      RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(jets, "AntiKt10LCTopoJets", 0, m_store) ,"");
      // look for "AntiKt10LCTopoJets" in only TEvent, enable verbose output
      RETURN_CHECK("JetCalibrator::execute()", HelperFunctions::retrieve(jets, "AntiKt10LCTopoJets", m_event, 0, true) ,"");
  */
  template <typename T>
  StatusCode retrieve(T*& cont, std::string name, xAOD::TEvent* event, xAOD::TStore* store, bool debug=false){
    /* Checking Order:
        - check if store contains 'xAOD::JetContainer' named 'name'
        --- attempt to retrieve from store
        --- return if failure
        - check if event contains 'xAOD::JetContainer' named 'name'
        --- attempt to retrieve from event
        --- return if failure
        - return FAILURE
        return SUCCESS (should never reach this last line)
    */

    if(debug) Info("HelperFunctions::retrieve()", "\tAttempting to retrieve %s of type %s", name.c_str(), type_name<T>().c_str());
    if((store == NULL) && (debug))                      Info("HelperFunctions::retrieve()", "\t\tLooking inside: xAOD::TEvent");
    if((event == NULL) && (debug))                      Info("HelperFunctions::retrieve()", "\t\tLooking inside: xAOD::TStore");
    if((event != NULL) && (store != NULL) && (debug))   Info("HelperFunctions::retrieve()", "\t\tLooking inside: xAOD::TStore, xAOD::TEvent");
    if((store != NULL) && (store->contains<T>(name))){
      if(debug) Info("HelperFunctions::retrieve()", "\t\t\tFound inside xAOD::TStore");
      if(!store->retrieve( cont, name ).isSuccess()) return StatusCode::FAILURE;
      if(debug) Info("HelperFunctions::retrieve()", "\t\t\tRetrieved from xAOD::TStore");
    } else if((event != NULL) && (event->contains<T>(name))){
      if(debug) Info("HelperFunctions::retrieve()", "\t\t\tFound inside xAOD::TEvent");
      if(!event->retrieve( cont, name ).isSuccess()) return StatusCode::FAILURE;
      if(debug) Info("HelperFunctions::retrieve()", "\t\t\tRetrieved from xAOD::TEvent");
    } else {
      if(debug) Info("HelperFunctions::retrieve()", "\t\tNot found at all");
      return StatusCode::FAILURE;
    }
    return StatusCode::SUCCESS;
  }

  /* update with better logic
      -- call HelperFunctions::retrieve() instead
      -- if user wants `const DataVector<T>` and `ConstDataVector<T>` exists but `const DataVector<T>` does not, auto-convert for them
      -- if user wants `const DataVector<T>` and `DataVector<T>` exists, but `const DataVector<T>` does not, const-cast is trivial
      -- if user wants `DataVector<T>` and `ConstDataVector<T>` or `const DataVector<T>` exist, raise a warning
      -- **** handle the special case of EventInfo which does not have a CDV<T> equivalent
  */
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
# endif

} // close namespace HelperFunctions

# endif

