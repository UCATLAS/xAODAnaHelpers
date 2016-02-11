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
class AsgPhotonIsEMSelector;
class ElectronPhotonShowerShapeFudgeTool;
class AsgPhotonEfficiencyCorrectionTool;

namespace CP {
  class EgammaCalibrationAndSmearingTool;
  class IsolationCorrectionTool;
}


// algorithm wrapper
#include <xAODAnaHelpers/Algorithm.h>

class PhotonCalibrator : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // configuration variables
  std::string m_inContainerName;
  std::string m_outContainerName;

  // sort after calibration
  bool    m_sort;
  bool    m_toolInitializationAtTheFirstEventDone; //!
  bool    m_isMC; //!

  // systematics
  std::string m_inputAlgoSystNames;  // this is the name of the vector of names of the systematically varied containers produced by the
  			             // upstream algo (e.g., the SC containers with calibration systematics)
  std::string m_outputAlgoSystNames; // this is the name of the vector of names of the systematically varied containers produced by THIS
  				     // algo ( these will be the m_inputAlgoSystNames of the algo downstream
  bool        m_useAFII;
  float       m_systVal;
  std::string m_systName;

  std::string m_esModel;
  std::string m_decorrelationModel;

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  std::string m_outAuxContainerName;
  std::string m_outSCContainerName;
  std::string m_outSCAuxContainerName;

  std::vector<CP::SystematicSet> m_systList; //!

  EL::StatusCode decorate(xAOD::Photon * photon);
  EL::StatusCode toolInitializationAtTheFirstEvent (const xAOD::EventInfo* eventInfo);

  // tools
  CP::EgammaCalibrationAndSmearingTool* m_EgammaCalibrationAndSmearingTool; //!
  CP::IsolationCorrectionTool*          m_IsolationCorrectionTool; //!
  ElectronPhotonShowerShapeFudgeTool*   m_photonFudgeMCTool; //!
  AsgPhotonIsEMSelector*                m_photonTightIsEMSelector; //!
  AsgPhotonIsEMSelector*                m_photonMediumIsEMSelector; //!
  AsgPhotonIsEMSelector*                m_photonLooseIsEMSelector; //!
  AsgPhotonEfficiencyCorrectionTool*    m_photonTightEffTool; //!
  AsgPhotonEfficiencyCorrectionTool*    m_photonMediumEffTool; //!
  AsgPhotonEfficiencyCorrectionTool*    m_photonLooseEffTool; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  PhotonCalibrator (std::string className = "PhotonCalibrator");

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
