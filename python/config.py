#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging
from functools import reduce
logger = logging.getLogger("xAH.config")

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

import inspect
from AnaAlgorithm.AnaAlgorithmConfig import AnaAlgorithmConfig
from AnaAlgorithm.PythonConfig import PythonConfig

from .utils import NameGenerator, vector

class Config(object):
  def __init__(self):
    self._algorithms = []
    self._samples    = {}
    self._outputs    = set([])
    self._log        = []

  def setalg(self, className, options):
    logger.warning("Config::setalg is being renamed to Config::algorithm.")
    import inspect
    frame, path, lineno, source, lines, _ = inspect.stack()[1]
    logger.warning("\tPossible call stack: {0:s}({1:d}): {2:s}".format(path, lineno, lines[0].strip()))
    return self.algorithm(className, options)

  def algorithm(self, *args):
    # check first argument
    # if isinstance(className, unicode):
    # if not isinstance(className, str):
    #   raise TypeError("className must be a string")

    assert len(args) == 1 or len(args) == 2

    className = args[0]
    options = args[1]
    if not isinstance(options, dict):
      raise TypeError("Must pass in a dictionary of options")

    # if m_name not set, randomly generate it
    algName = options.get("m_name", None)
    algName_is_random = False
    if algName is None:
      algName_is_random = True
      algName = str(NameGenerator())
      logger.warning("Setting missing m_name={0:s} for instance of {1:s}".format(algName, className))
      options['m_name'] = algName
    # if not isinstance(algName, str) and not isinstance(algName, unicode):
    #   raise TypeError("'m_name' must be a string for instance of {0:s}".format(className))

    # handle deprecation of m_debug, m_verbose
    if 'm_debug' in options:
      logger.warning("m_debug is being deprecated. See https://github.com/UCATLAS/xAODAnaHelpers/pull/882 . Set m_msgLevel='debug'")
    if 'm_verbose' in options:
      logger.warning("m_verbose is being deprecated. See https://github.com/UCATLAS/xAODAnaHelpers/pull/882 . Set m_msgLevel='verbose'.")

    # handle msgLevels, can be string or integer
    msgLevel = options.get("m_msgLevel", "info")
    # if isinstance(msgLevel, unicode): msgLevel = msgLevel.encode('utf-8')
    if not isinstance(msgLevel, str) and not isinstance(msgLevel, int):
      raise TypeError("m_msgLevel must be a string or integer for instance of {0:s}".format(className))
    if isinstance(msgLevel, str):
      if not hasattr(ROOT.MSG, msgLevel.upper()):
        raise ValueError("m_msgLevel must be a valid MSG::level: {0:s}".format(msgLevel))
      msgLevel = getattr(ROOT.MSG, msgLevel.upper())
    options['m_msgLevel'] = msgLevel

    # Construct the given constructor
    #    (complicated b/c we have to deal nesting of namespaces)
    alg = reduce(lambda x,y: getattr(x, y, None), className.split('::'), ROOT)
    if alg is None:
      raise AttributeError(className)

    # get a list of parent classes
    parents = inspect.getmro(alg)
    if ROOT.EL.Algorithm in parents:
      # Construct an instance of the alg and set its attributes
      alg_obj = alg()
      alg_obj.SetName(algName)
      self._log.append((className,algName))
      alg_obj.setMsgLevel(msgLevel)
      for k,v in options.items():
        # only crash on algorithm configurations that aren't m_msgLevel and m_name (xAH specific)
        if not hasattr(alg_obj, k) and k not in ['m_msgLevel', 'm_name']:
          raise AttributeError(k)
        elif hasattr(alg_obj, k):
          self._set_algo_attribute(alg_obj, k, v, className, algName)
    elif ROOT.EL.AnaAlgorithm in parents:
      alg_obj = AnaAlgorithmConfig(className)
      alg_obj.setName(algName)
      if 'm_outputStream' in options:
        self.output(options['m_outputStream'])
      self._log.append((className, algName))
      # TODO
      #setattr(alg_obj, "OutputLevel", msgLevel)
      for k,v in options.items():
        if k in ['m_msgLevel', 'm_name']: continue
        self._set_algo_attribute(alg_obj, k, v, className, algName)
    elif ROOT.EL.AnaReentrantAlgorithm in parents:
      alg_obj = PythonConfig(className+"/"+algName)
      alg_obj.setComponentType( "AnaReentrantAlgorithm" )
      if 'm_outputStream' in options:
        self.output(options['m_outputStream'])
      self._log.append((className, algName))
      # TODO
      #setattr(alg_obj, "OutputLevel", msgLevel)
      for k,v in options.items():
        if k in ['m_msgLevel', 'm_name']: continue
        self._set_algo_attribute(alg_obj, k, v, className, algName)
    else:
      raise TypeError("Algorithm {0:s} is not an EL::Algorithm or EL::AnaAlgorithm or EL::AnaReentrantAlgorithm. I do not know how to configure it. {1}".format(className, parents))

    # Add the constructed algo to the list of algorithms to run
    self._algorithms.append(alg_obj)

  def _set_algo_attribute(self, alg_obj, name, value, className, algName):
    #handle unicode from json
    #if isinstance(value, unicode):
    #  value = value.encode('utf-8')
    if isinstance(value, (list, tuple)):
      # manually call vector in user code when this fails on an empty list/tuple
      value = vector(value) 
    self._log.append((algName, name, value))
    try:
      setattr(alg_obj, name, value)
    except:
      logger.error("There was a problem setting {0:s} to {1} for {2:s}::{3:s}".format(name, value, className, algName))
      raise
    

  # set based on patterns
  def sample(self, pattern, **kwargs):
    pattern = str(pattern)
    try:
      self._samples[pattern].update(kwargs)
    except KeyError:
      self._samples[pattern] = kwargs

  def output(self, name):
    self._outputs.add(str(name))
