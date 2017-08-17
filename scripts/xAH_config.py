#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging
logger = logging.getLogger("xAH.config")

from xAODAnaHelpers import Config as xAH_config

import inspect

logger.warning("xAH_config is being deprecated. Use 'from xAODAnaHelpers import Config as xAH_config' to maintain backwards compatibility... or 'from xAODAnaHelpers import Config' and rename 'xAH_config' to 'Config'. Possible call stack:")
for frame, path, lineno, source, lines, _ in inspect.stack()[1:]:
  if 'importhook' in source or path.endswith('ROOT.py'): continue
  logger.warning('\t{0:s}({1:d}): {2:s}'.format(path, lineno, lines[0].strip()))
