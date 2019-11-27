#include <iostream>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODAnaHelpers/METConstructor.h"

#include "METUtilities/CutsMETMaker.h"
#include "PATInterfaces/SystematicVariation.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODJet/JetContainer.h"

#include "xAODCore/ShallowCopy.h"

#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#include "xAODMissingET/MissingETComposition.h"
#include "xAODMissingET/MissingETAssociationMap.h"

#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include "TEnv.h"
#include "TSystem.h"

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

  ANA_MSG_INFO( "Calling setupJob");

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
  ANA_CHECK( xAH::Algorithm::algInitialize());
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

  ANA_MSG_INFO( "Initializing METConstructor Interface...");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_DEBUG( "Is MC? " << isMC() );

  //////////// IMETMaker ////////////////
  if ( m_dofJVTCut ) {
    ANA_CHECK(m_metmaker_handle.setProperty("JetRejectionDec", "passFJVT"));
  }
  if ( m_doPFlow ) {
    ANA_CHECK(m_metmaker_handle.setProperty("DoPFlow", true));
  }
  ANA_CHECK(m_metmaker_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_metmaker_handle);

  ///////////// IMETSystematicsTool ///////////////////
  if ( m_doPFlow ) { // do TST
    ANA_CHECK(m_metSyst_handle.setProperty("ConfigSoftTrkFile", "TrackSoftTerms-pflow.config"));
  }
  ANA_CHECK(m_metSyst_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_metSyst_handle);

  ANA_CHECK(m_tauSelTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_tauSelTool_handle);

  //////////// IMETSignificance ////////////////
  if ( m_calculateSignificance ) {
    ANA_CHECK( m_metSignificance_handle.setProperty("TreatPUJets", m_significanceTreatPUJets) );
    ANA_CHECK( m_metSignificance_handle.setProperty("SoftTermReso", m_significanceSoftTermReso) );

    // For AFII samples
    if ( isFastSim() ){ 
      ANA_MSG_INFO( "Setting simulation flavour to AFII");
      ANA_CHECK( m_metSignificance_handle.setProperty("IsAFII", true));
    }
    ANA_CHECK( m_metSignificance_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_metSignificance_handle);
  }

  ANA_MSG_INFO( "METConstructor Interface " << m_name << " succesfully initialized!");

  //use the helper function getlistofsystematics:

  // run syst
  if ( !m_runNominal && !m_systName.empty() ) { //  m_systName is set by default to m_systName= "All", do not change it
    // get the syst from met syst tool
    const CP::SystematicSet recSyst = m_metSyst_handle->recommendedSystematics();
    m_sysList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, msg() );
  } else { //run nominal
    m_sysList.push_back(CP::SystematicSet()); // add empty systematic (Nominal case)
  }

  ANA_MSG_INFO("Will be using METSystematicsTool systematic:");
  for ( const auto& syst_it : m_sysList ) {
    ANA_MSG_INFO("\t " << syst_it.name());
  }

  m_numEvent = 0; //just as a check

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_sysList, m_name, fileMD);
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode METConstructor :: execute ()
{
   // Here you do everything that needs to be done on every single
   // events, e.g. read input variables, apply cuts, and fill
   // histograms and trees.  This is where most of your actual analysis
   // code will go.

   ANA_MSG_DEBUG( "Performing MET reconstruction...");

   m_numEvent ++ ;
   //ANA_MSG_DEBUG("number of processed events now is : "<< m_numEvent);

   const xAOD::EventInfo* eventInfo(nullptr);
   ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()));

   const xAOD::MissingETContainer* coreMet(0);
   ANA_CHECK( HelperFunctions::retrieve(coreMet, m_coreName, m_event, m_store, msg()));

   const xAOD::MissingETAssociationMap* metMap = 0;
   ANA_CHECK( HelperFunctions::retrieve(metMap, m_mapName, m_event, m_store, msg()));

   std::vector<CP::SystematicSet>::const_iterator sysListItr;
   auto vecOutContainerNames = std::make_unique< std::vector< std::string > >();

   //
   // get vector of string giving the Systematic names:
   //
   // load each object systematic. This is done at the execution level
   // as systematic containers have to exist. To avoid adding several
   // times the same systematic a check has to be performed on sysList
   //

   //add the syst for jets
   std::vector<std::string>* sysJetsNames(nullptr);
   if(!m_runNominal && !m_jetSystematics.empty()){
     ANA_CHECK( HelperFunctions::retrieve(sysJetsNames, m_jetSystematics, 0, m_store, msg()));

     for ( auto systName : *sysJetsNames ) {
       if (systName != "" && !(std::find(m_sysList.begin(), m_sysList.end(), CP::SystematicSet(systName)) != m_sysList.end())) m_sysList.push_back(CP::SystematicSet(systName));
       ANA_MSG_DEBUG("jet syst added is = "<< systName);
     }
   }

   //add the syst for electrons
   std::vector<std::string>* sysElectronsNames(nullptr);
   if(!m_runNominal && !m_eleSystematics.empty()){
     ANA_CHECK( HelperFunctions::retrieve(sysElectronsNames, m_eleSystematics, 0, m_store, msg()));

     for ( auto systName : *sysElectronsNames ) {
       if (systName != "" && !(std::find(m_sysList.begin(), m_sysList.end(), CP::SystematicSet(systName)) != m_sysList.end())  ) m_sysList.push_back(CP::SystematicSet(systName));
       ANA_MSG_DEBUG("ele syst added is = "<< systName);
     }
   }

   //add the syst for muons
   std::vector<std::string>* sysMuonsNames(nullptr);
   if(!m_runNominal && !m_muonSystematics.empty()){
     ANA_CHECK( HelperFunctions::retrieve(sysMuonsNames, m_muonSystematics, 0, m_store, msg()));

     for ( auto systName : *sysMuonsNames ) {
       if (systName != "" && !(std::find(m_sysList.begin(), m_sysList.end(), CP::SystematicSet(systName)) != m_sysList.end())) m_sysList.push_back(CP::SystematicSet(systName));
       ANA_MSG_DEBUG("muon syst added is = "<< systName);
     }
   }

   //add the syst for tau
   std::vector<std::string>* sysTausNames(nullptr);
   if(!m_runNominal && !m_tauSystematics.empty()){
     ANA_CHECK( HelperFunctions::retrieve(sysTausNames, m_tauSystematics, 0, m_store, msg()));

     for ( auto systName : *sysTausNames ) {
       if (systName != "" && !(std::find(m_sysList.begin(), m_sysList.end(), CP::SystematicSet(systName)) != m_sysList.end())) m_sysList.push_back(CP::SystematicSet(systName));
       ANA_MSG_DEBUG("tau syst added is = "<< systName);
     }
   }

   //add the syst for photons
   std::vector<std::string>* sysPhotonsNames(nullptr);
   if(!m_runNominal && !m_phoSystematics.empty()){
     ANA_CHECK( HelperFunctions::retrieve(sysPhotonsNames, m_phoSystematics, 0, m_store, msg()));

     for ( auto systName : *sysPhotonsNames ) {
       if (systName != "" && !(std::find(m_sysList.begin(), m_sysList.end(), CP::SystematicSet(systName)) != m_sysList.end())) m_sysList.push_back(CP::SystematicSet(systName));
       ANA_MSG_DEBUG("photon syst added is = "<< systName);
     }
   }

   // now start the loop over systematics
   for (sysListItr = m_sysList.begin(); sysListItr != m_sysList.end(); ++sysListItr) {  // loop over systematics

      //this is kind of annoying, but applySystematicVariation only takes a SystematicSet, but *sysListItr is a SystematicVariation.
      //We use the SystematicSet constructor which just takes a SystematicVariation
      //CP::SystematicSet systSet({systName});
      //tell the tool that we are using this SystematicSet (of one SystematicVariation for now)
      //after this call, when we use applyCorrection, the given met term will be adjusted with this systematic applied
      //assert(    m_metSyst_handle->applySystematicVariation(sysList) );
      //
      // info from https://svnweb.cern.ch/trac/atlasoff/browser/Reconstruction/MET/METUtilities/trunk/util/example_METMaker_METSystematicsTool.cxx

      // just for convenience, to retrieve the containers
      std::string systName = (*sysListItr).name();

      ANA_MSG_DEBUG(" loop over systematic = " << systName);

      vecOutContainerNames->push_back( systName );

      //create a met container, one for each syst
      auto newMet = std::make_unique<xAOD::MissingETContainer>();
      auto metAuxCont = std::make_unique<xAOD::MissingETAuxContainer>();
      newMet->setStore(metAuxCont.get());

      metMap->resetObjSelectionFlags();

      // now retrieve the object containers and build the met
      // if the syst varied container exists take it, otherwise take the nominal one

      ///////////////////////
      ////// ELECTRONS  /////
      ///////////////////////

      if ( !m_inputElectrons.empty() ) {
         const xAOD::ElectronContainer* eleCont(0);
         std::string suffix = "";
         if (sysElectronsNames && std::find(std::begin(*sysElectronsNames), std::end(*sysElectronsNames), systName) != std::end(*sysElectronsNames)) {
           ANA_MSG_DEBUG("doing electron systematics");
           suffix = systName;
         }

         if ( m_store->contains<xAOD::ElectronContainer>(m_inputElectrons + suffix) || m_event->contains<xAOD::ElectronContainer>(m_inputElectrons + suffix) ) {
           ANA_CHECK( HelperFunctions::retrieve(eleCont, m_inputElectrons + suffix, m_event, m_store, msg()));
           ANA_MSG_DEBUG("retrieving ele container " << m_inputElectrons + suffix << " to be added to the MET");
         } else {
           ANA_MSG_DEBUG("container " << m_inputElectrons + suffix << " not available upstream - skipping systematics");
           continue;
         }

         ANA_MSG_DEBUG("rebuilding MET term: RefEle");
         if (m_doElectronCuts) {
           ConstDataVector<xAOD::ElectronContainer> metElectrons(SG::VIEW_ELEMENTS);
           for (const auto& el : *eleCont) if (CutsMETMaker::accept(el)) metElectrons.push_back(el);
           ANA_CHECK( m_metmaker_handle->rebuildMET("RefEle", xAOD::Type::Electron, newMet.get(), metElectrons.asDataVector(), metMap));
         } else {
           ANA_CHECK( m_metmaker_handle->rebuildMET("RefEle", xAOD::Type::Electron, newMet.get(), eleCont, metMap));
         }
      } // close "if( m_inputElectrons.Length() > 0 )"


      /////////////////////////
      /////////  PHOTONS  /////
      /////////////////////////

      if ( !m_inputPhotons.empty() ) {
         const xAOD::PhotonContainer* phoCont(0);
         std::string suffix = "";
         if (sysPhotonsNames && std::find(std::begin(*sysPhotonsNames), std::end(*sysPhotonsNames), systName) != std::end(*sysPhotonsNames)) {
           ANA_MSG_DEBUG("doing photon systematics");
           suffix = systName;
         }

         if ( m_store->contains<xAOD::PhotonContainer>(m_inputPhotons + suffix) || m_event->contains<xAOD::PhotonContainer>(m_inputPhotons + suffix) ) {
           ANA_CHECK( HelperFunctions::retrieve(phoCont, m_inputPhotons + suffix, m_event, m_store, msg()));
           ANA_MSG_DEBUG("retrieving ph container " << m_inputPhotons + suffix << " to be added to the MET");
         } else {
           ANA_MSG_DEBUG("container " << m_inputPhotons + suffix << " not available upstream - skipping systematics");
           continue;
         }

      ANA_MSG_DEBUG("rebuilding MET term: RefGamma");
      if (m_doPhotonCuts) {
        ConstDataVector<xAOD::PhotonContainer> metPhotons(SG::VIEW_ELEMENTS);
        for (const auto& ph : *phoCont) {

          bool testPID = 0;
          ph->passSelection(testPID, "Tight");
          if( !testPID ) continue;

          //ANA_MSG_VERBOSE("Photon author = " << ph->author() << " test " << (ph->author()&20));
          if (!(ph->author() & 20)) continue;

          if (ph->pt() < 25e3) continue;

          float feta = fabs(ph->eta());
          if (feta > 2.37 || (1.37 < feta && feta < 1.52)) continue;

          metPhotons.push_back(ph);
        }

        ANA_CHECK( m_metmaker_handle->rebuildMET("RefGamma", xAOD::Type::Photon, newMet.get(), metPhotons.asDataVector(), metMap));

      } else {
        ANA_CHECK( m_metmaker_handle->rebuildMET("RefGamma", xAOD::Type::Photon, newMet.get(), phoCont, metMap));
       }
     }

     //////////////////////
     /////////  TAUS  /////
     //////////////////////

     if ( !m_inputTaus.empty() ) {
        const xAOD::TauJetContainer* tauCont(0);
        std::string suffix = "";
        if (sysTausNames && std::find(std::begin(*sysTausNames), std::end(*sysTausNames), systName) != std::end(*sysTausNames)) {
          ANA_MSG_DEBUG("doing tau systematics");
          suffix = systName;
        }

        if ( m_store->contains<xAOD::TauJetContainer>(m_inputTaus + suffix) || m_event->contains<xAOD::TauJetContainer>(m_inputTaus + suffix) ) {
          ANA_CHECK( HelperFunctions::retrieve(tauCont, m_inputTaus + suffix, m_event, m_store, msg()));
          ANA_MSG_DEBUG("retrieving tau container " << m_inputTaus + suffix << " to be added to the MET");
        } else {
            ANA_MSG_DEBUG("container " << m_inputTaus + suffix << " not available upstream - skipping systematics");
            continue;
      }

      ANA_MSG_DEBUG("rebuilding MET term: RefTau");
       if (m_doTauCuts) {
         ConstDataVector<xAOD::TauJetContainer> metTaus(SG::VIEW_ELEMENTS);
         for (const auto& tau : *tauCont) {

           if (tau->pt() < 20e3) continue;
           if (fabs(tau->eta()) > 2.37) continue;
           if (!m_tauSelTool_handle->accept(tau)) continue;

           metTaus.push_back(tau);
         }
         ANA_CHECK( m_metmaker_handle->rebuildMET("RefTau", xAOD::Type::Tau, newMet.get(), metTaus.asDataVector(), metMap));
       } else {
         ANA_CHECK( m_metmaker_handle->rebuildMET("RefTau", xAOD::Type::Tau, newMet.get(), tauCont, metMap));
       }
     }

     ////////////////////
     //////  MUONS  /////
     ////////////////////

     if ( !m_inputMuons.empty() ) {
        const xAOD::MuonContainer* muonCont(0);
        std::string suffix = "";
        if (sysMuonsNames && std::find(std::begin(*sysMuonsNames), std::end(*sysMuonsNames), systName) != std::end(*sysMuonsNames)) {
          ANA_MSG_DEBUG("doing muon systematics");
          suffix = systName;
        }

        if ( m_store->contains<xAOD::MuonContainer>(m_inputMuons + suffix) || m_event->contains<xAOD::MuonContainer>(m_inputMuons + suffix) ) {
          ANA_CHECK( HelperFunctions::retrieve(muonCont, m_inputMuons + suffix, m_event, m_store, msg()));
          ANA_MSG_DEBUG("retrieving muon container " << m_inputMuons + suffix << " to be added to the MET");
        } else {
          ANA_MSG_DEBUG("container " << m_inputMuons + suffix << " not available upstream - skipping systematics");
          continue;
        }

        ANA_MSG_DEBUG("rebuilding MET term: Muons");
        if (m_doMuonCuts) {
          ConstDataVector<xAOD::MuonContainer> metMuons(SG::VIEW_ELEMENTS);
          for (const auto& mu : *muonCont) if (CutsMETMaker::accept(mu)) metMuons.push_back(mu);
          ANA_CHECK( m_metmaker_handle->rebuildMET("Muons", xAOD::Type::Muon, newMet.get(), metMuons.asDataVector(), metMap));
        } else {
          ANA_CHECK( m_metmaker_handle->rebuildMET("Muons", xAOD::Type::Muon, newMet.get(), muonCont, metMap));
        }
     }

     ////////////////////
     //////  Jets  /////
     ////////////////////

     if ( m_inputJets.empty() ) {
       ANA_MSG_ERROR("Jets are required for MET calculation.");
       return EL::StatusCode::FAILURE;
     }

     const xAOD::JetContainer* jetCont(0);
     std::string suffix = "";
     if (sysJetsNames && std::find(std::begin(*sysJetsNames), std::end(*sysJetsNames), systName) != std::end(*sysJetsNames)) {
       ANA_MSG_DEBUG("doing muon systematics");
       suffix = systName;
     }

     if ( m_store->contains<xAOD::JetContainer>(m_inputJets + suffix) || m_event->contains<xAOD::JetContainer>(m_inputJets + suffix) ) {
       ANA_CHECK( HelperFunctions::retrieve(jetCont, m_inputJets + suffix, m_event, m_store, msg()));
       ANA_MSG_DEBUG("retrieving jet container " << m_inputJets + suffix << " to be added to the MET");
     } else {
       ANA_MSG_DEBUG("container " << m_inputJets + suffix << " not available upstream - skipping systematics");
       continue;
     }

     // the jet term and soft term(s) are built simultaneously using METMaker::rebuildJetMET(...) or METMaker::rebuildTrackMET(...)
     // to build MET using a calorimeter or track based jet term, respectively.
     // pass to rebuildJetMET calibrated jets (full container)
     //

     // NOTE: you have to set m_doJVTCut correctly when running!

     // By default: rebuild MET using jets without soft cluster terms (just TST, no CST)
     // You can configure to add Cluster Soft Term (only affects the "use Jets" option)
     //         or to rebuild MET using the Tracks in Calorimeter Jets which doesn't make sense to have CST
     if( !m_rebuildUsingTracksInJets ) {
       if( m_addSoftClusterTerms ){
         ANA_MSG_DEBUG("rebuilding MET term: RefJet + SoftClus + PVSoftTrk");
         ANA_CHECK( m_metmaker_handle->rebuildJetMET("RefJet", "SoftClus", "PVSoftTrk", newMet.get(), jetCont, coreMet, metMap, m_doJVTCut));
       } else {
         ANA_MSG_DEBUG("rebuilding MET term: RefJet + PVSoftTrk");
         ANA_CHECK( m_metmaker_handle->rebuildJetMET("RefJet", "PVSoftTrk", newMet.get(), jetCont, coreMet, metMap, m_doJVTCut));
       }
     } else {
       ANA_MSG_DEBUG("rebuilding MET term: RefJetTrk");
       ANA_CHECK( m_metmaker_handle->rebuildTrackMET("RefJetTrk", "PVSoftTrk", newMet.get(), jetCont, coreMet, metMap, m_doJVTCut));
     }

     //now tell the m_metSyst_handle that we are using this SystematicSet (of one SystematicVariation for now)
     //after this call, when we use applyCorrection, the given met term will be adjusted with this systematic applied
     // assert(   m_metSyst_handle->applySystematicVariation(systSet) );
     if (isMC()) {
       CP::SystematicSet systSet({systName}); // to pass from SystematicVariation to SystematicSet
       if( m_metSyst_handle->applySystematicVariation(systSet) != CP::SystematicCode::Ok) {
         ANA_MSG_ERROR("not able to applySystematicVariation ");
       }
     }

     if(!m_rebuildUsingTracksInJets && m_addSoftClusterTerms){
       //get the soft cluster term, and applyCorrection
       xAOD::MissingET * softClusMet = (*newMet)["SoftClus"];
       //assert( softClusMet != 0); //check we retrieved the clust term
       if( isMC() && m_metSyst_handle->applyCorrection(*softClusMet) != CP::CorrectionCode::Ok) {
         ANA_MSG_ERROR( "Could not apply correction to soft clus met !!!! ");
       }
       ANA_MSG_DEBUG("Soft cluster met term met : " << softClusMet->met());
     }

     //get the track soft term, and applyCorrection
     xAOD::MissingET * softTrkMet = (*newMet)["PVSoftTrk"];
     if( isMC() && m_metSyst_handle->applyCorrection(*softTrkMet) != CP::CorrectionCode::Ok) {
       ANA_MSG_ERROR( "Could not apply correction to soft track met !!!! ");
     }
     ANA_MSG_DEBUG("track met soft term : " << softTrkMet->met());

     //only for track jets
     /*xAOD::MissingET * jetMet = (*newMet)["RefJet"];
       if( (*jetMet)->applyCorrection(iSysSet) != CP::CorrectionCode::Ok) {
       ANA_MSG_ERROR( "Could not apply correction to jet met !!!! ");
       };
       ANA_MSG_DEBUG("Jet met term met " << jetMet->met());*/

     // build met:

     ANA_CHECK( m_metmaker_handle->buildMETSum("FinalClus", newMet.get(), MissingETBase::Source::LCTopo));
     ANA_CHECK( m_metmaker_handle->buildMETSum("FinalTrk",  newMet.get(), MissingETBase::Source::Track));

     // Calculate MET significance if enabled
     if ( m_calculateSignificance ) {
       std::vector<std::string> totalMETNames = {"FinalTrk", "FinalClus"};

       for ( const std::string &name : totalMETNames ) {
         // Calculate MET significance
         if ( !m_rebuildUsingTracksInJets ) {
           ANA_CHECK( m_metSignificance_handle->varianceMET(newMet.get(), eventInfo->averageInteractionsPerCrossing(), "RefJet", "PVSoftTrk", name) );
         } else {
           ANA_CHECK( m_metSignificance_handle->varianceMET(newMet.get(), eventInfo->averageInteractionsPerCrossing(), "RefJetTrk", "PVSoftTrk", name) );
         }

         // Decorate MET object with results
         const xAOD::MissingET *met = *(newMet->find(name));
         if ( !met ) {
           ANA_MSG_WARNING( "Cannot find MET object with name: " << name );
         }

         met->auxdecor<double>("METOverSqrtSumET") = m_metSignificance_handle->GetMETOverSqrtSumET();
         met->auxdecor<double>("METOverSqrtHT") = m_metSignificance_handle->GetMETOverSqrtHT();
         met->auxdecor<double>("Significance") = m_metSignificance_handle->GetSignificance();
         met->auxdecor<double>("SigDirectional") = m_metSignificance_handle->GetSigDirectional();
         met->auxdecor<double>("Rho") = m_metSignificance_handle->GetRho();
         met->auxdecor<double>("VarL") = m_metSignificance_handle->GetVarL();
         met->auxdecor<double>("VarT") = m_metSignificance_handle->GetVarT();
       }
     }

     ANA_MSG_DEBUG(" FinalClus met, for syst " << systName << " is = " << (*newMet->find("FinalClus"))->met());
     ANA_MSG_DEBUG(" FinalTrk met, for syst " << systName << " is = " << (*newMet->find("FinalTrk"))->met());
     ANA_MSG_DEBUG("storing met container :  " << (m_outputContainer + systName));
     ANA_MSG_DEBUG("storing  Aux met container :  "<< (m_outputContainer + systName + "Aux."));

     // Debug compare reference and recomputed MET
     if ( m_msgLevel <= MSG::DEBUG ) {
       const xAOD::MissingETContainer* oldMet(0);
       ANA_CHECK( HelperFunctions::retrieve(oldMet, m_referenceMETContainer, m_event, m_store, msg()) );

       ANA_MSG_DEBUG( ">>>>>>>>>>>>>>>>>>>>>>>>>>>>");
       if ( !m_inputElectrons.empty() ) ANA_MSG_DEBUG( "RefEle:       old=" << (*oldMet->find("RefEle"))->met() << " \tnew" << (*newMet->find("RefEle"))->met());
       if ( !m_inputPhotons.empty() )   ANA_MSG_DEBUG( "RefPhoton:    old=" << (*oldMet->find("RefGamma"))->met() << " \tnew" << (*newMet->find("RefGamma"))->met());
       if ( !m_inputTaus.empty() )      ANA_MSG_DEBUG( "RefTau:       old=" << (*oldMet->find("RefTau"))->met() << " \tnew" << (*newMet->find("RefTau"))->met());
       if ( !m_inputMuons.empty() )     ANA_MSG_DEBUG( "RefMuon:      old=" << (*oldMet->find("Muons"))->met() << " \tnew" << (*newMet->find("Muons"))->met());
       ANA_MSG_DEBUG( "RefJet:       old=" << (*oldMet->find("RefJet"))->met() << " \tnew" << (*newMet->find("RefJet"))->met());
       if ( m_addSoftClusterTerms ) {
         ANA_MSG_DEBUG( "SoftClus:     old=" << (*oldMet->find("SoftClus"))->met() << " \tnew" << (*newMet->find("SoftClus"))->met());
       }
       ANA_MSG_DEBUG( "PVSoftTrk:    old=" << (*oldMet->find("PVSoftTrk"))->met() << " \tnew" << (*newMet->find("PVSoftTrk"))->met());
       ANA_MSG_DEBUG( "  ");
       ANA_MSG_DEBUG( "FinalClus:    old=" << (*oldMet->find("FinalClus"))->met() << " \tnew" << (*newMet->find("FinalClus"))->met());
       ANA_MSG_DEBUG( "       >>>>> R=" << (*oldMet->find("FinalClus"))->met()/ (*newMet->find("FinalClus"))->met());
       ANA_MSG_INFO( "FinalTrk:     old=" << (*oldMet->find("FinalTrk"))->met() << " \tnew" << (*newMet->find("FinalTrk"))->met());
       ANA_MSG_INFO( "       >>>>> R=" << (*oldMet->find("FinalTrk"))->met()/ (*newMet->find("FinalTrk"))->met());
     }

     // Store MET
     ANA_CHECK( m_store->record( std::move(newMet), (m_outputContainer + systName) ));
     ANA_CHECK( m_store->record( std::move(metAuxCont), (m_outputContainer + systName + "Aux.")));

   } //end loop over systematics

   // might have already been stored by another execution of this algo
   // or by a previous iteration of the same
   if ( !m_store->contains< std::vector<std::string> >( m_outputAlgoSystNames ) ) {
      ANA_CHECK( m_store->record( std::move(vecOutContainerNames), m_outputAlgoSystNames));
   }

   if(msgLvl(MSG::VERBOSE)) m_store->print();

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode METConstructor :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG( "Calling postExecute");

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

  ANA_MSG_INFO( "Deleting tool instances...");

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

  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}
