#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

#include <xAODAnaHelpers/TreeReader.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

// for splitString
#include <xAODAnaHelpers/JetTriggerEfficiencies.h>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TKey.h"
#include "TLorentzVector.h"
#include "TEnv.h"
#include "TSystem.h"

#include <math.h>
#include <utility>      
#include <iostream>
#include <fstream>
#include <map>

// for adding to tstore?
#include "xAODJet/JetContainer.h"
#include "xAODCore/AuxContainerBase.h"
// #include "xAODJet/versions/Jet_v1.h"
// #include <IParticle.h>
// #include "xAODBase/IParticle.h"
#include <xAODAnaHelpers/JetContainer.h>

using namespace std;


// global variables
std::vector< JetInfo > global_jetCollectionInfos;
EventInfo global_eventInfo;

// this is needed to distribute the algorithm to the workers
ClassImp(TreeReader)


// maybe include the Algorithm() part, maybe not...
TreeReader :: TreeReader () :
  Algorithm("TreeReader")
{
}



EL::StatusCode TreeReader :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TreeReader :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.  

  ANA_MSG_INFO("this is histInitialize");

  // given m_jetBranchNames and m_jetContainerNames, initialise:
  m_NTUPjetContainerNames = splitString(m_jetBranchNames, " ");
  m_xAODjetContainerNames = splitString(m_jetContainerNames, " ");

  // check they're the same size
  if(m_NTUPjetContainerNames.size() != m_xAODjetContainerNames.size()) {
    ANA_MSG_ERROR("different number of jet container names for NTUP and xAOD");
    std::cout << "NTUP:" << std::endl;
    for(auto NTUPjetContainer : m_NTUPjetContainerNames) {
      std::cout << "  \"" << NTUPjetContainer << "\"" << std::endl;
    }
    std::cout << "xAOD:" << std::endl;
    for(auto xAODjetContainer : m_xAODjetContainerNames) {
      std::cout << "  \"" << xAODjetContainer << "\"" << std::endl;
    }
    return EL::StatusCode::FAILURE;
  }

  // fill the vector of JetInfo with the NTUP and xAOD names
  for(unsigned int i = 0; i < m_xAODjetContainerNames.size(); i++) {
    JetInfo jetCollectionInfo;
    jetCollectionInfo.NTUPname = m_NTUPjetContainerNames.at(i);
    jetCollectionInfo.xAODname = m_xAODjetContainerNames.at(i);
    m_jetCollectionInfos.push_back(jetCollectionInfo);
    std::cout << "  filled jet collection info with " << m_NTUPjetContainerNames.at(i) << ", " << m_xAODjetContainerNames.at(i) << std::endl;
  }


  ANA_MSG_DEBUG("returning SUCCESS from histInitialize()");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TreeReader :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  ANA_MSG_DEBUG("calling fileExecute()");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TreeReader :: changeInput (bool firstFile)
{

  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  ANA_MSG_INFO("calling changeInput()");


  TTree* tree = wk()->tree();

  // get all branches to fill m_branchesInTree, used by SetBranchStatusAndAddress
  if(firstFile) {
    for(int ob=0; ob < tree->GetListOfBranches()->GetEntries(); ob++ ) {
      TBranch* br = (TBranch*)tree->GetListOfBranches()->At(ob);
      m_branchesInTree.push_back(br->GetName());
    }
  }

  // turn off all branches, ones I want to be turned on
  tree->SetBranchStatus ("*", 0);

  //event-level variables
  this->SetBranchStatusAndAddress(tree, "runNumber",         m_eventInfo.runNumber);
  this->SetBranchStatusAndAddress(tree, "eventNumber",       m_eventInfo.eventNumber);
  this->SetBranchStatusAndAddress(tree, "lumiBlock",         m_eventInfo.lumiBlock);

  this->SetBranchStatusAndAddress(tree, "bcid",              m_eventInfo.bcid);
  // this->SetBranchStatusAndAddress(tree, "timeStamp",         m_eventInfo.timeStamp);
  // this->SetBranchStatusAndAddress(tree, "timeStampNSOffset", m_eventInfo.timeStampNSOffset);
  // this->SetBranchStatusAndAddress(tree, "LArError",          m_eventInfo.LArError);
  // this->SetBranchStatusAndAddress(tree, "LArFlags",          m_eventInfo.LArFlags);

  this->SetBranchStatusAndAddress(tree, "NPV",                            m_eventInfo.NPV);
  this->SetBranchStatusAndAddress(tree, "averageInteractionsPerCrossing", m_eventInfo.avgIntPerX);
  this->SetBranchStatusAndAddress(tree, "actualInteractionsPerCrossing",  m_eventInfo.actIntPerX);
  this->SetBranchStatusAndAddress(tree, "correct_mu",                     m_eventInfo.correct_mu);
  this->SetBranchStatusAndAddress(tree, "weight_pileup",                  m_eventInfo.pileupWeight);

  this->SetBranchStatusAndAddress(tree, "passedTriggers",   m_eventInfo.passedTriggers);
  this->SetBranchStatusAndAddress(tree, "triggerPrescales", m_eventInfo.triggerPrescales);
  this->SetBranchStatusAndAddress(tree, "isPassBitsNames",  m_eventInfo.isPassBitsNames);
  this->SetBranchStatusAndAddress(tree, "isPassBits",       m_eventInfo.isPassBits);


  for(unsigned int i = 0; i < m_jetCollectionInfos.size(); i++) {
    std::string jetCollectionName = m_jetCollectionInfos.at(i).NTUPname;
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_pt").c_str(), m_jetCollectionInfos.at(i).pt);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_eta").c_str(), m_jetCollectionInfos.at(i).eta);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_phi").c_str(), m_jetCollectionInfos.at(i).phi);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_E").c_str(), m_jetCollectionInfos.at(i).E);

    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_EMFrac").c_str(), m_jetCollectionInfos.at(i).EMFrac);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_HECFrac").c_str(), m_jetCollectionInfos.at(i).HECFrac);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_GhostMuonSegmentCount").c_str(), m_jetCollectionInfos.at(i).muonSegments);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_clean_passLooseBad").c_str(), m_jetCollectionInfos.at(i).clean_passLooseBad);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_Timing").c_str(), m_jetCollectionInfos.at(i).timing);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_NegativeE").c_str(), m_jetCollectionInfos.at(i).negativeE);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_LArQuality").c_str(), m_jetCollectionInfos.at(i).LArQuality);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_AverageLArQF").c_str(), m_jetCollectionInfos.at(i).AverageLArQF);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_HECQuality").c_str(), m_jetCollectionInfos.at(i).HECQuality);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_FracSamplingMax").c_str(), m_jetCollectionInfos.at(i).FracSamplingMax);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_FracSamplingMaxIndex").c_str(), m_jetCollectionInfos.at(i).FracSamplingMaxIndex);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_EnergyPerSampling").c_str(), m_jetCollectionInfos.at(i).EnergyPerSampling);

    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_LeadingClusterPt").c_str(), m_jetCollectionInfos.at(i).LeadingClusterPt);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_LeadingClusterSecondLambda").c_str(), m_jetCollectionInfos.at(i).LeadingClusterSecondLambda);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_LeadingClusterCenterLambda").c_str(), m_jetCollectionInfos.at(i).LeadingClusterCenterLambda);
    // this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_LeadingClusterSecondR").c_str(), m_jetCollectionInfos.at(i).LeadingClusterSecondR);
    
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_emScalePt").c_str(), m_jetCollectionInfos.at(i).emScalePt);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_constScalePt").c_str(), m_jetCollectionInfos.at(i).constScalePt);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_pileupScalePt").c_str(), m_jetCollectionInfos.at(i).pileupScalePt);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_originConstitScalePt").c_str(), m_jetCollectionInfos.at(i).originConstitScalePt);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_etaJESScalePt").c_str(), m_jetCollectionInfos.at(i).etaJESScalePt);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_gscScalePt").c_str(), m_jetCollectionInfos.at(i).gscScalePt);
    this->SetBranchStatusAndAddress(tree, (jetCollectionName+"_insituScalePt").c_str(), m_jetCollectionInfos.at(i).insituScalePt);

  }


  
  if(firstFile) {
    cout << "\nlist of activated branches:" << endl;
    for(int ob=0; ob < tree->GetListOfBranches()->GetEntries(); ob++ ) {
      TBranch* br = (TBranch*)tree->GetListOfBranches()->At(ob);
      TString brname = br->GetName();
      if ( tree->GetBranchStatus(brname) == 1 )
	cout << brname << endl;
    }
    cout << endl;
  }

  ANA_MSG_INFO("returning from changeInput()");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TreeReader :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.


  Info("initialize()", "Calling initialize \n");
  // m_event = wk()->xaodEvent();
  // m_store = wk()->xaodStore();

  // Info("initialize()", "Succesfully initialized! \n");


  return EL::StatusCode::SUCCESS;
}


EL::StatusCode TreeReader :: execute ()
{
  // Here you do everything that needs to be done on every single
  // event, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ++m_eventCounter;

  ANA_MSG_DEBUG("about to get entry " << wk()->treeEntry() << " of tree");
  wk()->tree()->GetEntry (wk()->treeEntry());
  ANA_MSG_DEBUG("entry got successfully");


  // xAOD::TEvent* event = wk()->xaodEvent();
  // ANA_MSG_INFO("got xaodEvent");
  // xAOD::TStore* store = wk()->xaodStore();
  // ANA_MSG_INFO("got xaodStore");


  if(m_eventCounter % 10000 == 0)
    ANA_MSG_INFO("executing event " << m_eventCounter);
  else
    ANA_MSG_DEBUG("executing event " << m_eventCounter);

  ANA_MSG_DEBUG("this event has runNum, LB, evtNum = " << m_eventInfo.runNumber << ", " << m_eventInfo.lumiBlock << ", " << m_eventInfo.eventNumber);
  
  
    
  // https://gitlab.cern.ch/atlas/athena/blob/21.2/Event/xAOD/xAODJet/Root/Jet_v1.cxx#L184
  for(unsigned int i = 0; i < m_jetCollectionInfos.size(); i++) {
    // ANA_MSG_DEBUG("setting TLV for " << m_jetCollectionInfos.at(i).NTUPname);
    // m_jetCollectionInfos.at(i).setTLV();
    ANA_MSG_DEBUG("there are " << m_jetCollectionInfos.at(i).pt->size() << " " << m_jetCollectionInfos.at(i).NTUPname << " jets");
    

    /*
    xAOD::JetContainer* goodJets = new xAOD::JetContainer();
    xAOD::AuxContainerBase* goodJetsAux = new xAOD::AuxContainerBase();
    goodJets->setStore( goodJetsAux ); //< Connect the two

    for(unsigned int i = 0; i < jetInfo.pt->size(); i++) {

        xAOD::Jet* jet = new xAOD::Jet();
        goodJets->push_back( jet ); // jet acquires the goodJets auxstore
        // jet->setJetP4(jetInfo.tlv->at(i));
        // jet->setPxPyPzE(jetInfo.tlv->at(i).Px(), jetInfo.tlv->at(i).Py(), jetInfo.tlv->at(i).Pz(), jetInfo.tlv->at(i).E());
        // IParticle::FourMom_t fourMom;
        xAOD::JetFourMom_t fourMom;
        // const xAOD::Jet::JetFourMom_t fourMom;
        fourMom.SetPxPyPzE(jetInfo.tlv->at(i).Px(), jetInfo.tlv->at(i).Py(), jetInfo.tlv->at(i).Pz(), jetInfo.tlv->at(i).E());
        jet->setJetP4(fourMom);

        // now to add any/all aux data I want???
        // ->auxdata< int >( "mySignal" ) = 1
      }

    ANA_CHECK(m_store->record( goodJets, jetInfo.xAODname ));
    ANA_CHECK(m_store->record( goodJetsAux, jetInfo.xAODname+"Aux." ));
    // of course this doesn't work
    */

  }

  // set global variables for use by other algorithms
  global_jetCollectionInfos = m_jetCollectionInfos;
  global_eventInfo = m_eventInfo;
  
  ANA_MSG_DEBUG("done with execute");
  return EL::StatusCode::SUCCESS;
}







EL::StatusCode TreeReader :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TreeReader :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  //this makes ROOT angry
  //delete m_dataForLArEventVeto;
    
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TreeReader :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.


  cout << endl;
  // event counter initialised to -1, after one event processed it reads 0
  std::cout << "this is histFinalize: this job processed " << m_eventCounter + 1 << " events" << endl;


  return EL::StatusCode::SUCCESS;
}



// in JetTriggerEfficiencies
// std::vector<std::string> splitString(std::string parentString, std::string sep) {
  // std::size_t start = 0, end = 0;
  // std::vector<std::string> splitVec;
  // while ((end = parentString.find(sep, start)) != std::string::npos) {
    // splitVec.push_back(parentString.substr(start, end - start));
    // start = end + sep.size();
  // }
  // splitVec.push_back(parentString.substr(start));
  // return splitVec;
// }



// overloaded functions to set branch status and branch address, for all relevant types
// there is probably a better way to do this 

// need c++14 for auto in function declaration :-(
// void SetBranchStatusAndAddress(TTree* &tree, std::string branchName, auto &m_var) {
  // if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    // tree->SetBranchStatus(branchName.c_str(), 1);
    // tree->SetBranchAddress(branchName.c_str(), &m_var);
  // }
// }
void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, bool &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " << branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, int &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, float &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, Long64_t &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, uint32_t &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<int>* &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<unsigned int>* &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<float>* &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}

 void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector<std::string>* &m_var) {
   if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
     tree->SetBranchStatus(branchName.c_str(), 1);
     tree->SetBranchAddress(branchName.c_str(), &m_var);
   }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
 }

void TreeReader::SetBranchStatusAndAddress(TTree* &tree, std::string branchName, std::vector< std::vector<float> >* &m_var) {
  if(std::find(m_branchesInTree.begin(), m_branchesInTree.end(), branchName) != m_branchesInTree.end()) {
    tree->SetBranchStatus(branchName.c_str(), 1);
    tree->SetBranchAddress(branchName.c_str(), &m_var);
  }
  else {
    std::cout << "not initialising " <<	branchName << " since it's not in the tree" << std::endl;
    m_var = NULL;
  }
}
