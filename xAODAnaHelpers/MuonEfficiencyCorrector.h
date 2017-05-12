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
  std::string   m_outContainerName = "";

  std::string   m_calibRelease = "Data15_allPeriods_241115";

  // Reco efficiency SF
  std::string   m_WorkingPointReco = "Loose";

  // Iso efficiency SF
  std::string   m_WorkingPointIso = "LooseTrackOnly";

  // Trigger efficiency SF
  int           m_runNumber2015 = 276329;
  int           m_runNumber2016 = 300345;
  bool          m_useRandomRunNumber = true;
  /// @brief Use with caution!!!
  bool          m_AllowZeroSF = false;
  std::string   m_WorkingPointRecoTrig = "Loose";
  std::string   m_WorkingPointIsoTrig = "LooseTrackOnly";
  /// @brief comma-separated list of years
  std::string   m_Years = "2016";
  std::string   m_MCCampaign = "";
  /// @brief list of comma-separated single-mu trigger corrections. Individual legs of di-mu menus can be parsed
  std::string   m_MuTrigLegs = "HLT_mu26_imedium";

  // TTVA efficiency SF
  std::string   m_WorkingPointTTVA = "TTVA";

  // systematics
  /// @brief this is the name of the vector of names of the systematically varied containers produced by the upstream algo (e.g., the SC containers with calibration systematics)
  std::string   m_inputAlgoSystNames = "";
  // this is the name of the vector of names of the systematically varied containers to be fed to the downstream algos. We need that as we deepcopy the input containers
  std::string   m_outputAlgoSystNames = "";
  // this is the name of the vector of names for the systematics to be used for the creation of a parallel container. This will be just a copy of the nominal one with the sys name appended. Use cases: MET-specific systematics.
  std::string   m_sysNamesForParCont = "";

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
  std::string   m_outputSystNamesTrigMCEff = "MuonEfficiencyCorrector_TrigMCEff";
  std::string   m_outputSystNamesTTVA = "MuonEfficiencyCorrector_TTVASyst";

  /// @brief will consider efficiency decorations only for the nominal run
  bool          m_decorateWithNomOnInputSys = true;

private:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!

  bool m_isMC;            //!

  int m_numEvent;         //!
  int m_numObject;        //!

  std::vector<CP::SystematicSet> m_systListReco; //!
  std::vector<CP::SystematicSet> m_systListIso;  //!
  std::vector<CP::SystematicSet> m_systListTrig; //!
  std::vector<CP::SystematicSet> m_systListTTVA; //!

  std::vector<std::string> m_sysNames; //!

  // tools
  asg::AnaToolHandle<CP::IPileupReweightingTool> m_pileup_tool_handle;     //!
  CP::MuonEfficiencyScaleFactors* m_muRecoSF_tool = nullptr;               //!
  std::string m_recoEffSF_tool_name;                                       //!
  CP::MuonEfficiencyScaleFactors* m_muIsoSF_tool = nullptr;                //!
  std::string m_isoEffSF_tool_name;                                        //!
  std::map<std::string, CP::MuonTriggerScaleFactors*>  m_muTrigSF_tools;   //!
  std::map<std::string, std::string> m_trigEffSF_tool_names;               //!
  std::vector<std::string> m_YearsList;                                    //!
  CP::MuonEfficiencyScaleFactors* m_muTTVASF_tool = nullptr;               //!
  std::string m_TTVAEffSF_tool_name;                                       //!
  std::vector<std::string> m_SingleMuTriggers;                             //!

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
  virtual EL::StatusCode executeSF ( const xAOD::EventInfo* eventInfo, const xAOD::MuonContainer* inputMuons, unsigned int countSyst, bool isNomSel );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonEfficiencyCorrector, 1);
  /// @endcond

};

#endif
