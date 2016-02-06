#ifndef xAODAnaHelpers_METConstructor_H
#define xAODAnaHelpers_METConstructor_H

#include <xAODAnaHelpers/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"



using std::string;

namespace met { class METMaker; }
namespace TauAnalysisTools { class TauSelectionTool; }


class METConstructor : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!

  // configuration variables
  TString m_referenceMETContainer;
  TString m_mapName;
  TString m_coreName;
  TString m_outputContainer;
  TString m_inputJets;
  TString m_inputElectrons;
  TString m_inputPhotons;
  TString m_inputTaus;
  TString m_inputMuons;

  bool    m_doElectronCuts;
  bool    m_doPhotonCuts;
  bool    m_doTauCuts;
  bool    m_doMuonCuts;

  bool    m_doMuonEloss;
  bool    m_doIsolMuonEloss;
  bool    m_doJVTCut;

  bool    m_useCaloJetTerm;
  bool    m_useTrackJetTerm;

private:

  // tools
  met::METMaker* m_metmaker; //!
  TauAnalysisTools::TauSelectionTool* m_tauSelTool; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  METConstructor (std::string className = "METConstructor");

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute();
  virtual EL::StatusCode finalize();
  virtual EL::StatusCode histFinalize();

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(METConstructor, 1);
  /// @endcond

};

#endif
