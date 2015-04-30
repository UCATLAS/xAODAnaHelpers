#ifndef xAODAnaHelpers_JetHists_H
#define xAODAnaHelpers_JetHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include <xAODJet/JetContainer.h>

class JetHists : public HistogramManager
{
  public:

    JetHists(std::string name, std::string detailStr);
    virtual ~JetHists() ;

    StatusCode initialize();
    StatusCode execute( const xAOD::JetContainer* jets, float eventWeight, int pvLoc = -1);
    StatusCode execute( const xAOD::Jet* jet, float eventWeight, int pvLoc = -1 );
    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  protected:

    // holds bools that control which histograms are filled
    HelperClasses::JetInfoSwitch* m_infoSwitch;

  private:
    //basic
    TH1F* m_jetPt;                  //!
    TH1F* m_jetEta;                 //!
    TH1F* m_jetPhi;                 //!
    TH1F* m_jetM;                   //!
    TH1F* m_jetE;                   //!
    TH1F* m_jetRapidity;            //!

    // kinematic
    TH1F* m_jetPx;                  //!
    TH1F* m_jetPy;                  //!
    TH1F* m_jetPz;                  //!

    //NLeadingJets
    std::vector< TH1F* > m_NjetsPt;       //!
    std::vector< TH1F* > m_NjetsEta;      //!
    std::vector< TH1F* > m_NjetsPhi;      //!
    std::vector< TH1F* > m_NjetsM;        //!
    std::vector< TH1F* > m_NjetsE;        //!
    std::vector< TH1F* > m_NjetsRapidity; //!

    // clean
    TH1F* m_jetTime;                //!
    TH1F* m_LArQuality;             //!
    TH1F* m_hecq;                   //!
    TH1F* m_negE;                   //!
    TH1F* m_avLArQF;                //!
    TH1F* m_bchCorrCell;            //!
    TH1F* m_N90Const;               //!

    // energy
    TH1F* m_HECf;                   //!
    TH1F* m_EMf;                    //!
    TH1F* m_centroidR;              //!

    //layer
    TH1F* m_PreSamplerB;
    TH1F* m_EMB1;
    TH1F* m_EMB2;
    TH1F* m_EMB3;
    TH1F* m_PreSamplerE;            //!
    TH1F* m_EME1;                   //!
    TH1F* m_EME2;                   //!
    TH1F* m_EME3;                   //!
    TH1F* m_HEC0;                   //!
    TH1F* m_HEC1;                   //!
    TH1F* m_HEC2;                   //!
    TH1F* m_HEC3;                   //!
    TH1F* m_TileBar0;               //!
    TH1F* m_TileBar1;               //!
    TH1F* m_TileBar2;               //!
    TH1F* m_TileGap1;               //!
    TH1F* m_TileGap2;               //!
    TH1F* m_TileGap3;               //!
    TH1F* m_TileExt0;               //!
    TH1F* m_TileExt1;               //!
    TH1F* m_TileExt2;               //!
    TH1F* m_FCAL0;                  //!
    TH1F* m_FCAL1;                  //!
    TH1F* m_FCAL2;                  //!

    // area
    TH1F* m_actArea;                //!


    TH1F* m_chf;                    //!

    //energy
    TH1F* m_HECf;                   //!
    TH1F* m_EMf;                    //!
    TH1F* m_centroidR;              //!
    TH1F* m_fracSampMax;            //!
    TH1F* m_fracSampMaxIdx;         //!
    TH1F* m_lowEtFrac;              //!

    // resolution
    TH1F* m_jetGhostTruthPt;        //!
    TH2F* m_jetPt_vs_resolution;    //!
    TH2F* m_jetGhostTruthPt_vs_resolution; //!

    // truth jets
    TH1F* m_truthLabelID;          //!
    TH1F* m_truthCount;            //!
    TH1F* m_truthPt;               //!
    TH1F* m_truthDr_B;             //!
    TH1F* m_truthDr_C;             //!
    TH1F* m_truthDr_T;             //!

    // Detailed truth jet plots
    TH1F* m_truthCount_BhadFinal;  //!
    TH1F* m_truthCount_BhadInit ;  //!
    TH1F* m_truthCount_BQFinal  ;  //!
    TH1F* m_truthPt_BhadFinal;  //!
    TH1F* m_truthPt_BhadInit ;  //!
    TH1F* m_truthPt_BQFinal  ;  //!

    TH1F* m_truthCount_ChadFinal;  //!
    TH1F* m_truthCount_ChadInit ;  //!
    TH1F* m_truthCount_CQFinal  ;  //!
    TH1F* m_truthPt_ChadFinal;  //!
    TH1F* m_truthPt_ChadInit ;  //!
    TH1F* m_truthPt_CQFinal  ;  //!


    TH1F* m_truthCount_TausFinal; //!
    TH1F* m_truthPt_TausFinal   ; //!

    // Flavor Tag
    TH1F* m_MV1   ; //!
    TH1F* m_SV1_plus_IP3D   ; //!
    TH1F* m_SV0             ; //!
    TH1F* m_SV1             ; //!
    TH1F* m_IP2D            ; //!
    TH1F* m_IP3D            ; //!
    TH1F* m_JetFitter       ; //!
    TH1F* m_JetFitterCombNN ; //!



};

#endif
