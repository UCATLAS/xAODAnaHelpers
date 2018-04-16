/**
 * @file   JetTriggerEfficiencies.h
 *
 * @author Will Kalderon <william.kalderon@cern.ch>
 *
 * @brief  Make jet trigger efficiency histograms using the trigger decision tool and emulation
 *
 */

#ifndef xAODAnaHelpers_JetTriggerEfficiencies_H
#define xAODAnaHelpers_JetTriggerEfficiencies_H

// EDM include(s):
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

// ROOT include(s):
#include "TH1D.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "JetJvtEfficiency/IJetJvtEfficiency.h"
#include "JetInterface/IJetModifier.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

class JetTriggerEfficiencies : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  /** @brief trigDecTool name for configurability if name is not default.  If empty, use the default name. If not empty, change the name. */
  std::string m_trigDecTool_name{""};

  
  // configuration variables
  /// @brief input container name
  std::string m_inContainerName = "";
  // also need the HLT containers
  // for L1, HLT_presel and HLT

private:

  /** @brief Trigger decision tool.

    If you need to use a TDT that was previously created before this algorithm with a different name, set the name in m_trigDecTool_name.
  */
  asg::AnaToolHandle<Trig::TrigDecisionTool>     m_trigDecTool_handle{"Trig::TrigDecisionTool"}; 

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
  int   m_jet_cutflow_eta_cut;       //!
  int   m_jet_cutflow_jvt_cut;       //!
  int   m_jet_cutflow_btag_cut;      //!

  std::vector<CP::SystematicSet> m_systListJVT; //!
  std::vector<CP::SystematicSet> m_systListfJVT; //!

  std::vector<std::string>            m_singleJetTrigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_singleJetTrigChains */
  std::vector<std::string>            m_diJetTrigChainsList;     //!  /* contains all the HLT trigger chains tokens extracted from m_diJetTrigChains */
  
  asg::AnaToolHandle<CP::IJetJvtEfficiency>  m_JVT_tool_handle{"CP::JetJvtEfficiency/JVT"}; //!
  asg::AnaToolHandle<CP::IJetJvtEfficiency>  m_fJVT_eff_tool_handle{"CP::JetJvtEfficiency/fJVT"}; //!
  asg::AnaToolHandle<IBTaggingSelectionTool> m_BJetSelectTool_handle{"BTaggingSelectionTool"};  //!

  asg::AnaToolHandle<Trig::IMatchingTool>    m_trigJetMatchTool_handle{"Trig::MatchingTool/MatchingTool", this}; //!
  /* asg::AnaToolHandle<Trig::TrigDecisionTool> m_trigDecTool_handle{"Trig::TrigDecisionTool/TrigDecisionTool"}; //! */ // Moved to top

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
  JetTriggerEfficiencies ();

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
  /* virtual bool executeSelection( const xAOD::JetContainer* inJets, float mcEvtWeight, bool count, std::string outContainerName, bool isNominal ); */

  // added functions not from Algorithm
  // why does this need to be virtual?
  /* virtual int PassCuts( const xAOD::Jet* jet ); */

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetTriggerEfficiencies, 1);
  /// @endcond

};

#endif
