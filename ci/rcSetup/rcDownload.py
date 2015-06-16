#!/usr/bin/env python
"""rcSetup [options] project,release

       where project/release are delimited by "," or space:
"""

svnCERN="svn.cern.ch"
rc_packages="packages.rc"

import sys, os
import re
from fnmatch import fnmatch
from glob import glob
from optparse import OptionParser, SUPPRESS_HELP
import commands, subprocess
import threading, Queue
# import shutil, time

pwd = os.getcwd()
myFullPath = os.path.realpath(__file__)
rcSetupConf=os.getenv("HOME") + "/.rcSetup.conf"

# define options
# ==============
optParser = OptionParser(usage=__doc__,conflict_handler="resolve")
optParser.add_option('-d', action='store_const',const=True, dest='download',
                  default=True, help="Download source code of a release")
optParser.add_option('--verbose', action='store_const',const=True, dest='verbose',
                  default=False, help=SUPPRESS_HELP)
optParser.add_option('-o', type="string", dest='filename', 
                  default=None, help=SUPPRESS_HELP)
optParser.add_option('-q','--quiet', action='store_const',const=True, dest='quiet',
                  default=False, help="Do not ask question, use default answer")
optParser.add_option('-V','--version', action='store_const', const=True,
                  dest='printVersion', default=False,
                  help="Print rcSetup version")
optParser.add_option('-l','--listPackages', action='store_const', const=True,
                  dest='listPackages', default=False,
                  help="Print list of packages in a given release")
optParser.add_option('-r','--releases', action='store_const', const=True,
                  dest='printReleases', default=False,
                  help="Print available releases for download")
optParser.add_option('-b','--buildRelease', action='store_const', const=True,
                  dest='buildRelease', default=False,
                  help="download and build release under current directory")
optParser.add_option('-D','--keepDebugSym', action='store_const', const=True,
                  dest='keepDebugSym', default=False,
                  help="keep debug-symbols in built libraries")
optParser.add_option('--downloadAll', action='store_const', const=True,
                  dest='downloadAll', default=False,
                  help=SUPPRESS_HELP)

# parse options and args
options, args = optParser.parse_args()
if options.verbose:
    print options  
    print "args=",args
    print

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


def Is_svnIn_sshConfig():
#   ====================
    """Check if svn.cern.ch is in $HONE/.ssh/config
    """
    sshConfigName = os.getenv("HOME") + "/.ssh/config"
    if not os.path.exists(sshConfigName):
       return False
    sshConfigFile = open(sshConfigName)
    svnIn_sshConfig = False
    for line in sshConfigFile:
       line = line.lstrip()
       if len(line) == 0:
          continue
       elif line[0] == '#':
          continue
       elif line.find(svnCERN) > 0:
          svnIn_sshConfig = True
          break
    sshConfigFile.close()
    return svnIn_sshConfig


#-----------------------------------------------------
# Check definition of SVNOFF
#  and check if it is accessible
# -----------------------------------------------------
SVNOFF = os.getenv("SVNOFF")
if not SVNOFF:
   SVNOFF="svn+ssh://svn.cern.ch/reps/atlasoff"

if not Is_svnIn_sshConfig():
   USER = os.getenv("USER")
   if SVNOFF.find(svnCERN) >= 0 and not options.quiet:
      answer = raw_input('Is "%s" your username at "%s"? y/n (default=y): ' % (USER,svnCERN) )
      if len(answer)>0 and not answer.lower().startswith("y"):
         USER = raw_input("Please input your username@%s: " % svnCERN)
         SVNOFF = re.sub("//.*%s" % svnCERN, "//%s@%s" % (USER,svnCERN), SVNOFF)

if options.verbose:
   print "Checking", SVNOFF
(s,o) = commands.getstatusoutput("svn ls %s" % SVNOFF)
if s != 0:
   print "Error with %s, quit now" % SVNOFF
   sys.exit(1)

prefASG="Analysis"
len_prefASG=len(prefASG)
rc_projects = []
for dir in o.split():
   if dir.startswith(prefASG):
      rc_projects += [ dir.rstrip('/')[len_prefASG:] ]

if options.verbose:
   print "projects=",rc_projects

alias_rc = os.getenv("alias_rc")
if alias_rc:
   __doc__ = __doc__.replace("%prog", alias_rc)


def get_projRels(projNames):
#   ======================
    projRels = {}
    for proj in rc_projects:
       if fnmatch(proj,projNames):
          fullProj = prefASG + proj
          (s,o) = commands.getstatusoutput("svn ls %s/%s/tags"
                           % (SVNOFF, fullProj) )
          tags_dash = []
          for line in o.split():
             if line.startswith(fullProj):
                tags_dash += [ line.replace("%s-" % fullProj, "").replace('/','') ]
          projRels[proj] = tags_dash
    return projRels


def print_projRels(projNames):
#   =========================
    projRels = get_projRels(projNames)
    print "Available source of ASG releases in SVN:"
    for projName in projRels:
       listOfRels = []
       for rel in projRels[projName]:
          listOfRels += [ re.sub("^0","", rel.replace("-0",".").replace('-','.') ) ]
       print " %s: \t%s" % (projName, ' '.join(listOfRels) )


def convert_relInDash(relInDot):
#   ===========================
    """convert release name from format "1.0.1" to "01-00-01" format
    """
    relInDash = ""
    relInDash_items = []
    for item in relInDot.split('.'):
        relInDash_items += [ item.zfill(2) ]
    relInDash = '-'.join(relInDash_items)
    return relInDash

class getPackage_worker(threading.Thread):
    nWorker = 0
    downloaded = []
    failed     = []
    def __init__(self, queue):
        self.q = queue
        self.tries = 4
        threading.Thread.__init__(self)   
        getPackage_worker.nWorker += 1
        self.nWorker = getPackage_worker.nWorker

    def run(self):
        import shutil, time
        while 1:
           item = self.q.get()
           if item == None:
              if options.verbose:
                 print "Worker#=",self.nWorker,", reaches the end of queue"
                 sys.stdout.flush()
              self.q.task_done()
              break
           [pack_svnPath, packName] = item
           svn_checkout = "svn co %s %s" % (pack_svnPath, packName)
           if options.verbose:
              print "Worker#=",self.nWorker,", svn_cmd=",svn_checkout
              sys.stdout.flush()
           s = 0
           for iTry in range(self.tries):
              (s,o) = commands.getstatusoutput(svn_checkout)
              if s == 0:
                 getPackage_worker.downloaded += [packName]
                 break
              else:
                 if options.verbose:
                    print "Warning!!! Error with svn checkout of package=",packName," at try-%d" % (iTry+1)
                    sys.stdout.flush()
              time.sleep( 5 ) # wait for 5s
           if (s != 0):
              print "Warning!!! Error with svn checkout of package=",packName," after %d tries" % self.tries
              sys.stdout.flush()
              shutil.rmtree(packName,ignore_errors=True)
              getPackage_worker.failed += [packName]
           self.q.task_done()


packNameRe = re.compile(".*/(.*)/tags/(.*)")
def getPackages_fullPath(packages):
#   ==============================
    """Convert the path in packages.rc to SVN full path
    """
    # packages_fullPath = {}
    packs_exist = []
    nJob = 10
    queue = Queue.Queue(nJob*2)
    packs_downloaded = getPackage_worker.downloaded
    for iJob in range(nJob):
        getPackage_worker(queue).start()  # start a worker

    for package in packages:
       print "Checking out package=",package
       matchObj = packNameRe.match(package)
       if matchObj:
          groups = matchObj.groups()
          packName = groups[0]
          packTag  = groups[1]
          if packName == "Asg_root" or packName == "Asg_Cxx11":
             if not options.downloadAll:
                print "Skip downloading package Asg_root"
                continue
          if os.path.exists(packName):
             packs_exist += [packName]
             continue
          pack_svnPath = package.replace("atlasoff/","%s/" % SVNOFF, 1)
          queue.put([pack_svnPath, packName])
          # packages_fullPath[packName] = pack_svnPath

    for iJob in range(nJob):
        queue.put(None)

    queue.join() # wait for all package download to be complete

    # return packages_fullPath
    packs_exist.sort()
    packs_downloaded.sort()
    packs_failed = getPackage_worker.failed
    packs_failed.sort()
    return (packs_exist, packs_downloaded, packs_failed)

def download_packs(asgProj,asgRel):
#   =============================
    """download packages in the specified ASG release
    """
    projRels = get_projRels(asgProj)
    if not asgProj in projRels:
       sys.exit("Warning!!! projectName=%s not found" % asgProj)
    projFullName = prefASG + asgProj
    asgRel_inDash = convert_relInDash(asgRel)
    if options.verbose:
       print "Input asgRel=",asgRel, "; converted asgRel in dash=",asgRel_inDash
    if asgRel_inDash in projRels[asgProj]:
       rel_tag = '-'.join( [projFullName,asgRel_inDash])
       rc_packages_fullPath = '/'.join( [SVNOFF,projFullName,'tags',rel_tag,rc_packages] )
       (s,o) = commands.getstatusoutput("svn cat %s" % rc_packages_fullPath)
       if s == 0:
          if options.listPackages:
             print "List of package for %s-%s: \n%s" % (asgProj,asgRel,o)
          else:
             print "Start to download packages in release %s-%s" % (asgProj,asgRel)
             (packs_exist, packs_downloaded, packs_failed) = getPackages_fullPath(o.split())
             if len(packs_exist) > 0:
                print "!!!Warning!!! Not download the following packages because they already exist"
                print packs_exist,"\n"
             if len(packs_downloaded) > 0:
                print "The following packages were just downloaded:"
                print packs_downloaded
             else:
                print "No package was downloaded"
             if len(packs_failed) > 0:
                print "The following packages failed in downloading:"
                print packs_failed
             # remove Asg_root
             # if os.path.exists("Asg_root"):
                

def writeout_build(filename):
#   ========================
    """write a script to build an ASG release under current directory
    """
    stdout_save = sys.stdout
    if filename:
       outFile = open(filename,'w')
       sys.stdout = outFile
    print "source RootCore/scripts/setup.sh"
    print "rc find_packages"
    platform = sys.platform
    import multiprocessing
    nCPUs = multiprocessing.cpu_count()
    if options.verbose:
       print >>stdout_save, "::writeout_build:: nCPUs=",nCPUs
    if nCPUs > 3:
       print "export ROOTCORE_NCPUS=%d" % (nCPUs/2)
    print "[ -e log ] || mkdir log"
    print "export ROOTCORELOG=$PWD/log"
    print "rc compile --continue --log-files"
    print "ret=$?"
    if not options.keepDebugSym:
       print "rc strip --remove-debug"
    print "return $ret"

    if filename:
       outFile.close()

    sys.stdout = stdout_save


def writeout_conf(workDir):
#   =======================
    """write a private configuration into file $HOME/.rcSetup.conf
    """
    import pprint
    global asgConf
    asgConf = {}
    # if os.path.exists(rcSetupConf):
    #    execfile(rcSetupConf,globals())
    asgConf["private"] = { "location":"%s" % workDir,
                             "pattern": "*.*.*",
                             "asgType": "private"}
    fileConf=open(rcSetupConf,'w+')
    fileConf.write("asgConf=")
    pp = pprint.PrettyPrinter(stream=fileConf,indent=2)
    pp.pprint(asgConf)
    fileConf.close()


# come back to the main program
asgRel=""
asgProj="*"


args_split = []
for arg in args:
   args_split += arg.split(',')

# print "args=",args_split
for arg in args_split:
  if arg == "":
     continue
  if arg in rc_projects:
     asgProj = arg
  elif fnmatch(arg, "[0-9]*"):
     asgRel = arg
  else:
     print "Warning!! invalid input argument=%s" % arg
     optParser.print_help()
     sys.exit(1)

if options.verbose:
   print "asgProj=", asgProj
   print "asgRel=", asgRel

if options.printReleases:
   print_projRels(asgProj)
   sys.exit(0)

if asgProj != "*" and len(asgRel) > 0:

   fullProj = prefASG + asgProj
   if not os.path.exists(fullProj):
      os.mkdir(fullProj)

   fullProjRel = fullProj + '/' + asgRel
   if not os.path.exists(fullProjRel):
      os.mkdir(fullProjRel)

   os.chdir(fullProjRel)
   download_packs(asgProj, asgRel)
   sys.stdout.flush()

# stop here
if not options.buildRelease:
   sys.exit(0)

# ensure ROOTCORECONFIG is set and ROOT is setup
# --------------------------------------------
# ROOTCORECONFIG = os.getenv("ROOTCORECONFIG")
# if ROOTCORECONFIG == None or ROOTCORECONFIG == "":
#   sys.exit("Warning!!! undefined or empty env variable 'ROOTCORECONFIG'")

ROOTSYS = commands.getoutput("which root 2>/dev/null && echo $ROOTSYS")
if ROOTSYS == "":
   sys.exit("Warning!!! No ROOT is set or ROOTSYS is not defined")

# write out a script to build release in private area
# ---------------------------------------------------
filename = options.filename
writeout_build(filename)
print "Be patient ! starting to compile packages in local area"
sys.stdout.flush()
if not filename.startswith("/"):
   filename = os.getcwd() + '/' + filename
status = subprocess.call([ ". %s" % filename], shell=True)
if os.path.exists(filename):
   os.remove(filename)
if status != 0:
   sys.exit("Warning!!! Release-%s building in private area failed with error=%d" % (','.join(args_split), status) )

# need write a private configuration file under $HOME directory
# -------------------------------------------------------------
writeout_conf(pwd)
asgConf = None

execfile(rcSetupConf)
if options.verbose:
   print "asgConf=",asgConf
if type(asgConf) != dict:
   sys.exit("Failed in writing private configuration to %s" % rcSetupConf)
else:
   print "Successful with building release in private area"

