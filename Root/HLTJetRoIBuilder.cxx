/******************************************
 *
 * This class builds HLT jets and thier associated objects
 *
 * John Alison (john.alison@cern.ch)
 *
 *
 ******************************************/

// c++ include(s):
#include <iostream>
#include <vector>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#include "xAODJet/Jet.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODBase/IParticleContainer.h"
#include "xAODBase/IParticle.h"
#include "AthContainers/ConstDataVector.h"
#include "AthContainers/DataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HLTJetRoIBuilder.h"

#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// this is needed to distribute the algorithm to the workers
ClassImp(HLTJetRoIBuilder)

HLTJetRoIBuilder :: HLTJetRoIBuilder () :
  Algorithm("HLTJetRoIBuilder")
{
}


EL::StatusCode HLTJetRoIBuilder :: setupJob (EL::Job& job)
{
  ANA_MSG_DEBUG( "Calling setupJob");
  job.useXAOD ();
  xAOD::Init( "HLTJetRoIBuilder" ).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: histInitialize ()
{
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: changeInput (bool /*firstFile*/)
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: initialize ()
{

  ANA_MSG_DEBUG( "Initializing HLTJetRoIBuilder Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // Grab the TrigDecTool from the ToolStore
  if(!setToolName(m_trigDecTool_handle, m_trigDecTool_name)){
    ANA_MSG_FATAL("A configured " << m_trigDecTool_handle.typeAndName() << " must have been previously created! Are you creating one in xAH::BasicEventSelection?" );
    return EL::StatusCode::FAILURE;
  }
  ANA_CHECK( m_trigDecTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);

  if(m_trigItem.find("split") != std::string::npos){
    m_jetName = "SplitJet";
    m_vtxName = "xPrimVx";
  }

  if(m_trigItem.find("gsc") != std::string::npos){
    m_jetName = "GSCJet";
    m_vtxName = "xPrimVx";
  }

  if(m_trigItem.find("FTK") != std::string::npos){
    m_trkName = "InDetTrigTrackingxAODCnv_Bjet_FTK";
    m_vtxName = "";
    //m_vtxName = "HLT_PrimVertexFTK";
  }

  if(m_trigItem.find("FTKRefit") != std::string::npos){
    m_trkName = "InDetTrigTrackingxAODCnv_Bjet_FTKRefit";
  }

  if(m_trigItem.find("FTKVtx") != std::string::npos){
    m_trkName = "InDetTrigTrackingxAODCnv_Bjet_IDTrig";
  }

  ANA_MSG_INFO("HLTJetRoIBuilder::Configured " << m_name << " with ");
  ANA_MSG_INFO("\tm_trigItem: " << m_trigItem);
  ANA_MSG_INFO("\tm_trigItemVeto: " << m_trigItemVeto);
  ANA_MSG_INFO("\tm_trkName: " << m_trkName);
  ANA_MSG_INFO("\tm_vtxName: " << m_vtxName);
  ANA_MSG_INFO("\tm_jetName: " << m_jetName);

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode HLTJetRoIBuilder :: execute ()
{
  ANA_MSG_DEBUG( "Doing HLT JEt ROI Building... ");

  if(m_doHLTBJet){
    return buildHLTBJets();
  }else if(m_doHLTJet){
    return buildHLTJets();
  }



  if(msgLvl(MSG::VERBOSE)) m_store->print();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: buildHLTBJets ()
{
  auto triggerChainGroup = m_trigDecTool_handle->getChainGroup(m_trigItem);

  std::vector<std::string> triggersUsed = triggerChainGroup->getListOfTriggers();
  std::vector<std::string> triggersAfterVeto;
  for(std::string trig : triggersUsed){
    if(trig.find("antimatchdr") != std::string::npos){
      continue;
    }

    if((m_trigItemVeto != "") && (trig.find(m_trigItemVeto) != std::string::npos)){
      continue;
    }
    triggersAfterVeto.push_back(trig);
  }

  std::string m_trigItemAfterVeto = "";
  bool firstItem = true;
  for(std::string trig : triggersAfterVeto){
    if(firstItem) m_trigItemAfterVeto += trig;
    else          m_trigItemAfterVeto += "||"+trig;
    firstItem = false;
  }

  ANA_MSG_DEBUG(m_name << " " << m_trigItem << " matches");
  ANA_MSG_DEBUG(m_trigItemAfterVeto);
  auto triggerChainGroupAfterVeto = m_trigDecTool_handle->getChainGroup(m_trigItemAfterVeto);
  std::vector<std::string> triggersUsedAfterVeto = triggerChainGroupAfterVeto->getListOfTriggers();
  for(std::string trig : triggersUsedAfterVeto){
    ANA_MSG_DEBUG(" \t " << trig);
  }


  //
  // Create the new container and its auxiliary store.
  //
  ANA_MSG_DEBUG("Creating the new container ");
  xAOD::JetContainer*     hltJets    = new xAOD::JetContainer();
  xAOD::JetAuxContainer*  hltJetsAux = new xAOD::JetAuxContainer();
  hltJets->setStore( hltJetsAux ); //< Connect the two

  //
  //  For Adding Tracks to the Jet
  //
  ANA_MSG_DEBUG("Making the decorators ");
  static xAOD::Jet::Decorator<std::vector<const xAOD::TrackParticle*> > m_track_decoration      ("HLTBJetTracks");
  static xAOD::Jet::Decorator<const xAOD::Vertex*>                 m_vtx_decoration        ("HLTBJetTracks_vtx");
  static xAOD::Jet::Decorator<const xAOD::Vertex*>                 m_vtx_decoration_bkg    ("HLTBJetTracks_vtx_bkg");
  static xAOD::Jet::Decorator<char >                               m_vtx_hadDummyPV        ("hadDummyPV");
  static xAOD::Jet::Decorator<const xAOD::Vertex*>                 m_offline_vtx_decoration("offline_vtx");

  static xAOD::Jet::Decorator<float >                              m_bs_online_vz       ("bs_online_vz");
  static xAOD::Jet::Decorator<float >                              m_bs_online_vy       ("bs_online_vy");
  static xAOD::Jet::Decorator<float >                              m_bs_online_vx       ("bs_online_vx");


  //
  // get primary vertex
  //
  ANA_MSG_DEBUG("Getting the PV ");
  const xAOD::VertexContainer *offline_vertices(nullptr);
  const xAOD::Vertex *offline_pvx(nullptr);
  if(HelperFunctions::isAvailable<xAOD::VertexContainer>("PrimaryVertices", m_event, m_store, msg())){
    ANA_CHECK( HelperFunctions::retrieve(offline_vertices, "PrimaryVertices", m_event, m_store, msg()) );
    offline_pvx = HelperFunctions::getPrimaryVertex(offline_vertices, msg());
  }

  //
  // get event info
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );


  //
  //  Make accessors/decorators
  //
  static SG::AuxElement::Decorator< const xAOD::BTagging* > hltBTagDecor( "HLTBTag" );

  Trig::FeatureContainer fc = m_trigDecTool_handle->features(m_trigItemAfterVeto, TrigDefs::Physics );
  Trig::FeatureContainer::combination_const_iterator comb   (fc.getCombinations().begin());
  Trig::FeatureContainer::combination_const_iterator combEnd(fc.getCombinations().end());
  ANA_MSG_DEBUG( m_name << " New Event --------------- ");

  for( ; comb!=combEnd ; ++comb) {
    std::vector< Trig::Feature<xAOD::JetContainer> >            jetCollections  = comb->containerFeature<xAOD::JetContainer>(m_jetName);
    std::vector< Trig::Feature<xAOD::BTaggingContainer> >       bjetCollections = comb->containerFeature<xAOD::BTaggingContainer>("HLTBjetFex");
    std::vector< Trig::Feature<xAOD::TrackParticleContainer> >  trkCollections;
    if(m_readHLTTracks) trkCollections = comb->containerFeature<xAOD::TrackParticleContainer>(m_trkName);
    //std::vector< Trig::Feature<xAOD::TrackParticleContainer> >  ftfCollections  = comb->containerFeature<xAOD::TrackParticleContainer>("InDetTrigTrackingxAODCnv_Bjet_FTF");

    std::vector<Trig::Feature<xAOD::VertexContainer> > vtxCollections;
    std::vector<Trig::Feature<xAOD::VertexContainer> > backupVtxCollections;
    if(m_readHLTVtx){
      backupVtxCollections = comb->containerFeature<xAOD::VertexContainer>("EFHistoPrmVtx");

      if(m_vtxName.size()){
	vtxCollections = comb->containerFeature<xAOD::VertexContainer>(m_vtxName);
      }else{
	vtxCollections = comb->containerFeature<xAOD::VertexContainer>();
      }
    }

    //ANA_MSG_INFO(" Test Size ");
    //ANA_MSG_INFO(" \tSplitJet: " << comb->containerFeature<xAOD::JetContainer>("SplitJet").size());
    //ANA_MSG_INFO(" \tGSCJet: "   << comb->containerFeature<xAOD::JetContainer>("GSCJet")  .size());
    //ANA_MSG_INFO(" \tEFJet:  "   << comb->containerFeature<xAOD::JetContainer>("EFJet")   .size());
    //
    //// FTK Vertex debugging
    //ANA_MSG_INFO(" Test Size " << vtxCollections.size() << "  '" << m_vtxName << "'");
    //ANA_MSG_INFO(" Test Size2 " << vtxCollections.size());
    //ANA_MSG_INFO(" \tempty: " << comb->containerFeature<xAOD::VertexContainer>().size());
    //ANA_MSG_INFO(" \tEFHistoPrmVtx: " << comb->containerFeature<xAOD::VertexContainer>("EFHistoPrmVtx").size();
    //ANA_MSG_INFO(" \txPrimVx: " << comb->containerFeature<xAOD::VertexContainer>("xPrimVx").size());
    //ANA_MSG_INFO(" \tPrimVertexFTK " << comb->containerFeature<xAOD::VertexContainer>("PrimVertexFTK").size());
    //ANA_MSG_INFO(" \tPrimVertexFTKRaw " << comb->containerFeature<xAOD::VertexContainer>("PrimVertexFTKRaw").size());
    //ANA_MSG_INFO(" \tPrimVertexFTKRefit " << comb->containerFeature<xAOD::VertexContainer>("PrimVertexFTKRefit").size();
    //ANA_MSG_INFO(" \tHLT_PrimVertexFTK " << comb->containerFeature<xAOD::VertexContainer>("HLT_PrimVertexFTK").size());
    //
    //    std::vector<Trig::Feature<xAOD::VertexContainer> > EvtxCollections = comb->containerFeature<xAOD::VertexContainer>();
    //    for ( unsigned ifeat=0 ; ifeat<EvtxCollections.size() ; ifeat++ ) {
    //      ANA_MSG_INFO(EvtxCollections.at(ifeat).label());
    //      for( auto vtx_itr : *(EvtxCollections.at(ifeat).cptr()) ){
    //	      ANA_MSG_INFO(vtx_itr->vertexType() << " ");
    //      }
    //    }

    bool isValid = true;

    ANA_MSG_DEBUG("ncontainers  " << bjetCollections.size());

    if(jetCollections.size() != bjetCollections.size()){
      ANA_MSG_ERROR("Problem in container size: " << m_name << " jets: "<< jetCollections.size() << " bjets: "<< bjetCollections.size());
      isValid = false;

      auto triggerChainGroupAfterVeto = m_trigDecTool_handle->getChainGroup(m_trigItemAfterVeto);
      std::vector<std::string> triggersUsedAfterVeto = triggerChainGroupAfterVeto->getListOfTriggers();
      ANA_MSG_DEBUG("Passed Triggers ");
      for(std::string trig : triggersUsedAfterVeto){
        auto trigChain = m_trigDecTool_handle->getChainGroup(trig);
        if(trigChain->isPassed()) ANA_MSG_DEBUG(" \t " << trig);
      }

    }

    if(m_readHLTTracks && jetCollections.size() != trkCollections.size()){
      ANA_MSG_ERROR("Problem in container size: " << m_name << " jets: "<< jetCollections.size() << " trks: "<< trkCollections.size());
      ANA_MSG_ERROR(" Jet Collection " << m_jetName << " Trk Collection:  " << m_trkName);
      isValid = false;
    }

    //if(jetCollections.size() != ftfCollections.size()){
    //  ANA_MSG_ERROR("Problem in container size: " << m_name  << " jets: "<< jetCollections.size() << " ftfs: "<< ftfCollections.size());
    //  isValid = false;
    //}

    if(m_readHLTVtx){
      if(vtxCollections.size() < jetCollections.size()){
	ANA_MSG_ERROR("Problem in container size: " << m_name
                       << " jets: "<< jetCollections.size() << " " << m_jetName
                       << " vtx: "<< vtxCollections.size()  << " " << m_vtxName);
	for ( unsigned ifeat=0 ; ifeat<vtxCollections.size() ; ifeat++ ) {
	  ANA_MSG_INFO("Feture: " << ifeat);
	  for( auto vtx_itr : *(vtxCollections.at(ifeat).cptr()) ){
	    ANA_MSG_INFO(vtx_itr->vertexType());
	  }
	}
	isValid = false;
      }
    }

    if(!isValid) continue;

    //Loop over jets until a jet with track size > 0 is found

    // Declare variables here as same bs for all jets
    float var_bs_online_vx = m_onlineBSTool.getOnlineBSInfo(eventInfo, xAH::OnlineBeamSpotTool::BSData::BSx);
    float var_bs_online_vy = m_onlineBSTool.getOnlineBSInfo(eventInfo, xAH::OnlineBeamSpotTool::BSData::BSy);
    float var_bs_online_vz = m_onlineBSTool.getOnlineBSInfo(eventInfo, xAH::OnlineBeamSpotTool::BSData::BSz);

    ANA_MSG_DEBUG(" bs_online_vx " << var_bs_online_vx << " bs_online_vy " << var_bs_online_vy << " bs_online_vz " << var_bs_online_vz);


    //ANA_MSG_INFO(" is Valid " << jetCollections.size() << " " << vtxCollections.size());
    for ( unsigned ifeat=0 ; ifeat<jetCollections.size() ; ifeat++ ) {
      const xAOD::Jet* hlt_jet = getTrigObject<xAOD::Jet, xAOD::JetContainer>(jetCollections.at(ifeat));
      if(!hlt_jet) continue;

      bool passOverlap = true;
      for( const xAOD::Jet* previousJet : *hltJets){
	if(previousJet->p4().DeltaR(hlt_jet->p4()) < 0.4) passOverlap = false;
      }

      if(!passOverlap) continue;
      ANA_MSG_DEBUG("New Jet: pt: " << hlt_jet->pt() << " eta: " << hlt_jet->eta() << " phi: " << hlt_jet->phi());

      const xAOD::BTagging* hlt_btag = getTrigObject<xAOD::BTagging, xAOD::BTaggingContainer>(bjetCollections.at(ifeat));
      if(!hlt_btag) continue;

      const xAOD::TrackParticleContainer* hlt_tracks(nullptr);
      if(m_readHLTTracks){
	hlt_tracks = trkCollections.at(ifeat).cptr();
	if(!hlt_tracks) continue;
      }

      xAOD::Jet* newHLTBJet = new xAOD::Jet();
      newHLTBJet->makePrivateStore( hlt_jet );

      //
      // Add Link to BTagging Info
      //
      newHLTBJet->auxdecor< const xAOD::BTagging* >("HLTBTag") = hlt_btag;

      //
      // Add Tracks to BJet
      //
      if(m_readHLTTracks){

        std::vector<const xAOD::TrackParticle*> matchedTracks;
	ANA_MSG_DEBUG("Trk Size" << hlt_tracks->size());

	for(const xAOD::TrackParticle* thisHLTTrk: *hlt_tracks){
	  ANA_MSG_DEBUG("\tAdding  track " << thisHLTTrk->pt()   << " " << thisHLTTrk->eta()  << " " << thisHLTTrk->phi());
	  matchedTracks.push_back(thisHLTTrk);
	}

	//
	// Adding online beamspot information from online track
	//

	if(hlt_tracks->size()){
	  ANA_MSG_DEBUG("Found a hlt_tracks   " << hlt_tracks->at(0)->vx() << " " << hlt_tracks->at(0)->vy() << " " << hlt_tracks->at(0)->vz());
	  ANA_MSG_DEBUG("Compares to variable " << " " << var_bs_online_vx << " " << var_bs_online_vy << " " << var_bs_online_vz);
	}

	m_bs_online_vx (*newHLTBJet) = var_bs_online_vx;
	m_bs_online_vy (*newHLTBJet) = var_bs_online_vy;
	m_bs_online_vz (*newHLTBJet) = var_bs_online_vz;

	ANA_MSG_DEBUG("Adding tracks to jet ");
	m_track_decoration(*newHLTBJet)         = matchedTracks;


      }

      ANA_MSG_DEBUG("Doing it for:        " << m_trigItem);
      ANA_MSG_DEBUG("Check for m_jetName: " << m_jetName);
      ANA_MSG_DEBUG("Check for m_vtxName: " << m_vtxName);

      //
      // Check for dummy verticies
      //
      // hadDummyPV => class with three option
      //               0 - IDTrig  Found Vertex
      //               1 - EFHisto Found Vertex
      //               2 - No Vertex found
      if(m_readHLTVtx){
	if(!HelperFunctions::getPrimaryVertex(vtxCollections.at(ifeat).cptr(), msg())){

	  ANA_MSG_DEBUG("HAVE  No Online Vtx!!! m_vtxName is  " << m_vtxName);
	  for( auto vtx_itr : *(vtxCollections.at(ifeat).cptr()) ) ANA_MSG_DEBUG(vtx_itr->vertexType());

	  //
	  //  Try the HistoPrmVtx
	  //
	  if(backupVtxCollections.size()){
	    ANA_MSG_DEBUG("Have EFHistoPrmVtx.  ");
	    m_vtx_hadDummyPV  (*newHLTBJet)         = '1';
	    const xAOD::Vertex *backup_pvx = HelperFunctions::getPrimaryVertex(backupVtxCollections.at(ifeat).cptr(), msg());
	    ANA_MSG_DEBUG("backup_pvx.  " << backup_pvx);
	    m_vtx_decoration  (*newHLTBJet)         = backup_pvx;
	    m_vtx_decoration_bkg(*newHLTBJet)       = backup_pvx;
	  }else{
	    ANA_MSG_INFO("No EFHistoPrmVtx....  ");
	    m_vtx_hadDummyPV  (*newHLTBJet)         = '2';
	    m_vtx_decoration  (*newHLTBJet)         = 0;
	    m_vtx_decoration_bkg(*newHLTBJet)       = 0;
	  }

	  //ANA_MSG_INFO("hadDummy and vtxType " << m_vtx_hadDummyPV (*newHLTBJet) << " " << m_vtxName);

	}else{

	  m_vtx_decoration  (*newHLTBJet)         = HelperFunctions::getPrimaryVertex(vtxCollections.at(ifeat).cptr(), msg());
	  m_vtx_hadDummyPV  (*newHLTBJet)         = '0';

	  if(backupVtxCollections.size()){
	    m_vtx_decoration_bkg(*newHLTBJet)     = HelperFunctions::getPrimaryVertex(backupVtxCollections.at(ifeat).cptr(), msg());
	  }else{
	    m_vtx_decoration_bkg(*newHLTBJet)     = 0;
	  }

	}

	m_offline_vtx_decoration (*newHLTBJet)  = offline_pvx;

	ANA_MSG_DEBUG("hadDummy and vtxType" << m_vtx_hadDummyPV (*newHLTBJet) << " " << m_vtxName);
	//if(m_vtx_hadDummyPV (*newHLTBJet) != '0' ){
	//   ANA_MSG_INFO("hadDummy and vtxType and m_outContainerName  " << m_vtx_hadDummyPV (*newHLTBJet) << " "
	//	<< m_vtxName << ' '<< m_outContainerName);
	//}
      } else{ //m_readHLTVtx

	m_vtx_decoration  (*newHLTBJet)         = 0;
	m_vtx_hadDummyPV  (*newHLTBJet)         = '0';
	m_vtx_decoration_bkg(*newHLTBJet)       = 0;
	m_offline_vtx_decoration (*newHLTBJet)  = 0;
      }

      hltJets->push_back( newHLTBJet );
      ANA_MSG_DEBUG("pushed back ");

    }//feature



  }// Combinations

  ANA_CHECK( m_store->record( hltJets,    m_outContainerName));
  ANA_CHECK( m_store->record( hltJetsAux, m_outContainerName+"Aux."));

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: buildHLTJets ()
{
  ANA_MSG_DEBUG("In buildHLTJets  ");
  //
  // Create the new container and its auxiliary store.
  //
  xAOD::JetContainer*     hltJets    = new xAOD::JetContainer();
  xAOD::JetAuxContainer*  hltJetsAux = new xAOD::JetAuxContainer();
  hltJets->setStore( hltJetsAux ); //< Connect the two

  Trig::FeatureContainer fc = m_trigDecTool_handle->features(m_trigItem);
  auto jetFeatureContainers = fc.containerFeature<xAOD::JetContainer>();

  ANA_MSG_DEBUG("ncontainers  " << jetFeatureContainers.size());

  //DataModel_detail::const_iterator<JetContainer >::reference {aka const xAOD::Jet_v1*}

  for(auto  jcont : jetFeatureContainers) {
    for (const xAOD::Jet*  hlt_jet : *jcont.cptr()) {

      xAOD::Jet* newHLTJet = new xAOD::Jet();
      newHLTJet->makePrivateStore( hlt_jet );

      hltJets->push_back( newHLTJet );
    }
  }

  ANA_CHECK( m_store->record( hltJets,    m_outContainerName));
  ANA_CHECK( m_store->record( hltJetsAux, m_outContainerName+"Aux."));
  ANA_MSG_DEBUG("Left buildHLTJets  ");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: postExecute ()
{
  ANA_MSG_DEBUG( "Calling postExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: finalize ()
{
  ANA_MSG_DEBUG( "Deleting tool instances...");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: histFinalize ()
{
  ANA_MSG_DEBUG( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}
