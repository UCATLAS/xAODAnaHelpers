#ifndef xAODAnaHelpers_JetSelector_H
#define xAODAnaHelpers_JetSelector_H

// EL include(s):
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// EDM include(s):  
#ifndef __CINT__
  #include "xAODJet/Jet.h"
  #include "xAODJet/JetContainer.h"
#endif

// ROOT include(s):
#include "TH1D.h"

class JetSelector : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;    //!
  int m_pvLocation;       //!

  std::string m_name;
  std::string m_configName;
  int m_type;

  bool m_debug;                  //!

  bool m_isEMjet;                //!
  bool m_isLCjet;                //!

  // cutflow
  bool m_useCutFlow;            //!
  TH1D* m_cutflowHist;          //!
  TH1D* m_cutflowHistW;         //!
  int   m_cutflow_bin;          //!

private:

  // configuration variables
  TString m_inContainerName;      //! input container name
  TString m_outContainerName;     //! output container name
  bool m_decorateSelectedObjects; //! decorate selected objects? defaul passSel
  bool m_createSelectedContainer; //! fill using SG::VIEW_ELEMENTS to be light weight
  int m_nToProcess;               //! look at n objects
  bool m_cleanJets;               //! require cleanJet decoration to not be set and false
  int m_pass_min;                 //! minimum number of objects passing cuts
  int m_pass_max;                 //! maximum number of objects passing cuts
  float m_pT_max;                 //! require pT < pt_max
  float m_pT_min;                 //! require pT > pt_max
  float m_eta_max;                //! require eta < eta_max
  float m_eta_min;                //! require eta > eta_max
  float m_detEta_max;             //! require detEta < detEta_max
  float m_detEta_min;             //! require detEta > detEta_max
  float m_mass_max;               //! require mass < mass_max
  float m_mass_min;               //! require mass > mass_max
  float m_rapidity_max;           //! require rapidity < rapidity_max
  float m_rapidity_min;           //! require rapidity > rapidity_min
  int   m_truthLabel;             //! require truth level on truth jets

  bool m_doJVF;                   //! check JVF
  float m_pt_max_JVF;             //! max pT (JVF is a pileup cut)
  float m_eta_max_JVF;            //! detector eta cut
  float m_JVFCut;                 //! cut value

  TString              m_passAuxDecorKeys;  //!
  TString              m_failAuxDecorKeys;  //!
  std::vector<TString> m_passKeys;  //!
  std::vector<TString> m_failKeys;  //!


  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  JetSelector ();
  JetSelector (std::string name, std::string configName);

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
#ifndef __CINT__   
  virtual EL::StatusCode executeConst( const xAOD::JetContainer* inJets, float mcEvtWeight );
#endif // not __CINT__

  // added functions not from Algorithm
  // why does this need to be virtual?
#ifndef __CINT__   
  virtual int PassCuts( const xAOD::Jet* jet );
#endif // not __CINT__
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetSelector, 1);
};

#endif
