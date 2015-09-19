import ROOT

#ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

class xAH_config(object):
  def __init__(self):
    self._algorithms = []

  def __setattr__(self, name, val):
    if name in ['_job', '_algorithms']:
      super(xAH_config, self).__setattr__(name, val)
    else:

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
      print "Confguring:",alg
      for k,v in val.iteritems():
        if not hasattr(alg_obj, k): 
          raise AttributeError(k)
        print "\t",k,"=",v
        setattr(alg_obj, k, v)
      print

        
      #
      # Add the constructed algo to the list of algorithms to run
      #
      self._algorithms.append(alg_obj)

  def __getattr__(self, name):
    if hasattr(self, name):
      return super(xAH_config, self).__getattr__(self, name)
    else:
      return getattr(ROOT, name)

