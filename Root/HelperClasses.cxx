#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODPrimitives/IsolationType.h"
#include "xAODMuon/Muon.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/egammaPIDdefs.h"
#include "ElectronPhotonSelectorTools/LikelihoodEnums.h"
#include "xAODTau/TauDefs.h"

namespace HelperClasses{

  /* parser for electron isolation enum */
  template <>
  EnumParser<xAOD::Iso::IsolationType>::EnumParser()
  {
    std::string etcone20("etcone20");         enumMap.insert(std::make_pair(etcone20,      xAOD::Iso::etcone20));
    std::string topoetcone20("topoetcone20"); enumMap.insert(std::make_pair(topoetcone20,  xAOD::Iso::topoetcone20));
    std::string topoetcone30("topoetcone30"); enumMap.insert(std::make_pair(topoetcone30,  xAOD::Iso::topoetcone30));
    std::string ptcone20("ptcone20");         enumMap.insert(std::make_pair(ptcone20,      xAOD::Iso::ptcone20));
    std::string ptcone30("ptcone30");         enumMap.insert(std::make_pair(ptcone30,      xAOD::Iso::ptcone30));
    std::string ptvarcone20("ptvarcone20");   enumMap.insert(std::make_pair(ptvarcone20,   xAOD::Iso::ptvarcone20));
    std::string ptvarcone30("ptvarcone30");   enumMap.insert(std::make_pair(ptvarcone30,   xAOD::Iso::ptvarcone30));
  }

  /* parser for electron likelihood PID enum */
  template <>
  EnumParser<LikeEnum::Menu>::EnumParser()
  {
    std::string VeryLoose("VeryLoose");         enumMap.insert(std::make_pair(VeryLoose  , LikeEnum::VeryLoose));
    std::string Loose("Loose");                 enumMap.insert(std::make_pair(Loose      , LikeEnum::Loose));
    std::string Medium("Medium");               enumMap.insert(std::make_pair(Medium     , LikeEnum::Medium));
    std::string Tight("Tight");                 enumMap.insert(std::make_pair(Tight      , LikeEnum::Tight));
  }

  /* parser for electron cut-based PID enum */
  /* Apparently this won't be useful for non-Athena users...  */
  template <>
  EnumParser<egammaPID::egammaIDQuality>::EnumParser()
  {
    std::string ElectronIDLoose("ElectronIDLoose");           enumMap.insert(std::make_pair(ElectronIDLoose    , egammaPID::ElectronIDLoose));
    std::string ElectronIDMedium("ElectronIDMedum");          enumMap.insert(std::make_pair(ElectronIDMedium   , egammaPID::ElectronIDMedium));
    std::string ElectronIDTight("ElectronIDTight");           enumMap.insert(std::make_pair(ElectronIDTight    , egammaPID::ElectronIDTight));
    std::string ElectronIDLoosePP("ElectronIDLoosePP");       enumMap.insert(std::make_pair(ElectronIDLoosePP  , egammaPID::ElectronIDLoosePP));
    std::string ElectronIDLoose1("ElectronIDLoose1");         enumMap.insert(std::make_pair(ElectronIDLoose1   , egammaPID::ElectronIDLoose1));
    std::string ElectronIDLooseHLT("ElectronIDLooseHLT");     enumMap.insert(std::make_pair(ElectronIDLooseHLT , egammaPID::ElectronIDLooseHLT));
    std::string ElectronIDMediumPP("ElectronIDMediumPP");     enumMap.insert(std::make_pair(ElectronIDMediumPP , egammaPID::ElectronIDMediumPP));
    std::string ElectronIDMedium1("ElectronIDMedium1");       enumMap.insert(std::make_pair(ElectronIDMedium1  , egammaPID::ElectronIDMedium1));
    std::string ElectronIDMediumHLT("ElectronIDMediumHLT");   enumMap.insert(std::make_pair(ElectronIDMediumHLT, egammaPID::ElectronIDMediumHLT));
    std::string ElectronIDTightPP("ElectronIDTightPP");       enumMap.insert(std::make_pair(ElectronIDTightPP  , egammaPID::ElectronIDTightPP));
    std::string ElectronIDTight1("ElectronIDTight1");         enumMap.insert(std::make_pair(ElectronIDTight1   , egammaPID::ElectronIDTight1));
    std::string ElectronIDTightHLT("ElectronIDTightHLT");     enumMap.insert(std::make_pair(ElectronIDTightHLT , egammaPID::ElectronIDTightHLT));
  }
//  template <>
//  EnumParser<egammaPID::PID>::EnumParser()
//  {
//    std::string IsEMLoose("IsEMLoose");     enumMap.insert(std::make_pair( IsEMLoose , egammaPID::IsEMLoose));
//    std::string IsEMMedium("IsEMMedium");   enumMap.insert(std::make_pair( IsEMMedium, egammaPID::IsEMMedium));
//    std::string IsEMTight("IsEMTight");     enumMap.insert(std::make_pair( IsEMTight , egammaPID::IsEMTight));
//  }


  /* parser for Tau BDT ID enum */
  /* Apparently this won't be useful for non-Athena users...  */

  template <>
  EnumParser<xAOD::TauJetParameters::IsTauFlag>::EnumParser()
  {
    std::string TauIDVeryLoose("TauIDVeryLoose"); enumMap.insert(std::make_pair(TauIDVeryLoose , xAOD::TauJetParameters::JetBDTSigVeryLoose));
    std::string TauIDLoose("TauIDLoose");         enumMap.insert(std::make_pair(TauIDLoose     , xAOD::TauJetParameters::JetBDTSigLoose));
    std::string TauIDMedium("TauIDMedium");       enumMap.insert(std::make_pair(TauIDMedium    , xAOD::TauJetParameters::JetBDTSigMedium));
    std::string TauIDTight("TauIDTight");         enumMap.insert(std::make_pair(TauIDTight     , xAOD::TauJetParameters::JetBDTSigTight));
  }

  /* parser for muon quality enum */
  template <>
  EnumParser<xAOD::Muon::Quality>::EnumParser()
  {
    std::string VeryLoose("VeryLoose");       enumMap.insert(std::make_pair(VeryLoose    , xAOD::Muon::VeryLoose));
    std::string Loose("Loose");               enumMap.insert(std::make_pair(Loose        , xAOD::Muon::Loose));
    std::string Medium("Medium");             enumMap.insert(std::make_pair(Medium       , xAOD::Muon::Medium));
    std::string Tight("Tight");               enumMap.insert(std::make_pair(Tight        , xAOD::Muon::Tight));
  }

  /* parser for muon type enum */
  template <>
  EnumParser<xAOD::Muon::MuonType>::EnumParser()
  {
    std::string Combined("Combined");       				        enumMap.insert(std::make_pair(Combined    		      , xAOD::Muon::Combined));
    std::string MuonStandAlone("MuonStandAlone");               	        enumMap.insert(std::make_pair(MuonStandAlone        	      , xAOD::Muon::MuonStandAlone));
    std::string SegmentTagged("SegmentTagged");             		        enumMap.insert(std::make_pair(SegmentTagged       	      , xAOD::Muon::SegmentTagged));
    std::string CaloTagged("CaloTagged");                                       enumMap.insert(std::make_pair(CaloTagged        	      , xAOD::Muon::CaloTagged));
    std::string SiliconAssociatedForwardMuon("SiliconAssociatedForwardMuon");   enumMap.insert(std::make_pair(SiliconAssociatedForwardMuon , xAOD::Muon::SiliconAssociatedForwardMuon));
  }

  std::string InfoSwitch::get_working_point(const std::string flag) {
    for (auto configDetail : m_configDetails) {
      if (configDetail.compare(0, flag.size(), flag) == 0) {
        return configDetail.substr(flag.size(), std::string::npos);
      }
    }
    return "";
  }

  std::vector<std::string>InfoSwitch::get_working_points(const std::string flag) {
    std::vector<std::string> wps;
    for (auto configDetail : m_configDetails) {
      if (configDetail.compare(0, flag.size(), flag) == 0) {
        wps.push_back(configDetail.substr(flag.size(), std::string::npos));
      }
    }
    return wps;
  }

} // close namespace HelperClasses
