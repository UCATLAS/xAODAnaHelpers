#include <GaudiKernel/DeclareFactoryEntries.h>

/* Algorithm Wrapper */
#include <xAODAnaHelpers/Algorithm.h>

/* Event and Jet Selectors */
#include <xAODAnaHelpers/BasicEventSelection.h>
#include <xAODAnaHelpers/ElectronSelector.h>
#include <xAODAnaHelpers/PhotonSelector.h>
#include <xAODAnaHelpers/TauSelector.h>
#include <xAODAnaHelpers/JetSelector.h>
#include <xAODAnaHelpers/DebugTool.h>
#include <xAODAnaHelpers/TruthSelector.h>
#include <xAODAnaHelpers/TrackSelector.h>
#include <xAODAnaHelpers/MuonSelector.h>

/* Calibrations */
#include <xAODAnaHelpers/ElectronCalibrator.h>
#include <xAODAnaHelpers/PhotonCalibrator.h>
#include <xAODAnaHelpers/JetCalibrator.h>
#include <xAODAnaHelpers/MuonCalibrator.h>
#include <xAODAnaHelpers/TauCalibrator.h>
/*#include <xAODAnaHelpers/GroomedFatJets.h>*/
#include <xAODAnaHelpers/HLTJetRoIBuilder.h>
#include <xAODAnaHelpers/HLTJetGetter.h>

/* Missing Energy Reconstruction */
#include <xAODAnaHelpers/METConstructor.h>

/* Scale Factors */
#include <xAODAnaHelpers/ElectronEfficiencyCorrector.h>
#include <xAODAnaHelpers/MuonEfficiencyCorrector.h>
#include <xAODAnaHelpers/TauEfficiencyCorrector.h>
#include <xAODAnaHelpers/BJetEfficiencyCorrector.h>

/* Plotting Tools */
#include <xAODAnaHelpers/IParticleHistsAlgo.h>
#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/MuonHistsAlgo.h>
#include <xAODAnaHelpers/PhotonHistsAlgo.h>
#include <xAODAnaHelpers/ElectronHistsAlgo.h>
#include <xAODAnaHelpers/MetHistsAlgo.h>
#include <xAODAnaHelpers/TrackHistsAlgo.h>
#include <xAODAnaHelpers/ClusterHistsAlgo.h>

/* Tree */
#include <xAODAnaHelpers/TreeAlgo.h>

/* Other */
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/OverlapRemover.h>
#include <xAODAnaHelpers/TrigMatcher.h>
#include <xAODAnaHelpers/TauJetMatching.h>
#include <xAODAnaHelpers/MessagePrinterAlgo.h>
#include <xAODAnaHelpers/MuonInFatJetCorrector.h>

// add all the declares

DECLARE_ALGORITHM_FACTORY (Algorithm)

DECLARE_ALGORITHM_FACTORY (BasicEventSelection)
DECLARE_ALGORITHM_FACTORY (ElectronSelector)
DECLARE_ALGORITHM_FACTORY (PhotonSelector)
DECLARE_ALGORITHM_FACTORY (TauSelector)
DECLARE_ALGORITHM_FACTORY (JetSelector)
DECLARE_ALGORITHM_FACTORY (DebugTool)
DECLARE_ALGORITHM_FACTORY (TruthSelector)
DECLARE_ALGORITHM_FACTORY (TrackSelector)
DECLARE_ALGORITHM_FACTORY (MuonSelector)

DECLARE_ALGORITHM_FACTORY (ElectronCalibrator)
DECLARE_ALGORITHM_FACTORY (PhotonCalibrator)
DECLARE_ALGORITHM_FACTORY (JetCalibrator)
DECLARE_ALGORITHM_FACTORY (MuonCalibrator)
DECLARE_ALGORITHM_FACTORY (TauCalibrator)
//DECLARE_ALGORITHM_FACTORY (GroomedFatJets)
DECLARE_ALGORITHM_FACTORY (HLTJetRoIBuilder)
DECLARE_ALGORITHM_FACTORY (HLTJetGetter)

DECLARE_ALGORITHM_FACTORY (METConstructor)

DECLARE_ALGORITHM_FACTORY (ElectronEfficiencyCorrector)
DECLARE_ALGORITHM_FACTORY (MuonEfficiencyCorrector)
DECLARE_ALGORITHM_FACTORY (TauEfficiencyCorrector)
DECLARE_ALGORITHM_FACTORY (BJetEfficiencyCorrector)

DECLARE_ALGORITHM_FACTORY (IParticleHistsAlgo)
DECLARE_ALGORITHM_FACTORY (JetHistsAlgo)
DECLARE_ALGORITHM_FACTORY (MuonHistsAlgo)
DECLARE_ALGORITHM_FACTORY (PhotonHistsAlgo)
DECLARE_ALGORITHM_FACTORY (ElectronHistsAlgo)
DECLARE_ALGORITHM_FACTORY (MetHistsAlgo)
DECLARE_ALGORITHM_FACTORY (TrackHistsAlgo)
DECLARE_ALGORITHM_FACTORY (ClusterHistsAlgo)

DECLARE_ALGORITHM_FACTORY (TreeAlgo)

DECLARE_ALGORITHM_FACTORY (HelperFunctions)
DECLARE_ALGORITHM_FACTORY (OverlapRemover)
DECLARE_ALGORITHM_FACTORY (TrigMatcher)
DECLARE_ALGORITHM_FACTORY (TauJetMatching)
DECLARE_ALGORITHM_FACTORY (MessagePrinterAlgo)
DECLARE_ALGORITHM_FACTORY (MuonInFatJetCorrector)
