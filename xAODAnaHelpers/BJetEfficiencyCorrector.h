#ifndef xAODAnaHelpers_BJetEfficiencyCorrector_H
#define xAODAnaHelpers_BJetEfficiencyCorrector_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// external tools include(s):
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class BJetEfficiencyCorrector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  std::string m_inContainerName;
  std::string m_inputAlgo;

  // systematics
  bool m_runAllSyst;
  std::string m_systName;
  std::string m_outputSystName;

  std::string m_corrFileName;
  std::string m_jetAuthor;
  std::string m_taggerName;
  bool        m_useDevelopmentFile;
  bool        m_coneFlavourLabel;

  std::string m_operatingPt;      // Operating point.
  std::string m_operatingPtCDI;   // the one CDI will understand
  std::string m_decor;            // The decoration key written to passing objects
  std::string m_decorSF;          // The decoration key written to passing objects

private:

  bool m_isMC;        //!

  // tools
  BTaggingSelectionTool   *m_BJetSelectTool; //!
  BTaggingEfficiencyTool  *m_BJetEffSFTool; //!

  // configuration variables
  bool m_getScaleFactors;  //!

  std::vector<CP::SystematicSet> m_systList; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:

  // this is a standard constructor
  BJetEfficiencyCorrector (std::string className = "BJetEfficiencyCorrector");

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  EL::StatusCode executeEfficiencyCorrection(const xAOD::JetContainer* inJets,
					     const xAOD::EventInfo* eventInfo,
					     bool doNominal);
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(BJetEfficiencyCorrector, 1);
  /// @endcond

};

#endif
