#!/bin/bash
source /home/atlas/release_setup.sh
set -ev
python -c "import ROOT; print ROOT.xAH"
python -c "import xAODAnaHelpers"
