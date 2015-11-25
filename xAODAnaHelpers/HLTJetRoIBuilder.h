/********************************************************
 * HLTJetRoIBuilder:
 *
 * This class builds HLT jets and thier associated objects
 *
 * John Alison (john.alison@cern.ch)
 *
 ********************************************************/

#ifndef xAODAnaHelpers_HLTJetRoIBuilder_H
#define xAODAnaHelpers_HLTJetRoIBuilder_H


// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"


namespace Trig {
  class TrigDecisionTool;
}


class HLTJetRoIBuilder : public xAH::Algorithm
{

public:

  // configuration variables
  std::string m_trigItem;
  std::string m_outContainerName;

  // sort after calibration
  bool    m_sort;

private:

  Trig::TrigDecisionTool*      m_trigDecTool;   //!

public:

  // this is a standard constructor
  HLTJetRoIBuilder (std::string className = "HLTJetRoIBuilder");

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
  ClassDef(HLTJetRoIBuilder, 1);
  /// @endcond

};

#endif
