#ifndef xAODAnaHelpers_MuonHists_H
#define xAODAnaHelpers_MuonHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include <xAODMuon/MuonContainer.h>

class MuonHists : public HistogramManager
{
  public:


    MuonHists(std::string name, std::string detailStr);
    virtual ~MuonHists() ;

    bool m_debug;
    StatusCode initialize();
    StatusCode execute( const xAOD::MuonContainer* muons, float eventWeight);
    StatusCode execute( const xAOD::Muon* muon, float eventWeight);
    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  protected:

    // holds bools that control which histograms are filled
    HelperClasses::MuonInfoSwitch* m_infoSwitch;

  private:
    
    //basic
    TH1F* m_Pt;                  //!
    TH1F* m_Eta;                 //!
    TH1F* m_Phi;                 //!
    TH1F* m_M;                   //!
    TH1F* m_E;                   //!

    // kinematic
    TH1F* m_Px;                  //!
    TH1F* m_Py;                  //!
    TH1F* m_Pz;                  //!

    // Isolation
    TH1F* m_isIsolated_LooseTrackOnly              ; //!
    TH1F* m_isIsolated_Loose			   ; //!
    TH1F* m_isIsolated_Tight			   ; //!
    TH1F* m_isIsolated_Gradient			   ; //!
    TH1F* m_isIsolated_GradientLoose		   ; //!
    TH1F* m_isIsolated_GradientT1		   ; //!
    TH1F* m_isIsolated_GradientT2		   ; //!
    TH1F* m_isIsolated_MU0p06			   ; //!
    TH1F* m_isIsolated_FixedCutLoose		   ; //!
    TH1F* m_isIsolated_FixedCutTight		   ; //!
    TH1F* m_isIsolated_FixedCutTightTrackOnly	   ; //!
    TH1F* m_isIsolated_UserDefinedFixEfficiency	   ; //!
    TH1F* m_isIsolated_UserDefinedCut		   ; //!
    
    TH1F* m_ptcone20				   ; //!
    TH1F* m_ptcone30				   ; //!
    TH1F* m_ptcone40				   ; //!
    TH1F* m_ptvarcone20				   ; //!
    TH1F* m_ptvarcone30				   ; //!
    TH1F* m_ptvarcone40				   ; //!
    TH1F* m_topoetcone20			   ; //!
    TH1F* m_topoetcone30			   ; //!
    TH1F* m_topoetcone40			   ; //!

    // quality
    TH1F* m_isVeryLoose				   ; //!
    TH1F* m_isLoose				   ; //!
    TH1F* m_isMedium				   ; //!
    TH1F* m_isTight                                ; //!
    
};

#endif
