# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration

#----------phy--------------------------------------------------------------#
#------------------------------------------------------------------------#
def setupMenu():

    from TriggerJobOpts.TriggerFlags          import TriggerFlags
    from AthenaCommon.Logging                 import logging
    log = logging.getLogger( 'TriggerMenu.menu.Physics_pp_v7.py' )

    from TriggerMenu.TriggerConfigLVL1 import TriggerConfigLVL1 as tcl1
    if tcl1.current:
        log.info("L1 items: %s " % tcl1.current.menu.items.itemNames())
    else:
        log.info("ERROR L1 menu has not yet been defined")

    PhysicsStream="Main"

    # stream, BW and RATE tags for Bphysics items that appear in Muon and Bphysics slice.signatures
    BPhysicsStream     = "BphysLS"
    BMultiMuonStream   = "Main"  
    RATE_BphysTag      = 'RATE:Bphysics'
    RATE_BMultiMuonTag = 'RATE:BMultiMuon'  # can become RATE:BMultiMuon' with one letter change 
    BW_BphysTag        = 'BW:Bphys'
    RATE_DYTag         = 'RATE:Muon'   
    BW_DYTag           = 'BW:Muon'   

    #---------------------------------------------------------------------
    # INPUT FORMAT FOR CHAINS:
    # ['chainName',  'L1itemforchain', [L1 items for chainParts], [stream], [groups], EBstep], OPTIONAL: [mergingStrategy, offset,[merginOrder] ]], topoStartsFrom = False
    #---------------------------------------------------------------------

    #---------------------------------------------------------------------
    # if it's needed to temporary remove almost all the chains from the menu
    # be aware that it is necessary to leave at least one chain in the muon slice
    # otherwise athenaHLT will seg-fault 
    #---------------------------------------------------------------------

    TriggerFlags.Slices_all_setOff()

    TriggerFlags.TestSlice.signatures = []

    TriggerFlags.AFPSlice.signatures = [
        ['afp_jetexc_L1AFP_A_AND_C_SPECTOF_J50',  'L1_AFP_A_AND_C_SPECTOF_J50',  [],  [PhysicsStream], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['afp_jetexc_L1AFP_A_AND_C_SPECTOF_J75',  'L1_AFP_A_AND_C_SPECTOF_J75',  [],  [PhysicsStream], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['afp_jetexc_L1AFP_A_AND_C_SPECTOF_J100',  'L1_AFP_A_AND_C_SPECTOF_J100',  [],  [PhysicsStream], [ 'RATE:MinBias','BW:MinBias'], -1],
        ]

    TriggerFlags.MuonSlice.signatures = [
        ['mu26_ivarmedium',	     'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream, 'express'], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
#        ['mu26_ivarmedium_zROItest', 'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu28_ivarmedium',	     'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu80',                   'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu50',                   'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['Primary:17000','RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu60',                   'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu60_0eta105_msonly',    'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_ivarmedium_L1MU10', 'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu26',                   'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu22',                   'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20',                   'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24',                   'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_msonly',            'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu14',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu14_ivarloose',         'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu6',                    'L1_MU6',            [], [PhysicsStream,'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu6_msonly',             'L1_MU6',            [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu4',                    'L1_MU4',            [], [PhysicsStream,'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24_idperf',            'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu20_idperf',            'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu40_idperf',            'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu10_idperf',            'L1_MU10',           [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu6_idperf',             'L1_MU6',            [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu4_idperf',             'L1_MU4',            [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],

        ['mu4_nomucomb',                   'L1_MU4',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu6_nomucomb',                   'L1_MU6',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_nomucomb',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu11_nomucomb',                   'L1_MU11',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],

        #ATR-16981
        ['mu24_ivarmedium',	     'L1_MU20MU21',   ['L1_MU20'], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],

        #Low-mu run triggers
        ['mu15',     'L1_MU11',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu16',     'L1_MU11',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu18',     'L1_MU11',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],

				# Toroid-off run (ATR-9923)
        ['mu8_mucombTag_noEF_L1MU40',  'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu15_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu25_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],

        # multi muons
        ['2mu14',                  'L1_2MU10',          [], [PhysicsStream, 'express'], ['Primary:20000','RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu15_L12MU10',          'L1_2MU10',          [], [PhysicsStream], ['Primary:20000','RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6',                   'L1_3MU6',           [], [PhysicsStream], ['Primary:20000','RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6_msonly',            'L1_3MU6',           [], [PhysicsStream], ['Primary:20000','RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu8_msonly',            'L1_3MU6',           [], [PhysicsStream], ['Primary:20000','RATE:MultiMuon', 'BW:Muon'], -1],
        ['mu24_mu8noL1',           'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream, 'express'], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','mu8noL1']]],
        ['mu24_mu10noL1',          'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','mu10noL1']]],
        ['mu24_2mu4noL1',          'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','2mu4noL1']]],
        ['mu26_mu8noL1',           'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu26','mu8noL1']]],
        ['mu26_mu10noL1',          'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu26','mu10noL1']]],
        ['mu28_mu8noL1',           'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu28','mu8noL1']]],
        ['mu22_mu8noL1',           'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:17000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu22','mu8noL1']]],
        ['mu22_2mu4noL1',          'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu22','2mu4noL1']]],
        ['mu20_2mu4noL1',          'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20','2mu4noL1']]],
        ['mu20_2mu2noL1_JpsimumuFS', 'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], [RATE_BMultiMuonTag,'BW:Muon'], -1,['serial',-1,['mu20','2mu2noL1_JpsimumuFS']]],  # OI - who owns these triggers? Bphys?
        ['mu20_2mu4_JpsimumuL2',     'L1_MU20MU21', ['L1_MU20','L1_2MU4'], [PhysicsStream], [RATE_BMultiMuonTag,'BW:Muon'], -1,['serial',-1,['2mu4_JpsimumuL2','mu20']]], # OI - who owns these triggers?
        
        # Primary (multi muon chains)
        ['3mu4',                   'L1_3MU4',           [], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        ['3mu4_mu2noL1',           'L1_3MU4', ['L1_3MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1,['serial',-1,['3mu4','mu2noL1']]],
        ['3mu3_mu3noL1_L13MU4',    'L1_3MU4', ['L1_3MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1,['serial',-1,['3mu3','mu3noL1']]],
        ['3mu4_mu4noL1',           'L1_3MU4', ['L1_3MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1,['serial',-1,['3mu4','mu4noL1']]],
        ['4mu4',                   'L1_4MU4', ['L1_4MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1,['serial',-1,['4mu4','']]],

#				#ATR-15246
        ['mu22_mu8noL1_mu6noL1',   'L1_MU20MU21',       ['L1_MU20','',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon', 'BW:Muon'],  -1,['serial',-1,['mu22','mu8noL1','mu6noL1']]],
				#ATR-15878
        ['mu22_mu8noL1_calotag_0eta010',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:17000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu22','mu8noL1_calotag_0eta010']]],
        ['mu24_mu8noL1_calotag_0eta010',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','mu8noL1_calotag_0eta010']]],
        ['mu26_mu8noL1_calotag_0eta010',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu26','mu8noL1_calotag_0eta010']]],
        ['mu28_mu8noL1_calotag_0eta010',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu28','mu8noL1_calotag_0eta010']]],
        ['mu24_mu10noL1_calotag_0eta010', 'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','mu10noL1_calotag_0eta010']]],
        ['mu26_mu10noL1_calotag_0eta010', 'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu26','mu10noL1_calotag_0eta010']]],

        #DiMuon for TAU OVERLAY
        ['2mu20_L12MU20_OVERLAY',   'L1_2MU20_OVERLAY', [], ['TauOverlay'], ["RATE:TauOverlay",  "BW:TauOverlay"], -1],
        
        ## ADD MU*_EMPTY TO L1 MENU
        ['mu4_cosmic_L1MU4_EMPTY',               'L1_MU4_EMPTY', [],   ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],
        ['mu4_cosmic_L1MU11_EMPTY',              'L1_MU11_EMPTY', [],  ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],
        ['mu4_msonly_cosmic_L1MU4_EMPTY',        'L1_MU4_EMPTY', [],   ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],
        ['mu4_msonly_cosmic_L1MU11_EMPTY',       'L1_MU11_EMPTY', [],  ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],                            
                
        # muon calibration
        ['mu0_muoncalib',             'L1_MU20MU21', ['L1_MU20'],['Muon_Calibration'],["RATE:Calibration","BW:Muon"],-1],
        ['mu0_muoncalib_L1MU4_EMPTY', 'L1_MU4_EMPTY',[],['Muon_Calibration'],["RATE:Calibration","BW:Muon"],-1],

        # muon calibration, data scouting
        ['mu0_muoncalib_ds3',             'L1_MU20MU21', ['L1_MU20'],['DataScouting_03_CosmicMuons'],["RATE:Calibration","BW:Muon"],-1],        
        ['mu0_muoncalib_ds3_L1MU4_EMPTY', 'L1_MU4_EMPTY',[],['DataScouting_03_CosmicMuons'],["RATE:Calibration","BW:Muon"],-1],   

        ## LFV topo based nscan (ATR-14352)
        ['mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU', 'L1_LFV-MU', ['L1_MU10','L2_mu11_nomucomb'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11_nomucomb','2mu4noL1_nscan03']]], #OI need to discuss what to do here
	# -----------------------------------------------------

        # LLP chains
        # notes: see ATR-11482
        # L1 is seeded from single EMPTY/UNPAIRED_ISO, but, at HLT it does a same sequence as main ones (e.g. MU4_EMPTY single seeded, but 3MU6 is required at HLT)
        # this is done on purpose (ATR-11482)
        ['3mu6_msonly_L1MU4_EMPTY',                              'L1_MU4_EMPTY',        ['L1_3MU6'], ["Late"], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6_msonly_L1MU6_EMPTY',                              'L1_MU6_EMPTY',        ['L1_3MU6'], ["Late"], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6_msonly_L1MU4_UNPAIRED_ISO',                       'L1_MU4_UNPAIRED_ISO', ['L1_3MU6'], ["Late"], ['RATE:MultiMuon', 'BW:Muon'], -1],


         ### ATR-14377: Emergency additional class of chains instead of mu20_msonly_mu10noL1_msonly_nscan05_noComb*
        ['mu20_msonly_mu15noL1_msonly_nscan05_noComb',   'L1_MU20MU21',   ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu15noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU6_EMPTY',          'L1_MU6_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu15noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO',   'L1_MU4_UNPAIRED_ISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu15noL1_msonly_nscan05_noComb']]],


        ['mu6_2mu4', 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        ['mu20_ivarmedium_L1MU10_2J20', 'L1_MU10_2J20', [], [PhysicsStream], ['Primary:20000','RATE:MuonJet', 'BW:Muon','BW:Jet'], -1],                
        ['mu20_ivarmedium_L1MU10_2J15_J20', 'L1_MU10_2J15_J20', [], [PhysicsStream], ['Primary:17000','RATE:MuonJet', 'BW:Muon','BW:Jet'], -1],  

        #nscan05 triggers rejecting CB muons

        ['mu20_msonly_mu6noL1_msonly_nscan05', 'L1_MU20MU21',      ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05']]],
        ['mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU20_XE30', 'L1_MU20_XE30', ['L1_MU20_XE30','L2_mu20_msonly_iloosems'], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly_iloosems','mu6noL1_msonly_nscan05']]],
        ['mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU20_J40', 'L1_MU20_J40',   ['L1_MU20_J40','L2_mu20_msonly_iloosems'], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly_iloosems','mu6noL1_msonly_nscan05']]],
        ['mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU6_EMPTY', 'L1_MU6_EMPTY',   ['L1_MU20','L2_mu20_msonly_iloosems'], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly_iloosems','mu6noL1_msonly_nscan05']]],
        ['mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO',   ['L1_MU20','L2_mu20_msonly_iloosems'], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly_iloosems','mu6noL1_msonly_nscan05']]],
        ['mu6_dRl1_mu20_msonly_iloosems_mu6noL1_dRl1_msonly','L1_MU11_2MU6', [['L1_MU6','L1_MU6'],'L1_MU20',''], [PhysicsStream], ['Primary:20000','RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu6_dRl1','mu20_msonly_iloosems','mu6noL1_dRl1_msonly']]],

        #Chains for testing 2e34 menus
        ['mu32_ivarmedium',          'L1_MU20MU21',           ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        

        # Bphys item, needs to be in muon slice: ATR-14367
        ['mu6_nomucomb_2mu4_nomucomb_L1MU6_3MU4',          'L1_MU6_3MU4',  ['L1_MU6','L1_2MU4'],          [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        ['2mu6_nomucomb_mu4_nomucomb_L12MU6_3MU4',          'L1_2MU6_3MU4',  ['L1_2MU6','L1_MU4'],          [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        ['3mu3_mu3noL1_calotag_0eta010_L13MU4',    'L1_3MU4', ['L1_3MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1,['serial',-1,['3mu3','mu3noL1_calotag_0eta010']]],
        ['3mu4_mu4noL1_calotag_0eta010',           'L1_3MU4', ['L1_3MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1,['serial',-1,['3mu4','mu4noL1_calotag_0eta010']]],

        #HI Muons for 5TeV run ATR-17162
        ['mu6_ivarloose',                   'L1_MU6',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu8',                   'L1_MU6',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10',                   'L1_MU10',           [], [PhysicsStream,'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_ivarloose',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu15_L1MU10',                   'L1_MU10',           [], [PhysicsStream,'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu18_L1MU10',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        
        ['2mu4',                 'L1_2MU4', [], [PhysicsStream,'express'], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        ['2mu6',                 'L1_2MU6', [], [PhysicsStream,'express'], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        
        ['2mu4_nomucomb',                 'L1_2MU4', [], [PhysicsStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        ['2mu6_nomucomb',                 'L1_2MU6', [], [PhysicsStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        

        
        #AFP muons
        ['mu4_L1MU4_AFP_A_AND_C',                        'L1_MU4_AFP_A_AND_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['mu4_L1MU4_AFP_A_OR_C',                        'L1_MU4_AFP_A_OR_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['mu4_L1MU4',                        'L1_MU4',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        
        #HMT muons in MB stream (ATR16442)
        ['mu6_L1MU6',                    'L1_MU6',            [], ['MinBias'], ['RATE:MinBias','BW:MinBias'], -1],
        ['mu10_L1MU10',                   'L1_MU10',           [], ['MinBias'], ['RATE:MinBias','BW:MinBias'], -1],
        ['mu11_L1MU10',                   'L1_MU10',           [], ['MinBias'], ['RATE:MinBias','BW:MinBias'], -1],
        ['mu14_L1MU10',                   'L1_MU10',           [], ['MinBias'], ['RATE:MinBias','BW:MinBias'], -1],


        ['mu10_mgonly_L1LATE-MU10_J50',          'L1_LATE-MU10_J50',           [], [PhysicsStream], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_mgonly_L1LATE-MU10_XE50',         'L1_LATE-MU10_XE50',          [], [PhysicsStream], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_mgonly_L1LATE-MU10_XE40',         'L1_LATE-MU10_XE40',          [], [PhysicsStream], ['Primary:20000','RATE:SingleMuon', 'BW:Muon'], -1],
	['mu50_mgonly_inTimeRoI',                'L1_MU20MU21',       ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu60_mgonly_inTimeRoI',                'L1_MU20MU21',       ['L1_MU20'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],

        #Low-mu run triggers
        ['mu4_mu2noL1_L1MU4', 'L1_MU4',  ['L1_MU4',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu4','mu2noL1']]],
        ['mu6_mu2noL1_L1MU6', 'L1_MU6',  ['L1_MU6',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu6','mu2noL1']]],

			 ]


    if TriggerFlags.doFTK():
        TriggerFlags.MuonSlice.signatures = TriggerFlags.MuonSlice.signatures() + [
            ['mu24_idperf_FTK_L1MU20MU21_FTK',       'L1_MU20MU21_FTK', ['L1_MU20_FTK'], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1], 
            ['mu6_idperf_FTK_L1MU6_FTK',             'L1_MU6_FTK',            [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
            ['mu24_idperf_FTKRefit_L1MU20MU21_FTK',  'L1_MU20MU21_FTK', ['L1_MU20_FTK'], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1], 
            ['mu6_idperf_FTKRefit_L1MU6_FTK',        'L1_MU6_FTK',            [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
            ['mu22_mu8noL1_FTKFS_L1MU20MU21_FTK',    'L1_MU20MU21_FTK', ['L1_MU20_FTK',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu22','mu8noL1_FTKFS']]],
            ['mu26_ivarmedium_FTK_L1MU20MU21_FTK',   'L1_MU20MU21_FTK', ['L1_MU20_FTK'], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
            ]




    TriggerFlags.JetSlice.signatures = [   

        #####
        ######################### Performance-style chains
        #####

        # Jet data scouting stream chains
        # These record only the trigger jets, but for every event passing the L1 trigger
        ['j0_perf_ds1_L1J50',      'L1_J50',  [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],
        ['j0_perf_ds1_L1J75',      'L1_J75',  [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],
        ['j0_perf_ds1_L1J100',     'L1_J100', [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],

        #Low-mu run triggers
        ['j0_perf_ds1_L1J15',      'L1_J15',  [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],
        ['j0_perf_ds1_L1J20',      'L1_J20',  [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],
        ['j0_perf_ds1_L1J30',      'L1_J30',  [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],

        # Performance chains
        ['ht0_L1J12_EMPTY',        'L1_J12_EMPTY', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j0_L1J12_EMPTY',         'L1_J12_EMPTY', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j0_perf_L1RD0_FILLED',   'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],



        #####
        ######################### Single-jet small-R trigger chains
        #####
        
        # Standard central jet triggers
        ['j15',                         'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j25',                         'L1_RD0_FILLED', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35',                         'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j45',                         'L1_J15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j45_L1RD0_FILLED',            'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j60',                         'L1_J20',  [], [PhysicsStream, 'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j60_L1RD0_FILLED',            'L1_RD0_FILLED', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85',                         'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j110',                        'L1_J30',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j175',                        'L1_J50',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260',                        'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j360',                        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j380',                        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400',                        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420',                        'L1_J100', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j440',                        'L1_J120', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j450',                        'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j460',                        'L1_J120', [], [PhysicsStream, 'express'], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j480',                        'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j500',                        'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],

        # Standard gsc central jet triggers (primaries)
        ['j225_gsc400_boffperf_split',  'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j225_gsc420_boffperf_split',  'L1_J100', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j225_gsc440_boffperf_split',  'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j225_gsc450_boffperf_split',  'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j225_gsc460_boffperf_split',  'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j225_gsc480_boffperf_split',  'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j225_gsc500_boffperf_split',  'L1_J120', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],

        # Alternative calibration central jet triggers
        ['j35_subjes',                  'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_jes',                     'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_sub',                     'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_nojcalib',                'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw',                     'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw_subjes',              'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw_jes',                 'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw_sub',                 'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw_nojcalib',            'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_subjes',                 'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_jes',                    'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_sub',                    'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_nojcalib',               'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_lcw',                    'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_lcw_subjes',             'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_lcw_jes',                'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_lcw_sub',                'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j450_lcw_nojcalib',           'L1_J120', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        # SoftKiller central jet triggers
        #['j35_sktc_lcw_nojcalib',       'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        # Standard forward jet triggers
        ['j15_320eta490',               'L1_RD0_FILLED',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j25_320eta490',               'L1_RD0_FILLED',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j35_320eta490',               'L1_RD0_FILLED',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j45_320eta490',               'L1_J15.31ETA49', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j60_320eta490',               'L1_J20.31ETA49', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_320eta490',               'L1_J20.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j110_320eta490',              'L1_J30.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j175_320eta490',              'L1_J50.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490',              'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j360_320eta490',              'L1_J100.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],

        # Alternative calibration forward jet triggers
        ['j35_320eta490_subjes',        'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_jes',           'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_sub',           'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_nojcalib',      'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_lcw',           'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_lcw_subjes',    'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_lcw_jes',       'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_lcw_sub',       'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_320eta490_lcw_nojcalib',  'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_subjes',       'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_jes',          'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_sub',          'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_nojcalib',     'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw',          'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw_subjes',   'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw_jes',      'L1_J75.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw_sub',      'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw_nojcalib', 'L1_J75.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet',  'BW:Jet'], -1],
        
        
        
        #####
        ######################### Single-jet large-R trigger chains
        #####

        # Standard topocluster large-R jet triggers
        ['j260_a10_lcw_subjes_L1J75',       'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10_lcw_subjes_L1J100',      'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10_lcw_subjes_L1J100',      'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10_lcw_subjes_L1J100',      'L1_J100', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_lcw_subjes_L1J100',      'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j500_a10_lcw_subjes_L1J100',      'L1_J100', [], [PhysicsStream, 'express'], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j520_a10_lcw_subjes_L1J100',      'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j540_a10_lcw_subjes_L1J100',      'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],

        # Alternative calibration topocluster large-R jet triggers
        ['j260_a10_sub_L1J75',              'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_nojcalib_L1J75',         'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_lcw_sub_L1J75',          'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_lcw_nojcalib_L1J75',     'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_sub_L1J100',             'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_nojcalib_L1J100',        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_lcw_sub_L1J100',         'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_lcw_nojcalib_L1J100',    'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # L1topo standard topocluster large-R jet triggers
        ['j260_a10_lcw_subjes_L1SC85',              'L1_SC85-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_lcw_subjes_L1HT150-J20.ETA31',   'L1_HT150-J20.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_lcw_subjes_L1HT150-J20s5.ETA31', 'L1_HT150-J20s5.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_lcw_subjes_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_lcw_subjes_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_lcw_subjes_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # Standard reclustered large-R jet triggers
        ['j260_a10r_L1J75',                 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10r_L1J100',                'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10r_L1J100',                'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10r_L1J100',                'L1_J100', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10r_L1J100',                'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j500_a10r_L1J100',                'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j520_a10r_L1J100',                'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j540_a10r_L1J100',                'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        
        # L1topo standard reclustered large-R jet triggers
        ['j260_a10r_L1SC85',                'L1_SC85-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10r_L1HT150-J20.ETA31',     'L1_HT150-J20.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10r_L1HT150-J20s5.ETA31',   'L1_HT150-J20s5.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10r_L1SC111',               'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10r_L1HT190-J15.ETA21',     'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10r_L1HT190-J15s5.ETA21',   'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # Standard trimmed large-R jet triggers
        ['j260_a10t_lcw_jes_L1J75',         'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_L1J100',        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_L1J100',        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10t_lcw_jes_L1J100',        'L1_J100', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10t_lcw_jes_L1J100',        'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j500_a10t_lcw_jes_L1J100',        'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j520_a10t_lcw_jes_L1J100',        'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j540_a10t_lcw_jes_L1J100',        'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],

        # Alternative calibration trimmed large-R jet triggers
        ['j260_a10t_lcw_nojcalib_L1J75',    'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10t_lcw_nojcalib_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # L1topo standard trimmed large-R jet triggers
        ['j260_a10t_lcw_jes_L1SC85',                'L1_SC85-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10t_lcw_jes_L1HT150-J20.ETA31',     'L1_HT150-J20.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10t_lcw_jes_L1HT150-J20s5.ETA31',   'L1_HT150-J20s5.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10t_lcw_jes_L1SC111',               'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10t_lcw_jes_L1HT190-J15.ETA21',     'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10t_lcw_jes_L1HT190-J15s5.ETA21',   'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # Trimmed + mass-cut single-jet triggers
        ['j375_a10t_lcw_jes_30smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_30smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_30smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_30smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_30smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_30smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_30smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_30smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j375_a10t_lcw_jes_35smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_35smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_35smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_35smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_35smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_35smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_35smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_35smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j375_a10t_lcw_jes_40smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_40smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_40smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_40smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_40smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_40smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_40smcINF_L1J100',               'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_40smcINF_L1SC111',              'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],


        # Trimmed + mass-cut single-jet support triggers (for mass efficiency bootstrap)
        ['j375_a10t_lcw_jes_L1J100',                        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_L1HT190-J15.ETA21',             'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j375_a10t_lcw_jes_L1SC111',                       'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_L1J100',                        'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_L1HT190-J15.ETA21',             'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j390_a10t_lcw_jes_L1SC111',                       'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        # j420 L1J100 is above
        ['j420_a10t_lcw_jes_L1HT190-J15.ETA21',             'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10t_lcw_jes_L1SC111',                       'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        # j440 L1J100 is above
        ['j440_a10t_lcw_jes_L1HT190-J15.ETA21',             'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10t_lcw_jes_L1SC111',                       'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # Trimmed + mass-cut di-jet triggers
        ['2j330_a10t_lcw_jes_30smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_30smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_30smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_30smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_30smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_30smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_30smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['2j330_a10t_lcw_jes_35smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_35smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_35smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_35smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_35smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_35smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_35smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['2j330_a10t_lcw_jes_40smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_40smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_40smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_40smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_40smcINF_L1J100',              'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_40smcINF_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_40smcINF_L1SC111',             'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # Trimmed + mass-cut asymmetric di-jet triggers (for rate optimization)
        ['j400_a10t_lcw_jes_30smcINF_j300_a10t_lcw_jes_30smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j350_a10t_lcw_jes_30smcINF_j300_a10t_lcw_jes_30smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j320_a10t_lcw_jes_30smcINF_j250_a10t_lcw_jes_30smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j400_a10t_lcw_jes_35smcINF_j300_a10t_lcw_jes_35smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j350_a10t_lcw_jes_35smcINF_j300_a10t_lcw_jes_35smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j320_a10t_lcw_jes_35smcINF_j250_a10t_lcw_jes_35smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j400_a10t_lcw_jes_40smcINF_j300_a10t_lcw_jes_40smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j350_a10t_lcw_jes_40smcINF_j300_a10t_lcw_jes_40smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j320_a10t_lcw_jes_40smcINF_j250_a10t_lcw_jes_40smcINF_L1J100',   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j310_a10t_lcw_jes_60smcINF_j310_a10t_lcw_jes_L1SC111',   'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j325_a10t_lcw_jes_60smcINF_j325_a10t_lcw_jes_L1SC111',   'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j340_a10t_lcw_jes_60smcINF_j340_a10t_lcw_jes_L1SC111',   'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j360_a10t_lcw_jes_60smcINF_j360_a10t_lcw_jes_L1SC111',   'L1_SC111-CJ15', [], [PhysicsStream], ['Primary:17000','RATE:SingleJet', 'BW:Jet'], -1],

        # Trimmed + mass-cut di-jet support triggers (for mass efficiency bootstrap)
        ['2j330_a10t_lcw_jes_L1J100',                       'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_L1HT190-J15.ETA21',            'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j330_a10t_lcw_jes_L1SC111',                      'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_L1J100',                       'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_L1HT190-J15.ETA21',            'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j340_a10t_lcw_jes_L1SC111',                      'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_L1J100',                       'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_L1HT190-J15.ETA21',            'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['2j350_a10t_lcw_jes_L1SC111',                      'L1_SC111-CJ15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        #Low-mu run triggers
        ['j360_a10_lcw_subjes_L1J100',     'L1_J100',[], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j110_a10_lcw_subjes_L1J30',      'L1_J30', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        #####
        ######################### Multi-jet small-R trigger chains
        #####

        # 3-jet single threshold triggers
        ['3j200',                       'L1_J100', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['3j225',                       'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['3j250',                       'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],

        # 4-jet single threshold triggers
        ['4j25',                        'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45',                        'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j100',                       'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j110',                       'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j120',                       'L1_3J50', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['4j130',                       'L1_3J50', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],

        # 4-jet gsc single threshold triggers
        ['4j60_gsc100_boffperf_split',  'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j60_gsc110_boffperf_split',  'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j60_gsc115_boffperf_split',  'L1_3J50', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['4j70_gsc120_boffperf_split',  'L1_3J50', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['4j70_gsc130_boffperf_split',  'L1_3J50', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],

        # 4-jet single threshold alternative calibration triggers
        ['4j45_subjes',                 'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_jes',                    'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_sub',                    'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_nojcalib',               'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_lcw',                    'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_lcw_subjes',             'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_lcw_jes',                'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_lcw_sub',                'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45_lcw_nojcalib',           'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        # 5-jet single threshold triggers
        ['5j25',                        'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j45',                        'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_L14J15',                 'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70',                        'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_L14J15',                 'L1_4J15', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85',                        'L1_4J20', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j90_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j90',                        'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j100_L14J150ETA25',          'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j100',                       'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 5-jet gsc single threshold triggers
        ['5j50_gsc70_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j50_gsc70_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j55_gsc75_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j55_gsc75_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60_gsc85_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60_gsc85_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60_gsc90_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60_gsc90_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60_gsc100_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60_gsc100_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 5-jet single threshold restricted eta range triggers
        ['5j65_0eta240_L14J15',         'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j65_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j65_0eta240',                'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_0eta240_L14J15',         'L1_4J15', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_0eta240',                'L1_4J20', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j75_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j75_0eta240',                'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_0eta240',                'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
 
        # 5-jet gsc single threshold restricted eta range triggers
        ['5j50_gsc65_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j50_gsc65_boffperf_split_0eta240',               'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j50_gsc70_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j50_gsc70_boffperf_split_0eta240',               'L1_4J20', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j55_gsc75_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['5j55_gsc75_boffperf_split_0eta240',               'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 6-jet single threshold triggers
        ['6j25',                        'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],        
        ['6j45',                        'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_L14J15',                 'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60',                        'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j70_L14J15',                 'L1_4J15', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j70_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j70',                        'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j85_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j85',                        'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 6-jet gsc single threshold triggers
        ['6j45_gsc60_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_gsc60_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_gsc60_boffperf_split_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_gsc65_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_gsc65_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_gsc65_boffperf_split_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_gsc70_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_gsc70_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_gsc70_boffperf_split_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_gsc85_boffperf_split',               'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_gsc85_boffperf_split_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_gsc85_boffperf_split_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],

        # 6-jet single threshold restricted eta range triggers
        ['6j45_0eta240',                'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_0eta240_L14J20',         'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_0eta240_L15J150ETA25',   'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_0eta240_L14J15',         'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_0eta240_L14J20',         'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_0eta240_L15J150ETA25',   'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j55_0eta240_L14J15',         'L1_4J15', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j55_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j55_0eta240_L14J20',         'L1_4J20', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j55_0eta240_L15J150ETA25',   'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_0eta240_L14J20',         'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_0eta240_L15J150ETA25',   'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 6-jet gsc single threshold restricted eta range triggers
        ['6j25_gsc45_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j25_gsc45_boffperf_split_0eta240_L14J20',        'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j25_gsc45_boffperf_split_0eta240_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j25_gsc50_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j25_gsc50_boffperf_split_0eta240_L14J20',        'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j25_gsc50_boffperf_split_0eta240_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j35_gsc55_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j35_gsc55_boffperf_split_0eta240_L14J20',        'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j35_gsc55_boffperf_split_0eta240_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_gsc60_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_gsc60_boffperf_split_0eta240_L14J20',        'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_gsc60_boffperf_split_0eta240_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],

        # 6-jet single threshold alternative calibration triggers
        ['6j45_subjes',                 'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_jes',                    'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_sub',                    'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_nojcalib',               'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_lcw',                    'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_lcw_subjes',             'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_lcw_jes',                'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_lcw_sub',                'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_lcw_nojcalib',           'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        # 7-jet single-threshold triggers
        ['7j25',                        'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],      
        ['7j45_L14J15',                 'L1_4J15', [], [PhysicsStream], ['Primary:17000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_L14J20',                 'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45',                        'L1_6J15', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j50_L14J150ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j50_L14J20',                 'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j50',                        'L1_6J15', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 7-jet gsc single threshold triggers
        ['7j25_gsc45_boffperf_split_L14J150ETA25',          'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc45_boffperf_split_L14J20',                'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc45_boffperf_split_L15J150ETA25',          'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc50_boffperf_split_L14J150ETA25',          'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc50_boffperf_split_L14J20',                'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc50_boffperf_split_L15J150ETA25',          'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 7-jet single-threshold restricted eta range triggers
        ['7j45_0eta240_L14J150ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_0eta240_L14J20',         'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_0eta240_L15J150ETA25',   'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_0eta240',                'L1_6J15', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        
        # 7-jet gsc single threshold restricted eta range triggers
        ['7j25_gsc45_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc45_boffperf_split_0eta240_L14J20',        'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc45_boffperf_split_0eta240_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc50_boffperf_split_0eta240_L14J150ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc50_boffperf_split_0eta240_L14J20',        'L1_4J20', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['7j25_gsc50_boffperf_split_0eta240_L15J150ETA25',  'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],

        # 10-jet single-threshold triggers
        ['10j40_L14J15',                'L1_4J15' , [], [PhysicsStream], ['Primary:17000','RATE:MultiJet', 'BW:Jet'], -1],
        ['10j40_L14J150ETA25',          'L1_4J15.0ETA25' , [], [PhysicsStream], ['Primary:20000','RATE:MultiJet', 'BW:Jet'], -1],
        ['10j40_L14J20',                'L1_4J20' , [], [PhysicsStream], ['Primary:20000','RATE:MultiJet', 'BW:Jet'], -1],
        ['10j40_L16J15',                'L1_6J15' , [], [PhysicsStream], ['Primary:20000','RATE:MultiJet', 'BW:Jet'], -1],

        # L1topo multi-jet triggers
        ['5j65_0eta240_L1J4-MATCH',     'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['5j70_L1J4-MATCH',             'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['5j85_L1J4-MATCH',             'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['6j45_0eta240_L1J4-MATCH',     'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['6j50_0eta240_L1J4-MATCH',     'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['7j45_L1J4-MATCH',             'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['7j50_L1J4-MATCH',             'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],



        #####
        ######################### HT trigger chains
        #####

        # HT triggers
        ['ht700_L1J75',     'L1_J75', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1], 
        ['ht700_L1J100',    'L1_J100', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1000_L1J100',   'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1200_L1J100',   'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1400_L1J100',   'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet', 'BW:Jet'], -1],

        # L1topo HT triggers
        ['ht700_L1HT150-J20.ETA31',     'L1_HT150-J20.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['ht700_L1HT150-J20s5.ETA31',   'L1_HT150-J20s5.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['ht1000_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1000_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1200_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1200_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1400_L1HT190-J15.ETA21',    'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1400_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],



        #####
        ######################### Analysis-specific jet trigger chains
        #####


        # Dijet invariant mass range plus y* cut triggers, mainly aimed at the dijet trigger-level analysis
        ['j0_1i2c300m500TLA',               'L1_J100',  [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j0_0i1c500m900TLA',               'L1_J100',  [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j0_1i2c200m8000TLA',              'L1_J100',  [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        ['2j220_j120',                      'L1_J100', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['2j250_j120',                      'L1_J100', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['2j275_j140',                      'L1_J100', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        # Analysis-driven multijet triggers
        ['j80_0eta240_2j60_320eta490',      'L1_J40.0ETA25_2J15.31ETA49', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['2j225_gsc250_boffperf_split_0eta240_j70_gsc120_boffperf_split_0eta240',      'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['2j225_gsc250_boffperf_split_0eta240_j70_gsc140_boffperf_split_0eta240',      'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiJet',  'BW:Jet'], -1],
        ['j80_0eta240_j60_j45_320eta490',   'L1_J40.0ETA25_2J25_J20.31ETA49', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j40_0eta490_invm400',            'L1_MJJ-400', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        # ATR-14356: L1Topo VBF chains
        ['ht300_2j40_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_HT150-JJ15.ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'],-1,['serial',-1,['ht300', '2j40_0eta490_invm700']]],
        # ATR-15044: 2017 VBF Hbb
        ['ht300_2j40_0eta490_invm700_L1HT150-J20s5.ETA31_MJJ-400-CF','L1_HT150-J20s5.ETA31_MJJ-400-CF', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet', 'BW:Jet'],-1,['serial',-1,[  'ht300', '2j40_0eta490_invm700']]],
        ['ht300_2j40_0eta490_invm700_L1HT150-J20s5.ETA31_MJJ-400-CF_AND_2j25_gsc45_bmv2c1070_split','L1_HT150-J20s5.ETA31_MJJ-400-CF', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet',  'BW:BJet', 'BW:Jet'],-1,['serial',-1,[  'ht300', '2j40_0eta490_invm700_L1HT150-J20s5.ETA31_MJJ-400-CF_AND','2j25_gsc45_bmv2c1070_split']]],

        #####
        ######################### Specialty jet trigger chains (mostly for performance studies)
        #####

        # Jet triggers to probe the EMEC IW region
        # This region is know to be more challenging
        ['j60_280eta320',       'L1_J20.28ETA31', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j60_j60_320eta490',   'L1_J20_J20.31ETA49', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        # Jet cleaning chains to probe NCB
        ['j85_cleanL',          'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j85_cleanT',          'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j85_cleanLLP',        'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        # Trigger tower (TT) jet triggers
        # See ATR-14446 for details of the request
        ['j60_TT', 'L1_J20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        #AFP + jet chains ATR-16124
        ['j10_L1AFP_A_OR_C',                        'L1_AFP_A_OR_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j20_L1AFP_A_OR_C',                        'L1_AFP_A_OR_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j20_L1AFP_A_OR_C_J12',                        'L1_AFP_A_OR_C_J12',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],

        ['j10_L1AFP_A_AND_C',                        'L1_AFP_A_AND_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j20_L1AFP_A_AND_C',                        'L1_AFP_A_AND_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j20_L1AFP_A_AND_C_J12',                        'L1_AFP_A_AND_C_J12',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],

        ['j10_320eta490_L1AFP_A_OR_C',                        'L1_AFP_A_OR_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['2j10_deta20_L1AFP_A_OR_C',                        'L1_AFP_A_OR_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j10_320eta490_L1AFP_A_AND_C',                        'L1_AFP_A_AND_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['2j10_deta20_L1AFP_A_AND_C',                        'L1_AFP_A_AND_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        
        ['j10_L1MBTS_2',                    'L1_MBTS_2', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j20_L1MBTS_2',                    'L1_MBTS_2', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j10_L1RD0_FILLED',                    'L1_RD0_FILLED', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j20_L1RD0_FILLED',                    'L1_RD0_FILLED', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j20_L1J12',                    'L1_J12', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j10_320eta490_L1MBTS_2',          'L1_MBTS_2', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2j10_deta20_L1MBTS_2',            'L1_MBTS_2', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #Low-mu run triggers
        ['j45_L1AFP_A_OR_C_J12',          'L1_AFP_A_OR_C_J12',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j45_L1AFP_A_AND_C_J12',         'L1_AFP_A_AND_C_J12',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j45_L1J12',                     'L1_J12', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #Jets for 5TeV run ATR-17162
        ['j10_0eta490_L1TE5',      'L1_TE5', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j20_0eta490_L1TE5',      'L1_TE5', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j20_0eta490_L1TE10',      'L1_TE10', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_0eta490_L1TE10',      'L1_TE10', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_0eta490_L1TE20',      'L1_TE20', [], [PhysicsStream,'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j40_0eta490_L1TE20',      'L1_TE20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        
        ['j10_L1TE5',      'L1_TE5', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j20_L1TE5',      'L1_TE5', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j20_L1TE10',      'L1_TE10', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_L1TE10',      'L1_TE10', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_L1TE20',      'L1_TE20', [], [PhysicsStream,'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j40_L1TE20',      'L1_TE20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        
        ['j50_L1J12',      'L1_J12', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j50_L1J15',      'L1_J15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j60_L1J15',      'L1_J15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j75_L1J20',      'L1_J20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j100_L1J20',      'L1_J20', [], [PhysicsStream,'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j100_L1J30',      'L1_J30', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j120_L1J30',      'L1_J30', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j150_L1J40',      'L1_J40', [], [PhysicsStream,'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        
        ['j15_320eta490_L1TE5',      'L1_TE5', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j25_320eta490_L1TE10',      'L1_TE10', [], [PhysicsStream,'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j35_320eta490_L1TE20',      'L1_TE20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j45_320eta490_L1TE30',      'L1_TE30', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j55_320eta490',      'L1_J15.31ETA49', [], [PhysicsStream,'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j75_320eta490',      'L1_J20.31ETA49', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        
        ['j45_200eta320_L1J12',      'L1_J12', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j50_200eta320_L1J12',      'L1_J12', [], [PhysicsStream,'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j60_200eta320_L1J20',      'L1_J20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j75_200eta320_L1J20',      'L1_J20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        
        ['2j10_320eta490_L1TE5',      'L1_TE5', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['2j20_320eta490_L1TE10',      'L1_TE10', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['2j30_320eta490_L1TE20',      'L1_TE20', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['2j40_320eta490_L1TE30',      'L1_TE30', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        
        ['3j50_L13J15',      'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['3j60_L13J15',      'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['3j75_L13J15',      'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['4j50_L13J15',      'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['4j60_L13J15',      'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['4j75_L14J20',      'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],


			 ]

    TriggerFlags.BjetSlice.signatures = [
   
        # 
        #  1b / 1j
        # 

        # perf / bperf
        ['j0_perf_boffperf_L1RD0_EMPTY', 'L1_RD0_EMPTY',[], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j0_perf_boffperf_L1MU10',       'L1_MU10',[], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j0_perf_boffperf_L1J12_EMPTY',  'L1_J12_EMPTY',[], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],



        # boffperf_split
        ['j35_boffperf_split',  'L1_J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_boffperf_split',  'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j150_boffperf_split', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j320_boffperf_split', 'L1_J85', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],


        #  gsc
        ['j15_gsc35_boffperf_split',   'L1_RD0_FILLED', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j35_gsc55_boffperf_split', 'L1_J15', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j60_gsc110_boffperf_split', 'L1_J20', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1], 
        ['j175_gsc225_boffperf_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc275_boffperf_split', 'L1_J75', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc300_boffperf_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc360_boffperf_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc380_boffperf_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        
        ['j175_gsc225_bmv2c1040_split', 'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleBJet', 'BW:BJet'], -1],
        ['j175_gsc225_bmv2c1050_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],        

        ['j225_gsc275_bmv2c1060_split', 'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc275_bmv2c1070_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc275_bmv2c1077_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['j225_gsc300_bmv2c1070_split', 'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc300_bmv2c1077_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['j225_gsc360_bmv2c1077_split', 'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:SingleBJet', 'BW:BJet'], -1],                   
        ['j225_gsc360_bmv2c1085_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],                   


        # 
        #  1b / 4j
        # 

        ['j50_gsc65_boffperf_split_3j50_gsc65_boffperf_split',      'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
        ['j50_gsc65_bmv2c1040_split_3j50_gsc65_boffperf_split',      'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],

        ['j55_gsc75_boffperf_split_3j55_gsc75_boffperf_split',        'L1_4J20',        [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_gsc75_bmv2c1040_split_3j55_gsc75_boffperf_split',       'L1_4J20',        [], [PhysicsStream], ['Primary:20000','RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_gsc75_bmv2c1050_split_3j55_gsc75_boffperf_split',       'L1_4J20',        [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['j60_gsc85_boffperf_split_3j60_gsc85_boffperf_split',        'L1_4J20',        [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j60_gsc85_bmv2c1050_split_3j60_gsc85_boffperf_split',       'L1_4J20',        [], [PhysicsStream], ['Primary:20000','RATE:SingleBJet', 'BW:BJet'], -1],
        ['j60_gsc85_bmv2c1060_split_3j60_gsc85_boffperf_split',       'L1_4J20',        [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['j150_gsc175_boffperf_split_j45_gsc60_boffperf_split',   'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j150_gsc175_bmv2c1060_split_j45_gsc60_bmv2c1060_split', 'L1_J100', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['j150_gsc175_bmv2c1070_split_j45_gsc60_bmv2c1070_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j110_gsc150_boffperf_split_2j35_gsc55_bmv2c1070_split_L1J85_3J30', 'L1_J85_3J30', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['j110_gsc150_boffperf_split_2j35_gsc55_bmv2c1077_split_L1J85_3J30', 'L1_J85_3J30', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j110_gsc150_boffperf_split_2j35_gsc55_boffperf_split_L1J85_3J30', 'L1_J85_3J30', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j55_gsc75_bmv2c1050_split_j55_gsc75_boffperf_split',        'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j55_gsc75_bmv2c1060_split_j55_gsc75_boffperf_split',        'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j60_gsc85_bmv2c1070_split_j60_gsc85_boffperf_split',        'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j60_gsc85_bmv2c1077_split_j60_gsc85_boffperf_split',        'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j35_boffperf_split_2j35_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],
        ['2j15_gsc35_boffperf_split_2j15_gsc35_boffperf_split_L14J15.0ETA25','L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],

        ['2j15_gsc35_bmv2c1040_split_2j15_gsc35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j15_gsc35_bmv2c1050_split_2j15_gsc35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j15_gsc35_bmv2c1060_split_2j15_gsc35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],


        ['2j25_gsc45_bmv2c1050_split_2j25_gsc45_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j25_gsc45_bmv2c1060_split_2j25_gsc45_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j35_gsc55_bmv2c1060_split_2j35_gsc55_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_gsc55_bmv2c1070_split_2j35_gsc55_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],


        #
        #  2b / 5j
        #
        ['2j15_gsc35_bmv2c1050_split_3j15_gsc35_boffperf_split', 'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j15_gsc35_bmv2c1060_split_3j15_gsc35_boffperf_split', 'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j25_gsc45_bmv2c1050_split_3j25_gsc45_boffperf_split',                 'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j25_gsc45_bmv2c1060_split_3j25_gsc45_boffperf_split',                 'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j25_gsc45_bmv2c1070_split_3j25_gsc45_boffperf_split',                 'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j25_gsc45_bmv2c1050_split_3j25_gsc45_boffperf_split_L14J15.0ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j25_gsc45_bmv2c1060_split_3j25_gsc45_boffperf_split_L14J15.0ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j25_gsc45_bmv2c1070_split_3j25_gsc45_boffperf_split_L14J15.0ETA25',   'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        #
        #  3b / 3j
        #
        ['3j65_boffperf_split_L13J35.0ETA23',        'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j50_gsc65_boffperf_split_L13J35.0ETA23',  'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j50_gsc65_bmv2c1077_split_L13J35.0ETA23', 'L1_3J35.0ETA23', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],

        #
        #  3b / 4j
        #
        ['3j15_gsc35_bmv2c1060_split_j15_gsc35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j15_gsc35_bmv2c1070_split_j15_gsc35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j15_gsc35_bmv2c1077_split_j15_gsc35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        #
        #  4b 
        #
        ['4j15_gsc35_bmv2c1077_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['4j15_gsc35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j15_gsc35_bmv2c1060_split_2j15_gsc35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j15_gsc35_bmv2c1070_split_2j15_gsc35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j15_gsc35_bmv2c1077_split_2j15_gsc35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        #
        # L1Topo VBF MJJ bjets
        #
        ['j55_gsc80_bmv2c1070_split_j45_gsc60_bmv2c1085_split_j45_320eta490', 'L1_J40.0ETA25_2J25_J20.31ETA49', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet','BW:BJet'], -1],

				# Backup ATR-16986
        ['2j35_bmv2c1040_split_2j35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c1050_split_2j35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c1060_split_2j35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c1050_split_3j35_boffperf_split', 'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c1060_split_3j35_boffperf_split', 'L1_5J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j35_bmv2c1060_split_j35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j35_bmv2c1070_split_j35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j35_bmv2c1077_split_j35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['4j35_bmv2c1077_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['4j35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c1060_split_2j35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c1070_split_2j35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c1077_split_2j35_bmv2c1085_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
			 
		#bjets for 5TeV run ATR-17162 going to Main stream
		['j35_bmv2c1070_split_0eta490_L1TE10',      'L1_TE10', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
		['j40_bmv2c1070_split_L1J12',      'L1_J12', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
		['j50_bmv2c1077_split_L1J12',      'L1_J12', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
		['j45_gsc60_bmv2c1085_split_L1J12',      'L1_J12', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
		['j55_gsc75_bmv2c1085_split_L1J15',      'L1_J15', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
		['j60_gsc85_bmv2c1085_split_L1J20',      'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
		['j60_gsc100_bmv2c1085_split_L1J20',      'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
		
		
			 ]
   

    if TriggerFlags.doFTK():
        TriggerFlags.BjetSlice.signatures = TriggerFlags.BjetSlice.signatures() + [
           # FTK boffperf chains
           ['j35_boffperf_split_FTK_L1J15_FTK', 'L1_J15_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j150_boffperf_split_FTK_L1J40_FTK', 'L1_J40_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j225_boffperf_split_FTK_L1J100_FTK',    'L1_J100_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

           ['j35_boffperf_split_FTKVtx_L1J15_FTK', 'L1_J15_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j150_boffperf_split_FTKVtx_L1J40_FTK', 'L1_J40_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j225_boffperf_split_FTKVtx_L1J100_FTK',    'L1_J100_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

           ['j35_boffperf_split_FTKRefit_L1J15_FTK', 'L1_J15_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j150_boffperf_split_FTKRefit_L1J40_FTK', 'L1_J40_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j225_boffperf_split_FTKRefit_L1J100_FTK',    'L1_J100_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

           # support chains
           ['2j35_boffperf_2j35_FTK_L14J15.0ETA25_FTK', 'L1_4J15.0ETA25_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
           ['j150_boffperf_j50_boffperf_FTK_L1J100_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

           ['2j35_boffperf_2j35_FTKVtx_L14J15.0ETA25_FTK', 'L1_4J15.0ETA25_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
           ['j150_boffperf_j50_boffperf_FTKVtx_L1J100_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

           ['2j35_boffperf_2j35_FTKRefit_L14J15.0ETA25_FTK', 'L1_4J15.0ETA25_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
           ['j150_boffperf_j50_boffperf_FTKRefit_L1J100_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

           # FTK physics chains
           ['2j35_bmv2c1050_2j35_FTK_L14J15.0ETA25_FTK', 'L1_4J15.0ETA25_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
           ['j225_bmv2c1050_FTK_L1J100_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j150_bmv2c1050_j50_bmv2c1050_FTK_L1J100_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

           ['2j35_bmv2c1050_2j35_FTKVtx_L14J15.0ETA25_FTK', 'L1_4J15.0ETA25_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
           ['j225_bmv2c1050_FTKVtx_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j150_bmv2c1050_j50_bmv2c1050_FTKVtx_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

           ['2j35_bmv2c1050_2j35_FTKRefit_L14J15.0ETA25_FTK', 'L1_4J15.0ETA25_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
           ['j225_bmv2c1050_FTKRefit_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
           ['j150_bmv2c1050_j50_bmv2c1050_FTKRefit_FTK', 'L1_J100_FTK', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
           ] # Additional FTK for BjetSlise



    TriggerFlags.METSlice.signatures = [
  # Rerun chains
        ['xe0noL1_l2fsperf',         '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_tc_lcw',  '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_tc_em',   '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_mht',     '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_mht_em',     '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_pufit',   '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe50', 'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

  ###############      
  ## L1_XE50 ####
  ###############

        ['xe90_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_pufit_wEFMu_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe95_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe95_pufit_wEFMu_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe100_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_wEFMu_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe110_L1XE50',                      'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_wEFMu_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_em_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_em_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_em_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_em_wEFMu_L1XE50',          'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_lcw_wEFMu_L1XE50',         'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],
 	['xe110_pufit_wEFMu_L1XE50',                'L1_XE50',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],

        ['xe120_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_em_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_wEFMu_L1XE50',                'L1_XE50',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],

        ['xe130_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_em_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_em_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],


    ## mht+cell ####
       # cell 70
        ['xe110_mht_xe70_L1XE50', 'L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe70_L1XE50'] ]],
	['xe120_mht_xe70_L1XE50', 'L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe70_L1XE50'] ]],

       # cell 75
        ['xe110_mht_xe75_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe75_L1XE50'] ]],
	['xe120_mht_xe75_L1XE50', 'L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe75_L1XE50'] ]],

       # cell 80
        ['xe110_mht_xe80_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe80_L1XE50'] ]],
	['xe120_mht_xe80_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe80_L1XE50'] ]],

     ## mht + trkmht ##

	['xe110_trkmht_xe80_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe80_mht_L1XE50'] ]],
	['xe110_trkmht_wEFMu_xe80_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht_wEFMu','xe80_mht_L1XE50'] ]],
	['xe120_trkmht_xe80_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe80_mht_L1XE50'] ]],

	['xe110_trkmht_xe90_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe90_mht_L1XE50'] ]],
	['xe110_trkmht_wEFMu_xe90_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht_wEFMu','xe90_mht_L1XE50'] ]],
	['xe120_trkmht_xe90_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe90_mht_L1XE50'] ]],

	['xe110_trkmht_xe100_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe100_mht_L1XE50'] ]],
	['xe110_trkmht_wEFMu_xe100_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht_wEFMu','xe100_mht_L1XE50'] ]],
	['xe120_trkmht_xe100_mht_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe100_mht_L1XE50'] ]],





  ##################
  ## L1_XE55 #######
  ##################

	['xe90_pufit_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_pufit_wEFMu_L1XE55',                 'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe95_pufit_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe95_pufit_wEFMu_L1XE55',                 'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe100_pufit_L1XE55',                'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_wEFMu_L1XE55',                'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe110_L1XE55',                      'L1_XE55',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],
        ['xe110_wEFMu_L1XE55',                'L1_XE55',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_L1XE55',                   'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_em_L1XE55',                   'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_em_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_tc_em_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_em_wEFMu_L1XE55',          'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
	['xe110_tc_lcw_L1XE55',               'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_lcw_wEFMu_L1XE55',         'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
	['xe110_pufit_L1XE55',                'L1_XE55',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],
 	['xe110_pufit_wEFMu_L1XE55',                'L1_XE55',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],

        ['xe120_L1XE55',                      'L1_XE55',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_em_L1XE55',                   'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1XE55',               'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_L1XE55',                'L1_XE55',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_wEFMu_L1XE55',              'L1_XE55',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],

        ['xe130_L1XE55','L1_XE55',[], [PhysicsStream], ['Primary:17000','RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_em_L1XE55',                   'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_em_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_lcw_L1XE55',               'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

     ## mht+cell ####

      # cell 70
        ['xe110_mht_xe70_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe70_L1XE55'] ]],
	['xe120_mht_xe70_L1XE55','L1_XE55',  ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe70_L1XE55'] ]],
	['xe130_mht_xe70_L1XE55','L1_XE55',  ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_mht','xe70_L1XE55'] ]],

      # cell 75
        ['xe110_mht_xe75_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe75_L1XE55'] ]],
	['xe120_mht_xe75_L1XE55','L1_XE55',  ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe75_L1XE55'] ]],
	['xe130_mht_xe75_L1XE55','L1_XE55',  ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_mht','xe75_L1XE55'] ]],

      # cell 80
        ['xe100_mht_xe80_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe80_L1XE55'] ]],
        ['xe110_mht_xe80_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe80_L1XE55'] ]],
	['xe120_mht_xe80_L1XE55','L1_XE55',  ['L1_XE55','L1_XE55'], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe80_L1XE55'] ]],
	['xe130_mht_xe80_L1XE55','L1_XE55',  ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_mht','xe80_L1XE55'] ]],


     ## mht + trkmht ##

	['xe110_trkmht_xe80_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe80_mht_L1XE55'] ]],
	['xe110_trkmht_wEFMu_xe80_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht_wEFMu','xe80_mht_L1XE55'] ]],
	['xe120_trkmht_xe80_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe80_mht_L1XE55'] ]],
	['xe130_trkmht_xe80_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_trkmht','xe80_mht_L1XE55'] ]],

	['xe110_trkmht_xe90_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe90_mht_L1XE55'] ]],
	['xe110_trkmht_wEFMu_xe90_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht_wEFMu','xe90_mht_L1XE55'] ]],
	['xe120_trkmht_xe90_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe90_mht_L1XE55'] ]],
	['xe130_trkmht_xe90_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_trkmht','xe90_mht_L1XE55'] ]],

	['xe110_trkmht_xe100_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe100_mht_L1XE55'] ]],
	['xe110_trkmht_wEFMu_xe100_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht_wEFMu','xe100_mht_L1XE55'] ]],
	['xe120_trkmht_xe100_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe100_mht_L1XE55'] ]],
	['xe130_trkmht_xe100_mht_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_trkmht','xe100_mht_L1XE55'] ]],

 
  #################
  ## L1_XE60 ######
  #################

	['xe90_pufit_L1XE60',                'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_pufit_wEFMu_L1XE60',                'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe95_pufit_L1XE60',                'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe95_pufit_wEFMu_L1XE60',                'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe100_pufit_L1XE60',                 'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_wEFMu_L1XE60',               'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

 	['xe110_pufit_L1XE60',                 'L1_XE60',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_wEFMu_L1XE60',               'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe120_L1XE60',                'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],  
        ['xe120_wEFMu_L1XE60',               'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1XE60',                   'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
        ['xe120_mht_wEFMu_L1XE60',           'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe120_mht_em_L1XE60',                   'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
        ['xe120_mht_em_wEFMu_L1XE60',           'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_L1XE60',         'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_wEFMu_L1XE60',         'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1XE60',               'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_wEFMu_L1XE60',        'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe120_pufit_L1XE60',                 'L1_XE60',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_wEFMu_L1XE60',         'L1_XE60', [], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],

        ['xe130_L1XE60',                'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],  
	['xe130_mht_L1XE60',                   'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
	['xe130_mht_em_L1XE60',                   'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
        ['xe130_tc_em_L1XE60',         'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
	['xe130_tc_lcw_L1XE60',               'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],


    ### mht+cell ####

      # cell 70
	['xe110_mht_xe70_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe70_L1XE60'] ]],
	['xe120_mht_xe70_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe70_L1XE60'] ]],
	['xe130_mht_xe70_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_mht','xe70_L1XE60'] ]],

      # cell 75
	['xe110_mht_xe75_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe75_L1XE60'] ]],
	['xe120_mht_xe75_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe75_L1XE60'] ]],
	['xe130_mht_xe75_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_mht','xe75_L1XE60'] ]],

      # cell 80
	['xe100_mht_xe80_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe80_L1XE60'] ]],
	['xe110_mht_xe80_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe80_L1XE60'] ]],
	['xe120_mht_xe80_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe80_L1XE60'] ]],
	['xe130_mht_xe80_L1XE60', 'L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_mht','xe80_L1XE60'] ]],

     ## mht + trkmht ##

	['xe110_trkmht_xe80_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe80_mht_L1XE60'] ]],
	['xe120_trkmht_xe80_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe80_mht_L1XE60'] ]],
	['xe120_trkmht_wEFMu_xe80_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht_wEFMu','xe80_mht_L1XE60'] ]],
	['xe130_trkmht_xe80_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_trkmht','xe80_mht_L1XE60'] ]],
	['xe140_trkmht_xe80_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe140_trkmht','xe80_mht_L1XE60'] ]],

	['xe110_trkmht_xe90_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe90_mht_L1XE60'] ]],
	['xe120_trkmht_xe90_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe90_mht_L1XE60'] ]],
	['xe120_trkmht_wEFMu_xe90_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht_wEFMu','xe90_mht_L1XE60'] ]],
	['xe130_trkmht_xe90_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_trkmht','xe90_mht_L1XE60'] ]],
	['xe140_trkmht_xe90_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe140_trkmht','xe90_mht_L1XE60'] ]],

	['xe110_trkmht_xe100_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe100_mht_L1XE60'] ]],
	['xe120_trkmht_xe100_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe100_mht_L1XE60'] ]],
	['xe120_trkmht_wEFMu_xe100_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht_wEFMu','xe100_mht_L1XE60'] ]],
	['xe130_trkmht_xe100_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_trkmht','xe100_mht_L1XE60'] ]],
	['xe140_trkmht_xe100_mht_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe140_trkmht','xe100_mht_L1XE60'] ]],


   ####################
   ## L1_XE70 #########
   ####################

	['xe90_pufit_L1XE70',                 'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_pufit_wEFMu_L1XE70',           'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe95_pufit_L1XE70',                 'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe95_pufit_wEFMu_L1XE70',           'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe100_pufit_L1XE70',                 'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_wEFMu_L1XE70',          'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe110_pufit_L1XE70',                 'L1_XE70',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_wEFMu_L1XE70',          'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

 	['xe120_mht_L1XE70',                   'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
	['xe120_mht_em_L1XE70',                   'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
        ['xe120_tc_em_L1XE70',         'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
	['xe120_tc_lcw_L1XE70',               'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
	['xe120_pufit_L1XE70',                 'L1_XE70',[], [PhysicsStream], ['Primary:20000','RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_wEFMu_L1XE70',          'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

    ## mht+cell ##

      # cell 65
	['xe100_mht_xe65_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe65_L1XE70'] ]],
	['xe110_mht_xe65_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe65_L1XE70'] ]],

      # cell 70
	['xe100_mht_xe70_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe70_L1XE70'] ]],
	['xe110_mht_xe70_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe70_L1XE70'] ]],

      # cell 75
	['xe100_mht_xe75_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe75_L1XE70'] ]],
	['xe110_mht_xe75_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe75_L1XE70'] ]],

      # cell 80
	['xe100_mht_xe80_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe80_L1XE70'] ]],
	['xe110_mht_xe80_L1XE70', 'L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe80_L1XE70'] ]],
    

     ## mht + trkmht ##

	['xe110_trkmht_xe100_mht_L1XE70','L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe100_mht_L1XE70'] ]],
	['xe120_trkmht_xe100_mht_L1XE70','L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe100_mht_L1XE70'] ]],
	['xe120_trkmht_wEFMu_xe100_mht_L1XE70','L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht_wEFMu','xe100_mht_L1XE70'] ]],
	['xe130_trkmht_xe100_mht_L1XE70','L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_trkmht','xe100_mht_L1XE70'] ]],
	['xe140_trkmht_xe100_mht_L1XE70','L1_XE70', ['L1_XE70','L1_XE70'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe140_trkmht','xe100_mht_L1XE70'] ]],

 
   ##############################
   ### L1Topo met chains ########
   ##############################

     ## L1_KF-XE60 ###

        ['xe90_pufit_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe110_L1KF-XE60',        'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_L1KF-XE60',    'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_em_L1KF-XE60',    'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_tc_lcw_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_tc_em_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe120_L1KF-XE60',        'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1KF-XE60',    'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_em_L1KF-XE60',    'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe100_mht_xe70_L1KF-XE60', 'L1_KF-XE60', ['L1_KF-XE60','L1_KF-XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe70_L1KF-XE60'] ]],
	['xe110_mht_xe70_L1KF-XE60', 'L1_KF-XE60', ['L1_KF-XE60','L1_KF-XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe70_L1KF-XE60'] ]],
	['xe120_mht_xe70_L1KF-XE60', 'L1_KF-XE60', ['L1_KF-XE60','L1_KF-XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe70_L1KF-XE60'] ]],


     ## L1_KF-XE65 ######

        ['xe90_pufit_L1KF-XE65',        'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe100_pufit_L1KF-XE65',        'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe110_L1KF-XE65',        'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_L1KF-XE65',    'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_em_L1KF-XE65',    'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_tc_lcw_L1KF-XE65', 'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_tc_em_L1KF-XE65', 'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_L1KF-XE65',        'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        
        ['xe120_L1KF-XE65',        'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1KF-XE65',    'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_em_L1KF-XE65',    'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1KF-XE65', 'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_L1KF-XE65', 'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe100_mht_xe70_L1KF-XE65', 'L1_KF-XE65', ['L1_KF-XE65','L1_KF-XE65'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe70_L1KF-XE65'] ]],
	['xe110_mht_xe70_L1KF-XE65', 'L1_KF-XE65', ['L1_KF-XE65','L1_KF-XE65'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe70_L1KF-XE65'] ]],
	['xe120_mht_xe70_L1KF-XE65', 'L1_KF-XE65', ['L1_KF-XE65','L1_KF-XE65'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe70_L1KF-XE65'] ]],


     ## L1_KF-XE75 #######

        ['xe90_pufit_L1KF-XE75',        'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe100_pufit_L1KF-XE75',        'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe110_L1KF-XE75',        'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_L1KF-XE75',    'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_em_L1KF-XE75',    'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_tc_lcw_L1KF-XE75', 'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_tc_em_L1KF-XE75', 'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_L1KF-XE75',        'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe120_L1KF-XE75',        'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1KF-XE75',    'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_em_L1KF-XE75',    'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1KF-XE75', 'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_L1KF-XE75', 'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

	['xe100_mht_xe70_L1KF-XE75', 'L1_KF-XE75', ['L1_KF-XE75','L1_KF-XE75'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_mht','xe70_L1KF-XE75'] ]],
	['xe110_mht_xe70_L1KF-XE75', 'L1_KF-XE75', ['L1_KF-XE75','L1_KF-XE75'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht','xe70_L1KF-XE75'] ]],
	['xe120_mht_xe70_L1KF-XE75', 'L1_KF-XE75', ['L1_KF-XE75','L1_KF-XE75'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht','xe70_L1KF-XE75'] ]],

### ATR-16713 mht+pufit

        ['xe100_trkmht_xe100_pufit_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_trkmht','xe100_pufit_L1XE55'] ]],
        ['xe100_trkmht_xe100_pufit_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe100_trkmht','xe100_pufit_L1XE60'] ]],
        ['xe110_trkmht_xe100_pufit_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe100_pufit_L1XE55'] ]],
        ['xe110_trkmht_xe100_pufit_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe100_pufit_L1XE60'] ]],
        ['xe120_trkmht_xe100_pufit_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe100_pufit_L1XE55'] ]],
        ['xe120_trkmht_xe100_pufit_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe100_pufit_L1XE60'] ]],

        ['xe110_trkmht_xe110_mht_xe80_L1XE55','L1_XE55', ['L1_XE55','L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe110_mht','xe80_L1XE55'] ]],
        ['xe110_trkmht_xe110_mht_xe80_L1XE60','L1_XE60', ['L1_XE60','L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_trkmht','xe110_mht','xe80_L1XE60'] ]],
        ['xe120_trkmht_xe110_mht_xe80_L1XE55','L1_XE55', ['L1_XE55','L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe110_mht','xe80_L1XE55'] ]],
        ['xe120_trkmht_xe110_mht_xe80_L1XE60','L1_XE60', ['L1_XE60','L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_trkmht','xe110_mht','xe80_L1XE60'] ]],
	
			 ]
    if TriggerFlags.doFTK():
        TriggerFlags.METSlice.signatures = TriggerFlags.METSlice.signatures() + [
           # FTK MET chains
            ['xe0noL1_l2fsperf_trkmht_FTK', '', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe35_trkmht_FTK', 'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe100_trkmht_FTK_L1XE50', 'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_L1XE50', 'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_wEFMu_L1XE50', 'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe120_trkmht_FTK_L1XE50', 'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_L1XE55', 'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_wEFMu_L1XE55', 'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe120_trkmht_FTK_L1XE55', 'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe130_trkmht_FTK_L1XE55', 'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_L1XE60', 'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
            ['xe120_trkmht_FTK_L1XE60', 'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
            ['xe120_trkmht_FTK_wEFMu_L1XE60', 'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe130_trkmht_FTK_L1XE60', 'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe140_trkmht_FTK_L1XE60', 'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe120_trkmht_FTK_L1XE70', 'L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
## L1topo            
            ['xe100_trkmht_FTK_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_L1KF-XE60', 'L1_KF-XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe100_trkmht_FTK_L1KF-XE65', 'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_L1KF-XE65', 'L1_KF-XE65',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe100_trkmht_FTK_L1KF-XE75', 'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
            ['xe110_trkmht_FTK_L1KF-XE75', 'L1_KF-XE75',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

            ]

    
    TriggerFlags.TauSlice.signatures = [
        # Energy calibration chain
        ['tau0_perf_ptonly_L1TAU12',               'L1_TAU12', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau0_perf_ptonly_L1TAU60',               'L1_TAU60', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        # Run-I comparison
        ['tau25_idperf_track',                     'L1_TAU12IM', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Tau', 'BW:ID'], -1],
        ['tau25_idperf_tracktwo',                  'L1_TAU12IM', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Tau', 'BW:ID'], -1],
        # Run-II - No BDT: main track-based items
        ['tau25_perf_track',                       'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_perf_tracktwo',                    'L1_TAU12IM', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_perf_tracktwo_L1TAU12',            'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_loose1_tracktwo',                  'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_medium1_tracktwo',                 'L1_TAU12IM', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_tight1_tracktwo',                  'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_medium1_tracktwo_L1TAU12',         'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        # Run-II - High-pT variations
        ['tau35_loose1_tracktwo',                  'L1_TAU20IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau35_medium1_tracktwo',                 'L1_TAU20IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau50_medium1_tracktwo_L1TAU12',         'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau60_medium1_tracktwo',                 'L1_TAU40', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau80_medium1_tracktwo',                 'L1_TAU40', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau80_medium1_tracktwo_L1TAU60',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau125_medium1_tracktwo',                'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_medium1_tracktwo',                'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_perf_tracktwo',                   'L1_TAU60', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_idperf_track',                    'L1_TAU60', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Tau', 'BW:ID'], -1],
        ['tau160_idperf_tracktwo',                 'L1_TAU60', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Tau', 'BW:ID'], -1],

        # chains for 2e34 backup (ATR-15225)
        ['tau160_medium1_tracktwo_L1TAU100',       'L1_TAU100', [], [PhysicsStream], ['Primary:20000','RATE:SingleTau', 'BW:Tau'], -1],
        ['tau200_medium1_tracktwo_L1TAU100',       'L1_TAU100', [], [PhysicsStream], ['Primary:20000','RATE:SingleTau', 'BW:Tau'], -1],

        ['tau0_perf_ptonly_L1TAU100',       'L1_TAU100', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_idperf_tracktwo_L1TAU100',       'L1_TAU100', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_perf_tracktwo_L1TAU100',       'L1_TAU100', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],

        ['tau35_perf_tracktwo_tau25_perf_tracktwo', 'L1_TAU20IM_2TAU12IM' , ['L1_TAU20IM','L1_TAU12IM'],[PhysicsStream, 'express'], ['RATE:MultiTau','BW:Tau'], -1],
        
        ['tau35_loose1_tracktwo_tau25_loose1_tracktwo',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_loose1_tracktwo","tau25_loose1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau40_medium1_tracktwo_tau35_medium1_tracktwo',   'L1_TAU25IM_2TAU20IM_2J25_3J20',['L1_TAU25IM','L1_TAU20IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau40_medium1_tracktwo","tau35_medium1_tracktwo"]]],
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM_4J12',   'L1_TAU20IM_2TAU12IM_4J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],

        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM',   'L1_TAU20IM_2TAU12IM',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],

	# l1topo ditau chains
	['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20ITAU12I-J25',   'L1_TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]]	,
	['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],

        # tau+tau topo_start_from
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],True],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_notautsf_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],False],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],True],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_notautsf_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],False],

	['tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],

        # chains for 2e34 backup (ATR-15225)
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],


        ['tau80_medium1_tracktwo_L1TAU60_tau35_medium1_tracktwo_L1TAU12IM_L1TAU60_DR-TAU20ITAU12I',   'L1_TAU60_DR-TAU20ITAU12I',['L1_TAU60','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau80_medium1_tracktwo_L1TAU60","tau35_medium1_tracktwo_L1TAU12IM"]]]	,
        # non-L1topo backup (ATR-15757)
        ['tau80_medium1_tracktwo_L1TAU60_tau60_medium1_tracktwo_L1TAU40',   'L1_TAU60_2TAU40',['L1_TAU60','L1_TAU40'], [PhysicsStream], ['Primary:20000','RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau80_medium1_tracktwo_L1TAU60","tau60_medium1_tracktwo_L1TAU40"]]]       ,

        #non-L1topo backup - ATR-16230
        ['2tau35_medium1_tracktwo_L12TAU20IM_3J20',  'L1_2TAU20IM_3J20', ['L1_TAU20IM', 'L1_TAU20IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1],
        ['2tau35_medium1_tracktwo',                   'L1_2TAU20IM_J25_3J20', ['L1_TAU20IM','L1_TAU20IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1],
        ['tau40_medium1_tracktwo_tau25_medium1_tracktwo',   'L1_TAU25IM_2TAU12IM_J25_3J12',['L1_TAU25IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau40_medium1_tracktwo","tau25_medium1_tracktwo"]]],

        # High-pT DiTau seeding off of single-tau
        ['tau80_medium1_tracktwo_L1TAU60_tau50_medium1_tracktwo_L1TAU12', 'L1_TAU60',['L1_TAU60','L1_TAU12'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau80_medium1_tracktwo_L1TAU60","tau50_medium1_tracktwo_L1TAU12"]]],
        ['tau125_medium1_tracktwo_tau50_medium1_tracktwo_L1TAU12', 'L1_TAU60',['L1_TAU60','L1_TAU12'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau125_medium1_tracktwo","tau50_medium1_tracktwo_L1TAU12"]]],

        ['tau35_perf_tracktwo_tau25_perf_tracktwo_ditauM', 'L1_TAU20IM_2TAU12IM' , ['L1_TAU20IM','L1_TAU12IM'],[PhysicsStream, 'express'], ['RATE:MultiTau','BW:Tau'], -1],
        ['tau60_medium1_tracktwo_tau35_medium1_tracktwo','L1_TAU40_2TAU20IM',['L1_TAU40','L1_TAU20IM'],[PhysicsStream],['RATE:MultiTau', 'BW:Tau'],-1,['serial',-1,["tau60_medium1_tracktwo","tau35_medium1_tracktwo"]]],


        # photon+pion chains (ATR-13525) 
        ['tau25_singlepion_tracktwo',                    'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_dikaonmass_tracktwo',                    'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

	#tau + photon (ATR-13841)
        ['tau25_dipion1_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_dipion2_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_kaonpi1_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_kaonpi2_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

        ['tau160_medium1HighptL_tracktwo',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_medium1HighptM_tracktwo',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_medium1HighptH_tracktwo',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        
			 ]

    if TriggerFlags.doFTK():
            TriggerFlags.TauSlice.signatures = TriggerFlags.TauSlice.signatures() + [
                ['tau12_idperf_FTK',              'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_perf_FTK',                'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_perf0_FTK',               'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_perf_FTKNoPrec',          'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_perf0_FTKNoPrec',         'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_medium0_FTK',             'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_medium1_FTK',             'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_medium0_FTKNoPrec',       'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau12_medium1_FTKNoPrec',       'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

                # standard single tau chains seeded from L1 FTK tau seeds for monitoring in CPS
                ['tau25_idperf_tracktwo_L1TAU12IM_FTK',              'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau25_perf_tracktwo_L1TAU12IM_FTK',                'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
                ['tau25_medium1_tracktwo_L1TAU12IM_FTK',             'L1_TAU12IM_FTK', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

                           ]

    TriggerFlags.EgammaSlice.signatures = [

        #Performance:
        ['e26_lhtight_idperf',        'L1_EM22VHI',    [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        ['e28_lhtight_idperf',        'L1_EM24VHI',    [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        ['e26_lhtight_idperf_L1EM22VHIM',        'L1_EM22VHIM',    [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        ['e28_lhtight_idperf_L1EM24VHIM',        'L1_EM24VHIM',    [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],

        ['2g3_loose_dPhi15_L12EM3_VTE50', 'L1_2EM3_VTE50', [], [PhysicsStream],['RATE:SinglePhoton','BW:Egamma'],-1],
        ['2g3_loose_L12EM3_VTE50', 'L1_2EM3_VTE50', [], [PhysicsStream],['RATE:SinglePhoton','BW:Egamma'],-1],
        ['2g3_medium_dPhi15_L12EM3_VTE50', 'L1_2EM3_VTE50', [], [PhysicsStream],['RATE:SinglePhoton','BW:Egamma'],-1],

        ['g0_perf_L1EM3_EMPTY',                  'L1_EM3_EMPTY', [], [PhysicsStream], ['RATE:SinglePhoton',   'BW:Egamma'], -1],        
        ['e0_perf_L1EM3_EMPTY',                  'L1_EM3_EMPTY', [], [PhysicsStream], ['RATE:SinglePhoton',   'BW:Egamma'], -1],        

        ['g10_loose',                            'L1_EM7',  [], [PhysicsStream,'express'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_loose_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['g10_etcut',                     'L1_EM7',   [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_etcut_L1EM12',              'L1_EM12',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['g3_loose_larpeb',                      'L1_EM3',     [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g12_loose_larpeb',                      'L1_EM10VH',     [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
	['g20_loose_larpeb_L1EM15',              'L1_EM15', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g40_loose_larpeb',                     'L1_EM24VHI', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g60_loose_larpeb',                     'L1_EM24VHI', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g80_loose_larpeb',                     'L1_EM24VHI', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],        
        ['g200_etcut',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g250_etcut',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g300_etcut',                    'L1_EM22VHI', [], [PhysicsStream], ['Primary:17000','RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g300_etcut_L1EM24VHI',          'L1_EM24VHI', [], [PhysicsStream], ['Primary:20000','RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g300_etcut_L1EM24VHIM',          'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SinglePhoton', 'BW:Egamma'],-1],

        ## Aranxtas request, to be run in rerun mode
        ['g22_tight',                             'L1_EM15VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g25_medium_L1EM22VHI', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM22VHI', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g45_tight_L1EM22VHI', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['e140_etcut',  'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_etcut_L1EM24VHIM',  'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhvloose_nod0', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

	['e160_etcut',  'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e160_etcut_L1EM24VHIM',  'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e180_etcut',  'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e180_etcut_L1EM24VHIM',  'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['g0_hiptrt_L1EM22VHI',                 'L1_EM22VHI', [], [PhysicsStream], ['Primary:17000','RATE:SinglePhoton', 'BW:Egamma'], -1],
        ['g0_hiptrt_L1EM24VHI',                 'L1_EM24VHI', [], [PhysicsStream], ['Primary:20000','RATE:SinglePhoton', 'BW:Egamma'], -1],
        ['g0_hiptrt_L1EM24VHIM',                'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SinglePhoton', 'BW:Egamma'], -1],

        # PS-ed trigger to supmbined chains
        ['g15_loose_L1EM7',               'L1_EM7',   [], [PhysicsStream,'express'], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g20_loose_L1EM12',              'L1_EM12',  [], [PhysicsStream,'express'], ['RATE:SinglePhoton', 'BW:Egamma'], -1], # pass through
        ['g40_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g45_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g50_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g80_loose',                     'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g100_loose',                    'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 

        # Rerun mode and PS
        ['g25_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream,'express'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream,'express'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g60_loose',                     'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g70_loose',                     'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        # Rerun mode
        ['g10_medium',                    'L1_EM7',   [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g15_loose_L1EM3',               'L1_EM3',   [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_loose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g40_tight',                     'L1_EM20VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g45_tight',                     'L1_EM20VHI',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g50_loose',                     'L1_EM15VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g60_loose_L1EM15VH',            'L1_EM15VH', [], [PhysicsStream, 'express'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

	# ATR-16089
	['g25_tight_L1EM20VH',            'L1_EM20VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
	['g15_loose_L1EM8VH',             'L1_EM8VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['2g20_tight_icalovloose_L12EM15VHI',           'L1_2EM15VHI', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g20_tight_icalotight_L12EM15VHI',            'L1_2EM15VHI', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g22_tight_L12EM15VHI',                   'L1_2EM15VHI', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g22_tight_icalovloose_L12EM15VHI',           'L1_2EM15VHI', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g22_tight_icalovloose',           'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g22_tight_icalotight_L12EM15VHI',            'L1_2EM15VHI', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g22_tight_icalotight',            'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g50_loose',                              'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g60_loose_L12EM15VH',                    'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],  
        ['2g50_loose_L12EM20VH',                    'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g60_loose_L12EM20VH',                    'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['g50_loose_L1EM20VH',                     'L1_EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['g60_loose_L1EM20VH',                     'L1_EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ##Adding tight diphoton triggers (ATR-10762)
        ['2g22_tight',                              'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g25_tight_L12EM20VH',                    'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['3g20_loose',                              'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g25_loose_g15_loose',                    'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g25_loose_g20_loose',                    'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['3g25_loose',                    'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        #prescaled
        ['2g20_loose_L12EM15',                      'L1_2EM15', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g20_loose',                   'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 


        ['2g10_loose',                           'L1_2EM7', [], [PhysicsStream,'express'], ['RATE:MultiPhoton', 'BW:Egamma'],-1],

        ['e17_lhloose_cutd0dphideta_L1EM15',     'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nod0_L1EM15',              'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nodeta_L1EM15',            'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nodphires_L1EM15',         'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #nod0 chains:
        ['e24_lhvloose_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e26_lhvloose_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e28_lhvloose_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e26_lhmedium_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e28_lhmedium_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  

        # ATR-17148
        ['e9_lhvloose_nod0',                          'L1_EM3',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e9_lhvloose_nod0_L1EM7',                    'L1_EM7',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e9_lhvloose_nod0_L1EM8VH',                  'L1_EM8VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #ATR-16981
        ['e24_lhtight_nod0_ivarloose',                'L1_EM20VHI',    [], [PhysicsStream, 'express'], ['RATE:SingleElectron', 'BW:Egamma'],-1],  

        ['e26_lhtight_nod0_e15_etcut_L1EM7_Zee',    	  'L1_EM22VHI', ['L1_EM22VHI','L1_EM7'], [PhysicsStream], ['Primary:17000','RATE:MultiElectron', 'BW:Egamma'],-1 ],
        ['e28_lhtight_nod0_e15_etcut_L1EM7_Zee',    	  'L1_EM24VHI', ['L1_EM24VHI','L1_EM7'], [PhysicsStream, 'express'], ['Primary:20000','RATE:MultiElectron', 'BW:Egamma'],-1 ],
        ['e26_lhtight_nod0_L1EM22VHIM_e15_etcut_L1EM7_Zee', 'L1_EM22VHIM',  ['L1_EM22VHIM','L1_EM7'], [PhysicsStream], ['Primary:17000','RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_L1EM24VHIM_e15_etcut_L1EM7_Zee', 'L1_EM24VHIM',  ['L1_EM24VHIM','L1_EM7'], [PhysicsStream], ['Primary:20000','RATE:MultiElectron', 'BW:Egamma'],-1],

        # Jpsiee T&P LH-based nod0
        ['e5_lhtight_nod0_e4_etcut',         'L1_2EM3',      [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0_e4_etcut_Jpsiee',  'L1_2EM3',      [], [PhysicsStream, 'express'], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e9_lhtight_nod0_e4_etcut_Jpsiee',  'L1_EM7_2EM3',  ['L1_EM7','L1_EM3'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0_e9_etcut_Jpsiee',  'L1_EM7_2EM3',  ['L1_EM3','L1_EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e14_lhtight_nod0_e4_etcut_Jpsiee', 'L1_EM12_2EM3', ['L1_EM12','L1_EM3'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0_e14_etcut_Jpsiee', 'L1_EM12_2EM3', ['L1_EM3','L1_EM12'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],

        # L1Topo JPSI prescaled performance:
        ['e5_lhtight_nod0_e4_etcut_L1JPSI-1M5',              'L1_JPSI-1M5',      ['L1_EM3','L1_EM3'],  [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1, True],
        ['e5_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5',       'L1_JPSI-1M5',      ['L1_EM3','L1_EM3'],  [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1, True],
        ['e9_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM7',   'L1_JPSI-1M5-EM7',  ['L1_EM7','L1_EM3'],  [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1, True],
        ['e5_lhtight_nod0_e9_etcut_Jpsiee_L1JPSI-1M5-EM7',   'L1_JPSI-1M5-EM7',  ['L1_EM3','L1_EM7'],  [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1, True],
        ['e14_lhtight_nod0_e4_etcut_Jpsiee_L1JPSI-1M5-EM12', 'L1_JPSI-1M5-EM12', ['L1_EM12','L1_EM3'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1, True],
        ['e5_lhtight_nod0_e14_etcut_Jpsiee_L1JPSI-1M5-EM12', 'L1_JPSI-1M5-EM12', ['L1_EM3','L1_EM12'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1, True],

        # Supporting trigger
        ['e0_perf_L1EM15',              'L1_EM15',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['g0_perf_L1EM15',                'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['e5_lhloose_idperf',                   'L1_EM3',       [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],

        # extra id perf chains for TRT HT studies                                                                                               

        ['e5_lhtight_idperf',                     'L1_EM3',       [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        ['e10_lhtight_idperf',                     'L1_EM7',       [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],

        ['e24_lhmedium_idperf_L1EM20VH',        'L1_EM20VH',    [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        # Single electron triggers
        ['e17_lhmedium_nod0_L1EM15',                    'L1_EM15',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_L1EM18VHI',                'L1_EM18VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e28_lhtight_nod0_noringer_ivarloose', 'L1_EM24VHI', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_noringer_ivarloose_L1EM24VHIM', 'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhmedium_nod0', 'L1_EM22VHI', [], [PhysicsStream], ['Primary:17000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['g140_loose',        'L1_EM24VHI', [], [PhysicsStream], ['Primary:17000','RATE:SinglePhoton','BW:Egamma'],-1],
        ['g140_tight',        'L1_EM24VHI', [], [PhysicsStream], ['Primary:20000','RATE:SinglePhoton','BW:Egamma'],-1],
        ['g140_tight_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SinglePhoton','BW:Egamma'],-1],
        ['g160_loose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1],
        ['g180_loose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1],
        ['g200_loose',        'L1_EM24VHI', [], [PhysicsStream, 'express'], ['Primary:20000','RATE:SinglePhoton','BW:Egamma'],-1],
        ['g200_loose_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SinglePhoton','BW:Egamma'],-1],
        ['e140_lhloose_nod0', 'L1_EM22VHI', [], [PhysicsStream], ['Primary:17000','RATE:SingleElectron', 'BW:Egamma'],-1],

        #2016 egamma primaries
        ['2e17_lhloose_nod0_L12EM15',    									'L1_2EM15',     [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1 ],
        ['e5_lhloose_nod0',             									'L1_EM3',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e5_lhloose_nod0_idperf',       									'L1_EM3',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e10_lhvloose_nod0_L1EM7',       								'L1_EM7',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e15_lhvloose_nod0_L1EM7',       								'L1_EM7',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e20_lhvloose_nod0_L1EM12',       								'L1_EM12',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e25_lhvloose_nod0_L1EM15',  										'L1_EM15',	    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e30_lhvloose_nod0_L1EM15',  										'L1_EM15',	    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e40_lhvloose_nod0_L1EM15',  										'L1_EM15',	    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e50_lhvloose_nod0_L1EM15',  										'L1_EM15',	    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e70_lhvloose_nod0',  														'L1_EM24VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e80_lhvloose_nod0',  														'L1_EM24VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e100_lhvloose_nod0', 														'L1_EM24VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e120_lhvloose_nod0', 														'L1_EM24VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e80_lhmedium_nod0',              'L1_EM22VHI',   [], [PhysicsStream], ['Primary:17000','RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e80_lhmedium_nod0_L1EM24VHI',    'L1_EM24VHI',   [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1 ],

        #Low-mu run triggers
        ['e15_lhloose_nod0_L1EM12',  'L1_EM12', [], [PhysicsStream], ['RATE:SingleElectron','BW:Egamma'], -1],
        ['e15_lhvloose_nod0_L1EM12', 'L1_EM12', [], [PhysicsStream], ['RATE:SingleElectron','BW:Egamma'], -1],

        # Prescaled triggers
        # Rate = 1 Hz each
        ['e5_etcut',                             'L1_EM3',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e10_etcut_L1EM7',                      'L1_EM7',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e15_etcut_L1EM7',                      'L1_EM7',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e20_etcut_L1EM12',                     'L1_EM12',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],         
        ['e25_etcut_L1EM15',                     'L1_EM15',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e30_etcut_L1EM15',                     'L1_EM15',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e40_etcut_L1EM15',                     'L1_EM15',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e50_etcut_L1EM15',                     'L1_EM15',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e60_etcut',                            'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e70_etcut',                            'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_etcut',                            'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e100_etcut',                           'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e120_etcut',                           'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e200_etcut',                           'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 

	['e5_lhvloose_nod0',                         'L1_EM3',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        # use coherent PS with lhvloose and lhvloose_nod0 variants below:


        ['e12_lhvloose_nod0_L1EM10VH',               'L1_EM10VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e17_lhvloose_nod0',                        'L1_EM15VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
	['e20_lhvloose_nod0',                        'L1_EM15VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e40_lhvloose_nod0',                       'L1_EM20VH',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e60_lhvloose_nod0',                        'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        #Single electron/chaing for testing mis-ant robust LH ID
        ['e24_lhmedium_nod0_L1EM18VHI',            'L1_EM18VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #New nod0 chains:
        ['e24_lhtight_nod0_L1EM20VH',                 'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 


	['e7_lhmedium_nod0',                           'L1_EM3',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],        
	['e9_lhloose_nod0',                           'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
	['e9_lhmedium_nod0',                          'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhmedium_nod0',                        'L1_EM8VH',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhmedium_nod0',                         'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhmedium_nod0_ivarloose_L1EM15VHI',     'L1_EM15VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 

        ['e20_lhmedium_nod0',                         'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_nod0_L1EM15VH',                'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhloose_nod0',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e70_lhloose_nod0',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhloose_nod0',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhloose_nod0_L1EM24VHI',                          'L1_EM24VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0',                           'L1_EM3',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
				['e5_lhtight_nod0_e4_etcut_Jpsiee_L1RD0_FILLED',       'L1_RD0_FILLED',      ['L1_EM3','L1_EM3'],  [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1, True],

        ['e9_lhtight_nod0',                           'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e14_lhtight_nod0',                          'L1_EM12',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0',              'L1_EM22VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],


# Start up menu
        ['e26_lhloose_nod0',              'L1_EM22VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

         # Supporting triggers
         
        ##########
        # Single photon triggers        
        ['g120_loose',                    'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 

        # rerun mode


        ['e12_lhloose_cutd0dphideta_L1EM10VH',   'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nod0_L1EM10VH',                 'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nodeta_L1EM10VH',          'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nodphires_L1EM10VH',       'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nod0',                          'L1_EM8VH',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],


        ['e17_lhloose_cutd0dphideta',            'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nod0',                          'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],


        ['e18_etcut_trkcut_L1EM15', 'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'], -1],

        ['g25_loose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g25_medium',                    'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_medium',                    'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g24_loose',                     'L1_EM20VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['2e17_lhvloose_nod0',                   'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e17_lhvloose_nod0_L12EM15VHI',         'L1_2EM15VHI', [], [PhysicsStream], ['Primary:20000','RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e19_lhvloose_nod0',         'L1_2EM15VHI', [], [PhysicsStream], ['Primary:20000','RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e24_lhvloose_nod0',                   'L1_2EM20VH', [], [PhysicsStream, 'express'], ['Primary:20000','RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nod0_2e12_lhloose_nod0_L1EM15VH_3EM10VH',                       'L1_EM15VH_3EM10VH', ['L1_EM15VH','L1_3EM10VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  

        ['e22_lhloose_nod0_e12_lhloose_nod0_e10_lhloose_nod0_L1EM20VH_2EM10VH_3EM8VH',    'L1_EM20VH_2EM10VH_3EM8VH', ['L1_EM20VH','L1_2EM10VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e22_lhvloose_nod0_e12_lhvloose_nod0_e10_lhvloose_nod0_L1EM20VH_2EM10VH_3EM8VH', 'L1_EM20VH_2EM10VH_3EM8VH', ['L1_EM20VH','L1_2EM10VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e24_lhloose_nod0_2e12_lhloose_nod0_L1EM20VH_3EM10VH',   'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['Primary:20000','RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e24_lhvloose_nod0_2e12_lhvloose_nod0_L1EM20VH_3EM10VH', 'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['Primary:20000','RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_nod0_2g12_loose',                          'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['Primary:20000','RATE:ElectronPhoton', 'BW:Egamma'],-1,['parallel',-1,[] ]],
        ['e24_lhmedium_nod0_2g12_medium',                         'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['Primary:20000','RATE:ElectronPhoton', 'BW:Egamma'],-1,['parallel',-1,[] ]],


        ##########
        # Monopole triggers (or rerun?)
        ['g25_medium_L1EM20VH',           'L1_EM20VH',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_medium_L1EM20VH',                  'L1_EM20VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM20VH',            'L1_EM20VH',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        # Di-EM triggers
        # cut-based


        # likelihood


#Physics (e+g):
        ['e24_lhmedium_nod0_L1EM15VH_g25_medium',  'L1_2EM15VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e24_lhmedium_nod0_L1EM20VH_g25_medium',  'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_nod0_g35_loose',            'L1_2EM15VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 

        #Diphoton triggers
        ['g35_loose_g25_loose',                      'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_loose_g25_loose_L12EM20VH',            'L1_2EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_medium_g25_medium_L12EM20VH',          'L1_2EM20VH', [], [PhysicsStream, 'express'], ['Primary:20000','RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_medium_g25_medium',                    'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 

        # LLP
        ['g35_medium_g25_medium_L1EM7_EMPTY',        'L1_EM7_EMPTY', ['L1_EM7_EMPTY','L1_2EM15VH'], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['g35_medium_g25_medium_L1EM7_UNPAIRED_ISO', 'L1_EM7_UNPAIRED_ISO', ['L1_EM7_UNPAIRED_ISO','L1_2EM15VH'], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],

        ['2g22_tight_L1EM7_EMPTY',    'L1_EM7_EMPTY', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g22_tight_L1EM7_UNPAIRED_ISO',        'L1_EM7_UNPAIRED_ISO', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g50_tight_L1EM7_EMPTY',    'L1_EM7_EMPTY', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g50_tight_L1EM7_UNPAIRED_ISO',        'L1_EM7_UNPAIRED_ISO', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['e24_lhmedium_nod0_L1EM20VH',           'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e26_lhtight_nod0',                  'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_nod0_ivarloose',        'L1_EM22VHI', [], [PhysicsStream], ['Primary:17000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_nod0_ivarloose_L1EM22VHIM',        'L1_EM22VHIM', [], [PhysicsStream], ['Primary:17000','RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e26_lhmedium_nod0_ivarloose',       'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_ivarmedium',      'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_ivartight',       'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e28_lhtight_nod0',                  'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_ivarloose',        'L1_EM24VHI', [], [PhysicsStream,'express'], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_ivarloose_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e28_lhmedium_nod0_ivarloose',       'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivarmedium',      'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivartight',       'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],


        #Supporting
        
        ['e28_lhtight_nod0_L1EM22VHI',            'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_ivarloose_L1EM22VHI',  'L1_EM22VHI', [], [PhysicsStream], ['Primary:17000','RATE:SingleElectron', 'BW:Egamma'],-1],

        # Et cut only chains
        ['e250_etcut',                           'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e300_etcut',                           'L1_EM24VHI',  [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e300_etcut_L1EM24VHIM',                 'L1_EM24VHIM',  [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1], 

        # 2e34 single items
        ['e60_lhmedium_nod0_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhmedium_nod0_L1EM24VHIM', 'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhloose_nod0_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhloose_nod0_L1EM24VHIM', 'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],
				
        
        #Chains for testing 2e34 menus
        ['e32_lhtight_nod0_ivarloose',        'L1_EM24VHI', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],  
        

        #ATR-16089 supporting triggers for MC16c
        ['e26_lhvloose_nod0',        'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhvloose_nod0',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_nod0_L1EM22VHIM',        'L1_EM22VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_L1EM22VHIM',        'L1_EM22VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhvloose_nod0_L1EM22VHIM',        'L1_EM22VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhvloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e26_lhmedium_nod0_ivarloose_L1EM22VHIM',        'L1_EM22VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_ivarmedium_L1EM22VHIM',        'L1_EM22VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_ivartight_L1EM22VHIM',        'L1_EM22VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivarloose_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivarmedium_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivartight_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhmedium_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['Primary:20000','RATE:SingleElectron', 'BW:Egamma'],-1],

        ['2e17_lhvloose_nod0_noringer_L12EM15VHI',        'L1_2EM15VHI', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e17_lhvloose_nod0_noringer_L1EM15VHI',        'L1_EM15VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhvloose_nod0_L1EM15VHI',        'L1_EM15VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e19_lhvloose_nod0',        'L1_EM15VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['g20_tight_L1EM15VHI',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_tight_icalovloose_L1EM15VHI',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_tight_icalotight_L1EM15VHI',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g22_tight_L1EM15VHI',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g22_tight_icalovloose_L1EM15VHI',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g22_tight_icalovloose',            'L1_EM15VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g22_tight_icalotight_L1EM15VHI',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g22_tight_icalotight',             'L1_EM15VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['g25_loose_L1EM20VH',                     'L1_EM20VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['e24_lhloose_nod0_L1EM20VH',        'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['g12_loose',                     'L1_EM10VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g12_medium',                     'L1_EM10VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['e60_lhloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e70_lhloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['g70_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g80_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g80_loose_icalovloose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['e60_etcut_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e70_etcut_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_etcut_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e100_etcut_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e120_etcut_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e200_etcut_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e250_etcut_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e70_lhvloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhvloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e100_lhvloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e120_lhvloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhvloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e160_lhvloose_nod0_L1EM24VHIM',        'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e160_lhvloose_nod0',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        
        ['g60_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g100_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g120_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g140_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g160_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g180_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['e20_lhmedium_nod0_L1EM18VHI',        'L1_EM18VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_lhtight_nod0_ivarloose_L1EM18VHI',        'L1_EM18VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['g35_loose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_tight_icalotight_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_tight_icalotight_L1EM24VHI',                     'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g40_tight_icalotight_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g40_tight_icalotight_L1EM24VHI',                      'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g85_tight_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g85_tight_icalovloose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g85_tight_icalovloose_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1],
        ['g100_tight_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g100_tight_icalovloose_L1EM24VHIM',                     'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g100_tight_icalovloose_L1EM24VHI',                     'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        #ATR-16089 performance chains
        ['e26_lhmedium_nod0_icalomedium',        'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_icalotight',        'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e26_lhmedium_nod0_ivarmedium_icalomedium',        'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_ivartight_icalotight',        'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e28_lhmedium_nod0_icaloloose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_icalomedium',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_icalotight',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e28_lhmedium_nod0_ivarloose_icaloloose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivarmedium_icalomedium',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivartight_icalotight',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhmedium_nod0_noringer_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhmedium_nod0_noringer_L1EM24VHIM', 'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhloose_nod0_noringer_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhloose_nod0_noringer_L1EM24VHIM', 'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #ATR-16089 test chains
        ['e25_mergedtight_ivarloose',           'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e25_mergedtight',                      'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'], -1],
        ['e30_mergedtight',                      'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'], -1],

        ['g35_medium_icalovloose',           'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['g35_medium_icalotight',           'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e26_lhmedium_nod0_ivartight_icalotight_L1EM22VHIM',           'L1_EM22VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhmedium_nod0_ivartight_icalotight_L1EM24VHIM',           'L1_EM24VHIM', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #AFP egamma       
        ['g10_loose_L1EM3_AFP_A_OR_C',                  'L1_EM3_AFP_A_OR_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g10_loose_L1EM3_AFP_A_AND_C',                  'L1_EM3_AFP_A_AND_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['e10_lhloose_L1EM3_AFP_A_OR_C',                  'L1_EM3_AFP_A_OR_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e10_lhloose_L1EM3_AFP_A_AND_C',                  'L1_EM3_AFP_A_AND_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['g10_loose_L1EM3',                  'L1_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e10_lhloose_L1EM3',                  'L1_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #HI egamma for 5TeV run ATR-17162 going to Main stream
        ['g15_etcut_L1EM7',                     'L1_EM7', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_medium_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_tight_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_etcut_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_medium',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_tight',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_etcut',                     'L1_EM15VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g25_medium_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g25_tight_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g25_etcut_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g30_loose_L1EM15',                     'L1_EM15', [], [PhysicsStream,'express'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g30_etcut_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['2g10_loose_L12EM7',                     'L1_2EM7', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['2g15_loose_L12EM7',                     'L1_2EM7', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        
        ['e10_loose_L1EM7',                     'L1_EM7',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e10_lhloose_L1EM7',                     'L1_EM7',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_loose_L1EM7',                     'L1_EM7',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_lhloose_L1EM7',                     'L1_EM7',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_lhloose_nod0_L1EM7',                     'L1_EM7',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_loose_L1EM12',                     'L1_EM12',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_lhloose_L1EM12',                     'L1_EM12',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_lhmedium_L1EM12',                     'L1_EM12',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_etcut_L1EM12',                     'L1_EM12',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_lhmedium_nod0_L1EM12',                     'L1_EM12',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_vloose_L1EM12',                     'L1_EM12',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_lhvloose_L1EM12',                     'L1_EM12',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_loose_L1EM15',                     'L1_EM15',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_lhloose_L1EM15',                     'L1_EM15',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_etcut_L1EM15',                     'L1_EM15',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_lhloose_nod0_L1EM15',                     'L1_EM15',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_vloose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_lhvloose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_etcut',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_vloose_L1EM15VH',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhvloose_L1EM15VH',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_etcut_L1EM15VH',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhvloose_nod0_L1EM15VH',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['2e15_lhloose_L12EM12',                     'L1_2EM12',[], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e15_loose_L12EM12',                     'L1_2EM12',[], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e15_lhloose_nod0_L12EM12',                     'L1_2EM12',[], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        
			 ]

    TriggerFlags.BphysicsSlice.signatures = [
        #ATR-16219
        ['3mu4_bDimu2700',                'L1_3MU4', [], [BPhysicsStream],             [RATE_BphysTag,BW_BphysTag], -1],

        ['2mu6_bPhi',                     'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bPhi',                 'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','mu6_bPhi']]],
        ['mu11_mu6noL1_bPhi_L1MU11_2MU6', 'L1_MU11_2MU6', ['L1_MU11','L2_mu6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','mu6noL1_bPhi']]],

        ['mu6_mu4_bBmumux_BsmumuPhi',     'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bBmumux_BsmumuPhi',        'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bBmumux_BsmumuPhi',    'L1_MU10_2MU6', ['L1_MU10','L1_MU6'],[BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],

        ['2mu4_bDimu',                    'L1_2MU4', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1], 
        ['2mu4_bDimu_noinvm_novtx_ss',    'L1_2MU4', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu4_bDimu_novtx_noos',         'L1_2MU4', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],  

        ['mu6_mu4_bBmumuxv2',             'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bBmumux_BcmumuDsloose', 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bBmumux_BpmumuKp',      'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],        
        ['mu6_mu4_bDimu',                 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bDimu_noinvm_novtx_ss', 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bDimu_novtx_noos',      'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bJpsimumu',             'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bUpsimumu',             'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bBmumu',                'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
	
        ['2mu6_bBmumux_BcmumuDsloose',    'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bDimu',                    'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1], 
        ['2mu6_bDimu_noinvm_novtx_ss',    'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bDimu_novtx_noos',         'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],  
        ['2mu6_bJpsimumu',                'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bUpsimumu',                'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bBmumu',                   'L1_2MU6', [], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bBmumuxv2', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bBmumux_BcmumuDsloose', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bBmumux_BpmumuKp',      'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bJpsimumu_Lxy0', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bJpsimumu', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bUpsimumu', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],



        ['mu6_mu4_bJpsimumu_Lxy0',                    'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_mu4_bBmumu_Lxy0',                    'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bDimu_Lxy0',                    'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bJpsimumu_Lxy0',                    'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bBmumu_Lxy0',                    'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
	
        ['mu4_bJpsi_Trkloose',            'L1_MU4',  [], [PhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_bJpsi_Trkloose',            'L1_MU6',  [], [PhysicsStream,'express'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_bJpsi_Trkloose',           'L1_MU10', [], [PhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu14_bJpsi_Trkloose',           'L1_MU10', [], [PhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu20_bJpsi_Trkloose',       'L1_MU20MU21', ['L1_MU20'], [PhysicsStream,'express'], [RATE_BphysTag,BW_BphysTag], -1],

        # ATR-16163
        ['mu4_bJpsi_TrkPEB',            'L1_MU4',  [], ['BphysPEB'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_bJpsi_TrkPEB',            'L1_MU6',  [], ['BphysPEB'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu6_bJpsi_lowpt_TrkPEB',      'L1_MU6',  [], ['BphysPEB'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_bJpsi_TrkPEB',           'L1_MU10', [], ['BphysPEB'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_bJpsi_TrkPEBmon',        'L1_MU10',  [], [BPhysicsStream,'express'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu14_bJpsi_TrkPEB',           'L1_MU10', [], ['BphysPEB'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_bJpsi_TrkPEB',           'L1_MU11', [], ['BphysPEB'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu20_bJpsi_TrkPEB',       'L1_MU20MU21', ['L1_MU20'], ['BphysPEB'], [RATE_BphysTag,BW_BphysTag], -1],
        
        ['3mu4_bDimu',                    'L1_3MU4', [], [BMultiMuonStream,'express'], [RATE_BMultiMuonTag,BW_BphysTag], -1],
        ['3mu4_bJpsi',                    'L1_3MU4', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['3mu4_bTau',                     'L1_3MU4', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['3mu4_nomucomb_bTau',            'L1_3MU4', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],  # OI validation w.r.t 2016 
        ['3mu4_bUpsi',                    'L1_3MU4', [], [BMultiMuonStream], [RATE_BMultiMuonTag,BW_BphysTag], -1],
        
        ['3mu6_bDimu',                   'L1_3MU6', [], [BMultiMuonStream], [RATE_BMultiMuonTag,BW_BphysTag], -1],
        ['3mu6_bJpsi',                   'L1_3MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['3mu6_bTau',                    'L1_3MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['3mu6_bUpsi',                   'L1_3MU6', [], [BMultiMuonStream], [RATE_BMultiMuonTag,BW_BphysTag], -1],

        ['mu6_2mu4_bJpsi', 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], [BPhysicsStream], [RATE_BphysTag, BW_BphysTag], -1],
        ['mu6_2mu4_bUpsi', 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],

        ### ATR-14616: L1Topo SM DY
        ['mu6_iloose_mu6_11invm24_noos_L1DY-BOX-2MU6',       'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1],
        ['mu6_iloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1],
        ['mu6_iloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1],

        ### ATR-14543
        ['mu6_ivarloose_mu6_11invm24_noos_L1DY-BOX-2MU6',       'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1],
        ['mu6_ivarloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1],
        ['mu6_ivarloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1],

        ['mu4_mu4_idperf_bJpsimumu_noid',  'L1_2MU4', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring',BW_BphysTag, 'BW:ID'], -1],  
        ['2mu6_bBmumux_BpmumuKp',    'L1_2MU6',     [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bBmumuxv2',              'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu13_mu13_idperf_Zmumu',     'L1_2MU10', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring','BW:Muon', 'BW:ID'], -1],
        ['2mu10_bDimu_noL2',                    'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu10_bJpsimumu_noL2',                    'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],

        ['2mu10_bBmumuxv2',               'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu10_bBmumux_BcmumuDsloose',   'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu10_bDimu',                   'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu10_bDimu_novtx_noos',        'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu10_bJpsimumu',               'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu10_bUpsimumu',               'L1_2MU10', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
     
       ### ATR-14350
       ['2mu6_bBmumuxv2_L1LFV-MU6',              'L1_LFV-MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu6_bBmumux_BcmumuDsloose_L1LFV-MU6',  'L1_LFV-MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu6_bDimu_L1LFV-MU6',                  'L1_LFV-MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu6_bDimu_novtx_noos_L1LFV-MU6',       'L1_LFV-MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       
       ['2mu4_bBmumu_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',         'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bBmumu_Lxy0_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',    'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bJpsimumu_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',      'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bJpsimumu_Lxy0_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', 'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bBmumuxv2_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',      'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bBmumux_BsmumuPhi_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', 'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bBmumux_BpmumuKp_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', 'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU4_BPH-0DR15-2MU4', 'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bDimu_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',                 'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['2mu4_bDimu_novtx_noos_L1BPH-2M9-2MU4_BPH-0DR15-2MU4',      'L1_BPH-2M9-2MU4_BPH-0DR15-2MU4', ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       
       ['mu6_mu4_bBmumu_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bBmumu_Lxy0_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bJpsimumu_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bJpsimumu_Lxy0_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bBmumuxv2_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bBmumux_BsmumuPhi_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bBmumux_BpmumuKp_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bDimu_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
       ['mu6_mu4_bDimu_novtx_noos_L1BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4','L1_BPH-2M9-MU6MU4_BPH-0DR15-MU6MU4', ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],

        ['mu11_mu6_bDimu2700',            'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bDimu_Lxy0',            'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bDimu2700_Lxy0',            'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        
        ['3mu4_bNocut',          'L1_3MU4',           [], [BMultiMuonStream, 'express'], [RATE_BMultiMuonTag, 'BW:Bphys'], -1],
        ['3mu4_bVertex2',          'L1_3MU4',           [], [BMultiMuonStream], [RATE_BMultiMuonTag, 'BW:Bphys'], -1],
        ['3mu4_bVertex3',          'L1_3MU4',           [], [BMultiMuonStream], [RATE_BMultiMuonTag, 'BW:Bphys'], -1],
        ['3mu3_mu3noL1_bNocut_L13MU4', 'L1_3MU4', ['L1_3MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag,'BW:Muon'], -1,['serial',-1,['3mu3','mu3noL1']]],
        ['2mu4_mu3_mu2noL1_bNocut_L13MU4', 'L1_3MU4', ['L1_2MU4','L1_MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag,'BW:Muon'], -1,['serial',-1,['2mu4','mu3','mu2noL1']]],
        ['2mu6_mu4_bUpsi', 'L1_2MU6_3MU4', ['L1_2MU6','L1_MU4'], [BMultiMuonStream], [RATE_BMultiMuonTag, BW_BphysTag], -1],
        ['4mu4_bNocut', 'L1_4MU4', ['L1_4MU4',''], [BMultiMuonStream], [RATE_BMultiMuonTag,'BW:Muon'], -1,['serial',-1,['4mu4','']]],

        #OI replacement for mu20_nomucomb_mu6noL1_nscan03 and mu11_nomucomb_2mu4noL1_nscan03_L1MU11_2MU6
        ['mu20_mu6noL1_bNocut',             'L1_MU20MU21',      ['L1_MU20','L2_mu20'],        [BPhysicsStream], [RATE_BphysTag,'BW:Muon'], -1,['serial',-1,['mu20','mu6noL1']]],  
        ['mu11_2mu4noL1_bNocut_L1MU11_2MU6', 'L1_MU11_2MU6', ['L1_MU11','L2_mu11'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','2mu4noL1']]], 
        ['mu11_2mu4noL1_bNocut_L1LFV-MU', 'L1_LFV-MU', ['L1_MU10','L2_mu11'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','2mu4noL1']]], 

				['3mu4_bPhi',                     'L1_3MU4', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],

        ['mu6_2mu4_bNocut_L1MU6_3MU4', 'L1_MU6_3MU4', ['L1_MU6','L1_2MU4'], [BMultiMuonStream], [RATE_BMultiMuonTag,'BW:Muon'], -1],
        ['2mu6_mu4_bNocut_L12MU6_3MU4', 'L1_2MU6_3MU4', ['L1_2MU6','L1_MU4'], [BMultiMuonStream], [RATE_BMultiMuonTag,'BW:Muon'], -1],


        ['mu11_2mu4noL1_bTau_L1MU11_2MU6', 'L1_MU11_2MU6', ['L1_MU11','L2_mu11'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','2mu4noL1']]],
        ['mu11_mu6noL1_bTau_L1MU11_2MU6',  'L1_MU11_2MU6', ['L1_MU11','L2_mu6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','mu6noL1']]],
        ['mu20_mu6noL1_bTau',              'L1_MU20MU21', ['L1_MU20','L2_mu20'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu20','mu6noL1']]],
        ## LFV topo based nscan (ATR-14352)
        ['mu11_2mu4noL1_bTau_L1LFV-MU',    'L1_LFV-MU', ['L1_MU10','L2_mu11'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','2mu4noL1']]],      


        
        ['mu6_2mu4_bTau_L1MU6_3MU4' , 'L1_MU6_3MU4',  ['L1_MU6','L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_mu4_bTau_L12MU6_3MU4', 'L1_2MU6_3MU4', ['L1_2MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        
        ['mu6_iloose_mu6_24invm60_noos_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1, False],

        ### ATR-14543
        ['mu6_ivarloose_mu6_24invm60_noos_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream],  [RATE_DYTag,BW_DYTag], -1, False],

        ### ATR-14478  
        ['2mu6_bBmumux_Taumumux',         'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu6_bBmumux_Taumumux_noL2',    'L1_2MU6', [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bBmumux_Taumumux',     'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu10_mu6_bBmumux_Taumumux_noL2','L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],


        ### ATR-14350: L1Topo bphysics chains
        # 71% bckg rejection WP

        ['mu6_mu4_bDimu_L1BPH-8M15-MU6MU4',     'L1_BPH-8M15-MU6MU4',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L1BPH-8M15-MU6MU4',     'L1_BPH-8M15-MU6MU4',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4', 'L1_BPH-8M15-MU6MU4',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],


        # 96% bckg rejection WP
        ['2mu6_bDimu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',      'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bDimu_novtx_noos_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',      'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bJpsimumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bUpsimumu_L1BPH-8M15-2MU6_BPH-0DR22-2MU6', 'L1_BPH-8M15-2MU6_BPH-0DR22-2MU6',     ['L1_2MU6'], [PhysicsStream, 'express'], ['Primary:20000','RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',     'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumuxv2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumux_BpmumuKp_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        # 95% bckg rejection WP (for Upsilon only)
        ['2mu6_bDimu_L1BPH-8M15-2MU6',     'L1_BPH-8M15-2MU6',     ['L1_2MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bDimu_novtx_noos_L1BPH-8M15-2MU6',     'L1_BPH-8M15-2MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bUpsimumu_L1BPH-8M15-2MU6', 'L1_BPH-8M15-2MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu4-OneBarrel
        ['2mu4_bDimu_L12MU4-B',      'L1_2MU4-B',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bDimu_novtx_noos_L12MU4-B',      'L1_2MU4-B',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bJpsimumu_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bBmumu_L12MU4-B',     'L1_2MU4-B',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bUpsimumu_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bBmumuxv2_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bBmumux_BcmumuDsloose_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu4-BarrelOnly
        ['2mu4_bDimu_L12MU4-BO',      'L1_2MU4-BO',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bDimu_novtx_noos_L12MU4-BO',      'L1_2MU4-BO',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bJpsimumu_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bBmumu_L12MU4-BO',     'L1_2MU4-BO',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bUpsimumu_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bBmumuxv2_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu4_bBmumux_BcmumuDsloose_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        # Simple OneBarrel and BarrelOnly: mu6_mu4-OneBarrel
        ['mu6_mu4_bDimu_L12MU4-B',      'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L12MU4-B',      'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bJpsimumu_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bBmumu_L12MU4-B',     'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bUpsimumu_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bBmumuxv2_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bBmumux_BcmumuDsloose_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        # Simple OneBarrel and BarrelOnly: mu6_mu4-BarrelOnly
        ['mu6_mu4_bDimu_L1MU6MU4-BO',      'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L1MU6MU4-BO',      'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bJpsimumu_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bBmumu_L1MU6MU4-BO',     'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bUpsimumu_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bBmumuxv2_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu6_mu4_bBmumux_BcmumuDsloose_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu6-OneBarrel
        ['2mu6_bDimu_L12MU6-B',      'L1_2MU6-B',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bDimu_novtx_noos_L12MU6-B',      'L1_2MU6-B',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bJpsimumu_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumu_L12MU6-B',     'L1_2MU6-B',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bUpsimumu_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumuxv2_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumux_BcmumuDsloose_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu6-BarrelOnly
        ['2mu6_bDimu_L12MU6-BO',      'L1_2MU6-BO',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bDimu_novtx_noos_L12MU6-BO',      'L1_2MU6-BO',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bJpsimumu_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumu_L12MU6-BO',     'L1_2MU6-BO',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bUpsimumu_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumuxv2_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumux_BcmumuDsloose_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],


        # For semileptonic modes
        ['mu10_mu6_bBmumuxv2_L1LFV-MU',  'L1_LFV-MU',     ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['mu10_mu6_bBmumux_BcmumuDsloose_L1LFV-MU',  'L1_LFV-MU',     ['L1_MU10','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],

        ['mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4', 'L1_BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4',     ['L1_MU6','L1_MU4'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
        #ATR-16219
        ['mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4-BO', 'L1_BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4-BO',     ['L1_MU6','L1_MU4'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],

# ATR-14350




        ['2mu6_bBmumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bBmumux_BsmumuPhi_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1, False],
        ['2mu6_bJpsimumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],




        ['mu11_mu6noL1_bTau_L1LFV-MU', 'L1_LFV-MU', ['L1_MU10','L2_mu6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','mu6noL1']]],
        
        # new baseline triggers, ATR-15503
        ['mu11_mu6_bBmumu',                'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bBmumuxv2',             'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bBmumux_BcmumuDsloose', 'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bBmumux_BpmumuKp',      'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bJpsimumu_Lxy0',        'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bJpsimumu',             'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bUpsimumu',             'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bDimu',                 'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bDimu_noinvm_novtx_ss', 'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6_bDimu_novtx_noos',      'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1],

        # ATR-16314
        ['mu11_mu6_bTau',                  'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream, 'express'], [RATE_BphysTag,BW_BphysTag], -1],

        
        # support triggers ATR-15851
        ['mu11_mu6_bNocut',                 'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['mu11_mu6noL1_bNocut_L1MU11_2MU6',  'L1_MU11_2MU6', ['L1_MU11','L2_mu6'], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1,['serial',-1,['mu11','mu6noL1']]],
        
        #HI bphysics for 5TeV run ATR-17162 going to Main stream
        ['2mu4_bJpsimumu',                 'L1_2MU4', [], [PhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['2mu4_bUpsimumu',                 'L1_2MU4', [], [PhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],

			 ]

    TriggerFlags.CombinedSlice.signatures = [
        

        ['j35_gsc55_boffperf_split_ht500_L1HT190-J15.ETA21',  'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht500','j35_gsc55_boffperf_split']]], 
        ['j35_gsc55_bmv2c1040_split_ht500_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht500','j35_gsc55_bmv2c1040_split']]],
        ['j35_gsc55_bmv2c1050_split_ht500_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht500','j35_gsc55_bmv2c1050_split']]],
        ['j55_gsc75_bmv2c1040_split_ht500_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht500','j55_gsc75_bmv2c1040_split']]],
        ['j55_gsc75_bmv2c1050_split_ht500_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht500','j55_gsc75_bmv2c1050_split']]],
        ['j35_gsc55_bmv2c1050_split_ht700_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht700','j35_gsc55_bmv2c1050_split']]],
        ['j35_gsc55_bmv2c1060_split_ht700_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht700','j35_gsc55_bmv2c1060_split']]],

        # kill these ?
        ['j35_gsc55_boffperf_split_ht500_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1, ['ht500','j35_gsc55_boffperf_split' ]]], 
        ['j35_gsc55_bmv2c1050_split_ht500_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht500','j35_gsc55_bmv2c1050_split']]],
        ['j35_gsc55_bmv2c1060_split_ht500_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht500','j35_gsc55_bmv2c1060_split']]],
        ['j35_gsc55_bmv2c1050_split_ht700_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['Primary:20000','RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht700','j35_gsc55_bmv2c1050_split']]],
        ['j35_gsc55_bmv2c1060_split_ht700_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht700','j35_gsc55_bmv2c1060_split']]],

        ['2j55_boffperf_split_ht300_L1HT190-J15.ETA21',       'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht300',   '2j55_boffperf_split']]], 
        ['2j35_gsc55_boffperf_split_ht300_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht300',  '2j35_gsc55_boffperf_split'  ]]], 
        ['2j35_gsc55_bmv2c1050_split_ht300_L1HT190-J15.ETA21','L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht300',  '2j35_gsc55_bmv2c1050_split' ]]], 
        ['2j35_gsc55_bmv2c1040_split_ht300_L1HT190-J15.ETA21','L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht300',  '2j35_gsc55_bmv2c1040_split' ]]], 
        ['2j35_gsc55_bmv2c1060_split_ht300_L1HT190-J15.ETA21','L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht300',  '2j35_gsc55_bmv2c1060_split' ]]], 
        ['2j35_gsc55_bmv2c1070_split_ht300_L1HT190-J15.ETA21','L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,['ht300',  '2j35_gsc55_bmv2c1070_split' ]]], 

#        # L1HT190-J15s5.ETA21
        ['2j55_boffperf_split_ht300_L1HT190-J15s5.ETA21',        'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,[ 'ht300', '2j55_boffperf_split'        ]]], 
        ['2j35_gsc55_boffperf_split_ht300_L1HT190-J15s5.ETA21',  'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,[ 'ht300', '2j35_gsc55_boffperf_split'  ]]], 
        ['2j35_gsc55_bmv2c1050_split_ht300_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,[ 'ht300', '2j35_gsc55_bmv2c1050_split' ]]], 
        ['2j35_gsc55_bmv2c1060_split_ht300_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,[ 'ht300', '2j35_gsc55_bmv2c1060_split' ]]], 
        ['2j35_gsc55_bmv2c1070_split_ht300_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'], -1,['serial',-1,[ 'ht300', '2j35_gsc55_bmv2c1070_split' ]]], 

        ['mu4_j35_gsc55_boffperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream, 'express'], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j35_gsc55_boffperf_split'] ]],

				# Backup ATR-16986
        ['mu4_j35_boffperf_split_dr05_dz99', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j35_boffperf_split'] ]],
        ['mu4_j35_boffperf_split', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j35_boffperf_split'] ]],
        ['mu4_j35_boffperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j35_boffperf_split'] ]],
        ['mu4_j35_boffperf_split_dr05_dz02_L1BTAG-MU4J15', 'L1_BTAG-MU4J15', ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j35_boffperf_split'] ], False],
        ['mu26_ivarmedium_j110_gsc150_boffperf_split_j35_boffperf_split', 'L1_MU20MU21',    ['L1_MU20',    '', ''], [PhysicsStream], ['Primary:20000','RATE:MuonBJet',  'BW:BJet_combined'],  -1, ['serial',-1,['mu26_ivarmedium', 'j110_gsc150_boffperf_split', 'j35_boffperf_split']]],
        ['e28_lhtight_nod0_ivarloose_j110_gsc150_boffperf_split_j35_boffperf_split',  'L1_EM24VHI', ['L1_EM24VHI', '', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:BJet_combined'],  -1, ['serial',-1,['e28_lhtight_nod0_ivarloose', 'j110_gsc150_boffperf_split','j35_boffperf_split']]],
        ['g20_tight_icaloloose_j35_bmv2c1077_split_3j35_0eta490_invm500', 'L1_EM18VHI_MJJ-300', ['','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g20_tight_icaloloose","3j35_0eta490_invm500","j35_bmv2c1077_split"]]],
        ['g27_medium_L1EM24VHI_j35_bmv2c1077_split_3j35_0eta490_invm700', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g27_medium_L1EM24VHI","3j35_0eta490_invm700","j35_bmv2c1077_split"]]],
        ['g27_medium_L1EM24VHI_2j35_bmv2c1077_split_2j35_0eta490', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g27_medium_L1EM24VHI","2j35_0eta490","2j35_bmv2c1077_split"]]],
        
        ['mu4_j40_dr05', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBjet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j40'] ]],
	    ['mu4_j50_dr05', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBjet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j50'] ]],
	    ['mu4_j60_dr05', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBjet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j60'] ]],
        ['mu6_j30_dr05', 'L1_MU6',     ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBjet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j30'] ]],




        ### ATR-14356: bjets+HT VBF chians
#        ['2j45_bmv2c1060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_HT150-JJ15.ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'],-1,['serial',-1,['2j45_bmv2c1060_split', '2j35_0eta490_invm700']]],
#        ['2j25_gsc45_bmv2c1060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_HT150-JJ15.ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'],-1,['serial',-1,['2j25_gsc45_bmv2c1060_split', '2j35_0eta490_invm700']]],
        ['2j45_bmv2c1060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_HT150-JJ15.ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'],-1,['serial',-1,['2j35_0eta490_invm700','2j45_bmv2c1060_split']]],
        ['2j25_gsc45_bmv2c1060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_HT150-JJ15.ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined'],-1,['serial',-1,['2j35_0eta490_invm700','2j25_gsc45_bmv2c1060_split']]],

	#
	#  1/2/3b + xe
	#
#        ['j60_gsc100_bmv2c1050_split_xe80_mht_L1XE60','L1_XE60',[],[PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1,['serial',-1,['j60_gsc100_bmv2c1050_split','xe80_mht_L1XE60']]],
#        ['j60_gsc100_bmv2c1060_split_xe80_mht_L1XE60','L1_XE60',[],[PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1,['serial',-1,['j60_gsc100_bmv2c1060_split','xe80_mht_L1XE60']]],
#        ['2j45_bmv2c1070_split_xe80_mht_L12J15_XE55','L1_2J15_XE55',[],[PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1,['serial',-1,['2j45_bmv2c1070_split','xe80_mht']]],
#        ['2j25_gsc45_bmv2c1070_split_xe80_mht_L12J15_XE55','L1_2J15_XE55',[],[PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1,['serial',-1,['2j25_gsc45_bmv2c1070_split','xe80_mht']]],
#        ['3j35_bmv2c1077_split_xe60_mht_L13J15.0ETA25_XE40',"L1_3J15.0ETA25_XE40",[],[PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1, ['serial',-1,['3j35_bmv2c1077_split','xe60_mht']]],
#        ['3j15_gsc35_bmv2c1077_split_xe60_mht_L13J15.0ETA25_XE40',"L1_3J15.0ETA25_XE40",[],[PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1, ['serial',-1,['3j15_gsc35_bmv2c1077_split','xe60_mht']]],
        ['j60_gsc100_bmv2c1050_split_xe80_mht_L1XE60','L1_XE60',[],[PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1,['serial',-1,['xe80_mht_L1XE60','j60_gsc100_bmv2c1050_split']]],
        ['2j25_gsc45_bmv2c1070_split_xe80_mht_L12J15_XE55','L1_2J15_XE55',[],[PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1,['serial',-1,['xe80_mht','2j25_gsc45_bmv2c1070_split']]],
        ['3j35_bmv2c1077_split_xe60_mht_L13J15.0ETA25_XE40',"L1_3J15.0ETA25_XE40",[],[PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1, ['serial',-1,['xe60_mht','3j35_bmv2c1077_split']]],
        ['3j15_gsc35_bmv2c1077_split_xe60_mht_L13J15.0ETA25_XE40',"L1_3J15.0ETA25_XE40",[],[PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet_combined', 'BW:MET'], -1, ['serial',-1,['xe60_mht','3j15_gsc35_bmv2c1077_split']]],

        # Non-L1Topo W T&P triggers
        ['e13_etcut_trkcut_xs30_xe30_mt35', 'L1_EM12_XS20', ['L1_EM12','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e13_etcut_trkcut","xs30","xe30"]]],
        ['e13_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35', 'L1_EM12_XS20', ['L1_EM12','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e13_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e13_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35', 'L1_EM12_XS20', ['L1_EM12','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e13_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e13_etcut_trkcut_xs30_j15_perf_xe30_2dphi05_mt35', 'L1_EM12_XS20', ['L1_EM12','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e13_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e13_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35', 'L1_EM12_XS20', ['L1_EM12','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e13_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_xs30_xe30_mt35', 'L1_EM15_XS30', ['L1_EM15','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","xe30"]]],
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35', 'L1_EM15_XS30', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35', 'L1_EM15_XS30', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi05_mt35', 'L1_EM15_XS30', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35', 'L1_EM15_XS30', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],

        ['e13_etcut_trkcut_xs15_mt25', 'L1_EM12_XS20', ['L1_EM12',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e13_etcut_trkcut","xs15"]]],
        ['e13_etcut_trkcut_xs15_j20_perf_xe15_2dphi05_mt25', 'L1_EM12_XS20', ['L1_EM12','','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e13_etcut_trkcut","xs15","j20_perf","xe15"]]],
        ['e13_etcut_trkcut_j20_perf_xe15_2dphi05_mt25',      'L1_EM12_XS20', ['L1_EM12','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e13_etcut_trkcut","j20_perf","xe15"]]],
        ['e13_etcut_trkcut_xs15_j20_perf_xe15_6dphi05_mt25', 'L1_EM12_XS20', ['L1_EM12','','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e13_etcut_trkcut","xs15","j20_perf","xe15"]]],
        ['e13_etcut_trkcut_j20_perf_xe15_6dphi05_mt25',      'L1_EM12_XS20', ['L1_EM12','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e13_etcut_trkcut","j20_perf","xe15"]]],

        # Non-L1Topo W T&P triggers
        ['e18_etcut_trkcut_xs20_mt35', 'L1_EM15_XS30', ['L1_EM15',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs20"]]],
        ['e18_etcut_trkcut_xs20_j20_perf_xe20_6dphi15_mt35', 'L1_EM15_XS30', ['L1_EM15','','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e18_etcut_trkcut","xs20","j20_perf","xe20"]]],
        ['e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35',      'L1_EM15_XS30', ['L1_EM15','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e18_etcut_trkcut","j20_perf","xe20"]]],


        ['2e12_lhloose_nod0_mu10',  'L1_2EM8VH_MU10', ['L1_2EM8VH', 'L1_MU10'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['2e12_lhmedium_nod0_mu10', 'L1_2EM8VH_MU10', ['L1_2EM8VH', 'L1_MU10'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['e12_lhloose_nod0_2mu10',  'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['e12_lhmedium_nod0_2mu10', 'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['e17_lhloose_nod0_mu14',   'L1_EM15VH_MU10', ['L1_EM15VH', 'L1_MU10'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['e26_lhmedium_nod0_mu8noL1', 'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['Primary:17000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e26_lhmedium_nod0','mu8noL1'] ]],
        ['e28_lhmedium_nod0_mu8noL1', 'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e28_lhmedium_nod0','mu8noL1'] ]],
        ['e26_lhmedium_nod0_L1EM22VHIM_mu8noL1', 'L1_EM22VHIM', ['L1_EM22VHIM', ''], [PhysicsStream], ['Primary:17000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e26_lhmedium_nod0_L1EM22VHIM','mu8noL1'] ]],
        ['e28_lhmedium_nod0_L1EM24VHIM_mu8noL1', 'L1_EM24VHIM', ['L1_EM24VHIM', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e28_lhmedium_nod0_L1EM24VHIM','mu8noL1'] ]],
        ['e28_lhmedium_nod0', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1],
        ['e70_lhloose_nod0_xe70noL1',    'L1_EM22VHI',['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e70_lhloose_nod0","xe70noL1"]]],
        ['e70_lhloose_nod0_L1EM24VHIM_xe70noL1',    'L1_EM24VHIM',['L1_EM24VHIM',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e70_lhloose_nod0_L1EM24VHIM","xe70noL1"]]],
        ['e80_lhloose_nod0_xe80noL1',    'L1_EM22VHI',['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e80_lhloose_nod0","xe80noL1"]]],
        ['e80_lhloose_nod0_L1EM24VHI_xe80noL1',    'L1_EM24VHI',['L1_EM24VHI',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e80_lhloose_nod0_L1EM24VHI","xe80noL1"]]],
        ['e80_lhloose_nod0_L1EM24VHIM_xe80noL1',    'L1_EM24VHIM',['L1_EM24VHIM',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e80_lhloose_nod0_L1EM24VHIM","xe80noL1"]]],

        ['e7_lhmedium_nod0_mu24',        'L1_MU20MU21', ['L1_EM3', 'L1_MU20'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['g45_tight_L1EM22VHI_xe45noL1', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_L1EM22VHI","xe45noL1"]]],
        ['g45_tight_L1EM24VHI_xe45noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_L1EM24VHI","xe45noL1"]]],
        ['g45_tight_L1EM24VHIM_xe45noL1', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_L1EM24VHIM","xe45noL1"]]],
        ['g45_tight_icalovloose_L1EM24VHIM_xe45noL1', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_icalovloose_L1EM24VHIM","xe45noL1"]]],
        ['g80_loose_L1EM24VHI_xe80noL1',           'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose_L1EM24VHI","xe80noL1"]]],
        ['g80_loose_L1EM24VHIM_xe80noL1',           'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['Primary:17000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose_L1EM24VHIM","xe80noL1"]]],
        ['g80_loose_icalovloose_L1EM24VHIM_xe80noL1',           'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose_icalovloose_L1EM24VHIM","xe80noL1"]]],
        ['g45_tight_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g45_tight_icalovloose_L1EM22VHI_xe45noL1', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_icalovloose_L1EM22VHI","xe45noL1"]]],
        ['g45_tight_icalovloose_L1EM24VHI_xe45noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_icalovloose_L1EM24VHI","xe45noL1"]]],
        ['g80_loose_icalovloose_xe80noL1',           'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose_icalovloose","xe80noL1"]]],
        ['g80_loose_icalovloose_L1EM24VHI_xe80noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose_icalovloose_L1EM24VHI","xe80noL1"]]],
        ['g85_tight_icalovloose_L1EM22VHI_3j50noL1',  'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight_icalovloose_L1EM22VHI","3j50noL1"]]],
        ['g100_tight_icalovloose_L1EM22VHI_3j50noL1', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight_icalovloose_L1EM22VHI","3j50noL1"]]],
        ['g85_tight_icalovloose_L1EM24VHI_3j50noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight_icalovloose_L1EM24VHI","3j50noL1"]]],
        ['g100_tight_icalovloose_L1EM24VHI_3j50noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight_icalovloose_L1EM24VHI","3j50noL1"]]],

        #ATR-14196
        ['g80_loose_xe80noL1',           'L1_EM22VHI',  ['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose","xe80noL1"]]],

        ['g25_medium_mu24',              'L1_MU20MU21', ['L1_EM15VH','L1_MU20'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,[] ]],

        ['g15_loose_2mu10_msonly',       'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,[] ]],

        ### ATR-14351
        ['j100_xe110_mht_1dphi10_L1J40_DPHI-J20s2XE50', 'L1_J40_DPHI-J20s2XE50', [], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1,['serial',-1,["j100","xe110_mht"]]],


        #ATR-15243 Higgsino
        #di-muon primary
        ['2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30',  'L1_2MU4_J20_XE30_DPHI-J20s2XE30', [], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe40_pufit"]]], 

        #di-muon backup
        ['2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30',  'L1_2MU4_J20_XE30_DPHI-J20s2XE30', [], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe60_pufit"] ]],
        ['2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE40_DPHI-J20s2XE30',  'L1_2MU4_J20_XE40_DPHI-J20s2XE30', [], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe60_pufit"] ]],
        ['2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE50_DPHI-J20s2XE30',  'L1_2MU4_J20_XE50_DPHI-J20s2XE30', [], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe60_pufit"] ]],
        ['2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30',  'L1_2MU4_J20_XE30_DPHI-J20s2XE30', [], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe80_pufit"] ]],
        ['2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J20_XE40_DPHI-J20s2XE30',  'L1_2MU4_J20_XE40_DPHI-J20s2XE30', [], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe80_pufit"] ]],
        ['2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J20_XE50_DPHI-J20s2XE30',  'L1_2MU4_J20_XE50_DPHI-J20s2XE30', [], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe80_pufit"] ]],
        ['2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J40_XE50',  'L1_2MU4_J40_XE50', ['L1_2MU4','L1_J40','L1_XE50'], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe60_pufit"] ]],
        ['2mu4_invm1_j20_xe80_pufit_2dphi10_L12MU4_J40_XE50',  'L1_2MU4_J40_XE50', ['L1_2MU4','L1_J40','L1_XE50'], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4_invm1","j20","xe80_pufit"] ]],

        #single muon
        ['mu4_j80_xe80_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',  'L1_MU4_J30_XE40_DPHI-J20s2XE30', [], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu4","j80","xe80_pufit"] ]],

        #single muon backup
        ['mu4_j80_xe80_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30',  'L1_MU4_J50_XE50_DPHI-J20s2XE30', [], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu4","j80","xe80_pufit"] ]],
        ['mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30',  'L1_MU4_J50_XE50_DPHI-J20s2XE30', [], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu4","j90","xe90_pufit"] ]],
        ['mu4_j80_xe80_pufit_2dphi10_L1MU4_XE60',  'L1_MU4_XE60', ['L1_MU4','','L1_XE60'], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu4","j80","xe80_pufit"] ]],
        ['mu4_j90_xe90_pufit_2dphi10_L1MU4_XE60',  'L1_MU4_XE60', [], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu4","j90","xe90_pufit"] ]],


  

        # mu-e
        ['e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30', 'L1_MU4_J30_XE40_DPHI-J20s2XE30', ['L1_EM3','','',''], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1,['serial',-1,["e5_lhvloose_nod0","mu4","j30","xe40_pufit"]]],

        #mu-e backup
        ['e5_lhloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',  'L1_MU4_J30_XE40_DPHI-J20s2XE30', ['L1_EM3','','',''], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Egamma', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["e5_lhloose_nod0","mu4","j30","xe40_pufit"] ]],
        ['e5_lhmedium_nod0_mu4_j30_xe50_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',  'L1_MU4_J30_XE40_DPHI-J20s2XE30', ['L1_EM3','','',''], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Egamma', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","mu4","j30","xe50_pufit"] ]], 

        ['e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',  'L1_MU4_J30_XE40_DPHI-J20s2XE30', ['L1_EM3','','',''], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Egamma', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","mu4","j30","xe65_pufit"] ]], 
        ['e5_lhmedium_nod0_mu4_j30_xe50_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30',  'L1_MU4_J50_XE50_DPHI-J20s2XE30', ['L1_EM3','','',''], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Egamma', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","mu4","j30","xe50_pufit"] ]], 
        ['e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30',  'L1_MU4_J50_XE50_DPHI-J20s2XE30', ['L1_EM3','','',''], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Egamma', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","mu4","j30","xe65_pufit"] ]], 
        ['e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_XE60',  'L1_MU4_XE60', ['L1_EM3','L1_MU4','','L1_XE60'], [PhysicsStream], ['Primary:20000','RATE:MuonJetMET', 'BW:Egamma', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","mu4","j30","xe65_pufit"] ]], 


        #di-electron
        ['2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhvloose_nod0","j40","xe70_pufit"] ]],

        #di-electron backup
        ['2e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhloose_nod0","j40","xe70_pufit"] ]],
        ['2e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhmedium_nod0","j40","xe80_pufit"] ]],
        ['2e5_lhmedium_nod0_j40_xe90_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhmedium_nod0","j40","xe90_pufit"] ]],
        ['2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhvloose_nod0","j40","xe70_pufit_L1XE60"] ]],
        ['2e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhloose_nod0","j40","xe70_pufit_L1XE60"] ]],
        ['2e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhmedium_nod0","j40","xe80_pufit_L1XE60"] ]],
        ['2e5_lhmedium_nod0_j40_xe90_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["2e5_lhmedium_nod0","j40","xe90_pufit_L1XE60"] ]],

        # single electron
        ['e5_lhvloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhvloose_nod0","j50","xe70_pufit"] ]],

        #single electron backup 
        ['e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhloose_nod0","j50","xe70_pufit"] ]],
        ['e5_lhmedium_nod0_j50_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","j50","xe80_pufit"] ]],
        ['e5_lhmedium_nod0_j50_xe90_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","j50","xe90_pufit"] ]],
        ['e5_lhvloose_nod0_j50_xe70_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhvloose_nod0","j50","xe70_pufit_L1XE60"] ]],
        ['e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhloose_nod0","j40","xe70_pufit_L1XE60"] ]],
        ['e5_lhmedium_nod0_j40_xe80_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","j40","xe80_pufit_L1XE60"] ]],
        ['e5_lhmedium_nod0_j40_xe90_pufit_2dphi10_L1XE60',  'L1_XE60', ['L1_EM3','',''], [PhysicsStream], ['RATE:EgammaJetMET', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e5_lhmedium_nod0","j40","xe90_pufit_L1XE60"] ]],

        #inclusive triggers
        ['j50_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', [], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j50","xe80_pufit"] ]],

        #inclusive backup
        ['j70_xe100_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',  'L1_J40_XE50_DPHI-J20s2XE50', [], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j70","xe100_pufit"] ]],

        #ATR-16089 combined egamma support chains
        ['e50_etcut_trkcut_xe50_mt35', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut","xe50"]]],
        ['e60_etcut_trkcut_xe60_mt35', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut","xe60"]]],
        ['e50_etcut_trkcut_j15_perf_xe50_6dphi15_mt35', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut","j15_perf","xe50"]]],
        ['e60_etcut_trkcut_j15_perf_xe60_6dphi15_mt35', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut","j15_perf","xe60"]]],
        ['e50_etcut_trkcut_j15_perf_xe50_6dphi05_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut","j15_perf","xe50"]]],
        ['e60_etcut_trkcut_j15_perf_xe60_6dphi05_mt35','L1_EM24VHIM', ['L1_EM24VHIM','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut","j15_perf","xe60"]]],
        ['e50_etcut_trkcut_xs30_xe30_mt35', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut","xs30","xe30"]]],
        ['e60_etcut_trkcut_xs30_xe30_mt35', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut","xs30","xe30"]]],
        ['e50_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35', 'L1_EM24VHI', ['L1_EM24VHI','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e60_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35', 'L1_EM24VHI', ['L1_EM24VHI','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e50_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35','L1_EM24VHIM', ['L1_EM24VHIM','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e60_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35','L1_EM24VHIM', ['L1_EM24VHIM','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut","xs30","j15_perf","xe30"]]],

        ['e50_etcut_trkcut_L1EM24VHIM_xe50_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut_L1EM24VHIM","xe50"]]],
        ['e60_etcut_trkcut_L1EM24VHIM_xe60_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut_L1EM24VHIM","xe60"]]],
        ['e50_etcut_trkcut_L1EM24VHIM_j15_perf_xe50_6dphi15_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut_L1EM24VHIM","j15_perf","xe50"]]],
        ['e60_etcut_trkcut_L1EM24VHIM_j15_perf_xe60_6dphi15_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut_L1EM24VHIM","j15_perf","xe60"]]],
        ['e50_etcut_trkcut_L1EM24VHIM_xs30_xe30_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut_L1EM24VHIM","xs30","xe30"]]],
        ['e60_etcut_trkcut_L1EM24VHIM_xs30_xe30_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut_L1EM24VHIM","xs30","xe30"]]],
        ['e50_etcut_trkcut_L1EM24VHIM_xs30_j15_perf_xe30_6dphi15_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e50_etcut_trkcut_L1EM24VHIM","xs30","j15_perf","xe30"]]],
        ['e60_etcut_trkcut_L1EM24VHIM_xs30_j15_perf_xe30_6dphi15_mt35', 'L1_EM24VHIM', ['L1_EM24VHIM','','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e60_etcut_trkcut_L1EM24VHIM","xs30","j15_perf","xe30"]]],



        ['mu4_j15_boffperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j15_boffperf_split'] ]],
        ['mu4_j25_boffperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j25_boffperf_split'] ]],
        ['mu4_j15_gsc35_boffperf_split_dr05_dz99', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j15_gsc35_boffperf_split'] ]],
        ['mu6_j110_gsc150_boffperf_split_dr05_dz99', 'L1_MU6_J40',     ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j110_gsc150_boffperf_split'] ]],
        ['mu6_j225_gsc320_boffperf_split_dr05_dz99', 'L1_MU6_J75',     ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j225_gsc320_boffperf_split'] ]],

        ['mu4_j15_gsc35_boffperf_split', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j15_gsc35_boffperf_split'] ]],
        ['mu4_j15_gsc35_boffperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j15_gsc35_boffperf_split'] ]],
        ['mu6_j60_gsc85_boffperf_split_dr05_dz02',  'L1_MU6_J20', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j60_gsc85_boffperf_split'] ]],
        ['mu6_j60_gsc110_boffperf_split_dr05_dz02', 'L1_MU6_J20', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j60_gsc110_boffperf_split'] ]],
        ['mu6_j110_gsc150_boffperf_split_dr05_dz02', 'L1_MU6_J40', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j110_gsc150_boffperf_split'] ]],

        ['mu6_j110_gsc150_boffperf_split', 'L1_MU6_J40',     ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j110_gsc150_boffperf_split'] ]],
        ['mu6_j150_gsc175_boffperf_split_dr05_dz02', 'L1_MU6_J40', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j150_gsc175_boffperf_split']]],
        ['mu6_j175_gsc260_boffperf_split_dr05_dz02', 'L1_MU6_J75', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j175_gsc260_boffperf_split'] ]],
        ['mu6_j225_gsc320_boffperf_split_dr05_dz02', 'L1_MU6_J75', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j225_gsc320_boffperf_split'] ]],

        ['mu6_j225_gsc320_boffperf_split', 'L1_MU6_J75',     ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j225_gsc320_boffperf_split'] ]],
        ['mu6_j225_gsc400_boffperf_split_dr05_dz02', 'L1_MU6_J75', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j225_gsc400_boffperf_split'] ]],

        ### ATR-14348: L1Topo egamma chains
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi05_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', 'L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', 'L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', 'L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', ['L1_EM15','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e18_etcut_trkcut","j20_perf","xe20"]]],
        ['e18_etcut_trkcut_xs30_xe30_mt35_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', 'L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30', ['L1_EM15','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","xe30"]]],
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi05_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_xs30_j15_perf_xe30_2dphi15_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE', ['L1_EM15','','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","j15_perf","xe30"]]],
        ['e18_etcut_trkcut_j20_perf_xe20_6dphi15_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE', ['L1_EM15','',''],[PhysicsStream],['RATE:EgammaMET','BW:Egamma'],-1,['serial',-1,["e18_etcut_trkcut","j20_perf","xe20"]]],
        ['e18_etcut_trkcut_xs30_xe30_mt35_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE', 'L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE',['L1_EM15','',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma'], -1, ['serial',-1,["e18_etcut_trkcut","xs30","xe30"]]],


        ### ATR-14355: L1Topo bjet chains
        ['mu4_2j40_dr05_j40_bmv2c1077_split_antimatchdr05mu_L13J15_BTAG-MU4J15', 'L1_3J15_BTAG-MU4J15', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', '2j40', 'j40_bmv2c1077_split_antimatchdr05mu'] ], False],
        ['mu4_2j35_dr05_j35_bmv2c1070_split_antimatchdr05mu_L13J15_BTAG-MU4J15', 'L1_3J15_BTAG-MU4J15', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', '2j35', 'j35_bmv2c1070_split_antimatchdr05mu'] ], False],
        ['mu4_2j40_dr05_j40_bmv2c1077_split_antimatchdr05mu_L13J20_BTAG-MU4J20', 'L1_3J20_BTAG-MU4J20', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', '2j40', 'j40_bmv2c1077_split_antimatchdr05mu'] ], False],
        ['mu4_2j35_dr05_j35_bmv2c1070_split_antimatchdr05mu_L13J20_BTAG-MU4J20', 'L1_3J20_BTAG-MU4J20', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', '2j35', 'j35_bmv2c1070_split_antimatchdr05mu'] ], False],
        ['mu4_3j45_dr05_L13J15_BTAG-MU4J15', 'L1_3J15_BTAG-MU4J15', ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', '3j45'] ], False],
        ['mu4_3j45_dr05_L13J20_BTAG-MU4J20', 'L1_3J20_BTAG-MU4J20', ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', '3j45'] ], False],

        ['mu4_j15_gsc35_boffperf_split_dr05_dz02_L1BTAG-MU4J15', 'L1_BTAG-MU4J15', ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu4', 'j15_gsc35_boffperf_split'] ], False],
        ['mu6_j60_gsc85_boffperf_split_dr05_dz02_L1BTAG-MU6J20', 'L1_BTAG-MU6J20', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet_combined'], -1,['serial',-1,['mu6', 'j60_gsc85_boffperf_split'] ], False],
 
        #
        # BJet signal Perf chains
        #
        # (Critical for BTagging SF Calculation with ttbar !) 

        # 2mu
        ['2mu14_2j35_boffperf_split',        'L1_2MU10',   ['L1_2MU10',   ''],     [PhysicsStream], ['Primary:20000','RATE:MuonBJet',  'BW:BJet_combined'],  -1, ['serial',-1,['2mu14', '2j35_boffperf_split']]],
        ['2mu14_2j15_gsc35_boffperf_split',  'L1_2MU10',   ['L1_2MU10',   ''],     [PhysicsStream], ['Primary:20000','RATE:MuonBJet',  'BW:BJet_combined'],  -1, ['serial',-1,['2mu14', '2j15_gsc35_boffperf_split']]],

        # 2e 
        ['2e24_lhvloose_nod0_2j35_boffperf_split',         'L1_2EM20VH', ['L1_2EM20VH', ''],     [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:BJet_combined'],   -1, ['serial',-1,['2e24_lhvloose_nod0', '2j35_boffperf_split']]],
        ['2e24_lhvloose_nod0_2j15_gsc35_boffperf_split',   'L1_2EM20VH', ['L1_2EM20VH', ''],     [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:BJet_combined'],   -1, ['serial',-1,['2e24_lhvloose_nod0', '2j15_gsc35_boffperf_split']]],

        # e-mu
        ['e28_lhmedium_nod0_L1EM24VHI_mu8noL1_2j35_boffperf_split',       'L1_EM24VHI', ['L1_EM24VHI', '', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon',  'BW:BJet_combined'], -1, ['serial',-1,['e28_lhmedium_nod0_L1EM24VHI','mu8noL1','2j35_boffperf_split'] ]],
        ['e28_lhmedium_nod0_L1EM24VHI_mu8noL1_2j15_gsc35_boffperf_split', 'L1_EM24VHI', ['L1_EM24VHI', '', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon',  'BW:BJet_combined'], -1, ['serial',-1,['e28_lhmedium_nod0_L1EM24VHI','mu8noL1','2j15_gsc35_boffperf_split'] ]],

        # mu +asymmetric
        ['mu26_ivarmedium_j150_boffperf_split_j35_boffperf_split',          'L1_MU20MU21',    ['L1_MU20',    '', ''], [PhysicsStream], ['Primary:20000','RATE:MuonBJet',  'BW:BJet_combined'],  -1, ['serial',-1,['mu26_ivarmedium', 'j150_boffperf_split','j35_boffperf_split']]],
        ['mu26_ivarmedium_j110_gsc150_boffperf_split_j15_gsc35_boffperf_split', 'L1_MU20MU21',    ['L1_MU20',    '', ''], [PhysicsStream], ['Primary:20000','RATE:MuonBJet',  'BW:BJet_combined'],  -1, ['serial',-1,['mu26_ivarmedium', 'j110_gsc150_boffperf_split', 'j15_gsc35_boffperf_split']]],

        # mu + symmetric
        ['mu26_ivarmedium_2j35_boffperf_split',       'L1_MU20MU21',    ['L1_MU20',    '', ''], [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet_combined'],  -1, ['serial',-1,['mu26_ivarmedium', '2j35_boffperf_split']]],
        ['mu26_ivarmedium_2j15_gsc35_boffperf_split', 'L1_MU20MU21',    ['L1_MU20',    '', ''], [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet_combined'],  -1, ['serial',-1,['mu26_ivarmedium', '2j15_gsc35_boffperf_split']]],


        # e
        ['e28_lhtight_nod0_ivarloose_j150_boffperf_split_j35_boffperf_split',               'L1_EM24VHI', ['L1_EM24VHI', '', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:BJet_combined'],  -1, ['serial',-1,['e28_lhtight_nod0_ivarloose', 'j150_boffperf_split','j35_boffperf_split']]],
        ['e28_lhtight_nod0_ivarloose_j110_gsc150_boffperf_split_j15_gsc35_boffperf_split',  'L1_EM24VHI', ['L1_EM24VHI', '', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:BJet_combined'],  -1, ['serial',-1,['e28_lhtight_nod0_ivarloose', 'j110_gsc150_boffperf_split','j15_gsc35_boffperf_split']]],
        ['e26_lhtight_nod0_ivarloose_L1EM22VHIM_2j35_boffperf_split',              'L1_EM22VHIM', ['L1_EM22VHIM', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet_combined'],  -1, ['serial',-1,['e26_lhtight_nod0_ivarloose_L1EM22VHIM', '2j35_boffperf_split']]],
        ['e26_lhtight_nod0_ivarloose_L1EM22VHIM_2j15_gsc35_boffperf_split',        'L1_EM22VHIM', ['L1_EM22VHIM', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet_combined'],  -1, ['serial',-1,['e26_lhtight_nod0_ivarloose_L1EM22VHIM', '2j15_gsc35_boffperf_split']]],
        ['e28_lhtight_nod0_ivarloose_L1EM24VHIM_2j35_boffperf_split',              'L1_EM24VHIM', ['L1_EM24VHIM', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet_combined'],  -1, ['serial',-1,['e28_lhtight_nod0_ivarloose_L1EM24VHIM', '2j35_boffperf_split']]],
        ['e28_lhtight_nod0_ivarloose_L1EM24VHIM_2j15_gsc35_boffperf_split',        'L1_EM24VHIM', ['L1_EM24VHIM', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet_combined'],  -1, ['serial',-1,['e28_lhtight_nod0_ivarloose_L1EM24VHIM', '2j15_gsc35_boffperf_split']]],
        ['2g10_loose_mu20',          'L1_MU20MU21',    ['L1_2EM7', 'L1_MU20'],  [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['2g10_loose','mu20'] ]],
        ['2g10_medium_mu20',         'L1_MU20MU21',    ['L1_2EM7', 'L1_MU20'],  [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['2g10_medium','mu20'] ]],

        # LLP trigger seeded by MUON items
        ['j30_muvtx',                    'L1_2MU10',            [], [PhysicsStream], ['Primary:20000','RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_noiso',              'L1_2MU10',            [], [PhysicsStream], ['Primary:20000','RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_L1MU6_EMPTY',        'L1_MU6_EMPTY',        [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO', [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],

        # LLP trigger seeded by TAU items
        ['j30_jes_PS_llp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_PS_llp_noiso_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU100',             'L1_TAU100',             [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],
      

        ['j30_jes_cleanLLP_PS_llp_L1TAU30_EMPTY',        'L1_TAU30_EMPTY',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU30_UNPAIRED_ISO', 'L1_TAU30_UNPAIRED_ISO', [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j30_jes_cleanLLP_PS_llp_pufix_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_pufix_noiso_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU100',       'L1_TAU100',             [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1],

        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_EMPTY',        'L1_TAU30_EMPTY',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_UNPAIRED_ISO',  'L1_TAU30_UNPAIRED_ISO',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],

        # L1Topo LLP with TAU
        ['j30_jes_cleanLLP_PS_llp_L1LLP-NOMATCH',       'L1_LLP-NOMATCH',       [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1, False],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1LLP-NOMATCH', 'L1_LLP-NOMATCH',       [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1, False],

        ['j30_jes_cleanLLP_PS_llp_L1LLP-RO',       'L1_LLP-RO',       [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1, False],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1LLP-RO', 'L1_LLP-RO',       [], [PhysicsStream], ['Primary:20000','RATE:SingleJet', 'BW:Jet'], -1, False],

        # reversed calo-ratio triggers
        ['j250_jes_cleanLLP_PS_revllp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j230_jes_cleanLLP_PS_revllp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j200_jes_cleanLLP_PS_revllp_trkiso_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j30_muvtx_noiso_L1MU6_EMPTY',        'L1_MU6_EMPTY',        [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_noiso_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO', [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],

        # LLP triggers
        ['g15_loose_2mu10_msonly_L1MU6_EMPTY',        'L1_MU6_EMPTY',['L1_EM8VH','L1_2MU10'], ["Late"], ['RATE:EgammaMuon', 'BW:Egamma','BW:Muon'],-1,['parallel',-1,[] ]],
        ['g15_loose_2mu10_msonly_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO',['L1_EM8VH','L1_2MU10'], ["Late"], ['RATE:EgammaMuon', 'BW:Egamma','BW:Muon'],-1,['parallel',-1,[] ]],

        ['j30_xe10_razor170', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe10']]],

        #razor triggers
        ['j30_xe10_razor100', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe10']]],
        ['j30_xe10_razor185', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe10']]],
        ['j30_xe10_razor195', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe10']]],

        # tighter razor triggers
        ['j30_xe60_razor100', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe60']]],
        ['j30_xe60_razor170', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe60']]],
        ['j30_xe60_razor185', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe60']]],
        ['j30_xe60_razor195', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe60']]],
        #ATR-14196
        ['j30_xe60_razor220', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['Primary:20000','RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe60']]],

        ['j55_0eta240_xe50_L1J30_EMPTY', 'L1_J30_EMPTY', ['',''], ['Late'], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j55_0eta240','xe50']]],
        ['g20_etcut_L1EM15_mu4_taumass',      'L1_EM15I_MU4',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_etcut_L1EM15","mu4"]] ],

        ['g10_loose_L1EM8I_mu10_iloose_taumass',      'L1_EM8I_MU10',  ['L1_EM8I', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_loose_L1EM8I","mu10_iloose"]] ],
        ['g20_loose_L1EM15_mu4_iloose_taumass',      'L1_EM15I_MU4',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_loose_L1EM15","mu4_iloose"]] ],
        ['g10_etcut_L1EM8I_mu10_iloose_taumass',      'L1_EM8I_MU10',  ['L1_EM8I', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_etcut_L1EM8I","mu10_iloose"]] ],

        ## LFV chains (ATR-14352)
        ['g10_loose_mu10_iloose_taumass_L1LFV-EM8I', 'L1_LFV-EM8I',  ['L1_EM7', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_loose","mu10_iloose"]] ],
        ['g20_loose_mu4_iloose_taumass_L1LFV-EM15I', 'L1_LFV-EM15I',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_loose","mu4_iloose"]] ],
        
        ### ATR-14543
        ['g10_loose_mu10_ivarloose_taumass_L1LFV-EM8I', 'L1_LFV-EM8I',  ['L1_EM7', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_loose","mu10_ivarloose"]] ],
        ['g20_loose_mu4_ivarloose_taumass_L1LFV-EM15I', 'L1_LFV-EM15I',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_loose","mu4_ivarloose"]] ],
        ['g35_tight_icalotight_L1EM24VHIM_mu18noL1',        'L1_EM24VHIM', ['L1_EM24VHIM', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_tight_icalotight_L1EM24VHIM','mu18noL1'] ]],
        ['g40_tight_icalotight_L1EM24VHIM_mu18noL1',        'L1_EM24VHIM', ['L1_EM24VHIM', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g40_tight_icalotight_L1EM24VHIM','mu18noL1'] ]],
        ['g35_tight_icalotight_L1EM24VHIM_mu15noL1_mu2noL1', 'L1_EM24VHIM', ['L1_EM24VHIM', '',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_tight_icalotight_L1EM24VHIM','mu15noL1','mu2noL1']]],
        ['g40_tight_icalotight_L1EM24VHIM_mu15noL1_mu2noL1', 'L1_EM24VHIM', ['L1_EM24VHIM', '',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g40_tight_icalotight_L1EM24VHIM','mu15noL1','mu2noL1']]],
 
        #ATR-11623 photon + muon/di-muon chains L1_EM24VHI and L1_EM24VHIM seed
        ['g35_tight_icalotight_L1EM24VHI_mu18noL1',        'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_tight_icalotight_L1EM24VHI','mu18noL1'] ]],
        ['g35_tight_icalotight_L1EM24VHI_mu15noL1_mu2noL1', 'L1_EM24VHI', ['L1_EM24VHI', '',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_tight_icalotight_L1EM24VHI','mu15noL1','mu2noL1']]],

        ['g40_tight_icalotight_L1EM24VHI_mu18noL1',        'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g40_tight_icalotight_L1EM24VHI','mu18noL1'] ]],
        ['g40_tight_icalotight_L1EM24VHI_mu15noL1_mu2noL1', 'L1_EM24VHI', ['L1_EM24VHI', '',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g40_tight_icalotight_L1EM24VHI','mu15noL1','mu2noL1']]],

        ['g35_loose_L1EM24VHI_mu18',        'L1_EM24VHI', ['L1_EM24VHI', 'L1_MU11'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM24VHI','mu18'] ]],
        ['g35_loose_L1EM24VHI_mu15_mu2noL1', 'L1_EM24VHI', ['L1_EM24VHI', 'L1_MU11',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM24VHI','mu15','mu2noL1']]],

        ['g35_loose_L1EM24VHIM_mu18',        'L1_EM24VHIM', ['L1_EM24VHIM', 'L1_MU11'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM24VHIM','mu18'] ]],
        ['g35_loose_L1EM24VHIM_mu15_mu2noL1', 'L1_EM24VHIM', ['L1_EM24VHIM', 'L1_MU11',''], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM24VHIM','mu15','mu2noL1']]],

        # Heg chains
        ['e25_mergedtight_g35_medium_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e30_mergedtight_g35_medium_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],

        # ATR-16089 Heg chains
        ['e25_mergedtight_g35_medium_icalovloose_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e25_mergedtight_g35_medium_icalotight_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e25_mergedtight_ivarloose_g35_medium_icalovloose_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['Primary:20000','RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e25_mergedtight_ivarloose_g35_medium_icalotight_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
 
        ['e30_mergedtight_g35_medium_icalovloose_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e30_mergedtight_g35_medium_icalotight_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e30_mergedtight_ivarloose_g35_medium_icalovloose_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e30_mergedtight_ivarloose_g35_medium_icalotight_Heg',       'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
	# new chain for 2e34 backup (ATR-15225)
        ['e24_lhmedium_nod0_ivarloose_tau35_medium1_tracktwo',  'L1_EM20VHI_TAU20IM_2TAU20_J25_3J20',['L1_EM20VHI','L1_TAU20IM'], [PhysicsStream], ['Primary:20000','RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e24_lhmedium_nod0_ivarloose","tau35_medium1_tracktwo"]]],
	                                               
        ['e17_lhmedium_nod0_tau80_medium1_tracktwo',   'L1_EM15VHI_TAU40_2TAU15',['L1_EM15VHI','L1_TAU40'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0","tau80_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo',  'L1_EM15VHI_2TAU12IM_J25_3J12',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream],['Primary:17000','RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],

        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15VHI_2TAU12IM_4J12',   'L1_EM15VHI_2TAU12IM_4J12',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream],['Primary:20000','RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15VHI_2TAU12IM',  'L1_EM15VHI_2TAU12IM',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],

	#L1Topo e+tau chains
        ['e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25',  'L1_DR-EM15TAU12I-J25',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1 ,["e17_lhmedium_nod0_iloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25', 'L1_DR-EM15TAU12I-J25',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:ElectronTau', 'BW:Tau'],-1,['serial',-1 ,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25', 'L1_EM15TAU12I-J25',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_tau25_medium1_tracktwo_L1EM15TAU12I-J25',  'L1_EM15TAU12I-J25',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',  'L1_EM15TAU12I-J25',['L1_EM15VHI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_iloose","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau35_medium1_tracktwo',  'L1_MU10_TAU20IM',['L1_MU10','L1_TAU20IM'], [PhysicsStream], ['Primary:17000','RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau35_medium1_tracktwo"]]],
        ['mu14_ivarloose_L1MU11_tau35_medium1_tracktwo_L1MU11_TAU20IM',  'L1_MU11_TAU20IM',['L1_MU11','L1_TAU20IM'], [PhysicsStream], ['Primary:20000','RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose_L1MU11","tau35_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau35_medium1_tracktwo_L1MU10_TAU20IM_J25_2J20',      'L1_MU10_TAU20IM_J25_2J20',['L1_MU10','L1_TAU20IM'], [PhysicsStream], ['Primary:20000','RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau35_medium1_tracktwo"]]],   

        ['mu14_iloose_tau25_perf_tracktwo', 'L1_MU10_TAU12IM',['L1_MU10','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_iloose","tau25_perf_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo',      'L1_MU10_TAU12IM_J25_2J12',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['Primary:17000','RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],        
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12IM_3J12',      'L1_MU10_TAU12IM_3J12',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12IM',  'L1_MU10_TAU12IM',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],

        # l1topo tau+mu chains
        ['mu14_tau25_medium1_tracktwo_L1DR-MU10TAU12I',  'L1_DR-MU10TAU12I',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25', 'L1_DR-MU10TAU12I_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['Primary:20000','RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],
        ['mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25', 'L1_DR-MU10TAU12I_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],
        ['mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I', 'L1_DR-MU10TAU12I',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I', 'L1_DR-MU10TAU12I',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25', 'L1_MU10_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],
        ['mu14_iloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25', 'L1_MU10_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],

        
        # Tau + MET
        ['tau35_medium1_tracktwo_xe70_L1XE45',  'L1_TAU20IM_2J20_XE45',['L1_TAU20IM','L1_XE45'], [PhysicsStream], ['Primary:20000','RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","xe70_L1XE45"]]],  
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_xe50', 'L1_TAU20IM_2TAU12IM_XE35',['L1_TAU20IM','L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo","xe50"]]],
        ['tau60_medium1_tracktwo_tau25_medium1_tracktwo_xe50', 'L1_TAU40_2TAU12IM_XE40',['L1_TAU40','L1_TAU12IM','L1_XE40'], [PhysicsStream], ['Primary:20000','RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau60_medium1_tracktwo","tau25_medium1_tracktwo","xe50"]]],
 
        # Tau + e + MET
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_xe50', 'L1_EM15VHI_2TAU12IM_XE35',['L1_EM15VHI','L1_TAU12IM','L1_XE35'], [PhysicsStream], ['Primary:20000','RATE:TauMET', 'BW:Tau', 'BW:Egamma'],-1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo", "xe50"]]],
        ['e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50', 'L1_EM15VHI_2TAU12IM_XE35',['L1_EM15VHI','L1_TAU12IM','L1_XE35'], [PhysicsStream], ['Primary:20000','RATE:TauMET', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0","tau25_medium1_tracktwo", "xe50"]]],


        # Tau + mu + MET
        ['mu14_tau25_medium1_tracktwo_xe50', 'L1_MU10_TAU12IM_XE35',['L1_MU10', 'L1_TAU12IM','L1_XE35'], [PhysicsStream], ['Primary:20000','RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["mu14", "tau25_medium1_tracktwo","xe50"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_xe50', 'L1_MU10_TAU12IM_XE35',['L1_MU10', 'L1_TAU12IM','L1_XE35'], [PhysicsStream], ['Primary:20000','RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose", "tau25_medium1_tracktwo","xe50"]]],


        ['j80_xe80',               'L1_J40_XE50',['',''], [PhysicsStream, 'express'], ['RATE:JetMET', 'BW:Jet', 'BW:MET'], -1,['serial',-1,["j80","xe80"]]], 

        # lepton + jets 
        ['e20_lhtight_nod0_3j20_L1EM18VHI_3J20',      'L1_EM18VHI_3J20',    ['L1_EM18VHI_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e20_lhtight_nod0","3j20"] ]],

        ### ATR-15524
        ['e20_lhtight_nod0_ivarloose_3j20_L1EM18VHI_3J20', 'L1_EM18VHI_3J20',    ['L1_EM18VHI_3J20',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e20_lhtight_nod0_ivarloose","3j20"] ]],
        ['e20_lhmedium_nod0_3j20_L1EM18VHI_3J20', 'L1_EM18VHI_3J20',    ['L1_EM18VHI_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e20_lhmedium_nod0","3j20"] ]],
         
         
         
        #nod0 chains:
        ['e28_lhvloose_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","3j20noL1"] ]],
        ['e28_lhvloose_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","4j20noL1"] ]],
        ['e28_lhvloose_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","5j15noL1"] ]],
        ['e28_lhvloose_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","6j15noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","3j20noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","4j20noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","5j15noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","6j15noL1"] ]],
				# e+jets ATR-14404
        ['e28_lhmedium_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","3j20noL1"] ]],
        ['e28_lhmedium_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","4j20noL1"] ]],
        ['e28_lhmedium_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","5j15noL1"] ]],
        ['e28_lhmedium_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","6j15noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","3j20noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","4j20noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","5j15noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","6j15noL1"] ]],


        ['mu26_2j20noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","2j20noL1"] ]],
        ['mu26_3j20noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","3j20noL1"] ]],
        ['mu26_4j15noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","4j15noL1"] ]],
        ['mu26_5j15noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","5j15noL1"] ]],
         
        ['mu24_2j20noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","2j20noL1"] ]],
        ['mu24_3j20noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","3j20noL1"] ]],
        ['mu24_4j15noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","4j15noL1"] ]],
        ['mu24_5j15noL1',  'L1_MU20MU21', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","5j15noL1"] ]],

        
        # VBF triggers

        ['g25_medium_L1EM22VHI_4j35_0eta490_invm700', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma'], -1,['serial',-1,["g25_medium_L1EM22VHI","4j35_0eta490_invm700"]]],
        #ATR-14196
        ['g25_medium_L1EM22VHI_4j35_0eta490_invm1000', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:Egamma'], -1,['serial',-1,["g25_medium_L1EM22VHI","4j35_0eta490_invm1000"]]],
        ['g25_loose_L1EM20VH_4j35_0eta490', 'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma'], -1,['serial',-1,["g25_loose_L1EM20VH","4j35_0eta490"]]],

	
#        ['g25_medium_L1EM22VHI_j35_0eta490_bmv2c1077_split_3j35_0eta490_invm700', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet_combined','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', 'j35_0eta490_bmv2c1077_split','3j35_0eta490_invm700']]],
#        ['g25_medium_L1EM22VHI_2j35_0eta490_bmv2c1077_split_2j35_0eta490', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet_combined','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', '2j35_0eta490_bmv2c1077_split','2j35_0eta490']]],	
        ['g25_medium_L1EM22VHI_j35_0eta490_bmv2c1077_split_3j35_0eta490_invm700', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:BJet_combined','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI','3j35_0eta490_invm700','j35_0eta490_bmv2c1077_split']]],
        ['g25_medium_L1EM22VHI_2j35_0eta490_bmv2c1077_split_2j35_0eta490', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:BJet_combined','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI','2j35_0eta490','2j35_0eta490_bmv2c1077_split']]],


        #ATR-15062
#        ['g20_tight_icaloloose_j15_gsc35_bmv2c1077_split_3j35_0eta490_invm500', 'L1_EM18VHI_MJJ-300', ['','',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g20_tight_icaloloose","j15_gsc35_bmv2c1077_split","3j35_0eta490_invm500"]]],
#        ['g27_medium_L1EM24VHI_j15_gsc35_bmv2c1077_split_3j35_0eta490_invm700', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g27_medium_L1EM24VHI","j15_gsc35_bmv2c1077_split","3j35_0eta490_invm700"]]],        
#        ['g27_medium_L1EM24VHI_2j15_gsc35_bmv2c1077_split_2j35_0eta490', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g27_medium_L1EM24VHI","2j15_gsc35_bmv2c1077_split","2j35_0eta490"]]],
        ['g20_tight_icaloloose_j15_gsc35_bmv2c1077_split_3j35_0eta490_invm500', 'L1_EM18VHI_MJJ-300', ['','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g20_tight_icaloloose","3j35_0eta490_invm500","j15_gsc35_bmv2c1077_split"]]],
        ['g27_medium_L1EM24VHI_j15_gsc35_bmv2c1077_split_3j35_0eta490_invm700', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g27_medium_L1EM24VHI","3j35_0eta490_invm700","j15_gsc35_bmv2c1077_split"]]],
        ['g27_medium_L1EM24VHI_2j15_gsc35_bmv2c1077_split_2j35_0eta490', 'L1_EM24VHI', ['L1_EM24VHI','',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g27_medium_L1EM24VHI","2j35_0eta490","2j15_gsc35_bmv2c1077_split"]]],


        ### ATR-14356: egamma+jets VBF chians
        ['g25_medium_L1EM22VHI_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_EM22VHI', ['L1_EM22VHI', 'L1_HT150-JJ15.ETA49_MJJ-400'], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', '2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400']]],

        # backups for L1Topo VBF items
        ['mu6_2j40_0eta490_invm400_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6","2j40_0eta490_invm400"]]],
        ['mu6_2j40_0eta490_invm600_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6","2j40_0eta490_invm600"]]],
        ['mu6_2j40_0eta490_invm800_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6","2j40_0eta490_invm800"]]],
        ['j80_0eta240_j60_j45_320eta490_AND_2j25_gsc45_bmv2c1070_split','L1_J40.0ETA25_2J25_J20.31ETA49', ['','','','',''], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet_combined' ], -1, ['serial',-1,['j80_0eta240','j60','j45_320eta490_AND','2j25_gsc45_bmv2c1070_split'] ]],

#        ['j55_bmv2c1070_split_2j45_320eta490_L1J25.0ETA23_2J15.31ETA49','L1_J25.0ETA23_2J15.31ETA49',['',''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined' ], -1, ['serial',-1,['j55_bmv2c1070_split','2j45_320eta490_L1J25.0ETA23_2J15.31ETA49'] ]],
#        ['j35_gsc55_bmv2c1070_split_2j45_320eta490_L1J25.0ETA23_2J15.31ETA49','L1_J25.0ETA23_2J15.31ETA49',['',''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined' ], -1, ['serial',-1,['j35_gsc55_bmv2c1070_split','2j45_320eta490_L1J25.0ETA23_2J15.31ETA49'] ]],
	['j55_0eta240_2j45_320eta490_L1J25.0ETA23_2J15.31ETA49','L1_J25.0ETA23_2J15.31ETA49',['',''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet_combined' ], -1, ['serial',-1,['j55_0eta240','2j45_320eta490_L1J25.0ETA23_2J15.31ETA49'] ]],
        ['j35_gsc55_bmv2c1070_split_2j45_320eta490_L1J25.0ETA23_2J15.31ETA49','L1_J25.0ETA23_2J15.31ETA49',['',''], [PhysicsStream], ['Primary:20000','RATE:MultiBJet', 'BW:BJet_combined' ], -1, ['serial',-1,['2j45_320eta490_L1J25.0ETA23_2J15.31ETA49','j35_gsc55_bmv2c1070_split'] ]],


        ['j40_0eta490_j30_0eta490_deta25_xe80_L1XE50',    'L1_XE50', ['','',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j40_0eta490","j30_0eta490_deta25","xe80_L1XE50"]]],
 
        # 2017 g+tau chains (ATR-16266)
	# ATR-14947
        ['g25_medium_L1EM24VHIM_tau25_dikaonmass_tracktwo_50mVis10000','L1_EM24VHIM', ['L1_EM24VHIM','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHIM','tau25_dikaonmass_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_dikaonmass_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_dikaonmass_tracktwo'],False]],
        ['g25_medium_L1EM24VHIM_tau25_singlepion_tracktwo_50mVis10000','L1_EM24VHIM', ['L1_EM24VHIM','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHIM','tau25_singlepion_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_singlepion_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_singlepion_tracktwo'],False]],
        ['g25_medium_L1EM24VHIM_tau25_dipion1loose_tracktwo_50mVis10000','L1_EM24VHIM', ['L1_EM24VHIM','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHIM','tau25_dipion1loose_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_dipion1loose_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_dipion1loose_tracktwo'],False]],
        ['g25_medium_L1EM24VHIM_tau25_dipion2_tracktwo_50mVis10000','L1_EM24VHIM', ['L1_EM24VHIM','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHIM','tau25_dipion2_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_dipion2_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_dipion2_tracktwo'],False]],
        ['g25_medium_L1EM24VHIM_tau25_kaonpi1_tracktwo_50mVis10000','L1_EM24VHIM', ['L1_EM24VHIM','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHIM','tau25_kaonpi1_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_kaonpi1_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_kaonpi1_tracktwo'],False]],
        ['g25_medium_L1EM24VHIM_tau25_kaonpi2_tracktwo_50mVis10000','L1_EM24VHIM', ['L1_EM24VHIM','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHIM','tau25_kaonpi2_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_kaonpi2_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_kaonpi2_tracktwo'],False]],	
        # new L1topo g+tau chain (ATR-14349)
        ['g25_medium_tau25_dikaonmass_tracktwo_50mVis10000_L130M-EM20ITAU12','L1_30M-EM20ITAU12', ['L1_EM20VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_dikaonmass_tracktwo'],False]],
        ['g25_medium_tau25_singlepion_tracktwo_50mVis10000_L130M-EM20ITAU12','L1_30M-EM20ITAU12', ['L1_EM20VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_singlepion_tracktwo'],False]],
        ['g25_medium_tau25_dipion1loose_tracktwo_50mVis10000_L130M-EM20ITAU12','L1_30M-EM20ITAU12', ['L1_EM20VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_dipion1loose_tracktwo'],False]],
        ['g25_medium_tau25_dipion2_tracktwo_50mVis10000_L130M-EM20ITAU12','L1_30M-EM20ITAU12', ['L1_EM20VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_dipion2_tracktwo'],False]],
        ['g25_medium_tau25_kaonpi1_tracktwo_50mVis10000_L130M-EM20ITAU12','L1_30M-EM20ITAU12', ['L1_EM20VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_kaonpi1_tracktwo'],False]],
        ['g25_medium_tau25_kaonpi2_tracktwo_50mVis10000_L130M-EM20ITAU12','L1_30M-EM20ITAU12', ['L1_EM20VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_kaonpi2_tracktwo'],False]],

        ### ATR-14196
        ['g85_tight_L1EM24VHIM_3j50noL1', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight_L1EM24VHIM","3j50noL1"]]],
        ['g85_tight_icalovloose_L1EM24VHIM_3j50noL1', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight_icalovloose_L1EM24VHIM","3j50noL1"]]],
        ['g100_tight_L1EM24VHIM_3j50noL1', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight_L1EM24VHIM","3j50noL1"]]],
        ['g100_tight_icalovloose_L1EM24VHIM_3j50noL1', 'L1_EM24VHIM', ['L1_EM24VHIM',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight_icalovloose_L1EM24VHIM","3j50noL1"]]],
       	['g85_tight_L1EM22VHI_3j50noL1', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight_L1EM22VHI","3j50noL1"]]],
        ['g85_tight_L1EM24VHI_3j50noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight_L1EM24VHI","3j50noL1"]]],
        ['g100_tight_L1EM22VHI_3j50noL1', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['Primary:17000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight_L1EM22VHI","3j50noL1"]]],
        ['g100_tight_L1EM24VHI_3j50noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['Primary:20000','RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight_L1EM24VHI","3j50noL1"]]],

        ['g85_tight', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g85_tight_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g100_tight', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g100_tight_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
     #ATR-14196
        ['j80_bmv2c1050_split_xe60_L12J50_XE40','L1_2J50_XE40',  [],  [PhysicsStream], ['Primary:17000','RATE:JetMET', 'BW:MET', 'BW:BJet_combined'], -1,['serial',-1,["xe60","j80_bmv2c1050_split"]]],
        ['j40_cleanT_xe85_tc_lcw_2dphi05_L1XE50',         'L1_XE50', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j40_cleanT","xe85_tc_lcw_L1XE50"]]],
        ['mu14_ivarloose_3j20_L1MU10_3J20', 'L1_MU10_3J20', ['L1_MU10_3J20', ''], [PhysicsStream], ['Primary:20000','RATE:MuonJet', 'BW:Muon','BW:Jet'], -1,['serial',-1,['mu14_ivarloose', '3j20'] ],True],
        ### ATR-15524
        ['mu14_3j20_L1MU10_3J20', 'L1_MU10_3J20', ['L1_MU10_3J20', ''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon','BW:Jet'], -1,['serial',-1,['mu14', '3j20'] ],True],

        
        
        ['mu6_ivarloose_2j40_0eta490_invm1000_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6_ivarloose","2j40_0eta490_invm1000"]]],

        #mujet matching chain 
        ['g15_loose_2mu10_msonly_L1MU4_EMPTY',        'L1_MU4_EMPTY',['L1_EM8VH','L1_2MU10'], ["Late"], ['RATE:EgammaMuon', 'BW:Egamma','BW:Muon'],-1,['parallel',-1,[] ]],

        # with pi0's (ATR-16600)
        ['g35_medium_L1EM24VHI_tau25_dipion3_tracktwo_60mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['Primary:20000','RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g35_medium_L1EM24VHI','tau25_dipion3_tracktwo'],False]],

        ## Lepton + jets with unisolated leptons:
        # For above 1.0 e34
        ['e26_lhmedium_nod0_5j30_0eta240_L1EM20VH_3J20',    	'L1_EM20VH_3J20',    ['L1_EM20VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e26_lhmedium_nod0","5j30_0eta240"] ]],
        ['mu26_5j30_0eta240_L1MU10_3J20',    	'L1_MU10_3J20',    ['L1_MU10_3J20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'],-1, ['serial',-1,["mu26","5j30_0eta240"] ]],
        # For below 1.0 e34
        ['e24_lhmedium_nod0_5j30_0eta240_L1EM20VH_3J20',    	'L1_EM20VH_3J20',    ['L1_EM20VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e24_lhmedium_nod0","5j30_0eta240"] ]],
        ['mu24_5j30_0eta240_L1MU10_3J20',    	'L1_MU10_3J20',    ['L1_MU10_3J20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'],-1, ['serial',-1,["mu24","5j30_0eta240"] ]],
        ['mu20_mu8noL1_e9_lhvloose_nod0',        'L1_MU20MU21',  ['L1_MU20','','L1_EM3'],   [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['serial',-1,['mu20','mu8noL1','e9_lhvloose_nod0'] ]],
        ['mu20_mu8noL1_e9_lhvloose_nod0_L1EM8VH_MU20',  'L1_EM8VH_MU20',['L1_MU20','','L1_EM8VH'], [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['serial',-1,['mu20','mu8noL1','e9_lhvloose_nod0'] ]],
        ['mu20_mu8noL1_e9_lhvloose_nod0_L1EM7_MU20',    'L1_EM7_MU20',  ['L1_MU20','','L1_EM7'],   [PhysicsStream], ['Primary:20000','RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['serial',-1,['mu20','mu8noL1','e9_lhvloose_nod0'] ]],

        #HMT + muons
        ['mu4_mb_sp600_trk40_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp600_trk40_hmt'],False]],
        ['mu4_mb_sp700_trk50_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp700_trk50_hmt'],False]],
        ['mu4_mb_sp900_trk60_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp900_trk60_hmt'],False]],
        ['mu4_mb_sp1100_trk70_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1100_trk70_hmt'],False]],
        ['mu4_mb_sp1200_trk80_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_trk80_hmt'],False]],
        ['mu4_mb_sp1400_trk90_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_trk90_hmt'],False]],
        ['mu4_mb_sp1600_trk100_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_trk100_hmt'],False]],

        ['mu4_mb_sp600_pusup300_trk40_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp600_pusup300_trk40_hmt'],False]],
        ['mu4_mb_sp700_pusup350_trk50_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp700_pusup350_trk50_hmt'],False]],
        ['mu4_mb_sp900_pusup400_trk60_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp900_pusup400_trk60_hmt'],False]],
        ['mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE10', 'L1_MU4_TE10', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        
        ['mu4_mb_sp1100_trk70_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1100_trk70_hmt'],False]],
        ['mu4_mb_sp1200_trk80_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_trk80_hmt'],False]],
        ['mu4_mb_sp1400_trk90_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_trk90_hmt'],False]],
        ['mu4_mb_sp1600_trk100_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_trk100_hmt'],False]],
        ['mu4_mb_sp1700_trk110_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_trk110_hmt'],False]],
        
        ['mu4_mb_sp1100_trk70_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1100_trk70_hmt'],False]],
        ['mu4_mb_sp1200_trk80_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_trk80_hmt'],False]],
        ['mu4_mb_sp1400_trk90_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_trk90_hmt'],False]],
        ['mu4_mb_sp1600_trk100_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_trk100_hmt'],False]],
        ['mu4_mb_sp1700_trk110_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_trk110_hmt'],False]],
        
        ['mu4_mb_sp1200_trk80_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_trk80_hmt'],False]],
        ['mu4_mb_sp1400_trk90_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_trk90_hmt'],False]],
        ['mu4_mb_sp1600_trk100_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_trk100_hmt'],False]],
        ['mu4_mb_sp1700_trk110_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_trk110_hmt'],False]],
        #['mu4_mb_sp1400_trk90_hmt_L1MU4_TE50', 'L1_MU4_TE50', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_trk90_hmt'],False]],
        #['mu4_mb_sp1600_trk100_hmt_L1MU4_TE50', 'L1_MU4_TE50', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_trk100_hmt'],False]],
        #['mu4_mb_sp1700_trk110_hmt_L1MU4_TE50', 'L1_MU4_TE50', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_trk110_hmt'],False]],

        #['mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE50', 'L1_MU4_TE50', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        #['mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE50', 'L1_MU4_TE50', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        #['mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE50', 'L1_MU4_TE50', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_pusup650_trk110_hmt'],False]],
        ['mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        ['mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE20', 'L1_MU4_TE20', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_pusup650_trk110_hmt'],False]],
		
        ['mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        ['mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE30', 'L1_MU4_TE30', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_pusup650_trk110_hmt'],False]],
		
        ['mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        ['mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE40', 'L1_MU4_TE40', ['', ''], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['mu4', 'mb_sp1700_pusup650_trk110_hmt'],False]],
		
		#HMT + jets
		['j40_mb_sp600_pusup300_trk40_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp600_pusup300_trk40_hmt'],False]],
        ['j40_mb_sp700_pusup350_trk50_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp700_pusup350_trk50_hmt'],False]],
        ['j40_mb_sp900_pusup400_trk60_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp900_pusup400_trk60_hmt'],False]],
        ['j40_mb_sp1100_pusup450_trk70_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['j40_mb_sp1200_pusup500_trk80_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['j40_mb_sp1400_pusup550_trk90_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['j40_mb_sp1600_pusup600_trk100_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        
        ['j40_mb_sp600_trk40_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp600_trk40_hmt'],False]],
        ['j40_mb_sp700_trk50_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp700_trk50_hmt'],False]],
        ['j40_mb_sp900_trk60_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp900_trk60_hmt'],False]],
        ['j40_mb_sp1100_trk70_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1100_trk70_hmt'],False]],
        ['j40_mb_sp1200_trk80_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1200_trk80_hmt'],False]],
        ['j40_mb_sp1400_trk90_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1400_trk90_hmt'],False]],
        ['j40_mb_sp1600_trk100_hmt', 'L1_J12',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j40', 'mb_sp1600_trk100_hmt'],False]],
        
        ['j50_mb_sp600_pusup300_trk40_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp600_pusup300_trk40_hmt'],False]],
        ['j50_mb_sp700_pusup350_trk50_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp700_pusup350_trk50_hmt'],False]],
        ['j50_mb_sp900_pusup400_trk60_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp900_pusup400_trk60_hmt'],False]],
        ['j50_mb_sp1100_pusup450_trk70_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['j50_mb_sp1200_pusup500_trk80_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['j50_mb_sp1400_pusup550_trk90_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['j50_mb_sp1600_pusup600_trk100_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        
        ['j50_mb_sp600_trk40_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp600_trk40_hmt'],False]],
        ['j50_mb_sp700_trk50_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp700_trk50_hmt'],False]],
        ['j50_mb_sp900_trk60_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp900_trk60_hmt'],False]],
        ['j50_mb_sp1100_trk70_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1100_trk70_hmt'],False]],
        ['j50_mb_sp1200_trk80_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1200_trk80_hmt'],False]],
        ['j50_mb_sp1400_trk90_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1400_trk90_hmt'],False]],
        ['j50_mb_sp1600_trk100_hmt', 'L1_J15',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j50', 'mb_sp1600_trk100_hmt'],False]],
        
        ['j60_mb_sp600_pusup300_trk40_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp600_pusup300_trk40_hmt'],False]],
        ['j60_mb_sp700_pusup350_trk50_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp700_pusup350_trk50_hmt'],False]],
        ['j60_mb_sp900_pusup400_trk60_hmt', 'L1_J20',  ['', ''], ['MinBias','express'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp900_pusup400_trk60_hmt'],False]],
        ['j60_mb_sp1100_pusup450_trk70_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['j60_mb_sp1200_pusup500_trk80_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['j60_mb_sp1400_pusup550_trk90_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['j60_mb_sp1600_pusup600_trk100_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        
        ['j60_mb_sp600_trk40_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp600_trk40_hmt'],False]],
        ['j60_mb_sp700_trk50_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp700_trk50_hmt'],False]],
        ['j60_mb_sp900_trk60_hmt', 'L1_J20',  ['', ''], ['MinBias','express'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp900_trk60_hmt'],False]],
        ['j60_mb_sp1100_trk70_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1100_trk70_hmt'],False]],
        ['j60_mb_sp1200_trk80_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1200_trk80_hmt'],False]],
        ['j60_mb_sp1400_trk90_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1400_trk90_hmt'],False]],
        ['j60_mb_sp1600_trk100_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j60', 'mb_sp1600_trk100_hmt'],False]],
        
        ['j75_mb_sp600_pusup300_trk40_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp600_pusup300_trk40_hmt'],False]],
        ['j75_mb_sp700_pusup350_trk50_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp700_pusup350_trk50_hmt'],False]],
        ['j75_mb_sp900_pusup400_trk60_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp900_pusup400_trk60_hmt'],False]],
        ['j75_mb_sp1100_pusup450_trk70_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['j75_mb_sp1200_pusup500_trk80_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['j75_mb_sp1400_pusup550_trk90_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['j75_mb_sp1600_pusup600_trk100_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        
        ['j75_mb_sp600_trk40_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp600_trk40_hmt'],False]],
        ['j75_mb_sp700_trk50_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp700_trk50_hmt'],False]],
        ['j75_mb_sp900_trk60_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp900_trk60_hmt'],False]],
        ['j75_mb_sp1100_trk70_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1100_trk70_hmt'],False]],
        ['j75_mb_sp1200_trk80_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1200_trk80_hmt'],False]],
        ['j75_mb_sp1400_trk90_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1400_trk90_hmt'],False]],
        ['j75_mb_sp1600_trk100_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j75', 'mb_sp1600_trk100_hmt'],False]],
        
        ['j85_mb_sp600_pusup300_trk40_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp600_pusup300_trk40_hmt'],False]],
        ['j85_mb_sp700_pusup350_trk50_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp700_pusup350_trk50_hmt'],False]],
        ['j85_mb_sp900_pusup400_trk60_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp900_pusup400_trk60_hmt'],False]],
        ['j85_mb_sp1100_pusup450_trk70_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['j85_mb_sp1200_pusup500_trk80_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['j85_mb_sp1400_pusup550_trk90_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['j85_mb_sp1600_pusup600_trk100_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        
        ['j85_mb_sp600_trk40_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp600_trk40_hmt'],False]],
        ['j85_mb_sp700_trk50_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp700_trk50_hmt'],False]],
        ['j85_mb_sp900_trk60_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp900_trk60_hmt'],False]],
        ['j85_mb_sp1100_trk70_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1100_trk70_hmt'],False]],
        ['j85_mb_sp1200_trk80_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1200_trk80_hmt'],False]],
        ['j85_mb_sp1400_trk90_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1400_trk90_hmt'],False]],
        ['j85_mb_sp1600_trk100_hmt', 'L1_J20',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j85', 'mb_sp1600_trk100_hmt'],False]],
        
        ['j110_mb_sp600_pusup300_trk40_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp600_pusup300_trk40_hmt'],False]],
        ['j110_mb_sp700_pusup350_trk50_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp700_pusup350_trk50_hmt'],False]],
        ['j110_mb_sp900_pusup400_trk60_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp900_pusup400_trk60_hmt'],False]],
        ['j110_mb_sp1100_pusup450_trk70_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1100_pusup450_trk70_hmt'],False]],
        ['j110_mb_sp1200_pusup500_trk80_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1200_pusup500_trk80_hmt'],False]],
        ['j110_mb_sp1400_pusup550_trk90_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1400_pusup550_trk90_hmt'],False]],
        ['j110_mb_sp1600_pusup600_trk100_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1600_pusup600_trk100_hmt'],False]],
        
        ['j110_mb_sp600_trk40_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp600_trk40_hmt'],False]],
        ['j110_mb_sp700_trk50_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp700_trk50_hmt'],False]],
        ['j110_mb_sp900_trk60_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp900_trk60_hmt'],False]],
        ['j110_mb_sp1100_trk70_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1100_trk70_hmt'],False]],
        ['j110_mb_sp1200_trk80_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1200_trk80_hmt'],False]],
        ['j110_mb_sp1400_trk90_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1400_trk90_hmt'],False]],
        ['j110_mb_sp1600_trk100_hmt', 'L1_J30',  ['', ''], ['MinBias'],["BW:MinBias", "RATE:MinBias"], 1, ['serial', -1, ['j110', 'mb_sp1600_trk100_hmt'],False]],
			 
			 ]


    if TriggerFlags.doFTK():
            TriggerFlags.CombinedSlice.signatures = TriggerFlags.CombinedSlice.signatures() + [

        # muon +tau FTK chains for T&P performance measurement
        ['mu26_ivarmedium_tau12_idperf_FTK',             'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_idperf_FTK"]]],
        ['mu26_ivarmedium_tau12_perf0_FTK',              'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_perf0_FTK"]]],
        ['mu26_ivarmedium_tau12_perf_FTK',               'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_perf_FTK"]]],
        ['mu26_ivarmedium_tau12_medium0_FTK',            'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_medium0_FTK"]]],
        ['mu26_ivarmedium_tau12_medium1_FTK',            'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_medium1_FTK"]]],
        ['mu26_ivarmedium_tau12_perf0_FTKNoPrec',        'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_perf0_FTKNoPrec"]]],
        ['mu26_ivarmedium_tau12_perf_FTKNoPrec',         'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_perf_FTKNoPrec"]]],
        ['mu26_ivarmedium_tau12_medium0_FTKNoPrec',      'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_medium0_FTKNoPrec"]]],
        ['mu26_ivarmedium_tau12_medium1_FTKNoPrec',      'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau12_medium1_FTKNoPrec"]]],
        ['mu26_ivarmedium_tau25_idperf_tracktwo_L1TAU12IM_FTK',    'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau25_idperf_tracktwo_L1TAU12IM_FTK"]]],
        ['mu26_ivarmedium_tau25_perf_tracktwo_L1TAU12IM_FTK',          'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau25_perf_tracktwo_L1TAU12IM_FTK"]]],
        ['mu26_ivarmedium_tau25_medium1_tracktwo_L1TAU12IM_FTK',       'L1_MU20_TAU12IM_FTK',['L1_MU20_FTK','L1_TAU12IM_FTK'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu26_ivarmedium","tau25_medium1_tracktwo_L1TAU12IM_FTK"]]],

             ]


    TriggerFlags.HeavyIonSlice.signatures = [
        ]


 #Beamspot chanis first try ATR-9847                                                                                                               
    TriggerFlags.BeamspotSlice.signatures = [

        ##trkFS --> no selection, write out PEB events
        ['beamspot_allTE_trkfast_peb_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_allTE_trkfast_peb_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_allTE_trkfast_peb',           'L1_4J20', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ##trkFS --> no selection, add TRT to PEB events        

        ['beamspot_allTE_trkfast_pebTRT_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_allTE_trkfast_pebTRT_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

# taking this out all together, not needed and even with PS=-1 makes histograms

        ['beamspot_allTE_trkfast_pebTRT',           'L1_4J20', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ##trkFS --> select good vertices, and write out PEB events

        ['beamspot_activeTE_trkfast_peb_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_activeTE_trkfast_peb_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],



        ['beamspot_activeTE_trkfast_peb',           'L1_4J20', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ##trkFS --> select good vertices, add TRT data to PEB events

        ['beamspot_activeTE_trkfast_pebTRT_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_activeTE_trkfast_pebTRT_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

# taking this out all together, not needed and even with PS=-1 makes histograms

        ['beamspot_activeTE_trkfast_pebTRT',           'L1_4J20', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ##trkFS --> online beam-spot determination

        ['beamspot_trkFS_trkfast_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ['beamspot_trkFS_trkfast_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],
        
# taking this out all together, not needed and even with PS=-1 makes histograms
        ['beamspot_trkFS_trkfast', 'L1_4J20', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],

        ['beamspot_activeTE_trkfast_peb_L13J15', 'L1_3J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_allTE_trkfast_peb_L13J15', 'L1_3J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_trkFS_trkfast_L13J15', 'L1_3J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],

        ['beamspot_activeTE_trkfast_peb_L1J15', 'L1_J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_allTE_trkfast_peb_L1J15', 'L1_J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_trkFS_trkfast_L1J15', 'L1_J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        
#        ['beamspot_activeTE_trkfast_peb_L1MBTS_1_VTE70', 'L1_MBTS_1_VTE70', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
#        ['beamspot_allTE_trkfast_peb_L1MBTS_1_VTE70', 'L1_MBTS_1_VTE70', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
#        ['beamspot_trkFS_trkfast_L1MBTS_1_VTE70', 'L1_MBTS_1_VTE70', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],

        ]   


    if TriggerFlags.doFTK():
        TriggerFlags.BeamspotSlice.signatures = TriggerFlags.BeamspotSlice.signatures() + [                                                                 
            ['beamspot_idperf_FTK_L14J20_FTK',      'L1_4J20_FTK', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],
            ['beamspot_trkFS_FTK_L14J20_FTK',       'L1_4J20_FTK', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],
            ['beamspot_trkFS_FTKRefit_L14J20_FTK',  'L1_4J20_FTK', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
            ['beamspot_idperf_FTKRefit_L14J20_FTK', 'L1_4J20_FTK', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
            ]


    TriggerFlags.MinBiasSlice.signatures = [
        # LS1 chains
        ['mb_sptrk',                     'L1_RD0_FILLED', [], ['MinBias', 'express'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_L1RD0_UNPAIRED_ISO',  'L1_RD0_UNPAIRED_ISO', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sp_L1RD0_UNPAIRED_ISO',     'L1_RD0_UNPAIRED_ISO', [], ['Background'], ["RATE:Calibration", "BW:Other"], -1],
        ['mb_sp_ncb_L1RD0_UNPAIRED_ISO', 'L1_RD0_UNPAIRED_ISO', [], ['Background'], ["RATE:Calibration", "BW:Other"], -1], #ATR-13982
        ['mb_sp_blayer_L1RD0_UNPAIRED_ISO', 'L1_RD0_UNPAIRED_ISO', [], ['Background'], ["RATE:Calibration", "BW:Other"], -1], #ATR-14301
        ['mb_sptrk_L1RD0_EMPTY',         'L1_RD0_EMPTY', [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_noisesup',            'L1_RD0_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_noisesup_L1RD0_UNPAIRED_ISO',    'L1_RD0_UNPAIRED_ISO', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_noisesup_L1RD0_EMPTY',   'L1_RD0_EMPTY', [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        
        #For Xe run
##         ['mb_sptrk_L1MBTS_1',   'L1_MBTS_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_1_VTE2',   'L1_MBTS_1_VTE2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_1_VTE3',   'L1_MBTS_1_VTE3', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_1_VTE4',   'L1_MBTS_1_VTE4', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_1_VTE5',   'L1_MBTS_1_VTE5', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_1_VTE10',   'L1_MBTS_1_VTE10', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        
##         ['mb_sptrk_L1MBTS_2_VTE2',   'L1_MBTS_2_VTE2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_2_VTE3',   'L1_MBTS_2_VTE3', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_2_VTE4',   'L1_MBTS_2_VTE4', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_2_VTE5',   'L1_MBTS_2_VTE5', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1MBTS_2_VTE10',   'L1_MBTS_2_VTE10', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        
##         ['mb_sptrk_L1VTE2',   'L1_VTE2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1VTE3',   'L1_VTE3', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1VTE4',   'L1_VTE4', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1VTE5',   'L1_VTE5', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
##         ['mb_sptrk_L1VTE10',   'L1_VTE10', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        
        #overlay
#        ['mb_sp_L1MBTS_1_OVERLAY',  'L1_MBTS_1_OVERLAY', [], ['MinBiasOverlay'],["BW:MinBiasOverlay", "RATE:MinBiasOverlay"], 1],
#        ['mb_sptrk_L1MBTS_1_OVERLAY',  'L1_MBTS_1_OVERLAY', [], ['MinBiasOverlay'],["BW:MinBiasOverlay", "RATE:MinBiasOverlay"], 1],


	#leading track for low-mu run
	['mb_sptrk_pt4_L1MBTS_1',                     'L1_MBTS_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_pt6_L1MBTS_1',                     'L1_MBTS_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_pt8_L1MBTS_1',                     'L1_MBTS_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
	['mb_sptrk_pt4_L1MBTS_2',                     'L1_MBTS_2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_pt6_L1MBTS_2',                     'L1_MBTS_2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
	['mb_sptrk_pt8_L1MBTS_2',                     'L1_MBTS_2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
	['mb_sptrk_pt4_L1MBTS_1_1',                   'L1_MBTS_1_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_pt6_L1MBTS_1_1',                   'L1_MBTS_1_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_pt8_L1MBTS_1_1',                   'L1_MBTS_1_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],

        ['mb_idperf_L1MBTS_2',                         'L1_MBTS_2', [], ['MinBias', 'express'], ["RATE:MinBias", "BW:MinBias"], -1],

        # Week 1
        ['mb_sptrk_L1RD3_FILLED',                     'L1_RD3_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_noisesup_L1RD3_FILLED',            'L1_RD3_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
	['mb_sptrk_pt4_L1RD3_FILLED',                 'L1_RD3_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_pt6_L1RD3_FILLED',                 'L1_RD3_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_pt8_L1RD3_FILLED',                 'L1_RD3_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],

        ['mb_perf_L1LUCID_UNPAIRED_ISO', 'L1_LUCID_UNPAIRED_ISO',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_perf_L1LUCID_EMPTY',        'L1_LUCID_EMPTY',        [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['mb_perf_L1LUCID',              'L1_LUCID',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],

        ['mb_sptrk_costr',               'L1_RD0_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_sptrk_costr_L1RD0_EMPTY',   'L1_RD0_EMPTY', [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],

        ['mb_perf_L1MBTS_2',             'L1_MBTS_2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_perf_L1RD1_FILLED',         'L1_RD1_FILLED', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
 
        ['mb_mbts_L1MBTS_1',             'L1_MBTS_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_mbts_L1MBTS_2',             'L1_MBTS_2', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1], 
        ['mb_mbts_L1MBTS_1_1',           'L1_MBTS_1_1', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_mbts_L1MBTS_1_EMPTY',       'L1_MBTS_1_EMPTY', [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['mb_mbts_L1MBTS_2_EMPTY',       'L1_MBTS_2_EMPTY', [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['mb_mbts_L1MBTS_1_1_EMPTY',     'L1_MBTS_1_1_EMPTY', [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['mb_mbts_L1MBTS_1_UNPAIRED_ISO',             'L1_MBTS_1_UNPAIRED_ISO', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_mbts_L1MBTS_2_UNPAIRED_ISO',             'L1_MBTS_2_UNPAIRED_ISO', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['mb_mbts_L1MBTS_1_1_UNPAIRED_ISO',           'L1_MBTS_1_1_UNPAIRED_ISO', [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],

        #physics                                                                                                                             
        ['mb_sp400_trk40_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp600_trk45_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp700_trk55_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk60_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk65_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_trk70_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk75_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk80_hmt_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        

        #HMT with pileup supprestion seeded by L1_TE triggers
        ['mb_sp600_pusup300_trk40_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp700_pusup350_trk50_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_pusup400_trk50_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_pusup400_trk60_hmt_L1TE5', 'L1_TE5', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_pusup450_trk70_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_pusup450_trk70_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk90_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk100_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
 
        ['mb_sp600_pusup300_trk40_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp700_pusup350_trk50_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_pusup400_trk60_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_pusup450_trk70_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_pusup450_trk70_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE10', 'L1_TE10', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_pusup650_trk110_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_pusup700_trk120_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp700_pusup350_trk50_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_pusup400_trk60_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_pusup450_trk70_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_pusup650_trk110_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_pusup700_trk120_hmt_L1TE20', 'L1_TE20', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup750_trk130_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_pusup800_trk140_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_pusup850_trk150_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1100_pusup450_trk70_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_pusup650_trk110_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_pusup700_trk120_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup750_trk130_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_pusup800_trk140_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_pusup850_trk150_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
 
        ['mb_sp1400_pusup550_trk90_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_pusup650_trk110_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_pusup700_trk120_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup750_trk130_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_pusup800_trk140_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_pusup850_trk150_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        ['mb_sp1600_pusup600_trk100_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_pusup650_trk110_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_pusup700_trk120_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup750_trk130_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_pusup800_trk140_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_pusup850_trk150_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #['mb_sp1700_pusup650_trk110_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1900_pusup700_trk120_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup750_trk130_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2200_pusup800_trk140_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2400_pusup850_trk150_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #['mb_sp1900_pusup700_trk120_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup750_trk130_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2200_pusup800_trk140_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2400_pusup850_trk150_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        
        #HMT without pileup supprestion seeded by L1_TE triggers
        ['mb_sp600_trk40_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp700_trk50_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk50_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk60_hmt_L1TE5', 'L1_TE5', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_trk70_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_trk70_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk90_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk100_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2300_trk130_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
 
        ['mb_sp600_trk40_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp700_trk50_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk60_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_trk70_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_trk70_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE10', 'L1_TE10', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_trk110_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_trk120_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp700_trk50_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk60_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_trk70_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_trk110_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_trk120_hmt_L1TE20', 'L1_TE20', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk130_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_trk140_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_trk150_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2500_trk140_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1100_trk70_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_trk110_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_trk120_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk130_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_trk140_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_trk150_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
 
        ['mb_sp1400_trk90_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_trk110_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_trk120_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk130_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_trk140_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_trk150_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        ['mb_sp1600_trk100_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_trk110_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_trk120_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk130_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2200_trk140_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2400_trk150_hmt_L1TE50', 'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #['mb_sp1700_trk110_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1900_trk120_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk130_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2200_trk140_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2400_trk150_hmt_L1TE60', 'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #['mb_sp1900_trk120_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk130_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2200_trk140_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2400_trk150_hmt_L1TE70', 'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #HMT with pileup supprestion seeded by L1_TE.0ETA24 triggers
        #['mb_sp900_pusup400_trk50_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp900_pusup400_trk60_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1000_pusup450_trk70_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_pusup500_trk80_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_pusup500_trk90_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_pusup500_trk100_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_pusup700_trk110_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup900_trk120_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        #['mb_sp2300_pusup1000_trk130_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1000_pusup450_trk70_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_pusup500_trk80_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1400_pusup550_trk90_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_pusup600_trk100_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_pusup700_trk110_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup900_trk120_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_pusup1000_trk130_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_pusup1100_trk140_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1200_pusup500_trk80_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1400_pusup550_trk90_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_pusup600_trk100_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_pusup700_trk110_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup900_trk120_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_pusup1000_trk130_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_pusup1100_trk140_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1400_pusup550_trk90_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_pusup600_trk100_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_pusup700_trk110_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup900_trk120_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_pusup1000_trk130_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_pusup1100_trk140_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_pusup1200_trk150_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1600_pusup600_trk100_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_pusup700_trk110_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup900_trk120_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_pusup1000_trk130_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_pusup1100_trk140_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_pusup1200_trk150_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2900_pusup1300_trk160_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1800_pusup700_trk110_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_pusup900_trk120_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_pusup1000_trk130_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_pusup1100_trk140_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_pusup1200_trk150_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2900_pusup1300_trk160_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp2300_pusup1000_trk130_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_pusup1100_trk140_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_pusup1200_trk150_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2900_pusup1300_trk160_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        ## HMTfor 5TeV run ATR-17162
        ['mb_sp700_trk50_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk60_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_trk70_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE15', 'L1_TE15', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_trk110_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_trk120_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_trk70_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_trk110_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_trk120_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp700_pusup350_trk50_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_pusup400_trk60_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_pusup450_trk70_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE15', 'L1_TE15', [], ['MinBias','express'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_pusup650_trk110_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_pusup700_trk120_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1100_pusup450_trk70_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1700_pusup650_trk110_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1900_pusup700_trk120_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #HMT without pileup supprestion seeded by L1_TE.0ETA24 triggers
        #['mb_sp900_trk50_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp900_trk60_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1000_trk70_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_trk80_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_trk90_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_trk100_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_trk110_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk120_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        #['mb_sp2300_trk130_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1000_trk70_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_trk80_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1400_trk90_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_trk100_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_trk110_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk120_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_trk130_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_trk140_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1200_trk80_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1400_trk90_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_trk100_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_trk110_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk120_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_trk130_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_trk140_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1400_trk90_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_trk100_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_trk110_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk120_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_trk130_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_trk140_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_trk150_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1600_trk100_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1800_trk110_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk120_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_trk130_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_trk140_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_trk150_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2900_trk160_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp1800_trk110_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_trk120_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2300_trk130_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_trk140_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_trk150_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2900_trk160_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp2300_trk130_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2500_trk140_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2700_trk150_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2900_trk160_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        
        #HMT performance
        ['mb_sp800_hmtperf_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp800_hmtperf_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_hmtperf_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_hmtperf_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #['mb_sp800_hmtperf_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp800_hmtperf_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_hmtperf_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1200_hmtperf_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_hmtperf_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp1600_hmtperf_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_hmtperf_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_hmtperf_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_hmtperf_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp2100_hmtperf_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp2_hmtperf_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2_hmtperf', 'L1_RD3_FILLED', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp500_hmtperf', 'L1_RD3_FILLED', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_hmtperf', 'L1_RD3_FILLED', [], ['MinBias', 'express'], ["BW:MinBias", "RATE:MinBias"], -1],
	
        # ALFA
        ['mb_sptrk_vetombts2in_L1ZDC_AND',                  'L1_ZDC_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_vetombts2in_L1ZDC_A_C',                  'L1_ZDC_A_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #AFP (ATR-14833) (removed ATR-15881)
        ['mb_sptrk_trk80_L1MBTS_2',                   'L1_MBTS_2',                     [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #AFP ATR-16124
        ['mb_sptrk_pt2_L1AFP_A_OR_C',                     'L1_AFP_A_OR_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt4_L1AFP_A_OR_C',                     'L1_AFP_A_OR_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt6_L1AFP_A_OR_C',                     'L1_AFP_A_OR_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt8_L1AFP_A_OR_C',                     'L1_AFP_A_OR_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt2_L1AFP_A_AND_C',                     'L1_AFP_A_AND_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt4_L1AFP_A_AND_C',                     'L1_AFP_A_AND_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt6_L1AFP_A_AND_C',                     'L1_AFP_A_AND_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt8_L1AFP_A_AND_C',                     'L1_AFP_A_AND_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sptrk_pt2_L1MBTS_2',                     'L1_MBTS_2',                     [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp_vetospmbts2in_L1AFP_A_OR_C',                     'L1_AFP_A_OR_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_vetospmbts2in_L1AFP_A_AND_C',                     'L1_AFP_A_AND_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_vetospmbts2in_L1AFP_A',                     'L1_AFP_A',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_vetospmbts2in_L1AFP_C',                     'L1_AFP_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_vetosp_L1AFP_A_AND_C',                     'L1_AFP_A_AND_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp_L1RD0_FILLED',                     'L1_RD0_FILLED',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        ]
        
    TriggerFlags.CalibSlice.signatures   = [
        
        ['lhcfpeb',       'L1_LHCF',       [], ['IDFwd'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lhcfpeb_L1LHCF_UNPAIRED_ISO',       'L1_LHCF_UNPAIRED_ISO',       [], ['IDFwd'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lhcfpeb_L1LHCF_EMPTY',       'L1_LHCF_EMPTY',       [], ['IDFwd'], ["RATE:Calibration", "BW:Detector"], -1],

        ['lumipeb_L1RD0_EMPTY',          'L1_RD0_EMPTY',        [], ['PixelNoise'], ["RATE:Calibration", "RATE:PixelCalibration", "BW:Detector"], -1],

        ['lumipeb_L1RD0_FILLED',            'L1_RD0_FILLED',            [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],
        ['lumipeb_L1RD0_UNPAIRED_ISO',      'L1_RD0_UNPAIRED_ISO',      [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],
        ['lumipeb_L1MBTS_1',                'L1_MBTS_1',                [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1], 
        ['lumipeb_L1MBTS_1_UNPAIRED_ISO',   'L1_MBTS_1_UNPAIRED_ISO',   [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],
        ['lumipeb_L1MBTS_2',                'L1_MBTS_2',                [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1], 
        ['lumipeb_L1MBTS_2_UNPAIRED_ISO',   'L1_MBTS_2_UNPAIRED_ISO',   [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],
        ['lumipeb_L1MBTS_2_LUCID',          'L1_MBTS_2,L1_LUCID',       [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1], 

        # triggers for VdM scan
        ['lumipeb_vdm_L1MBTS_1_BGRP11',       'L1_MBTS_1_BGRP11',       [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_2_BGRP11',       'L1_MBTS_2_BGRP11',       [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1RD0_BGRP11',          'L1_RD0_BGRP11',          [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_1_BGRP9',        'L1_MBTS_1_BGRP9',        [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_2_BGRP9',        'L1_MBTS_2_BGRP9',        [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1RD0_BGRP9',           'L1_RD0_BGRP9',           [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_1_UNPAIRED_ISO', 'L1_MBTS_1_UNPAIRED_ISO', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_2_UNPAIRED_ISO', 'L1_MBTS_2_UNPAIRED_ISO', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1RD0_UNPAIRED_ISO',    'L1_RD0_UNPAIRED_ISO',    [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1RD2_BGRP12',          'L1_RD2_BGRP12',          [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1RD0_BGRP10',          'L1_RD0_BGRP10',          [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],

        ['lumipeb_vdm_L1MBTS_1', 'L1_MBTS_1', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_2', 'L1_MBTS_2', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1RD0_FILLED',         'L1_RD0_FILLED', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
 

        ['lumipeb_vdm_L1MBTS_2_LUCID',                 'L1_MBTS_2,L1_LUCID',    [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1], 
        ['lumipeb_vdm_L1MBTS_2_LUCID_UNPAIRED_ISO',    'L1_MBTS_2_UNPAIRED_ISO,L1_LUCID_UNPAIRED_ISO',    [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],

        ['lumipeb_vdm_L1MBTS_2_LUCID_BGRP9',  'L1_MBTS_2_BGRP9,L1_LUCID_BGRP9',     [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_2_LUCID_BGRP11', 'L1_MBTS_2_BGRP11,L1_LUCID_BGRP11',   [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],

        ['lumipeb_L1RD0_ABORTGAPNOTCALIB',    'L1_RD0_ABORTGAPNOTCALIB',    [], ['PixelBeam'], ["RATE:Calibration", "BW:Detector"], -1],

        ['larpebcalib_L1RD0_BGRP11',          'L1_RD0_BGRP11',         [], ['LArPEB'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larpebcalib_L1RD0_EMPTY',           'L1_RD0_EMPTY',          [], ['LArPEB'], ["RATE:Calibration", "BW:Detector"], -1],

        ['larnoiseburst_loose_L1All',             'L1_All',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1All',             'L1_All',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1XE60',            'L1_XE60',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1XE70',            'L1_XE70',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1XE55',            'L1_XE55',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1J75',             'L1_J75',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1J40_XE60',            'L1_J40_XE60',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        

        ['larnoiseburst_L1XE80', 'L1_XE80', [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1J100', 'L1_J100', [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],

        #Special re-run chains
        ['larnoiseburst_rerun',                  '',             [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_loose_rerun',            '',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        

        ['l1calocalib',             'L1_RD0_FILLED',     [], ['L1CaloCalib'], ["RATE:Calibration", "RATE:L1CaloCalib", "BW:Detector"], -1],
        ['l1calocalib_L1BGRP9',     'L1_BGRP9',          [], ['L1CaloCalib'], ["RATE:Calibration", "RATE:L1CaloCalib", "BW:Detector"], -1],
        
        ['larcalib_L1EM3_EMPTY',    'L1_EM3_EMPTY',      [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larcalib_L1EM7_EMPTY',    'L1_EM7_EMPTY',      [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larcalib_L1TAU8_EMPTY',   'L1_TAU8_EMPTY',     [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larcalib_L1J12_EMPTY',    'L1_J12_EMPTY',      [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larcalib_L1J3031ETA49_EMPTY',  'L1_J30.31ETA49_EMPTY',     [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],


        #ALFA Calib
        ['alfacalib',      'L1_ALFA_Calib'    , [],  ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        
        ['alfacalib_L1ALFA_ELAS',   'L1_ALFA_ELAS'      , [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        
        ['alfacalib_L1ALFA_SYS',   'L1_ALFA_SYS'     , [], ['MinBias'], ["RATE:MinBias", "RATE:ALFA", "BW:Other"], -1], 
        ['alfacalib_L1ALFA_SYS_Calib',   'L1_ALFA_SYS_Calib'      , [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        
        ['noalg_L1ALFA_Phys',    'L1_ALFA_Phys',    [], ['MinBias'], ["RATE:MinBias", "RATE:ALFA", "BW:Other"], -1], 
        ['noalg_L1ALFA_PhysAny', 'L1_ALFA_PhysAny', [], ['MinBias'], ["RATE:MinBias", "RATE:ALFA", "BW:Other"], -1], 
        ['noalg_L1ALFA_ANY',     'L1_ALFA_ANY'     , [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        
        #
        ['ibllumi_L1RD0_ABORTGAPNOTCALIB',    'L1_RD0_ABORTGAPNOTCALIB', [], ['IBLLumi'], [ 'RATE:IBLLumi','BW:Detector'], -1],
        ['ibllumi_L1RD0_FILLED',    'L1_RD0_FILLED', [], ['IBLLumi'], [ 'RATE:IBLLumi','BW:Detector'], -1],
        ['ibllumi_L1RD0_UNPAIRED_ISO',    'L1_RD0_UNPAIRED_ISO', [], ['IBLLumi'], [ 'RATE:IBLLumi','BW:Detector'], -1],

        #
        ['idcalib_trk9_central',    'L1_TAU', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],
        ['idcalib_trk9_fwd',        'L1_TAU', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],
        ['idcalib_trk16_central',   'L1_TAU', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],
        ['idcalib_trk16_fwd',       'L1_TAU', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],
        ['idcalib_trk29_central',   'L1_TAU', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],
        ['idcalib_trk29_fwd',       'L1_TAU', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],
        
#        ['idcalib_trk9_central_L1MBTS_1_VTE70',    'L1_MBTS_1_VTE70', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],
#        ['idcalib_trk9_fwd_L1MBTS_1_VTE70',        'L1_MBTS_1_VTE70', [], ['IDTracks'], [ 'RATE:IDTracks','BW:Detector'], -1],

        ['conej40_larpebj',  'L1_J20',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['conej165_larpebj', 'L1_J100',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1], 
        ['conej75_320eta490_larpebj',  'L1_J30.31ETA49',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['conej140_320eta490_larpebj', 'L1_J75.31ETA49',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1],

        # Monitoritems for saturated towers
        ['satu20em_l1satmon_L1J100_FIRSTEMPTY',  'L1_J100_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1J100.31ETA49_FIRSTEMPTY',  'L1_J100.31ETA49_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1EM20VH_FIRSTEMPTY',  'L1_EM20VH_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1EM22VHI_FIRSTEMPTY',  'L1_EM22VHI_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1J100',  'L1_J100',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1J100.31ETA49',  'L1_J100.31ETA49',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],

        #AFP calib ATR-16124
        ['calibAFP_L1AFP_A_BGRP0',                   'L1_AFP_A_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_A_SPECTOF_BGRP0',                   'L1_AFP_A_SPECTOF_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_BGRP0',                   'L1_AFP_C_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_SPECTOF_BGRP0',                   'L1_AFP_C_SPECTOF_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_NSA_BGRP0',                   'L1_AFP_NSA_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_NSC_BGRP0',                   'L1_AFP_NSC_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_FSA_SIT_BGRP0',                   'L1_AFP_FSA_SIT_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_FSC_SIT_BGRP0',                   'L1_AFP_FSC_SIT_BGRP0'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],

        ['calibAFP_L1AFP_A_OR_C_UNPAIRED_ISO',                   'L1_AFP_A_OR_C_UNPAIRED_ISO'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_A_OR_C_UNPAIRED_NONISO',                   'L1_AFP_A_OR_C_UNPAIRED_NONISO'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],

        ['calibAFP_L1AFP_A_OR_C_EMPTY',                   'L1_AFP_A_OR_C_EMPTY'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_A_OR_C_FIRSTEMPTY',                   'L1_AFP_A_OR_C_FIRSTEMPTY'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],

        ['calibAFP_L1AFP_A_AND_C',                   'L1_AFP_A_AND_C'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_A_OR_C',                   'L1_AFP_A_OR_C'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_A_AND_C_SPECTOF',                   'L1_AFP_A_AND_C_SPECTOF'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],

        ['calibAFP_L1AFP_A',                   'L1_AFP_A'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C',                   'L1_AFP_C'    ,              [],  ['AFP'], ['RATE:CalibrationAFP','BW:Detector'], -1],

        ['rpcpeb_L1RD0_EMPTY', 'L1_RD0_EMPTY', [], ['RPCNoise'], ["BW:Detector", "RATE:Calibration", "RATE:RPCCalibration",], -1],

        ['idpsl1_L1IDprescaled','', [], ['IDprescaledL1'], ["RATE:SeededStreamers","BW:Other"], -1],

        ]

    TriggerFlags.CosmicSlice.signatures  = [ 
        ['tilecalib_laser',   'L1_CALREQ2', [], ['Tile'], ["RATE:Calibration", "RATE:Cosmic_TileCalibration", "BW:Detector"], -1],
        ['sct_noise',          'L1_RD0_EMPTY', [], ['SCTNoise'], ["RATE:Calibration", "RATE:SCTCalibration", "BW:Detector"], -1],
        
        # CosmicCalo
        ['larps_L1EM3_EMPTY',  'L1_EM3_EMPTY', [], ['CosmicCalo'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larps_L1EM7_EMPTY',  'L1_EM7_EMPTY', [], ['CosmicCalo'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larps_L1EM7_FIRSTEMPTY',  'L1_EM7_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larps_L1J12_EMPTY',  'L1_J12_EMPTY', [], ['CosmicCalo'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larps_L1J30_EMPTY',  'L1_J30_EMPTY', [], ['CosmicCalo'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larps_L1TAU8_EMPTY', 'L1_TAU8_EMPTY', [], ['CosmicCalo'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
	
        ['larps_L1J3031ETA49_EMPTY', 'L1_J30.31ETA49_EMPTY', [], ['CosmicCalo'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        #JetTauEtmiss 
        ['larps_L1J12_FIRSTEMPTY', 'L1_J12_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larps_L1J30_FIRSTEMPTY', 'L1_J30_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],

        ['larhec_L1J12_FIRSTEMPTY', 'L1_J12_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larhec_L1EM7_FIRSTEMPTY', 'L1_EM7_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larhec_L1TAU8_FIRSTEMPTY', 'L1_TAU8_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larhec_L1J30_FIRSTEMPTY', 'L1_J30_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        #CosmicID
        ['id_cosmicid_L1MU4_EMPTY',         'L1_MU4_EMPTY', [],  ['HLT_IDCosmic',  'express'], ['RATE:CosmicSlice', 'RATE:Cosmic_Tracking', 'BW:Detector'], -1],
        ['id_cosmicid_L1MU11_EMPTY',        'L1_MU11_EMPTY', [], ['HLT_IDCosmic',  'express'], ['RATE:CosmicSlice', 'RATE:Cosmic_Tracking', 'BW:Detector'], -1],

        ]
    TriggerFlags.StreamingSlice.signatures = [

        ['noalg_L1RD0_EMPTY', 'L1_RD0_EMPTY', [],   [PhysicsStream, 'express'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1RD0_FILLED', 'L1_RD0_FILLED', [], [PhysicsStream, 'express'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1RD1_FILLED', 'L1_RD1_FILLED', [], [PhysicsStream], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1RD2_EMPTY', 'L1_RD2_EMPTY', [],   [PhysicsStream], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1RD2_FILLED', 'L1_RD2_FILLED', [], [PhysicsStream], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1RD3_EMPTY', 'L1_RD3_EMPTY', [],   [PhysicsStream], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1RD3_FILLED', 'L1_RD3_FILLED', [], [PhysicsStream], ["RATE:MinBias", "BW:MinBias"], -1],

        ['noalg_standby_L1RD0_FILLED', 'L1_RD0_FILLED', [], ['Standby'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_standby_L1RD0_EMPTY', 'L1_RD0_EMPTY', [], ['Standby'], ["RATE:CoMinBias", "BW:MinBias"], -1],
	
        ['noalg_standby_L1J15', 'L1_J15', [], ['Standby','express'], ['RATE:MinBias', 'BW:MinBias'], -1],
	['noalg_standby_L13J15', 'L1_3J15', [], ['Standby','express'], ['RATE:MinBias', 'BW:MinBias'], -1],
	['noalg_standby_L14J15', 'L1_4J15', [], ['Standby','express'], ['RATE:MinBias', 'BW:MinBias'], -1],

        # bphys streamers (ATR-16140)
        ['noalg_L13MU4',           'L1_3MU4',           [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['noalg_L1MU6_2MU4',       'L1_MU6_2MU4',       [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1],
        ['noalg_L1BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4',  'L1_BPH-8M15-MU6MU4_BPH-0DR22-MU6MU4',           [], [BPhysicsStream], [RATE_BphysTag,BW_BphysTag], -1, False],

# Egamma streamers
        ['noalg_L1EM15',           'L1_EM15',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM12',           'L1_EM12',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L12EM7',           'L1_2EM7',           [], [PhysicsStream,'express'], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L12EM15',          'L1_2EM15',          [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM15VH_3EM7',    'L1_EM15VH_3EM7',    [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM7',            'L1_EM7',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM3',            'L1_EM3',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_mb_L1EM3',         'L1_EM3',            [], ['MinBias'], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L12EM3',           'L1_2EM3',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],

        ['noalg_L1EM3_EMPTY',            'L1_EM3_EMPTY',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],

        # Muon streamers
        ['noalg_L1MU20',           'L1_MU20',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU21',           'L1_MU21',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU20MU21',   'L1_MU20MU21',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU10',           'L1_MU10',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU11',           'L1_MU11',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L12MU6',           'L1_2MU6',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L12MU10',          'L1_2MU10',          [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L12MU4',           'L1_2MU4',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],

        ['noalg_L1MU4_EMPTY',      'L1_MU4_EMPTY',      [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU4_FIRSTEMPTY', 'L1_MU4_FIRSTEMPTY', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU11_EMPTY',      'L1_MU11_EMPTY',      [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU6_EMPTY',            'L1_MU6_EMPTY',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],

        ['noalg_L1MU20_FIRSTEMPTY',     'L1_MU20_FIRSTEMPTY', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU21_FIRSTEMPTY',     'L1_MU21_FIRSTEMPTY', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU20MU21_FIRSTEMPTY', 'L1_MU20MU21_FIRSTEMPTY', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        
##         ['noalg_mb_L1TE2',  'L1_TE2', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
##         ['noalg_mb_L1TE3',  'L1_TE3', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
##         ['noalg_mb_L1TE4',  'L1_TE4', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE5',  'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE10',  'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ['noalg_mb_L1TE20',  'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE30',  'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ['noalg_mb_L1TE40',  'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE50',  'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['noalg_mb_L1TE60',  'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['noalg_mb_L1TE70',  'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE15',  'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE25',  'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #data min-bias overlay
#	    ['noalg_L1TE20_OVERLAY',   'L1_TE20_OVERLAY', [], ['MinBiasOverlay'], ["BW:MinBiasOverlay", "RATE:MinBiasOverlay"], 1],
#	    ['noalg_L1MBTS_1_OVERLAY',   'L1_MBTS_1_OVERLAY', [], ['MinBiasOverlay'], ["BW:MinBiasOverlay", "RATE:MinBiasOverlay"], 1],

        #['noalg_mb_L1TE5.0ETA24',  'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['noalg_mb_L1TE10.0ETA24',  'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['noalg_mb_L1TE15.0ETA24',  'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        #['noalg_mb_L1TE20.0ETA24',  'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['noalg_mb_L1TE25.0ETA24',  'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        #['noalg_mb_L1TE30.0ETA24',  'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        #['noalg_mb_L1TE40.0ETA24',  'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        #['noalg_mb_L1TE70.0ETA24',  'L1_TE70.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        # ZDC streamers
        ['noalg_mb_L1ZDC_A',    'L1_ZDC_A', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1ZDC_C',    'L1_ZDC_C', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1ZDC_AND',  'L1_ZDC_AND', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1ZDC_A_C',  'L1_ZDC_A_C', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['zdcpeb_L1ZDC_A',      'L1_ZDC_A', [], ['zdcCalib'], ["BW:Detector", "RATE:Calibration"], -1],
        ['zdcpeb_L1ZDC_C',      'L1_ZDC_C', [], ['zdcCalib'], ["BW:Detector", "RATE:Calibration"], -1],
        ['zdcpeb_L1ZDC_AND',    'L1_ZDC_AND', [], ['zdcCalib'], ["BW:Detector", "RATE:Calibration"], -1],
        ['zdcpeb_L1ZDC_A_C',    'L1_ZDC_A_C', [], ['zdcCalib'], ["BW:Detector", "RATE:Calibration"], -1],

        # Tau streamers
        ['noalg_L1TAU30',            'L1_TAU30',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],
        ['noalg_L1TAU20',            'L1_TAU20',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],
        ['noalg_L1TAU20_2TAU12',     'L1_TAU20_2TAU12',     [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],
        ['noalg_L1TAU20IM_2TAU12IM', 'L1_TAU20IM_2TAU12IM', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],
        ['noalg_L1TAU12',            'L1_TAU12',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],
        ['noalg_L1TAU12IM',            'L1_TAU12IM',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],
        ['noalg_L1TAU20IM',            'L1_TAU20IM',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],

        ['noalg_L1TAU8_EMPTY',            'L1_TAU8_EMPTY',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Tau'], -1],

        # MET streamers
        ['noalg_L1XE35',             'L1_XE35',             [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:MET'], -1],
        ['noalg_L1XE10',             'L1_XE10',             [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:MET'], -1],
        ['noalg_L1XE30',             'L1_XE30',             [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:MET'], -1],
        ['noalg_L1XE300',            'L1_XE300',             [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:MET'], -1],

        # Jet streamers
        ['noalg_L1J20_J20.31ETA49',                'L1_J20_J20.31ETA49',                [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Jet'], -1],

        ['noalg_L1J12_EMPTY',            'L1_J12_EMPTY',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Jet'], -1],

        # VBF triggers
        ['noalg_L1MJJ-100', 'L1_MJJ-100', [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1],  

        # Chains for Week1 menu (ATR-11119)
        ['noalg_L1MU4',   'L1_MU4',  [], [PhysicsStream, 'express'], ["RATE:SeededStreamers", "BW:Muon"], -1 ], #AFP request
        ['noalg_mb_L1MU4', 'L1_MU4',  [], ['MinBias'], ["RATE:SeededStreamers", "BW:Muon"], -1 ], #AFP request
        ['noalg_L1MU6',   'L1_MU6',  [], [PhysicsStream,'express'], ["RATE:SeededStreamers", "BW:Muon"], -1 ],
        ['noalg_L1J50',   'L1_J50',  [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],

        #ATR-10976
        ['noalg_L1MU6_J20',    'L1_MU6_J20',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:BJet"], -1 ],

      
        ['noalg_L1CALREQ2',   'L1_CALREQ2', [], ['Tile'], ["RATE:Calibration", "RATE:Cosmic_TileCalibration", "BW:Detector"], -1],

        ['noalg_to_L12MU20_OVERLAY',   'L1_2MU20_OVERLAY', [], ['TauOverlay'], ["RATE:TauOverlay",  "BW:TauOverlay"], -1],


        # 
        ['noalg_L1TGC_BURST',   'L1_TGC_BURST', [], ['TgcNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        #background streamers
        ['noalg_bkg_L1Bkg',               'L1_Bkg',               [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1MU4_UNPAIRED_ISO',  'L1_MU4_UNPAIRED_ISO',  [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J12',               'L1_J12',               [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J30.31ETA49',       'L1_J30.31ETA49',       [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J12_BGRP12',        'L1_J12_BGRP12',         [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J30.31ETA49_BGRP12', 'L1_J30.31ETA49_BGRP12', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 

        ['noalg_bkg_L1MBTS_2_UNPAIRED_ISO', 'L1_MBTS_2_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1RD0_UNPAIRED_ISO', 'L1_RD0_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1MBTS_4_A_UNPAIRED_ISO', 'L1_MBTS_4_A_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1MBTS_4_C_UNPAIRED_ISO', 'L1_MBTS_4_C_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1J15.31ETA49_UNPAIRED_ISO', 'L1_J15.31ETA49_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],

        ['noalg_L1Standby',              'L1_Standby',          [], ['Standby'],    ["RATE:SeededStreamers", "BW:Other"], -1],         
        ['noalg_idcosmic_L1TRT_EMPTY',          'L1_TRT_EMPTY',              [], ['IDCosmic',  'express'],   ["RATE:SeededStreamers", "BW:Other"], -1],        
        ['noalg_idcosmic_L1TRT_FILLED',         'L1_TRT_FILLED',              [], ['IDCosmic'],   ["RATE:SeededStreamers", "BW:Other"], -1],         
        
        ['noalg_L1Calo',                  'L1_Calo',             [], ['L1Calo'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_L1Topo',                  'L1_Topo',             [], ['L1Topo'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_larcells_L1LAR-EM',                'L1_LAR-EM',           [], ['LArCells'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_larcells_L1LAR-J',                 'L1_LAR-J',            [], ['LArCells'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_L1Calo_EMPTY',            'L1_Calo_EMPTY',       [], ['L1Calo'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
      
        ['noalg_l1calo_L1J75',         'L1_J75', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1J100',        'L1_J100', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1J120',        'L1_J120', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1J5031ETA49',  'L1_J50.31ETA49', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1J7531ETA49',  'L1_J75.31ETA49', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1J10031ETA49', 'L1_J100.31ETA49', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1EM7',         'L1_EM7', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1EM12',        'L1_EM12', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1EM15',        'L1_EM15', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1TAU8',        'L1_TAU8', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1TAU12',       'L1_TAU12', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1TAU20',       'L1_TAU20', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],

        ['noalg_l1topo_L12EM3',   'L1_2EM3',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L12MU4',   'L1_2MU4',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L12MU6',   'L1_2MU6',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L14J15',   'L1_4J15',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L14J20.0ETA49',   'L1_4J20.0ETA49',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1EM12_2EM3',   'L1_EM12_2EM3',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1EM15I_MU4',   'L1_EM15I_MU4',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1EM3',   'L1_EM3',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1EM15',   'L1_EM15',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1EM7_2EM3',   'L1_EM7_2EM3',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1EM8I_MU10',   'L1_EM8I_MU10',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1EM7_MU10', 'L1_EM7_MU10', [], ['L1Topo'], ["RATE:SeededStreamers", "BW:Other"], -1],                                                                                                                              
        ['noalg_l1topo_L1EM7',   'L1_EM7',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1J100',   'L1_J100',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1J15',   'L1_J15',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1J20',   'L1_J20',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1J25',   'L1_J25',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1calo_L1J400', 'L1_J400', [], ['L1Calo'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1J30.0ETA49_2J20.0ETA49',   'L1_J30.0ETA49_2J20.0ETA49',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1J40_XE50',   'L1_J40_XE50',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1MU10_2MU6',   'L1_MU10_2MU6',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1MU10_TAU12IM',   'L1_MU10_TAU12IM',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1MU4',   'L1_MU4',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1MU6_2MU4',   'L1_MU6_2MU4',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1MU6_J20',   'L1_MU6_J20',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1TAU12IM',   'L1_TAU12IM',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1TAU20IM_2TAU12IM',   'L1_TAU20IM_2TAU12IM',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1TAU30',   'L1_TAU30',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1TAU40',   'L1_TAU40',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1XE35',   'L1_XE35',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1XE45',   'L1_XE45',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1XE55',   'L1_XE55',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1XE60',   'L1_XE60',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_l1topo_L1XE70',   'L1_XE70',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],


	#L1Topo streamer (ATR-14714)
	["noalg_l1topo_L1DR-MU10TAU12I_TAU12I-J25",          	"L1_DR-MU10TAU12I_TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU10_TAU12I-J25",          		"L1_MU10_TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-8M15-MU6MU4",          		"L1_BPH-8M15-MU6MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-8M15-2MU6",          		"L1_BPH-8M15-2MU6",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BTAG-MU4J15",          		"L1_BTAG-MU4J15",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BTAG-MU6J20",          		"L1_BTAG-MU6J20",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J15_BTAG-MU4J15",          		"L1_3J15_BTAG-MU4J15",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J15_BTAG-MU4J30",          		"L1_3J15_BTAG-MU4J30",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J15_BTAG-MU6J25",          		"L1_3J15_BTAG-MU6J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_BTAG-MU4J20",          		"L1_3J20_BTAG-MU4J20",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J40_DPHI-J20s2XE50",          		"L1_J40_DPHI-J20s2XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J40_DPHI-J20XE50",          		"L1_J40_DPHI-J20XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J40_DPHI-CJ20XE50",          		"L1_J40_DPHI-CJ20XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT190-J15.ETA21",          		"L1_HT190-J15.ETA21",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT190-J15s5.ETA21",          		"L1_HT190-J15s5.ETA21",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT150-J20.ETA31",          		"L1_HT150-J20.ETA31",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT150-J20s5.ETA31",          		"L1_HT150-J20s5.ETA31",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1JPSI-1M5",          			"L1_JPSI-1M5",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1JPSI-1M5-EM7",          		"L1_JPSI-1M5-EM7",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1JPSI-1M5-EM12",          		"L1_JPSI-1M5-EM12",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE55",          			"L1_KF-XE55",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE60",          			"L1_KF-XE60",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE65",          			"L1_KF-XE65",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE75",          			"L1_KF-XE75",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM12_W-MT25",          		"L1_EM12_W-MT25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          "L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-05RO-XEHT-0",          		"L1_W-05RO-XEHT-0",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-90RO2-XEHT-0",          		"L1_W-90RO2-XEHT-0",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-250RO2-XEHT-0",          		"L1_W-250RO2-XEHT-0",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-HT20-JJ15.ETA49",          		"L1_W-HT20-JJ15.ETA49",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-NOMATCH",          			"L1_W-NOMATCH",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35",          		"L1_EM15_W-MT35",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30",    	"L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          	"L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT150-JJ15.ETA49_MJJ-400",         	"L1_HT150-JJ15.ETA49_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-2M9-2MU6_BPH-2DR15-2MU6",          "L1_BPH-2M9-2MU6_BPH-2DR15-2MU6",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6MU4-BO",          			"L1_MU6MU4-BO",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU4-B",          			"L1_2MU4-B",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU6-B",          			"L1_2MU6-B",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU4-BO",          			"L1_2MU4-BO",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU6-BO",          			"L1_2MU6-BO",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6_2MU4-B",          			"L1_MU6_2MU4-B",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DY-DR-2MU4",          			"L1_DY-DR-2MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DY-BOX-2MU6",          		"L1_DY-BOX-2MU6",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LFV-MU",          			"L1_LFV-MU",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LFV-EM8I",          			"L1_LFV-EM8I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LFV-EM15I",          			"L1_LFV-EM15I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DPHI-J20s2XE50",          		"L1_DPHI-J20s2XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DPHI-J20XE50",          		"L1_DPHI-J20XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DPHI-CJ20XE50",          		"L1_DPHI-CJ20XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-800",          			"L1_MJJ-800",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-400",          			"L1_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-100",          			"L1_MJJ-100",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT150-JJ15.ETA49",          		"L1_HT150-JJ15.ETA49",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J4-MATCH",          			"L1_J4-MATCH",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LLP-NOMATCH",          		"L1_LLP-NOMATCH",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-MU10TAU12I",          		"L1_DR-MU10TAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15TAU12I-J25",          		"L1_EM15TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-EM15TAU12I-J25",          		"L1_DR-EM15TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1TAU20ITAU12I-J25",          		"L1_TAU20ITAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-TAU20ITAU12I",          		"L1_DR-TAU20ITAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-TAU20ITAU12I-J25",          	"L1_DR-TAU20ITAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LAR-EM",          			"L1_LAR-EM",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LAR-J",          			"L1_LAR-J",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6_MJJ-200",          		"L1_MU6_MJJ-200",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-400",         	"L1_3J20_4J20.0ETA49_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-700",          	"L1_3J20_4J20.0ETA49_MJJ-700",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-800",          	"L1_3J20_4J20.0ETA49_MJJ-800",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-900",          	"L1_3J20_4J20.0ETA49_MJJ-900",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L130M-EM20ITAU12",          		"L1_30M-EM20ITAU12",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1TAU60_DR-TAU20ITAU12I",          		"L1_TAU60_DR-TAU20ITAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
 	["noalg_l1topo_L1LLP-RO",          		"L1_LLP-RO",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
 	["noalg_l1topo_L1MJJ-400-CF",          		"L1_MJJ-400-CF",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
 	["noalg_l1topo_L1SC111-CJ15",          		"L1_SC111-CJ15",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
 	["noalg_l1topo_L1SC85-CJ15",          		"L1_SC85-CJ15",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
 	["noalg_l1topo_L1LATE-MU10_J50",          		"L1_LATE-MU10_J50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
 	["noalg_l1topo_L1LATE-MU10_XE40",          		"L1_LATE-MU10_XE40",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
 
        # #minbias streamer
        ['noalg_mb_L1MBTS_1',             'L1_MBTS_1',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_1_EMPTY',       'L1_MBTS_1_EMPTY',        [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_1_UNPAIRED_ISO','L1_MBTS_1_UNPAIRED_ISO',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_2',             'L1_MBTS_2',        [], ['MinBias', 'express'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_2_EMPTY',       'L1_MBTS_2_EMPTY',        [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_2_UNPAIRED_ISO',  'L1_MBTS_2_UNPAIRED_ISO',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_1_1',             'L1_MBTS_1_1',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_1_1_EMPTY',       'L1_MBTS_1_1_EMPTY',        [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1MBTS_1_1_UNPAIRED_ISO','L1_MBTS_1_1_UNPAIRED_ISO',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],

        # triggers for VdM scan
        ['noalg_L1MBTS_1_BGRP9',      'L1_MBTS_1_BGRP9',        [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['noalg_L1MBTS_2_BGRP9',      'L1_MBTS_2_BGRP9',        [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['noalg_L1RD0_BGRP9',         'L1_RD0_BGRP9',           [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['noalg_L1MBTS_1_BGRP11',     'L1_MBTS_1_BGRP11',       [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['noalg_L1MBTS_2_BGRP11',     'L1_MBTS_2_BGRP11',       [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['noalg_L1RD0_BGRP11',        'L1_RD0_BGRP11',          [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],

	#others
        ['noalg_mb_L1LUCID',             'L1_LUCID',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1LUCID_EMPTY',       'L1_LUCID_EMPTY',        [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1LUCID_UNPAIRED_ISO','L1_LUCID_UNPAIRED_ISO',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD0_EMPTY',          'L1_RD0_EMPTY',        [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD0_FILLED',         'L1_RD0_FILLED',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD0_UNPAIRED_ISO',   'L1_RD0_UNPAIRED_ISO', [], ['MinBias'], ["RATE:Cosmic_MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD1_FILLED',         'L1_RD1_FILLED',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD2_EMPTY',          'L1_RD2_EMPTY',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD2_FILLED',         'L1_RD2_FILLED',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD3_EMPTY',          'L1_RD3_EMPTY',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_mb_L1RD3_FILLED',         'L1_RD3_FILLED',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],

        # LHCF
        ['noalg_L1LHCF',             'L1_LHCF',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1LHCF_UNPAIRED_ISO','L1_LHCF_UNPAIRED_ISO',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1LHCF_EMPTY',       'L1_LHCF_EMPTY',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],


        #AFP streamers
        ['noalg_L1AFP_A_OR_C_J12',                        'L1_AFP_A_OR_C_J12',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_A_AND_C_J12',                        'L1_AFP_A_AND_C_J12',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_A_OR_C',                      'L1_AFP_A_OR_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_L1AFP_A',                      'L1_AFP_A',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_L1AFP_C',                      'L1_AFP_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_L1AFP_A_OR_C_MBTS_2',                      'L1_AFP_A_OR_C_MBTS_2',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_L1AFP_A_AND_C',                      'L1_AFP_A_AND_C',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_L1AFP_A_AND_C_MBTS_2',                      'L1_AFP_A_AND_C_MBTS_2',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['noalg_L1EM3_AFP_A_OR_C',                  'L1_EM3_AFP_A_OR_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_L1EM3_AFP_A_AND_C',                  'L1_EM3_AFP_A_AND_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_L1MU4_AFP_A_OR_C',                        'L1_MU4_AFP_A_OR_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1MU4_AFP_A_AND_C',                        'L1_MU4_AFP_A_AND_C',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],

        ['noalg_L1AFP_A_OR_C_TE5',                        'L1_AFP_A_OR_C_TE5',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_A_AND_C_TE5',                        'L1_AFP_A_AND_C_TE5',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],

        ['noalg_L1AFP_A_OR_C_UNPAIRED_ISO',                        'L1_AFP_A_OR_C_UNPAIRED_ISO',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_A_OR_C_UNPAIRED_NONISO',                        'L1_AFP_A_OR_C_UNPAIRED_NONISO',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],

        ['noalg_L1AFP_A_OR_C_EMPTY',                        'L1_AFP_A_OR_C_EMPTY',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_A_OR_C_FIRSTEMPTY',                        'L1_AFP_A_OR_C_FIRSTEMPTY',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],

        ['noalg_L1AFP_A_AND_C_SPECTOF_J50',                        'L1_AFP_A_AND_C_SPECTOF_J50',                  [],  [PhysicsStream], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_A_AND_C_SPECTOF_J75',                        'L1_AFP_A_AND_C_SPECTOF_J75',                  [],  [PhysicsStream], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_A_AND_C_SPECTOF_J100',                        'L1_AFP_A_AND_C_SPECTOF_J100',                  [],  [PhysicsStream], [ 'RATE:MinBias','BW:MinBias'], -1],

        ['noalg_L1MBTS_2',                        'L1_MBTS_2',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],



        #Zero bias streamer
        ['noalg_zb_L1ZB','L1_ZB', [], ['ZeroBias'], ["BW:ZeroBias", "RATE:ZeroBias"], -1],
        #Zero bias plus HLT jet
        ['j40_L1ZB','L1_ZB', [], ['ZeroBias'], ["BW:ZeroBias", "RATE:ZeroBias"], -1],
        

        # Cosmic calo streamer
        ['noalg_cosmiccalo_L1EM3_EMPTY',         'L1_EM3_EMPTY',        [], ['CosmicCalo'], ["BW:MinBias", "RATE:Cosmic_Calo"], -1],        
        ['noalg_cosmiccalo_L1RD1_EMPTY',         'L1_RD1_EMPTY',        [], ['CosmicCalo',  'express'], ["RATE:Calibration", "BW:Detector"], -1],        
        ['noalg_cosmiccalo_L1J3031ETA49_EMPTY',  'L1_J30.31ETA49_EMPTY',[], ['CosmicCalo'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],
        ['noalg_cosmiccalo_L1J12_EMPTY',         'L1_J12_EMPTY',        [], ['CosmicCalo',  'express'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],
        ['noalg_cosmiccalo_L1EM7_EMPTY',         'L1_EM7_EMPTY',        [], ['CosmicCalo',  'express'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],
        ['noalg_cosmiccalo_L1J30_EMPTY',         'L1_J30_EMPTY',        [], ['CosmicCalo',  'express'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],
        ['noalg_cosmiccalo_L1J12_FIRSTEMPTY',    'L1_J12_FIRSTEMPTY',   [], ['CosmicCalo',  'express'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],
        ['noalg_cosmiccalo_L1J30_FIRSTEMPTY',    'L1_J30_FIRSTEMPTY',   [], ['CosmicCalo',  'express'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],

        ['noalg_cosmiccalo_L1RD1_BGRP10',         'L1_RD1_BGRP10',      [], ['CosmicCalo',  'express'], ["RATE:Calibration", "BW:Detector"], -1],        

        #HLT pass through
        ['noalg_L1All',                  'L1_All',              [], ['HLTPassthrough'], ["RATE:SeededStreamers", "BW:Detector"], -1],
                
        # Enhanced bias Streamer items
        # noalg_eb_ defined in MC menu 
        
        #idmon streamer
        ['noalg_idmon_L1RD0_EMPTY',          'L1_RD0_EMPTY',        [], ['IDMonitoring', 'express'], ["RATE:Monitoring", "BW:Detector"], -1],
        ['noalg_idmon_L1RD0_FILLED',         'L1_RD0_FILLED',        [], ['IDMonitoring'], ["RATE:Monitoring", "BW:Detector"], -1],
        ['noalg_idmon_L1RD0_UNPAIRED_ISO',   'L1_RD0_UNPAIRED_ISO', [], ['IDMonitoring'], ["RATE:Monitoring", "BW:Detector"], -1],
        
        #cosmicmuon streamer
        ['noalg_cosmicmuons_L1MU4_EMPTY',   'L1_MU4_EMPTY',        [], ['CosmicMuons', 'express'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],        
        ['noalg_cosmicmuons_L1MU11_EMPTY',  'L1_MU11_EMPTY',        [], ['CosmicMuons', 'express'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],        

        ['noalg_L1LowLumi', 'L1_LowLumi', [], [PhysicsStream,'express'], ["RATE:SeededStreamers", "BW:Other"], -1],

        
        # M9 L1Topo streamers
        ['noalg_L1LFV-MU',          'L1_LFV-MU',          [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        ['noalg_L1DY-DR-2MU4',      'L1_DY-DR-2MU4',      [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        ['noalg_L1DY-BOX-2MU6',     'L1_DY-BOX-2MU6',     [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        ['noalg_L1DR-TAU20ITAU12I', 'L1_DR-TAU20ITAU12I', [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        ['noalg_L1DR-MU10TAU12I',   'L1_DR-MU10TAU12I',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],

        # Note: These EB noalg chains (bar RD3) are multi-seeded. All L1 seeds must be PS=1 during EB campaign for unweighting to work
        ## Enhanced Bias Physics ##
        ['noalg_eb_L1RD3_FILLED',             'L1_RD3_FILLED',             [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['noalg_eb_L1PhysicsLow_noPS',        'L1_PhysicsLow_noPS',        [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['noalg_eb_L1PhysicsHigh_noPS',       'L1_PhysicsHigh_noPS',       [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ## Enhanced Bias Background ##
        ['noalg_eb_L1RD3_EMPTY',              'L1_RD3_EMPTY',              [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['noalg_eb_L1EMPTY_noPS',             'L1_EMPTY_noPS',             [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['noalg_eb_L1FIRSTEMPTY_noPS',        'L1_FIRSTEMPTY_noPS',        [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['noalg_eb_L1UNPAIRED_ISO_noPS',      'L1_UNPAIRED_ISO_noPS',      [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['noalg_eb_L1UNPAIRED_NONISO_noPS',   'L1_UNPAIRED_NONISO_noPS',   [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['noalg_eb_L1ABORTGAPNOTCALIB_noPS',  'L1_ABORTGAPNOTCALIB_noPS',  [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
	
        #L1Calo requested streamers
	['noalg_L1J100',  'L1_J100',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1],
	['noalg_L1J100.31ETA49',  'L1_J100.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ], 
        ['noalg_L1J12',  'L1_J12',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_mb_L1J12',  'L1_J12',   [], ['MinBias'], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
	['noalg_L1J120',  'L1_J120',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1],
	['noalg_L1J15',  'L1_J15',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
	['noalg_L1J15.31ETA49',  'L1_J15.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
	['noalg_L1J20',  'L1_J20',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J20.28ETA31',  'L1_J20.28ETA31',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J20.31ETA49',  'L1_J20.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J25',  'L1_J25',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J30',  'L1_J30',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J30.31ETA49',  'L1_J30.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J40',  'L1_J40',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J400',  'L1_J400',   [], [PhysicsStream, 'express'], ["Primary:20000","RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J50.31ETA49',  'L1_J50.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J75',  'L1_J75',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J75.31ETA49',  'L1_J75.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ], 
        ['noalg_L1J85',  'L1_J85',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],

        ['noalg_L1EM8VH',  'L1_EM8VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM22VHI',  'L1_EM22VHI',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM20VH',  'L1_EM20VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM15VH',  'L1_EM15VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM10VH',  'L1_EM10VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],

        ['noalg_L1TAU8',  'L1_TAU8',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Tau"], -1 ],
        ['noalg_L1TAU60',  'L1_TAU60',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Tau"], -1],
        ['noalg_L1TAU40',  'L1_TAU40',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Tau"], -1],

        ['noalg_L1XE80',  'L1_XE80',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1XE70',  'L1_XE70',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1XE60',  'L1_XE60',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1XE55',  'L1_XE55',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1XE50',  'L1_XE50',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1XE45',  'L1_XE45',   [], [PhysicsStream, 'express'], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1XE40',  'L1_XE40',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
	
        ['noalg_L1TE15',  'L1_TE15',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE25',  'L1_TE25',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        #['noalg_L1TE70',  'L1_TE70',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        #['noalg_L1TE60',  'L1_TE60',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE50',  'L1_TE50',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE40',  'L1_TE40',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE30',  'L1_TE30',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE20',  'L1_TE20',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE10',   'L1_TE10', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE5',   'L1_TE5', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
#        ['noalg_L1TE4',   'L1_TE4', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
#        ['noalg_L1TE3',   'L1_TE3', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
#        ['noalg_L1TE2',   'L1_TE2', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        #['noalg_L1TE5.0ETA24',   'L1_TE5.0ETA24', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        #['noalg_L1TE10.0ETA24',   'L1_TE10.0ETA24', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        #['noalg_L1TE15.0ETA24',   'L1_TE15.0ETA24', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        #['noalg_L1TE20.0ETA24',  'L1_TE20.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        #['noalg_L1TE25.0ETA24',   'L1_TE25.0ETA24', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        #['noalg_L1TE30.0ETA24',  'L1_TE30.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        #['noalg_L1TE40.0ETA24',  'L1_TE40.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        
        ]

    TriggerFlags.MonitorSlice.signatures = [
        ['l1calooverflow','', [], ['L1Calo'], ['RATE:Monitoring','BW:Other'], -1],
        ['l1topodebug', 'L1_Topo', [], ['L1TopoMismatches'], ['RATE:Monitoring','BW:Other'], -1],
        ## # enhancedbias
        
        ['mistimemonl1bccorr', '', [], ['Mistimed'], ["RATE:Monitoring","BW:Other"], -1],
        ['mistimemonl1bccorrnomu', '', [], ['Mistimed'], ["RATE:Monitoring","BW:Other"], -1],
        ['mistimemoncaltimenomu', '', [], ['Mistimed'], ["RATE:Monitoring","BW:Other"], -1],
        ['mistimemoncaltime', '', [], ['Mistimed'], ["RATE:Monitoring","BW:Other"], -1],
        ['mistimemonj400', '', [], ['Mistimed'], ["RATE:Monitoring","BW:Other"], -1],
        
        ['timeburner', '', [], ['DISCARD'], ["RATE:DISCARD","BW:DISCARD"], -1],
        ['robrequest', '', [], ['DISCARD'], ["RATE:DISCARD","BW:DISCARD"], -1],
        ['robrequest_L1RD0_EMPTY', 'L1_RD0_EMPTY', [], ['DISCARD'], ["RATE:DISCARD","BW:DISCARD"], -1],   
        
        ['costmonitor', '', [], ['CostMonitoring'], ['RATE:Monitoring','BW:Other'],1],

        ['cscmon_L1All', 'L1_All',         [], ['CSC'], ["RATE:Monitoring", "BW:Detector"], -1]

        ]

    # Random Seeded EB chains which select at the HLT based on L1 TBP bits
    TriggerFlags.EnhancedBiasSlice.signatures = [
        # Enhanced bias HLT items
        ['eb_low_L1RD2_FILLED',               'L1_RD2_FILLED',             [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['eb_high_L1RD2_FILLED',              'L1_RD2_FILLED',             [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ]


    ###############################################################
    #################################################################
    signatureList=[]
    for prop in dir(TriggerFlags):
        if prop[-5:]=='Slice':
            sliceName=prop
            slice=getattr(TriggerFlags,sliceName)
            if slice.signatures():
                signatureList.extend(slice.signatures())
            else:
                log.debug('SKIPPING '+str(sliceName))
    mySigList=[]
    for allInfo in signatureList:
        mySigList.append(allInfo[0])
    mydict={}
    for chain in mySigList:
        mydict[chain]=[-1,0,0]
    mydict.update(Prescales.HLTPrescales_cosmics)
    mydict.update(Prescales.HLTPrescales_unprescaled_only_physics_prescale)
    mydict.update(Prescales.L1Prescales_unprescaled_only_physics_prescale)
    from copy import deepcopy
    Prescales.HLTPrescales_cosmics = deepcopy(mydict)
    Prescales.L1Prescales_unprescaled_only_physics_prescale  = deepcopy(mydict)
    Prescales.HLTPrescales_unprescaled_only_physics_prescale = deepcopy(mydict)


class Prescales:
    #   Item name             | Prescale
    #----------------------------------------------------------
    L1Prescales = {}

    #   Signature name   | [ HLTprescale, HLTpass-through, rerun]
    #   - Prescale values should be a positive integer (default=1)
    #   - If the current pass_through value is non-zero,
    #     the value given here will be used as pass_through rate
    #     Assuming that pass through chains are configured so
    #     in the slice files and won't change. Also prescale
    #     and pass_through will not be used together.
    #   - If only the first value is specified,
    #     the default value of pass-through (=0) will be used
    #----------------------------------------------------------
    HLTPrescales = {
        'larnoiseburst_rerun'       : [ 0, 0 , 1, "LArNoiseBurst"],
        'larnoiseburst_loose_rerun' : [ 0, 0 , 1, "LArNoiseBurst"],
        }

    L1Prescales_cosmics  = {}
    HLTPrescales_cosmics = {}
    chain_list=[
        'e0_perf_L1EM3_EMPTY',
        'g0_perf_L1EM3_EMPTY',
        'ht0_L1J12_EMPTY',
        'id_cosmicid_L1MU11_EMPTY',
        'id_cosmicid_L1MU4_EMPTY',
        'j0_L1J12_EMPTY',
        'j0_perf_boffperf_L1J12_EMPTY',
        'larcalib_L1EM3_EMPTY',
        'larcalib_L1J12_EMPTY',
        'larcalib_L1J3031ETA49_EMPTY',
        'larcalib_L1TAU8_EMPTY',
        'larps_L1EM3_EMPTY',
        'larps_L1EM7_EMPTY',
        'larps_L1J12_EMPTY',
        'larps_L1J3031ETA49_EMPTY',
        'larps_L1J30_EMPTY',
        'larps_L1TAU8_EMPTY',
        'mb_sptrk_costr',
        'mb_sptrk_costr_L1RD0_EMPTY',
        'mu4_cosmic_L1MU11_EMPTY',
        'mu4_cosmic_L1MU4_EMPTY',
        'mu4_msonly_cosmic_L1MU11_EMPTY',
        'mu4_msonly_cosmic_L1MU4_EMPTY',
        'sct_noise',
        'tilecalib_laser',
        ]
    HLTPrescales_cosmics.update(dict(map(None,chain_list,len(chain_list)*[ [1, 0, 0] ])))

    HLTPrescales_unprescaled_only_physics_prescale = {}
    HLTPrescales_unprescaled_only_physics_prescale.update(dict(map(None,chain_list,len(chain_list)*[ [-1, 0, -1] ])))
    L1Prescales_unprescaled_only_physics_prescale = {}
    L1Prescales_unprescaled_only_physics_prescale.update(dict(map(None,chain_list,len(chain_list)*[ [-1, 0, -1] ])))


    HLT_chain_list = [
        'xe110_pufit_L1XE50',
        'xe110_pufit_wEFMu_L1XE50',
        'xe120_pufit_L1XE50',
        'xe120_pufit_wEFMu_L1XE50',
        'xe120_mht_xe80_L1XE50',
        'tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I-J25',
        'g35_medium_L1EM24VHI_tau25_dipion3_tracktwo_60mVis10000',
        'e60_etcut_trkcut_L1EM24VHIM_j15_perf_xe60_6dphi15_mt35',
        'e60_etcut_trkcut_L1EM24VHIM_xs30_j15_perf_xe30_6dphi15_mt35',
        'xe100_pufit_L1XE55',
        'xe100_pufit_L1XE60',
        '2e24_lhvloose_nod0_2j15_gsc35_boffperf_split',
        '2e24_lhvloose_nod0_2j35_boffperf_split',
        '2j15_gsc35_bmv2c1040_split_2j15_gsc35_boffperf_split_L14J15.0ETA25',
        '2j15_gsc35_bmv2c1050_split_3j15_gsc35_boffperf_split',
        '2j15_gsc35_bmv2c1070_split_2j15_gsc35_bmv2c1085_split_L14J15.0ETA25',
        '2j25_gsc45_bmv2c1050_split_2j25_gsc45_boffperf_split_L14J15.0ETA25',
        '2j25_gsc45_bmv2c1060_split_3j25_gsc45_boffperf_split',
        '2j25_gsc45_bmv2c1070_split_xe80_mht_L12J15_XE55',
        '2j35_gsc55_bmv2c1050_split_ht300_L1HT190-J15s5.ETA21',
        '2j35_gsc55_bmv2c1060_split_2j35_gsc55_boffperf_split_L14J15.0ETA25',
        '2mu14_2j15_gsc35_boffperf_split',
        '2mu14_2j35_boffperf_split',
        '3j15_gsc35_bmv2c1070_split_j15_gsc35_boffperf_split_L14J15.0ETA25',
        '3j15_gsc35_bmv2c1077_split_xe60_mht_L13J15.0ETA25_XE40',
        '3j50_gsc65_bmv2c1077_split_L13J35.0ETA23',
        '4j15_gsc35_bmv2c1077_split_L14J15.0ETA25',
        'e28_lhmedium_nod0_L1EM24VHI_mu8noL1_2j15_gsc35_boffperf_split',
        'e28_lhmedium_nod0_L1EM24VHI_mu8noL1_2j35_boffperf_split',
        'e28_lhtight_nod0_ivarloose_j110_gsc150_boffperf_split_j15_gsc35_boffperf_split',
        'e28_lhtight_nod0_ivarloose_j150_boffperf_split_j35_boffperf_split',
        'g25_medium_L1EM22VHI_2j35_0eta490_bmv2c1077_split_2j35_0eta490',
        'g25_medium_L1EM22VHI_j35_0eta490_bmv2c1077_split_3j35_0eta490_invm700',
        'ht300_2j40_0eta490_invm700_L1HT150-J20s5.ETA31_MJJ-400-CF_AND_2j25_gsc45_bmv2c1070_split',
        'j110_gsc150_boffperf_split_2j35_gsc55_bmv2c1070_split_L1J85_3J30',
        'j150_gsc175_bmv2c1060_split_j45_gsc60_bmv2c1060_split',
        'j175_gsc225_bmv2c1040_split',
        'j225_gsc275_bmv2c1060_split',
        'j225_gsc300_bmv2c1070_split',
        'j225_gsc360_bmv2c1077_split',
        'j35_gsc55_bmv2c1050_split_ht700_L1HT190-J15s5.ETA21',
        'j35_gsc55_bmv2c1070_split_2j45_320eta490_L1J25.0ETA23_2J15.31ETA49',
        'j55_gsc75_bmv2c1040_split_3j55_gsc75_boffperf_split',
        'j55_gsc80_bmv2c1070_split_j45_gsc60_bmv2c1085_split_j45_320eta490',
        'j60_gsc100_bmv2c1050_split_xe80_mht_L1XE60',
        'j60_gsc85_bmv2c1050_split_3j60_gsc85_boffperf_split',
        'j80_0eta240_j60_j45_320eta490_AND_2j25_gsc45_bmv2c1070_split',
        'mu26_ivarmedium_j110_gsc150_boffperf_split_j15_gsc35_boffperf_split',
        'mu26_ivarmedium_j150_boffperf_split_j35_boffperf_split',
        'mu6_j225_gsc400_boffperf_split_dr05_dz02',
        '2mu10_bBmumuxv2',
        '2mu10_bJpsimumu',
        '2mu10_bUpsimumu',
        '2mu6_bBmumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
        '2mu6_bBmumux_BpmumuKp_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
        '2mu6_bBmumux_BsmumuPhi_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
        '2mu6_bJpsimumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
        '2mu6_bJpsimumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',
        '2mu6_bUpsimumu_L1BPH-8M15-2MU6_BPH-0DR22-2MU6',
        '2mu6_mu4_bTau_L12MU6_3MU4',
        '2mu6_mu4_bUpsi',
        '3mu3_mu3noL1_L13MU4',
        '3mu3_mu3noL1_calotag_0eta010_L13MU4',
        '3mu4_bDimu',
        '3mu4_bJpsi',
        '3mu4_bPhi',
        '3mu4_bTau',
        '3mu4_bUpsi',
        '3mu4_mu2noL1',
        '3mu4_mu4noL1',
        '3mu4_mu4noL1_calotag_0eta010',
        '3mu6_bJpsi',
        '3mu6_bTau',
        '3mu6_bUpsi',
        '4mu4',
        'mu11_2mu4noL1_bTau_L1MU11_2MU6',
        'mu11_mu6_bBmumu',
        'mu11_mu6_bBmumux_BpmumuKp',
        'mu11_mu6_bBmumuxv2',
        'mu11_mu6_bDimu',
        'mu11_mu6_bDimu2700',
        'mu11_mu6_bDimu2700_Lxy0',
        'mu11_mu6_bDimu_Lxy0',
        'mu11_mu6_bJpsimumu',
        'mu11_mu6_bJpsimumu_Lxy0',
        'mu11_mu6_bPhi',
        'mu11_mu6_bTau',
        'mu11_mu6_bUpsimumu',
        'mu11_mu6noL1_bPhi_L1MU11_2MU6',
        'mu20_mu6noL1_bTau',
        'mu6_2mu4_bTau_L1MU6_3MU4',
        'mu6_2mu4_bUpsi',
        'larcalib_L1J3031ETA49_EMPTY',
        'larhec_L1EM7_FIRSTEMPTY',
        'larhec_L1J12_FIRSTEMPTY',
        'larhec_L1J30_FIRSTEMPTY',
        'larnoiseburst_L1J100',
        'larnoiseburst_L1J40_XE60',
        'larnoiseburst_L1XE60',
        'larnoiseburst_L1XE70',
        'larnoiseburst_L1XE80',
        'larps_L1J3031ETA49_EMPTY',
        'larps_L1J30_FIRSTEMPTY',
        'noalg_L1TGC_BURST',
        'satu20em_l1satmon_L1EM20VH_FIRSTEMPTY',
        'satu20em_l1satmon_L1J100',
        'satu20em_l1satmon_L1J100.31ETA49',
        'satu20em_l1satmon_L1J100.31ETA49_FIRSTEMPTY',
        'satu20em_l1satmon_L1J100_FIRSTEMPTY',
        'tilecalib_laser',
        '2e12_lhloose_nod0_mu10',
        '2e12_lhmedium_nod0_mu10',
        '2e17_lhvloose_nod0_L12EM15VHI',
        '2e19_lhvloose_nod0',
        '2e24_lhvloose_nod0',
        '2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',
        '2e5_lhvloose_nod0_j40_xe70_pufit_2dphi10_L1XE60',
        '2g10_loose_mu20',
        '2g10_medium_mu20',
        '2g20_tight_icalotight_L12EM15VHI',
        '2g20_tight_icalovloose_L12EM15VHI',
        '2g22_tight_L12EM15VHI',
        '2g22_tight_L1EM7_EMPTY',
        '2g22_tight_L1EM7_UNPAIRED_ISO',
        '2g22_tight_icalotight_L12EM15VHI',
        '2g22_tight_icalovloose_L12EM15VHI',
        '2g25_loose_g15_loose',
        '2g25_loose_g20_loose',
        '2g25_tight_L12EM20VH',
        '2g50_loose_L12EM20VH',
        '2g50_tight_L1EM7_EMPTY',
        '2g50_tight_L1EM7_UNPAIRED_ISO',
        '2g60_loose_L12EM20VH',
        '3g25_loose',
        'e12_lhloose_nod0_2mu10',
        'e12_lhmedium_nod0_2mu10',
        'e140_lhloose_nod0',
        'e140_lhloose_nod0_L1EM24VHI',
        'e140_lhloose_nod0_L1EM24VHIM',
        'e17_lhloose_nod0_mu14',
        'e20_lhtight_nod0_ivarloose_3j20_L1EM18VHI_3J20',
        'e24_lhloose_nod0_2e12_lhloose_nod0_L1EM20VH_3EM10VH',
        'e24_lhmedium_nod0_2g12_loose',
        'e24_lhmedium_nod0_2g12_medium',
        'e24_lhmedium_nod0_L1EM20VH_g25_medium',
        'e24_lhvloose_nod0_2e12_lhvloose_nod0_L1EM20VH_3EM10VH',
        'e25_mergedtight_g35_medium_Heg',
        'e25_mergedtight_ivarloose_g35_medium_icalovloose_Heg',
        'e26_lhmedium_nod0_L1EM22VHIM_mu8noL1',
        'e26_lhmedium_nod0_mu8noL1',
        'e26_lhtight_nod0_L1EM22VHIM_e15_etcut_L1EM7_Zee',
        'e26_lhtight_nod0_e15_etcut_L1EM7_Zee',
        'e26_lhtight_nod0_ivarloose',
        'e26_lhtight_nod0_ivarloose_L1EM22VHIM',
        'e28_lhmedium_nod0_L1EM24VHIM_mu8noL1',
        'e28_lhmedium_nod0_mu8noL1',
        'e28_lhtight_nod0_L1EM24VHIM_e15_etcut_L1EM7_Zee',
        'e28_lhtight_nod0_e15_etcut_L1EM7_Zee',
        'e28_lhtight_nod0_ivarloose',
        'e28_lhtight_nod0_ivarloose_L1EM22VHI',
        'e28_lhtight_nod0_ivarloose_L1EM24VHIM',
        'e28_lhtight_nod0_noringer_ivarloose',
        'e28_lhtight_nod0_noringer_ivarloose_L1EM24VHIM',
        'e300_etcut',
        'e300_etcut_L1EM24VHIM',
        'e30_mergedtight_g35_medium_Heg',
        'e32_lhtight_nod0_ivarloose',
        'e5_lhloose_nod0_j40_xe70_pufit_2dphi10_L1XE60',
        'e5_lhloose_nod0_j50_xe70_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',
        'e5_lhloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',
        'e5_lhmedium_nod0_j50_xe80_pufit_2dphi10_L1J40_XE50_DPHI-J20s2XE50',
        'e5_lhmedium_nod0_mu4_j30_xe65_pufit_2dphi10_L1MU4_XE60',
        'e60_lhmedium_nod0',
        'e60_lhmedium_nod0_L1EM24VHI',
        'e60_lhmedium_nod0_L1EM24VHIM',
        'e70_lhloose_nod0_L1EM24VHIM_xe70noL1',
        'e70_lhloose_nod0_xe70noL1',
        'e7_lhmedium_nod0_mu24',
        'e80_lhloose_nod0_L1EM24VHIM_xe80noL1',
        'e80_lhloose_nod0_L1EM24VHI_xe80noL1',
        'e80_lhloose_nod0_xe80noL1',
        'e80_lhmedium_nod0',
        'e80_lhmedium_nod0_L1EM24VHI',
        'e80_lhmedium_nod0_L1EM24VHIM',
        'g0_hiptrt_L1EM22VHI',
        'g0_hiptrt_L1EM24VHI',
        'g0_hiptrt_L1EM24VHIM',
        'g100_tight_L1EM22VHI_3j50noL1',
        'g100_tight_L1EM24VHIM_3j50noL1',
        'g100_tight_L1EM24VHI_3j50noL1',
        'g100_tight_icalovloose_L1EM22VHI_3j50noL1',
        'g100_tight_icalovloose_L1EM24VHIM_3j50noL1',
        'g100_tight_icalovloose_L1EM24VHI_3j50noL1',
        'g140_loose',
        'g140_tight',
        'g140_tight_L1EM24VHIM',
        'g15_loose_2mu10_msonly',
        'g15_loose_2mu10_msonly_L1MU4_UNPAIRED_ISO',
        'g15_loose_2mu10_msonly_L1MU6_EMPTY',
        'g200_loose',
        'g200_loose_L1EM24VHIM',
        'g20_tight_icaloloose_j15_gsc35_bmv2c1077_split_3j35_0eta490_invm500',
        'g25_medium_L1EM22VHI_4j35_0eta490_invm1000',
        'g25_medium_mu24',
        'g27_medium_L1EM24VHI_2j15_gsc35_bmv2c1077_split_2j35_0eta490',
        'g27_medium_L1EM24VHI_j15_gsc35_bmv2c1077_split_3j35_0eta490_invm700',
        'g300_etcut',
        'g300_etcut_L1EM24VHI',
        'g300_etcut_L1EM24VHIM',
        'g35_loose_L1EM24VHIM_mu15_mu2noL1',
        'g35_loose_L1EM24VHIM_mu18',
        'g35_loose_L1EM24VHI_mu15_mu2noL1',
        'g35_loose_L1EM24VHI_mu18',
        'g35_medium_g25_medium_L12EM20VH',
        'g35_medium_g25_medium_L1EM7_EMPTY',
        'g35_medium_g25_medium_L1EM7_UNPAIRED_ISO',
        'g35_tight_icalotight_L1EM24VHIM_mu15noL1_mu2noL1',
        'g35_tight_icalotight_L1EM24VHIM_mu18noL1',
        'g35_tight_icalotight_L1EM24VHI_mu15noL1_mu2noL1',
        'g35_tight_icalotight_L1EM24VHI_mu18noL1',
        'g40_tight_icalotight_L1EM24VHIM_mu15noL1_mu2noL1',
        'g40_tight_icalotight_L1EM24VHIM_mu18noL1',
        'g40_tight_icalotight_L1EM24VHI_mu15noL1_mu2noL1',
        'g40_tight_icalotight_L1EM24VHI_mu18noL1',
        'g80_loose_L1EM24VHIM_xe80noL1',
        'g80_loose_L1EM24VHI_xe80noL1',
        'g80_loose_icalovloose_L1EM24VHIM_xe80noL1',
        'g80_loose_icalovloose_L1EM24VHI_xe80noL1',
        'g80_loose_icalovloose_xe80noL1',
        'g80_loose_xe80noL1',
        'g85_tight_L1EM22VHI_3j50noL1',
        'g85_tight_L1EM24VHIM_3j50noL1',
        'g85_tight_L1EM24VHI_3j50noL1',
        'g85_tight_icalovloose_L1EM22VHI_3j50noL1',
        'g85_tight_icalovloose_L1EM24VHIM_3j50noL1',
        'g85_tight_icalovloose_L1EM24VHI_3j50noL1',
        'mu20_mu8noL1_e9_lhvloose_nod0',
        'mu20_mu8noL1_e9_lhvloose_nod0_L1EM7_MU20',
        'mu20_mu8noL1_e9_lhvloose_nod0_L1EM8VH_MU20',
        '10j40_L14J15',
        '10j40_L14J150ETA25',
        '10j40_L14J20',
        '10j40_L16J15',
        '2j225_gsc250_boffperf_split_0eta240_j70_gsc120_boffperf_split_0eta240',
        '2j225_gsc250_boffperf_split_0eta240_j70_gsc140_boffperf_split_0eta240',
        '2j330_a10t_lcw_jes_40smcINF_L1J100',
        '2j340_a10t_lcw_jes_40smcINF_L1J100',
        '2j350_a10t_lcw_jes_40smcINF_L1J100',
        '3j200',
        '3j225',
        '3j250',
        '4j120',
        '4j130',
        '4j60_gsc115_boffperf_split',
        '4j70_gsc120_boffperf_split',
        '4j70_gsc130_boffperf_split',
        '5j100',
        '5j100_L14J150ETA25',
        '5j50_gsc70_boffperf_split_0eta240',
        '5j50_gsc70_boffperf_split_0eta240_L14J150ETA25',
        '5j55_gsc75_boffperf_split_0eta240',
        '5j55_gsc75_boffperf_split_0eta240_L14J150ETA25',
        '5j60_gsc100_boffperf_split',
        '5j60_gsc100_boffperf_split_L14J150ETA25',
        '5j60_gsc85_boffperf_split',
        '5j60_gsc85_boffperf_split_L14J150ETA25',
        '5j60_gsc90_boffperf_split',
        '5j60_gsc90_boffperf_split_L14J150ETA25',
        '5j70_0eta240',
        '5j70_0eta240_L14J15',
        '5j70_0eta240_L14J150ETA25',
        '5j75_0eta240',
        '5j75_0eta240_L14J150ETA25',
        '5j85',
        '5j85_0eta240',
        '5j85_0eta240_L14J150ETA25',
        '5j85_L14J15',
        '5j85_L14J150ETA25',
        '5j90',
        '5j90_L14J150ETA25',
        '6j35_gsc55_boffperf_split_0eta240_L14J150ETA25',
        '6j35_gsc55_boffperf_split_0eta240_L14J20',
        '6j35_gsc55_boffperf_split_0eta240_L15J150ETA25',
        '6j45_gsc60_boffperf_split_0eta240_L14J150ETA25',
        '6j45_gsc60_boffperf_split_0eta240_L14J20',
        '6j45_gsc60_boffperf_split_0eta240_L15J150ETA25',
        '6j50_gsc70_boffperf_split',
        '6j50_gsc70_boffperf_split_L14J150ETA25',
        '6j50_gsc70_boffperf_split_L15J150ETA25',
        '6j55_0eta240_L14J15',
        '6j55_0eta240_L14J150ETA25',
        '6j55_0eta240_L14J20',
        '6j55_0eta240_L15J150ETA25',
        '6j60_0eta240_L14J150ETA25',
        '6j60_0eta240_L14J20',
        '6j60_0eta240_L15J150ETA25',
        '6j60_gsc85_boffperf_split',
        '6j60_gsc85_boffperf_split_L14J150ETA25',
        '6j60_gsc85_boffperf_split_L15J150ETA25',
        '6j70',
        '6j70_L14J15',
        '6j70_L14J150ETA25',
        '6j85',
        '6j85_L14J150ETA25',
        '7j25_gsc45_boffperf_split_0eta240_L14J150ETA25',
        '7j25_gsc45_boffperf_split_0eta240_L14J20',
        '7j25_gsc45_boffperf_split_0eta240_L15J150ETA25',
        '7j25_gsc45_boffperf_split_L14J150ETA25',
        '7j25_gsc45_boffperf_split_L14J20',
        '7j25_gsc45_boffperf_split_L15J150ETA25',
        '7j25_gsc50_boffperf_split_0eta240_L14J150ETA25',
        '7j25_gsc50_boffperf_split_0eta240_L14J20',
        '7j25_gsc50_boffperf_split_0eta240_L15J150ETA25',
        '7j25_gsc50_boffperf_split_L14J150ETA25',
        '7j25_gsc50_boffperf_split_L14J20',
        '7j25_gsc50_boffperf_split_L15J150ETA25',
        '7j45',
        '7j45_0eta240',
        '7j45_0eta240_L14J150ETA25',
        '7j45_0eta240_L14J20',
        '7j45_0eta240_L15J150ETA25',
        '7j45_L14J15',
        '7j45_L14J150ETA25',
        '7j45_L14J20',
        '7j50',
        '7j50_L14J150ETA25',
        '7j50_L14J20',
        'ht1000_L1J100',
        'ht1200_L1J100',
        'ht1400_L1J100',
        'j0_perf_ds1_L1J100',
        'j225_gsc420_boffperf_split',
        'j225_gsc440_boffperf_split',
        'j225_gsc450_boffperf_split',
        'j225_gsc460_boffperf_split',
        'j225_gsc480_boffperf_split',
        'j225_gsc500_boffperf_split',
        'j260_320eta490',
        'j260_320eta490_jes',
        'j260_320eta490_lcw',
        'j260_320eta490_lcw_jes',
        'j260_320eta490_lcw_nojcalib',
        'j260_320eta490_lcw_sub',
        'j260_320eta490_lcw_subjes',
        'j260_320eta490_nojcalib',
        'j260_320eta490_sub',
        'j260_320eta490_subjes',
        'j30_jes_cleanLLP_PS_llp_L1LLP-NOMATCH',
        'j30_jes_cleanLLP_PS_llp_L1LLP-RO',
        'j30_jes_cleanLLP_PS_llp_L1TAU100',
        'j30_jes_cleanLLP_PS_llp_L1TAU30_EMPTY',
        'j30_jes_cleanLLP_PS_llp_L1TAU30_UNPAIRED_ISO',
        'j30_jes_cleanLLP_PS_llp_noiso_L1LLP-NOMATCH',
        'j30_jes_cleanLLP_PS_llp_noiso_L1LLP-RO',
        'j30_jes_cleanLLP_PS_llp_noiso_L1TAU100',
        'j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_EMPTY',
        'j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_UNPAIRED_ISO',
        'j30_xe60_razor220',
        'j360_320eta490',
        'j420',
        'j420_a10t_lcw_jes_40smcINF_L1J100',
        'j440',
        'j440_a10t_lcw_jes_40smcINF_L1J100',
        'j450',
        'j460',
        'j460_a10_lcw_subjes_L1J100',
        'j460_a10r_L1J100',
        'j460_a10t_lcw_jes_L1J100',
        'j480',
        'j480_a10_lcw_subjes_L1J100',
        'j480_a10r_L1J100',
        'j480_a10t_lcw_jes_L1J100',
        'j500',
        'j500_a10_lcw_subjes_L1J100',
        'j500_a10r_L1J100',
        'j500_a10t_lcw_jes_L1J100',
        'j520_a10_lcw_subjes_L1J100',
        'j520_a10r_L1J100',
        'j520_a10t_lcw_jes_L1J100',
        'j540_a10_lcw_subjes_L1J100',
        'j540_a10r_L1J100',
        'j540_a10t_lcw_jes_L1J100',
        'j55_0eta240_xe50_L1J30_EMPTY',
        'noalg_L1J400',
        'noalg_cosmiccalo_L1J3031ETA49_EMPTY',
        'j80_bmv2c1050_split_xe60_L12J50_XE40',
        'xe100_pufit_L1XE55',
        'xe100_pufit_L1XE60',
        'xe110_L1XE55',
        'xe110_pufit_L1XE55',
        'xe110_pufit_L1XE60',
        'xe110_pufit_L1XE70',
        'xe110_pufit_wEFMu_L1XE55',
        'xe110_wEFMu_L1XE55',
        'xe120_L1XE55',
        'xe120_mht_xe80_L1XE55',
        'xe120_mht_xe80_L1XE60',
        'xe120_pufit_L1XE55',
        'xe120_pufit_L1XE60',
        'xe120_pufit_L1XE70',
        'xe120_pufit_wEFMu_L1XE55',
        'xe120_pufit_wEFMu_L1XE60',
        'xe130_L1XE55',
        '2mu14',
        '2mu15_L12MU10',
        '2mu4_invm1_j20_xe40_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30',
        '2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J20_XE30_DPHI-J20s2XE30',
        '2mu4_invm1_j20_xe60_pufit_2dphi10_L12MU4_J40_XE50',
        '2mu4_mu3_mu2noL1_bNocut_L13MU4',
        '3mu3_mu3noL1_bNocut_L13MU4',
        '3mu6',
        '3mu6_msonly',
        '3mu6_msonly_L1MU4_UNPAIRED_ISO',
        '3mu6_msonly_L1MU6_EMPTY',
        '3mu8_msonly',
        '4mu4_bNocut',
        'e5_lhvloose_nod0_mu4_j30_xe40_pufit_2dphi10_L1MU4_J30_XE40_DPHI-J20s2XE30',
        'j30_muvtx',
        'j30_muvtx_L1MU4_UNPAIRED_ISO',
        'j30_muvtx_L1MU6_EMPTY',
        'j30_muvtx_noiso',
        'j30_muvtx_noiso_L1MU4_UNPAIRED_ISO',
        'j30_muvtx_noiso_L1MU6_EMPTY',
        'mu0_muoncalib',
        'mu10_mgonly_L1LATE-MU10_J50',
        'mu10_mgonly_L1LATE-MU10_XE40',
        'mu10_mgonly_L1LATE-MU10_XE50',
        'mu11_2mu4noL1_bNocut_L1MU11_2MU6',
        'mu13_mu13_idperf_Zmumu',
        'mu14_ivarloose_3j20_L1MU10_3J20',
        'mu20_2mu4noL1',
        'mu20_ivarmedium_L1MU10_2J15_J20',
        'mu20_ivarmedium_L1MU10_2J20',
        'mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU20_J40',
        'mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU20_XE30',
        'mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU4_UNPAIRED_ISO',
        'mu20_msonly_iloosems_mu6noL1_msonly_nscan05_L1MU6_EMPTY',
        'mu22_2mu4noL1',
        'mu22_mu8noL1',
        'mu22_mu8noL1_calotag_0eta010',
        'mu22_mu8noL1_mu6noL1',
        'mu24_2mu4noL1',
        'mu24_mu10noL1',
        'mu24_mu10noL1_calotag_0eta010',
        'mu24_mu8noL1',
        'mu24_mu8noL1_calotag_0eta010',
        'mu26_ivarmedium',
        'mu26_mu10noL1',
        'mu26_mu10noL1_calotag_0eta010',
        'mu26_mu8noL1',
        'mu26_mu8noL1_calotag_0eta010',
        'mu28_ivarmedium',
        'mu28_mu8noL1',
        'mu28_mu8noL1_calotag_0eta010',
        'mu4_j90_xe90_pufit_2dphi10_L1MU4_J50_XE50_DPHI-J20s2XE30',
        'mu4_j90_xe90_pufit_2dphi10_L1MU4_XE60',
        'mu50',
        'mu60',
        'mu60_0eta105_msonly',
        'mu80',
        'noalg_bkg_L1J15.31ETA49_UNPAIRED_ISO',
        'noalg_l1calo_L1J400',
        'e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo',
        'e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25',
        'e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15VHI_2TAU12IM_4J12',
        'e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_xe50',
        'e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50',
        'e24_lhmedium_nod0_ivarloose_tau35_medium1_tracktwo',
        'g25_medium_L1EM24VHIM_tau25_dikaonmass_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHIM_tau25_dipion1loose_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHIM_tau25_dipion2_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHIM_tau25_kaonpi1_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHIM_tau25_kaonpi2_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHIM_tau25_singlepion_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHI_tau25_dikaonmass_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHI_tau25_dipion1loose_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHI_tau25_dipion2_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHI_tau25_kaonpi1_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHI_tau25_kaonpi2_tracktwo_50mVis10000',
        'g25_medium_L1EM24VHI_tau25_singlepion_tracktwo_50mVis10000',
        'mu14_ivarloose_L1MU11_tau35_medium1_tracktwo_L1MU11_TAU20IM',
        'mu14_ivarloose_tau25_medium1_tracktwo',
        'mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25',
        'mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12IM_3J12',
        'mu14_ivarloose_tau25_medium1_tracktwo_xe50',
        'mu14_ivarloose_tau35_medium1_tracktwo',
        'mu14_ivarloose_tau35_medium1_tracktwo_L1MU10_TAU20IM_J25_2J20',
        'mu14_tau25_medium1_tracktwo_xe50',
        'tau160_medium1_tracktwo_L1TAU100',
        'tau200_medium1_tracktwo_L1TAU100',
        'tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',
        'tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I-J25',
        'tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM_4J12',
        'tau35_medium1_tracktwo_xe70_L1XE45',
        'tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',
        'tau35_tight1_tracktwo_tau25_tight1_tracktwo_L1DR-TAU20ITAU12I-J25',
        'tau40_medium1_tracktwo_tau35_medium1_tracktwo',
        'tau60_medium1_tracktwo_tau25_medium1_tracktwo_xe50',
        'tau80_medium1_tracktwo_L1TAU60_tau35_medium1_tracktwo_L1TAU12IM_L1TAU60_DR-TAU20ITAU12I',
        'tau80_medium1_tracktwo_L1TAU60_tau60_medium1_tracktwo_L1TAU40',
        '2tau35_medium1_tracktwo_L12TAU20IM_3J20',
        '2tau35_medium1_tracktwo',
        'tau40_medium1_tracktwo_tau25_medium1_tracktwo',
        ]

    HLTPrescales_unprescaled_only_physics_prescale = {}
    HLTPrescales_unprescaled_only_physics_prescale.update(dict(map(None,HLT_chain_list,len(HLT_chain_list)*[ [1, 0, -1] ])))



######################################################
# TIGHT physics prescales
######################################################
from copy import deepcopy
# enable if desired: # setting all L1 prescales to 1
#Prescales.L1Prescales = dict([(ctpid,1) for ctpid in Prescales.L1Prescales]) 
 
Prescales.L1Prescales_tight_physics_prescale  = deepcopy(Prescales.L1Prescales)
Prescales.HLTPrescales_tight_physics_prescale = deepcopy(Prescales.HLTPrescales)

chain_list=[
    #
    # ID cosmic
    'id_cosmicid_L1MU11_EMPTY',
    'id_cosmicid_L1MU4_EMPTY',
    #
    # minBias chains
    'mb_mbts_L1MBTS_1',
    'mb_mbts_L1MBTS_1_1',
    'mb_mbts_L1MBTS_1_1_EMPTY',
    'mb_mbts_L1MBTS_1_1_UNPAIRED_ISO',
    'mb_mbts_L1MBTS_1_EMPTY',
    'mb_mbts_L1MBTS_1_UNPAIRED_ISO',
    'mb_mbts_L1MBTS_2',
    'mb_mbts_L1MBTS_2_EMPTY',
    'mb_mbts_L1MBTS_2_UNPAIRED_ISO',
    'mb_idperf_L1MBTS_2',
    'mb_perf_L1LUCID',
    'mb_perf_L1LUCID_EMPTY',
    'mb_perf_L1LUCID_UNPAIRED_ISO',
    'mb_perf_L1MBTS_2',
    'mb_perf_L1RD1_FILLED',
    'mb_sp600_pusup300_trk40_hmt_L1TE5',
    'mb_sp700_pusup350_trk50_hmt_L1TE5',
    'mb_sp900_pusup400_trk50_hmt_L1TE5',
    'mb_sp900_pusup400_trk60_hmt_L1TE5',
    'mb_sp1000_pusup450_trk70_hmt_L1TE5',
    'mb_sp1100_pusup450_trk70_hmt_L1TE5',
    'mb_sp1200_pusup500_trk80_hmt_L1TE5',
    'mb_sp1200_pusup500_trk90_hmt_L1TE5',
    'mb_sp1200_pusup500_trk100_hmt_L1TE5',
    'mb_sp1400_pusup550_trk90_hmt_L1TE5',
    'mb_sp1600_pusup600_trk100_hmt_L1TE5',
    'mb_sp1800_pusup700_trk110_hmt_L1TE5',
    'mb_sp2100_pusup900_trk120_hmt_L1TE5',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE5',
    'mb_sp600_pusup300_trk40_hmt_L1TE10',
    'mb_sp700_pusup350_trk50_hmt_L1TE10',
    'mb_sp900_pusup400_trk60_hmt_L1TE10',
    'mb_sp1000_pusup450_trk70_hmt_L1TE10',
    'mb_sp1100_pusup450_trk70_hmt_L1TE10',
    'mb_sp1200_pusup500_trk80_hmt_L1TE10',
    'mb_sp1400_pusup550_trk90_hmt_L1TE10',
    'mb_sp1600_pusup600_trk100_hmt_L1TE10',
    'mb_sp1700_pusup650_trk110_hmt_L1TE10',
    'mb_sp1800_pusup700_trk110_hmt_L1TE10',
    'mb_sp1900_pusup700_trk120_hmt_L1TE10',
    'mb_sp2100_pusup900_trk120_hmt_L1TE10',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE10',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE10',
    'mb_sp700_pusup350_trk50_hmt_L1TE20',
    'mb_sp900_pusup400_trk60_hmt_L1TE20',
    'mb_sp1100_pusup450_trk70_hmt_L1TE20',
    'mb_sp1200_pusup500_trk80_hmt_L1TE20',
    'mb_sp1400_pusup550_trk90_hmt_L1TE20',
    'mb_sp1600_pusup600_trk100_hmt_L1TE20',
    'mb_sp1700_pusup650_trk110_hmt_L1TE20',
    'mb_sp1800_pusup700_trk110_hmt_L1TE20',
    'mb_sp1900_pusup700_trk120_hmt_L1TE20',
    'mb_sp2100_pusup750_trk130_hmt_L1TE20',
    'mb_sp2100_pusup900_trk120_hmt_L1TE20',
    'mb_sp2200_pusup800_trk140_hmt_L1TE20',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE20',
    'mb_sp2400_pusup850_trk150_hmt_L1TE20',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE20',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE20',
    'mb_sp1100_pusup450_trk70_hmt_L1TE30',
    'mb_sp1200_pusup500_trk80_hmt_L1TE30',
    'mb_sp1400_pusup550_trk90_hmt_L1TE30',
    'mb_sp1600_pusup600_trk100_hmt_L1TE30',
    'mb_sp1700_pusup650_trk110_hmt_L1TE30',
    'mb_sp1800_pusup700_trk110_hmt_L1TE30',
    'mb_sp1900_pusup700_trk120_hmt_L1TE30',
    'mb_sp2100_pusup750_trk130_hmt_L1TE30',
    'mb_sp2100_pusup900_trk120_hmt_L1TE30',
    'mb_sp2200_pusup800_trk140_hmt_L1TE30',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE30',
    'mb_sp2400_pusup850_trk150_hmt_L1TE30',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE30',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE30',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE30',
    'mb_sp1400_pusup550_trk90_hmt_L1TE40',
    'mb_sp1600_pusup600_trk100_hmt_L1TE40',
    'mb_sp1700_pusup650_trk110_hmt_L1TE40',
    'mb_sp1900_pusup700_trk120_hmt_L1TE40',
    'mb_sp2100_pusup750_trk130_hmt_L1TE40',
    'mb_sp2200_pusup800_trk140_hmt_L1TE40',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE40',
    'mb_sp2400_pusup850_trk150_hmt_L1TE40',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE40',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE40',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE40',
    'mb_sp1600_pusup600_trk100_hmt_L1TE50',
    'mb_sp1700_pusup650_trk110_hmt_L1TE50',
    'mb_sp1900_pusup700_trk120_hmt_L1TE50',
    'mb_sp2100_pusup750_trk130_hmt_L1TE50',
    'mb_sp2200_pusup800_trk140_hmt_L1TE50',
    'mb_sp2400_pusup850_trk150_hmt_L1TE50',
    #'mb_sp1700_pusup650_trk110_hmt_L1TE60',
    #'mb_sp1900_pusup700_trk120_hmt_L1TE60',
    #'mb_sp2100_pusup750_trk130_hmt_L1TE60',
    #'mb_sp2200_pusup800_trk140_hmt_L1TE60',
    #'mb_sp2400_pusup850_trk150_hmt_L1TE60',
    #'mb_sp1900_pusup700_trk120_hmt_L1TE70',
    #'mb_sp2100_pusup750_trk130_hmt_L1TE70',
    #'mb_sp2200_pusup800_trk140_hmt_L1TE70',
    #'mb_sp2400_pusup850_trk150_hmt_L1TE70',
    'mb_sp600_trk40_hmt_L1TE5',
    'mb_sp700_trk50_hmt_L1TE5',
    'mb_sp900_trk50_hmt_L1TE5',
    'mb_sp900_trk60_hmt_L1TE5',
    'mb_sp1000_trk70_hmt_L1TE5',
    'mb_sp1100_trk70_hmt_L1TE5',
    'mb_sp1200_trk80_hmt_L1TE5',
    'mb_sp1200_trk90_hmt_L1TE5',
    'mb_sp1200_trk100_hmt_L1TE5',
    'mb_sp1400_trk90_hmt_L1TE5',
    'mb_sp1600_trk100_hmt_L1TE5',
    'mb_sp1800_trk110_hmt_L1TE5',
    'mb_sp2100_trk120_hmt_L1TE5',
    'mb_sp2300_trk130_hmt_L1TE5',
    'mb_sp600_trk40_hmt_L1TE10',
    'mb_sp700_trk50_hmt_L1TE10',
    'mb_sp900_trk60_hmt_L1TE10',
    'mb_sp1000_trk70_hmt_L1TE10',
    'mb_sp1100_trk70_hmt_L1TE10',
    'mb_sp1200_trk80_hmt_L1TE10',
    'mb_sp1400_trk90_hmt_L1TE10',
    'mb_sp1600_trk100_hmt_L1TE10',
    'mb_sp1700_trk110_hmt_L1TE10',
    'mb_sp1800_trk110_hmt_L1TE10',
    'mb_sp1900_trk120_hmt_L1TE10',
    'mb_sp2100_trk120_hmt_L1TE10',
    'mb_sp2300_trk130_hmt_L1TE10',
    'mb_sp2500_trk140_hmt_L1TE10',
    'mb_sp700_trk50_hmt_L1TE20',
    'mb_sp900_trk60_hmt_L1TE20',
    'mb_sp1100_trk70_hmt_L1TE20',
    'mb_sp1200_trk80_hmt_L1TE20',
    'mb_sp1400_trk90_hmt_L1TE20',
    'mb_sp1600_trk100_hmt_L1TE20',
    'mb_sp1700_trk110_hmt_L1TE20',
    'mb_sp1800_trk110_hmt_L1TE20',
    'mb_sp1900_trk120_hmt_L1TE20',
    'mb_sp2100_trk130_hmt_L1TE20',
    'mb_sp2100_trk120_hmt_L1TE20',
    'mb_sp2200_trk140_hmt_L1TE20',
    'mb_sp2300_trk130_hmt_L1TE20',
    'mb_sp2400_trk150_hmt_L1TE20',
    'mb_sp2500_trk140_hmt_L1TE20',
    'mb_sp2700_trk150_hmt_L1TE20',
    'mb_sp1100_trk70_hmt_L1TE30',
    'mb_sp1200_trk80_hmt_L1TE30',
    'mb_sp1400_trk90_hmt_L1TE30',
    'mb_sp1600_trk100_hmt_L1TE30',
    'mb_sp1700_trk110_hmt_L1TE30',
    'mb_sp1800_trk110_hmt_L1TE30',
    'mb_sp1900_trk120_hmt_L1TE30',
    'mb_sp2100_trk130_hmt_L1TE30',
    'mb_sp2100_trk120_hmt_L1TE30',
    'mb_sp2200_trk140_hmt_L1TE30',
    'mb_sp2300_trk130_hmt_L1TE30',
    'mb_sp2400_trk150_hmt_L1TE30',
    'mb_sp2500_trk140_hmt_L1TE30',
    'mb_sp2700_trk150_hmt_L1TE30',
    'mb_sp2900_trk160_hmt_L1TE30',
    'mb_sp1400_trk90_hmt_L1TE40',
    'mb_sp1600_trk100_hmt_L1TE40',
    'mb_sp1700_trk110_hmt_L1TE40',
    'mb_sp1900_trk120_hmt_L1TE40',
    'mb_sp2100_trk130_hmt_L1TE40',
    'mb_sp2200_trk140_hmt_L1TE40',
    'mb_sp2300_trk130_hmt_L1TE40',
    'mb_sp2400_trk150_hmt_L1TE40',
    'mb_sp2500_trk140_hmt_L1TE40',
    'mb_sp2700_trk150_hmt_L1TE40',
    'mb_sp2900_trk160_hmt_L1TE40',
    'mb_sp1600_trk100_hmt_L1TE50',
    'mb_sp1700_trk110_hmt_L1TE50',
    'mb_sp1900_trk120_hmt_L1TE50',
    'mb_sp2100_trk130_hmt_L1TE50',
    'mb_sp2200_trk140_hmt_L1TE50',
    'mb_sp2400_trk150_hmt_L1TE50',
    #'mb_sp1700_trk110_hmt_L1TE60',
    #'mb_sp1900_trk120_hmt_L1TE60',
    #'mb_sp2100_trk130_hmt_L1TE60',
    #'mb_sp2200_trk140_hmt_L1TE60',
    #'mb_sp2400_trk150_hmt_L1TE60',
    #'mb_sp1900_trk120_hmt_L1TE70',
    #'mb_sp2100_trk130_hmt_L1TE70',
    #'mb_sp2200_trk140_hmt_L1TE70',
    #'mb_sp2400_trk150_hmt_L1TE70',
    'mb_sp700_trk50_hmt_L1TE15',
    'mb_sp900_trk60_hmt_L1TE15',
    'mb_sp1100_trk70_hmt_L1TE15',
    'mb_sp1200_trk80_hmt_L1TE15',
    'mb_sp1400_trk90_hmt_L1TE15',
    'mb_sp1600_trk100_hmt_L1TE15',
    'mb_sp1700_trk110_hmt_L1TE15',
    'mb_sp1900_trk120_hmt_L1TE15',
    'mb_sp1100_trk70_hmt_L1TE25',
    'mb_sp1200_trk80_hmt_L1TE25',
    'mb_sp1400_trk90_hmt_L1TE25',
    'mb_sp1600_trk100_hmt_L1TE25',
    'mb_sp1700_trk110_hmt_L1TE25',
    'mb_sp1900_trk120_hmt_L1TE25',
    'mb_sp700_pusup350_trk50_hmt_L1TE15',
    'mb_sp900_pusup400_trk60_hmt_L1TE15',
    'mb_sp1100_pusup450_trk70_hmt_L1TE15',
    'mb_sp1200_pusup500_trk80_hmt_L1TE15',
    'mb_sp1400_pusup550_trk90_hmt_L1TE15',
    'mb_sp1600_pusup600_trk100_hmt_L1TE15',
    'mb_sp1700_pusup650_trk110_hmt_L1TE15',
    'mb_sp1900_pusup700_trk120_hmt_L1TE15',
    'mb_sp1100_pusup450_trk70_hmt_L1TE25',
    'mb_sp1200_pusup500_trk80_hmt_L1TE25',
    'mb_sp1400_pusup550_trk90_hmt_L1TE25',
    'mb_sp1600_pusup600_trk100_hmt_L1TE25',
    'mb_sp1700_pusup650_trk110_hmt_L1TE25',
    'mb_sp1900_pusup700_trk120_hmt_L1TE25',
    #'mb_sp900_pusup400_trk50_hmt_L1TE5.0ETA24',
    #'mb_sp900_pusup400_trk60_hmt_L1TE5.0ETA24',
    #'mb_sp1000_pusup450_trk70_hmt_L1TE5.0ETA24',
    #'mb_sp1200_pusup500_trk80_hmt_L1TE5.0ETA24',
    #'mb_sp1200_pusup500_trk90_hmt_L1TE5.0ETA24',
    #'mb_sp1200_pusup500_trk100_hmt_L1TE5.0ETA24',
    #'mb_sp1800_pusup700_trk110_hmt_L1TE5.0ETA24',
    #'mb_sp2100_pusup900_trk120_hmt_L1TE5.0ETA24',
    #'mb_sp2300_pusup1000_trk130_hmt_L1TE5.0ETA24',
    #'mb_sp1000_pusup450_trk70_hmt_L1TE10.0ETA24',
    #'mb_sp1200_pusup500_trk80_hmt_L1TE10.0ETA24',
    #'mb_sp1400_pusup550_trk90_hmt_L1TE10.0ETA24',
    #'mb_sp1600_pusup600_trk100_hmt_L1TE10.0ETA24',
    #'mb_sp1800_pusup700_trk110_hmt_L1TE10.0ETA24',
    #'mb_sp2100_pusup900_trk120_hmt_L1TE10.0ETA24',
    #'mb_sp2300_pusup1000_trk130_hmt_L1TE10.0ETA24',
    #'mb_sp2500_pusup1100_trk140_hmt_L1TE10.0ETA24',
    #'mb_sp1200_pusup500_trk80_hmt_L1TE15.0ETA24',
    #'mb_sp1400_pusup550_trk90_hmt_L1TE15.0ETA24',
    #'mb_sp1600_pusup600_trk100_hmt_L1TE15.0ETA24',
    #'mb_sp1800_pusup700_trk110_hmt_L1TE15.0ETA24',
    #'mb_sp2100_pusup900_trk120_hmt_L1TE15.0ETA24',
    #'mb_sp2300_pusup1000_trk130_hmt_L1TE15.0ETA24',
    #'mb_sp2500_pusup1100_trk140_hmt_L1TE15.0ETA24',
    #'mb_sp1400_pusup550_trk90_hmt_L1TE20.0ETA24',
    #'mb_sp1600_pusup600_trk100_hmt_L1TE20.0ETA24',
    #'mb_sp1800_pusup700_trk110_hmt_L1TE20.0ETA24',
    #'mb_sp2100_pusup900_trk120_hmt_L1TE20.0ETA24',
    #'mb_sp2300_pusup1000_trk130_hmt_L1TE20.0ETA24',
    #'mb_sp2500_pusup1100_trk140_hmt_L1TE20.0ETA24',
    #'mb_sp2700_pusup1200_trk150_hmt_L1TE20.0ETA24',
    #'mb_sp1600_pusup600_trk100_hmt_L1TE25.0ETA24',
    #'mb_sp1800_pusup700_trk110_hmt_L1TE25.0ETA24',
    #'mb_sp2100_pusup900_trk120_hmt_L1TE25.0ETA24',
    #'mb_sp2300_pusup1000_trk130_hmt_L1TE25.0ETA24',
    #'mb_sp2500_pusup1100_trk140_hmt_L1TE25.0ETA24',
    #'mb_sp2700_pusup1200_trk150_hmt_L1TE25.0ETA24',
    #'mb_sp2900_pusup1300_trk160_hmt_L1TE25.0ETA24',
    #'mb_sp1800_pusup700_trk110_hmt_L1TE30.0ETA24',
    #'mb_sp2100_pusup900_trk120_hmt_L1TE30.0ETA24',
    #'mb_sp2300_pusup1000_trk130_hmt_L1TE30.0ETA24',
    #'mb_sp2500_pusup1100_trk140_hmt_L1TE30.0ETA24',
    #'mb_sp2700_pusup1200_trk150_hmt_L1TE30.0ETA24',
    #'mb_sp2900_pusup1300_trk160_hmt_L1TE30.0ETA24',
    #'mb_sp2300_pusup1000_trk130_hmt_L1TE40.0ETA24',
    #'mb_sp2500_pusup1100_trk140_hmt_L1TE40.0ETA24',
    #'mb_sp2700_pusup1200_trk150_hmt_L1TE40.0ETA24',
    #'mb_sp2900_pusup1300_trk160_hmt_L1TE40.0ETA24',
    'mu4_mb_sp600_trk40_hmt_L1MU4_TE10',
    'mu4_mb_sp700_trk50_hmt_L1MU4_TE10',
    'mu4_mb_sp900_trk60_hmt_L1MU4_TE10',
    'mu4_mb_sp1100_trk70_hmt_L1MU4_TE10',
    'mu4_mb_sp1200_trk80_hmt_L1MU4_TE10',
    'mu4_mb_sp1400_trk90_hmt_L1MU4_TE10',
    'mu4_mb_sp1600_trk100_hmt_L1MU4_TE10',
    'mu4_mb_sp1100_trk70_hmt_L1MU4_TE20',
    'mu4_mb_sp1200_trk80_hmt_L1MU4_TE20',
    'mu4_mb_sp1400_trk90_hmt_L1MU4_TE20',
    'mu4_mb_sp1600_trk100_hmt_L1MU4_TE20',
    'mu4_mb_sp1700_trk110_hmt_L1MU4_TE20',
    'mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE20',
    'mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE20',
    'mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE20',
    'mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE20',
    'mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE20',
    'mu4_mb_sp1100_trk70_hmt_L1MU4_TE30',
    'mu4_mb_sp1200_trk80_hmt_L1MU4_TE30',
    'mu4_mb_sp1400_trk90_hmt_L1MU4_TE30',
    'mu4_mb_sp1600_trk100_hmt_L1MU4_TE30',
    'mu4_mb_sp1700_trk110_hmt_L1MU4_TE30',
    'mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE30',
    'mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE30',
    'mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE30',
    'mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE30',
    'mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE30',
    'mu4_mb_sp600_pusup300_trk40_hmt_L1MU4_TE10',
    'mu4_mb_sp700_pusup350_trk50_hmt_L1MU4_TE10',
    'mu4_mb_sp900_pusup400_trk60_hmt_L1MU4_TE10',
    'mu4_mb_sp1100_pusup450_trk70_hmt_L1MU4_TE10',
    'mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE10',
    'mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE10',
    'mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE10',
    'mu4_mb_sp1200_trk80_hmt_L1MU4_TE40',
    'mu4_mb_sp1400_trk90_hmt_L1MU4_TE40',
    'mu4_mb_sp1600_trk100_hmt_L1MU4_TE40',
    'mu4_mb_sp1700_trk110_hmt_L1MU4_TE40',
    #'mu4_mb_sp1400_trk90_hmt_L1MU4_TE50',
    #'mu4_mb_sp1600_trk100_hmt_L1MU4_TE50',
    #'mu4_mb_sp1700_trk110_hmt_L1MU4_TE50',
    #'mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE50',
    #'mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE50',
    #'mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE50',
    'mu4_mb_sp1200_pusup500_trk80_hmt_L1MU4_TE40',
    'mu4_mb_sp1400_pusup550_trk90_hmt_L1MU4_TE40',
    'mu4_mb_sp1600_pusup600_trk100_hmt_L1MU4_TE40',
    'mu4_mb_sp1700_pusup650_trk110_hmt_L1MU4_TE40',
    'j40_mb_sp600_pusup300_trk40_hmt',
    'j40_mb_sp700_pusup350_trk50_hmt',
    'j40_mb_sp900_pusup400_trk60_hmt',
    'j40_mb_sp1100_pusup450_trk70_hmt',
    'j40_mb_sp1200_pusup500_trk80_hmt',
    'j40_mb_sp1400_pusup550_trk90_hmt',
    'j40_mb_sp1600_pusup600_trk100_hmt',
    'j40_mb_sp600_trk40_hmt',
    'j40_mb_sp700_trk50_hmt',
    'j40_mb_sp900_trk60_hmt',
    'j40_mb_sp1100_trk70_hmt',
    'j40_mb_sp1200_trk80_hmt',
    'j40_mb_sp1400_trk90_hmt',
    'j40_mb_sp1600_trk100_hmt',
    'j50_mb_sp600_pusup300_trk40_hmt',
    'j50_mb_sp700_pusup350_trk50_hmt',
    'j50_mb_sp900_pusup400_trk60_hmt',
    'j50_mb_sp1100_pusup450_trk70_hmt',
    'j50_mb_sp1200_pusup500_trk80_hmt',
    'j50_mb_sp1400_pusup550_trk90_hmt',
    'j50_mb_sp1600_pusup600_trk100_hmt',
    'j50_mb_sp600_trk40_hmt',
    'j50_mb_sp700_trk50_hmt',
    'j50_mb_sp900_trk60_hmt',
    'j50_mb_sp1100_trk70_hmt',
    'j50_mb_sp1200_trk80_hmt',
    'j50_mb_sp1400_trk90_hmt',
    'j50_mb_sp1600_trk100_hmt',
    'j60_mb_sp600_pusup300_trk40_hmt',
    'j60_mb_sp700_pusup350_trk50_hmt',
    'j60_mb_sp900_pusup400_trk60_hmt',
    'j60_mb_sp1100_pusup450_trk70_hmt',
    'j60_mb_sp1200_pusup500_trk80_hmt',
    'j60_mb_sp1400_pusup550_trk90_hmt',
    'j60_mb_sp1600_pusup600_trk100_hmt',
    'j60_mb_sp600_trk40_hmt',
    'j60_mb_sp700_trk50_hmt',
    'j60_mb_sp900_trk60_hmt',
    'j60_mb_sp1100_trk70_hmt',
    'j60_mb_sp1200_trk80_hmt',
    'j60_mb_sp1400_trk90_hmt',
    'j60_mb_sp1600_trk100_hmt',
    'j110_mb_sp600_pusup300_trk40_hmt',
    'j110_mb_sp700_pusup350_trk50_hmt',
    'j110_mb_sp900_pusup400_trk60_hmt',
    'j110_mb_sp1100_pusup450_trk70_hmt',
    'j110_mb_sp1200_pusup500_trk80_hmt',
    'j110_mb_sp1400_pusup550_trk90_hmt',
    'j110_mb_sp1600_pusup600_trk100_hmt',
    'j110_mb_sp600_trk40_hmt',
    'j110_mb_sp700_trk50_hmt',
    'j110_mb_sp900_trk60_hmt',
    'j110_mb_sp1100_trk70_hmt',
    'j110_mb_sp1200_trk80_hmt',
    'j110_mb_sp1400_trk90_hmt',
    'j110_mb_sp1600_trk100_hmt',
    'j75_mb_sp600_pusup300_trk40_hmt',
    'j75_mb_sp700_pusup350_trk50_hmt',
    'j75_mb_sp900_pusup400_trk60_hmt',
    'j75_mb_sp1100_pusup450_trk70_hmt',
    'j75_mb_sp1200_pusup500_trk80_hmt',
    'j75_mb_sp1400_pusup550_trk90_hmt',
    'j75_mb_sp1600_pusup600_trk100_hmt',
    'j75_mb_sp600_trk40_hmt',
    'j75_mb_sp700_trk50_hmt',
    'j75_mb_sp900_trk60_hmt',
    'j75_mb_sp1100_trk70_hmt',
    'j75_mb_sp1200_trk80_hmt',
    'j75_mb_sp1400_trk90_hmt',
    'j75_mb_sp1600_trk100_hmt',
    'j85_mb_sp600_pusup300_trk40_hmt',
    'j85_mb_sp700_pusup350_trk50_hmt',
    'j85_mb_sp900_pusup400_trk60_hmt',
    'j85_mb_sp1100_pusup450_trk70_hmt',
    'j85_mb_sp1200_pusup500_trk80_hmt',
    'j85_mb_sp1400_pusup550_trk90_hmt',
    'j85_mb_sp1600_pusup600_trk100_hmt',
    'j85_mb_sp600_trk40_hmt',
    'j85_mb_sp700_trk50_hmt',
    'j85_mb_sp900_trk60_hmt',
    'j85_mb_sp1100_trk70_hmt',
    'j85_mb_sp1200_trk80_hmt',
    'j85_mb_sp1400_trk90_hmt',
    'j85_mb_sp1600_trk100_hmt',
    'mb_sp400_trk40_hmt_L1MBTS_1_1',
    'mb_sp600_trk45_hmt_L1MBTS_1_1',
    'mb_sp700_trk55_hmt_L1MBTS_1_1',
    'mb_sp900_trk60_hmt_L1MBTS_1_1',
    'mb_sp900_trk65_hmt_L1MBTS_1_1',
    'mb_sp1000_trk70_hmt_L1MBTS_1_1',
    'mb_sp1200_trk75_hmt_L1MBTS_1_1',
    'mb_sp1400_trk80_hmt_L1MBTS_1_1',
    #'mb_sp900_trk50_hmt_L1TE5.0ETA24',
    #'mb_sp900_trk60_hmt_L1TE5.0ETA24',
    #'mb_sp1000_trk70_hmt_L1TE5.0ETA24',
    #'mb_sp1200_trk80_hmt_L1TE5.0ETA24',
    #'mb_sp1200_trk90_hmt_L1TE5.0ETA24',
    #'mb_sp1200_trk100_hmt_L1TE5.0ETA24',
    #'mb_sp1800_trk110_hmt_L1TE5.0ETA24',
    #'mb_sp2100_trk120_hmt_L1TE5.0ETA24',
    #'mb_sp2300_trk130_hmt_L1TE5.0ETA24',
    #'mb_sp1000_trk70_hmt_L1TE10.0ETA24',
    #'mb_sp1200_trk80_hmt_L1TE10.0ETA24',
    #'mb_sp1400_trk90_hmt_L1TE10.0ETA24',
    #'mb_sp1600_trk100_hmt_L1TE10.0ETA24',
    #'mb_sp1800_trk110_hmt_L1TE10.0ETA24',
    #'mb_sp2100_trk120_hmt_L1TE10.0ETA24',
    #'mb_sp2300_trk130_hmt_L1TE10.0ETA24',
    #'mb_sp2500_trk140_hmt_L1TE10.0ETA24',
    #'mb_sp1200_trk80_hmt_L1TE15.0ETA24',
    #'mb_sp1400_trk90_hmt_L1TE15.0ETA24',
    #'mb_sp1600_trk100_hmt_L1TE15.0ETA24',
    #'mb_sp1800_trk110_hmt_L1TE15.0ETA24',
    #'mb_sp2100_trk120_hmt_L1TE15.0ETA24',
    #'mb_sp2300_trk130_hmt_L1TE15.0ETA24',
    #'mb_sp2500_trk140_hmt_L1TE15.0ETA24',
    #'mb_sp1400_trk90_hmt_L1TE20.0ETA24',
    #'mb_sp1600_trk100_hmt_L1TE20.0ETA24',
    #'mb_sp1800_trk110_hmt_L1TE20.0ETA24',
    #'mb_sp2100_trk120_hmt_L1TE20.0ETA24',
    #'mb_sp2300_trk130_hmt_L1TE20.0ETA24',
    #'mb_sp2500_trk140_hmt_L1TE20.0ETA24',
    #'mb_sp2700_trk150_hmt_L1TE20.0ETA24',
    #'mb_sp1600_trk100_hmt_L1TE25.0ETA24',
    #'mb_sp1800_trk110_hmt_L1TE25.0ETA24',
    #'mb_sp2100_trk120_hmt_L1TE25.0ETA24',
    #'mb_sp2300_trk130_hmt_L1TE25.0ETA24',
    #'mb_sp2500_trk140_hmt_L1TE25.0ETA24',
    #'mb_sp2700_trk150_hmt_L1TE25.0ETA24',
    #'mb_sp2900_trk160_hmt_L1TE25.0ETA24',
    #'mb_sp1800_trk110_hmt_L1TE30.0ETA24',
    #'mb_sp2100_trk120_hmt_L1TE30.0ETA24',
    #'mb_sp2300_trk130_hmt_L1TE30.0ETA24',
    #'mb_sp2500_trk140_hmt_L1TE30.0ETA24',
    #'mb_sp2700_trk150_hmt_L1TE30.0ETA24',
    #'mb_sp2900_trk160_hmt_L1TE30.0ETA24',
    #'mb_sp2300_trk130_hmt_L1TE40.0ETA24',
    #'mb_sp2500_trk140_hmt_L1TE40.0ETA24',
    #'mb_sp2700_trk150_hmt_L1TE40.0ETA24',
    #'mb_sp2900_trk160_hmt_L1TE40.0ETA24',
    'mb_sp800_hmtperf_L1TE5',
    'mb_sp800_hmtperf_L1TE10',
    'mb_sp1200_hmtperf_L1TE10',
    'mb_sp1600_hmtperf_L1TE20',
    'mb_sp2100_hmtperf_L1TE20',
    'mb_sp2100_hmtperf_L1TE30',
    'mb_sp2100_hmtperf_L1TE40',
    #'mb_sp800_hmtperf_L1TE5.0ETA24',
    #'mb_sp800_hmtperf_L1TE10.0ETA24',
    #'mb_sp1200_hmtperf_L1TE10.0ETA24',
    #'mb_sp1200_hmtperf_L1TE15.0ETA24',
    #'mb_sp1600_hmtperf_L1TE15.0ETA24',
    #'mb_sp1600_hmtperf_L1TE20.0ETA24',
    #'mb_sp2100_hmtperf_L1TE20.0ETA24',
    #'mb_sp2100_hmtperf_L1TE25.0ETA24',
    #'mb_sp2100_hmtperf_L1TE30.0ETA24',
    #'mb_sp2100_hmtperf_L1TE40.0ETA24',
    'mb_sp2_hmtperf_L1MBTS_1_1',
    'mb_sp2_hmtperf',
    'mb_sp500_hmtperf',
    'mb_sp1800_hmtperf',
    'mb_sptrk',
    'mb_sptrk_L1RD0_EMPTY',
    'mb_sptrk_L1RD0_UNPAIRED_ISO',
    'mb_sp_L1RD0_UNPAIRED_ISO',
    'mb_sp_ncb_L1RD0_UNPAIRED_ISO',
    'mb_sp_blayer_L1RD0_UNPAIRED_ISO',
    'mb_sptrk_L1RD3_FILLED',
    'mb_sptrk_costr',
    'mb_sptrk_costr_L1RD0_EMPTY',
    'mb_sptrk_noisesup',
    'mb_sptrk_noisesup_L1RD0_EMPTY',
    'mb_sptrk_noisesup_L1RD0_UNPAIRED_ISO',
    'mb_sptrk_noisesup_L1RD3_FILLED',
    'mb_sptrk_pt4_L1MBTS_1',
    'mb_sptrk_pt4_L1MBTS_1_1',
    'mb_sptrk_pt4_L1MBTS_2',
    'mb_sptrk_pt4_L1RD3_FILLED',
    'mb_sptrk_pt6_L1MBTS_1',
    'mb_sptrk_pt6_L1MBTS_1_1',
    'mb_sptrk_pt6_L1MBTS_2',
    'mb_sptrk_pt6_L1RD3_FILLED',
    'mb_sptrk_pt8_L1MBTS_1',
    'mb_sptrk_pt8_L1MBTS_1_1',
    'mb_sptrk_pt8_L1MBTS_2',
    'mb_sptrk_pt8_L1RD3_FILLED',
    'mb_sptrk_vetombts2in_L1ZDC_AND',
    'mb_sptrk_vetombts2in_L1ZDC_A_C',
    'mb_sptrk_trk80_L1MBTS_2',
    'mb_sptrk_pt2_L1MBTS_2',
#    'mb_sptrk_L1MBTS_1',
##     'mb_sptrk_L1MBTS_1_VTE2',
##     'mb_sptrk_L1MBTS_1_VTE3',
##     'mb_sptrk_L1MBTS_1_VTE4',
##     'mb_sptrk_L1MBTS_1_VTE5',
##     'mb_sptrk_L1MBTS_1_VTE10',
##     'mb_sptrk_L1MBTS_2_VTE2',
##     'mb_sptrk_L1MBTS_2_VTE3',
##     'mb_sptrk_L1MBTS_2_VTE4',
##     'mb_sptrk_L1MBTS_2_VTE5',
##     'mb_sptrk_L1MBTS_2_VTE10',
##     'mb_sptrk_L1VTE2',
##     'mb_sptrk_L1VTE3',
##     'mb_sptrk_L1VTE4',
##     'mb_sptrk_L1VTE5',
##     'mb_sptrk_L1VTE10',   
    
    # minBias streamer
    'noalg_mb_L1LUCID',
    'noalg_mb_L1LUCID_EMPTY',
    'noalg_mb_L1LUCID_UNPAIRED_ISO',
    'noalg_mb_L1MBTS_1',
    'noalg_mb_L1MBTS_1_1',
    'noalg_mb_L1MBTS_1_1_EMPTY',
    'noalg_mb_L1MBTS_1_1_UNPAIRED_ISO',
    'noalg_mb_L1MBTS_1_EMPTY',
    'noalg_mb_L1MBTS_1_UNPAIRED_ISO',
    'noalg_mb_L1MBTS_2',
    'noalg_mb_L1MBTS_2_EMPTY',
    'noalg_mb_L1MBTS_2_UNPAIRED_ISO',
    'noalg_mb_L1RD0_EMPTY',
    'noalg_mb_L1RD0_FILLED',
    'noalg_mb_L1RD0_UNPAIRED_ISO',
    'noalg_mb_L1RD1_FILLED',
    'noalg_mb_L1RD2_EMPTY',
    'noalg_mb_L1RD2_FILLED',
    'noalg_mb_L1RD3_EMPTY',
    'noalg_mb_L1RD3_FILLED',
 ##    'noalg_mb_L1TE2',
##     'noalg_mb_L1TE3',
##     'noalg_mb_L1TE4',
    'noalg_mb_L1TE5',
    #'noalg_mb_L1TE5.0ETA24',
    'noalg_mb_L1TE10',
    #'noalg_mb_L1TE10.0ETA24',
    'noalg_mb_L1TE20',
    #'noalg_mb_L1TE20.0ETA24',
    'noalg_mb_L1TE30',
    #'noalg_mb_L1TE30.0ETA24',
    'noalg_mb_L1TE40',
    #'noalg_mb_L1TE40.0ETA24',
    'noalg_mb_L1TE50',
    #'noalg_mb_L1TE60',
    #'noalg_mb_L1TE70',
    'noalg_mb_L1TE15',
    'noalg_mb_L1TE25',
    #'noalg_mb_L1TE70.0ETA24',
    'noalg_mb_L1ZDC_A',
    'noalg_mb_L1ZDC_C',
    'noalg_mb_L1ZDC_A_C',
    'noalg_mb_L1ZDC_AND',
    'zdcpeb_L1ZDC_A',  
    'zdcpeb_L1ZDC_C',   
    'zdcpeb_L1ZDC_AND', 
    'zdcpeb_L1ZDC_A_C',
    'timeburner',
#    'mb_sp_L1MBTS_1_OVERLAY',
#    'mb_sptrk_L1MBTS_1_OVERLAY',
#    'noalg_L1MBTS_1_OVERLAY',
#    'noalg_L1TE20_OVERLAY',
    ]
Prescales.HLTPrescales_tight_physics_prescale.update(dict(map(None,chain_list,len(chain_list)*[ [-1, 0,-1] ])))
######################################################
