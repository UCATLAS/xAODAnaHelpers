#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging
from .utils import ColoredLogger
logging.setLoggerClass(ColoredLogger)
logger = logging.getLogger("xAH")
logger.info("Welcome to xAODAnaHelpers. Don't forget to read the documentation: https://xaodanahelpers.readthedocs.io/en/latest/.")
