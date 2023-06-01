#ifndef XAODANAHELPERS_ISOCLOSEBYCORR_H
#define XAODANAHELPERS_ISOCLOSEBYCORR_H

// EDM include(s):
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTau/TauJetContainer.h"

// external tools include(s):
#include "AssociationUtils/OverlapRemovalInit.h"
#include "AssociationUtils/OverlapRemovalTool.h"
#include "AssociationUtils/ToolBox.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// ROOT include(s):
#include "TH1D.h"

#include "IsolationSelection/IsolationSelectionTool.h"
#include "IsolationSelection/IsolationCloseByCorrectionTool.h"

class IsoCloseByCorr : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:


  /** @brief Decorate selected objects (the default decoration string is `passOR`) */
  bool     m_decorateSelectedObjects;
  std::string m_decor = "passOR";

  // Electrons
  std::string  m_inContainerName_Electrons = "";
  std::string  m_inputAlgoElectrons = "";
  std::string  m_el_iso_WP = "";
  // Muons
  std::string  m_inContainerName_Muons = "";
  std::string  m_outContainerName_Muons = "";
  std::string  m_mu_iso_WP = "";
  // Photons
  bool m_doPhotons = false;
  std::string  m_inContainerName_Photons = "";
  std::string  m_outContainerName_Photons = "";

protected:
  int dummyVaraibleToKeepExample; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

private:
  asg::AnaToolHandle<CP::IsolationSelectionTool> m_IsoSelCloseBy_tool; //!
  std::string m_IsoSelToolCloseBy_name; //!

  asg::AnaToolHandle<CP::IsolationCloseByCorrectionTool> m_IsoCloseByCorr_tool; //!
  std::string m_IsoCloseByCorr_tool_name; //!

public:

  // this is a standard constructor
  IsoCloseByCorr ();

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

  // this is needed to distribute the algorithm to the workers
  ClassDef(IsoCloseByCorr, 1);

};

#endif