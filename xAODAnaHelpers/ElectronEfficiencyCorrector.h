/**
 * @file   ElectronEfficiencyCorrector.h
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @brief  Interface to the tools of the ElectronEfficiencyCorrection package.
 *
 */

#ifndef xAODAnaHelpers_ElectronEfficiencyCorrector_H
#define xAODAnaHelpers_ElectronEfficiencyCorrector_H

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"

// external tools include(s):
#include "ElectronEfficiencyCorrection/AsgElectronEfficiencyCorrectionTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

/**
  @rst
    This is the algorithm class that applies generic corrections to electrons. At the moment, only data/MC efficiency correction is included (electron trigger SF and others will follow...).

    In a nutshell, this algorithm performs the following actions:

      - retrieves an ``xAOD::ElectronContainer`` from either ``TEvent`` or ``TStore``
      - adds a scale factor (SF) decoration for each electron in the input container calculated via the ``AsgElectronEfficiencyCorrectionTool`` in `Tools Used <ToolsUsed.html>`__
      - the nominal SF and all the systematically-varied ones are saved as a ``vector<double>`` decoration for each electron

    .. note:: Bear in mind that this algorithm must be called after :cpp:class:`~ElectronSelector`. In fact, the configuration file(s) being used must have the same working point as the one chosen in the selector.

  @endrst
*/
class ElectronEfficiencyCorrector : public xAH::Algorithm
{
public:
  /// @brief The name of the input container for this algorithm to read from ``TEvent`` or ``TStore``
  std::string  m_inContainerName = "";

// systematics
  /**
    @brief The name of the vector containing the names of the systematically-varied electrons-related containers from the upstream algorithm, which will be processed by this algorithm.

    Only electron calibration systematics or any other that create shallow copies of electron containers should be passed to this tool. It is advised to run this algorithm before running algorithms combining multiple calibration systematics (e.g. overlap removal).
  */
  std::string m_inputSystNamesElectrons;

  /// @brief Write systematics names to metadata
  bool m_writeSystToMetadata = false;

  float m_systValPID = 0.0;
  float m_systValIso = 0.0;
  float m_systValReco = 0.0;
  float m_systValTrig = 0.0;
  std::string m_systNamePID = "";
  std::string m_systNameIso = "";
  std::string m_systNameReco = "";
  std::string m_systNameTrig = "";
  std::string m_outputSystNamesPID = "EleEffCorr_PIDSyst";
  std::string m_outputSystNamesIso = "EleEffCorr_IsoSyst";
  std::string m_outputSystNamesReco = "EleEffCorr_RecoSyst";
  std::string m_outputSystNamesTrig = "EleEffCorr_TrigSyst";

  /// @brief Systematic correlation model
  std::string m_correlationModel = "FULL";

  /// @brief PID working point (LooseBLayer, Medium, Tight)
  std::string m_WorkingPointPID = "";

  /// @brief Isolation working point
  std::string m_WorkingPointIso = "";

  /// @brief Reconstruction working point (Reconstruction only)
  std::string m_WorkingPointReco = "";

  /// @brief Trigger working point
  std::string m_WorkingPointTrig = "";

  /// @brief Override corrections map file (not recommended)
  std::string m_overrideMapFilePath = "";

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  std::vector<CP::SystematicSet> m_systListPID;  //!
  std::vector<CP::SystematicSet> m_systListIso;  //!
  std::vector<CP::SystematicSet> m_systListReco; //!
  std::vector<CP::SystematicSet> m_systListTrig; //!

  // tools
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_PID = nullptr;  //!
  std::string m_pidEffSF_tool_name;                                   //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_Iso = nullptr;  //!
  std::string m_IsoEffSF_tool_name;                                   //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_Reco = nullptr; //!
  std::string m_RecoEffSF_tool_name;                                  //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_Trig = nullptr; //!
  std::string m_TrigEffSF_tool_name;                                  //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_TrigMCEff = nullptr; //!
  std::string m_TrigMCEff_tool_name;                                  //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:

  // Tree *myTree; //!
  // TH1 *myHist;  //!


  // this is a standard constructor
  ElectronEfficiencyCorrector ();

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
  virtual EL::StatusCode executeSF ( const xAOD::ElectronContainer* inputElectrons, bool nominal, bool writeSystNames );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(ElectronEfficiencyCorrector, 1);
  /// @endcond

};

#endif
