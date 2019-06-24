#include <iostream>
#include <TVector3.h>
#include <TLorentzVector.h>

#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODTruth/TruthVertexContainer.h>

#include "xAODAnaHelpers/SecondaryVertexContainer.h"
#include "xAODAnaHelpers/VsiBonsaiTool.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

using namespace xAH;

SecondaryVertexContainer :: SecondaryVertexContainer ( const std::string& name, const std::string& detailStr, float units,
                   bool mc, bool emtopo, bool pflow ) :
  VertexContainer ( name, detailStr, units, mc )
{

  if ( m_debug ) std::cout << " Setup Secondary Vertex Container " << std::endl;

  m_doEMTopo = emtopo;
  m_doPFlow  = pflow;
  
  m_ID            = new std::vector<int>;
  m_x             = new std::vector<float>;
  m_y             = new std::vector<float>;
  m_z             = new std::vector<float>;
  m_r             = new std::vector<float>;
  m_pt            = new std::vector<float>;
  m_eta           = new std::vector<float>;
  m_phi           = new std::vector<float>;
  m_mass          = new std::vector<float>;
  m_mass_nonAssoc = new std::vector<float>;
  m_covariance    = new std::vector<std::vector<float>>;
  m_chi2          = new std::vector<float>;
  m_direction     = new std::vector<float>;
  m_charge        = new std::vector<int>;
  m_H             = new std::vector<float>;
  m_Ht            = new std::vector<float>;
  m_minOpAng      = new std::vector<float>;
  m_maxOpAng      = new std::vector<float>;
  m_mind0         = new std::vector<float>;
  m_maxd0         = new std::vector<float>;
  
  m_minOneTrackRemovedMass = new std::vector<float>;
  m_twoTracksMass          = new std::vector<std::vector<float>>;
  m_twoTracksMassRest      = new std::vector<std::vector<float>>;
  m_twoTracksCharge        = new std::vector<std::vector<int>>;
  
  m_ntrk       = new std::vector<uint32_t>;
  m_ntrk_sel   = new std::vector<uint32_t>;
  m_ntrk_assoc = new std::vector<uint32_t>;

  m_pass_sel   = new std::vector<uint32_t>;

  // tracks
  if ( m_infoSwitch.m_tracks ) {
    m_trk_ID     = new std::vector<std::vector<int>>;
    m_trk_qOverP = new std::vector<std::vector<float>>;
    m_trk_theta  = new std::vector<std::vector<float>>;
    m_trk_vz     = new std::vector<std::vector<float>>;
    m_trk_E      = new std::vector<std::vector<float>>;
    m_trk_M      = new std::vector<std::vector<float>>;
    m_trk_pt     = new std::vector<std::vector<float>>;
    m_trk_eta    = new std::vector<std::vector<float>>;
    m_trk_phi    = new std::vector<std::vector<float>>;
    m_trk_d0     = new std::vector<std::vector<float>>;
    m_trk_z0     = new std::vector<std::vector<float>>;
    m_trk_errd0  = new std::vector<std::vector<float>>;
    m_trk_errz0  = new std::vector<std::vector<float>>;
    m_trk_chi2   = new std::vector<std::vector<float>>;

    m_trk_pt_wrtSV    = new std::vector<std::vector<float>>;
    m_trk_eta_wrtSV   = new std::vector<std::vector<float>>;
    m_trk_phi_wrtSV   = new std::vector<std::vector<float>>;
    m_trk_d0_wrtSV    = new std::vector<std::vector<float>>;
    m_trk_z0_wrtSV    = new std::vector<std::vector<float>>;
    m_trk_errd0_wrtSV = new std::vector<std::vector<float>>;
    m_trk_errz0_wrtSV = new std::vector<std::vector<float>>;
    m_trk_chi2_toSV   = new std::vector<std::vector<float>>;

    m_trk_charge             = new std::vector<std::vector<int>>;
    m_trk_isFinal            = new std::vector<std::vector<uint8_t>>;
    m_trk_isSelected         = new std::vector<std::vector<uint8_t>>;
    m_trk_isAssociated       = new std::vector<std::vector<uint8_t>>;
    m_trk_nPixelHits         = new std::vector<std::vector<uint8_t>>;
    m_trk_nSCTHits           = new std::vector<std::vector<uint8_t>>;
    m_trk_nTRTHits           = new std::vector<std::vector<uint8_t>>;
    m_trk_nPixelBarrelLayers = new std::vector<std::vector<uint8_t>>;
    m_trk_nPixelEndCapLayers = new std::vector<std::vector<uint8_t>>;
    m_trk_nSCTBarrelLayers   = new std::vector<std::vector<uint8_t>>;
    m_trk_nSCTEndCapLayers   = new std::vector<std::vector<uint8_t>>;
    m_trk_hitPattern         = new std::vector<std::vector<uint32_t>>;
    m_trk_dEdx               = new std::vector<std::vector<float>>;
    m_trk_nUsedHitsdEdx      = new std::vector<std::vector<uint8_t>>;
    m_trk_nIBLOverflowsdEdx  = new std::vector<std::vector<uint8_t>>;
    m_trk_radiusOfFirstHit   = new std::vector<std::vector<float>>;
 
    if ( m_infoSwitch.m_truth || m_infoSwitch.m_close || m_infoSwitch.m_linked ) {
      m_trk_truthProb                       = new std::vector<std::vector<float>>;
      m_trk_truthID                         = new std::vector<std::vector<int>>;
      m_trk_truthBarcode                    = new std::vector<std::vector<int>>;
      m_trk_truthPdgId                      = new std::vector<std::vector<int>>;
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_close )
        m_trk_truthPointsToClosestTV        = new std::vector<std::vector<uint8_t>>;
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
        m_trk_truthPointsToMaxlinkTV        = new std::vector<std::vector<uint8_t>>;
        m_trk_truthParentPointsToMaxlinkPTV = new std::vector<std::vector<uint8_t>>;
      }
    }
  }

  // close matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_close ) {
    m_closeTruth_distance               = new std::vector<std::vector<float>>;
    m_closeTruth_ID                     = new std::vector<std::vector<int>>;
    m_closeTruth_barcode                = new std::vector<std::vector<int>>;
    
    m_closestTruth_distance               = new std::vector<float>;    

    m_closestTruth_ID      = new std::vector<int>;
    m_closestTruth_x       = new std::vector<float>;
    m_closestTruth_y       = new std::vector<float>;
    m_closestTruth_z       = new std::vector<float>;
    m_closestTruth_r       = new std::vector<float>;
    m_closestTruth_eta     = new std::vector<float>;
    m_closestTruth_phi     = new std::vector<float>;
    m_closestTruth_barcode = new std::vector<int>;

    m_closestTruth_inE  = new std::vector<float>;
    m_closestTruth_outE = new std::vector<float>;

    m_closestTruth_parent_ID      = new std::vector<int>;
    m_closestTruth_parent_pt      = new std::vector<float>;
    m_closestTruth_parent_eta     = new std::vector<float>;
    m_closestTruth_parent_phi     = new std::vector<float>;
    m_closestTruth_parent_E       = new std::vector<float>;
    m_closestTruth_parent_M       = new std::vector<float>;
    m_closestTruth_parent_charge  = new std::vector<float>;
    m_closestTruth_parent_pdgId   = new std::vector<int>;
    m_closestTruth_parent_status  = new std::vector<int>;
    m_closestTruth_parent_barcode = new std::vector<int>;

    m_closestTruth_outP_ID                = new std::vector<std::vector<int>>;
    m_closestTruth_outP_pt                = new std::vector<std::vector<float>>;
    m_closestTruth_outP_eta               = new std::vector<std::vector<float>>;
    m_closestTruth_outP_phi               = new std::vector<std::vector<float>>;
    m_closestTruth_outP_E                 = new std::vector<std::vector<float>>;
    m_closestTruth_outP_M                 = new std::vector<std::vector<float>>;
    m_closestTruth_outP_charge            = new std::vector<std::vector<float>>;
    m_closestTruth_outP_pdgId             = new std::vector<std::vector<int>>;
    m_closestTruth_outP_status            = new std::vector<std::vector<int>>;
    m_closestTruth_outP_barcode           = new std::vector<std::vector<int>>;
    m_closestTruth_outP_isReco            = new std::vector<std::vector<uint8_t>>;
    m_closestTruth_outP_recoProb          = new std::vector<std::vector<float>>;
    m_closestTruth_outP_recoID            = new std::vector<std::vector<int>>;
    m_closestTruth_outP_recoIsSelected    = new std::vector<std::vector<uint8_t>>;
    m_closestTruth_outP_recoIsAssociated  = new std::vector<std::vector<uint8_t>>;
    m_closestTruth_outP_isStable          = new std::vector<std::vector<uint8_t>>;
    m_closestTruth_outP_isInteracting     = new std::vector<std::vector<uint8_t>>;
    m_closestTruth_outP_isReconstructible = new std::vector<std::vector<uint8_t>>;
  }
  // track-truth-link matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
    m_linkTruth_score                  = new std::vector<std::vector<float>>;
    m_linkTruth_ID                     = new std::vector<std::vector<int>>;
    m_linkTruth_barcode                = new std::vector<std::vector<int>>;
    
    m_maxlinkTruth_score                  = new std::vector<float>;  

    m_maxlinkTruth_ID      = new std::vector<int>;
    m_maxlinkTruth_x       = new std::vector<float>;
    m_maxlinkTruth_y       = new std::vector<float>;
    m_maxlinkTruth_z       = new std::vector<float>;
    m_maxlinkTruth_r       = new std::vector<float>;
    m_maxlinkTruth_eta     = new std::vector<float>;
    m_maxlinkTruth_phi     = new std::vector<float>;
    m_maxlinkTruth_barcode = new std::vector<int>;

    m_maxlinkTruth_inE  = new std::vector<float>;
    m_maxlinkTruth_outE = new std::vector<float>;

    m_maxlinkTruth_parent_ID      = new std::vector<int>;
    m_maxlinkTruth_parent_pt      = new std::vector<float>;
    m_maxlinkTruth_parent_eta     = new std::vector<float>;
    m_maxlinkTruth_parent_phi     = new std::vector<float>;
    m_maxlinkTruth_parent_E       = new std::vector<float>;
    m_maxlinkTruth_parent_M       = new std::vector<float>;
    m_maxlinkTruth_parent_charge  = new std::vector<float>;
    m_maxlinkTruth_parent_pdgId   = new std::vector<int>;
    m_maxlinkTruth_parent_status  = new std::vector<int>;
    m_maxlinkTruth_parent_barcode = new std::vector<int>;

    m_maxlinkTruth_outP_ID                = new std::vector<std::vector<int>>;
    m_maxlinkTruth_outP_pt                = new std::vector<std::vector<float>>;
    m_maxlinkTruth_outP_eta               = new std::vector<std::vector<float>>;
    m_maxlinkTruth_outP_phi               = new std::vector<std::vector<float>>;
    m_maxlinkTruth_outP_E                 = new std::vector<std::vector<float>>;
    m_maxlinkTruth_outP_M                 = new std::vector<std::vector<float>>;
    m_maxlinkTruth_outP_charge            = new std::vector<std::vector<float>>;
    m_maxlinkTruth_outP_pdgId             = new std::vector<std::vector<int>>;
    m_maxlinkTruth_outP_status            = new std::vector<std::vector<int>>;
    m_maxlinkTruth_outP_barcode           = new std::vector<std::vector<int>>;
    m_maxlinkTruth_outP_isReco            = new std::vector<std::vector<uint8_t>>;
    m_maxlinkTruth_outP_recoProb          = new std::vector<std::vector<float>>;
    m_maxlinkTruth_outP_recoID            = new std::vector<std::vector<int>>;
    m_maxlinkTruth_outP_recoIsSelected    = new std::vector<std::vector<uint8_t>>;
    m_maxlinkTruth_outP_recoIsAssociated  = new std::vector<std::vector<uint8_t>>;
    m_maxlinkTruth_outP_isStable          = new std::vector<std::vector<uint8_t>>;
    m_maxlinkTruth_outP_isInteracting     = new std::vector<std::vector<uint8_t>>;
    m_maxlinkTruth_outP_isReconstructible = new std::vector<std::vector<uint8_t>>;
    

    m_linkParentTruth_score                  = new std::vector<std::vector<float>>;
    m_linkParentTruth_ID                     = new std::vector<std::vector<int>>;
    m_linkParentTruth_barcode                = new std::vector<std::vector<int>>;
    
    m_maxlinkParentTruth_score               = new std::vector<float>;    

    m_maxlinkParentTruth_ID      = new std::vector<int>;
    m_maxlinkParentTruth_x       = new std::vector<float>;
    m_maxlinkParentTruth_y       = new std::vector<float>;
    m_maxlinkParentTruth_z       = new std::vector<float>;
    m_maxlinkParentTruth_r       = new std::vector<float>;
    m_maxlinkParentTruth_eta     = new std::vector<float>;
    m_maxlinkParentTruth_phi     = new std::vector<float>;
    m_maxlinkParentTruth_barcode = new std::vector<int>;

    m_maxlinkParentTruth_inE  = new std::vector<float>;
    m_maxlinkParentTruth_outE = new std::vector<float>;

    m_maxlinkParentTruth_parent_ID      = new std::vector<int>;
    m_maxlinkParentTruth_parent_pt      = new std::vector<float>;
    m_maxlinkParentTruth_parent_eta     = new std::vector<float>;
    m_maxlinkParentTruth_parent_phi     = new std::vector<float>;
    m_maxlinkParentTruth_parent_E       = new std::vector<float>;
    m_maxlinkParentTruth_parent_M       = new std::vector<float>;
    m_maxlinkParentTruth_parent_charge  = new std::vector<float>;
    m_maxlinkParentTruth_parent_pdgId   = new std::vector<int>;
    m_maxlinkParentTruth_parent_status  = new std::vector<int>;
    m_maxlinkParentTruth_parent_barcode = new std::vector<int>;

    m_maxlinkParentTruth_outP_ID                = new std::vector<std::vector<int>>;
    m_maxlinkParentTruth_outP_pt                = new std::vector<std::vector<float>>;
    m_maxlinkParentTruth_outP_eta               = new std::vector<std::vector<float>>;
    m_maxlinkParentTruth_outP_phi               = new std::vector<std::vector<float>>;
    m_maxlinkParentTruth_outP_E                 = new std::vector<std::vector<float>>;
    m_maxlinkParentTruth_outP_M                 = new std::vector<std::vector<float>>;
    m_maxlinkParentTruth_outP_charge            = new std::vector<std::vector<float>>;
    m_maxlinkParentTruth_outP_pdgId             = new std::vector<std::vector<int>>;
    m_maxlinkParentTruth_outP_status            = new std::vector<std::vector<int>>;
    m_maxlinkParentTruth_outP_barcode           = new std::vector<std::vector<int>>;
    m_maxlinkParentTruth_outP_isReco            = new std::vector<std::vector<uint8_t>>;
    m_maxlinkParentTruth_outP_recoProb          = new std::vector<std::vector<float>>;
    m_maxlinkParentTruth_outP_recoID            = new std::vector<std::vector<int>>;
    m_maxlinkParentTruth_outP_recoIsSelected    = new std::vector<std::vector<uint8_t>>;
    m_maxlinkParentTruth_outP_recoIsAssociated  = new std::vector<std::vector<uint8_t>>;
    m_maxlinkParentTruth_outP_isStable          = new std::vector<std::vector<uint8_t>>;
    m_maxlinkParentTruth_outP_isInteracting     = new std::vector<std::vector<uint8_t>>;
    m_maxlinkParentTruth_outP_isReconstructible = new std::vector<std::vector<uint8_t>>;
  }

  // matched to jets
  if ( m_infoSwitch.m_jetMatched ) {
    if ( m_mc ) {
      m_truthJetMatch     = new std::vector<uint8_t>;
      m_truthJetMatch_ID  = new std::vector<std::vector<int>>;
      m_truthJetMatch_dR  = new std::vector<std::vector<float>>;
      m_darkJetMatch      = new std::vector<uint8_t>;
      m_darkJetMatch_ID   = new std::vector<std::vector<int>>;
      m_darkJetMatch_dR   = new std::vector<std::vector<float>>;
    }
    if ( m_doEMTopo ) {
      m_emtopoJetMatch    = new std::vector<uint8_t>;
      m_emtopoJetMatch_ID = new std::vector<std::vector<int>>;
      m_emtopoJetMatch_dR = new std::vector<std::vector<float>>;
    }
    if ( m_doPFlow ) {
      m_pflowJetMatch     = new std::vector<uint8_t>;
      m_pflowJetMatch_ID  = new std::vector<std::vector<int>>;
      m_pflowJetMatch_dR  = new std::vector<std::vector<float>>;
    }
  }
}


SecondaryVertexContainer :: ~SecondaryVertexContainer ()
{
  if ( m_debug ) std::cout << " Deleting Secondary Vertex Container " << std::endl;

  delete m_ID;
  delete m_x;
  delete m_y;
  delete m_z;
  delete m_r;
  delete m_pt;
  delete m_eta;
  delete m_phi;
  delete m_mass;
  delete m_mass_nonAssoc;
  delete m_covariance;
  delete m_chi2;
  delete m_direction;
  delete m_charge;
  delete m_H;
  delete m_Ht;
  delete m_minOpAng;
  delete m_maxOpAng;
  delete m_mind0;
  delete m_maxd0;
  
  delete m_minOneTrackRemovedMass;
  delete m_twoTracksMass;
  delete m_twoTracksMassRest;
  delete m_twoTracksCharge;
  
  delete m_ntrk;
  delete m_ntrk_sel;
  delete m_ntrk_assoc;

  delete m_pass_sel;

  // tracks
  if ( m_infoSwitch.m_tracks ) {
    delete m_trk_ID;
    delete m_trk_qOverP;
    delete m_trk_theta;
    delete m_trk_vz;
    delete m_trk_E;
    delete m_trk_M;
    delete m_trk_pt;
    delete m_trk_eta;
    delete m_trk_phi;
    delete m_trk_d0;
    delete m_trk_z0;
    delete m_trk_errd0;
    delete m_trk_errz0;
    delete m_trk_chi2;

    delete m_trk_pt_wrtSV;
    delete m_trk_eta_wrtSV;
    delete m_trk_phi_wrtSV;
    delete m_trk_d0_wrtSV;
    delete m_trk_z0_wrtSV;
    delete m_trk_errd0_wrtSV;
    delete m_trk_errz0_wrtSV;
    delete m_trk_chi2_toSV;

    delete m_trk_charge;
    delete m_trk_isFinal;
    delete m_trk_isSelected;
    delete m_trk_isAssociated;
    delete m_trk_nPixelHits;
    delete m_trk_nSCTHits;
    delete m_trk_nTRTHits;
    delete m_trk_nPixelBarrelLayers;
    delete m_trk_nPixelEndCapLayers;
    delete m_trk_nSCTBarrelLayers;
    delete m_trk_nSCTEndCapLayers;
    delete m_trk_hitPattern;
    delete m_trk_dEdx;
    delete m_trk_nUsedHitsdEdx;
    delete m_trk_nIBLOverflowsdEdx;
    delete m_trk_radiusOfFirstHit;

    if ( m_infoSwitch.m_truth || m_infoSwitch.m_close || m_infoSwitch.m_linked ) {
      delete m_trk_truthProb;
      delete m_trk_truthID;
      delete m_trk_truthBarcode;
      delete m_trk_truthPdgId;
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_close )
        delete m_trk_truthPointsToClosestTV;
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
        delete m_trk_truthPointsToMaxlinkTV;
        delete m_trk_truthParentPointsToMaxlinkPTV;
      }
    }
  }

  // close matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_close ) {
    delete m_closeTruth_distance;
    delete m_closeTruth_ID;
    delete m_closeTruth_barcode;
    
    delete m_closestTruth_distance;

    delete m_closestTruth_ID;
    delete m_closestTruth_x;
    delete m_closestTruth_y;
    delete m_closestTruth_z;
    delete m_closestTruth_r;
    delete m_closestTruth_eta;
    delete m_closestTruth_phi;
    delete m_closestTruth_barcode;

    delete m_closestTruth_inE;
    delete m_closestTruth_outE;

    delete m_closestTruth_parent_ID;
    delete m_closestTruth_parent_pt;
    delete m_closestTruth_parent_eta;
    delete m_closestTruth_parent_phi;
    delete m_closestTruth_parent_E;
    delete m_closestTruth_parent_M;
    delete m_closestTruth_parent_charge;
    delete m_closestTruth_parent_pdgId;
    delete m_closestTruth_parent_barcode;
    delete m_closestTruth_parent_status;

    delete m_closestTruth_outP_ID;
    delete m_closestTruth_outP_pt;
    delete m_closestTruth_outP_eta;
    delete m_closestTruth_outP_phi;
    delete m_closestTruth_outP_E;
    delete m_closestTruth_outP_M;
    delete m_closestTruth_outP_charge;
    delete m_closestTruth_outP_pdgId;
    delete m_closestTruth_outP_status;
    delete m_closestTruth_outP_barcode;
    delete m_closestTruth_outP_isReco;
    delete m_closestTruth_outP_recoProb;
    delete m_closestTruth_outP_recoID;
    delete m_closestTruth_outP_recoIsSelected;
    delete m_closestTruth_outP_recoIsAssociated;
    delete m_closestTruth_outP_isStable;
    delete m_closestTruth_outP_isInteracting;
    delete m_closestTruth_outP_isReconstructible;
  }
  // track-truth-link matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
    delete m_linkTruth_score;
    delete m_linkTruth_ID;
    delete m_linkTruth_barcode;
    
    delete m_maxlinkTruth_score;   

    delete m_maxlinkTruth_ID;
    delete m_maxlinkTruth_x;
    delete m_maxlinkTruth_y;
    delete m_maxlinkTruth_z;
    delete m_maxlinkTruth_r;
    delete m_maxlinkTruth_eta;
    delete m_maxlinkTruth_phi;
    delete m_maxlinkTruth_barcode;

    delete m_maxlinkTruth_inE;
    delete m_maxlinkTruth_outE;

    delete m_maxlinkTruth_parent_ID;
    delete m_maxlinkTruth_parent_pt;
    delete m_maxlinkTruth_parent_eta;
    delete m_maxlinkTruth_parent_phi;
    delete m_maxlinkTruth_parent_E;
    delete m_maxlinkTruth_parent_M;
    delete m_maxlinkTruth_parent_charge;
    delete m_maxlinkTruth_parent_pdgId;
    delete m_maxlinkTruth_parent_status;
    delete m_maxlinkTruth_parent_barcode;

    delete m_maxlinkTruth_outP_ID;
    delete m_maxlinkTruth_outP_pt;
    delete m_maxlinkTruth_outP_eta;
    delete m_maxlinkTruth_outP_phi;
    delete m_maxlinkTruth_outP_E;
    delete m_maxlinkTruth_outP_M;
    delete m_maxlinkTruth_outP_charge;
    delete m_maxlinkTruth_outP_pdgId;
    delete m_maxlinkTruth_outP_status;
    delete m_maxlinkTruth_outP_barcode;
    delete m_maxlinkTruth_outP_isReco;
    delete m_maxlinkTruth_outP_recoProb;
    delete m_maxlinkTruth_outP_recoID;
    delete m_maxlinkTruth_outP_recoIsSelected;
    delete m_maxlinkTruth_outP_recoIsAssociated;
    delete m_maxlinkTruth_outP_isStable;
    delete m_maxlinkTruth_outP_isInteracting;
    delete m_maxlinkTruth_outP_isReconstructible;


    delete m_linkParentTruth_score;
    delete m_linkParentTruth_ID;
    delete m_linkParentTruth_barcode;
    
    delete m_maxlinkParentTruth_score;   

    delete m_maxlinkParentTruth_ID;
    delete m_maxlinkParentTruth_x;
    delete m_maxlinkParentTruth_y;
    delete m_maxlinkParentTruth_z;
    delete m_maxlinkParentTruth_r;
    delete m_maxlinkParentTruth_eta;
    delete m_maxlinkParentTruth_phi;
    delete m_maxlinkParentTruth_barcode;

    delete m_maxlinkParentTruth_inE;
    delete m_maxlinkParentTruth_outE;

    delete m_maxlinkParentTruth_parent_ID;
    delete m_maxlinkParentTruth_parent_pt;
    delete m_maxlinkParentTruth_parent_eta;
    delete m_maxlinkParentTruth_parent_phi;
    delete m_maxlinkParentTruth_parent_E;
    delete m_maxlinkParentTruth_parent_M;
    delete m_maxlinkParentTruth_parent_charge;
    delete m_maxlinkParentTruth_parent_pdgId;
    delete m_maxlinkParentTruth_parent_status;
    delete m_maxlinkParentTruth_parent_barcode;

    delete m_maxlinkParentTruth_outP_ID;
    delete m_maxlinkParentTruth_outP_pt;
    delete m_maxlinkParentTruth_outP_eta;
    delete m_maxlinkParentTruth_outP_phi;
    delete m_maxlinkParentTruth_outP_E;
    delete m_maxlinkParentTruth_outP_M;
    delete m_maxlinkParentTruth_outP_charge;
    delete m_maxlinkParentTruth_outP_pdgId;
    delete m_maxlinkParentTruth_outP_status;
    delete m_maxlinkParentTruth_outP_barcode;
    delete m_maxlinkParentTruth_outP_isReco;
    delete m_maxlinkParentTruth_outP_recoProb;
    delete m_maxlinkParentTruth_outP_recoID;
    delete m_maxlinkParentTruth_outP_recoIsSelected;
    delete m_maxlinkParentTruth_outP_recoIsAssociated;
    delete m_maxlinkParentTruth_outP_isStable;
    delete m_maxlinkParentTruth_outP_isInteracting;
    delete m_maxlinkParentTruth_outP_isReconstructible;
  }
  // matched to jets
  if ( m_infoSwitch.m_jetMatched ) {
    if ( m_mc ) {
      delete m_truthJetMatch;
      delete m_truthJetMatch_ID;
      delete m_truthJetMatch_dR;
      delete m_darkJetMatch;
      delete m_darkJetMatch_ID;
      delete m_darkJetMatch_dR;
    }
    if ( m_doEMTopo ) {
      delete m_emtopoJetMatch;
      delete m_emtopoJetMatch_ID;
      delete m_emtopoJetMatch_dR;
    }
    if ( m_doPFlow ) {
      delete m_pflowJetMatch;
      delete m_pflowJetMatch_ID;
      delete m_pflowJetMatch_dR;
    }
  }
}


void SecondaryVertexContainer :: setTree ( TTree* tree )
{
  if ( m_debug ) std::cout << "SecondaryVertex : setTree()" << std::endl;
  
  VertexContainer::setTree ( tree );

  connectBranch<int>                ( tree, "ID",            &m_ID            );
  connectBranch<float>              ( tree, "x",             &m_x             );
  connectBranch<float>              ( tree, "y",             &m_y             );
  connectBranch<float>              ( tree, "z",             &m_z             );
  connectBranch<float>              ( tree, "r",             &m_r             );
  connectBranch<float>              ( tree, "pt",            &m_pt            );
  connectBranch<float>              ( tree, "eta",           &m_eta           );
  connectBranch<float>              ( tree, "phi",           &m_phi           );
  connectBranch<float>              ( tree, "mass",          &m_mass          );
  connectBranch<float>              ( tree, "mass_nonAssoc", &m_mass_nonAssoc );
  connectBranch<std::vector<float>> ( tree, "covariance",    &m_covariance    );
  connectBranch<float>              ( tree, "chi2",          &m_chi2          );
  connectBranch<float>              ( tree, "direction",     &m_direction     );
  connectBranch<int>                ( tree, "charge",        &m_charge        );
  connectBranch<float>              ( tree, "H",             &m_H             );
  connectBranch<float>              ( tree, "Ht",            &m_Ht            );
  connectBranch<float>              ( tree, "minOpAng",      &m_minOpAng      );
  connectBranch<float>              ( tree, "maxOpAng",      &m_maxOpAng      );
  connectBranch<float>              ( tree, "mind0",         &m_mind0         );
  connectBranch<float>              ( tree, "maxd0",         &m_maxd0         );
  
  connectBranch<float>              ( tree, "minOneTrackRemovedMass", &m_minOneTrackRemovedMass );
  connectBranch<std::vector<float>> ( tree, "twoTracksMass",          &m_twoTracksMass          );
  connectBranch<std::vector<float>> ( tree, "twoTracksMassRest",      &m_twoTracksMassRest      );
  connectBranch<std::vector<int>>   ( tree, "twoTracksCharge",        &m_twoTracksCharge        );
  
  connectBranch<uint32_t> ( tree, "ntrk",       &m_ntrk       );
  connectBranch<uint32_t> ( tree, "ntrk_sel",   &m_ntrk_sel   );
  connectBranch<uint32_t> ( tree, "ntrk_assoc", &m_ntrk_assoc );

  connectBranch<uint32_t> ( tree, "pass_sel", &m_pass_sel );

  // tracks
  if ( m_infoSwitch.m_tracks ) {
    connectBranch<std::vector<int>>   ( tree, "trk_ID",     &m_trk_ID     );
    connectBranch<std::vector<float>> ( tree, "trk_qOverP", &m_trk_qOverP );
    connectBranch<std::vector<float>> ( tree, "trk_theta",  &m_trk_theta  );
    connectBranch<std::vector<float>> ( tree, "trk_vz",     &m_trk_vz     );
    connectBranch<std::vector<float>> ( tree, "trk_E",      &m_trk_E      );
    connectBranch<std::vector<float>> ( tree, "trk_M",      &m_trk_M      );
    connectBranch<std::vector<float>> ( tree, "trk_pt",     &m_trk_pt     );
    connectBranch<std::vector<float>> ( tree, "trk_eta",    &m_trk_eta    );
    connectBranch<std::vector<float>> ( tree, "trk_phi",    &m_trk_phi    );
    connectBranch<std::vector<float>> ( tree, "trk_d0",     &m_trk_d0     );
    connectBranch<std::vector<float>> ( tree, "trk_z0",     &m_trk_z0     );
    connectBranch<std::vector<float>> ( tree, "trk_errd0",  &m_trk_errd0  );
    connectBranch<std::vector<float>> ( tree, "trk_errz0",  &m_trk_errz0  );
    connectBranch<std::vector<float>> ( tree, "trk_chi2",   &m_trk_chi2   );

    connectBranch<std::vector<float>> ( tree, "trk_pt_wrtSV",    &m_trk_pt_wrtSV    );
    connectBranch<std::vector<float>> ( tree, "trk_eta_wrtSV",   &m_trk_eta_wrtSV   );
    connectBranch<std::vector<float>> ( tree, "trk_phi_wrtSV",   &m_trk_phi_wrtSV   );
    connectBranch<std::vector<float>> ( tree, "trk_d0_wrtSV",    &m_trk_d0_wrtSV    );
    connectBranch<std::vector<float>> ( tree, "trk_z0_wrtSV",    &m_trk_z0_wrtSV    );
    connectBranch<std::vector<float>> ( tree, "trk_errd0_wrtSV", &m_trk_errd0_wrtSV );
    connectBranch<std::vector<float>> ( tree, "trk_errz0_wrtSV", &m_trk_errz0_wrtSV );
    connectBranch<std::vector<float>> ( tree, "trk_chi2_toSV",   &m_trk_chi2_toSV   );

    connectBranch<std::vector<int>>      ( tree, "trk_charge",             &m_trk_charge             );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_isFinal",            &m_trk_isFinal            );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_isSelected",         &m_trk_isSelected         );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_isAssociated",       &m_trk_isAssociated       );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nPixelHits",         &m_trk_nPixelHits         );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nSCTHits",           &m_trk_nSCTHits           );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nTRTHits",           &m_trk_nTRTHits           );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nPixelBarrelLayers", &m_trk_nPixelBarrelLayers );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nPixelEndCapLayers", &m_trk_nPixelEndCapLayers );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nSCTBarrelLayers",   &m_trk_nSCTBarrelLayers   );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nSCTEndCapLayers",   &m_trk_nSCTEndCapLayers   );
    connectBranch<std::vector<uint32_t>> ( tree, "trk_hitPattern",         &m_trk_hitPattern         );
    connectBranch<std::vector<float>>    ( tree, "trk_dEdx",               &m_trk_dEdx               );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nUsedHitsdEdx",      &m_trk_nUsedHitsdEdx      );
    connectBranch<std::vector<uint8_t>>  ( tree, "trk_nIBLOverflowsdEdx",  &m_trk_nIBLOverflowsdEdx  );
    connectBranch<std::vector<float>>    ( tree, "trk_radiusOfFirstHit",   &m_trk_radiusOfFirstHit   );

    if ( m_infoSwitch.m_truth || m_infoSwitch.m_close || m_infoSwitch.m_linked ) {
      connectBranch<std::vector<float>>     ( tree, "trk_truthProb",                     &m_trk_truthProb                     );
      connectBranch<std::vector<int>>       ( tree, "trk_truthID",                       &m_trk_truthID                       );
      connectBranch<std::vector<int>>       ( tree, "trk_truthBarcode",                  &m_trk_truthBarcode                  );
      connectBranch<std::vector<int>>       ( tree, "trk_truthPdgId",                    &m_trk_truthPdgId                    );
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_close )
        connectBranch<std::vector<uint8_t>> ( tree, "trk_truthPointsToClosestTV",        &m_trk_truthPointsToClosestTV        );
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
        connectBranch<std::vector<uint8_t>> ( tree, "trk_truthPointsToMaxlinkTV",        &m_trk_truthPointsToMaxlinkTV        );
        connectBranch<std::vector<uint8_t>> ( tree, "trk_truthParentPointsToMaxlinkPTV", &m_trk_truthParentPointsToMaxlinkPTV );
      }
    }
  }

  // close matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_close ) {
    connectBranch<std::vector<float>>   ( tree, "closeTruth_distance",               &m_closeTruth_distance               );
    connectBranch<std::vector<int>>     ( tree, "closeTruth_ID",                     &m_closeTruth_ID                     );
    connectBranch<std::vector<int>>     ( tree, "closeTruth_barcode",                &m_closeTruth_barcode                );
    
    connectBranch<float>   ( tree, "closestTruth_distance",               &m_closestTruth_distance               );

    connectBranch<int>   ( tree, "closestTruth_ID",      &m_closestTruth_ID      );
    connectBranch<float> ( tree, "closestTruth_x",       &m_closestTruth_x       );
    connectBranch<float> ( tree, "closestTruth_y",       &m_closestTruth_y       );
    connectBranch<float> ( tree, "closestTruth_z",       &m_closestTruth_z       );
    connectBranch<float> ( tree, "closestTruth_r",       &m_closestTruth_r       );
    connectBranch<float> ( tree, "closestTruth_eta",     &m_closestTruth_eta     );
    connectBranch<float> ( tree, "closestTruth_phi",     &m_closestTruth_phi     );
    connectBranch<int>   ( tree, "closestTruth_barcode", &m_closestTruth_barcode );

    connectBranch<float> ( tree, "closestTruth_inE",  &m_closestTruth_inE  );
    connectBranch<float> ( tree, "closestTruth_outE", &m_closestTruth_outE );

    connectBranch<int>   ( tree, "closestTruth_parent_ID",      &m_closestTruth_parent_ID      );
    connectBranch<float> ( tree, "closestTruth_parent_pt",      &m_closestTruth_parent_pt      );
    connectBranch<float> ( tree, "closestTruth_parent_eta",     &m_closestTruth_parent_eta     );
    connectBranch<float> ( tree, "closestTruth_parent_phi",     &m_closestTruth_parent_phi     );
    connectBranch<float> ( tree, "closestTruth_parent_E",       &m_closestTruth_parent_E       );
    connectBranch<float> ( tree, "closestTruth_parent_M",       &m_closestTruth_parent_M       );
    connectBranch<float> ( tree, "closestTruth_parent_charge",  &m_closestTruth_parent_charge  );
    connectBranch<int>   ( tree, "closestTruth_parent_pdgId",   &m_closestTruth_parent_pdgId   );
    connectBranch<int>   ( tree, "closestTruth_parent_status",  &m_closestTruth_parent_status  );
    connectBranch<int>   ( tree, "closestTruth_parent_barcode", &m_closestTruth_parent_barcode );

    connectBranch<std::vector<int>>     ( tree, "closestTruth_outP_ID",                &m_closestTruth_outP_ID                );
    connectBranch<std::vector<float>>   ( tree, "closestTruth_outP_pt",                &m_closestTruth_outP_pt                );
    connectBranch<std::vector<float>>   ( tree, "closestTruth_outP_eta",               &m_closestTruth_outP_eta               );
    connectBranch<std::vector<float>>   ( tree, "closestTruth_outP_phi",               &m_closestTruth_outP_phi               );
    connectBranch<std::vector<float>>   ( tree, "closestTruth_outP_E",                 &m_closestTruth_outP_E                 );
    connectBranch<std::vector<float>>   ( tree, "closestTruth_outP_M",                 &m_closestTruth_outP_M                 );
    connectBranch<std::vector<float>>   ( tree, "closestTruth_outP_charge",            &m_closestTruth_outP_charge            );
    connectBranch<std::vector<int>>     ( tree, "closestTruth_outP_pdgId",             &m_closestTruth_outP_pdgId             );
    connectBranch<std::vector<int>>     ( tree, "closestTruth_outP_status",            &m_closestTruth_outP_status            );
    connectBranch<std::vector<int>>     ( tree, "closestTruth_outP_barcode",           &m_closestTruth_outP_barcode           );
    connectBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isReco",            &m_closestTruth_outP_isReco            );
    connectBranch<std::vector<float>>   ( tree, "closestTruth_outP_recoProb",          &m_closestTruth_outP_recoProb          );
    connectBranch<std::vector<int>>     ( tree, "closestTruth_outP_recoID",            &m_closestTruth_outP_recoID            );
    connectBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_recoIsSelected",    &m_closestTruth_outP_recoIsSelected    );
    connectBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_recoIsAssociated",  &m_closestTruth_outP_recoIsAssociated  );
    connectBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isStable",          &m_closestTruth_outP_isStable          );
    connectBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isInteracting",     &m_closestTruth_outP_isInteracting     );
    connectBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isReconstructible", &m_closestTruth_outP_isReconstructible );
  }
  // track-truth-link matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
    connectBranch<std::vector<float>>   ( tree, "linkTruth_score",                  &m_linkTruth_score                  );
    connectBranch<std::vector<int>>     ( tree, "linkTruth_ID",                     &m_linkTruth_ID                     );
    connectBranch<std::vector<int>>     ( tree, "linkTruth_barcode",                &m_linkTruth_barcode                );
    
    connectBranch<float>   ( tree, "maxlinkTruth_score",                  &m_maxlinkTruth_score                  ); 

    connectBranch<int>   ( tree, "maxlinkTruth_ID",      &m_maxlinkTruth_ID      );
    connectBranch<float> ( tree, "maxlinkTruth_x",       &m_maxlinkTruth_x       );
    connectBranch<float> ( tree, "maxlinkTruth_y",       &m_maxlinkTruth_y       );
    connectBranch<float> ( tree, "maxlinkTruth_z",       &m_maxlinkTruth_z       );
    connectBranch<float> ( tree, "maxlinkTruth_r",       &m_maxlinkTruth_r       );
    connectBranch<float> ( tree, "maxlinkTruth_eta",     &m_maxlinkTruth_eta     );
    connectBranch<float> ( tree, "maxlinkTruth_phi",     &m_maxlinkTruth_phi     );
    connectBranch<int>   ( tree, "maxlinkTruth_barcode", &m_maxlinkTruth_barcode );

    connectBranch<float> ( tree, "maxlinkTruth_inE",  &m_maxlinkTruth_inE  );
    connectBranch<float> ( tree, "maxlinkTruth_outE", &m_maxlinkTruth_outE );

    connectBranch<int>   ( tree, "maxlinkTruth_parent_ID",      &m_maxlinkTruth_parent_ID      );
    connectBranch<float> ( tree, "maxlinkTruth_parent_pt",      &m_maxlinkTruth_parent_pt      );
    connectBranch<float> ( tree, "maxlinkTruth_parent_eta",     &m_maxlinkTruth_parent_eta     );
    connectBranch<float> ( tree, "maxlinkTruth_parent_phi",     &m_maxlinkTruth_parent_phi     );
    connectBranch<float> ( tree, "maxlinkTruth_parent_E",       &m_maxlinkTruth_parent_E       );
    connectBranch<float> ( tree, "maxlinkTruth_parent_M",       &m_maxlinkTruth_parent_M       );
    connectBranch<float> ( tree, "maxlinkTruth_parent_charge",  &m_maxlinkTruth_parent_charge  );
    connectBranch<int>   ( tree, "maxlinkTruth_parent_pdgId",   &m_maxlinkTruth_parent_pdgId   );
    connectBranch<int>   ( tree, "maxlinkTruth_parent_status",  &m_maxlinkTruth_parent_status  );
    connectBranch<int>   ( tree, "maxlinkTruth_parent_barcode", &m_maxlinkTruth_parent_barcode );

    connectBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_ID",                &m_maxlinkTruth_outP_ID                );
    connectBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_pt",                &m_maxlinkTruth_outP_pt                );
    connectBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_eta",               &m_maxlinkTruth_outP_eta               );
    connectBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_phi",               &m_maxlinkTruth_outP_phi               );
    connectBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_E",                 &m_maxlinkTruth_outP_E                 );
    connectBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_M",                 &m_maxlinkTruth_outP_M                 );
    connectBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_charge",            &m_maxlinkTruth_outP_charge            );
    connectBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_pdgId",             &m_maxlinkTruth_outP_pdgId             );
    connectBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_status",            &m_maxlinkTruth_outP_status            );
    connectBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_barcode",           &m_maxlinkTruth_outP_barcode           );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isReco",            &m_maxlinkTruth_outP_isReco            );
    connectBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_recoProb",          &m_maxlinkTruth_outP_recoProb          );
    connectBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_recoID",            &m_maxlinkTruth_outP_recoID            );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_recoIsSelected",    &m_maxlinkTruth_outP_recoIsSelected    );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_recoIsAssociated",  &m_maxlinkTruth_outP_recoIsAssociated  );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isStable",          &m_maxlinkTruth_outP_isStable          );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isInteracting",     &m_maxlinkTruth_outP_isInteracting     );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isReconstructible", &m_maxlinkTruth_outP_isReconstructible );
    

    connectBranch<std::vector<float>>   ( tree, "linkParentTruth_score",                  &m_linkParentTruth_score                  );
    connectBranch<std::vector<int>>     ( tree, "linkParentTruth_ID",                     &m_linkParentTruth_ID                     );
    connectBranch<std::vector<int>>     ( tree, "linkParentTruth_barcode",                &m_linkParentTruth_barcode                );
    
    connectBranch<float>   ( tree, "maxlinkParentTruth_score",                  &m_maxlinkParentTruth_score                  );   

    connectBranch<int>   ( tree, "maxlinkParentTruth_ID",      &m_maxlinkParentTruth_ID      );
    connectBranch<float> ( tree, "maxlinkParentTruth_x",       &m_maxlinkParentTruth_x       );
    connectBranch<float> ( tree, "maxlinkParentTruth_y",       &m_maxlinkParentTruth_y       );
    connectBranch<float> ( tree, "maxlinkParentTruth_z",       &m_maxlinkParentTruth_z       );
    connectBranch<float> ( tree, "maxlinkParentTruth_r",       &m_maxlinkParentTruth_r       );
    connectBranch<float> ( tree, "maxlinkParentTruth_eta",     &m_maxlinkParentTruth_eta     );
    connectBranch<float> ( tree, "maxlinkParentTruth_phi",     &m_maxlinkParentTruth_phi     );
    connectBranch<int>   ( tree, "maxlinkParentTruth_barcode", &m_maxlinkParentTruth_barcode );

    connectBranch<float> ( tree, "maxlinkParentTruth_inE",  &m_maxlinkParentTruth_inE  );
    connectBranch<float> ( tree, "maxlinkParentTruth_outE", &m_maxlinkParentTruth_outE );

    connectBranch<int>   ( tree, "maxlinkParentTruth_parent_ID",      &m_maxlinkParentTruth_parent_ID      );
    connectBranch<float> ( tree, "maxlinkParentTruth_parent_pt",      &m_maxlinkParentTruth_parent_pt      );
    connectBranch<float> ( tree, "maxlinkParentTruth_parent_eta",     &m_maxlinkParentTruth_parent_eta     );
    connectBranch<float> ( tree, "maxlinkParentTruth_parent_phi",     &m_maxlinkParentTruth_parent_phi     );
    connectBranch<float> ( tree, "maxlinkParentTruth_parent_E",       &m_maxlinkParentTruth_parent_E       );
    connectBranch<float> ( tree, "maxlinkParentTruth_parent_M",       &m_maxlinkParentTruth_parent_M       );
    connectBranch<float> ( tree, "maxlinkParentTruth_parent_charge",  &m_maxlinkParentTruth_parent_charge  );
    connectBranch<int>   ( tree, "maxlinkParentTruth_parent_pdgId",   &m_maxlinkParentTruth_parent_pdgId   );
    connectBranch<int>   ( tree, "maxlinkParentTruth_parent_status",  &m_maxlinkParentTruth_parent_status  );
    connectBranch<int>   ( tree, "maxlinkParentTruth_parent_barcode", &m_maxlinkParentTruth_parent_barcode );

    connectBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_ID",                &m_maxlinkParentTruth_outP_ID                );
    connectBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_pt",                &m_maxlinkParentTruth_outP_pt                );
    connectBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_eta",               &m_maxlinkParentTruth_outP_eta               );
    connectBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_phi",               &m_maxlinkParentTruth_outP_phi               );
    connectBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_E",                 &m_maxlinkParentTruth_outP_E                 );
    connectBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_M",                 &m_maxlinkParentTruth_outP_M                 );
    connectBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_charge",            &m_maxlinkParentTruth_outP_charge            );
    connectBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_pdgId",             &m_maxlinkParentTruth_outP_pdgId             );
    connectBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_status",            &m_maxlinkParentTruth_outP_status            );
    connectBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_barcode",           &m_maxlinkParentTruth_outP_barcode           );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isReco",            &m_maxlinkParentTruth_outP_isReco            );
    connectBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_recoProb",          &m_maxlinkParentTruth_outP_recoProb          );
    connectBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_recoID",            &m_maxlinkParentTruth_outP_recoID            );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_recoIsSelected",    &m_maxlinkParentTruth_outP_recoIsSelected    );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_recoIsAssociated",  &m_maxlinkParentTruth_outP_recoIsAssociated  );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isStable",          &m_maxlinkParentTruth_outP_isStable          );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isInteracting",     &m_maxlinkParentTruth_outP_isInteracting     );
    connectBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isReconstructible", &m_maxlinkParentTruth_outP_isReconstructible );
  }

  // matched to jets
  if ( m_infoSwitch.m_jetMatched ) {
    if ( m_mc ) {
      connectBranch<uint8_t>            ( tree, "isTruthJetMatched",  &m_truthJetMatch     );
      connectBranch<std::vector<int>>   ( tree, "truthJetMatch_ID",   &m_truthJetMatch_ID  );
      connectBranch<std::vector<float>> ( tree, "truthJetMatch_dR",   &m_truthJetMatch_dR  );
      connectBranch<std::vector<int>>   ( tree, "darkJetMatch_ID",    &m_darkJetMatch_ID   );
      connectBranch<std::vector<float>> ( tree, "darkJetMatch_dR",    &m_darkJetMatch_dR   );
    }
    if ( m_doEMTopo ) {
      connectBranch<uint8_t>            ( tree, "isEMTopoJetMatched", &m_emtopoJetMatch    );
      connectBranch<std::vector<int>>   ( tree, "EMTopoJetMatch_ID",  &m_emtopoJetMatch_ID );
      connectBranch<std::vector<float>> ( tree, "EMTopoJetMatch_dR",  &m_emtopoJetMatch_dR );
    }
    if ( m_doPFlow ) {
      connectBranch<uint8_t>            ( tree, "isPFlowJetMatched",  &m_pflowJetMatch     );
      connectBranch<std::vector<int>>   ( tree, "PFlowJetMatch_ID",   &m_pflowJetMatch_ID  );
      connectBranch<std::vector<float>> ( tree, "PFlowJetMatch_dR",   &m_pflowJetMatch_dR  );
    }
  }
}


void SecondaryVertexContainer :: setBranches ( TTree* tree )
{
  if ( m_debug ) std::cout << "SecondaryVertex : setBranches()" << std::endl;
  
  VertexContainer::setBranches ( tree );

  setBranch<int>                ( tree, "ID",            m_ID            );
  setBranch<float>              ( tree, "x",             m_x             );
  setBranch<float>              ( tree, "y",             m_y             );
  setBranch<float>              ( tree, "z",             m_z             );
  setBranch<float>              ( tree, "r",             m_r             );
  setBranch<float>              ( tree, "pt",            m_pt            );
  setBranch<float>              ( tree, "eta",           m_eta           );
  setBranch<float>              ( tree, "phi",           m_phi           );
  setBranch<float>              ( tree, "mass",          m_mass          );
  setBranch<float>              ( tree, "mass_nonAssoc", m_mass_nonAssoc );
  setBranch<std::vector<float>> ( tree, "covariance",    m_covariance    );
  setBranch<float>              ( tree, "chi2",          m_chi2          );
  setBranch<float>              ( tree, "direction",     m_direction     );
  setBranch<int>                ( tree, "charge",        m_charge        );
  setBranch<float>              ( tree, "H",             m_H             );
  setBranch<float>              ( tree, "Ht",            m_Ht            );
  setBranch<float>              ( tree, "minOpAng",      m_minOpAng      );
  setBranch<float>              ( tree, "maxOpAng",      m_maxOpAng      );
  setBranch<float>              ( tree, "mind0",         m_mind0         );
  setBranch<float>              ( tree, "maxd0",         m_maxd0         );

  setBranch<float>              ( tree, "minOneTrackRemovedMass", m_minOneTrackRemovedMass );
  setBranch<std::vector<float>> ( tree, "twoTracksMass",          m_twoTracksMass          );
  setBranch<std::vector<float>> ( tree, "twoTracksMassRest",      m_twoTracksMassRest      );
  setBranch<std::vector<int>>   ( tree, "twoTracksCharge",        m_twoTracksCharge        );

  setBranch<uint32_t> ( tree, "ntrk",       m_ntrk       );
  setBranch<uint32_t> ( tree, "ntrk_sel",   m_ntrk_sel   );
  setBranch<uint32_t> ( tree, "ntrk_assoc", m_ntrk_assoc );

  setBranch<uint32_t> ( tree, "pass_sel", m_pass_sel );

  // tracks
  if ( m_infoSwitch.m_tracks ) {
    setBranch<std::vector<int>>   ( tree, "trk_ID",     m_trk_ID     );
    setBranch<std::vector<float>> ( tree, "trk_qOverP", m_trk_qOverP );
    setBranch<std::vector<float>> ( tree, "trk_theta",  m_trk_theta  );
    setBranch<std::vector<float>> ( tree, "trk_vz",     m_trk_vz     );
    setBranch<std::vector<float>> ( tree, "trk_E",      m_trk_E      );
    setBranch<std::vector<float>> ( tree, "trk_M",      m_trk_M      );
    setBranch<std::vector<float>> ( tree, "trk_pt",     m_trk_pt     );
    setBranch<std::vector<float>> ( tree, "trk_eta",    m_trk_eta    );
    setBranch<std::vector<float>> ( tree, "trk_phi",    m_trk_phi    );
    setBranch<std::vector<float>> ( tree, "trk_d0",     m_trk_d0     );
    setBranch<std::vector<float>> ( tree, "trk_z0",     m_trk_z0     );
    setBranch<std::vector<float>> ( tree, "trk_errd0",  m_trk_errd0  );
    setBranch<std::vector<float>> ( tree, "trk_errz0",  m_trk_errz0  );
    setBranch<std::vector<float>> ( tree, "trk_chi2",   m_trk_chi2   );

    setBranch<std::vector<float>> ( tree, "trk_pt_wrtSV",    m_trk_pt_wrtSV    );
    setBranch<std::vector<float>> ( tree, "trk_eta_wrtSV",   m_trk_eta_wrtSV   );
    setBranch<std::vector<float>> ( tree, "trk_phi_wrtSV",   m_trk_phi_wrtSV   );
    setBranch<std::vector<float>> ( tree, "trk_d0_wrtSV",    m_trk_d0_wrtSV    );
    setBranch<std::vector<float>> ( tree, "trk_z0_wrtSV",    m_trk_z0_wrtSV    );
    setBranch<std::vector<float>> ( tree, "trk_errd0_wrtSV", m_trk_errd0_wrtSV );
    setBranch<std::vector<float>> ( tree, "trk_errz0_wrtSV", m_trk_errz0_wrtSV );
    setBranch<std::vector<float>> ( tree, "trk_chi2_toSV",   m_trk_chi2_toSV   );

    setBranch<std::vector<int>>      ( tree, "trk_charge",             m_trk_charge             );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_isFinal",            m_trk_isFinal            );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_isSelected",         m_trk_isSelected         );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_isAssociated",       m_trk_isAssociated       );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_nPixelBarrelLayers", m_trk_nPixelBarrelLayers );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_nPixelEndCapLayers", m_trk_nPixelEndCapLayers );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_nSCTBarrelLayers",   m_trk_nSCTBarrelLayers   );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_nSCTEndCapLayers",   m_trk_nSCTEndCapLayers   );
    setBranch<std::vector<uint32_t>> ( tree, "trk_hitPattern",         m_trk_hitPattern         );
    setBranch<std::vector<float>>    ( tree, "trk_dEdx",               m_trk_dEdx               );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_nUsedHitsdEdx",      m_trk_nUsedHitsdEdx      );
    setBranch<std::vector<uint8_t>>  ( tree, "trk_nIBLOverflowsdEdx",  m_trk_nIBLOverflowsdEdx  );
    setBranch<std::vector<float>>    ( tree, "trk_radiusOfFirstHit",   m_trk_radiusOfFirstHit   );

    if ( m_infoSwitch.m_truth || m_infoSwitch.m_close || m_infoSwitch.m_linked ) {
      setBranch<std::vector<float>>     ( tree, "trk_truthProb",                     m_trk_truthProb                     );
      setBranch<std::vector<int>>       ( tree, "trk_truthID",                       m_trk_truthID                       );
      setBranch<std::vector<int>>       ( tree, "trk_truthBarcode",                  m_trk_truthBarcode                  );
      setBranch<std::vector<int>>       ( tree, "trk_truthPdgId",                    m_trk_truthPdgId                    );
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_close )
        setBranch<std::vector<uint8_t>> ( tree, "trk_truthPointsToClosestTV",        m_trk_truthPointsToClosestTV        );
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
        setBranch<std::vector<uint8_t>> ( tree, "trk_truthPointsToMaxlinkTV",        m_trk_truthPointsToMaxlinkTV        );
        setBranch<std::vector<uint8_t>> ( tree, "trk_truthParentPointsToMaxlinkPTV", m_trk_truthParentPointsToMaxlinkPTV );
      }
    }
  }

  // close matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_close ) {
    setBranch<std::vector<float>>   ( tree, "closeTruth_distance",               m_closeTruth_distance               );
    setBranch<std::vector<int>>     ( tree, "closeTruth_ID",                     m_closeTruth_ID                     );
    setBranch<std::vector<int>>     ( tree, "closeTruth_barcode",                m_closeTruth_barcode                );
    
    setBranch<float>   ( tree, "closestTruth_distance",               m_closestTruth_distance               ); 

    setBranch<int>   ( tree, "closestTruth_ID",      m_closestTruth_ID      );
    setBranch<float> ( tree, "closestTruth_x",       m_closestTruth_x       );
    setBranch<float> ( tree, "closestTruth_y",       m_closestTruth_y       );
    setBranch<float> ( tree, "closestTruth_z",       m_closestTruth_z       );
    setBranch<float> ( tree, "closestTruth_r",       m_closestTruth_r       );
    setBranch<float> ( tree, "closestTruth_eta",     m_closestTruth_eta     );
    setBranch<float> ( tree, "closestTruth_phi",     m_closestTruth_phi     );
    setBranch<int>   ( tree, "closestTruth_barcode", m_closestTruth_barcode );

    setBranch<float> ( tree, "closestTruth_inE",  m_closestTruth_inE  );
    setBranch<float> ( tree, "closestTruth_outE", m_closestTruth_outE );

    setBranch<int>   ( tree, "closestTruth_parent_ID",      m_closestTruth_parent_ID      );
    setBranch<float> ( tree, "closestTruth_parent_pt",      m_closestTruth_parent_pt      );
    setBranch<float> ( tree, "closestTruth_parent_eta",     m_closestTruth_parent_eta     );
    setBranch<float> ( tree, "closestTruth_parent_phi",     m_closestTruth_parent_phi     );
    setBranch<float> ( tree, "closestTruth_parent_E",       m_closestTruth_parent_E       );
    setBranch<float> ( tree, "closestTruth_parent_M",       m_closestTruth_parent_M       );
    setBranch<float> ( tree, "closestTruth_parent_charge",  m_closestTruth_parent_charge  );
    setBranch<int>   ( tree, "closestTruth_parent_pdgId",   m_closestTruth_parent_pdgId   );
    setBranch<int>   ( tree, "closestTruth_parent_status",  m_closestTruth_parent_status  );
    setBranch<int>   ( tree, "closestTruth_parent_barcode", m_closestTruth_parent_barcode );

    setBranch<std::vector<int>>     ( tree, "closestTruth_outP_ID",                m_closestTruth_outP_ID                );
    setBranch<std::vector<float>>   ( tree, "closestTruth_outP_pt",                m_closestTruth_outP_pt                );
    setBranch<std::vector<float>>   ( tree, "closestTruth_outP_eta",               m_closestTruth_outP_eta               );
    setBranch<std::vector<float>>   ( tree, "closestTruth_outP_phi",               m_closestTruth_outP_phi               );
    setBranch<std::vector<float>>   ( tree, "closestTruth_outP_E",                 m_closestTruth_outP_E                 );
    setBranch<std::vector<float>>   ( tree, "closestTruth_outP_M",                 m_closestTruth_outP_M                 );
    setBranch<std::vector<float>>   ( tree, "closestTruth_outP_charge",            m_closestTruth_outP_charge            );
    setBranch<std::vector<int>>     ( tree, "closestTruth_outP_pdgId",             m_closestTruth_outP_pdgId             );
    setBranch<std::vector<int>>     ( tree, "closestTruth_outP_status",            m_closestTruth_outP_status            );
    setBranch<std::vector<int>>     ( tree, "closestTruth_outP_barcode",           m_closestTruth_outP_barcode           );
    setBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isReco",            m_closestTruth_outP_isReco            );
    setBranch<std::vector<float>>   ( tree, "closestTruth_outP_recoProb",          m_closestTruth_outP_recoProb          );
    setBranch<std::vector<int>>     ( tree, "closestTruth_outP_recoID",            m_closestTruth_outP_recoID            );
    setBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_recoIsSelected",    m_closestTruth_outP_recoIsSelected    );
    setBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_recoIsAssociated",  m_closestTruth_outP_recoIsAssociated  );
    setBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isStable",          m_closestTruth_outP_isStable          );
    setBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isInteracting",     m_closestTruth_outP_isInteracting     );
    setBranch<std::vector<uint8_t>> ( tree, "closestTruth_outP_isReconstructible", m_closestTruth_outP_isReconstructible );
  }
  // track-truth-link matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
    setBranch<std::vector<float>>   ( tree, "linkTruth_score",                  m_linkTruth_score                  );
    setBranch<std::vector<int>>     ( tree, "linkTruth_ID",                     m_linkTruth_ID                     );
    setBranch<std::vector<int>>     ( tree, "linkTruth_barcode",                m_linkTruth_barcode                );
    
    setBranch<float>   ( tree, "maxlinkTruth_score",                  m_maxlinkTruth_score                  );

    setBranch<int>   ( tree, "maxlinkTruth_ID",      m_maxlinkTruth_ID      );
    setBranch<float> ( tree, "maxlinkTruth_x",       m_maxlinkTruth_x       );
    setBranch<float> ( tree, "maxlinkTruth_y",       m_maxlinkTruth_y       );
    setBranch<float> ( tree, "maxlinkTruth_z",       m_maxlinkTruth_z       );
    setBranch<float> ( tree, "maxlinkTruth_r",       m_maxlinkTruth_r       );
    setBranch<float> ( tree, "maxlinkTruth_eta",     m_maxlinkTruth_eta     );
    setBranch<float> ( tree, "maxlinkTruth_phi",     m_maxlinkTruth_phi     );
    setBranch<int>   ( tree, "maxlinkTruth_barcode", m_maxlinkTruth_barcode );

    setBranch<float> ( tree, "maxlinkTruth_inE",  m_maxlinkTruth_inE  );
    setBranch<float> ( tree, "maxlinkTruth_outE", m_maxlinkTruth_outE );

    setBranch<int>   ( tree, "maxlinkTruth_parent_ID",      m_maxlinkTruth_parent_ID      );
    setBranch<float> ( tree, "maxlinkTruth_parent_pt",      m_maxlinkTruth_parent_pt      );
    setBranch<float> ( tree, "maxlinkTruth_parent_eta",     m_maxlinkTruth_parent_eta     );
    setBranch<float> ( tree, "maxlinkTruth_parent_phi",     m_maxlinkTruth_parent_phi     );
    setBranch<float> ( tree, "maxlinkTruth_parent_E",       m_maxlinkTruth_parent_E       );
    setBranch<float> ( tree, "maxlinkTruth_parent_M",       m_maxlinkTruth_parent_M       );
    setBranch<float> ( tree, "maxlinkTruth_parent_charge",  m_maxlinkTruth_parent_charge  );
    setBranch<int>   ( tree, "maxlinkTruth_parent_pdgId",   m_maxlinkTruth_parent_pdgId   );
    setBranch<int>   ( tree, "maxlinkTruth_parent_status",  m_maxlinkTruth_parent_status  );
    setBranch<int>   ( tree, "maxlinkTruth_parent_barcode", m_maxlinkTruth_parent_barcode );

    setBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_ID",                m_maxlinkTruth_outP_ID                );
    setBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_pt",                m_maxlinkTruth_outP_pt                );
    setBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_eta",               m_maxlinkTruth_outP_eta               );
    setBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_phi",               m_maxlinkTruth_outP_phi               );
    setBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_E",                 m_maxlinkTruth_outP_E                 );
    setBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_M",                 m_maxlinkTruth_outP_M                 );
    setBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_charge",            m_maxlinkTruth_outP_charge            );
    setBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_pdgId",             m_maxlinkTruth_outP_pdgId             );
    setBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_status",            m_maxlinkTruth_outP_status            );
    setBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_barcode",           m_maxlinkTruth_outP_barcode           );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isReco",            m_maxlinkTruth_outP_isReco            );
    setBranch<std::vector<float>>   ( tree, "maxlinkTruth_outP_recoProb",          m_maxlinkTruth_outP_recoProb          );
    setBranch<std::vector<int>>     ( tree, "maxlinkTruth_outP_recoID",            m_maxlinkTruth_outP_recoID            );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_recoIsSelected",    m_maxlinkTruth_outP_recoIsSelected    );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_recoIsAssociated",  m_maxlinkTruth_outP_recoIsAssociated  );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isStable",          m_maxlinkTruth_outP_isStable          );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isInteracting",     m_maxlinkTruth_outP_isInteracting     );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkTruth_outP_isReconstructible", m_maxlinkTruth_outP_isReconstructible );


    setBranch<std::vector<float>>   ( tree, "linkParentTruth_score",                  m_linkParentTruth_score                  );
    setBranch<std::vector<int>>     ( tree, "linkParentTruth_ID",                     m_linkParentTruth_ID                     );
    setBranch<std::vector<int>>     ( tree, "linkParentTruth_barcode",                m_linkParentTruth_barcode                );
    
    setBranch<float>   ( tree, "maxlinkParentTruth_score",                  m_maxlinkParentTruth_score                  );

    setBranch<int>   ( tree, "maxlinkParentTruth_ID",      m_maxlinkParentTruth_ID      );
    setBranch<float> ( tree, "maxlinkParentTruth_x",       m_maxlinkParentTruth_x       );
    setBranch<float> ( tree, "maxlinkParentTruth_y",       m_maxlinkParentTruth_y       );
    setBranch<float> ( tree, "maxlinkParentTruth_z",       m_maxlinkParentTruth_z       );
    setBranch<float> ( tree, "maxlinkParentTruth_r",       m_maxlinkParentTruth_r       );
    setBranch<float> ( tree, "maxlinkParentTruth_eta",     m_maxlinkParentTruth_eta     );
    setBranch<float> ( tree, "maxlinkParentTruth_phi",     m_maxlinkParentTruth_phi     );
    setBranch<int>   ( tree, "maxlinkParentTruth_barcode", m_maxlinkParentTruth_barcode );

    setBranch<float> ( tree, "maxlinkParentTruth_inE",  m_maxlinkParentTruth_inE  );
    setBranch<float> ( tree, "maxlinkParentTruth_outE", m_maxlinkParentTruth_outE );

    setBranch<int>   ( tree, "maxlinkParentTruth_parent_ID",      m_maxlinkParentTruth_parent_ID      );
    setBranch<float> ( tree, "maxlinkParentTruth_parent_pt",      m_maxlinkParentTruth_parent_pt      );
    setBranch<float> ( tree, "maxlinkParentTruth_parent_eta",     m_maxlinkParentTruth_parent_eta     );
    setBranch<float> ( tree, "maxlinkParentTruth_parent_phi",     m_maxlinkParentTruth_parent_phi     );
    setBranch<float> ( tree, "maxlinkParentTruth_parent_E",       m_maxlinkParentTruth_parent_E       );
    setBranch<float> ( tree, "maxlinkParentTruth_parent_M",       m_maxlinkParentTruth_parent_M       );
    setBranch<float> ( tree, "maxlinkParentTruth_parent_charge",  m_maxlinkParentTruth_parent_charge  );
    setBranch<int>   ( tree, "maxlinkParentTruth_parent_pdgId",   m_maxlinkParentTruth_parent_pdgId   );
    setBranch<int>   ( tree, "maxlinkParentTruth_parent_status",  m_maxlinkParentTruth_parent_status  );
    setBranch<int>   ( tree, "maxlinkParentTruth_parent_barcode", m_maxlinkParentTruth_parent_barcode );

    setBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_ID",                m_maxlinkParentTruth_outP_ID                );
    setBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_pt",                m_maxlinkParentTruth_outP_pt                );
    setBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_eta",               m_maxlinkParentTruth_outP_eta               );
    setBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_phi",               m_maxlinkParentTruth_outP_phi               );
    setBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_E",                 m_maxlinkParentTruth_outP_E                 );
    setBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_M",                 m_maxlinkParentTruth_outP_M                 );
    setBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_charge",            m_maxlinkParentTruth_outP_charge            );
    setBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_pdgId",             m_maxlinkParentTruth_outP_pdgId             );
    setBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_status",            m_maxlinkParentTruth_outP_status            );
    setBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_barcode",           m_maxlinkParentTruth_outP_barcode           );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isReco",            m_maxlinkParentTruth_outP_isReco            );
    setBranch<std::vector<float>>   ( tree, "maxlinkParentTruth_outP_recoProb",          m_maxlinkParentTruth_outP_recoProb          );
    setBranch<std::vector<int>>     ( tree, "maxlinkParentTruth_outP_recoID",            m_maxlinkParentTruth_outP_recoID            );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_recoIsSelected",    m_maxlinkParentTruth_outP_recoIsSelected    );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_recoIsAssociated",  m_maxlinkParentTruth_outP_recoIsAssociated  );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isStable",          m_maxlinkParentTruth_outP_isStable          );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isInteracting",     m_maxlinkParentTruth_outP_isInteracting     );
    setBranch<std::vector<uint8_t>> ( tree, "maxlinkParentTruth_outP_isReconstructible", m_maxlinkParentTruth_outP_isReconstructible );
  }

  // matched to jets
  if ( m_infoSwitch.m_jetMatched ) {
    if ( m_mc ) {
      setBranch<uint8_t>            ( tree, "isTruthJetMatched",  m_truthJetMatch     );
      setBranch<std::vector<int>>   ( tree, "truthJetMatch_ID",   m_truthJetMatch_ID  );
      setBranch<std::vector<float>> ( tree, "truthJetMatch_dR",   m_truthJetMatch_dR  );
      setBranch<std::vector<int>>   ( tree, "darkJetMatch_ID",    m_darkJetMatch_ID   );
      setBranch<std::vector<float>> ( tree, "darkJetMatch_dR",    m_darkJetMatch_dR   );
    }
    if ( m_doEMTopo ) {
      setBranch<uint8_t>            ( tree, "isEMTopoJetMatched", m_emtopoJetMatch    );
      setBranch<std::vector<int>>   ( tree, "EMTopoJetMatch_ID",  m_emtopoJetMatch_ID );
      setBranch<std::vector<float>> ( tree, "EMTopoJetMatch_dR",  m_emtopoJetMatch_dR );
    }
    if ( m_doPFlow ) {
      setBranch<uint8_t>            ( tree, "isPFlowJetMatched",  m_pflowJetMatch     );
      setBranch<std::vector<int>>   ( tree, "PFlowJetMatch_ID",   m_pflowJetMatch_ID  );
      setBranch<std::vector<float>> ( tree, "PFlowJetMatch_dR",   m_pflowJetMatch_dR  );
    }
  }
}


void SecondaryVertexContainer :: clear ()
{
  if ( m_debug ) std::cout << "SecondaryVertex : clear()" << std::endl;
  
  VertexContainer::clear ();

  m_ID            ->clear();
  m_x             ->clear();
  m_y             ->clear();
  m_z             ->clear();
  m_r             ->clear();
  m_pt            ->clear();
  m_eta           ->clear();
  m_phi           ->clear();
  m_mass          ->clear();
  m_mass_nonAssoc ->clear();
  m_covariance    ->clear();
  m_chi2          ->clear();
  m_direction     ->clear();
  m_charge        ->clear();
  m_H             ->clear();
  m_Ht            ->clear();
  m_minOpAng      ->clear();
  m_maxOpAng      ->clear();
  m_mind0         ->clear();
  m_maxd0         ->clear();
  
  m_minOneTrackRemovedMass ->clear();
  m_twoTracksMass          ->clear();
  m_twoTracksMassRest      ->clear();
  m_twoTracksCharge        ->clear();

  m_ntrk       ->clear();
  m_ntrk_sel   ->clear();
  m_ntrk_assoc ->clear();

  m_pass_sel ->clear();

  // tracks
  if ( m_infoSwitch.m_tracks ) {
    m_trk_ID     ->clear();
    m_trk_qOverP ->clear();
    m_trk_theta  ->clear();
    m_trk_vz     ->clear();
    m_trk_E      ->clear();
    m_trk_M      ->clear();
    m_trk_pt     ->clear();
    m_trk_eta    ->clear();
    m_trk_phi    ->clear();
    m_trk_d0     ->clear();
    m_trk_z0     ->clear();
    m_trk_errd0  ->clear();
    m_trk_errz0  ->clear();
    m_trk_chi2   ->clear();

    m_trk_pt_wrtSV    ->clear();
    m_trk_eta_wrtSV   ->clear();
    m_trk_phi_wrtSV   ->clear();
    m_trk_d0_wrtSV    ->clear();
    m_trk_z0_wrtSV    ->clear();
    m_trk_errd0_wrtSV ->clear();
    m_trk_errz0_wrtSV ->clear();
    m_trk_chi2_toSV   ->clear();

    m_trk_charge             ->clear();
    m_trk_isFinal            ->clear();
    m_trk_isSelected         ->clear();
    m_trk_isAssociated       ->clear();
    m_trk_nPixelHits         ->clear();
    m_trk_nSCTHits           ->clear();
    m_trk_nTRTHits           ->clear();
    m_trk_nPixelBarrelLayers ->clear();
    m_trk_nPixelEndCapLayers ->clear();
    m_trk_nSCTBarrelLayers   ->clear();
    m_trk_nSCTEndCapLayers   ->clear();
    m_trk_hitPattern         ->clear();
    m_trk_dEdx               ->clear();
    m_trk_nUsedHitsdEdx      ->clear();
    m_trk_nIBLOverflowsdEdx  ->clear();
    m_trk_radiusOfFirstHit   ->clear();

    if ( m_infoSwitch.m_truth || m_infoSwitch.m_close || m_infoSwitch.m_linked ) {
      m_trk_truthProb                       ->clear();
      m_trk_truthID                         ->clear();
      m_trk_truthBarcode                    ->clear();
      m_trk_truthPdgId                      ->clear();
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_close )
        m_trk_truthPointsToClosestTV        ->clear();
      if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
        m_trk_truthPointsToMaxlinkTV        ->clear();
        m_trk_truthParentPointsToMaxlinkPTV ->clear();
      }
    }
  }

  // close matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_close ) {
    m_closeTruth_distance               ->clear();  
    m_closeTruth_ID                     ->clear();  
    m_closeTruth_barcode                ->clear();
    
    m_closestTruth_distance               ->clear();  

    m_closestTruth_ID      ->clear();
    m_closestTruth_x       ->clear();
    m_closestTruth_y       ->clear();
    m_closestTruth_z       ->clear();
    m_closestTruth_r       ->clear();
    m_closestTruth_eta     ->clear();
    m_closestTruth_phi     ->clear();
    m_closestTruth_barcode ->clear();

    m_closestTruth_inE  ->clear();
    m_closestTruth_outE ->clear();

    m_closestTruth_parent_ID      ->clear();
    m_closestTruth_parent_pt      ->clear();
    m_closestTruth_parent_eta     ->clear();
    m_closestTruth_parent_phi     ->clear();
    m_closestTruth_parent_E       ->clear();
    m_closestTruth_parent_M       ->clear();
    m_closestTruth_parent_charge  ->clear();
    m_closestTruth_parent_pdgId   ->clear();
    m_closestTruth_parent_status  ->clear();
    m_closestTruth_parent_barcode ->clear();

    m_closestTruth_outP_ID                ->clear();
    m_closestTruth_outP_pt                ->clear();
    m_closestTruth_outP_eta               ->clear();
    m_closestTruth_outP_phi               ->clear();
    m_closestTruth_outP_E                 ->clear();
    m_closestTruth_outP_M                 ->clear();
    m_closestTruth_outP_charge            ->clear();
    m_closestTruth_outP_pdgId             ->clear();
    m_closestTruth_outP_status            ->clear();
    m_closestTruth_outP_barcode           ->clear();
    m_closestTruth_outP_isReco            ->clear();
    m_closestTruth_outP_recoProb          ->clear();
    m_closestTruth_outP_recoID            ->clear();
    m_closestTruth_outP_recoIsSelected    ->clear();
    m_closestTruth_outP_recoIsAssociated  ->clear();
    m_closestTruth_outP_isStable          ->clear();
    m_closestTruth_outP_isInteracting     ->clear();
    m_closestTruth_outP_isReconstructible ->clear();
  }
  // track-truth-link matched truth vertices
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) {
    m_linkTruth_score                  ->clear();  
    m_linkTruth_ID                     ->clear();  
    m_linkTruth_barcode                ->clear();
    
    m_maxlinkTruth_score                  ->clear();  

    m_maxlinkTruth_ID      ->clear();
    m_maxlinkTruth_x       ->clear();
    m_maxlinkTruth_y       ->clear();
    m_maxlinkTruth_z       ->clear();
    m_maxlinkTruth_r       ->clear();
    m_maxlinkTruth_eta     ->clear();
    m_maxlinkTruth_phi     ->clear();
    m_maxlinkTruth_barcode ->clear();

    m_maxlinkTruth_inE  ->clear();
    m_maxlinkTruth_outE ->clear();

    m_maxlinkTruth_parent_ID      ->clear();
    m_maxlinkTruth_parent_pt      ->clear();
    m_maxlinkTruth_parent_eta     ->clear();
    m_maxlinkTruth_parent_phi     ->clear();
    m_maxlinkTruth_parent_E       ->clear();
    m_maxlinkTruth_parent_M       ->clear();
    m_maxlinkTruth_parent_charge  ->clear();
    m_maxlinkTruth_parent_pdgId   ->clear();
    m_maxlinkTruth_parent_status  ->clear();
    m_maxlinkTruth_parent_barcode ->clear();

    m_maxlinkTruth_outP_ID                ->clear();
    m_maxlinkTruth_outP_pt                ->clear();
    m_maxlinkTruth_outP_eta               ->clear();
    m_maxlinkTruth_outP_phi               ->clear();
    m_maxlinkTruth_outP_E                 ->clear();
    m_maxlinkTruth_outP_M                 ->clear();
    m_maxlinkTruth_outP_charge            ->clear();
    m_maxlinkTruth_outP_pdgId             ->clear();
    m_maxlinkTruth_outP_status            ->clear();
    m_maxlinkTruth_outP_barcode           ->clear();
    m_maxlinkTruth_outP_isReco            ->clear();
    m_maxlinkTruth_outP_recoProb          ->clear();
    m_maxlinkTruth_outP_recoID            ->clear();
    m_maxlinkTruth_outP_recoIsSelected    ->clear();
    m_maxlinkTruth_outP_recoIsAssociated  ->clear();
    m_maxlinkTruth_outP_isStable          ->clear();
    m_maxlinkTruth_outP_isInteracting     ->clear();
    m_maxlinkTruth_outP_isReconstructible ->clear();


    m_linkParentTruth_score                  ->clear();  
    m_linkParentTruth_ID                     ->clear();  
    m_linkParentTruth_barcode                ->clear();
    
    m_maxlinkParentTruth_score                  ->clear();  

    m_maxlinkParentTruth_ID      ->clear();
    m_maxlinkParentTruth_x       ->clear();
    m_maxlinkParentTruth_y       ->clear();
    m_maxlinkParentTruth_z       ->clear();
    m_maxlinkParentTruth_r       ->clear();
    m_maxlinkParentTruth_eta     ->clear();
    m_maxlinkParentTruth_phi     ->clear();
    m_maxlinkParentTruth_barcode ->clear();

    m_maxlinkParentTruth_inE  ->clear();
    m_maxlinkParentTruth_outE ->clear();

    m_maxlinkParentTruth_parent_ID      ->clear();
    m_maxlinkParentTruth_parent_pt      ->clear();
    m_maxlinkParentTruth_parent_eta     ->clear();
    m_maxlinkParentTruth_parent_phi     ->clear();
    m_maxlinkParentTruth_parent_E       ->clear();
    m_maxlinkParentTruth_parent_M       ->clear();
    m_maxlinkParentTruth_parent_charge  ->clear();
    m_maxlinkParentTruth_parent_pdgId   ->clear();
    m_maxlinkParentTruth_parent_status  ->clear();
    m_maxlinkParentTruth_parent_barcode ->clear();

    m_maxlinkParentTruth_outP_ID                ->clear();
    m_maxlinkParentTruth_outP_pt                ->clear();
    m_maxlinkParentTruth_outP_eta               ->clear();
    m_maxlinkParentTruth_outP_phi               ->clear();
    m_maxlinkParentTruth_outP_E                 ->clear();
    m_maxlinkParentTruth_outP_M                 ->clear();
    m_maxlinkParentTruth_outP_charge            ->clear();
    m_maxlinkParentTruth_outP_pdgId             ->clear();
    m_maxlinkParentTruth_outP_status            ->clear();
    m_maxlinkParentTruth_outP_barcode           ->clear();
    m_maxlinkParentTruth_outP_isReco            ->clear();
    m_maxlinkParentTruth_outP_recoProb          ->clear();
    m_maxlinkParentTruth_outP_recoID            ->clear();
    m_maxlinkParentTruth_outP_recoIsSelected    ->clear();
    m_maxlinkParentTruth_outP_recoIsAssociated  ->clear();
    m_maxlinkParentTruth_outP_isStable          ->clear();
    m_maxlinkParentTruth_outP_isInteracting     ->clear();
    m_maxlinkParentTruth_outP_isReconstructible ->clear();
  }
  // matched to jets
  if ( m_infoSwitch.m_jetMatched ) {
    if ( m_mc ) {
      m_truthJetMatch     ->clear();
      m_truthJetMatch_ID  ->clear();
      m_truthJetMatch_dR  ->clear();
      m_darkJetMatch      ->clear();
      m_darkJetMatch_ID   ->clear();
      m_darkJetMatch_dR   ->clear();
    }
    if ( m_doEMTopo ) {
      m_emtopoJetMatch    ->clear();
      m_emtopoJetMatch_ID ->clear();
      m_emtopoJetMatch_dR ->clear();
    }
    if ( m_doPFlow ) {
      m_pflowJetMatch     ->clear();
      m_pflowJetMatch_ID  ->clear();
      m_pflowJetMatch_dR  ->clear();
    }
  }
}


void SecondaryVertexContainer :: FillSecondaryVertex ( const xAOD::Vertex* secVtx, const std::string treeName )
{
  if ( m_debug ) std::cout << "SecondaryVertex : FillSecondaryVertex()" << std::endl;
  

  
  // get vector of filtered tracks
  std::vector< const xAOD::TrackParticle* > filteredTracks;
  HelperFunctions::getFilteredTracks( secVtx, filteredTracks );
  
  if(filteredTracks.size() < 2) {
    return;
  }

  VertexContainer::FillVertex ();
  const TVector3 pos( secVtx->x(), secVtx->y(), secVtx->z() );

  // // check if vertex passed selections ...
  bool pass_sel = secVtx->auxdataConst<char>("passSel");

  // re-calculate vertex kinematic variables using filtered tracks
  const TLorentzVector& sumP4                  = VsiBonsai::sumP4       ( filteredTracks );
  const double          scalarSumP             = VsiBonsai::scalarSumP  ( filteredTracks );
  const double          scalarSumPt            = VsiBonsai::scalarSumPt ( filteredTracks );
  const int             sumCharge              = VsiBonsai::sumCharge   ( filteredTracks );
  const double          minOpAngle             = VsiBonsai::minOpAng    ( filteredTracks );
  const double          maxOpAngle             = VsiBonsai::maxOpAng    ( filteredTracks );
  const double          mind0                  = VsiBonsai::mind0       ( filteredTracks );
  const double          maxd0                  = VsiBonsai::maxd0       ( filteredTracks );


  const double          minOneTrackRemovedMass = VsiBonsai::minOneTrackRemovedMass       ( filteredTracks );
  const auto            twoTrackMassTuples     = VsiBonsai::allTwoTracksMassCombinations ( filteredTracks );
  
  const double dir  = sumP4.Vect().Dot( pos ) / sumP4.Vect().Mag() / pos.Mag();

  float mass_nonAssoc = AUXDYN( secVtx, float, "mass_selectedTracks" );

  /* pt = sqrt( px*px + py*py ) (same as 'sumP4.Pt())
     eta = pz / TMath::Abs(p)  * TMath::ACosH(p/pt) (same as 'sumP4.Eta()')
     phi = py / TMath::Abs(py) * TMath::ACos(px/pt) (same as 'sumP4.Phi()')
     --> NOT the same as 'secVtx->position().phi()' ... which is right ???? 

     --> not sure why 'sumP4.P*[M]()' differs from "vtx_p*[mass]" decorator, 
     but stick w/ 'sumP4.P*()' since "vtx_p*" decorator will become irrelevant
     once we start filtering tracks ...
     --> do we want to save px/y/z ?? do we want to save sumP4 as a whole ??
     do we want to save p4 (using track parameters wrt pv instead of dv) ?? */

  m_ID            ->push_back( AUXDYN( secVtx, int, "ID" )                );
  m_x             ->push_back( secVtx->x()                                );
  m_y             ->push_back( secVtx->y()                                );
  m_z             ->push_back( secVtx->z()                                );
  m_r             ->push_back( secVtx->position().perp()                  );
  m_pt            ->push_back( sumP4.Pt() / m_units                       );
  m_eta           ->push_back( sumP4.Eta()                                );
  m_phi           ->push_back( sumP4.Phi()                                ); 
  m_mass          ->push_back( sumP4.M() / m_units                        );
  m_mass_nonAssoc ->push_back( mass_nonAssoc / m_units                    );
  m_covariance    ->push_back( secVtx->covariance()                       );
  m_chi2          ->push_back( secVtx->chiSquared() / secVtx->numberDoF() );
  m_direction     ->push_back( dir                                        );
  m_charge        ->push_back( sumCharge                                  );
  m_H             ->push_back( scalarSumP / m_units                       );
  m_Ht            ->push_back( scalarSumPt / m_units                      );
  m_minOpAng      ->push_back( minOpAngle                                 );
  m_maxOpAng      ->push_back( maxOpAngle                                 );
  m_mind0         ->push_back( mind0                                      );
  m_maxd0         ->push_back( maxd0                                      );
  m_pass_sel      ->push_back( pass_sel                                   );

  std::vector<float> twoTracksMass;
  std::vector<float> twoTracksMassRest;
  std::vector<int>   twoTracksCharge;
  for ( const auto& tuple : twoTrackMassTuples ) {
    double mass     = std::get<0>( tuple );
    double massRest = std::get<1>( tuple );

    const auto* trk1 = std::get<2>( tuple ).first;
    const auto* trk2 = std::get<2>( tuple ).second;
    
    int charge = trk1->charge() + trk2->charge();

    twoTracksMass     .push_back( mass / m_units     );
    twoTracksMassRest .push_back( massRest / m_units );
    twoTracksCharge   .push_back( charge             );
  }

  m_minOneTrackRemovedMass ->push_back( minOneTrackRemovedMass / m_units );
  m_twoTracksMass          ->push_back( twoTracksMass                    );
  m_twoTracksMassRest      ->push_back( twoTracksMassRest                );
  m_twoTracksCharge        ->push_back( twoTracksCharge                  );

  size_t ntrk_sel = 0;
  size_t ntrk_assoc = 0;
  for ( const auto& trk : filteredTracks ) {
    if ( trk->isAvailable<char>( "is_selected" ) )
      if ( trk->auxdataConst<char>( "is_selected" ) ) ++ntrk_sel;
    if ( trk->isAvailable<char>( "is_associated" ) )
      if ( trk->auxdataConst<char>( "is_associated" ) ) ++ntrk_assoc;
  }

  m_ntrk       ->push_back( filteredTracks.size() );
  m_ntrk_sel   ->push_back( ntrk_sel              );
  m_ntrk_assoc ->push_back( ntrk_assoc            );


  if ( m_infoSwitch.m_jetMatched ) {
    std::string jet_str = treeName;
    if ( treeName == "nominal" ) jet_str = "";

    uint8_t isTruthJetMatch = false;
    if ( secVtx->isAvailable<char>("isMatchedToTruthJet") ) {
      isTruthJetMatch = secVtx->auxdataConst<char>("isMatchedToTruthJet");
    }
    
    uint8_t isEMTopoJetMatch = false;
    std::string emtopo_str = "";
    if ( secVtx->isAvailable<char>("isMatchedToRecoJet_EMTopo" + jet_str) ) {
      isEMTopoJetMatch = secVtx->auxdataConst<char>("isMatchedToRecoJet_EMTopo" + jet_str);
      emtopo_str = jet_str;
    }
    else if ( secVtx->isAvailable<char>("isMatchedToRecoJet_EMTopo" ) )
      isEMTopoJetMatch = secVtx->auxdataConst<char>("isMatchedToRecoJet_EMTopo");
    
    uint8_t isPFlowJetMatch = false;
    std::string pflow_str = "";
    if ( secVtx->isAvailable<char>("isMatchedToRecoJet_PFlow" + jet_str) ) {
      isPFlowJetMatch = secVtx->auxdataConst<char>("isMatchedToRecoJet_PFlow" + jet_str);
      pflow_str = jet_str;
    }
    else if ( secVtx->isAvailable<char>("isMatchedToRecoJet_PFlow") )
      isPFlowJetMatch = secVtx->auxdataConst<char>("isMatchedToRecoJet_PFlow");

    if ( m_mc ) {
      m_truthJetMatch     ->push_back( isTruthJetMatch );
      m_truthJetMatch_ID  ->push_back( AUXDYNVEC( secVtx, int,   "truthJetMatchIDs" ) );
      m_truthJetMatch_dR  ->push_back( AUXDYNVEC( secVtx, float, "truthJetMatchDRs" ) );
      m_darkJetMatch_ID   ->push_back( AUXDYNVEC( secVtx, int,   "darkJetMatchIDs"  ) );
      m_darkJetMatch_dR   ->push_back( AUXDYNVEC( secVtx, float, "darkJetMatchDRs"  ) );
    }
    if ( m_doEMTopo ) {
      m_emtopoJetMatch    ->push_back( isEMTopoJetMatch );
      m_emtopoJetMatch_ID ->push_back( AUXDYNVEC( secVtx, int,   "recoJetMatchIDs_EMTopo" + emtopo_str ) );
      m_emtopoJetMatch_dR ->push_back( AUXDYNVEC( secVtx, float, "recoJetMatchDRs_EMTopo" + emtopo_str ) );
    }
    if ( m_doPFlow ) {
      m_pflowJetMatch     ->push_back( isPFlowJetMatch  );
      m_pflowJetMatch_ID  ->push_back( AUXDYNVEC( secVtx, int,   "recoJetMatchIDs_PFlow"  + pflow_str  ) );
      m_pflowJetMatch_dR  ->push_back( AUXDYNVEC( secVtx, float, "recoJetMatchDRs_PFlow"  + pflow_str  ) );
    }
  }

  //////////////////////////////
  // tracks associated to DVs //
  //////////////////////////////
  if ( m_infoSwitch.m_tracks ) {
    recordTracks( filteredTracks );
  }

  // ////////////////////////////
  // // DV-Jets (experimental) //
  // ////////////////////////////
  // if ( m_infoSwitch.m_jets ) {
  //   processDVJets( filteredTracks );
  // }

  ////////////////////
  // truth matching //
  ////////////////////
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_close )  // distance-based
    processCloseTruth( secVtx, filteredTracks );
  if ( m_infoSwitch.m_truth || m_infoSwitch.m_linked ) // truth-track-link-based
    processLinkedTruth( secVtx, filteredTracks );
  // --> these cover the two cases (ClosestTruth + ReprTruth) in VsiPerf
  // --> --> may want to add another case using our own definition of "representative" truth...
  // --> --> --> see efficiency processor ...
}


void SecondaryVertexContainer :: recordTracks ( const std::vector<const xAOD::TrackParticle*>& filteredTracks )
{
  if ( m_debug ) std::cout << "SecondaryVertexContainer : recordTracks()" << std::endl;

  std::vector<int>   trk_ID;
  std::vector<float> trk_qOverP;
  std::vector<float> trk_theta;
  std::vector<float> trk_vz;
  std::vector<float> trk_E;
  std::vector<float> trk_M;
  std::vector<float> trk_pt;
  std::vector<float> trk_eta;
  std::vector<float> trk_phi;
  std::vector<float> trk_d0;
  std::vector<float> trk_z0;
  std::vector<float> trk_errd0;
  std::vector<float> trk_errz0;
  std::vector<float> trk_chi2;

  std::vector<float> trk_pt_wrtSV;
  std::vector<float> trk_eta_wrtSV;
  std::vector<float> trk_phi_wrtSV;
  std::vector<float> trk_d0_wrtSV;
  std::vector<float> trk_z0_wrtSV;
  std::vector<float> trk_errd0_wrtSV;
  std::vector<float> trk_errz0_wrtSV;
  std::vector<float> trk_chi2_toSV;

  std::vector<int>      trk_charge;
  std::vector<uint8_t>  trk_isFinal;
  std::vector<uint8_t>  trk_isSelected;
  std::vector<uint8_t>  trk_isAssociated;
  std::vector<uint8_t>  trk_nPixelHits;
  std::vector<uint8_t>  trk_nSCTHits;
  std::vector<uint8_t>  trk_nTRTHits;
  std::vector<uint8_t>  trk_nPixelBarrelLayers;
  std::vector<uint8_t>  trk_nPixelEndCapLayers;
  std::vector<uint8_t>  trk_nSCTBarrelLayers;
  std::vector<uint8_t>  trk_nSCTEndCapLayers;
  std::vector<uint32_t> trk_hitPattern;
  std::vector<float>    trk_dEdx;
  std::vector<uint8_t>  trk_nUsedHitsdEdx;
  std::vector<uint8_t>  trk_nIBLOverflowsdEdx;
  std::vector<float>    trk_radiusOfFirstHit;

  std::vector<float> trk_truthProb;
  std::vector<int>   trk_truthID;
  std::vector<int>   trk_truthBarcode;
  std::vector<int>   trk_truthPdgId;

  for ( const auto& trk : filteredTracks ) {
    if ( !trk ) continue;

    trk_ID     .push_back( AUXDYN( trk, int, "ID" )                );
    trk_qOverP .push_back( trk->qOverP()                           );
    trk_theta  .push_back( trk->theta()                            );
    trk_vz     .push_back( trk->vz()                               );
    trk_E      .push_back( trk->e()  / m_units                     );
    trk_M      .push_back( trk->m()  / m_units                     );
    trk_pt     .push_back( trk->pt() / m_units                     );
    trk_eta    .push_back( trk->eta()                              );
    trk_phi    .push_back( trk->phi()                              );
    trk_d0     .push_back( trk->d0()                               );
    trk_z0     .push_back( trk->z0()                               );
    trk_errd0  .push_back( trk->definingParametersCovMatrix()(0,0) );
    trk_errz0  .push_back( trk->definingParametersCovMatrix()(1,1) );
    trk_chi2   .push_back( trk->chiSquared() / trk->numberDoF()    );

    trk_pt_wrtSV    .push_back( AUXDYN( trk, float, "pt_wrtSV"    ) / m_units );
    trk_eta_wrtSV   .push_back( AUXDYN( trk, float, "eta_wrtSV"   )           );
    trk_phi_wrtSV   .push_back( AUXDYN( trk, float, "phi_wrtSV"   )           );
    trk_d0_wrtSV    .push_back( AUXDYN( trk, float, "d0_wrtSV"    )           );
    trk_z0_wrtSV    .push_back( AUXDYN( trk, float, "z0_wrtSV"    )           );
    trk_errd0_wrtSV .push_back( AUXDYN( trk, float, "errd0_wrtSV" )           );
    trk_errz0_wrtSV .push_back( AUXDYN( trk, float, "errz0_wrtSV" )           );
    trk_chi2_toSV   .push_back( AUXDYN( trk, float, "chi2_toSV"   )           );

    trk_charge       .push_back( trk->charge() );
    trk_isFinal      .push_back( AUXDYN( trk, char, "is_svtrk_final" ) );
    trk_isSelected   .push_back( AUXDYN( trk, char, "is_selected"    ) );
    trk_isAssociated .push_back( AUXDYN( trk, char, "is_associated"  ) );

    uint8_t nPixelHits = 0;
    uint8_t nSCTHits   = 0;
    uint8_t nTRTHits   = 0;
    float   pixeldEdx  = 0;
    trk->summaryValue ( nPixelHits, xAOD::numberOfPixelHits );
    trk->summaryValue ( nSCTHits,   xAOD::numberOfSCTHits   );
    trk->summaryValue ( nTRTHits,   xAOD::numberOfTRTHits   );
    trk->summaryValue ( pixeldEdx,  xAOD::pixeldEdx         );

    uint8_t nPixelBarrelLayers = VsiTool::nPixelBarrelLayers ( trk->hitPattern() );
    uint8_t nPixelEndCapLayers = VsiTool::nPixelEndCapLayers ( trk->hitPattern() );
    uint8_t nSCTBarrelLayers   = VsiTool::nSctBarrelLayers   ( trk->hitPattern() );
    uint8_t nSCTEndCapLayers   = VsiTool::nSctEndCapLayers   ( trk->hitPattern() );
    
    trk_nPixelHits         .push_back( nPixelHits                      );
    trk_nSCTHits           .push_back( nSCTHits                        );
    trk_nTRTHits           .push_back( nTRTHits                        );
    trk_nPixelBarrelLayers .push_back( nPixelBarrelLayers              );
    trk_nPixelEndCapLayers .push_back( nPixelEndCapLayers              );
    trk_nSCTBarrelLayers   .push_back( nSCTBarrelLayers                );
    trk_nSCTEndCapLayers   .push_back( nSCTEndCapLayers                );
    trk_hitPattern         .push_back( trk->hitPattern()               );
    trk_dEdx               .push_back( pixeldEdx                       );
//    trk_nUsedHitsdEdx      .push_back( trk->numberOfUsedHitsdEdx()     );
//    trk_nIBLOverflowsdEdx  .push_back( trk->numberOfIBLOverflowsdEdx() );
    trk_radiusOfFirstHit   .push_back( trk->radiusOfFirstHit()         );

    const xAOD::TruthParticle* truthPart = 0;
    static SG::AuxElement::ConstAccessor<HelperFunctions::TruthParticleLink_t> truthAccess("truthParticleLink");
    if ( truthAccess.isAvailable( *trk ) ) {
      try {
        const HelperFunctions::TruthParticleLink_t& truthPartLink = truthAccess( *trk );
        truthPart = *truthPartLink;
      } catch(...) {}
    }
    if ( truthPart ) {
      trk_truthProb    .push_back( trk->auxdataConst<float>("truthMatchProbability") );
      trk_truthID      .push_back( AUXDYN( truthPart, int, "ID" )                    );
      trk_truthBarcode .push_back( truthPart->barcode()                              );
      trk_truthPdgId   .push_back( truthPart->pdgId()                                );
    }
    else {
      trk_truthProb    .push_back( AlgConsts::invalidFloat );
      trk_truthID      .push_back( AlgConsts::invalidInt   );
      trk_truthBarcode .push_back( AlgConsts::invalidInt   );
      trk_truthPdgId   .push_back( AlgConsts::invalidInt   );
    }
    
  }

  m_trk_ID     ->push_back( trk_ID     );
  m_trk_qOverP ->push_back( trk_qOverP );
  m_trk_theta  ->push_back( trk_theta  );
  m_trk_vz     ->push_back( trk_vz     );
  m_trk_E      ->push_back( trk_E      );
  m_trk_M      ->push_back( trk_M      );
  m_trk_pt     ->push_back( trk_pt     );
  m_trk_eta    ->push_back( trk_eta    );
  m_trk_phi    ->push_back( trk_phi    );
  m_trk_d0     ->push_back( trk_d0     );
  m_trk_z0     ->push_back( trk_z0     );
  m_trk_errd0  ->push_back( trk_errd0  );
  m_trk_errz0  ->push_back( trk_errz0  );
  m_trk_chi2   ->push_back( trk_chi2   );

  m_trk_pt_wrtSV    ->push_back( trk_pt_wrtSV    );
  m_trk_eta_wrtSV   ->push_back( trk_eta_wrtSV   );
  m_trk_phi_wrtSV   ->push_back( trk_phi_wrtSV   );
  m_trk_d0_wrtSV    ->push_back( trk_d0_wrtSV    );
  m_trk_z0_wrtSV    ->push_back( trk_z0_wrtSV    );
  m_trk_errd0_wrtSV ->push_back( trk_errd0_wrtSV );
  m_trk_errz0_wrtSV ->push_back( trk_errz0_wrtSV );
  m_trk_chi2_toSV   ->push_back( trk_chi2_toSV   );

  m_trk_charge             ->push_back( trk_charge       );
  m_trk_isFinal            ->push_back( trk_isFinal      );
  m_trk_isSelected         ->push_back( trk_isSelected   );
  m_trk_isAssociated       ->push_back( trk_isAssociated );
  m_trk_nPixelHits         ->push_back( trk_nPixelHits   );
  m_trk_nSCTHits           ->push_back( trk_nSCTHits     );
  m_trk_nTRTHits           ->push_back( trk_nTRTHits     );
  m_trk_nPixelBarrelLayers ->push_back( trk_nPixelBarrelLayers );
  m_trk_nPixelEndCapLayers ->push_back( trk_nPixelEndCapLayers );
  m_trk_nSCTBarrelLayers   ->push_back( trk_nSCTBarrelLayers   );
  m_trk_nSCTEndCapLayers   ->push_back( trk_nSCTEndCapLayers   );
  m_trk_hitPattern         ->push_back( trk_hitPattern         );
  m_trk_dEdx               ->push_back( trk_dEdx               );
  m_trk_nUsedHitsdEdx      ->push_back( trk_nUsedHitsdEdx      );
  m_trk_nIBLOverflowsdEdx  ->push_back( trk_nIBLOverflowsdEdx  );
  m_trk_radiusOfFirstHit   ->push_back( trk_radiusOfFirstHit   );

  if ( m_infoSwitch.m_truth || m_infoSwitch.m_close || m_infoSwitch.m_linked ) {
    m_trk_truthProb    ->push_back( trk_truthProb );
    m_trk_truthID      ->push_back( trk_truthID        );
    m_trk_truthBarcode ->push_back( trk_truthBarcode   );
    m_trk_truthPdgId   ->push_back( trk_truthPdgId     );
  }

  // m_trk_truthPdgId, m_trk_truthIsFromClosestTV, m_trk_truthIsFromReprTV

} // end recordTracks


void SecondaryVertexContainer :: processCloseTruth ( const xAOD::Vertex* secVtx,
                 const std::vector<const xAOD::TrackParticle*>& filteredTracks )
{
  if ( m_debug ) std::cout << "SecondaryVertexContainer : processCloseTruth()" << std::endl;

  std::vector<uint8_t> close_isPid;
  std::vector<uint8_t> close_isOffPid;
  std::vector<int>     close_ID;
  std::vector<int>     close_barcode;
  
  // access close truth vertex links
  static SG::AuxElement::ConstAccessor<HelperFunctions::TruthVertexLinkVector_t> closeTVAccess("closeMatchedTruthVertexLinks");
  if ( closeTVAccess.isAvailable( *secVtx ) ) {
    const HelperFunctions::TruthVertexLinkVector_t closeTVLinks = closeTVAccess( *secVtx );
    for ( const auto& closelink : closeTVLinks ) {
      if ( !closelink.isValid() ) continue;
      close_ID       .push_back( AUXDYN( (*closelink), int, "ID" )                );
      close_barcode  .push_back( (*closelink)->barcode()                          );
    }
  }

  m_closeTruth_distance               ->push_back( AUXDYNVEC( secVtx, float, "closeMatchedTruthVertex_dist" ) );
  m_closeTruth_ID                     ->push_back( close_ID                                                   );
  m_closeTruth_barcode                ->push_back( close_barcode                                              );

  
  // access closest truth vertex link
  const xAOD::TruthVertex* closestTruthVertex = 0;
  static SG::AuxElement::ConstAccessor<HelperFunctions::TruthVertexLink_t> closestTVAccess("closestTruthVertexLink");
  if ( closestTVAccess.isAvailable( *secVtx ) ) {
    try {
      const HelperFunctions::TruthVertexLink_t& closestTVLink = closestTVAccess( *secVtx );
      closestTruthVertex = *closestTVLink;
    } catch(...) {}
  }

  float    dist     = AUXDYN( secVtx, double, "closestTruthVertex_dist" ); 
  unsigned isPid    = AlgConsts::invalidUnsigned;
  unsigned isOffPid = AlgConsts::invalidUnsigned;
  int      ID       = AlgConsts::invalidInt;
  float    x        = AlgConsts::invalidFloat;
  float    y        = AlgConsts::invalidFloat;
  float    z        = AlgConsts::invalidFloat;
  float    r        = AlgConsts::invalidFloat;
  float    eta      = AlgConsts::invalidFloat;
  float    phi      = AlgConsts::invalidFloat;
  int      barcode  = AlgConsts::invalidInt;
  
  TLorentzVector sumP4_in;
  TLorentzVector sumP4_out;

  int   parent_ID      = AlgConsts::invalidInt;
  float parent_pt      = AlgConsts::invalidFloat;
  float parent_eta     = AlgConsts::invalidFloat;
  float parent_phi     = AlgConsts::invalidFloat;
  float parent_E       = AlgConsts::invalidFloat;
  float parent_M       = AlgConsts::invalidFloat;
  float parent_charge  = AlgConsts::invalidFloat;
  int   parent_pdgId   = AlgConsts::invalidInt;
  int   parent_status  = AlgConsts::invalidInt;
  int   parent_barcode = AlgConsts::invalidInt;

  std::vector<int>     outP_ID;
  std::vector<float>   outP_pt;
  std::vector<float>   outP_eta;
  std::vector<float>   outP_phi;
  std::vector<float>   outP_E;
  std::vector<float>   outP_M;
  std::vector<float>   outP_charge;
  std::vector<int>     outP_pdgId;
  std::vector<int>     outP_status;
  std::vector<int>     outP_barcode;
  std::vector<uint8_t> outP_isReco;
  std::vector<float>   outP_recoProb;
  std::vector<int>     outP_recoID;
  std::vector<uint8_t> outP_recoIsSelected;
  std::vector<uint8_t> outP_recoIsAssociated;
  std::vector<uint8_t> outP_isStable;
  std::vector<uint8_t> outP_isInteracting;
  std::vector<uint8_t> outP_isReconstructible;

  std::vector<uint8_t> trk_truthPointsToTV;

  if ( closestTruthVertex ) {
//    isLLP    = HelperFunctions::selectLLP        ( closestTruthVertex, LLPpid );

    ID      = AUXDYN( closestTruthVertex, int, "ID" );
    x       = closestTruthVertex->x();
    y       = closestTruthVertex->y();
    z       = closestTruthVertex->z();
    r       = closestTruthVertex->perp();
    eta     = closestTruthVertex->eta();
    phi     = closestTruthVertex->phi();
    barcode = closestTruthVertex->barcode();

    for ( auto inLink : closestTruthVertex->incomingParticleLinks()){
      if ( !inLink.isValid()) continue;
      const xAOD::TruthParticle* inPart = *inLink;
      if ( !inPart ) continue;
      TLorentzVector p4;
      p4.SetPtEtaPhiM( inPart->pt(), inPart->eta(), inPart->phi(), inPart->m() );
      sumP4_in += p4;
    }
    for ( auto outLink : closestTruthVertex->outgoingParticleLinks()) {
      if (!outLink.isValid()) continue;
      const xAOD::TruthParticle* outPart = *outLink;
      if ( !outPart ) continue;
      TLorentzVector p4;
      p4.SetPtEtaPhiM( outPart->pt(), outPart->eta(), outPart->phi(), outPart->m() );
      sumP4_out += p4;
    }

    const auto* parent = closestTruthVertex->incomingParticle(0);
    if ( parent ) {
      parent_ID      = AUXDYN( parent, int, "ID" );
      parent_pt      = parent->pt() / m_units;
      parent_eta     = parent->eta();
      parent_phi     = parent->phi();
      parent_E       = parent->e()  / m_units;
      parent_M       = parent->m()  / m_units;
      parent_charge  = parent->charge();
      parent_pdgId   = parent->pdgId();
      parent_status  = parent->status();
      parent_barcode = parent->barcode();
    }

    for ( auto outPLink : closestTruthVertex->outgoingParticleLinks() ) {
      if ( !outPLink.isValid() ) continue;
      const auto* outP = *outPLink;
      if ( !outP ) continue;
      outP_ID               .push_back( AUXDYN( outP, int, "ID"                       ) );
      outP_pt               .push_back( outP->pt() / m_units                            );
      outP_eta              .push_back( outP->eta()                                     );
      outP_phi              .push_back( outP->phi()                                     );
      outP_E                .push_back( outP->e()  / m_units                            );
      outP_M                .push_back( outP->m()  / m_units                            );
      outP_charge           .push_back( outP->charge()                                  );
      outP_pdgId            .push_back( outP->pdgId()                                   );
      outP_status           .push_back( outP->status()                                  );
      outP_barcode          .push_back( outP->barcode()                                 );
      outP_isReco           .push_back( AUXDYN( outP, char,   "isTrackMatch"          ) );
      outP_recoProb         .push_back( AUXDYN( outP, double, "trackMatchProbability" ) );
      // get linked track
      int     recoID      = AlgConsts::invalidUnsigned;
      uint8_t recoIsSel   = AlgConsts::invalidUnsigned;
      uint8_t recoIsAssoc = AlgConsts::invalidUnsigned;
      static SG::AuxElement::ConstAccessor<HelperFunctions::TrackLink_t> recoAccess("trackLink");
      if ( recoAccess.isAvailable( *outP ) ) {
  try {
    const HelperFunctions::TrackLink_t& recoLink = recoAccess( *outP );
    recoID      = AUXDYN( (*recoLink), int,  "ID"            );
    recoIsSel   = AUXDYN( (*recoLink), char, "is_selected"   );
    recoIsAssoc = AUXDYN( (*recoLink), char, "is_associated" );
  } catch(...) {}
      }
      outP_recoID            .push_back( recoID                                );
      outP_recoIsSelected    .push_back( recoIsSel                             );
      outP_recoIsAssociated  .push_back( recoIsAssoc                           );
      outP_isStable          .push_back( HelperFunctions::isStable          ( outP ) );
      outP_isInteracting     .push_back( HelperFunctions::isInteracting     ( outP ) );
      outP_isReconstructible .push_back( HelperFunctions::isReconstructible ( outP ) );
    }

    for ( const auto& trk : filteredTracks ) {
      const auto* truthProdVtx = HelperFunctions::getProdVtx( trk );
      bool truthPointsToTV = false;
      if ( truthProdVtx ) truthPointsToTV = ( closestTruthVertex == truthProdVtx );
      trk_truthPointsToTV .push_back( truthPointsToTV );
    }
  }
  else trk_truthPointsToTV .resize( filteredTracks.size(), false );

  m_closestTruth_distance               ->push_back( dist     );

  m_closestTruth_ID      ->push_back( ID      );
  m_closestTruth_x       ->push_back( x       );
  m_closestTruth_y       ->push_back( y       );
  m_closestTruth_z       ->push_back( z       );
  m_closestTruth_r       ->push_back( r       );
  m_closestTruth_eta     ->push_back( eta     );
  m_closestTruth_phi     ->push_back( phi     );
  m_closestTruth_barcode ->push_back( barcode );

  m_closestTruth_inE  ->push_back( sumP4_in.E()  / m_units );
  m_closestTruth_outE ->push_back( sumP4_out.E() / m_units );

  m_closestTruth_parent_ID      ->push_back( parent_ID      );
  m_closestTruth_parent_pt      ->push_back( parent_pt      );
  m_closestTruth_parent_eta     ->push_back( parent_eta     );
  m_closestTruth_parent_phi     ->push_back( parent_phi     );
  m_closestTruth_parent_E       ->push_back( parent_E       );
  m_closestTruth_parent_M       ->push_back( parent_M       );
  m_closestTruth_parent_charge  ->push_back( parent_charge  );
  m_closestTruth_parent_pdgId   ->push_back( parent_pdgId   );
  m_closestTruth_parent_status  ->push_back( parent_status  );
  m_closestTruth_parent_barcode ->push_back( parent_barcode );

  m_closestTruth_outP_ID                ->push_back( outP_ID                );
  m_closestTruth_outP_pt                ->push_back( outP_pt                );
  m_closestTruth_outP_eta               ->push_back( outP_eta               );
  m_closestTruth_outP_phi               ->push_back( outP_phi               );
  m_closestTruth_outP_E                 ->push_back( outP_E                 );
  m_closestTruth_outP_M                 ->push_back( outP_M                 );
  m_closestTruth_outP_charge            ->push_back( outP_charge            );
  m_closestTruth_outP_pdgId             ->push_back( outP_pdgId             );
  m_closestTruth_outP_status            ->push_back( outP_status            );
  m_closestTruth_outP_barcode           ->push_back( outP_barcode           );
  m_closestTruth_outP_isReco            ->push_back( outP_isReco            );
  m_closestTruth_outP_recoProb          ->push_back( outP_recoProb          );
  m_closestTruth_outP_recoID            ->push_back( outP_recoID            );
  m_closestTruth_outP_recoIsSelected    ->push_back( outP_recoIsSelected    );
  m_closestTruth_outP_recoIsAssociated  ->push_back( outP_recoIsAssociated  );
  m_closestTruth_outP_isStable          ->push_back( outP_isStable          );
  m_closestTruth_outP_isInteracting     ->push_back( outP_isInteracting     );
  m_closestTruth_outP_isReconstructible ->push_back( outP_isReconstructible );

  if ( m_infoSwitch.m_tracks )
    m_trk_truthPointsToClosestTV ->push_back( trk_truthPointsToTV );
  
} // end processCloseTruth


void SecondaryVertexContainer :: processLinkedTruth ( const xAOD::Vertex* secVtx,
                  const std::vector<const xAOD::TrackParticle*>& filteredTracks )
{
  if ( m_debug ) std::cout << "SecondaryVertex : processLinkedTruth()" << std::endl;

  std::vector<uint8_t> link_isPid;
  std::vector<uint8_t> link_isOffPid;
  std::vector<int>     link_ID;
  std::vector<int>     link_barcode;
  
  // access linked truth vertex links
  static SG::AuxElement::ConstAccessor<HelperFunctions::TruthVertexLinkVector_t> linkTVAccess("trackLinkedTruthVertexLinks");
  if ( linkTVAccess.isAvailable( *secVtx ) ) {
    const HelperFunctions::TruthVertexLinkVector_t linkTVLinks = linkTVAccess( *secVtx );
    for ( const auto& linklink : linkTVLinks ) {
      if ( !linklink.isValid() ) continue;
      link_ID       .push_back( AUXDYN( (*linklink), int, "ID" )                );
      link_barcode  .push_back( (*linklink)->barcode()                          );
    }
  }

  m_linkTruth_score                  ->push_back( AUXDYNVEC( secVtx, float, "trackLinkedTruthVertex_score" ) );
  m_linkTruth_ID                     ->push_back( link_ID                                                    );
  m_linkTruth_barcode                ->push_back( link_barcode                                               );


  // access max-linked truth vertex link
  const xAOD::TruthVertex* maxlinkedTruthVertex = 0;
  static SG::AuxElement::ConstAccessor<HelperFunctions::TruthVertexLink_t> maxlinkedTVAccess("maxlinkedTruthVertexLink");
  if ( maxlinkedTVAccess.isAvailable( *secVtx ) ) {
    try {
      const HelperFunctions::TruthVertexLink_t& maxlinkedTVLink = maxlinkedTVAccess( *secVtx );
      maxlinkedTruthVertex = *maxlinkedTVLink;
    } catch(...) {}
  }

  float    tv_score    = AUXDYN( secVtx, double, "maxlinkedTruthVertex_score" );
  unsigned tv_isPid    = AlgConsts::invalidUnsigned;
  unsigned tv_isOffPid = AlgConsts::invalidUnsigned;
  int      tv_ID       = AlgConsts::invalidInt;
  float    tv_x        = AlgConsts::invalidFloat;
  float    tv_y        = AlgConsts::invalidFloat;
  float    tv_z        = AlgConsts::invalidFloat;
  float    tv_r        = AlgConsts::invalidFloat;
  float    tv_eta      = AlgConsts::invalidFloat;
  float    tv_phi      = AlgConsts::invalidFloat;
  int      tv_barcode  = AlgConsts::invalidInt;

  TLorentzVector tv_sumP4_in;
  TLorentzVector tv_sumP4_out;

  int   tv_parent_ID      = AlgConsts::invalidInt;
  float tv_parent_pt      = AlgConsts::invalidFloat;
  float tv_parent_eta     = AlgConsts::invalidFloat;
  float tv_parent_phi     = AlgConsts::invalidFloat;
  float tv_parent_E       = AlgConsts::invalidFloat;
  float tv_parent_M       = AlgConsts::invalidFloat;
  float tv_parent_charge  = AlgConsts::invalidFloat;
  int   tv_parent_pdgId   = AlgConsts::invalidInt;
  int   tv_parent_status  = AlgConsts::invalidInt;
  int   tv_parent_barcode = AlgConsts::invalidInt;

  std::vector<int>     tv_outP_ID;
  std::vector<float>   tv_outP_pt;
  std::vector<float>   tv_outP_eta;
  std::vector<float>   tv_outP_phi;
  std::vector<float>   tv_outP_E;
  std::vector<float>   tv_outP_M;
  std::vector<float>   tv_outP_charge;
  std::vector<int>     tv_outP_pdgId;
  std::vector<int>     tv_outP_status;
  std::vector<int>     tv_outP_barcode;
  std::vector<uint8_t> tv_outP_isReco;
  std::vector<float>   tv_outP_recoProb;
  std::vector<int>     tv_outP_recoID;
  std::vector<uint8_t> tv_outP_recoIsSelected;
  std::vector<uint8_t> tv_outP_recoIsAssociated;
  std::vector<uint8_t> tv_outP_isStable;
  std::vector<uint8_t> tv_outP_isInteracting;
  std::vector<uint8_t> tv_outP_isReconstructible;

  std::vector<uint8_t> trk_truthPointsToTV;

  if ( maxlinkedTruthVertex ) {
//    tv_isPid    = HelperFunctions::selectLLP        ( maxlinkedTruthVertex, LLPpid );

    tv_ID      = AUXDYN( maxlinkedTruthVertex, int, "ID" );
    tv_x       = maxlinkedTruthVertex->x();
    tv_y       = maxlinkedTruthVertex->y();
    tv_z       = maxlinkedTruthVertex->z();
    tv_r       = maxlinkedTruthVertex->perp();
    tv_eta     = maxlinkedTruthVertex->eta();
    tv_phi     = maxlinkedTruthVertex->phi();
    tv_barcode = maxlinkedTruthVertex->barcode();

    for ( auto inPartLink : maxlinkedTruthVertex->incomingParticleLinks() ) {
      if ( !inPartLink.isValid() ) continue;
      const xAOD::TruthParticle* inPart = *inPartLink;
      if ( !inPart ) continue;
      TLorentzVector p4;
      p4.SetPtEtaPhiM( inPart->pt(), inPart->eta(), inPart->phi(), inPart->m() );
      tv_sumP4_in += p4;
    }
    for ( auto outPartLink : maxlinkedTruthVertex->outgoingParticleLinks() ) {
      if ( !outPartLink.isValid() ) continue;
      const xAOD::TruthParticle* outPart = *outPartLink;
      if ( !outPart ) continue;
      TLorentzVector p4;
      p4.SetPtEtaPhiM( outPart->pt(), outPart->eta(), outPart->phi(), outPart->m() );
      tv_sumP4_out += p4;
    }

    const auto* parent = maxlinkedTruthVertex->incomingParticle(0);
    if ( parent ) {
      tv_parent_ID      = AUXDYN( parent, int, "ID" );
      tv_parent_pt      = parent->pt() / m_units;
      tv_parent_eta     = parent->eta();
      tv_parent_phi     = parent->phi();
      tv_parent_E       = parent->e()  / m_units;
      tv_parent_M       = parent->m()  / m_units;
      tv_parent_charge  = parent->charge();
      tv_parent_pdgId   = parent->pdgId();
      tv_parent_status  = parent->status();
      tv_parent_barcode = parent->barcode();
    }

    for ( auto outPLink : maxlinkedTruthVertex->outgoingParticleLinks() ) {
      if ( !outPLink.isValid() ) continue;
      const xAOD::TruthParticle* outP = *outPLink;
      if ( !outP ) continue;
      tv_outP_ID               .push_back( AUXDYN( outP, int, "ID"                       ) );
      tv_outP_pt               .push_back( outP->pt() / m_units                            );
      tv_outP_eta              .push_back( outP->eta()                                     );
      tv_outP_phi              .push_back( outP->phi()                                     );
      tv_outP_E                .push_back( outP->e()  / m_units                            );
      tv_outP_M                .push_back( outP->m()  / m_units                            );
      tv_outP_charge           .push_back( outP->charge()                                  );
      tv_outP_pdgId            .push_back( outP->pdgId()                                   );
      tv_outP_status           .push_back( outP->status()                                  );
      tv_outP_barcode          .push_back( outP->barcode()                                 );
      tv_outP_isReco           .push_back( AUXDYN( outP, char,   "isTrackMatch"          ) );
      tv_outP_recoProb         .push_back( AUXDYN( outP, double, "trackMatchProbability" ) );
      // get linked track
      int     recoID      = AlgConsts::invalidUnsigned;
      uint8_t recoIsSel   = AlgConsts::invalidUnsigned;
      uint8_t recoIsAssoc = AlgConsts::invalidUnsigned;
      static SG::AuxElement::ConstAccessor<HelperFunctions::TrackLink_t> recoAccess("trackLink");
      if ( recoAccess.isAvailable( *outP ) ) {
        try {
        const HelperFunctions::TrackLink_t& recoLink = recoAccess( *outP );
        recoID      = AUXDYN( (*recoLink), int,  "ID"            );
        recoIsSel   = AUXDYN( (*recoLink), char, "is_selected"   );
        recoIsAssoc = AUXDYN( (*recoLink), char, "is_associated" );
        } catch(...) {}
      }
      tv_outP_recoID            .push_back( recoID                                );
      tv_outP_recoIsSelected    .push_back( recoIsSel                             );
      tv_outP_recoIsAssociated  .push_back( recoIsAssoc                           );
      tv_outP_isStable          .push_back( HelperFunctions::isStable          ( outP ) );
      tv_outP_isInteracting     .push_back( HelperFunctions::isInteracting     ( outP ) );
      tv_outP_isReconstructible .push_back( HelperFunctions::isReconstructible ( outP ) );
    }

    for ( const auto& trk : filteredTracks ) {
      const auto* truthProdVtx = HelperFunctions::getProdVtx( trk );
      bool truthPointsToTV = false;
      if ( truthProdVtx ) truthPointsToTV = ( maxlinkedTruthVertex == truthProdVtx );
      trk_truthPointsToTV .push_back( truthPointsToTV );
    }
  }
  else trk_truthPointsToTV .resize( filteredTracks.size(), false );

  m_maxlinkTruth_score                  ->push_back( tv_score    );

  m_maxlinkTruth_ID      ->push_back( tv_ID      );
  m_maxlinkTruth_x       ->push_back( tv_x       );
  m_maxlinkTruth_y       ->push_back( tv_y       );
  m_maxlinkTruth_z       ->push_back( tv_z       );
  m_maxlinkTruth_r       ->push_back( tv_r       );
  m_maxlinkTruth_eta     ->push_back( tv_eta     );
  m_maxlinkTruth_phi     ->push_back( tv_phi     );
  m_maxlinkTruth_barcode ->push_back( tv_barcode );

  m_maxlinkTruth_inE  ->push_back( tv_sumP4_in.E()  / m_units );
  m_maxlinkTruth_outE ->push_back( tv_sumP4_out.E() / m_units );

  m_maxlinkTruth_parent_ID      ->push_back( tv_parent_ID      );
  m_maxlinkTruth_parent_pt      ->push_back( tv_parent_pt      );
  m_maxlinkTruth_parent_eta     ->push_back( tv_parent_eta     );
  m_maxlinkTruth_parent_phi     ->push_back( tv_parent_phi     );
  m_maxlinkTruth_parent_E       ->push_back( tv_parent_E       );
  m_maxlinkTruth_parent_M       ->push_back( tv_parent_M       );
  m_maxlinkTruth_parent_charge  ->push_back( tv_parent_charge  );
  m_maxlinkTruth_parent_pdgId   ->push_back( tv_parent_pdgId   );
  m_maxlinkTruth_parent_status  ->push_back( tv_parent_status  );
  m_maxlinkTruth_parent_barcode ->push_back( tv_parent_barcode );

  m_maxlinkTruth_outP_ID                ->push_back( tv_outP_ID                );
  m_maxlinkTruth_outP_pt                ->push_back( tv_outP_pt                );
  m_maxlinkTruth_outP_eta               ->push_back( tv_outP_eta               );
  m_maxlinkTruth_outP_phi               ->push_back( tv_outP_phi               );
  m_maxlinkTruth_outP_E                 ->push_back( tv_outP_E                 );
  m_maxlinkTruth_outP_M                 ->push_back( tv_outP_M                 );
  m_maxlinkTruth_outP_charge            ->push_back( tv_outP_charge            );
  m_maxlinkTruth_outP_pdgId             ->push_back( tv_outP_pdgId             );
  m_maxlinkTruth_outP_status            ->push_back( tv_outP_status            );
  m_maxlinkTruth_outP_barcode           ->push_back( tv_outP_barcode           );
  m_maxlinkTruth_outP_isReco            ->push_back( tv_outP_isReco            );
  m_maxlinkTruth_outP_recoProb          ->push_back( tv_outP_recoProb          );
  m_maxlinkTruth_outP_recoID            ->push_back( tv_outP_recoID            );
  m_maxlinkTruth_outP_recoIsSelected    ->push_back( tv_outP_recoIsSelected    );
  m_maxlinkTruth_outP_recoIsAssociated  ->push_back( tv_outP_recoIsAssociated  );
  m_maxlinkTruth_outP_isStable          ->push_back( tv_outP_isStable          );
  m_maxlinkTruth_outP_isInteracting     ->push_back( tv_outP_isInteracting     );
  m_maxlinkTruth_outP_isReconstructible ->push_back( tv_outP_isReconstructible );

  if ( m_infoSwitch.m_tracks )
    m_trk_truthPointsToMaxlinkTV ->push_back( trk_truthPointsToTV );


  std::vector<uint8_t> linkP_isPid;
  std::vector<uint8_t> linkP_isOffPid;
  std::vector<int>     linkP_ID;
  std::vector<int>     linkP_barcode;
  
  // access linked parent truth vertex links
  static SG::AuxElement::ConstAccessor<HelperFunctions::TruthVertexLinkVector_t> linkPTVAccess("trackLinkedParentTruthVertexLinks");
  if ( linkPTVAccess.isAvailable( *secVtx ) ) {
    const HelperFunctions::TruthVertexLinkVector_t linkPTVLinks = linkPTVAccess( *secVtx );
    for ( const auto& linkPlink : linkPTVLinks ) {
      if ( !linkPlink.isValid() ) continue;
      linkP_ID       .push_back( AUXDYN( (*linkPlink), int, "ID" )                );
      linkP_barcode  .push_back( (*linkPlink)->barcode()                          );
    }
  }

  m_linkParentTruth_score                  ->push_back( AUXDYNVEC( secVtx, float, "trackLinkedParentTruthVertex_score" ) );
  m_linkParentTruth_ID                     ->push_back( linkP_ID                                                         );
  m_linkParentTruth_barcode                ->push_back( linkP_barcode                                                    );
  

  // access max-linked parent truth vertex link
  const xAOD::TruthVertex* maxlinkedParentTruthVertex = 0;
  static SG::AuxElement::ConstAccessor<HelperFunctions::TruthVertexLink_t> maxlinkedPTVAccess("maxlinkedParentTruthVertexLink");
  if ( maxlinkedPTVAccess.isAvailable( *secVtx ) ) {
    try {
      const HelperFunctions::TruthVertexLink_t& maxlinkedPTVLink = maxlinkedPTVAccess( *secVtx );
      maxlinkedParentTruthVertex = *maxlinkedPTVLink;
    } catch(...) {}
  }

  float    ptv_score    = AUXDYN( secVtx, double, "maxlinkedTruthVertex_score" );
  unsigned ptv_isPid    = AlgConsts::invalidUnsigned;
  unsigned ptv_isOffPid = AlgConsts::invalidUnsigned;
  int      ptv_ID       = AlgConsts::invalidInt;
  float    ptv_x        = AlgConsts::invalidFloat;
  float    ptv_y        = AlgConsts::invalidFloat;
  float    ptv_z        = AlgConsts::invalidFloat;
  float    ptv_r        = AlgConsts::invalidFloat;
  float    ptv_eta      = AlgConsts::invalidFloat;
  float    ptv_phi      = AlgConsts::invalidFloat;
  int      ptv_barcode  = AlgConsts::invalidInt;

  TLorentzVector ptv_sumP4_in;
  TLorentzVector ptv_sumP4_out;

  int   ptv_parent_ID      = AlgConsts::invalidInt;
  float ptv_parent_pt      = AlgConsts::invalidFloat;
  float ptv_parent_eta     = AlgConsts::invalidFloat;
  float ptv_parent_phi     = AlgConsts::invalidFloat;
  float ptv_parent_E       = AlgConsts::invalidFloat;
  float ptv_parent_M       = AlgConsts::invalidFloat;
  float ptv_parent_charge  = AlgConsts::invalidFloat;
  int   ptv_parent_pdgId   = AlgConsts::invalidInt;
  int   ptv_parent_status  = AlgConsts::invalidInt;
  int   ptv_parent_barcode = AlgConsts::invalidInt;

  std::vector<int>     ptv_outP_ID;
  std::vector<float>   ptv_outP_pt;
  std::vector<float>   ptv_outP_eta;
  std::vector<float>   ptv_outP_phi;
  std::vector<float>   ptv_outP_E;
  std::vector<float>   ptv_outP_M;
  std::vector<float>   ptv_outP_charge;
  std::vector<int>     ptv_outP_pdgId;
  std::vector<int>     ptv_outP_status;
  std::vector<int>     ptv_outP_barcode;
  std::vector<uint8_t> ptv_outP_isReco;
  std::vector<float>   ptv_outP_recoProb;
  std::vector<int>     ptv_outP_recoID;
  std::vector<uint8_t> ptv_outP_recoIsSelected;
  std::vector<uint8_t> ptv_outP_recoIsAssociated;
  std::vector<uint8_t> ptv_outP_isStable;
  std::vector<uint8_t> ptv_outP_isInteracting;
  std::vector<uint8_t> ptv_outP_isReconstructible;

  std::vector<uint8_t> trk_truthParentPointsToPTV;

  if ( maxlinkedParentTruthVertex ) {

    ptv_ID      = AUXDYN( maxlinkedParentTruthVertex, int, "ID" );
    ptv_x       = maxlinkedParentTruthVertex->x();
    ptv_y       = maxlinkedParentTruthVertex->y();
    ptv_z       = maxlinkedParentTruthVertex->z();
    ptv_r       = maxlinkedParentTruthVertex->perp();
    ptv_eta     = maxlinkedParentTruthVertex->eta();
    ptv_phi     = maxlinkedParentTruthVertex->phi();
    ptv_barcode = maxlinkedParentTruthVertex->barcode();

    for ( auto inPartLink : maxlinkedParentTruthVertex->incomingParticleLinks() ) {
      if ( !inPartLink.isValid() ) continue;
      const xAOD::TruthParticle* inPart = *inPartLink;
      if ( !inPart ) continue;
      TLorentzVector p4;
      p4.SetPtEtaPhiM( inPart->pt(), inPart->eta(), inPart->phi(), inPart->m() );
      ptv_sumP4_in += p4;
    }
    for ( auto outPartLink : maxlinkedParentTruthVertex->outgoingParticleLinks() ) {
      if ( !outPartLink.isValid() ) continue;
      const xAOD::TruthParticle* outPart = *outPartLink;
      if ( !outPart ) continue;
      TLorentzVector p4;
      p4.SetPtEtaPhiM( outPart->pt(), outPart->eta(), outPart->phi(), outPart->m() );
      ptv_sumP4_out += p4;
    }

    const auto* parent = maxlinkedParentTruthVertex->incomingParticle(0);
    if ( parent ) {
      ptv_parent_ID      = AUXDYN( parent, int, "ID" );
      ptv_parent_pt      = parent->pt() / m_units;
      ptv_parent_eta     = parent->eta();
      ptv_parent_phi     = parent->phi();
      ptv_parent_E       = parent->e()  / m_units;
      ptv_parent_M       = parent->m()  / m_units;
      ptv_parent_charge  = parent->charge();
      ptv_parent_pdgId   = parent->pdgId();
      ptv_parent_status  = parent->status();
      ptv_parent_barcode = parent->barcode();
    }

    for ( auto outPLink : maxlinkedParentTruthVertex->outgoingParticleLinks() ) {
      if ( !outPLink.isValid() ) continue;
      const xAOD::TruthParticle* outP = *outPLink;
      if ( !outP ) continue;
      ptv_outP_ID               .push_back( AUXDYN( outP, int, "ID"                       ) );
      ptv_outP_pt               .push_back( outP->pt() / m_units                            );
      ptv_outP_eta              .push_back( outP->eta()                                     );
      ptv_outP_phi              .push_back( outP->phi()                                     );
      ptv_outP_E                .push_back( outP->e()  / m_units                            );
      ptv_outP_M                .push_back( outP->m()  / m_units                            );
      ptv_outP_charge           .push_back( outP->charge()                                  );
      ptv_outP_pdgId            .push_back( outP->pdgId()                                   );
      ptv_outP_status           .push_back( outP->status()                                  );
      ptv_outP_barcode          .push_back( outP->barcode()                                 );
      ptv_outP_isReco           .push_back( AUXDYN( outP, char,   "isTrackMatch"          ) );
      ptv_outP_recoProb         .push_back( AUXDYN( outP, double, "trackMatchProbability" ) );
      // get linked track
      int     recoID      = AlgConsts::invalidUnsigned;
      uint8_t recoIsSel   = AlgConsts::invalidUnsigned;
      uint8_t recoIsAssoc = AlgConsts::invalidUnsigned;
      static SG::AuxElement::ConstAccessor<HelperFunctions::TrackLink_t> recoAccess("trackLink");
      if ( recoAccess.isAvailable( *outP ) ) {
  try {
    const HelperFunctions::TrackLink_t& recoLink = recoAccess( *outP );
    recoID      = AUXDYN( (*recoLink), int,  "ID"            );
    recoIsSel   = AUXDYN( (*recoLink), char, "is_selected"   );
    recoIsAssoc = AUXDYN( (*recoLink), char, "is_associated" );
  } catch(...) {}
      }
      ptv_outP_recoID            .push_back( recoID                                );
      ptv_outP_recoIsSelected    .push_back( recoIsSel                             );
      ptv_outP_recoIsAssociated  .push_back( recoIsAssoc                           );
      ptv_outP_isStable          .push_back( HelperFunctions::isStable          ( outP ) );
      ptv_outP_isInteracting     .push_back( HelperFunctions::isInteracting     ( outP ) );
      ptv_outP_isReconstructible .push_back( HelperFunctions::isReconstructible ( outP ) );
    }

    for ( const auto& trk : filteredTracks ) {
      const auto* truthParentProdVtx = HelperFunctions::getParentProdVtx( trk );
      bool truthParentPointsToPTV = false;
      if ( truthParentProdVtx ) truthParentPointsToPTV = ( maxlinkedParentTruthVertex == truthParentProdVtx );
      trk_truthParentPointsToPTV .push_back( truthParentPointsToPTV );
    }
  }
  else trk_truthParentPointsToPTV .resize( filteredTracks.size(), false );

  m_maxlinkParentTruth_score                  ->push_back( ptv_score    );

  m_maxlinkParentTruth_ID      ->push_back( ptv_ID      );
  m_maxlinkParentTruth_x       ->push_back( ptv_x       );
  m_maxlinkParentTruth_y       ->push_back( ptv_y       );
  m_maxlinkParentTruth_z       ->push_back( ptv_z       );
  m_maxlinkParentTruth_r       ->push_back( ptv_r       );
  m_maxlinkParentTruth_eta     ->push_back( ptv_eta     );
  m_maxlinkParentTruth_phi     ->push_back( ptv_phi     );
  m_maxlinkParentTruth_barcode ->push_back( ptv_barcode );

  m_maxlinkParentTruth_inE  ->push_back( ptv_sumP4_in.E()  / m_units );
  m_maxlinkParentTruth_outE ->push_back( ptv_sumP4_out.E() / m_units );

  m_maxlinkParentTruth_parent_ID      ->push_back( ptv_parent_ID      );
  m_maxlinkParentTruth_parent_pt      ->push_back( ptv_parent_pt      );
  m_maxlinkParentTruth_parent_eta     ->push_back( ptv_parent_eta     );
  m_maxlinkParentTruth_parent_phi     ->push_back( ptv_parent_phi     );
  m_maxlinkParentTruth_parent_E       ->push_back( ptv_parent_E       );
  m_maxlinkParentTruth_parent_M       ->push_back( ptv_parent_M       );
  m_maxlinkParentTruth_parent_charge  ->push_back( ptv_parent_charge  );
  m_maxlinkParentTruth_parent_pdgId   ->push_back( ptv_parent_pdgId   );
  m_maxlinkParentTruth_parent_status  ->push_back( ptv_parent_status  );
  m_maxlinkParentTruth_parent_barcode ->push_back( ptv_parent_barcode );

  m_maxlinkParentTruth_outP_ID                ->push_back( ptv_outP_ID                );
  m_maxlinkParentTruth_outP_pt                ->push_back( ptv_outP_pt                );
  m_maxlinkParentTruth_outP_eta               ->push_back( ptv_outP_eta               );
  m_maxlinkParentTruth_outP_phi               ->push_back( ptv_outP_phi               );
  m_maxlinkParentTruth_outP_E                 ->push_back( ptv_outP_E                 );
  m_maxlinkParentTruth_outP_M                 ->push_back( ptv_outP_M                 );
  m_maxlinkParentTruth_outP_charge            ->push_back( ptv_outP_charge            );
  m_maxlinkParentTruth_outP_pdgId             ->push_back( ptv_outP_pdgId             );
  m_maxlinkParentTruth_outP_status            ->push_back( ptv_outP_status            );
  m_maxlinkParentTruth_outP_barcode           ->push_back( ptv_outP_barcode           );
  m_maxlinkParentTruth_outP_isReco            ->push_back( ptv_outP_isReco            );
  m_maxlinkParentTruth_outP_recoProb          ->push_back( ptv_outP_recoProb          );
  m_maxlinkParentTruth_outP_recoID            ->push_back( ptv_outP_recoID            );
  m_maxlinkParentTruth_outP_recoIsSelected    ->push_back( ptv_outP_recoIsSelected    );
  m_maxlinkParentTruth_outP_recoIsAssociated  ->push_back( ptv_outP_recoIsAssociated  );
  m_maxlinkParentTruth_outP_isStable          ->push_back( ptv_outP_isStable          );
  m_maxlinkParentTruth_outP_isInteracting     ->push_back( ptv_outP_isInteracting     );
  m_maxlinkParentTruth_outP_isReconstructible ->push_back( ptv_outP_isReconstructible );

  if ( m_infoSwitch.m_tracks )
    m_trk_truthParentPointsToMaxlinkPTV ->push_back( trk_truthParentPointsToPTV );
  
} // end processLinkedTruth
