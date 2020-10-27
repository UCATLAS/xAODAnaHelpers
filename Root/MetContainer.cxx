#include "xAODAnaHelpers/MetContainer.h"
#include <iostream>


using namespace xAH;

MetContainer::MetContainer(const std::string& name, const std::string& detailStr, float units)
  : m_name(name), m_infoSwitch(detailStr), m_debug(false), m_units(units)
{
}

MetContainer::~MetContainer()
{
  if(m_debug) std::cout << " Deleting MetContainer "  << std::endl;
}

void MetContainer::setTree(TTree *tree)
{

  if ( m_infoSwitch.m_metClus ) {
    connectBranch<float>(tree, "FinalClus",         &m_metFinalClus     );
    connectBranch<float>(tree, "FinalClusPx",       &m_metFinalClusPx   );
    connectBranch<float>(tree, "FinalClusPy",       &m_metFinalClusPy   );
    connectBranch<float>(tree, "FinalClusSumEt",    &m_metFinalClusSumEt);
    connectBranch<float>(tree, "FinalClusPhi",      &m_metFinalClusPhi  );
  }

  if ( m_infoSwitch.m_sigClus ) {
    connectBranch<float>(tree, "FinalClusOverSqrtSumEt",  &m_metFinalClusOverSqrtSumEt );
    connectBranch<float>(tree, "FinalClusOverSqrtHt",     &m_metFinalClusOverSqrtHt    );
    connectBranch<float>(tree, "FinalClusSignificance",   &m_metFinalClusSignificance  );
    connectBranch<float>(tree, "FinalClusSigDirectional", &m_metFinalClusSigDirectional);
  }

  if ( m_infoSwitch.m_sigResolutionClus ) {
    connectBranch<float>(tree, "FinalClusRho",  &m_metFinalClusRho );
    connectBranch<float>(tree, "FinalClusVarL", &m_metFinalClusVarL);
    connectBranch<float>(tree, "FinalClusVarT", &m_metFinalClusVarT);
  }

  if ( m_infoSwitch.m_metTrk ) {
    connectBranch<float>(tree, "FinalTrk",          &m_metFinalTrk     );
    connectBranch<float>(tree, "FinalTrkPx",        &m_metFinalTrkPx   );
    connectBranch<float>(tree, "FinalTrkPy",        &m_metFinalTrkPy   );
    connectBranch<float>(tree, "FinalTrkSumEt",     &m_metFinalTrkSumEt);
    connectBranch<float>(tree, "FinalTrkPhi",       &m_metFinalTrkPhi  );
  }

  if ( m_infoSwitch.m_sigTrk ) {
    connectBranch<float>(tree, "FinalTrkOverSqrtSumEt",  &m_metFinalTrkOverSqrtSumEt );
    connectBranch<float>(tree, "FinalTrkOverSqrtHt",     &m_metFinalTrkOverSqrtHt    );
    connectBranch<float>(tree, "FinalTrkSignificance",   &m_metFinalTrkSignificance  );
    connectBranch<float>(tree, "FinalTrkSigDirectional", &m_metFinalTrkSigDirectional);
  }

  if ( m_infoSwitch.m_sigResolutionTrk ) {
    connectBranch<float>(tree, "FinalTrkRho",  &m_metFinalTrkRho );
    connectBranch<float>(tree, "FinalTrkVarL", &m_metFinalTrkVarL);
    connectBranch<float>(tree, "FinalTrkVarT", &m_metFinalTrkVarT);
  }

  if ( m_infoSwitch.m_refEle ) {
    connectBranch<float>(tree, "Ele",             &m_metEle       );
    connectBranch<float>(tree, "EleSumEt",        &m_metEleSumEt  );
    connectBranch<float>(tree, "ElePhi",          &m_metElePhi    );
  }

  if ( m_infoSwitch.m_refGamma ) {
    connectBranch<float>(tree, "Gamma",           &m_metGamma     );
    connectBranch<float>(tree, "GammaSumEt",      &m_metGammaSumEt);
    connectBranch<float>(tree, "GammaPhi",        &m_metGammaPhi  );
  }

  if ( m_infoSwitch.m_refTau ) {
    connectBranch<float>(tree, "Tau",             &m_metTau      );
    connectBranch<float>(tree, "TauSumEt",        &m_metTauSumEt );
    connectBranch<float>(tree, "TauPhi",          &m_metTauPhi   );
  }

  if ( m_infoSwitch.m_refMuons ) {
    connectBranch<float>(tree, "Muons",           &m_metMuons      );
    connectBranch<float>(tree, "MuonsSumEt",      &m_metMuonsSumEt );
    connectBranch<float>(tree, "MuonsPhi",        &m_metMuonsPhi   );
  }

  if ( m_infoSwitch.m_refJet ) {
    connectBranch<float>(tree, "Jet",             &m_metJet      );
    connectBranch<float>(tree, "JetSumEt",        &m_metJetSumEt );
    connectBranch<float>(tree, "JetPhi",          &m_metJetPhi   );
  }

  if ( m_infoSwitch.m_refJetTrk ) {
    connectBranch<float>(tree, "JetTrk",          &m_metJetTrk     );
    connectBranch<float>(tree, "JetTrkSumEt",     &m_metJetTrkSumEt);
    connectBranch<float>(tree, "JetTrkPhi",       &m_metJetTrkPhi  );
  }

  if ( m_infoSwitch.m_softClus) {
    connectBranch<float>(tree, "SoftClus",        &m_metSoftClus     );
    connectBranch<float>(tree, "SoftClusSumEt",   &m_metSoftClusSumEt);
    connectBranch<float>(tree, "SoftClusPhi",     &m_metSoftClusPhi  );
  }

  if ( m_infoSwitch.m_softTrk) {
    connectBranch<float>(tree, "SoftTrk",         &m_metSoftTrk     );
    connectBranch<float>(tree, "SoftTrkSumEt",    &m_metSoftTrkSumEt);
    connectBranch<float>(tree, "SoftTrkPhi",      &m_metSoftTrkPhi  );
  }

}


void MetContainer::setBranches(TTree *tree)
{

  if ( m_infoSwitch.m_metClus ) {
    setBranch(tree, "FinalClus",         &m_metFinalClus,      "F");
    setBranch(tree, "FinalClusPx",       &m_metFinalClusPx,    "F");
    setBranch(tree, "FinalClusPy",       &m_metFinalClusPy,    "F");
    setBranch(tree, "FinalClusSumEt",    &m_metFinalClusSumEt, "F");
    setBranch(tree, "FinalClusPhi",      &m_metFinalClusPhi,   "F");
  }

  if ( m_infoSwitch.m_sigClus ) {
    setBranch(tree, "FinalClusOverSqrtSumEt",  &m_metFinalClusOverSqrtSumEt,  "/F");
    setBranch(tree, "FinalClusOverSqrtHt",     &m_metFinalClusOverSqrtHt,     "/F");
    setBranch(tree, "FinalClusSignificance",   &m_metFinalClusSignificance,   "/F");
    setBranch(tree, "FinalClusSigDirectional", &m_metFinalClusSigDirectional, "/F");
  }

  if ( m_infoSwitch.m_sigResolutionClus ) {
    setBranch(tree, "FinalClusRho",  &m_metFinalClusRho,  "F");
    setBranch(tree, "FinalClusVarL", &m_metFinalClusVarL, "F");
    setBranch(tree, "FinalClusVarT", &m_metFinalClusVarT, "F");
  }

  if ( m_infoSwitch.m_metTrk ) {
    setBranch(tree, "FinalTrk",          &m_metFinalTrk,       "F");
    setBranch(tree, "FinalTrkPx",        &m_metFinalTrkPx,     "F");
    setBranch(tree, "FinalTrkPy",        &m_metFinalTrkPy,     "F");
    setBranch(tree, "FinalTrkSumEt",     &m_metFinalTrkSumEt,  "F");
    setBranch(tree, "FinalTrkPhi",       &m_metFinalTrkPhi,    "F");
  }

  if ( m_infoSwitch.m_sigTrk ) {
    setBranch(tree, "FinalTrkOverSqrtSumEt",  &m_metFinalTrkOverSqrtSumEt,  "F");
    setBranch(tree, "FinalTrkOverSqrtHt",     &m_metFinalTrkOverSqrtHt,     "F");
    setBranch(tree, "FinalTrkSignificance",   &m_metFinalTrkSignificance,   "F");
    setBranch(tree, "FinalTrkSigDirectional", &m_metFinalTrkSigDirectional, "F");
  }

  if ( m_infoSwitch.m_sigResolutionTrk ) {
    setBranch(tree, "FinalTrkRho",  &m_metFinalTrkRho,  "F");
    setBranch(tree, "FinalTrkVarL", &m_metFinalTrkVarL, "F");
    setBranch(tree, "FinalTrkVarT", &m_metFinalTrkVarT, "F");
  }

  if ( m_infoSwitch.m_refEle ) {
    setBranch(tree, "Ele",             &m_metEle,            "F");
    setBranch(tree, "EleSumEt",        &m_metEleSumEt,       "F");
    setBranch(tree, "ElePhi",          &m_metElePhi,         "F");
  }

  if ( m_infoSwitch.m_refGamma ) {
    setBranch(tree, "Gamma",           &m_metGamma,          "F");
    setBranch(tree, "GammaSumEt",      &m_metGammaSumEt,     "F");
    setBranch(tree, "GammaPhi",        &m_metGammaPhi,       "F");
  }

  if ( m_infoSwitch.m_refTau ) {
    setBranch(tree, "Tau",             &m_metTau,            "F");
    setBranch(tree, "TauSumEt",        &m_metTauSumEt,       "F");
    setBranch(tree, "TauPhi",          &m_metTauPhi,         "F");
  }

  if ( m_infoSwitch.m_refMuons ) {
    setBranch(tree, "Muons",           &m_metMuons,          "F");
    setBranch(tree, "MuonsSumEt",      &m_metMuonsSumEt,     "F");
    setBranch(tree, "MuonsPhi",        &m_metMuonsPhi,       "F");
  }

  if ( m_infoSwitch.m_refJet ) {
    setBranch(tree, "Jet",             &m_metJet,            "F");
    setBranch(tree, "JetSumEt",        &m_metJetSumEt,       "F");
    setBranch(tree, "JetPhi",          &m_metJetPhi,         "F");
  }

  if ( m_infoSwitch.m_refJetTrk ) {
    setBranch(tree, "JetTrk",          &m_metJetTrk,         "F");
    setBranch(tree, "JetTrkSumEt",     &m_metJetTrkSumEt,    "F");
    setBranch(tree, "JetTrkPhi",       &m_metJetTrkPhi,      "F");
  }

  if ( m_infoSwitch.m_softClus) {
    setBranch(tree, "SoftClus",        &m_metSoftClus,       "F");
    setBranch(tree, "SoftClusSumEt",   &m_metSoftClusSumEt,  "F");
    setBranch(tree, "SoftClusPhi",     &m_metSoftClusPhi,    "F");
  }

  if ( m_infoSwitch.m_softTrk) {
    setBranch(tree, "SoftTrk",         &m_metSoftTrk,        "F");
    setBranch(tree, "SoftTrkSumEt",    &m_metSoftTrkSumEt,   "F");
    setBranch(tree, "SoftTrkPhi",      &m_metSoftTrkPhi,     "F");
  }


  return;
}


void MetContainer::clear()
{
  if ( m_infoSwitch.m_metClus ) {
    m_metFinalClus      = -999;
    m_metFinalClusPx    = -999;
    m_metFinalClusPy    = -999;
    m_metFinalClusSumEt = -999;
    m_metFinalClusPhi   = -999;
  }

  if ( m_infoSwitch.m_metTrk ) {
    m_metFinalTrk	      = -999;
    m_metFinalTrkPx     = -999;
    m_metFinalTrkPy     = -999;
    m_metFinalTrkSumEt  = -999;
    m_metFinalTrkPhi    = -999;
  }

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

  if ( m_infoSwitch.m_metClus ) {
    const xAOD::MissingET* final_clus = *met->find("FinalClus"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based onleares)
    m_metFinalClus      = final_clus->met() / m_units;
    m_metFinalClusPx    = final_clus->mpx() / m_units;
    m_metFinalClusPy    = final_clus->mpy() / m_units;
    m_metFinalClusSumEt = final_clus->sumet() / m_units;
    m_metFinalClusPhi   = final_clus->phi();
    
    if ( m_infoSwitch.m_sigClus ) {
      m_metFinalClusOverSqrtSumEt  = final_clus->auxdecor<double>("METOverSqrtSumET");
      m_metFinalClusOverSqrtHt     = final_clus->auxdecor<double>("METOverSqrtHT");
      m_metFinalClusSignificance   = final_clus->auxdecor<double>("Significance");
      m_metFinalClusSigDirectional = final_clus->auxdecor<double>("SigDirectional");
    }
    
    if ( m_infoSwitch.m_sigResolutionClus ) {
      m_metFinalClusRho  = final_clus->auxdecor<double>("Rho");
      m_metFinalClusVarL = final_clus->auxdecor<double>("VarL");
      m_metFinalClusVarT = final_clus->auxdecor<double>("VarT");
    }
  }

  if ( m_infoSwitch.m_metTrk ) {
    const xAOD::MissingET* final_trk = *met->find("FinalTrk"); // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
    m_metFinalTrk       = final_trk->met() / m_units;
    m_metFinalTrkPx     = final_trk->mpx() / m_units;
    m_metFinalTrkPy     = final_trk->mpy() / m_units;
    m_metFinalTrkSumEt  = final_trk->sumet() / m_units;
    m_metFinalTrkPhi    = final_trk->phi();

    if ( m_infoSwitch.m_sigTrk ) {
      m_metFinalTrkOverSqrtSumEt  = final_trk->auxdecor<double>("METOverSqrtSumET");
      m_metFinalTrkOverSqrtHt     = final_trk->auxdecor<double>("METOverSqrtHT");
      m_metFinalTrkSignificance   = final_trk->auxdecor<double>("Significance");
      m_metFinalTrkSigDirectional = final_trk->auxdecor<double>("SigDirectional");
    }

    if ( m_infoSwitch.m_sigResolutionTrk ) {
      m_metFinalTrkRho  = final_trk->auxdecor<double>("Rho");
      m_metFinalTrkVarL = final_trk->auxdecor<double>("VarL");
      m_metFinalTrkVarT = final_trk->auxdecor<double>("VarT");
    }
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
