How to Document Code
====================

The documentation for |xAH| uses a slightly non-trivial workflow:

1. Doxygen_ parses the header and source files to generate an XML tree of the code
1. breathe_  is a sphinx_ wrapper that enables us to parse the XML tree from doxygen
1. sphinx_ is what produces the various output formats such as html, latex, e-pub from source code comments
1. ReadTheDocs.org_ uses ``doxygen``, ``breathe``, and ``sphinx`` to automatically produce our documentation everytime ``master`` changes.

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

.. _Doxygen: http://www.doxygen.org/
.. _breathe: http://breathe.readthedocs.org/en/latest/
.. _sphinx: http://sphinx-doc.org/
.. _ReadTheDocs.org: https://readthedocs.org/
