/********************************************
 *
 * Interface to CP trigger matching tool(s).
 *
 *******************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include <xAODEventInfo/EventInfo.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODAnaHelpers/TrigMatcher.h>

// ROOT include(s):
#include <TFile.h>
#include <TObjArray.h>
#include <TObjString.h>


// this is needed to distribute the algorithm to the workers
ClassImp(TrigMatcher)

TrigMatcher :: TrigMatcher ()
: Algorithm("TrigMatcher")
{
}

TrigMatcher::~TrigMatcher() {}

EL::StatusCode TrigMatcher :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_INFO( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "TrigMatcher" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher :: initialize ()
{
  ANA_MSG_INFO( "Initializing TrigMatcher Interface... ");

  ANA_CHECK( xAH::Algorithm::algInitialize());

  // Standard containers
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // Configuration
  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
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
  m_trigMatchTool = new Trig::MatchingTool("TrigMatchTool_"+m_name);
  ANA_CHECK( m_trigMatchTool->setProperty( "OutputLevel", msg().level()) );

  // **********************************************************************************************

  ANA_MSG_INFO( "TrigMatcher Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher :: execute ()
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

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher :: executeMatching ( const xAOD::IParticleContainer* inParticles )
{
  static const SG::AuxElement::Decorator< std::vector< std::string > > isTrigMatchedDecor( "trigMatched" );

  for( auto particle : *inParticles )
    {
      ANA_MSG_DEBUG( "Trigger matching an object");

      if ( !isTrigMatchedDecor.isAvailable( *particle ) )
	isTrigMatchedDecor( *particle ) = std::vector<std::string>();

      for ( auto const &chain : m_trigChainsList ) {
	ANA_MSG_DEBUG( "\t checking trigger chain " << chain);

	bool matched = m_trigMatchTool->match( *particle, chain, 0.07 );
	ANA_MSG_DEBUG( "\t\t result = " << matched );
	if(matched) isTrigMatchedDecor( *particle ).push_back( chain );
      }
    }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher :: finalize ()
{
  ANA_MSG_INFO( "Cleaning up...");
  if(m_trigMatchTool) { delete m_trigMatchTool; m_trigMatchTool=nullptr; }

  return EL::StatusCode::SUCCESS;
}
