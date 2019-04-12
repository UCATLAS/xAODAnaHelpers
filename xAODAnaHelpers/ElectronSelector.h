/**
 * @file   ElectronSelector.h
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @brief  Interface to the tools of the ElectronPhotonSelectorTools package.
 *
 */

#ifndef xAODAnaHelpers_ElectronSelector_H
#define xAODAnaHelpers_ElectronSelector_H

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODTracking/Vertex.h"

// package include(s):
#include "xAODAnaHelpers/ParticlePIDManager.h"

// ROOT include(s):
#include "TH1D.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "IsolationSelection/IIsolationSelectionTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// forward-declare for now until IsolationSelectionTool interface is updated
namespace CP {
  class IsolationSelectionTool;
}

/**
  @rst
    This is the algorithm class that selects electrons according to user's choice.

    In a nutshell, this algorithm performs the following actions:

      - retrieves an ``xAOD::ElectronContainer`` from either ``TEvent`` or ``TStore``
      - iterates over the input container, and if electron passes selection, copies it in a ``ConstDataVector(SG::VIEW_ELEMENTS)`` container. Otherwise, the electron is skipped
      - saves the view container to ``TStore``, from where it can be retrieved by algorithms downstream via a name lookup

  @endrst
*/
class ElectronSelector : public xAH::Algorithm
{
public:

  bool m_useCutFlow = true;

  /* configuration variables */

  /// @brief The name of the input container for this algorithm read from ``TEvent`` or ``TStore``
  std::string    m_inContainerName = "";
  /// @brief The name of the nominal output container written by the algorithm to ``TStore``
  std::string    m_outContainerName = "";

// systematics
  /**
    @brief The name of the vector containing the names of the systematically-varied containers from the upstream algorithm, which will be processed by this algorithm.

    This vector is retrieved from the ``TStore``. If left blank, it means there is no upstream algorithm which applies systematics. This is the case when processing straight from the original ``xAOD`` or ``DxAOD``.
  */
  std::string    m_inputAlgoSystNames = "";
  /**
    @brief The name of the vector containing the names of the systematically-varied containers created by by this algorithm.

    @rst
      If :cpp:member:`~xAH::Algorithm::m_systName` is empty, the vector will contain only an empty string. When running on systematics, this is the string a downstream algorithm needs to process electrons.
    @endrst
  */
  std::string    m_outputAlgoSystNames = "ElectronSelector_Syst";

  /// @brief Adds a ``passSel`` decoration for objects that pass selection
  bool       	 m_decorateSelectedObjects = true;
  /// @brief Fill using a read-only container (``SG::VIEW_ELEMENTS``) to ``TStore``
  bool       	 m_createSelectedContainer = false;
  /// @brief Number of objects to process, set ``n=-1`` to look at all
  int        	 m_nToProcess = -1;
  /// @brief Require event to have minimum number of objects passing selection
  int        	 m_pass_min = -1;
  /// @brief Require event to have maximum number of objects passing selection
  int        	 m_pass_max = -1;
  /// @brief [MeV] Require objects to have maximum transverse momentum threshold
  float      	 m_pT_max = 1e8;
  /// @brief [MeV] Require objects to have minimum transverse momentum threshold
  float      	 m_pT_min = 1e8;

  /**
    @rst
      Require objects to have maximum :math:`|\eta|` value
    @endrst
  */
  float      	 m_eta_max = 1e8;

  /**
    @rst
      Require objects to have :math:`|\eta|` outside the crack region using ``caloCluster->eta()``
    @endrst
  */
  bool	     	 m_vetoCrack = true;

  /**
    @rst
      Require objects to have a maximum :math:`d_{0}` [mm] (transverse impact parameter)
    @endrst
  */
  float      	 m_d0_max = 1e8;

  /**
    @rst
      Require objects to have a maximum :math:`d_{0}` significance at BL
    @endrst
  */
  float      	 m_d0sig_max = 1e8;

  /**
    @rst
      Require objects to have maximum :math:`z_{0}\sin(\theta)` [mm] (longitudinal impact paramter) at BL - corrected with vertex info
    @endrst
  */
  float	     	 m_z0sintheta_max = 1e8;

  /// @brief Perform author kinematic cut
  bool           m_doAuthorCut = true;
  /// @brief Perform object quality cut
  bool           m_doOQCut = true;

  ///// electron PID /////

  /** @brief To read electron PID decision from DAOD, rather than recalculate with tool */
  bool           m_readIDFlagsFromDerivation = false;

  /** @brief To correct egamma bug, see ATLSUSYSW-445 */
  bool          m_doModifiedEleId = false;

//// likelihood-based  ////
  /** @brief Instantiate and perform the electron Likelihood PID */
  bool           m_doLHPID = true;
  /** @brief Cut on electron Likelihood PID (recommended) */
  bool           m_doLHPIDcut = false;
  /** @brief Loosest Likelihood PID operating point to save */
  std::string    m_LHOperatingPoint = "Loose";

//// cut-based ////
  /** @brief Instantiate and perform the electron cut-based PID */
  bool           m_doCutBasedPID = false;
  /** @brief Cut on electron cut-based PID */
  bool           m_doCutBasedPIDcut = false;
  /** @brief Loosest cut-based PID operating point to save */
  std::string    m_CutBasedOperatingPoint = "Loose";

/* isolation */
  /** @brief reject objects which do not pass this isolation cut - default = "" (no cut) */
  std::string    m_MinIsoWPCut = "";
  /** @brief decorate objects with ``isIsolated_*`` flag for each WP in this input list - default = all current ASG WPs */
  std::string    m_IsoWPList = "FCLoose,FCTight,Gradient,FCHighPtCaloOnly";
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_CaloIsoEff = "0.1*x+90";
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_TrackIsoEff = "98";
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_CaloBasedIsoType = "topoetcone20";
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_TrackBasedIsoType = "ptvarcone20";

/* trigger matching */
  /**
    @brief A comma-separated string w/ alll the HLT single electron trigger chains for which you want to perform the matching.
          This is passed by the user as input in configuration If left empty (as it is by default), no trigger matching will be attempted at all
  */
  std::string    m_singleElTrigChains = "";
  /**
    @brief A comma-separated string w/ alll the HLT di-electron trigger chains for which you want to perform the matching.
           This is passed by the user as input in configuration If left empty (as it is by default), no trigger matching will be attempted at all
  */
  std::string    m_diElTrigChains = "";
  /// Recommended threshold for egamma triggers: see https://svnweb.cern.ch/trac/atlasoff/browser/Trigger/TrigAnalysis/TriggerMatchingTool/trunk/src/TestMatchingToolAlg.cxx
  double         m_minDeltaR = 0.07;

private:

  /**
    @brief Performs the Likelihood PID B-Layer cut locally.
    @rst
      .. note:: Occurs automatically only if :cpp:member:`~ElectronSelector::m_LHOperatingPoint` is ``LooseBL`` and :cpp:member:`~ElectronSelector::m_readIDFlagsFromDerivation` is ``true``

    @endrst
  */
  bool           m_doBLTrackQualityCut;

  /// @brief the name of the auxiliary store for the output container
  std::string    m_outAuxContainerName;

  /// @brief keep track of the total number of events processed
  int m_numEvent;           //!
  /// @brief keep track of the total number of objects processed
  int m_numObject;          //!
  /** @rst
    keep track of the number of passed events, and fill the cutflow (relevant only if using the algo to skim events: see :cpp:member:`~ElectronSelector::m_pass_max` and :cpp:member:`~ElectronSelector::m_pass_min` above)
  @endrst */
  int m_numEventPass;       //!
  /** @rst
    keep track of the number of weighted passed events, and fill the cutflow (relevant only if using the algo to skim events: see :cpp:member:`~ElectronSelector::m_pass_max` and :cpp:member:`~ElectronSelector::m_pass_min` above)
  @endrst */
  int m_weightNumEventPass; //!
  /// @brief keep track of the number of selected objects
  int m_numObjectPass;      //!

/* event-level cutflow */

  /// @brief histogram for event cutflow
  TH1D* m_cutflowHist = nullptr;      //!
  /// @brief histgram for weighted event cutflow
  TH1D* m_cutflowHistW = nullptr;     //!
  /// @brief index of bin corresponding to this step of the full cutflow
  int   m_cutflow_bin;      //!
  /// @brief checks if the algorithm has been used already
  bool  m_isUsedBefore = false;     //!

  /* object-level cutflow */

  TH1D* m_el_cutflowHist_1 = nullptr;            //!
  TH1D* m_el_cutflowHist_2 = nullptr;            //!

  int   m_el_cutflow_all;              //!
  int   m_el_cutflow_author_cut;       //!
  int   m_el_cutflow_OQ_cut;           //!
  int   m_el_cutflow_ptmax_cut;        //!
  int   m_el_cutflow_ptmin_cut;        //!
  int   m_el_cutflow_eta_cut;          //!
  int   m_el_cutflow_z0sintheta_cut;   //!
  int   m_el_cutflow_d0_cut;           //!
  int   m_el_cutflow_d0sig_cut;        //!
  int   m_el_cutflow_BL_cut;           //!
  int   m_el_cutflow_PID_cut;          //!
  int   m_el_cutflow_iso_cut;          //!

  std::vector<std::string> m_IsoKeys;  //!

  /* tools */

  /// @brief MC15 ASG tool for isolation
  asg::AnaToolHandle<CP::IIsolationSelectionTool> m_isolationSelectionTool_handle{"CP::IsolationSelectionTool/IsolationSelectionTool", this}; //!
  // this only exists because the interface needs to be updated, complain on pathelp, remove forward declaration for this when fixed
  CP::IsolationSelectionTool*                     m_isolationSelectionTool{nullptr};                                                                 //!
  asg::AnaToolHandle<Trig::TrigDecisionTool>      m_trigDecTool_handle           {"Trig::TrigDecisionTool/TrigDecisionTool"                       }; //!
  asg::AnaToolHandle<Trig::IMatchingTool>         m_trigElectronMatchTool_handle {"Trig::MatchingTool/MatchingTool"                  , this}; //!

  /// @brief This internal variable gets set to false if no triggers are defined or if TrigDecisionTool is missing
  bool m_doTrigMatch = true;

  /* PID manager(s) */
  /// @brief class to manage LH PID selection/decorations - see ISSUE for explaination
  ElectronLHPIDManager*                    m_el_LH_PIDManager = nullptr;        //!
  /// @brief class to manage cut-based PID selection/decorations - see ISSUE for explaination
  ElectronCutBasedPIDManager*              m_el_CutBased_PIDManager = nullptr;  //!

  /* other private members */

  /** @rst
    contains all the HLT trigger chains tokens extracted from :cpp:member:`~ElectronSelector::m_singleElTrigChains`
  @endrst */
  std::vector<std::string>            m_singleElTrigChainsList;  //!
  /** @rst
    contains all the HLT trigger chains tokens extracted from :cpp:member:`~ElectronSelector::m_diElTrigChains`
  @endrst */
  std::vector<std::string>            m_diElTrigChainsList;      //!

public:

  /* this is a standard constructor */

  ElectronSelector ();

  ~ElectronSelector();

  /* these are the functions inherited from Algorithm */

  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  /* added functions not from Algorithm */

  bool executeSelection( const xAOD::ElectronContainer* inElectrons, float mcEvtWeight, bool countPass,
                         ConstDataVector<xAOD::ElectronContainer>* selectedElectrons );
  virtual int passCuts( const xAOD::Electron* electron, const xAOD::Vertex *primaryVertex );

  /// @cond
  /* this is needed to distribute the algorithm to the workers */
  ClassDef(ElectronSelector, 1);
  /// @endcond
};

#endif
