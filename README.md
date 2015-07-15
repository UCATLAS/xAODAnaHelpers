# xAODAnaHelpers (xAH)

The xAOD analysis framework, born out of ProofAna...or not.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Versioning](#versioning)
  - [Current](#current)
  - [Previous](#previous)
- [Common Issues](#common-issues)
  - [Missing Metadata Information](#missing-metadata-information)
- [Who uses us?](#who-uses-us)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Versioning

If you need to bump up the version of the AnalysisBase being used, please make sure you update the [travis.yml](.travis.yml) file first.

### Current
Please try to keep changes to a minimum until after EPS, July 23rd 2015.

Current AB is **Base,2.3.19**

The 00-03-08 tag is for use with **Base,2.3.18**.

### Previous

The 00-03-04 tag works well with **Base,2.3.14, Base,2.3.15**. After checking out the package please execute a helper script to get any additional packages needed.
These packages are needed to temporarily solve issues that will be resolved in up coming releases.
For the current release, please do:
```
python xAODAnaHelpers/scripts/checkoutASGtags.py 2.3.14
```

or

```
python xAODAnaHelpers/scripts/checkoutASGtags.py 2.3.15
```

## Common Issues

### Missing Metadata Information

It has been noticed that some of the latest derived xAODs have missing metadata info due to some bug in Derivation Framework. If you are incurring in a nasty crash at runtime, make sure you have set the name of the derivation property DerivationName property of BasicEventSelection.  If that does not work then switched off the configuration flag

```
UseMetadata False
```

and try again.

## Who uses us?

The following list are packages / analyses searches that depend on xAH. We thank them for their hard work and hope they continue to use us!

- [ttH->multileptonic final state](https://github.com/mmilesi/HTopMultilepAnalysis)
  - HTop - former HSG8 group
- [dijet](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ExoticDijets2015)
- [multijet](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ExoticMultiJetRun2)
- [hh->4b](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/XtoYYtobbbbRun2)
- [VBF + invisible](https://twiki.cern.ch/twiki/bin/view/AtlasProtected/HiggsInvisibleRun2)
- [g->tt susy multi-b-jet](https://github.com/kratsg/TheAccountant)
- Jet/MET
- Jet Cleaning
- [jet inputs to reconstruction](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetInputsCONF2015)
- Punch-through studies
- [Multijet balance](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/MultijetBalanceRunIIFramework)
- [Standard Model inclusive jet cross section](https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/InclusiveJetCrossSection)
