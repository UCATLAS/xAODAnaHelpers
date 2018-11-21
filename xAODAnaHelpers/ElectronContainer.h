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
      ElectronContainer(const std::string& name = "el", const std::string& detailStr="", float units = 1e3, bool mc = false, bool storeSystSFs = true);
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
      std::vector<float>* m_charge;

      // trigger
      std::vector<int>*  m_isTrigMatched;
      std::vector<std::vector<int> >* m_isTrigMatchedToChain;
      std::vector<std::string>*       m_listTrigChains;

      // isolation
      std::map< std::string, std::vector< int > >* m_isIsolated;
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
      std::map< std::string, std::vector< int > >* m_PID;

      // scale factors w/ sys
      // per object
      std::vector< std::vector< float > >* m_RecoEff_SF;

      std::map< std::string, std::vector< std::vector< float > > >* m_PIDEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_IsoEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TrigEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TrigMCEff;

      // reco parameters
      std::vector<int>* m_author;
      std::vector<int>* m_OQ;

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

      // prompt lepton
      std::vector<float>* m_PromptLeptonInput_DL1mu;
      std::vector<float>* m_PromptLeptonInput_DRlj;
      std::vector<float>* m_PromptLeptonInput_LepJetPtFrac;
      std::vector<float>* m_PromptLeptonInput_PtFrac;
      std::vector<float>* m_PromptLeptonInput_PtRel;
      std::vector<int>*   m_PromptLeptonInput_TrackJetNTrack;
      std::vector<float>* m_PromptLeptonInput_ip2;
      std::vector<float>* m_PromptLeptonInput_ip3;
      std::vector<float>* m_PromptLeptonInput_rnnip;
      std::vector<int>*   m_PromptLeptonInput_sv1_jf_ntrkv;
      std::vector<float>* m_PromptLeptonIso;
      std::vector<float>* m_PromptLeptonVeto;

    };
}
#endif // xAODAnaHelpers_ElectronContainer_H
