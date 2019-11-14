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

#include "xAODAnaHelpers/MuonInFatJetCorrector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"


// Needed to distribute the algorithm to the workers
ClassImp(MuonInFatJetCorrector)

MuonInFatJetCorrector :: MuonInFatJetCorrector() :
  Algorithm("MuonInFatJetCorrector")
{
}

EL::StatusCode MuonInFatJetCorrector :: setupJob(EL::Job& job)
{
  ANA_MSG_DEBUG("Calling setupJob");
  job.useXAOD();
  xAOD::Init("MuonInFatJetCorrector").ignore();
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrector :: histInitialize()
{
  ANA_MSG_DEBUG("Calling histInitialize");
  ANA_CHECK(xAH::Algorithm::algInitialize());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrector :: fileExecute()
{
  ANA_MSG_DEBUG("Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrector :: changeInput(bool /*firstFile*/)
{
  ANA_MSG_DEBUG("Calling changeInput");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrector :: initialize()
{
  ANA_MSG_DEBUG("Calling initialize");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrector :: execute()
{
  //
  // Do muon matching
  ANA_CHECK(matchTrackJetsToMuons());

  //
  // Loop over large-R jets (all systematics) and calculate correction
  std::vector<std::string>* systNames(nullptr);
  if ( !m_inputAlgo.empty() )
    {
      ANA_CHECK( HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, msg()) );
    }
  else
    {
      systNames=new std::vector<std::string>({""});
    }

  // Decorator holding muon in fatjet corrected fatjets.
  static SG::AuxElement::Decorator<TLorentzVector> dec_correctedFatJets_tlv("correctedFatJets_tlv");  
  for(const std::string& systName : *systNames)
    {
      // Retrieve calibrated fatjets.
      const xAOD::JetContainer *fatJets(nullptr);
      ANA_CHECK(HelperFunctions::retrieve(fatJets, m_fatJetContainerName+systName, m_event, m_store, msg()));

      // Loop over fatjets
      for(const xAOD::Jet *fatJet : *fatJets)
	{
	  // Get corrected fatjet.
	  TLorentzVector correctedVector = getHbbCorrectedVector(*fatJet);

	  dec_correctedFatJets_tlv(*fatJet) = correctedVector;
	}
    }

  // Clean up systematics list if none exists
  if(m_inputAlgo.empty())
    delete systNames;

  return EL::StatusCode::SUCCESS;
} 

EL::StatusCode MuonInFatJetCorrector :: postExecute ()
{
  ANA_MSG_DEBUG("Calling postExecute");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonInFatJetCorrector :: finalize()
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MuonInFatJetCorrector :: histFinalize ()
{

  ANA_MSG_DEBUG( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}


TLorentzVector MuonInFatJetCorrector::getHbbCorrectedVector(const xAOD::Jet& jet)
{
  /* Steps:
     1. Get all track jets asssociated with the ungroomed jet
     2. Match muons to these b-tagged track-jets
     - if more than 1 muon matches a track jet, only use the muon closest in DR
     3. Correct the fat-jet mass by putting the matched muon back
  */

  //
  // Step 1
  std::vector<const xAOD::Jet*> associated_trackJets;

  // get the element links to the parent, ungroomed jet
  static const SG::AuxElement::ConstAccessor<ElementLink<xAOD::JetContainer>> acc_Parent("Parent");
  if (!acc_Parent.isAvailable(jet))
    {
      ANA_MSG_FATAL("Parent (ungroomed) jet collection does not exist.");
      return TLorentzVector();
    }
  ElementLink<xAOD::JetContainer> parentEL = acc_Parent(jet);
  if (!parentEL.isValid())
    {
      ANA_MSG_FATAL("Parent link is not valid.");
      return TLorentzVector();
    }
  
  // access the track jets
  const xAOD::Jet* parentJet = *parentEL;
  if (!parentJet->getAssociatedObjects<xAOD::Jet>(m_trackJetLinkName, associated_trackJets))
    {
      ANA_MSG_FATAL("No associated track jets found on parent jet.");
      return TLorentzVector();
    }
  
  // get trackjets of interest
  std::vector<const xAOD::Jet*> associated_trackJets_filtered;
  for (const xAOD::Jet* trackJet : associated_trackJets)
    {
      if (trackJet->pt() < m_trackJetPtMin) continue;
      if (fabs(trackJet->eta()) > m_trackJetEtaMax) continue;
      if (trackJet->numConstituents() < m_trackJetNConst) continue;

      associated_trackJets_filtered.push_back(trackJet);
  }

  std::sort(associated_trackJets_filtered.begin(), associated_trackJets_filtered.end(), [](const xAOD::Jet* lhs, const xAOD::Jet* rhs) -> bool { return (lhs->pt() > rhs->pt()); });

  //
  // Step 2
  std::vector<const xAOD::Muon*> matched_muons;
  for (const xAOD::Jet* trackJet : associated_trackJets_filtered)
    {
      const xAOD::Muon* closest_muon=nullptr;
      float maxDR=m_muonDrMax;

      // get muons from jet decoration
      static const SG::AuxElement::Accessor<std::vector<ElementLink<xAOD::MuonContainer>>> acc_MuonsInTrackJet("MuonsInTrackJet");

      if(!acc_MuonsInTrackJet.isAvailable(*trackJet))
	{
	  ANA_MSG_FATAL("No muons associated to track jet.");
	  return TLorentzVector();
	}

      std::vector<ElementLink<xAOD::MuonContainer>> associated_muons=acc_MuonsInTrackJet(*trackJet);	    
      for (const ElementLink<xAOD::MuonContainer>& muonEL : associated_muons)
	{
	  const xAOD::Muon* muon=(*muonEL);

	  // muon quality selection
	  if (muon->pt() < m_muonPtMin) continue;
	  if (muon->quality() > xAOD::Muon::Medium) continue;
	  if (fabs(muon->eta()) > m_muonEtaMax) continue;
	  // find clostest muon
	  float DR = trackJet->p4().DeltaR(muon->p4());
	  float cutDR=std::min(0.4,0.04 + 10000.0/muon->pt());
	  if (DR > cutDR) continue;
	  if (DR > maxDR) continue;
	  maxDR = DR;
	  closest_muon = muon;
	}

      // check if the closest muon was already selected
      if(std::find(matched_muons.begin(),matched_muons.end(),closest_muon)!=matched_muons.end())
	{
	  closest_muon = nullptr;
	  ANA_MSG_DEBUG("Muon duplicate found! Skipping.");
	  break;
	}

      if (closest_muon)
	matched_muons.push_back(closest_muon);
    }

  //
  // Step 3
  xAOD::JetFourMom_t corrected_jet_p4 = getMuonCorrectedJetFourMom(jet, matched_muons, Scheme::Combined);
  TLorentzVector corrected_jet(corrected_jet_p4.x(), corrected_jet_p4.y(), corrected_jet_p4.z(), corrected_jet_p4.t());

  return corrected_jet;
}

EL::StatusCode MuonInFatJetCorrector::matchTrackJetsToMuons() const
{
  // retrieve muons from StoreGate
  const xAOD::MuonContainer *muons(nullptr);
  ANA_CHECK(HelperFunctions::retrieve(muons, m_muonContainerName, m_event, m_store, msg()));

  // retrieve track jets from StoreGate
  const xAOD::JetContainer *trackJets(nullptr);
  ANA_CHECK(HelperFunctions::retrieve(trackJets, m_trackJetContainerName, m_event, m_store, msg()));

  // decorate all track jets by default, no selection, no muon overlap removal (will be done later)
  static SG::AuxElement::Decorator<std::vector<ElementLink<xAOD::MuonContainer>>> dec_MuonsInTrackJet("MuonsInTrackJet");
  for (const xAOD::Jet* trackJet : *trackJets)
    {
      std::vector<ElementLink<xAOD::MuonContainer>> muons_in_jet;
      uint32_t idx=0;
      for (const xAOD::Muon* muon : *muons)
	{
	  float DR=trackJet->p4().DeltaR(muon->p4());
	  if (DR < m_muonDrMax)
	    {
	      ElementLink<xAOD::MuonContainer> muonEL(*muons, idx);
	      muons_in_jet.push_back(muonEL);
	    }
	  ++idx;
	}

      dec_MuonsInTrackJet(*trackJet) = muons_in_jet;

      ANA_MSG_DEBUG("Found " << muons_in_jet.size() << " muons within R < " << m_muonDrMax << " of associated track jet.");
    }

  return StatusCode::SUCCESS;
}

const xAOD::JetFourMom_t MuonInFatJetCorrector::getMuonCorrectedJetFourMom(const xAOD::Jet &jet, std::vector<const xAOD::Muon*> muons, Scheme scheme, bool useJMSScale) const
{
  xAOD::JetFourMom_t JetCorr_tlv = jet.jetP4();
  if (muons.size() == 0)
    return JetCorr_tlv;

  ANA_MSG_DEBUG("Derive muon-in-jet correction: nMuons = " << (int) muons.size() << "\tMuonCorrectionScheme = " << scheme << "\tuseJMSScale = " << useJMSScale);
  switch(scheme)
    {
    case Scheme::Calorimeter:
      {
	// muon-in-jet correction for jets calibrated using calorimeter mass
	xAOD::JetFourMom_t CaloJet_tlv = jet.jetP4();
	if (useJMSScale) CaloJet_tlv = jet.jetP4("JetJMSScaleMomentumCalo");
	for(const xAOD::Muon* muon : muons)
	  {
	    // get energy loss of muon in the calorimeter
	    float eLoss=0.0;
	    muon->parameter(eLoss, xAOD::Muon::EnergyLoss);
	    ANA_MSG_DEBUG("Energy loss in calorimter = " << eLoss);
	    // use muon tlv to get x/y/z compontent of energy loss
	    TLorentzVector muon_tlv = muon->p4();
	    double eLossX = eLoss * sin(muon_tlv.Theta()) * cos(muon_tlv.Phi());
	    double eLossY = eLoss * sin(muon_tlv.Theta()) * sin(muon_tlv.Phi());
	    double eLossZ = eLoss * cos(muon_tlv.Theta());
	    TLorentzVector mLoss(eLossX, eLossY, eLossZ, eLoss);
	    TLorentzVector muonCorr_tlv = muon_tlv - mLoss;
	    // apply muon-in-jet correction for track-assisted mass calibration to output jet TLV
	    CaloJet_tlv += xAOD::JetFourMom_t(muonCorr_tlv.Pt(), muonCorr_tlv.Eta(), muonCorr_tlv.Phi(), muonCorr_tlv.M());
	  }
	JetCorr_tlv = CaloJet_tlv;
	break;
      }
    case Scheme::TrackAssisted:
      {
	// muon-in-jet correction for jets calibrated using track-assisted mass
	xAOD::JetFourMom_t TAJet_tlv = jet.jetP4();
	if (useJMSScale) TAJet_tlv = jet.jetP4("JetJMSScaleMomentumTA");
	xAOD::JetFourMom_t CaloJet_tlv = jet.jetP4("JetJMSScaleMomentumCalo");
	xAOD::JetFourMom_t CaloJetCorr_tlv =  getMuonCorrectedJetFourMom(jet, muons, Scheme::Calorimeter, true);
	float TAJetCorr_m = TAJet_tlv.M() / CaloJet_tlv.Pt() * CaloJetCorr_tlv.Pt() ;
	float TAJetCorr_pt = sqrt((CaloJetCorr_tlv.E() * CaloJetCorr_tlv.E()) - (TAJetCorr_m * TAJetCorr_m)) / cosh(CaloJetCorr_tlv.Eta());
	// apply muon-in-jet correction for track-assisted mass calibration to output jet TLV
	JetCorr_tlv = xAOD::JetFourMom_t(TAJetCorr_pt, CaloJetCorr_tlv.Eta(), CaloJetCorr_tlv.Phi(), TAJetCorr_m);
	break;
      }
    case Scheme::Combined:
      {
	// muon-in-jet correction for jets calibrated using combined mass
	xAOD::JetFourMom_t TAJet_tlv       = jet.jetP4("JetJMSScaleMomentumTA");
	xAOD::JetFourMom_t TAJetCorr_tlv   = getMuonCorrectedJetFourMom(jet, muons, Scheme::TrackAssisted, true);
	xAOD::JetFourMom_t CaloJet_tlv     = jet.jetP4("JetJMSScaleMomentumCalo");
	xAOD::JetFourMom_t CaloJetCorr_tlv = getMuonCorrectedJetFourMom(jet, muons, Scheme::Calorimeter  , true);
	xAOD::JetFourMom_t CombJet_tlv = jet.jetP4();
	float CaloWeight = (CombJet_tlv.M() -   TAJet_tlv.M()) / (CaloJet_tlv.M() - TAJet_tlv.M());
	float TAWeight   = (CaloJet_tlv.M() - CombJet_tlv.M()) / (CaloJet_tlv.M() - TAJet_tlv.M());
	ANA_MSG_DEBUG("CaloWeight = " << CaloWeight << "\tTAWeight = " << TAWeight);
	float CombJetCorr_m = CaloWeight * CaloJetCorr_tlv.M() + TAWeight * TAJetCorr_tlv.M();
	float CombJetCorr_pt = sqrt((CaloJetCorr_tlv.E() * CaloJetCorr_tlv.E()) - (CombJetCorr_m * CombJetCorr_m)) / cosh(CaloJetCorr_tlv.Eta());
	// apply muon-in-jet correction for track-assisted mass calibration to output jet TLV
	JetCorr_tlv = xAOD::JetFourMom_t(CombJetCorr_pt, CaloJetCorr_tlv.Eta(), CaloJetCorr_tlv.Phi(), CombJetCorr_m);
	break;
      }
    case SimpleMuon:
      {
	// unknown mass calibration; just add muon 4-momentum
	for(const xAOD::Muon* muon : muons)
	  {
	    JetCorr_tlv += xAOD::JetFourMom_t(muon->pt(), muon->eta(), muon->phi(), muon->m());
	  }
	break;
      }
    default:
      {
	ANA_MSG_FATAL("Unknown muon correction scheme.");
      }
    }

  ANA_MSG_DEBUG("Before muon-in-jet: pt = " << jet.pt() << "\t eta = " << jet.eta()
		<< "\tphi = " << jet.phi() << "\tm = " << jet.m());
  ANA_MSG_DEBUG("After muon-in-jet:  pt = " << JetCorr_tlv.pt() << "\teta = " << JetCorr_tlv.eta()
		<< "\tphi = " << JetCorr_tlv.phi() << "\tm = " << JetCorr_tlv.M());
  for(const xAOD::Muon* muon : muons)
    {
      ANA_MSG_DEBUG("muons: pt = " << muon->pt() << "\teta = " << muon->eta()
		    << "\tphi = " << muon->phi() << "\tm = " << muon->m());
    }

  return JetCorr_tlv;
}
