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
    m_expectInnermostPixelLayerHit = new std::vector< char >;
    m_expectNextToInnermostPixelLayerHit = new std::vector< char >;
    m_numberDoF = new std::vector<float >;
  }

  if(m_infoSwitch.m_numbers){
    m_numberOfInnermostPixelLayerHits = new std::vector< char >;
    m_numberOfNextToInnermostPixelLayerHits = new std::vector< char >;
    m_numberOfPhiHoleLayers = new std::vector< char >;
    m_numberOfPhiLayers = new std::vector< char >;
    m_numberOfPixelDeadSensors = new std::vector< char >;
    m_numberOfPixelHits = new std::vector< char >;
    m_numberOfPixelHoles = new std::vector< char >;
    m_numberOfPixelSharedHits = new std::vector< char >;
    m_numberOfPrecisionHoleLayers = new std::vector< char >;
    m_numberOfPrecisionLayers = new std::vector< char >;
    m_numberOfSCTDeadSensors = new std::vector< char >;
    m_numberOfSCTHits = new std::vector< char >;
    m_numberOfSCTHoles = new std::vector< char >;
    m_numberOfSCTSharedHits = new std::vector< char >;
    m_numberOfTRTHits = new std::vector< char >;
    m_numberOfTRTOutliers = new std::vector< char >;
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
    connectBranch<char>(tree, "expectInnermostPixelLayerHit", &m_expectInnermostPixelLayerHit);
    connectBranch<char>(tree, "expectNextToInnermostPixelLayerHit", &m_expectNextToInnermostPixelLayerHit);
    connectBranch<float>(tree, "numberDoF", &m_numberDoF);
  }

  if(m_infoSwitch.m_numbers){
    connectBranch<char>(tree, "numberOfInnermostPixelLayerHits", &m_numberOfInnermostPixelLayerHits);
    connectBranch<char>(tree, "numberOfNextToInnermostPixelLayerHits", &m_numberOfNextToInnermostPixelLayerHits);
    connectBranch<char>(tree, "numberOfPhiHoleLayers", &m_numberOfPhiHoleLayers);
    connectBranch<char>(tree, "numberOfPhiLayers", &m_numberOfPhiLayers);
    connectBranch<char>(tree, "numberOfPixelDeadSensors", &m_numberOfPixelDeadSensors);
    connectBranch<char>(tree, "numberOfPixelHits", &m_numberOfPixelHits);
    connectBranch<char>(tree, "numberOfPixelHoles", &m_numberOfPixelHoles);
    connectBranch<char>(tree, "numberOfPixelSharedHits", &m_numberOfPixelSharedHits);
    connectBranch<char>(tree, "numberOfPrecisionHoleLayers", &m_numberOfPrecisionHoleLayers);
    connectBranch<char>(tree, "numberOfPrecisionLayers", &m_numberOfPrecisionLayers);
    connectBranch<char>(tree, "numberOfSCTDeadSensors", &m_numberOfSCTDeadSensors);
    connectBranch<char>(tree, "numberOfSCTHits", &m_numberOfSCTHits);
    connectBranch<char>(tree, "numberOfSCTHoles", &m_numberOfSCTHoles);
    connectBranch<char>(tree, "numberOfSCTSharedHits", &m_numberOfSCTSharedHits);
    connectBranch<char>(tree, "numberOfTRTHits", &m_numberOfTRTHits);
    connectBranch<char>(tree, "numberOfTRTOutliers", &m_numberOfTRTOutliers);
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
  setBranch<char>(tree, "expectInnermostPixelLayerHit", m_expectInnermostPixelLayerHit);
  setBranch<char>(tree, "expectNextToInnermostPixelLayerHit", m_expectNextToInnermostPixelLayerHit);
  setBranch<float>(tree, "numberDoF", m_numberDoF);
  }

  if(m_infoSwitch.m_numbers){
    setBranch<char>(tree, "numberOfInnermostPixelLayerHits", m_numberOfInnermostPixelLayerHits);
    setBranch<char>(tree, "numberOfNextToInnermostPixelLayerHits", m_numberOfNextToInnermostPixelLayerHits);
    setBranch<char>(tree, "numberOfPhiHoleLayers", m_numberOfPhiHoleLayers);
    setBranch<char>(tree, "numberOfPhiLayers", m_numberOfPhiLayers);
    setBranch<char>(tree, "numberOfPixelDeadSensors", m_numberOfPixelDeadSensors);
    setBranch<char>(tree, "numberOfPixelHits", m_numberOfPixelHits);
    setBranch<char>(tree, "numberOfPixelHoles", m_numberOfPixelHoles);
    setBranch<char>(tree, "numberOfPixelSharedHits", m_numberOfPixelSharedHits);
    setBranch<char>(tree, "numberOfPrecisionHoleLayers", m_numberOfPrecisionHoleLayers);
    setBranch<char>(tree, "numberOfPrecisionLayers", m_numberOfPrecisionLayers);
    setBranch<char>(tree, "numberOfSCTDeadSensors", m_numberOfSCTDeadSensors);
    setBranch<char>(tree, "numberOfSCTHits", m_numberOfSCTHits);
    setBranch<char>(tree, "numberOfSCTHoles", m_numberOfSCTHoles);
    setBranch<char>(tree, "numberOfSCTSharedHits", m_numberOfSCTSharedHits);
    setBranch<char>(tree, "numberOfTRTHits", m_numberOfTRTHits);
    setBranch<char>(tree, "numberOfTRTOutliers", m_numberOfTRTOutliers);
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
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, expectInnermostPixelLayerHit, m_expectInnermostPixelLayerHit, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> expectNextToInnermostPixelLayerHit("expectNextToInnermostPixelLayerHit");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, expectNextToInnermostPixelLayerHit, m_expectNextToInnermostPixelLayerHit, -1);
  }

  if(m_infoSwitch.m_numbers){
    if(m_debug) std::cout << "Filling numbers" << std::endl;

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfInnermostPixelLayerHits("numberOfInnermostPixelLayerHits");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfInnermostPixelLayerHits, m_numberOfInnermostPixelLayerHits, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfNextToInnermostPixelLayerHits("numberOfNextToInnermostPixelLayerHits");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfNextToInnermostPixelLayerHits, m_numberOfNextToInnermostPixelLayerHits, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPhiHoleLayers("numberOfPhiHoleLayers");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPhiHoleLayers, m_numberOfPhiHoleLayers, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPhiLayers("numberOfPhiLayers");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPhiLayers, m_numberOfPhiLayers, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelDeadSensors("numberOfPixelDeadSensors");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPixelDeadSensors, m_numberOfPixelDeadSensors, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelHits("numberOfPixelHits");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPixelHits, m_numberOfPixelHits, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelHoles("numberOfPixelHoles");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPixelHoles, m_numberOfPixelHoles, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelSharedHits("numberOfPixelSharedHits");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPixelSharedHits, m_numberOfPixelSharedHits, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPrecisionHoleLayers("numberOfPrecisionHoleLayers");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPrecisionHoleLayers, m_numberOfPrecisionHoleLayers, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfPrecisionLayers("numberOfPrecisionLayers");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfPrecisionLayers, m_numberOfPrecisionLayers, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTDeadSensors("numberOfSCTDeadSensors");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfSCTDeadSensors, m_numberOfSCTDeadSensors, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTHits("numberOfSCTHits");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfSCTHits, m_numberOfSCTHits, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTHoles("numberOfSCTHoles");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfSCTHoles, m_numberOfSCTHoles, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTSharedHits("numberOfSCTSharedHits");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfSCTSharedHits, m_numberOfSCTSharedHits, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfTRTHits("numberOfTRTHits");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfTRTHits, m_numberOfTRTHits, -1);

    static SG::AuxElement::ConstAccessor<unsigned char> numberOfTRTOutliers("numberOfTRTOutliers");
    safeFill<unsigned char, char, xAOD::TrackParticle>(track, numberOfTRTOutliers, m_numberOfTRTOutliers, -1);
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
