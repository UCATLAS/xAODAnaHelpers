#ifndef xAODAnaHelpers_L1JetContainer_H
#define xAODAnaHelpers_L1JetContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODTrigger/JetRoIContainer.h>

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
      virtual void FillL1Jets( const xAOD::JetRoIContainer* jets, bool sort);
      virtual void updateParticle(uint idx, Jet& jet);

    private:
      // Vector branches
      std::vector<float>* m_l1Jet_et8x8;
      std::vector<float>* m_l1Jet_eta;
      std::vector<float>* m_l1Jet_phi;
    };

}

#endif // xAODAnaHelpers_L1JetContainer_H
