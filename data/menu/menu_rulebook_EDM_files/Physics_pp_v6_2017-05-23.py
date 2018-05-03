# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration

#----------phy--------------------------------------------------------------#
#------------------------------------------------------------------------#
def setupMenu():

    from TriggerJobOpts.TriggerFlags          import TriggerFlags
    from AthenaCommon.Logging                 import logging
    log = logging.getLogger( 'TriggerMenu.menu.Physics_pp_v6.py' )

    from TriggerMenu.TriggerConfigLVL1 import TriggerConfigLVL1 as tcl1
    if tcl1.current:
        log.info("L1 items: %s " % tcl1.current.menu.items.itemNames())
    else:
        log.info("ERROR L1 menu has not yet been defined")

    PhysicsStream="Main"

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


    TriggerFlags.MuonSlice.signatures = [
        # single muon

        ['mu24_ivarloose', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24_iloose', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24_ivarmedium',	      'L1_MU20',	   [], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        # ATR-14489: add express: ['mu26_ivarmedium',	     'L1_MU20', 	  [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu26_ivarmedium',	     'L1_MU20', 	  [], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu28_ivarmedium',	     'L1_MU20', 	  [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu35_ivarmedium',	     'L1_MU20', 	  [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],

        ['mu0_perf',              'L1_MU4',            [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1], 
        # ATR-14489: add express: ['mu26_imedium',          'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu26_imedium',          'L1_MU20',           [], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu28_imedium',          'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu100',                 'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu80',                  'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu50',                  'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu60',                  'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu60_0eta105_msonly',   'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu40',                   'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24_imedium',           'L1_MU20',           [], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_imedium_L1MU10',    'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_ivarmedium_L1MU10', 'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu26',                   'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24_L1MU15',            'L1_MU15',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu22',                   'L1_MU20',           [], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20',                   'L1_MU20',           [], [PhysicsStream, 'express'], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_L1MU15',            'L1_MU15',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24',                   'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_msonly',            'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu18',                   'L1_MU15',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu14',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu11',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu14_iloose',            'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu14_ivarloose',         'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_msonly',            'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu6',                    'L1_MU6',            [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu6_msonly',             'L1_MU6',            [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu4',                    'L1_MU4',            [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu4_msonly',             'L1_MU4',            [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu24_idperf',            'L1_MU20',           [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu20_idperf',            'L1_MU20',           [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu10_idperf',            'L1_MU10',           [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu6_idperf',             'L1_MU6',            [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],
        ['mu4_idperf',             'L1_MU4',            [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Muon', 'BW:ID'], -1],

        ['mu4_nomucomb',                   'L1_MU4',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu6_nomucomb',                   'L1_MU6',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_nomucomb',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu11_nomucomb',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu14_nomucomb',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_nomucomb',                   'L1_MU10',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],

				# Toroid-off run (ATR-9923)
        ['mu8_mucombTag_noEF_L1MU40',  'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu10_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu15_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu20_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu25_mucombTag_noEF_L1MU40', 'L1_MU20', [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],

        # multi muons
        ['2mu14',                  'L1_2MU10',          [], [PhysicsStream, 'express'], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu15_L12MU10',          'L1_2MU10',          [], [PhysicsStream, 'express'], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu16_L12MU10',          'L1_2MU10',          [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu18_L12MU10',          'L1_2MU10',          [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu14_nomucomb',         'L1_2MU10',          [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6',                   'L1_3MU6',           [], [PhysicsStream, 'express'], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6_msonly',            'L1_3MU6',           [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu8_msonly',            'L1_3MU6',           [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu10_msonly_L13MU6',    'L1_3MU6',           [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu10',                  'L1_2MU10',          [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu10_nomucomb',         'L1_2MU10',          [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['mu24_mu8noL1',           'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','mu8noL1']]],
        ['mu24_mu10noL1',          'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','mu10noL1']]],
        ['mu24_2mu4noL1',          'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu24','2mu4noL1']]],
        ['mu26_mu8noL1',           'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu26','mu8noL1']]],
        ['mu26_mu10noL1',          'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu26','mu10noL1']]],
        ['mu28_mu8noL1',           'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu28','mu8noL1']]],
        ['mu22_mu8noL1',           'L1_MU20', ['L1_MU20',''], [PhysicsStream, 'express'], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu22','mu8noL1']]],
        ['mu22_2mu4noL1',          'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu22','2mu4noL1']]],
        ['mu20_mu8noL1',           'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20','mu8noL1']]],
        ['mu20_2mu4noL1',          'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20','2mu4noL1']]],
        ['2mu6',                    'L1_2MU6',  [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['2mu4',                    'L1_2MU4',  [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['mu20_2mu0noL1_JpsimumuFS', 'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20','2mu0noL1_JpsimumuFS']]],
        ['mu20_2mu4_JpsimumuL2',     'L1_MU20', ['L1_MU20','L1_2MU4'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['2mu4_JpsimumuL2','mu20']]],
        # Primary (multi muon chains)
        ['3mu4',                   'L1_3MU4',           [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],

        #DiMuon for TAU OVERLAY
        ['2mu20_L12MU20_OVERLAY',   'L1_2MU20_OVERLAY', [], ['TauOverlay'], ["RATE:TauOverlay",  "BW:TauOverlay"], -1],
        
        ## ADD MU*_EMPTY TO L1 MENU
        ['mu4_cosmic_L1MU4_EMPTY',               'L1_MU4_EMPTY', [],   ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],
        ['mu4_cosmic_L1MU11_EMPTY',              'L1_MU11_EMPTY', [],  ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],
        ['mu4_msonly_cosmic_L1MU4_EMPTY',        'L1_MU4_EMPTY', [],   ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],
        ['mu4_msonly_cosmic_L1MU11_EMPTY',       'L1_MU11_EMPTY', [],  ['CosmicMuons'], ["RATE:Cosmic_Muon", "BW:Muon"], -1],                            
                
        ##Streaming name?
        #['mu4_cosmicEF_ds1_L1MU4', 'L1_MU4', [], ['DataScouting_01_CosmicMuons'], ["RATE:Cosmic_Muon_DS", "BW:Muon"], -1],
        #['mu4_cosmicEF_ds2_L1MU4', 'L1_MU4', [], ['DataScouting_01_CosmicMuons','DataScouting_02_CosmicMuons'], ["RATE:Cosmic_Muon_DS", "BW:Muon"], -1],

        # muon calibration
        ['mu0_muoncalib',             'L1_MU20',     [],['Muon_Calibration'],["RATE:Calibration","BW:Muon"],-1],
        ['mu0_muoncalib_L1MU15',      'L1_MU15',     [],['Muon_Calibration'],["RATE:Calibration","BW:Muon"],-1],
        ['mu0_muoncalib_L1MU4_EMPTY', 'L1_MU4_EMPTY',[],['Muon_Calibration'],["RATE:Calibration","BW:Muon"],-1],

        # muon calibration, data scouting
        ['mu0_muoncalib_ds3',             'L1_MU20',     [],['DataScouting_03_CosmicMuons'],["RATE:Calibration","BW:Muon"],-1],        
        ['mu0_muoncalib_ds3_L1MU15',      'L1_MU15',     [],['DataScouting_03_CosmicMuons'],["RATE:Calibration","BW:Muon"],-1],
        ['mu0_muoncalib_ds3_L1MU4_EMPTY', 'L1_MU4_EMPTY',[],['DataScouting_03_CosmicMuons'],["RATE:Calibration","BW:Muon"],-1],   

        # Narrow scan
        # notes: see ATR-11846
        # inputTE for narrow scan sequence (2nd inputTE) is a dummy string for MuonDef.py
        # To indicate narrow scan uses lastTE of previous sequence and also to avoid duplicated sequences with same TE name, write L2_ TE name of the 1st sequence.

        ['mu20_nomucomb_mu6noL1_nscan03',                     'L1_MU20',      ['L1_MU20','L2_mu20_nomucomb'],        [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_nomucomb','mu6noL1_nscan03']]],

        ['mu11_nomucomb_2mu4noL1_nscan03_L1MU11_2MU6', 'L1_MU11_2MU6', ['L1_MU11','L2_mu11_nomucomb'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu11_nomucomb','2mu4noL1_nscan03']]],

        ## LFV topo based nscan (ATR-14352)
        ['mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU', 'L1_LFV-MU', ['L1_MU10','L2_mu11_nomucomb'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu11_nomucomb','2mu4noL1_nscan03']]],
	# -----------------------------------------------------

        # LLP chains
        # notes: see ATR-11482
        # L1 is seeded from single EMPTY/UNPAIRED_ISO, but, at HLT it does a same sequence as main ones (e.g. MU4_EMPTY single seeded, but 3MU6 is required at HLT)
        # this is done on purpose (ATR-11482)
        ['3mu6_msonly_L1MU4_EMPTY',                              'L1_MU4_EMPTY',        ['L1_3MU6'], ["Late"], ['RATE:MultiMuon', 'BW:Muon'], -1],
       ['3mu6_msonly_L1MU6_EMPTY',                              'L1_MU6_EMPTY',        ['L1_3MU6'], ["Late"], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6_msonly_L1MU4_UNPAIRED_ISO',                       'L1_MU4_UNPAIRED_ISO', ['L1_3MU6'], ["Late"], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['3mu6_msonly_L1MU4_UNPAIRED_NONISO',                     'L1_MU4_UNPAIRED_NONISO', ['L1_3MU6'], ["Late"], ['RATE:MultiMuon', 'BW:Muon'], -1],


        ['mu20_msonly_mu10noL1_msonly_nscan05_noComb',            'L1_MU20',   ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu10noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu10noL1_msonly_nscan03_noComb',            'L1_MU20',   ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu10noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU6_EMPTY',       'L1_MU6_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu10noL1_msonly_nscan03_noComb_L1MU6_EMPTY',       'L1_MU6_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO','L1_MU4_UNPAIRED_ISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu10noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO','L1_MU4_UNPAIRED_ISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu10noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_NONISO','L1_MU4_UNPAIRED_NONISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],
        #['mu20_msonly_mu10noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','L1_MU4_UNPAIRED_NONISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],


         ### ATR-14377: Emergency additional class of chains instead of mu20_msonly_mu10noL1_msonly_nscan05_noComb*
         ['mu20_msonly_mu15noL1_msonly_nscan05_noComb',            'L1_MU20',   ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu15noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU6_EMPTY',       'L1_MU6_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO','L1_MU4_UNPAIRED_ISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_NONISO','L1_MU4_UNPAIRED_NONISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],

        ### ATR-14377:
        ['mu20_msonly_mu15noL1_msonly_nscan03_noComb',            'L1_MU20',   ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu15noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU6_EMPTY',       'L1_MU6_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO','L1_MU4_UNPAIRED_ISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu15noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','L1_MU4_UNPAIRED_NONISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],

        ['mu20_msonly_mu12noL1_msonly_nscan03_noComb',            'L1_MU20',   ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu12noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU6_EMPTY',       'L1_MU6_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_ISO','L1_MU4_UNPAIRED_ISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],
        ['mu20_msonly_mu12noL1_msonly_nscan03_noComb_L1MU4_UNPAIRED_NONISO','L1_MU4_UNPAIRED_NONISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan03_noComb']]],        



        # exclusive di-lep
        ['2mu6_10invm30_pt2_z10', 'L1_2MU6', [], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],

    
        #new narrow-scan muon triggers for tau->3mu
        ['mu6_2mu4', 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], [PhysicsStream], ['RATE:MultiMuon', 'BW:Muon'], -1],
        ['mu20_imedium_L1MU10_2J20', 'L1_MU10_2J20', [], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon','BW:Jet'], -1],                
        ['mu20_ivarmedium_L1MU10_2J20', 'L1_MU10_2J20', [], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon','BW:Jet'], -1],                
        ['mu20_ivarmedium_L1MU10_2J15_J20', 'L1_MU10_2J15_J20', [], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon','BW:Jet'], -1],  

        #nscan05 triggers rejecting CB muons

        ['mu20_msonly_mu6noL1_msonly_nscan05',                 'L1_MU20', ['L1_MU20','L2_mu20_msonly'], [PhysicsStream], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05']]],

        ['mu20_msonly_mu6noL1_msonly_nscan05_L1MU4_EMPTY',       'L1_MU4_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05']]],
        ['mu20_msonly_mu6noL1_msonly_nscan05_noComb_L1MU4_EMPTY',       'L1_MU4_EMPTY',        ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],
        ['mu20_msonly_mu6noL1_msonly_nscan05_noComb_L1MU4_UNPAIRED_ISO','L1_MU4_UNPAIRED_ISO', ['L1_MU20','L2_mu20_msonly'], ["Late"], ['RATE:MultiMuon','BW:Muon'], -1,['serial',-1,['mu20_msonly','mu6noL1_msonly_nscan05_noComb']]],

        
        #Chains for testing 2e34 menus
        ['mu32_ivarmedium',          'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu36_ivarmedium',          'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu40_ivarmedium',          'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        ['mu50_ivarmedium',          'L1_MU20',           [], [PhysicsStream], ['RATE:SingleMuon', 'BW:Muon'], -1],
        

				#Delayed Bphys item, needs to be in muon slice: ATR-14367
        ['3mu4_nomucomb_delayed',                                  'L1_3MU4', [], ['BphysDelayed'], ['RATE:BphysDelayed', 'BW:Bphys'], -1],
        ['mu6_nomucomb_2mu4_nomucomb_delayed_L1MU6_3MU4',          'L1_MU6_3MU4',  ['L1_MU6','L1_2MU4'],          ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],

        #AFP muons
        ['mu4_L1AFP_C_MU4',                        'L1_AFP_C_MU4',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['mu6_L1AFP_C_MU4',                        'L1_AFP_C_MU4',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],


			 ]
	
    TriggerFlags.JetSlice.signatures = [   
       
        ['j0_perf_ds1_L1J75',      'L1_J75',  [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],
        ['j0_perf_ds1_L1J100',     'L1_J100', [], ['DataScouting_05_Jets'], ['RATE:Jets_DS', 'BW:Jet'], -1],

        # Performance chains
        ['j0_L1J12_EMPTY',         'L1_J12_EMPTY', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['ht0_L1J12_EMPTY',        'L1_J12_EMPTY', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j0_perf_L1RD0_FILLED',   'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j15_320eta490', 'L1_RD0_FILLED',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j25_320eta490', 'L1_RD0_FILLED',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j35_320eta490', 'L1_RD0_FILLED',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j45_320eta490', 'L1_J15.31ETA49', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j55_320eta490', 'L1_J15.31ETA49', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j15_j15_320eta490',              'L1_RD0_FILLED',      [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j25_j25_320eta490',              'L1_RD0_FILLED',      [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j45_j45_320eta490',              'L1_RD0_FILLED',      [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
#        ['j55_j55_320eta490',              'L1_J15_J15.31ETA49', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j60_j60_320eta490',              'L1_J20_J20.31ETA49', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        # forward jets
        ['j85_280eta320_nojcalib',         'L1_J20.28ETA31', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_280eta320_lcw_nojcalib',     'L1_J20.28ETA31', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['3j175',            'L1_J100', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['3j200',            'L1_J100', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['3j220',            'L1_J100', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        ['4j100',                   'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j110',                   'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j120',                   'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j130',                   'L1_3J50', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j25',      'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j45',                    'L1_3J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j85',                    'L1_3J40', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j85_jes',                'L1_3J40', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j85_lcw',                'L1_3J40', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j85_lcw_jes',            'L1_3J40', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j85_lcw_nojcalib',       'L1_3J40', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['4j85_nojcalib',           'L1_3J40', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        ['5j25',      'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j45',                    'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j55',                    'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60',                    'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j60_L14J15',             'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j65_0eta240',            'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_0eta240',            'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70',                    'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85',                    'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_jes',                'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_lcw',                'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_lcw_jes',            'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_nojcalib',            'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_lcw_nojcalib',        'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_0eta240',            'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j100',                    'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
 
        ['6j25',      'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],        
        ['6j45',                          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_0eta240',                  'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_0eta240_L14J20',           'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j45_0eta240_L15J150ETA25',     'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_0eta240_L14J20',           'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_0eta240_L15J150ETA25',     'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j55_0eta240_L14J20',           'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j55_0eta240_L15J150ETA25',     'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60',                          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j70',                          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j85',                          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        ['5j65_0eta240_L14J15',          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_0eta240_L14J15',          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j55_0eta240_L14J15',          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j60_L14J15',                  'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j70_L14J15',                  'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        ['5j70_jes_L14J15',              'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_lcw_L14J15',              'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_lcw_jes_L14J15',          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_lcw_nojcalib_L14J15',     'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j70_nojcalib_L14J15',         'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_jes_L14J15',              'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_lcw_L14J15',              'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_lcw_jes_L14J15',          'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_lcw_nojcalib_L14J15',     'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_nojcalib_L14J15',         'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],

        ['j400_lcw_nojcalib',            'L1_J100', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        

        ['7j25',      'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],      
        ['7j45',                          'L1_6J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_0eta240_L14J20',           'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_0eta240_L15J150ETA25',     'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_L14J20',                   'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['7j45_L14J15',                   'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['7j50',                          'L1_6J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['10j40_L14J20', 'L1_4J20' , [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['10j40_L14J15', 'L1_4J15' , [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        ['ht1000_L1J100',    'L1_J100', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1200_L1J100',    'L1_J100', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht1400_L1J100',    'L1_J100', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        ['5j70_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['5j85_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['6j50_0eta240_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiJet',  'BW:Jet'], -1],
        ['j110',                   'L1_J30',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j110_320eta490',         'L1_J30.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j15',                    'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j150',                   'L1_J40',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j175',                   'L1_J50',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j175_320eta490',           'L1_J50.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j25',            'L1_RD0_FILLED', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j260',                   'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_320eta490',           'L1_J75.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j320',                   'L1_J85', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j35',            'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_jes',        'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw',        'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw_jes',    'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_lcw_nojcalib',     'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j35_nojcalib',         'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j45',            'L1_J15', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j340',                   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j360',                   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j360_320eta490',           'L1_J100.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j360_320eta490_jes',       'L1_J100.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j360_320eta490_lcw',       'L1_J100.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j360_320eta490_lcw_jes',    'L1_J100.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j360_320eta490_lcw_nojcalib',    'L1_J100.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j360_320eta490_nojcalib',        'L1_J100.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j380',                   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400',                   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420',                   'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j440',                   'L1_J120', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j45_L1RD0_FILLED',      'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j260_a10_nojcalib_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_sub_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_lcw_sub_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_lcw_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j260_a10_lcw_nojcalib_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
# Not yet in L1 menu       ['j260_a10_lcw_L1SC85', 'L1_SC85', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
# Not yet in L1 menu       ['j260_a10_lcw_nojcalib_L1SC85', 'L1_SC85', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
# Not yet in L1 menu       ['j260_a10_lcw_sub_L1SC85', 'L1_SC85', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],  

        ['j300_a10_lcw_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
# Not yet in L1 menu        ['j300_a10_lcw_L1SC85', 'L1_SC85', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j260_a10r_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j300_a10r_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        
        ['j460',                   'L1_J120', [], [PhysicsStream, 'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10_sub_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10r_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10r_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10_nojcalib_L1J100'                   ,  'L1_J100',    [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j460_a10_lcw_nojcalib_L1J100'               ,  'L1_J100',    [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j460_a10_lcw_sub_L1J100'               , 'L1_J100',    [], [PhysicsStream,'express'], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['ht850_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht700_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1], 

        ['ht850_L1J100',     'L1_J100', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['ht700_L1J100',     'L1_J100', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j360_a10r_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j55',                    'L1_J15',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j60',                    'L1_J20',  [], [PhysicsStream, 'express'], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j60_280eta320',          'L1_J20.28ETA31', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j60_320eta490',          'L1_J20.31ETA49', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j60_L1RD0_FILLED',       'L1_RD0_FILLED', [], [PhysicsStream, 'express'], ['RATE:SingleJet',  'BW:Jet'], -1],

        ['j85',                    'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j85_280eta320',           'L1_J20.28ETA31', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_280eta320_jes',       'L1_J20.28ETA31', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_280eta320_lcw',       'L1_J20.28ETA31', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_280eta320_lcw_jes',    'L1_J20.28ETA31', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_320eta490',           'L1_J20.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_L1RD0_FILLED',      'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j85_jes',                'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j360_a10_lcw_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j380_a10_lcw_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400_a10_lcw_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10_lcw_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10_lcw_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10_lcw_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j480_a10_lcw_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400_a10r_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_a10r_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j440_a10r_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j380_a10r_L1J100', 'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j85_cleanL',                    'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j85_cleanT',                    'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],


        #multi-jet
#        ['5j65_0eta240', 'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
#        ['5j70_0eta240', 'L1_4J20', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        #Forward jets
        #['j60_240eta490',          'L1_J15.23ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],

        # Test chains, to be removed
        ['j85_cleanLLP',                    'L1_J20',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j0_1i2c300m500TLA',             'L1_J100',  [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j0_0i1c500m900TLA',             'L1_J100',  [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j0_1i2c200m8000TLA',             'L1_J100',  [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        ['10j40_L16J15', 'L1_6J15' , [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        
        ['j80_0eta240_2j60_320eta490', 'L1_J40.0ETA25_2J15.31ETA49', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        
				#Alternative calibrations for single jet: ATR:-13369
        ['j380_jes',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j380_lcw',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j380_lcw_jes',           'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j380_lcw_nojcalib',      'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j380_nojcalib',          'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400_jes',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400_lcw',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400_lcw_jes',           'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400_nojcalib',          'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j400_sub',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_jes',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_lcw',               'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_lcw_jes',           'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_lcw_nojcalib',      'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j420_nojcalib',          'L1_J100', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j175_320eta490_jes',       'L1_J50.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j175_320eta490_lcw',       'L1_J50.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j175_320eta490_lcw_jes',    'L1_J50.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j175_320eta490_lcw_nojcalib',    'L1_J50.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j175_320eta490_nojcalib',        'L1_J50.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_jes',       'L1_J75.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw',       'L1_J75.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw_jes',    'L1_J75.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_lcw_nojcalib',    'L1_J75.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j260_320eta490_nojcalib',        'L1_J75.31ETA49', [], [PhysicsStream], ['RATE:SingleJet',  'BW:Jet'], -1],
        ['j300_a10_sub_L1J75', 'L1_J75', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
 
        # VBF triggers
        ['2j40_0eta490_invm250_L1XE55', 'L1_XE55',         [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['j80_0eta240_j60_j45_320eta490', 'L1_J40.0ETA25_2J25_J20.31ETA49', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],


        ['2j40_0eta490_invm400', 'L1_MJJ-400', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

				# Delayed stream: ATR-14091
        ['j0_0i1c500m900TLA_delayed',              'L1_J100',  [], ['ExoDelayed'], ['RATE:ExoDelayed', 'BW:Jet'], -1],
        ['j0_1i2c200m8000TLA_delayed',             'L1_J100',  [], ['ExoDelayed'], ['RATE:ExoDelayed', 'BW:Jet'], -1],
        ['j0_0i1c500m2000TLA_delayed',             'L1_J100',  [], ['ExoDelayed'], ['RATE:ExoDelayed', 'BW:Jet'], -1],

        # Trigger tower (TT) triggers ATR-14446
        ['j60_TT', 'L1_J20', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        #AFP jets
        ['j10_L1AFP_C_AND',                        'L1_AFP_C_AND',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j10_L1AFP_C_ANY',                        'L1_AFP_C_ANY',                  [],  ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['j15_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j20_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j30_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j40_L1AFP_C_J12',                  'L1_AFP_C_J12',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j10_320eta490_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j15_320eta490_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j25_320eta490_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j35_320eta490_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j45_320eta490_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2j10_deta20_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j10_L1MBTS_2',                  'L1_MBTS_2',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['j10_320eta490_L1MBTS_2',                  'L1_MBTS_2',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2j10_deta20_L1MBTS_2',                  'L1_MBTS_2',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2j10_deta20_L1J12',                  'L1_J12',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2j10_deta20_L1J12_test4',                  'L1_J12',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],


        ### ATR-14353: L1Topo jet chains:
        ['j360_a10_lcw_L1HT150-J20.ETA31',   'L1_HT150-J20.ETA31',   [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j360_a10_lcw_L1HT150-J20s5.ETA31', 'L1_HT150-J20s5.ETA31', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j360_lcw_L1HT150-JJ15.ETA49',      'L1_HT150-JJ15.ETA49',  [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10_lcw_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j460_a10_lcw_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],


        ### ATR-14356: L1Topo VBF chians
        ['ht300_2j40_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_HT150-JJ15.ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'],-1,['serial',-1,['ht300', '2j40_0eta490_invm700']]],


        ['5j65_0eta240_L1J4-MATCH'  ,'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['5j70_L1J4-MATCH'  ,'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['6j45_0eta240_L1J4-MATCH'  ,'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],
        ['7j45_L1J4-MATCH'  ,'L1_J4-MATCH', [], [PhysicsStream], ['RATE:MultiJet', 'BW:Jet'], -1],

        
    ]

    TriggerFlags.BjetSlice.signatures = [

   
        ['j0_perf_boffperf_L1RD0_EMPTY', 'L1_RD0_EMPTY',[], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j0_perf_boffperf_L1MU10',       'L1_MU10',[], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j0_perf_boffperf_L1J12_EMPTY',  'L1_J12_EMPTY',[], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],



        ['j15_bperf', 'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_bperf', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        #bperf	
        ['j45_bperf_3j45', 'L1_3J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_bperf_3j55', 'L1_4J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j85_bperf', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j150_bperf', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j320_bperf', 'L1_J85', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        
        ['j15_bperf_split', 'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j85_bperf_split', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j150_bperf_split', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j320_bperf_split', 'L1_J85', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j150_bmv2c2077_split_j50_bmv2c2077_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j150_bmv2c2060_split_j50_bmv2c2060_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j175_bmv2c2060_split_j50_bmv2c2050_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j175_bmv2c2050_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j175_bmv2c2040_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j175_bmv2c2077_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
	
        ['2j35_bmv2c2077_split_3j35', 'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j45_bmv2c2085_split_3j45', 'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        # Calibration chains (default, offline taggers)
        ['j15_boffperf', 'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j35_boffperf', 'L1_J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_boffperf', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j85_boffperf', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j150_boffperf', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j320_boffperf', 'L1_J85', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j45_boffperf_3j45', 'L1_3J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        
        # Calibration chains (split, offline taggers)
        ['j15_boffperf_split', 'L1_RD0_FILLED', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j25_boffperf_split', 'L1_RD0_FILLED', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j35_boffperf_split', 'L1_J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j45_boffperf_split', 'L1_J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_boffperf_split', 'L1_J20', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j65_boffperf_split', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j85_boffperf_split', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j110_boffperf_split', 'L1_J30', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1], 
        ['j150_boffperf_split', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j175_boffperf_split', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
#        ['j200_boffperf_split', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j260_boffperf_split', 'L1_J75', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j320_boffperf_split', 'L1_J85', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j400_boffperf_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j45_boffperf_split_3j45', 'L1_3J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j45_boffperf_split_3j45_L13J15.0ETA25', 'L1_3J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j45_boffperf_split_3j45_L13J20', 'L1_3J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        #['2j45_bmv2c2077_split_2j45_L13J25.0ETA23', 'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],                        
        ['j100_2j55_bmv2c2060_split', 'L1_J75_3J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j100_2j55_bmv2c2060_split_L1J85_3J30', 'L1_J85_3J30', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j100_2j55_bmv2c2070_split', 'L1_J75_3J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j100_2j55_bmv2c2077_split', 'L1_J75_3J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        

        #bperf                                                                                                                                       
        ['j55_bperf_split', 'L1_J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j45_bperf_split_3j45', 'L1_3J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        
        
	['j225_bmv2c2070_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
#        ['2j35_bmv2c2070_split_2j35_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j45_bmv2c2077_split_2j45_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j300_bmv2c2085_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j175_bmv2c2077_split_j60_bmv2c2077_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j225_bmv2c2060_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_bmv2c2077_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_bmv2c2085_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        #ATR-14196
        ['j275_bmv2c2060_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j275_bmv2c2070_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j300_bmv2c2077_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j360_bmv2c2085_split', 'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],                   
        # L1Topo VBF MJJ bjets

        
        #GSC test 
        ['j225_boffperf_split',        'L1_J100', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j360_boffperf_split',        'L1_J100', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc360_boffperf_split', 'L1_J100', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc380_boffperf_split', 'L1_J100', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j225_gsc400_boffperf_split', 'L1_J100', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j55_gsc110_boffperf_split',   'L1_J30', [], [PhysicsStream, 'express'], ['RATE:SingleBJet', 'BW:BJet'], -1],


#        ['2j55_bmv2c2085_split_L1J30_2J20_4J20.0ETA49_MJJ-800', 'L1_J30_2J20_4J20.0ETA49_MJJ-800', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2085_split_L1J30_2J20_4J20.0ETA49_MJJ-900', 'L1_J30_2J20_4J20.0ETA49_MJJ-900', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-400', 'L1_3J20_4J20.0ETA49_MJJ-400', [''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-700', 'L1_3J20_4J20.0ETA49_MJJ-700', [''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-800', 'L1_3J20_4J20.0ETA49_MJJ-800', [''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2085_split_L13J20_4J20.0ETA49_MJJ-900', 'L1_3J20_4J20.0ETA49_MJJ-900', [''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-400', 'L1_3J20_4J20.0ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-700', 'L1_3J20_4J20.0ETA49_MJJ-700', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-800', 'L1_3J20_4J20.0ETA49_MJJ-800', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L13J20_4J20.0ETA49_MJJ-900', 'L1_3J20_4J20.0ETA49_MJJ-900', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-400', 'L1_J30_2J20_4J20.0ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-700', 'L1_J30_2J20_4J20.0ETA49_MJJ-700', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-800', 'L1_J30_2J20_4J20.0ETA49_MJJ-800', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['2j55_bmv2c2077_split_L1J30_2J20_4J20.0ETA49_MJJ-900', 'L1_J30_2J20_4J20.0ETA49_MJJ-900', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],


        ['j80_bmv2c2070_split_j60_bmv2c2085_split_j45_320eta490', 'L1_J40.0ETA25_2J25_J20.31ETA49', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ## b-jet mania
        ['j65_bmv2c2070_split_3j65_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j70_bmv2c2077_split_3j70_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],      
        ['j75_bmv2c2077_split_3j75_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],	
        ['2j45_bmv2c2077_split_2j45_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],	
        ['2j45_bmv2c2070_split_2j45_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],	
        ['2j35_bmv2c2070_split_2j35_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],	
        ['2j35_bmv2c2060_split_2j35_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j75_bmv2c2070_split_3j75_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j50_bmv2c2040_split_3j50_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        
        ['j80_bmv2c2085_split_2j60_320eta490', 'L1_J40.0ETA25_2J15.31ETA49', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j65_bmv2c2070_split_j65', 'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j70_bmv2c2077_split_j70', 'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j70_bmv2c2070_split_j70', 'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j75_bmv2c2077_split_j75', 'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['3j35_bmv2c2077_split_j35', 'L1_4J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j35_bmv2c2077_split_j35_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j55_bmv2c2070_split', 'L1_3J40', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j35_bmv2c2060_split', 'L1_4J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j35_bmv2c2060_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j65_bmv2c2070_split_3j65_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
        ['j70_bmv2c2077_split_3j70_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
        ['j75_bmv2c2070_split_3j75_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
        ['j65_bmv2c2050_split_3j65_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
        ['j70_bmv2c2070_split_3j70_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],
        ['j75_bmv2c2060_split_3j75_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:SingleBJet',  'BW:BJet'], -1],       
        ['2j35_bmv2c2060_split_2j35_L14J15.0ETA25',            'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],
        ['2j45_bmv2c2060_split_2j45_L14J15.0ETA25',           'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],        
        ['2j45_bmv2c2085_split_3j45_L14J15.0ETA25',          'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],
        ['2j45_bmv2c2077_split_3j45_L14J15.0ETA25',          'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],
        ['2j45_bmv2c2070_split_3j45_L14J15.0ETA25',          'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],        
        ['2j70_bmv2c2060_split_j70',                         'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],
        ['2j75_bmv2c2070_split_j75',                         'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],

        #tight b-jets for 2e34 physics ATR-15226
        ['2j75_bmv2c2060_split_j75_L13J40', 'L1_3J40', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j70_bmv2c2050_split_j70_L13J40', 'L1_3J40', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['3j55_bmv2c2070_split_L13J35.0ETA23', 'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j45_bmv2c2060_split_L13J35.0ETA23', 'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['3j45_bmv2c2060_split_L13J35.0ETA23', 'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        #tight b-jets for 2e34 backup menu ATR-15226
        ['2j75_boffperf_split_j75_L13J40', 'L1_3J40', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j70_boffperf_split_j70_L13J40', 'L1_3J40', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['3j55_boffperf_split_L13J35.0ETA23', 'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['3j45_boffperf_split_L13J35.0ETA23', 'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
#        ['3j45_boffperf_split_L13J35.0ETA23', 'L1_3J35.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

 
        ['j150_boffperf_split_j50_boffperf_split', 'L1_J50', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['j175_boffperf_split_j50_boffperf_split', 'L1_J40', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['2j45_boffperf_split_3j45', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],	

        ['3j35_boffperf_split_j35_L14J20', 'L1_4J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['3j35_boffperf_split_j35_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['3j55_boffperf_split', 'L1_3J40', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['3j35_boffperf_split', 'L1_3J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['3j35_boffperf_split_L14J15.0ETA25', 'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j75_boffperf_split_3j75_L14J20', 'L1_4J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['2j35_boffperf_split_2j35_L14J20', 'L1_4J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],



        #b-jets for 1.5e34
        ['2j70_bmv2c2050_split_j70',                    'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],
        ['2j75_bmv2c2060_split_j75',                    'L1_3J25.0ETA23', [], [PhysicsStream], ['RATE:MultiBJet',  'BW:BJet'], -1],

        ['2j45_bmv2c2077_split_3j45',                   'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j45_bmv2c2070_split_3j45',                   'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j45_bmv2c2060_split_3j45',                   'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j45_bmv2c2050_split_3j45',                   'L1_5J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j45_bmv2c2060_split_3j45_L14J15.0ETA25',     'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j45_bmv2c2050_split_3j45_L14J15.0ETA25',     'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['2j35_bmv2c2050_split_2j35_L14J15',         'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c2050_split_2j35_L14J15.0ETA25',  'L1_4J15.0ETA25', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],	
        ['j100_2j55_bmv2c2050_split',                'L1_J75_3J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j100_2j55_bmv2c2050_split_L1J85_3J30',                'L1_J85_3J30', [], [PhysicsStream],['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j150_bmv2c2050_split_j50_bmv2c2050_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j175_bmv2c2060_split_j60_bmv2c2060_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
        ['j175_bmv2c2050_split_j50_bmv2c2050_split', 'L1_J100', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],

        ['j225_bmv2c2050_split',                     'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j75_bmv2c2060_split_3j75_L14J15',          'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['j300_bmv2c2070_split',                     'L1_J100', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],

        ['j75_bmv2c2060_split_3j75_L14J20', 'L1_4J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1],
        ['2j35_bmv2c2050_split_2j35_L14J20', 'L1_4J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1],
]
   
    TriggerFlags.METSlice.signatures = [
           # Rerun chains
        ['xe0noL1_l2fsperf',         '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_tc_lcw',  '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_tc_em',   '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_mht',     '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_pueta',   '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe0noL1_l2fsperf_pufit',   '',        [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe35',                                   'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_tc_lcw',                            'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_tc_em',                             'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_pueta',                             'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_pufit',                             'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_mht',                               'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe35_wEFMu',                    'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_tc_lcw_wEFMu',             'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_tc_em_wEFMu',              'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_mht_wEFMu',                'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_pueta_wEFMu',              'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe35_pufit_wEFMu',              'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],


        ['xe50',                                   'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        
        ['xe70_L1XE45',                       'L1_XE45', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe80_tc_lcw_L1XE50',       'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_tc_lcw_L1XE50',             'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_tc_lcw_wEFMu_L1XE50',       'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_mht_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe70_L1XE50',                    'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe75_L1XE50',                    'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe80_L1XE50',                    'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_L1XE50',                    'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe75_L1XE55',                    'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe80_L1XE55',                    'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_L1XE55',                    'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe70_L1XE50_metrate_studies',                    'L1_XE50', [], ['MetRateStudies'], ['RATE:MET', 'BW:MET'], -1],
        ['xe75_L1XE50_metrate_studies',                    'L1_XE50', [], ['MetRateStudies'], ['RATE:MET', 'BW:MET'], -1],
        ['xe80_L1XE50_metrate_studies',                    'L1_XE50', [], ['MetRateStudies'], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_L1XE50_metrate_studies',                    'L1_XE50', [], ['MetRateStudies'], ['RATE:MET', 'BW:MET'], -1],

        ['xe75_L1XE55_metrate_studies',                    'L1_XE55', [], ['MetRateStudies'], ['RATE:MET', 'BW:MET'], -1],
        ['xe80_L1XE55_metrate_studies',                    'L1_XE55', [], ['MetRateStudies'], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_L1XE55_metrate_studies',                    'L1_XE55', [], ['MetRateStudies'], ['RATE:MET', 'BW:MET'], -1],
        
 	['xe95_pufit_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe95_pufit_L1XE55',               'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
                
        ['xe100_L1XE50',                    'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_lcw_L1XE50',             'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_em_L1XE50',              'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
#        ['xe100_pueta_L1XE50',              'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_L1XE50',              'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_L1XE55',              'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
#        
 	['xe105_pufit_L1XE50',              'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe105_pufit_L1XE55',              'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe100_wEFMu_L1XE50',              'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_lcw_wEFMu_L1XE50',       'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_em_wEFMu_L1XE50',        'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_mht_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
#        ['xe100_pueta_wEFMu_L1XE50',        'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
#        ['xe100_pufit_wEFMu_L1XE50',        'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe80_L1XE35',                     'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_L1XE35',                    'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_lcw_L1XE35',             'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_em_L1XE35',              'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_lcw_wEFMu_L1XE35',       'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_tc_em_wEFMu_L1XE35',        'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_mht_L1XE35',                'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_mht_wEFMu_L1XE35',          'L1_XE35', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        

        ## add chains for MC16 (ATR-15096)
        ['xe110_pueta_wEFMu_L1XE50',        'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_wEFMu_L1XE50',        'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pueta_wEFMu_L1XE50',        'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_wEFMu_L1XE50',        'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_L1XE50',                    'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_wEFMu_L1XE50',              'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_pufit_L1XE55',               'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_pufit_wEFMu_L1XE55',         'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_wEFMu_L1XE55',        'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pueta_wEFMu_L1XE55',        'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_wEFMu_L1XE55',        'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pueta_wEFMu_L1XE55',        'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_wEFMu_L1XE55',        'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_wEFMu_L1XE55',              'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_wEFMu_L1XE55',          'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_mht_wEFMu_L1XE55',          'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_wEFMu_L1XE55',          'L1_XE55', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_pufit_wEFMu_L1XE60',        'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1XE60',                'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
        ['xe120_tc_lcw_L1XE60',             'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pueta_L1XE60',              'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1], 
        ['xe120_pufit_L1XE60',              'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_L1XE60',             'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_L1XE60',              'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_wEFMu_L1XE60',       'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_wEFMu_L1XE60',          'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_wEFMu_L1XE60',        'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_L1XE60',             'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_L1XE60',              'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_wEFMu_L1XE60',       'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_wEFMu_L1XE60',          'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_wEFMu_L1XE60',        'L1_XE60', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit',                     'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_pufit_wEFMu',               'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_lcw_L1XE70',             'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_L1XE70',                'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pufit_L1XE70',              'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_lcw_wEFMu_L1XE70',       'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_wEFMu_L1XE70',          'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pufit_wEFMu_L1XE70',        'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_L1XE70',             'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_L1XE70',              'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_L1XE70',             'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_L1XE70',              'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_wEFMu_L1XE70',       'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_wEFMu_L1XE70',          'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_wEFMu_L1XE70',        'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_L1XE80',             'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_L1XE80',                'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_L1XE80',              'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_wEFMu_L1XE80',       'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_wEFMu_L1XE80',          'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_wEFMu_L1XE80',        'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_L1XE80',             'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_L1XE80',                'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_L1XE80',              'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_wEFMu_L1XE80',       'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_wEFMu_L1XE80',          'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_wEFMu_L1XE80',        'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe120_mht_L1XE60_AND_xe70_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe120_mht_L1XE60_AND','xe70_L1XE60'] ]],


        ##add muon corrections
        ['xe100',                    'L1_XE70', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],


        ###xe120 from L1_XE80
        ['xe120',                    'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw',             'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em',              'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht',                'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pueta',              'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit',              'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe120_wEFMu',              'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_wEFMu',       'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_wEFMu',        'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_wEFMu',          'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pueta_wEFMu',        'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_wEFMu',        'L1_XE80', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        
        ['xs15_L1XS20',                            'L1_XS20',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xs20_L1XS30',                            'L1_XS30',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe100_L1XE60',               'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],  
	
 	['xe100_tc_lcw_L1XE60',               'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe100_mht_L1XE60',                   'L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

 	['xe110_tc_em_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_L1XE50',                      'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_wEFMu_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_em_wEFMu_L1XE50',          'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe120_tc_em_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe120_L1XE50',                      'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe120_wEFMu_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe120_tc_em_wEFMu_L1XE50',          'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe110_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_L1XE50',                'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_wEFMu_L1XE50',          'L1_XE50', [], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        
        ['xe110_pueta_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pueta_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pueta_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pueta_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_L1XE50',                'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        
 	['xe110_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_lcw_wEFMu_L1XE50',         'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe120_tc_lcw_wEFMu_L1XE50',         'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe130_tc_lcw_wEFMu_L1XE50',         'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe140_tc_lcw_wEFMu_L1XE50',         'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe150_tc_lcw_wEFMu_L1XE50',         'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_L1XE50',               'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe160_tc_lcw_wEFMu_L1XE50',         'L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe90_mht_L1XE55',                   'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_mht_L1XE55',                   'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_L1XE55',                   'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        
 	['xe110_tc_lcw_L1XE55',               'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_L1XE55',               'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_lcw_L1XE55',               'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

 	['xe110_pueta_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_pufit_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_pueta_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe120_pufit_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pueta_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe130_pufit_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe100_L1XE55',                      'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_L1XE55',                      'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_L1XE55',                      'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        
        ['xe110_tc_em_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_lcw_wEFMu_L1XE55',         'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe90_mht_wEFMu_L1XE55',             'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_wEFMu_L1XE55',                'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
 	['xe110_tc_em_wEFMu_L1XE55',          'L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],


        ['xe45_pufit_wEFMu',          'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_wEFMu',                'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_mht_wEFMu',            'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_pueta',                'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_tc_lcw',               'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_tc_em',                'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_mht',                  'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_pueta_wEFMu',          'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_tc_lcw_wEFMu',         'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_tc_em_wEFMu',          'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45_pufit',                'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe45',                             'L1_XE45',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

 	['xe100_wEFMu_L1XE35',                'L1_XE35',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],


        
        ['xe100_mht_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_lcw_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_lcw_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_em_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_tc_em_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_em_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pufit_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pufit_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pueta_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_pueta_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_mht_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_mht_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_tc_lcw_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_tc_lcw_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_tc_lcw_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_em_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_em_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_em_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_tc_em_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_em_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_em_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pufit_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pufit_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pufit_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pufit_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pufit_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pufit_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pueta_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pueta_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pueta_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pueta_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pueta_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pueta_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_pueta_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pueta_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_pueta_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pueta_wEFMu_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_pueta_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_wEFMu_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe140_mht_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_mht_L1XE70','L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_mht_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_mht_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_mht_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_mht_L1XE70','L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_L1XE50','L1_XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_mht_L1XE70','L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

        ['xe130_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe160_L1XE55','L1_XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe150_L1XE60','L1_XE60',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe100_L1XE70','L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_L1XE70','L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe120_L1XE70','L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe130_L1XE70','L1_XE70',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],

				###cell+mht XE
        ['xe110_mht_L1XE50_AND_xe65_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE50_AND','xe65_L1XE50'] ]],
        ['xe110_mht_L1XE50_AND_xe70_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE50_AND','xe70_L1XE50'] ]],
        ['xe110_mht_L1XE50_AND_xe75_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE50_AND','xe75_L1XE50'] ]],
        ['xe110_mht_L1XE50_AND_xe80_L1XE50','L1_XE50', ['L1_XE50','L1_XE50'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE50_AND','xe80_L1XE50'] ]],
        ['xe110_mht_L1XE55_AND_xe65_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE55_AND','xe65_L1XE55'] ]],
        ['xe110_mht_L1XE55_AND_xe70_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE55_AND','xe70_L1XE55'] ]],
        ['xe110_mht_L1XE55_AND_xe75_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE55_AND','xe75_L1XE55'] ]],
        ['xe110_mht_L1XE55_AND_xe80_L1XE55','L1_XE55', ['L1_XE55','L1_XE55'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe110_mht_L1XE55_AND','xe80_L1XE55'] ]],
    
        ### ATR-14354: L1Topo met chains
        #['xe110_mht_L1KF-XE50',    'L1_KF-XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_mht_L1KF-XE55',    'L1_KF-XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        #['xe140_tc_lcw_L1KF-XE50', 'L1_KF-XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe140_tc_lcw_L1KF-XE55', 'L1_KF-XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        #['xe110_L1KF-XE50',        'L1_KF-XE50',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        ['xe110_L1KF-XE55',        'L1_KF-XE55',[], [PhysicsStream], ['RATE:MET', 'BW:MET'], -1],
        
        #Chains for testing 2e34 menus
        ['xe130_mht_L1XE60_AND_xe90_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe130_mht_L1XE60_AND','xe90_L1XE60'] ]], 
        ['xe150_mht_L1XE60_AND_xe100_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe150_mht_L1XE60_AND','xe100_L1XE60'] ]], 
        ['xe200_mht_L1XE60_AND_xe150_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe200_mht_L1XE60_AND','xe150_L1XE60'] ]], 
        ['xe250_mht_L1XE60_AND_xe200_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe250_mht_L1XE60_AND','xe200_L1XE60'] ]], 
        ['xe300_mht_L1XE60_AND_xe250_L1XE60','L1_XE60', ['L1_XE60','L1_XE60'], [PhysicsStream], ['RATE:MET', 'BW:MET' ], -1, ['serial',-1,['xe300_mht_L1XE60_AND','xe250_L1XE60'] ]], 
    
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
        # Run-II - No BDT: variations
        ['tau25_perf_ptonly',                      'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_perf_ptonly_L1TAU12',              'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        # Run-II - With BDT: main track-based items
        ['tau25_medium1_track',                    'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_loose1_tracktwo',                  'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_medium1_tracktwo',                 'L1_TAU12IM', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_tight1_tracktwo',                  'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_medium1_tracktwo_L1TAU12',         'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        # Run-II - With BDT: main calo-based items
        # Run-II - With BDT: pt only
        ['tau25_loose1_ptonly',                    'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_medium1_ptonly',                   'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_tight1_ptonly',                    'L1_TAU12IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        # Run-II - High-pT variations
        ['tau35_loose1_tracktwo',                  'L1_TAU20IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau35_medium1_tracktwo',                 'L1_TAU20IM', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau50_medium1_tracktwo_L1TAU12',         'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau80_medium1_tracktwo',                 'L1_TAU40', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau80_medium1_tracktwo_L1TAU60',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau125_medium1_tracktwo',                'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_medium1_tracktwo',                'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_perf_tracktwo',                   'L1_TAU60', [], [PhysicsStream, 'express'], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_idperf_track',                    'L1_TAU60', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Tau', 'BW:ID'], -1],
        ['tau160_idperf_tracktwo',                 'L1_TAU60', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Tau', 'BW:ID'], -1],

        # chains for 2e34 backup (ATR-15225)
        ['tau160_medium1_tracktwo_L1TAU100',       'L1_TAU100', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau200_medium1_tracktwo_L1TAU100',       'L1_TAU100', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],


        #
        ['tau8_cosmic_track', 'L1_TAU8_EMPTY', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau8_cosmic_ptonly', 'L1_TAU8_EMPTY', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau1_cosmic_track_L1MU4_EMPTY',  'L1_MU4_EMPTY', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau1_cosmic_ptonly_L1MU4_EMPTY', 'L1_MU4_EMPTY', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

        
        ['tau35_perf_tracktwo_tau25_perf_tracktwo', 'L1_TAU20IM_2TAU12IM' , ['L1_TAU20IM','L1_TAU12IM'],[PhysicsStream, 'express'], ['RATE:MultiTau','BW:Tau'], -1],
        
        ['tau35_loose1_tracktwo_tau25_loose1_tracktwo',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_loose1_tracktwo","tau25_loose1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],


        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20IM_2TAU12IM',   'L1_TAU20IM_2TAU12IM',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],

	# l1topo ditau chains
	['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1TAU20ITAU12I-J25',   'L1_TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]]	,
	['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_L1BOX-TAU20ITAU12I',   'L1_BOX-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],

        # tau+tau topo_start_from
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],True],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_notautsf_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],False],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_tautsf_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],True],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_notautsf_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]],False],

	['tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo"]]],

        # chains for 2e34 backup (ATR-15225)
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30',   'L1_TAU20IM_2TAU12IM_J25_2J20_3J12',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I',   'L1_DR-TAU20ITAU12I',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],
        ['tau35_tight1_tracktwo_tau25_tight1_tracktwo_03dR30_L1DR-TAU20ITAU12I-J25',   'L1_DR-TAU20ITAU12I-J25',['L1_TAU20IM','L1_TAU12IM'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau35_tight1_tracktwo","tau25_tight1_tracktwo"]]],




        # High-pT DiTau seeding off of single-tau
        ['tau80_medium1_tracktwo_L1TAU60_tau50_medium1_tracktwo_L1TAU12', 'L1_TAU60',['L1_TAU60','L1_TAU12'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau80_medium1_tracktwo_L1TAU60","tau50_medium1_tracktwo_L1TAU12"]]],
        ['tau125_medium1_tracktwo_tau50_medium1_tracktwo_L1TAU12', 'L1_TAU60',['L1_TAU60','L1_TAU12'], [PhysicsStream], ['RATE:MultiTau', 'BW:Tau'], -1,['serial',-1,["tau125_medium1_tracktwo","tau50_medium1_tracktwo_L1TAU12"]]],

        ['tau35_perf_tracktwo_tau25_perf_tracktwo_ditauM', 'L1_TAU20IM_2TAU12IM' , ['L1_TAU20IM','L1_TAU12IM'],[PhysicsStream, 'express'], ['RATE:MultiTau','BW:Tau'], -1],
        ['tau60_medium1_tracktwo_tau35_medium1_tracktwo','L1_TAU40_2TAU20IM',['L1_TAU40','L1_TAU20IM'],[PhysicsStream],['RATE:MultiTau', 'BW:Tau'],-1,['serial',-1,["tau60_medium1_tracktwo","tau35_medium1_tracktwo"]]],


        # photon+pion chains (ATR-13525) 
        ['tau25_singlepion_tracktwo',                    'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_singlepiontight_tracktwo',                    'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

        #tau+photon (ATR-13061)
        ['tau35_dikaonmasstight_tracktwo_L1TAU12',            'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau35_dikaonmass_tracktwo_L1TAU12',            'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_dikaonmasstight_tracktwo',                    'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_dikaonmass_tracktwo',                    'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

	#tau + photon (ATR-13841)
        ['tau25_dipion1_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau35_dipion1loose_tracktwo_L1TAU12',  'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_dipion2_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau35_dipion2_tracktwo_L1TAU12',       'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_kaonpi1_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau35_kaonpi1_tracktwo_L1TAU12',       'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau25_kaonpi2_tracktwo',               'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau35_kaonpi2_tracktwo_L1TAU12',       'L1_TAU12', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

        ['tau160_medium1HighptL_tracktwo',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_medium1HighptM_tracktwo',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],
        ['tau160_medium1HighptH_tracktwo',         'L1_TAU60', [], [PhysicsStream], ['RATE:SingleTau', 'BW:Tau'], -1],

			 ]



    TriggerFlags.EgammaSlice.signatures = [

        ['g0_perf_L1EM3_EMPTY',                  'L1_EM3_EMPTY', [], [PhysicsStream], ['RATE:SinglePhoton',   'BW:Egamma'], -1],        
        ['e0_perf_L1EM3_EMPTY',                  'L1_EM3_EMPTY', [], [PhysicsStream], ['RATE:SinglePhoton',   'BW:Egamma'], -1],        

        ['g10_loose',                            'L1_EM7',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_loose_L1EM15',                     'L1_EM15', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['g10_etcut',                     'L1_EM7',   [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_etcut_L1EM12',              'L1_EM12',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['g3_loose_larpeb',                      'L1_EM3',     [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g12_loose_larpeb',                      'L1_EM10VH',     [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
	['g20_loose_larpeb_L1EM15',              'L1_EM15', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g40_loose_larpeb',                     'L1_EM20VHI', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g60_loose_larpeb',                     'L1_EM20VHI', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g80_loose_larpeb',                     'L1_EM20VHI', [], ['LArCells'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],        
        ['g200_etcut',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g250_etcut',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g300_etcut',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g300_etcut_L1EM24VHI',          'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
        ['g300_etcut_L1EM26VHI',          'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 

        ## Aranxtas request, to be run in rerun mode
        ['g22_tight',                             'L1_EM15VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['e15_lhtight_iloose_L1EM13VH', 'L1_EM13VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#        ['g10_loose_L1EM8I', 'L1_EM8I', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
#        ['g20_loose_L1EM15I', 'L1_EM15I', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g25_medium_L1EM22VHI', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM22VHI', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g45_tight_L1EM22VHI', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM26VHI', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['e140_etcut',  'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhvloose', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhvloose_nod0', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['g0_hiptrt_L1EM20VH',                  'L1_EM20VH',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1],
        ['g0_hiptrt_L1EM20VHI',                 'L1_EM20VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1],
        ['g0_hiptrt_L1EM22VHI',                 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1],
        ['g0_hiptrt_L1EM24VHI',                 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1],
        ['g0_hiptrt_L1EM26VHI',                 'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1],

        # PS-ed trigger to supmbined chains
        ['g15_loose_L1EM7',               'L1_EM7',   [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g20_loose_L1EM12',              'L1_EM12',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], # pass through
        ['g40_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g45_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g50_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g80_loose',                     'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 
        ['g100_loose',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'], -1], 

        # Rerun mode and PS
        ['g25_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose_L1EM15',              'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g60_loose',                     'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g70_loose',                     'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        # Rerun mode
        ['g10_medium',                    'L1_EM7',   [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g15_loose_L1EM3',               'L1_EM3',   [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g15_loose',                     'L1_EM13VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_loose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_tight',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g40_tight',                     'L1_EM20VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g45_tight',                     'L1_EM20VHI',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g50_loose',                     'L1_EM15VH', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g60_loose_L1EM15VH',            'L1_EM15VH', [], [PhysicsStream, 'express'], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        # Di-photon triggers
        ['g35_loose_L1EM15_g25_loose_L1EM15',       'L1_2EM15', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        #['g35_medium1_g25_medium1',                 'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g20_tight',                              'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g50_loose',                              'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g60_loose_L12EM15VH',                    'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],  
        ['2g50_loose_L12EM18VH',                    'L1_2EM18VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g60_loose_L12EM18VH',                    'L1_2EM18VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g50_loose_L12EM20VH',                    'L1_2EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g60_loose_L12EM20VH',                    'L1_2EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['g50_loose_L12EM18VH',                     'L1_EM18VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['g60_loose_L12EM18VH',                     'L1_EM18VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['g50_loose_L12EM20VH',                     'L1_EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['g60_loose_L12EM20VH',                     'L1_EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ##Adding tight diphoton triggers (ATR-10762)
        ['2g22_tight',                              'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],

        # Tri-photon triggers
        ['3g15_loose',                              'L1_2EM13VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['g20_loose_2g15_loose_L12EM13VH',          'L1_2EM13VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g20_loose_g15_loose',                    'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['3g20_loose',                              'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['3g20_loose_L12EM18VH',                    'L1_2EM18VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['3g22_loose_L12EM20VH',                    'L1_2EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 

        #prescaled
        ['2g20_loose_L12EM15',                      'L1_2EM15', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 
        ['2g20_loose',                   'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 


        ['2g10_loose',                           'L1_2EM7', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],

        ['e17_lhloose_L1EM15',                   'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_cutd0dphideta_L1EM15',     'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nod0_L1EM15',              'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nodeta_L1EM15',            'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nodphires_L1EM15',         'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #nod0 chains:
       	['e24_lhvloose_nod0_L1EM18VH',                'L1_EM18VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e24_lhvloose_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e26_lhvloose_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e28_lhvloose_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e26_lhmedium_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e28_lhmedium_nod0_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e24_lhmedium_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['2e12_lhvloose_L12EM10VH',              'L1_2EM10VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e12_lhvloose_nod0_L12EM10VH',         'L1_2EM10VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e17_lhloose',                    'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1], 

        # Zee T&P
        ['e24_lhtight_L1EM20VH_e15_etcut_L1EM7_Zee',      'L1_EM20VH',  ['L1_EM20VH','L1_EM7'], [PhysicsStream], ['RATE:MultiElectron','BW:Egamma'],-1],
        ['e24_lhtight_nod0_L1EM20VH_e15_etcut_L1EM7_Zee', 'L1_EM20VH',  ['L1_EM20VH','L1_EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_e15_etcut_L1EM7_Zee',               'L1_EM22VHI', ['L1_EM22VHI','L1_EM7'], [PhysicsStream], ['RATE:MultiElectron','BW:Egamma'],-1], 
        ['e26_lhtight_nod0_e15_etcut_L1EM7_Zee',    	  'L1_EM22VHI', ['L1_EM22VHI','L1_EM7'], [PhysicsStream, 'express'], ['RATE:MultiElectron', 'BW:Egamma'],-1 ],
        ['e28_lhtight_nod0_e15_etcut_L1EM7_Zee',    	  'L1_EM24VHI', ['L1_EM24VHI','L1_EM7'], [PhysicsStream, 'express'], ['RATE:MultiElectron', 'BW:Egamma'],-1 ],
        ['e30_lhtight_nod0_e15_etcut_L1EM7_Zee',          'L1_EM26VHI', ['L1_EM26VHI','L1_EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],

        # Jpsiee T&P LH-based
        ['e5_lhtight_e4_etcut',              'L1_2EM3',      [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_e4_etcut_Jpsiee',       'L1_2EM3',      [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1], 
        ['e9_lhtight_e4_etcut_Jpsiee',       'L1_EM7_2EM3',  ['L1_EM7','L1_EM3'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_e9_etcut_Jpsiee',       'L1_EM7_2EM3',  ['L1_EM3','L1_EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1], 
        ['e14_lhtight_e4_etcut_Jpsiee',      'L1_EM12_2EM3', ['L1_EM12','L1_EM3'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_e14_etcut_Jpsiee',      'L1_EM12_2EM3', ['L1_EM3','L1_EM12'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],

        # Jpsiee T&P LH-based nod0
        ['e5_lhtight_nod0_e4_etcut',         'L1_2EM3',      [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0_e4_etcut_Jpsiee',  'L1_2EM3',      [], [PhysicsStream, 'express'], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e9_lhtight_nod0_e4_etcut_Jpsiee',  'L1_EM7_2EM3',  ['L1_EM7','L1_EM3'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0_e9_etcut_Jpsiee',  'L1_EM7_2EM3',  ['L1_EM3','L1_EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e14_lhtight_nod0_e4_etcut_Jpsiee', 'L1_EM12_2EM3', ['L1_EM12','L1_EM3'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0_e14_etcut_Jpsiee', 'L1_EM12_2EM3', ['L1_EM3','L1_EM12'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],

        # Supporting trigger
        ['e0_perf_L1EM15',              'L1_EM15',[], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['g0_perf_L1EM15',                'L1_EM15',  [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1], 
 
        ['e5_lhloose',                          'L1_EM3',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e5_lhloose_idperf',                   'L1_EM3',       [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],

        # extra id perf chains for TRT HT studies                                                                                               

        ['e5_lhtight_idperf',                     'L1_EM3',       [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        ['e10_lhtight_idperf',                     'L1_EM7',       [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],

        ['e24_lhmedium_idperf_L1EM20VH',        'L1_EM20VH',    [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        # Single electron triggers
        ['e17_lhmedium_nod0_L1EM15',                    'L1_EM15',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_L1EM18VH',                'L1_EM18VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhtight_L1EM20VH',                 'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 

        ['e28_lhtight_nod0_ringer_ivarloose', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e60_medium',                           'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhmedium',                         'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e60_medium_L1EM24VHI',               	 'L1_EM24VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e60_lhmedium_nod0', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#ATR-15233        ['e24_lhtight_iloose',                   'L1_EM20VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#ATR-15233        ['e24_lhtight_nod0_iloose', 'L1_EM20VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#ATR-15233        ['e24_lhtight_nod0_ivarloose', 'L1_EM20VHI', [], [PhysicsStream], ['RATE:SingleElectron'  , 'BW:Egamma'],-1],

#ATR-15233        ['e24_lhmedium_iloose',                  'L1_EM20VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_iloose_L1EM20VH',         'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e24_lhmedium_nod0_iloose_L1EM20VH', 'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e120_lhloose',                         'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e120_lhloose_nod0', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['g140_loose',        'L1_EM22VHI', [], [PhysicsStream, 'express'], ['RATE:SinglePhoton','BW:Egamma'],-1],
        ['g140_tight',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1],
        ['g160_loose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1],
        ['g180_loose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1],
        ['g200_loose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1],
        ['e140_lhloose',                         'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e140_lhloose_nod0', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

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
        ['e70_lhvloose_nod0',  														'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e80_lhvloose_nod0',  														'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e100_lhvloose_nod0', 														'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],
        ['e120_lhvloose_nod0', 														'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1 ],

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
        ['e60_etcut',                            'L1_EM22VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e70_etcut',                            'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_etcut',                            'L1_EM22VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e100_etcut',                           'L1_EM22VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e120_etcut',                           'L1_EM22VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e200_etcut',                           'L1_EM22VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 

        ['e5_lhvloose',                         'L1_EM3',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e10_lhvloose_L1EM7',                  'L1_EM7',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e15_lhvloose_L1EM7',                  'L1_EM7',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e20_lhvloose_L1EM12',                 'L1_EM12',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e25_lhvloose_L1EM15',                 'L1_EM15',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e30_lhvloose_L1EM15',                 'L1_EM15',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e40_lhvloose_L1EM15',                 'L1_EM15',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e50_lhvloose_L1EM15',                 'L1_EM15',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e70_lhvloose',                        'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e80_lhvloose',                        'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e100_lhvloose',                       'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e120_lhvloose',                       'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
	['e5_lhvloose_nod0',                         'L1_EM3',       [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        # use coherent PS with lhvloose and lhvloose_nod0 variants below:


        ['e12_lhvloose_L1EM10VH',               'L1_EM10VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e15_lhvloose_L1EM13VH',               'L1_EM13VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e17_lhvloose',                        'L1_EM15VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e20_lhvloose',                        'L1_EM15VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e40_lhvloose',                       'L1_EM20VH',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e60_lhvloose',                       'L1_EM22VHI',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e12_lhvloose_nod0_L1EM10VH',               'L1_EM10VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e15_lhvloose_nod0_L1EM13VH',               'L1_EM13VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e17_lhvloose_nod0',                        'L1_EM15VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
	['e20_lhvloose_nod0',                        'L1_EM15VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e40_lhvloose_nod0',                       'L1_EM20VH',      [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e60_lhvloose_nod0',                        'L1_EM22VHI',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        #Single electron/chaing for testing mis-ant robust LH ID
        ['e24_lhmedium_nod0_L1EM18VH',            'L1_EM18VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #New nod0 chains:
        ['e24_lhtight_nod0_L1EM20VH',                 'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        # Rerun mode
        ['e4_etcut',                             'L1_EM3',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e9_etcut',                             'L1_EM7',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e14_etcut',                            'L1_EM12',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 

	['e7_lhmedium',                          'L1_EM3',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],        
	['e9_lhloose',                          'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhmedium',                        'L1_EM8VH',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e9_lhmedium',                          'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhmedium',                         'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhmedium_iloose',                  'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e20_lhmedium',                         'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_L1EM15VH',                'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_L1EM22VHI',              'L1_EM22VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e60_lhloose',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e70_lhloose',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e5_lhtight',                           'L1_EM3',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e9_lhtight',                           'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e14_lhtight',                          'L1_EM12',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

	['e7_lhmedium_nod0',                           'L1_EM3',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],        
	['e9_lhloose_nod0',                           'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
	['e9_lhmedium_nod0',                          'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhmedium_nod0',                        'L1_EM8VH',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhmedium_nod0',                         'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhmedium_nod0_iloose',                  'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        

        ['e20_lhmedium_nod0',                         'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_nod0_L1EM15VH',                'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhloose_nod0',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e70_lhloose_nod0',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhloose_nod0',                          'L1_EM22VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhloose_nod0_L1EM26VHI',                          'L1_EM26VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e70_lhloose_nod0_L1EM26VHI',                          'L1_EM26VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhloose_nod0_L1EM26VHI',                          'L1_EM26VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e80_lhloose_nod0_L1EM24VHI',                          'L1_EM24VHI',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e5_lhtight_nod0',                           'L1_EM3',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e9_lhtight_nod0',                           'L1_EM7',     [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e14_lhtight_nod0',                          'L1_EM12',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#ATR-15233        ['e24_lhmedium_nod0_L1EM20VHI',              'L1_EM20VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhmedium_nod0_L1EM22VHI',              'L1_EM22VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],


         # Supporting triggers
         # pairs of triggers (cut-based -- likelihood) should be in coherent PS
         
        ['e24_lhvloose_L1EM18VH',                'L1_EM18VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhvloose_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhvloose_L1EM20VH',                'L1_EM20VH',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ##########
        # Single photon triggers        
        ['g120_loose',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g60_loose_L1EM24VHI',           'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g70_loose_L1EM24VHI',           'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g80_loose_L1EM24VHI',           'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g100_loose_L1EM24VHI',          'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g120_loose_L1EM24VHI',          'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g60_loose_L1EM26VHI',           'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g70_loose_L1EM26VHI',           'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g80_loose_L1EM26VHI',           'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g100_loose_L1EM26VHI',          'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g120_loose_L1EM26VHI',          'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g140_loose_L1EM26VHI',          'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g160_loose_L1EM26VHI',          'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g180_loose_L1EM26VHI',          'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 
        ['g200_loose_L1EM26VHI',          'L1_EM26VHI', [], [PhysicsStream], ['RATE:SinglePhoton','BW:Egamma'],-1], 

        # rerun mode


        ['e12_lhloose',                          'L1_EM8VH',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_L1EM10VH',                 'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_cutd0dphideta_L1EM10VH',   'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nod0_L1EM10VH',                 'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nodeta_L1EM10VH',          'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nodphires_L1EM10VH',       'L1_EM10VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e12_lhloose_nod0',                          'L1_EM8VH',   [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],


        ['e17_lhloose',                          'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_cutd0dphideta',            'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e17_lhloose_nod0',                          'L1_EM15VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],


        ['e15_lhloose_L1EM13VH',                 'L1_EM13VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_lhloose_cutd0dphideta_L1EM13VH',   'L1_EM13VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e15_lhloose_nod0_L1EM13VH',                 'L1_EM13VH',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e18_etcut_trkcut_L1EM15', 'L1_EM15', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'], -1],

        ['g25_loose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g25_medium',                    'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_loose',                     'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_medium',                    'L1_EM15VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g20_loose_L1EM18VH',            'L1_EM18VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g22_loose_L1EM20VH',            'L1_EM20VH',[], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['2e17_lhloose_L12EM15',            'L1_2EM15',   [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1], 
        ['2e17_lhvloose',                        'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e17_lhvloose_nod0',                   'L1_2EM15VH', [], [PhysicsStream, 'express'], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e20_lhvloose_nod0',                   'L1_2EM18VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e22_lhvloose_nod0',                   'L1_2EM20VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e15_lhvloose_L12EM13VH',              'L1_2EM13VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['2e15_lhvloose_nod0_L12EM13VH',         'L1_2EM13VH', [], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],

        # support for g+tau chains (ATR-13841)
        ['g35_medium_L1EM22VHI',                    'L1_EM22VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],
        ['g35_medium_L1EM24VHI',                    'L1_EM24VHI', [], [PhysicsStream], ['RATE:SinglePhoton', 'BW:Egamma'],-1],

        ['e17_lhloose_2e9_lhloose',         'L1_EM15VH_3EM7', ['L1_EM15VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e17_lhmedium_2e9_lhmedium',       'L1_EM15VH_3EM7', ['L1_EM15VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
#Rerun:         
#Physics (tri-ele):
        ['e17_lhloose_nod0_2e9_lhloose_nod0',         'L1_EM15VH_3EM7', ['L1_EM15VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e17_lhmedium_nod0_2e9_lhmedium_nod0',       'L1_EM15VH_3EM7', ['L1_EM15VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e17_lhloose_nod0_2e10_lhloose_nod0_L1EM15VH_3EM8VH',                        'L1_EM15VH_3EM8VH', ['L1_EM15VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e17_lhvloose_nod0_2e10_lhvloose_nod0_L1EM15VH_3EM8VH',                        'L1_EM15VH_3EM8VH', ['L1_EM15VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e17_lhloose_nod0_2e12_lhloose_nod0_L1EM15VH_3EM10VH',                       'L1_EM15VH_3EM10VH', ['L1_EM15VH','L1_3EM10VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e20_lhloose_nod0_2e10_lhloose_nod0_L1EM18VH_3EM8VH',                        'L1_EM18VH_3EM8VH', ['L1_EM18VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e20_lhloose_nod0_e15_lhloose_nod0_e10_lhloose_nod0_L1EM18VH_3EM8VH',        'L1_EM18VH_3EM8VH', ['L1_EM18VH','L1_3EM8VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e20_lhloose_nod0_e15_lhloose_nod0_e10_lhloose_nod0_L1EM18VH_2EM10VH_3EM7',  'L1_EM18VH_2EM10VH_3EM7', ['L1_EM18VH','L1_2EM10VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e20_lhloose_nod0_e12_lhloose_nod0_e9_lhloose_nod0_L1EM18VH_2EM10VH_3EM7',  'L1_EM18VH_2EM10VH_3EM7', ['L1_EM18VH','L1_2EM10VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e35_lhloose_nod0_e25_lhloose_nod0_e15_lhloose_nod0_L1EM18VH_3EM8VH',        'L1_EM18VH_3EM8VH', ['L1_EM18VH','L1_3EM8VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e17_lhmedium_nod0_2e10_lhmedium_nod0_L1EM15VH_3EM8VH',                      'L1_EM15VH_3EM8VH', ['L1_EM15VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e17_lhloose_nod0_e12_lhloose_nod0_e9_lhloose_nod0_L1EM15VH_2EM10VH_3EM7',   'L1_EM15VH_2EM10VH_3EM7', ['L1_EM15VH','L1_2EM10VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  
        ['e17_lhmedium_nod0_e12_lhmedium_nod0_e9_lhmedium_nod0_L1EM15VH_2EM10VH_3EM7','L1_EM15VH_2EM10VH_3EM7', ['L1_EM15VH','L1_2EM10VH','L1_3EM7'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],  

        ['e22_lhloose_nod0_e12_lhloose_nod0_e10_lhloose_nod0_L1EM20VH_2EM10VH_3EM8VH',    'L1_EM20VH_2EM10VH_3EM8VH', ['L1_EM20VH','L1_2EM10VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e22_lhvloose_nod0_e12_lhvloose_nod0_e10_lhvloose_nod0_L1EM20VH_2EM10VH_3EM8VH', 'L1_EM20VH_2EM10VH_3EM8VH', ['L1_EM20VH','L1_2EM10VH','L1_3EM8VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e22_lhloose_nod0_2e12_lhloose_nod0_L1EM20VH_3EM10VH',                           'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e22_lhvloose_nod0_2e12_lhvloose_nod0_L1EM20VH_3EM10VH',    'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['RATE:MultiElectron', 'BW:Egamma'],-1],
        ['e22_lhmedium_nod0_2g12_loose_L1EM20VH_3EM10VH',     'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'],-1,['parallel',-1,[] ]],
        ['e22_lhmedium_nod0_2g12_medium_L1EM20VH_3EM10VH',    'L1_EM20VH_3EM10VH', ['L1_EM20VH','L1_3EM10VH'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'],-1,['parallel',-1,[] ]],

        # Di-EM triggers
        # cut-based


        # likelihood
        ['e24_lhmedium_L1EM15VH_g25_medium',  'L1_2EM15VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_g35_loose',            'L1_2EM15VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 


#Physics (e+g):
        ['e24_lhmedium_nod0_L1EM15VH_g25_medium',  'L1_2EM15VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e24_lhmedium_nod0_L1EM18VH_g25_medium',  'L1_2EM18VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e24_lhmedium_nod0_L1EM20VH_g25_medium',  'L1_2EM20VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_nod0_g35_loose',            'L1_2EM15VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_nod0_g35_loose_L12EM18VH',   'L1_2EM18VH', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e20_lhmedium_nod0_2g10_loose',           'L1_EM15VH_3EM7', ['L1_EM15VH','L1_2EM7'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_nod0_2g10_medium',          'L1_EM15VH_3EM7', ['L1_EM15VH','L1_2EM7'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_nod0_2g10_loose_L1EM15VH_3EM8VH',           'L1_EM15VH_3EM8VH', ['L1_EM15VH','L1_2EM8VH'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_nod0_2g10_loose_L1EM18VH_3EM8VH',           'L1_EM18VH_3EM8VH', ['L1_EM18VH','L1_2EM8VH'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 
        ['e20_lhmedium_nod0_2g10_medium_L1EM15VH_3EM8VH',          'L1_EM15VH_3EM8VH', ['L1_EM15VH','L1_2EM8VH'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]], 

        #Diphoton triggers
        ['g35_loose_g25_loose',                      'L1_2EM15VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_loose_g25_loose_L12EM18VH',            'L1_2EM18VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_loose_g25_loose_L12EM20VH',            'L1_2EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_medium_g25_medium_L12EM18VH',          'L1_2EM18VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_medium_g25_medium_L12EM20VH',          'L1_2EM20VH', [], [PhysicsStream], ['RATE:MultiPhoton', 'BW:Egamma'],-1],
        ['g35_medium_g25_medium',                    'L1_2EM15VH', [], [PhysicsStream, 'express'], ['RATE:MultiPhoton', 'BW:Egamma'],-1], 

        # LLP
        ['g35_medium_g25_medium_L1EM7_EMPTY',        'L1_EM7_EMPTY', ['L1_EM7_EMPTY','L1_2EM15VH'], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['g35_medium_g25_medium_L1EM7_UNPAIRED_ISO', 'L1_EM7_UNPAIRED_ISO', ['L1_EM7_UNPAIRED_ISO','L1_2EM15VH'], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],

        ['2g22_tight_L1EM7_EMPTY',    'L1_EM7_EMPTY', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g22_tight_L1EM7_UNPAIRED_ISO',        'L1_EM7_UNPAIRED_ISO', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g22_tight_L1EM7_UNPAIRED_NONISO',        'L1_EM7_UNPAIRED_NONISO', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g50_tight_L1EM7_EMPTY',    'L1_EM7_EMPTY', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g50_tight_L1EM7_UNPAIRED_ISO',        'L1_EM7_UNPAIRED_ISO', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],
        ['2g50_tight_L1EM7_UNPAIRED_NONISO',        'L1_EM7_UNPAIRED_NONISO', [], ["Late"], ['RATE:MultiPhoton', 'BW:Egamma'], -1],

        # L1Topo W T&P 
        ['e13_etcut_trkcut_L1EM12', 'L1_EM12', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'], -1],
        
     
        # ATR-12916, Egamma Trigger Menu 2016   

        ['e24_lhmedium_ivarloose_L1EM20VH',      'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_nod0_L1EM20VH',           'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e24_lhmedium_nod0_ivarloose_L1EM20VH', 'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

#ATR-15233        ['e24_lhmedium_ivarloose',             'L1_EM20VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#ATR-15233        ['e24_lhmedium_nod0_iloose',           'L1_EM20VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#ATR-15233        ['e24_lhmedium_nod0_ivarloose',        'L1_EM20VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
#ATR-15233
#ATR-15233        ['e24_lhtight_ivarloose',             'L1_EM20VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e26_lhtight_ivarloose',             'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_nod0',                  'L1_EM22VHI', [], [PhysicsStream,'express'], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_nod0_ivarloose',        'L1_EM22VHI', [], [PhysicsStream,'express'], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_smooth_ivarloose',      'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e28_lhtight_ivarloose',             'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0',                  'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_ivarloose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_smooth_ivarloose',      'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        ['e30_lhtight_nod0',                  'L1_EM26VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e30_lhtight_cutd0dphideta_ivarloose','L1_EM26VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e30_lhtight_idperf',                'L1_EM26VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e30_lhtight_nod0_ivarloose',        'L1_EM26VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e30_lhtight_nod0_ringer_ivarloose', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        #Supporting
        ['e30_lhvloose_nod0_L1EM20VH',                  'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e30_lhmedium_nod0_L1EM20VH',                  'L1_EM20VH', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        
        ['e28_lhtight_nod0_L1EM22VHI',            'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e28_lhtight_nod0_ivarloose_L1EM22VHI',  'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],

        # Et cut only chains
        ['e250_etcut',                           'L1_EM22VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e300_etcut',                           'L1_EM22VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e300_etcut_L1EM24VHI',                 'L1_EM24VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        ['e300_etcut_L1EM26VHI',                 'L1_EM26VHI',  [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 

        # 2e34 single items
        ['e60_lhmedium_L1EM24VHI',      'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhmedium_nod0_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhloose_L1EM24VHI',      'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhloose_nod0_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e26_lhtight_cutd0dphideta_ivarloose', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e60_lhmedium_nod0_L1EM26VHI', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        ['e140_lhloose_nod0_L1EM26VHI', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
				
#ATR-15233        ['e24_lhmedium_L1EM20VHI',              'L1_EM20VHI',    [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],
        
        #Chains for testing 2e34 menus
        ['e32_lhtight_nod0_ivarloose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e35_lhtight_nod0_ivarloose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e40_lhtight_nod0_ivarloose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1],  
        ['e50_lhtight_nod0_ivarloose',        'L1_EM24VHI', [], [PhysicsStream], ['RATE:SingleElectron', 'BW:Egamma'],-1], 
        
        
        #AFP photons
        ['g10_loose_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g10_medium_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g15_loose_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g15_medium_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g20_loose_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g20_medium_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2g10_loose_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2g10_medium_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['3g10_loose_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g10_loose_L1EM3',                  'L1_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['g10_medium_L1EM3',                  'L1_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        #AFP electrons
        ['e10_lhloose_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e10_lhmedium_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e15_lhloose_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e15_lhmedium_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e20_lhloose_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e20_lhmedium_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2e10_lhloose_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['2e10_lhmedium_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['3e10_lhloose_nod0_L1AFP_C_EM3',                  'L1_AFP_C_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e10_lhloose_nod0_L1EM3',                  'L1_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['e10_lhmedium_nod0_L1EM3',                  'L1_EM3',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],


			 ]

    TriggerFlags.BphysicsSlice.signatures = [

        ['2mu6_nomucomb_bPhi',                     'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu11_nomucomb_mu6_nomucomb_bPhi',        'L1_MU11_2MU6', ['L1_MU11','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','mu6_nomucomb_bPhi']]],
        ['mu11_nomucomb_mu6noL1_nscan03_L1MU11_2MU6_bPhi', 'L1_MU11_2MU6', ['L1_MU11','L2_mu6_nomucomb'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','mu6noL1_nscan03_bPhi']]],

        ['mu6_mu4_bBmumux_BsmumuPhi_delayed',                    'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu6_bBmumux_BsmumuPhi_delayed',                    'L1_2MU6', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu10_mu6_bBmumux_BsmumuPhi_delayed',            'L1_MU10_2MU6', ['L1_MU10','L1_MU6'],['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],

        ['3mu4_nomucomb_bTau',            'L1_3MU4', [], [PhysicsStream], ['RATE:Bphysics', 'BW:Bphys'], -1],
        ['2mu4_bDimu',                    'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1], 
        ['2mu4_bDimu_noinvm_novtx_ss',    'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu4_bDimu_novtx_noos',         'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],  

        ['mu6_mu4_bBmumuxv2',             'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bBmumux_BcmumuDsloose', 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bBmumux_BpmumuKp',      'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],        
        ['mu6_mu4_bDimu',                 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bDimu_noinvm_novtx_ss', 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bDimu_novtx_noos',      'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bJpsimumu',             'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bUpsimumu',             'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bBmumu',                'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
	
        ['2mu6_bBmumux_BcmumuDsloose',    'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bDimu',                    'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1], 
        ['2mu6_bDimu_noinvm_novtx_ss',    'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bDimu_novtx_noos',         'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],  
        ['2mu6_bJpsimumu',                'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bUpsimumu',                'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bBmumu',                   'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bBmumuxv2', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bBmumux_BcmumuDsloose', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bBmumux_BpmumuKp',      'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bJpsimumu_Lxy0', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bJpsimumu', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bUpsimumu', 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ['mu6_mu4_bJpsimumu_Lxy0',                    'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_mu4_bBmumu_Lxy0',                    'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bDimu_Lxy0',                    'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bJpsimumu_Lxy0',                    'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bBmumu_Lxy0',                    'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
	
        ['mu4_bJpsi_Trkloose',            'L1_MU4',  [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_bJpsi_Trkloose',            'L1_MU6',  [], [PhysicsStream,'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_bJpsi_Trkloose',           'L1_MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu18_bJpsi_Trkloose',           'L1_MU15', [], [PhysicsStream,'express'], ['RATE:Bphysics','BW:Bphys'], -1],

        ['3mu4_bDimu',                    'L1_3MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['3mu4_bJpsi',                    'L1_3MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['3mu4_bTau',                     'L1_3MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['3mu4_bUpsi',                    'L1_3MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ['3mu6_bDimu',                   'L1_3MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['3mu6_bJpsi',                   'L1_3MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['3mu6_bTau',                    'L1_3MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['3mu6_bUpsi',                   'L1_3MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ['mu6_2mu4_bJpsi', 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], [PhysicsStream], ['RATE:Bphysics', 'BW:Bphys'], -1],
        ['mu6_2mu4_bUpsi', 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], [PhysicsStream], ['RATE:Bphysics', 'BW:Bphys'], -1],
		
        ['mu4_iloose_mu4_11invm60_noos', 'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_iloose_mu4_7invm9_noos',   'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_iloose_mu6_11invm24_noos', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_iloose_mu6_24invm60_noos', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ### ATR-14543
        ['mu4_ivarloose_mu4_11invm60_noos', 'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_ivarloose_mu4_7invm9_noos',   'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_ivarloose_mu6_11invm24_noos', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_ivarloose_mu6_24invm60_noos', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ['mu4_iloose_mu4_11invm60_noos_novtx', 'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_iloose_mu4_7invm9_noos_novtx',   'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_iloose_mu6_11invm24_noos_novtx', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_iloose_mu6_24invm60_noos_novtx', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ### ATR-14543
        ['mu4_ivarloose_mu4_11invm60_noos_novtx', 'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_ivarloose_mu4_7invm9_noos_novtx',   'L1_2MU4', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_ivarloose_mu6_11invm24_noos_novtx', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_ivarloose_mu6_24invm60_noos_novtx', 'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ### ATR-14616: L1Topo SM DY
        ['mu6_iloose_mu6_11invm24_noos_L1DY-BOX-2MU6',       'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_iloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_iloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_iloose_mu4_11invm24_noos_L1DY-BOX-2MU4',       'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_iloose_mu4_11invm24_noos_novtx_L1DY-BOX-2MU4', 'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_iloose_mu4_24invm60_noos_L1DY-BOX-2MU4',       'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_iloose_mu4_24invm60_noos_novtx_L1DY-BOX-2MU4', 'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ### ATR-14543
        ['mu6_ivarloose_mu6_11invm24_noos_L1DY-BOX-2MU6',       'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_ivarloose_mu6_11invm24_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu6_ivarloose_mu6_24invm60_noos_novtx_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_ivarloose_mu4_11invm24_noos_L1DY-BOX-2MU4',       'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_ivarloose_mu4_11invm24_noos_novtx_L1DY-BOX-2MU4', 'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_ivarloose_mu4_24invm60_noos_L1DY-BOX-2MU4',       'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
#        ['mu4_ivarloose_mu4_24invm60_noos_novtx_L1DY-BOX-2MU4', 'L1_DY-BOX-2MU4', ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ['mu4_mu4_idperf_bJpsimumu_noid',  'L1_2MU4', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring','BW:Bphys', 'BW:ID'], -1],  
        ['2mu6_bBmumux_BpmumuKp',    'L1_2MU6',     [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bBmumuxv2',              'L1_2MU6', [], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu13_mu13_idperf_Zmumu',     'L1_2MU10', [], [PhysicsStream, 'express'], ['RATE:IDMonitoring','BW:Bphys', 'BW:ID'], -1],
        ['2mu10_bDimu_noL2',                    'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu10_bJpsimumu_noL2',                    'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ['2mu10_bBmumuxv2',               'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu10_bBmumux_BcmumuDsloose',   'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu10_bDimu',                   'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu10_bDimu_novtx_noos',        'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu10_bJpsimumu',               'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu10_bUpsimumu',               'L1_2MU10', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],


        ['mu11_nomucomb_2mu4noL1_nscan03_L1MU11_2MU6_bTau', 'L1_MU11_2MU6', ['L1_MU11','L2_mu11_nomucomb'], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','2mu4noL1_nscan03']]],
        ['mu11_nomucomb_mu6noL1_nscan03_L1MU11_2MU6_bTau', 'L1_MU11_2MU6', ['L1_MU11','L2_mu6_nomucomb'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','mu6noL1_nscan03']]],
        ['mu20_nomucomb_mu6noL1_nscan03_bTau', 'L1_MU20', ['L1_MU20','L2_mu20_nomucomb'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1,['serial',-1,['mu20_nomucomb','mu6noL1_nscan03']]],
        ## LFV topo based nscan (ATR-14352)
        ['mu11_nomucomb_2mu4noL1_nscan03_L1LFV-MU_bTau', 'L1_LFV-MU', ['L1_MU10','L2_mu11_nomucomb'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','2mu4noL1_nscan03']]],      

        ['mu6_nomucomb_2mu4_nomucomb_bTau_L1MU6_3MU4' , 'L1_MU6_3MU4',  ['L1_MU6','L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_nomucomb_mu4_nomucomb_bTau_L12MU6_3MU4', 'L1_2MU6_3MU4', ['L1_2MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        
        ['mu6_iloose_mu6_24invm60_noos_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu4_iloose_mu4_11invm60_noos_L1MU6_2MU4',    'L1_MU6_2MU4',    ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_iloose_mu4_7invm9_noos_L1MU6_2MU4',           'L1_MU6_2MU4',                         ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_iloose_mu4_11invm60_noos_novtx_L1MU6_2MU4',           'L1_MU6_2MU4',                         ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_iloose_mu4_7invm9_noos_novtx_L1MU6_2MU4',           'L1_MU6_2MU4',                         ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ### ATR-14543
        ['mu6_ivarloose_mu6_24invm60_noos_L1DY-BOX-2MU6', 'L1_DY-BOX-2MU6', ['L1_MU6','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu4_ivarloose_mu4_11invm60_noos_L1MU6_2MU4',    'L1_MU6_2MU4',    ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_ivarloose_mu4_7invm9_noos_L1MU6_2MU4',           'L1_MU6_2MU4',                         ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_ivarloose_mu4_11invm60_noos_novtx_L1MU6_2MU4',           'L1_MU6_2MU4',                         ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu4_ivarloose_mu4_7invm9_noos_novtx_L1MU6_2MU4',           'L1_MU6_2MU4',                         ['L1_MU4','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ### ATR-14478
        ['2mu6_bBmumux_Taumumux',         'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu6_bBmumux_Taumumux_noL2',    'L1_2MU6', [], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bBmumux_Taumumux',     'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['mu10_mu6_bBmumux_Taumumux_noL2','L1_MU10_2MU6', ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],


        ### ATR-14350: L1Topo bphysics chains
        # 50% bckg rejection WP
        #['2mu4_bDimu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',      'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bDimu_novtx_noos_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',      'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bJpsimumu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',     'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bUpsimumu_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumuxv2_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # 71% bckg rejection WP
        ['2mu4_bDimu_L1BPH-2M8-2MU4',    'L1_BPH-2M8-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bDimu_novtx_noos_L1BPH-2M8-2MU4',    'L1_BPH-2M8-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bJpsimumu_L1BPH-2M8-2MU4',  'L1_BPH-2M8-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumu_L1BPH-2M8-2MU4',     'L1_BPH-2M8-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumuxv2_L1BPH-2M8-2MU4',  'L1_BPH-2M8-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-2MU4',  'L1_BPH-2M8-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bDimu_L1BPH-8M15-MU6MU4',     'L1_BPH-8M15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L1BPH-8M15-MU6MU4',     'L1_BPH-8M15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bUpsimumu_L1BPH-8M15-MU6MU4', 'L1_BPH-8M15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # 86% bckg rejection WP
        ['mu6_mu4_bDimu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',      'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',      'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumu_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumuxv2_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # 96% bckg rejection WP
        ['2mu6_bDimu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',      'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bDimu_novtx_noos_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',      'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bJpsimumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumu_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',     'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumuxv2_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumux_BcmumuDsloose_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # 95% bckg rejection WP (for Upsilon only)
        ['2mu6_bDimu_L1BPH-8M15-2MU6',     'L1_BPH-8M15-2MU6',     ['L1_2MU6'], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bDimu_novtx_noos_L1BPH-8M15-2MU6',     'L1_BPH-8M15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bUpsimumu_L1BPH-8M15-2MU6', 'L1_BPH-8M15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu4-OneBarrel
        ['2mu4_bDimu_L12MU4-B',      'L1_2MU4-B',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bDimu_novtx_noos_L12MU4-B',      'L1_2MU4-B',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bJpsimumu_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumu_L12MU4-B',     'L1_2MU4-B',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bUpsimumu_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumuxv2_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumux_BcmumuDsloose_L12MU4-B',  'L1_2MU4-B',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu4-BarrelOnly
        ['2mu4_bDimu_L12MU4-BO',      'L1_2MU4-BO',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bDimu_novtx_noos_L12MU4-BO',      'L1_2MU4-BO',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bJpsimumu_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumu_L12MU4-BO',     'L1_2MU4-BO',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bUpsimumu_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumuxv2_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu4_bBmumux_BcmumuDsloose_L12MU4-BO',  'L1_2MU4-BO',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # Simple OneBarrel and BarrelOnly: mu6_mu4-OneBarrel
        ['mu6_mu4_bDimu_L12MU4-B',      'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L12MU4-B',      'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumu_L12MU4-B',     'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bUpsimumu_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumuxv2_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumux_BcmumuDsloose_L12MU4-B',  'L1_MU6_2MU4-B',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # Simple OneBarrel and BarrelOnly: mu6_mu4-BarrelOnly
        ['mu6_mu4_bDimu_L1MU6MU4-BO',      'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L1MU6MU4-BO',      'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumu_L1MU6MU4-BO',     'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bUpsimumu_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumuxv2_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumux_BcmumuDsloose_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu6-OneBarrel
        ['2mu6_bDimu_L12MU6-B',      'L1_2MU6-B',     ['L1_2MU6'], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bDimu_novtx_noos_L12MU6-B',      'L1_2MU6-B',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bJpsimumu_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumu_L12MU6-B',     'L1_2MU6-B',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bUpsimumu_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumuxv2_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumux_BcmumuDsloose_L12MU6-B',  'L1_2MU6-B',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # Simple OneBarrel and BarrelOnly: 2mu6-BarrelOnly
        ['2mu6_bDimu_L12MU6-BO',      'L1_2MU6-BO',     ['L1_2MU6'], [PhysicsStream, 'express'], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bDimu_novtx_noos_L12MU6-BO',      'L1_2MU6-BO',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bJpsimumu_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumu_L12MU6-BO',     'L1_2MU6-BO',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bUpsimumu_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumuxv2_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumux_BcmumuDsloose_L12MU6-BO',  'L1_2MU6-BO',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # OneBarrel or BarrelOnly: 50% bckg rejection WP + OneBarrel
        #['2mu4_bDimu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4',       'L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bJpsimumu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4',     'L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bUpsimumu_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumuxv2_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False], 
        # OneBarrel or BarrelOnly: 50% bckg rejection WP + BarrelOnly
        #['2mu4_bDimu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',      'L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bDimu_novtx_noos_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',      'L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bJpsimumu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     'L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bUpsimumu_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumuxv2_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        #['2mu4_bBmumux_BcmumuDsloose_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',  'L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        # OneBarrel or BarrelOnly: 86% bckg rejection WP + OneBarrel
        ['mu6_mu4_bDimu_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',      'L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bDimu_novtx_noos_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',      'L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumu_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',     'L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumuxv2_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumux_BcmumuDsloose_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False], 
        # For semileptonic modes
        ['mu10_mu6_bBmumuxv2_L1LFV-MU',  'L1_LFV-MU',     ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu10_mu6_bBmumux_BcmumuDsloose_L1LFV-MU',  'L1_LFV-MU',     ['L1_MU10','L1_MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],

# ATR-14350
        ['2mu4_bBmumu_Lxy0_L1BPH-2M8-2MU4',      'L1_BPH-2M8-2MU4',     ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumu_Lxy0_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', 'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumux_BsmumuPhi_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', 'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_Lxy0_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', 'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bBmumux_BsmumuPhi_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['2mu6_bJpsimumu_Lxy0_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False],
        ['mu10_mu6_bBmumux_BsmumuPhi',            'L1_MU10_2MU6', ['L1_MU10','L1_MU6'],[PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],

        ### ATR-15050
        ['2mu6_bBmumux_BsmumuPhi',  'L1_2MU6', ['L1_2MU6'],[PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1],
        ['2mu4_bBmumux_BsmumuPhi_L1BPH-2M8-2MU4', 'L1_BPH-2M8-2MU4', ['L1_2MU4'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1, False], 


# Delayed stream ATR-14350
        ['mu6_mu4_bBmumuxv2_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],	
        ['mu6_mu4_bBmumux_BsmumuPhi_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', 'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumux_BcmumuDsloose_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',  'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_Lxy0_delayed_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4', 'L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu6_mu4_bJpsimumu_delayed_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu6_mu4_bBmumuxv2_delayed_L1MU6MU4-BO',  'L1_MU6MU4-BO',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu6_mu4_bUpsimumu_delayed_L1BPH-8M15-MU6MU4', 'L1_BPH-8M15-MU6MU4',     ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu6_bBmumuxv2_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu6_bBmumux_BsmumuPhi_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu6_bBmumux_BcmumuDsloose_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu6_bJpsimumu_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6',  'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu6_bJpsimumu_Lxy0_delayed_L1BPH-2M9-2MU6_BPH-2DR15-2MU6', 'L1_BPH-2M9-2MU6_BPH-2DR15-2MU6',     ['L1_2MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu6_bUpsimumu_delayed_L1BPH-8M15-2MU6', 'L1_BPH-8M15-2MU6',     ['L1_2MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu10_mu6_bBmumuxv2_delayed_L1LFV-MU',  'L1_LFV-MU',     ['L1_MU10','L1_MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['mu10_mu6_bBmumux_BcmumuDsloose_delayed_L1LFV-MU',  'L1_LFV-MU',     ['L1_MU10','L1_MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],

        ### ATR-15050
        ['2mu4_bBmumux_BsmumuPhi_delayed_L1BPH-2M8-2MU4', 'L1_BPH-2M8-2MU4',     ['L1_2MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu4_bJpsimumu_Lxy0_delayed_L1BPH-2M8-2MU4', 'L1_BPH-2M8-2MU4',     ['L1_2MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],
        ['2mu4_bJpsimumu_delayed_L1BPH-2M8-2MU4', 'L1_BPH-2M8-2MU4',     ['L1_2MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1, False],


				#Delayed stream ATR-14091
        ['mu6_mu4_bBmumuxv2_delayed',             								 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu6_mu4_bBmumux_BcmumuDsloose_delayed', 								 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu6_mu4_bJpsimumu_delayed',             								 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu6_mu4_bJpsimumu_Lxy0_delayed',        								 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu6_mu4_bUpsimumu_delayed',             								 'L1_MU6_2MU4', ['L1_MU6','L1_MU4'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu6_bBmumuxv2_delayed',              									 'L1_2MU6', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu6_bBmumux_BcmumuDsloose_delayed',    								 'L1_2MU6', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu6_bJpsimumu_delayed',                								 'L1_2MU6', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu6_bJpsimumu_Lxy0_delayed',           								 'L1_2MU6', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu6_bUpsimumu_delayed',                								 'L1_2MU6', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu10_mu6_bBmumuxv2_delayed',            								 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu10_mu6_bBmumux_BcmumuDsloose_delayed',								 'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu10_mu6_bJpsimumu_delayed',                             'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu10_mu6_bJpsimumu_Lxy0_delayed',                        'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu10_mu6_bUpsimumu_delayed',                             'L1_MU10_2MU6', ['L1_MU10','L1_MU6'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['3mu4_bJpsi_delayed',     									               'L1_3MU4', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['3mu4_bUpsi_delayed',     									               'L1_3MU4', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['mu6_2mu4_bJpsi_delayed', 																 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], ['BphysDelayed'], ['RATE:BphysDelayed', 'BW:Bphys'], -1],
        ['mu6_2mu4_bUpsi_delayed', 																 'L1_MU6_3MU4', ['L1_MU6','L1_3MU4'], ['BphysDelayed'], ['RATE:BphysDelayed', 'BW:Bphys'], -1],
        ['mu11_nomucomb_mu6noL1_nscan03_L1MU11_2MU6_bTau_delayed', 'L1_MU11_2MU6', ['L1_MU11','L2_mu6_nomucomb'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','mu6noL1_nscan03']]],
        #moved to muons slice ['mu6_nomucomb_2mu4_nomucomb_delayed_L1MU6_3MU4',          'L1_MU6_3MU4',  ['L1_MU6','L1_2MU4'],          ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu10_bBmumux_BcmumuDsloose_delayed',                    'L1_2MU10', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu10_bJpsimumu_delayed',                                'L1_2MU10', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu10_bUpsimumu_delayed',                                'L1_2MU10', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],
        ['2mu10_bBmumuxv2_delayed',                                'L1_2MU10', [], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1],

        ## LFV topo based *delayed* nscan (ATR-14352)
        ## original proposed item: ['mu11_nomucomb_mu6noL1_nscan03_L1LFV-MU_bTau', 'L1_LFV-MU', ['L1_MU10','L2_mu6_nomucomb'], [PhysicsStream], ['RATE:Bphysics','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','mu6noL1_nscan03']]],
        ['mu11_nomucomb_mu6noL1_nscan03_L1LFV-MU_bTau_delayed', 'L1_LFV-MU', ['L1_MU10','L2_mu6_nomucomb'], ['BphysDelayed'], ['RATE:BphysDelayed','BW:Bphys'], -1,['serial',-1,['mu11_nomucomb','mu6noL1_nscan03']]],
			 ]

    TriggerFlags.CombinedSlice.signatures = [

#        ['2j55_bmv2c2060_split_ht300_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['2j55_bmv2c2060_split', 'ht300_L14J15']]],
#        ['2j55_bmv2c2070_split_ht300_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['2j55_bmv2c2070_split', 'ht300_L14J15']]],
#        ['2j55_bmv2c2050_split_ht300_L14J15',        'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['2j55_bmv2c2050_split', 'ht300_L14J15']]],
#        ['j55_bmv2c2060_ht500_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[  'j55_bmv2c2060', 'ht500_L14J15']]],
#        ['j55_bmv2c2050_ht500_L14J15',               'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[ 'j55_bmv2c2050', 'ht500_L14J15']]],
#
#        ['j55_bmv2c2060_split_ht500_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[  'j55_bmv2c2060_split', 'ht500_L14J15',]]],
#        ['j55_bmv2c2050_split_ht500_L14J15',               'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'],  -1,['serial',-1,[ 'j55_bmv2c2050_split', 'ht500_L14J15']]],

        ['2j55_bmv2c2060_split_ht300_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300_L14J15','2j55_bmv2c2060_split']]],
        ['2j55_bmv2c2070_split_ht300_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300_L14J15','2j55_bmv2c2070_split']]],
        ['2j55_bmv2c2050_split_ht300_L14J15',        'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300_L14J15','2j55_bmv2c2050_split']]],

        ['2j55_bmv2c2050_split_ht300_L14J20',        'L1_4J20', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300_L14J20','2j55_bmv2c2050_split']]],

        ['2j55_bmv2c2050_split_ht300_L1HT190-J15.ETA21',        'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300','2j55_bmv2c2050_split']]],

        ['2j55_bmv2c2050_split_ht300_L1HT190-J15s5.ETA21',        'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300','2j55_bmv2c2050_split']]],


        ['j55_bmv2c2060_ht500_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[  'ht500_L14J15','j55_bmv2c2060']]],
        ['j55_bmv2c2050_ht500_L14J15',               'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[ 'ht500_L14J15','j55_bmv2c2050']]],

        ['j55_bmv2c2050_split_ht500_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[ 'ht500', 'j55_bmv2c2050_split',]]],

        ['j55_bmv2c2050_split_ht500_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[ 'ht500', 'j55_bmv2c2050_split',]]],


        # ATR-15226
        ['j55_boffperf_split_ht500_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[  'j55_boffperf_split', 'ht500_L14J15']]], 

        ['2j55_boffperf_split_ht300_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,[  '2j55_boffperf_split', 'ht300_L14J15']]], 

        ['j55_boffperf_split_ht500_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[  'j55_boffperf_split', 'ht500']]], 

        ['2j55_boffperf_split_ht300_L1HT190-J15.ETA21', 'L1_HT190-J15.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,[  '2j55_boffperf_split', 'ht300']]], 

        ['j55_boffperf_split_ht500_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[  'j55_boffperf_split', 'ht500']]], 

        ['2j55_boffperf_split_ht300_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,[  '2j55_boffperf_split', 'ht300']]], 

        ['j55_bmv2c2060_split_ht500_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[ 'ht500_L14J15', 'j55_bmv2c2060_split',]]],
        ['j55_bmv2c2050_split_ht500_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'],  -1,['serial',-1,[ 'ht500_L14J15','j55_bmv2c2050_split']]],
        ['j55_bmv2c2050_split_ht500_L14J20', 'L1_4J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'],  -1,['serial',-1,[ 'ht500_L14J20','j55_bmv2c2050_split']]],



#        ['j55_bmv2c2060_ht500_L14J20', 'L1_4J20', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,['j55_bmv2c2060', 'ht500_L14J20']]],
      
        ## ATR-14355: L1Topo bjet/HT items
        ['j55_bmv2c2060_split_ht500_L1HT150-J20s5.ETA31', 'L1_HT150-J20s5.ETA31', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,['ht500','j55_bmv2c2060_split']]],
        ['j55_bmv2c2060_split_ht500_L1HT150-J20.ETA31',   'L1_HT150-J20.ETA31',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,[ 'ht500','j55_bmv2c2060_split']]],
        ['j55_bmv2c2060_split_ht500_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,['ht500','j55_bmv2c2060_split']]],
        ['j55_bmv2c2060_split_ht500_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:SingleBJet', 'BW:BJet'], -1,['serial',-1,['ht500','j55_bmv2c2060_split']]],
        ['2j55_bmv2c2070_split_ht300_L1HT150-J20s5.ETA31', 'L1_HT150-J20s5.ETA31', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300','2j55_bmv2c2070_split']]],
        ['2j55_bmv2c2070_split_ht300_L1HT150-J20.ETA31',   'L1_HT150-J20.ETA31',   [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300','2j55_bmv2c2070_split']]],
        ['2j55_bmv2c2070_split_ht300_L1HT190-J15.ETA21',   'L1_HT190-J15.ETA21',   [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300','2j55_bmv2c2070_split']]],
        ['2j55_bmv2c2070_split_ht300_L1HT190-J15s5.ETA21', 'L1_HT190-J15s5.ETA21', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'], -1,['serial',-1,['ht300','2j55_bmv2c2070_split']]],

        ### ATR-14356: bjets+HT VBF chians
        ['2j45_bmv2c2060_split_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_HT150-JJ15.ETA49_MJJ-400', [], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet'],-1,['serial',-1,['2j45_bmv2c2060_split', '2j35_0eta490_invm700']]],

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


        ['2e12_lhloose_mu10',       'L1_2EM8VH_MU10', ['L1_2EM8VH', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['2e12_lhloose_nod0_mu10',  'L1_2EM8VH_MU10', ['L1_2EM8VH', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['2e12_lhmedium_mu10',      'L1_2EM8VH_MU10', ['L1_2EM8VH', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['2e12_lhmedium_nod0_mu10', 'L1_2EM8VH_MU10', ['L1_2EM8VH', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['e12_lhloose_2mu10',       'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['e12_lhloose_nod0_2mu10',  'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['e12_lhmedium_2mu10',      'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['e12_lhmedium_nod0_2mu10', 'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],


        ['e17_lhloose_mu14',        'L1_EM15VH_MU10', ['L1_EM15VH', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['e17_lhloose_nod0_mu14',   'L1_EM15VH_MU10', ['L1_EM15VH', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],

        ['e24_lhmedium_L1EM20VHI_mu8noL1',      'L1_EM20VHI', ['L1_EM20VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e24_lhmedium_L1EM20VHI','mu8noL1'] ]],
        ['e24_lhmedium_nod0_L1EM20VHI_mu8noL1', 'L1_EM20VHI', ['L1_EM20VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e24_lhmedium_nod0_L1EM20VHI','mu8noL1'] ]],


        ['e26_lhmedium_L1EM22VHI_mu8noL1',      'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e26_lhmedium_L1EM22VHI','mu8noL1'] ]],
        ['e26_lhmedium_nod0_L1EM22VHI_mu8noL1', 'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e26_lhmedium_nod0_L1EM22VHI','mu8noL1'] ]],
        ['e28_lhmedium_nod0_L1EM24VHI_mu8noL1', 'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e28_lhmedium_nod0_L1EM24VHI','mu8noL1'] ]],
        ['e30_lhmedium_nod0_L1EM26VHI_mu8noL1', 'L1_EM26VHI', ['L1_EM26VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['e30_lhmedium_nod0_L1EM26VHI','mu8noL1'] ]],
        ['e28_lhmedium_nod0_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1],
        ['e30_lhmedium_nod0_L1EM26VHI', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1],

        ['e60_lhloose_xe60noL1',         'L1_EM22VHI',['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e60_lhloose","xe60noL1"]]],
        ['e60_lhloose_nod0_xe60noL1',    'L1_EM22VHI',['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e60_lhloose_nod0","xe60noL1"]]],
        ['e70_lhloose_xe70noL1',         'L1_EM22VHI',['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e70_lhloose","xe70noL1"]]],
        ['e70_lhloose_nod0_xe70noL1',    'L1_EM22VHI',['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e70_lhloose_nod0","xe70noL1"]]],
        ['e60_lhloose_nod0_L1EM26VHI_xe60noL1',    'L1_EM26VHI',['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e60_lhloose_nod0_L1EM26VHI","xe60noL1"]]],
        ['e70_lhloose_nod0_L1EM26VHI_xe70noL1',    'L1_EM26VHI',['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e70_lhloose_nod0_L1EM26VHI","xe70noL1"]]],
        ['e80_lhloose_nod0_xe80noL1',    'L1_EM22VHI',['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e80_lhloose_nod0","xe80noL1"]]],
        ['e80_lhloose_nod0_L1EM24VHI_xe80noL1',    'L1_EM24VHI',['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e80_lhloose_nod0_L1EM24VHI","xe80noL1"]]],
        ['e80_lhloose_nod0_L1EM26VHI_xe80noL1',    'L1_EM26VHI',['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["e80_lhloose_nod0_L1EM26VHI","xe80noL1"]]],


        ['e7_lhmedium_mu24',             'L1_MU20', ['L1_EM3', 'L1_MU20'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],
        ['e7_lhmedium_nod0_mu24',        'L1_MU20', ['L1_EM3', 'L1_MU20'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1,['parallel',-1,[] ]],

        ['g40_tight_xe40noL1',           'L1_EM20VH',  ['L1_EM20VH',''],  [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g40_tight","xe40noL1"]]],
        ['g45_tight_xe45noL1',           'L1_EM20VH',  ['L1_EM20VH',''],  [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight","xe45noL1"]]],
        ['g45_tight_L1EM22VHI_xe45noL1', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_L1EM22VHI","xe45noL1"]]],
        ['g45_tight_L1EM24VHI_xe45noL1', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_L1EM24VHI","xe45noL1"]]],
        ['g45_tight_L1EM26VHI_xe45noL1', 'L1_EM26VHI', ['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g45_tight_L1EM26VHI","xe45noL1"]]],
        ['g60_loose_xe60noL1',           'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g60_loose","xe60noL1"]]],
        ['g70_loose_xe70noL1',           'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g70_loose","xe70noL1"]]],
        ['g60_loose_L1EM24VHI_xe60noL1',           'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g60_loose_L1EM24VHI","xe60noL1"]]],
        ['g70_loose_L1EM24VHI_xe70noL1',           'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g70_loose_L1EM24VHI","xe70noL1"]]],
        ['g80_loose_L1EM24VHI_xe80noL1',           'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose_L1EM24VHI","xe80noL1"]]],
        ['g60_loose_L1EM26VHI_xe60noL1',           'L1_EM26VHI', ['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g60_loose_L1EM26VHI","xe60noL1"]]],
        ['g70_loose_L1EM26VHI_xe70noL1',           'L1_EM26VHI', ['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g70_loose_L1EM26VHI","xe70noL1"]]],
        ['g80_loose_L1EM26VHI_xe80noL1',           'L1_EM26VHI', ['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose_L1EM26VHI","xe80noL1"]]],
        ['g45_tight_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g45_tight_L1EM26VHI', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        #ATR-14196
        ['g80_loose_xe80noL1',           'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaMET', 'BW:Egamma', 'BW:MET'], -1,['serial',-1,["g80_loose","xe80noL1"]]],

        ['g25_medium_mu24',              'L1_MU20', ['L1_EM15VH','L1_MU20'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,[] ]],

        ['g15_loose_2mu10_msonly',       'L1_2MU10', ['L1_EM8VH', 'L1_2MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,[] ]],

        ['j100_xe80',                    'L1_J40_XE50',['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet', 'BW:MET'], -1,['serial',-1,["j100","xe80"]]],

        ### ATR-14351
        ['j100_xe110_mht_1dphi10_L1J40_DPHI-J20s2XE50', 'L1_J40_DPHI-J20s2XE50', [], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1,['serial',-1,["j100","xe110_mht"]]],
        
        ['mu4_j100_xe80_mht',  'L1_MU4_J50_XE40', ['L1_MU4','L1_J50', 'L1_XE40'], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu4","j100","xe80_mht"] ]],
        ['2mu4_j80_xe40_mht',  'L1_2MU4_J40_XE20', ['L1_2MU4','L1_J40', 'L1_XE20'], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4","j80","xe40_mht"] ]],

	# ATR-14717
        ['mu4_j125_xe90_mht',  'L1_MU4_J50_XE40', ['L1_MU4','L1_J50', 'L1_XE40'], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu4","j125","xe90_mht"] ]],
        ['2mu4_j85_xe50_mht',  'L1_2MU4_J40_XE20', ['L1_2MU4','L1_J40', 'L1_XE20'], [PhysicsStream], ['RATE:MuonJetMET', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["2mu4","j85","xe50_mht"] ]],

        ['mu4_j15_bperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', 'j15_bperf_split'] ]],
        ['mu4_j25_bperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', 'j25_bperf_split'] ]],
        ['mu4_j35_bperf_split', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', 'j35_bperf_split'] ]],
        ['mu4_j35_bperf_split_dr05_dz02', 'L1_MU4',     ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', 'j35_bperf_split'] ]],
        ['mu4_j55_bperf_split_dr05_dz02',  'L1_MU4_J12', ['L1_MU4', ''], [PhysicsStream, 'express'], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', 'j55_bperf_split'] ]],
        ['mu6_j85_bperf_split_dr05_dz02',  'L1_MU6_J20', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j85_bperf_split'] ]],
        ['mu6_j110_bperf_split_dr05_dz02', 'L1_MU6_J20', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j110_bperf_split'] ]],
        ['mu6_j150_bperf_split_dr05_dz02', 'L1_MU6_J40', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j150_bperf_split'] ]],

        ['mu6_j150_bperf_split', 'L1_MU6_J40',     ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j150_bperf_split'] ]],
        ['mu6_j175_bperf_split_dr05_dz02', 'L1_MU6_J40', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j175_bperf_split']]],
        ['mu6_j260_bperf_split_dr05_dz02', 'L1_MU6_J75', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j260_bperf_split'] ]],
        ['mu6_j320_bperf_split_dr05_dz02', 'L1_MU6_J75', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j320_bperf_split'] ]],

        ['mu6_j320_bperf_split', 'L1_MU6_J75',     ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j320_bperf_split'] ]],
        ['mu6_j400_bperf_split_dr05_dz02', 'L1_MU6_J75', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j400_bperf_split'] ]],

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
        ['mu4_2j40_dr05_j40_bmv2c2077_split_antimatchdr05mu_L13J15_BTAG-MU4J15', 'L1_3J15_BTAG-MU4J15', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '2j40', 'j40_bmv2c2077_split_antimatchdr05mu'] ], False],
        ['mu4_2j35_dr05_j35_bmv2c2070_split_antimatchdr05mu_L13J15_BTAG-MU4J15', 'L1_3J15_BTAG-MU4J15', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '2j35', 'j35_bmv2c2070_split_antimatchdr05mu'] ], False],
        ['mu4_2j40_dr05_j40_bmv2c2077_split_antimatchdr05mu_L13J20_BTAG-MU4J20', 'L1_3J20_BTAG-MU4J20', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '2j40', 'j40_bmv2c2077_split_antimatchdr05mu'] ], False],
        ['mu4_2j35_dr05_j35_bmv2c2070_split_antimatchdr05mu_L13J20_BTAG-MU4J20', 'L1_3J20_BTAG-MU4J20', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '2j35', 'j35_bmv2c2070_split_antimatchdr05mu'] ], False],
        ['mu4_3j45_dr05_L13J15_BTAG-MU4J15', 'L1_3J15_BTAG-MU4J15', ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '3j45'] ], False],
        ['mu4_3j45_dr05_L13J20_BTAG-MU4J20', 'L1_3J20_BTAG-MU4J20', ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '3j45'] ], False],

        ['mu4_j35_bperf_split_dr05_dz02_L1BTAG-MU4J15', 'L1_BTAG-MU4J15', ['L1_MU4', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', 'j35_bperf_split'] ], False],
        ['mu6_j85_bperf_split_dr05_dz02_L1BTAG-MU6J20', 'L1_BTAG-MU6J20', ['L1_MU6', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu6', 'j85_bperf_split'] ], False],
 
        # BJet signal Perf chains

        # bperf 
        ['2mu14_2j35_bperf',              'L1_2MU10',   ['L1_2MU10', ''],   [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['2mu14', '2j35_bperf']]],
        ['2e17_loose_2j35_bperf',         'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_loose', '2j35_bperf']]],
        ['2e17_lhloose_2j35_bperf',       'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_lhloose', '2j35_bperf']]],
        ['mu26_imedium_2j35_bperf',       'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_imedium', '2j35_bperf']]],
        ### ATR-14543
        ['mu26_ivarmedium_2j35_bperf',    'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_ivarmedium', '2j35_bperf']]],
        ['e26_tight_iloose_2j35_bperf',   'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_tight_iloose', '2j35_bperf']]],
        ['e26_lhtight_nod0_ivarloose_2j35_bperf', 'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_lhtight_nod0_ivarloose', '2j35_bperf']]],
        ['e28_lhtight_nod0_ivarloose_2j35_bperf', 'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e28_lhtight_nod0_ivarloose', '2j35_bperf']]],
        ['e28_lhtight_nod0_ivarloose_2j35_bperf_split', 'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e28_lhtight_nod0_ivarloose', '2j35_bperf_split']]],
        ['e28_lhtight_nod0_ivarloose_2j35_boffperf', 'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e28_lhtight_nod0_ivarloose', '2j35_boffperf']]],
        ['e30_lhtight_nod0_ivarloose_2j35_bperf', 'L1_EM26VHI', ['L1_EM26VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e30_lhtight_nod0_ivarloose', '2j35_bperf']]],
        ['e30_lhtight_nod0_ivarloose_2j35_bperf_split', 'L1_EM26VHI', ['L1_EM26VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e30_lhtight_nod0_ivarloose', '2j35_bperf_split']]],
        ['e30_lhtight_nod0_ivarloose_2j35_boffperf', 'L1_EM26VHI', ['L1_EM26VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e30_lhtight_nod0_ivarloose', '2j35_boffperf']]],
        ['e30_lhtight_nod0_ivarloose_2j35_boffperf_split', 'L1_EM26VHI', ['L1_EM26VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e30_lhtight_nod0_ivarloose', '2j35_boffperf_split']]],


        # bperf_split
        ['2mu14_2j35_bperf_split',              'L1_2MU10',   ['L1_2MU10', ''],   [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['2mu14', '2j35_bperf_split']]],
        ['2e17_loose_2j35_bperf_split',         'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_loose', '2j35_bperf_split']]],
        ['2e17_lhloose_2j35_bperf_split',       'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_lhloose', '2j35_bperf_split']]],
        ['mu26_imedium_2j35_bperf_split',       'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_imedium', '2j35_bperf_split']]],
        ### ATR-14543
        ['mu26_ivarmedium_2j35_bperf_split',    'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_ivarmedium', '2j35_bperf_split']]],
        ['e26_tight_iloose_2j35_bperf_split',   'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_tight_iloose', '2j35_bperf_split']]],
        ['e26_lhtight_nod0_ivarloose_2j35_bperf_split', 'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_lhtight_nod0_ivarloose', '2j35_bperf_split']]],

        # boffperf 
        ['2mu14_2j35_boffperf',              'L1_2MU10',   ['L1_2MU10', ''],   [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['2mu14', '2j35_boffperf']]],
        ['2e17_loose_2j35_boffperf',         'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_loose', '2j35_boffperf']]],
        ['2e17_lhloose_2j35_boffperf',       'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_lhloose', '2j35_boffperf']]],
        ['mu26_imedium_2j35_boffperf',       'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_imedium', '2j35_boffperf']]],
        ### ATR-14543
        ['mu26_ivarmedium_2j35_boffperf',       'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_ivarmedium', '2j35_boffperf']]],
        ['e26_tight_iloose_2j35_boffperf',   'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_tight_iloose', '2j35_boffperf']]],
        ['e26_lhtight_nod0_ivarloose_2j35_boffperf', 'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_lhtight_nod0_ivarloose', '2j35_boffperf']]],

        # boffperf_split
        ['2mu14_2j35_boffperf_split',              'L1_2MU10',   ['L1_2MU10', ''],   [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['2mu14', '2j35_boffperf_split']]],
        ['2e17_loose_2j35_boffperf_split',         'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_loose', '2j35_boffperf_split']]],
        ['2e17_lhloose_2j35_boffperf_split',       'L1_2EM15VH', ['L1_2EM15VH', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['2e17_lhloose', '2j35_boffperf_split']]],
        ['mu26_imedium_2j35_boffperf_split',       'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_imedium', '2j35_boffperf_split']]],
        ### ATR-14543
        ['mu26_ivarmedium_2j35_boffperf_split',    'L1_MU20',    ['L1_MU20', ''],    [PhysicsStream], ['RATE:MuonBJet',  'BW:BJet'],-1,['serial',-1,['mu26_ivarmedium', '2j35_boffperf_split']]],
        ['e26_tight_iloose_2j35_boffperf_split',   'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_tight_iloose', '2j35_boffperf_split']]],
        ['e26_lhtight_nod0_ivarloose_2j35_boffperf_split', 'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e26_lhtight_nod0_ivarloose', '2j35_boffperf_split']]],
        ['e28_lhtight_nod0_ivarloose_2j35_boffperf_split', 'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet'],-1,['serial',-1,['e28_lhtight_nod0_ivarloose', '2j35_boffperf_split']]],



        # Wgg triggers

        ['e20_lhmedium_2g10_loose',  'L1_EM15VH_3EM7', ['L1_EM15VH','L1_2EM7'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['e20_lhmedium_2g10_medium', 'L1_EM15VH_3EM7', ['L1_EM15VH','L1_2EM7'], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1,['parallel',-1,[] ]],
        ['2g10_loose_mu20',          'L1_MU20',        ['L1_2EM7', 'L1_MU20'],  [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['2g10_loose','mu20'] ]],
        ['2g10_medium_mu20',         'L1_MU20',        ['L1_2EM7', 'L1_MU20'],  [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['2g10_medium','mu20'] ]],

        # LLP trigger seeded by MUON items
        ['j30_muvtx',                    'L1_2MU10',            [], [PhysicsStream], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_noiso',              'L1_2MU10',            [], [PhysicsStream], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_L1MU6_EMPTY',        'L1_MU6_EMPTY',        [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO', [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_L1MU4_UNPAIRED_NONISO', 'L1_MU4_UNPAIRED_NONISO', [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],

        # LLP trigger seeded by TAU items
        ['j30_jes_PS_llp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_PS_llp_noiso_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU8_EMPTY',        'L1_TAU8_EMPTY',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU8_UNPAIRED_ISO', 'L1_TAU8_UNPAIRED_ISO', [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU8_UNPAIRED_NONISO', 'L1_TAU8_UNPAIRED_NONISO', [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
      

        ['j30_jes_cleanLLP_PS_llp_L1TAU30_EMPTY',        'L1_TAU30_EMPTY',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_L1TAU30_UNPAIRED_ISO', 'L1_TAU30_UNPAIRED_ISO', [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j30_jes_cleanLLP_PS_llp_pufix_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_pufix_noiso_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU60',       'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU8_EMPTY',        'L1_TAU8_EMPTY',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU8_UNPAIRED_ISO',        'L1_TAU8_UNPAIRED_ISO',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU8_UNPAIRED_NONISO',     'L1_TAU8_UNPAIRED_NONISO',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_EMPTY',        'L1_TAU30_EMPTY',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1TAU30_UNPAIRED_ISO',  'L1_TAU30_UNPAIRED_ISO',        [], ["Late"], ['RATE:SingleJet', 'BW:Jet'], -1],

        # L1Topo LLP with TAU
        ['j30_jes_cleanLLP_PS_llp_L1LLP-NOMATCH',       'L1_LLP-NOMATCH',       [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1, False],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1LLP-NOMATCH', 'L1_LLP-NOMATCH',       [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1, False],

        ['j30_jes_cleanLLP_PS_llp_L1LLP-RO',       'L1_LLP-RO',       [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1, False],
        ['j30_jes_cleanLLP_PS_llp_noiso_L1LLP-RO', 'L1_LLP-RO',       [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1, False],

        # reversed calo-ratio triggers
        ['j250_jes_cleanLLP_PS_revllp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j230_jes_cleanLLP_PS_revllp_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],
        ['j200_jes_cleanLLP_PS_revllp_trkiso_L1TAU60',             'L1_TAU60',             [], [PhysicsStream], ['RATE:SingleJet', 'BW:Jet'], -1],

        ['j30_muvtx_noiso_L1MU6_EMPTY',        'L1_MU6_EMPTY',        [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_noiso_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO', [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_noiso_L1MU4_UNPAIRED_NONISO', 'L1_MU4_UNPAIRED_NONISO', [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],

        # LLP triggers
        ['g15_loose_2mu10_msonly_L1MU6_EMPTY',        'L1_MU6_EMPTY',['L1_EM8VH','L1_2MU10'], ["Late"], ['RATE:EgammaMuon', 'BW:Egamma','BW:Muon'],-1,['parallel',-1,[] ]],
        ['g15_loose_2mu10_msonly_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO',['L1_EM8VH','L1_2MU10'], ["Late"], ['RATE:EgammaMuon', 'BW:Egamma','BW:Muon'],-1,['parallel',-1,[] ]],
        ['g15_loose_2mu10_msonly_L1MU4_UNPAIRED_NONISO', 'L1_MU4_UNPAIRED_NONISO',['L1_EM8VH','L1_2MU10'], ["Late"], ['RATE:EgammaMuon', 'BW:Egamma','BW:Muon'],-1,['parallel',-1,[] ]],

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
        ['j30_xe60_razor220', 'L1_2J15_XE55', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j30','xe60']]],

        ['j55_0eta240_xe50_L1J30_EMPTY', 'L1_J30_EMPTY', ['',''], ['Late'], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,['j55_0eta240','xe50']]],


        # Electron + Muon
        ['g10_etcut_L1EM8I_mu10_taumass',      'L1_EM8I_MU10',  ['L1_EM8I', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_etcut_L1EM8I","mu10"]] ],
        ['g20_etcut_L1EM15_mu4_taumass',      'L1_EM15I_MU4',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_etcut_L1EM15","mu4"]] ],

        ['g10_loose_L1EM8I_mu10_iloose_taumass',      'L1_EM8I_MU10',  ['L1_EM8I', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_loose_L1EM8I","mu10_iloose"]] ],
        ['g20_loose_L1EM15_mu4_iloose_taumass',      'L1_EM15I_MU4',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_loose_L1EM15","mu4_iloose"]] ],
        ['g10_etcut_L1EM8I_mu10_iloose_taumass',      'L1_EM8I_MU10',  ['L1_EM8I', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_etcut_L1EM8I","mu10_iloose"]] ],
        ['g20_etcut_L1EM15_mu4_iloose_taumass',      'L1_EM15I_MU4',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_etcut_L1EM15","mu4_iloose"]] ],

        ### ATR-14543
        ['g10_loose_L1EM8I_mu10_ivarloose_taumass',      'L1_EM8I_MU10',  ['L1_EM8I', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_loose_L1EM8I","mu10_ivarloose"]] ],
        ['g20_loose_L1EM15_mu4_ivarloose_taumass',      'L1_EM15I_MU4',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_loose_L1EM15","mu4_ivarloose"]] ],
        ['g10_etcut_L1EM8I_mu10_ivarloose_taumass',      'L1_EM8I_MU10',  ['L1_EM8I', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_etcut_L1EM8I","mu10_ivarloose"]] ],
        ['g20_etcut_L1EM15_mu4_ivarloose_taumass',      'L1_EM15I_MU4',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_etcut_L1EM15","mu4_ivarloose"]] ],

        ## LFV chains (ATR-14352)
        ['g10_loose_mu10_iloose_taumass_L1LFV-EM8I', 'L1_LFV-EM8I',  ['L1_EM7', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_loose","mu10_iloose"]] ],
        ['g20_loose_mu4_iloose_taumass_L1LFV-EM15I', 'L1_LFV-EM15I',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_loose","mu4_iloose"]] ],
        
        ### ATR-14543
        ['g10_loose_mu10_ivarloose_taumass_L1LFV-EM8I', 'L1_LFV-EM8I',  ['L1_EM7', 'L1_MU10'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g10_loose","mu10_ivarloose"]] ],
        ['g20_loose_mu4_ivarloose_taumass_L1LFV-EM15I', 'L1_LFV-EM15I',  ['L1_EM15', 'L1_MU4'], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['parallel',-1,["g20_loose","mu4_ivarloose"]] ],

        # HLT photon and di-muon chains
        ['g35_loose_L1EM22VHI_mu18noL1',        'L1_EM22VHI', ['L1_EM22VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM22VHI','mu18noL1'] ]],
        ['g35_loose_L1EM22VHI_mu15noL1_mu2noL1', 'L1_EM22VHI', ['L1_EM22VHI', '',''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM22VHI','mu15noL1','mu2noL1']]],
        # alternative HLT photon and di-muon chains with L1EM24VHI seed
        ['g35_loose_L1EM24VHI_mu18noL1',        'L1_EM24VHI', ['L1_EM24VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM24VHI','mu18noL1'] ]],
        ['g35_loose_L1EM24VHI_mu15noL1_mu2noL1', 'L1_EM24VHI', ['L1_EM24VHI', '',''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM24VHI','mu15noL1','mu2noL1']]],
        ['g35_loose_L1EM26VHI_mu18noL1',        'L1_EM26VHI', ['L1_EM26VHI', ''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM26VHI','mu18noL1'] ]],
        ['g35_loose_L1EM26VHI_mu15noL1_mu2noL1', 'L1_EM26VHI', ['L1_EM26VHI', '',''], [PhysicsStream], ['RATE:EgammaMuon', 'BW:Egamma', 'BW:Muon'], -1, ['serial',-1,['g35_loose_L1EM26VHI','mu15noL1','mu2noL1']]],

        #Performance:
        ['e26_lhtight_idperf',        'L1_EM22VHI',    [], [PhysicsStream, 'express'], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],
        ['e28_lhtight_idperf',        'L1_EM24VHI',    [], [PhysicsStream], ['RATE:IDMonitoring', 'BW:Egamma', 'BW:ID'],-1],

	# COMBINED TAU CHAINS

	# e+tau
        #new ones   
	                                                                                                                     
	['e17_lhmedium_nod0_ivarloose_tau35_medium1_tracktwo_L1EM15HI_TAU20IM_2TAU15_J25_2J20_3J15',   'L1_EM15HI_TAU20IM_2TAU15_J25_2J20_3J15',['L1_EM15HI','L1_TAU20IM'], [PhysicsStream],['RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau35_medium1_tracktwo"]]],
	# new chain for 2e34 backup (ATR-15225)
        ['e24_lhmedium_nod0_ivarloose_tau35_medium1_tracktwo',  'L1_EM20VHI_TAU20IM_2TAU20_J25_3J20',['L1_EM20VHI','L1_TAU20IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e24_lhmedium_nod0_ivarloose","tau35_medium1_tracktwo"]]],
	                                               
        ['e17_lhmedium_nod0_tau80_medium1_tracktwo',   'L1_EM15HI_TAU40_2TAU15',['L1_EM15HI','L1_TAU40'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0","tau80_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo',  'L1_EM15HI_2TAU12IM_J25_3J12',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream],['RATE:ElectronTau', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],

	# support for l1topo chains
        ['e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15HI_2TAU12IM',  'L1_EM15HI_2TAU12IM',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_iloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15HI_2TAU12IM',  'L1_EM15HI_2TAU12IM',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],

	#L1Topo e+tau chains
        ['e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25',  'L1_DR-EM15TAU12I-J25',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1 ,["e17_lhmedium_nod0_iloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1DR-EM15TAU12I-J25', 'L1_DR-EM15TAU12I-J25',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'],-1,['serial',-1 ,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25', 'L1_EM15TAU12I-J25',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_tau25_medium1_tracktwo_L1EM15TAU12I-J25',  'L1_EM15TAU12I-J25',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0","tau25_medium1_tracktwo"]]],
        ['e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15TAU12I-J25',  'L1_EM15TAU12I-J25',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_iloose","tau25_medium1_tracktwo"]]],
#ATR-15197        ['e17_lhmedium_nod0_iloose_tau25_medium1_tracktwo_L1EM15-TAU12I',  'L1_EM15-TAU12I',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_iloose","tau25_medium1_tracktwo"]]],
#ATR-15197        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_L1EM15-TAU12I',  'L1_EM15-TAU12I',['L1_EM15HI','L1_TAU12IM'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo"]]],

#ATR-15197        ['e17_lhmedium_nod0_tau80_medium1_tracktwo_L1EM15-TAU40',   'L1_EM15-TAU40',['L1_EM15HI','L1_TAU40'], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0","tau80_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau35_medium1_tracktwo',  'L1_MU10_TAU20IM',['L1_MU10','L1_TAU20IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau35_medium1_tracktwo"]]],
        ['mu14_ivarloose_L1MU11_tau35_medium1_tracktwo_L1MU11_TAU20IM',  'L1_MU11_TAU20IM',['L1_MU11','L1_TAU20IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose_L1MU11","tau35_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau35_medium1_tracktwo_L1MU10_TAU20IM_J25_2J20',      'L1_MU10_TAU20IM_J25_2J20',['L1_MU10','L1_TAU20IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau35_medium1_tracktwo"]]],   

        ['mu14_iloose_tau25_perf_tracktwo', 'L1_MU10_TAU12IM',['L1_MU10','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_iloose","tau25_perf_tracktwo"]]],
        ### ATR-14543
        ['mu14_ivarloose_tau25_perf_tracktwo', 'L1_MU10_TAU12IM',['L1_MU10','L1_TAU12IM'], [PhysicsStream, 'express'], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau25_perf_tracktwo"]]],

        ['mu14_iloose_tau25_medium1_tracktwo',      'L1_MU10_TAU12IM_J25_2J12',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],        
        ['mu14_ivarloose_tau25_medium1_tracktwo',      'L1_MU10_TAU12IM_J25_2J12',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],        
        ['mu14_iloose_tau35_medium1_tracktwo',  'L1_MU10_TAU20IM',['L1_MU10','L1_TAU20IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau', 'BW:Muon'], -1,['serial',-1,["mu14_iloose","tau35_medium1_tracktwo"]]],

        #mu+tau chains seeded by L1MU10TAU20IM, needed as supporting triggers for the primary chains with either L1 jet or L1topo
        ['mu14_iloose_tau25_medium1_tracktwo_L1MU10_TAU12IM',  'L1_MU10_TAU12IM',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12IM',  'L1_MU10_TAU12IM',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],

        # l1topo tau+mu chains
        ['mu14_tau25_medium1_tracktwo_L1DR-MU10TAU12I',  'L1_DR-MU10TAU12I',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25', 'L1_DR-MU10TAU12I_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],
        ['mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I_TAU12I-J25', 'L1_DR-MU10TAU12I_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],
        ['mu14_iloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I', 'L1_DR-MU10TAU12I',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1DR-MU10TAU12I', 'L1_DR-MU10TAU12I',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],
        ['mu14_ivarloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25', 'L1_MU10_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose","tau25_medium1_tracktwo"]]],
        ['mu14_iloose_tau25_medium1_tracktwo_L1MU10_TAU12I-J25', 'L1_MU10_TAU12I-J25',['L1_MU10','L1_TAU12IM'], [PhysicsStream], ['RATE:MuonTau', 'BW:Tau'], -1,['serial',-1,["mu14_iloose","tau25_medium1_tracktwo"]]],

        
        # Tau + MET
        ['tau35_medium1_tracktwo_xe70_L1XE45',  'L1_TAU20IM_2J20_XE45',['L1_TAU20IM','L1_XE45'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","xe70_L1XE45"]]],  
#        ['tau35_medium1_tracktwo_L1TAU20_xe70_L1XE45',  'L1_TAU20_2J20_XE45',['L1_TAU20','L1_XE45'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo_L1TAU20","xe70_L1XE45"]]],
	# l1topo tau+met
	#['tau35_medium1_tracktwo_L1TAU20_xe70_L1XE45_L1XE45_TAU20-J20', 'L1_XE45_TAU20-J20',['L1_TAU20', 'L1_XE45'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo_L1TAU20","xe70_L1XE45"]]],
        
        # Ditau + MET
        ['tau35_medium1_tracktwo_L1TAU20_tau25_medium1_tracktwo_L1TAU12_xe50', 'L1_TAU20_2TAU12_XE35',['L1_TAU20','L1_TAU12','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo_L1TAU20","tau25_medium1_tracktwo_L1TAU12","xe50"]]],
        ['tau35_medium1_tracktwo_tau25_medium1_tracktwo_xe50', 'L1_TAU20IM_2TAU12IM_XE35',['L1_TAU20IM','L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau35_medium1_tracktwo","tau25_medium1_tracktwo","xe50"]]],
        ['tau60_medium1_tracktwo_tau25_medium1_tracktwo_xe50', 'L1_TAU40_2TAU12IM_XE40',['L1_TAU40','L1_TAU12IM','L1_XE40'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["tau60_medium1_tracktwo","tau25_medium1_tracktwo","xe50"]]],
        
        # Tau + e + MET
        ['e17_lhmedium_nod0_ivarloose_tau25_medium1_tracktwo_xe50', 'L1_EM15HI_2TAU12IM_XE35',['L1_EM15HI','L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau', 'BW:Egamma'],-1,['serial',-1,["e17_lhmedium_nod0_ivarloose","tau25_medium1_tracktwo", "xe50"]]],
        ['e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50', 'L1_EM15HI_2TAU12IM_XE35',['L1_EM15HI','L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["e17_lhmedium_nod0","tau25_medium1_tracktwo", "xe50"]]],

	#l1topo tau+e+met
#ATR-15197        ['e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50_L1XE35_EM15-TAU12I', 'L1_XE35_EM15-TAU12I',['L1_EM15HI','L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0","tau25_medium1_tracktwo", "xe50"]]],
#ATR-15197        ['e17_lhmedium_nod0_tau25_medium1_tracktwo_xe50_L1XE40_EM15-TAU12I', 'L1_XE40_EM15-TAU12I',['L1_EM15HI','L1_TAU12IM','L1_XE40'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["e17_lhmedium_nod0","tau25_medium1_tracktwo", "xe50"]]],


        # Tau + mu + MET
        ['mu14_tau25_medium1_tracktwo_xe50', 'L1_MU10_TAU12IM_XE35',['L1_MU10', 'L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["mu14", "tau25_medium1_tracktwo","xe50"]]],
        ['mu14_iloose_tau25_medium1_tracktwo_xe50', 'L1_MU10_TAU12IM_XE35',['L1_MU10', 'L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["mu14_iloose", "tau25_medium1_tracktwo","xe50"]]],        
        ['mu14_ivarloose_tau25_medium1_tracktwo_xe50', 'L1_MU10_TAU12IM_XE35',['L1_MU10', 'L1_TAU12IM','L1_XE35'], [PhysicsStream], ['RATE:TauMET', 'BW:Tau'], -1,['serial',-1,["mu14_ivarloose", "tau25_medium1_tracktwo","xe50"]]],

        ##['e18_lhloose_tau80_medium1_calo',                 'L1_EM15-TAU40',[], [PhysicsStream], ['RATE:ElectronTau', 'BW:Tau'], -1,['serial',-1,["e18_lhloose","tau80_medium1_calo"]]],

        ['j80_xe80',               'L1_J40_XE50',['',''], [PhysicsStream, 'express'], ['RATE:JetMET', 'BW:Jet', 'BW:MET'], -1,['serial',-1,["j80","xe80"]]], 

        # lepton + jets
        ['e15_lhtight_ivarloose_3j20_L1EM13VH_3J20',    	'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e15_lhtight_ivarloose","3j20"] ]], 
        ['e15_lhtight_nod0_ivarloose_3j20_L1EM13VH_3J20', 'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e15_lhtight_nod0_ivarloose","3j20"] ]], 
         
        ['e28_lhvloose_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_L1EM20VH","3j20noL1"] ]],
        ['e28_lhvloose_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_L1EM20VH","4j20noL1"] ]],
        ['e28_lhvloose_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_L1EM20VH","5j15noL1"] ]],
        ['e28_lhvloose_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_L1EM20VH","6j15noL1"] ]],
         
        ['e26_lhvloose_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_L1EM20VH","3j20noL1"] ]],
        ['e26_lhvloose_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_L1EM20VH","4j20noL1"] ]],
        ['e26_lhvloose_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_L1EM20VH","5j15noL1"] ]],
        ['e26_lhvloose_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_L1EM20VH","6j15noL1"] ]],
         
        ['e24_lhvloose_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_L1EM20VH","3j20noL1"] ]],
        ['e24_lhvloose_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_L1EM20VH","4j20noL1"] ]],
        ['e24_lhvloose_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_L1EM20VH","5j15noL1"] ]],
        ['e24_lhvloose_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_L1EM20VH","6j15noL1"] ]],
        #nod0 chains:
        ['e28_lhvloose_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","3j20noL1"] ]],
        ['e28_lhvloose_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","4j20noL1"] ]],
        ['e28_lhvloose_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","5j15noL1"] ]],
        ['e28_lhvloose_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhvloose_nod0_L1EM20VH","6j15noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","3j20noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","4j20noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","5j15noL1"] ]],
        ['e26_lhvloose_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhvloose_nod0_L1EM20VH","6j15noL1"] ]],
        ['e24_lhvloose_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_nod0_L1EM20VH","3j20noL1"] ]],
        ['e24_lhvloose_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_nod0_L1EM20VH","4j20noL1"] ]],
        ['e24_lhvloose_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_nod0_L1EM20VH","5j15noL1"] ]],
        ['e24_lhvloose_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhvloose_nod0_L1EM20VH","6j15noL1"] ]],
				# e+jets ATR-14404
        ['e28_lhmedium_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","3j20noL1"] ]],
        ['e28_lhmedium_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","4j20noL1"] ]],
        ['e28_lhmedium_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","5j15noL1"] ]],
        ['e28_lhmedium_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e28_lhmedium_nod0_L1EM20VH","6j15noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","3j20noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","4j20noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","5j15noL1"] ]],
        ['e26_lhmedium_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e26_lhmedium_nod0_L1EM20VH","6j15noL1"] ]],
        ['e24_lhmedium_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhmedium_nod0_L1EM20VH","3j20noL1"] ]],
        ['e24_lhmedium_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhmedium_nod0_L1EM20VH","4j20noL1"] ]],
        ['e24_lhmedium_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhmedium_nod0_L1EM20VH","5j15noL1"] ]],
        ['e24_lhmedium_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e24_lhmedium_nod0_L1EM20VH","6j15noL1"] ]],

        ['e30_lhvloose_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhvloose_nod0_L1EM20VH","3j20noL1"] ]],
        ['e30_lhvloose_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhvloose_nod0_L1EM20VH","4j20noL1"] ]],
        ['e30_lhvloose_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhvloose_nod0_L1EM20VH","5j15noL1"] ]],
        ['e30_lhvloose_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhvloose_nod0_L1EM20VH","6j15noL1"] ]],
        ['e30_lhmedium_nod0_L1EM20VH_3j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhmedium_nod0_L1EM20VH","3j20noL1"] ]],
        ['e30_lhmedium_nod0_L1EM20VH_4j20noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhmedium_nod0_L1EM20VH","4j20noL1"] ]],
        ['e30_lhmedium_nod0_L1EM20VH_5j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhmedium_nod0_L1EM20VH","5j15noL1"] ]],
        ['e30_lhmedium_nod0_L1EM20VH_6j15noL1',  'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1, ['serial',-1,["e30_lhmedium_nod0_L1EM20VH","6j15noL1"] ]],

        ['mu26_2j20noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","2j20noL1"] ]],
        ['mu26_3j20noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","3j20noL1"] ]],
        ['mu26_4j15noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","4j15noL1"] ]],
        ['mu26_5j15noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu26","5j15noL1"] ]],
         
        ['mu24_2j20noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","2j20noL1"] ]],
        ['mu24_3j20noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","3j20noL1"] ]],
        ['mu24_4j15noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","4j15noL1"] ]],
        ['mu24_5j15noL1',  'L1_MU20', ['L1_MU20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu24","5j15noL1"] ]],

        
        # VBF triggers

        ['g25_medium_L1EM22VHI_4j35_0eta490_invm700', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma'], -1,['serial',-1,["g25_medium_L1EM22VHI","4j35_0eta490_invm700"]]],
        #ATR-14196
        ['g25_medium_L1EM22VHI_4j35_0eta490_invm1000', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma'], -1,['serial',-1,["g25_medium_L1EM22VHI","4j35_0eta490_invm1000"]]],
        ['g25_loose_L1EM20VH_4j35_0eta490', 'L1_EM20VH', ['L1_EM20VH',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma'], -1,['serial',-1,["g25_loose_L1EM20VH","4j35_0eta490"]]],
        ['g25_medium_L1EM22VHI_j35_0eta490_bmv2c2077_3j35_0eta490_invm700', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', 'j35_0eta490_bmv2c2077','3j35_0eta490_invm700']]],
        ['g25_medium_L1EM22VHI_2j35_0eta490_bmv2c2077_2j35_0eta490', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', '2j35_0eta490_bmv2c2077','2j35_0eta490']]],
	
        ['g25_medium_L1EM22VHI_j35_0eta490_bmv2c2077_split_3j35_0eta490_invm700', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', 'j35_0eta490_bmv2c2077_split','3j35_0eta490_invm700']]],
	
        ['g25_medium_L1EM22VHI_2j35_0eta490_bmv2c2077_split_2j35_0eta490', 'L1_EM22VHI', ['L1_EM22VHI', '', ''], [PhysicsStream], ['RATE:EgammaJet', 'BW:BJet','BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', '2j35_0eta490_bmv2c2077_split','2j35_0eta490']]],	

        ### ATR-14356: egamma+jets VBF chians
        ['g25_medium_L1EM22VHI_2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400', 'L1_EM22VHI', ['L1_EM22VHI', 'L1_HT150-JJ15.ETA49_MJJ-400'], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma'],-1,['serial',-1,['g25_medium_L1EM22VHI', '2j35_0eta490_invm700_L1HT150-JJ15.ETA49_MJJ-400']]],

        # backups for L1Topo VBF items
        ['mu6_2j40_0eta490_invm400_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6","2j40_0eta490_invm400"]]],
        ['mu6_2j40_0eta490_invm600_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6","2j40_0eta490_invm600"]]],
        ['mu6_2j40_0eta490_invm800_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6","2j40_0eta490_invm800"]]],

	# Hbb VBF trigger (ATR-13526)
        ['j80_0eta240_j60_j45_320eta490_invm700_AND_j45_bmv2c2070_split_j45_bmv2c2077_split','L1_J40.0ETA25_2J25_J20.31ETA49', ['','','','',''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet' ], -1, ['serial',-1,['j80_0eta240','j60','j45_320eta490_invm700_AND','j45_bmv2c2070_split','j45_bmv2c2077_split'] ]],
        ['j80_0eta240_j60_j45_320eta490_AND_2j45_bmv2c2070_split','L1_J40.0ETA25_2J25_J20.31ETA49', ['','','','',''], [PhysicsStream], ['RATE:MultiBJet', 'BW:BJet' ], -1, ['serial',-1,['j80_0eta240','j60','j45_320eta490_AND','2j45_bmv2c2070_split'] ]],

        #['2j40_0eta490_invm250_xe80_L1XE70',    'L1_XE70', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["2j40_0eta490_invm250","xe80_L1XE70"]]],
        #['j40_0eta490_j30_0eta490_deta25_xe80_L1XE70',    'L1_XE70', ['','',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j40_0eta490","j30_0eta490_deta25","xe80_L1XE70"]]],
        ['j40_0eta490_j30_0eta490_deta25_xe80_L1XE50',    'L1_XE50', ['','',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j40_0eta490","j30_0eta490_deta25","xe80_L1XE50"]]],
 

        #tau+photon (ATR-13061)
        ['g35_medium_tau25_dikaonmasstight_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_dikaonmasstight_tracktwo_L1TAU12"]]],
        ['g35_medium_tau35_dikaonmasstight_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau35_dikaonmasstight_tracktwo_L1TAU12"]]],
        ['g35_medium_tau25_dikaonmass_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_dikaonmass_tracktwo_L1TAU12"]]],
        ['g35_medium_tau35_dikaonmass_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau35_dikaonmass_tracktwo_L1TAU12"]]],

        ['g35_medium_L1EM24VHI_tau25_dikaonmasstight_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_dikaonmasstight_tracktwo_L1TAU12"]]], 
        ['g35_medium_L1EM24VHI_tau25_dikaonmass_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_dikaonmass_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau35_dikaonmass_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau35_dikaonmass_tracktwo_L1TAU12"]]],

        #tau + photon (ATR-13841)
        ['g35_medium_tau25_dipion1_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_dipion1_tracktwo_L1TAU12"]]],
        ['g35_medium_tau35_dipion1loose_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau35_dipion1loose_tracktwo_L1TAU12"]]],
        ['g35_medium_tau25_dipion2_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_dipion2_tracktwo_L1TAU12"]]],
        ['g35_medium_tau35_dipion2_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau35_dipion2_tracktwo_L1TAU12"]]],
        ['g35_medium_tau25_kaonpi1_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_kaonpi1_tracktwo_L1TAU12"]]],
        ['g35_medium_tau35_kaonpi1_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau35_kaonpi1_tracktwo_L1TAU12"]]],
        ['g35_medium_tau25_kaonpi2_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_kaonpi2_tracktwo_L1TAU12"]]],
        ['g35_medium_tau35_kaonpi2_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau35_kaonpi2_tracktwo_L1TAU12"]]],

        ['g35_medium_L1EM24VHI_tau25_dipion1_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_dipion1_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau35_dipion1loose_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau35_dipion1loose_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau25_dipion2_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_dipion2_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau35_dipion2_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau35_dipion2_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau25_kaonpi1_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_kaonpi1_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau35_kaonpi1_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau35_kaonpi1_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau25_kaonpi2_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_kaonpi2_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau35_kaonpi2_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau35_kaonpi2_tracktwo_L1TAU12"]]],

	# ATR-14643
        ['g25_medium_tau25_dikaonmasstight_tracktwo_60mVis10000','L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_dikaonmasstight_tracktwo'],False]],	
	['g25_medium_L1EM24VHI_tau25_dikaonmasstight_tracktwo_60mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_dikaonmasstight_tracktwo'],False] ],

	# ATR-14947
        ['g25_medium_tau25_dikaonmass_tracktwo_50mVis10000','L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_dikaonmass_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_dikaonmass_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_dikaonmass_tracktwo'],False]],
        ['g25_medium_tau25_singlepion_tracktwo_50mVis10000','L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_singlepion_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_singlepion_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_singlepion_tracktwo'],False]],
        ['g25_medium_tau25_dipion1loose_tracktwo_50mVis10000','L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_dipion1loose_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_dipion1loose_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_dipion1loose_tracktwo'],False]],
        ['g25_medium_tau25_dipion2_tracktwo_50mVis10000','L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_dipion2_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_dipion2_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_dipion2_tracktwo'],False]],
        ['g25_medium_tau25_kaonpi1_tracktwo_50mVis10000','L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_kaonpi1_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_kaonpi1_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_kaonpi1_tracktwo'],False]],
        ['g25_medium_tau25_kaonpi2_tracktwo_50mVis10000','L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium','tau25_kaonpi2_tracktwo'],False]],
        ['g25_medium_L1EM24VHI_tau25_kaonpi2_tracktwo_50mVis10000','L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,['g25_medium_L1EM24VHI','tau25_kaonpi2_tracktwo'],False]],

        # photon+pion (ATR-13525)
        ['g35_medium_tau25_singlepion_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_singlepion_tracktwo_L1TAU12"]]],
        ['g35_medium_tau25_singlepiontight_tracktwo_L1TAU12',     'L1_EM22VHI', ['L1_EM22VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium","tau25_singlepiontight_tracktwo_L1TAU12"]]],

        ['g35_medium_L1EM24VHI_tau25_singlepion_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_singlepion_tracktwo_L1TAU12"]]],
        ['g35_medium_L1EM24VHI_tau25_singlepiontight_tracktwo_L1TAU12',     'L1_EM24VHI', ['L1_EM24VHI','L1_TAU12'], [PhysicsStream], ['RATE:TauGamma', 'BW:Tau', 'BW:Egamma'], -1,['serial',-1,["g35_medium_L1EM24VHI","tau25_singlepiontight_tracktwo_L1TAU12"]]],	
	


        ['g75_tight_3j50noL1_L1EM22VHI', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g75_tight","3j50noL1_L1EM22VHI"]]],

        ### ATR-14196
        ['g85_tight_3j50noL1_L1EM22VHI', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight","3j50noL1_L1EM22VHI"]]],
        ['g85_tight_3j50noL1_L1EM24VHI', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight","3j50noL1_L1EM24VHI"]]],
        ['g85_tight_3j50noL1_L1EM26VHI', 'L1_EM26VHI', ['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g85_tight","3j50noL1_L1EM26VHI"]]],
        ['g100_tight_3j50noL1_L1EM22VHI', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight","3j50noL1_L1EM22VHI"]]],
        ['g100_tight_3j50noL1_L1EM24VHI', 'L1_EM24VHI', ['L1_EM24VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight","3j50noL1_L1EM24VHI"]]],
        ['g100_tight_3j50noL1_L1EM26VHI', 'L1_EM26VHI', ['L1_EM26VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g100_tight","3j50noL1_L1EM26VHI"]]],

        ['g85_tight', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g85_tight_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g85_tight_L1EM26VHI', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g100_tight', 'L1_EM22VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g100_tight_L1EM24VHI', 'L1_EM24VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],
        ['g100_tight_L1EM26VHI', 'L1_EM26VHI', [], [PhysicsStream], ['RATE:ElectronPhoton', 'BW:Egamma'], -1],

#        ['g45_tight_iloose_3j50noL1_L1EM20VHI', 'L1_EM20VHI', ['L1_EM20VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g45_tight_iloose","3j50noL1_L1EM20VHI"]]],
#        ['g45_tight_iloose_3j50noL1_L1EM22VHI', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g45_tight_iloose","3j50noL1_L1EM22VHI"]]],
#
#        ['g75_tight_iloose_3j25noL1_L1EM20VHI', 'L1_EM20VHI', ['L1_EM20VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g75_tight_iloose","3j25noL1_L1EM20VHI"]]],
#        ['g75_tight_iloose_3j25noL1_L1EM22VHI', 'L1_EM22VHI', ['L1_EM22VHI',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["g75_tight_iloose","3j25noL1_L1EM22VHI"]]],
#



        ['e15_lhtight_iloose_3j20_L1EM13VH_3J20','L1_EM13VH_3J20',  ['L1_EM13VH_3J20',''],  [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'], -1,['serial',-1,["e15_lhtight_iloose","3j20"]]],

#        ['j80_bmv2c2060_split_xe60_L12J50_XE40','L1_2J50_XE40',  [],  [PhysicsStream], ['RATE:JetMET', 'BW:MET', 'BW:BJet'], -1,['serial',-1,['xe60',"j80_bmv2c2060_split"]]]           ,
        ['j80_bmv2c2060_split_xe60_L12J50_XE40','L1_2J50_XE40',  [],  [PhysicsStream], ['RATE:JetMET', 'BW:MET', 'BW:BJet'], -1,['serial',-1,["j80_bmv2c2060_split","xe60"]]], 
     #ATR-14196
     ['j80_bmv2c2050_split_xe60_L12J50_XE40','L1_2J50_XE40',  [],  [PhysicsStream], ['RATE:JetMET', 'BW:MET', 'BW:BJet'], -1,['serial',-1,["j80_bmv2c2050_split","xe60"]]],

      ['j80_bmv2c2077_split_xe70_L1J400ETA25_XE50','L1_J40.0ETA25_XE50',  ['L1_J40.0ETA25','XE50'],  [PhysicsStream], ['RATE:JetMET', 'BW:MET', 'BW:BJet'], -1,['serial',-1,["j80_bmv2c2077_split","xe70_L1J400ETA25_XE50"]]],

        # MET+jet chains with cleaning
        ['j40_cleanT_xe80_L1XE60',    'L1_XE60', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j40_cleanT","xe80_L1XE60"]]],
        ['j40_cleanT_xe85_tc_lcw_2dphi05_L1XE50',         'L1_XE50', ['',''], [PhysicsStream], ['RATE:JetMET', 'BW:Jet'], -1, ['serial',-1,["j40_cleanT","xe85_tc_lcw_L1XE50"]]],

        # muon plus multijets
        ['mu14_iloose_3j20_L1MU10_3J20', 'L1_MU10_3J20', ['L1_MU10_3J20', ''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon','BW:Jet'], -1,['serial',-1,['mu14_iloose', '3j20'] ],True],
        ['mu14_ivarloose_3j20_L1MU10_3J20', 'L1_MU10_3J20', ['L1_MU10_3J20', ''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon','BW:Jet'], -1,['serial',-1,['mu14_ivarloose', '3j20'] ],True],

        
        #Backup
#        ['mu4_3j35_dr05_j35_bloose_split_antimatchdr05mu_L1MU4_3J15', 'L1_MU4_3J15', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '3j35', 'j35_bloose_split_antimatchdr05mu'] ]],
#        ['mu4_3j30_dr05_j30_bmedium_split_antimatchdr05mu_L1MU4_3J15', 'L1_MU4_3J15', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '3j30', 'j30_bmedium_split_antimatchdr05mu'] ]],
#        ['mu4_3j35_dr05_j35_bloose_split_antimatchdr05mu_L1MU4_3J20', 'L1_MU4_3J20', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '3j35', 'j35_bloose_split_antimatchdr05mu'] ]],
#        ['mu4_3j30_dr05_j30_bmedium_split_antimatchdr05mu_L1MU4_3J20', 'L1_MU4_3J20', ['L1_MU4', '', ''], [PhysicsStream], ['RATE:MuonBJet', 'BW:BJet'], -1,['serial',-1,['mu4', '3j30', 'j30_bmedium_split_antimatchdr05mu'] ]],
        
        ['mu6_ivarloose_2j40_0eta490_invm1000_L1MU6_J30.0ETA49_2J20.0ETA49', 'L1_MU6_J30.0ETA49_2J20.0ETA49', ['L1_MU6',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'], -1, ['serial',-1,["mu6_ivarloose","2j40_0eta490_invm1000"]]],

        #mujet matching chain 
        ['g15_loose_2mu10_msonly_L1MU4_EMPTY',        'L1_MU4_EMPTY',['L1_EM8VH','L1_2MU10'], ["Late"], ['RATE:EgammaMuon', 'BW:Egamma','BW:Muon'],-1,['parallel',-1,[] ]],

        ['j30_muvtx_L1MU4_EMPTY',        'L1_MU4_EMPTY',        [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],
        ['j30_muvtx_noiso_L1MU4_EMPTY',        'L1_MU4_EMPTY',        [], ["Late"], ['RATE:MuonJet','BW:Muon', 'BW:Jet'], -1],



        ## Lepton + jets with unisolated leptons:
        # For above 1.0 e34
        ['e26_lhmedium_nod0_5j30_0eta240_L1EM13VH_3J20',    	'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e26_lhmedium_nod0","5j30_0eta240"] ]],
        ['e26_lhmedium_5j30_0eta240_L1EM13VH_3J20',    	'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e26_lhmedium","5j30_0eta240"] ]],
        ['mu26_5j30_0eta240_L1MU10_3J20',    	'L1_MU10_3J20',    ['L1_MU10_3J20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'],-1, ['serial',-1,["mu26","5j30_0eta240"] ]],
        # For below 1.0 e34
        ['e24_lhmedium_nod0_5j30_0eta240_L1EM13VH_3J20',    	'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e24_lhmedium_nod0","5j30_0eta240"] ]],
        ['e24_lhmedium_5j30_0eta240_L1EM13VH_3J20',    	'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e24_lhmedium","5j30_0eta240"] ]],
        ['mu24_5j30_0eta240_L1MU10_3J20',    	'L1_MU10_3J20',    ['L1_MU10_3J20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'],-1, ['serial',-1,["mu24","5j30_0eta240"] ]],

        ## Out of curiosity with j25 for a rate calculation.
        ## Will most likely not be enabled and then deleted again for the next cache
        # ['e26_lhmedium_5j25_0eta240_L1EM13VH_3J20',    	'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e26_lhmedium","5j25_0eta240"] ]],
        # ['mu26_5j25_0eta240_L1MU10_3J20',    	'L1_MU10_3J20',    ['L1_MU10_3J20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'],-1, ['serial',-1,["mu26","5j25_0eta240"] ]],
        # ['e24_lhmedium_5j25_0eta240_L1EM13VH_3J20',    	'L1_EM13VH_3J20',    ['L1_EM13VH_3J20',''], [PhysicsStream], ['RATE:EgammaJet', 'BW:Egamma', 'BW:Jet'],-1, ['serial',-1,["e24_lhmedium","5j25_0eta240"] ]],
        # ['mu24_5j25_0eta240_L1MU10_3J20',    	'L1_MU10_3J20',    ['L1_MU10_3J20',''], [PhysicsStream], ['RATE:MuonJet', 'BW:Muon', 'BW:Jet'],-1, ['serial',-1,["mu24","5j25_0eta240"] ]],

        
			 ]


    TriggerFlags.HeavyIonSlice.signatures = [
        ]


 #Beamspot chanis first try ATR-9847                                                                                                               
    TriggerFlags.BeamspotSlice.signatures = [
        #['beamspot_allTE_trkfast_L1TRT',           'L1_TRT', [], [PhysicsStream], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ##trkFS --> no selection, write out PEB events
        ['beamspot_allTE_trkfast_peb_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_allTE_trkfast_peb_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_allTE_trkfast_peb',           'L1_4J15', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ##trkFS --> no selection, add TRT to PEB events        

        ['beamspot_allTE_trkfast_pebTRT_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_allTE_trkfast_pebTRT_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

# taking this out all together, not needed and even with PS=-1 makes histograms

        ['beamspot_allTE_trkfast_pebTRT',           'L1_4J15', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ##trkFS --> select good vertices, and write out PEB events

        ['beamspot_activeTE_trkfast_peb_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_activeTE_trkfast_peb_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],



        ['beamspot_activeTE_trkfast_peb',           'L1_4J15', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ##trkFS --> select good vertices, add TRT data to PEB events

        ['beamspot_activeTE_trkfast_pebTRT_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],


        ['beamspot_activeTE_trkfast_pebTRT_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

# taking this out all together, not needed and even with PS=-1 makes histograms

        ['beamspot_activeTE_trkfast_pebTRT',           'L1_4J15', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ##trkFS --> online beam-spot determination

        ['beamspot_trkFS_trkfast_L1TRT_FILLED',           'L1_TRT_FILLED', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],

        ['beamspot_trkFS_trkfast_L1TRT_EMPTY',           'L1_TRT_EMPTY', [], ["BeamSpot"], ['RATE:BeamSpot',  'BW:BeamSpot'], -1],
        
# taking this out all together, not needed and even with PS=-1 makes histograms
        ['beamspot_trkFS_trkfast', 'L1_4J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],

        ['beamspot_activeTE_trkfast_peb_L13J15', 'L1_3J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_allTE_trkfast_peb_L13J15', 'L1_3J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_trkFS_trkfast_L13J15', 'L1_3J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],

        ['beamspot_activeTE_trkfast_peb_L1J15', 'L1_J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_allTE_trkfast_peb_L1J15', 'L1_J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],
        ['beamspot_trkFS_trkfast_L1J15', 'L1_J15', [], ["BeamSpot"], ['RATE:BeamSpot', 'BW:BeamSpot'], -1],

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
        ##['mb_sp2000_trk70_hmt',        'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:HMT"], -1], #disabled for M4 see https://savannah.cern.ch/bugs/?104744

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
        ['mb_sp900_pusup400_trk50_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_pusup400_trk60_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_pusup450_trk70_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk90_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk100_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1000_pusup450_trk70_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1200_pusup500_trk80_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1400_pusup550_trk90_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1600_pusup600_trk100_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1800_pusup700_trk110_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp2300_pusup1000_trk130_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        
        #HMT without pileup supprestion seeded by L1_TE triggers
        ['mb_sp900_trk50_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk60_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_trk70_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk90_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk100_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2300_trk130_hmt_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1000_trk70_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1200_trk80_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1400_trk90_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1600_trk100_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1800_trk110_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp2300_trk130_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
                #HMT with pileup supprestion seeded by L1_TE.0ETA24 triggers
        ['mb_sp900_pusup400_trk50_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_pusup400_trk60_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_pusup450_trk70_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk90_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk100_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1000_pusup450_trk70_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_pusup500_trk80_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1200_pusup500_trk80_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_pusup550_trk90_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1400_pusup550_trk90_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_pusup600_trk100_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1600_pusup600_trk100_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_pusup700_trk110_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1800_pusup700_trk110_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_pusup900_trk120_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_pusup1000_trk130_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp2300_pusup1000_trk130_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_pusup1100_trk140_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_pusup1200_trk150_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_pusup1300_trk160_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        
        #HMT without pileup supprestion seeded by L1_TE.0ETA24 triggers
        ['mb_sp900_trk50_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp900_trk60_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1000_trk70_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk90_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk100_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sp2300_trk130_hmt_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1000_trk70_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_trk80_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1200_trk80_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1400_trk90_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1400_trk90_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_trk100_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1600_trk100_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_trk110_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp1800_trk110_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_trk120_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2300_trk130_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp2300_trk130_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2500_trk140_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2700_trk150_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2900_trk160_hmt_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #HMT + jets
        #['mb_sp900_pusup400_trk50_hmt_j15_L1TE5', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp900_pusup400_trk60_hmt_j15_L1TE5', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp900_pusup400_trk50_hmt_j20_L1TE5', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        #['mb_sp900_pusup400_trk60_hmt_j20_L1TE5', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #HMT performance
        ['mb_sp800_hmtperf_L1TE5', 'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp800_hmtperf_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_hmtperf_L1TE10', 'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_hmtperf_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_hmtperf_L1TE15', 'L1_TE15', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_hmtperf_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE20', 'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE25', 'L1_TE25', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE30', 'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE40', 'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp800_hmtperf_L1TE5.0ETA24', 'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp800_hmtperf_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_hmtperf_L1TE10.0ETA24', 'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1200_hmtperf_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_hmtperf_L1TE15.0ETA24', 'L1_TE15.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1600_hmtperf_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE20.0ETA24', 'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE25.0ETA24', 'L1_TE25.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE30.0ETA24', 'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2100_hmtperf_L1TE40.0ETA24', 'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['mb_sp2_hmtperf_L1MBTS_1_1', 'L1_MBTS_1_1', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp2_hmtperf', 'L1_RD3_FILLED', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp500_hmtperf', 'L1_RD3_FILLED', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp1800_hmtperf', 'L1_RD3_FILLED', [], ['MinBias', 'express'], ["BW:MinBias", "RATE:MinBias"], -1],
	
        # ALFA
#	['mb_sptrk_vetombts2in_L1ALFA_CEP',                  'L1_ALFA_CEP',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
#ATR-13743        ['mb_sptrk_vetombts2in_L1TRT_ALFA_EINE',             'L1_TRT_ALFA_EINE',             [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

#      	['mb_sptrk_vetombts2in_L1ALFA_ANY',                  'L1_ALFA_ANY',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
#      	['mb_sptrk_vetombts2in_L1ALFA_ANY_UNPAIRED_ISO',     'L1_ALFA_ANY_UNPAIRED_ISO',     [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
#        ['mb_sptrk_vetombts2in_L1TRT_ALFA_ANY',              'L1_TRT_ALFA_ANY',              [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
#        ['mb_sptrk_vetombts2in_L1TRT_ALFA_ANY_UNPAIRED_ISO', 'L1_TRT_ALFA_ANY_UNPAIRED_ISO', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
#      	['mb_sptrk_vetombts2in_peb_L1ALFA_ANY',                  'L1_ALFA_ANY',                  [], ['IDFwd'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#      	['mb_sptrk_vetombts2in_peb_L1ALFA_ANY_UNPAIRED_ISO',     'L1_ALFA_ANY_UNPAIRED_ISO',     [], ['IDFwd'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY',              'L1_TRT_ALFA_ANY',              [], ['IDFwd'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY_UNPAIRED_ISO', 'L1_TRT_ALFA_ANY_UNPAIRED_ISO', [], ['IDFwd'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
        ['mb_sptrk_vetombts2in_L1ZDC_AND',                  'L1_ZDC_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_vetombts2in_L1ZDC_A_C',                  'L1_ZDC_A_C',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #AFP standalone 
        ['mb_sp_L1AFP_C_MBTS_A',                      'L1_AFP_C_MBTS_A',               [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_L1AFP_C_ANY_MBTS_A',                  'L1_AFP_C_ANY_MBTS_A',           [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_L1AFP_C_AND',                         'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt2_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt4_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt6_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt8_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],        
        ['mb_sptrk_trk3_L1AFP_C_AND',                 'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_trk5_L1AFP_C_AND',                 'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_trk10_L1AFP_C_AND',                'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_trk15_L1AFP_C_AND',                'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_trk80_L1AFP_C_AND',                'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        
        #AFP (ATR-14833)
        ['mb_sptrk_trk80_L1MBTS_2',                   'L1_MBTS_2',                     [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sptrk_pt2_L1MBTS_2',                     'L1_MBTS_2',                     [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_vetospmbts2in_L1AFP_C_AND',           'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['mb_sp_vetosp_L1AFP_C_AND',                  'L1_AFP_C_AND',                  [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
                        
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
#ATR-13743        ['lumipeb_L1ALFA_BGT',              'L1_ALFA_BGT',              [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],
#        ['lumipeb_L1ALFA_BGT_UNPAIRED_ISO', 'L1_ALFA_BGT_UNPAIRED_ISO', [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],
#        ['lumipeb_L1ALFA_BGT_BGRP10',       'L1_ALFA_BGT_BGRP10',       [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],

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
        
        ['lumipeb_vdm_L1MBTS_1', 'L1_MBTS_1', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1MBTS_2', 'L1_MBTS_2', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        ['lumipeb_vdm_L1RD0_FILLED',         'L1_RD0_FILLED', [], ['VdM'], ["RATE:Calibration", "BW:Detector"], -1],
        

        ['lumipeb_L1RD0_ABORTGAPNOTCALIB',    'L1_RD0_ABORTGAPNOTCALIB',    [], ['PixelBeam'], ["RATE:Calibration", "BW:Detector"], -1],

        ['larnoiseburst_loose_L1All',             'L1_All',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1All',             'L1_All',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1XE35',            'L1_XE35',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1XE50',            'L1_XE50',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1XE55',            'L1_XE55',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1J50',             'L1_J50',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1J75',             'L1_J75',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        ['larnoiseburst_L1J40_XE50',            'L1_J40_XE50',       [], ['LArNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
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

        #['tilelarcalib_L1EM3_EMPTY',     'L1_EM3_EMPTY',      [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],
        #['tilelarcalib_L1TAU8_EMPTY',    'L1_TAU8_EMPTY',     [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],
        #['tilelarcalib_L1J12_EMPTY',     'L1_J12_EMPTY',      [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],
        #['tilelarcalib_L1J3031ETA49_EMPTY',   'L1_J30.31ETA49_EMPTY',     [], ['LArCellsEmpty'], ["RATE:Calibration", "BW:Detector"], -1],

        #ALFA Calib
        ['alfacalib',      'L1_ALFA_Calib'    , [],  ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        
#ATR-13743        ['alfacalib_L1ALFA_ANY',   'L1_ALFA_ANY'      , [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        
#        ['alfacalib_L1ALFA_ELAST15', 'L1_ALFA_ELAST15', [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['alfacalib_L1ALFA_ELAST18', 'L1_ALFA_ELAST18', [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['alfacalib_L1ALFA_A7L1_OD',                       'L1_ALFA_A7L1_OD',                    [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['alfacalib_L1ALFA_B7L1_OD',                       'L1_ALFA_B7L1_OD',                    [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['alfacalib_L1ALFA_A7R1_OD',                       'L1_ALFA_A7R1_OD',                    [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['alfacalib_L1ALFA_B7R1_OD',                       'L1_ALFA_B7R1_OD',                    [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],
#        ['alfacalib_L1ALFA_SYS',   'L1_ALFA_SYS'      , [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        
#        ['alfacalib_L1ALFA_ELAS',   'L1_ALFA_ELAS'      , [], ['ALFACalib'], [ 'RATE:ALFACalibration','BW:Detector'], -1],        

        # ALFA+LHCf+L1 triggers bits only readout
#        ['alfacalib_L1LHCF_ALFA_ANY_A',                    'L1_LHCF_ALFA_ANY_A',                 [], ['ALFACalib'], [ 'RATE:ALFA','BW:Detector'], -1],
#        ['alfacalib_L1LHCF_ALFA_ANY_C',                    'L1_LHCF_ALFA_ANY_C',                 [], ['ALFACalib'], [ 'RATE:ALFA','BW:Detector'], -1],
#        ['alfacalib_L1LHCF_ALFA_ANY_A_UNPAIRED_ISO',       'L1_LHCF_ALFA_ANY_A_UNPAIRED_ISO',    [], ['ALFACalib'], [ 'RATE:ALFA','BW:Detector'], -1],
#        ['alfacalib_L1LHCF_ALFA_ANY_C_UNPAIRED_ISO',       'L1_LHCF_ALFA_ANY_C_UNPAIRED_ISO',    [], ['ALFACalib'], [ 'RATE:ALFA','BW:Detector'], -1],

        # ALFA+ID+LHCf+L1 triggers bits only readout
#        ['lhcfpeb_L1LHCF_ALFA_ANY_A',                    'L1_LHCF_ALFA_ANY_A',                 [], ['IDFwd'], [ 'RATE:ALFA','BW:Detector'], -1],
#        ['lhcfpeb_L1LHCF_ALFA_ANY_C',                    'L1_LHCF_ALFA_ANY_C',                 [], ['IDFwd'], [ 'RATE:ALFA','BW:Detector'], -1],
#        ['lhcfpeb_L1LHCF_ALFA_ANY_A_UNPAIRED_ISO',       'L1_LHCF_ALFA_ANY_A_UNPAIRED_ISO',    [], ['IDFwd'], [ 'RATE:ALFA','BW:Detector'], -1],
#        ['lhcfpeb_L1LHCF_ALFA_ANY_C_UNPAIRED_ISO',       'L1_LHCF_ALFA_ANY_C_UNPAIRED_ISO',    [], ['IDFwd'], [ 'RATE:ALFA','BW:Detector'], -1],


        #AFP Calib
        ['calibAFP_L1AFP_C_ANY',                   'L1_AFP_C_ANY'    ,              [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_ANY_UNPAIRED_ISO',      'L1_AFP_C_ANY_UNPAIRED_ISO'    , [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_ANY_UNPAIRED_NONISO',   'L1_AFP_C_ANY_UNPAIRED_NONISO' , [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_ANY_EMPTY',             'L1_AFP_C_ANY_EMPTY'    ,        [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_ANY_FIRSTEMPTY',        'L1_AFP_C_ANY_FIRSTEMPTY' ,      [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_AND',                   'L1_AFP_C_AND'    ,              [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_NSC',                   'L1_AFP_NSC'    ,              [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_FSC',                   'L1_AFP_FSC'    ,              [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        ['calibAFP_L1AFP_C_MBTS_A',                   'L1_AFP_C_MBTS_A'    ,              [],  ['AFP'], [ 'RATE:CalibrationAFP','BW:Detector'], -1],
        
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

        ['conej40_larpebj',  'L1_J12',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['conej165_larpebj', 'L1_J75',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1], 
        ['conej75_320eta490_larpebj',  'L1_J30.31ETA49',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['conej140_320eta490_larpebj', 'L1_J75.31ETA49',     [], ['LArCells'], ['RATE:Calibration', 'BW:Detector'],-1],

        # Monitoritems for saturated towers
        ['satu20em_l1satmon_L1J100_FIRSTEMPTY',  'L1_J100_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1J100.31ETA49_FIRSTEMPTY',  'L1_J100.31ETA49_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1EM20VH_FIRSTEMPTY',  'L1_EM20VH_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        #['satu20em_l1satmon_L1EM20VHI_FIRSTEMPTY',  'L1_EM20VHI_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        # the one above was replaced with a em22vhi_firstempty seeded one:
        ['satu20em_l1satmon_L1EM22VHI_FIRSTEMPTY',  'L1_EM22VHI_FIRSTEMPTY',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1J100',  'L1_J100',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],
        ['satu20em_l1satmon_L1J100.31ETA49',  'L1_J100.31ETA49',     [], ['L1Calo'], ['RATE:Calibration', 'BW:Detector'],-1],

        ]

    TriggerFlags.CosmicSlice.signatures  = [ 
        ['tilecalib_laser',   'L1_CALREQ2', [], ['Tile'], ["RATE:Calibration", "RATE:Cosmic_TileCalibration", "BW:Detector"], -1],
        #['pixel_noise',        'L1_RD0_EMPTY', [], ['PixelNoise'], ["RATE:Calibration", "RATE:PixelCalibration", "BW:Detector"], -1],
        #['pixel_beam',         'L1_RD0_FILLED', [], ['PixelBeam'], ["RATE:Calibration", "RATE:PixelBeamCalibration", "BW:Detector"], -1],
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
#        ['larhec_L1RD0_FIRSTEMPTY', 'L1_RD0_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        ['larhec_L1J30_FIRSTEMPTY', 'L1_J30_FIRSTEMPTY', [], ['LArCellsEmpty'], ['RATE:CosmicSlice', 'RATE:Cosmic_LArCalibration', 'BW:Detector'], -1],
        #CosmicID
        ['id_cosmicid_L1MU4_EMPTY',         'L1_MU4_EMPTY', [],  ['HLT_IDCosmic',  'express'], ['RATE:CosmicSlice', 'RATE:Cosmic_Tracking', 'BW:Detector'], -1],
        ['id_cosmicid_L1MU11_EMPTY',        'L1_MU11_EMPTY', [], ['HLT_IDCosmic',  'express'], ['RATE:CosmicSlice', 'RATE:Cosmic_Tracking', 'BW:Detector'], -1],

        #['id_cosmicid',               '', [], ['HLT_IDCosmic',  'express'], ['RATE:CosmicSlice', 'RATE:Cosmic_Tracking', 'BW:Detector'], -1],
        #['id_cosmicid_trtxk',         '', [], ['HLT_IDCosmic'],            ['RATE:CosmicSlice', 'RATE:Cosmic_Tracking', 'BW:Detector'], -1],
        #['id_cosmicid_trtxk_central', '', [], ['HLT_IDCosmic'],            ['RATE:CosmicSlice', 'RATE:Cosmic_Tracking', 'BW:Detector'], -1],

        # ['id_cosmicid_ds',            '', [], ['DataScouting_04_IDCosmic'], ['RATE:CosmicSlice', 'BW:Detector'], -1],        

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

# Egamma streamers
        ['noalg_L1EM18VH',         'L1_EM18VH',         [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM15',           'L1_EM15',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM12',           'L1_EM12',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L12EM7',           'L1_2EM7',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L12EM15',          'L1_2EM15',          [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM15VH_3EM7',    'L1_EM15VH_3EM7',    [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM7',            'L1_EM7',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L1EM3',            'L1_EM3',            [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],
        ['noalg_L12EM3',           'L1_2EM3',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Egamma'], -1],

        # Muon streamers
        ['noalg_L1MU20',           'L1_MU20',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU15',           'L1_MU15',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU10',           'L1_MU10',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU11',           'L1_MU11',           [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L12MU6',           'L1_2MU6',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L12MU10',          'L1_2MU10',          [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L12MU4',           'L1_2MU4',           [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],

        ['noalg_L1MU4_EMPTY',      'L1_MU4_EMPTY',      [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU4_FIRSTEMPTY', 'L1_MU4_FIRSTEMPTY', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU11_EMPTY',      'L1_MU11_EMPTY',      [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        ['noalg_L1MU4_UNPAIRED_ISO', 'L1_MU4_UNPAIRED_ISO', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],

        ['noalg_L1MU20_FIRSTEMPTY', 'L1_MU20_FIRSTEMPTY', [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Muon'], -1],
        
        ['noalg_mb_L1TE5',  'L1_TE5', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE10',  'L1_TE10', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ['noalg_mb_L1TE20',  'L1_TE20', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE30',  'L1_TE30', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ['noalg_mb_L1TE40',  'L1_TE40', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ##B['noalg_mb_L1TE50',  'L1_TE50', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ##B['noalg_mb_L1TE60',  'L1_TE60', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE70',  'L1_TE70', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

        ['noalg_mb_L1TE5.0ETA24',  'L1_TE5.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE10.0ETA24',  'L1_TE10.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ['noalg_mb_L1TE20.0ETA24',  'L1_TE20.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ['noalg_mb_L1TE30.0ETA24',  'L1_TE30.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ['noalg_mb_L1TE40.0ETA24',  'L1_TE40.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ##B['noalg_mb_L1TE50.0ETA24',  'L1_TE50.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1], 
        ##B['noalg_mb_L1TE60.0ETA24',  'L1_TE60.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],
        ['noalg_mb_L1TE70.0ETA24',  'L1_TE70.0ETA24', [], ['MinBias'], ["BW:MinBias", "RATE:MinBias"], -1],

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

        # MET streamers
        ['noalg_L1XE35',             'L1_XE35',             [], [PhysicsStream, 'express'], ['RATE:SeededStreamers', 'BW:MET'], -1],
        ['noalg_L1XE10',             'L1_XE10',             [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:MET'], -1],
        ['noalg_L1XE30',             'L1_XE30',             [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:MET'], -1],
        ['noalg_L1XE150',            'L1_XE150',             [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:MET'], -1],
        ['noalg_L1XE300',            'L1_XE300',             [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:MET'], -1],

#        ['noalg_L1J15_J15.31ETA49',         'L1_J15_J15.31ETA49',                [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Jet'], -1],
        ['noalg_L1J20_J20.31ETA49',                'L1_J20_J20.31ETA49',                [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:Jet'], -1],

        # VBF triggers
#        ['noalg_L1J30.0ETA49_2J20.0ETA49',    'L1_J30.0ETA49_2J20.0ETA49',    [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1],
        ['noalg_L1MJJ-100', 'L1_MJJ-100', [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1],  

        # Chains for Week1 menu (ATR-11119)
        ['noalg_L1MU4',   'L1_MU4',  [], [PhysicsStream, 'express'], ["RATE:SeededStreamers", "BW:Muon"], -1 ],
        ['noalg_L1MU6',   'L1_MU6',  [], [PhysicsStream,'express'], ["RATE:SeededStreamers", "BW:Muon"], -1 ],
        ['noalg_L1J50',   'L1_J50',  [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],

        #ATR-10976
#        ['noalg_L1MU4_3J15',   'L1_MU4_3J15',  [], [PhysicsStream], ["RATE:SeededStreamers", "BW:BJet"], -1 ],
#        ['noalg_L1MU4_J30',    'L1_MU4_J30',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:BJet"], -1 ],
        ['noalg_L1MU6_J20',    'L1_MU6_J20',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:BJet"], -1 ],
        ['noalg_L1MU4_J12',    'L1_MU4_J12',   [], [PhysicsStream], ['RATE:SeededStreamers', 'BW:BJet'], -1],

      
        ['noalg_L1CALREQ2',   'L1_CALREQ2', [], ['Tile'], ["RATE:Calibration", "RATE:Cosmic_TileCalibration", "BW:Detector"], -1],

        ['noalg_to_L12MU20_OVERLAY',   'L1_2MU20_OVERLAY', [], ['TauOverlay'], ["RATE:TauOverlay",  "BW:TauOverlay"], -1],

        # beam splash streamer
#        ['noalg_L1EM20A',   'L1_EM20A',        [], ['MinBias',  'express'], ["RATE:MinBias", "BW:MinBias"], -1],
#        ['noalg_L1EM20C',   'L1_EM20C',        [], ['MinBias',  'express'], ["RATE:MinBias", "BW:MinBias"], -1],
#        
#        ['noalg_L1J75A',   'L1_J75A',          [], ['MinBias',  'express'], ["RATE:MinBias", "BW:MinBias"], -1],
#        ['noalg_L1J75C',   'L1_J75C',          [], ['MinBias',  'express'], ["RATE:MinBias", "BW:MinBias"], -1],
#        
#        ['noalg_L1TAU20A',   'L1_TAU20A',      [], ['MinBias',  'express'], ["RATE:MinBias", "BW:MinBias"], -1], 
#        ['noalg_L1TAU20C',   'L1_TAU20C',      [], ['MinBias',  'express'], ["RATE:MinBias", "BW:MinBias"], -1], 
#
        # ALFA_ANY streamer
#ATR-13743        ['noalg_L1ALFA_ANY',     'L1_ALFA_ANY',     [], ['MinBias'], ["RATE:MinBias", "RATE:ALFA", "BW:Other"], -1], 
#        ['noalg_L1ALFA_Phys',    'L1_ALFA_Phys',    [], ['MinBias'], ["RATE:MinBias", "RATE:ALFA", "BW:Other"], -1], 
#        ['noalg_L1ALFA_PhysAny', 'L1_ALFA_PhysAny', [], ['MinBias'], ["RATE:MinBias", "RATE:ALFA", "BW:Other"], -1], 
#        ['noalg_L1ALFA_SYS',     'L1_ALFA_SYS',     [], ['MinBias'], ["RATE:MinBias", "RATE:ALFA", "BW:Other"], -1], 

        # ALFA Physics Streamer
#        ['noalg_L1ALFA_Diff_Phys',  'L1_ALFA_Diff_Phys',  [], ['MinBias'], ["RATE:ALFA","RATE:MinBias", "BW:Other"], -1], 
#        ['noalg_L1ALFA_CDiff_Phys', 'L1_ALFA_CDiff_Phys',  [], ['MinBias'], ["RATE:ALFA","RATE:MinBias", "BW:Other"], -1], 
#        ['noalg_L1ALFA_Jet_Phys', 'L1_ALFA_Jet_Phys',  [], ['MinBias'], ["RATE:ALFA","RATE:MinBias", "BW:Other"], -1], 

        # AFP Streamer
        ['noalg_L1AFP_NSC',  'L1_AFP_NSC',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_FSC',  'L1_AFP_FSC',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_ANY',  'L1_AFP_C_ANY',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_AND',  'L1_AFP_C_AND',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_ANY_EMPTY',  'L1_AFP_C_ANY_EMPTY',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_ANY_FIRSTEMPTY',  'L1_AFP_C_ANY_FIRSTEMPTY',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        
        ['noalg_L1AFP_C_EM3',  'L1_AFP_C_EM3',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_ANY_MBTS_A',  'L1_AFP_C_ANY_MBTS_A',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_MBTS_A',  'L1_AFP_C_MBTS_A',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_TE5',  'L1_AFP_C_TE5',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_J12',  'L1_AFP_C_J12',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1],
        ['noalg_L1AFP_C_MU4',  'L1_AFP_C_MU4',  [], ['MinBias'], [ 'RATE:MinBias','BW:MinBias'], -1], 

        # 
        ['noalg_L1TGC_BURST',   'L1_TGC_BURST', [], ['TgcNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],
        # ['noalg_L1TGC_BURST_EMPTY', 'L1_TGC_BURST_EMPTY', [], ['TgcNoiseBurst'], ["RATE:Calibration", "BW:Detector"], -1],        
        #DCM level monitoring test chain
#        ['noalg_dcmmon_L1RD0_EMPTY',   'L1_RD0_EMPTY',        [], ['monitoring_random'], ["RATE:DISCARD","BW:DISCARD"], -1], 
#
#        ['noalg_dcmmon_L1RD2_EMPTY',   'L1_RD2_EMPTY',        [], ['monitoring_random'], ["RATE:DISCARD","BW:DISCARD"], -1], 
#        ['noalg_dcmmon_L1RD2_BGRP12',  'L1_RD2_BGRP12',       [], ['monitoring_random'], ["RATE:DISCARD","BW:DISCARD"], -1], 
#
        #background streamers
        ['noalg_bkg_L1Bkg',               'L1_Bkg',               [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1MU4_UNPAIRED_ISO',  'L1_MU4_UNPAIRED_ISO',  [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J12',               'L1_J12',               [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J30.31ETA49',       'L1_J30.31ETA49',       [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J12_BGRP12',        'L1_J12_BGRP12',         [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_bkg_L1J30.31ETA49_BGRP12', 'L1_J30.31ETA49_BGRP12', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        #['noalg_bkg_L1RD0_UNPAIRED_ISO', 'L1_RD0_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        #['noalg_bkg_L1RD0_EMPTY',        'L1_RD0_EMPTY',        [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 
        #['noalg_bkg_L1RD1_FILLED',       'L1_RD1_FILLED',       [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1], 

        ['noalg_bkg_L1MBTS_2_UNPAIRED_ISO', 'L1_MBTS_2_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1RD0_UNPAIRED_ISO', 'L1_RD0_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1MBTS_4_A_UNPAIRED_ISO', 'L1_MBTS_4_A_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1MBTS_4_C_UNPAIRED_ISO', 'L1_MBTS_4_C_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],
        ['noalg_bkg_L1J15.31ETA49_UNPAIRED_ISO', 'L1_J15.31ETA49_UNPAIRED_ISO', [], ['Background'], ["RATE:SeededStreamers", "BW:Other"], -1],

        ['noalg_L1Standby',              'L1_Standby',          [], ['Standby'],    ["RATE:SeededStreamers", "BW:Other"], -1],         
        #['noalg_idcosmic_L1TRT',         'L1_TRT',              [], ['IDCosmic'],   ["RATE:SeededStreamers", "BW:Other"], -1],        
        ['noalg_idcosmic_L1TRT_EMPTY',          'L1_TRT_EMPTY',              [], ['IDCosmic',  'express'],   ["RATE:SeededStreamers", "BW:Other"], -1],        
        ['noalg_idcosmic_L1TRT_FILLED',         'L1_TRT_FILLED',              [], ['IDCosmic'],   ["RATE:SeededStreamers", "BW:Other"], -1],        

        # standby streamer
        # disabled (ATR-9101) ['noalg_L1Standby',  'L1_Standby',          [], ['Standby'],    ["RATE:SeededStreamers", "BW:Other"], -1], 
        #
        # L1 streamers
        #disabled see #104204   ['noalg_L1Muon',                'L1_Muon',             [], ['L1Muon'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        #disabled see #104204   ['noalg_L1Muon_EMPTY',          'L1_Muon_EMPTY',       [], ['L1Muon'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        
        ['noalg_L1Calo',                  'L1_Calo',             [], ['L1Calo'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_L1Topo',                  'L1_Topo',             [], ['L1Topo'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_larcells_L1LAR-EM',                'L1_LAR-EM',           [], ['LArCells'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_larcells_L1LAR-J',                 'L1_LAR-J',            [], ['LArCells'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        #['noalg_L1MinBias',               'L1_MinBias',          [], ['L1MinBias'],  ["RATE:SeededStreamers", "BW:Other"], -1], 
        ['noalg_L1Calo_EMPTY',            'L1_Calo_EMPTY',       [], ['L1Calo'],     ["RATE:SeededStreamers", "BW:Other"], -1], 
        # #['noalg_L1MinBias_EMPTY',      'L1_MinBias_EMPTY', [], ['L1MinBias'], [], -1], 

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
#ATR-13743         ['noalg_l1topo_L1MU4_J30',   'L1_MU4_J30',    [], ['L1Topo'],    ["RATE:SeededStreamers", "BW:Other"], -1],
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
        ['noalg_l1topo_L1MU4_J12', 'L1_MU4_J12', [], ['L1Topo'], ["RATE:SeededStreamers", "BW:Other"], -1],


	#L1Topo streamer (ATR-14714)
	["noalg_l1topo_L1J25_2J20_3J12_BOX-TAU20ITAU12I",   	"L1_J25_2J20_3J12_BOX-TAU20ITAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-MU10TAU12I_TAU12I-J25",          	"L1_DR-MU10TAU12I_TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU10_TAU12I-J25",          		"L1_MU10_TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1XE45_TAU20-J20",          		"L1_XE45_TAU20-J20",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1XE35_EM15-TAU12I",          		"L1_XE35_EM15-TAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1XE40_EM15-TAU12I",          		"L1_XE40_EM15-TAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-2M8-2MU4",          		"L1_BPH-2M8-2MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-8M15-MU6MU4",          		"L1_BPH-8M15-MU6MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-8M15-2MU6",          		"L1_BPH-8M15-2MU6",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BTAG-MU4J15",          		"L1_BTAG-MU4J15",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BTAG-MU4J30",          		"L1_BTAG-MU4J30",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BTAG-MU6J20",          		"L1_BTAG-MU6J20",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BTAG-MU6J25",          		"L1_BTAG-MU6J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J15_BTAG-MU4J15",          		"L1_3J15_BTAG-MU4J15",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J15_BTAG-MU4J30",          		"L1_3J15_BTAG-MU4J30",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J15_BTAG-MU6J25",          		"L1_3J15_BTAG-MU6J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_BTAG-MU4J20",          		"L1_3J20_BTAG-MU4J20",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1J40_DPHI-Js2XE50",          		"L1_J40_DPHI-Js2XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
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
# ATR-15197	["noalg_l1topo_L1KF-XE35",          			"L1_KF-XE35",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
# ATR-15197	["noalg_l1topo_L1KF-XE45",          			"L1_KF-XE45",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE55",          			"L1_KF-XE55",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE60",          			"L1_KF-XE60",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE65",          			"L1_KF-XE65",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1KF-XE75",          			"L1_KF-XE75",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM12_W-MT25",          		"L1_EM12_W-MT25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1EM12_W-MT30",          		"L1_EM12_W-MT30",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          "L1_EM15_W-MT35_W-250RO2-XEHT-0_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-05RO-XEHT-0",          		"L1_W-05RO-XEHT-0",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-90RO2-XEHT-0",          		"L1_W-90RO2-XEHT-0",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-250RO2-XEHT-0",          		"L1_W-250RO2-XEHT-0",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-HT20-JJ15.ETA49",          		"L1_W-HT20-JJ15.ETA49",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1W-NOMATCH",          			"L1_W-NOMATCH",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1W-NOMATCH_W-05RO-XEEMHT",          	"L1_W-NOMATCH_W-05RO-XEEMHT",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_XS60_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          "L1_EM15_W-MT35_XS60_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          "L1_EM15_W-MT35_XS40_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35",          		"L1_EM15_W-MT35",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_XS60",          		"L1_EM15_W-MT35_XS60",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15VH_W-MT35_XS60",          		"L1_EM15VH_W-MT35_XS60",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM20VH_W-MT35_XS60",          		"L1_EM20VH_W-MT35_XS60",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM22VHI_W-MT35_XS40",          	"L1_EM22VHI_W-MT35_XS40",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30",    	"L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE_XS30",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          	"L1_EM15_W-MT35_W-05DPHI-JXE-0_W-05DPHI-EM15XE",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT150-JJ15.ETA49_MJJ-400",         	"L1_HT150-JJ15.ETA49_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1BPH-1M19-2MU4_BPH-0DR34-2MU4",       	"L1_BPH-1M19-2MU4_BPH-0DR34-2MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4",    	"L1_BPH-2M8-MU6MU4_BPH-0DR15-MU6MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-2M9-2MU6_BPH-2DR15-2MU6",          "L1_BPH-2M9-2MU6_BPH-2DR15-2MU6",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6MU4-BO",          			"L1_MU6MU4-BO",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU4-B",          			"L1_2MU4-B",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU6-B",          			"L1_2MU6-B",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1BPH-1M19-2MU4-B_BPH-0DR34-2MU4",    	"L1_BPH-1M19-2MU4-B_BPH-0DR34-2MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1BPH-1M19-2MU4-BO_BPH-0DR34-2MU4",   	"L1_BPH-1M19-2MU4-BO_BPH-0DR34-2MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4",  	"L1_BPH-2M8-MU6MU4-B_BPH-0DR15-MU6MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU4-BO",          			"L1_2MU4-BO",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L12MU6-BO",          			"L1_2MU6-BO",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6_2MU4-B",          			"L1_MU6_2MU4-B",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DY-DR-2MU4",          			"L1_DY-DR-2MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#	["noalg_l1topo_L1DY-BOX-2MU4",          		"L1_DY-BOX-2MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#	["noalg_l1topo_L1DY-BOX-MU6MU4",          		"L1_DY-BOX-MU6MU4",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DY-BOX-2MU6",          		"L1_DY-BOX-2MU6",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LFV-MU",          			"L1_LFV-MU",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LFV-EM8I",          			"L1_LFV-EM8I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LFV-EM15I",          			"L1_LFV-EM15I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DPHI-J20s2XE50",          		"L1_DPHI-J20s2XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DPHI-J20XE50",          		"L1_DPHI-J20XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DPHI-CJ20XE50",          		"L1_DPHI-CJ20XE50",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-900",          			"L1_MJJ-900",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-800",          			"L1_MJJ-800",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-700",          			"L1_MJJ-700",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-400",          			"L1_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MJJ-100",          			"L1_MJJ-100",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1HT150-JJ15.ETA49",          		"L1_HT150-JJ15.ETA49",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#	["noalg_l1topo_L1DETA-JJ",          			"L1_DETA-JJ",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J4-MATCH",          			"L1_J4-MATCH",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LLP-NOMATCH",          		"L1_LLP-NOMATCH",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-MU10TAU12I",          		"L1_DR-MU10TAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1EM15-TAU40",          			"L1_EM15-TAU40",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	#["noalg_l1topo_L1EM15-TAU12I",          		"L1_EM15-TAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1EM15TAU12I-J25",          		"L1_EM15TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-EM15TAU12I-J25",          		"L1_DR-EM15TAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1TAU20ITAU12I-J25",          		"L1_TAU20ITAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-TAU20ITAU12I",          		"L1_DR-TAU20ITAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1BOX-TAU20ITAU12I",          		"L1_BOX-TAU20ITAU12I",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1DR-TAU20ITAU12I-J25",          	"L1_DR-TAU20ITAU12I-J25",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LAR-EM",          			"L1_LAR-EM",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1LAR-J",          			"L1_LAR-J",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6_MJJ-200",          		"L1_MU6_MJJ-200",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6_MJJ-300",         	 		"L1_MU6_MJJ-300",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6_MJJ-400",          		"L1_MU6_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1MU6_MJJ-500",          		"L1_MU6_MJJ-500",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-400",       	"L1_J30_2J20_4J20.0ETA49_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-700",       	"L1_J30_2J20_4J20.0ETA49_MJJ-700",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-800",     	"L1_J30_2J20_4J20.0ETA49_MJJ-800",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1J30_2J20_4J20.0ETA49_MJJ-900",         "L1_J30_2J20_4J20.0ETA49_MJJ-900",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-400",         	"L1_3J20_4J20.0ETA49_MJJ-400",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-700",          	"L1_3J20_4J20.0ETA49_MJJ-700",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-800",          	"L1_3J20_4J20.0ETA49_MJJ-800",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L13J20_4J20.0ETA49_MJJ-900",          	"L1_3J20_4J20.0ETA49_MJJ-900",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
	["noalg_l1topo_L1XE35_MJJ-200",          		"L1_XE35_MJJ-200",          [], ["L1Topo"], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],

 
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
        # L1 seed doesn't exits ['noalg_mb_L1RD1_UNPAIRED_ISO',  'L1_RD1_UNPAIRED_ISO', [], ['MinBias'], ["BW:Unpaired_MinBias", "RATE:Cosmic_MinBias"], -1],

        # LHCF
        ['noalg_L1LHCF',             'L1_LHCF',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1LHCF_UNPAIRED_ISO','L1_LHCF_UNPAIRED_ISO',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
        ['noalg_L1LHCF_EMPTY',       'L1_LHCF_EMPTY',        [], ['MinBias'], ["RATE:MinBias", "BW:MinBias"], -1],
                
        # id cosmic streamer
        #['noalg_cosmicid_L1TRT',    'L1_TRT',              [], ['IDCosmic'], ["BW:MinBias", "RATE:MinBias"], -1],

        #Zero bias streamer
        ['noalg_zb_L1ZB','L1_ZB', [], ['ZeroBias'], ["BW:ZeroBias", "RATE:ZeroBias"], -1],
        #Zero bias plus HLT jet
        ['j40_L1ZB','L1_ZB', [], ['ZeroBias'], ["BW:ZeroBias", "RATE:ZeroBias"], -1],
        

        # Cosmic calo streamer
        ['noalg_cosmiccalo_L1EM3_EMPTY',         'L1_EM3_EMPTY',        [], ['CosmicCalo'], ["BW:MinBias", "RATE:Cosmic_Calo"], -1],        
        ['noalg_cosmiccalo_L1RD1_EMPTY',         'L1_RD1_EMPTY',        [], ['CosmicCalo',  'express'], ["RATE:Calibration", "BW:Detector"], -1],        
        ['noalg_cosmiccalo_L1J3031ETA49_EMPTY',  'L1_J30.31ETA49_EMPTY',[], ['CosmicCalo'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],
        ['noalg_cosmiccalo_L1J12_EMPTY',         'L1_J12_EMPTY',        [], ['CosmicCalo',  'express'], ["BW:Jet", "RATE:Cosmic_Calo"], -1],
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

        # L1Topo streamers for M8
        #['noalg_L1MJJ-4',   'L1_MJJ-4',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1MJJ-3',   'L1_MJJ-3',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1MJJ-2',   'L1_MJJ-2',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1MJJ-1',   'L1_MJJ-1',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        
        #['noalg_L1MJJ-350-0',   'L1_MJJ-350-0',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1MJJ-300-0',   'L1_MJJ-300-0',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1MJJ-250-0',   'L1_MJJ-250-0',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1MJJ-200-0',   'L1_MJJ-200-0',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ], 
        
        #['noalg_L1HT200-J20s5.ETA49', 'L1_HT200-J20s5.ETA49', [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1HT190-J20s5.ETA49', 'L1_HT190-J20s5.ETA49', [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1HT1-J0.ETA49',   'L1_HT1-J0.ETA49',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        
        #['noalg_L1JPSI-1M5-EMs',  'L1_JPSI-1M5-EMs',  [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1JPSI-1M5-EM6s', 'L1_JPSI-1M5-EM6s', [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        #['noalg_L1JPSI-1M5-EM12s','L1_JPSI-1M5-EM12s',[], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],

        #['noalg_L1DETA-JJ',         'L1_DETA-JJ',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        
        # M9 L1Topo streamers
        ['noalg_L1LFV-MU',          'L1_LFV-MU',          [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
        ['noalg_L1DY-DR-2MU4',      'L1_DY-DR-2MU4',      [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#SX
#        ['noalg_L1BPH-DR-2MU4',     'L1_BPH-DR-2MU4',     [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],   
#        ['noalg_L1BPH-DR-2MU6',     'L1_BPH-DR-2MU6',     [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1BPH-DR-MU6MU4',   'L1_BPH-DR-MU6MU4',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1BPH-2M-2MU4',     'L1_BPH-2M-2MU4',     [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1BPH-2M-2MU6',     'L1_BPH-2M-2MU6',     [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1BPH-2M-MU6MU4',   'L1_BPH-2M-MU6MU4',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1BPH-4M8-2MU4',    'L1_BPH-4M8-2MU4',    [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1BPH-4M8-2MU6',    'L1_BPH-4M8-2MU6',    [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1BPH-4M8-MU6MU4',  'L1_BPH-4M8-MU6MU4',  [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1DY-BOX-2MU4',     'L1_DY-BOX-2MU4',     [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
#        ['noalg_L1DY-BOX-MU6MU4',   'L1_DY-BOX-MU6MU4',   [], [PhysicsStream], ["RATE:L1TopoStreamers", "BW:Other"], -1 ],
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
	['noalg_L1J120',  'L1_J120',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1],
	['noalg_L1J15',  'L1_J15',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
#	['noalg_L1J15.28ETA31',  'L1_J15.28ETA31',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
	['noalg_L1J15.31ETA49',  'L1_J15.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
	['noalg_L1J20',  'L1_J20',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J20.28ETA31',  'L1_J20.28ETA31',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J20.31ETA49',  'L1_J20.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J25',  'L1_J25',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J30',  'L1_J30',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J30.31ETA49',  'L1_J30.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J40',  'L1_J40',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J400',  'L1_J400',   [], [PhysicsStream, 'express'], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J50.31ETA49',  'L1_J50.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J75',  'L1_J75',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],
        ['noalg_L1J75.31ETA49',  'L1_J75.31ETA49',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ], 
        ['noalg_L1J85',  'L1_J85',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Jet"], -1 ],

        ['noalg_L1EM8VH',  'L1_EM8VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM22VHI',  'L1_EM22VHI',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
#ATR-15233        ['noalg_L1EM20VHI',  'L1_EM20VHI',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM20VH',  'L1_EM20VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM15VH',  'L1_EM15VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
        ['noalg_L1EM13VH',  'L1_EM13VH',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:Egamma"], -1 ],
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
	
        ['noalg_L1TE70',  'L1_TE70',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ##B['noalg_L1TE60',  'L1_TE60',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ##B['noalg_L1TE50',  'L1_TE50',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE40',  'L1_TE40',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE30',  'L1_TE30',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE20',  'L1_TE20',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE10',   'L1_TE10', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE5',   'L1_TE5', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE5.0ETA24',   'L1_TE5.0ETA24', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE10.0ETA24',   'L1_TE10.0ETA24', [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1TE20.0ETA24',  'L1_TE20.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE30.0ETA24',  'L1_TE30.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ['noalg_L1TE40.0ETA24',  'L1_TE40.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        ##B['noalg_L1TE50.0ETA24',  'L1_TE50.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
	##B['noalg_L1TE60.0ETA24',  'L1_TE60.0ETA24',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ], 
        
        ['noalg_L1XS30',  'L1_XS30',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
        ['noalg_L1XS20',  'L1_XS20',   [], [PhysicsStream], ["RATE:SeededStreamers", "BW:MET"], -1 ],
	
        ]

    TriggerFlags.MonitorSlice.signatures = [
        ['l1calooverflow','', [], ['L1Calo'], ['RATE:Monitoring','BW:Other'], -1],
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

        #['cscmon_L1MU10',   'L1_MU10',     [], ['CSC'], ["RATE:Monitoring", "BW:Detector"], -1],
        #['cscmon_L1EM3',    'L1_EM3',     [], ['CSC'], ["RATE:Monitoring", "BW:Detector"], -1],
        #['cscmon_L1J12',    'L1_J12',     [], ['CSC'], ["RATE:Monitoring", "BW:Detector"], -1],
        ['cscmon_L1All', 'L1_All',         [], ['CSC'], ["RATE:Monitoring", "BW:Detector"], -1]
        
        ## check L1 ['idmon_trkFS_L14J15', 'L1_4J15', [], [PhysicsStream], ['RATE:InDetTracking', 'BW:Detector'], -1],

        ]

    # Random Seeded EB chains which select at the HLT based on L1 TBP bits
    TriggerFlags.EnhancedBiasSlice.signatures = [
        # Enhanced bias HLT items
        ['eb_low_L1RD2_FILLED',               'L1_RD2_FILLED',             [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ['eb_high_L1RD2_FILLED',              'L1_RD2_FILLED',             [], ['EnhancedBias'], ["RATE:EnhancedBias", "BW:Detector"], -1],
        ]

    #TriggerFlags.GenericSlice.signatures = []
    

    
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
    from copy import deepcopy
    Prescales.HLTPrescales_cosmics = deepcopy(mydict)
    

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
        'j0_perf_bperf_L1J12_EMPTY',
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
        'tau1_cosmic_ptonly_L1MU4_EMPTY',
        'tau1_cosmic_track_L1MU4_EMPTY',
        'tau8_cosmic_ptonly',
        'tau8_cosmic_track',
        'tilecalib_laser',
        ]
    HLTPrescales_cosmics.update(dict(map(None,chain_list,len(chain_list)*[ [1, 0, 0] ])))


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
    'mb_sp900_pusup400_trk50_hmt_L1TE5',
    'mb_sp900_pusup400_trk60_hmt_L1TE5',
    'mb_sp1000_pusup450_trk70_hmt_L1TE5',
    'mb_sp1200_pusup500_trk80_hmt_L1TE5',
    'mb_sp1200_pusup500_trk90_hmt_L1TE5',
    'mb_sp1200_pusup500_trk100_hmt_L1TE5',
    'mb_sp1800_pusup700_trk110_hmt_L1TE5',
    'mb_sp2100_pusup900_trk120_hmt_L1TE5',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE5',
    'mb_sp1000_pusup450_trk70_hmt_L1TE10',
    'mb_sp1200_pusup500_trk80_hmt_L1TE10',
    'mb_sp1400_pusup550_trk90_hmt_L1TE10',
    'mb_sp1600_pusup600_trk100_hmt_L1TE10',
    'mb_sp1800_pusup700_trk110_hmt_L1TE10',
    'mb_sp2100_pusup900_trk120_hmt_L1TE10',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE10',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE10',
    'mb_sp1200_pusup500_trk80_hmt_L1TE15',
    'mb_sp1400_pusup550_trk90_hmt_L1TE15',
    'mb_sp1600_pusup600_trk100_hmt_L1TE15',
    'mb_sp1800_pusup700_trk110_hmt_L1TE15',
    'mb_sp2100_pusup900_trk120_hmt_L1TE15',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE15',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE15',
    'mb_sp1400_pusup550_trk90_hmt_L1TE20',
    'mb_sp1600_pusup600_trk100_hmt_L1TE20',
    'mb_sp1800_pusup700_trk110_hmt_L1TE20',
    'mb_sp2100_pusup900_trk120_hmt_L1TE20',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE20',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE20',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE20',
    'mb_sp1600_pusup600_trk100_hmt_L1TE25',
    'mb_sp1800_pusup700_trk110_hmt_L1TE25',
    'mb_sp2100_pusup900_trk120_hmt_L1TE25',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE25',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE25',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE25',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE25',
    'mb_sp1800_pusup700_trk110_hmt_L1TE30',
    'mb_sp2100_pusup900_trk120_hmt_L1TE30',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE30',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE30',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE30',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE30',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE40',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE40',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE40',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE40',
    'mb_sp900_trk50_hmt_L1TE5',
    'mb_sp900_trk60_hmt_L1TE5',
    'mb_sp1000_trk70_hmt_L1TE5',
    'mb_sp1200_trk80_hmt_L1TE5',
    'mb_sp1200_trk90_hmt_L1TE5',
    'mb_sp1200_trk100_hmt_L1TE5',
    'mb_sp1800_trk110_hmt_L1TE5',
    'mb_sp2100_trk120_hmt_L1TE5',
    'mb_sp2300_trk130_hmt_L1TE5',
    'mb_sp1000_trk70_hmt_L1TE10',
    'mb_sp1200_trk80_hmt_L1TE10',
    'mb_sp1400_trk90_hmt_L1TE10',
    'mb_sp1600_trk100_hmt_L1TE10',
    'mb_sp1800_trk110_hmt_L1TE10',
    'mb_sp2100_trk120_hmt_L1TE10',
    'mb_sp2300_trk130_hmt_L1TE10',
    'mb_sp2500_trk140_hmt_L1TE10',
    'mb_sp1200_trk80_hmt_L1TE15',
    'mb_sp1400_trk90_hmt_L1TE15',
    'mb_sp1600_trk100_hmt_L1TE15',
    'mb_sp1800_trk110_hmt_L1TE15',
    'mb_sp2100_trk120_hmt_L1TE15',
    'mb_sp2300_trk130_hmt_L1TE15',
    'mb_sp2500_trk140_hmt_L1TE15',
    'mb_sp1400_trk90_hmt_L1TE20',
    'mb_sp1600_trk100_hmt_L1TE20',
    'mb_sp1800_trk110_hmt_L1TE20',
    'mb_sp2100_trk120_hmt_L1TE20',
    'mb_sp2300_trk130_hmt_L1TE20',
    'mb_sp2500_trk140_hmt_L1TE20',
    'mb_sp2700_trk150_hmt_L1TE20',
    'mb_sp1600_trk100_hmt_L1TE25',
    'mb_sp1800_trk110_hmt_L1TE25',
    'mb_sp2100_trk120_hmt_L1TE25',
    'mb_sp2300_trk130_hmt_L1TE25',
    'mb_sp2500_trk140_hmt_L1TE25',
    'mb_sp2700_trk150_hmt_L1TE25',
    'mb_sp2900_trk160_hmt_L1TE25',
    'mb_sp1800_trk110_hmt_L1TE30',
    'mb_sp2100_trk120_hmt_L1TE30',
    'mb_sp2300_trk130_hmt_L1TE30',
    'mb_sp2500_trk140_hmt_L1TE30',
    'mb_sp2700_trk150_hmt_L1TE30',
    'mb_sp2900_trk160_hmt_L1TE30',
    'mb_sp2300_trk130_hmt_L1TE40',
    'mb_sp2500_trk140_hmt_L1TE40',
    'mb_sp2700_trk150_hmt_L1TE40',
    'mb_sp2900_trk160_hmt_L1TE40',
    'mb_sp900_pusup400_trk50_hmt_L1TE5.0ETA24',
    'mb_sp900_pusup400_trk60_hmt_L1TE5.0ETA24',
    'mb_sp1000_pusup450_trk70_hmt_L1TE5.0ETA24',
    'mb_sp1200_pusup500_trk80_hmt_L1TE5.0ETA24',
    'mb_sp1200_pusup500_trk90_hmt_L1TE5.0ETA24',
    'mb_sp1200_pusup500_trk100_hmt_L1TE5.0ETA24',
    'mb_sp1800_pusup700_trk110_hmt_L1TE5.0ETA24',
    'mb_sp2100_pusup900_trk120_hmt_L1TE5.0ETA24',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE5.0ETA24',
    'mb_sp1000_pusup450_trk70_hmt_L1TE10.0ETA24',
    'mb_sp1200_pusup500_trk80_hmt_L1TE10.0ETA24',
    'mb_sp1400_pusup550_trk90_hmt_L1TE10.0ETA24',
    'mb_sp1600_pusup600_trk100_hmt_L1TE10.0ETA24',
    'mb_sp1800_pusup700_trk110_hmt_L1TE10.0ETA24',
    'mb_sp2100_pusup900_trk120_hmt_L1TE10.0ETA24',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE10.0ETA24',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE10.0ETA24',
    'mb_sp1200_pusup500_trk80_hmt_L1TE15.0ETA24',
    'mb_sp1400_pusup550_trk90_hmt_L1TE15.0ETA24',
    'mb_sp1600_pusup600_trk100_hmt_L1TE15.0ETA24',
    'mb_sp1800_pusup700_trk110_hmt_L1TE15.0ETA24',
    'mb_sp2100_pusup900_trk120_hmt_L1TE15.0ETA24',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE15.0ETA24',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE15.0ETA24',
    'mb_sp1400_pusup550_trk90_hmt_L1TE20.0ETA24',
    'mb_sp1600_pusup600_trk100_hmt_L1TE20.0ETA24',
    'mb_sp1800_pusup700_trk110_hmt_L1TE20.0ETA24',
    'mb_sp2100_pusup900_trk120_hmt_L1TE20.0ETA24',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE20.0ETA24',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE20.0ETA24',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE20.0ETA24',
    'mb_sp1600_pusup600_trk100_hmt_L1TE25.0ETA24',
    'mb_sp1800_pusup700_trk110_hmt_L1TE25.0ETA24',
    'mb_sp2100_pusup900_trk120_hmt_L1TE25.0ETA24',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE25.0ETA24',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE25.0ETA24',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE25.0ETA24',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE25.0ETA24',
    'mb_sp1800_pusup700_trk110_hmt_L1TE30.0ETA24',
    'mb_sp2100_pusup900_trk120_hmt_L1TE30.0ETA24',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE30.0ETA24',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE30.0ETA24',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE30.0ETA24',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE30.0ETA24',
    'mb_sp2300_pusup1000_trk130_hmt_L1TE40.0ETA24',
    'mb_sp2500_pusup1100_trk140_hmt_L1TE40.0ETA24',
    'mb_sp2700_pusup1200_trk150_hmt_L1TE40.0ETA24',
    'mb_sp2900_pusup1300_trk160_hmt_L1TE40.0ETA24',
    'mb_sp400_trk40_hmt_L1MBTS_1_1',
    'mb_sp600_trk45_hmt_L1MBTS_1_1',
    'mb_sp700_trk55_hmt_L1MBTS_1_1',
    'mb_sp900_trk60_hmt_L1MBTS_1_1',
    'mb_sp900_trk65_hmt_L1MBTS_1_1',
    'mb_sp1000_trk70_hmt_L1MBTS_1_1',
    'mb_sp1200_trk75_hmt_L1MBTS_1_1',
    'mb_sp1400_trk80_hmt_L1MBTS_1_1',
    'mb_sp900_trk50_hmt_L1TE5.0ETA24',
    'mb_sp900_trk60_hmt_L1TE5.0ETA24',
    'mb_sp1000_trk70_hmt_L1TE5.0ETA24',
    'mb_sp1200_trk80_hmt_L1TE5.0ETA24',
    'mb_sp1200_trk90_hmt_L1TE5.0ETA24',
    'mb_sp1200_trk100_hmt_L1TE5.0ETA24',
    'mb_sp1800_trk110_hmt_L1TE5.0ETA24',
    'mb_sp2100_trk120_hmt_L1TE5.0ETA24',
    'mb_sp2300_trk130_hmt_L1TE5.0ETA24',
    'mb_sp1000_trk70_hmt_L1TE10.0ETA24',
    'mb_sp1200_trk80_hmt_L1TE10.0ETA24',
    'mb_sp1400_trk90_hmt_L1TE10.0ETA24',
    'mb_sp1600_trk100_hmt_L1TE10.0ETA24',
    'mb_sp1800_trk110_hmt_L1TE10.0ETA24',
    'mb_sp2100_trk120_hmt_L1TE10.0ETA24',
    'mb_sp2300_trk130_hmt_L1TE10.0ETA24',
    'mb_sp2500_trk140_hmt_L1TE10.0ETA24',
    'mb_sp1200_trk80_hmt_L1TE15.0ETA24',
    'mb_sp1400_trk90_hmt_L1TE15.0ETA24',
    'mb_sp1600_trk100_hmt_L1TE15.0ETA24',
    'mb_sp1800_trk110_hmt_L1TE15.0ETA24',
    'mb_sp2100_trk120_hmt_L1TE15.0ETA24',
    'mb_sp2300_trk130_hmt_L1TE15.0ETA24',
    'mb_sp2500_trk140_hmt_L1TE15.0ETA24',
    'mb_sp1400_trk90_hmt_L1TE20.0ETA24',
    'mb_sp1600_trk100_hmt_L1TE20.0ETA24',
    'mb_sp1800_trk110_hmt_L1TE20.0ETA24',
    'mb_sp2100_trk120_hmt_L1TE20.0ETA24',
    'mb_sp2300_trk130_hmt_L1TE20.0ETA24',
    'mb_sp2500_trk140_hmt_L1TE20.0ETA24',
    'mb_sp2700_trk150_hmt_L1TE20.0ETA24',
    'mb_sp1600_trk100_hmt_L1TE25.0ETA24',
    'mb_sp1800_trk110_hmt_L1TE25.0ETA24',
    'mb_sp2100_trk120_hmt_L1TE25.0ETA24',
    'mb_sp2300_trk130_hmt_L1TE25.0ETA24',
    'mb_sp2500_trk140_hmt_L1TE25.0ETA24',
    'mb_sp2700_trk150_hmt_L1TE25.0ETA24',
    'mb_sp2900_trk160_hmt_L1TE25.0ETA24',
    'mb_sp1800_trk110_hmt_L1TE30.0ETA24',
    'mb_sp2100_trk120_hmt_L1TE30.0ETA24',
    'mb_sp2300_trk130_hmt_L1TE30.0ETA24',
    'mb_sp2500_trk140_hmt_L1TE30.0ETA24',
    'mb_sp2700_trk150_hmt_L1TE30.0ETA24',
    'mb_sp2900_trk160_hmt_L1TE30.0ETA24',
    'mb_sp2300_trk130_hmt_L1TE40.0ETA24',
    'mb_sp2500_trk140_hmt_L1TE40.0ETA24',
    'mb_sp2700_trk150_hmt_L1TE40.0ETA24',
    'mb_sp2900_trk160_hmt_L1TE40.0ETA24',
    'mb_sp800_hmtperf_L1TE5',
    'mb_sp800_hmtperf_L1TE10',
    'mb_sp1200_hmtperf_L1TE10',
    'mb_sp1200_hmtperf_L1TE15',
    'mb_sp1600_hmtperf_L1TE15',
    'mb_sp1600_hmtperf_L1TE20',
    'mb_sp2100_hmtperf_L1TE20',
    'mb_sp2100_hmtperf_L1TE25',
    'mb_sp2100_hmtperf_L1TE30',
    'mb_sp2100_hmtperf_L1TE40',
    'mb_sp800_hmtperf_L1TE5.0ETA24',
    'mb_sp800_hmtperf_L1TE10.0ETA24',
    'mb_sp1200_hmtperf_L1TE10.0ETA24',
    'mb_sp1200_hmtperf_L1TE15.0ETA24',
    'mb_sp1600_hmtperf_L1TE15.0ETA24',
    'mb_sp1600_hmtperf_L1TE20.0ETA24',
    'mb_sp2100_hmtperf_L1TE20.0ETA24',
    'mb_sp2100_hmtperf_L1TE25.0ETA24',
    'mb_sp2100_hmtperf_L1TE30.0ETA24',
    'mb_sp2100_hmtperf_L1TE40.0ETA24',
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
#ATR-13743    'mb_sptrk_vetombts2in_L1ALFA_CEP',
#    'mb_sptrk_vetombts2in_L1TRT_ALFA_EINE',
#    'mb_sptrk_vetombts2in_L1ALFA_ANY',
#    'mb_sptrk_vetombts2in_L1ALFA_ANY_UNPAIRED_ISO',
#    'mb_sptrk_vetombts2in_L1TRT_ALFA_ANY',
#    'mb_sptrk_vetombts2in_L1TRT_ALFA_ANY_UNPAIRED_ISO',
#    'mb_sptrk_vetombts2in_peb_L1ALFA_ANY',
#    'mb_sptrk_vetombts2in_peb_L1ALFA_ANY_UNPAIRED_ISO',
#    'mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY',
#    'mb_sptrk_vetombts2in_peb_L1TRT_ALFA_ANY_UNPAIRED_ISO',
    'mb_sptrk_vetombts2in_L1ZDC_AND',
    'mb_sptrk_vetombts2in_L1ZDC_A_C',
    'mb_sptrk_trk80_L1MBTS_2',
    'mb_sptrk_pt2_L1MBTS_2',
    'mb_sp_L1AFP_C_MBTS_A',
    'mb_sp_L1AFP_C_ANY_MBTS_A',
    'mb_sp_L1AFP_C_AND',          
    'mb_sptrk_pt2_L1AFP_C_AND',  
    'mb_sptrk_pt4_L1AFP_C_AND',  
    'mb_sptrk_pt6_L1AFP_C_AND',  
    'mb_sptrk_pt8_L1AFP_C_AND',  
    'mb_sptrk_trk3_L1AFP_C_AND', 
    'mb_sptrk_trk5_L1AFP_C_AND', 
    'mb_sptrk_trk10_L1AFP_C_AND',
    'mb_sptrk_trk15_L1AFP_C_AND',
    'mb_sptrk_trk80_L1AFP_C_AND',
    'mb_sp_vetospmbts2in_L1AFP_C_AND',
    'mb_sp_vetosp_L1AFP_C_AND',
    #
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
    'noalg_mb_L1TE5',
    'noalg_mb_L1TE5.0ETA24',
    'noalg_mb_L1TE10',
    'noalg_mb_L1TE10.0ETA24',
    'noalg_mb_L1TE20',
    'noalg_mb_L1TE20.0ETA24',
    'noalg_mb_L1TE30',
    'noalg_mb_L1TE30.0ETA24',
    'noalg_mb_L1TE40',
    'noalg_mb_L1TE40.0ETA24',
    ##B'noalg_mb_L1TE50',
    ##B'noalg_mb_L1TE50.0ETA24',
    ##B'noalg_mb_L1TE60',
    ##B'noalg_mb_L1TE60.0ETA24',
    'noalg_mb_L1TE70',
    'noalg_mb_L1TE70.0ETA24',
    'noalg_mb_L1ZDC_A',
    'noalg_mb_L1ZDC_C',
    'noalg_mb_L1ZDC_A_C',
    'noalg_mb_L1ZDC_AND',
    'zdcpeb_L1ZDC_A',  
    'zdcpeb_L1ZDC_C',   
    'zdcpeb_L1ZDC_AND', 
    'zdcpeb_L1ZDC_A_C',
    'timeburner',
    ]
Prescales.HLTPrescales_tight_physics_prescale.update(dict(map(None,chain_list,len(chain_list)*[ [-1, 0,-1] ])))
######################################################

