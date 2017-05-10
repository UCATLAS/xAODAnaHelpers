import ROOT
# load this for the MSG::level values. See https://its.cern.ch/jira/browse/ATLASG-270
ROOT.asg.ToolStore()

import logging
logger = logging.getLogger("xAH_config")
logger.setLevel(10) # we use info

class xAH_config(object):
  def __init__(self):
    self._algorithms = []
    self._log        = []

  def setalg(self, className, options):
    # check first argument
    if not isinstance(className, str):
      raise TypeError("className must be a string")

    if not isinstance(options, dict):
      raise TypeError("Must pass in a dictionary of options")

    algName = options.get("m_name", None)
    if algName is None:
      raise KeyError("'m_name' is not set for instance of {0:s}".format(className))
    if not isinstance(algName, str):
      raise TypeError("'m_name' must be a string for instance of {0:s}".format(className))

    if 'm_debug' in options:
      logger.warning("m_debug is being deprecated. See https://github.com/UCATLAS/xAODAnaHelpers/pull/882 .")

    debugLevel = options.get("m_debugLevel", "info")
    if not isinstance(debugLevel, str):
      raise TypeError("'m_debugLevel' must be a string for instance of {0:s}".format(className))
    if not hasattr(ROOT.MSG, debugLevel.upper()):
      raise ValueError("'m_debugLevel' must be a valid MSG::level: {0:s}".format(debugLevel))
    debugLevel = getattr(ROOT.MSG, debugLevel.upper())
    options['m_debugLevel'] = debugLevel

    #
    # Construct the given constructor
    #    (complicated b/c we have to deal nesting of namespaces)
    #
    alg = reduce(lambda x,y: getattr(x, y, None), className.split('.'), ROOT)
    if alg is None:
      raise AttributeError(className)

    #
    # Construct an instance of the alg and set its attributes
    #
    alg_obj = alg()
    alg_obj.SetName(algName)
    alg_obj.msg().setLevel(debugLevel)
    self._log.append((alg,algName))
    for k,v in options.iteritems():
      if not hasattr(alg_obj, k):
        raise AttributeError(k)
      self._log.append((alg, k, v))
      setattr(alg_obj, k, v)
    #print

    #
    # Add the constructed algo to the list of algorithms to run
    #
    self._algorithms.append(alg_obj)
