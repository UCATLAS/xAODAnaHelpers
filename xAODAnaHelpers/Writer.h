#ifndef xAODAnaHelpers_Writer_H
#define xAODAnaHelpers_Writer_H

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include <EventLoop/Algorithm.h>


class Writer : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;
  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!
  int m_numEvent;         //!

  std::string m_name;
  std::string m_configName;
  bool m_isMC;

private:

  TString m_outputLabel;

  TString m_jetContainerNamesStr;
  std::vector<TString> m_jetContainerNames;

  TString m_electronContainerNamesStr;
  std::vector<TString> m_electronContainerNames;

  TString m_muonContainerNamesStr;
  std::vector<TString> m_muonContainerNames;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  Writer ();
  Writer (std::string name, std::string configName);

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

  // this is needed to distribute the algorithm to the workers
  ClassDef(Writer, 1);
};

#endif
