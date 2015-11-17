How to Document Code
====================

The documentation for |xAH| uses a slightly non-trivial workflow:

#. Doxygen_ parses the header and source files to generate an XML tree of the code
#. breathe_  is a sphinx_ wrapper that enables us to parse the XML tree from doxygen
#. sphinx_ is what produces the various output formats such as html, latex, e-pub from source code comments
#. ReadTheDocs.org_ uses ``doxygen``, ``breathe``, and ``sphinx`` to automatically produce our documentation everytime ``master`` changes.

Our documentation is automatically generated for us so we will always guarantee that our documentation is up-to-date for all users.

The aim of this document is to help you get started with producing your own documentation locally to help resolve errors, typos, and make sure you're formatting it the way that you want before pushing it to our github repo.

Setting it up Locally
---------------------

Locally, we are going to need doxygen_ to do the initial parsing. Note that out of the box without doxygen, we can parse python scripts, such as xAH_run.py :ref:`xAHRunAPI`, which are included as part of |xAH|. However, if we wish to have all of our C++ code's documentation included, we will need doxygen_ to do parse it.

Doxygen
~~~~~~~

Get doxygen_ however you want. For Macs, we can use::

    brew info doxygen

to install it. At this point, one should be able to generate the XML tree by navigating to the ``docs`` folder and running ``doxygen`` with no arguments::

    cd docs
    doxygen

since we provide a ``Doxyfile`` in the ``docs`` directory with the correct configurations.

Python Virtual Environment
~~~~~~~~~~~~~~~~~~~~~~~~~~

Next, I suggest setting up a python virtual environment. Luckily, this solution is the hardest part.  Most (rational) people use virtualenvwrapper_ to manage my python dependencies and workspace. It is assumed you already have pip_.

To get the entire functionality of ``venvwrapper``, we just need to grab the package and update our environment when we want to use it::

    pip install virtualenvwrapper
    echo "source /usr/local/bin/virtualenvwrapper.sh" >> ~/.bash_profile

.. note::

    Don't forget to source your profile if you're going to use the same shell::

        source ~/.bash_profile

From now on, we will have commands like ``mkvirtualenv``, ``workon``, and ``rmvirtualenv`` in our shell. As a first-time user, you haven't made a virtual environment yet, so the first thing we do is make one::

    mkvirtualenv xAH

This will also automatically call ``workon xAH`` for us in the future.

.. note::

    If you ever forget the name of the virtual environment you made, just run ``workon`` without any arguments. There is also tab completion.

Python Packages
~~~~~~~~~~~~~~~

.. note::

    If you choose to use a virtual environment, enter it ``workon xAH``

This is super easy. We provide a ``requirements.txt`` file::

    cd docs
    pip install -r requirements.txt

which will install all the required packages for you. As of the time of this document, this contains the following packages:

.. include:: requirements.txt
   :literal:

Generate Docs Locally
---------------------

Now that we have ``doxygen`` and all of the required python packages installed, all you need to do now is process everything::

    cd docs
    make clean
    doxygen
    make html
    open _build/html/index.html

and we're good to go. Sphinx provides a ``Makefile`` in ``docs/`` to make the html generation much easier to work with.

.. _Doxygen: http://www.doxygen.org/
.. _breathe: http://breathe.readthedocs.org/en/latest/
.. _sphinx: http://sphinx-doc.org/
.. _ReadTheDocs.org: https://readthedocs.org/
.. _virtualenvwrapper: https://virtualenvwrapper.readthedocs.org/en/latest/
.. _pip: https://pip.readthedocs.org/en/stable/
