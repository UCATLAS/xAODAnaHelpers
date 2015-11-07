import optparse
parser = optparse.OptionParser()
parser.add_option('-i', '--input',           dest="inFileName",         default="", help="Input file name")
o, a = parser.parse_args()


try:
    import pyAMI.client
except:
    print "Failed to load pyAMI.client, you need to set up local PyAMI"
    print  "do"
    print "  > localSetupPyAMI"
    import sys
    sys.exit(-1)

import pyAMI.atlas.api as AtlasAPI

client = pyAMI.client.Client('atlas')
AtlasAPI.init()

inputDS = []
eventCounts = {}
inputFile = open(o.inFileName,"r")

for line in inputFile:
    if line.startswith("#"): continue
    words = line.split()
    if not len(words): continue
    
    dsName = words[0].rstrip("/")
    dsID = dsName.split(".")[1]
    print dsName

    dsInfo = AtlasAPI.list_datasets(client, patterns = dsName, fields = ['events'])
    nEvents = dsInfo[0]['events']

    inputDS.append(dsName)
    eventCounts[dsName] = nEvents

for ds in inputDS:
    print ds,"\t",eventCounts[ds]
