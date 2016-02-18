#include <xAODAnaHelpers/PhotonHists.h>
#include <sstream>

#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"

PhotonHists :: PhotonHists (std::string name, std::string detailStr) :
  IParticleHists(name, detailStr, "photon", "photon"),
  m_infoSwitch(new HelperClasses::PhotonInfoSwitch(m_detailStr))
{ }

PhotonHists :: ~PhotonHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode PhotonHists::initialize() {
  RETURN_CHECK("IParticleHists::initialize()", IParticleHists::initialize(), "");

  // // details for jet cleaning
  // if( m_infoSwitch->m_clean ) {
  //   if(m_debug) Info("JetHists::initialize()", "adding clean plots");
  //   // units?
  //   m_jetTime     = book(m_name, "JetTimming" ,   "Jet Timming",      120, -80, 80);
  //   m_LArQuality  = book(m_name, "LArQuality" ,   "LAr Quality",      120, -600, 600);
  //   m_hecq        = book(m_name, "HECQuality" ,   "HEC Quality",      120, -10, 10);
  //   m_negE        = book(m_name, "NegativeE" ,    "Negative Energy",  120, -10, 10);
  //   m_avLArQF     = book(m_name, "AverageLArQF" , "<LAr Quality Factor>" , 120, 0, 1000);
  //   m_bchCorrCell = book(m_name, "BchCorrCell" ,  "BCH Corr Cell" ,   120, 0, 600);
  //   m_N90Const    = book(m_name, "N90Constituents", "N90 Constituents" ,  120, 0, 40);
  // }

  return StatusCode::SUCCESS;
}

StatusCode PhotonHists::execute( const xAOD::Photon* photon, float eventWeight ) {
  RETURN_CHECK("IParticleHists::execute()", IParticleHists::execute(photon, eventWeight), "");

  if(m_debug) std::cout << "in execute " <<std::endl;

  // //basic
  // m_photonPt ->        Fill( photon->pt()/1e3,    eventWeight );
  // m_photonEta->        Fill( photon->eta(),       eventWeight );
  // m_photonPhi->        Fill( photon->phi(),       eventWeight );
  // m_photonM->          Fill( photon->m()/1e3,     eventWeight );
  // m_photonE->          Fill( photon->e()/1e3,     eventWeight );
  // m_photonRapidity->   Fill( photon->rapidity(),  eventWeight );

  return StatusCode::SUCCESS;
}

