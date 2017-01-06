
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
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

using std::cout;  using std::endl;
using std::vector;

// this is needed to distribute the algorithm to the workers
ClassImp(HLTJetRoIBuilder)

HLTJetRoIBuilder :: HLTJetRoIBuilder (std::string className) :
  Algorithm(className),
  m_trigItem(""),
  m_trigItemVeto(""),
  m_doHLTBJet(true),
  m_doHLTJet (false),
  m_readHLTTracks(true),
  m_outContainerName(""),
  m_trigDecTool(nullptr),
  m_jetName("EFJet"),
  m_trkName("InDetTrigTrackingxAODCnv_Bjet_IDTrig"),
  m_vtxName("EFHistoPrmVtx")
{
  if(m_debug) Info("HLTJetRoIBuilder()", "Calling constructor");

  // read debug flag from .config file
  m_debug                   = false;

}


EL::StatusCode HLTJetRoIBuilder :: setupJob (EL::Job& job)
{
  if(m_debug) Info("setupJob()", "Calling setupJob");
  job.useXAOD ();
  xAOD::Init( "HLTJetRoIBuilder" ).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: histInitialize ()
{
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");
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

  if(m_debug) Info("initialize()", "Initializing HLTJetRoIBuilder Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //
  // Grab the TrigDecTool from the ToolStore
  //
  m_trigDecTool = dynamic_cast<Trig::TrigDecisionTool*>(asg::ToolStore::get("TrigDecisionTool"));

  if(m_trigItem.find("split") != std::string::npos){
    m_jetName = "SplitJet";
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

  cout << "HLTJetRoIBuilder::Configured " << m_name << " with " << endl;
  cout << "m_trigItem: " << m_trigItem << endl;
  cout << "m_trigItemVeto: " << m_trigItemVeto << endl;
  cout << "m_trkName: " << m_trkName << endl;
  cout << "m_vtxName: " << m_vtxName << endl;
  cout << "m_jetName: " << m_jetName << endl;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode HLTJetRoIBuilder :: execute ()
{
  if ( m_debug ) { Info("execute()", "Doing HLT JEt ROI Building... "); }

  if(m_doHLTBJet){
    return buildHLTBJets();
  }else if(m_doHLTJet){
    return buildHLTJets();
  }



  if ( m_debug ) { m_store->print(); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: buildHLTBJets ()
{
  auto triggerChainGroup = m_trigDecTool->getChainGroup(m_trigItem);
  
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

  if(m_debug){
    cout << m_name << " " << m_trigItem << " matches" << endl;
    cout << m_trigItemAfterVeto << endl;
    auto triggerChainGroupAfterVeto = m_trigDecTool->getChainGroup(m_trigItemAfterVeto);
    std::vector<std::string> triggersUsedAfterVeto = triggerChainGroupAfterVeto->getListOfTriggers();
    for(std::string trig : triggersUsedAfterVeto){
      cout << " \t " << trig << endl;
    }
  }


  //
  // Create the new container and its auxiliary store.
  //
  xAOD::JetContainer*     hltJets    = new xAOD::JetContainer();
  xAOD::JetAuxContainer*  hltJetsAux = new xAOD::JetAuxContainer();
  hltJets->setStore( hltJetsAux ); //< Connect the two

  //
  //  For Adding Tracks to the Jet
  //
  static xAOD::Jet::Decorator<vector<const xAOD::TrackParticle*> > m_track_decoration      ("HLTBJetTracks");
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
  const xAOD::VertexContainer *offline_vertices(nullptr);
  RETURN_CHECK("TrackHistsAlgo::execute()", HelperFunctions::retrieve(offline_vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  const xAOD::Vertex *offline_pvx = HelperFunctions::getPrimaryVertex(offline_vertices);


  //
  //  Make accessors/decorators
  //
  static SG::AuxElement::Decorator< const xAOD::BTagging* > hltBTagDecor( "HLTBTag" );

  Trig::FeatureContainer fc = m_trigDecTool->features(m_trigItemAfterVeto, TrigDefs::Physics );
  Trig::FeatureContainer::combination_const_iterator comb   (fc.getCombinations().begin());
  Trig::FeatureContainer::combination_const_iterator combEnd(fc.getCombinations().end());
  if(m_debug) cout << m_name << " New Event --------------- " << endl;
    
  for( ; comb!=combEnd ; ++comb) {
    std::vector< Trig::Feature<xAOD::JetContainer> >            jetCollections  = comb->containerFeature<xAOD::JetContainer>(m_jetName);
    std::vector< Trig::Feature<xAOD::BTaggingContainer> >       bjetCollections = comb->containerFeature<xAOD::BTaggingContainer>("HLTBjetFex");
    std::vector< Trig::Feature<xAOD::TrackParticleContainer> >  trkCollections;
    if(m_readHLTTracks) trkCollections = comb->containerFeature<xAOD::TrackParticleContainer>(m_trkName);
    //std::vector< Trig::Feature<xAOD::TrackParticleContainer> >  ftfCollections  = comb->containerFeature<xAOD::TrackParticleContainer>("InDetTrigTrackingxAODCnv_Bjet_FTF");
    std::vector<Trig::Feature<xAOD::VertexContainer> > vtxCollections;
    std::vector<Trig::Feature<xAOD::VertexContainer> > backupVtxCollections = comb->containerFeature<xAOD::VertexContainer>("EFHistoPrmVtx");

    if(m_vtxName.size()){
      vtxCollections = comb->containerFeature<xAOD::VertexContainer>(m_vtxName);
    }else{
      vtxCollections = comb->containerFeature<xAOD::VertexContainer>();
    }

    // FTK Vertex debugging
    //    cout << " Test Size " << endl;
    //    cout << " \tempty: " << comb->containerFeature<xAOD::VertexContainer>().size() << endl;
    //    cout << " \tEFHistoPrmVtx: " << comb->containerFeature<xAOD::VertexContainer>("EFHistoPrmVtx").size() <<endl;
    //    cout << " \txPrimVx: " << comb->containerFeature<xAOD::VertexContainer>("xPrimVx").size() << endl;
    //    cout << " \tPrimVertexFTK " << comb->containerFeature<xAOD::VertexContainer>("PrimVertexFTK").size() << endl;
    //    cout << " \tPrimVertexFTKRaw " << comb->containerFeature<xAOD::VertexContainer>("PrimVertexFTKRaw").size() << endl;
    //    cout << " \tPrimVertexFTKRefit " << comb->containerFeature<xAOD::VertexContainer>("PrimVertexFTKRefit").size()<< endl;
    //    cout << " \tHLT_PrimVertexFTK " << comb->containerFeature<xAOD::VertexContainer>("HLT_PrimVertexFTK").size() << endl;
    //
    //    std::vector<Trig::Feature<xAOD::VertexContainer> > EvtxCollections = comb->containerFeature<xAOD::VertexContainer>();
    //    for ( unsigned ifeat=0 ; ifeat<EvtxCollections.size() ; ifeat++ ) {
    //      cout << EvtxCollections.at(ifeat).label() << endl;
    //      for( auto vtx_itr : *(EvtxCollections.at(ifeat).cptr()) ){
    //	cout << vtx_itr->vertexType() << " " << endl;
    //      }
    //    }

    bool isValid = true;

    if(m_debug) cout << "ncontainers  " << bjetCollections.size() << endl;

    if(jetCollections.size() != bjetCollections.size()){
      cout << "ERROR Problem in container size: " << m_name << " jets: "<< jetCollections.size() << " bjets: "<< bjetCollections.size() << endl;
      isValid = false;

      if(m_debug){
	auto triggerChainGroupAfterVeto = m_trigDecTool->getChainGroup(m_trigItemAfterVeto);
	std::vector<std::string> triggersUsedAfterVeto = triggerChainGroupAfterVeto->getListOfTriggers();
	cout << "Passed Triggers " << endl;
	for(std::string trig : triggersUsedAfterVeto){
	  auto trigChain = m_trigDecTool->getChainGroup(trig);
	  if(trigChain->isPassed())
	    cout << " \t " << trig << endl;
	}
      }


    }

    if(m_readHLTTracks && jetCollections.size() != trkCollections.size()){
      cout << "ERROR Problem in container size: " << m_name << " jets: "<< jetCollections.size() << " trks: "<< trkCollections.size() << endl;
      cout << " Jet Collection " << m_jetName << " Trk Collection:  " << m_trkName << endl;
      isValid = false;
    }

    //if(jetCollections.size() != ftfCollections.size()){
    //  cout << "ERROR Problem in container size: " << m_name  << " jets: "<< jetCollections.size() << " ftfs: "<< ftfCollections.size() << endl;
    //  isValid = false;
    //}

    if(vtxCollections.size() < jetCollections.size()){
      cout << "ERROR Problem in container size: " << m_name  
	   << " jets: "<< jetCollections.size() << " " << m_jetName 
	   << " vtx: "<< vtxCollections.size()  << " " << m_vtxName << endl;
      for ( unsigned ifeat=0 ; ifeat<vtxCollections.size() ; ifeat++ ) {
	cout << "Feture: " << ifeat << endl;
	for( auto vtx_itr : *(vtxCollections.at(ifeat).cptr()) ){
	  cout << vtx_itr->vertexType() <<endl;
	}
      }
      isValid = false;
    }

    if(!isValid) continue;

    //Loop over jets until a jet with track size > 0 is found
    
    // Declare variables here as same bs for all jets
    float var_bs_online_vx=999;
    float var_bs_online_vy=999;
    float var_bs_online_vz=999;  

    if(m_readHLTTracks){
      for ( unsigned ifeat=0 ; ifeat<jetCollections.size() ; ifeat++ ) {
	
	const xAOD::TrackParticleContainer* hlt_tracks(nullptr);

	hlt_tracks = trkCollections.at(ifeat).cptr();
	if(!hlt_tracks) continue;
      
	if(hlt_tracks->size()){
	  var_bs_online_vx=hlt_tracks->at(0)->vx();
	  var_bs_online_vy=hlt_tracks->at(0)->vy();
	  var_bs_online_vz=hlt_tracks->at(0)->vz();
	  break;
	}	
      }
    }
    
    if(m_debug){
      cout << " bs_online_vx " << var_bs_online_vx
	   << " bs_online_vy " << var_bs_online_vy
	   << " bs_online_vz " << var_bs_online_vz << endl;
    }

    
    //cout << " is Valid " << jetCollections.size() << " " << vtxCollections.size() << endl;
    for ( unsigned ifeat=0 ; ifeat<jetCollections.size() ; ifeat++ ) {
      const xAOD::Jet* hlt_jet = getTrigObject<xAOD::Jet, xAOD::JetContainer>(jetCollections.at(ifeat));
      if(!hlt_jet) continue;

      bool passOverlap = true;
      for( const xAOD::Jet* previousJet : *hltJets){
	if(previousJet->p4().DeltaR(hlt_jet->p4()) < 0.4) passOverlap = false;
      }

      if(!passOverlap) continue;
      if(m_debug) cout << "New Jet: pt: " << hlt_jet->pt() << " eta: " << hlt_jet->eta() << " phi: " << hlt_jet->phi() << endl;

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

	vector<const xAOD::TrackParticle*> matchedTracks;
	if(m_debug)cout << "Trk Size" << hlt_tracks->size() << endl;
      
	for(const xAOD::TrackParticle* thisHLTTrk: *hlt_tracks){
	  if(m_debug) cout <<  "\tAdding  track "
			   << thisHLTTrk->pt()   << " "
			   << thisHLTTrk->eta()  << " "
			   << thisHLTTrk->phi()  << endl;
	  matchedTracks.push_back(thisHLTTrk);
	}

	//
	// Adding online beamspot information from online track
	//

	if(hlt_tracks->size()){
	  if(m_debug) {
	    cout << "Found a hlt_tracks   "
		 << hlt_tracks->at(0)->vx() << " "
		 << hlt_tracks->at(0)->vy() << " "
		 << hlt_tracks->at(0)->vz() << endl;
	    cout << "Compares to variable " << " "
		 << var_bs_online_vx << " "
		 << var_bs_online_vy << " "
		 << var_bs_online_vz << endl;
	  }
	}

	m_bs_online_vx (*newHLTBJet) = var_bs_online_vx;
	m_bs_online_vy (*newHLTBJet) = var_bs_online_vy;
	m_bs_online_vz (*newHLTBJet) = var_bs_online_vz;
      
	if(m_debug) cout <<  "Adding tracks to jet " << endl;
	m_track_decoration(*newHLTBJet)         = matchedTracks;


      }

      if(m_debug){
	cout << "Doing it for:        " << m_trigItem << endl;
	cout << "Check for m_jetName: " << m_jetName << endl;
	cout << "Check for m_vtxName: " << m_vtxName << endl;
      }

      //
      // Check for dummy verticies
      //
      // hadDummyPV => class with three option
      //               0 - IDTrig  Found Vertex
      //               1 - EFHisto Found Vertex
      //               2 - No Vertex found
      
      if(!HelperFunctions::getPrimaryVertex(vtxCollections.at(ifeat).cptr(), true)){

	if(m_debug){
	  cout << "HAVE  No Online Vtx!!! m_vtxName is  " << m_vtxName << endl;
	  for( auto vtx_itr : *(vtxCollections.at(ifeat).cptr()) ){
	    cout << vtx_itr->vertexType() <<endl;
	  }
	}

	//
	//  Try the HistoPrmVtx
	//
	if(backupVtxCollections.size()){
	  if(m_debug) cout << "Have EFHistoPrmVtx.  " << endl;
	  m_vtx_hadDummyPV  (*newHLTBJet)         = '1';
	  const xAOD::Vertex *backup_pvx = HelperFunctions::getPrimaryVertex(backupVtxCollections.at(ifeat).cptr());
	  if(m_debug) cout << "backup_pvx.  " << backup_pvx << endl;
	  m_vtx_decoration  (*newHLTBJet)         = backup_pvx;	    
	  m_vtx_decoration_bkg(*newHLTBJet)       = backup_pvx;	    
	}else{
	  cout << "No EFHistoPrmVtx....  " << endl;
	  m_vtx_hadDummyPV  (*newHLTBJet)         = '2';
	  m_vtx_decoration  (*newHLTBJet)         = 0;	    
	  m_vtx_decoration_bkg(*newHLTBJet)       = 0;
	}

	//cout << "hadDummy and vtxType " << m_vtx_hadDummyPV (*newHLTBJet) << " " << m_vtxName << endl;

      }else{

	m_vtx_decoration  (*newHLTBJet)         = HelperFunctions::getPrimaryVertex(vtxCollections.at(ifeat).cptr());
	m_vtx_hadDummyPV  (*newHLTBJet)         = '0';

	if(backupVtxCollections.size()){
	  m_vtx_decoration_bkg(*newHLTBJet)     = HelperFunctions::getPrimaryVertex(backupVtxCollections.at(ifeat).cptr());
	}else{
	  m_vtx_decoration_bkg(*newHLTBJet)     = 0;
	}

      }

      m_offline_vtx_decoration (*newHLTBJet)  = offline_pvx;

      if(m_debug) cout << "hadDummy and vtxType" << m_vtx_hadDummyPV (*newHLTBJet) << " " << m_vtxName << endl;

      //if(m_vtx_hadDummyPV (*newHLTBJet) != '0' ){
      //   cout << "hadDummy and vtxType and m_outContainerName  " << m_vtx_hadDummyPV (*newHLTBJet) << " "
      //	<< m_vtxName << ' '<< m_outContainerName <<endl;
      //}
      
      hltJets->push_back( newHLTBJet );
      if(m_debug) cout << "pushed back " << endl;

    }//feature



  }// Combinations

  RETURN_CHECK("PlotHLTBJetFex::selected()", m_store->record( hltJets,    m_outContainerName),     "Failed to record selected dijets");
  RETURN_CHECK("PlotHLTBJetFex::selected()", m_store->record( hltJetsAux, m_outContainerName+"Aux."), "Failed to record selected dijetsAux.");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: buildHLTJets ()
{
  if(m_debug) cout << "In buildHLTJets  " <<endl;
  //
  // Create the new container and its auxiliary store.
  //
  xAOD::JetContainer*     hltJets    = new xAOD::JetContainer();
  xAOD::JetAuxContainer*  hltJetsAux = new xAOD::JetAuxContainer();
  hltJets->setStore( hltJetsAux ); //< Connect the two

  Trig::FeatureContainer fc = m_trigDecTool->features(m_trigItem);
  auto jetFeatureContainers = fc.containerFeature<xAOD::JetContainer>();

  if(m_debug) cout << "ncontainers  " << jetFeatureContainers.size() << endl;

  //DataModel_detail::const_iterator<JetContainer >::reference {aka const xAOD::Jet_v1*}

  for(auto  jcont : jetFeatureContainers) {
    for (const xAOD::Jet*  hlt_jet : *jcont.cptr()) {

      xAOD::Jet* newHLTJet = new xAOD::Jet();
      newHLTJet->makePrivateStore( hlt_jet );

      hltJets->push_back( newHLTJet );
    }
  }

  RETURN_CHECK("PlotHLTBJetFex::selected()", m_store->record( hltJets,    m_outContainerName),     "Failed to record selected dijets");
  RETURN_CHECK("PlotHLTBJetFex::selected()", m_store->record( hltJetsAux, m_outContainerName+"Aux."), "Failed to record selected dijetsAux.");
  if(m_debug) cout << "Left buildHLTJets  " <<endl;
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: postExecute ()
{
  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: finalize ()
{
  if(m_debug) Info("finalize()", "Deleting tool instances...");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode HLTJetRoIBuilder :: histFinalize ()
{
  if(m_debug) Info("histFinalize()", "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");
  return EL::StatusCode::SUCCESS;
}
