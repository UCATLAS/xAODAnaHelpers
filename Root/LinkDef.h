#include <xAODAnaHelpers/ContainerRecording.h>

/* Event and Jet Selectors */
#include <xAODAnaHelpers/BasicEventSelection.h>
#include <xAODAnaHelpers/BJetSelector.h>
#include <xAODAnaHelpers/ElectronSelector.h>
#include <xAODAnaHelpers/JetSelector.h>
#include <xAODAnaHelpers/TrackSelector.h>
#include <xAODAnaHelpers/MuonSelector.h>

/* Calibrations */
#include <xAODAnaHelpers/ElectronCalibrator.h>
#include <xAODAnaHelpers/JetCalibrator.h>
#include <xAODAnaHelpers/MuonCalibrator.h>
/*#include <xAODAnaHelpers/GroomedFatJets.h>*/

/* Plotting Tools */
#include <xAODAnaHelpers/JetHistsAlgo.h>
#include <xAODAnaHelpers/TrackHistsAlgo.h>


/* Other */
#include <xAODAnaHelpers/JERShifter.h>
#include <xAODAnaHelpers/OverlapRemover.h>
#include <xAODAnaHelpers/Writer.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#pragma link C++ class BasicEventSelection+;
#pragma link C++ class BJetSelector+;
#pragma link C++ class ElectronSelector+;
#pragma link C++ class JetSelector+;
#pragma link C++ class TrackSelector+;
#pragma link C++ class MuonSelector+;

#pragma link C++ class ElectronCalibrator+;
#pragma link C++ class JetCalibrator+;
#pragma link C++ class MuonCalibrator+;
/*#pragma link C++ class GroomedFatJets+;*/

#pragma link C++ class JetHistsAlgo+;
#pragma link C++ class TrackHistsAlgo+;

#pragma link C++ class JERShifter+;
#pragma link C++ class OverlapRemover+;
#pragma link C++ class Writer+;
#endif

#ifdef __CINT__
#pragma link C++ class ContainerRecording+;
#endif
