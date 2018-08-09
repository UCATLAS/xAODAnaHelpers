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
    
    std::map< std::string, std::vector< float > > TauEff_SF;
    std::map< std::string, std::vector< float > > TauTrigEff_SF;
    
    // might need to delete these
    int               isJetBDTSigVeryLoose;
    int               isJetBDTSigLoose;
    int               isJetBDTSigMedium;
    int               isJetBDTSigTight;

    float             JetBDTScore;
    float             JetBDTScoreSigTrans;

    int               isEleBDTLoose;
    int               isEleBDTMedium;
    int               isEleBDTTight;
 
    float             EleBDTScore;

    int               passEleOLR;

  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
