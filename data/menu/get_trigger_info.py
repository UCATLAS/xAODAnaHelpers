
# C. W. Kalderon, 10.03.17
# This script returns various useful information given an HLT chain name (or partial name)
# Usage:
#    set the menu, EDM and rulebook paths in main(), and the verbosity (or use defaults)
#    "python get_trigger_info.py --trigger triggerName [--verbosity {0,1,2,3}] [--menuSet {2016,2017,2018}] [--print {all,rulebook}] [--forceExact {0,1}]"
# 2015 not implemented yet since everything lived in svn and this is all taken from gitlab


import os, sys
import copy
# so it can be called from other places, get absolute path to imports
thispath = os.path.realpath(__file__).replace('get_trigger_info.py','')
sys.path.append(thispath+'menu_rulebook_EDM_files')
from parse_menu import getMenuList, slimMenuList
from parse_EDM import getJetContainerList
from parse_rulebook import hack_rulebook, getRulesDict


def main():
    
    # get command line arguments / set defaults

    # set the trigger to enquire about
    triggerName = get_commandLine_option(sys.argv, '--trigger', 'HLT_j35_subjes')

    # choose the menu and rulebook set
    menu_EDM_rulebook_set = get_commandLine_option(sys.argv, '--menuSet', '2018')
    
    # set the verbosity of output you want. 0 - error, 1 - warning, 2 - info, 3 - debug
    verbosity = int(get_commandLine_option(sys.argv, '--verbosity', '2'))

    # control print output. If 'rulebook' then only that entry
    printOutput =  get_commandLine_option(sys.argv, '--print', 'all')

    # control print output. If 'rulebook' then only that entry
    forceExact = bool(int(get_commandLine_option(sys.argv, '--forceExact', '0')))
    

    # set print output
    if printOutput == 'all':
        onlyPrint = None
    else:
        onlyPrint = []
        if 'rulebook' in printOutput:
            onlyPrint.append('rulebook entry')
        if 'container' in printOutput:
            onlyPrint.append('HLT jet container preselection')
            onlyPrint.append('HLT jet container')
        if 'L1' in printOutput:
            onlyPrint.append('L1')
            onlyPrint.append('L1 too Complicated')
            onlyPrint += ['L1 selection', 'L1 extra']
            

        # 'signature',
        # 'L1',
        # 'L1 too Complicated',
        # 'L1 multiplicity', 'L1 ET', 'L1 eta', 'L1 extra',
        # 'HLT too Complicated',
        # 'HLT multiplicity',
        # 'HLT jet container preselection',
        # 'ET_preselection',
        # 'HLT jet container',
        # 'topocluster formation', 'clusters', 'clustering', 'calibration steps',
        # 'ET', 'eta', 'HT', 'single jet mass',
        # 'comment',
        # 'rulebook entry',


    
    # translation of menu_EDM_rulebook_set into the relevant files    
    year, menupath, EDMpath, rulebookpath = get_EDM_rulebook_paths(menu_EDM_rulebook_set)

        
    # get triggers in menu
    menuList = getMenuList(menupath, verbosity=verbosity)
    menuList = slimMenuList(menuList)

    # get containers in EDM
    containerList = getJetContainerList(EDMpath, verbosity=verbosity)

    # get rulebook entries
    if rulebookpath != None:
        hack_rulebook(rulebookpath, verbosity=verbosity)
        rulebookDict = getRulesDict(verbosity=verbosity)


    # check trigger name
    HLT_names = check_trigger_name(triggerName, menuList, verbosity=verbosity, forceExact=forceExact)
    
    # get the actual result
    for item_exact in HLT_names:
        add_jet_trigger_info(item_exact, EDMpath, containerList, year)
        if rulebookpath != None:
            add_rulebook_info(item_exact, rulebookDict)
        print_item(item_exact, onlyPrint)
    

    # do all items in menu
    # for item in menuList:
        # add_jet_trigger_info(item, EDMpath, containerList)
        # if rulebookpath != None:
            # add_rulebook_info(item, rulebookDict)
        # if 'HLT too Complicated' not in item:
            # continue
        # print_item(item, onlyPrint)


def get_commandLine_option(args, thisarg, default):
    try:
        argValue = args[args.index(thisarg)+1]
    except:
        argValue = default
    return argValue



def get_EDM_rulebook_paths(menu_EDM_rulebook_set, path=thispath+'menu_rulebook_EDM_files/'):
        
    # trigger menus live, for example, at https://gitlab.cern.ch/atlas/athena/blob/21.1/Trigger/TriggerCommon/TriggerMenu/python/menu/Physics_pp_v7.py
    # trigger EDM files live at https://gitlab.cern.ch/atlas/athena/blob/21.1/Trigger/TriggerCommon/TrigEDMConfig/python/TriggerEDM.py
    # rulebook files are at https://gitlab.cern.ch/atlas-trigger-menu/TrigMenuRulebook/tree/master
    
    # these three files need to be in sync for things to make sense
    # for the menu and rulebook they just need to come from the same athena release
    # for the rulebook you need to browse the history of eg https://gitlab.cern.ch/atlas-trigger-menu/TrigMenuRulebook/commits/master/python/Physics_pp_v7_rules.py

    if menu_EDM_rulebook_set == '2018':
        year = 2018
        menupath = path+'Physics_pp_v7_2018-03-11.py'
        EDMpath = path+'TriggerEDM_2018-02-19.py'
        rulebookpath = path+'Physics_pp_v7_rules_2018-03-06.py'
        # new 2018
        menupath = path+'Physics_pp_v7_2018-04-13.py'
        rulebookpath = path+'Physics_pp_v7_rules_2018-04-13.py'

    elif menu_EDM_rulebook_set == '2017':
        year = 2017
        menupath = path+'Physics_pp_v7_2017-12-05.py'
        # you could also use for instance MC_pp_v7 (the "v7" is for 2017, "v6" was used in 2016 data and "v8" will appear for 2018)
        EDMpath = path+'TriggerEDM_2017-12-05.py'

        # the file included here is from https://gitlab.cern.ch/atlas-trigger-menu/TrigMenuRulebook/blob/acceeb02ea9b982efb1aed08d77c8f6e0d8e1ce6/python/Physics_pp_v7_rules.py
        # it was committed on 2017-11-08
        rulebookpath = path+'Physics_pp_v7_rules_2017-11-08.py'

    elif menu_EDM_rulebook_set == '2016':
        year = 2016
        menupath = path+'Physics_pp_v6_2017-05-23.py'
        # https://gitlab.cern.ch/atlas/athena/blob/f3fe4d75c43a1e857b6b622e0134bafaed977558/Trigger/TriggerCommon/TrigEDMConfig/python/TriggerEDM.py
        EDMpath = path+'TriggerEDM_2016-06-14.py'
        rulebookpath = path+'Physics_pp_v6_rules_2016-05-25.py'

    else:
        print "did not recognise the menuSet option you provided (" + menu_EDM_rulebook_set + ")"
        sys.exit()

    return year, menupath, EDMpath, rulebookpath



def check_trigger_name(triggerName, menuList, verbosity=2, forceExact=False):
    if 'j' not in triggerName and 'ht' not in triggerName:
        sys.exit("you provided the trigger name " + triggerName + " which does not contain the required 'j' or 'ht' for an HLT jet trigger")

    if verbosity >= 2:
        print "I am getting info for", triggerName

    item_exact = None
    items_partial = []
    triggerNameParts = triggerName.split('*')
    for item in menuList:
        if triggerName == item['HLT']:
            item_exact = item
        else:
            remnant = item['HLT']
            matches = True
            for i in range(len(triggerNameParts)):
                if triggerNameParts[i] not in remnant:
                    matches = False
                    break
                else:
                    remnant = triggerNameParts[i].join(remnant.split(triggerNameParts[i])[1:])
                    
            if matches:
                items_partial.append(item)

    if item_exact == None:
        if verbosity >= 1: print "I found no exact matches",
        if forceExact:
            if verbosity >= 1: print "but I am going to take this anyway for information"
            if 'HLT' not in triggerName:
                HLT = None
            else:
                HLT = triggerName.split('_L1')[0]
            if 'L1' not in triggerName:
                sys.exit('you need to define an L1 for '+triggerName)
            else:
                L1 = ('L1_'+triggerName.split('L1')[1]).replace('__','_')
            item_exact = {'comment': 'not in menu', 'HLT': HLT, 'signature': 'custom jet', 'L1': L1}

        else:
            if len(items_partial) == 0:
                if verbosity >= 1: print "and no partial matches either. Try with something else or a different menu?"
                sys.exit("No exact or partial matches found for " + triggerName)

            elif len(items_partial) == 1:
                if verbosity >= 2:
                    print "but only a single partial match:"
                    print "   ", items_partial[0]['HLT']
                item_exact = items_partial[0]

            else:
                print "I found", len(items_partial), "partial matches"
                for item in items_partial:
                    print "   ", item['HLT']
                # print "you need to choose one"
                # sys.exit()

    else:
        if verbosity >= 2:
            print "I found an exact match"
    if item_exact == None:
        return items_partial
    else:
        return [item_exact]

        
def add_jet_trigger_info(item, EDMpath, containerList, year):
    # item is a dictionary containing at least 'HLT'
    name = item['HLT']
    item['HLT selection'] = {}
    item['L1 selection'] = {}

    # HT?
    if 'ht' in name:
        item['HT'] = int(name.split('ht')[1].split('_')[0])
    
    # does it have multiple thresholds?
    if name.count('j') > 1:
        following_chars = [substr[0] for substr in name.split('j')[1:]]
        following_ints = []
        for c in following_chars:
            try:
                following_ints.append(int(c))
            except:
                pass

        # if it's complicated, maybe I can trim it for now
        if len(following_ints) > 1:
            failed = False
            segment = 2
            while not failed:
                newname = 'j'.join(name.split('j')[:segment])
                try:
                    i = int(name.split('j')[segment][0])
                    failed = True
                except:
                    segment += 1
                    pass

            newname = '_'.join(newname.split('_')[:-1]) # remove trailing "_" or "_2" from "..._j..." or "..._2j..."
            item['HLT too Complicated'] = 'there is more, I chopped off ' + name.split(newname)[1]
            name = newname

    # HLT multiplicity
    try:
        item['HLT selection']['multiplicity'] = int(name.split('HLT_')[1].split('j')[0])
    except:
        if 'HLT_j' in name:
            item['HLT selection']['multiplicity'] = 1
        else:
            if 'ht' in name:
                pass
            else:
                print "can't decipher HLT multiplicity for", name
                pass

    # ET
    try:
        item['HLT selection']['ET'] = int(name.split('j')[1].split('_')[0])
    except:
        if 'ht' in name:
            pass
        else:
            print "can't decipher HLT multiplicity for", name
            pass
        
    if 'gsc' in name:
        item['HLT selection']['ET_preselection'] = item['HLT selection']['ET']
        item['HLT selection']['ET'] = int(name.split('gsc')[1].split('_')[0])

    # eta
    if 'eta' in name:
        etalow = float(int(name.split('eta')[0].split('_')[-1])/100.0)
        etahigh = float(int(name.split('eta')[1].split('_')[0])/100.0)
    else:
        etalow = 0.0
        etahigh = 3.2
    item['HLT selection']['eta'] = [etalow, etahigh]

    # mass
    if 'smc' in name:
        masslow = int(name.split('smc')[0].split('_')[-1])
        masshigh = name.split('smc')[1].split('_')[0]
        if masshigh == 'INF':
            masshigh = float("inf")
        else:
            masshigh = int(masshigh)
        item['HLT selection']['single jet mass'] = [masslow, masshigh]


    # calibration and container name - https://twiki.cern.ch/twiki/bin/viewauth/Atlas/JetTriggerNames

    containerName = 'HLT_xAOD__JetContainer_'

        
    # clustering algorithm
    if '_a10_' in name:
        item['clustering'] = 'radius 1.0 anti-kt'
        containerName += 'a10'
    elif '_a10r_' in name:
        item['clustering'] = 'radius 0.4 anti-kt, reclustered to radius 1.0'
        containerName += 'a10r_'
    elif '_a10t_' in name:
        item['clustering'] = 'radius 1.0 anti-kt, trimmed'
        containerName += 'a10t'
    else:
        item['clustering'] = 'radius 0.4 anti-kt'
        containerName += 'a4'

    # clustering inputs
    if 'lcw' in name:
        item['clusters'] = 'lcw topoclusters'
        containerName += 'tclcw'
    elif 'TT' in name:
        item['clusters'] = 'trigger towers'
        containerName += 'TTem' # also TThad
    else:
        item['clusters'] = 'em topoclusters'
        containerName += 'tcem'

        
    # calibration    
    default = True
    if 'nojcalib' in name or 'TT' in name:
        item['calibration steps'] = 'None'
        containerName += 'nojcalib'
        default = False
    else:
        item['calibration steps'] = []
        if 'sub' in name:
            default = False
            item['calibration steps'].append('pileup subtraction')
            containerName += 'sub'
        if 'jes' in name:
            default = False
            item['calibration steps'].append('Jet Energy Scale correction')
            containerName += 'jes'
        if 'IS' in name:
            default = False
            item['calibration steps'].append('eta and JES in-situ corrections')
            item['calibration steps'].append('calorimeter parts of GSC')
            containerName += 'IS'

        if default: # varies by year
            if year < 2017:
                item['calibration steps'].append('pileup subtraction')
                item['calibration steps'].append('Jet Energy Scale correction')
                containerName += 'subjes'
            else:
                item['calibration steps'].append('pileup subtraction')
                item['calibration steps'].append('Jet Energy Scale correction')
                item['calibration steps'].append('eta and JES in-situ corrections')
                item['calibration steps'].append('calorimeter parts of GSC')
                containerName += 'subjesIS'


                
        if 'gsc' in name:
            item['calibration steps'].append('ID track parts of GSC')


    # full or partial topoclustering
    if 'PS' in name:
        item['topocluster formation'] = 'partial scan'
        containerName += 'PS'
    else:
        item['topocluster formation'] = 'full scan'
        containerName += 'FS'

    # finalise container name
    if 'gsc' in name:
        item['HLT jet container'] = 'HLT_xAOD__JetContainer_GSCJet'
        item['HLT jet container preselection'] = 'HLT_xAOD__JetContainer_a4tcemsubjesISFS'
    elif 'ds1' in name:
        item['HLT jet container'] = 'HLT_xAOD__JetContainer_TrigHLTJetDSSelectorCollection'
        item['HLT jet container preselection'] = 'HLT_xAOD__JetContainer_a4tcemsubjesISFS'
    else:
        item['HLT jet container'] = containerName

    # check container name against the EDM
    EDMcontainerName = item['HLT jet container'].replace('xAOD__JetContainer_','')
    if EDMcontainerName not in containerList:
        print "For the trigger", name, "I thought the container name should be", item['HLT jet container'],
        print "but this (" + EDMcontainerName + ") is not in the EDM at", EDMpath
        sys.exit()
            


    # L1 information
    # this isn't very debugged
    name = item['L1']

    # does it have multiple thresholds?
    if 'MJJ' not in name and name.count('J') > 1:
        following_chars = [substr[0] for substr in name.split('J')[1:]]
        following_ints = []
        for c in following_chars:
            try:
                following_ints.append(int(c))
            except:
                pass

        # if it's complicated, maybe I can trim it for now
        if len(following_ints) > 1:
            failed = False
            segment = 2
            while not failed:
                newname = 'J'.join(name.split('J')[:segment])
                try:
                    i = int(name.split('J')[segment][0])
                    failed = True
                except:
                    segment += 1
                    pass

            newname = '_'.join(newname.split('_')[:-1]) # remove trailing "_" or "_2" from "..._j..." or "..._2j..."
            item['L1 too Complicated'] = 'there is more, I chopped off ' + name.split(newname)[1]
            name = newname


    # L1 details
    ignoreMult = False
    ignoreET = False
    ignoreeta = False
    
    # special ones
    if '-MATCH' in name:
        name = name.replace('-MATCH','')
        item['L1 extra'] = 'MATCH'

    if 'RD0' in name:
        item['L1 extra'] = 'random L1 trigger, be careful when combining with L1_J* (different bunch group requirements)'
        ignoreMult = True
        ignoreET = True
        ignoreeta = True
        
    if 'HT' in name:
        ignoreMult = True
        ignoreET = True
        ignoreeta = True
        item['L1 selection']['HT'] = int(name.split('HT')[1].split('_')[0].split('-')[0])
        if 'L1_extra' in item:
            item['L1 extra'] += ' - '
        else:
            item['L1 extra'] = ''
        item['L1 extra'] += 'HT formed from ' + name.split('HT')[1].split('_')[0].split('-')[1]

    if 'TE' in name:
        ignoreMult = True
        ignoreET = True
        ignoreeta = True
        item['L1 selection']['TE'] = int(name.split('TE')[1].split('_')[0].split('-')[0])
        if 'L1_extra' in item:
            item['L1 extra'] += ' - '
        else:
            item['L1 extra'] = ''
        item['L1 extra'] += 'Total Energy formed from ?'

    if 'SC' in name:
        ignoreMult = True
        ignoreET = True
        ignoreeta = True
        SC_thresh = int(name.split('SC')[1].split('-')[0])
        jet_ET_thresh = int(name.split('CJ')[1])
        if 'L1_extra' in item:
            item['L1 extra'] += ' - '
        else:
            item['L1 extra'] = ''
        item['L1 extra'] += ' - Simple Cone, sum(ET) of all ' + str(jet_ET_thresh) + ' GeV ROIs > ' + str(SC_thresh) + ' GeV'

    if 'MJJ' in name:
        ignoreMult = True
        ignoreET = True
        ignoreeta = True
        MJJ_thresh = int(name.split('MJJ')[1].split('-')[1])
        jet_ET_thresh = '?'
        if 'L1_extra' in item:
            item['L1 extra'] += ' - '
        else:
            item['L1 extra'] = ''
        item['L1 extra'] += 'MJJ, sum(ET) of all ' + str(jet_ET_thresh) + ' GeV ROIs > ' + str(MJJ_thresh) + ' GeV'
        

    if not ignoreMult:
        if '_J' in name:
            item['L1 selection']['multiplicity'] = 1
        else:
            try:
                item['L1 selection']['multiplicity'] = int(name.split('L1_')[1].split('J')[0])
            except:
                print "can't decipher L1 multiplicity for", name

    # ET
    if not ignoreET:
        try:
            item['L1 selection']['ET'] = int(name.split('J')[1].split('.')[0])
        except:
            try:
                item['L1 selection']['ET'] = int(name.split('J')[1].split('_')[0])
            except:
                print "can't decipher L1 multiplicity for", name
        
    # eta
    if not ignoreeta:
        if 'ETA' in name:
            etalow = float(int(name.split('ETA')[0].split('.')[-1])/10.0)
            etahigh = float(int(name.split('ETA')[1].split('.')[0])/10.0)
            item['L1 selection']['eta'] = [etalow, etahigh]
        elif 'J' in name:
            etalow = 0.0
            etahigh = 3.1
            item['L1 selection']['eta'] = [etalow, etahigh]

        

    # fill recommendations

    # for now, just eta because easy...
    etaRange = item['HLT selection']['eta']
    jetRadius = float(item['clustering'].split('radius ')[-1].split()[0])

    offlineEtaRange = copy.deepcopy(etaRange)
    if etaRange[0] == 0:
        offlineEtaRange[0] = 0
    else:
        offlineEtaRange[0] = etaRange[0]+jetRadius
    offlineEtaRange[1] = etaRange[1]-jetRadius

    item['offline selection recommended'] = {'multiplicity': item['HLT selection']['multiplicity'], 'eta': offlineEtaRange}


def add_rulebook_info(item_exact, rulebookDict):
    trigger = item_exact['HLT']
    try:
        rulebookEntry = rulebookDict[trigger]
    except:
        rulebookEntry = 'NOT IN RULEBOOK'

    item_exact['rulebook entry'] = rulebookEntry



def print_item(item, onlyPrint=None):
    # print in this order
    printorder = [
        'signature',
        'L1',
        'L1 too Complicated',
        'L1 multiplicity', 'L1 ET', 'L1 eta', 'L1 extra',
        'L1 selection',
        'HLT too Complicated',
        'HLT multiplicity',
        'HLT jet container preselection',
        'ET_preselection',
        'HLT jet container',
        'topocluster formation', 'clusters', 'clustering', 'calibration steps',
        'ET', 'eta', 'HT', 'single jet mass',
        'HLT selection',
        'offline selection recommended',
        'comment',
        'rulebook entry',
        ]

    print item['HLT']
    if onlyPrint == None:
        for thing in printorder:
            if thing in item:
                print '  ', thing+':', ' '*(25-len(thing)), item[thing]
    else:
        for thing in printorder:
            if thing in item and thing in onlyPrint:
                print '  ', thing+':', ' '*(25-len(thing)), item[thing]
                
    # print anything not in above order that - ie added elsewhere and forgotten about
    for thing in sorted(item):
        if thing not in printorder and thing != 'HLT':
            print '  ', thing+':', ' '*(25-len(thing)), item[thing]
            


        
if __name__ == '__main__':
    main()
