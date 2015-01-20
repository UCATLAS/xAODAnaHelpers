/*
=======================================================
https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/AnalysisCommon/AssociationUtils/trunk/doc/README.rst
========================================================
*/

#ifndef xAODAnaHelpers_OVERLAPREMOVER_H
#define xAODAnaHelpers_OVERLAPREMOVER_H

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

/*
namespace xAOD {
#ifndef XAODMUON_MUONCONTAINER_H 
  class MuonContainer;
#endif
#ifndef XAODMUON_MUON_H 
  class Muon;
#endif
#ifndef XAODEGAMMA_ELECTRONCONTAINER_H 
  class ElectronContainer;
#endif
#ifndef XAODEGAMMA_ELECTRON_H 
  class Electron;
#endif
#ifndef XAODEGAMMA_PHOTONCONTAINER_H 
  class PhotonContainer;
#endif
#ifndef XAODEGAMMA_PHOTON_H 
  class Photon;
#endif
#ifndef XAODTAU_TAUJETCONTAINER_H 
  class TauJetContainer;
#endif
#ifndef XAODTAU_TAU_H 
  class Tau;
#endif
#ifndef XAODJET_JETCONTAINER_H 
  class JetContainer;
#endif
#ifndef XAODJET_JET_H 
  class Jet;
#endif
}
*/

class OverlapRemovalTool;

class OverlapRemover : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!
  int m_numEvent;         //!
  int m_numObject;        //!

  std::string m_name;
  std::string m_configName;

  bool m_debug;

private:

  // tools
  OverlapRemovalTool * m_overlapRemovalTool; //!

  // configuration variables
  
  /* Muons */
  TString m_inContainerName_Muons;
  /* Electrons */
  TString m_inContainerName_Electrons;
  /* Photons */
  TString m_inContainerName_Photons;
  /* Taus */
  TString m_inContainerName_Taus;
  /* Jets */
  TString m_inContainerName_Jets;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  OverlapRemover ();
  OverlapRemover (std::string name, std::string configName);

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
  
  // this is needed to distribute the algorithm to the workers
  ClassDef(OverlapRemover, 1);
};

#endif
