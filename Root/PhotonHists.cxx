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

  // isolation
  if( m_infoSwitch->m_isolation ) {
    if(m_debug) Info("PhotonHists::initialize()", "adding isolation plots");
    m_ptcone20    = book(m_name, "ptcone20" ,     "ptcone20",      120, -10, 100);
    m_ptcone30    = book(m_name, "ptcone30" ,     "ptcone30",      120, -10, 100);
    m_ptcone40    = book(m_name, "ptcone40" ,     "ptcone40",      120, -10, 100);
    m_ptvarcone20 = book(m_name, "ptvarcone20" ,  "ptvarcone20",   120, -10, 100);
    m_ptvarcone30 = book(m_name, "ptvarcone30" ,  "ptvarcone30",   120, -10, 100);
    m_ptvarcone40 = book(m_name, "ptvarcone40" ,  "ptvarcone40",   120, -10, 100);
    m_topoetcone20= book(m_name, "topoetcone20" , "topoetcone20",  120, -10, 100);
    m_topoetcone30= book(m_name, "topoetcone30" , "topoetcone30",  120, -10, 100);
    m_topoetcone40= book(m_name, "topoetcone40" , "topoetcone40",  120, -10, 100);
  }

  return StatusCode::SUCCESS;
}

StatusCode PhotonHists::execute( const xAOD::Photon* photon, float eventWeight, const xAOD::EventInfo* eventInfo ) {
  return execute(static_cast<const xAOD::IParticle*>(photon), eventWeight, eventInfo);
}

StatusCode PhotonHists::execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* eventInfo ) {
  RETURN_CHECK("IParticleHists::execute()", IParticleHists::execute(particle, eventWeight, eventInfo), "");

  if(m_debug) std::cout << "PhotonHists: in execute " <<std::endl;

  const xAOD::Photon* photon=dynamic_cast<const xAOD::Photon*>(particle);
  if(photon==0)
    {
      ::Error( "PhotonHists::execute()", XAOD_MESSAGE( "Cannot convert IParticle to Photon" ));
      return StatusCode::FAILURE;
    }

  // isolation
  if ( m_infoSwitch->m_isolation ) {
    m_ptcone20    ->Fill( photon->isolation( xAOD::Iso::ptcone20    ) / 1e3, eventWeight );
    m_ptcone30    ->Fill( photon->isolation( xAOD::Iso::ptcone30    ) / 1e3, eventWeight );
    m_ptcone40    ->Fill( photon->isolation( xAOD::Iso::ptcone40    ) / 1e3, eventWeight );
    m_ptvarcone20 ->Fill( photon->isolation( xAOD::Iso::ptvarcone20 ) / 1e3, eventWeight );
    m_ptvarcone30 ->Fill( photon->isolation( xAOD::Iso::ptvarcone30 ) / 1e3, eventWeight );
    m_ptvarcone40 ->Fill( photon->isolation( xAOD::Iso::ptvarcone40 ) / 1e3, eventWeight );
    m_topoetcone20->Fill( photon->isolation( xAOD::Iso::topoetcone20) / 1e3, eventWeight );
    m_topoetcone30->Fill( photon->isolation( xAOD::Iso::topoetcone30) / 1e3, eventWeight );
    m_topoetcone40->Fill( photon->isolation( xAOD::Iso::topoetcone40) / 1e3, eventWeight );
  }

  return StatusCode::SUCCESS;
}

