#ifndef xAODAnaHelpers_JetCalibrator_H
#define xAODAnaHelpers_JetCalibrator_H

// EL include(s)
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

class JetCalibrationTool;
class JetCleaningTool;

class JetCalibrator : public EL::Algorithm
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
  bool m_isMC;            //!
  bool m_isFullSim;       //!

  bool m_debug;           //!

private:

  // tools
#ifndef __CINT__    
  JetCalibrationTool * m_jetCalibration; //!
  JetCleaningTool    * m_jetCleaning;    //!
#endif // not __CINT__

  // configuration variables
  TString m_inContainerName;        //!
  TString m_outContainerName;       //!
  TString m_outSCContainerName;     //!
  TString m_outSCAuxContainerName;  //!
  
  TString m_jetAlgo;                //!
  TString m_calibConfigData;        //!
  TString m_calibConfigFullSim;     //!
  TString m_calibConfigAFII;        //! 
  TString m_calibConfig;            //!
  TString m_calibSequence;          //!
  // sort after calibration
  bool    m_sort;                   //!

  std::string m_jetCalibCutLevel;   //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  JetCalibrator ();
  JetCalibrator (std::string name, std::string configName);

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
  ClassDef(JetCalibrator, 1);
};

#endif
