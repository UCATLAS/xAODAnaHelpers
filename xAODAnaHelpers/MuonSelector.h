#ifndef xAODAnaHelpers_MuonSelector_H
#define xAODAnaHelpers_MuonSelector_H

// EL include(s):
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// EDM include(s):
#ifndef __CINT__
  #include "xAODMuon/Muon.h"
  #include "xAODMuon/MuonContainer.h"
  #include "xAODTracking/Vertex.h"
#endif

// ROOT include(s):
#include "TH1D.h"

namespace CP{
  class MuonSelectionTool;
}


class MuonSelector : public EL::Algorithm
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

  std::string m_name;
  std::string m_configName;

  bool m_debug;                 //!
  // cutflow
  bool m_useCutFlow;            //!
  TH1D* m_cutflowHist;          //!
  TH1D* m_cutflowHistW;         //!
  int   m_cutflow_bin;          //!

private:

  // tools
#ifndef __CINT__
  CP::MuonSelectionTool *m_muonSelectionTool;//!
#endif
  // configuration variables

  std::string  m_inContainerName;     // input container name
  std::string  m_outContainerName;    // output container name
  std::string  m_outAuxContainerName; // output auxiliary container name
  bool     m_decorateSelectedObjects; // decorate selected objects? default passSel
  bool     m_createSelectedContainer; // fill using SG::VIEW_ELEMENTS to be light weight
  int      m_nToProcess;              // look at n objects
  bool     m_sort;                    // sort jets before selection
  int      m_pass_min;  	      // minimum number of objects passing cuts
  int      m_pass_max;  	      // maximum number of objects passing cuts
  float    m_pT_max;		      // require pT < pt_max
  float    m_pT_min;		      // require pT > pt_min
  std::string  m_muonQuality;	      // require quality
  std::string  m_muonType;	      // require type
  float    m_eta_max;		      // require |eta| < eta_max
  float    m_d0_max;                  // require d0 < m_d0_max
  float    m_d0sig_max; 	      // require d0 significance (at BL) < m_d0sig_max
  float	   m_z0sintheta_max;          // require z0*sin(theta) (at BL - corrected with vertex info) < m_z0sintheta_max
  bool     m_doIsolation;
  std::string  m_CaloBasedIsoType;
  float    m_CaloBasedIsoCut;
  std::string  m_TrackBasedIsoType;
  float    m_TrackBasedIsoCut;

  std::string              m_passAuxDecorKeys;  //!
  std::string              m_failAuxDecorKeys;  //!
  std::vector<std::string> m_passKeys;  //!
  std::vector<std::string> m_failKeys;  //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  MuonSelector ();
  MuonSelector (std::string name, std::string configName);

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

  // these are the functions not inherited from Algorithm
  virtual EL::StatusCode configure ();
#ifndef __CINT__
  virtual EL::StatusCode executeConst( const xAOD::MuonContainer* inMuons, float mcEvtWeight );
#endif
  // added functions not from Algorithm
  // why does this need to be virtual?
#ifndef __CINT__
  virtual int PassCuts( const xAOD::Muon* muon, const xAOD::Vertex *primaryVertex );
#endif
  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonSelector, 1);
};

#endif
