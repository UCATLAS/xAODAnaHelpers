import os,sys
import subprocess

if len(sys.argv) < 3:
   print "At least 2 arguments needed, too less argument in",sys.argv[0]
   sys.exit(1)

outFilename = sys.argv[1]
outFile = None
old_env = os.environ

isDebug = '--Debug' in sys.argv[2:]
source = ' '.join(['source'] + sys.argv[2:])
# proc = subprocess.call('%s; exec 1>%s; echo returnValue=$?; printenv; alias' 
#                        % (source,outFilename),
#                        shell=True, executable='/bin/bash' )
proc = subprocess.call('%s; ret=$?; exec 1>%s; echo returnValue=$ret; printenv' 
                        % (source,outFilename),
                        shell=True, executable='/bin/bash' )
new_env_print = open(outFilename).readlines()


# comparison between old_env and new_env
# --------------------------------------
vars_ignore=["_", "SHLVL", "LINES", "COLUMNS"]
stdout_save = sys.stdout

No=0
startEnv=False
returnValue=0 
for line in new_env_print:
   No += 1
   line = line.strip()
   if not startEnv:
      if line.startswith("returnValue="):
         startEnv=True
         returnValue=line.strip().split('=')[1]
         if outFilename:
            outFile = open(outFilename,'w')
            sys.stdout = outFile
      else:
         print line
      continue
   # processing alias
   # do not define alias in tcsh for the moment
   # because it may override some pre-defined tsh aliases
   # and python-2.6.5p2 does not print the prefix of "alias" from bash command "alias"
   if isDebug:
      print "#--Debug:",line
   if line.startswith("alias"):
      print line.replace('=',' ',1).replace(".sh",".csh")
      continue
   index_assign = line.find('=')
   if index_assign < 1:
      continue
   # (varName, _, varString) = line.partition('=')
   varName = line[:index_assign]
   if varName in vars_ignore:
      continue
   varString = line[index_assign+1:]

   # special handling of defined function like
   # "module() defined in /usr/share/Modules/init/bash"
   if varString.startswith("()"):
      continue

   if not varName in old_env:
      print "setenv %s '%s'" % (varName, varString)
      continue
   if varString != old_env[varName]:
      print "setenv %s '%s'" % (varName, varString)

if outFile:
   if isDebug:
      print "#rm -f %s" % outFilename
   else:
      print "rm -f %s" % outFilename
   print "exit %d" % int(returnValue)
   outFile.close()

sys.stdout = stdout_save
