#include "xAODAnaHelpers/JetContainer.h"

#include <iostream>

using namespace xAH;

JetContainer::JetContainer(const std::string& name, const std::string& detailStr)
  : ParticleContainer(name,detailStr)
{
  // clean
  m_Timing                    =new std::vector<float>();
  m_LArQuality                =new std::vector<float>();
  m_HECQuality                =new std::vector<float>();
  m_NegativeE                 =new std::vector<float>();
  m_AverageLArQF              =new std::vector<float>();
  m_BchCorrCell               =new std::vector<float>();
  m_N90Constituents           =new std::vector<float>();
  m_LArBadHVEFrac             =new std::vector<float>();
  m_LArBadHVNCell             =new std::vector<int>();
  m_OotFracClusters5          =new std::vector<float>();
  m_OotFracClusters10         =new std::vector<float>();
  m_LeadingClusterPt          =new std::vector<float>();
  m_LeadingClusterSecondLambda=new std::vector<float>();
  m_LeadingClusterCenterLambda=new std::vector<float>();
  m_LeadingClusterSecondR     =new std::vector<float>();
  m_clean_passLooseBad        =new std::vector<int>();
  m_clean_passLooseBadUgly    =new std::vector<int>();
  m_clean_passTightBad        =new std::vector<int>();
  m_clean_passTightBadUgly    =new std::vector<int>();

  // energy
  m_HECFrac               = new std::vector<float>();
  m_EMFrac                = new std::vector<float>();
  m_CentroidR             = new std::vector<float>();
  m_FracSamplingMax       = new std::vector<float>();
  m_FracSamplingMaxIndex  = new std::vector<float>();
  m_LowEtConstituentsFrac = new std::vector<float>();
  m_GhostMuonSegmentCount = new std::vector<float>();
  m_Width                 = new std::vector<float>();

  // trackPV
  m_NumTrkPt1000PV       =new std::vector<float>();
  m_SumPtTrkPt1000PV     =new std::vector<float>();
  m_TrackWidthPt1000PV   =new std::vector<float>();
  m_NumTrkPt500PV        =new std::vector<float>();
  m_SumPtTrkPt500PV      =new std::vector<float>();
  m_TrackWidthPt500PV    =new std::vector<float>();
  m_JVFPV                =new std::vector<float>();

  // trackAll or trackPV
  m_Jvt       =new std::vector<float>;
  m_JvtJvfcorr=new std::vector<float>;
  m_JvtRpt    =new std::vector<float>;

  // flavTag
  m_SV0                       =new std::vector<float>();
  m_SV1                       =new std::vector<float>();
  m_IP3D                      =new std::vector<float>();
  m_SV1IP3D                   =new std::vector<float>();
  //m_MV1                       =new std::vector<float>();
  m_MV2c00                    =new std::vector<float>();
  m_MV2c10                    =new std::vector<float>();
  m_MV2c20                    =new std::vector<float>();
  m_MV2                       =new std::vector<float>();
  m_HadronConeExclTruthLabelID=new std::vector<int>();

  m_MV2c20_isFix30       =new std::vector<int>  ();
  m_MV2c20_sfFix30       =new std::vector< std::vector<float> >();

  m_MV2c20_isFix50       =new std::vector<int>  ();
  m_MV2c20_sfFix50       =new std::vector< std::vector<float> >();

  m_MV2c20_isFix60       =new std::vector<int>  ();
  m_MV2c20_sfFix60       =new std::vector< std::vector<float> >();

  m_MV2c20_isFix70       =new std::vector<int>  ();
  m_MV2c20_sfFix70       =new std::vector< std::vector<float> >();

  m_MV2c20_isFix77       =new std::vector<int>  ();
  m_MV2c20_sfFix77       =new std::vector< std::vector<float> >();

  m_MV2c20_isFix80       =new std::vector<int>  ();
  m_MV2c20_sfFix80       =new std::vector< std::vector<float> >();

  m_MV2c20_isFix85       =new std::vector<int>  ();
  m_MV2c20_sfFix85       =new std::vector< std::vector<float> >();

  m_MV2c20_isFix90       =new std::vector<int>  ();
  m_MV2c20_sfFix90       =new std::vector< std::vector<float> >();

  m_MV2c20_isFlt30       =new std::vector<int>  ();
  m_MV2c20_sfFlt30       =new std::vector< std::vector<float> >();

  m_MV2c20_isFlt40       =new std::vector<int>  ();
  m_MV2c20_sfFlt40       =new std::vector< std::vector<float> >();

  m_MV2c20_isFlt50       =new std::vector<int>  ();
  m_MV2c20_sfFlt50       =new std::vector< std::vector<float> >();

  m_MV2c20_isFlt60       =new std::vector<int>  ();
  m_MV2c20_sfFlt60       =new std::vector< std::vector<float> >();

  m_MV2c20_isFlt70       =new std::vector<int>  ();
  m_MV2c20_sfFlt70       =new std::vector< std::vector<float> >();

  m_MV2c20_isFlt77       =new std::vector<int>  ();
  m_MV2c20_sfFlt77       =new std::vector< std::vector<float> >();

  m_MV2c20_isFlt85       =new std::vector<int>  ();
  m_MV2c20_sfFlt85       =new std::vector< std::vector<float> >();

  // truth
  m_ConeTruthLabelID  =new std::vector<int>();
  m_TruthCount        =new std::vector<int>();
  m_TruthLabelDeltaR_B=new std::vector<float>;
  m_TruthLabelDeltaR_C=new std::vector<float>;
  m_TruthLabelDeltaR_T=new std::vector<float>;
  m_PartonTruthLabelID=new std::vector<int>();
  m_GhostTruthAssociationFraction=new std::vector<float>;
  m_truth_E  =new std::vector<float>;
  m_truth_pt =new std::vector<float>;
  m_truth_phi=new std::vector<float>;
  m_truth_eta=new std::vector<float>;

  // charge
  m_charge   =new std::vector<double>();
}

JetContainer::~JetContainer()
{
  // clean
  delete m_Timing;
  delete m_LArQuality;
  delete m_HECQuality;
  delete m_NegativeE;
  delete m_AverageLArQF;
  delete m_BchCorrCell;
  delete m_N90Constituents;
  delete m_LArBadHVEFrac;
  delete m_LArBadHVNCell;
  delete m_OotFracClusters5;
  delete m_OotFracClusters10;
  delete m_LeadingClusterPt;
  delete m_LeadingClusterSecondLambda;
  delete m_LeadingClusterCenterLambda;
  delete m_LeadingClusterSecondR;
  delete m_clean_passLooseBad;
  delete m_clean_passLooseBadUgly;
  delete m_clean_passTightBad;
  delete m_clean_passTightBadUgly;

  // energy
  delete m_HECFrac;
  delete m_EMFrac;
  delete m_CentroidR;
  delete m_FracSamplingMax;
  delete m_FracSamplingMaxIndex;
  delete m_LowEtConstituentsFrac;
  delete m_GhostMuonSegmentCount;
  delete m_Width;

  // trackPV
  delete m_NumTrkPt1000PV;
  delete m_SumPtTrkPt1000PV;
  delete m_TrackWidthPt1000PV;
  delete m_NumTrkPt500PV;
  delete m_SumPtTrkPt500PV;
  delete m_TrackWidthPt500PV;
  delete m_JVFPV;

  // trackAll or trackPV
  delete m_Jvt;
  delete m_JvtJvfcorr;
  delete m_JvtRpt;

  // flavTag
  delete m_SV0;
  delete m_SV1;
  delete m_IP3D;
  delete m_SV1IP3D;
  //delete m_MV1;
  delete m_MV2;
  delete m_MV2c00;
  delete m_MV2c10;
  delete m_MV2c20;

  delete m_HadronConeExclTruthLabelID;

  delete m_MV2c20_isFix30;
  delete m_MV2c20_sfFix30;

  delete m_MV2c20_isFix50;
  delete m_MV2c20_sfFix50;

  delete m_MV2c20_isFix60;
  delete m_MV2c20_sfFix60;

  delete m_MV2c20_isFix70;
  delete m_MV2c20_sfFix70;

  delete m_MV2c20_isFix77;
  delete m_MV2c20_sfFix77;

  delete m_MV2c20_isFix80;
  delete m_MV2c20_sfFix80;

  delete m_MV2c20_isFix85;
  delete m_MV2c20_sfFix85;

  delete m_MV2c20_isFix90;
  delete m_MV2c20_sfFix90;

  delete m_MV2c20_isFlt30;
  delete m_MV2c20_sfFlt30;

  delete m_MV2c20_isFlt40;
  delete m_MV2c20_sfFlt40;

  delete m_MV2c20_isFlt50;
  delete m_MV2c20_sfFlt50;

  delete m_MV2c20_isFlt60;
  delete m_MV2c20_sfFlt60;

  delete m_MV2c20_isFlt70;
  delete m_MV2c20_sfFlt70;

  delete m_MV2c20_isFlt77;
  delete m_MV2c20_sfFlt77;

  delete m_MV2c20_isFlt85;
  delete m_MV2c20_sfFlt85;

  // truth
  delete m_ConeTruthLabelID;
  delete m_TruthCount;
  delete m_TruthLabelDeltaR_B;
  delete m_TruthLabelDeltaR_C;
  delete m_TruthLabelDeltaR_T;
  delete m_PartonTruthLabelID;
  delete m_GhostTruthAssociationFraction;
  delete m_truth_E;
  delete m_truth_pt;
  delete m_truth_phi;
  delete m_truth_eta;

  // charge
  delete m_charge;

}

void JetContainer::setTree(TTree *tree, std::string tagger)
{
  //
  // Connect branches
  ParticleContainer::setTree(tree);

  tree->SetBranchStatus  (("n"+m_name+"s").c_str() , 1);
  tree->SetBranchAddress (("n"+m_name+"s").c_str() , &m_n);

  if(m_infoSwitch.m_clean)
    {
      connectBranch<float>(tree,"Timing",                      &m_Timing);
      connectBranch<float>(tree,"LArQuality",                  &m_LArQuality);

      connectBranch<int>  (tree, "LArBadHVNCell",              &m_LArBadHVNCell);

      connectBranch<float>(tree, "LArQuality",                 &m_LArQuality);
      connectBranch<float>(tree, "HECQuality",                 &m_HECQuality);
      connectBranch<float>(tree, "NegativeE",                  &m_NegativeE);
      connectBranch<float>(tree, "AverageLArQF",               &m_AverageLArQF);
      connectBranch<float>(tree, "BchCorrCell",                &m_BchCorrCell);
      connectBranch<float>(tree, "N90Constituents",            &m_N90Constituents);
      connectBranch<float>(tree, "LArBadHVEFracnergyFrac",     &m_LArBadHVEFrac);
      connectBranch<float>(tree, "OotFracClusters5",           &m_OotFracClusters5);
      connectBranch<float>(tree, "OotFracClusters10",          &m_OotFracClusters10);
      connectBranch<float>(tree, "LeadingClusterPt",           &m_LeadingClusterPt);
      connectBranch<float>(tree, "LeadingClusterSecondLambda", &m_LeadingClusterSecondLambda);
      connectBranch<float>(tree, "LeadingClusterCenterLambda", &m_LeadingClusterCenterLambda);
      connectBranch<float>(tree, "LeadingClusterSecondR",      &m_LeadingClusterSecondR);
      connectBranch<int>  (tree, "clean_passLooseBad",         &m_clean_passLooseBad);
      connectBranch<int>  (tree, "clean_passLooseBadUgly",     &m_clean_passLooseBadUgly);
      connectBranch<int>  (tree, "clean_passTightBad",         &m_clean_passTightBad);
      connectBranch<int>  (tree, "clean_passTightBadUgly",     &m_clean_passTightBadUgly);
    }

  if(m_infoSwitch.m_energy)
    {
      connectBranch<float>(tree, "HECFrac",               &m_HECFrac);
      connectBranch<float>(tree, "EMFrac",                &m_EMFrac);
      connectBranch<float>(tree, "CentroidR",             &m_CentroidR);
      connectBranch<float>(tree, "FracSamplingMax",       &m_FracSamplingMax);
      connectBranch<float>(tree, "FracSamplingMaxIndex",  &m_FracSamplingMaxIndex);
      connectBranch<float>(tree, "LowEtConstituentsFrac", &m_LowEtConstituentsFrac);
      connectBranch<float>(tree, "GhostMuonSegmentCount", &m_GhostMuonSegmentCount);
      connectBranch<float>(tree, "Width",                 &m_Width);
    }

  if(m_infoSwitch.m_trackPV)
    {
      connectBranch<float>(tree, "NumTrkPt1000PV",     &m_NumTrkPt1000PV);
      connectBranch<float>(tree, "SumPtTrkPt1000PV",   &m_SumPtTrkPt1000PV);
      connectBranch<float>(tree, "TrackWidthPt1000PV", &m_TrackWidthPt1000PV);
      connectBranch<float>(tree, "NumTrkPt500PV",      &m_NumTrkPt500PV);
      connectBranch<float>(tree, "SumPtTrkPt500PV",    &m_SumPtTrkPt500PV);
      connectBranch<float>(tree, "TrackWidthPt500PV",  &m_TrackWidthPt500PV);
      connectBranch<float>(tree, "JVFPV",              &m_JVFPV);
    }

  if(m_infoSwitch.m_trackAll || m_infoSwitch.m_trackPV)
    {
      connectBranch<float>(tree, "Jvt",        &m_Jvt);
      connectBranch<float>(tree, "JvtJvfcorr", &m_JvtJvfcorr);
      connectBranch<float>(tree, "JvtRpt",     &m_JvtRpt);
    }

  if(m_infoSwitch.m_flavTag)
    {
      connectBranch<float>(tree,"SV0",                  &m_SV0);
      connectBranch<float>(tree,"SV1",                  &m_SV1);
      connectBranch<float>(tree,"IP3D",                 &m_IP3D);
      connectBranch<float>(tree,"SV1IP3D",              &m_SV1IP3D);
      //connectBranch<float>(tree,"MV1",                  &m_MV1);
      connectBranch<float>(tree,"MV2c00",               &m_MV2c00);
      connectBranch<float>(tree,"MV2c10",               &m_MV2c10);
      connectBranch<float>(tree,"MV2c20",               &m_MV2c20);
      connectBranch<int>  (tree,"HadronConeExclTruthLabelID",&m_HadronConeExclTruthLabelID);

      if(tagger == "MV2c20")  m_MV2 = m_MV2c20;
      if(tagger == "MV2c10")  m_MV2 = m_MV2c10;

    }

  for(uint i=0; i<m_infoSwitch.m_sfFTagFix.size(); i++ ) 
    {
      switch( m_infoSwitch.m_sfFTagFix[i] ) 
	{
	case 30:
	  tree->SetBranchStatus  ("njets_mv2c20_Fix30", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix30", &m_njets_MV2c20_Fix30);
	  connectBranch<int>                (tree,"MV2c20_isFix30",       &m_MV2c20_isFix30);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix30",       &m_MV2c20_sfFix30);
	  break;
	case 50:
	  tree->SetBranchStatus  ("njets_mv2c20_Fix50", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix50", &m_njets_MV2c20_Fix50);
	  connectBranch<int>                (tree,"MV2c20_isFix50",       &m_MV2c20_isFix50);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix50",       &m_MV2c20_sfFix50);
	  break;
	case 60:
	  tree->SetBranchStatus  ("njets_mv2c20_Fix60", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix60", &m_njets_MV2c20_Fix60);
	  connectBranch<int>                (tree,"MV2c20_isFix60",       &m_MV2c20_isFix60);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix60",       &m_MV2c20_sfFix60);
	  break;
	case 70:
	  tree->SetBranchStatus  ("njets_mv2c20_Fix70", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix70", &m_njets_MV2c20_Fix70);
	  connectBranch<int>                (tree,"MV2c20_isFix70",       &m_MV2c20_isFix70);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix70",       &m_MV2c20_sfFix70);
	  break;
	case 77:
	  tree->SetBranchStatus  ("njets_mv2c20_Fix77", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix77", &m_njets_MV2c20_Fix77);
	  connectBranch<int>                (tree,"MV2c20_isFix77",       &m_MV2c20_isFix77);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix77",       &m_MV2c20_sfFix77);
	  break;
	case 80:
	  tree->SetBranchStatus  ("njets_mv2c20_Fix80", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix80", &m_njets_MV2c20_Fix80);
	  connectBranch<int>                (tree,"MV2c20_isFix80",       &m_MV2c20_isFix80);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix80",       &m_MV2c20_sfFix80);
	  break;
	case 85:	  
	  tree->SetBranchStatus  ("njets_mv2c20_Fix85", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix85", &m_njets_MV2c20_Fix85);
	  connectBranch<int>                (tree,"MV2c20_isFix85",       &m_MV2c20_isFix85);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix85",       &m_MV2c20_sfFix85);
	  break;
	case 90:
	  tree->SetBranchStatus  ("njets_mv2c20_Fix90", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Fix90", &m_njets_MV2c20_Fix90);
	  connectBranch<int>                (tree,"MV2c20_isFix90",       &m_MV2c20_isFix90);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFix90",       &m_MV2c20_sfFix90);
	  break;
	}
    }

  for(uint i=0; i<m_infoSwitch.m_sfFTagFlt.size(); i++ ) 
    {
      switch( m_infoSwitch.m_sfFTagFlt[i] ) 
	{
	case 30:
	  tree->SetBranchStatus  ("njets_mv2c20_Flt30", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Flt30", &m_njets_MV2c20_Flt30);
	  connectBranch<int>                (tree,"MV2c20_isFlt30",       &m_MV2c20_isFlt30);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFlt30",       &m_MV2c20_sfFlt30);
	  break;
	case 40:
	  tree->SetBranchStatus  ("njets_mv2c20_Flt40", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Flt40", &m_njets_MV2c20_Flt40);
	  connectBranch<int>                (tree,"MV2c20_isFlt40",       &m_MV2c20_isFlt40);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFlt40",       &m_MV2c20_sfFlt40);
	  break;
	case 50:
	  tree->SetBranchStatus  ("njets_mv2c20_Flt50", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Flt50", &m_njets_MV2c20_Flt50);
	  connectBranch<int>                (tree,"MV2c20_isFlt50",       &m_MV2c20_isFlt50);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFlt50",       &m_MV2c20_sfFlt50);
	  break;
	case 60:	      
	  tree->SetBranchStatus  ("njets_mv2c20_Flt60", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Flt60", &m_njets_MV2c20_Flt60);
	  connectBranch<int>                (tree,"MV2c20_isFlt60",       &m_MV2c20_isFlt60);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFlt60",       &m_MV2c20_sfFlt60);
	  break;
	case 70:
	  tree->SetBranchStatus  ("njets_mv2c20_Flt70", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Flt70", &m_njets_MV2c20_Flt70);
	  connectBranch<int>                (tree,"MV2c20_isFlt70",       &m_MV2c20_isFlt70);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFlt70",       &m_MV2c20_sfFlt70);
	  break;
	case 77:
	  tree->SetBranchStatus  ("njets_mv2c20_Flt77", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Flt77", &m_njets_MV2c20_Flt77);
	  connectBranch<int>                (tree,"MV2c20_isFlt77",       &m_MV2c20_isFlt77);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFlt77",       &m_MV2c20_sfFlt77);
	  break;
	case 85:
	  tree->SetBranchStatus  ("njets_mv2c20_Flt85", 1);
	  tree->SetBranchAddress ("njets_mv2c20_Flt85", &m_njets_MV2c20_Flt85);
	  connectBranch<int>                (tree,"MV2c20_isFlt85",       &m_MV2c20_isFlt85);
	  if(m_mc) connectBranch<std::vector<float> >(tree,"MV2c20_SFFlt85",       &m_MV2c20_sfFlt85);
	  break;
	}
    }

  // truth
  if(m_infoSwitch.m_truth)
    {
      connectBranch<int>  (tree,"ConeTruthLabelID",   &m_ConeTruthLabelID);
      connectBranch<int>  (tree,"TruthCount",         &m_TruthCount);
      connectBranch<float>(tree,"TruthLabelDeltaR_B", &m_TruthLabelDeltaR_B);
      connectBranch<float>(tree,"TruthLabelDeltaR_C", &m_TruthLabelDeltaR_C);
      connectBranch<float>(tree,"TruthLabelDeltaR_T", &m_TruthLabelDeltaR_T);
      connectBranch<int>  (tree,"PartonTruthLabelID", &m_PartonTruthLabelID);
      connectBranch<float>(tree,"GhostTruthAssociationFraction", &m_GhostTruthAssociationFraction);
      connectBranch<float>(tree,"truth_E",   &m_truth_E);
      connectBranch<float>(tree,"truth_pt",  &m_truth_pt);
      connectBranch<float>(tree,"truth_phi", &m_truth_phi);
      connectBranch<float>(tree,"truth_eta", &m_truth_eta);
    }

  // charge
  if(m_infoSwitch.m_charge)
    {
      connectBranch<double>(tree,"charge", &m_charge);
    }
}

void JetContainer::updateParticle(uint idx, Jet& jet)
{
  ParticleContainer::updateParticle(idx,jet);  

  if(m_infoSwitch.m_clean)
    {
      jet.Timing                    =m_Timing                    ->at(idx);
      jet.LArQuality                =m_LArQuality                ->at(idx);
      jet.HECQuality                =m_HECQuality                ->at(idx);
      jet.NegativeE                 =m_NegativeE                 ->at(idx);
      jet.AverageLArQF              =m_AverageLArQF              ->at(idx);
      jet.BchCorrCell               =m_BchCorrCell               ->at(idx);
      jet.N90Constituents           =m_N90Constituents           ->at(idx);
      jet.LArBadHVEFrac             =m_LArBadHVEFrac             ->at(idx);
      jet.LArBadHVNCell             =m_LArBadHVNCell             ->at(idx);
      jet.OotFracClusters5          =m_OotFracClusters5          ->at(idx);
      jet.OotFracClusters10         =m_OotFracClusters10         ->at(idx);
      jet.LeadingClusterPt          =m_LeadingClusterPt          ->at(idx);
      jet.LeadingClusterSecondLambda=m_LeadingClusterSecondLambda->at(idx);
      jet.LeadingClusterCenterLambda=m_LeadingClusterCenterLambda->at(idx);
      jet.LeadingClusterSecondR     =m_LeadingClusterSecondR     ->at(idx);
      jet.clean_passLooseBad        =m_clean_passLooseBad        ->at(idx);
      jet.clean_passLooseBadUgly    =m_clean_passLooseBadUgly    ->at(idx);
      jet.clean_passTightBad        =m_clean_passTightBad        ->at(idx);
      jet.clean_passTightBadUgly    =m_clean_passTightBadUgly    ->at(idx);
    }

  if(m_infoSwitch.m_energy)
    {
      jet.HECFrac              =m_HECFrac              ->at(idx);
      jet.EMFrac               =m_EMFrac               ->at(idx);
      jet.CentroidR            =m_CentroidR            ->at(idx);
      jet.FracSamplingMax      =m_FracSamplingMax      ->at(idx);
      jet.FracSamplingMaxIndex =m_FracSamplingMaxIndex ->at(idx);
      jet.LowEtConstituentsFrac=m_LowEtConstituentsFrac->at(idx);
      jet.GhostMuonSegmentCount=m_GhostMuonSegmentCount->at(idx);
      jet.Width                =m_Width                ->at(idx);
    }
  
  if(m_infoSwitch.m_trackPV)
    {
      jet.NumTrkPt1000PV    =m_NumTrkPt1000PV    ->at(idx);
      jet.SumPtTrkPt1000PV  =m_SumPtTrkPt1000PV  ->at(idx);
      jet.TrackWidthPt1000PV=m_TrackWidthPt1000PV->at(idx);
      jet.NumTrkPt500PV     =m_NumTrkPt500PV     ->at(idx);
      jet.SumPtTrkPt500PV   =m_SumPtTrkPt500PV   ->at(idx);
      jet.TrackWidthPt500PV =m_TrackWidthPt500PV ->at(idx);
      jet.JVFPV             =m_JVFPV             ->at(idx);
    }

  if(m_infoSwitch.m_trackPV || m_infoSwitch.m_trackAll)
    {
      jet.Jvt       =m_Jvt       ->at(idx);
      jet.JvtJvfcorr=m_JvtJvfcorr->at(idx);
      jet.JvtRpt    =m_JvtRpt    ->at(idx);
    }
  
  if(m_infoSwitch.m_flavTag)
    {
      //jet.SV0                       =m_SV0                  ->at(idx);
      //jet.SV1                       =m_SV1                  ->at(idx);
      //jet.IP3D                      =m_IP3D                 ->at(idx);
      jet.SV1IP3D                   =m_SV1IP3D              ->at(idx);
      //jet.MV1                       =m_MV1                  ->at(idx);
      jet.MV2c00                    =m_MV2c00               ->at(idx);
      jet.MV2c10                    =m_MV2c10               ->at(idx);
      jet.MV2c20                    =m_MV2c20               ->at(idx);
      jet.MV2                       =m_MV2                  ->at(idx);
      //std::cout << m_HadronConeExclTruthLabelID->size() << std::endl;
      jet.HadronConeExclTruthLabelID=m_HadronConeExclTruthLabelID->at(idx);
    }

  static const std::vector<float> dummy1 = {1.};
  for(uint i=0; i<m_infoSwitch.m_sfFTagFix.size(); i++ ) 
    {
      switch( m_infoSwitch.m_sfFTagFix[i] ) 
	{
        case 30:
	  jet.MV2c20_isFix30       =m_MV2c20_isFix30       ->at(idx);
	  jet.MV2c20_sfFix30       =(m_mc)?m_MV2c20_sfFix30->at(idx):dummy1;
	  break;
        case 50:
	  jet.MV2c20_isFix50       =m_MV2c20_isFix50       ->at(idx);
	  jet.MV2c20_sfFix50       =(m_mc)?m_MV2c20_sfFix50->at(idx):dummy1;
	  break;
        case 60:
	  jet.MV2c20_isFix60       =m_MV2c20_isFix60       ->at(idx);
	  jet.MV2c20_sfFix60       =(m_mc)?m_MV2c20_sfFix60->at(idx):dummy1;
	  break;
        case 70:
	  jet.MV2c20_isFix70       =m_MV2c20_isFix70       ->at(idx);
	  jet.MV2c20_sfFix70       =(m_mc)?m_MV2c20_sfFix70->at(idx):dummy1;
	  break;
        case 77:
	  jet.MV2c20_isFix77       =m_MV2c20_isFix77       ->at(idx);
	  jet.MV2c20_sfFix77       =(m_mc)?m_MV2c20_sfFix77->at(idx):dummy1;
	  break;
        case 80:
	  jet.MV2c20_isFix80       =m_MV2c20_isFix80       ->at(idx);
	  jet.MV2c20_sfFix80       =(m_mc)?m_MV2c20_sfFix80->at(idx):dummy1;
	  break;
        case 85:
	  jet.MV2c20_isFix85       =m_MV2c20_isFix85       ->at(idx);
	  jet.MV2c20_sfFix85       =(m_mc)?m_MV2c20_sfFix85->at(idx):dummy1;
	  break;
        case 90:
	  jet.MV2c20_isFix90       =m_MV2c20_isFix90       ->at(idx);
	  jet.MV2c20_sfFix90       =(m_mc)?m_MV2c20_sfFix90->at(idx):dummy1;
	  break;
	}
    }

  for(uint i=0; i<m_infoSwitch.m_sfFTagFlt.size(); i++ ) 
    {
      switch( m_infoSwitch.m_sfFTagFlt[i] ) 
	{
        case 30:
	  jet.MV2c20_isFlt30       =m_MV2c20_isFlt30       ->at(idx);
	  jet.MV2c20_sfFlt30       =(m_mc)?m_MV2c20_sfFlt30->at(idx):dummy1;
	  break;
        case 40:
	  jet.MV2c20_isFlt40       =m_MV2c20_isFlt40       ->at(idx);
	  jet.MV2c20_sfFlt40       =(m_mc)?m_MV2c20_sfFlt40->at(idx):dummy1;
	  break;
        case 50:
	  jet.MV2c20_isFlt50       =m_MV2c20_isFlt50       ->at(idx);
	  jet.MV2c20_sfFlt50       =(m_mc)?m_MV2c20_sfFlt50->at(idx):dummy1;
	  break;
        case 60:
	  jet.MV2c20_isFlt60       =m_MV2c20_isFlt60       ->at(idx);
	  jet.MV2c20_sfFlt60       =(m_mc)?m_MV2c20_sfFlt60->at(idx):dummy1;
	  break;
        case 70:
	  jet.MV2c20_isFlt70       =m_MV2c20_isFlt70       ->at(idx);
	  jet.MV2c20_sfFlt70       =(m_mc)?m_MV2c20_sfFlt70->at(idx):dummy1;
	  break;
        case 77:
	  jet.MV2c20_isFlt77       =m_MV2c20_isFlt77       ->at(idx);
	  jet.MV2c20_sfFlt77       =(m_mc)?m_MV2c20_sfFlt77->at(idx):dummy1;
	  break;
        case 85:
	  jet.MV2c20_isFlt85       =m_MV2c20_isFlt85       ->at(idx);
	  jet.MV2c20_sfFlt85       =(m_mc)?m_MV2c20_sfFlt85->at(idx):dummy1;
	  break;
	}
    }

  // truth
  if(m_infoSwitch.m_truth)
    {
      jet.ConeTruthLabelID  =m_ConeTruthLabelID->at(idx);
      jet.TruthCount        =m_TruthCount      ->at(idx);
      jet.TruthLabelDeltaR_B=m_TruthLabelDeltaR_B->at(idx);
      jet.TruthLabelDeltaR_C=m_TruthLabelDeltaR_C->at(idx);
      jet.TruthLabelDeltaR_T=m_TruthLabelDeltaR_T->at(idx);
      jet.PartonTruthLabelID=m_PartonTruthLabelID->at(idx);
      jet.GhostTruthAssociationFraction= m_GhostTruthAssociationFraction->at(idx);

      jet.truth_p4.SetPtEtaPhiE(m_truth_pt ->at(idx),
				m_truth_eta->at(idx),
				m_truth_phi->at(idx),
				m_truth_E  ->at(idx));
    }

  // charge
  if(m_infoSwitch.m_charge)
    {
      jet.charge=m_charge->at(idx);
    }

}
