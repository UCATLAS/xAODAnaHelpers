Algorithms
==========

Each algorithm will retrieve a container from either TEvent or TStore
and if an output container is specified, it is saved to the TStore
object such that the following algorithms can access the container.
TStore will manage the memory for the user.

Event Selection
---------------

(moved to BasicEventSelection.h)

Jet Related
-----------

HLTJetGetter
~~~~~~~~~~~~~

The HLTJetGetter Algorithm retrieves jets from the TrigDecisionTool.
The output is a shallow copy of the trigger feature that is requested
via the combination of the name of the jet collection (m_inContainerName) and
the jet trigger that is selected m_triggerList. The name of the copy
is chosen via the m_outContainerName.
The input container name should be given without any HLT_xAOD__JetContainer prefix
if the collection comes directly from the xAOD.
The list of triggers must be given as a regular expression using an or (|) if
more than a trigger is requested. To request all triggers, use \*.


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
https://xaodanahelpers.readthedocs.io/en/latest/JetCalibrator.html

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
Uncertainty to use, MC20 or MC21

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

JetSelector
~~~~~~~~~~~

Applies cuts on the input jet collection(s). The jets can be decorated
with the decision of the cuts. A new xAOD::JetCollection can be made
from the ones passing the cuts.

The JVT selection is OFF by default, it can be turned ON with ```m_doJVT```.
The default algorithm is NNJvt. If another algorithm is needed, use corresponding index for the enum here: https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/PhysicsAnalysis/Interfaces/JetAnalysisInterfaces/JetAnalysisInterfaces/IJetJvtEfficiency.h#0022 (note: this link points to the latest r22 version, i.e. master, if a release is used, please check the corresponding enum for the given release: https://gitlab.cern.ch/atlas/athena/-/tags?search=release%2F22.2&sort=updated_desc)
The default JVT working point is ```FixedEffPt```, it can be changed with ```m_WorkingPointJVT```.
JVT is no longer recalculated in JetCalibrator, it is now done in JetSelector.
It can be disabled by setting ```m_recalculateJvtScores``` to ```false```.
Furthermore, the JVT algorithm is now NNJvt (default for R22).


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

Produces a container of muons decorated with efficiencies and scale factors.
This container is not simply the one in input, but is a deepCopy of that.
This allows flexibility in decorating muons of systematically altered containers.
The output container has a configurable name and is only created for MC events.
When working with systematic uncertainties, a list of systematics is passed
to this algorithm *m_inputAlgoSystNames*\. This algorithm supports comma
separated lists as inputs, which will be considered as a unique list. The
systematic names in this list will be looked for to retrieve the muon containers
to decorate. Systematic variations on the decorations themselves might be
unnecessary for all systematic muon containers and might only be considered
for the nominal container (need of deepCopy). This is the default configuration.
Otherwise the  option *m_decorateWithNomOnInputSys*\ can be set to false.
The algorithm features the option *m_sysNamesForParCont*\ which is a list of systematic
names. For each of them, a copy of the nominal muon container is put in the
store carrying the name of the systematic. These containers are only decorated
with the nominal efficiencies and scale factors. The use case of this are MET
systematics for which one does not want systematic variations on efficiencies,
but still wants to retrieve a nominal muon container in the tree algo. Retrieving
the nominal would indeed carry all the unnecessary uncertainties.


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

See the MuonEfficiencyCorrector

ElectronSelector
~~~~~~~~~~~~~~~~

ElectronHists and ElectronHistsAlgo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Tau Related
-----------

TauCalibrator
~~~~~~~~~~~~~

TauSelector
~~~~~~~~~~~
Similar to other selectors. Tau identification/selection algorithms working points are 
decorated onto the tau automatically. Tha flag m_decorateWithTracks controls the decoration
of the tau track information. It can be dumped in the ntuples by specifying the trackparams
flag in the tau details string of the TreeAlgo.

TauEfficiencyCorrector
~~~~~~~~~~~~~~~~~~~~~~~~~~~
The TauEfficientyCorrector provides one cumulative decoration with one SF corresponding to
the combination of working points used for the tau selection/identification algorithms.
Several initialisations of the algorithm are needed with different combinations in order 
to dump in the ntuples different combined working points. Tau trigger SFs are saved separately
and wrt said cumulative working point which has to be specified in the initialisation of a new
instance of the algorithm together with the trigger menu.


TauJetMatching
~~~~~~~~~~~~~~
This algorithm is introduced to match an arbitrary and configurable collection of jets 
with the TauJet object. This is useful for cases where the tau seedJet (LC topo jet) 
is not available in xAOD but one would need to get a handle on some original jet info. 
The algorithm has a configurable DeltaR matching criterion and for now decorates taus 
with the matched jet width. It should be executed before OLR. It can be used
before tau selection and after tau calibration.


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
