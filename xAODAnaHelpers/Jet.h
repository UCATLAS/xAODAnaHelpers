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
	DL1dv01_FixedCutBEff_60  , DL1dv01_FixedCutBEff_70  , DL1dv01_FixedCutBEff_77  , DL1dv01_FixedCutBEff_85  ,
	GN120220509_FixedCutBEff_60  , GN120220509_FixedCutBEff_70  , GN120220509_FixedCutBEff_77  , GN120220509_FixedCutBEff_85  ,
	DL1dv00_Continuous, DL1r_Continuous, DL1dv01_Continuous, GN120220509_Continuous , // Continuous
  GN2v00LegacyWP_FixedCutBEff_60  , GN2v00LegacyWP_FixedCutBEff_70  , GN2v00LegacyWP_FixedCutBEff_77  , GN2v00LegacyWP_FixedCutBEff_85  ,
  GN2v00NewAliasWP_FixedCutBEff_60  , GN2v00NewAliasWP_FixedCutBEff_70  , GN2v00NewAliasWP_FixedCutBEff_77  , GN2v00NewAliasWP_FixedCutBEff_85  ,
  GN2v01_FixedCutBEff_65  , GN2v01_FixedCutBEff_70  , GN2v01_FixedCutBEff_77  , GN2v01_FixedCutBEff_85  , GN2v01_FixedCutBEff_90  ,
    GN2v01_Continuous , // Continuous 
      };

      float rapidity;

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
      float DL1dv01;
      float DL1dv01_pu;
      float DL1dv01_pc;
      float DL1dv01_pb;
      float fastDIPS;
      float fastDIPS_pu;
      float fastDIPS_pc;
      float fastDIPS_pb;
      float GN1;
      float GN1_pu;
      float GN1_pc;
      float GN1_pb;
      float GN2v00LegacyWP;
      float GN2v00LegacyWP_pu;
      float GN2v00LegacyWP_pc;
      float GN2v00LegacyWP_pb;
      float GN2v00NewAliasWP;
      float GN2v00NewAliasWP_pu;
      float GN2v00NewAliasWP_pc;
      float GN2v00NewAliasWP_pb;
      float GN2v01;
      float GN2v01_pu;
      float GN2v01_pc;
      float GN2v01_pb;
      float GN2v01_ptau;
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
      int is_DL1dv01_FixedCutBEff_60;
      std::vector<float> SF_DL1dv01_FixedCutBEff_60;
      int is_DL1dv01_FixedCutBEff_70;
      std::vector<float> SF_DL1dv01_FixedCutBEff_70;
      int is_DL1dv01_FixedCutBEff_77;
      std::vector<float> SF_DL1dv01_FixedCutBEff_77;
      int is_DL1dv01_FixedCutBEff_85;
      std::vector<float> SF_DL1dv01_FixedCutBEff_85;
      int is_GN120220509_FixedCutBEff_60;
      std::vector<float> SF_GN120220509_FixedCutBEff_60;
      int is_GN120220509_FixedCutBEff_70;
      std::vector<float> SF_GN120220509_FixedCutBEff_70;
      int is_GN120220509_FixedCutBEff_77;
      std::vector<float> SF_GN120220509_FixedCutBEff_77;
      int is_GN120220509_FixedCutBEff_85;
      std::vector<float> SF_GN120220509_FixedCutBEff_85;
      int is_GN2v00LegacyWP_FixedCutBEff_60;
      std::vector<float> SF_GN2v00LegacyWP_FixedCutBEff_60;
      int is_GN2v00LegacyWP_FixedCutBEff_70;
      std::vector<float> SF_GN2v00LegacyWP_FixedCutBEff_70;
      int is_GN2v00LegacyWP_FixedCutBEff_77;
      std::vector<float> SF_GN2v00LegacyWP_FixedCutBEff_77;
      int is_GN2v00LegacyWP_FixedCutBEff_85;
      std::vector<float> SF_GN2v00LegacyWP_FixedCutBEff_85;
      int is_GN2v00NewAliasWP_FixedCutBEff_60;
      std::vector<float> SF_GN2v00NewAliasWP_FixedCutBEff_60;
      int is_GN2v00NewAliasWP_FixedCutBEff_70;
      std::vector<float> SF_GN2v00NewAliasWP_FixedCutBEff_70;
      int is_GN2v00NewAliasWP_FixedCutBEff_77;
      std::vector<float> SF_GN2v00NewAliasWP_FixedCutBEff_77;
      int is_GN2v00NewAliasWP_FixedCutBEff_85;
      std::vector<float> SF_GN2v00NewAliasWP_FixedCutBEff_85;
      int is_GN2v01_FixedCutBEff_65;
      std::vector<float> SF_GN2v01_FixedCutBEff_65;
      int is_GN2v01_FixedCutBEff_70;
      std::vector<float> SF_GN2v01_FixedCutBEff_70;
      int is_GN2v01_FixedCutBEff_77;
      std::vector<float> SF_GN2v01_FixedCutBEff_77;
      int is_GN2v01_FixedCutBEff_85;
      std::vector<float> SF_GN2v01_FixedCutBEff_85;
      int is_GN2v01_FixedCutBEff_90;
      std::vector<float> SF_GN2v01_FixedCutBEff_90;

      // Continuous
      int is_DL1r_Continuous;
      std::vector<float> SF_DL1r_Continuous;
      std::vector<float> inEffSF_DL1r_Continuous;
      int is_DL1dv00_Continuous;
      std::vector<float> SF_DL1dv00_Continuous;
      std::vector<float> inEffSF_DL1dv00_Continuous;
      int is_DL1dv01_Continuous;
      std::vector<float> SF_DL1dv01_Continuous;
      std::vector<float> inEffSF_DL1dv01_Continuous;
      int is_GN120220509_Continuous;
      std::vector<float> SF_GN120220509_Continuous;
      std::vector<float> inEffSF_GN120220509_Continuous;
      int is_GN2v00LegacyWP_Continuous;
      std::vector<float> SF_GN2v00LegacyWP_Continuous;
      std::vector<float> inEffSF_GN2v00LegacyWP_Continuous;
      int is_GN2v00NewAliasWP_Continuous;
      std::vector<float> SF_GN2v00NewAliasWP_Continuous;
      std::vector<float> inEffSF_GN2v00NewAliasWP_Continuous;
      int is_GN2v01_Continuous;
      std::vector<float> SF_GN2v01_Continuous;
      std::vector<float> inEffSF_GN2v01_Continuous;

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

      // passOR
      char passOR;

      const Muon* matchedMuon =nullptr; //!
      const Jet * matchedJet  =nullptr; //!

    public:
      int is_btag(BTaggerOP op) const;
      const std::vector<float>& SF_btag(BTaggerOP op) const;

      void muonInJetCorrection(const xAH::MuonContainer* muons);

    };

}//xAH
#endif // xAODAnaHelpers_Particle_H
