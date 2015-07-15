#ifndef xAODAnaHelpers_ParticlePIDManager_H
#define xAODAnaHelpers_ParticlePIDManager_H

// package include(s):
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "AsgTools/StatusCode.h"

#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronPhotonSelectorTools/AsgElectronIsEMSelector.h"
#include "ElectronPhotonSelectorTools/egammaPIDdefs.h"

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"

// ROOT include(s):
#include "TObject.h"

// C++ include(s)
#include <string>

class ElectronLHPIDManager
{
   public: 
     ElectronLHPIDManager ();
     ElectronLHPIDManager ( std::string WP, bool debug = false ) :
        m_asgElectronLikelihoodTool_VeryLoose(nullptr),
	m_asgElectronLikelihoodTool_Loose(nullptr),    
	m_asgElectronLikelihoodTool_Medium(nullptr),   
	m_asgElectronLikelihoodTool_Tight(nullptr)    
     {
	m_selectedWP = WP;
	m_debug      = debug;
        
        /*  fill the multimap with WPs and corresponding tools */
	std::pair < std::string, AsgElectronLikelihoodTool* > veryloose = std::make_pair( std::string("VeryLoose"), m_asgElectronLikelihoodTool_VeryLoose );
        m_allWPs.insert(veryloose);
	std::pair < std::string, AsgElectronLikelihoodTool* > loose = std::make_pair( std::string("Loose"), m_asgElectronLikelihoodTool_Loose );
        m_allWPs.insert(loose);
	std::pair < std::string, AsgElectronLikelihoodTool* > medium = std::make_pair( std::string("Medium"), m_asgElectronLikelihoodTool_Medium );
        m_allWPs.insert(medium);
	std::pair < std::string, AsgElectronLikelihoodTool* > tight = std::make_pair( std::string("Tight"), m_asgElectronLikelihoodTool_Tight );
        m_allWPs.insert(tight);
     };
     
     ~ElectronLHPIDManager()
     {
     	if ( m_asgElectronLikelihoodTool_VeryLoose ) { m_asgElectronLikelihoodTool_VeryLoose = nullptr; delete m_asgElectronLikelihoodTool_VeryLoose; }
     	if ( m_asgElectronLikelihoodTool_Loose )     { m_asgElectronLikelihoodTool_Loose = nullptr;	delete m_asgElectronLikelihoodTool_Loose;     }
     	if ( m_asgElectronLikelihoodTool_Medium )    { m_asgElectronLikelihoodTool_Medium = nullptr;	delete m_asgElectronLikelihoodTool_Medium;    }
     	if ( m_asgElectronLikelihoodTool_Tight )     { m_asgElectronLikelihoodTool_Tight = nullptr;	delete m_asgElectronLikelihoodTool_Tight;     }
     };
     
     
     StatusCode setupTools( std::string selector_name, std::string confDir, std::string year ) {
     
        HelperClasses::EnumParser<LikeEnum::Menu> selectedWP_parser;
        unsigned int selectedWP_enum = static_cast<unsigned int>( selectedWP_parser.parseEnum(m_selectedWP) );
       
        /*  
	/
	/ By converting the string to the corresponding LikeEnum, we can exploit the ordering of the enum itself 
	/ ( see ElectronPhotonID/ElectronPhotonSelectorTools/trunk/ElectronPhotonSelectorTools/TElectronLikelihoodTool.h for definition)
	/ to initialise ONLY the tools with WP tighter (or equal) the selected one.
	/ The selected WP will be used to cut loose electrons in the selector, the tighter WPs to decorate! 
	/
	*/       
	for ( auto it : (m_allWPs) ) {

	    /* instantiate tools (do it for all) */
	    
	    std::string tool_name = it.first + selector_name;
	    it.second =  new AsgElectronLikelihoodTool( tool_name.c_str() );
	    
            HelperClasses::EnumParser<LikeEnum::Menu>  itWP_parser;
            unsigned int itWP_enum = static_cast<unsigned int>( itWP_parser.parseEnum(it.first) );
            
            /* if this WP is looser than user's WP, skip to next */
            if ( itWP_enum < selectedWP_enum ) { continue; }
        
            /* configure and initialise only tools with (WP >= selectedWP) */
            it.second->msg().setLevel( MSG::INFO); /* ERROR, VERBOSE, DEBUG, INFO */
	    RETURN_CHECK( "ParticlePIDManager::setupTools()", it.second->setProperty("primaryVertexContainer", "PrimaryVertices"), "Failed to set primaryVertexContainer property");
	    std::string config_string = confDir + "ElectronLikelihood" + it.first + "OfflineConfig" + year + ".conf";
	    
	    Info("setupTools()", "Configuration file for LH tool: %s", config_string.c_str() ); 
	    
            RETURN_CHECK( "ParticlePIDManager::setupTools()", it.second->setProperty("ConfigFile", config_string ), "Failed to set ConfigFile property");
	    RETURN_CHECK( "ParticlePIDManager::setupTools()", it.second->initialize(), "Failed to initialize tool." );
            
	    /* copy map element into container of valid tools for later usage */
	    m_validWPs.insert( it );
	    
        }
	
	return StatusCode::SUCCESS;
     
     };
     
     /* set default values for decorations (do it for all WPs) */
     StatusCode setDecorations( const xAOD::Electron* electron ) {
       
       for ( auto it : (m_allWPs) ) { 
         const std::string defaultDecorWP =  "LH" + it.first;  
         electron->auxdecor<char>(defaultDecorWP) = -1;
       }

       return StatusCode::SUCCESS;
     }  
     
     const std::string getSelectedWP ( ) { return m_selectedWP; }  
	
     /* returns a map containing all the tools  */
     std::multimap< std::string, AsgElectronLikelihoodTool* > getTools() { return m_allWPs; };
     
     /* returns a map containing only the tools w/ (WP >= selected WP) */
     std::multimap< std::string, AsgElectronLikelihoodTool* > getValidTools() { return m_validWPs; };
     
   private:   
     
     std::string m_selectedWP;
     bool        m_debug;
     std::multimap<std::string, AsgElectronLikelihoodTool*> m_allWPs;
     std::multimap<std::string, AsgElectronLikelihoodTool*> m_validWPs;
     

     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_VeryLoose;  
     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Loose;	 
     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Medium;	 
     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Tight;	 

};

class ElectronCutBasedPIDManager
{
   public: 
     ElectronCutBasedPIDManager ();
     ElectronCutBasedPIDManager ( std::string WP, bool debug = false ) :
	m_asgElectronIsEMSelector_Loose(nullptr),    
	m_asgElectronIsEMSelector_Medium(nullptr),   
	m_asgElectronIsEMSelector_Tight(nullptr)    
     {
	m_selectedWP = WP;
	m_debug      = debug;
	
        /*  fill the multimap with WPs and corresponding tools. Use an ordered index to reflect the tightness order (0: loosest WP, ...) */
	std::pair < std::string, AsgElectronIsEMSelector* > loose = std::make_pair( std::string("IsEMLoose"), m_asgElectronIsEMSelector_Loose );
        m_allWPs.insert( loose );
	std::pair < std::string, AsgElectronIsEMSelector* > medium = std::make_pair( std::string("IsEMMedium"), m_asgElectronIsEMSelector_Medium );
        m_allWPs.insert( medium );
	std::pair < std::string, AsgElectronIsEMSelector* > tight = std::make_pair( std::string("IsEMTight"), m_asgElectronIsEMSelector_Tight );
        m_allWPs.insert( tight );
     };
     
     ~ElectronCutBasedPIDManager()
     {
     	if ( m_asgElectronIsEMSelector_Loose )     { m_asgElectronIsEMSelector_Loose = nullptr;  delete m_asgElectronIsEMSelector_Loose;  }
     	if ( m_asgElectronIsEMSelector_Medium )    { m_asgElectronIsEMSelector_Medium = nullptr; delete m_asgElectronIsEMSelector_Medium; }
     	if ( m_asgElectronIsEMSelector_Tight )     { m_asgElectronIsEMSelector_Tight = nullptr;  delete m_asgElectronIsEMSelector_Tight;  }
     };
     
     
     StatusCode setupTools( std::string selector_name, std::string confDir, std::string year ) {
     
        HelperClasses::EnumParser<egammaPID::PID> selectedWP_parser;
        unsigned int selectedWP_enum = static_cast<unsigned int>( selectedWP_parser.parseEnum(m_selectedWP) );
       
        /*  
	/
	/ By converting the string to the corresponding egammaPID, we can exploit the ordering of the enum itself 
	/ ( see ElectronPhotonID/ElectronPhotonSelectorTools/trunk/ElectronPhotonSelectorTools/TElectronIsEMSelector.h for definition)
	/ to initialise ONLY the tools with WP tighter (or equal) the selected one.
	/ The selected WP will be used to cut loose electrons in the selector, the tighter WPs to decorate! 
	/
	/ egammaPID enums :http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Reconstruction/egamma/egammaEvent/egammaEvent/egammaPIDdefs.h
	/
	*/       
	
	for ( auto it : (m_allWPs) ) {

	    /* instantiate tools (do it for all) */
	    std::string tool_name = it.first + selector_name;
	    it.second =  new AsgElectronIsEMSelector( tool_name.c_str() );
           
            HelperClasses::EnumParser<egammaPID::PID>  itWP_parser;
            unsigned int itWP_enum = static_cast<unsigned int>( itWP_parser.parseEnum(it.first) );
            
            /* if this WP is looser than user's WP, skip to next */
            if ( itWP_enum < selectedWP_enum ) { continue; }

            /* configure and initialise only tools with (WP >= selectedWP) */

            it.second->msg().setLevel( MSG::INFO); /* ERROR, VERBOSE, DEBUG, INFO */	    
	    std::string config_string = confDir + "Electron" + it.first + "SelectorCutDefs" + year + ".conf";
	    
	    Info("setupTools()", "Configuration file for cut-based tool: %s", config_string.c_str() ); 
	    
            RETURN_CHECK( "ParticlePIDManager::setupTools()", it.second->setProperty("ConfigFile", config_string ), "Failed to set ConfigFile property");
    	    
	    /* set the bitmask only for samples with 2012 config */
    	    if ( year == "2012" )  {
    	      unsigned int EMMask = 999;
    	      if ( (it.first).find("IsEMLoose") != std::string::npos ) {
    		EMMask = egammaPID::ElectronLoosePP;
    	      } else if ( (it.first).find("IsEMMedium") != std::string::npos ) {
    		EMMask = egammaPID::ElectronMediumPP;
    	      } else if ( (it.first).find("IsEMTight") != std::string::npos ) {
    		EMMask = egammaPID::ElectronTightPP;
    	      } else {
    		Error("setupTools()", "Unavailable electron cut-based PID bitmask for this operating point!");
    		return EL::StatusCode::FAILURE;
    	      }
    	      RETURN_CHECK( "ParticlePIDManager::setupTools()", it.second->setProperty("isEMMask", EMMask ), "Failed to set isEMMask property");
    	    }
            RETURN_CHECK( "ParticlePIDManager::setupTools()", it.second->initialize(), "Failed to initialize tool." );
            
	    /* copy map element into container of valid tools for later usage */
	    m_validWPs.insert( it );
	    
        }
	
	return StatusCode::SUCCESS;
     
     };
     
     /* set default values for decorations (do it for all WPs) */
     StatusCode setDecorations( const xAOD::Electron* electron ) {
       
       for ( auto it : (m_allWPs) ) { 
         std::string defaultDecorWP = it.first;  
	 defaultDecorWP.erase(0,4);
         electron->auxdecor<char>(defaultDecorWP) = -1;
       }
       
       return StatusCode::SUCCESS;
	
     }

     const std::string getSelectedWP ( ) { return m_selectedWP; }  
     
     /* returns a map containing all the tools */
     std::multimap< std::string, AsgElectronIsEMSelector* > getTools() { return m_allWPs; };
    
     /* returns a map containing only the tools w/ (WP >= selected WP) */
     std::multimap< std::string, AsgElectronIsEMSelector* > getValidTools() { return m_validWPs; };
     
   private:   
     
     std::string m_selectedWP;
     bool        m_debug;
     
     std::multimap<std::string, AsgElectronIsEMSelector*> m_allWPs;
     std::multimap<std::string, AsgElectronIsEMSelector*> m_validWPs;

     AsgElectronIsEMSelector*  m_asgElectronIsEMSelector_Loose;	 
     AsgElectronIsEMSelector*  m_asgElectronIsEMSelector_Medium;	 
     AsgElectronIsEMSelector*  m_asgElectronIsEMSelector_Tight;	 

};

#endif


