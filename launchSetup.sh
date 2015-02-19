rcSetup Base,2.0.24
rc checkout_pkg AssociationUtils
cd AssociationUtils
patch -p0 -i ../xAODAnaHelpers/data/AssociationUtils.diff
cd ..
rc clean
rc find_packages
rc compile