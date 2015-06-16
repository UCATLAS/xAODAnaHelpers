#!/bin/bash

myTag="rcSetup-test"
BaseRel="2.0.11"
packName=CPAnalysisExamples
svn_pack="svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/AnalysisCommon/CPAnalysisExamples/tags/CPAnalysisExamples-00-00-22"

# get the directory of this script, architecture, and lcg external dir
#--------------------------------------------------------------------
myName="${BASH_SOURCE:-$0}"
myDir=`dirname $myName`
sh_rcSetup=$myDir/rcSetup.sh

# print the version of rcSetup
# ----------------------------
echo -e "\nTesting ::$(source $sh_rcSetup -V)::\n"

# listing ASG binary releases
# ---------------------------
jobName="listing ASG binary releases"
source $sh_rcSetup  -r
if [ $? != 0 ]; then
   echo -e "\n::${myTag}:: Error in $jobName\n"
   exit 1
else
   echo -e "\n::${myTag}:: Success in $jobName\n"
fi


# listing ASG releases in SVN
# ----------------------------
jobName="listing ASG releases in SVN"
source $sh_rcSetup  -q -d -r
if [ $? != 0 ]; then
   echo -e "\n::${myTag}:: Error in $jobName\n"
   exit 1
else
   echo -e "\n::${myTag}:: Success in $jobName\n"
fi


# setup of ASG Base
# -----------------------
jobName="Setup of ASG Base,$BaseRel"
source $sh_rcSetup  Base,$BaseRel
if [ $? != 0 ]; then
   echo -e "\n::${myTag}:: Error in $jobName\n"
   exit 1
else
   echo -e "\n::${myTag}:: Success in $jobName\n"
fi


# checkout
# --------
svn co $svn_pack $packName
if [ $? != 0 ]; then
   echo -e "\n::${myTag}:: Error in checking out $packName from SVN failed, quit now"
   exit 1
fi

# compilation
# -----------
jobName="compiling $packName"
rc clean
rc find_packages
rc compile
if [ $? != 0 ]; then
   echo -e "\n::${myTag}:: Error in $jobName\n"
   exit 1
else
   echo -e "\n::${myTag}:: Success in $jobName\n"
fi

# loading library
# ---------------
jobName="loading library of $packName"
rootScript=load-${packName}.C
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
if [ $? != 0 ]; then
   echo -e "\n::${myTag}:: Error in $jobName\n"
   exit 1
else
   echo -e "\n::${myTag}:: Success in $jobName\n"
fi

echo "::${myTag}:: Success in passing the whole test"
