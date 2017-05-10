#ifndef xAODAnaHelpers_MetContainer_H
#define xAODAnaHelpers_MetContainer_H

#include <TTree.h>

#include <string>

#include "xAODMissingET/MissingETContainer.h"

#include <xAODAnaHelpers/HelperClasses.h>

namespace xAH {

  class MetContainer
  {
  public:
    MetContainer(const std::string& detailStr="", float units = 1e3);
    ~MetContainer();

    void setTree    (TTree *tree);
    void setBranches(TTree *tree);
    void clear();
    void FillMET( const xAOD::MissingETContainer* met);
    template <typename T_BR>
      void connectBranch(TTree *tree, std::string name, T_BR *variable);

  public:

    HelperClasses::METInfoSwitch  m_infoSwitch;
    bool m_debug;
    float m_units;

  public:

    // met
    float m_metFinalClus;
    float m_metFinalClusPx;
    float m_metFinalClusPy;
    float m_metFinalClusPhi;
    float m_metFinalClusSumEt;

    float m_metFinalTrk;
    float m_metFinalTrkPx;
    float m_metFinalTrkPy;
    float m_metFinalTrkPhi;
    float m_metFinalTrkSumEt;

    float m_metEle;       float m_metEleSumEt;      float m_metElePhi;
    float m_metGamma;     float m_metGammaSumEt;    float m_metGammaPhi;
    float m_metTau;       float m_metTauSumEt;      float m_metTauPhi;
    float m_metMuons;     float m_metMuonsSumEt;    float m_metMuonsPhi;
    float m_metJet;       float m_metJetSumEt;      float m_metJetPhi;
    float m_metJetTrk;    float m_metJetTrkSumEt;   float m_metJetTrkPhi;
    float m_metSoftClus;  float m_metSoftClusSumEt; float m_metSoftClusPhi;
    float m_metSoftTrk;   float m_metSoftTrkSumEt;  float m_metSoftTrkPhi;



  };

  template <typename T_BR> void MetContainer::connectBranch(TTree *tree, std::string name, T_BR *variable)
    {
      tree->SetBranchStatus  (name.c_str()  , 1);
      tree->SetBranchAddress (name.c_str()  , variable);
    }


}



#endif // xAODAnaHelpers_MetContainer_H
