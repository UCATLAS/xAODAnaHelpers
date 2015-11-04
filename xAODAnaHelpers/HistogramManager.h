#ifndef xAODAnaHelpers_HistogramManager_H
#define xAODAnaHelpers_HistogramManager_H

/** @file HistogramManager.h
 *  @brief Manage your histograms
 *  This is used by any class extending to pre-define a set of histograms to book by default.

 *  @author Giordon Stark
 *  @bug No known bugs
 */

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
    /** @brief generically the main name assigned to all histograms */
    std::string m_name;
    /** @brief a detail level in the form of a string */
    std::string m_detailStr;
    /** @brief a container holding all generated histograms
     *  Loop over this to record to EL output */
    std::vector< TH1* > m_allHists; //!

  public:
    HistogramManager(std::string name, std::string detailStr);
    virtual ~HistogramManager();

    // the following should be defined in other classes
    virtual StatusCode initialize(){      return StatusCode::SUCCESS; };
    virtual StatusCode execute(){         return StatusCode::SUCCESS; };
    virtual StatusCode finalize(){        return StatusCode::SUCCESS; };

    /**
     * @brief record a histogram and call various functions
     * @note This is an overloaded function. It will build the right histogram
     * given the correct number of input arguments.
     *
     *  @param name     name of histogram, access it in ROOT file like "h_jetPt->Draw()"
     *  @param title    usually pointless,put a description of the histogram in here
     *  @param xlabel   label to put on the x-axis
     *  @param xbins    number of xbins to use
     *  @param xlow     lower bound on xbins
     *  @param xhigh    upper bound on xbins
     *  @param xbinsArr variable xbins
     *  @param ylabel   label to put on the y-axis
     *  @param ylow     lower bound on ybins
     *  @param yhigh    upper bound on ybins
     *  @param ybinsArr variable ybins
     *  @param zlabel   label to put on the z-axix
     *  @param zlow     lower bound on zbins
     *  @param zhigh    upper bound on zbins
     *  @param zbinsArr variable zbins
     */
    TH1F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh);

    /**
     * @overload
     */
    TH2F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh,
               std::string xyabel, int ybins, double ylow, double yhigh);

    /**
     * @overload
     */
    TH3F* book(std::string name, std::string title,
               std::string xlabel, int xbins, double xlow, double xhigh,
               std::string ylabel, int ybins, double ylow, double yhigh,
               std::string zlabel, int zbins, double zlow, double zhigh);

    /**
     * @overload
     */
    TH1F* book(std::string name, std::string title,
               std::string xlabel, int xbins, const Double_t* xbinsArr);

    /**
     * @overload
     */
    TH2F* book(std::string name, std::string title,
               std::string xlabel, int xbins, const Double_t* xbinsArr,
               std::string ylabel, int ybins, double ylow, double yhigh);

    /**
     * @overload
     */
    TH2F* book(std::string name, std::string title,
               std::string xyabel, int xbins, double xlow, double xhigh,
               std::string ylabel, int ybins, const Double_t* ybinsArr);

    /**
     * @overload
     */
    TH2F* book(std::string name, std::string title,
               std::string xyabel, int xbins, const Double_t* xbinsArr,
               std::string ylabel, int ybins, const Double_t* ybinsArr);

    /**
     * @overload
     */
    TH3F* book(std::string name, std::string title,
               std::string xlabel, int xbins, const Double_t* xbinsArr,
               std::string ylabel, int ybins, const Double_t* ybinsArr,
               std::string zlabel, int zbins, const Double_t* zbinsArr);

    /**
     * @brief record all histograms from HistogramManager#m_allHists to the worker
     */
    void record(EL::Worker* wk);

  private:
    /**
     * @brief Turn on Sumw2 for the histogram
     *
     * @param hist      The histogram to modify
     * @param flag      Pass in whether to turn on Sumw2 or not
     */
    void Sumw2(TH1* hist, bool flag=true);

    /**
     * @brief Push the new histogram to HistogramManager#m_allHists
     */
    void record(TH1* hist);

    /**
     * @brief Set the labels on a histogram
     *
     * @param hist      The histogram to set the labels on
     * @param xlabel    The xlabel to set
     * @param ylabel    The ylabel to set
     * @param zlabel    The zlabel to set
     */
    void SetLabel(TH1* hist, std::string xlabel);
    /**
     * @overload
     */
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel);
    /**
     * @overload
     */
    void SetLabel(TH1* hist, std::string xlabel, std::string ylabel, std::string zlabel);

};

#endif
