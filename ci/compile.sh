#!/bin/bash
source /home/atlas/release_setup.sh
set -ev
# set tmpdir in a writeable location
mkdir -p /workarea/tmp/
export TMPDIR=/workarea/tmp/
# make a symbolic link for a top-level CMakeLists.txt file
ln -s /workarea/src/xAODAnaHelpers/ci/top_CMakeLists.txt /workarea/src/CMakeLists.txt
# make the build directory and start doing stuff
mkdir -p /workarea/build
cd /workarea/build
time cmake ../src
time make
time cpack
cd ../
printenv
