#!/bin/tcsh

set myTag="rcSetup-test"
set BaseRel="2.0.11"
set packName=CPAnalysisExamples
set svn_pack="svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/AnalysisCommon/CPAnalysisExamples/tags/CPAnalysisExamples-00-00-22"

set myName=$0
set myDir=`dirname $myName`
set rcSetup=$myDir
set csh_rcSetup=$myDir/rcSetup.csh

# print the version of rcSetup
# ----------------------------
echo
echo  "Testing ::`source $csh_rcSetup -V`::"
echo

# listing ASG binary releases
# ---------------------------
set jobName="listing ASG binary releases"
source $csh_rcSetup  -r
if ( $? != 0 ) then
   echo
   echo "::${myTag}:: Error in $jobName"
   echo
   exit 1
else
   echo
   echo "::${myTag}:: Success in $jobName"
   echo
endif


# listing ASG releases in SVN
# ----------------------------
set jobName="listing ASG releases in SVN"
source $csh_rcSetup  -q -d -r
if ( $? != 0 ) then
   echo
   echo "::${myTag}:: Error in $jobName"
   echo
   exit 1
else
   echo
   echo "::${myTag}:: Success in $jobName"
   echo
endif


# setup of ASG Base
# -----------------------
set jobName="setup of ASG Base,$BaseRel"
source $csh_rcSetup  Base,$BaseRel
if ( $? != 0 ) then
   echo
   echo "::${myTag}:: Error in $jobName"
   echo
   exit 1
else
   echo
   echo "::${myTag}:: Success in $jobName"
   echo
endif


# checkout
# --------
svn co $svn_pack $packName
if ( $? != 0 ) then
   echo
   echo "::${myTag}:: Error in checking out $packName from SVN failed, quit now"
   exit 1
endif

# compilation
# -----------
set jobName="compiling $packName"
rc clean
rc find_packages
rc compile
if ( $? != 0 ) then
   echo
   echo "::${myTag}:: Error in $jobName"
   echo
   exit 1
else
   echo
   echo "::${myTag}:: Success in $jobName"
   echo
endif

# loading library
# ---------------
set jobName="loading library of $packName"
set rootScript=load-${packName}.C
cat > $rootScript << EOF
Int_t load_${packName}()
{
  const char* rootcoredir = gSystem->Getenv("ROOTCOREDIR");
  gROOT->Macro(TString::Format("%s/scripts/load_packages.C", rootcoredir));
  Int_t ret = gSystem->Load("lib${packName}.so");
  if (ret == 1) return 0;
  else return -1;
}
EOF

root -q -b -l $rootScript
if ( $? != 0 ) then
   echo
   echo "::${myTag}:: Error in $jobName"
   echo
   exit 1
else
   echo
   echo "::${myTag}:: Success in $jobName"
   echo
endif

echo "::${myTag}:: Success in passing the whole test"
