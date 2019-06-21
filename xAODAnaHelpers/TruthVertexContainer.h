#ifndef xAODAnaHelpers_TruthVertexContainer_H
#define xAODAnaHelpers_TruthVertexContainer_H

/* TruthVertexContainer is the truth-vertex-specific derived class
   of the VertexContainer base class, responsible for handling truth vertex
   information (including LLP decay vertices of interest)
   to fill output ntuples; based partially on VSI performance code
   (https://gitlab.cern.ch/Atlas-Inner-Tracking/SecVtxPUBNote/tree/master/Code/VSIPerf) */

#include <vector>
#include <string>

#include <TTree.h>

#include <xAODTruth/TruthVertex.h>

#include "xAODAnaHelpers/VertexContainer.h"
#include "xAODAnaHelpers/HelperClasses.h"

namespace xAH {

  class TruthVertexContainer : public VertexContainer < HelperClasses::TruthVertexInfoSwitch >
  {
  public:
    TruthVertexContainer ( const std::string& name = "truthVtx", const std::string& detailStr = "", float units = 1e3,
			   bool mc = false, bool emtopo = true, bool pflow = false);
    virtual ~TruthVertexContainer ();

    virtual void setTree         ( TTree* tree );
    virtual void setBranches     ( TTree* tree );
    virtual void clear           ( );
    virtual void FillTruthVertex ( const xAOD::TruthVertex* truthVtx, const std::string treeName = "" );

    
  private:
    bool m_doEMTopo;
    bool m_doPFlow;
    
    // vector branches

    std::vector<int>*   m_ID;
    std::vector<float>* m_x;
    std::vector<float>* m_y;
    std::vector<float>* m_z;
    std::vector<float>* m_r;
    std::vector<float>* m_pt;
    std::vector<float>* m_eta;
    std::vector<float>* m_phi;
    std::vector<float>* m_mass;
    std::vector<int>*   m_nOutP;
    std::vector<int>*   m_barcode;

    std::vector<int>*   m_parent_ID; 
    std::vector<float>* m_parent_pt;
    std::vector<float>* m_parent_eta;
    std::vector<float>* m_parent_phi;
    std::vector<float>* m_parent_E;
    std::vector<float>* m_parent_M;
    std::vector<float>* m_parent_charge;
    std::vector<int>*   m_parent_pdgId;
    std::vector<int>*   m_parent_status;
    std::vector<int>*   m_parent_barcode;

    std::vector<int>*   m_parent_prod_ID;
    std::vector<float>* m_parent_prod_x;
    std::vector<float>* m_parent_prod_y;
    std::vector<float>* m_parent_prod_z;
    std::vector<float>* m_parent_prod_r;
    std::vector<float>* m_parent_prod_eta;
    std::vector<float>* m_parent_prod_phi;
    std::vector<int>*   m_parent_prod_barcode;

    std::vector<std::vector<int>>*     m_outP_ID;
    std::vector<std::vector<float>>*   m_outP_pt;
    std::vector<std::vector<float>>*   m_outP_eta;
    std::vector<std::vector<float>>*   m_outP_phi;
    std::vector<std::vector<float>>*   m_outP_E;
    std::vector<std::vector<float>>*   m_outP_M;
    std::vector<std::vector<float>>*   m_outP_charge;
    std::vector<std::vector<int>>*     m_outP_pdgId;
    std::vector<std::vector<int>>*     m_outP_status;
    std::vector<std::vector<int>>*     m_outP_barcode;
    std::vector<std::vector<uint8_t>>* m_outP_isReco;
    std::vector<std::vector<float>>*   m_outP_recoProb;
    std::vector<std::vector<int>>*     m_outP_recoID;
    std::vector<std::vector<uint8_t>>* m_outP_recoIsSelected;
    std::vector<std::vector<uint8_t>>* m_outP_recoIsAssociated;
    std::vector<std::vector<uint8_t>>* m_outP_isStable;
    std::vector<std::vector<uint8_t>>* m_outP_isInteracting;
    std::vector<std::vector<uint8_t>>* m_outP_isReconstructible;
    std::vector<std::vector<uint8_t>>* m_outP_isDark;
    // --> add branches for corresponding isReco tracks (??) ...
    // --> also want to look at POTENTIALLY RECONSTRUCTIBLE tracks ...

    // --> add matched reco vertex branches ...
    // --> --> need to do reco-to-truth matching + decorate truth vertices accordingly first
    // --> --> see VsiPerf efficiency processor ...

    // truth vertices matched to reco vertices
    std::vector<uint8_t>*            m_isCloseToRecoVtx;
    std::vector<std::vector<int>>*   m_closeRecoVtx_ID;
    std::vector<std::vector<float>>* m_closeRecoVtx_distance;
    std::vector<uint8_t>*            m_isClosestToRecoVtx;
    std::vector<std::vector<int>>*   m_closestRecoVtx_ID;
    std::vector<std::vector<float>>* m_closestRecoVtx_distance;
    std::vector<uint8_t>*            m_isLinkedToRecoVtx;
    std::vector<std::vector<int>>*   m_linkedRecoVtx_ID;
    std::vector<std::vector<float>>* m_linkedRecoVtx_score;
    std::vector<uint8_t>*            m_isMaxlinkedToRecoVtx;
    std::vector<int>*                m_maxlinkedRecoVtx_ID;
    std::vector<float>*              m_maxlinkedRecoVtx_score;
    std::vector<uint8_t>*            m_isLinkedParentToRecoVtx;
    std::vector<std::vector<int>>*   m_linkedParentRecoVtx_ID;
    std::vector<std::vector<float>>* m_linkedParentRecoVtx_score;
    std::vector<uint8_t>*            m_isMaxlinkedParentToRecoVtx;
    std::vector<std::vector<int>>*   m_maxlinkedParentRecoVtx_ID;
    std::vector<std::vector<float>>* m_maxlinkedParentRecoVtx_score;

    // truth vertices matched to jets
    std::vector<uint8_t>*            m_truthJetMatch;
    std::vector<std::vector<int>>*   m_truthJetMatch_ID;
    std::vector<std::vector<float>>* m_truthJetMatch_dR;
    std::vector<uint8_t>*            m_darkJetMatch;
    std::vector<std::vector<int>>*   m_darkJetMatch_ID;
    std::vector<std::vector<float>>* m_darkJetMatch_dR;
    std::vector<uint8_t>*            m_emtopoJetMatch;
    std::vector<std::vector<int>>*   m_emtopoJetMatch_ID;
    std::vector<std::vector<float>>* m_emtopoJetMatch_dR;
    std::vector<uint8_t>*            m_pflowJetMatch;
    std::vector<std::vector<int>>*   m_pflowJetMatch_ID;
    std::vector<std::vector<float>>* m_pflowJetMatch_dR;
    
  };
  
} // xAH

#endif // xAODAnaHelpers_TruthVertexContainer_H
