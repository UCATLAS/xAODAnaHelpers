#include <xAODAnaHelpers/Jet.h>

using namespace xAH;

Jet::Jet() : matchedJet(0) 
{
}


void Jet::muonInJetCorrection(const xAH::MuonContainer* muons){

  TLorentzVector& jetVec = p4;
  float minDr            = 0.5;
  const Muon* minDr_muon  = 0;
  
  for(unsigned int iMuon = 0; iMuon < muons->size(); ++iMuon){
    const Muon* thisMuon = &(muons->at(iMuon));
    const TLorentzVector& muonVec = thisMuon->p4;
    
    if(muonVec.Pt()  < 4)                       continue;
    if(!thisMuon->isMedium && !thisMuon->isTight) continue;
    
    float thisDr = jetVec.DeltaR(muonVec);
    if(thisDr < minDr){
      minDr      = thisDr;
      minDr_muon = thisMuon;
    }
    
  }
  
  if(minDr < 0.4){
    matchedMuon = minDr_muon;
    
    const TLorentzVector& matchedMuonVec = minDr_muon->p4;
    TLorentzVector muon_elossVec  = minDr_muon->vec_eLoss();
    
    TLorentzVector JetNoMuon = (p4 - muon_elossVec);
    TLorentzVector newVec    = (JetNoMuon + matchedMuonVec);

    p4.SetPtEtaPhiE(newVec.Pt (),
		    newVec.Eta(),
		    newVec.Phi(),
		    newVec.E  ());
  }
  
  return;
}


