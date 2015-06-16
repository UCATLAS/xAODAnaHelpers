#!/bin/bash

if [ "X" != "X$BASH_SOURCE" ]; then
  shopt -s expand_aliases
fi
alias py_readlink="python -c 'import os,sys;print os.path.realpath(sys.argv[1])'"

# get the directory of this script
#---------------------------------
myName_orig="${BASH_SOURCE:-$0}"
myDir_orig=`dirname $myName_orig`
myName=`py_readlink $myName_orig`
myDir=`dirname $myName`

currDir=$PWD

export alias_rc="source $myName"
eval echo $(alias rcsetup 2>/dev/null | cut -d"'" -f2) | grep -q "source\s*$myName" && alias_rc="rcsetup"
eval echo $(alias rcSetup 2>/dev/null | cut -d"'" -f2) | grep -q "source\s*$myName" && alias_rc="rcSetup"

setup_temp=/tmp/asg_setup-$$

removeTemp=true
for opt in $*
do
  case "$opt" in
     --test)
         $myDir/test_rcSetup.sh;
         return $?;;
     -d|-b|--buildRelease) $myDir/rcDownload.py -o $setup_temp $*;
         unset alias_rc
         return 0;;
     --Debug)
         removeTemp=false;;
  esac
done

if [[ "$myDir_orig" == "." || "$myDir_orig" == "$currDir" ]]; then
   sameDir=1
else
   if [[ -L $myName_orig && -d $myDir_orig/RootCoreBin ]]; then
      sameDir=0
   else
      sameDir=1
   fi
fi

if [ $sameDir -eq 0 ]; then
   cd $myDir_orig
fi
$myDir/rcSetup.py -o $setup_temp $*
ret=$?
unset alias_rc

if [ -f $setup_temp ]; then
   source $setup_temp
   ret=$?
   $removeTemp && rm -f $setup_temp
fi

if [ $sameDir -eq 0 ]; then
   cd $currDir
fi

return $ret

# test -f $setup_temp && source $setup_temp

# get the last bash/zsh command other than itself:
# fc -ln -0   #does not work, it lists only previous cmd
# history -1  #print just previous cmd

