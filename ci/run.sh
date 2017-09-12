#!/bin/bash
source /home/atlas/release_setup.sh
set -ev
# make a symbolic link for a top-level CMakeLists.txt file
ln -s /home/atlas/workarea/src/xAODAnaHelpers/ci/CMakeLists.txt /home/atlas/workarea/src/CMakeLists.txt
mkdir -p /home/atlas/workarea/build
cd /home/atlas/workarea/build
cmake ../src
make
cd ../
printenv
source build/${AnalysisBase_PLATFORM}/setup.sh
python -c "import ROOT; print ROOT.xAH"
