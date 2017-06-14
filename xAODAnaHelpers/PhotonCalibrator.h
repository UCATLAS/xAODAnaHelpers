#ifndef xAODAnaHelpers_PhotonCalibrator_H
#define xAODAnaHelpers_PhotonCalibrator_H

// CP interface includes
#include <PATInterfaces/SystematicRegistry.h>
#include <PATInterfaces/SystematicSet.h>
#include <PATInterfaces/SystematicsUtil.h>
#include <PATInterfaces/SystematicVariation.h>
#include <PATInterfaces/SystematicCode.h>
#include <xAODEventInfo/EventInfo.h>

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "PhotonEfficiencyCorrection/IAsgPhotonEfficiencyCorrectionTool.h"
#include "IsolationCorrections/IIsolationCorrectionTool.h"

class AsgPhotonIsEMSelector;
class ElectronPhotonShowerShapeFudgeTool;

namespace CP {
  class EgammaCalibrationAndSmearingTool;
}


// algorithm wrapper
#include <xAODAnaHelpers/Algorithm.h>

class PhotonCalibrator : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // configuration variables
  std::string m_inContainerName = "";
  std::string m_outContainerName = "";

  // Calibration information
  // Tool recommends using map, rather than setting individual calib paths.
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PhotonEfficiencyRun2#Recommendations_for_full_2015_an
  std::string m_photonCalibMap = "PhotonEfficiencyCorrection/2015_2016/rel20.7/Moriond2017_v1/map0.txt";

  // recommended files here: https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PhotonEfficiencyRun2#Recommended_input_files
  std::string m_conEffCalibPath = "PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2016.13TeV.rel20.7.25ns.con.v00.root";
  std::string m_uncEffCalibPath = "PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2016.13TeV.rel20.7.25ns.unc.v00.root";
  std::string m_conEffAFIICalibPath = "PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.AFII.con.v01.root";
  std::string m_uncEffAFIICalibPath = "PhotonEfficiencyCorrection/efficiencySF.offline.Tight.2015.13TeV.rel20.AFII.unc.v01.root";

  // ID information
  // recommended files here: ElectronLikelihoodLooseOfflineConfig2016_Smooth.conf
  std::string m_tightIDConfigPath = "ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMTightSelectorCutDefs.conf";
  std::string m_mediumIDConfigPath = "ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMMediumSelectorCutDefs.conf";
  std::string m_looseIDConfigPath = "ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMLooseSelectorCutDefs.conf";

  // sort after calibration
  bool    m_sort = true;

  // systematics
  /// @brief this is the name of the vector of names of the systematically varied containers produced by the upstream algo (e.g., the SC containers with calibration systematics)
  std::string m_inputAlgoSystNames = "";
  /// @brief this is the name of the vector of names of the systematically varied containers produced by THIS algo ( these will be the m_inputAlgoSystNames of the algo downstream
  std::string m_outputAlgoSystNames = "PhotonCalibrator_Syst";

  bool        m_useAFII = false;
  float       m_systVal = 0.0;
  std::string m_systName = "";

  std::string m_esModel = "es2016data_mc15c";
  std::string m_decorrelationModel = "";
  int m_randomRunNumber = -1;

private:
  bool    m_isMC = false; //!

  std::string m_outAuxContainerName;
  std::string m_outSCContainerName;
  std::string m_outSCAuxContainerName;

  std::vector<CP::SystematicSet> m_systList; //!

  EL::StatusCode decorate(xAOD::Photon * photon);

  // tools
  CP::EgammaCalibrationAndSmearingTool* m_EgammaCalibrationAndSmearingTool = nullptr; //!
  asg::AnaToolHandle<CP::IIsolationCorrectionTool> m_isolationCorrectionTool_handle{"CP::IsolationCorrectionTool"}; //!
  ElectronPhotonShowerShapeFudgeTool*   m_photonFudgeMCTool = nullptr; //!
  AsgPhotonIsEMSelector*                m_photonTightIsEMSelector = nullptr; //!
  AsgPhotonIsEMSelector*                m_photonMediumIsEMSelector = nullptr; //!
  AsgPhotonIsEMSelector*                m_photonLooseIsEMSelector = nullptr; //!
  asg::AnaToolHandle<IAsgPhotonEfficiencyCorrectionTool> m_photonTightEffTool_handle{"AsgPhotonEfficiencyCorrectionTool/tight"}; //!
  asg::AnaToolHandle<IAsgPhotonEfficiencyCorrectionTool> m_photonMediumEffTool_handle{"AsgPhotonEfficiencyCorrectionTool/medium"}; //!
  asg::AnaToolHandle<IAsgPhotonEfficiencyCorrectionTool> m_photonLooseEffTool_handle{"AsgPhotonEfficiencyCorrectionTool/loose"}; //!

public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  PhotonCalibrator ();

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
  ClassDef(PhotonCalibrator, 1);
  /// @endcond

};

#endif
