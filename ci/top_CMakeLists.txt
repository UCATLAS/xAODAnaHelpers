# Project build configuration.

# Set the minimum required CMake version.
cmake_minimum_required( VERSION 3.4 FATAL_ERROR )

# Look for any AnalysisBase-21.2.X release:
find_package( AnalysisBase 21.2 REQUIRED )

# Set up the usage of CTest:
atlas_ctest_setup()

# Set up the project:
atlas_project( WorkDir 1.0.0
   USE AnalysisBase ${AnalysisBase_VERSION} )

# Generate an environment setup script:
lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh
   DESTINATION . )

# Set up the usage of CPack:
atlas_cpack_setup()
