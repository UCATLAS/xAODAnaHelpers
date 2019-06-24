#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

#include <AthContainers/ConstDataVector.h>

#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/VertexContainer.h>

#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODAnaHelpers/SecondaryVertexSelector.h>
#include "TVector3.h"

// needed to distribute algorithm to workers
ClassImp ( SecondaryVertexSelector )



SecondaryVertexSelector :: SecondaryVertexSelector () 
  : Algorithm ( "SecondaryVertexSelector" )
  , m_bonsaiCfg( VsiBonsai::Configurator{} )
  , prop_chi2Cut            ( 5.0 )
  , prop_hitPatternReq      ( VsiTool::HitPatternCondition::NONE )
  , prop_doDropAssociated   ( false )
  , prop_fillTracks         ( false )
  , prop_trackStorePrescale ( 1.e-3 )
  , prop_d0_wrtSVCut        ( 0.8 /*AlgConsts::invalidFloat*/ )
  , prop_z0_wrtSVCut        ( 1.2 /*AlgConsts::invalidFloat*/ )
  , prop_errd0_wrtSVCut     ( AlgConsts::invalidFloat )
  , prop_errz0_wrtSVCut     ( AlgConsts::invalidFloat )
  , prop_d0signif_wrtSVCut  ( 5.0 /* AlgConsts::invalidFloat*/ )
  , prop_z0signif_wrtSVCut  ( 5.0 /*AlgConsts::invalidFloat*/ )
  , prop_chi2_toSVCut       ( 5.0 /*AlgConsts::invalidFloat*/ )
  , m_mapInFile              ( "" )
  , m_mapOutFile              ( "" )
  , m_mapInName              ( "FinalMap_inner" )
  , m_mapOutName              ( "matmap_outer" )
  , m_matrixInName              ( "FoldingInfo" )
{
  // code for base initialization of variables, e.g. initialize
  // all pointers to zero; note this method called on submission +
  // worker node, so only put most basic initialization here;
  // most initialization will go in histInitialize() and initialize()

  m_materialMap_v3_Outer = nullptr;
  m_materialMap_v32_Inner = nullptr;
  m_materialMap_v32_Matrix = nullptr;
  
}



EL::StatusCode SecondaryVertexSelector :: setupJob ( EL::Job& job )
{
  // code to set up job on submission object so it's ready to work
  // w/ alg, e.g. request D3PDReader service or add output files;
  // code here automatically activated/deactivated when alg
  // added/removed from job
  
  // initialize alg to use xAODRootAccess package
  job.useXAOD();
  xAOD::Init( "SecondaryVertexSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: histInitialize ()
{
  // everything that needs to be done at the beginning on each
  // worker node, e.g. create histos + output trees;
  // this method called before any input files connected
  
  ANA_CHECK( xAH::Algorithm::algInitialize() );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: fileExecute ()
{
  // everything that needs to be done exactly once for every
  // single file, e.g. collect list of all lumi-blocks processed

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: changeInput ( bool /*firstFile*/ )
{
  // everything that needs to be done when input files change,
  // e.g. resetting branch addresses on trees; not needed if
  // using D3PDReader or similar service...

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: initialize ()
{
  // everything that needs to be done after first input file
  // connected and before first event processed, e.g. create
  // additional histograms based on variables available in input files;
  // can also create all histograms + trees here, but note this method
  // doesn't get called if no events processed, so objects created
  // here won't be available in output if no input events

  ANA_MSG_INFO( "Initializing SecondaryVertexSelector..." );

  VsiBonsai::setDefaultConfig( m_bonsaiCfg );
  m_bonsaiCfg[ VsiBonsai::Config::trackChi2Cut ]        = static_cast<double>( prop_chi2Cut );
  m_bonsaiCfg[ VsiBonsai::Config::hitPatternCondition ] = static_cast<VsiTool::HitPatternCondition>( prop_hitPatternReq );
  m_bonsaiCfg[ VsiBonsai::Config::dropAssociated ]      = static_cast<bool>( prop_doDropAssociated );
  m_bonsaiCfg[ VsiBonsai::Config::d0_wrtSVCut ]         = static_cast<double>( prop_d0_wrtSVCut );
  m_bonsaiCfg[ VsiBonsai::Config::z0_wrtSVCut ]         = static_cast<double>( prop_z0_wrtSVCut );
  m_bonsaiCfg[ VsiBonsai::Config::errd0_wrtSVCut ]      = static_cast<double>( prop_errd0_wrtSVCut );
  m_bonsaiCfg[ VsiBonsai::Config::errz0_wrtSVCut ]      = static_cast<double>( prop_errz0_wrtSVCut );
  m_bonsaiCfg[ VsiBonsai::Config::d0signif_wrtSVCut ]   = static_cast<double>( prop_d0signif_wrtSVCut );
  m_bonsaiCfg[ VsiBonsai::Config::z0signif_wrtSVCut ]   = static_cast<double>( prop_z0signif_wrtSVCut );
  m_bonsaiCfg[ VsiBonsai::Config::chi2_toSVCut ]        = static_cast<double>( prop_chi2_toSVCut );


  if(m_do_matMapVeto) {
   
    TFile* mapIn = new TFile(m_mapInFile.c_str(), "READ");
    TFile* mapOut = new TFile(m_mapOutFile.c_str(), "READ");

    if( mapIn->IsOpen() && mapOut->IsOpen() ) {
      std::cout << "files open!" << std::endl;
      mapOut ->GetObject(m_mapOutName.c_str(), m_materialMap_v3_Outer);
      mapIn->GetObject(m_mapInName.c_str(), m_materialMap_v32_Inner);
      mapIn ->GetObject(m_matrixInName.c_str(), m_materialMap_v32_Matrix);

      

      if(m_materialMap_v3_Outer) {
        std::cout << "Outer map open!" << std::endl;
        m_materialMap_v3_Outer->SetDirectory(0);
      }
      if(m_materialMap_v32_Inner) {
        std::cout << "Inner map open!" << std::endl;
        m_materialMap_v32_Inner->SetDirectory(0);
      }
    }
     
    mapIn->Close();
    mapOut->Close();

    delete mapIn;
    delete mapOut;

  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // cutflow

  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "No input container provided! Exiting." );
    return EL::StatusCode::FAILURE;
  }

  ANA_MSG_DEBUG( "SecondaryVertexSelector Interface successfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: execute ()
{
  // everything that needs to be done on every single event,
  // e.g. read input variables, apply cuts, fill histos + trees;
  // here is where most of actual analysis will go...

  ANA_MSG_DEBUG( "Applying Secondary Vertex Selection..." );

  // count number of events

  // get primary vertex

  const xAOD::VertexContainer* vertices = 0;
  ANA_CHECK( HelperFunctions::retrieve( vertices, m_vertexContainerName, m_event, m_store, msg() ) );
  const xAOD::Vertex* primaryVertex = HelperFunctions::getPrimaryVertex( vertices, msg() ); 

  // get secondary vertex collection from TEvent or TStore
  const xAOD::VertexContainer* inSecVerts = 0;
  ANA_CHECK( HelperFunctions::retrieve( inSecVerts, m_inContainerName, m_event, m_store, msg() ) );

  // create output container (if requested) -- deep copy
  auto selectedSecVerts =
    std::make_unique<ConstDataVector<xAOD::VertexContainer>>(SG::VIEW_ELEMENTS);
  
  for ( const auto& vtx : *inSecVerts ) {
    int passSel = this->PassCuts( vtx, primaryVertex );
    if ( m_decorateSelectedObjects ) vtx->auxdecor<char>( "passSel" ) = passSel;

    if ( !passSel ) continue;
    
    // copy selected vertex to ouput container (if requested)
    if ( m_createSelectedContainer ) selectedSecVerts->push_back( vtx );
  }

  // apply event selected based on min/max requirements on number of objects passing cuts per event

  // add output container to TStore
  if ( m_createSelectedContainer )
    ANA_CHECK( m_store->record( selectedSecVerts.release(), m_outContainerName ) );

  // cutflow

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: postExecute ()
{
  // everything that need to be done after main event processing;
  // mainly used in implementing NTupleSvc

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: finalize ()
{
  // mirror image of initialize(); gets called after last event
  // processed on worker node; allows finishing up objects created
  // in initialize() before they're written to disk; only gets
  // called on worker nodes that processed input events

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode SecondaryVertexSelector :: histFinalize ()
{
  // mirror image of histInitialize(); gets called after last event
  // processed on worker node; allows finishing up objects created in
  // histInitialize() before they're written to disk; gets called on
  // all worker nodes regardless of whether they processed input events

  ANA_CHECK( xAH::Algorithm::algFinalize() );

  return EL::StatusCode::SUCCESS;
}



int SecondaryVertexSelector :: PassCuts ( const xAOD::Vertex* vtx, const xAOD::Vertex* primaryVtx  )
{
  // here's where we do the cuts --> if vertex fails cuts, return 0

  std::vector<VsiBonsai::Trimmer> trimmers {
    VsiBonsai::chi2Filter,
    VsiBonsai::ipWrtSVFilter,
    VsiBonsai::hitPatternFilter
  };

  // filter vertex tracks using VsiBonsai config
  std::vector<const xAOD::TrackParticle*> filteredTracks;
  std::vector<const xAOD::TrackParticle*> droppedTracks;


  std::vector< ElementLink< xAOD::TrackParticleContainer > > trackLinks = vtx->trackParticleLinks();

  for(const auto& trkLink: trackLinks){
    if(!trkLink.isValid()) break; // error out somehow
    const xAOD::TrackParticle* trk = *trkLink;

    bool flag = true;
 
    if(m_do_trackTrimming) {

      for( auto trimmer : trimmers ) {       
        if( !trimmer( vtx, trk, primaryVtx, m_bonsaiCfg ) ) {
          flag = false;
          break;
        }
      }
     
    }

    if( flag ) {    
      filteredTracks.emplace_back( trk );
    } else {
      droppedTracks.emplace_back( trk );
    }
 
    trk->auxdecor<char>( "isFiltered" ) = flag;

  }

  if ( filteredTracks.size() < 2 ) return 0;





  // material map veto... --> have config option, "do_matMapVeto" ...
  if(m_do_matMapVeto) {

    bool passMaterialVeto_v3 = false;

    const TVector3 dv_pos( vtx->x(), vtx->y(), vtx->z() );

    if (dv_pos.Perp() > 150){

      passMaterialVeto_v3 = ( m_materialMap_v3_Outer->GetBinContent(m_materialMap_v3_Outer->FindBin( dv_pos.Perp() ,
				  								   dv_pos.Phi() ,
					  							   dv_pos.z() ) ) == 0);
    }
    else {
      for ( Int_t i=0;i<5;i++){
        if (dv_pos.Perp() < (*m_materialMap_v32_Matrix)[i][0]){
	        float test_x = dv_pos.x() + (*m_materialMap_v32_Matrix)[i][1];
	        float test_y = dv_pos.y() + (*m_materialMap_v32_Matrix)[i][2];
	        double calc_phi = fmod( TMath::ATan2(test_y,test_x),TMath::Pi()/(*m_materialMap_v32_Matrix)[i][3] );
	        if (calc_phi <0) calc_phi = calc_phi + TMath::Pi()/(*m_materialMap_v32_Matrix)[i][3];
	    
	        passMaterialVeto_v3 = ( m_materialMap_v32_Inner->GetBinContent(m_materialMap_v32_Inner->FindBin( sqrt(test_x*test_x + test_y*test_y ) ,  calc_phi ,  dv_pos.z() ) ) == 0);
	        break;
	      }
      }
    } 

    if(!passMaterialVeto_v3)  {
      return 0;
    }

  }

  // other vertex cuts...
  
  return 1;
}
