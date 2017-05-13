#include "xAODAnaHelpers/ParticlePIDManager.h"

ANA_MSG_SOURCE(msgPIDManager, "PIDManager")

ElectronLHPIDManager :: ElectronLHPIDManager ( std::string WP, bool debug ) :
  m_asgElectronLikelihoodTool_VeryLoose(nullptr),
  m_asgElectronLikelihoodTool_Loose(nullptr),
  m_asgElectronLikelihoodTool_LooseBL(nullptr),
  m_asgElectronLikelihoodTool_Medium(nullptr),
  m_asgElectronLikelihoodTool_Tight(nullptr)
{
      m_selectedWP = WP;
      m_debug      = debug;

      /*  fill the multimap with WPs and corresponding tools */
      std::pair < std::string, AsgElectronLikelihoodTool* > veryloose = std::make_pair( std::string("VeryLoose"), m_asgElectronLikelihoodTool_VeryLoose );
      std::pair < std::string, AsgElectronLikelihoodTool* > loose     = std::make_pair( std::string("Loose"),     m_asgElectronLikelihoodTool_Loose     );
      std::pair < std::string, AsgElectronLikelihoodTool* > loosebl   = std::make_pair( std::string("LooseBL"),     m_asgElectronLikelihoodTool_LooseBL     );
      std::pair < std::string, AsgElectronLikelihoodTool* > medium    = std::make_pair( std::string("Medium"),    m_asgElectronLikelihoodTool_Medium    );
      std::pair < std::string, AsgElectronLikelihoodTool* > tight     = std::make_pair( std::string("Tight"),     m_asgElectronLikelihoodTool_Tight     );
      m_allWPTools.insert(veryloose); m_allWPAuxDecors.insert("VeryLoose");
      m_allWPTools.insert(loose);     m_allWPAuxDecors.insert("Loose");
      m_allWPTools.insert(loosebl);   m_allWPAuxDecors.insert("Loose"); //Not saved in DAODs, so use Loose decision
      m_allWPTools.insert(medium);    m_allWPAuxDecors.insert("Medium");
      m_allWPTools.insert(tight);     m_allWPAuxDecors.insert("Tight");
}

ElectronLHPIDManager ::  ~ElectronLHPIDManager()
{
  if ( m_asgElectronLikelihoodTool_VeryLoose ) { delete m_asgElectronLikelihoodTool_VeryLoose; m_asgElectronLikelihoodTool_VeryLoose = nullptr; }
  if ( m_asgElectronLikelihoodTool_Loose )     { delete m_asgElectronLikelihoodTool_Loose;     m_asgElectronLikelihoodTool_Loose     = nullptr; }
  if ( m_asgElectronLikelihoodTool_LooseBL )   { delete m_asgElectronLikelihoodTool_LooseBL;   m_asgElectronLikelihoodTool_LooseBL   = nullptr; }
  if ( m_asgElectronLikelihoodTool_Medium )    { delete m_asgElectronLikelihoodTool_Medium;    m_asgElectronLikelihoodTool_Medium    = nullptr; }
  if ( m_asgElectronLikelihoodTool_Tight )     { delete m_asgElectronLikelihoodTool_Tight;     m_asgElectronLikelihoodTool_Tight     = nullptr; }
}


StatusCode ElectronLHPIDManager :: setupWPs( bool configTools, std::string selector_name) {
  using namespace msgPIDManager;
  const std::string selectedWP = ( m_selectedWP == "LooseBL" ) ? "Loose" : m_selectedWP;
  HelperClasses::EnumParser<LikeEnum::Menu> selectedWP_parser;
  unsigned int selectedWP_enum = static_cast<unsigned int>( selectedWP_parser.parseEnum(selectedWP) );

  /* By converting the string to the corresponding LikeEnum, we can exploit the ordering of the enum itself
  / ( see ElectronPhotonID/ElectronPhotonSelectorTools/trunk/ElectronPhotonSelectorTools/TElectronLikelihoodTool.h for definition)
  / to initialise ONLY the tools with WP tighter (or equal) the selected one.
  / The selected WP will be used to cut loose electrons in the selector, the tighter WPs to decorate!
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

      /* if this WP is looser than user's WP, skip to next */
      if ( WP_enum < selectedWP_enum ) { continue; }

      /* configure and initialise only tools with (WP >= selectedWP) */
      it.second->msg().setLevel( MSG::INFO); /* ERROR, VERBOSE, DEBUG, INFO */

      ANA_CHECK( it.second->setProperty("WorkingPoint", WP+"LHElectron" ) );
      ANA_CHECK( it.second->initialize());

      /* copy map element into container of valid WPs for later usage */
      m_validWPTools.insert( it );

    }
  } else {
    for ( auto it : (m_allWPAuxDecors) ) {

      HelperClasses::EnumParser<LikeEnum::Menu>  WP_parser;
      unsigned int WP_enum = static_cast<unsigned int>( WP_parser.parseEnum(it) );

      /* if this WP is looser than user's WP, skip to next */
      if ( WP_enum < selectedWP_enum ) { continue; }

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

  const std::string WP = ( m_selectedWP == "LooseAndBLayer" ) ? "Loose" : m_selectedWP;
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
