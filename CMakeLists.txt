################################################################################
# Package: xAODAnaHelpers
################################################################################

# Declare the package name:
atlas_subdir( xAODAnaHelpers )

set( release_libs xAODTriggerCnvLib )

if( AnalysisBase_VERSION AND AnalysisBase_VERSION VERSION_GREATER 21.0 )
  # 21.X
  set( release_libs xAODTriggerCnvLib )
elseif( AnalysisTop_VERSION AND AnalysisTop_VERSION VERSION_GREATER 21.0 )
  # AnalysisTop 21.X
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
                          PhysicsAnalysis/Interfaces/FTagAnalysisInterfaces
                          PhysicsAnalysis/Interfaces/JetAnalysisInterfaces
                          PhysicsAnalysis/Interfaces/MuonAnalysisInterfaces
                          PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonMomentumCorrections
                          PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections
                          PhysicsAnalysis/TauID/TauAnalysisTools
                          Reconstruction/Jet/JetAnalysisTools/JetTileCorrection
                          Reconstruction/Jet/JetCalibTools
                          Reconstruction/Jet/JetJvtEfficiency
                          Reconstruction/Jet/JetMomentTools
                          Reconstruction/Jet/JetResolution
                          Reconstruction/Jet/JetSubStructureUtils
                          Reconstruction/Jet/JetUncertainties
                          Reconstruction/MET/METInterface
                          Reconstruction/MET/METUtilities
                          Tools/PathResolver
                          Trigger/TrigAnalysis/TrigDecisionTool
                          Trigger/TrigAnalysis/TriggerMatchingTool
                          Trigger/TrigConfiguration/TrigConfxAOD
                          ${release_depends}
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
                   FTagAnalysisInterfacesLib JetAnalysisInterfacesLib MuonAnalysisInterfacesLib
                   PhotonEfficiencyCorrectionLib METUtilitiesLib METInterface
                   TauAnalysisToolsLib AsgTools xAODMissingET JetResolutionLib
                   AssociationUtilsLib JetEDM JetUncertaintiesLib
                   JetCPInterfaces xAODBTaggingEfficiencyLib TrigConfxAODLib
                   TrigDecisionToolLib xAODCutFlow JetMomentToolsLib
                   TriggerMatchingToolLib xAODMetaDataCnv xAODMetaData
                   JetJvtEfficiencyLib PMGToolsLib JetSubStructureUtils JetTileCorrectionLib BoostedJetTaggersLib
                   ${release_libs}
)

# Install files from the package:
atlas_install_python_modules( python/*.py )
atlas_install_scripts( scripts/*.py )
atlas_install_data( data/* )


### Install python packages
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(SetupPythonPackage)

# Install argcomplete:
setup_python_package( argcomplete
   "https://pypi.python.org/packages/3b/db/c524f0d72842b44b179cc50d4257f1e72f447fef0919556e8b28a9b0f80f/argcomplete-1.9.2.tar.gz"
   "13243e0b88102f13f7537f5aa70a48c0"
   PATCH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/patch-setup.cfg setup.cfg
)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/xAH_env_copied
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/cmake/setup.sh >> ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/setup.sh
                   COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/xAH_env_copied
)
add_custom_target(Setup_xAH_Environment ALL DEPENDS ${CMAKE_BINARY_DIR}/xAH_env_copied)
execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/xAH_env_copied)
