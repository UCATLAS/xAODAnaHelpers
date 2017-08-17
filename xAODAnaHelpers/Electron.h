#ifndef xAODAnaHelpers_Electron_H
#define xAODAnaHelpers_Electron_H

#include "xAODAnaHelpers/Particle.h"


namespace xAH {
  
  class Electron : public Particle
  {
  public:

    // kinematics
    float  caloCluster_eta;
 
    // trigger
    int               isTrigMatched;
    std::vector<int>  isTrigMatchedToChain;
    std::string       listTrigChains;

    // isolation
    int   isIsolated_LooseTrackOnly;
    int   isIsolated_Loose;
    int   isIsolated_Tight;
    int   isIsolated_Gradient;
    int   isIsolated_GradientLoose;
    int   isIsolated_FixedCutLoose;
    int   isIsolated_FixedCutTight;
    int   isIsolated_FixedCutTightTrackOnly;
    int   isIsolated_UserDefinedFixEfficiency;
    int   isIsolated_UserDefinedCut;
    float etcone20;
    float ptcone20;
    float ptcone30;
    float ptcone40;
    float ptvarcone20;
    float ptvarcone30;
    float ptvarcone40;
    float topoetcone20;
    float topoetcone30;
    float topoetcone40;

    // PID
    int   LHLoose;
    int   LHLooseBL;
    int   LHMedium;
    int   LHTight;
    int   IsEMLoose;
    int   IsEMMedium;
    int   IsEMTight;
    
    // scale factors w/ sys
    // per object
    std::vector< float > RecoEff_SF;
    std::vector< float > PIDEff_SF_LHLooseAndBLayer;
    std::vector< float > PIDEff_SF_LHLoose;
    std::vector< float > PIDEff_SF_LHMedium;
    std::vector< float > PIDEff_SF_LHTight;
    
    std::map< std::string, std::vector< float > > IsoEff_SF;
    std::map< std::string, std::vector< float > > TrigEff_SF;
    std::map< std::string, std::vector< float > > TrigMCEff;
    //const std::vector< std::string > m_PIDWPs = {"LooseAndBLayerLLH","MediumLLH","TightLLH"};
    //const std::vector< std::string > m_isolWPs = {"","_isolFixedCutLoose","_isolFixedCutTight","_isolFixedCutTightTrackOnly","_isolGradient","_isolGradientLoose","_isolLoose","_isolLooseTrackOnly","_isolTight"};
    
    // track parameters
    float  trkd0;
    float  trkd0sig;
    float  trkz0;
    float  trkz0sintheta;
    float  trkphi0;
    float  trktheta;
    float  trkcharge;
    float  trkqOverP;
    
    // track hit content
    int  trknSiHits;
    int  trknPixHits;
    int  trknPixHoles;
    int  trknSCTHits;
    int  trknSCTHoles;
    int  trknTRTHits;
    int  trknTRTHoles;
    int  trknBLayerHits;
    int  trknInnermostPixLayHits; // not available in DC14
    float trkPixdEdX;            // not available in DC14


  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
