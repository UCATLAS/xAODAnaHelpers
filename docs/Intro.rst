Introduction
============

This package is meant to be the minimal needed to use the CP tools
properly to calibrate, select, and correct the physics objects used for
most physics analyses. Each step of the analysis chain is done by an
EL::Algorithm which utilizes TStore to pass information to the Algos
down the chain. The final product can be a TTree, histograms, or a mini
xAOD (coming soon!). The philosophy adopted is not to remake the EDM or
to alter it but to make minimal wrapper around CP tools to help users
configure them properly and connect the full chain without much hassle.
To this end, some details are hidden for the user and set automatically
in the tools. As much as possible we used the same names as is shipped
with the xAOD objects or the CP tools themselves. The user is not meant
to learn a new EDM but rather to learn the minimal needed to start doing
the fun stuff - Physics!!

.. include:: Installing.rst

Background
----------

An analysis is done in steps with a ``EL::Algorithm`` running for each.
For example, one algo is used to calibrate the jet collection, another
to apply some selection cuts, and then a third algorithm can contain
your analysis code to calculate something with the jets or one of the
general plotting algorithms that will fill a configurable set of plots.
A second jet calibrator and selector can be added to have a second jet
collection at the same time. A base class for a tree has also been
created with some standard branches and a way for the user to add more
as well. The tree is configurable with the same options as the
histograming classes - with a string of categories of interest. Each
algorithm can be configured via a text file read by TEnv. Example for
all are given and one can look for the "configure" function to see what
options are available (also noted below). Development changes and help
requests can be obtained on the e-group "atlas-sw-xAODAnaHelpersFW" or
directly here on GitHub.

A word on **systematics**. When the object itself is altered (i.e. JES
calibration and JES systematics) a new collection is made and put into
TStore. The name of the nominal collection after calibration is set from
the config file. The name of the systematically varied collection uses
the same name plus the name of the systematic directly from the CP tool.
The next algo in the chain using these objects needs to know which
collections where created. To avoid hardcoding things and all that, when
the systematics are applied a vector is created containing the names of
each systematic. Downstream, algos pick up this vector then know which
collections to run over. Each Algo will loop over all collections before
going to the next step. If selectors are configured with limits in the
number of events passing the cuts, only collections passing the cuts
will be passed to algos downstream. If none pass, the next event is
analyzed.

Helpers Available
=================

Each algorithm will retrieve a container from either TEvent or TStore
and if an output container is specified, it is saved to the TStore
object such that the following algorithms can access the container.
TStore will manage the memory for the user.

Event Selection
---------------

BasicEventSelection
~~~~~~~~~~~~~~~~~~~

This algorithm performs the very basic event selection. This should be
the first algo in the algo chain. It can create weighted and unweighted
cutflow objects to be picked up downstream by other xAH algos, and your
own. The selection applied in **data** only is:

1. GRL ( can be turned off )
2. LAr Error
3. Tile Error
4. Core Flag

For **MC** only, the pileup reweight can be applied.

In **both** data and simulation the following cuts are applied 1. the
highest sum pT^2 PV has 2 or more tracks 2. trigger requirements

For derivations the MetaData can be accessed and added to the cutflow
for normalization

The parameters to control the trigger and all cuts in general are
described in the header documentation:
https://github.com/UCATLAS/xAODAnaHelpers/wiki/xAH\_BasicEventSelection.h

Jet Related
-----------

JetCalibrator
~~~~~~~~~~~~~

The JetCalibrator Algorithm handles the calibration, cleaning, and
common uncertainties associated with Jets. It interfaces with many
JetETMiss packages. The output is a shallow copy of the input jet
container (i.e. a new xAOD::JetCollection). A separate shallow copy is
made for each systematic variation requested, and the names of the
containers are saved in a vector for downstream algorithms to use. The
algorithm does not apply any selection to the jets.

The parameters to control the Algorithm are described in the header
documentation:
https://github.com/UCATLAS/xAODAnaHelpers/wiki/xAH\_JetCalibrator.h

Calibration
^^^^^^^^^^^

Jets are calibrated using JetCalibTools. The option CalibSequence
determines the calibration sequence to be applied. "Insitu" in
automatically added to data samples if it is not provided, and MC jobs
will fail if it is included. Config file options can be provided with
CalibConfigData, CalibConfigAFII, and CalibConfigFullSim. The config
variable to use is determined from metadata.

1. Sort - Sort the output jets by pT
2. SetAFII - Force AFII configurations (in case metadata isn't working)
3. InputContainer - Jet Collection to use
4. OutputContainer - Output Jet Collection for calibrated jets
5. JetAlgorithm - Input Jet Algorithm
6. OutputAlgo - Output Jet Algorithm

JES/JER Uncertainties
^^^^^^^^^^^^^^^^^^^^^

Outputs new containers for each systematic variation.

For JES: Uses JetUncertaintiesTool. JES is different for AFII, so
m\_setAFII may be required if metadata isn't working. 1. JESUncertConfig
- Configuration file for JES 2. JESUncertMCType - Type of JES
Uncertainty to use, MC15 or MC12

For JER: Uses JERSmearingTool 1. JERUncertConfig - Configuration file
for JER 2. JERFullSys - Run full list of systematics for data and MC.
Not currently recommended by JER Group (10/08/2015). 3. JERApplyNominal
- Apply a nominal JER Smearing.

Cleaning
^^^^^^^^

Cleaning decisions are saved as auxiliary data; No selections are made
to the container. The decision is saved as an auxdecor with names like
clean\_passLooseBadUgly clean\_pass Settings: 1. JetCleanCutLevel - Cut
Level, may be LooseBad or TightBad 2. JetCleanUgly - Clean Ugly Jets (in
gap region) 3. SaveAllCleanDecisions - Save all four decisions
(LooseBad, LooseBadUgly, TightBad, TightBadUgly) 4. CleanParent - Apply
cleaning decision to the jet's parent

JVT Recalculation
^^^^^^^^^^^^^^^^^

If RedoJVT is set, the JVT is recalculated.

JetSelector
~~~~~~~~~~~

Applies cuts on the input jet collection(s). The jets can be decorated
with the decision of the cuts. A new xAOD::JetCollection can be made
from the ones passing the cuts.

JetHists and JetHistsAlgo
~~~~~~~~~~~~~~~~~~~~~~~~~

JetHists is a class of histograms for jet variables. In the constructor
one gives a name which is used as the first half of the name of all the
histograms in the class and the detail string which determines which
plots are made. One can declare a JetHists object in the event selection
algorithm and fill the hists with every jet in a collection or pick jets
by hand to fill histograms.This is done in the example event selection
described below. JetHistsAlgo is a algorithm that holds a JetHists
object and fills it with every jet in the user specified container.

BJetEfficiencyCorrector
~~~~~~~~~~~~~~~~~~~~~~~

This Algo wraps two flavor tagging tools - one returns the decision on
if the jet is tagged or not for the given operating point (OP) and the
other returns the efficiency scale factors needed for MC only. Like
other Algos, "All" gives all the systematic variations. Two decorations
are added to the input jets: 1. The B-Tag decision. Decoration name
(default)
"BTag\_:math:`{OP}"  2. The B-Tag efficiency scale factor. If the jet fails the cut the inefficiency scale factor is saved. Decoration name (default) "BTAG_SF_`\ {OP}".
This decoration is not added if the OP has not been calibrated.

The available **calibrated** operating points in the `August 2015 CDI
file <https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTagCalib2015#Pre_Recommendation_August_2015>`__
for *fixed* b-jet efficiencies: \* FixedCutBEff\_60 \* FixedCutBEff\_70
\* FixedCutBEff\_77 \* FixedCutBEff\_85

and the *fixed* cut **un-calibrated** operating points are: \*
FixedCutBEff\_30 \* FixedCutBEff\_50 \* FixedCutBEff\_80 \*
FixedCutBEff\_90

Finally, the *flat* efficiency **un-calibrated** operating points are:
\* FlatBEff\_30 \* FlatBEff\_40 \* FlatBEff\_50 \* FlatBEff\_60 \*
FlatBEff\_70 \* FlatBEff\_77 \* FlatBEff\_85

These decorations can be added to a TTree via HelpTreeBase. The info
switch looks for "*sfFTagFix*\ " and "*sfFTagFlt*\ " for the fixed and
flat efficiency OPs respectively. "*sfFTagFix607785*\ " will try to add
the decision and efficiency/inefficiency scale factors for the fixed 60,
70, and 85% b-jet efficiency OPs to the tree. The user must also create
and run BJetEfficiencyCorrector for each OP.

Note: To those paying attention. Currently the two flavor tagging tools
use different names for the same operating point. This is handled
internally in xAH until the flavor tagging group harmonizes the two.

Muon Related
------------

MuonCalibrator
~~~~~~~~~~~~~~

MuonEfficiencyCorrector
~~~~~~~~~~~~~~~~~~~~~~~

MuonSelector
~~~~~~~~~~~~

MuonHists and MuonHistsAlgo
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Electron Related
----------------

ElectronCalibrator
~~~~~~~~~~~~~~~~~~

ElectronEfficiencyCorrector
~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElectronSelector
~~~~~~~~~~~~~~~~

ElectronHists and ElectronHistsAlgo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

HelperFunctions
---------------

HelperClasses
-------------

NTuple Creation
---------------

HelpTreeBase is a class (not an algorithm) that creates and fills a
TTree. When adding an object type i.e. jet, muon, or event level
quantities, it can be easily configured with "info switches" ( See
HelperClasses ) that take a space separated list of categories.
