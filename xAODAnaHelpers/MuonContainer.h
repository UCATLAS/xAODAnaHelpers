#ifndef xAODAnaHelpers_MuonContainer_H
#define xAODAnaHelpers_MuonContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/Muon.h>
#include <xAODAnaHelpers/ParticleContainer.h>

namespace xAH {

  class MuonContainer : public ParticleContainer<Muon,HelperClasses::MuonInfoSwitch>
    {
    public:
      MuonContainer(const std::string& name = "muon", const std::string& detailStr="");
      virtual ~MuonContainer();
    
      virtual void setTree(TTree *tree);
    
    protected:
      virtual void updateParticle(uint idx, Muon& muon);
    
    private:
      //
      // Vector branches
      //
      std::vector<int>*    m_isMedium   ;
      std::vector<int>*    m_isTight    ;
      std::vector<float>*  m_EnergyLoss ;
    
    };
}
#endif // xAODAnaHelpers_MuonContainer_H
