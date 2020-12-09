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
    unsigned char expectInnermostPixelLayerHit;
    unsigned char expectNextToInnermostPixelLayerHit;

    float numberDoF;

    unsigned char numberOfInnermostPixelLayerHits;
    unsigned char numberOfNextToInnermostPixelLayerHits;
    unsigned char numberOfPhiHoleLayers;
    unsigned char numberOfPhiLayers;
    unsigned char numberOfPixelDeadSensors;
    unsigned char numberOfPixelHits;
    unsigned char numberOfPixelHoles;
    unsigned char numberOfPixelSharedHits;
    unsigned char numberOfPrecisionHoleLayers;
    unsigned char numberOfPrecisionLayers;
    unsigned char numberOfSCTDeadSensors;
    unsigned char numberOfSCTHits;
    unsigned char numberOfSCTHoles;
    unsigned char numberOfSCTSharedHits;
    unsigned char numberOfTRTHits;
    unsigned char numberOfTRTOutliers;

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

