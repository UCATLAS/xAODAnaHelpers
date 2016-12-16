/**
 * @file   BasicEventSelection.h
 * @author Gabriel Facini <gabriel.facini@cern.ch>
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @author Jeff Dandoy <jeff.dandoy@cern.ch>
 * @author John Alison <john.alison@cern.ch>
 * @brief  Algorithm performing general basic cuts for an analysis (GRL, Event Cleaning, Min nr. Tracks for PV candidate).
 *
 */

#ifndef xAODAnaHelpers_BasicEventSelection_H
#define xAODAnaHelpers_BasicEventSelection_H

// ROOT include(s):
#include "TH1D.h"

// rootcore includes
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "PileupReweighting/PileupReweightingTool.h"
#include "AsgTools/AnaToolHandle.h"
#include "PMGTools/PMGSherpa22VJetsWeightTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

namespace TrigConf {
  class xAODConfigTool;
}

namespace Trig {
  class TrigDecisionTool;
}

class BasicEventSelection : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
  public:
    // variables read in through configuration file

    bool m_truthLevelOnly;

    // GRL
    bool m_applyGRLCut;
    std::string m_GRLxml;
    std::string m_GRLExcludeList;

    // Clean Powheg huge weight
    bool m_cleanPowheg;

    // Reweight Sherpa 2.2 Samples
    bool  m_reweightSherpa22;

    //PU Reweighting
    bool m_doPUreweighting;
    std::string m_lumiCalcFileNames;
    std::string m_PRWFileNames;

    // Unprescaling data
    bool m_savePrescaleDataWeight;

    // Primary Vertex
    std::string m_vertexContainerName;
    bool m_applyPrimaryVertexCut;
    int m_PVNTrack;

    // Event Cleaning
    bool m_applyEventCleaningCut;
    bool m_applyCoreFlagsCut;

    // Trigger
    /**
       @brief Decisions of Triggers listed in m_triggerSelection are saved and cut on depending on m_applyTriggerCut
    */
    std::string m_triggerSelection;

    /**
       @brief Decisions of Triggers listed in m_extraTriggerSelection are saved but not cut on
    */
    std::string m_extraTriggerSelection;

    bool m_applyTriggerCut;
    bool m_storeTrigDecisions;
    bool m_storePassL1;
    bool m_storePassHLT;
    bool m_storeTrigKeys;

    // Metadata
    std::string m_derivationName;
    bool m_useMetaData;

    /* Output Stream Names */

    /*
        The following public variables allow for rerouting of the
        metadata and cutflow histograms to non-default output
        streams. E.g. can combine all outputs into a single stream
    */

    std::string m_metaDataStreamName;
    std::string m_cutFlowStreamName;
    std::string m_duplicatesStreamName;

    /* Check for duplicated events in Data and MC */
    bool m_checkDuplicatesData;
    bool m_checkDuplicatesMC;
    std::set<std::pair<uint32_t,uint32_t> > m_RunNr_VS_EvtNr;

  private:

    GoodRunsListSelectionTool*   m_grl;        //!
    asg::AnaToolHandle<CP::IPileupReweightingTool>   m_pileup_tool_handle; //!
    TrigConf::xAODConfigTool*    m_trigConfTool;  //!
    Trig::TrigDecisionTool*      m_trigDecTool;   //!

    asg::AnaToolHandle<IWeightTool> m_reweightSherpa22_tool_handle; //!

    bool m_isMC;      //!

    int m_eventCounter;     //!

    // read from MetaData
    TH1D* m_histEventCount;          //!
    uint64_t m_MD_initialNevents;    //!
    uint64_t m_MD_finalNevents;      //!
    double m_MD_initialSumW;         //!
    double m_MD_finalSumW;	     //!
    double m_MD_initialSumWSquared;  //!
    double m_MD_finalSumWSquared;    //!

    // cutflow
    TH1D* m_cutflowHist;      //!
    TH1D* m_cutflowHistW;     //!
    int m_cutflow_all;        //!
    int m_cutflow_duplicates; //!
    int m_cutflow_grl;        //!
    int m_cutflow_lar;        //!
    int m_cutflow_tile;       //!
    int m_cutflow_SCT;        //!
    int m_cutflow_core;       //!
    int m_cutflow_npv;        //!
    int m_cutflow_trigger;    //!

    // object cutflow
    TH1D* m_el_cutflowHist_1;    //!
    TH1D* m_el_cutflowHist_2;    //!
    TH1D* m_mu_cutflowHist_1;    //!
    TH1D* m_mu_cutflowHist_2;    //!
    TH1D* m_ph_cutflowHist_1;    //!
    TH1D* m_tau_cutflowHist_1;   //!
    TH1D* m_tau_cutflowHist_2;   //!
    TH1D* m_jet_cutflowHist_1;   //!
    TH1D* m_trk_cutflowHist_1;   //!
    TH1D* m_truth_cutflowHist_1; //!

    /** TTree for duplicates bookeeping */
    
    TTree*   m_duplicatesTree;  //!
    int      m_duplRunNumber;
    long int m_duplEventNumber;

    // variables that don't get filled at submission time should be
    // protected from being send from the submission node to the worker
    // node (done by the //!)
  public:
    // Tree *myTree; //!
    // TH1 *myHist; //!
    //

    // this is a standard constructor
    BasicEventSelection (std::string className = "BasicEventSelection");

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
    ClassDef(BasicEventSelection, 1);
    /// @endcond
};

#endif
