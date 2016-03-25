#ifndef xAODAnaHelpers_ElectronSelector_H
#define xAODAnaHelpers_ElectronSelector_H

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODTracking/Vertex.h"

// package include(s):
#include "xAODAnaHelpers/ParticlePIDManager.h"

// ROOT include(s):
#include "TH1D.h"

// external tools include(s):
#include "IsolationSelection/IsolationSelectionTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"


namespace Trig {
  class TrigDecisionTool;
  //class TrigEgammaMatchingTool;
}

class ElectronSelector : public xAH::Algorithm
{
  /*
  put your configuration variables here as public variables.
  that way they can be set directly from CINT and python.

  variables that don't get filled at submission time should be
  protected from being send from the submission node to the worker
  node (done by the //!)
  */

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
  float      	 m_d0_max;		     /* require d0 < m_d0_max */
  float      	 m_d0sig_max;		     /* require d0 significance (at BL) < m_d0sig_max */
  float	     	 m_z0sintheta_max;	     /* require z0*sin(theta) (at BL - corrected with vertex info) < m_z0sintheta_max */
  bool           m_doAuthorCut;
  bool           m_doOQCut;
  bool           m_doBLTrackQualityCut;

  /* electron PID */

  bool           m_readIDFlagsFromDerivation;
  std::string    m_confDirPID;

  /* likelihood-based  */
  bool           m_doLHPIDcut;
  std::string    m_LHConfigYear;
  std::string    m_LHOperatingPoint;

  /* cut-based */
  bool           m_doCutBasedPIDcut;
  std::string    m_CutBasedConfigYear;
  std::string    m_CutBasedOperatingPoint;

  /* isolation */

  std::string    m_MinIsoWPCut;              /* reject objects which do not pass this isolation cut - default = "" (no cut) */
  std::string    m_IsoWPList;                /* decorate objects with 'isIsolated_*' flag for each WP in this input list - default = all current ASG WPs */
  std::string    m_CaloIsoEff;               /* to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_TrackIsoEff;              /* to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_CaloBasedIsoType;         /* to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_TrackBasedIsoType;        /* to define a custom WP - make sure "UserDefined" is added in the above input list! */

  /* trigger matching */

  std::string    m_ElTrigChains;   /* A comma-separated string w/ alll the HLT electron trigger chains for which you want to perform the matching.
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

  bool  m_isUsedBefore;     //!

  /* object-level cutflow */

  TH1D* m_el_cutflowHist_1;            //!
  TH1D* m_el_cutflowHist_2;            //!

  int   m_el_cutflow_all;              //!
  int   m_el_cutflow_author_cut;       //!
  int   m_el_cutflow_OQ_cut;           //!
  int   m_el_cutflow_ptmax_cut;        //!
  int   m_el_cutflow_ptmin_cut;        //!
  int   m_el_cutflow_eta_cut;          //!
  int   m_el_cutflow_z0sintheta_cut;   //!
  int   m_el_cutflow_d0_cut;           //!
  int   m_el_cutflow_d0sig_cut;        //!
  int   m_el_cutflow_BL_cut;           //!
  int   m_el_cutflow_PID_cut;          //!
  int   m_el_cutflow_iso_cut;          //!

  std::vector<std::string> m_IsoKeys;  //!

  /* tools */

  CP::IsolationSelectionTool*      m_IsolationSelectionTool; //!

  /* PID manager(s) */
  ElectronLHPIDManager*            m_el_LH_PIDManager;       //!
  ElectronCutBasedPIDManager*      m_el_CutBased_PIDManager; //!

  Trig::TrigDecisionTool*          m_trigDecTool;            //!
  //Trig::TrigEgammaMatchingTool*    m_trigElMatchTool;        //!

  /* other private members */

  std::vector<std::string>            m_ElTrigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_ElTrigChains */

public:

  /* this is a standard constructor */

  ElectronSelector (std::string className = "ElectronSelector");

  ~ElectronSelector();

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

  /* added functions not from Algorithm */

  bool executeSelection( const xAOD::ElectronContainer* inElectrons, float mcEvtWeight, bool countPass,
                         ConstDataVector<xAOD::ElectronContainer>* selectedElectrons );
  virtual int passCuts( const xAOD::Electron* electron, const xAOD::Vertex *primaryVertex );

  /// @cond
  /* this is needed to distribute the algorithm to the workers */
  ClassDef(ElectronSelector, 1);
  /// @endcond
};

#endif
