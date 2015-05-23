#ifndef xAODAnaHelpers_TrackSelector_H
#define xAODAnaHelpers_TrackSelector_H

// ROOT include(s):
#include "TH1D.h"

#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/TrackParticleContainer.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class TrackSelector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_numObjectPass;    //!

  std::string              m_passAuxDecorKeys;
  std::string              m_failAuxDecorKeys;
  std::vector<std::string> m_passKeys;
  std::vector<std::string> m_failKeys;

  // cutflow
  bool m_useCutFlow;            //!
  TH1D* m_cutflowHist;          //!
  TH1D* m_cutflowHistW;         //!
  int   m_cutflow_bin;          //!

private:

  // configuration variables
  std::string m_inContainerName;      // input container name
  std::string m_outContainerName;     // output container name
  bool m_decorateSelectedObjects; // decorate selected objects? defaul passSel
  bool m_createSelectedContainer; // fill using SG::VIEW_ELEMENTS to be light weight
  int  m_nToProcess;               // look at n objects
  int m_pass_min;                 // minimum number of objects passing cuts
  int m_pass_max;                 // maximum number of objects passing cuts
  float m_pT_max;                 // require pT < pt_max
  float m_pT_min;                 // require pT > pt_max
  float m_eta_max;                // require eta < eta_max
  float m_eta_min;                // require eta > eta_max
  float m_d0_max;                 // require |d0| < d0_max
  float m_z0_max;                 // require |z0| < z0_max
  float m_z0sinT_max;             // require |z0xsin(theat)| < z0sinT_max
  int   m_nBL_min;                // require nBL > nBL_min
  int   m_nSi_min;                // require nSi > nSi_min
  float m_nPixHoles_max;          // require nPixHoles < nPixHoles_max
  float m_chi2NdofCut_max;        // require chi2/ndof < chi2NdofCut_max
  float m_chi2Prob_max;           // require TMath::Prob(chi2,ndof) < chi2ProbMax


  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  TrackSelector ();

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

  // added functions not from Algorithm
  // why does this need to be virtual?
  virtual int PassCuts( const xAOD::TrackParticle* jet, const xAOD::Vertex *pvx );

  // this is needed to distribute the algorithm to the workers
  ClassDef(TrackSelector, 1);
};

#endif
