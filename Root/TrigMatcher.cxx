/********************************************
 *
 * Interface to CP trigger matching tool(s).
 *
 *******************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>


// EDM include(s):
#include <xAODEventInfo/EventInfo.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODAnaHelpers/TrigMatcher.h>

#include "TriggerMatchingTool/MatchFromCompositeTool.h"

// ROOT include(s):
#include <TFile.h>
#include <TObjArray.h>
#include <TObjString.h>


// this is needed to distribute the algorithm to the workers

TrigMatcher :: TrigMatcher (const std::string& name, ISvcLocator *pSvcLocator)
: Algorithm(name, pSvcLocator, "TrigMatcher")
{
    declareProperty("inContainerName", m_inContainerName);
    declareProperty("systNames", m_systNames);
    declareProperty("trigChains", m_trigChains);
}

StatusCode TrigMatcher :: initialize ()
{
  ANA_MSG_INFO( "Initializing TrigMatcher Interface... ");

  ANA_CHECK( xAH::Algorithm::algInitialize());

  // Standard containers
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // Configuration
  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return StatusCode::FAILURE;
  }

  if( !isPHYS() ) {
    // Grab the TrigDecTool from the ToolStore
    if(!m_trigDecTool_handle.isUserConfigured()){
      ANA_MSG_FATAL("A configured " << m_trigDecTool_handle.typeAndName() << " must have been previously created! Are you creating one in xAH::BasicEventSelection?" );
      return StatusCode::FAILURE;
    }
    ANA_CHECK( m_trigDecTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);
  }

  // ***************************************
  //
  // Initialise Trig::TrigMatchingTool
  //
  // ***************************************
  if( !m_trigChains.empty() ) {
    std::string trig;
    std::istringstream ss(m_trigChains);
    while ( std::getline(ss, trig, ',') ) {
      m_trigChainsList.push_back(trig);
    }
  }

  //  everything went fine, let's initialise the tool!
  //
  if( !isPHYS() ) {
    m_trigMatchTool_handle = asg::AnaToolHandle<Trig::IMatchingTool>("Trig::MatchingTool/MatchingTool");
    ANA_CHECK( m_trigMatchTool_handle.setProperty( "TrigDecisionTool", m_trigDecTool_handle ));
  } else { // For DAOD_PHYS samples
    m_trigMatchTool_handle = asg::AnaToolHandle<Trig::IMatchingTool>("Trig::MatchFromCompositeTool/MatchFromCompositeTool");
  }
  ANA_CHECK( m_trigMatchTool_handle.retrieve() );

  // **********************************************************************************************

  ANA_MSG_INFO( "TrigMatcher Interface succesfully initialized!" );

  return StatusCode::SUCCESS;
}

StatusCode TrigMatcher :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG( "Applying trigger matching... ");

  const xAOD::IParticleContainer* inParticles(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  //
  if ( m_systNames.empty() ) {

    // this will be the collection processed - no matter what!!
    //
    ANA_CHECK( HelperFunctions::retrieve(inParticles, m_inContainerName, m_event, m_store, msg()) );
    ANA_CHECK( executeMatching( inParticles ) );

  } else { // get the list of systematics to run over

    // get vector of string giving the syst names of the upstream algo from TStore (rememeber: 1st element is a blank string: nominal case!)
    //
    std::vector< std::string >* systNames(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_systNames, 0, m_store, msg()) );

    // loop over systematic sets
    //
    for ( auto systName : *systNames) {

      ANA_MSG_DEBUG( " syst name: " << systName << "  input container name:  " << m_inContainerName+systName );

      ANA_CHECK( HelperFunctions::retrieve(inParticles, m_inContainerName + systName, m_event, m_store, msg()));
      ANA_CHECK( executeMatching( inParticles ) );
    }

  }

  return StatusCode::SUCCESS;
}

StatusCode TrigMatcher :: executeMatching ( const xAOD::IParticleContainer* inParticles )
{
  static const SG::AuxElement::Decorator< std::vector< std::string > > isTrigMatchedDecor( "trigMatched" );

  for( auto particle : *inParticles )
    {
      ANA_MSG_DEBUG( "Trigger matching an object");

      if ( !isTrigMatchedDecor.isAvailable( *particle ) )
	isTrigMatchedDecor( *particle ) = std::vector<std::string>();

      for ( auto const &chain : m_trigChainsList ) {
	ANA_MSG_DEBUG( "\t checking trigger chain " << chain);

	bool matched = m_trigMatchTool_handle->match( *particle, chain, 0.07 );
	ANA_MSG_DEBUG( "\t\t result = " << matched );
	if(matched) isTrigMatchedDecor( *particle ).push_back( chain );
      }
    }

  return StatusCode::SUCCESS;
}
