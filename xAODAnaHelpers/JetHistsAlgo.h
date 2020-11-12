#ifndef xAODAnaHelpers_JetHistsAlgo_H
#define xAODAnaHelpers_JetHistsAlgo_H

#include "xAODAnaHelpers/IParticleHistsAlgo.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class JetHistsAlgo : public IParticleHistsAlgo
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the )
public:
  // Tree *myTree; 
  // TH1 *myHist; 

  // this is a standard constructor
  JetHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator);

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode execute ();

  // these are the functions not inherited from Algorithm
  EL::StatusCode AddHists( std::string name );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetHistsAlgo, 1);
  /// @endcond

};

#endif
