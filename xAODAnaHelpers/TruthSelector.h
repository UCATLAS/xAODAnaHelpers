#ifndef xAODAnaHelpers_TruthSelector_H
#define xAODAnaHelpers_TruthSelector_H

// EDM include(s):
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthParticleContainer.h"

// ROOT include(s):
#include "TH1D.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// external tools include(s):
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"

class TruthSelector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  bool m_useCutFlow = true;

  // configuration variables
  /// @brief input container name
  std::string m_inContainerName = "";
  /// @brief output container name
  std::string m_outContainerName = "";
  /// @brief The decoration key written to passing objects
  std::string m_decor = "passSel";
  /// @brief decorate selected objects? defaul passSel
  bool m_decorateSelectedObjects = true;
  /// @brief fill using SG::VIEW_ELEMENTS to be light weight
  bool m_createSelectedContainer = false;
  /// @brief look at n objects
  int m_nToProcess = -1;
  /// @brief minimum number of objects passing cuts
  int m_pass_min = -1;
  /// @brief maximum number of objects passing cuts
  int m_pass_max = -1;
  /// @brief require pT < pt_max
  float m_pT_max = 1e8;
  /// @brief require pT > pt_min
  float m_pT_min = 1e8;
  /// @brief require eta < eta_max
  float m_eta_max = 1e8;
  /// @brief require eta > eta_max
  float m_eta_min = 1e8;
  /// @brief require mass < mass_max
  float m_mass_max = 1e8;
  /// @brief require mass > mass_max
  float m_mass_min = 1e8;
  /// @brief require rapidity < rapidity_max
  float m_rapidity_max = 1e8;
  /// @brief require rapidity > rapidity_min
  float m_rapidity_min = 1e8;
  /// @brief require classifierParticleType == type (defined by TruthClassifier: https://gitlab.cern.ch/atlas/athena/blob/21.2/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h)
  unsigned int m_type = 1000; // this will apply no selection
  /// @brief require classifierParticleOrigin == origin (defined by TruthClassifier: https://gitlab.cern.ch/atlas/athena/blob/21.2/PhysicsAnalysis/MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h)
  unsigned int m_origin = 1000; // this will apply no selection
  /// @brief require classifierParticleOrigin to match any of the "|" separated origin values (e.g. "10|12|13")
  std::string m_originOptions; // this will apply no selection
  /// @brief require pt_dressed > pt_dressed_min
  float m_pT_dressed_min = 1e8;
  /// @brief require eta_dressed > eta_dressed_min
  float m_eta_dressed_min = 1e8;
  /// @brief require eta_dressed > eta_dressed_max
  float m_eta_dressed_max = 1e8;

private:
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;    //!

  // cutflow
  TH1D* m_cutflowHist = nullptr;          //!
  TH1D* m_cutflowHistW = nullptr;         //!
  int   m_cutflow_bin;          //!

  /* object-level cutflow */

  TH1D* m_truth_cutflowHist_1 = nullptr;  //!

  int   m_truth_cutflow_all;           //!
  int   m_truth_cutflow_ptmax_cut;     //!
  int   m_truth_cutflow_ptmin_cut;     //!
  int   m_truth_cutflow_eta_cut;       //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  TruthSelector ();

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
  virtual bool executeSelection( const xAOD::TruthParticleContainer* inTruthParts, float mcEvtWeight, bool count, std::string outContainerName );

  // added functions not from Algorithm
  // why does this need to be virtual?
  virtual int PassCuts( const xAOD::TruthParticle* truthPart );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(TruthSelector, 1);
  /// @endcond

};

#endif
