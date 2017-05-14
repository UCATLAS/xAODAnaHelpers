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
    m_isIsolated_LooseTrackOnly           = book(m_name, "isIsolated_LooseTrackOnly"       ,   "isIsolated_LooseTrackOnly", 3, -1.5, 1.5);
    m_isIsolated_Loose                    = book(m_name, "isIsolated_Loose"            ,       "isIsolated_Loose", 3, -1.5, 1.5);
    m_isIsolated_Tight                    = book(m_name, "isIsolated_Tight"             ,      "isIsolated_Tight", 3, -1.5, 1.5);
    m_isIsolated_Gradient                 = book(m_name, "isIsolated_Gradient"      ,          "isIsolated_Gradient", 3, -1.5, 1.5);
    m_isIsolated_GradientLoose            = book(m_name, "isIsolated_GradientLoose",           "isIsolated_GradientLoose", 3, -1.5, 1.5);
    m_isIsolated_GradientT1               = book(m_name, "isIsolated_GradientT1",              "isIsolated_GradientT1", 3, -1.5, 1.5);
    m_isIsolated_GradientT2               = book(m_name, "isIsolated_GradientT2",              "isIsolated_GradientT2", 3, -1.5, 1.5);
    m_isIsolated_MU0p06                   = book(m_name, "isIsolated_MU0p06",                  "isIsolated_MU0p06", 3, -1.5, 1.5);
    m_isIsolated_FixedCutLoose            = book(m_name, "isIsolated_FixedCutLoose",           "isIsolated_FixedCutLoose", 3, -1.5, 1.5);
    m_isIsolated_FixedCutTight            = book(m_name, "isIsolated_FixedCutTight",           "isIsolated_FixedCutTight", 3, -1.5, 1.5);
    m_isIsolated_FixedCutTightTrackOnly   = book(m_name, "isIsolated_FixedCutTightTrackOnly"  ,"isIsolated_FixedCutTightTrackOnly", 3, -1.5, 1.5);
    m_isIsolated_UserDefinedFixEfficiency = book(m_name, "isIsolated_UserDefinedFixEfficiency","isIsolated_UserDefinedFixEfficiency", 3, -1.5, 1.5);
    m_isIsolated_UserDefinedCut           = book(m_name, "isIsolated_UserDefinedCut",          "isIsolated_UserDefinedCut", 3, -1.5, 1.5);

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
    m_isVeryLoose = book(m_name, "isVeryLoose", "isVeryLoose", 3, -1.5, 1.5);
    m_isLoose     = book(m_name, "isLoose"    , "isLoose"    , 3, -1.5, 1.5);
    m_isMedium    = book(m_name, "isMedium"   , "isMedium"   , 3, -1.5, 1.5);
    m_isTight     = book(m_name, "isTight"    , "isTight"    , 3, -1.5, 1.5);
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

    static SG::AuxElement::Accessor<char> isIsoLooseTrackOnlyAcc ("isIsolated_LooseTrackOnly");
    static SG::AuxElement::Accessor<char> isIsoLooseAcc ("isIsolated_Loose");
    static SG::AuxElement::Accessor<char> isIsoTightAcc ("isIsolated_Tight");
    static SG::AuxElement::Accessor<char> isIsoGradientAcc ("isIsolated_Gradient");
    static SG::AuxElement::Accessor<char> isIsoGradientLooseAcc ("isIsolated_GradientLoose");
    static SG::AuxElement::Accessor<char> isIsoGradientT1Acc ("isIsolated_GradientT1");
    static SG::AuxElement::Accessor<char> isIsoGradientT2Acc ("isIsolated_GradientT2");
    static SG::AuxElement::Accessor<char> isIsoMU0p06Acc ("isIsolated_MU0p06");
    static SG::AuxElement::Accessor<char> isIsoFixedCutLooseAcc ("isIsolated_FixedCutLoose");
    static SG::AuxElement::Accessor<char> isIsoFixedCutTightAcc ("isIsolated_FixedCutTight");
    static SG::AuxElement::Accessor<char> isIsoFixedCutTightTrackOnlyAcc ("isIsolated_FixedCutTightTrackOnly");
    static SG::AuxElement::Accessor<char> isIsoUserDefinedFixEfficiencyAcc ("isIsolated_UserDefinedFixEfficiency");
    static SG::AuxElement::Accessor<char> isIsoUserDefinedCutAcc ("isIsolated_UserDefinedCut");

    if ( isIsoLooseTrackOnlyAcc.isAvailable( *muon ) ) { m_isIsolated_LooseTrackOnly->Fill( isIsoLooseTrackOnlyAcc( *muon ) ,  eventWeight ); } else {m_isIsolated_LooseTrackOnly->Fill( -1 ,  eventWeight );}
    if ( isIsoLooseAcc.isAvailable( *muon ) )          { m_isIsolated_Loose->Fill( isIsoLooseAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_Loose->Fill( -1 ,  eventWeight ); }
    if ( isIsoTightAcc.isAvailable( *muon ) )          { m_isIsolated_Tight->Fill( isIsoTightAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_Tight->Fill( -1 ,  eventWeight ); }
    if ( isIsoGradientAcc.isAvailable( *muon ) )       { m_isIsolated_Gradient->Fill( isIsoGradientAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_Gradient->Fill( -1 ,  eventWeight ); }
    if ( isIsoGradientLooseAcc.isAvailable( *muon ) )  { m_isIsolated_GradientLoose->Fill( isIsoGradientLooseAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_GradientLoose->Fill( -1 ,  eventWeight ); }
    if ( isIsoGradientT1Acc.isAvailable( *muon ) )     { m_isIsolated_GradientT1->Fill( isIsoGradientT1Acc( *muon ) ,  eventWeight ); } else { m_isIsolated_GradientT1->Fill( -1 ,  eventWeight ); }
    if ( isIsoGradientT2Acc.isAvailable( *muon ) )     { m_isIsolated_GradientT2->Fill( isIsoGradientT2Acc( *muon ) ,  eventWeight ); } else { m_isIsolated_GradientT2->Fill( -1 ,  eventWeight ); }
    if ( isIsoMU0p06Acc.isAvailable( *muon ) )          { m_isIsolated_MU0p06->Fill( isIsoMU0p06Acc( *muon ) ,  eventWeight ); } else { m_isIsolated_MU0p06->Fill( -1 ,  eventWeight ); }
    if ( isIsoFixedCutLooseAcc.isAvailable( *muon ) )          { m_isIsolated_FixedCutLoose->Fill( isIsoFixedCutLooseAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_FixedCutLoose->Fill( -1 ,  eventWeight ); }
    if ( isIsoFixedCutTightAcc.isAvailable( *muon ) )          { m_isIsolated_FixedCutTight->Fill( isIsoFixedCutTightAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_FixedCutTight->Fill( -1 ,  eventWeight ); }
    if ( isIsoFixedCutTightTrackOnlyAcc.isAvailable( *muon ) )          { m_isIsolated_FixedCutTightTrackOnly->Fill( isIsoFixedCutTightTrackOnlyAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_FixedCutTightTrackOnly->Fill( -1 ,  eventWeight ); }
    if ( isIsoUserDefinedFixEfficiencyAcc.isAvailable( *muon ) ) { m_isIsolated_UserDefinedFixEfficiency->Fill( isIsoUserDefinedFixEfficiencyAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_UserDefinedFixEfficiency->Fill( -1 ,  eventWeight ); }
    if ( isIsoUserDefinedCutAcc.isAvailable( *muon ) )           { m_isIsolated_UserDefinedCut->Fill( isIsoUserDefinedCutAcc( *muon ) ,  eventWeight ); } else { m_isIsolated_UserDefinedCut->Fill( -1 ,  eventWeight ); }

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
    static SG::AuxElement::Accessor<char> isVeryLooseQAcc ("isVeryLooseQ");
    static SG::AuxElement::Accessor<char> isLooseQAcc ("isLooseQ");
    static SG::AuxElement::Accessor<char> isMediumQAcc ("isMediumQ");
    static SG::AuxElement::Accessor<char> isTightQAcc ("isTightQ");

    if( isVeryLooseQAcc.isAvailable( *muon ) ) { m_isVeryLoose->Fill( static_cast<int>(isVeryLooseQAcc( *muon )),  eventWeight ); } else { m_isVeryLoose->Fill( -1 ,  eventWeight ); }
    if( isLooseQAcc.isAvailable( *muon ) )     { m_isLoose    ->Fill( static_cast<int>(isLooseQAcc    ( *muon )),  eventWeight ); }         else { m_isLoose->Fill( -1 ,  eventWeight ); }
    if( isMediumQAcc.isAvailable( *muon ) )    { m_isMedium   ->Fill( static_cast<int>(isMediumQAcc   ( *muon )),  eventWeight ); }       else { m_isMedium->Fill( -1 ,  eventWeight ); }
    if( isTightQAcc.isAvailable( *muon ) )     { m_isTight    ->Fill( static_cast<int>(isTightQAcc    ( *muon )),  eventWeight ); }         else { m_isTight->Fill( -1 ,  eventWeight ); }

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


    m_isIsolated_LooseTrackOnly           ->Fill( muon->isIsolated_LooseTrackOnly ,  eventWeight );
    m_isIsolated_Loose                    ->Fill( muon->isIsolated_Loose ,  eventWeight );
    m_isIsolated_Tight                    ->Fill( muon->isIsolated_Tight ,  eventWeight );
    m_isIsolated_Gradient                 ->Fill( muon->isIsolated_Gradient ,  eventWeight );
    m_isIsolated_GradientLoose            ->Fill( muon->isIsolated_GradientLoose ,  eventWeight );
    //m_isIsolated_GradientT1               ->Fill( muon->isIsolated_GradientLoose ,  eventWeight );
    //m_isIsolated_GradientT2               ->Fill( muon->isIsoGradientT2Acc ,  eventWeight );
    //m_isIsolated_MU0p06                   ->Fill( muon->isIsoMU0p06Acc ,  eventWeight );
    m_isIsolated_FixedCutLoose            ->Fill( muon->isIsolated_FixedCutLoose ,  eventWeight );
    //m_isIsolated_FixedCutTight            ->Fill( muon->isIsolated_FixedCutTight ,  eventWeight );
    m_isIsolated_FixedCutTightTrackOnly   ->Fill( muon->isIsolated_FixedCutTightTrackOnly ,  eventWeight );
    m_isIsolated_UserDefinedFixEfficiency ->Fill( muon->isIsolated_UserDefinedFixEfficiency ,  eventWeight );
    m_isIsolated_UserDefinedCut           ->Fill( muon->isIsolated_UserDefinedCut ,  eventWeight );

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

    m_isVeryLoose->Fill( muon->  isVeryLoose,  eventWeight );
    m_isLoose    ->Fill( muon->  isLoose    ,  eventWeight );
    m_isMedium   ->Fill( (muon->isMedium + muon->isTight)  ,  eventWeight );
    m_isTight    ->Fill( muon->  isTight    ,  eventWeight );

  }

  return StatusCode::SUCCESS;
}
