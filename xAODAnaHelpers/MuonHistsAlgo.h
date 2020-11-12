#ifndef xAODAnaHelpers_MuonHistsAlgo_H
#define xAODAnaHelpers_MuonHistsAlgo_H

#include "xAODAnaHelpers/IParticleHistsAlgo.h"

class MuonHistsAlgo : public IParticleHistsAlgo
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
  MuonHistsAlgo ();

  // these are the functions inherited from Algorithm
  virtual StatusCode setupJob (EL::Job& job);
  virtual StatusCode execute ();

  // these are the functions not inherited from Algorithm
  virtual StatusCode AddHists( std::string name );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MuonHistsAlgo, 1);
  /// @endcond

};

#endif
