#ifndef xAODAnaHelpers_TrackContainer_H
#define xAODAnaHelpers_TrackContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include "xAODTracking/TrackParticle.h"

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODAnaHelpers/TrackPart.h>
#include <xAODAnaHelpers/ParticleContainer.h>

namespace xAH {

  class TrackContainer : public ParticleContainer<TrackPart, HelperClasses::TrackInfoSwitch>
    {
    public:
      TrackContainer(const std::string& name = "track", const std::string& detailStr="", float units = 1e3);
      virtual ~TrackContainer();

      virtual void setTree    (TTree *tree);
      virtual void setBranches(TTree *tree);
      virtual void clear();
      virtual void FillTrack( const xAOD::TrackParticle* track );
      virtual void FillTrack( const xAOD::IParticle* particle );
      using ParticleContainer::setTree; // make other overloaded version of execute() to show up in subclass

    protected:
      
      virtual void updateParticle(uint idx, TrackPart& track);

    private:
      // 
      // Vector branches
      //

      std::vector<float>* m_chiSquared;
      std::vector<float>* m_d0;
      std::vector<std::vector<float> >* m_definingParametersCovMatrix;
      std::vector<char>* m_expectInnermostPixelLayerHit;
      std::vector<char>* m_expectNextToInnermostPixelLayerHit;
      std::vector<float>* m_numberDoF;
      std::vector<char>* m_numberOfInnermostPixelLayerHits;
      std::vector<char>* m_numberOfNextToInnermostPixelLayerHits;
      std::vector<char>* m_numberOfPhiHoleLayers;
      std::vector<char>* m_numberOfPhiLayers;
      std::vector<char>* m_numberOfPixelDeadSensors;
      std::vector<char>* m_numberOfPixelHits;
      std::vector<char>* m_numberOfPixelHoles;
      std::vector<char>* m_numberOfPixelSharedHits;
      std::vector<char>* m_numberOfPrecisionHoleLayers;
      std::vector<char>* m_numberOfPrecisionLayers;
      std::vector<char>* m_numberOfSCTDeadSensors;
      std::vector<char>* m_numberOfSCTHits;
      std::vector<char>* m_numberOfSCTHoles;
      std::vector<char>* m_numberOfSCTSharedHits;
      std::vector<char>* m_numberOfTRTHits;
      std::vector<char>* m_numberOfTRTOutliers;
      std::vector<float>* m_phi;
      std::vector<float>* m_qOverP;
      std::vector<float>* m_theta;
      std::vector<Int_t>* m_vertexLink;
      std::vector<UInt_t>* m_vertexLink_persIndex;
      std::vector<UInt_t>* m_vertexLink_persKey;
      std::vector<float>* m_vz;
      std::vector<float>* m_z0;
    };
}

#endif // xAODAnaHelpers_TrackContainer_H
