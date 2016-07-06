#ifndef xAODAnaHelpers_JetContainer_H
#define xAODAnaHelpers_JetContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/Jet.h>
#include <xAODAnaHelpers/ParticleContainer.h>

namespace xAH {

    class JetContainer : public ParticleContainer<Jet,HelperClasses::JetInfoSwitch>
    {
    public:
      JetContainer(const std::string& name = "jet", const std::string& detailStr="");
      virtual ~JetContainer();
    
      virtual void setTree(TTree *tree, std::string tagger="MV2c10");
      using ParticleContainer::setTree; // make other overloaded version of execute() to show up in subclass
    
    protected:
      virtual void updateParticle(uint idx, Jet& jet);
    
    private:
      //
      // Vector branches
    
      // rapidity
      std::vector<float> m_jet_rapidity;

      // clean
      std::vector<float> *m_Timing;
      std::vector<float> *m_LArQuality;
      std::vector<float> *m_HECQuality;
      std::vector<float> *m_NegativeE;
      std::vector<float> *m_AverageLArQF;
      std::vector<float> *m_BchCorrCell;
      std::vector<float> *m_N90Constituents;
      std::vector<float> *m_LArBadHVEFrac;
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
    
      // sclaes
      

      // layer

      // tracksAll


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
      std::vector<float> *m_JvtJvfcorr;
      std::vector<float> *m_JvtRpt;
    
      // allTrack

      // allTrackDetail

    // constituent

      // flavTag
      std::vector<float> *m_SV0;
      std::vector<float> *m_SV1;
      std::vector<float> *m_IP3D;
      std::vector<float> *m_SV1IP3D;
      //std::vector<float> *m_MV1;
      std::vector<float> *m_MV2c00;
      std::vector<float> *m_MV2c10;
      std::vector<float> *m_MV2c20;
      std::vector<float> *m_MV2;
      std::vector<int>   *m_HadronConeExclTruthLabelID;
    
      int m_njets_MV2c20_Fix30;
      std::vector<int>                  *m_MV2c20_isFix30;
      std::vector< std::vector<float> > *m_MV2c20_sfFix30;
    
      int m_njets_MV2c20_Fix50;
      std::vector<int> *m_MV2c20_isFix50;
      std::vector< std::vector<float> > *m_MV2c20_sfFix50;
    
      int m_njets_MV2c20_Fix60;
      std::vector<int> *m_MV2c20_isFix60;
      std::vector< std::vector<float> > *m_MV2c20_sfFix60;
    
      int m_njets_MV2c20_Fix70;
      std::vector<int> *m_MV2c20_isFix70;
      std::vector< std::vector<float> > *m_MV2c20_sfFix70;
    
      int m_njets_MV2c20_Fix77;
      std::vector<int> *m_MV2c20_isFix77;
      std::vector< std::vector<float> > *m_MV2c20_sfFix77;
    
      int m_njets_MV2c20_Fix80;
      std::vector<int> *m_MV2c20_isFix80;
      std::vector< std::vector<float> > *m_MV2c20_sfFix80;
    
      int m_njets_MV2c20_Fix85;
      std::vector<int> *m_MV2c20_isFix85;
      std::vector< std::vector<float> > *m_MV2c20_sfFix85;
    
      int m_njets_MV2c20_Fix90;
      std::vector<int> *m_MV2c20_isFix90;
      std::vector< std::vector<float> > *m_MV2c20_sfFix90;
    
      int m_njets_MV2c20_Flt30;
      std::vector<int> *m_MV2c20_isFlt30;
      std::vector< std::vector<float> > *m_MV2c20_sfFlt30;
    
      int m_njets_MV2c20_Flt40;
      std::vector<int> *m_MV2c20_isFlt40;
      std::vector< std::vector<float> > *m_MV2c20_sfFlt40;
    
      int m_njets_MV2c20_Flt50;
      std::vector<int> *m_MV2c20_isFlt50;
      std::vector< std::vector<float> > *m_MV2c20_sfFlt50;
    
      int m_njets_MV2c20_Flt60;
      std::vector<int> *m_MV2c20_isFlt60;
      std::vector< std::vector<float> > *m_MV2c20_sfFlt60;
    
      int m_njets_MV2c20_Flt70;
      std::vector<int> *m_MV2c20_isFlt70;
      std::vector< std::vector<float> > *m_MV2c20_sfFlt70;
      
      int m_njets_MV2c20_Flt77;
      std::vector<int> *m_MV2c20_isFlt77;
      std::vector< std::vector<float> > *m_MV2c20_sfFlt77;
    
      int m_njets_MV2c20_Flt85;
      std::vector<int> *m_MV2c20_isFlt85;
      std::vector< std::vector<float> > *m_MV2c20_sfFlt85;
    
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
    
      // charge
      std::vector<double> *m_charge;
    };
}
#endif // xAODAnaHelpers_JetContainer_H
