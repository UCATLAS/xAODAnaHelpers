#ifndef xAODAnaHelpers_MuonEfficiencyCorrector_H
#define xAODAnaHelpers_MuonEfficiencyCorrector_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// external tools include(s):
#include "MuonEfficiencyCorrections/MuonEfficiencyScaleFactors.h"
#include "MuonEfficiencyCorrections/MuonTriggerScaleFactors.h"
#include "MuonSelectorTools/MuonSelectionTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class MuonEfficiencyCorrector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.

public:
  // configuration variables
  std::string   m_inContainerName;

  // Efficiency SF
  std::string   m_WorkingPoint;
  std::string   m_DataPeriod;

  // Trigger SF                                                                                                                                             
  int           m_runNumber;
  std::string   m_SingleMuTrig;      // this can be either a single muon trigger chain, or an OR of ( 2 single muon chains )
  std::string   m_DiMuTrig;          // this can be either a dimuon trigger chain, or an OR of ( N single muon trigger chains, dimuon chain ) 

  // systematics
  std::string m_inputAlgoSystNames;  // this is the name of the vector of names of the systematically varied containers produced by the
  			             // upstream algo (e.g., the SC containers with calibration systematics)
  bool          m_runAllSyst;
  std::string   m_outputSystNames;
  bool          m_runSystsEff;
  bool          m_runSystTrig;
  std::string   m_systNameEff;
  std::string   m_systNameTrig;
  std::string   m_outputSystNamesEff;
  std::string   m_outputSystNamesTrig;
  float         m_systValEff;
  float         m_systValTrig;
  bool          m_runAllSystEff;
  bool          m_runAllSystTrig;

private:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!
  int m_numEvent;         //!
  int m_numObject;        //!

  std::vector<CP::SystematicSet> m_systListEff;  //!
  std::vector<CP::SystematicSet> m_systListTrig; //!

  // tools
  CP::MuonSelectionTool           *m_muonSelectionTool; //!
  CP::MuonEfficiencyScaleFactors  *m_muonEffSFTool;     //!
  CP::MuonTriggerScaleFactors     *m_muonTrigSFTool;    //!

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
  virtual EL::StatusCode configure ();

  virtual EL::StatusCode executeSF (  const xAOD::MuonContainer* inputMuons, const xAOD::EventInfo* eventInfo, unsigned int countSyst  );


  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonEfficiencyCorrector, 1);
};

#endif
