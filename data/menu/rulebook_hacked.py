from RuleClasses import TriggerRule
# from runOptions import filled_bunches as NumBunches
from UserFunctions import disableLowerLumiPrimaries, RulebookDict
from runOptions import do3bunches, add2kHxPixel, doAFP2017
      
tag = 'Physics_pp_v7'

# Only for special runs. Can be removed 
doLumiTotalRate=False
rate_HLT_lumipeb_L1RD0_FILLED = 1000 #no extra rate, but override normal rules for 200Hz still
doL1Calo=False #Default: False --> can be removed
doLowLumi=False #Default: False --> can be removed

# 8b4e emergency key with disabling multi bjets (Default False->Keep, Emergency True->Disable)
NoGSCMBjets=False # Default: False

peakLumi=24001
## YU: Nov 2017
# 2018 menu study
do2018=False # If we run 2018 menu, this should be set True
if do2018:
    NoGSCMBjets=True
# 2.2e34 study
switchToBackup=False # Default: False
disableBphys=False # Default: False
# NB: by "disable3MU4" we do not really mean the disabling of the L1_3MU4 seed
# Instead, we mean the disabling all 4mu4-analysis related chains, see below
disable3MU4=True # Default: False
disableEoFChains=False # if True, disable end-of-fill B-physics chains
# 2017 menus for 2018 higher lumis
MainPrimary2017Oct1p7E34=24001
Primary2017Oct1p7E34=24001 # Default: 18001. If we run 2017 1.7e34 menu for higher luminisoty points, these two should be set 24001 and do2018=False
Primary2017Oct2p0E34=24001 # Default: 20001. If we run 2017 2.0e34 menu for higher luminisoty points, this should be set 24001 and do2018=False

# 2017 menu as designed in 1.5e34, 1.7e34, 2.0e34 # do2018=False, Primary2017Oct1p7E34=18001 and Primary2017Oct2p0E34=20001
# 2017 menu 1.7e34 # do2018=False, Primary2017Oct1p7E34=24001 and Primary2017Oct2p0E34=24001
# 2017 menu 2.0e34 # do2018=False, Primary2017Oct1p7E34=18001 and Primary2017Oct2p0E34=24001
# 2018 menu 2.0e34 # do2018=True, Primary2017Oct1p7E34=18001 and Primary2017Oct2p0E34=20001
# If we want to disable Bphys or switch to backup triggers, disableBphys=True or switchToBackup=True

doStartup2018 = False # keep False here to disable startup chains

    
from runOptions import bgrp9_bunches

#################################################################################################
#
#
# Muon  
#
#################################################################################################


########################################
# 2e34 

muon_Rules_2e34=RulebookDict(tag,'muon_Rules_2e34')

L1_list=['L1_MU20', 'L1_MU21', 'L1_2MU10', 'L1_3MU6']
muon_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary L1'),
    }])))

L1_list=['L1_MU11_2MU6',]
muon_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary L1'),
#   Waiting validation of new mu11_mu6 chais (ATR-17673) before disabling
#   18001 : TriggerRule(PS=-1, comment='Disabled, replaced by L1_LFV-MU11, ATR-16732'),
    }])))



L1_list=['L1_MU10_2MU6',]
muon_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                    7001 : TriggerRule(PS=-1, comment='Disable'),
    }])))

HLT_list=['HLT_mu26_ivarmedium']
if do2018:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(PS=1, comment='Primary, Express', ESRate=0.6),
       MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'), }])))
else:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(PS=1, comment='Primary, Express', ESRate=0.6),
                        }])))

HLT_list=['HLT_mu26_ivartight', 'HLT_mu28_ivartight']
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary'),
                        }])))



HLT_list=[ 
    'HLT_mu24_2mu4noL1',
    'HLT_mu60_0eta105_msonly',
    'HLT_mu26_mu8noL1',
    "HLT_mu26_mu10noL1", 
    'HLT_mu22_2mu4noL1', 'HLT_mu20_2mu4noL1',
    'HLT_3mu6',
    ]
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

HLT_list=['HLT_mu60',]
if do2018:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable'),
                        }])))
else:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                        }])))

HLT_list=['HLT_mu24_mu8noL1',]
if do2018:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable', ESRate=0),
                        }])))
else:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary express', ESRate=0.1),
                        }])))

HLT_list=['HLT_2mu14']
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Express', ESRate=0.3), 
                    }])))

HLT_list=['HLT_3mu6_msonly']    
if do2018:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable'),
                        }])))
else:
    muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, dedicated trigger LLP, ATR-10933'),
                        }])))

HLT_list=[
    'HLT_mu28_ivarmedium',                
    "HLT_mu80",
    "HLT_mu24_mu10noL1",
    "HLT_mu24_mu12noL1", 
    "HLT_mu28_mu8noL1",
    "HLT_2mu15_L12MU10", 
    "HLT_3mu8_msonly",
]
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary backup ATR-15290'), 
                    }])))

########################################
# 1.5e34 

muon_Rules_1p5e34=RulebookDict(tag,'muon_Rules_1p5e34')

HLT_list=[ 'HLT_mu50',]
muon_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_mu22_mu8noL1',]
muon_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    MainPrimary2017Oct1p7E34 : TriggerRule(rate=0.1, comment='Support'),
                    }])))

########################################
# 1.0e34 

muon_Rules_1e34=RulebookDict(tag,'muon_Rules_1e34')

HLT_list=['HLT_mu24_imedium']
muon_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_mu24_ivarmedium']
muon_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-16981', ESValue=0), 
                    10001: TriggerRule(PS=-1, comment='Disabled', ESValue=0),
                    }])))

HLT_list=['HLT_mu20_mu8noL1', ]
muon_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Backup'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))

L1_list=['L1_MU6_3MU4']
muon_Rules_1e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1, tau->3mu ATR-11754'),
                        peakLumi : TriggerRule(PS=-1, comment='Disabled'),
    }])))    

########################################
# 0.7e34 

muon_Rules_0p7e34=RulebookDict(tag,'muon_Rules_0p7e34')

HLT_list=['HLT_mu40',]
muon_Rules_0p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    7001: TriggerRule(PS=-1, comment='Disabled'),
                    }])))

########################################
# 0.5e34 

muon_Rules_0p5e34=RulebookDict(tag,'muon_Rules_0p5e34')

HLT_list=['HLT_mu24_ivarloose']
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-13367'), 
                    5001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_2mu10']
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      5001 : TriggerRule(PS=-1, comment='Disabled'),
                      }])))


HLT_list=[
    'HLT_2mu6_nomucomb_mu4_nomucomb_L12MU6_3MU4',
    ]
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, ATR-15503'),
                    }])))

########################################
# supporting

muon_Rules_supporting=RulebookDict(tag,'muon_Rules_supporting')

muon_Rules_supporting.update({
    # ----------------------------------------
    # J/Psi T&P trigger
    # rate so small can stay always unPS    
    'HLT_mu20_2mu2noL1_JpsimumuFS'             : {  1000 : TriggerRule(rate=5, comment='Support, Jpsi tagprobe') },
    'HLT_mu20_2mu4_JpsimumuL2'                 : {  1000 : TriggerRule(rate=5, comment='Support, Jpsi tagprobe') },

    # ----------------------------------------
    # Z T&P idperf
    'HLT_mu13_mu13_idperf_Zmumu'               : {  1000 : TriggerRule(PS=1, comment='Support Express, Jpsi tagprobe', ESRate=0.2) },
})

# ========================================
# many rerun . heavy CPU? 

HLT_list=[
    'HLT_mu26',
    'HLT_mu6_msonly',
    'HLT_mu11_nomucomb','HLT_mu6_nomucomb','HLT_mu4_nomucomb',
    'HLT_mu14_ivarloose', 
    'HLT_mu20_ivarmedium_L1MU10',
]
muon_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support',rerun=1), 
}])))

HLT_list=['HLT_mu20', 'HLT_mu22']
muon_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.4, comment='Support',rerun=1), 
}])))

# ========================================

muon_Rules_supporting.update({

    # ----------------------------------------
    # Level-1
    # low pT used for supporting (also for bJpsi_Trkloose, TrkPEB)
        'L1_MU4'                                   : {  1000  : TriggerRule(rate=930, reoptimize=False, comment='Support L1, also for bjets and bphys TrkPEB (ATR-16689)')	,
                                                        10001 : TriggerRule(rate=50, reoptimize=False, comment='Support L1, also for bjets and bphys TrkPEB (ATR-16689)'), 
									#this one will always be 50 Hz, because for sure MU4 rate > 100 Hz at 1e33 - 1e34.
                                                        },
        'L1_MU6'                                   : {  1000  : TriggerRule(rate=2250, reoptimize=False, comment='Support L1, 2500Hz for TrkPEB chains (ATR-16689)'),
                                                        10001 : TriggerRule(rate=625, reoptimize=False, comment='Support L1, Maxrate for CPU-heavy Trkloose chains, 500Hz for TrkPEB'),
                                                        },
})
    

muon_Rules_supporting.update({
    # ----------------------------------------
    # un-isolated single muon for Background estimation
    'HLT_mu24'                                 : {  1000 : TriggerRule(PS=50, comment='Support, express', rerun=1, ESRate=0.4) },
})


muon_Rules_supporting.update({              

    # 1.5e34
    'HLT_mu24_2j20noL1'                        : {  1000 : TriggerRule(PS=-1, comment='Disable') },
    'HLT_mu24_3j20noL1'                        : {  1000 : TriggerRule(PS=-1, comment='Disable') },
    'HLT_mu24_4j15noL1'                        : {  1000 : TriggerRule(PS=-1, comment='Disable') },
    'HLT_mu24_5j15noL1'                        : {  1000 : TriggerRule(PS=-1, comment='Disable') },


    # ----------------------------------------
    # ~1Hz from each of these.

    # SM W+nJets
    'HLT_mu26_2j20noL1'                        : {  1000 : TriggerRule(PS=224, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    'HLT_mu26_3j20noL1'                        : {  1000 : TriggerRule(PS=156, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    'HLT_mu26_4j15noL1'                        : {  1000 : TriggerRule(PS=156, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    'HLT_mu26_5j15noL1'                        : {  1000 : TriggerRule(PS=104, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    
    })


muon_Rules_supporting.update({              
    # ----------------------------------------
    # prescale + rerun
    # quite hevy CPU wise ?
    'HLT_mu4'                                  : {  1000 : TriggerRule(rate=2.0, ESValue=0, comment='Support', rerun=1) },
    'HLT_mu6'                                  : {  1000 : TriggerRule(rate=1.0, ESValue=0, comment='Support', rerun=1) },
    'HLT_mu14'                                 : {  1000 : TriggerRule(rate=1.0, comment='Support', rerun=1) },
    'HLT_mu10_nomucomb'                        : {  1000 : TriggerRule(rate=1.0, comment='Support', rerun=1) },
})

muon_Rules_supporting.update({              
        # ----------------------------------------
        # L1 streamers
        'HLT_noalg_L1MU4'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495 Express',ESRate=0.1) },
        'HLT_noalg_L1MU6'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495 Express',ESRate=0.1) },
        'HLT_noalg_L1MU10'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495 Express',ESRate=0.1) },
        'HLT_noalg_L1MU11'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495 Express',ESRate=0.1) }, 
        'HLT_noalg_L1MU20'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495 Express',ESRate=0.1) },
        'HLT_noalg_L1MU21'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495 Express',ESRate=0.1) },

        'HLT_noalg_L12MU4'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495') },
        'HLT_noalg_L12MU6'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495') },
        'HLT_noalg_L12MU10'                        : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495') },
        
        'L1_MU4_EMPTY'                             : {  1000 : TriggerRule(PS=50,   scaling='bunches', comment='Support') },
        'L1_MU4_FIRSTEMPTY'                        : {  1000 : TriggerRule(PS=1000*100, scaling='bunches', comment='Support') },
        'L1_MU11_EMPTY'                            : {  1000 : TriggerRule(PS=1,    scaling='bunches', comment='Support') },
        #
        'HLT_noalg_L1MU4_EMPTY'                    : {  1000 : TriggerRule(inputRate=0.5, comment='Support'),
                                                        5001 : TriggerRule(PS=200, comment='Support ATR-14383') },
        
        'HLT_noalg_L1MU4_FIRSTEMPTY'               : {  1000 : TriggerRule(inputRate=0.005, comment='Support') },
        'HLT_noalg_L1MU4_UNPAIRED_ISO'             : {  1000 : TriggerRule(inputRate=0.5, comment='Support') },
        
        'HLT_noalg_L1MU11_EMPTY'                    : {  1000 : TriggerRule(inputRate=0.5, comment='Support'),  
                                                         5001 : TriggerRule(PS=400, comment='Support ATR-14383') },
        })


muon_Rules_supporting.update({ 
    'HLT_mu4_idperf'                           : {  1000 : TriggerRule(rate=0.5, comment='Support', rerun=1) },
    'HLT_mu6_idperf'                           : {  1000 : TriggerRule(rate=0.5, comment='Support Express', ESRate=0.1, rerun=1) },
    'HLT_mu10_idperf'                          : {  1000 : TriggerRule(rate=0.5, comment='Support', rerun=1) },
    'HLT_mu20_idperf'                          : {  1000 : TriggerRule(rate=5, comment='Support Express', ESRate=0.4, rerun=1) },
    'HLT_mu24_idperf'                          : {  1000 : TriggerRule(rate=0.5, comment='Support', rerun=1, ESValue=0) },
    'HLT_mu40_idperf'                          : {  1000 : TriggerRule(rate=0.5, comment='Support Express', ESRate=0.1, rerun=1) },
})

########################################

muon_Rules=RulebookDict(tag,'muon_Rules')

RulesList=[muon_Rules_2e34, muon_Rules_1p5e34, muon_Rules_1e34, muon_Rules_0p7e34, muon_Rules_0p5e34, muon_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in muon_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    muon_Rules.update(Rules)


#################################################################################################
#
#
# B-physics
#
#################################################################################################


bphys_Rules_2p5e34=RulebookDict(tag,'bphys_Rules_2p5e34')

HLT_list=[
    'HLT_mu20_mu6noL1_bTau', # to be replaced by HLT_mu20_mu6btrk_bTauTight after commissioning (ATR-17617)
    'HLT_mu20_mu6btrk_bTauTight',
    'HLT_mu20_2mu2btrk_bTauTight',
    'HLT_2mu10_bJpsimumu',
    'HLT_2mu10_bUpsimumu',
    'HLT_2mu10_bBmumuxv2',
]
if disableBphys:
    bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable, EndOfFill, ATR-15503'), 
                        }])))
else:
    bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-15503'), 
                        }])))
    
HLT_list=[ # new chains as discussed in ATR-15503
    'HLT_mu11_2mu4noL1_bTau_L1MU11_2MU6', # to be replaced by HLT_mu11_2mu2btrk_bTauTight_L1MU11_2MU6 after commissioning (ATR-17617)
    'HLT_mu11_2mu2btrk_bTauTight_L1MU11_2MU6',
    'HLT_mu11_mu6_bUpsimumu',
    'HLT_mu11_mu6_bPhi',
    'HLT_mu11_mu6_bBmumux_BpmumuKp',
    'HLT_mu11_mu6noL1_bPhi_L1MU11_2MU6',
    'HLT_mu11_mu6_bJpsimumu_Lxy0',
    'HLT_mu11_2mu4noL1_bNocut_L1MU11_2MU6', # 4.4 Hz
    'HLT_mu11_mu6_bDimu2700_Lxy0', # subset of *_bDimu2700
    'HLT_mu11_mu6_bDimu_Lxy0', # subset of *_bDimu
    'HLT_mu11_mu6_bDimu2700', # 54.4 Hz
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-15503'),
                    }])))

HLT_list=[
    "HLT_mu11_mu6_bDimu2700_L1LFV-MU11",
    "HLT_mu11_mu6_bBmumux_BpmumuKp_L1LFV-MU11",
    "HLT_mu11_mu6_bJpsimumu_L1LFV-MU11",
    "HLT_mu11_mu6_bJpsimumu_L1LFV-MU",
    "HLT_mu11_mu6_bTau_L1LFV-MU11",
    "HLT_mu11_mu6_bJpsimumu_Lxy0_L1LFV-MU11",
    "HLT_mu11_mu6_bDimu2700_Lxy0_L1LFV-MU11",
    "HLT_mu11_mu6_bDimu_L1LFV-MU11",
    "HLT_mu11_mu6_bBmumuxv2_L1LFV-MU11",
    "HLT_mu11_mu6_bDimu_Lxy0_L1LFV-MU11",
    "HLT_mu11_mu6_bUpsimumu_L1LFV-MU11",
    "HLT_mu11_mu6_bBmumu_L1LFV-MU11",
    "HLT_mu11_mu6_bPhi_L1LFV-MU11",
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-16732'),
                    }])))

HLT_list=[
    "HLT_2mu6_bPhi_L1LFV-MU6",
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-17819'),
                    }])))


HLT_list=[
    "HLT_2mu6_bJpsimumu_L1BPH-2M9-2MU6",
    "HLT_mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4-B",
    "HLT_2mu6_bJpsimumu_L1LFV-MU6",
    ]

bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                1000 : TriggerRule(PS=-1, comment='Disable, ATR-17818'),
                }])))

HLT_list=[

    "HLT_e9_etcut_e5_etcut_bBeexM6000",
    "HLT_e9_etcut_e5_etcut_bBeexM6000_L1BPH-0M9-EM7-EM5", # L1Topo

    "HLT_e9_etcut_bBeexM6000_L1EM7",
    "HLT_e9_etcut_bBeexM6000_L1BPH-0DR3-EM7J15", # L1Topo
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable, ATR-17818'),
                    }])))

HLT_list=[
    'HLT_mu11_mu6_bJpsimumu',
    'HLT_mu11_mu6_bBmumuxv2',
    'HLT_mu11_mu6_bTau',
    'HLT_mu11_mu6_bDimu', # 60.5 Hz
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, ESRate=0.2, comment='Primary, express, ATR-15503'),
                    }])))

HLT_list=[
    'HLT_mu11_mu6_bBmumu', # 4.5 Hz
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, ESRate=0.1, comment='Primary, express, ATR-15503'),
                    }])))


HLT_list=[
    'HLT_2mu6_bBmumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
    'HLT_2mu6_bBmumux_BsmumuPhi_L1BPH-2M9-2MU6_BPH-2DR15-2MU6'
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, ESRate=0.1, comment='Primary L1Topo Bphys, ATR-14108 express'),
    }])))

HLT_list=[
    'HLT_2mu6_bJpsimumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
    'HLT_2mu6_bBmumux_BpmumuKp_L1BPH-2M9-2MU6_BPH-2DR15-2MU6'
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='L1Topo Bphys, ATR-14108'),
    }])))
    
HLT_list=[
    'HLT_2mu6_bJpsimumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
    'HLT_2mu6_bUpsimumu_L1BPH-8M15-2MU6_BPH-0DR22-2MU6'
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, ESRate=0.2, comment='Primary, L1Topo Bphys, ATR-15503, express')  ,
}])))

#Support chains for all-lumi primaries
bphys_Rules_2p5e34.update({
    'HLT_mu11_mu6_bDimu_novtx_noos'      : { 1000 : TriggerRule(rate=0.2, ESValue=1, comment='Support, express')},
    'HLT_mu11_mu6_bDimu_noinvm_novtx_ss' : { 1000 : TriggerRule(rate=0.2, comment='Support')},
    'HLT_2mu6_bDimu'                     : { 1000 : TriggerRule(rate=0.5, comment='Support')},
    'HLT_2mu6_bDimu_novtx_noos_L1BPH-2M9-2MU6_BPH-2DR15-2MU6' : { 1000 : TriggerRule(rate=0.2, comment='Support L1Topo')},
    'HLT_2mu6_bBmumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6' : { 1000 : TriggerRule(PS=20, ESRate=0.1, comment='Support L1Topo, express')},
    'HLT_2mu6_bDimu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6' : {1000 : TriggerRule(rate=0.2, ESValue=0, comment='Support')},
    'HLT_mu11_mu6_bDimu_noinvm_novtx_ss_L1LFV-MU11' : { 1000 : TriggerRule(rate=0.2, comment='Support')},
    'HLT_mu11_mu6_bDimu_novtx_noos_L1LFV-MU11'      : { 1000 : TriggerRule(rate=0.2, comment='Support')},
    'HLT_mu11_mu6_bNocut_L1LFV-MU11'                : { 1000 : TriggerRule(rate=0.2, comment='Support')}
    })

# support triggers in Main (are these really in main? check.)
# and why do we need these, anyways? if they are in main that is not so helpful.
if not disable3MU4:
    HLT_list=[
        'HLT_2mu6_mu4_bNocut_L12MU6_3MU4',
        'HLT_mu6_2mu4_bNocut_L1MU6_3MU4',
        ]
    bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
        1000 : TriggerRule(rate=0.2, comment='Support, ATR-15503'),
        }])))

HLT_list=[
    'HLT_mu11_mu6noL1_bNocut_L1MU11_2MU6',
    'HLT_mu11_mu6_bNocut',
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.2, comment='Support, ATR-15503'),
    }])))


#To check: Is this L1 Rate optimized?
L1_list=['L1_2MU4']
bphys_Rules_2p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule( rate = 1000, reoptimize=False, comment='Support L1'),
                    }])))
#To check: is this L1 Rate optimized?
L1_list=['L1_2MU6'] # 9000 Hz (at some random lumi?)
bphys_Rules_2p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, reoptimize=False, comment='Primary L1'),
                    7001 : TriggerRule(PS=100, reoptimize=False, comment='Support, ATR-16576'),
                    }])))

#is this rate one optimized? seems unlikely... what uses it at 2e34? why is it running?? (nothing). Needs to be fixed!
L1_list=['L1_MU6_2MU4'] #16000 Hz
bphys_Rules_2p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=100, reoptimize=False, comment='Support L1, ATR-16576'), 
                    }])))

#######################################
# 1.5e34 - end of fill items enabled at 1.5e34 in 2017 --> 1.7e34 in 2018

bphys_Rules_1p5e34=RulebookDict(tag,'bphys_Rules_1p5e34')
endOfFillThreshold1 = 17001
endOfFillThreshold2 = 15001
endOfFillThreshold3 = 12001

#primary triggers in B-physics stream
HLT_list=[
    'HLT_mu6_mu4_bBmumux_BsmumuPhi_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4',#13.5 Hz
    'HLT_mu6_mu4_bBmumu_Lxy0_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', # 24.3 Hz
    ]
bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-15503'),
    endOfFillThreshold1 : TriggerRule(PS=-1, comment='Disable, ATR-16576'),
    }])))

# support triggers in B-physics stream below 1.5e34

bphys_Rules_1p5e34.update({
    'HLT_mu6_mu4_bDimu' : { 1000 : TriggerRule(rate=0.5, comment='Support'),
                           endOfFillThreshold1 : TriggerRule(PS=-1, comment='Disabled')},
})

HLT_list=[
    'HLT_mu6_mu4_bBmumu_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', # 32 Hz @2E34
    'HLT_mu6_mu4_bBmumux_BpmumuKp_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4' #110 Hz @ 2e34
]
bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=20, comment='Support, ATR-15503'),
   endOfFillThreshold1 : TriggerRule(PS=-1, comment='Disable, ATR-15503'),
    }])))

HLT_list=[
    'HLT_mu20_mu6noL1_bNocut',
    'HLT_mu6_mu4_bDimu_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4',
    'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4',
]
bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.2, comment='Support, ATR-15503'),
    endOfFillThreshold1 : TriggerRule(PS=-1, comment='Disable, ATR-16576'),
    }])))


HLT_list=['HLT_mu6_mu4_bJpsimumu_Lxy0_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4']
bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 :  TriggerRule(PS=20, ESRate=0.1, comment='Support ATR-15503 express')  ,
    endOfFillThreshold3 : TriggerRule(PS=50, ESRate=0.1, comment='Support ATR-15503 express')  ,
    endOfFillThreshold1 : TriggerRule(PS=-1, ESRate=0.1, comment='Support ATR-15503, 122.5 Hz unPSed at 2E34, express'),
    }])))

HLT_list=[
    'HLT_mu6_mu4_bJpsimumu_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', # 175 Hz at 2e34
]
bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=100, comment='Support, ATR-15503'),
    endOfFillThreshold1 : TriggerRule(PS=-1, comment='Support, ATR-15503'),
    }])))

########################################
#End-of-Fill triggers enabled at 1.2e34 in 2017 --> 1.5e34 in 2018
bphys_Rules_1p2e34=RulebookDict(tag,'bphys_Rules_1p2e34')

#primary triggers
HLT_list=[
    'HLT_mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4-BO', #12 Hz at 2e34
    'HLT_2mu6_bBmumuxv2_L1LFV-MU6', # 90 Hz at 2e34
]

bphys_Rules_1p2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary,EndOfFill, ATR-16219, ATR-15503'),
       endOfFillThreshold2 : TriggerRule(PS=-1, comment='Disable, ATR-16219'),
    }])))

#supporting triggers
HLT_list = [
    'HLT_2mu6_bDimu_novtx_noos_L1LFV-MU6',
    'HLT_2mu6_bDimu_L1LFV-MU6',
    ]
bphys_Rules_1p2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.2, comment='Support, ATR-15503'),
    }])))

########################################
#End-of-Fill triggers enabled at 1.0e34 in 2017 --> 1.2e34 in 2018
bphys_Rules_1e34=RulebookDict(tag,'bphys_Rules_1e34')

#NOTE: This L1Topo item is currently disabled at L1 - so these rules do nothing.

#primary triggers
HLT_list=[
    'HLT_2mu4_bBmumux_BsmumuPhi_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', # 12.7 Hz @2E34
    'HLT_2mu4_bBmumu_Lxy0_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', # 27.7 Hz @2E34
]
bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                  1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-15503'),
   endOfFillThreshold3 : TriggerRule(PS=-1, comment='Disable, ATR-15503'),
    }])))

#support triggers
HLT_list=[
    'HLT_2mu4_bBmumu_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', # 47.3 Hz
]
bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=20, comment='Support, ATR-15503'),
    }])))

HLT_list=[
    'HLT_2mu4_bBmumux_BpmumuKp_L1BPH-2M9-2MU4_BPH-0DR15-2MU4'
]
bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
  1000 : TriggerRule(PS=20, comment='Support, ATR-15503'),
   endOfFillThreshold3 : TriggerRule(PS=-1, comment='Disable'),
    }])))

HLT_list=[
    'HLT_2mu4_bJpsimumu_Lxy0_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', # 158.8 Hz
]
bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=5, comment='Support, ATR-15503'),
   endOfFillThreshold3 : TriggerRule(PS=-1, comment='Disable, ATR-15503'),
    }])))

######################################################
#lower lumi rules
#this turned on at 0.7e34... Likely not so useful? Need to check.
bphys_Rules_lowlumi=RulebookDict(tag,'bphys_Rules_lowlumi')

bphys_Rules_lowlumi.update({
    'HLT_mu6_mu4_bBmumuxv2': { 1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'),
                               4001 : TriggerRule(PS=20, comment='Primary, EndOfFill'),
                               7001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill')}
     })

########################################################
#3mu + 4m chains ---------------------------------------
bphys_Rules_3mu=RulebookDict(tag,'bphys_Rules_3mu')

L1_list=['L1_3MU4']
if do2018:
    bphys_Rules_3mu.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'),
                        Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:
    bphys_Rules_3mu.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'),
                        peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                        }])))


if not disable3MU4:
    HLT_list=[
        'HLT_2mu4_mu3_mu2noL1_bNocut_L13MU4', # 9.5 Hz at 2E34
        'HLT_mu6_2mu4_bDimu',
        'HLT_3mu3_mu3noL1_bNocut_L13MU4', # 2.2 Hz at 2E34
        ]
    bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
        1000 : TriggerRule(PS=1, comment='Primary, ATR-15503, express'),
        }])))

HLT_list=[
    'HLT_3mu4_bPhi', # 2-3 Hz at 2E34
    'HLT_3mu4_bTau',
    'HLT_3mu4_bUpsi',
#    'HLT_3mu4_bDimu2700',  should this stay unprescaled above 1E34?
    'HLT_3mu4_bJpsi',
    'HLT_mu6_2mu4_bDimu2700',
]
bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-15503, express'),
                    }])))
if not disable3MU4:
    HLT_list=['HLT_3mu4_bDimu']
    bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
        1000 : TriggerRule(PS=1, ESRate=0.2, comment='Primary, ATR-15503, express'),
        Primary2017Oct1p7E34 : TriggerRule(PS=-1, ESRate=0, comment='Disable'),
        }])))
    
HLT_list=['HLT_3mu4_bDimu2700']
bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
                1000 : TriggerRule(PS= 1, comment='Support, ATR-16219'),
                10001 : TriggerRule(PS=-1, comment='Disable, ATR-16219'),
                }])))

if not disable3MU4:
    HLT_list=['HLT_3mu4_bNocut']
    bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
        1000 : TriggerRule(rate=0.2, ESValue=1, comment='Support, ATR-15503, express'),
        }])))
    HLT_list=['HLT_3mu4']
    bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
                1000 : TriggerRule(PS=1, comment='Primary below 1.4E34, EndOfFill, ATR-15503'),
                14001 : TriggerRule(PS=4, comment='Support, ATR-17021'),
                }])))
                    # 4mu
                    
if not disable3MU4:
    HLT_list=[
    # 3MU4 seed
        'HLT_3mu3_mu3noL1_L13MU4', # requested by the analysis team finally
        'HLT_3mu4_mu4noL1',
        'HLT_3mu4_mu2noL1', # requested by James initially
        'HLT_4mu4_bNocut', # 4.3 Hz at 2E34
        ]
    bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
        1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-15704'),
        }])))
    
# 4mu (what about this? do we need this? no longer a subset of 3mu4...
HLT_list=[
    # 4MU4 seed
    'HLT_4mu4',
]
bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
            1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-15704'),
            }])))

#chains needed for non-4mu b-phys analyses
HLT_list=[
    'HLT_2mu6_mu4_bTau_L12MU6_3MU4',
    'HLT_mu6_2mu4_bTau_L1MU6_3MU4',
    'HLT_mu6_2mu4_bJpsi',
    'HLT_2mu6_mu4_bUpsi',
    'HLT_mu6_2mu4_bUpsi', # subset of HLT_3mu4_bUpsi, high lumi backup
    ]
bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, 4mu analysis, ATR-15503'),
                    }])))

# 3 mu6 - these always stay enabled? high-lumi backups.
HLT_list=['HLT_3mu6_bTau',
          'HLT_3mu6_bUpsi',
          'HLT_3mu6_bJpsi',
          'HLT_3mu6_bDimu']
bphys_Rules_3mu.update(dict(map(None,HLT_list,len(HLT_list)*[{
                1000 : TriggerRule(PS=1, comment='Primary'),
                }])))

########################################
# supporting trigger

bphys_Rules_supporting=RulebookDict(tag,'bphys_Rules_supporting')

# ========================================
# Support chains for trigger + MCP studies

bphys_Rules_supporting.update({
    # ----------------------------------------
    # idperf at Jpsi mumu
    # 1.5 Hz @ Run1
    'HLT_mu4_mu4_idperf_bJpsimumu_noid'  : {  1000 : TriggerRule(rate=1.5, comment='Support Express, Jpsi idperf', ESRate=0.1) },
    'HLT_mu6_mu2noL1_msonly_bJpsimumu_noid_PEB'  : {  1000 : TriggerRule(rate=1.5, comment='Support, Jpsi idperf, ATR-17600') },
    'HLT_mu6_mu2noL1_msonly_bJpsimumu_noid' : {  1000 : TriggerRule(PS=-1, comment='Disable, Jpsi idperf, ATR-17600') },
    
    # ----------------------------------------
    # Jpsi Trkloose
    'HLT_mu4_bJpsi_Trkloose'             : {  1000 : TriggerRule(PS=-1, comment='Disable, Jpsi idperf')}, 
    'HLT_mu6_bJpsi_Trkloose'             : {  1000 : TriggerRule(PS=-1, comment='Disable')},
    'HLT_mu10_bJpsi_Trkloose'            : {  1000 : TriggerRule(PS=-1, comment='Disable, Jpsi idperf')},
    'HLT_mu14_bJpsi_Trkloose'            : {  1000 : TriggerRule(PS=-1, comment='Disable, Jpsi idperf')},
    'HLT_mu20_bJpsi_Trkloose'            : {  1000 : TriggerRule(PS=-1, comment='Disable')},
#Supporting BPhysics triggers for TrkPEB stream, ATR-16689

    'HLT_mu4_bJpsi_TrkPEB'       : {  1000 : TriggerRule(rate=8.0,  comment='Support, ATR-16689, Jpsi TrkPEB for low-lumi'),
                                      10001: TriggerRule(rate=0.5,  comment='Support, ATR-16689, Jpsi TrkPEB for high-lumi') },
    'HLT_mu6_bJpsi_TrkPEB'       : {  1000 : TriggerRule(rate=20.0, comment='Support, ATR-16689, Jpsi TrkPEB for low-lumi'),
                                      10001: TriggerRule(rate=5.5,  comment='Support, ATR-16689, Jpsi TrkPEB for high-lumi') },
    'HLT_mu6_bJpsi_lowpt_TrkPEB' : {  1000 : TriggerRule(rate=10.0, comment='Support, ATR-16689, Jpsi TrkPEB for low-lumi'),
                                      10001: TriggerRule(rate=4.0,  comment='Support, ATR-16689, Jpsi TrkPEB for high-lumi') },
    'HLT_mu10_bJpsi_TrkPEB'      : {  1000 : TriggerRule(rate=4.0,  comment='Support, ATR-16689, Jpsi TrkPEB for low-lumi'),
                                      10001: TriggerRule(rate=1.0,  comment='Support, ATR-16689, Jpsi TrkPEB for high-lumi') },
    'HLT_mu10_bJpsi_TrkPEBmon'   : {  1000 : TriggerRule(rate=0.2, ESRate=0.2,  comment='Support, ATR-16689,ATR-17203, Jpsi TrkPEB') },
    'HLT_mu11_bJpsi_TrkPEB'      : {  1000 : TriggerRule(PS=-1,  comment='Disable, ATR-17054, Jpsi TrkPEB')}, 
    'HLT_mu14_bJpsi_TrkPEB'      : {  1000 : TriggerRule(PS=-1,  comment='Disable, ATR-16689, Jpsi TrkPEB')},
    'HLT_mu20_bJpsi_TrkPEB'      : {  1000 : TriggerRule(PS=-1,  comment='Disable, ATR-16689, Jpsi TrkPEB')},
})

########################################

#always disabled
bphys_Rules_disabled=RulebookDict(tag,'bphys_Rules_disabled')
HLT_list=[
    'HLT_3mu4_bVertex2',
    'HLT_3mu4_bVertex3',
    'HLT_3mu4_nomucomb',
    'HLT_3mu3_mu3noL1_calotag_0eta010_L13MU4',
    'HLT_3mu4_mu4noL1_calotag_0eta010',
    'HLT_3mu4_nomucomb_bTau',
    'HLT_mu6_nomucomb_2mu4_nomucomb_delayed_L1MU6_3MU4',
    'HLT_mu6_nomucomb_2mu4_nomucomb_L1MU6_3MU4',
    'HLT_mu6_2mu4', #88Hz at2e34
#    'HLT_mu6_2mu4_bJpsi', disabled in 2017, enabled in 2018 (confirmed by Heather)
    
    'HLT_2mu10_bJpsimumu_noL2',
    'HLT_2mu10_bDimu_noL2',

    'HLT_mu10_mu6_bBmumuxv2_L1LFV-MU', # 20 Hz
    'HLT_mu11_2mu4noL1_bNocut_L1LFV-MU', # 3.9 Hz
    'HLT_mu11_mu6noL1_bTau_L1LFV-MU',
    'HLT_mu11_2mu4noL1_bTau_L1LFV-MU',
    
    'HLT_mu10_mu6_bBmumuxv2',
    'HLT_mu10_mu6_bBmumux_Taumumux_noL2',
    'HLT_mu10_mu6_bBmumux_Taumumux',
    'HLT_mu10_mu6_bJpsimumu', 
    'HLT_mu10_mu6_bJpsimumu_Lxy0',
    'HLT_mu10_mu6_bUpsimumu',
    'HLT_mu10_mu6_bBmumux_BpmumuKp',
    
    'HLT_2mu6_bPhi',
    'HLT_2mu6_bJpsimumu',
    'HLT_2mu6_bBmumuxv2',
    'HLT_2mu6_bBmumu_Lxy0',
    'HLT_2mu6_bJpsimumu_Lxy0',
    'HLT_2mu6_bBmumux_Taumumux',
    'HLT_2mu6_bBmumux_BpmumuKp',
    'HLT_2mu6_bUpsimumu',
    'HLT_2mu6_bDimu_Lxy0',
    'HLT_2mu6_bBmumux_BsmumuPhi',
    'HLT_2mu6_bDimu_noinvm_novtx_ss',
    'HLT_2mu6_bDimu_novtx_noos',
    
    'HLT_mu6_mu4_bUpsimumu',
    'HLT_mu6_mu4_bDimu_noinvm_novtx_ss',
    'HLT_mu6_mu4_bDimu_novtx_noos',
    'HLT_mu6_mu4_bBmumux_BpmumuKp',
    'HLT_mu6_mu4_bBmumu',
    'HLT_mu6_mu4_bBmumu_Lxy0',
    'HLT_mu6_mu4_bJpsimumu',
    'HLT_mu6_mu4_bJpsimumu_Lxy0',
    
    'HLT_2mu4_bBmumux_BpmumuKp',
    'HLT_2mu4_bJpsimumu',
    'HLT_2mu4_bUpsimumu',

    'HLT_2mu6_bDimu_L12MU6-B',

    'HLT_2mu6_bJpsimumu_L12MU6-BO',
    'HLT_2mu6_bBmumu_L12MU6-BO',
    'HLT_2mu6_bUpsimumu_L12MU6-BO',
    'HLT_2mu6_bDimu_L12MU6-BO',
    
    'HLT_2mu6_bUpsimumu_L1BPH-8M15-2MU6',

    'HLT_2mu6_bBmumuxv2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
    
#    'HLT_mu6_mu4_bBmumux_BpmumuKp_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4',
    'HLT_mu6_mu4_bBmumuxv2_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4',
    
    'HLT_2mu4_bJpsimumu_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',
    'HLT_2mu4_bDimu_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',
    'HLT_2mu4_bJpsimumu_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',
    'HLT_2mu4_bDimu_novtx_noos_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',
]
bphys_Rules_disabled.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, ATR-16576'), 
                    }])))

#disabled chains that are defined as express stream, so that needs disabling too
HLT_list = [
    'HLT_mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4',
    'HLT_mu11_mu6noL1_bTau_L1MU11_2MU6',
    'HLT_2mu6_bDimu_L1BPH-8M15-2MU6',
    'HLT_2mu6_bBmumu'
    ]
bphys_Rules_disabled.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, ESValue=0, comment='Disabled, ATR-16576'),
                    }])))

HLT_list=[
    'HLT_2mu6_bBmumux_BcmumuDsloose_L1LFV-MU6',
    'HLT_2mu10_bBmumux_BcmumuDsloose',
    'HLT_mu10_mu6_bBmumux_BcmumuDsloose',
    'HLT_2mu6_bBmumux_BcmumuDsloose',
    'HLT_2mu6_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 
    'HLT_mu6_mu4_bBmumux_BcmumuDsloose',
    'HLT_2mu4_bBmumux_BcmumuDsloose',
    'HLT_mu11_mu6_bBmumux_BcmumuDsloose',
    'HLT_2mu6_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
    'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4',
    'HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',
]
bphys_Rules_disabled.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1,comment='Disable (memory leak)'), 
                    }])))

bphys_Rules=RulebookDict(tag,'bphys_Rules')
RulesList=[]
if disableEoFChains:
    RulesList = [bphys_Rules_2p5e34, bphys_Rules_3mu, bphys_Rules_supporting, bphys_Rules_disabled]
else:
    RulesList=[bphys_Rules_2p5e34, bphys_Rules_1p5e34, bphys_Rules_1p2e34, bphys_Rules_1e34, bphys_Rules_lowlumi, bphys_Rules_3mu, bphys_Rules_supporting,bphys_Rules_disabled]
 
for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in bphys_Rules:
            pass # print 'FATAL  Physics_pp_v7_rules   Duplicated rule inside bphys_Rules. Cannot add rule: ',newrule
    bphys_Rules.update(Rules)






#################################################################################################
#
#
# Missing ET
#
#
#################################################################################################



########################################
# 2p5e34 

met_Rules_2p5e34=RulebookDict(tag,'met_Rules_2p5e34')

L1_list=['L1_XE70', 'L1_XE80']
met_Rules_2p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'), 
                    }])))


HLT_list=["HLT_xe120_pufit_L1XE70",]
met_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Backup HLT'), 
                    }])))
    
########################################
# 2e34


met_Rules_2e34=RulebookDict(tag,'met_Rules_2e34')

L1_list=['L1_XE60'] #4000 Hz
met_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'), 
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))


HLT_list=[
    "HLT_xe120_pufit_L1XE60",
    "HLT_xe120_pufit_wEFMu_L1XE60",
]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                    }])))    
HLT_list=[
    "HLT_xe110_pufit_xe70_L1XE50",
    "HLT_xe110_pufit_xe75_L1XE50",

    "HLT_xe110_pufit_xe65_L1XE55",
    "HLT_xe110_pufit_xe70_L1XE55",
    "HLT_xe110_pufit_xe75_L1XE55",

    "HLT_xe100_pufit_xe75_L1XE60",
    "HLT_xe100_pufit_xe80_L1XE60",
    "HLT_xe100_pufit_xe85_L1XE60",
    "HLT_xe110_pufit_xe65_L1XE60",
    "HLT_xe110_pufit_xe70_L1XE60",
    "HLT_xe110_pufit_xe75_L1XE60",
]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-17405'), 
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                    }])))    

HLT_list=[
    "HLT_xe110_pufit_xe65_L1XE50",

    "HLT_xe100_pufit_xe75_L1XE50",
    "HLT_xe100_pufit_xe80_L1XE50",
    "HLT_xe100_pufit_xe85_L1XE50",

    "HLT_xe100_pufit_xe75_L1XE55",
    "HLT_xe100_pufit_xe85_L1XE55",
    "HLT_xe100_pufit_xe80_L1XE55",

    "HLT_xe110_pufit_xe60_L1XE60",
    "HLT_xe110_pufit_L1XE70",    
    ]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=5, comment='Support, ATR-17405'), 
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    "HLT_xe120_trkmht_xe50_L1XE50",
    ]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                100 : TriggerRule(PS=-1, comment='Disable, ATR-17814'), 
                }])))


HLT_list=[
    "HLT_xe100_pufit_xe120_mht_L1XE50",
    "HLT_xe100_pufit_xe130_mht_L1XE50",
    "HLT_xe100_pufit_xe120_mht_L1XE55",
    "HLT_xe100_pufit_xe130_mht_L1XE55",
    
    "HLT_xe110_pufit_xe110_mht_L1XE50",
    "HLT_xe110_pufit_xe110_mht_L1XE55",
    "HLT_xe110_pufit_xe120_mht_L1XE50",
    "HLT_xe110_pufit_xe120_mht_L1XE55",

    "HLT_xe100_pufit_xe120_mht_L1XE60",
    "HLT_xe100_pufit_xe130_mht_L1XE60",

    "HLT_xe110_pufit_xe100_mht_L1XE60",
    "HLT_xe110_pufit_xe110_mht_L1XE60",
    "HLT_xe110_pufit_xe120_mht_L1XE60",
    ]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=100, comment='Prescale, ATR-17405'), 
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    'HLT_xe140_trkmht_xe90_mht_L1XE60',
    'HLT_xe140_trkmht_xe100_mht_L1XE60',
    'HLT_xe140_trkmht_xe90_mht_L1XE70',
    'HLT_xe140_trkmht_xe100_mht_L1XE70',
]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled until cost issue is solved'), 
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    "HLT_xe90_pufit_L1XE70",
    "HLT_xe95_pufit_L1XE70",
    "HLT_xe100_pufit_L1XE70",

    "HLT_xe90_pufit_wEFMu_L1XE70",
    "HLT_xe95_pufit_wEFMu_L1XE70",
    "HLT_xe100_pufit_wEFMu_L1XE70",
    "HLT_xe110_pufit_wEFMu_L1XE70",
    "HLT_xe120_pufit_wEFMu_L1XE70",

    "HLT_xe110_pufit_wEFMu_L1XE60",
    
    "HLT_xe120_tc_em_L1XE60",
    "HLT_xe120_tc_em_wEFMu_L1XE60",
    "HLT_xe120_tc_lcw_wEFMu_L1XE60",

    "HLT_xe120_tc_lcw_L1XE70",
    "HLT_xe120_tc_em_L1XE70",
    
    "HLT_xe130_tc_lcw_L1XE60",

    "HLT_xe120_mht_wEFMu_L1XE60",    
    "HLT_xe120_mht_em_wEFMu_L1XE60",

    "HLT_xe120_wEFMu_L1XE60",

    "HLT_xe130_mht_L1XE60",    

    ]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support, ATR-15777'),
                    5001 : TriggerRule(PS=-1, comment='Disable, ATR-15777'),
                    }])))

HLT_list=[
    "HLT_xe90_pufit_L1XE60",
    "HLT_xe90_pufit_wEFMu_L1XE60",
    "HLT_xe95_pufit_L1XE60",
    "HLT_xe95_pufit_wEFMu_L1XE60",
    "HLT_xe100_mht_xe80_L1XE60",
    "HLT_xe110_mht_xe80_L1XE60",
    'HLT_xe110_L1XE50','HLT_xe110_L1XE55',
    'HLT_xe110_wEFMu_L1XE50','HLT_xe110_wEFMu_L1XE55', 
    'HLT_xe120_L1XE55','HLT_xe130_L1XE55',
    'HLT_xe120_L1XE60', 'HLT_xe130_L1XE60',
    "HLT_xe100_pufit_wEFMu_L1XE50",
    "HLT_xe100_pufit_wEFMu_L1XE55",
    "HLT_xe100_pufit_wEFMu_L1XE60",
    ]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable, ATR-17814'),
                    }])))

HLT_list=[
    #   trkmht with lower thresholds (rates >~30Hz at 2e34)
    "HLT_xe100_trkmht_xe80_mht_L1XE50", "HLT_xe100_trkmht_xe80_mht_L1XE55", "HLT_xe100_trkmht_xe80_mht_L1XE60", "HLT_xe100_trkmht_xe90_mht_L1XE50", "HLT_xe100_trkmht_xe90_mht_L1XE55", "HLT_xe100_trkmht_xe90_mht_L1XE60", "HLT_xe110_trkmht_wEFMu_xe100_mht_L1XE50", "HLT_xe110_trkmht_wEFMu_xe100_mht_L1XE55", "HLT_xe110_trkmht_wEFMu_xe80_mht_L1XE50", "HLT_xe110_trkmht_wEFMu_xe80_mht_L1XE55", "HLT_xe110_trkmht_wEFMu_xe90_mht_L1XE50", "HLT_xe110_trkmht_wEFMu_xe90_mht_L1XE55", "HLT_xe110_trkmht_xe100_mht_L1XE50", "HLT_xe110_trkmht_xe100_mht_L1XE55", "HLT_xe110_trkmht_xe100_mht_L1XE60", "HLT_xe110_trkmht_xe100_mht_L1XE70", "HLT_xe110_trkmht_xe80_mht_L1XE50", "HLT_xe110_trkmht_xe80_mht_L1XE55", "HLT_xe110_trkmht_xe80_mht_L1XE60", "HLT_xe110_trkmht_xe90_mht_L1XE50", "HLT_xe110_trkmht_xe90_mht_L1XE55", "HLT_xe110_trkmht_xe90_mht_L1XE60", "HLT_xe120_trkmht_wEFMu_xe100_mht_L1XE60", "HLT_xe120_trkmht_wEFMu_xe100_mht_L1XE70", "HLT_xe120_trkmht_wEFMu_xe80_mht_L1XE60", "HLT_xe120_trkmht_wEFMu_xe90_mht_L1XE60", "HLT_xe120_trkmht_xe100_mht_L1XE50", "HLT_xe120_trkmht_xe100_mht_L1XE55", "HLT_xe120_trkmht_xe100_mht_L1XE60", "HLT_xe120_trkmht_xe100_mht_L1XE70", "HLT_xe120_trkmht_xe80_mht_L1XE50", "HLT_xe120_trkmht_xe80_mht_L1XE55", "HLT_xe120_trkmht_xe80_mht_L1XE60", "HLT_xe120_trkmht_xe90_mht_L1XE50", "HLT_xe120_trkmht_xe90_mht_L1XE55", "HLT_xe120_trkmht_xe90_mht_L1XE60", "HLT_xe130_trkmht_xe100_mht_L1XE55", "HLT_xe130_trkmht_xe100_mht_L1XE60", "HLT_xe130_trkmht_xe100_mht_L1XE70", "HLT_xe130_trkmht_xe80_mht_L1XE55", "HLT_xe130_trkmht_xe80_mht_L1XE60", "HLT_xe130_trkmht_xe90_mht_L1XE55", "HLT_xe130_trkmht_xe90_mht_L1XE60", "HLT_xe140_trkmht_xe80_mht_L1XE60", "HLT_xe90_trkmht_xe80_mht_L1XE50", "HLT_xe90_trkmht_xe80_mht_L1XE55", "HLT_xe90_trkmht_xe80_mht_L1XE60",
]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled Commission until cost issue is solved'), 
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

########################################
# 1.5e34 

met_Rules_1p5e34=RulebookDict(tag,'met_Rules_1p5e34')

L1_list=['L1_XE55'] 
met_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'), 
                    Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

L1_list=['L1_XE50']
met_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   Primary2017Oct1p7E34 : TriggerRule(rate=100, comment='Support'),
                   }])))

HLT_list=[
    #primary
    'HLT_xe130_mht_L1XE50',
    "HLT_xe120_mht_xe80_L1XE60",
]

met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                   12001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_xe120_pufit_L1XE55',
    'HLT_xe120_pufit_wEFMu_L1XE55',
    ]
met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_xe120_mht_xe80_L1XE55',
    'HLT_xe130_mht_xe80_L1XE55',
]
if do2018:
    met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:
    met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                        Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))


HLT_list=[
    'HLT_xe110_pufit_L1XE50',
    "HLT_xe110_pufit_wEFMu_L1XE50", 
    'HLT_xe110_pufit_L1XE55',
    'HLT_xe110_pufit_wEFMu_L1XE55',
    "HLT_xe110_pufit_L1XE60",
    ]
met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable'),
                    }])))
    
HLT_list=[
    "HLT_xe120_pufit_L1XE50",
    "HLT_xe120_pufit_wEFMu_L1XE50",
    "HLT_xe120_mht_xe80_L1XE50",
    ]
met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_xe100_pufit_L1XE50', # 130Hz at 1.5e34
    'HLT_xe100_pufit_L1XE55', # 125Hz at 1.5e34
    'HLT_xe100_pufit_L1XE60', # 100Hz at 1.5e34
    ]
met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary EndOfFill'),
                    5001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

########################################
# 1e34 
met_Rules_1e34=RulebookDict(tag,'met_Rules_1e34')
HLT_list=[
    "HLT_xe90_pufit_L1XE50", "HLT_xe90_pufit_L1XE55",
    "HLT_xe90_pufit_wEFMu_L1XE50", "HLT_xe90_pufit_wEFMu_L1XE55", 

    'HLT_xe95_pufit_L1XE50', 'HLT_xe95_pufit_L1XE55',
    "HLT_xe95_pufit_wEFMu_L1XE50", "HLT_xe95_pufit_wEFMu_L1XE55", 
]
met_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary EndOfFill'),
                    9001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_xe130_pufit_L1XE50', 'HLT_xe130_pueta_L1XE50',
    'HLT_xe130_pufit_L1XE55', 'HLT_xe130_pueta_L1XE55',
    'HLT_xe130_pufit_wEFMu_L1XE50', 'HLT_xe130_pueta_wEFMu_L1XE50',
    'HLT_xe130_pufit_wEFMu_L1XE55', 'HLT_xe130_pueta_wEFMu_L1XE55',
    ]
met_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


########################################
# 0.5e34 

met_Rules_0p5e34=RulebookDict(tag,'met_Rules_0p5e34')

# same as what runs at 1e34
HLT_list=[
    'HLT_xe90_mht_L1XE50','HLT_xe90_mht_wEFMu_L1XE50',
    'HLT_xe90_mht_L1XE55','HLT_xe90_mht_wEFMu_L1XE55',
    ]
met_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_xe110_pueta_L1XE50',  'HLT_xe120_pueta_L1XE50',
    "HLT_xe120_pueta_L1XE60",
    "HLT_xe110_pueta_wEFMu_L1XE50", 
    "HLT_xe110_pueta_wEFMu_L1XE55", 
    "HLT_xe120_pueta_wEFMu_L1XE50", 
    "HLT_xe120_pueta_wEFMu_L1XE55",

    ]
met_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14446'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

########################################
# supporting trigger

met_Rules_supporting=RulebookDict(tag,'met_Rules_supporting')

HLT_list=[
    'HLT_xe35_mht_em', 'HLT_xe35',    'HLT_xe35_mht', 'HLT_xe35_tc_lcw', 'HLT_xe35_pufit', 'HLT_xe35_tc_em',
    #'HLT_xe35_trkmht',
    'HLT_xe45_pufit_wEFMu', 'HLT_xe45_wEFMu', 'HLT_xe45_mht_wEFMu', 'HLT_xe45_pueta', 'HLT_xe45_tc_lcw', 'HLT_xe45_tc_em', 'HLT_xe45_mht',   'HLT_xe45_pueta_wEFMu', 'HLT_xe45_tc_lcw_wEFMu', 'HLT_xe45_pufit', 'HLT_xe45', 'HLT_xe45_tc_em_wEFMu', 'HLT_xe45_mht_xe45',
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled ATR-14226'),  
                    }])))

#----------------------------------------

HLT_list=['HLT_xe50']  # rerun needed for tau (ATR-11530)
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=0, comment='Disabled ATR-14226', rerun=1), 
                    }])))

# ========================================
# very important
# rerun, so very expensive. But really needed to do emaulation for all cases, and measure efficiency in data.

HLT_list=[
    'HLT_xe0noL1_l2fsperf',       'HLT_xe0noL1_l2fsperf_mht',
    'HLT_xe0noL1_l2fsperf_pufit', 'HLT_xe0noL1_l2fsperf_tc_em', 'HLT_xe0noL1_l2fsperf_tc_lcw',
    #'HLT_xe0noL1_l2fsperf_trkmht',
    'HLT_xe0noL1_l2fsperf_mht_em',
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support',rerun=1), 
    }])))

# ========================================

HLT_list=['HLT_noalg_L1XE300']
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support ATR-14177'),
                   14001 : TriggerRule(PS=10, comment='Support ATR-14177, needs higher threshold for PS=1'),
    }])))

HLT_list=['HLT_noalg_L1XE40', 'HLT_noalg_L1XE45',]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(inputRate=0.5, comment='Support, ATR-17754'), 
    }])))

HLT_list=['HLT_noalg_L1XE30', 'HLT_noalg_L1XE35', ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(inputRate=0.25, comment='Support, ATR-17754'), 
                   }])))

HLT_list=['HLT_noalg_L1XE50',]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(inputRate=3, comment='Support, ATR-17754,  Express ATR-17782', ESRate=0.5), 
                   }])))

# ========================================
HLT_list=['HLT_noalg_L1RD0_FILLED']
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(inputRate=15, comment='Support Express',ESValue=75), ## Make sure the PS*ES=0.2 Hz
    }])))

HLT_list=['HLT_noalg_L1RD0_EMPTY']
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, comment='Support Express',ESValue=1),
    }])))


########################################

met_Rules=RulebookDict(tag,'met_Rules')

RulesList=[met_Rules_2p5e34, met_Rules_2e34, met_Rules_1p5e34, met_Rules_1e34, met_Rules_0p5e34, met_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in met_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    met_Rules.update(Rules)



#################################################################################################
#
#
# Jet
#
#
#################################################################################################


########################################
# 2e34 

jet_Rules_2e34=RulebookDict(tag,'jet_Rules_2e34')


# L1 primaries and backup primaries for 2e34
L1_list = [
    # Single-jet
    'L1_J100',
    'L1_J120',
    
    # Forward-jet
    'L1_J75.31ETA49',
    #'L1_J100.31ETA49',

    # 3-jet
    'L1_3J50',

    # 4-jet
    'L1_4J15.0ETA25',
    'L1_4J20',

    # 5-jet
    'L1_5J15.0ETA25',

    # 6-jet
    'L1_6J15',
]
jet_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary L1'), 
                     }])))

HLT_list = [
    # 1-jet GSC
    'HLT_j225_gsc440_boffperf_split',

    # 4-jet
    'HLT_4j120',

    # 4-jet GSC
    'HLT_4j85_gsc115_boffperf_split',
    ]
if do2018:
    jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable'),
                        }])))
else:
    jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'),
                        }])))
    
# HLT primaries for 2e34
HLT_list = [
    # 1-jet
    'HLT_j450',

    # 1-jet GSC
    #'HLT_j225_gsc440_boffperf_split',

    # forward jet
    'HLT_j260_320eta490',

    # large-R topocluster jet
    'HLT_j480_a10_lcw_subjes_L1J100',

    # large-R reclustered jet
    'HLT_j480_a10r_L1J100',

    # large-R trimmed topocluster jet
    'HLT_j480_a10t_lcw_jes_L1J100',

    # large-R trimmed+mass-cut single-jet
    'HLT_j440_a10t_lcw_jes_40smcINF_L1J100',

    # large-R trimmed+mass-cut di-jet
    'HLT_2j330_a10t_lcw_jes_35smcINF_L1J100',
    'HLT_2j330_a10t_lcw_jes_40smcINF_L1J100', # If this is modified, support triggers have to change too (search for 2j330)
    'HLT_2j340_a10t_lcw_jes_35smcINF_L1J100',
    'HLT_2j340_a10t_lcw_jes_40smcINF_L1J100',
    'HLT_2j350_a10t_lcw_jes_35smcINF_L1J100',
    'HLT_2j350_a10t_lcw_jes_40smcINF_L1J100', 

    'HLT_j420_a10t_lcw_jes_35smcINF_L1J100',
    'HLT_j440_a10t_lcw_jes_35smcINF_L1J100',
    
    # 3-jet
    'HLT_3j225',

    # 4-jet
    #'HLT_4j120',

    # 4-jet GSC
    #'HLT_4j60_gsc115_boffperf_split', 

    # 5-jet
    'HLT_5j90_L14J150ETA25',
    'HLT_5j90', #L1_4J20

    # 5-jet central
    'HLT_5j75_0eta240_L14J150ETA25',
    'HLT_5j75_0eta240', #L1_4J20

    # 5-jet central GSC
    #'HLT_5j50_gsc70_boffperf_split_0eta240_L14J150ETA25',## disable
    #'HLT_5j50_gsc70_boffperf_split_0eta240', #L1_4J20## disable

    # 6-jet
    'HLT_6j70_L14J150ETA25',
    'HLT_6j70', #L1_4J20

    # 6-jet GSC
    'HLT_6j50_gsc70_boffperf_split_L14J150ETA25',
    'HLT_6j50_gsc70_boffperf_split', #L1_4J20
    'HLT_6j50_gsc70_boffperf_split_L15J150ETA25',

    # 6-jet central
    'HLT_6j60_0eta240_L14J150ETA25',
    'HLT_6j60_0eta240_L14J20',
    'HLT_6j60_0eta240_L15J150ETA25',

    # 6-jet central GSC
    'HLT_6j45_gsc55_boffperf_split_0eta240_L14J20',
    'HLT_6j45_gsc55_boffperf_split_0eta240_L15J150ETA25',

    # 7-jet
    'HLT_7j45_L14J150ETA25',
    'HLT_7j45_L14J20',
    'HLT_7j45', #L1_6J15

    # 7-jet GSC
    'HLT_7j35_gsc45_boffperf_split_L14J150ETA25',
    'HLT_7j35_gsc45_boffperf_split_L14J20',
    'HLT_7j35_gsc45_boffperf_split_L15J150ETA25',

    # 7-jet central
    'HLT_7j45_0eta240_L14J150ETA25',
    'HLT_7j45_0eta240_L14J20',
    'HLT_7j45_0eta240_L15J150ETA25',
    'HLT_7j45_0eta240', #L1_6J15

    # 7-jet central GSC
    'HLT_7j35_gsc45_boffperf_split_0eta240_L14J150ETA25',
    'HLT_7j35_gsc45_boffperf_split_0eta240_L14J20',
    'HLT_7j35_gsc45_boffperf_split_0eta240_L15J150ETA25',

    # 10-jet (Razor)
    'HLT_10j40_L14J150ETA25',
    'HLT_10j40_L14J20',
    'HLT_10j40_L16J15',

    # HT
    'HLT_ht1000_L1J100',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary'), 
                       }])))

HLT_list = [
    'HLT_2j330_a10t_lcw_jes_35smcINF_L1SC111',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, Express ATR-17782', ESRate=0.2), 
                    }])))

HLT_list = [
    'HLT_2j340_a10t_lcw_jes_35smcINF_L1SC111',
    'HLT_2j350_a10t_lcw_jes_35smcINF_L1SC111',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

HLT_list = [
    # 6-jet central GSC
    'HLT_6j45_gsc55_boffperf_split_0eta240_L14J150ETA25',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary, Express ATR-17782', ESRate=0.2), 
                       }])))

# HLT backup primaries for 2e34
HLT_list = [
    # 1-jet
    # See below for backup j460, which is also an express stream chain
    'HLT_j480',
    # See below for backup j500, which also is a pass-through chain
    'HLT_j520',

    # 1-jet GSC
    'HLT_j225_gsc450_boffperf_split',
    'HLT_j225_gsc460_boffperf_split',
    'HLT_j225_gsc480_boffperf_split',
    'HLT_j225_gsc500_boffperf_split',

    # forward jet
    # See below for backup j360_320eta490, which also is a pass-through chain

    # large-R topocluster jet
    'HLT_j520_a10_lcw_subjes_L1J100',
    'HLT_j540_a10_lcw_subjes_L1J100',

    # large-R reclustered jet
    'HLT_j500_a10r_L1J100',
    'HLT_j520_a10r_L1J100',
    'HLT_j540_a10r_L1J100',

    # large-R trimmed topocluster jet
    'HLT_j500_a10t_lcw_jes_L1J100',
    'HLT_j520_a10t_lcw_jes_L1J100',
    'HLT_j540_a10t_lcw_jes_L1J100',

    # large-R trimmed+mass-cut single-jet
    # No backup items as it's not useful anymore if the threshold is raised

    # 3-jet
    'HLT_3j250',

    # 4-jet
    'HLT_4j130',

    # 4-jet GSC
    'HLT_4j85_gsc120_boffperf_split',
    'HLT_4j85_gsc130_boffperf_split',

    # 5-jet
    'HLT_5j100_L14J150ETA25',
    'HLT_5j100', #L1_4J20

    # 5-jet GSC
    'HLT_5j60_gsc90_boffperf_split_L14J150ETA25',
    'HLT_5j60_gsc90_boffperf_split', #L1_4J20
    'HLT_5j85_gsc100_boffperf_split_L14J150ETA25',
    'HLT_5j85_gsc100_boffperf_split', #L1_4J20

    # 5-jet central
    'HLT_5j85_0eta240_L14J150ETA25',
    'HLT_5j85_0eta240', #L1_4J20

    # 5-jet central GSC
    'HLT_5j55_gsc75_boffperf_split_0eta240_L14J150ETA25',
    'HLT_5j55_gsc75_boffperf_split_0eta240', #L1_4J20
    
    # 6-jet
    'HLT_6j85_L14J150ETA25',
    'HLT_6j85', #L1_4J20

    # 6-jet GSC
    'HLT_6j60_gsc85_boffperf_split_L14J150ETA25',
    'HLT_6j60_gsc85_boffperf_split', #L1_4J20
    'HLT_6j60_gsc85_boffperf_split_L15J150ETA25',

    # 6-jet central
    # None foreseen, fall back on the GSC 6j central trigger below instead

    # 6-jet central GSC
    'HLT_6j45_gsc60_boffperf_split_0eta240_L14J150ETA25',
    'HLT_6j45_gsc60_boffperf_split_0eta240_L14J20',
    'HLT_6j45_gsc60_boffperf_split_0eta240_L15J150ETA25',

    # 7-jet
    'HLT_7j50_L14J150ETA25',
    'HLT_7j50_L14J20',
    'HLT_7j50', #L1_6J15

    # 7-jet GSC
    'HLT_7j35_gsc50_boffperf_split_L14J150ETA25',
    'HLT_7j35_gsc50_boffperf_split_L14J20',
    'HLT_7j35_gsc50_boffperf_split_L15J150ETA25',

    # 7-jet central
    # None foreseen, fall back on the GSC 7j central trigger below instead

    # 7-jet central GSC
    'HLT_7j35_gsc50_boffperf_split_0eta240_L14J150ETA25',
    'HLT_7j35_gsc50_boffperf_split_0eta240_L14J20',
    'HLT_7j35_gsc50_boffperf_split_0eta240_L15J150ETA25',

    # 10-jet (Razor)
    # Not needed, rate is negligible

    # HT
    'HLT_ht1200_L1J100',
    'HLT_ht1400_L1J100',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary backup')
                   }])))

HLT_list = [ # large-R topocluster jet
    'HLT_j500_a10_lcw_subjes_L1J100',]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

# HLT backup primaries for 2e34, express stream
HLT_list = ['HLT_j460']
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary backup'), 
                    }])))


# HLT backup primaries for 2e34, pass-through
HLT_list = [
    'HLT_j500', # Raised from j400
    #'HLT_j360_320eta490',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary backup, pass-through 1Hz ATR-11862', PTRate=1.0),
                    }])))


##### START L1Topo primaries and backup primaries
# HLT primaries for 2e34 (with L1Topo seeds)
HLT_list = [
        # large-R topocluster jet
        'HLT_j480_a10_lcw_subjes_L1HT190-J15.ETA21',
        'HLT_j480_a10_lcw_subjes_L1HT190-J15s5.ETA21',
    
        # large-R reclustered jet
        'HLT_j480_a10r_L1HT190-J15.ETA21',
        'HLT_j480_a10r_L1HT190-J15s5.ETA21',
    
        # large-R trimmed topocluster jet
        'HLT_j480_a10t_lcw_jes_L1HT190-J15.ETA21',
        'HLT_j480_a10t_lcw_jes_L1HT190-J15s5.ETA21',
    
        # large-R trimmed+mass-cut single-jet
        'HLT_j440_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',
    
        # large-R trimmed+mass-cut di-jet
        'HLT_2j330_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',
    
        # HT triggers
        'HLT_ht1000_L1HT190-J15.ETA21',
        'HLT_ht1000_L1HT190-J15s5.ETA21',
        ]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled, L1Topo')
    }])))
    

# HLT backup primaries for 2e34 (with L1Topo seeds)
HLT_list = [
        # large-R trimmed+mass-cut di-jet
        'HLT_2j340_a10t_lcw_jes_40smcINF_L1SC111',
        'HLT_2j340_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',
        'HLT_2j350_a10t_lcw_jes_40smcINF_L1SC111',
        'HLT_2j350_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',
    
        # HT triggers
        'HLT_ht1200_L1HT190-J15.ETA21',
        'HLT_ht1200_L1HT190-J15s5.ETA21',
        'HLT_ht1400_L1HT190-J15.ETA21',
        'HLT_ht1400_L1HT190-J15s5.ETA21',
    ]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled, L1Topo')
}])))

##### END L1Topo primaries and backup primaries







# Analysis-specific primaries
# Several chains for the moment, we need to choose one primary and one backup before data taking begins
# This will be done using the results of running the below in the next reprocessing
HLT_list=[
    'HLT_2j225_gsc250_boffperf_split_0eta240_j85_gsc120_boffperf_split_0eta240',
    'HLT_2j225_gsc250_boffperf_split_0eta240_j85_gsc140_boffperf_split_0eta240',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, exotics resonance searches in multijets, ATR-15939',),
                    peakLumi : TriggerRule(PS=-1, comment='Disabled',), 
                       }])))





########################################
# 1.7e34

jet_Rules_1p7e34=RulebookDict(tag,'jet_Rules_1p7e34')

# L1 primaries for 1.7e34
L1_list=['L1_4J15',]
jet_Rules_1p7e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled L1'), 
                     }])))



# HLT primaries for 1.7e34
HLT_list = [
    # 1-jet
    'HLT_j440',
    
    # large-R topocluster jet
    'HLT_j460_a10_lcw_subjes_L1J100',

    # large-R reclustered jet
    'HLT_j460_a10r_L1J100',

    # large-R trimmed jet
    'HLT_j460_a10t_lcw_jes_L1J100',

    # large-R trimmed+mass-cut single-jet
    'HLT_j420_a10t_lcw_jes_40smcINF_L1J100',

    # 3-jet
    'HLT_3j200',

    # 5-jet
    'HLT_5j85_L14J15',
    'HLT_5j85_L14J150ETA25',
    'HLT_5j85', #L1_4J20

    # 5-jet GSC
    'HLT_5j60_gsc85_boffperf_split_L14J150ETA25',
    'HLT_5j60_gsc85_boffperf_split', #L1_4J20

    # 5-jet central
    'HLT_5j70_0eta240_L14J15',
    'HLT_5j70_0eta240_L14J150ETA25',
    'HLT_5j70_0eta240', #L1_4J20

    # 5-jet central GSC
    'HLT_5j50_gsc70_boffperf_split_0eta240_L14J150ETA25',
    'HLT_5j50_gsc70_boffperf_split_0eta240',
    
    # 6-jet
    'HLT_6j70_L14J15',
    # Other L1 seeds of 6j70 remain active to 2e34

    # 6-jet central
    'HLT_6j55_0eta240_L14J15',
    'HLT_6j55_0eta240_L14J150ETA25',
    'HLT_6j55_0eta240_L14J20',
    'HLT_6j55_0eta240_L15J150ETA25',

    # 6-jet central GSC
    # 6j55 gsc remains unprescaled to 2e34

    # 7-jet
    'HLT_7j45_L14J15',
    # Other L1 seeds of 7j45 remain active to 2e34

    # 10-jet (Razor)
    'HLT_10j40_L14J15',
]
jet_Rules_1p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.7e34'),
                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'), 
                     }])))

HLT_list = [
    # 1-jet
    'HLT_j420',

    # 1-jet GSC
    'HLT_j225_gsc420_boffperf_split',
]
jet_Rules_1p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.7e34, Express ATR-17782', ESRate=0.2),
                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'), 
                     }])))

HLT_list = [

    # 5-jet
    'HLT_5j90_L14J15',
    'HLT_5j100_L14J15',

    # 5-jet GSC
    'HLT_5j60_gsc85_boffperf_split_L14J15',
    'HLT_5j60_gsc90_boffperf_split_L14J15',

    # 5-jet central
    'HLT_5j75_0eta240_L14J15',
    'HLT_5j85_0eta240_L14J15',

    # 5-jet central GSC
    'HLT_5j50_gsc70_boffperf_split_0eta240_L14J15',
    'HLT_5j55_gsc75_boffperf_split_0eta240_L14J15',

    # 6-jet
    'HLT_6j85_L14J15',

    # 6-jet GSC
    'HLT_6j50_gsc70_boffperf_split_L14J15',
    'HLT_6j60_gsc85_boffperf_split_L14J15',

    # 6-jet central
    'HLT_6j60_0eta240_L14J15',

    # 6-jet central GSC
    'HLT_6j45_gsc55_boffperf_split_0eta240_L14J15',
    'HLT_6j45_gsc60_boffperf_split_0eta240_L14J15',

    # 7-jet
    'HLT_7j50_L14J15',

    # 7-jet GSC
    'HLT_7j35_gsc45_boffperf_split_L14J15',
    'HLT_7j35_gsc50_boffperf_split_L14J15',

    # 7-jet central
    'HLT_7j45_0eta240_L14J15',

    # 7-jet central GSC
    'HLT_7j35_gsc45_boffperf_split_0eta240_L14J15',
    'HLT_7j35_gsc50_boffperf_split_0eta240_L14J15',
]
jet_Rules_1p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.7e34, ATR-17450'),
                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'), 
                     }])))

########################################
# 1.5e34


jet_Rules_1p5e34=RulebookDict(tag,'jet_Rules_1p5e34')

# L1 primaries for 1.5e34
# NOTE this is actually only used in b-jet chains
L1_list = ['L1_J75_3J20']
jet_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   15001 : TriggerRule(PS=-1, comment='Disabled, primary J85_3J30'), 
                     }])))


# HLT primaries for 1.5e34
HLT_list = [
    # 1-jet
    'HLT_j400',
    
    # 1-jet GSC
    'HLT_j225_gsc400_boffperf_split',
    
    # large-R topocluster jet
    'HLT_j440_a10_lcw_subjes_L1J100',
    
    # large-R reclustered jet
    'HLT_j440_a10r_L1J100',
    
    # large-R trimmed jet
    # see below for special case, j440 becomes prescaled
    
    # large-R trimmed+mass-cut single-jet
    'HLT_j390_a10t_lcw_jes_40smcINF_L1J100',
    
    # 5-jet
    'HLT_5j70_L14J15',
    'HLT_5j70_L14J150ETA25',
    'HLT_5j70', #L1_4J20
    
    # 5-jet GSC
    'HLT_5j50_gsc70_boffperf_split_L14J150ETA25',
    'HLT_5j50_gsc70_boffperf_split', #L1_4J20
    'HLT_5j55_gsc75_boffperf_split_L14J150ETA25',
    'HLT_5j55_gsc75_boffperf_split',
    
    # 5-jet central
    'HLT_5j65_0eta240_L14J15',
    'HLT_5j65_0eta240_L14J150ETA25',
    'HLT_5j65_0eta240', #L1_4J20
    
    # 5-jet central GSC
    'HLT_5j50_gsc65_boffperf_split_0eta240_L14J150ETA25',
    'HLT_5j50_gsc65_boffperf_split_0eta240', #L1_4J20
    
    # 6-jet
    'HLT_6j60_L14J15',
    'HLT_6j60_L14J150ETA25',
    'HLT_6j60', #L1_4J20

    # 6-jet GSC
    'HLT_6j45_gsc60_boffperf_split_L14J150ETA25',
    'HLT_6j45_gsc60_boffperf_split', # L1_4J20
    'HLT_6j45_gsc60_boffperf_split_L15J150ETA25',
    'HLT_6j50_gsc65_boffperf_split_L14J150ETA25',
    'HLT_6j50_gsc65_boffperf_split', # L1_4J20
    'HLT_6j50_gsc65_boffperf_split_L15J150ETA25',

    # 6-jet central
    'HLT_6j50_0eta240_L14J15',
    'HLT_6j50_0eta240_L14J150ETA25',
    'HLT_6j50_0eta240_L14J20',
    'HLT_6j50_0eta240_L15J150ETA25',

    # 6-jet central GSC
    'HLT_6j35_gsc50_boffperf_split_0eta240_L14J150ETA25',
    'HLT_6j35_gsc50_boffperf_split_0eta240_L14J20',
    'HLT_6j35_gsc50_boffperf_split_0eta240_L15J150ETA25',
]
jet_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.5e34'),
                    15001 : TriggerRule(PS=-1, comment='Disabled'), 
                     }])))

########################################
# 1.2e34

jet_Rules_1p2e34=RulebookDict(tag,'jet_Rules_1p2e34')

# HLT primaries for 1.2e34 (~the 2016 primaries menu)
HLT_list = [
    # 1-jet
    'HLT_j380',

    # 1-jet GSC
    ##'HLT_j225_gsc360_boffperf_split', below with bjet support rule
    'HLT_j225_gsc380_boffperf_split',

    # large-R topocluster jet
    'HLT_j420_a10_lcw_subjes_L1J100',

    # large-R reclustered jet
    'HLT_j420_a10r_L1J100',

    # large-R trimmed jet
    # see below for special case, j420 becomes prescaled

    # 4-jet
    'HLT_4j100',
    'HLT_4j110',

    # 4-jet GSC
    'HLT_4j85_gsc100_boffperf_split', 
    'HLT_4j85_gsc110_boffperf_split',

    # 5-jet GSC
    
    # 6-jet central
    'HLT_6j45_0eta240_L14J150ETA25',
    # See below for HLT_6j45_0eta240_L14J20, which is kept as support past 1.4e34
    'HLT_6j45_0eta240_L15J150ETA25',
    
    # 6-jet central GSC
    'HLT_6j35_gsc45_boffperf_split_0eta240_L14J20',
    'HLT_6j35_gsc45_boffperf_split_0eta240_L15J150ETA25',
]
jet_Rules_1p2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.2e34'),
                   12001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

HLT_list = [
    # 6-jet central
    'HLT_6j45_0eta240', #L1_4J15
]
jet_Rules_1p2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.2e34'),
                   12001 : TriggerRule(rate=0.2, comment='Express ATR-17782', ESRate=0.2), 
                    }])))

HLT_list=['HLT_j225_gsc360_boffperf_split',]
jet_Rules_1p2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.2e34 jet'), 
                    12001 : TriggerRule(rate=0.1, comment='Support for gsc bjet'), 
                    }])))

# HLT primaries for 1.2e34 which become support triggers
HLT_list = ['HLT_j420_a10t_lcw_jes_L1J100',]
jet_Rules_1p2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary 1.2e34'), 
                     12001 : TriggerRule(rate=1.0, comment='Support for j420 SMC'),
                      }])))

# HLT primaries for 1.4e34 which become support triggers (with L1Topo)
# Removed for now (ATR-16021), these are for a Phase-1 study

HLT_list = [
    # 5-jet 
    'HLT_5j70_L1J4-MATCH',
    'HLT_5j85_L1J4-MATCH',
    
    # 5-jet central
    'HLT_5j65_0eta240_L1J4-MATCH',
    
    # 6-jet central
    'HLT_6j45_0eta240_L1J4-MATCH',
    'HLT_6j50_0eta240_L1J4-MATCH',
    
    # 7-jet
    'HLT_7j45_L1J4-MATCH',
    'HLT_7j50_L1J4-MATCH',
    ]
jet_Rules_1p2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled ATR-15236, ATR-14353'),
    }])))
    
########################################
# 1.0e34

jet_Rules_1e34=RulebookDict(tag,'jet_Rules_1e34')


# HLT primaries for 1.0e34
HLT_list = [
    # large-R topocluster jet
    'HLT_j400_a10_lcw_subjes_L1J100',

    # large-R reclustered jet
    'HLT_j400_a10r_L1J100',

    # 3-jet
    'HLT_3j175',
]
jet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
           1000 : TriggerRule(PS=1, comment='Primary'),
           10001 : TriggerRule(PS=-1, comment='Disabled'),
           }])))


HLT_list = [
    'HLT_2j220_j120', 
    'HLT_2j250_j120',
    'HLT_2j275_j140'
    ]
jet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
           1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-16933'),
           10001 : TriggerRule(PS=-1, comment='Disabled'),
           }])))


########################################
# 0p5e34 - 0p75e34

jet_Rules_0p5e34=RulebookDict(tag,'jet_Rules_0p5e34')

# L1 primaries for 0.7e34
L1_list=['L1_J75',]
jet_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   7001 : TriggerRule(rate=100, reoptimize=False, comment='Support'),
                   }])))


# HLT primaries for 0.7e34
HLT_list = [
    # 1-jet
    'HLT_j340',

    # large-R topocluster jet
    'HLT_j380_a10_lcw_subjes_L1J100',

    # large-R reclustered jet
    'HLT_j380_a10r_L1J100',
]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'),
                     7001 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))




# L1 primaries for 0.5e34
# NOTE only used by b-jet triggers
L1_list=['L1_3J40']
jet_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'), 
                    5001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))




########################################
# prescaled 


rate_L1RD0_FILLED=1000
if doLumiTotalRate==True:
    rate_L1RD0_FILLED=rate_HLT_lumipeb_L1RD0_FILLED

jet_Rules_supporting = {
        'L1_RD0_FILLED'  : { 1000 : TriggerRule(rate=rate_L1RD0_FILLED, scaling='bunches', comment='Support L1') },
        }



# ========================================
# prescaled single jets
# Enormous rate due to offline jet performance group needs (used for jet calibration)
# Make sure what can ve set in CPS is done to make this as small as possible

# Standard single-jet support triggers for jet calibration
# NOTE these should have rules that vary during a single fill
HLT_list = [
    # L1_RD0_FILLED
    # See below for HLT_j15 (has pass-through part)
    # See below for HLT_j25 (has express stream part)
    # L1_J20
    # See below for HLT_j60 (hass express stream part)
    'HLT_j85',

    # L1_J30
    'HLT_j110',

    # L1_J50
    'HLT_j175',

    # L1_J75
    'HLT_j260',

    # L1_J100
    'HLT_j360',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=4.0, comment='Support'), # 3Hz in average
                    15001 : TriggerRule(rate=2.0, comment='Support'),
                   }])))


# Support triggers with express stream
# NOTE these should have rules that vary during a single fill
HLT_list = ['HLT_j25']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=4.0, comment='Support'), # 3Hz
                    }])))
HLT_list = [
    'HLT_j35',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=4.0, comment='Support, Express ATR-17782',ESRate=0.5), # 3Hz
                    }])))

HLT_list = [
    # L1_J15
    'HLT_j45',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=4.0, comment='Support'), # 3Hz
                    }])))

HLT_list = [
    'HLT_j60',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=3.0, comment='Support, ATR-17450'),
                    }])))

# Standard forward single-jet support triggers for jet calibration
# NOTE these should have rules that vary during a single fill
HLT_list = [
    # L1_RD0_FILLED
    # See below for HLT_j15_320eta490 (has pass-through)

    # L1_J20.31ETA49
    # See below for HLT_j60_320eta490 (hass express stream part)
    'HLT_j85_320eta490',

    # L1_J30.31ETA49
    'HLT_j110_320eta490',

    # L1_J50.31ETA49
    'HLT_j175_320eta490',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.0, comment='Support')
                   }])))


# Support triggers with express stream
# NOTE these should have rules that vary during a single fill
HLT_list = ['HLT_j60_320eta490']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=1.0, comment='Support'), 
                    }])))


# Standard forward single-jet support triggers for jet calibration, pass-through
# NOTE these should vary during a single fill
HLT_list = [
    'HLT_j15_320eta490',
    'HLT_j25_320eta490',

    # L1_J15.31ETA49
    'HLT_j45_320eta490',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.5, comment='Support', PTRate=0.5)
                   }])))

HLT_list = [
    'HLT_j35_320eta490',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.5, comment='Support, Express ATR-17782', PTRate=0.5, ESRate=0.2)
                   }])))

# Alternative calibration single-jet triggers
# These are in mega-CPS sets (each comment header defines a new CPS set)
HLT_list = [
    'HLT_j35_subjes',
    'HLT_j35_jes',
    'HLT_j35_sub',
    'HLT_j35_nojcalib',
    'HLT_j35_lcw',
    'HLT_j35_320eta490_subjes',
    'HLT_j35_320eta490_jes',
    'HLT_j35_320eta490_sub',
    'HLT_j35_320eta490_nojcalib',
    'HLT_j35_320eta490_lcw',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.1, comment='Support, Express ATR-17782', ESRate=0.1)
                   }])))

HLT_list = [
    # HLT_j35 (L1_RD0_FILLED)
    'HLT_j35_lcw_subjes',
    'HLT_j35_lcw_jes',
    'HLT_j35_lcw_sub',
    'HLT_j35_lcw_nojcalib',
    'HLT_j35_320eta490_lcw_subjes',
    'HLT_j35_320eta490_lcw_jes',
    'HLT_j35_320eta490_lcw_sub',
    'HLT_j35_320eta490_lcw_nojcalib',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.1, comment='Support')
                   }])))

HLT_list = [
    # HLT_j450 (L1_J120)
    'HLT_j450_subjes',
    'HLT_j450_jes',
    'HLT_j450_sub',
    'HLT_j450_nojcalib',
    'HLT_j450_lcw',
    'HLT_j450_lcw_subjes',
    'HLT_j450_lcw_jes',
    'HLT_j450_lcw_sub',
    'HLT_j450_lcw_nojcalib',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.1, comment='Support')
                   }])))

HLT_list = [
# These are in mega-CPS sets (each comment header defines a new CPS set)
    # HLT_j260_320eta490 (L1_J75.31ETA49)
    'HLT_j260_320eta490_subjes',      
    'HLT_j260_320eta490_jes',         
    'HLT_j260_320eta490_sub',         
    'HLT_j260_320eta490_nojcalib',    
    'HLT_j260_320eta490_lcw',         
    'HLT_j260_320eta490_lcw_subjes',  
    'HLT_j260_320eta490_lcw_jes',     
    'HLT_j260_320eta490_lcw_sub',     
    'HLT_j260_320eta490_lcw_nojcalib',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.1, comment='Support')
                   }])))

HLT_list = [
    'HLT_j35_sktc_lcw_nojcalib',
    'HLT_2j330_a10t_lcw_jes_40smcINF_L1SC111', 
    # large-R trimmed+mass-cut di-jet (for mass efficiency bootstrap)
    'HLT_2j330_a10t_lcw_jes_L1J100',
    'HLT_6j35_gsc45_boffperf_split_0eta240_L14J150ETA25',
    'HLT_6j45_0eta240_L14J20',
    'HLT_ht700_L1J100',
    'HLT_j15',
    'HLT_j440_a10t_lcw_jes_40smcINF_L1SC111',
    'HLT_j440_a10t_lcw_jes_L1J100',
    # EMEC IW region
    'HLT_j60_280eta320',
    'HLT_j60_j60_320eta490', 
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=-1, comment='Disabled, ATR-17450')
                   }])))

# Specialty single-jet supporting trigger chains
# These are mostly for studying detector effects (cracks, beam background, etc)
HLT_list = [
    # Jet cleaning (CPS)
    'HLT_j85_cleanL',
    'HLT_j85_cleanT',
    'HLT_j85_cleanLLP',

    # L1 bias
    'HLT_j45_L1RD0_FILLED',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.5, comment='Support')
                   }])))


# Specialty single-jet supporting trigger chains, with express stream part
# These are mostly for studying detector effects (cracks, beam background, etc)
HLT_list = [
    # L1 bias
    'HLT_j60_L1RD0_FILLED',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.5, comment='Support'), 
                   }])))


# Trigger tower based single-jet chains
HLT_list = ['HLT_j60_TT',]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable, Support ATR-14446, ATR-16228')
                    #1000 : TriggerRule(rate=1.0, comment='Disable, Support ATR-14446')
                   }])))


# j0_perf
HLT_list=['HLT_j0_perf_L1RD0_FILLED']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=3.0, comment='Support'),
                   }])))

HLT_list=['HLT_j110_a10_lcw_subjes_L1J30', 'HLT_j175_a10_lcw_subjes_L1J50']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1, comment='Support ATR-17614'),
                   }])))

HLT_list=['HLT_3j160', 'HLT_4j90']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.5, comment='Support ATR-17614'),
                   }])))

# ========================================
# prescaled large-R jets


# Bootstrapping support triggers
HLT_list = [
    # large-R trimmed+mass-cut single-jet (for mass efficiency bootstrap)
    'HLT_j390_a10t_lcw_jes_L1J100',
    ## 'HLT_j400_a10t_lcw_jes_L1J100', # not available
    # j420 and j440 are defined above, as they are unprescaled to 1.2e34 or 1.5e34 respectively, then become support triggers

    #'HLT_2j340_a10t_lcw_jes_L1J100', # Only needed if 2j330 (currently unprescaled at 2e34) is changed
    #'HLT_2j350_a10t_lcw_jes_L1J100', # Only needed if 2j330 (currently unprescaled at 2e34) is changed
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.0, comment='Support'), 
                   }])))


# Bootstrapping support triggers (with L1Topo)

HLT_list = [
        # large-R topocluster jet
        'HLT_j260_a10_lcw_subjes_L1SC85',
        'HLT_j260_a10_lcw_subjes_L1HT150-J20.ETA31',
        'HLT_j260_a10_lcw_subjes_L1HT150-J20s5.ETA31',
    
        # large-R reclustered jet
        'HLT_j260_a10r_L1SC85',
        'HLT_j260_a10r_L1HT150-J20.ETA31',
        'HLT_j260_a10r_L1HT150-J20s5.ETA31',
    
        # large-R trimmed jet
        'HLT_j260_a10t_lcw_jes_L1SC85',
        'HLT_j260_a10t_lcw_jes_L1HT150-J20.ETA31',
        'HLT_j260_a10t_lcw_jes_L1HT150-J20s5.ETA31',
    
        # large-R trimmed+mass-cut single-jet (for mass efficiency bootstrap)
        # NOTE: This needs to be cleaned up, way too much, need to decide on L1Topo usage ASAP
        'HLT_j390_a10t_lcw_jes_L1SC111',
        'HLT_j390_a10t_lcw_jes_L1HT190-J15.ETA21',
        #'HLT_j400_a10t_lcw_jes_L1SC111',
        #'HLT_j400_a10t_lcw_jes_L1HT190-J15.ETA21',
        #'HLT_j420_a10t_lcw_jes_L1SC111',
        #'HLT_j420_a10t_lcw_jes_L1HT190-J15.ETA21',
        'HLT_j440_a10t_lcw_jes_L1SC111',
        'HLT_j440_a10t_lcw_jes_L1HT190-J15.ETA21',
    
        # large-R trimmed+mass-cut di-jet (for mass efficiency bootstrap)
        'HLT_2j330_a10t_lcw_jes_L1HT190-J15.ETA21',
        #'HLT_2j340_a10t_lcw_jes_L1SC111', # Only needed if 2j330 (currently unprescaled at 2e34) is changed
        #'HLT_2j340_a10t_lcw_jes_L1HT190-J15.ETA21', # Only needed if 2j330 (currently unprescaled at 2e34) is changed
        #'HLT_2j350_a10t_lcw_jes_L1SC111', # Only needed if 2j330 (currently unprescaled at 2e34) is changed
        #'HLT_2j350_a10t_lcw_jes_L1HT190-J15.ETA21', # Only needed if 2j330 (currently unprescaled at 2e34) is changed
        ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled L1Topo'), 
    }])))

HLT_list = [
    'HLT_2j330_a10t_lcw_jes_L1SC111',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support L1Topo, ATR-17450'), 
                    }])))

# Large-R jet alternative calibration chains
# These are in CPS sets (each comment header defines a new CPS set)
HLT_list = [
    # HLT_j260_a10* (L1_J75)
    'HLT_j260_a10_sub_L1J75',
    'HLT_j260_a10_nojcalib_L1J75',
    'HLT_j260_a10_lcw_sub_L1J75',
    'HLT_j260_a10_lcw_nojcalib_L1J75',
    'HLT_j260_a10t_lcw_nojcalib_L1J75',
    'HLT_j260_a10_lcw_subjes_L1J75',
    'HLT_j260_a10r_L1J75',
    'HLT_j260_a10t_lcw_jes_L1J75',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.1, comment='Support'), 
                   }])))

HLT_list = [

    # HLT_j480_a10* (L1_J100)
    'HLT_j480_a10_sub_L1J100',
    'HLT_j480_a10_nojcalib_L1J100',
    'HLT_j480_a10_lcw_sub_L1J100',
    'HLT_j480_a10_lcw_nojcalib_L1J100',

    'HLT_j480_a10t_lcw_nojcalib_L1J100',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.1, comment='Support'), 
                   }])))



HLT_list=[
          'HLT_j440_a10t_lcw_jes_30smcINF_L1J100',
          'HLT_2j330_a10t_lcw_jes_30smcINF_L1J100',
          'HLT_j420_a10t_lcw_jes_30smcINF_L1J100',
          'HLT_j390_a10t_lcw_jes_35smcINF_L1J100',
          'HLT_j390_a10t_lcw_jes_30smcINF_L1J100',
          ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, Startup SMC, ATR-16165'),
                    15001 : TriggerRule(PS=-1, comment='Disabled',),
                    }])))


# ========================================
# prescaled multi jets

# HLT multi-jet support triggers
HLT_list = [
    # 4-jet
    'HLT_4j25', # L1_RD0_FILLED

    # 5-jet
    'HLT_5j45', # L1_4J15

    # 6-jet
    'HLT_6j25', # L1_RD0_FILLED

    # 7-jet
    'HLT_7j25', # L1_RD0_FILLED
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.0, comment='Support'), 
                   }])))

HLT_list = [
    # 6-jet
    'HLT_6j45', # L1_4J15
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.0, comment='Support, Express ATR-17782', ESRate=0.2), 
                   }])))

HLT_list = [
    'HLT_5j25', # L1_RD0_FILLED
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.5, comment='Support'), 
                   }])))


# Alternative calibration multi-jet triggers
# These are in mega-CPS sets (each comment header defines a new CPS set)
HLT_list = [
    # HLT_4j45* (L1_3J15)
    'HLT_4j45', # L1_3J15
    'HLT_4j45_subjes',
    'HLT_4j45_jes',
    'HLT_4j45_sub',
    'HLT_4j45_nojcalib',
    'HLT_4j45_lcw',
    'HLT_4j45_lcw_subjes',
    'HLT_4j45_lcw_jes',
    'HLT_4j45_lcw_sub',
    'HLT_4j45_lcw_nojcalib',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disable'), 
    }])))

HLT_list = [
    # HLT_6j45* (L1_4J15)
    'HLT_6j45_subjes',
    'HLT_6j45_jes',
    'HLT_6j45_sub',
    'HLT_6j45_nojcalib',
    'HLT_6j45_lcw',
    'HLT_6j45_lcw_subjes',
    'HLT_6j45_lcw_jes',
    'HLT_6j45_lcw_sub',
    'HLT_6j45_lcw_nojcalib',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.1, comment='Support'), 
                   }])))


# ========================================
# prescaled HT

# Support HT
HLT_list=[
    'HLT_ht700_L1J75',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.0, comment='Support'), 
                   }])))


# Support HT (with L1Topo)
HLT_list=[
    'HLT_ht700_L1HT150-J20.ETA31',
    'HLT_ht700_L1HT150-J20s5.ETA31',
    ]

jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disable'), 
    }])))


########################################

jet_Rules=RulebookDict(tag,'jet_Rules')

RulesList=[jet_Rules_2e34, jet_Rules_1p7e34, jet_Rules_1p5e34, jet_Rules_1p2e34, jet_Rules_1e34, jet_Rules_0p5e34, jet_Rules_supporting] 
 
for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in jet_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    jet_Rules.update(Rules)

    
#################################################################################################
#
#
# b-Jet
#
#
#################################################################################################
# for bjets, the default in 2016 is split and bmv2cxxxx
# several boffperf_split will be needed as well, as support
#
# no idea of L1 topo plans for bjets -> still missing
#
########################################
# 2e34 

bjet_Rules_2e34=RulebookDict(tag,'bjet_Rules_2e34')

#L1_4J20,4J15 is in jet_Rules_2e34

HLT_list=[
    # 1b
    'HLT_j175_gsc225_bmv2c1040_split',
    'HLT_j225_gsc275_bmv2c1060_split',
    'HLT_j225_gsc300_bmv2c1070_split',
    'HLT_j225_gsc360_bmv2c1077_split',
    # 1b+3j
    "HLT_j55_gsc75_bmv2c1040_split_3j55_gsc75_boffperf_split",
    "HLT_j60_gsc85_bmv2c1050_split_3j60_gsc85_boffperf_split",
    # 2b
    "HLT_j150_gsc175_bmv2c1060_split_j45_gsc60_bmv2c1060_split",
    # 1j+2b
    "HLT_j110_gsc150_boffperf_split_2j45_gsc55_bmv2c1070_split_L1J85_3J30",
    # 2b+2j no extra cost on top of non-gsc "HLT_2j35_bmv2c1060_split_2j35_boffperf_split_L14J15.0ETA25"
    "HLT_2j45_gsc55_bmv2c1060_split_2j45_gsc55_boffperf_split_L14J15.0ETA25",
    # 3b
    "HLT_2j55_gsc75_bmv2c1050_split_j55_gsc75_boffperf_split", # disable at L1
    "HLT_2j60_gsc85_bmv2c1070_split_j60_gsc85_boffperf_split", # disable at L1
    # 3b
    "HLT_3j50_gsc65_bmv2c1077_split_L13J35.0ETA23", 
    # 1b+HT
    "HLT_j45_gsc55_bmv2c1050_split_ht700_L1HT190-J15s5.ETA21",
    # 2b+HT
    "HLT_2j45_gsc55_bmv2c1050_split_ht300_L1HT190-J15s5.ETA21",    
    ]
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-16161'), 
                    }])))

HLT_list=[
    'HLT_j225_gsc275_bhmv2c1060_split',
    'HLT_j225_gsc275_bhmv2c1070_split',
    'HLT_j225_gsc275_bhmv2c1077_split',
    'HLT_j225_gsc300_bhmv2c1070_split',
    'HLT_j225_gsc300_bhmv2c1077_split',
    'HLT_j225_gsc300_bhmv2c1085_split',
    'HLT_j225_gsc360_bhmv2c1077_split',
    'HLT_j225_gsc360_bhmv2c1085_split',
    ]
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable, b-jet triggers with hybrid tuning, ATR-17676'), 
                    }])))

HLT_list=[
    # 2b+2j
    "HLT_2j35_gsc45_bmv2c1050_split_2j35_gsc45_boffperf_split_L14J15.0ETA25", #==>BACKUP
    # 2b+3j
    "HLT_2j35_gsc45_bmv2c1060_split_3j35_gsc45_boffperf_split",     #==>BACKUP
    ]
if NoGSCMBjets:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable, ATR-16161 -> To be replaced with non-gsc below in emergency keys'), 
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-16161 -> To be replaced with non-gsc below in emergency keys'), 
                        }])))
    
HLT_list=[
    # 2b+2j
    "HLT_2j35_bmv2c1040_split_2j35_L14J15.0ETA25", #OK
    "HLT_2j35_bmv2c1050_split_2j35_L14J15.0ETA25", #OK
    "HLT_2j35_bmv2c1060_split_2j35_L14J15.0ETA25", #OK
    # 2b+3j
    "HLT_2j35_bmv2c1050_split_3j35", #OK
    "HLT_2j35_bmv2c1060_split_3j35", #OK
    # 4b
    "HLT_2j35_bmv2c1070_split_2j35_bmv2c1085_split_L14J15.0ETA25",  #OK   
    "HLT_4j35_bmv2c1077_split_L14J15.0ETA25", #OK 
    # 3b+1j 
    "HLT_3j35_bmv2c1070_split_j35_L14J15.0ETA25", # OK
]
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-16986 backup'), 
                    }])))

########################################
# 1p5e34 

bjet_Rules_1p5e34=RulebookDict(tag,'bjet_Rules_1p5e34')

HLT_list=[
    "HLT_j225_gsc275_bmv2c1070_split",
    "HLT_j225_gsc300_bmv2c1077_split",
    "HLT_j225_gsc360_bmv2c1085_split",
    "HLT_j50_gsc65_bmv2c1040_split_3j50_gsc65_boffperf_split",
    "HLT_j150_gsc175_bmv2c1070_split_j45_gsc60_bmv2c1070_split",
    "HLT_j110_gsc150_boffperf_split_2j45_gsc55_bmv2c1077_split_L1J85_3J30",
    "HLT_2j55_gsc75_bmv2c1060_split_j55_gsc75_boffperf_split", # disabled at L1
    "HLT_2j60_gsc85_bmv2c1077_split_j60_gsc85_boffperf_split", # disabled at L1
    "HLT_2j35_gsc45_bmv2c1060_split_2j35_gsc45_boffperf_split_L14J15.0ETA25",
    "HLT_2j45_gsc55_bmv2c1070_split_2j45_gsc55_boffperf_split_L14J15.0ETA25",
    "HLT_2j35_gsc45_bmv2c1070_split_3j35_gsc45_boffperf_split",
    "HLT_2j45_gsc55_bmv2c1060_split_ht300_L1HT190-J15s5.ETA21", 
    ]
bjet_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(PS=1, comment='Primary'), 
                           15001 : TriggerRule(PS=-1, comment='Disabled'), 
                           }])))

########################################
# 1e34 

bjet_Rules_1e34=RulebookDict(tag,'bjet_Rules_1e34')

L1_list=['L1_3J25.0ETA23']
bjet_Rules_1e34.update(dict(map(None,L1_list,len(L1_list)*[{
                           1000 : TriggerRule(PS=1, comment='Primary L1'), 
                           14001 : TriggerRule(PS=-1, comment='Disabled'), 
                           }])))

########################################
# supporting

bjet_Rules_supporting=RulebookDict(tag,'bjet_Rules_supporting')

# ========================================
# lepton + boffperf + split,
# purpose is (data) to save sample of ttbar (using the main e/mu trigger, so no additional rate) for calibration, where jets and btagging info are prepared 

HLT_list=[
    'HLT_mu26_ivarmedium_2j35_boffperf_split',
    
    'HLT_e28_lhtight_nod0_ivarloose_L1EM24VHIM_2j35_boffperf_split',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Support'), 
                        }])))

HLT_list=[
    'HLT_2mu14_2j35_boffperf_split',

    'HLT_2e24_lhvloose_nod0_2j35_boffperf_split',

    'HLT_e28_lhtight_nod0_ivarloose_j110_gsc150_boffperf_split_j35_boffperf_split',
    'HLT_e28_lhtight_nod0_ivarloose_j150_boffperf_split_j35_boffperf_split',

    "HLT_mu26_ivarmedium_j110_gsc150_boffperf_split_j35_boffperf_split", 
    'HLT_mu26_ivarmedium_j150_boffperf_split_j35_boffperf_split',
    
    'HLT_e28_lhmedium_nod0_L1EM24VHI_mu8noL1_2j35_boffperf_split',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, new di-lepton bjet calib chans (ATR-15898)'), 
                        }])))

HLT_list=[

        ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, new di-lepton bjet calib chans (ATR-16986)'), 
                        }])))

# ========================================
# bperf
HLT_list=[
    # 1b
    'HLT_j175_gsc225_boffperf_split',
    'HLT_j225_gsc275_boffperf_split',
    'HLT_j225_gsc300_boffperf_split',
    # 1b+3j
    "HLT_j55_gsc75_boffperf_split_3j55_gsc75_boffperf_split",
    "HLT_j60_gsc85_boffperf_split_3j60_gsc85_boffperf_split",
    # 2b
    "HLT_j150_gsc175_boffperf_split_j45_gsc60_boffperf_split",
    # 1j+2b
    "HLT_j110_gsc150_boffperf_split_2j45_gsc55_boffperf_split_L1J85_3J30",
    # 3b
    "HLT_3j50_gsc65_boffperf_split_L13J35.0ETA23",
    # 2b+HT
    "HLT_2j45_gsc55_boffperf_split_ht300_L1HT190-J15s5.ETA21",
    "HLT_j50_gsc65_boffperf_split_3j50_gsc65_boffperf_split",    
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(rate=0.1, comment='Support for gsc'), 
                            }])))

HLT_list=[
    "HLT_2j35_boffperf_split_2j35_L14J15.0ETA25",
    "HLT_3j65_boffperf_split_L13J35.0ETA23",
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(rate=0.1, comment='Support'), 
                            }])))

HLT_list=[
    'HLT_j150_boffperf_split',
    'HLT_j320_boffperf_split',
]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.3, comment='Support'), 
}])))

HLT_list=[
    'HLT_j35_gsc45_boffperf_split_L1J15',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(rate=0.6, comment='Support, Express, ATR-17898', ESValue=0), 
                           }])))

HLT_list=[
    'HLT_j45_gsc55_boffperf_split',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(rate=0.6, comment='Support, Express, ATR-17898', ESRate=0.4), 
                           }])))

HLT_list=[
    'HLT_j85_gsc110_boffperf_split',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(rate=0.6, comment='Support, Express, ATR-17898', ESRate=0.2), 
                           }])))

HLT_list=[
    'HLT_j35_boffperf_split',
    'HLT_j55_boffperf_split',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(rate=0.6, comment='Support'), 
                           }])))

HLT_list=[
    # MU4
    'HLT_mu4_j15_boffperf_split_dr05_dz02',
    'HLT_mu4_j25_boffperf_split_dr05_dz02',
    "HLT_mu4_j35_boffperf_split", 
    "HLT_mu4_j35_boffperf_split_dr05_dz02", 
    "HLT_mu4_j35_boffperf_split_dr05_dz99", 
    # MU6_J20
    'HLT_mu6_j60_gsc85_boffperf_split_dr05_dz02',
    'HLT_mu6_j85_gsc110_boffperf_split_dr05_dz02',
    # MU6_J40
    'HLT_mu6_j110_gsc150_boffperf_split',
    'HLT_mu6_j110_gsc150_boffperf_split_dr05_dz02',
    'HLT_mu6_j150_gsc175_boffperf_split_dr05_dz02',
    'HLT_mu6_j110_gsc150_boffperf_split_dr05_dz99',
    # MU6_J75
    'HLT_mu6_j175_gsc260_boffperf_split_dr05_dz02',
    'HLT_mu6_j225_gsc320_boffperf_split',
    'HLT_mu6_j225_gsc320_boffperf_split_dr05_dz02',
    'HLT_mu6_j225_gsc320_boffperf_split_dr05_dz99',
    'HLT_mu6_j225_gsc400_boffperf_split_dr05_dz02',

    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(rate=1.0, comment='Support'),
                       }])))

# MU4_J12
HLT_list=['HLT_mu4_j45_gsc55_boffperf_split_dr05_dz02',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(rate=1.0, ESRate=0.2, comment='Support Express'),
                       }])))

HLT_list=['HLT_mu4_j55_boffperf_split_dr05_dz02']
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=-1, comment='Disable'),
                       }])))


########################################

bjet_Rules=RulebookDict(tag,'bjet_Rules')

RulesList=[bjet_Rules_2e34, bjet_Rules_1p5e34, bjet_Rules_1e34, bjet_Rules_supporting] 
 
for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in bjet_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule

    bjet_Rules.update(Rules)


#################################################################################################
#
#
# Tau
#
#
#################################################################################################


########################################
# 2e34 

tau_Rules_2e34=RulebookDict(tag,'tau_Rules_2e34')

HLT_list=[
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I-J25',
    "HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I-J25",
    "HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_L1DR-TAU20ITAU12I-J25",
    ]
if do2018:
    tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                        peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:
    tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                        peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                        }])))

HLT_list=[
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',
    "HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25",
    'HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25',
    "HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25",
    'HLT_tau80_medium1_tracktwo_L1TAU60_tau35_medium1_tracktwo_L1TAU12IM_L1TAU60_DR-TAU20ITAU12I', #ATR-15757
    ]
    
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   peakLumi : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))    

HLT_list=[
    "HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I",
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I',
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I', # 27Hz
    ] 
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'), 
    }])))

HLT_list=['HLT_noalg_l1topo_L1LAR-EM','HLT_noalg_l1topo_L1LAR-J',]
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'),
    }])))    

HLT_list=[

    # L1_TAU60 disabled at 1.5e34
    'HLT_tau125_medium1_tracktwo_tau50_medium1_tracktwo_L1TAU12',
    'HLT_tau80_medium1_tracktwo_L1TAU60_tau50_medium1_tracktwo_L1TAU12',

    # single tau
    "HLT_tau160_medium1_tracktwo_L1TAU100",         
    "HLT_tau200_medium1_tracktwo_L1TAU100",

    # di-tau+1J
    "HLT_tau40_medium1_tracktwo_tau35_medium1_tracktwo",

    # di-tau+2J
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM_4J12.0ETA23',

    # di-tau
    'HLT_tau80_medium1_tracktwo_L1TAU60_tau60_medium1_tracktwo_L1TAU40',

    # di-tau+xe
    'HLT_tau60_medium1_tracktwo_tau25_medium1_tracktwo_xe50',

    # tau+e+1J
    "HLT_e24_lhmedium_nod0_ivarloose_tau35_medium1_tracktwo",

    # tau+e+2J
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15VHI_2TAU12IM_4J12',

    # tau+e+xe
    'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50',
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_xe50',

    # tau+mu+1J
    'HLT_mu14_ivarloose_tau35_medium1_tracktwo_L1MU10_TAU20IM_J25_2J20', #backup for SOS

    # tau+mu+2J
    'HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12IM_3J12',

    # tau+mu
    'HLT_mu14_ivarloose_L1MU11_tau35_medium1_tracktwo_L1MU11_TAU20IM',

    # tau+mu+xe
    'HLT_mu14_tau25_medium1_tracktwo_xe50',
    'HLT_mu14_ivarloose_tau25_medium1_tracktwo_xe50',

    # tau+xe+1J
    'HLT_tau35_medium1_tracktwo_xe70_L1XE45',
    ]

tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary'),
    peakLumi : TriggerRule(PS=-1, comment='Disable at 2e34'), 
    }])))

HLT_list=[

    # single tau
    'HLT_tau160_medium1_tracktwoEF_L1TAU100',
    'HLT_tau200_medium1_tracktwoEF_L1TAU100',

    # di-tau+1J
    'HLT_tau35_medium1_tracktwoEF_tau25_medium1_tracktwoEF',
    'HLT_tau40_medium1_tracktwoEF_tau35_medium1_tracktwoEF',

    # di-tau+2J
    'HLT_tau35_medium1_tracktwoEF_tau25_medium1_tracktwoEF_L1TAU20IM_2TAU12IM_4J12.0ETA23',

    # di-tau
    'HLT_tau80_medium1_tracktwoEF_L1TAU60_tau60_medium1_tracktwoEF_L1TAU40',

    # di-tau+xe
    'HLT_tau60_medium1_tracktwoEF_tau25_medium1_tracktwoEF_xe50',

    # tau+e+1J
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoEF',
    'HLT_e24_lhmedium_nod0_ivarloose_tau35_medium1_tracktwoEF',

    # tau+e+2J
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoEF_L1EM15VHI_2TAU12IM_4J12',

    # tau+e+xe
    'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwoEF_xe50',
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoEF_xe50',

    # tau+mu+1J
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoEF',
    'HLT_mu14_ivarloose_tau35_medium1_tracktwoEF_L1MU10_TAU20IM_J25_2J20',

    # tau+mu+2J
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoEF_L1MU10_TAU12IM_3J12',

    # tau+mu
    'HLT_mu14_ivarloose_tau35_medium1_tracktwoEF',
    'HLT_mu14_ivarloose_L1MU11_tau35_medium1_tracktwoEF_L1MU11_TAU20IM',

    # tau+mu+xe
    'HLT_mu14_tau25_medium1_tracktwoEF_xe50',
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoEF_xe50',

    # tau+xe+1J
    'HLT_tau35_medium1_tracktwoEF_xe70_L1XE45',
]
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Non-L1Topo tau medium1_tracktwoEF primary triggers, ATR-17807'),
    peakLumi : TriggerRule(PS=-1, comment='Disable at 2e34'),
    }])))

HLT_list=[

    # di-tau+1J (L1Topo)
    'HLT_tau35_medium1_tracktwoEF_tau25_medium1_tracktwoEF_L1DR-TAU20ITAU12I-J25',
    'HLT_tau35_medium1_tracktwoEF_tau25_medium1_tracktwoEF_03dR30_L1DR-TAU20ITAU12I-J25',

    # di-tau (L1Topo)
    'HLT_tau80_medium1_tracktwoEF_L1TAU60_tau35_medium1_tracktwoEF_L1TAU12IM_L1TAU60_DR-TAU20ITAU12I',

    # tau+e+1J (L1Topo)
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoEF_L1DR-EM15TAU12I-J25',

    # tau+mu+1J (L1Topo)
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoEF_L1DR-MU10TAU12I_TAU12I-J25',
]
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1Topo tau medium1_tracktwoEF triggers, ATR-17807'),
                    peakLumi : TriggerRule(PS=-1, comment='Disable at 2e34'),
                    }])))

HLT_list=[

    # single tau
    'HLT_tau160_medium1_tracktwoMVA_L1TAU100',
    'HLT_tau200_medium1_tracktwoMVA_L1TAU100',

    # di-tau+1J (L1Topo)
    'HLT_tau35_medium1_tracktwoMVA_tau25_medium1_tracktwoMVA_L1DR-TAU20ITAU12I-J25',
    'HLT_tau35_medium1_tracktwoMVA_tau25_medium1_tracktwoMVA_03dR30_L1DR-TAU20ITAU12I-J25',

    # di-tau+1J
    'HLT_tau35_medium1_tracktwoMVA_tau25_medium1_tracktwoMVA',
    'HLT_tau40_medium1_tracktwoMVA_tau35_medium1_tracktwoMVA',

    # di-tau+2J
    'HLT_tau35_medium1_tracktwoMVA_tau25_medium1_tracktwoMVA_L1TAU20IM_2TAU12IM_4J12.0ETA23',

    # di-tau (L1Topo)
    'HLT_tau80_medium1_tracktwoMVA_L1TAU60_tau35_medium1_tracktwoMVA_L1TAU12IM_L1TAU60_DR-TAU20ITAU12I',

    # di-tau
    'HLT_tau80_medium1_tracktwoMVA_L1TAU60_tau60_medium1_tracktwoMVA_L1TAU40',

    # di-tau+xe
    'HLT_tau60_medium1_tracktwoMVA_tau25_medium1_tracktwoMVA_xe50',

    # tau+e+1J (L1Topo)
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoMVA_L1DR-EM15TAU12I-J25',

    # tau+e+1J
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoMVA',
    'HLT_e24_lhmedium_nod0_ivarloose_tau35_medium1_tracktwoMVA',

    # tau+e+2J
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoMVA_L1EM15VHI_2TAU12IM_4J12',

    # tau+e+xe
    'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwoMVA_xe50',
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwoMVA_xe50',

    # tau+mu+1J (L1Topo)
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoMVA_L1DR-MU10TAU12I_TAU12I-J25',

    # tau+mu+1J
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoMVA',
    'HLT_mu14_ivarloose_tau35_medium1_tracktwoMVA_L1MU10_TAU20IM_J25_2J20',

    # tau+mu+2J
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoMVA_L1MU10_TAU12IM_3J12',

    # tau+mu
    'HLT_mu14_ivarloose_tau35_medium1_tracktwoMVA',
    'HLT_mu14_ivarloose_L1MU11_tau35_medium1_tracktwoMVA_L1MU11_TAU20IM',

    # tau+mu+xe
    'HLT_mu14_tau25_medium1_tracktwoMVA_xe50',
    'HLT_mu14_ivarloose_tau25_medium1_tracktwoMVA_xe50',

    # tau+xe+1J
    'HLT_tau35_medium1_tracktwoMVA_xe70_L1XE45',
]
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Primary medium1_tracktwoMVA, ATR-17807'),
    peakLumi : TriggerRule(PS=-1, comment='Disable at 2e34'),
    }])))

HLT_list=[

    # single tau
    'HLT_tau160_mediumRNN_tracktwoMVA_L1TAU100',
    'HLT_tau200_mediumRNN_tracktwoMVA_L1TAU100',

    # di-tau+1J (L1Topo)
    'HLT_tau35_mediumRNN_tracktwoMVA_tau25_mediumRNN_tracktwoMVA_L1DR-TAU20ITAU12I-J25',
    'HLT_tau35_mediumRNN_tracktwoMVA_tau25_mediumRNN_tracktwoMVA_03dR30_L1DR-TAU20ITAU12I-J25',

    # di-tau+1J
    'HLT_tau35_mediumRNN_tracktwoMVA_tau25_mediumRNN_tracktwoMVA',
    'HLT_tau40_mediumRNN_tracktwoMVA_tau35_mediumRNN_tracktwoMVA',

    # di-tau+2J
    'HLT_tau35_mediumRNN_tracktwoMVA_tau25_mediumRNN_tracktwoMVA_L1TAU20IM_2TAU12IM_4J12.0ETA23',

    # di-tau (L1Topo)
    'HLT_tau80_mediumRNN_tracktwoMVA_L1TAU60_tau35_mediumRNN_tracktwoMVA_L1TAU12IM_L1TAU60_DR-TAU20ITAU12I',

    # di-tau
    'HLT_tau80_mediumRNN_tracktwoMVA_L1TAU60_tau60_mediumRNN_tracktwoMVA_L1TAU40',

    # di-tau+xe
    'HLT_tau60_mediumRNN_tracktwoMVA_tau25_mediumRNN_tracktwoMVA_xe50',

    # tau+e+1J (L1Topo)
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_mediumRNN_tracktwoMVA_L1DR-EM15TAU12I-J25',

    # tau+e+1J
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_mediumRNN_tracktwoMVA',
    'HLT_e24_lhmedium_nod0_ivarloose_tau35_mediumRNN_tracktwoMVA',

    # tau+e+2J
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_mediumRNN_tracktwoMVA_L1EM15VHI_2TAU12IM_4J12',

    # tau+e+xe
    'HLT_e17_lhmedium_nod0_tau25_mediumRNN_tracktwoMVA_xe50',
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_mediumRNN_tracktwoMVA_xe50',

    # tau+mu+1J (L1Topo)
    'HLT_mu14_ivarloose_tau25_mediumRNN_tracktwoMVA_L1DR-MU10TAU12I_TAU12I-J25',

    # tau+mu+1J
    'HLT_mu14_ivarloose_tau25_mediumRNN_tracktwoMVA',
    'HLT_mu14_ivarloose_tau35_mediumRNN_tracktwoMVA_L1MU10_TAU20IM_J25_2J20',

    # tau+mu+2J
    'HLT_mu14_ivarloose_tau25_mediumRNN_tracktwoMVA_L1MU10_TAU12IM_3J12',

    # tau+mu
    'HLT_mu14_ivarloose_tau35_mediumRNN_tracktwoMVA',
    'HLT_mu14_ivarloose_L1MU11_tau35_mediumRNN_tracktwoMVA_L1MU11_TAU20IM',

    # tau+mu+xe
    'HLT_mu14_tau25_mediumRNN_tracktwoMVA_xe50',
    'HLT_mu14_ivarloose_tau25_mediumRNN_tracktwoMVA_xe50',

    # tau+xe+1J
    'HLT_tau35_mediumRNN_tracktwoMVA_xe70_L1XE45',
]
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Primary mediumRNN_tracktwoMVA, ATR-17807'),
    peakLumi : TriggerRule(PS=-1, comment='Disable at 2e34'),
    }])))

L1_list=['L1_TAU60']
tau_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   15001 : TriggerRule(PS=-1, comment='Disable L1'), 
    }])))
HLT_list=[
    'HLT_tau160_medium1_tracktwo',
    ]
tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   15001 : TriggerRule(PS=-1, comment='Disable at 1.5e34'), 
    }])))
    
HLT_list=[
    'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_xe50',
    ]

tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled - ATR14490'), 
    }])))


########################################
# 1p5e34 

tau_Rules_1p5e34=RulebookDict(tag,'tau_Rules_1p5e34')

#L1 topo (more efficient than tau+X+jets regular)

L1_list=[# di-tau + xe
    'L1_TAU20IM_2TAU12IM_XE35',
    ]
tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   15001 : TriggerRule(PS=-1, comment='Disabled L1'), 
    }])))

HLT_list=[# di-tau + xe
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_xe50',
    ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   15001 : TriggerRule(PS=-1, comment='Disabled L1'), 
    }])))
    
# --------------------------------------
HLT_list=[
    # tau + mu (w/o jet also at L1)
    'HLT_mu14_ivarloose_tau35_medium1_tracktwo',
    ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable, huge L1 rate, 5kHz at 2e34'),
                            }])))
    
HLT_list=['HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo',
          'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15VHI_2TAU12IM', 
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14490'), 
                            }])))

HLT_list=[
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo',
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            Primary2017Oct1p7E34 : TriggerRule(rate=0.1, comment='Support, ATR-12219', rerun=1),
                            }])))

# due to tau+e+xe, for trigger efficiency measurement (ATR-12219)
HLT_list=[
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15VHI_2TAU12IM', 
    ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            10001 : TriggerRule(rate=0.1, comment='Support, ATR-12219', rerun=1),
                            }])))


HLT_list=['HLT_e17_lhmedium_nod0_tau80_medium1_tracktwo',
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary L1'), 
                            15001 : TriggerRule(PS=-1, comment='Disabled, no users, SOS in 1.5e34'),
                            }])))


# --------------------------------------
# rerun due to tau+mu+xe, for trigger efficiency measurement (ATR-12219) # 15 and 37 Hz
HLT_list=['HLT_mu14_ivarloose_tau25_medium1_tracktwo',
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(PS=1, comment='Primary'), 
                             Primary2017Oct1p7E34 : TriggerRule(rate=0.1, comment='Support, ATR-12219', rerun=1),
                             }])))

# --------------------------------------

L1_list=['L1_TAU20IM_2TAU12IM_J25_2J20_3J12',] # 6200 Hz
tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                              1000 : TriggerRule(PS=1, comment='Primary L1'), 
                              12001 : TriggerRule(rate=100, comment='Support'),
                              }])))

# rerun due to tau+tau+xe, for trigger efficiency measurement(ATR-12219)  20 Hz
HLT_list=['HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo',
          ] 
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(PS=1, comment='Primary', ESRate=0.2), 
                              Primary2017Oct1p7E34 : TriggerRule(rate=0.2, comment='Support, ATR-12219', ESValue=1, rerun=1),
                              }])))

HLT_list=['HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo']
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(PS=1, comment=''), 
                              Primary2017Oct1p7E34 : TriggerRule(rate=0.1, comment='Prescaled'),
                              }])))

HLT_list=["HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30",]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(PS=1, comment='', ESValue=0), 
                              Primary2017Oct1p7E34 : TriggerRule(rate=0.1, comment='Prescaled', ESValue=0),
                              }])))

########################################
# 1e34 

tau_Rules_1e34=RulebookDict(tag,'tau_Rules_1e34')

HLT_list=['HLT_tau125_medium1_tracktwo', 
]
tau_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            10001 : TriggerRule(PS=0, comment='Support',rerun=1),
                            }])))
## removed rerun entirely (ATR-13884)
HLT_list=['HLT_tau60_medium1_tracktwo_tau35_medium1_tracktwo',]
tau_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            10001 : TriggerRule(PS=0, comment='Support',rerun=0),
                            }])))

## removed rerun entirely (ATR-13884)
HLT_list=[
          'HLT_mu14_tau25_medium1_tracktwo',
          'HLT_mu14_tau35_medium1_tracktwo',
          'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo',
          ]
tau_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(PS=-1, comment='Disabled'), 
                             }])))

########################################
# 0.7e34 

tau_Rules_0p7e34=RulebookDict(tag,'tau_Rules_0p7e34')

HLT_list=['HLT_tau35_loose1_tracktwo_tau25_loose1_tracktwo',]
tau_Rules_0p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=1, comment='Primary'), 
                          7001 : TriggerRule(rate=0.1, comment='Support'), # is higher rate needed ? for background determination
                          }])))

# --------------------------------------

HLT_list=['HLT_tau35_medium1_tracktwo_L1TAU20_tau25_medium1_tracktwo_L1TAU12_xe50',]
tau_Rules_0p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=1, comment='Primary'), 
                         7001 : TriggerRule(PS=-1, comment='Disabled'),
                         }])))


########################################

# 0.5e34 

tau_Rules_0p5e34=RulebookDict(tag,'tau_Rules_0p5e34')
HLT_list=['HLT_tau80_medium1_tracktwo_L1TAU60',]
tau_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     5001 : TriggerRule(PS=0, comment='Disabled', rerun=1),
                     }])))


########################################
# 0.3e34 

tau_Rules_0p3e34=RulebookDict(tag,'tau_Rules_0p3e34')

HLT_list=['HLT_tau80_medium1_tracktwo', ]
tau_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     1000 : TriggerRule(PS=0, comment='Support', rerun=1),
                     }])))

HLT_list=[
    'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20ITAU12I-J25',
    ]

tau_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, ATR-13366'),
                    }])))
    

# --------------------------------------

# due to tau+tau+xe, for trigger efficiency measurement  (ATR-12219)
HLT_list=['HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_L1TAU20IM_2TAU12IM',]
tau_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=-1, comment='Disabled'),
                         }])))


########################################
# 0.15e34 

tau_Rules_0p15e34=RulebookDict(tag,'tau_Rules_0p15e34')

HLT_list=['HLT_tau35_loose1_tracktwo_tau25_loose1_tracktwo_L1TAU20IM_2TAU12IM',]
tau_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     1000 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))


# --------------------------------------

HLT_list=[
    # 2tau
    'HLT_tau35_medium1_tracktwo_L1TAU20_tau25_medium1_tracktwo_L1TAU12',
    # tau + mu (high et, w/o jet)
    # w/o L1 tau iso
    'HLT_mu14_tau35_medium1_tracktwo_L1TAU20',
    # tau + xe
    'HLT_tau35_medium1_tracktwo_L1TAU20_xe70_L1XE45',
    ]
tau_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))


########################################
# supporting


tau_Rules_supporting = {
    # --------------------------------------
    # perf
    'HLT_tau0_perf_ptonly_L1TAU12'                : {  1000 : TriggerRule(rate=0.2, comment='Support Express', ESValue=1, rerun=1) }, 
    'HLT_tau0_perf_ptonly_L1TAU60'                : {  1000 : TriggerRule(rate=0.2, comment='Support', ESValue=0) },
    'HLT_tau0_perf_ptonly_L1TAU100'               : {  1000 : TriggerRule(rate=0.2, comment='Support Express', ESValue=1) },   
    # --------------------------------------
    # single & di-tau
    'HLT_tau35_perf_tracktwo_tau25_perf_tracktwo' : {  1000 : TriggerRule(rate=0.2, comment='Support Express', ESValue=1, rerun=1) },
    
    # --------------------------------------
    # Z T&P
    # assign rerun (ATR-12219)
    'HLT_mu14_iloose_tau25_perf_tracktwo'         : {  1000 : TriggerRule(rate=0.4, comment='Support Express', ESValue=1, rerun=1) },
    
    # --------------------------------------
    # idperf
    'HLT_tau25_idperf_track'                      : {  1000 : TriggerRule(rate=0.5, comment='Support', ESValue=0) }, 
    'HLT_tau160_idperf_tracktwo'                  : {  1000 : TriggerRule(rate=0.6, comment='Support Express', ESValue=1, rerun=1) },
    'HLT_tau160_idperf_track'                     : {  1000 : TriggerRule(rate=0.3, comment='Support', ESValue=0) },
    'HLT_tau160_perf_tracktwo'                    : {  1000 : TriggerRule(rate=0.3, comment='Support', ESValue=0, rerun=1) },
    'HLT_tau160_idperf_tracktwo_L1TAU100'         : {  1000 : TriggerRule(rate=0.3, comment='Support Express', ESValue=1, rerun=1) },
    'HLT_tau160_perf_tracktwo_L1TAU100'           : {  1000 : TriggerRule(rate=0.3, comment='Support Express', ESValue=1, rerun=1) },

    # for commissioning

    'HLT_tau25_idperf_tracktwo'                   : {  1000 : TriggerRule(rate=1.0,      comment='Support Express', ESValue=1, rerun=1) },
    'HLT_tau25_perf_tracktwo'                     : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express', ESValue=1, rerun=1) },
    'HLT_tau25_medium1_tracktwo'                  : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express', ESValue=1, rerun=1) },

    'HLT_tau25_idperf_tracktwoEF'                 : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_perf_tracktwoEF'                   : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_medium1_tracktwoEF'                : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },

    'HLT_tau25_idperf_tracktwoEFmvaTES'           : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_perf_tracktwoEFmvaTES'             : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_medium1_tracktwoEFmvaTES'          : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },

    'HLT_tau25_medium1NoPt_tracktwoEFmvaTES'      : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },

    'HLT_tau25_idperf_tracktwoMVA'                : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_perf_tracktwoMVA'                  : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_medium1_tracktwoMVA'               : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },

    'HLT_tau25_verylooseRNN_tracktwo'             : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_looseRNN_tracktwo'                 : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_mediumRNN_tracktwo'                : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_tightRNN_tracktwo'                 : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },

    'HLT_tau25_verylooseRNN_tracktwoMVA'          : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_looseRNN_tracktwoMVA'              : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_mediumRNN_tracktwoMVA'             : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    'HLT_tau25_tightRNN_tracktwoMVA'              : {  1000 : TriggerRule(inputRate=1.5, comment='Support Express, ATR-17782', ESValue=1, rerun=1) },
    }  

# ========================================


HLT_list=[
    'HLT_tau25_loose1_tracktwo',
    'HLT_tau25_medium1_tracktwo_L1TAU12',    
    'HLT_tau25_perf_tracktwo_L1TAU12',   
    'HLT_tau25_tight1_tracktwo', 
    'HLT_tau35_loose1_tracktwo',        
    'HLT_tau35_medium1_tracktwo',
    'HLT_tau50_medium1_tracktwo_L1TAU12',
    'HLT_tau60_medium1_tracktwo',
    ]
tau_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1), 
}])))

HLT_list=['HLT_tau35_perf_tracktwo_tau25_perf_tracktwo_ditauM'] #ATR-13494
tau_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1,  ESValue=0), 
}])))



HLT_list = ['HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12IM',  #0.2Hz
#            'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15HI_2TAU12IM', #0.2Hz
          ]
tau_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, rerun=1, comment='Support for L1Topo (ATR-14349)'), 
    }])))

HLT_list = ['HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM'] #HLT 77Hz #L1 15kHz
tau_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1, rerun=1, comment='Support for L1Topo (ATR-14349)'), 
    }])))


########################################

tau_Rules=RulebookDict(tag,'tau_Rules')

RulesList=[tau_Rules_2e34, tau_Rules_1p5e34, tau_Rules_1e34, tau_Rules_0p7e34, tau_Rules_0p5e34, tau_Rules_0p3e34, tau_Rules_0p15e34, tau_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in tau_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    tau_Rules.update(Rules)


#################################################################################################
#
#
# e/gamma
#
#
#################################################################################################


########################################
# runs up to 2e34 :

egamma_Rules_2e34=RulebookDict(tag,'egamma_Rules_2e34')

L1_list=['L1_EM24VHIM',]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   }])))

HLT_list=[
    'HLT_e28_lhtight_nod0_ivarloose_L1EM24VHIM',
    'HLT_e80_lhmedium_nod0_L1EM24VHIM', 
    'HLT_e140_lhloose_nod0_L1EM24VHIM',
    'HLT_e300_etcut_L1EM24VHIM',
    'HLT_g300_etcut_L1EM24VHIM',
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

HLT_list=[
    'HLT_g200_loose_L1EM24VHIM','HLT_g140_tight_L1EM24VHIM', 
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary '),
                    }])))

L1_list=['L1_EM24VHI', ]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   }])))

HLT_list=[
    'HLT_e28_lhtight_nod0_ivarloose',
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary express', ESRate=0.4), 
                    }])))

HLT_list=[
    'HLT_e28_lhtight_nod0_noringer_ivarloose',
    "HLT_e28_lhtight_nod0_noringer_ivarloose_L1EM24VHIM",
    ]
if do2018:
    egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable'),
                        }])))
else:
    egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Primary, Commission'),
                        }])))

HLT_list=['HLT_e32_lhtight_nod0_ivarloose', ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Backup ATR-15201, ATR-15236'),
                    }])))

HLT_list=[
    'HLT_e60_lhmedium_nod0_L1EM24VHI',
    'HLT_e60_lhmedium_nod0_L1EM24VHIM',
    ]
if do2018:
    egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable')
                        }])))
else:
    egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary')
                        }])))

HLT_list=[    
    'HLT_e80_lhmedium_nod0_L1EM24VHI',
    'HLT_e140_lhloose_nod0_L1EM24VHI',
    'HLT_e300_etcut',
    'HLT_g300_etcut_L1EM24VHI',
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary')
    }])))

HLT_list=['HLT_g200_loose',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, express', ESRate=0.2),
                    }])))

HLT_list=['HLT_g140_tight',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    }])))

## 2EM
L1_list=['L1_2EM20VH']
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   }])))

HLT_list=[
    # 2g
    "HLT_2g50_loose_L12EM20VH", "HLT_2g60_loose_L12EM20VH",
    "HLT_2g25_tight_L12EM20VH",
    # e+g
    "HLT_e24_lhmedium_nod0_L1EM20VH_g25_medium",
    # 3g
    "HLT_2g25_loose_g15_loose",
    "HLT_2g25_loose_g20_loose",
    "HLT_3g25_loose",
    # e+g (ATR-15259)
    "HLT_e30_mergedtight_g35_medium_Heg",
    "HLT_e25_mergedtight_g35_medium_Heg",
    "HLT_e25_mergedtight_ivarloose_g35_medium_icalovloose_Heg",
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34, 2EM20VH seed'), 
                    }])))

HLT_list=["HLT_g35_medium_g25_medium_L12EM20VH",]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34, 2EM20VH seed, express', ESRate=0.4), 
                    }])))

HLT_list=["HLT_2e24_lhvloose_nod0",]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34, 2EM20VH seed, express', ESRate=0.2), 
                    }])))

L1_list=['L1_2EM15VHI']
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                    peakLumi : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

HLT_list=['HLT_2e19_lhvloose_nod0', 'HLT_2e17_lhvloose_nod0_L12EM15VHI',
          ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34, 2EM15VHI seed' ),
                   peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_2g22_tight_L12EM15VHI',
    'HLT_2g20_tight_icalotight_L12EM15VHI',
    'HLT_2g20_tight_icalovloose_L12EM15VHI',
    'HLT_2g22_tight_icalotight_L12EM15VHI',
    'HLT_2g22_tight_icalovloose_L12EM15VHI',
    'HLT_2g22_tight_icalotight',
    'HLT_2g22_tight_icalovloose',
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   peakLumi : TriggerRule(PS=-1, comment='Disabled'),
}])))


L1_list=['L1_2EM15VH']
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                    7001 : TriggerRule(rate=1000, comment='Support'),
                   12001 : TriggerRule(PS=-1, comment='Disabled'), 
                   }])))

HLT_list=[ 'HLT_2e17_lhvloose_nod0',
           ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    12001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_g35_medium_g25_medium',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   12001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[    
    # 2g
    'HLT_2g60_loose_L12EM15VH',
    'HLT_2g50_loose', # ATR-13866    
    'HLT_2g22_tight',
    # 3g
    'HLT_3g20_loose',
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
}])))


# 3EM
L1_list=["L1_EM20VH_3EM10VH"]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                    }])))

HLT_list=['HLT_e24_lhvloose_nod0_2e12_lhvloose_nod0_L1EM20VH_3EM10VH',
          'HLT_e24_lhloose_nod0_2e12_lhloose_nod0_L1EM20VH_3EM10VH',
          'HLT_e24_lhmedium_nod0_2g12_loose',
          'HLT_e24_lhmedium_nod0_2g12_medium',
          ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34'), 
                    }])))

# 3EM
L1_list=["L1_EM20VH_2EM10VH_3EM8VH"]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                    7001 : TriggerRule(PS=-1, comment='Disable L1'),
                    }])))

HLT_list=["HLT_e22_lhloose_nod0_e12_lhloose_nod0_e10_lhloose_nod0_L1EM20VH_2EM10VH_3EM8VH",
          "HLT_e22_lhvloose_nod0_e12_lhvloose_nod0_e10_lhvloose_nod0_L1EM20VH_2EM10VH_3EM8VH",
          ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34'),
                    15001 : TriggerRule(PS=-1, comment='Disable'),
                    }])))

# 3EM
L1_list=["L1_EM15VH_3EM10VH"]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.0e34, EndOfFill'),
                   10001 : TriggerRule(PS=-1, comment='Disable'),
                         }])))

# Needs to add e+2g version
HLT_list=[    
    "HLT_e17_lhloose_nod0_2e12_lhloose_nod0_L1EM15VH_3EM10VH",     
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 1.0e34, EndOfFill'),
                   10001 : TriggerRule(PS=-1, comment='Disable'),
                    }])))

# --------------------------------------
# 1.5e34 menu
L1_list=['L1_EM22VHI']
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                  MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable'), 
                   }])))



HLT_list=[
    'HLT_e28_lhtight_nod0_ivarloose_L1EM22VHI',    
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary'), 
                  MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled')}])))

HLT_list=['HLT_e26_lhtight_nod0_ivarloose',
          ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary, EM22VHI(M) seed'), 
                  MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Support')}])))

HLT_list=[
    'HLT_e60_lhmedium_nod0',    
    'HLT_e80_lhmedium_nod0',    
    'HLT_e140_lhloose_nod0',
    'HLT_g300_etcut',
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EM22VHI seed'),
                   MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
}])))


HLT_list=['HLT_g140_loose',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EM24VHI seed but huge rate 50Hz at 2e34'),
                    MainPrimary2017Oct1p7E34 : TriggerRule(rate=0.5, comment='Support'),
                    }])))


# --------------------------------------
HLT_list=['HLT_e26_lhtight_idperf', ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support Tracking Validation'),                    
                    }])))

HLT_list=['HLT_e28_lhtight_idperf',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support Tracking Validation, Express', ESRate=0.25),
                    }])))

HLT_list=['HLT_e28_lhtight_idperf_L1EM24VHIM',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support Tracking Validation', ESValue=0),
                    }])))


########################################
# runs up to 1e34 :

egamma_Rules_1e34=RulebookDict(tag,'egamma_Rules_1e34')

HLT_list=['HLT_e24_lhtight_nod0_ivarloose']
egamma_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, ATR-17820'),
                    }])))

HLT_list=[
    # 2g
    'HLT_2g20_tight',
    'HLT_2g20_tight_L12EM15VHI',
    # 3g
    'HLT_2g20_loose_g15_loose',
    ]
egamma_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
    }])))

# --------------------------------------

HLT_list=['HLT_2e15_lhvloose_nod0_L12EM13VH',]
egamma_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary'), 
                       10001 : TriggerRule(PS=-1, comment='Disabled'),
                       }])))

########################################
# 0.5e34 

egamma_Rules_0p5e34=RulebookDict(tag,'egamma_Rules_0p5e34')
           
# --------------------------------------
L1_list=['L1_EM20VH']
egamma_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable'),
                    }])))

L1_list=['L1_EM22VH']
egamma_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(rate=2000, comment='Primary Commission depending on L1 IBL limit'),
                    3001 : TriggerRule(rate=500, reoptimize=False, comment='Support'),
                    }])))

# --------------------------------------
# keep some rate as support,0.5 Hz, when prescaled. Ask if ok.

HLT_list=['HLT_g35_loose_g25_loose',]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=1, comment='Primary 2016'), 
                         14001 : TriggerRule(rate=0.6, comment='Support'),
                         }])))

HLT_list=["HLT_g35_loose_g25_loose_L12EM20VH",]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=1, comment='Primary 2016'), 
                         14001 : TriggerRule(rate=1, comment='Support'),
                         }])))


# --------------------------------------
# keep some rate as support,0.5 Hz, when prescaled. Ask if ok.

HLT_list=['HLT_g120_loose',]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      5001 : TriggerRule(rate=0.5, comment='Support',rerun=1),
                      }])))

########################################
# 0.3e34 

egamma_Rules_0p3e34=RulebookDict(tag,'egamma_Rules_0p3e34')

# --------------------------------------

# keep some L1 rate as support,500 Hz, when prescaled. Ask why
L1_list=['L1_2EM15']
egamma_Rules_0p3e34.update(dict(map(None,L1_list,len(L1_list)*[{
                     1000 : TriggerRule(rate=500, comment='Prescaled'),
                     }])))

HLT_list=['HLT_g35_loose_L1EM15_g25_loose_L1EM15',]
egamma_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     3001 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))

HLT_list=['HLT_2e17_lhloose_nod0_L12EM15',]
egamma_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.2, comment='Support'),
                    }])))

########################################
# supporting

egamma_Rules_supporting=RulebookDict(tag,'egamma_Rules_supporting')

egamma_Rules_supporting.update({
    'HLT_e26_lhvloose_nod0_L1EM22VH'   : {  1000 : TriggerRule(PS=9,  rerun=1, comment='Support, target 1Hz') ,
                            MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e28_lhvloose_nod0_L1EM22VH'   : {  1000 : TriggerRule(PS=7, comment='Support, target 1Hz') ,
                            MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e26_lhmedium_nod0_L1EM22VH'   : {  1000 : TriggerRule(PS=1,  rerun=1, comment='Support, target 4Hz') ,
                            MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e28_lhmedium_nod0_L1EM22VH'   : {  1000 : TriggerRule(PS=1,  rerun=1, comment='Support, target 4Hz') ,
                            MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled') },
})


# ----------------------------------------
# di-electron


# ========================================
# non isolated single electron for background

# --------------------------------------
HLT_list=['HLT_e24_lhtight_nod0_L1EM20VH',]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'),
                      3001 : TriggerRule(rate=0.2, comment='Support'),
                      }])))

# ========================================
# other support for electron
# ATR-11040

# --------------------------------------
# Data-driven LH pdf's

# up to 10 Hz, is this really needed?
# CPS?
HLT_list=[
    'HLT_e5_etcut',
    'HLT_e10_etcut_L1EM7',
    'HLT_e15_etcut_L1EM7',
    'HLT_e20_etcut_L1EM12',
    'HLT_e25_etcut_L1EM15',
    'HLT_e30_etcut_L1EM15',
    'HLT_e40_etcut_L1EM15',
    'HLT_e50_etcut_L1EM15',

    'HLT_e60_etcut',
    'HLT_e70_etcut',
    'HLT_e80_etcut',
    'HLT_e100_etcut',
    'HLT_e120_etcut',
    'HLT_e140_etcut',
    'HLT_e160_etcut',
    'HLT_e180_etcut',
    'HLT_e200_etcut',
    'HLT_e250_etcut',

    'HLT_e60_etcut_L1EM24VHIM',
    'HLT_e70_etcut_L1EM24VHIM',
    'HLT_e80_etcut_L1EM24VHIM',
    'HLT_e100_etcut_L1EM24VHIM',
    'HLT_e120_etcut_L1EM24VHIM',
    'HLT_e140_etcut_L1EM24VHIM',
    'HLT_e160_etcut_L1EM24VHIM',
    'HLT_e180_etcut_L1EM24VHIM',
    'HLT_e200_etcut_L1EM24VHIM',
    'HLT_e250_etcut_L1EM24VHIM',

    'HLT_e60_lhvloose_nod0',

    'HLT_e140_lhvloose_nod0',
    'HLT_e160_lhvloose_nod0',

    'HLT_e140_lhvloose_nod0_L1EM24VHIM',
    'HLT_e160_lhvloose_nod0_L1EM24VHIM',

    'HLT_g200_etcut',
    'HLT_g250_etcut',
    
    "HLT_e80_lhloose_nod0_L1EM24VHIM", 
    ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=1.0, comment='Support'), 
                    }])))

HLT_list=[
    'HLT_e5_lhvloose_nod0',
    'HLT_e10_lhvloose_nod0_L1EM7', 
    'HLT_e15_lhvloose_nod0_L1EM7', 
    'HLT_e20_lhvloose_nod0_L1EM12', 
    'HLT_e25_lhvloose_nod0_L1EM15', 
    'HLT_e30_lhvloose_nod0_L1EM15', 
    'HLT_e40_lhvloose_nod0_L1EM15', 
    'HLT_e50_lhvloose_nod0_L1EM15', 

    'HLT_e70_lhvloose_nod0', 
    'HLT_e80_lhvloose_nod0', 
    'HLT_e100_lhvloose_nod0',
    'HLT_e120_lhvloose_nod0',

    'HLT_e70_lhvloose_nod0_L1EM24VHIM',
    'HLT_e80_lhvloose_nod0_L1EM24VHIM',
    'HLT_e100_lhvloose_nod0_L1EM24VHIM',
    'HLT_e120_lhvloose_nod0_L1EM24VHIM',
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.4, comment='Support, ATR-17721', rerun=1),
}])))

# --------------------------------------
# physics - background estimation
# how much rate does this sum to? is this needed? and why not use the rule with rate=1 Hz instead of these ad-hoc prescales?
# all this rerun potentially very CPU expensive. is this needed?

egamma_Rules_supporting.update({
    'HLT_e12_lhvloose_nod0_L1EM10VH'  : {  1000 : TriggerRule(PS=8800, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e17_lhvloose_nod0'           : {  1000 : TriggerRule(PS=1920, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    ## ringer 'HLT_e17_lhvloose_nod0_ringer'    : {  1000 : TriggerRule(PS=1920, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e20_lhvloose_nod0'           : {  1000 : TriggerRule(PS=1600, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e40_lhvloose_nod0'           : {  1000 : TriggerRule(PS=45, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    
})
    
# ========================================
# rerun
# this is potentially very expensive !! is this really needed ?

HLT_list=[
    'HLT_e7_lhmedium_nod0',    
    'HLT_e9_lhloose_nod0',
    'HLT_e9_lhvloose_nod0',
    'HLT_e9_lhvloose_nod0_L1EM7',
    'HLT_e9_lhvloose_nod0_L1EM8VH',
    'HLT_e9_lhmedium_nod0',
    'HLT_e12_lhmedium_nod0',
    'HLT_e17_lhmedium_nod0',
    "HLT_e17_lhmedium_nod0_L1EM15", 
    'HLT_e17_lhmedium_nod0_ivarloose_L1EM15VHI',

    'HLT_e20_lhmedium_nod0',
    'HLT_e20_lhmedium_nod0_L1EM18VHI',
    'HLT_e24_lhmedium_nod0_L1EM15VH',
    'HLT_e24_lhmedium_L1EM18VHI',
    'HLT_e24_lhmedium_nod0_L1EM18VHI',

    'HLT_e26_lhmedium_nod0',
    
    'HLT_e60_lhloose_nod0',
    'HLT_e60_lhloose_nod0_L1EM24VHIM',
    'HLT_e70_lhloose_nod0',
    'HLT_e70_lhloose_nod0_L1EM24VHIM',
    'HLT_e5_lhtight_nod0',
    'HLT_e9_lhtight_nod0',
    'HLT_e14_lhtight_nod0',

    'HLT_e5_lhloose_nod0',
    'HLT_e5_lhmedium_nod0',

    'HLT_e12_lhloose_nod0',
    'HLT_e12_lhloose_nod0_L1EM10VH',    
    'HLT_e12_lhloose_cutd0dphideta_L1EM10VH','HLT_e12_lhloose_nodeta_L1EM10VH','HLT_e12_lhloose_nodphires_L1EM10VH', 
    'HLT_e17_lhloose_nod0_L1EM15',
    'HLT_e17_lhloose_cutd0dphideta_L1EM15',  'HLT_e17_lhloose_nodeta_L1EM15',  'HLT_e17_lhloose_nodphires_L1EM15',
    'HLT_e17_lhloose_nod0',
    'HLT_e17_lhloose_cutd0dphideta',         

    'HLT_e26_lhvloose_nod0',
    
    # for 2e
    'HLT_e17_lhvloose_nod0_L1EM15VHI',
    ## ringer 'HLT_e17_lhvloose_nod0_ringer_L1EM15VHI',
    'HLT_e19_lhvloose_nod0',
    'HLT_e24_lhvloose_nod0_L1EM20VH',
    
    # for 3e
    'HLT_e24_lhloose_nod0_L1EM20VH',

    # for e+3j
    'HLT_e20_lhtight_nod0_ivarloose_L1EM18VHI',

    # for Heg 
    'HLT_e25_mergedtight_ivarloose',
    
    # for e+xe
    "HLT_e80_lhloose_nod0",  
    "HLT_e80_lhloose_nod0_L1EM24VHI",  

    # for e+mu
    "HLT_e28_lhmedium_nod0",
    "HLT_e28_lhmedium_nod0_L1EM24VHIM",
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1),
}])))

# ========================================
# photon prescaled
# 12 Hz just for this. Is this needed?

HLT_list=[
    'HLT_g40_loose_L1EM15',
    'HLT_g45_loose_L1EM15',
    'HLT_g50_loose_L1EM15',
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, comment='Support ATR-13381',rerun=1), 
    }])))

HLT_list=[
    'HLT_g15_loose_L1EM7',  
    'HLT_g25_loose_L1EM15',
    'HLT_g35_loose_L1EM15',
    ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, ESValue=0, comment='Support ATR-13381',rerun=1), 
    }])))

HLT_list=[
    'HLT_g60_loose',
    'HLT_g70_loose',
    'HLT_g80_loose',
    'HLT_g100_loose',    
    'HLT_g160_loose',
    'HLT_g180_loose',
    "HLT_g60_loose_L1EM24VHIM",
    "HLT_g70_loose_L1EM24VHIM",
    "HLT_g80_loose_L1EM24VHIM",
    "HLT_g100_loose_L1EM24VHIM",
    "HLT_g120_loose_L1EM24VHIM",
    "HLT_g140_loose_L1EM24VHIM",
    "HLT_g160_loose_L1EM24VHIM",
    "HLT_g180_loose_L1EM24VHIM", 
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, comment='Support ATR-13381',rerun=1),
}])))

#another 4 Hz. is this neeeded?
HLT_list=[
    'HLT_g10_loose',    
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, ESValue=0, comment='Support ATR-13381',rerun=1, PTRate=1.0),
    }])))
HLT_list=[
    'HLT_g20_loose_L1EM12',
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, comment='Support ATR-13381',rerun=1, PTRate=1.0, ESValue=0),
    }])))

# total of 24 Hz for prescaled support after HLT for egamma.

# ========================================
# photon rerun
# all this can be very CPU expensive . Is this really needed?

HLT_list=[
    'HLT_g10_etcut',
    'HLT_g20_etcut_L1EM12',
    'HLT_g10_medium',
    'HLT_g12_medium',
    'HLT_g15_loose_L1EM3',
    'HLT_g20_loose',
    'HLT_g24_loose',
    'HLT_g25_loose',
    'HLT_g35_loose',  
    'HLT_g50_loose',
    'HLT_g25_medium',
    'HLT_g35_medium',
    'HLT_g22_tight',
    'HLT_g40_tight',
    'HLT_g45_tight',

    'HLT_g20_loose_L1EM15',
    
    'HLT_g25_medium_L1EM22VHI',
    'HLT_g35_loose_L1EM22VHI',
    'HLT_g45_tight_L1EM22VHI',
    "HLT_g45_tight_L1EM24VHI",

    # for 2g
    'HLT_g20_tight_L1EM15VHI',
    'HLT_g22_tight_L1EM15VHI',
    'HLT_g20_tight_icalovloose_L1EM15VHI',
    'HLT_g22_tight_icalovloose_L1EM15VHI',
    'HLT_g20_tight_icalotight_L1EM15VHI',
    'HLT_g22_tight_icalotight_L1EM15VHI',
    "HLT_g22_tight_icalotight",
    "HLT_g22_tight_icalovloose", 
    'HLT_g35_medium_icalotight',
    'HLT_g35_medium_icalovloose',
    
    # for g+xe or 3j
    'HLT_g80_loose_icalovloose_L1EM24VHIM',
    'HLT_g85_tight_icalovloose_L1EM24VHIM',
    'HLT_g100_tight_icalovloose_L1EM24VHIM',
    'HLT_g85_tight_icalovloose_L1EM24VHI',
    'HLT_g100_tight_icalovloose_L1EM24VHI',
    
    # for 2g
    "HLT_g50_loose_L1EM20VH",
    "HLT_g60_loose_L1EM20VH",
    
    # for g+mu
    "HLT_g35_loose_L1EM24VHI",
    "HLT_g35_loose_L1EM24VHIM",
    'HLT_g35_tight_icalotight_L1EM24VHI',
    'HLT_g35_tight_icalotight_L1EM24VHIM',
    'HLT_g40_tight_icalotight_L1EM24VHI',
    'HLT_g40_tight_icalotight_L1EM24VHIM',
    
    # new for 2017
    "HLT_g85_tight",
    "HLT_g100_tight",
    "HLT_g85_tight_L1EM24VHI",
    "HLT_g100_tight_L1EM24VHI",
    "HLT_g85_tight_L1EM24VHIM",
    "HLT_g100_tight_L1EM24VHIM",

    # for 3g
    'HLT_g25_loose_L1EM20VH',
    # for e+2g
    'HLT_g12_loose',
    # for g+2mu
    'HLT_g15_loose_L1EM8VH',
    'HLT_g35_loose_L1EM20VH',
    'HLT_g25_medium_L1EM20VH',
    'HLT_g35_medium_L1EM20VH',
    'HLT_g25_tight_L1EM20VH',
    # for Heg
    'HLT_e25_mergedtight',
    'HLT_e30_mergedtight',
    # for low-mass diphoton searches (ATR-17462)
    'HLT_g6_tight_icalotight',
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1), 
}])))

HLT_list=[
    'HLT_e24_lhmedium_nod0_L1EM20VH', 
 ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, comment='Support ATR-16981',rerun=1),
}])))



HLT_list=['HLT_g60_loose_L1EM15VH']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1), 
}])))


########################################
# T&P for electron

# --------------------------------------
# Z

#v6
egamma_Rules_supporting.update({
     'HLT_e26_lhtight_nod0_e15_etcut_L1EM7_Zee'       : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-13378') },
     'HLT_e28_lhtight_nod0_e15_etcut_L1EM7_Zee'       : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-13378, Express', ESRate=0.8) },

     "HLT_e28_lhtight_nod0_L1EM24VHIM_e15_etcut_L1EM7_Zee"       : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-16089') },     
    })


# --------------------------------------
# W

L1_list=['L1_EM12_XS20', 'L1_EM15_XS30']
egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(rate=500, comment='L1'),
                    }])))


L1_list=['L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', ]
egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'), 
    }])))

L1_list=['L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',]
egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'), 
    }])))

egamma_Rules_supporting.update({
    # 8 Hz for W tag&probe support, all with rerun, is this not CPU expensive?
    #
    # ATR-11746: needs rerun also to W T&P triggers
    #
    # EM10_XS20
    # eff 0.16
    'HLT_e13_etcut_trkcut_j20_perf_xe15_2dphi05_mt25'       : {  1000 : TriggerRule(PS=-1, comment='Disabled, W tag probe') },
    'HLT_e13_etcut_trkcut_j20_perf_xe15_6dphi05_mt25'       : {  1000 : TriggerRule(PS=-1, comment='Disabled, W tag probe') },
    # eff 0.11
    'HLT_e13_etcut_trkcut_xs15_j20_perf_xe15_2dphi05_mt25'  : {  1000 : TriggerRule(PS=-1, comment='Disabled, W tag probe') },
    'HLT_e13_etcut_trkcut_xs15_j20_perf_xe15_6dphi05_mt25'  : {  1000 : TriggerRule(PS=-1, comment='Disabled, W tag probe') },
    #
    'HLT_e13_etcut_trkcut_xs15_mt25'                        : {  1000 : TriggerRule(PS=-1, comment='Disabled, W tag probe') },
    #
    # EM15_XS30    
    # eff 0.05
    'HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35'       : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    #
    'HLT_e18_etcut_trkcut_xs20_j20_perf_xe20_6dphi15_mt35'  : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    #
    'HLT_e18_etcut_trkcut_xs20_mt35'                        : {  1000 : TriggerRule(PS=-1, comment='Disable, W tag probe') },
    

})

HLT_list=[
    "HLT_e60_etcut_trkcut_L1EM24VHIM_xs30_j15_perf_xe30_6dphi15_mt35", 
    ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary W TP, ATR-16680', rerun=1),
    }])))

HLT_list=[
    "HLT_e60_etcut_trkcut_j15_perf_xe60_6dphi05_mt35", 
]        
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, comment='Support W TP, ATR-16680', rerun=1),
}])))

HLT_list=[
    "HLT_e60_etcut_trkcut_L1EM24VHIM_j15_perf_xe60_6dphi15_mt35", 
    "HLT_e60_etcut_trkcut_L1EM24VHIM_xe60_mt35", 
    "HLT_e60_etcut_trkcut_L1EM24VHIM_xs30_xe30_mt35", 
    "HLT_e60_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35",
]        
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disable'),
}])))

HLT_list=['HLT_e13_etcut_trkcut_xs30_xe30_mt35',
          'HLT_e13_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35',
          'HLT_e13_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35',
          'HLT_e13_etcut_trkcut_xs30_j15_perf_xe30_2dphi05_mt35',
          'HLT_e13_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35',
          'HLT_e18_etcut_trkcut_xs30_xe30_mt35' ,
          'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35',
          'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi05_mt35',
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, comment='Support W TP', rerun=1),
}])))
HLT_list=['HLT_e18_etcut_trkcut_L1EM15']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disable'),
}])))

HLT_list=['HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35',]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary W TP'),
}])))

HLT_list=['HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35',]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=5, comment='Support W TP'),
}])))

# --------------------------------------
# J/psi 

# PS adjustment by ATR-11886
# rate L1 3000 Hz for Jpsi support
egamma_Rules_supporting.update({
    'L1_2EM3'        : {  1000 : TriggerRule(rate=200, comment='Support L1') },
    'L1_EM7_2EM3'    : {  1000 : TriggerRule(PS=-1, comment='Disabled'),},
    'L1_EM12_2EM3'   : {  1000 : TriggerRule(PS=-1, comment='Disabled'),},
    })


HLT_list=[
    'HLT_e14_lhtight_nod0_e4_etcut_Jpsiee',
    'HLT_e9_lhtight_nod0_e4_etcut_Jpsiee',

    # new for 2017
    "HLT_e5_lhtight_nod0_e14_etcut_Jpsiee",
    "HLT_e5_lhtight_nod0_e9_etcut_Jpsiee",
    ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support, Jpsi ee'), 
                    }])))

HLT_list=['HLT_e5_lhtight_nod0_e4_etcut_Jpsiee']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support, Jpsi ee'), 
                    }])))

HLT_list=['HLT_e5_lhtight_nod0_e4_etcut_Jpsiee_L1RD0_FILLED']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support, Jpsi ee ATR-16855, Express ATR-17782', ESRate=0),
                   15001 : TriggerRule(PS=-1, comment='Disable, Jpsi ee ATR-16855, Express ATR-17782', ESRate=0), 
                    }])))

HLT_list=['HLT_e5_lhtight_nod0_e4_etcut']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support, Jpsi ee'),
                    }])))


########################################
# SM W + nJets

egamma_Rules_supporting.update({
    # 12 Hz support
    # 1.2e34
        'HLT_e26_lhvloose_nod0_L1EM22VH_3j20noL1'              : {  1000 : TriggerRule(PS=5, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                   MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        'HLT_e26_lhvloose_nod0_L1EM22VH_4j20noL1'              : {  1000 : TriggerRule(PS=3, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                    MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        'HLT_e26_lhvloose_nod0_L1EM22VH_5j15noL1'              : {  1000 : TriggerRule(PS=3, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                    MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        'HLT_e26_lhvloose_nod0_L1EM22VH_6j15noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                    MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        # 2e34
        'HLT_e28_lhvloose_nod0_L1EM22VH_3j20noL1'              : {  1000 : TriggerRule(PS=9, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        'HLT_e28_lhvloose_nod0_L1EM22VH_4j20noL1'              : {  1000 : TriggerRule(PS=4, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        'HLT_e28_lhvloose_nod0_L1EM22VH_5j15noL1'              : {  1000 : TriggerRule(PS=5, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        'HLT_e28_lhvloose_nod0_L1EM22VH_6j15noL1'              : {  1000 : TriggerRule(PS=3, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        
    # 12 Hz support
        
    'HLT_e26_lhmedium_nod0_L1EM22VH_3j20noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e26_lhmedium_nod0_L1EM22VH_4j20noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e26_lhmedium_nod0_L1EM22VH_5j15noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e26_lhmedium_nod0_L1EM22VH_6j15noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                MainPrimary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e28_lhmedium_nod0_L1EM22VH_3j20noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, targer 0.8Hz') ,
                                                                peakLumi : TriggerRule(PS=-1, comment='Disabled') }, 
    'HLT_e28_lhmedium_nod0_L1EM22VH_4j20noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                peakLumi : TriggerRule(PS=-1, comment='Disabled')  }, 
    'HLT_e28_lhmedium_nod0_L1EM22VH_5j15noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                peakLumi : TriggerRule(PS=-1, comment='Disabled')  }, 
    'HLT_e28_lhmedium_nod0_L1EM22VH_6j15noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                peakLumi : TriggerRule(PS=-1, comment='Disabled')  },
        
})

########################################
# other rules

egamma_Rules_supporting.update({

    # ----------------------------------------
    # Di-photon
    #
    # for 2g20_tight
    # 7 Hz at 1e34, can we not use rate = 1 command instead ?
    'HLT_2g20_loose'                               : {  1000 : TriggerRule(PS=10, comment='Support') },

    'HLT_2g20_loose_L12EM15'                       : {  1000 : TriggerRule(rate=1.0, comment='Support') },
    'HLT_2g10_loose'                               : {  1000 : TriggerRule(rate=0.2, ESValue=0, comment='Support') },

    #
    # what supporting ??? ask
    'HLT_g35_medium_g25_medium_L1EM7_EMPTY'        : {  1000 : TriggerRule(PS=1, comment='Support') },
    'HLT_g35_medium_g25_medium_L1EM7_UNPAIRED_ISO' : {  1000 : TriggerRule(PS=1, comment='Support') },

    'HLT_2g22_tight_L1EM7_UNPAIRED_ISO' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },
    'HLT_2g22_tight_L1EM7_EMPTY' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },
    'HLT_2g50_tight_L1EM7_UNPAIRED_ISO' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },
    'HLT_2g50_tight_L1EM7_EMPTY' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },

    # ----------------------------------------
    # idperf
    # rate= 6 Hz
    'HLT_e5_lhloose_idperf'                        : {  1000 : TriggerRule(rate=0.5, comment='Support') },
    'HLT_e5_lhloose_nod0_idperf'                   : {  1000 : TriggerRule(rate=0.5, comment='Support') },
    'HLT_e5_lhtight_idperf'                        : {  1000 : TriggerRule(rate=0.5, comment='Support') },
    'HLT_e10_lhtight_idperf'                       : {  1000 : TriggerRule(rate=0.5, comment='Support') },
    'HLT_e24_lhmedium_idperf_L1EM20VH'             : {  1000 : TriggerRule(rate=1.5, comment='Support') },

    # ----------------------------------------
    # perf
    # rate= 2 Hz
    'HLT_g0_perf_L1EM15'    : {  1000 : TriggerRule(rate=1.0, comment='Support') },
    'HLT_e0_perf_L1EM15'    : {  1000 : TriggerRule(rate=1.0, comment='Support') },
    'HLT_noalg_L1EM15VH'    : {  1000 : TriggerRule(rate=1.0, comment='Support ATR-13493') },
    
})





########################################

egamma_Rules=RulebookDict(tag,'egamma_Rules')

RulesList=[egamma_Rules_2e34, egamma_Rules_1e34, egamma_Rules_0p5e34, egamma_Rules_0p3e34, egamma_Rules_supporting]

for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in egamma_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    egamma_Rules.update(Rules)


#################################################################################################
#
#
# Combined
#
#
#################################################################################################


########################################
# 2e34 

combined_Rules_2e34=RulebookDict(tag,'combined_Rules_2e34')

HLT_list=[
    # ATR-9486
    # e + g
    'HLT_e20_lhmedium_nod0_g35_loose',
    'HLT_e24_lhmedium_nod0_L1EM15VH_g25_medium',
    # e + mu
    'HLT_e17_lhloose_nod0_mu14',
    'HLT_e7_lhmedium_nod0_mu24',
    'HLT_e26_lhmedium_nod0_mu8noL1',
    "HLT_e28_lhmedium_nod0_mu8noL1",
    "HLT_e28_lhmedium_nod0_L1EM24VHIM_mu8noL1",    
    # e + 2mu
    'HLT_e12_lhloose_nod0_2mu10',
    'HLT_e12_lhmedium_nod0_2mu10',
    # 2e + mu
    'HLT_2e12_lhloose_nod0_mu10',
    'HLT_2e12_lhmedium_nod0_mu10',
    # mu + g . ATR-11623
    'HLT_g25_medium_mu24',
    # mu + 2g
    'HLT_2g10_loose_mu20',
    'HLT_2g10_medium_mu20',
    # e + xe
    'HLT_e70_lhloose_nod0_xe70noL1',
    'HLT_e80_lhloose_nod0_xe80noL1',
    'HLT_e80_lhloose_nod0_L1EM24VHI_xe80noL1',
    "HLT_e70_lhloose_nod0_L1EM24VHIM_xe70noL1",
    "HLT_e80_lhloose_nod0_L1EM24VHIM_xe80noL1",
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     }])))




HLT_list=[
    # e+2g
    'HLT_e20_lhmedium_nod0_2g10_loose_L1EM15VH_3EM8VH', 
    'HLT_e20_lhmedium_nod0_2g10_medium_L1EM15VH_3EM8VH', 

    # 3e
    'HLT_e17_lhloose_nod0_2e10_lhloose_nod0_L1EM15VH_3EM8VH',
    "HLT_e17_lhvloose_nod0_2e10_lhvloose_nod0_L1EM15VH_3EM8VH",    
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    10001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

HLT_list=[
    # g + xe (loose)
    'HLT_g70_loose_xe70noL1',
    'HLT_g70_loose_L1EM24VHI_xe70noL1',
    'HLT_g70_loose_L1EM24VHIM_xe70noL1',
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Higgs'),
                   14001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

L1_list=['L1_EM15VH_3EM7']
combined_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(rate=1000, comment='Support, relatively huge rate at all lumis'),
                   12001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

HLT_list=[
    # e + 2g
    'HLT_e20_lhmedium_nod0_2g10_loose',
    'HLT_e20_lhmedium_nod0_2g10_medium',
    'HLT_e17_lhloose_nod0_2e9_lhloose_nod0',
    'HLT_e17_lhmedium_nod0_2e9_lhmedium_nod0',
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   12001 : TriggerRule(PS=-1, comment='Disabled, ATR-14662'), 
    }])))


HLT_list=[
    'HLT_g85_tight_L1EM22VHI_3j50noL1', 'HLT_g100_tight_L1EM22VHI_3j50noL1',
    "HLT_g85_tight_icalovloose_L1EM22VHI_3j50noL1",
    'HLT_g100_tight_icalovloose_L1EM22VHI_3j50noL1',

    "HLT_g85_tight_L1EM24VHI_3j50noL1",
    "HLT_g85_tight_L1EM24VHIM_3j50noL1",
    
    'HLT_g80_loose_xe80noL1', 
    'HLT_g80_loose_L1EM24VHI_xe80noL1',
    'HLT_g80_loose_L1EM24VHIM_xe80noL1',
    'HLT_g80_loose_icalovloose_xe80noL1',

    ]    
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'),
                   Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    'HLT_g85_tight_icalovloose_L1EM24VHI_3j50noL1',
    'HLT_g85_tight_icalovloose_L1EM24VHIM_3j50noL1',
    'HLT_g100_tight_icalovloose_L1EM24VHI_3j50noL1',
    'HLT_g100_tight_icalovloose_L1EM24VHIM_3j50noL1',
    "HLT_g100_tight_L1EM24VHI_3j50noL1",
    'HLT_g100_tight_L1EM24VHIM_3j50noL1',
    'HLT_g80_loose_icalovloose_L1EM24VHI_xe80noL1',
    'HLT_g80_loose_icalovloose_L1EM24VHIM_xe80noL1',      
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'),
                    }])))

HLT_list=['HLT_j80_bmv2c1050_split_xe60_L12J50_XE40',
]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'),
                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    # mu + g
    'HLT_g35_tight_icalotight_L1EM24VHIM_mu15noL1_mu2noL1',
    'HLT_g35_tight_icalotight_L1EM24VHIM_mu18noL1',
    'HLT_g40_tight_icalotight_L1EM24VHIM_mu15noL1_mu2noL1',
    'HLT_g40_tight_icalotight_L1EM24VHIM_mu18noL1',
    "HLT_g35_loose_L1EM24VHIM_mu15_mu2noL1",
    "HLT_g35_loose_L1EM24VHIM_mu18",
    
    "HLT_g35_tight_icalotight_L1EM24VHI_mu15noL1_mu2noL1",
    "HLT_g35_tight_icalotight_L1EM24VHI_mu18noL1",
    "HLT_g40_tight_icalotight_L1EM24VHI_mu15noL1_mu2noL1",
    "HLT_g40_tight_icalotight_L1EM24VHI_mu18noL1",
    "HLT_g35_loose_L1EM24VHI_mu15_mu2noL1",
    "HLT_g35_loose_L1EM24VHI_mu18",    
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary, ATR-15233, ATR-11623'),
                     }])))

HLT_list=[
    # mu + g . ATR-11623
    'HLT_g35_loose_L1EM24VHI_mu15noL1_mu2noL1',
    'HLT_g35_loose_L1EM24VHI_mu18noL1',
    'HLT_g35_loose_L1EM24VHIM_mu15noL1_mu2noL1',
    'HLT_g35_loose_L1EM24VHIM_mu18noL1',
    'HLT_g35_loose_L1EM22VHI_mu15noL1_mu2noL1',
    'HLT_g35_loose_L1EM22VHI_mu18noL1',
    'HLT_g35_loose_L1EM22VHIM_mu15noL1_mu2noL1',
    'HLT_g35_loose_L1EM22VHIM_mu18noL1',
]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=-1, comment='Disabled, ATR-11623, disabled tentatively for CPU prediction. Alternatives should be put with new release'),
                     }])))

HLT_list=[
    # 2j + xe (+invmass, deta): Higgs invisible, MET+2jet searches, ATR-12151
    'HLT_j40_0eta490_j30_0eta490_deta25_xe80_L1XE50',    
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
               1000 : TriggerRule(PS=-1, comment='Disabled, Higgs, ATR-12151', PTRate=1.0), 
               }])))




########################################
# 1e34 

combined_Rules_1e34=RulebookDict(tag,'combined_Rules_1e34')

#who asks for this? is it Higgs + photon, Higgs invisible? what about e+xe ?
HLT_list=[
    # e + xe
    'HLT_e60_lhloose_nod0_xe60noL1',
    'HLT_e60_lhloose_nod0_L1EM24VHIM_xe60noL1',
    # g + xe (loose)
    'HLT_g60_loose_xe60noL1',
    "HLT_g60_loose_L1EM24VHI_xe60noL1",
    "HLT_g60_loose_L1EM26VHI_xe60noL1",


    ]
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      7001 : TriggerRule(PS=-1, comment='Disabled'),
                      }])))

# unisolated lepton + jets for RPV l+multijets
HLT_list=['HLT_mu24_5j30_0eta240_L1MU10_3J20', 'HLT_e24_lhmedium_nod0_5j30_0eta240_L1EM20VH_3J20',]
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Support'), 
                      7001 : TriggerRule(PS=15, comment='Support'),
                      }])))
HLT_list=['HLT_mu26_5j30_0eta240_L1MU10_3J20', 'HLT_e26_lhmedium_nod0_5j30_0eta240_L1EM20VH_3J20',]
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Support'), 
                      10001 : TriggerRule(PS=15, comment='Support'),
                      }])))

########################################
# other rules

combined_Rules_other={

    # ----------------------------------------
    # jet + xe

    # ATR-11303, SUSY
    # --> keep both j80/100_xe80 unPS until 1.5e34

    # 1.5e34
    'HLT_j100_xe80'                      : {  1000 : TriggerRule(PS=-1, comment='Disabled'),
                                              }, 

    'HLT_j80_xe80'                       : {  1000 : TriggerRule(rate=0.2, comment='Support')},

    'HLT_j40_cleanT_xe85_tc_lcw_2dphi05_L1XE50' : {  1000 : TriggerRule(rate=0.5, comment='Support, ATR-13131')},
    
    # ----------------------------------------
    # g + xe (tight)- ATR-12901 Higgs -> photon + dark photon 

    # up to 2e34 w/o PS, after with PS
    'HLT_g45_tight_L1EM22VHI_xe45noL1'   : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                              7001 : TriggerRule(PS=50, comment='Primary, ATR-12901 EndOfFill'),
                                             Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled, ATR-12901'),
                                              },

    "HLT_g45_tight_L1EM24VHI_xe45noL1"    : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                               7001 : TriggerRule(PS=100, comment='Primary, ATR-12901 EndOfFill'),
                                              peakLumi : TriggerRule(PS=200, comment='Primary, ATR-12901 EndOfFill Needs proposal'),
                                               },

    "HLT_g45_tight_L1EM24VHIM_xe45noL1"    : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                               7001 : TriggerRule(PS=100, comment='Primary, ATR-12901 EndOfFill'),
                                              peakLumi : TriggerRule(PS=200, comment='Primary, ATR-12901 EndOfFill Needs proposal'),
                                               },
    
    "HLT_g45_tight_icalovloose_L1EM22VHI_xe45noL1"   : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                                          7001 : TriggerRule(PS=10, comment='Primary, ATR-12901 EndOfFill'),
                                                         Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled, ATR-12901'),
                                                          },

    "HLT_g45_tight_icalovloose_L1EM24VHI_xe45noL1":{   1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                                       7001 : TriggerRule(PS=10, comment='Primary, ATR-12901 EndOfFill'),
                                                      peakLumi : TriggerRule(PS=20, comment='Disabled, ATR-12901'),                                                      
                                                       },

    "HLT_g45_tight_icalovloose_L1EM24VHIM_xe45noL1":{   1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                                       7001 : TriggerRule(PS=10, comment='Primary, ATR-12901 EndOfFill'),
                                                      peakLumi : TriggerRule(PS=20, comment='Disabled, ATR-12901'),                                                      
                                                       },
    
    #------------------------
    # mono-b analysis , ATR-12947

    'HLT_j80_bmv2c1060_split_xe60_L12J50_XE40'      :  {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12947, ATR-14196'),
                                                         14001 : TriggerRule(PS=-1, comment='Disabled, ATR-12947'),
                                                          },
    'HLT_j80_bmv2c1077_split_xe70_L1J400ETA25_XE50' :  { 1000 : TriggerRule(PS=1, comment='Primary, ATR-12947'),
                                                         7001 : TriggerRule(PS=-1, comment='Disabled')},

    #--------------------
    # low-mass leptophobic Z' in association with an ISR photon

    'HLT_g75_tight_3j50noL1'             : {  1000 : TriggerRule(PS=-1, comment='Primary, ATR-12750')},
    'HLT_g75_tight_L1EM22VHI_3j50noL1'             : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12750'),
                                                       14001 : TriggerRule(PS=-1, comment='Disable, ATR-12750'),
                                                        },

    # ---------------------
    # hh -> WWbb -> lnu jj bb
    # Needs to tighten EM18VH_3J20 more

    'L1_MU10_3J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900') },
   
    'HLT_mu14_ivarloose_3j20_L1MU10_3J20' : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900') },
    'HLT_mu14_3j20_L1MU10_3J20' : {  1000 : TriggerRule(PS=100, comment='Support, ATR-15524') },

    'L1_EM18VHI_3J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-15449') },
    'HLT_e20_lhtight_nod0_ivarloose_3j20_L1EM18VHI_3J20' : { 1000 : TriggerRule(PS=1, comment='Primary, ttHbb hh -> WWbb -> lnu jj bb, ATR-15449') },

    'HLT_e20_lhtight_nod0_3j20_L1EM18VHI_3J20' : { 1000 : TriggerRule(PS=50, comment='Support, ATR-15449') },
    'HLT_e20_lhmedium_nod0_3j20_L1EM18VHI_3J20' : { 1000 : TriggerRule(PS=50, comment='Support, ATR-15449') },

    
    
    # ---------------------
    # VH , H->bb V=W -> mu
    'L1_MU10_2J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900') },
    'HLT_mu20_ivarmedium_L1MU10_2J20' : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13367') },
    'L1_MU10_2J15_J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900'),
                                Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable'),
                                 },
    'HLT_mu20_ivarmedium_L1MU10_2J15_J20' : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13367'),
                                              Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable'),
                                               },

    # SUSY 3bjet+MET analysis ATR-15232
    # replaced MET as per ATR-17678
    'HLT_j85_gsc100_bmv2c1050_split_xe80_mht_L1XE60' : {  1000 : TriggerRule(PS=-1, comment='Primary, SUSY 3bjet+MET analysis ATR-15232'),
                                                      }, 
    'HLT_2j35_gsc45_bmv2c1070_split_xe80_mht_L12J15_XE55' : {  1000 : TriggerRule(PS=-1, comment='Primary, SUSY 3bjet+MET analysis ATR-15232'), 
                                                        },
    'HLT_3j35_bmv2c1077_split_xe60_mht_L13J15.0ETA25_XE40' : {  1000 : TriggerRule(PS=-1, comment='Primary, SUSY 3bjet+MET analysis ATR-15232'), #6Hz
                                                        },
##
    'HLT_j85_gsc100_bmv2c1050_split_xe85_pufit_xe50_L1XE55' : {  1000 : TriggerRule(PS=1, comment='Primary, SUSY 3bjet+MET ATR-15232/ATR-17678'),# 13 Hz
                                                              }, 
    'HLT_2j35_gsc45_bmv2c1070_split_xe85_pufit_xe50_L12J15_XE55' : {  1000 : TriggerRule(PS=1, comment='Primary, SUSY 3bjet+MET ATR-15232/ATR-17678'), #5Hz
                                                                   },
    'HLT_3j35_bmv2c1077_split_xe70_pufit_xe50_L13J15.0ETA25_XE40' : {  1000 : TriggerRule(PS=1, comment='Primary, SUSY 3bjet+MET ATR-15232/ATR-17678'), #3Hz
                                                                  },

}
   
    
########################################


combined_Rules=RulebookDict(tag,'combined_Rules')

RulesList=[combined_Rules_2e34, combined_Rules_1e34, combined_Rules_other]
for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in combined_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    combined_Rules.update(Rules)


#################################################################################################
#
#
# VBF
#
#
#################################################################################################


########################################
# 2e34 

vbf_Rules_2e34=RulebookDict(tag,'vbf_Rules_2e34')

HLT_list=[
    'HLT_j55_gsc80_bmv2c1070_split_j45_gsc60_bmv2c1085_split_j45_320eta490',
    'HLT_j45_gsc55_bmv2c1070_split_2j45_320eta490_L1J25.0ETA23_2J15.31ETA49',
    ]
vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H->bb , ATR-13526, ATR-15044'), 
                    }])))

HLT_list=[
    'HLT_j55_0eta240_2j45_320eta490_L1J25.0ETA23_2J15.31ETA49',
    ]
vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support, VBF H->bb, ATR-15044'), 
                    }])))

HLT_list=[
    'HLT_ht300_2j40_0eta490_invm700_L1HT150-J20s5.ETA31_MJJ-400-CF_AND_2j35_gsc45_bmv2c1070_split',
    ]
vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H->bb , ATR-13526, ATR-15044'), 
                    }])))

HLT_list=[
    'HLT_ht300_2j40_0eta490_invm700_L1HT150-J20s5.ETA31_MJJ-400-CF',
    ]
vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support, VBF H->bb, ATR-15044'), 
                    }])))
    
# VBF + 4 j
HLT_list=['HLT_g25_medium_L1EM22VHI_2j35_0eta490_bmv2c1077_split_2j35_0eta490', 'HLT_g25_medium_L1EM22VHI_j35_0eta490_bmv2c1077_split_3j35_0eta490_invm700', ]
vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H->bb + photon , ATR-12120'), 
                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_g20_tight_icaloloose_j35_bmv2c1077_split_3j35_0eta490_invm500',
          'HLT_g27_medium_L1EM24VHI_j35_bmv2c1077_split_3j35_0eta490_invm700',
          'HLT_g27_medium_L1EM24VHI_2j35_bmv2c1077_split_2j35_0eta490',
          ]
vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H->bb + photon , ATR-16986. Non-gsc backup'), 
                    }])))


# 1.5e34 

vbf_Rules_1p5e34=RulebookDict(tag,'vbf_Rules_1p5e34')

HLT_list=['HLT_g25_medium_L1EM22VHI_4j35_0eta490_invm1000',]
vbf_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H +photon, ATR-12120, ATR-13373, 2e34?'), 
                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))
HLT_list=['HLT_g25_medium_L1EM22VHI_4j35_0eta490_invm700',]
vbf_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H +photon, ATR-12120, ATR-13373, backup, EndOfFill'),
                    7001 : TriggerRule(PS=10, comment='Support, VBF H +photon, ATR-12120, ATR-13373, backup'), 
                    }])))
    
########################################
# 1e34 

vbf_Rules_1e34=RulebookDict(tag,'vbf_Rules_1e34')
HLT_list=['HLT_mu6_ivarloose_2j40_0eta490_invm1000_L1MU6_J30.0ETA49_2J20.0ETA49',]

vbf_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(PS=100, comment='Support, Backup for L1Topo, mu+VBF, ATR-10979 ATR-13373, PSed until alternative trigger is found'), 
                             Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                             }]))) 

########################################

# 0.5e34 

vbf_Rules_0p5e34=RulebookDict(tag,'vbf_Rules_0p5e34')

# vbf + mu, very large rate , can we reduce?
HLT_list=['HLT_mu6_2j40_0eta490_invm800_L1MU6_J30.0ETA49_2J20.0ETA49',]
vbf_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=10, comment='Support, mu+VBF, ATR-13373, PSed until alternative trigger is found'), 
                         5001 : TriggerRule(PS=-1, comment='Disabled'),
                         }])))

########################################
# 0.3e34 

vbf_Rules_0p3e34=RulebookDict(tag,'vbf_Rules_0p3e34')

# vbf + mu, very large rate , can we reduce?
HLT_list=['HLT_mu6_2j40_0eta490_invm600_L1MU6_J30.0ETA49_2J20.0ETA49',]
vbf_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=10, comment='Support, mu+VBF, ATR-13373, PSed until alternative trigger is found'), 
                          3001 : TriggerRule(PS=-1, comment='Disabled'),
                          }])))

########################################
# 0.15e34 

vbf_Rules_0p15e34=RulebookDict(tag,'vbf_Rules_0p15e34')

# vbf + mu, very large rate , can we reduce?
HLT_list=['HLT_mu6_2j40_0eta490_invm400_L1MU6_J30.0ETA49_2J20.0ETA49',]
vbf_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=10, comment='Support, mu+VBF, ATR-13373, PSed until alternative trigger is found'), 
                          1501 : TriggerRule(PS=-1, comment='Disabled'),
                          }])))

########################################
# supporting

vbf_Rules_supporting=RulebookDict(tag,'vbf_Rules_supporting')

HLT_list=['HLT_g25_loose_L1EM20VH_4j35_0eta490']
vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(rate=1., comment='Support, ATR-12120'), 
                           }])))

HLT_list=['HLT_j80_0eta240_2j60_320eta490','HLT_j80_0eta240_j60_j45_320eta490',]
vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(rate=0.5, comment='Support, VBF H->bb , ATR-13526'), 
                      }])))

HLT_list=['HLT_j80_0eta240_j60_j45_320eta490_AND_2j35_gsc45_bmv2c1070_split', ]         
vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H->bb , ATR-13526'),
                    }])))

########################################

vbf_Rules=RulebookDict(tag,'vbf_Rules')

RulesList=[vbf_Rules_2e34, vbf_Rules_1p5e34, vbf_Rules_1e34, vbf_Rules_0p5e34, vbf_Rules_0p3e34, vbf_Rules_0p15e34, vbf_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in vbf_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    vbf_Rules.update(Rules)


#################################################################################################
#
#
# Dedicated
#
#
#################################################################################################

dedicated_Rules_supporting = {

    # ========================================
    # Drell-Yan
    
    # supporting
    # PS=10 wrt main
    'HLT_mu6_iloose_mu6_11invm24_noos_novtx'                    : {  1000 : TriggerRule(PS=-1, comment='Disable 2016/2015 Support, DY, ATR-13363,ATR-14680')},
    'HLT_mu6_iloose_mu6_24invm60_noos_novtx'                    : {  1000 : TriggerRule(PS=-1, comment='Disable 2016/2015 Support, DY, ATR-13363,ATR-14680')},
    'HLT_mu4_iloose_mu4_7invm9_noos_novtx'                      : {  1000 : TriggerRule(PS=-1, comment='Disable 2016/2015 Support, DY, ATR-13363') },
    'HLT_mu4_iloose_mu4_11invm60_noos_novtx'                    : {  1000 : TriggerRule(PS=-1, comment='Disable 2016/2015 Support, DY, ATR-13363') },

    # ========================================
    # razor

    # supporting
    'HLT_j30_xe10_razor100'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },

    'HLT_j30_xe60_razor100'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },

    # ========================================
    # LLP
    
    # supporting (background, rerun)
    'HLT_mu20_msonly'                                           : {  1000 : TriggerRule(rate=0.2, comment='Support, LLP', rerun=1) },
 
    # ========================================
    # LFV tau->mu gamma

    # supporting 

    'HLT_g10_etcut_L1EM8I_mu10_iloose_taumass'                   : {  1000 : TriggerRule(PS=-1, comment='Disable') },
    #'HLT_g20_etcut_L1EM15I_mu4_iloose_taumass'                   : {  1000 : TriggerRule(rate=1.0, comment='Support') },
    "HLT_g20_etcut_L1EM15_mu4_taumass"                           : {  1000 : TriggerRule(PS=-1, comment='Disable') },
    # ========================================
    # h->phi gamma -> KK gamma

    # supporting    
    'HLT_tau25_dikaonmass_tracktwo'                 : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13061',rerun=1) },
    'HLT_tau25_dipion1_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau25_kaonpi1_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau25_dipion2_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau25_kaonpi2_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau25_singlepion_tracktwo'                 : {  1000 : TriggerRule(PS=0, comment='Support, ATR-11531',rerun=1) },

    
    }

dedicated_Rules_2e34_L1Topo=RulebookDict(tag,'dedicated_Rules_2e34_L1Topo')

HLT_list=[
    'HLT_g10_loose_mu10_iloose_taumass_L1LFV-EM8I',
    'HLT_g20_loose_mu4_iloose_taumass_L1LFV-EM15I',
    ]
dedicated_Rules_2e34_L1Topo.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, LFV/LLP L1Topo'), 
                   15001 : TriggerRule(PS=-1, comment='Disabled, LFV/LLP L1Topo'), 
                    }])))
    
dedicated_Rules_2e34 = {

    # ========================================
    # Drell-Yan
    #

    'HLT_mu6_iloose_mu6_11invm24_noos'                          : {  1000 : TriggerRule(PS=-1, comment='Disable, 2016/2015 Primary, DY, ATR-13363,ATR-14680')},
    'HLT_mu6_iloose_mu6_24invm60_noos'                          : {  1000 : TriggerRule(PS=-1, comment='Disable, 2016/2015 Primary, DY, ATR-13363,ATR-14680')},
    'HLT_mu4_iloose_mu4_7invm9_noos'                            : {  1000 : TriggerRule(PS=-1, comment='Disable, 2016/2015 Primary, DY, ATR-13363, EndOfFill') },
    'HLT_mu4_iloose_mu4_11invm60_noos'                          : {  1000 : TriggerRule(PS=-1, comment='Disable, 2016/2015 Primary, DY, ATR-13363, EndOfFill') },

    'HLT_2mu6_10invm30_pt2_z10'                                 : {  1000 : TriggerRule(PS=-1, comment='Disable, 2016/2015 Primary, DY, ATR-11516, EndOfFill'),
                                                                     10001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill') },

   

    # ========================================
    # razor

    'L1_2J15_XE55'                                              : {  1000 : TriggerRule(PS=1, comment='Primary L1,razor, ATR-12890') },

    'HLT_j30_xe60_razor170'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },
    
    'HLT_j30_xe60_razor185'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },
    
    'HLT_j30_xe60_razor195'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },

    'HLT_j30_xe10_razor170'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },

    'HLT_j30_xe10_razor185'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },

    'HLT_j30_xe10_razor195'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },

    'HLT_j30_xe60_razor220'                                     : {  1000 : TriggerRule(PS=-1, comment='Disable razor, ATR-17795') },

    # ========================================
    # LLP

    # ----------------------------------------
    'HLT_j30_muvtx'                                             : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933') },
    # supporting
    'HLT_j30_muvtx_noiso'                                       : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') },
    'HLT_j30_muvtx_L1MU6_EMPTY'                                 : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') },
    'HLT_j30_muvtx_L1MU4_UNPAIRED_ISO'                          : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') },
    'HLT_j30_muvtx_noiso_L1MU6_EMPTY'                           : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled, ATR-13379') },
    'HLT_j30_muvtx_noiso_L1MU4_UNPAIRED_ISO'                    : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled, ATR-13379') },

    # ----------------------------------------
    #
    'HLT_g15_loose_2mu10_msonly'                                : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933') },
    # supporting
    'HLT_g15_loose_2mu10_msonly_L1MU6_EMPTY'                    : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') },
    'HLT_g15_loose_2mu10_msonly_L1MU4_UNPAIRED_ISO'             : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') },

    # ----------------------------------------
    #
    # supporting
    'HLT_3mu6_msonly_L1MU6_EMPTY'                               : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') },
    'HLT_3mu6_msonly_L1MU4_UNPAIRED_ISO'                        : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') },

    # ----------------------------------------
    # 
    #v6
    'HLT_j30_jes_PS_llp_L1TAU60'                                : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933'),
                                                                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_L1TAU60'                       : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933'),
                                                                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_pufix_L1TAU60'                 : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-15255'),
                                                                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-15255'), 
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_pufix_noiso_L1TAU60'           : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-15255'),
                                                                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-15255'), 
                                                                     },
    'HLT_j30_jes_PS_llp_noiso_L1TAU60'                          : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933'),
                                                                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU60'                 : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933'),
                                                                     Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),                                                                   },
    'HLT_j200_jes_cleanLLP_PS_revllp_trkiso_L1TAU60'           : {  1000 : TriggerRule(PS=100, comment='Support, REVLLP, ATR-14293, Needs alternative, apparently buggy'),
                                                                    7001 : TriggerRule(PS=-1, comment='Disabled, REVLLP, ATR-14293') }, 
    'HLT_j230_jes_cleanLLP_PS_revllp_L1TAU60'                  : {  1000 : TriggerRule(PS=100, comment='Support, REVLLP, ATR-14293, EndOfFill'),
                                                                    7001 : TriggerRule(PS=-1, comment='Disabled, REVLLP, ATR-14293') }, 
    
    'HLT_j30_jes_cleanLLP_PS_llp_L1TAU40'                       : {  1000 : TriggerRule(PS=-1, comment='Primary, LLP, ATR-10933'),
                                                                     7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_j30_jes_cleanLLP_PS_llp_L1TAU30'                       : { 1000 : TriggerRule(PS=-1, comment='Disabled') },


    'HLT_j30_jes_cleanLLP_PS_llp_L1TAU30_EMPTY'                  : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') }, 
    'HLT_j30_jes_cleanLLP_PS_llp_L1TAU30_UNPAIRED_ISO'           : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-10933 should not be prescaled') }, 
 
    'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_EMPTY'                  : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-13379') }, 
    'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_UNPAIRED_ISO'           : {  1000 : TriggerRule(PS=1, comment='Support, LLP, ATR-13379') }, 
    

    
    # ========================================
    # LFV tau->3mu 
    'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1MU11_2MU6'        : {  1000 : TriggerRule(PS=-1, comment='Primary, LFV tau 3mu') },            
    'HLT_2mu10_nomucomb'                                    : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu'),
                                                                 5001: TriggerRule(PS=-1, comment='Disabled') },  #SX 090216          

    'L1_2MU6_3MU4'                                          : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu')},

    'HLT_mu20_nomucomb_mu6noL1_nscan03'            : {  1000 : TriggerRule(PS=-1, comment='Disable') },


    
    

    # ========================================
    # LFV tau->emu 

    'L1_EM8I_MU10'                                               : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
                                                                      10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    'L1_EM15I_MU4'                                               : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
                                                                      10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    'HLT_g10_loose_L1EM8I_mu10_iloose_taumass'                   : {  1000 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo'),},

    #'HLT_g20_loose_L1EM15I_mu4_iloose_taumass'                   : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
    #                                                                  10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    "HLT_g20_loose_L1EM15_mu4_iloose_taumass"                    : {  1000 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    # ========================================
    # HIP
    "HLT_g0_hiptrt_L1EM24VHIM"                                  : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259') },
    "HLT_g0_hiptrt_L1EM24VHI"                                   : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259') },
    'HLT_g0_hiptrt_L1EM22VHI'                                   : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259'),
                                                                     Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g0_hiptrt_L1EM20VHI'                                   : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259'),
                                                                     10001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g0_hiptrt_L1EM20VH'                                    : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259'),
                                                                     3001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g0_hiptrt_L1EM18VH'                                    : {  1000 : TriggerRule(PS=-1, comment='Disabled') },

    
    # ========================================
    # stopped gluino

    'HLT_j55_0eta240_xe50_L1J30_EMPTY'                          : {  1000 : TriggerRule(PS=1, comment='Primary, stopped gluino, ATR-11193') },
    'HLT_j55_0eta240_xe50_L1J30_FIRSTEMPTY'                          : {  1000 : TriggerRule(PS=1, comment='Primary, stopped gluino, ATR-17705') },

    # Low-mass diphoton searches
    'HLT_2g6_tight_icalotight_L1J100': {  1000 : TriggerRule(PS=1, comment='Primary, low-mass diphoton searches, ATR-17462'),},
    'HLT_2g6_loose_L1J100': {  1000 : TriggerRule(rate=0.2, comment='Support, ATR-17462'),},

    # SUSY photon+multijet searches
    'HLT_g45_loose_6j45_0eta240': {  1000 : TriggerRule(PS=1, comment='Primary, SUSY photon+multijet searches, ATR-17502'),},
    'HLT_g45_loose_5j45_0eta240': {  1000 : TriggerRule(PS=10, comment='Primary, SUSY photon+multijet searches, ATR-17502'),},

    # Exotics Z' searches
    'HLT_mu80_msonly_3layersEC': {  1000 : TriggerRule(PS=1, comment='Primary, search for a neutral, long-lived Zprime, ATR-13685'),},
    
    # exotics HNL
    'HLT_mu22_mu8noL1_mu6noL1': {  1000 : TriggerRule(PS=1, comment='Primary, exotics HNL, ATR-15792'),},
    'HLT_mu20_mu8noL1_e9_lhvloose_nod0_L1EM8VH_MU20' : {  1000 : TriggerRule(PS=1, comment='Backup, exotics HNL, ATR-15792, ATR-15724'),},
    'HLT_mu20_mu8noL1_e9_lhvloose_nod0_L1EM7_MU20' : {  1000 : TriggerRule(PS=1, comment='Backup, exotics HNL, ATR-15792, ATR-15724'),},
    'HLT_mu20_mu8noL1_e9_lhvloose_nod0' : {  1000 : TriggerRule(PS=1, comment='Primary, exotics HNL, ATR-15792, ATR-15724'),},
    
    # H->aa->bbmumu
    'HLT_mu20_ivarmedium_mu8noL1' : {  1000 : TriggerRule(PS=1, comment='Primary, H->aa->bbmumu, ATR-17539'),},
}

L1_list=['L1_MU6_EMPTY']
if do2018:
    dedicated_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        peakLumi : TriggerRule(PS=10, comment='Primary (PSed for the 2018 menu study only. Rate prediction with costAna is huge 25kHz)'),
                        }])))
else:
    dedicated_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=10, comment='Primary L1'),
                        peakLumi : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
    

HLT_list=['HLT_g25_medium_L1EM24VHI_tau25_dikaonmass_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_dipion1loose_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_dipion2_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_kaonpi1_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_kaonpi2_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_singlepion_tracktwo_50mVis10000',
          "HLT_g35_medium_L1EM24VHI_tau25_dipion3_tracktwo_60mVis10000",
          ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14947, ATR-16266, ATR-16600')
                    }])))
HLT_list=['HLT_g25_medium_L1EM24VHIM_tau25_dikaonmass_tracktwo_50mVis10000', 
          'HLT_g25_medium_L1EM24VHIM_tau25_dipion1loose_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHIM_tau25_dipion2_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHIM_tau25_kaonpi1_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHIM_tau25_kaonpi2_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHIM_tau25_singlepion_tracktwo_50mVis10000',
          ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14947, ATR-16266')
                    }])))

HLT_list=[
    'HLT_j310_a10t_lcw_jes_60smcINF_j310_a10t_lcw_jes_L1SC111'
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, ATR-17822'),
                    }])))

HLT_list=[
    'HLT_j325_a10t_lcw_jes_60smcINF_j325_a10t_lcw_jes_L1SC111'
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-16940'),
                    12001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_j340_a10t_lcw_jes_60smcINF_j340_a10t_lcw_jes_L1SC111'
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-16940'),
                    15001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_j360_a10t_lcw_jes_60smcINF_j360_a10t_lcw_jes_L1SC111'
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill, ATR-16940'),
                    Primary2017Oct1p7E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_j370_a10t_lcw_jes_35smcINF_j370_a10t_lcw_jes_L1SC111',
    'HLT_j370_a10t_lcw_jes_40smcINF_j370_a10t_lcw_jes_L1SC111',
    'HLT_j370_a10t_lcw_jes_50smcINF_j370_a10t_lcw_jes_L1SC111',
    'HLT_j370_a10t_lcw_jes_60smcINF_j370_a10t_lcw_jes_L1SC111',
    'HLT_j380_a10t_lcw_jes_40smcINF_j380_a10t_lcw_jes_L1SC111',
    'HLT_j380_a10t_lcw_jes_50smcINF_j380_a10t_lcw_jes_L1SC111',
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-17691'),
                    Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_2j360_a10t_lcw_jes_35smcINF_L1J100',
    'HLT_2j370_a10t_lcw_jes_35smcINF_L1J100',
    'HLT_2j380_a10t_lcw_jes_35smcINF_L1J100',
]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-17614'),
                    Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[

    'HLT_j420_a10_lcw_subjes_L1SC111',
    'HLT_j440_a10_lcw_subjes_L1SC111',
    #'HLT_j460_a10_lcw_subjes_L1SC111', in ES (below)
    'HLT_j480_a10_lcw_subjes_L1SC111',
    'HLT_j500_a10_lcw_subjes_L1SC111',
    'HLT_j520_a10_lcw_subjes_L1SC111',
    'HLT_j540_a10_lcw_subjes_L1SC111',

    'HLT_j420_a10r_L1SC111',
    'HLT_j440_a10r_L1SC111',
    #'HLT_j460_a10r_L1SC111', in ES (below)
    'HLT_j480_a10r_L1SC111',
    'HLT_j500_a10r_L1SC111',
    'HLT_j520_a10r_L1SC111',
    'HLT_j540_a10r_L1SC111',

    #'HLT_j460_a10t_lcw_jes_L1SC111', in ES (below)
    'HLT_j480_a10t_lcw_jes_L1SC111',
    'HLT_j500_a10t_lcw_jes_L1SC111',
    'HLT_j520_a10t_lcw_jes_L1SC111',
    'HLT_j540_a10t_lcw_jes_L1SC111',
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-17614'),
                    Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_j460_a10_lcw_subjes_L1SC111',
    'HLT_j460_a10r_L1SC111',
    'HLT_j460_a10t_lcw_jes_L1SC111',
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-17614, Express ATR-17782', ESRate=0.4),
                    Primary2017Oct2p0E34 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    # mu-mu
    'HLT_2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30',
    # single-mu 
    'HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30',
    # mu-e 
    'HLT_e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',
    # e-e
    'HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',    
    # single-e
    'HLT_e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                1000 : TriggerRule(PS=1, comment='Primary, SUSY Higgsino, ATR-15243')
                }])))    
    
HLT_list=[
    'HLT_2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30',
    'HLT_e5_lhloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',
    'HLT_e5_lhmedium_nod0_j50_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',
    
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                1000 : TriggerRule(PS=1, comment='Primary, backup, SUSY Higgsino, ATR-15243')
                }])))    
    
HLT_list=[    
    'HLT_2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J40_XE50',
    'HLT_mu4_j90_xe90_pufit_2dphi10_L1MU4_XE60',
    'HLT_e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_XE60',
    'HLT_2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1XE60',
    'HLT_e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1XE60',
    ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, Backup, SUSY Higgsino, ATR-15243')
                    }])))    

HLT_list=['HLT_j250_jes_cleanLLP_PS_revllp_L1TAU60',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=100, comment='Support, Needs alternative, revLLP, ATR-14293'),
                    }])))
HLT_list=['HLT_mu20_msonly_mu15noL1_msonly_nscan05_noComb','HLT_mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary LLP, ATR-10933 should not be prescaled. Tentatively disabled. Alternative chains are being developed (ATR-14377) EndOfFill'),
                   10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
 'HLT_mu6_dRl1_mu20_msonly_iloosems_mu6noL1_dRl1_msonly',
 'HLT_mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU20_J40', 'HLT_mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU20_XE30', 'HLT_mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU6_EMPTY', 'HLT_mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU4_UNPAIRED_ISO',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary LLP, ATR-14377, New with L1_MU20_J40 or _XE30')
                     }])))  


HLT_list=['HLT_mu6_dRl1_mu20_msonly_mu6noL1_dRl1_msonly', 'HLT_mu20_msonly_mu6noL1_msonly_nscan05_L1MU20_J40', 'HLT_mu20_msonly_mu6noL1_msonly_nscan05_L1MU20_XE30', 'HLT_mu20_msonly_mu6noL1_msonly_nscan05_L1MU6_EMPTY', 'HLT_mu20_msonly_mu6noL1_msonly_nscan05_L1MU4_UNPAIRED_ISO',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled LLP, ATR-14377, New with L1_MU20_J40 or _XE30')
                     }])))  


HLT_list=['HLT_mu20_msonly_mu6noL1_msonly_nscan05', ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support LLP, ATR-14377')
                     }])))  


HLT_list=['HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb','HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled')
                    }])))
HLT_list=['HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO','HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO'] 
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled ATR-15236, ATR-14377')  
                    }])))
    
dedicated_Rules=RulebookDict(tag,'dedicated_Rules')


RulesList=[dedicated_Rules_2e34, dedicated_Rules_supporting, dedicated_Rules_2e34_L1Topo] 
    
for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in dedicated_Rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    dedicated_Rules.update(Rules)

#################################################################################################
#
#
# Data scouting
#
#
#################################################################################################

datascouting_Rules = {
    'HLT_j0_perf_ds1_L1J100'               : {  1000 : TriggerRule(PS=1, comment='Unprescaled for all lumis') },
    'HLT_j0_perf_ds1_L1J75'                : {  1000 : TriggerRule(PS=-1, comment='Primary, DS (Disabled until ATR-13922 is solved)'), 
                                                7001 : TriggerRule(PS=-1, comment='Disabled') },

    'HLT_j0_1i2c200m8000TLA' : {  1000 : TriggerRule(rate=1, comment='DS TLA unprescaled, ATR-14081'), },
    'HLT_j0_1i2c300m500TLA': {  1000 : TriggerRule(rate=1, comment='DS TLA at 1Hz'), },
    'HLT_j0_0i1c500m900TLA' : {  1000 : TriggerRule(rate=1, comment='DS TLA unprescaled, ATR-14081'), },

    'HLT_j0_0i1c500m2000TLA_delayed' : {  1000 : TriggerRule(PS=-1, comment='Disabled for 2017 DS'), },
    'HLT_j0_0i1c500m900TLA_delayed'  : {  1000 : TriggerRule(PS=-1, comment='Disabled for 2017 DS TLA Disabled'), },
    'HLT_j0_1i2c200m8000TLA_delayed' : {  1000 : TriggerRule(PS=-1, comment='Disabled for 2017 DS TLA EndOfFill until tighted more'),
                                          },

# expected rates (ATR-17883): J50 ==> 33 kHz at 2E34
#    PS=4 at 1.8E34 ==> 7.4 kHz
#    PS=3 at 1.5E34 ==> 8.2 kHz
#    P2=2 at 1.2E34 ==> 10 kHz
    'HLT_j0_perf_ds1_L1J50'                :
    {  100 : TriggerRule(PS=1, comment='DS, ATR-17198)'), 
     10001 : TriggerRule(PS=2, comment='DS, ATR-17198'),
     12001 : TriggerRule(PS=2, comment='DS, ATR-17198'),
     15001 : TriggerRule(PS=3, comment='DS, ATR-17198'),
     18001 : TriggerRule(PS=4, comment='DS, ATR-17198'),
       }, 
    }


#################################################################################################
#
#
# MinBias
#
#
#################################################################################################

#Prepare for ES rules
MinBias_Rules=RulebookDict(tag,'MinBias_Rules')
HLT_list=[ 'HLT_mb_sptrk','HLT_noalg_mb_L1MBTS_2','HLT_mb_sp1800_hmtperf' ,'HLT_mb_idperf_L1MBTS_2']
MinBias_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled mb',ESRate=0), 
    }])))


#################################################################################################
#
#
# monitoring
#
#
#################################################################################################

# import Monitoring_pp_v7_rules

zerobias_Rules = {
    # ATR-9878
    'HLT_noalg_zb_L1ZB'        : { 1 : TriggerRule(rate=7, comment='7 Hz target at 1.2e34') },
    'HLT_j40_L1ZB'             : { 1 : TriggerRule(rate=3, comment='3 Hz target at 1.2e34, HLT acceptance is 3%') },
    }

tauoverlay_Rules = { 
    'L1_2MU20_OVERLAY'             : { 1 : TriggerRule(rate=180, reoptimize=False, comment='TauOverlay, ATR-10801') },
    'HLT_noalg_to_L12MU20_OVERLAY' : { 1 : TriggerRule(rate=0.1, comment='TauOverlay') },
    'HLT_2mu20_L12MU20_OVERLAY'    : { 1 : TriggerRule(rate=5, comment='TauOverlay, ATR-10801') },  
    }

# grep sig_nam costMonitoring_20.11.0.2-ATR13473-13TeV-L1.0e34-all-RB00-04-20_280500/xml/TrigRate_20.11.0.2-ATR13473-13TeV-L1.0e34-all-RB00-04-20_HLT_SMK_294_L1_91_HLT_227.xml | grep "-" | sort | sed "s/<sig_name>/'/g"  | sed "s,</sig_name>,AAA,g" | sed "s/AAA/',/g" | awk '{pass # printf("%s ",$1)}'

#L1Topo item for commisioning
topo_items=['L1_J4-MATCH',        
            'L1_MJJ-100',        
            'L1_MJJ-400',        
            'L1_MJJ-800',        
            'L1_HT150-J20.ETA31',  
            'L1_HT150-J20s5.ETA31',
            'L1_W-HT20-JJ15.ETA49'
            ]

topo_Rules=RulebookDict(tag,'topo_Rules')
topo_Rules.update(dict(map(None,topo_items,len(topo_items)*[{
    1000 : TriggerRule(PS=-1, comment='Topo'),
    }])))

sometopo_items=['L1_HT190-J15.ETA21',
                'L1_HT190-J15s5.ETA21'
                ]
topo_Rules.update(dict(map(None,sometopo_items,len(sometopo_items)*[{
    1000 : TriggerRule(PS=-1, comment='Topo'),
    }])))

hlt_topo_items=[ 'HLT_noalg_l1topo_L1J4-MATCH',
    'HLT_noalg_l1topo_L1MJJ-800',
    'HLT_noalg_l1topo_L1MJJ-400',
    'HLT_noalg_l1topo_L1MJJ-100',
    'HLT_noalg_l1topo_L1HT150-J20.ETA31',
    'HLT_noalg_l1topo_L1HT150-J20s5.ETA31',
    'HLT_noalg_l1topo_L1W-HT20-JJ15.ETA49',
    ]

topo_Rules.update(dict(map(None,hlt_topo_items,len(hlt_topo_items)*[{
                    1000 : TriggerRule(PS=1, comment='Support HLT Topo'),
                    }])))

hlt_topo_items=['HLT_noalg_l1topo_L1HT190-J15.ETA21', 'HLT_noalg_l1topo_L1HT190-J15s5.ETA21',
    ]
topo_Rules.update(dict(map(None,hlt_topo_items,len(hlt_topo_items)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support HLT Topo'),
                    }])))


topo_unPS_items=['L1_DR-TAU20ITAU12I-J25']  #2.8kHz
topo_Rules.update(dict(map(None,topo_unPS_items,len(topo_unPS_items)*[{
    1000 : TriggerRule(PS=-1, comment='Topo'),
    }])))

topo_unPS_items=['L1_DR-TAU20ITAU12I',]  #4.2kHz
topo_Rules.update(dict(map(None,topo_unPS_items,len(topo_unPS_items)*[{
    1000 : TriggerRule(PS=-1, comment='Topo'),
    }])))
    
hlt_topo_tau_rules = {
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_notautsf_L1DR-TAU20ITAU12I' : { 1: TriggerRule(rate=0.1, comment='L1Topo Support')},
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_notautsf_L1DR-TAU20ITAU12I-J25' : { 1: TriggerRule(rate=0.1, comment='L1Topo Support')},
    }

l1_topo_dphijxe_rules = {
      'L1_J40_DPHI-J20s2XE50'   : { 1: TriggerRule(PS=1, comment='L1Topo DPHI(J,XE), EndOfFill, ATR-14351') ,  7001 : TriggerRule(PS=1, comment='L1Topo')},
      }
hlt_topo_dphijxe_rules = {
     'HLT_j100_xe110_mht_1dphi10_L1J40_DPHI-J20s2XE50'  : { 1: TriggerRule(PS=1, comment='L1Topo DPHI(J,XE), EndOfFill, ATR-14351') ,
                                                            #7001 : TriggerRule(PS=-1, comment='L1Topo')
     },
    }


l1_topo_bphys_rules = {
#  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6'      : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  Primary2017Oct1p7E34 : TriggerRule(PS=-1,  comment='Disabled if 2MU6 is disabled')},
    
  'L1_2MU4-B'                            : { 1: TriggerRule(PS=10, comment='L1Topo Bphys, EndOfFill, ATR-14108') , 7001 : TriggerRule(rate=0.5, comment='L1Topo DY')},
  'L1_MU6MU4-BO'                        : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  9001 : TriggerRule(rate=0.5, comment='L1Topo DY')},
}



hlt_topo_bphys_rules = {
    'HLT_2mu4_bJpsimumu_L1BPH-2M8-2MU4'                        : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    
    'HLT_mu6_mu4_bBmumu_L12MU4-B'                              : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bJpsimumu_L12MU4-B'                          : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bBmumu_L1MU6MU4-BO'                          : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bJpsimumu_L1MU6MU4-BO'                        : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },

    'HLT_mu6_mu4_bBmumux_BsmumuPhi':
    { 1: TriggerRule(PS=1, comment='Primary')      
      },
    
    'HLT_2mu4_bBmumu_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 22Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_2mu4_bBmumu_Lxy0_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 10Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_mu6_mu4_bBmumu_Lxy0_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': {
        1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108'),
     4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 7Hz at 1e34 PS=1'),
     7001 : TriggerRule(PS=1, comment='L1Topo Bphys'),
    12001 : TriggerRule(PS=2, comment='L1Topo Bphys, ATR-15503'),
        },

    'HLT_mu6_mu4_bJpsimumu_Lxy0_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': {
        1 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108'),
     4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 55Hz at 1e34 PS=1'),
     7001 : TriggerRule(PS=5, comment='L1Topo Bphys, ATR-15503'),
    10001 : TriggerRule(PS=10, comment='L1Topo Bphys, ATR-15503'),
    15001 : TriggerRule(PS=20, comment='L1Topo Bphys, ATR-15503'),
    },

    }

l1_topo_lfv_items=['L1_LFV-MU','L1_LFV-EM8I','L1_LFV-EM15I',]
hlt_topo_lfv_items = [
#'HLT_g10_loose_mu10_iloose_taumass_L1LFV-EM8I', #5.8Hz #L1 830Hz at 1e34
#'HLT_g20_loose_mu4_iloose_taumass_L1LFV-EM15I', #5.5Hz #L1 430Hz
'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU', #3Hz L1 2.7kHz
#'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU_bTau', #0.3Hz
#'HLT_mu11_nomucomb_mu6noL1_nscan03_L1LFV-MU_bTau_delayed', #17Hz # unique ~1Hz
]

l1_topo_dy_rules = {
  'L1_DY-BOX-2MU6'  : { 1: TriggerRule(PS=20, comment='L1Topo DY, ATR-14108') ,  7001 : TriggerRule(rate=0.5, comment='L1Topo DY') },
}
  
hlt_topo_dy_rules = {
  'HLT_mu6_iloose_mu6_11invm24_noos_L1DY-BOX-2MU6' : { 1: TriggerRule(PS=1, comment='L1Topo DY, EndOfFill, ATR-14108') ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
  'HLT_mu6_iloose_mu6_24invm60_noos_L1DY-BOX-2MU6' : { 1: TriggerRule(PS=1, comment='L1Topo DY, EndOfFill, ATR-14108') ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
  'HLT_mu6_iloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6' : { 1: TriggerRule(PS=5, comment='L1Topo DY, EndOfFill, ATR-14108') ,  7001 : TriggerRule(PS=-1, comment='Disabled') }, 
  'HLT_mu6_iloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6' : { 1: TriggerRule(PS=5, comment='L1Topo DY, EndOfFill, ATR-14108') ,  7001 : TriggerRule(PS=-1, comment='Disabled') }, 
}

topo_Rules.update(dict(map(None,hlt_topo_items,len(hlt_topo_items)*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))

topo_Rules.update(dict(map(None,hlt_topo_tau_rules.keys(),len(hlt_topo_tau_rules.keys())*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))

topo_Rules.update(dict(map(None,l1_topo_dphijxe_rules.keys(),len(l1_topo_dphijxe_rules.keys())*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))
topo_Rules.update(dict(map(None,hlt_topo_dphijxe_rules.keys(),len(hlt_topo_dphijxe_rules.keys())*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))

topo_Rules.update(dict(map(None,l1_topo_bphys_rules.keys(),len(l1_topo_bphys_rules.keys())*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))
topo_Rules.update(dict(map(None,hlt_topo_bphys_rules.keys(),len(hlt_topo_bphys_rules.keys())*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))

topo_Rules.update(dict(map(None,l1_topo_lfv_items,len(l1_topo_lfv_items)*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))
topo_Rules.update(dict(map(None,hlt_topo_lfv_items,len(hlt_topo_lfv_items)*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))

topo_Rules.update(dict(map(None,l1_topo_dy_rules.keys(),len(l1_topo_dy_rules.keys())*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))
topo_Rules.update(dict(map(None,hlt_topo_dy_rules.keys(),len(hlt_topo_dy_rules.keys())*[{
                1000 : TriggerRule(PS=-1, comment='disabled'),
                }])))

disabled_topo_list=[
 'L1_3J15_BTAG-MU4J15', 
 'L1_3J15_BTAG-MU4J30', 'L1_3J15_BTAG-MU6J25', 'L1_3J20_4J20.0ETA49_MJJ-400', 'L1_3J20_4J20.0ETA49_MJJ-700', 'L1_3J20_4J20.0ETA49_MJJ-800', 'L1_3J20_4J20.0ETA49_MJJ-900', 'L1_3J20_BTAG-MU4J20','L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4', 'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4', 'L1_BPH-2M8-2MU4-BO_BPH-0DR15-2MU4', 'L1_BPH-2M8-2MU4-B_BPH-0DR15-2MU4', 'L1_BPH-2M8-2MU4_BPH-0DR15-2MU4',  'L1_BPH-7M15-2MU4_BPH-0DR24-2MU4-B', 'L1_BPH-7M15-2MU4_BPH-0DR24-2MU4-BO', 'L1_BPH-7M15-2MU4_BPH-0DR24-2MU4', 'L1_BTAG-MU4J20', 'L1_DPHI-AJ20s2XE50', 'L1_DPHI-CJ20XE50', 'L1_DPHI-J20XE50', 'L1_DPHI-J20s2XE50', 'L1_DPHI-Js2XE50', 'L1_DR-EM15TAU12I-J25', 'L1_DR-EM15TAU12I', 'L1_DR-MU10TAU12I', 'L1_EM10VH_W-MT35_XS50', 'L1_EM12_W-MT25', 'L1_EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE', 'L1_EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_W-90RO2-XEHT-0', 'L1_EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20', 'L1_EM12_W-MT30', 'L1_EM12_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EMXE', 'L1_EM12_W-MT35_W-90RO2-XEHT-0_W-15DPHI-JXE-0_W-15DPHI-EMXE', 'L1_EM12_W-MT35_XS30_W-15DPHI-JXE-0_W-15DPHI-EMXE', 'L1_EM12_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EMXE', 'L1_EM12_W-MT35_XS50', 'L1_EM15-TAU12I', 'L1_EM15-TAU40', 'L1_EM15TAU12I-J25', 'L1_EM15VH_W-MT35_XS60', 'L1_EM15_W-MT35', 'L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_W-250RO2-XEHT-0_W-15DPHI-JXE-0_W-15DPHI-EM15XE', 'L1_EM15_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_XS40_W-15DPHI-JXE-0_W-15DPHI-EM15XE', 'L1_EM15_W-MT35_XS60', 'L1_EM15_W-MT35_XS60_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_XS60_W-15DPHI-JXE-0_W-15DPHI-EM15XE', 'L1_EM20VH_W-MT35_XS60', 'L1_EM22VHI_W-MT35_XS40',  'L1_HT150-JJ15.ETA49', 'L1_J25_2J20_3J12_BOX-TAU20ITAU12I', 'L1_J30_2J20_4J20.0ETA49_MJJ-400', 'L1_J30_2J20_4J20.0ETA49_MJJ-700', 'L1_J30_2J20_4J20.0ETA49_MJJ-800', 'L1_J30_2J20_4J20.0ETA49_MJJ-900',  'L1_J40_DPHI-CJ20XE50', 'L1_J40_DPHI-J20XE50', 'L1_J40_DPHI-Js2XE50', 'L1_JPSI-1M5-EM12', 'L1_JPSI-1M5-EM7', 'L1_JPSI-1M5', 'L1_KF-XE35', 'L1_KF-XE45', 'L1_KF-XE55', 'L1_KF-XE60', 'L1_KF-XE65', 'L1_KF-XE75', 'L1_MJJ-200', 'L1_MJJ-300',  'L1_MU10_TAU12I-J25', 'L1_MU6_MJJ-200', 'L1_MU6_MJJ-300', 'L1_MU6_MJJ-400', 'L1_MU6_MJJ-500', 'L1_TAU12I-J25', 'L1_TAU20-J20', 'L1_TAU20ITAU12I-J25', 'L1_W-05DPHI-EMXE-1', 'L1_W-05DPHI-JXE-0', 'L1_W-05RO-XEHT-0', 'L1_W-10DPHI-EMXE-0', 'L1_W-10DPHI-JXE-0', 'L1_W-15DPHI-EMXE-0', 'L1_W-15DPHI-JXE-0', 'L1_W-250RO2-XEHT-0', 'L1_W-90RO2-XEHT-0', 'L1_W-NOMATCH', 'L1_W-NOMATCH_W-05RO-XEEMHT', 'L1_XE35_EM15-TAU12I', 'L1_XE35_MJJ-200', 'L1_XE40_EM15-TAU12I', 'L1_XE45_TAU20-J20', 'L1_CMU4',
 'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4',
 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',
 'L1_EM18VHI_MJJ-300',
 'L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',
 'L1_LLP-NOMATCH',
 'L1_HT150-JJ15.ETA49_MJJ-400'
 'L1_SC85-CJ15', 'L1_MJJ-400-CF', 'L1_30M-EM20ITAU12',
 'L1_LATE-MU10_XE40',
 'L1_LATE-MU10_XE50',
 'L1_LATE-MU10_J50',
 'L1_TAU60_DR-TAU20ITAU12I',
 'L1_LFV-MU6',
 'L1_SC85-CJ15',
 'L1_HT150-JJ15.ETA49_MJJ-400',
 'L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', 
 'L1_BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4'
    ]

topo_Rules.update(dict(map(None,disabled_topo_list,len(disabled_topo_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled Topo'),
                        }])))

disabled_topo_list=[
   'L1_LLP-RO','L1_LFV-MU','L1_LFV-EM8I','L1_LFV-EM15I',
   'L1_BPH-2M8-2MU4','L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',
   'L1_2MU4-B','L1_2MU4-BO','L1_MU6_2MU4-B','L1_MU6MU4-BO','L1_2MU6-B','L1_2MU6-BO',
   'L1_DY-BOX-2MU6','L1_DY-BOX-MU6MU4','L1_DY-DR-2MU4',
   'L1_BTAG-MU4J15','L1_BTAG-MU4J30','L1_BTAG-MU6J20','L1_BTAG-MU6J25','L1_DY-BOX-2MU4',       
    ]

topo_Rules.update(dict(map(None,disabled_topo_list,len(disabled_topo_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled Topo'),
                        }])),allowDuplicates=True)

HLT_LLP_list = [
    'HLT_j30_jes_cleanLLP_PS_llp_L1LLP-RO', 'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1LLP-RO',
    'HLT_j30_jes_cleanLLP_PS_llp_L1LLP-NOMATCH', 'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1LLP-NOMATCH',
    ]
topo_Rules.update(dict(map(None,HLT_LLP_list,len(HLT_LLP_list)*[{
                    1 : TriggerRule(PS=1, comment='Primary, ATR-14428, ATR-15236'),
                    }])))

HLT_LLP_list_noTopo = [
    'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU100',
    'HLT_j30_jes_cleanLLP_PS_llp_L1TAU100',
    ]
topo_Rules.update(dict(map(None,HLT_LLP_list_noTopo,len(HLT_LLP_list_noTopo)*[{
                    1 : TriggerRule(PS=1, comment='Primary, ATR-14428, ATR-15236'),
                    }])))

HLT_LLP_list = [
    'HLT_mu10_mgonly_L1LATE-MU10_J50',
    'HLT_mu10_mgonly_L1LATE-MU10_XE40',
    'HLT_mu10_mgonly_L1LATE-MU10_XE50',
    ]
topo_Rules.update(dict(map(None,HLT_LLP_list,len(HLT_LLP_list)*[{
                    1 : TriggerRule(PS=1, comment='Primary, LATE MU, ATR-16594'),
                    }])))

L1Topo_list=[
    "L1_DR-TAU20ITAU12I-J25",
    "L1_DR-MU10TAU12I_TAU12I-J25",
    "L1_DR-EM15TAU12I-J25",
    "L1_LLP-NOMATCH",
    "L1_LLP-RO",
    "L1_TAU60_DR-TAU20ITAU12I",
    "L1_HT190-J15s5.ETA21",
    "L1_2MU4_J20_XE30_DPHI-J20s2XE30",
    "L1_2MU4_J20_XE40_DPHI-J20s2XE30",
    "L1_MU4_J50_XE50_DPHI-J20s2XE30",
    "L1_MU4_J30_XE40_DPHI-J20s2XE30",
    "L1_J40_XE50_DPHI-J20s2XE50",
    "L1_HT150-J20s5.ETA31_MJJ-400-CF",
    "L1_BPH-2M9-2MU6_BPH-2DR15-2MU6",
    "L1_BPH-8M15-2MU6_BPH-0DR22-2MU6",
    "L1_LAR-J",
    "L1_LAR-EM",
    'L1_EM18VHI_MJJ-300',
    'L1_LATE-MU10_XE40',
    'L1_LATE-MU10_XE50',
    'L1_LATE-MU10_J50',
    ]
    
topo_Rules.update(dict(map(None,L1Topo_list,len(L1Topo_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1Topo'),
                        }])))

L1Topo_list=[
    'L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4',
    ]
topo_Rules.update(dict(map(None,L1Topo_list,len(L1Topo_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1Topo'),
                    endOfFillThreshold1 : TriggerRule(PS=-1, comment='Disable, ATR-16576'),
                    }])))

L1Topo_list=[
    'L1_BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4-BO',
    ]
topo_Rules.update(dict(map(None,L1Topo_list,len(L1Topo_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1Topo'),
                    endOfFillThreshold2 : TriggerRule(PS=-1, comment='Disable, ATR-16576'),
                    }])))

L1Topo_list=[
    'L1_LFV-MU6',
    ]
topo_Rules.update(dict(map(None,L1Topo_list,len(L1Topo_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1Topo'),
                    endOfFillThreshold2 : TriggerRule(PS=-1, comment='Disable, ATR-16576'),
                    }])))

L1Topo_list=[
    'L1_LFV-EM8I','L1_LFV-EM15I',         
    ]
topo_Rules.update(dict(map(None,L1Topo_list,len(L1Topo_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1Topo'),
                    15001 : TriggerRule(PS=-1, comment='Disable, ATR-16576'),
                    }])))
    
L1Topo_list=[
        'L1_DR-TAU20ITAU12I', 'L1_HT150-JJ15.ETA49', 'L1_MJJ-100', 'L1_HT150-JJ15.ETA49_MJJ-400', 'L1_MJJ-800', 'L1_KF-XE60', 'L1_KF-XE65', 'L1_DPHI-J20XE50', 'L1_DPHI-CJ20XE50', 'L1_J4-MATCH', 'L1_KF-XE55', 'L1_DY-DR-2MU4', 'L1_DY-BOX-2MU6', 'L1_EM15TAU12I-J25', 'L1_DPHI-J20s2XE50', 'L1_MJJ-400', 'L1_TAU20ITAU12I-J25', 'L1_DR-MU10TAU12I', 'L1_SC85-CJ15', 'L1_MJJ-400-CF', 'L1_30M-EM20ITAU12',      ]
topo_Rules.update(dict(map(None,L1Topo_list,len(L1Topo_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support L1Topo'),
                        }])))

hlt_jpsiee_list=[
    'HLT_e14_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM12',
    'HLT_e5_lhtight_nod0_e14_etcut_Jpsiee_L1JPSI-1M5-EM12',
    'HLT_e5_lhtight_nod0_e9_etcut_Jpsiee_L1JPSI-1M5-EM7', 
]
topo_Rules.update({
'L1_JPSI-1M5-EM7': {  1000 : TriggerRule(rate=1200, comment='Support L1'),
                      12001: TriggerRule(rate=60,   comment='Support L1')},
'L1_JPSI-1M5-EM12': { 1000 : TriggerRule(rate=2600, comment='Support L1'),
                      12001: TriggerRule(rate=130,  comment='Support L1')}  
 })
topo_Rules.update(dict(map(None,hlt_jpsiee_list,len(hlt_jpsiee_list)*[{
                1000 : TriggerRule(PS=1, comment='Support, Jpsi ee l1topo'),
                }])))

hlt_jpsiee_list=[
    'HLT_e9_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM7',
]
topo_Rules.update(dict(map(None,hlt_jpsiee_list,len(hlt_jpsiee_list)*[{
                1000 : TriggerRule(PS=1, comment='Support, Jpsi ee l1topo, Express ATR-17782', ESRate=0.3),
                }])))

l1calo_streamer_Rules = {
    'HLT_noalg_l1topo_L12EM3'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU6'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L14J15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L14J20.0ETA49'    : { 1000 : TriggerRule(rate=0.001, comment='Huge rate with L1Calo low-mu filter'),
                                            5001 : TriggerRule(rate=0.5, comment=''),
                                            },
    'HLT_noalg_l1topo_L1EM12_2EM3'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM15I_MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM3'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM7_2EM3'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM8I_MU10'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM7_MU10'    : { 1000 : TriggerRule(rate=0.5, comment='')}, 
    'HLT_noalg_l1topo_L1EM7'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1J100'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1J15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1J20'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1J25'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    #'HLT_noalg_l1calo_L1J400'    : { 1000 : TriggerRule(PS=1, comment='Went to Monitoring_pp_v7')},
    'HLT_noalg_l1topo_L1J30.0ETA49_2J20.0ETA49'    : { 1000 : TriggerRule(rate=0.001, comment='Huge rate with L1Calo low-mu filter'),
                                                       5001 : TriggerRule(rate=0.5, comment=''), 
                                                       },
    'HLT_noalg_l1topo_L1J40_XE50'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU10_2MU6'    : { 1000 : TriggerRule(PS=-1, comment='Disable')},
    'HLT_noalg_l1topo_L1MU10_TAU12IM'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU6_2MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU6_J20'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1TAU12IM'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1TAU20IM_2TAU12IM'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1TAU30'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1TAU40'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1XE35'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1XE45'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1XE55'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1XE60'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1XE70'    : { 1000 : TriggerRule(rate=0.5, comment='')},
}

l1topo_streamer_Rules = {
    'HLT_noalg_l1topo_L1DR-TAU20ITAU12I'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1HT150-JJ15.ETA49'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MJJ-100'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LFV-MU'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LFV-MU11'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1HT150-JJ15.ETA49_MJJ-400'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LFV-EM8I'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LFV-EM15I'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MJJ-800'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1KF-XE60'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1KF-XE65'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LLP-NOMATCH'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L130M-EM20ITAU12'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DPHI-J20XE50'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DPHI-CJ20XE50'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1TAU60_DR-TAU20ITAU12I'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1J4-MATCH'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LAR-EM'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1KF-XE55'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DR-TAU20ITAU12I-J25'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DR-MU10TAU12I_TAU12I-J25'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DY-DR-2MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DY-BOX-2MU6'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM15TAU12I-J25'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BPH-2M9-2MU6_BPH-2DR15-2MU6'    : { 1000 : TriggerRule(rate=0.5, comment='')},    
    'HLT_noalg_l1topo_L1LLP-RO'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LAR-J'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1SC111-CJ15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DPHI-J20s2XE50'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DR-EM15TAU12I-J25'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MJJ-400'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1TAU20ITAU12I-J25'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1SC85-CJ15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DR-MU10TAU12I'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MJJ-400-CF'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LATE-MU10_XE40'    : { 1000 : TriggerRule(PS=300, comment='Target rate 0.5Hz')},
    'HLT_noalg_l1topo_L1LATE-MU10_J50'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MJJ-500-NFF'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1J50_DETA20-J50J'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BPH-0M9-EM7-EM5'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BPH-0DR3-EM7J15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DPHI-2EM3'    : { 1000 : TriggerRule(rate=0.5, comment='')},
}


HLT_list=[
    'HLT_mu4_j35_boffperf_split_dr05_dz02_L1BTAG-MU4J15', 'HLT_mu6_j60_gsc85_boffperf_split_dr05_dz02_L1BTAG-MU6J20',
    ]
topo_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1 : TriggerRule(rate=1, comment="bperf L1Topo", rerun=1), 
                    }])))

#################################################################################################
#
#
# disable rules
#
#
#################################################################################################

disabled_Rules=RulebookDict(tag,'disabled_Rules')

# --------------------------------------
# signatures, EB

#grep "has no rules and will be disabled" a.log | awk -F"--> " '{pass # print $2}' | sed "s/ has no rules and will be disabled//g" | sort | awk '{pass # printf("\"%s\", ",$1)}'

HLT_list=[
    "HLT_2e17_lhvloose_nod0_noringer_L12EM15VHI", "HLT_2e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50", "HLT_2e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1XE60", "HLT_2e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50", "HLT_2e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1XE60", "HLT_2e5_lhmedium_nod0_j40_xe90_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50", "HLT_2e5_lhmedium_nod0_j40_xe90_pufit_2dphi10_L1XE60", "HLT_2j10_deta20_L1MBTS_2", "HLT_2j15_gsc35_bmv2c1060_split_2j15_gsc35_bmv2c1085_split_L14J15.0ETA25", "HLT_2j15_gsc35_bmv2c1077_split_2j15_gsc35_bmv2c1085_split_L14J15.0ETA25", "HLT_2j35_gsc45_bmv2c1050_split_3j35_gsc45_boffperf_split", "HLT_2j35_gsc45_bmv2c1050_split_3j35_gsc45_boffperf_split_L14J15.0ETA25", "HLT_2j35_gsc45_bmv2c1060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400", "HLT_2j35_gsc45_bmv2c1060_split_3j35_gsc45_boffperf_split_L14J15.0ETA25", "HLT_2j35_gsc45_bmv2c1070_split_3j35_gsc45_boffperf_split_L14J15.0ETA25", "HLT_2j330_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21", "HLT_2j330_a10t_lcw_jes_30smcINF_L1SC111", "HLT_2j330_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21", "HLT_2j340_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21", "HLT_2j340_a10t_lcw_jes_30smcINF_L1J100", "HLT_2j340_a10t_lcw_jes_30smcINF_L1SC111", "HLT_2j340_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21", "HLT_2j340_a10t_lcw_jes_L1HT190-J15.ETA21", "HLT_2j340_a10t_lcw_jes_L1J100", "HLT_2j340_a10t_lcw_jes_L1SC111", "HLT_2j350_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21", "HLT_2j350_a10t_lcw_jes_30smcINF_L1J100", "HLT_2j350_a10t_lcw_jes_30smcINF_L1SC111", "HLT_2j350_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21","HLT_2j350_a10t_lcw_jes_L1HT190-J15.ETA21", "HLT_2j350_a10t_lcw_jes_L1J100", "HLT_2j350_a10t_lcw_jes_L1SC111", "HLT_2j45_gsc55_bmv2c1040_split_ht300_L1HT190-J15.ETA21", "HLT_2j45_gsc55_bmv2c1050_split_ht300_L1HT190-J15.ETA21", "HLT_2j45_gsc55_bmv2c1060_split_ht300_L1HT190-J15.ETA21", "HLT_2j45_gsc55_bmv2c1070_split_ht300_L1HT190-J15.ETA21", "HLT_2j45_gsc55_bmv2c1070_split_ht300_L1HT190-J15s5.ETA21", "HLT_2j45_gsc55_boffperf_split_ht300_L1HT190-J15.ETA21", "HLT_2j55_boffperf_split_ht300_L1HT190-J15.ETA21", "HLT_2j55_boffperf_split_ht300_L1HT190-J15s5.ETA21", "HLT_2mu10_bDimu", "HLT_2mu10_bDimu_novtx_noos", "HLT_2mu4_bBmumu_L12MU4-B", "HLT_2mu4_bBmumu_L12MU4-BO", "HLT_2mu4_bBmumux_BcmumuDsloose_L12MU4-B", "HLT_2mu4_bBmumux_BcmumuDsloose_L12MU4-BO", "HLT_2mu4_bBmumuxv2_L12MU4-B", "HLT_2mu4_bBmumuxv2_L12MU4-BO", "HLT_2mu4_bBmumuxv2_L1BPH-2M9-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bDimu", "HLT_2mu4_bDimu_L12MU4-B", "HLT_2mu4_bDimu_L12MU4-BO", "HLT_2mu4_bDimu_noinvm_novtx_ss", "HLT_2mu4_bDimu_novtx_noos", "HLT_2mu4_bDimu_novtx_noos_L12MU4-B", "HLT_2mu4_bDimu_novtx_noos_L12MU4-BO", "HLT_2mu4_bJpsimumu_L12MU4-B", "HLT_2mu4_bJpsimumu_L12MU4-BO", "HLT_2mu4_bUpsimumu_L12MU4-B", "HLT_2mu4_bUpsimumu_L12MU4-BO", "HLT_2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE40_DPHI-J20s2XE30", "HLT_2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE50_DPHI-J20s2XE30", "HLT_2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30", "HLT_2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J20_XE40_DPHI-J20s2XE30", "HLT_2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J20_XE50_DPHI-J20s2XE30", "HLT_2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J40_XE50", "HLT_2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_XE60", "HLT_2mu6_bBmumu_L12MU6-B", "HLT_2mu6_bBmumux_BcmumuDsloose_L12MU6-B", "HLT_2mu6_bBmumux_BcmumuDsloose_L12MU6-BO", "HLT_2mu6_bBmumux_Taumumux_noL2", "HLT_2mu6_bBmumuxv2_L12MU6-B", "HLT_2mu6_bBmumuxv2_L12MU6-BO", "HLT_2mu6_bDimu_novtx_noos_L12MU6-B", "HLT_2mu6_bDimu_novtx_noos_L12MU6-BO", "HLT_2mu6_bDimu_novtx_noos_L1BPH-8M15-2MU6", "HLT_2mu6_bJpsimumu_L12MU6-B", "HLT_2mu6_bUpsimumu_L12MU6-B", "HLT_3j35_bmv2c1060_split_j35_L14J15.0ETA25", "HLT_3j35_bmv2c1077_split_j35_L14J15.0ETA25", "HLT_3mu6_msonly_L1MU4_EMPTY", "HLT_4j15_gsc35_bmv2c1085_split_L14J15.0ETA25", "HLT_beamspot_activeTE_trkfast_pebTRT", "HLT_beamspot_activeTE_trkfast_peb_L13J15", "HLT_beamspot_activeTE_trkfast_peb_L1J15", "HLT_beamspot_allTE_trkfast_pebTRT", "HLT_beamspot_allTE_trkfast_peb_L13J15", "HLT_beamspot_allTE_trkfast_peb_L1J15", "HLT_beamspot_trkFS_trkfast_L13J15", "HLT_beamspot_trkFS_trkfast_L1J15", "HLT_e0_perf_L1EM3_EMPTY", "HLT_e140_lhloose_nod0_noringer_L1EM24VHI", "HLT_e140_lhloose_nod0_noringer_L1EM24VHIM", "HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25", "HLT_e17_lhvloose_nod0_noringer_L1EM15VHI", "HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30", "HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE", "HLT_e25_mergedtight_g35_medium_icalotight_Heg", "HLT_e25_mergedtight_g35_medium_icalovloose_Heg", "HLT_e25_mergedtight_ivarloose_g35_medium_icalotight_Heg", "HLT_e26_lhloose_nod0", "HLT_e26_lhmedium_nod0_icalomedium", "HLT_e26_lhmedium_nod0_icalotight", "HLT_e26_lhmedium_nod0_ivarloose", "HLT_e26_lhmedium_nod0_ivarloose_L1EM22VHIM", "HLT_e26_lhmedium_nod0_ivarmedium", "HLT_e26_lhmedium_nod0_ivarmedium_L1EM22VHIM", "HLT_e26_lhmedium_nod0_ivarmedium_icalomedium", "HLT_e26_lhmedium_nod0_ivartight", "HLT_e26_lhmedium_nod0_ivartight_L1EM22VHIM", "HLT_e26_lhmedium_nod0_ivartight_icalotight", "HLT_e26_lhmedium_nod0_ivartight_icalotight_L1EM22VHIM", "HLT_e26_lhtight_nod0", "HLT_e26_lhtight_nod0_L1EM22VHIM", "HLT_e28_lhmedium_nod0_icaloloose", "HLT_e28_lhmedium_nod0_icalomedium", "HLT_e28_lhmedium_nod0_icalotight", "HLT_e28_lhmedium_nod0_ivarloose", "HLT_e28_lhmedium_nod0_ivarloose_L1EM24VHIM", "HLT_e28_lhmedium_nod0_ivarloose_icaloloose", "HLT_e28_lhmedium_nod0_ivarmedium", "HLT_e28_lhmedium_nod0_ivarmedium_L1EM24VHIM", "HLT_e28_lhmedium_nod0_ivarmedium_icalomedium", "HLT_e28_lhmedium_nod0_ivartight", "HLT_e28_lhmedium_nod0_ivartight_L1EM24VHIM", "HLT_e28_lhmedium_nod0_ivartight_icalotight", "HLT_e28_lhmedium_nod0_ivartight_icalotight_L1EM24VHIM", "HLT_e28_lhtight_nod0", "HLT_e28_lhtight_nod0_L1EM22VHI", "HLT_e28_lhtight_nod0_L1EM24VHIM", "HLT_e28_lhvloose_nod0", "HLT_e28_lhvloose_nod0_L1EM24VHIM", "HLT_e30_mergedtight_g35_medium_icalotight_Heg", "HLT_e30_mergedtight_g35_medium_icalovloose_Heg", "HLT_e30_mergedtight_ivarloose_g35_medium_icalotight_Heg", "HLT_e30_mergedtight_ivarloose_g35_medium_icalovloose_Heg", "HLT_e50_etcut_trkcut_L1EM24VHIM_j15_perf_xe50_6dphi15_mt35", "HLT_e50_etcut_trkcut_L1EM24VHIM_xe50_mt35", "HLT_e50_etcut_trkcut_L1EM24VHIM_xs30_j15_perf_xe30_6dphi15_mt35", "HLT_e50_etcut_trkcut_L1EM24VHIM_xs30_xe30_mt35", "HLT_e50_etcut_trkcut_j15_perf_xe50_6dphi05_mt35", "HLT_e50_etcut_trkcut_j15_perf_xe50_6dphi15_mt35", "HLT_e50_etcut_trkcut_xe50_mt35", "HLT_e50_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35", "HLT_e50_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35", "HLT_e50_etcut_trkcut_xs30_xe30_mt35", "HLT_e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1XE60", "HLT_e5_lhmedium_nod0_j40_xe90_pufit_2dphi10_L1XE60", "HLT_e5_lhmedium_nod0_j50_xe90_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50", "HLT_e5_lhmedium_nod0_mu4_j30_xe50_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30", "HLT_e5_lhmedium_nod0_mu4_j30_xe50_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30", "HLT_e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30", "HLT_e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30", "HLT_e5_lhvloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50", "HLT_e5_lhvloose_nod0_j50_xe70_pufit_2dphi10_L1XE60", "HLT_e60_etcut_trkcut_j15_perf_xe60_6dphi15_mt35", "HLT_e60_etcut_trkcut_xe60_mt35", "HLT_e60_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35", "HLT_e60_etcut_trkcut_xs30_xe30_mt35", "HLT_e60_lhmedium_nod0_noringer_L1EM24VHI", "HLT_e60_lhmedium_nod0_noringer_L1EM24VHIM", "HLT_eb_high_L1RD2_FILLED", "HLT_eb_low_L1RD2_FILLED", "HLT_g0_perf_L1EM3_EMPTY", "HLT_g10_loose_L1EM3", "HLT_g15_loose_2mu10_msonly_L1MU4_EMPTY", "HLT_g25_medium_tau25_dikaonmass_tracktwo_50mVis10000_L130M-EM20ITAU12", "HLT_g25_medium_tau25_dipion1loose_tracktwo_50mVis10000_L130M-EM20ITAU12", "HLT_g25_medium_tau25_dipion2_tracktwo_50mVis10000_L130M-EM20ITAU12", "HLT_g25_medium_tau25_kaonpi1_tracktwo_50mVis10000_L130M-EM20ITAU12", "HLT_g25_medium_tau25_kaonpi2_tracktwo_50mVis10000_L130M-EM20ITAU12", "HLT_g25_medium_tau25_singlepion_tracktwo_50mVis10000_L130M-EM20ITAU12", "HLT_ht0_L1J12_EMPTY", "HLT_j0_L1J12_EMPTY", "HLT_j0_perf_boffperf_L1J12_EMPTY", "HLT_j0_perf_boffperf_L1MU10", "HLT_j0_perf_boffperf_L1RD0_EMPTY", "HLT_j10_320eta490_L1MBTS_2", "HLT_j10_L1MBTS_2", "HLT_j175_gsc225_bmv2c1050_split", "HLT_j225_gsc275_bmv2c1077_split", "HLT_j320_a10t_lcw_jes_30smcINF_j250_a10t_lcw_jes_30smcINF_L1J100", "HLT_j320_a10t_lcw_jes_35smcINF_j250_a10t_lcw_jes_35smcINF_L1J100", "HLT_j320_a10t_lcw_jes_40smcINF_j250_a10t_lcw_jes_40smcINF_L1J100", "HLT_j350_a10t_lcw_jes_30smcINF_j300_a10t_lcw_jes_30smcINF_L1J100", "HLT_j350_a10t_lcw_jes_35smcINF_j300_a10t_lcw_jes_35smcINF_L1J100", "HLT_j350_a10t_lcw_jes_40smcINF_j300_a10t_lcw_jes_40smcINF_L1J100", "HLT_j45_gsc55_bmv2c1040_split_ht500_L1HT190-J15.ETA21", "HLT_j45_gsc55_bmv2c1050_split_ht500_L1HT190-J15.ETA21", "HLT_j45_gsc55_bmv2c1050_split_ht500_L1HT190-J15s5.ETA21", "HLT_j45_gsc55_bmv2c1050_split_ht700_L1HT190-J15.ETA21", "HLT_j45_gsc55_bmv2c1060_split_ht500_L1HT190-J15s5.ETA21", "HLT_j45_gsc55_bmv2c1060_split_ht700_L1HT190-J15.ETA21", "HLT_j45_gsc55_bmv2c1060_split_ht700_L1HT190-J15s5.ETA21", "HLT_j45_gsc55_boffperf_split_ht500_L1HT190-J15.ETA21", "HLT_j45_gsc55_boffperf_split_ht500_L1HT190-J15s5.ETA21", "HLT_j375_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21", "HLT_j375_a10t_lcw_jes_30smcINF_L1J100", "HLT_j375_a10t_lcw_jes_30smcINF_L1SC111", "HLT_j375_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21", "HLT_j375_a10t_lcw_jes_35smcINF_L1J100", "HLT_j375_a10t_lcw_jes_35smcINF_L1SC111", "HLT_j375_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21", "HLT_j375_a10t_lcw_jes_40smcINF_L1J100", "HLT_j375_a10t_lcw_jes_40smcINF_L1SC111", "HLT_j375_a10t_lcw_jes_L1HT190-J15.ETA21", "HLT_j375_a10t_lcw_jes_L1J100", "HLT_j375_a10t_lcw_jes_L1SC111", "HLT_j390_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21", "HLT_j390_a10t_lcw_jes_30smcINF_L1SC111", "HLT_j390_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21", "HLT_j390_a10t_lcw_jes_35smcINF_L1SC111", "HLT_j390_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21", "HLT_j390_a10t_lcw_jes_40smcINF_L1SC111", "HLT_j400_a10t_lcw_jes_30smcINF_j300_a10t_lcw_jes_30smcINF_L1J100", "HLT_j400_a10t_lcw_jes_35smcINF_j300_a10t_lcw_jes_35smcINF_L1J100", "HLT_j400_a10t_lcw_jes_40smcINF_j300_a10t_lcw_jes_40smcINF_L1J100", "HLT_j420_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21", "HLT_j420_a10t_lcw_jes_30smcINF_L1SC111", "HLT_j420_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21", "HLT_j420_a10t_lcw_jes_35smcINF_L1SC111", "HLT_j420_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21", "HLT_j420_a10t_lcw_jes_40smcINF_L1SC111", "HLT_j420_a10t_lcw_jes_L1HT190-J15.ETA21", "HLT_j420_a10t_lcw_jes_L1SC111", "HLT_j440_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21", "HLT_j440_a10t_lcw_jes_30smcINF_L1SC111", "HLT_j440_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21", "HLT_j440_a10t_lcw_jes_35smcINF_L1SC111", "HLT_j50_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50", "HLT_j55_gsc75_bmv2c1040_split_ht500_L1HT190-J15.ETA21", "HLT_j55_gsc75_bmv2c1050_split_3j55_gsc75_boffperf_split", "HLT_j55_gsc75_bmv2c1050_split_ht500_L1HT190-J15.ETA21", "HLT_j60_gsc85_bmv2c1060_split_3j60_gsc85_boffperf_split", "HLT_j70_xe100_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50", "HLT_lumipeb_L1MBTS_2_LUCID", "HLT_lumipeb_vdm_L1MBTS_2_LUCID", "HLT_lumipeb_vdm_L1MBTS_2_LUCID_BGRP11", "HLT_lumipeb_vdm_L1MBTS_2_LUCID_BGRP9", "HLT_lumipeb_vdm_L1MBTS_2_LUCID_UNPAIRED_ISO", "HLT_lumipeb_vdm_L1RD2_BGRP12", "HLT_mu10_L1MU10", "HLT_mu10_mu6_bBmumux_BcmumuDsloose_L1LFV-MU", "HLT_mu10_mu6_bBmumux_BsmumuPhi", "HLT_mu10_mucombTag_noEF_L1MU40", "HLT_mu11_L1MU10", "HLT_mu14_L1MU10", "HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I", "HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25", "HLT_mu15_mucombTag_noEF_L1MU40", "HLT_mu20_mucombTag_noEF_L1MU40", "HLT_mu25_mucombTag_noEF_L1MU40", "HLT_mu32_ivarmedium", "HLT_mu4_3j45_dr05_L13J15_BTAG-MU4J15", "HLT_mu4_3j45_dr05_L13J20_BTAG-MU4J20", "HLT_mu4_cosmic_L1MU11_EMPTY", "HLT_mu4_cosmic_L1MU4_EMPTY", "HLT_mu4_j80_xe80_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30", "HLT_mu4_j80_xe80_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30", "HLT_mu4_j80_xe80_pufit_2dphi10_L1MU4_XE60", "HLT_mu4_msonly_cosmic_L1MU11_EMPTY", "HLT_mu4_msonly_cosmic_L1MU4_EMPTY", "HLT_mu50_mgonly_inTimeRoI", "HLT_mu60_mgonly_inTimeRoI", "HLT_mu6_L1MU6", "HLT_mu6_ivarloose_mu6_11invm24_noos_L1DY-BOX-2MU6", "HLT_mu6_ivarloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6", "HLT_mu6_ivarloose_mu6_24invm60_noos_L1DY-BOX-2MU6", "HLT_mu6_ivarloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_L12MU4-B", "HLT_mu6_mu4_bBmumuxv2_L12MU4-B", "HLT_mu6_mu4_bDimu_L12MU4-B", "HLT_mu6_mu4_bDimu_L1BPH-8M15-MU6MU4", "HLT_mu6_mu4_bDimu_novtx_noos_L12MU4-B", "HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-8M15-MU6MU4", "HLT_mu6_mu4_bUpsimumu_L12MU4-B", "HLT_mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4", "HLT_mu8_mucombTag_noEF_L1MU40", "HLT_noalg_L13MU4", "HLT_noalg_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4", "HLT_noalg_L1MU20MU21_FIRSTEMPTY", "HLT_noalg_L1MU20_FIRSTEMPTY", "HLT_noalg_L1MU21_FIRSTEMPTY", "HLT_noalg_L1MU6_2MU4", "HLT_noalg_L1TE60","HLT_noalg_bkg_L1MBTS_2_UNPAIRED_ISO", "HLT_noalg_l1calo_L1EM12", "HLT_noalg_l1calo_L1EM15", "HLT_noalg_l1calo_L1EM7", "HLT_noalg_l1calo_L1J100", "HLT_noalg_l1calo_L1J10031ETA49", "HLT_noalg_l1calo_L1J120", "HLT_noalg_l1calo_L1J5031ETA49", "HLT_noalg_l1calo_L1J75", "HLT_noalg_l1calo_L1J7531ETA49", "HLT_noalg_l1calo_L1TAU12", "HLT_noalg_l1calo_L1TAU20", "HLT_noalg_l1calo_L1TAU8", "HLT_rpcpeb_L1RD0_EMPTY", "HLT_xe100_mht_xe65_L1XE50", "HLT_xe100_mht_xe65_L1XE55", "HLT_xe100_mht_xe65_L1XE70", "HLT_xe100_mht_xe70_L1XE50", "HLT_xe100_mht_xe70_L1XE55", "HLT_xe100_mht_xe70_L1XE70", "HLT_xe100_mht_xe75_L1XE50", "HLT_xe100_mht_xe75_L1XE55", "HLT_xe100_mht_xe75_L1XE70", "HLT_xe100_mht_xe80_L1XE50", "HLT_xe100_mht_xe80_L1XE55", "HLT_xe100_mht_xe80_L1XE70", "HLT_xe110_mht_L1XE50", "HLT_xe110_mht_L1XE55", "HLT_xe110_mht_em_L1XE50", "HLT_xe110_mht_em_L1XE55", "HLT_xe110_mht_em_wEFMu_L1XE50", "HLT_xe110_mht_em_wEFMu_L1XE55", "HLT_xe110_mht_wEFMu_L1XE50", "HLT_xe110_mht_wEFMu_L1XE55", "HLT_xe110_mht_xe65_L1XE50", "HLT_xe110_mht_xe65_L1XE55", "HLT_xe110_mht_xe65_L1XE70", "HLT_xe110_mht_xe70_L1XE50", "HLT_xe110_mht_xe70_L1XE55", "HLT_xe110_mht_xe70_L1XE60","HLT_xe110_mht_xe70_L1XE70", "HLT_xe110_mht_xe75_L1XE50", "HLT_xe110_mht_xe75_L1XE55", "HLT_xe110_mht_xe75_L1XE60", "HLT_xe110_mht_xe75_L1XE70", "HLT_xe110_mht_xe80_L1XE50", "HLT_xe110_mht_xe80_L1XE55", "HLT_xe110_mht_xe80_L1XE70", "HLT_xe110_tc_em_L1XE50", "HLT_xe110_tc_em_L1XE55", "HLT_xe110_tc_em_wEFMu_L1XE50", "HLT_xe110_tc_em_wEFMu_L1XE55", "HLT_xe110_tc_lcw_L1XE50", "HLT_xe110_tc_lcw_L1XE55", "HLT_xe110_tc_lcw_wEFMu_L1XE50", "HLT_xe110_tc_lcw_wEFMu_L1XE55", "HLT_xe120_mht_L1XE50", "HLT_xe120_mht_L1XE55","HLT_xe120_mht_L1XE60","HLT_xe120_mht_L1XE70", "HLT_xe120_mht_em_L1XE50", "HLT_xe120_mht_em_L1XE55", "HLT_xe120_mht_em_L1XE60", "HLT_xe120_mht_em_L1XE70", "HLT_xe120_mht_xe65_L1XE50", "HLT_xe120_mht_xe65_L1XE55", "HLT_xe120_mht_xe65_L1XE60", "HLT_xe120_mht_xe70_L1XE50", "HLT_xe120_mht_xe70_L1XE55", "HLT_xe120_mht_xe70_L1XE60", "HLT_xe120_mht_xe75_L1XE50", "HLT_xe120_mht_xe75_L1XE55", "HLT_xe120_mht_xe75_L1XE60", "HLT_xe120_tc_em_L1XE50", "HLT_xe120_tc_em_L1XE55", "HLT_xe120_tc_lcw_L1XE50", "HLT_xe120_tc_lcw_L1XE55", "HLT_xe120_tc_lcw_L1XE60", "HLT_xe130_mht_L1XE55", "HLT_xe130_mht_em_L1XE50", "HLT_xe130_mht_em_L1XE55", "HLT_xe130_mht_em_L1XE60", "HLT_xe130_mht_xe65_L1XE55", "HLT_xe130_mht_xe65_L1XE60", "HLT_xe130_mht_xe70_L1XE55", "HLT_xe130_mht_xe70_L1XE60", "HLT_xe130_mht_xe75_L1XE55", "HLT_xe130_mht_xe75_L1XE60", "HLT_xe130_mht_xe80_L1XE60", "HLT_xe130_tc_em_L1XE50","HLT_xe130_tc_em_L1XE55","HLT_xe130_tc_em_L1XE60", "HLT_xe130_tc_lcw_L1XE50", "HLT_xe130_tc_lcw_L1XE55", "HLT_xe90_mht_xe65_L1XE50", "HLT_xe90_mht_xe65_L1XE55", "HLT_xe90_mht_xe70_L1XE50", "HLT_xe90_mht_xe70_L1XE55", "HLT_xe90_mht_xe75_L1XE50", "HLT_xe90_mht_xe75_L1XE55", "HLT_xe90_mht_xe80_L1XE50", "HLT_xe90_mht_xe80_L1XE55", "HLT_zdcpeb_L1ZDC_A", "HLT_zdcpeb_L1ZDC_AND", "HLT_zdcpeb_L1ZDC_A_C", "HLT_zdcpeb_L1ZDC_C", 'HLT_tau25_perf_track',
    "HLT_4j15_gsc35_bmv2c1085_split_L14J15.0ETA25",
    "HLT_4j35_bmv2c1085_split_L14J15.0ETA25",
    "HLT_2j35_bmv2c1077_split_2j35_bmv2c1085_split_L14J15.0ETA25",
    "HLT_2j35_bmv2c1060_split_2j35_bmv2c1085_split_L14J15.0ETA25",
    "HLT_3j35_bmv2c1077_split_j35_boffperf_split_L14J15.0ETA25",
    "HLT_3j35_bmv2c1060_split_j35_boffperf_split_L14J15.0ETA25",
    "HLT_mu60_msonly_3layersEC",
    "HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM_4J12",
    "HLT_j70_j50_0eta490_invm900j50_dphi26_xe90_pufit_L1MJJ-400-NFF",
    "HLT_j70_j50_0eta490_invm900j50_dphi24_L1MJJ-400-NFF",
    "HLT_j70_j50_0eta490_invm1000j50_dphi26_j30_bmv2c1077_split_L1MJJ-400-NFF",
    "HLT_j70_j50_0eta490_invm900j50_dphi26_L1MJJ-400-NFF",
    "HLT_j70_j50_0eta490_invm700j50_L1MJJ-400-NFF_AND_2j45_bmv2c1070_split",
    "HLT_e12_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_j70_j50_0eta490_invm900j50_L1MJJ-400-NFF",
    "HLT_g35_loose_j70_j50_0eta490_invm700j50_L1MJJ-400-NFF",
    "HLT_j70_j50_0eta490_invm900j50_dphi26_xe90_pufit_L1MJJ-500-NFF",
    "HLT_j70_j50_0eta490_invm900j50_dphi24_L1MJJ-500-NFF",
    "HLT_j70_j50_0eta490_invm1000j50_dphi26_j30_bmv2c1077_split_L1MJJ-500-NFF",
    "HLT_j70_j50_0eta490_invm900j50_dphi26_L1MJJ-500-NFF",
    "HLT_j70_j50_0eta490_invm900j50_L1MJJ-500-NFF_AND_2j45_bmv2c1070_split",
    "HLT_e12_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_j70_j50_0eta490_invm900j50_L1MJJ-500-NFF",
    "HLT_g35_loose_j70_j50_0eta490_invm900j50_L1MJJ-500-NFF",
    "HLT_mu8_ivarloose_tau25_medium1_tracktwo_2j40_0eta490_invm400j40_L1MU6_MJJ-200",
    "HLT_j80_0eta240_2j60_320eta490_invm700", # to be moved to MC menu (ATR-17534, MR !8877)
    "HLT_g6_loose", # to be removed (ATR-17462)
    ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Undefined'), 
    }])))

HLT_list=["HLT_xe100_mht_xe70_L1KF-XE60", "HLT_xe100_mht_xe70_L1KF-XE65", "HLT_xe100_mht_xe70_L1KF-XE75", "HLT_xe100_pufit_L1KF-XE60", "HLT_xe100_pufit_L1KF-XE65", "HLT_xe100_pufit_L1KF-XE75", "HLT_xe100_trkmht_L1KF-XE60", "HLT_xe100_trkmht_L1KF-XE65", "HLT_xe100_trkmht_L1KF-XE75", "HLT_xe110_L1KF-XE60", "HLT_xe110_L1KF-XE65", "HLT_xe110_L1KF-XE75", "HLT_xe110_mht_L1KF-XE60", "HLT_xe110_mht_L1KF-XE65", "HLT_xe110_mht_L1KF-XE75", "HLT_xe110_mht_em_L1KF-XE60", "HLT_xe110_mht_em_L1KF-XE65", "HLT_xe110_mht_em_L1KF-XE75", "HLT_xe110_mht_xe70_L1KF-XE60", "HLT_xe110_mht_xe70_L1KF-XE65", "HLT_xe110_mht_xe70_L1KF-XE75", "HLT_xe110_pufit_L1KF-XE60", "HLT_xe110_pufit_L1KF-XE65", "HLT_xe110_pufit_L1KF-XE75", "HLT_xe110_tc_em_L1KF-XE60", "HLT_xe110_tc_em_L1KF-XE65", "HLT_xe110_tc_em_L1KF-XE75", "HLT_xe110_tc_lcw_L1KF-XE60", "HLT_xe110_tc_lcw_L1KF-XE65", "HLT_xe110_tc_lcw_L1KF-XE75", "HLT_xe110_trkmht_L1KF-XE60", "HLT_xe110_trkmht_L1KF-XE65", "HLT_xe110_trkmht_L1KF-XE75", "HLT_xe120_L1KF-XE60", "HLT_xe120_L1KF-XE65", "HLT_xe120_L1KF-XE75", "HLT_xe120_mht_L1KF-XE60", "HLT_xe120_mht_L1KF-XE65", "HLT_xe120_mht_L1KF-XE75", "HLT_xe120_mht_em_L1KF-XE60", "HLT_xe120_mht_em_L1KF-XE65", "HLT_xe120_mht_em_L1KF-XE75", "HLT_xe120_mht_xe70_L1KF-XE60", "HLT_xe120_mht_xe70_L1KF-XE65", "HLT_xe120_mht_xe70_L1KF-XE75", "HLT_xe120_tc_em_L1KF-XE60", "HLT_xe120_tc_em_L1KF-XE65", "HLT_xe120_tc_em_L1KF-XE75", "HLT_xe120_tc_lcw_L1KF-XE60", "HLT_xe120_tc_lcw_L1KF-XE65", "HLT_xe120_tc_lcw_L1KF-XE75", "HLT_xe90_pufit_L1KF-XE60", "HLT_xe90_pufit_L1KF-XE65", "HLT_xe90_pufit_L1KF-XE75", "HLT_xe90_trkmht_L1KF-XE65", "HLT_xe90_trkmht_L1KF-XE75"]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled new KF chains'), 
    }])))


HLT_list=[
    "HLT_afp_jetexc_L1AFP_A_AND_C_SPECTOF_J100",
    "HLT_afp_jetexc_L1AFP_A_AND_C_SPECTOF_J75",
    "HLT_afp_jetexc_L1AFP_A_AND_C_SPECTOF_J50",
    
'HLT_mb_sp3_L1AFP_C_MBTS_A',
'HLT_mb_sp15_L1AFP_C_MBTS_A',
'HLT_mb_sp10_L1AFP_C_MBTS_A',
'HLT_mb_sp5_L1AFP_C_MBTS_A',
'HLT_noalg_L1AFP_C_ANY_FIRSTEMPTY',
'HLT_mb_sp10_L1AFP_C_ANY_MBTS_A',
'HLT_mb_sp3_L1AFP_C_ANY_MBTS_A',
'HLT_mb_sp5_L1AFP_C_ANY_MBTS_A',
'HLT_mb_sptrk_trk10_L1AFP_C_AND',
'HLT_mb_sptrk_trk5_L1AFP_C_AND',
'HLT_j35_320eta490_L1AFP_C_AND',
'HLT_j15_L1AFP_C_AND',
'HLT_mb_sptrk_pt6_L1AFP_C_AND',
'HLT_mb_sptrk_pt4_L1AFP_C_AND',
'HLT_mb_sp10_L1AFP_C_AND',
'HLT_j10_320eta490_L1AFP_C_AND',
'HLT_mb_sp15_L1AFP_C_AND',
'HLT_j20_L1AFP_C_AND',
'HLT_j30_L1AFP_C_AND',
'HLT_mb_sptrk_trk15_L1AFP_C_AND',
'HLT_mb_sptrk_pt8_L1AFP_C_AND',
'HLT_mb_sptrk_pt2_L1AFP_C_AND',
'HLT_mb_sp5_L1AFP_C_AND',
'HLT_j15_320eta490_L1AFP_C_AND',
'HLT_j25_320eta490_L1AFP_C_AND',
'HLT_j45_320eta490_L1AFP_C_AND',
'HLT_mb_sptrk_trk3_L1AFP_C_AND',
'HLT_mb_sp3_L1AFP_C_AND',
'HLT_noalg_L1AFP_C_ANY_EMPTY',
'HLT_j40_L1AFP_C_J12',
'HLT_e15_lhmedium_nod0_L1AFP_C_EM3',
'HLT_e10_lhmedium_nod0_L1AFP_C_EM3',
'HLT_e20_lhloose_nod0_L1AFP_C_EM3',
'HLT_2g10_medium_L1AFP_C_EM3',
'HLT_2e10_lhmedium_nod0_L1AFP_C_EM3',
'HLT_g10_medium_L1AFP_C_EM3',
'HLT_e20_lhmedium_nod0_L1AFP_C_EM3',
'HLT_2g10_loose_L1AFP_C_EM3',
'HLT_g10_loose_L1AFP_C_EM3',
'HLT_2e10_lhloose_nod0_L1AFP_C_EM3',
'HLT_g15_loose_L1AFP_C_EM3',
'HLT_3e10_lhloose_nod0_L1AFP_C_EM3',
'HLT_3g10_loose_L1AFP_C_EM3',
'HLT_g15_medium_L1AFP_C_EM3',
'HLT_g20_medium_L1AFP_C_EM3',
'HLT_g20_loose_L1AFP_C_EM3',
'HLT_e10_lhloose_nod0_L1AFP_C_EM3',
'HLT_e15_lhloose_nod0_L1AFP_C_EM3',
'HLT_2j10_deta20_L1AFP_C_AND',
'HLT_mb_sptrk_trk80_L1AFP_C_AND',
'HLT_noalg_L1AFP_NSA',
'HLT_noalg_L1AFP_NSC',
'HLT_noalg_L1AFP_FSA_SIT',
'HLT_noalg_L1AFP_FSC_SIT',
'HLT_calibAFP_L1AFP_NSA',
'HLT_calibAFP_L1AFP_NSC',
'HLT_calibAFP_L1AFP_FSA_SIT',
'HLT_calibAFP_L1AFP_FSC_SIT',
'HLT_calibAFP_L1AFP_FSC',
"HLT_calibAFP_L1AFP_C_MBTS_A", "HLT_mb_sp_L1AFP_C_AND", "HLT_mb_sp_L1AFP_C_ANY_MBTS_A", "HLT_mb_sp_L1AFP_C_MBTS_A", "HLT_mb_sp_vetosp_L1AFP_C_AND", "HLT_mb_sp_vetospmbts2in_L1AFP_C_AND", "HLT_mu4_L1AFP_C_MU4", "HLT_mu6_L1AFP_C_MU4", "HLT_noalg_L1AFP_C_MU4",
"HLT_2j10_deta20_L1AFP_A_AND_C", "HLT_2j10_deta20_L1AFP_A_OR_C", "HLT_calibAFP_L1AFP_A", "HLT_calibAFP_L1AFP_A_AND_C", "HLT_calibAFP_L1AFP_A_AND_C_SPECTOF", "HLT_calibAFP_L1AFP_A_BGRP0", "HLT_calibAFP_L1AFP_A_OR_C", "HLT_calibAFP_L1AFP_A_OR_C_EMPTY", "HLT_calibAFP_L1AFP_A_OR_C_FIRSTEMPTY", "HLT_calibAFP_L1AFP_A_OR_C_UNPAIRED_ISO", "HLT_calibAFP_L1AFP_A_OR_C_UNPAIRED_NONISO", "HLT_calibAFP_L1AFP_A_SPECTOF_BGRP0", "HLT_calibAFP_L1AFP_C", "HLT_calibAFP_L1AFP_C_BGRP0", "HLT_calibAFP_L1AFP_C_SPECTOF_BGRP0", "HLT_calibAFP_L1AFP_FSA_SIT_BGRP0", "HLT_calibAFP_L1AFP_FSC_SIT_BGRP0", "HLT_calibAFP_L1AFP_NSA_BGRP0", "HLT_calibAFP_L1AFP_NSC_BGRP0", "HLT_j10_320eta490_L1AFP_A_AND_C", "HLT_j10_320eta490_L1AFP_A_OR_C", "HLT_j10_L1AFP_A_AND_C", "HLT_j10_L1AFP_A_OR_C", "HLT_j20_L1AFP_A_AND_C", "HLT_j20_L1AFP_A_AND_C_J12", "HLT_j20_L1AFP_A_OR_C", "HLT_j20_L1AFP_A_OR_C_J12", "HLT_mb_sptrk_pt2_L1AFP_A_AND_C", "HLT_mb_sptrk_pt2_L1AFP_A_OR_C", "HLT_mb_sptrk_pt4_L1AFP_A_AND_C", "HLT_mb_sptrk_pt4_L1AFP_A_OR_C", "HLT_mb_sptrk_pt6_L1AFP_A_AND_C", "HLT_mb_sptrk_pt6_L1AFP_A_OR_C", "HLT_mb_sptrk_pt8_L1AFP_A_AND_C", "HLT_mb_sptrk_pt8_L1AFP_A_OR_C", "HLT_noalg_L1AFP_A", "HLT_noalg_L1AFP_A_AND_C", "HLT_noalg_L1AFP_A_AND_C_EM3", "HLT_noalg_L1AFP_A_AND_C_J12", "HLT_noalg_L1AFP_A_AND_C_MBTS_2", "HLT_noalg_L1AFP_A_AND_C_MU4", "HLT_noalg_L1AFP_A_AND_C_SPECTOF_J100", "HLT_noalg_L1AFP_A_AND_C_SPECTOF_J50", "HLT_noalg_L1AFP_A_AND_C_SPECTOF_J75", "HLT_noalg_L1AFP_A_AND_C_TE5", "HLT_noalg_L1AFP_A_OR_C", "HLT_noalg_L1AFP_A_OR_C_EM3", "HLT_noalg_L1AFP_A_OR_C_EMPTY", "HLT_noalg_L1AFP_A_OR_C_FIRSTEMPTY", "HLT_noalg_L1AFP_A_OR_C_J12", "HLT_noalg_L1AFP_A_OR_C_MBTS_2", "HLT_noalg_L1AFP_A_OR_C_MU4", "HLT_noalg_L1AFP_A_OR_C_TE5", "HLT_noalg_L1AFP_A_OR_C_UNPAIRED_ISO", "HLT_noalg_L1AFP_A_OR_C_UNPAIRED_NONISO", "HLT_noalg_L1AFP_C",
"HLT_noalg_L1MBTS_2",
"HLT_j20_L1MBTS_2",
"HLT_j10_L1RD0_FILLED",
"HLT_j10_320eta490_L1RD0_FILLED",
"HLT_j20_L1RD0_FILLED",
"HLT_j20_L1J12",
"HLT_e10_lhloose_L1EM3",
"HLT_mu4_L1MU4",
"HLT_e10_lhloose_L1EM3_AFP_A_AND_C", "HLT_e10_lhloose_L1EM3_AFP_A_OR_C", "HLT_g10_loose_L1EM3_AFP_A_AND_C", "HLT_g10_loose_L1EM3_AFP_A_OR_C", "HLT_mb_sp_L1RD0_FILLED", "HLT_mb_sp_vetosp_L1AFP_A_AND_C", "HLT_mb_sp_vetospmbts2in_L1AFP_A", "HLT_mb_sp_vetospmbts2in_L1AFP_A_AND_C", "HLT_mb_sp_vetospmbts2in_L1AFP_A_OR_C", "HLT_mb_sp_vetospmbts2in_L1AFP_C", "HLT_mu4_L1MU4_AFP_A_AND_C", "HLT_mu4_L1MU4_AFP_A_OR_C", "HLT_noalg_L1EM3_AFP_A_AND_C", "HLT_noalg_L1EM3_AFP_A_OR_C", "HLT_noalg_L1MU4_AFP_A_AND_C", "HLT_noalg_L1MU4_AFP_A_OR_C",
'HLT_xe100_trkmht_xe100_pufit_L1XE55',
'HLT_xe100_trkmht_xe100_pufit_L1XE60',
'HLT_xe110_trkmht_xe110_mht_xe80_L1XE55',
'HLT_xe110_trkmht_xe100_pufit_L1XE55',
'HLT_xe110_trkmht_xe100_pufit_L1XE60',
'HLT_xe110_trkmht_xe110_mht_xe80_L1XE60',
'HLT_xe120_trkmht_xe100_pufit_L1XE55',
'HLT_xe120_trkmht_xe110_mht_xe80_L1XE55',
'HLT_xe120_trkmht_xe110_mht_xe80_L1XE60',
'HLT_xe120_trkmht_xe100_pufit_L1XE60',
'HLT_larpebcalib_L1RD0_EMPTY',
'HLT_lumipeb_vdm_L1RD0_BGRP10',
'HLT_larpebcalib_L1RD0_BGRP11',
# backup di-tau triggers (ATR-16230)
'HLT_tau40_medium1_tracktwo_tau25_medium1_tracktwo',
'HLT_2tau35_medium1_tracktwo',
'HLT_2tau35_medium1_tracktwo_L12TAU20IM_3J20',
'HLT_2g3_medium_dPhi15_L12EM3_VTE70',
'HLT_2g3_loose_L12EM3_VTE70',
'HLT_2g3_loose_dPhi15_L12EM3_VTE70',
'HLT_noalg_L1J12_EMPTY',
'HLT_noalg_L1TAU8_EMPTY',
'HLT_noalg_L1MU6_EMPTY',
'HLT_noalg_L1EM3_EMPTY',
'HLT_j45_L1AFP_A_AND_C_J12',
'HLT_j45_L1AFP_A_OR_C_J12',
'HLT_e20_lhloose_L1EM12_AFP_A_OR_C',
'HLT_g20_loose_L1EM12_AFP_A_OR_C',
'HLT_e15_lhloose_L1EM7_AFP_A_OR_C',
'HLT_g15_loose_L1EM7_AFP_A_OR_C',
]

disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled AFP-seeded chains'), 
    }])))


# --------------------------------------
# low mu run chains

HLT_list=[
 'HLT_lhcfpeb', 'HLT_lhcfpeb_L1LHCF_EMPTY', 'HLT_lhcfpeb_L1LHCF_UNPAIRED_ISO',]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled low-mu'), 
    }])))

# --------------------------------------
# streamers

HLT_list=[
"HLT_noalg_L1Topo",
"HLT_noalg_l1topo_L1J25_2J20_3J12_BOX-TAU20ITAU12I",
"HLT_noalg_l1topo_L1MU10_TAU12I-J25",
"HLT_noalg_l1topo_L1XE45_TAU20-J20",
"HLT_noalg_l1topo_L1XE35_EM15-TAU12I",
"HLT_noalg_l1topo_L1XE40_EM15-TAU12I",
"HLT_noalg_l1topo_L1BTAG-MU4J15",
"HLT_noalg_l1topo_L1BTAG-MU4J30",
"HLT_noalg_l1topo_L1BTAG-MU6J20",
"HLT_noalg_l1topo_L1BTAG-MU6J25",
"HLT_noalg_l1topo_L13J15_BTAG-MU4J15",
"HLT_noalg_l1topo_L13J15_BTAG-MU4J30",
"HLT_noalg_l1topo_L13J15_BTAG-MU6J25",
"HLT_noalg_l1topo_L13J20_BTAG-MU4J20",
"HLT_noalg_l1topo_L1J40_DPHI-Js2XE50",
"HLT_noalg_l1topo_L1J40_DPHI-J20XE50",
"HLT_noalg_l1topo_L1J40_DPHI-CJ20XE50",
"HLT_noalg_l1topo_L1J40_DPHI-J20s2XE50",
"HLT_noalg_l1topo_L1JPSI-1M5",
"HLT_noalg_l1topo_L1JPSI-1M5-EM7",
"HLT_noalg_l1topo_L1JPSI-1M5-EM12",
"HLT_noalg_l1topo_L1KF-XE35",
"HLT_noalg_l1topo_L1KF-XE45",
"HLT_noalg_l1topo_L1KF-XE75",
"HLT_noalg_l1topo_L1EM12_W-MT25",
"HLT_noalg_l1topo_L1EM12_W-MT30",
"HLT_noalg_l1topo_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE",
"HLT_noalg_l1topo_L1W-05RO-XEHT-0",
"HLT_noalg_l1topo_L1W-90RO2-XEHT-0",
"HLT_noalg_l1topo_L1W-250RO2-XEHT-0",
"HLT_noalg_l1topo_L1W-NOMATCH",
"HLT_noalg_l1topo_L1W-NOMATCH_W-05RO-XEEMHT",
"HLT_noalg_l1topo_L1EM15_W-MT35_XS60_W-05DPHI-JXE-0_W-05DPHI-EM15XE",
"HLT_noalg_l1topo_L1EM15_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EM15XE",
"HLT_noalg_l1topo_L1EM15_W-MT35",
"HLT_noalg_l1topo_L1EM15_W-MT35_XS60",
"HLT_noalg_l1topo_L1EM15VH_W-MT35_XS60",
"HLT_noalg_l1topo_L1EM20VH_W-MT35_XS60",
"HLT_noalg_l1topo_L1EM22VHI_W-MT35_XS40",
"HLT_noalg_l1topo_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30",
"HLT_noalg_l1topo_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE",
"HLT_noalg_l1topo_L1BPH-1M19-2MU4_BPH-0DR34-2MU4",
"HLT_noalg_l1topo_L1MU6MU4-BO",
"HLT_noalg_l1topo_L12MU4-B",
"HLT_noalg_l1topo_L12MU6-B",
"HLT_noalg_l1topo_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4",
"HLT_noalg_l1topo_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4",
"HLT_noalg_l1topo_L12MU4-BO",
"HLT_noalg_l1topo_L12MU6-BO",
"HLT_noalg_l1topo_L1MU6_2MU4-B",
#"HLT_noalg_l1topo_L1DY-DR-2MU4",
#"HLT_noalg_l1topo_L1DY-BOX-2MU4",
#"HLT_noalg_l1topo_L1DY-BOX-2MU6",
#"HLT_noalg_l1topo_L1LFV-EM8I",
#"HLT_noalg_l1topo_L1LFV-EM15I",
"HLT_noalg_l1topo_L1DPHI-Js2XE50",

"HLT_noalg_l1topo_L1EM15-TAU40",
"HLT_noalg_l1topo_L1EM15-TAU12I",
#"HLT_noalg_l1topo_L1DR-TAU20ITAU12I",
#"HLT_noalg_l1topo_L1DR-TAU20ITAU12I-J25",
"HLT_noalg_l1topo_L1MU6_MJJ-200",
"HLT_noalg_l1topo_L1MU6_MJJ-300",
"HLT_noalg_l1topo_L1MU6_MJJ-400",
"HLT_noalg_l1topo_L1MU6_MJJ-500",
"HLT_noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-400",
"HLT_noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-700",
"HLT_noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-800",
"HLT_noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-900",
"HLT_noalg_l1topo_L13J20_4J20.0ETA49_MJJ-400",
"HLT_noalg_l1topo_L13J20_4J20.0ETA49_MJJ-700",
"HLT_noalg_l1topo_L13J20_4J20.0ETA49_MJJ-800",
"HLT_noalg_l1topo_L13J20_4J20.0ETA49_MJJ-900",
"HLT_noalg_l1topo_L1XE35_MJJ-200",

'HLT_noalg_L12EM15', 'HLT_noalg_L12EM3', 'HLT_noalg_L1ALFA_Diff', 'HLT_noalg_L1ALFA_Phys', 'HLT_noalg_L1ALFA_PhysAny', 'HLT_noalg_L1ALFA_SYS', 'HLT_noalg_L1All', 'HLT_noalg_L1CALREQ2', 'HLT_noalg_L1Calo', 'HLT_noalg_L1Calo_EMPTY', 'HLT_noalg_L1EM12', 'HLT_noalg_L1EM15', 'HLT_noalg_L1EM15VH_3EM7', 'HLT_noalg_L1EM18VH', 'HLT_noalg_L1EM20VH', 'HLT_noalg_L1EM3', 'HLT_noalg_L1EM7', 'HLT_noalg_L1J20_J20.31ETA49', 'HLT_noalg_L1J50', 'HLT_noalg_L1J75', 'HLT_noalg_L1J75.31ETA49', 'HLT_noalg_L1LHCF', 'HLT_noalg_L1LHCF_EMPTY', 'HLT_noalg_L1LHCF_UNPAIRED_ISO', 'HLT_noalg_L1MBTS_1_BGRP11', 'HLT_noalg_L1MBTS_1_BGRP9', 'HLT_noalg_L1MBTS_2_BGRP11', 'HLT_noalg_L1MBTS_2_BGRP9', 'HLT_noalg_L1MJJ-100', 'HLT_noalg_L1MU4_J12', 'HLT_noalg_L1MU6_J20', 'HLT_noalg_L1RD0_BGRP11', 'HLT_noalg_L1RD0_BGRP9', 'HLT_noalg_L1Standby', 'HLT_noalg_L1TAU12', 'HLT_noalg_L1TAU12IL', 'HLT_noalg_L1TAU12IM', 'HLT_noalg_L1TAU12IT', 'HLT_noalg_L1TAU20', 'HLT_noalg_L1TAU20IL', 'HLT_noalg_L1TAU20IM', 'HLT_noalg_L1TAU20IM_2TAU12IM', 'HLT_noalg_L1TAU20IT', 'HLT_noalg_L1TAU20_2TAU12', 'HLT_noalg_L1TAU30', 'HLT_noalg_L1TAU60', 'HLT_noalg_L1XE10', 'HLT_noalg_bkg_L1J12', 'HLT_noalg_bkg_L1J30.31ETA49', 'HLT_noalg_eb_L1ABORTGAPNOTCALIB_noPS', 'HLT_noalg_eb_L1EMPTY_noPS', 'HLT_noalg_eb_L1FIRSTEMPTY_noPS', 'HLT_noalg_eb_L1PhysicsHigh_noPS', 'HLT_noalg_eb_L1PhysicsLow_noPS', 'HLT_noalg_eb_L1RD3_EMPTY', 'HLT_noalg_eb_L1RD3_FILLED', 'HLT_noalg_eb_L1UNPAIRED_ISO_noPS', 'HLT_noalg_eb_L1UNPAIRED_NONISO_noPS', 'HLT_noalg_idcosmic_L1TRT_FILLED', 'HLT_noalg_L1ALFA_ANY', 'HLT_noalg_L1BPH-2M-2MU4', 'HLT_noalg_L1BPH-2M-2MU6', 'HLT_noalg_L1BPH-2M-MU6MU4', 'HLT_noalg_L1BPH-4M8-2MU4', 'HLT_noalg_L1BPH-4M8-2MU6', 'HLT_noalg_L1BPH-4M8-MU6MU4', 'HLT_noalg_L1BPH-DR-2MU4', 'HLT_noalg_L1BPH-DR-2MU6', 'HLT_noalg_L1BPH-DR-MU6MU4', 'HLT_noalg_L1DR-MU10TAU12I', 'HLT_noalg_L1DR-TAU20ITAU12I', 'HLT_noalg_L1DY-BOX-2MU4', 'HLT_noalg_L1DY-BOX-2MU6', 'HLT_noalg_L1DY-BOX-MU6MU4', 'HLT_noalg_L1DY-DR-2MU4', 'HLT_noalg_L1EM10', 'HLT_noalg_L1EM10VH', 'HLT_noalg_L1EM13VH', 'HLT_noalg_L1EM20VHI', 'HLT_noalg_L1EM22VHI', 'HLT_noalg_L1EM8VH', 'HLT_noalg_L1J100', 'HLT_noalg_L1J12', 'HLT_noalg_L1J120', 'HLT_noalg_L1J15', 'HLT_noalg_L1J20', 'HLT_noalg_L1J25', 'HLT_noalg_L1J30', 'HLT_noalg_L1J40', 'HLT_noalg_L1J85', 'HLT_noalg_L1LFV-MU', 'HLT_noalg_L1LFV-MU11', 'HLT_noalg_L1TAU40', 'HLT_noalg_L1TAU8', 'HLT_noalg_L1TE10', 'HLT_noalg_L1TE10.0ETA24', 'HLT_noalg_L1TE20', 'HLT_noalg_L1TE20.0ETA24', 'HLT_noalg_L1TE30', 'HLT_noalg_L1TE30.0ETA24', 'HLT_noalg_L1TE40', 'HLT_noalg_L1TE50', 'HLT_noalg_L1XE55', 'HLT_noalg_L1XE60', 'HLT_noalg_L1XE70', 'HLT_noalg_L1XE80', 'HLT_noalg_L1XS20', 'HLT_noalg_L1XS30', 'HLT_noalg_L1XS40', 'HLT_noalg_L1XS50', 'HLT_noalg_L1XS60', 'HLT_noalg_L1J100.31ETA49', 'HLT_noalg_L1J15.31ETA49', 'HLT_noalg_L1J20.28ETA31', 'HLT_noalg_L1J20.31ETA49', 'HLT_noalg_L1J30.31ETA49', 'HLT_noalg_L1J50.31ETA49', 'HLT_noalg_L1RD1_FILLED', 'HLT_noalg_L1RD2_FILLED', 'HLT_noalg_L1RD2_EMPTY', 'HLT_noalg_L1RD3_FILLED', 'HLT_noalg_L1RD3_EMPTY', 'HLT_noalg_standby_L1RD0_FILLED', 'HLT_noalg_standby_L1RD0_EMPTY', "HLT_noalg_L1TE40.0ETA24", "HLT_noalg_L1TE50.0ETA24", "HLT_noalg_L1TE70", "HLT_noalg_mb_L1TE50.0ETA24", "HLT_noalg_mb_L1TE70", "HLT_noalg_mb_L1TE70.0ETA24", "HLT_noalg_mb_L1ZDC_A", "HLT_noalg_mb_L1ZDC_AND", "HLT_noalg_mb_L1ZDC_A_C", "HLT_noalg_mb_L1ZDC_C",


    ]

disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled Streamer'), 
    }])))

#disabled with express tag
HLT_list = [
'HLT_noalg_cosmicmuons_L1MU4_EMPTY',
"HLT_noalg_standby_L13J15", 
'HLT_noalg_cosmiccalo_L1J30_FIRSTEMPTY', 
"HLT_noalg_standby_L14J15", 
'HLT_noalg_cosmicmuons_L1MU11_EMPTY', 
'HLT_noalg_L1LowLumi', 
"HLT_noalg_cosmiccalo_L1RD1_BGRP10", 
"HLT_noalg_standby_L1J15", 
'HLT_noalg_cosmiccalo_L1J12_FIRSTEMPTY',  
'HLT_noalg_idcosmic_L1TRT_EMPTY', 
"HLT_noalg_L1MU20MU21", 
]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled Streamer',ESValue=0), 
    }])))


# --------------------------------------
# beam spot

HLT_list=[
 'HLT_beamspot_activeTE_trkfast_pebTRT_L1TRT_EMPTY', 'HLT_beamspot_activeTE_trkfast_pebTRT_L1TRT_FILLED', 'HLT_beamspot_activeTE_trkfast_peb_L1TRT_EMPTY', 'HLT_beamspot_activeTE_trkfast_peb_L1TRT_FILLED', 'HLT_beamspot_allTE_trkfast', 'HLT_beamspot_allTE_trkfast_pebTRT_L1TRT_EMPTY', 'HLT_beamspot_allTE_trkfast_pebTRT_L1TRT_FILLED', 'HLT_beamspot_allTE_trkfast_peb_L1TRT_EMPTY', 'HLT_beamspot_allTE_trkfast_peb_L1TRT_FILLED', 'HLT_beamspot_trkFS_trkfast_L1TRT_EMPTY', 'HLT_beamspot_trkFS_trkfast_L1TRT_FILLED', ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled beamspot'), 
    }])))

# --------------------------------------
# calibration

HLT_list=[
    'HLT_alfacalib_L1ALFA_A7L1_OD', 'HLT_alfacalib_L1ALFA_A7R1_OD', 'HLT_alfacalib_L1ALFA_B7L1_OD', 'HLT_alfacalib_L1ALFA_B7R1_OD', 'HLT_alfacalib_L1ALFA_ELAS', 'HLT_alfacalib_L1ALFA_ELAST15', 'HLT_alfacalib_L1ALFA_ELAST18', 'HLT_alfacalib_L1ALFA_SYS', 'HLT_ibllumi_L1RD0_ABORTGAPNOTCALIB', 'HLT_ibllumi_L1RD0_FILLED', 'HLT_ibllumi_L1RD0_UNPAIRED_ISO', 'HLT_l1calocalib', 'HLT_l1calocalib_L1BGRP9', 'HLT_larnoiseburst_L1All', 'HLT_larnoiseburst_loose_L1All', 'HLT_larnoiseburst_loose_rerun', 'HLT_lumipeb_vdm_L1MBTS_1', 'HLT_lumipeb_vdm_L1MBTS_1_BGRP11', 'HLT_lumipeb_vdm_L1MBTS_1_BGRP9', 'HLT_lumipeb_vdm_L1MBTS_1_UNPAIRED_ISO', 'HLT_lumipeb_vdm_L1MBTS_2', 'HLT_lumipeb_vdm_L1MBTS_2_BGRP11', 'HLT_lumipeb_vdm_L1MBTS_2_BGRP9', 'HLT_lumipeb_vdm_L1MBTS_2_UNPAIRED_ISO', 'HLT_lumipeb_vdm_L1RD0_BGRP11', 'HLT_lumipeb_vdm_L1RD0_BGRP9', 'HLT_lumipeb_vdm_L1RD0_FILLED', 'HLT_lumipeb_vdm_L1RD0_UNPAIRED_ISO',
    'HLT_alfacalib_L1ALFA_SYS_Calib',
    ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled calib'), 
    }])))

# --------------------------------------
# minbias

HLT_list=['HLT_mb_sp_L1RD0_UNPAIRED_ISO','HLT_noalg_bkg_L1MBTS_4_A_UNPAIRED_ISO','HLT_noalg_bkg_L1MBTS_4_C_UNPAIRED_ISO','HLT_mb_mbts_L1MBTS_1', 'HLT_mb_mbts_L1MBTS_1_1', 'HLT_mb_mbts_L1MBTS_1_1_EMPTY', 'HLT_mb_mbts_L1MBTS_1_1_UNPAIRED_ISO', 'HLT_mb_mbts_L1MBTS_1_EMPTY', 'HLT_mb_mbts_L1MBTS_1_UNPAIRED_ISO', 'HLT_mb_mbts_L1MBTS_2', 'HLT_mb_mbts_L1MBTS_2_EMPTY', 'HLT_mb_mbts_L1MBTS_2_UNPAIRED_ISO', 'HLT_mb_perf_L1LUCID', 'HLT_mb_perf_L1LUCID_EMPTY', 'HLT_mb_perf_L1LUCID_UNPAIRED_ISO', 'HLT_mb_perf_L1MBTS_2', 'HLT_mb_perf_L1RD1_FILLED', 'HLT_mb_sp1000_trk70_hmt_L1MBTS_1_1', 'HLT_mb_sp1200_trk75_hmt_L1MBTS_1_1', 'HLT_mb_sp1400_hmtperf_L1TE10', 'HLT_mb_sp1400_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1400_hmtperf_L1TE20', 'HLT_mb_sp1400_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1400_trk100_hmt_L1TE20', 'HLT_mb_sp1400_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1400_trk80_hmt_L1MBTS_1_1', 'HLT_mb_sp1400_trk90_hmt_L1TE10', 'HLT_mb_sp1400_trk90_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_hmtperf_L1TE20', 'HLT_mb_sp2000_pusup600_trk70_hmt', 'HLT_mb_sp2000_pusup600_trk70_hmt_L1TE30', 'HLT_mb_sp2000_pusup600_trk70_hmt_L1TE40', 'HLT_mb_sp2000_pusup700_trk50_sumet110_hmt_L1TE40', 'HLT_mb_sp2000_pusup700_trk50_sumet150_hmt_L1TE50', 'HLT_mb_sp2000_pusup700_trk50_sumet70_hmt_L1TE30', 'HLT_mb_sp2000_pusup700_trk60_hmt_L1TE30', 'HLT_mb_sp2000_pusup700_trk70_hmt_L1TE30', 'HLT_mb_sp2000_pusup700_trk70_hmt_L1TE40', 'HLT_mb_sp2000_trk70_hmt', 'HLT_mb_sp2500_hmtperf_L1TE20', 'HLT_mb_sp2500_pusup750_trk90_hmt_L1TE40', 'HLT_mb_sp2_hmtperf', 'HLT_mb_sp2_hmtperf_L1MBTS_1_1', 'HLT_mb_sp3000_pusup800_trk120_hmt_L1TE50', 'HLT_mb_sp300_trk10_sumet40_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet50_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet50_hmt_L1RD3_FILLED', 'HLT_mb_sp300_trk10_sumet50_hmt_L1TE10', 'HLT_mb_sp300_trk10_sumet60_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet60_hmt_L1TE20', 'HLT_mb_sp300_trk10_sumet70_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet80_hmt_L1MBTS_1_1', 'HLT_mb_sp400_trk40_hmt_L1MBTS_1_1', 'HLT_mb_sp500_hmtperf', 'HLT_mb_sp600_trk10_sumet40_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet50_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet50_hmt_L1RD3_FILLED', 'HLT_mb_sp600_trk10_sumet50_hmt_L1TE10.0ETA24', 'HLT_mb_sp600_trk10_sumet60_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet60_hmt_L1TE20.0ETA24', 'HLT_mb_sp600_trk10_sumet70_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet80_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk45_hmt_L1MBTS_1_1', 'HLT_mb_sp700_trk50_hmt_L1MBTS_1_1', 'HLT_mb_sp700_trk50_hmt_L1RD3_FILLED', 'HLT_mb_sp700_trk55_hmt_L1MBTS_1_1', 'HLT_mb_sp900_trk60_hmt_L1MBTS_1_1', 'HLT_mb_sp900_trk65_hmt_L1MBTS_1_1','HLT_mb_sptrk_L1RD0_EMPTY', 'HLT_mb_sptrk_L1RD0_UNPAIRED_ISO', 'HLT_mb_sptrk_L1RD3_FILLED', 'HLT_mb_sptrk_costr', 'HLT_mb_sptrk_costr_L1RD0_EMPTY', 'HLT_mb_sptrk_noisesup', 'HLT_mb_sptrk_noisesup_L1RD0_EMPTY', 'HLT_mb_sptrk_noisesup_L1RD0_UNPAIRED_ISO', 'HLT_mb_sptrk_noisesup_L1RD3_FILLED', 'HLT_mb_sptrk_pt4_L1MBTS_1', 'HLT_mb_sptrk_pt4_L1MBTS_1_1', 'HLT_mb_sptrk_pt4_L1MBTS_2', 'HLT_mb_sptrk_pt4_L1RD3_FILLED', 'HLT_mb_sptrk_pt6_L1MBTS_1', 'HLT_mb_sptrk_pt6_L1MBTS_1_1', 'HLT_mb_sptrk_pt6_L1MBTS_2', 'HLT_mb_sptrk_pt6_L1RD3_FILLED', 'HLT_mb_sptrk_pt8_L1MBTS_1', 'HLT_mb_sptrk_pt8_L1MBTS_1_1', 'HLT_mb_sptrk_pt8_L1MBTS_2', 'HLT_mb_sptrk_pt8_L1RD3_FILLED', 'HLT_mb_sptrk_vetombts2in_L1ALFA_CEP', 'HLT_mb_sptrk_vetombts2in_peb_L1ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_peb_L1ALFA_ANY_UNPAIRED_ISO', 'HLT_noalg_mb_L1LUCID', 'HLT_noalg_mb_L1LUCID_EMPTY', 'HLT_noalg_mb_L1LUCID_UNPAIRED_ISO', 'HLT_noalg_mb_L1MBTS_1', 'HLT_noalg_mb_L1MBTS_1_1', 'HLT_noalg_mb_L1MBTS_1_1_EMPTY', 'HLT_noalg_mb_L1MBTS_1_1_UNPAIRED_ISO', 'HLT_noalg_mb_L1MBTS_1_EMPTY', 'HLT_noalg_mb_L1MBTS_1_UNPAIRED_ISO', 'HLT_noalg_mb_L1MBTS_2_EMPTY', 'HLT_noalg_mb_L1MBTS_2_UNPAIRED_ISO', 'HLT_noalg_mb_L1RD0_EMPTY', 'HLT_noalg_mb_L1RD0_FILLED', 'HLT_noalg_mb_L1RD0_UNPAIRED_ISO', 'HLT_noalg_mb_L1RD1_FILLED', 'HLT_noalg_mb_L1RD2_EMPTY', 'HLT_noalg_mb_L1RD2_FILLED', 'HLT_noalg_mb_L1RD3_EMPTY', 'HLT_noalg_mb_L1RD3_FILLED', 'HLT_noalg_mb_L1TE10', 'HLT_noalg_mb_L1TE10.0ETA24', 'HLT_noalg_mb_L1TE20', 'HLT_noalg_mb_L1TE20.0ETA24', 'HLT_noalg_mb_L1TE30', 'HLT_noalg_mb_L1TE40', 'HLT_noalg_mb_L1TE50','HLT_mb_sp1800_hmtperf_L1TE30.0ETA24','HLT_noalg_mb_L1TE30.0ETA24','HLT_noalg_L1TE0', 'HLT_noalg_L1TE0.0ETA24', 'HLT_noalg_L1TE15', 'HLT_noalg_L1TE5', 'HLT_noalg_L1TE5.0ETA24', 'HLT_noalg_mb_L1TE0', 'HLT_noalg_mb_L1TE0.0ETA24', 'HLT_noalg_mb_L1TE15', 'HLT_noalg_mb_L1TE5', 'HLT_noalg_mb_L1TE5.0ETA24', 'HLT_mb_sp1300_hmtperf_L1TE10', 'HLT_mb_sp1300_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1300_hmtperf_L1TE15', 'HLT_mb_sp1300_hmtperf_L1TE20', 'HLT_mb_sp1300_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1300_hmtperf_L1TE5', 'HLT_mb_sp1300_hmtperf_L1TE5.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE10', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE10.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE15', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE20', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE20.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE5', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE5.0ETA24', 'HLT_mb_sp1400_trk90_hmt_L1TE15', 'HLT_mb_sp1400_trk90_hmt_L1TE20', 'HLT_mb_sp1400_trk90_hmt_L1TE20.0ETA24', 'HLT_mb_sp1400_trk90_hmt_L1TE5', 'HLT_mb_sp1400_trk90_hmt_L1TE5.0ETA24', 'HLT_mb_sp1500_hmtperf_L1TE10', 'HLT_mb_sp1500_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1500_hmtperf_L1TE15', 'HLT_mb_sp1500_hmtperf_L1TE20', 'HLT_mb_sp1500_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1500_hmtperf_L1TE5', 'HLT_mb_sp1500_hmtperf_L1TE5.0ETA24', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE10', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE15', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE20', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE5', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1600_trk100_hmt_L1TE10', 'HLT_mb_sp1600_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1600_trk100_hmt_L1TE20', 'HLT_mb_sp1600_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1600_trk100_hmt_L1TE5', 'HLT_mb_sp1600_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1700_hmtperf_L1TE10', 'HLT_mb_sp1700_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1700_hmtperf_L1TE15', 'HLT_mb_sp1700_hmtperf_L1TE20', 'HLT_mb_sp1700_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1700_hmtperf_L1TE5', 'HLT_mb_sp1700_hmtperf_L1TE5.0ETA24', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE10', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE15', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE20', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE20.0ETA24', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE5', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE5.0ETA24', 'HLT_mb_sp1800_trk110_hmt_L1TE10', 'HLT_mb_sp1800_trk110_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_trk110_hmt_L1TE15', 'HLT_mb_sp1800_trk110_hmt_L1TE20', 'HLT_mb_sp1800_trk110_hmt_L1TE20.0ETA24', 'HLT_mb_sp1800_trk110_hmt_L1TE5', 'HLT_mb_sp1800_trk110_hmt_L1TE5.0ETA24', 'HLT_mb_sp2_hmtperf_L1TE0', 'HLT_mb_sp2_hmtperf_L1TE0.0ETA24', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE10', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE10.0ETA24', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE5', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE5.0ETA24', 'HLT_mb_sp900_trk60_hmt_L1TE10', 'HLT_mb_sp900_trk60_hmt_L1TE10.0ETA24', 'HLT_mb_sp900_trk60_hmt_L1TE5.0ETA24', 'HLT_lumipeb_L1MBTS_2', 'HLT_mb_sptrk_vetombts2in_L1TRT_ALFA_EINE', 'HLT_mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY_UNPAIRED_ISO','mb_sptrk_vetombts2in_L1TRT_ALFA_EINE','mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY','mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY_UNPAIRED_ISO','lumipeb_L1MBTS_2', 'HLT_lumipeb_L1ALFA_BGT', 'HLT_lumipeb_L1ALFA_BGT_BGRP10', 'HLT_lumipeb_L1ALFA_BGT_UNPAIRED_ISO', 'HLT_lumipeb_L1MBTS_1', 'HLT_lumipeb_L1MBTS_1_UNPAIRED_ISO', 'HLT_lumipeb_L1MBTS_2_UNPAIRED_ISO', 'HLT_mb_sp1300_hmtperf_L1TE3', 'HLT_mb_sp1300_hmtperf_L1TE3.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE25', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE10', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE15', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE20', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE25', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE5', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1500_trk100_hmt_L1TE10', 'HLT_mb_sp1500_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1500_trk100_hmt_L1TE15', 'HLT_mb_sp1500_trk100_hmt_L1TE20', 'HLT_mb_sp1500_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1500_trk100_hmt_L1TE5', 'HLT_mb_sp1500_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE10', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE15', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE20', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE20.0ETA24', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE25', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE5', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE5.0ETA24', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE10', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE15', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE20', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE25', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE10', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE15', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE20', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE25', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE10', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE15', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE20', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE25', 'HLT_mb_sp2_hmtperf_L1TE3', 'HLT_mb_sp2_hmtperf_L1TE3.0ETA24', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE10', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE15', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE20', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE25', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE30', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE10', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE15', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE20', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE25', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE30', 'HLT_mb_sptrk_vetombts2in_L1ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_L1ALFA_ANY_UNPAIRED_ISO', 'HLT_mb_sptrk_vetombts2in_L1TRT_ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_L1TRT_ALFA_ANY_UNPAIRED_ISO', 'HLT_noalg_L1ALFA_CDiff_Phys', 'HLT_noalg_L1ALFA_Diff_Phys', 'HLT_noalg_L1ALFA_Jet_Phys', 'HLT_noalg_L1TE15.0ETA24', 'HLT_noalg_L1TE25', 'HLT_noalg_L1TE25.0ETA24', 'HLT_noalg_L1TE3', 'HLT_noalg_L1TE3.0ETA24', 'HLT_noalg_mb_L1TE15.0ETA24', 'HLT_noalg_mb_L1TE25', 'HLT_noalg_mb_L1TE25.0ETA24', 'HLT_noalg_mb_L1TE3', 'HLT_noalg_mb_L1TE3.0ETA24', 'HLT_noalg_mb_L1TE40.0ETA24', "HLT_te20", "HLT_te20_tc_lcw", "HLT_te40_L1MBTS_1_1", "HLT_te50_L1MBTS_1_1", "HLT_te50_L1RD3_FILLED", "HLT_te50_L1TE20", "HLT_te50_L1TE20.0ETA24", "HLT_te60_L1MBTS_1_1", "HLT_te60_L1TE40", "HLT_te60_L1TE40.0ETA24", "HLT_te70_L1MBTS_1_1", "HLT_te80_L1MBTS_1_1",
"HLT_mb_sp1000_pusup450_trk70_hmt_L1TE10", "HLT_mb_sp1000_pusup450_trk70_hmt_L1TE10.0ETA24", "HLT_mb_sp1000_pusup450_trk70_hmt_L1TE5", "HLT_mb_sp1000_pusup450_trk70_hmt_L1TE5.0ETA24", "HLT_mb_sp1000_trk70_hmt_L1TE10", "HLT_mb_sp1000_trk70_hmt_L1TE10.0ETA24", "HLT_mb_sp1000_trk70_hmt_L1TE5", "HLT_mb_sp1000_trk70_hmt_L1TE5.0ETA24", "HLT_mb_sp1100_pusup450_trk70_hmt_L1TE10", "HLT_mb_sp1100_pusup450_trk70_hmt_L1TE20", "HLT_mb_sp1100_pusup450_trk70_hmt_L1TE30", "HLT_mb_sp1100_pusup450_trk70_hmt_L1TE5", "HLT_mb_sp1100_trk70_hmt_L1TE10", "HLT_mb_sp1100_trk70_hmt_L1TE20", "HLT_mb_sp1100_trk70_hmt_L1TE30", "HLT_mb_sp1100_trk70_hmt_L1TE5", "HLT_mb_sp1200_hmtperf_L1TE10", "HLT_mb_sp1200_hmtperf_L1TE10.0ETA24", "HLT_mb_sp1200_hmtperf_L1TE15.0ETA24", "HLT_mb_sp1200_pusup500_trk100_hmt_L1TE5", "HLT_mb_sp1200_pusup500_trk100_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE10.0ETA24", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE15.0ETA24", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE20", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE30", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE5", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_pusup500_trk90_hmt_L1TE5", "HLT_mb_sp1200_pusup500_trk90_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_trk100_hmt_L1TE5", "HLT_mb_sp1200_trk100_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_trk80_hmt_L1TE10.0ETA24", "HLT_mb_sp1200_trk80_hmt_L1TE15.0ETA24", "HLT_mb_sp1200_trk80_hmt_L1TE20", "HLT_mb_sp1200_trk80_hmt_L1TE30", "HLT_mb_sp1200_trk80_hmt_L1TE5", "HLT_mb_sp1200_trk80_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_trk90_hmt_L1TE5", "HLT_mb_sp1200_trk90_hmt_L1TE5.0ETA24", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE15.0ETA24", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE30", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE40", "HLT_mb_sp1400_trk90_hmt_L1TE15.0ETA24", "HLT_mb_sp1400_trk90_hmt_L1TE30", "HLT_mb_sp1400_trk90_hmt_L1TE40", "HLT_mb_sp1600_hmtperf_L1TE15.0ETA24", "HLT_mb_sp1600_hmtperf_L1TE20", "HLT_mb_sp1600_hmtperf_L1TE20.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE10", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE10.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE15.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE20", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE20.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE25.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE30", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE40", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE5", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE50", "HLT_mb_sp1600_trk100_hmt_L1TE15.0ETA24", "HLT_mb_sp1600_trk100_hmt_L1TE25.0ETA24", "HLT_mb_sp1600_trk100_hmt_L1TE30", "HLT_mb_sp1600_trk100_hmt_L1TE40", "HLT_mb_sp1600_trk100_hmt_L1TE50", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE10", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE20", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE30", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE40", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE50", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE60", "HLT_mb_sp1700_trk110_hmt_L1TE10", "HLT_mb_sp1700_trk110_hmt_L1TE20", "HLT_mb_sp1700_trk110_hmt_L1TE30", "HLT_mb_sp1700_trk110_hmt_L1TE40", "HLT_mb_sp1700_trk110_hmt_L1TE50", "HLT_mb_sp1700_trk110_hmt_L1TE60", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE10", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE10.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE15.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE20", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE20.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE25.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE30", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE30.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE5", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE5.0ETA24", "HLT_mb_sp1800_trk110_hmt_L1TE15.0ETA24", "HLT_mb_sp1800_trk110_hmt_L1TE25.0ETA24", "HLT_mb_sp1800_trk110_hmt_L1TE30", "HLT_mb_sp1800_trk110_hmt_L1TE30.0ETA24", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE10", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE30", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE40", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE50", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE60", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE70", "HLT_mb_sp1900_trk120_hmt_L1TE10", "HLT_mb_sp1900_trk120_hmt_L1TE30", "HLT_mb_sp1900_trk120_hmt_L1TE40", "HLT_mb_sp1900_trk120_hmt_L1TE50", "HLT_mb_sp1900_trk120_hmt_L1TE60", "HLT_mb_sp1900_trk120_hmt_L1TE70", "HLT_mb_sp2100_hmtperf_L1TE20", "HLT_mb_sp2100_hmtperf_L1TE20.0ETA24", "HLT_mb_sp2100_hmtperf_L1TE25.0ETA24", "HLT_mb_sp2100_hmtperf_L1TE30", "HLT_mb_sp2100_hmtperf_L1TE30.0ETA24", "HLT_mb_sp2100_hmtperf_L1TE40", "HLT_mb_sp2100_hmtperf_L1TE40.0ETA24", "HLT_mb_sp2100_pusup750_trk130_hmt_L1TE20", "HLT_mb_sp2100_pusup750_trk130_hmt_L1TE30", "HLT_mb_sp2100_pusup750_trk130_hmt_L1TE40", "HLT_mb_sp2100_pusup750_trk130_hmt_L1TE50", "HLT_mb_sp2100_pusup750_trk130_hmt_L1TE60", "HLT_mb_sp2100_pusup750_trk130_hmt_L1TE70", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE10", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE10.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE15.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE20", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE20.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE25.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE30", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE30.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE5", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE5.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE10", "HLT_mb_sp2100_trk120_hmt_L1TE10.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE15.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE20", "HLT_mb_sp2100_trk120_hmt_L1TE20.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE25.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE30", "HLT_mb_sp2100_trk120_hmt_L1TE30.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE5", "HLT_mb_sp2100_trk120_hmt_L1TE5.0ETA24", "HLT_mb_sp2100_trk130_hmt_L1TE20", "HLT_mb_sp2100_trk130_hmt_L1TE30", "HLT_mb_sp2100_trk130_hmt_L1TE40", "HLT_mb_sp2100_trk130_hmt_L1TE50", "HLT_mb_sp2100_trk130_hmt_L1TE60", "HLT_mb_sp2100_trk130_hmt_L1TE70", "HLT_mb_sp2200_pusup800_trk140_hmt_L1TE20", "HLT_mb_sp2200_pusup800_trk140_hmt_L1TE30", "HLT_mb_sp2200_pusup800_trk140_hmt_L1TE40", "HLT_mb_sp2200_pusup800_trk140_hmt_L1TE50", "HLT_mb_sp2200_pusup800_trk140_hmt_L1TE60", "HLT_mb_sp2200_pusup800_trk140_hmt_L1TE70", "HLT_mb_sp2200_trk140_hmt_L1TE20", "HLT_mb_sp2200_trk140_hmt_L1TE30", "HLT_mb_sp2200_trk140_hmt_L1TE40", "HLT_mb_sp2200_trk140_hmt_L1TE50", "HLT_mb_sp2200_trk140_hmt_L1TE60", "HLT_mb_sp2200_trk140_hmt_L1TE70", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE10", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE10.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE15.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE20", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE20.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE25.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE30", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE30.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE40", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE40.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE5", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE5.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE10", "HLT_mb_sp2300_trk130_hmt_L1TE10.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE15.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE20", "HLT_mb_sp2300_trk130_hmt_L1TE20.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE25.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE30", "HLT_mb_sp2300_trk130_hmt_L1TE30.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE40", "HLT_mb_sp2300_trk130_hmt_L1TE40.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE5", "HLT_mb_sp2300_trk130_hmt_L1TE5.0ETA24", "HLT_mb_sp2400_pusup850_trk150_hmt_L1TE20", "HLT_mb_sp2400_pusup850_trk150_hmt_L1TE30", "HLT_mb_sp2400_pusup850_trk150_hmt_L1TE40", "HLT_mb_sp2400_pusup850_trk150_hmt_L1TE50", "HLT_mb_sp2400_pusup850_trk150_hmt_L1TE60", "HLT_mb_sp2400_pusup850_trk150_hmt_L1TE70", "HLT_mb_sp2400_trk150_hmt_L1TE20", "HLT_mb_sp2400_trk150_hmt_L1TE30", "HLT_mb_sp2400_trk150_hmt_L1TE40", "HLT_mb_sp2400_trk150_hmt_L1TE50", "HLT_mb_sp2400_trk150_hmt_L1TE60", "HLT_mb_sp2400_trk150_hmt_L1TE70", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE10", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE10.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE15.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE20", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE20.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE25.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE30", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE30.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE40", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE40.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE10", "HLT_mb_sp2500_trk140_hmt_L1TE10.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE15.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE20", "HLT_mb_sp2500_trk140_hmt_L1TE20.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE25.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE30", "HLT_mb_sp2500_trk140_hmt_L1TE30.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE40", "HLT_mb_sp2500_trk140_hmt_L1TE40.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE20", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE20.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE25.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE30", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE30.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE40", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE40.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE20", "HLT_mb_sp2700_trk150_hmt_L1TE20.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE25.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE30", "HLT_mb_sp2700_trk150_hmt_L1TE30.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE40", "HLT_mb_sp2700_trk150_hmt_L1TE40.0ETA24", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE25.0ETA24", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE30", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE30.0ETA24", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE40", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE40.0ETA24", "HLT_mb_sp2900_trk160_hmt_L1TE25.0ETA24", "HLT_mb_sp2900_trk160_hmt_L1TE30", "HLT_mb_sp2900_trk160_hmt_L1TE30.0ETA24", "HLT_mb_sp2900_trk160_hmt_L1TE40", "HLT_mb_sp2900_trk160_hmt_L1TE40.0ETA24", "HLT_mb_sp600_pusup300_trk40_hmt_L1TE10", "HLT_mb_sp600_pusup300_trk40_hmt_L1TE5", "HLT_mb_sp600_trk40_hmt_L1TE10", "HLT_mb_sp600_trk40_hmt_L1TE5", "HLT_mb_sp700_pusup350_trk50_hmt_L1TE10", "HLT_mb_sp700_pusup350_trk50_hmt_L1TE20", "HLT_mb_sp700_pusup350_trk50_hmt_L1TE5", "HLT_mb_sp700_trk50_hmt_L1TE10", "HLT_mb_sp700_trk50_hmt_L1TE20", "HLT_mb_sp700_trk50_hmt_L1TE5", "HLT_mb_sp800_hmtperf_L1TE10", "HLT_mb_sp800_hmtperf_L1TE10.0ETA24", "HLT_mb_sp800_hmtperf_L1TE5", "HLT_mb_sp800_hmtperf_L1TE5.0ETA24", "HLT_mb_sp900_pusup400_trk50_hmt_L1TE5", "HLT_mb_sp900_pusup400_trk50_hmt_L1TE5.0ETA24", "HLT_mb_sp900_pusup400_trk60_hmt_L1TE10", "HLT_mb_sp900_pusup400_trk60_hmt_L1TE20", "HLT_mb_sp900_pusup400_trk60_hmt_L1TE5.0ETA24", "HLT_mb_sp900_trk50_hmt_L1TE5", "HLT_mb_sp900_trk50_hmt_L1TE5.0ETA24", "HLT_mb_sp900_trk60_hmt_L1TE20", "HLT_mb_sptrk_pt2_L1MBTS_2", "HLT_mb_sptrk_trk80_L1MBTS_2", "HLT_mb_sptrk_vetombts2in_L1ZDC_AND", "HLT_mb_sptrk_vetombts2in_L1ZDC_A_C", "HLT_mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE10", "HLT_mu4_mb_sp1100_trk70_hmt_L1MU4_TE10", "HLT_mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE10", "HLT_mu4_mb_sp1200_trk80_hmt_L1MU4_TE10", "HLT_mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE10", "HLT_mu4_mb_sp1400_trk90_hmt_L1MU4_TE10", "HLT_mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE10", "HLT_mu4_mb_sp1600_trk100_hmt_L1MU4_TE10", "HLT_mu4_mb_sp600_pusup300_trk40_hmt_L1MU4_TE10", "HLT_mu4_mb_sp600_trk40_hmt_L1MU4_TE10", "HLT_mu4_mb_sp700_pusup350_trk50_hmt_L1MU4_TE10", "HLT_mu4_mb_sp700_trk50_hmt_L1MU4_TE10", "HLT_noalg_mb_L1EM3", "HLT_noalg_mb_L1J12", "HLT_noalg_mb_L1MU4", "HLT_noalg_mb_L1TE60", 
"HLT_beamspot_activeTE_trkfast_peb_L1MBTS_1_VTE70", "HLT_beamspot_allTE_trkfast_peb_L1MBTS_1_VTE70", "HLT_beamspot_trkFS_trkfast_L1MBTS_1_VTE70", "HLT_idcalib_trk9_central_L1MBTS_1_VTE70", "HLT_idcalib_trk9_fwd_L1MBTS_1_VTE70", "HLT_mb_sp_L1MBTS_1_OVERLAY", "HLT_mb_sptrk_L1MBTS_1", "HLT_mb_sptrk_L1MBTS_1_OVERLAY", "HLT_mb_sptrk_L1MBTS_1_VTE10", "HLT_mb_sptrk_L1MBTS_1_VTE2", "HLT_mb_sptrk_L1MBTS_1_VTE3", "HLT_mb_sptrk_L1MBTS_1_VTE4", "HLT_mb_sptrk_L1MBTS_1_VTE5", "HLT_mb_sptrk_L1MBTS_2_VTE10", "HLT_mb_sptrk_L1MBTS_2_VTE2", "HLT_mb_sptrk_L1MBTS_2_VTE3", "HLT_mb_sptrk_L1MBTS_2_VTE4", "HLT_mb_sptrk_L1MBTS_2_VTE5", "HLT_noalg_L1MBTS_1_OVERLAY", 
"HLT_mb_sptrk_L1VTE10", "HLT_mb_sptrk_L1VTE2", "HLT_mb_sptrk_L1VTE3", "HLT_mb_sptrk_L1VTE4", "HLT_mb_sptrk_L1VTE5", "HLT_noalg_L1TE2", "HLT_noalg_L1TE20_OVERLAY", "HLT_noalg_L1TE4", "HLT_noalg_mb_L1TE2", "HLT_noalg_mb_L1TE4",
          "HLT_2g3_loose_L12EM3_VTE50", "HLT_2g3_loose_dPhi15_L12EM3_VTE50", "HLT_2g3_medium_dPhi15_L12EM3_VTE50", "HLT_2j10_320eta490_L1TE5", "HLT_2j20_320eta490_L1TE10", "HLT_2j30_320eta490_L1TE20", "HLT_2j40_320eta490_L1TE30", "HLT_j10_0eta490_L1TE5", "HLT_j10_L1TE5", "HLT_j15_320eta490_L1TE5", "HLT_j20_0eta490_L1TE10", "HLT_j20_0eta490_L1TE5", "HLT_j20_L1TE10", "HLT_j20_L1TE5", "HLT_j30_0eta490_L1TE10", "HLT_j30_L1TE10", "HLT_j35_320eta490_L1TE20", "HLT_j35_bmv2c1070_split_0eta490_L1TE10", "HLT_j40_0eta490_L1TE20", "HLT_j40_L1TE20", "HLT_j45_320eta490_L1TE30", "HLT_mb_sp1100_pusup450_trk70_hmt_L1TE15", "HLT_mb_sp1100_pusup450_trk70_hmt_L1TE25", "HLT_mb_sp1100_trk70_hmt_L1TE15", "HLT_mb_sp1100_trk70_hmt_L1TE25", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE15", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE25", "HLT_mb_sp1200_trk80_hmt_L1TE15", "HLT_mb_sp1200_trk80_hmt_L1TE25", "HLT_mb_sp1400_trk90_hmt_L1TE25", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE25", "HLT_mb_sp1600_trk100_hmt_L1TE25", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE15", "HLT_mb_sp1700_pusup650_trk110_hmt_L1TE25", "HLT_mb_sp1700_trk110_hmt_L1TE15", "HLT_mb_sp1700_trk110_hmt_L1TE25", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE15", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE25", "HLT_mb_sp1900_trk120_hmt_L1TE15", "HLT_mb_sp1900_trk120_hmt_L1TE25", "HLT_mb_sp700_pusup350_trk50_hmt_L1TE15", "HLT_mb_sp700_trk50_hmt_L1TE15", "HLT_mb_sp900_pusup400_trk60_hmt_L1TE15", "HLT_mb_sp900_trk60_hmt_L1TE15", "HLT_mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1100_trk70_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1100_trk70_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE40", "HLT_mu4_mb_sp1200_trk80_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1200_trk80_hmt_L1MU4_TE40", "HLT_mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE40", "HLT_mu4_mb_sp1400_trk90_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1400_trk90_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1400_trk90_hmt_L1MU4_TE40", "HLT_mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE40", "HLT_mu4_mb_sp1600_trk100_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1600_trk100_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1600_trk100_hmt_L1MU4_TE40", "HLT_mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE40", "HLT_mu4_mb_sp1700_trk110_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1700_trk110_hmt_L1MU4_TE30", "HLT_mu4_mb_sp1700_trk110_hmt_L1MU4_TE40",
          "HLT_2mu4_nomucomb", "HLT_2mu6_nomucomb", "HLT_j110_mb_sp1100_pusup450_trk70_hmt", "HLT_j110_mb_sp1100_trk70_hmt", "HLT_j110_mb_sp1200_pusup500_trk80_hmt", "HLT_j110_mb_sp1200_trk80_hmt", "HLT_j110_mb_sp1400_pusup550_trk90_hmt", "HLT_j110_mb_sp1400_trk90_hmt", "HLT_j110_mb_sp1600_pusup600_trk100_hmt", "HLT_j110_mb_sp1600_trk100_hmt", "HLT_j110_mb_sp600_pusup300_trk40_hmt", "HLT_j110_mb_sp600_trk40_hmt", "HLT_j110_mb_sp700_pusup350_trk50_hmt", "HLT_j110_mb_sp700_trk50_hmt", "HLT_j110_mb_sp900_pusup400_trk60_hmt", "HLT_j110_mb_sp900_trk60_hmt", "HLT_j40_mb_sp1100_pusup450_trk70_hmt", "HLT_j40_mb_sp1100_trk70_hmt", "HLT_j40_mb_sp1200_pusup500_trk80_hmt", "HLT_j40_mb_sp1200_trk80_hmt", "HLT_j40_mb_sp1400_pusup550_trk90_hmt", "HLT_j40_mb_sp1400_trk90_hmt", "HLT_j40_mb_sp1600_pusup600_trk100_hmt", "HLT_j40_mb_sp1600_trk100_hmt", "HLT_j40_mb_sp600_pusup300_trk40_hmt", "HLT_j40_mb_sp600_trk40_hmt", "HLT_j40_mb_sp700_pusup350_trk50_hmt", "HLT_j40_mb_sp700_trk50_hmt", "HLT_j40_mb_sp900_pusup400_trk60_hmt", "HLT_j40_mb_sp900_trk60_hmt", "HLT_j50_mb_sp1100_pusup450_trk70_hmt", "HLT_j50_mb_sp1100_trk70_hmt", "HLT_j50_mb_sp1200_pusup500_trk80_hmt", "HLT_j50_mb_sp1200_trk80_hmt", "HLT_j50_mb_sp1400_pusup550_trk90_hmt", "HLT_j50_mb_sp1400_trk90_hmt", "HLT_j50_mb_sp1600_pusup600_trk100_hmt", "HLT_j50_mb_sp1600_trk100_hmt", "HLT_j50_mb_sp600_pusup300_trk40_hmt", "HLT_j50_mb_sp600_trk40_hmt", "HLT_j50_mb_sp700_pusup350_trk50_hmt", "HLT_j50_mb_sp700_trk50_hmt", "HLT_j50_mb_sp900_pusup400_trk60_hmt", "HLT_j50_mb_sp900_trk60_hmt", "HLT_j60_mb_sp1100_pusup450_trk70_hmt", "HLT_j60_mb_sp1100_trk70_hmt", "HLT_j60_mb_sp1200_pusup500_trk80_hmt", "HLT_j60_mb_sp1200_trk80_hmt", "HLT_j60_mb_sp1400_pusup550_trk90_hmt", "HLT_j60_mb_sp1400_trk90_hmt", "HLT_j60_mb_sp1600_pusup600_trk100_hmt", "HLT_j60_mb_sp1600_trk100_hmt", "HLT_j60_mb_sp600_pusup300_trk40_hmt", "HLT_j60_mb_sp600_trk40_hmt", "HLT_j60_mb_sp700_pusup350_trk50_hmt", "HLT_j60_mb_sp700_trk50_hmt",  "HLT_j75_mb_sp1100_pusup450_trk70_hmt", "HLT_j75_mb_sp1100_trk70_hmt", "HLT_j75_mb_sp1200_pusup500_trk80_hmt", "HLT_j75_mb_sp1200_trk80_hmt", "HLT_j75_mb_sp1400_pusup550_trk90_hmt", "HLT_j75_mb_sp1400_trk90_hmt", "HLT_j75_mb_sp1600_pusup600_trk100_hmt", "HLT_j75_mb_sp1600_trk100_hmt", "HLT_j75_mb_sp600_pusup300_trk40_hmt", "HLT_j75_mb_sp600_trk40_hmt", "HLT_j75_mb_sp700_pusup350_trk50_hmt", "HLT_j75_mb_sp700_trk50_hmt", "HLT_j75_mb_sp900_pusup400_trk60_hmt", "HLT_j75_mb_sp900_trk60_hmt", "HLT_j85_mb_sp1100_pusup450_trk70_hmt", "HLT_j85_mb_sp1100_trk70_hmt", "HLT_j85_mb_sp1200_pusup500_trk80_hmt", "HLT_j85_mb_sp1200_trk80_hmt", "HLT_j85_mb_sp1400_pusup550_trk90_hmt", "HLT_j85_mb_sp1400_trk90_hmt", "HLT_j85_mb_sp1600_pusup600_trk100_hmt", "HLT_j85_mb_sp1600_trk100_hmt", "HLT_j85_mb_sp600_pusup300_trk40_hmt", "HLT_j85_mb_sp600_trk40_hmt", "HLT_j85_mb_sp700_pusup350_trk50_hmt", "HLT_j85_mb_sp700_trk50_hmt", "HLT_j85_mb_sp900_pusup400_trk60_hmt", "HLT_j85_mb_sp900_trk60_hmt",
          ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled mb'), 
    }])))

HLT_list=[
    "HLT_2e15_lhloose_L12EM12", "HLT_2e15_lhloose_nod0_L12EM12", "HLT_2e15_loose_L12EM12", "HLT_2g10_loose_L12EM7", "HLT_2g15_loose_L12EM7", "HLT_3j50_L13J15", "HLT_3j60_L13J15", "HLT_3j75_L13J15", "HLT_4j50_L13J15", "HLT_4j60_L13J15", "HLT_4j75_L14J20", "HLT_e10_lhloose_L1EM7", "HLT_e10_loose_L1EM7", "HLT_e15_etcut_L1EM12", "HLT_e15_lhloose_L1EM12", "HLT_e15_lhloose_L1EM7", "HLT_e15_lhloose_nod0_L1EM12", "HLT_e15_lhloose_nod0_L1EM7", "HLT_e15_lhmedium_L1EM12", "HLT_e15_lhmedium_nod0_L1EM12", "HLT_e15_lhvloose_nod0_L1EM12", "HLT_e15_loose_L1EM12", "HLT_e15_loose_L1EM7", "HLT_e20_lhloose_L1EM12", "HLT_e20_etcut", "HLT_e20_etcut_L1EM15", "HLT_e20_lhloose_L1EM15", "HLT_e20_lhloose_nod0_L1EM15", "HLT_e20_lhvloose", "HLT_e20_lhvloose_L1EM12", "HLT_e20_loose_L1EM15", "HLT_e20_vloose", "HLT_e20_vloose_L1EM12", "HLT_e24_etcut_L1EM15VH", "HLT_e24_lhvloose_L1EM15VH", "HLT_e24_lhvloose_nod0_L1EM15VH", "HLT_e24_vloose_L1EM15VH", "HLT_g15_etcut_L1EM7", "HLT_g20_etcut", "HLT_g20_etcut_L1EM15", "HLT_g20_medium", "HLT_g20_medium_L1EM15", "HLT_g20_tight", "HLT_g20_tight_L1EM15", "HLT_g25_etcut_L1EM15", "HLT_g25_medium_L1EM15", "HLT_g25_tight_L1EM15", "HLT_g30_etcut_L1EM15", "HLT_j0_perf_ds1_L1J15", "HLT_j0_perf_ds1_L1J20", "HLT_j0_perf_ds1_L1J30", "HLT_j100_L1J30", "HLT_j120_L1J30", "HLT_j360_a10_lcw_subjes_L1J100", "HLT_j40_bmv2c1070_split_L1J12", "HLT_j45_200eta320_L1J12", "HLT_j45_L1J12", "HLT_j45_gsc60_bmv2c1085_split_L1J12", "HLT_j50_L1J12", "HLT_j50_L1J15", "HLT_j50_bmv2c1077_split_L1J12", "HLT_j55_gsc75_bmv2c1085_split_L1J15", "HLT_j60_200eta320_L1J20", "HLT_j60_L1J15", "HLT_j85_gsc100_bmv2c1085_split_L1J20", "HLT_j60_gsc85_bmv2c1085_split_L1J20", "HLT_j75_200eta320_L1J20", "HLT_j75_320eta490", "HLT_j75_L1J20", "HLT_mu10_ivarloose", "HLT_mu15", "HLT_mu16", "HLT_mu18", "HLT_mu18_L1MU10", "HLT_mu4_j40_dr05", "HLT_mu4_j50_dr05", "HLT_mu4_j60_dr05", "HLT_mu4_mu2noL1_L1MU4", "HLT_mu6_ivarloose", "HLT_mu6_j30_dr05", "HLT_mu6_mu2noL1_L1MU6", "HLT_mu8", 
    ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled 13TeV low-mu or 5TeV'), 
    }])))

HLT_list=[
    "HLT_2mu4", "HLT_2mu6", "HLT_g30_loose_L1EM15", "HLT_j100_L1J20", "HLT_j150_L1J40", "HLT_j25_320eta490_L1TE10", "HLT_j30_0eta490_L1TE20", "HLT_j30_L1TE20", "HLT_j50_200eta320_L1J12", "HLT_j55_320eta490", "HLT_j60_mb_sp900_pusup400_trk60_hmt", "HLT_j60_mb_sp900_trk60_hmt", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE10", "HLT_mb_sp1200_trk80_hmt_L1TE10", "HLT_mb_sp1600_trk100_hmt_L1TE15", "HLT_mb_sp1900_pusup700_trk120_hmt_L1TE20", "HLT_mb_sp1900_trk120_hmt_L1TE20", "HLT_mb_sp900_pusup400_trk60_hmt_L1TE5", "HLT_mb_sp900_trk60_hmt_L1TE5", "HLT_mu10", "HLT_mu15_L1MU10", "HLT_mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE20", "HLT_mu4_mb_sp1200_trk80_hmt_L1MU4_TE20", "HLT_mu4_mb_sp900_pusup400_trk60_hmt_L1MU4_TE10", "HLT_noalg_L12EM7",
    "HLT_mu4_mb_sp900_trk60_hmt_L1MU4_TE10", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE15",
]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, ESValue=0, comment='Disabled 13TeV low-mu or 5TeV'), 
    }])))

# --------------------------------------
# L1Topo seeded
# need to be all updated.

HLT_list=[
    'HLT_2j40_0eta490_invm400',
 'HLT_mu11_2mu4noL1_nscan03_L1MU11_LFV-MU',
 'HLT_2j55_bloose_L13J20_4J20.0ETA49_MJJ-400',
 'HLT_2j55_bloose_L13J20_4J20.0ETA49_MJJ-700',
 'HLT_2j55_bloose_L13J20_4J20.0ETA49_MJJ-800',
 'HLT_2j55_bloose_L13J20_4J20.0ETA49_MJJ-900',
 'HLT_2j55_bloose_L1J30_2J20_4J20.0ETA49_MJJ-400',
 'HLT_2j55_bloose_L1J30_2J20_4J20.0ETA49_MJJ-700',
 'HLT_2j55_bloose_L1J30_2J20_4J20.0ETA49_MJJ-800',
 'HLT_2j55_bloose_L1J30_2J20_4J20.0ETA49_MJJ-900',
 'HLT_2j55_bmedium_L13J20_4J20.0ETA49_MJJ-400',
 'HLT_2j55_bmedium_L13J20_4J20.0ETA49_MJJ-700',
 'HLT_2j55_bmedium_L13J20_4J20.0ETA49_MJJ-800',
 'HLT_2j55_bmedium_L13J20_4J20.0ETA49_MJJ-900',
 'HLT_2j55_bmedium_L1J30_2J20_4J20.0ETA49_MJJ-400',
 'HLT_2j55_bmedium_L1J30_2J20_4J20.0ETA49_MJJ-700',
 'HLT_2j55_bmedium_L1J30_2J20_4J20.0ETA49_MJJ-800',
 'HLT_2j55_bmedium_L1J30_2J20_4J20.0ETA49_MJJ-900',
 'HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-400',
 'HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-700',
 'HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-800',
 'HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-900',
 'HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-400',
 'HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-700',
 'HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-800',
 'HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-900',
 'HLT_2mu4_bBmumu_L1BPH-2M-2MU4',
 'HLT_2mu4_bBmumu_L1BPH-2M-2MU4-B',
 'HLT_2mu4_bBmumu_L1BPH-2M-2MU4-BO',
 'HLT_2mu4_bBmumu_L1BPH-4M8-2MU4',
 'HLT_2mu4_bBmumu_L1BPH-4M8-2MU4-B',
 'HLT_2mu4_bBmumu_L1BPH-4M8-2MU4-BO',
 'HLT_2mu4_bBmumu_L1BPH-DR-2MU4',
 'HLT_2mu4_bBmumu_L1BPH-DR-2MU4-B',
 'HLT_2mu4_bBmumu_L1BPH-DR-2MU4-BO',
 'HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-DR-2MU4',
 'HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-DR-2MU4-B',
 'HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-DR-2MU4-BO',
 'HLT_2mu4_bBmumuxv2_L1BPH-2M-2MU4',
 'HLT_2mu4_bBmumuxv2_L1BPH-2M-2MU4-B',
 'HLT_2mu4_bBmumuxv2_L1BPH-2M-2MU4-BO',
 'HLT_2mu4_bBmumuxv2_L1BPH-4M8-2MU4',
 'HLT_2mu4_bBmumuxv2_L1BPH-4M8-2MU4-B',
 'HLT_2mu4_bBmumuxv2_L1BPH-4M8-2MU4-BO',
 'HLT_2mu4_bBmumuxv2_L1BPH-DR-2MU4',
 'HLT_2mu4_bBmumuxv2_L1BPH-DR-2MU4-B',
 'HLT_2mu4_bBmumuxv2_L1BPH-DR-2MU4-BO',
 'HLT_2mu4_bDimu_L1BPH-2M-2MU4',
 'HLT_2mu4_bDimu_L1BPH-2M-2MU4-B',
 'HLT_2mu4_bDimu_L1BPH-2M-2MU4-BO',
 'HLT_2mu4_bDimu_L1BPH-4M8-2MU4',
 'HLT_2mu4_bDimu_L1BPH-4M8-2MU4-B',
 'HLT_2mu4_bDimu_L1BPH-4M8-2MU4-BO',
 'HLT_2mu4_bDimu_L1BPH-DR-2MU4',
 'HLT_2mu4_bDimu_L1BPH-DR-2MU4-B',
 'HLT_2mu4_bDimu_L1BPH-DR-2MU4-BO',
 'HLT_2mu4_bDimu_L1BPH-DR-2MU4-BPH-2M-2MU4',
 'HLT_2mu4_bDimu_L1BPH-DR-2MU4-BPH-4M8-2MU4',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-2M-2MU4',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-2M-2MU4-B',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-2M-2MU4-BO',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-4M8-2MU4',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-4M8-2MU4-B',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-4M8-2MU4-BO',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-DR-2MU4',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-DR-2MU4-B',
 'HLT_2mu4_bDimu_novtx_noos_L1BPH-DR-2MU4-BO',
 'HLT_2mu4_bJpsimumu_L1BPH-2M-2MU4',
 'HLT_2mu4_bJpsimumu_L1BPH-2M-2MU4-B',
 'HLT_2mu4_bJpsimumu_L1BPH-2M-2MU4-BO',
 'HLT_2mu4_bJpsimumu_L1BPH-DR-2MU4',
 'HLT_2mu4_bJpsimumu_L1BPH-DR-2MU4-B',
 'HLT_2mu4_bJpsimumu_L1BPH-DR-2MU4-BO',
 'HLT_2mu4_bUpsimumu_L1BPH-2M-2MU4',
 'HLT_2mu4_bUpsimumu_L1BPH-2M-2MU4-B',
 'HLT_2mu4_bUpsimumu_L1BPH-2M-2MU4-BO',
 'HLT_2mu6_bBmumu_L1BPH-2M-2MU6',
 'HLT_2mu6_bBmumu_L1BPH-4M8-2MU6',
 'HLT_2mu6_bBmumu_L1BPH-DR-2MU6',
 'HLT_2mu6_bBmumux_BcmumuDsloose_L1BPH-4M8-2MU6',
 'HLT_2mu6_bBmumux_BcmumuDsloose_L1BPH-DR-2MU6',
 'HLT_2mu6_bBmumuxv2_L1BPH-4M8-2MU6',
 'HLT_2mu6_bBmumuxv2_L1BPH-DR-2MU6',
 'HLT_2mu6_bDimu_L1BPH-2M-2MU6',
 'HLT_2mu6_bDimu_L1BPH-4M8-2MU6',
 'HLT_2mu6_bDimu_L1BPH-DR-2MU6',
 'HLT_2mu6_bDimu_novtx_noos_L1BPH-2M-2MU6',
 'HLT_2mu6_bDimu_novtx_noos_L1BPH-4M8-2MU6',
 'HLT_2mu6_bDimu_novtx_noos_L1BPH-DR-2MU6',
 'HLT_2mu6_bJpsimumu_L1BPH-2M-2MU6',
 'HLT_2mu6_bJpsimumu_L1BPH-DR-2MU6',
 'HLT_2mu6_bUpsimumu_L1BPH-2M-2MU6',
 'HLT_e13_etcut_L1EM10_W-MT25',
 'HLT_e13_etcut_L1EM10_W-MT30',
 'HLT_e13_etcut_L1W-NOMATCH',
 'HLT_e13_etcut_L1W-NOMATCH_W-05RO-XEEMHT',
 'HLT_e13_etcut_trkcut_L1EM10_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE',
 'HLT_e13_etcut_trkcut_L1EM10_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20_xe20_mt25',
 'HLT_e13_etcut_trkcut_j20_perf_xe15_6dphi05_mt25_L1EM10_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_W-90RO2-XEHT-0',
 'HLT_e13_etcut_trkcut_j20_perf_xe15_6dphi05_mt25_L1EM10_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20',
 'HLT_e13_etcut_trkcut_xe20_L1EM10_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20',
 'HLT_e13_etcut_trkcut_xs15_L1EM10_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20',
 'HLT_e14_etcut_e5_lhtight_Jpsiee_L1JPSI-1M5-EM12',
 'HLT_e14_etcut_e5_lhtight_nod0_Jpsiee_L1JPSI-1M5-EM12',
 'HLT_e14_etcut_e5_tight_Jpsiee_L1JPSI-1M5-EM12',
 'HLT_e14_lhtight_e4_etcut_Jpsiee_L1JPSI-1M5-EM12',
 'HLT_e14_tight_e4_etcut_Jpsiee_L1JPSI-1M5-EM12',
 'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
 'HLT_e17_lhmedium_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25',
 'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25',
 'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_L1EM15-TAU12I',
 'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50_L1XE35_EM15-TAU12I',
 'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50_L1XE40_EM15-TAU12I',
 'HLT_e17_lhmedium_nod0_tau80_medium1_tracktwo_L1EM15-TAU40',
 'HLT_e17_lhmedium_tau25_medium1_tracktwo_L1EM15-TAU12I',
 'HLT_e17_lhmedium_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
 'HLT_e17_lhmedium_tau25_medium1_tracktwo_xe50_L1XE35_EM15-TAU12I',
 'HLT_e17_lhmedium_tau25_medium1_tracktwo_xe50_L1XE40_EM15-TAU12I',
 'HLT_e17_lhmedium_tau80_medium1_tracktwo_L1EM15-TAU40',
 'HLT_e17_medium_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25',
 'HLT_e17_medium_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25',
 'HLT_e17_medium_tau25_medium1_tracktwo_L1EM15-TAU12I',
 'HLT_e17_medium_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
 'HLT_e17_medium_tau25_medium1_tracktwo_xe50_L1XE35_EM15-TAU12I',
 'HLT_e17_medium_tau25_medium1_tracktwo_xe50_L1XE40_EM15-TAU12I',
 'HLT_e17_medium_tau80_medium1_tracktwo_L1EM15-TAU40',
 'HLT_e18_etcut_L1EM15_W-MT35',
 'HLT_e18_etcut_trkcut_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EMXE',
 'HLT_e18_etcut_trkcut_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EMXE_XS30_xe35_mt35',
 'HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EMXE_W-250RO2-XEHT-0',
 'HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EMXE_XS30',
 'HLT_e18_etcut_trkcut_xe35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EMXE_XS30',
 'HLT_e5_etcut_L1W-05DPHI-EMXE-1',
 'HLT_e5_etcut_L1W-05DPHI-JXE-0',
 'HLT_e5_etcut_L1W-05RO-XEHT-0',
 'HLT_e5_etcut_L1W-10DPHI-EMXE-0',
 'HLT_e5_etcut_L1W-10DPHI-JXE-0',
 'HLT_e5_etcut_L1W-15DPHI-EMXE-0',
 'HLT_e5_etcut_L1W-15DPHI-JXE-0',
 'HLT_e5_etcut_L1W-250RO2-XEHT-0',
 'HLT_e5_etcut_L1W-90RO2-XEHT-0',
 'HLT_e5_etcut_L1W-HT20-JJ15.ETA49',
 'HLT_e5_lhtight_e4_etcut_Jpsiee_L1JPSI-1M5',
 'HLT_e5_lhtight_e4_etcut_L1JPSI-1M5',
 'HLT_e5_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5',
 'HLT_e5_lhtight_nod0_e4_etcut_L1JPSI-1M5',
 'HLT_e5_tight_e4_etcut_Jpsiee_L1JPSI-1M5',
 'HLT_e5_tight_e4_etcut_L1JPSI-1M5',
 'HLT_e9_etcut_e5_lhtight_Jpsiee_L1JPSI-1M5-EM7',
 'HLT_e9_etcut_e5_lhtight_nod0_Jpsiee_L1JPSI-1M5-EM7',
 'HLT_e9_etcut_e5_tight_Jpsiee_L1JPSI-1M5-EM7',
 'HLT_e9_lhtight_e4_etcut_Jpsiee_L1JPSI-1M5-EM7',
 'HLT_e9_tight_e4_etcut_Jpsiee_L1JPSI-1M5-EM7',
 'HLT_g10_etcut_mu10_L1LFV-EM8I',
 'HLT_g10_etcut_mu10_iloose_taumass_L1LFV-EM8I',
    'HLT_g10_loose_mu10_ivarloose_taumass_L1LFV-EM8I',
    'HLT_g20_loose_mu4_ivarloose_taumass_L1LFV-EM15I',
 'HLT_g20_etcut_mu4_L1LFV-EM15I',
 'HLT_g20_etcut_mu4_iloose_taumass_L1LFV-EM15I',
 'HLT_g20_loose_2j40_0eta490_3j25_0eta490_L1MJJ-700',
 'HLT_g20_loose_2j40_0eta490_3j25_0eta490_L1MJJ-900',
 'HLT_ht400_L1HT150-J20.ETA31',
 'HLT_ht550_L1HT150-J20.ETA31',
 'HLT_ht700_L1HT190-J15.ETA21',
 'HLT_ht850_L1HT190-J15.ETA21',
 'HLT_j100_xe80_L1J40_DPHI-CJ20XE50',
 'HLT_j100_xe80_L1J40_DPHI-J20XE50',
 'HLT_j100_xe80_L1J40_DPHI-J20s2XE50',
 'HLT_j100_xe80_L1J40_DPHI-Js2XE50',
 'HLT_j360_a10_lcw_nojcalib_L1HT150-J20.ETA31',
 'HLT_j360_a10_lcw_sub_L1HT150-J20.ETA31',
 'HLT_j360_a10_nojcalib_L1HT150-J20.ETA31',
 'HLT_j360_a10_sub_L1HT150-J20.ETA31',
 'HLT_j460_a10_lcw_nojcalib_L1HT190-J15.ETA21',
    'HLT_j460_a10_lcw_sub_L1HT190-J15.ETA21',
    'HLT_j460_a10_lcw_subjes_L1HT190-J15.ETA21',
    'HLT_j360_a10_lcw_subjes_L1HT150-J20.ETA31',
    'HLT_j460_a10_lcw_subjes_L1HT190-J15s5.ETA21',
 'HLT_j460_a10_nojcalib_L1HT190-J15.ETA21',
 'HLT_j460_a10_sub_L1HT190-J15.ETA21',
 'HLT_j80_xe80_1dphi10_L1J40_DPHI-CJ20XE50',
 'HLT_j80_xe80_1dphi10_L1J40_DPHI-J20XE50',
 'HLT_j80_xe80_1dphi10_L1J40_DPHI-J20s2XE50',
 'HLT_j80_xe80_1dphi10_L1J40_DPHI-Js2XE50',
 'HLT_mu14_tau25_medium1_tracktwo_L1DR-MU10TAU12I',
 'HLT_mu14_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12-J25',
 'HLT_mu4_iloose_mu4_11invm60_noos_L1DY-BOX-2MU4',
 'HLT_mu4_iloose_mu4_11invm60_noos_novtx_L1DY-BOX-2MU4',
 'HLT_mu4_iloose_mu4_7invm9_noos_L1DY-BOX-2MU4',
 'HLT_mu4_iloose_mu4_7invm9_noos_novtx_L1DY-BOX-2MU4',
 'HLT_mu4_j60_dr05_2j35_L13J15_BTAG-MU4J30',
 'HLT_mu4_j70_dr05_L1BTAG-MU4J30',
 'HLT_mu6_j50_dr05_2j35_L13J15_BTAG-MU6J25',
 'HLT_mu6_j60_dr05_L1BTAG-MU6J25',
 'HLT_mu6_mu4_bBmumu_L1BPH-2M-MU6MU4',
 'HLT_mu6_mu4_bBmumu_L1BPH-2M-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumu_L1BPH-4M8-MU6MU4',
 'HLT_mu6_mu4_bBmumu_L1BPH-4M8-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumu_L1BPH-DR-MU6MU4',
 'HLT_mu6_mu4_bBmumu_L1BPH-DR-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M-MU6MU4-B',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-4M8-MU6MU4-B',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-4M8-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-DR-MU6MU4-B',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-DR-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumuxv2_L1BPH-2M-MU6MU4',
 'HLT_mu6_mu4_bBmumuxv2_L1BPH-2M-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumuxv2_L1BPH-4M8-MU6MU4',
 'HLT_mu6_mu4_bBmumuxv2_L1BPH-4M8-MU6MU4-BO',
 'HLT_mu6_mu4_bBmumuxv2_L1BPH-DR-MU6MU4',
 'HLT_mu6_mu4_bBmumuxv2_L1BPH-DR-MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_L1BPH-2M-MU6MU4',
 'HLT_mu6_mu4_bDimu_L1BPH-2M-MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_L1BPH-4M8-MU6MU4',
 'HLT_mu6_mu4_bDimu_L1BPH-4M8-MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_L1BPH-BPH-DR-MU6MU4',
 'HLT_mu6_mu4_bDimu_L1BPH-BPH-DR-MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-2M-MU6MU4-B',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-2M-MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-4M8-MU6MU4-B',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-4M8-MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-DR-MU6MU4-B',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-DR-MU6MU4-BO',
 'HLT_mu6_mu4_bJpsimumu_L1BPH-2M-MU6MU4',
 'HLT_mu6_mu4_bJpsimumu_L1BPH-2M-MU6MU4-BO',
 'HLT_mu6_mu4_bJpsimumu_L1BPH-DR-MU6MU4',
 'HLT_mu6_mu4_bJpsimumu_L1BPH-DR-MU6MU4-BO',
 'HLT_mu6_mu4_bUpsimumu_L1BPH-2M-MU6MU4',
 'HLT_mu6_mu4_bUpsimumu_L1BPH-2M-MU6MU4-BO',
 'HLT_mu6_mu4_bUpsimumu_L1BPH-DR-MU6MU4',
 'HLT_mu6_mu4_bUpsimumu_L1BPH-DR-MU6MU4-BO',
 'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1BOX-TAU20ITAU12I',
 'HLT_mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I',
 'HLT_mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25',
 'HLT_mu14_iloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25',
 'HLT_mu4_j15_gsc35_bperf_split_dr05_dz02_L1BTAG-MU4J15',
 'HLT_mu4_j55_bperf_split_dr05_dz02_L1BTAG-MU4J15',
 'HLT_mu6_j60_gsc110_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j110_gsc150_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j150_gsc175_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j260_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j225_gsc320_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j400_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j60_gsc85_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_e17_lhmedium_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
 'HLT_e17_medium_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi05_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',
 'HLT_e18_etcut_trkcut_xs30_xe30_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',
 'HLT_j360_lcw_L1HT150-JJ15.ETA49',
 'HLT_j460_a10_lcw_L1HT190-J15.ETA21',
    'HLT_j360_a10_lcw_L1HT150-J20s5.ETA31',
    'HLT_j360_a10_lcw_subjes_L1HT150-J20s5.ETA31',
 'HLT_xe110_L1KF-XE55',
 'HLT_xe110_mht_L1KF-XE55',
 'HLT_xe140_tc_lcw_L1KF-XE55',
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I',
 'HLT_mu4_2j35_dr05_j35_bmv2c1070_split_antimatchdr05mu_L13J15_BTAG-MU4J15',
 'HLT_mu4_2j40_dr05_j40_bmv2c1077_split_antimatchdr05mu_L13J15_BTAG-MU4J15',
 'HLT_j460_a10_lcw_L1HT190-J15s5.ETA21',
 'HLT_mu4_2j35_dr05_j35_bmv2c1070_split_antimatchdr05mu_L13J20_BTAG-MU4J20',
 'HLT_mu4_2j40_dr05_j40_bmv2c1077_split_antimatchdr05mu_L13J20_BTAG-MU4J20',
 'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15-TAU12I',
 'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15-TAU12I',
 'HLT_j360_a10_lcw_L1HT150-J20.ETA31',
 'HLT_mu6_mu4_bUpsimumu_L1MU6MU4-BO',
 'HLT_mu6_mu4_bBmumuxv2_L1MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_L1MU6MU4-BO',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1MU6MU4-BO',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30',
 'HLT_e18_etcut_trkcut_xs30_xe30_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30',


    'HLT_2j45_bmv2c1060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400',
    'HLT_2j55_bmv2c1070_split_ht300_L1HT150-J20.ETA31',
    'HLT_2j55_bmv2c1070_split_ht300_L1HT150-J20s5.ETA31',
    'HLT_g25_medium_L1EM22VHI_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400',
    'HLT_ht300_2j40_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400',
    
    'HLT_j55_bmv2c1060_split_ht500_L1HT150-J20.ETA31',
    'HLT_j55_bmv2c1060_split_ht500_L1HT150-J20s5.ETA31',
    'HLT_mu4_iloose_mu4_11invm24_noos_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_11invm24_noos_L1DY-BOX-2MU4',
    'HLT_mu4_iloose_mu4_11invm24_noos_novtx_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_11invm24_noos_novtx_L1DY-BOX-2MU4',
    'HLT_mu4_iloose_mu4_24invm60_noos_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_24invm60_noos_L1DY-BOX-2MU4',
    'HLT_mu4_iloose_mu4_24invm60_noos_novtx_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_24invm60_noos_novtx_L1DY-BOX-2MU4',
    'HLT_e26_lhtight_nod0_ivarloose_e15_etcut_L1EM7_Zee',
    'HLT_e26_lhtight_nod0_ivarloose_L1EM22VHIM_e15_etcut_L1EM7_Zee',
    'HLT_e28_lhtight_nod0_ivarloose_e15_etcut_L1EM7_Zee',
    'HLT_e28_lhtight_nod0_ivarloose_L1EM24VHIM_e15_etcut_L1EM7_Zee',
 ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled L1Topo'), 
    }])))


#################################################################################################
###3brules####
# doLowLumi
others_Rules_startup_3b=RulebookDict(tag,'others_Rules_startup_3b')

HLT_list=['HLT_noalg_L1LowLumi']
others_Rules_startup_3b.update(dict(map(None,HLT_list,len(HLT_list)*[{ 
                    1 : TriggerRule(PS=5, ESValue=1, comment='for (number of filled bunches=2 (3), L=1e31, rate=20Hz, ES=20Hz') ,
                    200 : TriggerRule(PS=100, ESValue=1, comment='for (number of filled bunches=, L=2e32, rate=20Hz, ES=20Hz') ,
                    500 : TriggerRule(PS=250, ESValue=1, comment='for (number of filled bunches=~50, L=5e32, rate=20Hz, ES=20Hz') ,
                    1000 : TriggerRule(PS=500, ESValue=1, comment='for (number of filled bunches, L=1e33, rate=20Hz, ES=20Hz') ,
                    2000 : TriggerRule(PS=1000, ESValue=1, comment='for (number of filled bunches, L=2e33, rate=20Hz, ES=20Hz') ,
                    4000 : TriggerRule(PS=2000, ESValue=1, comment='for (number of filled bunches=600, L=4e33, rate=20Hz, ES=20Hz') ,
                    6000 : TriggerRule(PS=3500, ESValue=1, comment='for (number of filled bunches=900, L=6e33, rate=10Hz, ES=10Hz') ,
                    9001 : TriggerRule(PS=4500, ESValue=1, comment='for (number of filled bunches=1300, L=9e33, rate=10Hz, ES=10Hz') ,
                    }]))) 

HLT_list=["HLT_tau25_idperf_tracktwo"]
others_Rules_startup_3b.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=100., ESRate=1, rerun=1, comment="for ID Tracks"), 
                    3001 : TriggerRule(rate=0.5, ESValue=1, rerun=1, comment='Support Express'),
                    }])))

L1_list=["L1_TAU12IM",]
others_Rules_startup_3b.update(dict(map(None,L1_list,len(L1_list)*[{
                    1 : TriggerRule(rate=150, comment="for ID Tracks", rerun=1), 
                    }])))

L1_list=["L1_2EM8VH_MU10","L1_2J15_XE55","L1_2MU6","L1_3J25.0ETA23","L1_3J40","L1_3MU4","L1_4J15","L1_EM15VHI_2TAU12IM_J25_3J12","L1_EM15VHI_2TAU12IM_XE35","L1_EM15VHI_TAU40_2TAU15","L1_EM15VH_MU10","L1_J75","L1_J75.31ETA49","L1_MU10_TAU12IM","L1_MU10_TAU12IM_J25_2J12","L1_MU10_TAU12IM_XE35","L1_MU21","L1_MU6_J40","L1_TAU20IM_2TAU12IM_J25_2J20_3J12","L1_TAU20IM_2TAU12IM_XE35","L1_TAU60","L1_XE50",]

others_Rules_startup_3b.update(dict(map(None,L1_list,len(L1_list)*[{
                    1 : TriggerRule(PS=1, comment='L1Seed for noalg_L1LowLumi'),
                    }])))


########

rules = RulebookDict(tag)

# full list
RulesList=[muon_Rules, bphys_Rules, met_Rules, jet_Rules, bjet_Rules, tau_Rules, egamma_Rules, combined_Rules, vbf_Rules, dedicated_Rules, disabled_Rules]

for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    rules.update(Rules)

lumi_Rules = {
	'L1_RD0_BGRP9':	{ 1 : TriggerRule(rate=10000, scaling='bunches', comment='target 20 kHz, ATR-15089') },
  'HLT_lumipeb_vdm_L1RD0_BGRP9' : {  1 : TriggerRule(PS=1, comment='target 20 kHz'), },
	}
if add2kHxPixel:
    rules.update(lumi_Rules)

afp_Rules = {
        'L1_AFP_A_SPECTOF_BGRP0': { 1000 : TriggerRule(PS=20000, comment='enabled AFP calib+streamers, ATR-15926')},
        'L1_AFP_C_SPECTOF_BGRP0': { 1000 : TriggerRule(PS=20000, comment='enabled AFP calib+streamers, ATR-15926')},
        'L1_AFP_A_AND_C_SPECTOF': { 1000 : TriggerRule(PS=1000, comment='enabled AFP calib+streamers, ATR-15926')},
        'L1_AFP_A_AND_C_SPECTOF_J50': { 1000 : TriggerRule(PS=1, comment='enabled AFP to Main+streamers, ATR-15926')},
        'L1_AFP_A_AND_C_SPECTOF_J75': { 1000 : TriggerRule(PS=1, comment='enabled AFP to Main+streamers, ATR-15926')},
        'L1_AFP_A_AND_C_SPECTOF_J100': { 1000 : TriggerRule(PS=1, comment='enabled AFP to Main+streamers, ATR-15926')},
        'L1_AFP_A_SPECTOF'      : { 1000 : TriggerRule(PS=-1, comment='enabled AFP calib+streamers, ATR-17561')},
        'L1_AFP_C_SPECTOF'      : { 1000 : TriggerRule(PS=-1, comment='enabled AFP calib+streamers, ATR-17561')},

        'HLT_afp_jetexc_L1J50' : { 1000 : TriggerRule(PS=-1, comment='enabled AFP to Main+streamers, ATR-17798')},
        'HLT_afp_jetexc_L1J75' : { 1000 : TriggerRule(PS=-1, comment='enabled AFP to Main+streamers, ATR-17798')},
        'HLT_afp_jetexc_L1J100': { 1000 : TriggerRule(PS=-1, comment='enabled AFP to Main+streamers, ATR-17798')},

        'HLT_calibAFP_L1AFP_A_SPECTOF_BGRP0': { 1000 : TriggerRule(PS=1, comment='enabled AFP calib+streamers, ATR-15926')},
        'HLT_calibAFP_L1AFP_C_SPECTOF_BGRP0': { 1000 : TriggerRule(PS=1, comment='enabled AFP calib+streamers, ATR-15926')},
        'HLT_calibAFP_L1AFP_A_AND_C_SPECTOF': { 1000 : TriggerRule(PS=1, comment='enabled AFP calib+streamers, ATR-15926')},
        'HLT_calibAFP_L1AFP_A_SPECTOF'      : { 1000 : TriggerRule(PS=-1, comment='enabled AFP calib+streamers, ATR-17561')},
        'HLT_calibAFP_L1AFP_C_SPECTOF'      : { 1000 : TriggerRule(PS=-1, comment='enabled AFP calib+streamers, ATR-17561')},
        'HLT_calibAFP_L1RD0_FILLED'         : { 1000 : TriggerRule(PS=-1, comment='enabled AFP calib+streamers, ATR-17561')},

        'HLT_noalg_L1AFP_A_AND_C_SPECTOF_J50': { 1000 : TriggerRule(PS=20, comment='enabled AFP to Main+streamers, ATR-15926')},
        'HLT_noalg_L1AFP_A_AND_C_SPECTOF_J75': { 1000 : TriggerRule(PS=10, comment='enabled AFP to Main+streamers, ATR-15926')},
        'HLT_noalg_L1AFP_A_AND_C_SPECTOF_J100': { 1000 : TriggerRule(PS=5, comment='enabled AFP to Main+streamers, ATR-15926')},
        'HLT_noalg_L1AFP_A_SPECTOF'           : { 1000 : TriggerRule(PS=-1, comment='Support Express, ATR-17561',ESValue=1)},
        'HLT_noalg_L1AFP_C_SPECTOF'           : { 1000 : TriggerRule(PS=-1, comment='Support Express, ATR-17561',ESValue=1)},
        }

if doAFP2017:
    rules.update(afp_Rules,allowDuplicates=True)

# ----------------------------------------

RulesList=[datascouting_Rules]

for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule
    rules.update(Rules)

# ----------------------------------------

RulesList=[zerobias_Rules, tauoverlay_Rules, MinBias_Rules]
for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in rules:
            pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule, "CHECK"
    rules.update(Rules)

RulesList=[l1calo_streamer_Rules, l1topo_streamer_Rules, topo_Rules,]
for Rules in RulesList:
    for newrule in Rules.keys():
        if newrule in rules:
            if 0: pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule, ". For L1Topo, overwritten intentionally."
    rules.update(Rules)

# ----------------------------------------
#lowlumi
if doLowLumi:
    RulesList=[others_Rules_startup_3b]                                        
    for Rules in RulesList:
        for newrule in Rules.keys():
            if newrule in rules:
                if 0: pass # print 'FATAL     Physics_pp_v7_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule, ". For L1Topo, overwritten intentionally."
    rules.update(Rules)
#

# rules.update(Monitoring_pp_v7_rules.physics_rules) #FIXME, should not complain on LAR duplication
            
#################################################################################################
# rules to override already-existing rules on purpose

override_rules = {
    # defined in monitoring rule
    'L1_MU4_UNPAIRED_ISO'                      : {  1000 : TriggerRule(PS=1,    comment='') },
    'L1_TAU8_EMPTY'                            : {  1000 : TriggerRule(PS=1,   scaling='bunches', comment='Support') },
    'L1_TAU8_UNPAIRED_ISO'                     : {  1000 : TriggerRule(PS=1,   scaling='bunches', comment='Support') },
    'L1_EM7_EMPTY'                             : {  1000 : TriggerRule(PS=1,   scaling='bunches', comment='Support') },
    'L1_EM7_UNPAIRED_ISO'                      : {  1000 : TriggerRule(PS=1,   scaling='bunches', comment='Support') },
    'L1_J30_EMPTY'                             : {  1000 : TriggerRule(PS=1,   scaling='bunches', comment='Support') },
		#'HLT_l1calocalib'													 : {  1000 : TriggerRule(rate=2000, comment='2kHz for L1Calo special run') },
    'HLT_noalg_L1J400'                         : { 1 : TriggerRule(PS=1, comment='L1Calo request, moved to Monitoring_pp_v7_rules.py and overwritten in Physics_pp_v7 for ES', ESRate=0.4)}, 
}

rules.update(override_rules,allowDuplicates=True)

if disableBphys:
    L1_list=['L1_MU10_2MU6', 'L1_MU11_2MU6', 'L1_3MU4',
             'L1_BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4-BO', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-8M15-2MU6_BPH-0DR22-2MU6',
             'L1_MU6_3MU4', 'L1_2MU4', 'L1_2MU6', 'L1_MU6_2MU4',]
    rules.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable L1 for 2.2e34 studies'),
                        }])))

if switchToBackup:
    L1_list=['L1_MU20', 'L1_EM24VHI',]
    rules.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable L1 for 2.2e34 studies'),
                        }])))     

if disable3MU4:
    HLT_list=['HLT_3mu4_bNocut', 'HLT_2mu4_mu3_mu2noL1_bNocut_L13MU4', 'HLT_3mu4', 'HLT_3mu4_mu2noL1', 'HLT_3mu3_mu3noL1_L13MU4', 'HLT_3mu4_mu4noL1', 'HLT_3mu4_bDimu', 'HLT_mu6_2mu4_bDimu', 'HLT_3mu3_mu3noL1_bNocut_L13MU4', 'HLT_mu6_2mu4_bNocut_L1MU6_3MU4', 'HLT_2mu6_mu4_bNocut_L12MU6_3MU4', 'HLT_4mu4_bNocut']  
    rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disable 3mu4 chains'),
                        }])))
 
    
lumi_specialrules=RulebookDict(tag,'lumi_specialrules')
# rules for stream_PixelBeam
if doLumiTotalRate:
    HLT_list=['HLT_lumipeb_L1RD0_FILLED']
    lumi_specialrules.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1E-10 : TriggerRule(PS=-1, comment='PixelBeam disabled in cosmic',  rerun=0),
                        2 : TriggerRule(inputRate=rate_HLT_lumipeb_L1RD0_FILLED, comment='PixelBeam ATR-16334',  rerun=0),
                        }])))
    #lumi_specialrules.update({ # defined in L1_RD0_FILLED rule
    #        'L1_RD0_FILLED'  : {2 : TriggerRule(rate=rate_HLT_lumipeb_L1RD0_FILLED, maxRate=rate_HLT_lumipeb_L1RD0_FILLED, scaling='bunches', comment='Support L1') },
    #        })

    if bgrp9_bunches:
        HLT_list=['HLT_lumipeb_vdm_L1RD0_BGRP9']
        lumi_specialrules.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1E-10 : TriggerRule(PS=-1, comment='disabled for 3b and 12b fills; ATR-16334',  rerun=0),
                            2 : TriggerRule(inputRate=500*bgrp9_bunches, comment='500 times BGRP9, for 600b and 900b',  rerun=0),
                            }])))
    rules.update(lumi_specialrules)  

if doL1Calo:
    HLT_list=['HLT_noalg_l1calo_L1J120', 'HLT_noalg_l1calo_L1J10031ETA49', 'HLT_noalg_l1calo_L1J7531ETA49',]
    rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1 : TriggerRule(PS=1, comment='ATR-16446, L1Calo streamer to l1calo stream')
                        }])))  
    
### Remove primaries for luminosities lower than the given value
disableLowerLumiPrimaries(rules,14001)

#################################################################################################
