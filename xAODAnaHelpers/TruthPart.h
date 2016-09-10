#ifndef xAODAnaHelpers_TruthPart_H
#define xAODAnaHelpers_TruthPart_H

#include "xAODAnaHelpers/Particle.h"

namespace xAH {

  class TruthPart : public Particle
    {
    public:
      
      //TruthPart() { };
      //virtual ~TruthPart();
      int pdgId;
      int status;

      // type
      bool is_higgs;
      bool is_bhad;

      // BVtx
      float Bdecay_x;
      float Bdecay_y;
      float Bdecay_z;
      
    };

}//xAH
#endif // xAODAnaHelpers_Particle_H
