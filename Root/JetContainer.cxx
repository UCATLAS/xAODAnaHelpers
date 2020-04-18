#include "xAODAnaHelpers/JetContainer.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <iostream>
#include <exception> // std::domain_error
#include "xAODTruth/TruthEventContainer.h"

using namespace xAH;

JetContainer::JetContainer(const std::string& name, const std::string& detailStr, float units, bool mc)
  : ParticleContainer(name,detailStr,units,mc),
    m_trkSelTool(nullptr)

{
  // rapidity
  if(m_infoSwitch.m_rapidity) {
    m_rapidity                  =new std::vector<float>();
  }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched          = new     std::vector<int>               ();
    m_isTrigMatchedToChain   = new     std::vector<std::vector<int> > ();
    m_listTrigChains         = new     std::vector<std::string>       ();
  }
  
  // clean
  if(m_infoSwitch.m_cleanTrig && ! (m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight)) {
    std::cout << "JetContainer              WARNING You asked for cleanTrig for " << name << "but didn't specify clean or cleanLight. Going to assume you wanted clean." << std::endl;
    m_infoSwitch.m_clean = true;
  }
  if(m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight) {
    if(m_infoSwitch.m_clean){
      m_Timing                    =new std::vector<float>();
      m_LArQuality                =new std::vector<float>();
      m_HECQuality                =new std::vector<float>();
      m_NegativeE                 =new std::vector<float>();
      m_AverageLArQF              =new std::vector<float>();
      m_BchCorrCell               =new std::vector<float>();
      m_N90Constituents           =new std::vector<float>();
      m_LArBadHVEnergyFrac        =new std::vector<float>();
      m_LArBadHVNCell             =new std::vector<int>();
      m_OotFracClusters5          =new std::vector<float>();
      m_OotFracClusters10         =new std::vector<float>();
      m_LeadingClusterPt          =new std::vector<float>();
      m_LeadingClusterSecondLambda=new std::vector<float>();
      m_LeadingClusterCenterLambda=new std::vector<float>();
      m_LeadingClusterSecondR     =new std::vector<float>();
      if(m_infoSwitch.m_cleanTrig) {
        m_clean_passLooseBadTriggerUgly=new std::vector<int>();
      }
      else {
        m_clean_passLooseBadUgly    =new std::vector<int>();
        m_clean_passTightBadUgly    =new std::vector<int>();
      }
    }
    if(m_infoSwitch.m_cleanTrig) {
      m_clean_passLooseBadTrigger =new std::vector<int>();
    }
    else {
      m_clean_passLooseBad        =new std::vector<int>();
      m_clean_passTightBad        =new std::vector<int>();
    }
  }

  // energy
  if ( m_infoSwitch.m_energy || m_infoSwitch.m_energyLight ) {
    if ( m_infoSwitch.m_energy )  {
      m_HECFrac               = new std::vector<float>();
      m_CentroidR             = new std::vector<float>();
      m_LowEtConstituentsFrac = new std::vector<float>();
    }
    m_EMFrac                = new std::vector<float>();
    m_FracSamplingMax       = new std::vector<float>();
    m_FracSamplingMaxIndex  = new std::vector<float>();
    m_GhostMuonSegmentCount = new std::vector<float>();
    m_Width                 = new std::vector<float>();
  }

  // scales
  if ( m_infoSwitch.m_scales ) {
    m_emScalePt            = new std::vector<float>();
    m_constScalePt	   = new std::vector<float>();
    m_pileupScalePt	   = new std::vector<float>();
    m_originConstitScalePt = new std::vector<float>();
    m_etaJESScalePt	   = new std::vector<float>();
    m_gscScalePt	   = new std::vector<float>();
    m_jmsScalePt           = new std::vector<float>();
    m_insituScalePt	   = new std::vector<float>();

    m_emScaleM             = new std::vector<float>();
    m_constScaleM          = new std::vector<float>();
    m_pileupScaleM         = new std::vector<float>();
    m_originConstitScaleM  = new std::vector<float>();
    m_etaJESScaleM         = new std::vector<float>();
    m_gscScaleM            = new std::vector<float>();
    m_jmsScaleM            = new std::vector<float>();
    m_insituScaleM         = new std::vector<float>();
  }

  // constscale eta
  if ( m_infoSwitch.m_constscaleEta ) {
    m_constScaleEta            = new std::vector<float>();
  }

  // detector eta
  if ( m_infoSwitch.m_detectorEta ) {
    m_detectorEta              = new std::vector<float>();
  }

  // layer
  if ( m_infoSwitch.m_layer ) {
    m_EnergyPerSampling        = new std::vector< std::vector<float> >();
  }

  // tracksAll
  if ( m_infoSwitch.m_trackAll ) {
    m_NumTrkPt1000             = new std::vector< std::vector<int> >    ();
    m_SumPtTrkPt1000           = new std::vector< std::vector<float> >  ();
    m_TrackWidthPt1000         = new std::vector< std::vector<float> >  ();
    m_NumTrkPt500              = new std::vector< std::vector<int> >    ();
    m_SumPtTrkPt500            = new std::vector< std::vector<float> >  ();
    m_TrackWidthPt500          = new std::vector< std::vector<float> >  ();
    m_JVF                      = new std::vector< std::vector<float> >  ();
  }

  // trackPV
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_jvt ) {
    if (m_infoSwitch.m_trackPV){
      m_NumTrkPt1000PV       = new std::vector<float>();
      m_SumPtTrkPt1000PV     = new std::vector<float>();
      m_TrackWidthPt1000PV   = new std::vector<float>();
      m_NumTrkPt500PV        = new std::vector<float>();
      m_SumPtTrkPt500PV      = new std::vector<float>();
      m_TrackWidthPt500PV    = new std::vector<float>();
      m_JVFPV                = new std::vector<float>();
      m_JvtJvfcorr           = new std::vector<float>();
      m_JvtRpt               = new std::vector<float>();
    }
    m_Jvt                = new std::vector<float>();
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Loose" ) {
    m_JvtPass_Loose    = new std::vector<int>();
    if ( m_mc ) {
      m_JvtEff_SF_Loose  = new std::vector< std::vector<float> > ();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Medium" ) {
    m_JvtPass_Medium   = new std::vector<int>();
    if ( m_mc ) {
      m_JvtEff_SF_Medium = new std::vector< std::vector<float> > ();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Tight" ) {
    m_JvtPass_Tight    = new std::vector<int>();
    if ( m_mc ) {
      m_JvtEff_SF_Tight  = new std::vector< std::vector<float> > ();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Medium" ) {
    m_fJvtPass_Medium   = new std::vector<int>();
    if ( m_mc ) {
      m_fJvtEff_SF_Medium = new std::vector< std::vector<float> > ();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Tight" ) {
    m_fJvtPass_Tight    = new std::vector<int>();
    if ( m_mc ) {
      m_fJvtEff_SF_Tight  = new std::vector< std::vector<float> > ();
    }
  }


  // allTrack
  // trackAll or trackPV
  if ( m_infoSwitch.m_allTrack ) {
    m_GhostTrackCount     = new std::vector<int>                  ();
    m_GhostTrackPt        = new std::vector<float>                ();
    m_GhostTrack_pt       = new std::vector< std::vector<float> > ();
    m_GhostTrack_qOverP   = new std::vector< std::vector<float> > ();
    m_GhostTrack_eta      = new std::vector< std::vector<float> > ();
    m_GhostTrack_phi      = new std::vector< std::vector<float> > ();
    m_GhostTrack_e        = new std::vector< std::vector<float> > ();
    m_GhostTrack_d0       = new std::vector< std::vector<float> > ();
    m_GhostTrack_z0       = new std::vector< std::vector<float> > ();


    // allTrackDetail
    if(m_infoSwitch.m_allTrackDetail){
      m_GhostTrack_nPixelHits                               = new std::vector< std::vector<int> >();
      m_GhostTrack_nSCTHits                                 = new std::vector< std::vector<int> >();
      m_GhostTrack_nTRTHits                                 = new std::vector< std::vector<int> >();
      m_GhostTrack_nPixelSharedHits                         = new std::vector< std::vector<int> >();
      m_GhostTrack_nPixelSplitHits                          = new std::vector< std::vector<int> >();
      m_GhostTrack_nInnermostPixelLayerHits                 = new std::vector< std::vector<int> >();
      m_GhostTrack_nInnermostPixelLayerSharedHits           = new std::vector< std::vector<int> >();
      m_GhostTrack_nInnermostPixelLayerSplitHits            = new std::vector< std::vector<int> >();
      m_GhostTrack_nNextToInnermostPixelLayerHits           = new std::vector< std::vector<int> >();
      m_GhostTrack_nNextToInnermostPixelLayerSharedHits     = new std::vector< std::vector<int> >();
      m_GhostTrack_nNextToInnermostPixelLayerSplitHits      = new std::vector< std::vector<int> >();
    }
  }

  // constituent
  if ( m_infoSwitch.m_constituent ) {
    m_numConstituents        = new  std::vector< int >                ();
  }

  if ( m_infoSwitch.m_constituentAll ) {
    m_constituentWeights     = new  std::vector< std::vector<float> > ();
    m_constituent_pt         = new  std::vector< std::vector<float> > ();
    m_constituent_eta        = new  std::vector< std::vector<float> > ();
    m_constituent_phi        = new  std::vector< std::vector<float> > ();
    m_constituent_e          = new  std::vector< std::vector<float> > ();
  }

  // flavorTag
  if( m_infoSwitch.m_flavorTag  || m_infoSwitch.m_flavorTagHLT  ) {

    //m_MV1                               =new std::vector<float>();
    m_MV2c00                            =new std::vector<float>();
    m_MV2c10                            =new std::vector<float>();
    m_MV2c10mu                          =new std::vector<float>();
    m_MV2c10rnn                         =new std::vector<float>();
    m_MV2rmu                            =new std::vector<float>();
    m_MV2r                              =new std::vector<float>();
    m_MV2c20                            =new std::vector<float>();
    m_MV2c100                           =new std::vector<float>();
    m_DL1                               =new std::vector<float>();
    m_DL1_pu                            =new std::vector<float>();
    m_DL1_pc                            =new std::vector<float>();
    m_DL1_pb                            =new std::vector<float>();
    m_DL1mu                             =new std::vector<float>();
    m_DL1mu_pu                          =new std::vector<float>();
    m_DL1mu_pc                          =new std::vector<float>();
    m_DL1mu_pb                          =new std::vector<float>();
    m_DL1rnn                            =new std::vector<float>();
    m_DL1rnn_pu                         =new std::vector<float>();
    m_DL1rnn_pc                         =new std::vector<float>();
    m_DL1rnn_pb                         =new std::vector<float>();
    m_DL1rmu                            =new std::vector<float>();
    m_DL1rmu_pu                         =new std::vector<float>();
    m_DL1rmu_pc                         =new std::vector<float>();
    m_DL1rmu_pb                         =new std::vector<float>();
    m_DL1r                              =new std::vector<float>();
    m_DL1r_pu                           =new std::vector<float>();
    m_DL1r_pc                           =new std::vector<float>();
    m_DL1r_pb                           =new std::vector<float>();
    m_HadronConeExclTruthLabelID        =new std::vector<int>();
    m_HadronConeExclExtendedTruthLabelID=new std::vector<int>();

    // Jet Fitter
    if( m_infoSwitch.m_jetFitterDetails){
      m_JetFitter_nVTX             = new std::vector<float>();
      m_JetFitter_nSingleTracks    = new std::vector<float>();
      m_JetFitter_nTracksAtVtx     = new std::vector<float>();
      m_JetFitter_mass             = new std::vector<float>();
      m_JetFitter_energyFraction   = new std::vector<float>();
      m_JetFitter_significance3d   = new std::vector<float>();
      m_JetFitter_deltaeta         = new std::vector<float>();
      m_JetFitter_deltaphi         = new std::vector<float>();
      m_JetFitter_N2Tpar           = new std::vector<float>();
    }

    // SV Details
    if( m_infoSwitch.m_svDetails){

      m_SV0               = new      std::vector<float>();


      m_sv0_NGTinSvx      = new     std::vector<float>();
      m_sv0_N2Tpair       = new     std::vector<float>();
      m_sv0_massvx        = new     std::vector<float>();
      m_sv0_efracsvx      = new     std::vector<float>();
      m_sv0_normdist      = new     std::vector<float>();

      m_SV1               = new     std::vector<float>();
      m_SV1IP3D           = new     std::vector<float>();
      m_COMBx             = new     std::vector<float>();
      m_sv1_pu            = new     std::vector<float>();
      m_sv1_pb            = new     std::vector<float>();
      m_sv1_pc            = new     std::vector<float>();
      m_sv1_c             = new     std::vector<float>();
      m_sv1_cu            = new     std::vector<float>();
      m_sv1_NGTinSvx      = new     std::vector<float>();
      m_sv1_N2Tpair       = new     std::vector<float>();
      m_sv1_massvx        = new     std::vector<float>();
      m_sv1_efracsvx      = new     std::vector<float>();
      m_sv1_normdist      = new     std::vector<float>();
      m_sv1_Lxy           = new     std::vector<float>();
      m_sv1_sig3d         = new     std::vector<float>();
      m_sv1_L3d           = new     std::vector<float>();
      m_sv1_distmatlay    = new     std::vector<float>();
      m_sv1_dR            = new     std::vector<float>();
    }

    // IP3D
    if( m_infoSwitch.m_ipDetails){
      m_IP2D_pu        = new std::vector<float>();
      m_IP2D_pb        = new std::vector<float>();
      m_IP2D_pc        = new std::vector<float>();
      m_IP2D           = new std::vector<float>();
      m_IP2D_c         = new std::vector<float>();
      m_IP2D_cu        = new std::vector<float>();
      m_nIP2DTracks    = new std::vector<float>();

      m_IP2D_gradeOfTracks              = new std::vector<std::vector<float> >();
      m_IP2D_flagFromV0ofTracks         = new std::vector<std::vector<float> >();
      m_IP2D_valD0wrtPVofTracks         = new std::vector<std::vector<float> >();
      m_IP2D_sigD0wrtPVofTracks         = new std::vector<std::vector<float> >();
      m_IP2D_weightBofTracks            = new std::vector<std::vector<float> >();
      m_IP2D_weightCofTracks            = new std::vector<std::vector<float> >();
      m_IP2D_weightUofTracks            = new std::vector<std::vector<float> >();

      m_IP3D         = new std::vector<float>();
      m_IP3D_pu      = new std::vector<float>();
      m_IP3D_pb      = new std::vector<float>();
      m_IP3D_pc      = new std::vector<float>();
      m_IP3D_c       = new std::vector<float>();
      m_IP3D_cu      = new std::vector<float>();
      m_nIP3DTracks  = new std::vector<float>();
      m_IP3D_gradeOfTracks        = new  std::vector<std::vector<float> >();
      m_IP3D_flagFromV0ofTracks   = new  std::vector<std::vector<float> >();
      m_IP3D_valD0wrtPVofTracks   = new  std::vector<std::vector<float> >();
      m_IP3D_sigD0wrtPVofTracks   = new  std::vector<std::vector<float> >();
      m_IP3D_valZ0wrtPVofTracks   = new  std::vector<std::vector<float> >();
      m_IP3D_sigZ0wrtPVofTracks   = new  std::vector<std::vector<float> >();
      m_IP3D_weightBofTracks      = new  std::vector<std::vector<float> >();
      m_IP3D_weightCofTracks      = new  std::vector<std::vector<float> >();
      m_IP3D_weightUofTracks      = new  std::vector<std::vector<float> >();
    }

    if( m_infoSwitch.m_JVC ) {
      m_JetVertexCharge_discriminant = new std::vector<double>();
    }

  }

  //  flavorTagHLT
  if( m_infoSwitch.m_flavorTagHLT  ) {
    m_vtxOnlineValid     = new  std::vector<float>();
    m_vtxHadDummy        = new  std::vector<float>();

    m_bs_online_vx       = new  std::vector<float>();
    m_bs_online_vy       = new  std::vector<float>();
    m_bs_online_vz       = new  std::vector<float>();

    m_vtx_offline_x0     = new  std::vector<float>();
    m_vtx_offline_y0     = new  std::vector<float>();
    m_vtx_offline_z0     = new  std::vector<float>();

    m_vtx_online_x0      = new  std::vector<float>();
    m_vtx_online_y0      = new  std::vector<float>();
    m_vtx_online_z0      = new  std::vector<float>();

    m_vtx_online_bkg_x0  = new  std::vector<float>();
    m_vtx_online_bkg_y0  = new  std::vector<float>();
    m_vtx_online_bkg_z0  = new  std::vector<float>();
  }

  if( !m_infoSwitch.m_jetBTag.empty() ) {
    std::stringstream ss_wpName;
    for(const auto& btaginfo : m_infoSwitch.m_jetBTag)
      {
	for(auto wp : btaginfo.second)
	  {
	    ss_wpName.str("");
	    ss_wpName << wp.first << "_" << std::setfill('0') << std::setw(2) << wp.second;
	    m_btags.push_back(new btagOpPoint(m_mc, btaginfo.first, ss_wpName.str()));
	  }
      }
  }

  if ( !m_infoSwitch.m_jetBTagCts.empty() ){
    for(const auto& tagger: m_infoSwitch.m_jetBTagCts)
    {
      m_btags.push_back(new btagOpPoint(m_mc,tagger,"Continuous"));
    }
  }

  // area
  if( m_infoSwitch.m_area ) {
    m_GhostArea          = new std::vector<float>();
    m_ActiveArea         = new std::vector<float>();
    m_VoronoiArea        = new std::vector<float>();
    m_ActiveArea4vec_pt  = new std::vector<float>();
    m_ActiveArea4vec_eta = new std::vector<float>();
    m_ActiveArea4vec_phi = new std::vector<float>();
    m_ActiveArea4vec_m   = new std::vector<float>();
  }


  // truth
  if ( m_infoSwitch.m_truth && m_mc ) {
    m_ConeTruthLabelID  =new std::vector<int>();
    m_TruthCount        =new std::vector<int>();
    m_TruthLabelDeltaR_B=new std::vector<float>;
    m_TruthLabelDeltaR_C=new std::vector<float>;
    m_TruthLabelDeltaR_T=new std::vector<float>;
    m_PartonTruthLabelID=new std::vector<int>();
    m_GhostTruthAssociationFraction=new std::vector<float>;
    m_truth_E  =new std::vector<float>;
    m_truth_pt =new std::vector<float>;
    m_truth_phi=new std::vector<float>;
    m_truth_eta=new std::vector<float>;
  }

  // truth detail
  if ( m_infoSwitch.m_truthDetails ) {
    m_GhostBHadronsFinalCount       = new       std::vector<int>   ();
    m_GhostBHadronsInitialCount     = new       std::vector<int>   ();
    m_GhostBQuarksFinalCount        = new       std::vector<int>   ();
    m_GhostBHadronsFinalPt          = new       std::vector<float> ();
    m_GhostBHadronsInitialPt        = new       std::vector<float> ();
    m_GhostBQuarksFinalPt           = new       std::vector<float> ();

    m_GhostCHadronsFinalCount       = new       std::vector<int>   ();
    m_GhostCHadronsInitialCount     = new       std::vector<int>   ();
    m_GhostCQuarksFinalCount        = new       std::vector<int>   ();
    m_GhostCHadronsFinalPt          = new       std::vector<float> ();
    m_GhostCHadronsInitialPt        = new       std::vector<float> ();
    m_GhostCQuarksFinalPt           = new       std::vector<float> ();

    m_GhostTausFinalCount           = new       std::vector<int>   ();
    m_GhostTausFinalPt              = new       std::vector<float> ();

    m_truth_pdgId                   = new       std::vector<int>   ();
    m_truth_partonPt                = new       std::vector<float> ();
    m_truth_partonDR                = new      std::vector<float>  ();
  }

  // charge
  if ( m_infoSwitch.m_charge ) {
    m_charge   =new std::vector<double>();
  }

  // passSel
  if ( m_infoSwitch.m_passSel ) {
    m_passSel  =new std::vector<char>();
  }

}

JetContainer::~JetContainer()
{
  if(m_debug) std::cout << " Deleting JetContainer "  << std::endl;
  if(m_infoSwitch.m_rapidity) {
    delete m_rapidity;
  }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    delete m_isTrigMatched         ;
    delete m_isTrigMatchedToChain  ;
    delete m_listTrigChains        ;
  }

  // clean
  if(m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight) {
    if(m_infoSwitch.m_clean){
      delete m_Timing;
      delete m_LArQuality;
      delete m_HECQuality;
      delete m_NegativeE;
      delete m_AverageLArQF;
      delete m_BchCorrCell;
      delete m_N90Constituents;
      delete m_LArBadHVEnergyFrac;
      delete m_LArBadHVNCell;
      delete m_OotFracClusters5;
      delete m_OotFracClusters10;
      delete m_LeadingClusterPt;
      delete m_LeadingClusterSecondLambda;
      delete m_LeadingClusterCenterLambda;
      delete m_LeadingClusterSecondR;
      if(m_infoSwitch.m_cleanTrig) {
        delete m_clean_passLooseBadTriggerUgly;
      }
      else {
        delete m_clean_passLooseBadUgly;
        delete m_clean_passTightBadUgly;
      }
    }
    if(m_infoSwitch.m_cleanTrig) {
      delete m_clean_passLooseBadTrigger;
    }
    else {
      delete m_clean_passLooseBad;
      delete m_clean_passTightBad;
    }
  }

  // energy
  if ( m_infoSwitch.m_energy || m_infoSwitch.m_energyLight ) {
    if ( m_infoSwitch.m_energy ){
      delete m_HECFrac;
      delete m_CentroidR;
      delete m_LowEtConstituentsFrac;
    }
    delete m_EMFrac;
    delete m_FracSamplingMax;
    delete m_FracSamplingMaxIndex;
    delete m_GhostMuonSegmentCount;
    delete m_Width;
  }

  // scales
  if ( m_infoSwitch.m_scales ) {
    delete m_emScalePt             ;
    delete m_constScalePt	   ;
    delete m_pileupScalePt		   ;
    delete m_originConstitScalePt  ;
    delete m_etaJESScalePt	   ;
    delete m_gscScalePt		   ;
    delete m_jmsScalePt            ;
    delete m_insituScalePt	   ;

    delete m_emScaleM             ;
    delete m_constScaleM          ;
    delete m_pileupScaleM                 ;
    delete m_originConstitScaleM  ;
    delete m_etaJESScaleM         ;
    delete m_gscScaleM            ;
    delete m_jmsScaleM            ;
    delete m_insituScaleM         ;
  }

  // constscale eta
  if ( m_infoSwitch.m_constscaleEta ) {
    delete m_constScaleEta         ;
  }

  // detector eta
  if ( m_infoSwitch.m_detectorEta ) {
    delete m_detectorEta;
  }

  // layer
  if ( m_infoSwitch.m_layer ) {
    delete m_EnergyPerSampling;
  }

  // tracksAll
  if ( m_infoSwitch.m_trackAll ) {
    delete m_NumTrkPt1000     ;
    delete m_SumPtTrkPt1000   ;
    delete m_TrackWidthPt1000 ;
    delete m_NumTrkPt500      ;
    delete m_SumPtTrkPt500    ;
    delete m_TrackWidthPt500  ;
    delete m_JVF              ;
  }


  // trackPV
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_jvt ) {
    if ( m_infoSwitch.m_trackPV ){
      delete m_NumTrkPt1000PV;
      delete m_SumPtTrkPt1000PV;
      delete m_TrackWidthPt1000PV;
      delete m_NumTrkPt500PV;
      delete m_SumPtTrkPt500PV;
      delete m_TrackWidthPt500PV;
      delete m_JVFPV;
      delete m_JvtJvfcorr;
      delete m_JvtRpt;
    }
    delete m_Jvt;
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Loose" ) {
    delete m_JvtPass_Loose;
      if ( m_mc ) {
          delete m_JvtEff_SF_Loose;
      }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Medium" ) {
    delete m_JvtPass_Medium;
    if ( m_mc ) {
      delete m_JvtEff_SF_Medium;
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Tight" ) {
    delete m_JvtPass_Tight;
    if ( m_mc ) {
      delete m_JvtEff_SF_Tight;
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Medium" ) {
    delete m_fJvtPass_Medium;
    if ( m_mc ) {
      delete m_fJvtEff_SF_Medium;
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Tight" ) {
    delete m_fJvtPass_Tight;
    if ( m_mc ) {
      delete m_fJvtEff_SF_Tight;
    }
  }

  // allTrack
  // trackAll or trackPV
  if ( m_infoSwitch.m_allTrack ) {
    delete m_GhostTrackCount    ;
    delete m_GhostTrackPt       ;
    delete m_GhostTrack_pt      ;
    delete m_GhostTrack_qOverP  ;
    delete m_GhostTrack_eta     ;
    delete m_GhostTrack_phi     ;
    delete m_GhostTrack_e       ;
    delete m_GhostTrack_d0      ;
    delete m_GhostTrack_z0      ;


    // allTrackDetail
    if(m_infoSwitch.m_allTrackDetail){
      delete m_GhostTrack_nPixelHits                               ;
      delete m_GhostTrack_nSCTHits                                 ;
      delete m_GhostTrack_nTRTHits                                 ;
      delete m_GhostTrack_nPixelSharedHits                         ;
      delete m_GhostTrack_nPixelSplitHits                          ;
      delete m_GhostTrack_nInnermostPixelLayerHits                 ;
      delete m_GhostTrack_nInnermostPixelLayerSharedHits           ;
      delete m_GhostTrack_nInnermostPixelLayerSplitHits            ;
      delete m_GhostTrack_nNextToInnermostPixelLayerHits           ;
      delete m_GhostTrack_nNextToInnermostPixelLayerSharedHits     ;
      delete m_GhostTrack_nNextToInnermostPixelLayerSplitHits      ;
    }
  }

  // constituent
  if ( m_infoSwitch.m_constituent ) {
    delete m_numConstituents;
  }

  if ( m_infoSwitch.m_constituentAll ) {
    delete m_constituentWeights;
    delete m_constituent_pt    ;
    delete m_constituent_eta   ;
    delete m_constituent_phi   ;
    delete m_constituent_e     ;
  }


  // flavorTag
  if( m_infoSwitch.m_flavorTag  || m_infoSwitch.m_flavorTagHLT  ) {
    // flavorTag

    //delete m_MV1;
    delete m_MV2c00;
    delete m_MV2c10;
    delete m_MV2c10mu;
    delete m_MV2c10rnn;
    delete m_MV2rmu;
    delete m_MV2r;
    delete m_MV2c20;
    delete m_MV2c100;
    delete m_DL1;
    delete m_DL1_pu;
    delete m_DL1_pc;
    delete m_DL1_pb;
    delete m_DL1mu;
    delete m_DL1mu_pu;
    delete m_DL1mu_pc;
    delete m_DL1mu_pb;
    delete m_DL1rnn;
    delete m_DL1rnn_pu;
    delete m_DL1rnn_pc;
    delete m_DL1rnn_pb;
    delete m_DL1rmu;
    delete m_DL1rmu_pu;
    delete m_DL1rmu_pc;
    delete m_DL1rmu_pb;
    delete m_DL1r;
    delete m_DL1r_pu;
    delete m_DL1r_pc;
    delete m_DL1r_pb;

    delete m_HadronConeExclTruthLabelID;
    delete m_HadronConeExclExtendedTruthLabelID;

    // Jet Fitter
    if( m_infoSwitch.m_jetFitterDetails){
      delete m_JetFitter_nVTX          ;
      delete m_JetFitter_nSingleTracks ;
      delete m_JetFitter_nTracksAtVtx  ;
      delete m_JetFitter_mass          ;
      delete m_JetFitter_energyFraction;
      delete m_JetFitter_significance3d;
      delete m_JetFitter_deltaeta      ;
      delete m_JetFitter_deltaphi      ;
      delete m_JetFitter_N2Tpar        ;
    }
    // SV Details
    if( m_infoSwitch.m_svDetails){

      delete m_SV0;
      delete m_sv0_NGTinSvx  ;
      delete m_sv0_N2Tpair   ;
      delete m_sv0_massvx    ;
      delete m_sv0_efracsvx  ;
      delete m_sv0_normdist  ;

      delete m_SV1;
      delete m_SV1IP3D;
      delete m_COMBx;
      delete m_sv1_pb        ;
      delete m_sv1_pc        ;
      delete m_sv1_c         ;
      delete m_sv1_cu        ;
      delete m_sv1_NGTinSvx  ;
      delete m_sv1_N2Tpair   ;
      delete m_sv1_massvx    ;
      delete m_sv1_efracsvx  ;
      delete m_sv1_normdist  ;
      delete m_sv1_Lxy       ;
      delete m_sv1_sig3d     ;
      delete m_sv1_L3d       ;
      delete m_sv1_distmatlay;
      delete m_sv1_dR        ;
    }

    // IP3D
    if( m_infoSwitch.m_ipDetails){

      delete m_IP2D_pu     ;
      delete m_IP2D_pb     ;
      delete m_IP2D_pc     ;
      delete m_IP2D        ;
      delete m_IP2D_c      ;
      delete m_IP2D_cu     ;
      delete m_nIP2DTracks ;

      delete m_IP2D_gradeOfTracks     ;
      delete m_IP2D_flagFromV0ofTracks;
      delete m_IP2D_valD0wrtPVofTracks;
      delete m_IP2D_sigD0wrtPVofTracks;
      delete m_IP2D_weightBofTracks   ;
      delete m_IP2D_weightCofTracks   ;
      delete m_IP2D_weightUofTracks   ;

      delete m_IP3D;
      delete m_IP3D_pu     ;
      delete m_IP3D_pb     ;
      delete m_IP3D_pc     ;
      delete m_IP3D_c      ;
      delete m_IP3D_cu     ;

      delete m_nIP3DTracks ;
      delete m_IP3D_gradeOfTracks       ;
      delete m_IP3D_flagFromV0ofTracks  ;
      delete m_IP3D_valD0wrtPVofTracks  ;
      delete m_IP3D_sigD0wrtPVofTracks  ;
      delete m_IP3D_valZ0wrtPVofTracks  ;
      delete m_IP3D_sigZ0wrtPVofTracks  ;
      delete m_IP3D_weightBofTracks     ;
      delete m_IP3D_weightCofTracks     ;
      delete m_IP3D_weightUofTracks     ;

    }

    if( m_infoSwitch.m_JVC ) {
      delete m_JetVertexCharge_discriminant          ;
    }

  }

    //  flavorTagHLT
  if( m_infoSwitch.m_flavorTagHLT  ) {
    delete m_vtxOnlineValid     ;
    delete m_vtxHadDummy        ;
    delete m_bs_online_vx       ;
    delete m_bs_online_vy       ;
    delete m_bs_online_vz       ;

    delete m_vtx_offline_x0     ;
    delete m_vtx_offline_y0     ;
    delete m_vtx_offline_z0     ;

    delete m_vtx_online_x0      ;
    delete m_vtx_online_y0      ;
    delete m_vtx_online_z0      ;

    delete m_vtx_online_bkg_x0  ;
    delete m_vtx_online_bkg_y0  ;
    delete m_vtx_online_bkg_z0  ;
  }

  for(auto btag : m_btags)
    delete btag;

  // area
  if( m_infoSwitch.m_area ) {
    delete m_GhostArea          ;
    delete m_ActiveArea         ;
    delete m_VoronoiArea        ;
    delete m_ActiveArea4vec_pt  ;
    delete m_ActiveArea4vec_eta ;
    delete m_ActiveArea4vec_phi ;
    delete m_ActiveArea4vec_m   ;
  }


  // truth
  if ( m_infoSwitch.m_truth && m_mc ) {
    delete m_ConeTruthLabelID;
    delete m_TruthCount;
    delete m_TruthLabelDeltaR_B;
    delete m_TruthLabelDeltaR_C;
    delete m_TruthLabelDeltaR_T;
    delete m_PartonTruthLabelID;
    delete m_GhostTruthAssociationFraction;
    delete m_truth_E;
    delete m_truth_pt;
    delete m_truth_phi;
    delete m_truth_eta;
  }

    // truth detail
  if ( m_infoSwitch.m_truthDetails ) {
    delete m_GhostBHadronsFinalCount   ;
    delete m_GhostBHadronsInitialCount ;
    delete m_GhostBQuarksFinalCount    ;
    delete m_GhostBHadronsFinalPt      ;
    delete m_GhostBHadronsInitialPt    ;
    delete m_GhostBQuarksFinalPt       ;

    delete m_GhostCHadronsFinalCount   ;
    delete m_GhostCHadronsInitialCount ;
    delete m_GhostCQuarksFinalCount    ;
    delete m_GhostCHadronsFinalPt      ;
    delete m_GhostCHadronsInitialPt    ;
    delete m_GhostCQuarksFinalPt       ;

    delete m_GhostTausFinalCount       ;
    delete m_GhostTausFinalPt          ;

    delete m_truth_pdgId               ;
    delete m_truth_partonPt            ;
    delete m_truth_partonDR            ;
  }


  // charge
  if ( m_infoSwitch.m_charge ) {
    delete m_charge;
  }

  // passSel
  if ( m_infoSwitch.m_passSel ) {
    delete m_passSel;
  }

}

void JetContainer::setTree(TTree *tree)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  if(m_infoSwitch.m_rapidity)
    {
      connectBranch<float>(tree,"rapidity",                      &m_rapidity);
    }

  if ( m_infoSwitch.m_trigger ){
    connectBranch<int>              (tree, "isTrigMatched",        &m_isTrigMatched);
    connectBranch<std::vector<int> >(tree, "isTrigMatchedToChain", &m_isTrigMatchedToChain );
    connectBranch<std::string>      (tree, "listTrigChains",       &m_listTrigChains );
  }

  if(m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight)
    {
      if(m_infoSwitch.m_clean){
        connectBranch<float>(tree, "Timing",                     &m_Timing);
        connectBranch<float>(tree, "LArQuality",                 &m_LArQuality);
        connectBranch<int>  (tree, "LArBadHVNCell",              &m_LArBadHVNCell);
        connectBranch<float>(tree, "LArQuality",                 &m_LArQuality);
        connectBranch<float>(tree, "HECQuality",                 &m_HECQuality);
        connectBranch<float>(tree, "NegativeE",                  &m_NegativeE);
        connectBranch<float>(tree, "AverageLArQF",               &m_AverageLArQF);
        connectBranch<float>(tree, "BchCorrCell",                &m_BchCorrCell);
        connectBranch<float>(tree, "N90Constituents",            &m_N90Constituents);
        connectBranch<float>(tree, "LArBadHVEnergyFrac",         &m_LArBadHVEnergyFrac);
        connectBranch<float>(tree, "OotFracClusters5",           &m_OotFracClusters5);
        connectBranch<float>(tree, "OotFracClusters10",          &m_OotFracClusters10);
        connectBranch<float>(tree, "LeadingClusterPt",           &m_LeadingClusterPt);
        connectBranch<float>(tree, "LeadingClusterSecondLambda", &m_LeadingClusterSecondLambda);
        connectBranch<float>(tree, "LeadingClusterCenterLambda", &m_LeadingClusterCenterLambda);
        connectBranch<float>(tree, "LeadingClusterSecondR",      &m_LeadingClusterSecondR);
        if(m_infoSwitch.m_cleanTrig) {
          connectBranch<int>  (tree, "clean_passLooseBadTriggerUgly",&m_clean_passLooseBadTriggerUgly);
        }
        else {
          connectBranch<int>  (tree, "clean_passLooseBadUgly",     &m_clean_passLooseBadUgly);
          connectBranch<int>  (tree, "clean_passTightBadUgly",     &m_clean_passTightBadUgly);
        }
      }
      if(m_infoSwitch.m_cleanTrig) {
        connectBranch<int>  (tree, "clean_passLooseBadTrigger",  &m_clean_passLooseBadTrigger);
      }
      else {
        connectBranch<int>  (tree, "clean_passLooseBad",         &m_clean_passLooseBad);
        connectBranch<int>  (tree, "clean_passTightBad",         &m_clean_passTightBad);
      }
    }

  if(m_infoSwitch.m_energy || m_infoSwitch.m_energyLight )
    {
      if ( m_infoSwitch.m_energy ){
        connectBranch<float>(tree, "HECFrac",               &m_HECFrac);
        connectBranch<float>(tree, "CentroidR",             &m_CentroidR);
        connectBranch<float>(tree, "LowEtConstituentsFrac", &m_LowEtConstituentsFrac);
      }
      connectBranch<float>(tree, "EMFrac",                &m_EMFrac);
      connectBranch<float>(tree, "FracSamplingMax",       &m_FracSamplingMax);
      connectBranch<float>(tree, "FracSamplingMaxIndex",  &m_FracSamplingMaxIndex);
      connectBranch<float>(tree, "GhostMuonSegmentCount", &m_GhostMuonSegmentCount);
      connectBranch<float>(tree, "Width",                 &m_Width);
    }

  if(m_infoSwitch.m_trackPV)
    {
      connectBranch<float>(tree, "NumTrkPt1000PV",     &m_NumTrkPt1000PV);
      connectBranch<float>(tree, "SumPtTrkPt1000PV",   &m_SumPtTrkPt1000PV);
      connectBranch<float>(tree, "TrackWidthPt1000PV", &m_TrackWidthPt1000PV);
      connectBranch<float>(tree, "NumTrkPt500PV",      &m_NumTrkPt500PV);
      connectBranch<float>(tree, "SumPtTrkPt500PV",    &m_SumPtTrkPt500PV);
      connectBranch<float>(tree, "TrackWidthPt500PV",  &m_TrackWidthPt500PV);
      connectBranch<float>(tree, "JVFPV",              &m_JVFPV);
    }

  if(m_infoSwitch.m_trackAll || m_infoSwitch.m_trackPV || m_infoSwitch.m_jvt)
    {
      if(m_infoSwitch.m_trackAll || m_infoSwitch.m_trackPV){
        connectBranch<float>(tree, "JvtJvfcorr", &m_JvtJvfcorr);
        connectBranch<float>(tree, "JvtRpt",     &m_JvtRpt);
      }
      connectBranch<float>(tree, "Jvt",        &m_Jvt);
    }

  if(m_infoSwitch.m_JVC)
    {
      connectBranch<double>(tree,"JetVertexCharge_discriminant", &m_JetVertexCharge_discriminant);
    }

  if(m_infoSwitch.m_flavorTag || m_infoSwitch.m_flavorTagHLT)
    {
      connectBranch<float>(tree,"MV2c00"                            ,&m_MV2c00);
      connectBranch<float>(tree,"MV2c10"                            ,&m_MV2c10);
      connectBranch<float>(tree,"MV2c10mu"                          ,&m_MV2c10mu);
      connectBranch<float>(tree,"MV2c10rnn"                         ,&m_MV2c10rnn);
      connectBranch<float>(tree,"MV2rmu"                            ,&m_MV2rmu);
      connectBranch<float>(tree,"MV2r"                              ,&m_MV2r);
      connectBranch<float>(tree,"MV2c20"                            ,&m_MV2c20);
      connectBranch<float>(tree,"MV2c100"                           ,&m_MV2c100);
      connectBranch<float>(tree,"DL1"                               ,&m_DL1      );
      connectBranch<float>(tree,"DL1_pu"                            ,&m_DL1_pu   );
      connectBranch<float>(tree,"DL1_pc"                            ,&m_DL1_pc   );
      connectBranch<float>(tree,"DL1_pb"                            ,&m_DL1_pb   );
      connectBranch<float>(tree,"DL1mu"                             ,&m_DL1mu    );
      connectBranch<float>(tree,"DL1mu_pu"                          ,&m_DL1mu_pu );
      connectBranch<float>(tree,"DL1mu_pc"                          ,&m_DL1mu_pc );
      connectBranch<float>(tree,"DL1mu_pb"                          ,&m_DL1mu_pb );
      connectBranch<float>(tree,"DL1rnn"                            ,&m_DL1rnn   );
      connectBranch<float>(tree,"DL1rnn_pu"                         ,&m_DL1rnn_pu);
      connectBranch<float>(tree,"DL1rnn_pc"                         ,&m_DL1rnn_pc);
      connectBranch<float>(tree,"DL1rnn_pb"                         ,&m_DL1rnn_pb);
      connectBranch<float>(tree,"DL1rmu"                            ,&m_DL1rmu   );
      connectBranch<float>(tree,"DL1rmu_pu"                         ,&m_DL1rmu_pu);
      connectBranch<float>(tree,"DL1rmu_pc"                         ,&m_DL1rmu_pc);
      connectBranch<float>(tree,"DL1rmu_pb"                         ,&m_DL1rmu_pb);
      connectBranch<float>(tree,"DL1r"                              ,&m_DL1r     );
      connectBranch<float>(tree,"DL1r_pu"                           ,&m_DL1r_pu  );
      connectBranch<float>(tree,"DL1r_pc"                           ,&m_DL1r_pc  );
      connectBranch<float>(tree,"DL1r_pb"                           ,&m_DL1r_pb  );
      connectBranch<int>  (tree,"HadronConeExclTruthLabelID"        ,&m_HadronConeExclTruthLabelID);
      connectBranch<int>  (tree,"HadronConeExclExtendedTruthLabelID",&m_HadronConeExclExtendedTruthLabelID);
    }

  if(m_infoSwitch.m_flavorTagHLT)
    {
      connectBranch<float>(tree,"vtxHadDummy",    &m_vtxHadDummy);
      connectBranch<float>(tree,"bs_online_vx",   &m_bs_online_vx);
      connectBranch<float>(tree,"bs_online_vy",   &m_bs_online_vy);
      connectBranch<float>(tree,"bs_online_vz",   &m_bs_online_vz);

      connectBranch<float>(tree,"vtx_offline_x0", &m_vtx_offline_x0);
      connectBranch<float>(tree,"vtx_offline_y0", &m_vtx_offline_y0);
      connectBranch<float>(tree,"vtx_offline_z0", &m_vtx_offline_z0);

      connectBranch<float>(tree,"vtx_online_x0",  &m_vtx_online_x0);
      connectBranch<float>(tree,"vtx_online_y0",  &m_vtx_online_y0);
      connectBranch<float>(tree,"vtx_online_z0",  &m_vtx_online_z0);

      connectBranch<float>(tree,"vtx_online_bkg_x0",  &m_vtx_online_bkg_x0);
      connectBranch<float>(tree,"vtx_online_bkg_y0",  &m_vtx_online_bkg_y0);
      connectBranch<float>(tree,"vtx_online_bkg_z0",  &m_vtx_online_bkg_z0);
    }

  if(m_infoSwitch.m_jetFitterDetails)
    {
      connectBranch<float>(tree,"JetFitter_nVTX"          ,  &m_JetFitter_nVTX           );
      connectBranch<float>(tree,"JetFitter_nSingleTracks" ,  &m_JetFitter_nSingleTracks  );
      connectBranch<float>(tree,"JetFitter_nTracksAtVtx"  ,  &m_JetFitter_nTracksAtVtx   );
      connectBranch<float>(tree,"JetFitter_mass"          ,  &m_JetFitter_mass           );
      connectBranch<float>(tree,"JetFitter_energyFraction",  &m_JetFitter_energyFraction );
      connectBranch<float>(tree,"JetFitter_significance3d",  &m_JetFitter_significance3d );
      connectBranch<float>(tree,"JetFitter_deltaeta"      ,  &m_JetFitter_deltaeta       );
      connectBranch<float>(tree,"JetFitter_deltaphi"      ,  &m_JetFitter_deltaphi       );
      connectBranch<float>(tree,"JetFitter_N2Tpair"       ,  &m_JetFitter_N2Tpar         );

    }

    if( m_infoSwitch.m_svDetails){

      connectBranch<float>(tree, "SV0",               &m_SV0);
      connectBranch<float>(tree, "sv0_NGTinSvx",      &m_sv0_NGTinSvx  );
      connectBranch<float>(tree, "sv0_N2Tpair",       &m_sv0_N2Tpair   );
      connectBranch<float>(tree, "sv0_massvx",        &m_sv0_massvx    );
      connectBranch<float>(tree, "sv0_efracsvx",      &m_sv0_efracsvx  );
      connectBranch<float>(tree, "sv0_normdist",      &m_sv0_normdist  );

      connectBranch<float>(tree, "SV1",               &m_SV1);
      connectBranch<float>(tree, "SV1IP3D",           &m_SV1IP3D);
      connectBranch<float>(tree, "COMBx",             &m_COMBx);
      connectBranch<float>(tree, "sv1_pu",            &m_sv1_pu        );
      connectBranch<float>(tree, "sv1_pb",            &m_sv1_pb        );
      connectBranch<float>(tree, "sv1_pc",            &m_sv1_pc        );
      connectBranch<float>(tree, "sv1_c",             &m_sv1_c         );
      connectBranch<float>(tree, "sv1_cu",            &m_sv1_cu        );
      connectBranch<float>(tree, "sv1_NGTinSvx",      &m_sv1_NGTinSvx  );
      connectBranch<float>(tree, "sv1_N2Tpair",       &m_sv1_N2Tpair   );
      connectBranch<float>(tree, "sv1_massvx",        &m_sv1_massvx    );
      connectBranch<float>(tree, "sv1_efracsvx",      &m_sv1_efracsvx  );
      connectBranch<float>(tree, "sv1_normdist",      &m_sv1_normdist  );
      connectBranch<float>(tree, "sv1_Lxy",           &m_sv1_Lxy       );
      connectBranch<float>(tree, "sv1_sig3d",         &m_sv1_sig3d       );
      connectBranch<float>(tree, "sv1_L3d",           &m_sv1_L3d       );
      connectBranch<float>(tree, "sv1_distmatlay",    &m_sv1_distmatlay);
      connectBranch<float>(tree, "sv1_dR",            &m_sv1_dR        );

    }

    if( m_infoSwitch.m_ipDetails){

      connectBranch<float>         (tree,  "IP2D_pu",                   &m_IP2D_pu                   );
      connectBranch<float>         (tree,  "IP2D_pb",                   &m_IP2D_pb                   );
      connectBranch<float>         (tree,  "IP2D_pc",                   &m_IP2D_pc                   );
      connectBranch<float>         (tree,  "IP2D",                      &m_IP2D                      );
      connectBranch<float>         (tree,  "IP2D_c",                    &m_IP2D_c                    );
      connectBranch<float>         (tree,  "IP2D_cu",                   &m_IP2D_cu                   );
      connectBranch<std::vector<float> >(tree,  "IP2D_gradeOfTracks"       , &m_IP2D_gradeOfTracks        );
      connectBranch<std::vector<float> >(tree,  "IP2D_flagFromV0ofTracks"  , &m_IP2D_flagFromV0ofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP2D_valD0wrtPVofTracks"  , &m_IP2D_valD0wrtPVofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP2D_sigD0wrtPVofTracks"  , &m_IP2D_sigD0wrtPVofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP2D_weightBofTracks"     , &m_IP2D_weightBofTracks      );
      connectBranch<std::vector<float> >(tree,  "IP2D_weightCofTracks"     , &m_IP2D_weightCofTracks      );
      connectBranch<std::vector<float> >(tree,  "IP2D_weightUofTracks"     , &m_IP2D_weightUofTracks      );

      connectBranch<float>         (tree,  "IP3D",                      &m_IP3D);
      connectBranch<float>         (tree,  "IP3D_pu",                   &m_IP3D_pu                   );
      connectBranch<float>         (tree,  "IP3D_pb",                   &m_IP3D_pb                   );
      connectBranch<float>         (tree,  "IP3D_pc",                   &m_IP3D_pc                   );
      connectBranch<float>         (tree,  "IP3D_c",                    &m_IP3D_c                    );
      connectBranch<float>         (tree,  "IP3D_cu",                   &m_IP3D_cu                   );
      connectBranch<std::vector<float> >(tree,  "IP3D_gradeOfTracks"       , &m_IP3D_gradeOfTracks        );
      connectBranch<std::vector<float> >(tree,  "IP3D_flagFromV0ofTracks"  , &m_IP3D_flagFromV0ofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP3D_valD0wrtPVofTracks"  , &m_IP3D_valD0wrtPVofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP3D_sigD0wrtPVofTracks"  , &m_IP3D_sigD0wrtPVofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP3D_valZ0wrtPVofTracks"  , &m_IP3D_valZ0wrtPVofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP3D_sigZ0wrtPVofTracks"  , &m_IP3D_sigZ0wrtPVofTracks   );
      connectBranch<std::vector<float> >(tree,  "IP3D_weightBofTracks"     , &m_IP3D_weightBofTracks      );
      connectBranch<std::vector<float> >(tree,  "IP3D_weightCofTracks"     , &m_IP3D_weightCofTracks      );
      connectBranch<std::vector<float> >(tree,  "IP3D_weightUofTracks"     , &m_IP3D_weightUofTracks      );

    }

    for(auto btag : m_btags)
      btag->setTree(tree, m_name);

  // truth
  if(m_infoSwitch.m_truth)
    {
      connectBranch<int>  (tree,"ConeTruthLabelID",   &m_ConeTruthLabelID);
      connectBranch<int>  (tree,"TruthCount",         &m_TruthCount);
      connectBranch<float>(tree,"TruthLabelDeltaR_B", &m_TruthLabelDeltaR_B);
      connectBranch<float>(tree,"TruthLabelDeltaR_C", &m_TruthLabelDeltaR_C);
      connectBranch<float>(tree,"TruthLabelDeltaR_T", &m_TruthLabelDeltaR_T);
      connectBranch<int>  (tree,"PartonTruthLabelID", &m_PartonTruthLabelID);
      connectBranch<float>(tree,"GhostTruthAssociationFraction", &m_GhostTruthAssociationFraction);
      connectBranch<float>(tree,"truth_E",   &m_truth_E);
      connectBranch<float>(tree,"truth_pt",  &m_truth_pt);
      connectBranch<float>(tree,"truth_phi", &m_truth_phi);
      connectBranch<float>(tree,"truth_eta", &m_truth_eta);
    }

  // charge
  if(m_infoSwitch.m_charge)
    {
      connectBranch<double>(tree,"charge", &m_charge);
    }

  // passSel
  if(m_infoSwitch.m_passSel) connectBranch<char>(tree,"passSel", &m_passSel);

}

void JetContainer::updateParticle(uint idx, Jet& jet)
{
  if(m_debug) std::cout << "in JetContainer::updateParticle " << std::endl;
  ParticleContainer::updateParticle(idx,jet);

  if(m_infoSwitch.m_rapidity)
    {
      jet.rapidity                    =m_rapidity                    ->at(idx);
    }

  // trigger
  if ( m_infoSwitch.m_trigger ) {
    jet.isTrigMatched         =     m_isTrigMatched         ->at(idx);
    jet.isTrigMatchedToChain  =     m_isTrigMatchedToChain  ->at(idx);
    jet.listTrigChains        =     m_listTrigChains        ->at(idx);
  }

  if(m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight)
    {
      if(m_debug) std::cout << "updating clean " << std::endl;
      if(m_infoSwitch.m_clean){
        jet.Timing                    =m_Timing                    ->at(idx);
        jet.LArQuality                =m_LArQuality                ->at(idx);
        jet.HECQuality                =m_HECQuality                ->at(idx);
        jet.NegativeE                 =m_NegativeE                 ->at(idx);
        jet.AverageLArQF              =m_AverageLArQF              ->at(idx);
        jet.BchCorrCell               =m_BchCorrCell               ->at(idx);
        jet.N90Constituents           =m_N90Constituents           ->at(idx);
        jet.LArBadHVEFrac             =m_LArBadHVEnergyFrac       ->at(idx);
        jet.LArBadHVNCell             =m_LArBadHVNCell             ->at(idx);
        jet.OotFracClusters5          =m_OotFracClusters5          ->at(idx);
        jet.OotFracClusters10         =m_OotFracClusters10         ->at(idx);
        jet.LeadingClusterPt          =m_LeadingClusterPt          ->at(idx);
        jet.LeadingClusterSecondLambda=m_LeadingClusterSecondLambda->at(idx);
        jet.LeadingClusterCenterLambda=m_LeadingClusterCenterLambda->at(idx);
        jet.LeadingClusterSecondR     =m_LeadingClusterSecondR     ->at(idx);
        if(m_infoSwitch.m_cleanTrig) {
          jet.clean_passLooseBadTriggerUgly =m_clean_passLooseBadTriggerUgly    ->at(idx);
        }
        else {
          jet.clean_passLooseBadUgly    =m_clean_passLooseBadUgly    ->at(idx);
          jet.clean_passTightBadUgly    =m_clean_passTightBadUgly    ->at(idx);
        }
      }
      if(m_infoSwitch.m_cleanTrig) {
        jet.clean_passLooseBadTrigger =m_clean_passLooseBadTrigger ->at(idx);
      }
      else {
        jet.clean_passLooseBad        =m_clean_passLooseBad        ->at(idx);
        jet.clean_passTightBad        =m_clean_passTightBad        ->at(idx);
      }
    }

  if(m_infoSwitch.m_energy || m_infoSwitch.m_energyLight)
    {
      if(m_debug) std::cout << "updating energy " << std::endl;
      if ( m_infoSwitch.m_energy ){
        jet.HECFrac              =m_HECFrac              ->at(idx);
        jet.CentroidR            =m_CentroidR            ->at(idx);
        jet.LowEtConstituentsFrac=m_LowEtConstituentsFrac->at(idx);
      }
      jet.EMFrac               =m_EMFrac               ->at(idx);
      jet.FracSamplingMax      =m_FracSamplingMax      ->at(idx);
      jet.FracSamplingMaxIndex =m_FracSamplingMaxIndex ->at(idx);
      jet.GhostMuonSegmentCount=m_GhostMuonSegmentCount->at(idx);
      jet.Width                =m_Width                ->at(idx);
    }

  if(m_infoSwitch.m_trackPV)
    {
      jet.NumTrkPt1000PV    =m_NumTrkPt1000PV    ->at(idx);
      jet.SumPtTrkPt1000PV  =m_SumPtTrkPt1000PV  ->at(idx);
      jet.TrackWidthPt1000PV=m_TrackWidthPt1000PV->at(idx);
      jet.NumTrkPt500PV     =m_NumTrkPt500PV     ->at(idx);
      jet.SumPtTrkPt500PV   =m_SumPtTrkPt500PV   ->at(idx);
      jet.TrackWidthPt500PV =m_TrackWidthPt500PV ->at(idx);
      jet.JVFPV             =m_JVFPV             ->at(idx);
    }

  if(m_infoSwitch.m_trackPV || m_infoSwitch.m_trackAll || m_infoSwitch.m_jvt)
    {
      if(m_infoSwitch.m_trackPV || m_infoSwitch.m_trackAll){
        jet.JvtJvfcorr=m_JvtJvfcorr->at(idx);
        jet.JvtRpt    =m_JvtRpt    ->at(idx);
      }
      jet.Jvt       =m_Jvt       ->at(idx);
    }

  if( m_infoSwitch.m_JVC ) {
    if(m_debug) std::cout << "updating JVC " << std::endl;
    if(m_debug) std::cout << m_JetVertexCharge_discriminant->size() << std::endl;
    jet.JVC = m_JetVertexCharge_discriminant->at(idx);
  }

  if(m_infoSwitch.m_flavorTag  || m_infoSwitch.m_flavorTagHLT)
    {
      if(m_debug) std::cout << "updating flavorTag " << std::endl;
      jet.MV2c00                    =m_MV2c00               ->at(idx);
      jet.MV2c10                    =m_MV2c10               ->at(idx);
      if(m_MV2c10mu)  jet.MV2c10mu  =m_MV2c10mu             ->at(idx);
      if(m_MV2c10rnn) jet.MV2c10rnn =m_MV2c10rnn            ->at(idx);
      if(m_MV2rmu)    jet.MV2rmu    =m_MV2rmu               ->at(idx);
      if(m_MV2r)      jet.MV2r      =m_MV2r                 ->at(idx);
      jet.MV2c20                    =m_MV2c20               ->at(idx);
      jet.MV2c100                   =m_MV2c100              ->at(idx);
      if(m_DL1)       jet.DL1       =m_DL1                  ->at(idx);
      if(m_DL1_pu)    jet.DL1_pu    =m_DL1_pu               ->at(idx);
      if(m_DL1_pc)    jet.DL1_pc    =m_DL1_pc               ->at(idx);
      if(m_DL1_pb)    jet.DL1_pb    =m_DL1_pb               ->at(idx);
      if(m_DL1mu)     jet.DL1mu     =m_DL1mu                ->at(idx);
      if(m_DL1mu_pu)  jet.DL1mu_pu  =m_DL1mu_pu             ->at(idx);
      if(m_DL1mu_pc)  jet.DL1mu_pc  =m_DL1mu_pc             ->at(idx);
      if(m_DL1mu_pb)  jet.DL1mu_pb  =m_DL1mu_pb             ->at(idx);
      if(m_DL1rnn)    jet.DL1rnn    =m_DL1rnn               ->at(idx);
      if(m_DL1rnn_pu) jet.DL1rnn_pu =m_DL1rnn_pu            ->at(idx);
      if(m_DL1rnn_pc) jet.DL1rnn_pc =m_DL1rnn_pc            ->at(idx);
      if(m_DL1rnn_pb) jet.DL1rnn_pb =m_DL1rnn_pb            ->at(idx);
      if(m_DL1rmu)    jet.DL1rmu    =m_DL1rmu               ->at(idx);
      if(m_DL1rmu_pu) jet.DL1rmu_pu =m_DL1rmu_pu            ->at(idx);
      if(m_DL1rmu_pc) jet.DL1rmu_pc =m_DL1rmu_pc            ->at(idx);
      if(m_DL1rmu_pb) jet.DL1rmu_pb =m_DL1rmu_pb            ->at(idx);
      if(m_DL1r)      jet.DL1r      =m_DL1r                 ->at(idx);
      if(m_DL1r_pu)   jet.DL1r_pu   =m_DL1r_pu              ->at(idx);
      if(m_DL1r_pc)   jet.DL1r_pc   =m_DL1r_pc              ->at(idx);
      if(m_DL1r_pb)   jet.DL1r_pb   =m_DL1r_pb              ->at(idx);
      //std::cout << m_HadronConeExclTruthLabelID->size() << std::endl;
      if(m_HadronConeExclTruthLabelID)         jet.HadronConeExclTruthLabelID        =m_HadronConeExclTruthLabelID        ->at(idx);
      if(m_HadronConeExclExtendedTruthLabelID) jet.HadronConeExclExtendedTruthLabelID=m_HadronConeExclExtendedTruthLabelID->at(idx);
      if(m_debug) std::cout << "leave flavorTag " << std::endl;
    }


  if(m_infoSwitch.m_flavorTagHLT)
    {
      if(m_debug) std::cout << "updating flavorTagHLT " << std::endl;
      jet.bs_online_vx                      =m_bs_online_vx                  ->at(idx);
      jet.bs_online_vy                      =m_bs_online_vy                  ->at(idx);
      jet.bs_online_vz                      =m_bs_online_vz                  ->at(idx);
      jet.vtxHadDummy                       =m_vtxHadDummy                   ->at(idx);
      jet.vtx_offline_x0                    =m_vtx_offline_x0                  ->at(idx);
      jet.vtx_offline_y0                    =m_vtx_offline_y0                  ->at(idx);
      jet.vtx_offline_z0                    =m_vtx_offline_z0                  ->at(idx);

      jet.vtx_online_x0                     =m_vtx_online_x0                  ->at(idx);
      jet.vtx_online_y0                     =m_vtx_online_y0                  ->at(idx);
      jet.vtx_online_z0                     =m_vtx_online_z0                  ->at(idx);

      jet.vtx_online_bkg_x0                     =m_vtx_online_bkg_x0                  ->at(idx);
      jet.vtx_online_bkg_y0                     =m_vtx_online_bkg_y0                  ->at(idx);
      jet.vtx_online_bkg_z0                     =m_vtx_online_bkg_z0                  ->at(idx);

    }

  if(m_infoSwitch.m_jetFitterDetails)
    {
      jet.JetFitter_nVTX                  =m_JetFitter_nVTX           ->at(idx);
      jet.JetFitter_nSingleTracks         =m_JetFitter_nSingleTracks  ->at(idx);
      jet.JetFitter_nTracksAtVtx          =m_JetFitter_nTracksAtVtx   ->at(idx);
      jet.JetFitter_mass                  =m_JetFitter_mass           ->at(idx);
      jet.JetFitter_energyFraction        =m_JetFitter_energyFraction ->at(idx);
      jet.JetFitter_significance3d        =m_JetFitter_significance3d ->at(idx);
      jet.JetFitter_deltaeta              =m_JetFitter_deltaeta       ->at(idx);
      jet.JetFitter_deltaphi              =m_JetFitter_deltaphi       ->at(idx);
      jet.JetFitter_N2Tpar                =m_JetFitter_N2Tpar         ->at(idx);

    }

  if(m_infoSwitch.m_svDetails){

    jet.SV0            = m_SV0           ->at(idx);
    jet.sv0_NGTinSvx   = m_sv0_NGTinSvx  ->at(idx);
    jet.sv0_N2Tpair    = m_sv0_N2Tpair   ->at(idx);
    jet.sv0_massvx     = m_sv0_massvx    ->at(idx);
    jet.sv0_efracsvx   = m_sv0_efracsvx  ->at(idx);
    jet.sv0_normdist   = m_sv0_normdist  ->at(idx);

    jet.SV1            = m_SV1           ->at(idx);
    jet.SV1IP3D        = m_SV1IP3D       ->at(idx);
    jet.COMBx          = m_COMBx         ->at(idx);
    jet.sv1_pu         = m_sv1_pu        ->at(idx);
    jet.sv1_pb         = m_sv1_pb        ->at(idx);
    jet.sv1_pc         = m_sv1_pc        ->at(idx);
    jet.sv1_c          = m_sv1_c         ->at(idx);
    jet.sv1_cu         = m_sv1_cu        ->at(idx);
    jet.sv1_NGTinSvx   = m_sv1_NGTinSvx  ->at(idx);
    jet.sv1_N2Tpair    = m_sv1_N2Tpair   ->at(idx);
    jet.sv1_massvx     = m_sv1_massvx    ->at(idx);
    jet.sv1_efracsvx   = m_sv1_efracsvx  ->at(idx);
    jet.sv1_normdist   = m_sv1_normdist  ->at(idx);
    jet.sv1_Lxy        = m_sv1_Lxy       ->at(idx);
    if(m_sv1_sig3d->size())
      jet.sv1_sig3d      = m_sv1_sig3d     ->at(idx);
    jet.sv1_L3d        = m_sv1_L3d       ->at(idx);
    jet.sv1_distmatlay = m_sv1_distmatlay->at(idx);
    jet.sv1_dR         = m_sv1_dR        ->at(idx);
  }

  if(m_infoSwitch.m_ipDetails){
    jet.IP2D_pu                          = m_IP2D_pu                   ->at(idx);
    jet.IP2D_pb                          = m_IP2D_pb                   ->at(idx);
    jet.IP2D_pc                          = m_IP2D_pc                   ->at(idx);
    jet.IP2D                             = m_IP2D                      ->at(idx);
    jet.IP2D_c                           = m_IP2D_c                    ->at(idx);
    jet.IP2D_cu                          = m_IP2D_cu                   ->at(idx);
    jet.nIP2DTracks                      = m_IP2D_gradeOfTracks        ->at(idx).size();

    jet.IP2D_gradeOfTracks               = m_IP2D_gradeOfTracks        ->at(idx);
    jet.IP2D_flagFromV0ofTracks          = m_IP2D_flagFromV0ofTracks   ->at(idx);
    jet.IP2D_valD0wrtPVofTracks          = m_IP2D_valD0wrtPVofTracks   ->at(idx);
    jet.IP2D_sigD0wrtPVofTracks          = m_IP2D_sigD0wrtPVofTracks   ->at(idx);
    jet.IP2D_weightBofTracks             = m_IP2D_weightBofTracks      ->at(idx);
    jet.IP2D_weightCofTracks             = m_IP2D_weightCofTracks      ->at(idx);
    jet.IP2D_weightUofTracks             = m_IP2D_weightUofTracks      ->at(idx);

    jet.IP3D                             = m_IP3D                      ->at(idx);
    jet.IP3D_pu                          = m_IP3D_pu                   ->at(idx);
    jet.IP3D_pb                          = m_IP3D_pb                   ->at(idx);
    jet.IP3D_pc                          = m_IP3D_pc                   ->at(idx);
    jet.IP3D_c                           = m_IP3D_c                    ->at(idx);
    jet.IP3D_cu                          = m_IP3D_cu                   ->at(idx);
    jet.nIP3DTracks                      = m_IP3D_gradeOfTracks        ->at(idx).size();
    jet.IP3D_gradeOfTracks               = m_IP3D_gradeOfTracks        ->at(idx);
    jet.IP3D_flagFromV0ofTracks          = m_IP3D_flagFromV0ofTracks   ->at(idx);
    jet.IP3D_valD0wrtPVofTracks          = m_IP3D_valD0wrtPVofTracks   ->at(idx);
    jet.IP3D_sigD0wrtPVofTracks          = m_IP3D_sigD0wrtPVofTracks   ->at(idx);
    jet.IP3D_valZ0wrtPVofTracks          = m_IP3D_valZ0wrtPVofTracks   ->at(idx);
    jet.IP3D_sigZ0wrtPVofTracks          = m_IP3D_sigZ0wrtPVofTracks   ->at(idx);
    jet.IP3D_weightBofTracks             = m_IP3D_weightBofTracks      ->at(idx);
    jet.IP3D_weightCofTracks             = m_IP3D_weightCofTracks      ->at(idx);
    jet.IP3D_weightUofTracks             = m_IP3D_weightUofTracks      ->at(idx);
  }

  static const std::vector<float> dummy1 = {1.};
  for(btagOpPoint* btag : m_btags)
    {
      switch(btag->m_op)
	{
	case Jet::BTaggerOP::DL1_FixedCutBEff_60:
	  jet.is_DL1_FixedCutBEff_60=       btag->m_isTag->at(idx);
	  jet.SF_DL1_FixedCutBEff_60=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1_FixedCutBEff_70:
	  jet.is_DL1_FixedCutBEff_70=       btag->m_isTag->at(idx);
	  jet.SF_DL1_FixedCutBEff_70=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1_FixedCutBEff_77:
	  jet.is_DL1_FixedCutBEff_77=       btag->m_isTag->at(idx);
	  jet.SF_DL1_FixedCutBEff_77=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1_FixedCutBEff_85:
	  jet.is_DL1_FixedCutBEff_85=       btag->m_isTag->at(idx);
	  jet.SF_DL1_FixedCutBEff_85=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;	  
	case Jet::BTaggerOP::DL1r_FixedCutBEff_60:
	  jet.is_DL1r_FixedCutBEff_60=       btag->m_isTag->at(idx);
	  jet.SF_DL1r_FixedCutBEff_60=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1r_FixedCutBEff_70:
	  jet.is_DL1r_FixedCutBEff_70=       btag->m_isTag->at(idx);
	  jet.SF_DL1r_FixedCutBEff_70=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1r_FixedCutBEff_77:
	  jet.is_DL1r_FixedCutBEff_77=       btag->m_isTag->at(idx);
	  jet.SF_DL1r_FixedCutBEff_77=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1r_FixedCutBEff_85:
	  jet.is_DL1r_FixedCutBEff_85=       btag->m_isTag->at(idx);
	  jet.SF_DL1r_FixedCutBEff_85=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;	  
	case Jet::BTaggerOP::DL1rmu_FixedCutBEff_60:
	  jet.is_DL1rmu_FixedCutBEff_60=       btag->m_isTag->at(idx);
	  jet.SF_DL1rmu_FixedCutBEff_60=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1rmu_FixedCutBEff_70:
	  jet.is_DL1rmu_FixedCutBEff_70=       btag->m_isTag->at(idx);
	  jet.SF_DL1rmu_FixedCutBEff_70=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1rmu_FixedCutBEff_77:
	  jet.is_DL1rmu_FixedCutBEff_77=       btag->m_isTag->at(idx);
	  jet.SF_DL1rmu_FixedCutBEff_77=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1rmu_FixedCutBEff_85:
	  jet.is_DL1rmu_FixedCutBEff_85=       btag->m_isTag->at(idx);
	  jet.SF_DL1rmu_FixedCutBEff_85=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::MV2c10_FixedCutBEff_60:
	  jet.is_MV2c10_FixedCutBEff_60=       btag->m_isTag->at(idx);
	  jet.SF_MV2c10_FixedCutBEff_60=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::MV2c10_FixedCutBEff_70:
	  jet.is_MV2c10_FixedCutBEff_70=       btag->m_isTag->at(idx);
	  jet.SF_MV2c10_FixedCutBEff_70=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::MV2c10_FixedCutBEff_77:
	  jet.is_MV2c10_FixedCutBEff_77=       btag->m_isTag->at(idx);
	  jet.SF_MV2c10_FixedCutBEff_77=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::MV2c10_FixedCutBEff_85:
	  jet.is_MV2c10_FixedCutBEff_85=       btag->m_isTag->at(idx);
	  jet.SF_MV2c10_FixedCutBEff_85=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::MV2c10_Continuous:
	  jet.is_MV2c10_Continuous=       btag->m_isTag->at(idx);
	  jet.SF_MV2c10_Continuous=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1_Continuous:
	  jet.is_DL1_Continuous=       btag->m_isTag->at(idx);
	  jet.SF_DL1_Continuous=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1r_Continuous:
	  jet.is_DL1r_Continuous=       btag->m_isTag->at(idx);
	  jet.SF_DL1r_Continuous=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	case Jet::BTaggerOP::DL1rmu_Continuous:
	  jet.is_DL1rmu_Continuous=       btag->m_isTag->at(idx);
	  jet.SF_DL1rmu_Continuous=(m_mc)?btag->m_sf   ->at(idx):dummy1;
	  break;
	default:
	  throw std::domain_error(
				  __FILE__ + std::string(", in ") + __func__ + ": "
				  + "unexpected value of btag->m_op (of type xAH::Jet::BTaggerOP)");
	}
    }

  // truth
  if(m_infoSwitch.m_truth)
    {
      jet.ConeTruthLabelID  =m_ConeTruthLabelID->at(idx);
      jet.TruthCount        =m_TruthCount      ->at(idx);
      jet.TruthLabelDeltaR_B=m_TruthLabelDeltaR_B->at(idx);
      jet.TruthLabelDeltaR_C=m_TruthLabelDeltaR_C->at(idx);
      jet.TruthLabelDeltaR_T=m_TruthLabelDeltaR_T->at(idx);
      jet.PartonTruthLabelID=m_PartonTruthLabelID->at(idx);
      jet.GhostTruthAssociationFraction= m_GhostTruthAssociationFraction->at(idx);

      jet.truth_p4.SetPtEtaPhiE(m_truth_pt ->at(idx),
                                m_truth_eta->at(idx),
                                m_truth_phi->at(idx),
                                m_truth_E  ->at(idx));
    }

  // charge
  if(m_infoSwitch.m_charge)
    {
      jet.charge=m_charge->at(idx);
    }

  // passSel
  if(m_infoSwitch.m_passSel) jet.passSel=m_passSel->at(idx);

  if(m_debug) std::cout << "leave JetContainer::updateParticle " << std::endl;
  return;
}



void JetContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  if ( m_infoSwitch.m_rapidity ) {
    setBranch<float>(tree,"rapidity",                      m_rapidity              );
  }

  if ( m_infoSwitch.m_trigger ){
    // this is true if there's a match for at least one trigger chain
    setBranch<int>(tree,"isTrigMatched", m_isTrigMatched);
    // a vector of trigger match decision for each jet trigger chain
    setBranch<std::vector<int> >(tree,"isTrigMatchedToChain", m_isTrigMatchedToChain );
    // a vector of strings for each jet trigger chain - 1:1 correspondence w/ vector above
    setBranch<std::string>(tree, "listTrigChains", m_listTrigChains );
  }

  if( m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight) {
    if(m_infoSwitch.m_clean){
      setBranch<float>(tree,"Timing",                        m_Timing               );
      setBranch<float>(tree,"LArQuality",                    m_LArQuality         );
      setBranch<float>(tree,"HECQuality",                    m_HECQuality               );
      setBranch<float>(tree,"NegativeE",                     m_NegativeE               );
      setBranch<float>(tree,"AverageLArQF",                  m_AverageLArQF            );
      setBranch<float>(tree,"BchCorrCell",                   m_BchCorrCell        );
      setBranch<float>(tree,"N90Constituents",               m_N90Constituents           );
      setBranch<float>(tree,"LArBadHVEnergyFrac",            m_LArBadHVEnergyFrac   );
      setBranch<int>  (tree,"LArBadHVNCell",                 m_LArBadHVNCell  	  );
      setBranch<float>(tree,"OotFracClusters5",              m_OotFracClusters5  	    );
      setBranch<float>(tree,"OotFracClusters10",             m_OotFracClusters10  	  );
      setBranch<float>(tree,"LeadingClusterPt",              m_LeadingClusterPt  	            );
      setBranch<float>(tree,"LeadingClusterSecondLambda",    m_LeadingClusterSecondLambda  	  );
      setBranch<float>(tree,"LeadingClusterCenterLambda",    m_LeadingClusterCenterLambda  	  );
      setBranch<float>(tree,"LeadingClusterSecondR",         m_LeadingClusterSecondR  	      );
      if(m_infoSwitch.m_cleanTrig) {
        setBranch<int>  (tree,"clean_passLooseBadTriggerUgly", m_clean_passLooseBadTriggerUgly  );
      }
      else {
        setBranch<int>  (tree,"clean_passLooseBadUgly",        m_clean_passLooseBadUgly         );
        setBranch<int>  (tree,"clean_passTightBadUgly",        m_clean_passTightBadUgly         );
      }
    }
    if(m_infoSwitch.m_cleanTrig) {
      setBranch<int>  (tree,"clean_passLooseBadTrigger",     m_clean_passLooseBadTrigger      );
    }
    else {
      setBranch<int>  (tree,"clean_passLooseBad",            m_clean_passLooseBad             );
      setBranch<int>  (tree,"clean_passTightBad",            m_clean_passTightBad             );
    }
  }


  if ( m_infoSwitch.m_energy || m_infoSwitch.m_energyLight ) {
    if ( m_infoSwitch.m_energy ){
      setBranch<float>(tree,"HECFrac",                   m_HECFrac            );
      setBranch<float>(tree,"CentroidR",                 m_CentroidR      );
      setBranch<float>(tree,"LowEtConstituentsFrac",     m_LowEtConstituentsFrac      );
    }
    setBranch<float>(tree,"EMFrac",                    m_EMFrac     );
    setBranch<float>(tree,"FracSamplingMax",           m_FracSamplingMax    );
    setBranch<float>(tree,"FracSamplingMaxIndex",      m_FracSamplingMaxIndex );
    setBranch<float>(tree,"GhostMuonSegmentCount",     m_GhostMuonSegmentCount   );
    setBranch<float>(tree,"Width",                     m_Width          );
  }

  if ( m_infoSwitch.m_scales ) {
    setBranch<float>(tree,"emScalePt",              m_emScalePt            );
    setBranch<float>(tree,"constScalePt",           m_constScalePt         );
    setBranch<float>(tree,"pileupScalePt",          m_pileupScalePt        );
    setBranch<float>(tree,"originConstitScalePt",   m_originConstitScalePt );
    setBranch<float>(tree,"etaJESScalePt",          m_etaJESScalePt        );
    setBranch<float>(tree,"gscScalePt",             m_gscScalePt           );
    setBranch<float>(tree,"jmsScalePt",             m_jmsScalePt           );
    setBranch<float>(tree,"insituScalePt",          m_insituScalePt        );

    setBranch<float>(tree,"emScaleM",              m_emScaleM            );
    setBranch<float>(tree,"constScaleM",           m_constScaleM         );
    setBranch<float>(tree,"pileupScaleM",          m_pileupScaleM        );
    setBranch<float>(tree,"originConstitScaleM",   m_originConstitScaleM );
    setBranch<float>(tree,"etaJESScaleM",          m_etaJESScaleM        );
    setBranch<float>(tree,"gscScaleM",             m_gscScaleM           );
    setBranch<float>(tree,"jmsScaleM",             m_jmsScaleM           );
    setBranch<float>(tree,"insituScaleM",          m_insituScaleM        );
  }

  if ( m_infoSwitch.m_constscaleEta ) {
    setBranch<float>(tree,"constScaleEta",              m_constScaleEta            );
  }

  if ( m_infoSwitch.m_detectorEta ) {
    setBranch<float>(tree,"detectorEta",              m_detectorEta            );
  }

  if ( m_infoSwitch.m_layer ) {
    setBranch<std::vector<float> >(tree,"EnergyPerSampling",     m_EnergyPerSampling   );
  }

  if ( m_infoSwitch.m_trackAll ) {
    setBranch<std::vector<int  > >(tree,"NumTrkPt1000",      m_NumTrkPt1000   );
    setBranch<std::vector<float> >(tree,"SumPtTrkPt1000",    m_SumPtTrkPt1000  );
    setBranch<std::vector<float> >(tree,"TrackWidthPt1000",  m_TrackWidthPt1000   );
    setBranch<std::vector<int  > >(tree,"NumTrkPt500",       m_NumTrkPt500    );
    setBranch<std::vector<float> >(tree,"SumPtTrkPt500",     m_SumPtTrkPt500  );
    setBranch<std::vector<float> >(tree,"TrackWidthPt500",   m_TrackWidthPt500    );
    setBranch<std::vector<float> >(tree,"JVF",               m_JVF         );
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_jvt ) {
    if ( m_infoSwitch.m_trackPV ) {
      setBranch<float>(tree,"NumTrkPt1000PV",       m_NumTrkPt1000PV   );
      setBranch<float>(tree,"SumPtTrkPt1000PV",     m_SumPtTrkPt1000PV  );
      setBranch<float>(tree,"TrackWidthPt1000PV",   m_TrackWidthPt1000PV   );
      setBranch<float>(tree,"NumTrkPt500PV",        m_NumTrkPt500PV    );
      setBranch<float>(tree,"SumPtTrkPt500PV",      m_SumPtTrkPt500PV   );
      setBranch<float>(tree,"TrackWidthPt500PV",    m_TrackWidthPt500PV    );
      setBranch<float>(tree,"JVFPV",                m_JVFPV             );
      setBranch<float>(tree,"JvtJvfcorr",           m_JvtJvfcorr     );
      setBranch<float>(tree,"JvtRpt",               m_JvtRpt         );
    }
    setBranch<float>(tree,"Jvt",                  m_Jvt                 );
    //setBranch<float>(tree,"GhostTrackAssociationFraction", m_ghostTrackAssFrac);
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Loose" ) {
    setBranch<int>(tree,"JvtPass_Loose",        m_JvtPass_Loose );
    if ( m_mc ) {
      setBranch<std::vector<float> >(tree,"JvtEff_SF_Loose",     m_JvtEff_SF_Loose );
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Medium" ) {
    setBranch<int>(tree,"JvtPass_Medium",       m_JvtPass_Medium );
    if ( m_mc ) {
      setBranch<std::vector<float> >(tree,"JvtEff_SF_Medium",    m_JvtEff_SF_Medium );
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Tight" ) {
    setBranch<int>(tree,"JvtPass_Tight",        m_JvtPass_Tight );
    if ( m_mc ) {
      setBranch<std::vector<float> >(tree,"JvtEff_SF_Tight",     m_JvtEff_SF_Tight );
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Medium" ) {
    setBranch<int>(tree,"fJvtPass_Medium",       m_fJvtPass_Medium );
    if ( m_mc ) {
      setBranch<std::vector<float> >(tree,"fJvtEff_SF_Medium",    m_fJvtEff_SF_Medium );
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Tight" ) {
    setBranch<int>(tree,"fJvtPass_Tight",        m_fJvtPass_Tight );
    if ( m_mc ) {
      setBranch<std::vector<float> >(tree,"fJvtEff_SF_Tight",     m_fJvtEff_SF_Tight );
    }
  }


  if ( m_infoSwitch.m_allTrack ) {
    // if want to apply the selection of the PV then need to setup track selection tool
    // this applies the JVF/JVT selection cuts
    // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JvtManualRecalculation
    if( m_infoSwitch.m_allTrackPVSel ) {
      m_trkSelTool = new InDet::InDetTrackSelectionTool( "JetTrackSelection", "Loose" );
      m_trkSelTool->initialize();
      // to do this need to have AddJets return a status code
      //ANA_CHECK( m_trkSelTool->initialize());
    }
    setBranch<int  >(tree,"GhostTrackCount",  m_GhostTrackCount );
    setBranch<float>(tree,"GhostTrackPt",     m_GhostTrackPt    );
    setBranch<std::vector<float> >(tree,"GhostTrack_pt",    m_GhostTrack_pt   );
    setBranch<std::vector<float> >(tree,"GhostTrack_qOverP",m_GhostTrack_qOverP);
    setBranch<std::vector<float> >(tree,"GhostTrack_eta",   m_GhostTrack_eta  );
    setBranch<std::vector<float> >(tree,"GhostTrack_phi",   m_GhostTrack_phi  );
    setBranch<std::vector<float> >(tree,"GhostTrack_e",     m_GhostTrack_e    );
    setBranch<std::vector<float> >(tree,"GhostTrack_d0",    m_GhostTrack_d0   );
    setBranch<std::vector<float> >(tree,"GhostTrack_z0",    m_GhostTrack_z0   );
    if ( m_infoSwitch.m_allTrackDetail ) {
      setBranch<std::vector<int> >(tree,"GhostTrack_nPixelHits",                           m_GhostTrack_nPixelHits);
      setBranch<std::vector<int> >(tree,"GhostTrack_nSCTHits",                             m_GhostTrack_nSCTHits);
      setBranch<std::vector<int> >(tree,"GhostTrack_nTRTHits",                             m_GhostTrack_nTRTHits);
      setBranch<std::vector<int> >(tree,"GhostTrack_nPixelSharedHits",                     m_GhostTrack_nPixelSharedHits);
      setBranch<std::vector<int> >(tree,"GhostTrack_nPixelSplitHits",                      m_GhostTrack_nPixelSplitHits);
      setBranch<std::vector<int> >(tree,"m_GhostTrack_nInnermostPixelLayerHits",             m_GhostTrack_nInnermostPixelLayerHits);
      setBranch<std::vector<int> >(tree,"m_GhostTrack_nInnermostPixelLayerSharedHits",       m_GhostTrack_nInnermostPixelLayerSharedHits);
      setBranch<std::vector<int> >(tree,"m_GhostTrack_nInnermostPixelLayerSplitHits",        m_GhostTrack_nInnermostPixelLayerSplitHits);
      setBranch<std::vector<int> >(tree,"m_GhostTrack_nNextToInnermostPixelLayerHits",       m_GhostTrack_nNextToInnermostPixelLayerHits);
      setBranch<std::vector<int> >(tree,"m_GhostTrack_nNextToInnermostPixelLayerSharedHits", m_GhostTrack_nNextToInnermostPixelLayerSharedHits);
      setBranch<std::vector<int> >(tree,"m_GhostTrack_nNextToInnermostPixelLayerSplitHits",  m_GhostTrack_nNextToInnermostPixelLayerSplitHits);
    }
  }

  if ( m_infoSwitch.m_constituent ) {
    setBranch<int  >(tree,"numConstituents",   m_numConstituents);
  }

  if ( m_infoSwitch.m_constituentAll ) {
    setBranch<std::vector<float> >(tree,"constituentWeights", m_constituentWeights);
    setBranch<std::vector<float> >(tree,"constituent_pt",     m_constituent_pt    );
    setBranch<std::vector<float> >(tree,"constituent_eta",    m_constituent_eta   );
    setBranch<std::vector<float> >(tree,"constituent_phi",    m_constituent_phi   );
    setBranch<std::vector<float> >(tree,"constituent_e",      m_constituent_e     );
  }

  if( m_infoSwitch.m_flavorTag  || m_infoSwitch.m_flavorTagHLT  ) {

    setBranch<float>(tree,"MV2c00",    m_MV2c00);
    setBranch<float>(tree,"MV2c10",    m_MV2c10);
    setBranch<float>(tree,"MV2c10mu",  m_MV2c10mu);
    setBranch<float>(tree,"MV2c10rnn", m_MV2c10rnn);
    setBranch<float>(tree,"MV2rmu",    m_MV2rmu);
    setBranch<float>(tree,"MV2r",      m_MV2r);
    setBranch<float>(tree,"MV2c20",    m_MV2c20);
    setBranch<float>(tree,"MV2c100",   m_MV2c100);
    setBranch<float>(tree,"DL1",       m_DL1);
    setBranch<float>(tree,"DL1_pu",    m_DL1_pu);
    setBranch<float>(tree,"DL1_pc",    m_DL1_pc);
    setBranch<float>(tree,"DL1_pb",    m_DL1_pb);
    setBranch<float>(tree,"DL1mu",     m_DL1mu);
    setBranch<float>(tree,"DL1mu_pu",  m_DL1mu_pu);
    setBranch<float>(tree,"DL1mu_pc",  m_DL1mu_pc);
    setBranch<float>(tree,"DL1mu_pb",  m_DL1mu_pb);
    setBranch<float>(tree,"DL1rnn",    m_DL1rnn);
    setBranch<float>(tree,"DL1rnn_pu", m_DL1rnn_pu);
    setBranch<float>(tree,"DL1rnn_pc", m_DL1rnn_pc);
    setBranch<float>(tree,"DL1rnn_pb", m_DL1rnn_pb);
    setBranch<float>(tree,"DL1rmu",     m_DL1rmu);
    setBranch<float>(tree,"DL1rmu_pu",  m_DL1rmu_pu);
    setBranch<float>(tree,"DL1rmu_pc",  m_DL1rmu_pc);
    setBranch<float>(tree,"DL1rmu_pb",  m_DL1rmu_pb);
    setBranch<float>(tree,"DL1r",    m_DL1r);
    setBranch<float>(tree,"DL1r_pu", m_DL1r_pu);
    setBranch<float>(tree,"DL1r_pc", m_DL1r_pc);
    setBranch<float>(tree,"DL1r_pb", m_DL1r_pb);

    setBranch<int  >(tree,"HadronConeExclTruthLabelID", m_HadronConeExclTruthLabelID);
    setBranch<int  >(tree,"HadronConeExclExtendedTruthLabelID", m_HadronConeExclExtendedTruthLabelID);

    if( m_infoSwitch.m_jetFitterDetails){

      setBranch<float>(tree, "JetFitter_nVTX"          ,   m_JetFitter_nVTX            );
      setBranch<float>(tree, "JetFitter_nSingleTracks" ,   m_JetFitter_nSingleTracks   );
      setBranch<float>(tree, "JetFitter_nTracksAtVtx"  ,   m_JetFitter_nTracksAtVtx    );
      setBranch<float>(tree, "JetFitter_mass"          ,   m_JetFitter_mass            );
      setBranch<float>(tree, "JetFitter_energyFraction",   m_JetFitter_energyFraction  );
      setBranch<float>(tree, "JetFitter_significance3d",   m_JetFitter_significance3d  );
      setBranch<float>(tree, "JetFitter_deltaeta"      ,   m_JetFitter_deltaeta        );
      setBranch<float>(tree, "JetFitter_deltaphi"      ,   m_JetFitter_deltaphi        );
      setBranch<float>(tree, "JetFitter_N2Tpair"       ,   m_JetFitter_N2Tpar          );
      //setBranch<float>(tree, "JetFitter_pb"            ,   m_JetFitter_pb              );
      //setBranch<float>(tree, "JetFitter_pc"            ,   m_JetFitter_pc              );
      //setBranch<float>(tree, "JetFitter_pu"            ,   m_JetFitter_pu              );

    }

    if( m_infoSwitch.m_svDetails){
      setBranch<float>(tree, "SV0",               m_SV0);
      setBranch<float>(tree, "sv0_NGTinSvx",      m_sv0_NGTinSvx  );
      setBranch<float>(tree, "sv0_N2Tpair",       m_sv0_N2Tpair   );
      setBranch<float>(tree, "sv0_massvx",        m_sv0_massvx    );
      setBranch<float>(tree, "sv0_efracsvx",      m_sv0_efracsvx  );
      setBranch<float>(tree, "sv0_normdist",      m_sv0_normdist  );

      setBranch<float>(tree, "SV1",               m_SV1);
      setBranch<float>(tree, "SV1IP3D",           m_SV1IP3D);
      setBranch<float>(tree, "COMBx",             m_COMBx);
      setBranch<float>(tree, "sv1_pu",            m_sv1_pu        );
      setBranch<float>(tree, "sv1_pb",            m_sv1_pb        );
      setBranch<float>(tree, "sv1_pc",            m_sv1_pc        );
      setBranch<float>(tree, "sv1_c",             m_sv1_c         );
      setBranch<float>(tree, "sv1_cu",            m_sv1_cu        );
      setBranch<float>(tree, "sv1_NGTinSvx",      m_sv1_NGTinSvx  );
      setBranch<float>(tree, "sv1_N2Tpair",       m_sv1_N2Tpair   );
      setBranch<float>(tree, "sv1_massvx",        m_sv1_massvx    );
      setBranch<float>(tree, "sv1_efracsvx",      m_sv1_efracsvx  );
      setBranch<float>(tree, "sv1_normdist",      m_sv1_normdist  );
      setBranch<float>(tree, "sv1_Lxy",           m_sv1_Lxy       );
      setBranch<float>(tree, "sv1_sig3d",         m_sv1_sig3d       );
      setBranch<float>(tree, "sv1_L3d",           m_sv1_L3d       );
      setBranch<float>(tree, "sv1_distmatlay",    m_sv1_distmatlay);
      setBranch<float>(tree, "sv1_dR",            m_sv1_dR        );
    }

    if( m_infoSwitch.m_ipDetails){

      setBranch<float>(tree,  "IP2D_pu",                   m_IP2D_pu                   );
      setBranch<float>(tree,  "IP2D_pb",                   m_IP2D_pb                   );
      setBranch<float>(tree,  "IP2D_pc",                   m_IP2D_pc                   );
      setBranch<float>(tree,  "IP2D",                      m_IP2D                      );
      setBranch<float>(tree,  "IP2D_c",                    m_IP2D_c                    );
      setBranch<float>(tree,  "IP2D_cu",                   m_IP2D_cu                   );
      setBranch<float>(tree,  "nIP2DTracks"              , m_nIP2DTracks               );
      setBranch<std::vector<float> >(tree,  "IP2D_gradeOfTracks"       , m_IP2D_gradeOfTracks        );
      setBranch<std::vector<float> >(tree,  "IP2D_flagFromV0ofTracks"  , m_IP2D_flagFromV0ofTracks   );
      setBranch<std::vector<float> >(tree,  "IP2D_valD0wrtPVofTracks"  , m_IP2D_valD0wrtPVofTracks   );
      setBranch<std::vector<float> >(tree,  "IP2D_sigD0wrtPVofTracks"  , m_IP2D_sigD0wrtPVofTracks   );
      setBranch<std::vector<float> >(tree,  "IP2D_weightBofTracks"     , m_IP2D_weightBofTracks      );
      setBranch<std::vector<float> >(tree,  "IP2D_weightCofTracks"     , m_IP2D_weightCofTracks      );
      setBranch<std::vector<float> >(tree,  "IP2D_weightUofTracks"     , m_IP2D_weightUofTracks      );

      setBranch<float>(tree,  "IP3D",                      m_IP3D);
      setBranch<float>(tree,  "IP3D_pu",                   m_IP3D_pu                   );
      setBranch<float>(tree,  "IP3D_pb",                   m_IP3D_pb                   );
      setBranch<float>(tree,  "IP3D_pc",                   m_IP3D_pc                   );
      setBranch<float>(tree,  "IP3D",                      m_IP3D                      );
      setBranch<float>(tree,  "IP3D_c",                    m_IP3D_c                    );
      setBranch<float>(tree,  "IP3D_cu",                   m_IP3D_cu                   );
      setBranch<float>(tree,  "nIP3DTracks"              , m_nIP3DTracks               );
      setBranch<std::vector<float> >(tree,  "IP3D_gradeOfTracks"       , m_IP3D_gradeOfTracks        );
      setBranch<std::vector<float> >(tree,  "IP3D_flagFromV0ofTracks"  , m_IP3D_flagFromV0ofTracks   );
      setBranch<std::vector<float> >(tree,  "IP3D_valD0wrtPVofTracks"  , m_IP3D_valD0wrtPVofTracks   );
      setBranch<std::vector<float> >(tree,  "IP3D_sigD0wrtPVofTracks"  , m_IP3D_sigD0wrtPVofTracks   );
      setBranch<std::vector<float> >(tree,  "IP3D_valZ0wrtPVofTracks"  , m_IP3D_valZ0wrtPVofTracks   );
      setBranch<std::vector<float> >(tree,  "IP3D_sigZ0wrtPVofTracks"  , m_IP3D_sigZ0wrtPVofTracks   );
      setBranch<std::vector<float> >(tree,  "IP3D_weightBofTracks"     , m_IP3D_weightBofTracks      );
      setBranch<std::vector<float> >(tree,  "IP3D_weightCofTracks"     , m_IP3D_weightCofTracks      );
      setBranch<std::vector<float> >(tree,  "IP3D_weightUofTracks"     , m_IP3D_weightUofTracks      );

    }

    if( m_infoSwitch.m_JVC ) {
      setBranch<double>(tree,"JetVertexCharge_discriminant",     m_JetVertexCharge_discriminant);
    }
  }

  if( m_infoSwitch.m_flavorTagHLT  ) {

    setBranch<float>(tree,"vtxOnlineValid",m_vtxOnlineValid);
    setBranch<float>(tree,"vtxHadDummy"   ,m_vtxHadDummy   );
    setBranch<float>(tree,"bs_online_vx"   ,m_bs_online_vx   );
    setBranch<float>(tree,"bs_online_vy"   ,m_bs_online_vy   );
    setBranch<float>(tree,"bs_online_vz"   ,m_bs_online_vz   );

    setBranch<float>(tree,"vtx_offline_x0"     ,m_vtx_offline_x0     );
    setBranch<float>(tree,"vtx_offline_y0"     ,m_vtx_offline_y0     );
    setBranch<float>(tree,"vtx_offline_z0"     ,m_vtx_offline_z0     );

    setBranch<float>(tree,"vtx_online_x0"      ,m_vtx_online_x0     );
    setBranch<float>(tree,"vtx_online_y0"      ,m_vtx_online_y0     );
    setBranch<float>(tree,"vtx_online_z0"      ,m_vtx_online_z0     );

    setBranch<float>(tree,"vtx_online_bkg_x0"  ,m_vtx_online_bkg_x0     );
    setBranch<float>(tree,"vtx_online_bkg_y0"  ,m_vtx_online_bkg_y0     );
    setBranch<float>(tree,"vtx_online_bkg_z0"  ,m_vtx_online_bkg_z0     );

  }

  if( !m_infoSwitch.m_jetBTag.empty() || !m_infoSwitch.m_jetBTagCts.empty() ) {
    for(auto btag : m_btags)
      btag->setBranch(tree, m_name);
  }

  if( m_infoSwitch.m_area ) {
    setBranch<float>(tree,"GhostArea",     m_GhostArea);
    setBranch<float>(tree,"ActiveArea",    m_ActiveArea);
    setBranch<float>(tree,"VoronoiArea",   m_VoronoiArea);
    setBranch<float>(tree,"ActiveArea4vec_pt", m_ActiveArea4vec_pt);
    setBranch<float>(tree,"ActiveArea4vec_eta", m_ActiveArea4vec_eta);
    setBranch<float>(tree,"ActiveArea4vec_phi", m_ActiveArea4vec_phi);
    setBranch<float>(tree,"ActiveArea4vec_m",   m_ActiveArea4vec_m);
  }

  if ( m_infoSwitch.m_truth && m_mc ) {
    setBranch<int  >(tree,"ConeTruthLabelID",   m_ConeTruthLabelID );
    setBranch<int  >(tree,"TruthCount",         m_TruthCount     );
    setBranch<float>(tree,"TruthLabelDeltaR_B", m_TruthLabelDeltaR_B     );
    setBranch<float>(tree,"TruthLabelDeltaR_C", m_TruthLabelDeltaR_C      );
    setBranch<float>(tree,"TruthLabelDeltaR_T", m_TruthLabelDeltaR_T      );
    setBranch<int  >(tree,"PartonTruthLabelID", m_PartonTruthLabelID  );
    setBranch<float>(tree,"GhostTruthAssociationFraction", m_GhostTruthAssociationFraction);
    setBranch<float>(tree,"truth_E",   m_truth_E);
    setBranch<float>(tree,"truth_pt",  m_truth_pt);
    setBranch<float>(tree,"truth_phi", m_truth_phi);
    setBranch<float>(tree,"truth_eta", m_truth_eta);
  }


  if ( m_infoSwitch.m_truthDetails ) {
    setBranch<int  >(tree,"GhostBHadronsFinalCount",   m_GhostBHadronsFinalCount   );
    setBranch<int  >(tree,"GhostBHadronsInitialCount", m_GhostBHadronsInitialCount );
    setBranch<int  >(tree,"GhostBQuarksFinalCount",    m_GhostBQuarksFinalCount    );
    setBranch<float>(tree,"GhostBHadronsFinalPt",      m_GhostBHadronsFinalPt      );
    setBranch<float>(tree,"GhostBHadronsInitialPt",    m_GhostBHadronsInitialPt    );
    setBranch<float>(tree,"GhostBQuarksFinalPt",       m_GhostBQuarksFinalPt       );

    setBranch<int  >(tree,"GhostCHadronsFinalCount"  , m_GhostCHadronsFinalCount  );
    setBranch<int  >(tree,"GhostCHadronsInitialCount", m_GhostCHadronsInitialCount);
    setBranch<int  >(tree,"GhostCQuarksFinalCount"   , m_GhostCQuarksFinalCount   );
    setBranch<float>(tree,"GhostCHadronsFinalPt"     , m_GhostCHadronsFinalPt     );
    setBranch<float>(tree,"GhostCHadronsInitialPt"   , m_GhostCHadronsInitialPt   );
    setBranch<float>(tree,"GhostCQuarksFinalPt"      , m_GhostCQuarksFinalPt      );

    setBranch<int  >(tree,"GhostTausFinalCount",       m_GhostTausFinalCount );
    setBranch<float>(tree,"GhostTausFinalPt"   ,       m_GhostTausFinalPt    );

    setBranch<int  >(tree,"truth_pdgId"   , m_truth_pdgId);
    setBranch<float>(tree,"truth_partonPt", m_truth_partonPt);
    setBranch<float>(tree,"truth_partonDR", m_truth_partonDR);
  }

  if ( m_infoSwitch.m_charge ) {
    setBranch<double>(tree,"charge", m_charge);
  }

  if ( m_infoSwitch.m_passSel ) {
    setBranch<char>(tree,"passSel", m_passSel);
  }

  return;
}


void JetContainer::clear()
{

  ParticleContainer::clear();

  // rapidity
  if( m_infoSwitch.m_rapidity ) {
    m_rapidity->clear();
  }
  
  // trigger
  if ( m_infoSwitch.m_trigger ) {
    m_isTrigMatched->clear();
    m_isTrigMatchedToChain->clear();
    m_listTrigChains->clear();
  }
  
  // clean
  if( m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight ) {
    if(m_infoSwitch.m_clean){
      m_Timing                    ->clear();
      m_LArQuality                ->clear();
      m_HECQuality                ->clear();
      m_NegativeE                 ->clear();
      m_AverageLArQF              ->clear();
      m_BchCorrCell               ->clear();
      m_N90Constituents           ->clear();
      m_LArBadHVEnergyFrac        ->clear();
      m_LArBadHVNCell             ->clear();
      m_OotFracClusters5          ->clear();
      m_OotFracClusters10         ->clear();
      m_LeadingClusterPt          ->clear();
      m_LeadingClusterSecondLambda->clear();
      m_LeadingClusterCenterLambda->clear();
      m_LeadingClusterSecondR     ->clear();
      if(m_infoSwitch.m_cleanTrig) {
        m_clean_passLooseBadTriggerUgly->clear();
      }
      else {
        m_clean_passTightBadUgly    ->clear();
        m_clean_passLooseBadUgly    ->clear();
      }
    }
    if(m_infoSwitch.m_cleanTrig) {
      m_clean_passLooseBadTrigger ->clear();
    }
    else {
      m_clean_passLooseBad        ->clear();
      m_clean_passTightBad        ->clear();
    }
  }


  // energy
  if ( m_infoSwitch.m_energy || m_infoSwitch.m_energyLight ) {
    if ( m_infoSwitch.m_energy ){
      m_HECFrac               ->clear();
      m_CentroidR             ->clear();
      m_LowEtConstituentsFrac ->clear();
    }
    m_EMFrac                ->clear();
    m_FracSamplingMax       ->clear();
    m_FracSamplingMaxIndex  ->clear();
    m_GhostMuonSegmentCount ->clear();
    m_Width                 ->clear();
  }

  // each step of the calibration sequence
  if ( m_infoSwitch.m_scales ) {
    m_emScalePt             ->clear();
    m_constScalePt	    ->clear();
    m_pileupScalePt	    ->clear();
    m_originConstitScalePt  ->clear();
    m_etaJESScalePt	    ->clear();
    m_gscScalePt	    ->clear();
    m_jmsScalePt            ->clear();
    m_insituScalePt	    ->clear();

    m_emScaleM             ->clear();
    m_constScaleM          ->clear();
    m_pileupScaleM         ->clear();
    m_originConstitScaleM  ->clear();
    m_etaJESScaleM         ->clear();
    m_gscScaleM            ->clear();
    m_jmsScaleM            ->clear();
    m_insituScaleM         ->clear();
  }

  // eta at constScale
  if ( m_infoSwitch.m_constscaleEta ) {
    m_constScaleEta	    ->clear();
  }

  // detector eta
  if ( m_infoSwitch.m_detectorEta ) {
    m_detectorEta           ->clear();
  }

  // layer
  if ( m_infoSwitch.m_layer ) {
    m_EnergyPerSampling->clear();
  }

  // trackAll
  if ( m_infoSwitch.m_trackAll ) {
    m_NumTrkPt1000             ->clear();
    m_SumPtTrkPt1000           ->clear();
    m_TrackWidthPt1000         ->clear();
    m_NumTrkPt500              ->clear();
    m_SumPtTrkPt500            ->clear();
    m_TrackWidthPt500          ->clear();
    m_JVF                      ->clear();
  }

  // trackPV
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_jvt ) {
    if ( m_infoSwitch.m_trackPV ) {
      m_NumTrkPt1000PV    ->clear();
      m_SumPtTrkPt1000PV  ->clear();
      m_TrackWidthPt1000PV->clear();
      m_NumTrkPt500PV     ->clear();
      m_SumPtTrkPt500PV   ->clear();
      m_TrackWidthPt500PV ->clear();
      m_JVFPV             ->clear();
      m_JvtJvfcorr        ->clear();
      m_JvtRpt            ->clear();
    }
    m_Jvt               ->clear();
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Loose" ) {
    m_JvtPass_Loose   ->clear();
    if ( m_mc ) {
      m_JvtEff_SF_Loose ->clear();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Medium" ) {
    m_JvtPass_Medium    ->clear();
    if ( m_mc ) {
      m_JvtEff_SF_Medium->clear();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Tight" ) {
    m_JvtPass_Tight     ->clear();
    if ( m_mc ) {
      m_JvtEff_SF_Tight ->clear();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Medium" ) {
    m_fJvtPass_Medium    ->clear();
    if ( m_mc ) {
      m_fJvtEff_SF_Medium->clear();
    }
  }
  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Tight" ) {
    m_fJvtPass_Tight     ->clear();
    if ( m_mc ) {
      m_fJvtEff_SF_Tight ->clear();
    }
  }

  if ( m_infoSwitch.m_allTrack ) {
    m_GhostTrackCount     ->clear();
    m_GhostTrackPt        ->clear();
    m_GhostTrack_pt       ->clear();
    m_GhostTrack_qOverP   ->clear();
    m_GhostTrack_eta      ->clear();
    m_GhostTrack_phi      ->clear();
    m_GhostTrack_e        ->clear();
    m_GhostTrack_d0       ->clear();
    m_GhostTrack_z0       ->clear();

    if ( m_infoSwitch.m_allTrackDetail ) {
      m_GhostTrack_nPixelHits                               ->clear();
      m_GhostTrack_nSCTHits                                 ->clear();
      m_GhostTrack_nTRTHits                                 ->clear();
      m_GhostTrack_nPixelSharedHits                         ->clear();
      m_GhostTrack_nPixelSplitHits                          ->clear();
      m_GhostTrack_nInnermostPixelLayerHits                 ->clear();
      m_GhostTrack_nInnermostPixelLayerSharedHits           ->clear();
      m_GhostTrack_nInnermostPixelLayerSplitHits            ->clear();
      m_GhostTrack_nNextToInnermostPixelLayerHits           ->clear();
      m_GhostTrack_nNextToInnermostPixelLayerSharedHits     ->clear();
      m_GhostTrack_nNextToInnermostPixelLayerSplitHits      ->clear();
    }
  }

  if( m_infoSwitch.m_constituent ) {
    m_numConstituents->clear();
  }

  if( m_infoSwitch.m_constituentAll ) {
    m_constituentWeights ->clear();
    m_constituent_pt     ->clear();
    m_constituent_eta    ->clear();
    m_constituent_phi    ->clear();
    m_constituent_e      ->clear();
  }

  // flavor tag
  if ( m_infoSwitch.m_flavorTag || m_infoSwitch.m_flavorTagHLT  ) {

    m_MV2c00                            ->clear();
    m_MV2c10                            ->clear();
    m_MV2c10mu                          ->clear();
    m_MV2c10rnn                         ->clear();
    m_MV2rmu                            ->clear();
    m_MV2r                              ->clear();
    m_MV2c20                            ->clear();
    m_MV2c100                           ->clear();
    m_DL1                               ->clear();
    m_DL1_pu                            ->clear();
    m_DL1_pc                            ->clear();
    m_DL1_pb                            ->clear();
    m_DL1mu                             ->clear();
    m_DL1mu_pu                          ->clear();
    m_DL1mu_pc                          ->clear();
    m_DL1mu_pb                          ->clear();
    m_DL1rnn                            ->clear();
    m_DL1rnn_pu                         ->clear();
    m_DL1rnn_pc                         ->clear();
    m_DL1rnn_pb                         ->clear();
    m_DL1rmu                            ->clear();
    m_DL1rmu_pu                         ->clear();
    m_DL1rmu_pc                         ->clear();
    m_DL1rmu_pb                         ->clear();
    m_DL1r                              ->clear();
    m_DL1r_pu                           ->clear();
    m_DL1r_pc                           ->clear();
    m_DL1r_pb                           ->clear();
    m_HadronConeExclTruthLabelID        ->clear();
    m_HadronConeExclExtendedTruthLabelID->clear();


    if( m_infoSwitch.m_jetFitterDetails){
      m_JetFitter_nVTX             ->clear();
      m_JetFitter_nSingleTracks    ->clear();
      m_JetFitter_nTracksAtVtx     ->clear();
      m_JetFitter_mass             ->clear();
      m_JetFitter_energyFraction   ->clear();
      m_JetFitter_significance3d   ->clear();
      m_JetFitter_deltaeta         ->clear();
      m_JetFitter_deltaphi         ->clear();
      m_JetFitter_N2Tpar           ->clear();
    }

    if( m_infoSwitch.m_svDetails){
      m_SV0               ->clear();
      m_sv0_NGTinSvx      ->clear();
      m_sv0_N2Tpair       ->clear();
      m_sv0_massvx        ->clear();
      m_sv0_efracsvx      ->clear();
      m_sv0_normdist      ->clear();

      m_SV1               ->clear();
      m_SV1IP3D           ->clear();
      m_COMBx             ->clear();
      m_sv1_pu            ->clear();
      m_sv1_pb            ->clear();
      m_sv1_pc            ->clear();
      m_sv1_c             ->clear();
      m_sv1_cu            ->clear();
      m_sv1_NGTinSvx      ->clear();
      m_sv1_N2Tpair       ->clear();
      m_sv1_massvx        ->clear();
      m_sv1_efracsvx      ->clear();
      m_sv1_normdist      ->clear();
      m_sv1_Lxy           ->clear();
      m_sv1_sig3d         ->clear();
      m_sv1_L3d           ->clear();
      m_sv1_distmatlay    ->clear();
      m_sv1_dR            ->clear();
    }

    if( m_infoSwitch.m_ipDetails){
      m_IP2D_pu        ->clear();
      m_IP2D_pb        ->clear();
      m_IP2D_pc        ->clear();
      m_IP2D           ->clear();
      m_IP2D_c         ->clear();
      m_IP2D_cu        ->clear();
      m_nIP2DTracks    ->clear();

      m_IP2D_gradeOfTracks              ->clear();
      m_IP2D_flagFromV0ofTracks         ->clear();
      m_IP2D_valD0wrtPVofTracks         ->clear();
      m_IP2D_sigD0wrtPVofTracks         ->clear();
      m_IP2D_weightBofTracks            ->clear();
      m_IP2D_weightCofTracks            ->clear();
      m_IP2D_weightUofTracks            ->clear();

      m_IP3D         ->clear();
      m_IP3D_pu      ->clear();
      m_IP3D_pb      ->clear();
      m_IP3D_pc      ->clear();
      m_IP3D         ->clear();
      m_IP3D_c       ->clear();
      m_IP3D_cu      ->clear();
      m_nIP3DTracks  ->clear();
      m_IP3D_gradeOfTracks        ->clear();
      m_IP3D_flagFromV0ofTracks   ->clear();
      m_IP3D_valD0wrtPVofTracks   ->clear();
      m_IP3D_sigD0wrtPVofTracks   ->clear();
      m_IP3D_valZ0wrtPVofTracks   ->clear();
      m_IP3D_sigZ0wrtPVofTracks   ->clear();
      m_IP3D_weightBofTracks      ->clear();
      m_IP3D_weightCofTracks      ->clear();
      m_IP3D_weightUofTracks      ->clear();
    }

    if( m_infoSwitch.m_JVC ){
      m_JetVertexCharge_discriminant->clear();
    }
  }

  if ( m_infoSwitch.m_flavorTagHLT  ) {
    m_vtxOnlineValid->clear();
    m_vtxHadDummy->clear();
    m_bs_online_vx->clear();
    m_bs_online_vy->clear();
    m_bs_online_vz->clear();

    m_vtx_offline_x0->clear();
    m_vtx_offline_y0->clear();
    m_vtx_offline_z0->clear();

    m_vtx_online_x0->clear();
    m_vtx_online_y0->clear();
    m_vtx_online_z0->clear();

    m_vtx_online_bkg_x0->clear();
    m_vtx_online_bkg_y0->clear();
    m_vtx_online_bkg_z0->clear();

  }


  if( !m_infoSwitch.m_jetBTag.empty() || !m_infoSwitch.m_jetBTagCts.empty()) { // just clear them all....
    for(auto btag : m_btags)
      btag->clear();
  }

  if ( m_infoSwitch.m_area ) {
    m_GhostArea          ->clear();
    m_ActiveArea         ->clear();
    m_VoronoiArea        ->clear();
    m_ActiveArea4vec_pt  ->clear();
    m_ActiveArea4vec_eta ->clear();
    m_ActiveArea4vec_phi ->clear();
    m_ActiveArea4vec_m   ->clear();
  }

  // truth
  if ( m_infoSwitch.m_truth && m_mc ) {
    m_ConeTruthLabelID  ->clear();
    m_TruthCount        ->clear();
    m_TruthLabelDeltaR_B->clear();
    m_TruthLabelDeltaR_C->clear();
    m_TruthLabelDeltaR_T->clear();
    m_PartonTruthLabelID->clear();
    m_GhostTruthAssociationFraction->clear();
    m_truth_E  ->clear();
    m_truth_pt ->clear();
    m_truth_phi->clear();
    m_truth_eta->clear();
  }

  // truth_detail
  if ( m_infoSwitch.m_truthDetails ) {
    m_GhostBHadronsFinalCount       ->clear();
    m_GhostBHadronsInitialCount     ->clear();
    m_GhostBQuarksFinalCount        ->clear();
    m_GhostBHadronsFinalPt          ->clear();
    m_GhostBHadronsInitialPt        ->clear();
    m_GhostBQuarksFinalPt           ->clear();

    m_GhostCHadronsFinalCount       ->clear();
    m_GhostCHadronsInitialCount     ->clear();
    m_GhostCQuarksFinalCount        ->clear();
    m_GhostCHadronsFinalPt          ->clear();
    m_GhostCHadronsInitialPt        ->clear();
    m_GhostCQuarksFinalPt           ->clear();

    m_GhostTausFinalCount           ->clear();
    m_GhostTausFinalPt              ->clear();

    m_truth_pdgId                   ->clear();
    m_truth_partonPt                ->clear();
    m_truth_partonDR                ->clear();

  }

  if( m_infoSwitch.m_charge ) {
    m_charge->clear();
  }

  if( m_infoSwitch.m_passSel ) {
    m_passSel->clear();
  }

  return;
}

void JetContainer::FillJet( const xAOD::Jet* jet, const xAOD::Vertex* pv, int pvLocation ){
  return FillJet(static_cast<const xAOD::IParticle*>(jet), pv, pvLocation);
}

void JetContainer::FillJet( const xAOD::IParticle* particle, const xAOD::Vertex* pv, int pvLocation ){
  if(m_debug) std::cout << "In JetContainer::FillJet " << std::endl;

  ParticleContainer::FillParticle(particle);

  const xAOD::Jet* jet=dynamic_cast<const xAOD::Jet*>(particle);

  if( m_infoSwitch.m_rapidity ){
    m_rapidity->push_back( jet->rapidity() );
  }

  if ( m_infoSwitch.m_trigger ) {

    // retrieve map<string,char> w/ <chain,isMatched>
    //
    static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapJetAcc("isTrigMatchedMapJet");

    std::vector<int> matches;

    if ( isTrigMatchedMapJetAcc.isAvailable( *jet ) ) {
      // loop over map and fill branches
      //
      for ( auto const &it : (isTrigMatchedMapJetAcc( *jet )) ) {
	matches.push_back( static_cast<int>(it.second) );
	m_listTrigChains->push_back( it.first );
      }
    } else {
      matches.push_back( -1 );
      m_listTrigChains->push_back("NONE");
    }

    m_isTrigMatchedToChain->push_back(matches);
    
    // if at least one match among the chains is found, say this jet is trigger matched
    if ( std::find(matches.begin(), matches.end(), 1) != matches.end() ) { m_isTrigMatched->push_back(1); }
    else { m_isTrigMatched->push_back(0); }
    
  }

  if (m_infoSwitch.m_clean || m_infoSwitch.m_cleanLight) {

    if(m_infoSwitch.m_clean){

      static SG::AuxElement::ConstAccessor<float> jetTime ("Timing");
      safeFill<float, float, xAOD::Jet>(jet, jetTime, m_Timing, -999);

      static SG::AuxElement::ConstAccessor<float> LArQuality ("LArQuality");
      safeFill<float, float, xAOD::Jet>(jet, LArQuality, m_LArQuality, -999);

      static SG::AuxElement::ConstAccessor<float> hecq ("HECQuality");
      safeFill<float, float, xAOD::Jet>(jet, hecq, m_HECQuality, -999);

      static SG::AuxElement::ConstAccessor<float> negE ("NegativeE");
      safeFill<float, float, xAOD::Jet>(jet, negE, m_NegativeE, -999, m_units);

      static SG::AuxElement::ConstAccessor<float> avLArQF ("AverageLArQF");
      safeFill<float, float, xAOD::Jet>(jet, avLArQF, m_AverageLArQF, -999);

      static SG::AuxElement::ConstAccessor<float> bchCorrCell ("BchCorrCell");
      safeFill<float, float, xAOD::Jet>(jet, bchCorrCell, m_BchCorrCell, -999);

      static SG::AuxElement::ConstAccessor<float> N90Const ("N90Constituents");
      safeFill<float, float, xAOD::Jet>(jet, N90Const, m_N90Constituents, -999);

      static SG::AuxElement::ConstAccessor<float> LArBadHVEFrac ("LArBadHVEnergyFrac");
      safeFill<float, float, xAOD::Jet>(jet, LArBadHVEFrac, m_LArBadHVEnergyFrac, -999);

      static SG::AuxElement::ConstAccessor<int> LArBadHVNCell ("LArBadHVNCell");
      safeFill<int, int, xAOD::Jet>(jet, LArBadHVNCell, m_LArBadHVNCell, -999);

      static SG::AuxElement::ConstAccessor<float> OotFracClus5 ("OotFracClusters5");
      safeFill<float, float, xAOD::Jet>(jet, OotFracClus5, m_OotFracClusters5, -999);

      static SG::AuxElement::ConstAccessor<float> OotFracClus10 ("OotFracClusters10");
      safeFill<float, float, xAOD::Jet>(jet, OotFracClus10, m_OotFracClusters10, -999);

      static SG::AuxElement::ConstAccessor<float> leadClusPt ("LeadingClusterPt");
      safeFill<float, float, xAOD::Jet>(jet, leadClusPt, m_LeadingClusterPt, -999);

      static SG::AuxElement::ConstAccessor<float> leadClusSecondLambda ("LeadingClusterSecondLambda");
      safeFill<float, float, xAOD::Jet>(jet, leadClusSecondLambda, m_LeadingClusterSecondLambda, -999);

      static SG::AuxElement::ConstAccessor<float> leadClusCenterLambda ("LeadingClusterCenterLambda");
      safeFill<float, float, xAOD::Jet>(jet, leadClusCenterLambda, m_LeadingClusterCenterLambda, -999);

      static SG::AuxElement::ConstAccessor<float> leadClusSecondR ("LeadingClusterSecondR");
      safeFill<float, float, xAOD::Jet>(jet, leadClusSecondR, m_LeadingClusterSecondR, -999);

      if(!m_infoSwitch.m_cleanTrig) {
        static SG::AuxElement::ConstAccessor<int> clean_passLooseBadUgly ("clean_passLooseBadUgly");
        safeFill<int, int, xAOD::Jet>(jet, clean_passLooseBadUgly, m_clean_passLooseBadUgly, -999);
        
        static SG::AuxElement::ConstAccessor<int> clean_passTightBadUgly ("clean_passTightBadUgly");
        safeFill<int, int, xAOD::Jet>(jet, clean_passTightBadUgly, m_clean_passTightBadUgly, -999);
      }
      else {
        static SG::AuxElement::ConstAccessor<int> clean_passLooseBadTriggerUgly ("clean_passLooseBadTriggerUgly");
        safeFill<int, int, xAOD::Jet>(jet, clean_passLooseBadTriggerUgly, m_clean_passLooseBadTriggerUgly, -999);
      }
      
    }

    if(!m_infoSwitch.m_cleanTrig) {
      static SG::AuxElement::ConstAccessor<int> clean_passLooseBad ("clean_passLooseBad");
      safeFill<int, int, xAOD::Jet>(jet, clean_passLooseBad, m_clean_passLooseBad, -999);

      static SG::AuxElement::ConstAccessor<int> clean_passTightBad ("clean_passTightBad");
      safeFill<int, int, xAOD::Jet>(jet, clean_passTightBad, m_clean_passTightBad, -999);
    }
    else {
      static SG::AuxElement::ConstAccessor<int> clean_passLooseBadTrigger ("clean_passLooseBadTrigger");
      safeFill<int, int, xAOD::Jet>(jet, clean_passLooseBadTrigger, m_clean_passLooseBadTrigger, -999);
    }

  } // clean


  if ( m_infoSwitch.m_energy | m_infoSwitch.m_energyLight ) {

    if ( m_infoSwitch.m_energy ){

      static SG::AuxElement::ConstAccessor<float> HECf ("HECFrac");
      safeFill<float, float, xAOD::Jet>(jet, HECf, m_HECFrac, -999);

      static SG::AuxElement::ConstAccessor<float> centroidR ("CentroidR");
      safeFill<float, float, xAOD::Jet>(jet, centroidR, m_CentroidR, -999);

      static SG::AuxElement::ConstAccessor<float> lowEtFrac ("LowEtConstituentsFrac");
      safeFill<float, float, xAOD::Jet>(jet, lowEtFrac, m_LowEtConstituentsFrac, -999);

    }

    static SG::AuxElement::ConstAccessor<float> EMf ("EMFrac");
    safeFill<float, float, xAOD::Jet>(jet, EMf, m_EMFrac, -999);

    static SG::AuxElement::ConstAccessor<float> fracSampMax ("FracSamplingMax");
    safeFill<float, float, xAOD::Jet>(jet, fracSampMax, m_FracSamplingMax, -999);

    static SG::AuxElement::ConstAccessor<int> fracSampMaxIdx ("FracSamplingMaxIndex");
    safeFill<int, float, xAOD::Jet>(jet, fracSampMaxIdx, m_FracSamplingMaxIndex, -999);

    static SG::AuxElement::ConstAccessor<int> muonSegCount ("GhostMuonSegmentCount");
    safeFill<int, float, xAOD::Jet>(jet, muonSegCount, m_GhostMuonSegmentCount, -999);

    static SG::AuxElement::ConstAccessor<float> width ("Width");
    safeFill<float, float, xAOD::Jet>(jet, width, m_Width, -999);

  } // energy

  // each step of the calibration sequence
  if ( m_infoSwitch.m_scales ) {
    xAOD::JetFourMom_t fourVec;
    bool status(false);
    // EM Scale
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetEMScaleMomentum", fourVec );
    if( status ) { 
      m_emScalePt->push_back( fourVec.Pt() / m_units );
      m_emScaleM->push_back( fourVec.M() / m_units );
    }
    else { 
      m_emScalePt->push_back( -999 ); 
      m_emScaleM->push_back( -999 ); 
    }
    // Constit Scale
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetConstitScaleMomentum", fourVec );
    if( status ) { 
      m_constScalePt->push_back( fourVec.Pt() / m_units ); 
      m_constScaleM->push_back( fourVec.M() / m_units ); 
    }
    else { 
      m_constScalePt->push_back( -999 ); 
      m_constScaleM->push_back( -999 ); 
    }
    // Pileup Scale
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetPileupScaleMomentum", fourVec );
    if( status ) { 
      m_pileupScalePt->push_back( fourVec.Pt() / m_units ); 
      m_pileupScaleM->push_back( fourVec.M() / m_units ); 
    }
    else { 
      m_pileupScalePt->push_back( -999 ); 
      m_pileupScaleM->push_back( -999 ); 
    }
    // OriginConstit Scale
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetOriginConstitScaleMomentum", fourVec );
    if( status ) {
      m_originConstitScalePt->push_back( fourVec.Pt() / m_units ); 
      m_originConstitScaleM->push_back( fourVec.M() / m_units ); 
    }
    else { 
      m_originConstitScalePt->push_back( -999 ); 
      m_originConstitScaleM->push_back( -999 ); 
    }
    // EtaJES Scale
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetEtaJESScaleMomentum", fourVec );
    if( status ) { 
      m_etaJESScalePt->push_back( fourVec.Pt() / m_units );
      m_etaJESScaleM->push_back( fourVec.M() / m_units );
    }
    else { 
      m_etaJESScalePt->push_back( -999 ); 
      m_etaJESScaleM->push_back( -999 ); 
    }
    // GSC Scale
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetGSCScaleMomentum", fourVec );
    if( status ) { 
      m_gscScalePt->push_back( fourVec.Pt() / m_units ); 
      m_gscScaleM->push_back( fourVec.M() / m_units ); 
    }
    else {
      m_gscScalePt->push_back( -999 ); 
      m_gscScaleM->push_back( -999 ); 
    }
    // EtaJES Scale
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetJMSScaleMomentum", fourVec );
    if( status ) {
      m_jmsScalePt->push_back( fourVec.Pt() / m_units );
      m_jmsScaleM->push_back( fourVec.M() / m_units );
    }
    else {
      m_jmsScalePt->push_back( -999 );
      m_jmsScaleM->push_back( -999 );
    }
    // only available in data
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetInsituScaleMomentum", fourVec );
    if(status) { 
      m_insituScalePt->push_back( fourVec.Pt() / m_units ); 
      m_insituScaleM->push_back( fourVec.M() / m_units ); 
    }
    else { 
      m_insituScalePt->push_back( -999 ); 
      m_insituScaleM->push_back( -999 ); 
    }
  }

  if ( m_infoSwitch.m_constscaleEta ) {
    xAOD::JetFourMom_t fourVec;
    bool status(false);
    status = jet->getAttribute<xAOD::JetFourMom_t>( "JetConstitScaleMomentum", fourVec );
    if( status ) { m_constScaleEta->push_back( fourVec.Eta() ); }
    else { m_constScaleEta->push_back( -999 ); }
  }

  if ( m_infoSwitch.m_detectorEta ) {
    static SG::AuxElement::ConstAccessor<float> DetEta ("DetectorEta");
    safeFill<float, float, xAOD::Jet>(jet, DetEta, m_detectorEta, -999);
  }

  if ( m_infoSwitch.m_layer ) {
    static SG::AuxElement::ConstAccessor< std::vector<float> > ePerSamp ("EnergyPerSampling");
    if ( ePerSamp.isAvailable( *jet ) ) {
      m_EnergyPerSampling->push_back( ePerSamp( *jet ) );
      m_EnergyPerSampling->back();
      std::transform((m_EnergyPerSampling->back()).begin(),
                     (m_EnergyPerSampling->back()).end(),
                     (m_EnergyPerSampling->back()).begin(),
                     std::bind2nd(std::divides<float>(), m_units));
    } else {
      // could push back a vector of 24...
      // ... waste of space vs prevention of out of range down stream
      std::vector<float> junk(1,-999);
      m_EnergyPerSampling->push_back( junk );
    }
  }

  if ( m_infoSwitch.m_trackAll || m_infoSwitch.m_trackPV || m_infoSwitch.m_jvt ) {

    // several moments calculated from all verticies
    // one accessor for each and just use appropiately in the following
    static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk1000("NumTrkPt1000");
    static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt1000("SumPtTrkPt1000");
    static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth1000("TrackWidthPt1000");
    static SG::AuxElement::ConstAccessor< std::vector<int> >   nTrk500 ("NumTrkPt500");
    static SG::AuxElement::ConstAccessor< std::vector<float> > sumPt500 ("SumPtTrkPt500");
    static SG::AuxElement::ConstAccessor< std::vector<float> > trkWidth500 ("TrackWidthPt500");
    static SG::AuxElement::ConstAccessor< std::vector<float> > jvf("JVF");

    if ( m_infoSwitch.m_trackAll ) {

      std::vector<int> junkInt(1,-999);
      std::vector<float> junkFlt(1,-999);

      if ( nTrk1000.isAvailable( *jet ) ) {
        m_NumTrkPt1000->push_back( nTrk1000( *jet ) );
      } else { m_NumTrkPt1000->push_back( junkInt ); }

      if ( sumPt1000.isAvailable( *jet ) ) {
        m_SumPtTrkPt1000->push_back( sumPt1000( *jet ) );
        std::transform((m_SumPtTrkPt1000->back()).begin(),
                     (m_SumPtTrkPt1000->back()).end(),
                     (m_SumPtTrkPt1000->back()).begin(),
                     std::bind2nd(std::divides<float>(), m_units));
      } else { m_SumPtTrkPt1000->push_back( junkFlt ); }

      if ( trkWidth1000.isAvailable( *jet ) ) {
        m_TrackWidthPt1000->push_back( trkWidth1000( *jet ) );
      } else { m_TrackWidthPt1000->push_back( junkFlt ); }

      if ( nTrk500.isAvailable( *jet ) ) {
        m_NumTrkPt500->push_back( nTrk500( *jet ) );
      } else { m_NumTrkPt500->push_back( junkInt ); }

      if ( sumPt500.isAvailable( *jet ) ) {
        m_SumPtTrkPt500->push_back( sumPt500( *jet ) );
        std::transform((m_SumPtTrkPt500->back()).begin(),
                     (m_SumPtTrkPt500->back()).end(),
                     (m_SumPtTrkPt500->back()).begin(),
                     std::bind2nd(std::divides<float>(), m_units));
      } else { m_SumPtTrkPt500->push_back( junkFlt ); }

      if ( trkWidth500.isAvailable( *jet ) ) {
        m_TrackWidthPt500->push_back( trkWidth500( *jet ) );
      } else { m_TrackWidthPt500->push_back( junkFlt ); }

      if ( jvf.isAvailable( *jet ) ) {
        m_JVF->push_back( jvf( *jet ) );
      } else { m_JVF->push_back( junkFlt ); }

    } // trackAll

    if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_jvt ) {

      if ( m_infoSwitch.m_trackPV && pvLocation >= 0 ) {

        if ( nTrk1000.isAvailable( *jet ) ) {
          m_NumTrkPt1000PV->push_back( nTrk1000( *jet )[pvLocation] );
        } else { m_NumTrkPt1000PV->push_back( -999 ); }

        if ( sumPt1000.isAvailable( *jet ) ) {
          m_SumPtTrkPt1000PV->push_back( sumPt1000( *jet )[pvLocation] / m_units );
        } else { m_SumPtTrkPt1000PV->push_back( -999 ); }

        if ( trkWidth1000.isAvailable( *jet ) ) {
          m_TrackWidthPt1000PV->push_back( trkWidth1000( *jet )[pvLocation] );
        } else { m_TrackWidthPt1000PV->push_back( -999 ); }

        if ( nTrk500.isAvailable( *jet ) ) {
          m_NumTrkPt500PV->push_back( nTrk500( *jet )[pvLocation] );
        } else { m_NumTrkPt500PV->push_back( -999 ); }

        if ( sumPt500.isAvailable( *jet ) ) {
          m_SumPtTrkPt500PV->push_back( sumPt500( *jet )[pvLocation] / m_units );
        } else { m_SumPtTrkPt500PV->push_back( -999 ); }

        if ( trkWidth500.isAvailable( *jet ) ) {
          m_TrackWidthPt500PV->push_back( trkWidth500( *jet )[pvLocation] );
        } else { m_TrackWidthPt500PV->push_back( -999 ); }

        if ( jvf.isAvailable( *jet ) ) {
          m_JVFPV->push_back( jvf( *jet )[pvLocation] );
        } else { m_JVFPV->push_back( -999 ); }

        static SG::AuxElement::ConstAccessor< float > jvtJvfcorr ("JvtJvfcorr");
        safeFill<float, float, xAOD::Jet>(jet, jvtJvfcorr, m_JvtJvfcorr, -999);

        static SG::AuxElement::ConstAccessor< float > jvtRpt ("JvtRpt");
        safeFill<float, float, xAOD::Jet>(jet, jvtRpt, m_JvtRpt, -999);

      } // trackPV

      static SG::AuxElement::ConstAccessor< float > jvt ("Jvt");
      safeFill<float, float, xAOD::Jet>(jet, jvt, m_Jvt, -999);

      //      static SG::AuxElement::ConstAccessor<float> ghostTrackAssFrac("GhostTrackAssociationFraction");
      //      if ( ghostTrackAssFrac.isAvailable( *jet) ) {
      //        m_ghostTrackAssFrac->push_back( ghostTrackAssFrac( *jet) );
      //      } else { m_ghostTrackAssFrac->push_back( -999 ) ; }

    } // trackPV || JVT

  } // trackAll || trackPV || JVT

  static SG::AuxElement::ConstAccessor< char > jvtPass_Loose("JetJVT_Passed_Loose");
  static SG::AuxElement::ConstAccessor< char > jvtPass_Medium("JetJVT_Passed_Medium");
  static SG::AuxElement::ConstAccessor< char > jvtPass_Tight("JetJVT_Passed_Tight");
  static SG::AuxElement::ConstAccessor< char > fjvtPass_Medium("JetfJVT_Passed_Medium");
  static SG::AuxElement::ConstAccessor< char > fjvtPass_Tight("JetfJVT_Passed_Tight");
  static SG::AuxElement::ConstAccessor< std::vector< float > > jvtSF_Loose("JetJvtEfficiency_JVTSyst_JVT_Loose");
  static SG::AuxElement::ConstAccessor< std::vector< float > > jvtSF_Medium("JetJvtEfficiency_JVTSyst_JVT_Medium");
  static SG::AuxElement::ConstAccessor< std::vector< float > > jvtSF_Tight("JetJvtEfficiency_JVTSyst_JVT_Tight");
  static SG::AuxElement::ConstAccessor< std::vector< float > > fjvtSF_Medium("JetJvtEfficiency_fJVTSyst_fJVT_Medium");
  static SG::AuxElement::ConstAccessor< std::vector< float > > fjvtSF_Tight("JetJvtEfficiency_fJVTSyst_fJVT_Tight");

  std::vector<float> junkSF(1,1.0);

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Loose" ) {
    safeFill<char, int, xAOD::Jet>(jet, jvtPass_Loose, m_JvtPass_Loose, -1);
    if ( m_mc ) {
      if ( jvtSF_Loose.isAvailable( *jet ) ) {
        m_JvtEff_SF_Loose->push_back( jvtSF_Loose( *jet ) );
      } else {
        m_JvtEff_SF_Loose->push_back( junkSF );
      }
    }
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Medium" ) {
    safeFill<char, int, xAOD::Jet>(jet, jvtPass_Medium, m_JvtPass_Medium, -1);
    if ( m_mc ) {
      if ( jvtSF_Medium.isAvailable( *jet ) ) {
        m_JvtEff_SF_Medium->push_back( jvtSF_Medium( *jet ) );
      } else {
        m_JvtEff_SF_Medium->push_back( junkSF );
      }
    }
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sfJVTName == "Tight" ) {
    safeFill<char, int, xAOD::Jet>(jet, jvtPass_Tight, m_JvtPass_Tight, -1);
    if ( m_mc ) {
      if ( jvtSF_Tight.isAvailable( *jet ) ) {
        m_JvtEff_SF_Tight->push_back( jvtSF_Tight( *jet ) );
      } else {
        m_JvtEff_SF_Tight->push_back( junkSF );
      }
    }
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Medium" ) {
    safeFill<char, int, xAOD::Jet>(jet, fjvtPass_Medium, m_fJvtPass_Medium, -1);
    if ( m_mc ) {
      if ( fjvtSF_Medium.isAvailable( *jet ) ) {
        m_fJvtEff_SF_Medium->push_back( fjvtSF_Medium( *jet ) );
      } else {
        m_fJvtEff_SF_Medium->push_back( junkSF );
      }
    }
  }

  if ( m_infoSwitch.m_trackPV || m_infoSwitch.m_sffJVTName == "Tight" ) {
    safeFill<char, int, xAOD::Jet>(jet, fjvtPass_Tight, m_fJvtPass_Tight, -1);
    if ( m_mc ) {
      if ( fjvtSF_Tight.isAvailable( *jet ) ) {
        m_fJvtEff_SF_Tight->push_back( fjvtSF_Tight( *jet ) );
      } else {
        m_fJvtEff_SF_Tight->push_back( junkSF );
      }
    }
  }

  if ( m_infoSwitch.m_allTrack ) {
    static SG::AuxElement::ConstAccessor< int > ghostTrackCount("GhostTrackCount");
    safeFill<int, int, xAOD::Jet>(jet, ghostTrackCount, m_GhostTrackCount, -999);

    static SG::AuxElement::ConstAccessor< float > ghostTrackPt ("GhostTrackPt");
    safeFill<float, float, xAOD::Jet>(jet, ghostTrackPt, m_GhostTrackPt, -999, m_units);

    std::vector<float> pt;
    std::vector<float> qOverP;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<float> e;
    std::vector<float> d0;
    std::vector<float> z0;
    std::vector<int> nPixHits;
    std::vector<int> nSCTHits;
    std::vector<int> nTRTHits;
    std::vector<int> nPixSharedHits;
    std::vector<int> nPixSplitHits;
    std::vector<int> nIMLPixHits;
    std::vector<int> nIMLPixSharedHits;
    std::vector<int> nIMLPixSplitHits;
    std::vector<int> nNIMLPixHits;
    std::vector<int> nNIMLPixSharedHits;
    std::vector<int> nNIMLPixSplitHits;
    static SG::AuxElement::ConstAccessor< std::vector<ElementLink<DataVector<xAOD::IParticle> > > >ghostTrack ("GhostTrack");
    if ( ghostTrack.isAvailable( *jet ) ) {
      std::vector<ElementLink<DataVector<xAOD::IParticle> > > trackLinks = ghostTrack( *jet );
      //std::vector<float> pt(trackLinks.size(),-999);
      for ( auto link_itr : trackLinks ) {
        if( !link_itr.isValid() ) { continue; }
        const xAOD::TrackParticle* track = dynamic_cast<const xAOD::TrackParticle*>( *link_itr );
        // if asking for tracks passing PV selection ( i.e. JVF JVT tracks )
        if( m_infoSwitch.m_allTrackPVSel ) {
          // PV selection from
          // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JvtManualRecalculation
          if( track->pt() < 500 )                { continue; } // pT cut
          if( !m_trkSelTool->accept(*track,pv) ) { continue; } // ID quality cut
          if( track->vertex() != pv ) {                        // if not in PV vertex fit
            if( track->vertex() != 0 )           { continue; } // make sure in no vertex fits
            if( fabs((track->z0()+track->vz()-pv->z())*sin(track->theta())) > 3.0 ) { continue; } // make sure close to PV in z
          }
        }
        pt. push_back( track->pt() / m_units );
        qOverP.push_back( track->qOverP() * m_units );
        eta.push_back( track->eta() );
        phi.push_back( track->phi() );
        e.  push_back( track->e()  / m_units );
        d0. push_back( track->d0() );
        z0. push_back( track->z0() + track->vz() - pv->z() ); // store z0 wrt PV...most useful
        if( m_infoSwitch.m_allTrackDetail ) {
          uint8_t getInt(0);
          // n pix, sct, trt
          track->summaryValue( getInt, xAOD::numberOfPixelHits );
          nPixHits.push_back( getInt );
          track->summaryValue( getInt, xAOD::numberOfSCTHits );
          nSCTHits.push_back( getInt );
          track->summaryValue( getInt, xAOD::numberOfTRTHits );
          nTRTHits.push_back( getInt );
          // pixel split shared
          track->summaryValue( getInt, xAOD::numberOfPixelSharedHits );
          nPixSharedHits.push_back( getInt );
          track->summaryValue( getInt, xAOD::numberOfPixelSplitHits );
          nPixSplitHits.push_back( getInt );
          // n ibl, split, shared
          track->summaryValue( getInt, xAOD::numberOfInnermostPixelLayerHits );
          nIMLPixHits.push_back( getInt );
          track->summaryValue( getInt, xAOD::numberOfInnermostPixelLayerSharedHits );
          nIMLPixSharedHits.push_back( getInt );
          track->summaryValue( getInt, xAOD::numberOfInnermostPixelLayerSplitHits );
          nIMLPixSplitHits.push_back( getInt );
          // n bl,  split, shared
          track->summaryValue( getInt, xAOD::numberOfNextToInnermostPixelLayerHits );
          nNIMLPixHits.push_back( getInt );
          track->summaryValue( getInt, xAOD::numberOfNextToInnermostPixelLayerSharedHits );
          nNIMLPixSharedHits.push_back( getInt );
          track->summaryValue( getInt, xAOD::numberOfNextToInnermostPixelLayerSplitHits );
          nNIMLPixSplitHits.push_back( getInt );
        }
      }
    } // if ghostTrack available
    m_GhostTrack_pt-> push_back( pt  );
    m_GhostTrack_qOverP-> push_back( qOverP );
    m_GhostTrack_eta->push_back( eta );
    m_GhostTrack_phi->push_back( phi );
    m_GhostTrack_e->  push_back( e   );
    m_GhostTrack_d0-> push_back( d0  );
    m_GhostTrack_z0-> push_back( z0  );
    if( m_infoSwitch.m_allTrackDetail ) {
      m_GhostTrack_nPixelHits->push_back( nPixHits );
      m_GhostTrack_nSCTHits->push_back( nSCTHits );
      m_GhostTrack_nTRTHits->push_back( nTRTHits );
      m_GhostTrack_nPixelSharedHits->push_back( nPixSharedHits );
      m_GhostTrack_nPixelSplitHits->push_back( nPixSplitHits );
      m_GhostTrack_nInnermostPixelLayerHits->push_back( nIMLPixHits );
      m_GhostTrack_nInnermostPixelLayerSharedHits->push_back( nIMLPixSharedHits );
      m_GhostTrack_nInnermostPixelLayerSplitHits->push_back( nIMLPixSplitHits );
      m_GhostTrack_nNextToInnermostPixelLayerHits->push_back( nNIMLPixHits );
      m_GhostTrack_nNextToInnermostPixelLayerSharedHits->push_back( nNIMLPixSharedHits );
      m_GhostTrack_nNextToInnermostPixelLayerSplitHits->push_back( nNIMLPixSplitHits );
    }
  } // allTrack switch

  if( m_infoSwitch.m_constituent ) {
    m_numConstituents->push_back( jet->numConstituents() );
  }

  if( m_infoSwitch.m_constituentAll ) {
    m_constituentWeights->push_back( jet->getAttribute< std::vector<float> >( "constituentWeights" ) );
    std::vector<float> pt;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<float> e;
    xAOD::JetConstituentVector consVec = jet->getConstituents();
    if( consVec.isValid() ) {
      // don't use auto since iterator can also set the scale ...
      // not sure what that does with auto - probably default but just incase
      // use the example provided in
      // http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODJet/xAODJet/JetConstituentVector.h
      xAOD::JetConstituentVector::iterator constit = consVec.begin();
      xAOD::JetConstituentVector::iterator constitE = consVec.end();
      for( ; constit != constitE; constit++){
        pt. push_back( constit->pt() / m_units );
        eta.push_back( constit->eta() );
        phi.push_back( constit->phi() );
        e.  push_back( constit->e() / m_units  );
      }
    }
    m_constituent_pt-> push_back( pt  );
    m_constituent_eta->push_back( eta );
    m_constituent_phi->push_back( phi );
    m_constituent_e->  push_back( e   );
  }

  if ( m_infoSwitch.m_flavorTag || m_infoSwitch.m_flavorTagHLT ) {
    const xAOD::BTagging * myBTag(0);

    if(m_infoSwitch.m_flavorTag){
      myBTag = jet->btagging();
    }else if(m_infoSwitch.m_flavorTagHLT){
      myBTag = jet->auxdata< const xAOD::BTagging* >("HLTBTag");
    }

    if(m_infoSwitch.m_JVC ) {
      static SG::AuxElement::ConstAccessor<double> JetVertexCharge_discriminant("JetVertexCharge_discriminant");
      safeFill<double, double, xAOD::BTagging>(myBTag, JetVertexCharge_discriminant, m_JetVertexCharge_discriminant, -999);
    }

    // MV2c taggers
    double val;

    val=-999;
    myBTag->variable<double>("MV2c00"   , "discriminant", val);
    m_MV2c00   ->push_back( val );

    val=-999;
    myBTag->variable<double>("MV2c10"   , "discriminant", val);
    m_MV2c10   ->push_back( val );
    val=-999;
    myBTag->variable<double>("MV2c10mu" , "discriminant", val);
    m_MV2c10mu ->push_back( val );

    val=-999;
    myBTag->variable<double>("MV2c10rnn", "discriminant", val);
    m_MV2c10rnn->push_back( val );
    val=-999;
    myBTag->variable<double>("MV2c20"   , "discriminant", val);
    m_MV2c20   ->push_back( val );
    
    val=-999;
    myBTag->variable<double>("MV2rmu" , "discriminant", val);
    m_MV2rmu ->push_back( val );

    val=-999;
    myBTag->variable<double>("MV2r", "discriminant", val);
    m_MV2r->push_back( val );

    val=-999;
    myBTag->variable<double>("MV2c100"  , "discriminant", val);
    m_MV2c100  ->push_back( val );

    // DL1 taggers
    double pu, pb, pc, score;

    pu=0; pb=0; pc=0;
    myBTag->variable<double>("DL1" , "pu", pu);
    myBTag->variable<double>("DL1" , "pc", pc);
    myBTag->variable<double>("DL1" , "pb", pb);
    score=log( pb / (0.08*pc+0.92*pu) );
    m_DL1_pu->push_back(pu);
    m_DL1_pc->push_back(pc);
    m_DL1_pb->push_back(pb);
    m_DL1->push_back( score );

    pu=0; pb=0; pc=0;
    myBTag->variable<double>("DL1mu" , "pu", pu);
    myBTag->variable<double>("DL1mu" , "pc", pc);
    myBTag->variable<double>("DL1mu" , "pb", pb);
    score=log( pb / (0.08*pc+0.92*pu) );
    m_DL1mu_pu->push_back(pu);
    m_DL1mu_pc->push_back(pc);
    m_DL1mu_pb->push_back(pb);
    m_DL1mu->push_back( score );

    pu=0; pb=0; pc=0;
    myBTag->variable<double>("DL1rnn" , "pu", pu);
    myBTag->variable<double>("DL1rnn" , "pc", pc);
    myBTag->variable<double>("DL1rnn" , "pb", pb);
    score=log( pb / (0.03*pc+0.97*pu) );
    m_DL1rnn_pu->push_back(pu);
    m_DL1rnn_pc->push_back(pc);
    m_DL1rnn_pb->push_back(pb);
    m_DL1rnn->push_back( score );
    
    pu=0; pb=0; pc=0;
    myBTag->variable<double>("DL1rmu" , "pu", pu);
    myBTag->variable<double>("DL1rmu" , "pc", pc);
    myBTag->variable<double>("DL1rmu" , "pb", pb);
    score=log( pb / (0.08*pc+0.92*pu) );
    m_DL1rmu_pu->push_back(pu);
    m_DL1rmu_pc->push_back(pc);
    m_DL1rmu_pb->push_back(pb);
    m_DL1rmu->push_back( score );

    pu=0; pb=0; pc=0;
    myBTag->variable<double>("DL1r" , "pu", pu);
    myBTag->variable<double>("DL1r" , "pc", pc);
    myBTag->variable<double>("DL1r" , "pb", pb);
    score=log( pb / (0.03*pc+0.97*pu) );
    m_DL1r_pu->push_back(pu);
    m_DL1r_pc->push_back(pc);
    m_DL1r_pb->push_back(pb);
    m_DL1r->push_back( score );

    // flavor groups truth definition
    static SG::AuxElement::ConstAccessor<int> hadConeExclTruthLabel("HadronConeExclTruthLabelID");
    safeFill<int, int, xAOD::Jet>(jet, hadConeExclTruthLabel, m_HadronConeExclTruthLabelID, -999);

    static SG::AuxElement::ConstAccessor<int> hadConeExclExtendedTruthLabel("HadronConeExclExtendedTruthLabelID");
    safeFill<int, int, xAOD::Jet>(jet, hadConeExclExtendedTruthLabel, m_HadronConeExclExtendedTruthLabelID, -999);

    if(m_infoSwitch.m_jetFitterDetails ) {

      static SG::AuxElement::ConstAccessor< int   > jf_nVTXAcc       ("JetFitter_nVTX");
      safeFill<int, float, xAOD::BTagging>(myBTag, jf_nVTXAcc, m_JetFitter_nVTX, -999);

      static SG::AuxElement::ConstAccessor< int   > jf_nSingleTracks ("JetFitter_nSingleTracks");
      safeFill<int, float, xAOD::BTagging>(myBTag, jf_nSingleTracks, m_JetFitter_nSingleTracks, -999);

      static SG::AuxElement::ConstAccessor< int   > jf_nTracksAtVtx  ("JetFitter_nTracksAtVtx");
      safeFill<int, float, xAOD::BTagging>(myBTag, jf_nTracksAtVtx, m_JetFitter_nTracksAtVtx, -999);

      static SG::AuxElement::ConstAccessor< float > jf_mass          ("JetFitter_mass");
      safeFill<float, float, xAOD::BTagging>(myBTag, jf_mass, m_JetFitter_mass, -999);

      static SG::AuxElement::ConstAccessor< float > jf_energyFraction("JetFitter_energyFraction");
      safeFill<float, float, xAOD::BTagging>(myBTag, jf_energyFraction, m_JetFitter_energyFraction, -999);

      static SG::AuxElement::ConstAccessor< float > jf_significance3d("JetFitter_significance3d");
      safeFill<float, float, xAOD::BTagging>(myBTag, jf_significance3d, m_JetFitter_significance3d, -999);

      static SG::AuxElement::ConstAccessor< float > jf_deltaeta      ("JetFitter_deltaeta");
      safeFill<float, float, xAOD::BTagging>(myBTag, jf_deltaeta, m_JetFitter_deltaeta, -999);

      static SG::AuxElement::ConstAccessor< float > jf_deltaphi      ("JetFitter_deltaphi");
      safeFill<float, float, xAOD::BTagging>(myBTag, jf_deltaphi, m_JetFitter_deltaphi, -999);

      static SG::AuxElement::ConstAccessor< int   > jf_N2Tpar        ("JetFitter_N2Tpair");
      safeFill<int, float, xAOD::BTagging>(myBTag, jf_N2Tpar, m_JetFitter_N2Tpar, -999);

      //static SG::AuxElement::ConstAccessor< double > jf_pb           ("JetFitterCombNN_pb");
      //safeFill<double, float, xAOD::BTagging>(myBTag, jf_pb, m_JetFitter_pb, -999);
      //
      //static SG::AuxElement::ConstAccessor< double > jf_pc           ("JetFitterCombNN_pc");
      //safeFill<double, float, xAOD::BTagging>(myBTag, jf_pc, m_JetFitter_pc, -999);
      //
      //static SG::AuxElement::ConstAccessor< double > jf_pu           ("JetFitterCombNN_pu");
      //safeFill<double, float, xAOD::BTagging>(myBTag, jf_pu, m_JetFitter_pu, -999);

    }

    if(m_infoSwitch.m_svDetails ) {
      if(m_debug) std::cout << "Filling m_svDetails " << std::endl;

      /// @brief SV0 : Number of good tracks in vertex
      static SG::AuxElement::ConstAccessor< int   >   sv0_NGTinSvxAcc     ("SV0_NGTinSvx");
      safeFill<int, float, xAOD::BTagging>(myBTag,    sv0_NGTinSvxAcc, m_sv0_NGTinSvx, -999);

      // @brief SV0 : Number of 2-track pairs
      static SG::AuxElement::ConstAccessor< int   >   sv0_N2TpairAcc      ("SV0_N2Tpair");
      safeFill<int, float, xAOD::BTagging>(myBTag, sv0_N2TpairAcc, m_sv0_N2Tpair, -999);

      /// @brief SV0 : vertex mass
      static SG::AuxElement::ConstAccessor< float   > sv0_masssvxAcc      ("SV0_masssvx");
      safeFill<float, float, xAOD::BTagging>(myBTag, sv0_masssvxAcc, m_sv0_massvx, -999);

      /// @brief SV0 : energy fraction
      static SG::AuxElement::ConstAccessor< float   > sv0_efracsvxAcc     ("SV0_efracsvx");
      safeFill<float, float, xAOD::BTagging>(myBTag, sv0_efracsvxAcc, m_sv0_efracsvx, -999);

      /// @brief SV0 : 3D vertex significance
      static SG::AuxElement::ConstAccessor< float   > sv0_normdistAcc     ("SV0_normdist");
      safeFill<float, float, xAOD::BTagging>(myBTag, sv0_normdistAcc, m_sv0_normdist, -999);

      double sv0;
      myBTag->variable<double>("SV0", "significance3D", sv0);
      m_SV0->push_back(sv0);

      m_SV1IP3D->push_back( myBTag -> SV1plusIP3D_discriminant() );

      double w=(myBTag->IP3D_pb()/myBTag->IP3D_pu()) * (myBTag->SV1_pb()/myBTag->SV1_pu());
      double x=50;
      if(w/(1+w)<1) x=-1.0*TMath::Log10(1-(w/(1+w)));
      m_COMBx->push_back(x);

      /// @brief SV1 : Number of good tracks in vertex
      static SG::AuxElement::ConstAccessor< int   >   sv1_NGTinSvxAcc     ("SV1_NGTinSvx");
      safeFill<int, float, xAOD::BTagging>(myBTag, sv1_NGTinSvxAcc, m_sv1_NGTinSvx, -999);

      // @brief SV1 : Number of 2-track pairs
      static SG::AuxElement::ConstAccessor< int   >   sv1_N2TpairAcc      ("SV1_N2Tpair");
      safeFill<int, float, xAOD::BTagging>(myBTag, sv1_N2TpairAcc, m_sv1_N2Tpair, -999);

      /// @brief SV1 : vertex mass
      static SG::AuxElement::ConstAccessor< float   > sv1_masssvxAcc      ("SV1_masssvx");
      safeFill<float, float, xAOD::BTagging>(myBTag, sv1_masssvxAcc, m_sv1_massvx, -999);

      /// @brief SV1 : energy fraction
      static SG::AuxElement::ConstAccessor< float   > sv1_efracsvxAcc     ("SV1_efracsvx");
      safeFill<float, float, xAOD::BTagging>(myBTag, sv1_efracsvxAcc, m_sv1_efracsvx, -999);

      /// @brief SV1 : 3D vertex significance
      static SG::AuxElement::ConstAccessor< float   > sv1_normdistAcc     ("SV1_normdist");
      safeFill<float, float, xAOD::BTagging>(myBTag, sv1_normdistAcc, m_sv1_normdist, -999);

      double sv1_pu = -30;  myBTag->variable<double>("SV1", "pu", sv1_pu);
      double sv1_pb = -30;  myBTag->variable<double>("SV1", "pb", sv1_pb);
      double sv1_pc = -30;  myBTag->variable<double>("SV1", "pc", sv1_pc);

      m_sv1_pu         ->push_back(sv1_pu);
      m_sv1_pb         ->push_back(sv1_pb);
      m_sv1_pc         ->push_back(sv1_pc);
      m_SV1            ->push_back( myBTag->calcLLR(sv1_pb,sv1_pu)  );
      m_sv1_c          ->push_back( myBTag->calcLLR(sv1_pb,sv1_pc)  );
      m_sv1_cu         ->push_back( myBTag->calcLLR(sv1_pc,sv1_pu)  );

      float sv1_Lxy;        myBTag->variable<float>("SV1", "Lxy"         , sv1_Lxy);
      float sv1_sig3d;      myBTag->variable<float>("SV1", "significance3d"         , sv1_sig3d);
      float sv1_L3d;        myBTag->variable<float>("SV1", "L3d"         , sv1_L3d);
      float sv1_distmatlay; myBTag->variable<float>("SV1", "dstToMatLay" , sv1_distmatlay);
      float sv1_dR;         myBTag->variable<float>("SV1", "deltaR"      , sv1_dR );

      m_sv1_Lxy        ->push_back(sv1_Lxy        );
      m_sv1_sig3d      ->push_back(sv1_sig3d      );
      m_sv1_L3d        ->push_back(sv1_L3d        );
      m_sv1_distmatlay ->push_back(sv1_distmatlay );
      m_sv1_dR         ->push_back(sv1_dR         );


    }

    if(m_infoSwitch.m_ipDetails ) {
      if(m_debug) std::cout << "Filling m_ipDetails " << std::endl;

      //
      // IP2D
      //

      /// @brief IP2D: track grade
      static SG::AuxElement::ConstAccessor< std::vector<int>   >   IP2D_gradeOfTracksAcc     ("IP2D_gradeOfTracks");
      safeVecFill<int, float, xAOD::BTagging>(myBTag, IP2D_gradeOfTracksAcc, m_IP2D_gradeOfTracks);

      /// @brief IP2D : tracks from V0
      static SG::AuxElement::ConstAccessor< std::vector<bool>   >  IP2D_flagFromV0ofTracksAcc("IP2D_flagFromV0ofTracks");
      safeVecFill<bool, float, xAOD::BTagging>(myBTag, IP2D_flagFromV0ofTracksAcc, m_IP2D_flagFromV0ofTracks);

      /// @brief IP2D : d0 value with respect to primary vertex
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP2D_valD0wrtPVofTracksAcc("IP2D_valD0wrtPVofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP2D_valD0wrtPVofTracksAcc, m_IP2D_valD0wrtPVofTracks);

      /// @brief IP2D : d0 significance with respect to primary vertex
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP2D_sigD0wrtPVofTracksAcc("IP2D_sigD0wrtPVofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP2D_sigD0wrtPVofTracksAcc, m_IP2D_sigD0wrtPVofTracks);

      /// @brief IP2D : track contribution to B likelihood
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP2D_weightBofTracksAcc   ("IP2D_weightBofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP2D_weightBofTracksAcc, m_IP2D_weightBofTracks);

      /// @brief IP2D : track contribution to C likelihood
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP2D_weightCofTracksAcc   ("IP2D_weightCofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP2D_weightCofTracksAcc, m_IP2D_weightCofTracks);

      /// @brief IP2D : track contribution to U likelihood
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP2D_weightUofTracksAcc   ("IP2D_weightUofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP2D_weightUofTracksAcc, m_IP2D_weightUofTracks);

      double ip2_pu = -99;  myBTag->variable<double>("IP2D", "pu", ip2_pu);
      double ip2_pb = -99;  myBTag->variable<double>("IP2D", "pb", ip2_pb);
      double ip2_pc = -99;  myBTag->variable<double>("IP2D", "pc", ip2_pc);

      m_IP2D_pu         ->push_back(ip2_pu);
      m_IP2D_pb         ->push_back(ip2_pb);
      m_IP2D_pc         ->push_back(ip2_pc);

      m_IP2D            ->push_back( myBTag->calcLLR(ip2_pb,ip2_pu)  );
      m_IP2D_c          ->push_back( myBTag->calcLLR(ip2_pb,ip2_pc)  );
      m_IP2D_cu         ->push_back( myBTag->calcLLR(ip2_pc,ip2_pu)  );


      //
      // IP3D
      //

      /// @brief IP3D: track grade
      static SG::AuxElement::ConstAccessor< std::vector<int>   >   IP3D_gradeOfTracksAcc     ("IP3D_gradeOfTracks");
      safeVecFill<int, float, xAOD::BTagging>(myBTag, IP3D_gradeOfTracksAcc, m_IP3D_gradeOfTracks);

      /// @brief IP3D : tracks from V0
      static SG::AuxElement::ConstAccessor< std::vector<bool>   >  IP3D_flagFromV0ofTracksAcc("IP3D_flagFromV0ofTracks");
      safeVecFill<bool, float, xAOD::BTagging>(myBTag, IP3D_flagFromV0ofTracksAcc, m_IP3D_flagFromV0ofTracks);

      /// @brief IP3D : d0 value with respect to primary vertex
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP3D_valD0wrtPVofTracksAcc("IP3D_valD0wrtPVofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP3D_valD0wrtPVofTracksAcc, m_IP3D_valD0wrtPVofTracks);

      /// @brief IP3D : d0 significance with respect to primary vertex
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP3D_sigD0wrtPVofTracksAcc("IP3D_sigD0wrtPVofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP3D_sigD0wrtPVofTracksAcc, m_IP3D_sigD0wrtPVofTracks);

      /// @brief IP3D : z0 value with respect to primary vertex
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP3D_valZ0wrtPVofTracksAcc("IP3D_valZ0wrtPVofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP3D_valZ0wrtPVofTracksAcc, m_IP3D_valZ0wrtPVofTracks);

      /// @brief IP3D : z0 significance with respect to primary vertex
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP3D_sigZ0wrtPVofTracksAcc("IP3D_sigZ0wrtPVofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP3D_sigZ0wrtPVofTracksAcc, m_IP3D_sigZ0wrtPVofTracks);

      /// @brief IP3D : track contribution to B likelihood
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP3D_weightBofTracksAcc   ("IP3D_weightBofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP3D_weightBofTracksAcc, m_IP3D_weightBofTracks);

      /// @brief IP3D : track contribution to C likelihood
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP3D_weightCofTracksAcc   ("IP3D_weightCofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP3D_weightCofTracksAcc, m_IP3D_weightCofTracks);

      /// @brief IP3D : track contribution to U likelihood
      static SG::AuxElement::ConstAccessor< std::vector<float>   > IP3D_weightUofTracksAcc   ("IP3D_weightUofTracks");
      safeVecFill<float, float, xAOD::BTagging>(myBTag, IP3D_weightUofTracksAcc, m_IP3D_weightUofTracks);

      double ip3_pu = -30;  myBTag->variable<double>("IP3D", "pu", ip3_pu);
      double ip3_pb = -30;  myBTag->variable<double>("IP3D", "pb", ip3_pb);
      double ip3_pc = -30;  myBTag->variable<double>("IP3D", "pc", ip3_pc);

      m_IP3D_pu         ->push_back(ip3_pu  );
      m_IP3D_pb         ->push_back(ip3_pb  );
      m_IP3D_pc         ->push_back(ip3_pc  );

      m_IP3D            ->push_back( myBTag->calcLLR(ip3_pb,ip3_pu)  );
      m_IP3D_c          ->push_back( myBTag->calcLLR(ip3_pb,ip3_pc)  );
      m_IP3D_cu         ->push_back( myBTag->calcLLR(ip3_pc,ip3_pu)  );

    }



    if(m_infoSwitch.m_flavorTagHLT ) {
      if(m_debug) std::cout << "Filling m_flavorTagHLT " << std::endl;
      const xAOD::Vertex *online_pvx       = jet->auxdata<const xAOD::Vertex*>("HLTBJetTracks_vtx");
      const xAOD::Vertex *online_pvx_bkg   = jet->auxdata<const xAOD::Vertex*>("HLTBJetTracks_vtx_bkg");
      const xAOD::Vertex *offline_pvx      = jet->auxdata<const xAOD::Vertex*>("offline_vtx");

      if(online_pvx)  m_vtxOnlineValid->push_back(1.0);
      else            m_vtxOnlineValid->push_back(0.0);

      char hadDummyPV = jet->auxdata< char >("hadDummyPV");
      if( hadDummyPV == '0')  m_vtxHadDummy->push_back(0.0);
      if( hadDummyPV == '1')  m_vtxHadDummy->push_back(1.0);
      if( hadDummyPV == '2')  m_vtxHadDummy->push_back(2.0);

      static SG::AuxElement::ConstAccessor< float > acc_bs_online_vs ("bs_online_vz");
      if(acc_bs_online_vs.isAvailable( *jet) ){
	if(m_debug) std::cout << "Have bs_online_vz " << std::endl;
	float bs_online_vz = jet->auxdata< float >("bs_online_vz");
	//std::cout << "**bs_online_vz " << bs_online_vz << std::endl;
	m_bs_online_vz->push_back( bs_online_vz );

	float bs_online_vx = jet->auxdata< float >("bs_online_vx");
	//std::cout << "**bs_online_vx " << bs_online_vx << std::endl;
	m_bs_online_vx->push_back( bs_online_vx );

	float bs_online_vy = jet->auxdata< float >("bs_online_vy");
	//std::cout << "**bs_online_vy " << bs_online_vy << std::endl;
	m_bs_online_vy->push_back( bs_online_vy );
      }else{
	m_bs_online_vz->push_back( -999 );
	m_bs_online_vx->push_back( -999 );
	m_bs_online_vy->push_back( -999 );
      }

      if(m_debug) std::cout << "Filling m_vtx_offline " << std::endl;
      if(offline_pvx){
	m_vtx_offline_x0->push_back( offline_pvx->x() );
	m_vtx_offline_y0->push_back( offline_pvx->y() );
	m_vtx_offline_z0->push_back( offline_pvx->z() );
      }else{
	m_vtx_offline_x0->push_back( -999 );
	m_vtx_offline_y0->push_back( -999 );
	m_vtx_offline_z0->push_back( -999 );
      }

      if(m_debug) std::cout << "Done Filling m_vtx_offline " << std::endl;

      if(m_debug) std::cout << "Filling m_vtx_online... " << std::endl;
      if(online_pvx){
	if(m_debug) std::cout << " ... online_pvx valid " << std::endl;
        m_vtx_online_x0->push_back( online_pvx->x() );
        m_vtx_online_y0->push_back( online_pvx->y() );
        m_vtx_online_z0->push_back( online_pvx->z() );
      }else{
        m_vtx_online_x0->push_back( -999 );
        m_vtx_online_y0->push_back( -999 );
        m_vtx_online_z0->push_back( -999 );
      }

      if(m_debug) std::cout << "Filling m_vtx_online... " << std::endl;
      if(online_pvx_bkg){
	if(m_debug) std::cout << " ...online_pvx_bkg valid " << std::endl;
        m_vtx_online_bkg_x0->push_back( online_pvx_bkg->x() );
        m_vtx_online_bkg_y0->push_back( online_pvx_bkg->y() );
        m_vtx_online_bkg_z0->push_back( online_pvx_bkg->z() );
      }else{
        m_vtx_online_bkg_x0->push_back( -999 );
        m_vtx_online_bkg_y0->push_back( -999 );
        m_vtx_online_bkg_z0->push_back( -999 );
      }

    }// m_flavorTagHLT
    if(m_debug) std::cout << "Done m_flavorTagHLT " << std::endl;
  }


  if( !m_infoSwitch.m_jetBTag.empty() || !m_infoSwitch.m_jetBTagCts.empty() ) {
    for(auto btag : m_btags)
      btag->Fill( jet );
  } // jetBTag

  if ( m_infoSwitch.m_area ) {

    static SG::AuxElement::ConstAccessor<float> ghostArea("JetGhostArea");
    safeFill<float, float, xAOD::Jet>(jet, ghostArea, m_GhostArea, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea("ActiveArea");
    safeFill<float, float, xAOD::Jet>(jet, activeArea, m_ActiveArea, -999);

    static SG::AuxElement::ConstAccessor<float> voronoiArea("VoronoiArea");
    safeFill<float, float, xAOD::Jet>(jet, voronoiArea, m_VoronoiArea, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_pt("ActiveArea4vec_pt");
    safeFill<float, float, xAOD::Jet>(jet, activeArea_pt, m_ActiveArea4vec_pt, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_eta("ActiveArea4vec_eta");
    safeFill<float, float, xAOD::Jet>(jet, activeArea_eta, m_ActiveArea4vec_eta, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_phi("ActiveArea4vec_phi");
    safeFill<float, float, xAOD::Jet>(jet, activeArea_phi, m_ActiveArea4vec_phi, -999);

    static SG::AuxElement::ConstAccessor<float> activeArea_m("ActiveArea4vec_m");
    safeFill<float, float, xAOD::Jet>(jet, activeArea_m, m_ActiveArea4vec_m, -999);
  }


  if ( m_infoSwitch.m_truth && m_mc ) {

    static SG::AuxElement::ConstAccessor<int> ConeTruthLabelID ("ConeTruthLabelID");
    safeFill<int, int, xAOD::Jet>(jet, ConeTruthLabelID, m_ConeTruthLabelID, -999);

    static SG::AuxElement::ConstAccessor<int> TruthCount ("TruthCount");
    safeFill<int, int, xAOD::Jet>(jet, TruthCount, m_TruthCount, -999);

    //    seems to be empty
    //      static SG::AuxElement::ConstAccessor<float> TruthPt ("TruthPt");
    //      if ( TruthPt.isAvailable( *jet) ) {
    //        m_truthPt->push_back( TruthPt( *jet)/1000 );
    //      } else { m_truthPt->push_back( -999 ); }

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_B ("TruthLabelDeltaR_B");
    safeFill<float, float, xAOD::Jet>(jet, TruthLabelDeltaR_B, m_TruthLabelDeltaR_B, -999);

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_C ("TruthLabelDeltaR_C");
    safeFill<float, float, xAOD::Jet>(jet, TruthLabelDeltaR_C, m_TruthLabelDeltaR_C, -999);

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_T ("TruthLabelDeltaR_T");
    safeFill<float, float, xAOD::Jet>(jet, TruthLabelDeltaR_T, m_TruthLabelDeltaR_T, -999);

    static SG::AuxElement::ConstAccessor<int> partonLabel("PartonTruthLabelID");
    safeFill<int, int, xAOD::Jet>(jet, partonLabel, m_PartonTruthLabelID, -999);

    static SG::AuxElement::ConstAccessor<float> ghostTruthAssFrac("GhostTruthAssociationFraction");
    safeFill<float, float, xAOD::Jet>(jet, ghostTruthAssFrac, m_GhostTruthAssociationFraction, -999);

    const xAOD::Jet* truthJet = HelperFunctions::getLink<xAOD::Jet>( jet, "GhostTruthAssociationLink" );
    if(truthJet) {
      m_truth_pt->push_back ( truthJet->pt() / m_units );
      m_truth_eta->push_back( truthJet->eta() );
      m_truth_phi->push_back( truthJet->phi() );
      m_truth_E->push_back  ( truthJet->e() / m_units );
    } else {
      m_truth_pt->push_back ( -999 );
      m_truth_eta->push_back( -999 );
      m_truth_phi->push_back( -999 );
      m_truth_E->push_back  ( -999 );
    }

  }

  if ( m_infoSwitch.m_truthDetails ) {

    //
    // B-Hadron Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostBHadronsFinalCount ("GhostBHadronsFinalCount");
    safeFill<int, int, xAOD::Jet>(jet, GhostBHadronsFinalCount, m_GhostBHadronsFinalCount, -999);

    static SG::AuxElement::ConstAccessor<int> GhostBHadronsInitialCount ("GhostBHadronsInitialCount");
    safeFill<int, int, xAOD::Jet>(jet, GhostBHadronsInitialCount, m_GhostBHadronsInitialCount, -999);

    static SG::AuxElement::ConstAccessor<int> GhostBQuarksFinalCount ("GhostBQuarksFinalCount");
    safeFill<int, int, xAOD::Jet>(jet, GhostBQuarksFinalCount, m_GhostBQuarksFinalCount, -999);

    static SG::AuxElement::ConstAccessor<float> GhostBHadronsFinalPt ("GhostBHadronsFinalPt");
    safeFill<float, float, xAOD::Jet>(jet, GhostBHadronsFinalPt, m_GhostBHadronsFinalPt, -999);

    static SG::AuxElement::ConstAccessor<float> GhostBHadronsInitialPt ("GhostBHadronsInitialPt");
    safeFill<float, float, xAOD::Jet>(jet, GhostBHadronsInitialPt, m_GhostBHadronsInitialPt, -999);

    static SG::AuxElement::ConstAccessor<float> GhostBQuarksFinalPt ("GhostBQuarksFinalPt");
    safeFill<float, float, xAOD::Jet>(jet, GhostBQuarksFinalPt, m_GhostBQuarksFinalPt, -999);

    //
    // C-Hadron Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostCHadronsFinalCount ("GhostCHadronsFinalCount");
    safeFill<int, int, xAOD::Jet>(jet, GhostCHadronsFinalCount, m_GhostCHadronsFinalCount, -999);

    static SG::AuxElement::ConstAccessor<int> GhostCHadronsInitialCount ("GhostCHadronsInitialCount");
    safeFill<int, int, xAOD::Jet>(jet, GhostCHadronsInitialCount, m_GhostCHadronsInitialCount, -999);

    static SG::AuxElement::ConstAccessor<int> GhostCQuarksFinalCount ("GhostCQuarksFinalCount");
    safeFill<int, int, xAOD::Jet>(jet, GhostCQuarksFinalCount, m_GhostCQuarksFinalCount, -999);

    static SG::AuxElement::ConstAccessor<float> GhostCHadronsFinalPt ("GhostCHadronsFinalPt");
    safeFill<float, float, xAOD::Jet>(jet, GhostCHadronsFinalPt, m_GhostCHadronsFinalPt, -999);

    static SG::AuxElement::ConstAccessor<float> GhostCHadronsInitialPt ("GhostCHadronsInitialPt");
    safeFill<float, float, xAOD::Jet>(jet, GhostCHadronsInitialPt, m_GhostCHadronsInitialPt, -999);

    static SG::AuxElement::ConstAccessor<float> GhostCQuarksFinalPt ("GhostCQuarksFinalPt");
    safeFill<float, float, xAOD::Jet>(jet, GhostCQuarksFinalPt, m_GhostCQuarksFinalPt, -999);

    //
    // Tau Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostTausFinalCount ("GhostTausFinalCount");
    safeFill<int, int, xAOD::Jet>(jet, GhostTausFinalCount, m_GhostTausFinalCount, -999);

    // THE ONLY UN-OFFICIAL PIECE OF CODE HERE USE WITH CAUTION
    static SG::AuxElement::ConstAccessor<float> GhostTausFinalPt ("GhostTausFinalPt");
    safeFill<float, float, xAOD::Jet>(jet, GhostTausFinalPt, m_GhostTausFinalPt, -999);

    // light quark(1,2,3) , gluon (21 or 9), charm(4) and b(5)
    // GhostPartons should select for these pdgIds only
    //    static SG::AuxElement::ConstAccessor< std::vector<const xAOD::TruthParticle*> > ghostPartons("GhostPartons");
    //    if( ghostPartons.isAvailable( *jet )) {
    //    std::vector<const xAOD::TruthParticle*> truthPartons = ghostPartons( *jet );

    std::vector<const xAOD::TruthParticle*> truthPartons = jet->getAssociatedObjects<xAOD::TruthParticle>("GhostPartons");

    if( truthPartons.size() == 0){
      m_truth_pdgId->push_back(-999);
    } else {
      int iParent = 0;
      for(unsigned int i=1; i < truthPartons.size(); ++i){
        if( (truthPartons.at(i)->pt() > 0.001) && (truthPartons.at(i)->e() > truthPartons.at(iParent)->e()) )
          iParent = i;
      }
      m_truth_pdgId->push_back(truthPartons.at(iParent)->pdgId());
      m_truth_partonPt->push_back(truthPartons.at(iParent)->pt() / m_units);
      m_truth_partonDR->push_back(truthPartons.at(iParent)->p4().DeltaR( jet->p4() ));
    }

  }


  if ( m_infoSwitch.m_charge ) {
    xAOD::JetFourMom_t p4UsedInJetCharge;
    bool status = jet->getAttribute<xAOD::JetFourMom_t>( "JetPileupScaleMomentum", p4UsedInJetCharge );
    static SG::AuxElement::ConstAccessor<float>              uncalibratedJetCharge ("Charge");

    if(status){
      float ptUsedInJetCharge   = p4UsedInJetCharge.Pt();
      float calibratedJetCharge = jet->pt() ? (ptUsedInJetCharge * uncalibratedJetCharge(*jet) / jet->pt()) : -99;
      m_charge->push_back(calibratedJetCharge);
    }else{
      m_charge->push_back(-99);
    }

  }

  if ( m_infoSwitch.m_passSel ) {
    char passSel;
    bool status = jet->getAttribute<char>( "passSel", passSel );
    if(status){
      m_passSel->push_back(passSel);
    }else{
      m_passSel->push_back(-99);
    }
  }

  return;
}
