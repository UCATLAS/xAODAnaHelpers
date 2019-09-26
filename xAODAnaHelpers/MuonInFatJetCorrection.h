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

    CP::MuonSelectionTool *m_muonSelectionTool = new CP::MuonSelectionTool("MuonSelectionTool");
    CP::MuonCalibrationPeriodTool *m_muonCalibrationPeriodTool = new CP::MuonCalibrationPeriodTool("MuonCalibrationPeriodToolForHbb");

  public:

    MuonInFatJetCorrection();
    
    std::string m_inContainerName = "";
    //CP::MuonSelectionTool *m_muonSelectionTool;
    //CP::MuonCalibrationPeriodTool *m_muonCalibrationPeriodTool;

    // Don't need this anymore (it may be the null pointer that was causing the crash as well)	
    //JetCalibrationTool *m_fatJetCalibration;
    
    float m_trackJetPtMin = 10000.0;
    float m_trackJetEtaMax = 2.5;
    float m_trackJetNConst = 2.0;
    float m_muonPtMin = 10000.0;
    float m_muonEtaMax = 2.7;
    float m_muonDrMax = 0.4;
    bool  doVR = true;
    bool  m_debug = false;

    virtual EL::StatusCode setupJob(EL::Job& job);
    virtual EL::StatusCode histInitialize();
    virtual EL::StatusCode fileExecute();
    virtual EL::StatusCode changeInput(bool firstFile);
    virtual EL::StatusCode initialize();
    virtual EL::StatusCode execute();
    virtual EL::StatusCode postExecute();
    virtual EL::StatusCode finalize();
    virtual EL::StatusCode histFinalize();

    //TTree *tree;
    std::vector<TLorentzVector> muonCorrectedFatJet;

    EL::StatusCode getHbbCorrectedVector(const xAOD::Jet &jet, TLorentzVector &correctedVector, const bool doVR);
    EL::StatusCode decorateWithMuons(const xAOD::Jet& jet, const bool doVR) const;
    const xAOD::JetFourMom_t getMuonCorrectedJetFourMom(const xAOD::Jet &jet, std::vector<const xAOD::Muon*> muons,
                                                        std::string scheme, bool useJMSScale = false) const;
 
    
    ClassDef(MuonInFatJetCorrection, 1);
};

#endif
