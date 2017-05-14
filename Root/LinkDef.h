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
/*#include <xAODAnaHelpers/GroomedFatJets.h>*/
#include <xAODAnaHelpers/HLTJetRoIBuilder.h>
#include <xAODAnaHelpers/HLTJetGetter.h>

/* Missing Energy Reconstruction */
#include <xAODAnaHelpers/METConstructor.h>

/* Scale Factors */
#include <xAODAnaHelpers/ElectronEfficiencyCorrector.h>
#include <xAODAnaHelpers/MuonEfficiencyCorrector.h>
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

/* Mini xAOD */
#include <xAODAnaHelpers/MinixAOD.h>

/* Other */
#include <xAODAnaHelpers/OverlapRemover.h>
#include <xAODAnaHelpers/TrigMatcher.h>
#include <xAODAnaHelpers/Writer.h>
#include <xAODAnaHelpers/MessagePrinterAlgo.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#pragma link C++ class xAH::Algorithm+;

#pragma link C++ class BasicEventSelection+;
#pragma link C++ class ElectronSelector+;
#pragma link C++ class PhotonSelector+;
#pragma link C++ class TauSelector+;
#pragma link C++ class JetSelector+;
#pragma link C++ class DebugTool+;
#pragma link C++ class TruthSelector+;
#pragma link C++ class TrackSelector+;
#pragma link C++ class MuonSelector+;

#pragma link C++ class ElectronCalibrator+;
#pragma link C++ class PhotonCalibrator+;
#pragma link C++ class JetCalibrator+;
#pragma link C++ class HLTJetRoIBuilder+;
#pragma link C++ class MuonCalibrator+;
#pragma link C++ class HLTJetGetter+;
/*#pragma link C++ class GroomedFatJets+;*/

#pragma link C++ class METConstructor+;

#pragma link C++ class ElectronEfficiencyCorrector+;
#pragma link C++ class MuonEfficiencyCorrector+;
#pragma link C++ class BJetEfficiencyCorrector+;

#pragma link C++ class IParticleHistsAlgo+;
#pragma link C++ class JetHistsAlgo+;
#pragma link C++ class MuonHistsAlgo+;
#pragma link C++ class PhotonHistsAlgo+;
#pragma link C++ class ElectronHistsAlgo+;
#pragma link C++ class MetHistsAlgo+;
#pragma link C++ class TrackHistsAlgo+;
#pragma link C++ class ClusterHistsAlgo+;

#pragma link C++ class TreeAlgo+;

#pragma link C++ class MinixAOD+;

#pragma link C++ class OverlapRemover+;
#pragma link C++ class TrigMatcher+;
#pragma link C++ class Writer+;
#pragma link C++ class MessagePrinterAlgo+;
#endif
