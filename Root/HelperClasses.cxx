#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODPrimitives/IsolationType.h"
#include "xAODMuon/Muon.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/egammaPIDdefs.h"


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
  }

  /* parser for electron likelihood PID enum */
  template <>
  EnumParser<LikeEnum::Menu>::EnumParser()
  {
    std::string VeryLoose("VeryLoose");         enumMap.insert(std::make_pair(VeryLoose  , LikeEnum::VeryLoose));
    std::string Loose("Loose");                 enumMap.insert(std::make_pair(Loose      , LikeEnum::Loose));
    std::string Medium("Medium");               enumMap.insert(std::make_pair(Medium     , LikeEnum::Medium));
    std::string Tight("Tight");                 enumMap.insert(std::make_pair(Tight      , LikeEnum::Tight));
    std::string VeryTight("VeryTight");         enumMap.insert(std::make_pair(VeryTight  , LikeEnum::VeryTight));
    std::string LooseRelaxed("LooseRelaxed");   enumMap.insert(std::make_pair(LooseRelaxed      , LikeEnum::LooseRelaxed));
  }

  /* parser for electron cut-based PID enum */
  /* Apparently this won't be useful for non-Athena users...  */
  template <>
  EnumParser<egammaPID::egammaIDQuality>::EnumParser()
  {
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
  template <>
  EnumParser<egammaPID::PID>::EnumParser()
  {
    std::string IsEMLoose("IsEMLoose");     enumMap.insert(std::make_pair( IsEMLoose , egammaPID::IsEMLoose));
    std::string IsEMMedium("IsEMMedium");   enumMap.insert(std::make_pair( IsEMMedium, egammaPID::IsEMMedium));
    std::string IsEMTight("IsEMTight");     enumMap.insert(std::make_pair( IsEMTight , egammaPID::IsEMTight));
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


  /**************************************
   *
   *  Strings are used to turn on and off
   *  histograms and branches in the tree
   *  The following structs hold the bools
   *  used to control the content and also
   *  have the string which is necessary to
   *  turn a set on
   *
   **************************************/
  bool InfoSwitch::parse(const std::string flag)
  {
    return m_configStr.find(flag) != std::string::npos;
  }

  void EventInfoSwitch::initialize(){
    m_pileup        = parse("pileup");
    m_shapeEM       = parse("shapeEM");
    m_shapeLC       = parse("shapeLC");
    m_truth         = parse("truth");
  }

  void MuonInfoSwitch::initialize(){
    m_kinematic     = parse("kinematic");
    m_trackparams   = parse("trackparams");
    m_trackhitcont  = parse("trackhitcont");
  }

  void ElectronInfoSwitch::initialize(){
    m_kinematic     = parse("kinematic");
    m_trackparams   = parse("trackparams");
    m_trackhitcont  = parse("trackhitcont");
  }

  void JetInfoSwitch::initialize(){
    m_kinematic     = parse("kinematic");
    m_clean         = parse("clean");
    m_energy        = parse("energy");
    m_resolution    = parse("resolution");
    m_truth         = parse("truth");
    m_truthDetails  = parse("truth_details");
    m_layer         = parse("layer");
    m_trackPV       = parse("trackPV");
    m_trackAll      = parse("trackAll");
    m_allTrack      = parse("allTrack");
    m_flavTag       = parse("flavorTag");
    if( parse("LeadingJets") ){
      m_numLeadingJets = std::atoi( (m_configStr.substr( m_configStr.find("LeadingJets")-2 , 2)).c_str() );
      if (m_numLeadingJets == 0){ //Perhaps infoSwitches are combined and Njets < 10
        m_numLeadingJets = std::atoi( (m_configStr.substr( m_configStr.find("LeadingJets")-1 , 1)).c_str() );
      }
    }else{
      m_numLeadingJets = 0;
    }
  }
  
  void TauInfoSwitch::initialize(){
    m_kinematic     = parse("kinematic");
    m_trackparams   = parse("trackparams");
    m_trackhitcont  = parse("trackhitcont");
  }

} // close namespace HelperClasses
