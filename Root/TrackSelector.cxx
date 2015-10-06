#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>

#include "AthContainers/ConstDataVector.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/TrackSelector.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// this is needed to distribute the algorithm to the workers
ClassImp(TrackSelector)


TrackSelector :: TrackSelector () :
  m_cutflowHist(nullptr),
  m_cutflowHistW(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("TrackSelector()", "Calling constructor");

  // read debug flag from .config file
  m_debug         = false;
  m_useCutFlow    = true;

  // input container to be read from TEvent or TStore
  m_inContainerName  = "";

  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::VIEW_ELEMENTS option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainer = false;
  // if requested, a new container is made using the SG::VIEW_ELEMENTS option
  m_outContainerName        = "";
  // if only want to look at a subset of object
  m_nToProcess              = -1;

  // cuts
  m_pass_max                = -1;
  m_pass_min                = -1;
  m_pT_max                  = 1e8;
  m_pT_min                  = 1e8;
  m_eta_max                 = 1e8;
  m_eta_min                 = 1e8;
  m_d0_max                  = 1e8;
  m_z0_max                  = 1e8;
  m_z0sinT_max              = 1e8;
  m_nBL_min                 = 1e8;
  m_nSi_min                 = 1e8;
  m_nPixHoles_max           = 1e8;
  m_chi2NdofCut_max         = 1e8;
  m_chi2Prob_max            = 1e8;

  m_passAuxDecorKeys        = "";

  m_failAuxDecorKeys        = "";

}

EL::StatusCode  TrackSelector :: configure ()
{
  if(!getConfig().empty()){
    Info("configure()", "Configuing TrackSelector Interface. User configuration read from : %s ", getConfig().c_str());

    TEnv* config = new TEnv(getConfig(true).c_str());

    // read debug flag from .config file
    m_debug         = config->GetValue("Debug" ,      m_debug);
    m_useCutFlow    = config->GetValue("UseCutFlow",  m_useCutFlow);

    // input container to be read from TEvent or TStore
    m_inContainerName  = config->GetValue("InputContainer",  m_inContainerName.c_str());

    // decorate selected objects that pass the cuts
    m_decorateSelectedObjects = config->GetValue("DecorateSelectedObjects", m_decorateSelectedObjects);
    // additional functionality : create output container of selected objects
    //                            using the SG::VIEW_ELEMENTS option
    //                            decorating and output container should not be mutually exclusive
    m_createSelectedContainer = config->GetValue("CreateSelectedContainer", m_createSelectedContainer);
    // if requested, a new container is made using the SG::VIEW_ELEMENTS option
    m_outContainerName        = config->GetValue("OutputContainer", m_outContainerName.c_str());
    // if only want to look at a subset of object
    m_nToProcess              = config->GetValue("NToProcess", m_nToProcess);

    // cuts
    m_pass_max                = config->GetValue("PassMax",      m_pass_max);
    m_pass_min                = config->GetValue("PassMin",      m_pass_min);
    m_pT_max                  = config->GetValue("pTMax",        m_pT_max);
    m_pT_min                  = config->GetValue("pTMin",        m_pT_min);
    m_eta_max                 = config->GetValue("etaMax",       m_eta_max);
    m_eta_min                 = config->GetValue("etaMin",       m_eta_min);
    m_d0_max                  = config->GetValue("d0Max",        m_d0_max);
    m_z0_max                  = config->GetValue("z0Max",        m_z0_max);
    m_z0sinT_max              = config->GetValue("z0SinTMax",    m_z0sinT_max);
    m_nBL_min                 = config->GetValue("nBLMin",       m_nBL_min);
    m_nSi_min                 = config->GetValue("nSiMin",       m_nSi_min);
    m_nPixHoles_max           = config->GetValue("nPixHolesMax", m_nPixHoles_max);
    m_chi2NdofCut_max         = config->GetValue("chi2NdofMax",  m_chi2NdofCut_max);
    m_chi2Prob_max            = config->GetValue("chi2ProbMax",  m_chi2Prob_max);

    m_passAuxDecorKeys        = config->GetValue("PassDecorKeys", m_passAuxDecorKeys.c_str());

    m_failAuxDecorKeys        = config->GetValue("FailDecorKeys", m_failAuxDecorKeys.c_str());

    config->Print();
    Info("configure()", "TrackSelector Interface succesfully configured! ");

    delete config;
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
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }



  return EL::StatusCode::SUCCESS;
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

  Info("setupJob()", "Calling setupJob");

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

  Info("histInitialize()", "Calling histInitialize");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput");

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

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_numObjectPass = 0;

  Info("initialize()", "TrackSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TrackSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Track Selection... ");

  float mcEvtWeight(1); // FIXME - set to something from eventInfo

  m_numEvent++;

  // get the collection from TEvent or TStore
  const xAOD::TrackParticleContainer* inTracks(nullptr);
  RETURN_CHECK("TrackSelector::execute()", HelperFunctions::retrieve(inTracks, m_inContainerName, m_event, m_store, m_verbose) ,"");

  // get primary vertex
  const xAOD::VertexContainer *vertices(nullptr);
  RETURN_CHECK("TrackSelector::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);


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
  if( m_pass_max > 0 && nPass > m_pass_max ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  // add output container to TStore
  if( m_createSelectedContainer ) {
    RETURN_CHECK( "TrackSelector::execute()", m_store->record( selectedTracks, m_outContainerName ), "Failed to store container.");
  }

  m_numEventPass++;
  if(m_useCutFlow) {
    m_cutflowHist ->Fill( m_cutflow_bin, 1 );
    m_cutflowHistW->Fill( m_cutflow_bin, mcEvtWeight);
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode TrackSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute");

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

  Info("finalize()", "Deleting tool instances...");

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
  return EL::StatusCode::SUCCESS;
}

int TrackSelector :: PassCuts( const xAOD::TrackParticle* trk, const xAOD::Vertex *pvx ) {


  // pT
  if( m_pT_max != 1e8 ) {
    if( trk->pt() > m_pT_max ) { return 0; }
  }
  if( m_pT_min != 1e8 ) {
    if( trk->pt() < m_pT_min ) { return 0; }
  }

  // eta
  if( m_eta_max != 1e8 ) {
    if( trk->eta() > m_eta_max ) { return 0; }
  }
  if( m_eta_min != 1e8 ) {
    if( trk->eta() < m_eta_min ) { return 0; }
  }

  //
  //  D0
  //
  if( m_d0_max != 1e8 ){
    if( fabs(trk->d0()) > m_d0_max ) {return 0; }
  }

  //
  //  Z0
  //
  float z0 = (trk->z0() + trk->vz() - pvx->z());
  if( m_z0_max != 1e8 ){
    if( fabs(z0) > m_z0_max ) {return 0; }
  }

  //
  //  z0 sin(theta)
  //
  float sinT        = sin(trk->theta());
  if( m_z0sinT_max != 1e8 ){
    if( fabs(z0*sinT) > m_z0sinT_max ) {return 0; }
  }

  //
  //  nBLayer
  //
  uint8_t nBL       = -1;
  if( m_nBL_min != 1e8 ){
    if(!trk->summaryValue(nBL,       xAOD::numberOfBLayerHits))      Error("PassCuts()", "BLayer hits not filled");
    if( nBL < m_nBL_min ) {return 0; }
  }

  //
  //  nSi_min
  //
  uint8_t nSCT      = -1;
  uint8_t nPix      = -1;
  if( m_nSi_min != 1e8 ){
    if(!trk->summaryValue(nPix,      xAOD::numberOfPixelHits))        Error("PassCuts()", "Pix hits not filled");
    if(!trk->summaryValue(nSCT,      xAOD::numberOfSCTHits))          Error("PassCuts()", "SCT hits not filled");
    if( (nSCT+nPix) < m_nSi_min ) {return 0;}
  }

  //
  //  nPix Holes
  //
  uint8_t nPixHoles = -1;
  if( m_nPixHoles_max != 1e8 ){
    if(!trk->summaryValue(nPixHoles, xAOD::numberOfPixelHoles))       Error("PassCuts()", "Pix holes not filled");
    if( nPixHoles > m_nPixHoles_max ) {return 0;}
  }

  //
  //  chi2
  //
  float        chi2        = trk->chiSquared();
  float        ndof        = trk->numberDoF();
  if( m_chi2NdofCut_max != 1e8){
    float chi2NDoF     = (ndof > 0) ? chi2/ndof : -1;
    if( chi2NDoF > m_chi2NdofCut_max ) {return 0;}
  }

  if( m_chi2Prob_max != 1e8 ){
    float        chi2Prob    = TMath::Prob(chi2,ndof);
    if( chi2Prob > m_chi2Prob_max) {return 0;}
  }


  //
  //  Pass Keys
  //
  for(auto& passKey : m_passKeys){
    if(!(trk->auxdata< char >(passKey) == '1')) { return 0;}
  }

  //
  //  Fail Keys
  //
  for(auto& failKey : m_failKeys){
    if(!(trk->auxdata< char >(failKey) == '0')) {return 0;}
  }


  return 1;
}


