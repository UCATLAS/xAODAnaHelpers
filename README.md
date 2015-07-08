<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [xAODAnaHelpers (xAH)](#xaodanahelpers-xah)
  - [ASG Release for development: **2.3.18**](#asg-release-for-development-2-.-3-.-18)
  - [Current Working Version: **00-03-04**](#current-working-version-00-03-04)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# xAODAnaHelpers (xAH)

The xAOD analysis framework, born out of ProofAna...or not.

## Current Working Version: **00-03-08**

The 00-03-08 tag is for use with **Base,2.3.18**.

## Previous Working Version: **00-03-04**

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

**Caveat**

It has been noticed that some of the latest derived xAODs have missing metadata info due to some bug in Derivation Framework. If you are incurring in a nasty crash at runtime, make sure you have set the name of the derivation property DerivationName property of BasicEventSelection.  If that does not work then switched off the configuration flag

```
UseMetadata False
```

and try again.
