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
  std::string m_trigJetDetailStr;
  std::string m_muDetailStr;
  std::string m_elDetailStr;
  std::string m_jetDetailStr;
  std::string m_truthJetDetailStr;
  std::string m_fatJetDetailStr;
  std::string m_tauDetailStr;
  std::string m_METDetailStr;
  std::string m_photonDetailStr;

  std::string m_evtContainerName;
  std::string m_muContainerName;
  std::string m_elContainerName;
  std::string m_jetContainerName;
  std::string m_truthJetContainerName;
  std::string m_trigJetContainerName;
  std::string m_fatJetContainerName;
  std::string m_tauContainerName;
  std::string m_METContainerName;
  std::string m_photonContainerName;

  // if these are set, assume systematics are being processed over
  std::string m_muSystsVec;
  std::string m_elSystsVec;
  std::string m_jetSystsVec;
  std::string m_photonSystsVec;

  bool m_DC14;
  float m_units;

protected:
  std::map<std::string, HelpTreeBase*> m_trees;            //!

public:

  // this is a standard constructor
  TreeAlgo (std::string className = "TreeAlgo");                                           //!

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);           //!
  virtual EL::StatusCode fileExecute ();                    //!
  virtual EL::StatusCode histInitialize ();                 //!
  virtual EL::StatusCode changeInput (bool firstFile);      //!
  virtual EL::StatusCode initialize ();                     //!
  virtual EL::StatusCode execute ();                        //!
  virtual EL::StatusCode postExecute ();                    //!
  virtual EL::StatusCode finalize ();                       //!
  virtual EL::StatusCode histFinalize ();                   //!

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(TreeAlgo, 1);                                 //!
  /// @endcond
};

#endif
