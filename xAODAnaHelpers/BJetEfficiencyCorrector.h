#ifndef xAODAnaHelpers_BJetEfficiencyCorrector_H
#define xAODAnaHelpers_BJetEfficiencyCorrector_H

// CP interface includes
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicSet.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/ISystematicsTool.h"

// external tools include(s):
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class BJetEfficiencyCorrector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  std::string m_inContainerName;

  // systematics
  bool m_runAllSyst;
  std::string m_systName;
  std::string m_outputSystName;

  std::string m_corrFileName;
  std::string m_jetAuthor;
  std::string m_taggerName;
  bool        m_useDevelopmentFile;
  bool        m_coneFlavourLabel;
  bool  m_btag_veryloose;
  bool  m_btag_loose;
  bool  m_btag_medium;
  bool  m_btag_tight;

  std::string m_decor;            // The decoration key written to passing objects

private:

  // tools
  BTaggingEfficiencyTool  *m_BJetEffSFTool; //!

  bool m_isEMjet;                //!
  bool m_isLCjet;                //!

  // configuration variables
  std::string m_operatingPt;

  std::vector<CP::SystematicSet> m_systList; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:

  // this is a standard constructor
  BJetEfficiencyCorrector ();

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

  // this is needed to distribute the algorithm to the workers
  ClassDef(BJetEfficiencyCorrector, 1);

};

#endif
