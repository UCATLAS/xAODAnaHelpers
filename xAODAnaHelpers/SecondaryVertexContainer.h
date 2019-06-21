#ifndef xAODAnaHelpers_SecondaryVertexContainer_H
#define xAODAnaHelpers_SecondaryVertexContainer_H

/* SecondaryVertexContainer is the secondary-vertex-specific derived class
   of the VertexContainer base class, responsible for handling VrtSecInclusive DV
   information (including associated tracks, matched truth vertices, etc.)
   to fill output ntuples; based partially on VSI performance code
   (https://gitlab.cern.ch/Atlas-Inner-Tracking/SecVtxPUBNote/tree/master/Code/VSIPerf) */

#include <vector>
#include <string>

#include <TTree.h>

#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/VertexContainer.h>

#include "xAODAnaHelpers/VertexContainer.h"
#include "xAODAnaHelpers/HelperClasses.h"

namespace xAH {

  class SecondaryVertexContainer : public VertexContainer < HelperClasses::SecondaryVertexInfoSwitch >
  {
  public:
    SecondaryVertexContainer ( const std::string& name = "secVtx", const std::string& detailStr = "", float units = 1e3,
			       bool mc = false, bool emtopo = true, bool pflow = false );
    virtual ~SecondaryVertexContainer ();

    virtual void setTree             ( TTree* tree );
    virtual void setBranches         ( TTree* tree );
    virtual void clear               ( );
    virtual void FillSecondaryVertex ( const xAOD::Vertex* secVtx, const std::string treeName = "" );


  private:   
    void recordTracks        ( const std::vector<const xAOD::TrackParticle*>& );
    //void processDVJets       ( const std::vector<const xAOD::TrackParticle*>& );
    void processCloseTruth   ( const xAOD::Vertex*, const std::vector<const xAOD::TrackParticle*>& );
    void processLinkedTruth  ( const xAOD::Vertex*, const std::vector<const xAOD::TrackParticle*>& );

    bool m_doEMTopo;
    bool m_doPFlow;

    
    // vector branches
    std::vector<int>*                m_ID;
    std::vector<float>*              m_x;
    std::vector<float>*              m_y;
    std::vector<float>*              m_z;
    std::vector<float>*              m_r;
    std::vector<float>*              m_pt;
    std::vector<float>*              m_eta;
    std::vector<float>*              m_phi;
    std::vector<float>*              m_mass;
    std::vector<float>*              m_mass_nonAssoc;
    std::vector<std::vector<float>>* m_covariance;
    std::vector<float>*              m_chi2;
    std::vector<float>*              m_direction;
    std::vector<int>*                m_charge;
    std::vector<float>*              m_H;
    std::vector<float>*              m_Ht;
    std::vector<float>*              m_minOpAng;
    std::vector<float>*              m_maxOpAng;
    std::vector<float>*              m_mind0;
    std::vector<float>*              m_maxd0;
    
    std::vector<float>*              m_minOneTrackRemovedMass;
    std::vector<std::vector<float>>* m_twoTracksMass;
    std::vector<std::vector<float>>* m_twoTracksMassRest;
    std::vector<std::vector<int>>*   m_twoTracksCharge;
    
    std::vector<uint32_t>*           m_ntrk;
    std::vector<uint32_t>*           m_ntrk_sel;
    std::vector<uint32_t>*           m_ntrk_assoc;

    std::vector<uint32_t>*           m_pass_sel;

    // do we want to save trimming info?
    // --> i.e. whether trimming was applied and values of trimming cuts... 
    // save p4 ??

    
    // tracks
    std::vector<std::vector<int>>*   m_trk_ID;
    std::vector<std::vector<float>>* m_trk_qOverP;
    std::vector<std::vector<float>>* m_trk_theta;
    std::vector<std::vector<float>>* m_trk_vz;
    std::vector<std::vector<float>>* m_trk_E;
    std::vector<std::vector<float>>* m_trk_M;
    std::vector<std::vector<float>>* m_trk_pt;
    std::vector<std::vector<float>>* m_trk_eta;
    std::vector<std::vector<float>>* m_trk_phi;
    std::vector<std::vector<float>>* m_trk_d0;
    std::vector<std::vector<float>>* m_trk_z0;
    std::vector<std::vector<float>>* m_trk_errd0;
    std::vector<std::vector<float>>* m_trk_errz0;
    std::vector<std::vector<float>>* m_trk_chi2;

    std::vector<std::vector<float>>* m_trk_pt_wrtSV;
    std::vector<std::vector<float>>* m_trk_eta_wrtSV;
    std::vector<std::vector<float>>* m_trk_phi_wrtSV;
    std::vector<std::vector<float>>* m_trk_d0_wrtSV;
    std::vector<std::vector<float>>* m_trk_z0_wrtSV;
    std::vector<std::vector<float>>* m_trk_errd0_wrtSV;
    std::vector<std::vector<float>>* m_trk_errz0_wrtSV;
    std::vector<std::vector<float>>* m_trk_chi2_toSV;

    std::vector<std::vector<int>>*      m_trk_charge;
    std::vector<std::vector<uint8_t>>*  m_trk_isFinal;
    std::vector<std::vector<uint8_t>>*  m_trk_isSelected;
    std::vector<std::vector<uint8_t>>*  m_trk_isAssociated;
    std::vector<std::vector<uint8_t>>*  m_trk_nPixelHits;
    std::vector<std::vector<uint8_t>>*  m_trk_nSCTHits;
    std::vector<std::vector<uint8_t>>*  m_trk_nTRTHits;
    std::vector<std::vector<uint8_t>>*  m_trk_nPixelBarrelLayers;
    std::vector<std::vector<uint8_t>>*  m_trk_nPixelEndCapLayers;
    std::vector<std::vector<uint8_t>>*  m_trk_nSCTBarrelLayers;
    std::vector<std::vector<uint8_t>>*  m_trk_nSCTEndCapLayers;
    std::vector<std::vector<uint32_t>>* m_trk_hitPattern;
    std::vector<std::vector<float>>*    m_trk_dEdx;
    std::vector<std::vector<uint8_t>>*  m_trk_nUsedHitsdEdx;
    std::vector<std::vector<uint8_t>>*  m_trk_nIBLOverflowsdEdx;
    std::vector<std::vector<float>>*    m_trk_radiusOfFirstHit;

    std::vector<std::vector<float>>*    m_trk_truthProb;
    std::vector<std::vector<int>>*      m_trk_truthID;
    std::vector<std::vector<int>>*      m_trk_truthBarcode;
    std::vector<std::vector<int>>*      m_trk_truthPdgId;
    std::vector<std::vector<uint8_t>>*  m_trk_truthPointsToClosestTV;
    std::vector<std::vector<uint8_t>>*  m_trk_truthPointsToMaxlinkTV;
    std::vector<std::vector<uint8_t>>*  m_trk_truthParentPointsToMaxlinkPTV;
    // do we want to look at other close / linked (P)TVs track points to (not necessary closest / max-linked) ??
    // --> would need to also save which TV track points to (i.e. index in vector of matched TVs)
    // also want to check if track's truth-link is descended from LLP
    // --> will need to account for truth pointing directly to TV/LLP or descending from...
    // --> --> i.e. trk_truthPVtxIs*TV vs trk_truthAncestorPVtxIs*TV ...
    // --> --> ... or trk_truthIsLLP vs trk_truthAncestorIsLLP
    // --> --> --> can keep singular branch for both (i.e. trk_truthIsFrom*TV or trk_truthIsLLPDesc) ...
    // --> --> --> ... and add branch holding generation of descendant, ...
    // --> --> --> ... where 0 means truth is direct child of TV/LLP, 1 is grandchild, etc.


    // jets
    // jets ... some alg that makes jets out of filtered tracks ?? probably don't need this...


    // close-matched truth vertices
    // close truth
    std::vector<std::vector<float>>*   m_closeTruth_distance;
    std::vector<std::vector<int>>*     m_closeTruth_ID;
    std::vector<std::vector<int>>*     m_closeTruth_barcode;
    
    // closest truth
    std::vector<float>*   m_closestTruth_distance;

    std::vector<int>*   m_closestTruth_ID;
    std::vector<float>* m_closestTruth_x;
    std::vector<float>* m_closestTruth_y;
    std::vector<float>* m_closestTruth_z;
    std::vector<float>* m_closestTruth_r;
    std::vector<float>* m_closestTruth_eta;
    std::vector<float>* m_closestTruth_phi;
    std::vector<int>*   m_closestTruth_barcode;

    std::vector<float>* m_closestTruth_inE;
    std::vector<float>* m_closestTruth_outE;
    // --> anything else from sumP4 worth saving ??

    std::vector<int>*   m_closestTruth_parent_ID;
    std::vector<float>* m_closestTruth_parent_pt;
    std::vector<float>* m_closestTruth_parent_eta;
    std::vector<float>* m_closestTruth_parent_phi;
    std::vector<float>* m_closestTruth_parent_E;
    std::vector<float>* m_closestTruth_parent_M;
    std::vector<float>* m_closestTruth_parent_charge;
    std::vector<int>*   m_closestTruth_parent_pdgId;
    std::vector<int>*   m_closestTruth_parent_status;
    std::vector<int>*   m_closestTruth_parent_barcode;

    std::vector<std::vector<int>>*     m_closestTruth_outP_ID;
    std::vector<std::vector<float>>*   m_closestTruth_outP_pt;
    std::vector<std::vector<float>>*   m_closestTruth_outP_eta;
    std::vector<std::vector<float>>*   m_closestTruth_outP_phi;
    std::vector<std::vector<float>>*   m_closestTruth_outP_E;
    std::vector<std::vector<float>>*   m_closestTruth_outP_M;
    std::vector<std::vector<float>>*   m_closestTruth_outP_charge;
    std::vector<std::vector<int>>*     m_closestTruth_outP_pdgId;
    std::vector<std::vector<int>>*     m_closestTruth_outP_status;
    std::vector<std::vector<int>>*     m_closestTruth_outP_barcode;
    std::vector<std::vector<uint8_t>>* m_closestTruth_outP_isReco;
    std::vector<std::vector<float>>*   m_closestTruth_outP_recoProb;
    std::vector<std::vector<int>>*     m_closestTruth_outP_recoID;
    std::vector<std::vector<uint8_t>>* m_closestTruth_outP_recoIsSelected;
    std::vector<std::vector<uint8_t>>* m_closestTruth_outP_recoIsAssociated;
    std::vector<std::vector<uint8_t>>* m_closestTruth_outP_isStable;
    std::vector<std::vector<uint8_t>>* m_closestTruth_outP_isInteracting;
    std::vector<std::vector<uint8_t>>* m_closestTruth_outP_isReconstructible;

    
    // track-truth-link-matched (parent) truth vertices
    // linked (parent) truth
    std::vector<std::vector<float>>*   m_linkTruth_score;
    std::vector<std::vector<int>>*     m_linkTruth_ID;
    std::vector<std::vector<int>>*     m_linkTruth_barcode;

    std::vector<std::vector<float>>*   m_linkParentTruth_score;
    std::vector<std::vector<int>>*     m_linkParentTruth_ID;
    std::vector<std::vector<int>>*     m_linkParentTruth_barcode;
    
    // max-linked (parent) truth
    std::vector<float>*   m_maxlinkTruth_score;

    std::vector<int>*   m_maxlinkTruth_ID;
    std::vector<float>* m_maxlinkTruth_x;
    std::vector<float>* m_maxlinkTruth_y;
    std::vector<float>* m_maxlinkTruth_z;
    std::vector<float>* m_maxlinkTruth_r;
    std::vector<float>* m_maxlinkTruth_eta;
    std::vector<float>* m_maxlinkTruth_phi;
    std::vector<int>*   m_maxlinkTruth_barcode;

    std::vector<float>* m_maxlinkTruth_inE;
    std::vector<float>* m_maxlinkTruth_outE;

    std::vector<int>*   m_maxlinkTruth_parent_ID;
    std::vector<float>* m_maxlinkTruth_parent_pt;
    std::vector<float>* m_maxlinkTruth_parent_eta;
    std::vector<float>* m_maxlinkTruth_parent_phi;
    std::vector<float>* m_maxlinkTruth_parent_E;
    std::vector<float>* m_maxlinkTruth_parent_M;
    std::vector<float>* m_maxlinkTruth_parent_charge;
    std::vector<int>*   m_maxlinkTruth_parent_pdgId;
    std::vector<int>*   m_maxlinkTruth_parent_status;
    std::vector<int>*   m_maxlinkTruth_parent_barcode;

    std::vector<std::vector<int>>*     m_maxlinkTruth_outP_ID;
    std::vector<std::vector<float>>*   m_maxlinkTruth_outP_pt;
    std::vector<std::vector<float>>*   m_maxlinkTruth_outP_eta;
    std::vector<std::vector<float>>*   m_maxlinkTruth_outP_phi;
    std::vector<std::vector<float>>*   m_maxlinkTruth_outP_E;
    std::vector<std::vector<float>>*   m_maxlinkTruth_outP_M;
    std::vector<std::vector<float>>*   m_maxlinkTruth_outP_charge;
    std::vector<std::vector<int>>*     m_maxlinkTruth_outP_pdgId;
    std::vector<std::vector<int>>*     m_maxlinkTruth_outP_status;
    std::vector<std::vector<int>>*     m_maxlinkTruth_outP_barcode;
    std::vector<std::vector<uint8_t>>* m_maxlinkTruth_outP_isReco;
    std::vector<std::vector<float>>*   m_maxlinkTruth_outP_recoProb;
    std::vector<std::vector<int>>*     m_maxlinkTruth_outP_recoID;
    std::vector<std::vector<uint8_t>>* m_maxlinkTruth_outP_recoIsSelected;
    std::vector<std::vector<uint8_t>>* m_maxlinkTruth_outP_recoIsAssociated;
    std::vector<std::vector<uint8_t>>* m_maxlinkTruth_outP_isStable;
    std::vector<std::vector<uint8_t>>* m_maxlinkTruth_outP_isInteracting;
    std::vector<std::vector<uint8_t>>* m_maxlinkTruth_outP_isReconstructible;


    std::vector<float>*   m_maxlinkParentTruth_score;

    std::vector<int>*   m_maxlinkParentTruth_ID;
    std::vector<float>* m_maxlinkParentTruth_x;
    std::vector<float>* m_maxlinkParentTruth_y;
    std::vector<float>* m_maxlinkParentTruth_z;
    std::vector<float>* m_maxlinkParentTruth_r;
    std::vector<float>* m_maxlinkParentTruth_eta;
    std::vector<float>* m_maxlinkParentTruth_phi;
    std::vector<int>*   m_maxlinkParentTruth_barcode;

    std::vector<float>* m_maxlinkParentTruth_inE;
    std::vector<float>* m_maxlinkParentTruth_outE;

    std::vector<int>*   m_maxlinkParentTruth_parent_ID;
    std::vector<float>* m_maxlinkParentTruth_parent_pt;
    std::vector<float>* m_maxlinkParentTruth_parent_eta;
    std::vector<float>* m_maxlinkParentTruth_parent_phi;
    std::vector<float>* m_maxlinkParentTruth_parent_E;
    std::vector<float>* m_maxlinkParentTruth_parent_M;
    std::vector<float>* m_maxlinkParentTruth_parent_charge;
    std::vector<int>*   m_maxlinkParentTruth_parent_pdgId;
    std::vector<int>*   m_maxlinkParentTruth_parent_status;
    std::vector<int>*   m_maxlinkParentTruth_parent_barcode;

    std::vector<std::vector<int>>*     m_maxlinkParentTruth_outP_ID;
    std::vector<std::vector<float>>*   m_maxlinkParentTruth_outP_pt;
    std::vector<std::vector<float>>*   m_maxlinkParentTruth_outP_eta;
    std::vector<std::vector<float>>*   m_maxlinkParentTruth_outP_phi;
    std::vector<std::vector<float>>*   m_maxlinkParentTruth_outP_E;
    std::vector<std::vector<float>>*   m_maxlinkParentTruth_outP_M;
    std::vector<std::vector<float>>*   m_maxlinkParentTruth_outP_charge;
    std::vector<std::vector<int>>*     m_maxlinkParentTruth_outP_pdgId;
    std::vector<std::vector<int>>*     m_maxlinkParentTruth_outP_status;
    std::vector<std::vector<int>>*     m_maxlinkParentTruth_outP_barcode;
    std::vector<std::vector<uint8_t>>* m_maxlinkParentTruth_outP_isReco;
    std::vector<std::vector<float>>*   m_maxlinkParentTruth_outP_recoProb;
    std::vector<std::vector<int>>*     m_maxlinkParentTruth_outP_recoID;
    std::vector<std::vector<uint8_t>>* m_maxlinkParentTruth_outP_recoIsSelected;
    std::vector<std::vector<uint8_t>>* m_maxlinkParentTruth_outP_recoIsAssociated;
    std::vector<std::vector<uint8_t>>* m_maxlinkParentTruth_outP_isStable;
    std::vector<std::vector<uint8_t>>* m_maxlinkParentTruth_outP_isInteracting;
    std::vector<std::vector<uint8_t>>* m_maxlinkParentTruth_outP_isReconstructible;


    // matched jets
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

#endif // xAODAnaHelpers_SecondaryVertexContainer_H
