#ifndef xAODAnaHelpers_TrackPart_H
#define xAODAnaHelpers_TrackPart_H

#include "xAODAnaHelpers/Particle.h"

namespace xAH {
  class TrackPart : public Particle
  {
  public:
    // TrackPart() { };
    // virtual ~TrackPart();
    
    float chiSquared;
    float d0;
    
    std::vector<float> definingParametersCovMatrix;
    char expectInnermostPixelLayerHit;
    char expectNextToInnermostPixelLayerHit;

    float numberDoF;

    char numberOfInnermostPixelLayerHits;
    char numberOfNextToInnermostPixelLayerHits;
    char numberOfPhiHoleLayers;
    char numberOfPhiLayers;
    char numberOfPixelDeadSensors;
    char numberOfPixelHits;
    char numberOfPixelHoles;
    char numberOfPixelSharedHits;
    char numberOfPrecisionHoleLayers;
    char numberOfPrecisionLayers;
    char numberOfSCTDeadSensors;
    char numberOfSCTHits;
    char numberOfSCTHoles;
    char numberOfSCTSharedHits;
    char numberOfTRTHits;
    char numberOfTRTOutliers;

    float phi;
    float qOverP;
    float theta;

    Int_t vertexLink;
    UInt_t vertexLink_persIndex;
    UInt_t vertexLink_persKey;

    float vz;
    float z0;

  };
} //xAH

#endif // xAODAnaHelpers_TrackPart_H

