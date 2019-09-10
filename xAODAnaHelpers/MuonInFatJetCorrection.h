#include <xAODAnaHelpers/Algorithm.h>
//#include <EventLoop/Algorithm.h>
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include "MuonMomentumCorrections/MuonCalibrationPeriodTool.h"


class MuonInJetCorrection : public xAH::Algorithm {

public:

  MuonInJetCorrection(const std::string &name, ConfigStore &config, xAOD::TEvent *event,
                EventInfoHandler & eventInfoHandler);

  ~MuonInJetCorrection();

  EL::StatusCode getHbbCorrectedVector(const xAOD::Jet &jet, TLorentzVector &correctedVector, const bool doVR);
  EL::StatusCode decorateWithMuons(const xAOD::Jet& jet, const bool doVR) const;
  const xAOD::JetFourMom_t getMuonCorrectedJetFourMom(const xAOD::Jet &jet, std::vector<const xAOD::Muon*> muons,
                                                      std::string scheme, bool useJMSScale = false) const;
  CP::MuonSelectionTool *m_muonSelectionTool;
  CP::MuonCalibrationPeriodTool *m_muonCalibrationPeriodTool;
  float m_trackJetPtMin;
  float m_trackJetEtaMax;
  float m_trackJetNConst;
  float m_muonPtMin;
  float m_muonEtaMax;
  float m_muonDrMax;

};

#endif

