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

dict_pkg = {'2.1.29': ["ElectronEfficiencyCorrection"],
            '2.1.30': ["atlasoff/Reconstruction/egamma/egammaMVACalib/tags/egammaMVACalib-01-00-43",
                       "atlasoff/Control/xAODRootAccess/tags/xAODRootAccess-00-01-04",
                       "ElectronEfficiencyCorrection"],
            '2.1.31': ["atlasoff/Reconstruction/egamma/egammaMVACalib/tags/egammaMVACalib-01-00-43",
                       "atlasoff/Control/xAODRootAccess/tags/xAODRootAccess-00-01-04",
                       "atlasoff/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-37",
                       "ElectronEfficiencyCorrection"],
            '2.1.32': ["atlasoff/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-37",
                       "ElectronEfficiencyCorrection"],
            '2.3.12': ["atlasoff/PhysicsAnalysis/ElectronPhotonID/ElectronEfficiencyCorrection/tags/ElectronEfficiencyCorrection-00-01-20",
                       "atlasoff/PhysicsAnalysis/MuonID/MuonSelectorTools/tags/MuonSelectorTools-00-05-10",
                       "atlasoff/PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections/tags/MuonEfficiencyCorrections-03-00-19",
                       "atlasoff/PhysicsAnalysis/D3PDTools/EventLoop/tags/EventLoop-00-01-09",
                       "atlasoff/Reconstruction/MET/METUtilities/tags/METUtilities-00-01-39"],
            '2.3.13': ["atlasoff/Reconstruction/Jet/JetCalibTools/tags/JetCalibTools-00-04-41",
                       "atlasoff/Reconstruction/MET/METUtilities/tags/METUtilities-00-01-39"]
           }

try:
  packages_to_checkout = dict_pkg[args.version]
except KeyError:
  print "That version isn't supported! If this is a problem, tell someone important."
  import sys
  sys.exit(0)

for pkg in packages_to_checkout:
  print "checking out package: {0}".format(pkg)
  subprocess.Popen(['cd $ROOTCOREBIN/.. && pwd && rc checkout_pkg {0}'.format(pkg) ], env=rc_env, shell=True).wait()

packages_to_patch = ["ElectronEfficiencyCorrection"]

print "applying svn patches..."
for pkg in packages_to_patch:
  if pkg in packages_to_checkout:
    print "  patching {0}".format(pkg)
    subprocess.Popen(['cd $ROOTCOREBIN/../{0} && pwd && patch -p0 -i $ROOTCOREBIN/../xAODAnaHelpers/data/{0}_Base.{1}.diff && cd - && pwd'.format(pkg, args.version)], env=rc_env, shell=True ).wait()
  else:
    print "  no patches to be applied!"


