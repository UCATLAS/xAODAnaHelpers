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


  struct InfoSwitch {
    const std::string m_configStr;
    std::set<std::string> m_configDetails;
    InfoSwitch(const std::string configStr) : m_configStr(configStr) {
        std::stringstream ss(m_configStr);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        m_configDetails = std::set<std::string>(begin, end);
        //std::copy(m_configDetails.begin(), m_configDetails.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    };
    bool parse(const std::string flag) { return m_configDetails.find(flag) != m_configDetails.end(); };
  };

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

  struct TriggerInfoSwitch : InfoSwitch {
    bool m_basic;
    bool m_menuKeys;
    bool m_passTriggers;
    void initialize();
    TriggerInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  struct JetTriggerInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_clean;
    void initialize();
    JetTriggerInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  struct MuonInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_trigger;
    bool m_isolation;
    bool m_quality;
    bool m_trackparams;
    bool m_trackhitcont;
    bool m_effSF;
    void initialize();
    MuonInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

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

  struct PhotonInfoSwitch : InfoSwitch {
    bool m_kinematic;
    void initialize();
    PhotonInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

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
    bool m_area;
    int  m_numLeadingJets;
    void initialize();
    std::vector<int> m_sfFTagFix;
    std::vector<int> m_sfFTagFlt;
    JetInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

  struct TruthInfoSwitch : InfoSwitch {
    bool m_kinematic;
    void initialize();
    TruthInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };


  struct TauInfoSwitch : InfoSwitch {
    bool m_kinematic;
    bool m_trackparams;
    bool m_trackhitcont;
    void initialize();
    TauInfoSwitch(const std::string configStr) : InfoSwitch(configStr) { initialize(); };
  };

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
