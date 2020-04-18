#ifndef xAODAnaHelpers_HELPERCLASSES_H
#define xAODAnaHelpers_HELPERCLASSES_H

#include <map>
#include <iostream>
#include <sstream>

#include "TString.h"

/* stuff below is for templating getContainer */
#include <RootCoreUtils/ThrowMsg.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include <xAODEventInfo/EventInfo.h>

namespace HelperClasses {

  enum class ContainerType {
      UNKNOWN      = 0,
      CONSTDV      = 1,
      CONSTCONT    = 2,
  };

  enum class ToolName {
      MUONSELECTOR,
      ELECTRONSELECTOR,
      PHOTONSELECTOR,
      JETSELECTOR,
      BJETSELECTOR,
      CALIBRATOR,
      CORRECTOR,
      SELECTOR,
      DEFAULT
  };

  /** template enum parser. Copied from: http://stackoverflow.com/a/726681
   */
  template <typename T>
  class EnumParser
  {
     std::multimap <std::string, T> enumMap;
   public:
     EnumParser();

     T parseEnum(const std::string &value)
     {
        typename std::multimap <std::string, T>::const_iterator iValue = enumMap.find(value);
        if (iValue == enumMap.end()){
            std::cerr << "Could not find input string in enum!" << std::endl;
        }
        return iValue->second;
     }
  };


  /**
    @brief A struct that is used for parsing configuration strings and assigning booleans to various properties. Currently used in plotting code.

    @rst
        Strings are used to turn on and off histograms and branches in the tree The following structs hold the bools used to control the content and also have the string which is necessary to turn a set on.
        See the derived members for more information about what is supported. Each derived member should provide a table of parameters, patterns, and type of matching scheme used. The pattern will use standard PCRE-syntax when appropriate.

        We support two major matching schemes:

            Exact
                If a variable is matched exactly to a string, then a boolean is set to True or False based on whether an exact match exists or not.

            Partial
                If a variable is partially matched to a string, then there is some specific pattern we are extracting that will succeed the partial match that determines what the variable will be set to (usually not a bool).

    @endrst
   */
  class InfoSwitch {
  protected:
    /**
        @brief The input configuration string from which we split up into tokens.
     */
    const std::string m_configStr;
    /**
        @brief The vector of tokens from which we search through for finding matches.
     */
    std::set<std::string> m_configDetails;
  public:
    /**
        @brief Constructor. Take in input string, create vector of tokens.
        @param configStr        The configuration string to split up.
     */
    InfoSwitch(const std::string configStr) : m_configStr(configStr) {
        // parse and split by space
        std::string token;
        std::istringstream ss(m_configStr);
        while ( std::getline(ss, token, ' ') )
            m_configDetails.insert(token);
    };
    /**
        @rst
            Search for an exact match in :cpp:member:`~HelperClasses::InfoSwitch::m_configDetails`.

        @endrst
        @param flag     The string we search for.
     */
    bool has_exact(const std::string flag) { return m_configDetails.find(flag) != m_configDetails.end(); };
    /**
        @rst
            Search for a partial match in :cpp:member:`~HelperClasses::InfoSwitch::m_configStr`.

        @endrst
        @param flag     The string we search for.
     */
    bool has_match(const std::string flag) { return m_configStr.find(flag) != std::string::npos; };
    /**
        @rst
            Search for a single flag in :cpp:member:`~HelperClasses::InfoSwitch::m_configDetails` and parse out the working point.

        @endrst
        @param flag     The string we search for.
     */
    std::string get_working_point(const std::string flag);
    /**
        @rst
            Search for multiple flags in :cpp:member:`~HelperClasses::InfoSwitch::m_configDetails` and parse out the working points.
        @endrst
        @param flag     The string we search for.
     */
    std::vector<std::string> get_working_points(const std::string flag);
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Event Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_eventCleaning  eventCleaning  exact
        m_bcidInfo       bcidInfo       exact
        m_pileup         pileup         exact
        m_pileupsys      pileupsys      exact
        m_shapeEM        shapeEM        exact
        m_shapeEMPFLOW   shapeEMPFLOW   exact
        m_shapeLC        shapeLC        exact
        m_truth          truth          exact
        m_caloClus       caloClusters   exact
        m_weightsSys     weightsSys     exact
        ================ ============== =======

    @endrst
   */
  class EventInfoSwitch : public InfoSwitch {
  public:
    bool m_eventCleaning;
    bool m_bcidInfo;
    bool m_pileup;
    bool m_pileupsys;
    bool m_shapeEM;
    bool m_shapeEMPFLOW;
    bool m_shapeLC;
    bool m_truth;
    bool m_caloClus;
    bool m_weightsSys;
    EventInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  protected:
    void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Trigger Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_basic          basic          exact
        m_menuKeys       menuKeys       exact
        m_passTriggers   passTriggers   exact
        m_passTrigBits   passTrigBits   exact
        m_prescales      prescales      exact
        m_prescalesLumi  prescalesLumi  exact
        ================ ============== =======

        .. note::
            ``m_prescales`` contains information from the ``TrigDecisionTool`` for every trigger used in event selection and event trigger-matching. ``m_prescalesLumi`` contains information retrieved from the pile-up reweighting tool based on the actual luminosities of triggers.

    @endrst
   */
  class TriggerInfoSwitch : public InfoSwitch {
  public:
    bool m_basic;
    bool m_menuKeys;
    bool m_passTriggers;
    bool m_passTrigBits;
    bool m_prescales;
    bool m_prescalesLumi;
    TriggerInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  protected:
    void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for IParticle Information.

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_kinematic    kinematic    exact
        m_numLeading   NLeading     partial
        m_useTheS      useTheS      exact
        ============== ============ =======

        .. note::
            ``m_numLeading`` requires a number ``XX`` to follow it, defining the number of leading partiles and associate it with that variable.

            For example::

                m_configStr = "... NLeading4 ..."

            will define ``int m_numLeading = 4``.


    @endrst
   */
  class IParticleInfoSwitch : public InfoSwitch {
  public:
    bool m_kinematic;
    int  m_numLeading;
    bool m_useTheS;
    IParticleInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); }
    virtual ~IParticleInfoSwitch() {}
  protected:
    virtual void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::IParticleInfoSwitch` class for Muon Information.

        ====================== ==================== =======
        Parameter              Pattern              Match
        ====================== ==================== =======
        m_trigger              trigger              exact
        m_isolation            isolation            exact
        m_isolationKinematics  isolationKinematics  exact
        m_quality              quality              exact
        m_recoparams           recoparams           exact
        m_trackparams          trackparams          exact
        m_trackhitcont         trackhitcont         exact
        m_effSF                effSF                exact
        m_energyLoss           energyLoss           exact
        m_recoWPs[XYZ]         RECO_XYZ             pattern
        m_isolWPs[""]          ISOL_                exact
        m_isolWPs[""]          ISOL_NONE            exact
        m_isolWPs[XYZ]         ISOL_XYZ             pattern
        m_trigWPs[XYZ]         TRIG_XYZ             pattern
        ====================== ==================== =======

        .. note::

             ``quality``, ``isolation`` and ``effSF`` switches do not enable any additional output by themselves. They require additional working point pattern using ``RECO_XYZ`` for quality working points and scale factors, ``ISOL_XYZ`` for isolation working points and scale factors, and ``TRIG_XYZ`` for trigger scale factors. ``XYZ`` in the pattern should be replaced using the working point name, for example::

                 m_configStr = "... RECO_Medium ..."

             will define the ``Medium`` quality working point and the accompanying scale factors.

             Isolation supports ``NONE`` or empty option which will enable scale factors without additional isolation requirements, for example::

                 m_configStr = "... ISOL_NONE ISOL_Loose ..."

             will define the ``Loose`` isolation working point status branch, and scale factors without isolation requirements and using the ``Loose`` WP.

    @endrst
   */
  class MuonInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_trigger;
    bool m_isolation;
    bool m_isolationKinematics;
    bool m_quality;
    bool m_trackparams;
    bool m_trackhitcont;
    bool m_effSF;
    bool m_energyLoss;
    bool m_promptlepton;

    std::vector< std::string > m_recoWPs;
    std::vector< std::string > m_isolWPs;
    std::vector< std::string > m_trigWPs;

    bool m_recoEff_sysNames;
    bool m_isoEff_sysNames;
    bool m_trigEff_sysNames;
    bool m_ttvaEff_sysNames;


    MuonInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); };
    virtual ~MuonInfoSwitch() {}
  protected:
    virtual void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::IParticleInfoSwitch` class for Electron Information.

        ===================== =================== =======
        Parameter             Pattern             Match
        ===================== =================== =======
        m_trigger             trigger             exact
        m_isolation           isolation           exact
        m_isolationKinematics isolationKinematics exact
        m_PID                 PID                 exact
        m_trackparams         trackparams         exact
        m_trackhitcont        trackhitcont        exact
        m_effSF               effSF               exact
        m_PIDWPs[XYZ]         PID_XYZ             pattern
        m_PIDSFWPs[XYZ]       PIDSF_XYZ           pattern
        m_isolWPs[""]         ISOL_               exact
        m_isolWPs[""]         ISOL_NONE           exact
        m_isolWPs[XYZ]        ISOL_XYZ            pattern
        m_trigWPs[XYZ]        TRIG_XYZ            pattern
        ===================== =================== =======

        .. note::

            ``PID``, ``isolation`` and ``effSF`` switches do not enable any additional output by themselves. They require additional working point pattern using ``PID_XYZ`` for PID working points, ``PIDSF_XYZ`` for PID scale factors, ``ISOL_XYZ`` for isolation working points and scale factors, and ``TRIG_XYZ`` for trigger scale factors. ``XYZ`` in the pattern should be replaced using the working point name, for example::

                m_configStr = "... PID_LHMedium PIDSF_MediumLLH ..."

            will define the ``LHMedium`` PID working point and the accompanying scale factors. Note that not all PID working points have scale factors available.

            Isolation supports ``NONE`` or empty option which will enable scale factors without additional isolation requirements, for example::

                m_configStr = "... ISOL_NONE ISOL_Loose ..."

            will define the ``Loose`` isolation working point status branch, and scale factors without isolation requirements and using the ``Loose`` WP.

    @endrst
   */
  class ElectronInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_trigger;
    bool m_isolation;
    bool m_isolationKinematics;
    bool m_quality;
    bool m_PID;
    bool m_recoparams;
    bool m_trackparams;
    bool m_trackhitcont;
    bool m_effSF;
    bool m_promptlepton;
    std::vector< std::string > m_PIDWPs;
    std::vector< std::string > m_PIDSFWPs;
    std::vector< std::string > m_isolWPs;
    std::vector< std::string > m_trigWPs;
    ElectronInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); };
    virtual ~ElectronInfoSwitch() {}
  protected:
    virtual void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::IParticleInfoSwitch` class for Photon Information.

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_isolation    isolation    exact
        m_PID          PID          exact
        m_purity       purity       exact
        m_effSF        effSF        exact
        m_trigger      trigger      exact
        m_isoCones     isoCone      partial
        ============== ============ =======

        .. note::

            ``isoCone`` can be repeated but requires a number after it, for example::

                m_configStr = "... isoCone20 isoCone40 ..."

            which will define ``std::vector<int> m_isoCones = {20,40}``.

    @endrst
   */
  class PhotonInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_isolation;
    bool m_PID;
    bool m_purity;
    bool m_effSF;
    bool m_trigger;
    std::vector<std::string> m_isoCones;
    PhotonInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); }
    virtual ~PhotonInfoSwitch() {}
  protected:
    virtual void initialize();
  };

  class ClusterInfoSwitch : public IParticleInfoSwitch {
  public:
    ClusterInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); }
    virtual ~ClusterInfoSwitch() {}
  protected:
    virtual void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::IParticleInfoSwitch` class for Jet Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_kinematic      kinematic      exact
        m_trigger        trigger        exact
        m_substructure   substructure   exact
        m_bosonCount     bosonCount     exact
        m_VTags          VTags          exact
        m_rapidity       rapidity       exact
        m_clean          clean          exact
        m_cleanLight     cleanLight     exact
        m_cleanTrig      cleanTrig      exact
        m_energy         energy         exact
        m_energyLight    energyLight    exact
        m_scales         scales         exact
        m_constscaleEta  constscaleEta  exact
        m_detectorEta    detectorEta    exact
        m_resolution     resolution     exact
        m_truth          truth          exact
        m_truthDetails   truth_details  exact
        m_layer          layer          exact
        m_trackPV        trackPV        exact
        m_trackAll       trackAll       exact
        m_jvt            JVT            exact
        m_sfJVTName      sfJVT          partial
        m_sffJVTName     sffJVT         partial
        m_allTrack       allTrack       exact
        m_allTrackPVSel  allTrackPVSel  exact
        m_allTrackDetail allTrackDetail exact
        m_constituent    constituent    exact
        m_constituentAll constituentAll exact
        m_flavorTag      flavorTag      exact
        m_flavorTagHLT   flavorTagHLT   exact
        m_sfFTagFix      sfFTagFix      partial
        m_sfFTagFlt      sfFTagFlt      partial
        m_sfFTagHyb      sfFTagHyb      partial
        m_jetBTag        jetBTag        partial
        m_area           area           exact
        m_JVC            JVC            exact
        m_tracksInJet    tracksInJet    partial
        m_trackJetName   trackJetName   partial
        m_hltVtxComp     hltVtxComp     exact
        m_onlineBS       onlineBS       exact
        m_onlineBSTool   onlineBSTool   exact
        m_charge         charge         exact
        m_passSel        passSel        exact
        m_vsLumiBlock    vsLumiBlock    exact
        m_vsActualMu     vsActualMu     exact
        m_lumiB_runN     lumiB_runN     exact
        m_byAverageMu    byAverageMu    exact
        m_byEta          byEta          exact
        m_etaPhiMap      etaPhiMap      exact
        m_muonCorrection muonCorrection exact
        ================ ============== =======

        .. note::

            ``sfJVT`` requires a working point after it, for example::

                m_configStr = "... sfJVTMedium ..."

            ``jetBTag`` expects the format ``jetBTag_tagger_type_AABB..MM..YY.ZZ``. This will create a vector of working points (AA, BB, CC, ..., ZZ) associated with that tagger. Several entries can be given. For example::

                m_configStr = "... jetBTag_MV2c10_FixedCutBEff_60707785 ..."

            will define ``std::map<std::vector<std::pair<std::string,uint>>> m_jetBTag["MV2c10"] = {std::make_pair("FixedCutBEff",60), std::make_pair("FixedCutBEff",70) ,std::make_pair("FixedCutBEff",77), std::make_pair("FixedCutBEff",85)}``.

	    ``trackJetName`` expects one or more track jet container names separated by an underscore. For example, the string ``trackJetName_GhostAntiKt2TrackJet_GhostVR30Rmax4Rmin02TrackJet`` will set the attriubte ``m_trackJetNames``
	    to ``{"GhostAntiKt2TrackJet", "GhostVR30Rmax4Rmin02TrackJet"}``.
    @endrst
   */
  class JetInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_trigger;
    bool m_substructure;
    bool m_bosonCount;
    bool m_VTags;
    bool m_rapidity;
    bool m_clean;
    bool m_cleanLight;
    bool m_cleanTrig;
    bool m_energy;
    bool m_energyLight;
    bool m_scales;
    bool m_constscaleEta;
    bool m_detectorEta;
    bool m_resolution;
    bool m_truth;
    bool m_truthDetails;
    bool m_layer;
    bool m_trackPV;
    bool m_trackAll;
    bool m_jvt;
    bool m_allTrack;
    bool m_allTrackDetail;
    bool m_allTrackPVSel;
    bool m_constituent;
    bool m_constituentAll;
    bool m_flavorTag;
    bool m_flavorTagHLT;
    bool m_btag_jettrk;
    bool m_jetFitterDetails;
    bool m_svDetails;
    bool m_ipDetails;
    bool m_tracksInJet;
    bool m_hltVtxComp;
    bool m_onlineBS;
    bool m_onlineBSTool;
    bool m_charge;
    bool m_passSel;
    bool m_etaPhiMap;
    bool m_vsLumiBlock;
    bool m_vsActualMu;
    bool m_lumiB_runN;
    bool m_byEta;
    bool m_byAverageMu;
    bool m_area;
    bool m_JVC;
    bool m_muonCorrection;
    std::string              m_trackName;
    std::vector<std::string> m_trackJetNames;
    std::string              m_sfJVTName;
    std::string              m_sffJVTName;
    std::map<std::string,std::vector<std::pair<std::string,uint>>> m_jetBTag;
    std::vector<std::string> m_jetBTagCts;
    JetInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); };
    virtual ~JetInfoSwitch() {}
  protected:
    virtual void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Truth Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_kinematic      kinematic      exact
        m_type           type           exact
        m_bVtx           bVtx           exact
        m_parents        parents        exact
        m_children       children       exact
        m_dressed        dressed        exact
        m_origin         origin         exact
        ================ ============== =======


    @endrst
   */
  class TruthInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_type;
    bool m_bVtx;
    bool m_parents;
    bool m_children;
    bool m_dressed;
    bool m_origin;
    TruthInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); };
  protected:
    void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Track Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_kinematic      kinematic      exact
        m_fitpars        fitpars        exact
        m_numbers        numbers        exact
        m_vertex         vertex         exact
        m_useTheS        useTheS        exact
        ================ ============== =======


    @endrst
  */
  class TrackInfoSwitch : public InfoSwitch {
  public:
    bool m_kinematic;
    bool m_fitpars;
    bool m_numbers;
    bool m_vertex;
    bool m_useTheS;
  TrackInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  protected:
    void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::IParticleInfoSwitch` struct for Tau Information.

        ==================  ================  =======
        Parameter           Pattern           Match
        ==================  ================  =======
        m_trigger           trigger           exact
        m_effSF             effSF             exact
        m_JetID             JetID             exact
        m_EleVeto           EleVeto           exact
        m_trackparams       trackparams       exact
        m_xahTauJetMatching xahTauJetMatching exact
        m_trackAll          trackAll          exact
        m_trackhitcont      trackhitcont      exact
        m_tauEffWP          TAUEFF_XYZ        pattern
        m_trigWP            TRIG_XYZ          pattern
        ==================  ================  =======

        .. note::

             ``identification`` and ``effSF`` switches do not enable any additional output by themselves.
             They require additional working point pattern using ``TAUEFF_XYZ`` for combined scale factors, and ``TRIG_XYZ``
             for trigger scale factors. ``XYZ`` in the pattern should be replaced using the working point name, for example::

                 m_configStr = "... TAUEFF_EleOLRElectronEleBDTLoose_TauIDMedium ... TRIG_EleOLRElectronEleBDTMedium_TauIDLoose_TrigMyTriggerMenu"

             Notice that the working point for TAUEFF is a combination of two working points from EleOLRElectron and TauID.


    @endrst
   */
  class TauInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_trigger;
    bool m_JetID;
    bool m_EleVeto;
    bool m_xahTauJetMatching;
    bool m_trackAll;
    bool m_trackparams;
    bool m_trackhitcont;
    bool m_effSF;

    std::vector< std::string > m_tauEffWPs;
    std::vector< std::string > m_trigWPs;

    TauInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); };
    virtual ~TauInfoSwitch() { }
  protected:
    virtual void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Missing :math:`\text{E}_{\text{T}}` Information.

        ==================== ====================== =======
        Parameter            Pattern                Match
        ==================== ====================== =======
        m_metClus            metClus                exact
        m_metTrk             metTrk                 exact
        m_sigClus            sigClus|all            exact
        m_sigTrk             sigTrk|all             exact
        m_sigResolutionClus  sigResolutionClus|all  exact
        m_sigResolutionTrk   sigResolutionTrk|all   exact
        m_refEle             refEle|all             exact
        m_refGamma           refGamma|all           exact
        m_refTau             refTau|all             exact
        m_refMuons           refMuons|all           exact
        m_refJet             refJet|all             exact
        m_refJetTrk          refJetTrk              exact
        m_softClus           softClus|all           exact
        m_softTrk            softTrk|all            exact
        ==================== ====================== =======


        .. note:: For all except :cpp:member:`~HelperClasses::METInfoSwitch::m_refJetTrk`, you can pass in the string ``"all"`` to enable all information. You can force only calocluster- or track-based MET using :cpp:member:`~HelperClasses::METInfoSwitch::m_metClus` or :cpp:member:`~HelperClasses::METInfoSwitch::m_metTrk`.

    @endrst
   */
  class METInfoSwitch : public InfoSwitch {
  public:
    bool m_metClus;
    bool m_metTrk;
    bool m_sigClus;
    bool m_sigTrk;
    bool m_sigResolutionClus;
    bool m_sigResolutionTrk;
    bool m_refEle;
    bool m_refGamma;
    bool m_refTau;
    bool m_refMuons;
    bool m_refJet;
    bool m_refJetTrk;
    bool m_softClus;
    bool m_softTrk;
    METInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  protected:
    void initialize();
  };

} // close namespace HelperClasses


# endif
