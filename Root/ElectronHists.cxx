#include <xAODAnaHelpers/ElectronHists.h>
#include <sstream>

#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"

ElectronHists :: ElectronHists (std::string name, std::string detailStr) :
  IParticleHists(name, detailStr, "electron", "electron"),
  m_infoSwitch(new HelperClasses::ElectronInfoSwitch(m_detailStr))
{ }

ElectronHists :: ~ElectronHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode ElectronHists::initialize() {
  RETURN_CHECK("IParticleHists::initialize()", IParticleHists::initialize(), "");

  // isolation
  if( m_infoSwitch->m_isolation ) {
    if(m_debug) Info("ElectronHists::initialize()", "adding isolation plots");
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

StatusCode ElectronHists::execute( const xAOD::Electron* electron, float eventWeight, const xAOD::EventInfo* eventInfo ) {
  return execute(static_cast<const xAOD::IParticle*>(electron), eventWeight, eventInfo);
}

StatusCode ElectronHists::execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* eventInfo ) {
  RETURN_CHECK("IParticleHists::execute()", IParticleHists::execute(particle, eventWeight, eventInfo), "");

  if(m_debug) std::cout << "ElectronHists: in execute " <<std::endl;

  const xAOD::Electron* electron=dynamic_cast<const xAOD::Electron*>(particle);
  if(electron==0)
    {
      ::Error( "ElectronHists::execute()", XAOD_MESSAGE( "Cannot convert IParticle to Electron" ));
      return StatusCode::FAILURE;
    }

  // isolation
  if ( m_infoSwitch->m_isolation ) {
    m_ptcone20    ->Fill( electron->isolation( xAOD::Iso::ptcone20    ) / 1e3, eventWeight );
    m_ptcone30    ->Fill( electron->isolation( xAOD::Iso::ptcone30    ) / 1e3, eventWeight );
    m_ptcone40    ->Fill( electron->isolation( xAOD::Iso::ptcone40    ) / 1e3, eventWeight );
    m_ptvarcone20 ->Fill( electron->isolation( xAOD::Iso::ptvarcone20 ) / 1e3, eventWeight );
    m_ptvarcone30 ->Fill( electron->isolation( xAOD::Iso::ptvarcone30 ) / 1e3, eventWeight );
    m_ptvarcone40 ->Fill( electron->isolation( xAOD::Iso::ptvarcone40 ) / 1e3, eventWeight );
    m_topoetcone20->Fill( electron->isolation( xAOD::Iso::topoetcone20) / 1e3, eventWeight );
    m_topoetcone30->Fill( electron->isolation( xAOD::Iso::topoetcone30) / 1e3, eventWeight );
    m_topoetcone40->Fill( electron->isolation( xAOD::Iso::topoetcone40) / 1e3, eventWeight );
  }

  return StatusCode::SUCCESS;
}



StatusCode ElectronHists::execute( const xAH::Electron* elec, float eventWeight, const xAH::EventInfo* eventInfo ) {
  return execute(static_cast<const xAH::Particle*>(elec), eventWeight, eventInfo);
}


StatusCode ElectronHists::execute( const xAH::Particle* particle, float eventWeight, const xAH::EventInfo* /*eventInfo*/  ) {
  RETURN_CHECK("IParticleHists::execute()", IParticleHists::execute(particle, eventWeight), "");

  if(m_debug) std::cout << "MuonHists: in execute " <<std::endl;

  const xAH::Electron* elec=dynamic_cast<const xAH::Electron*>(particle);
  if(elec==0)
    {
      ::Error( "ElctronHists::execute()", XAOD_MESSAGE( "Cannot convert IParticle to Electron" ));
      return StatusCode::FAILURE;
    }


  // isolation
  if ( m_infoSwitch->m_isolation ) {
    m_ptcone20    ->Fill( elec->ptcone20    , eventWeight );
    m_ptcone30    ->Fill( elec->ptcone30    , eventWeight );
    m_ptcone40    ->Fill( elec->ptcone40    , eventWeight );
    m_ptvarcone20 ->Fill( elec->ptvarcone20 , eventWeight );
    m_ptvarcone30 ->Fill( elec->ptvarcone30 , eventWeight );
    m_ptvarcone40 ->Fill( elec->ptvarcone40 , eventWeight );
    m_topoetcone20->Fill( elec->topoetcone20, eventWeight );
    m_topoetcone30->Fill( elec->topoetcone30, eventWeight );
    m_topoetcone40->Fill( elec->topoetcone40, eventWeight );
  }

  return StatusCode::SUCCESS;
}
