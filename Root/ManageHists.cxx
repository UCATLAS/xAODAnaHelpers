/******************************************
 *
 * Base class used to book a set of histograms.  
 * Many sets can be created and compared for 
 * efficiecny studies or population studies.
 *
 * G. Facini
 * Sep 9 08:58:12 CEST 2014
 *
 ******************************************/

#include "xAODAnaHelpers/ManageHists.h"

ManageHists::ManageHists() {}
ManageHists::ManageHists(std::string name, int detailLevel) :
  m_name(name),
  m_detailLevel(detailLevel)
{
}

ManageHists::ManageHists(std::string name, std::string detailStr) :
  m_name(name),
  m_detailStr(detailStr)
{
}

ManageHists::~ManageHists() {}

void ManageHists::AddToOutput(EL::Worker* wk) {
  for( auto hist : m_allHists ) {
    wk->addOutput (hist);
  }
}

TH1F* ManageHists::Book1D(TString cat, TString name, TString xaxis, Int_t xbins, Float_t xlow, Float_t xhigh) {
  TH1F* tmp = new TH1F(TString(cat+"__"+name),name,xbins,xlow,xhigh);
  tmp->GetXaxis()->SetTitle(xaxis);
  tmp->Sumw2();
  m_allHists.push_back( tmp );
  return tmp;
}; // Book1D


TH2F* ManageHists::Book2D(TString cat, TString name, TString xaxis, TString yaxis, Int_t xbins, Float_t xlow, Float_t xhigh, Int_t ybins, Float_t ylow, Float_t yhigh) {
  TH2F* tmp = new TH2F(TString(cat+"__"+name),name,xbins,xlow,xhigh,ybins,ylow,yhigh);
  tmp->GetXaxis()->SetTitle(xaxis);
  tmp->GetYaxis()->SetTitle(yaxis);
  tmp->Sumw2();
  m_allHists.push_back( tmp );
  return tmp;
}; // Book2D

TH3F* ManageHists::Book3D(TString cat, TString name, TString xaxis, TString yaxis, TString zaxis, Int_t xbins, Float_t xlow, Float_t xhigh, Int_t ybins, Float_t ylow, Float_t yhigh, Int_t zbins, Float_t zlow, Float_t zhigh) {
  TH3F* tmp = new TH3F(TString(cat+"__"+name),name,xbins,xlow,xhigh,ybins,ylow,yhigh,zbins,zlow,zhigh);
  tmp->GetXaxis()->SetTitle(xaxis);
  tmp->GetYaxis()->SetTitle(yaxis);
  tmp->GetZaxis()->SetTitle(zaxis);
  tmp->Sumw2();
  m_allHists.push_back( tmp );
  return tmp;
}; // Book3D
