#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging
logger = logging.getLogger("xAH.utils")
logger.setLevel(10) # we use info

from PathResolver import PathResolver
import random

import os

class NameGenerator(object):
  adjectives = None
  animals = None
  def __init__(self):
    if self.__class__.adjectives is None:
      self.__class__.adjectives = file(PathResolver.FindCalibFile("xAODAnaHelpers/xAH_adjectives")).read().splitlines()
    if self.__class__.animals is None:
      self.__class__.animals = file(PathResolver.FindCalibFile("xAODAnaHelpers/xAH_animals")).read().splitlines()
  def __repr__(self):
    return "{0:s}{1:s}".format(random.choice(self.__class__.adjectives).capitalize(), random.choice(self.__class__.animals).capitalize())

  def __str__(self):
    return self.__repr__()


def is_release20():
  return int(os.environ.get('ROOTCORE_RELEASE_SERIES', 0)) < 25

def is_release21():
  return not is_release20()
