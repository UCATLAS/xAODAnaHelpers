#ifndef xAODAnaHelpers_MuonEfficiencyCorrector_H
#define xAODAnaHelpers_MuonEfficiencyCorrector_H

// EL include(s):
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// external tools include(s):
#include "MuonEfficiencyCorrections/MuonEfficiencyScaleFactors.h"

class MuonEfficiencyCorrector : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!
  int m_numEvent;         //!
  int m_numObject;        //!

  std::string m_name;
  std::string m_configName;

  std::string m_corrFileName1;
  //std::string m_corrFileName1;

  // systematics
  bool m_runAllSyst;
  std::string m_systName;
  float m_systSigma;
  bool m_runSysts;
  std::vector<CP::SystematicSet> m_systList; //!

  bool m_debug;

private:

  // tools
  CP::MuonEfficiencyScaleFactors  *m_MuonEffSFTool; //!

  // configuration variables
  std::string m_inContainerName;
  std::string m_outContainerName;
  std::string m_outAuxContainerName;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  MuonEfficiencyCorrector ();
  MuonEfficiencyCorrector (std::string name, std::string configName);

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
