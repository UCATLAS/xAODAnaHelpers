#include "xAODTracking/TrackParticleContainer.h"
#include "xAODAnaHelpers/TrackPlotsClass.h"
#include <math.h>


using std::cout;  using std::endl;

TrackPlotsClass :: TrackPlotsClass () {
}

TrackPlotsClass :: TrackPlotsClass (std::string name, int detailLevel) : 
  ManageHists(name,detailLevel)
{
}


EL::StatusCode TrackPlotsClass::BookHistograms(EL::Worker* wk) {

  // These plots are always made
  m_trk_Pt        = Book1D(m_name, "pt",          "trk p_{T} [GeV]",  100, 0, 10);
  m_trk_Pt_l      = Book1D(m_name, "pt_l",        "trk p_{T} [GeV]",  100, 0, 100);
  m_trk_Eta       = Book1D(m_name, "eta",         "trk #eta",         80, -4, 4);
  m_trk_Phi       = Book1D(m_name, "phi",         "trk Phi",120, -TMath::Pi(), TMath::Pi() );
  m_trk_dO        = Book1D(m_name, "d0",          "d0[mm]",   100,  -1.0, 1.0 );
  m_trk_z0        = Book1D(m_name, "z0",          "z0[mm]",   100,  -5.0, 5.0 );
  m_trk_chi2Prob  = Book1D(m_name, "chi2Prob",    "chi2Prob", 100,   -0.01,     1.0);
  m_trk_charge    = Book1D(m_name, "charge" ,     "charge",   3,  -1.5,  1.5   );

  //
  //  Level 2 Details
  //
  if(m_detailLevel > 1){
    m_trk_d0_l         = Book1D(m_name, "d0_l" ,            "d0[mm]",           100,  -10.0, 10.0 );
    m_trk_d0Err        = Book1D(m_name, "d0Err",            "d0Err[mm]",        100,  0, 0.4 );
    m_trk_d0Sig        = Book1D(m_name, "d0Sig",            "d0Sig",            240,  -20.0, 40.0 );

    m_trk_z0_l         = Book1D(m_name, "z0_l" ,            "z0[mm]",                         100,  -600.0, 600.0 );
    m_trk_z0Err        = Book1D(m_name, "z0Err",            "z0Err[mm]",                      100,  0, 1.0 );
    m_trk_z0Sig        = Book1D(m_name, "z0Sig",            "z0Sig",                          100,  -25.0, 25.0 );
    m_trk_z0sinT       = Book1D(m_name, "z0sinT",           "z0xsin(#theta)[mm]",             100,  -2.0, 2.0 );
    m_trk_z0SigsinT    = Book1D(m_name, "z0SigsinT",        "z0 significance x sin(#theta)",  100,  -25.0, 25.0 );

    m_trk_chi2Prob_l   = Book1D(m_name, "chi2Prob_l",       "chi2Prob",  100,   -0.01,     1.0);
    m_trk_chi2ndof     = Book1D(m_name, "chi2ndof",         "chi2ndof",  100,    0.0,     8.0 );
    m_trk_chi2ndof_l   = Book1D(m_name, "chi2ndof_l",       "chi2ndof",  100,    0.0,     80.0 );

    m_trk_phiErr       = Book1D(m_name, "phiErr"  ,   "phi Err[rad]",  100,  0, 0.01 );
    m_trk_thetaErr     = Book1D(m_name, "thetaErr",   "theta Err",     100,  0, 0.01 );
    m_trk_qOpErr       = Book1D(m_name, "qOpErr"  ,   "q/p Err",       100,  0, 1.0e-04);

    //m_trk_mc_prob      = Book1D(m_name, "mc_prob",      "mc_prob",     100,  -0.1, 1.1 );
    //m_trk_mc_barcode   = Book1D(m_name, "mc_barcode",   "mc_barcode",  100,  -0.1, 0.5e6 );
    //m_trk_mc_barcode_s = Book1D(m_name, "mc_barcode_s", "mc_barcode",  100,  -0.1, 25e3 );
  }

  //
  //  Level 3 Details
  //
  if(m_detailLevel > 2){
    m_trk_nSi        = Book1D(m_name, "nSi",        "nSi",         30,   -0.5, 29.5 );
    m_trk_nSiAndDead = Book1D(m_name, "nSiAndDead", "nSi(+Dead)",  30,   -0.5, 29.5 );
    m_trk_nSiDead    = Book1D(m_name, "nSiDead",    "nSiDead",     10,   -0.5, 9.5 );
    m_trk_nSCT       = Book1D(m_name, "nSCT",       "nSCTHits",    20,   -0.5, 19.5 );
    m_trk_nPix       = Book1D(m_name, "nPix",       "nPix",        10,   -0.5, 9.5 );
    m_trk_nPixHoles  = Book1D(m_name, "nPixHoles",  "nPixHoles",   10,   -0.5, 9.5 );
    m_trk_nBL        = Book1D(m_name, "nBL",        "nBL",          3,   -0.5,  2.5 );
  }

  //
  //  Level 4 Details
  //
  if(m_detailLevel > 4){
    //new TProfile(m_name, "chi2ndofvseta",  "chi2ndofvseta;    eta;       chi2",  20,  -0.1, 2.7, 0, 4, "s" );
    //new TProfile(m_name, "nhitsvseta",  "nhitsvseta;    eta;       nhits",  20,  -0.1, 2.7, 0, 15 , "s" );
    //new TProfile(m_name, "chi2ndofvspt",  "chi2ndofvseta;    pt;       chi2",  50,  -0.1, 20, 0 , 4, "s" );

    //  new TProfile(m_name, "d0Errvseta",      "d0Errvseta;    eta;       d0Err",  20,  -0.1, 2.7, 0, 0.4, "s" );
    //  new TProfile(m_name, "d0ErrvsIpt",      "d0ErrvsIpt;    Ipt;       d0Err",  20,  -6e-4, 6e4, 0, 0.4, "s" );
    //  new TProfile(m_name, "d0Errvsphi",      "d0Errvsphi;    phi;       d0Err",  1000,  -3.2, 3.2, 0, 0.4);
    //
    
    //  new TH2F(m_name, "z0d0"    ,    "z0d0;     z0[mm];  d0[mm];",  50,  -300.0, 300.0, 50, -4.0, 4.0  );
    //  new TH2F(m_name, "z0sinTd0",    "z0sinTd0;     z0xsin0[mm](signed);  d0[mm](signed);",  50,-2.0, 2.0, 50, -2.0, 2.0  );

    //  new TH2F(m_name, "d0vsPt"    ,    "d0vsPt;     d0[mm] (signed);  Pt[GeV];",  100,  -2,2.0, 50, 0, 10  );
    //  new TH2F(m_name, "d0SigvsPt"    ,    "d0SigvsPt;     d0Sig(signed);  Pt[GeV];",  240, -20, 40.0, 50, 0, 10  );
  }

  //
  // Level 10 
  //
  if(m_detailLevel > 9){
    m_trk_eta_vl      = Book1D(m_name, "eta_vl",        "eta",       100,  -6,    6     );
    m_trk_z0_vl       = Book1D(m_name, "z0_vl",         "z0[mm]",    100,  -10000.0, 10000.0 );
    m_trk_d0_vl       = Book1D(m_name, "d0_vl",         "d0[mm]",    100,  -10000.0, 10000.0 );
    m_trk_pt_ss       = Book1D(m_name, "pt_ss",         "Pt[GeV",    100,  0,     2.0  );
    m_trk_phiManyBins = Book1D(m_name, "phiManyBins" ,  "phi",      1000,  -3.2,  3.2   );
  }

  // if worker is passed to the class add histograms to the output
  if(wk) {
    AddToOutput(wk);
  }
  return EL::StatusCode::SUCCESS;
}

void TrackPlotsClass::FillHistograms( const xAOD::TrackParticleContainer* trks, float eventWeight ) {
  xAOD::TrackParticleContainer::const_iterator trk_itr = trks->begin();
  xAOD::TrackParticleContainer::const_iterator trk_end = trks->end();
  for( ; trk_itr != trk_end; ++trk_itr ) {
    this->FillHistograms( (*trk_itr), eventWeight );
  }
}

void TrackPlotsClass::FillHistograms( const xAOD::TrackParticle* trk, float eventWeight ) { 

  //basic
  float        trkPt       = trk->pt()/1e3;
  float        chi2        = trk->chiSquared();
  float        ndof        = trk->numberDoF();
  float        chi2Prob    = TMath::Prob(chi2,ndof);
  float        d0          = trk->d0();
  float        z0          = trk->z0();

  m_trk_Pt       -> Fill( trkPt,            eventWeight );
  m_trk_Pt_l     -> Fill( trkPt,            eventWeight );
  m_trk_Eta      -> Fill( trk->eta(),       eventWeight );
  m_trk_Phi      -> Fill( trk->phi(),       eventWeight );
  m_trk_dO       -> Fill( d0,               eventWeight );
  m_trk_z0       -> Fill( z0,               eventWeight );
  m_trk_chi2Prob -> Fill( chi2Prob ,        eventWeight );
  m_trk_charge   -> Fill( trk->charge() ,   eventWeight );

  if(m_detailLevel > 1){
    float d0Err = sqrt((trk->definingParametersCovMatrixVec().at(0)));
    float d0Sig = (d0Err > 0) ? d0/d0Err : -1 ;
    m_trk_d0_l         -> Fill(d0    , eventWeight );
    m_trk_d0Err        -> Fill(d0Err , eventWeight );
    m_trk_d0Sig        -> Fill(d0Sig , eventWeight );

    float z0Err = sqrt((trk->definingParametersCovMatrixVec().at(1)));
    float z0Sig = (z0Err > 0) ? z0/z0Err : -1 ;
    float sinT  = sin(trk->theta());

    m_trk_z0_l         -> Fill(z0         , eventWeight );
    m_trk_z0Err        -> Fill(z0Err      , eventWeight );
    m_trk_z0Sig        -> Fill(z0Sig      , eventWeight );
    m_trk_z0sinT       -> Fill(z0*sinT    , eventWeight );
    m_trk_z0SigsinT    -> Fill(z0Sig*sinT , eventWeight );

    float chi2NDoF     = (ndof > 0) ? chi2/ndof : -1;
    m_trk_chi2Prob_l   -> Fill(chi2Prob   , eventWeight );
    m_trk_chi2ndof     -> Fill(chi2NDoF   , eventWeight );
    m_trk_chi2ndof_l   -> Fill(chi2NDoF   , eventWeight );

    m_trk_phiErr       -> Fill( sqrt((trk->definingParametersCovMatrixVec().at(2))) , eventWeight );
    m_trk_thetaErr     -> Fill( sqrt((trk->definingParametersCovMatrixVec().at(3))) , eventWeight );
    m_trk_qOpErr       -> Fill( sqrt((trk->definingParametersCovMatrixVec().at(4))) , eventWeight );
  }

  if(m_detailLevel > 2){

    uint8_t nBL       = -1;  
    uint8_t nPix      = -1;  
    uint8_t nPixDead  = -1;  
    uint8_t nPixHoles = -1;  
    uint8_t nSCT      = -1;  
    uint8_t nSCTDead  = -1;  

    if(!trk->summaryValue(nBL,       xAOD::numberOfBLayerHits))       cout << "ERROR: BLayer hits not filled" << endl;
    if(!trk->summaryValue(nPix,      xAOD::numberOfPixelHits))        cout << "ERROR: Pix hits not filled" << endl;
    if(!trk->summaryValue(nPixDead,  xAOD::numberOfPixelDeadSensors)) cout << "ERROR: Pix Dead not filled" << endl;
    if(!trk->summaryValue(nPixHoles, xAOD::numberOfPixelHoles))       cout << "ERROR: Pix holes not filled" << endl;
    if(!trk->summaryValue(nSCT,      xAOD::numberOfSCTHits))          cout << "ERROR: SCT hits not filled" << endl;
    if(!trk->summaryValue(nSCTDead,  xAOD::numberOfSCTDeadSensors))   cout << "ERROR: SCT Dead not filled" << endl;
      
    uint8_t nSi     = nPix     + nSCT;
    uint8_t nSiDead = nPixDead + nSCTDead;
    m_trk_nBL        -> Fill( nBL         , eventWeight );
    m_trk_nSi        -> Fill( nSi         , eventWeight ); 
    m_trk_nSiAndDead -> Fill( nSi+nSiDead , eventWeight );
    m_trk_nSiDead    -> Fill( nSiDead     , eventWeight );
    m_trk_nSCT       -> Fill( nSCT        , eventWeight );
    m_trk_nPix       -> Fill( nPix        , eventWeight );
    m_trk_nPixHoles  -> Fill( nPixHoles   , eventWeight );

  }


  if(m_detailLevel > 9){
    m_trk_eta_vl      -> Fill( trk->eta(), eventWeight );
    m_trk_z0_vl       -> Fill( z0,         eventWeight );
    m_trk_d0_vl       -> Fill( d0,         eventWeight );
    m_trk_pt_ss       -> Fill( trkPt,      eventWeight );
    m_trk_phiManyBins -> Fill( trk->phi(), eventWeight );
  }



}
