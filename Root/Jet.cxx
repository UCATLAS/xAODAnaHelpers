#include <xAODAnaHelpers/Jet.h>

using namespace xAH;

int Jet::is_btag(BTaggerOP op) const
{
  switch(op)
    {
    case Jet::BTaggerOP::DL1r_FixedCutBEff_60:
      return is_DL1r_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::DL1r_FixedCutBEff_70:
      return is_DL1r_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::DL1r_FixedCutBEff_77:
      return is_DL1r_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::DL1r_FixedCutBEff_85:
      return is_DL1r_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::DL1r_Continuous:
      return is_DL1r_Continuous;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_60:
      return is_DL1dv00_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_70:
      return is_DL1dv00_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_77:
      return is_DL1dv00_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_85:
      return is_DL1dv00_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::DL1dv00_Continuous:
      return is_DL1dv00_Continuous;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_60:
      return is_DL1dv01_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_70:
      return is_DL1dv01_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_77:
      return is_DL1dv01_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_85:
      return is_DL1dv01_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::DL1dv01_Continuous:
      return is_DL1dv01_Continuous;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_60:
      return is_GN120220509_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_70:
      return is_GN120220509_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_77:
      return is_GN120220509_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_85:
      return is_GN120220509_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN120220509_Continuous:
      return is_GN120220509_Continuous;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_60:
      return is_GN2v00LegacyWP_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_70:
      return is_GN2v00LegacyWP_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_77:
      return is_GN2v00LegacyWP_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_85:
      return is_GN2v00LegacyWP_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_60:
      return is_GN2v00NewAliasWP_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_70:
      return is_GN2v00NewAliasWP_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_77:
      return is_GN2v00NewAliasWP_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_85:
      return is_GN2v00NewAliasWP_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_65:
      return is_GN2v01_FixedCutBEff_65;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_70:
      return is_GN2v01_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_77:
      return is_GN2v01_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_85:
      return is_GN2v01_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_90:
      return is_GN2v01_FixedCutBEff_90;
      break;
    case Jet::BTaggerOP::GN2v01_Continuous:
      return is_GN2v01_Continuous;
      break;
    default:
      return 0;
      break;
    }
}

const std::vector<float>& Jet::SF_btag(BTaggerOP op) const
{
  switch(op)
    {
    case Jet::BTaggerOP::DL1r_FixedCutBEff_60:
      return SF_DL1r_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::DL1r_FixedCutBEff_70:
      return SF_DL1r_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::DL1r_FixedCutBEff_77:
      return SF_DL1r_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::DL1r_FixedCutBEff_85:
      return SF_DL1r_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::DL1r_Continuous:
      return SF_DL1r_Continuous;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_60:
      return SF_DL1dv00_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_70:
      return SF_DL1dv00_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_77:
      return SF_DL1dv00_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::DL1dv00_FixedCutBEff_85:
      return SF_DL1dv00_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::DL1dv00_Continuous:
      return SF_DL1dv00_Continuous;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_60:
      return SF_DL1dv01_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_70:
      return SF_DL1dv01_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_77:
      return SF_DL1dv01_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::DL1dv01_FixedCutBEff_85:
      return SF_DL1dv01_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::DL1dv01_Continuous:
      return SF_DL1dv01_Continuous;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_60:
      return SF_GN120220509_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_70:
      return SF_GN120220509_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_77:
      return SF_GN120220509_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN120220509_FixedCutBEff_85:
      return SF_GN120220509_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN120220509_Continuous:
      return SF_GN120220509_Continuous;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_60:
      return SF_GN2v00LegacyWP_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_70:
      return SF_GN2v00LegacyWP_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_77:
      return SF_GN2v00LegacyWP_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN2v00LegacyWP_FixedCutBEff_85:
      return SF_GN2v00LegacyWP_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_60:
      return SF_GN2v00NewAliasWP_FixedCutBEff_60;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_70:
      return SF_GN2v00NewAliasWP_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_77:
      return SF_GN2v00NewAliasWP_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN2v00NewAliasWP_FixedCutBEff_85:
      return SF_GN2v00NewAliasWP_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_65:
      return SF_GN2v01_FixedCutBEff_65;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_70:
      return SF_GN2v01_FixedCutBEff_70;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_77:
      return SF_GN2v01_FixedCutBEff_77;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_85:
      return SF_GN2v01_FixedCutBEff_85;
      break;
    case Jet::BTaggerOP::GN2v01_FixedCutBEff_90:
      return SF_GN2v01_FixedCutBEff_90;
      break;
    case Jet::BTaggerOP::GN2v01_Continuous:
      return SF_GN2v01_Continuous;
      break;
    default:
      static const std::vector<float> dummySF = {1.};
      return dummySF;
      break;
    }
}


void Jet::muonInJetCorrection(const xAH::MuonContainer* muons){

  TLorentzVector& jetVec = p4;
  float minDr            = 0.5;
  const Muon* minDr_muon  = 0;
  
  for(unsigned int iMuon = 0; iMuon < muons->size(); ++iMuon){
    const Muon* thisMuon = &(muons->at(iMuon));
    const TLorentzVector& muonVec = thisMuon->p4;
    
    if(muonVec.Pt()  < 4)                       continue;
    if(!(thisMuon->quality.find("Medium") != thisMuon->quality.end() && thisMuon->quality.at("Medium"))) continue;
    
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
