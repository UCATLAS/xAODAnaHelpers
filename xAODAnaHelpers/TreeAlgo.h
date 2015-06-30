#ifndef xAODAnaHelpers_TreeAlgo_H
#define xAODAnaHelpers_TreeAlgo_H

#include "TTree.h"

#include <xAODAnaHelpers/HelpTreeBase.h>

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class TreeAlgo : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // choose whether the tree gets saved in the same directory as output histograms
  bool m_outHistDir;

  // holds bools that control which branches are filled
  std::string m_evtDetailStr;
  std::string m_trigDetailStr;
  std::string m_jetTrigDetailStr;
  std::string m_muDetailStr;
  std::string m_elDetailStr;
  std::string m_jetDetailStr;
  std::string m_fatJetDetailStr;
  std::string m_tauDetailStr;
  std::string m_METDetailStr;
  
  std::string m_evtContainerName;
  std::string m_muContainerName;
  std::string m_elContainerName;
  std::string m_jetContainerName;
  std::string m_fatJetContainerName;
  std::string m_tauContainerName;
  std::string m_METContainerName;  

  bool m_DC14;

private:
  HelpTreeBase* m_helpTree;            //!

public:

  // this is a standard constructor
  TreeAlgo ();                                           //!

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
