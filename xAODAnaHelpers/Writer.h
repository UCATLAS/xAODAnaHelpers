#ifndef xAODAnaHelpers_Writer_H
#define xAODAnaHelpers_Writer_H

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class Writer : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  TString m_outputStreamName = "";

  TString m_jetContainerNamesStr = "";
  TString m_electronContainerNamesStr = "";
  TString m_muonContainerNamesStr = "";

private:
  int m_numEvent;

  std::vector<TString> m_jetContainerNames;
  std::vector<TString> m_electronContainerNames;
  std::vector<TString> m_muonContainerNames;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the )
public:
  // Tree *myTree;
  // TH1 *myHist;

  // this is a standard constructor
  Writer (const std::string& name, ISvcLocator *pSvcLocator);

  // these are the functions inherited from Algorithm
  virtual StatusCode fileExecute ();
  virtual StatusCode histInitialize ();
  virtual StatusCode changeInput (bool firstFile);
  virtual StatusCode initialize ();
  virtual StatusCode execute ();
  virtual StatusCode finalize ();
  virtual StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
};

#endif
