#include "xAODAnaHelpers/TrackContainer.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <iostream>
#include <xAODTracking/TrackParticle.h>

using namespace xAH;

TrackContainer::TrackContainer(const std::string& name, const std::string& detailStr, float units) : ParticleContainer(name,detailStr,units,true)

{
  if(m_infoSwitch.m_fitpars){
    m_chiSquared = new std::vector<float >;
    m_d0 = new std::vector<float >;
    m_definingParametersCovMatrix = new std::vector<std::vector<float> >;
    m_expectInnermostPixelLayerHit = new std::vector<unsigned char >;
    m_expectNextToInnermostPixelLayerHit = new std::vector<unsigned char >;
    m_numberDoF = new std::vector<float >;
  }

  if(m_infoSwitch.m_numbers){
    m_numberOfInnermostPixelLayerHits = new std::vector<unsigned char >;
    m_numberOfNextToInnermostPixelLayerHits = new std::vector<unsigned char >;
    m_numberOfPhiHoleLayers = new std::vector<unsigned char >;
    m_numberOfPhiLayers = new std::vector<unsigned char >;
    m_numberOfPixelDeadSensors = new std::vector<unsigned char >;
    m_numberOfPixelHits = new std::vector<unsigned char >;
    m_numberOfPixelHoles = new std::vector<unsigned char >;
    m_numberOfPixelSharedHits = new std::vector<unsigned char >;
    m_numberOfPrecisionHoleLayers = new std::vector<unsigned char >;
    m_numberOfPrecisionLayers = new std::vector<unsigned char >;
    m_numberOfSCTDeadSensors = new std::vector<unsigned char >;
    m_numberOfSCTHits = new std::vector<unsigned char >;
    m_numberOfSCTHoles = new std::vector<unsigned char >;
    m_numberOfSCTSharedHits = new std::vector<unsigned char >;
    m_numberOfTRTHits = new std::vector<unsigned char >;
    m_numberOfTRTOutliers = new std::vector<unsigned char >;
  }

  m_phi = new std::vector<float >;
  m_qOverP = new std::vector<float >;
  m_theta = new std::vector<float >;

  if(m_infoSwitch.m_vertex){
    /*
      m_vertexLink = new std::vector<Int_t >;
      m_vertexLink_persIndex = new std::vector<UInt_t >;
      m_vertexLink_persKey = new std::vector<UInt_t >;
    */
    m_vz = new std::vector<float >;
    m_z0 = new std::vector<float >;
  }
}

TrackContainer::~TrackContainer()
{
  if(m_debug) std::cout << " Deleting TrackContainer "<< std::endl;

  if(m_infoSwitch.m_fitpars){
    delete m_chiSquared;
    delete m_d0;
    delete m_definingParametersCovMatrix;
    delete m_expectInnermostPixelLayerHit;
    delete m_expectNextToInnermostPixelLayerHit;
    delete m_numberDoF;
  }

  if(m_infoSwitch.m_numbers){
    delete m_numberOfInnermostPixelLayerHits;
    delete m_numberOfNextToInnermostPixelLayerHits;
    delete m_numberOfPhiHoleLayers;
    delete m_numberOfPhiLayers;
    delete m_numberOfPixelDeadSensors;
    delete m_numberOfPixelHits;
    delete m_numberOfPixelHoles;
    delete m_numberOfPixelSharedHits;
    delete m_numberOfPrecisionHoleLayers;
    delete m_numberOfPrecisionLayers;
    delete m_numberOfSCTDeadSensors;
    delete m_numberOfSCTHits;
    delete m_numberOfSCTHoles;
    delete m_numberOfSCTSharedHits;
    delete m_numberOfTRTHits;
    delete m_numberOfTRTOutliers;
  }

  delete m_phi;
  delete m_qOverP;
  delete m_theta;

  if(m_infoSwitch.m_vertex){
    /*
      delete m_vertexLink;
      delete m_vertexLink_persIndex;
      delete m_vertexLink_persKey;
    */
    delete m_vz;
    delete m_z0;
  }
}

void TrackContainer::setTree(TTree *tree)
{
  ParticleContainer::setTree(tree);

  if(m_infoSwitch.m_fitpars){
    connectBranch<float>(tree, "chiSquared", &m_chiSquared);
    connectBranch<float>(tree, "d0", &m_d0);
    connectBranch<std::vector<float> >(tree, "definingParametersCovMatrix", &m_definingParametersCovMatrix);
    connectBranch<unsigned char>(tree, "expectInnermostPixelLayerHit", &m_expectInnermostPixelLayerHit);
    connectBranch<unsigned char>(tree, "expectNextToInnermostPixelLayerHit", &m_expectNextToInnermostPixelLayerHit);
    connectBranch<float>(tree, "numberDoF", &m_numberDoF);
  }

  if(m_infoSwitch.m_numbers){
    connectBranch<unsigned char>(tree, "numberOfInnermostPixelLayerHits", &m_numberOfInnermostPixelLayerHits);
    connectBranch<unsigned char>(tree, "numberOfNextToInnermostPixelLayerHits", &m_numberOfNextToInnermostPixelLayerHits);
    connectBranch<unsigned char>(tree, "numberOfPhiHoleLayers", &m_numberOfPhiHoleLayers);
    connectBranch<unsigned char>(tree, "numberOfPhiLayers", &m_numberOfPhiLayers);
    connectBranch<unsigned char>(tree, "numberOfPixelDeadSensors", &m_numberOfPixelDeadSensors);
    connectBranch<unsigned char>(tree, "numberOfPixelHits", &m_numberOfPixelHits);
    connectBranch<unsigned char>(tree, "numberOfPixelHoles", &m_numberOfPixelHoles);
    connectBranch<unsigned char>(tree, "numberOfPixelSharedHits", &m_numberOfPixelSharedHits);
    connectBranch<unsigned char>(tree, "numberOfPrecisionHoleLayers", &m_numberOfPrecisionHoleLayers);
    connectBranch<unsigned char>(tree, "numberOfPrecisionLayers", &m_numberOfPrecisionLayers);
    connectBranch<unsigned char>(tree, "numberOfSCTDeadSensors", &m_numberOfSCTDeadSensors);
    connectBranch<unsigned char>(tree, "numberOfSCTHits", &m_numberOfSCTHits);
    connectBranch<unsigned char>(tree, "numberOfSCTHoles", &m_numberOfSCTHoles);
    connectBranch<unsigned char>(tree, "numberOfSCTSharedHits", &m_numberOfSCTSharedHits);
    connectBranch<unsigned char>(tree, "numberOfTRTHits", &m_numberOfTRTHits);
    connectBranch<unsigned char>(tree, "numberOfTRTOutliers", &m_numberOfTRTOutliers);
  }

  connectBranch<float>(tree, "phi", &m_phi);
  connectBranch<float>(tree, "qOverP", &m_qOverP);
  connectBranch<float>(tree, "theta", &m_theta);

  if(m_infoSwitch.m_vertex){
    /*
      connectBranch<Int_t>(tree, "vertexLink", &m_vertexLink);
      connectBranch<UInt_t>(tree, "vertexLink_persIndex", &m_vertexLink_persIndex);
      connectBranch<UInt_t>(tree, "vertexLink_persKey", &m_vertexLink_persKey);
    */
    connectBranch<float>(tree, "vz", &m_vz);
    connectBranch<float>(tree, "z0", &m_z0);
  }
}

void TrackContainer::updateParticle(uint idx, TrackPart& track)
{
  if(m_debug) std::cout << "in TrackContainer::updateParticle" << std::endl;
  ParticleContainer::updateParticle(idx, track);

  if(m_infoSwitch.m_fitpars){
    track.chiSquared = m_chiSquared->at(idx);
    track.d0 = m_d0->at(idx);
    track.definingParametersCovMatrix = m_definingParametersCovMatrix->at(idx);
    track.expectInnermostPixelLayerHit = m_expectInnermostPixelLayerHit->at(idx);
    track.expectNextToInnermostPixelLayerHit = m_expectNextToInnermostPixelLayerHit->at(idx);
  }

  if(m_infoSwitch.m_numbers){
    track.numberDoF = m_numberDoF->at(idx);
    track.numberOfInnermostPixelLayerHits = m_numberOfInnermostPixelLayerHits->at(idx);
    track.numberOfNextToInnermostPixelLayerHits = m_numberOfNextToInnermostPixelLayerHits->at(idx);
    track.numberOfPhiHoleLayers = m_numberOfPhiHoleLayers->at(idx);
    track.numberOfPhiLayers = m_numberOfPhiLayers->at(idx);
    track.numberOfPixelDeadSensors = m_numberOfPixelDeadSensors->at(idx);
    track.numberOfPixelHits = m_numberOfPixelHits->at(idx);
    track.numberOfPixelHoles = m_numberOfPixelHoles->at(idx);
    track.numberOfPixelSharedHits = m_numberOfPixelSharedHits->at(idx);
    track.numberOfPrecisionHoleLayers = m_numberOfPrecisionHoleLayers->at(idx);
    track.numberOfPrecisionLayers = m_numberOfPrecisionLayers->at(idx);
    track.numberOfSCTDeadSensors = m_numberOfSCTDeadSensors->at(idx);
    track.numberOfSCTHits = m_numberOfSCTHits->at(idx);
    track.numberOfSCTHoles = m_numberOfSCTHoles->at(idx);
    track.numberOfSCTSharedHits = m_numberOfSCTSharedHits->at(idx);
    track.numberOfTRTHits = m_numberOfTRTHits->at(idx);
    track.numberOfTRTOutliers = m_numberOfTRTOutliers->at(idx);
  }

  track.phi = m_phi->at(idx);
  track.qOverP = m_qOverP->at(idx);
  track.theta = m_theta->at(idx);

  if(m_infoSwitch.m_vertex){
    /*
      track.vertexLink = m_vertexLink->at(idx);
      track.vertexLink_persIndex = m_vertexLink_persIndex->at(idx);
      track.vertexLink_persKey = m_vertexLink_persKey->at(idx);
    */
    track.vz = m_vz->at(idx);
    track.z0 = m_z0->at(idx);
  }

  if(m_debug) std::cout << "leaving TrackContainer::updateParticle" << std::endl;
  return;
}

void TrackContainer::setBranches(TTree *tree)
{
  ParticleContainer::setBranches(tree);

  if(m_infoSwitch.m_fitpars){
  setBranch<float>(tree, "chiSquared", m_chiSquared);
  setBranch<float>(tree, "d0", m_d0);
  setBranch<std::vector<float>>(tree, "definingParametersCovMatrix", m_definingParametersCovMatrix);
  setBranch<unsigned char>(tree, "expectInnermostPixelLayerHit", m_expectInnermostPixelLayerHit);
  setBranch<unsigned char>(tree, "expectNextToInnermostPixelLayerHit", m_expectNextToInnermostPixelLayerHit);
  setBranch<float>(tree, "numberDoF", m_numberDoF);
  }

  if(m_infoSwitch.m_numbers){
    setBranch<unsigned char>(tree, "numberOfInnermostPixelLayerHits", m_numberOfInnermostPixelLayerHits);
    setBranch<unsigned char>(tree, "numberOfNextToInnermostPixelLayerHits", m_numberOfNextToInnermostPixelLayerHits);
    setBranch<unsigned char>(tree, "numberOfPhiHoleLayers", m_numberOfPhiHoleLayers);
    setBranch<unsigned char>(tree, "numberOfPhiLayers", m_numberOfPhiLayers);
    setBranch<unsigned char>(tree, "numberOfPixelDeadSensors", m_numberOfPixelDeadSensors);
    setBranch<unsigned char>(tree, "numberOfPixelHits", m_numberOfPixelHits);
    setBranch<unsigned char>(tree, "numberOfPixelHoles", m_numberOfPixelHoles);
    setBranch<unsigned char>(tree, "numberOfPixelSharedHits", m_numberOfPixelSharedHits);
    setBranch<unsigned char>(tree, "numberOfPrecisionHoleLayers", m_numberOfPrecisionHoleLayers);
    setBranch<unsigned char>(tree, "numberOfPrecisionLayers", m_numberOfPrecisionLayers);
    setBranch<unsigned char>(tree, "numberOfSCTDeadSensors", m_numberOfSCTDeadSensors);
    setBranch<unsigned char>(tree, "numberOfSCTHits", m_numberOfSCTHits);
    setBranch<unsigned char>(tree, "numberOfSCTHoles", m_numberOfSCTHoles);
    setBranch<unsigned char>(tree, "numberOfSCTSharedHits", m_numberOfSCTSharedHits);
    setBranch<unsigned char>(tree, "numberOfTRTHits", m_numberOfTRTHits);
    setBranch<unsigned char>(tree, "numberOfTRTOutliers", m_numberOfTRTOutliers);
  }

  setBranch<float>(tree, "phi", m_phi);
  setBranch<float>(tree, "qOverP", m_qOverP);
  setBranch<float>(tree, "theta", m_theta);

  if(m_infoSwitch.m_vertex){
    /*
      setBranch<Int_t>(tree, "vertexLink", m_vertexLink);
      setBranch<UInt_t>(tree, "vertexLink_persIndex", m_vertexLink_persIndex);
      setBranch<UInt_t>(tree, "vertexLink_persKey", m_vertexLink_persKey);
    */
    setBranch<float>(tree, "vz", m_vz);
    setBranch<float>(tree, "z0", m_z0);
  }
}

void TrackContainer::clear()
{
  ParticleContainer::clear();

  if(m_infoSwitch.m_fitpars){
    m_chiSquared->clear();
    m_d0->clear();
    m_definingParametersCovMatrix->clear();
    m_expectInnermostPixelLayerHit->clear();
    m_expectNextToInnermostPixelLayerHit->clear();
    m_numberDoF->clear();
  }

  if(m_infoSwitch.m_numbers){
    m_numberOfInnermostPixelLayerHits->clear();
    m_numberOfNextToInnermostPixelLayerHits->clear();
    m_numberOfPhiHoleLayers->clear();
    m_numberOfPhiLayers->clear();
    m_numberOfPixelDeadSensors->clear();
    m_numberOfPixelHits->clear();
    m_numberOfPixelHoles->clear();
    m_numberOfPixelSharedHits->clear();
    m_numberOfPrecisionHoleLayers->clear();
    m_numberOfPrecisionLayers->clear();
    m_numberOfSCTDeadSensors->clear();
    m_numberOfSCTHits->clear();
    m_numberOfSCTHoles->clear();
    m_numberOfSCTSharedHits->clear();
    m_numberOfTRTHits->clear();
    m_numberOfTRTOutliers->clear();
  }

  m_phi->clear();
  m_qOverP->clear();
  m_theta->clear();

  if(m_infoSwitch.m_vertex){
    /*
      m_vertexLink->clear();
      m_vertexLink_persIndex->clear();
      m_vertexLink_persKey->clear();
    */
    m_vz->clear();
    m_z0->clear();
  }
}

void TrackContainer::FillTrack( const xAOD::TrackParticle* track ){
  return FillTrack(static_cast<const xAOD::IParticle*>(track));
}

void TrackContainer::FillTrack( const xAOD::IParticle* particle ){
  if(m_debug) std::cout << "In Fill Track" << std::endl;
  ParticleContainer::FillParticle(particle);

  const xAOD::TrackParticle* track=dynamic_cast<const xAOD::TrackParticle*>(particle);
  if(m_debug) std::cout << "Got TrackParticle" << std::endl;

  if(m_infoSwitch.m_fitpars){
    if(m_debug) std::cout << "Filling fitpars" << std::endl;

    m_chiSquared->push_back( track->chiSquared() );
    m_d0->push_back( track->d0() );
    m_numberDoF->push_back( track->numberDoF() );
    //m_definingParametersCovMatrix ->push_back(track->definingParametersCovMatrix() ); // fix this too

    static SG::AuxElement::ConstAccessor<unsigned char> expectInnermostPixelLayerHit("expectInnermostPixelLayerHit");
    //safeFill<char, int, xAOD::TrackParticle>(track, expectInnermostPixelLayerHit, m_expectInnermostPixelLayerHit, -999);
    m_expectInnermostPixelLayerHit->push_back(expectInnermostPixelLayerHit(*track));

    //m_expectNextToInnermostPixelLayerHit->push_back(track->expectNextToInnermostPixelLayerHit() );
    static SG::AuxElement::ConstAccessor<unsigned char> expectNextToInnermostPixelLayerHit("expectNextToInnermostPixelLayerHit");
    m_expectNextToInnermostPixelLayerHit->push_back(expectNextToInnermostPixelLayerHit(*track));
  }

  if(m_infoSwitch.m_numbers){
    if(m_debug) std::cout << "Filling numbers" << std::endl;

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfInnermostPixelLayerHits("numberOfInnermostPixelLayerHits");
    m_numberOfInnermostPixelLayerHits->push_back(numberOfInnermostPixelLayerHits(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfNextToInnermostPixelLayerHits("numberOfNextToInnermostPixelLayerHits");
    m_numberOfNextToInnermostPixelLayerHits->push_back(numberOfNextToInnermostPixelLayerHits(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPhiHoleLayers("numberOfPhiHoleLayers");
    m_numberOfPhiHoleLayers->push_back(numberOfPhiHoleLayers(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPhiLayers("numberOfPhiLayers");
    m_numberOfPhiLayers->push_back(numberOfPhiLayers(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelDeadSensors("numberOfPixelDeadSensors");
    m_numberOfPixelDeadSensors->push_back(numberOfPixelDeadSensors(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelHits("numberOfPixelHits");
    m_numberOfPixelHits->push_back(numberOfPixelHits(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelHoles("numberOfPixelHoles");
    m_numberOfPixelHoles->push_back(numberOfPixelHoles(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelSharedHits("numberOfPixelSharedHits");
    m_numberOfPixelSharedHits->push_back(numberOfPixelSharedHits(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPrecisionHoleLayers("numberOfPrecisionHoleLayers");
    m_numberOfPrecisionHoleLayers->push_back(numberOfPrecisionHoleLayers(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPrecisionLayers("numberOfPrecisionLayers");
    m_numberOfPrecisionLayers->push_back(numberOfPrecisionLayers(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTDeadSensors("numberOfSCTDeadSensors");
    m_numberOfSCTDeadSensors->push_back(numberOfSCTDeadSensors(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTHits("numberOfSCTHits");
    m_numberOfSCTHits->push_back(numberOfSCTHits(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTHoles("numberOfSCTHoles");
    m_numberOfSCTHoles->push_back(numberOfSCTHoles(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTSharedHits("numberOfSCTSharedHits");
    m_numberOfSCTSharedHits->push_back(numberOfSCTSharedHits(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfTRTHits("numberOfTRTHits");
    m_numberOfTRTHits->push_back(numberOfTRTHits(*track) );

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfTRTOutliers("numberOfTRTOutliers");
    m_numberOfTRTOutliers->push_back(numberOfTRTOutliers(*track) );
  }

  m_phi->push_back(track->phi() );
  m_qOverP->push_back(track->qOverP() );
  m_theta->push_back(track->theta() );

  if(m_infoSwitch.m_vertex){
    if(m_debug) std::cout << "Filling vertex" << std::endl;

    //static SG::AuxElement::ConstAccessor<int> vertexLink("vertexLink");
    /*
      m_vertexLink->push_back(vertexLink(*track) );
      //m_vertexLink_persIndex->push_back(track->vertexLink.m_persIndex() );
      //m_vertexLink_persKey->push_back(track->vertexLink.m_persKey() );
      m_vertexLink_persIndex->push_back( -999 );
      m_vertexLink_persKey->push_back( -999 );
    */
    m_vz->push_back(track->vz() );
    m_z0->push_back(track->z0() );
  }

  if(m_debug) std::cout << "Leave Fill Track" << std::endl;
  return;
}
