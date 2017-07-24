#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging
logger = logging.getLogger("xAH")
logger.addHandler(logging.StreamHandler())

# Set up ROOT
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

__version__ = '1.0.0'
__all__ = ['']

from .config import Config
