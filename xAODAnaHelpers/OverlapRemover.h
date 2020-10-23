/**
 * @file OverlapRemover.h
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @author Jeff Dandoy
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
#include "AssociationUtils/OverlapRemovalInit.h"
#include "AssociationUtils/OverlapRemovalTool.h"
#include "AssociationUtils/ToolBox.h"


// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// ROOT include(s):
#include "TH1D.h"

/**
   @brief A wrapper of the overlap removal tool in the ASG [AssociationUtils](https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/AnalysisCommon/AssociationUtils/trunk/doc/README.rst) package.
   @rst
      The logic of the OLR belongs to the ASG tool itself, and is described extensively in the `Analysis Harmonisation Task Force <https://cds.cern.ch/record/1700874>`_ note.

      If you wish to apply a custom OLR scheme, please contact the author marco.milesi@cern.ch for detailed instructions.

      The idea behind this algorithm is to consistently thread together the inputs from upstream |xAH| algorithms based on user's configuration, handling also the case where systematics on the input physics objects are taken into account. Here follows a usage example.

      Consider the simplified scenario where we care only about **jets** and **electrons**.
      Assuming the typical |xAH| analysis configuration through :py:class:`~xAH_config`, the analysis workflow could look like the following:

      .. code-block:: python

          c = xAH_config()
          # ...
          c.algorithm("JetSelector", JetSelectorDict)
          c.algorithm("ElectronSelector", ElectronSelectorDict)
          # ...
          c.algorithm("OverlapRemover", OverlapRemoverDict)
          # ...

      where each algorithm has the following I/O systematics configuration (via python dictionaries):

      .. code-block:: python

          JetSelectorDict = {
              # ...
              "m_inputAlgo" : "JetCalibrator_Syst",
              "m_outputAlgo" : "JetSelector_Syst",
              # ...
          }

          ElectronSelectorDict = {
              # ...
              "m_inputAlgo" : "ElectronCalibrator_Syst",
              "m_outputAlgo" : "ElectronSelector_Syst",
              # ...
         }

         OverlapRemoverDict = {
              # ...
              "m_inputAlgoJets" : "JetSelector_Syst", # leave empty when not considering jet systematics
              "m_inputAlgoElectrons" : "ElectronSelector_Syst", # leave empty when not considering electron systematics
              # ...
         }

      In this way the overlap removal algorithm will be able to correctly work out all the combinatorics, generating output :cpp:any:`xAOD` containers for jets and electrons for each input systematics combination to be subsequently used downstream according to the user's needs. The overlap removal algorithm creates an output systematic list that is a combination of systematics from all input containers.

   @endrst
 */
class OverlapRemover : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
 public:

  // configuration variables

  /** @brief Fill the cutflow histogram(s) for object counting */
  bool m_useCutFlow = true;

  /** @brief Decorate selected objects (the default decoration string is `passSel`) */
  bool     m_decorateSelectedObjects;
  /**
     @rst
        Make a copy of input container(s) with selected objects (using :cpp:any:`SG::VIEW_ELEMENTS` to be light weight)
     @endrst
  */
  bool     m_createSelectedContainers;
  /** @brief In the OLR, consider only objects passing a (pre)selection */
  bool     m_useSelected = false;
  /** @brief Use b-tagging decision, set previously with the given decoration name, to remove electrons and muons
   * @note This is automatically set by BJetEfficiencyCorrector */
  std::string m_bTagWP = "";
  /** @brief Create a link between overlapped objects */
  bool m_linkOverlapObjects = true;
  /** @brief Use boosted object working point */
  bool m_useBoostedLeptons = false;
  /** @brief Do overlap removal between electrons (HSG2 prescription) */
  bool m_doEleEleOR = false;

  /** @brief Output systematics list container name */
  std::string  m_outputAlgoSystNames = "ORAlgo_Syst";

  /** @brief Input container name */
  std::string  m_inContainerName_Electrons = "";
  /** @brief Output container name */
  std::string  m_outContainerName_Electrons = "";
  /**
     @rst
        Name of the :cpp:type:`std::vector<std::string>` of systematics coming from the upstream algorithm
     @endrst
  */
  std::string  m_inputAlgoElectrons = "";

  // Muons
  std::string  m_inContainerName_Muons = "";
  std::string  m_outContainerName_Muons = "";
  std::string  m_inputAlgoMuons = "";
  // Jets
  std::string  m_inContainerName_Jets = "";
  std::string  m_outContainerName_Jets = "";
  std::string  m_inputAlgoJets = "";
  // Photons
  std::string  m_inContainerName_Photons = "";
  std::string  m_outContainerName_Photons = "";
  std::string  m_inputAlgoPhotons = "";
  // Taus
  std::string  m_inContainerName_Taus = "";
  std::string  m_outContainerName_Taus = "";
  std::string  m_inputAlgoTaus = "";

  /** @brief To remove muons reconstructed as p-flow jets
  https://twiki.cern.ch/twiki/bin/view/AtlasProtected/HowToCleanJetsR21#Muons_Reconstructed_as_Jets_in_P */
  bool m_doMuPFJetOR = false;

 protected:

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
       This is set to ``false`` if :cpp:member:`~m_inContainerName_Electrons` is set as an empty string.
       Electrons (unlike jets) are considered "optional" objetcs in the OLR.
     @endrst
  */
  bool m_useElectrons = false;
  /**
     @brief Consider muons in the OLR
     @rst
       This is set to ``false`` if :cpp:member:`~m_inContainerName_Muons` is set as an empty string.
       Muons (unlike jets) are considered "optional" objects in the OLR.
     @endrst
  */
  bool m_useMuons = false;
  /**
     @brief Consider photons in the OLR
     @rst
       This is set to ``false`` if :cpp:member:`~m_inContainerName_Photons` is set as an empty string.
       Photons (unlike jets) are considered "optional" objects in the OLR.
     @endrst
  */
  bool m_usePhotons = false;
  /**
     @brief Consider taus in the OLR
     @rst
       This is set to ``false`` if :cpp:member:`~m_inContainerName_Taus` is set as an empty string.
       Taus (unlike jets) are considered "optional" objects in the OLR.
     @endrst
  */
  bool m_useTaus = false;

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

  /** @brief Pointer to the CP Tool which performs the actual OLR. */
  ORUtils::ToolBox m_ORToolbox;        //!

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
  TH1D* m_el_cutflowHist_1 = nullptr;    //!
  /**  @brief Pointer to the histogram for the muon cutflow */
  TH1D* m_mu_cutflowHist_1 = nullptr;    //!
  /**  @brief Pointer to the histogram for the jet cutflow */
  TH1D* m_jet_cutflowHist_1 = nullptr;   //!
  /**  @brief Pointer to the histogram for the photon cutflow */
  TH1D* m_ph_cutflowHist_1 = nullptr;    //!
  /**  @brief Pointer to the histogram for the tau cutflow */
  TH1D* m_tau_cutflowHist_1 = nullptr;   //!

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
  /**
     @brief Fill the cutflow histograms
     @param objCont          The `xAOD` container to be considered
     @param overlapFlag      The string identifying objects not overlapping with another object, to be kept (default is `"passOR"`)
     @param selectFlag       The string identifying selected objects (default is `"passSel"`)
  */
  virtual EL::StatusCode fillObjectCutflow (const xAOD::IParticleContainer* objCont,
					    const std::string& overlapFlag = "passOR",
					    const std::string& selectFlag = "passSel");

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
  virtual EL::StatusCode executeOR( const xAOD::ElectronContainer* inElectrons,
				    const xAOD::MuonContainer* inMuons,
				    const xAOD::JetContainer* inJets,
				    const xAOD::PhotonContainer* inPhotons,
				    const xAOD::TauJetContainer* inTaus,
				    SystType syst_type = NOMINAL,
				    std::vector<std::string>* sysVec = nullptr,
            std::vector<std::string>* sysVecOut = nullptr);

  /** @brief Setup cutflow histograms */
  EL::StatusCode setCutFlowHist();
  /** @brief Initialise counters for events/objects */
  EL::StatusCode setCounters();

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(OverlapRemover, 1);
  /// @endcond

};

#endif
