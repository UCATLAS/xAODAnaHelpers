#ifndef xAODAnaHelpers_Tau_H
#define xAODAnaHelpers_Tau_H

#include "xAODAnaHelpers/Particle.h"


namespace xAH {
  
  class Tau : public Particle
  {
  public:

    int               ntrk;
    float             charge;

  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
