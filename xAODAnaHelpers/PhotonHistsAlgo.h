#ifndef xAODAnaHelpers_PhotonHistsAlgo_H
#define xAODAnaHelpers_PhotonHistsAlgo_H

#include "xAODAnaHelpers/IParticleHistsAlgo.h"

class PhotonHistsAlgo : public IParticleHistsAlgo
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
  PhotonHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator);

  // these are the functions inherited from Algorithm
  virtual StatusCode execute ();

  // these are the functions not inherited from Algorithm
  virtual StatusCode AddHists( std::string name );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  /// @endcond

};

#endif
