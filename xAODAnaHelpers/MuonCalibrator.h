#ifndef xAODAnaHelpers_MuonCalibrator_H
#define xAODAnaHelpers_MuonCalibrator_H

// EL include(s)
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#ifndef __CINT__
namespace CP{
   class MuonCalibrationAndSmearingTool; 
}
#endif

class MuonCalibrator : public EL::Algorithm
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
  bool m_debug;

private:

  // tools
#ifndef __CINT__  
  CP::MuonCalibrationAndSmearingTool *m_muonCalibrationAndSmearingTool; //! 
#endif // not __CINT__

  // configuration variables
  TString m_inContainerName;
  TString m_outContainerName;
  TString m_outAuxContainerName;
  TString m_outSCContainerName;
  TString m_outSCAuxContainerName; 

  // sort after calibration
  bool    m_sort;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  MuonCalibrator ();
  MuonCalibrator (std::string name, std::string configName);

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
  ClassDef(MuonCalibrator, 1);
};

#endif
