#ifndef xAODAnaHelpers_HELPERFUNCTIONS_H
#define xAODAnaHelpers_HELPERFUNCTIONS_H

// for typing in template
#include <typeinfo>
#include <cxxabi.h>
// Gaudi/Athena include(s):
#include "AthContainers/normalizedTypeinfoName.h"

// local includes
#include "AsgTools/StatusCode.h"
#include <AsgTools/MessageCheck.h>

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
#include "TH1D.h"
#include "TObjArray.h"

// messaging includes
#include <AsgTools/MsgStream.h>

namespace HelperFunctions {

  /**
    Static object that provides athena-based message logging functionality
  */
  MsgStream& msg( MSG::Level lvl = MSG::INFO );

  // primary vertex
  bool passPrimaryVertexSelection(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  int countPrimaryVertices(const xAOD::VertexContainer* vertexContainer, int Ntracks = 2);
  const xAOD::Vertex* getPrimaryVertex(const xAOD::VertexContainer* vertexContainer, MsgStream& msg);
  inline const xAOD::Vertex* getPrimaryVertex(const xAOD::VertexContainer* vertexContainer) { return getPrimaryVertex(vertexContainer, msg()); }
  float getPrimaryVertexZ(const xAOD::Vertex* pvx);
  int getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer, MsgStream& msg);
  inline int getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer){ return getPrimaryVertexLocation(vertexContainer, msg()); }
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
  std::string parse_wp( const std::string& type, const std::string& config_name, MsgStream& msg );
  inline std::string parse_wp( const std::string& type, const std::string& config_name ) { return parse_wp(type, config_name, msg()); }

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

  /**
    @brief Get a list of systematics
    @param inSysts    systematics set retrieved from the tool
    @param systNames  comma separated list of wanted systematics names, use "Nominal" for nominal and "All" for all systematics
    @param systVal    continuous systematics sigma value
    @param msg        the MsgStream object with appropriate level for debugging
  */
  std::vector< CP::SystematicSet > getListofSystematics( const CP::SystematicSet inSysts, std::string systNames, float systVal, MsgStream& msg );

  void writeSystematicsListHist( const std::vector< CP::SystematicSet > &systs, std::string histName, TFile *file );

  /*    type_name<T>()      The awesome type demangler!
          - normally, typeid(T).name() is gibberish with gcc. This decodes it. Fucking magic man.

          @ useXAOD [true]  If set to false, will use the standard demangling
                            tool. Otherwise, use built-in StoreGate tool

        Example Usage:
        template <typename T>
        void echoType(){
          std::cout << "This is type " << HelperFunctions::type_name<T>() << std::endl;
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
  StatusCode makeSubsetCont( T1*& intCont, T2*& outCont, MsgStream& msg, const std::string& flagSelect = "", HelperClasses::ToolName tool_name = HelperClasses::ToolName::DEFAULT){
     std::string funcName{"in makeSubsetCont<"+type_name<T1>()+","+type_name<T2>()+">(): "};

     if ( tool_name == HelperClasses::ToolName::DEFAULT ) {

       for ( auto in_itr : *(intCont) ) { outCont->push_back( in_itr ); }
       return StatusCode::SUCCESS;

     }

     if ( flagSelect.empty() ) {
       msg << MSG::ERROR << funcName << "flagSelect is an empty string, and passing a non-DEFAULT tool (presumably a SELECTOR). Please pass a non-empty flagSelect!" << endmsg;
       return StatusCode::FAILURE;
     }

     SG::AuxElement::ConstAccessor<char> myAccessor(flagSelect);

     for ( auto in_itr : *(intCont) ) {

       if ( !myAccessor.isAvailable(*(in_itr)) ) {
     	 std::stringstream ss; ss << in_itr->type();
         msg << MSG::ERROR << funcName << "flag " << flagSelect << " is missing for object of type " << ss.str() << " ! Will not make a subset of its container" << endmsg;
     	 return StatusCode::FAILURE;
       }

     	 if ( myAccessor(*(in_itr)) ) { outCont->push_back( in_itr ); }

     }

     return StatusCode::SUCCESS;

   }
  template< typename T1, typename T2 >
  StatusCode makeSubsetCont( T1*& intCont, T2*& outCont, const std::string& flagSelect = "", HelperClasses::ToolName tool_name = HelperClasses::ToolName::DEFAULT) { return makeSubsetCont<T1, T2>(intCont, outCont, msg(), flagSelect, tool_name); }

  /** @brief Retrieve an arbitrary object from TStore / TEvent
    @param cont  pass in a pointer to the object to store the retrieved container in
    @param name  the name of the object to look up
    @param event the TEvent, usually wk()->xaodEvent(). Set to 0 to not search TEvent.
    @param store the TStore, usually wk()->xaodStore(). Set to 0 to not search TStore.
    @param msg   the MsgStream object with appropriate level for debugging

    @rst
      This tries to make your life simple by providing a one-stop container retrieval shop for all types.

      Example Usage::

        const xAOD::JetContainer* jets(0);
        // look for "AntiKt10LCTopoJets" in both TEvent and TStore
        ANA_CHECK( HelperFunctions::retrieve(jets, "AntiKt10LCTopoJets", m_event, m_store) );
        // look for "AntiKt10LCTopoJets" in only TStore
        ANA_CHECK( HelperFunctions::retrieve(jets, "AntiKt10LCTopoJets", 0, m_store) );
        // look for "AntiKt10LCTopoJets" in only TEvent, enable verbose output
        ANA_CHECK( HelperFunctions::retrieve(jets, "AntiKt10LCTopoJets", m_event, 0, msg()) );

      Checking Order:

      - start by checking TStore

        - check if store contains 'xAOD::JetContainer' named 'name'

          - attempt to retrieve from store
          - return if failure

      - next check TEvent

        - check if event contains 'xAOD::JetContainer' named 'name'

          - attempt to retrieve from event
          - return if failure

        - return FAILURE

      - return SUCCESS (should never reach this last line)

    @endrst
  */
  template <typename T>
  StatusCode retrieve(T*& cont, std::string name, xAOD::TEvent* event, xAOD::TStore* store, MsgStream& msg){
    std::string funcName{"in retrieve<"+type_name<T>()+">(" + name + "): "};
    msg << MSG::DEBUG << funcName << "\tAttempting to retrieve " << name << " of type " << type_name<T>() << endmsg;
    if(store == NULL)                      msg << MSG::DEBUG << funcName << "\t\tLooking inside: xAOD::TEvent" << endmsg;
    if(event == NULL)                      msg << MSG::DEBUG << funcName << "\t\tLooking inside: xAOD::TStore" << endmsg;
    if((event != NULL) && (store != NULL)) msg << MSG::DEBUG << funcName << "\t\tLooking inside: xAOD::TStore, xAOD::TEvent" << endmsg;
    if((store != NULL) && (store->contains<T>(name))){
      msg << MSG::DEBUG << funcName << "\t\t\tFound inside xAOD::TStore" << endmsg;
      if(!store->retrieve( cont, name ).isSuccess()) return StatusCode::FAILURE;
      msg << MSG::DEBUG << funcName << "\t\t\tRetrieved from xAOD::TStore" << endmsg;
    } else if((event != NULL) && (event->contains<T>(name))){
      msg << MSG::DEBUG << funcName << "\t\t\tFound inside xAOD::TEvent" << endmsg;
      if(!event->retrieve( cont, name ).isSuccess()) return StatusCode::FAILURE;
      msg << MSG::DEBUG << funcName << "\t\t\tRetrieved from xAOD::TEvent" << endmsg;
    } else {
      msg << MSG::DEBUG << funcName << "\t\tNot found at all" << endmsg;
      return StatusCode::FAILURE;
    }
    return StatusCode::SUCCESS;
  }
  /* retrieve() overload for no msgStream object passed in */
  template <typename T>
  StatusCode retrieve(T*& cont, std::string name, xAOD::TEvent* event, xAOD::TStore* store) { return retrieve<T>(cont, name, event, store, msg()); }
  template <typename T>
  StatusCode retrieve(T*& cont, std::string name, xAOD::TEvent* event, xAOD::TStore* store, bool debug) { msg() << MSG::WARNING << "retrieve<T>(..., bool) is deprecated. See https://github.com/UCATLAS/xAODAnaHelpers/pull/882" << endmsg; return retrieve<T>(cont, name, event, store, msg()); }

  /** @brief Return true if an arbitrary object from TStore / TEvent is available
    @param name  the name of the object to look up
    @param event the TEvent, usually wk()->xaodEvent(). Set to 0 to not search TEvent.
    @param store the TStore, usually wk()->xaodStore(). Set to 0 to not search TStore.
    @param msg   the MsgStream object with appropriate level for debugging

    @rst

      This tries to make your life simple by providing a one-stop container check shop for all types

      Example Usage::

        const xAOD::JetContainer* jets(0);
        // look for "AntiKt10LCTopoJets" in both TEvent and TStore
        HelperFunctions::isAvailable<xAOD::JetContainer>("AntiKt10LCTopoJets", m_event, m_store)
        // look for "AntiKt10LCTopoJets" in only TStore
        HelperFunctions::isAvailable<xAOD::JetContainer>("AntiKt10LCTopoJets", 0, m_store)
        // look for "AntiKt10LCTopoJets" in only TEvent, enable verbose output
        HelperFunctions::isAvailable<xAOD::JetContainer>("AntiKt10LCTopoJets", m_event, 0, MSG::VERBOSE)

    @endrst
  */
  template <typename T>
  bool isAvailable(std::string name, xAOD::TEvent* event, xAOD::TStore* store, MsgStream& msg){
    /* Checking Order:
        - check if store contains 'xAOD::JetContainer' named 'name'
        --- checkstore store
        - check if event contains 'xAOD::JetContainer' named 'name'
        --- checkstore event
    */
    std::string funcName{"in isAvailable<"+type_name<T>()+">(" + name + "): "};
    msg << MSG::DEBUG << funcName << "\tAttempting to retrieve " << name << " of type " << type_name<T>() << endmsg;
    if(store == NULL)                      msg << MSG::DEBUG << funcName << "\t\tLooking inside: xAOD::TEvent" << endmsg;
    if(event == NULL)                      msg << MSG::DEBUG << funcName << "\t\tLooking inside: xAOD::TStore" << endmsg;
    if((event != NULL) && (store != NULL)) msg << MSG::DEBUG << funcName << "\t\tLooking inside: xAOD::TStore, xAOD::TEvent" << endmsg;
    if((store != NULL) && (store->contains<T>(name))){
      msg << MSG::DEBUG << funcName << "\t\t\tFound inside xAOD::TStore" << endmsg;;
      return true;
    } else if((event != NULL) && (event->contains<T>(name))){
      msg << MSG::DEBUG << funcName << "\t\t\tFound inside xAOD::TEvent" << endmsg;
      return true;
    } else {
      return false;
    }
    return false;
  }
  /* isAvailable() overload for no msgStream object passed in */
  template <typename T>
  bool isAvailable(std::string name, xAOD::TEvent* event, xAOD::TStore* store) { return isAvailable<T>(name, event, store, msg()); }

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
        ANA_CHECK( m_event->retrieve( selected_jets, "SelectedJets" ));
        ANA_CHECK( (HelperFunctions::makeDeepCopy<xAOD::JetContainer, xAOD::JetAuxContainer, xAOD::Jet>(m_store, "BaselineJets", selected_jets)));

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

        ANA_CHECK( HelperFunctions::recordOutput<xAOD::JetContainer, xAOD::JetAuxContainer>(m_event, m_store, "BaselineJets"));

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

  // function to remove duplicates from a vector. Thank you, oh internet!
  // http://stackoverflow.com/questions/9237216/removing-duplicates-in-a-vector-of-strings
  template <typename T>
  void remove_duplicates(std::vector<T>& vec)
  {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

} // close namespace HelperFunctions

# endif
