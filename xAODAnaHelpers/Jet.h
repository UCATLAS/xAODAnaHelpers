#ifndef xAODAnaHelpers_Jet_H
#define xAODAnaHelpers_Jet_H

#include "xAODAnaHelpers/Particle.h"
#include "xAODAnaHelpers/MuonContainer.h"


namespace xAH {

  class Jet : public Particle
    {
    public:
      enum BTaggerOP {
	None,
	DL1r_FixedCutBEff_60  , DL1r_FixedCutBEff_70  , DL1r_FixedCutBEff_77  , DL1r_FixedCutBEff_85  ,
	DL1dv00_FixedCutBEff_60  , DL1dv00_FixedCutBEff_70  , DL1dv00_FixedCutBEff_77  , DL1dv00_FixedCutBEff_85  ,
	DL1dv00_Continuous, DL1r_Continuous, // Continuous
      };

      float rapidity;


      // displaced
      float ipsig;
      float ptrel;
      float ptrel_highest;
      float dchf;
      float chf;
      float alpha_max;	
      float maxd0Value;	
      float mind0Value;



      // trigger
      int               isTrigMatched;
      std::vector<int>  isTrigMatchedToChain;
      std::string       listTrigChains;
      
      // clean
      float Timing;
      float LArQuality;
      float HECQuality;
      float NegativeE;
      float AverageLArQF;
      float BchCorrCell;
      float N90Constituents;
      float LArBadHVEFrac;
      int   LArBadHVNCell;
      float ChargedFraction;
      float OotFracClusters5;
      float OotFracClusters10;
      float LeadingClusterPt;
      float LeadingClusterSecondLambda;
      float LeadingClusterCenterLambda;
      float LeadingClusterSecondR;
      int   clean_passLooseBad;
      int   clean_passLooseBadLLP;
      int   clean_passLooseBadTrigger;
      int   clean_passLooseBadTriggerUgly;
      int   clean_passLooseBadUgly;
      int   clean_passTightBad;
      int   clean_passTightBadUgly;
    
      // energy
      float HECFrac;
      float EMFrac;
      float CentroidR;
      float FracSamplingMax;
      float FracSamplingMaxIndex;
      float LowEtConstituentsFrac;
      float GhostMuonSegmentCount;
      float Width;
    
      // trackPV
      float NumTrkPt1000PV;
      float SumPtTrkPt1000PV;
      float TrackWidthPt1000PV;
      float NumTrkPt500PV;
      float SumPtTrkPt500PV;
      float TrackWidthPt500PV;
      float JVFPV;
    
      // trackAll or trackPV
      float Jvt;
      float JvtJvfcorr;
      float JvtRpt;

      // allTrack
      int   GhostTrackCount;
      float GhostTrackPt;
      std::vector<float> GhostTrack_pt;
      std::vector<float> GhostTrack_qOverP;
      std::vector<float> GhostTrack_eta;
      std::vector<float> GhostTrack_phi;
      std::vector<float> GhostTrack_e;
      std::vector<float> GhostTrack_d0;
      std::vector<float> GhostTrack_z0;

      // allTrackDetail
      std::vector<int> GhostTrack_nPixelHits;
      std::vector<int> GhostTrack_nSCTHits;
      std::vector<int> GhostTrack_nTRTHits;
      std::vector<int> GhostTrack_nPixelSharedHits;
      std::vector<int> GhostTrack_nPixelSplitHits;
      std::vector<int> GhostTrack_nInnermostPixelLayerHits;
      std::vector<int> GhostTrack_nInnermostPixelLayerSharedHits;
      std::vector<int> GhostTrack_nInnermostPixelLayerSplitHits;
      std::vector<int> GhostTrack_nNextToInnermostPixelLayerHits;
      std::vector<int> GhostTrack_nNextToInnermostPixelLayerSharedHits;
      std::vector<int> GhostTrack_nNextToInnermostPixelLayerSplitHits;

      // chargedPFOPV 
      float SumPtChargedPFOPt500PV;
      float fCharged;
    
      //JVC
      float JVC;

      // flavTag
      float SV0;
      float SV1;
      float IP3D;
      float SV1IP3D;
      float COMBx;
      float DL1r;
      float DL1r_pu;
      float DL1r_pc;
      float DL1r_pb;
      float DL1dv00;
      float DL1dv00_pu;
      float DL1dv00_pc;
      float DL1dv00_pb;
      int  HadronConeExclTruthLabelID;
      int  HadronConeExclExtendedTruthLabelID;

      float vtxOnlineValid;
      float vtxHadDummy;
      
      float bs_online_vx;
      float bs_online_vy;
      float bs_online_vz;

      float vtx_offline_x0;
      float vtx_offline_y0;
      float vtx_offline_z0;
    
      float vtx_online_x0;
      float vtx_online_y0;
      float vtx_online_z0;

      float vtx_online_bkg_x0;
      float vtx_online_bkg_y0;
      float vtx_online_bkg_z0;
    
      float JetFitter_nVTX           ;
      float JetFitter_nSingleTracks  ;
      float JetFitter_nTracksAtVtx   ;
      float JetFitter_mass           ;
      float JetFitter_energyFraction ;
      float JetFitter_significance3d ;
      float JetFitter_deltaeta       ;
      float JetFitter_deltaphi       ;
      float JetFitter_N2Tpar         ;
    
      float sv0_NGTinSvx  ;
      float sv0_N2Tpair   ;
      float sv0_massvx    ;
      float sv0_efracsvx  ;
      float sv0_normdist  ;
      float sv1_pu        ;
      float sv1_pb        ;
      float sv1_pc        ;
      float sv1_c         ;
      float sv1_cu        ;
      float sv1_NGTinSvx  ;
      float sv1_N2Tpair   ;
      float sv1_massvx    ;
      float sv1_efracsvx  ;
      float sv1_normdist  ;
      float sv1_Lxy       ;
      float sv1_sig3d     ;
      float sv1_L3d       ;
      float sv1_distmatlay;
      float sv1_dR        ;
    
      float IP2D_pu     ;
      float IP2D_pb     ;
      float IP2D_pc     ;
      float IP2D        ;
      float IP2D_c      ;
      float IP2D_cu     ;
      float nIP2DTracks ;
    
      std::vector<float> IP2D_gradeOfTracks         ;
      std::vector<float> IP2D_flagFromV0ofTracks    ;
      std::vector<float> IP2D_valD0wrtPVofTracks    ;
      std::vector<float> IP2D_sigD0wrtPVofTracks    ;
      std::vector<float> IP2D_weightBofTracks       ;
      std::vector<float> IP2D_weightCofTracks       ;
      std::vector<float> IP2D_weightUofTracks       ;
    
      float IP3D_pu     ;
      float IP3D_pb     ;
      float IP3D_pc     ;
      float IP3D_c      ;
      float IP3D_cu     ;
      float nIP3DTracks ;
    
      std::vector<float> IP3D_gradeOfTracks      ;
      std::vector<float> IP3D_flagFromV0ofTracks ;
      std::vector<float> IP3D_valD0wrtPVofTracks ;
      std::vector<float> IP3D_sigD0wrtPVofTracks ;
      std::vector<float> IP3D_valZ0wrtPVofTracks ;
      std::vector<float> IP3D_sigZ0wrtPVofTracks ;
      std::vector<float> IP3D_weightBofTracks    ;
      std::vector<float> IP3D_weightCofTracks    ;
      std::vector<float> IP3D_weightUofTracks    ;

      // jetBTag
      
      int is_DL1r_FixedCutBEff_60;
      std::vector<float> SF_DL1r_FixedCutBEff_60;
      int is_DL1r_FixedCutBEff_70;
      std::vector<float> SF_DL1r_FixedCutBEff_70;
      int is_DL1r_FixedCutBEff_77;
      std::vector<float> SF_DL1r_FixedCutBEff_77;
      int is_DL1r_FixedCutBEff_85;
      std::vector<float> SF_DL1r_FixedCutBEff_85;
      int is_DL1dv00_FixedCutBEff_60;
      std::vector<float> SF_DL1dv00_FixedCutBEff_60;
      int is_DL1dv00_FixedCutBEff_70;
      std::vector<float> SF_DL1dv00_FixedCutBEff_70;
      int is_DL1dv00_FixedCutBEff_77;
      std::vector<float> SF_DL1dv00_FixedCutBEff_77;
      int is_DL1dv00_FixedCutBEff_85;
      std::vector<float> SF_DL1dv00_FixedCutBEff_85;

      // Continuous
      int is_DL1r_Continuous;
      std::vector<float> SF_DL1r_Continuous;
      std::vector<float> inEffSF_DL1r_Continuous;
      int is_DL1dv00_Continuous;
      std::vector<float> SF_DL1dv00_Continuous;
      std::vector<float> inEffSF_DL1dv00_Continuous;


      // truth
      int   ConeTruthLabelID;
      int   TruthCount;
      float TruthLabelDeltaR_B;
      float TruthLabelDeltaR_C;
      float TruthLabelDeltaR_T;
      int   PartonTruthLabelID;
      float GhostTruthAssociationFraction;
      TLorentzVector truth_p4;

      // charge
      double charge;

      // passSel
      char passSel;

      const Muon* matchedMuon =nullptr; //!
      const Jet * matchedJet  =nullptr; //!

    public:
      int is_btag(BTaggerOP op) const;
      const std::vector<float>& SF_btag(BTaggerOP op) const;

      void muonInJetCorrection(const xAH::MuonContainer* muons);

    };

}//xAH
#endif // xAODAnaHelpers_Particle_H
