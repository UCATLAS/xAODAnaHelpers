/**
 * @file   TauEfficiencyCorrector.h
 * @author Federico Scutti <federico.scutti@cern.ch>
 * @brief  Interface to the tools of the TauEfficiencyCorrections package.
 *
 */

#ifndef xAODAnaHelpers_TauEfficiencyCorrector_H
#define xAODAnaHelpers_TauEfficiencyCorrector_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "PileupReweighting/PileupReweightingTool.h"
#include "TauAnalysisTools/TauEfficiencyCorrectionsTool.h"
#include "TauAnalysisTools/ITauEfficiencyCorrectionsTool.h"

class TauEfficiencyCorrector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.

public:
 
  std::string m_RecommendationTag = "";

  // configuration variables
  std::string   m_inContainerName = "";

  // Reco efficiency SF
  std::string   m_WorkingPointReco = "";
  
  // EleOLR efficiency SF if the the reco tau is a true tau
  std::string   m_WorkingPointEleOLRHadTau = "";

  // EleOLR efficiency SF if the the reco tau is a true electron
  std::string   m_WorkingPointEleOLRElectron = "";
  
  // TauID efficiency SF
  std::string   m_WorkingPointTauID = "";

  // working point for tauID used in 
  // combination with the trigger
  std::string   m_TriggerName = "";

  // systematics
  /**
    @brief The name of the vector containing the names of the systematically-varied taus-related containers from the upstream algorithm, which will be processed by this algorithm.

    Only tau systematics or any other that create shallow copies of tau containers should be passed to this tool. It is advised to run this algorithm before running algorithms combining multiple calibration systematics (e.g. overlap removal).
  */
  std::string   m_inputSystNamesTaus = "";

  /// @brief Write systematics names to metadata
  bool          m_writeSystToMetadata = false;

  float         m_systVal = 0.0;
  
  std::string   m_systName = "";
  
  std::string   m_outputSystNames = "TauEfficiencyCorrector_Syst";

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  std::vector<CP::SystematicSet> m_systList;  //!

  // tools
  asg::AnaToolHandle<CP::IPileupReweightingTool> m_pileup_tool_handle{"CP::PileupReweightingTool/Pileup"}; //!
  asg::AnaToolHandle<TauAnalysisTools::ITauEfficiencyCorrectionsTool> m_tauEffCorrTool_handle{"TauAnalysisTools::TauEfficiencyCorrectionsTool/TauEfficiencyCorrectionsTool",           this}; //! 
  // the selection tool is needed for baseline configuration
  asg::AnaToolHandle<TauAnalysisTools::ITauSelectionTool> m_tauSelTool_handle{"TauAnalysisTools::TauSelectionTool/TauSelectionTool"}; //!
  
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:

  // Tree *myTree; //!
  // TH1 *myHist;  //!

  // this is a standard constructor
  TauEfficiencyCorrector ();

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
  virtual EL::StatusCode executeSF ( const xAOD::EventInfo* eventInfo, const xAOD::TauJetContainer* inputTaus, bool nominal, bool writeSystNames );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(TauEfficiencyCorrector, 1);
  /// @endcond

};

#endif
