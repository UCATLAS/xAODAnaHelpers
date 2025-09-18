/**
 * @file   JetSelector.h
 *
 * @author Gabriel Facini <gabriel.facini@cern.ch>
 * @author Jeff Dandoy  <jeff.dandoy@cern.ch>
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @author John Alison <john.alison@cern.ch>
 *
 * @brief  Select jets and apply JVT corrections.
 *
 */

#ifndef xAODAnaHelpers_JetSelector_H
#define xAODAnaHelpers_JetSelector_H

// EDM include(s):
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

// ROOT include(s):
#include "TH1D.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "ParticleJetTools/JetPileupLabelingTool.h"
#include "JetMomentTools/JetVertexNNTagger.h"
#include "JetAnalysisInterfaces/IJvtEfficiencyTool.h"
#include "PATCore/IAsgSelectionTool.h"
#include "JetInterface/IJetModifier.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"
#include "TriggerMatchingTool/IMatchScoringTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

class JetSelector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  bool m_useCutFlow = true;

  // configuration variables
  /// @brief input container name
  std::string m_inContainerName = "";
  /// @brief output container name
  std::string m_outContainerName = "";
  /// @brief truth jet container name (used for JVT SF)
  std::string m_truthJetContainer = "AntiKt4TruthJets";
  /// @brief input type - from xAOD or from xAODAnaHelper Algo output
  std::string m_inputAlgo = "";
  /// @brief output type - this is how the vector<string> w/ syst names will be saved in TStore
  std::string m_outputAlgo = "";
  /// @brief Write systematics names to metadata
  bool        m_writeSystToMetadata = false;
  /// @brief Type of Scale Momementum
  std::string m_jetScaleType = "";
  /// @brief The decoration key written to passing objects
  std::string m_decor = "passSel";
  /// @brief decorate selected objects? defaul passSel
  bool m_decorateSelectedObjects = true;
  /// @brief fill using SG::VIEW_ELEMENTS to be light weight
  bool m_createSelectedContainer = false;
  /// @brief look at n objects
  int m_nToProcess = -1;
  /// @brief require cleanJet decoration to not be set and false
  bool m_cleanJets = true;
  /// @brief kill event if any of the N leading jets are not clean
  int m_cleanEvtLeadJets = -1;
  /** @brief Kill event if any passing jets are not clean */
  /**
      @rst
          .. note:: The jets need the `cleanJet` decoration which is set when you enable :cpp:member:`JetCalibrator::m_doCleaning`

      @endrst

   */
  bool m_cleanEvent = false;
  /** @brief Mark event with decorator if any passing jets are not clean */
  bool m_markCleanEvent = false;
  /** @brief Choose the scale at which the selection is performed (default "Final", i.e. default 4vector) */
  std::string m_jetScale4Selection = "Final";
  /// @brief (MC-only) Kill pileup overlay event if reconstructed jets avg(pT1,pT2) > 1.4*(truth jet pT1)
  bool m_doMCCleaning = false;
  /// @brief Change the default 1.4 cut to x > 1.0
  float m_mcCleaningCut = 1.4;
  /// @brief minimum number of objects passing cuts
  int m_pass_min = -1;
  /// @brief maximum number of objects passing cuts
  int m_pass_max = -1;
  /// @brief require pT < pt_max
  float m_pT_max = 1e8;
  /// @brief require pT > pt_min
  float m_pT_min = 1e8;
  /// @brief require ET < ET_max
  float m_ET_max = 1e8;
  /// @brief require ET > ET_min
  float m_ET_min = 1e8;
  /// @brief require eta < eta_max
  float m_eta_max = 1e8;
  /// @brief require eta > eta_min
  float m_eta_min = 1e8;
  /// @brief require detEta < detEta_max
  float m_detEta_max = 1e8;
  /// @brief require detEta > detEta_min
  float m_detEta_min = 1e8;
  /// @brief require mass < mass_max
  float m_mass_max = 1e8;
  /// @brief require mass > mass_min
  float m_mass_min = 1e8;
  /// @brief require rapidity < rapidity_max
  float m_rapidity_max = 1e8;
  /// @brief require rapidity > rapidity_min
  float m_rapidity_min = 1e8;
  /// @brief require truth level on truth jets
  int   m_truthLabel =  -1;
  /// @brief use HadronConeExclTruthLabelID for truth match (default)
  bool  m_useHadronConeExcl = true;

  /// @brief check JVF
  bool m_doJVF = false;
  /// @brief max pT [GeV] (JVF is a pileup cut)
  float m_pt_max_JVF = 50e3;
  /// @brief detector eta cut
  float m_eta_max_JVF = 2.4;
  /// @brief cut value
  float m_JVFCut = 0.5;
  /// @brief (re-)evaluate the hard-scatter jet label (isJvtHS) required for Jvt efficiency tools
  bool m_doTruthJetTagging = true;
  /// @brief check JVT
  bool m_doJVT = false;
  /// @brief keep JVT-rejected jets and decorate passing status
  bool m_noJVTVeto = false;
  /// @brief check forward JVT
  bool m_dofJVT = false;
  /// @brief Remove jets that fail fJVT. Like JVT, the default is to clean the collection
  bool m_dofJVTVeto = true;
  /// @brief max pT [GeV] (JVT is a pileup cut)
  float m_pt_max_JVT = 60e3;
  /// @brief detector eta cut
  float m_eta_max_JVT = 2.4;
  /// @brief was JVT already run in an earlier instance of JetSelector?
  bool m_jvtUsedBefore=false;
  /// @brief Does the input have truth jets? If not, cannot decorate with true hard scatter / pileup info
  bool m_haveTruthJets = true;
  /// @brief Retrieve JVT SFs (true by default, when false: allows to get JVT decision w/o needing truth jets)
  bool m_getJVTSF = true;

  /**
    @brief Minimum value of JVT for selecting jets.

    @rst
        .. warning:: If set to a non-negative value (default is -1.0), it will override any set value for :cpp:member:`JetSelector::m_WorkingPointJVT`
    @endrst
  */
  float m_JVTCut = -1.0;

  /// @brief WP for NNJvt
  std::string m_WorkingPointJVT = "FixedEffPt";

 /// @brief SF file for NNJvtEfficiencyTool
  std::string m_SFFileJVT = ""; // empty string means to apply dummy SFs
  std::string m_outputSystNamesJVT = "JetJvtEfficiency_JVTSyst";
  /// @brief Do re-calculation of NNJvt - scores need to be re-evaluated in case jet pt changed w.r.t. derivation
  bool m_recalculateJvtScores = true;

  float         m_systValJVT = 0.0;
  std::string   m_systNameJVT = "";

  /// @brief WP for fJvt
  std::string m_WorkingPointfJVT = "Loose";

  /// @brief SF file for fJvtEfficiencyTool 
  std::string m_SFFilefJVT = ""; // empty string means to apply dummy SFs

  std::string m_outputSystNamesfJVT = "JetJvtEfficiency_fJVTSyst";

  float         m_systValfJVT = 0.0;
  std::string   m_systNamefJVT = "";
  /// @brief was fJVT already run in an earlier instance of JetSelector?
  bool m_fjvtUsedBefore=false;

  /// @brief Timing cut
  bool          m_doJetTimingCut = false;
  float         m_jetTiming_max = -1;

  /// @brief Flag to apply btagging cut, if false just decorate decisions
  bool  m_doBTagCut = false;
  std::string m_corrFileName = "xAODBTaggingEfficiency/cutprofiles_22072015.root";
  std::string m_jetAuthor = "AntiKt4EMPFlowJets";
  std::string m_taggerName = "DL1r";
  std::string m_operatingPt = "FixedCutBEff_70";
  bool        m_readFromBTaggingObject = false;
  // for BTaggingSelectionTool -- doubles are needed or will crash
  // for the b-tagging tool - these are the b-tagging groups minimums
  // users making tighter cuts can use the selector's parameters to keep
  // things consistent
  double m_b_eta_max = 2.5;
  double m_b_pt_min = 20e3;

  // HLT Btag quality
  bool m_doHLTBTagCut = false;
  std::string m_HLTBTagTaggerName = "DL1r";
  float m_HLTBTagCutValue = -0.4434;
  bool  m_requireHLTVtx = false;
  bool  m_requireNoHLTVtx = false;

  std::string              m_passAuxDecorKeys = "";
  std::string              m_failAuxDecorKeys = "";

  /* trigger matching */
  /** A comma-separated string w/ alll the HLT single jet trigger chains for which you want to perform the matching. If left empty (as it is by default), no trigger matching will be attempted at all */
  std::string    m_singleJetTrigChains = "";
  /** A comma-separated string w/ all the HLT dijet trigger chains for which you want to perform the matching.  If left empty (as it is by default), no trigger matching will be attempted at all */
  std::string    m_diJetTrigChains = "";

  /// @brief remove duplicate jets (exactly the same eta)
  bool m_removeDuplicates = false;
  /// @brief number of events with duplicates
  int m_count_events_with_duplicates = 0;
  /// @brief sort jets (normally done by JetCalibrator, but HLT jets need sorting and don't get calibrated here)
  bool m_sort = false;

private:
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;    //!
  int m_pvLocation;       //!

  bool m_isEMjet;                //!
  bool m_isLCjet;                //!

  // cutflow
  TH1D* m_cutflowHist = nullptr;          //!
  TH1D* m_cutflowHistW = nullptr;         //!
  int   m_cutflow_bin;          //!

  std::vector<std::string> m_passKeys;  //!
  std::vector<std::string> m_failKeys;  //!

  /* object-level cutflow */

  TH1D* m_jet_cutflowHist_1 = nullptr;  //!

  int   m_jet_cutflow_all;           //!
  int   m_jet_cutflow_cleaning_cut;  //!
  int   m_jet_cutflow_ptmax_cut;     //!
  int   m_jet_cutflow_ptmin_cut;     //!
  int   m_jet_cutflow_etmax_cut;     //!
  int   m_jet_cutflow_etmin_cut;     //!
  int   m_jet_cutflow_eta_cut;       //!
  int   m_jet_cutflow_jvt_cut;       //!
  int   m_jet_cutflow_timing_cut;    //!
  int   m_jet_cutflow_btag_cut;      //!

  std::vector<CP::SystematicSet> m_systListJVT; //!
  std::vector<CP::SystematicSet> m_systListfJVT; //!

  std::vector<std::string>            m_singleJetTrigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_singleJetTrigChains */
  std::vector<std::string>            m_diJetTrigChainsList;     //!  /* contains all the HLT trigger chains tokens extracted from m_diJetTrigChains */

  asg::AnaToolHandle<JetPileupLabelingTool>  m_jetPileupLabelingTool;  //!
  asg::AnaToolHandle<JetPileupTag::JetVertexNNTagger>  m_jetNNJvtMomentTool;  //!
  asg::AnaToolHandle<IAsgSelectionTool> m_jetNNJvtSelectionTool;  //!
  asg::AnaToolHandle<CP::IJvtEfficiencyTool> m_jetNNJvtEfficiencyTool;  //!
  asg::AnaToolHandle<IAsgSelectionTool> m_jetfJvtSelectionTool;  //!
  asg::AnaToolHandle<CP::IJvtEfficiencyTool> m_jetfJvtEfficiencyTool;  //!

  asg::AnaToolHandle<IBTaggingSelectionTool> m_BJetSelectTool_handle{"BTaggingSelectionTool"};  //!

  asg::AnaToolHandle<Trig::IMatchingTool>    m_trigJetMatchTool_handle; //!
  asg::AnaToolHandle<Trig::TrigDecisionTool> m_trigDecTool_handle{"Trig::TrigDecisionTool/TrigDecisionTool"}; //!
  asg::AnaToolHandle<Trig::IMatchScoringTool>  m_scoreTool{"Trig::DRScoringTool/DRScoringTool"}; //!

  /// @brief This internal variable gets set to false if no triggers are defined or if TrigDecisionTool is missing
  bool m_doTrigMatch = true; //!

  std::string m_outputJVTPassed = "JetJVT_Passed"; //!
  std::string m_outputfJVTPassed = "JetfJVT_Passed"; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  JetSelector ();

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

  // these are the functions not inherited from Algorithm
  virtual bool executeSelection( const xAOD::JetContainer* inJets, float mcEvtWeight, bool count, std::string outContainerName, bool isNominal );

  // added functions not from Algorithm
  // why does this need to be virtual?
  virtual int PassCuts( const xAOD::Jet* jet );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetSelector, 1);
  /// @endcond

};

#endif
