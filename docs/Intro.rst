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
