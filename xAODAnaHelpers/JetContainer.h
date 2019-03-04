#ifndef xAODAnaHelpers_JetContainer_H
#define xAODAnaHelpers_JetContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include "xAODJet/JetContainer.h"

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODAnaHelpers/Jet.h>
#include <xAODAnaHelpers/ParticleContainer.h>

#include "InDetTrackSelectionTool/InDetTrackSelectionTool.h"


namespace xAH {

    class JetContainer : public ParticleContainer<Jet,HelperClasses::JetInfoSwitch>
    {
    public:
      JetContainer(const std::string& name = "jet", const std::string& detailStr="", float units = 1e3, bool mc = false);
      virtual ~JetContainer();

      virtual void setTree    (TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillJet( const xAOD::Jet* jet,            const xAOD::Vertex* pv, int pvLocation );
      virtual void FillJet( const xAOD::IParticle* particle, const xAOD::Vertex* pv, int pvLocation );

      virtual void updateParticle(uint idx, Jet& jet);

//template<typename T>
//  void setBranch(TTree* tree, std::string varName, std::vector<T>* localVectorPtr);

    private:

      InDet::InDetTrackSelectionTool * m_trkSelTool;

      //
      // Vector branches

      // rapidity
      std::vector<float> *m_rapidity;

      // trigger
      std::vector<int>               *m_isTrigMatched;
      std::vector<std::vector<int> > *m_isTrigMatchedToChain;
      std::vector<std::string>       *m_listTrigChains;
      
      // clean
      std::vector<float> *m_Timing;
      std::vector<float> *m_LArQuality;
      std::vector<float> *m_HECQuality;
      std::vector<float> *m_NegativeE;
      std::vector<float> *m_AverageLArQF;
      std::vector<float> *m_BchCorrCell;
      std::vector<float> *m_N90Constituents;
      std::vector<float> *m_LArBadHVEnergyFrac;
      std::vector<int>   *m_LArBadHVNCell;
      std::vector<float> *m_OotFracClusters5;
      std::vector<float> *m_OotFracClusters10;
      std::vector<float> *m_LeadingClusterPt;
      std::vector<float> *m_LeadingClusterSecondLambda;
      std::vector<float> *m_LeadingClusterCenterLambda;
      std::vector<float> *m_LeadingClusterSecondR;
      std::vector<int>   *m_clean_passLooseBad;
      std::vector<int>   *m_clean_passLooseBadUgly;
      std::vector<int>   *m_clean_passTightBad;
      std::vector<int>   *m_clean_passTightBadUgly;

      // energy
      std::vector<float> *m_HECFrac;
      std::vector<float> *m_EMFrac;
      std::vector<float> *m_CentroidR;
      std::vector<float> *m_FracSamplingMax;
      std::vector<float> *m_FracSamplingMaxIndex;
      std::vector<float> *m_LowEtConstituentsFrac;
      std::vector<float> *m_GhostMuonSegmentCount;
      std::vector<float> *m_Width;

      // scales
      std::vector<float> *m_emScalePt;
      std::vector<float> *m_constScalePt;
      std::vector<float> *m_pileupScalePt;
      std::vector<float> *m_originConstitScalePt;
      std::vector<float> *m_etaJESScalePt;
      std::vector<float> *m_gscScalePt;
      std::vector<float> *m_jmsScalePt;
      std::vector<float> *m_insituScalePt;

      std::vector<float> *m_emScaleM;
      std::vector<float> *m_constScaleM;
      std::vector<float> *m_pileupScaleM;
      std::vector<float> *m_originConstitScaleM;
      std::vector<float> *m_etaJESScaleM;
      std::vector<float> *m_gscScaleM;
      std::vector<float> *m_jmsScaleM;
      std::vector<float> *m_insituScaleM;

      // constScale Eta
      std::vector<float> *m_constScaleEta;

      // detector Eta
      std::vector<float> *m_detectorEta;

      // layer
      std::vector< std::vector<float> > *m_EnergyPerSampling;

      // tracksAll
      std::vector< std::vector<int> >   *m_NumTrkPt1000;
      std::vector< std::vector<float> > *m_SumPtTrkPt1000;
      std::vector< std::vector<float> > *m_TrackWidthPt1000;
      std::vector< std::vector<int> >   *m_NumTrkPt500;
      std::vector< std::vector<float> > *m_SumPtTrkPt500;
      std::vector< std::vector<float> > *m_TrackWidthPt500;
      std::vector< std::vector<float> > *m_JVF;

      // trackPV
      std::vector<float> *m_NumTrkPt1000PV;
      std::vector<float> *m_SumPtTrkPt1000PV;
      std::vector<float> *m_TrackWidthPt1000PV;
      std::vector<float> *m_NumTrkPt500PV;
      std::vector<float> *m_SumPtTrkPt500PV;
      std::vector<float> *m_TrackWidthPt500PV;
      std::vector<float> *m_JVFPV;


      // trackAll or trackPV
      std::vector<float> *m_Jvt;
      std::vector<int> *m_JvtPass_Loose;
      std::vector< std::vector<float> > *m_JvtEff_SF_Loose;
      std::vector<int> *m_JvtPass_Medium;
      std::vector< std::vector<float> > *m_JvtEff_SF_Medium;
      std::vector<int> *m_JvtPass_Tight;
      std::vector< std::vector<float> > *m_JvtEff_SF_Tight;
      std::vector<float> *m_JvtJvfcorr;
      std::vector<float> *m_JvtRpt;
      std::vector<int> *m_fJvtPass_Medium;
      std::vector< std::vector<float> > *m_fJvtEff_SF_Medium;
      std::vector<int> *m_fJvtPass_Tight;
      std::vector< std::vector<float> > *m_fJvtEff_SF_Tight;

      // allTrack
      std::vector<int>                  *m_GhostTrackCount;
      std::vector<float>                *m_GhostTrackPt;
      std::vector< std::vector<float> > *m_GhostTrack_pt;
      std::vector< std::vector<float> > *m_GhostTrack_qOverP;
      std::vector< std::vector<float> > *m_GhostTrack_eta;
      std::vector< std::vector<float> > *m_GhostTrack_phi;
      std::vector< std::vector<float> > *m_GhostTrack_e;
      std::vector< std::vector<float> > *m_GhostTrack_d0;
      std::vector< std::vector<float> > *m_GhostTrack_z0;

      // allTrackDetail
      std::vector< std::vector<int> > *m_GhostTrack_nPixelHits;
      std::vector< std::vector<int> > *m_GhostTrack_nSCTHits;
      std::vector< std::vector<int> > *m_GhostTrack_nTRTHits;
      std::vector< std::vector<int> > *m_GhostTrack_nPixelSharedHits;
      std::vector< std::vector<int> > *m_GhostTrack_nPixelSplitHits;
      std::vector< std::vector<int> > *m_GhostTrack_nInnermostPixelLayerHits;
      std::vector< std::vector<int> > *m_GhostTrack_nInnermostPixelLayerSharedHits;
      std::vector< std::vector<int> > *m_GhostTrack_nInnermostPixelLayerSplitHits;
      std::vector< std::vector<int> > *m_GhostTrack_nNextToInnermostPixelLayerHits;
      std::vector< std::vector<int> > *m_GhostTrack_nNextToInnermostPixelLayerSharedHits;
      std::vector< std::vector<int> > *m_GhostTrack_nNextToInnermostPixelLayerSplitHits;

      // constituent
      std::vector< int >                *m_numConstituents;
      std::vector< std::vector<float> > *m_constituentWeights;
      std::vector< std::vector<float> > *m_constituent_pt;
      std::vector< std::vector<float> > *m_constituent_eta;
      std::vector< std::vector<float> > *m_constituent_phi;
      std::vector< std::vector<float> > *m_constituent_e;


      // flavTag
      std::vector<float> *m_MV2c00;
      std::vector<float> *m_MV2c10;
      std::vector<float> *m_MV2c10mu;
      std::vector<float> *m_MV2c10rnn;
      std::vector<float> *m_MV2rmu;
      std::vector<float> *m_MV2r;
      std::vector<float> *m_MV2c20;
      std::vector<float> *m_MV2c100;
      std::vector<float> *m_DL1;
      std::vector<float> *m_DL1_pu;
      std::vector<float> *m_DL1_pc;
      std::vector<float> *m_DL1_pb;

      std::vector<float> *m_DL1mu;
      std::vector<float> *m_DL1mu_pu;
      std::vector<float> *m_DL1mu_pc;
      std::vector<float> *m_DL1mu_pb;

      std::vector<float> *m_DL1rnn;
      std::vector<float> *m_DL1rnn_pu;
      std::vector<float> *m_DL1rnn_pc;
      std::vector<float> *m_DL1rnn_pb;
      std::vector<int>   *m_HadronConeExclTruthLabelID;
      std::vector<int>   *m_HadronConeExclExtendedTruthLabelID;

      std::vector<float> *m_DL1rmu;
      std::vector<float> *m_DL1rmu_pu;
      std::vector<float> *m_DL1rmu_pc;
      std::vector<float> *m_DL1rmu_pb;

      std::vector<float> *m_DL1r;
      std::vector<float> *m_DL1r_pu;
      std::vector<float> *m_DL1r_pc;
      std::vector<float> *m_DL1r_pb;
 
      // Jet Fitter
      std::vector<float>  *m_JetFitter_nVTX           ;
      std::vector<float>  *m_JetFitter_nSingleTracks  ;
      std::vector<float>  *m_JetFitter_nTracksAtVtx   ;
      std::vector<float>  *m_JetFitter_mass           ;
      std::vector<float>  *m_JetFitter_energyFraction ;
      std::vector<float>  *m_JetFitter_significance3d ;
      std::vector<float>  *m_JetFitter_deltaeta       ;
      std::vector<float>  *m_JetFitter_deltaphi       ;
      std::vector<float>  *m_JetFitter_N2Tpar         ;


      // SV Details
      std::vector<float> *m_SV0;
      std::vector<float> *m_sv0_NGTinSvx  ;
      std::vector<float> *m_sv0_N2Tpair   ;
      std::vector<float> *m_sv0_massvx    ;
      std::vector<float> *m_sv0_efracsvx  ;
      std::vector<float> *m_sv0_normdist  ;

      std::vector<float> *m_SV1;
      std::vector<float> *m_SV1IP3D;
      std::vector<float> *m_COMBx;
      std::vector<float> *m_sv1_pu        ;
      std::vector<float> *m_sv1_pb        ;
      std::vector<float> *m_sv1_pc        ;
      std::vector<float> *m_sv1_c         ;
      std::vector<float> *m_sv1_cu        ;
      std::vector<float> *m_sv1_NGTinSvx  ;
      std::vector<float> *m_sv1_N2Tpair   ;
      std::vector<float> *m_sv1_massvx    ;
      std::vector<float> *m_sv1_efracsvx  ;
      std::vector<float> *m_sv1_normdist  ;
      std::vector<float> *m_sv1_Lxy       ;
      std::vector<float> *m_sv1_sig3d       ;
      std::vector<float> *m_sv1_L3d       ;
      std::vector<float> *m_sv1_distmatlay;
      std::vector<float> *m_sv1_dR        ;

      // IP3D
      std::vector<float> *m_IP2D_pu                   ;
      std::vector<float> *m_IP2D_pb                   ;
      std::vector<float> *m_IP2D_pc                   ;
      std::vector<float> *m_IP2D                      ;
      std::vector<float> *m_IP2D_c                    ;
      std::vector<float> *m_IP2D_cu                   ;
      std::vector<float> *m_nIP2DTracks               ;

      std::vector<std::vector<float> > *m_IP2D_gradeOfTracks        ;
      std::vector<std::vector<float> > *m_IP2D_flagFromV0ofTracks   ;
      std::vector<std::vector<float> > *m_IP2D_valD0wrtPVofTracks   ;
      std::vector<std::vector<float> > *m_IP2D_sigD0wrtPVofTracks   ;
      std::vector<std::vector<float> > *m_IP2D_weightBofTracks      ;
      std::vector<std::vector<float> > *m_IP2D_weightCofTracks      ;
      std::vector<std::vector<float> > *m_IP2D_weightUofTracks      ;

      std::vector<float> *m_IP3D_pu                   ;
      std::vector<float> *m_IP3D_pb                   ;
      std::vector<float> *m_IP3D_pc                   ;
      std::vector<float> *m_IP3D                      ;
      std::vector<float> *m_IP3D_c                    ;
      std::vector<float> *m_IP3D_cu                   ;
      std::vector<float> *m_nIP3DTracks               ;
      std::vector<std::vector<float> > *m_IP3D_gradeOfTracks        ;
      std::vector<std::vector<float> > *m_IP3D_flagFromV0ofTracks   ;
      std::vector<std::vector<float> > *m_IP3D_valD0wrtPVofTracks   ;
      std::vector<std::vector<float> > *m_IP3D_sigD0wrtPVofTracks   ;
      std::vector<std::vector<float> > *m_IP3D_valZ0wrtPVofTracks   ;
      std::vector<std::vector<float> > *m_IP3D_sigZ0wrtPVofTracks   ;
      std::vector<std::vector<float> > *m_IP3D_weightBofTracks      ;
      std::vector<std::vector<float> > *m_IP3D_weightCofTracks      ;
      std::vector<std::vector<float> > *m_IP3D_weightUofTracks      ;

      std::vector<float> *m_vtxOnlineValid;
      std::vector<float> *m_vtxHadDummy;


      std::vector<float> *m_bs_online_vx;
      std::vector<float> *m_bs_online_vy;
      std::vector<float> *m_bs_online_vz;


      std::vector<float> *m_vtx_offline_x0;
      std::vector<float> *m_vtx_offline_y0;
      std::vector<float> *m_vtx_offline_z0;

      std::vector<float> *m_vtx_online_x0;
      std::vector<float> *m_vtx_online_y0;
      std::vector<float> *m_vtx_online_z0;

      std::vector<float> *m_vtx_online_bkg_x0;
      std::vector<float> *m_vtx_online_bkg_y0;
      std::vector<float> *m_vtx_online_bkg_z0;

      struct btagOpPoint {
        bool m_mc;
        std::string m_accessorName;
	Jet::BTaggerOP m_op;
	bool m_old;

	// branches
        std::vector<int>*                  m_isTag;
        std::vector< std::vector<float> >* m_sf;

        btagOpPoint(bool mc, const std::string& accessorName)
	  : m_mc(mc), m_accessorName(accessorName),m_old(true) {
          m_isTag = new std::vector<int>();
          m_sf    = new std::vector< std::vector<float> >();

	  if(m_accessorName=="Fix30")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_30;
	  else if(m_accessorName=="Fix50")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_50;
	  else if(m_accessorName=="Fix60")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_60;
	  else if(m_accessorName=="Fix70")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_70;
	  else if(m_accessorName=="Fix77")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_77;
	  else if(m_accessorName=="Fix85")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_85;
	  else if(m_accessorName=="Fix90")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_90;
	  else if(m_accessorName=="Flt30")
	    m_op=Jet::BTaggerOP::MV2c10_FlatBEff_30;
	  else if(m_accessorName=="Flt50")
	    m_op=Jet::BTaggerOP::MV2c10_FlatBEff_50;
	  else if(m_accessorName=="Flt60")
	    m_op=Jet::BTaggerOP::MV2c10_FlatBEff_60;
	  else if(m_accessorName=="Flt70")
	    m_op=Jet::BTaggerOP::MV2c10_FlatBEff_70;
	  else if(m_accessorName=="Flt77")
	    m_op=Jet::BTaggerOP::MV2c10_FlatBEff_77;
	  else if(m_accessorName=="Flt85")
	    m_op=Jet::BTaggerOP::MV2c10_FlatBEff_85;
	  else if(m_accessorName=="Hyb60")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_60;
	  else if(m_accessorName=="Hyb70")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_70;
	  else if(m_accessorName=="Hyb77")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_77;
	  else if(m_accessorName=="Hyb85")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_85;
        }

        btagOpPoint(bool mc, const std::string& tagger, const std::string& wp)
	  : m_mc(mc), m_accessorName(tagger+"_"+wp),m_old(false) {
          m_isTag     = new std::vector<int>();
          m_sf        = new std::vector< std::vector<float> >();

	  if(m_accessorName=="DL1rnn_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1rnn_FixedCutBEff_60;
	  else if(m_accessorName=="DL1rnn_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1rnn_FixedCutBEff_70;
	  else if(m_accessorName=="DL1rnn_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1rnn_FixedCutBEff_77;
	  else if(m_accessorName=="DL1rnn_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1rnn_FixedCutBEff_85;
	  else if(m_accessorName=="DL1rnn_HybBEff_60")
	    m_op=Jet::BTaggerOP::DL1rnn_HybBEff_60;
	  else if(m_accessorName=="DL1rnn_HybBEff_70")
	    m_op=Jet::BTaggerOP::DL1rnn_HybBEff_70;
	  else if(m_accessorName=="DL1rnn_HybBEff_77")
	    m_op=Jet::BTaggerOP::DL1rnn_HybBEff_77;
	  else if(m_accessorName=="DL1rnn_HybBEff_85")
	    m_op=Jet::BTaggerOP::DL1rnn_HybBEff_85;
	  else if(m_accessorName=="DL1mu_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1mu_FixedCutBEff_60;
	  else if(m_accessorName=="DL1mu_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1mu_FixedCutBEff_70;
	  else if(m_accessorName=="DL1mu_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1mu_FixedCutBEff_77;
	  else if(m_accessorName=="DL1mu_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1mu_FixedCutBEff_85;
	  else if(m_accessorName=="DL1mu_HybBEff_60")
	    m_op=Jet::BTaggerOP::DL1mu_HybBEff_60;
	  else if(m_accessorName=="DL1mu_HybBEff_70")
	    m_op=Jet::BTaggerOP::DL1mu_HybBEff_70;
	  else if(m_accessorName=="DL1mu_HybBEff_77")
	    m_op=Jet::BTaggerOP::DL1mu_HybBEff_77;
	  else if(m_accessorName=="DL1mu_HybBEff_85")
	    m_op=Jet::BTaggerOP::DL1mu_HybBEff_85;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_60;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_70;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_77;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_85;
	  else if(m_accessorName=="DL1rmu_HybBEff_60")
	    m_op=Jet::BTaggerOP::DL1rmu_HybBEff_60;
	  else if(m_accessorName=="DL1rmu_HybBEff_70")
	    m_op=Jet::BTaggerOP::DL1rmu_HybBEff_70;
	  else if(m_accessorName=="DL1rmu_HybBEff_77")
	    m_op=Jet::BTaggerOP::DL1rmu_HybBEff_77;
	  else if(m_accessorName=="DL1rmu_HybBEff_85")
	    m_op=Jet::BTaggerOP::DL1rmu_HybBEff_85;
	  else if(m_accessorName=="DL1r_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_60;
	  else if(m_accessorName=="DL1r_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_70;
	  else if(m_accessorName=="DL1r_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_77;
	  else if(m_accessorName=="DL1r_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_85;
	  else if(m_accessorName=="DL1r_HybBEff_60")
	    m_op=Jet::BTaggerOP::DL1r_HybBEff_60;
	  else if(m_accessorName=="DL1r_HybBEff_70")
	    m_op=Jet::BTaggerOP::DL1r_HybBEff_70;
	  else if(m_accessorName=="DL1r_HybBEff_77")
	    m_op=Jet::BTaggerOP::DL1r_HybBEff_77;
	  else if(m_accessorName=="DL1r_HybBEff_85")
	    m_op=Jet::BTaggerOP::DL1r_HybBEff_85;
	  else if(m_accessorName=="DL1_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_60;
	  else if(m_accessorName=="DL1_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_70;
	  else if(m_accessorName=="DL1_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_77;
	  else if(m_accessorName=="DL1_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_85;
	  else if(m_accessorName=="DL1_HybBEff_60")
	    m_op=Jet::BTaggerOP::DL1_HybBEff_60;
	  else if(m_accessorName=="DL1_HybBEff_70")
	    m_op=Jet::BTaggerOP::DL1_HybBEff_70;
	  else if(m_accessorName=="DL1_HybBEff_77")
	    m_op=Jet::BTaggerOP::DL1_HybBEff_77;
	  else if(m_accessorName=="DL1_HybBEff_85")
	    m_op=Jet::BTaggerOP::DL1_HybBEff_85;
	  else if(m_accessorName=="MV2c10rnn_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::MV2c10rnn_FixedCutBEff_60;
	  else if(m_accessorName=="MV2c10rnn_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::MV2c10rnn_FixedCutBEff_70;
	  else if(m_accessorName=="MV2c10rnn_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::MV2c10rnn_FixedCutBEff_77;
	  else if(m_accessorName=="MV2c10rnn_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::MV2c10rnn_FixedCutBEff_85;
	  else if(m_accessorName=="MV2c10rnn_HybBEff_60")
	    m_op=Jet::BTaggerOP::MV2c10rnn_HybBEff_60;
	  else if(m_accessorName=="MV2c10rnn_HybBEff_70")
	    m_op=Jet::BTaggerOP::MV2c10rnn_HybBEff_70;
	  else if(m_accessorName=="MV2c10rnn_HybBEff_77")
	    m_op=Jet::BTaggerOP::MV2c10rnn_HybBEff_77;
	  else if(m_accessorName=="MV2c10rnn_HybBEff_85")
	    m_op=Jet::BTaggerOP::MV2c10rnn_HybBEff_85;
	  else if(m_accessorName=="MV2c10mu_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::MV2c10mu_FixedCutBEff_60;
	  else if(m_accessorName=="MV2c10mu_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::MV2c10mu_FixedCutBEff_70;
	  else if(m_accessorName=="MV2c10mu_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::MV2c10mu_FixedCutBEff_77;
	  else if(m_accessorName=="MV2c10mu_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::MV2c10mu_FixedCutBEff_85;
	  else if(m_accessorName=="MV2c10mu_HybBEff_60")
	    m_op=Jet::BTaggerOP::MV2c10mu_HybBEff_60;
	  else if(m_accessorName=="MV2c10mu_HybBEff_70")
	    m_op=Jet::BTaggerOP::MV2c10mu_HybBEff_70;
	  else if(m_accessorName=="MV2c10mu_HybBEff_77")
	    m_op=Jet::BTaggerOP::MV2c10mu_HybBEff_77;
	  else if(m_accessorName=="MV2c10mu_HybBEff_85")
	    m_op=Jet::BTaggerOP::MV2c10mu_HybBEff_85;
          else if(m_accessorName=="MV2r_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::MV2r_FixedCutBEff_60;
	  else if(m_accessorName=="MV2r_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::MV2r_FixedCutBEff_70;
	  else if(m_accessorName=="MV2r_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::MV2r_FixedCutBEff_77;
	  else if(m_accessorName=="MV2r_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::MV2r_FixedCutBEff_85;
	  else if(m_accessorName=="MV2r_HybBEff_60")
	    m_op=Jet::BTaggerOP::MV2r_HybBEff_60;
	  else if(m_accessorName=="MV2r_HybBEff_70")
	    m_op=Jet::BTaggerOP::MV2r_HybBEff_70;
	  else if(m_accessorName=="MV2r_HybBEff_77")
	    m_op=Jet::BTaggerOP::MV2r_HybBEff_77;
	  else if(m_accessorName=="MV2r_HybBEff_85")
	    m_op=Jet::BTaggerOP::MV2r_HybBEff_85;
	  else if(m_accessorName=="MV2rmu_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::MV2rmu_FixedCutBEff_60;
	  else if(m_accessorName=="MV2rmu_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::MV2rmu_FixedCutBEff_70;
	  else if(m_accessorName=="MV2rmu_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::MV2rmu_FixedCutBEff_77;
	  else if(m_accessorName=="MV2rmu_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::MV2rmu_FixedCutBEff_85;
	  else if(m_accessorName=="MV2rmu_HybBEff_60")
	    m_op=Jet::BTaggerOP::MV2rmu_HybBEff_60;
	  else if(m_accessorName=="MV2rmu_HybBEff_70")
	    m_op=Jet::BTaggerOP::MV2rmu_HybBEff_70;
	  else if(m_accessorName=="MV2rmu_HybBEff_77")
	    m_op=Jet::BTaggerOP::MV2rmu_HybBEff_77;
	  else if(m_accessorName=="MV2rmu_HybBEff_85")
	    m_op=Jet::BTaggerOP::MV2rmu_HybBEff_85;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_30")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_30;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_50")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_50;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_60;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_70;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_77;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_85;
	  else if(m_accessorName=="MV2c10_HybBEff_60")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_60;
	  else if(m_accessorName=="MV2c10_HybBEff_70")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_70;
	  else if(m_accessorName=="MV2c10_HybBEff_77")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_77;
	  else if(m_accessorName=="MV2c10_HybBEff_85")
	    m_op=Jet::BTaggerOP::MV2c10_HybBEff_85;
        }

        ~btagOpPoint() {
          delete m_isTag;
          delete m_sf;
        }

        void setTree(TTree *tree, std::string jetName) {
	  if(m_old)
	    {
	      HelperFunctions::connectBranch<int>                  (jetName, tree,"is"+m_accessorName, &m_isTag);
	      if(m_mc) {
		HelperFunctions::connectBranch<std::vector<float> >(jetName, tree,"SF"+m_accessorName, &m_sf);
	      }
	    }
	  else
	    {
	      HelperFunctions::connectBranch<int>                  (jetName, tree,"is_"+m_accessorName, &m_isTag);
	      if(m_mc) {
		HelperFunctions::connectBranch<std::vector<float> >(jetName, tree,"SF_"+m_accessorName, &m_sf);
	      }
	    }
        }


        void setBranch(TTree *tree, std::string jetName) {
	  if(m_old) return; // DEPRICATED
          tree->Branch((jetName+"_is_"+m_accessorName).c_str(),   &m_isTag);

          if ( m_mc ) {
            tree->Branch((jetName+"_SF_"+m_accessorName).c_str(),           &m_sf);
          }
        }


        void clear() {
	  if(m_old) return; // DEPRICATED
          m_isTag->clear();
          m_sf->clear();
        }

        void Fill( const xAOD::Jet* jet ) {
	  if(m_old) return; // DEPRICATED
          SG::AuxElement::ConstAccessor< char > isTag("BTag_"+m_accessorName);
          if( isTag.isAvailable( *jet ) )
            m_isTag->push_back( isTag( *jet ) );
          else
            m_isTag->push_back( -1 );

          if(!m_mc) { return; }
          SG::AuxElement::ConstAccessor< std::vector<float> > sf("BTag_SF_"+m_accessorName);
	  static const std::vector<float> junk(1,-999);
          if ( sf.isAvailable( *jet ) )
            m_sf->push_back( sf( *jet ) );
          else
            m_sf->push_back(junk);
        } // Fill
      };

      std::vector<btagOpPoint*> m_btags;

      // JVC
      std::vector<double> *m_JetVertexCharge_discriminant;

      // area
      std::vector<float> *m_GhostArea;
      std::vector<float> *m_ActiveArea;
      std::vector<float> *m_VoronoiArea;
      std::vector<float> *m_ActiveArea4vec_pt;
      std::vector<float> *m_ActiveArea4vec_eta;
      std::vector<float> *m_ActiveArea4vec_phi;
      std::vector<float> *m_ActiveArea4vec_m;

      // truth
      std::vector<int>   *m_ConeTruthLabelID;
      std::vector<int>   *m_TruthCount;
      std::vector<float> *m_TruthLabelDeltaR_B;
      std::vector<float> *m_TruthLabelDeltaR_C;
      std::vector<float> *m_TruthLabelDeltaR_T;
      std::vector<int>   *m_PartonTruthLabelID;
      std::vector<float> *m_GhostTruthAssociationFraction;
      std::vector<float> *m_truth_E;
      std::vector<float> *m_truth_pt;
      std::vector<float> *m_truth_phi;
      std::vector<float> *m_truth_eta;


      // truth detail
      std::vector<int>   *m_GhostBHadronsFinalCount;
      std::vector<int>   *m_GhostBHadronsInitialCount;
      std::vector<int>   *m_GhostBQuarksFinalCount;
      std::vector<float> *m_GhostBHadronsFinalPt;
      std::vector<float> *m_GhostBHadronsInitialPt;
      std::vector<float> *m_GhostBQuarksFinalPt;

      std::vector<int>   *m_GhostCHadronsFinalCount;
      std::vector<int>   *m_GhostCHadronsInitialCount;
      std::vector<int>   *m_GhostCQuarksFinalCount;
      std::vector<float> *m_GhostCHadronsFinalPt;
      std::vector<float> *m_GhostCHadronsInitialPt;
      std::vector<float> *m_GhostCQuarksFinalPt;

      std::vector<int>   *m_GhostTausFinalCount;
      std::vector<float> *m_GhostTausFinalPt;

      std::vector<int>   *m_truth_pdgId;
      std::vector<float> *m_truth_partonPt;
      std::vector<float> *m_truth_partonDR;

      // charge
      std::vector<double> *m_charge;
    };
}



#endif // xAODAnaHelpers_JetContainer_H
