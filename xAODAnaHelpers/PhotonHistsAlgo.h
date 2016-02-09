#ifndef xAODAnaHelpers_PhotonHistsAlgo_H
#define xAODAnaHelpers_PhotonHistsAlgo_H

#include <xAODAnaHelpers/PhotonHists.h>

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class PhotonHistsAlgo : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // configuration variables
  std::string m_inContainerName;
  std::string m_detailStr;
  std::string m_inputAlgo;

private:
  std::map< std::string, PhotonHists* > m_plots; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  PhotonHistsAlgo ();

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
  EL::StatusCode AddHists( std::string name );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(PhotonHistsAlgo, 1);
  /// @endcond

};

#endif
