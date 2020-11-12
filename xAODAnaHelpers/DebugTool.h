#ifndef xAODAnaHelpers_DebugTool_H
#define xAODAnaHelpers_DebugTool_H


// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class DebugTool : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  // configuration variables

  bool m_printStore = false;

public:

  // this is a standard constructor
  DebugTool (const std::string& name, ISvcLocator *pSvcLocator);

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
  ClassDef(DebugTool, 1);
  /// @endcond
};

#endif
