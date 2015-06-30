#include <iostream>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODAnaHelpers/METConstructor.h"

// top of file, outside of algorithm declaration
// #include "METUtilities/METRebuilder.h"
#include "METUtilities/METMaker.h"
#include "METUtilities/CutsMETMaker.h"

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

#include "TEnv.h"
#include "TSystem.h"

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


METConstructor :: METConstructor () : m_metmaker(0) {}

EL::StatusCode  METConstructor :: configure ()
{
  Info("configure()", "Configuing METConstructor Interface. User configuration read from : %s \n", getConfig(true).c_str());

  getConfig(true) = gSystem->ExpandPathName( getConfig(true).c_str() );
  // check if file exists
  /* https://root.cern.ch/root/roottalk/roottalk02/5332.html */
  FileStat_t fStats;
  int fSuccess = gSystem->GetPathInfo(getConfig(true).c_str(), fStats);
  if(fSuccess != 0){
    Error("configure()", "Could not find the configuration file");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Found configuration file");

  TEnv* config = new TEnv(getConfig(true).c_str());

  // read debug flag from .config file
  m_debug           = config->GetValue("Debug" , false );
  m_referenceMETContainer = config->GetValue("Reference",       "MET_Reference_AntiKt4LCTopo");

  m_mapName         = config->GetValue("MapName",         "METAssoc_AntiKt4LCTopo");
  m_coreName        = config->GetValue("CoreName",        "MET_Core_AntiKt4LCTopo");
  m_outputContainer = config->GetValue("OutputContainer", "NewRefFinal");

  m_inputJets       = config->GetValue("InputJets",       "");
  m_inputElectrons  = config->GetValue("InputElectrons",  "");
  m_inputPhotons    = config->GetValue("InputPhotons",    "");
  m_inputTaus       = config->GetValue("InputTaus",       "");
  m_inputMuons      = config->GetValue("InputMuons",      "");

  m_doElectronCuts  = config->GetValue("ApplyElectronCuts", false);
  m_doPhotonCuts    = config->GetValue("ApplyPhotonCuts",   false);
  m_doTauCuts       = config->GetValue("ApplyTauCuts",      false);
  m_doMuonCuts      = config->GetValue("ApplyMuonCuts",     false);

  if( m_mapName.Length() == 0 ) {
    Error("configure()", "MapName is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "METConstructor Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
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

  Info("setupJob()", "Calling setupJob \n");

  job.useXAOD ();
  xAOD::Init( "METConstructor" ).ignore(); // call before opening first file
  Info("setupJob()", "METConstructor Ready\n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode METConstructor :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
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

  Info("initialize()", "Initializing METConstructor Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  //m_store->print();

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_metmaker = new met::METMaker("METMaker");
  if(m_metmaker->initialize().isFailure()){
    Error("initialize()", "Failed to properly initialize. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  Info("initialize()", "METConstructor Interface %s succesfully initialized!", m_name.c_str());

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode METConstructor :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Performing MET reconstruction... \n");


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
      for (const auto& ph : *phoCont) if (CutsMETMaker::accept(ph)) metPhotons.push_back(ph);
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
      for (const auto& tau : *tauCont) if (CutsMETMaker::accept(tau)) metTaus.push_back(tau);
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
  RETURN_CHECK("METConstructor::execute()", m_metmaker->rebuildJetMET("RefJet", "SoftClus", "PVSoftTrk", newMet, jetCont, coreMet, metMap, false), "Failed to build jet/MET.");


  RETURN_CHECK("METConstructor::execute()", m_metmaker->buildMETSum("FinalClus", newMet, MissingETBase::Source::LCTopo), "Failed to build FinalClus MET.");
  RETURN_CHECK("METConstructor::execute()", m_metmaker->buildMETSum("FinalTrk",  newMet, MissingETBase::Source::Track),  "Failed to build FinalTrk MET.");


  if (m_debug) {
    const xAOD::MissingETContainer* oldMet(0);
    RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(oldMet, m_referenceMETContainer.Data(), m_event, m_store, m_verbose) ,"");
    xAOD::MissingETContainer::const_iterator final(oldMet->find("FinalClus"));
    xAOD::MissingETContainer::const_iterator newfinal(newMet->find("FinalClus"));
    Info("execute()", ">>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    if( m_inputElectrons.Length() > 0 ) Info("execute()", "RefEle:    old=%8.f  new=%8.f", (*oldMet->find("RefEle")   )->met(), (*newMet->find("RefEle")   )->met());
    if( m_inputPhotons.Length() > 0 ) Info("execute()", "RefGamma:  old=%8.f  new=%8.f", (*oldMet->find("RefGamma") )->met(), (*newMet->find("RefGamma") )->met());
    if( m_inputTaus.Length() > 0 ) Info("execute()", "RefTau:    old=%8.f  new=%8.f", (*oldMet->find("RefTau")   )->met(), (*newMet->find("RefTau")   )->met());
    if( m_inputMuons.Length() > 0 ) Info("execute()", "Muons:     old=%8.f  new=%8.f", (*oldMet->find("Muons")    )->met(), (*newMet->find("Muons")    )->met());
    Info("execute()", "RefJet:    old=%8.f  new=%8.f", (*oldMet->find("RefJet")   )->met(), (*newMet->find("RefJet")   )->met());
    Info("execute()", "SoftClus:  old=%8.f  new=%8.f", (*oldMet->find("SoftClus") )->met(), (*newMet->find("SoftClus") )->met());
    Info("execute()", "FinalClus: old=%8.f  new=%8.f", (*oldMet->find("FinalClus"))->met(), (*newMet->find("FinalClus"))->met());
    Info("execute()", "       >>>>> R=%.3f",          (*oldMet->find("FinalClus"))->met()/ (*newMet->find("FinalClus"))->met());
  }

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode METConstructor :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

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

  Info("finalize()", "Deleting tool instances... \n");

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

  Info("histFinalize()", "Calling histFinalize \n");

  return EL::StatusCode::SUCCESS;
}
