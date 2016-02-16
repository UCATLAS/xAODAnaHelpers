#!/usr/bin/env python

# @file:    getDSInfo.py
# @purpose: generate SUSY metadata files
# @author:  John Alison <john.alison@cern.ch>
# @date:    January 2016
#
# @example:
# @code
# getDSInfo.py --help
# @endcode
#

try:
    import pyAMI.client
except:
    print "Failed to load pyAMI.client, setting up local PyAMI"
    import os
    os.system("localSetupPyAMI")
    import pyAMI.client

import pyAMI.atlas.api as AtlasAPI
import argparse

parser = argparse.ArgumentParser(description='Generate SUSY metadata files.')
parser.add_argument('input',metavar='dslist.txt',help='Distributions to compare')
parser.add_argument('-o','--output',metavar='output_crosssections_13TeV.txt',type=str,default=None,help='Output file name')
args=parser.parse_args()

client = pyAMI.client.Client('atlas')
AtlasAPI.init()

inputDS = []

inputFile = open(args.input,"r")

for line in inputFile:
    if line.startswith("#"): continue
    line = line.strip()
    if line=='': continue

    parts=line.split(':')
    dsName = parts[-1].rstrip("/")
    dsID = dsName.split(".")[1]
    print dsName

    dsProv = AtlasAPI.get_dataset_prov(client,dataset=dsName)
    for prov in dsProv["node"]:
        if prov['dataType'] == "EVNT":
            thisProvDSName = prov['logicalDatasetName']
            thisProvDSID   = thisProvDSName.split(".")[1]
            if thisProvDSID == dsID:
                print "\tUsing ",thisProvDSName
                inputDS.append(thisProvDSName)

def getUnitSF(unit):
    if unit == "nano barn":
        return 1000
    print "Unknown unit..."
    return 1.0

fh_out=open(args.output,'w') if args.output!=None else None
for ds in inputDS:
    dsList = AtlasAPI.get_dataset_info(client,dataset=ds)
    dsInfo = dsList[0]
    #print dsInfo['logicalDatasetName']
    #print "\tcross section",dsInfo["crossSection_mean"]
    #print "\tfilter Eff.",dsInfo["GenFiltEff_mean"]
    unit = dsInfo['crossSection_unit']
    getSF = getUnitSF(unit)
    if fh_out==None:
        print dsInfo['datasetNumber']," ",dsInfo['physicsShort']," ",float(dsInfo["crossSection_mean"])*getSF," 1.  ",float(dsInfo["GenFiltEff_mean"])," 1."
    else:
        fh_out.write("%s\t%s\t%e\t1.\t%e\t1.\n"%(dsInfo['datasetNumber'],dsInfo['physicsShort'],float(dsInfo["crossSection_mean"])*getSF,float(dsInfo["GenFiltEff_mean"])))


