#ifndef xAODAnaHelpers_HELPERCLASSES_H
#define xAODAnaHelpers_HELPERCLASSES_H

#include <map>
#include <iostream>

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
      OVERLAPREMOVER,
      CALIBRATOR,
      CORRECTOR,
  };

  /* template enum parser
  copied from: http://stackoverflow.com/a/726681
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
  struct InfoSwitch {
    /**
        The input configuration string from which we split up into tokens.
     */
    const std::string m_configStr;
    /**
        The vector of tokens from which we search through for finding matches.
     */
    std::set<std::string> m_configDetails;
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
            .. deprecated:: 00-03-26
               Use :cpp:func:`~HelperClasses::InfoSwitch::has_match` instead.

        @endrst
        @param flag     The string we search for.
     */
    bool parse(const std::string flag) { std::cout << "InfoSwitch::parse() deprecated soon!" << std::endl; return has_match(flag); };
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
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Event Information.

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_pileup       pileup       exact
        m_shapeEM      shapeEM      exact
        m_shapeLC      shapeLC      exact
        m_truth        truth        exact
        m_caloClus     caloClusters exact
        m_muonSF       muonSF       exact
        m_electronSF   electronSF   exact
        ============== ============ =======
    @endrst
   */
  struct EventInfoSwitch : InfoSwitch {
    bool m_pileup;
    bool m_shapeEM;
    bool m_shapeLC;
    bool m_truth;
    bool m_caloClus;
    bool m_muonSF;
    bool m_electronSF;
    void initialize();
    EventInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
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
        ============== ============ =======
    @endrst
   */
  struct TriggerInfoSwitch : InfoSwitch {
    bool m_basic;
    bool m_menuKeys;
    bool m_passTriggers;
    void initialize();
    TriggerInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
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
  struct JetTriggerInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_clean;
    void initialize();
    JetTriggerInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Muon Information.

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_kinematic    kinematic    exact
        m_trigger      trigger      exact
        m_isolation    isolation    exact
        m_quality      quality      exact
        m_trackparams  trackparams  exact
        m_trackhitcont trackhitcont exact
        m_effSF        effSF        exact
        m_energyLoss   energyLoss   exact
        ============== ============ =======
    @endrst
   */
  struct MuonInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_trigger;
    bool m_isolation;
    bool m_quality;
    bool m_trackparams;
    bool m_trackhitcont;
    bool m_effSF;
    bool m_energyLoss;
    void initialize();
    MuonInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Electron Information.

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_kinematic    kinematic    exact
        m_trigger      trigger      exact
        m_isolation    isolation    exact
        m_PID          PID          exact
        m_trackparams  trackparams  exact
        m_trackhitcont trackhitcont exact
        m_effSF        effSF        exact
        ============== ============ =======
    @endrst
   */
  struct ElectronInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_trigger;
    bool m_isolation;
    bool m_PID;
    bool m_trackparams;
    bool m_trackhitcont;
    bool m_effSF;
    void initialize();
    ElectronInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Photon Information.

        ============== ============ =======
        Parameter      Pattern      Match
        ============== ============ =======
        m_kinematic    kinematic    exact
        m_isolation    isolation    exact
        m_PID          PID          exact
        ============== ============ =======
    @endrst
   */
  struct PhotonInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_isolation;
    bool m_PID;
    void initialize();
    PhotonInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Jet Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_kinematic      kinematic      exact
        m_substructure   substructure   exact
        m_rapidity       rapidity       exact
        m_clean          clean          exact
        m_energy         energy         exact
        m_scales         scales         exact
        m_resolution     resolution     exact
        m_truth          truth          exact
        m_truthDetails   truth_details  exact
        m_layer          layer          exact
        m_trackPV        trackPV        exact
        m_trackAll       trackAll       exact
        m_allTrack       allTrack       exact
        m_allTrackPVSel  allTrackPVSel  exact
        m_allTrackDetail allTrackDetail exact
        m_constituent    constituent    exact
        m_constituentAll constituentAll exact
        m_flavTag        flavorTag      exact
        m_flavTagHLT     flavorTagHLT   exact
        m_sfFTagFix      sfFTagFix      partial
        m_sfFTagFlt      sfFTagFlt      partial
        m_area           area           exact
        m_numLeadingJets LeadingJets    partial
        ================ ============== =======

        .. note::
            ``sfFTagFix`` and ``sfFTagFlt`` require a string of numbers pairwise ``AABB..MM..YYZZ`` succeeding it. This will create a vector of numbers (AA, BB, CC, ..., ZZ) associated with that variable.

            For example::

                m_configStr = "... sfFTagFix010203 ..."

            will define :code:`std::vector<int> m_sfFTagFix = {1,2,3}`.

        .. note::
            ``m_numLeadingJets`` requires a number ``XX`` to succeed it defining the number of leading jets and associate it with that variable.

            For example::

                m_configStr = "... LeadingJets4 ..."

            will define :code:`int m_numLeadingJets = 4`.


    @endrst

   */
  struct JetInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_substructure;
    bool m_rapidity;
    bool m_clean;
    bool m_energy;
    bool m_scales;
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
    bool m_jetFitterDetails;
    bool m_svDetails;
    bool m_ipDetails;
    bool m_area;
    int  m_numLeadingJets;
    void initialize();
    std::vector<int> m_sfFTagFix;
    std::vector<int> m_sfFTagFlt;
    JetInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Truth Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_kinematic      kinematic      exact
        ================ ============== =======
    @endrst
   */
  struct TruthInfoSwitch : InfoSwitch {
    bool m_kinematic;
    void initialize();
    TruthInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Tau Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_kinematic      kinematic      exact
        m_trackparams    trackparams    exact
        m_trackhitcont   trackhitcont   exact
        ================ ============== =======
    @endrst
   */
  struct TauInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_trackparams;
    bool m_trackhitcont;
    void initialize();
    TauInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  /**
    @rst
        The :cpp:class:`HelperClasses::InfoSwitch` struct for Missing :math:`\text{E}_{\text{T}}` Information.

        ================ ============== =======
        Parameter        Pattern        Match
        ================ ============== =======
        m_refEle         refEle|all     exact
        m_refGamma       refGamma|all   exact
        m_refTau         refTau|all     exact
        m_refMuons       refMuons|all   exact
        m_refJet         refJet|all     exact
        m_refJetTrk      refJetTrk      exact
        m_softClus       softClus|all   exact
        m_softTrk        softTrk|all    exact
        ================ ============== =======

        .. note:: For all except :cpp:member:`~HelperClasses::METInfoSwitch::m_refJetTrk`, you can pass in the string ``"all"`` to enable all information.

    @endrst
   */
  struct METInfoSwitch : InfoSwitch {
    bool m_refEle;
    bool m_refGamma;
    bool m_refTau;
    bool m_refMuons;
    bool m_refJet;
    bool m_refJetTrk;
    bool m_softClus;
    bool m_softTrk;
    void initialize();
    METInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

} // close namespace HelperClasses


# endif
