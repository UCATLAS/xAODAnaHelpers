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
 
import subprocess, argparse

parser = argparse.ArgumentParser(description='Checkout packages and apply patches!', usage='%(prog)s version')
parser.add_argument('version', metavar='ASGversion',
                    help='the ASG release you have set up')
args = parser.parse_args()

print "using ASG version %s" %(args.version)

dict_pkg = {'2.1.29': ["ElectronEfficiencyCorrection"],
            '2.1.30': ["atlasoff/Reconstruction/egamma/egammaMVACalib/tags/egammaMVACalib-01-00-43",
                       "atlasoff/Control/xAODRootAccess/tags/xAODRootAccess-00-01-04",
                       "ElectronEfficiencyCorrection"],
            '2.1.31': ["atlasoff/Reconstruction/egamma/egammaMVACalib/tags/egammaMVACalib-01-00-43",
                       "atlasoff/Control/xAODRootAccess/tags/xAODRootAccess-00-01-04",
                       "ElectronEfficiencyCorrection"]
            }

for key in dict_pkg.keys():
  if ( key != args.version ) :
    continue
  for item in dict_pkg[key]:
    print "checking out package: %s" %(item)
    subprocess.call(['rc','checkout_pkg', '%s' %(item) ], shell=True ) 
 
packages_to_patch = ["ElectronEfficiencyCorrection"]

if len(packages_to_patch)>0 :
  print "applying svn patches"

for pkg in packages_to_patch:
  print "  patching %s " % (pkg)
  subprocess.call(['cd', './%s' %(pkg) ], shell=True )
  subprocess.call(['patch', '-p0', '-i', '../xAODAnaHelpers/data/%s_Base.%s.diff' % (pkg, args.version)], shell=True )
  subprocess.call(['cd', '../'], shell=True)

