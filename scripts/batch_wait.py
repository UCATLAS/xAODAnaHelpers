#!/usr/bin/env python

# @file:    batch_wait.py
# @purpose: wait for a batch EventLoop job to finish
# @author:  Karol Krizka <kkrizka@cern.ch>
# @date:    May 2016
#
# @example:
# @code
# batch_wait.py --help
# @endcode
#

from __future__ import print_function
#TODO: move into __main__
import logging

root_logger = logging.getLogger()
root_logger.addHandler(logging.StreamHandler())
xAH_logger = logging.getLogger("xAH")

import argparse
import os
import subprocess
import sys
import datetime
import time

try:
  __version__ = subprocess.check_output(["git", "describe", "--always"], cwd=os.path.dirname(os.path.realpath(__file__))).strip()
  __short_hash__ = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"], cwd=os.path.dirname(os.path.realpath(__file__))).strip()
except:
  print('git not available, assuming svn')
  __version__ = "private"
  __short_hash__ = "0"

baseUsageStr = "batch_wait.py --submitDir submit_dir [options]"
parser = argparse.ArgumentParser(add_help=False, description='Wait for batch EventLoop jobs to finish.',
                                 usage=baseUsageStr)

# add custom help
parser.add_argument('-h', '--help', help='show this help message and exit.')

# http://stackoverflow.com/a/16981688
parser._positionals.title = "required"
parser._optionals.title = "optional"

# positional argument, require the first argument to be the input filename
parser.add_argument('--submitDir', dest='submit_dir', metavar='<directory>', type=str, required=True, help='Directory with the submission.', default='submitDir')

parser.add_argument('-v', '--verbose', dest='verbose', action='count', default=0, help='Enable verbose output of various levels. Can increase verbosity by adding more ``-vv``. Default: no verbosity')

# start the script
if __name__ == "__main__":
  SCRIPT_START_TIME = datetime.datetime.now()

  # parse the arguments, throw errors if missing any
  args = parser.parse_args()

  # set verbosity for python printing
  if args.verbose < 4:
    xAH_logger.setLevel(25 - args.verbose*5)
  else:
    xAH_logger.setLevel(logging.NOTSET + 1)

  try:
    # check submission directory
    if not os.path.exists(args.submit_dir):
        raise OSError('Output directory {0:s} does not exist!'.format(args.submit_dir))

    # at this point, we should import ROOT and do stuff
    import ROOT
    xAH_logger.info("loading packages")
    ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

    # Wait
    if not ROOT.EL.Driver.wait(args.submit_dir):
      sys.exit(1)
  except Exception, e:
    # we crashed
    xAH_logger.exception("{0}\nAn exception was caught!".format("-"*20))
    sys.exit(1)
