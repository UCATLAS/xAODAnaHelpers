/*************************************************
 *
 * Interface to ASG overlap removal tool
 ( applying recommendations from Harmonisation TF ).
 *
 * M. Milesi (marco.milesi@cern.ch)
 * J. Dandoy
 *
 ************************************************/

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

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(OverlapRemover)


OverlapRemover :: OverlapRemover (std::string className) :
    Algorithm(className),
    m_useElectrons(false),
    m_useMuons(false),
    m_usePhotons(false),
    m_useTaus(false),
    m_dummyElectronContainer(nullptr),
    m_dummyMuonContainer(nullptr),
    m_el_cutflowHist_1(nullptr),
    m_mu_cutflowHist_1(nullptr),
    m_jet_cutflowHist_1(nullptr),
    m_ph_cutflowHist_1(nullptr),
    m_tau_cutflowHist_1(nullptr)
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
  m_useCutFlow    = true;

  // input container(s) to be read from TEvent or TStore
  m_outputAlgo                  = "OR_Algo";                   // name of vector<string> of combined syst pushed in TStore. 

  /* Muons */
  m_inContainerName_Muons       = "";
  m_inputAlgoMuons              = "";  // name of vector<string> of syst retrieved from TStore

  /* Electrons */
  m_inContainerName_Electrons   = "";
  m_inputAlgoElectrons          = "";  // name of vector<string> of syst retrieved from TStore

  /* Jets */
  m_inContainerName_Jets        = "";
  m_inputAlgoJets               = "";  // name of vector<string> of syst retrieved from TStore

  /* Photons */
  m_inContainerName_Photons     = "";
  m_inputAlgoPhotons            = "";  // name of vector<string> of syst retrieved from TStore

  /* Taus */
  m_inContainerName_Taus        = "";
  m_inputAlgoTaus               = "";  // name of vector<string> of syst retrieved from TStore

  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects     = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::View_Element option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainers    = false;

  m_useSelected = false;
  m_bTagWP      = "";  // Decoration name like "BTag", potentially from BJetEfficiencyCorrector
  m_linkOverlapObjects = true;
  m_useBoostedLeptons  = false;
  m_doEleEleOR = false;


  m_outContainerName_Electrons  = "";
  m_outContainerName_Muons      = "";
  m_outContainerName_Jets       = "";
  m_outContainerName_Photons    = "";
  m_outContainerName_Taus       = "";

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
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
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

  if ( setCutFlowHist() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to setup cutflow histograms. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( m_inContainerName_Jets.empty() ) {
    Error("initialize()", "InputContainerJets is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }

  // be more flexible w/ electrons, muons, photons and taus :)
  if ( !m_inContainerName_Electrons.empty() ) {
    m_useElectrons = true;
  } else{
    m_dummyElectronContainer = new xAOD::ElectronContainer();
  }

  if ( !m_inContainerName_Muons.empty() )     {
    m_useMuons     = true;
  } else {
    m_dummyMuonContainer = new xAOD::MuonContainer();
  }

  if ( !m_inContainerName_Photons.empty() )   { m_usePhotons   = true; }
  if ( !m_inContainerName_Taus.empty() )      { m_useTaus      = true; }
  m_outAuxContainerName_Electrons   = m_outContainerName_Electrons + "Aux."; // the period is very important!
  m_outAuxContainerName_Muons       = m_outContainerName_Muons + "Aux.";     // the period is very important!
  m_outAuxContainerName_Jets        = m_outContainerName_Jets + "Aux.";      // the period is very important!
  m_outAuxContainerName_Photons     = m_outContainerName_Photons + "Aux.";   // the period is very important!
  m_outAuxContainerName_Taus        = m_outContainerName_Taus + "Aux.";      // the period is very important!

  if ( setCounters() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly set event/object counters. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // initialize ASG overlap removal tool
  const std::string selected_label = ( m_useSelected ) ? "passSel" : "";  // set with decoration flag you use for selected objects if want to consider only selected objects in OR, otherwise it will perform OR on all objects

  //Set Flags for recommended overlap procedures
  ORUtils::ORFlags orFlags("OverlapRemovalTool", selected_label, "passOR");

  orFlags.outputPassValue     = true; 
  orFlags.linkOverlapObjects  = m_linkOverlapObjects; 
  orFlags.bJetLabel           = m_bTagWP; 
  orFlags.boostedLeptons      = m_useBoostedLeptons; 
  orFlags.doEleEleOR          = m_doEleEleOR; 

  orFlags.doJets      = true;
  orFlags.doMuons     = m_useMuons;
  orFlags.doElectrons = m_useElectrons;
  orFlags.doTaus      = m_useTaus;
  orFlags.doPhotons   = m_usePhotons;
  orFlags.doFatJets   = false;

  RETURN_CHECK( "OverlapRemover::initialize()", ORUtils::recommendedTools(orFlags, m_ORToolbox), "Failed to get recommended tools in Overlap Removal Tool." );
  RETURN_CHECK( "OverlapRemover::initialize()", m_ORToolbox.initialize(), "Failed to initialize Overlap Removal Tool." );
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

  // syst container name 
  m_vecOutContainerNames = new std::vector< std::string >;

  // --------------------------------------------------------------------------------------------
  //
  // always run the nominal case

  executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus, NOMINAL);

  // look what do we have in TStore
  if ( m_verbose ) { m_store->print(); }

  // -----------------------------------------------------------------------------------------------
  //
  // if at least one of the m_inputAlgo* is not empty, and there's at least one non-empty syst name,
  // then perform OR for every non-empty systematic set

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

    if ( HelperFunctions::found_non_dummy_sys(systNames_el) ) {
      executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus,  ELSYST, systNames_el);
    }

  }

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

    if ( HelperFunctions::found_non_dummy_sys(systNames_mu) ) {
      executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus,  MUSYST, systNames_mu);
    }

  }

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

    if ( HelperFunctions::found_non_dummy_sys(systNames_jet) ) {
      executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus,  JETSYST, systNames_jet);
    }

  }

  // **************** //
  //     Photons      //
  // **************** //

  if ( !m_inputAlgoPhotons.empty() ) {

    // -----------------------
    //
    // get the systematic sets:

    // get vector of string giving the syst names (rememeber: 1st element is a blank string: nominal case!)
    std::vector<std::string>* systNames_photon;
    RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(systNames_photon, m_inputAlgoPhotons, 0, m_store, m_verbose) ,"");

    executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus,  PHSYST, systNames_photon);

  }

  // **************** //
  //       Taus       //
  // **************** //

  if ( !m_inputAlgoTaus.empty() ) {

    // -----------------------
    //
    // get the systematic sets:

    // get vector of string giving the syst names (rememeber: 1st element is a blank string: nominal case!)
    std::vector<std::string>* systNames_tau;
    RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(systNames_tau, m_inputAlgoTaus, 0, m_store, m_verbose) ,"");

    executeOR(inElectrons, inMuons, inJets, inPhotons, inTaus, TAUSYST, systNames_tau);

  }

  // save list of systs that should be considered down stream
  RETURN_CHECK( "OverlapRemover::execute()", m_store->record( m_vecOutContainerNames, m_outputAlgo), "Failed to record vector of output container names.");

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

  if ( m_dummyElectronContainer){ delete m_dummyElectronContainer; m_dummyElectronContainer = nullptr; }
  if ( m_dummyMuonContainer    ){ delete m_dummyMuonContainer;     m_dummyMuonContainer     = nullptr; }

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
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: fillObjectCutflow (const xAOD::IParticleContainer* objCont, const std::string& overlapFlag, const std::string& selectFlag)
{
  SG::AuxElement::ConstAccessor<char> selectAcc(selectFlag);
  SG::AuxElement::ConstAccessor<char> overlapAcc(overlapFlag);
  static SG::AuxElement::ConstAccessor< ElementLink<xAOD::IParticleContainer> > objLinkAcc("overlapObject");

  for ( auto obj_itr : *(objCont) ) {

    std::string type;

    // Safety check
    //
    if ( !overlapAcc.isAvailable( *obj_itr ) ) {
      Error("fillObjectCutflow()", "Overlap decoration missing for this object");
      return EL::StatusCode::FAILURE;
    }
    switch(obj_itr->type()) {
      case xAOD::Type::Electron:
        type = "electron";
        if (!overlapAcc( *obj_itr )) 
          m_el_cutflowHist_1->Fill( m_el_cutflow_OR_cut, 1 );
        break;
      case xAOD::Type::Muon:
        if (!overlapAcc( *obj_itr )) 
          m_mu_cutflowHist_1->Fill( m_mu_cutflow_OR_cut, 1 );
        type = "muon";
        break;
      case xAOD::Type::Jet:
        if (!overlapAcc( *obj_itr )) 
          m_jet_cutflowHist_1->Fill( m_jet_cutflow_OR_cut, 1 );
        type = "jet";
        break;
      case xAOD::Type::Photon:
        if (!overlapAcc( *obj_itr )) 
          m_ph_cutflowHist_1->Fill( m_ph_cutflow_OR_cut, 1 );
        type = "photon";
        break;
      case xAOD::Type::Tau:
        if (!overlapAcc( *obj_itr )) 
          m_tau_cutflowHist_1->Fill( m_tau_cutflow_OR_cut, 1 );
        type = "tau";
        break;
      default:
        Error("OverlapRemover::fillObjectCutflow()","Unsupported object");
        return EL::StatusCode::FAILURE;
        break;
    }

    if ( m_debug ) {
      SG::AuxElement::Decorator< char > isBTag( m_bTagWP );
      int isBTagged = 0;
      if( isBTag.isAvailable( *obj_itr ) && isBTag(*obj_itr)==true )
        isBTagged = 1;

      if ( selectAcc.isAvailable( *obj_itr) ){
        Info("fillObjectCutflow()", "  %s pt %6.2f eta %5.2f phi %5.2f btagged %i selected %i passesOR %i ", type.c_str(), (obj_itr)->pt()/1000., (obj_itr)->eta(), (obj_itr)->phi(), isBTagged, selectAcc( *obj_itr ), overlapAcc( *obj_itr ) );
      } else {
        Info("fillObjectCutflow()", "  %s pt %6.2f eta %5.2f phi %5.2f btagged %i passesOR %i ", type.c_str(), (obj_itr)->pt()/1000., (obj_itr)->eta(), (obj_itr)->phi(), isBTagged, overlapAcc( *obj_itr) );
      }
      // Check for overlap object link
      if ( objLinkAcc.isAvailable( *obj_itr ) && objLinkAcc( *obj_itr ).isValid() ) {
        const xAOD::IParticle* overlapObj = *objLinkAcc( *obj_itr );
        std::stringstream ss_or; ss_or << overlapObj->type();
        Info("fillObjectCutflow()", " Overlap: type %s pt %6.2f", (ss_or.str()).c_str(), overlapObj->pt()/1e3);
      }
    }

  }

  return EL::StatusCode::SUCCESS;

}


EL::StatusCode OverlapRemover :: executeOR(  const xAOD::ElectronContainer* inElectrons, const xAOD::MuonContainer* inMuons, const xAOD::JetContainer* inJets,
               const xAOD::PhotonContainer* inPhotons,   const xAOD::TauJetContainer* inTaus,
               SystType syst_type, std::vector<std::string>* sysVec)
{

  // instantiate output container(s)
  //
  ConstDataVector<xAOD::ElectronContainer> *selectedElectrons   (nullptr);
  ConstDataVector<xAOD::MuonContainer>     *selectedMuons (nullptr);
  ConstDataVector<xAOD::JetContainer>      *selectedJets  (nullptr);
  ConstDataVector<xAOD::PhotonContainer>   *selectedPhotons (nullptr);
  ConstDataVector<xAOD::TauJetContainer>   *selectedTaus  (nullptr);

  // Set input containers to dummys if not using particles
  //
  if(!m_useElectrons) inElectrons = m_dummyElectronContainer;
  if(!m_useMuons)     inMuons     = m_dummyMuonContainer;

  // make a switch for systematics types
  //
  switch ( static_cast<int>(syst_type) )
  {

    case NOMINAL:  // this is the nominal case
    {
      if ( m_debug ) { Info("execute()",  "Doing nominal case"); }
      bool nomContainerNotFound(false);

      if( m_useElectrons ) {
        if ( m_store->contains<ConstDataVector<xAOD::ElectronContainer> >(m_inContainerName_Electrons) ) {
          RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, m_event, m_store, m_verbose) ,"");
        } else {
          nomContainerNotFound = true;
          if ( m_numEvent == 1 ) { Warning("executeOR()", "Could not find nominal container %s in xAOD::TStore. Overlap Removal will not be done for the 'all-nominal' case...", m_inContainerName_Electrons.c_str());  }
        }
      }

      if( m_useMuons ) {
        if ( m_store->contains<ConstDataVector<xAOD::MuonContainer> >(m_inContainerName_Muons) ) {
          RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, m_event, m_store, m_verbose) ,"");
        } else {
          nomContainerNotFound = true;
          if ( m_numEvent == 1 ) { Warning("executeOR()", "Could not find nominal container %s in xAOD::TStore. Overlap Removal will not be done for the 'all-nominal' case...", m_inContainerName_Muons.c_str()); }
        }
      }

      if ( m_store->contains<ConstDataVector<xAOD::JetContainer> >(m_inContainerName_Jets) ) {
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, m_event, m_store, m_verbose) ,"");
      } else {
        nomContainerNotFound = true;
        if ( m_numEvent == 1 ) { Warning("executeOR()", "Could not find nominal container %s in xAOD::TStore. Overlap Removal will not be done for the 'all-nominal' case...", m_inContainerName_Jets.c_str()); }
      }

      if ( m_usePhotons ) {
        if ( m_store->contains<ConstDataVector<xAOD::PhotonContainer> >(m_inContainerName_Photons) ) {
          RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, m_event, m_store, m_verbose) ,"");
        } else {
          nomContainerNotFound = true;
          if ( m_numEvent == 1 ) { Warning("executeOR()", "Could not find nominal container %s in xAOD::TStore. Overlap Removal will not be done for the 'all-nominal' case...", m_inContainerName_Photons.c_str()); }
        }
      }

      if ( m_useTaus ) {
        if ( m_store->contains<ConstDataVector<xAOD::TauJetContainer> >(m_inContainerName_Taus) ) {
          RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, m_event, m_store, m_verbose) ,"");
        } else {
          nomContainerNotFound = true;
          if ( m_numEvent == 1 ) { Warning("executeOR()", "Could not find nominal container %s in xAOD::TStore. Overlap Removal will not be done for the 'all-nominal' case...", m_inContainerName_Taus.c_str()); }
        }
      }

      if ( nomContainerNotFound ) {return EL::StatusCode::SUCCESS;}

      if ( m_debug ) {
        if ( m_useElectrons ) { Info("execute()",  "inElectrons : %lu", inElectrons->size()); }
        if ( m_useMuons )     { Info("execute()",  "inMuons : %lu", inMuons->size()); }
        Info("execute()",  "inJets : %lu", inJets->size() );
        if ( m_usePhotons )   { Info("execute()",  "inPhotons : %lu", inPhotons->size());  }
        if ( m_useTaus    )   { Info("execute()",  "inTaus : %lu",    inTaus->size());  }
      }

      // do the actual OR
      //
      if ( m_debug ) { Info("execute()",  "Calling removeOverlaps()"); }
      RETURN_CHECK( "OverlapRemover::execute()", m_ORToolbox.masterTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

      std::string ORdecor("passOR");
      if(m_useCutFlow){
        // fill cutflow histograms
        //
        if ( m_debug ) { Info("execute()",  "Filling Cut Flow Histograms"); }
        if ( m_useElectrons ) fillObjectCutflow(inElectrons);
        if ( m_useMuons )     fillObjectCutflow(inMuons);
        fillObjectCutflow(inJets);
        if ( m_usePhotons )   fillObjectCutflow(inPhotons);
        if ( m_useTaus )      fillObjectCutflow(inTaus);
      }

      // make a copy of input container(s) with selected objects
      //
      if ( m_createSelectedContainers ) {
        if ( m_debug ) { Info("execute()",  "Creating selected Containers"); }
        if( m_useElectrons ) selectedElectrons  = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
        if( m_useMuons )     selectedMuons      = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
        selectedJets      = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
        if ( m_usePhotons )  selectedPhotons  = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
        if ( m_useTaus )     selectedTaus = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
      }

      // resize containers basd on OR decision:
      //
      // if an object has been flagged as 'passOR', it will be stored in the 'selected' container
      //
      if ( m_debug ) { Info("execute()",  "Resizing"); }
      if ( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
      if ( m_useMuons )     { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
      RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::SELECTOR), "");
      if ( m_usePhotons )   { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
      if ( m_useTaus )      { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::SELECTOR), ""); }

      if ( m_debug ) {
        if ( m_useElectrons) { Info("execute()",  "selectedElectrons : %lu", selectedElectrons->size()); }
        if ( m_useMuons )    { Info("execute()",  "selectedMuons : %lu",     selectedMuons->size()); }
        Info("execute()",  "selectedJets : %lu", selectedJets->size());
        if ( m_usePhotons )  { Info("execute()",  "selectedPhotons : %lu", selectedPhotons->size()); }
        if ( m_useTaus )     { Info("execute()",  "selectedTaus : %lu", selectedTaus->size() ); }
      }

      // add ConstDataVector to TStore
      //
      if ( m_createSelectedContainers ) {
        if ( m_debug ) { Info("execute()",  "Recording"); }
        if ( m_useElectrons ){ RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons,   m_outContainerName_Electrons ), "Failed to store const data container"); }
        if ( m_useMuons )    { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,  m_outContainerName_Muons ), "Failed to store const data container"); }
        RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,  m_outContainerName_Jets ), "Failed to store const data container");
        if ( m_usePhotons )  { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons ), "Failed to store const data container"); }
        if ( m_useTaus )     { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus ), "Failed to store const data container"); }
      }

      m_vecOutContainerNames->push_back("");
      break;
    }
    case ELSYST : // electron syst
    {
      if ( m_debug ) { Info("execute()",  "Doing electron systematics"); }
      // just to check everything is fine
      if ( m_debug ) {
           Info("execute()","will consider the following ELECTRON systematics:" );
           for ( auto it : *sysVec ) {  Info("execute()" ,"\t %s ", it.c_str()); }
      }

      // these input containers won't change in the electron syst loop ...
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, 0, m_store, m_verbose) , ("Could not find Jets container "+m_inContainerName_Jets+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useMuons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, 0, m_store, m_verbose) , ("Could not find Muons container "+m_inContainerName_Muons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_usePhotons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, 0, m_store, m_verbose) , ("Could not find Photons container "+m_inContainerName_Photons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useTaus ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, 0, m_store, m_verbose) , ("Could not find Taus container "+m_inContainerName_Taus+" in xAOD::TStore. Aborting").c_str() );

      for ( auto systName : *sysVec) {

        if ( systName.empty() ) continue;

        // ... instead, the electron input container will be different for each syst
        //
        std::string el_syst_cont_name = m_inContainerName_Electrons + systName;
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, el_syst_cont_name, 0, m_store, m_verbose) , ("Could not find Electrons syst container "+el_syst_cont_name+" in xAOD::TStore. Aborting").c_str() );

        if ( m_debug ) { Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size() );  }

        // do the actual OR
        //
        RETURN_CHECK( "OverlapRemover::execute()", m_ORToolbox.masterTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

        std::string ORdecor("passOR");
        if(m_useCutFlow){
          // fill cutflow histograms
          //
          fillObjectCutflow(inElectrons);
          if ( m_useMuons )   fillObjectCutflow(inMuons);
          fillObjectCutflow(inJets);
          if ( m_usePhotons ) fillObjectCutflow(inPhotons);
          if ( m_useTaus )    fillObjectCutflow(inTaus);
        }

        // make a copy of input container(s) with selected objects
        //
        if ( m_createSelectedContainers ) {
          selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          if ( m_useMuons )    selectedMuons  = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets        = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          if ( m_usePhotons )  selectedPhotons  = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          if ( m_useTaus )     selectedTaus = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers basd on OR decision
        //
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::SELECTOR), "");
        if ( m_useMuons )  {  RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::SELECTOR), "");
        if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        if ( m_useTaus )   {  RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::SELECTOR), ""); }

        // add ConstDataVector to TStore
        //
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container");
          if ( m_useMuons )  { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container"); }
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          if ( m_usePhotons ){ RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons + systName ), "Failed to store const data container"); }
          if ( m_useTaus )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container"); }
        }

        m_vecOutContainerNames->push_back(systName);
      } // close loop on systematic sets available from upstream algo (Electrons)

      break;
    }
    case MUSYST: // muon syst
    {
      if ( m_debug ) { Info("execute()",  "Doing  muon systematics"); }
      // just to check everything is fine
      if ( m_debug ) {
         Info("execute()","will consider the following MUON systematics:" );
         for ( auto it : *sysVec ){ Info("execute()" ,"\t %s ", it.c_str()); }
      }

      // these input containers won't change in the muon syst loop ...
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, 0, m_store, m_verbose) , ("Could not find Jets container "+m_inContainerName_Jets+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useElectrons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, 0, m_store, m_verbose) , ("Could not find Electrons container "+m_inContainerName_Electrons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_usePhotons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, 0, m_store, m_verbose) , ("Could not find Photons container "+m_inContainerName_Photons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useTaus ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, 0, m_store, m_verbose) , ("Could not find Taus container "+m_inContainerName_Taus+" in xAOD::TStore. Aborting").c_str() );


      for ( auto systName : *sysVec) {

        if ( systName.empty() ) continue;

        // ... instead, the muon input container will be different for each syst
        //
        std::string mu_syst_cont_name = m_inContainerName_Muons + systName;
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, mu_syst_cont_name, 0, m_store, m_verbose) , ("Could not find Muons Systematic container "+mu_syst_cont_name+" in xAOD::TStore. Aborting").c_str() );

        if ( m_debug ) { Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu ", inElectrons->size(), inMuons->size(),  inJets->size() );  }

        // do the actual OR
        //
        RETURN_CHECK( "OverlapRemover::execute()", m_ORToolbox.masterTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

        std::string ORdecor("passOR");
        if(m_useCutFlow){
          // fill cutflow histograms
          //
          if ( m_useElectrons ) fillObjectCutflow(inElectrons);
          fillObjectCutflow(inMuons);
          fillObjectCutflow(inJets);
          if( m_usePhotons )    fillObjectCutflow(inPhotons);
          if( m_useTaus )       fillObjectCutflow(inTaus);
        }
        // make a copy of input container(s) with selected objects
        //
        if ( m_createSelectedContainers ) {
          if ( m_useElectrons ) selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          selectedMuons       = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets        = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          if ( m_usePhotons )   selectedPhotons = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          if ( m_useTaus )      selectedTaus  = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers based on OR decision
        //
        if ( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::SELECTOR), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::SELECTOR), "");
        if ( m_usePhotons )   { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        if ( m_useTaus )      { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::SELECTOR), ""); }

        // add ConstDataVector to TStore
        //
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          //
          if ( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container"); }
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          if ( m_usePhotons )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons + systName ), "Failed to store const data container"); }
          if ( m_useTaus )      { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container"); }
        }

        m_vecOutContainerNames->push_back(systName);
      } // close loop on systematic sets available from upstream algo (Muons)

      break;
    }
    case JETSYST: // jet systematics
    {
      if ( m_debug ) { Info("execute()",  "Doing  jet systematics"); }
      // just to check everything is fine
      if ( m_debug ) {
        Info("execute()","will consider the following JET systematics:" );
        for ( auto it : *sysVec ) { Info("execute()" ,"\t %s ", it.c_str());  }
      }

      // these input containers won't change in the jet syst loop ...
      if ( m_useMuons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, 0, m_store, m_verbose) , ("Could not find Muons container "+m_inContainerName_Muons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useElectrons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, 0, m_store, m_verbose) , ("Could not find Electrons container "+m_inContainerName_Electrons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_usePhotons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, 0, m_store, m_verbose) , ("Could not find Photons container "+m_inContainerName_Photons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useTaus ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, 0, m_store, m_verbose) , ("Could not find Taus container "+m_inContainerName_Taus+" in xAOD::TStore. Aborting").c_str() );

      for( auto systName : *sysVec ) {

        if ( systName.empty() ) continue;

        // ... instead, the jet input container will be different for each syst
        //
        std::string jet_syst_cont_name = m_inContainerName_Jets + systName;
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, jet_syst_cont_name, 0, m_store, m_verbose) , ("Could not find Jets container "+jet_syst_cont_name+" in xAOD::TStore. Aborting").c_str() );

        if ( m_debug ) { Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu ", inElectrons->size(), inMuons->size(),  inJets->size() );  }

        // do the actual OR
        //
        RETURN_CHECK( "OverlapRemover::execute()", m_ORToolbox.masterTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

        std::string ORdecor("passOR");
        if(m_useCutFlow){
          // fill cutflow histograms
          //
          if ( m_useElectrons ) fillObjectCutflow(inElectrons);
          if ( m_useMuons )     fillObjectCutflow(inMuons);
          fillObjectCutflow(inJets);
          if( m_usePhotons )    fillObjectCutflow(inPhotons);
          if( m_useTaus )       fillObjectCutflow(inTaus);
        }

        // make a copy of input container(s) with selected objects
        //
        if ( m_createSelectedContainers ) {
          if ( m_useElectrons ) selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          if ( m_useMuons )     selectedMuons      = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets       = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          if ( m_usePhotons )   selectedPhotons  = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          if ( m_useTaus )      selectedTaus = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers basd on OR decision
        //
        if ( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        if ( m_useMuons )     { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::SELECTOR), "");
        if ( m_usePhotons )   { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        if ( m_useTaus )      { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::SELECTOR), ""); }

        // add ConstDataVector to TStore
        //
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          //
          if ( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container"); }
          if ( m_useMuons )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container"); }
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          if ( m_usePhotons )   { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons + systName ), "Failed to store const data container"); }
          if ( m_useTaus )      { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container"); }
        }

        m_vecOutContainerNames->push_back(systName);
      } // close loop on systematic sets available from upstream algo (Jets)

      break;
    }
    case PHSYST : // photon systematics
    {
      if ( m_debug ) { Info("execute()",  "Doing  photon systematics"); }
      // just to check everything is fine
      if ( m_debug ) {
        Info("execute()","will consider the following PHOTON systematics:" );
        for ( auto it : *sysVec ) { Info("execute()" ,"\t %s ", it.c_str());  }
      }

      // these input containers won't change in the photon syst loop ...
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, 0, m_store, m_verbose) , ("Could not find Jets container "+m_inContainerName_Jets+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useElectrons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, 0, m_store, m_verbose) , ("Could not find Electrons container "+m_inContainerName_Electrons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useMuons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, 0, m_store, m_verbose) , ("Could not find Muons container "+m_inContainerName_Muons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useTaus ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, m_inContainerName_Taus, 0, m_store, m_verbose) , ("Could not find Taus container "+m_inContainerName_Taus+" in xAOD::TStore. Aborting").c_str() );

      for( auto systName : *sysVec ) {

        if ( systName.empty() ) continue;

        // ... instead, the photon input container will be different for each syst
        //
        std::string photon_syst_cont_name = m_inContainerName_Photons + systName;
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, photon_syst_cont_name, 0, m_store, m_verbose) , ("Could not find Photons systematic container "+photon_syst_cont_name+" in xAOD::TStore. Aborting").c_str() );

        if ( m_debug ) {
          Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size() );
        }

        // do the actual OR
        //
        RETURN_CHECK( "OverlapRemover::execute()", m_ORToolbox.masterTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");


        std::string ORdecor = std::string("passOR");
        if(m_useCutFlow){
          // fill cutflow histograms
          //
          if( m_useElectrons ) fillObjectCutflow(inElectrons);
          if( m_useMuons     ) fillObjectCutflow(inMuons);
          fillObjectCutflow(inJets);
          fillObjectCutflow(inPhotons);
          if( m_useTaus )      fillObjectCutflow(inTaus);
        }

        // make a copy of input container(s) with selected objects
        //
        if ( m_createSelectedContainers ) {
          if( m_useElectrons ) selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          if( m_useMuons     ) selectedMuons       = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets        = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          selectedPhotons     = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          if ( m_useTaus )     selectedTaus    = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers based on OR decision
        //
        if( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        if( m_useMuons )     { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::SELECTOR), "");
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::SELECTOR), "");
        if ( m_useTaus )     { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::SELECTOR), ""); }

        // add ConstDataVector to TStore
        //
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          //
          if( m_useElectrons ){ RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container"); }
          if( m_useMuons )    { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container"); }
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons,   m_outContainerName_Photons + systName ), "Failed to store const data container");
          if ( m_useTaus )    { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container"); }
        }

        m_vecOutContainerNames->push_back(systName);
      } // close loop on systematic sets available from upstream algo (Photons)

      break;
    }
    case TAUSYST : // tau systematics
    {
      if ( m_debug ) { Info("execute()",  "Doing tau systematics"); }

      // just to check everything is fine
      if ( m_debug ) {
        Info("execute()","output vector already contains the following TAU systematics:" );
        for ( auto it : *sysVec ) { Info("execute()" ,"\t %s ", it.c_str());  }
      }

      // these input containers won't change in the tau syst loop ...
      RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, 0, m_store, m_verbose) , ("Could not find Jets container "+m_inContainerName_Jets+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useElectrons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, 0, m_store, m_verbose) , ("Could not find Electrons container "+m_inContainerName_Electrons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_useMuons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inMuons, m_inContainerName_Muons, 0, m_store, m_verbose) , ("Could not find Muons container "+m_inContainerName_Muons+" in xAOD::TStore. Aborting").c_str() );
      if ( m_usePhotons ) 
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName_Photons, 0, m_store, m_verbose) , ("Could not find Photons container "+m_inContainerName_Photons+" in xAOD::TStore. Aborting").c_str() );

      for( auto systName : *sysVec ) {

        if ( systName.empty() ) continue;
      
        // ... instead, the tau input container will be different for each syst
        //
        std::string tau_syst_cont_name = m_inContainerName_Taus + systName;
        RETURN_CHECK("OverlapRemover::execute()", HelperFunctions::retrieve(inTaus, tau_syst_cont_name, 0, m_store, m_verbose) , ("Could not find Taus container "+tau_syst_cont_name+" in xAOD::TStore. Aborting").c_str() );

        if ( m_debug ) {
          Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size() );
        }

        // do the actual OR
        //
        RETURN_CHECK( "OverlapRemover::execute()", m_ORToolbox.masterTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons), "");

        std::string ORdecor = std::string("passOR");
        if(m_useCutFlow){
          // fill cutflow histograms
          //
          if( m_useElectrons ) fillObjectCutflow(inElectrons);
          if( m_useMuons     ) fillObjectCutflow(inMuons);
          fillObjectCutflow(inJets);
          if( m_usePhotons ) fillObjectCutflow(inPhotons);
          fillObjectCutflow(inTaus);
        }

        // make a copy of input container(s) with selected objects
        //
        if ( m_createSelectedContainers ) {
          if( m_useElectrons ) selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
          if( m_useMuons )     selectedMuons       = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
          selectedJets       = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
          if ( m_usePhotons )  selectedPhotons     = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
          selectedTaus       = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
        }

        // resize containers based on OR decision
        //
        if( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        if( m_useMuons )     { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, ORdecor.c_str(), ToolName::SELECTOR), "");
        if ( m_usePhotons )  { RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, ORdecor.c_str(), ToolName::SELECTOR), ""); }
        RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, ORdecor.c_str(), ToolName::SELECTOR), "");

        // add ConstDataVector to TStore
        //
        if ( m_createSelectedContainers ) {
          // a different syst varied container will be stored for each syst variation
          //
          if( m_useElectrons ) { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons + systName ), "Failed to store const data container"); }
          if( m_useMuons )     { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons,     m_outContainerName_Muons + systName ), "Failed to store const data container"); }
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets,      m_outContainerName_Jets + systName ), "Failed to store const data container");
          if ( m_usePhotons )  { RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons,   m_outContainerName_Photons + systName ), "Failed to store const data container"); }
          RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus + systName ), "Failed to store const data container");
        }

        m_vecOutContainerNames->push_back(systName);
      } // close loop on systematic sets available from upstream algo (Taus)

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

EL::StatusCode OverlapRemover :: setCutFlowHist( )
{

  if ( m_useCutFlow ) {

    // retrieve the file in which the cutflow hists are stored
    //
    TFile *file     = wk()->getOutputFile ("cutflow");

    // retrieve the object cutflow
    //
    m_el_cutflowHist_1  = (TH1D*)file->Get("cutflow_electrons_1");
    m_el_cutflow_OR_cut   = m_el_cutflowHist_1->GetXaxis()->FindBin("OR_cut");
    m_mu_cutflowHist_1  = (TH1D*)file->Get("cutflow_muons_1");
    m_mu_cutflow_OR_cut   = m_mu_cutflowHist_1->GetXaxis()->FindBin("OR_cut");
    m_jet_cutflowHist_1   = (TH1D*)file->Get("cutflow_jets_1");
    m_jet_cutflow_OR_cut  = m_jet_cutflowHist_1->GetXaxis()->FindBin("OR_cut");
    m_ph_cutflowHist_1  = (TH1D*)file->Get("cutflow_photons_1");
    m_ph_cutflow_OR_cut   = m_ph_cutflowHist_1->GetXaxis()->FindBin("OR_cut");
    m_tau_cutflowHist_1   = (TH1D*)file->Get("cutflow_taus_1");
    m_tau_cutflow_OR_cut  = m_tau_cutflowHist_1->GetXaxis()->FindBin("OR_cut");
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode OverlapRemover :: setCounters( )
{
  m_numEvent      = 0;
  m_numObject     = 0;

  return EL::StatusCode::SUCCESS;
}
