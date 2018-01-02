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
#include "JetResolution/IJERTool.h"
#include "JetResolution/IJERSmearingTool.h"
#include "JetInterface/IJetSelector.h"
#include "JetInterface/IJetUpdateJvt.h"
#include "JetCPInterfaces/IJetTileCorrectionTool.h"

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
  /// @brief config for JetCalibrationTool for Data
  std::string m_calibConfigData = "JES_MC15Prerecommendation_April2015.config";
  /// @brief config for JetCalibrationTool for Full Sim MC
  std::string m_calibConfigFullSim = "JES_MC15Prerecommendation_April2015.config";
  /// @brief config for JetCalibrationTool for AFII MC
  std::string m_calibConfigAFII = "JES_Prerecommendation2015_AFII_Apr2015.config";
  /// @brief config files actually passed to JetCalibrationTool chosen from the above depending on what information stored in the input file
  std::string m_calibConfig = "";
  /// @brief List of calibration steps. "Insitu" added automatically if running on data
  std::string m_calibSequence = "JetArea_Residual_Origin_EtaJES_GSC";
  /// @brief CalibArea tag on CVMFS (for pointing at an area outside of the current release)
  std::string m_calibArea = "00-04-81"; // current as of 19/11/17
  /// @brief config for JES Uncertainty Tool
  std::string m_JESUncertConfig = "";
  /// @brief JetUncertaintiesTool parameter
  std::string m_JESUncertMCType = "MC15";
  /** @rst
    If you do not want to use SampleHandler to mark samples as AFII, this flag can be used to force run the AFII configurations.

    With SampleHandler, one can define sample metadata in job steering macro. You can do this with relevant samples doing something like:

    .. code-block:: c++

      // access a single sample
      Sample *sample = sh.get ("mc14_13TeV.blahblahblah");
      sample->setMetaString("SimulationFlavour", "AFII");

  @endrst */
  bool m_setAFII = false;
  /// @brief when running data "_Insitu" is appended to this string
  bool m_forceInsitu = true;
  /// @brief when using DEV mode of JetCalibTools
  bool m_jetCalibToolsDEV = false;

  // @brief Config for JER Uncert Tool. If not empty the tool will run
  std::string m_JERUncertConfig = "";
  /// @brief Set systematic mode as Full (true) or Simple (false)
  bool m_JERFullSys = false;
  /// @brief Apply nominal smearing
  bool m_JERApplyNominal = false;

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

  /// @brief Name of Jvt aux decoration.  Was "JvtJvfcorr" in Rel 20.7, is now "JVFCorr" in Rel 21. Leave empty to use JetMomentTools default.  This must be left empty for RootCore (r20.7) code!
  std::string m_JvtAuxName = "";
  /// @brief Sort the processed container elements by transverse momentum
  bool    m_sort = true;
  /// @brief Apply jet cleaning to parent jet
  bool    m_cleanParent = false;
  bool    m_applyFatJetPreSel = false;

// systematics
  /// @brief jet tile correction
  bool m_doJetTileCorr = false;

  /** If running systematics, the name of the systematic */
  std::string m_systNameJES = "";
  /** If running systematics, the value to set the systematic to
      @rst
          .. note:: This will set the systematic to the value :math:`\pm x`.
      @endrst
   */
  float m_systValJES = 0.0;

  /** If running systematics, the name of the systematic */
  std::string m_systNameJER = "";

private:
  /// @brief set to true if systematics asked for and exist
  bool m_runSysts = false; //!

  int m_numEvent;         //!
  int m_numObject;        //!

  bool m_isFullSim;       //!

  std::vector<CP::SystematicSet> m_systList; //!
  std::vector<int> m_systType; //!

  // tools
  asg::AnaToolHandle<IJetCalibrationTool>        m_JetCalibrationTool_handle   {"JetCalibrationTool"   , this}; //!
  asg::AnaToolHandle<ICPJetUncertaintiesTool>    m_JetUncertaintiesTool_handle {"JetUncertaintiesTool" , this}; //!
  asg::AnaToolHandle<IJERTool>                   m_JERTool_handle              {"JERTool"              , this}; //!
  asg::AnaToolHandle<IJERSmearingTool>           m_JERSmearingTool_handle      {"JERSmearingTool"      , this}; //!
  asg::AnaToolHandle<IJetUpdateJvt>              m_JVTUpdateTool_handle        {"JetVertexTaggerTool"  , this}; //!
  asg::AnaToolHandle<IJetModifier>               m_fJVTTool_handle             {"JetForwardJvtTool"    , this}; //!
  asg::AnaToolHandle<IJetSelector>               m_JetCleaningTool_handle      {"JetCleaningTool"      , this}; //!
  asg::AnaToolHandle<CP::IJetTileCorrectionTool> m_JetTileCorrectionTool_handle{"JetTileCorrectionTool", this}; //!

  std::vector<asg::AnaToolHandle<IJetSelector>>  m_AllJetCleaningTool_handles; //!
  std::vector<std::string>  m_decisionNames;    //!

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
