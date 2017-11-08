#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODPrimitives/IsolationType.h"
#include "xAODMuon/Muon.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/egammaPIDdefs.h"
#include "ElectronPhotonSelectorTools/LikelihoodEnums.h"


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

  /*
            !!!!!!!!!!!!!WARNING!!!!!!!!!!!!!
              If you change the string here,
                please update the relevant
            documentation in the header file.
            !!!!!!!!!!!!!WARNING!!!!!!!!!!!!!
  */

  void EventInfoSwitch::initialize(){
    m_pileup        = has_exact("pileup");
    m_pileupsys     = has_exact("pileupsys");
    m_eventCleaning = has_exact("eventCleaning");
    m_shapeEM       = has_exact("shapeEM");
    m_shapeLC       = has_exact("shapeLC");
    m_truth         = has_exact("truth");
    m_caloClus      = has_exact("caloClusters");
    m_weightsSys    = has_exact("weightsSys");
  }

  void TriggerInfoSwitch::initialize(){
    m_basic             = has_exact("basic");
    m_menuKeys          = has_exact("menuKeys");
    m_passTriggers      = has_exact("passTriggers");
    m_passTrigBits      = has_exact("passTrigBits");
  }

  void JetTriggerInfoSwitch::initialize(){
    m_kinematic     = has_exact("kinematic");
    m_clean         = has_exact("clean");
  }

  void IParticleInfoSwitch::initialize(){
    m_kinematic     = has_exact("kinematic");

    m_numLeading    = 0;
    for(auto configDetail : m_configDetails)
      {
	if( configDetail.compare(0,8,"NLeading")==0)
	  {
	    m_numLeading = std::atoi( configDetail.substr(8, std::string::npos).c_str() );
	    break;
	  }
      }

    m_useTheS   = has_exact("useTheS");
  }

  void MuonInfoSwitch::initialize(){
    m_trigger       = has_exact("trigger");
    m_isolation     = has_exact("isolation");
    m_isolationKinematics = has_exact("isolationKinematics");
    m_quality       = has_exact("quality");
    m_trackparams   = has_exact("trackparams");
    m_trackhitcont  = has_exact("trackhitcont");
    m_effSF         = has_exact("effSF");
    m_energyLoss    = has_exact("energyLoss");
    m_promptlepton  = has_exact("promptlepton");

    // working points combinations for trigger corrections
    std::string token;
    std::string reco_prfx = "Reco";
    std::string isol_prfx = "Iso";
    std::string trig_prfx = "HLT";

    std::istringstream ss(m_configStr);
    while ( std::getline(ss, token, ' ') ) {
      if ( token.compare( 0, reco_prfx.length(), reco_prfx ) == 0 ) { m_recoWPs.push_back(token); }
      if ( token.compare( 0, isol_prfx.length(), isol_prfx ) == 0 ) { m_isolWPs.push_back(token); }
      if ( token.compare( 0, trig_prfx.length(), trig_prfx ) == 0 ) { m_trigWPs.push_back(token); }
    }

    m_recoEff_sysNames = has_exact("recoEff_sysNames");
    m_isoEff_sysNames  = has_exact("isoEff_sysNames");
    m_trigEff_sysNames = has_exact("trigEff_sysNames");
    m_ttvaEff_sysNames = has_exact("ttvaEff_sysNames");

  }

  void ElectronInfoSwitch::initialize(){
    m_trigger       = has_exact("trigger");
    m_isolation     = has_exact("isolation");
    m_isolationKinematics = has_exact("isolationKinematics");
    m_quality       = has_exact("quality");
    m_PID           = has_exact("PID");
    m_trackparams   = has_exact("trackparams");
    m_trackhitcont  = has_exact("trackhitcont");
    m_effSF         = has_exact("effSF");
    m_promptlepton  = has_exact("promptlepton");
    // working points for scale-factors

    // working points combinations for trigger corrections
    std::string token;
    std::string pid_keyword = "PID_";
    std::string isol_keyword = "ISOL_";
    std::string trig_keyword = "TRIG_";

    std::istringstream ss(m_configStr);
    while ( std::getline(ss, token, ' ') ) {
      auto pid_substr = token.find(pid_keyword);
      auto isol_substr = token.find(isol_keyword);
      auto trig_substr = token.find(trig_keyword);
      if( pid_substr != std::string::npos ){
        m_PIDWPs.push_back(token.substr(4));
      } else if(isol_substr != std::string::npos){
        if(token.substr(5) == "NONE" || token == isol_keyword) m_isolWPs.push_back("");
        else m_isolWPs.push_back(token.substr(5));
      } else if(trig_substr != std::string::npos){
        m_trigWPs.push_back(token.substr(5));
      }
    }
  }

  void PhotonInfoSwitch::initialize(){
    m_isolation     = has_exact("isolation");
    m_PID           = has_exact("PID");
    m_purity        = has_exact("purity");
    m_effSF         = has_exact("effSF");
    m_trigger       = has_exact("trigger");
  }

  void JetInfoSwitch::initialize(){
    std::string tmpConfigStr; // temporary config string used to extract multiple values

    m_substructure  = has_exact("substructure");
    m_bosonCount    = has_exact("bosonCount");
    m_VTags         = has_exact("VTags");
    m_rapidity      = has_exact("rapidity");
    m_clean         = has_exact("clean");
    m_energy        = has_exact("energy");
    m_scales        = has_exact("scales");
    m_constscaleEta = has_exact("constscaleEta");
    m_resolution    = has_exact("resolution");
    m_truth         = has_exact("truth");
    m_truthDetails  = has_exact("truth_details");
    m_layer         = has_exact("layer");
    m_trackPV       = has_exact("trackPV");
    m_trackAll      = has_exact("trackAll");
    m_allTrack      = has_exact("allTrack");
    m_allTrackPVSel = has_exact("allTrackPVSel");
    m_allTrackDetail= has_exact("allTrackDetail");
    if( m_allTrackDetail ) {
      m_allTrackPVSel = m_allTrackPVSel || has_exact("allTrackDetailPVSel") ;
    }
    m_constituent       = has_exact("constituent");
    m_constituentAll    = has_exact("constituentAll");
    m_flavTag           = has_exact("flavorTag");
    m_flavTagHLT        = has_exact("flavorTagHLT");
    m_btag_jettrk       = has_exact("btag_jettrk");
    m_jetFitterDetails  = has_exact("jetFitterDetails");
    m_svDetails         = has_exact("svDetails");
    m_ipDetails         = has_exact("ipDetails");

    if(has_match("tracksInJet")){
      m_tracksInJet       = true;
      std::string input(m_configStr);
      // erase everything before the interesting string
      input.erase( 0, input.find("tracksInJet_") );
      // erase everything after the interesting string
      // only if there is something after the string
      if( input.find(" ") != std::string::npos ) {
        input.erase( input.find_first_of(" "), input.size() );
      }
      // remove tracksInJet_ to just leave the tack name
      input.erase(0,12);

      m_trackName = input;
    }else{
      m_tracksInJet       = false;
      m_trackName         = "";
    }


    m_trackJetNames.clear();
    if(has_match("trackJetName")){
      std::string input(m_configStr);
      // erase everything before the interesting string
      input.erase( 0, input.find("trackJetName") );
      input.erase( input.find(" "), std::string::npos );
      input.erase( 0, 13 );

      std::stringstream ss(input);
      std::string s;
      while(std::getline(ss, s, '_'))
	m_trackJetNames.push_back(s);
    }


    m_hltVtxComp          = has_exact("hltVtxComp");
    m_onlineBS            = has_exact("onlineBS");
    m_onlineBSTool        = has_exact("onlineBSTool");


    m_charge              = has_exact("charge");
    m_etaPhiMap           = has_exact("etaPhiMap");
    m_byAverageMu         = has_exact("byAverageMu");
    m_byEta               = has_exact("byEta");
    m_vsLumiBlock         = has_exact("vsLumiBlock");
    m_vsActualMu          = has_exact("vsActualMu");
    m_lumiB_runN          = has_exact("lumiB_runN");

    m_sfJVTName           = get_working_point("sfJVT");
    m_sffJVTName          = get_working_point("sffJVT");

    m_sfFTagFix.clear();
    if( has_match( "sfFTagFix" ) ) {
      std::string input(m_configStr);
      // erase everything before the interesting string
      input.erase( 0, input.find("sfFTagFix") );
      // erase everything after the interesting string
      // only if there is something after the string
      if( input.find(" ") != std::string::npos ) {
        input.erase( input.find_first_of(" "), input.size() );
      }
      // remove fTagSFFix to just leave the numbers
      input.erase(0,9);
      // two by two take the characters and push back an int into this vector
      std::vector<int> values;
      int size( input.size()/2 );
      int count(0);
      while( count < size ) {
        std::string number = input.substr(0,2);
        m_sfFTagFix.push_back( atoi( number.c_str() ) );
        input.erase(0,2);
        count++;
      }
    } // sfFTagFix
    m_sfFTagFlt.clear();
    if( has_match( "sfFTagFlt" ) ) {
      std::string input(m_configStr);
      // erase everything before the interesting string
      input.erase( 0, input.find("sfFTagFlt") );
      // erase everything after the interesting string
      // only if there is something after the string
      if( input.find(" ") != std::string::npos ) {
        input.erase( input.find_first_of(" "), input.size() );
      }
      // remove fTagSFFlt to just leave the numbers
      input.erase(0,9);
      // two by two take the characters and push back an int into this vector
      std::vector<int> values;
      int size( input.size()/2 );
      int count(0);
      while( count < size ) {
        std::string number = input.substr(0,2);
        m_sfFTagFlt.push_back( atoi( number.c_str() ) );
        input.erase(0,2);
        count++;
      }
    } // sfFTagFlt
    m_sfFTagHyb.clear();
    if( has_match( "sfFTagHyb" ) ) {
      std::string input(m_configStr);
      // erase everything before the interesting string
      input.erase( 0, input.find("sfFTagHyb") );
      // erase everything after the interesting string
      // only if there is something after the string
      if( input.find(" ") != std::string::npos ) {
        input.erase( input.find_first_of(" "), input.size() );
      }
      // remove fTagSFHyb to just leave the numbers
      input.erase(0,9);
      // two by two take the characters and push back an int into this vector
      std::vector<int> values;
      int size( input.size()/2 );
      int count(0);
      while( count < size ) {
        std::string number = input.substr(0,2);
        m_sfFTagHyb.push_back( atoi( number.c_str() ) );
        input.erase(0,2);
        count++;
      }
    } // sfFTagHyb

    m_jetBTag.clear();
    tmpConfigStr=std::string(m_configStr);
    while( tmpConfigStr.find("jetBTag") != std::string::npos ) { // jetBTag
      // erase everything before the interesting string
      tmpConfigStr.erase( 0, tmpConfigStr.find("jetBTag") );
      // extract interesting string
      std::size_t pos  =tmpConfigStr.find(" ");
      std::string input=tmpConfigStr.substr(0,pos);
      // remove interesting string from configStr being processed
      tmpConfigStr.erase(0,pos);
      // extracted the tagger and numbers
      std::stringstream ss(input);
      std::string s;
      uint idx=0;
      std::string tagger;
      std::string type;
      std::vector<uint> wps;
      while(std::getline(ss, s, '_')) {
	switch(idx)
	  {
	  case 0: // jetBTag
	    break;
	  case 1: // tagger
	    tagger=s;
	    break;
	  case 2: // efficiency type
	    type=s;
	    break;
	  case 3: // list of efficiency working points
	    uint size( s.size()/2 );
	    for(uint i=0;i<size;i++) {
	      std::string number = s.substr(0,2);
	      wps.push_back( atoi( number.c_str() ) );
	      s.erase(0,2);
	    }
	  }
	idx++;
      }
      if(m_jetBTag.find(tagger)==m_jetBTag.end()) m_jetBTag[tagger]=std::vector<std::pair<std::string,uint>>();
      for(auto wp : wps)
	m_jetBTag[tagger].push_back(std::make_pair(type,wp));
    } // jetBTag

    m_area          = has_exact("area");
    m_JVC           = has_exact("JVC");
  }

  void TruthInfoSwitch::initialize(){
    m_type          = has_exact("type");
    m_bVtx          = has_exact("bVtx");
    m_parents       = has_exact("parents");
    m_children      = has_exact("children");
  }

  void TrackInfoSwitch::initialize(){
    m_kinematic     = has_exact("kinematic");
    m_fitpars	    = has_exact("fitpars");
    m_numbers	    = has_exact("numbers");
    m_vertex	    = has_exact("vertex");
    m_useTheS       = has_exact("useTheS");
  }

  void TauInfoSwitch::initialize(){
    m_trackparams   = has_exact("trackparams");
    m_trackhitcont  = has_exact("trackhitcont");
  }

  void METInfoSwitch::initialize(){
    m_metClus   = has_exact("metClus");
    m_metTrk    = has_exact("metTrk");
    m_sigClus   = has_exact("sigClus")  || has_exact("all");
    m_sigTrk    = has_exact("sigTrk")   || has_exact("all");
    m_sigResolutionClus = has_exact("sigResolutionClus") || has_exact("all");
    m_sigResolutionTrk  = has_exact("sigResolutionTrk")  || has_exact("all");
    m_refEle    = has_exact("refEle")   || has_exact("all");
    m_refGamma  = has_exact("refGamma") || has_exact("all");
    m_refTau    = has_exact("refTau")   || has_exact("all");
    m_refMuons  = has_exact("refMuons") || has_exact("all");
    m_refJet    = has_exact("refJet")   || has_exact("all");
    m_refJetTrk = has_exact("refJetTrk"); // take this one *only* if requested by user explicitly
    m_softClus  = has_exact("softClus") || has_exact("all");
    m_softTrk   = has_exact("softTrk")  || has_exact("all");
  }

} // close namespace HelperClasses
