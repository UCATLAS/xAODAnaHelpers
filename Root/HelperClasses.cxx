#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODPrimitives/IsolationType.h"
#include "xAODMuon/Muon.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"


namespace HelperClasses{

  /* parser for electron isolation enum */
  template <>
  EnumParser<xAOD::Iso::IsolationType>::EnumParser()
  {

    TString etcone20("etcone20");         enumMap.insert(std::make_pair(etcone20,      xAOD::Iso::etcone20));
    TString topoetcone20("topoetcone20"); enumMap.insert(std::make_pair(topoetcone20,  xAOD::Iso::topoetcone20));
    TString topoetcone30("topoetcone30"); enumMap.insert(std::make_pair(topoetcone30,  xAOD::Iso::topoetcone30));
    TString ptcone20("ptcone20");         enumMap.insert(std::make_pair(ptcone20,      xAOD::Iso::ptcone20));
    TString ptcone30("ptcone30");         enumMap.insert(std::make_pair(ptcone30,      xAOD::Iso::ptcone30));
  }

  /* parser for electron likelihood PID enum */
  template <>
  EnumParser<LikeEnum::Menu>::EnumParser()
  {
    TString VeryLoose("VeryLoose");         enumMap.insert(std::make_pair(VeryLoose  , LikeEnum::VeryLoose));
    TString Loose("Loose");                 enumMap.insert(std::make_pair(Loose      , LikeEnum::Loose));
    TString Medium("Medium");               enumMap.insert(std::make_pair(Medium     , LikeEnum::Medium));
    TString Tight("Tight");                 enumMap.insert(std::make_pair(Tight      , LikeEnum::Tight));
    TString VeryTight("VeryTight");         enumMap.insert(std::make_pair(VeryTight  , LikeEnum::VeryTight));
    TString LooseRelaxed("LooseRelaxed");   enumMap.insert(std::make_pair(LooseRelaxed      , LikeEnum::LooseRelaxed));
  }

  /* parser for muon quality enum */
  template <>
  EnumParser<xAOD::Muon::Quality>::EnumParser()
  {
    TString VeryLoose("VeryLoose");       enumMap.insert(std::make_pair(VeryLoose    , xAOD::Muon::VeryLoose));
    TString Loose("Loose");               enumMap.insert(std::make_pair(Loose        , xAOD::Muon::Loose));
    TString Medium("Medium");             enumMap.insert(std::make_pair(Medium       , xAOD::Muon::Medium));
    TString Tight("Tight");               enumMap.insert(std::make_pair(Tight        , xAOD::Muon::Tight));
  }

  /* parser for muon type enum */
  template <>
  EnumParser<xAOD::Muon::MuonType>::EnumParser()
  {
    TString Combined("Combined");       				   enumMap.insert(std::make_pair(Combined    		      , xAOD::Muon::Combined));
    TString MuonStandAlone("MuonStandAlone");               		   enumMap.insert(std::make_pair(MuonStandAlone        	      , xAOD::Muon::MuonStandAlone));
    TString SegmentTagged("SegmentTagged");             		   enumMap.insert(std::make_pair(SegmentTagged       	      , xAOD::Muon::SegmentTagged));
    TString CaloTagged("CaloTagged");                                      enumMap.insert(std::make_pair(CaloTagged        	      , xAOD::Muon::CaloTagged));
    TString SiliconAssociatedForwardMuon("SiliconAssociatedForwardMuon");  enumMap.insert(std::make_pair(SiliconAssociatedForwardMuon , xAOD::Muon::SiliconAssociatedForwardMuon));
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
  bool InfoSwitch::parse(std::string flag)
  {
    return m_configStr.find(flag) != std::string::npos;
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
    m_MJB           = parse("MJB");
  }


} // close namespace HelperClasses
