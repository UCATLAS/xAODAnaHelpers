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
  m_phInfoSwitch(nullptr),
  m_trigConfTool(nullptr),
  m_trigDecTool(nullptr),
  m_eventInfo(nullptr),
  m_met(nullptr)
{

  m_units = units;
  m_debug = debug;
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
    for (auto elec: m_elecs)
      delete elec.second;

    //ph
    delete m_phInfoSwitch;

    //fatjet
    for (auto fatjet: m_fatjets)
      delete fatjet.second;

    //tau
    for (auto tau: m_taus)
      delete tau.second;

    //met
    delete m_met;

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

  this->ClearMuonsUser(muonName);

}

/*********************
 *
 *   ELECTRONS
 *
 ********************/

void HelpTreeBase::AddElectrons(const std::string detailStr, const std::string elecName) {

  if(m_debug)  Info("AddElectrons()", "Adding electron variables: %s", detailStr.c_str());

  m_elecs[elecName] = new xAH::ElectronContainer(elecName, detailStr, m_units, m_isMC);

  xAH::ElectronContainer* thisElec = m_elecs[elecName];

  thisElec->setBranches(m_tree);
  this->AddElectronsUser();
}


void HelpTreeBase::FillElectrons( const xAOD::ElectronContainer* electrons, const xAOD::Vertex* primaryVertex, const std::string elecName ) {

  this->ClearElectrons(elecName);

  for ( auto el_itr : *electrons ) {
    this->FillElectron(el_itr, primaryVertex, elecName);
  }
}

void HelpTreeBase::FillElectron ( const xAOD::Electron* elec, const xAOD::Vertex* primaryVertex, const std::string elecName ) {

  xAH::ElectronContainer* thisElec = m_elecs[elecName];

  thisElec->FillElectron(elec, primaryVertex);

  this->FillElectronsUser(elec, elecName);

  return;
}


void HelpTreeBase::ClearElectrons(const std::string elecName) {

  xAH::ElectronContainer* thisElec = m_elecs[elecName];
  thisElec->clear();

  this->ClearElectronsUser(elecName);
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
  float truthparticle_etamax = 8.0;

  for( auto truth_itr : *truthParts ) {
    
    if((truth_itr->pt() / m_units < truthparticle_ptmin) || (fabs(truth_itr->eta()) > truthparticle_etamax) ){
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

  const std::string collectionName = FatJetCollectionName(fatjetName, suffix);
  m_fatjets[collectionName] = new xAH::FatJetContainer(fatjetName, detailStr, suffix, m_units, m_isMC);

  xAH::FatJetContainer* thisFatJet = m_fatjets[collectionName];
  thisFatJet->setBranches(m_tree);

  this->AddFatJetsUser(detailStr, fatjetName, suffix);
}

void HelpTreeBase::AddTruthFatJets(const std::string& detailStr, const std::string& truthFatJetName) {

  if(m_debug) Info("AddTruthFatJets()", "Adding fat jet variables: %s", detailStr.c_str());

  m_truth_fatjets[truthFatJetName] = new xAH::FatJetContainer(truthFatJetName, detailStr, "", m_units, m_isMC);

  xAH::FatJetContainer* thisTruthFatJet = m_truth_fatjets[truthFatJetName];
  thisTruthFatJet->setBranches(m_tree);

  this->AddTruthFatJetsUser(detailStr, truthFatJetName);
}


void HelpTreeBase::FillFatJets( const xAOD::JetContainer* fatJets , const std::string& fatjetName, const std::string& suffix) {
  
  this->ClearFatJets(fatjetName, suffix);

  for( auto fatjet_itr : *fatJets ) {
    
    this->FillFatJet(fatjet_itr, fatjetName, suffix);

  } // loop over fat jets

}

void HelpTreeBase::FillFatJet( const xAOD::Jet* fatjet_itr, const std::string& fatjetName, const std::string& suffix ) {

  const std::string collectionName = FatJetCollectionName(fatjetName, suffix);
  xAH::FatJetContainer* thisFatJet = m_fatjets[collectionName];

  thisFatJet->FillFatJet(fatjet_itr);

  this->FillFatJetsUser(fatjet_itr, fatjetName, suffix);

  return;
}



void HelpTreeBase::FillTruthFatJets( const xAOD::JetContainer* truthTruthFatJets, const std::string& truthFatJetName ) {
  this->ClearTruthFatJets();

  for( auto truth_fatjet_itr : *truthTruthFatJets ) {

    this->FillTruthFatJet(truth_fatjet_itr, truthFatJetName);

  } // loop over fat jets

}

void HelpTreeBase::FillTruthFatJet( const xAOD::Jet* truth_fatjet_itr, const std::string& truthFatJetName ) {

  xAH::FatJetContainer* thisTruthFatJet = m_truth_fatjets[truthFatJetName];

  thisTruthFatJet->FillFatJet(truth_fatjet_itr);

  this->FillTruthFatJetsUser(truth_fatjet_itr, truthFatJetName);

  return;
}


void HelpTreeBase::ClearFatJets(const std::string& fatjetName, const std::string& suffix) {
  const std::string collectionName = FatJetCollectionName(fatjetName, suffix);

  xAH::FatJetContainer* thisFatJet = m_fatjets[collectionName];
  thisFatJet->clear();

  this->ClearFatJetsUser(fatjetName, suffix);
}

void HelpTreeBase::ClearTruthFatJets(const std::string& truthFatJetName) {

  xAH::FatJetContainer* thisTruthFatJet = m_truth_fatjets[truthFatJetName];
  thisTruthFatJet->clear();

  this->ClearTruthFatJetsUser(truthFatJetName);
}

void HelpTreeBase::ClearEvent() {
  m_eventInfo->clear();
}


/*********************
 *
 *   TAUS
 *
 ********************/

void HelpTreeBase::AddTaus(const std::string detailStr, const std::string& tauName) {

  if ( m_debug )  Info("AddTaus()", "Adding tau variables: %s", detailStr.c_str());

  m_taus[tauName] = new xAH::TauContainer(tauName, detailStr, m_units, m_isMC);

  xAH::TauContainer* thisTau = m_taus[tauName];

  thisTau->setBranches(m_tree);
  this->AddTausUser();
}

void HelpTreeBase::FillTaus( const xAOD::TauJetContainer* taus, const std::string tauName ) {

  this->ClearTaus();

  for( auto tau_itr : *taus ) {
    this->FillTau(tau_itr, tauName);
  }
}

void HelpTreeBase::FillTau( const xAOD::TauJet* tau, const std::string tauName ) {

  xAH::TauContainer* thisTau = m_taus[tauName];

  thisTau->FillTau(tau);

  this->FillTausUser(tau, tauName);
}

void HelpTreeBase::ClearTaus(const std::string tauName) {

  xAH::TauContainer* thisTau = m_taus[tauName];
  thisTau->clear();
  
  this->ClearTausUser(tauName);
  
}



/*********************
 *
 *     MET
 *
 ********************/
void HelpTreeBase::AddMET( const std::string detailStr ) {

  if(m_debug) Info("AddMET()", "Adding MET variables: %s", detailStr.c_str());

  m_met = new xAH::MetContainer(detailStr, m_units);
  m_met -> setBranches(m_tree);
  this->AddMETUser();
}

void HelpTreeBase::FillMET( const xAOD::MissingETContainer* met ) {

  // Clear previous events
  this->ClearMET();
  this->ClearMETUser();

  m_met->FillMET(met);

  this->FillMETUser(met);
}

void HelpTreeBase::ClearMET() {
  m_met->clear();
}


bool HelpTreeBase::writeTo( TFile* file ) {
  file->cd(); // necessary?
  int status( m_tree->Write() );
  if ( status == 0 ) { return false; }
  return true;
}

