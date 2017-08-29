#include "xAODAnaHelpers/MetContainer.h"
#include <iostream>


using namespace xAH;

MetContainer::MetContainer(const std::string& detailStr, float units)
  : m_infoSwitch(detailStr), m_debug(false), m_units(units)
{
}

MetContainer::~MetContainer()
{
  if(m_debug) std::cout << " Deleting MetContainer "  << std::endl;
}

void MetContainer::setTree(TTree *tree)
{

  connectBranch<float>(tree, "metFinalClus",         &m_metFinalClus     );
  connectBranch<float>(tree, "metFinalClusPx",       &m_metFinalClusPx	 );
  connectBranch<float>(tree, "metFinalClusPy",       &m_metFinalClusPy	 );
  connectBranch<float>(tree, "metFinalClusSumEt",    &m_metFinalClusSumEt);
  connectBranch<float>(tree, "metFinalClusPhi",      &m_metFinalClusPhi  );

  if ( m_infoSwitch.m_sigClus ) {
    connectBranch<float>(tree, "metFinalClusOverSqrtSumEt",  &m_metFinalClusOverSqrtSumEt );
    connectBranch<float>(tree, "metFinalClusOverSqrtHt",     &m_metFinalClusOverSqrtHt    );
    connectBranch<float>(tree, "metFinalClusSignificance",   &m_metFinalClusSignificance  );
    connectBranch<float>(tree, "metFinalClusSigDirectional", &m_metFinalClusSigDirectional);
  }

  if ( m_infoSwitch.m_sigResolutionClus ) {
    connectBranch<float>(tree, "metFinalClusRho",  &m_metFinalClusRho );
    connectBranch<float>(tree, "metFinalClusVarL", &m_metFinalClusVarL);
    connectBranch<float>(tree, "metFinalClusVarT", &m_metFinalClusVarT);
  }

  connectBranch<float>(tree, "metFinalTrk",          &m_metFinalTrk      );
  connectBranch<float>(tree, "metFinalTrkPx",        &m_metFinalTrkPx	 );
  connectBranch<float>(tree, "metFinalTrkPy",        &m_metFinalTrkPy	 );
  connectBranch<float>(tree, "metFinalTrkSumEt",     &m_metFinalTrkSumEt );
  connectBranch<float>(tree, "metFinalTrkPhi",       &m_metFinalTrkPhi   );

  if ( m_infoSwitch.m_sigTrk ) {
    connectBranch<float>(tree, "metFinalTrkOverSqrtSumEt",  &m_metFinalTrkOverSqrtSumEt );
    connectBranch<float>(tree, "metFinalTrkOverSqrtHt",     &m_metFinalTrkOverSqrtHt    );
    connectBranch<float>(tree, "metFinalTrkSignificance",   &m_metFinalTrkSignificance  );
    connectBranch<float>(tree, "metFinalTrkSigDirectional", &m_metFinalTrkSigDirectional);
  }

  if ( m_infoSwitch.m_sigResolutionTrk ) {
    connectBranch<float>(tree, "metFinalTrkRho",  &m_metFinalTrkRho );
    connectBranch<float>(tree, "metFinalTrkVarL", &m_metFinalTrkVarL);
    connectBranch<float>(tree, "metFinalTrkVarT", &m_metFinalTrkVarT);
  }

  if ( m_infoSwitch.m_refEle ) {
    connectBranch<float>(tree, "metEle",             &m_metEle       );
    connectBranch<float>(tree, "metEleSumEt",        &m_metEleSumEt  );
    connectBranch<float>(tree, "metElePhi",          &m_metElePhi    );
  }

  if ( m_infoSwitch.m_refGamma ) {
    connectBranch<float>(tree, "metGamma",           &m_metGamma     );
    connectBranch<float>(tree, "metGammaSumEt",      &m_metGammaSumEt);
    connectBranch<float>(tree, "metGammaPhi",        &m_metGammaPhi  );
  }

  if ( m_infoSwitch.m_refTau ) {
    connectBranch<float>(tree, "metTau",             &m_metTau      );
    connectBranch<float>(tree, "metTauSumEt",        &m_metTauSumEt );
    connectBranch<float>(tree, "metTauPhi",          &m_metTauPhi   );
  }

  if ( m_infoSwitch.m_refMuons ) {
    connectBranch<float>(tree, "metMuons",           &m_metMuons      );
    connectBranch<float>(tree, "metMuonsSumEt",      &m_metMuonsSumEt );
    connectBranch<float>(tree, "metMuonsPhi",        &m_metMuonsPhi   );
  }

  if ( m_infoSwitch.m_refJet ) {
    connectBranch<float>(tree, "metJet",             &m_metJet      );
    connectBranch<float>(tree, "metJetSumEt",        &m_metJetSumEt );
    connectBranch<float>(tree, "metJetPhi",          &m_metJetPhi   );
  }

  if ( m_infoSwitch.m_refJetTrk ) {
    connectBranch<float>(tree, "metJetTrk",          &m_metJetTrk     );
    connectBranch<float>(tree, "metJetTrkSumEt",     &m_metJetTrkSumEt);
    connectBranch<float>(tree, "metJetTrkPhi",       &m_metJetTrkPhi  );
  }

  if ( m_infoSwitch.m_softClus) {
    connectBranch<float>(tree, "metSoftClus",        &m_metSoftClus     );
    connectBranch<float>(tree, "metSoftClusSumEt",   &m_metSoftClusSumEt);
    connectBranch<float>(tree, "metSoftClusPhi",     &m_metSoftClusPhi  );
  }

  if ( m_infoSwitch.m_softTrk) {
    connectBranch<float>(tree, "metSoftTrk",         &m_metSoftTrk      );
    connectBranch<float>(tree, "metSoftTrkSumEt",    &m_metSoftTrkSumEt	);
    connectBranch<float>(tree, "metSoftTrkPhi",      &m_metSoftTrkPhi   );
  }

}


void MetContainer::setBranches(TTree *tree)
{


  tree->Branch("metFinalClus",         &m_metFinalClus,      "metFinalClus/F");
  tree->Branch("metFinalClusPx",       &m_metFinalClusPx,    "metFinalClusPx/F");
  tree->Branch("metFinalClusPy",       &m_metFinalClusPy,    "metFinalClusPy/F");
  tree->Branch("metFinalClusSumEt",    &m_metFinalClusSumEt, "metFinalClusSumEt/F");
  tree->Branch("metFinalClusPhi",      &m_metFinalClusPhi,   "metFinalClusPhi/F");

  if ( m_infoSwitch.m_sigClus ) {
    tree->Branch("metFinalClusOverSqrtSumEt",  &m_metFinalClusOverSqrtSumEt,  "metFinalClusOverSqrtSumEt/F" );
    tree->Branch("metFinalClusOverSqrtHt",     &m_metFinalClusOverSqrtHt,     "metFinalClusOverSqrtHt/F"    );
    tree->Branch("metFinalClusSignificance",   &m_metFinalClusSignificance,   "metFinalClusSignificance/F"  );
    tree->Branch("metFinalClusSigDirectional", &m_metFinalClusSigDirectional, "metFinalClusSigDirectional/F");
  }

  if ( m_infoSwitch.m_sigResolutionClus ) {
    tree->Branch("metFinalClusRho",  &m_metFinalClusRho,  "metFinalClusRho/F" );
    tree->Branch("metFinalClusVarL", &m_metFinalClusVarL, "metFinalClusVarL/F");
    tree->Branch("metFinalClusVarT", &m_metFinalClusVarT, "metFinalClusVarT/F");
  }

  tree->Branch("metFinalTrk",          &m_metFinalTrk,       "metFinalTrk/F");
  tree->Branch("metFinalTrkPx",        &m_metFinalTrkPx,     "metFinalTrkPx/F");
  tree->Branch("metFinalTrkPy",        &m_metFinalTrkPy,     "metFinalTrkPy/F");
  tree->Branch("metFinalTrkSumEt",     &m_metFinalTrkSumEt,  "metFinalTrkSumEt/F");
  tree->Branch("metFinalTrkPhi",       &m_metFinalTrkPhi,    "metFinalTrkPhi/F");

  if ( m_infoSwitch.m_sigTrk ) {
    tree->Branch("metFinalTrkOverSqrtSumEt",  &m_metFinalTrkOverSqrtSumEt,  "metFinalTrkOverSqrtSumEt/F" );
    tree->Branch("metFinalTrkOverSqrtHt",     &m_metFinalTrkOverSqrtHt,     "metFinalTrkOverSqrtHt/F"    );
    tree->Branch("metFinalTrkSignificance",   &m_metFinalTrkSignificance,   "metFinalTrkSignificance/F"  );
    tree->Branch("metFinalTrkSigDirectional", &m_metFinalTrkSigDirectional, "metFinalTrkSigDirectional/F");
  }

  if ( m_infoSwitch.m_sigResolutionTrk ) {
    tree->Branch("metFinalTrkRho",  &m_metFinalTrkRho,  "metFinalTrkRho/F" );
    tree->Branch("metFinalTrkVarL", &m_metFinalTrkVarL, "metFinalTrkVarL/F");
    tree->Branch("metFinalTrkVarT", &m_metFinalTrkVarT, "metFinalTrkVarT/F");
  }

  if ( m_infoSwitch.m_refEle ) {
    tree->Branch("metEle",             &m_metEle,            "metEle/F");
    tree->Branch("metEleSumEt",        &m_metEleSumEt,       "metEleSumEt/F");
    tree->Branch("metElePhi",          &m_metElePhi,         "metElePhi/F");
  }

  if ( m_infoSwitch.m_refGamma ) {
    tree->Branch("metGamma",           &m_metGamma,          "metGamma/F");
    tree->Branch("metGammaSumEt",      &m_metGammaSumEt,     "metGammaSumEt/F");
    tree->Branch("metGammaPhi",        &m_metGammaPhi,       "metGammaPhi/F");
  }

  if ( m_infoSwitch.m_refTau ) {
    tree->Branch("metTau",             &m_metTau,            "metTau/F");
    tree->Branch("metTauSumEt",        &m_metTauSumEt,       "metTauSumEt/F");
    tree->Branch("metTauPhi",          &m_metTauPhi,         "metTauPhi/F");
  }

  if ( m_infoSwitch.m_refMuons ) {
    tree->Branch("metMuons",           &m_metMuons,          "metMuons/F");
    tree->Branch("metMuonsSumEt",      &m_metMuonsSumEt,     "metMuonsSumEt/F");
    tree->Branch("metMuonsPhi",        &m_metMuonsPhi,       "metMuonsPhi/F");
  }

  if ( m_infoSwitch.m_refJet ) {
    tree->Branch("metJet",             &m_metJet,            "metJet/F");
    tree->Branch("metJetSumEt",        &m_metJetSumEt,       "metJetSumEt/F");
    tree->Branch("metJetPhi",          &m_metJetPhi,         "metJetPhi/F");
  }

  if ( m_infoSwitch.m_refJetTrk ) {
    tree->Branch("metJetTrk",          &m_metJetTrk,         "metJetTrk/F");
    tree->Branch("metJetTrkSumEt",     &m_metJetTrkSumEt,    "metJetTrkSumEt/F");
    tree->Branch("metJetTrkPhi",       &m_metJetTrkPhi,      "metJetTrkPhi/F");
  }

  if ( m_infoSwitch.m_softClus) {
    tree->Branch("metSoftClus",        &m_metSoftClus,       "metSoftClus/F");
    tree->Branch("metSoftClusSumEt",   &m_metSoftClusSumEt,  "metSoftClusSumEt/F");
    tree->Branch("metSoftClusPhi",     &m_metSoftClusPhi,    "metSoftClussPhi/F");
  }

  if ( m_infoSwitch.m_softTrk) {
    tree->Branch("metSoftTrk",         &m_metSoftTrk,        "metSoftTrk/F");
    tree->Branch("metSoftTrkSumEt",    &m_metSoftTrkSumEt,   "metSoftTrkSumEt/F");
    tree->Branch("metSoftTrkPhi",      &m_metSoftTrkPhi,     "metSoftTrksPhi/F");
  }


  return;
}


void MetContainer::clear()
{
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

  if ( m_infoSwitch.m_sigClus ) {
    m_metFinalClusOverSqrtSumEt  = -999;
    m_metFinalClusOverSqrtHt     = -999;
    m_metFinalClusSignificance   = -999;
    m_metFinalClusSigDirectional = -999;
  }
  if ( m_infoSwitch.m_sigTrk ) {
    m_metFinalTrkOverSqrtSumEt  = -999;
    m_metFinalTrkOverSqrtHt     = -999;
    m_metFinalTrkSignificance   = -999;
    m_metFinalTrkSigDirectional = -999;
  }
  if ( m_infoSwitch.m_sigResolutionClus ) {
    m_metFinalClusRho  = -999;
    m_metFinalClusVarL = -999;
    m_metFinalClusVarT = -999;
  }
  if ( m_infoSwitch.m_sigResolutionTrk ) {
    m_metFinalTrkRho  = -999;
    m_metFinalTrkVarL = -999;
    m_metFinalTrkVarT = -999;
  }

  if ( m_infoSwitch.m_refEle ) {
    m_metEle = m_metEleSumEt = m_metElePhi = -999;
  }
  if ( m_infoSwitch.m_refGamma ) {
    m_metGamma = m_metGammaSumEt = m_metGammaPhi = -999;
  }
  if ( m_infoSwitch.m_refTau ) {
    m_metTau = m_metTauSumEt = m_metTauPhi = -999;
  }
  if ( m_infoSwitch.m_refMuons ) {
    m_metMuons = m_metMuonsSumEt = m_metMuonsPhi = -999;
  }
  if ( m_infoSwitch.m_refJet ) {
    m_metJet = m_metJetSumEt = m_metJetPhi = -999;
  }
  if ( m_infoSwitch.m_refJetTrk ) {
    m_metJetTrk = m_metJetTrkSumEt = m_metJetTrkPhi = -999;
  }
  if ( m_infoSwitch.m_softClus) {
    m_metSoftClus = m_metSoftClusSumEt = m_metSoftClusPhi = -999;
  }

  return;
}

void MetContainer::FillMET( const xAOD::MissingETContainer* met) {


  //if ( m_debug ) { Info("HelpTreeBase::FillMET()", "Filling MET info"); }

  const xAOD::MissingET* final_clus = *met->find("FinalClus"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based onleares)
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

  if ( m_infoSwitch.m_sigClus ) {
    m_metFinalClusOverSqrtSumEt  = final_clus->auxdecor<double>("METOverSqrtSumET");
    m_metFinalClusOverSqrtHt     = final_clus->auxdecor<double>("METOverSqrtHT");
    m_metFinalClusSignificance   = final_clus->auxdecor<double>("Significance");
    m_metFinalClusSigDirectional = final_clus->auxdecor<double>("SigDirectional");
  }

  if ( m_infoSwitch.m_sigTrk ) {
    m_metFinalTrkOverSqrtSumEt  = final_trk->auxdecor<double>("METOverSqrtSumET");
    m_metFinalTrkOverSqrtHt     = final_trk->auxdecor<double>("METOverSqrtHT");
    m_metFinalTrkSignificance   = final_trk->auxdecor<double>("Significance");
    m_metFinalTrkSigDirectional = final_trk->auxdecor<double>("SigDirectional");
  }

  if ( m_infoSwitch.m_sigResolutionClus ) {
    m_metFinalClusRho  = final_clus->auxdecor<double>("Rho");
    m_metFinalClusVarL = final_clus->auxdecor<double>("VarL");
    m_metFinalClusVarT = final_clus->auxdecor<double>("VarT");
  }

  if ( m_infoSwitch.m_sigResolutionTrk ) {
    m_metFinalTrkRho  = final_trk->auxdecor<double>("Rho");
    m_metFinalTrkVarL = final_trk->auxdecor<double>("VarL");
    m_metFinalTrkVarT = final_trk->auxdecor<double>("VarT");
  }

  if ( m_infoSwitch.m_refEle ) {
    const xAOD::MissingET* ref_ele       = *met->find("RefEle");
    m_metEle         		         = ref_ele->met() / m_units;
    m_metEleSumEt    		         = ref_ele->sumet() / m_units;
    m_metElePhi      		         = ref_ele->phi();
  }

  if ( m_infoSwitch.m_refGamma ) {
    const xAOD::MissingET* ref_gamma     = *met->find("RefGamma");
    m_metGamma             	         = ref_gamma->met() / m_units;
    m_metGammaSumEt        	         = ref_gamma->sumet() / m_units;
    m_metGammaPhi          	         = ref_gamma->phi();
  }

  if ( m_infoSwitch.m_refTau ) {
    const xAOD::MissingET* ref_tau  	 = *met->find("RefTau");
    m_metTau             	    	 = ref_tau->met() / m_units;
    m_metTauSumEt        	    	 = ref_tau->sumet() / m_units;
    m_metTauPhi          	    	 = ref_tau->phi();
  }

  if ( m_infoSwitch.m_refMuons ) {
    const xAOD::MissingET* ref_muon  	 = *met->find("Muons");
    m_metMuons             	     	 = ref_muon->met() / m_units;
    m_metMuonsSumEt        	     	 = ref_muon->sumet() / m_units;
    m_metMuonsPhi          	     	 = ref_muon->phi();
  }

  if ( m_infoSwitch.m_refJet ) {
    const xAOD::MissingET* ref_jet  	 = *met->find("RefJet");
    m_metJet             	     	 = ref_jet->met() / m_units;
    m_metJetSumEt        	     	 = ref_jet->sumet() / m_units;
    m_metJetPhi          	     	 = ref_jet->phi();
  }

  if ( m_infoSwitch.m_refJetTrk ) {
    const xAOD::MissingET* ref_jet_trk   = *met->find("RefJetTrk");
    m_metJetTrk             	     	 = ref_jet_trk->met() / m_units;
    m_metJetTrkSumEt        	     	 = ref_jet_trk->sumet() / m_units;
    m_metJetTrkPhi          	     	 = ref_jet_trk->phi();
  }

  if ( m_infoSwitch.m_softClus) {
    const xAOD::MissingET* ref_soft_clus = *met->find("SoftClus");
    m_metSoftClus            		 = ref_soft_clus->met() / m_units;
    m_metSoftClusSumEt       		 = ref_soft_clus->sumet() / m_units;
    m_metSoftClusPhi         		 = ref_soft_clus->phi();
  }

  if ( m_infoSwitch.m_softTrk) {
    const xAOD::MissingET* ref_soft_trk  = *met->find("PVSoftTrk");
    m_metSoftTrk             		 = ref_soft_trk->met() / m_units;
    m_metSoftTrkSumEt        		 = ref_soft_trk->sumet() / m_units;
    m_metSoftTrkPhi          		 = ref_soft_trk->phi();
  }


  return;
}
