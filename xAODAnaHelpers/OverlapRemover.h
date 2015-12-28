/**
 * @file OverlapRemover.h
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @brief |xAH| algorithm to perform overlap removal between reconstructed physics objects.
 */

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

  /** @brief Fill the cutflow histogram for object (electrons, jets, ... ) counting */
  bool m_useCutFlow;

  /** @brief Decorate selected objects (the default decoration string is `passSel`)*/
  bool     m_decorateSelectedObjects;
  /**
     @rst
        Make a copy of input container(s) with selected objects (using :cpp:any:`SG::VIEW_ELEMENTS` to be light weight)
     @endrst
  */
  bool     m_createSelectedContainers;
  /** @brief In the OLR, consider only objects passing a (pre)selection */
  bool     m_useSelected;

  /** @brief Input container name */
  std::string  m_inContainerName_Electrons;
  /** @brief Output container name */
  std::string  m_outContainerName_Electrons;
  /**
     @brief
     @rst
        Name of the :cpp:type:`std::vector<std::string>` of systematics coming from the upstream algorithm
     @endrst
  */
  std::string  m_inputAlgoElectrons;
  /**
     @brief
     @rst
        Name of the :cpp:type:`std::vector<std::string>` of systematics to be considered by the algos downstream
     @endrst
  */
  std::string  m_outputAlgoElectrons;

  /* Muons */
  std::string  m_inContainerName_Muons;
  std::string  m_outContainerName_Muons;
  std::string  m_inputAlgoMuons;
  std::string  m_outputAlgoMuons;
  /* Jets */
  std::string  m_inContainerName_Jets;
  std::string  m_outContainerName_Jets;
  std::string  m_inputAlgoJets;
  std::string  m_outputAlgoJets;
  /* Photons */
  std::string  m_inContainerName_Photons;
  std::string  m_outContainerName_Photons;
  std::string  m_inputAlgoPhotons;
  std::string  m_outputAlgoPhotons;
  /* Taus */
  std::string  m_inContainerName_Taus;
  std::string  m_outContainerName_Taus;
  std::string  m_inputAlgoTaus;
  std::string  m_outputAlgoTaus;

private:

  /** @brief A counter for the number of processed events */
  int m_numEvent;           //!
  /** @brief A counter for the number of processed objects */
  int m_numObject;          //!
  /** @brief A counter for the number of passed events */
  int m_numEventPass;       //!
  /** @brief A counter for the number of passed *weighted* events */
  int m_weightNumEventPass; //!
  /** @brief A counter for the number of passed objects */
  int m_numObjectPass;      //!

  /**
     @brief Consider electrons in the OLR
     @rst
       This is set to :cpp:any:`false` if :cpp:member:`~m_inContainerName_Electrons` is set as an empty string.
       Electrons (unlike jets) are considered "optional" objetcs in the OLR.
     @endrst
  */
  bool m_useElectrons;
  /**
     @brief Consider muons in the OLR
     @rst
       This is set to :cpp:any:`false` if :cpp:member:`~m_inContainerName_Muons` is set as an empty string.
       Muons (unlike jets) are considered "optional" objects in the OLR.
     @endrst
  */
  bool m_useMuons;
  /**
     @brief Consider photons in the OLR
     @rst
       This is set to :cpp:any:`false` if :cpp:member:`~m_inContainerName_Photons` is set as an empty string.
       Photons (unlike jets) are considered "optional" objects in the OLR.
     @endrst
  */
  bool m_usePhotons;
  /**
     @brief Consider taus in the OLR
     @rst
       This is set to :cpp:any:`false` if :cpp:member:`~m_inContainerName_Taus` is set as an empty string.
       Taus (unlike jets) are considered "optional" objects in the OLR.
     @endrst
  */
  bool m_useTaus;

  /**
     @rst
       If :cpp:member:`m_useElectrons=false`, a dummy input electron contaner is passed as input to prevent the code from crashing, since electrons were considered as "default" objects in the original OLR logic implementation.
     @endrst
  */
  const xAOD::ElectronContainer* m_dummyElectronContainer; //!
  /**
     @rst
       If :cpp:member:`m_useMuons=false`, a dummy input electron contaner is passed as input to prevent the code from crashing, since muons were considered as "default" objects in the original OLR logic implementation.
     @endrst
  */
  const xAOD::MuonContainer*     m_dummyMuonContainer;     //!

  /** @brief Output auxiliary container name */
  std::string  m_outAuxContainerName_Electrons;
  /** @brief Output auxiliary container name */
  std::string  m_outAuxContainerName_Muons;
  /** @brief Output auxiliary container name */
  std::string  m_outAuxContainerName_Jets;
  /** @brief Output auxiliary container name */
  std::string  m_outAuxContainerName_Photons;
  /** @brief Output auxiliary container name */
  std::string  m_outAuxContainerName_Taus;

  // tools
 protected:
  /** @brief Pointer to the CP Tool which performs the actual OLR. */
  OverlapRemovalTool *m_overlapRemovalTool; //!

 private:

  /** @brief An enum encoding systematics according to the various objects */
  enum SystType {
    NOMINAL = 0,
    ELSYST  = 1,
    MUSYST  = 2,
    JETSYST = 3,
    PHSYST  = 4,
    TAUSYST = 5,
  };

  // object-level cutflow

  /**  @brief Pointer to the histogram for the electron cutflow */
  TH1D* m_el_cutflowHist_1;    //!
  /**  @brief Pointer to the histogram for the muon cutflow */
  TH1D* m_mu_cutflowHist_1;    //!
  /**  @brief Pointer to the histogram for the jet cutflow */
  TH1D* m_jet_cutflowHist_1;   //!
  /**  @brief Pointer to the histogram for the photon cutflow */
  TH1D* m_ph_cutflowHist_1;    //!
  /**  @brief Pointer to the histogram for the tau cutflow */
  TH1D* m_tau_cutflowHist_1;   //!

  int m_el_cutflow_OR_cut;     //!
  int m_mu_cutflow_OR_cut;     //!
  int m_jet_cutflow_OR_cut;    //!
  int m_ph_cutflow_OR_cut;     //!
  int m_tau_cutflow_OR_cut;    //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

public:

  // this is a standard constructor
  /**
     @brief Initialization
     @param className    This is the name of the class that inherits from
        @rst
          :cpp:class:`~xAH::Algorithm`
	@endrst
  */
  OverlapRemover (std::string className = "OverlapRemover");

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
  /**
     @brief Fill the cutflow histograms
     @param objCont          The `xAOD` container to be considered
     @param overlapFlag      The string identifying objects overlapping with another object, to be removed (default is `overlaps`)
     @param selectFlag       The string identifying selected objects (default is `passSel`)
  */
  virtual EL::StatusCode fillObjectCutflow (const xAOD::IParticleContainer* objCont, const std::string& overlapFlag="overlaps", const std::string& selectFlag="passSel");

  /**
     @brief Function that internally calls the OLR tool for the input containers (and systematics)
     @param inElectrons    Input `xAOD` container for electrons
     @param inMuons        Input `xAOD` container for muons
     @param inJets         Input `xAOD` container for jets
     @param inPhotons      Input `xAOD` container for photons
     @param inTaus         Input `xAOD` container for taus
     @param syst_type      The type of object for which input systematics should be considered. Default is `NOMINAL`
     @param sysVec         The list of the input systematics for a given object. Must match with the choice of `syst_type`. Default is `nullptr`
  */
  virtual EL::StatusCode executeOR( const xAOD::ElectronContainer* inElectrons, const xAOD::MuonContainer* inMuons, const xAOD::JetContainer* inJets,
				    const xAOD::PhotonContainer* inPhotons,	const xAOD::TauJetContainer* inTaus,
				    SystType syst_type = NOMINAL, std::vector<std::string>* sysVec = nullptr);

  /** @brief Setup cutflow histogrmas */
  EL::StatusCode setCutFlowHist();
  /**@brief Initialise counters for events/objects */
  EL::StatusCode setCounters();

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(OverlapRemover, 1);
  /// @endcond
};

#endif
