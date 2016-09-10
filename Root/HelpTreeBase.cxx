// c++ include(s):
#include <iostream>

// EDM include(s):
#include "xAODBTagging/BTagging.h"
#include "xAODTracking/TrackParticle.h"
#include "xAODTracking/TrackSummaryAccessors_v1.h"
#include "xAODJet/JetConstituentVector.h"
#include "xAODPrimitives/IsolationType.h"


#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelpTreeBase.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>


#include "AsgTools/StatusCode.h"

using std::vector;

// needed? should it be here?
#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif

HelpTreeBase::HelpTreeBase(xAOD::TEvent* event, TTree* tree, TFile* file, const float units, bool debug, bool DC14, xAOD::TStore* store):
  m_trigInfoSwitch(nullptr),
  m_elInfoSwitch(nullptr),
  m_phInfoSwitch(nullptr),
  m_truthInfoSwitch(nullptr),
  m_fatJetInfoSwitch(nullptr),
  m_tauInfoSwitch(nullptr),
  m_metInfoSwitch(nullptr),
  m_trigConfTool(nullptr),
  m_trigDecTool(nullptr),
  m_eventInfo(nullptr)
{

  m_units = units;
  m_debug = debug;
  m_DC14  = DC14;
  m_tree = tree;
  m_tree->SetDirectory( file );
  m_event = event;
  m_store = store;
  Info("HelpTreeBase()", "HelpTreeBase setup");

  // turn things off it this is data...since TStore is not a needed input
  // default isMC to true so more is added to the tree than less
  const xAOD::EventInfo* eventInfo(nullptr);
  HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, false);
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

}

HelpTreeBase::~HelpTreeBase() {

    //delete all the info switches that have been built earlier on
    //event
    delete m_eventInfo;

    //trig
    delete m_trigInfoSwitch;

    //muon
    for (auto muon: m_muons)
      delete muon.second;

    //el
    delete m_elInfoSwitch;

    //ph
    delete m_phInfoSwitch;

    //truth
    delete m_truthInfoSwitch;

    //fatjet
    delete m_fatJetInfoSwitch;

    //tau
    delete m_tauInfoSwitch;

    //met
    delete m_metInfoSwitch;

    //jet
    for (auto jet: m_jets)
      delete jet.second;

}


HelpTreeBase::HelpTreeBase(TTree* tree, TFile* file, xAOD::TEvent* event, xAOD::TStore* store, const float units, bool debug, bool DC14):
  HelpTreeBase(event, tree, file, units, debug, DC14, store)
{
  // use the other constructor for everything
}


void HelpTreeBase::Fill() {
  m_tree->Fill();
}

/*********************
 *
 *   EVENT
 *
 ********************/

void HelpTreeBase::AddEvent( const std::string detailStr ) {

  if(m_debug)  Info("AddEvent()", "Adding event variables: %s", detailStr.c_str());

  m_eventInfo       = new xAH::EventInfo(detailStr, m_units, m_isMC);
  m_eventInfo -> setBranches(m_tree);
  this->AddEventUser();
}

void HelpTreeBase::FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* /*event*/ ) {

  this->ClearEvent();
  this->ClearEventUser();

  m_eventInfo->FillEvent(eventInfo, m_event);

  this->FillEventUser(eventInfo);
}

/*********************
 *
 *   TRIGGER
 *
 ********************/
void HelpTreeBase::AddTrigger( const std::string detailStr ) {

  if(m_debug) Info("AddTrigger()", "Adding trigger variables: %s", detailStr.c_str());

  m_trigInfoSwitch = new HelperClasses::TriggerInfoSwitch( detailStr );

  // Add these basic branches
  if ( m_trigInfoSwitch->m_basic ) {
    m_tree->Branch("passL1",          &m_passL1,       "passL1/I"      );
    m_tree->Branch("passHLT",         &m_passHLT,      "passHLT/I"     );
  }

  // Detailed trigger infoformation related to the menu (might be useful)
  // This is useful for using this database: https://atlas-trigconf.cern.ch/
  if ( m_trigInfoSwitch->m_menuKeys ) {
    m_tree->Branch("masterKey",       &m_masterKey,    "masterKey/I"         );
    m_tree->Branch("lvl1PrescaleKey", &m_L1PSKey,      "lvl1PrescaleKey/I"   );
    m_tree->Branch("hltPrescaleKey",  &m_HLTPSKey,     "hltPrescaleKey/I"    );
  }

  // Trigger Decision for each and every trigger in a vector
  if ( m_trigInfoSwitch->m_passTriggers ) {
    // vector of strings for trigger names which fired
    m_tree->Branch("passedTriggers",       &m_passTriggers        );
    m_tree->Branch("triggerPrescales",     &m_triggerPrescales    );
  }

  if ( m_trigInfoSwitch->m_passTrigBits ) {
    m_tree->Branch("isPassBits",           &m_isPassBits          );
    m_tree->Branch("isPassBitsNames",      &m_isPassBitsNames     );
  }

  //this->AddTriggerUser();
}

// Fill the information in the trigger branches
void HelpTreeBase::FillTrigger( const xAOD::EventInfo* eventInfo ) {

  if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Filling trigger info"); }

  // Clear previous events
  this->ClearTrigger();
  this->ClearTriggerUser();

  // Grab the global pass information from the TrigDecisionTool
  if ( m_trigInfoSwitch->m_basic ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Switch: m_trigInfoSwitch->m_basic"); }

    static SG::AuxElement::ConstAccessor< int > passAny("passAny");
    static SG::AuxElement::ConstAccessor< int > passL1("passL1");
    if( passL1.isAvailable( *eventInfo ) ) { m_passL1 = passL1( *eventInfo ); }
    else { m_passL1 = -1; }
    static SG::AuxElement::ConstAccessor< int > passHLT("passHLT");
    if( passHLT.isAvailable( *eventInfo ) ) { m_passHLT = passHLT( *eventInfo ); }
    else { m_passHLT = -1; }

  }

  // If detailed menu information about the configuration keys, turn this on.
  // This is useful for using this database: https://atlas-trigconf.cern.ch/
  if ( m_trigInfoSwitch->m_menuKeys ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Switch: m_trigInfoSwitch->m_menuKeys"); }

    static SG::AuxElement::ConstAccessor< int > masterKey("masterKey");
    if( masterKey.isAvailable( *eventInfo ) ) { m_masterKey = masterKey( *eventInfo ); }
    else { m_masterKey = -999; }
    static SG::AuxElement::ConstAccessor< int > L1PSKey("L1PSKey");
    if( L1PSKey.isAvailable( *eventInfo ) ) { m_L1PSKey = L1PSKey( *eventInfo ); }
    else { m_L1PSKey = -999; }
    static SG::AuxElement::ConstAccessor< int > HLTPSKey("HLTPSKey");
    if( HLTPSKey.isAvailable( *eventInfo ) ) { m_HLTPSKey = HLTPSKey( *eventInfo ); }
    else { m_HLTPSKey = -999; }

  }

  // If detailed information about each and every trigger is desired
  // save a vector of strings holding passing decisions
  if ( m_trigInfoSwitch->m_passTriggers ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTrigger()", "Switch: m_trigInfoSwitch->m_passTriggers"); }
    static SG::AuxElement::ConstAccessor< std::vector< std::string > > passTrigs("passTriggers");
    if( passTrigs.isAvailable( *eventInfo ) ) { m_passTriggers = passTrigs( *eventInfo ); }

    static SG::AuxElement::ConstAccessor< std::vector< float > > trigPrescales("triggerPrescales");
    if( trigPrescales.isAvailable( *eventInfo ) ) { m_triggerPrescales = trigPrescales( *eventInfo ); }

  }

  if ( m_trigInfoSwitch->m_passTrigBits ) {

    static SG::AuxElement::ConstAccessor< std::vector< unsigned int > > isPassBits("isPassedBits");
    if( isPassBits.isAvailable( *eventInfo ) ) { m_isPassBits = isPassBits( *eventInfo ); }

    static SG::AuxElement::ConstAccessor< std::vector< std::string > > isPassBitsNames("isPassedBitsNames");
    if( isPassBitsNames.isAvailable( *eventInfo ) ) { m_isPassBitsNames = isPassBitsNames( *eventInfo ); }

  }

}

// Clear Trigger
void HelpTreeBase::ClearTrigger() {

  m_passL1  = -999;
  m_passHLT = -999;

  m_masterKey = 0;
  m_L1PSKey   = 0;
  m_HLTPSKey  = 0;

  m_passTriggers.clear();
  m_triggerPrescales.clear();
  m_isPassBits.clear();
  m_isPassBitsNames.clear();

}

/*********************
 *
 *   JET TRIGGER
 *
 ********************/

/* TODO: jet trigger */
//CD: is this useful at all?
void HelpTreeBase::AddJetTrigger( const std::string detailStr )
{
  if ( m_debug )  Info("AddJetTrigger()", "Adding jet trigger variables: %s", detailStr.c_str());
}
void HelpTreeBase::FillJetTrigger(  ) { }
void HelpTreeBase::ClearJetTrigger(  ) { }


/*********************
 *
 *   MUONS
 *
 ********************/

void HelpTreeBase::AddMuons(const std::string detailStr, const std::string muonName) {

  if ( m_debug )  Info("AddMuons()", "Adding muon variables: %s", detailStr.c_str());

  m_muons[muonName] = new xAH::MuonContainer(muonName, detailStr, m_units, m_isMC);

  xAH::MuonContainer* thisMuon = m_muons[muonName];

  thisMuon->setBranches(m_tree);
  this->AddMuonsUser();
}

void HelpTreeBase::FillMuons( const xAOD::MuonContainer* muons, const xAOD::Vertex* primaryVertex, const std::string muonName ) {

  this->ClearMuons(muonName);

  for( auto muon_itr : *muons ) {
    this->FillMuon(muon_itr, primaryVertex, muonName);
  }

}

void HelpTreeBase::FillMuon( const xAOD::Muon* muon, const xAOD::Vertex* primaryVertex, const std::string muonName ) {

  xAH::MuonContainer* thisMuon = m_muons[muonName];

  thisMuon->FillMuon(muon, primaryVertex);

  this->FillMuonsUser(muon, muonName);

  return;
}

void HelpTreeBase::ClearMuons(const std::string muonName) {

  xAH::MuonContainer* thisMuon = m_muons[muonName];
  thisMuon->clear();

  this->ClearMuonsUser();

}

/*********************
 *
 *   ELECTRONS
 *
 ********************/

void HelpTreeBase::AddElectrons(const std::string detailStr) {

  if(m_debug)  Info("AddElectrons()", "Adding electron variables: %s", detailStr.c_str());

  m_elInfoSwitch = new HelperClasses::ElectronInfoSwitch( detailStr );

  // always
  m_tree->Branch("nel",    &m_nel,"nel/I");

  if ( m_elInfoSwitch->m_kinematic ) {
    m_tree->Branch("el_pt",  &m_el_pt);
    m_tree->Branch("el_phi", &m_el_phi);
    m_tree->Branch("el_eta", &m_el_eta);
    m_tree->Branch("el_m",   &m_el_m);
    m_tree->Branch("el_caloCluster_eta", &m_el_caloCluster_eta);
  }

  if ( m_elInfoSwitch->m_trigger ){
    // this is true if there's a match for at least one trigger chain
    m_tree->Branch("el_isTrigMatched", &m_el_isTrigMatched);
    // a vector of trigger match decision for each electron trigger chain
    m_tree->Branch("el_isTrigMatchedToChain", &m_el_isTrigMatchedToChain);
    // a vector of strings with the electron trigger chain names - 1:1 correspondence w/ vector above
    m_tree->Branch("el_listTrigChains", &m_el_listTrigChains);
  }

  if ( m_elInfoSwitch->m_isolation ) {
    m_tree->Branch("el_isIsolated_LooseTrackOnly", &m_el_isIsolated_LooseTrackOnly);
    m_tree->Branch("el_isIsolated_Loose",	   &m_el_isIsolated_Loose);
    m_tree->Branch("el_isIsolated_Tight",	   &m_el_isIsolated_Tight);
    m_tree->Branch("el_isIsolated_Gradient",	   &m_el_isIsolated_Gradient);
    m_tree->Branch("el_isIsolated_GradientLoose",  &m_el_isIsolated_GradientLoose);
    m_tree->Branch("el_isIsolated_FixedCutLoose",	   &m_el_isIsolated_FixedCutLoose);
    m_tree->Branch("el_isIsolated_FixedCutTight",	   &m_el_isIsolated_FixedCutTight);
    m_tree->Branch("el_isIsolated_FixedCutTightTrackOnly", &m_el_isIsolated_FixedCutTightTrackOnly);
    m_tree->Branch("el_isIsolated_UserDefinedFixEfficiency",    &m_el_isIsolated_UserDefinedFixEfficiency);
    m_tree->Branch("el_isIsolated_UserDefinedCut",              &m_el_isIsolated_UserDefinedCut);
    m_tree->Branch("el_etcone20",	  &m_el_etcone20);
    m_tree->Branch("el_ptcone20",	  &m_el_ptcone20);
    m_tree->Branch("el_ptcone30",	  &m_el_ptcone30);
    m_tree->Branch("el_ptcone40",	  &m_el_ptcone40);
    m_tree->Branch("el_ptvarcone20",	  &m_el_ptvarcone20);
    m_tree->Branch("el_ptvarcone30",	  &m_el_ptvarcone30);
    m_tree->Branch("el_ptvarcone40",	  &m_el_ptvarcone40);
    m_tree->Branch("el_topoetcone20",     &m_el_topoetcone20);
    m_tree->Branch("el_topoetcone30",     &m_el_topoetcone30);
    m_tree->Branch("el_topoetcone40",     &m_el_topoetcone40);
  }

  if ( m_elInfoSwitch->m_PID ) {
    m_tree->Branch("nel_LHLoose",      &m_nel_LHLoose);
    m_tree->Branch("el_LHLoose",       &m_el_LHLoose);
    m_tree->Branch("nel_LHMedium",     &m_nel_LHMedium);
    m_tree->Branch("el_LHMedium",      &m_el_LHMedium);
    m_tree->Branch("nel_LHTight",      &m_nel_LHTight);
    m_tree->Branch("el_LHTight",       &m_el_LHTight);
    m_tree->Branch("nel_IsEMLoose",    &m_nel_IsEMLoose);
    m_tree->Branch("el_IsEMLoose",     &m_el_IsEMLoose);
    m_tree->Branch("nel_IsEMMedium",   &m_nel_IsEMMedium);
    m_tree->Branch("el_IsEMMedium",    &m_el_IsEMMedium);
    m_tree->Branch("nel_IsEMTight",    &m_nel_IsEMTight);
    m_tree->Branch("el_IsEMTight",     &m_el_IsEMTight);
  }

  if ( m_elInfoSwitch->m_effSF && m_isMC ) {
    m_tree->Branch("el_RecoEff_SF"  ,		 &m_el_RecoEff_SF  );
    m_tree->Branch("el_TrigEff_SF_LHLooseAndBLayer"  ,	 &m_el_TrigEff_SF_LHLooseAndBLayer  );
    m_tree->Branch("el_TrigEff_SF_LHTight"  ,&m_el_TrigEff_SF_LHTight  );
    m_tree->Branch("el_TrigMCEff_LHLooseAndBLayer"   ,&m_el_TrigMCEff_LHLooseAndBLayer  );
    m_tree->Branch("el_TrigMCEff_LHTight"   ,&m_el_TrigMCEff_LHTight  );
    m_tree->Branch("el_IsoEff_SF_Loose"  , &m_el_IsoEff_SF_Loose );
    m_tree->Branch("el_IsoEff_SF_FixedCutTight"  , &m_el_IsoEff_SF_FixedCutTight );
    m_tree->Branch("el_PIDEff_SF_LHLooseAndBLayer",  &m_el_PIDEff_SF_LHLooseAndBLayer);
    m_tree->Branch("el_PIDEff_SF_LHLoose",       &m_el_PIDEff_SF_LHLoose);
    m_tree->Branch("el_PIDEff_SF_LHMedium",      &m_el_PIDEff_SF_LHMedium);
    m_tree->Branch("el_PIDEff_SF_LHTight",       &m_el_PIDEff_SF_LHTight);
  }

  if ( m_elInfoSwitch->m_trackparams ) {
    m_tree->Branch("el_trkd0",      &m_el_trkd0);
    m_tree->Branch("el_trkd0sig",   &m_el_trkd0sig);
    m_tree->Branch("el_trkz0",      &m_el_trkz0);
    m_tree->Branch("el_trkz0sintheta",  &m_el_trkz0sintheta);
    m_tree->Branch("el_trkphi0",    &m_el_trkphi0);
    m_tree->Branch("el_trktheta",   &m_el_trktheta);
    m_tree->Branch("el_trkcharge",  &m_el_trkcharge);
    m_tree->Branch("el_trkqOverP",  &m_el_trkqOverP);
  }

  if ( m_elInfoSwitch->m_trackhitcont ) {
    m_tree->Branch("el_trknSiHits",    &m_el_trknSiHits);
    m_tree->Branch("el_trknPixHits",   &m_el_trknPixHits);
    m_tree->Branch("el_trknPixHoles",  &m_el_trknPixHoles);
    m_tree->Branch("el_trknSCTHits",   &m_el_trknSCTHits);
    m_tree->Branch("el_trknSCTHoles",  &m_el_trknSCTHoles);
    m_tree->Branch("el_trknTRTHits",   &m_el_trknTRTHits);
    m_tree->Branch("el_trknTRTHoles",  &m_el_trknTRTHoles);
    m_tree->Branch("el_trknBLayerHits",&m_el_trknBLayerHits);
    if ( !m_DC14 ) {
      m_tree->Branch("el_trknInnermostPixLayHits",  &m_el_trknInnermostPixLayHits);
      m_tree->Branch("el_trkPixdEdX",    &m_el_trkPixdEdX);
    }
  }

  this->AddElectronsUser();
}

void HelpTreeBase::FillElectrons( const xAOD::ElectronContainer* electrons, const xAOD::Vertex* primaryVertex ) {

  this->ClearElectrons();
  this->ClearElectronsUser();

  m_nel = 0;
  m_nel_LHLoose = 0;
  m_nel_LHMedium = 0;
  m_nel_LHTight = 0;
  m_nel_IsEMLoose = 0;
  m_nel_IsEMMedium = 0;
  m_nel_IsEMTight = 0;

  for ( auto el_itr : *(electrons) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillElectrons()", "Filling electron w/ pT = %2f", el_itr->pt() / m_units ); }

    const xAOD::TrackParticle* trk = el_itr->trackParticle();

    if ( m_elInfoSwitch->m_kinematic ) {
      m_el_pt.push_back ( (el_itr)->pt() / m_units );
      m_el_eta.push_back( (el_itr)->eta() );
      m_el_phi.push_back( (el_itr)->phi() );
      m_el_m.push_back  ( (el_itr)->m() / m_units );

      float calo_eta   = ( el_itr->caloCluster() ) ? el_itr->caloCluster()->etaBE(2) : -999.0;
      m_el_caloCluster_eta.push_back( calo_eta );

    }

    if ( m_elInfoSwitch->m_trigger ) {

      // retrieve map<string,char> w/ <chain,isMatched>
      //
      static SG::AuxElement::Accessor< std::map<std::string,char> > isTrigMatchedMapElAcc("isTrigMatchedMapEl");

      std::vector<int> matches;

      if ( isTrigMatchedMapElAcc.isAvailable( *el_itr ) ) {
	 // loop over map and fill branches
	 //
	 for ( auto const &it : (isTrigMatchedMapElAcc( *el_itr )) ) {
  	   matches.push_back( static_cast<int>(it.second) );
	   m_el_listTrigChains.push_back( it.first );
	 }
       } else {
	 matches.push_back( -1 );
	 m_el_listTrigChains.push_back("NONE");
       }

       m_el_isTrigMatchedToChain.push_back(matches);

       // if at least one match among the chains is found, say this electron is trigger matched
       if ( std::find(matches.begin(), matches.end(), 1) != matches.end() ) { m_el_isTrigMatched.push_back(1); }
       else { m_el_isTrigMatched.push_back(0); }

    }

    if ( m_elInfoSwitch->m_isolation ) {

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

      if ( isIsoLooseTrackOnlyAcc.isAvailable( *el_itr ) ) { m_el_isIsolated_LooseTrackOnly.push_back( isIsoLooseTrackOnlyAcc( *el_itr ) ); } else { m_el_isIsolated_LooseTrackOnly.push_back( -1 ); }
      if ( isIsoLooseAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_Loose.push_back( isIsoLooseAcc( *el_itr ) ); } else { m_el_isIsolated_Loose.push_back( -1 ); }
      if ( isIsoTightAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_Tight.push_back( isIsoTightAcc( *el_itr ) ); } else { m_el_isIsolated_Tight.push_back( -1 ); }
      if ( isIsoGradientAcc.isAvailable( *el_itr ) )       { m_el_isIsolated_Gradient.push_back( isIsoGradientAcc( *el_itr ) ); } else { m_el_isIsolated_Gradient.push_back( -1 ); }
      if ( isIsoGradientLooseAcc.isAvailable( *el_itr ) )  { m_el_isIsolated_GradientLoose.push_back( isIsoGradientLooseAcc( *el_itr ) ); } else { m_el_isIsolated_GradientLoose.push_back( -1 ); }
      if ( isIsoFixedCutLooseAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_FixedCutLoose.push_back( isIsoFixedCutLooseAcc( *el_itr ) ); } else { m_el_isIsolated_FixedCutLoose.push_back( -1 ); }
      if ( isIsoFixedCutTightAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_FixedCutTight.push_back( isIsoFixedCutTightAcc( *el_itr ) ); } else { m_el_isIsolated_FixedCutTight.push_back( -1 ); }
      if ( isIsoFixedCutTightTrackOnlyAcc.isAvailable( *el_itr ) )          { m_el_isIsolated_FixedCutTightTrackOnly.push_back( isIsoFixedCutTightTrackOnlyAcc( *el_itr ) ); } else { m_el_isIsolated_FixedCutTightTrackOnly.push_back( -1 ); }
      if ( isIsoUserDefinedFixEfficiencyAcc.isAvailable( *el_itr ) ) { m_el_isIsolated_UserDefinedFixEfficiency.push_back( isIsoUserDefinedFixEfficiencyAcc( *el_itr ) ); } else { m_el_isIsolated_UserDefinedFixEfficiency.push_back( -1 ); }
      if ( isIsoUserDefinedCutAcc.isAvailable( *el_itr ) )           { m_el_isIsolated_UserDefinedCut.push_back( isIsoUserDefinedCutAcc( *el_itr ) ); } else { m_el_isIsolated_UserDefinedCut.push_back( -1 ); }

      m_el_etcone20.push_back( el_itr->isolation( xAOD::Iso::etcone20 ) );
      m_el_ptcone20.push_back( el_itr->isolation( xAOD::Iso::ptcone20 ) );
      m_el_ptcone30.push_back( el_itr->isolation( xAOD::Iso::ptcone30 ) );
      m_el_ptcone40.push_back( el_itr->isolation( xAOD::Iso::ptcone40 ) );
      m_el_ptvarcone20.push_back( el_itr->isolation( xAOD::Iso::ptvarcone20 ) );
      m_el_ptvarcone30.push_back( el_itr->isolation( xAOD::Iso::ptvarcone30 ) );
      m_el_ptvarcone40.push_back( el_itr->isolation( xAOD::Iso::ptvarcone40 ) );
      m_el_topoetcone20.push_back( el_itr->isolation( xAOD::Iso::topoetcone20 ) );
      m_el_topoetcone30.push_back( el_itr->isolation( xAOD::Iso::topoetcone30 ) );
      m_el_topoetcone40.push_back( el_itr->isolation( xAOD::Iso::topoetcone40 ) );
    }

    if ( m_elInfoSwitch->m_PID ) {

      static SG::AuxElement::Accessor<char> LHLooseAcc ("LHLoose");
      static SG::AuxElement::Accessor<char> LHMediumAcc ("LHMedium");
      static SG::AuxElement::Accessor<char> LHTightAcc ("LHTight");

      static SG::AuxElement::Accessor<char> EMLooseAcc ("Loose");
      static SG::AuxElement::Accessor<char> EMMediumAcc ("Medium");
      static SG::AuxElement::Accessor<char> EMTightAcc ("Tight");

      if ( LHLooseAcc.isAvailable( *el_itr ) ) {
        m_el_LHLoose.push_back( LHLooseAcc( *el_itr ) );
        if ( LHLooseAcc( *el_itr ) == 1 ) { ++m_nel_LHLoose; }
      }  else { m_el_LHLoose.push_back( -1 ); }
      if ( LHMediumAcc.isAvailable( *el_itr ) ) {
        m_el_LHMedium.push_back( LHMediumAcc( *el_itr ) );
        if ( LHMediumAcc( *el_itr ) == 1 ) { ++m_nel_LHMedium; }
      }  else { m_el_LHMedium.push_back( -1 ); }
      if ( LHTightAcc.isAvailable( *el_itr ) ) {
        m_el_LHTight.push_back( LHTightAcc( *el_itr ) );
        if ( LHTightAcc( *el_itr ) == 1 ) { ++m_nel_LHTight; }
      } else { m_el_LHTight.push_back( -1 ); }

      if ( EMLooseAcc.isAvailable( *el_itr ) ) {
        m_el_IsEMLoose.push_back( EMLooseAcc( *el_itr ) );
        if ( EMLooseAcc( *el_itr ) == 1 ) { ++m_nel_IsEMLoose; }
      } else { m_el_IsEMLoose.push_back( -1 ); }
      if ( EMMediumAcc.isAvailable( *el_itr ) ) {
        m_el_IsEMMedium.push_back( EMMediumAcc( *el_itr ) );
	if ( EMMediumAcc( *el_itr ) == 1 ) { ++m_nel_IsEMMedium; }
      } else { m_el_IsEMMedium.push_back( -1 ); }
      if ( EMTightAcc.isAvailable( *el_itr ) ) {
        m_el_IsEMTight.push_back( EMTightAcc( *el_itr ) );
	if ( EMTightAcc( *el_itr ) == 1 ) { ++m_nel_IsEMTight; }
      } else { m_el_IsEMTight.push_back( -1 ); }

    }

    if ( m_elInfoSwitch->m_trackparams ) {
      if ( trk ) {
        //
	// NB.:
	// All track parameters are calculated at the perigee, i.e., the point of closest approach to the origin of some r.f. (which in RunII is NOT the ATLAS detector r.f!).
	// The reference  frame is chosen to be a system centered in the beamspot position, with z axis parallel to the beam line.
	// Remember that the beamspot size ( of O(10 micrometers) in the transverse plane) is << average vertex transverse position resolution ( O(60-80 micrometers) )
	// The coordinates of this r.f. wrt. the ATLAS system origin are returned by means of vx(), vy(), vz()
	//
        m_el_trkd0.push_back( trk->d0() );
        static SG::AuxElement::Accessor<float> d0SigAcc ("d0sig");
        float d0_significance =  ( d0SigAcc.isAvailable( *el_itr ) ) ? fabs( d0SigAcc( *el_itr ) ) : -1.0;
	m_el_trkd0sig.push_back( d0_significance );
        m_el_trkz0.push_back( trk->z0()  - ( primaryVertex->z() - trk->vz() ) );
        static SG::AuxElement::Accessor<float> z0sinthetaAcc("z0sintheta");
        float z0sintheta =  ( z0sinthetaAcc.isAvailable( *el_itr ) ) ? z0sinthetaAcc( *el_itr ) : -999.0;
        m_el_trkz0sintheta.push_back( z0sintheta );
        m_el_trkphi0.push_back( trk->phi0() );
        m_el_trktheta.push_back( trk->theta() );
        m_el_trkcharge.push_back( trk->charge() );
        m_el_trkqOverP.push_back( trk->qOverP() );
      } else {
        m_el_trkd0.push_back( -999.0 );
        m_el_trkd0sig.push_back( -1.0 );
        m_el_trkz0.push_back( -999.0 );
        m_el_trkz0sintheta.push_back( -999.0 );
        m_el_trkphi0.push_back( -999.0 );
        m_el_trktheta.push_back( -999.0 );
        m_el_trkcharge.push_back( -999.0 );
        m_el_trkqOverP.push_back( -999.0 );
      }
    }

    if ( m_elInfoSwitch->m_trackhitcont ) {
      uint8_t nPixHits(-1), nPixHoles(-1), nSCTHits(-1), nSCTHoles(-1), nTRTHits(-1), nTRTHoles(-1), nBLayerHits(-1), nInnermostPixLayHits(-1);
      float pixdEdX(-1.0);
      if ( trk ) {
        trk->summaryValue( nPixHits,  xAOD::numberOfPixelHits );
        trk->summaryValue( nPixHoles, xAOD::numberOfPixelHoles );
        trk->summaryValue( nSCTHits,  xAOD::numberOfSCTHits );
        trk->summaryValue( nSCTHoles, xAOD::numberOfSCTHoles );
        trk->summaryValue( nTRTHits,  xAOD::numberOfTRTHits );
        trk->summaryValue( nTRTHoles, xAOD::numberOfTRTHoles );
        trk->summaryValue( nBLayerHits,  xAOD::numberOfBLayerHits );
	      if ( !m_DC14 ) {
          trk->summaryValue( nInnermostPixLayHits, xAOD::numberOfInnermostPixelLayerHits );
          trk->summaryValue( pixdEdX,   xAOD::pixeldEdx);
        }
      }
      m_el_trknSiHits.push_back( nPixHits + nSCTHits );
      m_el_trknPixHits.push_back( nPixHits );
      m_el_trknPixHoles.push_back( nPixHoles );
      m_el_trknSCTHits.push_back( nSCTHits );
      m_el_trknSCTHoles.push_back( nSCTHoles );
      m_el_trknTRTHits.push_back( nTRTHits );
      m_el_trknTRTHoles.push_back( nTRTHoles );
      m_el_trknBLayerHits.push_back( nBLayerHits );
      if ( !m_DC14 ) {
        m_el_trknInnermostPixLayHits.push_back( nInnermostPixLayHits );
        m_el_trkPixdEdX.push_back( pixdEdX );
      }
    }

    if ( m_elInfoSwitch->m_effSF && m_isMC ) {

      static SG::AuxElement::Accessor< std::vector< float > > accRecoSF("ElectronEfficiencyCorrector_RecoSyst");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigSF_LHLooseAndBLayer("ElectronEfficiencyCorrector_TrigSyst_LHLooseAndBLayer");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigSF_LHTight("ElectronEfficiencyCorrector_TrigSyst_LHTight");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigMCEff_LHLooseAndBLayer("ElectronEfficiencyCorrector_TrigMCEffSyst_LHLooseAndBLayer");
      static SG::AuxElement::Accessor< std::vector< float > > accTrigMCEff_LHTight("ElectronEfficiencyCorrector_TrigMCEffSyst_LHTight");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_Loose("ElectronEfficiencyCorrector_IsoSyst_IsoLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accIsoSF_FixedCutTight("ElectronEfficiencyCorrector_IsoSyst_IsoFixedCutTight");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHLooseAndBLayer("ElectronEfficiencyCorrector_PIDSyst_LHLooseAndBLayer");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHLoose("ElectronEfficiencyCorrector_PIDSyst_LHLoose");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHMedium("ElectronEfficiencyCorrector_PIDSyst_LHMedium");
      static SG::AuxElement::Accessor< std::vector< float > > accPIDSF_LHTight("ElectronEfficiencyCorrector_PIDSyst_LHTight");

      std::vector<float> junkSF(1,1.0);
      std::vector<float> junkEff(1,0.0);

      if( accRecoSF.isAvailable( *el_itr ) )                     { m_el_RecoEff_SF.push_back( accRecoSF( *el_itr ) ); } else { m_el_RecoEff_SF.push_back( junkSF ); }
      if( accTrigSF_LHLooseAndBLayer.isAvailable( *el_itr ) )    { m_el_TrigEff_SF_LHLooseAndBLayer.push_back( accTrigSF_LHLooseAndBLayer( *el_itr ) ); } else { m_el_TrigEff_SF_LHLooseAndBLayer.push_back( junkSF ); }
      if( accTrigSF_LHTight.isAvailable( *el_itr ) )             { m_el_TrigEff_SF_LHTight.push_back( accTrigSF_LHTight( *el_itr ) ); } else { m_el_TrigEff_SF_LHTight.push_back( junkSF ); }
      if( accTrigMCEff_LHLooseAndBLayer.isAvailable( *el_itr ) ) { m_el_TrigMCEff_LHLooseAndBLayer.push_back( accTrigMCEff_LHLooseAndBLayer( *el_itr ) ); } else { m_el_TrigMCEff_LHLooseAndBLayer.push_back( junkEff ); }
      if( accTrigMCEff_LHTight.isAvailable( *el_itr ) )          { m_el_TrigMCEff_LHTight.push_back( accTrigMCEff_LHTight( *el_itr ) ); } else { m_el_TrigMCEff_LHTight.push_back( junkEff ); }
      if( accIsoSF_Loose.isAvailable( *el_itr ) )                { m_el_IsoEff_SF_Loose.push_back( accIsoSF_Loose( *el_itr ) ); } else { m_el_IsoEff_SF_Loose.push_back( junkSF ); }
      if( accIsoSF_FixedCutTight.isAvailable( *el_itr ) )        { m_el_IsoEff_SF_FixedCutTight.push_back( accIsoSF_FixedCutTight( *el_itr ) ); } else { m_el_IsoEff_SF_FixedCutTight.push_back( junkSF ); }
      if( accPIDSF_LHLooseAndBLayer.isAvailable( *el_itr ) )     { m_el_PIDEff_SF_LHLooseAndBLayer.push_back( accPIDSF_LHLooseAndBLayer( *el_itr ) ); } else { m_el_PIDEff_SF_LHLooseAndBLayer.push_back( junkSF ); }
      if( accPIDSF_LHLoose.isAvailable( *el_itr ) )              { m_el_PIDEff_SF_LHLoose.push_back( accPIDSF_LHLoose( *el_itr ) ); } else { m_el_PIDEff_SF_LHLoose.push_back( junkSF ); }
      if( accPIDSF_LHMedium.isAvailable( *el_itr ) )             { m_el_PIDEff_SF_LHMedium.push_back( accPIDSF_LHMedium( *el_itr ) ); } else { m_el_PIDEff_SF_LHMedium.push_back( junkSF ); }
      if( accPIDSF_LHTight.isAvailable( *el_itr ) )              { m_el_PIDEff_SF_LHTight.push_back( accPIDSF_LHTight( *el_itr ) ); } else { m_el_PIDEff_SF_LHTight.push_back( junkSF ); }

    }

    this->FillElectronsUser(el_itr);

    m_nel++;
  }
}

void HelpTreeBase::ClearElectrons() {

  m_nel = 0;

  if ( m_elInfoSwitch->m_kinematic ){
    m_el_pt.clear();
    m_el_eta.clear();
    m_el_phi.clear();
    m_el_m.clear();
    m_el_caloCluster_eta.clear();
  }

  if ( m_elInfoSwitch->m_trigger ) {
    m_el_isTrigMatched.clear();
    m_el_isTrigMatchedToChain.clear();
    m_el_listTrigChains.clear();
  }

  if ( m_elInfoSwitch->m_isolation ) {
    m_el_isIsolated_LooseTrackOnly.clear();
    m_el_isIsolated_Loose.clear();
    m_el_isIsolated_Tight.clear();
    m_el_isIsolated_Gradient.clear();
    m_el_isIsolated_GradientLoose.clear();
    m_el_isIsolated_FixedCutLoose.clear();
    m_el_isIsolated_FixedCutTight.clear();
    m_el_isIsolated_FixedCutTightTrackOnly.clear();
    m_el_isIsolated_UserDefinedFixEfficiency.clear();
    m_el_isIsolated_UserDefinedCut.clear();
    m_el_etcone20.clear();
    m_el_ptcone20.clear();
    m_el_ptcone30.clear();
    m_el_ptcone40.clear();
    m_el_ptvarcone20.clear();
    m_el_ptvarcone30.clear();
    m_el_ptvarcone40.clear();
    m_el_topoetcone20.clear();
    m_el_topoetcone30.clear();
    m_el_topoetcone40.clear();
  }

  if ( m_elInfoSwitch->m_PID ) {
    m_nel_LHLoose = 0;
    m_el_LHLoose.clear();
    m_nel_LHMedium = 0;
    m_el_LHMedium.clear();
    m_nel_LHTight = 0;
    m_el_LHTight.clear();
    m_nel_IsEMLoose = 0;
    m_el_IsEMLoose.clear();
    m_nel_IsEMMedium = 0;
    m_el_IsEMMedium.clear();
    m_nel_IsEMTight = 0;
    m_el_IsEMTight.clear();
  }

  if ( m_elInfoSwitch->m_trackparams ) {
    m_el_trkd0.clear();
    m_el_trkd0sig.clear();
    m_el_trkz0.clear();
    m_el_trkz0sintheta.clear();
    m_el_trkphi0.clear();
    m_el_trktheta.clear();
    m_el_trkcharge.clear();
    m_el_trkqOverP.clear();
  }

  if ( m_elInfoSwitch->m_trackhitcont ) {
    m_el_trknSiHits.clear();
    m_el_trknPixHits.clear();
    m_el_trknPixHoles.clear();
    m_el_trknSCTHits.clear();
    m_el_trknSCTHoles.clear();
    m_el_trknTRTHits.clear();
    m_el_trknTRTHoles.clear();
    m_el_trknBLayerHits.clear();
    if ( !m_DC14 ) {
      m_el_trknInnermostPixLayHits.clear();
      m_el_trkPixdEdX.clear();
    }
  }

  if( m_elInfoSwitch->m_effSF && m_isMC ) {
    m_el_RecoEff_SF.clear();
    m_el_TrigEff_SF_LHLooseAndBLayer.clear();
    m_el_TrigEff_SF_LHTight.clear();
    m_el_TrigMCEff_LHLooseAndBLayer.clear();
    m_el_TrigMCEff_LHTight.clear();
    m_el_IsoEff_SF_Loose.clear();
    m_el_IsoEff_SF_FixedCutTight.clear();
    m_el_PIDEff_SF_LHLooseAndBLayer.clear();
    m_el_PIDEff_SF_LHLoose.clear();
    m_el_PIDEff_SF_LHMedium.clear();
    m_el_PIDEff_SF_LHTight.clear();
  }
}

/*********************
 *
 *   PHOTONS
 *
 ********************/

void HelpTreeBase::AddPhotons(const std::string detailStr) {

  if(m_debug)  Info("AddPhotons()", "Adding photon variables: %s", detailStr.c_str());

  m_phInfoSwitch = new HelperClasses::PhotonInfoSwitch( detailStr );

  // always
  m_tree->Branch("nph",    &m_nph,"nph/I");

  if ( m_phInfoSwitch->m_kinematic ) {
    m_tree->Branch("ph_pt",  &m_ph_pt);
    m_tree->Branch("ph_phi", &m_ph_phi);
    m_tree->Branch("ph_eta", &m_ph_eta);
    m_tree->Branch("ph_m",   &m_ph_m);
    m_tree->Branch("ph_E",   &m_ph_E);
  }

  if ( m_phInfoSwitch->m_isolation ) {

    m_tree->Branch("ph_isIsolated_Cone40CaloOnly", &m_ph_isIsolated_Cone40CaloOnly);
    m_tree->Branch("ph_isIsolated_Cone40",         &m_ph_isIsolated_Cone40);
    m_tree->Branch("ph_isIsolated_Cone20",         &m_ph_isIsolated_Cone20);

    //m_tree->Branch("ph_etcone20",         &m_ph_etcone20);
    m_tree->Branch("ph_ptcone20",         &m_ph_ptcone20);
    m_tree->Branch("ph_ptcone30",         &m_ph_ptcone30);
    m_tree->Branch("ph_ptcone40",         &m_ph_ptcone40);
    m_tree->Branch("ph_ptvarcone20",      &m_ph_ptvarcone20);
    m_tree->Branch("ph_ptvarcone30",      &m_ph_ptvarcone30);
    m_tree->Branch("ph_ptvarcone40",      &m_ph_ptvarcone40);
    m_tree->Branch("ph_topoetcone20",     &m_ph_topoetcone20);
    m_tree->Branch("ph_topoetcone30",     &m_ph_topoetcone30);
    m_tree->Branch("ph_topoetcone40",     &m_ph_topoetcone40);
  }

  if ( m_phInfoSwitch->m_PID ) {
    m_tree->Branch("nph_IsLoose",    &m_nph_IsLoose);
    m_tree->Branch("ph_IsLoose",     &m_ph_IsLoose);
    m_tree->Branch("nph_IsMedium",   &m_nph_IsMedium);
    m_tree->Branch("ph_IsMedium",    &m_ph_IsMedium);
    m_tree->Branch("nph_IsTight",    &m_nph_IsTight);
    m_tree->Branch("ph_IsTight",     &m_ph_IsTight);
  }

  if ( m_phInfoSwitch->m_purity) {
    m_tree->Branch("m_ph_radhad1",   &m_ph_radhad1);
    m_tree->Branch("m_ph_radhad",    &m_ph_radhad );
    m_tree->Branch("m_ph_e277",      &m_ph_e277   );
    m_tree->Branch("m_ph_reta",      &m_ph_reta   );
    m_tree->Branch("m_ph_rphi",      &m_ph_rphi   );
    m_tree->Branch("m_ph_weta2",     &m_ph_weta2  );
    m_tree->Branch("m_ph_f1",        &m_ph_f1     );
    m_tree->Branch("m_ph_wtot",      &m_ph_wtot   );
    m_tree->Branch("m_ph_deltae",    &m_ph_deltae );
    m_tree->Branch("m_ph_eratio",    &m_ph_eratio );
  }

  this->AddPhotonsUser();
}

void HelpTreeBase::FillPhotons( const xAOD::PhotonContainer* photons ) {

  this->ClearPhotons();
  this->ClearPhotonsUser();

  m_nph = 0;

  for ( auto ph_itr : *(photons) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillPhotons()", "Filling photon w/ pT = %2f", ph_itr->pt() / m_units ); }

    if ( m_phInfoSwitch->m_kinematic ) {
      m_ph_pt.push_back ( (ph_itr)->pt() / m_units );
      m_ph_eta.push_back( (ph_itr)->eta() );
      m_ph_phi.push_back( (ph_itr)->phi() );
      m_ph_m.push_back  ( (ph_itr)->m()  / m_units );
      m_ph_E.push_back  ( (ph_itr)->e()  / m_units );
    }

    if ( m_phInfoSwitch->m_isolation ) {

      static SG::AuxElement::Accessor<char> isIsoCone40CaloOnlyAcc    ("isIsolated_FixedCutTightCaloOnly");
      static SG::AuxElement::Accessor<char> isIsoCone40Acc            ("isIsolated_FixedCutTight");
      static SG::AuxElement::Accessor<char> isIsoCone20Acc            ("isIsolated_FixedCutLoose");

      if ( isIsoCone40CaloOnlyAcc.isAvailable( *ph_itr ) ) {
	m_ph_isIsolated_Cone40CaloOnly.push_back( isIsoCone40CaloOnlyAcc( *ph_itr ) );
      } else {
	m_ph_isIsolated_Cone40CaloOnly.push_back( -1 );
      }

      if ( isIsoCone40Acc.isAvailable( *ph_itr ) ) {
	m_ph_isIsolated_Cone40.push_back( isIsoCone40Acc( *ph_itr ) );
      } else {
	m_ph_isIsolated_Cone40.push_back( -1 );
      }

      if ( isIsoCone20Acc.isAvailable( *ph_itr ) ) {
	m_ph_isIsolated_Cone20.push_back( isIsoCone20Acc( *ph_itr ) );
      } else {
	m_ph_isIsolated_Cone20.push_back( -1 );
      }

      //m_ph_etcone20    .push_back( ph_itr->isolation( xAOD::Iso::etcone20    ) / m_units  );
      m_ph_ptcone20    .push_back( ph_itr->isolation( xAOD::Iso::ptcone20    ) / m_units  );
      m_ph_ptcone30    .push_back( ph_itr->isolation( xAOD::Iso::ptcone30    ) / m_units  );
      m_ph_ptcone40    .push_back( ph_itr->isolation( xAOD::Iso::ptcone40    ) / m_units  );
      m_ph_ptvarcone20 .push_back( ph_itr->isolation( xAOD::Iso::ptvarcone20 ) / m_units  );
      m_ph_ptvarcone30 .push_back( ph_itr->isolation( xAOD::Iso::ptvarcone30 ) / m_units  );
      m_ph_ptvarcone40 .push_back( ph_itr->isolation( xAOD::Iso::ptvarcone40 ) / m_units  );
      m_ph_topoetcone20.push_back( ph_itr->isolation( xAOD::Iso::topoetcone20) / m_units  );
      m_ph_topoetcone30.push_back( ph_itr->isolation( xAOD::Iso::topoetcone30) / m_units  );
      m_ph_topoetcone40.push_back( ph_itr->isolation( xAOD::Iso::topoetcone40) / m_units  );

    }

    if ( m_phInfoSwitch->m_PID ) {
      static SG::AuxElement::Accessor<bool> phLooseAcc  ("PhotonID_Loose");
      static SG::AuxElement::Accessor<bool> phMediumAcc ("PhotonID_Medium");
      static SG::AuxElement::Accessor<bool> phTightAcc  ("PhotonID_Tight");

      if ( phLooseAcc.isAvailable( *ph_itr ) ) {
        m_ph_IsLoose.push_back( phLooseAcc( *ph_itr ) );
	if ( phLooseAcc( *ph_itr ) == 1 ) { ++m_nph_IsLoose; }
      } else { m_ph_IsLoose.push_back( -1 ); }

      if ( phMediumAcc.isAvailable( *ph_itr ) ) {
        m_ph_IsMedium.push_back( phMediumAcc( *ph_itr ) );
	if ( phMediumAcc( *ph_itr ) == 1 ) { ++m_nph_IsMedium; }
      } else { m_ph_IsMedium.push_back( -1 ); }

      if ( phTightAcc.isAvailable( *ph_itr ) ) {
        m_ph_IsTight.push_back( phTightAcc( *ph_itr ) );
	if ( phTightAcc( *ph_itr ) == 1 ) { ++m_nph_IsTight; }
      } else { m_ph_IsTight.push_back( -1 ); }

    }

    if (m_phInfoSwitch->m_purity) {
      static SG::AuxElement::Accessor<float> radhad1  ("Rhad1"  );
      static SG::AuxElement::Accessor<float> radhad   ("Rhad"   );
      static SG::AuxElement::Accessor<float> e277     ("e277"   );
      static SG::AuxElement::Accessor<float> reta     ("Reta"   );
      static SG::AuxElement::Accessor<float> rphi     ("Rphi"   );
      static SG::AuxElement::Accessor<float> weta2    ("weta2"  );
      static SG::AuxElement::Accessor<float> f1       ("f1"     );
      static SG::AuxElement::Accessor<float> wtot     ("wtots1" );
      //static SG::AuxElement::Accessor<float> w1       ("w1"     );
      static SG::AuxElement::Accessor<float> deltae   ("DeltaE" );
      static SG::AuxElement::Accessor<float> eratio   ("Eratio" );
      
      m_ph_radhad1  .push_back( radhad1(*ph_itr) );
      m_ph_radhad   .push_back( radhad (*ph_itr) );
      m_ph_e277     .push_back( e277   (*ph_itr) );
      m_ph_reta     .push_back( reta   (*ph_itr) );
      m_ph_rphi     .push_back( rphi   (*ph_itr) );
      m_ph_weta2    .push_back( weta2  (*ph_itr) );
      m_ph_f1       .push_back( f1     (*ph_itr) );
      m_ph_wtot     .push_back( wtot   (*ph_itr) );
      //m_ph_w1       .push_back( w1     (*ph_itr) );
      m_ph_deltae   .push_back( deltae (*ph_itr) );
      m_ph_eratio   .push_back( eratio (*ph_itr) );

    }

    this->FillPhotonsUser(ph_itr);

    m_nph++;
  }
}

void HelpTreeBase::ClearPhotons() {

  m_nph = 0;

  if ( m_phInfoSwitch->m_kinematic ){
    m_ph_pt.clear();
    m_ph_eta.clear();
    m_ph_phi.clear();
    m_ph_m.clear();
    m_ph_E.clear();
  }


  if ( m_phInfoSwitch->m_isolation ){
    m_ph_isIsolated_Cone40CaloOnly.clear();
    m_ph_isIsolated_Cone40.clear();
    m_ph_isIsolated_Cone20.clear();
    //m_ph_etcone20.clear();
    m_ph_ptcone20.clear();
    m_ph_ptcone30.clear();
    m_ph_ptcone40.clear();
    m_ph_ptvarcone20.clear();
    m_ph_ptvarcone30.clear();
    m_ph_ptvarcone40.clear();
    m_ph_topoetcone20.clear();
    m_ph_topoetcone30.clear();
    m_ph_topoetcone40.clear();
  }


  if ( m_phInfoSwitch->m_PID ) {
    m_nph_IsLoose = 0;
    m_ph_IsLoose.clear();
    m_nph_IsMedium = 0;
    m_ph_IsMedium.clear();
    m_nph_IsTight = 0;
    m_ph_IsTight.clear();
  }

  if(m_phInfoSwitch->m_purity){
    m_ph_radhad1.clear();
    m_ph_radhad.clear();
    m_ph_e277.clear();
    m_ph_reta.clear();
    m_ph_rphi.clear();
    m_ph_weta2.clear();
    m_ph_f1.clear();
    m_ph_wtot.clear();
    m_ph_deltae.clear();
    m_ph_eratio.clear(); 

  }

}

/*********************
 *
 *   JETS
 *
 ********************/

void HelpTreeBase::AddJets(const std::string detailStr, const std::string jetName)
{

  if(m_debug) Info("AddJets()", "Adding jet %s with variables: %s", jetName.c_str(), detailStr.c_str());

  m_jets[jetName] = new xAH::JetContainer(jetName, detailStr, m_units, m_isMC);

  xAH::JetContainer* thisJet = m_jets[jetName];
  thisJet->setBranches(m_tree);
  this->AddJetsUser(detailStr, jetName);

}


void HelpTreeBase::FillJets( const xAOD::JetContainer* jets, int pvLocation, const std::string jetName ) {

  this->ClearJets(jetName);

  const xAOD::VertexContainer* vertices(nullptr);
  const xAOD::Vertex *pv = 0;

  xAH::JetContainer* thisJet = m_jets[jetName];

  if( thisJet->m_infoSwitch.m_trackPV || thisJet->m_infoSwitch.m_allTrack ) {
    HelperFunctions::retrieve( vertices, "PrimaryVertices", m_event, 0 );
    pvLocation = HelperFunctions::getPrimaryVertexLocation( vertices );
    if ( pvLocation >= 0 ) pv = vertices->at( pvLocation );
  }



  // Global event BTag SF weight (--> the product of each object's weight)
  //
  if ( m_isMC ) {
    const xAOD::EventInfo* eventInfo(nullptr);
    HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, false);

    thisJet->FillGlobalBTagSF(eventInfo);

  }

  for( auto jet_itr : *jets ) {
    this->FillJet(jet_itr, pv, pvLocation, jetName);
  }

}



void HelpTreeBase::FillJet( const xAOD::Jet* jet_itr, const xAOD::Vertex* pv, int pvLocation, const std::string jetName ) {

  xAH::JetContainer* thisJet = m_jets[jetName];

  thisJet->FillJet(jet_itr, pv, pvLocation);

  this->FillJetsUser(jet_itr, jetName);

  return;
}

void HelpTreeBase::ClearJets(const std::string jetName) {

  xAH::JetContainer* thisJet = m_jets[jetName];
  thisJet->clear();

  this->ClearJetsUser(jetName);

}

/*********************
 *
 *   TRUTH
 *
 ********************/

void HelpTreeBase::AddTruthParts(const std::string truthName, const std::string detailStr)
{

  if(m_debug) Info("AddTruthParts()", "Adding truth particle %s with variables: %s", truthName.c_str(), detailStr.c_str());
  m_truth[truthName] = new xAH::TruthContainer(truthName, detailStr, m_units);
  
  xAH::TruthContainer* thisTruth = m_truth[truthName];
  thisTruth->setBranches(m_tree);
  this->AddTruthUser(truthName);
}

void HelpTreeBase::FillTruth( const std::string truthName, const xAOD::TruthParticleContainer* truthParts ) {

  this->ClearTruth(truthName);

  // We need some basic cuts here to avoid many PseudoRapiditity warnings being thrown ...
  float truthparticle_ptmin  = 2.0;
  float truthparticle_etamin = 8.0;

  for( auto truth_itr : *truthParts ) {
    
    if((truth_itr->pt() / m_units < truthparticle_ptmin) || (fabs(truth_itr->eta()) < truthparticle_etamin) ){
      continue;
    }
    
    this->FillTruth(truth_itr, truthName);
  }

}

void HelpTreeBase::FillTruth( const xAOD::TruthParticle* truthPart, const std::string truthName )
{
  xAH::TruthContainer* thisTruth = m_truth[truthName];

  thisTruth->FillTruth(truthPart);

  this->FillTruthUser(truthName, truthPart);

  return;
}

void HelpTreeBase::ClearTruth(const std::string truthName) {
  
  xAH::TruthContainer* thisTruth = m_truth[truthName];
  thisTruth->clear();

  this->ClearTruthUser(truthName);

}


/*********************
 *
 *   FAT JETS
 *
 ********************/

// make a unique container:suffix key to lookup the branches in the maps
std::string HelpTreeBase::FatJetCollectionName(const std::string& fatjetName,
    const std::string& suffix) {
  return suffix.empty() ? fatjetName : (fatjetName + ":" + suffix);
}

void HelpTreeBase::AddFatJets(const std::string& detailStr, const std::string& fatjetName,
        const std::string& suffix) {

  if(m_debug) Info("AddFatJets()", "Adding fat jet variables: %s", detailStr.c_str());

  // Simple function to formulate the branch names given the variable name.
  // Eliminates a lot of verbosity in the rest of the function...
  auto bname = [&](const std::string& varName) {
    std::string name = fatjetName + "_" + varName;
    if (not suffix.empty()) {
      name += "_" + suffix;
    }
    return name;
  };

  const std::string collectionName = FatJetCollectionName(fatjetName, suffix);

  m_fatJetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );

  // always
  std::string counterName = "n"+fatjetName;
  if (not suffix.empty()) { counterName += "_" + suffix; }
  m_tree->Branch(counterName.c_str(), &m_nfatjet[collectionName], (counterName+"/I").c_str());

  if ( m_fatJetInfoSwitch->m_kinematic ) {
    m_tree->Branch(bname("E").c_str(),   &m_fatjet_E[collectionName]);
    m_tree->Branch(bname("m").c_str(),   &m_fatjet_m[collectionName]);
    m_tree->Branch(bname("pt").c_str(),  &m_fatjet_pt[collectionName]);
    m_tree->Branch(bname("phi").c_str(), &m_fatjet_phi[collectionName]);
    m_tree->Branch(bname("eta").c_str(), &m_fatjet_eta[collectionName]);
  }
  if ( m_fatJetInfoSwitch->m_substructure ) {
    m_tree->Branch(bname("Split12").c_str(),      &m_fatjet_Split12[collectionName]);
    m_tree->Branch(bname("Split23").c_str(),      &m_fatjet_Split23[collectionName]);
    m_tree->Branch(bname("Split34").c_str(),      &m_fatjet_Split34[collectionName]);
    m_tree->Branch(bname("tau1_wta").c_str(),     &m_fatjet_tau1_wta[collectionName]);
    m_tree->Branch(bname("tau2_wta").c_str(),     &m_fatjet_tau2_wta[collectionName]);
    m_tree->Branch(bname("tau3_wta").c_str(),     &m_fatjet_tau3_wta[collectionName]);
    m_tree->Branch(bname("tau21_wta").c_str(),    &m_fatjet_tau21_wta[collectionName]);
    m_tree->Branch(bname("tau32_wta").c_str(),    &m_fatjet_tau32_wta[collectionName]);
    m_tree->Branch(bname("ECF1").c_str(),         &m_fatjet_ECF1[collectionName]);
    m_tree->Branch(bname("ECF2").c_str(),         &m_fatjet_ECF2[collectionName]);
    m_tree->Branch(bname("ECF3").c_str(),         &m_fatjet_ECF3[collectionName]);
    m_tree->Branch(bname("C2").c_str(),           &m_fatjet_C2[collectionName]);
    m_tree->Branch(bname("D2").c_str(),           &m_fatjet_D2[collectionName]);
    m_tree->Branch(bname("NTrimSubjets").c_str(), &m_fatjet_NTrimSubjets[collectionName]);
    m_tree->Branch(bname("Nclusters").c_str(),    &m_fatjet_NClusters[collectionName]);
  }
  if ( m_fatJetInfoSwitch->m_constituent) {
    m_tree->Branch(bname("numConstituents").c_str(), &m_fatjet_numConstituents[collectionName]);
  }
  if ( m_fatJetInfoSwitch->m_constituentAll) {
    m_tree->Branch(bname("constituentWeights").c_str(), &m_fatjet_constituentWeights[collectionName]);
    m_tree->Branch(bname("constituent_pt").c_str(),     &m_fatjet_constituent_pt[collectionName]);
    m_tree->Branch(bname("constituent_eta").c_str(),    &m_fatjet_constituent_eta[collectionName]);
    m_tree->Branch(bname("constituent_phi").c_str(),    &m_fatjet_constituent_phi[collectionName]);
    m_tree->Branch(bname("constituent_e").c_str(),      &m_fatjet_constituent_e[collectionName]);
  }

  this->AddFatJetsUser(detailStr, fatjetName, suffix);
}

void HelpTreeBase::AddTruthFatJets(std::string detailStr) {

  if(m_debug) Info("AddTruthFatJets()", "Adding fat jet variables: %s", detailStr.c_str());

  m_truthFatJetInfoSwitch = new HelperClasses::JetInfoSwitch( detailStr );

  // always
  m_tree->Branch("ntruthfatjets",    &m_ntruthfatjet,"ntruthfatjets/I");

  if ( m_truthFatJetInfoSwitch->m_kinematic ) {
    m_tree->Branch("truth_fatjet_E",   &m_truth_fatjet_E);
    m_tree->Branch("truth_fatjet_m",   &m_truth_fatjet_m);
    m_tree->Branch("truth_fatjet_pt",  &m_truth_fatjet_pt);
    m_tree->Branch("truth_fatjet_phi", &m_truth_fatjet_phi);
    m_tree->Branch("truth_fatjet_eta", &m_truth_fatjet_eta);
  }
  if ( m_truthFatJetInfoSwitch->m_substructure ) {

    m_tree->Branch("truth_fatjet_Split12",&m_truth_fatjet_Split12);
    m_tree->Branch("truth_fatjet_Split23",&m_truth_fatjet_Split23);
    m_tree->Branch("truth_fatjet_Split34",&m_truth_fatjet_Split34);
    m_tree->Branch("truth_fatjet_tau1_wta",&m_truth_fatjet_tau1_wta);
    m_tree->Branch("truth_fatjet_tau2_wta",&m_truth_fatjet_tau2_wta);
    m_tree->Branch("truth_fatjet_tau3_wta",&m_truth_fatjet_tau3_wta);
    m_tree->Branch("truth_fatjet_tau21_wta",   &m_truth_fatjet_tau21_wta);
    m_tree->Branch("truth_fatjet_tau32_wta",   &m_truth_fatjet_tau32_wta);
    m_tree->Branch("truth_fatjet_ECF1",&m_truth_fatjet_ECF1);
    m_tree->Branch("truth_fatjet_ECF2",&m_truth_fatjet_ECF2);
    m_tree->Branch("truth_fatjet_ECF3",&m_truth_fatjet_ECF3);
    m_tree->Branch("truth_fatjet_C2",&m_truth_fatjet_C2);
    m_tree->Branch("truth_fatjet_D2",&m_truth_fatjet_D2);
    m_tree->Branch("truth_fatjet_NTrimSubjets",&m_truth_fatjet_NTrimSubjets);
  }
  if ( m_truthFatJetInfoSwitch->m_constituent) {
    m_tree->Branch("truth_fatjet_numConstituents",&m_truth_fatjet_numConstituents);
  }
  if ( m_truthFatJetInfoSwitch->m_constituentAll) {
    m_tree->Branch("truth_fatjet_constituentWeights",&m_truth_fatjet_constituentWeights);
    m_tree->Branch("truth_fatjet_constituent_pt",&m_truth_fatjet_constituent_pt);
    m_tree->Branch("truth_fatjet_constituent_eta",&m_truth_fatjet_constituent_eta);
    m_tree->Branch("truth_fatjet_constituent_phi",&m_truth_fatjet_constituent_phi);
    m_tree->Branch("truth_fatjet_constituent_e",&m_truth_fatjet_constituent_e);
  }
  if (m_truthFatJetInfoSwitch->m_truth) {
    m_tree->Branch("truth_fatjet_GhostTQuarks",&m_truth_fatjet_GhostTQuarks);
    m_tree->Branch("truth_fatjet_GhostWBosons",&m_truth_fatjet_GhostWBosons);
    m_tree->Branch("truth_fatjet_GhostZBosons",&m_truth_fatjet_GhostZBosons);
    m_tree->Branch("truth_fatjet_GhostHBosons",&m_truth_fatjet_GhostHBosons);
  }

  this->AddTruthFatJetsUser();
}


void HelpTreeBase::FillFatJets( const xAOD::JetContainer* fatJets , const std::string& fatjetName, const std::string& suffix) {
  this->ClearFatJets(fatjetName, suffix);
  this->ClearFatJetsUser(fatjetName, suffix);

  const std::string collectionName = FatJetCollectionName(fatjetName, suffix);

  for( auto fatjet_itr : *fatJets ) {

    if( m_fatJetInfoSwitch->m_kinematic ){
      m_fatjet_pt[collectionName].push_back ( fatjet_itr->pt() / m_units );
      m_fatjet_m[collectionName].push_back ( fatjet_itr->m() / m_units );
      m_fatjet_eta[collectionName].push_back( fatjet_itr->eta() );
      m_fatjet_phi[collectionName].push_back( fatjet_itr->phi() );
      m_fatjet_E[collectionName].push_back  ( fatjet_itr->e() / m_units );
    }
    if( m_fatJetInfoSwitch->m_substructure ){
      static SG::AuxElement::ConstAccessor<float> acc_Split12("Split12");
      static SG::AuxElement::ConstAccessor<float> acc_Split23("Split23");
      static SG::AuxElement::ConstAccessor<float> acc_Split34("Split34");
      if ( acc_Split12.isAvailable( *fatjet_itr ) ) {
        m_fatjet_Split12[collectionName].push_back( acc_Split12( *fatjet_itr ) / m_units );
      } else { m_fatjet_Split12[collectionName].push_back( -999 ); }
      if ( acc_Split23.isAvailable( *fatjet_itr ) ) {
	m_fatjet_Split23[collectionName].push_back(acc_Split23( *fatjet_itr ) / m_units );
      } else { m_fatjet_Split34[collectionName].push_back( -999 ); }
      if ( acc_Split34.isAvailable( *fatjet_itr ) ) {
	m_fatjet_Split34[collectionName].push_back( acc_Split34( *fatjet_itr ) / m_units );
      } else { m_fatjet_Split34[collectionName].push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> acc_tau1_wta ("Tau1_wta");
      static SG::AuxElement::ConstAccessor<float> acc_tau2_wta ("Tau2_wta");
      static SG::AuxElement::ConstAccessor<float> acc_tau3_wta ("Tau3_wta");
      static SG::AuxElement::ConstAccessor<float> acc_tau21_wta ("Tau21_wta");
      static SG::AuxElement::ConstAccessor<float> acc_tau32_wta ("Tau32_wta");

      if ( acc_tau1_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau1_wta[collectionName].push_back( acc_tau1_wta( *fatjet_itr ) );
      } else { m_fatjet_tau1_wta[collectionName].push_back( -999 ); }
      if ( acc_tau2_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau2_wta[collectionName].push_back( acc_tau2_wta( *fatjet_itr ) );
      } else { m_fatjet_tau2_wta[collectionName].push_back( -999 ); }
      if ( acc_tau3_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau3_wta[collectionName].push_back( acc_tau3_wta( *fatjet_itr ) );
      } else { m_fatjet_tau3_wta[collectionName].push_back( -999 ); }
      if(acc_tau21_wta.isAvailable( *fatjet_itr )){
        m_fatjet_tau21_wta[collectionName].push_back( acc_tau21_wta( *fatjet_itr ) );
      } else if ( acc_tau1_wta.isAvailable( *fatjet_itr ) and acc_tau2_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau21_wta[collectionName].push_back( acc_tau2_wta( *fatjet_itr ) / acc_tau1_wta( *fatjet_itr ) );
      } else { m_fatjet_tau21_wta[collectionName].push_back( -999 ); }
      if(acc_tau32_wta.isAvailable( *fatjet_itr )){
	m_fatjet_tau32_wta[collectionName].push_back( acc_tau32_wta( *fatjet_itr ) );
      } else if ( acc_tau2_wta.isAvailable( *fatjet_itr ) and acc_tau3_wta.isAvailable( *fatjet_itr ) ) {
        m_fatjet_tau32_wta[collectionName].push_back( acc_tau3_wta( *fatjet_itr ) / acc_tau2_wta( *fatjet_itr ) );
      } else { m_fatjet_tau32_wta[collectionName].push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> acc_ECF1 ("ECF1");
      static SG::AuxElement::ConstAccessor<float> acc_ECF2("ECF2");
      static SG::AuxElement::ConstAccessor<float> acc_ECF3 ("ECF3");
      static SG::AuxElement::ConstAccessor<float> acc_C2("C2");
      static SG::AuxElement::ConstAccessor<float> acc_D2 ("D2");
      static SG::AuxElement::ConstAccessor<int> acc_NClusters ("MyNClusters");

      if(acc_NClusters.isAvailable(*fatjet_itr)){ m_fatjet_NClusters[collectionName].push_back(acc_NClusters(*fatjet_itr));}
      else{ m_fatjet_NClusters[collectionName].push_back(-999); }

      if(acc_C2.isAvailable(*fatjet_itr)){
	m_fatjet_C2[collectionName].push_back(acc_C2(*fatjet_itr));
      } else if( acc_ECF1.isAvailable(*fatjet_itr) && acc_ECF2.isAvailable(*fatjet_itr) && acc_ECF3.isAvailable(*fatjet_itr)){
	m_fatjet_C2[collectionName].push_back( acc_ECF3(*fatjet_itr)*acc_ECF1(*fatjet_itr)/pow(acc_ECF2(*fatjet_itr),2.0));
      } else{ m_fatjet_C2[collectionName].push_back(-999); }

      if( acc_D2.isAvailable( *fatjet_itr ) ) {
	m_fatjet_D2[collectionName].push_back( acc_D2( *fatjet_itr ));
      } else if (acc_ECF1.isAvailable( *fatjet_itr ) && acc_ECF2.isAvailable( *fatjet_itr ) && acc_ECF3.isAvailable( *fatjet_itr )){
	float e2=(acc_ECF2( *fatjet_itr )/(acc_ECF1( *fatjet_itr )*acc_ECF1( *fatjet_itr )));
	float e3=(acc_ECF3( *fatjet_itr )/(acc_ECF1( *fatjet_itr )*acc_ECF1( *fatjet_itr )*acc_ECF1( *fatjet_itr )));
	m_fatjet_D2[collectionName].push_back( e3/(e2*e2*e2) );
      } else{ m_fatjet_D2[collectionName].push_back(-999); }

      if ( acc_ECF1.isAvailable( *fatjet_itr ) ) {
	m_fatjet_ECF1[collectionName].push_back( acc_ECF1( *fatjet_itr ) / m_units);
      } else { m_fatjet_ECF1[collectionName].push_back( -999 ); }
      if ( acc_ECF2.isAvailable( *fatjet_itr ) ) {
	m_fatjet_ECF2[collectionName].push_back( acc_ECF2( *fatjet_itr ) / m_units);
      } else { m_fatjet_ECF2[collectionName].push_back( -999 ); }
      if ( acc_ECF3.isAvailable( *fatjet_itr ) ) {
	m_fatjet_ECF3[collectionName].push_back( acc_ECF3( *fatjet_itr ) / m_units);
      } else { m_fatjet_ECF3[collectionName].push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> NTrimSubjets("NTrimSubjets");
      if( NTrimSubjets.isAvailable(*fatjet_itr) ){
	m_fatjet_NTrimSubjets[collectionName].push_back(NTrimSubjets(*fatjet_itr));
      } else{ m_fatjet_NTrimSubjets[collectionName].push_back(-999); }
    }

    if( m_fatJetInfoSwitch->m_constituent ){
      m_fatjet_numConstituents[collectionName].push_back( fatjet_itr->numConstituents() );
    }
    if( m_fatJetInfoSwitch->m_constituentAll ){
      m_fatjet_constituentWeights[collectionName].push_back( fatjet_itr->getAttribute< std::vector<float> >( "constituentWeights" ) );
      std::vector<float> pt;
      std::vector<float> eta;
      std::vector<float> phi;
      std::vector<float> e;
      xAOD::JetConstituentVector consVec = fatjet_itr->getConstituents();
      if( consVec.isValid() ) {
	// use the example provided in
	// http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODJet/xAODJet/JetConstituentVector.h
	xAOD::JetConstituentVector::iterator constit = consVec.begin();
	xAOD::JetConstituentVector::iterator constitE = consVec.end();
	for( ; constit != constitE; constit++){
	  pt. push_back( constit->pt() / m_units );
	  eta.push_back( constit->eta() );
	  phi.push_back( constit->phi() );
	  e.  push_back( constit->e() / m_units  );
	}
      }
      m_fatjet_constituent_pt[collectionName].push_back( pt  );
      m_fatjet_constituent_eta[collectionName].push_back( eta );
      m_fatjet_constituent_phi[collectionName].push_back( phi );
      m_fatjet_constituent_e[collectionName].push_back( e   );
    }

    this->FillFatJetsUser(fatjet_itr, fatjetName, suffix);

    m_nfatjet[collectionName]++;

  } // loop over fat jets

}


void HelpTreeBase::FillTruthFatJets( const xAOD::JetContainer* truthTruthFatJets ) {
  this->ClearTruthFatJets();
  this->ClearTruthFatJetsUser();

  for( auto truth_fatjet_itr : *truthTruthFatJets ) {

    if( m_truthFatJetInfoSwitch->m_kinematic ){
      m_truth_fatjet_pt.push_back ( truth_fatjet_itr->pt() / m_units );
      m_truth_fatjet_m.push_back ( truth_fatjet_itr->m() / m_units );
      m_truth_fatjet_eta.push_back( truth_fatjet_itr->eta() );
      m_truth_fatjet_phi.push_back( truth_fatjet_itr->phi() );
      m_truth_fatjet_E.push_back  ( truth_fatjet_itr->e() / m_units );
    }
    if( m_truthFatJetInfoSwitch->m_substructure ){
      static SG::AuxElement::ConstAccessor<float> Split12("Split12");
      static SG::AuxElement::ConstAccessor<float> Split23("Split23");
      static SG::AuxElement::ConstAccessor<float> Split34("Split34");
      if ( Split12.isAvailable( *truth_fatjet_itr ) ) {
	m_truth_fatjet_Split12.push_back( Split12( *truth_fatjet_itr ) / m_units );
      } else { m_truth_fatjet_Split12.push_back( -999 ); }
      if ( Split23.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_Split23.push_back(Split23( *truth_fatjet_itr ) / m_units );
      } else { m_truth_fatjet_Split34.push_back( -999 ); }
      if ( Split34.isAvailable( *truth_fatjet_itr ) ) {
	m_truth_fatjet_Split34.push_back( Split34( *truth_fatjet_itr ) / m_units );
      } else { m_truth_fatjet_Split34.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> tau1_wta ("Tau1_wta");
      static SG::AuxElement::ConstAccessor<float> tau2_wta ("Tau2_wta");
      static SG::AuxElement::ConstAccessor<float> tau3_wta ("Tau3_wta");
      static SG::AuxElement::ConstAccessor<float> tau21_wta ("Tau21_wta");
      static SG::AuxElement::ConstAccessor<float> tau32_wta ("Tau32_wta");

      if ( tau1_wta.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_tau1_wta.push_back( tau1_wta( *truth_fatjet_itr ) );
      } else { m_truth_fatjet_tau1_wta.push_back( -999 ); }
      if ( tau2_wta.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_tau2_wta.push_back( tau2_wta( *truth_fatjet_itr ) );
      } else { m_truth_fatjet_tau2_wta.push_back( -999 ); }
      if ( tau3_wta.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_tau3_wta.push_back( tau3_wta( *truth_fatjet_itr ) );
      } else { m_truth_fatjet_tau3_wta.push_back( -999 ); }
      if(tau21_wta.isAvailable( *truth_fatjet_itr )){
        m_truth_fatjet_tau21_wta.push_back( tau21_wta( *truth_fatjet_itr ) );
      } else if ( tau1_wta.isAvailable( *truth_fatjet_itr ) and tau2_wta.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_tau21_wta.push_back( tau2_wta( *truth_fatjet_itr ) / tau1_wta( *truth_fatjet_itr ) );
      } else { m_truth_fatjet_tau21_wta.push_back( -999 ); }
      if(tau32_wta.isAvailable( *truth_fatjet_itr )){
        m_truth_fatjet_tau32_wta.push_back( tau32_wta( *truth_fatjet_itr ) );
      } else if ( tau2_wta.isAvailable( *truth_fatjet_itr ) and tau3_wta.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_tau32_wta.push_back( tau3_wta( *truth_fatjet_itr ) / tau2_wta( *truth_fatjet_itr ) );
      } else { m_truth_fatjet_tau32_wta.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<float> ECF1 ("ECF1");
      static SG::AuxElement::ConstAccessor<float> ECF2("ECF2");
      static SG::AuxElement::ConstAccessor<float> ECF3 ("ECF3");
      static SG::AuxElement::ConstAccessor<float> C2("C2");
      static SG::AuxElement::ConstAccessor<float> D2 ("D2");

      if(C2.isAvailable(*truth_fatjet_itr)){
	m_truth_fatjet_C2.push_back(C2(*truth_fatjet_itr));
      } else if( ECF1.isAvailable(*truth_fatjet_itr) && ECF2.isAvailable(*truth_fatjet_itr) && ECF3.isAvailable(*truth_fatjet_itr)){
        m_truth_fatjet_C2.push_back( ECF3(*truth_fatjet_itr)*ECF1(*truth_fatjet_itr)/pow(ECF2(*truth_fatjet_itr),2.0));
      } else{ m_truth_fatjet_C2.push_back(-999); }

      if( D2.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_D2.push_back( D2( *truth_fatjet_itr ));
      } else if (ECF1.isAvailable( *truth_fatjet_itr ) && ECF2.isAvailable( *truth_fatjet_itr ) && ECF3.isAvailable( *truth_fatjet_itr )){
	float e2=(ECF2( *truth_fatjet_itr )/(ECF1( *truth_fatjet_itr )*ECF1( *truth_fatjet_itr )));
	float e3=(ECF3( *truth_fatjet_itr )/(ECF1( *truth_fatjet_itr )*ECF1( *truth_fatjet_itr )*ECF1( *truth_fatjet_itr )));
        m_truth_fatjet_D2.push_back( e3/(e2*e2*e2) );
      } else{ m_truth_fatjet_D2.push_back(-999); }

      if ( ECF1.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_ECF1.push_back( ECF1( *truth_fatjet_itr ) / m_units);
      } else { m_truth_fatjet_ECF1.push_back( -999 ); }
      if ( ECF2.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_ECF2.push_back( ECF2( *truth_fatjet_itr ) / m_units);
      } else { m_truth_fatjet_ECF2.push_back( -999 ); }
      if ( ECF3.isAvailable( *truth_fatjet_itr ) ) {
        m_truth_fatjet_ECF3.push_back( ECF3( *truth_fatjet_itr ) / m_units);
      } else { m_truth_fatjet_ECF3.push_back( -999 ); }

      static SG::AuxElement::ConstAccessor<int> NTrimSubjets("NTrimSubjets");
      if( NTrimSubjets.isAvailable(*truth_fatjet_itr) ){
        m_truth_fatjet_NTrimSubjets.push_back(NTrimSubjets(*truth_fatjet_itr));
      } else{ m_truth_fatjet_NTrimSubjets.push_back(-999); }
    }

    if( m_truthFatJetInfoSwitch->m_constituent ){
      m_truth_fatjet_numConstituents.push_back( truth_fatjet_itr->numConstituents() );
    }
    if( m_truthFatJetInfoSwitch->m_constituentAll ){
      m_truth_fatjet_constituentWeights.push_back( truth_fatjet_itr->getAttribute< std::vector<float> >( "constituentWeights" ) );
      std::vector<float> pt;
      std::vector<float> eta;
      std::vector<float> phi;
      std::vector<float> e;
      xAOD::JetConstituentVector consVec = truth_fatjet_itr->getConstituents();
      if( consVec.isValid() ) {
	// use the example provided in
        // http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODJet/xAODJet/JetConstituentVector.h
	xAOD::JetConstituentVector::iterator constit = consVec.begin();
	xAOD::JetConstituentVector::iterator constitE = consVec.end();
        for( ; constit != constitE; constit++){
          pt. push_back( constit->pt() / m_units );
          eta.push_back( constit->eta() );
          phi.push_back( constit->phi() );
          e.  push_back( constit->e() / m_units  );
        }
      }
      m_truth_fatjet_constituent_pt.push_back( pt  );
      m_truth_fatjet_constituent_eta.push_back( eta );
      m_truth_fatjet_constituent_phi.push_back( phi );
      m_truth_fatjet_constituent_e.push_back( e   );
    }

    if( m_truthFatJetInfoSwitch->m_truth && m_isMC){
      static SG::AuxElement::ConstAccessor< int > truthfatjet_TQuarks("GhostTQuarksFinalCount");
      if(truthfatjet_TQuarks.isAvailable(*truth_fatjet_itr)){m_truth_fatjet_GhostTQuarks.push_back(truthfatjet_TQuarks(*truth_fatjet_itr));} else{m_truth_fatjet_GhostTQuarks.push_back(-999);}
      static SG::AuxElement::ConstAccessor< int > truthfatjet_WBosons("GhostWBosonsCount");
      if(truthfatjet_WBosons.isAvailable(*truth_fatjet_itr)){m_truth_fatjet_GhostWBosons.push_back(truthfatjet_WBosons(*truth_fatjet_itr));} else{m_truth_fatjet_GhostWBosons.push_back(-999);}
      static SG::AuxElement::ConstAccessor< int > truthfatjet_ZBosons("GhostZBosonsCount");
      if(truthfatjet_ZBosons.isAvailable(*truth_fatjet_itr)){m_truth_fatjet_GhostZBosons.push_back(truthfatjet_ZBosons(*truth_fatjet_itr));} else{m_truth_fatjet_GhostZBosons.push_back(-999);}
      static SG::AuxElement::ConstAccessor< int > truthfatjet_HBosons("GhostHBosonsCount");
      if(truthfatjet_HBosons.isAvailable(*truth_fatjet_itr)){m_truth_fatjet_GhostHBosons.push_back(truthfatjet_HBosons(*truth_fatjet_itr));} else{m_truth_fatjet_GhostHBosons.push_back(-999);}
    }

    this->FillTruthFatJetsUser(truth_fatjet_itr);

    m_ntruthfatjet++;

  } // loop over fat jets

}

void HelpTreeBase::ClearFatJets(const std::string& fatjetName, const std::string& suffix) {
  const std::string collectionName = FatJetCollectionName(fatjetName, suffix);

  m_nfatjet[collectionName] = 0;
  if( m_fatJetInfoSwitch->m_kinematic ){
    m_fatjet_pt[collectionName].clear();
    m_fatjet_eta[collectionName].clear();
    m_fatjet_phi[collectionName].clear();
    m_fatjet_E[collectionName].clear();
    m_fatjet_m[collectionName].clear();
  }
  if( m_fatJetInfoSwitch->m_substructure ){
    m_fatjet_Split12[collectionName].clear();
    m_fatjet_Split23[collectionName].clear();
    m_fatjet_Split34[collectionName].clear();
    m_fatjet_tau1_wta[collectionName].clear();
    m_fatjet_tau2_wta[collectionName].clear();
    m_fatjet_tau3_wta[collectionName].clear();
    m_fatjet_tau21_wta[collectionName].clear();
    m_fatjet_tau32_wta[collectionName].clear();
    m_fatjet_ECF1[collectionName].clear();
    m_fatjet_ECF2[collectionName].clear();
    m_fatjet_ECF3[collectionName].clear();
    m_fatjet_C2[collectionName].clear();
    m_fatjet_D2[collectionName].clear();
    m_fatjet_NTrimSubjets[collectionName].clear();
  }

  if( m_fatJetInfoSwitch->m_constituent ){
    m_fatjet_numConstituents[collectionName].clear();
  }
  if( m_fatJetInfoSwitch->m_constituentAll ){
    m_fatjet_constituentWeights[collectionName].clear();
    m_fatjet_constituent_pt[collectionName].clear();
    m_fatjet_constituent_eta[collectionName].clear();
    m_fatjet_constituent_phi[collectionName].clear();
    m_fatjet_constituent_e[collectionName].clear();
  }
}

void HelpTreeBase::ClearTruthFatJets() {

  m_ntruthfatjet = 0;
  if( m_truthFatJetInfoSwitch->m_kinematic ){
    m_truth_fatjet_pt.clear();
    m_truth_fatjet_eta.clear();
    m_truth_fatjet_phi.clear();
    m_truth_fatjet_E.clear();
    m_truth_fatjet_m.clear();
  }
  if( m_truthFatJetInfoSwitch->m_substructure ){
    m_truth_fatjet_Split12.clear();
    m_truth_fatjet_Split23.clear();
    m_truth_fatjet_Split34.clear();
    m_truth_fatjet_tau1_wta.clear();
    m_truth_fatjet_tau2_wta.clear();
    m_truth_fatjet_tau3_wta.clear();
    m_truth_fatjet_tau21_wta.clear();
    m_truth_fatjet_tau32_wta.clear();
    m_truth_fatjet_ECF1.clear();
    m_truth_fatjet_ECF2.clear();
    m_truth_fatjet_ECF3.clear();
    m_truth_fatjet_C2.clear();
    m_truth_fatjet_D2.clear();
    m_truth_fatjet_NTrimSubjets.clear();
  }
  if( m_truthFatJetInfoSwitch->m_constituent ){
    m_truth_fatjet_numConstituents.clear();
  }
  if( m_truthFatJetInfoSwitch->m_constituentAll ){
    m_truth_fatjet_constituentWeights.clear();
    m_truth_fatjet_constituent_pt.clear();
    m_truth_fatjet_constituent_eta.clear();
    m_truth_fatjet_constituent_phi.clear();
    m_truth_fatjet_constituent_e.clear();
  }
  if( m_truthFatJetInfoSwitch->m_truth ){
    m_truth_fatjet_GhostTQuarks.clear();
    m_truth_fatjet_GhostWBosons.clear();
    m_truth_fatjet_GhostZBosons.clear();
    m_truth_fatjet_GhostHBosons.clear();
  }
}

void HelpTreeBase::ClearEvent() {
  m_eventInfo->clear();
}


/*********************
 *
 *   TAUS
 *
 ********************/

void HelpTreeBase::AddTaus(const std::string detailStr) {

  m_tauInfoSwitch = new HelperClasses::TauInfoSwitch( detailStr );

  // always
  m_tree->Branch("ntau",   &m_ntau, "ntau/I");

  if ( m_tauInfoSwitch->m_kinematic ) {
    m_tree->Branch("tau_pt",     &m_tau_pt);
    m_tree->Branch("tau_phi",    &m_tau_phi);
    m_tree->Branch("tau_eta",    &m_tau_eta);
    m_tree->Branch("tau_m",      &m_tau_m);
    m_tree->Branch("tau_ntrk",   &m_tau_ntrk);
    m_tree->Branch("tau_charge", &m_tau_charge);
  }

  this->AddTausUser();
}

void HelpTreeBase::FillTaus( const xAOD::TauJetContainer* taus ) {

  this->ClearTaus();
  this->ClearTausUser();

  m_ntau = 0;
  for ( auto tau_itr : *(taus) ) {

    if ( m_debug ) { Info("HelpTreeBase::FillTaus()", "Filling tau w/ pT = %2f", tau_itr->pt() / m_units ); }

    if ( m_tauInfoSwitch->m_kinematic ) {
      m_tau_pt.push_back ( tau_itr->pt() / m_units  );
      m_tau_eta.push_back( tau_itr->eta() );
      m_tau_phi.push_back( tau_itr->phi() );
      m_tau_m.push_back  ( tau_itr->m() / m_units  );
      m_tau_charge.push_back( tau_itr->charge() );
      m_tau_ntrk.push_back( tau_itr->nTracks() );
    }

    this->FillTausUser(tau_itr);

    m_ntau++;
  }
}

void HelpTreeBase::ClearTaus() {

  m_ntau = 0;

  if ( m_tauInfoSwitch->m_kinematic ){
    m_tau_pt.clear();
    m_tau_eta.clear();
    m_tau_phi.clear();
    m_tau_m.clear();
    m_tau_charge.clear();
    m_tau_ntrk.clear();
  }

}



/*********************
 *
 *     MET
 *
 ********************/
void HelpTreeBase::AddMET( const std::string detailStr ) {

  if(m_debug) Info("AddMET()", "Adding MET variables: %s", detailStr.c_str());

  m_metInfoSwitch = new HelperClasses::METInfoSwitch( detailStr );

  m_tree->Branch("metFinalClus",	 &m_metFinalClus,      "metFinalClus/F");
  m_tree->Branch("metFinalClusPx",       &m_metFinalClusPx,    "metFinalClusPx/F");
  m_tree->Branch("metFinalClusPy",       &m_metFinalClusPy,    "metFinalClusPy/F");
  m_tree->Branch("metFinalClusSumEt",    &m_metFinalClusSumEt, "metFinalClusSumEt/F");
  m_tree->Branch("metFinalClusPhi",      &m_metFinalClusPhi,   "metFinalClusPhi/F");

  m_tree->Branch("metFinalTrk",	         &m_metFinalTrk,       "metFinalTrk/F");
  m_tree->Branch("metFinalTrkPx",        &m_metFinalTrkPx,     "metFinalTrkPx/F");
  m_tree->Branch("metFinalTrkPy",        &m_metFinalTrkPy,     "metFinalTrkPy/F");
  m_tree->Branch("metFinalTrkSumEt",     &m_metFinalTrkSumEt,  "metFinalTrkSumEt/F");
  m_tree->Branch("metFinalTrkPhi",       &m_metFinalTrkPhi,    "metFinalTrkPhi/F");

  if ( m_metInfoSwitch->m_refEle ) {
    m_tree->Branch("metEle",             &m_metEle,	       "metEle/F");
    m_tree->Branch("metEleSumEt",        &m_metEleSumEt,       "metEleSumEt/F");
    m_tree->Branch("metElePhi",          &m_metElePhi,         "metElePhi/F");
  }
  if ( m_metInfoSwitch->m_refGamma ) {
    m_tree->Branch("metGamma",       	 &m_metGamma,	       "metGamma/F");
    m_tree->Branch("metGammaSumEt",  	 &m_metGammaSumEt,     "metGammaSumEt/F");
    m_tree->Branch("metGammaPhi",    	 &m_metGammaPhi,       "metGammaPhi/F");
  }
  if ( m_metInfoSwitch->m_refTau ) {
    m_tree->Branch("metTau",             &m_metTau,	       "metTau/F");
    m_tree->Branch("metTauSumEt",        &m_metTauSumEt,       "metTauSumEt/F");
    m_tree->Branch("metTauPhi",          &m_metTauPhi,         "metTauPhi/F");
  }
  if ( m_metInfoSwitch->m_refMuons ) {
    m_tree->Branch("metMuons",           &m_metMuons,	       "metMuons/F");
    m_tree->Branch("metMuonsSumEt",      &m_metMuonsSumEt,     "metMuonsSumEt/F");
    m_tree->Branch("metMuonsPhi",        &m_metMuonsPhi,       "metMuonsPhi/F");
  }
  if ( m_metInfoSwitch->m_refJet ) {
    m_tree->Branch("metJet",             &m_metJet,	       "metJet/F");
    m_tree->Branch("metJetSumEt",        &m_metJetSumEt,       "metJetSumEt/F");
    m_tree->Branch("metJetPhi",          &m_metJetPhi,         "metJetPhi/F");
  }
  if ( m_metInfoSwitch->m_refJetTrk ) {
    m_tree->Branch("metJetTrk",          &m_metJetTrk,         "metJetTrk/F");
    m_tree->Branch("metJetTrkSumEt",     &m_metJetTrkSumEt,    "metJetTrkSumEt/F");
    m_tree->Branch("metJetTrkPhi",       &m_metJetTrkPhi,      "metJetTrkPhi/F");
  }
  if ( m_metInfoSwitch->m_softClus) {
    m_tree->Branch("metSoftClus",        &m_metSoftClus,       "metSoftClus/F");
    m_tree->Branch("metSoftClusSumEt",   &m_metSoftClusSumEt,  "metSoftClusSumEt/F");
    m_tree->Branch("metSoftClusPhi",     &m_metSoftClusPhi,    "metSoftClussPhi/F");
  }
  if ( m_metInfoSwitch->m_softTrk) {
    m_tree->Branch("metSoftTrk",         &m_metSoftTrk,        "metSoftTrk/F");
    m_tree->Branch("metSoftTrkSumEt",    &m_metSoftTrkSumEt,   "metSoftTrkSumEt/F");
    m_tree->Branch("metSoftTrkPhi",      &m_metSoftTrkPhi,     "metSoftTrksPhi/F");
  }

  this->AddMETUser();
}

void HelpTreeBase::FillMET( const xAOD::MissingETContainer* met ) {

  // Clear previous events
  this->ClearMET();
  this->ClearMETUser();

  if ( m_debug ) { Info("HelpTreeBase::FillMET()", "Filling MET info"); }

  const xAOD::MissingET* final_clus = *met->find("FinalClus"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
  m_metFinalClus      = final_clus->met() / m_units;
  m_metFinalClusPx    = final_clus->mpx() / m_units;
  m_metFinalClusPy    = final_clus->mpy() / m_units;
  m_metFinalClusSumEt = final_clus->sumet() / m_units;
  m_metFinalClusPhi   = final_clus->phi();

  const xAOD::MissingET* final_trk = *met->find("FinalTrk"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
  m_metFinalTrk	      = final_trk->met() / m_units;
  m_metFinalTrkPx     = final_trk->mpx() / m_units;
  m_metFinalTrkPy     = final_trk->mpy() / m_units;
  m_metFinalTrkSumEt  = final_trk->sumet() / m_units;
  m_metFinalTrkPhi    = final_trk->phi();

  if ( m_metInfoSwitch->m_refEle ) {
    const xAOD::MissingET* ref_ele       = *met->find("RefEle");
    m_metEle         		         = ref_ele->met() / m_units;
    m_metEleSumEt    		         = ref_ele->sumet() / m_units;
    m_metElePhi      		         = ref_ele->phi();
  }
  if ( m_metInfoSwitch->m_refGamma ) {
    const xAOD::MissingET* ref_gamma     = *met->find("RefGamma");
    m_metGamma             	         = ref_gamma->met() / m_units;
    m_metGammaSumEt        	         = ref_gamma->sumet() / m_units;
    m_metGammaPhi          	         = ref_gamma->phi();
  }
  if ( m_metInfoSwitch->m_refTau ) {
    const xAOD::MissingET* ref_tau  	 = *met->find("RefTau");
    m_metTau             	    	 = ref_tau->met() / m_units;
    m_metTauSumEt        	    	 = ref_tau->sumet() / m_units;
    m_metTauPhi          	    	 = ref_tau->phi();
  }
  if ( m_metInfoSwitch->m_refMuons ) {
    const xAOD::MissingET* ref_muon  	 = *met->find("Muons");
    m_metMuons             	     	 = ref_muon->met() / m_units;
    m_metMuonsSumEt        	     	 = ref_muon->sumet() / m_units;
    m_metMuonsPhi          	     	 = ref_muon->phi();
  }
  if ( m_metInfoSwitch->m_refJet ) {
    const xAOD::MissingET* ref_jet  	 = *met->find("RefJet");
    m_metJet             	     	 = ref_jet->met() / m_units;
    m_metJetSumEt        	     	 = ref_jet->sumet() / m_units;
    m_metJetPhi          	     	 = ref_jet->phi();
  }
  if ( m_metInfoSwitch->m_refJetTrk ) {
    const xAOD::MissingET* ref_jet_trk   = *met->find("RefJetTrk");
    m_metJetTrk             	     	 = ref_jet_trk->met() / m_units;
    m_metJetTrkSumEt        	     	 = ref_jet_trk->sumet() / m_units;
    m_metJetTrkPhi          	     	 = ref_jet_trk->phi();
  }
  if ( m_metInfoSwitch->m_softClus) {
    const xAOD::MissingET* ref_soft_clus = *met->find("SoftClus");
    m_metSoftClus            		 = ref_soft_clus->met() / m_units;
    m_metSoftClusSumEt       		 = ref_soft_clus->sumet() / m_units;
    m_metSoftClusPhi         		 = ref_soft_clus->phi();
  }
  if ( m_metInfoSwitch->m_softTrk) {
    const xAOD::MissingET* ref_soft_trk  = *met->find("PVSoftTrk");
    m_metSoftTrk             		 = ref_soft_trk->met() / m_units;
    m_metSoftTrkSumEt        		 = ref_soft_trk->sumet() / m_units;
    m_metSoftTrkPhi          		 = ref_soft_trk->phi();
  }

  this->FillMETUser(met);
}

void HelpTreeBase::ClearMET() {

  m_metFinalClus      = -999;
  m_metFinalClusPx    = -999;
  m_metFinalClusPy    = -999;
  m_metFinalClusSumEt = -999;
  m_metFinalClusPhi   = -999;

  m_metFinalTrk	      = -999;
  m_metFinalTrkPx     = -999;
  m_metFinalTrkPy     = -999;
  m_metFinalTrkSumEt  = -999;
  m_metFinalTrkPhi    = -999;

  if ( m_metInfoSwitch->m_refEle ) {
    m_metEle = m_metEleSumEt = m_metElePhi = -999;
  }
  if ( m_metInfoSwitch->m_refGamma ) {
    m_metGamma = m_metGammaSumEt = m_metGammaPhi = -999;
  }
  if ( m_metInfoSwitch->m_refTau ) {
    m_metTau = m_metTauSumEt = m_metTauPhi = -999;
  }
  if ( m_metInfoSwitch->m_refMuons ) {
    m_metMuons = m_metMuonsSumEt = m_metMuonsPhi = -999;
  }
  if ( m_metInfoSwitch->m_refJet ) {
    m_metJet = m_metJetSumEt = m_metJetPhi = -999;
  }
  if ( m_metInfoSwitch->m_refJetTrk ) {
    m_metJetTrk = m_metJetTrkSumEt = m_metJetTrkPhi = -999;
  }
	  if ( m_metInfoSwitch->m_softClus) {
    m_metSoftClus = m_metSoftClusSumEt = m_metSoftClusPhi = -999;
  }
}


bool HelpTreeBase::writeTo( TFile* file ) {
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if ( status == 0 ) { return false; }
  return true;
}

