Development
===========

.. warning::
    Never push to master. Always create a new branch for changes, rebase your branch with master ``git pull --rebase origin master`` and use the branch for creating a pull-request to merge with master. This keeps master mergeable for everyone for all development.

.. toctree::
   :maxdepth: 2

   HowToDocumentation
   Issues

Development Workflow
--------------------

Changes should be tested properly ( "it compiles" is not sufficient ). We use the ``git rebase`` workflow.

New User
~~~~~~~~

This is for users who do not have write access to ``UCATLAS/xAODAnaHelpers`` to make branches. Instead, they fork and write their changes to their own repository and submit pull-requests.

-  For *very* new users, you may want to setup SSH key access to your personal repository. To do this, follow the instructions at `Generating SSH keys <https://help.github.com/articles/generating-ssh-keys/>`_. So you go here: https://github.com/UCATLAS/xAODAnaHelpers/ and just click the 'Fork' at top right. This forks a copy into your account (``yourAccount/xAODAnaHelpers``). Next, clone it. Set the upstream::

    git clone git@github.com:yourAccount/xAODAnaHelpers
    cd xAODAnaHelpers
    git remote add upstream git@github.com:UCATLAS/xAODAnaHelpers

Make your changes, and when you are ready to submit a pull-request, do
the following first::

    git fetch upstream
    git rebase upstream/master

to make sure your code is up to date with the forked repository. You will want to rebase all of your changes into a single commit if you wish, and that can be done via::

    git rebase -i HEAD~N

where ``N`` is the number of commits to rebase. Then you just follow the instructions. Take care not to rebase through commits that are already on master of the upstream repo. Then submit a pull-request! See https://help.github.com/articles/creating-a-pull-request/ for information on this.

After the pull-request has been merged, you can bring yourself up to
date with::

    git fetch upstream
    git rebase upstream/master

Trusted Dev User
~~~~~~~~~~~~~~~~

In this case, you have write access to this repository. Any new feature you wish to add will need to be in a new branch::

    git checkout -b feature/newFeature

and then make your commits... then maybe rebase all commits into a few good ones::

    git rebase -i HEAD~N

where ``N`` is the number of commits to rebase. And then rebase with master to make sure your branch is as up-to-date as possible when making the pull-request::

    git pull --rebase origin master

and push your commits to the remote (setting upstream)::

    git push -u origin newFeature

and then submit a pull request by going to `xAODAnaHelpers <https://github.com/UCATLAS/xAODAnaHelpers>`__, finding your branch, and making a pull request (usually shiny green buttons).  When it's been merged, you can run::

    git checkout master
    git pull --rebase origin master
    git remote prune origin

to delete your local copy of the branch after bringing your local copy up to date.

Helpful Suggestions
-------------------

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

.. code:: python

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

    Info("%s", m_inContainerName.Data());
    Info("%s", m_inContainerName.c_str());

