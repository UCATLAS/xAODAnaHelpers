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

which switches you from main to a branch of the given version.


Compiling
---------

For all sets of instructions below, make sure you run ``setupATLAS`` first.

CMake-based (21.2.X)
~~~~~~~~~~~~~~~~~~~~

This step requires a little extra work, but compiles significantly faster. First, inside the ``workdir`` directory, we'll create a build and source directory. The source directory will contain all packages we build in CMake:

.. parsed-literal::

  mkdir src build

Then we'll set up a release inside the source:

.. parsed-literal::

  cd src
  asetup (RELEASE),here

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

  source build/*/setup.sh

Environment variables like ``${AnalysisBase_PLATFORM}`` (or ``${AnalysisTop_PLATFORM}``) seem to contain the correct variable which represents the architecture of the system, e.g. ``x86_64-slc6-gcc49-opt``.

Docker
------

Assuming you have `docker <https://www.docker.com/)>`_, you can always grab the latest image for a given release (e.g. 21.2.4) like so:

.. code-block:: bash

  docker pull ucatlas/xah:21.2.4-latest
  docker run -it --rm ucatlas/xah:21.2.4-latest bash

which puts you into the docker image and xAH is precompiled and the environment is set up so you can:

* compile your package on top of xAH [using cmake, make]
* run vanilla ```xAH_run.py``` with a config on some ROOT files

For example, if you want to have the docker image have access to ROOT files locally on your computer, you can "mount" a folder in it like so:

.. code-block:: bash

  docker run -it --rm -v /path/to/data/files:/home/atlas/data ucatlas/xah:21.2.4-latest bash

and /home/atlas/data inside the docker file will map to /path/to/data/files on your computer (host).
