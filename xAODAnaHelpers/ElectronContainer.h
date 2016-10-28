#ifndef xAODAnaHelpers_ElectronContainer_H
#define xAODAnaHelpers_ElectronContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include "xAODEgamma/ElectronContainer.h"

#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/Electron.h>
#include <xAODAnaHelpers/ParticleContainer.h>

namespace xAH {

  class ElectronContainer : public ParticleContainer<Electron,HelperClasses::ElectronInfoSwitch>
    {
    public:
      ElectronContainer(const std::string& name = "el", const std::string& detailStr="", float units = 1e3, bool mc = false);
      virtual ~ElectronContainer();
    
      virtual void setTree(TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillElectron( const xAOD::Electron* elec, const xAOD::Vertex* primaryVertex );
      virtual void FillElectron( const xAOD::IParticle* particle, const xAOD::Vertex* primaryVertex );
      using ParticleContainer::setTree; // make other overloaded version of execute() to show up in subclass

    protected:
      virtual void updateParticle(uint idx, Electron& elec);
    
    private:

      // kinematics
      std::vector<float>*  m_caloCluster_eta;
 
      // trigger
      std::vector<int>*  m_isTrigMatched;
      std::vector<std::vector<int> >* m_isTrigMatchedToChain;
      std::vector<std::string>*       m_listTrigChains;

      // isolation
      std::vector<int>*   m_isIsolated_LooseTrackOnly;
      std::vector<int>*   m_isIsolated_Loose;
      std::vector<int>*   m_isIsolated_Tight;
      std::vector<int>*   m_isIsolated_Gradient;
      std::vector<int>*   m_isIsolated_GradientLoose;
      std::vector<int>*   m_isIsolated_FixedCutLoose;
      std::vector<int>*   m_isIsolated_FixedCutTight;
      std::vector<int>*   m_isIsolated_FixedCutTightTrackOnly;
      std::vector<int>*   m_isIsolated_UserDefinedFixEfficiency;
      std::vector<int>*   m_isIsolated_UserDefinedCut;
      std::vector<float>* m_etcone20;
      std::vector<float>* m_ptcone20;
      std::vector<float>* m_ptcone30;
      std::vector<float>* m_ptcone40;
      std::vector<float>* m_ptvarcone20;
      std::vector<float>* m_ptvarcone30;
      std::vector<float>* m_ptvarcone40;
      std::vector<float>* m_topoetcone20;
      std::vector<float>* m_topoetcone30;
      std::vector<float>* m_topoetcone40;

      // PID
      int m_n_LHLoose;
      std::vector<int>*   m_LHLoose;
      int m_n_LHLooseBL;
      std::vector<int>*   m_LHLooseBL;
      int m_n_LHMedium;
      std::vector<int>*   m_LHMedium;
      int m_n_LHTight;
      std::vector<int>*   m_LHTight;
      int m_n_IsEMLoose;
      std::vector<int>*   m_IsEMLoose;
      int m_n_IsEMMedium;
      std::vector<int>*   m_IsEMMedium;
      int m_n_IsEMTight;
      std::vector<int>*   m_IsEMTight;
    
      // scale factors w/ sys
      // per object
      std::vector< std::vector< float > >* m_RecoEff_SF;
      std::vector< std::vector< float > >* m_PIDEff_SF_LHLooseAndBLayer;
      std::vector< std::vector< float > >* m_PIDEff_SF_LHLoose;
      std::vector< std::vector< float > >* m_PIDEff_SF_LHMedium;
      std::vector< std::vector< float > >* m_PIDEff_SF_LHTight;
    
      std::map< std::string, std::vector< std::vector< float > > >* m_IsoEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TrigEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TrigMCEff;
      
      // track parameters
      std::vector<float>* m_trkd0;
      std::vector<float>* m_trkd0sig;
      std::vector<float>* m_trkz0;
      std::vector<float>* m_trkz0sintheta;
      std::vector<float>* m_trkphi0;
      std::vector<float>* m_trktheta;
      std::vector<float>* m_trkcharge;
      std::vector<float>* m_trkqOverP;
    
      // track hit content
      std::vector<int>*   m_trknSiHits;
      std::vector<int>*   m_trknPixHits;
      std::vector<int>*   m_trknPixHoles;
      std::vector<int>*   m_trknSCTHits;
      std::vector<int>*   m_trknSCTHoles;
      std::vector<int>*   m_trknTRTHits;
      std::vector<int>*   m_trknTRTHoles;
      std::vector<int>*   m_trknBLayerHits;
      std::vector<int>*   m_trknInnermostPixLayHits; // not available in DC14
      std::vector<float>* m_trkPixdEdX;            // not available in DC14

    
    };
}
#endif // xAODAnaHelpers_ElectronContainer_H
