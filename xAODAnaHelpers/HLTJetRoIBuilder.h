/**
 * @file   HLTJetRoIBuilder.h
 * @author John Alison <john.alison@cern.ch>
 * @brief  This class builds HLT jets and thier associated objects
 *
 */

#ifndef xAODAnaHelpers_HLTJetRoIBuilder_H
#define xAODAnaHelpers_HLTJetRoIBuilder_H


// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"
#include "TrigDecisionTool/Feature.h"

namespace Trig {
  class TrigDecisionTool;
}


class HLTJetRoIBuilder : public xAH::Algorithm
{

public:

  // configuration variables

  /**
    @brief name of trigger chain to load
   */
  std::string m_trigItem;
  std::string m_trigItemVeto;

  /**
    @brief Do we load a full BJet trigger chain?
   */
  bool        m_doHLTBJet;

  /**
    @brief Do we load a jet trigger chain?
   */
  bool        m_doHLTJet;

  /**
    @brief Do we read in the HLT tracks?
   */
  bool        m_readHLTTracks;


  /**
    @brief Name of the output container
   */
  std::string m_outContainerName;

private:

  Trig::TrigDecisionTool*      m_trigDecTool;   //!
  std::string                  m_jetName;       //!
  std::string                  m_trkName;       //!
  std::string                  m_vtxName;       //!

  EL::StatusCode buildHLTBJets ();
  EL::StatusCode buildHLTJets  ();

public:

  // this is a standard constructor
  HLTJetRoIBuilder (std::string className = "HLTJetRoIBuilder");

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();


  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(HLTJetRoIBuilder, 1);
  /// @endcond

  template<class Object, class Collection>
    const Object* getTrigObject(Trig::Feature<Collection>& feature){

    const Collection* trigCol = feature.cptr();
    if ( !trigCol ) {
      std::cout << "ERROR: No Trig Collection pointer" << std::endl;
      return 0;
    }

    if(trigCol->size() != 1){
      std::cout << "ERROR Trig Collection size " << trigCol->size() << std::endl;
      return 0;;
    }

    return trigCol->at(0);
  }

};


#endif
