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

#include <xAODEventInfo/EventInfo.h>

// #include "xAODParticleEvent/Particle.h"
// #include "xAODParticleEvent/ParticleContainer.h"
// #include "xAODParticleEvent/ParticleAuxContainer.h"

#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>


// for METsyst
#include <xAODAnaHelpers/HelperClasses.h>
#include "PATInterfaces/SystematicVariation.h"
#include "METUtilities/METSystematicsTool.h"
#include "assert.h"

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
using std::string;

// this is needed to distribute the algorithm to the workers
ClassImp(METConstructor)


METConstructor :: METConstructor () :
    Algorithm("METConstructor")
{
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

  ATH_MSG_INFO( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "METConstructor" ).ignore(); // call before opening first file

   // to validate and check:
   //enable status code failures
   //CP::CorrectionCode::enableFailure();
   //CP::SystematicCode::enableFailure();
   //StatusCode::enableFailure();// do not decomment this, maybe an unchecked status code gives a crash...


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

  ATH_MSG_INFO( "Initializing METConstructor Interface...");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //////////// IMETMaker ////////////////
  ASG_SET_ANA_TOOL_TYPE(m_metmaker_handle, met::METMaker);
  m_metmaker_handle.setName("METMaker");
  m_metmaker_handle.retrieve();

  ///////////// IMETSystematicsTool ///////////////////
  ASG_SET_ANA_TOOL_TYPE(m_metSyst_handle, met::METSystematicsTool);
  m_metSyst_handle.setName("METSyst");
  m_metSyst_handle.retrieve();

  m_tauSelTool = new TauAnalysisTools::TauSelectionTool( "TauSelectionTool" );
  if (m_tauSelTool->initialize().isFailure()) {
    ATH_MSG_ERROR( "Failed to properly initialize tau selection tool. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  ATH_MSG_INFO( "METConstructor Interface " << m_name << " succesfully initialized!");

  //use the helper function getlistofsystematics:

  // run syst
  if(!m_runNominal && !m_systName.empty()) { //  m_systName is set by default to m_systName= "All", do not change it
      // get the syst from met syst tool
      const CP::SystematicSet recSyst = m_metSyst_handle->recommendedSystematics();
      sysList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, msg() );

  } else { //run nominal
    sysList.push_back(CP::SystematicSet()); // add empty systematic (Nominal case)
  }

  for ( const auto& syst_it : sysList ) {
    ATH_MSG_DEBUG("syst_it = "<<syst_it.name());
    ATH_MSG_INFO("\t " << syst_it.name());
  }

  m_numEvent = 0; //just as a check

  // define m_isMC
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("METConstructor::initialize()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) ,"");

  m_isMC = eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );
  ATH_MSG_DEBUG( "Is MC? " << static_cast<int>(m_isMC) );


  return EL::StatusCode::SUCCESS;
}


EL::StatusCode METConstructor :: execute ()
{
   // Here you do everything that needs to be done on every single
   // events, e.g. read input variables, apply cuts, and fill
   // histograms and trees.  This is where most of your actual analysis
   // code will go.

   ATH_MSG_DEBUG( "Performing MET reconstruction...");

   m_numEvent ++ ;
   //if (m_debug) cout<< "number of processed events now is : "<< m_numEvent <<endl;


   const xAOD::MissingETContainer* coreMet(0);
   RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(coreMet, m_coreName.Data(), m_event, m_store, msg()), "Failed retrieving MET Core.");

   const xAOD::MissingETAssociationMap* metMap = 0;
   RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(metMap,  m_mapName.Data(), m_event, m_store, msg()), "Failed retrieving MET Map.");

   std::vector<CP::SystematicSet>::const_iterator sysListItr;
   std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;

   //
   // get vector of string giving the Systematic names:
   //
   // load each object systematic. This is done at the execution level
   // as systematic containers have to exist. To avoid adding several
   // times the same systematic a check has to be performed on sysList
   //

   //add the syst for jets
   if(!m_runNominal && !m_jetSystematics.empty()){
     std::vector<std::string>* sysJetsNames(nullptr);
     RETURN_CHECK("METConstructor::initialize()", HelperFunctions::retrieve(sysJetsNames, m_jetSystematics, 0, m_store, msg()), "Failed retrieving object systematics.");

     for ( auto systName : *sysJetsNames ) {
       if (systName != "" && !(std::find(sysList.begin(), sysList.end(), CP::SystematicSet(systName)) != sysList.end())) sysList.push_back(CP::SystematicSet(systName));
       ATH_MSG_DEBUG("jet syst added is = "<< systName);
     }
   }

   //add the syst for electrons
   if(!m_runNominal && !m_eleSystematics.empty()){
     std::vector<std::string>* sysElectronsNames(nullptr);
     RETURN_CHECK("METConstructor::initialize()", HelperFunctions::retrieve(sysElectronsNames, m_eleSystematics, 0, m_store, msg()), "Failed retrieving object systematics.");

     for ( auto systName : *sysElectronsNames ) {
       if (systName != "" && !(std::find(sysList.begin(), sysList.end(), CP::SystematicSet(systName)) != sysList.end())  ) sysList.push_back(CP::SystematicSet(systName));
       ATH_MSG_DEBUG("ele syst added is = "<< systName);
     }
   }

   //add the syst for muons
   if(!m_runNominal && !m_muonSystematics.empty()){
     std::vector<std::string>* sysMuonsNames(nullptr);
     RETURN_CHECK("METConstructor::initialize()", HelperFunctions::retrieve(sysMuonsNames, m_muonSystematics, 0, m_store, msg()), "Failed retrieving object systematics.");

     for ( auto systName : *sysMuonsNames ) {
       if (systName != "" && !(std::find(sysList.begin(), sysList.end(), CP::SystematicSet(systName)) != sysList.end())) sysList.push_back(CP::SystematicSet(systName));
       ATH_MSG_DEBUG("muon syst added is = "<< systName);
     }
   }

   //add the syst for photons
   if(!m_runNominal && !m_phoSystematics.empty()){
     std::vector<std::string>* sysPhotonsNames(nullptr);
     RETURN_CHECK("METConstructor::initialize()", HelperFunctions::retrieve(sysPhotonsNames, m_phoSystematics, 0, m_store, msg()), "Failed retrieving object systematics.");

     for ( auto systName : *sysPhotonsNames ) {
       if (systName != "" && !(std::find(sysList.begin(), sysList.end(), CP::SystematicSet(systName)) != sysList.end())) sysList.push_back(CP::SystematicSet(systName));
       ATH_MSG_DEBUG("photon syst added is = "<< systName);
     }
   }

   // now start the loop over systematics
   for (sysListItr = sysList.begin(); sysListItr != sysList.end(); ++sysListItr) {  // loop over systematics

      //this is kind of annoying, but applySystematicVariation only takes a SystematicSet, but *sysListItr is a SystematicVariation.
      //We use the SystematicSet constructor which just takes a SystematicVariation
      //CP::SystematicSet iSysSet( (*sysListItr).name());
      //tell the tool that we are using this SystematicSet (of one SystematicVariation for now)
      //after this call, when we use applyCorrection, the given met term will be adjusted with this systematic applied
      //assert(    m_metSyst_handle->applySystematicVariation(sysList) );
      //
      // info from https://svnweb.cern.ch/trac/atlasoff/browser/Reconstruction/MET/METUtilities/trunk/util/example_METMaker_METSystematicsTool.cxx

      CP::SystematicSet iSysSet( (*sysListItr).name()); // to pass from SystematicVariation to SystematicSet
      //
      std::string sysListItrString;// just for convenience, to retrieve the containers
      sysListItrString= (*sysListItr).name();

      ATH_MSG_DEBUG(" loop over systematic = "<<sysListItr->name());

      vecOutContainerNames->push_back( sysListItr->name() );

      //create a met container, one for each syst
      xAOD::MissingETContainer* newMet = new xAOD::MissingETContainer();
      xAOD::MissingETAuxContainer* metAuxCont = new xAOD::MissingETAuxContainer();
      newMet->setStore(metAuxCont);

      metMap->resetObjSelectionFlags();

      // now retrieve the object containers and build the met
      // if the syst varied container exists take it, otherwise take the nominal one

      ///////////////////////
      ////// ELECTRONS  /////
      ///////////////////////

      if( m_inputElectrons.Length() > 0  && m_store->contains<xAOD::ElectronContainer>(m_inputElectrons.Data()+sysListItrString )) {
         const xAOD::ElectronContainer* eleCont(0);
         if ( m_store->contains<xAOD::ElectronContainer>(m_inputElectrons.Data()+sysListItrString ) ) {
           RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(eleCont, m_inputElectrons.Data()+sysListItrString, m_event, m_store, msg()), "Failed retrieving electron cont.");
           ATH_MSG_DEBUG("retrieving ele container "<<    m_inputElectrons.Data() +sysListItrString << " to be added to the met ");
         }else{
           RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(eleCont, m_inputElectrons.Data(), m_event, m_store, msg()), "Failed retrieving electron cont.");
         }

         if (m_doElectronCuts) {
           ConstDataVector<xAOD::ElectronContainer> metElectrons(SG::VIEW_ELEMENTS);
           for (const auto& el : *eleCont) if (CutsMETMaker::accept(el)) metElectrons.push_back(el);
           RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("RefEle", xAOD::Type::Electron, newMet, metElectrons.asDataVector(), metMap), "Failed rebuilding electron component.");
         } else {
           RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("RefEle", xAOD::Type::Electron, newMet, eleCont, metMap), "Failed rebuilding electron component.");
         }
      } // close "if( m_inputElectrons.Length() > 0 )"


      /////////////////////////
      /////////  PHOTONS  /////
      /////////////////////////

      if( m_inputPhotons.Length() > 0  && m_store->contains<xAOD::PhotonContainer>(m_inputPhotons.Data()+sysListItrString )) {
         const xAOD::PhotonContainer* phoCont(0);
         if ( m_store->contains<xAOD::PhotonContainer>(m_inputPhotons.Data()+sysListItrString ) ) {
           RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(phoCont, m_inputPhotons.Data()+sysListItrString, m_event, m_store, msg()), "Failed retrieving photon cont.");
           ATH_MSG_DEBUG("retrieving ph container "<<    m_inputPhotons.Data() +sysListItrString << " to be added to the met ");
         } else {
         RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(phoCont, m_inputPhotons.Data(), m_event, m_store, msg()), "Failed retrieving electron cont.");
      }

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

        RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("RefGamma", xAOD::Type::Photon, newMet, metPhotons.asDataVector(), metMap), "Failed rebuilding photon component.");

      } else {
        RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("RefGamma", xAOD::Type::Photon, newMet, phoCont, metMap), "Failed rebuilding photon component.");
       }
     }

     //////////////////////
     /////////  TAUS  /////
     //////////////////////

     ///// NOTE: for taus we are not applying systematics! since "m_inputTaus.Data()+sysListItrString" is not in Tstore!

     if( m_inputTaus.Length() > 0  && m_store->contains<xAOD::TauJetContainer>(m_inputTaus.Data()+sysListItrString ) ) {
        const xAOD::TauJetContainer* tauCont(0);
        if ( m_store->contains<xAOD::TauJetContainer>(m_inputTaus.Data()+sysListItrString ) ) {

          RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(tauCont, m_inputTaus.Data()+sysListItrString, m_event, m_store, msg()), "Failed retrieving photon cont.");
          ATH_MSG_DEBUG("retrieving tau container "<< m_inputTaus.Data()+sysListItrString << " to be added to the met ");

        } else {
        RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(tauCont, m_inputTaus.Data(), m_event, m_store, msg()), "Failed retrieving electron cont.");
      }

       if (m_doTauCuts) {
         ConstDataVector<xAOD::TauJetContainer> metTaus(SG::VIEW_ELEMENTS);
         for (const auto& tau : *tauCont) {

           if (tau->pt() < 20e3) continue;
           if (fabs(tau->eta()) > 2.37) continue;
           if (!m_tauSelTool->accept(tau)) continue;

           metTaus.push_back(tau);
         }
         RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("RefTau", xAOD::Type::Tau, newMet, metTaus.asDataVector(), metMap), "Failed rebuilding tau component.");
       } else {
         RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("RefTau", xAOD::Type::Tau, newMet, tauCont, metMap), "Failed rebuilding tau component.");
       }
     }

     ////////////////////
     //////  MUONS  /////
     ////////////////////

     if( m_inputMuons.Length() > 0  && m_store->contains<xAOD::MuonContainer>(m_inputMuons.Data()+sysListItrString ) ) {
       std::string m_inputMuons_Syst =  m_inputMuons.Data() +sysListItrString;
        const xAOD::MuonContainer* muonCont(0);
        if ( m_store->contains<xAOD::MuonContainer>(m_inputMuons.Data()+sysListItrString ) ) {
          RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(muonCont, m_inputMuons.Data()+sysListItrString, m_event, m_store, msg()), "Failed retrieving muon cont.");
        } else {
          RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(muonCont, m_inputMuons.Data(), m_event, m_store, msg()), "Failed retrieving electronmuon cont.");
        }
        if (m_doMuonCuts) {
          ConstDataVector<xAOD::MuonContainer> metMuons(SG::VIEW_ELEMENTS);
          for (const auto& mu : *muonCont) if (CutsMETMaker::accept(mu)) metMuons.push_back(mu);
          RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("Muons", xAOD::Type::Muon, newMet, metMuons.asDataVector(), metMap), "Failed rebuilding muon component.");
        } else {
          RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildMET("Muons", xAOD::Type::Muon, newMet, muonCont, metMap), "Failed rebuilding muon component.");
        }
     }

     ////////////////////
     //////  Jets  /////
     ////////////////////

     const xAOD::JetContainer* jetCont(0);
     std::string m_inputJets_Syst =  m_inputJets.Data() +sysListItrString;// just for convenience
     ATH_MSG_DEBUG(" the jet container name is : "<<m_inputJets_Syst);

     if ( m_store->contains<xAOD::JetContainer>(m_inputJets.Data()+sysListItrString ) ) {
       ATH_MSG_DEBUG("syst is = "<<sysListItrString);
       //RETURN_CHECK( "METConstructor::execute()", m_metSyst_handle->evtStore()->retrieve( jetCont,m_inputJets_Syst  ), "");// is this necessary?
       RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(jetCont,m_inputJets_Syst, m_event, m_store, msg()), " Failed retrieving jet cont.");
     } else {
       ATH_MSG_DEBUG(" not found this jet container : "<< m_inputJets.Data()+sysListItrString);
       //RETURN_CHECK( "METConstructor::execute()", m_metSyst_handle->evtStore()->retrieve( jetCont, m_inputJets.Data() ), "");// is this necessary?
       RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(jetCont, m_inputJets.Data(), m_event, m_store, msg()), " Failed retrieving jet cont.");
     }

     // the jet term and soft term(s) are built simultaneously using METMaker::rebuildJetMET(...) or METMaker::rebuildTrackMET(...)
     // to build MET using a calorimeter or track based jet term, respectively.
     // pass to rebuildJetMET calibrated jets (full container)
     //

     // NOTE: you have to set m_doJVTCut correctly when running!

     if ( m_useCaloJetTerm ) {
       RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildJetMET("RefJet", "SoftClus", "PVSoftTrk", newMet, jetCont, coreMet, metMap, m_doJVTCut), "Failed to build cluster-based jet/MET.");
     } else if ( m_useTrackJetTerm ) {
       RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->rebuildTrackMET("RefJetTrk", "PVSoftTrk", newMet, jetCont, coreMet, metMap, m_doJVTCut), "Failed to build track-based jet/MET.");
     } else {
       ATH_MSG_ERROR( "Both m_useCaloJetTerm and m_useTrackJetTerm appear to be set to 'false'. This should not happen. Please check your MET configuration file");
       return EL::StatusCode::FAILURE;
     }

     //now tell the m_metSyst_handle that we are using this SystematicSet (of one SystematicVariation for now)
     //after this call, when we use applyCorrection, the given met term will be adjusted with this systematic applied
     // assert(   m_metSyst_handle->applySystematicVariation(iSysSet) );
     //

     if (m_isMC) {
       if( m_metSyst_handle->applySystematicVariation(iSysSet) != CP::SystematicCode::Ok) {
         cout<<"Error !!! not able to applySystematicVariation "<< endl;
       }
     }

     //get the soft cluster term, and applyCorrection
     xAOD::MissingET * softClusMet = (*newMet)["SoftClus"];
     //assert( softClusMet != 0); //check we retrieved the clust term
     if( m_isMC && m_metSyst_handle->applyCorrection(*softClusMet) != CP::CorrectionCode::Ok) {
       ATH_MSG_ERROR( "Could not apply correction to soft clus met !!!! ");
     }
     ATH_MSG_DEBUG("Soft cluster met term met : " << softClusMet->met());

     //get the track soft term, and applyCorrection
     xAOD::MissingET * softTrkMet = (*newMet)["PVSoftTrk"];
     if( m_isMC && m_metSyst_handle->applyCorrection(*softTrkMet) != CP::CorrectionCode::Ok) {
       ATH_MSG_ERROR( "Could not apply correction to soft track met !!!! ");
     }
     ATH_MSG_DEBUG("track met soft term : " << softTrkMet->met());

     //only for track jets
     /*xAOD::MissingET * jetMet = (*newMet)["RefJet"];
       if( (*jetMet)->applyCorrection(iSysSet) != CP::CorrectionCode::Ok) {
       ATH_MSG_ERROR( "Could not apply correction to jet met !!!! ");
       };
       std::cout << "Jet met term met " << jetMet->met() << std::endl;*/

     // build met:

     RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->buildMETSum("FinalClus", newMet, MissingETBase::Source::LCTopo), "Failed to build FinalClus MET.");
     RETURN_CHECK("METConstructor::execute()", m_metmaker_handle->buildMETSum("FinalTrk",  newMet, MissingETBase::Source::Track),  "Failed to build FinalTrk MET.");

     RETURN_CHECK("METConstructor::execute()", m_store->record(newMet, (m_outputContainer+sysListItr->name()).Data() ), "Failed to store MET output container.");
     RETURN_CHECK("METConstructor::execute()", m_store->record(metAuxCont, (m_outputContainer+sysListItr->name() + "Aux.").Data()), "Failed to store MET output container.");

     ATH_MSG_DEBUG(" FinalClus met, for syst " << sysListItr->name() << " is = " << (*newMet->find("FinalClus"))->met());
     ATH_MSG_DEBUG(" FinalTrk met, for syst " << sysListItr->name() << " is = " << (*newMet->find("FinalTrk"))->met());
     ATH_MSG_DEBUG("storing met container :  " << (m_outputContainer+ sysListItr->name()).Data());
     ATH_MSG_DEBUG("storing  Aux met container :  "<< (m_outputContainer+ sysListItr->name() + "Aux.").Data());


     /* something causes a crash down here
     if ( m_debug ) {
       const xAOD::MissingETContainer* oldMet(0);
       RETURN_CHECK("METConstructor::execute()", HelperFunctions::retrieve(oldMet, m_referenceMETContainer.Data(), m_event, m_store, msg()) ,"");
       //xAOD::MissingETContainer::const_iterator final(oldMet->find("FinalClus"));
       //xAOD::MissingETContainer::const_iterator newfinal(newMet->find("FinalClus"));
       ATH_MSG_INFO( ">>>>>>>>>>>>>>>>>>>>>>>>>>>>");
       if( m_inputElectrons.Length() > 0 ) ATH_MSG_INFO( "RefEle:     old=%8.f  new=%8.f", (*oldMet->find("RefEle"))->met(), (*newMet->find("RefEle"))->met());
       if( m_inputPhotons.Length() > 0 )   ATH_MSG_INFO( "RefPhoton:  old=%8.f  new=%8.f", (*oldMet->find("RefGamma"))->met(), (*newMet->find("RefGamma"))->met());
       if( m_inputTaus.Length() > 0 )      ATH_MSG_INFO( "RefTau:     old=%8.f  new=%8.f", (*oldMet->find("RefTau"))->met(), (*newMet->find("RefTau"))->met());
       if( m_inputMuons.Length() > 0 )     ATH_MSG_INFO( "RefMuon:    old=%8.f  new=%8.f", (*oldMet->find("Muons"))->met(), (*newMet->find("Muons"))->met());
       ATH_MSG_INFO( "RefJet:       old=%8.f  new=%8.f", (*oldMet->find("RefJet"))->met(), (*newMet->find("RefJet"))->met());
       ATH_MSG_INFO( "SoftClus:     old=%8.f  new=%8.f", (*oldMet->find("SoftClus"))->met(), (*newMet->find("SoftClus"))->met());
       ATH_MSG_INFO( "PVSoftTrk:    old=%8.f  new=%8.f", (*oldMet->find("PVSoftTrk"))->met(), (*newMet->find("PVSoftTrk"))->met());
       ATH_MSG_INFO( "  ");
       ATH_MSG_INFO( "FinalClus:    old=%8.f  new=%8.f", (*oldMet->find("FinalClus"))->met(), (*newMet->find("FinalClus"))->met());
       ATH_MSG_INFO( "       >>>>> R=%.3f",          (*oldMet->find("FinalClus"))->met()/ (*newMet->find("FinalClus"))->met());
       // ATH_MSG_INFO( "FinalTrk:     old=%8.f  new=%8.f", (*oldMet->find("FinalTrk"))->met(), (*newMet->find("FinalTrk"))->met());
       // ATH_MSG_INFO( "       >>>>> R=%.3f",          (*oldMet->find("FinalTrk"))->met()/ (*newMet->find("FinalTrk"))->met());
     }
     */


   } //end loop over systematics

   // might have already been stored by another execution of this algo
   // or by a previous iteration of the same
   if ( !m_store->contains< std::vector<std::string> >( m_outputAlgoSystNames ) ) {
      RETURN_CHECK( "METConstructor::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");
   }

   ATH_EXEC_VERBOSE(m_store->print());// print TStore content

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode METConstructor :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ATH_MSG_DEBUG( "Calling postExecute");

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

  ATH_MSG_INFO( "Deleting tool instances...");

//  if (m_metmaker_handle) {
//    delete m_metmaker_handle;
//    m_metmaker_handle = 0;
//  }

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

  ATH_MSG_INFO( "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
