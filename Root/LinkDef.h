/* Algorithm Wrapper */
#include <xAODAnaHelpers/Algorithm.h>

/* Event and Jet Selectors */
#include <xAODAnaHelpers/BasicEventSelection.h>
#include <xAODAnaHelpers/ElectronSelector.h>
#include <xAODAnaHelpers/JetSelector.h>
#include <xAODAnaHelpers/TrackSelector.h>
#include <xAODAnaHelpers/MuonSelector.h>

/* Calibrations */
#include <xAODAnaHelpers/ElectronCalibrator.h>
#include <xAODAnaHelpers/JetCalibrator.h>
#include <xAODAnaHelpers/MuonCalibrator.h>
/*#include <xAODAnaHelpers/GroomedFatJets.h>*/

/* Missing Energy Reconstruction */
#include <xAODAnaHelpers/METConstructor.h>

/* Scale Factors */
#include <xAODAnaHelpers/ElectronEfficiencyCorrector.h>
#include <xAODAnaHelpers/MuonEfficiencyCorrector.h>
#include <xAODAnaHelpers/BJetEfficiencyCorrector.h>

/* Plotting Tools */
#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/TrackHistsAlgo.h>

/* Tree */
#include <xAODAnaHelpers/TreeAlgo.h>


/* Other */
#include <xAODAnaHelpers/OverlapRemover.h>
#include <xAODAnaHelpers/Writer.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#pragma link C++ class xAH::AlgorithmRegistry+;
#pragma link C++ class xAH::Algorithm+;

#pragma link C++ class BasicEventSelection+;
#pragma link C++ class ElectronSelector+;
#pragma link C++ class JetSelector+;
#pragma link C++ class TrackSelector+;
#pragma link C++ class MuonSelector+;

#pragma link C++ class ElectronCalibrator+;
#pragma link C++ class JetCalibrator+;
#pragma link C++ class MuonCalibrator+;
/*#pragma link C++ class GroomedFatJets+;*/

#pragma link C++ class METConstructor+;

#pragma link C++ class ElectronEfficiencyCorrector+;
#pragma link C++ class MuonEfficiencyCorrector+;
#pragma link C++ class BJetEfficiencyCorrector+;

#pragma link C++ class JetHistsAlgo+;
#pragma link C++ class TrackHistsAlgo+;

#pragma link C++ class TreeAlgo+;

#pragma link C++ class OverlapRemover+;
#pragma link C++ class Writer+;
#endif
