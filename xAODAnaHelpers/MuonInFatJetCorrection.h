#ifndef xAODAnaHelpers_MuonInFatJetCorrection_H
#define xAODAnaHelpers_MuonInFatJetCorrection_H

#include <xAODAnaHelpers/Algorithm.h>
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include "MuonMomentumCorrections/MuonCalibrationPeriodTool.h"

#include "JetCalibTools/JetCalibrationTool.h"

class MuonInFatJetCorrection : public xAH::Algorithm {

  private:
    float m_trackJetPtMin;
    float m_trackJetEtaMax;
    float m_trackJetNConst;
    float m_muonPtMin;
    float m_muonEtaMax;
    float m_muonDrMax;
    bool  doVR;
    bool  m_debug;

    std::string m_inContainerName = "";

    CP::MuonSelectionTool *m_muonSelectionTool;
    CP::MuonCalibrationPeriodTool *m_muonCalibrationPeriodTool;
	
    JetCalibrationTool *m_fatJetCalibration;

    std::string m_fatJetAlgo;
    std::string m_fatJetCalibSeq;
    std::string m_fatCalibArea;
    std::string m_fatJetConfig;

  public:

    MuonInFatJetCorrection();

    virtual EL::StatusCode setupJob(EL::Job &job);
    virtual EL::StatusCode histInitialize();
    virtual EL::StatusCode fileExecute();
    virtual EL::StatusCode changeInput(bool firstFile);
    virtual EL::StatusCode initialize();
    virtual EL::StatusCode execute();
    virtual EL::StatusCode finalize();

    TTree *tree;
    std::vector<TLorentzVector> muonCorrectedFatJet;

    EL::StatusCode getHbbCorrectedVector(const xAOD::Jet &jet, TLorentzVector &correctedVector, const bool doVR);
    EL::StatusCode decorateWithMuons(const xAOD::Jet& jet, const bool doVR) const;
    const xAOD::JetFourMom_t getMuonCorrectedJetFourMom(const xAOD::Jet &jet, std::vector<const xAOD::Muon*> muons,
                                                        std::string scheme, bool useJMSScale = false) const;
 
    
    ClassDef(MuonInFatJetCorrection, 1);
};

#endif
