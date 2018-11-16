#ifndef xAODAnaHelpers_Electron_H
#define xAODAnaHelpers_Electron_H

#include "xAODAnaHelpers/Particle.h"


namespace xAH {

  class Electron : public Particle
  {
  public:

    // kinematics
    float  caloCluster_eta;
    float  charge;

    // trigger
    int               isTrigMatched;
    std::vector<int>  isTrigMatchedToChain;
    std::string       listTrigChains;

    // isolation
    std::map< std::string, int > isIsolated;
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
    std::map< std::string, int > PID;

    // scale factors w/ sys
    // per object
    std::vector< float > RecoEff_SF;

    std::map< std::string, std::vector< float > > PIDEff_SF;
    std::map< std::string, std::vector< float > > IsoEff_SF;
    std::map< std::string, std::vector< float > > TrigEff_SF;
    std::map< std::string, std::vector< float > > TrigMCEff;
    //const std::vector< std::string > m_PIDWPs = {"LooseAndBLayerLLH","MediumLLH","TightLLH"};
    //const std::vector< std::string > m_isolWPs = {"","_isolFixedCutLoose","_isolFixedCutTight","_isolFixedCutTightTrackOnly","_isolGradient","_isolGradientLoose","_isolLoose","_isolLooseTrackOnly","_isolTight"};

    // reco parameters
    int  author;
    int  OQ;

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

    // prompt lepton
    float PromptLeptonInput_DL1mu;
    float PromptLeptonInput_DRlj;
    float PromptLeptonInput_LepJetPtFrac;
    float PromptLeptonInput_PtFrac;
    float PromptLeptonInput_PtRel;
    int   PromptLeptonInput_TrackJetNTrack;
    float PromptLeptonInput_ip2;
    float PromptLeptonInput_ip3;
    float PromptLeptonInput_rnnip;
    int   PromptLeptonInput_sv1_jf_ntrkv;
    float PromptLeptonIso;
    float PromptLeptonVeto;

  };

}//xAH
#endif // xAODAnaHelpers_Particle_H
