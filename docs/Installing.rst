Installing
----------

::

    setupATLAS
    rcSetup Base,X.Y.Z
    git clone https://github.com/UCATLAS/xAODAnaHelpers

or for a specific tag

::

    rc checkout_pkg atlasinst/Institutes/UChicago/xAODAnaHelpers/tags/xAODAnaHelpers-XX-YY-ZZ xAODAnaHelpers

::

    git clone https://github.com/UCATLAS/xAODAnaHelpers
    cd xAODAnaHelpers
    git checkout tags/XX-YY-ZZ

.. note::

    `If you have ssh-keys set up <https://help.github.com/articles/generating-ssh-keys/>`_, then you can clone over SSH instead of HTTPS::

        git clone git@github.com:UCATLAS/xAODAnaHelpers

At this point, you have the FULL state of the code. You can run
``git log`` to view the recent changes (no more ChangeLog!). You can run
``git tag`` to view all current tags.

Getting tag XX-YY-ZZ
--------------------

So now you want to check out a specific tag. We will provide tags on the
svn as well as on github. If you are using git, then you can switch
between tags in the same folder in seconds. If you are using svn, you
need to redownload the full tag.

With git
~~~~~~~~

::

    git checkout -b XX-YY-ZZ tags/XX-YY-ZZ

This switches you from master to a branch of the given version.

With svn
~~~~~~~~

::

    svn co svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UChicago/xAODAnaHelpers/tags/xAODAnaHelpers-XX-YY-ZZ xAODAnaHelpers

This will download the full tag from svn for you.

With RootCore
~~~~~~~~~~~~~

::
    rc checkout_pkg atlasinst/Institutes/UChicago/xAODAnaHelpers/tags/xAODAnaHelpers-XX-YY-ZZ

This uses the svn call, but it is a little less verbose :smile:

Updating changes
----------------

If you're on branch ``myBranch`` and you have commits that you want to
push to the remote ``origin`` - the first thing you should do is always
update so you're current::

    git pull --rebase

will do it all. If you want more control, use::

    git fetch
    git rebase origin/master

or::

    git fetch origin
    git rebase origin/master myBranch

.. note::
    - ``git fetch`` will fetch from ``origin`` (see ``git remote -v`` for what that's defined as) by default, but you can explicitly provide a different remote repository.
    - ``git rebase origin/master`` will rebase the current branch you are on.  You can specify another branch if you want.
