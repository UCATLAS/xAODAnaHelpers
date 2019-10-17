#ifndef xAODAnaHelpers_MuonInFatJetCorrection_H
#define xAODAnaHelpers_MuonInFatJetCorrection_H

#include <xAODAnaHelpers/Algorithm.h>
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include "MuonMomentumCorrections/MuonCalibrationPeriodTool.h"

#include "JetCalibTools/JetCalibrationTool.h"

/** @rst
    Algorithm for correcting the momentum of largeR jets containing muon decays.

    Only muons associated to track jets are used. Quality and kinematic cuts for the muons and track jets can be adjusted by the user.

    There are currently four correction schemes; Calorimeter, TrackAssisted, Combined, and SimpleMuon. At present, Combined is used, which takes a weighted sum of corrections from both the TrackAssisted and Calorimeter Schemes.

    The corrected large-R are saved as a TLorentzVector in a decorator named "correctedFatJets_tlv".

    @endrst */
class MuonInFatJetCorrection : public xAH::Algorithm
{
public:
  /// @brief Different schemes for the muon in jet correction
  enum Scheme {Calorimeter, TrackAssisted, Combined, SimpleMuon};
  
  MuonInFatJetCorrection();

  /// @brief The name of the container with fat jets to be corrected
  std::string m_fatJetContainerName = "";
  /// @brief The name of the container with muons to be used for the correction
  std::string m_muonContainerName = "";
  /// @brief The name of the track jet container to match with muons
  std::string m_trackJetContainerName = "GhostVR30Rmax4Rmin02TrackJet";
  /// @brief Algortihm systematics loop
  std::string m_inputAlgo;

  /// @brief Minimum pt of track jets to use for correction
  float m_trackJetPtMin = 10000.0;
  /// @brief Maximum eta of track jets to use for correction
  float m_trackJetEtaMax = 2.5;
  /// @brief Minimum number of constituents (tracks) of track jets to use for correction
  float m_trackJetNConst = 2.0;

  /// @brief Minimum pt of muons to use for correction
  float m_muonPtMin = 10000.0;
  /// @brief Maximum eta of muons to use for correction
  float m_muonEtaMax = 2.7;

  /// @brief DR cut to use when matching muons to track jets
  float m_muonDrMax = 0.4;

  virtual EL::StatusCode setupJob(EL::Job& job);
  virtual EL::StatusCode histInitialize();
  virtual EL::StatusCode fileExecute();
  virtual EL::StatusCode changeInput(bool firstFile);
  virtual EL::StatusCode initialize();
  virtual EL::StatusCode execute();
  virtual EL::StatusCode postExecute();
  virtual EL::StatusCode finalize();
  virtual EL::StatusCode histFinalize();

  TLorentzVector getHbbCorrectedVector(const xAOD::Jet &jet);
  EL::StatusCode decorateWithMuons(const xAOD::Jet& jet) const;
  const xAOD::JetFourMom_t getMuonCorrectedJetFourMom(const xAOD::Jet &jet, std::vector<const xAOD::Muon*> muons,
						      Scheme scheme, bool useJMSScale = false) const;


  ClassDef(MuonInFatJetCorrection, 1);
};

#endif
