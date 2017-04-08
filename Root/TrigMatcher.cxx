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
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include <xAODAnaHelpers/TrigMatcher.h>

// ROOT include(s):
#include <TFile.h>
#include <TObjArray.h>
#include <TObjString.h>


// this is needed to distribute the algorithm to the workers
ClassImp(TrigMatcher)

TrigMatcher::TrigMatcher(const std::string& className)
  : Algorithm(className),
  m_trigMatchTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("TrigMatcher()", "Calling constructor");

  m_debug = false;

  // input container to be read from TEvent or TStore
  //
  m_inContainerName = "";

  // Systematics stuff
  //
  m_systNames = "";


  // trigger matching stuff
  //
  m_trigChains = "";
}

TrigMatcher::~TrigMatcher() {}

EL::StatusCode TrigMatcher::setupJob(EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob");

  job.useXAOD();
  xAOD::Init("TrigMatcher").ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher::initialize()
{
  Info("initialize()", "Initializing TrigMatcher Interface... ");

  RETURN_CHECK("xAH::Algorithm::algInitialize()",
               xAH::Algorithm::algInitialize(), "");

  // Standard containers
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // Configuration
  if (m_inContainerName.empty()) {
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  // ***************************************
  //
  // Initialise Trig::TrigMatchingTool
  //
  // ***************************************
  if (!m_trigChains.empty()) {
    std::string trig;
    std::istringstream ss(m_trigChains);

    while (std::getline(ss, trig, ',')) {
      m_trigChainsList.push_back(trig);
    }
  }

  //  everything went fine, let's initialise the tool!
  //
  m_trigMatchTool = new Trig::MatchingTool("TrigMatchTool_" + m_name);

  if (m_debug) ANA_CHECK(m_trigMatchTool->setProperty("OutputLevel", MSG::DEBUG));


  // **********************************************************************************************

  Info("initialize()", "TrigMatcher Interface succesfully initialized!");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher::execute()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if (m_debug) Info("execute()", "Applying trigger matching... ");
  const xAOD::IParticleContainer *inParticles(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  //
  if (m_systNames.empty()) {
    // this will be the collection processed - no matter what!!
    //
    RETURN_CHECK("TrigMatcher::execute()",
                 HelperFunctions::retrieve(inParticles, m_inContainerName,
                                           m_event, m_store,
                                           m_verbose), "");
    ANA_CHECK(executeMatching(inParticles));
  } else { // get the list of systematics to run over
    // get vector of string giving the syst names of the upstream algo from
    // TStore (rememeber: 1st element is a blank string: nominal case!)
    //
    std::vector<std::string> *systNames(nullptr);
    RETURN_CHECK("TrigMatcher::execute()",
                 HelperFunctions::retrieve(systNames, m_systNames, 0, m_store,
                                           m_verbose), "");

    // loop over systematic sets
    //
    for (auto systName : *systNames) {
      if (m_debug) Info("execute()",
                        " syst name: %s  input container name: %s ",
                        systName.c_str(),
                        (m_inContainerName + systName).c_str());
      RETURN_CHECK("TrigMatcher::execute()",
                   HelperFunctions::retrieve(inParticles,
                                             m_inContainerName + systName,
                                             m_event, m_store, m_verbose), "");
      ANA_CHECK(executeMatching(inParticles));
    }
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher::executeMatching(
  const xAOD::IParticleContainer *inParticles)
{
  static const SG::AuxElement::Decorator<std::vector<std::string> >
  isTrigMatchedDecor("trigMatched");

  for (auto particle : *inParticles)
  {
    if (m_debug) Info("executeMatching()", "Trigger matching an object");

    if (!isTrigMatchedDecor.isAvailable(*particle)) isTrigMatchedDecor(*particle)
        = std::vector<std::string>();

    for (auto const& chain : m_trigChainsList) {
      if (m_debug) Info("executeMatching()",
                        "\t checking trigger chain %s",
                        chain.c_str());
      bool matched = m_trigMatchTool->match(*particle, chain, 0.07);

      if (m_debug) Info("executeMatching()", "\t\t result = %d", matched);

      if (matched) isTrigMatchedDecor(*particle).push_back(chain);
    }
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrigMatcher::finalize()
{
  Info("finalize()", "Cleaning up...");

  if (m_trigMatchTool) { delete m_trigMatchTool; m_trigMatchTool = nullptr; }

  return EL::StatusCode::SUCCESS;
}
