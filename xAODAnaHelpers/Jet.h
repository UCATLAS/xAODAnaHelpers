#ifndef xAODAnaHelpers_Jet_H
#define xAODAnaHelpers_Jet_H

#include "xAODAnaHelpers/Particle.h"
#include "xAODAnaHelpers/MuonContainer.h"


namespace xAH {

  class Jet : public Particle
    {
    public:
      
      //Jet() { };
      //virtual ~Jet();

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
    
      // flavTag
      float SV0;
      float SV1;
      float IP3D;
      float SV1IP3D;
      float MV1;
      float MV2c00;
      float MV2c10;
      float MV2c20;
      float MV2;
      int  HadronConeExclTruthLabelID;
    
      int MV2c20_isFix30;
      std::vector<float> MV2c20_sfFix30;
    
      int MV2c20_isFix50;
      std::vector<float> MV2c20_sfFix50;
    
      int MV2c20_isFix60;
      std::vector<float> MV2c20_sfFix60;
    
      int MV2c20_isFix70;
      std::vector<float> MV2c20_sfFix70;
    
      int MV2c20_isFix77;
      std::vector<float> MV2c20_sfFix77;
    
      int MV2c20_isFix80;
      std::vector<float> MV2c20_sfFix80;
    
      int MV2c20_isFix85;
      std::vector<float> MV2c20_sfFix85;
    
      int MV2c20_isFix90;
      std::vector<float> MV2c20_sfFix90;
    
      int MV2c20_isFlt30;
      std::vector<float> MV2c20_sfFlt30;
    
      int MV2c20_isFlt40;
      std::vector<float> MV2c20_sfFlt40;
    
      int MV2c20_isFlt50;
      std::vector<float> MV2c20_sfFlt50;
    
      int MV2c20_isFlt60;
      std::vector<float> MV2c20_sfFlt60;
    
      int MV2c20_isFlt70;
      std::vector<float> MV2c20_sfFlt70;
    
      int MV2c20_isFlt77;
      std::vector<float> MV2c20_sfFlt77;
    
      int MV2c20_isFlt85;
      std::vector<float> MV2c20_sfFlt85;
    
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

    public:

      void muonInJetCorrection(const xAH::MuonContainer* muons);

    };

}//xAH
#endif // xAODAnaHelpers_Particle_H
