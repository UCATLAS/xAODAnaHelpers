#ifndef xAODAnaHelpers_TreeAlgo_H
#define xAODAnaHelpers_TreeAlgo_H

#include <EventLoop/Algorithm.h>
// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "TTree.h"
class HelpTreeBase;

class TreeAlgo : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  std::string m_name;                  
  std::string m_configName;            

  xAOD::TEvent *m_event;               //!
  xAOD::TStore *m_store;               //!


private:

  HelpTreeBase* m_helpTree;            //!

  // configuration variables
  std::string m_inContainerName;       //!

  // holds bools that control which branches are filled
  std::string m_evtDetailStr;          //!
  std::string m_muDetailStr;           //!
  std::string m_elDetailStr;           //!
  std::string m_jetDetailStr;          //!
  std::string m_fatJetDetailStr;       //!

  std::string m_evtContainerName;      //!
  std::string m_muContainerName;       //!
  std::string m_elContainerName;       //!
  std::string m_jetContainerName;      //!
  std::string m_fatJetContainerName;   //!

public:
  // this is a standard constructor
  TreeAlgo ();                                           //!
  TreeAlgo (std::string name, std::string configName);   //!

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);           //!
  virtual EL::StatusCode fileExecute ();                    //!
  virtual EL::StatusCode treeInitialize ();                 //!
  virtual EL::StatusCode changeInput (bool firstFile);      //!
  virtual EL::StatusCode initialize ();                     //!
  virtual EL::StatusCode execute ();                        //!
  virtual EL::StatusCode postExecute ();                    //!
  virtual EL::StatusCode finalize ();                       //!
  virtual EL::StatusCode treeFinalize ();                   //!

  // these are the functions not inherited from Algorithm
  virtual EL::StatusCode configure ();                      //!

  // this is needed to distribute the algorithm to the workers
  ClassDef(TreeAlgo, 1);                                 //!
};

#endif
