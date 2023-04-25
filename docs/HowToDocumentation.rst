How to Document Code
====================

The documentation for |xAH| uses a slightly non-trivial workflow:

#. Doxygen_ parses the header and source files to generate an XML tree of the code
#. breathe_  is a sphinx_ wrapper that enables us to parse the XML tree from doxygen
#. sphinx_ is what produces the various output formats such as html, latex, e-pub from source code comments
#. ReadTheDocs.org_ uses ``doxygen``, ``breathe``, and ``sphinx`` to automatically produce our documentation everytime ``main`` changes.

Our documentation is automatically generated for us so we will always guarantee that our documentation is up-to-date for all users.

The aim of this document is to help you get started with producing your own documentation locally to help resolve errors, typos, and make sure you're formatting it the way that you want before pushing it to our github repo.

Setting it up Locally
---------------------

Locally, we are going to need doxygen_ to do the initial parsing. Note that out of the box without doxygen, we can parse python scripts, such as xAH_run.py :ref:`xAHRunAPI`, which are included as part of |xAH|. However, if we wish to have all of our C++ code's documentation included, we will need doxygen_ to do parse it.

Doxygen
~~~~~~~

Get doxygen_ however you want. For Macs, we can use::

    brew install doxygen

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

This will also automatically call ``workon xAH``. This is something we will always run in the future to enter the virtual environment.

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

You may not always run all of these pieces each time you generate documentation. For example, if you need to make a change to the header/source files of any kind, you will need to re-run ``doxygen``. In the rare case that the html generation isn't working right, you might want to run ``make clean`` so you start over again. If you're only changing the reStructuredText (rst) files in ``docs/`` you might only ever need to run ``make html``. All in all, it doesn't take more than 10-15 seconds to generate the necessary documentation.

Documenting Code
----------------

In most cases, we will want to follow the reStructuredText directives and formatting for doing the code documentation. We just want to use ``doxygen`` + ``breathe`` to expose those comments to ``sphinx`` to parse and display correctly. In what follows, we provide a set of guidelines (really, examples) to make it easier to document our code specifically.

.. note:: All comments for a given class, function, variable should be prior to the given item you're adding documentation for.

If you have a question about how to do something, google it in the context of reStructuredText or ask on the mailing list. Also have a look through most of our source code and compare it to the docs to figure out how we do something.

One-Line Comments
~~~~~~~~~~~~~~~~~

One-line comments are very useful in cases where we do not have much to say about something, perhaps because it is a rather trivial item::

    /** @brief generically the main name assigned to all histograms */
    std::string m_name;

which will render as

.. doxygenvariable:: HistogramManager::m_name
   :no-link:

Block Comments
~~~~~~~~~~~~~~

Block comments are very useful in all other cases. When in doubt, you can always make a block comment with just a single line, even for a variable. The flexibility allows us to include a lot more detail and formatting such as tables and latex::

    /**
        @brief Destructor, allows the user to delete histograms that are not being recorded.
    */
    virtual ~HistogramManager();

which will render as

.. doxygenfunction:: HistogramManager::~HistogramManager
   :no-link:

Doxygen ``rst`` directive
~~~~~~~~~~~~~~~~~~~~~~~~~

To tell ``doxygen`` and ``breathe`` that a given block of text should be considered as reStructuredText, we simply need to wrap it:

.. code-block:: none

    @rst
        This is now inside a doxygen directive that tells doxygen not to parse it, so that breathe can parse it for Sphinx.

    @endrst

which will render as expected if we were writing it inside a standard ``.rst`` file.  As usual, we have an example::

    /**
        @brief This is used by any class extending to pre-define a set of histograms to book by default.
        @rst
            .. note:: The expectation is that the user does not directly use this class but rather inherits from it.

            We expect the user to create a new group of histograms, such as for jets::

                class JetHists : public HistogramManager
                {
                  public:
                    JetHists(std::string name, std::string detailStr);
                    virtual ~JetHists() ;

                    StatusCode initialize();
                    StatusCode execute( const xAOD::JetContainer* jets, float eventWeight, int pvLoc = -1);
                    StatusCode execute( const xAOD::Jet* jet, float eventWeight, int pvLoc = -1 );
                    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
                    using HistogramManager::execute; // overload
                };

            The above example is taken from our implementation in :cpp:class:`JetHists`.

        @endrst
     */
    class HistogramManager {};

which will render as

.. doxygenclass:: HistogramManager
   :no-link:

For everything else...
----------------------

These cover the general basics of how to document code for |xAH|. Everything else is specific to how doxygen and Sphinx and breathe work. Most of these are well-supported with a large community, so googling is always very helpful here. Otherwise, feel free to ask on the mailing list.

.. _Doxygen: http://www.doxygen.org/
.. _breathe: http://breathe.readthedocs.org/en/latest/
.. _sphinx: http://sphinx-doc.org/
.. _ReadTheDocs.org: https://readthedocs.org/
.. _virtualenvwrapper: https://virtualenvwrapper.readthedocs.org/en/latest/
.. _pip: https://pip.readthedocs.org/en/stable/
