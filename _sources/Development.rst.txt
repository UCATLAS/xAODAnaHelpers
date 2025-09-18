Development
===========

.. warning::
    Never push to main. Always create a new branch for changes, rebase your branch with main ``git pull --rebase origin main`` and use the branch for creating a pull-request to merge with main. This keeps main mergeable for everyone for all development.

.. toctree::
   :maxdepth: 2

   HowToDocumentation
   Issues

Development Workflow
--------------------

Changes should be tested properly ( "it compiles" is not sufficient ). We use the ``git rebase`` workflow.

New User
~~~~~~~~

.. highlight:: none

This is for users who do not have write access to ``UCATLAS/xAODAnaHelpers`` to make branches. Instead, they fork and write their changes to their own repository and submit pull-requests.

-  For *very* new users, you may want to setup SSH key access to your personal repository. To do this, follow the instructions at `Generating SSH keys <https://help.github.com/articles/generating-ssh-keys/>`_. So you go here: https://github.com/UCATLAS/xAODAnaHelpers/ and just click the 'Fork' at top right. This forks a copy into your account (``yourAccount/xAODAnaHelpers``). Next, clone it. Set the upstream::

    git clone git@github.com:yourAccount/xAODAnaHelpers
    cd xAODAnaHelpers
    git remote add upstream git@github.com:UCATLAS/xAODAnaHelpers

Note: If you do not have an ssh-key set up, you may want to use the HTTPS version of the above URL::

    git remote add upstream https://github.com/UCATLAS/xAODAnaHelpers

Next, make your changes for the feature/bug/fix::

    vim Root/JetSelector.cxx
    vim Root/HelpTreeBase.cxx
    git status # make sure you changed the files you want to change
    git diff # make sure the changes are what you want

Then go ahead and commit your changes::

    git add Root/JetSelector.cxx
    git commit -m "Update with new jet calibration recommendations"
    git add Root/HelpTreeBase.cxx
    git commit -m "make sure that the tree dumps the systematics for new calibrations"

When you are ready to submit a pull-request, do the following first::

    git fetch upstream
    git rebase upstream/main

to make sure your code is up to date with the `upstream repository <https://github.com/UCATLAS/xAODAnaHelpers>`__.

You may want to rebase all of your changes into a single commit if you wish, and that can be done via::

    git rebase -i HEAD~N

where ``N`` is the number of commits to rebase. Then you just follow the instructions. Take care not to rebase through commits that are already on main of the upstream repo. Then submit a pull-request! See https://help.github.com/articles/creating-a-pull-request/ for information on this.

After the pull-request has been merged, you can bring yourself up to date with::

    git fetch upstream
    git rebase upstream/main

Trusted Dev User
~~~~~~~~~~~~~~~~

In this case, you have write access to this repository. Any new feature you wish to add will need to be in a new branch::

    git checkout -b feature/newFeature

and then make your commits... then maybe rebase all commits into a few good ones::

    git rebase -i HEAD~N

where ``N`` is the number of commits to rebase. And then rebase with main to make sure your branch is as up-to-date as possible when making the pull-request::

    git pull --rebase origin main

and push your commits to the remote (setting upstream)::

    git push -u origin feature/newFeature

and then submit a pull request by going to `xAODAnaHelpers <https://github.com/UCATLAS/xAODAnaHelpers>`__, finding your branch, and making a pull request (usually shiny green buttons).  When it's been merged, you can run::

    git checkout main
    git pull --rebase origin main
    git remote prune origin

to delete your local copy of the branch after bringing your local copy up to date.

Helpful Suggestions
-------------------

Updating changes
~~~~~~~~~~~~~~~~

If you're on branch ``myBranch`` and you have commits that you want to
push to the remote ``origin`` - the first thing you should do is always
update so you're current::

    git pull --rebase

will do it all. If you want more control, use::

    git fetch
    git rebase origin/main

or::

    git fetch origin
    git rebase origin/main myBranch

.. note::
    - ``git fetch`` will fetch from ``origin`` (see ``git remote -v`` for what that's defined as) by default, but you can explicitly provide a different remote repository.
    - ``git rebase origin/main`` will rebase the current branch you are on.  You can specify another branch if you want.


Changing Author Info
~~~~~~~~~~~~~~~~~~~~

See https://help.github.com/articles/changing-author-info/ for more information.

Renaming lots of tags
~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

    git tag -l "xAODAnaHelpers*" |
    cut -d "-" -f 2-4 |
    while read ref
    do
      git tag "$ref" "xAODAnaHelpers-$ref"
      git tag -d "xAODAnaHelpers-$ref"
      git push origin ":refs/tags/xAODAnaHelpers-$ref"
      git push --tags
    done

Tagging and releasing on svn
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Only a few people should be doing this. Encourage folks to checkout tags
using git. Here are my general steps::

    git clone git@github.com:UCATLAS/xAODAnaHelpers xAHGIT
    svn co svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UChicago/xAODAnaHelpers/trunk xAODAnaHelpers
    mv xAHGIT/.git xAODAnaHelpers/.git
    rm -rf xAHGIT
    cd xAODAnaHelpers

At this point, I need to reset all changes locally (these are due to svn)::

    git reset HEAD --hard

and then I can look at the changes with::

    git status

If I'm happy with things on the ``git`` side, I move over to svn side with::

    svn status

and make sure any new files to add ``svn add newFile`` and remove ``svn del oldFile`` are dealt with. Then I can commit to trunk::

    svn commit -m "Release a tag for xAH"

then I can copy trunk to my new tag ``xx-yy-zz``::

    svn copy svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UChicago/xAODAnaHelpers/trunk svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UChicago/xAODAnaHelpers/tags/xAODAnaHelpers-XX-YY-ZZ

and I should be good to go.

Skimming new test files
~~~~~~~~~~~~~~~~~~~~~~~

In order to skim some new test files, I use athena::

    asetup 20.1.4.7,here

with the following python file executed via ``athena.py skimming.py`` on an ``input.root`` file

.. code-block:: python

    from AthenaCommon.AppMgr import ServiceMgr as svcMgr
    import AthenaPoolCnvSvc.ReadAthenaPool

    svcMgr.EventSelector.InputCollections = ['input.root']

    from GaudiSequencer.PyComps import PyEvtFilter
    filterseq = CfgMgr.AthSequencer("AthFilterSeq")
    #filterseq += PyEvtFilter("MyFilter",evt_list=[18559067]) #will execute main sequence only for these eventnumbers

    # Create a POOL output file with the StoreGate contents:
    from OutputStreamAthenaPool.MultipleStreamManager import MSMgr
    xaodStream = MSMgr.NewPoolRootStream( "StreamXAOD", "xAOD.root" )

    DetDescrVersion = 'ATLAS-R2-2015-03-01-00'
    include("RecExCond/AllDet_detDescr.py")

    # Set up its contents:
    #xaodStream.AcceptAlgs(["MyFilter"])
    xaodStream.GetEventStream().TakeItemsFromInput = True
    #needed to have xAOD readable outside athena (but I thought this is not needed any more?)
    #theApp.CreateSvc += [ "xAODMaker::EventFormatSvc" ]
    xaodStream.AddMetaDataItem(["EventBookkeeperCollection#*"])
    #xaodStream.AddMetaDataItem(["xAOD::EventFormat#EventFormat"])

    theApp.EvtMax=5

which will skim 5 events. I've found that sometimes it will not work because of the wrong geometry information specified, eg::

    GeoModelSvc         ERROR *** *** Geometry configured through jobOptions does not match TagInfo tags! *** ***
    GeoModelSvc          INFO ** Job Option configuration:
    GeoModelSvc          INFO * ATLAS tag: ATLAS-R2-2015-01-01-00
    ...
    GeoModelSvc          INFO ** TAG INFO configuration:
    GeoModelSvc          INFO * ATLAS tag: ATLAS-R2-2015-03-01-00

and I just have to change the line in the above python file::

    DetDescrVersion = 'ATLAS-R2-2015-03-01-00'

to match the correct geometry. Ask `Giordon Stark <https://github.com/kratsg>`__ for more information if you're stuck.

Files with trigger data
~~~~~~~~~~~~~~~~~~~~~~~

On tier3, we have MC::

    /atlas/uct3/data/users/fizisist/xAOD/mc15_13TeV.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.merge.AOD.e3668_s2576_s2132_r6630_r6264/AOD.05403652._000001.pool.root.1

13 TeV data::

    /atlas/uct3/data/users/fizisist/xAOD/data15_comm.00265573.physics_L1Calo.merge.AOD.x331_m1423/data15_comm.00265573.physics_L1Calo.merge.AOD.x331_m1423._lb0179-lb0183._0001.1

which are useful for testing using::

    test_multiAlgo submitDir /atlas/uct3/data/users/fizisist/xAOD <sample> <root file>


Decorations
~~~~~~~~~~~

.. highlight:: c++

As a follow-up on the discussions in yesterday's meeting, xAOD decorations can be assigned and read more efficiently defining an decorators/accessors, since auxdata requires a string-comparison search for the correct branch on every call, whereas the static accessor finds this once and then no longer has the overhead.

You can define a decorator ``static SG::AuxElement::Decorator<char> dec_baseline("baseline");`` which then can be used like ``dec_baseline(input) = isbaseline;`` and then in your code you can replace::

    input.auxdecor<char>("baseline");

by::

    dec_baseline(input);

These are the relevant lines of code inside SUSYObjDef \_xAOD:

-  https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-05-00-14/Root/SUSYObjDef\_xAOD.cxx#L17
-  https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-05-00-14/Root/SUSYObjDef\_xAOD.cxx#L595

In SUSYToolsTester there is also an example of an AuxElement::Accessor like this::

    static SG::AuxElement::Accessor<int> acc_truthType("truthType");
    if (acc_truthType.isAvailable(*trackParticle)  ) muonTruthType = acc_truthType(*trackParticle);

in:

-  https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-05-00-14/util/SUSYToolsTester.cxx#L428

Note that the difference between accessors and decorators is that accessors are for auxdata branches in general but will not let you modify a const object, whereas Decorators permit adding information to const collections.

TString versus std::string
~~~~~~~~~~~~~~~~~~~~~~~~~~

I've noticed that TString slows us down a little bit, so try to use std::string where possible. Code changes and equivalencies look like::

    m_inContainerName.IsNull()
    m_inContainerName.empty()

    m_event->retrieve(jets, m_inContainerName.Data());
    m_event->retrieve(jets, m_inContainerName);

    ANA_MSG_INFO(m_inContainerName.Data());
    ANA_MSG_INFO(m_inContainerName);


Creating a new xAH::Algorithm
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you are planning to write an :cpp:class:`xAH::Algorithm`, there are two requirements you must abide by to fit within the |xAH| ecosystem.

#. Only allow empty constructors, no parameters or arguments passed in.
#. Constructors must initialize an :cpp:class:`xAH::Algorithm` instance passing in the name of itself::

     ExampleClass :: ExampleClass() : Algorithm("ExampleClass") {}

The first requirement is necessary to make sure streamable code (such as EventLoop) can handle and set up your algorithms correctly when submitting jobs. The second requirement is currently necessary for |xAH| to keep track of the number of instances of a given class that has been created. This is a registry book-keeping operation that allows users to write smarter algorithms, the kind that know how many instances of itself have been created!

Adding and Initializing Tools
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is albeit a litle bit trickier for anyone new to how Athena tools work. First, I'll provide header and source code blocks showing an example for a tool, and then I will explain the concepts.

**Header File**::

  // external tools include(s):
  #include "AsgTools/AnaToolHandle.h"
  #include "JetCalibTools/IJetCalibrationTool.h"

  class JetCalibrator : public xAH::Algorithm {

    public:
      //...

    private:
      // tools
      asg::AnaToolHandle<IJetCalibrationTool> m_JetCalibrationTool_handle{"JetCalibrationTool", this};//!

  }

**Source File**::

  // tools
  #include "JetCalibTools/JetCalibrationTool.h"

  //...

  EL::StatusCode JetCalibrator :: initialize () {
    //...

    // initialize jet calibration tool
    ANA_CHECK( m_JetCalibrationTool_handle.setProperty("JetCollection",m_jetAlgo));
    //... other setProperty() calls and other logic can be in here for tool configuration
    ANA_CHECK( m_JetCalibrationTool_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_JetCalibrationTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_JetCalibrationTool_handle);
  }

  EL::StatusCode JetCalibrator :: execute () {
    //...
    m_JetCalibrationTool_handle->apply(*jet);
    //...
  }

  //...

**Header Discussion**

First, let's talk about the header file. You need to include the header file for the tool handles ``AsgTools/AnaToolHandle.h``. As this is a templated method, you really don't to try and forward-declare this or you're gonna have a bad time. Next, you'll want to include the header file for the tool's interface class, e.g. ``JetCalibTools/IJetCalibrationTool.h``.

.. note::
  To find the correct header file for a tool's interface, look in the header file for the tool itself, e.g. ``JetCalibTools/JetCalibrationTool.h``, and cross-check by looking at the classes the tool inherits from. For example, ``JetTileCorrectionTool`` has the ``IJetTileCorrectionTool`` interface class because in its header file::

    class JetTileCorrectionTool : public virtual IJetTileCorrectionTool,
                                  public asg::AsgMetadataTool


You might wonder why we don't just include the tool's header file in our header file. One choice is that the interface header file is smaller and easier to compile quickly. This is roughly equivalent to forward-declaring our tool, where we only include the header file for our tool in the source and put a ``class ClassName;`` in the header.

Lastly for the header, we make the tool handle a private member of our class. Make sure that this gets constructed with a type only by specifying the tool itself, e.g. ``JetCalibrationTool``. By adding the ``this`` parameter, we make sure that the tool handle is indeed made as a private tool for the given algorithm.

.. note:: We will prefer the suffix ``_handle`` to refer to the fact that the variable is a tool handle in |xAH|.

**Source Discussion**

Next, looking at the source code... we include the header file for our tool. Although this may not always be needed, it is good practice to help others figure out where the tool is. As of writing this documentation, the interface and the tool may be defined in different packages! Moving on, we will want to put tool initializations in ``initialize()`` as this will only get called on files that have events. Files without events will not create a tool, conserving memory and processing power.

If you need to use/retrieve a tool created in another class, you will need to have the same name in both places for the ToolHandle to find it, and you need to make sure the tool isn't made private (do't use ``this`` for the second parameter for initializatiton).

If you don't set a name for the tool, only a type, the default name is the type. For example::

    asg::AnaToolHandle<IJetCalibrationTool> test_handle{"JetCalibrationTool", this};
    ANA_MSG_INFO(test_handle.name()); // will output "JetCalibrationTool"

.. note::

  In ASG Software, tools created through AnaToolHandle can be found in the ToolStore via preprending ``ToolSvc.`` to the name of the tool::

    asg::ToolStore::contains<Trig::TrigDecisionTool>("ToolSvc."+ m_trigDecTool_handle.name())

  This is a slight gotcha that will trip up people. Because of this, |xAH| prefers the convention of using :code:`isUserConfigured()` instead as this doesn't need the additional ``ToolSvc.`` prepended to the tool name to look it up!

If it has :code:`isUserConfigured()==0` (e.g. "not configured before": a tool with that type and name has not been created), then let's go ahead and configure it with ``setProperty()``!  One thing you should **always** do is set the output level of the tool ``OutputLevel``. It is usually best to set it to the same output level that the algorithm is configured to ``msg().level()`` and is probably the safest prescription.

.. note::
  For setting properties or managing tools through the tool handle, you access functions through the dot (``.``) operator. For using the tool, you access functions through the arrow (``->``) operator.

If a tool handle has been configured previously, but not initialized (such as using a tool handle of the same type and name as a previously created tool handle), then all :code:`setProperty()` calls will be further ignored. I can demonstrate this with a neat code example::

  // set up the players
  asg::AnaToolHandle<IJetCalibrationTool> alice{"JetCalibrationTool/MyName"};
  asg::AnaToolHandle<IJetCalibrationTool> bob  {"JetCalibrationTool/MyName"};

  // set configurations on the first handle
  ANA_CHECK(alice.setProperty("p1", v1)); // will set the underlying tool MyName->p1 = v1
  ANA_CHECK(alice.setProperty("p2", v2)); // will set the underlying tool MyName->p2 = v2
  ANA_CHECK(alice.retrieve()); // creates the tool MyName

  ANA_CHECK(bob.setProperty("p1", v9)); // will be ignored as bob.isUserConfigured() == 1 [alice owns the tool]
  ANA_CHECK(bob.setProperty("p3", v3)); // will be ignored as bob.isUserConfigured() == 1 [alice owns the tool]
  ANA_CHECK(bob.retrieve()); // retrieves the existing tool MyName

AnaToolHandle will also not let us change the configuration of a previously initialized tool (one which :code:`handle.retrieve()` has been called on). In this case, the tool has been :code:`initialized`. Continuing the code example from before, if you were annoyed that the :code:`setProperty()` calls were ignored, you might try setting it again on ``alice``::

  ANA_CHECK(alice.setProperty("p3", v3)); // will crash as alice.isInitialized() == 1 [alice already created its tool]

Finally, we :code:`retrieve()` (:code:`initialize()`) the tool of the given type and name from the tool store. :code:`retreive()` and :code:`initialize()` are synonyms and will almost always create a new tool. The only two exceptions are if the user configured the tool (:code:`isUserConfigured()==1`) or if another ToolHandle created the tool as a public tool and holds on to it. But that's it, the memory will be managed for you and you do not need to delete it or do anything else but use it in your code!

.. note::
  Did you get a bus error, segfault, or abort in the code because of the tools? If so, it is most likely due to a typo in the tool's header file. Please identify which tool causes the error and file an issue so we can inform the tool developers that their tool needs to be fixed. In the meantime, this can be fixed using a macro::

    ANA_CHECK( ASG_MAKE_ANA_TOOL(m_JVT_tool_handle, CP::JetJvtEfficiency));

  An example of a reported issue for the above tool is here: https://its.cern.ch/jira/browse/ATLASG-1214.

Check if a tool exists and reuse it (Trig::TrigDecisionTool)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The TrigDecisionTool is a special case that needs attention. This tool is unique in that the templaed methods require us to use the tool as its own interface. It is also a singleton which means it will complain heavily if it detects more than one instance of itself. How do we deal with this in |xAH|?

**Header File**::

  // external tools include(s):
  #include "AsgTools/AnaToolHandle.h"
  #include "TrigDecisionTool/TrigDecisionTool.h"

  class MyAlgorithm : public xAH::Algorithm {

    public:
      /** @brief trigDecTool name for configurability if name is not default.  If empty, use the default name. If not empty, change the name. */
      std::string m_trigDecTool_name{""};

    private:
      /** @brief Trigger decision tool.

      If you need to use a TDT that was previously created before this algorithm with a different name, set the name in m_trigDecTool_name.
      */
      asg::AnaToolHandle<Trig::TrigDecisionTool>     m_trigDecTool_handle{"Trig::TrigDecisionTool"};                         //!
  };


**Source File**::

  EL::StatusCode MyAlgorithm :: initialize(){

    // Grab the TrigDecTool from the ToolStore
    if(!m_trigDecTool_handle.isUserConfigured()){
      ANA_MSG_FATAL("A configured " << m_trigDecTool_handle.typeAndName() << " must have been previously created! Double-check the name of the tool." );
      return EL::StatusCode::FAILURE;
    }
    ANA_CHECK( m_trigDecTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);

  }

The above is an example of how one designs an algorithm that requires the TrigDecisionTool and will crash if it cannot find it. It also prints the name of the tool it is using to make it much easier for a user to debug. By convention in |xAH|, :cpp:member:`BasicEventSelection::m_trigDecTool_name` will default to :code:`"xAH::TrigDecTool"`. All algorithms follow this default if they need the trigger decision tool. If there is an external algorithm that creates it and you want |xAH| to pick it up instead of creating one, this can be done by setting :code:`m_trigDecTool_name` to a non-empty value and you're good to go. For example, :cpp:class:`BasicEventSelection` will create a trigger decision tool if it does not exist::

  ANA_CHECK( m_trigDecTool_handle.setProperty( "ConfigTool", m_trigConfTool_handle ));
  ANA_CHECK( m_trigDecTool_handle.setProperty( "TrigDecisionKey", "xTrigDecision" ));
  ANA_CHECK( m_trigDecTool_handle.setProperty( "OutputLevel", msg().level() ));
  ANA_CHECK( m_trigDecTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);

so that if such a tool already was created before ``BasicEventSelection`` tries to create it, it will retrieve it (and the :code:`setProperty()` calls will be ignored). If it has not been created/configured before, it will configure and then create the tool. No extra logic needed on the users' part.
