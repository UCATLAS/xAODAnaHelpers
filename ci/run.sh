source /home/atlas/analysis_release_setup.sh
mkdir -p /workarea/build
cd /workarea/build
cmake ../src
make
cd ../
printenv
source build/${CMTCONFIG}/setup.sh
python -c "import ROOT; print ROOT.xAH"
