#include <xAODAnaHelpers/JetHists.h>
#include <sstream>

#include "xAODAnaHelpers/tools/ReturnCheck.h"


JetHists :: JetHists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr),
  m_infoSwitch(new HelperClasses::JetInfoSwitch(m_detailStr))
{
  m_debug = false;
}

JetHists :: ~JetHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode JetHists::initialize() {

  // These plots are always made
  m_jetPt          = book(m_name, "jetPt",   "jet p_{T} [GeV]", 120, 0, 3000.);
  m_jetPt_s        = book(m_name, "jetPt_s", "jet p_{T} [GeV]", 120, 0, 300.);
  m_jetEta         = book(m_name, "jetEta", "jet #eta",         80, -4, 4);
  m_jetPhi         = book(m_name, "jetPhi", "jet Phi",120, -TMath::Pi(), TMath::Pi() );
  m_jetM           = book(m_name, "jetMass", "jet Mass [GeV]",120, 0, 400);
  m_jetE           = book(m_name, "jetEnergy", "jet Energy [GeV]",120, 0, 4000.);
  m_jetRapidity    = book(m_name, "jetRapidity", "jet Rapidity",120, -10, 10);

  if(m_debug) Info("JetHists::initialize()", m_name.c_str());
  // details of the jet kinematics
  if( m_infoSwitch->m_kinematic ) {
    if(m_debug) Info("JetHists::initialize()", "adding kinematic plots");
    m_jetPx     = book(m_name, "jetPx",     "jet Px [GeV]",     120, 0, 1000);
    m_jetPy     = book(m_name, "jetPy",     "jet Py [GeV]",     120, 0, 1000);
    m_jetPz     = book(m_name, "jetPz",     "jet Pz [GeV]",     120, 0, 4000);
  }

  // N leading jets
  if( m_infoSwitch->m_numLeadingJets > 0 ){
    std::stringstream jetNum;
    for(int iJet=0; iJet < m_infoSwitch->m_numLeadingJets; ++iJet){
      jetNum << iJet;
      m_NjetsPt.push_back(       book(m_name, ("jetPt_jet"+jetNum.str()),       "jet p_{T} [GeV]", 120, 0, 3000.) );
      m_NjetsEta.push_back(      book(m_name, ("jetEta_jet"+jetNum.str()),      "jet #eta",         80, -4, 4) );
      m_NjetsPhi.push_back(      book(m_name, ("jetPhi_jet"+jetNum.str()),      "jet Phi",120, -TMath::Pi(), TMath::Pi() ) );
      m_NjetsM.push_back(        book(m_name, ("jetMass_jet"+jetNum.str()),     "jet Mass [GeV]",120, 0, 400) );
      m_NjetsE.push_back(        book(m_name, ("jetEnergy_jet"+jetNum.str()),   "jet Energy [GeV]",120, 0, 4000.) );
      m_NjetsRapidity.push_back( book(m_name, ("jetRapidity_jet"+jetNum.str()), "jet Rapidity",120, -10, 10) );
      jetNum.str("");
    }//for iJet
  }


  // details for jet cleaning
  if( m_infoSwitch->m_clean ) {
    if(m_debug) Info("JetHists::initialize()", "adding clean plots");
    // units?
    m_jetTime     = book(m_name, "JetTimming" ,   "Jet Timming",      120, -80, 80);
    m_LArQuality  = book(m_name, "LArQuality" ,   "LAr Quality",      120, -600, 600);
    m_hecq        = book(m_name, "HECQuality" ,   "HEC Quality",      120, -10, 10);
    m_negE        = book(m_name, "NegativeE" ,    "Negative Energy",  120, -10, 10);
    m_avLArQF     = book(m_name, "AverageLArQF" , "<LAr Quality Factor>" , 120, 0, 1000);
    m_bchCorrCell = book(m_name, "BchCorrCell" ,  "BCH Corr Cell" ,   120, 0, 600);
    m_N90Const    = book(m_name, "N90Constituents", "N90 Constituents" ,  120, 0, 40);
  }

  // details for jet energy information
  if( m_infoSwitch->m_energy ) {
    if(m_debug) Info("JetHists::initialize()", "adding energy plots");
    m_HECf      = book(m_name, "HECFrac",         "HEC Fraction" ,    120, 0, 5);
    m_EMf       = book(m_name, "EMFrac",          "EM Fraction" ,     120, 0, 2);
    m_actArea   = book(m_name, "ActiveArea",      "Jet Active Area" , 120, 0, 1);
    m_centroidR = book(m_name, "CentroidR",       "CentroidR" ,       120, 0, 600);
  }

  // details for jet energy in each layer
  // plotted as fraction instead of absolute to make the plotting easier
  if( m_infoSwitch->m_layer ) {
    m_PreSamplerB  = book(m_name, "PreSamplerB",   "Pre sample barrel", 120, -0.1, 1.1);
    m_EMB1 = book(m_name, "EMB1", "EM Barrel  1", 120, -0.1, 1.1);
    m_EMB2 = book(m_name, "EMB2", "EM Barrel  2", 120, -0.1, 1.1);
    m_EMB3 = book(m_name, "EMB3", "EM Barrel  3", 120, -0.1, 1.1);
    m_PreSamplerE  = book(m_name, "PreSamplerE",   "Pre sample end cap", 120, -0.1, 1.1);
    m_EME1 = book(m_name, "EME1", "EM Endcap  1", 120, -0.1, 1.1);
    m_EME2 = book(m_name, "EME2", "EM Endcap  2", 120, -0.1, 1.1);
    m_EME3 = book(m_name, "EME3", "EM Endcap  3", 120, -0.1, 1.1);
    m_HEC0 = book(m_name, "HEC0", "Hadronic Endcap  0", 120, -0.1, 1.1);
    m_HEC1 = book(m_name, "HEC1", "Hadronic Endcap  1", 120, -0.1, 1.1);
    m_HEC2 = book(m_name, "HEC2", "Hadronic Endcap  2", 120, -0.1, 1.1);
    m_HEC3 = book(m_name, "HEC3", "Hadronic Endcap  3", 120, -0.1, 1.1);
    m_TileBar0 = book(m_name, "TileBar0", "Tile Barrel  0", 120, -0.1, 1.1);
    m_TileBar1 = book(m_name, "TileBar1", "Tile Barrel  1", 120, -0.1, 1.1);
    m_TileBar2 = book(m_name, "TileBar2", "Tile Barrel  2", 120, -0.1, 1.1);
    m_TileGap1 = book(m_name, "TileGap1", "Tile Gap  1", 120, -0.1, 1.1);
    m_TileGap2 = book(m_name, "TileGap2", "Tile Gap  2", 120, -0.1, 1.1);
    m_TileGap3 = book(m_name, "TileGap3", "Tile Gap  3", 120, -0.1, 1.1);
    m_TileExt0 = book(m_name, "TileExt0", "Tile extended barrel  0", 120, -0.1, 1.1);
    m_TileExt1 = book(m_name, "TileExt1", "Tile extended barrel  1", 120, -0.1, 1.1);
    m_TileExt2 = book(m_name, "TileExt2", "Tile extended barrel  2", 120, -0.1, 1.1);
    m_FCAL0 = book(m_name, "FCAL0", "Foward EM endcap  0", 120, -0.1, 1.1);
    m_FCAL1 = book(m_name, "FCAL1", "Foward EM endcap  1", 120, -0.1, 1.1);
    m_FCAL2 = book(m_name, "FCAL2", "Foward EM endcap  2", 120, -0.1, 1.1);

//      LAr calo barrel
//      PreSamplerB 0
//      EMB1 1
//      EMB2 2
//      EMB3 3
//      LAr calo endcap
//      PreSamplerE 4
//      EME1  5
//      EME2  6
//      EME3  7
//      Hadronic endcap
//      HEC0  8
//      HEC1  9
//      HEC2  10
//      HEC3  11
//      Tile barrel
//      TileBar0  12
//      TileBar1  13
//      TileBar2  14
//      Tile gap (ITC & scint)
//      TileGap1  15
//      TileGap2  16
//      TileGap3  17
//      Tile extended barrel
//      TileExt0  18
//      TileExt1  19
//      TileExt2  20
//      Forward EM endcap
//      FCAL0 21
//      FCAL1 22
//      FCAL2 23
//      Mini FCAL
//      MINIFCAL0 24
//      MINIFCAL1 25
//      MINIFCAL2 26
//      MINIFCAL3 27

  }

  m_chf         = book(m_name, "chfPV" ,    "PV(chf)" ,     120, 0, 600);

  // details for jet resolutions
  if( m_infoSwitch->m_resolution ) {
    if(m_debug) Info("JetHists::initialize()", "adding resolution plots");
    // 1D
    m_jetGhostTruthPt   = book(m_name, "jetGhostTruthPt",  "jet ghost truth p_{T} [GeV]", 120, 0, 600);
    // 2D
    m_jetPt_vs_resolution = book(m_name, "jetPt_vs_resolution",
      "jet p_{T} [GeV]", 120, 0, 600,
      "resolution", 30, -5, 35
    );
    m_jetGhostTruthPt_vs_resolution = book(m_name, "jetGhostTruthPt_vs_resolution",
      "jet ghost truth p_{T} [GeV]", 120, 0, 600,
      "resolution", 30, -5, 35
    );
  }

  // details for jet energy information
  if( m_infoSwitch->m_truth ) {
    if(m_debug) Info("JetHists::initialize()", "adding truth plots");

    m_truthLabelID   = book(m_name, "TruthLabelID",        "Truth Label" ,          30,  -0.5,  29.5);
    m_truthCount     = book(m_name, "TruthCount",          "Truth Count" ,          50,  -0.5,  49.5);
    m_truthPt        = book(m_name, "TruthPt",             "Truth Pt",              100,   0,   100.0);

    m_truthDr_B            = book(m_name, "TruthLabelDeltaR_B",   "Truth Label dR(b)" ,          120, -0.1,   1.0);
    m_truthDr_C      = book(m_name, "TruthLabelDeltaR_C",  "Truth Label dR(c)" ,    120, -0.1, 1.0);
    m_truthDr_T      = book(m_name, "TruthLabelDeltaR_T",  "Truth Label dR(tau)" ,  120, -0.1, 1.0);

  }

  if( m_infoSwitch->m_truthDetails ) {
    if(m_debug) Info("JetHists::initialize()", "adding detailed truth plots");

    m_truthCount_BhadFinal = book(m_name, "GhostBHadronsFinalCount",    "Truth Count BHad (final)" ,    10, -0.5,   9.5);
    m_truthCount_BhadInit  = book(m_name, "GhostBHadronsInitialCount",  "Truth Count BHad (initial)" ,  10, -0.5,   9.5);
    m_truthCount_BQFinal   = book(m_name, "GhostBQuarksFinalCount",     "Truth Count BQuark (final)" ,  10, -0.5,   9.5);
    m_truthPt_BhadFinal    = book(m_name, "GhostBHadronsFinalPt",       "Truth p_{T} BHad (final)" ,      100,    0,   100);
    m_truthPt_BhadInit     = book(m_name, "GhostBHadronsInitialPt",     "Truth p_{T} BHad (initial)" ,    100,    0,   100);
    m_truthPt_BQFinal      = book(m_name, "GhostBQuarksFinalPt",        "Truth p_{T} BQuark (final)" ,    100,    0,   100);

    m_truthCount_ChadFinal = book(m_name, "GhostCHadronsFinalCount",   "Truth Count CHad (final)" ,    10, -0.5,   9.5);
    m_truthCount_ChadInit  = book(m_name, "GhostCHadronsInitialCount", "Truth Count CHad (initial)" ,  10, -0.5,   9.5);
    m_truthCount_CQFinal   = book(m_name, "GhostCQuarksFinalCount",    "Truth Count CQuark (final)" ,  10, -0.5,   9.5);
    m_truthPt_ChadFinal    = book(m_name, "GhostCHadronsFinalPt",      "Truth p_{T} CHad (final)" ,      100,    0,   100);
    m_truthPt_ChadInit     = book(m_name, "GhostCHadronsInitialPt",    "Truth p_{T} CHad (initial)" ,    100,    0,   100);
    m_truthPt_CQFinal      = book(m_name, "GhostCQuarksFinalPt",       "Truth p_{T} CQuark (final)" ,    100,    0,   100);

    m_truthCount_TausFinal = book(m_name, "GhostTausFinalCount", "Truth Count Taus (final)" ,    10, -0.5,   9.5);
    m_truthPt_TausFinal    = book(m_name, "GhostTausFinalPt",    "Truth p_{T} Taus (final)" ,      100,    0,   100);

  }

  if( m_infoSwitch->m_flavTag ) {
    if(m_debug) Info("JetHists::initialize()", "adding btagging plots");

    m_MV1             = book(m_name, "MV1",    "MV1" ,      100,    -0.1,   1.1);
    m_SV1_plus_IP3D   = book(m_name, "SV1_plus_IP3D",    "SV1_plus_IP3D" ,      100,    -0.1,   1.1);
    m_SV0             = book(m_name, "SV0",    "SV0" ,      100,    -20,  200);
    m_SV1             = book(m_name, "SV1",    "SV1" ,      100,    -5,   15);
    m_IP2D            = book(m_name, "IP2D",   "IP2D" ,     100,    -10,   40);
    m_IP3D            = book(m_name, "IP3D",   "IP3D" ,     100,    -20,   40);
    m_JetFitter       = book(m_name, "JetFitter",   "JetFitter" ,     100,    -10,   10);
    m_JetFitterCombNN = book(m_name, "JetFitterCombNN",   "JetFitterCombNN" ,     100,    -10,   10);
  }


  return StatusCode::SUCCESS;
}

StatusCode JetHists::execute( const xAOD::JetContainer* jets, float eventWeight, int pvLoc ) {
  for( auto jet_itr : *jets ) {
    RETURN_CHECK("JetHists::execute()", this->execute( jet_itr, eventWeight, pvLoc ), "");
  }

  if( m_infoSwitch->m_numLeadingJets > 0){

    int numJets = std::min( m_infoSwitch->m_numLeadingJets, (int)jets->size() );
    for(int iJet=0; iJet < numJets; ++iJet){
      m_NjetsPt.at(iJet)->        Fill( jets->at(iJet)->pt()/1e3,   eventWeight);
      m_NjetsEta.at(iJet)->       Fill( jets->at(iJet)->eta(),      eventWeight);
      m_NjetsPhi.at(iJet)->       Fill( jets->at(iJet)->phi(),      eventWeight);
      m_NjetsM.at(iJet)->         Fill( jets->at(iJet)->m()/1e3,    eventWeight);
      m_NjetsE.at(iJet)->         Fill( jets->at(iJet)->e()/1e3,    eventWeight);
      m_NjetsRapidity.at(iJet)->  Fill( jets->at(iJet)->rapidity(), eventWeight);
    }
  }

  return StatusCode::SUCCESS;
}

StatusCode JetHists::execute( const xAOD::Jet* jet, float eventWeight, int pvLoc ) {

  //basic
  m_jetPt   ->      Fill( jet->pt()/1e3,    eventWeight );
  m_jetPt_s ->      Fill( jet->pt()/1e3,    eventWeight );
  m_jetEta->        Fill( jet->eta(),       eventWeight );
  m_jetPhi->        Fill( jet->phi(),       eventWeight );
  m_jetM->          Fill( jet->m()/1e3,     eventWeight );
  m_jetE->          Fill( jet->e()/1e3,     eventWeight );
  m_jetRapidity->   Fill( jet->rapidity(),  eventWeight );

  // kinematic
  if( m_infoSwitch->m_kinematic ) {
    m_jetPx->  Fill( jet->px()/1e3,  eventWeight );
    m_jetPy->  Fill( jet->py()/1e3,  eventWeight );
    m_jetPz->  Fill( jet->pz()/1e3,  eventWeight );
  } // fillKinematic

  // clean
  if( m_infoSwitch->m_clean ) {

    static SG::AuxElement::ConstAccessor<float> jetTime ("Timing");
    if( jetTime.isAvailable( *jet ) ) {
      m_jetTime ->  Fill( jetTime( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> LArQuality ("LArQuality");
    if( LArQuality.isAvailable( *jet ) ) {
      m_LArQuality ->  Fill( LArQuality( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> hecq ("HECQuality");
    if( hecq.isAvailable( *jet ) ) {
      m_hecq ->  Fill( hecq( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> negE ("NegativeE");
    if( negE.isAvailable( *jet ) ) {
      m_negE ->  Fill( negE( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> avLArQF ("AverageLArQF");
    if( avLArQF.isAvailable( *jet ) ) {
      m_avLArQF ->  Fill( avLArQF( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> bchCorrCell ("BchCorrCell");
    if( bchCorrCell.isAvailable( *jet ) ) {
      m_bchCorrCell ->  Fill( bchCorrCell( *jet ), eventWeight );
    }

    // 0062       N90Cells?
    static SG::AuxElement::ConstAccessor<float> N90Const ("N90Constituents");
    if( N90Const.isAvailable( *jet ) ) {
      m_N90Const ->  Fill( N90Const( *jet ), eventWeight );
    }


 // 0030       LArBadHVEnergy,
 // 0031       LArBadHVRatio,
 // 0035       NumTowers,
 // 0057       isBadLoose,
 // 0058       isBadMedium,
 // 0059       isBadTight,
 // 0060       isUgly,
 // 0063       OotFracClusters10,
 // 0064       OotFracClusters5,
 // 0065       OotFracCells5,
 // 0066       OotFracCells10,

  } // fillClean

  // energy
  if( m_infoSwitch->m_energy ) {

    static SG::AuxElement::ConstAccessor<float> HECf ("HECFrac");
    if( HECf.isAvailable( *jet ) ) {
      m_HECf ->  Fill( HECf( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> EMf ("EMFrac");
    if( EMf.isAvailable( *jet ) ) {
      m_EMf ->  Fill( EMf( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> centroidR ("CentroidR");
    if( centroidR.isAvailable( *jet ) ) {
      m_centroidR ->  Fill( centroidR( *jet ), eventWeight );
    }

    /*

    static SG::AuxElement::ConstAccessor<float> samplingMax ("SamplingMax");
    if( samplingMax.isAvailable( *jet ) ) {
      m_samplingMax ->  Fill( samplingMax( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> ePerSamp ("EnergyPerSampling");
    if( ePerSamp.isAvailable( *jet ) ) {
      m_ePerSamp ->  Fill( ePerSamp( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> fracSampMax ("FracSamplingMax");
    if( fracSampMax.isAvailable( *jet ) ) {
      m_fracSampMax ->  Fill( fracSampMax( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> lowEtFrac ("LowEtConstituentsFrac");
    if( lowEtFrac.isAvailable( *jet ) ) {
      m_lowEtFrac ->  Fill( lowEtFrac( *jet ), eventWeight );
    }

 // 0036       Offset,
 // 0037       OriginIndex    ,
    */

  }

  if( m_infoSwitch->m_layer ){
    static SG::AuxElement::ConstAccessor< std::vector<float> > ePerSamp ("EnergyPerSampling");
    if( ePerSamp.isAvailable( *jet ) ) {
      std::vector<float> ePerSampVals = ePerSamp( *jet );
      float jetE = jet->e();
      m_PreSamplerB -> Fill( ePerSampVals.at(0) / jetE );
      m_EMB1        -> Fill( ePerSampVals.at(1) / jetE );
      m_EMB2        -> Fill( ePerSampVals.at(2) / jetE );
      m_EMB3        -> Fill( ePerSampVals.at(3) / jetE );
      m_PreSamplerE -> Fill( ePerSampVals.at(4) / jetE );
      m_EME1        -> Fill( ePerSampVals.at(5) / jetE );
      m_EME2        -> Fill( ePerSampVals.at(6) / jetE );
      m_EME3        -> Fill( ePerSampVals.at(7) / jetE );
      m_HEC0        -> Fill( ePerSampVals.at(8) / jetE );
      m_HEC1        -> Fill( ePerSampVals.at(9) / jetE );
      m_HEC2        -> Fill( ePerSampVals.at(10) / jetE );
      m_HEC3        -> Fill( ePerSampVals.at(11) / jetE );
      m_TileBar0    -> Fill( ePerSampVals.at(12) / jetE );
      m_TileBar1    -> Fill( ePerSampVals.at(13) / jetE );
      m_TileBar2    -> Fill( ePerSampVals.at(14) / jetE );
      m_TileGap1    -> Fill( ePerSampVals.at(15) / jetE );
      m_TileGap2    -> Fill( ePerSampVals.at(16) / jetE );
      m_TileGap3    -> Fill( ePerSampVals.at(17) / jetE );
      m_TileExt0    -> Fill( ePerSampVals.at(18) / jetE );
      m_TileExt1    -> Fill( ePerSampVals.at(19) / jetE );
      m_TileExt2    -> Fill( ePerSampVals.at(20) / jetE );
      m_FCAL0       -> Fill( ePerSampVals.at(21) / jetE );
      m_FCAL1       -> Fill( ePerSampVals.at(22) / jetE );
      m_FCAL2       -> Fill( ePerSampVals.at(23) / jetE );
    }
  }



  // area
  /*
  if ( m_fillArea ) {

    static SG::AuxElement::ConstAccessor<int> actArea ("ActiveArea");
    if( actArea.isAvailable( *jet ) ) {
      m_actArea ->  Fill( actArea( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> voroniA ("VoronoiArea");
    if( voroniA.isAvailable( *jet ) ) {
      m_voroniA ->  Fill( voroniA( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> voroniAE ("VoronoiAreaE");
    if( voroniAE.isAvailable( *jet ) ) {
      m_voroniAE ->  Fill( voroniAE( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> voroniAPx ("VoronoiAreaPx");
    if( voroniAPx.isAvailable( *jet ) ) {
      m_voroniAPx ->  Fill( voroniAPx( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> voroniAPy ("CentroidR");
    if( voroniAPy.isAvailable( *jet ) ) {
      m_voroniAPy ->  Fill( voroniAPy( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> voroniAPz ("CentroidR");
    if( voroniAPz.isAvailable( *jet ) ) {
      m_voroniAPz ->  Fill( voroniAPz( *jet ), eventWeight );
    }

  }
  */


  /*
  // tracks
  if ( m_fillTrks ) {
 // 0040       TrackMF,
 // 0041       TrackMFindex,
 // 0049       WIDTH,
 // 0067       NumTrkPt1000,
 // 0068       NumTrkPt500,
 // 0069       SumPtTrkPt1000,
 // 0070       SumPtTrkPt500,
 // 0071       TrackWidthPt1000,
 // 0072       TrackWidthPt500,
 // 0012       GhostTrackCount,
 // 0011       GhostMuonSegmentCount,
 // 0027       HighestJVFVtx,
 } */



  /*
  // isolation
  if ( m_fillIso ) {
 // 0024       IsoKR20Par,
 // 0025       IsoKR20Perp,
  }
  */

  /*
  // substructure
  if( m_fillSubstructure) {
    // 0029       KtDR,
    static SG::AuxElement::ConstAccessor<int> ktDR ("KtDR");
    if( ktDR.isAvailable( *jet ) ) {
      m_ktDR ->  Fill( ktDR( *jet ), eventWeight );
    }
 // 0050       YFlip12,
 // 0051       YFlip13,
 // 0074       Tau1,
 // 0075       Tau2,
 // 0076       Tau3,
 // 0077       Dip12,
 // 0078       Dip13,
 // 0079       Dip23,
 // 0080       DipExcl12,
 //
 // 0081       ThrustMin,
 // 0082       ThrustMaj,
 //
 // 0083       FoxWolfram0,
 // 0084       FoxWolfram1,
 // 0085       FoxWolfram2,
 // 0086       FoxWolfram3,
 // 0087       FoxWolfram4,
 //
 // 0088       Sphericity,
 // 0089       Aplanarity,
  }

  */

  // truth
 // 0073       PtTruth,
 // 0013       GhostTruthParticleCount,
 // 0028       JetLabel,
 // 0042       TruthMF,
 // 0043       TruthMFindex,

  if( m_infoSwitch->m_truth ) {

    static SG::AuxElement::ConstAccessor<int> TruthLabelID ("TruthLabelID");
    if( TruthLabelID.isAvailable( *jet ) ) {
      m_truthLabelID ->  Fill( TruthLabelID( *jet ), eventWeight );
    }else{
      static SG::AuxElement::ConstAccessor<int> PartonTruthLabelID ("PartonTruthLabelID");
      if( PartonTruthLabelID.isAvailable( *jet ) ) {
	m_truthLabelID ->  Fill( PartonTruthLabelID( *jet ), eventWeight );
      }
    }

    static SG::AuxElement::ConstAccessor<int> TruthCount ("TruthCount");
    if( TruthCount.isAvailable( *jet ) ) {
      m_truthCount ->  Fill( TruthCount( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> TruthPt ("TruthPt");
    if( TruthPt.isAvailable( *jet ) ) {
      m_truthPt ->  Fill( TruthPt( *jet )/1000, eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_B ("TruthLabelDeltaR_B");
    if( TruthLabelDeltaR_B.isAvailable( *jet ) ) {
      m_truthDr_B ->  Fill( TruthLabelDeltaR_B( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_C ("TruthLabelDeltaR_C");
    if( TruthLabelDeltaR_C.isAvailable( *jet ) ) {
      m_truthDr_C ->  Fill( TruthLabelDeltaR_C( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> TruthLabelDeltaR_T ("TruthLabelDeltaR_T");
    if( TruthLabelDeltaR_T.isAvailable( *jet ) ) {
      m_truthDr_T ->  Fill( TruthLabelDeltaR_T( *jet ), eventWeight );
    }

  }


  if( m_infoSwitch->m_truthDetails ) {

    //
    // B-Hadron Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostBHadronsFinalCount ("GhostBHadronsFinalCount");
    if( GhostBHadronsFinalCount.isAvailable( *jet ) ) {
      m_truthCount_BhadFinal ->  Fill( GhostBHadronsFinalCount( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> GhostBHadronsInitialCount ("GhostBHadronsInitialCount");
    if( GhostBHadronsInitialCount.isAvailable( *jet ) ) {
      m_truthCount_BhadInit ->  Fill( GhostBHadronsInitialCount( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> GhostBQuarksFinalCount ("GhostBQuarksFinalCount");
    if( GhostBQuarksFinalCount.isAvailable( *jet ) ) {
      m_truthCount_BQFinal ->  Fill( GhostBQuarksFinalCount( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> GhostBHadronsFinalPt ("GhostBHadronsFinalPt");
    if( GhostBHadronsFinalPt.isAvailable( *jet ) ) {
      m_truthPt_BhadFinal ->  Fill( GhostBHadronsFinalPt( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> GhostBHadronsInitialPt ("GhostBHadronsInitialPt");
    if( GhostBHadronsInitialPt.isAvailable( *jet ) ) {
      m_truthPt_BhadInit ->  Fill( GhostBHadronsInitialPt( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> GhostBQuarksFinalPt ("GhostBQuarksFinalPt");
    if( GhostBQuarksFinalPt.isAvailable( *jet ) ) {
      m_truthPt_BQFinal ->  Fill( GhostBQuarksFinalPt( *jet ), eventWeight );
    }


    //
    // C-Hadron Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostCHadronsFinalCount ("GhostCHadronsFinalCount");
    if( GhostCHadronsFinalCount.isAvailable( *jet ) ) {
      m_truthCount_ChadFinal ->  Fill( GhostCHadronsFinalCount( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> GhostCHadronsInitialCount ("GhostCHadronsInitialCount");
    if( GhostCHadronsInitialCount.isAvailable( *jet ) ) {
      m_truthCount_ChadInit ->  Fill( GhostCHadronsInitialCount( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<int> GhostCQuarksFinalCount ("GhostCQuarksFinalCount");
    if( GhostCQuarksFinalCount.isAvailable( *jet ) ) {
      m_truthCount_CQFinal ->  Fill( GhostCQuarksFinalCount( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> GhostCHadronsFinalPt ("GhostCHadronsFinalPt");
    if( GhostCHadronsFinalPt.isAvailable( *jet ) ) {
      m_truthPt_ChadFinal ->  Fill( GhostCHadronsFinalPt( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> GhostCHadronsInitialPt ("GhostCHadronsInitialPt");
    if( GhostCHadronsInitialPt.isAvailable( *jet ) ) {
      m_truthPt_ChadInit ->  Fill( GhostCHadronsInitialPt( *jet ), eventWeight );
    }

    static SG::AuxElement::ConstAccessor<float> GhostCQuarksFinalPt ("GhostCQuarksFinalPt");
    if( GhostCQuarksFinalPt.isAvailable( *jet ) ) {
      m_truthPt_CQFinal ->  Fill( GhostCQuarksFinalPt( *jet ), eventWeight );
    }


    //
    // Tau Details
    //
    static SG::AuxElement::ConstAccessor<int> GhostTausFinalCount ("GhostTausFinalCount");
    if( GhostTausFinalCount.isAvailable( *jet ) ) {
      m_truthCount_TausFinal ->  Fill( GhostTausFinalCount( *jet ), eventWeight );
    }


    static SG::AuxElement::ConstAccessor<float> GhostTausFinalPt ("GhostTausFinalPt");
    if( GhostTausFinalPt.isAvailable( *jet ) ) {
      m_truthPt_TausFinal ->  Fill( GhostTausFinalPt( *jet ), eventWeight );
    }


  }

  //
  // BTagging
  //
  if( m_infoSwitch->m_flavTag ) {

    const xAOD::BTagging *btag_info = jet->btagging();
    m_MV1 ->  Fill( btag_info->MV1_discriminant() , eventWeight );
    m_SV1_plus_IP3D ->  Fill( btag_info->MV1_discriminant() , eventWeight );
    m_SV0 ->  Fill( btag_info->SV0_significance3D() , eventWeight );
    m_SV1 ->  Fill( btag_info->SV1_loglikelihoodratio() , eventWeight );
    m_IP2D ->  Fill( btag_info->IP2D_loglikelihoodratio() , eventWeight );
    m_IP3D ->  Fill( btag_info->IP3D_loglikelihoodratio() , eventWeight );
    m_JetFitter ->  Fill( btag_info->JetFitter_loglikelihoodratio() , eventWeight );
    m_JetFitterCombNN ->  Fill( btag_info->JetFitterCombNN_loglikelihoodratio() , eventWeight );

  }




  /*
  std::vector<float> chfs = jet->getAttribute< std::vector<float> >(xAOD::JetAttribute::SumPtTrkPt1000);
  float chf(-1);
  if( pvLoc >= 0 && pvLoc < (int)chfs.size() ) {
    m_chf ->  Fill( chfs.at( pvLoc ) , eventWeight );
  }
  */


  // testing
  if( m_infoSwitch->m_resolution ) {
    //float ghostTruthPt = jet->getAttribute( xAOD::JetAttribute::GhostTruthPt );
    float ghostTruthPt = jet->auxdata< float >( "GhostTruthPt" );
    m_jetGhostTruthPt -> Fill( ghostTruthPt/1e3, eventWeight );
    float resolution = jet->pt()/ghostTruthPt - 1;
    m_jetPt_vs_resolution -> Fill( jet->pt()/1e3, resolution, eventWeight );
    m_jetGhostTruthPt_vs_resolution -> Fill( ghostTruthPt/1e3, resolution, eventWeight );
  }


  return StatusCode::SUCCESS;
}

