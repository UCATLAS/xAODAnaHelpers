#!/bin/bash
source /home/atlas/release_setup.sh
sleep 1
set -ex
# make a symbolic link for a top-level CMakeLists.txt file
ln -s /workarea/src/xAODAnaHelpers/ci/top_CMakeLists.txt /workarea/src/CMakeLists.txt
# make the build directory and start doing stuff
mkdir -p /workarea/build
cd /workarea/build
# cmake and make
time cmake ../src
time make
# printenv just to be sure
printenv
set +ex
