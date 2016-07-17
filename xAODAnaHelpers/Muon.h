#ifndef xAODAnaHelpers_Muon_H
#define xAODAnaHelpers_Muon_H

#include "xAODAnaHelpers/Particle.h"


namespace xAH {
  
  class Muon : public Particle
  {
  public:

    //Muon() {};
    //virtual ~Muon() { };

    // trigger
    int               isTrigMatched;
    std::vector<int>  isTrigMatchedToChain;
    std::string       listTrigChains;
    
      // isolation
    int   isIsolated_LooseTrackOnly;
    int   isIsolated_Loose;
    int   isIsolated_Tight;
    int   isIsolated_Gradient;
    int   isIsolated_GradientLoose;
    int   isIsolated_FixedCutLoose;
    int   isIsolated_FixedCutTightTrackOnly;
    int   isIsolated_UserDefinedFixEfficiency;
    int   isIsolated_UserDefinedCut;
    float ptcone20;
    float ptcone30;
    float ptcone40;
    float ptvarcone20;
    float ptvarcone30;
    float ptvarcone40;
    float topoetcone20;
    float topoetcone30;
    float topoetcone40;
    
    // quality
    int   isVeryLoose;
    int   isLoose;
    int   isMedium;
    int   isTight;
    
    // scale factors w/ sys
    // per object
    std::vector< float >  RecoEff_SF_Loose;
    std::vector< float >  TrigEff_SF_Loose_Loose;
    std::vector< float >  TrigEff_SF_Loose_FixedCutTightTrackOnly;
    std::vector< float >  TrigMCEff_Loose_Loose;
    std::vector< float >  TrigMCEff_Loose_FixedCutTightTrackOnly;
    std::vector< float >  IsoEff_SF_LooseTrackOnly;
    std::vector< float >  IsoEff_SF_Loose;
    std::vector< float >  IsoEff_SF_Tight;
    std::vector< float >  IsoEff_SF_Gradient;
    std::vector< float >  IsoEff_SF_GradientLoose;
    std::vector< float >  IsoEff_SF_FixedCutLoose;
    std::vector< float >  IsoEff_SF_FixedCutTightTrackOnly;
    std::vector< float >  TTVAEff_SF;
    
    // track parameters
    float trkd0;
    float trkd0sig;
    float trkz0;
    float trkz0sintheta;
    float trkphi0;
    float trktheta;
    float trkcharge;
    float trkqOverP;
    
      // track hit content
    int   trknSiHits;
    int   trknPixHits;
    int   trknPixHoles;
    int   trknSCTHits;
    int   trknSCTHoles;
    int   trknTRTHits;
    int   trknTRTHoles;
    int   trknBLayerHits;
    int   trknInnermostPixLayHits; // not available in DC14
    float trkPixdEdX;            // not available in DC14
    
    float         EnergyLoss;
    float         EnergyLossSigma;
    unsigned char energyLossType;
    float         MeasEnergyLoss;
    float         MeasEnergyLossSigma;
    float         ParamEnergyLoss;
    float         ParamEnergyLossSigmaMinus;
    float         ParamEnergyLossSigmaPlus;


    TLorentzVector vec_eLoss() const{

      float theta_muon = p4.Theta();
      float phi_muon   = p4.Phi();
      float elossX     = EnergyLoss * sin(theta_muon) * cos(phi_muon);
      float elossY     = EnergyLoss * sin(theta_muon) * sin(phi_muon);
      float elossZ     = EnergyLoss * cos(theta_muon);

      TLorentzVector muonELoss(elossX,elossY,elossZ,EnergyLoss);

      return muonELoss;
    }


  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
