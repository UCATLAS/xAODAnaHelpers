/********************************************************
 * JetCalibrator:
 *
 * This class handles jet calibration and systematics
 *
 * Jeff Dandoy (jeff.dandoy@cern.ch)
 *
 ********************************************************/

#ifndef xAODAnaHelpers_JetCalibrator_H
#define xAODAnaHelpers_JetCalibrator_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicVariation.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "JetCalibTools/IJetCalibrationTool.h"
#include "JetCPInterfaces/ICPJetUncertaintiesTool.h"
#include "JetInterface/IJetSelector.h"
#include "JetInterface/IJetUpdateJvt.h"
#include "JetCPInterfaces/IJetTileCorrectionTool.h"
#include "BoostedJetTaggers/SmoothedWZTagger.h"
#include "xAODCore/ShallowCopy.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

/** @rst
  A wrapper to a few JetETMiss packages. By setting the configuration parameters detailed in the header documentation, one can:

    - calibrate a given jet collection
    - apply systematic variations for JES
    - apply systematic variations for JER
    - decorate the jet with the decision of the Jet Cleaning tool

  When considering systematics, a new ``xAOD::JetCollection`` is created for each systematic variation. The names are then saved in a vector for downstream algorithms to use.

@endrst */
class JetCalibrator : public xAH::Algorithm
{
public:
  /// @brief The name of the input container for this algorithm to read from ``TEvent`` or ``TStore``
  std::string m_inContainerName = "";
  /**
      @brief The name of the nominal output container written by the algorithm to ``TStore``

      If the algorithm applies systematic variations, for each shallow copy saved to ``TStore``, the systematic name will be appended to this.
  */
  std::string m_outContainerName = "";

  /// @brief set to ``AntiKt4EMTopo`` for ``AntiKt4EMTopoJets``
  std::string m_jetAlgo = "";
  /// @brief name of vector holding names of jet systematics given by the JetEtmiss Tools
  std::string m_outputAlgo = "";
  /// @brief Write systematics names to metadata
  bool        m_writeSystToMetadata = false;

  /// @brief config for JetCalibrationTool ConfigDir, set it to override tool defaults
  std::string m_calibConfigDir = "";
  /// @brief config for JetCalibrationTool for Data
  std::string m_calibConfigData = "JES_data2017_2016_2015_Recommendation_Aug2018_rel21.config";
  /// @brief config for JetCalibrationTool for Full Sim MC
  std::string m_calibConfigFullSim = "JES_data2017_2016_2015_Recommendation_Aug2018_rel21.config";
  /// @brief config for JetCalibrationTool for AFII MC
  std::string m_calibConfigAFII = "JES_MC16Recommendation_AFII_EMTopo_April2018_rel21.config";
  /// @brief List of calibration steps. Auto-configured to the Jet/Etmiss recommendation if left blank.
  std::string m_calibSequence = "";
  /// @brief config for Jet Uncertainty Tool
  std::string m_uncertConfig = "";
  /// @brief MC type for Jet Uncertainty Tool
  std::string m_uncertMCType = "";
  /// @brief Override CalibArea tag (default recommended)
  std::string m_overrideCalibArea = "";
  /// @brief Override uncertainties CalibArea tag (default recommended)
  std::string m_overrideUncertCalibArea = "";
  /// @brief Set analysis-specific jet flavour composition file for JetUncertainties (default: unknown comp.)
  std::string m_overrideAnalysisFile = "";
  /// @brief Override uncertainties path (not recommended)
  std::string m_overrideUncertPath = "";

  /// @brief when running data "_Insitu" is appended to calibration sequence
  bool m_forceInsitu = false;
  /// @brief when running FullSim "_Smear" is appended to calibration sequence
  bool m_forceSmear = false;
  /// @brief when using DEV mode of JetCalibTools
  bool m_jetCalibToolsDEV = false;

  /// @brief Run muon-to-jet ghost association (recommended for MET)
  bool m_addGhostMuonsToJets = false;

  /// @brief enable to apply jet cleaning decoration
  bool m_doCleaning = true;
  /// @brief Cut Level
  std::string m_jetCleanCutLevel = "LooseBad";
  /// @brief Save all cleaning decisions as decorators
  bool m_saveAllCleanDecisions = false;
  /// @brief Do Ugly cleaning ( i.e. TileGap 3 )
  bool m_jetCleanUgly = false;
  /// @brief Recalculate JVT using the calibrated jet pT
  bool m_redoJVT = false;

  /// @brief Calculate fJVT using the calibrated jet pT
  bool m_calculatefJVT = false;
  /// @brief Maximum pT of central jets used to compute fJVT decision
  double m_fJVTCentralMaxPt = -1;
  /// @brief fJVT working point
  std::string m_fJVTWorkingPoint = "Medium";

  /// @brief Name of Jvt aux decoration.  Was "JvtJvfcorr" in Rel 20.7, is now "JVFCorr" in Rel 21. Leave empty to use JetMomentTools default.
  std::string m_JvtAuxName = "";
  /// @brief Sort the processed container elements by transverse momentum
  bool    m_sort = true;
  /// @brief Apply jet cleaning to parent jet
  bool    m_cleanParent = false;
  bool    m_applyFatJetPreSel = false;

// systematics
  /// @brief jet tile correction
  bool m_doJetTileCorr = false;

  /// @brief needed in case want to treat MC as pseudoData for JER uncertainty propagation
  bool m_pseudoData = false;

  /// @brief Treat MC as usual, then run the JER uncertainties on it a second time treating it as pseudodata. Overrides m_pseudodata if true.
  bool m_mcAndPseudoData = false;

private:
  /// @brief set to true if systematics asked for and exist
  bool m_runSysts = false; //!

  int m_numEvent;         //!
  int m_numObject;        //!

  std::string m_calibConfig; //!

  std::vector<CP::SystematicSet> m_systList; //!

  // tools
  asg::AnaToolHandle<IJetCalibrationTool>        m_JetCalibrationTool_handle   {"JetCalibrationTool"   , this}; //!
  asg::AnaToolHandle<ICPJetUncertaintiesTool>    m_JetUncertaintiesTool_handle {"JetUncertaintiesTool" , this}; //!
  asg::AnaToolHandle<ICPJetUncertaintiesTool>    m_pseudodataJERTool_handle    {"PseudodataJERTool"    , this}; //!
  asg::AnaToolHandle<IJetUpdateJvt>              m_JVTUpdateTool_handle        {"JetVertexTaggerTool"  , this}; //!
  asg::AnaToolHandle<IJetModifier>               m_fJVTTool_handle             {"JetForwardJvtTool"    , this}; //!
  asg::AnaToolHandle<IJetSelector>               m_JetCleaningTool_handle      {"JetCleaningTool"      , this}; //!
  asg::AnaToolHandle<CP::IJetTileCorrectionTool> m_JetTileCorrectionTool_handle{"JetTileCorrectionTool", this}; //!
  asg::AnaToolHandle<SmoothedWZTagger>           m_SmoothedWZTagger_handle     {"SmoothedWZTagger"     , this}; //!

  std::vector<asg::AnaToolHandle<IJetSelector>>  m_AllJetCleaningTool_handles; //!
  std::vector<std::string>  m_decisionNames;    //!

  // Helper functions
  EL::StatusCode executeSystematic(const CP::SystematicSet& thisSyst, const xAOD::JetContainer* inJets,
                                   std::pair<xAOD::JetContainer*, xAOD::ShallowAuxContainer*>& calibJetsSC,
                                   std::vector<std::string>& vecOutContainerNames, bool isPDCopy);
  EL::StatusCode initializeUncertaintiesTool(asg::AnaToolHandle<ICPJetUncertaintiesTool>& uncToolHandle, bool isData);

public:

  // this is a standard constructor
  JetCalibrator ();

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

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetCalibrator, 1);
  /// @endcond

};

#endif
