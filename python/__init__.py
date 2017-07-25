#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function

# Set up ROOT correctly
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

__version__ = '1.0.0'
__all__ = ['']

from . import logging as xAH_logging
from .config import Config
import logging
logger = logging.getLogger("xAH")

# load the standard algorithm since pyroot delays quickly
logger.info("Loading up your analysis dictionaries now, give us a second.")
ROOT.EL.Algorithm()
# load this for the MSG::level values. See https://its.cern.ch/jira/browse/ATLASG-270
ROOT.asg.ToolStore()
logger.info("All loaded and good to go. Have a wonderful day :)")
