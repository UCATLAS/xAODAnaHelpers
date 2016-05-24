/********************************************************
 *
 * Muon Histogramming
 *
 * J. Alison (john.alison@cern.ch)
 *
 *******************************************************/


#include <xAODAnaHelpers/MuonHists.h>
#include <sstream>

#include "xAODAnaHelpers/tools/ReturnCheck.h"

using std::vector;

MuonHists :: MuonHists (std::string name, std::string detailStr) :
  IParticleHists(name, detailStr, "muon", "muon"),
  m_infoSwitch(new HelperClasses::MuonInfoSwitch(m_detailStr))
{ }

MuonHists :: ~MuonHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode MuonHists::initialize() {
  RETURN_CHECK("IParticleHists::initialize()", IParticleHists::initialize(), "");

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

    m_ptcone20     = book(m_name, "ptcone20",     "ptcone20",     100, 0, 10);
    m_ptcone30     = book(m_name, "ptcone30",     "ptcone30",     100, 0, 10);
    m_ptcone40     = book(m_name, "ptcone40",     "ptcone40",     100, 0, 10);
    m_ptvarcone20  = book(m_name, "ptvarcone20",  "ptvarcone20",  100, 0, 10);
    m_ptvarcone30  = book(m_name, "ptvarcone30",  "ptvarcone30",  100, 0, 10);
    m_ptvarcone40  = book(m_name, "ptvarcone40",  "ptvarcone40",  100, 0, 10);
    m_topoetcone20 = book(m_name, "topoetcone20", "topoetcone20", 100, 0, 10);
    m_topoetcone30 = book(m_name, "topoetcone30", "topoetcone30", 100, 0, 10);
    m_topoetcone40 = book(m_name, "topoetcone40", "topoetcone40", 100, 0, 10);

  }


  // quality
  if(m_infoSwitch->m_isolation){
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
  RETURN_CHECK("IParticleHists::execute()", IParticleHists::execute(particle, eventWeight, eventInfo), "");

  if(m_debug) std::cout << "MuonHists: in execute " <<std::endl;

  const xAOD::Muon* muon=dynamic_cast<const xAOD::Muon*>(particle);
  if(muon==0)
    {
      ::Error( "MuonHists::execute()", XAOD_MESSAGE( "Cannot convert IParticle to Muon" ));
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

    m_ptcone20     ->Fill( muon->isolation( xAOD::Iso::ptcone20 )     / 1e3,  eventWeight );
    m_ptcone30     ->Fill( muon->isolation( xAOD::Iso::ptcone30 )     / 1e3,  eventWeight );
    m_ptcone40     ->Fill( muon->isolation( xAOD::Iso::ptcone40 )     / 1e3,  eventWeight );
    m_ptvarcone20  ->Fill( muon->isolation( xAOD::Iso::ptvarcone20 )  / 1e3,  eventWeight );
    m_ptvarcone30  ->Fill( muon->isolation( xAOD::Iso::ptvarcone30 )  / 1e3,  eventWeight );
    m_ptvarcone40  ->Fill( muon->isolation( xAOD::Iso::ptvarcone40 )  / 1e3,  eventWeight );
    m_topoetcone20 ->Fill( muon->isolation( xAOD::Iso::topoetcone20 ) / 1e3,  eventWeight );
    m_topoetcone30 ->Fill( muon->isolation( xAOD::Iso::topoetcone30 ) / 1e3,  eventWeight );
    m_topoetcone40 ->Fill( muon->isolation( xAOD::Iso::topoetcone40 ) / 1e3,  eventWeight );

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

