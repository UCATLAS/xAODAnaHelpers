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
        m_pileup         pileup         exact
        m_pileupsys      pileupsys      exact
        m_shapeEM        shapeEM        exact
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
    bool m_pileup;
    bool m_pileupsys;
    bool m_shapeEM;
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

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_basic        basic        exact
        m_menuKeys     menuKeys     exact
        m_passTriggers passTriggers exact
        m_passTrigBits passTrigBits exact
        ============== ============ =======

    @endrst
   */
  class TriggerInfoSwitch : public InfoSwitch {
  public:
    bool m_basic;
    bool m_menuKeys;
    bool m_passTriggers;
    bool m_passTrigBits;
    TriggerInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  protected:
    void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Jet Trigger Information.

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_kinematic    kinematic    exact
        m_clean        clean        exact
        ============== ============ =======

    @endrst
   */

  class JetTriggerInfoSwitch : public InfoSwitch {
  public:
    bool m_kinematic;
    bool m_clean;
    JetTriggerInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
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
        m_trackparams          trackparams          exact
        m_trackhitcont         trackhitcont         exact
        m_effSF                effSF                exact
        m_energyLoss           energyLoss           exact
        ====================== ==================== =======

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

        =============================================================================================================================================================== =============================================================================================================================================== =======
        Parameter                                                                                                                                                       Pattern                                                                                                                                         Match
        =============================================================================================================================================================== =============================================================================================================================================== =======
        m_trigger                                                                                                                                                       trigger                                                                                                                                         exact
        m_isolation                                                                                                                                                     isolation                                                                                                                                       exact
        m_isolationKinematics                                                                                                                                           isolationKinematics                                                                                                                             exact
        m_quality                                                                                                                                                       quality                                                                                                                                         exact
        m_PID                                                                                                                                                           PID                                                                                                                                             exact
        m_trackparams                                                                                                                                                   trackparams                                                                                                                                     exact
        m_trackhitcont                                                                                                                                                  trackhitcont                                                                                                                                    exact
        m_effSF                                                                                                                                                         effSF                                                                                                                                           exact
        m_PIDWPs["LooseAndBLayerLLH"]                                                                                                                                   LooseAndBLayerLLH                                                                                                                               exact
        m_PIDWPs["MediumLLH"]                                                                                                                                           MediumLLH                                                                                                                                       exact
        m_PIDWPs["TightLLH"]                                                                                                                                            TightLLH                                                                                                                                        exact
        m_isolWPs["isolFixedCutLoose"]                                                                                                                                  isolFixedCutLoose                                                                                                                               exact
        m_isolWPs["isolFixedCutTight"]                                                                                                                                  isolFixedCutTight                                                                                                                               exact
        m_isolWPs["isolFixedCutTightTrackOnly"]                                                                                                                         isolFixedCutTightTrackOnly                                                                                                                      exact
        m_isolWPs["isolGradient"]                                                                                                                                       isolGradient                                                                                                                                    exact
        m_isolWPs["isolGradientLoose"]                                                                                                                                  isolGradientLoose                                                                                                                               exact
        m_isolWPs["isolLoose"]                                                                                                                                          isolLoose                                                                                                                                       exact
        m_isolWPs["isolLooseTrackOnly"]                                                                                                                                 isolLooseTrackOnly                                                                                                                              exact
        m_isolWPs["isolTight"]                                                                                                                                          isolTight                                                                                                                                       exact
        m_isolWPs[""]                                                                                                                                                   isolNoRequirement                                                                                                                               exact
        m_trigWPs[DI_E_2015_e12_lhloose_L1EM10VH_2016_e15_lhvloose_nod0_L1EM13VH]                                                                                       DI_E_2015_e12_lhloose_L1EM10VH_2016_e15_lhvloose_nod0_L1EM13VH                                                                                  exact
        m_trigWPs[DI_E_2015_e12_lhloose_L1EM10VH_2016_e17_lhvloose_nod0]                                                                                                DI_E_2015_e12_lhloose_L1EM10VH_2016_e17_lhvloose_nod0                                                                                           exact
        m_trigWPs[DI_E_2015_e17_lhloose_2016_e17_lhloose]                                                                                                               DI_E_2015_e17_lhloose_2016_e17_lhloose                                                                                                          exact
        m_trigWPs[MULTI_L_2015_e7_lhmedium_2016_e7_lhmedium_nod0]                                                                                                       MULTI_L_2015_e7_lhmedium_2016_e7_lhmedium_nod0                                                                                                  exact
        m_trigWPs[MULTI_L_2015_e12_lhloose_2016_e12_lhloose_nod0]                                                                                                       MULTI_L_2015_e12_lhloose_2016_e12_lhloose_nod0                                                                                                  exact
        m_trigWPs[MULTI_L_2015_e17_lhloose_2016_e17_lhloose_nod0]                                                                                                       MULTI_L_2015_e17_lhloose_2016_e17_lhloose_nod0                                                                                                  exact
        m_trigWPs[MULTI_L_2015_e24_lhmedium_L1EM20VHI_2016_e24_lhmedium_nod0_L1EM20VHI]                                                                                 MULTI_L_2015_e24_lhmedium_L1EM20VHI_2016_e24_lhmedium_nod0_L1EM20VHI                                                                            exact
        m_trigWPs[MULTI_L_2015_e24_lhmedium_L1EM20VHI_2016_e26_lhmedium_nod0_L1EM22VHI]                                                                                 MULTI_L_2015_e24_lhmedium_L1EM20VHI_2016_e26_lhmedium_nod0_L1EM22VHI                                                                            exact
        m_trigWPs[SINGLE_E_2015_e24_lhmedium_L1EM20VH_OR_e60_lhmedium_OR_e120_lhloose_2016_e24_lhtight_nod0_ivarloose_OR_e60_lhmedium_nod0_OR_e140_lhloose_nod0]        SINGLE_E_2015_e24_lhmedium_L1EM20VH_OR_e60_lhmedium_OR_e120_lhloose_2016_e24_lhtight_nod0_ivarloose_OR_e60_lhmedium_nod0_OR_e140_lhloose_nod0   exact
        m_trigWPs[SINGLE_E_2015_e24_lhmedium_L1EM20VH_OR_e60_lhmedium_OR_e120_lhloose_2016_e26_lhtight_nod0_ivarloose_OR_e60_lhmedium_nod0_OR_e140_lhloose_nod0]        SINGLE_E_2015_e24_lhmedium_L1EM20VH_OR_e60_lhmedium_OR_e120_lhloose_2016_e26_lhtight_nod0_ivarloose_OR_e60_lhmedium_nod0_OR_e140_lhloose_nod0   exact
        m_trigWPs[TRI_E_2015_e9_lhloose_2016_e9_lhloose_nod0]                                                                                                           TRI_E_2015_e9_lhloose_2016_e9_lhloose_nod0                                                                                                      exact
        m_trigWPs[TRI_E_2015_e9_lhloose_2016_e9_lhmedium_nod0]                                                                                                          TRI_E_2015_e9_lhloose_2016_e9_lhmedium_nod0                                                                                                     exact
        m_trigWPs[TRI_E_2015_e17_lhloose_2016_e17_lhloose_nod0]                                                                                                         TRI_E_2015_e17_lhloose_2016_e17_lhloose_nod0                                                                                                    exact
        m_trigWPs[TRI_E_2015_e17_lhloose_2016_e17_lhmedium_nod0]                                                                                                        TRI_E_2015_e17_lhloose_2016_e17_lhmedium_nod0                                                                                                   exact
        =============================================================================================================================================================== =============================================================================================================================================== =======

    @endrst
   */
  class ElectronInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_trigger;
    bool m_isolation;
    bool m_isolationKinematics;
    bool m_quality;
    bool m_PID;
    bool m_trackparams;
    bool m_trackhitcont;
    bool m_effSF;
    bool m_promptlepton;
    std::vector< std::string > m_PIDWPs;
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
        ============== ============ =======

    @endrst
   */
  class PhotonInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_isolation;
    bool m_PID;
    bool m_purity;
    bool m_effSF;
    bool m_trigger;
    PhotonInfoSwitch(const std::string configStr) : IParticleInfoSwitch(configStr) { initialize(); }
    virtual ~PhotonInfoSwitch() {}
  protected:
    virtual void initialize();
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::IParticleInfoSwitch` class for Jet Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_substructure   substructure   exact
        m_bosonCount     bosonCount     exact
        m_VTags          VTags          exact
        m_rapidity       rapidity       exact
        m_clean          clean          exact
        m_energy         energy         exact
        m_scales         scales         exact
        m_constscaleEta  constscaleEta  exact
        m_resolution     resolution     exact
        m_truth          truth          exact
        m_truthDetails   truth_details  exact
        m_layer          layer          exact
        m_trackPV        trackPV        exact
        m_trackAll       trackAll       exact
        m_sfJVTName      sfJVT          partial
        m_sffJVTName     sffJVT         partial
        m_allTrack       allTrack       exact
        m_allTrackPVSel  allTrackPVSel  exact
        m_allTrackDetail allTrackDetail exact
        m_constituent    constituent    exact
        m_constituentAll constituentAll exact
        m_flavTag        flavorTag      exact
        m_flavTagHLT     flavorTagHLT   exact
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
        m_vsLumiBlock    vsLumiBlock    exact
        m_lumiB_runN     lumiB_runN     exact
        m_byAverageMu    byAverageMu    exact
        m_byEta          byEta          exact
        m_etaPhiMap      etaPhiMap      exact
        ================ ============== =======

        .. note::

            ``sfJVT`` requires a working point after it, for example::

                m_configStr = "... sfJVTMedium ..."

            ``sfFTagFix``, ``sfFTagFlt`` and ``sfFTagHyb`` require a string of numbers pairwise ``AABB..MM..YYZZ`` succeeding it. This will create a vector of numbers (AA, BB, CC, ..., ZZ) associated with that variable. For example::

                m_configStr = "... sfFTagFix010203 ..."

            will define ``std::vector<int> m_sfFTagFix = {1,2,3}``. THIS OPTION IS DEPRICATED!

            ``jetBTag`` expects the format ``jetBTag_tagger_type_AABB..MM..YY.ZZ``. This will create a vector of working points (AA, BB, CC, ..., ZZ) associated with that tagger. Several entries can be given. For example::

                m_configStr = "... jetBTag_MV2c10_HybBEff_60707785 ..."

            will define ``std::map<std::vector<std::pair<std::string,uint>>> m_jetBTag["MV2c10"] = {std::make_pair("HybBEff",60), std::make_pair("HybBEff",70) ,std::make_pair("HybBEff",77), std::make_pair("HybBEff",85)}``.

    @endrst
   */
  class JetInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_substructure;
    bool m_bosonCount;
    bool m_VTags;
    bool m_rapidity;
    bool m_clean;
    bool m_energy;
    bool m_scales;
    bool m_constscaleEta;
    bool m_resolution;
    bool m_truth;
    bool m_truthDetails;
    bool m_layer;
    bool m_trackPV;
    bool m_trackAll;
    bool m_allTrack;
    bool m_allTrackDetail;
    bool m_allTrackPVSel;
    bool m_constituent;
    bool m_constituentAll;
    bool m_flavTag;
    bool m_flavTagHLT;
    bool m_btag_jettrk;
    bool m_jetFitterDetails;
    bool m_svDetails;
    bool m_ipDetails;
    bool m_tracksInJet;
    bool m_trackJets;
    bool m_hltVtxComp;
    bool m_onlineBS;
    bool m_onlineBSTool;
    bool m_charge;
    bool m_etaPhiMap;
    bool m_vsLumiBlock;
    bool m_lumiB_runN;
    bool m_byEta;
    bool m_byAverageMu;
    bool m_area;
    bool m_JVC;
    std::string      m_trackName;
    std::string      m_trackJetName;
    std::string      m_sfJVTName;
    std::string      m_sffJVTName;
    std::vector<int> m_sfFTagFix;
    std::vector<int> m_sfFTagFlt;
    std::vector<int> m_sfFTagHyb;
    std::map<std::string,std::vector<std::pair<std::string,uint>>> m_jetBTag;
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
        ================ ============== =======


    @endrst
   */
  class TruthInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_type;
    bool m_bVtx;
    bool m_parents;
    bool m_children;
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

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_trackparams    trackparams    exact
        m_trackhitcont   trackhitcont   exact
        ================ ============== =======


    @endrst
   */
  class TauInfoSwitch : public IParticleInfoSwitch {
  public:
    bool m_trackparams;
    bool m_trackhitcont;
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
