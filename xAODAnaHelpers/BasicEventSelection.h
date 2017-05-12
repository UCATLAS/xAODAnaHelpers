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

/**
  @rst
    This algorithm performs the very basic event selection. This should be the first algo in the algo chain. It can create weighted and unweighted cutflow objects to be picked up downstream by other xAH algos, and your own. The selection applied in data only is:

      - GRL (can be turned off)
      - LAr Error
      - Tile Error
      - Core Flag

    .. note:: For MC only, the pileup reweight can also be applied.

    In both data and simulation (MC), the following cuts are applied

      - the highest sum :math:`p_{T}^2` primary vertex has 2 or more tracks (see :cpp:member:`~BasicEventSelection::m_applyPrimaryVertexCut`)
      - trigger requirements (see :cpp:member:`~BasicEventSelection::m_applyTriggerCut`)

    For derivations, the metadata can be accessed and added to the cutflow for normalization. The parameters to control the trigger are described in this header file. If one wants to write out some of the trigger information into a tree using :cpp:class:`~HelpTreeBase`, flags must be set here.

  @endrst
*/
class BasicEventSelection : public xAH::Algorithm
{
  public:
    /// @brief Protection when running on truth xAOD
    bool m_truthLevelOnly;

  // GRL
    /// @brief Apply GRL selection
    bool m_applyGRLCut;
    /// @brief Path to GRL XML file
    std::string m_GRLxml;
    /// @brief Run numbers to skip in GRL
    std::string m_GRLExcludeList;

  // Clean Powheg huge weight
    bool m_cleanPowheg;

  // Reweight Sherpa 2.2 Samples
    bool  m_reweightSherpa22;

  //PU Reweighting
    /**
      @rst
        Reweight pile-up profile :math:`\mu`
      @endrst
    */
    bool m_doPUreweighting;
    bool m_doPUreweightingSys;
    /// @brief Comma separated list of filenames
    std::string m_lumiCalcFileNames;
    /// @brief Comma separated list of filenames
    std::string m_PRWFileNames;

    // Unprescaling data
    bool m_savePrescaleDataWeight;

  // Primary Vertex
    /// @brief Name of vertex container
    std::string m_vertexContainerName;
    /// @brief Enable to apply a primary vertex cut
    bool m_applyPrimaryVertexCut;
    /// @brief Minimum number of tracks from **the** primary vertex (Harmonized Cut)
    int m_PVNTrack;

  // Event Cleaning
    bool m_applyEventCleaningCut;
    bool m_applyCoreFlagsCut;

    // Print Branch List
    bool m_printBranchList;

  // Trigger
    /**
      @rst
        RegEx expression to choose triggers to consider to be cut on with :cpp:member:`~BasicEventSelection::m_applyTriggerCut`
      @endrst
    */
    std::string m_triggerSelection;

    /// @brief Decisions of triggers which are saved but not cut on
    std::string m_extraTriggerSelection;

    /**
      @rst
        Skip events in which the trigger string :cpp:member:`~BasicEventSelection::m_triggerSelection` does not fire
      @endrst
    */
    bool m_applyTriggerCut;
    /**
      @rst
        Save string of fired triggers matching :cpp:member:`~BasicEventSelection::m_triggerSelection`
      @endrst
    */
    bool m_storeTrigDecisions;
    /// @brief Save if any L1 trigger fired, e.g. ``"L1_.*"``
    bool m_storePassL1;
    /// @brief Save if any HLT trigger fired, e.g. ``"HLT_.*"``
    bool m_storePassHLT;
    /// @brief Save master, L1, and HLT key
    bool m_storeTrigKeys;

  // Metadata
    /// @brief The name of the derivation
    std::string m_derivationName;
    /// @brief Retrieve and save information on DAOD selection
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
    BasicEventSelection ();

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
