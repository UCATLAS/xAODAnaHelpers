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
#include "xAODJet/JetContainer.h"

#include "xAODTracking/VertexContainer.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODAnaHelpers/HelperClasses.h"

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// ROOT includes
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TObjArray.h"


namespace HelperFunctions {

  // primary vertex
  bool passPrimaryVertexSelection(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  int countPrimaryVertices(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  const xAOD::Vertex* getPrimaryVertex(const xAOD::VertexContainer* vertexContainer, bool quiet=false);
  float getPrimaryVertexZ(const xAOD::Vertex* pvx);
  int getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer);
  bool applyPrimaryVertexSelection( const xAOD::JetContainer* jets, const xAOD::VertexContainer* vertices );
  float GetBTagMV2c20_Cut( int efficiency );
  std::string GetBTagMV2c20_CutStr( int efficiency );
  std::string replaceString(std::string subjet, const std::string& search, const std::string& replace);
  std::vector<TString> SplitString(TString& orig, const char separator);
  float dPhi(float phi1, float phi2);
  bool has_exact(const std::string input, const std::string flag); 

  /**
    Function which returns the position of the n-th occurence of a character in a string searching backwards.
    Returns -1 if no occurencies are found.

    Source: http://stackoverflow.com/questions/18972258/index-of-nth-occurrence-of-the-string
  */
  std::size_t string_pos( const std::string& inputstr, const char& occurence, int n_occurencies );

  /**
    Function which returns the WP for ISO/ID from a config file.
    Returns empty string if no WP is found.

  */  
  std::string parse_wp( const std::string& type, const std::string& config_name );

  /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*\
  |                                                                            |
  |   Author  : Marco Milesi                                                   |
  |   Email   : marco.milesi@cern.ch                                           |
  |   Logic copied from:
  	PhysicsAnalysis/TopPhys/xAOD/TopAnalysis/trunk/Root/Tools.cxx          |

  Function to check if file is Full xAOD

  \*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

  StatusCode isAvailableMetaData(TTree* metaData);
  bool isFilePrimaryxAOD(TFile* inputFile);

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

  std::vector< CP::SystematicSet > getListofSystematics( const CP::SystematicSet inSysts, std::string systName, float systVal, bool debug = false );

  /**
   * @author Marco Milesi (marco.milesi@cern.ch)
   * @brief Function to copy a subset of a generic input xAOD container into a generic output xAOD container.
   *
   * If the optional parameters aren't specified, the function will just make a full copy of the input
   * container into the output one.
   *
   * @param [in] intCont input container
   * @param [in,out] outCont output container
   * @param [in] flagSelect (optional) the name of the decoration for objects passing a certain selection (e.g. "passSel", "overlaps" ...). When explicitly specified, it must not be empty.
   * @param [in] tool_name (optional) an enum specifying the tool type which is calling this function (definition in `HelperClasses::ToolName`)
   */
  template< typename T1, typename T2 >
    StatusCode makeSubsetCont( T1*& intCont, T2*& outCont, const std::string& flagSelect = "", HelperClasses::ToolName tool_name = HelperClasses::ToolName::DEFAULT ){

     if ( tool_name == HelperClasses::ToolName::DEFAULT ) {

       for ( auto in_itr : *(intCont) ) { outCont->push_back( in_itr ); }
       return StatusCode::SUCCESS;

     }

     if ( flagSelect.empty() ) {
       Error("HelperFunctions::makeSubsetCont()", "flagSelect is an empty string, and passing a non-DEFAULT tool (presumably a SELECTOR). Please pass a non-empty flagSelect!" );
       return StatusCode::FAILURE;
     }

     SG::AuxElement::ConstAccessor<char> myAccessor(flagSelect);

     for ( auto in_itr : *(intCont) ) {

       if ( !myAccessor.isAvailable(*(in_itr)) ) {
     	 std::stringstream ss; ss << in_itr->type();
         Error("HelperFunctions::makeSubsetCont()", "flag %s is missing for object of type %s ! Will not make a subset of its container", flagSelect.c_str(), (ss.str()).c_str() );
     	 return StatusCode::FAILURE;
       }

     	 if ( myAccessor(*(in_itr)) ) { outCont->push_back( in_itr ); }

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
    Warning("HelperFunctions::getContainer()", "THIS IS BEING DEPRECATED. PLEASE USE HelperFunctions::retrieve() INSTEAD!!!!");
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

  // stolen from here
  // https://svnweb.cern.ch/trac/atlasoff/browser/Event/xAOD/xAODEgamma/trunk/xAODEgamma/EgammaTruthxAODHelpers.h#L20
  // util becomes a general xAOD tool
  /// @brief Access to element link to object of type T stored in auxdata
  template<class T>
    const T* getLink(const xAOD::IParticle* particle, std::string name){
      if (!particle) return 0;
      typedef ElementLink< DataVector<T> > Link_t;

      if (!particle->isAvailable< Link_t >(name) ) {
        return 0;
      }
      const Link_t link = particle->auxdata<Link_t>(name);
      if (!link.isValid()) {
        return 0;
      }
      return *link;
    }

  //
  // For Sorting
  //
  struct pt_sort
  {

    inline bool operator() (const TLorentzVector& lhs, const TLorentzVector& rhs)
    {
      return (lhs.Pt() > rhs.Pt());
    }

    inline bool operator() (const TLorentzVector* lhs, const TLorentzVector* rhs)
    {
      return (lhs->Pt() > rhs->Pt());
    }

    inline bool operator() (const xAOD::IParticle& lhs, const xAOD::IParticle& rhs)
    {
      return (lhs.pt() > rhs.pt());
    }

    inline bool operator() (const xAOD::IParticle* lhs, const xAOD::IParticle* rhs)
    {
      return (lhs->pt() > rhs->pt());
    }
  };


  template<typename T>
    T sort_container_pt(T* inCont){
    T sortedCont(SG::VIEW_ELEMENTS);
    for(auto el : *inCont) sortedCont.push_back( el );
    std::sort(sortedCont.begin(), sortedCont.end(), pt_sort());
    return sortedCont;
  }

  template<typename T>
    const T sort_container_pt(const T* inCont){
    ConstDataVector<T> sortedCont(SG::VIEW_ELEMENTS);

    for(auto el : *inCont) sortedCont.push_back( el );
    std::sort(sortedCont.begin(), sortedCont.end(), pt_sort());
    return *sortedCont.asDataVector();
  }

  /* return true if there's a least one non-empty string (i.e., syst name) in input list */
  inline bool found_non_dummy_sys(std::vector<std::string>* sys_list) {
    if ( sys_list ) {
      for ( auto sys : *sys_list ) {
	if ( !sys.empty() ) { return true; }
      }
    }
    return false;
  }

  /**
    @brief Make a deep copy of a container and put it in the TStore
    @tparam T1              The type of the container you're going to deep copy into
    @tparam T2              The type of the aux container you're going to deep copy into
    @tparam T3              The type of the object inside the container you're going to deep copy
    @param m_store          A pointer to the TStore object
    @param containerName    The name of the container to create as output in the TStore
    @param cont             The container to deep copy, it should be a container of pointers (IParticleContainer or ConstDataVector)

    @rst
      This is a very powerful templating function. The point is to remove the triviality of making deep copies by specifying all that is needed. The best way is to demonstrate via example::

        const xAOD::JetContainer* selected_jets(nullptr);
        RETURN_CHECK("execute()", m_event->retrieve( selected_jets, "SelectedJets" ));
        RETURN_CHECK("execute()", HelperFunctions::makeDeepCopy<xAOD::JetContainer, xAOD::JetAuxContainer, xAOD::Jet>(m_store, "BaselineJets", selected_jets));

    @endrst
   */
  template <typename T1, typename T2, typename T3>
  StatusCode makeDeepCopy(xAOD::TStore* m_store, std::string containerName, const T1* cont){
    T1* cont_new = new T1;
    T2* auxcont_new = new T2;
    cont_new->setStore(auxcont_new);

    if(!m_store->record(cont_new, containerName).isSuccess()){
      std::cout << "can't record " << containerName << std::endl;
      return StatusCode::FAILURE;
    }
    if(!m_store->record(auxcont_new, containerName+"Aux.").isSuccess()){
      std::cout << "can't record aux" << containerName << std::endl;
      return StatusCode::FAILURE;
    }

    for(const auto p: *cont){
      T3* p_new = new T3;
      cont_new->push_back(p_new);
      *p_new = *p;
    }
    return StatusCode::SUCCESS;
  }

  /**
    @brief Copy a container from the TStore to be recorded in the TEvent (eg: to an output)
    @tparam T1              The type of the container you're going to record
    @tparam T2              The type of the aux container you're going to record
    @param m_event          A pointer to the TEvent object
    @param m_store          A pointer to the TStore object
    @param containerName    The name of the container in the TStore to record to TEvent

    @rst
      If you have a container in the TStore, this function will record it into the output for you without an issue. As an example::

        RETURN_CHECK("execute()", HelperFunctions::recordOutput<xAOD::JetContainer, xAOD::JetAuxContainer>(m_event, m_store, "BaselineJets"));

      where we build off the previous example of making a deep copy (see :cpp:func:`HelperFunctions::makeDeepCopy`).
    @endrst
   */
  template <typename T1, typename T2>
  StatusCode recordOutput(xAOD::TEvent* m_event, xAOD::TStore* m_store, std::string containerName){
    T1* cont(nullptr);
    T2* auxcont(nullptr);

    if(!m_store->retrieve(cont, containerName).isSuccess()) return StatusCode::FAILURE;
    if(!m_store->retrieve(auxcont, containerName+"Aux.").isSuccess()) return StatusCode::FAILURE;

    if(!m_event->record(cont, containerName).isSuccess()) return StatusCode::FAILURE;
    if(!m_event->record(auxcont, containerName+"Aux.").isSuccess()) return StatusCode::FAILURE;
    return StatusCode::SUCCESS;
  }

  template <typename T_BR> void connectBranch(std::string name, TTree *tree, const std::string& branch, std::vector<T_BR> **variable)
    {
      tree->SetBranchStatus  ((name+"_"+branch).c_str()  , 1);
      tree->SetBranchAddress ((name+"_"+branch).c_str()  , variable);
    }


} // close namespace HelperFunctions

# endif

