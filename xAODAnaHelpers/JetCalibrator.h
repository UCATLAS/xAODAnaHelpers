/********************************************************
 * JetCalibrator:
 *
 * This class handles jet calibration and systematics
 *
 * Jeff Dandoy (jeff.dandoy@cern.ch)
 *
 ********************************************************/

#ifndef xAODAnaHelpers_JetCalibrator_H
#define xAODAnaHelpers_JetCalibrator_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicVariation.h"

// external tools include(s):
#include "JetCalibTools/JetCalibrationTool.h"
#include "JetUncertainties/JetUncertaintiesTool.h"
#include "JetResolution/JERTool.h"
#include "JetResolution/JERSmearingTool.h"
#include "JetSelectorTools/JetCleaningTool.h"
#include "JetMomentTools/JetVertexTaggerTool.h"
#include "AsgTools/AnaToolHandle.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class JetCalibrator : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // configuration variables
  std::string m_inContainerName;
  std::string m_outContainerName;

  std::string m_jetAlgo;
  std::string m_outputAlgo;
  std::string m_calibConfigData;
  std::string m_calibConfigFullSim;
  std::string m_calibConfigAFII;
  std::string m_calibConfig;
  std::string m_calibSequence;
  std::string m_JESUncertConfig;
  std::string m_JESUncertMCType;
  bool m_setAFII;

  bool m_isTrigger; // whether the jet collection is trigger or not (soon: different calibrations)

  std::string m_JERUncertConfig;
  bool m_JERFullSys;
  bool m_JERApplyNominal;

  /// enable to apply jet cleaning
  bool m_doCleaning;
  std::string m_jetCleanCutLevel;
  bool m_saveAllCleanDecisions;
  bool m_jetCleanUgly;
  bool m_redoJVT;
  // sort after calibration
  bool    m_sort;
  //Apply jet cleaning to parent jet
  bool    m_cleanParent;
  bool    m_applyFatJetPreSel;

  // systematics
  bool m_runSysts;

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  bool m_isMC;            //!
  bool m_isFullSim;       //!

  std::vector<CP::SystematicSet> m_systList; //!
  std::vector<int> m_systType; //!

  // tools
  asg::AnaToolHandle<IJetCalibrationTool> m_JetCalibrationTool_handle; //!
  asg::AnaToolHandle<ICPJetUncertaintiesTool> m_JetUncertaintiesTool_handle; //!

  asg::AnaToolHandle<IJERTool> m_JERTool_handle;    //!
  asg::AnaToolHandle<IJERSmearingTool> m_JERSmearingTool_handle;    //!

  asg::AnaToolHandle<IJetUpdateJvt> m_JVTUpdateTool_handle; //!

  asg::AnaToolHandle<IJetSelector> m_JetCleaningTool_handle; //!
  std::vector<std::string>  m_decisionNames;    //!
  std::vector< asg::AnaToolHandle<IJetSelector> > m_AllJetCleaningTool_handles;   //!


  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  JetCalibrator (std::string className = "JetCalibrator");

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
  ClassDef(JetCalibrator, 1);
  /// @endcond

};

#endif
