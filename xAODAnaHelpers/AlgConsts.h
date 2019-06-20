#ifndef xAODAnaHelpers_AlgConsts_H
#define xAODAnaHelpers_AlgConsts_H

/* General constants 
   --> taken from SUSY FactoryTools / SecVtxPUBNote */

#include <limits>

namespace PhysConsts {
  constexpr double mass_chargedPion = 139.57018;
  constexpr double mass_electron    = 0.511;
  constexpr double mass_proton      = 938.27205;
}

namespace AlgConsts {
  constexpr double   infinitesimal            = 1.e-9;
  constexpr double   maxValue                 = 1.e10;
  constexpr double   chi2PerTrackInitValue    = 1.e5;
  constexpr double   minVertexChi2Probability = 1.e-3;
  constexpr double   invalidFloat             = std::numeric_limits<double>   :: quiet_NaN();
  constexpr int      invalidInt               = std::numeric_limits<int>      :: quiet_NaN();
  constexpr unsigned invalidUnsigned          = std::numeric_limits<unsigned> :: quiet_NaN();
}

#endif // xAODAnaHelpers_AlgConsts_H
