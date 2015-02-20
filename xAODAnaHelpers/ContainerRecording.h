#ifndef xAODAnaHelpers_ContainerRecording_H
#define xAODAnaHelpers_ContainerRecording_H

#include <EventLoop/Algorithm.h>
// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

class ContainerRecording : public EL::Algorithm
{
public:

  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

  // this is a standard constructor
  ContainerRecording ();

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

  // this is needed to distribute the algorithm to the workers
  ClassDef(ContainerRecording, 1);
};

#endif
