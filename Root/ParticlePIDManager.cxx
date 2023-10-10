#include "xAODAnaHelpers/ParticlePIDManager.h"

ANA_MSG_SOURCE(msgPIDManager, "PIDManager")

ElectronLHPIDManager :: ElectronLHPIDManager ( std::string WP, bool debug ) :
  m_asgElectronLikelihoodTool_VeryLoose(nullptr),
  m_asgElectronLikelihoodTool_Loose(nullptr),
  m_asgElectronLikelihoodTool_LooseBL(nullptr),
  m_asgElectronLikelihoodTool_Medium(nullptr),
  m_asgElectronLikelihoodTool_Tight(nullptr),
  m_asgElectronLikelihoodTool_VeryLooseLLP(nullptr),
  m_asgElectronLikelihoodTool_LooseLLP(nullptr),
  m_asgElectronLikelihoodTool_MediumLLP(nullptr),
  m_asgElectronLikelihoodTool_TightLLP(nullptr)

{
      m_selectedWP = WP;
      m_debug      = debug;

      /*  fill the multimap with WPs and corresponding tools */
      std::pair < std::string, AsgElectronLikelihoodTool* > veryloose   = std::make_pair( std::string("VeryLoose"), m_asgElectronLikelihoodTool_VeryLoose );
      std::pair < std::string, AsgElectronLikelihoodTool* > loose       = std::make_pair( std::string("Loose"),     m_asgElectronLikelihoodTool_Loose     );
      std::pair < std::string, AsgElectronLikelihoodTool* > loosebl     = std::make_pair( std::string("LooseBL"),     m_asgElectronLikelihoodTool_LooseBL     );
      std::pair < std::string, AsgElectronLikelihoodTool* > medium      = std::make_pair( std::string("Medium"),    m_asgElectronLikelihoodTool_Medium    );
      std::pair < std::string, AsgElectronLikelihoodTool* > tight       = std::make_pair( std::string("Tight"),     m_asgElectronLikelihoodTool_Tight     );

      std::pair < std::string, AsgElectronLikelihoodTool* > verylooseLLP   = std::make_pair( std::string("VeryLooseLLP"), m_asgElectronLikelihoodTool_VeryLooseLLP );
      std::pair < std::string, AsgElectronLikelihoodTool* > looseLLP       = std::make_pair( std::string("LooseLLP"),     m_asgElectronLikelihoodTool_LooseLLP     );
      std::pair < std::string, AsgElectronLikelihoodTool* > mediumLLP      = std::make_pair( std::string("MediumLLP"),    m_asgElectronLikelihoodTool_MediumLLP    );
      std::pair < std::string, AsgElectronLikelihoodTool* > tightLLP       = std::make_pair( std::string("TightLLP"),     m_asgElectronLikelihoodTool_TightLLP     );
      
      m_allWPTools.insert(veryloose);   m_allWPAuxDecors.insert("VeryLoose");
      m_allWPTools.insert(loose);       m_allWPAuxDecors.insert("Loose");
      m_allWPTools.insert(loosebl);     m_allWPAuxDecors.insert("LooseBL");
      m_allWPTools.insert(medium);      m_allWPAuxDecors.insert("Medium");
      m_allWPTools.insert(tight);       m_allWPAuxDecors.insert("Tight");

      m_allWPTools.insert(verylooseLLP);   m_allWPAuxDecors.insert("VeryLooseNoPix");
      m_allWPTools.insert(looseLLP);       m_allWPAuxDecors.insert("LooseNoPix");
      m_allWPTools.insert(mediumLLP);      m_allWPAuxDecors.insert("MediumNoPix");
      m_allWPTools.insert(tightLLP);       m_allWPAuxDecors.insert("TightNoPix");
}

ElectronLHPIDManager ::  ~ElectronLHPIDManager()
{
  if ( m_asgElectronLikelihoodTool_VeryLoose )   { delete m_asgElectronLikelihoodTool_VeryLoose; m_asgElectronLikelihoodTool_VeryLoose = nullptr; }
  if ( m_asgElectronLikelihoodTool_Loose )       { delete m_asgElectronLikelihoodTool_Loose;     m_asgElectronLikelihoodTool_Loose     = nullptr; }
  if ( m_asgElectronLikelihoodTool_LooseBL )     { delete m_asgElectronLikelihoodTool_LooseBL;   m_asgElectronLikelihoodTool_LooseBL   = nullptr; }
  if ( m_asgElectronLikelihoodTool_Medium )      { delete m_asgElectronLikelihoodTool_Medium;    m_asgElectronLikelihoodTool_Medium    = nullptr; }
  if ( m_asgElectronLikelihoodTool_Tight )       { delete m_asgElectronLikelihoodTool_Tight;     m_asgElectronLikelihoodTool_Tight     = nullptr; }

  if ( m_asgElectronLikelihoodTool_VeryLooseLLP )   { delete m_asgElectronLikelihoodTool_VeryLooseLLP; m_asgElectronLikelihoodTool_VeryLooseLLP = nullptr; }
  if ( m_asgElectronLikelihoodTool_LooseLLP )       { delete m_asgElectronLikelihoodTool_LooseLLP;     m_asgElectronLikelihoodTool_LooseLLP     = nullptr; }
  if ( m_asgElectronLikelihoodTool_MediumLLP )      { delete m_asgElectronLikelihoodTool_MediumLLP;    m_asgElectronLikelihoodTool_MediumLLP    = nullptr; }
  if ( m_asgElectronLikelihoodTool_TightLLP )       { delete m_asgElectronLikelihoodTool_TightLLP;     m_asgElectronLikelihoodTool_TightLLP     = nullptr; }

}


StatusCode ElectronLHPIDManager :: setupWPs( bool configTools, std::string selector_name) {
  using namespace msgPIDManager;
  std::string selectedWP = m_selectedWP;
  if (m_selectedWP.find("NoPix") != std::string::npos) selectedWP = m_selectedWP.substr(0, m_selectedWP.size()-5) + "LLP";
  HelperClasses::EnumParser<LikeEnum::Menu> selectedWP_parser;
  unsigned int selectedWP_enum = static_cast<unsigned int>( selectedWP_parser.parseEnum(selectedWP) );

  /* By converting the string to the corresponding LikeEnum, we can exploit the ordering of the enum itself
  / ( see ElectronPhotonID/ElectronPhotonSelectorTools/trunk/ElectronPhotonSelectorTools/TElectronLikelihoodTool.h for definition)
  / to initialise ONLY the tools with WP tighter (or equal) the selected one.
  / The selected WP will be used to cut loose electrons in the selector, the tighter WPs to decorate!
  Beware, this doesn't work if you are using LLP WPs since they are not ordered in the enum.
  */
  if ( configTools ) {
    for ( auto it : (m_allWPTools) ) {

      /* instantiate tools (do it for all) */

      const std::string WP            = it.first;
      std::string tool_name = selector_name + "_" + WP;

      Info("setupWPs()", "Initializing AsgElectronLikelihoodTool w/ name: %s", tool_name.c_str() );

      it.second =  new AsgElectronLikelihoodTool( tool_name.c_str() );

      HelperClasses::EnumParser<LikeEnum::Menu>  WP_parser;
      unsigned int WP_enum = static_cast<unsigned int>( WP_parser.parseEnum(WP) );

      /* if this WP is looser than user's WP, skip to next. Don't do this if using LLP WPs. */
      if ( WP_enum < selectedWP_enum && (selectedWP.find("LLP") == std::string::npos) ) { continue; }

      /* configure and initialise only tools with (WP >= selectedWP) */
      it.second->msg().setLevel( MSG::INFO); /* ERROR, VERBOSE, DEBUG, INFO */

      std::string WPParseString = WP + "LHElectron";
      if (WP.find("LLP") != std::string::npos) WPParseString = WP.substr(0, WP.size()-3) + "LHElectron_LLP";

      ANA_CHECK( it.second->setProperty("WorkingPoint", WPParseString ) );
      ANA_CHECK( it.second->initialize());

      /* copy map element into container of valid WPs for later usage */
      m_validWPTools.insert( it );

    }
  } else {
    for ( auto it : (m_allWPAuxDecors) ) {

      HelperClasses::EnumParser<LikeEnum::Menu>  WP_parser;
      std::string WPParseString = it;
      if (it.find("NoPix") != std::string::npos) WPParseString = it.substr(0, it.size()-5) + "LLP";
      unsigned int WP_enum = static_cast<unsigned int>( WP_parser.parseEnum(WPParseString) );

      /* if this WP is looser than user's WP, skip to next. Don't do this if using LLP WPs. */
      if ( WP_enum < selectedWP_enum && (selectedWP.find("LLP") == std::string::npos) ) { continue; }

      /* copy map element into container of valid WPs for later usage */
      m_validWPs.insert( it );

    }

  }
  return StatusCode::SUCCESS;

}

/* set default values for decorations (do it for all WPs) */
StatusCode ElectronLHPIDManager :: setDecorations( const xAOD::Electron* electron ) {

      for ( auto it : (m_allWPTools) ) {
        const std::string defaultDecorWP =  "LH" + it.first;
        electron->auxdecor<char>(defaultDecorWP) = -1;
      }

      return StatusCode::SUCCESS;
}

const std::string ElectronLHPIDManager :: getSelectedWP () {

  const std::string WP = m_selectedWP;
  return WP;

}

ElectronCutBasedPIDManager :: ElectronCutBasedPIDManager ( std::string WP, bool debug ) :
  m_asgElectronIsEMSelector_Loose(nullptr),
  m_asgElectronIsEMSelector_Medium(nullptr),
  m_asgElectronIsEMSelector_Tight(nullptr)
{
  m_selectedWP = WP;
  m_debug      = debug;

  /*  fill the multimap with WPs and corresponding tools. Use an ordered index to reflect the tightness order (0: loosest WP, ...) */
  std::pair < std::string, AsgElectronIsEMSelector* > loose = std::make_pair( std::string("Loose"), m_asgElectronIsEMSelector_Loose );
  m_allWPTools.insert( loose );
  m_allWPAuxDecors.insert("Loose");
  std::pair < std::string, AsgElectronIsEMSelector* > medium = std::make_pair( std::string("Medium"), m_asgElectronIsEMSelector_Medium );
  m_allWPTools.insert( medium );
  m_allWPAuxDecors.insert("Medium");
  std::pair < std::string, AsgElectronIsEMSelector* > tight = std::make_pair( std::string("Tight"), m_asgElectronIsEMSelector_Tight );
  m_allWPTools.insert( tight );
  m_allWPAuxDecors.insert("Tight");
}

ElectronCutBasedPIDManager ::  ~ElectronCutBasedPIDManager() {
  if ( m_asgElectronIsEMSelector_Loose )     { m_asgElectronIsEMSelector_Loose = nullptr;  delete m_asgElectronIsEMSelector_Loose;  }
  if ( m_asgElectronIsEMSelector_Medium )    { m_asgElectronIsEMSelector_Medium = nullptr; delete m_asgElectronIsEMSelector_Medium; }
  if ( m_asgElectronIsEMSelector_Tight )     { m_asgElectronIsEMSelector_Tight = nullptr;  delete m_asgElectronIsEMSelector_Tight;  }
}

StatusCode ElectronCutBasedPIDManager :: setupWPs( bool configTools, std::string selector_name) {
  using namespace msgPIDManager;
  HelperClasses::EnumParser<egammaPID::egammaIDQuality> selectedWP_parser;
  unsigned int selectedWP_enum = static_cast<unsigned int>( selectedWP_parser.parseEnum(m_selectedWP) );

  /* By converting the string to the corresponding egammaPID, we can exploit the ordering of the enum itself
  / ( see ElectronPhotonID/ElectronPhotonSelectorTools/trunk/ElectronPhotonSelectorTools/TElectronIsEMSelector.h for definition)
  / to initialise ONLY the tools with WP tighter (or equal) the selected one.
  / The selected WP will be used to cut loose electrons in the selector, the tighter WPs to decorate!
  /
  / egammaPID enums :http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Reconstruction/egamma/egammaEvent/egammaEvent/egammaPIDdefs.h
  */
  if ( configTools ) {

    for ( auto it : (m_allWPTools) ) {

      const std::string WP            = it.first;
      /* instantiate tools (do it for all) */
      std::string tool_name = WP + selector_name;
      it.second =  new AsgElectronIsEMSelector( tool_name.c_str() );

      HelperClasses::EnumParser<egammaPID::egammaIDQuality>  itWP_parser;
      unsigned int itWP_enum = static_cast<unsigned int>( itWP_parser.parseEnum(WP) );

      /* if this WP is looser than user's WP, skip to next */
      if ( itWP_enum < selectedWP_enum ) { continue; }

      /* configure and initialise only tools with (WP >= selectedWP) */

      it.second->msg().setLevel( MSG::INFO); /* ERROR, VERBOSE, DEBUG, INFO */

      ANA_CHECK( it.second->setProperty("WorkingPoint", WP+"Electron" ) );
      ANA_CHECK( it.second->initialize());

      /* copy map element into container of valid tools for later usage */
      m_validWPTools.insert( it );

    }

  } else {

    for ( auto it : (m_allWPAuxDecors) ) {

      HelperClasses::EnumParser<egammaPID::egammaIDQuality>  itWP_parser;
      unsigned int itWP_enum = static_cast<unsigned int>( itWP_parser.parseEnum(it) );

      /* if this WP is looser than user's WP, skip to next */
      if ( itWP_enum < selectedWP_enum ) { continue; }

      /* copy map element into container of valid WPs for later usage */
      m_validWPs.insert( it );

    }

  }

  return StatusCode::SUCCESS;
}

/* set default values for decorations (do it for all WPs) */
StatusCode ElectronCutBasedPIDManager :: setDecorations( const xAOD::Electron* electron ) {
  for ( auto it : (m_allWPTools) ) {
    std::string defaultDecorWP = "IsEM"+it.first;
    electron->auxdecor<char>(defaultDecorWP) = -1;
  }

  return StatusCode::SUCCESS;
}
