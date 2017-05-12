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
  std::string  m_inContainerName;
  /**
      @brief The name of the nominal output container written by the algorithm to ``TStore``

      If the algorithm applies systematic variations, for each shallow copy saved to ``TStore``, the systematic name will be appended to this.
  */
  std::string  m_outContainerName;

// systematics
  /**
    @brief The name of the vector containing the names of the systematically-varied containers from the upstream algorithm, which will be processed by this algorithm.

    This vector is retrieved from the ``TStore``. If left blank, it means there is no upstream algorithm which applies systematics. This is the case when processing straight from the original ``xAOD`` or ``DxAOD``.
  */
  std::string m_inputAlgoSystNames;

  /**
    @brief The name of the vector containing the names of the systematically-varied containers created by by this algorithm.

    @rst
      If :cpp:member:`~xAH::Algorithm::m_systName` is empty, the vector will contain only an empty string. When running on systematics, this is the string a downstream algorithm needs to process electrons.

      .. note:: We need this as we deep-copy the input containers.
    @endrst
  */
  std::string   m_outputAlgoSystNames;

  /** @brief Force AFII flag in calibration, in case metadata is broken */
  bool m_setAFII;

  float m_systValPID;
  float m_systValIso;
  float m_systValReco;
  float m_systValTrig;
  float m_systValTrigMCEff;
  std::string m_systNamePID;
  std::string m_systNameIso;
  std::string m_systNameReco;
  std::string m_systNameTrig;
  std::string m_systNameTrigMCEff;
  std::string m_outputSystNamesPID;
  std::string m_outputSystNamesIso;
  std::string m_outputSystNamesReco;
  std::string m_outputSystNamesTrig;
  std::string m_outputSystNamesTrigMCEff;
  std::string m_corrFileNamePID;
  std::string m_corrFileNameIso;
  std::string m_corrFileNameReco;
  std::string m_corrFileNameTrig;
  std::string m_corrFileNameTrigMCEff;
  std::string m_correlationModel;

private:
  int m_numEvent;         //!
  int m_numObject;        //!

  bool m_isMC;            //!

  std::string m_PID_WP;    //!
  std::string m_Iso_WP;    //!
  std::string m_IsoPID_WP; //!

  std::string m_WorkingPointIDTrig;  //!
  std::string m_WorkingPointIsoTrig; //!
  std::string m_WorkingPointTrigTrig; //!

  std::vector<CP::SystematicSet> m_systListPID;  //!
  std::vector<CP::SystematicSet> m_systListIso;  //!
  std::vector<CP::SystematicSet> m_systListReco; //!
  std::vector<CP::SystematicSet> m_systListTrig; //!
  std::vector<CP::SystematicSet> m_systListTrigMCEff; //!

  // tools
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_PID;  //!
  std::string m_pidEffSF_tool_name;                                   //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_Iso;  //!
  std::string m_IsoEffSF_tool_name;                                   //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_Reco; //!
  std::string m_RecoEffSF_tool_name;                                  //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_Trig; //!
  std::string m_TrigEffSF_tool_name;                                  //!
  AsgElectronEfficiencyCorrectionTool  *m_asgElEffCorrTool_elSF_TrigMCEff; //!
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
  virtual EL::StatusCode executeSF ( const xAOD::ElectronContainer* inputElectrons, unsigned int countSyst );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(ElectronEfficiencyCorrector, 1);
  /// @endcond

};

#endif
