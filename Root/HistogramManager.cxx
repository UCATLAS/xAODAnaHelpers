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


#include "xAODAnaHelpers/HistogramManager.h"

/* constructors and destructors */
HistogramManager::HistogramManager(std::string name, std::string detailStr, std::string delimiter):
  m_name(name),
  m_detailStr(detailStr),
  m_delimiter(delimiter)
{
}

HistogramManager::~HistogramManager() {}

/* Main book() functions for 1D, 2D, 3D histograms */
TH1F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             bool sumw2)
{
  TH1F* tmp = new TH1F( concat(name, title).c_str(), title.c_str(), xbins, xlow, xhigh);
  SetLabel(tmp, xlabel);
  if(sumw2) this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH2F* HistogramManager::book(std::string name, std::string title,
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             std::string ylabel, int ybins, double ylow, double yhigh,
                             bool sumw2)
{
  TH2F* tmp = new TH2F( concat(name, title).c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh);
  SetLabel(tmp, xlabel, ylabel);
  if(sumw2) this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

TH3F* HistogramManager::book(std::string name, std::string title, 
                             std::string xlabel, int xbins, double xlow, double xhigh,
                             std::string ylabel, int ybins, double ylow, double yhigh,
                             std::string zlabel, int zbins, double zlow, double zhigh,
                             bool sumw2)
{
  TH3F* tmp = new TH3F( concat(name, title).c_str(), title.c_str(), xbins, xlow, xhigh, ybins, ylow, yhigh, zbins, zlow, zhigh);
  SetLabel(tmp, xlabel, ylabel, zlabel);
  if(sumw2) this->Sumw2(tmp);
  this->record(tmp);
  return tmp;
}

/* Helper functions */
void HistogramManager::Sumw2(TH1* hist) {
  hist->Sumw2();
}

void HistogramManager::record(TH1* hist) {
  m_allHists.push_back( hist );
}

void HistogramManager::record(EL::Worker* wk) {
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

std::string HistogramManager::concat(std::string left, std::string right)
{
  return left+m_delimiter+right;
}
