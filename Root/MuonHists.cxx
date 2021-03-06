/********************************************************
 *
 * Muon Histogramming
 *
 * J. Alison (john.alison@cern.ch)
 *
 *******************************************************/


#include <xAODAnaHelpers/MuonHists.h>
#include <sstream>

ANA_MSG_SOURCE(msgMuonHists, "MuonHists")

using std::vector;

MuonHists :: MuonHists (std::string name, std::string detailStr, const std::string& prefix, const std::string& titlePrefix) :
  IParticleHists(name, detailStr, prefix, titlePrefix),
  m_infoSwitch(new HelperClasses::MuonInfoSwitch(m_detailStr))
{ }

MuonHists :: ~MuonHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode MuonHists::initialize() {
  using namespace msgMuonHists;
  ANA_CHECK( IParticleHists::initialize());

  // Isolation
  if(m_infoSwitch->m_isolation){
    for (auto& isol : m_infoSwitch->m_isolWPs) {
      if (isol.empty()) continue;

      m_isIsolated[isol] = book(m_name, "isIsolated_" + isol, "isIsolated_" + isol, 3, -1.5, 1.5);
    }

    m_ptcone20     = book(m_name, "ptcone20",     "ptcone20",     101, -0.2, 20);
    m_ptcone30     = book(m_name, "ptcone30",     "ptcone30",     101, -0.2, 20);
    m_ptcone40     = book(m_name, "ptcone40",     "ptcone40",     101, -0.2, 20);
    m_ptvarcone20  = book(m_name, "ptvarcone20",  "ptvarcone20",  101, -0.2, 20);
    m_ptvarcone30  = book(m_name, "ptvarcone30",  "ptvarcone30",  101, -0.2, 20);
    m_ptvarcone40  = book(m_name, "ptvarcone40",  "ptvarcone40",  101, -0.2, 20);
    m_topoetcone20 = book(m_name, "topoetcone20", "topoetcone20", 101, -0.2, 20);
    m_topoetcone30 = book(m_name, "topoetcone30", "topoetcone30", 101, -0.2, 20);
    m_topoetcone40 = book(m_name, "topoetcone40", "topoetcone40", 101, -0.2, 20);

    m_ptcone20_rel     = book(m_name, "ptcone20_rel",     "ptcone20_rel",     110, -0.2, 2);
    m_ptcone30_rel     = book(m_name, "ptcone30_rel",     "ptcone30_rel",     110, -0.2, 2);
    m_ptcone40_rel     = book(m_name, "ptcone40_rel",     "ptcone40_rel",     110, -0.2, 2);
    m_ptvarcone20_rel  = book(m_name, "ptvarcone20_rel",  "ptvarcone20_rel",  110, -0.2, 2);
    m_ptvarcone30_rel  = book(m_name, "ptvarcone30_rel",  "ptvarcone30_rel",  110, -0.2, 2);
    m_ptvarcone40_rel  = book(m_name, "ptvarcone40_rel",  "ptvarcone40_rel",  110, -0.2, 2);
    m_topoetcone20_rel = book(m_name, "topoetcone20_rel", "topoetcone20_rel", 110, -0.2, 2);
    m_topoetcone30_rel = book(m_name, "topoetcone30_rel", "topoetcone30_rel", 110, -0.2, 2);
    m_topoetcone40_rel = book(m_name, "topoetcone40_rel", "topoetcone40_rel", 110, -0.2, 2);


  }

  // quality
  if(m_infoSwitch->m_quality){
    for (auto& quality : m_infoSwitch->m_recoWPs) {
      if (quality.empty()) continue;

      m_quality[quality] = book(m_name, "is" + quality, "is" + quality, 3, -1.5, 1.5);
    }
  }


  return StatusCode::SUCCESS;
}

StatusCode MuonHists::execute( const xAOD::Muon* muon, float eventWeight, const xAOD::EventInfo* eventInfo) {
  return execute(static_cast<const xAOD::IParticle*>(muon), eventWeight, eventInfo);
}

StatusCode MuonHists::execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* eventInfo  ) {
  using namespace msgMuonHists;
  ANA_CHECK( IParticleHists::execute(particle, eventWeight, eventInfo));

  if(m_debug) std::cout << "MuonHists: in execute " <<std::endl;

  const xAOD::Muon* muon=dynamic_cast<const xAOD::Muon*>(particle);
  if(muon==0)
    {
      ANA_MSG_ERROR( "Cannot convert IParticle to Muon" );
      return StatusCode::FAILURE;
    }

  if ( m_infoSwitch->m_isolation ) {
    static std::map< std::string, SG::AuxElement::Accessor<char> > accIsol;

    for (auto& isol : m_infoSwitch->m_isolWPs) {
      if (!isol.empty() && isol != "NONE") {
        std::string isolWP = "isIsolated_" + isol;
        accIsol.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( isol , SG::AuxElement::Accessor<char>( isolWP ) ) );
          
        if (accIsol.at(isol).isAvailable(*muon)) {
          m_isIsolated[isol]->Fill(accIsol.at(isol)(*muon), eventWeight);
        } else {
          m_isIsolated[isol]->Fill(-1, eventWeight);
        }
      }
    }

    m_ptcone20     ->Fill( muon->isolation( xAOD::Iso::ptcone20 )     ,  eventWeight );
    m_ptcone30     ->Fill( muon->isolation( xAOD::Iso::ptcone30 )     ,  eventWeight );
    m_ptcone40     ->Fill( muon->isolation( xAOD::Iso::ptcone40 )     ,  eventWeight );
    m_ptvarcone20  ->Fill( muon->isolation( xAOD::Iso::ptvarcone20 )  ,  eventWeight );
    m_ptvarcone30  ->Fill( muon->isolation( xAOD::Iso::ptvarcone30 )  ,  eventWeight );
    m_ptvarcone40  ->Fill( muon->isolation( xAOD::Iso::ptvarcone40 )  ,  eventWeight );
    m_topoetcone20 ->Fill( muon->isolation( xAOD::Iso::topoetcone20 ) ,  eventWeight );
    m_topoetcone30 ->Fill( muon->isolation( xAOD::Iso::topoetcone30 ) ,  eventWeight );
    m_topoetcone40 ->Fill( muon->isolation( xAOD::Iso::topoetcone40 ) ,  eventWeight );

    float muonPt = muon->pt();
    m_ptcone20_rel     ->Fill( muon->isolation( xAOD::Iso::ptcone20 )     / muonPt,  eventWeight );
    m_ptcone30_rel     ->Fill( muon->isolation( xAOD::Iso::ptcone30 )     / muonPt,  eventWeight );
    m_ptcone40_rel     ->Fill( muon->isolation( xAOD::Iso::ptcone40 )     / muonPt,  eventWeight );
    m_ptvarcone20_rel  ->Fill( muon->isolation( xAOD::Iso::ptvarcone20 )  / muonPt,  eventWeight );
    m_ptvarcone30_rel  ->Fill( muon->isolation( xAOD::Iso::ptvarcone30 )  / muonPt,  eventWeight );
    m_ptvarcone40_rel  ->Fill( muon->isolation( xAOD::Iso::ptvarcone40 )  / muonPt,  eventWeight );
    m_topoetcone20_rel ->Fill( muon->isolation( xAOD::Iso::topoetcone20 ) / muonPt,  eventWeight );
    m_topoetcone30_rel ->Fill( muon->isolation( xAOD::Iso::topoetcone30 ) / muonPt,  eventWeight );
    m_topoetcone40_rel ->Fill( muon->isolation( xAOD::Iso::topoetcone40 ) / muonPt,  eventWeight );


  }


  if ( m_infoSwitch->m_quality ) {
    static std::map< std::string, SG::AuxElement::Accessor<char> > accQuality;

    for (auto& quality : m_infoSwitch->m_recoWPs) {
      if (!quality.empty()) {
        accQuality.insert( std::pair<std::string, SG::AuxElement::Accessor<char> > ( quality , SG::AuxElement::Accessor<char>( quality ) ) );
        
        if (accQuality.at(quality).isAvailable(*muon)) {
          m_quality[quality]->Fill(accQuality.at(quality)(*muon), eventWeight);
        } else {
          m_quality[quality]->Fill(-1, eventWeight);
        }
      }
    }
  }



  return StatusCode::SUCCESS;
}

StatusCode MuonHists::execute( const xAH::Muon* muon, float eventWeight, const xAH::EventInfo* eventInfo ) {
  return execute(static_cast<const xAH::Particle*>(muon), eventWeight, eventInfo);
}


StatusCode MuonHists::execute( const xAH::Particle* particle, float eventWeight, const xAH::EventInfo* /*eventInfo*/  ) {
  using namespace msgMuonHists;
  ANA_CHECK( IParticleHists::execute(particle, eventWeight));

  if(m_debug) std::cout << "MuonHists: in execute " <<std::endl;

  const xAH::Muon* muon=dynamic_cast<const xAH::Muon*>(particle);
  if(muon==0)
    {
      ANA_MSG_ERROR( "Cannot convert IParticle to Muon" );
      return StatusCode::FAILURE;
    }

  if ( m_infoSwitch->m_isolation ) {
    for (auto& isol : m_infoSwitch->m_isolWPs) {
      if (isol.empty()) continue;

      m_isIsolated[isol]->Fill(muon->isIsolated.at(isol), eventWeight);
    }

    m_ptcone20     ->Fill( muon->ptcone20      ,  eventWeight );
    m_ptcone30     ->Fill( muon->ptcone30      ,  eventWeight );
    m_ptcone40     ->Fill( muon->ptcone40      ,  eventWeight );
    m_ptvarcone20  ->Fill( muon->ptvarcone20   ,  eventWeight );
    m_ptvarcone30  ->Fill( muon->ptvarcone30   ,  eventWeight );
    m_ptvarcone40  ->Fill( muon->ptvarcone40   ,  eventWeight );
    m_topoetcone20 ->Fill( muon->topoetcone20  ,  eventWeight );
    m_topoetcone30 ->Fill( muon->topoetcone30  ,  eventWeight );
    m_topoetcone40 ->Fill( muon->topoetcone40  ,  eventWeight );

    float muonPt = muon->p4.Pt();
    m_ptcone20_rel     ->Fill( muon->ptcone20/muonPt        ,  eventWeight );
    m_ptcone30_rel     ->Fill( muon->ptcone30/muonPt        ,  eventWeight );
    m_ptcone40_rel     ->Fill( muon->ptcone40/muonPt        ,  eventWeight );
    m_ptvarcone20_rel  ->Fill( muon->ptvarcone20 /muonPt    ,  eventWeight );
    m_ptvarcone30_rel  ->Fill( muon->ptvarcone30 /muonPt    ,  eventWeight );
    m_ptvarcone40_rel  ->Fill( muon->ptvarcone40 /muonPt    ,  eventWeight );
    m_topoetcone20_rel ->Fill( muon->topoetcone20/muonPt    ,  eventWeight );
    m_topoetcone30_rel ->Fill( muon->topoetcone30/muonPt    ,  eventWeight );
    m_topoetcone40_rel ->Fill( muon->topoetcone40/muonPt    ,  eventWeight );

  }


  if ( m_infoSwitch->m_quality ) {
    for (auto& quality : m_infoSwitch->m_recoWPs) {
      if (quality.empty()) continue;

      m_quality[quality]->Fill(muon->quality.at(quality), eventWeight);
    }
  }

  return StatusCode::SUCCESS;
}
