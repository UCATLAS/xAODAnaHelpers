#ifndef xAODAnaHelpers_FatJetContainer_H
#define xAODAnaHelpers_FatJetContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include "xAODJet/JetContainer.h"

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include "JetSubStructureUtils/BosonTag.h"

#include <xAODAnaHelpers/FatJet.h>
#include <xAODAnaHelpers/ParticleContainer.h>
#include <xAODAnaHelpers/JetContainer.h>

#include "InDetTrackSelectionTool/InDetTrackSelectionTool.h"


namespace xAH {

    class FatJetContainer : public ParticleContainer<FatJet,HelperClasses::JetInfoSwitch>
    {
    public:
      FatJetContainer(const std::string& name = "fatjet", const std::string& detailStr="", const std::string& suffix="",
		      float units = 1e3, bool mc = false);
      virtual ~FatJetContainer();
    
      virtual void setTree    (TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillFatJet( const xAOD::Jet* jet );
      virtual void FillFatJet( const xAOD::IParticle* particle );
      using ParticleContainer::setTree; // make other overloaded version of execute() to show up in subclass

      float       m_trackJetPtCut;
      float       m_trackJetEtaCut;
      std::string m_trackJetName;

    protected:

      virtual void updateParticle(uint idx, FatJet& jet);

    private:
      
      JetSubStructureUtils::BosonTag*      m_WbosonTaggerMedium;
      JetSubStructureUtils::BosonTag*      m_ZbosonTaggerMedium;
      JetSubStructureUtils::BosonTag*      m_WbosonTaggerTight ;
      JetSubStructureUtils::BosonTag*      m_ZbosonTaggerTight ;

      bool SelectTrackJet(const xAOD::Jet* TrackJet);

    private:

      //
      // Vector branches
      //

      // scales
      std::vector<float> *m_JetConstitScaleMomentum_eta;
      std::vector<float> *m_JetConstitScaleMomentum_phi;
      std::vector<float> *m_JetConstitScaleMomentum_m;
      std::vector<float> *m_JetConstitScaleMomentum_pt;

      std::vector<float> *m_JetEMScaleMomentum_eta;
      std::vector<float> *m_JetEMScaleMomentum_phi;
      std::vector<float> *m_JetEMScaleMomentum_m;
      std::vector<float> *m_JetEMScaleMomentum_pt;

      // area
      std::vector<float> *m_GhostArea;
      std::vector<float> *m_ActiveArea;
      std::vector<float> *m_VoronoiArea;

      std::vector<float> *m_ActiveArea4vec_pt;
      std::vector<float> *m_ActiveArea4vec_eta;
      std::vector<float> *m_ActiveArea4vec_phi;
      std::vector<float> *m_ActiveArea4vec_m;

      // substructure
      std::vector<float> *m_Split12;
      std::vector<float> *m_Split23;
      std::vector<float> *m_Split34;
      std::vector<float> *m_tau1_wta;
      std::vector<float> *m_tau2_wta;
      std::vector<float> *m_tau3_wta;
      std::vector<float> *m_tau21_wta;
      std::vector<float> *m_tau32_wta;
      std::vector<float> *m_ECF1;
      std::vector<float> *m_ECF2;
      std::vector<float> *m_ECF3;
      std::vector<float> *m_C2;
      std::vector<float> *m_D2;
      std::vector<float> *m_NTrimSubjets;
      std::vector<int>   *m_NClusters;
      std::vector<int>   *m_nTracks;

      // constituent
      std::vector< int > *m_numConstituents;

      // constituentAll
      std::vector< std::vector<float> >  *m_constituentWeights;
      std::vector< std::vector<float> >  *m_constituent_pt;
      std::vector< std::vector<float> >  *m_constituent_eta;
      std::vector< std::vector<float> >  *m_constituent_phi;
      std::vector< std::vector<float> >  *m_constituent_e;

      // bosonCount
      std::vector< int > *m_nTQuarks;
      std::vector< int > *m_nHBosons;      
      std::vector< int > *m_nWBosons;            
      std::vector< int > *m_nZBosons;      

      // VTag
      std::vector< int > *m_Wtag_medium;
      std::vector< int > *m_Ztag_medium;

      std::vector< int > *m_Wtag_tight;
      std::vector< int > *m_Ztag_tight;

      // Assocated Track Jets 
      std::vector< std::vector<unsigned int> >* m_trkJetsIdx;
      xAH::JetContainer*  m_trkJets;
      
    };
}



#endif // xAODAnaHelpers_FatJetContainer_H
