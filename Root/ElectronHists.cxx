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

    
    m_isIsolated_LooseTrackOnly           = book(m_name, "isIsolated_LooseTrackOnly"       ,   "isIsolated_LooseTrackOnly", 3, -1.5, 1.5);
    m_isIsolated_Loose                    = book(m_name, "isIsolated_Loose"            ,       "isIsolated_Loose", 3, -1.5, 1.5);
    m_isIsolated_Tight                    = book(m_name, "isIsolated_Tight"             ,      "isIsolated_Tight", 3, -1.5, 1.5);
    m_isIsolated_Gradient                 = book(m_name, "isIsolated_Gradient"      ,          "isIsolated_Gradient", 3, -1.5, 1.5);
    m_isIsolated_GradientLoose            = book(m_name, "isIsolated_GradientLoose",           "isIsolated_GradientLoose", 3, -1.5, 1.5);
    m_isIsolated_FixedCutLoose            = book(m_name, "isIsolated_FixedCutLoose",           "isIsolated_FixedCutLoose", 3, -1.5, 1.5);
    m_isIsolated_FixedCutTight            = book(m_name, "isIsolated_FixedCutTight",           "isIsolated_FixedCutTight", 3, -1.5, 1.5);
    m_isIsolated_FixedCutTightTrackOnly   = book(m_name, "isIsolated_FixedCutTightTrackOnly"  ,"isIsolated_FixedCutTightTrackOnly", 3, -1.5, 1.5);
    m_isIsolated_UserDefinedFixEfficiency = book(m_name, "isIsolated_UserDefinedFixEfficiency","isIsolated_UserDefinedFixEfficiency", 3, -1.5, 1.5);
    m_isIsolated_UserDefinedCut           = book(m_name, "isIsolated_UserDefinedCut",          "isIsolated_UserDefinedCut", 3, -1.5, 1.5);

    
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
    
    static SG::AuxElement::Accessor<char> isIsoLooseTrackOnlyAcc ("isIsolated_LooseTrackOnly");
    static SG::AuxElement::Accessor<char> isIsoLooseAcc ("isIsolated_Loose");
    static SG::AuxElement::Accessor<char> isIsoTightAcc ("isIsolated_Tight");
    static SG::AuxElement::Accessor<char> isIsoGradientAcc ("isIsolated_Gradient");
    static SG::AuxElement::Accessor<char> isIsoGradientLooseAcc ("isIsolated_GradientLoose");
    static SG::AuxElement::Accessor<char> isIsoFixedCutLooseAcc ("isIsolated_FixedCutLoose");
    static SG::AuxElement::Accessor<char> isIsoFixedCutTightAcc ("isIsolated_FixedCutTight");
    static SG::AuxElement::Accessor<char> isIsoFixedCutTightTrackOnlyAcc ("isIsolated_FixedCutTightTrackOnly");
    static SG::AuxElement::Accessor<char> isIsoUserDefinedFixEfficiencyAcc ("isIsolated_UserDefinedFixEfficiency");
    static SG::AuxElement::Accessor<char> isIsoUserDefinedCutAcc ("isIsolated_UserDefinedCut");

    if (isIsoLooseTrackOnlyAcc.isAvailable(*electron)) {m_isIsolated_LooseTrackOnly->Fill(isIsoLooseTrackOnlyAcc(*electron), eventWeight);} else {m_isIsolated_LooseTrackOnly->Fill(-1,eventWeight);}
    if (isIsoLooseAcc.isAvailable(*electron) )         {m_isIsolated_Loose->Fill( isIsoLooseAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_Loose->Fill( -1 ,  eventWeight ); }
    if (isIsoTightAcc.isAvailable( *electron ) )       { m_isIsolated_Tight->Fill( isIsoTightAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_Tight->Fill( -1 ,  eventWeight ); }
    if (isIsoGradientAcc.isAvailable( *electron ) )       { m_isIsolated_Gradient->Fill( isIsoGradientAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_Gradient->Fill( -1 ,  eventWeight ); }
    if (isIsoGradientLooseAcc.isAvailable(*electron))  { m_isIsolated_GradientLoose->Fill( isIsoGradientLooseAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_GradientLoose->Fill( -1 ,  eventWeight ); }
    if (isIsoFixedCutLooseAcc.isAvailable(*electron))          { m_isIsolated_FixedCutLoose->Fill( isIsoFixedCutLooseAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_FixedCutLoose->Fill( -1 ,  eventWeight ); }
    if (isIsoFixedCutTightAcc.isAvailable(*electron))          { m_isIsolated_FixedCutTight->Fill( isIsoFixedCutTightAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_FixedCutTight->Fill( -1 ,  eventWeight ); }
    if (isIsoFixedCutTightTrackOnlyAcc.isAvailable(*electron))          { m_isIsolated_FixedCutTightTrackOnly->Fill( isIsoFixedCutTightTrackOnlyAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_FixedCutTightTrackOnly->Fill( -1 ,  eventWeight ); }
    if ( isIsoUserDefinedFixEfficiencyAcc.isAvailable(*electron)) {m_isIsolated_UserDefinedFixEfficiency->Fill(isIsoUserDefinedFixEfficiencyAcc(*electron), eventWeight);} else {m_isIsolated_UserDefinedFixEfficiency->Fill(-1,eventWeight);}
    if ( isIsoUserDefinedCutAcc.isAvailable( *electron ) )           { m_isIsolated_UserDefinedCut->Fill( isIsoUserDefinedCutAcc( *electron ) ,  eventWeight ); } else { m_isIsolated_UserDefinedCut->Fill( -1 ,  eventWeight ); }

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

  if(m_debug) std::cout << "ElectronHists: in execute " <<std::endl;

  const xAH::Electron* elec=dynamic_cast<const xAH::Electron*>(particle);
  if(elec==0)
    {
      ::Error( "ElctronHists::execute()", XAOD_MESSAGE( "Cannot convert IParticle to Electron" ));
      return StatusCode::FAILURE;
    }

  m_isIsolated_LooseTrackOnly           ->Fill( elec->isIsolated_LooseTrackOnly ,  eventWeight );
  m_isIsolated_Loose                    ->Fill( elec->isIsolated_Loose ,  eventWeight ); 
  m_isIsolated_Tight                    ->Fill( elec->isIsolated_Tight ,  eventWeight ); 
  m_isIsolated_Gradient                 ->Fill( elec->isIsolated_Gradient ,  eventWeight );
  m_isIsolated_GradientLoose            ->Fill( elec->isIsolated_GradientLoose ,  eventWeight );
  //m_isIsolated_GradientT1               ->Fill( elec->isIsolated_GradientLoose ,  eventWeight ); 
  //m_isIsolated_GradientT2               ->Fill( elec->isIsoGradientT2Acc ,  eventWeight ); 
  //m_isIsolated_MU0p06                   ->Fill( elec->isIsoMU0p06Acc ,  eventWeight ); 
  m_isIsolated_FixedCutLoose            ->Fill( elec->isIsolated_FixedCutLoose ,  eventWeight ); 
  //m_isIsolated_FixedCutTight            ->Fill( elec->isIsolated_FixedCutTight ,  eventWeight ); 
  m_isIsolated_FixedCutTightTrackOnly   ->Fill( elec->isIsolated_FixedCutTightTrackOnly ,  eventWeight ); 
  m_isIsolated_UserDefinedFixEfficiency ->Fill( elec->isIsolated_UserDefinedFixEfficiency ,  eventWeight ); 
  m_isIsolated_UserDefinedCut           ->Fill( elec->isIsolated_UserDefinedCut ,  eventWeight ); 

  
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
