#ifndef xAODAnaHelpers_JetHists_H
#define xAODAnaHelpers_JetHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include <xAODJet/JetContainer.h>

class JetHists : public HistogramManager
{
  public:

    JetHists(std::string name, std::string detailStr);
    ~JetHists();

    EL::StatusCode initialize();
    EL::StatusCode execute( const xAOD::JetContainer* jets, float eventWeight );
    EL::StatusCode execute( const xAOD::Jet* jet, float eventWeight );
    EL::StatusCode execute( const xAOD::JetContainer& jets, const xAOD::EventInfo* eventInfo );
    //EL::StatusCode execute( const xAOD::JetContainer* jets, const xAOD::EventInfo* eventInfo );

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
    TH1F* m_jetN90Cell;             //!
    TH1F* m_N90Const;               //!
    TH1F* m_HECf;                   //!
    TH1F* m_EMf;                    //!
    TH1F* m_actArea;                //!
    TH1F* m_centroidR;              //!

    // clean
    TH1F* m_jetTime;                //!
    TH1F* m_LArQuality;             //!
    TH1F* m_hecq;                   //!
    TH1F* m_negE;                   //!
    TH1F* m_avLArQF;                //!
    TH1F* m_bchCorrCell;            //!
    TH1F* m_chf;                    //!

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


    // MJB plots
    TH2F* m_jet0Pt_jet1Pt;          //!
    TH2F* m_jet0Pt_avgBeta;         //!
    TH2F* m_jet0Pt_alpha;           //!
    TH2F* m_ptAsym_njet;            //!
    TH2F* m_jet0Pt_invPtBalCorr1;   //!
    TH2F* m_jet0Pt_invPtBalCorr2;   //!

    TH1F* m_ptBalCorr1;    //!
    TH1F* m_ptBalCorr2;    //!
    TH1F* m_jet0Pt;        //!
    TH1F* m_jet1Pt;        //!
    TH1F* m_jet2Pt;        //!
    TH1F* m_jet3Pt;        //!
    TH1F* m_jet4Pt;        //!
    TH1F* m_jet5Pt;        //!
    TH1F* m_jet6Pt;        //!
    TH1F* m_jet7Pt;        //!
    TH1F* m_jet8Pt;        //!
    TH1F* m_jet9Pt;        //!
    TH1F* m_correction;    //!



};

#endif
