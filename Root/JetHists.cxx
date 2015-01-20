#include <xAODAnaHelpers/JetHists.h>

JetHists :: JetHists (std::string name, std::string detailStr, std::string delimiter) :
  HistogramManager(name, detailStr, delimiter)
{
}

JetHists :: ~JetHists () {}

EL::StatusCode JetHists::initialize() {

  // These plots are always made
  m_jetPt          = book(m_name, "jetPt",  "jet p_{T} [GeV]", 120, 0, 600);
  m_jetEta         = book(m_name, "jetEta", "jet #eta",         80, -4, 4);
  m_jetPhi         = book(m_name, "jetPhi", "jet Phi",120, -TMath::Pi(), TMath::Pi() );
  m_jetM           = book(m_name, "jetMass", "jet Mass [GeV]",120, 0, 400);
  m_jetE           = book(m_name, "jetEnergy", "jet Energy [GeV]",120, 0, 4000);
  m_jetRapidity    = book(m_name, "jetRapidity", "jet Rapidity",120, -10, 10);

  // details of the jet kinematics
  m_fillKinematic = false;
  if( m_detailStr.find("kinematic") != std::string::npos ) { 
    m_fillKinematic = true;
    m_jetPx     = book(m_name, "jetPx",     "jet Px [GeV]",     120, 0, 1000);
    m_jetPy     = book(m_name, "jetPy",     "jet Py [GeV]",     120, 0, 1000);
    m_jetPz     = book(m_name, "jetPz",     "jet Pz [GeV]",     120, 0, 4000);
  }

  // details for jet cleaning
  m_fillClean = false;
  if( m_detailStr.find("clean") != std::string::npos ) { 
    m_fillClean = true;
    // units?
    m_jetTime     = book(m_name, "JetTimming" ,   "Jet Timming",      120, -80, 80);
    m_LArQuality  = book(m_name, "LArQuality" ,   "LAr Quality",      120, -600, 600);
    m_hecq        = book(m_name, "HECQuality" ,   "HEC Quality",      120, -10, 10);
    m_negE        = book(m_name, "NegativeE" ,    "Negative Energy",  120, -10, 10);
    m_avLArQF     = book(m_name, "AverageLArQF" , "<LAr Quality Factor>" , 120, 0, 1000);
    m_bchCorrCell = book(m_name, "BchCorrCell" ,  "BCH Corr Cell" ,   120, 0, 600);
    m_N90Const  = book(m_name, "N90Constituents", "N90 Constituents" ,  120, 0, 40);
  }

  // details for jet energy information
  m_fillEnergy = false;
  if( m_detailStr.find("energy") != std::string::npos ) { 
    m_fillEnergy = true;
    m_HECf      = book(m_name, "HECFrac",         "HEC Fraction" ,    120, 0, 5);
    m_EMf       = book(m_name, "EMFrac",          "EM Fraction" ,     120, 0, 2);
    m_actArea   = book(m_name, "ActiveArea",      "Jet Active Area" , 120, 0, 1);
    m_centroidR = book(m_name, "CentroidR",       "CentroidR" ,       120, 0, 600); 
  }

  m_chf         = book(m_name, "chfPV" ,    "PV(chf)" ,     120, 0, 600);

  // details for jet resolutions
  m_fillResolution = false;
  if( m_detailStr.find("resolution") != std::string::npos ) { 
    m_fillResolution = true;
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

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHists::execute( const xAOD::JetContainer* jets, float eventWeight ) {
  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();
  for( ; jet_itr != jet_end; ++jet_itr ) {
    this->execute( (*jet_itr), eventWeight );
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHists::execute( const xAOD::Jet* jet, float eventWeight ) { 

  //basic
  m_jetPt ->        Fill( jet->pt()/1e3,    eventWeight );
  m_jetEta->        Fill( jet->eta(),       eventWeight );
  m_jetPhi->        Fill( jet->phi(),       eventWeight );
  m_jetM->          Fill( jet->m()/1e3,     eventWeight );
  m_jetE->          Fill( jet->e()/1e3,     eventWeight );
  m_jetRapidity->   Fill( jet->rapidity(),  eventWeight );

  // kinematic
  if( m_fillKinematic ) {
    m_jetPx->  Fill( jet->px()/1e3,  eventWeight );
    m_jetPy->  Fill( jet->py()/1e3,  eventWeight );
    m_jetPz->  Fill( jet->pz()/1e3,  eventWeight );
  } // fillKinematic

  // clean 
  if( m_fillClean) {

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
  if( m_fillEnergy) {

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

  /*
  // area
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
 
 


  std::vector<float> chfs = jet->getAttribute< std::vector<float> >(xAOD::JetAttribute::SumPtTrkPt1000);
  float chf(-1);
  //if( pvLocation >= 0 && pvLocation < (int)chfs.size() ) { chf = chfs.at( pvLocation ); }
  m_chf ->  Fill( chf , eventWeight );


  // testing 
  if( m_fillResolution ) {
    //float ghostTruthPt = jet->getAttribute( xAOD::JetAttribute::GhostTruthPt );
    float ghostTruthPt = jet->auxdata< float >( "GhostTruthPt" );
    m_jetGhostTruthPt -> Fill( ghostTruthPt/1e3, eventWeight );
    float resolution = jet->pt()/ghostTruthPt - 1;
    m_jetPt_vs_resolution -> Fill( jet->pt()/1e3, resolution, eventWeight );
    m_jetGhostTruthPt_vs_resolution -> Fill( ghostTruthPt/1e3, resolution, eventWeight );
  }

  return EL::StatusCode::SUCCESS;
}

