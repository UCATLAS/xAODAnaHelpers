#include <xAODAnaHelpers/JetHists.h>
#include <sstream>

#include "xAODAnaHelpers/tools/ReturnCheck.h"

using std::vector;

JetHists :: JetHists (std::string name, std::string detailStr) :
  IParticleHists(name, detailStr, "jet", "jet"),
  m_infoSwitch(new HelperClasses::JetInfoSwitch(m_detailStr))
{ }

JetHists :: ~JetHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}


StatusCode JetHists::initialize() {
  RETURN_CHECK("IParticleHists::initialize()", IParticleHists::initialize(), "");

  if(m_debug) Info("JetHists::initialize()", m_name.c_str());

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

  //m_chf         = book(m_name, "chfPV" ,    "PV(chf)" ,     120, 0, 600);

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

    m_truthLabelID   = book(m_name, "TruthLabelID",        "Truth Label" ,          40,  -10.5,  29.5);
    m_truthCount     = book(m_name, "TruthCount",          "Truth Count" ,          60,  -10.5,  49.5);
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

  if( m_infoSwitch->m_flavTag || m_infoSwitch->m_flavTagHLT ) {
    if(m_debug) Info("JetHists::initialize()", "adding btagging plots");

    m_MV2c00          = book(m_name, "MV2c00", "MV2c00" ,   100,    -1.1,   1.1);
    m_MV2c10          = book(m_name, "MV2c10", "MV2c10" ,   100,    -1.1,   1.1);
    m_MV2c20          = book(m_name, "MV2c20", "MV2c20" ,   100,    -1.1,   1.1);
    m_COMB            = book(m_name, "COMB",   "COMB" ,     100,    -20,   40);
    m_SV0             = book(m_name, "SV0",    "SV0" ,      100,    -20,  200);
    m_SV1             = book(m_name, "SV1",    "SV1" ,      100,    -5,   15);
    m_IP2D            = book(m_name, "IP2D",   "IP2D" ,     100,    -10,   40);
    m_IP3D            = book(m_name, "IP3D",   "IP3D" ,     100,    -20,   40);
    m_JetFitter       = book(m_name, "JetFitter",   "JetFitter" ,     100,    -10,   10);
    m_JetFitterCombNN = book(m_name, "JetFitterCombNN",   "JetFitterCombNN" ,     100,    -10,   10);
  }

  if( m_infoSwitch->m_jetFitterDetails ) {
    if(m_debug) Info("JetHists::initialize()", "adding JetFitter Detail plots");

    m_jf_nVTX           = book(m_name, "JetFitter_nVTX"          , "JetFitter_nVTX"          ,  10,  -0.5,   9.5 );
    m_jf_nSingleTracks  = book(m_name, "JetFitter_nSingleTracks" , "JetFitter_nSingleTracks" ,  10,  -0.5,   9.5 );
    m_jf_nTracksAtVtx   = book(m_name, "JetFitter_nTracksAtVtx"  , "JetFitter_nTracksAtVtx"  ,  20,  -0.5,  19.5 );
    m_jf_mass           = book(m_name, "JetFitter_mass"          , "JetFitter_mass"          , 100,   0,     10 );
    m_jf_energyFraction = book(m_name, "JetFitter_energyFraction", "JetFitter_energyFraction", 100,  -0.1,   1.1 );
    m_jf_significance3d = book(m_name, "JetFitter_significance3d", "JetFitter_significance3d", 100,   0,    100 );
    m_jf_deltaeta       = book(m_name, "JetFitter_deltaeta"      , "JetFitter_deltaeta"      , 100,  -0.2,   0.2);
    m_jf_deltaeta_l     = book(m_name, "JetFitter_deltaeta_l"    , "JetFitter_deltaeta"      , 100,  -0.4,   0.4);
    m_jf_deltaphi       = book(m_name, "JetFitter_deltaphi"      , "JetFitter_deltaphi"      , 100,  -0.2,   0.2);
    m_jf_deltaphi_l     = book(m_name, "JetFitter_deltaphi_l"    , "JetFitter_deltaphi"      , 100,  -0.4,   0.4);
    m_jf_N2Tpar         = book(m_name, "JetFitter_N2Tpair"       , "JetFitter_N2Tpair"       ,  20,  -0.5,  19.5);
    m_jf_pb             = book(m_name, "JetFitter_pb"            , "JetFitter_pb"            , 100,  -0.1,   1);
    m_jf_pc             = book(m_name, "JetFitter_pc"            , "JetFitter_pc"            , 100,  -0.1,   1);
    m_jf_pu             = book(m_name, "JetFitter_pu"            , "JetFitter_pu"            , 100,  -0.1,   1);

  }

  if( m_infoSwitch->m_svDetails ) {
    if(m_debug) Info("JetHists::initialize()", "adding JetFitter Detail plots");
    m_sv0_NGTinSvx  = book(m_name, "SV0_NGTinSvx",   "SV0_NGTinSvx",   20,  -0.5,  19.5);
    m_sv0_N2Tpair   = book(m_name, "SV0_N2Tpair ",   "SV0_N2Tpair ",   40,  -9.5,  29.5);
    m_sv0_massvx    = book(m_name, "SV0_massvx  ",   "SV0_massvx  ",   100, -0.1,    8);
    m_sv0_efracsvx  = book(m_name, "SV0_efracsvx",   "SV0_efracsvx",   100, -0.1,   1.2);
    m_sv0_normdist  = book(m_name, "SV0_normdist",   "SV0_normdist",   100, -10,    70);

    m_sv1_NGTinSvx  = book(m_name, "SV1_NGTinSvx",   "SV1_NGTinSvx",   20,  -0.5,  19.5);
    m_sv1_N2Tpair   = book(m_name, "SV1_N2Tpair ",   "SV1_N2Tpair ",   40,  -9.5,  29.5);
    m_sv1_massvx    = book(m_name, "SV1_massvx  ",   "SV1_massvx  ",   100, -0.1,     8);
    m_sv1_efracsvx  = book(m_name, "SV1_efracsvx",   "SV1_efracsvx",   100, -0.1,   1.2);
    m_sv1_normdist  = book(m_name, "SV1_normdist",   "SV1_normdist",   100, -10,    70);

  }

  if( m_infoSwitch->m_ipDetails ) {
    m_nIP2DTracks               = book(m_name, "nIP2DTracks"              , "nIP2DTracks"            ,  20,  -0.5, 19.5);
    m_IP2D_gradeOfTracks        = book(m_name, "IP2D_gradeOfTracks"       , "IP2D_gradeOfTracks"     ,  20,  -0.5, 19.5);
    m_IP2D_flagFromV0ofTracks   = book(m_name, "IP2D_flagFromV0ofTracks"  , "IP2D_flagFromV0ofTracks",   5,  -0.5,  4.5);
    m_IP2D_valD0wrtPVofTracks   = book(m_name, "IP2D_valD0wrtPVofTracks"  , "IP2D_valD0wrtPVofTracks", 100,  -2.0,  2.0);
    m_IP2D_sigD0wrtPVofTracks   = book(m_name, "IP2D_sigD0wrtPVofTracks"  , "IP2D_sigD0wrtPVofTracks", 100, -15.0, 15.0);
    m_IP2D_sigD0wrtPVofTracks_l = book(m_name, "IP2D_sigD0wrtPVofTracks_l", "IP2D_sigD0wrtPVofTracks", 100, -50.0, 50.0);
    m_IP2D_weightBofTracks      = book(m_name, "IP2D_weightBofTracks"     , "IP2D_weightBofTracks"   , 100,  -0.1, 1.5);
    m_IP2D_weightCofTracks      = book(m_name, "IP2D_weightCofTracks"     , "IP2D_weightCofTracks"   , 100,  -0.1, 1.5);
    m_IP2D_weightUofTracks      = book(m_name, "IP2D_weightUofTracks"     , "IP2D_weightUofTracks"   , 100,  -0.1, 1.5);

    m_nIP3DTracks               = book(m_name, "nIP3DTracks"              , "nIP3DTracks"            ,  20,  -0.5, 19.5);
    m_IP3D_gradeOfTracks        = book(m_name, "IP3D_gradeOfTracks"       , "IP3D_gradeOfTracks"     ,  20,  -0.5, 19.5);
    m_IP3D_flagFromV0ofTracks   = book(m_name, "IP3D_flagFromV0ofTracks"  , "IP3D_flagFromV0ofTracks",   5,  -0.5,  4.5);
    m_IP3D_valD0wrtPVofTracks   = book(m_name, "IP3D_valD0wrtPVofTracks"  , "IP3D_valD0wrtPVofTracks", 100,  -2.0,  2.0);
    m_IP3D_sigD0wrtPVofTracks   = book(m_name, "IP3D_sigD0wrtPVofTracks"  , "IP3D_sigD0wrtPVofTracks", 100, -15.0, 15.0);
    m_IP3D_sigD0wrtPVofTracks_l = book(m_name, "IP3D_sigD0wrtPVofTracks_l", "IP3D_sigD0wrtPVofTracks", 100, -50.0, 50.0);
    m_IP3D_valZ0wrtPVofTracks   = book(m_name, "IP3D_valZ0wrtPVofTracks"  , "IP3D_valZ0wrtPVofTracks", 100,  -2.0,  2.0);
    m_IP3D_sigZ0wrtPVofTracks   = book(m_name, "IP3D_sigZ0wrtPVofTracks"  , "IP3D_sigZ0wrtPVofTracks", 100, -15.0, 15.0);
    m_IP3D_sigZ0wrtPVofTracks_l = book(m_name, "IP3D_sigZ0wrtPVofTracks_l", "IP3D_sigZ0wrtPVofTracks", 100, -50.0, 50.0);
    m_IP3D_weightBofTracks      = book(m_name, "IP3D_weightBofTracks"     , "IP3D_weightBofTracks"   , 100,  -0.1, 1.5);
    m_IP3D_weightCofTracks      = book(m_name, "IP3D_weightCofTracks"     , "IP3D_weightCofTracks"   , 100,  -0.1, 1.5);
    m_IP3D_weightUofTracks      = book(m_name, "IP3D_weightUofTracks"     , "IP3D_weightUofTracks"   , 100,  -0.1, 1.5);

  }

  if( m_infoSwitch->m_substructure ){
    m_tau1                      = book(m_name, "Tau1", "#Tau_{1}", 100, 0, 1.0);
    m_tau2                      = book(m_name, "Tau2", "#Tau_{2}", 100, 0, 1.0);
    m_tau3                      = book(m_name, "Tau3", "#Tau_{3}", 100, 0, 1.0);
    m_tau21                     = book(m_name, "Tau21", "#Tau_{21}", 100, 0, 1.0);
    m_tau32                     = book(m_name, "Tau32", "#Tau_{32}", 100, 0, 1.0);
    m_tau1_wta                  = book(m_name, "Tau1_wta", "#Tau_{1}^{wta}", 100, 0, 1.0);
    m_tau2_wta                  = book(m_name, "Tau2_wta", "#Tau_{2}^{wta}", 100, 0, 1.0);
    m_tau3_wta                  = book(m_name, "Tau3_wta", "#Tau_{3}^{wta}", 100, 0, 1.0);
    m_tau21_wta                 = book(m_name, "Tau21_wta", "#Tau_{21}^{wta}", 100, 0, 1.0);
    m_tau32_wta                 = book(m_name, "Tau32_wta", "#Tau_{32}^{wta}", 100, 0, 1.0);
    m_numConstituents           = book(m_name, "numConstituents", "num. constituents", 501, -0.5, 500.5);
  }

  //
  // Tracks in Jet
  //
  if( m_infoSwitch->m_tracksInJet ){
    m_nTrk                      = book(m_name, "nTrk", "nTrk", 100, -0.5, 99.5);

    m_tracksInJet = new TracksInJetHists(m_name+"trk_", "");
    m_tracksInJet -> initialize( );
  }

  return StatusCode::SUCCESS;
}

StatusCode JetHists::execute( const xAOD::Jet* jet, float eventWeight ) {
  RETURN_CHECK("IParticleHists::execute()", IParticleHists::execute(jet, eventWeight), "");

  if(m_debug) std::cout << "in execute " <<std::endl;

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
    static SG::AuxElement::ConstAccessor< vector<float> > ePerSamp ("EnergyPerSampling");
    if( ePerSamp.isAvailable( *jet ) ) {
      vector<float> ePerSampVals = ePerSamp( *jet );
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
  if( m_infoSwitch->m_flavTag || m_infoSwitch->m_flavTagHLT ) {

    const xAOD::BTagging *btag_info(0);
    if(m_infoSwitch->m_flavTag){
      btag_info = jet->btagging();
    }else if(m_infoSwitch->m_flavTagHLT){
      btag_info = jet->auxdata< const xAOD::BTagging* >("HLTBTag");
    }

    double MV2c00 = -99;
    double MV2c10 = -99;
    double MV2c20 = -99;
    btag_info->MVx_discriminant("MV2c00", MV2c00);
    btag_info->MVx_discriminant("MV2c10", MV2c10);
    btag_info->MVx_discriminant("MV2c20", MV2c20);
    m_MV2c00 ->  Fill( MV2c00, eventWeight );
    m_MV2c10 ->  Fill( MV2c10, eventWeight );
    m_MV2c20 ->  Fill( MV2c20, eventWeight );

    static SG::AuxElement::ConstAccessor<double> SV0_significance3DAcc ("SV0_significance3D");
    if ( SV0_significance3DAcc.isAvailable(*btag_info) ) {
      m_SV0             ->  Fill( btag_info->SV0_significance3D() , eventWeight );
      m_SV1             ->  Fill( btag_info->SV1_loglikelihoodratio() , eventWeight );
      m_IP2D            ->  Fill( btag_info->IP2D_loglikelihoodratio() , eventWeight );
      m_IP3D            ->  Fill( btag_info->IP3D_loglikelihoodratio() , eventWeight );
      m_COMB            ->  Fill( btag_info->SV1_loglikelihoodratio() + btag_info->IP3D_loglikelihoodratio() , eventWeight );
      m_JetFitter       ->  Fill( btag_info->JetFitter_loglikelihoodratio() , eventWeight );
      m_JetFitterCombNN ->  Fill( btag_info->JetFitterCombNN_loglikelihoodratio() , eventWeight );
    }

    if(m_infoSwitch->m_jetFitterDetails){
      static SG::AuxElement::ConstAccessor< int   > jf_nVTXAcc       ("JetFitter_nVTX");
      static SG::AuxElement::ConstAccessor< int   > jf_nSingleTracks ("JetFitter_nSingleTracks");
      static SG::AuxElement::ConstAccessor< int   > jf_nTracksAtVtx  ("JetFitter_nTracksAtVtx");
      static SG::AuxElement::ConstAccessor< float > jf_mass          ("JetFitter_mass");
      static SG::AuxElement::ConstAccessor< float > jf_energyFraction("JetFitter_energyFraction");
      static SG::AuxElement::ConstAccessor< float > jf_significance3d("JetFitter_significance3d");
      static SG::AuxElement::ConstAccessor< float > jf_deltaeta      ("JetFitter_deltaeta");
      static SG::AuxElement::ConstAccessor< float > jf_deltaphi      ("JetFitter_deltaphi");
      static SG::AuxElement::ConstAccessor< int   > jf_N2Tpar        ("JetFitter_N2Tpair");
      static SG::AuxElement::ConstAccessor< double > jf_pb           ("JetFitter_pb");
      static SG::AuxElement::ConstAccessor< double > jf_pc           ("JetFitter_pc");
      static SG::AuxElement::ConstAccessor< double > jf_pu           ("JetFitter_pu");

      if(jf_nVTXAcc.isAvailable       (*btag_info)) m_jf_nVTX           ->Fill(jf_nVTXAcc       (*btag_info), eventWeight);
      if(jf_nSingleTracks.isAvailable (*btag_info)) m_jf_nSingleTracks  ->Fill(jf_nSingleTracks (*btag_info), eventWeight);
      if(jf_nTracksAtVtx.isAvailable  (*btag_info)) m_jf_nTracksAtVtx   ->Fill(jf_nTracksAtVtx  (*btag_info), eventWeight);
      if(jf_mass.isAvailable          (*btag_info)) m_jf_mass           ->Fill(jf_mass          (*btag_info)/1000, eventWeight);
      if(jf_energyFraction.isAvailable(*btag_info)) m_jf_energyFraction ->Fill(jf_energyFraction(*btag_info), eventWeight);
      if(jf_significance3d.isAvailable(*btag_info)) m_jf_significance3d ->Fill(jf_significance3d(*btag_info), eventWeight);
      if(jf_deltaeta.isAvailable      (*btag_info)){
	m_jf_deltaeta       ->Fill(jf_deltaeta      (*btag_info), eventWeight);
	m_jf_deltaeta_l     ->Fill(jf_deltaeta      (*btag_info), eventWeight);
      }
      if(jf_deltaphi.isAvailable      (*btag_info)){
	m_jf_deltaphi       ->Fill(jf_deltaphi      (*btag_info), eventWeight);
	m_jf_deltaphi_l     ->Fill(jf_deltaphi      (*btag_info), eventWeight);
      }
      if(jf_N2Tpar.isAvailable        (*btag_info)) m_jf_N2Tpar         ->Fill(jf_N2Tpar        (*btag_info), eventWeight);
      if(jf_pb.isAvailable            (*btag_info)) m_jf_pb             ->Fill(jf_pb            (*btag_info), eventWeight);
      if(jf_pc.isAvailable            (*btag_info)) m_jf_pc             ->Fill(jf_pc            (*btag_info), eventWeight);
      if(jf_pu.isAvailable            (*btag_info)) m_jf_pu             ->Fill(jf_pu            (*btag_info), eventWeight);
    }


    if(m_infoSwitch->m_svDetails){

      //
      // SV0
      //

      /// @brief SV0 : Number of good tracks in vertex
      static SG::AuxElement::ConstAccessor< int   >   sv0_NGTinSvxAcc     ("SV0_NGTinSvx");
      // @brief SV0 : Number of 2-track pairs
      static SG::AuxElement::ConstAccessor< int   >   sv0_N2TpairAcc      ("SV0_N2Tpair");
      /// @brief SV0 : vertex mass
      static SG::AuxElement::ConstAccessor< float   > sv0_masssvxAcc      ("SV0_masssvx");
      /// @brief SV0 : energy fraction
      static SG::AuxElement::ConstAccessor< float   > sv0_efracsvxAcc     ("SV0_efracsvx");                                                                  	/// @brief SV0 : 3D vertex significance
      static SG::AuxElement::ConstAccessor< float   > sv0_normdistAcc     ("SV0_normdist");

      if(sv0_NGTinSvxAcc .isAvailable(*btag_info)) m_sv0_NGTinSvx -> Fill( sv0_NGTinSvxAcc (*btag_info), eventWeight);
      if(sv0_N2TpairAcc  .isAvailable(*btag_info)) m_sv0_N2Tpair  -> Fill( sv0_N2TpairAcc  (*btag_info), eventWeight);
      if(sv0_masssvxAcc  .isAvailable(*btag_info)) m_sv0_massvx   -> Fill( sv0_masssvxAcc  (*btag_info)/1000, eventWeight);
      if(sv0_efracsvxAcc .isAvailable(*btag_info)) m_sv0_efracsvx -> Fill( sv0_efracsvxAcc (*btag_info), eventWeight);
      if(sv0_normdistAcc .isAvailable(*btag_info)) m_sv0_normdist -> Fill( sv0_normdistAcc (*btag_info), eventWeight);

      //
      // SV1
      //

      /// @brief SV1 : Number of good tracks in vertex
      static SG::AuxElement::ConstAccessor< int   >   sv1_NGTinSvxAcc     ("SV1_NGTinSvx");
      // @brief SV1 : Number of 2-track pairs
      static SG::AuxElement::ConstAccessor< int   >   sv1_N2TpairAcc      ("SV1_N2Tpair");
      /// @brief SV1 : vertex mass
      static SG::AuxElement::ConstAccessor< float   > sv1_masssvxAcc      ("SV1_masssvx");
      /// @brief SV1 : energy fraction
      static SG::AuxElement::ConstAccessor< float   > sv1_efracsvxAcc     ("SV1_efracsvx");                                                                  	/// @brief SV1 : 3D vertex significance
      static SG::AuxElement::ConstAccessor< float   > sv1_normdistAcc     ("SV1_normdist");

      if(sv1_NGTinSvxAcc .isAvailable(*btag_info)) m_sv1_NGTinSvx -> Fill( sv1_NGTinSvxAcc (*btag_info), eventWeight);
      if(sv1_N2TpairAcc  .isAvailable(*btag_info)) m_sv1_N2Tpair  -> Fill( sv1_N2TpairAcc  (*btag_info), eventWeight);
      if(sv1_masssvxAcc  .isAvailable(*btag_info)) m_sv1_massvx   -> Fill( sv1_masssvxAcc  (*btag_info)/1000, eventWeight);
      if(sv1_efracsvxAcc .isAvailable(*btag_info)) m_sv1_efracsvx -> Fill( sv1_efracsvxAcc (*btag_info), eventWeight);
      if(sv1_normdistAcc .isAvailable(*btag_info)) m_sv1_normdist -> Fill( sv1_normdistAcc (*btag_info), eventWeight);

    }


    if(m_infoSwitch->m_ipDetails){

      //
      // IP2D
      //

      /// @brief IP2D: track grade
      static SG::AuxElement::ConstAccessor< vector<int>   >   IP2D_gradeOfTracksAcc     ("IP2D_gradeOfTracks");
      /// @brief IP2D : tracks from V0
      static SG::AuxElement::ConstAccessor< vector<bool>   >  IP2D_flagFromV0ofTracksAcc("IP2D_flagFromV0ofTracks");
      /// @brief IP2D : d0 value with respect to primary vertex
      static SG::AuxElement::ConstAccessor< vector<float>   > IP2D_valD0wrtPVofTracksAcc("IP2D_valD0wrtPVofTracks");
      /// @brief IP2D : d0 significance with respect to primary vertex
      static SG::AuxElement::ConstAccessor< vector<float>   > IP2D_sigD0wrtPVofTracksAcc("IP2D_sigD0wrtPVofTracks");
      /// @brief IP2D : track contribution to B likelihood
      static SG::AuxElement::ConstAccessor< vector<float>   > IP2D_weightBofTracksAcc   ("IP2D_weightBofTracks");
      /// @brief IP2D : track contribution to C likelihood
      static SG::AuxElement::ConstAccessor< vector<float>   > IP2D_weightCofTracksAcc   ("IP2D_weightCofTracks");
      /// @brief IP2D : track contribution to U likelihood
      static SG::AuxElement::ConstAccessor< vector<float>   > IP2D_weightUofTracksAcc   ("IP2D_weightUofTracks");

      if(IP2D_gradeOfTracksAcc .isAvailable(*btag_info)){
	unsigned int nIP2DTracks = IP2D_gradeOfTracksAcc(*btag_info).size();
	m_nIP2DTracks -> Fill( nIP2DTracks, eventWeight);
	for(int grade : IP2D_gradeOfTracksAcc(*btag_info))        m_IP2D_gradeOfTracks->Fill(grade, eventWeight);
      }

      if(IP2D_flagFromV0ofTracksAcc .isAvailable(*btag_info)){
	for(bool flag : IP2D_flagFromV0ofTracksAcc(*btag_info))   m_IP2D_flagFromV0ofTracks->Fill(flag, eventWeight);
      }

      if(IP2D_valD0wrtPVofTracksAcc .isAvailable(*btag_info)){
	for(float d0 : IP2D_valD0wrtPVofTracksAcc(*btag_info))    m_IP2D_valD0wrtPVofTracks->Fill(d0, eventWeight);
      }

      if(IP2D_sigD0wrtPVofTracksAcc .isAvailable(*btag_info)){
	for(float d0Sig : IP2D_sigD0wrtPVofTracksAcc(*btag_info)) {
	  m_IP2D_sigD0wrtPVofTracks  ->Fill(d0Sig, eventWeight);
	  m_IP2D_sigD0wrtPVofTracks_l->Fill(d0Sig, eventWeight);
	}
      }

      if(IP2D_weightBofTracksAcc .isAvailable(*btag_info)){
	for(float weightB : IP2D_weightBofTracksAcc(*btag_info))  m_IP2D_weightBofTracks->Fill(weightB, eventWeight);
      }

      if(IP2D_weightCofTracksAcc .isAvailable(*btag_info)){
	for(float weightC : IP2D_weightCofTracksAcc(*btag_info))  m_IP2D_weightCofTracks->Fill(weightC, eventWeight);
      }

      if(IP2D_weightUofTracksAcc .isAvailable(*btag_info)){
	for(float weightU : IP2D_weightUofTracksAcc(*btag_info))  m_IP2D_weightUofTracks->Fill(weightU, eventWeight);
      }

      //
      // IP3D
      //

      /// @brief IP3D: track grade
      static SG::AuxElement::ConstAccessor< vector<int>   >   IP3D_gradeOfTracksAcc     ("IP3D_gradeOfTracks");
      /// @brief IP3D : tracks from V0
      static SG::AuxElement::ConstAccessor< vector<bool>   >  IP3D_flagFromV0ofTracksAcc("IP3D_flagFromV0ofTracks");
      /// @brief IP3D : d0 value with respect to primary vertex
      static SG::AuxElement::ConstAccessor< vector<float>   > IP3D_valD0wrtPVofTracksAcc("IP3D_valD0wrtPVofTracks");
      /// @brief IP3D : d0 significance with respect to primary vertex
      static SG::AuxElement::ConstAccessor< vector<float>   > IP3D_sigD0wrtPVofTracksAcc("IP3D_sigD0wrtPVofTracks");
      /// @brief IP3D : z0 value with respect to primary vertex
      static SG::AuxElement::ConstAccessor< vector<float>   > IP3D_valZ0wrtPVofTracksAcc("IP3D_valZ0wrtPVofTracks");
      /// @brief IP3D : z0 significance with respect to primary vertex
      static SG::AuxElement::ConstAccessor< vector<float>   > IP3D_sigZ0wrtPVofTracksAcc("IP3D_sigZ0wrtPVofTracks");
      /// @brief IP3D : track contribution to B likelihood
      static SG::AuxElement::ConstAccessor< vector<float>   > IP3D_weightBofTracksAcc   ("IP3D_weightBofTracks");
      /// @brief IP3D : track contribution to C likelihood
      static SG::AuxElement::ConstAccessor< vector<float>   > IP3D_weightCofTracksAcc   ("IP3D_weightCofTracks");
      /// @brief IP3D : track contribution to U likelihood
      static SG::AuxElement::ConstAccessor< vector<float>   > IP3D_weightUofTracksAcc   ("IP3D_weightUofTracks");

      if(IP3D_gradeOfTracksAcc .isAvailable(*btag_info)){
	unsigned int nIP3DTracks = IP3D_gradeOfTracksAcc(*btag_info).size();
	m_nIP3DTracks -> Fill( nIP3DTracks, eventWeight);
	for(int grade : IP3D_gradeOfTracksAcc(*btag_info))        m_IP3D_gradeOfTracks->Fill(grade, eventWeight);
      }

      if(IP3D_flagFromV0ofTracksAcc .isAvailable(*btag_info)){
	for(bool flag : IP3D_flagFromV0ofTracksAcc(*btag_info))   m_IP3D_flagFromV0ofTracks->Fill(flag, eventWeight);
      }

      if(IP3D_valD0wrtPVofTracksAcc .isAvailable(*btag_info)){
	for(float d0 : IP3D_valD0wrtPVofTracksAcc(*btag_info))    m_IP3D_valD0wrtPVofTracks->Fill(d0, eventWeight);
      }

      if(IP3D_sigD0wrtPVofTracksAcc .isAvailable(*btag_info)){
	for(float d0Sig : IP3D_sigD0wrtPVofTracksAcc(*btag_info)){
	  m_IP3D_sigD0wrtPVofTracks  ->Fill(d0Sig, eventWeight);
	  m_IP3D_sigD0wrtPVofTracks_l->Fill(d0Sig, eventWeight);
	}
      }

      if(IP3D_valZ0wrtPVofTracksAcc .isAvailable(*btag_info)){
	for(float z0 : IP3D_valZ0wrtPVofTracksAcc(*btag_info))    m_IP3D_valZ0wrtPVofTracks->Fill(z0, eventWeight);
      }

      if(IP3D_sigZ0wrtPVofTracksAcc .isAvailable(*btag_info)){
	for(float z0Sig : IP3D_sigZ0wrtPVofTracksAcc(*btag_info)){
	  m_IP3D_sigZ0wrtPVofTracks  ->Fill(z0Sig, eventWeight);
	  m_IP3D_sigZ0wrtPVofTracks_l->Fill(z0Sig, eventWeight);
	}
      }

      if(IP3D_weightBofTracksAcc .isAvailable(*btag_info)){
	for(float weightB : IP3D_weightBofTracksAcc(*btag_info))  m_IP3D_weightBofTracks->Fill(weightB, eventWeight);
      }

      if(IP3D_weightCofTracksAcc .isAvailable(*btag_info)){
	for(float weightC : IP3D_weightCofTracksAcc(*btag_info))  m_IP3D_weightCofTracks->Fill(weightC, eventWeight);
      }

      if(IP3D_weightUofTracksAcc .isAvailable(*btag_info)){
	for(float weightU : IP3D_weightUofTracksAcc(*btag_info))  m_IP3D_weightUofTracks->Fill(weightU, eventWeight);
      }



    }
  }




  /*
  vector<float> chfs = jet->getAttribute< vector<float> >(xAOD::JetAttribute::SumPtTrkPt1000);
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

  if( m_infoSwitch->m_substructure ){
    static SG::AuxElement::ConstAccessor<float> Tau1("Tau1");
    static SG::AuxElement::ConstAccessor<float> Tau2("Tau2");
    static SG::AuxElement::ConstAccessor<float> Tau3("Tau3");
    static SG::AuxElement::ConstAccessor<float> Tau1_wta("Tau1_wta");
    static SG::AuxElement::ConstAccessor<float> Tau2_wta("Tau2_wta");
    static SG::AuxElement::ConstAccessor<float> Tau3_wta("Tau3_wta");

    if(Tau1.isAvailable(*jet)) m_tau1->Fill( Tau1(*jet), eventWeight );
    if(Tau2.isAvailable(*jet)) m_tau2->Fill( Tau2(*jet), eventWeight );
    if(Tau3.isAvailable(*jet)) m_tau3->Fill( Tau3(*jet), eventWeight );
    if(Tau1.isAvailable(*jet) && Tau2.isAvailable(*jet)) m_tau21->Fill( Tau2(*jet)/Tau1(*jet), eventWeight );
    if(Tau2.isAvailable(*jet) && Tau3.isAvailable(*jet)) m_tau32->Fill( Tau3(*jet)/Tau2(*jet), eventWeight );
    if(Tau1_wta.isAvailable(*jet)) m_tau1_wta->Fill( Tau1_wta(*jet), eventWeight );
    if(Tau2_wta.isAvailable(*jet)) m_tau2_wta->Fill( Tau2_wta(*jet), eventWeight );
    if(Tau3_wta.isAvailable(*jet)) m_tau3_wta->Fill( Tau3_wta(*jet), eventWeight );
    if(Tau1_wta.isAvailable(*jet) && Tau2_wta.isAvailable(*jet)) m_tau21_wta->Fill( Tau2_wta(*jet)/Tau1_wta(*jet), eventWeight );
    if(Tau2_wta.isAvailable(*jet) && Tau3_wta.isAvailable(*jet)) m_tau32_wta->Fill( Tau3_wta(*jet)/Tau2_wta(*jet), eventWeight );

    m_numConstituents->Fill( jet->numConstituents(), eventWeight );

  }

  if( m_infoSwitch->m_tracksInJet ){

    const vector<const xAOD::TrackParticle*> matchedTracks = jet->auxdata< vector<const xAOD::TrackParticle*>  >(m_infoSwitch->m_trackName);
    const xAOD::Vertex *pvx  = jet->auxdata<const xAOD::Vertex*>(m_infoSwitch->m_trackName+"_vtx");

    m_nTrk->Fill(matchedTracks.size(), eventWeight);

    for(auto& trkPtr: matchedTracks){
      RETURN_CHECK("JetHists::execute()", m_tracksInJet->execute(trkPtr, jet, pvx, eventWeight), "");
    }
  }

  return StatusCode::SUCCESS;
}

