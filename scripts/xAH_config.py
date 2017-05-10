import ROOT

class xAH_config(object):
  def __init__(self):
    self._algorithms = []
    self._log        = []

  def setalg(self, className, options):
    # check first argument
    if not isinstance(className, str):
      raise ValueError("className must be a string")

    if not isinstance(options, dict):
      raise ValueError("Must pass in a dictionary of options")

    algName = options.get("m_name", None)
    if algName is None:
      raise KeyError("'m_name' is not set for instance of {0:s}".format(className))
    if not isinstance(algName, str):
      raise ValueError("'m_name' must be a string for instance of {0:s}".format(className))

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
