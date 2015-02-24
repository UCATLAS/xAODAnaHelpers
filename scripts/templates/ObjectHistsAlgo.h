#ifndef ${package}_${name}HistsAlgo_H
#define ${package}_${name}HistsAlgo_H

#include <EventLoop/Algorithm.h>
#include <EventLoop/StatusCode.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

class ${name}Hists;

class ${name}HistsAlgo : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  std::string m_name;
  std::string m_configName;

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!


private:
  ${name}Hists* m_plots; //!

  // configuration variables
  std::string m_inContainerName;  //!
  std::string m_detailStr;        //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  ${name}HistsAlgo ();
  ${name}HistsAlgo (std::string name, std::string configName);

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
  ClassDef(${name}HistsAlgo, 1);
};

#endif
