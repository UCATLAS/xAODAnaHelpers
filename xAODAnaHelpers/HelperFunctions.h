#ifndef xAODAnaHelpers_HELPERFUNCTIONS_H
#define xAODAnaHelpers_HELPERFUNCTIONS_H

// local includes
#include "AsgTools/StatusCode.h"

// jet reclustering and trimming
#include <fastjet/JetDefinition.hh>

#ifndef __CINT__
  #include "xAODTracking/VertexContainer.h"
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

#ifndef __CINT__
  template< typename T1, typename T2 >
    StatusCode makeSubsetCont( T1*& intCont, T2*& outCont, const std::string& flagSelect, HelperClasses::ToolName tool_name ){ // template function to copy a subset of intCont into outCont

     /* calibrators do not need to resize: make full copy of input container */
     if (tool_name == HelperClasses::ToolName::CALIBRATOR){
       for ( auto in_itr : *(intCont) ) {
         outCont->push_back( in_itr );
       }
     } else {
       static SG::AuxElement::ConstAccessor<char> myAccessor(flagSelect);
       for ( auto in_itr : *(intCont) ) {
         if(!myAccessor.isAvailable(*(in_itr))){
           std::stringstream ss; ss << in_itr->type();
           std::cout<< "HelperFunctions::makeSubsetCont() - flag " << flagSelect << " is missing for object of type " << (ss.str()).c_str() << " ! Will not make a subset of its container" << std::endl;
           return StatusCode::FAILURE;
         }
         if (tool_name == HelperClasses::ToolName::OVERLAPREMOVER){ // this tool uses reverted logic, that's why I put this check
           if ( !myAccessor(*(in_itr)) ){ outCont->push_back( in_itr ); }
         } else {
           if ( myAccessor(*(in_itr)) ) { outCont->push_back( in_itr ); }
         }
       }
     }

     return StatusCode::SUCCESS;

    }

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

