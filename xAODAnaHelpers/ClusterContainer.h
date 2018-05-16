#ifndef xAODAnaHelpers_ClusterContainer_H
#define xAODAnaHelpers_ClusterContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include "xAODCaloEvent/CaloClusterContainer.h"

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODAnaHelpers/Cluster.h>
#include <xAODAnaHelpers/ParticleContainer.h>



namespace xAH {

    class ClusterContainer : public ParticleContainer<Cluster,HelperClasses::ClusterInfoSwitch>
    {
    public:
      ClusterContainer(const std::string& name = "cluster", const std::string& detailStr="");
      virtual ~ClusterContainer();

      virtual void setTree    (TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillCluster( const xAOD::CaloCluster* cluster );
      virtual void FillCluster( const xAOD::IParticle* particle );

      virtual void updateParticle(uint idx, Cluster& cluster);

//template<typename T>
//  void setBranch(TTree* tree, std::string varName, std::vector<T>* localVectorPtr);

    private:

      //
      // Vector branches

      // rapidity
      std::vector<float> *m_rapidity;
      
      // clean
      std::vector<float> *m_Timing;

    };
}



#endif // xAODAnaHelpers_ClusterContainer_H
