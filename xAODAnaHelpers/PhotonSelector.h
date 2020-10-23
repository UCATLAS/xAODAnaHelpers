#ifndef xAODAnaHelpers_PhotonSelector_H
#define xAODAnaHelpers_PhotonSelector_H

// algorithm wrapper
#include <xAODAnaHelpers/Algorithm.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEgamma/PhotonContainer.h>

namespace CP {
  class IsolationSelectionTool;
}

#include <TH1D.h>

class PhotonSelector : public xAH::Algorithm
{
public:

  bool m_useCutFlow = true;

  /** configuration variables */
  /** input container name */
  std::string    m_inContainerName = "";
  /** output container name */
  std::string    m_outContainerName = "";
  /** output auxiliary container name */
  std::string    m_inputAlgoSystNames = "";
  std::string    m_outputAlgoSystNames = "PhotonSelector_Syst";
  /** decorate selected objects - default "passSel" */
  bool       	 m_decorateSelectedObjects = true;
  /** fill using SG::VIEW_ELEMENTS to be light weight */
  bool       	 m_createSelectedContainer = true;
  /** look at n objects */
  int        	 m_nToProcess = -1;
  /** minimum number of objects passing cuts */
  int        	 m_pass_min = -1;
  /** maximum number of objects passing cuts */
  int        	 m_pass_max = -1;
  /** require pT < pt_max */
  float      	 m_pT_max = 1e8;
  /** require pT > pt_min */
  float      	 m_pT_min = 1e8;
  /** require |eta| < eta_max */
  float      	 m_eta_max = 1e8;
  /** require |eta| outside crack region */
  bool	     	 m_vetoCrack = true;
  bool           m_doAuthorCut = true;
  bool           m_doOQCut = true;
  /** read object quality from derivation, rather than calculating it on the fly */
  bool           m_readOQFromDerivation = false;

  /* photon PID */
  /** Name of ID variable to cut */
  std::string    m_photonIdCut = "None";

  /* isolation */
  /** reject objects which do not pass this isolation cut - default = "" (no cut) */
  std::string    m_MinIsoWPCut = "";
  /** decorate objects with 'isIsolated_*' flag for each WP in this input list - default = all current ASG WPs */
  std::string    m_IsoWPList = "FixedCutTightCaloOnly,FixedCutTight,FixedCutLoose";

private:
  std::string    m_outAuxContainerName; //!

  int m_numEvent;           //!
  int m_numObject;          //!
  int m_numEventPass;       //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;      //!

  /* event-level cutflow */

  TH1D* m_cutflowHist = nullptr;      //!
  TH1D* m_cutflowHistW = nullptr;     //!
  int   m_cutflow_bin;      //!

  /* object-level cutflow */

  TH1D* m_ph_cutflowHist_1 = nullptr;            //!

  int   m_ph_cutflow_all;              //!
  int   m_ph_cutflow_author_cut;       //!
  int   m_ph_cutflow_OQ_cut;           //!
  int   m_ph_cutflow_PID_cut;          //!
  int   m_ph_cutflow_ptmax_cut;        //!
  int   m_ph_cutflow_ptmin_cut;        //!
  int   m_ph_cutflow_eta_cut;          //!
  int   m_ph_cutflow_iso_cut;          //!


  std::vector<std::string> m_IsoKeys;  //!

  /* tools */
  CP::IsolationSelectionTool* m_IsolationSelectionTool = nullptr; //!

public:

  /* this is a standard constructor */

  PhotonSelector ();

  ~PhotonSelector();

  /* these are the functions inherited from Algorithm */

  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  /* these are the functions not inherited from Algorithm */

  bool executeSelection( const xAOD::PhotonContainer* inPhotons, float mcEvtWeight, bool countPass,
			  ConstDataVector<xAOD::PhotonContainer>* selectedPhotons );
  virtual bool passCuts( const xAOD::Photon* photon );


  /// @cond
  ClassDef(PhotonSelector, 1);
  /// @endcond

};


#endif //#ifndef xAODAnaHelpers_PhotonSelector_H
