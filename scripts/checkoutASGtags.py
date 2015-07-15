#!/usr/bin/python
#
# *******************************************************************************
# script to checkout specific ASG packages for a given release,
# and apply svn patches if needed
#
# Authors:
#  M.Milesi(mrco.milesi@cern.ch), G.Stark(g.stark@cern.ch)
#
# Usage (from the directory where ASG release has been set up):
#  python () (./xAODAnaHelpers/scripts/)checkoutASGtags.py RELEASE_NUMBER [X.Y.Z]
# *******************************************************************************

import subprocess
import argparse
import os

rc_env = os.environ.copy()

parser = argparse.ArgumentParser(description='Checkout packages and apply patches!', usage='%(prog)s version')
parser.add_argument('version', help='the ASG release you have set up')
args = parser.parse_args()

print "using ASG version {0}".format(args.version)

dict_pkg = {'2.3.15': ["atlasoff/Reconstruction/Jet/JetUncertainties/tags/JetUncertainties-00-09-27",
                       "atlasoff/PhysicsAnalysis/JetMissingEtID/JetSelectorTools/tags/JetSelectorTools-00-05-02",
                       "atlasoff/Reconstruction/Jet/JetMomentTools/tags/JetMomentTools-00-03-20",
                       "atlasoff/PhysicsAnalysis/AnalysisCommon/PileupReweighting/tags/PileupReweighting-00-03-03",
                       "atlasoff/PhysicsAnalysis/AnalysisCommon/IsolationSelection/tags/IsolationSelection-00-00-05",
                       "atlasoff/Event/xAOD/xAODTracking/tags/xAODTracking-00-13-15"],
            '2.3.18': ["atlasoff/Reconstruction/Jet/JetUncertainties/tags/JetUncertainties-00-09-28",
                       "atlasoff/Reconstruction/Jet/JetResolution/tags/JetResolution-03-00-36",
                       "atlasoff/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-46",
                       "atlasoff/Trigger/TrigAnalysis/TrigEgammaMatchingTool/tags/TrigEgammaMatchingTool-00-00-05",
		                   "atlasoff/PhysicsAnalysis/AnalysisCommon/IsolationSelection/tags/IsolationSelection-00-00-10"],
            '2.3.19': ["atlasoff/Trigger/TrigAnalysis/TrigEgammaMatchingTool/tags/TrigEgammaMatchingTool-00-00-05",
		                   "atlasoff/PhysicsAnalysis/AnalysisCommon/IsolationSelection/tags/IsolationSelection-00-00-10",
                       "atlasoff/Reconstruction/Jet/JetUncertainties/tags/JetUncertainties-00-09-29"]
           }

try:
  packages_to_checkout = dict_pkg[args.version]
except KeyError:
  print "Warning: that version isn't supported! This may not be a problem if you're using a new ASG release."
  import sys
  sys.exit(0)

for pkg in packages_to_checkout:
  print "checking out package: {0}".format(pkg)
  subprocess.Popen(['cd $ROOTCOREBIN/.. && pwd && rc checkout_pkg {0}'.format(pkg) ], env=rc_env, shell=True).wait()

packages_to_patch = []

print "applying svn patches..."
for pkg in packages_to_patch:
  if pkg in packages_to_checkout:
    print "  patching {0}".format(pkg)
    subprocess.Popen(['cd $ROOTCOREBIN/../{0} && pwd && patch -p0 -i $ROOTCOREBIN/../xAODAnaHelpers/data/{0}_Base.{1}.diff && cd - && pwd'.format(pkg, args.version)], env=rc_env, shell=True ).wait()
  else:
    print "  no patches to be applied!"


