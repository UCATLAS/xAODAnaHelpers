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

      std::vector<int>   *m_ntrk;
      std::vector<float> *m_charge;

    
    };
}
#endif // xAODAnaHelpers_TauContainer_H
