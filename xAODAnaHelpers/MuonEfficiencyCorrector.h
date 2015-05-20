#ifndef xAODAnaHelpers_MuonEfficiencyCorrector_H
#define xAODAnaHelpers_MuonEfficiencyCorrector_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// external tools include(s):
#include "MuonEfficiencyCorrections/MuonEfficiencyScaleFactors.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class MuonEfficiencyCorrector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.

public:
  // configuration variables
  std::string   m_inContainerName;
  std::string   m_outContainerName;

  std::string   m_WorkingPoint;
  std::string   m_DataPeriod;

  // systematics
  bool          m_runAllSyst;

private:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!
  int m_numEvent;         //!
  int m_numObject;        //!

  bool m_runSysts;
  std::vector<CP::SystematicSet> m_systList; //!
  std::string m_outAuxContainerName;

  // tools
  CP::MuonEfficiencyScaleFactors  *m_MuonEffSFTool; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:

  // Tree *myTree; //!
  // TH1 *myHist;  //!


  // this is a standard constructor
  MuonEfficiencyCorrector ();

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
  virtual EL::StatusCode configure ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonEfficiencyCorrector, 1);
};

#endif
