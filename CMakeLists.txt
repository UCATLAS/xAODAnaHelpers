################################################################################
# Package: xAODAnaHelpers
################################################################################

# Declare the package name:
atlas_subdir( xAODAnaHelpers )

set( release_libs xAODTriggerCnvLib )
if( ${AnalysisBase_VERSION} VERSION_GREATER 21.0 )
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
                          Reconstruction/MET/METInterface
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
atlas_install_python_modules( python/*.py )
atlas_install_scripts( scripts/*.py )
atlas_install_data( data/* )


### Install python packages
find_package( PythonInterp 2.7 REQUIRED )

# Tell the user what's happening:
message( STATUS "Building python modules for xAODAnaHelpers as part of this project" )

# Build/install argcomplete:
set( _source
    "https://pypi.python.org/packages/3b/db/c524f0d72842b44b179cc50d4257f1e72f447fef0919556e8b28a9b0f80f/argcomplete-1.9.2.tar.gz" )
set( _md5 "13243e0b88102f13f7537f5aa70a48c0" )
set( _buildDir
   ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/argcompleteBuild  )
set( _sitePkgsDir ${_buildDir}/lib/python2.7/site-packages )
ExternalProject_Add( argcomplete
   PREFIX ${CMAKE_BINARY_DIR}
   INSTALL_DIR ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}
   URL ${_source}
   URL_MD5 ${_md5}
   BUILD_IN_SOURCE 1
   CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory ${_sitePkgsDir}
   BUILD_COMMAND ${CMAKE_BINARY_DIR}/atlas_build_run.sh
   ${PYTHON_EXECUTABLE} <SOURCE_DIR>/setup.py build
   INSTALL_COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${_sitePkgsDir}
   ${CMAKE_BINARY_DIR}/atlas_build_run.sh
   ${PYTHON_EXECUTABLE} <SOURCE_DIR>/setup.py install --prefix=${_buildDir}
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${_buildDir}/bin <INSTALL_DIR>/bin
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${_sitePkgsDir}/argcomplete-1.9.2-py2.7.egg/argcomplete <INSTALL_DIR>/python/argcomplete
   COMMAND echo <SOURCE_DIR>
   COMMAND echo <INSTALL_DIR>
   COMMAND echo ${_buildDir}
   COMMAND echo ${_sitePkgsDir}
   )
add_dependencies( Package_xAODAnaHelpers argcomplete )
install( DIRECTORY ${_buildDir}/
   DESTINATION . USE_SOURCE_PERMISSIONS OPTIONAL )
