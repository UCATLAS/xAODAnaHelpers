#ifndef xAODAnaHelpers_ElectronCalibrator_H
#define xAODAnaHelpers_ElectronCalibrator_H

// EL include(s):
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// EDM include(s):
#ifndef __CINT__
  #include "xAODEgamma/ElectronContainer.h"
#endif

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"

namespace CP{
   class EgammaCalibrationAndSmearingTool;
}
class AsgElectronEfficiencyCorrectionTool;


class ElectronCalibrator : public EL::Algorithm
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
  bool m_doSyst;
  bool m_runSingleSyst;
  std::string m_systName;
  float m_systSigma;
  bool m_runSysts;
  std::vector<CP::SystematicSet> m_systList; //!

  bool m_debug;

private:

  // tools
#ifndef __CINT__
  CP::EgammaCalibrationAndSmearingTool *m_EgammaCalibrationAndSmearingTool; //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElectronEfficiencyCorrectionTool; //!
#endif // not __CINT__

  // configuration variables
  std::string m_inContainerName;
  std::string m_outContainerName;
  std::string m_outAuxContainerName;
  std::string m_outSCContainerName;
  std::string m_outSCAuxContainerName;

  // sort after calibration
  bool    m_sort;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  ElectronCalibrator ();
  ElectronCalibrator (std::string name, std::string configName);

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
#ifndef __CINT__
  virtual EL::StatusCode calibrate (xAOD::ElectronContainer* electrons);
#endif // not __CINT__

  // this is needed to distribute the algorithm to the workers
  ClassDef(ElectronCalibrator, 1);
};

#endif
