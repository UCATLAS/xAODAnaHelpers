#ifndef xAODAnaHelpers_BJetEfficiencyCorrector_H
#define xAODAnaHelpers_BJetEfficiencyCorrector_H
// EDM includes
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// external tools include(s):
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "FTagAnalysisInterfaces/IBTaggingEfficiencyTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class BJetEfficiencyCorrector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  std::string m_inContainerName = "";
  /**
    @brief The name of the vector containing the names of the systematically-varied jet-related containers from the upstream algorithm, which will be processed by this algorithm.

    Only jet calibration systematics or any other that create shallow copies of jet containers should be passed to this tool. It is advised to run this algorithm before running algorithms combining multiple calibration systematics (e.g. overlap removal).
  */
  std::string m_inputAlgo = "";

  // systematics
  std::string m_systName = "";
  std::string m_outputSystName = "BJetEfficiency_Algo";
  bool        m_writeSystToMetadata = false;

  std::string m_corrFileName = "xAODBTaggingEfficiency/13p6TeV/2023-22-13p6TeV-MC21-CDI_Test_2023-08-1_v1.root";

  std::string m_jetAuthor = "AntiKt4EMPFlowJets";
  /// @brief Minimum pT in MeV for taggable jets
  float       m_minPt = 20e3;
  std::string m_taggerName = "DL1r";
  bool        m_useDevelopmentFile = true;
  bool        m_coneFlavourLabel = true;
  std::string m_systematicsStrategy = "SFEigen";
  /// @brief BTaggingSelectionTool throws an error on missing tagging weights. If false, a warning is given instead
  bool        m_errorOnTagWeightFailure = true;
  /// @brief Decorate tag weights even if we're not doing pseudocontinuous b-tagging
  bool        m_alwaysGetTagWeight = false;

  /// @brief Flag to get Run3 MC-MC SFs
  bool        m_isRun3 = false;

  // allowed operating points:
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingCalibrationDataInterface#xAOD_interface
  //For the fixed cut, valid options are: [ "FixedCutBEff_60", "FixedCutBEff_70", "FixedCutBEff_77", "FixedCutBEff_85" ]

  /// @brief Operating point.
  std::string m_operatingPt = "FixedCutBEff_70";
  /// @brief Operating point that CDI will understand
  std::string m_operatingPtCDI = "";
  /// @brief will only get scale factors for calibrated working points
  bool m_getScaleFactors = false;
  /// @brief will get tagWeight, quantile, and SF
  bool m_useContinuous = false;
  /// @brief The decoration key written to passing objects
  std::string m_decor = "BTag";
  /// @brief Only apply b-tag decision decoration; don't retrieve scale factors (Not recommended. For expert use.)
  bool m_tagDecisionOnly = false;

  /// @brief Select an efficiency map for use in MC/MC and inefficiency scale factors, based on user specified selection of efficiency maps
  bool m_setMapIndex = false;
  std::string m_DSIDtoGenerator_filename = "xAODAnaHelpers/DSIDtoGenerator.txt";

  /// @brief upper pt threshold of b-jet in OR in unit of GeV, negative value means no pt threshold
  float m_orBJetPtUpperThres=-1;

  /// @brief Calibration to use for MC (EfficiencyB/C/T/LightCalibrations), "auto" to determine from sample name (multiple samples can be provided as long as they are separated by ';')
  /// @brief Example: "410470;410250;410558;410464" (Pythia8,Sherpa22,Herwig7,MG)
  std::string m_EfficiencyCalibration = "";
  /// @brief Allow to fallback to "default" configuration when the shower type can't be determined automatically
  bool m_allowCalibrationFallback = false;

  /// @brief To change NP scheme for b-tagging systematics - Loose is the default value in athena
  std::string m_EigenvectorReductionB = "Loose";
  std::string m_EigenvectorReductionC = "Loose";
  std::string m_EigenvectorReductionLight = "Loose";

private:

  /// @brief The decoration key written to passing objects
  std::string m_decorSF = "";
  std::string m_decorWeight = ""; // only for continuous b-tagging
  std::string m_decorQuantile = ""; // only for continuous b-tagging

  std::map<int,std::string> m_DSIDtoGenerator; //!
  std::map<std::string,unsigned int> m_MCIndexes; //!

  std::vector<std::string> m_inputAlgoList; //!

  bool m_runAllSyst = false; //!

  // tools
  asg::AnaToolHandle<IBTaggingSelectionTool> m_BJetSelectTool_handle{"BTaggingSelectionTool" , this}; //!
  asg::AnaToolHandle<IBTaggingEfficiencyTool> m_BJetEffSFTool_handle{"BTaggingEfficiencyTool", this}; //!

  std::vector<CP::SystematicSet> m_systList; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:

  // this is a standard constructor
  BJetEfficiencyCorrector ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  EL::StatusCode executeEfficiencyCorrection(const xAOD::JetContainer* inJets,
					     const xAOD::EventInfo* eventInfo,
					     bool doNominal);
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // Functions needed only if m_setMapIndex is true
  unsigned int getMCIndex (int dsid);
  void makeMCIndexMap (std::string effCalib);
  std::string getFlavorLabel (const xAOD::Jet &jet) const;

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(BJetEfficiencyCorrector, 1);
  /// @endcond

};

#endif
