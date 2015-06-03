<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [xAODAnaHelpers (xAH)](#xaodanahelpers-xah)
  - [Current Working Version](#current-working-version)
  - [Migrating](#migrating)
    - [From 00-00-04 to 00-00-05](#from-00-00-04-to-00-00-05)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# xAODAnaHelpers (xAH)

The xAOD analysis framework, born out of ProofAna...or not.

## Current Working Version: **00-03-00**

This version uses AB 2.3.12.
The 00-03-00 tag works well with this realease.
After checking out the package please execute a helper script to get any additional packages needed.
These packages are needed to temporarily solve issues that will be resolved in up coming releases.
For the current release, please do:
```
python xAODAnaHelpers/scripts/checkoutASGtags.py 2.3.12
```
**Caveat** 

It has been noticed that some of the latest derived xAODs have missing metadata info due to some bug in Derivation Framework. If you are incurring in a nasty crash at runtime, make sure you have switched off the configuration flag 

```
UseMetadata False
```

and try again

## Migrating

### From 00-00-04 to 00-03-00

The constructors for the algorithms have all been changed to default constructors. We have also centralized a lot of code so that `EL::Algorithm` is replaced by `xAH::Algorithm` where possible. For updating constructors in your code, replace

```
BasicEventSelection* baseEventSel = new BasicEventSelection(  "baseEventSel", localDataDir+"baseEvent.config");
```

with

```
BasicEventSelection* baseEventSel             = new BasicEventSelection();
baseEventSel->setName("baseEventSel")->setConfig(localDataDir+"baseEvent.config");
```

See [xAODAnaHelpers/Algorithm.h](xAODAnaHelpers/Algorithm.h) for more details on the uniform constructors.
