#ifndef xAODAnaHelpers_MuonCalibrator_H
#define xAODAnaHelpers_MuonCalibrator_H

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// external tools include(s):
#include <AsgTools/AnaToolHandle.h>
#include <AsgAnalysisInterfaces/IPileupReweightingTool.h>
#include <MuonAnalysisInterfaces/IMuonCalibrationAndSmearingTool.h>

class MuonCalibrator : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  // configuration variables
  std::string m_inContainerName = "";
  std::string m_outContainerName = "";

  /// @brief Recommendations release (not recommended to change)
  std::string m_overrideRelease = "";
  /// @brief Recommendations saggita release for 2015 & 2016 (not recommended to change)
  std::string m_overrideSagittaRelease1516 = "";
  /// @brief Recommendations saggita release for 2017 (not recommended to change)
  std::string m_overrideSagittaRelease17 = "";
  /// @brief Recommendations saggita release for 2018 (not recommended to change)
  std::string m_overrideSagittaRelease18 = "";
  /// @brief Set StatComb1516 property if different than -1
  int m_statComb1516 = -1;
  /// @brief Set StatComb17 property if different than -1
  int m_statComb17 = -1;
  /// @brief Set StatComb18 property if different than -1
  int m_statComb18 = -1;
  /// @brief Set SagittaCorr1516 property if different than -1
  int m_sagittaCorr1516 = -1;
  /// @brief Set SagittaCorr17 property if different than -1
  int m_sagittaCorr17 = -1;
  /// @brief Set SagittaCorr18 property if different than -1
  int m_sagittaCorr18 = -1;
  /// @brief Set doSagittaMCDistortion1516 property if different than -1
  int m_doSagittaMCDistortion1516 = -1;
  /// @brief Set doSagittaMCDistortion17 property if different than -1
  int m_doSagittaMCDistortion17 = -1;
  /// @brief Set doSagittaMCDistortion18 property if different than -1
  int m_doSagittaMCDistortion18 = -1;
  /// @brief Set SagittaCorrPhaseSpace1516 property if different than -1
  int m_sagittaCorrPhaseSpace1516 = -1;
  /// @brief Set SagittaCorrPhaseSpace17 property if different than -1
  int m_sagittaCorrPhaseSpace17 = -1;
  /// @brief Set SagittaCorrPhaseSpace18 property if different than -1
  int m_sagittaCorrPhaseSpace18 = -1;

  // sort after calibration
  bool m_sort = true;

  // systematics
  /// @brief this is the name of the vector of names of the systematically varied containers produced by the upstream algo (e.g., the SC containers with calibration systematics)
  std::string m_inputAlgoSystNames = "";
  // this is the name of the vector of names of the systematically varied containers produced by THIS algo (these will be the m_inputAlgoSystNames of the algo downstream)
  std::string m_outputAlgoSystNames = "MuonCalibrator_Syst";
  /// @brief Write systematics names to metadata
  bool        m_writeSystToMetadata = false;

  float       m_systVal = 0.0;
  std::string m_systName = "";

  /**
      @rst
          Force ``MuonCalibrationPeriodTool.h`` to calibrate data.

          ``MuonSelectorTool`` depends on a specific decoration existing on Muons, namely ``MuonSpectrometerPt``. This is decorated by the ``MuonCalibrationAndSmearingTool``. However, you do not calibrate data by default so this tool would not be run on data.

          In the case where you need the tool to be forced to run on data in order to have this decoration on your muons, you need to flip this boolean. See `the Muon Combined Performance Working Group twiki <https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/McpAnalysisFaq#SG_ExcBadAuxVar_from_MuonSelecti>`_ for more information.

          .. note:: This should *not* modify the momentum of muons in data (according to the tool as of ``MuonMomentumCorrections-01-00-37``).
      @endrst
   */
  bool    m_forceDataCalib = false;

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  std::string m_outAuxContainerName;
  std::string m_outSCContainerName;
  std::string m_outSCAuxContainerName;

  std::vector<CP::SystematicSet> m_systList; //!

  // tools
  asg::AnaToolHandle<CP::IMuonCalibrationAndSmearingTool> m_muonCalibrationTool_handle{"CP::MuonCalibrationPeriodTool/MuonCalibrationAndSmearingTool", this}; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  MuonCalibrator ();

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
  ClassDef(MuonCalibrator, 1);
  /// @endcond

};

#endif
