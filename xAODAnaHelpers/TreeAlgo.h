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
  bool m_outHistDir = false;

  // holds bools that control which branches are filled
  std::string m_evtDetailStr = "";
  std::string m_trigDetailStr = "";
  std::string m_muDetailStr = "";
  std::string m_elDetailStr = "";
  std::string m_jetDetailStr = "";
  std::string m_trigJetDetailStr = "";
  std::string m_truthJetDetailStr = "";
  std::string m_fatJetDetailStr = "";
  std::string m_truthFatJetDetailStr = "";
  std::string m_tauDetailStr = "";
  std::string m_METDetailStr = "";
  std::string m_METReferenceDetailStr = "";
  std::string m_photonDetailStr = "";
  std::string m_clusterDetailStr = "";
  std::string m_truthParticlesDetailStr = "";
  std::string m_trackParticlesDetailStr = "";

  std::string m_evtContainerName = "";
  std::string m_muContainerName = "";
  std::string m_elContainerName = "";
  std::string m_jetContainerName = "";
  std::string m_jetBranchName = "jet";
  std::string m_truthJetContainerName = "";
  std::string m_truthJetBranchName = "truthJet";
  std::string m_trigJetContainerName = "";
  std::string m_trigJetBranchName = "trigJet";
  std::string m_fatJetContainerName = "";
  std::string m_truthFatJetContainerName = "";
  std::string m_tauContainerName = "";
  std::string m_METContainerName = "";
  std::string m_METReferenceContainerName = "";
  std::string m_photonContainerName = "";
  std::string m_clusterContainerName = "";
  std::string m_truthParticlesContainerName = "";
  std::string m_trackParticlesContainerName = "";
  std::string m_l1JetContainerName = "";
  bool m_sortL1Jets = false;
  bool m_retrievePV = true;

  // if these are set, assume systematics are being processed over
  std::string m_muSystsVec = "";
  std::string m_elSystsVec = "";
  std::string m_jetSystsVec = "";
  std::string m_photonSystsVec = "";
  std::string m_fatJetSystsVec = "";
  std::string m_metSystsVec = "";

  /// @brief unit conversion from MeV, default is GeV
  float m_units = 1e3;

protected:
  std::vector<std::string> m_jetDetails; //!
  std::vector<std::string> m_trigJetDetails; //!

  std::vector<std::string> m_jetContainers; //!
  std::vector<std::string> m_truthJetContainers; //!
  std::vector<std::string> m_trigJetContainers; //!

  std::vector<std::string> m_jetBranches; //!
  std::vector<std::string> m_truthJetBranches; //!
  std::vector<std::string> m_trigJetBranches; //!

  std::map<std::string, HelpTreeBase*> m_trees;            //!

public:

  // this is a standard constructor
  TreeAlgo ();

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

  // Help tree creator function
  virtual HelpTreeBase* createTree(xAOD::TEvent *event, TTree* tree, TFile* file, const float units, bool debug, xAOD::TStore* store); //!

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(TreeAlgo, 1);                                 //!
  /// @endcond
};

#endif
