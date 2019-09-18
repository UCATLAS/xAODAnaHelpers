#include <iostream>
#include <typeinfo>

#include <EventLoop/Job.h>
#include <EventLoop/Algorithm.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

#include "MuonSelectorTools/MuonSelectionTool.h"
#include "MuonMomentumCorrections/MuonCalibrationPeriodTool.h"

#include "xAODAnaHelpers/MuonInFatJetCorrection.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

static const SG::AuxElement::ConstAccessor<ElementLink<xAOD::JetContainer>> acc_parent("Parent");
static const SG::AuxElement::ConstAccessor<std::vector<ElementLink<xAOD::IParticleContainer>>> acc_ghostMatchedTrackJets("GhostAntiKt2TrackJet");
static const SG::AuxElement::ConstAccessor<std::vector<ElementLink<xAOD::IParticleContainer>>> acc_ghostMatchedTrackJetsVR("GhostVR30Rmax4Rmin02TrackJet");
SG::AuxElement::Decorator<std::vector<ElementLink<xAOD::IParticleContainer>>> m_decMuonsInTrackJetLink("MuonsInTrackJet");

// Needed to distribute the algorithm to the workers
ClassImp(MuonInFatJetCorrection)

MuonInFatJetCorrection :: MuonInFatJetCorrection() :
  Algorithm("MuonInFatJetCorrection")
{
  m_trackJetPtMin = 10000.0,
  m_trackJetEtaMax = 2.5,
  m_trackJetNConst = 2.0,
  m_muonPtMin = 10000.0,
  m_muonEtaMax = 2.7,
  m_muonDrMax = 0.4,
  doVR = true, 
  m_debug = false;

}

EL::StatusCode MuonInFatJetCorrection :: setupJob(EL::Job &job)
{
  ANA_MSG_DEBUG("Calling setupJob");
  job.useXAOD();
  xAOD::Init("MuonInFatJetCorrection").ignore();
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrection :: histInitialize()
{
  ANA_MSG_DEBUG("Calling histInitialize");
  ANA_CHECK(xAH::Algorithm::algInitialize());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrection :: fileExecute()
{
  ANA_MSG_DEBUG("Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrection :: changeInput(bool /*firstFile*/)
{
  ANA_MSG_DEBUG("Calling changeInput");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrection :: initialize()
{
  ANA_MSG_DEBUG("Calling initialize");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // FatJet Calibration
  m_fatJetCalibration = new JetCalibrationTool("HbbISRJets");
  m_fatJetCalibration->setProperty("JetCollection",m_fatJetAlgo);
  m_fatJetCalibration->setProperty("ConfigFile"   ,m_fatJetConfig);
  m_fatJetCalibration->setProperty("CalibSequence",m_fatJetCalibSeq);
  m_fatJetCalibration->setProperty("CalibArea"    ,m_fatCalibArea);


  ANA_CHECK( m_fatJetCalibration->initialize() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrection :: execute()
{
 
 // static const char *FatJetCollectionName = "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets";
 // const xAOD::JetContainer *fatJets = 0;
 // 
 // // Retrieve fatJets
 // if (!m_event->retrieve(fatJets, FatJetCollectionName).isSuccess()) {
 //   Error("MuonInFatJetCorrection::execute()", "Failed to retrieve FatJet container.");
 //   return EL::StatusCode::FAILURE;
 // }

  static SG::AuxElement::Decorator<TLorentzVector> dec_4vecMuon("muonCorrectedP4");
  const xAOD::JetContainer *fatJets(nullptr);
  ANA_CHECK(HelperFunctions::retrieve(fatJets, m_inContainerName, m_event, m_store, msg()));
  
  // Loop over fatjets
  for (auto fatJet = fatJets->begin(); fatJet != fatJets->end(); fatJet++){
    
    //xAOD::Jet * fatJetCal = 0;
    //m_fatJetCalibration->calibratedCopy(**fatJet, fatJetCal);
    TLorentzVector correctedVector;

    getHbbCorrectedVector(*fatJet, correctedVector, doVR);

    dec_4vecMuon(*fatJet) =  correctedVector;
  }
  return EL::StatusCode::SUCCESS;
} 


EL::StatusCode MuonInFatJetCorrection :: finalize()
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrection::getHbbCorrectedVector(const xAOD::Jet& jet, TLorentzVector& correctedVector, const bool doVR)
{
  /* Steps:
     1. Get all AntiKt2TrackJets/AntiKtVR30Rmax4Rmin02TrackJets asssociated with the ungroomed jet
     2. Match muons to these b-tagged track-jets
     - if more than 1 muon matches a track jet, only use the muon closest in DR
     3. Correct the fat-jet mass by putting the matched muon back
  */

  // no associated muons available at the moment in DxAOD
  // decorate track jets with muons using simple dR matching
  if (decorateWithMuons(jet, doVR) == StatusCode::FAILURE) {
    Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "No muon decoration for muon-in-jet correction available.");
  }

  // Step 1

  std::vector<ElementLink<xAOD::IParticleContainer> > associated_trackJets;

  // get the track jets from the parent
  bool problemWithParent = false;
  ElementLink<xAOD::JetContainer> parentEL;
  if (!acc_parent.isAvailable(jet)) problemWithParent = true;
  else parentEL = acc_parent(jet);
  if (problemWithParent || !parentEL.isValid()){
    if (problemWithParent) Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "Parent decoration does not exist.");
    if (!parentEL.isValid()) Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "Parent link is not valid.");
  }
  else {
    const xAOD::Jet *parentJet = *parentEL;
    // use accessor instead of getAssociatedObject in order to have EL
    if (doVR){
      if (!acc_ghostMatchedTrackJetsVR.isAvailable(*parentJet)) {
        Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "Ghostmatched jet collection does not exist.");
      }
      associated_trackJets = acc_ghostMatchedTrackJetsVR(*parentJet);
    } else {
      if (!acc_ghostMatchedTrackJets.isAvailable(*parentJet)) {
        Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "Ghostmatched jet collection does not exist.");
      }
      associated_trackJets = acc_ghostMatchedTrackJets(*parentJet);
    }
  }
 // decorate all trackjets by default
  std::vector<ElementLink<xAOD::IParticleContainer> > associated_trackJets_filtered;
  for (const auto &trackJetEL : associated_trackJets) {
    if (!trackJetEL.isValid()) {
      Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "Track jet link is not valid.");
    }
    const xAOD::Jet *trackJet(static_cast<const xAOD::Jet*> (*trackJetEL));
    if (trackJet->pt() < m_trackJetPtMin) continue;
    if (fabs(trackJet->eta()) > m_trackJetEtaMax) continue;
    if (trackJet->numConstituents() < m_trackJetNConst) continue;
    associated_trackJets_filtered.push_back(trackJetEL);
  }
  std::sort(associated_trackJets_filtered.begin(), associated_trackJets_filtered.end(), [](ElementLink<xAOD::IParticleContainer> lhs, ElementLink<xAOD::IParticleContainer> rhs) -> bool { return ((static_cast<const xAOD::IParticle*>(*lhs))->pt() > (static_cast<const xAOD::IParticle*>(*rhs))->pt()); });

  // Step 2

  std::vector<xAOD::Muon*> calibratedMuons;
  std::vector<const xAOD::Muon*> matched_muons;
  for (const auto &trackJetEL : associated_trackJets_filtered) {
    const xAOD::Jet *trackJet(static_cast<const xAOD::Jet*> (*trackJetEL));
    float maxDR(m_muonDrMax);
    const xAOD::Muon *closest_muon(nullptr);
    // get muons from jet decoration
    if (!m_decMuonsInTrackJetLink.isAvailable(*trackJet)) {
      Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "No muons link found for jet.");
    }
    for (auto muonLink : m_decMuonsInTrackJetLink(*trackJet)) {
      if (!muonLink.isValid()) {
        if (m_debug) std::cout << "Muon link not valid." << std::endl;
        continue;
      }
      const xAOD::Muon *muon(static_cast<const xAOD::Muon*> (*muonLink));
      // apply muon correction if not calibrated yet
      static SG::AuxElement::Accessor<float> acc_muonSpectrometerPt("MuonSpectrometerPt");
      static SG::AuxElement::Accessor<float> acc_innerDetectorPt("InnerDetectorPt");
      if (!(acc_innerDetectorPt.isAvailable(*muon) && acc_muonSpectrometerPt.isAvailable(*muon))) {
        if (m_debug) std::cout << "No decorators for MuonSpectrometerPt or InnerDetectorPt found. Calibrate muons on-the-fly." << std::endl;
        xAOD::Muon *muon_calib(nullptr);
        if (m_muonCalibrationPeriodTool->correctedCopy(*muon, muon_calib) != CP::CorrectionCode::Ok) {
          Error("MuonInFatJetCorrection::getHbbCorrectedVector()", "Could not get calibrated copy of muon.");
        }
        // save the pointers for deletion later
        calibratedMuons.push_back(muon_calib);
        // work with calibrated muon
        muon = muon_calib;
      }
     // muon quality selection
      if (muon->pt() < m_muonPtMin) continue;
      if (m_muonSelectionTool->getQuality(*muon) > xAOD::Muon::Medium) continue;
      if (fabs(muon->eta()) > m_muonEtaMax) continue;
      // find clostest muon
      float DR( trackJet->p4().DeltaR(muon->p4()) );
      float cutDR=std::min(0.4,0.04 + 10000.0/muon->pt());
      if (DR > cutDR) continue;
      if (DR > maxDR) continue;
      maxDR = DR;
      closest_muon = muon;
    }

    // check if the closest muon was already selected
    for (const auto muon : matched_muons){
      if (closest_muon == muon){
        closest_muon = nullptr;
        if (m_debug) std::cout << "Muon duplicate found! Skipping." << std::endl;
        break;
      }
    }
    if (closest_muon) {
      matched_muons.push_back(closest_muon);
    }
  }

  // Step 3

  xAOD::JetFourMom_t corrected_jet_p4 = getMuonCorrectedJetFourMom(jet, matched_muons, "Combined");
  TLorentzVector corrected_jet(corrected_jet_p4.x(), corrected_jet_p4.y(), corrected_jet_p4.z(), corrected_jet_p4.t());
  correctedVector = corrected_jet;

  // delete the vector of pointers to muons after you have used them to correct the four vector
  for (xAOD::Muon *muonPointer : calibratedMuons) {
    delete muonPointer;
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrection::decorateWithMuons(const xAOD::Jet& jet, const bool doVR) const
{
  // retrieve muons from StoreGate
  const xAOD::MuonContainer *muons;
  if (!m_event->retrieve(muons, "Muons")) {
    Error("MuonInFatJetCorrection::decorateWithMuons()", "Cannot retrieve muon collection Muons from event store.");
    return StatusCode::FAILURE;
  }
  // retrieve ghost-associated track jets from large-R jet
  std::vector<const xAOD::Jet*> associated_trackJets;
  // get the element links to the parent, ungroomed jet
  if (!acc_parent.isAvailable(jet)) {
    Error("MuonInFatJetCorrection::decorateWithMuons()", "Parent (ungroomed) jet collection does not exist.");
    return StatusCode::FAILURE;
  }
  ElementLink<xAOD::JetContainer> parentEL = acc_parent(jet);
  if (!parentEL.isValid()) {
    Error("MuonInFatJetCorrection::decorateWithMuons()", "Parent link is not valid.");
    return StatusCode::FAILURE;
  }
  // access the track jets
  const xAOD::Jet* parentJet = *parentEL;
  if (doVR) {
    if (!parentJet->getAssociatedObjects<xAOD::Jet>("GhostVR30Rmax4Rmin02TrackJet", associated_trackJets)) {
      if (m_debug) std::cout << "No associated track jets found on parent jet." << std::endl;
      return StatusCode::SUCCESS;
    }
  } else {
    if (!parentJet->getAssociatedObjects<xAOD::Jet>("GhostAntiKt2TrackJet", associated_trackJets)) {
      if (m_debug) std::cout << "No associated track jets found on parent jet." << std::endl;
      return StatusCode::SUCCESS;
    }
  }

  // decorate all track jets by default, no selection, no muon overlap removal (will be done later)
  float maxDR(m_muonDrMax);
  for (auto trackJet : associated_trackJets) {
    std::vector<ElementLink<xAOD::IParticleContainer>> muons_in_jet;
    for (const auto& muon: *muons) {
      float DR(trackJet->p4().DeltaR(muon->p4()));
      if (DR > maxDR) continue;
      ElementLink<xAOD::IParticleContainer> muonEL(*muons, muon->index());
      muons_in_jet.push_back(muonEL);
    }
    m_decMuonsInTrackJetLink(*trackJet) = muons_in_jet;
    if (m_debug) std::cout << "Found " << muons_in_jet.size() << " muons within R < " << m_muonDrMax << " of associated track jet."<< std::endl;
  }
  return StatusCode::SUCCESS;
}

const xAOD::JetFourMom_t MuonInFatJetCorrection::getMuonCorrectedJetFourMom(const xAOD::Jet &jet, std::vector<const xAOD::Muon*> muons, std::string scheme, bool useJMSScale) const
{
  xAOD::JetFourMom_t JetCorr_tlv = jet.jetP4();
  if (muons.size() == 0) {
    return JetCorr_tlv;
  }
  if (m_debug) std::cout << "Derive muon-in-jet correction: nMuons = " << (int) muons.size() << "\tMuonCorrectionScheme = " << scheme << "\tuseJMSScale = " << useJMSScale<<std::endl;
  if (scheme == "Calorimeter") {
    // muon-in-jet correction for jets calibrated using calorimeter mass
    xAOD::JetFourMom_t CaloJet_tlv = jet.jetP4();
    if (useJMSScale) CaloJet_tlv = jet.jetP4("JetJMSScaleMomentumCalo");
    for(auto muon : muons) {
      // get energy loss of muon in the calorimeter
      float eLoss(0.0);
      muon->parameter(eLoss, xAOD::Muon::EnergyLoss);
      if (m_debug) std::cout << "Energy loss in calorimter = " << eLoss<<std::endl;
      // use muon tlv to get x/y/z compontent of energy loss
      auto muon_tlv = muon->p4();
      double eLossX = eLoss * sin(muon_tlv.Theta()) * cos(muon_tlv.Phi());
      double eLossY = eLoss * sin(muon_tlv.Theta()) * sin(muon_tlv.Phi());
      double eLossZ = eLoss * cos(muon_tlv.Theta());
      TLorentzVector mLoss(eLossX, eLossY, eLossZ, eLoss);
      auto muonCorr_tlv = muon_tlv - mLoss;
      // apply muon-in-jet correction for track-assisted mass calibration to output jet TLV
      CaloJet_tlv += xAOD::JetFourMom_t(muonCorr_tlv.Pt(), muonCorr_tlv.Eta(), muonCorr_tlv.Phi(), muonCorr_tlv.M());
    }
    JetCorr_tlv = CaloJet_tlv;
  } else if (scheme == "TrackAssisted") {
    // muon-in-jet correction for jets calibrated using track-assisted mass
    xAOD::JetFourMom_t TAJet_tlv = jet.jetP4();
    if (useJMSScale) TAJet_tlv = jet.jetP4("JetJMSScaleMomentumTA");
    xAOD::JetFourMom_t CaloJet_tlv = jet.jetP4("JetJMSScaleMomentumCalo");
    xAOD::JetFourMom_t CaloJetCorr_tlv =  getMuonCorrectedJetFourMom(jet, muons, "Calorimeter", true);
    float TAJetCorr_m = TAJet_tlv.M() / CaloJet_tlv.Pt() * CaloJetCorr_tlv.Pt() ;
    float TAJetCorr_pt = sqrt((CaloJetCorr_tlv.E() * CaloJetCorr_tlv.E()) - (TAJetCorr_m * TAJetCorr_m)) / cosh(CaloJetCorr_tlv.Eta());
    // apply muon-in-jet correction for track-assisted mass calibration to output jet TLV
    JetCorr_tlv = xAOD::JetFourMom_t(TAJetCorr_pt, CaloJetCorr_tlv.Eta(), CaloJetCorr_tlv.Phi(), TAJetCorr_m);
  } else if (scheme == "Combined") {
    // muon-in-jet correction for jets calibrated using combined mass
    xAOD::JetFourMom_t TAJet_tlv = jet.jetP4("JetJMSScaleMomentumTA");
    xAOD::JetFourMom_t TAJetCorr_tlv = getMuonCorrectedJetFourMom(jet, muons, "TrackAssisted", true);
    xAOD::JetFourMom_t CaloJet_tlv = jet.jetP4("JetJMSScaleMomentumCalo");
    xAOD::JetFourMom_t CaloJetCorr_tlv = getMuonCorrectedJetFourMom(jet, muons, "Calorimeter", true);
    xAOD::JetFourMom_t CombJet_tlv = jet.jetP4();
    float CaloWeight = (CombJet_tlv.M() -   TAJet_tlv.M()) / (CaloJet_tlv.M() - TAJet_tlv.M());
    float TAWeight   = (CaloJet_tlv.M() - CombJet_tlv.M()) / (CaloJet_tlv.M() - TAJet_tlv.M());
    if (m_debug) std::cout << "CaloWeight = " << CaloWeight << "\tTAWeight = " << TAWeight << std::endl;
    float CombJetCorr_m = CaloWeight * CaloJetCorr_tlv.M() + TAWeight * TAJetCorr_tlv.M();
    float CombJetCorr_pt = sqrt((CaloJetCorr_tlv.E() * CaloJetCorr_tlv.E()) - (CombJetCorr_m * CombJetCorr_m)) / cosh(CaloJetCorr_tlv.Eta());
    // apply muon-in-jet correction for track-assisted mass calibration to output jet TLV
    JetCorr_tlv = xAOD::JetFourMom_t(CombJetCorr_pt, CaloJetCorr_tlv.Eta(), CaloJetCorr_tlv.Phi(), CombJetCorr_m);
  } else if (scheme == "SimpleMuon") {
    // unknown mass calibration; just add muon 4-momentum
    for(auto muon : muons) {
      JetCorr_tlv += xAOD::JetFourMom_t(muon->pt(), muon->eta(), muon->phi(), muon->m());
    }
  } else {
    Error("MuonInFatJetCorrection::getMuonCorrectedJetFourMom()", "Unknown muon correction scheme.");
  }
  if (m_debug) std::cout << "Before muon-in-jet: pt = " << jet.pt() << "\t eta = " << jet.eta()
                         << "\tphi = " << jet.phi() << "\tm = " << jet.m() << std::endl;
  if (m_debug) std::cout << "After muon-in-jet:  pt = " << JetCorr_tlv.pt() << "\teta = " << JetCorr_tlv.eta()
                         << "\tphi = " << JetCorr_tlv.phi() << "\tm = " << JetCorr_tlv.M() << std::endl;
  for(auto muon : muons) {
    if(m_debug) std::cout<<"muons: pt = " << muon->pt() << "\teta = " << muon->eta()
                         << "\tphi = " << muon->phi() << "\tm = " << muon->m() << std::endl;
  }
  return JetCorr_tlv;
}
