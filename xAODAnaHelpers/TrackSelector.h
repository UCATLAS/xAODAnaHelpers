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

  bool m_useCutFlow = true;

  // configuration variables
  /// @brief input container name
  std::string m_inContainerName = "";
  /// @brief output container name
  std::string m_outContainerName = "";
  /// @brief input jet container name
  std::string m_inJetContainerName = "";
  /// @brief decorate selected objects? defaul passSel
  bool  m_decorateSelectedObjects = true;
  /// @brief fill using SG::VIEW_ELEMENTS to be light weight
  bool  m_createSelectedContainer = false;
  /// @brief look at n objects
  int   m_nToProcess = -1;
  /// @brief minimum number of objects passing cuts
  int   m_pass_min = -1;
  /// @brief maximum number of objects passing cuts
  int   m_pass_max = -1;
  /// @brief require pT < pt_max
  float m_pT_max = 1e8;
  /// @brief require pT > pt_max
  float m_pT_min = 1e8;
  /// @brief require eta < eta_max
  float m_eta_max = 1e8;
  /// @brief require eta > eta_max
  float m_eta_min = 1e8;
  /// @brief require |d0| < d0_max
  float m_d0_max = 1e8;
  /// @brief require |z0| < z0_max
  float m_z0_max = 1e8;
  /// @brief require |z0xsin(theat)| < z0sinT_max
  float m_z0sinT_max = 1e8;
  /// @brief require nBL > nBL_min
  int   m_nBL_min = 1e8;
  /// @brief require nSi > nSi_min
  int   m_nSi_min = 1e8;
  /// @brief require nPixHoles < nPixHoles_max
  float m_nPixHoles_max = 1e8;
  /// @brief require chi2/ndof < chi2NdofCut_max
  float m_chi2NdofCut_max = 1e8;
  /// @brief require TMath::Prob(chi2,ndof) < chi2ProbMax
  float m_chi2Prob_max = 1e8;

  std::string              m_passAuxDecorKeys = "";
  std::string              m_failAuxDecorKeys = "";

  /// @brief do track selection on track within jets
  bool m_doTracksInJets = false;

private:

  std::vector<std::string> m_passKeys;
  std::vector<std::string> m_failKeys;

  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_numObjectPass;    //!

  // cutflow
  TH1D* m_cutflowHist = nullptr;          //!
  TH1D* m_cutflowHistW = nullptr;         //!
  int   m_cutflow_bin;          //!

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
  EL::StatusCode executeTrackCollection ();
  EL::StatusCode executeTracksInJets ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // added functions not from Algorithm
  // why does this need to be virtual?
  virtual int PassCuts( const xAOD::TrackParticle* jet, const xAOD::Vertex *pvx );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(TrackSelector, 1);
  /// @endcond
};

#endif
