#ifndef xAODAnaHelpers_JetHistsAlgo_H
#define xAODAnaHelpers_JetHistsAlgo_H

#include <EventLoop/Algorithm.h>
// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include <memory>

// package include(s):
#ifndef __CINT__
  #include "xAODAnaHelpers/HelperClasses.h"
#endif

class JetHists;

class JetHistsAlgo : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  std::string m_name;
  std::string m_configName;
#ifndef __CINT__
  HelperClasses::ContainerType m_type;
#endif

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!


private:

#ifndef __CINT__
  std::shared_ptr<JetHists> m_plots; //!
#endif //not __CINT__

  // configuration variables
  std::string m_inContainerName;      //!
  std::string m_detailStr;        //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  JetHistsAlgo ();
  JetHistsAlgo (std::string name, std::string configName);

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
  ClassDef(JetHistsAlgo, 1);
};

#endif
