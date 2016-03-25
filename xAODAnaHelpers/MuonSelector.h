#ifndef xAODAnaHelpers_MuonSelector_H
#define xAODAnaHelpers_MuonSelector_H

// EDM include(s):
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/Vertex.h"

// ROOT include(s):
#include "TH1D.h"

// external tools include(s):
#include "IsolationSelection/IsolationSelectionTool.h"
#include "MuonSelectorTools/MuonSelectionTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

namespace Trig {
  class TrigDecisionTool;
  //class TrigMuonMatching;
}

class MuonSelector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.

public:

  // cutflow
  bool m_useCutFlow;

  // configuration variables
  std::string    m_inContainerName;          /* input container name */
  std::string    m_outContainerName;         /* output container name */
  std::string    m_outAuxContainerName;      /* output auxiliary container name */
  std::string    m_inputAlgoSystNames;
  std::string    m_outputAlgoSystNames;
  bool       	 m_decorateSelectedObjects;  /* decorate selected objects - default "passSel" */
  bool       	 m_createSelectedContainer;  /* fill using SG::VIEW_ELEMENTS to be light weight */
  int            m_nToProcess;               /* look at n objects */
  int            m_pass_min;  	             /* minimum number of objects passing cuts */
  int            m_pass_max;  	             /* maximum number of objects passing cuts */
  float          m_pT_max;		     /* require pT < pt_max */
  float          m_pT_min;		     /* require pT > pt_min */
  int            m_muonQuality;	             /* require quality */
  std::string    m_muonQualityStr;           /* require type */
  std::string    m_muonType;	             /* require type */
  float          m_eta_max;		     /* require |eta| < eta_max */
  float          m_d0_max;                   /* require d0 < m_d0_max */
  float          m_d0sig_max; 	             /* require d0 significance (at BL) < m_d0sig_max */
  float	         m_z0sintheta_max;           /* require z0*sin(theta) (at BL - corrected with vertex info) < m_z0sintheta_max */

  // isolation
  std::string    m_MinIsoWPCut;              /* reject objects which do not pass this isolation cut - default = "" (no cut) */
  std::string    m_IsoWPList;                /* decorate objects with 'isIsolated_*' flag for each WP in this input list - default = all current ASG WPs */
  std::string    m_CaloIsoEff;               /* to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_TrackIsoEff;              /* to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_CaloBasedIsoType;         /* to define a custom WP - make sure "UserDefined" is added in the above input list! */
  std::string    m_TrackBasedIsoType;        /* to define a custom WP - make sure "UserDefined" is added in the above input list! */

  /* trigger matching */
  std::string    m_singleMuTrigChains;       /* A comma-separated string w/ alll the HLT single muon trigger chains for which you want to perform the matching.
  		                                If left empty (as it is by default), no trigger matching will be attempted at all */
  std::string    m_diMuTrigChains;           /* A comma-separated string w/ alll the HLT dimuon trigger chains for which you want to perform the matching.
  					     	If left empty (as it is by default), no trigger matching will be attempted at all */
  float          m_minDeltaR;

  std::string    m_passAuxDecorKeys;
  std::string    m_failAuxDecorKeys;

private:

  int m_numEvent;           //!
  int m_numObject;          //!
  int m_numEventPass;       //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;      //!

  // cutflow
  TH1D* m_cutflowHist;      //!
  TH1D* m_cutflowHistW;     //!
  int   m_cutflow_bin;      //!

  bool  m_isUsedBefore;     //!

  // object cutflow
  TH1D* m_mu_cutflowHist_1;                 //!
  TH1D* m_mu_cutflowHist_2;                 //!

  int   m_mu_cutflow_all;		    //!
  int   m_mu_cutflow_eta_and_quaility_cut;  //!
  int   m_mu_cutflow_ptmax_cut;  	    //!
  int   m_mu_cutflow_ptmin_cut;  	    //!
  int   m_mu_cutflow_type_cut;		    //!
  int   m_mu_cutflow_z0sintheta_cut;	    //!
  int   m_mu_cutflow_d0_cut;		    //!
  int   m_mu_cutflow_d0sig_cut;  	    //!
  int   m_mu_cutflow_iso_cut;		    //!

  std::vector<std::string> m_IsoKeys;       //!

  /* other private members */

  std::vector<std::string>            m_singleMuTrigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_singleMuTrigChains */
  std::vector<std::string>            m_diMuTrigChainsList;     //!  /* contains all the HLT trigger chains tokens extracted from m_diMuTrigChains */


  // tools
  CP::IsolationSelectionTool*    m_IsolationSelectionTool;  //!
  CP::MuonSelectionTool*         m_muonSelectionTool;	    //!

  Trig::TrigDecisionTool*        m_trigDecTool;	            //!
  //Trig::TrigMuonMatching*        m_trigMuonMatchTool;       //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  MuonSelector (std::string className = "MuonSelector");

  ~MuonSelector();

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

  // added functions not from Algorithm
  bool executeSelection( const xAOD::MuonContainer* inMuons, float mcEvtWeight, bool countPass,
                         ConstDataVector<xAOD::MuonContainer>* selectedMuons );
  virtual int passCuts( const xAOD::Muon* muon, const xAOD::Vertex *primaryVertex );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonSelector, 1);
  /// @endcond

};

#endif
