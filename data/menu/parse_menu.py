
import os, sys

def main():
    if os.environ['USER'] == 'ckaldero':
        menupath = '/afs/cern.ch/work/c/ckaldero/athena_fork/athena/Trigger/TriggerCommon/TriggerMenu/python/menu/Physics_pp_v7.py'

    else:
        menupath = 'Physics_pp_v7_2017-12-05.py'

    menuList = getMenuList(menupath)
    menuList = slimMenuList(menuList)

    specialTriggers = {
        'HLT_j360': 'lowest unprescaled, 2015',
        'HLT_j380': 'lowest unprescaled, 2016',
        'HLT_j400': 'lowest unprescaled, 2017 (1.5e34 menu)',
        'HLT_j420': 'lowest unprescaled, 2017 (1.7e34 menu)',
        'HLT_j450': 'lowest unprescaled, 2017 (2.0e34 menu)',

        'HLT_j225_gsc400_boffperf_split': 'lowest unprescaled, 2017 (1.5e34 menu)',
        'HLT_j225_gsc420_boffperf_split': 'lowest unprescaled, 2017 (1.7e34 menu)',
        'HLT_j225_gsc440_boffperf_split': 'lowest unprescaled, 2017 (2.0e34 menu)',

        'HLT_j260_320eta490': 'lowest unprescaled, 2015 - 2017 inclusive',

        'HLT_j360_a10_lcw_sub_L1J100': 'lowest unprescaled, 2015',
        'HLT_j420_a10_lcw_L1J100': 'lowest unprescaled, 2016',
        'HLT_j440_a10_lcw_subjes_L1J100': 'lowest unprescaled, 2017 (1.5e34 menu)',
        'HLT_j460_a10_lcw_subjes_L1J100': 'lowest unprescaled, 2017 (1.7e34 menu)',
        'HLT_j480_a10_lcw_subjes_L1J100': 'lowest unprescaled, 2017 (2.0e34 menu)',

        'HLT_j440_a10t_lcw_jes_L1J100': 'lowest unprescaled, 2017 (1.5e34 menu)',
        'HLT_j460_a10t_lcw_jes_L1J100': 'lowest unprescaled, 2017 (1.7e34 menu)',
        'HLT_j480_a10t_lcw_jes_L1J100': 'lowest unprescaled, 2017 (2.0e34 menu)',

        'HLT_j390_a10t_lcw_jes_30smcINF_L1J100': 'lowest unprescaled, 2017 (1.5e34 menu)',
        'HLT_j420_a10t_lcw_jes_40smcINF_L1J100': 'lowest unprescaled, 2017 (1.7e34 menu)',
        'HLT_j440_a10t_lcw_jes_40smcINF_L1J100': 'lowest unprescaled, 2017 (2.0e34 menu)',

        'HLT_2j330_a10t_lcw_jes_30smcINF_L1J100': 'lowest unprescaled, 2017 (1.5e34 menu)',
        'HLT_2j330_a10t_lcw_jes_40smcINF_L1J100': 'lowest unprescaled, 2017 (1.7e34 menu)',

        'HLT_j360_a10r_L1J100': 'lowest unprescaled, 2015 - TRIGGER HAD A BUG IN 2015',
        'HLT_j420_a10r_L1J100': 'lowest unprescaled, 2016',
        'HLT_j440_a10r_L1J100': 'lowest unprescaled, 2017 (1.5e34 menu)',
        'HLT_j460_a10r_L1J100': 'lowest unprescaled, 2017 (1.7e34 menu)',
        'HLT_j480_a10r_L1J100': 'lowest unprescaled, 2017 (2.0e34 menu)',

        }

    # prepare PhysicsAnalysis/DerivationFramework/DerivationFrameworkJetEtMiss/python/TriggerLists.py
    comment = None
    for item in menuList:
        if item['comment'] != comment:
            comment = item['comment']
            print '\n#', comment
        print "'"+item['HLT']+"',",
        if item['HLT'] in specialTriggers:
            print '#', specialTriggers[item['HLT']]
        else:
            print ''


def getMenuList(menupath, verbosity = 1):
    # verbosity: 0 - error, 1 - warning, 2 - info, 3 - debug

    if verbosity >= 2:
        print "getting menu from", menupath
        # ideally, print git branch
    if verbosity >= 3:
        print "bear in mind you might be on the wrong branch"


    lines = open(menupath).readlines()

    menuList = []
    signature = None
    HLTname = None
    L1name = None
    comment = None
    for line in lines:

        if 'TriggerFlags.' in line and 'Slice.signatures = [' in line:
            try:
                signature = line.split('TriggerFlags.')[1].split('Slice.signatures = [')[0]
            except:
                continue
            continue

        # end of signatures definitions
        if 'signatureList=[]' in line: break


        # if line.lstrip().startswith('#'): continue

        # only want list entries
        if not line.lstrip().startswith('[') and not line.lstrip().startswith('#'): continue
        # if not line.rstrip(' \n').endswith('],'): continue


        # get trigger names
        if line.lstrip().startswith("['"): # normal
            HLTname = 'HLT_'+line.split("['")[1].split("',")[0]
            L1name = line.split("',")[1].lstrip(' ').split("'")[1]
        elif line.lstrip().startswith('["'): # some tau triggers
            HLTname = 'HLT_'+line.split('["')[1].split('",')[0]
            L1name = line.split('",')[1].lstrip(' ').split('"')[1]
        elif line.lstrip().startswith('#'): # comments
            potentialcomment = line.split('#')[-1].rstrip('\n').lstrip(' ')
            if potentialcomment == '': continue
            comment = potentialcomment
            continue
        else:
            print "skipping", line,
            continue

        menuList.append({'HLT': HLTname, 'L1': L1name, 'signature': signature, 'comment': comment})

    return menuList



def slimMenuList(menuList):
    newMenuList = []
    for item in menuList:

        if item['signature'] == 'Bjet':
            if 'boffperf' not in item['HLT']: continue
            if 'bmv2c' in item['HLT']: continue
            if 'gsc' not in item['HLT']: continue

        elif item['signature'] != 'Jet':
            continue

        elif 'AFP' in item['comment']:
            continue

        newMenuList.append(item)
    return newMenuList


if __name__ == '__main__':
    main()
