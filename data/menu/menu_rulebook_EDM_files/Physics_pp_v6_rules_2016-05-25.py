from RuleClasses import TriggerRule
from runOptions import filled_bunches as NumBunches
from UserFunctions import disableLowerLumiPrimaries
from runOptions import doNoL1Topo, do3bunches, add2kHxPixel, emergency2200b, emergency2200b_HLTOnly, startup2200b

tag = 'Physics_pp_v6'

# on/off the HLT chains seeded by L1Topo items
NoHLTTopo=True
NoL1TopoHLTCom=False
# on/off the L1 Topo items used in L1Topo commissioning, also disable L1Topo item set in Monitoring_pp_v6_rules.py
NoL1Topo=False

if doNoL1Topo:
    NoL1Topo=True
    NoHLTTopo=True
    NoL1TopoHLTCom=True
print "NoL1Topo,NoHLTTopo,NoL1TopoHLTCom:", NoL1Topo,NoHLTTopo,NoL1TopoHLTCom
    
#################################################################################################
#
#
# Muon  
#
#################################################################################################


########################################
# 2e34 

muon_Rules_2e34={}

L1_list=['L1_MU20', 'L1_2MU10', 'L1_3MU6', ]
muon_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary L1'),
    }])))

L1_list=['L1_MU10_2MU6',]
muon_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   15001 : TriggerRule(PS=2, comment='Primary L1'),
                   17001 : TriggerRule(PS=10, comment='Support L1'),
    }])))

HLT_list=[ 
    "HLT_mu35_ivarmedium", 
    "HLT_mu80", 
    "HLT_mu100", 
    "HLT_mu24_mu10noL1", 
    "HLT_mu26_mu10noL1", 
    "HLT_mu28_mu8noL1",
    "HLT_2mu15_L12MU10", 
    "HLT_2mu16_L12MU10", 
    "HLT_2mu18_L12MU10",
    "HLT_3mu10_msonly_L13MU6", 
    "HLT_3mu8_msonly",
]
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2017 ATR-15290'), 
                    }])))
HLT_list=[ 
           'HLT_mu60_0eta105_msonly',
           'HLT_mu24_mu8noL1', 'HLT_mu24_2mu4noL1',

           'HLT_mu60',
           'HLT_mu28_ivarmedium', 
           'HLT_mu26_mu8noL1', 
           ]
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

HLT_list=['HLT_mu26_imedium']
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.6, comment='Support Express', ESValue=1),
                    }])))

HLT_list=['HLT_mu26_ivarmedium']
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary', ESRate=0.6), 
                    }])))

HLT_list=['HLT_mu32_ivarmedium']
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary backup ATR-15236'),
                    }])))

HLT_list=['HLT_mu36_ivarmedium','HLT_mu40_ivarmedium','HLT_mu50_ivarmedium']    
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{  
                    1000 : TriggerRule(PS=-1, comment='Primary backup ATR-15236'),  
                    }])))

HLT_list=[ 'HLT_2mu14' ]
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary', ESRate=0.3), 
                    5001 : TriggerRule(PS=1, comment='Primary Express', ESRate=0.3), 
                    }])))

HLT_list=[ 'HLT_2mu14_nomucomb',
           # tighter version
           ]
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[
                { 1000 : TriggerRule(PS=1, comment='Primary'),
                }])))

HLT_list=[ 'HLT_3mu6' ]
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Express', ESRate=0.1), 
                    }])))

HLT_list=['HLT_3mu6_msonly' ]    
muon_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, dedicated trigger LLP, ATR-10933'), 
                    }])))

########################################
# 1.5e34 

muon_Rules_1p5e34={}

HLT_list=[ 'HLT_mu50', 'HLT_mu22_2mu4noL1', 'HLT_mu20_2mu4noL1']
muon_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    17001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_mu22_mu8noL1']
muon_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary', ESRate=0.1), 
                    17001 : TriggerRule(rate=0.1, comment='Support, ES', ESValue=1),
                    }])))


########################################
# 1.0e34 

muon_Rules_1e34={}

# all MU20 seeded
HLT_list=['HLT_mu24_imedium']
muon_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Express', ESRate=0), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_mu24_ivarmedium']
muon_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary', ESRate=0),
                    10001: TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_mu20_mu8noL1', ]
muon_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Backup'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))

L1_list=['L1_MU6_3MU4']
if NoHLTTopo:
    muon_Rules_1e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1, tau->3mu ATR-11754 EndOfFill'),
                        14001 : TriggerRule(PS=5, comment='Support L1, tau->3mu ATR-11754 EndOfFill'),
                        20001 : TriggerRule(PS=-1, comment='Disabled'),
    }])))
    
else:
    muon_Rules_1e34.update(dict(map(None,L1_list,len(L1_list)*[{
        1000 : TriggerRule(PS=1, comment='Primary L1, tau->3mu ATR-11754'),
        10001 : TriggerRule(PS=-1, comment='Disabled'),
    }])))

########################################
# 0.7e34 

muon_Rules_0p7e34={}

HLT_list=['HLT_mu40',]
muon_Rules_0p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    7001: TriggerRule(PS=-1, comment='Disabled'),
                    }])))

########################################
# 0.5e34 

muon_Rules_0p5e34={}

# MU20 seeded. 75Hz at 5e33
HLT_list=['HLT_mu24_ivarloose']
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-13367'), 
                    5001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

L1_list=['L1_MU15']
muon_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(rate=200, maxRate=200, comment='Support, add maxRate for CPU heavy trkloose'),
                    }])))

HLT_list=['HLT_mu24_iloose_L1MU15', 'HLT_mu24_ivarloose_L1MU15']
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Primary, ATR-13367, in MC only'), 
                    }])))

HLT_list=['HLT_mu18_mu8noL1', 'HLT_mu18_2mu4noL1', ]
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Primary'), 
                     }])))

# 120Hz at 5e33
HLT_list=['HLT_mu20_ivarloose_L1MU15']
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=-1, comment='Primary'), 
                     5001 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))

HLT_list=['HLT_2mu10']
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      5001 : TriggerRule(PS=-1, comment='Disabled'),
                      }])))

HLT_list=['HLT_3mu4']
muon_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    #5001 : TriggerRule(PS=-1, comment='Disabled YU'),
                    }])))

########################################
# supporting

muon_Rules_supporting={}

muon_Rules_supporting.update({
    # ----------------------------------------
    # J/Psi T&P trigger
    # rate so small can stay always unPS    
#v6
    'HLT_mu20_2mu0noL1_JpsimumuFS'             : {  1000 : TriggerRule(rate=5, comment='Support, Jpsi tagprobe') },
    'HLT_mu20_2mu4_JpsimumuL2'                 : {  1000 : TriggerRule(rate=5, comment='Support, Jpsi tagprobe') },

    # 1.0e34
    # this could always stay unPS actualy. is it expected rate is so low (< 1Hz at1e34)?
    'HLT_mu18_2mu0noL1_JpsimumuFS'             : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu18_2mu4_JpsimumuL2'                 : { 1000 : TriggerRule(PS=-1, comment='Disabled') },

    # ----------------------------------------
    # Z T&P idperf
    # rate is 10 Hz at 1e34
    'HLT_mu13_mu13_idperf_Zmumu'               : {  1000 : TriggerRule(PS=1, comment='Support Express, Jpsi tagprobe', ESRate=0.2) },
})

# ========================================
# many rerun . heavy CPU? 

HLT_list=[
    # ATR-13886 'HLT_mu11',
    'HLT_mu26',
    # ATR-13886 'HLT_mu4_msonly',
    'HLT_mu6_msonly','HLT_mu10_msonly',
    'HLT_mu0_perf',
    'HLT_mu20_nomucomb', 'HLT_mu11_nomucomb','HLT_mu6_nomucomb','HLT_mu4_nomucomb',
    'HLT_mu14_ivarloose', #ATR-13494 for tau
    'HLT_mu20_ivarmedium_L1MU10',
]
muon_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support',rerun=1), 
}])))

HLT_list=['HLT_mu20']
muon_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.4, comment='Support',rerun=1), 
}])))


#Eventually enable as monitoring for mu22_mu4noL1
HLT_list=['HLT_mu22']
muon_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.4, comment='Support, Express Stream',ESValue=1,rerun=1), 
}])))



# ========================================

muon_Rules_supporting.update({

    # ----------------------------------------
    # Level-1
    # low pT used for supporting (also for bJpsi_Trkloose)
        'L1_MU4'                                   : {  1000 : TriggerRule(rate=1000, maxRate=1000, comment='Support L1, needed also by bjets '),
                                                        5001 : TriggerRule(rate=3000, maxRate=3000, comment='Support L1, needed also by bjets '),
                                                        9001 : TriggerRule(rate=50, maxRate=50, comment='Support L1, needed also by bjets. SOSminus'), 
                                                        },
        'L1_MU6'                                   : {  1000 : TriggerRule(rate=300, maxRate=300, comment='Support L1, Maxrate for CPU-heavy trakloose'),
                                                        },

})
    

muon_Rules_supporting.update({
    # ----------------------------------------
    # un-isolated single muon for Background estimation
    
    # heavy for CPU maybe ?
    
    # PS=10 (2015.06.25 MCG), increase to PS=30 else at 1e34 is 35 Hz from each !! is this ok?
    # Q: I guess PS instead of rate =xxx in order to have a constant fraction wrt primary trigger ? 
    # this gives 30-40 Hz from muon
    
    # For mu20/24_iloose_L1MU15
    'HLT_mu20_L1MU15'                          : {  1000 : TriggerRule(PS=30, comment='Support', rerun=1),
                                                    5001 : TriggerRule(PS=0, comment='Support', rerun=1) },
    # For mu24_imedium and mu26_imedium
    'HLT_mu24'                                 : {  1000 : TriggerRule(PS=50, comment='Support', rerun=1) },
})


muon_Rules_supporting.update({              
    # ----------------------------------------
    # ~1Hz from each of these.

    # SM W+nJets
    'HLT_mu26_2j20noL1'                        : {  1000 : TriggerRule(PS=224, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    'HLT_mu26_3j20noL1'                        : {  1000 : TriggerRule(PS=156, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    'HLT_mu26_4j15noL1'                        : {  1000 : TriggerRule(PS=156, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    'HLT_mu26_5j15noL1'                        : {  1000 : TriggerRule(PS=104, comment='Support, Phys-SM, 1 Hz target at 1.3e34') },
    
    # 1.5e34
    'HLT_mu24_2j20noL1'                        : {  1000 : TriggerRule(PS=224, comment='Support, Phys-SM, 1 Hz target at 1e34'),
                                                    17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu24_3j20noL1'                        : {  1000 : TriggerRule(PS=156, comment='Support, Phys-SM, 1 Hz target at 1e34'),
                                                    17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu24_4j15noL1'                        : {  1000 : TriggerRule(PS=156, comment='Support, Phys-SM, 1 Hz target at 1e34'),
                                                    17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu24_5j15noL1'                        : {  1000 : TriggerRule(PS=104, comment='Support, Phys-SM, 1 Hz target at 1e34'),
                                                    17001 : TriggerRule(PS=-1, comment='Disabled') },

    # 1.0e34
    'HLT_mu20_L1MU15_2j20noL1'                 : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu20_L1MU15_3j20noL1'                 : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu20_L1MU15_4j15noL1'                 : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu20_L1MU15_5j15noL1'                 : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    })


muon_Rules_supporting.update({              
    # ----------------------------------------
    # prescale + rerun
    # quite hevy CPU wise ?
    'HLT_mu4'                                  : {  1000 : TriggerRule(rate=2.0, comment='Support', rerun=1) },
    'HLT_mu6'                                  : {  1000 : TriggerRule(rate=1.0, comment='Support', rerun=1) },
    'HLT_mu10'                                 : {  1000 : TriggerRule(rate=1.0, comment='Support', rerun=1) },
    'HLT_mu14'                                 : {  1000 : TriggerRule(rate=1.0, comment='Support', rerun=1) },
    'HLT_mu10_nomucomb'                        : {  1000 : TriggerRule(rate=1.0, comment='Support', rerun=1) },
    'HLT_mu14_nomucomb'                        : {  1000 : TriggerRule(rate=1.0, comment='Support', rerun=1) },
    'HLT_mu18'                                 : {  1000 : TriggerRule(rate=1.0, comment='Support Express', rerun=1) },
})

muon_Rules_supporting.update({              
            # ----------------------------------------
            # L1 streamers
        'HLT_noalg_L1MU4'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495',ESRate=0.1) },
        'HLT_noalg_L1MU6'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495',ESRate=0.1) },
        'HLT_noalg_L1MU10'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495',ESRate=0.1) },
        'HLT_noalg_L1MU11'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495',ESRate=0.1) },        
        'HLT_noalg_L1MU15'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495',ESRate=0.1) },
        'HLT_noalg_L1MU20'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495',ESRate=0.1) },

        'HLT_noalg_L12MU4'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495') },
        'HLT_noalg_L12MU6'                         : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495') },
        'HLT_noalg_L12MU10'                        : {  1000 : TriggerRule(inputRate=1.0, comment='Support ATR-13495') },
        
            # ATR-11652, high rates seen
            # MU4_EMPTY: PS=1000 (700 Hz in this morning's run)
            # MU4_FIRSTEMPTY: PS=30000 (16000 Hz in this morning's run)
            # MU4_UNPAIRED_ISO: PS=10 (6 Hz in this morning's run)
            # 25ns.2 update: 2142 Hz for MU4_E, 2051 Hz for MU4_FE, 50 Hz for MU4_UISO, 5 Hz for MU11_E
            # needs 100 Hz input from MU4_E for IDCosmic
            'L1_MU4_EMPTY'                             : {  1000 : TriggerRule(PS=50,   scaling='bunches', comment='Support') },
            'L1_MU4_FIRSTEMPTY'                        : {  1000 : TriggerRule(PS=1000, scaling='bunches', comment='Support') },
            'L1_MU11_EMPTY'                            : {  1000 : TriggerRule(PS=1,    scaling='bunches', comment='Support') },
            #
            'HLT_noalg_L1MU4_EMPTY'                    : {  1000 : TriggerRule(inputRate=0.5, comment='Support'),
                                                        5001 : TriggerRule(PS=200, comment='Support ATR-14383') },

            'HLT_noalg_L1MU4_FIRSTEMPTY'               : {  1000 : TriggerRule(inputRate=0.5, comment='Support') },
            'HLT_noalg_L1MU4_UNPAIRED_ISO'             : {  1000 : TriggerRule(inputRate=0.5, comment='Support') },
            
            'HLT_noalg_L1MU11_EMPTY'                    : {  1000 : TriggerRule(inputRate=0.5, comment='Support'),       
                                                             5001 : TriggerRule(PS=400, comment='Support ATR-14383') },
        
            # out-of-time monitoring item, disabled for the moment
            'HLT_noalg_L1MU20_FIRSTEMPTY'              : {  1000 : TriggerRule(PS=-1, comment='Support') },
            
            # ----------------------------------------
            # idperf
            # c.f. Run1: 0.1 Hz for mu18, 0.5 Hz for mu22
            # this has 7 Hz is it needed ? 
            # heavy CPU too
            })


muon_Rules_supporting.update({ 
    'HLT_mu4_idperf'                           : {  1000 : TriggerRule(rate=0.5, comment='Support', rerun=1) },
    'HLT_mu6_idperf'                           : {  1000 : TriggerRule(rate=0.5, comment='Support Express', ESRate=0.1, rerun=1) },
    'HLT_mu10_idperf'                          : {  1000 : TriggerRule(rate=0.5, comment='Support', rerun=1) },
    'HLT_mu20_idperf'                          : {  1000 : TriggerRule(rate=5, comment='Support Express', ESRate=0.4, rerun=1) },
    'HLT_mu24_idperf'                          : {  1000 : TriggerRule(rate=0.5, comment='Support', rerun=1) },
})

########################################

muon_Rules={}

RulesList=[muon_Rules_2e34, muon_Rules_1p5e34, muon_Rules_1e34, muon_Rules_0p7e34, muon_Rules_0p5e34, muon_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if muon_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    muon_Rules.update(Rules)


#################################################################################################
#
#
# B-physics
#
# comments:
# looking at https://atlas-trig-cost.cern.ch/?dir=RateProcessings-2016&type=20.7.3.4&tag=mcv6-20.7.3.4-13TeV-nops-L2.0e34&run=280500:
# MU10_2MU6 is 6 kHz at 2e34,  L1BPH-2M9-2MU6_BPH-2DR15-2MU6  has 800 Hz at 2e34.
# 2MU6 for 1.5e34 7kHz , high. HLT 100 Hz, high too. 
# L1 topo for 1.5e34 is L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4, 2kHz at L1 (HLT 100 Hz. too high ?)
# for 1e34 MU6_MU4 for non L1 topo is 16 kHz at L1. 
# for L1 topo, L1BPH-2M8-2MU4_BPH-0DR15-2MU4 has 6 kHz (HLT 100 Hz, too high ?).
# for 5e33 MU6_MU4 ok , 8 kHz at L1.
# for L1 topo, L1BPH-2M8-2MU4_BPH-0DR15-2MU4 has 3 kHz  (HLT 50 Hz)
#
# From James and Semen, feedback:
# For B->J/psi, B0->mumuK* and Bs->mumu samples it's likeefficiency drop is
# L1 2mu4/mu6_2mu6/2mu6/mu10_mu6/2mu10 = 100/59/33/16/5 (with little variation between the samples).
# For Upsilon it's a bit different: 2mu4/mu6_2mu6/2mu6/mu10_mu6/2mu10 = 100/44/23/12/6.
#
# ==> Needs to revisit with fixed rate estimation (multi-MU4,6 rates are currently 20% higher than those in v5)
#################################################################################################


bphys_Rules_2p5e34={}

HLT_list=[
    'HLT_2mu10_bJpsimumu',
    'HLT_2mu10_bUpsimumu',
    'HLT_2mu10_bBmumuxv2',
    'HLT_2mu10_bBmumux_BcmumuDsloose',
    'HLT_2mu10_bJpsimumu_noL2',
    'HLT_mu11_nomucomb_mu6_nomucomb_bPhi',
    'HLT_mu11_nomucomb_mu6noL1_nscan03_L1MU11_2MU6_bPhi',
]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))


L1_list=['L1_LFV-MU'] #2.7kHz
HLT_list=['HLT_mu10_mu6_bBmumuxv2_L1LFV-MU',] # 20Hz
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=10, comment='Primary. Adds delayed?'),
                    }])))
bphys_Rules_2p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, may not be necessary if MU10_2MU6 is kept'),
                    15001 : TriggerRule(PS=2, comment='Primary, may not be necessary if MU10_2MU6 is kept'),
                    }])))

HLT_list=[
    'HLT_mu10_mu6_bBmumuxv2',
    'HLT_mu10_mu6_bBmumux_BcmumuDsloose',
    'HLT_mu10_mu6_bJpsimumu', 
    'HLT_mu10_mu6_bJpsimumu_Lxy0',
        ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=10, comment='Primary'), 
                    }])))

HLT_list=[
    'HLT_mu10_mu6_bUpsimumu',
    ]
bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), # v4
                    10001 : TriggerRule(PS=2, comment='Primary'), # v4
                    }])))

HLT_list=[
    'HLT_mu10_mu6_bBmumux_BpmumuKp',
    'HLT_mu10_mu6_bBmumux_Taumumux_noL2', 'HLT_mu10_mu6_bBmumux_Taumumux',]
    
if NoHLTTopo :
    bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=10, comment='Primary'), # v4
                        }])))
else :
    bphys_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        10001 : TriggerRule(PS=3, comment='Primary'),
                        }])))


########################################
# 2e34 

bphys_Rules_2e34={}

# ----------
#
# non l1 topo
#
L1_list=['L1_2MU6', ] # 9000 Hz

if NoHLTTopo:
    bphys_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'),
                       15001 : TriggerRule(PS=4, comment='Primary L1'),
                       17001 : TriggerRule(PS=10, comment='Primary L1'),
                        }])))
else:
    bphys_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1, EndOfFill'), 
                        10001 : TriggerRule(rate=500, maxRate=500, comment='Support L1 topo, EndOfFill'),
                        }])))



HLT_list=[
    'HLT_2mu6_bBmumu_Lxy0',
    'HLT_2mu6_nomucomb_bPhi',
]
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    }])))


HLT_list=['HLT_2mu6_bBmumu', 
          ]  # v4
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, Express', ESRate=0.1), 
                    }])))

HLT_list=['HLT_2mu6_bUpsimumu',] # v4
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, Express, EndOfFill', ESRate=0.1), 
                    7001 : TriggerRule(PS=4, comment='Primary, Express, EndOfFill', ESRate=0.1), 
                    }])))

HLT_list=[
    'HLT_2mu6_bBmumux_BpmumuKp',
    ]
if NoHLTTopo :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=10, comment='Primary, EndOfFill',ESRate=0.1),
                        }])))
else :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill',ESRate=0.1), 
                    5001 : TriggerRule(PS=2, comment='Primary, EndOfFill',ESRate=0.1),
                    10001 : TriggerRule(rate=0.1, comment='Disabled, Express only, EndOfFill', ESValue=1),
                    }])))

HLT_list=[
    'HLT_2mu6_bBmumux_BcmumuDsloose',
    'HLT_2mu6_bJpsimumu', 
    'HLT_2mu6_bJpsimumu_Lxy0',
    'HLT_2mu6_bBmumuxv2',
    'HLT_2mu6_bBmumux_Taumumux',
    ]
    
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=10, comment='Primary'), 
                    }])))

HLT_list=['HLT_2mu6_bBmumux_BsmumuPhi']
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=10, comment='Primary'), 
                    }])))

HLT_list=["HLT_2mu6_bDimu_Lxy0"]
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.1, comment='Primary',ESValue=0.1), 
                    }])))


HLT_list=[
    'HLT_2mu6_bBmumuxv2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', #23Hz #640Hz #delayed?
    'HLT_2mu6_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', #5Hz #640Hz
 ]
if NoHLTTopo :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=2, comment='Primary, EndOfFill'),
                    }])))


HLT_list=[ "HLT_2mu6_bDimu_L12MU6-B","HLT_2mu6_bDimu_L12MU6-BO",'HLT_2mu6_bDimu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6','HLT_2mu6_bDimu_L1BPH-8M15-2MU6']
if NoHLTTopo :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'), 
                    }])))
else :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Express', ESRate=0), 
                    }])))



HLT_list=[
    'HLT_2mu6_bJpsimumu_L12MU6-BO',
    'HLT_2mu6_bJpsimumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
 ]
if NoHLTTopo :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))


HLT_list=[
    'HLT_2mu6_bBmumu_L12MU6-BO',
    'HLT_2mu6_bBmumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
    'HLT_2mu6_bUpsimumu_L12MU6-BO',
    # Not in phys menu 'HLT_2mu6_bUpsimumu_L1BPH-8M15-2MU6_BPH-0DR22-2MU6',
 ]
if NoHLTTopo :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

# 3 mu6

HLT_list=['HLT_3mu6_bTau',
          'HLT_3mu6_bUpsi']
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                1000 : TriggerRule(PS=1, comment='Primary'), 
                }])))

HLT_list=['HLT_3mu6_bJpsi']
bphys_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Express', ESRate=0.1), 
                    }])))

########################################
# 1.5e34

bphys_Rules_1p5e34={}
L1_list=['L1_MU6_2MU4'] #16000 Hz
if emergency2200b:
    bphys_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary L1, no L1Topo, EndOfFill'),
                        }])))
elif NoHLTTopo:
    bphys_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1, no L1Topo, EndOfFill'),
                        9001 : TriggerRule(PS=5, comment='Support L1, SOSminus, no L1Topo, EndOfFill'),
                       14001 : TriggerRule(PS=100, comment='Support L1, SOS, no L1Topo, EndOfFill'),
                       20001 : TriggerRule(rate=1000, maxRate=1000, comment='Support L1 topo, EndOfFill'),
                        }])))
else:        
    bphys_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1, EndOfFill'),                        
                        5001 : TriggerRule(rate=1000, maxRate=1000, comment='Support L1 topo, EndOfFill'),
                        }])))

HLT_list=[
    'HLT_mu6_mu4_bBmumuxv2',
    'HLT_mu6_mu4_bBmumux_BcmumuDsloose',
]
bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'),
                    4001 : TriggerRule(PS=20, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'), 
                    }])))

HLT_list=[
    'HLT_mu6_mu4_bJpsimumu',
    'HLT_mu6_mu4_bJpsimumu_Lxy0',    
]
if NoHLTTopo:
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=20, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=50, comment='Support, EndOfFill'), 
                    }])))
else :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(rate=0.5,maxRate=0.5, comment='Support to L1 topo, EndOfFill'), 
                    }])))

HLT_list=[
    'HLT_mu6_mu4_bBmumux_BpmumuKp',
]
if NoHLTTopo:
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=20, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=-1, comment='Primary, EndOfFill'), 
                    }])))
else :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(rate=0.5,maxRate=0.5, comment='Support to L1 topo, EndOfFill'), 
                    }])))


HLT_list=[
    'HLT_mu6_mu4_bBmumu',
]
if NoHLTTopo :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=2, comment='Primary, EndOfFill'), 
                    }])))
else :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(rate=0.5,maxRate=0.5, comment='Support to L1 topo, EndOfFill'),
                    }])))


HLT_list=[
    'HLT_mu6_mu4_bBmumu_Lxy0',
]
if NoHLTTopo :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))
else :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(rate=0.5,maxRate=0.5, comment='Support to L1 topo, EndOfFill'),
                    }])))    

HLT_list=[
    'HLT_mu6_mu4_bUpsimumu',
]
bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))
    

HLT_list=[
    'HLT_mu6_2mu4', #88Hz at2e34
    ]
if NoHLTTopo:
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'),
                        5001 : TriggerRule(PS=10, comment='Primary, EndOfFill'),
                        }])))  
else:
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
    
    
    
HLT_list=[
    'HLT_mu6_mu4_bBmumu_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4', #L1 1.7kHz
    'HLT_mu6_mu4_bBmumu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', #L1 3.4kHz
    ]
if NoHLTTopo :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        }])))

HLT_list=[
    'HLT_mu6_mu4_bJpsimumu_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4', #35Hz
    # Not in phys menu 'HLT_mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4',
]
if NoHLTTopo :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=2, comment='Primary, EndOfFill'),
                    10001 : TriggerRule(PS=4, comment='Primary, EndOfFill'),
                    }])))

########################################
# 1e34 

bphys_Rules_1e34={}

HLT_list=[
    #Not in phys menu 'HLT_2mu4_bBmumu_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4',
    #Not in phys menu 'HLT_2mu4_bBmumu_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4',
]
if NoHLTTopo :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    10001 : TriggerRule(PS=12, comment='Primary, EndOfFill'), 
                    }])))

HLT_list=[
    #Not in phys menu 'HLT_2mu4_bBmumu_L1BPH-2M8-2MU4_BPH-0DR15-2MU4',
]
if NoHLTTopo :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(PS=2, comment='Primary, EndOfFill'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'), 
                    }])))

HLT_list=[
    # Not in phys menu 'HLT_2mu4_bUpsimumu_L1BPH-7M15-2MU4_BPH-0DR24-2MU4',
]
if NoHLTTopo :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(PS=4, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=5, comment='Primary, EndOfFill'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'), 
                    }])))

HLT_list=[
#    'HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4',
#    'HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',
# Not in phys menu    'HLT_2mu4_bUpsimumu_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-B',
# Not in phys menu    'HLT_2mu4_bUpsimumu_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-BO',
]
if NoHLTTopo :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(PS=2, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=3, comment='Primary, EndOfFill'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'), 
                    }])))

HLT_list=[
    'HLT_2mu4_bBmumuxv2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4', #17kHz
    'HLT_2mu4_bBmumuxv2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4', #37kHz
    # Not in phys menu 'HLT_2mu4_bJpsimumu_L1BPH-2M8-2MU4_BPH-0DR15-2MU4',
]
if NoHLTTopo :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(PS=6, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=10, comment='Primary, EndOfFill'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'), 
                    }])))

HLT_list=[
    # Not in phys menu 'HLT_2mu4_bJpsimumu_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4',
    # Not in phys menu 'HLT_2mu4_bJpsimumu_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4',
]
if NoHLTTopo :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'),
                    }])))
else :
    bphys_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'),
                    5001 : TriggerRule(PS=3, comment='Primary, EndOfFill'),
                    7001 : TriggerRule(PS=5, comment='Primary, EndOfFill'),
                    10001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'),
                    }])))

########################################
# 0.4e34 

bphys_Rules_0p4e34={}
L1_list=['L1_2MU4']
if NoHLTTopo :
    bphys_Rules_0p4e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(rate=1000, maxRate=1000, comment='Support L1'),
                        }])))
else :
    bphys_Rules_0p4e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(rate=500, maxRate=500, comment='Support L1'),
                        }])))


HLT_list=[
    'HLT_2mu4_bBmumux_BcmumuDsloose',
    'HLT_2mu4_bBmumux_BpmumuKp',
    'HLT_2mu4_bJpsimumu',
    'HLT_2mu4_bUpsimumu',
]
bphys_Rules_0p4e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


###########################
# 0.5 e34

bphys_Rules_0p5e34={} 
L1_list=['L1_3MU4']
if NoHLTTopo :
    bphys_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'),
                       17001 : TriggerRule(PS=2, comment='Prmary L1'), 
                        #5001 : TriggerRule(PS=-1, comment='Dsiabled YU'),
                        }])))
else :
    bphys_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'),
                        17001 : TriggerRule(PS=2, comment='Prmary L1'), 
                        #5001 : TriggerRule(PS=2, comment='Support L1 YU'),
                        }])))

HLT_list=[
    'HLT_3mu4_bJpsi',
    'HLT_3mu4_bUpsi',
    'HLT_mu6_2mu4_bJpsi', 
    'HLT_mu6_2mu4_bUpsi',
    ]
bphys_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    #1000 : TriggerRule(PS=-1, comment='Disabled YU'),
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    }])))

HLT_list=['HLT_3mu4_bTau',]
bphys_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=0, comment='Support, ATR-13501'),
                    }])))

HLT_list=['HLT_3mu4_nomucomb_bTau',]
bphys_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-13501, EndOfFill'),
                    5001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'),
                    }])))

bphys_Rules_delayed={}

HLT_list=[
    'HLT_2mu6_bBmumuxv2_delayed', 
    'HLT_2mu6_bBmumux_BcmumuDsloose_delayed', 
    'HLT_2mu6_bUpsimumu_delayed',
    'HLT_mu10_mu6_bBmumuxv2_delayed',
    'HLT_mu10_mu6_bBmumuxv2_delayed_L1LFV-MU', 
    'HLT_mu10_mu6_bBmumux_BcmumuDsloose_delayed', 
    'HLT_mu10_mu6_bUpsimumu_delayed',
    'HLT_2mu10_bBmumux_BcmumuDsloose_delayed', 
    'HLT_2mu10_bJpsimumu_delayed', 
    'HLT_2mu10_bUpsimumu_delayed', 
    'HLT_2mu10_bBmumuxv2_delayed',
    'HLT_mu6_2mu4_bJpsi_delayed', 
    'HLT_mu6_2mu4_bUpsi_delayed',
    'HLT_mu11_nomucomb_mu6noL1_nscan03_L1MU11_2MU6_bTau_delayed', 
    'HLT_3mu4_nomucomb_delayed',
    'HLT_mu6_nomucomb_2mu4_nomucomb_delayed_L1MU6_3MU4',
    'HLT_mu6_mu4_bBmumux_BsmumuPhi_delayed',
    'HLT_2mu6_bBmumux_BsmumuPhi_delayed',
    'HLT_mu10_mu6_bBmumux_BsmumuPhi_delayed',
    ]

bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))


HLT_list=['HLT_2mu6_bJpsimumu_delayed', 'HLT_2mu6_bJpsimumu_Lxy0_delayed', ]
if emergency2200b_HLTOnly:
    bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=4, comment='Primary'), 
                        }])))
else:
    bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        }])))

HLT_list=['HLT_mu10_mu6_bJpsimumu_delayed', 'HLT_mu10_mu6_bJpsimumu_Lxy0_delayed', ] 
if emergency2200b_HLTOnly:
    bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=2, comment='Primary'), 
                        }])))
else:
    bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        }])))

HLT_list=['HLT_mu6_mu4_bUpsimumu_delayed',] 
bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=20, comment='Support, EndOfFill'), 
                    }])))

HLT_list=['HLT_mu6_mu4_bJpsimumu_delayed', 
          ] 

bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    5001 : TriggerRule(PS=5, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=50, comment='Support, EndOfFill'), 
                    }])))

HLT_list=[ 'HLT_mu6_mu4_bJpsimumu_Lxy0_delayed',
          ] 

bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=50, comment='Support, EndOfFill'), 
                    }])))

HLT_list=['HLT_3mu4_bJpsi_delayed', 'HLT_3mu4_bUpsi_delayed',] 
bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    #5001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'),
                    }])))

HLT_list=['HLT_mu6_mu4_bBmumuxv2_delayed', 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_delayed', ]
bphys_Rules_delayed.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EndOfFill'), 
                    7001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill'), 
                    }])))


########################################
# supporting trigger

bphys_Rules_supporting={}


HLT_list=[
    'HLT_2mu10_bDimu_noL2',
]
bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.2, comment='Support'), 
                    }])))

# ----------------------------------------

HLT_list=['HLT_2mu6_bDimu', 'HLT_2mu6_bDimu_novtx_noos', 'HLT_2mu6_bDimu_noinvm_novtx_ss']
bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=0.2, comment='Support Express', ESValue=1),
                        7001 : TriggerRule(rate=1.0, comment='Support Express, 1 Hz when 2MU6 is primary, Express', ESRate=0.2),
                        10001     : TriggerRule(rate=0.2, comment='Support Express', ESValue=1),
                        }])))
#------------------------------------


HLT_list=['HLT_mu6_mu4_bDimu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', #220 Hz . prescale by 100 ?
          # Not in phys menu 'HLT_mu6_mu4_bDimu_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4', #65 Hz
          'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', #350 Hz. prescale by 100?
          # Not in phys menu 'HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4', # 200 Hz. prescale by 100?
          ]

if NoHLTTopo :
    bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=1.0, comment='Support'),
                        20001 : TriggerRule(rate=0.2, comment='Support'),
                        }])))
          
# ----------------------------------------

# kn: 2015.09.08  assign 1 Hz even when 2MU4 is supposed to be primary, as 2MU4 can be prescaled by hand on the fly at P1

HLT_list=['HLT_mu6_mu4_bDimu', 'HLT_mu6_mu4_bDimu_novtx_noos', 'HLT_mu6_mu4_bDimu_noinvm_novtx_ss',
         ]
bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=1.0, comment='Support'),
                        7001 : TriggerRule(rate=0.2, comment='Support'),
                        }])))


#---------------------------------------

HLT_list=[# Not in phys menu 'HLT_2mu4_bDimu_L1BPH-7M15-2MU4_BPH-0DR24-2MU4',
          # Not in phys menu 'HLT_2mu4_bDimu_novtx_noos_L1BPH-7M15-2MU4_BPH-0DR24-2MU4', 
          ]
if NoHLTTopo :
    bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=1.0, comment='Support'),
                    10001 : TriggerRule(rate=0.2, comment='Support'),
                    }])))
         
# ----------------------------------------

HLT_list=['HLT_3mu4_bDimu']
bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    #1000 : TriggerRule(rate=0.2, comment='Support'),
                    #5001 : TriggerRule(PS=-1, comment='Support'),
                    1000 : TriggerRule(PS=1, comment='Support YU'),
                    }])))

HLT_list=['HLT_3mu6_bDimu']
bphys_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'),
                    5001 : TriggerRule(rate=0.2, comment='Support, ATR-13501'),
                    }])))

# ========================================

bphys_Rules_supporting.update({
    # ----------------------------------------
    # di-muons w/o cuts
    'HLT_2mu4'                           : {  1000 : TriggerRule(rate=0.2, comment='Support') },
    'HLT_2mu6'                           : {  1000 : TriggerRule(rate=0.2, comment='Support') },

    # ----------------------------------------
    # idperf at Jpsi mumu
    # 1.5 Hz @ Run1
    'HLT_mu4_mu4_idperf_bJpsimumu_noid'  : {  1000 : TriggerRule(rate=1.5, comment='Support Express, Jpsi idperf', ESRate=0.1) },

    # ----------------------------------------
    # Jpsi Trkloose


    'HLT_mu4_bJpsi_Trkloose'             : {  1000 : TriggerRule(rate=5.0, comment='Support, Jpsi idperf')}, 
    'HLT_mu6_bJpsi_Trkloose'             : {  1000 : TriggerRule(rate=5.0, comment='Support, Jpsi idperf', ESRate=0.1)},
    'HLT_mu10_bJpsi_Trkloose'            : {  1000 : TriggerRule(rate=1.0, comment='Support, Jpsi idperf')},
    'HLT_mu18_bJpsi_Trkloose'            : {  1000 : TriggerRule(rate=1.0, comment='Support, Jpsi idperf', ESRate=0.1)},


})

########################################

bphys_Rules={}

RulesList=[bphys_Rules_2p5e34, bphys_Rules_2e34, bphys_Rules_1p5e34, bphys_Rules_1e34, bphys_Rules_0p5e34, bphys_Rules_0p4e34, bphys_Rules_supporting, bphys_Rules_delayed]
 
for Rules in RulesList:
    for newrule in Rules.keys():
        if bphys_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    bphys_Rules.update(Rules)


#################################################################################################
#
#
# Missing ET
#
# we miss some xe100_xxx from L1 XE60. also, maybe we need to activate also all tighter xe thresholds?
#
#################################################################################################



########################################
# 2p5e34 

met_Rules_2p5e34={}

L1_list=['L1_XE70', 'L1_XE80']
met_Rules_2p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Primary L1'), 
                    }])))

HLT_list=['HLT_xe100', 
          'HLT_xe120',
          'HLT_xe100_L1XE70',
          'HLT_xe110_L1XE70',
          'HLT_xe120_L1XE70',          
          'HLT_xe130_L1XE70',
          'HLT_xe140_L1XE70',
          'HLT_xe150_L1XE70',

        
          
          'HLT_xe120_wEFMu',      

          'HLT_xe120_mht',  'HLT_xe120_pueta',
          'HLT_xe120_mht_wEFMu',  'HLT_xe120_pueta_wEFMu',
          'HLT_xe140_mht_L1XE70', 'HLT_xe150_mht_L1XE70',
          'HLT_xe160_mht_L1XE70',
          
          'HLT_xe120_pufit','HLT_xe120_tc_em','HLT_xe120_tc_lcw',
          'HLT_xe120_pufit_wEFMu','HLT_xe120_tc_em_wEFMu','HLT_xe120_tc_lcw_wEFMu',

        
    ]
met_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable ATR-14226'),
                    }])))
HLT_list=[
  "HLT_xe110_pufit", 
    "HLT_xe110_pufit_wEFMu", 
    "HLT_xe130_mht_L1XE70", 
    "HLT_xe130_mht_wEFMu_L1XE70", 
    "HLT_xe130_pufit_L1XE70", 
    "HLT_xe130_pufit_wEFMu_L1XE70", 
    "HLT_xe140_pufit_L1XE70", 
    "HLT_xe160_mht_wEFMu_L1XE70", 
    "HLT_xe160_pufit_L1XE70", 
    "HLT_xe160_pufit_wEFMu_L1XE70", 
    "HLT_xe160_tc_lcw_L1XE70", 
    "HLT_xe160_tc_lcw_wEFMu_L1XE70",

    "HLT_xe140_mht_L1XE80", 
    "HLT_xe140_mht_wEFMu_L1XE80", 
    "HLT_xe140_pufit_L1XE80", 
    "HLT_xe140_pufit_wEFMu_L1XE80", 
    "HLT_xe160_mht_L1XE80", 
    "HLT_xe160_mht_wEFMu_L1XE80", 
    "HLT_xe160_pufit_L1XE80", 
    "HLT_xe160_pufit_wEFMu_L1XE80", 
    "HLT_xe160_tc_lcw_L1XE80", 
    "HLT_xe160_tc_lcw_wEFMu_L1XE80",]
met_Rules_2p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Disabled (with XE70,80)'),
                    }])))
########################################
# 2e34


met_Rules_2e34={}

L1_list=['L1_XE60'] #4000 Hz
met_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'), 
                    20001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

HLT_list=[
    "HLT_xe120_mht_L1XE60_AND_xe70_L1XE60",
    "HLT_xe120_pufit_L1XE60", 
    ]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    20001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))
HLT_list=[
          'HLT_xe110_L1XE60', 'HLT_xe120_L1XE60', 'HLT_xe130_L1XE60', 'HLT_xe140_L1XE60', 'HLT_xe150_L1XE60',
          'HLT_xe140_mht_L1XE60', 'HLT_xe150_mht_L1XE60', 'HLT_xe160_mht_L1XE60',
    "HLT_xe140_pufit_L1XE60", 
    "HLT_xe140_pufit_wEFMu_L1XE60", 
    "HLT_xe160_pufit_L1XE60", 
    "HLT_xe160_pufit_wEFMu_L1XE60", 
    "HLT_xe140_mht_wEFMu_L1XE60", 
    "HLT_xe160_mht_wEFMu_L1XE60", 
    "HLT_xe160_tc_lcw_L1XE60", 
    "HLT_xe160_tc_lcw_wEFMu_L1XE60",
          ]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, Backup'), 
                    20001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_xe100_L1XE60',]
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary. With expo extraoplation, rate inflates to 435Hz'), 
                   17001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))          



HLT_list=['HLT_xe130_mht_L1XE60_AND_xe90_L1XE60','HLT_xe150_mht_L1XE60_AND_xe100_L1XE60','HLT_xe200_mht_L1XE60_AND_xe150_L1XE60','HLT_xe250_mht_L1XE60_AND_xe200_L1XE60','HLT_xe300_mht_L1XE60_AND_xe250_L1XE60',] 
met_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled, Backup, ATR-15236'),
                    }])))

########################################
# 1.5e34 

met_Rules_1p5e34={}

L1_list=['L1_XE55'] # for now (21/06/16) - to be reviewed
met_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'), 
                    17001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

L1_list=['L1_XE50']
met_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   15001 : TriggerRule(rate=500, maxRate=500, comment='Support'),
                   }])))


HLT_list=[
    #primary
    'HLT_xe130_mht_L1XE50','HLT_xe130_mht_wEFMu_L1XE50',
    'HLT_xe140_mht_L1XE50',
    'HLT_xe160_tc_lcw_L1XE50','HLT_xe160_tc_lcw_wEFMu_L1XE50', 
    'HLT_xe150_pufit_L1XE50', 'HLT_xe150_pueta_L1XE50', #25Hz
    'HLT_xe150_pufit_wEFMu_L1XE50', 'HLT_xe150_pueta_wEFMu_L1XE50',     
    'HLT_xe130_tc_em_L1XE50','HLT_xe130_tc_em_wEFMu_L1XE50', #14Hz 
    'HLT_xe100_L1XE50','HLT_xe100_wEFMu_L1XE50', #40Hz

    #backup
    'HLT_xe110_L1XE50','HLT_xe110_wEFMu_L1XE50', #20Hz
    'HLT_xe150_mht_L1XE50', 'HLT_xe160_mht_L1XE50',

    # New mht+cell-based triggers 
    'HLT_xe110_mht_L1XE50_AND_xe70_L1XE50',
    'HLT_xe110_mht_L1XE50_AND_xe75_L1XE50',
    'HLT_xe110_mht_L1XE50_AND_xe80_L1XE50',    
    'HLT_xe110_mht_L1XE55_AND_xe70_L1XE55',
    'HLT_xe110_mht_L1XE55_AND_xe75_L1XE55',
    'HLT_xe110_mht_L1XE55_AND_xe80_L1XE55',

]

met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    17001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    #primary
    'HLT_xe110_mht_L1XE50','HLT_xe110_mht_wEFMu_L1XE50',
    'HLT_xe120_mht_L1XE50','HLT_xe120_mht_wEFMu_L1XE50', 
    'HLT_xe140_pufit_L1XE50', 'HLT_xe140_pueta_L1XE50',
    'HLT_xe140_pufit_wEFMu_L1XE50', 'HLT_xe140_pueta_wEFMu_L1XE50',
    "HLT_xe120_mht_L1XE60",
    # New mht+cell-based triggers 
    'HLT_xe110_mht_L1XE50_AND_xe65_L1XE50',
    'HLT_xe110_mht_L1XE55_AND_xe65_L1XE55',
]

met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    14001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    'HLT_xe130_tc_lcw_L1XE50', 'HLT_xe130_tc_lcw_wEFMu_L1XE50',
    'HLT_xe130_tc_lcw_L1XE55', 'HLT_xe130_tc_lcw_wEFMu_L1XE55',
    'HLT_xe140_tc_lcw_L1XE50', 'HLT_xe140_tc_lcw_wEFMu_L1XE50',    
    'HLT_xe140_tc_lcw_L1XE55', 'HLT_xe140_tc_lcw_wEFMu_L1XE55',
    'HLT_xe150_tc_lcw_L1XE50', 'HLT_xe150_tc_lcw_wEFMu_L1XE50',
    'HLT_xe150_tc_lcw_L1XE55', 'HLT_xe150_tc_lcw_wEFMu_L1XE55',
    "HLT_xe120_tc_lcw_L1XE60", 
    "HLT_xe130_tc_lcw_L1XE70", 
    "HLT_xe130_tc_lcw_wEFMu_L1XE70", 
    "HLT_xe140_tc_lcw_L1XE60", 
    "HLT_xe140_tc_lcw_wEFMu_L1XE60", 
    "HLT_xe140_tc_lcw_L1XE80", 
    "HLT_xe140_tc_lcw_wEFMu_L1XE80", 
    "HLT_xe140_tc_lcw_L1XE70",

    ]

met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
#backup
'HLT_xe130_mht_L1XE55','HLT_xe130_mht_wEFMu_L1XE55',
'HLT_xe140_mht_L1XE55',
'HLT_xe150_mht_L1XE55', 'HLT_xe160_mht_L1XE55',
"HLT_xe150_mht_wEFMu_L1XE55", 
"HLT_xe160_mht_wEFMu_L1XE55",
    
'HLT_xe140_tc_em_L1XE50', 
'HLT_xe140_tc_em_wEFMu_L1XE50', 

'HLT_xe130_tc_em_L1XE55', 
'HLT_xe130_tc_em_wEFMu_L1XE55',
"HLT_xe140_mht_wEFMu_L1XE50", 
"HLT_xe140_mht_wEFMu_L1XE55",
    
'HLT_xe140_tc_em_L1XE55', 
'HLT_xe140_tc_em_wEFMu_L1XE55',

'HLT_xe150_pueta_L1XE55', 
'HLT_xe150_pueta_wEFMu_L1XE55', 
'HLT_xe150_pufit_L1XE55', 
'HLT_xe150_pufit_wEFMu_L1XE55', 

'HLT_xe160_pueta_L1XE50',
'HLT_xe160_pueta_wEFMu_L1XE50',
'HLT_xe160_pufit_L1XE50',
'HLT_xe160_pufit_wEFMu_L1XE50',

'HLT_xe160_pueta_L1XE55',
'HLT_xe160_pueta_wEFMu_L1XE55',
'HLT_xe160_pufit_L1XE55',
'HLT_xe160_pufit_wEFMu_L1XE55',

'HLT_xe160_tc_lcw_L1XE55',
'HLT_xe160_tc_lcw_wEFMu_L1XE55',

'HLT_xe100_L1XE55','HLT_xe100_wEFMu_L1XE55',
'HLT_xe110_L1XE55','HLT_xe110_wEFMu_L1XE55',
'HLT_xe120_L1XE50','HLT_xe120_wEFMu_L1XE50',
'HLT_xe120_L1XE55','HLT_xe120_wEFMu_L1XE55',
'HLT_xe130_L1XE55','HLT_xe140_L1XE55',
'HLT_xe150_L1XE55','HLT_xe160_L1XE55',
    "HLT_xe130_L1XE50", 
    "HLT_xe130_wEFMu_L1XE50", 
    "HLT_xe130_wEFMu_L1XE55",
]

met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary backup'), 
                    17001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    'HLT_xe110_mht_L1XE55',    
    'HLT_xe110_mht_wEFMu_L1XE55',
    'HLT_xe120_mht_L1XE55',
    'HLT_xe120_mht_wEFMu_L1XE55',
    'HLT_xe140_pufit_L1XE55', 'HLT_xe140_pueta_L1XE55', 
    'HLT_xe140_pueta_wEFMu_L1XE55', 'HLT_xe140_pufit_wEFMu_L1XE55',
    ]
met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary backup'), 
                    14001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))
HLT_list=[
    'HLT_xe105_pufit_L1XE50',
    'HLT_xe105_pufit_L1XE55',
    'HLT_xe110_pufit_L1XE50',
    'HLT_xe110_pufit_L1XE55',
    "HLT_xe110_pufit_wEFMu_L1XE50", 
    "HLT_xe110_pufit_wEFMu_L1XE55", 
    "HLT_xe120_pufit_wEFMu_L1XE50", 
    "HLT_xe120_pufit_wEFMu_L1XE55", 
    "HLT_xe120_pufit_L1XE50", 
    ]
met_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
    17001 : TriggerRule(PS=-1, comment='Disabled'),

                    }])))


########################################
# 1e34 

met_Rules_1e34={}

HLT_list=[
    'HLT_xe95_pufit_L1XE50',
    'HLT_xe95_pufit_L1XE55',
    'HLT_xe100_pufit_L1XE50',
    'HLT_xe100_pufit_L1XE55',
    "HLT_xe90_pufit_L1XE55", 
    "HLT_xe90_pufit_wEFMu_L1XE55", 
    "HLT_xe100_pufit_wEFMu_L1XE55", 
    "HLT_xe100_pufit_wEFMu_L1XE60",
]
met_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary EndOfFill'),
                    9001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    'HLT_xe100_mht_L1XE50','HLT_xe100_mht_wEFMu_L1XE50',
    'HLT_xe100_mht_L1XE60',
    'HLT_xe100_mht_wEFMu_L1XE55', 
    'HLT_xe100_mht_L1XE55',
    'HLT_xe130_pufit_L1XE50', 'HLT_xe130_pueta_L1XE50',
    'HLT_xe130_pufit_L1XE55', 'HLT_xe130_pueta_L1XE55',
    'HLT_xe130_pufit_wEFMu_L1XE50', 
    'HLT_xe130_pueta_wEFMu_L1XE50',
    'HLT_xe130_pufit_wEFMu_L1XE55',
    'HLT_xe130_pueta_wEFMu_L1XE55',
    ]
met_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
'HLT_xe120_tc_lcw_L1XE50',
'HLT_xe120_tc_lcw_wEFMu_L1XE50',
]
met_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14446'), 
                    14001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
'HLT_xe120_tc_em_L1XE50',
'HLT_xe120_tc_em_wEFMu_L1XE50',
]
met_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14446'), 
                    14001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    'HLT_xe120_tc_lcw_L1XE55',
    'HLT_xe120_tc_em_L1XE55', 
    'HLT_xe120_tc_em_wEFMu_L1XE55', 
    'HLT_xe120_tc_lcw_wEFMu_L1XE55', 
    ]
met_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14446'), 
                    14001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

########################################
# 0.5e34 

met_Rules_0p5e34={}

# same as what runs at 1e34
HLT_list=[
    #primary
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
    'HLT_xe100_tc_lcw_L1XE60',
    "HLT_xe110_pueta_wEFMu_L1XE50", 
    "HLT_xe110_pueta_wEFMu_L1XE55", 
    "HLT_xe120_pueta_wEFMu_L1XE50", 
    "HLT_xe120_pueta_wEFMu_L1XE55",

    ]
met_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14446'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=[
    'HLT_xe100_tc_lcw_L1XE50', 'HLT_xe100_tc_lcw_wEFMu_L1XE50', 'HLT_xe100_tc_em_L1XE50', 'HLT_xe110_tc_em_L1XE50', 'HLT_xe100_tc_em_wEFMu_L1XE50', 'HLT_xe110_tc_em_wEFMu_L1XE50',
    ]
met_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=['HLT_xe80_tc_lcw_L1XE50',    'HLT_xe90_tc_lcw_L1XE50', 'HLT_xe90_tc_lcw_wEFMu_L1XE50', ]
met_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    5001 : TriggerRule(PS=-1, comment='Disabled'),
                   }])))

#disabled, add long list at the end

HLT_list=['HLT_noalg_L1XE150', ]
met_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))


########################################
# supporting trigger

met_Rules_supporting={}

# what is the point of this?
L1_list=['L1_XE35', 'L1_XE45']
met_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
    1000 : TriggerRule(rate=100, maxRate=100, comment='Support L1'), 
    }])))

# what is the point of this? it is not set to rerun, and it does not go to ES.
HLT_list=[
    # XE35
    'HLT_xe35',      'HLT_xe35_mht',      'HLT_xe35_pueta',      'HLT_xe35_pufit',      'HLT_xe35_tc_em',      'HLT_xe35_tc_lcw',
    'HLT_xe35_wEFMu','HLT_xe35_mht_wEFMu','HLT_xe35_pueta_wEFMu','HLT_xe35_pufit_wEFMu','HLT_xe35_tc_em_wEFMu','HLT_xe35_tc_lcw_wEFMu',
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=2.0, comment='Support ATR-13496'),
                    5001 : TriggerRule(PS=-1, comment='Disabled'),  
                    }])))

HLT_list=[
    'HLT_xe45_pufit_wEFMu', 'HLT_xe45_wEFMu', 'HLT_xe45_mht_wEFMu', 'HLT_xe45_pueta', 'HLT_xe45_tc_lcw', 'HLT_xe45_tc_em', 'HLT_xe45_mht',   'HLT_xe45_pueta_wEFMu', 'HLT_xe45_tc_lcw_wEFMu', 'HLT_xe45_pufit', 'HLT_xe45', 'HLT_xe45_tc_em_wEFMu',
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable ATR-14226'),  
                    }])))

HLT_list=[
    'HLT_xe100_L1XE35',
    'HLT_xe100_mht_L1XE35',
    'HLT_xe100_mht_wEFMu_L1XE35',
    'HLT_xe100_tc_em_L1XE35',
    'HLT_xe100_tc_em_wEFMu_L1XE35',
    'HLT_xe100_tc_lcw_L1XE35',
    'HLT_xe100_tc_lcw_wEFMu_L1XE35',
    'HLT_xe100_wEFMu_L1XE35',
    'HLT_xe80_L1XE35',
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable ATR-14226'), 
                    }])))


HLT_list=[
    'HLT_xe70_L1XE50',
    'HLT_xe75_L1XE50',
    'HLT_xe75_L1XE55',
    'HLT_xe80_L1XE50',
    'HLT_xe80_L1XE55',
    'HLT_xe90_L1XE50',
    'HLT_xe90_L1XE55',

    
    "HLT_xe70_L1XE50_metrate_studies", 
    "HLT_xe75_L1XE50_metrate_studies", 
    "HLT_xe75_L1XE55_metrate_studies", 
    "HLT_xe80_L1XE50_metrate_studies", 
    "HLT_xe80_L1XE55_metrate_studies", 
    "HLT_xe90_L1XE50_metrate_studies", 
    "HLT_xe90_L1XE55_metrate_studies",
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Support, MetRateStudies'),
                    }])))

HLT_list=[
"HLT_xe70_L1XE50", 
"HLT_xe75_L1XE50", 
"HLT_xe75_L1XE55", 
"HLT_xe80_L1XE50", 
"HLT_xe80_L1XE55", 
"HLT_xe90_L1XE50", 
"HLT_xe90_L1XE55",
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=1.0, comment='Support, MetRateStudies'),
                    }])))

#----------------------------------------

HLT_list=['HLT_xe50']  # rerun needed for tau (ATR-11530)
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=0, comment='Disable ATR-14226', rerun=1), 
                    }])))

HLT_list=['HLT_xe70_L1XE45']  # rerun needed for tau (ATR-11530)
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disable ATR-14226', rerun=1), 
                    }])))


# ========================================
# very important
# rerun, so very expensive. But really needed to do emaulation for all cases, and measure efficiency in data.

HLT_list=[
    'HLT_xe0noL1_l2fsperf',       'HLT_xe0noL1_l2fsperf_mht',   'HLT_xe0noL1_l2fsperf_pueta',
    'HLT_xe0noL1_l2fsperf_pufit', 'HLT_xe0noL1_l2fsperf_tc_em', 'HLT_xe0noL1_l2fsperf_tc_lcw',
    ]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support',rerun=1), 
    }])))

# ========================================

HLT_list=['HLT_noalg_L1XE300']
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support ATR-14177'),
                   17001 : TriggerRule(PS=10, comment='Support ATR-14177, needs higher threshold for PS=1'),
    }])))

HLT_list=['HLT_noalg_L1XE45']
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(inputRate=0.5, comment='Support Express', ESValue=1),
    }])))

HLT_list=['HLT_noalg_L1XE30', 'HLT_noalg_L1XE40',]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1.5, comment='Support'), #SX1404
                   }])))

HLT_list=['HLT_noalg_L1XE10',]
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=3, comment='Support'), 
                   }])))


# ========================================
# why is this here and not in Monitoring rules?
HLT_list=['HLT_noalg_L1RD0_FILLED']
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(inputRate=15, comment='Support Express',ESValue=75), ## Make sure the PS*ES=0.2 Hz
    }])))

# why is this here and not in Monitoring rules?
HLT_list=['HLT_noalg_L1RD0_EMPTY']
met_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, comment='Support Express',ESValue=1),
    }])))


########################################

met_Rules={}

RulesList=[met_Rules_2p5e34, met_Rules_2e34, met_Rules_1p5e34, met_Rules_1e34, met_Rules_0p5e34, met_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if met_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
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

jet_Rules_2e34={}

L1_list=['L1_J75.31ETA49',]
jet_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   }])))
HLT_list=[
"HLT_ht1200_L1J100", 
"HLT_ht1400_L1J100", 
"HLT_j480_a10_lcw_L1J100", 
"HLT_j480_a10r_L1J100", 
"HLT_3j220", 
"HLT_4j130", 
"HLT_5j100", 
"HLT_5j85_0eta240", 
"HLT_6j85", 
"HLT_7j50",
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=1, comment='Support 2e34 backup (to run unPSed)')
                   }])))

HLT_list=[
    # forward jet
    'HLT_j260_320eta490_jes',
    'HLT_j260_320eta490_lcw',
    'HLT_j260_320eta490_lcw_jes',
    'HLT_j260_320eta490_lcw_nojcalib',
    'HLT_j260_320eta490_nojcalib',
    ]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(rate=0.2, comment='Support')
                   }]))) #SX1404


HLT_list=[
    # forward jet
    'HLT_j260_320eta490',
    ]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary'), 
                   20001 : TriggerRule(PS=-1, comment='Disabled'),
                   }]))) #SX1404


L1_list=['L1_J100','L1_J100.31ETA49','L1_3J50', 'L1_4J20','L1_5J15.0ETA25','L1_6J15',]
jet_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary L1'), 
                     }])))

L1_list=['L1_J75_3J20']
jet_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   15001 : TriggerRule(PS=2, comment='Disabled, Needs to add tighter primary L1 (J85_3J30)'), 
                     }])))

HLT_list=[
    # 1jet
    'HLT_j420',
    'HLT_j420_jes',
    'HLT_j420_lcw',
    'HLT_j420_lcw_jes',
    'HLT_j420_nojcalib',
    'HLT_j420_lcw_nojcalib',
    'HLT_j440',
    # Forward jet
    'HLT_j360_320eta490_jes',
    'HLT_j360_320eta490_lcw',
    'HLT_j360_320eta490_lcw_jes',
    'HLT_j360_320eta490_lcw_nojcalib',
    'HLT_j360_320eta490_nojcalib',
    # 3jet
    'HLT_3j200',  #v6
    # 4jet
    'HLT_4j110',
    'HLT_4j120',
    # 5jet 
    'HLT_5j85',
    'HLT_5j85_jes',
    'HLT_5j85_lcw',
    'HLT_5j85_lcw_jes',
    'HLT_5j85_lcw_nojcalib',
    'HLT_5j85_nojcalib',

    # 5 jet top analysis
    'HLT_5j65_0eta240',
    'HLT_5j70_0eta240',

    # 6jet
    'HLT_6j50_0eta240_L14J20',
    'HLT_6j50_0eta240_L15J150ETA25',

    'HLT_6j55_0eta240_L14J20',
    'HLT_6j55_0eta240_L15J150ETA25',

    'HLT_6j70',

    # 7jet
    'HLT_7j45',
    'HLT_7j45_L14J20',

    'HLT_7j45_0eta240_L14J20',
    'HLT_7j45_0eta240_L15J150ETA25',
    # HT
    'HLT_ht1000_L1J100',
    # Razor
    'HLT_10j40_L14J20',
    'HLT_10j40_L16J15',
    ]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary'), 
                       }])))

HLT_list=['HLT_j460']
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, Express', ESRate=0.2), 
                    }])))


HLT_list=['HLT_j400',
          'HLT_j360_320eta490']
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, pass-through 1Hz ATR-11862', PTRate=1.0),
                   17001 : TriggerRule(rate=1, comment='Disabled, pass-through 1Hz ATR-11862', PTRate=1.0), 
                    }])))

HLT_list=['HLT_j400_jes',
          'HLT_j400_lcw',
          'HLT_j400_lcw_jes',
          'HLT_j400_lcw_nojcalib',
          'HLT_j400_nojcalib',
          'HLT_j400_sub',]
 
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, pass-through 1Hz ATR-11862'),
                   17001 : TriggerRule(PS=-1, comment='Disabled'), 
                       }])))

# Fat jet
HLT_list=['HLT_j440_a10_lcw_L1J100',
          'HLT_j460_a10_sub_L1J100',
          'HLT_j460_a10_lcw_L1J100',
          'HLT_j460_a10_nojcalib_L1J100',
          'HLT_j460_a10_lcw_nojcalib_L1J100',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
     1000 : TriggerRule(PS=1, comment='Primary'),
    20001 : TriggerRule(PS=-1, comment='Disabled'),
    }])))

HLT_list=['HLT_j460_a10_lcw_sub_L1J100']
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
     1000 : TriggerRule(PS=1, comment='Primary',ESRate=0.2),
    20001 : TriggerRule(rate=0.2, comment='Disabled',ESValue=1),
    }])))

HLT_list=[
          'HLT_j420_a10_lcw_L1J100',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
     1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'),
    14001 : TriggerRule(PS=-1, comment='Disabled'),
    }])))


#reclustered jet

HLT_list=[ 
    'HLT_j440_a10r_L1J100',
    'HLT_j460_a10r_L1J100',
    ]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
     1000 : TriggerRule(PS=1, comment='Primary'),
    20001 : TriggerRule(PS=-1, comment='Disabled'),
     }])))

HLT_list=[ 
    'HLT_j420_a10r_L1J100',
    ]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
     1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'),
    14001 : TriggerRule(PS=-1, comment='Disabled'),
    }])))

# ----------------------------------------

HLT_list=['HLT_noalg_L1J400',]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Support Express', ESRate=0.4), 
                       }])))

# ----------------------------------------
HLT_list=[
    'HLT_j225_gsc400_boffperf_split',
]
jet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-13434'),
                    17001 : TriggerRule(rate=1, comment='Disabled, the same rule as j400'), 
                     }])))

########################################
# 1e34 

jet_Rules_1e34={}
L1_list=['L1_4J15',]
jet_Rules_1e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   17001 : TriggerRule(PS=-1, comment='Disabled L1'), 
                     }])))
HLT_list=[
    # 5jet 
    'HLT_5j85_L14J15',
    'HLT_5j85_lcw_jes_L14J15',
    'HLT_5j85_lcw_nojcalib_L14J15',
    'HLT_5j85_lcw_L14J15',
    'HLT_5j85_nojcalib_L14J15',
    'HLT_5j85_jes_L14J15',
    # 5 jet top analysis
    'HLT_5j65_0eta240_L14J15',
    'HLT_5j70_0eta240_L14J15',
    # 6jet
    'HLT_6j50_0eta240_L14J15',
    'HLT_6j55_0eta240_L14J15',
    'HLT_6j70_L14J15',
    # 7jet
    'HLT_7j45_L14J15',
    # Razor
    'HLT_10j40_L14J15',
]
jet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary'), 
                      17001 : TriggerRule(PS=-1, comment='Disabled'), 
                       }])))


HLT_list=[
    # 1jet
    'HLT_j380',
    'HLT_j380_jes',
    'HLT_j380_lcw',
    'HLT_j380_lcw_jes',
    'HLT_j380_lcw_nojcalib',
    'HLT_j380_nojcalib',
    'HLT_4j100',
    'HLT_5j70',
    'HLT_6j60',
    'HLT_5j70_L14J15',
    'HLT_6j60_L14J15',
    ]

jet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'), 
                      14001 : TriggerRule(PS=-1, comment='Disabled'), 
                       }])))


HLT_list=[
          'HLT_j400_a10_lcw_L1J100',
          ]
jet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
           1000 : TriggerRule(PS=1, comment='Primary'),
           10001 : TriggerRule(PS=-1, comment='Disabled'),
           }])))

HLT_list=[
           'HLT_j400_a10r_L1J100'
         ]
jet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=1, comment='Primary'), 
                          10001 : TriggerRule(PS=-1, comment='Disabled'),
                          }])))

HLT_list=[
           'HLT_3j175',
         ]
jet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=1, comment='Primary'), 
                          10001 : TriggerRule(PS=-1, comment='Disabled'),
                          }])))

########################################
# 0p5e34 - 0p75e34

jet_Rules_0p5e34={}

HLT_list=[
    'HLT_j55_gsc110_boffperf_split',
    'HLT_j225_gsc360_boffperf_split',
    'HLT_j225_boffperf_split',
    'HLT_j360_boffperf_split'
]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(rate=0.5, comment='Support'),
                     }])))

HLT_list=[
    'HLT_j225_gsc380_boffperf_split'
]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(rate=0.5, comment='Support'),
                     }])))


HLT_list=[          
          'HLT_j380_a10_lcw_L1J100',
          'HLT_j380_a10r_L1J100',
          ]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'),
                     7001 : TriggerRule(rate=0.5, comment='Support'),
                     }])))

L1_list=['L1_J75',]
jet_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   7001 : TriggerRule(rate=100, maxRate=100, comment='Support'),
                   }])))

HLT_list=[
    # ht
    'HLT_ht850_L1J75',
    'HLT_ht850_L1J100',
    ]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary'), 
                   7001 : TriggerRule(PS=-1, comment='Disabled'),
                   }])))

# ----------------------------------------

L1_list=['L1_3J40']
jet_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'), 
                    5001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


HLT_list=['HLT_4j85',
]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     5001 : TriggerRule(rate=1., comment='Support'),
                     }]))) #SX1404


HLT_list=[
          'HLT_4j85_jes',
          'HLT_4j85_lcw',
          'HLT_4j85_lcw_jes',
          'HLT_4j85_lcw_nojcalib',
          'HLT_4j85_nojcalib',]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     5001 : TriggerRule(rate=0.2, comment='Support'),
                     }]))) #SX1404

# ----------------------------------------

HLT_list=['HLT_5j55','HLT_5j60','HLT_5j60_L14J15',]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      5001 : TriggerRule(rate=1.0, comment='Support'),
                      }])))

HLT_list=['HLT_6j45',
          ]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      7001 : TriggerRule(PS=-1, comment='Disabled'),
                      }])))

HLT_list=[
          'HLT_6j45_0eta240_L14J20',
          'HLT_6j45_0eta240_L15J150ETA25',]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                     14001 : TriggerRule(rate=1.0, comment='Support, ATR-13382'),
                      }])))
HLT_list=['HLT_6j45_0eta240_L1J4-MATCH','HLT_5j65_0eta240_L1J4-MATCH','HLT_5j70_L1J4-MATCH','HLT_7j45_L1J4-MATCH',] 
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='J4-MATCH seeded multi jet'),
                     14001 : TriggerRule(rate=1.0, comment='ATR-15236, ATR-14353'),
                      }])))

HLT_list=['HLT_6j45_0eta240',
]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary, ATR-13369, SUSY searches'), 
                      14001 : TriggerRule(rate=1.0, comment='Support, ATR-13382'),
                      }])))

# ----------------------------------------

HLT_list=['HLT_j360_a10r_L1J100',
          'HLT_j360_a10_lcw_L1J100',
          ]
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=1, comment='Primary'), 
                          5001 : TriggerRule(rate=0.5, comment='Support'), 
                          }])))

HLT_list=['HLT_j320']
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        5001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))

HLT_list=['HLT_j340']
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        7001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))

HLT_list=['HLT_j360']
jet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        7001 : TriggerRule(PS=-1, comment='Disable'),
                        }])))

########################################
# 0p15e34 

jet_Rules_0p15e34={}

HLT_list=['HLT_ht700_L1J75',
          'HLT_ht700_L1J100']
jet_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                                1000 : TriggerRule(PS=1, comment='Primary'), 
                                1501 : TriggerRule(PS=-1, comment='Disabled'),
                                }])))

########################################
# prescaled 

jet_Rules_supporting = {
    'L1_RD0_FILLED'  : { 1000 : TriggerRule(rate=1000, maxRate=1000, scaling='bunches', comment='Support L1') },
    }

# ========================================
# prescaled single jets
# 64 Hz total. is this really needed !?! make sure what can be set in CPS it is, to make this small

L1_list=['L1_J15', 'L1_J20', 'L1_J25', 'L1_J30', 'L1_J40', 'L1_J50', 'L1_J85']
jet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                       1000 : TriggerRule(rate=200, maxRate=200, comment='Support L1'),
                       }])))

HLT_list=[
    # RD0
    'HLT_j45_L1RD0_FILLED',
    'HLT_j85_L1RD0_FILLED',
    'HLT_j85_jes',
    'HLT_j85_cleanT',
    'HLT_j85_cleanL',
    'HLT_j85_cleanLLP',
    'HLT_j150',

    'HLT_j260_a10_lcw_L1J75',
    'HLT_j260_a10_lcw_nojcalib_L1J75',
    'HLT_j260_a10_nojcalib_L1J75',
    'HLT_j260_a10_sub_L1J75',
    'HLT_j260_a10_lcw_sub_L1J75',

    'HLT_j300_a10_sub_L1J75',
    'HLT_j300_a10_lcw_L1J75',
    'HLT_j260_a10r_L1J75',
    'HLT_j300_a10r_L1J75',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=1.0, comment='Support'), 
                        }])))
HLT_list=[ 
    'HLT_j35_jes',
    'HLT_j35_lcw',
    'HLT_j35_lcw_jes',
    'HLT_j35_lcw_nojcalib',
    'HLT_j35_nojcalib',
] #SX1404
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=0.2, comment='Support'), 
                        }])))

HLT_list=['HLT_j15',]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=3.0, comment='Support', PTRate=0.5), 
                        }])))

HLT_list=[ 'HLT_j15_320eta490',
]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, comment='Support', PTRate=0.5), 
}])))

HLT_list=['HLT_j35','HLT_j110','HLT_j175','HLT_j260',]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=3.0, comment='Support'), 
                        }])))

HLT_list=['HLT_j60']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=3.0, comment='Support',ESRate=0.2), 
                        }])))


HLT_list=['HLT_j25']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=3.0, comment='Support, Express',ESRate=0.8), 
                    }])))



HLT_list=[
    # RD0
    # J15
    'HLT_j45',
    'HLT_j55',
    # J20
    'HLT_j85',
    # J15.31ETA49

    'HLT_j25_320eta490',
    'HLT_j35_320eta490',
    'HLT_j45_320eta490',
    'HLT_j55_320eta490',
    'HLT_j110_320eta490',

    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(rate=1.0, comment='Support'), 
                          }])))


HLT_list=['HLT_j60_320eta490']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(rate=1.0, comment='Support, Express', ESRate=0.2), 
                          }])))



# ----------------------------------------

L1_list=['L1_J15.31ETA49', 'L1_J20.28ETA31', 'L1_J50.31ETA49',]
jet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                          1000 : TriggerRule(rate=500, maxRate=500, comment='Support L1'),
                          }])))

L1_list=['L1_J20.31ETA49','L1_J30.31ETA49']
jet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                           1000 : TriggerRule(rate=500, maxRate=500, comment='Support L1'),
                           }])))
    
HLT_list=[
    # J20.28ETA31
    'HLT_j85_280eta320',
    'HLT_j85_280eta320_jes',
    'HLT_j85_280eta320_lcw',
    'HLT_j85_280eta320_lcw_jes',
    'HLT_j85_280eta320_lcw_nojcalib',
    'HLT_j85_280eta320_nojcalib',
    # J50.31ETA49
    'HLT_j175_320eta490_jes',
    'HLT_j175_320eta490_lcw',
    'HLT_j175_320eta490_lcw_jes',
    'HLT_j175_320eta490_lcw_nojcalib',
    'HLT_j175_320eta490_nojcalib',
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=0.2, comment='Support'), #SX1404 
                             }])))


HLT_list=['HLT_j175_320eta490', ] #SX1404
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=1.0, comment='Support'),
                             }])))


HLT_list=['HLT_j85_320eta490',]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=1.0, comment='Support'),
                             }])))

HLT_list=['HLT_j60_280eta320',]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=1.0, comment='Support Express', ESRate=0.2), 
                             }])))

HLT_list=['HLT_j60_L1RD0_FILLED',]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=0.2, comment='Support Express', ESRate=0.2), 
                             }])))




# ----------------------------------------
# j0_perf

HLT_list=['HLT_j0_perf_L1RD0_FILLED']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=0.5, comment='Support'),
                             }])))

# ========================================
# Djet
# 8 Hz. make sure what needs to be in CPS it is.

HLT_list=[
    # RD0_FILLED
    'HLT_j15_j15_320eta490',
    'HLT_j25_j25_320eta490',


    # J15_J15.31ETA49

    # J20_J20.31ETA49
    'HLT_j60_j60_320eta490', 
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=1.0, comment='Support'), 
                             }])))

HLT_list=['HLT_j45_j45_320eta490',
          ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(rate=1.0, comment='Support'), 
                             }])))

# ========================================
# prescaled multi jets
# 6 Hz

L1_list=['L1_3J15']
jet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                              1000 : TriggerRule(rate=1000, maxRate=1000, comment='Support'),
                              }])))

HLT_list=[
    'HLT_4j25', # RD0
    'HLT_4j45', # 3J15
    # 5j
    'HLT_5j25', # RD0
    # 6j
    'HLT_6j25', # RD0
    # 7j
    'HLT_7j25', # RD0
    ]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(rate=1.0, comment='Support'), 
                              }])))

# ----------------------------------------
# SUSY 0-lepton + multijets analysis request on 5j45
# - envisaged prescale: 7-10 (flat, i.e. no change)
# - envisaged period for the prescale change: early 50 ns data-taking for lumis <= 5e33

HLT_list=['HLT_5j45']
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(rate=1.0, comment='Support'), 
                              }])))

HLT_list=['HLT_j60_TT',]
jet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(rate=0.5, comment='Support ATR-14446'), 
                        }])))

########################################

jet_Rules={}

RulesList=[jet_Rules_2e34, jet_Rules_1e34, jet_Rules_0p5e34, jet_Rules_0p15e34, jet_Rules_supporting] 
 
for Rules in RulesList:
    for newrule in Rules.keys():
        if jet_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
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

bjet_Rules_2e34={}

HLT_list=["HLT_2j35_bmv2c2050_split_2j35_L14J20",

          "HLT_j55_bmv2c2050_split_ht500_L14J20", 
          "HLT_j55_bmv2c2050_split_ht500_L1HT190-J15.ETA21", 
          "HLT_j55_bmv2c2050_split_ht500_L1HT190-J15s5.ETA21",
          
          "HLT_2j55_bmv2c2050_split_ht300_L14J20", 
          "HLT_2j55_bmv2c2050_split_ht300_L1HT190-J15.ETA21", 
          "HLT_2j55_bmv2c2050_split_ht300_L1HT190-J15s5.ETA21",
          
          "HLT_3j35_bmv2c2077_split_j35", 
          "HLT_3j35_bmv2c2077_split_j35_L14J15.0ETA25",
          
          "HLT_j75_bmv2c2060_split_3j75_L14J20",

          "HLT_3j55_bmv2c2070_split_L13J35.0ETA23",
          "HLT_3j45_bmv2c2060_split_L13J35.0ETA23",

          "HLT_2j70_bmv2c2050_split_j70_L13J40",
          "HLT_2j75_bmv2c2060_split_j75_L13J40",          
] 
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary, backup 2e34'), 
}])))

HLT_list=[
    "HLT_3j35_bmv2c2060_split", 
    "HLT_3j55_bmv2c2070_split", 
    "HLT_3j35_bmv2c2060_split_L14J15.0ETA25", ] 
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary, 2e34 primaries'), 
}])))

#L1_4J20,4J15 is in jet_Rules_2e34

HLT_list=[
    # 1b 2e34
    'HLT_j360_bmv2c2085_split',
    'HLT_j300_bmv2c2070_split',
    'HLT_j225_bmv2c2050_split',

    # b+b 2e34
    'HLT_j150_bmv2c2050_split_j50_bmv2c2050_split',
    'HLT_j175_bmv2c2050_split_j50_bmv2c2050_split',

    # 1j+2b 2e34
    'HLT_j100_2j55_bmv2c2050_split',
    
    # 2b+3j 2e34
    'HLT_2j45_bmv2c2050_split_3j45_L14J15.0ETA25',
    'HLT_2j45_bmv2c2060_split_3j45_L14J15.0ETA25',
    'HLT_2j45_bmv2c2050_split_3j45',
    'HLT_2j45_bmv2c2060_split_3j45',

    # 2b+2j 2e34
    'HLT_2j35_bmv2c2050_split_2j35_L14J15',

    # 1/2b+ht 2e34, needs to tighten
    'HLT_j55_bmv2c2050_split_ht500_L14J15',
    'HLT_2j55_bmv2c2050_split_ht300_L14J15',

    # 1b+3j 2e34, needs to tighten
    'HLT_j75_bmv2c2060_split_3j75_L14J15',
    'HLT_j75_bmv2c2060_split_3j75_L14J15.0ETA25',

    ]
if startup2200b:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'), 
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'), 
                        }])))

HLT_list=[
    'HLT_2j35_bmv2c2050_split_2j35_L14J15.0ETA25',
]
if startup2200b:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'), 
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'), 
                        }])))

HLT_list=[
'HLT_j55_bmv2c2060_split_ht500_L1HT190-J15.ETA21',
'HLT_j55_bmv2c2060_split_ht500_L1HT190-J15s5.ETA21',
'HLT_2j55_bmv2c2070_split_ht300_L1HT190-J15.ETA21',
'HLT_2j55_bmv2c2070_split_ht300_L1HT190-J15s5.ETA21',
]
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                 1000 : TriggerRule(rate=1, comment='(ATR-15133), EndOfFill',rerun=0), 
                 }])))

HLT_list=[
    # b+b 1.5e34
    'HLT_j175_bmv2c2060_split_j50_bmv2c2050_split',

    # 2b+3j 1.5e34
    'HLT_2j45_bmv2c2070_split_3j45_L14J15.0ETA25',
    'HLT_2j45_bmv2c2070_split_3j45',
        
    # 2b+2j 1.5e34
    'HLT_2j35_bmv2c2060_split_2j35_L14J15',
    ]
if startup2200b:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else:  
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

HLT_list=[
    'HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25', # keep
]
if startup2200b: 
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'),
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'), 
                 }])))

HLT_list=[ 'HLT_j100_2j55_bmv2c2060_split', # keep
]
if startup2200b:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'),
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

HLT_list=[
    'HLT_j150_bmv2c2060_split_j50_bmv2c2060_split', # keep
]
if startup2200b: 
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'),
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

HLT_list=[
    'HLT_2j35_bmv2c2077_split_3j35', 
    'HLT_j300_bmv2c2085_split',
    'HLT_j175_bmv2c2050_split',

]
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'),
}])))

HLT_list=[
    'HLT_j225_bmv2c2060_split',
    'HLT_2j55_bmv2c2060_split_ht300_L14J15',    
    'HLT_j300_bmv2c2077_split',
    'HLT_2j45_bmv2c2077_split_3j45_L14J15.0ETA25',
    'HLT_2j45_bmv2c2077_split_3j45',
    'HLT_j75_bmv2c2070_split_3j75_L14J15.0ETA25',
    'HLT_j275_bmv2c2070_split',
    'HLT_j175_bmv2c2040_split',
]
if startup2200b:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary'),
                        14001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                        14001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
    
HLT_list=['HLT_j55_bmv2c2060_split_ht500_L14J15',
]
if startup2200b: # this chain is used by the ACR shifter to monitor when to switch to default keys
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{ 
                        1000 : TriggerRule(PS=10, comment='Primary'),
                        14001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                        14001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))


HLT_list=[
'HLT_2j45_bmv2c2060_split_2j45_L14J15.0ETA25',    
'HLT_j65_bmv2c2050_split_3j65_L14J15.0ETA25',
]
bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'),
}])))

HLT_list=[
    'HLT_j275_bmv2c2060_split',
    ]
if startup2200b:    
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:     
    bjet_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))

########################################
# 1e34 

bjet_Rules_1e34={}

L1_list=['L1_3J25.0ETA23']
bjet_Rules_1e34.update(dict(map(None,L1_list,len(L1_list)*[{
                           1000 : TriggerRule(PS=1, comment='Primary L1'), 
                           14001 : TriggerRule(PS=-1, comment='Disabled'), 
                           }])))

HLT_list=[

    'HLT_j65_bmv2c2070_split_3j65_L14J15',
    'HLT_j70_bmv2c2077_split_3j70_L14J15',     
    'HLT_j75_bmv2c2077_split_3j75_L14J15',
    'HLT_j75_bmv2c2070_split_3j75_L14J15',
    'HLT_2j45_bmv2c2070_split_2j45_L14J15',

    'HLT_j70_bmv2c2070_split_3j70_L14J15.0ETA25',
    
    ]
if startup2200b:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary'), 
                        #                           14001 : TriggerRule(PS=-1, comment='Disabled'), 
                        10001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        #                           14001 : TriggerRule(PS=-1, comment='Disabled'), 
                        10001 : TriggerRule(PS=-1, comment='Disabled'), 
                           }])))
# v6
HLT_list=[
    # L1_3J25.0ETA23 seeds
    'HLT_2j70_bmv2c2060_split_j70', 
    'HLT_2j75_bmv2c2070_split_j75', 

    # tighter version
    'HLT_2j70_bmv2c2050_split_j70',
    'HLT_2j75_bmv2c2060_split_j75',    
    ]
if startup2200b:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary'), 
                        14001 : TriggerRule(PS=-1, comment='Disabled'), 
                        #                           14001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        14001 : TriggerRule(PS=-1, comment='Disabled'), 
                        #                           14001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

HLT_list=[

    'HLT_2j70_bmv2c2070_split_j70',
    'HLT_2j75_bmv2c2077_split_j75',
    
    ]
if startup2200b:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary'), 
                        7001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        7001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

HLT_list=[

    'HLT_2j70_bmv2c2077_split_j70',
    'HLT_2j65_bmv2c2070_split_j65',
    
    ]
if startup2200b:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary'), 
                        5001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        5001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

HLT_list=['HLT_j100_2j55_bmv2c2070_split',
]
if startup2200b:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'), 
                        7001 : TriggerRule(PS=-1, comment='Disabled, rate is 30 Hz'),
                        }])))
else:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'), 
                        7001 : TriggerRule(PS=-1, comment='Disabled, rate is 30 Hz'),
                        }])))

# --------------------------------------

HLT_list=['HLT_mu4_j40_dr05_3j40_L14J20',]
bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=-1, comment='Disabled until lowers rate. Primary, VBF H->bb, hh->bbbb'), 
                      10001 : TriggerRule(PS=-1, comment='Disabled'), 
                      }])))


HLT_list=['HLT_j225_bmv2c2077_split','HLT_j225_bmv2c2085_split','HLT_j225_bmv2c2070_split'
]
if startup2200b:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-13386'), 
                        7001 : TriggerRule(PS=-1, comment='Disabled, rate is 35 Hz'), 
                        }])))
else:
    bjet_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13386'), 
                        7001 : TriggerRule(PS=-1, comment='Disabled, rate is 35 Hz'), 
                        }])))

########################################
# 0.5e34 

bjet_Rules_0p5e34={}

HLT_list=[

    'HLT_j100_2j55_bmv2c2077_split',
    'HLT_j150_bmv2c2077_split_j50_bmv2c2077_split',
    'HLT_j175_bmv2c2077_split_j60_bmv2c2077_split',

     'HLT_2j35_bmv2c2070_split_2j35_L14J15',
     'HLT_2j45_bmv2c2077_split_2j45_L14J15',
     'HLT_2j55_bmv2c2070_split_ht300_L14J15',
    
   
]
if startup2200b:
    bjet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment=''), 
                        5001 : TriggerRule(PS=-1, comment='ATR-13386'),
                        }])))
else:
    bjet_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment=''), 
                        5001 : TriggerRule(PS=-1, comment='ATR-13386'),
                        }])))


# --------------------------------------



########################################
# 0.3e34 

bjet_Rules_0p3e34={}

HLT_list=['HLT_j175_bmv2c2077_split',
    ]
if startup2200b:
    bjet_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary'), 
                        1000 : TriggerRule(PS=-1, comment='Disabled'),
                          }])))
else:
    bjet_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'), 
                        1000 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))

########################################
# 0.15e34 

bjet_Rules_0p15e34={}


########################################
# supporting

bjet_Rules_supporting={}



# ========================================
# lepton + boffperf + split,
# purpose is (data) to save sample of ttbar (using the main e/mu trigger, so no additional rate) for calibration, where jets and btagging info are prepared 

HLT_list=[
    'HLT_mu26_ivarmedium_2j35_boffperf_split', 
    'HLT_2mu14_2j35_boffperf_split',
    'HLT_2mu14_2j35_bperf_split',
    'HLT_mu26_ivarmedium_2j35_bperf_split', 
    'HLT_2mu14_2j35_boffperf',
    'HLT_mu26_ivarmedium_2j35_boffperf', 
    'HLT_2mu14_2j35_bperf',
    'HLT_mu26_ivarmedium_2j35_bperf',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Support'), 
                        }])))

HLT_list=[
    'HLT_2e17_lhloose_2j35_boffperf_split',
    'HLT_2e17_lhloose_2j35_bperf_split',
    'HLT_2e17_lhloose_2j35_boffperf',
    'HLT_2e17_lhloose_2j35_bperf',
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support'),
                   17001 : TriggerRule(PS=1, comment='Disabled, Needs to add 2EM18VH, 2EM20VH version'), 
                        }])))


HLT_list=[
        'HLT_e26_lhtight_nod0_ivarloose_2j35_boffperf', 
        'HLT_e26_lhtight_nod0_ivarloose_2j35_bperf',
        'HLT_e26_lhtight_nod0_ivarloose_2j35_bperf_split',
        'HLT_e26_lhtight_nod0_ivarloose_2j35_boffperf_split',
        'HLT_e28_lhtight_nod0_ivarloose_2j35_boffperf_split',  # 2e34
        'HLT_e28_lhtight_nod0_ivarloose_2j35_bperf',
        'HLT_e28_lhtight_nod0_ivarloose_2j35_bperf_split',
        'HLT_e28_lhtight_nod0_ivarloose_2j35_boffperf',
        'HLT_e30_lhtight_nod0_ivarloose_2j35_bperf',
        'HLT_e30_lhtight_nod0_ivarloose_2j35_bperf_split',
        'HLT_e30_lhtight_nod0_ivarloose_2j35_boffperf',
        'HLT_e30_lhtight_nod0_ivarloose_2j35_boffperf_split',
    ]
    
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Support'), 
                        }])))
    
# ========================================
# bperf

L1_list=['L1_3J15.0ETA25', 'L1_3J20']
bjet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(rate=1000, maxRate=1000, comment='Support L1'),
                        }])))

# are these in CPS? else, what's the point?

HLT_list=[
    # 3J15
    'HLT_j45_bperf_3j45',               'HLT_j45_bperf_split_3j45',               'HLT_j45_boffperf_3j45',               'HLT_j45_boffperf_split_3j45',
    # 3J15.0ETA25
    'HLT_j45_boffperf_split_3j45_L13J15.0ETA25',
    'HLT_j45_boffperf_split_3j45_L13J20',
    # 3J35.0ETA23
    "HLT_3j55_boffperf_split_L13J35.0ETA23",
    "HLT_3j45_boffperf_split_L13J35.0ETA23",
    # 3J40
    "HLT_3j55_boffperf_split",
    "HLT_3j35_boffperf_split",
    # 4J20
    'HLT_j55_bperf_3j55',    
    "HLT_j75_boffperf_split_3j75_L14J20",
    "HLT_3j35_boffperf_split_j35_L14J20",
    # 4J15.0ETA25
    "HLT_3j35_boffperf_split_L14J15.0ETA25",
    "HLT_3j35_boffperf_split_j35_L14J15.0ETA25",
    # 4J15
    "HLT_2j45_boffperf_split_3j45", 
    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(rate=0.1, comment='Support'), 
                            }])))

HLT_list=[
    'HLT_j25_boffperf_split',
    'HLT_j35_boffperf_split',
    'HLT_j45_boffperf_split',
    'HLT_j65_boffperf_split',
    'HLT_j85_boffperf_split',
    'HLT_j150_boffperf_split',
    'HLT_j175_boffperf_split',
    'HLT_j260_boffperf_split',
    'HLT_j320_boffperf_split',
    'HLT_j400_boffperf_split',
]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.3, comment='Support'), 
}])))



HLT_list=[
    'HLT_j15_boffperf',
    'HLT_j35_boffperf',
    'HLT_j55_boffperf',
    'HLT_j85_boffperf',
    'HLT_j150_boffperf',
    'HLT_j320_boffperf',
]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.1, comment='Support'), 
}])))

HLT_list=[
    'HLT_j15_bperf_split',
    'HLT_j55_bperf_split',
    'HLT_j85_bperf_split',
    'HLT_j150_bperf_split',

    'HLT_j320_bperf_split',
]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.05, comment='Support'), 
}])))

HLT_list=[
    'HLT_j15_bperf',
    'HLT_j55_bperf',
    'HLT_j85_bperf',
    'HLT_j150_bperf',
    'HLT_j320_bperf',
]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.05, comment='Support'), 
}])))

# purpose in data? # duplicated as the list above without ES

HLT_list=[
    # RD0
    'HLT_j15_boffperf_split', # Needs to define 'express' in menu first
    # J20
    'HLT_j55_boffperf_split',
    # J30
    'HLT_j110_boffperf_split',

    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(rate=0.6, comment='Support Express', ESRate=0.2), 
                           }])))

HLT_list=[ 
"HLT_2j70_boffperf_split_j70_L13J40",
"HLT_2j75_boffperf_split_j75_L13J40",
"HLT_2j55_boffperf_split_ht300_L14J15",
"HLT_j55_boffperf_split_ht500_L14J15",
"HLT_2j35_boffperf_split_2j35_L14J20",
"HLT_2j55_boffperf_split_ht300_L1HT190-J15.ETA21",
"HLT_j55_boffperf_split_ht500_L1HT190-J15.ETA21",
"HLT_2j55_boffperf_split_ht300_L1HT190-J15s5.ETA21",
"HLT_j55_boffperf_split_ht500_L1HT190-J15s5.ETA21",
"HLT_j175_boffperf_split_j50_boffperf_split",
"HLT_j150_boffperf_split_j50_boffperf_split",
]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(PS=-1, comment='Disable, new boffperf chains to be added appropriately'), 
                           }])))

L1_list=['L1_MU4_J12']
bjet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                         1000 : TriggerRule(rate=500, maxRate=500, comment='Support L1'),
                         }])))
L1_list=['L1_MU6_J20']
bjet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                         1000 : TriggerRule(rate=500, maxRate=500, comment='Support L1'),
                         }])))
L1_list=['L1_MU6_J40']
bjet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                         1000 : TriggerRule(rate=500, maxRate=500, comment='Support L1'),
                         }])))


L1_list=['L1_MU6_J75']
bjet_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                         1000 : TriggerRule(PS=1, comment='Support L1'),
                         }])))


HLT_list=[
    # MU4
    'HLT_mu4_j15_bperf_split_dr05_dz02',
    'HLT_mu4_j25_bperf_split_dr05_dz02',
    'HLT_mu4_j35_bperf_split',
    'HLT_mu4_j35_bperf_split_dr05',
    'HLT_mu4_j35_bperf_split_dr05_dz02',
    # MU6_J20
    'HLT_mu6_j85_bperf_split_dr05_dz02',
    'HLT_mu6_j110_bperf_split_dr05_dz02',
    # MU6_J40
    'HLT_mu6_j150_bperf_split',
    'HLT_mu6_j150_bperf_split_dr05',
    'HLT_mu6_j150_bperf_split_dr05_dz02',
    'HLT_mu6_j175_bperf_split_dr05_dz02',
    # MU6_J75
    'HLT_mu6_j260_bperf_split_dr05_dz02',
    'HLT_mu6_j320_bperf_split',
    'HLT_mu6_j320_bperf_split_dr05',
    'HLT_mu6_j320_bperf_split_dr05_dz02',
    'HLT_mu6_j400_bperf_split_dr05_dz02',

    ]
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(rate=1.0, comment='Support'),
                       }])))


    # MU4_J12
HLT_list=['HLT_mu4_j55_bperf_split_dr05_dz02']
bjet_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(rate=1.0, comment='Support, Express', ESRate=0.2),
                       }])))



########################################

bjet_Rules={}

RulesList=[bjet_Rules_2e34, bjet_Rules_1e34, bjet_Rules_0p5e34, bjet_Rules_0p3e34, bjet_Rules_0p15e34, bjet_Rules_supporting] 
 
for Rules in RulesList:
    for newrule in Rules.keys():
        if bjet_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule

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

tau_Rules_2e34={}


HLT_list=[
          "HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25", 
          'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',

          'HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25',
          ] 
if NoL1Topo :
    tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            }])))


HLT_list=[
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I-J25', #15 Hz #20Hz for nonL1Topo
    "HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I",
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I',
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I', # 27Hz
    ] 
if NoL1Topo :
    tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary'),
                       17001 : TriggerRule(PS=-1, comment='Disabled'), 
                            }])))
    
HLT_list=['HLT_noalg_l1topo_L1LAR-EM','HLT_noalg_l1topo_L1LAR-J',]
if NoL1Topo :
    tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))    
L1_list=[
    # single
    'L1_TAU100',
    # tau + xe, w/ jet
    'L1_TAU20IM_2J20_XE45',
    # di-tau + xe
    # tau + e + xe
    'L1_EM15HI_2TAU12IM_XE35',
    # tau + mu + xe
    'L1_MU10_TAU12IM_XE35',
    ]

tau_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary L1'), 
    }])))

L1_list=[# di-tau + xe
        'L1_TAU20IM_2TAU12IM_XE35',
    ]
tau_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   15001 : TriggerRule(PS=2, comment='Support L1, Backup(tmp). Needs to add tighter primary'), 
    }])))

HLT_list=[
    # single tau
    "HLT_tau160_medium1_tracktwo_L1TAU100",         
    "HLT_tau200_medium1_tracktwo_L1TAU100",
    # high-pT di-tau from single Tau
    'HLT_tau125_medium1_tracktwo_tau50_medium1_tracktwo_L1TAU12',
    # tau + xe
    'HLT_tau35_medium1_tracktwo_xe70_L1XE45',
    # di-tau + xe
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_xe50',
    # tau + e + xe
    'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50',
    'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_xe50',
    # tau + mu iso (high et, w/o jet)
    'HLT_mu14_ivarloose_tau35_medium1_tracktwo_L1MU10_TAU20IM_J25_2J20', #backup for SOS
    # tau + mu + xe
    'HLT_mu14_ivarloose_tau25_medium1_tracktwo_xe50',
    'HLT_mu14_tau25_medium1_tracktwo_xe50',
    # tau tau high pt
    'HLT_tau80_medium1_tracktwo_L1TAU60_tau50_medium1_tracktwo_L1TAU12',
    # tau + e (high et, w/o jet) seed L1_EM20VHI_TAU20IM_2TAU20_J25_3J20
    "HLT_e24_lhmedium_nod0_ivarloose_tau35_medium1_tracktwo",
    ]

tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary'),
    20001 : TriggerRule(PS=-1, comment='Disable at 2e34'), 
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
                   17001 : TriggerRule(PS=-1, comment='Disable at 1.7e34'), 
    }])))
    
HLT_list=[
    'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_xe50',
    ]

tau_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled - ATR14490'), 
    }])))

########################################
# 1p5e34 

tau_Rules_1p5e34={}

#L1 topo (more efficient than tau+X+jets regular)

HLT_list=[#'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I', #30 Hz
          # Not in phys menu 'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_L1DR-EM15TAU12I', #12 Hz
          # Not in phys menu 'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_L1DR-EM15TAU12I',
          # Not in phys menu 'HLT_mu14_tau25_medium1_tracktwo_L1MU10_TAU12I-J25', # 30 Hz
          ] 
if NoHLTTopo :
    tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))
else :
    tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            17001 : TriggerRule(PS=-1, comment='Disable'),
                            }])))
          
# --------------------------------------
HLT_list=[
    # tau + e (high et, w/o jet)
    'HLT_e17_lhmedium_nod0_ivarloose_tau35_medium1_tracktwo_L1EM15HI_TAU20IM_2TAU15_J25_2J20_3J15', #backup for SOS
    # tau + mu (w/o jet also at L1)
    'HLT_mu14_ivarloose_tau35_medium1_tracktwo',
    ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            17001 : TriggerRule(PS=-1, comment='Disable, huge L1 rate, 5kHz at 2e34'),
                            }])))
    
L1_list=['L1_EM15HI_2TAU12IM_J25_3J12',# 3000 Hz
         ] 
if emergency2200b:
    tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Emergency 2200b'), 
                        }])))
else:
    tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'), 
                        12001 : TriggerRule(PS=-1, comment='High lumi PS 2200b'),
                        17001 : TriggerRule(rate=500, maxRate=500, comment='Support'),
                        }])))

HLT_list=['HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo',
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14490'), 
                            }])))

# due to tau+e+xe, for trigger efficiency measurement (ATR-12219)
HLT_list=[
          'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo',
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary'), 
                            17001 : TriggerRule(rate=0.1, comment='Support, ATR-12219', rerun=1),
                            }])))

L1_list=[
    # tau + e (high et, w/o jet)
    'L1_EM15HI_TAU40_2TAU15',
    ]
if emergency2200b:
    tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled, no users, SOS in 1.5e34'),
                        }])))
else:
    tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'),
                        12001 : TriggerRule(PS=-1, comment='High Lumi PS 2220b'), 
                        17001 : TriggerRule(PS=-1, comment='Disabled, no users, SOS in 1.5e34'),
                        }])))

HLT_list=['HLT_e17_lhmedium_nod0_tau80_medium1_tracktwo',
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                            1000 : TriggerRule(PS=1, comment='Primary L1'), 
                            17001 : TriggerRule(PS=-1, comment='Disabled, no users, SOS in 1.5e34'),
                            }])))


# --------------------------------------

L1_list=['L1_MU10_TAU12IM_J25_2J12',] # 1500 Hz
if emergency2200b:
    tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=-1, maxRate=1900, comment='emergency 2200b, Support'),  # why is this needed?
                        }])))
else:
    tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary L1'), 
                        12001 : TriggerRule(PS=-1, maxRate=1900, comment='High Lumi PS 2220b'),
                        17001 : TriggerRule(rate=500, maxRate=500, comment='Support'),  # why is this needed?
                        }])))
    
# rerun due to tau+mu+xe, for trigger efficiency measurement (ATR-12219) # 15 and 37 Hz
HLT_list=['HLT_mu14_ivarloose_tau25_medium1_tracktwo',
          ]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(PS=1, comment='Primary'), 
                             17001 : TriggerRule(rate=0.1, comment='Support, ATR-12219', rerun=1),
                             }])))

# --------------------------------------

L1_list=['L1_TAU20IM_2TAU12IM_J25_2J20_3J12',] # 6200 Hz
tau_Rules_1p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                              1000 : TriggerRule(PS=1, comment='Primary L1'), 
                              12001 : TriggerRule(rate=500, maxRate=500, comment='Support'),
                              }])))

# rerun due to tau+tau+xe, for trigger efficiency measurement(ATR-12219)  20 Hz
HLT_list=['HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo',
          ] 
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(PS=1, comment='Primary', ESRate=0.2), 
                              17001 : TriggerRule(rate=0.2, comment='Support, ATR-12219', ESValue=1, rerun=1),
                              }])))

HLT_list=[          'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30',
          ] 
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(PS=1, comment='Primary'), 
                              17001 : TriggerRule(rate=0.2, comment='Support, ATR-12219'),
                              }])))

HLT_list=['HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo',
          "HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30",]
tau_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                              1000 : TriggerRule(PS=1, comment=''), 
                              17001 : TriggerRule(rate=0.1, comment='Prescaled'),
                              }])))

########################################
# 1e34 

tau_Rules_1e34={}

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

tau_Rules_0p7e34={}

HLT_list=['HLT_tau35_loose1_tracktwo_tau25_loose1_tracktwo',]
tau_Rules_0p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=1, comment='Primary'), 
                          7001 : TriggerRule(rate=0.1, comment='Support'), # is higher rate needed ? for background determination
                          }])))

# --------------------------------------

L1_list=['L1_TAU20_2TAU12_XE35']
tau_Rules_0p7e34.update(dict(map(None,L1_list,len(L1_list)*[{
                         1000 : TriggerRule(PS=1, comment='Primary L1'), 
                         7001 : TriggerRule(PS=-1, comment='Disabled'),
                         }])))

HLT_list=['HLT_tau35_medium1_tracktwo_L1TAU20_tau25_medium1_tracktwo_L1TAU12_xe50',]
tau_Rules_0p7e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=1, comment='Primary'), 
                         7001 : TriggerRule(PS=-1, comment='Disabled'),
                         }])))


########################################

# 0.5e34 

tau_Rules_0p5e34={}

L1_list=['L1_TAU40']
tau_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary L1'), 
                     7001 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))

# --------------------------------------

HLT_list=['HLT_tau80_medium1_tracktwo_L1TAU60',]
tau_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     5001 : TriggerRule(PS=0, comment='Disabled', rerun=1),
                     }])))


########################################
# 0.3e34 

tau_Rules_0p3e34={}

HLT_list=['HLT_tau80_medium1_tracktwo', ]
tau_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     1000 : TriggerRule(PS=0, comment='Support', rerun=1),
                     }])))

HLT_list=[
    'HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_L1EM15TAU12I-J25', 'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20ITAU12I-J25',
    ]

if NoHLTTopo:
    tau_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled, ATR-13366'),
                        }])))
else:
    tau_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-13366'),
                       17001 : TriggerRule(PS=5, comment='Support Commission L1Topo, ATR-13366'),
                        }])))
    

# --------------------------------------

# due to tau+tau+xe, for trigger efficiency measurement  (ATR-12219)
HLT_list=['HLT_tau35_tight1_tracktwo_tau25_tight1_tracktwo_L1TAU20IM_2TAU12IM',]
tau_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=-1, comment='Disabled'),
                         }])))


########################################
# 0.15e34 

tau_Rules_0p15e34={}

HLT_list=['HLT_tau35_loose1_tracktwo_tau25_loose1_tracktwo_L1TAU20IM_2TAU12IM',]
tau_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     1000 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))


# --------------------------------------

L1_list=['L1_TAU30']
tau_Rules_0p15e34.update(dict(map(None,L1_list,len(L1_list)*[{
                     1000 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))

# --------------------------------------

L1_list=[
    'L1_TAU20_2TAU12',
    'L1_MU10_TAU20',
    # tau + xe, w/ jet
    'L1_TAU20_2J20_XE45',
    ]
tau_Rules_0p15e34.update(dict(map(None,L1_list,len(L1_list)*[{
                     1000 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))

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
    'HLT_tau0_perf_ptonly_L1TAU60'                : {  1000 : TriggerRule(rate=0.2, comment='Support Express', ESValue=1) }, 
    # --------------------------------------
    # single & di-tau
    'HLT_tau25_medium1_tracktwo'                  : {  1000 : TriggerRule(rate=0.5, comment='Support Express', ESValue=1, rerun=1) },
    'HLT_tau25_perf_tracktwo'                     : {  1000 : TriggerRule(rate=0.5, comment='Support Express', ESValue=1, rerun=1) }, 
    'HLT_tau35_perf_tracktwo_tau25_perf_tracktwo' : {  1000 : TriggerRule(rate=0.2, comment='Support Express', ESValue=1, rerun=1) },
    
    # --------------------------------------
    # Z T&P
    # assign rerun (ATR-12219)
    'HLT_mu14_iloose_tau25_perf_tracktwo'         : {  1000 : TriggerRule(rate=0.4, comment='Support Express', ESValue=1, rerun=1) },
    
    # --------------------------------------
    # idperf
    'HLT_tau25_idperf_tracktwo'                   : {  1000 : TriggerRule(rate=0.5, comment='Support Express', ESValue=1, rerun=1) }, 
    'HLT_tau25_idperf_track'                      : {  1000 : TriggerRule(rate=0.5, comment='Support Express', ESValue=1, rerun=1) }, 
    'HLT_tau160_idperf_tracktwo'                  : {  1000 : TriggerRule(rate=0.3, comment='Support Express', ESValue=1, rerun=1) },
    'HLT_tau160_idperf_track'                     : {  1000 : TriggerRule(rate=0.3, comment='Support Express', ESValue=1, rerun=1) },
    'HLT_tau160_perf_tracktwo'                     : {  1000 : TriggerRule(rate=0.3, comment='Support Express', ESValue=1, rerun=1) },
    }  

# ========================================


HLT_list=[
    'HLT_tau25_loose1_tracktwo', 'HLT_tau25_loose1_ptonly',
    'HLT_tau25_medium1_ptonly',  'HLT_tau25_medium1_track', 
    'HLT_tau25_medium1_tracktwo_L1TAU12',
    'HLT_tau25_perf_ptonly',           'HLT_tau25_perf_track',
    'HLT_tau25_perf_tracktwo_L1TAU12',   'HLT_tau25_perf_ptonly_L1TAU12',  
    'HLT_tau25_tight1_tracktwo', 'HLT_tau25_tight1_ptonly',
    'HLT_tau35_loose1_tracktwo',        
    'HLT_tau35_medium1_tracktwo', 
    'HLT_tau50_medium1_tracktwo_L1TAU12',
    
    'HLT_tau160_medium1HighptL_tracktwo',
    'HLT_tau160_medium1HighptM_tracktwo',
    'HLT_tau160_medium1HighptH_tracktwo',

    'HLT_tau35_perf_tracktwo_tau25_perf_tracktwo_ditauM', #ATR-13494
]
tau_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1), 
}])))



HLT_list = ['HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12IM',  #0.2Hz
            'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15HI_2TAU12IM', #0.2Hz
          ]
tau_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, rerun=1, comment='Support for L1Topo (ATR-14349)'), 
    }])))

# need to add ESRate=0.2 
HLT_list = ['HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM'] #HLT 77Hz #L1 15kHz
tau_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1, ESRate=0.2, rerun=1, comment='Support for L1Topo (ATR-14349)'), 
    }])))


########################################

tau_Rules={}

RulesList=[tau_Rules_2e34, tau_Rules_1p5e34, tau_Rules_1e34, tau_Rules_0p7e34, tau_Rules_0p5e34, tau_Rules_0p3e34, tau_Rules_0p15e34, tau_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if tau_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
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

egamma_Rules_2e34={}

L1_list=['L1_EM26VHI',]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   }])))
HLT_list=[
"HLT_e30_lhtight_nod0_ivarloose",
"HLT_e30_lhtight_nod0_ringer_ivarloose",
"HLT_e60_lhmedium_nod0_L1EM26VHI",
"HLT_e140_lhloose_nod0_L1EM26VHI",
"HLT_e300_etcut_L1EM26VHI",
# 1g
"HLT_g300_etcut_L1EM26VHI",
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

L1_list=['L1_EM24VHI',]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                   }])))

HLT_list=[
    'HLT_e28_lhtight_smooth_ivarloose',
    'HLT_e28_lhtight_nod0_ivarloose',
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    }])))

HLT_list=[
    'HLT_e28_lhtight_nod0_ringer_ivarloose',
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, Commission'), 
                    }])))

HLT_list=['HLT_e32_lhtight_nod0_ivarloose', 'HLT_e35_lhtight_nod0_ivarloose', 'HLT_e40_lhtight_nod0_ivarloose', 'HLT_e50_lhtight_nod0_ivarloose',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='ATR-15201, ATR-15236'),
                    }])))

HLT_list=[
    'HLT_e60_medium_L1EM24VHI',
    'HLT_e60_lhmedium_nod0_L1EM24VHI',
    'HLT_e140_lhloose_nod0_L1EM24VHI',
    'HLT_e300_etcut_L1EM24VHI',
    # 1g
    'HLT_g300_etcut_L1EM24VHI',
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='Primary')
    }])))

HLT_list=[
    'HLT_g200_loose','HLT_g140_tight', #EM24VHI seeds
    "HLT_g200_loose_L1EM26VHI", 
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary '),
                    }])))

## 2EM
L1_list=['L1_2EM20VH']
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   }])))

HLT_list=[
    "HLT_2e22_lhvloose_nod0", #L1_2EM20VH seed 
    "HLT_3g22_loose_L12EM20VH",
    "HLT_g35_medium_g25_medium_L12EM20VH",
    "HLT_e24_lhmedium_nod0_L1EM20VH_g25_medium",
    "HLT_2g50_loose_L12EM20VH", "HLT_2g60_loose_L12EM20VH",
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34, 2EM20VH seeds'), 
                    }])))

L1_list=['L1_2EM18VH']
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   17001 : TriggerRule(PS=-1, comment='Disabled'), 
                   }])))


HLT_list=[
    "HLT_2e20_lhvloose_nod0", #L1_2EM18VH seed 
    "HLT_3g20_loose_L12EM18VH",
    "HLT_g35_medium_g25_medium_L12EM18VH",
    "HLT_e20_lhmedium_nod0_g35_loose_L12EM18VH",
    "HLT_e24_lhmedium_nod0_L1EM18VH_g25_medium",
    "HLT_2g50_loose_L12EM18VH", "HLT_2g60_loose_L12EM18VH", 
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34 if possible, 2EM18VH seeds'),
                    17001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

L1_list=['L1_2EM15VH']
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary L1'),
                   12001 : TriggerRule(PS=-1, comment='Disabled'), 
                   }])))

HLT_list=[    
    # 2g
    'HLT_2g60_loose_L12EM15VH',
    'HLT_2g50_loose', # ATR-13866    
    'HLT_2g22_tight',
    #'HLT_2g25_tight', not in the menu
    # 3g
    'HLT_3g20_loose',
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   17001 : TriggerRule(PS=-1, comment='Disabled'),
}])))

HLT_list=[ 'HLT_2e17_lhvloose_nod0',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Express', ESRate=0.2),
                    17001 : TriggerRule(PS=-1, comment='Disabled', ESRate=0),
}])))

HLT_list=['HLT_2e17_lhloose']
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary', ESValue=0),
                   17001 : TriggerRule(PS=-1, comment='Disabled', ESRate=0),
                       }])))

HLT_list=['HLT_g35_medium_g25_medium']
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary', ESRate=0.4),
                   17001 : TriggerRule(PS=-1, comment='Disabled', ESRate=0),
                         }])))


L1_list=["L1_EM15VH_3EM10VH"]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   15001 : TriggerRule(PS=4, comment='Support Backup(tmp). Needs to add tighter primary'),
                         }])))


## 3EM
HLT_list=[    
    "HLT_e17_lhloose_nod0_2e12_lhloose_nod0_L1EM15VH_3EM10VH", 
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34 backup, EM18VH_3EM8VH EM15VH_3EM10VH seeds'), 
                    }])))


# Needs to add e+2g version
HLT_list=[    
    "HLT_e17_lhloose_nod0_2e12_lhloose_nod0_L1EM15VH_3EM10VH",     
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary 2e34 backup, EM18VH_3EM8VH EM15VH_3EM10VH seeds'), 
                    }])))

HLT_list=[
    "HLT_e20_lhloose_nod0_2e10_lhloose_nod0_L1EM18VH_3EM8VH", 
    "HLT_e20_lhloose_nod0_e15_lhloose_nod0_e10_lhloose_nod0_L1EM18VH_3EM8VH", 
    "HLT_e20_lhmedium_nod0_2g10_loose_L1EM18VH_3EM8VH", 
    "HLT_e35_lhloose_nod0_e25_lhloose_nod0_e15_lhloose_nod0_L1EM18VH_3EM8VH",
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, EM18VH_3EM8VH seeds'),
                    12001 : TriggerRule(PS=10, comment='Support, place holder not to forget. Huge L1 rate'), 
                    }])))


# --------------------------------------
# 1.5e34 menu
L1_list=['L1_EM22VHI',]
egamma_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary L1'), 
                  17001 : TriggerRule(rate=1000, comment='Support'), 
                   }])))

HLT_list=[
    'HLT_e26_lhtight_smooth_ivarloose',
    'HLT_e28_lhtight_nod0_ivarloose_L1EM22VHI',    
    ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary'), 
                  17001 : TriggerRule(PS=-1, comment='Disabled')}])))

#target rate of 0.2 to ES with no lumi-dependence
HLT_list=['HLT_e26_lhtight_nod0_ivarloose']
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=1, comment='Primary', ESRate=0.4), 
                  17001 : TriggerRule(rate=0.2, comment='Support', ESValue=1)}])))

HLT_list=[
    'HLT_e60_medium',    
    'HLT_e60_lhmedium_nod0',
    #'HLT_e80_medium', # not presente in the menu right now
    'HLT_e140_lhloose_nod0',
    # need to add alternative for when EM22VHI is prescaled ==> missing EM24VHI seeded one
    'HLT_e300_etcut',
    
    # 1g
    # need to add alternative for when EM22VHI is prescaled ==> missing EM24VHI seeded one
    'HLT_g300_etcut',
]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   17001 : TriggerRule(PS=-1, comment='Disabled'),
}])))


# --------------------------------------
HLT_list=['HLT_g140_loose',]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Express', ESRate=0.2),
                   17001 : TriggerRule(PS=-1, comment='Disabled', ESRate=0),
                    }])))

HLT_list=['HLT_e26_lhtight_idperf' ]
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support Tracking Validation', ESValue=1),                    
                    }])))

HLT_list=['HLT_e28_lhtight_idperf', 'HLT_e30_lhtight_idperf']
egamma_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.5, comment='Support Tracking Validation', ESValue=0),
                    }])))


########################################
# runs up to 1e34 :

egamma_Rules_1e34={}

HLT_list=['HLT_e24_lhtight_nod0_ivarloose']
egamma_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=['HLT_e24_lhtight_iloose','HLT_e24_lhtight_nod0_iloose','HLT_e24_lhtight_ivarloose',
          ]
egamma_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

#-------

HLT_list=[
    # 2g
    'HLT_2g20_tight',
    # 3g
    'HLT_2g20_loose_g15_loose',
    ]
egamma_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    10001 : TriggerRule(PS=-1, comment='Disabled'),
    }])))


# --------------------------------------

HLT_list=[
          'HLT_2e15_lhvloose_nod0_L12EM13VH',]
egamma_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary'), 
                       10001 : TriggerRule(PS=-1, comment='Disabled'),
                       }])))

########################################
# 0.5e34 

egamma_Rules_0p5e34={}


# --------------------------------------
HLT_list=['HLT_e24_lhmedium_nod0_iloose']
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1,  comment='Primary'), 
                       5001 : TriggerRule(PS=-1, comment='Disabled'),
                       }])))
           
HLT_list=['HLT_e24_lhmedium_iloose','HLT_e24_lhmedium_ivarloose','HLT_e24_lhmedium_nod0_ivarloose', 
          ]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1,  comment='Primary'), 
                       5001 : TriggerRule(PS=-1, comment='Disabled'),
                       }])))
# --------------------------------------
# keep some L1 rate as support,500 Hz, when prescaled. Ask why?
L1_list=['L1_EM20VH']
egamma_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(rate=2000, comment='Primary Commission depending on L1 IBL limit'),
                    3001 : TriggerRule(rate=500, maxRate=500, comment='Support'),
                    }])))

# --------------------------------------
# is this needed, to run in paralle with e24_lhmedium_iloose? would it not be better to keep it PS, low rate, for commissioning only? maybe PS=1 only at start?

HLT_list=['HLT_e24_lhmedium_iloose_L1EM20VH','HLT_e24_lhmedium_nod0_iloose_L1EM20VH',
          'HLT_e24_lhmedium_ivarloose_L1EM20VH',
          'HLT_e24_lhmedium_nod0_ivarloose_L1EM20VH',
          ]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    3001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))




HLT_list=['HLT_e24_lhmedium_L1EM20VH']
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1,  comment='Primary'), 
                    3001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))
        

HLT_list=['HLT_3g15_loose',]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'), 
                    5001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

# --------------------------------------
# keep some rate as support,0.5 Hz, when prescaled. Ask if ok.

HLT_list=['HLT_g35_loose_g25_loose', "HLT_g35_loose_g25_loose_L12EM18VH", "HLT_g35_loose_g25_loose_L12EM20VH", ]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=1, comment='Primary until ICHEP dataset', ESValue=0), 
                         14001 : TriggerRule(rate=0.6, comment='Support', ESValue=0),
                         }])))

# --------------------------------------
# keep some rate as support,0.5 Hz, when prescaled. Ask if ok.

HLT_list=['HLT_g120_loose',]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary Express', ESRate=0), 
                      5001 : TriggerRule(rate=0.5, comment='Support',rerun=1, ESValue=0),
                      }])))

HLT_list=['HLT_e120_lhloose','HLT_e120_lhloose_nod0',]
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      5001 : TriggerRule(PS=-1, comment='Disabled'),
                      }])))


# --------------------------------------

L1_list=['L1_2EM10VH']
egamma_Rules_0p5e34.update(dict(map(None,L1_list,len(L1_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary L1'),
                       5001 : TriggerRule(PS=-1, comment='Disabled'),
                       }])))

HLT_list=['HLT_2e12_lhvloose_L12EM10VH', 'HLT_2e12_lhvloose_nod0_L12EM10VH']
egamma_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary'),
                       5001 : TriggerRule(PS=-1, comment='Disabled', ESValue=0),
                       }])))


########################################
# 0.3e34 

egamma_Rules_0p3e34={}

# --------------------------------------

# keep some L1 rate as support,500 Hz, when prescaled. Ask why
L1_list=['L1_2EM15']
egamma_Rules_0p3e34.update(dict(map(None,L1_list,len(L1_list)*[{
                     1000 : TriggerRule(rate=500, maxRate=500, comment='Prescaled'),
                     }])))

HLT_list=['HLT_g35_loose_L1EM15_g25_loose_L1EM15',]
egamma_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     3001 : TriggerRule(PS=-1, comment='Disabled'),
                     }])))


egamma_Rules_0p15e34={}
# keep some L1 rate as support,500 Hz, when prescaled. Ask why
L1_list=['L1_EM18VH']
egamma_Rules_0p15e34.update(dict(map(None,L1_list,len(L1_list)*[{
                      1000 : TriggerRule(rate=500, maxRate=500, comment='Support L1'),
                      }])))

HLT_list=['HLT_e24_lhmedium_iloose_L1EM18VH','HLT_e24_lhmedium_nod0_iloose_L1EM18VH',
          'HLT_e24_lhmedium_ivarloose_L1EM18VH',
          'HLT_e24_lhmedium_nod0_ivarloose_L1EM18VH',
          ]
egamma_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=-1, comment='Disabled'),
                      }])))

HLT_list=['HLT_e24_lhmedium_nod0_ringer_iloose',
          'HLT_e24_lhmedium_nod0_ringer_ivarloose',
          ]
egamma_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))


########################################
# supporting

egamma_Rules_supporting={}


HLT_list=['HLT_e24_lhmedium_L1EM20VHI',
          'HLT_e24_lhmedium_cutd0dphideta_L1EM20VH',
          'HLT_e24_lhmedium_cutd0dphideta_iloose_L1EM20VH',
          'HLT_e26_lhtight_cutd0dphideta_ivarloose',
          "HLT_e30_lhtight_cutd0dphideta_ivarloose",
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000                  : TriggerRule(PS=-1, comment='Support Commission ATR-13493'),
                    }])))


egamma_Rules_supporting.update({
          'HLT_e24_lhmedium_nod0_L1EM20VH'              : {  1000 : TriggerRule(PS=-1, rerun=1, comment='Disabled') ,
                                                             17001 : TriggerRule(PS=-1, comment='Disabled') },
          'HLT_e26_lhmedium_nod0_L1EM20VH'              : {  1000 : TriggerRule(PS=1,  rerun=1, comment='Support, targer 4Hz') ,
                                                             17001 : TriggerRule(PS=-1, comment='Disabled') },
          'HLT_e28_lhmedium_nod0_L1EM20VH'              : {  1000 : TriggerRule(PS=1,  rerun=1, comment='Support, targer 4Hz') ,
                                                             17001 : TriggerRule(PS=-1, comment='Disabled') },
          "HLT_e30_lhmedium_nod0_L1EM20VH"             : {  1000 : TriggerRule(PS=1,  rerun=1, comment='Support, targer 4Hz') ,
                                                           20001 : TriggerRule(PS=-1, comment='Disabled') },
          
          'HLT_e24_lhvloose_nod0_L1EM20VH'              : {  1000 : TriggerRule(PS=-1, comment='Disabled') ,
                                                             17001 : TriggerRule(PS=-1, comment='Disabled') },
          'HLT_e26_lhvloose_nod0_L1EM20VH'              : {  1000 : TriggerRule(PS=9, comment='Support, targer 1Hz') ,
                                                             17001 : TriggerRule(PS=-1, comment='Disabled') },
          'HLT_e28_lhvloose_nod0_L1EM20VH'              : {  1000 : TriggerRule(PS=7, comment='Support, targer 1Hz') ,
                                                             17001 : TriggerRule(PS=-1, comment='Disabled') },
          "HLT_e30_lhvloose_nod0_L1EM20VH"             : {  1000 : TriggerRule(PS=5,  rerun=1, comment='Support, targer 1Hz') ,
                                                            20001 : TriggerRule(PS=-1, comment='Disabled') },

})


# ----------------------------------------
# di-electron
# what is the point of this? 

HLT_list=[
          'HLT_2e17_lhloose_L12EM15',
          'HLT_2e17_lhloose_nod0_L12EM15',
          ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=0.2, comment='Support'),
                    }])))

# ========================================
# non isolated single electron for background

# Exceptionally, e24_(lh)medium_L1EM18VH is kept alive until L1_EM18VH is prescaled. Is this still needed?
HLT_list=['HLT_e24_lhmedium_L1EM18VH']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(rate=0.2, comment='Support Express',ESValue=0),
                      }])))

HLT_list=['HLT_e24_lhmedium_nod0_L1EM18VH']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(rate=0.2, comment='Support'),
                       }])))
# --------------------------------------

# Exceptionally, e24_(lh)medium_L1EM20VH is kept alive until L1_EM20VH is prescaled
# --> tight is also kept alive (was PS=10 before)
# is this needed? would it not be better to keep it PS, low rate, for commissioning only? maybe PS=1 only at start?

HLT_list=['HLT_e24_lhtight_L1EM20VH',]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary', ESValue=0),
                      3001 : TriggerRule(rate=0.2, comment='Support Express', ESValue=0),
                      }])))

HLT_list=['HLT_e24_lhtight_nod0_L1EM20VH',]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'),
                      3001 : TriggerRule(rate=0.2, comment='Support'),
                      }])))


HLT_list=[
    'HLT_e28_tight_iloose', 
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=5, rerun=1, comment='Support, ATR-13378'), 
                    }])))

HLT_list= ["HLT_e26_lhtight_nod0", "HLT_e28_lhtight_nod0", "HLT_e30_lhtight_nod0",]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                   1000 : TriggerRule(PS=-1, comment='Disabled'), 
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
    'HLT_e200_etcut',
    'HLT_e250_etcut',

    'HLT_e60_lhvloose_nod0',
    'HLT_e140_etcut',
    'HLT_e140_lhvloose',
    'HLT_e140_lhvloose_nod0',
    'HLT_g200_etcut',
    'HLT_g250_etcut',
    
    "HLT_e80_lhloose_nod0_L1EM24VHI", 
    ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=1.0, comment='Support'), 
                    }])))

# up to 5 Hz, is this really needed?
# all this rerun potentially very CPU expensive. is this needed?


HLT_list=[
    'HLT_e5_lhvloose_nod0',
    'HLT_e10_lhvloose_L1EM7',
    'HLT_e15_lhvloose_L1EM7',
    'HLT_e20_lhvloose_L1EM12',
    'HLT_e25_lhvloose_L1EM15',
    'HLT_e30_lhvloose_L1EM15',
    'HLT_e40_lhvloose_L1EM15',
    'HLT_e50_lhvloose_L1EM15',
    'HLT_e70_lhvloose',
    'HLT_e80_lhvloose',
    'HLT_e100_lhvloose',
    'HLT_e120_lhvloose',
    'HLT_e120_lhvloose_nod0',
    
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.2, comment='Support',rerun=1), 
}])))

# 1 Hz SUSY strong-1l request. motivation?
HLT_list=['HLT_e5_lhvloose']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, comment='Support',rerun=1), 
}])))

# --------------------------------------
# physics - background estimation
# how much rate does this sum to? is this needed? and why not use the rule with rate=1 Hz instead of these ad-hoc prescales?
# all this rerun potentially very CPU expensive. is this needed?

egamma_Rules_supporting.update({
    'HLT_e12_lhvloose_L1EM10VH'       : {  1000 : TriggerRule(PS=8800, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e12_lhvloose_nod0_L1EM10VH'  : {  1000 : TriggerRule(PS=8800, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e15_lhvloose_L1EM13VH'       : {  1000 : TriggerRule(PS=3040, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e15_lhvloose_nod0_L1EM13VH'  : {  1000 : TriggerRule(PS=3040, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e17_lhvloose'                : {  1000 : TriggerRule(PS=1920, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e17_lhvloose_nod0'           : {  1000 : TriggerRule(PS=1920, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e20_lhvloose'                : {  1000 : TriggerRule(PS=1600, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e20_lhvloose_nod0'           : {  1000 : TriggerRule(PS=1600, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e40_lhvloose'                : {  1000 : TriggerRule(PS=45, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e40_lhvloose_nod0'           : {  1000 : TriggerRule(PS=45, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e60_lhvloose'                : {  1000 : TriggerRule(PS=48, comment='Support, 1 Hz target at 1.2e34', rerun=1) },
    'HLT_e24_lhvloose_nod0_L1EM18VH'  : {  1000 : TriggerRule(PS=40, comment='Support, 5 Hz target at 3e33, not enabled', rerun=1) },
    
})
    
# ========================================
# rerun
# this is potentially very expensive !! is this really needed ?

HLT_list=[
    'HLT_e7_lhmedium',                 'HLT_e7_lhmedium_nod0',    
    'HLT_e9_lhloose',                  'HLT_e9_lhloose_nod0',
    'HLT_e9_lhmedium',                 'HLT_e9_lhmedium_nod0',
    'HLT_e12_lhmedium',                'HLT_e12_lhmedium_nod0',
    'HLT_e17_lhmedium',                'HLT_e17_lhmedium_nod0',
    #'HLT_e17_lhmedium_L1EM15HI',       #'HLT_e17_lhmedium_nod0_L1EM15HI',
    "HLT_e17_lhmedium_nod0_L1EM15", 
    'HLT_e17_lhmedium_iloose',         'HLT_e17_lhmedium_nod0_iloose',
    #'HLT_e17_lhmedium_iloose_L1EM15HI', 'HLT_e17_lhmedium_nod0_iloose_L1EM15HI',
    #'HLT_e17_lhmedium_ivarloose_L1EM15HI', #'HLT_e17_lhmedium_nod0_ivarloose_L1EM15HI',
    'HLT_e20_lhmedium',                'HLT_e20_lhmedium_nod0',
    'HLT_e24_lhmedium_L1EM15VH',       'HLT_e24_lhmedium_nod0_L1EM15VH',


    
    'HLT_e26_lhmedium_L1EM22VHI',      'HLT_e26_lhmedium_nod0_L1EM22VHI',
    'HLT_e60_lhloose',                 'HLT_e60_lhloose_nod0',
    'HLT_e70_lhloose',                 'HLT_e70_lhloose_nod0',
    'HLT_e5_lhtight',                  'HLT_e5_lhtight_nod0',
    'HLT_e9_lhtight',                  'HLT_e9_lhtight_nod0',
    'HLT_e14_lhtight',                 'HLT_e14_lhtight_nod0',
    #
    # missing from list but most likely needed
    'HLT_e5_lhloose',                 'HLT_e5_lhloose_nod0',
    'HLT_e12_lhloose',                'HLT_e12_lhloose_nod0',
    'HLT_e12_lhloose_L1EM10VH',       'HLT_e12_lhloose_nod0_L1EM10VH',    
    'HLT_e12_lhloose_cutd0dphideta_L1EM10VH','HLT_e12_lhloose_nodeta_L1EM10VH','HLT_e12_lhloose_nodphires_L1EM10VH', 
    'HLT_e15_lhloose_L1EM13VH',       'HLT_e15_lhloose_nod0_L1EM13VH',
    'HLT_e15_lhloose_cutd0dphideta_L1EM13VH',
    'HLT_e17_lhloose_L1EM15',         'HLT_e17_lhloose_nod0_L1EM15',
    'HLT_e17_lhloose_cutd0dphideta_L1EM15',  'HLT_e17_lhloose_nodeta_L1EM15',  'HLT_e17_lhloose_nodphires_L1EM15',
    'HLT_e17_lhloose',                'HLT_e17_lhloose_nod0',
    'HLT_e17_lhloose_cutd0dphideta',         
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

    # 
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1),
}])))

# ========================================
# photon prescaled
# 12 Hz just for this. Is this needed?

HLT_list=[
    'HLT_g15_loose_L1EM7',
    'HLT_g25_loose_L1EM15',
    'HLT_g35_loose_L1EM15',
    'HLT_g40_loose_L1EM15',
    'HLT_g45_loose_L1EM15',
    'HLT_g50_loose_L1EM15',
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, comment='Support ATR-13381',rerun=1), 
}])))

# another 8 Hz. is this needed?
HLT_list=[
    'HLT_g60_loose',
    'HLT_g70_loose',
    'HLT_g80_loose',
    'HLT_g100_loose',
    
    "HLT_g60_loose_L1EM24VHI",
    "HLT_g70_loose_L1EM24VHI",
    "HLT_g80_loose_L1EM24VHI",
    "HLT_g100_loose_L1EM24VHI",
    "HLT_g120_loose_L1EM24VHI", 
    'HLT_g160_loose', 'HLT_g180_loose',
    
    "HLT_g60_loose_L1EM26VHI",
    "HLT_g70_loose_L1EM26VHI",
    "HLT_g80_loose_L1EM26VHI",
    "HLT_g100_loose_L1EM26VHI",
    "HLT_g120_loose_L1EM26VHI",
    "HLT_g140_loose_L1EM26VHI",
    "HLT_g160_loose_L1EM26VHI",
    "HLT_g180_loose_L1EM26VHI",
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.5, comment='Support ATR-13381',rerun=1),
}])))

#another 4 Hz. is this neeeded?
HLT_list=[
    'HLT_g10_loose',    
    'HLT_g20_loose_L1EM12',
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=1.0, comment='Support ATR-13381',rerun=1, PTRate=1.0),
}])))
# total of 24 Hz for prescaled support after HLT for egamma.

# ========================================
# photon rerun
# all this can be very CPU expensive . Is this really needed?

HLT_list=[
    'HLT_g10_etcut',
    'HLT_g20_etcut_L1EM12',
    'HLT_g10_medium',
    'HLT_g15_loose_L1EM3',
    'HLT_g15_loose',
    'HLT_g20_loose',
    'HLT_g25_loose',
    'HLT_g35_loose',  
    'HLT_g50_loose',
    'HLT_g25_medium',
    'HLT_g35_medium',
    'HLT_g20_tight',
    'HLT_g22_tight',
    'HLT_g40_tight',
    'HLT_g45_tight',
    
    
    'HLT_g20_loose_L1EM15',
    
    'HLT_e15_lhtight_iloose_L1EM13VH',
    'HLT_g25_medium_L1EM22VHI',
    'HLT_g35_loose_L1EM22VHI',
    'HLT_g45_tight_L1EM22VHI',

    "HLT_g35_loose_L1EM24VHI",
    "HLT_g45_tight_L1EM24VHI",
    
    "HLT_g45_tight_L1EM26VHI",
    
    # for 2e
    #"HLT_e20_lhvloose_nod0_L1EM18VH",
    #"HLT_e22_lhvloose_nod0", 
    
    # for 2g
    "HLT_g50_loose_L12EM18VH",
    "HLT_g60_loose_L12EM18VH",
    "HLT_g50_loose_L12EM20VH",
    "HLT_g60_loose_L12EM20VH",
    # for 3g
    "HLT_g20_loose_L1EM18VH",
    "HLT_g22_loose_L1EM20VH",
    
    # for g+mu
    "HLT_g35_loose_L1EM24VHI",
    "HLT_g35_loose_L1EM26VHI",

    # for e+xe
    "HLT_e60_lhloose_nod0_L1EM26VHI",  
    "HLT_e70_lhloose_nod0_L1EM26VHI",  
    "HLT_e80_lhloose_nod0",  
    "HLT_e80_lhloose_nod0_L1EM24VHI",  
    "HLT_e80_lhloose_nod0_L1EM26VHI",  

    # for e+mu
    "HLT_e28_lhmedium_nod0_L1EM24VHI",
    "HLT_e30_lhmedium_nod0_L1EM26VHI",

    # new for 2017
    "HLT_g100_tight",
    "HLT_g85_tight",
    "HLT_g100_tight_L1EM24VHI",
    "HLT_g85_tight_L1EM24VHI",
    "HLT_g100_tight_L1EM26VHI",
    "HLT_g85_tight_L1EM26VHI",    
]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1), 
}])))

HLT_list=['HLT_g60_loose_L1EM15VH']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1,ESRate=0.2), 
}])))


########################################
# T&P for electron

# --------------------------------------
# Z

#v6
egamma_Rules_supporting.update({

     'HLT_e24_lhtight_L1EM20VH_e15_etcut_L1EM7_Zee'         : { 1000 : TriggerRule(PS=1, comment='Primary Express, SM Zee', ESValue=0),
                                                                3001 : TriggerRule(PS=-1, comment='Disabled', ESValue=0) },
     'HLT_e24_tight_L1EM20VH_e15_etcut_L1EM7_Zee'           : { 1000 : TriggerRule(PS=1, comment='Primary'),
                                                                3001 : TriggerRule(PS=-1, comment='Disabled') },
     'HLT_e24_lhtight_nod0_L1EM20VH_e15_etcut_L1EM7_Zee'    : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-13378'), 
                                                                3001 : TriggerRule(PS=-1, comment='Disabled') },
     'HLT_e26_lhtight_nod0_e15_etcut_L1EM7_Zee'       : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-13378', ESRate=0.8) },
     'HLT_e28_lhtight_nod0_e15_etcut_L1EM7_Zee'       : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-13378', ESRate=0) },
     "HLT_e30_lhtight_nod0_e15_etcut_L1EM7_Zee"       : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-13378', ESRate=0) },
    })


# --------------------------------------
# W

L1_list=['L1_EM12_XS20', 'L1_EM15_XS30']
egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(rate=500, maxRate=500, comment='L1'),
                    }])))


L1_list=['L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', ]
if NoL1Topo :
    egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else :
    egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=20, comment='W TP L1Topo'), 
                        9001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

L1_list=['L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',]
if NoL1Topo :
    egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))
else :
    egamma_Rules_supporting.update(dict(map(None,L1_list,len(L1_list)*[{
                        1000 : TriggerRule(PS=10, comment='W TP L1Topo'), 
                        9001 : TriggerRule(PS=-1, comment='Disabled'), 
                        }])))

egamma_Rules_supporting.update({
    # 8 Hz for W tag&probe support, all with rerun, is this not CPU expensive?
    #
    # ATR-11746: needs rerun also to W T&P triggers
    #
    # EM10_XS20
    # eff 0.16
    'HLT_e13_etcut_trkcut_j20_perf_xe15_2dphi05_mt25'       : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    'HLT_e13_etcut_trkcut_j20_perf_xe15_6dphi05_mt25'       : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    # eff 0.11
    'HLT_e13_etcut_trkcut_xs15_j20_perf_xe15_2dphi05_mt25'  : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    'HLT_e13_etcut_trkcut_xs15_j20_perf_xe15_6dphi05_mt25'  : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    #
    'HLT_e13_etcut_trkcut_xs15_mt25'                        : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    #
    # EM15_XS30    
    # eff 0.05
    'HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35'       : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    # eff 0.03
    'HLT_e18_etcut_trkcut_xs20_j20_perf_xe20_6dphi15_mt35'  : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    #
    'HLT_e18_etcut_trkcut_xs20_mt35'                        : {  1000 : TriggerRule(rate=0.5, comment='Support, W tag probe', rerun=1) },
    
})

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
HLT_list=['HLT_e13_etcut_trkcut_L1EM12', 'HLT_e18_etcut_trkcut_L1EM15']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(rate=0.2, comment='Support', rerun=1),
}])))

HLT_list=['HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35',]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=1, comment='W TP L1Topo'),
}])))


HLT_list=['HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35',]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=5, comment='W TP L1Topo'),
}])))

# is this rerun not very CPU expensive? is this needed?
HLT_list=['HLT_xs15_L1XS20', 'HLT_xs20_L1XS30']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=0, comment='Support', rerun=1),
}])))

# --------------------------------------
# J/psi 

# PS adjustment by ATR-11886
# rate L1 3000 Hz for Jpsi support

egamma_Rules_supporting.update({
    'L1_2EM3'        : {  1000 : TriggerRule(rate=200, maxRate=200, comment='Support L1') },
    'L1_EM7_2EM3'    : {  1000 : TriggerRule(rate=1200, maxRate=1200, comment='Support L1'),
                          9001 : TriggerRule(rate=300, maxRate=300, comment='Support L1 SOSminus'), 
                          },
    'L1_EM12_2EM3'   : {  1000 : TriggerRule(rate=2600, maxRate=2600, comment='Support L1'),
                          9001 : TriggerRule(rate=650, maxRate=650, comment='Support L1 SOSminus'), 
                          },
    })


HLT_list=[
    'HLT_e14_lhtight_nod0_e4_etcut_Jpsiee',
    #'HLT_e14_etcut_e5_lhtight_nod0_Jpsiee',
    
    'HLT_e9_lhtight_nod0_e4_etcut_Jpsiee',
    #'HLT_e9_etcut_e5_lhtight_nod0_Jpsiee',

    # new for 2017
    "HLT_e5_lhtight_e14_etcut_Jpsiee",
    "HLT_e5_lhtight_nod0_e14_etcut_Jpsiee",
    "HLT_e5_tight_e14_etcut_Jpsiee",
    "HLT_e5_lhtight_e9_etcut_Jpsiee",
    "HLT_e5_lhtight_nod0_e9_etcut_Jpsiee",
    "HLT_e5_tight_e9_etcut_Jpsiee",
    ]
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support, Jpsi ee'), 
                    }])))

HLT_list=['HLT_e5_lhtight_nod0_e4_etcut_Jpsiee']
egamma_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Support, Jpsi ee',ESRate=0.4), 
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
        'HLT_e26_lhvloose_nod0_L1EM20VH_3j20noL1'              : {  1000 : TriggerRule(PS=5, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                   17001 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        'HLT_e26_lhvloose_nod0_L1EM20VH_4j20noL1'              : {  1000 : TriggerRule(PS=3, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        'HLT_e26_lhvloose_nod0_L1EM20VH_5j15noL1'              : {  1000 : TriggerRule(PS=3, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        'HLT_e26_lhvloose_nod0_L1EM20VH_6j15noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, 1 Hz target at 1.2e34'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disabled, SM W+jets'),
                                                                    },
        # 2e34
        'HLT_e28_lhvloose_nod0_L1EM20VH_3j20noL1'              : {  1000 : TriggerRule(PS=9, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        'HLT_e28_lhvloose_nod0_L1EM20VH_4j20noL1'              : {  1000 : TriggerRule(PS=4, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        'HLT_e28_lhvloose_nod0_L1EM20VH_5j15noL1'              : {  1000 : TriggerRule(PS=5, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        'HLT_e28_lhvloose_nod0_L1EM20VH_6j15noL1'              : {  1000 : TriggerRule(PS=3, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-13381)') },
        
        "HLT_e30_lhmedium_nod0_L1EM20VH_3j20noL1"              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-15233)') },
        "HLT_e30_lhmedium_nod0_L1EM20VH_4j20noL1"              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-15233)') },
        "HLT_e30_lhmedium_nod0_L1EM20VH_5j15noL1"              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-15233)') },
        "HLT_e30_lhmedium_nod0_L1EM20VH_6j15noL1"              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, 1 Hz target at 2e34 (ATR-15233)') },
    
    # 12 Hz support
    # 1e34
    'HLT_e24_lhvloose_nod0_L1EM20VH_3j20noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'),
                                                               17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_nod0_L1EM20VH_4j20noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'),
                                                               17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_nod0_L1EM20VH_5j15noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'),
                                                               17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_nod0_L1EM20VH_6j15noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'),
                                                               17001 : TriggerRule(PS=-1, comment='Disabled') },
    # 12 Hz support
    # 0.3e34
    'HLT_e24_vloose_L1EM18VH_3j20noL1'                     : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_vloose_L1EM18VH_4j20noL1'                     : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_vloose_L1EM18VH_5j15noL1'                     : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_vloose_L1EM18VH_6j15noL1'                     : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_L1EM18VH_3j20noL1'                   : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_L1EM18VH_4j20noL1'                   : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_L1EM18VH_5j15noL1'                   : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_L1EM18VH_6j15noL1'                   : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_nod0_L1EM18VH_3j20noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_nod0_L1EM18VH_4j20noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_nod0_L1EM18VH_5j15noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhvloose_nod0_L1EM18VH_6j15noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled') },

    'HLT_e24_lhmedium_nod0_L1EM20VH_3j20noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'),
                                                                17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_e24_lhmedium_nod0_L1EM20VH_4j20noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'), 
                                                                17001 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e24_lhmedium_nod0_L1EM20VH_5j15noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'), 
                                                                17001 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e24_lhmedium_nod0_L1EM20VH_6j15noL1'              : {  1000 : TriggerRule(PS=-1, comment='Disabled'), 
                                                                17001 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e26_lhmedium_nod0_L1EM20VH_3j20noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                17001 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e26_lhmedium_nod0_L1EM20VH_4j20noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                17001 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e26_lhmedium_nod0_L1EM20VH_5j15noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                17001 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e26_lhmedium_nod0_L1EM20VH_6j15noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                17001 : TriggerRule(PS=-1, comment='Disabled')  },
    'HLT_e28_lhmedium_nod0_L1EM20VH_3j20noL1'              : {  1000 : TriggerRule(PS=2, comment='Support, SM W+jets, targer 0.8Hz') , 
                                                                20001 : TriggerRule(PS=-1, comment='Disabled') }, 
    'HLT_e28_lhmedium_nod0_L1EM20VH_4j20noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                20001 : TriggerRule(PS=-1, comment='Disabled')  }, 
    'HLT_e28_lhmedium_nod0_L1EM20VH_5j15noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                20001 : TriggerRule(PS=-1, comment='Disabled')  }, 
    'HLT_e28_lhmedium_nod0_L1EM20VH_6j15noL1'              : {  1000 : TriggerRule(PS=1, comment='Support, SM W+jets, targer 0.8Hz'), 
                                                                20001 : TriggerRule(PS=-1, comment='Disabled')  },
        "HLT_e30_lhvloose_nod0_L1EM20VH_3j20noL1"              : {  1000 : TriggerRule(PS=-1, comment='Disabled (ATR-15233), backup') ,
                                                                    },
        "HLT_e30_lhvloose_nod0_L1EM20VH_4j20noL1"              : {  1000 : TriggerRule(PS=-1, comment='Disabled (ATR-15233), backup') },
        "HLT_e30_lhvloose_nod0_L1EM20VH_5j15noL1"              : {  1000 : TriggerRule(PS=-1, comment='Disabled (ATR-15233), backup') },
        "HLT_e30_lhvloose_nod0_L1EM20VH_6j15noL1"              : {  1000 : TriggerRule(PS=-1, comment='Disabled (ATR-15233), backup') },

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

    'HLT_2g20_loose_L12EM15'                       : {  1000 : TriggerRule(rate=0.2, comment='Support') },
    'HLT_2g10_loose'                               : {  1000 : TriggerRule(rate=0.2, comment='Support') },
    'HLT_g20_loose_2g15_loose_L12EM13VH'           : {  1000 : TriggerRule(rate=0.1, comment='Support (ATR-14757)'),
                                                       10001 : TriggerRule(PS=-1, comment='Disable'),
                                                        },

    #
    # what supporting ??? ask
    'HLT_g35_medium_g25_medium_L1EM7_EMPTY'        : {  1000 : TriggerRule(PS=1, comment='Support') },
    'HLT_g35_medium_g25_medium_L1EM7_UNPAIRED_ISO' : {  1000 : TriggerRule(PS=1, comment='Support') },

    'HLT_2g22_tight_L1EM7_UNPAIRED_ISO' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },
    'HLT_2g22_tight_L1EM7_EMPTY' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },
    ## 20.11.0.Y 'HLT_2g50_loose_L1EM7_UNPAIRED_ISO' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },
    ## 20.11.0.Y 'HLT_2g50_loose_L1EM7_EMPTY' : {  1000 : TriggerRule(PS=1, comment='Support, ATR-13379') },
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

egamma_Rules={}

RulesList=[egamma_Rules_2e34, egamma_Rules_1e34, egamma_Rules_0p5e34, egamma_Rules_0p3e34, egamma_Rules_0p15e34, egamma_Rules_supporting]

for Rules in RulesList:
    for newrule in Rules.keys():
        if egamma_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    egamma_Rules.update(Rules)


#################################################################################################
#
#
# Combined
#
# note: we need to add jet+clean*+xe items
#
#################################################################################################


########################################
# 2e34 

combined_Rules_2e34={}

HLT_list=[
    # ATR-9486
    # e + g
    'HLT_e20_lhmedium_nod0_g35_loose',
    'HLT_e24_lhmedium_nod0_L1EM15VH_g25_medium',
    # e + mu
    'HLT_e17_lhloose_nod0_mu14',
    'HLT_e7_lhmedium_nod0_mu24',
    'HLT_e26_lhmedium_nod0_L1EM22VHI_mu8noL1', 
    "HLT_e28_lhmedium_nod0_L1EM24VHI_mu8noL1",
    "HLT_e30_lhmedium_nod0_L1EM26VHI_mu8noL1",
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
    'HLT_e60_lhloose_nod0_L1EM26VHI_xe60noL1',
    'HLT_e70_lhloose_nod0_L1EM26VHI_xe70noL1',
    'HLT_e80_lhloose_nod0_L1EM24VHI_xe80noL1',
    'HLT_e80_lhloose_nod0_L1EM26VHI_xe80noL1',
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'), 
                     }])))

HLT_list=[
    # e+2g
    'HLT_e20_lhmedium_nod0_2g10_loose_L1EM15VH_3EM8VH', 
    'HLT_e20_lhmedium_nod0_2g10_medium_L1EM15VH_3EM8VH', 

    # 3e
    'HLT_e17_lhmedium_nod0_2e10_lhmedium_nod0_L1EM15VH_3EM8VH',
    'HLT_e17_lhloose_nod0_2e10_lhloose_nod0_L1EM15VH_3EM8VH',
    "HLT_e17_lhvloose_nod0_2e10_lhvloose_nod0_L1EM15VH_3EM8VH",    
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary'),
                   12001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

HLT_list=[
    # g + xe (loose)
    'HLT_g70_loose_xe70noL1',
    ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary Higgs'),
                   14001 : TriggerRule(PS=-1, comment='Disabled'), 
                    }])))

L1_list=['L1_EM15VH_3EM7']
combined_Rules_2e34.update(dict(map(None,L1_list,len(L1_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary EndOfFill Safe before checking the online rate of alternative 3EM items'),
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
    'HLT_g85_tight_3j50noL1_L1EM22VHI', 'HLT_g100_tight_3j50noL1_L1EM22VHI',  
    "HLT_g85_tight_3j50noL1_L1EM24VHI", "HLT_g100_tight_3j50noL1_L1EM24VHI", 
    "HLT_g85_tight_3j50noL1_L1EM26VHI", "HLT_g100_tight_3j50noL1_L1EM26VHI",
    
    'HLT_g80_loose_xe80noL1', 
    'HLT_g80_loose_L1EM24VHI_xe80noL1',
    ]    
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'),
                   17001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))

HLT_list=[
    "HLT_g100_tight_3j50noL1_L1EM26VHI",
    'HLT_g80_loose_L1EM26VHI_xe80noL1',
    ]    
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14196, ATR-15233'),
                    }])))


HLT_list=['HLT_j80_bmv2c2050_split_xe60_L12J50_XE40',
]
if startup2200b:
    combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, ATR-14196'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:
    combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, ATR-14196'),
                        17001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))

HLT_list=[
    # mu + g
    "HLT_g35_loose_L1EM26VHI_mu15noL1_mu2noL1",
    "HLT_g35_loose_L1EM26VHI_mu18noL1",    
        ]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary, ATR-15233'),
                     }])))        

HLT_list=[
    # mu + g . ATR-11623
    'HLT_g35_loose_L1EM24VHI_mu15noL1_mu2noL1',
    'HLT_g35_loose_L1EM24VHI_mu18noL1',
]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'),
                     }])))

HLT_list=[
    # mu + g . ATR-11623
    'HLT_g35_loose_L1EM22VHI_mu15noL1_mu2noL1',
    'HLT_g35_loose_L1EM22VHI_mu18noL1',
]
combined_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                     1000 : TriggerRule(PS=1, comment='Primary'),
                    17001 : TriggerRule(PS=-1, comment='Disabled'),
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

combined_Rules_1e34={}

#who asks for this? is it Higgs + photon, Higgs invisible? what about e+xe ?
HLT_list=[
    # e + xe
    'HLT_e60_lhloose_nod0_xe60noL1',
    # g + xe (loose)
    'HLT_g60_loose_xe60noL1',
    ]
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Primary'), 
                      7001 : TriggerRule(PS=-1, comment='Disabled'),
                      }])))

# who asks for this? 
HLT_list=[
          'HLT_e24_lhmedium_nod0_L1EM20VHI_mu8noL1',]
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                       1000 : TriggerRule(PS=1, comment='Primary'), 
                       10001 : TriggerRule(PS=-1, comment='Disabled'),
                       }])))

# unisolated lepton + jets for RPV l+multijets
HLT_list=['HLT_e24_lhmedium_5j30_0eta240_L1EM13VH_3J20',
          'HLT_e24_lhmedium_nod0_5j30_0eta240_L1EM13VH_3J20']
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Support'), 
                      7001 : TriggerRule(PS=5, comment='Support'),
                      }])))
HLT_list=['HLT_mu24_5j30_0eta240_L1MU10_3J20']
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Support'), 
                      7001 : TriggerRule(PS=15, comment='Support'),
                      }])))
HLT_list=['HLT_e26_lhmedium_5j30_0eta240_L1EM13VH_3J20',
          'HLT_e26_lhmedium_nod0_5j30_0eta240_L1EM13VH_3J20']
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Support'), 
                      10001 : TriggerRule(PS=5, comment='Support'),
                      }])))
HLT_list=['HLT_mu26_5j30_0eta240_L1MU10_3J20']
combined_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(PS=1, comment='Support'), 
                      10001 : TriggerRule(PS=15, comment='Support'),
                      }])))

########################################
# other rules

bmvPS = 1
if startup2200b:
    bmvPS=100
combined_Rules_other={

    # ----------------------------------------
    # jet + xe

    # ATR-11303, SUSY
    # --> keep both j80/100_xe80 unPS until 1.5e34

    # 1.5e34
    'HLT_j100_xe80'                      : {  1000 : TriggerRule(PS=-1, comment='Disabled'),
                                              }, 

    'HLT_j80_xe80'                       : {  1000 : TriggerRule(rate=0.2, comment='Express', ESValue=1)},

    'HLT_j40_cleanT_xe85_tc_lcw_2dphi05_L1XE50' : {  1000 : TriggerRule(rate=0.5, comment='Support, ATR-13131')},
    
    # ----------------------------------------
    # g + xe (tight)- ATR-12901 Higgs -> photon + dark photon 

    # up to 2e34 w/o PS, after with PS
    'HLT_g45_tight_L1EM22VHI_xe45noL1'   : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                              7001 : TriggerRule(PS=5, comment='Primary, ATR-12901 EndOfFill'),
                                              14001 : TriggerRule(PS=10, comment='Primary, ATR-12901 EndOfFill'),
                                              20001 : TriggerRule(PS=100, comment='Primary, ATR-12901 EndOfFill'),
                                              },

    "HLT_g45_tight_L1EM24VHI_xe45noL1"    : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                              7001 : TriggerRule(PS=5, comment='Primary, ATR-12901 EndOfFill'),
                                               14001 : TriggerRule(PS=10, comment='Primary, ATR-12901 EndOfFill'),
                                               20001 : TriggerRule(PS=100, comment='Primary, ATR-12901 EndOfFill Needs proposal'),
                                               },

    "HLT_g45_tight_L1EM26VHI_xe45noL1"    : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901 EndOfFill'),
                                               7001 : TriggerRule(PS=5, comment='Primary, ATR-12901 EndOfFill'),
                                               14001 : TriggerRule(PS=10, comment='Primary, ATR-12901 EndOfFill'),
                                               20001 : TriggerRule(PS=100, comment='Primary, ATR-12901 EndOfFill Needs proposal'),
                                               },

    # up to 5e33
    'HLT_g45_tight_xe45noL1'             : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901'),
                                              3001 : TriggerRule(PS=-1, comment='Disabled') },
    # up to 0.3e34
    'HLT_g40_tight_xe40noL1'             : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12901'),
                                              3001 : TriggerRule(PS=-1, comment='Disabled') },

    #------------------------
    # mono-b analysis , ATR-12947

    'HLT_j80_bmv2c2060_split_xe60_L12J50_XE40'      :  {  1000 : TriggerRule(PS=bmvPS, comment='Primary, ATR-12947, ATR-14196'),
                                                         14001 : TriggerRule(PS=-1, comment='Disabled, ATR-12947'),
                                                          },
    'HLT_j80_bmv2c2077_split_xe70_L1J400ETA25_XE50' :  { 1000 : TriggerRule(PS=bmvPS, comment='Primary, ATR-12947'),
                                                         7001 : TriggerRule(PS=-1, comment='Disabled')},

    #--------------------
    # low-mass leptophobic Z' in association with an ISR photon

    'HLT_g75_tight_3j50noL1'             : {  1000 : TriggerRule(PS=-1, comment='Primary, ATR-12750')},
    'HLT_g75_tight_3j50noL1_L1EM22VHI'             : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12750'),
                                                       14001 : TriggerRule(PS=-1, comment='Disable, ATR-12750'),
                                                        },

    # ---------------------
    # hh -> WWbb -> lnu jj bb
    # Needs to add EM15VH_3J20 or EM18VH_3J20
    'L1_EM13VH_3J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900'),
                               12001 : TriggerRule(PS=5, comment='Support, needs EM15VH_3J20 or EM18VH_3J20, ATR-12900'),
                               },
    'L1_MU10_3J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900') },
    'HLT_e15_lhtight_nod0_iloose_3j20_L1EM13VH_3J20'     : { 1000 : TriggerRule(PS=-1, comment='Disabled - ATR-14490') },
    'HLT_e15_lhtight_nod0_ivarloose_3j20_L1EM13VH_3J20' : { 1000 : TriggerRule(PS=1, comment='Primary, ATR-12900, ATR-13378'),
                                                           15001 : TriggerRule(PS=1, comment='Support, needs tigher chain with EM15VH_3J20 or EM18VH_3J20, ATR-12900'),
                                                            },
    'HLT_mu14_ivarloose_3j20_L1MU10_3J20' : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900') },
    # ---------------------
    # VH , H->bb V=W -> mu
    'L1_MU10_2J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900') },
    'HLT_mu20_ivarmedium_L1MU10_2J20' : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13367') },
    'L1_MU10_2J15_J20'      : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-12900'),
                                17001 : TriggerRule(PS=-1, comment='Disable'),
                                 },
    'HLT_mu20_ivarmedium_L1MU10_2J15_J20' : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13367'),
                                              17001 : TriggerRule(PS=-1, comment='Disable'),
                                               },

    # ---------------------
    # SUSY Higgsino triggers: ATR-14717
    'HLT_2mu4_j85_xe50_mht' : {  1000 : TriggerRule(PS=1, comment='Primary, too high rates, SUSY Higgsino triggers ATR-14717') },    
    'HLT_mu4_j125_xe90_mht' : {  1000 : TriggerRule(PS=1, comment='Primary, too high rates, SUSY Higgsino triggers ATR-14717') },    

}

    
########################################


combined_Rules={}

RulesList=[combined_Rules_2e34, combined_Rules_1e34, combined_Rules_other]
for Rules in RulesList:
    for newrule in Rules.keys():
        if combined_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
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

vbf_Rules_2e34={}

HLT_list=['HLT_j80_bmv2c2085_split_2j60_320eta490', 'HLT_j80_bmv2c2070_split_j60_bmv2c2085_split_j45_320eta490',]
if startup2200b:
    vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, VBF H->bb , ATR-13526'), 
                        }])))
else:
    vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, VBF H->bb , ATR-13526'), 
                        }])))


#need to add a g26 or so, when EM22VHI -> EM24VHI, at L>2e34 . ATR-12120
# VBF + 4 j
HLT_list=['HLT_g25_medium_L1EM22VHI_2j35_0eta490_bmv2c2077_split_2j35_0eta490', 'HLT_g25_medium_L1EM22VHI_j35_0eta490_bmv2c2077_split_3j35_0eta490_invm700', ]
if startup2200b:
    vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, VBF H->bb + photon , ATR-12120'), 
                        17001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))
else:
    vbf_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, VBF H->bb + photon , ATR-12120'), 
                        17001 : TriggerRule(PS=-1, comment='Disabled'),
                        }])))

# 1.5e34 

vbf_Rules_1p5e34={}

HLT_list=['HLT_g25_medium_L1EM22VHI_4j35_0eta490_invm1000',]
vbf_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H +photon, ATR-12120, ATR-13373, 2e34?'), 
                    17001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))
HLT_list=['HLT_g25_medium_L1EM22VHI_4j35_0eta490_invm700',]
vbf_Rules_1p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, VBF H +photon, ATR-12120, ATR-13373, backup, EndOfFill'),
                    7001 : TriggerRule(PS=10, comment='Support, VBF H +photon, ATR-12120, ATR-13373, backup'), 
                    }])))
    
########################################
# 1e34 

vbf_Rules_1e34={}
HLT_list=['HLT_mu6_ivarloose_2j40_0eta490_invm1000_L1MU6_J30.0ETA49_2J20.0ETA49',]
#[,]

vbf_Rules_1e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                             1000 : TriggerRule(PS=10, comment='Support, Backup for L1Topo, mu+VBF, ATR-10979 ATR-13373, PSed until alternative trigger is found'), 
                             17001 : TriggerRule(PS=-1, comment='Disabled'),
                             }]))) 

########################################

# 0.5e34 

vbf_Rules_0p5e34={}

# vbf + mu, very large rate , can we reduce?
HLT_list=['HLT_mu6_2j40_0eta490_invm800_L1MU6_J30.0ETA49_2J20.0ETA49',]
vbf_Rules_0p5e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                         1000 : TriggerRule(PS=10, comment='Support, mu+VBF, ATR-13373, PSed until alternative trigger is found'), 
                         5001 : TriggerRule(PS=-1, comment='Disabled'),
                         }])))

########################################
# 0.3e34 

vbf_Rules_0p3e34={}

# vbf + mu, very large rate , can we reduce?
HLT_list=['HLT_mu6_2j40_0eta490_invm600_L1MU6_J30.0ETA49_2J20.0ETA49',]
vbf_Rules_0p3e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=10, comment='Support, mu+VBF, ATR-13373, PSed until alternative trigger is found'), 
                          3001 : TriggerRule(PS=-1, comment='Disabled'),
                          }])))

########################################
# 0.15e34 

vbf_Rules_0p15e34={}

# vbf + mu, very large rate , can we reduce?
HLT_list=['HLT_mu6_2j40_0eta490_invm400_L1MU6_J30.0ETA49_2J20.0ETA49',]
vbf_Rules_0p15e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                          1000 : TriggerRule(PS=10, comment='Support, mu+VBF, ATR-13373, PSed until alternative trigger is found'), 
                          1501 : TriggerRule(PS=-1, comment='Disabled'),
                          }])))

########################################
# supporting

vbf_Rules_supporting={}

HLT_list=['HLT_g20_loose_L1EM18VH_2j40_0eta490_3j25_0eta490']
vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'),
                           }])))

HLT_list=['HLT_g25_loose_L1EM20VH_4j35_0eta490']
vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                           1000 : TriggerRule(rate=1., comment='Support, ATR-12120'), 
                           }])))

HLT_list=['HLT_j80_0eta240_2j60_320eta490','HLT_j80_0eta240_j60_j45_320eta490',]
vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                      1000 : TriggerRule(rate=0.5, comment='Support, VBF H->bb , ATR-13526'), 
                      }])))

HLT_list=['HLT_j80_0eta240_j60_j45_320eta490_AND_2j45_bmv2c2070_split']
if startup2200b:
    vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Support, VBF H->bb , ATR-13526'),
                        }])))
else:
    vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Support, VBF H->bb , ATR-13526'),
                        }])))
    
HLT_list=['HLT_j80_0eta240_j60_j45_320eta490_invm700_AND_j45_bmv2c2070_split_j45_bmv2c2077_split']
vbf_Rules_supporting.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled'),
                           }])))



# ----------------------------------------
# MET + VBF
# 1.8 kHz @ 2e34 for XE55 --> prescaled to 10 Hz
# why ? and why 10 Hz?
vbf_Rules_supporting.update({
     'HLT_2j40_0eta490_invm250_L1XE55'   : {  1000 : TriggerRule(PS=-1, comment='Support') },
})

########################################

vbf_Rules={}

RulesList=[vbf_Rules_2e34, vbf_Rules_1p5e34, vbf_Rules_1e34, vbf_Rules_0p5e34, vbf_Rules_0p3e34, vbf_Rules_0p15e34, vbf_Rules_supporting] 

for Rules in RulesList:
    for newrule in Rules.keys():
        if vbf_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
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
    'HLT_mu6_iloose_mu6_11invm24_noos_novtx'                    : {  1000 : TriggerRule(PS=50, comment='Support, DY, ATR-13363,ATR-14680')},
    'HLT_mu6_iloose_mu6_24invm60_noos_novtx'                    : {  1000 : TriggerRule(PS=20, comment='Support, DY, ATR-13363,ATR-14680')},
    'HLT_mu4_iloose_mu4_7invm9_noos_novtx'                      : {  1000 : TriggerRule(PS=10, comment='Support, DY, ATR-13363') },
    'HLT_mu4_iloose_mu4_11invm60_noos_novtx'                    : {  1000 : TriggerRule(PS=10, comment='Support, DY, ATR-13363') },

    # ========================================
    # razor

    # supporting
    'HLT_j30_xe10_razor100'                                     : {  1000 : TriggerRule(rate=1.0, comment='Support, razor, ATR-11539/11756') },

    'HLT_j30_xe60_razor100'                                     : {  1000 : TriggerRule(rate=1.0, comment='Support, razor, ATR-11539/11756') },

    # ========================================
    # LLP
    
    # supporting (background, rerun)
    'HLT_mu20_msonly'                                           : {  1000 : TriggerRule(rate=0.2, comment='Support, LLP', rerun=1) },
 
    # ========================================
    # LFV tau->mu gamma

    # supporting 

    'HLT_g10_etcut_L1EM8I_mu10_iloose_taumass'                   : {  1000 : TriggerRule(rate=1.0, comment='Support') },
    #'HLT_g20_etcut_L1EM15I_mu4_iloose_taumass'                   : {  1000 : TriggerRule(rate=1.0, comment='Support') },
    "HLT_g20_etcut_L1EM15_mu4_taumass"                           : {  1000 : TriggerRule(rate=1.0, comment='Support') },
    # ========================================
    # h->phi gamma -> KK gamma

    # supporting
    'HLT_tau25_dikaon_tracktwo'                                 : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_tau25_dikaontight_tracktwo'                            : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_tau35_dikaon_tracktwo_L1TAU12'                         : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_tau35_dikaontight_tracktwo_L1TAU12'                    : {  1000 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_tau25_dikaonmass_tracktwo'                                 : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13061',rerun=1) },
    'HLT_tau25_dikaonmasstight_tracktwo'                            : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13061',rerun=1) },
    'HLT_tau35_dikaonmass_tracktwo_L1TAU12'                         : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13061',rerun=1) },
    'HLT_tau35_dikaonmasstight_tracktwo_L1TAU12'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13061',rerun=1) },
    'HLT_tau25_singlepiontight_tracktwo'                         : {  1000 : TriggerRule(PS=0, comment='Support, ATR-11531',rerun=1) },
    'HLT_tau25_singlepion_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-11531',rerun=1) },
    'HLT_g35_medium_L1EM24VHI'                          : {  1000 : TriggerRule(PS=0, comment='Support, ATR-11531',rerun=1) },
    'HLT_g35_medium_L1EM22VHI'                          : {  1000 : TriggerRule(PS=0, comment='Support, ATR-11531',rerun=1) },

    'HLT_tau25_dipion1_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau25_kaonpi1_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau25_dipion2_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau25_kaonpi2_tracktwo'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },

    'HLT_tau35_kaonpi2_tracktwo_L1TAU12'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau35_dipion1loose_tracktwo_L1TAU12'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau35_dipion2_tracktwo_L1TAU12'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },
    'HLT_tau35_kaonpi1_tracktwo_L1TAU12'                    : {  1000 : TriggerRule(PS=0, comment='Support, ATR-13841',rerun=1) },

    }

dedicated_Rules_2e34_L1Topo={}

HLT_list=[
    'HLT_mu6_iloose_mu6_11invm24_noos_L1DY-BOX-2MU6', #28Hz
    'HLT_mu6_iloose_mu6_24invm60_noos_L1DY-BOX-2MU6', #13Hz
    ]
if NoHLTTopo :
    dedicated_Rules_2e34_L1Topo.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled, DY L1Topo ATR-11516'), 
                        }])))
else :
    dedicated_Rules_2e34_L1Topo.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=10, comment='Primary, DY L1Topo ATR-11516'), 
                        }])))


HLT_list=[
    'HLT_mu6_iloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6', #68Hz
    'HLT_mu6_iloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6', #32Hz
    ]
if NoHLTTopo :
    dedicated_Rules_2e34_L1Topo.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled, DY L1Topo ATR-11516'), 
                        }])))
else :
    dedicated_Rules_2e34_L1Topo.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=100, comment='Primary, DY L1Topo ATR-11516'), 
                        }])))    


HLT_list=['HLT_g10_loose_mu10_iloose_taumass_L1LFV-EM8I', #5.8Hz #L1 830Hz
          'HLT_g20_loose_mu4_iloose_taumass_L1LFV-EM15I', #5.5Hz #L1 430Hz
          'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU', #3Hz L1 2.7kHz
          'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU_bTau', #0.3Hz
          'HLT_mu11_nomucomb_mu6noL1_nscan03_L1LFV-MU_bTau_delayed', #20Hz
          'HLT_j30_jes_cleanLLP_PS_llp_L1LLP-NOMATCH', #5.5Hz #453Hz no LLP-RO...
          'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1LLP-NOMATCH', #7.2Hz
          ]
if NoL1Topo :
    dedicated_Rules_2e34_L1Topo.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled, LFV/LLP L1Topo'), 
                        }])))
else :
    dedicated_Rules_2e34_L1Topo.update(dict(map(None,HLT_list,len(HLT_list)*[{
                        1000 : TriggerRule(PS=1, comment='Primary, LFV/LLP L1Topo'), 
                        }])))
    
dedicated_Rules_2e34 = {

    # ========================================
    # Drell-Yan
    #

    'HLT_mu6_iloose_mu6_11invm24_noos'                          : {  1000 : TriggerRule(PS=5, comment='Primary, DY, ATR-13363,ATR-14680')},
    'HLT_mu6_iloose_mu6_24invm60_noos'                          : {  1000 : TriggerRule(PS=2, comment='Primary, DY, ATR-13363,ATR-14680')},
    'HLT_mu4_iloose_mu4_7invm9_noos'                            : {  1000 : TriggerRule(PS=1, comment='Primary, DY, ATR-13363, EndOfFill') },
    'HLT_mu4_iloose_mu4_11invm60_noos'                          : {  1000 : TriggerRule(PS=1, comment='Primary, DY, ATR-13363, EndOfFill') },

    'HLT_2mu6_10invm30_pt2_z10'                                 : {  1000 : TriggerRule(PS=1, comment='Primary, DY, ATR-11516, EndOfFill'),
                                                                     10001 : TriggerRule(PS=-1, comment='Disabled, EndOfFill') },

   

    # ========================================
    # razor

    'L1_2J15_XE55'                                              : {  1000 : TriggerRule(PS=1, comment='Primary L1,razor, ATR-12890') },

    'HLT_j30_xe60_razor170'                                     : {  1000 : TriggerRule(PS=1, comment='Primary, razor, ATR-12890'),
                                                                     7001 : TriggerRule(rate=0.5, comment='Support') }, # v4
    
    'HLT_j30_xe60_razor185'                                     : {  1000 : TriggerRule(PS=1, comment='Primary, razor, ATR-12890'),
                                                                     7001 : TriggerRule(rate=0.5, comment='Support') },
    
    'HLT_j30_xe60_razor195'                                     : {  1000 : TriggerRule(PS=1, comment='Primary, razor, ATR-12890'),
                                                                     14001 : TriggerRule(rate=0.5, comment='Support razor, ATR-12890'),
                                                                     },

    'HLT_j30_xe10_razor170'                                     : {  1000 : TriggerRule(PS=1, comment='Primary, razor, ATR-12890'),
                                                                     5001 : TriggerRule(rate=0.5, comment='Support 0.5Hz, ATR-13382') }, 
    'HLT_j30_xe10_razor185'                                     : {  1000 : TriggerRule(PS=1, comment='Primary, razor, ATR-12890'),
                                                                     7001 : TriggerRule(rate=0.5, comment='Support 0.5Hz, ATR-13382') }, 
    'HLT_j30_xe10_razor195'                                     : {  1000 : TriggerRule(PS=1, comment='Primary, razor, ATR-12890'),
                                                                     7001 : TriggerRule(rate=0.5, comment='Support 0.5Hz, ATR-13382') }, 

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
                                                                    17001 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_L1TAU60'                       : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_pufix_L1TAU60'                 : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-15255'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-15255'), 
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_pufix_noiso_L1TAU60'           : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-15255'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-15255'), 
                                                                     },
    'HLT_j30_jes_PS_llp_noiso_L1TAU60'                          : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),
                                                                     },
    'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU60'                 : {  1000 : TriggerRule(PS=1, comment='Primary, LLP, ATR-10933'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disable, LLP, ATR-10933'),                                                                   },
    'HLT_j200_jes_cleanLLP_PS_revllp_trkiso_L1TAU60'           : {  1000 : TriggerRule(PS=1, comment='Primary, REVLLP, ATR-14293'),
                                                                    10001 : TriggerRule(PS=100, comment='Support, Needs alternative, REVLLP, ATR-14293'), 
                                                                    }, 
    'HLT_j230_jes_cleanLLP_PS_revllp_L1TAU60'                  : {  1000 : TriggerRule(PS=1, comment='Primary, REVLLP, ATR-14293, EndOfFill'),
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

    'L1_MU11_2MU6'                                          : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu and B-phys') }, 

    'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1MU11_2MU6'        : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu') },            
    'HLT_mu11_nomucomb_mu6noL1_nscan03_L1MU11_2MU6_bTau'    : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu'),
                                                                 7001 : TriggerRule(PS=3, comment='Primary, LFV tau 3mu') },            
    'HLT_2mu10_nomucomb'                                    : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu'),
                                                                 5001: TriggerRule(PS=-1, comment='Disabled') },  #SX 090216          

    'L1_2MU6_3MU4'                                          : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu')},

    'HLT_2mu6_nomucomb_mu4_nomucomb_bTau_L12MU6_3MU4'            : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu') },
    'HLT_mu6_nomucomb_2mu4_nomucomb_bTau_L1MU6_3MU4'            : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau 3mu') },

    'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1MU11_2MU6_bTau'            : {  1000 : TriggerRule(PS=1, comment='Primary',ESRate=0.1) },
    'HLT_mu20_nomucomb_mu6noL1_nscan03_bTau'            : {  1000 : TriggerRule(PS=1, comment='Primary') },
    'HLT_mu20_nomucomb_mu6noL1_nscan03'            : {  1000 : TriggerRule(PS=1, comment='Primary') },


    
    

    # ========================================
    # LFV tau->emu 

    'L1_EM8I_MU10'                                               : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
                                                                      10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    'L1_EM15I_MU4'                                               : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
                                                                      10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    'HLT_g10_loose_L1EM8I_mu10_iloose_taumass'                   : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
                                                                      10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    #'HLT_g20_loose_L1EM15I_mu4_iloose_taumass'                   : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
    #                                                                  10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    "HLT_g20_loose_L1EM15_mu4_iloose_taumass"                    : {  1000 : TriggerRule(PS=1, comment='Primary, LFV tau mugamma, EndOfFill'),
                                                                      10001 : TriggerRule(PS=-1, comment='Disabled, replaced with L1Topo') },

    # ========================================
    # HIP
    "HLT_g0_hiptrt_L1EM26VHI"                                   : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259') }, 
    "HLT_g0_hiptrt_L1EM24VHI"                                   : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259') },
    'HLT_g0_hiptrt_L1EM22VHI'                                   : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259'),
                                                                     17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g0_hiptrt_L1EM20VHI'                                   : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259'),
                                                                     10001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g0_hiptrt_L1EM20VH'                                    : {  1000 : TriggerRule(PS=1, comment='Primary, monopole, ATR-9259'),
                                                                     3001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g0_hiptrt_L1EM18VH'                                    : {  1000 : TriggerRule(PS=-1, comment='Disabled') },

    
    # ========================================
    # stopped gluino

    'HLT_j55_0eta240_xe50_L1J30_EMPTY'                          : {  1000 : TriggerRule(PS=1, comment='Primary, stopped gluino, ATR-11193') },
    
    # ========================================
    # h->phi gamma -> KK gamma

    # we need to add alternative when EM22VHI gets PS out

    'HLT_g35_medium_tau25_dikaonmasstight_tracktwo_L1TAU12'         : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-13061') ,
                                                                         17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_dikaonmasstight_tracktwo_L1TAU12'         : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-13061') },

    'HLT_g35_medium_tau35_dikaonmasstight_tracktwo_L1TAU12'         : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-13061') ,
                                                                         17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau35_dikaonmass_tracktwo_L1TAU12'              : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-13061')},


    'HLT_g35_medium_tau25_dikaonmass_tracktwo_L1TAU12'              : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-13061'),
                                                                         17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_dikaonmass_tracktwo_L1TAU12'              : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-13061')},

    'HLT_g35_medium_tau35_dikaonmass_tracktwo_L1TAU12'              : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-13061'),
                                                                         17001 : TriggerRule(PS=-1, comment='Disabled') },

    'HLT_g25_medium_tau25_dikaonmasstight_tracktwo_60mVis10000'              : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-14643'),
                                                                         17001 : TriggerRule(PS=-1, comment='Disabled') }, 
    'HLT_g25_medium_L1EM24VHI_tau25_dikaonmasstight_tracktwo_60mVis10000'              : {  1000 : TriggerRule(PS=1, comment='Primary, Higgs, ATR-14643'),},

    'HLT_g35_medium_tau25_singlepion_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, W->pion+photon, ATR-13525'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_singlepion_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, W->pion+photon, ATR-1352')},


    'HLT_g35_medium_tau25_singlepiontight_tracktwo_L1TAU12'     : {  1000 : TriggerRule(PS=1, comment='Primary, W->pion+photon, ATR-13525'),
                                                                    17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_singlepiontight_tracktwo_L1TAU12'     : {  1000 : TriggerRule(PS=1, comment='Primary, W->pion+photon, ATR-13525')},


    'HLT_g35_medium_tau25_dipion1_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                 17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_dipion1_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},

    'HLT_g35_medium_tau35_dipion1loose_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                       17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau35_dipion1loose_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},


    'HLT_g35_medium_tau25_dipion2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                 17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_dipion2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},


    'HLT_g35_medium_tau35_dipion2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                  17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau35_dipion2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},


    'HLT_g35_medium_tau25_kaonpi1_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                 17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_kaonpi1_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},


    'HLT_g35_medium_tau35_kaonpi1_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                 17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau35_kaonpi1_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},

    'HLT_g35_medium_tau25_kaonpi2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                 17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau25_kaonpi2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},


    'HLT_g35_medium_tau35_kaonpi2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841'),
                                                                 17001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_g35_medium_L1EM24VHI_tau35_kaonpi2_tracktwo_L1TAU12'          : {  1000 : TriggerRule(PS=1, comment='Primary, ATR-13841')},



    
}

HLT_list=['HLT_g25_medium_L1EM24VHI_tau25_dikaonmass_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_dipion1loose_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_dipion2_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_kaonpi1_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_kaonpi2_tracktwo_50mVis10000',
          'HLT_g25_medium_L1EM24VHI_tau25_singlepion_tracktwo_50mVis10000',
          ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Primary, ATR-14947')
                    }])))
HLT_list=['HLT_g25_medium_tau25_dikaonmass_tracktwo_50mVis10000',
          'HLT_g25_medium_tau25_dipion1loose_tracktwo_50mVis10000',
          'HLT_g25_medium_tau25_dipion2_tracktwo_50mVis10000',
          'HLT_g25_medium_tau25_kaonpi1_tracktwo_50mVis10000',
          'HLT_g25_medium_tau25_kaonpi2_tracktwo_50mVis10000',
          'HLT_g25_medium_tau25_singlepion_tracktwo_50mVis10000',
          ]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary, ATR-14947')
                    }])))

HLT_list=['HLT_j30_xe60_razor220']
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary razor, ATR-12890')
                    }])))
HLT_list=['HLT_j250_jes_cleanLLP_PS_revllp_L1TAU60',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary revLLP, ATR-14293'),
                   10001 : TriggerRule(PS=100, comment='Support, Needs alternative, revLLP, ATR-14293')
                    }])))
HLT_list=['HLT_mu20_msonly_mu15noL1_msonly_nscan05_noComb','HLT_mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=1, comment='Primary LLP, ATR-10933 should not be prescaled'),
                    20001 : TriggerRule(PS=-1, comment='Disabled'),
                    }])))
HLT_list=['HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb','HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO',]
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled')
                    }])))
HLT_list=['HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU6_EMPTY','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','HLT_mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO','HLT_mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO'] 
dedicated_Rules_2e34.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(PS=-1, comment='Disabled ATR-15236, ATR-14377')  
                    }])))
    
dedicated_Rules={}


RulesList=[dedicated_Rules_2e34, dedicated_Rules_supporting, dedicated_Rules_2e34_L1Topo] 
    
for Rules in RulesList:
    for newrule in Rules.keys():
        if dedicated_Rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    dedicated_Rules.update(Rules)

#################################################################################################
#
#
# Data scouting
#
#
#################################################################################################

doEmergency2200b = 1
if emergency2200b_HLTOnly:
     doEmergency2200b = -1
    
datascouting_Rules = {
    'HLT_j0_perf_ds1_L1J100'               : {  1000 : TriggerRule(PS=1, comment='Unprescaled for all lumis') },
    'HLT_j0_perf_ds1_L1J75'                : {  1000 : TriggerRule(PS=-1, comment='Primary, DS (Disabled until ATR-13922 is solved)'), 
                                                7001 : TriggerRule(PS=-1, comment='Disabled') },

    'HLT_j0_1i2c200m8000TLA' : {  1000 : TriggerRule(rate=1, comment='DS TLA unprescaled, ATR-14081'), },
    'HLT_j0_1i2c300m500TLA': {  1000 : TriggerRule(rate=1, comment='DS TLA at 1Hz'), },
    'HLT_j0_0i1c500m900TLA' : {  1000 : TriggerRule(rate=1, comment='DS TLA unprescaled, ATR-14081'), },

    'HLT_j0_0i1c500m2000TLA_delayed' : {  1000 : TriggerRule(PS=1, comment='DS'), },
    'HLT_j0_0i1c500m900TLA_delayed'  : {  1000 : TriggerRule(PS=-1, comment='DS TLA Disabled'), },
    'HLT_j0_1i2c200m8000TLA_delayed' : {  1000 : TriggerRule(PS=doEmergency2200b, comment='emergency2200b, DS TLA EndOfFill'),
                                          7001 : TriggerRule(PS=-1, comment='DS TLA EndOfFill until tighted more'),
                                          },
    
    
    }
# we need to insert here the new DS like HLT_j0_0i1c400m600TLA

#################################################################################################
#
#
# MinBias
#
#
#################################################################################################

#Prepare for ES rules
MinBias_Rules={}
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

import Monitoring_pp_v6_rules

zerobias_Rules = {
    # ATR-9878
    'HLT_noalg_zb_L1ZB'        : { 1 : TriggerRule(rate=7, comment='7 Hz target at 1.2e34') },
    'HLT_j40_L1ZB'             : { 1 : TriggerRule(rate=3, comment='3 Hz target at 1.2e34, HLT acceptance is 3%') },
    }

tauoverlay_Rules = { 
    'L1_2MU20_OVERLAY'             : { 1 : TriggerRule(rate=180, comment='TauOverlay, ATR-10801') },
    'HLT_noalg_to_L12MU20_OVERLAY' : { 1 : TriggerRule(rate=0.1, comment='TauOverlay') },
    'HLT_2mu20_L12MU20_OVERLAY'    : { 1 : TriggerRule(rate=5, comment='TauOverlay, ATR-10801') },  
    }

PSstartup2200 = 500
if startup2200b:
    PSstartup2200 = 1500

# grep sig_nam costMonitoring_20.11.0.2-ATR13473-13TeV-L1.0e34-all-RB00-04-20_280500/xml/TrigRate_20.11.0.2-ATR13473-13TeV-L1.0e34-all-RB00-04-20_HLT_SMK_294_L1_91_HLT_227.xml | grep "-" | sort | sed "s/<sig_name>/'/g"  | sed "s,</sig_name>,AAA,g" | sed "s/AAA/',/g" | awk '{printf("%s ",$1)}'

#L1Topo item for commisioning
topo_items=['L1_J4-MATCH',        
            'L1_MJJ-100',        
            'L1_MJJ-400',        
            'L1_MJJ-700',        
            'L1_MJJ-800',        
            'L1_MJJ-900',        
            'L1_HT150-J20.ETA31',  
            'L1_HT150-J20s5.ETA31',
            'L1_W-HT20-JJ15.ETA49'
            ]

topo_Rules={}
if NoL1Topo:
    topo_Rules.update(dict(map(None,topo_items,len(topo_items)*[{
                        1000 : TriggerRule(PS=-1, comment='Topo'),
                        }])))
else:
    topo_Rules.update(dict(map(None,topo_items,len(topo_items)*[{
                        1000 : TriggerRule(rate=0.5, comment='Topo'),
                        }])))

sometopo_items=['L1_HT190-J15.ETA21',
                'L1_HT190-J15s5.ETA21'
                ]
if NoL1Topo:
    topo_Rules.update(dict(map(None,sometopo_items,len(sometopo_items)*[{
                        1000 : TriggerRule(PS=-1, comment='Topo'),
                        }])))

hlt_topo_items=[ 'HLT_noalg_l1topo_L1J4-MATCH',

    'HLT_noalg_l1topo_L1MJJ-900',
    'HLT_noalg_l1topo_L1MJJ-800',
    'HLT_noalg_l1topo_L1MJJ-700',
    'HLT_noalg_l1topo_L1MJJ-400',
    'HLT_noalg_l1topo_L1MJJ-100',
    'HLT_noalg_l1topo_L1HT150-J20.ETA31',
    'HLT_noalg_l1topo_L1HT150-J20s5.ETA31',
    'HLT_noalg_l1topo_L1W-HT20-JJ15.ETA49',
    ]

topo_Rules.update(dict(map(None,hlt_topo_items,len(hlt_topo_items)*[{
                    1000 : TriggerRule(PS=1, comment='HLT Topo'),
                    }])))

hlt_topo_items=['HLT_noalg_l1topo_L1HT190-J15.ETA21', 'HLT_noalg_l1topo_L1HT190-J15s5.ETA21',
    ]
topo_Rules.update(dict(map(None,hlt_topo_items,len(hlt_topo_items)*[{
                    1000 : TriggerRule(rate=0.5, comment='HLT Topo'),
                    }])))


topo_unPS_items=['L1_DR-TAU20ITAU12I-J25']  #2.8kHz
if NoL1Topo:
    topo_Rules.update(dict(map(None,topo_unPS_items,len(topo_unPS_items)*[{
                        1000 : TriggerRule(PS=-1, comment='Topo'),
                        }])))
else:
    topo_Rules.update(dict(map(None,topo_unPS_items,len(topo_unPS_items)*[{
        1000 : TriggerRule(PS=1, comment='Primary Topo'),
       20001 : TriggerRule(PS=-1, comment='Disable'),
                        }])))

topo_unPS_items=['L1_DR-TAU20ITAU12I',]  #4.2kHz
if NoL1Topo:
    topo_Rules.update(dict(map(None,topo_unPS_items,len(topo_unPS_items)*[{
                        1000 : TriggerRule(PS=-1, comment='Topo'),
                        }])))
else:
    topo_Rules.update(dict(map(None,topo_unPS_items,len(topo_unPS_items)*[{
        1000 : TriggerRule(PS=1, comment='Primary Topo'),
        12001 : TriggerRule(PS=-1, comment='Disable at high lumi Topo'),
                        }])))
    
#New L1_LLP_RO (ATR-14428)
L1_LLP_list=['L1_LLP-RO',]
topo_Rules.update(dict(map(None,L1_LLP_list,len(L1_LLP_list)*[{
                           1 : TriggerRule(PS=1, comment='Primary ATR-14428, ATR-15236'),
                        }])))

HLT_LLP_list = ['HLT_j30_jes_cleanLLP_PS_llp_L1LLP-RO', 'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1LLP-RO']
topo_Rules.update(dict(map(None,HLT_LLP_list,len(HLT_LLP_list)*[{
                          1 : TriggerRule(PS=1, comment='Primary, ATR-14428, ATR-15236'),
                        }])))

hlt_topo_items=[ 
                'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I', #HLT 27Hz #L1 4.2kHz
                'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I-J25', #HLT 16Hz #L1 2.8kHz
                
                ]
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
  'L1_BPH-2M8-2MU4'                      : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  9001 : TriggerRule(rate=0.5, comment='L1Topo DY')},
  'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4'  : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  9001 : TriggerRule(PS=2, comment='L1Topo DY'),  17001 : TriggerRule(PS=-1,  comment='Disabled if 2MU6 is disabled')},
  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6'      : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  17001 : TriggerRule(PS=-1,  comment='Disabled if 2MU6 is disabled')},
  'L1_2MU4-B'                            : { 1: TriggerRule(PS=10, comment='L1Topo Bphys, EndOfFill, ATR-14108') , 7001 : TriggerRule(rate=0.5, comment='L1Topo DY')},
  'L1_MU6MU4-BO'                        : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  9001 : TriggerRule(rate=0.5, comment='L1Topo DY')},
}



hlt_topo_bphys_rules = {
    ### YU 'HLT_2mu4_bBmumu_L1BPH-2M8-2MU4'                          : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_2mu4_bJpsimumu_L1BPH-2M8-2MU4'                        : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108') ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    ### YU 'HLT_mu6_mu4_bBmumu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4'    : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bJpsimumu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4'  : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    ### YU 'HLT_2mu6_bBmumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6'            : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_2mu6_bJpsimumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6'        : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bBmumu_L12MU4-B'                              : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bJpsimumu_L12MU4-B'                          : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bBmumu_L1MU6MU4-BO'                          : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    'HLT_mu6_mu4_bJpsimumu_L1MU6MU4-BO'                        : { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  ,  7001 : TriggerRule(PS=-1, comment='Disabled') },

    'HLT_mu6_mu4_bBmumuxv2_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=50, comment='65Hz at 1e34 PS=1') ,  7001 : TriggerRule(PS=-1, comment='Disabled') },
    
    'HLT_mu6_mu4_bBmumux_BsmumuPhi_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 4Hz at 1e34 PS=1') },
    
    'HLT_mu6_mu4_bBmumux_BcmumuDsloose_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 14Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled') },  
    'HLT_mu6_mu4_bJpsimumu_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=5, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=5, comment='L1Topo Bphys, EndOfFill, 75Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=20, comment='L1Topo Bphys') },
    
    'HLT_mu6_mu4_bJpsimumu_Lxy0_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 55Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=10, comment='L1Topo Bphys') },
    
    'HLT_mu6_mu4_bJpsimumu_delayed_L1MU6MU4-BO': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 30Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled') },
    
    'HLT_mu6_mu4_bBmumuxv2_delayed_L1MU6MU4-BO': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 30Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled') },
    
    'HLT_mu6_mu4_bUpsimumu_delayed_L1BPH-8M15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 39Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled') },
    
    'HLT_2mu6_bBmumuxv2_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 24Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled') },
    
    'HLT_2mu6_bBmumux_BsmumuPhi_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ~1Hz at 1e34 PS=1')},
    
    'HLT_2mu6_bBmumux_BcmumuDsloose_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ~5Hz at 1e34 PS=1')},
    
    'HLT_2mu6_bJpsimumu_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 29Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_2mu6_bJpsimumu_Lxy0_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 21Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_2mu6_bUpsimumu_delayed_L1BPH-8M15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 7Hz at 1e34 PS=1')},

    #'HLT_2mu4_bUpsimumu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill')  , 9001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 27Hz at 1e34 L1PS=2'), 14001: TriggerRule(PS=-1, comment='L1Topo Bphys, EndOfFill')},
    
    'HLT_2mu4_bBmumu_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 22Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_2mu4_bBmumu_Lxy0_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 10Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_2mu4_bBmumux_BsmumuPhi_delayed_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 10Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=1, comment='L1Topo Bphys')},

    'HLT_2mu4_bJpsimumu_Lxy0_delayed_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=20, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=20, comment='L1Topo Bphys, EndOfFill, 10Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=20, comment='L1Topo Bphys')},

    'HLT_2mu4_bJpsimumu_delayed_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=40, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=40, comment='L1Topo Bphys, EndOfFill, 10Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=40, comment='L1Topo Bphys')},

    'HLT_2mu4_bBmumux_BsmumuPhi_L1BPH-2M8-2MU4': { 1: TriggerRule(PS=10, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=10, comment='L1Topo Bphys, EndOfFill, 10Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=10, comment='Disabled')},

    'HLT_mu6_mu4_bBmumu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=2, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=2, comment='L1Topo Bphys, EndOfFill, 12Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=2, comment='L1Topo Bphys')},
    
    'HLT_mu6_mu4_bBmumu_Lxy0_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 7Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=1, comment='L1Topo Bphys')},
    
    'HLT_mu6_mu4_bBmumux_BsmumuPhi_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=10, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=10, comment='L1Topo Bphys, EndOfFill, 4Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=10, comment='L1Topo Bphys')},

    'HLT_mu6_mu4_bJpsimumu_Lxy0_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=10, comment='55Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},

    
    'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 14Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_2mu6_bBmumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 3Hz at 1e34 PS=1')},

    'HLT_2mu6_bBmumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ~2Hz at 1e34 PS=1')},

    'HLT_2mu6_bBmumux_BsmumuPhi_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 1Hz at 1e34 PS=1')},

    'HLT_2mu6_bJpsimumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=5, comment='21Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
    
    'HLT_2mu6_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU6_BPH-2DR15-2MU6': { 1: TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, ATR-14108')  , 4001 : TriggerRule(PS=1, comment='L1Topo Bphys, EndOfFill, 5Hz at 1e34 PS=1'), 7001 : TriggerRule(PS=-1, comment='Disabled')},
      
    }


l1_topo_lfv_items=['L1_LFV-MU','L1_LFV-EM8I','L1_LFV-EM15I',]
hlt_topo_lfv_items = [
'HLT_g10_loose_mu10_iloose_taumass_L1LFV-EM8I', #5.8Hz #L1 830Hz at 1e34
'HLT_g20_loose_mu4_iloose_taumass_L1LFV-EM15I', #5.5Hz #L1 430Hz
'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU', #3Hz L1 2.7kHz
'HLT_mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU_bTau', #0.3Hz
'HLT_mu11_nomucomb_mu6noL1_nscan03_L1LFV-MU_bTau_delayed', #17Hz # unique ~1Hz
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

if NoL1TopoHLTCom : 
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

else:
    topo_Rules.update(dict(map(None,hlt_topo_items,len(hlt_topo_items)*[{
                    1000 : TriggerRule(PS=1, comment='HLT Topo'),
                    }])))

    topo_Rules.update(hlt_topo_tau_rules)

    topo_Rules.update(l1_topo_dphijxe_rules)
    topo_Rules.update(hlt_topo_dphijxe_rules)
    
    topo_Rules.update(l1_topo_bphys_rules)
    topo_Rules.update(hlt_topo_bphys_rules)

    topo_Rules.update(dict(map(None,l1_topo_lfv_items,len(l1_topo_lfv_items)*[{
                    1000 : TriggerRule(PS=1, comment='L1 Topo, validated fully, unprescaled as long as MU11_2MU6 is unprescaled (ATR-14352)'),
                    }])))
    topo_Rules.update(dict(map(None,hlt_topo_lfv_items,len(hlt_topo_lfv_items)*[{
                    1000 : TriggerRule(PS=1, comment='HLT Topo, validated fully'),
                    }])))

    topo_Rules.update(l1_topo_dy_rules)
    topo_Rules.update(hlt_topo_dy_rules)

disabled_topo_list=[
 'L1_3J15_BTAG-MU4J15', 
'L1_3J15_BTAG-MU4J30', 'L1_3J15_BTAG-MU6J25', 'L1_3J20_4J20.0ETA49_MJJ-400', 'L1_3J20_4J20.0ETA49_MJJ-700', 'L1_3J20_4J20.0ETA49_MJJ-800', 'L1_3J20_4J20.0ETA49_MJJ-900', 'L1_3J20_BTAG-MU4J20', 'L1_BOX-TAU20ITAU12I','L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4', 'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4', 'L1_BPH-2M8-2MU4-BO_BPH-0DR15-2MU4', 'L1_BPH-2M8-2MU4-B_BPH-0DR15-2MU4', 'L1_BPH-2M8-2MU4_BPH-0DR15-2MU4',  'L1_BPH-7M15-2MU4_BPH-0DR24-2MU4-B', 'L1_BPH-7M15-2MU4_BPH-0DR24-2MU4-BO', 'L1_BPH-7M15-2MU4_BPH-0DR24-2MU4', 'L1_BPH-8M15-2MU6_BPH-0DR22-2MU6', 'L1_BPH-8M15-MU6MU4', 'L1_BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4', 'L1_BTAG-MU4J20', 'L1_DPHI-AJ20s2XE50', 'L1_DPHI-CJ20XE50', 'L1_DPHI-J20XE50', 'L1_DPHI-J20s2XE50', 'L1_DPHI-Js2XE50', 'L1_DR-EM15TAU12I-J25', 'L1_DR-EM15TAU12I', 'L1_DR-MU10TAU12I', 'L1_EM10VH_W-MT35_XS50', 'L1_EM12_W-MT25', 'L1_EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE', 'L1_EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_W-90RO2-XEHT-0', 'L1_EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20', 'L1_EM12_W-MT30', 'L1_EM12_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EMXE', 'L1_EM12_W-MT35_W-90RO2-XEHT-0_W-15DPHI-JXE-0_W-15DPHI-EMXE', 'L1_EM12_W-MT35_XS30_W-15DPHI-JXE-0_W-15DPHI-EMXE', 'L1_EM12_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EMXE', 'L1_EM12_W-MT35_XS50', 'L1_EM15-TAU12I', 'L1_EM15-TAU40', 'L1_EM15TAU12I-J25', 'L1_EM15VH_W-MT35_XS60', 'L1_EM15_W-MT35', 'L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_W-250RO2-XEHT-0_W-15DPHI-JXE-0_W-15DPHI-EM15XE', 'L1_EM15_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_XS40_W-15DPHI-JXE-0_W-15DPHI-EM15XE', 'L1_EM15_W-MT35_XS60', 'L1_EM15_W-MT35_XS60_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_XS60_W-15DPHI-JXE-0_W-15DPHI-EM15XE', 'L1_EM20VH_W-MT35_XS60', 'L1_EM22VHI_W-MT35_XS40',  'L1_HT150-JJ15.ETA49', 'L1_J25_2J20_3J12_BOX-TAU20ITAU12I', 'L1_J30_2J20_4J20.0ETA49_MJJ-400', 'L1_J30_2J20_4J20.0ETA49_MJJ-700', 'L1_J30_2J20_4J20.0ETA49_MJJ-800', 'L1_J30_2J20_4J20.0ETA49_MJJ-900',  'L1_J40_DPHI-CJ20XE50', 'L1_J40_DPHI-J20XE50', 'L1_J40_DPHI-Js2XE50', 'L1_JPSI-1M5-EM12', 'L1_JPSI-1M5-EM7', 'L1_JPSI-1M5', 'L1_KF-XE35', 'L1_KF-XE45', 'L1_KF-XE55', 'L1_KF-XE60', 'L1_KF-XE65', 'L1_KF-XE75', 'L1_MJJ-200', 'L1_MJJ-300',  'L1_MU10_TAU12I-J25', 'L1_MU6_MJJ-200', 'L1_MU6_MJJ-300', 'L1_MU6_MJJ-400', 'L1_MU6_MJJ-500', 'L1_TAU12I-J25', 'L1_TAU20-J20', 'L1_TAU20ITAU12I-J25', 'L1_W-05DPHI-EMXE-1', 'L1_W-05DPHI-JXE-0', 'L1_W-05RO-XEHT-0', 'L1_W-10DPHI-EMXE-0', 'L1_W-10DPHI-JXE-0', 'L1_W-15DPHI-EMXE-0', 'L1_W-15DPHI-JXE-0', 'L1_W-250RO2-XEHT-0', 'L1_W-90RO2-XEHT-0', 'L1_W-NOMATCH', 'L1_W-NOMATCH_W-05RO-XEEMHT', 'L1_XE35_EM15-TAU12I', 'L1_XE35_MJJ-200', 'L1_XE40_EM15-TAU12I', 'L1_XE45_TAU20-J20', 'L1_CMU4', 
    ]

if NoHLTTopo and NoL1TopoHLTCom:
    disabled_topo_list+=[
       'L1_LLP-RO','L1_LFV-MU','L1_LFV-EM8I','L1_LFV-EM15I',
				'L1_BPH-2M8-2MU4','L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',
				'L1_2MU4-B','L1_2MU4-BO','L1_MU6_2MU4-B','L1_MU6MU4-BO','L1_2MU6-B','L1_2MU6-BO',
				'L1_DY-BOX-2MU6','L1_DY-BOX-MU6MU4','L1_DY-DR-2MU4',
				'L1_BTAG-MU4J15','L1_BTAG-MU4J30','L1_BTAG-MU6J20','L1_BTAG-MU6J25','L1_DY-BOX-2MU4',
        ]
    
topo_Rules.update(dict(map(None,disabled_topo_list,len(disabled_topo_list)*[{
                        1000 : TriggerRule(PS=-1, comment='Disabled Topo'),
                        }])))

l1calo_streamer_Rules = {
    'HLT_noalg_l1topo_L12EM3'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU6'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L14J15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L14J20.0ETA49'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1EM12_2EM3'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    #'HLT_noalg_l1topo_L1EM15HI'    : { 1000 : TriggerRule(rate=0.5, comment='')},
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
    'HLT_noalg_l1calo_L1J400'    : { 1000 : TriggerRule(PS=1, comment='')},
    'HLT_noalg_l1topo_L1J30.0ETA49_2J20.0ETA49'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1J40_XE50'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU10_2MU6'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU10_TAU12IM'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU4_J12'    : { 1000 : TriggerRule(rate=0.5, comment='')},
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

    'HLT_noalg_l1topo_L1BPH-2M8-2MU4'  : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BPH-2M9-2MU6_BPH-2DR15-2MU6'        : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU4-B'        : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU4-BO'        : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU6_2MU4-B'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1MU6MU4-BO'      : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU6-B'        : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L12MU6-BO'        : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LFV-EM8I'        : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LFV-EM15I'      : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1LFV-MU'          : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DY-BOX-2MU6'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BTAG-MU4J15'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BTAG-MU4J30'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BTAG-MU6J20'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1BTAG-MU6J25'    : { 1000 : TriggerRule(rate=0.5, comment='')},
#    'HLT_noalg_l1topo_L1DY-BOX-2MU4'    : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DY-DR-2MU4'      : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DR-TAU20ITAU12I-J25' : { 1000 : TriggerRule(rate=0.5, comment='')},
    'HLT_noalg_l1topo_L1DR-TAU20ITAU12I'     : { 1000 : TriggerRule(rate=0.5, comment='')},


}


HLT_list=['HLT_mu4_j35_bperf_split_dr05_dz02_L1BTAG-MU4J15', 'HLT_mu6_j85_bperf_split_dr05_dz02_L1BTAG-MU6J20' ]
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

disabled_Rules={}

# --------------------------------------
# signatures, EB

#grep "has no rules and will be disabled" a.log | awk -F"--> " '{print $2}' | sed "s/ has no rules and will be disabled//g" | sort | awk '{printf("\"%s\", ",$1)}'

HLT_list=[
'HLT_mu20_msonly_mu10noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO',
'HLT_mb_sp1000_pusup450_trk70_hmt_L1TE5.0ETA24',
'HLT_mb_sp900_pusup400_trk60_hmt_L1TE5.0ETA24',
'HLT_2j10_deta20_L1J12',
'HLT_2j10_deta20_L1MBTS_2',
'HLT_e10_lhloose_nod0_L1EM3',
'HLT_e10_lhmedium_nod0_L1EM3',
'HLT_j10_320eta490_L1MBTS_2',
'HLT_j10_L1MBTS_2',
'HLT_g10_loose_L1EM3',
'HLT_g10_medium_L1EM3',

'HLT_mu20_msonly_mu10noL1_msonly_nscan03_noComb',
'HLT_mu20_msonly_mu10noL1_msonly_nscan03_noComb_L1MU6_EMPTY',
'HLT_mu14_ivarloose_tau25_perf_tracktwo',
'HLT_mu14_iloose_3j20_L1MU10_3J20',
'HLT_mu6_ivarloose_mu6_11invm24_noos_novtx',
'HLT_mu6_ivarloose_mu6_24invm60_noos',
'HLT_mu6_ivarloose_mu6_24invm60_noos_novtx',
'HLT_mu6_ivarloose_mu6_11invm24_noos',
'HLT_mu4_ivarloose_mu4_11invm60_noos',
'HLT_mu4_ivarloose_mu4_7invm9_noos_novtx',
'HLT_mu4_ivarloose_mu4_7invm9_noos',
'HLT_mu4_ivarloose_mu4_11invm60_noos_novtx',
'HLT_mu14_iloose_tau25_medium1_tracktwo_xe50',
'HLT_mu26_imedium_2j35_bperf',
'HLT_mu26_imedium_2j35_bperf_split',
'HLT_mu26_imedium_2j35_boffperf',
'HLT_mu26_imedium_2j35_boffperf_split',
'HLT_g20_etcut_L1EM15I_mu4_ivarloose_taumass',
'HLT_g10_etcut_L1EM8I_mu10_ivarloose_taumass',
'HLT_g10_loose_L1EM8I_mu10_ivarloose_taumass',
'HLT_g20_loose_L1EM15I_mu4_ivarloose_taumass',
"HLT_g20_etcut_L1EM15_mu4_ivarloose_taumass",
"HLT_g20_loose_L1EM15_mu4_iloose_taumass" 
"HLT_g20_loose_L1EM15_mu4_ivarloose_taumass", 
'HLT_mu20_imedium_L1MU10_2J20',
'HLT_mu20_imedium_L1MU10',
'HLT_mu28_imedium',
'HLT_mu24_iloose',
'HLT_mu14_iloose',
'HLT_mu20_iloose_L1MU15',
'HLT_mu14_iloose_tau25_medium1_tracktwo',
'HLT_mu14_iloose_tau35_medium1_tracktwo',
'HLT_mu14_iloose_tau25_medium1_tracktwo_L1MU10_TAU12IM',
'mu14_iloose_tau25_medium1_tracktwo_xe50',
'mu14_iloose_3j20_L1MU10_3J20',
    'HLT_e26_lhtight_iloose',
'HLT_e26_lhtight_ivarloose',
'HLT_e28_lhtight_iloose',
'HLT_e28_lhtight_ivarloose',
'HLT_e60_lhmedium',
'HLT_e60_lhmedium_L1EM24VHI',
'HLT_e140_lhloose',
'HLT_e140_lhloose_L1EM24VHI',
'HLT_2e15_lhvloose_L12EM13VH',
'HLT_2e17_lhvloose',
'HLT_e17_lhmedium_2e9_lhmedium',
'HLT_e17_lhloose_2e9_lhloose',
'HLT_e26_lhtight_e15_etcut_L1EM7_Zee',
'HLT_e5_lhtight_e4_etcut',
'HLT_e5_lhtight_e4_etcut_Jpsiee',
'HLT_e9_lhtight_e4_etcut_Jpsiee',
'HLT_e9_etcut_e5_lhtight_Jpsiee',
'HLT_e9_etcut_e5_tight_Jpsiee',
'HLT_e14_lhtight_e4_etcut_Jpsiee',
'HLT_e14_etcut_e5_lhtight_Jpsiee',
'HLT_e14_etcut_e5_tight_Jpsiee',
'HLT_e20_lhmedium_g35_loose',
'HLT_e20_lhmedium_2g10_loose',
'HLT_e20_lhmedium_2g10_medium',
'HLT_e24_lhmedium_L1EM15VH_g25_medium',
'HLT_e26_lhmedium_L1EM22VHI_mu8noL1',
'HLT_e24_lhmedium_L1EM20VHI_mu8noL1',
'HLT_e24_medium_L1EM20VHI_mu8noL1',
'HLT_e17_lhloose_mu14',
'HLT_e7_lhmedium_mu24',
'HLT_e12_lhloose_2mu10',
'HLT_e12_lhmedium_2mu10',
'HLT_2e12_lhloose_mu10',
'HLT_2e12_lhmedium_mu10',
'HLT_e17_lhmedium_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
'HLT_e60_loose_xe60noL1',
'HLT_e60_lhloose_xe60noL1',
'HLT_e70_lhloose_xe70noL1',
'HLT_e15_lhtight_iloose_3j20_L1EM13VH_3J20',
'HLT_e15_lhtight_ivarloose_3j20_L1EM13VH_3J20',
'HLT_e26_tight_iloose_2j35_bperf',
'HLT_e26_tight_iloose_2j35_bperf_split',
'HLT_e26_tight_iloose_2j35_boffperf',
'HLT_e26_tight_iloose_2j35_boffperf_split',
'HLT_2e17_loose_2j35_bperf',
'HLT_2e17_loose_2j35_bperf_split',
'HLT_2e17_loose_2j35_boffperf',
'HLT_2e17_loose_2j35_boffperf_split',
'HLT_e24_lhvloose_L1EM18VH',
'HLT_e24_lhvloose_L1EM20VH',
'HLT_e24_lhvloose_L1EM20VH_3j20noL1',
'HLT_e24_lhvloose_L1EM20VH_4j20noL1',
'HLT_e24_lhvloose_L1EM20VH_5j15noL1',
'HLT_e24_lhvloose_L1EM20VH_6j15noL1',
'HLT_e26_lhvloose_L1EM20VH',
'HLT_e26_lhvloose_L1EM20VH_3j20noL1',
'HLT_e26_lhvloose_L1EM20VH_4j20noL1',
'HLT_e26_lhvloose_L1EM20VH_5j15noL1',
'HLT_e26_lhvloose_L1EM20VH_6j15noL1',
'HLT_e28_lhvloose_L1EM20VH_3j20noL1',
'HLT_e28_lhvloose_L1EM20VH_4j20noL1',
'HLT_e28_lhvloose_L1EM20VH_5j15noL1',
'HLT_e28_lhvloose_L1EM20VH_6j15noL1',
'HLT_tau35_medium1_track',
"HLT_noalg_L1XE35", 'HLT_noalg_L1AFP_C_MBTS_A', 'HLT_noalg_L1AFP_NSC', 'HLT_xe110_tc_lcw_wEFMu_L1XE50', 'HLT_xe110_tc_lcw_L1XE50', 'HLT_xe110_tc_em_L1XE55', 'HLT_xe110_tc_lcw_wEFMu_L1XE55', 'HLT_xe110_pueta_L1XE55', 'HLT_xe120_pufit_L1XE55', 'HLT_xe120_pueta_L1XE55', 'HLT_xe110_tc_lcw_L1XE55', 'HLT_xe110_tc_em_wEFMu_L1XE55', 'HLT_noalg_L1AFP_C_ANY_MBTS_A', 'HLT_noalg_L1AFP_FSC', 'HLT_j10_L1AFP_C_ANY', 'HLT_noalg_L1AFP_C_ANY', 'HLT_j10_L1AFP_C_AND', 'HLT_noalg_L1AFP_C_AND', 'HLT_noalg_L1AFP_C_J12', 'HLT_noalg_L1AFP_C_TE5', 'HLT_noalg_L1AFP_C_EM3',"HLT_mb_sp1600_pusup600_trk100_hmt_L1TE20.0ETA24",'HLT_e4_etcut','HLT_e9_etcut','HLT_e14_etcut','HLT_j30_jes_cleanLLP_PS_llp_L1TAU8_EMPTY', 'HLT_j30_jes_cleanLLP_PS_llp_L1TAU8_UNPAIRED_ISO', 'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU8_EMPTY', 'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU8_UNPAIRED_ISO','HLT_noalg_bkg_L1MBTS_2_UNPAIRED_ISO',"HLT_noalg_cosmiccalo_L1RD1_BGRP10","HLT_mu0_muoncalib_ds3_L1MU15","HLT_beamspot_activeTE_trkfast_pebTRT","HLT_beamspot_allTE_trkfast_pebTRT","HLT_j50_bmv2c2040_split_3j50_L14J15","HLT_j30_muvtx_L1MU4_EMPTY","HLT_j30_muvtx_noiso_L1MU4_EMPTY","HLT_g15_loose_2mu10_msonly_L1MU4_EMPTY","HLT_3mu6_msonly_L1MU4_EMPTY",
"HLT_mu20_msonly_mu6noL1_msonly_nscan05_noComb_L1MU4_EMPTY","HLT_mu20_msonly_mu6noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO","HLT_noalg_standby_L14J15","HLT_2e17_loose_2j35_boffperf_split","HLT_2e17_loose_2j35_boffperf","HLT_2e17_loose_2j35_bperf_split","HLT_2e17_loose_2j35_bperf",
"HLT_5j70_lcw_nojcalib_L14J15", "HLT_5j70_jes_L14J15", "HLT_5j70_nojcalib_L14J15", "HLT_5j70_lcw_jes_L14J15", "HLT_5j70_lcw_L14J15","HLT_j40_cleanT_xe80_L1XE60","HLT_e26_tight_iloose_2j35_boffperf","HLT_e26_tight_iloose_2j35_boffperf_split","HLT_e26_tight_iloose_2j35_bperf","HLT_e26_tight_iloose_2j35_bperf_split","HLT_calibAFP_L1AFP_C_ANY_UNPAIRED_ISO", "HLT_calibAFP_L1AFP_C_ANY_UNPAIRED_NONISO", "HLT_calibAFP_L1AFP_C_ANY_EMPTY","HLT_noalg_standby_L1J15","HLT_noalg_standby_L13J15","HLT_beamspot_trkFS_trkfast_L1J15","HLT_beamspot_allTE_trkfast_peb_L1J15","HLT_beamspot_activeTE_trkfast_peb_L1J15","HLT_beamspot_allTE_trkfast_peb_L13J15","HLT_beamspot_activeTE_trkfast_peb_L13J15","HLT_beamspot_trkFS_trkfast_L13J15","HLT_calibAFP_L1AFP_C_ANY_FIRSTEMPTY","HLT_2j45_bmv2c2085_split_3j45", "HLT_mu24_L1MU15", 'HLT_mu4_msonly', 'HLT_mu11', 'HLT_tau35_medium1_tracktwo_L1TAU20',
        
        'HLT_mu20_msonly_mu6noL1_msonly_nscan05','HLT_mu20_msonly_mu6noL1_msonly_nscan05_L1MU4_EMPTY','HLT_mu20_ivarmedium_L1MU15' 'HLT_mu20_ivarmedium_L1MU15','HLT_mu20_imedium_L1MU15',
                'HLT_j50_bmv2c2040_split_3j50',  
                   'HLT_j65_bmv2c2070_split_3j65_L14J15.0ETA25',
                   'HLT_j70_bmv2c2077_split_3j70_L14J15.0ETA25',




        
 "HLT_2j30_0eta490_deta25_xe80_L1DPHI-AJ20s2XE50", "HLT_2j30_0eta490_deta35_xe60_L1DPHI-AJ20s2XE50", "HLT_2j30_0eta490_deta35_xe80_tc_lcw_L1DPHI-AJ20s2XE50", "HLT_2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-900", "HLT_2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-900","HLT_2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-900", "HLT_2j55_bmv2c2085_split_L1J30_2J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bmv2c2085_split_L1J30_2J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bmv2c2085_split_L1J30_2J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bmv2c2085_split_L1J30_2J20_4J20.0ETA49_MJJ-900", "HLT_2j55_boffperf_split_L13J20_4J20.0ETA49_MJJ-400", "HLT_2j55_boffperf_split_L13J20_4J20.0ETA49_MJJ-700", "HLT_2j55_boffperf_split_L13J20_4J20.0ETA49_MJJ-800", "HLT_2j55_boffperf_split_L13J20_4J20.0ETA49_MJJ-900", "HLT_2j55_boffperf_split_L1J30_2J20_4J20.0ETA49_MJJ-400", "HLT_2j55_boffperf_split_L1J30_2J20_4J20.0ETA49_MJJ-700", "HLT_2j55_boffperf_split_L1J30_2J20_4J20.0ETA49_MJJ-800", "HLT_2j55_boffperf_split_L1J30_2J20_4J20.0ETA49_MJJ-900", "HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bperf_L13J20_4J20.0ETA49_MJJ-900", "HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bperf_L1J30_2J20_4J20.0ETA49_MJJ-900", "HLT_2j55_bperf_split_L13J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bperf_split_L13J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bperf_split_L13J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bperf_split_L13J20_4J20.0ETA49_MJJ-900", "HLT_2j55_bperf_split_L1J30_2J20_4J20.0ETA49_MJJ-400", "HLT_2j55_bperf_split_L1J30_2J20_4J20.0ETA49_MJJ-700", "HLT_2j55_bperf_split_L1J30_2J20_4J20.0ETA49_MJJ-800", "HLT_2j55_bperf_split_L1J30_2J20_4J20.0ETA49_MJJ-900", "HLT_2mu10_bDimu",
 "HLT_2mu10_bDimu_novtx_noos",
 "HLT_2mu4_bBmumu_L12MU4-BO", "HLT_2mu4_bBmumu_L12MU4-B", "HLT_2mu4_bBmumu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bBmumu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bBmumu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bBmumu_L1BPH-2M8-2MU4",
 "HLT_2mu4_bBmumu_noL2_L12MU4-BO", "HLT_2mu4_bBmumu_noL2_L12MU4-B", "HLT_2mu4_bBmumu_noL2_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bBmumu_noL2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bBmumu_noL2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bBmumu_noL2_L1BPH-2M8-2MU4", "HLT_2mu4_bBmumu_noL2_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bBmumu_noL2_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bBmumu_noL2_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_L12MU4-BO", "HLT_2mu4_bBmumux_BcmumuDsloose_L12MU4-B", "HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L12MU4-BO", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L12MU4-B", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-2M8-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bBmumuxv2_L12MU4-BO", "HLT_2mu4_bBmumuxv2_L12MU4-B",
"HLT_2mu4_bBmumuxv2_L1BPH-2M8-2MU4", "HLT_2mu4_bBmumuxv2_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bBmumuxv2_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bBmumuxv2_noL2_L12MU4-BO", "HLT_2mu4_bBmumuxv2_noL2_L12MU4-B", "HLT_2mu4_bBmumuxv2_noL2_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bBmumuxv2_noL2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bBmumuxv2_noL2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bBmumuxv2_noL2_L1BPH-2M8-2MU4", "HLT_2mu4_bBmumuxv2_noL2_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bBmumuxv2_noL2_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bBmumuxv2_noL2_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_L12MU4-BO", "HLT_2mu4_bDimu_L12MU4-B", "HLT_2mu4_bDimu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_L1BPH-2M8-2MU4", "HLT_2mu4_bDimu_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-B", "HLT_2mu4_bDimu_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-BO", "HLT_2mu4_bDimu_noL2_L12MU4-BO", "HLT_2mu4_bDimu_noL2_L12MU4-B", "HLT_2mu4_bDimu_noL2_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-2M8-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-7M15-2MU4_BPH-0DR24-2MU4", "HLT_2mu4_bDimu_noL2_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-B", "HLT_2mu4_bDimu_noL2_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-BO", "HLT_2mu4_bDimu_novtx_noos_L12MU4-BO", "HLT_2mu4_bDimu_novtx_noos_L12MU4-B", "HLT_2mu4_bDimu_novtx_noos_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_novtx_noos_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_novtx_noos_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_novtx_noos_L1BPH-2M8-2MU4", "HLT_2mu4_bDimu_novtx_noos_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_novtx_noos_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_novtx_noos_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_novtx_noos_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-B", "HLT_2mu4_bDimu_novtx_noos_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-BO", "HLT_2mu4_bDimu_novtx_noos_noL2_L12MU4-BO", "HLT_2mu4_bDimu_novtx_noos_noL2_L12MU4-B", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-2M8-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-7M15-2MU4_BPH-0DR24-2MU4", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-B", "HLT_2mu4_bDimu_novtx_noos_noL2_L1BPH-7M15-2MU4_BPH-0DR24-2MU4-BO", "HLT_2mu4_bJpsimumu_L12MU4-BO", "HLT_2mu4_bJpsimumu_L12MU4-B", "HLT_2mu4_bJpsimumu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bJpsimumu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bJpsimumu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4",# "HLT_2mu4_bJpsimumu_L1BPH-2M8-2MU4",
 "HLT_2mu4_bJpsimumu_noL2_L12MU4-BO", "HLT_2mu4_bJpsimumu_noL2_L12MU4-B", "HLT_2mu4_bJpsimumu_noL2_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4", "HLT_2mu4_bJpsimumu_noL2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bJpsimumu_noL2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4", "HLT_2mu4_bJpsimumu_noL2_L1BPH-2M8-2MU4", "HLT_2mu4_bJpsimumu_noL2_L1BPH-2M8-2MU4-BO_BPH-0DR15-2MU4", "HLT_2mu4_bJpsimumu_noL2_L1BPH-2M8-2MU4-B_BPH-0DR15-2MU4", "HLT_2mu4_bJpsimumu_noL2_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bUpsimumu_L12MU4-BO", "HLT_2mu4_bUpsimumu_L12MU4-B", "HLT_2mu4_bUpsimumu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4", "HLT_2mu4_bUpsimumu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4",
"HLT_2mu6_bBmumu_L12MU6-B",
 "HLT_2mu6_bBmumu_noL2_L12MU6-BO", "HLT_2mu6_bBmumu_noL2_L12MU6-B", "HLT_2mu6_bBmumu_noL2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6", "HLT_2mu6_bBmumux_BcmumuDsloose_L12MU6-BO", "HLT_2mu6_bBmumux_BcmumuDsloose_L12MU6-B", "HLT_2mu6_bBmumux_BcmumuDsloose_noL2_L12MU6-BO", "HLT_2mu6_bBmumux_BcmumuDsloose_noL2_L12MU6-B","HLT_2mu6_bBmumux_BcmumuDsloose_noL2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6", "HLT_2mu6_bBmumuxv2_L12MU6-BO", "HLT_2mu6_bBmumuxv2_L12MU6-B", "HLT_2mu6_bBmumuxv2_noL2_L12MU6-BO", "HLT_2mu6_bBmumuxv2_noL2_L12MU6-B", "HLT_2mu6_bBmumuxv2_noL2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6", "HLT_2mu6_bDimu_L1BPH-8M15-2MU6_BPH-0DR22-2MU6", "HLT_2mu6_bDimu_noL2_L12MU6-BO", "HLT_2mu6_bDimu_noL2_L12MU6-B", "HLT_2mu6_bDimu_noL2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6", "HLT_2mu6_bDimu_noL2_L1BPH-8M15-2MU6", "HLT_2mu6_bDimu_noL2_L1BPH-8M15-2MU6_BPH-0DR22-2MU6", "HLT_2mu6_bDimu_novtx_noos_L12MU6-BO", "HLT_2mu6_bDimu_novtx_noos_L12MU6-B", "HLT_2mu6_bDimu_novtx_noos_L1BPH-2M9-2MU6_BPH-2DR15-2MU6", "HLT_2mu6_bDimu_novtx_noos_L1BPH-8M15-2MU6", "HLT_2mu6_bDimu_novtx_noos_L1BPH-8M15-2MU6_BPH-0DR22-2MU6", "HLT_2mu6_bDimu_novtx_noos_noL2_L12MU6-BO", "HLT_2mu6_bDimu_novtx_noos_noL2_L12MU6-B", "HLT_2mu6_bDimu_novtx_noos_noL2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6", "HLT_2mu6_bDimu_novtx_noos_noL2_L1BPH-8M15-2MU6", "HLT_2mu6_bDimu_novtx_noos_noL2_L1BPH-8M15-2MU6_BPH-0DR22-2MU6",
"HLT_2mu6_bJpsimumu_L12MU6-B",
 "HLT_2mu6_bJpsimumu_noL2_L12MU6-BO", "HLT_2mu6_bJpsimumu_noL2_L12MU6-B", "HLT_2mu6_bJpsimumu_noL2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6",
"HLT_2mu6_bUpsimumu_L12MU6-B", "HLT_2mu6_bUpsimumu_L1BPH-8M15-2MU6", "HLT_e0_perf_L1EM3_EMPTY", "HLT_e13_etcut_L1EM12_W-MT25", "HLT_e13_etcut_L1EM12_W-MT30", "HLT_e13_etcut_L1W-NOMATCH", "HLT_e13_etcut_L1W-NOMATCH_W-05RO-XEEMHT", "HLT_e13_etcut_trkcut_L1EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE", "HLT_e13_etcut_trkcut_L1EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20_xe20_mt25", "HLT_e13_etcut_trkcut_j20_perf_xe15_6dphi05_mt25_L1EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_W-90RO2-XEHT-0", "HLT_e13_etcut_trkcut_j20_perf_xe15_6dphi05_mt25_L1EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20", "HLT_e13_etcut_trkcut_xe20_L1EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20", "HLT_e13_etcut_trkcut_xs15_L1EM12_W-MT25_W-15DPHI-JXE-0_W-15DPHI-EMXE_XS20", "HLT_e13_etcut_trkcut_xs30_j10_perf_xe25_6dphi05_mt35_L1EM12_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EMXE", "HLT_e13_etcut_trkcut_xs30_j10_perf_xe25_6dphi15_mt35_L1EM12_W-MT35_W-90RO2-XEHT-0_W-15DPHI-JXE-0_W-15DPHI-EMXE", "HLT_e13_etcut_trkcut_xs30_j10_perf_xe25_6dphi15_mt35_L1EM12_W-MT35_XS30_W-15DPHI-JXE-0_W-15DPHI-EMXE", "HLT_e13_etcut_trkcut_xs40_j10_perf_xe25_6dphi05_mt35_L1EM12_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EMXE", "HLT_e13_etcut_trkcut_xs50_xe30_mt35_L1EM10VH_W-MT35_XS50", "HLT_e13_etcut_trkcut_xs50_xe30_mt35_L1EM12_W-MT35_XS50", "HLT_e14_etcut_e5_lhtight_Jpsiee_L1JPSI-1M5-EM12", "HLT_e14_etcut_e5_lhtight_nod0_Jpsiee_L1JPSI-1M5-EM12", "HLT_e14_etcut_e5_tight_Jpsiee_L1JPSI-1M5-EM12", "HLT_e14_lhtight_e4_etcut_Jpsiee_L1JPSI-1M5-EM12", "HLT_e14_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM12", "HLT_e14_tight_e4_etcut_Jpsiee_L1JPSI-1M5-EM12", "HLT_e17_lhmedium_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25", "HLT_e17_lhmedium_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25", "HLT_e17_lhmedium_ivarloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25", "HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I", "HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25", "HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I", 
"HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_L1EM15-TAU12I", "HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50_L1XE35_EM15-TAU12I", "HLT_e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50_L1XE40_EM15-TAU12I", "HLT_e17_lhmedium_nod0_tau80_medium1_tracktwo_L1EM15-TAU40", "HLT_e17_lhmedium_tau25_medium1_tracktwo_L1EM15-TAU12I", "HLT_e17_lhmedium_tau25_medium1_tracktwo_xe50_L1XE35_EM15-TAU12I", "HLT_e17_lhmedium_tau25_medium1_tracktwo_xe50_L1XE40_EM15-TAU12I", "HLT_e17_lhmedium_tau80_medium1_tracktwo_L1EM15-TAU40", "HLT_e17_medium_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25", "HLT_e17_medium_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25", "HLT_e17_medium_ivarloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25", "HLT_e17_medium_tau25_medium1_tracktwo_L1DR-EM15TAU12I", "HLT_e17_medium_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25", "HLT_e17_medium_tau25_medium1_tracktwo_L1EM15-TAU12I", "HLT_e17_medium_tau25_medium1_tracktwo_L1EM15TAU12I-J25", "HLT_e17_medium_tau25_medium1_tracktwo_xe50_L1XE35_EM15-TAU12I", "HLT_e17_medium_tau25_medium1_tracktwo_xe50_L1XE40_EM15-TAU12I", "HLT_e17_medium_tau80_medium1_tracktwo_L1EM15-TAU40", "HLT_e18_etcut_L1EM15_W-MT35", "HLT_e18_etcut_trkcut_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE", "HLT_e18_etcut_trkcut_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30_xe35_mt35", "HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30", "HLT_e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE", "HLT_e18_etcut_trkcut_xe35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30", "HLT_e18_etcut_trkcut_xs50_j10_perf_xe30_6dphi05_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE", "HLT_e18_etcut_trkcut_xs50_j10_perf_xe30_6dphi05_mt35_L1EM15_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EM15XE", "HLT_e18_etcut_trkcut_xs50_j10_perf_xe30_6dphi15_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-15DPHI-JXE-0_W-15DPHI-EM15XE", "HLT_e18_etcut_trkcut_xs50_j10_perf_xe30_6dphi15_mt35_L1EM15_W-MT35_XS40_W-15DPHI-JXE-0_W-15DPHI-EM15XE", "HLT_e18_etcut_trkcut_xs60_j10_perf_xe30_6dphi05_mt35_L1EM15_W-MT35_XS60_W-05DPHI-JXE-0_W-05DPHI-EM15XE", "HLT_e18_etcut_trkcut_xs60_j10_perf_xe30_6dphi15_mt35_L1EM15_W-MT35_XS60_W-15DPHI-JXE-0_W-15DPHI-EM15XE", "HLT_e18_etcut_trkcut_xs60_xe30_mt35_L1EM15VH_W-MT35_XS60", "HLT_e18_etcut_trkcut_xs60_xe30_mt35_L1EM15_W-MT35_XS60", "HLT_e24_etcut_trkcut_xs60_xe30_mt35_L1EM20VH_W-MT35_XS60", "HLT_e26_etcut_trkcut_xs40_xe30_mt35_L1EM22VHI_W-MT35_XS40", "HLT_e5_etcut_L1W-05DPHI-EMXE-1", "HLT_e5_etcut_L1W-05DPHI-JXE-0", "HLT_e5_etcut_L1W-05RO-XEHT-0", "HLT_e5_etcut_L1W-10DPHI-EMXE-0", "HLT_e5_etcut_L1W-10DPHI-JXE-0", "HLT_e5_etcut_L1W-15DPHI-EMXE-0", "HLT_e5_etcut_L1W-15DPHI-JXE-0", "HLT_e5_etcut_L1W-250RO2-XEHT-0", "HLT_e5_etcut_L1W-90RO2-XEHT-0", "HLT_e5_etcut_L1W-HT20-JJ15.ETA49", "HLT_e5_lhtight_e4_etcut_Jpsiee_L1JPSI-1M5", "HLT_e5_lhtight_e4_etcut_L1JPSI-1M5", "HLT_e5_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5", "HLT_e5_lhtight_nod0_e4_etcut_L1JPSI-1M5", "HLT_e5_tight_e4_etcut_Jpsiee_L1JPSI-1M5", "HLT_e5_tight_e4_etcut_L1JPSI-1M5", "HLT_e9_etcut_e5_lhtight_Jpsiee_L1JPSI-1M5-EM7", "HLT_e9_etcut_e5_lhtight_nod0_Jpsiee_L1JPSI-1M5-EM7", "HLT_e9_etcut_e5_tight_Jpsiee_L1JPSI-1M5-EM7", "HLT_e9_lhtight_e4_etcut_Jpsiee_L1JPSI-1M5-EM7", "HLT_e9_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM7", "HLT_e9_tight_e4_etcut_Jpsiee_L1JPSI-1M5-EM7", "HLT_eb_high_L1RD2_FILLED", "HLT_eb_low_L1RD2_FILLED", "HLT_g0_perf_L1EM3_EMPTY", "HLT_g10_etcut_mu10_L1LFV-EM8I", "HLT_g10_etcut_mu10_iloose_taumass_L1LFV-EM8I", "HLT_g20_etcut_mu4_L1LFV-EM15I", "HLT_g20_etcut_mu4_iloose_taumass_L1LFV-EM15I", "HLT_g20_loose_2j40_0eta490_3j25_0eta490_L1MJJ-700", "HLT_g20_loose_2j40_0eta490_3j25_0eta490_L1MJJ-900", "HLT_g25_medium_L1EM22VHI_2j35_0eta490_bmv2c2077_2j35_0eta490", "HLT_g25_medium_L1EM22VHI_j35_0eta490_bmv2c2077_3j35_0eta490_invm700", "HLT_ht0_L1J12_EMPTY", "HLT_ht1000_L1HT190-J15.ETA21", "HLT_ht400_L1HT150-J20.ETA31", "HLT_ht550_L1HT150-J20.ETA31", "HLT_ht700_L1HT190-J15.ETA21", "HLT_ht850_L1HT190-J15.ETA21", "HLT_j0_L1J12_EMPTY", "HLT_j100_xe80_L1J40_DPHI-CJ20XE50", "HLT_j100_xe80_L1J40_DPHI-J20XE50", "HLT_j100_xe80_L1J40_DPHI-J20s2XE50", "HLT_j100_xe80_L1J40_DPHI-Js2XE50", "HLT_j360_a10_lcw_nojcalib_L1HT150-J20.ETA31", "HLT_j360_a10_lcw_sub_L1HT150-J20.ETA31", "HLT_j360_a10_nojcalib_L1HT150-J20.ETA31", "HLT_j360_a10_sub_L1HT150-J20.ETA31", "HLT_j40_0eta490_xe80_1dphi10_L1DPHI-AJ20s2XE50", "HLT_j460_a10_lcw_nojcalib_L1HT190-J15.ETA21", "HLT_j460_a10_lcw_sub_L1HT190-J15.ETA21", "HLT_j460_a10_nojcalib_L1HT190-J15.ETA21", "HLT_j460_a10_sub_L1HT190-J15.ETA21", "HLT_j80_xe80_1dphi10_L1J40_DPHI-CJ20XE50", "HLT_j80_xe80_1dphi10_L1J40_DPHI-J20XE50", "HLT_j80_xe80_1dphi10_L1J40_DPHI-J20s2XE50", "HLT_j80_xe80_1dphi10_L1J40_DPHI-Js2XE50","HLT_mb_sp1000_pusup450_trk70_hmt_L1TE10", "HLT_mb_sp1000_pusup450_trk70_hmt_L1TE10.0ETA24", "HLT_mb_sp1000_trk70_hmt_L1TE10", "HLT_mb_sp1000_trk70_hmt_L1TE10.0ETA24", "HLT_mb_sp1000_trk70_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_hmtperf_L1TE10", "HLT_mb_sp1200_hmtperf_L1TE10.0ETA24", "HLT_mb_sp1200_hmtperf_L1TE15", "HLT_mb_sp1200_hmtperf_L1TE15.0ETA24", "HLT_mb_sp1200_pusup500_trk100_hmt_L1TE5", "HLT_mb_sp1200_pusup500_trk100_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE10", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE10.0ETA24", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE15", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE15.0ETA24", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_pusup500_trk90_hmt_L1TE5", "HLT_mb_sp1200_pusup500_trk90_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_trk100_hmt_L1TE5", "HLT_mb_sp1200_trk100_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_trk80_hmt_L1TE10", "HLT_mb_sp1200_trk80_hmt_L1TE10.0ETA24", "HLT_mb_sp1200_trk80_hmt_L1TE15", "HLT_mb_sp1200_trk80_hmt_L1TE15.0ETA24", "HLT_mb_sp1200_trk80_hmt_L1TE5.0ETA24", "HLT_mb_sp1200_trk90_hmt_L1TE5", "HLT_mb_sp1200_trk90_hmt_L1TE5.0ETA24", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE15.0ETA24", "HLT_mb_sp1400_trk90_hmt_L1TE15.0ETA24", "HLT_mb_sp1600_hmtperf_L1TE15", "HLT_mb_sp1600_hmtperf_L1TE15.0ETA24", "HLT_mb_sp1600_hmtperf_L1TE20", "HLT_mb_sp1600_hmtperf_L1TE20.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE15", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE15.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE25", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE25.0ETA24", "HLT_mb_sp1600_trk100_hmt_L1TE15.0ETA24", "HLT_mb_sp1600_trk100_hmt_L1TE25", "HLT_mb_sp1600_trk100_hmt_L1TE25.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE15", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE15.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE25", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE25.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE30", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE30.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE5", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE5.0ETA24", "HLT_mb_sp1800_trk110_hmt_L1TE15.0ETA24", "HLT_mb_sp1800_trk110_hmt_L1TE25", "HLT_mb_sp1800_trk110_hmt_L1TE25.0ETA24", "HLT_mb_sp1800_trk110_hmt_L1TE30.0ETA24", "HLT_mb_sp2100_hmtperf_L1TE20.0ETA24", "HLT_mb_sp2100_hmtperf_L1TE25", "HLT_mb_sp2100_hmtperf_L1TE25.0ETA24", "HLT_mb_sp2100_hmtperf_L1TE30.0ETA24", "HLT_mb_sp2100_hmtperf_L1TE40.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE10", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE10.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE15", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE15.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE20.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE25", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE25.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE5", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE5.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE10", "HLT_mb_sp2100_trk120_hmt_L1TE10.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE15", "HLT_mb_sp2100_trk120_hmt_L1TE15.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE20.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE25", "HLT_mb_sp2100_trk120_hmt_L1TE25.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE30.0ETA24", "HLT_mb_sp2100_trk120_hmt_L1TE5", "HLT_mb_sp2100_trk120_hmt_L1TE5.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE10", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE10.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE15", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE15.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE20.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE25", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE25.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE40", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE40.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE5", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE5.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE10", "HLT_mb_sp2300_trk130_hmt_L1TE10.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE15", "HLT_mb_sp2300_trk130_hmt_L1TE15.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE20.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE25", "HLT_mb_sp2300_trk130_hmt_L1TE25.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE30.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE40", "HLT_mb_sp2300_trk130_hmt_L1TE40.0ETA24", "HLT_mb_sp2300_trk130_hmt_L1TE5", "HLT_mb_sp2300_trk130_hmt_L1TE5.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE10", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE10.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE15", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE15.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE20", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE20.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE25", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE25.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE30", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE30.0ETA24", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE40.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE10", "HLT_mb_sp2500_trk140_hmt_L1TE10.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE15", "HLT_mb_sp2500_trk140_hmt_L1TE15.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE20", "HLT_mb_sp2500_trk140_hmt_L1TE20.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE25", "HLT_mb_sp2500_trk140_hmt_L1TE25.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE30", "HLT_mb_sp2500_trk140_hmt_L1TE30.0ETA24", "HLT_mb_sp2500_trk140_hmt_L1TE40.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE20", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE20.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE25", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE25.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE30", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE30.0ETA24", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE40.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE20", "HLT_mb_sp2700_trk150_hmt_L1TE20.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE25", "HLT_mb_sp2700_trk150_hmt_L1TE25.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE30", "HLT_mb_sp2700_trk150_hmt_L1TE30.0ETA24", "HLT_mb_sp2700_trk150_hmt_L1TE40.0ETA24", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE25", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE25.0ETA24", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE30", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE30.0ETA24", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE40", "HLT_mb_sp2900_pusup1300_trk160_hmt_L1TE40.0ETA24", "HLT_mb_sp2900_trk160_hmt_L1TE25", "HLT_mb_sp2900_trk160_hmt_L1TE25.0ETA24", "HLT_mb_sp2900_trk160_hmt_L1TE30", "HLT_mb_sp2900_trk160_hmt_L1TE30.0ETA24", "HLT_mb_sp2900_trk160_hmt_L1TE40", "HLT_mb_sp2900_trk160_hmt_L1TE40.0ETA24", "HLT_mb_sp800_hmtperf_L1TE10", "HLT_mb_sp800_hmtperf_L1TE10.0ETA24", "HLT_mb_sp800_hmtperf_L1TE5", "HLT_mb_sp800_hmtperf_L1TE5.0ETA24", "HLT_mb_sp900_pusup400_trk50_hmt_L1TE5", "HLT_mb_sp900_pusup400_trk50_hmt_L1TE5.0ETA24", "HLT_mb_sp900_trk50_hmt_L1TE5", "HLT_mb_sp900_trk50_hmt_L1TE5.0ETA24", "HLT_mb_sptrk_pt2_L1MBTS_2", "HLT_mb_sptrk_trk80_L1MBTS_2", 

"HLT_mb_sp1200_pusup700_trk70_hmt_L1TE5", "HLT_mb_sp1300_hmtperf_L1TE30", "HLT_mb_sp1300_hmtperf_L1TE40", "HLT_mb_sp1300_hmtperf_L1TE40.0ETA24", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE30", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE40", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE40.0ETA24", "HLT_mb_sp1400_pusup550_trk90_hmt_L1TE50", "HLT_mb_sp1400_pusup800_trk80_hmt_L1TE5", "HLT_mb_sp1400_trk100_hmt_L1TE40", "HLT_mb_sp1400_trk100_hmt_L1TE40.0ETA24", "HLT_mb_sp1400_trk90_hmt_L1TE30", "HLT_mb_sp1400_trk90_hmt_L1TE40", "HLT_mb_sp1400_trk90_hmt_L1TE40.0ETA24", "HLT_mb_sp1500_hmtperf_L1TE30", "HLT_mb_sp1500_hmtperf_L1TE40", "HLT_mb_sp1500_hmtperf_L1TE40.0ETA24", "HLT_mb_sp1500_hmtperf_L1TE50", "HLT_mb_sp1500_pusup700_trk100_hmt_L1TE30", "HLT_mb_sp1500_pusup700_trk100_hmt_L1TE40", "HLT_mb_sp1500_pusup700_trk100_hmt_L1TE40.0ETA24", "HLT_mb_sp1500_pusup700_trk100_hmt_L1TE50", "HLT_mb_sp1500_trk100_hmt_L1TE30", "HLT_mb_sp1500_trk100_hmt_L1TE40", "HLT_mb_sp1500_trk100_hmt_L1TE40.0ETA24", "HLT_mb_sp1600_pusup1100_trk90_hmt_L1TE10", "HLT_mb_sp1600_pusup1100_trk90_hmt_L1TE20", "HLT_mb_sp1600_pusup1100_trk90_hmt_L1TE30", "HLT_mb_sp1600_pusup1100_trk90_hmt_L1TE40", "HLT_mb_sp1600_pusup1100_trk90_hmt_L1TE50", "HLT_mb_sp1700_hmtperf_L1TE30", "HLT_mb_sp1700_hmtperf_L1TE40", "HLT_mb_sp1700_hmtperf_L1TE40.0ETA24", "HLT_mb_sp1800_pusup800_trk110_hmt_L1TE30", "HLT_mb_sp1800_pusup800_trk110_hmt_L1TE40", "HLT_mb_sp1800_pusup800_trk110_hmt_L1TE40.0ETA24", "HLT_mb_sp1800_pusup800_trk110_hmt_L1TE50", "HLT_mb_sp1800_trk110_hmt_L1TE30", "HLT_mb_sp1800_trk110_hmt_L1TE40", "HLT_mb_sp1800_trk110_hmt_L1TE40.0ETA24", "HLT_mb_sp2000_pusup1200_trk100_hmt_L1TE10", "HLT_mb_sp2000_pusup1200_trk100_hmt_L1TE20", "HLT_mb_sp2000_pusup1200_trk100_hmt_L1TE30", "HLT_mb_sp2000_pusup1200_trk100_hmt_L1TE40", "HLT_mb_sp2000_pusup1200_trk100_hmt_L1TE50", "HLT_mb_sp2100_hmtperf_L1TE10", "HLT_mb_sp2100_hmtperf_L1TE20", "HLT_mb_sp2100_hmtperf_L1TE30", "HLT_mb_sp2100_hmtperf_L1TE40", "HLT_mb_sp2100_hmtperf_L1TE50", "HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE30", "HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE40", "HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE50", "HLT_mb_sp2200_pusup1300_trk110_hmt_L1TE20", "HLT_mb_sp2200_pusup1300_trk110_hmt_L1TE30", "HLT_mb_sp2200_pusup1300_trk110_hmt_L1TE40", "HLT_mb_sp2200_pusup1300_trk110_hmt_L1TE50", "HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE30", "HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE40", "HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE50", "HLT_mb_sp2400_pusup1400_trk120_hmt_L1TE20", "HLT_mb_sp2400_pusup1400_trk120_hmt_L1TE30", "HLT_mb_sp2400_pusup1400_trk120_hmt_L1TE40", "HLT_mb_sp2400_pusup1400_trk120_hmt_L1TE50", "HLT_mb_sp2500_hmtperf_L1TE40", "HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE30", "HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE40", "HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE50", "HLT_mb_sp2_hmtperf_L1TE5", "HLT_mb_sp2_hmtperf_L1TE5.0ETA24", "HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE40", "HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE50", "HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE40", "HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE50", "HLT_mb_sp300_trk10_sumet50_hmt_L1TE20", "HLT_mb_sp300_trk10_sumet60_hmt_L1TE40", "HLT_mb_sp600_trk10_sumet50_hmt_L1TE20.0ETA24", "HLT_mb_sp600_trk10_sumet60_hmt_L1TE40.0ETA24", "HLT_mb_sp700_hmtperf_L1TE5", "HLT_mb_sp800_pusup400_trk50_hmt_L1TE5", "HLT_mb_sp900_pusup350_trk60_hmt_L1TE20", "HLT_mb_sp900_pusup350_trk60_hmt_L1TE20.0ETA24", "HLT_mb_sp900_pusup500_trk60_hmt_L1TE5", "HLT_mb_sp900_trk60_hmt_L1TE20", "HLT_mb_sp900_trk60_hmt_L1TE20.0ETA24", "HLT_mb_sptrk_vetombts2in_L1ZDC_AND", "HLT_mb_sptrk_vetombts2in_L1ZDC_A_C", "HLT_mu10_mu6_bBmumux_BcmumuDsloose_L1LFV-MU", "HLT_mu10_mu6_bBmumux_BcmumuDsloose_noL2_L1LFV-MU",
 "HLT_mu10_mucombTag_noEF_L1MU40", "HLT_mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I", "HLT_mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25", "HLT_mu14_iloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25", "HLT_mu15_mucombTag_noEF_L1MU40", "HLT_mu20_mucombTag_noEF_L1MU40", "HLT_mu25_mucombTag_noEF_L1MU40", "HLT_mu4_2j35_dr05_j35_bmedium_split_antimatchdr05mu_L13J15_BTAG-MU4J15", "HLT_mu4_2j35_dr05_j35_bmedium_split_antimatchdr05mu_L13J20_BTAG-MU4J20", "HLT_mu4_2j40_dr05_j40_bloose_split_antimatchdr05mu_L13J15_BTAG-MU4J15", "HLT_mu4_2j40_dr05_j40_bloose_split_antimatchdr05mu_L13J20_BTAG-MU4J20", "HLT_mu4_3j30_dr05_j30_bmedium_split_antimatchdr05mu_L13J15_BTAG-MU4J15", "HLT_mu4_3j30_dr05_j30_bmedium_split_antimatchdr05mu_L13J20_BTAG-MU4J20", "HLT_mu4_3j30_dr05_j30_boffperf_split_antimatchdr05mu_L13J15_BTAG-MU4J15", "HLT_mu4_3j35_dr05_j35_bloose_split_antimatchdr05mu_L13J15_BTAG-MU4J15", "HLT_mu4_3j35_dr05_j35_bloose_split_antimatchdr05mu_L13J20_BTAG-MU4J20", "HLT_mu4_3j35_dr05_j35_boffperf_split_antimatchdr05mu_L13J15_BTAG-MU4J15", "HLT_mu4_3j35_dr05_j35_boffperf_split_antimatchdr05mu_L13J20_BTAG-MU4J20", "HLT_mu4_3j45_dr05_L13J15_BTAG-MU4J15", "HLT_mu4_3j45_dr05_L13J20_BTAG-MU4J20", "HLT_mu4_4j40_dr05_L13J15_BTAG-MU4J15", "HLT_mu4_4j40_dr05_L13J20_BTAG-MU4J20", "HLT_j0_perf_boffperf_L1RD0_EMPTY","HLT_j0_perf_boffperf_L1MU10","HLT_j0_perf_boffperf_L1J12_EMPTY","HLT_mu4_cosmic_L1MU11_EMPTY", "HLT_mu4_cosmic_L1MU4_EMPTY", "HLT_mu4_iloose_mu4_11invm60_noos_L1DY-BOX-2MU4", "HLT_mu4_iloose_mu4_11invm60_noos_novtx_L1DY-BOX-2MU4", "HLT_mu4_iloose_mu4_7invm9_noos_L1DY-BOX-2MU4", "HLT_mu4_iloose_mu4_7invm9_noos_novtx_L1DY-BOX-2MU4",
    'HLT_mu6_ivarloose_mu6_11invm24_noos_L1DY-BOX-2MU6',
    'HLT_mu6_ivarloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6',
    'HLT_mu6_ivarloose_mu6_24invm60_noos_L1DY-BOX-2MU6',
'HLT_mu6_ivarloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6',
 "HLT_mu4_j35_bperf_split_dr05_dz02_L1BTAG-MU4J15", "HLT_mu4_j55_bperf_split_dr05_dz02_L1BTAG-MU4J15", "HLT_mu4_j60_dr05_2j35_L13J15_BTAG-MU4J30", "HLT_mu4_j70_dr05_L1BTAG-MU4J30", "HLT_mu4_msonly_cosmic_L1MU11_EMPTY", "HLT_mu4_msonly_cosmic_L1MU4_EMPTY", "HLT_mu6_j110_bperf_split_dr05_dz02_L1BTAG-MU6J20", "HLT_mu6_j150_bperf_split_dr05_dz02_L1BTAG-MU6J20", "HLT_mu6_j175_bperf_split_dr05_dz02_L1BTAG-MU6J20", "HLT_mu6_j260_bperf_split_dr05_dz02_L1BTAG-MU6J20", "HLT_mu6_j320_bperf_split_dr05_dz02_L1BTAG-MU6J20", "HLT_mu6_j400_bperf_split_dr05_dz02_L1BTAG-MU6J20", "HLT_mu6_j50_dr05_2j35_L13J15_BTAG-MU6J25", "HLT_mu6_j60_dr05_L1BTAG-MU6J25", "HLT_mu6_j85_bperf_split_dr05_dz02_L1BTAG-MU6J20", "HLT_mu6_mu4_bBmumu_L12MU4-BO", "HLT_mu6_mu4_bBmumu_L12MU4-B",
 "HLT_mu6_mu4_bBmumu_noL2_L12MU4-BO", "HLT_mu6_mu4_bBmumu_noL2_L12MU4-B", "HLT_mu6_mu4_bBmumu_noL2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumu_noL2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_L12MU4-BO", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_L12MU4-B", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_noL2_L12MU4-BO", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_noL2_L12MU4-B", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_noL2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumuxv2_L12MU4-BO", "HLT_mu6_mu4_bBmumuxv2_L12MU4-B", "HLT_mu6_mu4_bBmumuxv2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumuxv2_noL2_L12MU4-BO", "HLT_mu6_mu4_bBmumuxv2_noL2_L12MU4-B", "HLT_mu6_mu4_bBmumuxv2_noL2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumuxv2_noL2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bDimu_L12MU4-BO", "HLT_mu6_mu4_bDimu_L12MU4-B", "HLT_mu6_mu4_bDimu_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bDimu_L1BPH-8M15-MU6MU4", "HLT_mu6_mu4_bDimu_noL2_L12MU4-BO", "HLT_mu6_mu4_bDimu_noL2_L12MU4-B", "HLT_mu6_mu4_bDimu_noL2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bDimu_noL2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bDimu_noL2_L1BPH-8M15-MU6MU4", "HLT_mu6_mu4_bDimu_noL2_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4", "HLT_mu6_mu4_bDimu_novtx_noos_L12MU4-BO", "HLT_mu6_mu4_bDimu_novtx_noos_L12MU4-B", "HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bDimu_novtx_noos_L1BPH-8M15-MU6MU4", "HLT_mu6_mu4_bDimu_novtx_noos_noL2_L12MU4-BO", "HLT_mu6_mu4_bDimu_novtx_noos_noL2_L12MU4-B", "HLT_mu6_mu4_bDimu_novtx_noos_noL2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bDimu_novtx_noos_noL2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bDimu_novtx_noos_noL2_L1BPH-8M15-MU6MU4", "HLT_mu6_mu4_bDimu_novtx_noos_noL2_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4", "HLT_mu6_mu4_bJpsimumu_L12MU4-BO", "HLT_mu6_mu4_bJpsimumu_L12MU4-B", "HLT_mu6_mu4_bJpsimumu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4",
 "HLT_mu6_mu4_bJpsimumu_noL2_L12MU4-BO", "HLT_mu6_mu4_bJpsimumu_noL2_L12MU4-B", "HLT_mu6_mu4_bJpsimumu_noL2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bJpsimumu_noL2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bUpsimumu_L12MU4-BO", "HLT_mu6_mu4_bUpsimumu_L12MU4-B", "HLT_mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4", "HLT_mu8_mucombTag_noEF_L1MU40", "HLT_noalg_L1Topo", "HLT_tau1_cosmic_ptonly_L1MU4_EMPTY", "HLT_tau1_cosmic_track_L1MU4_EMPTY", "HLT_tau35_loose1_tracktwo_tau25_loose1_tracktwo_L1DR-TAU20ITAU12I", "HLT_tau35_loose1_tracktwo_tau25_loose1_tracktwo_L1DR-TAU20ITAU12I-J25", "HLT_tau35_loose1_tracktwo_tau25_loose1_tracktwo_L1TAU20ITAU12I-J25", "HLT_tau35_medium1_tracktwo_L1TAU20_xe70_L1XE45_L1XE45_TAU20-J20", "HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1BOX-TAU20ITAU12I", "HLT_tau8_cosmic_ptonly", "HLT_tau8_cosmic_track", "HLT_zdcpeb_L1ZDC_A", "HLT_zdcpeb_L1ZDC_AND", "HLT_zdcpeb_L1ZDC_A_C", "HLT_zdcpeb_L1ZDC_C", "HLT_mu6_mu4_bBmumuxv2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4", "HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4", "HLT_2mu4_bBmumuxv2_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", "HLT_2mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-2MU4_BPH-0DR15-2MU4", 
'HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25', "HLT_mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I", 
                'HLT_mu4_iloose_mu4_11invm60_noos_L1MU6_2MU4','HLT_mu4_ivarloose_mu4_11invm60_noos_L1MU6_2MU4',
                'HLT_mu4_iloose_mu4_7invm9_noos_novtx_L1MU6_2MU4', 'HLT_mu4_ivarloose_mu4_7invm9_noos_novtx_L1MU6_2MU4',
                'HLT_mu4_iloose_mu4_11invm60_noos_novtx_L1MU6_2MU4','HLT_mu4_ivarloose_mu4_11invm60_noos_novtx_L1MU6_2MU4', 
                'HLT_mu4_iloose_mu4_7invm9_noos_L1MU6_2MU4','HLT_mu4_ivarloose_mu4_7invm9_noos_L1MU6_2MU4',
               "HLT_mu20_ivarmedium_L1MU15",
              "HLT_mb_sp1000_pusup450_trk70_hmt_L1TE5", "HLT_mb_sp1000_trk70_hmt_L1TE5", "HLT_mb_sp1200_pusup500_trk80_hmt_L1TE5", "HLT_mb_sp1200_trk80_hmt_L1TE5", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE10", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE20", "HLT_mb_sp1800_hmtperf_L1TE5", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE10", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE20", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE20", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE30", "HLT_mb_sp2100_trk120_hmt_L1TE20", "HLT_mb_sp2100_trk120_hmt_L1TE30", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE20", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE30", "HLT_mb_sp2300_trk130_hmt_L1TE20", "HLT_mb_sp2300_trk130_hmt_L1TE30", "HLT_mb_sp2500_pusup1100_trk140_hmt_L1TE40", "HLT_mb_sp2500_trk140_hmt_L1TE40", "HLT_mb_sp2700_pusup1200_trk150_hmt_L1TE40", "HLT_mb_sp2700_trk150_hmt_L1TE40", "HLT_mb_sp900_pusup400_trk60_hmt_L1TE5",
"HLT_mb_sp1800_pusup700_trk110_hmt_L1TE20.0ETA24", "HLT_mb_sp2100_pusup900_trk120_hmt_L1TE30.0ETA24", "HLT_mb_sp2300_pusup1000_trk130_hmt_L1TE30.0ETA24", "HLT_mb_sp1600_pusup600_trk100_hmt_L1TE10.0ETA24", "HLT_mb_sp1800_pusup700_trk110_hmt_L1TE10.0ETA24", 
'HLT_2mu4_bDimu', 'HLT_2mu4_bDimu_noinvm_novtx_ss', 'HLT_2mu4_bDimu_novtx_noos',
'HLT_j55_bmv2c2050_ht500_L14J15', 'HLT_j175_bmv2c2060_split_j60_bmv2c2060_split',  'HLT_j55_bmv2c2060_ht500_L14J15',
    "HLT_2j45_bmv2c2085_split_3j45_L14J15.0ETA25",
"HLT_2mu6_bBmumux_Taumumux_noL2",
'HLT_g10_etcut_L1EM8I_mu10_taumass', 'HLT_g20_etcut_L1EM15I_mu4_taumass',
    'HLT_e28_lhtight_smooth_iloose', 'HLT_e28_lhtight_nod0_iloose',
'HLT_e26_lhtight_smooth_iloose', 'HLT_e26_lhtight_nod0_iloose',
'HLT_e28_lhtight_nod0_L1EM22VHI', 'HLT_e28_lhtight_nod0_ringer_iloose',
'HLT_e24_lhtight_cutd0dphideta_iloose', 'HLT_e60_lhmedium_cutd0dphideta','HLT_2e17_lhloose_cutd0dphideta',
'HLT_e17_lhmedium_nod0_e12_lhmedium_nod0_e9_lhmedium_nod0_L1EM15VH_2EM10VH_3EM7',
'HLT_e17_lhloose_nod0_e12_lhloose_nod0_e9_lhloose_nod0_L1EM15VH_2EM10VH_3EM7',
"HLT_e20_lhloose_nod0_e12_lhloose_nod0_e9_lhloose_nod0_L1EM18VH_2EM10VH_3EM7", 
"HLT_e20_lhloose_nod0_e15_lhloose_nod0_e10_lhloose_nod0_L1EM18VH_2EM10VH_3EM7", 

'HLT_mu10_mu6_bBmumux_BsmumuPhi', 'HLT_mu4_j100_xe80_mht',
'HLT_mu10_mu6_bBmumux_BcmumuDsloose_delayed_L1LFV-MU',
'HLT_2mu4_j80_xe40_mht',
'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15HI_2TAU12IM',
"HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25",
"HLT_g60_loose_L1EM24VHI_xe60noL1",
"HLT_g70_loose_L1EM24VHI_xe70noL1",
"HLT_g60_loose_L1EM26VHI_xe60noL1",
"HLT_g70_loose_L1EM26VHI_xe70noL1",
"HLT_g20_etcut_L1EM15_mu4_iloose_taumass",
"HLT_g20_loose_L1EM15_mu4_ivarloose_taumass", 
]
        
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Undefined'), 
    }])))

HLT_list=[
'HLT_j30_jes_cleanLLP_PS_llp_L1TAU8_UNPAIRED_NONISO',  
'HLT_j30_jes_cleanLLP_PS_llp_noiso_L1TAU8_UNPAIRED_NONISO',  
'HLT_j30_muvtx_L1MU4_UNPAIRED_NONISO',
'HLT_3mu6_msonly_L1MU4_UNPAIRED_NONISO',  
'HLT_mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_NONISO',
'HLT_mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_NONISO',
'HLT_g15_loose_2mu10_msonly_L1MU4_UNPAIRED_NONISO',  
'HLT_j30_muvtx_noiso_L1MU4_UNPAIRED_NONISO',  
'HLT_2g50_tight_L1EM7_UNPAIRED_NONISO',  
'HLT_2g22_tight_L1EM7_UNPAIRED_NONISO',  
]

disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled b-phys delayed and exotics'), 
    }])))


HLT_list=[
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
'HLT_calibAFP_L1AFP_FSC',
'HLT_calibAFP_L1AFP_NSC',
"HLT_calibAFP_L1AFP_C_MBTS_A", "HLT_mb_sp_L1AFP_C_AND", "HLT_mb_sp_L1AFP_C_ANY_MBTS_A", "HLT_mb_sp_L1AFP_C_MBTS_A", "HLT_mb_sp_vetosp_L1AFP_C_AND", "HLT_mb_sp_vetospmbts2in_L1AFP_C_AND", "HLT_mu4_L1AFP_C_MU4", "HLT_mu6_L1AFP_C_MU4", "HLT_noalg_L1AFP_C_MU4",
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
"HLT_noalg_l1topo_L1DR-MU10TAU12I_TAU12I-J25",
"HLT_noalg_l1topo_L1MU10_TAU12I-J25",
"HLT_noalg_l1topo_L1XE45_TAU20-J20",
"HLT_noalg_l1topo_L1XE35_EM15-TAU12I",
"HLT_noalg_l1topo_L1XE40_EM15-TAU12I",
"HLT_noalg_l1topo_L1BPH-2M8-2MU4",
"HLT_noalg_l1topo_L1BPH-8M15-MU6MU4",
"HLT_noalg_l1topo_L1BPH-8M15-2MU6",
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
"HLT_noalg_l1topo_L1HT150-J20.ETA31",
"HLT_noalg_l1topo_L1HT150-J20s5.ETA31",
"HLT_noalg_l1topo_L1JPSI-1M5",
"HLT_noalg_l1topo_L1JPSI-1M5-EM7",
"HLT_noalg_l1topo_L1JPSI-1M5-EM12",
"HLT_noalg_l1topo_L1KF-XE35",
"HLT_noalg_l1topo_L1KF-XE45",
"HLT_noalg_l1topo_L1KF-XE55",
"HLT_noalg_l1topo_L1KF-XE60",
"HLT_noalg_l1topo_L1KF-XE65",
"HLT_noalg_l1topo_L1KF-XE75",
"HLT_noalg_l1topo_L1EM12_W-MT25",
"HLT_noalg_l1topo_L1EM12_W-MT30",
"HLT_noalg_l1topo_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE",
"HLT_noalg_l1topo_L1W-05RO-XEHT-0",
"HLT_noalg_l1topo_L1W-90RO2-XEHT-0",
"HLT_noalg_l1topo_L1W-250RO2-XEHT-0",
"HLT_noalg_l1topo_L1W-HT20-JJ15.ETA49",
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
"HLT_noalg_l1topo_L1HT150-JJ15.ETA49_MJJ-400",
"HLT_noalg_l1topo_L1BPH-1M19-2MU4_BPH-0DR34-2MU4",
"HLT_noalg_l1topo_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4",
"HLT_noalg_l1topo_L1BPH-2M9-2MU6_BPH-2DR15-2MU6",
"HLT_noalg_l1topo_L1MU6MU4-BO",
"HLT_noalg_l1topo_L12MU4-B",
"HLT_noalg_l1topo_L12MU6-B",
"HLT_noalg_l1topo_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4",
"HLT_noalg_l1topo_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4",
"HLT_noalg_l1topo_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4",
"HLT_noalg_l1topo_L12MU4-BO",
"HLT_noalg_l1topo_L12MU6-BO",
"HLT_noalg_l1topo_L1MU6_2MU4-B",
"HLT_noalg_l1topo_L1DY-DR-2MU4",
#"HLT_noalg_l1topo_L1DY-BOX-2MU4",
"HLT_noalg_l1topo_L1DY-BOX-2MU6",
"HLT_noalg_l1topo_L1LFV-MU",
"HLT_noalg_l1topo_L1LFV-EM8I",
"HLT_noalg_l1topo_L1LFV-EM15I",
"HLT_noalg_l1topo_L1DPHI-Js2XE50",
"HLT_noalg_l1topo_L1DPHI-J20s2XE50",
"HLT_noalg_l1topo_L1DPHI-J20XE50",
"HLT_noalg_l1topo_L1DPHI-CJ20XE50",
"HLT_noalg_l1topo_L1MJJ-900",
"HLT_noalg_l1topo_L1MJJ-800",
"HLT_noalg_l1topo_L1MJJ-700",
"HLT_noalg_l1topo_L1MJJ-400",
"HLT_noalg_l1topo_L1MJJ-100",
"HLT_noalg_l1topo_L1HT150-JJ15.ETA49",

"HLT_noalg_l1topo_L1J4-MATCH",
"HLT_noalg_l1topo_L1LLP-NOMATCH",
"HLT_noalg_l1topo_L1DR-MU10TAU12I",
"HLT_noalg_l1topo_L1EM15-TAU40",
"HLT_noalg_l1topo_L1EM15-TAU12I",
"HLT_noalg_l1topo_L1EM15TAU12I-J25",
"HLT_noalg_l1topo_L1DR-EM15TAU12I-J25",
"HLT_noalg_l1topo_L1TAU20ITAU12I-J25",
"HLT_noalg_l1topo_L1DR-TAU20ITAU12I",
"HLT_noalg_l1topo_L1BOX-TAU20ITAU12I",
"HLT_noalg_l1topo_L1DR-TAU20ITAU12I-J25",
#"HLT_noalg_l1topo_L1LAR-EM",
#"HLT_noalg_l1topo_L1LAR-J",
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

 'HLT_noalg_L12EM15', 'HLT_noalg_L12EM3', 'HLT_noalg_L12EM7', 'HLT_noalg_L1ALFA_Diff', 'HLT_noalg_L1ALFA_Phys', 'HLT_noalg_L1ALFA_PhysAny', 'HLT_noalg_L1ALFA_SYS', 'HLT_noalg_L1All', 'HLT_noalg_L1CALREQ2', 'HLT_noalg_L1Calo', 'HLT_noalg_L1Calo_EMPTY', 'HLT_noalg_L1EM12', 'HLT_noalg_L1EM15', 'HLT_noalg_L1EM15VH_3EM7', 'HLT_noalg_L1EM18VH', 'HLT_noalg_L1EM20VH', 'HLT_noalg_L1EM3', 'HLT_noalg_L1EM7', 'HLT_noalg_L1J20_J20.31ETA49', 'HLT_noalg_L1J50', 'HLT_noalg_L1J75', 'HLT_noalg_L1J75.31ETA49', 'HLT_noalg_L1LHCF', 'HLT_noalg_L1LHCF_EMPTY', 'HLT_noalg_L1LHCF_UNPAIRED_ISO', 'HLT_noalg_L1LowLumi', 'HLT_noalg_L1MBTS_1_BGRP11', 'HLT_noalg_L1MBTS_1_BGRP9', 'HLT_noalg_L1MBTS_2_BGRP11', 'HLT_noalg_L1MBTS_2_BGRP9', 'HLT_noalg_L1MJJ-100', 'HLT_noalg_L1MU4_J12', 'HLT_noalg_L1MU6_J20', 'HLT_noalg_L1RD0_BGRP11', 'HLT_noalg_L1RD0_BGRP9', 'HLT_noalg_L1Standby', 'HLT_noalg_L1TAU12', 'HLT_noalg_L1TAU12IL', 'HLT_noalg_L1TAU12IM', 'HLT_noalg_L1TAU12IT', 'HLT_noalg_L1TAU20', 'HLT_noalg_L1TAU20IL', 'HLT_noalg_L1TAU20IM', 'HLT_noalg_L1TAU20IM_2TAU12IM', 'HLT_noalg_L1TAU20IT', 'HLT_noalg_L1TAU20_2TAU12', 'HLT_noalg_L1TAU30', 'HLT_noalg_L1TAU60', 'HLT_noalg_L1XE50', 'HLT_noalg_bkg_L1J12', 'HLT_noalg_bkg_L1J30.31ETA49', 'HLT_noalg_cosmiccalo_L1J12_FIRSTEMPTY', 'HLT_noalg_cosmiccalo_L1J30_FIRSTEMPTY', 'HLT_noalg_cosmicmuons_L1MU11_EMPTY', 'HLT_noalg_cosmicmuons_L1MU4_EMPTY', 'HLT_noalg_eb_L1ABORTGAPNOTCALIB_noPS', 'HLT_noalg_eb_L1EMPTY_noPS', 'HLT_noalg_eb_L1FIRSTEMPTY_noPS', 'HLT_noalg_eb_L1PhysicsHigh_noPS', 'HLT_noalg_eb_L1PhysicsLow_noPS', 'HLT_noalg_eb_L1RD3_EMPTY', 'HLT_noalg_eb_L1RD3_FILLED', 'HLT_noalg_eb_L1UNPAIRED_ISO_noPS', 'HLT_noalg_eb_L1UNPAIRED_NONISO_noPS', 'HLT_noalg_idcosmic_L1TRT_EMPTY', 'HLT_noalg_idcosmic_L1TRT_FILLED', 'HLT_noalg_L1ALFA_ANY', 'HLT_noalg_L1BPH-2M-2MU4', 'HLT_noalg_L1BPH-2M-2MU6', 'HLT_noalg_L1BPH-2M-MU6MU4', 'HLT_noalg_L1BPH-4M8-2MU4', 'HLT_noalg_L1BPH-4M8-2MU6', 'HLT_noalg_L1BPH-4M8-MU6MU4', 'HLT_noalg_L1BPH-DR-2MU4', 'HLT_noalg_L1BPH-DR-2MU6', 'HLT_noalg_L1BPH-DR-MU6MU4', 'HLT_noalg_L1DR-MU10TAU12I', 'HLT_noalg_L1DR-TAU20ITAU12I', 'HLT_noalg_L1DY-BOX-2MU4', 'HLT_noalg_L1DY-BOX-2MU6', 'HLT_noalg_L1DY-BOX-MU6MU4', 'HLT_noalg_L1DY-DR-2MU4', 'HLT_noalg_L1EM10', 'HLT_noalg_L1EM10VH', 'HLT_noalg_L1EM13VH', 'HLT_noalg_L1EM20VHI', 'HLT_noalg_L1EM22VHI', 'HLT_noalg_L1EM8VH', 'HLT_noalg_L1J100', 'HLT_noalg_L1J12', 'HLT_noalg_L1J120', 'HLT_noalg_L1J15', 'HLT_noalg_L1J20', 'HLT_noalg_L1J25', 'HLT_noalg_L1J30', 'HLT_noalg_L1J40', 'HLT_noalg_L1J85', 'HLT_noalg_L1LFV-MU', 'HLT_noalg_L1TAU40', 'HLT_noalg_L1TAU8', 'HLT_noalg_L1TE10', 'HLT_noalg_L1TE10.0ETA24', 'HLT_noalg_L1TE20', 'HLT_noalg_L1TE20.0ETA24', 'HLT_noalg_L1TE30', 'HLT_noalg_L1TE30.0ETA24', 'HLT_noalg_L1TE40', 'HLT_noalg_L1TE50', 'HLT_noalg_L1XE55', 'HLT_noalg_L1XE60', 'HLT_noalg_L1XE70', 'HLT_noalg_L1XE80', 'HLT_noalg_L1XS20', 'HLT_noalg_L1XS30', 'HLT_noalg_L1XS40', 'HLT_noalg_L1XS50', 'HLT_noalg_L1XS60', 'HLT_noalg_L1J100.31ETA49', 'HLT_noalg_L1J15.31ETA49', 'HLT_noalg_L1J20.28ETA31', 'HLT_noalg_L1J20.31ETA49', 'HLT_noalg_L1J30.31ETA49', 'HLT_noalg_L1J50.31ETA49', 'HLT_noalg_L1RD1_FILLED', 'HLT_noalg_L1RD2_FILLED', 'HLT_noalg_L1RD2_EMPTY', 'HLT_noalg_L1RD3_FILLED', 'HLT_noalg_L1RD3_EMPTY', 'HLT_noalg_standby_L1RD0_FILLED', 'HLT_noalg_standby_L1RD0_EMPTY', "HLT_noalg_L1TE40.0ETA24", "HLT_noalg_L1TE50.0ETA24", "HLT_noalg_L1TE70", "HLT_noalg_mb_L1TE50.0ETA24", "HLT_noalg_mb_L1TE70", "HLT_noalg_mb_L1TE70.0ETA24", "HLT_noalg_mb_L1ZDC_A", "HLT_noalg_mb_L1ZDC_AND", "HLT_noalg_mb_L1ZDC_A_C", "HLT_noalg_mb_L1ZDC_C",
    ]

disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled Streamer'), 
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
'HLT_alfacalib_L1ALFA_A7L1_OD', 'HLT_alfacalib_L1ALFA_A7R1_OD', 'HLT_alfacalib_L1ALFA_B7L1_OD', 'HLT_alfacalib_L1ALFA_B7R1_OD', 'HLT_alfacalib_L1ALFA_ELAS', 'HLT_alfacalib_L1ALFA_ELAST15', 'HLT_alfacalib_L1ALFA_ELAST18', 'HLT_alfacalib_L1ALFA_SYS', 'HLT_ibllumi_L1RD0_ABORTGAPNOTCALIB', 'HLT_ibllumi_L1RD0_FILLED', 'HLT_ibllumi_L1RD0_UNPAIRED_ISO', 'HLT_l1calocalib', 'HLT_l1calocalib_L1BGRP9', 'HLT_larnoiseburst_L1All', 'HLT_larnoiseburst_loose_L1All', 'HLT_larnoiseburst_loose_rerun', 'HLT_lumipeb_vdm_L1MBTS_1', 'HLT_lumipeb_vdm_L1MBTS_1_BGRP11', 'HLT_lumipeb_vdm_L1MBTS_1_BGRP9', 'HLT_lumipeb_vdm_L1MBTS_1_UNPAIRED_ISO', 'HLT_lumipeb_vdm_L1MBTS_2', 'HLT_lumipeb_vdm_L1MBTS_2_BGRP11', 'HLT_lumipeb_vdm_L1MBTS_2_BGRP9', 'HLT_lumipeb_vdm_L1MBTS_2_UNPAIRED_ISO', 'HLT_lumipeb_vdm_L1RD0_BGRP11', 'HLT_lumipeb_vdm_L1RD0_BGRP9', 'HLT_lumipeb_vdm_L1RD0_FILLED', 'HLT_lumipeb_vdm_L1RD0_UNPAIRED_ISO',]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled calib'), 
    }])))

# --------------------------------------
# minbias

HLT_list=['HLT_mb_sp_L1RD0_UNPAIRED_ISO','HLT_noalg_bkg_L1MBTS_4_A_UNPAIRED_ISO','HLT_noalg_bkg_L1MBTS_4_C_UNPAIRED_ISO','HLT_mb_mbts_L1MBTS_1', 'HLT_mb_mbts_L1MBTS_1_1', 'HLT_mb_mbts_L1MBTS_1_1_EMPTY', 'HLT_mb_mbts_L1MBTS_1_1_UNPAIRED_ISO', 'HLT_mb_mbts_L1MBTS_1_EMPTY', 'HLT_mb_mbts_L1MBTS_1_UNPAIRED_ISO', 'HLT_mb_mbts_L1MBTS_2', 'HLT_mb_mbts_L1MBTS_2_EMPTY', 'HLT_mb_mbts_L1MBTS_2_UNPAIRED_ISO', 'HLT_mb_perf_L1LUCID', 'HLT_mb_perf_L1LUCID_EMPTY', 'HLT_mb_perf_L1LUCID_UNPAIRED_ISO', 'HLT_mb_perf_L1MBTS_2', 'HLT_mb_perf_L1RD1_FILLED', 'HLT_mb_sp1000_trk70_hmt_L1MBTS_1_1', 'HLT_mb_sp1200_trk75_hmt_L1MBTS_1_1', 'HLT_mb_sp1400_hmtperf_L1TE10', 'HLT_mb_sp1400_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1400_hmtperf_L1TE20', 'HLT_mb_sp1400_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1400_trk100_hmt_L1TE20', 'HLT_mb_sp1400_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1400_trk80_hmt_L1MBTS_1_1', 'HLT_mb_sp1400_trk90_hmt_L1TE10', 'HLT_mb_sp1400_trk90_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_hmtperf_L1TE20', 'HLT_mb_sp2000_pusup600_trk70_hmt', 'HLT_mb_sp2000_pusup600_trk70_hmt_L1TE30', 'HLT_mb_sp2000_pusup600_trk70_hmt_L1TE40', 'HLT_mb_sp2000_pusup700_trk50_sumet110_hmt_L1TE40', 'HLT_mb_sp2000_pusup700_trk50_sumet150_hmt_L1TE50', 'HLT_mb_sp2000_pusup700_trk50_sumet70_hmt_L1TE30', 'HLT_mb_sp2000_pusup700_trk60_hmt_L1TE30', 'HLT_mb_sp2000_pusup700_trk70_hmt_L1TE30', 'HLT_mb_sp2000_pusup700_trk70_hmt_L1TE40', 'HLT_mb_sp2000_trk70_hmt', 'HLT_mb_sp2500_hmtperf_L1TE20', 'HLT_mb_sp2500_pusup750_trk90_hmt_L1TE40', 'HLT_mb_sp2_hmtperf', 'HLT_mb_sp2_hmtperf_L1MBTS_1_1', 'HLT_mb_sp3000_pusup800_trk120_hmt_L1TE50', 'HLT_mb_sp300_trk10_sumet40_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet50_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet50_hmt_L1RD3_FILLED', 'HLT_mb_sp300_trk10_sumet50_hmt_L1TE10', 'HLT_mb_sp300_trk10_sumet60_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet60_hmt_L1TE20', 'HLT_mb_sp300_trk10_sumet70_hmt_L1MBTS_1_1', 'HLT_mb_sp300_trk10_sumet80_hmt_L1MBTS_1_1', 'HLT_mb_sp400_trk40_hmt_L1MBTS_1_1', 'HLT_mb_sp500_hmtperf', 'HLT_mb_sp600_trk10_sumet40_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet50_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet50_hmt_L1RD3_FILLED', 'HLT_mb_sp600_trk10_sumet50_hmt_L1TE10.0ETA24', 'HLT_mb_sp600_trk10_sumet60_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet60_hmt_L1TE20.0ETA24', 'HLT_mb_sp600_trk10_sumet70_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk10_sumet80_hmt_L1MBTS_1_1', 'HLT_mb_sp600_trk45_hmt_L1MBTS_1_1', 'HLT_mb_sp700_trk50_hmt_L1MBTS_1_1', 'HLT_mb_sp700_trk50_hmt_L1RD3_FILLED', 'HLT_mb_sp700_trk55_hmt_L1MBTS_1_1', 'HLT_mb_sp900_trk60_hmt_L1MBTS_1_1', 'HLT_mb_sp900_trk65_hmt_L1MBTS_1_1','HLT_mb_sptrk_L1RD0_EMPTY', 'HLT_mb_sptrk_L1RD0_UNPAIRED_ISO', 'HLT_mb_sptrk_L1RD3_FILLED', 'HLT_mb_sptrk_costr', 'HLT_mb_sptrk_costr_L1RD0_EMPTY', 'HLT_mb_sptrk_noisesup', 'HLT_mb_sptrk_noisesup_L1RD0_EMPTY', 'HLT_mb_sptrk_noisesup_L1RD0_UNPAIRED_ISO', 'HLT_mb_sptrk_noisesup_L1RD3_FILLED', 'HLT_mb_sptrk_pt4_L1MBTS_1', 'HLT_mb_sptrk_pt4_L1MBTS_1_1', 'HLT_mb_sptrk_pt4_L1MBTS_2', 'HLT_mb_sptrk_pt4_L1RD3_FILLED', 'HLT_mb_sptrk_pt6_L1MBTS_1', 'HLT_mb_sptrk_pt6_L1MBTS_1_1', 'HLT_mb_sptrk_pt6_L1MBTS_2', 'HLT_mb_sptrk_pt6_L1RD3_FILLED', 'HLT_mb_sptrk_pt8_L1MBTS_1', 'HLT_mb_sptrk_pt8_L1MBTS_1_1', 'HLT_mb_sptrk_pt8_L1MBTS_2', 'HLT_mb_sptrk_pt8_L1RD3_FILLED', 'HLT_mb_sptrk_vetombts2in_L1ALFA_CEP', 'HLT_mb_sptrk_vetombts2in_peb_L1ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_peb_L1ALFA_ANY_UNPAIRED_ISO', 'HLT_noalg_mb_L1LUCID', 'HLT_noalg_mb_L1LUCID_EMPTY', 'HLT_noalg_mb_L1LUCID_UNPAIRED_ISO', 'HLT_noalg_mb_L1MBTS_1', 'HLT_noalg_mb_L1MBTS_1_1', 'HLT_noalg_mb_L1MBTS_1_1_EMPTY', 'HLT_noalg_mb_L1MBTS_1_1_UNPAIRED_ISO', 'HLT_noalg_mb_L1MBTS_1_EMPTY', 'HLT_noalg_mb_L1MBTS_1_UNPAIRED_ISO', 'HLT_noalg_mb_L1MBTS_2_EMPTY', 'HLT_noalg_mb_L1MBTS_2_UNPAIRED_ISO', 'HLT_noalg_mb_L1RD0_EMPTY', 'HLT_noalg_mb_L1RD0_FILLED', 'HLT_noalg_mb_L1RD0_UNPAIRED_ISO', 'HLT_noalg_mb_L1RD1_FILLED', 'HLT_noalg_mb_L1RD2_EMPTY', 'HLT_noalg_mb_L1RD2_FILLED', 'HLT_noalg_mb_L1RD3_EMPTY', 'HLT_noalg_mb_L1RD3_FILLED', 'HLT_noalg_mb_L1TE10', 'HLT_noalg_mb_L1TE10.0ETA24', 'HLT_noalg_mb_L1TE20', 'HLT_noalg_mb_L1TE20.0ETA24', 'HLT_noalg_mb_L1TE30', 'HLT_noalg_mb_L1TE40', 'HLT_noalg_mb_L1TE50','HLT_mb_sp1800_hmtperf_L1TE30.0ETA24','HLT_noalg_mb_L1TE30.0ETA24','HLT_noalg_L1TE0', 'HLT_noalg_L1TE0.0ETA24', 'HLT_noalg_L1TE15', 'HLT_noalg_L1TE5', 'HLT_noalg_L1TE5.0ETA24', 'HLT_noalg_mb_L1TE0', 'HLT_noalg_mb_L1TE0.0ETA24', 'HLT_noalg_mb_L1TE15', 'HLT_noalg_mb_L1TE5', 'HLT_noalg_mb_L1TE5.0ETA24', 'HLT_mb_sp1300_hmtperf_L1TE10', 'HLT_mb_sp1300_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1300_hmtperf_L1TE15', 'HLT_mb_sp1300_hmtperf_L1TE20', 'HLT_mb_sp1300_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1300_hmtperf_L1TE5', 'HLT_mb_sp1300_hmtperf_L1TE5.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE10', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE10.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE15', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE20', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE20.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE5', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE5.0ETA24', 'HLT_mb_sp1400_trk90_hmt_L1TE15', 'HLT_mb_sp1400_trk90_hmt_L1TE20', 'HLT_mb_sp1400_trk90_hmt_L1TE20.0ETA24', 'HLT_mb_sp1400_trk90_hmt_L1TE5', 'HLT_mb_sp1400_trk90_hmt_L1TE5.0ETA24', 'HLT_mb_sp1500_hmtperf_L1TE10', 'HLT_mb_sp1500_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1500_hmtperf_L1TE15', 'HLT_mb_sp1500_hmtperf_L1TE20', 'HLT_mb_sp1500_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1500_hmtperf_L1TE5', 'HLT_mb_sp1500_hmtperf_L1TE5.0ETA24', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE10', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE15', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE20', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE5', 'HLT_mb_sp1600_pusup550_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1600_trk100_hmt_L1TE10', 'HLT_mb_sp1600_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1600_trk100_hmt_L1TE15', 'HLT_mb_sp1600_trk100_hmt_L1TE20', 'HLT_mb_sp1600_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1600_trk100_hmt_L1TE5', 'HLT_mb_sp1600_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1700_hmtperf_L1TE10', 'HLT_mb_sp1700_hmtperf_L1TE10.0ETA24', 'HLT_mb_sp1700_hmtperf_L1TE15', 'HLT_mb_sp1700_hmtperf_L1TE20', 'HLT_mb_sp1700_hmtperf_L1TE20.0ETA24', 'HLT_mb_sp1700_hmtperf_L1TE5', 'HLT_mb_sp1700_hmtperf_L1TE5.0ETA24', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE10', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE15', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE20', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE20.0ETA24', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE5', 'HLT_mb_sp1800_pusup600_trk110_hmt_L1TE5.0ETA24', 'HLT_mb_sp1800_trk110_hmt_L1TE10', 'HLT_mb_sp1800_trk110_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_trk110_hmt_L1TE15', 'HLT_mb_sp1800_trk110_hmt_L1TE20', 'HLT_mb_sp1800_trk110_hmt_L1TE20.0ETA24', 'HLT_mb_sp1800_trk110_hmt_L1TE5', 'HLT_mb_sp1800_trk110_hmt_L1TE5.0ETA24', 'HLT_mb_sp2_hmtperf_L1TE0', 'HLT_mb_sp2_hmtperf_L1TE0.0ETA24', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE10', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE10.0ETA24', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE5', 'HLT_mb_sp900_pusup350_trk60_hmt_L1TE5.0ETA24', 'HLT_mb_sp900_trk60_hmt_L1TE10', 'HLT_mb_sp900_trk60_hmt_L1TE10.0ETA24', 'HLT_mb_sp900_trk60_hmt_L1TE5', 'HLT_mb_sp900_trk60_hmt_L1TE5.0ETA24', 'HLT_lumipeb_L1MBTS_2', 'HLT_mb_sptrk_vetombts2in_L1TRT_ALFA_EINE', 'HLT_mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY_UNPAIRED_ISO','mb_sptrk_vetombts2in_L1TRT_ALFA_EINE','mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY','mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY_UNPAIRED_ISO','lumipeb_L1MBTS_2', 'HLT_lumipeb_L1ALFA_BGT', 'HLT_lumipeb_L1ALFA_BGT_BGRP10', 'HLT_lumipeb_L1ALFA_BGT_UNPAIRED_ISO', 'HLT_lumipeb_L1MBTS_1', 'HLT_lumipeb_L1MBTS_1_UNPAIRED_ISO', 'HLT_lumipeb_L1MBTS_2_UNPAIRED_ISO', 'HLT_mb_sp1300_hmtperf_L1TE3', 'HLT_mb_sp1300_hmtperf_L1TE3.0ETA24', 'HLT_mb_sp1400_pusup550_trk90_hmt_L1TE25', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE10', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE15', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE20', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE25', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE5', 'HLT_mb_sp1500_pusup700_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1500_trk100_hmt_L1TE10', 'HLT_mb_sp1500_trk100_hmt_L1TE10.0ETA24', 'HLT_mb_sp1500_trk100_hmt_L1TE15', 'HLT_mb_sp1500_trk100_hmt_L1TE20', 'HLT_mb_sp1500_trk100_hmt_L1TE20.0ETA24', 'HLT_mb_sp1500_trk100_hmt_L1TE5', 'HLT_mb_sp1500_trk100_hmt_L1TE5.0ETA24', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE10', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE10.0ETA24', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE15', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE20', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE20.0ETA24', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE25', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE5', 'HLT_mb_sp1800_pusup800_trk110_hmt_L1TE5.0ETA24', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE10', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE15', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE20', 'HLT_mb_sp2100_pusup1000_trk120_hmt_L1TE25', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE10', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE15', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE20', 'HLT_mb_sp2400_pusup1100_trk130_hmt_L1TE25', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE10', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE15', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE20', 'HLT_mb_sp2700_pusup1300_trk140_hmt_L1TE25', 'HLT_mb_sp2_hmtperf_L1TE3', 'HLT_mb_sp2_hmtperf_L1TE3.0ETA24', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE10', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE15', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE20', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE25', 'HLT_mb_sp3000_pusup1400_trk150_hmt_L1TE30', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE10', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE15', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE20', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE25', 'HLT_mb_sp3000_pusup1500_trk160_hmt_L1TE30', 'HLT_mb_sptrk_vetombts2in_L1ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_L1ALFA_ANY_UNPAIRED_ISO', 'HLT_mb_sptrk_vetombts2in_L1TRT_ALFA_ANY', 'HLT_mb_sptrk_vetombts2in_L1TRT_ALFA_ANY_UNPAIRED_ISO', 'HLT_noalg_L1ALFA_CDiff_Phys', 'HLT_noalg_L1ALFA_Diff_Phys', 'HLT_noalg_L1ALFA_Jet_Phys', 'HLT_noalg_L1TE15.0ETA24', 'HLT_noalg_L1TE25', 'HLT_noalg_L1TE25.0ETA24', 'HLT_noalg_L1TE3', 'HLT_noalg_L1TE3.0ETA24', 'HLT_noalg_mb_L1TE15.0ETA24', 'HLT_noalg_mb_L1TE25', 'HLT_noalg_mb_L1TE25.0ETA24', 'HLT_noalg_mb_L1TE3', 'HLT_noalg_mb_L1TE3.0ETA24', 'HLT_noalg_mb_L1TE40.0ETA24', ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled mb'), 
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
 'HLT_e14_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM12',
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
 'HLT_e9_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM7',
 'HLT_e9_tight_e4_etcut_Jpsiee_L1JPSI-1M5-EM7',
 'HLT_g10_etcut_mu10_L1LFV-EM8I',
 'HLT_g10_etcut_mu10_iloose_taumass_L1LFV-EM8I',
    'HLT_g10_loose_mu10_ivarloose_taumass_L1LFV-EM8I',
    'HLT_g20_loose_mu4_ivarloose_taumass_L1LFV-EM15I',
 'HLT_g20_etcut_mu4_L1LFV-EM15I',
 'HLT_g20_etcut_mu4_iloose_taumass_L1LFV-EM15I',
 'HLT_g20_loose_2j40_0eta490_3j25_0eta490_L1MJJ-700',
 'HLT_g20_loose_2j40_0eta490_3j25_0eta490_L1MJJ-900',
 'HLT_ht1000_L1HT190-J15.ETA21',
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
 'HLT_mu4_j35_bperf_split_dr05_dz02_L1BTAG-MU4J15',
 'HLT_mu4_j55_bperf_split_dr05_dz02_L1BTAG-MU4J15',
 'HLT_mu6_j110_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j150_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j175_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j260_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j320_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j400_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_mu6_j85_bperf_split_dr05_dz02_L1BTAG-MU6J20',
 'HLT_e17_lhmedium_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
 'HLT_e17_medium_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi05_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',
 'HLT_e18_etcut_trkcut_xs30_xe30_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',
 'HLT_j360_lcw_L1HT150-JJ15.ETA49',
 'HLT_j460_a10_lcw_L1HT190-J15.ETA21',
 'HLT_j360_a10_lcw_L1HT150-J20s5.ETA31',
 'HLT_xe110_L1KF-XE55',
 'HLT_xe110_mht_L1KF-XE55',
 'HLT_xe140_tc_lcw_L1KF-XE55',
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I-J25',
    'HLT_tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I',
 'HLT_mu4_2j35_dr05_j35_bmv2c2070_split_antimatchdr05mu_L13J15_BTAG-MU4J15',
 'HLT_mu4_2j40_dr05_j40_bmv2c2077_split_antimatchdr05mu_L13J15_BTAG-MU4J15',
 'HLT_j460_a10_lcw_L1HT190-J15s5.ETA21',
 'HLT_mu4_2j35_dr05_j35_bmv2c2070_split_antimatchdr05mu_L13J20_BTAG-MU4J20',
 'HLT_mu4_2j40_dr05_j40_bmv2c2077_split_antimatchdr05mu_L13J20_BTAG-MU4J20',
 'HLT_e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15-TAU12I',
 'HLT_e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15-TAU12I',
 'HLT_j360_a10_lcw_L1HT150-J20.ETA31',
 'HLT_mu6_mu4_bUpsimumu_L1MU6MU4-BO',
 'HLT_mu6_mu4_bBmumuxv2_L1MU6MU4-BO',
 'HLT_mu6_mu4_bJpsimumu_L1MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_L1MU6MU4-BO',
 'HLT_mu6_mu4_bBmumux_BcmumuDsloose_L1MU6MU4-BO',
 'HLT_mu6_mu4_bBmumu_L1MU6MU4-BO',
 'HLT_mu6_mu4_bDimu_novtx_noos_L1MU6MU4-BO',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30',
 'HLT_e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30',
 'HLT_e18_etcut_trkcut_xs30_xe30_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30',


    'HLT_2j45_bmv2c2060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400',
    'HLT_2j55_bmv2c2070_split_ht300_L1HT150-J20.ETA31',
    'HLT_2j55_bmv2c2070_split_ht300_L1HT150-J20s5.ETA31',
    'HLT_g25_medium_L1EM22VHI_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400',
    'HLT_ht300_2j40_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400',
    
    'HLT_j55_bmv2c2060_split_ht500_L1HT150-J20.ETA31',
    'HLT_j55_bmv2c2060_split_ht500_L1HT150-J20s5.ETA31',
    'HLT_mu4_iloose_mu4_11invm24_noos_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_11invm24_noos_L1DY-BOX-2MU4',
    'HLT_mu4_iloose_mu4_11invm24_noos_novtx_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_11invm24_noos_novtx_L1DY-BOX-2MU4',
    'HLT_mu4_iloose_mu4_24invm60_noos_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_24invm60_noos_L1DY-BOX-2MU4',
    'HLT_mu4_iloose_mu4_24invm60_noos_novtx_L1DY-BOX-2MU4',
    'HLT_mu4_ivarloose_mu4_24invm60_noos_novtx_L1DY-BOX-2MU4',    
 ]
disabled_Rules.update(dict(map(None,HLT_list,len(HLT_list)*[{
    1000 : TriggerRule(PS=-1, comment='Disabled L1Topo'), 
    }])))


#################################################################################################
###3brules####
others_Rules_startup_3b={}

HLT_list=['HLT_noalg_L1LowLumi']
others_Rules_startup_3b.update(dict(map(None,HLT_list,len(HLT_list)*[{ 
                    1 : TriggerRule(PS=1, ESValue=4, comment='for (number of filled bunches=2, L=1e31, rate=100Hz') ,
                    }]))) 

HLT_list=["HLT_tau25_idperf_tracktwo"]
others_Rules_startup_3b.update(dict(map(None,HLT_list,len(HLT_list)*[{
                    1000 : TriggerRule(rate=100., ESRate=0.5, rerun=1, comment="for ID Tracks"), 
                    3001 : TriggerRule(rate=0.5, ESValue=1, rerun=1, comment='Support Express'),
                    }])))


L1_list=["L1_TAU12IM","L1_TAU12"]
others_Rules_startup_3b.update(dict(map(None,L1_list,len(L1_list)*[{
                    1 : TriggerRule(rate=1000, comment="for ID Tracks", rerun=1), 
                    }])))

L1_list=["L1_EM20VH","L1_2EM10VH","L1_MU15","L1_2MU6","L1_3MU4","L1_EM15VH_MU10","L1_EM15I_MU4","L1_EM7_MU10","L1_2EM8VH_MU10","L1_TAU60","L1_TAU20IM_2TAU12IM_J25_2J20_3J12","L1_EM15HI_2TAU12IM_XE35","L1_MU10_TAU12IM_XE35","L1_TAU20_2TAU12_XE35","L1_TAU20IM_2TAU12IM_XE35","L1_EM15HI_2TAU12IM","L1_EM15HI_TAU40_2TAU15","L1_MU10_TAU12IM_J25_2J12","L1_MU10_TAU12IM","L1_J75","L1_4J15","L1_XE50","L1_3J25.0ETA23","L1_3J40","L1_2J15_XE55","L1_MU6_J40","L1_J75.31ETA49",]

others_Rules_startup_3b.update(dict(map(None,L1_list,len(L1_list)*[{
                    1 : TriggerRule(PS=1, comment='L1Seed for HLT_noalg_L1LowLumi'),
                    5001 : TriggerRule(PS=-1, comment='Should follow the default rules'),
                    }])))


########

rules = {}

# full list
RulesList=[muon_Rules, bphys_Rules, met_Rules, jet_Rules, bjet_Rules, tau_Rules, egamma_Rules, combined_Rules, vbf_Rules, dedicated_Rules, disabled_Rules]

for Rules in RulesList:
    for newrule in Rules.keys():
        if rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside physics rule. Cannot be added:',newrule
    rules.update(Rules)

lumi_Rules = {
	'L1_RD0_BGRP9':	{ 1 : TriggerRule(rate=10000, scaling='bunches', comment='target 20 kHz, ATR-15089') },
  'HLT_lumipeb_vdm_L1RD0_BGRP9' : {  1 : TriggerRule(PS=1, comment='target 20 kHz'), },
	}
if add2kHxPixel:
    rules.update(lumi_Rules)

# ----------------------------------------

RulesList=[datascouting_Rules]

for Rules in RulesList:
    for newrule in Rules.keys():
        if rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule
    rules.update(Rules)

# ----------------------------------------

RulesList=[zerobias_Rules, tauoverlay_Rules, MinBias_Rules]
for Rules in RulesList:
    for newrule in Rules.keys():
        if rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule, "CHECK"
    rules.update(Rules)

RulesList=[l1calo_streamer_Rules, topo_Rules,]
for Rules in RulesList:
    for newrule in Rules.keys():
        if rules.has_key(newrule):
            if 0: print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule, ". For L1Topo, overwritten intentionally."
    rules.update(Rules)

# ----------------------------------------
#3bunches
if do3bunches:
    RulesList=[others_Rules_startup_3b]                                        
    for Rules in RulesList:
        for newrule in Rules.keys():
            if rules.has_key(newrule):
                if 0: print 'FATAL     Physics_pp_v6_rules     Duplicated rule inside calibration rule. Cannot be added:',newrule, ". For L1Topo, overwritten intentionally."
    rules.update(Rules)
#

RulesList=[Monitoring_pp_v6_rules.physics_rules]

# disable L1Topo from monitoring rules in case there are some still active

if NoL1Topo:
    for mon_rules in RulesList:
        for newrule in mon_rules.keys():
            if 'L1_' in newrule and '-' in newrule:
                print  "NoL1Topo set to True: Disabling ", newrule
                mon_rules[newrule]={1 : TriggerRule(PS=-1, comment='disable L1 Topo')}


    
for Rules in RulesList:
    for newrule in Rules.keys():
        if rules.has_key(newrule):
            print 'FATAL     Physics_pp_v6_rules     Duplicated rule in monitoring rule. Cannot be added from monitoring rule for:',newrule
    rules.update(Rules)

            
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

}

rules.update(override_rules)

### Remove primaries for luminosities lower than the given value
disableLowerLumiPrimaries(rules,10001)

#################################################################################################
