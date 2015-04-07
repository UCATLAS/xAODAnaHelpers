#ifndef XAODANAHELPERS_OVERLAPREMOVER_H
#define XAODANAHELPERS_OVERLAPREMOVER_H

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

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

class OverlapRemover : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;    //!

  std::string m_name;
  std::string m_configName;
  bool m_debug;

private:

  bool m_usePhotons;
  bool m_useTaus;

  // tools
  OverlapRemovalTool *m_overlapRemovalTool; //!

  // configuration variables

  bool     m_decorateSelectedObjects;  // decorate selected objects? default passSel
  bool     m_createSelectedContainers; // fill using SG::VIEW_ELEMENTS to be light weight

  bool     m_useSelected; // pass only object passing selection to O.R. tool

  /* Electrons */
  std::string  m_inContainerName_Electrons;
  std::string  m_outContainerName_Electrons;        // output container name
  std::string  m_outAuxContainerName_Electrons;     // output auxiliary container name
  std::string  m_inputAlgoElectrons;                // name of vector<string> of syst retrieved from TStore
  std::string  m_outputAlgoElectrons;               // name of vector<string> of syst pushed in TStore
  /* Muons */
  std::string m_inContainerName_Muons;
  std::string  m_outContainerName_Muons;        // output container name
  std::string  m_outAuxContainerName_Muons;     // output auxiliary container name
  std::string  m_inputAlgoMuons;                // name of vector<string> of syst retrieved from TStore
  std::string  m_outputAlgoMuons;               // name of vector<string> of syst pushed in TStore
  /* Jets */
  std::string m_inContainerName_Jets;
  std::string  m_outContainerName_Jets;        // output container name
  std::string  m_outAuxContainerName_Jets;     // output auxiliary container name
  std::string  m_inputAlgoJets;                // name of vector<string> of syst retrieved from TStore
  std::string  m_outputAlgoJets;               // name of vector<string> of syst pushed in TStore
  /* Photons */
  std::string m_inContainerName_Photons;
  std::string  m_outContainerName_Photons;        // output container name
  std::string  m_outAuxContainerName_Photons;     // output auxiliary container name
  /* Taus */
  std::string m_inContainerName_Taus;
  std::string  m_outContainerName_Taus;        // output container name
  std::string  m_outAuxContainerName_Taus;     // output auxiliary container name

  enum SystType {
    NOMINAL = 0,
    ELSYST  = 1,
    MUSYST  = 2,
    JETSYST = 3,
    PHSYST  = 4,
    TAUSYST = 5,
  };
  
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!


  // this is a standard constructor
  OverlapRemover ();
  OverlapRemover (std::string name, std::string configName);

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
  virtual EL::StatusCode printOverlapInfo (const char* type, const xAOD::IParticleContainer* objCont, const std::string& selectFlag, const std::string& overlapFlag);
  virtual EL::StatusCode printOverlapInfo (const char* type, xAOD::IParticle* obj, const std::string& selectFlag, const std::string& overlapFlag);
  
  virtual EL::StatusCode executeOR( const xAOD::ElectronContainer* inElectrons, const xAOD::MuonContainer* inMuons, const xAOD::JetContainer* inJets, 
				    const xAOD::PhotonContainer* inPhotons,	const xAOD::TauJetContainer* inTaus,
				    ConstDataVector<xAOD::ElectronContainer> *selectedElectrons, ConstDataVector<xAOD::MuonContainer> *selectedMuons, ConstDataVector<xAOD::JetContainer> *selectedJets,
				    ConstDataVector<xAOD::PhotonContainer>   *selectedPhotons, ConstDataVector<xAOD::TauJetContainer>   *selectedTaus,
				    SystType syst_type = NOMINAL, std::vector<std::string>* sysVec = nullptr);
					     
  // this is needed to distribute the algorithm to the workers
  ClassDef(OverlapRemover, 1);
};

#endif
