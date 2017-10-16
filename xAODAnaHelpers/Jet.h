#ifndef xAODAnaHelpers_Jet_H
#define xAODAnaHelpers_Jet_H

#include "xAODAnaHelpers/Particle.h"
#include "xAODAnaHelpers/MuonContainer.h"


namespace xAH {

  class Jet : public Particle
    {
    public:
      
      Jet();
      float rapidity;

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
      float OotFracClusters5;
      float OotFracClusters10;
      float LeadingClusterPt;
      float LeadingClusterSecondLambda;
      float LeadingClusterCenterLambda;
      float LeadingClusterSecondR;
      int   clean_passLooseBad;
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
    
      //JVC
      float JVC;

      // flavTag
      float SV0;
      float SV1;
      float IP3D;
      float SV1IP3D;
      float COMBx;
      float MV1;
      float MV2c00;
      float MV2c10;
      float MV2c20;
      float MV2c100;
      float MV2;
      int  HadronConeExclTruthLabelID;

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
      int is_DL1rnn_FixedCutBEff_60;
      std::vector<float> SF_DL1rnn_FixedCutBEff_60;
      int is_DL1rnn_FixedCutBEff_70;
      std::vector<float> SF_DL1rnn_FixedCutBEff_70;
      int is_DL1rnn_FixedCutBEff_77;
      std::vector<float> SF_DL1rnn_FixedCutBEff_77;
      int is_DL1rnn_FixedCutBEff_85;
      std::vector<float> SF_DL1rnn_FixedCutBEff_85;

      int is_DL1rnn_HybBEff_60;
      std::vector<float> SF_DL1rnn_HybBEff_60;
      int is_DL1rnn_HybBEff_70;
      std::vector<float> SF_DL1rnn_HybBEff_70;
      int is_DL1rnn_HybBEff_77;
      std::vector<float> SF_DL1rnn_HybBEff_77;
      int is_DL1rnn_HybBEff_85;
      std::vector<float> SF_DL1rnn_HybBEff_85;

      int is_DL1mu_FixedCutBEff_60;
      std::vector<float> SF_DL1mu_FixedCutBEff_60;
      int is_DL1mu_FixedCutBEff_70;
      std::vector<float> SF_DL1mu_FixedCutBEff_70;
      int is_DL1mu_FixedCutBEff_77;
      std::vector<float> SF_DL1mu_FixedCutBEff_77;
      int is_DL1mu_FixedCutBEff_85;
      std::vector<float> SF_DL1mu_FixedCutBEff_85;

      int is_DL1mu_HybBEff_60;
      std::vector<float> SF_DL1mu_HybBEff_60;
      int is_DL1mu_HybBEff_70;
      std::vector<float> SF_DL1mu_HybBEff_70;
      int is_DL1mu_HybBEff_77;
      std::vector<float> SF_DL1mu_HybBEff_77;
      int is_DL1mu_HybBEff_85;
      std::vector<float> SF_DL1mu_HybBEff_85;

      int is_DL1_FixedCutBEff_60;
      std::vector<float> SF_DL1_FixedCutBEff_60;
      int is_DL1_FixedCutBEff_70;
      std::vector<float> SF_DL1_FixedCutBEff_70;
      int is_DL1_FixedCutBEff_77;
      std::vector<float> SF_DL1_FixedCutBEff_77;
      int is_DL1_FixedCutBEff_85;
      std::vector<float> SF_DL1_FixedCutBEff_85;

      int is_DL1_HybBEff_60;
      std::vector<float> SF_DL1_HybBEff_60;
      int is_DL1_HybBEff_70;
      std::vector<float> SF_DL1_HybBEff_70;
      int is_DL1_HybBEff_77;
      std::vector<float> SF_DL1_HybBEff_77;
      int is_DL1_HybBEff_85;
      std::vector<float> SF_DL1_HybBEff_85;

      int is_MV2c10rnn_FixedCutBEff_60;
      std::vector<float> SF_MV2c10rnn_FixedCutBEff_60;
      int is_MV2c10rnn_FixedCutBEff_70;
      std::vector<float> SF_MV2c10rnn_FixedCutBEff_70;
      int is_MV2c10rnn_FixedCutBEff_77;
      std::vector<float> SF_MV2c10rnn_FixedCutBEff_77;
      int is_MV2c10rnn_FixedCutBEff_85;
      std::vector<float> SF_MV2c10rnn_FixedCutBEff_85;

      int is_MV2c10rnn_HybBEff_60;
      std::vector<float> SF_MV2c10rnn_HybBEff_60;
      int is_MV2c10rnn_HybBEff_70;
      std::vector<float> SF_MV2c10rnn_HybBEff_70;
      int is_MV2c10rnn_HybBEff_77;
      std::vector<float> SF_MV2c10rnn_HybBEff_77;
      int is_MV2c10rnn_HybBEff_85;
      std::vector<float> SF_MV2c10rnn_HybBEff_85;

      int is_MV2c10mu_FixedCutBEff_60;
      std::vector<float> SF_MV2c10mu_FixedCutBEff_60;
      int is_MV2c10mu_FixedCutBEff_70;
      std::vector<float> SF_MV2c10mu_FixedCutBEff_70;
      int is_MV2c10mu_FixedCutBEff_77;
      std::vector<float> SF_MV2c10mu_FixedCutBEff_77;
      int is_MV2c10mu_FixedCutBEff_85;
      std::vector<float> SF_MV2c10mu_FixedCutBEff_85;

      int is_MV2c10mu_HybBEff_60;
      std::vector<float> SF_MV2c10mu_HybBEff_60;
      int is_MV2c10mu_HybBEff_70;
      std::vector<float> SF_MV2c10mu_HybBEff_70;
      int is_MV2c10mu_HybBEff_77;
      std::vector<float> SF_MV2c10mu_HybBEff_77;
      int is_MV2c10mu_HybBEff_85;
      std::vector<float> SF_MV2c10mu_HybBEff_85;

      int is_MV2c10_FixedCutBEff_30;
      std::vector<float> SF_MV2c10_FixedCutBEff_30;
      int is_MV2c10_FixedCutBEff_50;
      std::vector<float> SF_MV2c10_FixedCutBEff_50;
      int is_MV2c10_FixedCutBEff_60;
      std::vector<float> SF_MV2c10_FixedCutBEff_60;
      int is_MV2c10_FixedCutBEff_70;
      std::vector<float> SF_MV2c10_FixedCutBEff_70;
      int is_MV2c10_FixedCutBEff_77;
      std::vector<float> SF_MV2c10_FixedCutBEff_77;
      int is_MV2c10_FixedCutBEff_85;
      std::vector<float> SF_MV2c10_FixedCutBEff_85;

      int is_MV2c10_HybBEff_60;
      std::vector<float> SF_MV2c10_HybBEff_60;
      int is_MV2c10_HybBEff_70;
      std::vector<float> SF_MV2c10_HybBEff_70;
      int is_MV2c10_HybBEff_77;
      std::vector<float> SF_MV2c10_HybBEff_77;
      int is_MV2c10_HybBEff_85;
      std::vector<float> SF_MV2c10_HybBEff_85;

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

      const Muon* matchedMuon;
      const Jet * matchedJet;

    public:

      void muonInJetCorrection(const xAH::MuonContainer* muons);

    };

}//xAH
#endif // xAODAnaHelpers_Particle_H
