#!/bin/bash
set -ev
source /home/atlas/analysis_release_setup.sh
mkdir -p /workarea/build
cd /workarea/build
# make a symbolic link for a top-level CMakeLists.txt file
ln -s /workarea/src/xAODAnaHelpers/ci/CMakeLists.txt /workarea/src/CMakeLists.txt
cmake ../src
make
cd ../
printenv
source build/${CMTCONFIG}/setup.sh
python -c "import ROOT; print ROOT.xAH"
