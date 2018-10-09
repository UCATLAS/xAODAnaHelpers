#ifndef xAODAnaHelpers_TauSelector_H
#define xAODAnaHelpers_TauSelector_H

// EDM include(s):
#include "xAODTau/TauJet.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTau/TauTrack.h"

// ROOT include(s):
#include "TH1D.h"

// external tools include(s):
//#include "TauAnalysisTools/TauSelectionTool.h"
#include "AsgTools/AnaToolHandle.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"
#include "TauAnalysisTools/ITauSelectionTool.h"

//#include "TauAnalysisTools/TauOverlappingElectronLLHDecorator.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class TauSelector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.

public:

  // cutflow
  bool m_useCutFlow = true;

  // configuration variables
  /* input container name */
  std::string    m_inContainerName = "";
  /* output container name */
  std::string    m_outContainerName;
  /* output auxiliary container name */
  std::string    m_outAuxContainerName;
  std::string    m_inputAlgoSystNames = "";
  std::string    m_outputAlgoSystNames = "TauSelector_Syst";
  /* decorate selected taus with tracks */
  bool       	 m_decorateWithTracks = false;
  /* decorate selected objects - default "passSel" */
  bool       	 m_decorateSelectedObjects = true;
  /* fill using SG::VIEW_ELEMENTS to be light weight */
  bool       	 m_createSelectedContainer = false;
  /* look at n objects */
  int            m_nToProcess = -1;
  /* minimum number of objects passing cuts */
  int            m_pass_min = -1;
  /* maximum number of objects passing cuts */
  int            m_pass_max = -1;
  /* path to config file for the TauSelectionTool */

  // IMPORTANT: if no working point is specified the one in this configuration will be used
  std::string    m_ConfigPath = "xAODAnaHelpers/TauConf/00-01-19/Selection/recommended_selection_mc15.conf";
  /* a minimal pT threshold b/c some derivations may apply a thinning on tau tracks' features needed by the TauSelectionTool, which would cause a crash at runtime */
  float          m_minPtDAOD = 15e3;
  std::string    m_JetIDWP = "";
  std::string    m_EleBDTWP = "";
  bool           m_EleOLR = true;

  /* trigger matching */
  
  /*
   * A comma-separated string w/ all the HLT 
   * single muon trigger chains for which you want 
   * to perform the matching. If left empty (as it is by default), 
   * no trigger matching will be attempted at all 
   */
  
  std::string    m_singleTauTrigChains = "";
  
  /*
   * A comma-separated string w/ all the HLT 
   * dimuon trigger chains for which you want 
   * to perform the matching.  If left empty (as it is by default), 
   * no trigger matching will be attempted at all 
   */
  
  std::string    m_diTauTrigChains = "";

private:

  int m_numEvent;           //!
  int m_numObject;          //!
  int m_numEventPass;       //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;      //!

  // cutflow
  TH1D* m_cutflowHist;      //!
  TH1D* m_cutflowHistW;     //!
  int   m_cutflow_bin;      //!

  bool  m_isUsedBefore;     //!

  // object cutflow
  TH1D* m_tau_cutflowHist_1;                //!
  TH1D* m_tau_cutflowHist_2;                //!

  int   m_tau_cutflow_all;		    //!
  int   m_tau_cutflow_selected;             //!

  // tools
  std::vector<std::string>            m_singleTauTrigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_singleTauTrigChains */
  std::vector<std::string>            m_diTauTrigChainsList;     //!  /* contains all the HLT trigger chains tokens extracted from m_diTauTrigChains */
  asg::AnaToolHandle<TauAnalysisTools::ITauSelectionTool> m_tauSelTool_handle{"TauAnalysisTools::TauSelectionTool/TauSelectionTool",     this}; //!
  asg::AnaToolHandle<Trig::TrigDecisionTool>              m_trigDecTool_handle{"Trig::TrigDecisionTool/TrigDecisionTool"    }; //!
  asg::AnaToolHandle<Trig::IMatchingTool>                 m_trigTauMatchTool_handle{"Trig::MatchingTool/MatchingTool",  this}; //!  

  /// @brief This internal variable gets set to false if no triggers are defined or if TrigDecisionTool is missing
  bool m_doTrigMatch = true; //!
  
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  TauSelector ();

  ~TauSelector();

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
  bool executeSelection( const xAOD::TauJetContainer* inTaus, float mcEvtWeight, bool countPass,
                         ConstDataVector<xAOD::TauJetContainer>* selectedTaus );
  virtual int passCuts( const xAOD::TauJet* tau );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(TauSelector, 1);
  /// @endcond

};

#endif
