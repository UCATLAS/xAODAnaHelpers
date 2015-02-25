#ifndef ${package}_${name}Hists_H
#define ${package}_${name}Hists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODJet/JetContainer.h>

class ${name}Hists : public HistogramManager
{
  public:
    ${name}Hists(std::string name, std::string detailStr);
    ~${name}Hists();

    EL::StatusCode initialize();
    EL::StatusCode execute( const xAOD::JetContainer* jets, float eventWeight );
    EL::StatusCode execute( const xAOD::Jet* jet, float eventWeight );
    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  protected:
    // bools to control which histograms are filled
    bool m_fillPosition;     //!

  private:
    //basic
    TH1F* m_jetPt;                  //!
    // position
    TH1F* m_jetEta;                 //!
    TH1F* m_jetPhi;                 //!
    TH1F* m_jetRapidity;            //!
};

#endif
