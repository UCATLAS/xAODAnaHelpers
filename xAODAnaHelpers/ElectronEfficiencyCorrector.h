#ifndef xAODAnaHelpers_ElectronEfficiencyCorrector_H
#define xAODAnaHelpers_ElectronEfficiencyCorrector_H

// EL include(s):
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"

// external tools include(s):
#include "ElectronEfficiencyCorrection/AsgElectronEfficiencyCorrectionTool.h"

class ElectronEfficiencyCorrector : public EL::Algorithm
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
  std::string m_systName;
  float m_systVal;
  bool m_runSysts;
  std::vector<CP::SystematicSet> m_systList; //!

  bool m_debug;

private:

  // tools
  AsgElectronEfficiencyCorrectionTool  *m_asgElectronEfficiencyCorrectionTool; //!

  // configuration variables
  std::string m_inContainerName;
  std::string m_outContainerName;
  std::string m_outAuxContainerName;
  std::string m_inputAlgo;               // input type - from xAOD or from xAODAnaHelpers Algo output
  std::string m_outputAlgo;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  ElectronEfficiencyCorrector ();
  ElectronEfficiencyCorrector (std::string name, std::string configName, std::string systName = "", float systVal = 0);

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
  virtual EL::StatusCode executeSF (  const xAOD::ElectronContainer* correctedElectrons  );

  // this is needed to distribute the algorithm to the workers
  ClassDef(ElectronEfficiencyCorrector, 1);
};

#endif
