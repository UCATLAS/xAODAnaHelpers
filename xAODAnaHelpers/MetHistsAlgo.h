#ifndef xAODAnaHelpers_MetHistsAlgo_H
#define xAODAnaHelpers_MetHistsAlgo_H

#include <xAODAnaHelpers/MetHists.h>

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class MetHistsAlgo : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  std::string m_inContainerName = "";

  // configuration variables
  std::string m_detailStr = "";

private:
  MetHists* m_plots = nullptr;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the )
public:
  // Tree *myTree;
  // TH1 *myHist;

  // this is a standard constructor
  MetHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator);

  // these are the functions inherited from Algorithm
  virtual StatusCode fileExecute ();
  virtual StatusCode histInitialize ();
  virtual StatusCode changeInput (bool firstFile);
  virtual StatusCode initialize ();
  virtual StatusCode execute ();
  virtual StatusCode postExecute ();
  virtual StatusCode finalize ();
  virtual StatusCode histFinalize ();

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MetHistsAlgo, 1);
  /// @endcond

};

#endif
