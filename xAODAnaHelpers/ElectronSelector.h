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
#include "IsolationSelection/IsolationSelectionTool.h"
#include "AsgTools/AnaToolHandle.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

namespace Trig {
  class TrigDecisionTool;
  class MatchingTool;
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

  bool m_useCutFlow;

  /* configuration variables */

  /// @brief The name of the input container for this algorithm read from ``TEvent`` or ``TStore``
  std::string    m_inContainerName;
  /// @brief The name of the nominal output container written by the algorithm to ``TStore``
  std::string    m_outContainerName;

// systematics
  /**
    @brief The name of the vector containing the names of the systematically-varied containers from the upstream algorithm, which will be processed by this algorithm.

    This vector is retrieved from the ``TStore``. If left blank, it means there is no upstream algorithm which applies systematics. This is the case when processing straight from the original ``xAOD`` or ``DxAOD``.
  */
  std::string    m_inputAlgoSystNames;
  /**
    @brief The name of the vector containing the names of the systematically-varied containers created by by this algorithm.

    @rst
      If :cpp:member:`~xAH::Algorithm::m_systName` is empty, the vector will contain only an empty string. When running on systematics, this is the string a downstream algorithm needs to process electrons.
    @endrst
  */
  std::string    m_outputAlgoSystNames;

  /// @brief Adds a ``passSel`` decoration for objects that pass selection
  bool       	 m_decorateSelectedObjects;
  /// @brief Fill using a read-only container (``SG::VIEW_ELEMENTS``) to ``TStore``
  bool       	 m_createSelectedContainer;
  /// @brief Number of objects to process, set ``n=-1`` to look at all
  int        	 m_nToProcess;
  /// @brief Require event to have minimum number of objects passing selection
  int        	 m_pass_min;
  /// @brief Require event to have maximum number of objects passing selection
  int        	 m_pass_max;
  /// @brief [MeV] Require objects to have maximum transverse momentum threshold
  float      	 m_pT_max;
  /// @brief [MeV] Require objects to have minimum transverse momentum threshold
  float      	 m_pT_min;
  /**
    @rst
      Require objects to have maximum :math:`|\eta|` value
    @endrst
  */
  float      	 m_eta_max;
  /**
    @rst
      Require objects to have :math:`|\eta|` outside the crack region using ``caloCluster->eta()``
    @endrst
  */
  bool	     	 m_vetoCrack;
  /**
    @rst
      Require objects to have a maximum :math:`d_{0}` [mm] (transverse impact parameter)
    @endrst
  */
  float      	 m_d0_max;
  /**
    @rst
      Require objects to have a maximum :math:`d_{0}` significance at BL
    @endrst
  */
  float      	 m_d0sig_max;
  /**
    @rst
      Require objects to have maximum :math:`z_{0}\sin(\theta)` [mm] (longitudinal impact paramter) at BL - corrected with vertex info
    @endrst
  */
  float	     	 m_z0sintheta_max;
  /// @brief Perform author kinematic cut
  bool           m_doAuthorCut;
  /// @brief Perform object quality cut
  bool           m_doOQCut;

  ///// electron PID /////

  /** @brief To read electron PID decision from DAOD, rather than recalculate with tool */
  bool           m_readIDFlagsFromDerivation;
  /**
    @brief Performs the Likelihood PID B-Layer cut locally.
    @rst
      .. note:: Occurs automatically only if :cpp:member:`~ElectronSelector::m_LHOperatingPoint` is ``LooseBL`` and :cpp:member:`~ElectronSelector::m_readIDFlagsFromDerivation` is ``true``

    @endrst
  */
  bool           m_doBLTrackQualityCut;

//// likelihood-based  ////
  /** @brief Instantiate and perform the electron Likelihood PID */
  bool           m_doLHPID;
  /** @brief Cut on electron Likelihood PID (recommended) */
  bool           m_doLHPIDcut;
  /** @brief Loosest Likelihood PID operating point to save */
  std::string    m_LHOperatingPoint;

//// cut-based ////
  /** @brief Instantiate and perform the electron cut-based PID */
  bool           m_doCutBasedPID;
  /** @brief Cut on electron cut-based PID */
  bool           m_doCutBasedPIDcut;
  /** @brief Loosest cut-based PID operating point to save */
  std::string    m_CutBasedOperatingPoint;

/* isolation */
  /** @brief reject objects which do not pass this isolation cut - default = "" (no cut) */
  std::string    m_MinIsoWPCut;
  /** @brief decorate objects with ``isIsolated_*`` flag for each WP in this input list - default = all current ASG WPs */
  std::string    m_IsoWPList;
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_CaloIsoEff;
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_TrackIsoEff;
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_CaloBasedIsoType;
  /** @rst
     to define a custom WP - make sure ``"UserDefined"`` is added in :cpp:member:`~ElectronSelector::m_IsoWPList`
  @endrst */
  std::string    m_TrackBasedIsoType;

/* trigger matching */
  std::string    m_singleElTrigChains;   /* A comma-separated string w/ alll the HLT single electron trigger chains for which you want to perform the matching.
  			      	            This is passed by the user as input in configuration
				            If left empty (as it is by default), no trigger matching will be attempted at all */
  std::string    m_diElTrigChains;       /* A comma-separated string w/ alll the HLT di-electron trigger chains for which you want to perform the matching.
  			   	            This is passed by the user as input in configuration
			                    If left empty (as it is by default), no trigger matching will be attempted at all */
  double         m_minDeltaR;

private:

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
  TH1D* m_cutflowHist;      //!
  /// @brief histgram for weighted event cutflow
  TH1D* m_cutflowHistW;     //!
  /// @brief index of bin corresponding to this step of the full cutflow
  int   m_cutflow_bin;      //!

  bool  m_isUsedBefore;     //!

  /* object-level cutflow */

  TH1D* m_el_cutflowHist_1;            //!
  TH1D* m_el_cutflowHist_2;            //!

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
  asg::AnaToolHandle<CP::IsolationSelectionTool> m_isolationSelectionTool_handle; //!
  std::string m_isolationSelectionTool_name;                                      //!

  /* PID manager(s) */
  /// @brief class to manage LH PID selection/decorations - see ISSUE for explaination
  ElectronLHPIDManager*                    m_el_LH_PIDManager;        //!
  /// @brief class to manage cut-based PID selection/decorations - see ISSUE for explaination
  ElectronCutBasedPIDManager*              m_el_CutBased_PIDManager;  //!
  Trig::TrigDecisionTool*                  m_trigDecTool;             //!
  asg::AnaToolHandle<Trig::MatchingTool>   m_trigElectronMatchTool_handle;  //!
  std::string m_trigElMatchTool_name;                                 //!
  bool m_doTrigMatch;

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
