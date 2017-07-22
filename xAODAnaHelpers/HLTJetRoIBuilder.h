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

// tools
#include "AsgTools/AnaToolHandle.h"
#include "xAODAnaHelpers/OnlineBeamSpotTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

class HLTJetRoIBuilder : public xAH::Algorithm
{

  public:

    // configuration variables

    /**
      @brief name of trigger chain to load
     */
    std::string m_trigItem = "";
    std::string m_trigItemVeto = "";

    /**
      @brief Do we load a full BJet trigger chain?
     */
    bool        m_doHLTBJet = true;

    /**
      @brief Do we load a jet trigger chain?
     */
    bool        m_doHLTJet = false;

    /**
      @brief Do we read in the HLT tracks?
     */
    bool        m_readHLTTracks = true;


    /**
      @brief Do we read in the HLT vertices?
     */
    bool        m_readHLTVtx = true;


    /**
      @brief Name of the output container
     */
    std::string m_outContainerName = "";

    /** @brief trigDecTool name for configurability if name is not default.  If empty, use the default name. If not empty, change the name. */
    std::string m_trigDecTool_name{"xAH_TDT"};

  private:

    /**
      @rst
        The name of this tool (if needs to be changed) can be set with :cpp:member:`HLTJetRoIBuilder::m_trigDecTool_name`.
      @endrst
    */
    asg::AnaToolHandle<Trig::TrigDecisionTool>     m_trigDecTool_handle{"Trig::TrigDecisionTool"};                         //!
    std::string                  m_jetName = "EFJet";       //!
    std::string                  m_trkName = "InDetTrigTrackingxAODCnv_Bjet_IDTrig";       //!
    std::string                  m_vtxName = "EFHistoPrmVtx";       //!
    xAH::OnlineBeamSpotTool      m_onlineBSTool;  //!

    EL::StatusCode buildHLTBJets ();
    EL::StatusCode buildHLTJets  ();

  public:

    // this is a standard constructor
    HLTJetRoIBuilder ();

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
        ANA_MSG_ERROR("ERROR: No Trig Collection pointer");
        return 0;
      }

      if(trigCol->size() != 1){
        ANA_MSG_ERROR("ERROR Trig Collection size " << trigCol->size());
        return 0;;
      }

      return trigCol->at(0);
    }

};


#endif
