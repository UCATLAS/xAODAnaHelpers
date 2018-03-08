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
      TauContainer(const std::string& name = "tau", const std::string& detailStr="", float units = 1e3, bool mc = false);
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
      
      // tau identification
      std::map< std::string, std::vector< int > >* m_isIdentified;
      
      // scale factors w/ sys
      // per object
      std::vector< std::vector< float > > *m_EleOLRHadTauEff_SF;
      std::vector< std::vector< float > > *m_RecoEff_SF;
      
      std::map< std::string, std::vector< std::vector< float > > >* m_TauIDEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_EleOLRElectronEff_SF;
      std::map< std::string, std::vector< std::vector< float > > >* m_TrigEff_SF;
      
      
      // might need to delete these
      std::vector<int>   *m_isJetBDTSigVeryLoose;
      std::vector<int>   *m_isJetBDTSigLoose;
      std::vector<int>   *m_isJetBDTSigMedium;
      std::vector<int>   *m_isJetBDTSigTight;
    
      std::vector<int>   *m_isJetBDTBkgLoose;
      std::vector<int>   *m_isJetBDTBkgMedium;
      std::vector<int>   *m_isJetBDTBkgTight;
      
      std::vector<float>   *m_JetBDTScore;
      std::vector<float>   *m_JetBDTScoreSigTrans;

    };
}
#endif // xAODAnaHelpers_TauContainer_H
