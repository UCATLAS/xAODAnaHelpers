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
#include "xAODBTaggingEfficiency/IBTaggingSelectionTool.h"
#include "xAODBTaggingEfficiency/IBTaggingEfficiencyTool.h"

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

  std::string m_corrFileName = "xAODBTaggingEfficiency/13TeV/2016-20_7-13TeV-MC15-CDI-July12_v1.root";

  std::string m_jetAuthor = "AntiKt4EMTopoJets";
  std::string m_taggerName = "MV2c10";
  bool        m_useDevelopmentFile = true;
  bool        m_coneFlavourLabel = true;
  std::string m_systematicsStrategy = "SFEigen";

  // allowed operating points:
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingCalibrationDataInterface#xAOD_interface
  //For the fixed cut, valid options are: [ "FixedCutBEff_30", "FixedCutBEff_50", "FixedCutBEff_60", "FixedCutBEff_70", "FixedCutBEff_77", "FixedCutBEff_80", "FixedCutBEff_85", "FixedCutBEff_90" ]
  //For the variable cut, valid options are: [ "FlatBEff_30", "FlatBEff_40", "FlatBEff_50", "FlatBEff_60", "FlatBEff_70", "FlatBEff_77", "FlatBEff_85" ]

  /// @brief Operating point.
  std::string m_operatingPt = "FixedCutBEff_70";
  /// @brief Operating point that CDI will understand
  std::string m_operatingPtCDI = "";
  /// @brief will only get scale factors for calibrated working points
  bool m_getScaleFactors = false;
  /// @brief The decoration key written to passing objects
  std::string m_decor = "BTag";

private:

  /// @brief The decoration key written to passing objects
  std::string m_decorSF = "";

  std::vector<std::string> m_inputAlgoList; //!

  bool m_runAllSyst = false; //!

  // tools
  asg::AnaToolHandle<IBTaggingSelectionTool> m_BJetSelectTool_handle{"BTaggingSelectionTool"};  //!
  asg::AnaToolHandle<IBTaggingEfficiencyTool> m_BJetEffSFTool_handle{"BTaggingEfficiencyTool"}; //!

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

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(BJetEfficiencyCorrector, 1);
  /// @endcond

};

#endif
