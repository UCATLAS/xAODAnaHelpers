

import os, sys

def main():
    if os.environ['USER'] == 'ckaldero':
        EDMpath = '/afs/cern.ch/work/c/ckaldero/athena_fork/athena/Trigger/TriggerCommon/TrigEDMConfig/python/TriggerEDM.py'

    else:
        EDMpath = 'TriggerEDM_2017-12-05.py'

    containerList = getJetContainerList(EDMpath)

    
def getJetContainerList(EDMpath, verbosity=2):
    # verbosity: 0 - error, 1 - warning, 2 - info, 3 - debug

    if verbosity >= 2:
        print "getting menu from", EDMpath
        # ideally, print git branch
    if verbosity >= 3:
        print "bear in mind you might be on the wrong branch"

    lines = open(EDMpath).readlines()

    jetContainerList = []
    started = False


    for line in lines:

        if not started:
            if 'TriggerHLTList = [' in line:
                started = True
                continue
        if not started:
            continue
        if started and ']' in line:
            started = False
            break

        if line.lstrip().startswith('#'):
            continue

        if 'xAOD::JetContainer' not in line:
            continue

        containerName = line.split("xAOD::JetContainer#")[1].split("'")[0]
        jetContainerList.append(containerName)

    return jetContainerList
        
if __name__ == '__main__':
    main()
