#ifndef xAODAnaHelpers_TauContainer_H
#define xAODAnaHelpers_TauContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODTau/TauJetContainer.h>

#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/Tau.h>
#include <xAODAnaHelpers/ParticleContainer.h>

namespace xAH {

  class TauContainer : public ParticleContainer<Tau,HelperClasses::TauInfoSwitch>
    {
    public:
      TauContainer(const std::string& name = "tau", const std::string& detailStr="", float units = 1e3, bool mc = false, bool storeSystSFs = true);
      virtual ~TauContainer();
    
      virtual void setTree(TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillTau( const xAOD::TauJet* tau          );
      virtual void FillTau( const xAOD::IParticle* particle  );
      using ParticleContainer::setTree; // make other overloaded version of execute() to show up in subclass

    protected:
      virtual void updateParticle(uint idx, Tau& tau);
    
    private:
      
      // trigger
      std::vector<int>               *m_isTrigMatched;
      std::vector<std::vector<int> > *m_isTrigMatchedToChain;
      std::vector<std::string>       *m_listTrigChains;

      std::vector<int>   *m_ntrk;
      std::vector<float> *m_charge;
      
      // scale factors w/ sys
      // per object
      std::map< std::string, std::vector< std::vector< float > > >* m_TauEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TauTrigEff_SF;
      
      // might need to delete these
      std::vector<int>   *m_isJetRNNSigVeryLoose;
      std::vector<int>   *m_isJetRNNSigLoose;
      std::vector<int>   *m_isJetRNNSigMedium;
      std::vector<int>   *m_isJetRNNSigTight;
    
      std::vector<float>   *m_JetRNNScore;
      std::vector<float>   *m_JetRNNScoreSigTrans;

      std::vector<int>   *m_isEleRNNLoose;
      std::vector<int>   *m_isEleRNNMedium;
      std::vector<int>   *m_isEleRNNTight;

      std::vector<float>   *m_EleRNNScore;

      std::vector<int>   *m_passEleOLR;

      std::vector< float > *m_tau_matchedJetWidth;
      std::vector< float > *m_tau_matchedJetJvt;
      
      std::vector< std::vector< float > > *m_tau_tracks_pt;
      std::vector< std::vector< float > > *m_tau_tracks_eta;
      std::vector< std::vector< float > > *m_tau_tracks_phi;
  
      std::vector< std::vector< int > > *m_tau_tracks_isCore;
      std::vector< std::vector< int > > *m_tau_tracks_isWide;
      std::vector< std::vector< int > > *m_tau_tracks_failTrackFilter;
      std::vector< std::vector< int > > *m_tau_tracks_passTrkSel;
      std::vector< std::vector< int > > *m_tau_tracks_isClCharged;
      std::vector< std::vector< int > > *m_tau_tracks_isClIso;
      std::vector< std::vector< int > > *m_tau_tracks_isClConv;
      std::vector< std::vector< int > > *m_tau_tracks_isClFake;       

    };
}
#endif // xAODAnaHelpers_TauContainer_H
