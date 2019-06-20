#include <xAODTruth/TruthVertex.h>
#include <xAODTruth/TruthParticle.h>

#include "xAODAnaHelpers/TruthVertexContainer.h"
#include "xAODAnaHelpers/AlgConsts.h"
#include "xAODAnaHelpers/HelperFunctions.h"

using namespace xAH;

TruthVertexContainer :: TruthVertexContainer ( const std::string& name, const std::string& detailStr, float units,
					       bool mc, bool emtopo, bool pflow ) :
  VertexContainer ( name, detailStr, units, mc )
{
  m_doEMTopo = emtopo;
  m_doPFlow  = pflow;
  
  if ( m_debug ) Info( "EJs::TruthVertexContainer()", "setting up" );

  m_isLLPDecay        = new std::vector<uint8_t>;
  m_ID                     = new std::vector<int>;
  m_x                      = new std::vector<float>;
  m_y                      = new std::vector<float>;
  m_z                      = new std::vector<float>;
  m_r                      = new std::vector<float>;
  m_pt                     = new std::vector<float>;
  m_eta                    = new std::vector<float>;
  m_phi                    = new std::vector<float>;
  m_mass                   = new std::vector<float>;
  m_nOutP                  = new std::vector<int>;
  m_barcode                = new std::vector<int>;
  m_parent_ID              = new std::vector<int>;
  m_parent_pt              = new std::vector<float>;
  m_parent_eta             = new std::vector<float>;
  m_parent_phi             = new std::vector<float>;
  m_parent_E               = new std::vector<float>;
  m_parent_M               = new std::vector<float>;
  m_parent_charge          = new std::vector<float>;
  m_parent_pdgId           = new std::vector<int>;
  m_parent_status          = new std::vector<int>;
  m_parent_barcode         = new std::vector<int>;
  m_parent_prod_ID         = new std::vector<int>;
  m_parent_prod_x          = new std::vector<float>;
  m_parent_prod_y          = new std::vector<float>;
  m_parent_prod_z          = new std::vector<float>;
  m_parent_prod_r          = new std::vector<float>;
  m_parent_prod_eta        = new std::vector<float>;
  m_parent_prod_phi        = new std::vector<float>;
  m_parent_prod_barcode    = new std::vector<int>;
  m_outP_ID                = new std::vector<std::vector<int>>;
  m_outP_pt                = new std::vector<std::vector<float>>;
  m_outP_eta               = new std::vector<std::vector<float>>;
  m_outP_phi               = new std::vector<std::vector<float>>;
  m_outP_E                 = new std::vector<std::vector<float>>;
  m_outP_M                 = new std::vector<std::vector<float>>;
  m_outP_charge            = new std::vector<std::vector<float>>;
  m_outP_pdgId             = new std::vector<std::vector<int>>;
  m_outP_status            = new std::vector<std::vector<int>>;
  m_outP_barcode           = new std::vector<std::vector<int>>;
  m_outP_isReco            = new std::vector<std::vector<uint8_t>>;
  m_outP_recoProb          = new std::vector<std::vector<float>>;
  m_outP_recoID            = new std::vector<std::vector<int>>;
  m_outP_recoIsSelected    = new std::vector<std::vector<uint8_t>>;
  m_outP_recoIsAssociated  = new std::vector<std::vector<uint8_t>>;
  m_outP_isStable          = new std::vector<std::vector<uint8_t>>;
  m_outP_isInteracting     = new std::vector<std::vector<uint8_t>>;
  m_outP_isReconstructible = new std::vector<std::vector<uint8_t>>;
  m_outP_isDark            = new std::vector<std::vector<uint8_t>>;
  
  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_recoVerts ) {
    m_isCloseToRecoVtx             = new std::vector<uint8_t>;
    m_closeRecoVtx_ID              = new std::vector<std::vector<int>>;
    m_closeRecoVtx_distance        = new std::vector<std::vector<float>>;
    m_isClosestToRecoVtx           = new std::vector<uint8_t>;
    m_closestRecoVtx_ID            = new std::vector<std::vector<int>>;
    m_closestRecoVtx_distance      = new std::vector<std::vector<float>>;
    m_isLinkedToRecoVtx            = new std::vector<uint8_t>;
    m_linkedRecoVtx_ID             = new std::vector<std::vector<int>>;
    m_linkedRecoVtx_score          = new std::vector<std::vector<float>>;
    m_isMaxlinkedToRecoVtx         = new std::vector<uint8_t>;
    m_maxlinkedRecoVtx_ID          = new std::vector<int>;
    m_maxlinkedRecoVtx_score       = new std::vector<float>;
    m_isLinkedParentToRecoVtx      = new std::vector<uint8_t>;
    m_linkedParentRecoVtx_ID       = new std::vector<std::vector<int>>;
    m_linkedParentRecoVtx_score    = new std::vector<std::vector<float>>;
    m_isMaxlinkedParentToRecoVtx   = new std::vector<uint8_t>;
    m_maxlinkedParentRecoVtx_ID    = new std::vector<std::vector<int>>;
    m_maxlinkedParentRecoVtx_score = new std::vector<std::vector<float>>;
  }
  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_jets ) {
    m_truthJetMatch    = new std::vector<uint8_t>;
    m_truthJetMatch_ID = new std::vector<std::vector<int>>;
    m_truthJetMatch_dR = new std::vector<std::vector<float>>;
    m_darkJetMatch     = new std::vector<uint8_t>;
    m_darkJetMatch_ID  = new std::vector<std::vector<int>>;
    m_darkJetMatch_dR  = new std::vector<std::vector<float>>;
    if ( m_doEMTopo ) {
      m_emtopoJetMatch    = new std::vector<uint8_t>;
      m_emtopoJetMatch_ID = new std::vector<std::vector<int>>;
      m_emtopoJetMatch_dR = new std::vector<std::vector<float>>;
    }
    if ( m_doPFlow ) {
      m_pflowJetMatch    = new std::vector<uint8_t>;
      m_pflowJetMatch_ID = new std::vector<std::vector<int>>;
      m_pflowJetMatch_dR = new std::vector<std::vector<float>>;
    }
  }
}


TruthVertexContainer :: ~TruthVertexContainer ()
{
  if ( m_debug ) Info( "EJs::TruthVertexContainer()", "deleting" );

  delete m_isLLPDecay;
  delete m_ID;
  delete m_x;
  delete m_y;
  delete m_z;
  delete m_r;
  delete m_pt;
  delete m_eta;
  delete m_phi;
  delete m_mass;
  delete m_nOutP;
  delete m_barcode;
  delete m_parent_ID;
  delete m_parent_pt;
  delete m_parent_eta;
  delete m_parent_phi;
  delete m_parent_E;
  delete m_parent_M;
  delete m_parent_charge;
  delete m_parent_pdgId;
  delete m_parent_status;
  delete m_parent_barcode;
  delete m_parent_prod_ID;
  delete m_parent_prod_x;
  delete m_parent_prod_y;
  delete m_parent_prod_z;
  delete m_parent_prod_r;
  delete m_parent_prod_eta;
  delete m_parent_prod_phi;
  delete m_parent_prod_barcode;
  delete m_outP_ID;
  delete m_outP_pt;
  delete m_outP_eta;
  delete m_outP_phi;
  delete m_outP_E;
  delete m_outP_M;
  delete m_outP_charge;
  delete m_outP_pdgId;
  delete m_outP_status;
  delete m_outP_barcode;
  delete m_outP_isReco;
  delete m_outP_recoProb;
  delete m_outP_recoID;
  delete m_outP_recoIsSelected;
  delete m_outP_recoIsAssociated;
  delete m_outP_isStable;
  delete m_outP_isInteracting;
  delete m_outP_isReconstructible;
  delete m_outP_isDark;

  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_recoVerts ) {
    delete m_isCloseToRecoVtx;
    delete m_closeRecoVtx_ID;
    delete m_closeRecoVtx_distance;
    delete m_isClosestToRecoVtx;
    delete m_closestRecoVtx_ID;
    delete m_closestRecoVtx_distance;
    delete m_isLinkedToRecoVtx;
    delete m_linkedRecoVtx_ID;
    delete m_linkedRecoVtx_score;
    delete m_isMaxlinkedToRecoVtx;
    delete m_maxlinkedRecoVtx_ID;
    delete m_maxlinkedRecoVtx_score;
    delete m_isLinkedParentToRecoVtx;
    delete m_linkedParentRecoVtx_ID;
    delete m_linkedParentRecoVtx_score;
    delete m_isMaxlinkedParentToRecoVtx;
    delete m_maxlinkedParentRecoVtx_ID;
    delete m_maxlinkedParentRecoVtx_score;
  }
  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_jets ) {
    delete m_truthJetMatch;
    delete m_truthJetMatch_ID;
    delete m_truthJetMatch_dR;
    delete m_darkJetMatch;
    delete m_darkJetMatch_ID;
    delete m_darkJetMatch_dR;
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


void TruthVertexContainer :: setTree ( TTree* tree )
{
  if ( m_debug ) Info( "EJs::TruthVertexContainer::setTree()", "setting tree" );
  
  VertexContainer::setTree ( tree );

  connectBranch<uint8_t>              ( tree, "isLLPDecay",        &m_isLLPDecay        );
  connectBranch<int>                  ( tree, "ID",                     &m_ID                     );
  connectBranch<float>                ( tree, "x",                      &m_x                      );
  connectBranch<float>                ( tree, "y",                      &m_y                      );
  connectBranch<float>                ( tree, "z",                      &m_z                      );
  connectBranch<float>                ( tree, "r",                      &m_r                      );
  connectBranch<float>                ( tree, "pt",                     &m_pt                     );
  connectBranch<float>                ( tree, "eta",                    &m_eta                    );
  connectBranch<float>                ( tree, "phi",                    &m_phi                    );
  connectBranch<float>                ( tree, "mass",                   &m_mass                   );
  connectBranch<int>                  ( tree, "nOutP",                  &m_nOutP                  );
  connectBranch<int>                  ( tree, "barcode",                &m_barcode                );
  connectBranch<int>                  ( tree, "parent_ID",              &m_parent_ID              );
  connectBranch<float>                ( tree, "parent_pt",              &m_parent_pt              );
  connectBranch<float>                ( tree, "parent_eta",             &m_parent_eta             );
  connectBranch<float>                ( tree, "parent_phi",             &m_parent_phi             );
  connectBranch<float>                ( tree, "parent_E",               &m_parent_E               );
  connectBranch<float>                ( tree, "parent_M",               &m_parent_M               );
  connectBranch<float>                ( tree, "parent_charge",          &m_parent_charge          );
  connectBranch<int>                  ( tree, "parent_pdgId",           &m_parent_pdgId           );
  connectBranch<int>                  ( tree, "parent_status",          &m_parent_status          );
  connectBranch<int>                  ( tree, "parent_barcode",         &m_parent_barcode         );
  connectBranch<int>                  ( tree, "parent_prod_ID",         &m_parent_prod_ID         );
  connectBranch<float>                ( tree, "parent_prod_x",          &m_parent_prod_x          );
  connectBranch<float>                ( tree, "parent_prod_y",          &m_parent_prod_y          );
  connectBranch<float>                ( tree, "parent_prod_z",          &m_parent_prod_z          );
  connectBranch<float>                ( tree, "parent_prod_r",          &m_parent_prod_r          );
  connectBranch<float>                ( tree, "parent_prod_eta",        &m_parent_prod_eta        );
  connectBranch<float>                ( tree, "parent_prod_phi",        &m_parent_prod_phi        );
  connectBranch<int>                  ( tree, "parent_prod_barcode",    &m_parent_prod_barcode    );
  connectBranch<std::vector<int>>     ( tree, "outP_ID",                &m_outP_ID                );
  connectBranch<std::vector<float>>   ( tree, "outP_pt",                &m_outP_pt                );
  connectBranch<std::vector<float>>   ( tree, "outP_eta",               &m_outP_eta               );
  connectBranch<std::vector<float>>   ( tree, "outP_phi",               &m_outP_phi               );
  connectBranch<std::vector<float>>   ( tree, "outP_E",                 &m_outP_E                 );
  connectBranch<std::vector<float>>   ( tree, "outP_M",                 &m_outP_M                 );
  connectBranch<std::vector<float>>   ( tree, "outP_charge",            &m_outP_charge            );
  connectBranch<std::vector<int>>     ( tree, "outP_pdgId",             &m_outP_pdgId             );
  connectBranch<std::vector<int>>     ( tree, "outP_status",            &m_outP_status            );
  connectBranch<std::vector<int>>     ( tree, "outP_barcode",           &m_outP_barcode           );
  connectBranch<std::vector<uint8_t>> ( tree, "outP_isReco",            &m_outP_isReco            );
  connectBranch<std::vector<float>>   ( tree, "outP_recoProb",          &m_outP_recoProb          );
  connectBranch<std::vector<int>>     ( tree, "outP_recoID",            &m_outP_recoID            );
  connectBranch<std::vector<uint8_t>> ( tree, "outP_recoIsSelected",    &m_outP_recoIsSelected    );
  connectBranch<std::vector<uint8_t>> ( tree, "outP_recoIsAssociated",  &m_outP_recoIsAssociated  );
  connectBranch<std::vector<uint8_t>> ( tree, "outP_isStable",          &m_outP_isStable          );
  connectBranch<std::vector<uint8_t>> ( tree, "outP_isInteracting",     &m_outP_isInteracting     );
  connectBranch<std::vector<uint8_t>> ( tree, "outP_isReconstructible", &m_outP_isReconstructible );
  connectBranch<std::vector<uint8_t>> ( tree, "outP_isDark",            &m_outP_isDark            );

  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_recoVerts ) {
    connectBranch<uint8_t>            ( tree, "isCloseToRecoVtx",             &m_isCloseToRecoVtx             );
    connectBranch<std::vector<int>>   ( tree, "closeRecoVtx_ID",              &m_closeRecoVtx_ID              );
    connectBranch<std::vector<float>> ( tree, "closeRecoVtx_distance",        &m_closeRecoVtx_distance        );
    connectBranch<uint8_t>            ( tree, "isClosestToRecoVtx",           &m_isClosestToRecoVtx           );
    connectBranch<std::vector<int>>   ( tree, "closestRecoVtx_ID",            &m_closestRecoVtx_ID            );
    connectBranch<std::vector<float>> ( tree, "closestRecoVtx_distance",      &m_closestRecoVtx_distance      );
    connectBranch<uint8_t>            ( tree, "isLinkedToRecoVtx",            &m_isLinkedToRecoVtx            );
    connectBranch<std::vector<int>>   ( tree, "linkedRecoVtx_ID",             &m_linkedRecoVtx_ID             );
    connectBranch<std::vector<float>> ( tree, "linkedRecoVtx_score",          &m_linkedRecoVtx_score          );
    connectBranch<uint8_t>            ( tree, "isMaxlinkedToRecoVtx",         &m_isMaxlinkedToRecoVtx         );
    connectBranch<int>                ( tree, "maxlinkedRecoVtx_ID",          &m_maxlinkedRecoVtx_ID          );
    connectBranch<float>              ( tree, "maxlinkedRecoVtx_score",       &m_maxlinkedRecoVtx_score       );
    connectBranch<uint8_t>            ( tree, "isLinkedParentToRecoVtx",      &m_isLinkedParentToRecoVtx      );
    connectBranch<std::vector<int>>   ( tree, "linkedParentRecoVtx_ID",       &m_linkedParentRecoVtx_ID       );
    connectBranch<std::vector<float>> ( tree, "linkedParentRecoVtx_score",    &m_linkedParentRecoVtx_score    );
    connectBranch<uint8_t>            ( tree, "isMaxlinkedParentToRecoVtx",   &m_isMaxlinkedParentToRecoVtx   );
    connectBranch<std::vector<int>>   ( tree, "maxlinkedParentRecoVtx_ID",    &m_maxlinkedParentRecoVtx_ID    );
    connectBranch<std::vector<float>> ( tree, "maxlinkedParentRecoVtx_score", &m_maxlinkedParentRecoVtx_score );
  }
  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_jets ) {
    connectBranch<uint8_t>              ( tree, "isTruthJetMatched",  &m_truthJetMatch     );
    connectBranch<std::vector<int>>     ( tree, "truthJetMatch_ID",   &m_truthJetMatch_ID  );
    connectBranch<std::vector<float>>   ( tree, "truthJetMatch_dR",   &m_truthJetMatch_dR  );
    connectBranch<uint8_t>              ( tree, "isDarkJetMatched",   &m_darkJetMatch      );
    connectBranch<std::vector<int>>     ( tree, "darkJetMatch_ID",    &m_darkJetMatch_ID   );
    connectBranch<std::vector<float>>   ( tree, "darkJetMatch_dR",    &m_darkJetMatch_dR   );
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


void TruthVertexContainer :: setBranches ( TTree* tree )
{
  if ( m_debug ) Info( "EJs::TruthVertexContainer::setBranches()", "setting branches" );
  
  VertexContainer::setBranches ( tree );

  setBranch<uint8_t>              ( tree, "isLLPDecay",        m_isLLPDecay        );
  setBranch<int>                  ( tree, "ID",                     m_ID                     );
  setBranch<float>                ( tree, "x",                      m_x                      );
  setBranch<float>                ( tree, "y",                      m_y                      );
  setBranch<float>                ( tree, "z",                      m_z                      );
  setBranch<float>                ( tree, "r",                      m_r                      );
  setBranch<float>                ( tree, "pt",                     m_pt                     );
  setBranch<float>                ( tree, "eta",                    m_eta                    );
  setBranch<float>                ( tree, "phi",                    m_phi                    );
  setBranch<float>                ( tree, "mass",                   m_mass                   );
  setBranch<int>                  ( tree, "nOutP",                  m_nOutP                  );
  setBranch<int>                  ( tree, "barcode",                m_barcode                );
  setBranch<int>                  ( tree, "parent_ID",              m_parent_ID              );
  setBranch<float>                ( tree, "parent_pt",              m_parent_pt              );
  setBranch<float>                ( tree, "parent_eta",             m_parent_eta             );
  setBranch<float>                ( tree, "parent_phi",             m_parent_phi             );
  setBranch<float>                ( tree, "parent_E",               m_parent_E               );
  setBranch<float>                ( tree, "parent_M",               m_parent_M               );
  setBranch<float>                ( tree, "parent_charge",          m_parent_charge          );
  setBranch<int>                  ( tree, "parent_pdgId",           m_parent_pdgId           );
  setBranch<int>                  ( tree, "parent_status",          m_parent_status          );
  setBranch<int>                  ( tree, "parent_barcode",         m_parent_barcode         );
  setBranch<int>                  ( tree, "parent_prod_ID",         m_parent_prod_ID         );
  setBranch<float>                ( tree, "parent_prod_x",          m_parent_prod_x          );
  setBranch<float>                ( tree, "parent_prod_y",          m_parent_prod_y          );
  setBranch<float>                ( tree, "parent_prod_z",          m_parent_prod_z          );
  setBranch<float>                ( tree, "parent_prod_r",          m_parent_prod_r          );
  setBranch<float>                ( tree, "parent_prod_eta",        m_parent_prod_eta        );
  setBranch<float>                ( tree, "parent_prod_phi",        m_parent_prod_phi        );
  setBranch<int>                  ( tree, "parent_prod_barcode",    m_parent_prod_barcode    );
  setBranch<std::vector<int>>     ( tree, "outP_ID",                m_outP_ID                );
  setBranch<std::vector<float>>   ( tree, "outP_pt",                m_outP_pt                );
  setBranch<std::vector<float>>   ( tree, "outP_eta",               m_outP_eta               );
  setBranch<std::vector<float>>   ( tree, "outP_phi",               m_outP_phi               );
  setBranch<std::vector<float>>   ( tree, "outP_E",                 m_outP_E                 );
  setBranch<std::vector<float>>   ( tree, "outP_M",                 m_outP_M                 );
  setBranch<std::vector<float>>   ( tree, "outP_charge",            m_outP_charge            );
  setBranch<std::vector<int>>     ( tree, "outP_pdgId",             m_outP_pdgId             );
  setBranch<std::vector<int>>     ( tree, "outP_status",            m_outP_status            );
  setBranch<std::vector<int>>     ( tree, "outP_barcode",           m_outP_barcode           );
  setBranch<std::vector<uint8_t>> ( tree, "outP_isReco",            m_outP_isReco            );
  setBranch<std::vector<float>>   ( tree, "outP_recoProb",          m_outP_recoProb          );
  setBranch<std::vector<int>>     ( tree, "outP_recoID",            m_outP_recoID            );
  setBranch<std::vector<uint8_t>> ( tree, "outP_recoIsSelected",    m_outP_recoIsSelected    );
  setBranch<std::vector<uint8_t>> ( tree, "outP_recoIsAssociated",  m_outP_recoIsAssociated  );
  setBranch<std::vector<uint8_t>> ( tree, "outP_isStable",          m_outP_isStable          );
  setBranch<std::vector<uint8_t>> ( tree, "outP_isInteracting",     m_outP_isInteracting     );
  setBranch<std::vector<uint8_t>> ( tree, "outP_isReconstructible", m_outP_isReconstructible );
  setBranch<std::vector<uint8_t>> ( tree, "outP_isDark",            m_outP_isDark            );

  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_recoVerts ) {
    setBranch<std::vector<int>>   ( tree, "linkedRecoVtx_ID",             m_linkedRecoVtx_ID             );
    setBranch<std::vector<float>> ( tree, "linkedRecoVtx_score",          m_linkedRecoVtx_score          );
    setBranch<int>                ( tree, "maxlinkedRecoVtx_ID",          m_maxlinkedRecoVtx_ID          );
    setBranch<float>              ( tree, "maxlinkedRecoVtx_score",       m_maxlinkedRecoVtx_score       );
  }
  if ( m_infoSwitch.m_isMatched && m_infoSwitch.m_jets ) {
    setBranch<uint8_t>              ( tree, "isTruthJetMatched",  m_truthJetMatch     );
    setBranch<std::vector<int>>     ( tree, "truthJetMatch_ID",   m_truthJetMatch_ID  );
    setBranch<std::vector<float>>   ( tree, "truthJetMatch_dR",   m_truthJetMatch_dR  );
    setBranch<uint8_t>              ( tree, "isDarkJetMatched",   m_darkJetMatch      );
    setBranch<std::vector<int>>     ( tree, "darkJetMatch_ID",    m_darkJetMatch_ID   );
    setBranch<std::vector<float>>   ( tree, "darkJetMatch_dR",    m_darkJetMatch_dR   );
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


void TruthVertexContainer :: clear ()
{
  if ( m_debug ) Info( "EJs::TruthVertexContainer::clear()", "clearing branches" );
  
  VertexContainer::clear ();

  m_isLLPDecay        ->clear();
  m_ID                     ->clear();
  m_x                      ->clear();
  m_y                      ->clear();
  m_z                      ->clear();
  m_r                      ->clear();
  m_pt                     ->clear();
  m_eta                    ->clear();
  m_phi                    ->clear();
  m_mass                   ->clear();
  m_nOutP                  ->clear();
  m_barcode                ->clear();
  m_parent_ID              ->clear();
  m_parent_pt              ->clear();
  m_parent_eta             ->clear();
  m_parent_phi             ->clear();
  m_parent_E               ->clear();
  m_parent_M               ->clear();
  m_parent_charge          ->clear();
  m_parent_pdgId           ->clear();
  m_parent_status          ->clear();
  m_parent_barcode         ->clear();
  m_parent_prod_ID         ->clear();
  m_parent_prod_x          ->clear();
  m_parent_prod_y          ->clear();
  m_parent_prod_z          ->clear();
  m_parent_prod_r          ->clear();
  m_parent_prod_eta        ->clear();
  m_parent_prod_phi        ->clear();
  m_parent_prod_barcode    ->clear();
  m_outP_ID                ->clear();
  m_outP_pt                ->clear();
  m_outP_eta               ->clear();
  m_outP_phi               ->clear();
  m_outP_E                 ->clear();
  m_outP_M                 ->clear();
  m_outP_charge            ->clear();
  m_outP_pdgId             ->clear();
  m_outP_status            ->clear();
  m_outP_barcode           ->clear();
  m_outP_isReco            ->clear();
  m_outP_recoProb          ->clear();
  m_outP_recoID            ->clear();
  m_outP_recoIsSelected    ->clear();
  m_outP_recoIsAssociated  ->clear();
  m_outP_isStable          ->clear();
  m_outP_isInteracting     ->clear();
  m_outP_isReconstructible ->clear();
  m_outP_isDark            ->clear();

  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_recoVerts ) {
    m_isCloseToRecoVtx             ->clear();
    m_closeRecoVtx_ID              ->clear();
    m_closeRecoVtx_distance        ->clear();
    m_isClosestToRecoVtx           ->clear();
    m_closestRecoVtx_ID            ->clear();
    m_closestRecoVtx_distance      ->clear();
    m_isLinkedToRecoVtx            ->clear();
    m_linkedRecoVtx_ID             ->clear();
    m_linkedRecoVtx_score          ->clear();
    m_isMaxlinkedToRecoVtx         ->clear();
    m_maxlinkedRecoVtx_ID          ->clear();
    m_maxlinkedRecoVtx_score       ->clear();
    m_isLinkedParentToRecoVtx      ->clear();
    m_linkedParentRecoVtx_ID       ->clear();
    m_linkedParentRecoVtx_score    ->clear();
    m_isMaxlinkedParentToRecoVtx   ->clear();
    m_maxlinkedParentRecoVtx_ID    ->clear();
    m_maxlinkedParentRecoVtx_score ->clear();
  }
  if ( m_infoSwitch.m_isMatched || m_infoSwitch.m_jets ) {
    m_truthJetMatch       ->clear();
    m_truthJetMatch_ID    ->clear();
    m_truthJetMatch_dR    ->clear();
    m_darkJetMatch        ->clear();
    m_darkJetMatch_ID     ->clear();
    m_darkJetMatch_dR     ->clear();
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


void TruthVertexContainer :: FillTruthVertex ( const xAOD::TruthVertex* truthVtx, const std::string treeName )
{
  if ( m_debug ) Info( "EJs::TruthVertexContainer::FillTruthVertex()", "filling branches" );
  
  VertexContainer::FillVertex ();

//  m_isLLPDecay        ->push_back( HelperFunctions::selectLLP        ( truthVtx ) );

  m_ID      ->push_back( AUXDYN( truthVtx, int, "ID" )  );
  m_x       ->push_back( truthVtx->x()                  );
  m_y       ->push_back( truthVtx->y()                  );
  m_z       ->push_back( truthVtx->z()                  );
  m_r       ->push_back( truthVtx->perp()               );
  m_eta     ->push_back( truthVtx->eta()                );
  m_phi     ->push_back( truthVtx->phi()                );
  m_nOutP   ->push_back( truthVtx->nOutgoingParticles() );
  m_barcode ->push_back( truthVtx->barcode()            );

  int   parent_ID      = AlgConsts::invalidInt;
  float parent_pt      = AlgConsts::invalidFloat;
  float parent_eta     = AlgConsts::invalidFloat;
  float parent_phi     = AlgConsts::invalidFloat;
  float parent_E       = AlgConsts::invalidFloat;
  float parent_M       = AlgConsts::invalidFloat;
  float parent_charge  = AlgConsts::invalidFloat;
  float parent_pdgId   = AlgConsts::invalidInt;
  int   parent_status  = AlgConsts::invalidInt;
  int   parent_barcode = AlgConsts::invalidInt;

  int   pProd_ID      = AlgConsts::invalidInt;
  float pProd_x       = AlgConsts::invalidFloat;
  float pProd_y       = AlgConsts::invalidFloat;
  float pProd_z       = AlgConsts::invalidFloat;
  float pProd_r       = AlgConsts::invalidFloat;
  float pProd_eta     = AlgConsts::invalidFloat;
  float pProd_phi     = AlgConsts::invalidFloat;
  int   pProd_barcode = AlgConsts::invalidInt;
  
  const auto* parent = truthVtx->incomingParticle(0);
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

    const auto* prodVtx = parent->prodVtx();
    if ( prodVtx ) {
      pProd_ID      = AUXDYN( prodVtx, int, "ID" );
      pProd_x       = prodVtx->x();
      pProd_y       = prodVtx->y();
      pProd_z       = prodVtx->z();
      pProd_r       = prodVtx->perp();
      pProd_eta     = prodVtx->eta();
      pProd_phi     = prodVtx->phi();
      pProd_barcode = prodVtx->barcode();
    }
  }

  m_parent_ID      ->push_back( parent_ID      );
  m_parent_pt      ->push_back( parent_pt      );
  m_parent_eta     ->push_back( parent_eta     );
  m_parent_phi     ->push_back( parent_phi     );
  m_parent_E       ->push_back( parent_E       );
  m_parent_M       ->push_back( parent_M       );
  m_parent_charge  ->push_back( parent_charge  );
  m_parent_pdgId   ->push_back( parent_pdgId   );
  m_parent_status  ->push_back( parent_status  );
  m_parent_barcode ->push_back( parent_barcode );

  m_parent_prod_ID      ->push_back( pProd_ID      );
  m_parent_prod_x       ->push_back( pProd_x       );
  m_parent_prod_y       ->push_back( pProd_y       );
  m_parent_prod_z       ->push_back( pProd_z       );
  m_parent_prod_r       ->push_back( pProd_r       );
  m_parent_prod_eta     ->push_back( pProd_eta     );
  m_parent_prod_phi     ->push_back( pProd_phi     );
  m_parent_prod_barcode ->push_back( pProd_barcode );

  std::vector<const xAOD::TruthParticle*> outParts;
  
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
  std::vector<uint8_t> outP_isDark;
  
  for ( size_t i = 0; i != truthVtx->nOutgoingParticles(); ++i ) {
    const auto* outP = truthVtx->outgoingParticle(i);
    if ( !outP )           continue;
    outParts               .push_back( outP                                            );
    outP_ID                .push_back( AUXDYN( outP, int, "ID"                       ) );
    outP_pt                .push_back( outP->pt() / m_units                            );
    outP_eta               .push_back( outP->eta()                                     );
    outP_phi               .push_back( outP->phi()                                     );
    outP_E                 .push_back( outP->e()  / m_units                            );
    outP_M                 .push_back( outP->m()  / m_units                            );
    outP_charge            .push_back( outP->charge()                                  );
    outP_pdgId             .push_back( outP->pdgId()                                   );
    outP_status            .push_back( outP->status()                                  );
    outP_barcode           .push_back( outP->barcode()                                 );
    outP_isReco            .push_back( AUXDYN( outP, char,   "isTrackMatch"          ) );
    outP_recoProb          .push_back( AUXDYN( outP, double, "trackMatchProbability" ) );
    outP_isStable          .push_back( HelperFunctions::isStable          ( outP           ) );
    outP_isInteracting     .push_back( HelperFunctions::isInteracting     ( outP           ) );
    outP_isReconstructible .push_back( HelperFunctions::isReconstructible ( outP           ) );
    outP_isDark            .push_back( HelperFunctions::isDark            ( outP           ) );
    // get linked track
    int     recoID      = AlgConsts::invalidInt;
    uint8_t recoIsSel   = AlgConsts::invalidUnsigned;
    uint8_t recoIsAssoc = AlgConsts::invalidUnsigned;
    static SG::AuxElement::ConstAccessor<HelperFunctions::TrackLink_t> recoAccess("trackLink");
    if ( recoAccess.isAvailable( *outP ) ) {
      try {
	const HelperFunctions::TrackLink_t& recoLink = recoAccess( *outP );
	recoID      = AUXDYN( (*recoLink), int,  "ID"            );
	recoIsSel   = AUXDYN( (*recoLink), char, "is_selected"   );
	recoIsAssoc = AUXDYN( (*recoLink), char, "is_associated" );
      } catch(...){}
    }
    outP_recoID           .push_back( recoID      );
    outP_recoIsSelected   .push_back( recoIsSel   );
    outP_recoIsAssociated .push_back( recoIsAssoc );
  }

//  const TLorentzVector& sumP4 = HelperFunctions::truthSumP4 ( outParts );
//  m_pt   ->push_back( sumP4.Pt() / m_units );
//  m_mass ->push_back( sumP4.M()  / m_units );

  m_outP_ID                ->push_back( outP_ID                );
  m_outP_pt                ->push_back( outP_pt                );
  m_outP_eta               ->push_back( outP_eta               );
  m_outP_phi               ->push_back( outP_phi               );
  m_outP_E                 ->push_back( outP_E                 );
  m_outP_M                 ->push_back( outP_M                 );
  m_outP_charge            ->push_back( outP_charge            );
  m_outP_pdgId             ->push_back( outP_pdgId             );
  m_outP_status            ->push_back( outP_status            );
  m_outP_barcode           ->push_back( outP_barcode           );
  m_outP_isReco            ->push_back( outP_isReco            );
  m_outP_recoProb          ->push_back( outP_recoProb          );
  m_outP_recoID            ->push_back( outP_recoID            );
  m_outP_recoIsSelected    ->push_back( outP_recoIsSelected    );
  m_outP_recoIsAssociated  ->push_back( outP_recoIsAssociated  );
  m_outP_isStable          ->push_back( outP_isStable          );
  m_outP_isInteracting     ->push_back( outP_isInteracting     );
  m_outP_isReconstructible ->push_back( outP_isReconstructible );
  m_outP_isDark            ->push_back( outP_isDark            );

  if ( m_infoSwitch.m_isMatched ) {
    std::vector<int> linkedReco_ID;
    
    int     linkedID       = AlgConsts::invalidInt;
    float   linkedScore    = AlgConsts::invalidFloat;
    // access reco vertex links truthVtx linked to
    static SG::AuxElement::ConstAccessor<HelperFunctions::VertexLinkVector_t> linkRecoAccess("linkedSecondaryVertexLinks");
    if ( linkRecoAccess.isAvailable( *truthVtx ) ) {
      const HelperFunctions::VertexLinkVector_t& vtxLinks = linkRecoAccess( *truthVtx );
      for ( const auto& link : vtxLinks ) {
	      if ( !link.isValid() ) continue;
	      linkedID = AUXDYN( (*link), int, "ID");
        linkedReco_ID .push_back( linkedID );
      }
    }
    
    const xAOD::Vertex* maxlinkedRecoVertex = 0;
    // access reco vertex links truthVtx max-linked to
    static SG::AuxElement::ConstAccessor<HelperFunctions::VertexLink_t> maxlinkRecoAccess("maxlinkedSecondaryVertexLink");
    if ( maxlinkRecoAccess.isAvailable( *truthVtx ) ) {
      try {
        const HelperFunctions::VertexLink_t& maxlinkedVtxLink = maxlinkRecoAccess( *truthVtx );
        maxlinkedRecoVertex = *maxlinkedVtxLink;
      } catch(...) {}
    }

    m_linkedRecoVtx_ID             ->push_back( linkedReco_ID                                                    );
    m_linkedRecoVtx_score          ->push_back( AUXDYNVEC( truthVtx, float, "linkedSecondaryVertexScores"      ) );

    if( maxlinkedRecoVertex ) {
      m_maxlinkedRecoVtx_ID          ->push_back( AUXDYN( maxlinkedRecoVertex, int, "ID"                         ) );
      m_maxlinkedRecoVtx_score       ->push_back( AUXDYN( truthVtx, float, "maxlinkedSecondaryVertexScore"       ) );
    }
    else {
      m_maxlinkedRecoVtx_ID          ->push_back( AlgConsts::invalidInt  );
      m_maxlinkedRecoVtx_score       ->push_back( 0.0                    );
    }

  }
  

  if ( m_infoSwitch.m_isMatched && m_infoSwitch.m_jets ) {
    std::string jet_str = treeName;
    if ( treeName == "nominal" ) jet_str = "";

    uint8_t isTruthJetMatch = false;
    uint8_t isDarkJetMatch  = false;
    if ( truthVtx->isAvailable<char>("isMatchedToTruthJet") ) {
      isTruthJetMatch = truthVtx->auxdataConst<char>("isMatchedToTruthJet");
      isDarkJetMatch  = truthVtx->auxdataConst<char>("isMatchedToDarkJet");
    }
      
    uint8_t isEMTopoJetMatch = false;
    std::string emtopo_str = "";
    if ( truthVtx->isAvailable<char>("isMatchedToRecoJet_EMTopo" + jet_str) ) {
      isEMTopoJetMatch = truthVtx->auxdataConst<char>("isMatchedToRecoJet_EMTopo" + jet_str);
      emtopo_str = jet_str;
    }
    else if ( truthVtx->isAvailable<char>("isMatchedToRecoJet_EMTopo") )
      isEMTopoJetMatch = truthVtx->auxdataConst<char>("isMatchedToRecoJet_EMTopo");
    
    uint8_t isPFlowJetMatch = false;
    std::string pflow_str = "";
    if ( truthVtx->isAvailable<char>("isMatchedToRecoJet_PFlow" + jet_str) ) {
      isPFlowJetMatch = truthVtx->auxdataConst<char>("isMatchedToRecoJet_PFlow" + jet_str);
      pflow_str = jet_str;
    }
    else if ( truthVtx->isAvailable<char>("isMatchedToRecoJet_PFlow") )
      isPFlowJetMatch = truthVtx->auxdataConst<char>("isMatchedToRecoJet_PFlow");

    m_truthJetMatch       ->push_back( isTruthJetMatch );
    m_truthJetMatch_ID    ->push_back( AUXDYNVEC( truthVtx, int,   "truthJetMatchIDs" ) );
    m_truthJetMatch_dR    ->push_back( AUXDYNVEC( truthVtx, float, "truthJetMatchDRs" ) );
    m_darkJetMatch        ->push_back( isDarkJetMatch  );
    m_darkJetMatch_ID     ->push_back( AUXDYNVEC( truthVtx, int,   "darkJetMatchIDs"  ) );
    m_darkJetMatch_dR     ->push_back( AUXDYNVEC( truthVtx, float, "darkJetMatchDRs"  ) );
    if ( m_doEMTopo ) {
      m_emtopoJetMatch    ->push_back( isEMTopoJetMatch );
      m_emtopoJetMatch_ID ->push_back( AUXDYNVEC( truthVtx, int,   "recoJetMatchIDs_EMTopo" + emtopo_str ) );
      m_emtopoJetMatch_dR ->push_back( AUXDYNVEC( truthVtx, float, "recoJetMatchDRs_EMTopo" + emtopo_str ) );
    }
    if ( m_doPFlow ) {
      m_pflowJetMatch     ->push_back( isPFlowJetMatch  );
      m_pflowJetMatch_ID  ->push_back( AUXDYNVEC( truthVtx, int,   "recoJetMatchIDs_PFlow"  + pflow_str  ) );
      m_pflowJetMatch_dR  ->push_back( AUXDYNVEC( truthVtx, float, "recoJetMatchDRs_PFlow"  + pflow_str  ) );
    }
  }
}
