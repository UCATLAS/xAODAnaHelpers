### Install python packages

# for setuptools
find_package( pytools )

# Helper macro for building and installing the packages. Documentation
# to be written later...
function( setup_python_package name file md5 )

   # Parse the optional argument(s):
   cmake_parse_arguments( ARG "SINGLE_VERSION" ""
      "DEPENDS;EXTRA_ARGS;PATCH" ${ARGN} )

   # Extra argument(s) for the installation:
   if( ARG_SINGLE_VERSION )
      set( _extraArg )
   else()
      set( _extraArg --single-version-externally-managed )
   endif()

   if ( ARG_PATCH )
     set( _patchCmd PATCH_COMMAND patch -p1 < ${ARG_PATCH} )
   endif()

   # Build the package with the help of python's distutils:
   ExternalProject_Add( ${name}
      PREFIX ${CMAKE_BINARY_DIR}
      URL ${file}
      URL_MD5 ${md5}
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E echo
      "Configuring the build of ${name}"
      ${_patchCmd}
      BUILD_COMMAND ${CMAKE_BINARY_DIR}/atlas_build_run.sh
      python setup.py ${ARG_EXTRA_ARGS} build
      INSTALL_COMMAND ${CMAKE_COMMAND} -E make_directory
      ${CMAKE_PYTHON_OUTPUT_DIRECTORY}
      COMMAND ${CMAKE_COMMAND} -E make_directory
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND ${CMAKE_BINARY_DIR}/atlas_build_run.sh
      python setup.py ${ARG_EXTRA_ARGS} install
      --prefix ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}
      --exec-prefix ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}
      --root /
      --install-lib ${CMAKE_PYTHON_OUTPUT_DIRECTORY}
      --install-scripts ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      ${_extraArg} )

   # Make the package target depend on this one:
   add_dependencies( Package_xAODAnaHelpers ${name} )

   # Add possible extra dependencies:
   if( ARG_DEPENDS )
      add_dependencies( ${name} ${ARG_DEPENDS} )
   endif()

   # Get the package directory:
   atlas_get_package_dir( pkgDir )

   # Add some metadata to the target:
   set_property( TARGET ${name} PROPERTY LABEL xAODAnaHelpers )
   set_property( TARGET ${name} PROPERTY FOLDER ${pkgDir} )

   # Generate the package installer script:
   configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkgbuildInstall.cmake.in
      ${CMAKE_CURRENT_BINARY_DIR}/${name}PkgbuildInstall.cmake
      @ONLY )

   # Use this script for installing the package:
   install( SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/${name}PkgbuildInstall.cmake )

endfunction( setup_python_package )

# Clean up:
unset( setup_python_package )
