#ifndef xAODAnaHelpers_EventInfo_H
#define xAODAnaHelpers_EventInfo_H

#include <TTree.h>
#include <string>

#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"

#include <xAODAnaHelpers/HelperClasses.h>

namespace xAH {

  class EventInfo
  {
  public:
    EventInfo(const std::string& detailStr="", float units = 1e3, bool mc = false, bool storeSyst = true);
    ~EventInfo();

    void setTree    (TTree *tree);
    void setBranches(TTree *tree);
    void clear();
    void FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event = nullptr, const xAOD::VertexContainer* vertices = nullptr);
    template <typename T_BR>
      void connectBranch(TTree *tree, std::string name, T_BR *variable);

  public:

    HelperClasses::EventInfoSwitch  m_infoSwitch;
    bool m_mc;
    bool m_debug;
    bool m_storeSyst;
    float m_units;

  public:

    int      m_runNumber;
    Long64_t m_eventNumber;
    int      m_lumiBlock;
    uint32_t m_coreFlags;
    uint32_t m_timeStamp;
    uint32_t m_timeStampNSOffset;
    bool     m_TileError;
    bool     m_LArError;
    bool     m_SCTError;
    uint32_t m_TileFlags;
    uint32_t m_LArFlags;
    uint32_t m_SCTFlags;
    bool     m_eventClean_LooseBad;
    int      m_mcEventNumber;
    int      m_mcChannelNumber;
    float    m_mcEventWeight;
    std::vector<float> m_mcEventWeights;
    float    m_weight_pileup;
    float    m_weight_pileup_up;
    float    m_weight_pileup_down;
    float    m_correctedAvgMu;
    float    m_correctedAndScaledAvgMu;
    float    m_correctedMu;
    float    m_correctedAndScaledMu;
    int      m_rand_run_nr;
    int      m_rand_lumiblock_nr;
    int      m_bcid;
    int      m_DistEmptyBCID;
    int      m_DistLastUnpairedBCID;
    int      m_DistNextUnpairedBCID;

    // event pileup
    int      m_npv;
    float    m_actualMu;
    float    m_averageMu;

    // event shapeEM
    double   m_rhoEM;
    double   m_rhoEMPFLOW;
    double   m_rhoLC;

    // truth
    int      m_pdgId1;
    int      m_pdgId2;
    int      m_pdfId1;
    int      m_pdfId2;
    float    m_x1;
    float    m_x2;
    //float m_scale;
    float    m_q;
    //float m_pdf1;
    //float m_pdf2;
    float    m_xf1;
    float    m_xf2;

    // CaloCluster
    std::vector<float> m_caloCluster_pt;
    std::vector<float> m_caloCluster_eta;
    std::vector<float> m_caloCluster_phi;
    std::vector<float> m_caloCluster_e;

  };

  template <typename T_BR> void EventInfo::connectBranch(TTree *tree, std::string name, T_BR *variable)
    {
      tree->SetBranchStatus  (name.c_str()  , 1);
      tree->SetBranchAddress (name.c_str()  , variable);
    }


}



#endif // xAODAnaHelpers_EventInfo_H
