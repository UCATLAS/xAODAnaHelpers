#ifndef xAODAnaHelpers_Tau_H
#define xAODAnaHelpers_Tau_H

#include "xAODAnaHelpers/Particle.h"


namespace xAH {
  
  class Tau : public Particle
  {
  public:

    // trigger
    int                 isTrigMatched;
    std::vector<int>    isTrigMatchedToChain;
    std::string         listTrigChains;
    
    int                 ntrk;
    float               charge;
    
    std::map< std::string, std::vector< float > > TauEff_SF;
    std::map< std::string, std::vector< float > > TauTrigEff_SF;
    
    // might need to delete these
    int                 isJetBDTSigVeryLoose;
    int                 isJetBDTSigLoose;
    int                 isJetBDTSigMedium;
    int                 isJetBDTSigTight;

    float               JetBDTScore;
    float               JetBDTScoreSigTrans;

    int                 isEleBDTLoose;
    int                 isEleBDTMedium;
    int                 isEleBDTTight;
 
    float               EleBDTScore;

    int                 passEleOLR;

    float               matchedJetWidth;
    float               matchedJetJvt;
    
    std::vector<float>  tracks_pt;
    std::vector<float>  tracks_eta;
    std::vector<float>  tracks_phi;

    std::vector< int >  tracks_isCore;
    std::vector< int >  tracks_isWide;
    std::vector< int >  tracks_failTrackFilter;
    std::vector< int >  tracks_passTrkSel;
    std::vector< int >  tracks_isClCharged;
    std::vector< int >  tracks_isClIso;
    std::vector< int >  tracks_isClConv;
    std::vector< int >  tracks_isClFake;

  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
