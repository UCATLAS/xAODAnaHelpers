#ifndef xAODAnaHelpers_Particle_H
#define xAODAnaHelpers_Particle_H

#include <TLorentzVector.h>

namespace xAH {

  class Particle
  {

  public:

    //Particle() {};
    virtual ~Particle() {}

    TLorentzVector p4;
  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
