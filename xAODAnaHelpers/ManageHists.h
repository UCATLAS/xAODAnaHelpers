#ifndef xAODAnaHelpers_ManageHists_H
#define xAODAnaHelpers_ManageHists_H

#include <vector>
#include <iostream>
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include <EventLoop/Worker.h>
#include <EventLoop/StatusCode.h>

class ManageHists{

  protected:

    std::string m_name;
    int m_detailLevel;
    TString m_detailStr;
    std::vector< TH1* > m_allHists; //!
    // configuration variables

  public:

    ManageHists();
    ManageHists(std::string name, int detailLevel);
    ManageHists(std::string name, std::string detailStr);
    virtual ~ManageHists();
    void AddToOutput(EL::Worker* wk);

    TH1F* Book1D(TString cat, TString name, TString xaxis, Int_t xbins, Float_t xlow, Float_t xhigh);

    TH2F* Book2D(TString cat, TString name, TString xaxis, TString yaxis, Int_t xbins, Float_t xlow, Float_t xhigh, Int_t ybins, Float_t ylow, Float_t yhigh);

    TH3F* Book3D(TString cat, TString name, TString xaxis, TString yaxis, TString zaxis, Int_t xbins, Float_t xlow, Float_t xhigh, Int_t ybins, Float_t ylow, Float_t yhigh, Int_t zbins, Float_t zlow, Float_t zhigh);


};

#endif //xAODAnaHelpers_ManageHists_H
