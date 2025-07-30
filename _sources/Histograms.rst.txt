Histograms
==========

There are three generic levels to include when building up an analysis
that involves plotting: \* HistogramManager \* JetHists, ElectronHists,
MuonHists, etc... \* JetHistsAlgo, ElectronHistsAlgo, MuonHistsAlgo,
etc...

In order: *HistogramManager* should rarely be changed. This manages the
histograms for you in *EventLoop* algorithms by initializing histograms
and adding it to the worker. *JetHists*, etc are plotting classes to
pre-define the set of plots you want to use for a given set of objects
-- as well as how to plot them. Finally, *JetHistsAlgo*, etc... are
*EventLoop* algorithms that you would include in your jobs and run to
actually apply those plots.


HistogramManager
----------------

This is the base class from which all histogram management classes are made for Muons, Jets, Electrons, etcetera. It is meant to be flexible enough for someone to use it to create their own set of histograms to produce for an algorithm from scratch using the class.

In particular, the ``book()`` functions are overloaded for good reason - they all do the same thing except the number of arguments supplied tells us what kind of histogram you want to make: 1D, 2D, or 3D. All histograms take in a ``name`` and a ``title`` which get concatenated to provide the ``stored name`` of the histogram ``(name+title)``. If you wish to use TDirectoryFiles automagically, append a forward-slash to the end of the ``name``, such as ``"AntiKt10/"``. The ``book()`` function will create the histogram, set up the title, the labels, append it to ``m_allHists``, and returns a pointer to the newly created histogram.  The last argument is ``sumw2`` which tells the function whether to enable ``sumw2()`` for the histogram or not, this defaults to ``true``. The order of the arguments are listed in the table.

.. doxygenclass:: HistogramManager
   :members:
   :undoc-members:
   :protected-members:
   :private-members:

Classes
-------

This is a class that predefines all the histograms, defines the *execute* function which fills in the histograms for you, given an object or a collection of objects, and handles a lot of other logic.  This class extends *HistogramManager*.

.. toctree::
   :maxdepth: 2

   ClusterHists
   JetHists
   MetHists
   MuonHists
   TrackHists
   VtxHists

Algorithms
----------

This is an EL Algorithm that incorporates the correspondingly-named class.


.. toctree::
   :maxdepth: 2

   ClusterHistsAlgo
   JetHistsAlgo
   MetHistsAlgo
   MuonHistsAlgo
   TrackHistsAlgo



