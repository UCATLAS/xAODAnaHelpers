#ifndef xAODAnaHelpers_PhotonSelector_H
#define xAODAnaHelpers_PhotonSelector_H

// algorithm wrapper
#include <xAODAnaHelpers/Algorithm.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEgamma/PhotonContainer.h>

namespace CP {
  class IsolationSelectionTool;
}

namespace Trig {
  class TrigDecisionTool;
  //class TrigEgammaMatchingTool;
}

#include <TH1D.h>

class PhotonSelector : public xAH::Algorithm
{
public:

  bool m_useCutFlow;

  /* configuration variables */

  std::string    m_inContainerName;          /* input container name */
  std::string    m_outContainerName;         /* output container name */
  std::string    m_outAuxContainerName;      /* output auxiliary container name */
  std::string    m_inputAlgoSystNames;
  std::string    m_outputAlgoSystNames;
  bool       	 m_decorateSelectedObjects;  /* decorate selected objects - default "passSel" */
  bool       	 m_createSelectedContainer;  /* fill using SG::VIEW_ELEMENTS to be light weight */
  int        	 m_nToProcess;  	     /* look at n objects */
  int        	 m_pass_min;		     /* minimum number of objects passing cuts */
  int        	 m_pass_max;		     /* maximum number of objects passing cuts */
  float      	 m_pT_max;		     /* require pT < pt_max */
  float      	 m_pT_min;		     /* require pT > pt_min */
  float      	 m_eta_max;		     /* require |eta| < eta_max */
  bool	     	 m_vetoCrack;		     /* require |eta| outside crack region */
  bool           m_doAuthorCut;
  bool           m_doOQCut;

  /* photon PID */
  std::string    m_photonIdCut;              /* Name of ID variable to cut */

  /* isolation */
  std::string    m_MinIsoWPCut;              /* reject objects which do not pass this isolation cut - default = "" (no cut) */
  std::string    m_IsoWPList;                /* decorate objects with 'isIsolated_*' flag for each WP in this input list - default = all current ASG WPs */

  /* trigger matching */

  std::string    m_PhTrigChains;   /* A comma-separated string w/ alll the HLT electron trigger chains for which you want to perform the matching.
  				      This is passed by the user as input in configuration
				      If left empty (as it is by default), no trigger matching will be attempted at all */

private:

  int m_numEvent;           //!
  int m_numObject;          //!
  int m_numEventPass;       //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;      //!

  /* event-level cutflow */

  TH1D* m_cutflowHist;      //!
  TH1D* m_cutflowHistW;     //!
  int   m_cutflow_bin;      //!

  /* object-level cutflow */

  TH1D* m_ph_cutflowHist_1;            //!

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
  CP::IsolationSelectionTool* m_IsolationSelectionTool; //!

  /* trigger decision and matching */
  Trig::TrigDecisionTool* m_trigDecTool; //!
  //Trig::TrigEgammaMatchingTool* m_match_Tool; //!

  std::vector<std::string> m_PhTrigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_ElTrigChains */

public:

  /* this is a standard constructor */

  PhotonSelector (std::string className = "PhotonSelector");

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
