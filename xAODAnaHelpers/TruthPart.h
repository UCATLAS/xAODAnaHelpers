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
      int barcode;

      // type
      bool is_higgs;
      bool is_bhad;

      // BVtx
      float Bdecay_x;
      float Bdecay_y;
      float Bdecay_z;
      
      // Parents
      int nParents;
      std::vector<int> parent_pdgId;
      std::vector<int> parent_barcode;
      std::vector<int> parent_status;

      // Children
      int nChildren;
      std::vector<int> child_pdgId;
      std::vector<int> child_barcode;
      std::vector<int> child_status;

      // Dressed
      float pt_dressed;
      float eta_dressed;
      float phi_dressed;
      float e_dressed;

      // Origin
      unsigned int origin;

    };

}//xAH
#endif // xAODAnaHelpers_Particle_H
