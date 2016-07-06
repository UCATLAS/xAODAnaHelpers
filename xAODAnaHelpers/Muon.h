#ifndef xAODAnaHelpers_Muon_H
#define xAODAnaHelpers_Muon_H

#include "xAODAnaHelpers/Particle.h"


namespace xAH {
  
  class Muon : public Particle
  {
  public:

    //Muon() {};
    //virtual ~Muon() { };

    // clean
    int isMedium;
    int isTight;
    float EnergyLoss;

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
