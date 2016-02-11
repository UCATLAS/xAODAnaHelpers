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
  bool m_useCutFlow;

  // configuration variables
  std::string m_inContainerName;   // input container name
  std::string m_outContainerName;  // output container name
  std::string m_decor;            // The decoration key written to passing objects
  bool m_decorateSelectedObjects; // decorate selected objects? defaul passSel
  bool m_createSelectedContainer; // fill using SG::VIEW_ELEMENTS to be light weight
  int m_nToProcess;               // look at n objects
  int m_pass_min;                 // minimum number of objects passing cuts
  int m_pass_max;                 // maximum number of objects passing cuts
  float m_pT_max;                 // require pT < pt_max
  float m_pT_min;                 // require pT > pt_max
  float m_eta_max;                // require eta < eta_max
  float m_eta_min;                // require eta > eta_max
  float m_mass_max;               // require mass < mass_max
  float m_mass_min;               // require mass > mass_max
  float m_rapidity_max;           // require rapidity < rapidity_max
  float m_rapidity_min;           // require rapidity > rapidity_min

private:
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;    //!

  // cutflow
  TH1D* m_cutflowHist;          //!
  TH1D* m_cutflowHistW;         //!
  int   m_cutflow_bin;          //!

  /* object-level cutflow */

  TH1D* m_truth_cutflowHist_1;  //!

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
  TruthSelector (std::string className = "TruthSelector");

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
