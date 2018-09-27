/**
 * @file   MuonEfficiencyCorrector.h
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @brief  Interface to the tools of the MuonEfficiencyCorrections package.
 *
 */

#ifndef xAODAnaHelpers_MuonEfficiencyCorrector_H
#define xAODAnaHelpers_MuonEfficiencyCorrector_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "PileupReweighting/PileupReweightingTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

namespace CP {
  class MuonEfficiencyScaleFactors;
  class MuonTriggerScaleFactors;
}

class MuonEfficiencyCorrector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.

public:

  // configuration variables
  std::string   m_inContainerName = "";

  /// @brief Recommendations release (not recommended to change)
  std::string   m_overrideCalibRelease = "";

  // Reco efficiency SF
  std::string   m_WorkingPointReco = "Loose";

  // Iso efficiency SF
  std::string   m_WorkingPointIso = "LooseTrackOnly";

  /// @brief Use with caution!!!
  bool          m_AllowZeroSF = false;

  /// @brief list of comma-separated single-mu trigger corrections. Individual legs of di-mu menus can be parsed
  std::string   m_MuTrigLegs = "HLT_mu26_imedium";

  // TTVA efficiency SF
  std::string   m_WorkingPointTTVA = "TTVA";

  // systematics
  /**
    @brief The name of the vector containing the names of the systematically-varied muons-related containers from the upstream algorithm, which will be processed by this algorithm.

    Only muon calibration systematics or any other that create shallow copies of electron containers should be passed to this tool. It is advised to run this algorithm before running algorithms combining multiple calibration systematics (e.g. overlap removal).
  */
  std::string   m_inputSystNamesMuons = "";

  /// @brief Write systematics names to metadata
  bool          m_writeSystToMetadata = false;

  float         m_systValReco = 0.0;
  float         m_systValIso = 0.0;
  float         m_systValTrig = 0.0;
  float         m_systValTTVA = 0.0;
  std::string   m_systNameReco = "";
  std::string   m_systNameIso = "";
  std::string   m_systNameTrig = "";
  std::string   m_systNameTTVA = "";
  std::string   m_outputSystNamesReco = "MuonEfficiencyCorrector_RecoSyst";
  std::string   m_outputSystNamesIso = "MuonEfficiencyCorrector_IsoSyst";
  std::string   m_outputSystNamesTrig = "MuonEfficiencyCorrector_TrigSyst";
  std::string   m_outputSystNamesTTVA = "MuonEfficiencyCorrector_TTVASyst";

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  std::vector<CP::SystematicSet> m_systListReco; //!
  std::vector<CP::SystematicSet> m_systListIso;  //!
  std::vector<CP::SystematicSet> m_systListTrig; //!
  std::vector<CP::SystematicSet> m_systListTTVA; //!

  std::string m_outputSystNamesTrigBase; //!

  // tools
  asg::AnaToolHandle<CP::IPileupReweightingTool> m_pileup_tool_handle{"CP::PileupReweightingTool/Pileup"}; //!
  CP::MuonEfficiencyScaleFactors* m_muRecoSF_tool = nullptr;               //!
  std::string m_recoEffSF_tool_name;                                       //!
  CP::MuonEfficiencyScaleFactors* m_muIsoSF_tool = nullptr;                //!
  std::string m_isoEffSF_tool_name;                                        //!
  CP::MuonTriggerScaleFactors* m_muTrigSF_tool = nullptr;                  //!
  std::string m_trigEffSF_tool_name;                                       //!
  CP::MuonEfficiencyScaleFactors* m_muTTVASF_tool = nullptr;               //!
  std::string m_TTVAEffSF_tool_name;                                       //!
  std::map<std::string, std::string> m_SingleMuTriggerMap; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:

  // Tree *myTree; //!
  // TH1 *myHist;  //!

  // this is a standard constructor
  MuonEfficiencyCorrector ();

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
  virtual EL::StatusCode executeSF ( const xAOD::EventInfo* eventInfo, const xAOD::MuonContainer* inputMuons, bool nominal, bool writeSystNames );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonEfficiencyCorrector, 1);
  /// @endcond

};

#endif
