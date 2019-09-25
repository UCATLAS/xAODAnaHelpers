/******************************************
 *
 * Base class used to book a set of histograms.
 * Many sets can be created and compared for
 * efficiecny studies or population studies.
 *
 * G. Facini
 * Sep 9 08:58:12 CEST 2014
 * G. Stark
 * Jan 20 10:29 CEST 2015
 *
 ******************************************/

#include <AsgTools/MsgStream.h>
#include "xAODAnaHelpers/HistogramManager.h"

/* constructors and destructors */
HistogramManager::HistogramManager(std::string name, std::string detailStr):
  m_name(name),
  m_detailStr(detailStr),
  m_msg(name)
{

  // if last character of name is a alphanumeric add a / so that
  // in the output file, a TDirectory is created with the histograms inside
  if( isalnum( m_name.back() ) && !ispunct( m_name.back() ) ) {
    m_name += "/";
    //Info("HistogramManager()", "Adding slash to put hists in TDirectories: %s",m_name.c_str());
  }

  m_msg.setLevel(MSG::INFO);
}

HistogramManager::~HistogramManager() {}

/* Main book() functions for 1D, 2D, 3D histograms */
TH1F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh)
{
  TH1F* tmp = new TH1F( (name + title).c_str(), title.c_str(), xbins, xlow, xhigh);
  SetLabel(tmp, xlabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH2F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             std::string ylabel, int ybins, double ylow, double yhigh)
{
  TH2F* tmp = new TH2F( (name + title).c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh);
  SetLabel(tmp, xlabel, ylabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH3F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             std::string ylabel, int ybins, double ylow, double yhigh,
                             std::string zlabel, int zbins, double zlow, double zhigh)
{
  TH3F* tmp = new TH3F( (name + title).c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh, zbins, zlow, zhigh);
  SetLabel(tmp, xlabel, ylabel, zlabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TProfile* HistogramManager::book(std::string name, std::string title,
				 std::string xlabel, int xbins, double xlow, double xhigh,
				 std::string ylabel, double ylow, double yhigh,
				 std::string option)
{
  TProfile* tmp = new TProfile( (name + title).c_str(), title.c_str(), xbins, xlow, xhigh, ylow, yhigh, option.c_str());
  SetLabel(tmp, xlabel, ylabel);
  //this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}


/////// Variable Binned Histograms ///////
TH1F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, const Double_t* xbinArr)
{
  TH1F* tmp = new TH1F( (name + title).c_str(), title.c_str(), xbins, xbinArr);
  SetLabel(tmp, xlabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH2F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, const Double_t* xbinArr,
                             std::string ylabel, int ybins, double ylow, double yhigh)
{
  TH2F* tmp = new TH2F( (name + title).c_str(), title.c_str(), xbins, xbinArr, ybins, ylow, yhigh);
  SetLabel(tmp, xlabel, ylabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH2F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             std::string ylabel, int ybins, const Double_t* ybinArr)
{
  TH2F* tmp = new TH2F( (name + title).c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ybinArr);
  SetLabel(tmp, xlabel, ylabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH2F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, const Double_t* xbinArr,
                             std::string ylabel, int ybins, const Double_t* ybinArr)
{
  TH2F* tmp = new TH2F( (name + title).c_str(), title.c_str(), xbins, xbinArr, ybins, ybinArr);
  SetLabel(tmp, xlabel, ylabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH3F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, const Double_t* xbinArr,
                             std::string ylabel, int ybins, const Double_t* ybinArr,
                             std::string zlabel, int zbins, const Double_t* zbinArr)
{
  TH3F* tmp = new TH3F( (name + title).c_str(), title.c_str(), xbins, xbinArr, ybins, ybinArr, zbins, zbinArr);
  SetLabel(tmp, xlabel, ylabel, zlabel);
  this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

MsgStream& HistogramManager :: msg () const { return m_msg; }
MsgStream& HistogramManager :: msg (int level) const {
    MsgStream& result = msg();
    result << MSG::Level (level);
    return result;
}

/* Helper functions */
void HistogramManager::Sumw2(TH1* hist, bool flag /*=true*/) {
  hist->Sumw2(flag);
}

void HistogramManager::record(TH1* hist) {
  m_allHists.push_back( hist );

  // Check if this histName already exists
  std::string histName = hist->GetName();
  HistMap_t::const_iterator it = m_histMap.find( histName );
  if ( it != m_histMap.end() ) // It does exist!
    {
      ANA_MSG_WARNING( "The histogram with name=" << histName << " already exists! "
                       << " NOT entering into the hist map, but prepare for unexpected behaviour" );
      return;
    }  
  m_histMap.insert( m_histMap.end(), std::pair< std::string, TH1* >( histName, hist ) );
}

void HistogramManager::record(EL::IWorker* wk) {
  for( auto hist : m_allHists ){
    wk->addOutput(hist);
  }
}

void HistogramManager::SetLabel(TH1* hist, std::string xlabel)
{
  hist->GetXaxis()->SetTitle(xlabel.c_str());
}

void HistogramManager::SetLabel(TH1* hist, std::string xlabel, std::string ylabel)
{
  hist->GetYaxis()->SetTitle(ylabel.c_str());
  this->SetLabel(hist, xlabel);
}

void HistogramManager::SetLabel(TH1* hist, std::string xlabel, std::string ylabel, std::string zlabel)
{
  hist->GetZaxis()->SetTitle(zlabel.c_str());
  this->SetLabel(hist, xlabel, ylabel);
}

TH1* HistogramManager::findHist(const std::string& histName) {
  // See if this entry exists in the map
  HistMap_t::const_iterator it = m_histMap.find( histName );
  if ( it == m_histMap.end() ) {
    ANA_MSG_ERROR("Histogram name " << histName << " not found");
    return NULL;
  }
  else {
    return it->second;
  }
}

void HistogramManager::fillHist(const std::string& histName, double value) {
  TH1* histPointer(NULL);
  histPointer = this->findHist(histName);
  histPointer->Fill(value);
}

void HistogramManager::fillHist(const std::string& histName, double value, double weight) {
  TH1* histPointer(NULL);
  histPointer = this->findHist(histName);
  histPointer->Fill(value, weight);
}


