#ifndef xAODAnaHelpers_TruthVertexContainer_H
#define xAODAnaHelpers_TruthVertexContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include "xAODTruth/TruthVertexContainer.h"

namespace xAH {

    class TruthVertexContainer
    {
    public:
      TruthVertexContainer(const std::string& detailStr, const std::string& name = "vertex");
      virtual ~TruthVertexContainer();

      virtual void setTree    (TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillTruthVertices( const xAOD::TruthVertexContainer* vertices);

      std::string m_name;

      std::string branchName(const std::string& varName)
      {
        std::string name = m_name + "_" + varName;
        return name;
      }

      template <typename T_BR> void connectBranch(TTree *tree, const std::string& branch, std::vector<T_BR> **variable)
      {
        std::string name = branchName(branch);
        if(*variable) { delete (*variable); (*variable)=0; }
        if(tree->GetBranch(name.c_str()))
          {
            (*variable)=new std::vector<T_BR>();
            tree->SetBranchStatus  (name.c_str()  , 1);
            tree->SetBranchAddress (name.c_str()  , variable);
          }
      }

      template<typename T> void setBranch(TTree* tree, std::string varName, std::vector<T>* localVectorPtr){
        std::string name = branchName(varName);
        tree->Branch(name.c_str(),        localVectorPtr);
      }

    private:
      // Vector branches
      std::vector<float>* m_vertex_x;
      std::vector<float>* m_vertex_y;
      std::vector<float>* m_vertex_z;
      std::string         m_detailStr;
    };

}

#endif // xAODAnaHelpers_TruthVertexContainer_H
