#ifndef xAODAnaHelpers_HistogramManager_H
#define xAODAnaHelpers_HistogramManager_H

#include <ctype.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <EventLoop/Worker.h>
#include <xAODRootAccess/TEvent.h>

// for StatusCode::isSuccess
#include "AsgTools/StatusCode.h"

class HistogramManager {

  protected:
    // generically the main name assigned to all histograms
    std::string m_name;
    // a detail level in the form of a string
    std::string m_detailStr;
    // a container holding all generated histograms
    //  - loop over this to record to EL output
    std::vector< TH1* > m_allHists; //!

  public:
    // initializer and destructor
    HistogramManager(std::string name, std::string detailStr);
    virtual ~HistogramManager();

    // this is used by any class extending to pre-define a set of histograms
    //      to book by default
    //  the following should be defined in other classes
    virtual StatusCode initialize(){      return StatusCode::SUCCESS; };
    virtual StatusCode execute(){         return StatusCode::SUCCESS; };
    virtual StatusCode finalize(){        return StatusCode::SUCCESS; };

    // @book - record a histogram and call various functions
    //      ** This is an overloaded function. It will build the right histogram
    //          given the correct number of input arguments.**
    //  @book.name - name of histogram, access it in ROOT file like "h_jetPt->Draw()"
    //  @book.title - usually pointless,put a description of the histogram in here
    //  @book.xlabel - label to put on the x-axis
    //  @book.xbins - number of xbins to use
    //  @book.xlow - lower bound on xbins
    //  @book.xhigh - upper bound on xbins
    //  @book.ylabel - label to put on the y-axis
    //  @book.ylow - lower bound on ybins
    //  @book.yhigh - upper bound on ybins
    //  @book.zlabel - label to put on the z-axix
    //  @book.zlow - lower bound on zbins
    //  @book.zhigh - upper bound on zbins
    //  @book.sumw2 - enable sumw2() for this histogram, default=True
    TH1F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh);

    TH2F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh,
               std::string xyabel, int ybins, double ylow, double yhigh);

    TH3F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh,
               std::string ylabel, int ybins, double ylow, double yhigh,
               std::string zlabel, int zbins, double zlow, double zhigh);


    //// Variable Binned Histograms ////
    TH1F* book(std::string name, std::string title,
               std::string xlabel, int xbins, const Double_t* xbinsArr);

    TH2F* book(std::string name, std::string title,
               std::string xlabel, int xbins, const Double_t* xbinsArr,
               std::string ylabel, int ybins, double ylow, double yhigh);

    TH2F* book(std::string name, std::string title,
               std::string xyabel, int xbins, double xlow, double xhigh,
               std::string ylabel, int ybins, const Double_t* ybinsArr);

    TH2F* book(std::string name, std::string title,
               std::string xyabel, int xbins, const Double_t* xbinsArr,
               std::string ylabel, int ybins, const Double_t* ybinsArr);

    TH3F* book(std::string name, std::string title,
               std::string xlabel, int xbins, const Double_t* xbinsArr,
               std::string ylabel, int ybins, const Double_t* ybinsArr,
               std::string zlabel, int zbins, const Double_t* zbinsArr);



    // Record all histograms from m_allHists to the worker
    void record(EL::Worker* wk);

  private:
    // Turn on Sumw2 for the histogram
    void Sumw2(TH1* hist, bool flag=true);

    // Push the new histogram to m_allHists
    void record(TH1* hist);

    // Set the xlabel
    void SetLabel(TH1* hist, std::string xlabel);
    // Set the xlabel, ylabel
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel);
    // Set the xlabel, ylabel, and zlabel
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel, std::string zlabel);

};

#endif
