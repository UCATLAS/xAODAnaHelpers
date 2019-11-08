/**
 * @file   MuonSelector.h
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @brief  Interface to the tools of the MuonSelectorTools package.
 *
 */

#ifndef xAODAnaHelpers_MuonSelector_H
#define xAODAnaHelpers_MuonSelector_H

// EDM include(s):
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/Vertex.h"

// ROOT include(s):
#include "TH1D.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "IsolationSelection/IIsolationSelectionTool.h"
#include "MuonAnalysisInterfaces/IMuonSelectionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// forward-declare for now until IsolationSelectionTool interface is updated
namespace CP {
  class IsolationSelectionTool;
}

class MuonSelector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.

public:

  // cutflow
  bool m_useCutFlow = true;

  // configuration variables
  /** input container name */
  std::string    m_inContainerName = "";
  /** output container name */
  std::string    m_outContainerName = "";
  /** output auxiliary container name */
  std::string    m_outAuxContainerName;
  std::string    m_inputAlgoSystNames = "";
  std::string    m_outputAlgoSystNames = "MuonSelector_Syst";
  /** decorate selected objects - default "passSel" */
  bool       	 m_decorateSelectedObjects = true;
  /** fill using SG::VIEW_ELEMENTS to be light weight */
  bool       	 m_createSelectedContainer = false;
  /** look at n objects */
  int            m_nToProcess = -1;
  /** minimum number of objects passing cuts */
  int            m_pass_min = -1;
  /** maximum number of objects passing cuts */
  int            m_pass_max = -1;
  /** require pT < pt_max */
  float          m_pT_max = 1e8;
  /** require pT > pt_min */
  float          m_pT_min = 1e8;
  /** require quality */
  std::string    m_muonQualityStr = "Medium";
  /** require type */
  //std::string    m_muonType;
  /** require |eta| < eta_max */
  float          m_eta_max = 1e8;
  /** require d0 < m_d0_max */
  float          m_d0_max = 1e8;
  /** require d0 significance (at BL) < m_d0sig_max */
  float          m_d0sig_max = 1e8;
  /** require z0*sin(theta) (at BL - corrected with vertex info) < m_z0sintheta_max */
  float	         m_z0sintheta_max = 1e8;

  /** @brief Remove cosmic muons that fail absolute z0 and d0 selections */
  bool           m_removeCosmicMuon = false;
  /** @brief Remove events with a bad muon, defined by poor q/p */
  bool           m_removeEventBadMuon = true;

  // isolation
  /** enable or disable isolation **/
  bool           m_doIsolation = true;
  /** reject objects which do not pass this isolation cut - default = "" (no cut) */
  std::string    m_MinIsoWPCut = "";
  /** decorate objects with 'isIsolated_*' flag for each WP in this input list - default = all current ASG WPs */
  std::string    m_IsoWPList = "FCTightTrackOnly_FixedRad,FCLoose_FixedRad,FCTight_FixedRad,FixedCutPflowTight,FixedCutPflowLoose";
  /** to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_CaloIsoEff = "0.1*x+90";
  /** to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_TrackIsoEff = "98";
  /** to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_CaloBasedIsoType = "topoetcone20";
  /** to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_TrackBasedIsoType = "ptvarcone30";

  /* trigger matching */
  /** A comma-separated string w/ alll the HLT single muon trigger chains for which you want to perform the matching. If left empty (as it is by default), no trigger matching will be attempted at all */
  std::string    m_singleMuTrigChains = "";
  /** A comma-separated string w/ all the HLT dimuon trigger chains for which you want to perform the matching.  If left empty (as it is by default), no trigger matching will be attempted at all */
  std::string    m_diMuTrigChains = "";
  /// @brief Recommended threshold for muon triggers: see https://svnweb.cern.ch/trac/atlasoff/browser/Trigger/TrigAnalysis/TriggerMatchingTool/trunk/src/TestMatchingToolAlg.cxx
  double         m_minDeltaR = 0.1;

private:

  int            m_muonQuality; //!

  int m_numEvent;           //!
  int m_numObject;          //!
  int m_numEventPass;       //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;      //!

  // cutflow
  TH1D* m_cutflowHist = nullptr;      //!
  TH1D* m_cutflowHistW = nullptr;     //!
  int   m_cutflow_bin;      //!

  bool  m_isUsedBefore = false;     //!

  // object cutflow
  TH1D* m_mu_cutflowHist_1 = nullptr;                 //!
  TH1D* m_mu_cutflowHist_2 = nullptr;                 //!

  int   m_mu_cutflow_all;		    //!
  int   m_mu_cutflow_eta_and_quaility_cut;  //!
  int   m_mu_cutflow_ptmax_cut;  	    //!
  int   m_mu_cutflow_ptmin_cut;  	    //!
  int   m_mu_cutflow_type_cut;		    //!
  int   m_mu_cutflow_z0sintheta_cut;	    //!
  int   m_mu_cutflow_d0_cut;		    //!
  int   m_mu_cutflow_d0sig_cut;  	    //!
  int   m_mu_cutflow_iso_cut;		    //!
  int   m_mu_cutflow_cosmic_cut;		    //!

  std::vector<std::string> m_IsoKeys;       //!

  /* other private members */

  std::vector<std::string>            m_singleMuTrigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_singleMuTrigChains */
  std::vector<std::string>            m_diMuTrigChainsList;     //!  /* contains all the HLT trigger chains tokens extracted from m_diMuTrigChains */

  // tools
  asg::AnaToolHandle<CP::IIsolationSelectionTool>  m_isolationSelectionTool_handle{"CP::IsolationSelectionTool/IsolationSelectionTool", this}; //!
  // this only exists because the interface needs to be updated, complain on pathelp, remove forward declaration for this when fixed
  CP::IsolationSelectionTool*                      m_isolationSelectionTool{nullptr}; //!
  asg::AnaToolHandle<CP::IMuonSelectionTool>       m_muonSelectionTool_handle     {"CP::MuonSelectionTool/MuonSelectionTool"          , this}; //!
  asg::AnaToolHandle<Trig::IMatchingTool>          m_trigMuonMatchTool_handle     {"Trig::MatchingTool/MatchingTool"                  , this}; //!
  asg::AnaToolHandle<Trig::TrigDecisionTool>       m_trigDecTool_handle           {"Trig::TrigDecisionTool/TrigDecisionTool"                       }; //!

  /// @brief This internal variable gets set to false if no triggers are defined or if TrigDecisionTool is missing
  bool m_doTrigMatch = true; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  MuonSelector ();

  ~MuonSelector();

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

  // added functions not from Algorithm
  bool executeSelection( const xAOD::MuonContainer* inMuons, float mcEvtWeight, bool countPass,
                         ConstDataVector<xAOD::MuonContainer>* selectedMuons );
  virtual int passCuts( const xAOD::Muon* muon, const xAOD::Vertex *primaryVertex );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonSelector, 1);
  /// @endcond

};

#endif
