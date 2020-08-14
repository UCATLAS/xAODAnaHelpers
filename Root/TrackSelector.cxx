#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>

#include "AthContainers/ConstDataVector.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/TrackSelector.h"
#include "InDetTrackSelectionTool/InDetTrackSelectionTool.h"

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

using std::vector;

// this is needed to distribute the algorithm to the workers
ClassImp(TrackSelector)


TrackSelector :: TrackSelector () :
    Algorithm("TrackSelector")
{
}

EL::StatusCode TrackSelector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_DEBUG("Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "TrackSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_DEBUG("Calling histInitialize");
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  ANA_MSG_DEBUG("Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  ANA_MSG_DEBUG("Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  if(m_useCutFlow) {
    TFile *file = wk()->getOutputFile ("cutflow");
    m_cutflowHist  = (TH1D*)file->Get("cutflow");
    m_cutflowHistW = (TH1D*)file->Get("cutflow_weighted");
    m_cutflow_bin  = m_cutflowHist->GetXaxis()->FindBin(m_name.c_str());
    m_cutflowHistW->GetXaxis()->FindBin(m_name.c_str());
  }

  // parse and split by comma
  std::string token;
  std::istringstream ss(m_passAuxDecorKeys);
  while(std::getline(ss, token, ',')){
    m_passKeys.push_back(token);
  }
  ss.clear();
  ss.str(m_failAuxDecorKeys);
  while(std::getline(ss, token, ',')){
    m_failKeys.push_back(token);
  }


  if( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_DEBUG("Number of events in file: " << m_event->getEntries() );

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_numObjectPass = 0;

  ANA_MSG_DEBUG( "Initializing InDetTrackSelectionTool..." );
  if(m_pT_min                           != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minPt",                           m_pT_min));
  if(m_p_min                            != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minP",                            m_p_min));
  if(m_eta_max                          != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxAbsEta",                       m_eta_max));
  if(m_d0_max                           != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxD0",                           m_d0_max));
  if(m_sigmad0_max                      != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxSigmaD0",                      m_sigmad0_max));
  if(m_d0oversigmad0_max                != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxD0overSigmaD0",                m_d0oversigmad0_max));
  if(m_z0_max                           != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxZ0",                           m_z0_max));
  if(m_z0sinT_max                       != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxZ0SinTheta",                   m_z0sinT_max));
  if(m_sigmaz0_max                      != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxSigmaZ0",                      m_sigmaz0_max));
  if(m_sigmaz0sintheta_max              != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxSigmaZ0SinTheta",              m_sigmaz0sintheta_max));
  if(m_z0oversigmaz0_max                != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxZ0overSigmaZ0",                m_z0oversigmaz0_max));
  if(m_z0sinthetaoversigmaz0sintheta_max!= 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxZ0SinThetaoverSigmaZ0SinTheta",m_z0sinthetaoversigmaz0sintheta_max));
  if(m_nInnermostPixel_min              != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNInnermostLayerHits",          m_nInnermostPixel_min));
  if(m_nNextToInnermostPixel_min        != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNNextToInnermostLayerHits",    m_nNextToInnermostPixel_min));
  if(m_nBothInnermostLayersHits_min     != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNBothInnermostLayersHits",     m_nBothInnermostLayersHits_min));
  if(m_nSi_min                          != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNSiHits",                      m_nSi_min));
  if(m_nSiPhysical_min                  != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNSiHitsPhysical",              m_nSiPhysical_min));
  if(m_nSiSharedHits_max                != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxNSiSharedHits",                m_nSiSharedHits_max));
  if(m_nSiHoles_max                     != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxNSiHoles",                     m_nSiHoles_max));
  if(m_nPixelHits_min                   != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNPixelHits",                   m_nPixelHits_min));
  if(m_nPixelHitsPhysical_min           != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNPixelHitsPhysical",           m_nPixelHitsPhysical_min));
  if(m_nPixelSharedHits_max             != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxNPixelSharedHits",             m_nPixelSharedHits_max));
  if(m_nPixHoles_max                    != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxNPixelHoles",                  m_nPixHoles_max));
  if(m_nSctHits_min                     != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNSctHits",                     m_nSctHits_min));
  if(m_nSctHitsPhysical_min             != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minNSctHitsPhysical",             m_nSctHitsPhysical_min));
  if(m_nSctSharedHits_max               != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxNSctSharedHits",               m_nSctSharedHits_max));
  if(m_nSctHoles_max                    != 2e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxNSctHoles",                    m_nSctHoles_max));
  if(m_nSiSharedModules_max             != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxNSiSharedModules",             m_nSiSharedModules_max));
  if(m_chi2Prob_min                     != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("minProb",                         m_chi2Prob_min));
  if(m_chi2NdofCut_max                  != 1e8)ANA_CHECK(m_trkSelTool_handle.setProperty("maxChiSqperNdf",                  m_chi2NdofCut_max));
  if(m_cutLevelString                   != "" )ANA_CHECK(m_trkSelTool_handle.setProperty("CutLevel",                        m_cutLevelString));

  ANA_CHECK( m_trkSelTool_handle.retrieve() );

  ANA_MSG_DEBUG( "TrackSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: execute ()
{

  ANA_MSG_DEBUG("Applying Track Selection... " << m_name);

  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // MC event weight
  //
  float mcEvtWeight(1);
  if ( eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) )
	mcEvtWeight = eventInfo->mcEventWeight();

  if(m_doTracksInJets){
    return executeTracksInJets();
  } else{
    return executeTrackCollection(mcEvtWeight);
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackSelector :: executeTrackCollection (float mcEvtWeight)
{
  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::TrackParticleContainer* inTracks(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(inTracks, m_inContainerName, m_event, m_store, msg()) );

  // get primary vertex
  const xAOD::VertexContainer *vertices(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(vertices, m_vertexContainerName, m_event, m_store, msg()) );
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices, msg());


  // create output container (if requested) - deep copy

  ConstDataVector<xAOD::TrackParticleContainer>* selectedTracks = 0;
  if(m_createSelectedContainer) {
    selectedTracks = new ConstDataVector<xAOD::TrackParticleContainer>(SG::VIEW_ELEMENTS);
  }

  xAOD::TrackParticleContainer::const_iterator trk_itr = inTracks->begin();
  xAOD::TrackParticleContainer::const_iterator trk_end = inTracks->end();
  int nPass(0); int nObj(0);
  for( ; trk_itr != trk_end; ++trk_itr ){

    // if only looking at a subset of tracks make sure all are decorrated
    if( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if(m_decorateSelectedObjects) {
        (*trk_itr)->auxdecor< char >( "passSel" ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    int passSel = this->PassCuts( (*trk_itr), pvx );
    if(m_decorateSelectedObjects) {
      (*trk_itr)->auxdecor< char >( "passSel" ) = passSel;
    }

    if(passSel) {
      nPass++;
      if(m_createSelectedContainer) {
        selectedTracks->push_back( *trk_itr );
      }
    }
  }

  m_numObject     += nObj;
  m_numObjectPass += nPass;

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  if( m_pass_min > 0 && nPass < m_pass_min ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }
  if( m_pass_max >= 0 && nPass > m_pass_max ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  // add output container to TStore
  if( m_createSelectedContainer ) {
    ANA_CHECK( m_store->record( selectedTracks, m_outContainerName ));
  }

  m_numEventPass++;
  if(m_useCutFlow) {
    m_cutflowHist ->Fill( m_cutflow_bin, 1 );
    m_cutflowHistW->Fill( m_cutflow_bin, mcEvtWeight);
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode TrackSelector :: executeTracksInJets ()
{
  ANA_MSG_DEBUG("Applying TracksInJet Selection... " << m_inJetContainerName);
  m_numEvent++;

  // get input jet collection
  const xAOD::JetContainer* inJets(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(inJets, m_inJetContainerName, m_event, m_store, msg()) );

  //// get primary vertex
  //const xAOD::VertexContainer *vertices(nullptr);
  //ANA_CHECK( HelperFunctions::retrieve(vertices, m_vertexContainerName, m_event, m_store, msg()) );
  //const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices, msg());

  int nPass(0); int nObj(0);

  //
  //  Accessor for adding the output jets
  //
  xAOD::Jet::Decorator<vector<const xAOD::TrackParticle*> > m_track_decoration(m_outContainerName.c_str());
  xAOD::Jet::Decorator<const xAOD::Vertex*>                 m_vtx_decoration  ((m_outContainerName+"_vtx").c_str());

  //
  // loop on Jets
  //
  for ( auto jet_itr : *inJets ) {

    //
    //  output container with in the jet
    //
    vector<const xAOD::TrackParticle*> outputTracks;

    //
    // loop on tracks with in jet
    //
    const vector<const xAOD::TrackParticle*> inputTracks = jet_itr->auxdata< vector<const xAOD::TrackParticle*>  >(m_inContainerName);
    const xAOD::Vertex* pvx                              = jet_itr->auxdata< const xAOD::Vertex*                 >(m_inContainerName+"_vtx");
    for(const xAOD::TrackParticle* trkInJet: inputTracks){

      nObj++;

      //
      // Get cut desicion
      //
      int passSel = this->PassCuts( trkInJet, pvx );

      //
      // if
      //
      if(passSel) {
	nPass++;
	outputTracks.push_back(trkInJet);
      }
    }// tracks

    m_numObject     += nObj;
    m_numObjectPass += nPass;

    m_track_decoration(*jet_itr)  = outputTracks;
    m_vtx_decoration(*jet_itr)    = jet_itr->auxdata<const xAOD::Vertex*>(m_inContainerName+"_vtx");

  }//jets

  m_numEventPass++;
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: finalize ()
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

  ANA_MSG_DEBUG("Deleting tool instances...");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: histFinalize ()
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
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}

int TrackSelector :: PassCuts( const xAOD::TrackParticle* trk, const xAOD::Vertex *pvx ) {

  // InDetTrackSelectionTool
  if ( !m_trkSelTool_handle->accept(*trk, pvx) ) { return 0; }

  // Cuts not available with the InDetTrackSelectionTool
  //
  // pT_max
  //
  if( m_pT_max != 1e8 ) {
    if ( trk->pt() > m_pT_max ) { return 0; }
  }

  //
  // eta 
  //
  if( m_eta_min != 1e8 ) {
    if( fabs(trk->eta()) < m_eta_min ) { return 0; }
  }
  if( m_etaSigned_max != 1e8 ) {
    if( trk->eta() > m_etaSigned_max ) { return 0; }
  }
  if( m_etaSigned_min != 1e8 ) {
    if( trk->eta() < m_etaSigned_min ) { return 0; }
  }


 
  //  
  //  nBLayer
  //
  uint8_t nBL       = -1;
  if( m_nBL_min != 1e8 ){
    //  xAOD::numberOfBLayerHits is deprecated, keeping it for compatibility 
    if(!trk->summaryValue(nBL, xAOD::numberOfBLayerHits)) ANA_MSG_ERROR( "BLayer hits not filled");
    if( nBL < m_nBL_min ) {return 0; }
  }

  //
  //  chi2Prob_max
  //
  float        chi2        = trk->chiSquared();
  float        ndof        = trk->numberDoF();
  if( m_chi2Prob_max != 1e8 ){
    float        chi2Prob    = TMath::Prob(chi2,ndof);
    if( chi2Prob > m_chi2Prob_max) { return 0; }
  }

  //
  //  Pass Keys
  //
  for(auto& passKey : m_passKeys){
    if(!(trk->auxdata< char >(passKey) == '1')) { return 0; }
  }

  //
  //  Fail Keys
  //
  for(auto& failKey : m_failKeys){
    if(!(trk->auxdata< char >(failKey) == '0')) { return 0; }
  }


  return 1;
}


