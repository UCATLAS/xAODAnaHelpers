#ifndef xAODAnaHelpers_TauCalibrator_H
#define xAODAnaHelpers_TauCalibrator_H

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "TauAnalysisTools/ITauSmearingTool.h"

class TauCalibrator : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  // configuration variables
  std::string m_inContainerName = "";
  std::string m_outContainerName = "";

  std::string m_RecommendationTag = "";
  
  bool m_applyMVATES = false;
  bool m_applyCombinedTES = false;
  bool m_setAFII = false;

  // sort after calibration
  bool    m_sort = true;

  // systematics
  /// @brief this is the name of the vector of names of the systematically varied containers produced by the upstream algo (e.g., the SC containers with calibration systematics)
  std::string m_inputAlgoSystNames = "";
  // this is the name of the vector of names of the systematically varied containers produced by THIS algo (these will be the m_inputAlgoSystNames of the algo downstream)
  std::string m_outputAlgoSystNames = "TauCalibrator_Syst";
  /// @brief Write systematics names to metadata
  bool        m_writeSystToMetadata = false;

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  std::string m_outAuxContainerName;
  std::string m_outSCContainerName;
  std::string m_outSCAuxContainerName;

  std::vector<CP::SystematicSet> m_systList; //!

  // tools
  asg::AnaToolHandle<TauAnalysisTools::ITauSmearingTool> m_tauSmearingTool_handle{"TauAnalysisTools::TauSmearingTool/TauSmearingTool",     this}; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  TauCalibrator ();

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
  ClassDef(TauCalibrator, 1);
  /// @endcond

};

#endif





