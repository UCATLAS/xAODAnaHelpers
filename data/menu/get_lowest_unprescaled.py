
# this script gets the lowest unprescaled trigger from a predefined set of jet signatures, eg "single jet", "6j gsc central" etc.


import os, sys
import copy

sys.path.append('menu_rulebook_EDM_files')
from get_trigger_info import get_EDM_rulebook_paths, get_commandLine_option, check_trigger_name, add_jet_trigger_info, print_item, add_rulebook_info

from parse_menu import getMenuList, slimMenuList
from parse_EDM import getJetContainerList
from parse_rulebook import hack_rulebook, getRulesDict


def main():

    menu_EDM_rulebook_set = get_commandLine_option(sys.argv, '--menuSet', '2017')
    verbosity = int(get_commandLine_option(sys.argv, '--verbosity', '0'))
    lumi_point = get_commandLine_option(sys.argv, '--lumi', '1.7')

    print "I am using:"
    print "- menu set", menu_EDM_rulebook_set
    print "- lumi point", lumi_point
    
    year, menupath, EDMpath, rulebookpath = get_EDM_rulebook_paths(menu_EDM_rulebook_set)

    # print disclaimer
    print "WARNING: this will not work properly for complicated triggers, eg multiple thresholds in one chain (apart from HLT_jX_gscY and HLT_jX_YsmcZ)"

    
    # get triggers in menu
    menuList = getMenuList(menupath, verbosity=verbosity)
    menuList = slimMenuList(menuList)

    # get containers in EDM
    containerList = getJetContainerList(EDMpath, verbosity=verbosity)

    # get rulebook entries
    if rulebookpath != None:
        hack_rulebook(rulebookpath, verbosity=verbosity)
        rulebookDict = getRulesDict(verbosity=verbosity)


    # I want to take all triggers with certain characteristics, and group them
    # for all triggers in menu
    group_by = [
        'has HT',
        'HLT multiplicity',
        'HLT jet container',
        'eta',
        # 'single jet mass',
        'has single jet mass',
    ]
    HLT_groups = group_triggers(group_by, menuList, EDMpath, containerList, year, verbosity)

    
    # get only unprescaled ones at lumi_point
    HLT_groups_unprescaled = get_unprescaled_groupings(HLT_groups, rulebookDict, lumi_point)

    # get lowest unprescaled
    # sort by these, in order of priority
    sort_by = [
        'ET',
        'HT',
        'L1 is L1_4J15',
        'single jet mass',
        ]
    HLT_groups_unprescaled_lowest = get_lowest_in_group(HLT_groups_unprescaled, sort_by)

    
    for grouping in sorted(HLT_groups_unprescaled_lowest):
        nice_string = decode_grouping(grouping, group_by)
        # print grouping
        print nice_string
        print '   ', len(HLT_groups_unprescaled_lowest[grouping]), [item['HLT'] for item in HLT_groups_unprescaled_lowest[grouping]]
        # for item in HLT_groups_unprescaled_lowest[grouping]:
            # print_item(item)



        


def group_triggers(group_by, menuList, EDMpath, containerList, year, verbosity):

    grouped_items = {}
    
    for item in menuList:
        HLT_names = check_trigger_name(item['HLT'], menuList, verbosity=verbosity)
        if len(HLT_names) != 1:
            sys.exit("somehow I don't have a list of length 1")
        triggerInfo = HLT_names[0]
        add_jet_trigger_info(triggerInfo, EDMpath, containerList, year)

        categorisation_list = []
        for categorisation in group_by:
            if categorisation.startswith('has '):
                lookingfor = categorisation.split('has ')[-1]
                if lookingfor in triggerInfo:
                    substr = 'has'+lookingfor
                else:
                    substr = 'no'+lookingfor
            else:
                if categorisation in triggerInfo:
                    substr = str(triggerInfo[categorisation])
                else:
                    substr = 'None'
            categorisation_list.append(substr)
        categorisation_string = '___'.join(categorisation_list)

        if categorisation_string in grouped_items:
            grouped_items[categorisation_string].append(item)
        else:
            grouped_items[categorisation_string] = [item]

    return grouped_items


def get_unprescaled_groupings(HLT_groups, rulebookDict, lumi_point):
    HLT_groups_unprescaled = {}
    for grouping in sorted(HLT_groups):
        unPS = []
        for item in HLT_groups[grouping]:
            add_rulebook_info(item, rulebookDict)

            # get the behaviour at lumi_point - if lumi_point is 1.8, and the rulebook has "1000" and "18001", then I want the behaviour of "1000"
            if item['rulebook entry'] == 'NOT IN RULEBOOK':
                item['isPrescaled'] = True
                continue
            
            for lumi in sorted(item['rulebook entry'])[::-1]:
                if float(lumi) > 10000*float(lumi_point): # 1.5 has 15001
                    continue
                break

            item['rulebookLumi'] = lumi
            
            if 'rate' in item['rulebook entry'][lumi]:
                item['isPrescaled'] = True
            elif 'PS' in item['rulebook entry'][lumi]:
                item['isPrescaled'] = (float(item['rulebook entry'][lumi]['PS']) != 1)
            else:
                print "not sure what to do for", item['HLT'],  item['rulebook entry'][lumi]

            if not item['isPrescaled']:
                unPS.append(item)

        if len(unPS) > 0:
            HLT_groups_unprescaled[grouping] = copy.deepcopy(unPS)

    return HLT_groups_unprescaled


def get_lowest_in_group(HLT_groups_unprescaled, sort_by):
    HLT_groups_unprescaled_lowest = {}
    
    for grouping in sorted(HLT_groups_unprescaled):
        # print grouping

        temp_list_start = HLT_groups_unprescaled[grouping]
        # print [item['HLT'] for item in temp_list_start]
        temp_list_end = []
        
        for quality in sort_by:
            # print '   start', quality, len(temp_list_start), len(temp_list_end)
            # print '     ', [t['HLT'] for t in temp_list_start], [t['HLT'] for t in temp_list_end]

            if ' is ' in quality:
                quality_required = quality.split(' is ')[0]
                value_required = quality.split(' is ')[1]
            else:
                quality_required = quality
                value_required = None
                
            current_minimum = None
            temp_list_end = []
            for item in temp_list_start:
                if quality_required not in item:
                    # is it in any?
                    inAny = False
                    for itemAny in temp_list_start:
                        if quality_required in itemAny:
                            inAny = True
                    if inAny:
                        sys.exit("ERROR! All items in a grouping should have the same qualities available...")
                    else:
                        # print item['HLT'], 'and all others in', grouping, 'do not have', quality_required
                        temp_list_end.append(item)
                        continue

                if value_required != None:
                    if item[quality_required] == value_required:
                        temp_list_end.append(item)
                
                else:    
                    # first one
                    if current_minimum == None:
                        current_minimum = item[quality_required]

                    # if a lower one is found, reset the final list
                    if item[quality_required] < current_minimum:
                        # print item['HLT'], "has min", quality_required, "less than", current_minimum, " - wiping"
                        temp_list_end = []
                        current_minimum = item[quality_required]

                    # fill with all with quality_required equal to the minimum
                    if item[quality_required] == current_minimum:
                        # print item['HLT'], "has min", quality_required, "of", item[quality_required], "i.e. equal to", current_minimum, " - keeping"
                        temp_list_end.append(item)

            # If I am prioritising a value, but it is in none of them, then do nothing
            if value_required != None:
                if len(temp_list_end) == 0:
                    temp_list_end = temp_list_start
                
            # after each quality, set the start list to what results from this quality
            # print '   end', quality, len(temp_list_start), len(temp_list_end)
            # print '     ', [t['HLT'] for t in temp_list_start], [t['HLT'] for t in temp_list_end] 
            temp_list_start = temp_list_end
            if len(temp_list_start) == 1:
                break

        if len(temp_list_end) == 0:
            sys.exit("ERROR! I managed to remove everything...")

        HLT_groups_unprescaled_lowest[grouping] = copy.deepcopy(temp_list_end)
        # print len(HLT_groups_unprescaled_lowest[grouping])
        
    return HLT_groups_unprescaled_lowest
    

def decode_grouping(grouping, group_by):
    nice_string_components = []

    splitgrouping = grouping.split('___')

    for i in range(len(group_by)):
        if 'has ' in group_by[i]:
            if splitgrouping[i].startswith('no'):
                continue
            else:
                nice_string_components.append(group_by[i].split('has ')[1])

        if group_by[i] == 'HLT multiplicity':
            if splitgrouping[i] != 'None':
                nice_string_components.append(splitgrouping[i] + ' jet')
                if int(splitgrouping[i]) > 1:
                    nice_string_components[-1] += 's'

        if group_by[i] == 'HLT jet container':
            jettype = splitgrouping[i].split('JetContainer_')[-1]
            if jettype == 'GSCJet':
                jettype = 'small-R GSC'
            if jettype == 'a4tcemsubjesISFS':
                jettype = 'small-R'
            if jettype == 'a10r_tcemsubjesISFS':
                jettype = 'large-R reclustered'
            if jettype == 'a10tclcwsubjesFS':
                jettype = 'large-R'
            if jettype == 'a10ttclcwjesFS':
                jettype = 'large-R trimmed'
                

            replaced = False
            for j in range(len(nice_string_components)):
                if 'jet' in nice_string_components[j]:
                    nice_string_components[j] = nice_string_components[j].replace('jet', jettype + ' jet')
                    replaced = True
                    break
            if not replaced:
                nice_string_components.append('('+jettype+' jets)')


        if group_by[i] == 'eta':
            if splitgrouping[i] != 'None':
                etaLimits = eval(splitgrouping[i])
                nice_string_components.append(str(etaLimits[0]) + ' < |eta| < ' + str(etaLimits[1]))
    

    return ', '.join(nice_string_components)
    

if __name__ == '__main__':
    main()
