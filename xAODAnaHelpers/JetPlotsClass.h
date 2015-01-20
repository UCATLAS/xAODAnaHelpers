#ifndef xAODAnaHelpers_JetPlotsClass_H
#define xAODAnaHelpers_JetPlotsClass_H

#include "xAODAnaHelpers/ManageHists.h"

namespace xAOD {
#ifndef XAODJET_JETCONTAINER_H 
  class JetContainer;
#endif
#ifndef XAODJET_JET_H 
  class Jet;
#endif
}

class JetPlotsClass : public ManageHists
{

  private:
    
    // bools to control which histograms are filled
    bool m_fillKinematic;     //!
    bool m_fillClean;         //!
    bool m_fillEnergy;        //!
    
    bool m_fillResolution;    //!

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

  public:

    JetPlotsClass();
    JetPlotsClass(std::string name, int detailLevel);
    JetPlotsClass(std::string name, std::string detailStr);

    EL::StatusCode BookHistograms(EL::Worker* wk = 0);
    void FillHistograms( const xAOD::JetContainer* jets,  float eventWeight );
    void FillHistograms( const xAOD::Jet* jet,            float eventWeight );

};

#endif
