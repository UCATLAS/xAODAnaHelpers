#ifndef xAODAnaHelpers_TauSelector_H
#define xAODAnaHelpers_TauSelector_H

// EDM include(s):
#include "xAODTau/TauJet.h"
#include "xAODTau/TauJetContainer.h"

// ROOT include(s):
#include "TH1D.h"

// external tools include(s):
#include "TauAnalysisTools/TauSelectionTool.h"
#include "TauAnalysisTools/TauOverlappingElectronLLHDecorator.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class TauSelector : public xAH::Algorithm
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
  std::string    m_ConfigPath;               /* path to config file for the TauSelectionTool */
  std::string    m_EleOLRFilePath;           /* path to input file for overlap-based electron veto */
  float          m_minPtDAOD;                /* a minimal pT threshold b/c some derivations may apply
                                                a thinning on tau tracks' features needed by the TauSelectionTool,
						which would cause a crash at runtime */
						
  bool           m_setTauOverlappingEleLLHDecor; 						

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
  TH1D* m_tau_cutflowHist_1;                //!
  TH1D* m_tau_cutflowHist_2;                //!

  int   m_tau_cutflow_all;		    //!
  int   m_tau_cutflow_selected;             //!

  // tools
  TauAnalysisTools::TauSelectionTool  *m_TauSelTool ; //!
  TauAnalysisTools::TauOverlappingElectronLLHDecorator *m_TOELLHDecorator; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  TauSelector (std::string className = "TauSelector");

  ~TauSelector();

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
  bool executeSelection( const xAOD::TauJetContainer* inTaus, float mcEvtWeight, bool countPass,
                         ConstDataVector<xAOD::TauJetContainer>* selectedTaus );
  virtual int passCuts( const xAOD::TauJet* tau );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(TauSelector, 1);
  /// @endcond

};

#endif
