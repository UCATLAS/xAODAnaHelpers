from PathResolver import PathResolver
import random

import logging
logger = logging.getLogger("xAH_nameGenerator")
logger.setLevel(10) # we use info

class xAH_nameGenerator(object):
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
