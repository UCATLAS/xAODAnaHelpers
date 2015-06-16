#!/usr/bin/env python
"""%prog [options] project,[asgType,]release,[,configs]

       where project/release/configs are delimited by "," or space:
          %projName   # project choice, such as "Base"
          asgType            # type of asg release, such as "private","1.0.X"
          64 | 32            # 64-bit (x86_64) or 32-bit (i686)
          slc5 | slc6        # using slc5 or slc6
          opt | dbg          # using opt or dbg version of ASG releas
          gcc43 | gcc46 | gcc47  # in format gcc* set gcc version

The default configuration can be overridden by defining env var "rcSetupSite".
"""

import sys, os
from fnmatch import fnmatch
from glob import glob
import re
from optparse import OptionParser, SUPPRESS_HELP
import commands

myName = sys.argv[0]
myDir  = os.path.dirname(myName)

# class rcSetup:
#   def __init__(self):
#      self.rcConf=""

stamp_RootCoreBin = ".RootCoreBin"
stamp_config = stamp_RootCoreBin + "/.RootCore-rcconfig"
myFullPath = os.path.realpath(__file__)
projPref = "Analysis"

#-----------------------------------------------------
# Determine where to get rcSetup configuration
#   and where to take ASG release
#
# 1. Use getenv("rcSetupSite") for configuration if defined
#   Otherwise, get from local file
#
# 2. Entry of ASG location will be removed from configuration
#    in case of non-existence.
#
# 3. If multiple location is provided for a same asgType
#    will try to avoid /afs/cern.ch/ if other file system is available
# --------------------------------------------------------------
useSite  = False
rcSetupSite = os.getenv("rcSetupSite")
if rcSetupSite:
   if os.path.isfile(rcSetupSite):
      execfile(rcSetupSite)
      useSite = True
   elif os.path.isdir(rcSetupSite):
      subdirs = glob(rcSetupSite+'/'+projPref+'*')
      if len(subdirs) > 0:
         print "Taking private release(s) into configuration at", rcSetupSite
         asgConf = {}
         asgConf_private = {'location':rcSetupSite,
                            'pattern': '*.*.*',
                            'asgType': "private"
                           }
         asgConf["private"] = asgConf_private
else:
   from rcSetup_conf import asgConf

if "private" not in asgConf:
   rcSetup_private = os.getenv("HOME") + '/' + ".rcSetup.conf"
   if os.path.isfile(rcSetup_private):
      private = {}
      execfile(rcSetup_private, private)
      if "asgConf" in private:
         asgConf.update(private["asgConf"])
         print "Adding private configuration in %s" % rcSetup_private


# separate ASG location on /afs/cern and not on that
asgConf_AfsCern = {}
asgConf_nonAfsCern = {}
for asgName in asgConf.keys():
    asgLoc = asgConf[asgName]["location"]
    subDirs = asgLoc.split('/')
    lenToCheck = 3
    if len(subDirs) <= lenToCheck:
       lenToCheck = 2
    pathToCheck = '/'.join(subDirs[:lenToCheck])
    if not os.path.exists(pathToCheck):
       # print "!!! ignore non-existent path=", asgLoc
       del asgConf[asgName]
       continue

    # for back-compatibility
    # otherwise only need: asgType = asgConf_type["asgType"]
    asgConf_type = asgConf[asgName]
    if "asgType" in asgConf_type:
       asgType = asgConf_type["asgType"]
    elif "nightly" in asgConf_type:
       asgType = asgConf_type["nightly"]
       asgConf_type["asgType"] = asgType
    else:
       asgType = "stable"
       asgConf_type["asgType"] = asgType

    if asgLoc.startswith("/afs/cern.ch"):
       if asgType not in asgConf_AfsCern:
          asgConf_AfsCern[asgType] = asgConf_type
    else:
       if asgType not in asgConf_nonAfsCern:
          asgConf_nonAfsCern[asgType] = asgConf_type

# take non-AfsCern first if exist
asgConf = asgConf_nonAfsCern
for asgName in asgConf_AfsCern.keys():
   if asgName not in asgConf:
      asgConf[asgName] = asgConf_AfsCern[asgName]

asgProjects = []
new_asgConf = {}
for asgName in asgConf:
   asgLoc = asgConf[asgName]["location"]
   asgConf_type = asgConf[asgName]
   for subdir in glob(asgLoc+'/'+projPref+'*'):
      project_full = os.path.basename(subdir)
      project = project_full.replace(projPref,"")
      # print "subdir=",subdir, ", project_full=",project_full
      if asgConf_type["asgType"] == "nightly":
         new_asgLoc = os.path.dirname(subdir)
         nightlyType = os.path.basename( new_asgLoc )
         # print "new_asgLoc=",new_asgLoc, ", nightlyType=",nightlyType
         new_asgName = nightlyType
         if new_asgName not in asgConf:
            new_asgConf_type = dict(asgConf_type)
            new_asgConf_type["location"] = new_asgLoc
            new_asgConf_type["asgType"] = nightlyType
            if new_asgName not in new_asgConf:
               new_asgConf[new_asgName] = new_asgConf_type
      else:
         if asgName not in new_asgConf:
            new_asgConf[asgName] = asgConf_type
      if project not in asgProjects:
         asgProjects += [project]

asgConf = new_asgConf

asgConf_patterns = {}
asgConf_relTypes = []
for asgName in asgConf:
   if asgName not in asgConf_relTypes:
      asgConf_relTypes += [asgName]
   asgConf_type = asgConf[asgName]
   asgConf_patterns[asgName] = asgConf_type["pattern"]


# print "asgProjects=",asgProjects


__doc__ = __doc__.replace("%projName",' | '.join(asgProjects), 1)

alias_rc = os.getenv("alias_rc")
if alias_rc:
   __doc__ = __doc__.replace("%prog", alias_rc)

# (status, shellType) = commands.getstatusoutput("ps -p `ps -p %s -o ppid=` -o comm=" % os.getpid() )

#********************
def writeout_unsetup(ROOTCOREDIR, filename):
#   ================
# write out a shell setup script 
# to unset previous ASG env
#-------------------------------
  stdout_save = sys.stdout
  if filename:
     outFile = open(filename,'w')
     sys.stdout = outFile
  print "source %s/scripts/unsetup.sh" % ROOTCOREDIR
  print "ret=$?; [ $ret -ne 0 ] && return $ret"

  if filename:
     # if not options.Debug:
     #    print "rm -f %s" % filename
     outFile.close()

  sys.stdout = stdout_save
  if options.Debug:
     print "::Debug:: Going to source", filename



#*****************
def writeout_setup(ROOTCORECONFIG, filename=None):
#   ==========
# write out a shell setup script for
# the selected ASG release
#-----------------------------------
  stdout_save = sys.stdout
  if filename:
     outFile = open(filename,'w')
     sys.stdout = outFile
  target_RootCoreBin = os.readlink(stamp_RootCoreBin)
  local_setup = "%s/local_setup.sh" % target_RootCoreBin
  print "export ROOTCORECONFIG=%s" % ROOTCORECONFIG
  print "rcSetupSite_saved=$rcSetupSite"
  if os.path.isdir("RootCore"):
     rootcoreVer_cwd = None
     svn_entries = "RootCore/.svn/entries"
     if os.path.exists(svn_entries):
        f = open(svn_entries, 'r')
        word_tags = "RootCore/tags/"
        for line in f:
           index_tags = line.find(word_tags)
           if index_tags >= 0:
              rootcoreVer_cwd = line[index_tags+len(word_tags):]
              f.close()
              break
     if rootcoreVer_cwd:
        absPath_rootcore = os.path.dirname(os.path.dirname(os.path.realpath(local_setup)))
        rootcoreVer_cmt = absPath_rootcore + "/cmt/version.cmt"
        if os.path.exists(rootcoreVer_cmt):
           with open(rootcoreVer_cmt, 'r') as f:
             rootcoreVer_inASG = f.readline().strip()
             if options.verbose:
                print >>stdout_save, "RootCore(local)=%s, RootCore(in ASG)=%s" % (rootcoreVer_cwd,rootcoreVer_inASG)
             if rootcoreVer_cwd >= rootcoreVer_inASG:
                print "export ROOTCOREBIN=%s" % target_RootCoreBin
                local_setup = "RootCore/scripts/setup.sh"
                print >>stdout_save, "Using local RootCore"

  target_stampConfig = os.readlink(stamp_config)
  # print >>stdout_save, "target_stampConfig=",target_stampConfig
  if target_stampConfig.startswith("/cvmfs"):
     print '[[ "X$ATLAS_LOCAL_ROOT_BASE" == "X" ]] && export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase'
  print "source %s" % local_setup
  print 'ret=$?; [ "X$rcSetupSite_saved" = "X" ] && unset rcSetupSite; [ $ret -ne 0 ] && return $ret'
  if options.findPackages:
     print "rc find_packages"
     print "ret=$?; [ $ret -ne 0 ] && return $ret"

  # define some SVN reps for convenience
  if os.getenv("SVNOFF") == None:
     print "export SVNOFF=svn+ssh://svn.cern.ch/reps/atlasoff"
  if os.getenv("SVNGRP") == None:
     print "export SVNGRP=svn+ssh://svn.cern.ch/reps/atlasgrp"
  if os.getenv("SVNGROUPS") == None:
     print "export SVNGROUPS=svn+ssh://svn.cern.ch/reps/atlasgroups"
  if os.getenv("SVNUSR") == None:
     from user_at_svnCERN import user_at_svnCERN
     svnUser = user_at_svnCERN()
     if svnUser == None:
        svnUser = os.getenv("USER")
     print "export SVNUSR=svn+ssh://svn.cern.ch/reps/atlas-%s" % svnUser
  if os.getenv("SVNINST") == None:
     print "export SVNINST=svn+ssh://svn.cern.ch/reps/atlasinst"
  if os.getenv("SVNPERF") == None:
     print "export SVNPERF=svn+ssh://svn.cern.ch/reps/atlasperf"
  if os.getenv("SVNPHYS") == None:
     print "export SVNPHYS=svn+ssh://svn.cern.ch/reps/atlasphys"
  if os.getenv("SVNROOT") == None:
     print "export SVNROOT=svn+ssh://svn.cern.ch/reps/atlasoff"
  
  if filename:
     # if not options.Debug:
     #    print "rm -f %s" % filename
     outFile.close()

  sys.stdout = stdout_save
  if options.Debug:
     print "::Debug:: Going to source", filename

#**********************
def print_RootVer_inRel(asgRelLoc):
#   ===================
# print out the ROOT version used in a given release
#
  filename = asgRelLoc + '/' + "Asg_root/cmt/RootVersion.txt"
  if not os.path.exists(filename):
     print filename," does not exist, could not find the ROOT version"
  else:
     (status, RootVer) = commands.getstatusoutput("cat %s" % filename)
     print "The ROOT version in the release is: \n\t%s" % RootVer

#**********************
def list_packages_inRel(asgRelLoc):
#   ===================
# print out the list of packages used in a given release
#
  filename = asgRelLoc + '/' + "packages.rc"
  if not os.path.exists(filename):
     print filename," does not exist at", asgRelLoc
  else:
     (status, packageList) = commands.getstatusoutput("cat %s" % filename)
     print "The packages in the release is: \n\n%s" % packageList

#**********************
def parse_stamp_config(stampFile):
#   ==================
# parse a stamp file if no argument given
#  to determine ASG release number and RootCoreConfig
#----------------------------------------------------
   if not stampFile:
      sys.exit("Warning!!! Invalid argument in parse_stamp_config")

   if not os.path.islink(stampFile):
      sys.exit("Warning!!! Invalid sym-link=%s" % stampFile)

   target_asgRel = os.path.normpath( os.readlink(stampFile) )
   re_asgRel = re.compile(r'(.*)/RootCore/bin/(.*)')
   matchObj = re_asgRel.match(target_asgRel)
   if not matchObj:
      sys.exit("Warning!!! Improper directory name in %s" % target_asgRel)
   (asgRelLoc, ROOTCORECONFIG) = matchObj.groups()

   if not os.path.isdir(asgRelLoc):
      sys.exit("Warning!!! Previous release=%s does not exist anymore,\n please provide release number to setup" % asgRelLoc )

   if not options.printRootVer:
      if not os.path.isdir(target_asgRel):
         sys.exit("Warning!!! Config=%s does not exist for release at the following location, please provide a new release or config to setup: \n\t%s" % (ROOTCORECONFIG, asgRelLoc) )

      print "In current directory, found previous release with config=%s at the following location, going to set up its env: \n\t%s" % (ROOTCORECONFIG, asgRelLoc)
      writeout_setup(ROOTCORECONFIG, filename=options.filename)
   else:
      print "In current directory, found previous release at \n\t%s" % asgRelLoc
      print_RootVer_inRel(asgRelLoc)




def get_projRels(asgLoc, projNames, relname):
#   ============
    locPref = asgLoc + '/' + projPref
    projLoc = locPref + projNames + '/'
    avaiRels = [ w.replace(locPref,'').split('/') for w in glob(projLoc + relname) ]
    projects = {}
    for rel in avaiRels:
       if rel[0] in projects:
          projects[rel[0]] += [ rel[1] ]
       else:
          projects[rel[0]] = [ rel[1] ]
    return projects

def sort_relNumber(list):
    """ Sort the given list in the way that humans expect.
    and break the list into multiple lists according to the first number n1 
    in n1.n2.n3.
    And special handling for 2.1.* which use ROOT-6
""" 
    convert = lambda text: int(text) if text.isdigit() else text
    alphanum = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ]
    list.sort(key=alphanum)
    mult_lists = []
    tmpString = ""
    last_firstNum = None
    special_21 = True
    for relNum in list:
       subNums = relNum.split('.')
       firstNum = subNums[0]
       if not firstNum.isdigit():
          mult_lists = [ ' '.join(list) ]
          break
       else:
          if firstNum != last_firstNum:
             if last_firstNum != None:
                mult_lists += [ tmpString ]
             tmpString = relNum
             last_firstNum = firstNum
          else:
             len_tmpString = len(tmpString)
             if len_tmpString == 0:
                tmpString = relNum
             elif len_tmpString+len(relNum) > 70:
                mult_lists += [ tmpString ]
                tmpString = relNum
             elif special_21 and subNums[0] == "2" and subNums[1] == "1":
                mult_lists += [ tmpString ]
                tmpString = relNum
                special_21 = False
             else:
                tmpString += ' ' + relNum
    if len(tmpString) > 0:
       mult_lists += [ tmpString ]
    return mult_lists



def print_projRels(asgLoc, projNames, relname):
#   ==============
    projects = get_projRels(asgLoc, projNames, relname)
    for projName in projects:
       relList = projects[projName]
       # relList.sort(key=alphanum)
       mult_list = sort_relNumber(relList)
       if len(mult_list) < 1:
          return
       else:
          print " %s: \t%s" % (projName, mult_list[0] )
          for list in mult_list[1:]:
              print "  \t%s" % list


def get_projRels_conf(asgLoc, projNames, relname, config):
#   =================
    locPref = asgLoc + '/' + projPref
    projLoc = locPref + projNames + '/'
    rcLib = "/RootCore/lib"
    avaiRels = glob(projLoc + relname + rcLib + '/' + config)
    projects = {}
    for rel in avaiRels:
       index_rcLib = rel.find(rcLib)
       asgRelLoc = rel[:index_rcLib]
       rel = rel.replace(locPref,'').replace(rcLib,'').split('/')
       if rel[0] in projects:
          projects[rel[0]]["releases"] += [ rel[1] + ',' + rel[2] ]
       else:
          projects[rel[0]] = {"location":asgRelLoc, 
                              "releases": [ rel[1] + ',' + rel[2] ] }
    return projects


def print_projRels_conf(asgLoc, projNames, relname, config):
#   ===================
    projects = get_projRels_conf(asgLoc, projNames, relname, config)
    for projName in projects:
       print " %s: \t%s" % (projName, ' '.join(projects[projName]["releases"]) )
       # ToDo: avoid printout of asgRelLoc in projects
       # print " %s: \t%s" % (projName, ' '.join(projects[projName][0][0]) )


def list_ASGRels(using_rcConf, rcConf, allTypes=False, myRelnam=""):
#   ============
   if not using_rcConf:
      print "Available releases are:"
   else:
      print "Available releases matching config=%s are :" %rcConf

   for type in sorted(asgConf):
      if not allTypes and asgType != "" and type != asgType:
         continue
      asgLoc = asgConf[type]["location"]
      print "asgType=%s (at %s):" % (type,asgLoc)
      if myRelnam == "":
         asgRelname = asgConf[type]["pattern"]
      else:
         asgRelname = myRelnam
      if not using_rcConf:
         print_projRels( asgLoc, asgProj, asgRelname )
      else:
         print_projRels_conf( asgLoc, asgProj, asgRelname, rcConf )


def make_workarea(asgRelLoc, ROOTCORECONFIG):
#   =============
# Set up the new ROOTCORE env
#----------------------------
  if not os.path.isdir(asgRelLoc):
     sys.exit("Warning!!! ASG release directory %s not found" % asgRelLoc)

  # echo "Found ASG release $asgRel,$ROOTCORECONFIG in $ROOTCORERel_new"

  ROOTCOREDIR_new = asgRelLoc + "/RootCore"
  makeBinArea_cmd = ROOTCOREDIR_new+ "/scripts/make_bin_area.sh"
  if not os.path.isfile(makeBinArea_cmd):
     sys.exit("Warning!!!, script (to make bin area) %s NOT FOUND" % makeBinArea_cmd)

  ROOTCOREBIN = os.getenv("ROOTCOREBIN")
  ROOTCOREBIN_local = os.getcwd()+"/RootCoreBin"
  if not ROOTCOREBIN:
     ROOTCOREBIN = ROOTCOREBIN_local

  print "Going to build a RootCore bin area=", ROOTCOREBIN
  (status,output) = commands.getstatusoutput("export ROOTCORECONFIG=%s; %s %s" % 
                            (ROOTCORECONFIG, makeBinArea_cmd, ROOTCOREBIN) )
  if status != 0:
     sys.exit("Error code=%d!!! Building RootCore bin area failed:\n%s" % (status,output) )

  if ROOTCOREBIN == ROOTCOREBIN_local:
     os.system("ln -fsn %s %s" % ("RootCoreBin", stamp_RootCoreBin) )
  else:
     os.system("ln -fsn %s %s" % (ROOTCOREBIN, stamp_RootCoreBin) )
  os.system("ln -fsn %s %s" % (ROOTCOREDIR_new+"/bin/"+ROOTCORECONFIG,
                               stamp_config) )
  os.system("ln -fsn %s/rcSetup.sh rcSetup.sh" % myDir )
  os.system("ln -fsn %s/rcSetup.csh rcSetup.csh" % myDir )


# come back to the main program
rcConf=""
rcConfSet=False
config_64="*"
config_opt="*"
config_slc="*"
config_gcc="*"
asgRel=""
asgProj="*"
asgType=""

optParser = OptionParser(usage=__doc__,conflict_handler="resolve")
optParser.add_option('-d', action='store_const',const=True, dest='download',
                  default=False, help="Download source code of a release, try '-d -h' for usage")
optParser.add_option('-b','--buildRelease', action='store_const', const=True,
                  dest='buildRelease', default=False,
                  help="Download & build release under current directory, try '-b -h' for usage")
optParser.add_option('--verbose', action='store_const',const=True, dest='verbose',
                  default=False, help=SUPPRESS_HELP)
optParser.add_option('--Debug', action='store_const',const=True, dest='Debug',
                  default=False, help=SUPPRESS_HELP)
optParser.add_option('-V','--version', action='store_const', const=True,
                  dest='printVersion', default=False,
                  help="Print rcSetup version")
optParser.add_option('-r','--releases', action='store_const', const=True,
                  dest='printReleases', default=False,
                  help="Print available releases")
optParser.add_option('-c','--printConfigs', action='store_const', const=True,
                  dest='printConfigs', default=False,
                  help="Print available config(s) for given project/release")
optParser.add_option('-R','--printRootVer', action='store_const', const=True,
                  dest='printRootVer', default=False,
                  help="Print ROOT version for a given release")
optParser.add_option('--listPackages', action='store_const', const=True,
                  dest='listPackages', default=False,
                  help="Print a list of packages for a given release")
optParser.add_option('-f','--findPackages', action='store_const', const=True,
                  dest='findPackages', default=False,
                  help="Automatic running 'rc find_packages' in this setup")
optParser.add_option('-o', type="string", dest='filename', 
                  default=None, help=SUPPRESS_HELP)
optParser.add_option('-u','--unSetup', action='store_const', const=True,
                  dest='unSetup', default=False,
                  help="unSet currently existing ASG environment")


# parse options and args
options, args = optParser.parse_args()
if options.verbose:
    print options
    print "args=",args
    print "asgConf=",asgConf
    print

args_split = []
for arg in args:
   args_split += arg.split(',')

# print "args=",args_split
asgType_matched = []
args_split_copy = list(args_split)
for arg in args_split_copy:
    for type in sorted(asgConf_relTypes):
        if arg == type:
           asgType = type
           del args_split[args_split.index(arg)]
           asgType_matched += [type]
if len(asgType_matched) > 1:
   print >> sys.stderr, "!!Error!! Multiple asgType matched=",asgType_matched
   sys.exit(1)

for arg in args_split:
  if arg == "":
     continue
  if arg == "64" or arg == "x86_64":
     config_64="x86_64"
     rcConfSet = True
  elif arg == "32" or arg == "i686":
     config_64="i686"
     rcConfSet = True
  elif arg == "opt" or arg == "dbg":
     config_opt = arg
     rcConfSet = True  
  elif fnmatch(arg, "slc*"):
     config_slc = arg
     rcConfSet = True  
  elif fnmatch(arg, "gcc*"):
     config_gcc = arg
     rcConfSet = True
  elif fnmatch(arg, "generic"):  # special case for unknown RootCoreConfig
     rcConf = arg
     rcConfSet = True  
  elif fnmatch(arg, "*-*-*-*"):
     rcConf = arg
     rcConfSet = True  
  elif arg in asgProjects:
     asgProj = arg
  else:
     if options.verbose:
        print "arg=",arg
     if asgType != "":
        if fnmatch(arg, asgConf_patterns[asgType]):
           asgRel  = arg
           continue
     else:
        for type in sorted(asgConf_patterns):
           if options.verbose:
              print "In arg parsing, type=",type, ", pattern=",asgConf_patterns[type]
           # if type == "private":  # "private" must be specified for private release
           #    continue
           if fnmatch(arg, asgConf_patterns[type]):
              asgRel  = arg
              asgType = type
              if options.verbose:
                 print "Match found, asgType=",type, " for given relNumber=",asgRel
              if asgType != "private":
                 break
        if asgRel == arg:
           continue
     print "Warning!! invalid input argument=%s" % arg
     optParser.print_help()
     sys.exit(1)

if rcConf == "" and rcConfSet:
   rcConf = '-'.join([config_64,config_slc,config_gcc,config_opt])
else:
   rcConf = "*"

if options.printReleases:
   if options.printConfigs:
      list_ASGRels(True, "*", False)
   else:
      list_ASGRels(False, "", False)
   sys.exit(0)

if options.printVersion:
   myDir = os.path.dirname(myFullPath)
   changeLog = open(myDir+"/ChangeLog", 'r')
   line = None
   for line in changeLog:
      if line.find("Tagged as ") > -1:
         changeLog.close()
         break
   if line:
      myVersion = line.split()[-1]
      print myVersion
   else:
      print "Version information not found in this package"
      sys.exit(1)
   sys.exit(0)

ROOTCOREDIR = os.getenv("ROOTCOREDIR")
if options.unSetup:
   if ROOTCOREDIR == None or ROOTCOREDIR == "":
      sys.exit("Warning!!! No ASG was previously set")
   else:
      print "Going to unset previous ASG env"
      writeout_unsetup(ROOTCOREDIR, filename=options.filename)
      sys.exit(0)

if ROOTCOREDIR and not options.printRootVer and not options.listPackages:
   sys.exit("Warning!!! Already set up a RootCore:\n\t%s\nif you really like to override it, please run 'rcSetup -u' first" % ROOTCOREDIR)

# check if there is already previous ASG release if no argument is given
if len(args) == 0:
   if os.path.islink( stamp_config ):
      parse_stamp_config( stamp_config )
      sys.exit(0)

# come to locate the required ASG release
asgLoc = ""
asgRelname = ""
if asgProj == "*":
   print "Warning!! No project provided, available projects are \n\t%s\n" % asgProjects
   optParser.print_help()
   sys.exit(1)

if asgType != "":
   asgLoc = asgConf[asgType]["location"]

if asgRel != "" and asgType != "":
   rels = get_projRels( asgLoc, asgProj, asgRel )
   if options.verbose:
      print "rels=",rels
   if len(rels) == 0:
      print "Warning!! %s,%s release %s NOT available" % (asgProj, asgType, asgRel)
      list_ASGRels(False, "", False)
      print "\nOr run 'rcSetup -r' for a full available release"
      sys.exit(1)
   elif options.printRootVer or options.listPackages:
      asgRelLoc = asgLoc + '/' + projPref + asgProj + '/' + rels.items()[0][1][0]
      if options.printRootVer:
         print_RootVer_inRel(asgRelLoc)
         sys.exit(0)
      elif options.listPackages:
         list_packages_inRel(asgRelLoc)
         sys.exit(0)
else:
   print "Warning!! Please specify a release number"
   list_ASGRels(False, "", True)
   sys.exit(1)


if options.printConfigs:
   print "Available configs for %s at %s:" % (','.join([asgProj,asgRel,asgType,rcConf]), asgLoc)
   print_projRels_conf(asgLoc, asgProj, asgRel, rcConf)
   sys.exit(0)

rels_info = get_projRels_conf( asgLoc, asgProj, asgRel, rcConf )
if len(rels_info) == 0:
   print "Warning!! No %s release %s comes with config=%s" % (asgProj, asgRel, rcConf)
   # list_ASGRels(True, "*-*-*-*", False, asgRel)
   list_ASGRels(True, rcConf, False, asgRel)
   sys.exit(1)


relinfo_proj = rels_info[asgProj]
rels_proj = relinfo_proj["releases"]
if len(rels_proj) != 1:
   confs_matched = []
   for rel_proj in rels_proj:
      confs_matched += [ rel_proj.split(',')[1] ]
   print "Warning!! Multiple %s release %s matching config=%s: \n%s" % (asgProj, asgRel, rcConf, ' '.join(confs_matched) )
   print "Please narrow the selection, and try again"
   sys.exit(1)

ROOTCORECONFIG = rels_proj[0].split(',')[1]

asgRelLoc = relinfo_proj["location"]
print "Found ASG release with config=%s at \n\t%s" % (ROOTCORECONFIG, asgRelLoc)
make_workarea(asgRelLoc, ROOTCORECONFIG)

writeout_setup(ROOTCORECONFIG, filename=options.filename)
