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
                   EGammaVariableCorrectionLib
                   FTagAnalysisInterfacesLib JetAnalysisInterfacesLib MuonAnalysisInterfacesLib
                   PhotonEfficiencyCorrectionLib METUtilitiesLib METInterface
                   TauAnalysisToolsLib AsgTools xAODMissingET
                   AssociationUtilsLib JetEDM JetUncertaintiesLib
                   JetCPInterfaces xAODBTagging xAODBTaggingEfficiencyLib TrigConfxAODLib
                   TrigDecisionToolLib xAODCutFlow JetMomentToolsLib
                   TriggerMatchingToolLib xAODMetaDataCnv xAODMetaData
                   JetJvtEfficiencyLib PMGToolsLib SystematicsHandlesLib
                   JetSubStructureUtils JetTileCorrectionLib BoostedJetTaggersLib
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
# setup_python_package( argcomplete
#    "https://pypi.python.org/packages/3b/db/c524f0d72842b44b179cc50d4257f1e72f447fef0919556e8b28a9b0f80f/argcomplete-1.9.2.tar.gz"
#    "13243e0b88102f13f7537f5aa70a48c0"
#    PATCH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/patch-setup.cfg setup.cfg
# )

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/xAH_env_copied
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/cmake/setup.sh >> ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/setup.sh
                   COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/xAH_env_copied
)
add_custom_target(Setup_xAH_Environment ALL DEPENDS ${CMAKE_BINARY_DIR}/xAH_env_copied)
execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/xAH_env_copied)
