
import os, sys

from RuleClasses import TriggerRule

def main():
    # the rulebook you want to read
    rulebookpath = 'Physics_pp_v7_rules_2017-11-08.py'
    verbosity = 2 # 0 - error, 1 - warning, 2 - info, 3 - debug

    
    
    # don't edit this part
    hackedrulebookpath = 'rulebook_hacked.py'
    hack_rulebook(rulebookpath, hackedrulebookpath = hackedrulebookpath, verbosity=verbosity)
    
    rulesDict = getRulesDict(hackedrulebookpath = hackedrulebookpath)

    

def hack_rulebook(rulebookpath, hackedrulebookpath = 'rulebook_hacked.py', verbosity=2):
    # verbosity: 0 - error, 1 - warning, 2 - info, 3 - debug

    if verbosity >= 2:
        print "I am hacking the rulebook from", rulebookpath, "and saving the result to", hackedrulebookpath
    
    # changes:
    # comment out "from runOptions import filled_bunches as NumBunches"
    # comment out "import Monitoring_pp_v7_rules"
    # comment out "RulesList=[Monitoring_pp_v7_rules.physics_rules]"
    # comment out "rules.update(Monitoring_pp_v7_rules.physics_rules)"
    # change print to pass # print
    # change allowDuplicates default in userFunctions.py

    os.system('cp '+ rulebookpath + ' ' + hackedrulebookpath)
    for pair in [
            ["from runOptions import filled_bunches as NumBunches", "\# from runOptions import filled_bunches as NumBunches"],
            ["import Monitoring_pp_v7_rules", "\# import Monitoring_pp_v7_rules"],
            ["RulesList=\[Monitoring_pp_v7_rules.physics_rules\]", "\# RulesList=\[Monitoring_pp_v7_rules.physics_rules\]"],
            ["rules.update(Monitoring_pp_v7_rules.physics_rules)", "\# rules.update(Monitoring_pp_v7_rules.physics_rules)"],
            ["import Monitoring_pp_v6_rules", "\# import Monitoring_pp_v6_rules"],
            ["RulesList=\[Monitoring_pp_v6_rules.physics_rules\]", "\# RulesList=\[Monitoring_pp_v6_rules.physics_rules\]"],
            ["print", "pass \# print"],
            ]:

        # sed behaves differently on mac OS and *nix
        if sys.platform == 'darwin':
            command = "sed -i '' 's/"+ pair[0] + "/" + pair[1] + "/g' " + hackedrulebookpath
        else:
            command = "sed -i 's/"+ pair[0] + "/" + pair[1] + "/g' " + hackedrulebookpath

        os.system(command)
            
    


    
def getRulesDict(rulebookpath = 'rulebook_hacked.py', verbosity=2):
    # verbosity: 0 - error, 1 - warning, 2 - info, 3 - debug

    if verbosity >= 2:
        print "getting rulebook from", rulebookpath
        # ideally, print git branch
    if verbosity >= 3:
        print "bear in mind you might be on the wrong branch"

    if rulebookpath == 'rulebook_hacked.py':
        from rulebook_hacked import rules
    else:
        sys.exit("I don't know where to import the rules from")

    # rules is a TriggerRule as defined in RuleClasses.py
    # convert it to a normal dictionary
    rulesDict = {}
    for trigger in rules.keys():
        rulesDict[trigger] = {}
        for lumi in rules[trigger]:
            rulesDict[trigger][lumi] = {}
            for key in rules[trigger][lumi].keys():
                rulesDict[trigger][lumi][key] = rules[trigger][lumi][key]
                
        # print trigger
        # print rules[trigger]
        # print rulesDict[trigger]
        # break

    return rulesDict



if __name__ == '__main__':
    main()
