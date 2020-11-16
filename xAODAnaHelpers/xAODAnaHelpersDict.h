#ifndef XAODANAHELPERS_XAODANAHELPERS_DICT_H
#define XAODANAHELPERS_XAODANAHELPERS_DICT_H

// This file includes all the header files that you need to create
// dictionaries for.

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
#include <xAODAnaHelpers/Writer.h>
#include <xAODAnaHelpers/MessagePrinterAlgo.h>
#include <xAODAnaHelpers/MuonInFatJetCorrector.h>

#endif
