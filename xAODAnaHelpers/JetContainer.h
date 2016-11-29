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
    
      virtual void setTree    (TTree *tree, std::string tagger="MV2c10");
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillJet( const xAOD::Jet* jet,            const xAOD::Vertex* pv, int pvLocation );
      virtual void FillJet( const xAOD::IParticle* particle, const xAOD::Vertex* pv, int pvLocation );
      virtual void FillGlobalBTagSF( const xAOD::EventInfo* eventInfo );
      using ParticleContainer::setTree; // make other overloaded version of execute() to show up in subclass

      virtual void updateParticle(uint idx, Jet& jet);

//template<typename T>
//  void setBranch(TTree* tree, std::string varName, std::vector<T>* localVectorPtr);
    
    private:

      bool findBTagSF(const std::vector<int>& sfList, int workingPt);
      
      InDet::InDetTrackSelectionTool * m_trkSelTool;

      //
      // Vector branches
    
      // rapidity
      std::vector<float> *m_rapidity;

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
    
      // sclaes
      std::vector<float> *m_emScalePt;
      std::vector<float> *m_constScalePt;
      std::vector<float> *m_pileupScalePt;
      std::vector<float> *m_originConstitScalePt;
      std::vector<float> *m_etaJESScalePt;
      std::vector<float> *m_gscScalePt;
      std::vector<float> *m_insituScalePt;
      

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
      std::vector< std::vector<float> > *m_JvtEff_SF_Loose;
      std::vector< std::vector<float> > *m_JvtEff_SF_Medium;
      std::vector< std::vector<float> > *m_JvtEff_SF_Tight;
      std::vector<float> *m_JvtJvfcorr;
      std::vector<float> *m_JvtRpt;
    
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
      std::vector<float> *m_MV2c20;
      std::vector<float> *m_MV2c100;
      std::vector<float> *m_MV2;
      std::vector<int>   *m_HadronConeExclTruthLabelID;
    
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
        std::string m_name;
        bool m_mc;
        std::string m_acessorName;
        std::string m_tagger;
        int m_njets;
        std::vector<int>*                  m_isTag;
        std::vector<float>                 m_weight_sf;
        std::vector< std::vector<float> >* m_sf;

        btagOpPoint(std::string name, bool mc, std::string acessorName, std::string tagger="mv2c10"): m_name(name), m_mc(mc), m_acessorName(acessorName), m_tagger(tagger) {
          m_isTag = new std::vector<int>();
          m_sf    = new std::vector< std::vector<float> >();
        }

        ~btagOpPoint(){
          delete m_isTag;
          delete m_sf;
        }

        void setTree(TTree *tree, std::string jetName){
          //tree->SetBranchStatus  (("n"+jetName+"s_"+m_tagger+"_"+m_name).c_str(), 1);
          //tree->SetBranchAddress (("n"+jetName+"s_"+m_tagger+"_"+m_name).c_str(), &m_njets);
          tree->SetBranchStatus  (("n"+jetName+"s_"+m_name).c_str(), 1);
          tree->SetBranchAddress (("n"+jetName+"s_"+m_name).c_str(), &m_njets);

          HelperFunctions::connectBranch<int>     (jetName, tree,"is"+m_name,      &m_isTag);
          if(m_mc) HelperFunctions::connectBranch<std::vector<float> >(jetName, tree,"SF"+m_name,       &m_sf);
        }


        void setBranch(TTree *tree, std::string jetName){
          tree->Branch(("n"+jetName+"s_"+m_name).c_str(), &m_njets, ("n"+jetName+"s_"+m_name+"/I").c_str());
          tree->Branch((jetName+"_is"+m_name).c_str(),        &m_isTag);

          if ( m_mc ) {
            tree->Branch((jetName+"_SF"+m_name).c_str(),        &m_sf);
            tree->Branch(("weight_"+jetName+"SF"+m_name).c_str(), &m_weight_sf);
          }
        }


        void clear(){
          m_njets = 0;
          m_isTag->clear();
          m_weight_sf.clear();
          m_sf->clear();
        }

        void Fill( const xAOD::Jet* jet ) {
      
          SG::AuxElement::ConstAccessor< char > isTag("BTag_"+m_acessorName);
          if( isTag.isAvailable( *jet ) ) {
            if ( isTag( *jet ) == 1 ) ++m_njets;
            m_isTag->push_back( isTag( *jet ) );
          } else { 
            m_isTag->push_back( -1 ); 
          }
          
          if(!m_mc) { return; }
          SG::AuxElement::ConstAccessor< std::vector<float> > sf("BTag_SF_"+m_acessorName);
          if ( sf.isAvailable( *jet ) ) {
            m_sf->push_back( sf( *jet ) );
          } else {
            std::vector<float> junk(1,-999);
            m_sf->push_back(junk);
          }
      
          return;
        } // Fill
      
        void FillGlobalSF( const xAOD::EventInfo* eventInfo ) {
          SG::AuxElement::ConstAccessor< std::vector<float> > sf_GLOBAL("BTag_SF_"+m_acessorName+"_GLOBAL");
          if ( sf_GLOBAL.isAvailable( *eventInfo ) ) { 
            m_weight_sf = sf_GLOBAL( *eventInfo ); 
          } else { 
            m_weight_sf.push_back(-999.0); 
          }
      
          return;
        }
  
      };  //struct btagOpPoint
      
      btagOpPoint* m_btag_Fix30;
      btagOpPoint* m_btag_Fix50;
      btagOpPoint* m_btag_Fix60;
      btagOpPoint* m_btag_Fix70;
      btagOpPoint* m_btag_Fix77;
      btagOpPoint* m_btag_Fix80;
      btagOpPoint* m_btag_Fix85;
      btagOpPoint* m_btag_Fix90;

      btagOpPoint* m_btag_Flt30;
      btagOpPoint* m_btag_Flt50;
      btagOpPoint* m_btag_Flt60;
      btagOpPoint* m_btag_Flt70;
      btagOpPoint* m_btag_Flt77;
      btagOpPoint* m_btag_Flt85;
      btagOpPoint* m_btag_Flt90;

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
