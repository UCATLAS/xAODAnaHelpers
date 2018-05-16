#ifndef xAODAnaHelpers_Cluster_H
#define xAODAnaHelpers_Cluster_H

#include "xAODAnaHelpers/Particle.h"

namespace xAH {

  class Cluster : public Particle
    {
    public:

      Cluster();
      float rapidity;

      // clean
      float Timing;
      
    };

}//xAH
#endif // xAODAnaHelpers_Cluster_H
