#ifndef xAODAnaHelpers_HistogramManager_H
#define xAODAnaHelpers_HistogramManager_H

/** @file HistogramManager.h
 *  @brief Manage your histograms
 *  @author See AUTHORS.md
 *  @bug No known bugs
 */

#include <ctype.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TProfile.h>
#include <EventLoop/IWorker.h>
#include <xAODRootAccess/TEvent.h>

// for StatusCode::isSuccess
#include <AsgTools/StatusCode.h>
#include <AsgTools/MessageCheck.h>

class MsgStream;

/**
    @brief This is used by any class extending to pre-define a set of histograms to book by default.
    @rst
        .. note:: The expectation is that the user does not directly use this class but rather inherits from it.

        We expect the user to create a new group of histograms, such as for jets::

            class JetHists : public HistogramManager
            {
              public:
                JetHists(std::string name, std::string detailStr);
                virtual ~JetHists() ;

                bool m_debug;
                StatusCode initialize();
                StatusCode execute( const xAOD::JetContainer* jets, float eventWeight, int pvLoc = -1);
                StatusCode execute( const xAOD::Jet* jet, float eventWeight, int pvLoc = -1 );
                using HistogramManager::book; // make other overloaded version of book() to show up in subclass
                using HistogramManager::execute; // overload
            };

        The above example is taken from our implementation in :cpp:class:`JetHists`.

    @endrst
 */
class HistogramManager {

  protected:
    /** @brief generically the main name assigned to all histograms */
    std::string m_name;
    /** @brief a detail level in the form of a string */
    std::string m_detailStr;
    /** @brief a container holding all generated histograms */
    std::vector< TH1* > m_allHists; //!
    /** @brief hold the MsgStream object */
    mutable MsgStream m_msg; //!

  public:
    /**
        @brief Initialization
        @param name             The top-level path in which all histograms are stored under (think of `TDirectory`)
        @param detailStr        Specify the various details of which to plot. For example, jets might want `"kinematic substructure"`.
    */
    HistogramManager(std::string name, std::string detailStr);
    /**
        @brief Destructor, allows the user to delete histograms that are not being recorded.
    */
    virtual ~HistogramManager();

    /**
        @brief Initialize and book all histograms.
        @rst
            .. note:: This should call the overloaded functions :cpp:func:`HistogramManager::book` to create the histograms so that the user can call `hists->record(wk())` to record all histograms to the EventLoop worker.

            Example implementation::

                StatusCode JetHists::initialize() {
                  m_jetPt          = book(m_name, "jetPt",  "jet p_{T} [GeV]", 120, 0, 3000.);
                  return StatusCode::SUCCESS;
                }

        @endrst
    */
    virtual StatusCode initialize(){      return StatusCode::SUCCESS; };
    /**
        @brief Execute by filling in the histograms.
        @rst
            Example implementation::

                StatusCode JetHists::execute( const xAOD::JetContainer* jets, float eventWeight ){
                  for(const auto& jet: *jets)
                    m_jetPt->Fill( jet->pt()/1.e3, eventWeight );
                  return StatusCode::SUCCESS;
                }

        @endrst
    */
    virtual StatusCode execute(){         return StatusCode::SUCCESS; };
    /**
        @brief Finalize anything that needs to be finalized.
        @rst
            .. warning:: This should rarely be used. There is not a good use case for this functionality but it needs to exist in the off-chance that a user comes along and needs it for their histogram class.

        @endrst
    */
    virtual StatusCode finalize(){        return StatusCode::SUCCESS; };

    /**
        @brief record a histogram and call various functions
        @note This is an overloaded function. It will build the right histogram given the correct number of input arguments.
         @param name     name of histogram, access it in ROOT file like `h_jetPt->Draw()`
         @param title    usually pointless,put a description of the histogram in here
         @param xlabel   label to put on the x-axis
         @param xbins    number of xbins to use
         @param xlow     lower bound on xbins
         @param xhigh    upper bound on xbins
         @param xbinsArr variable xbins, test math \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$
         @param ylabel   label to put on the y-axis
         @param ylow     lower bound on ybins
         @param yhigh    upper bound on ybins
         @param ybinsArr variable ybins
         @param zlabel   label to put on the z-axix
         @param zlow     lower bound on zbins
         @param zhigh    upper bound on zbins
         @param zbinsArr variable zbins
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
     * @overload
     */
    TProfile* book(std::string name, std::string title,
		   std::string xlabel, int xbins, double xlow, double xhigh,
		   std::string ylabel, double ylow, double yhigh,
		   std::string option = "");

    /**
     * @brief record all histograms from HistogramManager#m_allHists to the worker
     */
    void record(EL::IWorker* wk);

    /**
      * @brief the standard message stream for this algorithm
      */
    MsgStream& msg () const;
    /**
      * @brief allow ANA_MSG_XXXX macros to be used within algorithms for a given level
      */
    MsgStream& msg (int level) const;

    /**
     * @brief Typedef for convenience
     */
    typedef std::unordered_map< std::string, TH1* > HistMap_t;

    /**
     * @brief The map of histogram names to their pointers
     */
    HistMap_t m_histMap;

    /**
     * @brief Return the pointer to the histogram
     */
    TH1* findHist(const std::string& histName);

    /**
     * @brief Fill a histogram by name. Can be overloaded with weight.
     * 
     * @param histName   The name of the histogram to be filled
     * @param value      The value to fill the histogram with
     */
    void fillHist(const std::string& histName, double value);
    /**
     * @overload
     */
    void fillHist(const std::string& histName, double value, double weight);


  private:
    /**
     * @brief Turn on Sumw2 for the histogram
     *
     * @param hist      The histogram to modify
     * @param flag      Pass in whether to turn on Sumw2 or not
     */
    void Sumw2(TH1* hist, bool flag=true);

    /**
     * @brief Push the new histogram to HistogramManager#m_allHists and add its name to HistogramManager#m_histMap
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
