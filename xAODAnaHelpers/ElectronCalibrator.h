#ifndef xAODAnaHelpers_ElectronCalibrator_H
#define xAODAnaHelpers_ElectronCalibrator_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicCode.h"

// external tools include(s):
#include "ElectronPhotonFourMomentumCorrection/EgammaCalibrationAndSmearingTool.h"
#include "IsolationCorrections/IsolationCorrectionTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

/**
  @rst
    This is the algorithm class used to calibrate electrons.

    In a nutshell, this algorithm performs the following actions:

      - retrieves an ``xAOD::ElectronContainer`` from either ``TEvent`` or ``TStore``
      - makes a shallow copy container and fills it with energy-and-direction calibrated electrons using the ``EgammaCalibrationAndSmearingTool`` in `Tools Used <ToolsUsed.html>`__
      - saves the shallow copy container to ``TStore`` from where it can be retrieved by algorithms downstream via name lookup

  @endrst
*/
class ElectronCalibrator : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
// configuration variables
  /// @brief The name of the input container for this algorithm to read from ``TEvent`` or ``TStore``
  std::string m_inContainerName = "";
  /**
      @brief The name of the nominal output container written by the algorithm to ``TStore``

      If the algorithm applies systematic variations, for each shallow copy saved to ``TStore``, the systematic name will be appended to this.
  */
  std::string m_outContainerName = "";

  /// Sort the processed container elements by transverse momentum
  bool    m_sort = true;

// systematics
  /**
    @brief The name of the vector containing the names of the systematically-varied containers from the upstream algorithm, which will be processed by this algorithm.

    This vector is retrieved from the ``TStore``. If left blank, it means there is no upstream algorithm which applies systematics. This is the case when processing straight from the original ``xAOD`` or ``DxAOD``.
  */
  std::string m_inputAlgoSystNames = "";

  /**
    @brief The name of the vector containing the names of the systematically-varied containers created by by this algorithm.

    @rst
      If :cpp:member:`~xAH::Algorithm::m_systName` is empty, the vector will contain only an empty string. When running on systematics, this is the string a downstream algorithm needs to process electrons.
    @endrst
  */
  std::string m_outputAlgoSystNames = "ElectronCalibrator_Syst";

  /**
    @brief Write systematics names to metadata
  */
  bool        m_writeSystToMetadata = false;

  std::string m_esModel = "";
  std::string m_decorrelationModel = "";

  /** @brief Apply isolation correction, not needed by default */
  bool m_applyIsolationCorrection = false;

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  std::string m_outAuxContainerName;
  std::string m_outSCContainerName;
  std::string m_outSCAuxContainerName;

  std::vector<CP::SystematicSet> m_systList; //!

  // tools
  CP::EgammaCalibrationAndSmearingTool *m_EgammaCalibrationAndSmearingTool = nullptr; //!
  /// @brief apply leakage correction to calo based isolation variables for electrons
  CP::IsolationCorrectionTool          *m_IsolationCorrectionTool = nullptr;          //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  ElectronCalibrator ();

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
  ClassDef(ElectronCalibrator, 1);
  /// @endcond

};

#endif
