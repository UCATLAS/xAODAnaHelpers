#ifndef TLAAlgos_TreeReader_H
#define TLAAlgos_TreeReader_H

#include <EventLoop/StatusCode.h>
#include <EventLoop/Algorithm.h>
#include <EventLoop/Worker.h>


//algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"


// ROOT include(s):
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TTree.h"

#include <sstream>
#include <vector>
#include <map>

#include <string.h>
#include <regex>


using namespace std;


/* std::vector<std::string> splitString(std::string parentString, std::string sep); */
// in JetTriggerEfficiencies


class JetInfo {
 public:
  std::string NTUPname = "";
  std::string xAODname = "";

  vector<float>* pt = nullptr;
  vector<float>* eta = nullptr;
  vector<float>* phi = nullptr;
  vector<float>* E = nullptr;

  vector<float>* muonSegments = nullptr;
  vector<float>* EMFrac = nullptr;
  vector<float>* HECFrac = nullptr;
  vector<float>* timing = nullptr;
  vector<float>* negativeE = nullptr;

  vector<int>*   clean_passLooseBad = nullptr;
		
  vector<float>* LArQuality = nullptr;
  vector<float>* AverageLArQF = nullptr;
  vector<float>* HECQuality = nullptr;
  vector<float>* FracSamplingMax = nullptr;
  vector<int>*   FracSamplingMaxIndex = nullptr;
  vector< vector<float> >* EnergyPerSampling = nullptr;

  vector<float>* LeadingClusterPt = nullptr;
  vector<float>* LeadingClusterSecondLambda = nullptr;
  vector<float>* LeadingClusterCenterLambda = nullptr;
  vector<float>* LeadingClusterSecondR = nullptr;

  // scales
  vector<float>* emScalePt = nullptr;
  vector<float>* constScalePt = nullptr;
  vector<float>* pileupScalePt = nullptr;
  vector<float>* originConstitScalePt = nullptr;
  vector<float>* etaJESScalePt = nullptr;
  vector<float>* gscScalePt = nullptr;
  vector<float>* insituScalePt = nullptr;

};


class TreeReader : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
 public:
  std::string m_name;
  std::string m_jetBranchNames;
  std::string m_jetContainerNames;


 private:

  int m_eventCounter = 0; //!
  
  // event level quantities
  Int_t     m_runNumber; //!
  Long64_t  m_eventNumber; //!
  int       m_lumiBlock; //!

  int       m_bcid; //!

  //uint32_t  m_timeStamp; //!
  //uint32_t  m_timeStampNSOffset; //!
  //bool      m_LArError; //!
  //uint32_t  m_LArFlags; //!
  // coreFlags
  // TileError
  // SCTError
  // TileFlags
  // SCTFlags

  int       m_NPV = -1; //!  
  float     m_avgIntPerX = -1; //!
  float     m_actIntPerX = -1; //!
  float     m_correct_mu = -1; //!
  float     m_pileupWeight = -1; //!

  vector<std::string>*  m_passedTriggers = nullptr; //!
  vector<float>*        m_triggerPrescales = nullptr; //!
  vector<std::string>*  m_isPassBitsNames = nullptr;  //!
  vector<unsigned int>* m_isPassBits = nullptr;  //!



  
  // list of branches in tree
  std::vector<string> m_branchesInTree; //!

  // the jet collections
  std::vector<std::string> m_NTUPjetContainerNames; //!
  std::vector<std::string> m_xAODjetContainerNames; //!
  std::vector< JetInfo >   m_jetCollectionInfos; //!

  
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
 public:
  // Tree *myTree; //!
  // TH1 *myHist; //!
  
  // this is a standard constructor
  TreeReader ();
  
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
  // need c++14 for auto :-(
  /* void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, auto &m_var); */
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, bool &m_var);
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, int &m_var);
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, float &m_var);
  // void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, Int_t &m_var); // Int_t = int
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, Long64_t &m_var);
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, uint32_t &m_var);
  
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<int>* &m_var);
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<unsigned int>* &m_var);
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<float>* &m_var);
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<std::string>* &m_var);
  void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector< std::vector<float> >* &m_var);

  
  
  // this is needed to distribute the algorithm to the workers
  ClassDef(TreeReader, 1);
  
  
};

#endif
