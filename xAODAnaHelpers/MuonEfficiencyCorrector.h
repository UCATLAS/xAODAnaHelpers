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
  std::string   m_inContainerName;

  std::string   m_calibRelease;

  // Reco efficiency SF
  std::string   m_WorkingPointReco;

  // Iso efficiency SF
  std::string   m_WorkingPointIso;

  // Trigger efficiency SF
  int           m_runNumber2015;
  int           m_runNumber2016;
  bool          m_useRandomRunNumber;
  std::string   m_WorkingPointRecoTrig;
  std::string   m_WorkingPointIsoTrig;
  std::string   m_Years;
  std::string   m_MCCampaign;
  std::string   m_MuTrigLegs;  // list of comma-separated single-mu trigger corrections. Individual legs of di-mu menus can be parsed

  // TTVA efficiency SF
  std::string   m_WorkingPointTTVA;

  // systematics
  std::string   m_inputAlgoSystNames;  // this is the name of the vector of names of the systematically varied containers produced by the
  			               // upstream algo (e.g., the SC containers with calibration systematics)

  float         m_systValReco;
  float         m_systValIso;
  float         m_systValTrig;
  float         m_systValTTVA;
  std::string   m_systNameReco;
  std::string   m_systNameIso;
  std::string   m_systNameTrig;
  std::string   m_systNameTTVA;
  std::string   m_outputSystNamesReco;
  std::string   m_outputSystNamesIso;
  std::string   m_outputSystNamesTrig;
  std::string   m_outputSystNamesTrigMCEff;
  std::string   m_outputSystNamesTTVA;

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

  // tools
  asg::AnaToolHandle<CP::IPileupReweightingTool> m_pileup_tool_handle;     //!
  CP::MuonEfficiencyScaleFactors* m_muRecoSF_tool;                         //!
  std::string m_recoEffSF_tool_name;                                       //!
  CP::MuonEfficiencyScaleFactors* m_muIsoSF_tool;                          //!
  std::string m_isoEffSF_tool_name;                                        //!
  std::map<std::string, CP::MuonTriggerScaleFactors*>  m_muTrigSF_tools;   //!   
  std::map<std::string, std::string> m_trigEffSF_tool_names;               //!
  std::vector<std::string> m_YearsList;                                    //!
  CP::MuonEfficiencyScaleFactors* m_muTTVASF_tool;                         //!
  std::string m_TTVAEffSF_tool_name;                                       //!
  std::vector<std::string> m_SingleMuTriggers;                             //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:

  // Tree *myTree; //!
  // TH1 *myHist;  //!

  // this is a standard constructor
  MuonEfficiencyCorrector (std::string className = "MuonEfficiencyCorrector");

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
  virtual EL::StatusCode executeSF ( const xAOD::EventInfo* eventInfo, const xAOD::MuonContainer* inputMuons, unsigned int countSyst  );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonEfficiencyCorrector, 1);
  /// @endcond

};

#endif
