#ifndef xAODAnaHelpers_ParticlePIDManager_H
#define xAODAnaHelpers_ParticlePIDManager_H

// package include(s):
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "AsgTools/StatusCode.h"

#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"


class LikelihoodPIDManager
{
   public: 
     LikelihoodPIDManager ();
     LikelihoodPIDManager (std::string WP) :
        m_asgElectronLikelihoodTool_VeryLoose(nullptr),
	m_asgElectronLikelihoodTool_Loose(nullptr),    
	m_asgElectronLikelihoodTool_Medium(nullptr),   
	m_asgElectronLikelihoodTool_Tight(nullptr),    
	m_asgElectronLikelihoodTool_VeryTight(nullptr)
     {
	m_selectedWP = WP;
        
        /*  fill the multimap with WPs and corresponding tools */
	std::pair < std::string, AsgElectronLikelihoodTool* > veryloose = std::make_pair( std::string("VeryLoose"), m_asgElectronLikelihoodTool_VeryLoose );
        m_allWPs.insert(veryloose);
	std::pair < std::string, AsgElectronLikelihoodTool* > loose = std::make_pair( std::string("Loose"), m_asgElectronLikelihoodTool_Loose );
        m_allWPs.insert(loose);
	std::pair < std::string, AsgElectronLikelihoodTool* > medium = std::make_pair( std::string("Medium"), m_asgElectronLikelihoodTool_Medium );
        m_allWPs.insert(medium);
	std::pair < std::string, AsgElectronLikelihoodTool* > tight = std::make_pair( std::string("Tight"), m_asgElectronLikelihoodTool_Tight );
        m_allWPs.insert(tight);
	std::pair < std::string, AsgElectronLikelihoodTool* > verytight = std::make_pair( std::string("VeryTight"), m_asgElectronLikelihoodTool_VeryTight );
        m_allWPs.insert(verytight);

     };
     
     ~LikelihoodPIDManager()
     {
     	if ( m_asgElectronLikelihoodTool_VeryLoose ) { delete m_asgElectronLikelihoodTool_VeryLoose; m_asgElectronLikelihoodTool_VeryLoose = nullptr; }
     	if ( m_asgElectronLikelihoodTool_Loose )     { delete m_asgElectronLikelihoodTool_Loose; m_asgElectronLikelihoodTool_Loose = nullptr; }
     	if ( m_asgElectronLikelihoodTool_Medium )    { delete m_asgElectronLikelihoodTool_Medium; m_asgElectronLikelihoodTool_Medium = nullptr; }
     	if ( m_asgElectronLikelihoodTool_Tight )     { delete m_asgElectronLikelihoodTool_Tight; m_asgElectronLikelihoodTool_Tight = nullptr; }
     	if ( m_asgElectronLikelihoodTool_VeryTight ) { delete m_asgElectronLikelihoodTool_VeryTight; m_asgElectronLikelihoodTool_VeryTight = nullptr; }       
     };
     
     
     StatusCode setupLHTools( std::string confDir, std::string year ) {
     
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
            it.second =  new AsgElectronLikelihoodTool( (it.first).c_str() );
            
            HelperClasses::EnumParser<LikeEnum::Menu>  itWP_parser;
            unsigned int itWP_enum = static_cast<unsigned int>( itWP_parser.parseEnum(it.first) );
            
            /* if this WP is looser than user's WP, skip to next */
            if ( itWP_enum < selectedWP_enum ) { continue; }
        
            /* configure and initialise only tools with (WP >= selectedWP) */
            it.second->msg().setLevel( MSG::INFO); /* ERROR, VERBOSE, DEBUG, INFO */
	    RETURN_CHECK( "ParticlePIDManager::setupLHTools()", it.second->setProperty("primaryVertexContainer", "PrimaryVertices"), "Failed to set primaryVertexContainer property");
	    std::string config_string = confDir + "ElectronLikelihood" + it.first + "OfflineConfig" + year + ".conf";
            RETURN_CHECK( "ParticlePIDManager::setupLHTools()", it.second->setProperty("ConfigFile", config_string ), "Failed to set ConfigFile property");
	    RETURN_CHECK( "ParticlePIDManager::setupLHTools()", it.second->initialize(), "Failed to initialize tool." );
            
	    /* copy map element into container of valid tools for later usage */
	    m_validWPs.insert( it );
	    
        }
	
	return StatusCode::SUCCESS;
     
     };
     
     /* returns a map containing only the tools w/ (WP >= selected WP) */
     std::multimap< std::string, AsgElectronLikelihoodTool* > getValidLHTools() { return m_validWPs; };
     
   private:   
     
     std::string m_selectedWP;
     std::multimap<std::string, AsgElectronLikelihoodTool*> m_allWPs;
     std::multimap<std::string, AsgElectronLikelihoodTool*> m_validWPs;
     

     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_VeryLoose;  
     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Loose;	 
     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Medium;	 
     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_Tight;	 
     AsgElectronLikelihoodTool*  m_asgElectronLikelihoodTool_VeryTight;  

};

#endif
