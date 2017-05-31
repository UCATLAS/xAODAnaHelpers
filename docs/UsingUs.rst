xAH_run.py
==========

``xAH_run.py`` is the xAODAnaHelpers macro written fully in python. The
goal is to make it easier for a user to spin up an analysis without
(potentially) writing any C++ code at all!

Introduction
------------

An analysis job is defined by a few key things:
- the files to run over
- where to run the code
- what algorithms to run

and a few other minor features such as submission directory or how many events to run. Primarily, these three things listed above are all you need to get started. ``xAH_run.py`` manages all of these for you.

A configuration file, written in `json <http://www.json.org/>`_ or python, is used to specify what algorithms to run, and in what order. You pass in a list of files you want to run over to the script itself, as well as where to run the code. It will take care of the rest for you.

Getting Started
---------------

To get started, we assume you are little bit familiar with xAODAnaHelpers and RootCore in general. Recall that when you compile a bunch of packages, you generate a namespace under ``ROOT`` that all your algorithms are loaded into via

.. code:: python

    ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")

so that one could create an algorithm by something like ``ROOT.AlgorithmName()`` and then start configuring it. In fact, this is how one normally does it within python. Namespaces are automatically linked up by something like ``ROOT.Namespace.AlgorithmName()`` in case you wrapped the entire algorithm in a namespace.

A simple plotting example
~~~~~~~~~~~~~~~~~~~~~~~~~

To get started, let's just ask a simple question: "How can I make plots of Anti-Kt, R=0.4, LC-calibrated jets?" Let's assume ``xAODAnaHelpers`` has already been checked out and everything is compiled. We only need to know the three key things.

What algorithms to run
^^^^^^^^^^^^^^^^^^^^^^

We will run 2 algorithms. First is :class:`BasicEventSelection` to filter/clean events. The second is :class:`JetHistsAlgo` which will allow us to plot the jets we want. So start with the template JSON file:

.. code:: json

    [
      { "class": "BasicEventSelection",
        "configs": {
        }
      },
      {
        "class": "JetHistsAlgo",
        "configs": {
        }
      }
    ]

This gets us started. We make a list of algorithms that we want to run, this list is considered *sorted*. Each list contains a dictionary object, one which defines the ``class`` to run and another which defines a dictionary of configurations to pass into that algorithm.

Next, we should probably add some obvious configurations that work for us. I look up the header files of each and decide to flesh it out as below:

.. code:: json

    [
      { "class": "BasicEventSelection",
        "configs": {
          "m_truthLevelOnly": false,
          "m_applyGRLCut": true,
          "m_GRLxml": "$ROOTCOREBIN/data/xAODAnaHelpers/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml",
          "m_doPUreweighting": false,
          "m_vertexContainerName": "PrimaryVertices",
          "m_PVNTrack": 2,
          "m_name": "myBaseEventSel"
        }
      },
      {
        "class": "JetHistsAlgo",
        "configs": {
          "m_inContainerName": "AntiKt4EMTopoJets",
          "m_detailStr": "kinematic",
          "m_name": "NoPreSel"
        }
      }
    ]

and I save this into ``xah_run_example.json``. If you want more variables in your plots, add other possibilities in the detailStr field, separated by a space.

Running the script
^^^^^^^^^^^^^^^^^^

I pretty much have everything I need to work with. So, I run the following command

.. code:: bash

    xAH_run.py --files file1.root file2.root --config xah_run_example.json direct

which will run over two ROOT files locally (``direct``), using the configuration we made. We're all done! That was easy :beers: .

Configuration Details
---------------------

As mentioned previous, there are multiple facets to ``xAH_run.py``. The below details the configurations that are possible for the script itself, not for the algorithms you use. For details on what can be configured, look up the header files of the algorithms themselves.

For everything listed below, the script contains all this information and is self-documenting. Simply type

.. code:: bash

    xAH_run.py -h

to see all the help information.

.. note::
    The ``{driver}`` option tells the script where to run the code. There are lots of supported drivers and more can be added if you request it. For more information, you can type ``xAH_run.py -h drivers`` of available drivers.

.. _xAHRunAPI:

API Reference
-------------

.. argparse::
   :ref: xAH_run.parser
   :prog: xAH_run.py

   --mode : @after
       `class access <https://github.com/kratsg/TheAccountant/wiki/Access-Mode>`__ mode or `branch access <https://github.com/kratsg/TheAccountant/wiki/Access-Mode>`__ mode
