#ifndef xAODAnaHelpers_MetHists_H
#define xAODAnaHelpers_MetHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODMissingET/MissingETContainer.h"

class MetHists : public HistogramManager
{
  public:


    MetHists(std::string name, std::string detailStr);
    virtual ~MetHists() ;

    bool m_debug;
    StatusCode initialize();
    StatusCode execute( const xAOD::MissingETContainer* met, float eventWeight );

    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  protected:

    // holds bools that control which histograms are filled
    HelperClasses::METInfoSwitch* m_infoSwitch;

  private:

    TH1F*   m_metFinalClus      ;
    TH1F*   m_metFinalClusPx    ;
    TH1F*   m_metFinalClusPy    ;
    TH1F*   m_metFinalClusSumEt ;
    TH1F*   m_metFinalClusPhi   ;

    TH1F*   m_metFinalTrk       ;
    TH1F*   m_metFinalTrkPx     ;
    TH1F*   m_metFinalTrkPy     ;
    TH1F*   m_metFinalTrkSumEt  ;
    TH1F*   m_metFinalTrkPhi    ;

};

#endif
