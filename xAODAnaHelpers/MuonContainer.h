#ifndef xAODAnaHelpers_MuonContainer_H
#define xAODAnaHelpers_MuonContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODMuon/MuonContainer.h>

#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/Muon.h>
#include <xAODAnaHelpers/ParticleContainer.h>

namespace xAH {

  class MuonContainer : public ParticleContainer<Muon,HelperClasses::MuonInfoSwitch>
    {
    public:
      MuonContainer(const std::string& name = "muon", const std::string& detailStr="", float units = 1e3, bool mc = false);
      virtual ~MuonContainer();
    
      virtual void setTree(TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillMuon( const xAOD::Muon* muon, const xAOD::Vertex* primaryVertex );
      virtual void FillMuon( const xAOD::IParticle* particle, const xAOD::Vertex* primaryVertex );
      using ParticleContainer::setTree; // make other overloaded version of execute() to show up in subclass

    protected:
      virtual void updateParticle(uint idx, Muon& muon);
    
    private:

      // trigger
      std::vector<int>               *m_isTrigMatched;
      std::vector<std::vector<int> > *m_isTrigMatchedToChain;
      std::vector<std::string>       *m_listTrigChains;
    
      // isolation
      std::vector<int>   *m_isIsolated_LooseTrackOnly;
      std::vector<int>   *m_isIsolated_Loose;
      std::vector<int>   *m_isIsolated_Tight;
      std::vector<int>   *m_isIsolated_Gradient;
      std::vector<int>   *m_isIsolated_GradientLoose;
      std::vector<int>   *m_isIsolated_FixedCutLoose;
      std::vector<int>   *m_isIsolated_FixedCutTightTrackOnly;
      std::vector<int>   *m_isIsolated_UserDefinedFixEfficiency;
      std::vector<int>   *m_isIsolated_UserDefinedCut;
      std::vector<float> *m_ptcone20;
      std::vector<float> *m_ptcone30;
      std::vector<float> *m_ptcone40;
      std::vector<float> *m_ptvarcone20;
      std::vector<float> *m_ptvarcone30;
      std::vector<float> *m_ptvarcone40;
      std::vector<float> *m_topoetcone20;
      std::vector<float> *m_topoetcone30;
      std::vector<float> *m_topoetcone40;
    
      // quality
      std::vector<int>   *m_isVeryLoose;
      std::vector<int>   *m_isLoose;
      std::vector<int>   *m_isMedium;
      std::vector<int>   *m_isTight;
    
      // scale factors w/ sys
      // per object
      std::vector< std::vector< float > > *m_TTVAEff_SF;
    
      std::map< std::string, std::vector< std::vector< float > > >* m_RecoEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_IsoEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TrigEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TrigMCEff;
      
      // track parameters
      std::vector<float> *m_trkd0;
      std::vector<float> *m_trkd0sig;
      std::vector<float> *m_trkz0;
      std::vector<float> *m_trkz0sintheta;
      std::vector<float> *m_trkphi0;
      std::vector<float> *m_trktheta;
      std::vector<float> *m_trkcharge;
      std::vector<float> *m_trkqOverP;
    
      // track hit content
      std::vector<int>   *m_trknSiHits;
      std::vector<int>   *m_trknPixHits;
      std::vector<int>   *m_trknPixHoles;
      std::vector<int>   *m_trknSCTHits;
      std::vector<int>   *m_trknSCTHoles;
      std::vector<int>   *m_trknTRTHits;
      std::vector<int>   *m_trknTRTHoles;
      std::vector<int>   *m_trknBLayerHits;
      std::vector<int>   *m_trknInnermostPixLayHits; // not available in DC14
      std::vector<float> *m_trkPixdEdX;            // not available in DC14
    
      std::vector<float>         *m_EnergyLoss;
      std::vector<float>         *m_EnergyLossSigma;
      std::vector<unsigned char> *m_energyLossType;
      std::vector<float>         *m_MeasEnergyLoss;
      std::vector<float>         *m_MeasEnergyLossSigma;
      std::vector<float>         *m_ParamEnergyLoss;
      std::vector<float>         *m_ParamEnergyLossSigmaMinus;
      std::vector<float>         *m_ParamEnergyLossSigmaPlus;
    
    };
}
#endif // xAODAnaHelpers_MuonContainer_H
