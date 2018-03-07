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
    
    // identification
    std::map< std::string, int > isIdentified;

    std::map< std::string, std::vector< float > > TauIDEff_SF;
    std::map< std::string, std::vector< float > > EleOLRElectronEff_SF;
    std::map< std::string, std::vector< float > > TrigEff_SF;

    std::vector< float >  RecoEff_SF;
    std::vector< float >  EleOLRHadTauEff_SF;
    
    // might need to delete these
    int               isJetBDTSigVeryLoose;
    int               isJetBDTSigLoose;
    int               isJetBDTSigMedium;
    int               isJetBDTSigTight;
    
    int               isJetBDTBkgLoose;
    int               isJetBDTBkgMedium;
    int               isJetBDTBkgTight;
   

    float             JetBDTScore;
    float             JetBDTScoreSigTrans;
  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
