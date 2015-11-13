Common Issues
=============

Missing Metadata Information
----------------------------

It has been noticed that some of the latest derived xAODs have missing metadata info due to some bug in Derivation Framework. If you are incurring in a nasty crash at runtime, make sure you have set the name of the derivation property DerivationName property of BasicEventSelection.  If that does not work then switched off the configuration flag::

    UseMetadata False

and try again.

