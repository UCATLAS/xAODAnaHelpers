#include <iostream>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODAnaHelpers/METConstructor.h"

// top of file, outside of algorithm declaration
// #include "METUtilities/METRebuilder.h"
#include "METUtilities/METMaker.h"
#include "METUtilities/CutsMETMaker.h"
#include "TauAnalysisTools/TauSelectionTool.h"

#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODJet/JetContainer.h"

#include "xAODCore/ShallowCopy.h"

// #include "xAODMissingET/MissingET.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/MissingETComposition.h"
#include "xAODMissingET/MissingETAssociationMap.h"

// #include "xAODParticleEvent/Particle.h"
// #include "xAODParticleEvent/ParticleContainer.h"
// #include "xAODParticleEvent/ParticleAuxContainer.h"

#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

namespace xAOD {
#ifndef XAODJET_JETCONTAINER_H
  class JetContainer;
#endif
#ifndef XAODJET_JET_H
  class Jet;
#endif
}

using std::cout; using std::cerr; using std::endl;

// this is needed to distribute the algorithm to the workers
ClassImp(METConstructor)


METConstructor :: METConstructor (std::string className) :
    Algorithm(className),
    m_metmaker(0)
{

  m_debug           = false;

  m_referenceMETContainer = "MET_Reference_AntiKt4LCTopo";
  m_mapName               = "METAssoc_AntiKt4LCTopo";
  m_coreName              = "MET_Core_AntiKt4LCTopo";
  m_outputContainer       = "NewRefFinal";

  m_inputJets       = "";
  m_inputElectrons  = "";
  m_inputPhotons    = "";
  m_inputTaus       = "";
  m_inputMuons      = "";

  m_doElectronCuts  = false;
  m_doPhotonCuts    = false;
  m_doTauCuts       = false;
  m_doMuonCuts      = false;

  m_doMuonEloss     = false;
  m_doIsolMuonEloss = false;
  m_doJVTCut        = false;

  m_useCaloJetTerm  = true;
  m_useTrackJetTerm = false;

}

EL::StatusCode METConstructor :: setupJob (EL::Job& job)
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
  xAOD::Init( "METConstructor" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode METConstructor :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode METConstructor :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode METConstructor :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode METConstructor :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing METConstructor Interface...");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  //m_store->print();

  m_metmaker = new met::METMaker("METMaker");
  if(m_metmaker->initialize().isFailure()){
    Error("initialize()", "Failed to properly initialize MET maker. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_tauSelTool = new TauAnalysisTools::TauSelectionTool( "TauSelectionTool" );
  if (m_tauSelTool->initialize().isFailure()) {
    Error("initialize()", "Failed to properly initialize tau selection tool. Exiting." );
    return EL::StatusCode::FAILURE;
  }


  RETURN_CHECK( "METConstructor::initialize()", m_metmaker->setProperty( "DoMuonEloss", m_doMuonEloss), "");
  RETURN_CHECK( "METConstructor::initialize()", m_metmaker->setProperty( "DoIsolMuonEloss", m_doIsolMuonEloss), "");

  Info("initialize()", "METConstructor Interface %s succesfully initialized!", m_name.c_str());

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode METConstructor :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Performing MET reconstruction...");


  // Create a MissingETContainer with its aux store
  xAOD::MissingETContainer* newMet = new xAOD::MissingETContainer();
  RETURN_CHECK("METConstructor::execute()", m_store->record(newMet, m_outputContainer.Data()), "Failed to store MET output container.");

  xAOD::MissingETAuxContainer* metAuxCont = new xAOD::MissingETAuxContainer();
  RETURN_CHECK("METConstructor::execute()", m_store->record(metAuxCont, (m_outputContainer + "Aux.").Data()), "Failed to store MET output container.");
  newMet->setStore(metAuxCont);


  const xAOD::MissingETAssociationMap* metMap = 0;
  RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(metMap,  m_mapName.Data(), m_event, m_store, m_verbose), "Failed retrieving MET Map.");
  metMap->resetObjSelectionFlags();

  ///////////////////////
  ////// ELECTRONS  /////
  ///////////////////////
  if( m_inputElectrons.Length() > 0 ) {
    const xAOD::ElectronContainer* eleCont(0);
    RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(eleCont, m_inputElectrons.Data(), m_event, m_store, m_verbose), "Failed retrieving electron cont.");
    if (m_doElectronCuts) {
      ConstDataVector<xAOD::ElectronContainer> metElectrons(SG::VIEW_ELEMENTS);
      for (const auto& el : *eleCont) if (CutsMETMaker::accept(el)) metElectrons.push_back(el);
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("RefEle", xAOD::Type::Electron, newMet, metElectrons.asDataVector(), metMap), "Failed rebuilding electron component.");
    } else {
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("RefEle", xAOD::Type::Electron, newMet, eleCont, metMap), "Failed rebuilding electron component.");
    }
  }


  //////////////////////
  //////  PHOTONS  /////
  //////////////////////
  if( m_inputPhotons.Length() > 0 ) {
    const xAOD::PhotonContainer* phoCont(0);
    RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(phoCont, m_inputPhotons.Data(), m_event, m_store, m_verbose), "Failed retrieving photon cont.");
    if (m_doPhotonCuts) {
      ConstDataVector<xAOD::PhotonContainer> metPhotons(SG::VIEW_ELEMENTS);
      for (const auto& ph : *phoCont) {

        bool testPID = 0;
        ph->passSelection(testPID, "Tight");
        if( !testPID ) continue;

        //ATH_MSG_VERBOSE("Photon author = " << ph->author() << " test " << (ph->author()&20));
        if (!(ph->author() & 20)) continue;

        if (ph->pt() < 25e3) continue;

        float feta = fabs(ph->eta());
        if (feta > 2.37 || (1.37 < feta && feta < 1.52)) continue;

        metPhotons.push_back(ph);
      }
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("RefGamma", xAOD::Type::Photon, newMet, metPhotons.asDataVector(), metMap), "Failed rebuilding photon component.");
    } else {
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("RefGamma", xAOD::Type::Photon, newMet, phoCont, metMap), "Failed rebuilding photon component.");
    }
  }


  ///////////////////
  //////  TAUS  /////
  ///////////////////
  if( m_inputTaus.Length() > 0 ) {
    const xAOD::TauJetContainer* tauCont(0);
    RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(tauCont, m_inputTaus.Data(), m_event, m_store, m_verbose), "Failed retrieving tau cont.");
    if (m_doTauCuts) {
      ConstDataVector<xAOD::TauJetContainer> metTaus(SG::VIEW_ELEMENTS);
      for (const auto& tau : *tauCont) {

        if (tau->pt() < 20e3) continue;
        if (fabs(tau->eta()) > 2.37) continue;
        if (!m_tauSelTool->accept(tau)) continue;

        metTaus.push_back(tau);
      }
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("RefTau", xAOD::Type::Tau, newMet, metTaus.asDataVector(), metMap), "Failed rebuilding tau component.");
    } else {
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("RefTau", xAOD::Type::Tau, newMet, tauCont, metMap), "Failed rebuilding tau component.");
    }
  }


  ////////////////////
  //////  MUONS  /////
  ////////////////////
  if( m_inputMuons.Length() > 0 ) {
    const xAOD::MuonContainer* muonCont(0);
    RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(muonCont, m_inputMuons.Data(), m_event, m_store, m_verbose), "Failed retrieving muon cont.");
    if (m_doMuonCuts) {
      ConstDataVector<xAOD::MuonContainer> metMuons(SG::VIEW_ELEMENTS);
      for (const auto& mu : *muonCont) if (CutsMETMaker::accept(mu)) metMuons.push_back(mu);
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("Muons", xAOD::Type::Muon, newMet, metMuons.asDataVector(), metMap), "Failed rebuilding muon component.");
    } else {
      RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildMET("Muons", xAOD::Type::Muon, newMet, muonCont, metMap), "Failed rebuilding muon component.");
    }
  }

  const xAOD::JetContainer* jetCont(0);
  const xAOD::MissingETContainer* coreMet(0);
  RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(jetCont, m_inputJets.Data(), m_event, m_store, m_verbose), "Failed retrieving jet cont.");
  RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(coreMet, m_coreName.Data(), m_event, m_store, m_verbose), "Failed retrieving MET Core.");

  if ( m_useCaloJetTerm ) {
    RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildJetMET("RefJet", "SoftClus", "PVSoftTrk", newMet, jetCont, coreMet, metMap, m_doJVTCut), "Failed to build cluster-based jet/MET.");
  } else if ( m_useTrackJetTerm ) {
    RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildTrackMET("RefJetTrk", "PVSoftTrk", newMet, jetCont, coreMet, metMap, m_doJVTCut), "Failed to build track-based jet/MET.");
  } else {
    Error("execute()", "Both m_useCaloJetTerm and m_useTrackJetTerm appear to be set to 'false'. This should not happen. Please check your MET configuration file");
    return EL::StatusCode::FAILURE;
  }

  RETURN_CHECK("METConstructor::execute()", m_metmaker->buildMETSum("FinalClus", newMet, MissingETBase::Source::LCTopo), "Failed to build FinalClus MET.");
  RETURN_CHECK("METConstructor::execute()", m_metmaker->buildMETSum("FinalTrk",  newMet, MissingETBase::Source::Track),  "Failed to build FinalTrk MET.");

  if ( m_debug ) {
    const xAOD::MissingETContainer* oldMet(0);
    RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(oldMet, m_referenceMETContainer.Data(), m_event, m_store, m_verbose) ,"");
    //xAOD::MissingETContainer::const_iterator final(oldMet->find("FinalClus"));
    //xAOD::MissingETContainer::const_iterator newfinal(newMet->find("FinalClus"));
    Info("execute()", ">>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    if( m_inputElectrons.Length() > 0 ) Info("execute()", "RefEle:     old=%8.f  new=%8.f", (*oldMet->find("RefEle"))->met(), (*newMet->find("RefEle"))->met());
    if( m_inputPhotons.Length() > 0 )   Info("execute()", "RefPhoton:  old=%8.f  new=%8.f", (*oldMet->find("RefGamma"))->met(), (*newMet->find("RefGamma"))->met());
    if( m_inputTaus.Length() > 0 )      Info("execute()", "RefTau:     old=%8.f  new=%8.f", (*oldMet->find("RefTau"))->met(), (*newMet->find("RefTau"))->met());
    if( m_inputMuons.Length() > 0 )     Info("execute()", "RefMuon:    old=%8.f  new=%8.f", (*oldMet->find("Muons"))->met(), (*newMet->find("Muons"))->met());
    Info("execute()", "RefJet:       old=%8.f  new=%8.f", (*oldMet->find("RefJet"))->met(), (*newMet->find("RefJet"))->met());
    Info("execute()", "SoftClus:     old=%8.f  new=%8.f", (*oldMet->find("SoftClus"))->met(), (*newMet->find("SoftClus"))->met());
    Info("execute()", "PVSoftTrk:    old=%8.f  new=%8.f", (*oldMet->find("PVSoftTrk"))->met(), (*newMet->find("PVSoftTrk"))->met());
    Info("execute()", "  ");
    Info("execute()", "FinalClus:    old=%8.f  new=%8.f", (*oldMet->find("FinalClus"))->met(), (*newMet->find("FinalClus"))->met());
    Info("execute()", "       >>>>> R=%.3f",          (*oldMet->find("FinalClus"))->met()/ (*newMet->find("FinalClus"))->met());
    Info("execute()", "FinalTrk:     old=%8.f  new=%8.f", (*oldMet->find("FinalTrk"))->met(), (*newMet->find("FinalTrk"))->met());
    Info("execute()", "       >>>>> R=%.3f",          (*oldMet->find("FinalTrk"))->met()/ (*newMet->find("FinalTrk"))->met());
  }

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode METConstructor :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode METConstructor::finalize()
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

  if (m_metmaker) {
    delete m_metmaker;
    m_metmaker = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode METConstructor :: histFinalize ()
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
