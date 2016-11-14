#ifndef xAODAnaHelpers_METConstructor_H
#define xAODAnaHelpers_METConstructor_H

#include <xAODAnaHelpers/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "AsgTools/AnaToolHandle.h"


#include "PATInterfaces/SystematicRegistry.h"
//look at https://twiki.cern.ch/twiki/bin/view/AtlasComputing/SoftwareTutorialxAODAnalysisInROOT


using std::string;


class IMETMaker;
class IMETSystematicsTool;

namespace TauAnalysisTools { class TauSelectionTool; }


class METConstructor : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

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


  std::string  m_inputAlgoJets;  // name of vector<string> of syst retrieved from TStore
  std::string  m_inputAlgoSystMuons;  // name of vector<string> of syst retrieved from TStore
  std::string  m_inputAlgoSystEle;  // name of vector<string> of syst retrieved from TStore
  std::string m_inputAlgoPhotons; // name of vector<string> of syst retrieved from TStore

  bool    m_doElectronCuts;
  bool    m_doPhotonCuts;
  bool    m_doTauCuts;
  bool    m_doMuonCuts;

  bool    m_doMuonEloss;
  bool    m_doIsolMuonEloss;
  bool    m_doJVTCut;

  bool    m_useCaloJetTerm;
  bool    m_useTrackJetTerm;

  bool m_runNominal;
  
  bool m_isMC; //add bool 

  
  float m_systVal;
  std::string m_systName;

  std::string m_SoftTermSystConfigFile;

  /** @rst
        Name of jet systematics vector from  :cpp:class:`~JetCalibrator`.
      @endrst
   */
  std::string m_jetSystematics;
  /** @rst
        Name of electron systematics vector from  :cpp:class:`~ElectronCalibrator`.
      @endrst
   */
  std::string m_eleSystematics;
  /** @rst
        Name of muon systematics vector from  :cpp:class:`~MuonCalibrator`.
      @endrst
   */
  std::string m_muonSystematics;
  /** @rst
        Name of photon systematics vector from :cpp:class:`~PhotonCalibrator`.
      @endrst
   */
  std::string m_phoSystematics;


  

private:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!

  // tools
  asg::AnaToolHandle<IMETMaker> m_metmaker_handle; //!

  asg::AnaToolHandle<IMETSystematicsTool> m_metSyst_handle; //!   

  TauAnalysisTools::TauSelectionTool* m_tauSelTool; //!

  TString coreMetKey;
  std::vector<CP::SystematicSet> sysList; //!

  int m_numEvent;         //!

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
