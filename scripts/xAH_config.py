import ROOT

class xAH_config(object):
  def __init__(self):
    self._algorithms = []
    self._log        = []

  def setalg(self, name, val):
      #
      # Construct the given constructor
      #    (complicated b/c we have to deal nesting of namespaces)
      #
      alg = reduce(lambda x,y: getattr(x, y, None), name.split('.'), ROOT)
      if alg is None:
        raise AttributeError(name)

      #
      # Construct an instance of the alg and set its attributes
      #
      alg_obj = alg()
      self._log.append((alg,))
      for k,v in val.iteritems():
        if not hasattr(alg_obj, k):
          raise AttributeError(k)
        self._log.append((alg, k, v))
        setattr(alg_obj, k, v)
      #print


      #
      # Add the constructed algo to the list of algorithms to run
      #
      self._algorithms.append(alg_obj)


