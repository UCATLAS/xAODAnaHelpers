#ifndef xAODAnaHelpers_MinixAOD_H
#define xAODAnaHelpers_MinixAOD_H

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class MinixAOD : public xAH::Algorithm
{
public:

  std::string m_inContainerName;   // input container name
  std::string m_outContainerName;  // output container name

  // this is a standard constructor
  MinixAOD (std::string className = "MinixAOD");

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

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MinixAOD, 1);
  /// @endcond

};

#endif
