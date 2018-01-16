#ifndef xAODAnaHelpers_Tau_H
#define xAODAnaHelpers_Tau_H

#include "xAODAnaHelpers/Particle.h"


namespace xAH {
  
  class Tau : public Particle
  {
  public:

    // trigger
    int               isTrigMatched;
    std::vector<int>  isTrigMatchedToChain;
    std::string       listTrigChains;
    
    int               ntrk;
    float             charge;

    int               isJetBDTVeryLoose;
    int               isJetBDTLoose;
    int               isJetBDTMedium;
    int               isJetBDTTight;
  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
