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
      std::vector<int>   *m_clean_passLooseBadTrigger;
      std::vector<int>   *m_clean_passLooseBadTriggerUgly;
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
	Jet::BTaggerOP m_op = Jet::BTaggerOP::None;
        bool m_isContinuous;

	// branches
        std::vector<int>*                  m_isTag;
        std::vector< std::vector<float> >* m_sf;
        std::vector< std::vector<float> >* m_ineffSf; // for continuous

        btagOpPoint(bool mc, const std::string& tagger, const std::string& wp)
	  : m_mc(mc), m_accessorName(tagger+"_"+wp)
	{
          m_isTag     = new std::vector<int>();
          m_sf        = new std::vector< std::vector<float> >();

          m_isContinuous = (wp == "Continuous");
          if(m_isContinuous)
            m_ineffSf = new std::vector< std::vector<float> >();

	  if(m_accessorName=="DL1_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_60;
	  else if(m_accessorName=="DL1_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_70;
	  else if(m_accessorName=="DL1_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_77;
	  else if(m_accessorName=="DL1_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1_FixedCutBEff_85;	  
	  else if(m_accessorName=="DL1r_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_60;
	  else if(m_accessorName=="DL1r_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_70;
	  else if(m_accessorName=="DL1r_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_77;
	  else if(m_accessorName=="DL1r_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1r_FixedCutBEff_85;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_60;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_70;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_77;
	  else if(m_accessorName=="DL1rmu_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::DL1rmu_FixedCutBEff_85;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_60")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_60;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_70")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_70;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_77")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_77;
	  else if(m_accessorName=="MV2c10_FixedCutBEff_85")
	    m_op=Jet::BTaggerOP::MV2c10_FixedCutBEff_85;
	  else if(m_accessorName=="DL1_Continuous")
	    m_op=Jet::BTaggerOP::DL1_Continuous;
          else if(m_accessorName=="DL1r_Continuous")
	    m_op=Jet::BTaggerOP::DL1r_Continuous;
          else if(m_accessorName=="DL1rmu_Continuous")
	    m_op=Jet::BTaggerOP::DL1rmu_Continuous;
	  else if(m_accessorName=="MV2c10_Continuous")
	    m_op=Jet::BTaggerOP::MV2c10_Continuous;
        }

        ~btagOpPoint()
	{
          delete m_isTag;
          delete m_sf;
          if(m_isContinuous)
            delete m_ineffSf;
        }

        void setTree(TTree *tree, const std::string& jetName)
	{
	  std::string branch = m_isContinuous ? "Quantile_"+m_accessorName : "is_"+m_accessorName;
	  HelperFunctions::connectBranch<int>                   (jetName, tree, branch,               &m_isTag);
	  if(m_mc)
	    {
	      HelperFunctions::connectBranch<std::vector<float> >(jetName, tree,"SF_"+m_accessorName, &m_sf);
	      if(m_isContinuous)
		HelperFunctions::connectBranch<std::vector<float>>(jetName, tree, "InefficiencySF_"+m_accessorName, &m_ineffSf);
	    }
        }

        void setBranch(TTree *tree, const std::string& jetName)
	{
          std::string id = m_isContinuous ? "_Quantile_" : "_is_";
          tree->Branch((jetName+id+m_accessorName).c_str(), &m_isTag);
          if ( m_mc )
	    {
	      tree->Branch((jetName+"_SF_"+m_accessorName).c_str()              , &m_sf);
	      if(m_isContinuous)
		tree->Branch((jetName+"_InefficiencySF_"+m_accessorName).c_str(), &m_ineffSf);
	    }
        }

        void clear()
	{
          m_isTag->clear();
          m_sf->clear();
          if(m_isContinuous)
            m_ineffSf->clear();
        }

        void Fill( const xAOD::Jet* jet )
	{
          static const std::vector<float> junk(1,-999);

          if( m_isContinuous )
	    {
	      SG::AuxElement::ConstAccessor< int > quantile("BTag_Quantile_"+m_accessorName);
	      m_isTag->push_back( quantile.isAvailable(*jet) ? quantile(*jet) : -1 );

	      if(m_mc)
		{
		  SG::AuxElement::ConstAccessor< std::vector<float> > sf(     "BTag_SF_"            +m_accessorName);
		  SG::AuxElement::ConstAccessor< std::vector<float> > ineffSf("BTag_InefficiencySF_"+m_accessorName);

		  m_sf     ->push_back(      sf.isAvailable( *jet ) ?      sf( *jet ) : junk);
		  m_ineffSf->push_back( ineffSf.isAvailable( *jet ) ? ineffSf( *jet ) : junk);
		}
	    }
          else
	    {
	      SG::AuxElement::ConstAccessor< char > isTag("BTag_"+m_accessorName);
	      m_isTag->push_back( isTag.isAvailable(*jet) ? isTag(*jet) : -1 );

	      if(m_mc)
		{
		  SG::AuxElement::ConstAccessor< std::vector<float> > sf("BTag_SF_"+m_accessorName);
		  m_sf->push_back( sf.isAvailable( *jet ) ? sf( *jet ) : junk);
		}
	    }

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

      // passSel
      std::vector<char> *m_passSel;
    };
}



#endif // xAODAnaHelpers_JetContainer_H
