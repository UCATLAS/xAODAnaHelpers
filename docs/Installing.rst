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
    rc find_packages
    rc compile

CMake-based RootCore (> 2.5.X)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This step requires a little extra work, but compiles significantly faster. You will need a build directory that builds all your checked-out packages which is separate from your source code:

.. code-block:: bash

  mkdir build && cd $_

.. note:: This is inside the ``workdir``, so you will have ``workdir/xAODAnaHelpers`` and ``workdir/build`` as paths, for example.

Next, inside the ``build`` directory, we'll set up a CMake RC release and then run cmake to generate our makefiles, then compile

.. parsed-literal::

  asetup AnalysisBase,\ |ab_release_cm|\
  cmake ../
  make

The last thing you need to do is get your environment set up correctly, so you will need to source ``setup.sh``:

.. code-block:: bash

  source build/${BINARY_TAG}/setup.sh

Both ``${CMTCONFIG}`` and  ``${BINARY_TAG}`` seem to contain the correct variable which represents the architecture of the system, e.g. ``x86_64-slc6-gcc49-opt``.
