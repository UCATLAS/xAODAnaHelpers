#include "xAODAnaHelpers/EventInfo.h"
#include <xAODAnaHelpers/HelperFunctions.h>
#include <iostream>
#include "xAODTruth/TruthEventContainer.h"
#include "xAODEventShape/EventShape.h"
#include "xAODCaloEvent/CaloClusterContainer.h"


using namespace xAH;
using std::vector;  using std::endl;  using std::cout;

EventInfo::EventInfo(const std::string& detailStr, float units, bool mc)
  : m_infoSwitch(detailStr), m_mc(mc), m_debug(false), m_units(units)
{
}

EventInfo::~EventInfo()
{
  if(m_debug) cout << " Deleting EventInfo "  << endl;  
}

void EventInfo::setTree(TTree *tree)
{

  connectBranch<int>     (tree, "runNumber",   &m_runNumber);
  connectBranch<Long64_t>(tree, "eventNumber", &m_eventNumber);
  connectBranch<int>     (tree, "lumiBlock",   &m_lumiBlock);
  connectBranch<uint32_t>(tree, "coreFlags",   &m_coreFlags);           

  if(m_mc){
    connectBranch<int     >(tree, "mcEventNumber",              &m_mcEventNumber);       
    connectBranch<int     >(tree, "mcChannelNumber",            &m_mcChannelNumber);    
    connectBranch<float   >(tree, "mcEventWeight",              &m_mcEventWeight);       
  } else {
    connectBranch<int     >(tree, "bcid",                       &m_bcid);                 
    connectBranch<float   >(tree, "prescale_DataWeight",               &m_prescale_DataWeight);       
  }

  if ( m_infoSwitch.m_eventCleaning ) {
    connectBranch<uint32_t>(tree, "timeStamp",                  &m_timeStamp);           
    connectBranch<uint32_t>(tree, "timeStampNSOffset",          &m_timeStampNSOffset);  
    connectBranch<bool    >(tree, "TileError",                  &m_TileError);           
    connectBranch<bool    >(tree, "SCTError",                   &m_SCTError);            
    connectBranch<bool    >(tree, "LArError",                   &m_LArError);            
    connectBranch<uint32_t>(tree, "TileFlags",                  &m_TileFlags);           
    connectBranch<uint32_t>(tree, "SCTFlags",                   &m_SCTFlags);            
    connectBranch<uint32_t>(tree, "LArFlags",                   &m_LArFlags);            
  }

  if ( m_infoSwitch.m_pileup ) {
    connectBranch<int  >(tree, "NPV",                              &m_npv);
    connectBranch<float>(tree, "actualInteractionsPerCrossing",    &m_actualMu);
    connectBranch<float>(tree, "averageInteractionsPerCrossing",   &m_averageMu);
    connectBranch<float>(tree, "weight_pileup",                    &m_weight_pileup);       

    if(m_mc){
      connectBranch<float>(tree, "correct_mu",                 &m_correct_mu);          
      connectBranch<int  >(tree, "rand_run_nr",                &m_rand_run_nr);         
      connectBranch<int  >(tree, "rand_lumiblock_nr",          &m_rand_lumiblock_nr);  
    }
  }

  if ( m_infoSwitch.m_shapeEM ) {
    connectBranch<double>(tree, "rhoEM",   &m_rhoEM);
  }

  if ( m_infoSwitch.m_shapeLC ) {
    connectBranch<double>(tree, "rhoLC",   &m_rhoLC);
  }

    // truth
  if( m_infoSwitch.m_truth && m_mc ) {
    connectBranch<int  >(tree, "pdgId1",  &m_pdgId1);      
    connectBranch<int  >(tree, "pdgId2",  &m_pdgId2);      
    connectBranch<int  >(tree, "pdfId1",  &m_pdfId1);      
    connectBranch<int  >(tree, "pdfId2",  &m_pdfId2);      
    connectBranch<float>(tree, "x1",      &m_x1);  
    connectBranch<float>(tree, "x2",      &m_x2);  
    //connectBranch<float>(tree, "scale",   &m_scale);       
    //connectBranch<float>(tree, "q",       &m_q);   
    //connectBranch<float>(tree, "pdf1",    &m_pdf1);        
    //connectBranch<float>(tree, "pdf2",    &m_pdf2);        
    connectBranch<float>(tree, "xf1",     &m_xf1); 
    connectBranch<float>(tree, "xf2",     &m_xf2);    
  }
  
  // CaloCluster
  if ( m_infoSwitch.m_caloClus ) {
    std::vector<float>*  m_caloCluster_pt_addr = &m_caloCluster_pt;
    HelperFunctions::connectBranch<float>("caloCluster", tree, "pt",  &m_caloCluster_pt_addr  );

    std::vector<float>*  m_caloCluster_eta_addr = &m_caloCluster_eta;
    HelperFunctions::connectBranch<float>("caloCluster", tree, "eta", &m_caloCluster_eta_addr );

    std::vector<float>*  m_caloCluster_phi_addr = &m_caloCluster_phi;
    HelperFunctions::connectBranch<float>("caloCluster", tree, "phi", &m_caloCluster_phi_addr );

    std::vector<float>*  m_caloCluster_e_addr = &m_caloCluster_e;
    HelperFunctions::connectBranch<float>("caloCluster", tree, "e",   &m_caloCluster_e_addr   );
  }
}


void EventInfo::setBranches(TTree *tree)
{

  // always
  tree->Branch("runNumber",          &m_runNumber,      "runNumber/I");
  tree->Branch("eventNumber",        &m_eventNumber,    "eventNumber/LI");
  tree->Branch("lumiBlock",          &m_lumiBlock,      "lumiBlock/I");
  tree->Branch("coreFlags",          &m_coreFlags,      "coreFlags/i");
  if( m_mc ) {
    tree->Branch("mcEventNumber",      &m_mcEventNumber,  "mcEventNumber/I");
    tree->Branch("mcChannelNumber",    &m_mcChannelNumber,"mcChannelNumber/I");
    tree->Branch("mcEventWeight",      &m_mcEventWeight,  "mcEventWeight/F");
  } else {
    tree->Branch("bcid",               &m_bcid,           "bcid/I");
    tree->Branch("prescale_DataWeight",       &m_prescale_DataWeight,  "prescale_DataWeight/F");
  }

  if ( m_infoSwitch.m_eventCleaning ) {
    tree->Branch("timeStamp",          &m_timeStamp,         "timeStamp/i");
    tree->Branch("timeStampNSOffset",  &m_timeStampNSOffset, "timeStampNSOffset/i");
    tree->Branch("TileError",          &m_TileError,         "TileError/O");
    tree->Branch("SCTError",           &m_SCTError,          "SCTError/O");
    tree->Branch("LArError",           &m_LArError,          "LArError/O");
    tree->Branch("TileFlags",          &m_TileFlags,         "TileFlags/i");
    tree->Branch("SCTFlags",           &m_SCTFlags,          "SCTFlags/i");
    tree->Branch("LArFlags",           &m_LArFlags,          "LArFlags/i");
  }

  if ( m_infoSwitch.m_pileup ) {
    tree->Branch("NPV",                &m_npv,            "NPV/I");
    tree->Branch("actualInteractionsPerCrossing",  &m_actualMu,  "actualInteractionsPerCrossing/F");
    tree->Branch("averageInteractionsPerCrossing", &m_averageMu, "averageInteractionsPerCrossing/F");
    tree->Branch("weight_pileup",      &m_weight_pileup,  "weight_pileup/F");
    if(m_mc){
      tree->Branch("correct_mu"       ,          &m_correct_mu       ,"correct_mu/F"       );          
      tree->Branch("rand_run_nr"      ,          &m_rand_run_nr      ,"rand_run_nr/I"      );         
      tree->Branch("rand_lumiblock_nr",          &m_rand_lumiblock_nr,"rand_lumiblock_nr/I");  
    }
  }

  if ( m_infoSwitch.m_shapeEM ) {
    tree->Branch("rhoEM",                &m_rhoEM,            "rhoEM/D");
  }

  if ( m_infoSwitch.m_shapeLC ) {
    tree->Branch("rhoLC",                &m_rhoLC,            "rhoLC/D");
  }

  if( m_infoSwitch.m_truth && m_mc ) {
    tree->Branch("pdgId1",            &m_pdgId1,        "pdgId1/I" );
    tree->Branch("pdgId2",            &m_pdgId2,        "pdgId2/I" );
    tree->Branch("pdfId1",            &m_pdfId1,        "pdfId1/I" );
    tree->Branch("pdfId2",            &m_pdfId2,        "pdfId2/I" );
    tree->Branch("x1",                &m_x1,            "x1/F"  );
    tree->Branch("x2",                &m_x2,            "x2/F"  );
    //tree->Branch("scale",             &m_scale,         "scale/F");
    //tree->Branch("q",                 &m_q,             "q/F");
    //tree->Branch("pdf1",              &m_pdf1,          "pdf1/F");
    //tree->Branch("pdf2",              &m_pdf2,          "pdf2/F");
    tree->Branch("xf1",               &m_xf1,           "xf1/F");
    tree->Branch("xf2",               &m_xf2,           "xf2/F");
  }

  if ( m_infoSwitch.m_caloClus ) {
    tree->Branch("caloCluster_pt",  &m_caloCluster_pt);
    tree->Branch("caloCluster_phi", &m_caloCluster_phi);
    tree->Branch("caloCluster_eta", &m_caloCluster_eta);
    tree->Branch("caloCluster_e",   &m_caloCluster_e);
  }

  return;
}
    

void EventInfo::clear()
{
  m_runNumber = m_eventNumber = m_mcEventNumber = m_mcChannelNumber = m_bcid = m_lumiBlock;
  m_coreFlags = 0;
  //eventCleaning
  m_LArError = false;
  m_TileError = false;
  m_SCTError = false;
  m_LArFlags = 0;
  m_TileFlags = 0;
  m_SCTFlags = 0;
  m_mcEventWeight = 1.;
  m_prescale_DataWeight = 1.;
  m_weight_pileup = 1.;
  m_timeStamp = -999;
  m_timeStampNSOffset = -999;
  // pileup
  m_npv = -999;
  m_actualMu = m_averageMu = -999;
  // shapeEM
  m_rhoEM = -999;
  // shapeLC
  m_rhoLC = -999;
  // truth
  m_pdgId1 = m_pdgId2 = m_pdfId1 = m_pdfId2 = -999;
  m_x1 = m_x2 = -999;
  m_xf1 = m_xf2 = -999;

  //m_scale = m_q = m_pdf1 = m_pdf2 = -999;

  // CaloCluster
  if( m_infoSwitch.m_caloClus){
    m_caloCluster_pt.clear();
    m_caloCluster_eta.clear();
    m_caloCluster_phi.clear();
    m_caloCluster_e.clear();
  }

  return;
}

void EventInfo::FillEvent( const xAOD::EventInfo* eventInfo,  xAOD::TEvent* event) {
  
  m_runNumber             = eventInfo->runNumber();
  m_eventNumber           = eventInfo->eventNumber();
  m_lumiBlock             = eventInfo->lumiBlock();
  m_coreFlags             = eventInfo->eventFlags(xAOD::EventInfo::Core);
  if ( m_mc ) {
    m_mcEventNumber         = eventInfo->mcEventNumber();
    m_mcChannelNumber       = eventInfo->mcChannelNumber();
    m_mcEventWeight         = eventInfo->mcEventWeight();
  } else {
    m_bcid                  = eventInfo->bcid();
  }

  if ( m_infoSwitch.m_eventCleaning ) {

    if ( eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) m_LArError = true;
    else m_LArError = false;
    m_LArFlags = eventInfo->eventFlags(xAOD::EventInfo::LAr);

    if ( eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) m_TileError = true;
    else m_TileError = false;
    m_TileFlags = eventInfo->eventFlags(xAOD::EventInfo::Tile);

    if ( eventInfo->errorState(xAOD::EventInfo::SCT)==xAOD::EventInfo::Error ) m_SCTError = true;
    else m_SCTError = false;
    m_SCTFlags = eventInfo->eventFlags(xAOD::EventInfo::SCT);

    m_timeStamp = eventInfo->timeStamp();
    m_timeStampNSOffset = eventInfo->timeStampNSOffset();

  }

  if ( m_infoSwitch.m_pileup ) {

    if ( event ) {
      const xAOD::VertexContainer* vertices(nullptr);
      HelperFunctions::retrieve( vertices, "PrimaryVertices", event, 0 );
      m_npv = HelperFunctions::countPrimaryVertices(vertices, 2);
    } else {
      m_npv = -1;
    }

    m_actualMu  = eventInfo->actualInteractionsPerCrossing();
    m_averageMu = eventInfo->averageInteractionsPerCrossing();

    if ( m_mc ) {

      static SG::AuxElement::ConstAccessor< float > weight_pileup ("PileupWeight");
      static SG::AuxElement::ConstAccessor< float >  correct_mu("corrected_averageInteractionsPerCrossing");
      static SG::AuxElement::ConstAccessor< unsigned int > rand_run_nr("RandomRunNumber");
      static SG::AuxElement::ConstAccessor< unsigned int > rand_lumiblock_nr("RandomLumiBlockNumber");

      if ( weight_pileup.isAvailable( *eventInfo ) )	 { m_weight_pileup = weight_pileup( *eventInfo ); }	    else { m_weight_pileup = 1.0; }
      if ( correct_mu.isAvailable( *eventInfo ) )	 { m_correct_mu = correct_mu( *eventInfo ); }		    else { m_correct_mu = -1.0; }
      if ( rand_run_nr.isAvailable( *eventInfo ) )	 { m_rand_run_nr = rand_run_nr( *eventInfo ); } 	    else { m_rand_run_nr = 900000; }
      if ( rand_lumiblock_nr.isAvailable( *eventInfo ) ) { m_rand_lumiblock_nr = rand_lumiblock_nr( *eventInfo ); } else { m_rand_lumiblock_nr = 0; }

    }

    if ( !m_mc ) {

      static SG::AuxElement::ConstAccessor< float > prsc_DataWeight ("prescale_DataWeight");

      if ( prsc_DataWeight.isAvailable( *eventInfo ) )	 { m_prescale_DataWeight = prsc_DataWeight( *eventInfo ); }	    else { m_prescale_DataWeight = 1.0; }

    }

  }

  if ( m_infoSwitch.m_shapeLC && event ) {
    const xAOD::EventShape* evtShape(nullptr);
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoLC ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoLC = -999;
    }
  }

  if ( m_infoSwitch.m_shapeEM && event ) {
    const xAOD::EventShape* evtShape(nullptr);
    HelperFunctions::retrieve( evtShape, "Kt4EMTopoEventShape", event, 0 );
    if ( !evtShape->getDensity( xAOD::EventShape::Density, m_rhoEM ) ) {
      Info("FillEvent()","Could not retrieve xAOD::EventShape::Density from xAOD::EventShape");
      m_rhoEM = -999;
    }
  }

  if( m_infoSwitch.m_caloClus && event ) {
    const xAOD::CaloClusterContainer* caloClusters = 0;
    HelperFunctions::retrieve( caloClusters, "CaloCalTopoClusters", event, 0);
    // save the clusters at the EM scale
    for( auto clus : * caloClusters ) {
      if ( clus->pt ( xAOD::CaloCluster::State::UNCALIBRATED ) < 2000 ) { continue; } // 2 GeV cut
      m_caloCluster_pt. push_back( clus->pt ( xAOD::CaloCluster::State::UNCALIBRATED ) / m_units );
      m_caloCluster_eta.push_back( clus->eta( xAOD::CaloCluster::State::UNCALIBRATED ) );
      m_caloCluster_phi.push_back( clus->phi( xAOD::CaloCluster::State::UNCALIBRATED ) );
      m_caloCluster_e.  push_back( clus->e  ( xAOD::CaloCluster::State::UNCALIBRATED ) / m_units );
    }
  }

  if( m_infoSwitch.m_truth && event && m_mc ) {
    //MC Truth
    const xAOD::TruthEventContainer* truthE = 0;
    HelperFunctions::retrieve( truthE, "TruthEvents", event, 0 );
    if( truthE ) {
      const xAOD::TruthEvent* truthEvent = truthE->at(0);
      truthEvent->pdfInfoParameter(m_pdgId1,   xAOD::TruthEvent::PDGID1);
      truthEvent->pdfInfoParameter(m_pdgId2,   xAOD::TruthEvent::PDGID2);
      truthEvent->pdfInfoParameter(m_pdfId1,   xAOD::TruthEvent::PDFID1);
      truthEvent->pdfInfoParameter(m_pdfId2,   xAOD::TruthEvent::PDFID2);
      truthEvent->pdfInfoParameter(m_x1,       xAOD::TruthEvent::X1);
      truthEvent->pdfInfoParameter(m_x2,       xAOD::TruthEvent::X2);
      //truthEvent->pdfInfoParameter(m_scale,    xAOD::TruthEvent::SCALE);
      //truthEvent->pdfInfoParameter(m_q,        xAOD::TruthEvent::Q);
      //truthEvent->pdfInfoParameter(m_pdf1,     xAOD::TruthEvent::PDF1);
      //truthEvent->pdfInfoParameter(m_pdf2,     xAOD::TruthEvent::PDF2);
      truthEvent->pdfInfoParameter(m_xf1,      xAOD::TruthEvent::XF1);
      truthEvent->pdfInfoParameter(m_xf2,      xAOD::TruthEvent::XF2);

//      // crashes because of q?`
//        const xAOD::TruthEvent::PdfInfo info = truthEvent->pdfInfo();
//        if( info.valid() ) {
//          m_pdgId1 = info.pdgId1;
//          m_pdgId2 = info.pdgId2;
//          m_pdfId1 = info.pdfId1;
//          m_pdfId2 = info.pdfId2;
//          m_x1     = info.x1;
//          m_x2     = info.x2;
//          //m_q      = info.Q;
//          m_xf1    = info.xf1;
//          m_xf2    = info.xf2;
//        }

    }

  }


  return;
}




