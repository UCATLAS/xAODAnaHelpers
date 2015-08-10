#!/usr/bin/python

import os, math, sys
from time import strftime

import optparse
parser = optparse.OptionParser()
parser.add_option('-t', '--test',           action="store_true", dest="doTest",  default=False, help="Test")
parser.add_option('-f', '--file',           dest="inFile",       default="", help="Input file")
parser.add_option('-v', '--version',        dest="version",      default="", help="Version Tag")
parser.add_option('-c', '--config',         dest="configName",   default="", help="Config file")
parser.add_option('-p', '--production',     action="store_true", dest="doProduction",  default=False, help="Test")

o, a = parser.parse_args()

if not o.inFile:
  print "ERROR: give an input file"
  sys.exit(-1)

if not o.version:
  print "ERROR: give an version tag"
  sys.exit(-1)

if not o.configName:
  print "ERROR: give a configName"
  sys.exit(-1)  

test = False # does not run the jobs
if not o.doTest:
  if not os.path.exists("gridOutput"):
    os.system("mkdir gridOutput")
  if not os.path.exists("gridOutput/gridJobs"):
    os.system("mkdir gridOutput/gridJobs")


output_tag = o.version  # CHANGE BEFORE SUBMITTING - no periods!

timestamp = strftime("_%Y%m%d")
output_tag += timestamp

# has all samples - pick your favorites
file_in     = o.inFile
config_name = o.configName.replace("MultijetAlgo/data/","$ROOTCOREBIN/data/MultijetAlgo/")


#### list of systematics
systDict = { 'All' : [ 1 ]    # run all systematics and nominal
#           'Nominal' : [ 0 ]  # run nominal only
#        , 'JET_GroupedNP_1' : [-1, 1] # run one syst up and down 1 sigma
#        , 'JET_GroupedNP_2' : [-1, 1]
#        , 'JET_GroupedNP_3' : [-1, 1]
#        , 'JET_RelativeNonClosure_MC12' : [-1, 1]
        }



#############
# run over systematic variations
#############
for systName, values in systDict.iteritems():
  for systSize in values: # val should have a list of values for the systematic
    syst_tag = systName
    if not systSize == 0 :
      if systSize < 0:
        syst_tag = syst_tag + "Do"
      else:
        syst_tag = syst_tag + "Up"
      systValStr = str(math.fabs(systSize)).replace(".","-") # do not put more periods in name
      syst_tag = syst_tag + systValStr
    output_tag = syst_tag+"."+output_tag

    submit_dir = "gridOutput/gridJobs/submitDir_"+os.path.basename(file_in).rsplit('.',1)[0]+"."+output_tag # define submit dir name

    command = "runMultijet -inFile "+file_in+" -outputTag "+output_tag+" -submitDir "+submit_dir+" -configName "+config_name+" -syst "+systName+" "+str(systSize)

    if o.doProduction:
      command += " -production phys-exotics"

    print command
    if not o.doTest: os.system(command)


