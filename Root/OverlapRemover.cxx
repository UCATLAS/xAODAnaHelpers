/******************************************
 *
 * Interface to OverlapRemoval tool (applying recommendations from Harmonisation TF).
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 *
 ******************************************/

// c++ include(s):
#include <iostream>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/Photon.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/Jet.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTau/TauJet.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/OverlapRemover.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(OverlapRemover)


OverlapRemover :: OverlapRemover () :
  m_usePhotons(false),
  m_useTaus(false),
  m_overlapRemovalTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("OverlapRemover()", "Calling constructor");

  // read debug flag from .config file
  m_debug         = false;
  // input container(s) to be read from TEvent or TStore

  /* Muons */
  m_inContainerName_Muons       = "";
  m_inputAlgoMuons              = "";  // name of vector<string> of syst retrieved from TStore
  m_outputAlgoMuons             = "MuonCollection_OR_Algo";    // name of vector<string> of syst pushed in TStore
  /* Electrons */
  m_inContainerName_Electrons   = "";
  m_inputAlgoElectrons          = "";  // name of vector<string> of syst retrieved from TStore
  m_outputAlgoElectrons         = "ElectronCollection_OR_Algo";    // name of vector<string> of syst pushed in TStore
  /* Jets */
  m_inContainerName_Jets        = "";
  m_inputAlgoJets               = "";  // name of vector<string> of syst retrieved from TStore
  m_outputAlgoJets              = "JetCollection_OR_Algo";    // name of vector<string> of syst pushed in TStore
  /* Photons */
  m_inContainerName_Photons     = "";
  /* Taus */
  m_inContainerName_Taus        = "";

  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects     = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::View_Element option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainers    = false;

  m_useSelected = false;

  m_outContainerName_Electrons  = "";

  m_outContainerName_Muons      = "";

  m_outContainerName_Jets       = "";

  m_outContainerName_Photons    = "";

  m_outContainerName_Taus       = "";

}

EL::StatusCode  OverlapRemover :: configure ()
{

  if ( !getConfig().empty() ) {

    Info("configure()", "Configuing OverlapRemover Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug         = config->GetValue("Debug" , m_debug);
    // input container(s) to be read from TEvent or TStore

    /* Muons */
    m_inContainerName_Muons       = config->GetValue("InputContainerMuons",  m_inContainerName_Muons.c_str());
    m_inputAlgoMuons              = config->GetValue("InputAlgoMuons",  m_inputAlgoMuons.c_str());  // name of vector<string> of syst retrieved from TStore
    m_outputAlgoMuons             = config->GetValue("OutputAlgoMuons", m_outputAlgoMuons.c_str());    // name of vector<string> of syst pushed in TStore
    /* Electrons */
    m_inContainerName_Electrons   = config->GetValue("InputContainerElectrons",  m_inContainerName_Electrons.c_str());
    m_inputAlgoElectrons          = config->GetValue("InputAlgoElectrons",  m_inputAlgoElectrons.c_str());  // name of vector<string> of syst retrieved from TStore
    m_outputAlgoElectrons         = config->GetValue("OutputAlgoElectrons", m_outputAlgoElectrons.c_str());    // name of vector<string> of syst pushed in TStore
    /* Jets */
    m_inContainerName_Jets        = config->GetValue("InputContainerJets",  m_inContainerName_Jets.c_str());
    m_inputAlgoJets               = config->GetValue("InputAlgoJets",  m_inputAlgoJets.c_str());  // name of vector<string> of syst retrieved from TStore
    m_outputAlgoJets              = config->GetValue("OutputAlgoJets", m_outputAlgoJets.c_str());    // name of vector<string> of syst pushed in TStore
    /* Photons */
    m_inContainerName_Photons     = config->GetValue("InputContainerPhotons",  m_inContainerName_Photons.c_str());
    /* Taus */
    m_inContainerName_Taus        = config->GetValue("InputContainerTaus",  m_inContainerName_Taus.c_str());

    // decorate selected objects that pass the cuts
    m_decorateSelectedObjects     = config->GetValue("DecorateSelectedObjects", m_decorateSelectedObjects);
    // additional functionality : create output container of selected objects
    //                            using the SG::View_Element option
    //                            decorating and output container should not be mutually exclusive
    m_createSelectedContainers    = config->GetValue("CreateSelectedContainers", m_createSelectedContainers);

    m_useSelected = config->GetValue("UseSelected", m_useSelected);

    m_outContainerName_Electrons  = config->GetValue("OutputContainerElectrons", m_outContainerName_Electrons.c_str());

    m_outContainerName_Muons      = config->GetValue("OutputContainerMuons", m_outContainerName_Muons.c_str());

    m_outContainerName_Jets       = config->GetValue("OutputContainerJets", m_outContainerName_Jets.c_str());

    m_outContainerName_Photons    = config->GetValue("OutputContainerPhotons", m_outContainerName_Photons.c_str());

    m_outContainerName_Taus       = config->GetValue("OutputContainerTaus", m_outContainerName_Taus.c_str());

    config->Print();
    Info("configure()", "OverlapRemover Interface succesfully configured! ");

    delete config; config = nullptr;
  }

  if ( m_inContainerName_Muons.empty() ) {
    Error("configure()", "InputContainerMuons is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  if ( m_inContainerName_Electrons.empty() ) {
    Error("configure()", "InputContainerElectrons is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  if ( m_inContainerName_Jets.empty() ) {
    Error("configure()", "InputContainerJets is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }

  // be more flexible w/ photons and taus :)
  if ( !m_inContainerName_Photons.empty() ) { m_usePhotons = true; }
  if ( !m_inContainerName_Taus.empty() )    { m_useTaus = true; }
  m_outAuxContainerName_Electrons   = m_outContainerName_Electrons + "Aux."; // the period is very important!
  m_outAuxContainerName_Muons       = m_outContainerName_Muons + "Aux.";     // the period is very important!
  m_outAuxContainerName_Jets        = m_outContainerName_Jets + "Aux.";      // the period is very important!
  m_outAuxContainerName_Photons     = m_outContainerName_Photons + "Aux.";   // the period is very important!
  m_outAuxContainerName_Taus        = m_outContainerName_Taus + "Aux.";      // the period is very important!

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "OverlapRemover" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing OverlapRemover Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize overlap removal tool
  std::string or_tool_name = std::string("OverlapRemovalTool_") + m_name;
  m_overlapRemovalTool = new OverlapRemovalTool( or_tool_name.c_str() );
  m_overlapRemovalTool->msg().setLevel( MSG::INFO ); // VERBOSE, INFO, DEBUG

  // set input object "selection" decoration
  const std::string selected_label = ( m_useSelected ) ? "passSel" : "";  // set with decoration flag you use for selected objects if want to consider only selected objects in OR, otherwise it will perform OR on all objects
  RETURN_CHECK( "OverlapRemover::initialize()", m_overlapRemovalTool->setProperty("InputLabel",  selected_label), "");
  //RETURN_CHECK( "OverlapRemover::initialize()", m_overlapRemovalTool->setProperty("OverlapLabel", "overlaps"), ""); // tool will decorate objects with 'overlaps' boolean if they overlap (not possible to customise name atm!)
  RETURN_CHECK( "OverlapRemover::initialize()", m_overlapRemovalTool->initialize(), "Failed to properly initialize the OverlapRemovalTool.");

  Info("initialize()", "OverlapRemover Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Overlap Removal... "); }

  m_numEvent++;

  // get the collections from TEvent or TStore
  const xAOD::ElectronContainer* inElectrons (nullptr);
  const xAOD::MuonContainer* inMuons         (nullptr);
  const xAOD::JetContainer* inJets           (nullptr);
  const xAOD::PhotonContainer* inPhotons     (nullptr);
  const xAOD::TauJetContainer* inTaus        (nullptr);

  // --------------------------------------------------------------------------------------------
  //
  // always run the nominal case

  executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus, NOMINAL);

  // look what do we have in TStore
  if ( m_verbose ) { m_store->print(); }

  // --------------------------------------------------------------------------------------------
  //
  // if at least one of the m_inputAlgo* is not empty, then loop over its relative systematic set

  // ****************** //
  //      Electrons     //
  // ****************** //

  if ( !m_inputAlgoElectrons.empty() ) {

    // -----------------------
    //
    // get the systematic sets:

    // get vector of string giving the syst names (rememeber: 1st element is a blank string: nominal case!)
    std::vector<std::string>* systNames_el(nullptr);
    RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(systNames_el, m_inputAlgoElectrons, 0, m_store, m_verbose) ,"");

    executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus,  ELSYST, systNames_el);

  } // end of if ( !m_inputAlgoElectrons.empty() )


  // **************** //
  //      Muons       //
  // **************** //

  if ( !m_inputAlgoMuons.empty() ) {

    // -----------------------
    //
    // get the systematic sets:

    // get vector of string giving the syst names (rememeber: 1st element is a blank string: nominal case!)
    std::vector<std::string>* systNames_mu(nullptr);
    RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(systNames_mu, m_inputAlgoMuons, 0, m_store, m_verbose) ,"");

    executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus,  MUSYST, systNames_mu);

  }  // end of if ( !m_inputAlgoMuons.empty() )


  // **************** //
  //       Jets       //
  // **************** //

  if ( !m_inputAlgoJets.empty() ) {

    // -----------------------
    //
    // get the systematic sets:

    // get vector of string giving the syst names (rememeber: 1st element is a blank string: nominal case!)
    std::vector<std::string>* systNames_jet;
    RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(systNames_jet, m_inputAlgoJets, 0, m_store, m_verbose) ,"");

    executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus,  JETSYST, systNames_jet);

  }  // end of if ( !m_inputAlgoJets.empty() )

  // **************** //
  //     Photons      //
  // **************** //

  // **************** //
  //       Taus       //
  // **************** //

  // look what do we have in TStore
  if ( m_verbose ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode OverlapRemover :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  Info("finalize()", "Deleting tool instances...");

  if ( m_overlapRemovalTool ){ delete m_overlapRemovalTool; m_overlapRemovalTool = nullptr; }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.

  Info("histFinalize()", "Calling histFinalize");

  return EL::StatusCode::SUCCESS;
}


// Print object information

EL::StatusCode OverlapRemover :: printOverlapInfo (const char* type, const xAOD::IParticleContainer* objCont, const std::string& selectFlag, const std::string& overlapFlag)
{
  static SG::AuxElement::ConstAccessor<char> selectAcc(selectFlag);
  static SG::AuxElement::ConstAccessor<char> overlapAcc(overlapFlag);
  static SG::AuxElement::ConstAccessor< ElementLink<xAOD::IParticleContainer> > objLinkAcc("overlapObject");

  for ( auto obj_itr : *(objCont) ) {

    // Safety check
    if ( !overlapAcc.isAvailable( *obj_itr ) ) {
      Error("printObj()", "Overlap decoration missing for object of type %s ", type );
      return EL::StatusCode::FAILURE;
    }
    if ( selectAcc.isAvailable( *obj_itr) ){
      Info("printObj()", "  %s pt %6.2f eta %5.2f phi %5.2f selected %i overlaps %i ", type, (obj_itr)->pt()/1000., (obj_itr)->eta(), (obj_itr)->phi(), selectAcc( *obj_itr ), overlapAcc( *obj_itr ) );
    } else {
      Info("printObj()", "  %s pt %6.2f eta %5.2f phi %5.2f overlaps %i ", type, (obj_itr)->pt()/1000., (obj_itr)->eta(), (obj_itr)->phi(), overlapAcc( *obj_itr) );
    }
    // Check for overlap object link
    if ( objLinkAcc.isAvailable( *obj_itr ) && objLinkAcc( *obj_itr ).isValid() ) {
      const xAOD::IParticle* overlapObj = *objLinkAcc( *obj_itr );
      std::stringstream ss_or; ss_or << overlapObj->type();
      Info("printObj()", "    Overlap: type %s pt %6.2f", (ss_or.str()).c_str(), overlapObj->pt()/1e3);
    }
  }

  return EL::StatusCode::SUCCESS;

}


EL::StatusCode OverlapRemover :: executeOR(  const xAOD::ElectronContainer* inElectrons, const xAOD::MuonContainer* inMuons, const xAOD::JetContainer* inJets,
					     const xAOD::PhotonContainer* inPhotons,   const xAOD::TauJetContainer* inTaus,
					     SystType syst_type, std::vector<std::string>* sysVec)
{

  // instantiate output container(s)
  ConstDataVector<xAOD::ElectronContainer> *selectedElectrons   (nullptr);
  ConstDataVector<xAOD::MuonContainer>     *selectedMuons	(nullptr);
  ConstDataVector<xAOD::JetContainer>      *selectedJets	(nullptr);
  ConstDataVector<xAOD::PhotonContainer>   *selectedPhotons	(nullptr);
  ConstDataVector<xAOD::TauJetContainer>   *selectedTaus	(nullptr);


  // make a switch for systematics types
  switch ( static_cast<int>(syst_type) )
  {

    case(0) :  // this is the nominal case
    {
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, m_event, m_store, m_verbose) ,"");
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, m_event, m_store, m_verbose) ,"");
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, m_event, m_store, m_verbose) ,"");
      if ( m_usePhotons )  RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, m_event, m_store, m_verbose) ,"");
      if ( m_useTaus )     RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, m_event, m_store, m_verbose) ,"");

      if ( m_debug ) { Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size());  }

      std::string ORdecor = std::string("overlaps");

      RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->setProperty( "OverlapLabel", ORdecor.c_str() ), "Failed to set property OverlapLabel" );

      // do the actual OR
      RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

      // debug : check that something has been done
      if ( m_debug ) {
        // electrons
        Info("executeConst()", "Now dumping the electrons");
        printOverlapInfo("electron", inElectrons, "passSel", "overlaps");
        // muons
        Info("executeConst()", "Now dumping the muons");
        printOverlapInfo("muon", inMuons, "passSel", "overlaps");
        // jets
        Info("executeConst()",  "Now dumping the jets");
        printOverlapInfo("jet", inJets, "passSel", "overlaps");
      }

      // make a copy of input container(s) with selected objects
      if ( m_createSelectedContainers ) {
        selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
        selectedMuons	    = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
        selectedJets	    = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
        if ( m_usePhotons )  selectedPhotons	= new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
        if ( m_useTaus )     selectedTaus	= new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
      }

      // resize containers basd on OR decision
      RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
      RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
      RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
      if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }
      if ( m_useTaus )   { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }

      if ( m_debug ) { Info("execute()",  "selectedElectrons : %lu, selectedMuons : %lu, selectedJets : %lu", selectedElectrons->size(), selectedMuons->size(),  selectedJets->size()); }

      // add ConstDataVector to TStore
      if ( m_createSelectedContainers ) {
        RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons ), "Failed to store const data container");
        RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,	 m_outContainerName_Muons ), "Failed to store const data container");
        RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,	 m_outContainerName_Jets ), "Failed to store const data container");
        if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons ), "Failed to store const data container"); }
        if ( m_useTaus )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus ), "Failed to store const data container"); }
      }

      break;
    }
    case (1) : // electron syst
    {
      // prepare a vector of the names of CDV containers
      // must be a pointer to be recorded in TStore
      // for now just copy the one you just retrieved in it!
      std::vector< std::string >* vecOutContainerNames_el = new std::vector< std::string >(*sysVec);
      // just to check everything is fine
      if ( m_debug ) {
           Info("execute()","output vector already contains the following ELECTRON systematics:" );
           for ( auto it : *vecOutContainerNames_el ) {	Info("execute()" ,"\t %s ", it.c_str()); }
      }

      // these input containers won't change in the electron syst loop ...
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, m_event, m_store, m_verbose) ,"");
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, m_event, m_store, m_verbose) ,"");
      if ( m_usePhotons )  RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, m_event, m_store, m_verbose) ,"");
      if ( m_useTaus )     RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, m_event, m_store, m_verbose) ,"");

      for ( auto systName : *sysVec) {

	if ( systName.empty() ) continue;

        // ... instead, the electron input container will be different for each syst
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons + systName, m_event, m_store, m_verbose) ,"");
        if ( m_debug ) { Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size());  }

        std::string ORdecor = std::string("overlaps");
        // prepends syst name to decoration
        std::string prepend = systName + "_";
        ORdecor.insert( 0, prepend );

        RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->setProperty( "OverlapLabel", ORdecor.c_str() ), "Failed to set property OverlapLabel" );

        // do the actual OR
        RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

        // debug : check that something has been done
        if ( m_debug ) {
          // electrons
          Info("executeConst()", "Now dumping the electrons");
          printOverlapInfo("electron", inElectrons, "passSel", ORdecor.c_str());
          // muons
          Info("executeConst()", "Now dumping the muons");
          printOverlapInfo("muon", inMuons, "passSel", ORdecor.c_str());
          // jets
          Info("executeConst()",  "Now dumping the jets");
          printOverlapInfo("jet", inJets, "passSel", ORdecor.c_str());
        }

        // make a copy of input container(s) with selected objects
        if ( m_createSelectedContainers ) {
          selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          selectedMuons	      = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets	      = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          if ( m_usePhotons )  selectedPhotons	= new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          if ( m_useTaus )     selectedTaus	= new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers basd on OR decision
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }
        if ( m_useTaus )   {	RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }

        // add ConstDataVector to TStore
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons + systName ), "Failed to store const data container"); }
          if ( m_useTaus )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container"); }
        }
      } // close loop on systematic sets available from upstream algo (Electrons)

      // add vector<string container_names_syst> to TStore
      RETURN_CHECK( "execute()", m_store->record( vecOutContainerNames_el, m_outputAlgoMuons ), "Failed to record vector of output container names.");
      break;
    }
    case (2) : // muon syst
    {
      // prepare a vector of the names of CDV containers
      // must be a pointer to be recorded in TStore
      // for now just copy the one you just retrieved in it!
      std::vector< std::string >* vecOutContainerNames_mu = new std::vector< std::string >(*sysVec);
      // just to check everything is fine
      if ( m_debug ) {
         Info("execute()","output vector already contains MUON systematics:" );
         for (auto it : *vecOutContainerNames_mu){	Info("execute()" ,"\t %s ", it.c_str()); }
      }

      // these input containers won't change in the muon syst loop ...
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, m_event, m_store, m_verbose) ,"");
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, m_event, m_store, m_verbose) ,"");
      if ( m_usePhotons )  RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, m_event, m_store, m_verbose) ,"");
      if ( m_useTaus )     RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, m_event, m_store, m_verbose) ,"");

      for ( auto systName : *sysVec) {

	if ( systName.empty() ) continue;

	// ... instead, the muon input container will be different for each syst
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons + systName, m_event, m_store, m_verbose) ,"");
        if ( m_debug ) { Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size());  }

        std::string ORdecor = std::string("overlaps");
        // prepends syst name to decoration
        std::string prepend = systName + "_";
        ORdecor.insert( 0, prepend );

        RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->setProperty( "OverlapLabel", ORdecor.c_str() ), "Failed to set property OverlapLabel" );

        // do the actual OR
        RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

        // debug : check that something has been done
        if ( m_debug ) {
          // electrons
          Info("executeConst()", "Now dumping the electrons");
          printOverlapInfo("electron", inElectrons, "passSel", ORdecor.c_str());
          // muons
          Info("executeConst()", "Now dumping the muons");
          printOverlapInfo("muon", inMuons, "passSel", ORdecor.c_str());
          // jets
          Info("executeConst()",  "Now dumping the jets");
          printOverlapInfo("jet", inJets, "passSel", ORdecor.c_str());
        }

        // make a copy of input container(s) with selected objects
        if ( m_createSelectedContainers ) {
          selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          selectedMuons	      = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets	      = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          if ( m_usePhotons )  selectedPhotons	= new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          if ( m_useTaus )     selectedTaus	= new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers based on OR decision
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }
        if ( m_useTaus )   {	RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }

        // add ConstDataVector to TStore
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons + systName ), "Failed to store const data container"); }
          if ( m_useTaus )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container"); }
        }

      } // close loop on systematic sets available from upstream algo (Muons)

      // add vector<string container_names_syst> to TStore
      RETURN_CHECK( "execute()", m_store->record( vecOutContainerNames_mu, m_outputAlgoMuons ), "Failed to record vector of output container names.");

      break;
    }
    case (3) : // jet systematics
    {
      // prepare a vector of the names of CDV containers
      // must be a pointer to be recorded in TStore
      // for now just copy the one you just retrieved in it!
      std::vector< std::string >* vecOutContainerNames_jet = new std::vector< std::string >(*sysVec);
      // just to check everything is fine
      if ( m_debug ) {
        Info("execute()","output vector already contains the following JET systematics:" );
        for ( auto it : *vecOutContainerNames_jet) { Info("execute()" ,"\t %s ", it.c_str());  }
      }

      // these input containers won't change in the jet syst loop ...
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, m_event, m_store, m_verbose) ,"");
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, m_event, m_store, m_verbose) ,"");
      if ( m_usePhotons )  RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, m_event, m_store, m_verbose) ,"");
      if ( m_useTaus )     RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, m_event, m_store, m_verbose) ,"");

      for( auto systName : *sysVec ) {

	 if ( systName.empty() ) continue;

	 // ... instead, the jet input container will be different for each syst
         RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets + systName, m_event, m_store, m_verbose) ,"");
         if ( m_debug ) { Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size());  }

         std::string ORdecor = std::string("overlaps");
         // prepends syst name to decoration
         std::string prepend = systName + "_";
         ORdecor.insert( 0, prepend );

         RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->setProperty( "OverlapLabel", ORdecor.c_str() ), "Failed to set property OverlapLabel" );

         // do the actual OR
         RETURN_CHECK( "OverlapRemover::execute()", m_overlapRemovalTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

         // debug : check that something has been done
         if ( m_debug ) {
           // electrons
           Info("executeConst()", "Now dumping the electrons");
           printOverlapInfo("electron", inElectrons, "passSel", ORdecor.c_str());
           // muons
           Info("executeConst()", "Now dumping the muons");
           printOverlapInfo("muon", inMuons, "passSel", ORdecor.c_str());
           // jets
           Info("executeConst()",  "Now dumping the jets");
           printOverlapInfo("jet", inJets, "passSel", ORdecor.c_str());
         }

        // make a copy of input container(s) with selected objects
        if ( m_createSelectedContainers ) {
          selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          selectedMuons	      = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets	      = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          if ( m_usePhotons )  selectedPhotons	= new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          if ( m_useTaus )     selectedTaus	= new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers basd on OR decision
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::OVERLAPREMOVER), "");
        if ( m_usePhotons ) { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }
        if ( m_useTaus )    { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::OVERLAPREMOVER), ""); }

        // add ConstDataVector to TStore
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons + systName ), "Failed to store const data container"); }
          if ( m_useTaus )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container"); }
        }
      } // close loop on systematic sets available from upstream algo (Jets)

      // add vector<string container_names_syst> to TStore
      RETURN_CHECK( "OverlapRemover::execute()", m_store->record( vecOutContainerNames_jet, m_outputAlgoJets ), "Failed to record vector of output container names.");

      break;
    }
    case(4) : // photon systematics
    {
      break;
    }
    case 5 : // tau systematics
    {
      break;
    }
    default :
    {
      Error("OverlapRemover::execute()","Unknown systematics type. Aborting");
      return EL::StatusCode::FAILURE;
    }
  } // end of switch

  return EL::StatusCode::SUCCESS;

}
