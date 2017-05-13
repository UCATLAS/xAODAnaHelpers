#ifndef xAODAnaHelpers_ParticlePIDManager_H
#define xAODAnaHelpers_ParticlePIDManager_H

// package include(s):
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <AsgTools/MessageCheck.h>
#include "AsgTools/StatusCode.h"

#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/AsgElectronIsEMSelector.h"
#include "ElectronPhotonSelectorTools/egammaPIDdefs.h"
#include "ElectronPhotonSelectorTools/LikelihoodEnums.h"

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"

// ROOT include(s):
#include "TObject.h"

// C++ include(s)
#include <string>

ANA_MSG_HEADER(msgPIDManager)

class ElectronLHPIDManager
{
  public:
    ElectronLHPIDManager ();
    ElectronLHPIDManager ( std::string WP, bool debug = false );
    ~ElectronLHPIDManager();

    StatusCode setupWPs( bool configTools, std::string selector_name = "" );

    /* set default values for decorations (do it for all WPs) */
    StatusCode setDecorations( const xAOD::Electron* electron );

    const std::string getSelectedWP ();

    /* returns a map containing all the tools */
    std::multimap< std::string, AsgElectronLikelihoodTool* > getAllWPTools()   { return m_allWPTools; };
    /* returns a map containing only the tools w/ (WP >= selected WP) */
    std::multimap< std::string, AsgElectronLikelihoodTool* > getValidWPTools() { return m_validWPTools; };
    /* returns a string containing all the WPs */
    const std::set<std::string>  getAllWPs()   { return m_allWPAuxDecors; };
    /* returns a string containing only the WPs >= selected WP */
    const std::set<std::string>  getValidWPs() { return m_validWPs; };

  private:

    std::string m_selectedWP;
    bool        m_debug;
    std::multimap<std::string, AsgElectronLikelihoodTool*> m_allWPTools;
    std::multimap<std::string, AsgElectronLikelihoodTool*> m_validWPTools;
    std::set<std::string> m_allWPAuxDecors;
    std::set<std::string> m_validWPs;

    AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_VeryLoose;
    AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Loose;
    AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_LooseBL;
    AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Medium;
    AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Tight;

};

class ElectronCutBasedPIDManager
{
  public:
    ElectronCutBasedPIDManager ();
    ElectronCutBasedPIDManager ( std::string WP, bool debug = false );
    ~ElectronCutBasedPIDManager();

    StatusCode setupWPs( bool configTools, std::string selector_name = "" );

    /* set default values for decorations (do it for all WPs) */
    StatusCode setDecorations( const xAOD::Electron* electron );

    const std::string getSelectedWP ( ) { return m_selectedWP; }

    /* returns a map containing all the tools */
    std::multimap< std::string, AsgElectronIsEMSelector* > getAllWPTools() { return m_allWPTools; };
    /* returns a map containing only the tools w/ (WP >= selected WP) */
    std::multimap< std::string, AsgElectronIsEMSelector* > getValidWPTools() { return m_validWPTools; };
    /* returns a string containing all the WPs */
    const std::set<std::string>  getAllWPs()   { return m_allWPAuxDecors; };
    /* returns a string containing only the WPs >= selected WP */
    const std::set<std::string>  getValidWPs() { return m_validWPs; };

  private:

    std::string m_selectedWP;
    bool        m_debug;

    std::multimap<std::string, AsgElectronIsEMSelector*> m_allWPTools;
    std::multimap<std::string, AsgElectronIsEMSelector*> m_validWPTools;
    std::set<std::string> m_allWPAuxDecors;
    std::set<std::string> m_validWPs;

    AsgElectronIsEMSelector*  m_asgElectronIsEMSelector_Loose;
    AsgElectronIsEMSelector*  m_asgElectronIsEMSelector_Medium;
    AsgElectronIsEMSelector*  m_asgElectronIsEMSelector_Tight;

};

#endif


