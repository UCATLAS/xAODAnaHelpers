################################################################################
# Package: xAODAnaHelpers
################################################################################

# Declare the package name:
atlas_subdir( xAODAnaHelpers )

set( release_libs )
if( ${AtlasVersion} GREATER_EQUAL 21 )
  # 21.X
  set( release_libs xAODTriggerCnvLib )
else()
  # 2.6.X
  set( release_libs xAODTriggerCnv )
endif()

# Declare the package's dependencies:
atlas_depends_on_subdirs( PUBLIC
                          Control/AthContainers
                          Control/AthToolSupport/AsgTools
                          Control/xAODRootAccess
                          DataQuality/GoodRunsLists
                          Event/xAOD/xAODBase
                          Event/xAOD/xAODCaloEvent
                          Event/xAOD/xAODCutFlow
                          Event/xAOD/xAODEgamma
                          Event/xAOD/xAODEventInfo
                          Event/xAOD/xAODJet
                          Event/xAOD/xAODMetaDataCnv
                          Event/xAOD/xAODMissingET
                          Event/xAOD/xAODMuon
                          Event/xAOD/xAODTau
                          Event/xAOD/xAODTracking
                          Event/xAOD/xAODTrigger
                          Event/xAOD/xAODTriggerCnv
                          Event/xAOD/xAODTruth
                          PhysicsAnalysis/D3PDTools/EventLoop
                          PhysicsAnalysis/D3PDTools/EventLoopGrid
                          InnerDetector/InDetRecTools/InDetTrackSelectionTool
                          PhysicsAnalysis/AnalysisCommon/AssociationUtils
                          PhysicsAnalysis/AnalysisCommon/IsolationSelection
                          PhysicsAnalysis/AnalysisCommon/PATInterfaces
                          PhysicsAnalysis/AnalysisCommon/PMGTools
                          PhysicsAnalysis/AnalysisCommon/PileupReweighting
                          PhysicsAnalysis/D3PDTools/RootCoreUtils
                          PhysicsAnalysis/D3PDTools/SampleHandler
                          PhysicsAnalysis/ElectronPhotonID/ElectronEfficiencyCorrection
                          PhysicsAnalysis/ElectronPhotonID/ElectronPhotonFourMomentumCorrection
                          PhysicsAnalysis/ElectronPhotonID/ElectronPhotonSelectorTools
                          PhysicsAnalysis/ElectronPhotonID/IsolationCorrections
                          PhysicsAnalysis/ElectronPhotonID/ElectronPhotonShowerShapeFudgeTool
                          PhysicsAnalysis/ElectronPhotonID/PhotonEfficiencyCorrection
                          PhysicsAnalysis/JetMissingEtID/JetSelectorTools
                          PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/xAODBTaggingEfficiency
                          PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonMomentumCorrections
                          PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections
                          PhysicsAnalysis/MuonID/MuonSelectorTools
                          PhysicsAnalysis/TauID/TauAnalysisTools
                          Reconstruction/Jet/JetAnalysisTools/JetTileCorrection
                          Reconstruction/Jet/JetCalibTools
                          Reconstruction/Jet/JetJvtEfficiency
                          Reconstruction/Jet/JetMomentTools
                          Reconstruction/Jet/JetResolution
                          Reconstruction/Jet/JetSubStructureUtils
                          Reconstruction/Jet/JetUncertainties
                          Reconstruction/MET/METUtilities
                          Tools/PathResolver
                          Trigger/TrigAnalysis/TrigDecisionTool
                          Trigger/TrigAnalysis/TriggerMatchingTool
                          Trigger/TrigConfiguration/TrigConfxAOD
)

# Find the needed external(s):
find_package( ROOT COMPONENTS Core RIO Hist Tree )

# build a dictionary for the library
atlas_add_root_dictionary ( xAODAnaHelpersLib xAODAnaHelpersDictSource
                            ROOT_HEADERS xAODAnaHelpers/*.h Root/LinkDef.h
                            EXTERNAL_PACKAGES ROOT
)

# build a shared library
atlas_add_library( xAODAnaHelpersLib xAODAnaHelpers/*.h Root/*.h Root/*.cxx ${xAODAnaHelpersDictSource}
                   PUBLIC_HEADERS xAODAnaHelpers
                   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
                   LINK_LIBRARIES ${ROOT_LIBRARIES} EventLoop xAODBase xAODRootAccess
                   xAODEventInfo GoodRunsListsLib PileupReweightingLib PATInterfaces
                   PathResolver xAODTau xAODJet xAODMuon xAODEgamma
                   xAODTracking xAODTruth MuonMomentumCorrectionsLib
                   MuonEfficiencyCorrectionsLib MuonSelectorToolsLib JetCalibToolsLib
                   JetSelectorToolsLib AthContainers
                   ElectronPhotonFourMomentumCorrectionLib
                   ElectronEfficiencyCorrectionLib ElectronPhotonSelectorToolsLib
                   IsolationSelectionLib IsolationCorrectionsLib
                   ElectronPhotonShowerShapeFudgeToolLib
                   PhotonEfficiencyCorrectionLib METUtilitiesLib METInterface
                   TauAnalysisToolsLib AsgTools xAODMissingET JetResolutionLib
                   AssociationUtilsLib JetEDM JetUncertaintiesLib
                   JetCPInterfaces xAODBTaggingEfficiencyLib TrigConfxAODLib
                   TrigDecisionToolLib xAODCutFlow JetMomentToolsLib
                   TriggerMatchingToolLib xAODMetaDataCnv xAODMetaData
                   JetJvtEfficiencyLib PMGToolsLib JetSubStructureUtils JetTileCorrectionLib
                   ${release_libs}
)

# Install files from the package:
atlas_install_scripts( scripts/*.py )
atlas_install_data( data/* )
