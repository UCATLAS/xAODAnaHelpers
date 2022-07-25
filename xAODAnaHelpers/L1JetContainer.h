#ifndef xAODAnaHelpers_L1JetContainer_H
#define xAODAnaHelpers_L1JetContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODTrigger/JetRoIContainer.h>
#include <xAODTrigger/jFexSRJetRoIContainer.h>
#include <xAODTrigger/jFexLRJetRoIContainer.h>
#include <xAODTrigger/gFexJetRoIContainer.h>

#include <xAODAnaHelpers/Jet.h>
#include <xAODAnaHelpers/ParticleContainer.h>

namespace xAH {

    class L1JetContainer : public ParticleContainer<Jet,HelperClasses::JetInfoSwitch>
    {
    public:
      L1JetContainer(const std::string& name = "L1Jet", float units = 1e3, bool mc = false);
      virtual ~L1JetContainer();

      virtual void setTree    (TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillLegacyL1Jets( const xAOD::JetRoIContainer* jets, bool sort);
      virtual void updateParticle(uint idx, Jet& jet);

      // access to et, eta and phi is the same for all the Phase1 L1 jet collections
      template <typename T>
      void FillPhase1L1Jets(T*& jets, bool sort){
        if(!sort) {
          for( auto jet_itr : *jets ) {
            m_l1Jet_et->push_back ( static_cast<float>(jet_itr->et()) / m_units );            
            m_l1Jet_eta->push_back( jet_itr->eta() );
            m_l1Jet_phi->push_back( jet_itr->phi() );
          }
        } else {
          std::vector< std::vector<float> > vec;
          for( auto jet_itr : *jets ) {
            std::vector<float> row;
            row.clear();
            row.push_back ( static_cast<float>(jet_itr->et()) / m_units );
            row.push_back(jet_itr->eta());
            row.push_back(jet_itr->phi());
            vec.push_back(row);
          }
          
          std::sort(vec.begin(), vec.end(), [&](const std::vector<float> a, const std::vector<float> b) { return a.at(0) < b.at(0);});
          for (int i = int(vec.size())-1; i >= 0; i--) {
            m_l1Jet_et->push_back((vec.at(i)).at(0) / m_units);
            m_l1Jet_eta->push_back((vec.at(i)).at(1));
            m_l1Jet_phi->push_back((vec.at(i)).at(2));
          }
          vec.clear();
        }
      }
    
      

    private:
      // Vector branches
      std::vector<float>* m_l1Jet_et;
      std::vector<float>* m_l1Jet_eta;
      std::vector<float>* m_l1Jet_phi;
    };

}

#endif // xAODAnaHelpers_L1JetContainer_H
