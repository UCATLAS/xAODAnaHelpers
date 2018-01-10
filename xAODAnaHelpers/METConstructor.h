#ifndef xAODAnaHelpers_METConstructor_H
#define xAODAnaHelpers_METConstructor_H

#include <xAODAnaHelpers/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "AsgTools/AnaToolHandle.h"

#include "METInterface/IMETMaker.h"
#include "METInterface/IMETSystematicsTool.h"
#include "METInterface/IMETSignificance.h"

#include "TauAnalysisTools/ITauSelectionTool.h"

#include "PATInterfaces/SystematicRegistry.h"
//look at https://twiki.cern.ch/twiki/bin/view/AtlasComputing/SoftwareTutorialxAODAnalysisInROOT


using std::string;

class METConstructor : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  // configuration variables
  TString m_referenceMETContainer = "MET_Reference_AntiKt4LCTopo";
  TString m_mapName = "METAssoc_AntiKt4LCTopo";
  TString m_coreName = "MET_Core_AntiKt4LCTopo";
  TString m_outputContainer = "NewRefFinal";
  TString m_inputJets = "";
  TString m_inputElectrons = "";
  TString m_inputPhotons = "";
  TString m_inputTaus = "";
  TString m_inputMuons = "";


  //std::string  m_inputAlgoJets;  // name of vector<string> of syst retrieved from TStore
  //std::string  m_inputAlgoSystMuons;  // name of vector<string> of syst retrieved from TStore
  //std::string  m_inputAlgoSystEle;  // name of vector<string> of syst retrieved from TStore
  //std::string m_inputAlgoPhotons; // name of vector<string> of syst retrieved from TStore

  bool    m_doElectronCuts = false;
  bool    m_doPhotonCuts = false;
  bool    m_doTauCuts = false;
  bool    m_doMuonCuts = false;

  bool    m_doMuonEloss = false;
  bool    m_doIsolMuonEloss = false;
  bool    m_doJVTCut = false;
  bool    m_dofJVTCut = false;

  /// Rebuild MET using tracks in calo jets
  bool    m_rebuildUsingTracksInJets = false;
  /**
    @rst
      Include soft cluster terms if rebuilding MET using jet terms (only considered if :cpp:member:`~METConstructor::m_rebuildUsingTracksInJets` is false)

    @endrst
  */
  bool    m_addSoftClusterTerms = false;

  // MET significance
  /// @brief Force AFII flag in calculation, in case metadata is broken
  bool m_setAFII = false;
  /// @brief Enable MET significance calculation
  bool m_calculateSignificance = false;
  /// @brief Introduce "resolution" for jets with low JVT, if the analysis is sensitive to pileup jets
  bool m_significanceTreatPUJets = true;
  /// @brief Set soft term resolution
  double m_significanceSoftTermReso = 10.0;

  // used for systematics
  /// @brief set to false if you want to run met systematics
  bool m_runNominal = true;
  /// @brief do not change it, not useful
  std::string m_systName = "All";
  float m_systVal = 1.0;

  /// @brief Write systematics names to metadata
  bool        m_writeSystToMetadata = false;

  std::string m_SoftTermSystConfigFile = "TrackSoftTerms.config";

  /** @rst
        Name of jet systematics vector from  :cpp:class:`~JetCalibrator`.
      @endrst
   */
  std::string m_jetSystematics = "";
  /** @rst
        Name of electron systematics vector from  :cpp:class:`~ElectronCalibrator`.
      @endrst
   */
  std::string m_eleSystematics = "";
  /** @rst
        Name of muon systematics vector from  :cpp:class:`~MuonCalibrator`.
      @endrst
   */
  std::string m_muonSystematics = "";
  /** @rst
        Name of photon systematics vector from :cpp:class:`~PhotonCalibrator`.
      @endrst
   */
  std::string m_phoSystematics = "";

  std::string m_outputAlgoSystNames = "";


private:
  // tools
  asg::AnaToolHandle<IMETMaker> m_metmaker_handle{"met::METMaker", this}; //!
  asg::AnaToolHandle<IMETSystematicsTool> m_metSyst_handle{"met::METSystematicsTool", this}; //!
  asg::AnaToolHandle<IMETSignificance> m_metSignificance_handle{"met::METSignificance", this}; //!

  asg::AnaToolHandle<TauAnalysisTools::ITauSelectionTool> m_tauSelTool{"TauAnalysisTools::TauSelectionTool", this}; //!

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
  METConstructor ();

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
