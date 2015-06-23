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

dict_pkg = {'2.3.14': ["atlasoff/Reconstruction/Jet/JetUncertainties/tags/JetUncertainties-00-09-25",
                       "atlasoff/Reconstruction/Jet/JetMomentTools/tags/JetMomentTools-00-03-20"],
            '2.3.15': ["atlasoff/Reconstruction/Jet/JetUncertainties/tags/JetUncertainties-00-09-25",
                       "atlasoff/Reconstruction/Jet/JetMomentTools/tags/JetMomentTools-00-03-20"]
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

packages_to_patch = []

print "applying svn patches..."
for pkg in packages_to_patch:
  if pkg in packages_to_checkout:
    print "  patching {0}".format(pkg)
    subprocess.Popen(['cd $ROOTCOREBIN/../{0} && pwd && patch -p0 -i $ROOTCOREBIN/../xAODAnaHelpers/data/{0}_Base.{1}.diff && cd - && pwd'.format(pkg, args.version)], env=rc_env, shell=True ).wait()
  else:
    print "  no patches to be applied!"


