#!/bin/bash
source /home/atlas/release_setup.sh
set -ev
# make a symbolic link for a top-level CMakeLists.txt file
ln -s /workarea/src/xAODAnaHelpers/ci/top_CMakeLists.txt /workarea/src/CMakeLists.txt
# make the build directory and start doing stuff
mkdir -p /workarea/build
cd /workarea/build
cmake ../src
make
cd ../
printenv
source build/${AnalysisBase_PLATFORM}/setup.sh
python -c "import ROOT; print ROOT.xAH"