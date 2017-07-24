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


class ColoredFormatter(logging.Formatter):
  RESET_SEQ = "\033[0m"
  COLOR_SEQ = "\033[1;%dm"
  BOLD_SEQ = "\033[1m"
  BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE = range(8)
  #The background is set with 40 plus the number of the color, and the foreground with 30
  #These are the sequences need to get colored ouput
  COLORS = {
    'WARNING': YELLOW,
    'INFO': WHITE,
    'DEBUG': BLUE,
    'CRITICAL': YELLOW,
    'ERROR': RED
  }

  def __init__(self, msg=None, use_color = True):
    if msg is None:
      msg = "[$BOLD%(asctime)s$RESET][%(levelname)-18s]  %(message)s ($BOLD%(filename)s$RESET:%(lineno)d)"
    msg = msg.replace("$RESET", self.RESET_SEQ).replace("$BOLD", self.BOLD_SEQ)
    logging.Formatter.__init__(self, msg)
    self.use_color = use_color

  def format(self, record):
    levelname = record.levelname
    if self.use_color and levelname in self.COLORS:
      levelname_color = self.COLOR_SEQ % (30 + self.COLORS[levelname]) + levelname + self.RESET_SEQ
      record.levelname = levelname_color
    return logging.Formatter.format(self, record)

# Custom logger class with multiple destinations
class ColoredLogger(logging.Logger):
  def __init__(self, name):
    logging.Logger.__init__(self, name, logging.DEBUG)
    color_formatter = ColoredFormatter()
    if '.' in name: return
    console = logging.StreamHandler()
    console.setFormatter(color_formatter)
    self.addHandler(console)
    return
