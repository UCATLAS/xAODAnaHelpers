Installing
==========

Getting the Source
------------------

Start in a work directory

.. code-block:: bash

  mkdir workdir && cd $_

Then clone the source

.. code-block:: bash

  git clone https://github.com/UCATLAS/xAODAnaHelpers

.. note::

    `If you have ssh-keys set up <https://help.github.com/articles/generating-ssh-keys/>`_, then you can clone over SSH instead of HTTPS:

      .. code-block:: bash

        git clone git@github.com:UCATLAS/xAODAnaHelpers

At this point, you have the FULL state of the code. You can run ``git log`` to view the recent changes (no more ChangeLog!).

Checking out a specific tag
~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can run ``git tag`` to view all current tags. You can checkout a specific tag (in a detached head state):

.. code-block:: bash

  cd xAODAnaHelpers
  git checkout tags/XX-YY-ZZ
  cd ../

or you can use:

.. code-block:: bash

  cd xAODAnaHelpers
  git checkout -b XX-YY-ZZ tags/XX-YY-ZZ
  cd ../

which switches you from master to a branch of the given version.


Compiling
---------

For all sets of instructions below, make sure you run ``setupATLAS`` first.

RootCore (< 2.5.X)
~~~~~~~~~~~~~~~~~~

.. parsed-literal::

    rcSetup Base,\ |ab_release_rc|\

and then find all packages and then compile:

.. code-block:: bash

    rc find_packages
    rc compile

.. important::

    EventLoopGrid-00-00-54 has a bug affecting job submissions on the grid. Please downgrade via::

      atlasoff/PhysicsAnalysis/D3PDTools/EventLoopGrid/tags/EventLoopGrid-00-00-53

CMake-based RootCore (> 2.5.X)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This step requires a little extra work, but compiles significantly faster. First, inside the ``workdir`` directory, we'll create a build and source directory. The source directory will contain all packages we build in CMake:

.. parsed-literal::

  mkdir src build

Then we'll set up a CMake RC release inside the source:

.. parsed-literal::

  cd src
  asetup AnalysisBase,\ |ab_release_cm|\,here

This also sets up a ``CMakeLists.txt`` file in this top-level directory that searches for all packages you've checked out inside it. At this point, clone/checkout all packages you need such as |xAH|:

.. parsed-literal::

  git clone <url>/UCATLAS/xAODAnaHelpers.git

Next, you will need to change to your build directory that builds all your checked-out packages which is separate from your source code:

.. parsed-literal::

  cd ../build

.. note:: This is inside the ``workdir``, so you will have ``workdir/src/xAODAnaHelpers`` and ``workdir/build`` as paths, for example.

and then run cmake to generate our makefiles, then compile:

.. code-block:: bash

  cmake ../src
  make
  cd ../

The last thing you need to do is get your environment set up correctly, so you will need to source ``setup.sh`` (from the top-level directory):

.. code-block:: bash

  source build/${CMTCONFIG}/setup.sh

Environment variables like ``${AnalysisBase_PLATFORM}`` seem to contain the correct variable which represents the architecture of the system, e.g. ``x86_64-slc6-gcc49-opt``.

.. warning::

  If you run into a RuntimeError about ``RootCore/Packages.h``, this is due to a known bug in ROOT auto-loading the dictionary for this file. To fix it, you just need to run

  .. code-block:: bash

    export ROOT_INCLUDE_PATH=/cvmfs/atlas.cern.ch/repo/sw/ASG/2.6/AnalysisBase/2.6.1/InstallArea/x86_64-slc6-gcc49-opt/RootCore/include:$ROOT_INCLUDE_PATH

  before running the ``xAH_run.py`` commands. This should fix things up. Don't forget to include the right version if you're using 2.6.X! This should not happen in 21.2 releases.
