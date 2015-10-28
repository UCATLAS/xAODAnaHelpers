#ifndef XAODANAHELPERS_OVERLAPREMOVER_H
#define XAODANAHELPERS_OVERLAPREMOVER_H

// EDM include(s):
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"

// external tools include(s):
#include "AssociationUtils/OverlapRemovalTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// ROOT include(s):
#include "TH1D.h"

class OverlapRemover : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  
  // configuration variables
  
  bool m_useCutFlow;            

  bool     m_decorateSelectedObjects;  // decorate selected objects? default passSel
  bool     m_createSelectedContainers; // fill using SG::VIEW_ELEMENTS to be light weight
  bool     m_useSelected; // pass only object passing selection to O.R. tool

  /* Electrons */
  std::string  m_inContainerName_Electrons;
  std::string  m_outContainerName_Electrons;        // output container name
  std::string  m_inputAlgoElectrons;                // name of vector<string> of syst retrieved from TStore
  std::string  m_outputAlgoElectrons;               // name of vector<string> of syst pushed in TStore
  /* Muons */
  std::string m_inContainerName_Muons;
  std::string  m_outContainerName_Muons;        // output container name
  std::string  m_inputAlgoMuons;                // name of vector<string> of syst retrieved from TStore
  std::string  m_outputAlgoMuons;               // name of vector<string> of syst pushed in TStore
  /* Jets */
  std::string m_inContainerName_Jets;
  std::string  m_outContainerName_Jets;        // output container name
  std::string  m_inputAlgoJets;                // name of vector<string> of syst retrieved from TStore
  std::string  m_outputAlgoJets;               // name of vector<string> of syst pushed in TStore
  /* Photons */
  std::string m_inContainerName_Photons;
  std::string  m_outContainerName_Photons;        // output container name
  std::string  m_inputAlgoPhotons;                // name of vector<string> of syst retrieved from TStore
  std::string  m_outputAlgoPhotons;               // name of vector<string> of syst pushed in TStore
  /* Taus */
  std::string m_inContainerName_Taus;
  std::string  m_outContainerName_Taus;        // output container name


private:
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;    //!

  bool m_usePhotons;
  bool m_useTaus;

  /* Electrons */
  std::string  m_outAuxContainerName_Electrons;     // output auxiliary container name
  /* Muons */
  std::string  m_outAuxContainerName_Muons;     // output auxiliary container name
  /* Jets */
  std::string  m_outAuxContainerName_Jets;     // output auxiliary container name
  /* Photons */
  std::string  m_outAuxContainerName_Photons;     // output auxiliary container name
  /* Taus */
  std::string  m_outAuxContainerName_Taus;     // output auxiliary container name


  // tools
  OverlapRemovalTool *m_overlapRemovalTool; //!

  enum SystType {
    NOMINAL = 0,
    ELSYST  = 1,
    MUSYST  = 2,
    JETSYST = 3,
    PHSYST  = 4,
    TAUSYST = 5,
  };

  /* object-level cutflow */
  
  TH1D* m_el_cutflowHist_1;    //!
  TH1D* m_mu_cutflowHist_1;    //!
  TH1D* m_jet_cutflowHist_1;   //!
  TH1D* m_ph_cutflowHist_1;   //!

  int m_el_cutflow_OR_cut;     //!
  int m_mu_cutflow_OR_cut;     //!
  int m_jet_cutflow_OR_cut;    //!
  int m_ph_cutflow_OR_cut;    //!
  

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  OverlapRemover ();

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
  virtual EL::StatusCode fillObjectCutflow (const char* type, const xAOD::IParticleContainer* objCont, const std::string& selectFlag, const std::string& overlapFlag);

  virtual EL::StatusCode executeOR( const xAOD::ElectronContainer* inElectrons, const xAOD::MuonContainer* inMuons, const xAOD::JetContainer* inJets,
				    const xAOD::PhotonContainer* inPhotons,	const xAOD::TauJetContainer* inTaus,
				    SystType syst_type = NOMINAL, std::vector<std::string>* sysVec = nullptr);

  // this is needed to distribute the algorithm to the workers
  ClassDef(OverlapRemover, 1);
};

#endif
