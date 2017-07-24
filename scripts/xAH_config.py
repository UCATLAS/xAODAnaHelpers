#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging

from xAODAnaHelpers import Config as xAH_config

logger = logging.getLogger("xAH.config")
logger.setLevel(10) # we use info

logger.warning("xAH_config is being deprecated. Use 'from xAODAnaHelpers import Config as xAH_config' to maintain backwards compatibility... or 'from xAODAnaHelpers import Config' and rename 'xAH_config' to 'Config'.")
