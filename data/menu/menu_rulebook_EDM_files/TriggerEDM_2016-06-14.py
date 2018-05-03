# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration

# ********************* WARNING **********************
#
# In order to preserve backward compatibility, please 
#                  !!!!DO NOT!!!
# modify an existing key, simply add the new
# one after the old one.
# If old one is not going to be needed leave the destionations tag list empty.
# (there are examples below)
# 
# This is to ensure we will be able to read datasets
# produced with older versions of the reconstruction program.
#
# ****************************************************

#type/key destination tag (this tag is configure in JO and selects the objects whcih go to ESD AOD (prefix)
# (note that for L2 order matters, there might be not enough space for this at the end of the list so add there low priority objects)

# In Trigger*List, 3rd field holds the category of the trigger container
# Current categories
#	Bjet
#	Bphys
#	Config
#	Egamma
#	ID
#	Jet
#	L1
#	MET
#	MinBias
#	Muon
#	Steer
#	Tau
#	Calo       <-- new
#	Tracking   <-- new
#	Combined   <-- new
#	Unknown    <-- should be temporary until you figure out what to classify it as
#	Other?...maybe inDet(ID?), Truth

from TrigEDMConfig.TriggerEDMRun1 import TriggerL2List,TriggerEFList,TriggerResultsRun1List

#####################
#####################
# Container Slimming
#
# Here are the definitions of the strings which will be used to slim containers
# String to separate EDM members
identifier = ".-"

# ID Triggers
# ID Variables to be slimmed away
UnusedIDVariables = ["trackParameterCovarianceMatrices", "parameterX", "parameterY", "parameterZ", "parameterPX",
                     "parameterPY", "parameterPZ", "parameterPosition", "caloExtension"]

# Combine them into a string
RemoveIDVariables = ".-"+identifier.join(UnusedIDVariables)

# Tau Triggers
# Tau Trigger Variables to be slimmed away
PanTauVars = [ "pantau_CellBasedInput_isPanTauCandidate", "pantau_CellBasedInput_DecayModeProto", "pantau_CellBasedInput_DecayMode",
               "pantau_CellBasedInput_BDTValue_1p0n_vs_1p1n", "pantau_CellBasedInput_BDTValue_1p1n_vs_1pXn",
               "pantau_CellBasedInput_BDTValue_3p0n_vs_3pXn", "pantau_CellBasedInput_BDTVar_Basic_NNeutralConsts",
               "pantau_CellBasedInput_BDTVar_Charged_JetMoment_EtDRxTotalEt", "pantau_CellBasedInput_BDTVar_Charged_StdDev_Et_WrtEtAllConsts",
               "pantau_CellBasedInput_BDTVar_Neutral_HLV_SumM", "pantau_CellBasedInput_BDTVar_Neutral_PID_BDTValues_BDTSort_1",
               "pantau_CellBasedInput_BDTVar_Neutral_PID_BDTValues_BDTSort_2", "pantau_CellBasedInput_BDTVar_Neutral_Ratio_1stBDTEtOverEtAllConsts",
               "pantau_CellBasedInput_BDTVar_Neutral_Ratio_EtOverEtAllConsts", "pantau_CellBasedInput_BDTVar_Neutral_Shots_NPhotonsInSeed",
               "pantau_CellBasedInput_BDTVar_Combined_DeltaR1stNeutralTo1stCharged", "pantau_CellBasedInput_BDTVar_Charged_HLV_SumM",
               "ptPanTauCellBasedProto", "etaPanTauCellBasedProto", "phiPanTauCellBasedProto", "mPanTauCellBasedProto", "ptPanTauCellBased",
               "etaPanTauCellBased", "phiPanTauCellBased", "mPanTauCellBased" ]

PFOs = ["chargedPFOLinks", "neutralPFOLinks", "pi0PFOLinks", "protoChargedPFOLinks", "protoNeutralPFOLinks", "protoPi0PFOLinks",
        "shotPFOLinks", "hadronicPFOLinks" ]

EFlow = ["etEflow", "mEflow", "nPi0", "nCharged", "etEflowTopo", "mEflowTopo", "ptRatioEflowTopo", "nPi0Topo", "nChargedTopo",
         "ptRatioEflow", "pi0ConeDR" ]

Samplings = ["cellBasedEnergyRing1", "cellBasedEnergyRing2", "cellBasedEnergyRing3", "cellBasedEnergyRing4", "cellBasedEnergyRing5",
             "cellBasedEnergyRing6", "cellBasedEnergyRing7", "PSSFraction", "ele_E237E277", "ele_PresamplerFraction",
             "ele_ECALFirstFraction", "TRT_NHT_OVER_NLT" ]

UnusedOldTau = ["ipZ0SinThetaSigLeadTrk", "trkWidth2", "numEffTopoClusters", "topoInvMass",
                "effTopoInvMass", "topoMeanDeltaR", "effTopoMeanDeltaR", "isolFrac", "stripWidth2",
                "nStrip", "seedCalo_eta", "seedCalo_phi", "trkAvgDist", "trkRmsDist"
                "lead2ClusterEOverAllClusterE", "lead3ClusterEOverAllClusterE", "secMaxStripEt",
                "sumEMCellEtOverLeadTrkPt", "hadLeakEt", "caloIso", "caloIsoCorrected" ]

UnusedProperties = ["Likelihood", "SafeLikelihood", "BDTEleScore", "BDTJetScoreSigTrans", "BDTJetScoreBkgTrans",
                    "vetoFlags", "isTauFlags", "trackFlags", "trackFilterProngs", "trackFilterQuality", "trackEtaStrip", "trackPhiStrip",
                    "TauJetVtxFraction" ]

UnusedFourMom = ["ptIntermediateAxis", "etaIntermediateAxis", "phiIntermediateAxis", "mIntermediateAxis",
                 "ptTauEtaCalib", "etaTauEtaCalib", "phiTauEtaCalib", "mTauEtaCalib", "EM_TES_scale"]

# Combine them into a string
UnusedTauVariables = PanTauVars + PFOs + EFlow + Samplings + UnusedOldTau + UnusedProperties + UnusedFourMom
RemoveTauVariables = ".-"+identifier.join(UnusedTauVariables)


# Done Slimming
#####################
#####################

# temporary functionality to discover version of EDM
####################
    

TriggerHLTList = [
 
    #steer
    ('TrigRoiDescriptor#HLT_secondaryRoI_HLT',                             'BS ESD AODFULL AODSLIM',  'Steer'),
    ('TrigOperationalInfo#HLT_OPI_HLT',                                    'BS',                      'Steer'),
    ('TrigOperationalInfo#HLT_EXPRESS_OPI_HLT',                            'BS ESD',                  'Steer'),
    ('TrigMonConfig#HLT_OPI_HLT_monitoring_config',                        'BS',                      'Steer'),
    ('TrigMonEvent#HLT_OPI_HLT_monitoring_event',                          'BS',                      'Steer'), 

    ('xAOD::TrigCompositeContainer#HLT_L1TopoComposite',                   'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::TrigCompositeAuxContainer#HLT_L1TopoCompositeAux.',            'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::TrigCompositeContainer#HLT_L1TopoMET',                         'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::TrigCompositeAuxContainer#HLT_L1TopoMETAux.',                  'BS ESD AODFULL AODSLIM',  'Steer'),  

    ('xAOD::TrigCompositeContainer#HLT_MuonRoICluster',                   'BS ESD AODFULL AODSLIM',  'Combined'),
    ('xAOD::TrigCompositeAuxContainer#HLT_MuonRoIClusterAux.',            'BS ESD AODFULL AODSLIM',  'Combined'),

    ('xAOD::TrigCompositeContainer#HLT_MuonCalibrationStream', 		   'DS',	 	      'Muon'),
    ('xAOD::TrigCompositeAuxContainer#HLT_MuonCalibrationStreamAux.',      'DS', 		      'Muon'),

    ('xAOD::EmTauRoIContainer#HLT_L1TopoEM',                               'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::EmTauRoIAuxContainer#HLT_L1TopoEMAux.',                        'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::EmTauRoIContainer#HLT_L1TopoTau',                              'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::EmTauRoIAuxContainer#HLT_L1TopoTauAux.',                       'BS ESD AODFULL AODSLIM',  'Steer'),  

    ('xAOD::MuonRoIContainer#HLT_L1TopoMuon',                              'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::MuonRoIAuxContainer#HLT_L1TopoMuonAux.',                       'BS ESD AODFULL AODSLIM',  'Steer'),  

    ('xAOD::JetRoIContainer#HLT_L1TopoJet',                                'BS ESD AODFULL AODSLIM',  'Steer'),  
    ('xAOD::JetRoIAuxContainer#HLT_L1TopoJetAux.',                         'BS ESD AODFULL AODSLIM',  'Steer'),  

    #calo
    ('xAOD::CaloClusterContainer#HLT_TrigCaloClusterMaker', 'BS ESD AODFULL', 'Calo'), 
    ('xAOD::CaloClusterTrigAuxContainer#HLT_TrigCaloClusterMakerAux.', 'BS ESD AODFULL', 'Calo'), 
    ('xAOD::CaloClusterContainer#HLT_TrigCaloClusterMaker_slw', 'BS ESD AODFULL', 'Calo'), 
    ('xAOD::CaloClusterTrigAuxContainer#HLT_TrigCaloClusterMaker_slwAux.', 'BS ESD AODFULL', 'Calo'), 
    ('xAOD::CaloClusterContainer#HLT_TrigCaloClusterMaker_topo', 'BS ESD', 'Calo'), 
    ('xAOD::CaloClusterTrigAuxContainer#HLT_TrigCaloClusterMaker_topoAux.','BS ESD', 'Calo'), 
    ('xAOD::CaloClusterContainer#HLT_TrigEFCaloCalibFex', 'BS ESD AODFULL AODSLIM', 'Calo'),
    ('xAOD::CaloClusterTrigAuxContainer#HLT_TrigEFCaloCalibFexAux.', 'BS ESD AODFULL AODSLIM', 'Calo'),

    #egamma
    ('xAOD::ElectronContainer#HLT_egamma_Electrons',                'BS ESD AODFULL AODSLIM', 'Egamma'),
    ('xAOD::ElectronTrigAuxContainer#HLT_egamma_ElectronsAux.',  'BS ESD AODFULL AODSLIM', 'Egamma'),
    ('xAOD::PhotonContainer#HLT_egamma_Photons',                    'BS ESD AODFULL AODSLIM', 'Egamma'),
    ('xAOD::PhotonTrigAuxContainer#HLT_egamma_PhotonsAux.',        'BS ESD AODFULL AODSLIM', 'Egamma'), 

    ('xAOD::TrigElectronContainer#HLT_L2ElectronFex',            'BS ESD AODFULL AODSLIM', 'Egamma'),
    ('xAOD::TrigElectronAuxContainer#HLT_L2ElectronFexAux.',     'BS ESD AODFULL AODSLIM', 'Egamma'), 
    ('xAOD::TrigElectronContainer#HLT_L2IDCaloFex',              'BS ESD AODFULL AODSLIM', 'Egamma'),
    ('xAOD::TrigElectronAuxContainer#HLT_L2IDCaloFexAux.',       'BS ESD AODFULL AODSLIM', 'Egamma'), 
    ('xAOD::TrigPhotonContainer#HLT_L2PhotonFex',                'BS ESD AODFULL AODSLIM', 'Egamma'),
    ('xAOD::TrigPhotonAuxContainer#HLT_L2PhotonFexAux.',         'BS ESD AODFULL AODSLIM', 'Egamma'), 

    ('xAOD::TrigEMCluster#HLT_TrigT2CaloEgamma',                    'BS ESD AODFULL', 'Egamma'), 
    ('xAOD::TrigEMClusterAuxContainer#HLT_TrigT2CaloEgammaAux.',    'BS ESD AODFULL', 'Egamma'), 
    ('xAOD::TrigRingerRings#HLT_TrigT2CaloEgamma',                  'BS ESD AODFULL', 'Egamma'), 
    ('xAOD::TrigRingerRingsAuxContainer#HLT_TrigT2CaloEgammaAux.',  'BS ESD AODFULL', 'Egamma'), 
    ('xAOD::TrigRNNOutput#HLT_TrigRingerNeuralFex',                 'BS ESD AODFULL', 'Egamma'), 
    ('xAOD::TrigRNNOutputAuxContainer#HLT_TrigRingerNeuralFexAux.', 'BS ESD AODFULL', 'Egamma'),   
    ('xAOD::TrigRNNOutput#HLT_TrigTRTHTCounts', 		    'BS ESD AODFULL AODSLIM', 'Egamma'),
    ('xAOD::TrigRNNOutputAuxContainer#HLT_TrigTRTHTCountsAux.',     'BS ESD AODFULL AODSLIM', 'Egamma'),

    #bphys
    ('xAOD::TrigBphysContainer#HLT_EFBMuMuFex',                               'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_EFBMuMuFexAux.',                        'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_EFBMuMuXFex',                              'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_EFBMuMuXFexAux.',                       'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_EFDsPhiPiFex',                             'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_EFDsPhiPiFexAux.',                      'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_EFMuPairs',                                'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_EFMuPairsAux.',                         'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_EFTrackMass',                              'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_EFTrackMassAux.',                       'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_EFMultiMuFex',                             'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_EFMultiMuFexAux.',                      'BS ESD AODFULL AODSLIM',  'Bphys'),

    ('xAOD::TrigBphysContainer#HLT_L2BMuMuFex',                                  'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2BMuMuFexAux.',                           'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_L2BMuMuXFex',                                 'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2BMuMuXFexAux.',                          'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_L2DiMuXFex',                                  'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2DiMuXFexAux.',                           'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_L2DsPhiPiFexDs',                              'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2DsPhiPiFexDsAux.',                       'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_L2DsPhiPiFexPhi',                             'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2DsPhiPiFexPhiAux.',                      'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_L2JpsieeFex',                                 'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2JpsieeFexAux.',                          'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_L2TrackMass',                                 'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2TrackMassAux.',                          'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_L2MultiMuFex',                                'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_L2MultiMuFexAux.',                         'BS ESD AODFULL AODSLIM', 'Bphys'),
    ('xAOD::TrigBphysContainer#HLT_EFBMuMuXFex_X',                               'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrigBphysAuxContainer#HLT_EFBMuMuXFex_XAux.',                        'BS ESD AODFULL AODSLIM',  'Bphys'),
    
    #minbias
    # MinBias algs attach single objects therefore the pattern is slightly different, note also changes in the EDMList which define feature -> collection mapping
    ('xAOD::TrigSpacePointCounts#HLT_spacepoints',			         'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigSpacePointCountsAuxContainer#HLT_spacepointsAux.',		 'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigT2MbtsBits#HLT_T2Mbts',					         'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigT2MbtsBitsAuxContainer#HLT_T2MbtsAux.',				 'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigVertexCounts#HLT_vertexcounts',			     		 'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigVertexCountsAuxContainer#HLT_vertexcountsAux.',			 'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigTrackCounts#HLT_trackcounts',			       		 'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigTrackCountsAuxContainer#HLT_trackcountsAux.',			 'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigT2ZdcSignals#HLT_zdcsignals',                                    'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('xAOD::TrigT2ZdcSignalsAuxContainer#HLT_zdcsignalsAux.',                    'BS ESD AODFULL AODSLIM',  'MinBias'),

    #taus
    ('xAOD::TauJetContainer#HLT_TrigTauRecMerged',                                 'BS ESD AODFULL AODSLIM', 'Tau'), 
    ('xAOD::TauJetAuxContainer#HLT_TrigTauRecMergedAux'+RemoveTauVariables,        'BS ESD AODFULL AODSLIM', 'Tau'), 
    ('xAOD::TauJetContainer#HLT_TrigTauRecPreselection',                           'BS ESD AODFULL AODSLIM', 'Tau'),
    ('xAOD::TauJetAuxContainer#HLT_TrigTauRecPreselectionAux'+RemoveTauVariables,  'BS ESD AODFULL AODSLIM', 'Tau'),

    ('xAOD::TauJetContainer#HLT_TrigTauRecCaloOnly', 				   '', 'Tau'),
    ('xAOD::TauJetAuxContainer#HLT_TrigTauRecCaloOnlyAux'+RemoveTauVariables, 	   '', 'Tau'),

    ('xAOD::TauTrackContainer#HLT_TrigTauRecMergedTracks',                                 'BS ESD AODFULL AODSLIM', 'Tau'), 
    ('xAOD::TauTrackAuxContainer#HLT_TrigTauRecMergedTracksAux.',        'BS ESD AODFULL AODSLIM', 'Tau'), 
    ('xAOD::TauTrackContainer#HLT_TrigTauRecPreselectionTracks',                           'BS ESD AODFULL AODSLIM', 'Tau'),
    ('xAOD::TauTrackAuxContainer#HLT_TrigTauRecPreselectionTracksAux.',  'BS ESD AODFULL AODSLIM', 'Tau'),

    ('xAOD::TauTrackContainer#HLT_TrigTauRecCaloOnlyTracks',    '', 'Tau'),
    ('xAOD::TauTrackAuxContainer#HLT_TrigTauRecCaloOnlyTracksAux.',    '', 'Tau'),


    ('xAOD::JetContainer#HLT_TrigTauJet',                                          'BS ESD AODFULL AODSLIM', 'Tau'), 
    ('xAOD::JetTrigAuxContainer#HLT_TrigTauJetAux.',                               'BS ESD AODFULL AODSLIM', 'Tau'), 

    #muons
    #L2 Muons
    ('xAOD::L2StandAloneMuonContainer#HLT_MuonL2SAInfo',                        'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::L2StandAloneMuonAuxContainer#HLT_MuonL2SAInfoAux.',                 'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::L2CombinedMuonContainer#HLT_MuonL2CBInfo',                          'BS ESD AODFULL AODSLIM',                'Muon'), #fp
    ('xAOD::L2CombinedMuonAuxContainer#HLT_MuonL2CBInfoAux.',                   'BS ESD AODFULL AODSLIM',                'Muon'), #fp
    ('xAOD::L2IsoMuonContainer#HLT_MuonL2ISInfo',                               'BS ESD AODFULL AODSLIM',                'Muon'), #fp
    ('xAOD::L2IsoMuonAuxContainer#HLT_MuonL2ISInfoAux.',                        'BS ESD AODFULL AODSLIM',                'Muon'), #fp

    #MuonEFInfo
    ('xAOD::MuonContainer#HLT_MuonEFInfo',                                         'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::MuonAuxContainer#HLT_MuonEFInfoAux.',                                  'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::TrackParticleContainer#HLT_MuonEFInfo_CombTrackParticles',             'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::TrackParticleAuxContainer#HLT_MuonEFInfo_CombTrackParticlesAux'+RemoveIDVariables,   'BS ESD AODFULL AODSLIM','Muon'),#fp
    ('xAOD::TrackParticleContainer#HLT_MuonEFInfo_ExtrapTrackParticles',           'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::TrackParticleAuxContainer#HLT_MuonEFInfo_ExtrapTrackParticlesAux'+RemoveIDVariables, 'BS ESD AODFULL AODSLIM', 'Muon'),#fp

    ('xAOD::MuonContainer#HLT_MuonEFInfo_FullScan',                                         'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::TrackParticleContainer#HLT_MuonEFInfo_ExtrapTrackParticles_FullScan',           'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::TrackParticleContainer#HLT_MuonEFInfo_CombTrackParticles_FullScan',             'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::MuonContainer#HLT_MuonEFInfo_MSonlyTrackParticles_FullScan',                    'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::MuonAuxContainer#HLT_MuonEFInfo_FullScanAux.',                                                               'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_MuonEFInfo_ExtrapTrackParticles_FullScanAux'+RemoveIDVariables, 'BS ESD AODFULL AODSLIM', 'Muon'),#fp
    ('xAOD::TrackParticleAuxContainer#HLT_MuonEFInfo_CombTrackParticles_FullScanAux'+RemoveIDVariables,   'BS ESD AODFULL AODSLIM','Muon'),#fp
    ('xAOD::MuonAuxContainer#HLT_MuonEFInfo_MSonlyTrackParticles_FullScanAux.',                                          'BS ESD AODFULL AODSLIM',                'Muon'),


    #eMuonEFInfo
    ('xAOD::MuonContainer#HLT_eMuonEFInfo',                                        'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::MuonAuxContainer#HLT_eMuonEFInfoAux.',                                 'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::TrackParticleContainer#HLT_eMuonEFInfo_CombTrackParticles',            'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::TrackParticleAuxContainer#HLT_eMuonEFInfo_CombTrackParticlesAux'+RemoveIDVariables,   'BS ESD AODFULL AODSLIM', 'Muon'),#fp
    ('xAOD::TrackParticleContainer#HLT_eMuonEFInfo_ExtrapTrackParticles',          'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::TrackParticleAuxContainer#HLT_eMuonEFInfo_ExtrapTrackParticlesAux'+RemoveIDVariables, 'BS ESD AODFULL AODSLIM', 'Muon'),#fp
    
    #MuTagIMO_EF
    ('xAOD::MuonContainer#HLT_MuTagIMO_EF',                                        'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::MuonAuxContainer#HLT_MuTagIMO_EFAux.',                                 'BS ESD AODFULL AODSLIM',                'Muon'),
    ('xAOD::TrackParticleContainer#HLT_MuTagIMO_EF_CombTrackParticles',            'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::TrackParticleAuxContainer#HLT_MuTagIMO_EF_CombTrackParticlesAux'+RemoveIDVariables,   'BS ESD AODFULL AODSLIM', 'Muon'),#fp
    ('xAOD::TrackParticleContainer#HLT_MuTagIMO_EF_ExtrapTrackParticles',          'BS ESD AODFULL AODSLIM',                'Muon'),#fp
    ('xAOD::TrackParticleAuxContainer#HLT_MuTagIMO_EF_ExtrapTrackParticlesAux'+RemoveIDVariables, 'BS ESD AODFULL AODSLIM', 'Muon'),#fp

    #MET
    ('xAOD::TrigMissingET#HLT_TrigEFMissingET',                                 'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigEFMissingETAux.',                          'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_TrigEFMissingET_noiseSupp',                       '',                                      'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigEFMissingET_noiseSuppAux.',                '',                                      'MET'),
    ('xAOD::TrigMissingET#HLT_TrigEFMissingET_FEB',                             'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigEFMissingET_FEBAux.',                      'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_TrigEFMissingET_topocl',                          'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigEFMissingET_topoclAux.',                   'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_TrigEFMissingET_topocl_PS',                       'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigEFMissingET_topocl_PSAux.',                'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_EFJetEtSum',                                      'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_EFJetEtSumAux.',                               'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_T2MissingET',                                     'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_T2MissingETAux.',                              'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_TrigEFMissingET_topocl_PUC',                      'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigEFMissingET_topocl_PUCAux.',               'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_EFMissingET_Fex_2sidednoiseSupp_PUC',             'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_EFMissingET_Fex_2sidednoiseSupp_PUCAux.',      'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingET#HLT_TrigEFMissingET_mht',                             'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigEFMissingET_mhtAux.',                      'BS ESD AODFULL AODSLIM',                'MET'),

    ('xAOD::TrigMissingET#HLT_TrigL2MissingET_FEB',                             'BS ESD AODFULL AODSLIM',                'MET'),
    ('xAOD::TrigMissingETAuxContainer#HLT_TrigL2MissingET_FEBAux.',                      'BS ESD AODFULL AODSLIM',                'MET'),

    #tracking
    #fp
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_EFID',             'BS ESD AODFULL',          'Bjet'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bphysics_EFID',         'BS ESD AODFULL',          'Bphys'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnvIOTRT_CosmicsN_EFID',    'BS ESD AODFULL',          'Cosmics'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_CosmicsN_EFID',         'BS ESD AODFULL',          'Cosmics'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Electron_EFID',         'BS ESD AODFULL AODSLIM',  'Egamma'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnvCombined_Electron_EFID', '',                        'Egamma'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_FullScan_EFID',         'BS ESD AODFULL',          'Bphys'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_minBias_EFID',          'BS ESD',                  'MinBias'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Muon_EFID',             'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Photon_EFID',           'BS ESD AODFULL',  'Egamma'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Tau_EFID',              'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_Tau_EFID',       '',          'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_Electron_EFID',  '',          'Egamma'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_Muon_EFID',      '',          'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_FullScan_EFID',  '',          'Bphys'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_MuonIso_EFID',          'BS ESD AODFULL',          'Muon'),
    #
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Electron_FTF',         'BS ESD AODFULL AODSLIM',  'Egamma'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Electron_L2ID',        'BS ESD AODFULL',          'Egamma'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTF',             'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTF',              'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_TauCore_FTF',          'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_TauIso_FTF',           'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTF',             'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_BjetPrmVtx_FTF',       'BS ESD AODFULL AODSLIM',  'Bjet'),

    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Electron_IDTrig',      'BS ESD AODFULL AODSLIM',  'Egamma'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Muon_IDTrig',          'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Tau_IDTrig',           'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_IDTrig',          'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bphysics_IDTrig',      'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bphysics_FTF',         'BS ESD AODFULL AODSLIM',  'Bphys'),

    #FTK
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTK',             'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTK',              'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTK',             'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTKRefit',        'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTKRefit',         'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTKRefit',        'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTK_IDTrig',      'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTK_IDTrig',       'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTK_IDTrig',      'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTKRefit_IDTrig', 'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTKRefit_IDTrig',  'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTKRefit_IDTrig', 'BS ESD AODFULL AODSLIM',  'Bjet'),

    #EF tracking
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_EFIDAux'+RemoveIDVariables,             'BS ESD AODFULL',          'Bjet'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bphysics_EFIDAux'+RemoveIDVariables,         'BS ESD AODFULL',          'Bphys'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnvIOTRT_CosmicsN_EFIDAux'+RemoveIDVariables,    'BS ESD AODFULL',          'Cosmics'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_CosmicsN_EFIDAux'+RemoveIDVariables,         'BS ESD AODFULL',          'Cosmics'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Electron_EFIDAux'+RemoveIDVariables,         'BS ESD AODFULL AODSLIM',  'Egamma'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnvCombined_Electron_EFIDAux'+RemoveIDVariables, 'BS ESD AODFULL AODSLIM',  'Egamma'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_FullScan_EFIDAux'+RemoveIDVariables,         'BS ESD AODFULL',          'Bphys'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_minBias_EFIDAux'+RemoveIDVariables,          'BS ESD',                  'MinBias'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Muon_EFIDAux'+RemoveIDVariables,             'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Photon_EFIDAux'+RemoveIDVariables,           'BS ESD AODFULL',  'Egamma'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Tau_EFIDAux'+RemoveIDVariables,              'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_Tau_EFIDAux'+RemoveIDVariables,       'BS ESD AODFULL',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_Electron_EFIDAux'+RemoveIDVariables,  'BS ESD AODFULL',          'Egamma'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_Muon_EFIDAux'+RemoveIDVariables,      'BS ESD AODFULL',          'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnvTRTOnly_FullScan_EFIDAux'+RemoveIDVariables,  'BS ESD AODFULL',          'Bphys'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_MuonIso_EFIDAux'+RemoveIDVariables,          'BS ESD AODFULL',          'Muon'),
    #
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Electron_FTFAux'+RemoveIDVariables,          'BS ESD AODFULL',  'Egamma'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Electron_L2IDAux'+RemoveIDVariables,         'BS ESD AODFULL',  'Egamma'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTFAux'+RemoveIDVariables,              'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTFAux'+RemoveIDVariables,               'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_TauCore_FTFAux'+RemoveIDVariables,           'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_TauIso_FTFAux'+RemoveIDVariables,            'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTFAux'+RemoveIDVariables,              'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_BjetPrmVtx_FTFAux'+RemoveIDVariables,        'BS ESD AODFULL AODSLIM',  'Bjet'),

    #FTK
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTKAux'+RemoveIDVariables,              'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTKAux'+RemoveIDVariables,               'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTKAux'+RemoveIDVariables,              'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTKRefitAux'+RemoveIDVariables,         'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTKRefitAux'+RemoveIDVariables,          'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTKRefitAux'+RemoveIDVariables,         'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTK_IDTrigAux'+RemoveIDVariables,       'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTK_IDTrigAux'+RemoveIDVariables,        'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTK_IDTrigAux'+RemoveIDVariables,       'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Muon_FTKRefit_IDTrigAux'+RemoveIDVariables,  'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Tau_FTKRefit_IDTrigAux'+RemoveIDVariables,   'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_FTKRefit_IDTrigAux'+RemoveIDVariables,  'BS ESD AODFULL AODSLIM',  'Bjet'),

    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Electron_IDTrigAux'+RemoveIDVariables,       'BS ESD AODFULL',  'Egamma'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Muon_IDTrigAux'+RemoveIDVariables,           'BS ESD AODFULL AODSLIM',  'Muon'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Tau_IDTrigAux'+RemoveIDVariables,            'BS ESD AODFULL AODSLIM',  'Tau'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bjet_IDTrigAux'+RemoveIDVariables,           'BS ESD AODFULL AODSLIM',  'Bjet'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bphysics_IDTrigAux'+RemoveIDVariables,       'BS ESD AODFULL AODSLIM',  'Bphys'),
    ('xAOD::TrackParticleAuxContainer#HLT_InDetTrigTrackingxAODCnv_Bphysics_FTFAux'+RemoveIDVariables,          'BS ESD AODFULL AODSLIM',  'Bphys'),

    #jets
    ('xAOD::JetContainer#HLT_TrigHLTJetDSSelectorCollection',            'DS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_TrigHLTJetDSSelectorCollectionAux.',  'DS ESD AODFULL', 'Jet'),

    ('xAOD::JetContainer#HLT_TrigHLTJetRec',				 '',                       'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_TrigHLTJetRecAux.',			 '',                       'Jet'),

    # antiKt0.4
    # em  clusters Full Scan
    ('xAOD::JetContainer#HLT_a4tcemFS',                                  '',                       'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemFSAux.',                       '',                       'Jet'),
    ('xAOD::JetContainer#HLT_a4tcemjesFS',                               'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemjesFSAux.',                    'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a4tcemsubFS',                               'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemsubFSAux.',                    'BS ESD AODFULL', 'Jet'),


    ('xAOD::JetContainer#HLT_a4tcemsubjesFS',                            'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemsubjesFSAux.',                 'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetContainer#HLT_a4tcemnojcalibFS',                          'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemnojcalibFSAux.',               'BS ESD AODFULL AODSLIM', 'Jet'),

    # em clusters Partial Scan
    ('xAOD::JetContainer#HLT_a4tcemPS',                                  '',                       'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemPSAux.',                       '',                       'Jet'),
    ('xAOD::JetContainer#HLT_a4tcemjesPS',                               'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemjesPSAux.',                    'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a4tcemnojcalibPS',                          'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tcemnojcalibPSAux.',               'BS ESD AODFULL', 'Jet'),

    # lcw clusters Full Scan
    ('xAOD::JetContainer#HLT_a4tclcwFS',                                 '',                       'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tclcwFSAux.',                      '',                       'Jet'),
    ('xAOD::JetContainer#HLT_a4tclcwjesFS',                              'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tclcwjesFSAux.',                   'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a4tclcwsubFS',                              'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tclcwsubFSAux.',                   'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a4tclcwsubjesFS',                           'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tclcwsubjesFSAux.',                'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a4tclcwnojcalibFS',                         'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tclcwnojcalibFSAux.',              'BS ESD AODFULL', 'Jet'),

    # lcw clusters Partial Scan
    ('xAOD::JetContainer#HLT_a4tclcwjesPS',                              'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tclcwjesPSAux.',                   'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a4tclcwnojcalibPS',                         'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4tclcwnojcalibPSAux.',              'BS ESD AODFULL', 'Jet'),

    # TriggerTower Full Scan
    ('xAOD::JetContainer#HLT_a4TTemnojcalibFS',                          'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4TTemnojcalibFSAux.',               'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a4TThadnojcalibFS',                         'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4TThadnojcalibFSAux.',              'BS ESD AODFULL', 'Jet'),

    # antiKt1.0
    # em  clusters Full Scan
    ('xAOD::JetContainer#HLT_a10tcemsubFS',                              'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tcemsubFSAux.',                   'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tcemjesFS',                              'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tcemjesFSAux.',                   'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tcemsubjesFS',                           'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tcemsubjesFSAux.',                'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tcemnojcalibFS',                         'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tcemnojcalibFSAux.',              'BS ESD AODFULL AODSLIM', 'Jet'),
    
    # em clusters Partial Scan
    ('xAOD::JetContainer#HLT_a10tcemjesPS',                              'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tcemjesPSAux.',                   'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tcemnojcalibPS',                         'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tcemnojcalibPSAux.',              'BS ESD AODFULL', 'Jet'),

    # lcw clusters Full Scan
    ('xAOD::JetContainer#HLT_a10tclcwsubFS',                             'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tclcwsubFSAux.',                  'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tclcwjesFS',                             'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tclcwjesFSAux.',                  'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tclcwsubjesFS',                          'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tclcwsubjesFSAux.',               'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tclcwnojcalibFS',                        'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tclcwnojcalibFSAux.',             'BS ESD AODFULL', 'Jet'),

    # lcw clusters Partial Scan
    ('xAOD::JetContainer#HLT_a10tclcwjesPS',                             'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tclcwjesPSAux.',                  'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10tclcwnojcalibPS',                        'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10tclcwnojcalibPSAux.',             'BS ESD AODFULL', 'Jet'),

    # TriggerTower Full Scan
    ('xAOD::JetContainer#HLT_a10TTemnojcalibFS',                         'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10TTemnojcalibFSAux.',              'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetContainer#HLT_a10TThadnojcalibFS',                        'BS ESD AODFULL', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10TThadnojcalibFSAux.',             'BS ESD AODFULL', 'Jet'),

    # Reclustered jets
    ('xAOD::JetContainer#HLT_a10r_tcemsubjesFS',                         'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a10r_tcemsubjesFSAux.',              'BS ESD AODFULL AODSLIM', 'Jet'),

    # Cosmics
    ('xAOD::JetContainer#HLT_TrigCosmicJetRec',                          '',                       'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_TrigCosmicJetRecAux.',               '',                       'Jet'),

    # R=0.4 jets
    ('xAOD::JetContainer#HLT_a4ionemsubjesFS', 'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a4ionemsubjesFSAux.', 'BS ESD AODFULL AODSLIM', 'Jet'),
    # R=0.3 jets
    ('xAOD::JetContainer#HLT_a3ionemsubjesFS', 'BS ESD AODFULL AODSLIM', 'Jet'),
    ('xAOD::JetTrigAuxContainer#HLT_a3ionemsubjesFSAux.', 'BS ESD AODFULL AODSLIM', 'Jet'),

    #btagging
    ('xAOD::BTaggingContainer#HLT_HLTBjetFex',                           'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::BTaggingTrigAuxContainer#HLT_HLTBjetFexAux.',                'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::BTagVertexContainer#HLT_BjetVertexFex', 	  	         'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::BTagVertexAuxContainer#HLT_BjetVertexFexAux.', 	         'BS ESD AODFULL AODSLIM', 'Bjet'),

    #vertex
    ('xAOD::VertexContainer#HLT_xPrimVx',                                'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexAuxContainer#HLT_xPrimVxAux.',                         'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexContainer#HLT_EFHistoPrmVtx',                          'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexAuxContainer#HLT_EFHistoPrmVtxAux.',                   'BS ESD AODFULL AODSLIM', 'Bjet'),  

    # FTK vertex
    ('xAOD::VertexContainer#HLT_PrimVertexFTK',                                'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexContainer#HLT_PrimVertexFTKRefit',                        'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexContainer#HLT_PrimVertexFTKRaw',                         'BS ESD AODFULL AODSLIM', 'Bjet'),

    # FTK vertexAux
    ('xAOD::VertexAuxContainer#HLT_PrimVertexFTKAux.',                                'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexAuxContainer#HLT_PrimVertexFTKRefitAux.',                        'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexAuxContainer#HLT_PrimVertexFTKRawAux.',                         'BS ESD AODFULL AODSLIM', 'Bjet'),

    # b-jets  
    ('xAOD::JetContainer#HLT_EFJet',                                     'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::JetContainer#HLT_FarawayJet',                                'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::JetTrigAuxContainer#HLT_FarawayJetAux.',                     'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::JetTrigAuxContainer#HLT_EFJetAux.',                          'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::JetContainer#HLT_SplitJet',                                  'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::JetTrigAuxContainer#HLT_SplitJetAux.',                       'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::JetContainer#HLT_SuperRoi',                                  'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::JetTrigAuxContainer#HLT_SuperRoiAux.',                       'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexContainer#HLT_BjetSecondaryVertexFex',                 'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexAuxContainer#HLT_BjetSecondaryVertexFexAux.',          'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexContainer#HLT_SecondaryVertex',                        'BS ESD AODFULL AODSLIM', 'Bjet'),
    ('xAOD::VertexAuxContainer#HLT_SecondaryVertexAux.',                 'BS ESD AODFULL AODSLIM', 'Bjet'), 

    ('xAOD::HIEventShapeContainer#HLT_HIUE',                          'BS ESD AODFULL AODSLIM', 'HeavyIon'),
    ('xAOD::HIEventShapeAuxContainer#HLT_HIUEAux.',                   'BS ESD AODFULL AODSLIM', 'HeavyIon'),

    # start of L2+EF list

#   ('TrigPassBits#HLT_passbits',                                         'BS ESD AODFULL AODSLIM', 'Steer'),

   ('xAOD::TrigPassBits#HLT_passbits',                 'BS ESD AODFULL AODSLIM', 'Steer'),
   ('xAOD::TrigPassBitsAuxContainer#HLT_passbitsAux.', 'BS ESD AODFULL AODSLIM', 'Steer'),

    # rois == L2 List
    ('TrigRoiDescriptor#HLT',                                             'BS ESD',                 'Steer'),
    ('TrigRoiDescriptor#HLT_forID',                                       'BS ESD AODFULL AODSLIM', 'Tracking'),
    ('TrigRoiDescriptor#HLT_forID1',                                      'BS ESD AODFULL AODSLIM', 'Tracking'),
    ('TrigRoiDescriptor#HLT_forID2',                                      'BS ESD AODFULL AODSLIM', 'Tracking'),
    ('TrigRoiDescriptor#HLT_forID3',                                      'BS ESD AODFULL AODSLIM', 'Tracking'),
    ('TrigRoiDescriptor#HLT_forMS',                                       'BS ESD AODFULL AODSLIM', 'Muon'),
    ('TrigRoiDescriptor#HLT_initialRoI',                                  'BS ESD AODFULL AODSLIM', 'Steer'),
    ('TrigRoiDescriptor#HLT_secondaryRoI_L2',                             'BS ESD AODFULL AODSLIM', 'Steer'),
    ('TrigRoiDescriptor#HLT_T2TauFinal',                                  '',                       'Tau'),
    #    ('TrigRoiDescriptor#HLT_TrigT2CaloEgamma',                            'BS ESD',                 'Egamma'), 
    ('TrigRoiDescriptor#HLT_TrigT2CaloJet',                               'BS ESD',                 'Jet'),
    ('TrigRoiDescriptor#HLT_TrigT2CosmicJet',                             'BS ESD',                 'Cosmics'),
    ('TrigRoiDescriptor#HLT_TrigT2CaloTau',                               '',                       'Tau'),
    #    ('TrigRoiDescriptor#HLT_TrigCaloRinger',                              '',                       'Egamma'), 
    ('TrigRoiDescriptor#HLT_ReverseRoI',                                  '',                       'Steer'),        # maybe L1?
    ('TrigRoiDescriptor#HLT_TrigJetRec',                                  'BS ESD',                 'Bjet'),
    ('TrigRoiDescriptor#HLT_SplitJet',                                    'BS ESD AODFULL',         'Tracking'),
    ('TrigRoiDescriptor#HLT_SuperRoi',                                    'BS ESD AODFULL',         'Tracking'),
    ('TrigPassBits#HLT_passbits',                                         'BS ESD AODFULL AODSLIM', 'Steer'),
    ('TrigPassFlags#HLT_passflags',                                       'BS ESD AODFULL AODSLIM', 'Steer'),
    
    # particles == L2 List
    ('MuonFeature#HLT',                                                   'BS ESD AODFULL AODSLIM', 'Muon'),
    ('IsoMuonFeature#HLT',                                                'BS ESD',                 'Muon'),
    ('TrigMuonClusterFeature#HLT_MuonCluster',                            'BS ESD AODFULL AODSLIM', 'Muon'),
    ('CombinedMuonFeature#HLT',                                           'BS ESD AODFULL AODSLIM', 'Muon'),
    ('TrigPhotonContainer#HLT_L2PhotonFex',                               '',                       'Egamma'),
    ('TrigT2Jet#HLT_TrigT2CaloJet',                                       '',                       'Jet'),
    ('TrigT2Jet#HLT_TrigT2CosmicJet',                                     '',                       'Cosmics'), # ?slim
    ('TrigTau#HLT',                                                       '',                       'Tau'),
    ('TileMuFeature#HLT',                                                 'BS ESD',                 'Muon'),
    ('TileTrackMuFeature#HLT',                                            'BS ESD',                 'Muon'),
    ('TrigElectronContainer#HLT_L2IDCaloFex',                             '',                       'Egamma'),  # ? is this still in use?? 
    ('TrigElectronContainer#HLT_L2ElectronFex',                           '',                       'Egamma'),
    ('TrigL2BjetContainer#HLT_L2BjetFex',                                 '',                       'Bjet'),
    ('TrigT2MbtsBits#HLT_T2Mbts',                                         'BS ESD AODFULL AODSLIM', 'MinBias'),
    ('TrigSpacePointCounts#HLT_spacepoints',                              'BS ESD AODFULL AODSLIM', 'MinBias'), # ? do we realy need it!
    ('TrigTrtHitCounts#HLT_TrtHitCount',                                  '',                       'MinBias'),
    ('TrigMissingET#HLT_T2MissingET',                                     'BS ESD AODFULL AODSLIM', 'MET'),
    ('TrigMissingET#HLT_L2JetEtSum',                                      'BS ESD AODFULL AODSLIM', 'MET'),
    ('TrigMissingET#HLT_L2MissingET_FEB',                                 'BS ESD AODFULL AODSLIM', 'MET'),
    ('ElectronMuonTopoInfo#HLT_EgMuTopoFEX',                              '',                       'Combined'),        # Muon? Egamma? Combined?
    ('ElectronMuonTopoInfo#HLT_L2_PhotonMuonTopoFEX',                     '',                       'Combined'),        # Muon? Egamma? Combined?

    #clusters == L2 List
    ('TrigEMCluster#HLT',                                                 '',                 'Egamma'),
    ('TrigEMCluster#HLT_TrigT2CaloEgamma',                                '',                 'Egamma'),
    ('TrigEMCluster#HLT_TrigCaloRinger',                                  '',                       'Egamma'),
    ('TrigEMCluster#HLT_TrigT2CaloCosmic',                                'BS ESD AODFULL',         'Cosmics'),
    ('TrigEMCluster#HLT_T2CaloSwCluster',                                 '',                 'Egamma'),
    ('TrigEMCluster#HLT_TrigcaloSwCluster',                               '',                 'Egamma'),
    ('TrigCaloCluster#HLT_TrigT2Calo',                                    '',                 'Egamma'),
    #    ('RingerRings#HLT_TrigT2CaloEgamma',                                  'BS ESD',                 'Egamma'), 
    #    ('RingerRings#HLT_TrigCaloRinger',                                    '',                       'Egamma'),
    ('TrigRNNOutput#HLT_TrigRingerNeuralFex',                             '',                 'Egamma'),
    ('TrigTauCluster#HLT_TrigT2CaloTau',                                  '',                       'Tau'),
    ('TrigTauClusterDetails#HLT_TrigT2CaloTauDetails',                    '',                       'Tau'),
    ('TrigTauTracksInfo#HLT',                                             '',                       'Tau'),
    ('TrigRNNOutput#HLT_TrigTRTHTCounts',                                 '',                 'Egamma'),

    # tracking == L2 List
   ('TrigInDetTrackCollection#HLT',                                      'BS ESD',                  'ID'),      # category?
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_Jet',                       '',                        'Bjet'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_Tau',                       '',                        'Tau'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_eGamma',                    '',                        'Egamma'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_Muon',                      '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_muonIso',                   '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_Bphysics',                  '',                        'Bphys'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_FullScan',                  '',                        'Bphys'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_Cosmics',                   '',                        'Cosmics'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_eGamma_Brem',               '',                        'Egamma'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_Tile',                      '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_eGamma',                   '',                        'Egamma'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Muon',                     '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_muonIso',                  '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Tau',                      '',                        'Tau'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Jet',                      '',                        'Bjet'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Bphysics',                 '',                        'Bphys'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_FullScan',                 '',                        'Bphys'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Cosmics',                  '',                        'Cosmics'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Tile',                     '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_eGamma_robust',            '',                        'Egamma'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Muon_robust',              '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_muonIso_robust',           '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Tau_robust',               '',                        'Tau'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Jet_robust',               '',                        'Jet'),
   ('TrigInDetTrackCollection#HLT_TrigIDSCAN_Tau_IDOnly',                '',                        'Tau'),
   ('TrigInDetTrackCollection#HLT_TrigSiTrack_Tile_robust',              '',                        'Muon'),
   ('TrigInDetTrackCollection#HLT_TRTSegmentFinder',                     '',                        'ID'),      # category?
   ('TrigInDetTrackCollection#HLT_TRTxK',                                '',                        'ID'),      # category?
   ('TrigVertexCollection#HLT_T2HistoPrmVtx',                            '',                        'Bjet'),

   # temporary fix for MinBias issue (ATR-9216)
   ('TrigVertexCollection#HLT_TrigL2SiTrackFinder_FullScan_ZF_Only',     'BS ESD AODFULL AODSLIM', 'MinBias'), #fp
   
   ('TrigVertexCollection#HLT_TrigBeamSpotVertex',                       'BS ESD AODFULL AODSLIM',  'Bjet'),
   ('TrigVertexCollection#HLT_TrigBeamSpotVertex_SiTrack',               '',                        'Bjet'),
   ('TrigOperationalInfo#HLT_OPI_L2',                                    'BS',                      'Steer'),
   ('TrigMonConfig#HLT_OPI_L2_monitoring_config',                        'BS',                      'Steer'),
   ('TrigMonEvent#HLT_OPI_L2_monitoring_event',                          'BS',                      'Steer'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_Muon' ,            '',  'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_eGamma' ,          '',                        'Egamma'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_eGamma' ,          '',                        'Egamma'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_muonIso' ,         '',  'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_Tau' ,             '',          'Tau'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_TauCore' ,         '',          'Tau'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_TauIso' ,          '',          'Tau'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_Jet' ,             '',          'Bjet'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_Bphysics' ,        '',          'Bphys'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_FullScan' ,        '',          'Bphys'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder_Tile' ,            '',          'Muon'),
   ('TrigInDetTrackCollection#HLT_TrigL2SiTrackFinder' ,                 '',          'Tracking'),
   ('TrigInDetTrackCollection#HLT_TrigFastTrackFinder_TrigInDetTrack_Muon', '',       'Muon'),

    # calocell == L2 List
    ('CaloCellContainer#HLT_TrigT2CaloEgammaCells',                       'BS ESD',   'Egamma'), 
    ('CaloCellContainer#HLT_TrigT2CaloTauCells',                          'BS ESD',   'Tau'), 

    #('CaloClusterCellLinkContainer#HLT_TrigCaloClusterMaker_Link',        'BS ESD', 'Calo'),

    ('MuonFeatureDetails#HLT',                                            'BS ESD AODFULL AODSLIM',  'Muon'),

    # == EF List
   ('TrigRoiDescriptor#HLT_secondaryRoI_EF',                             'BS ESD AODFULL AODSLIM',                'Steer'),
   ('TrigRoiDescriptor#HLT_TrigJetRec',                                  'BS ESD',                                'Steer'),
   ('TrigPassBits#HLT_passbits',                                         'BS ESD AODFULL AODSLIM',                'Steer'),
   ('TrigPassFlags#HLT_passflags',                                       'BS ESD AODFULL AODSLIM',                'Steer'),
   ('TrigPassFlags#HLT_isEM',                                            'BS ESD AODFULL AODSLIM',                'Steer'),
   ('CaloClusterContainer#HLT',                                          '',                                      'Calo'),       # Egamma?
   ('CaloClusterContainer#HLT_TrigCaloClusterMaker',                     '',                                      'Calo'),       # Egamma?
   ('CaloClusterContainer#HLT_TrigCaloClusterMaker_slw',                 '',                                      'Calo'),       # Egamma?
   ('CaloClusterContainer#HLT_TrigCaloClusterMaker_topo',                '',                                      'Calo'),       # Egamma?
#    ('CaloCellLinkContainer#HLT_TrigCaloClusterMaker_topo_Link',          'BS ESD',                                'Calo'),       # Egamma? reincludedd by FP
   ('TrigMuonEFContainer#HLT_MuonEF',                                    '',                                      'Muon'),
   ('TrigMuonEFInfoContainer#HLT_MuonEFInfo',                            'BS ESD AODFULL AODSLIM',                'Muon'),
   ('TrigMuonEFInfoContainer#HLT_eMuonEFInfo',                           'BS ESD AODFULL AODSLIM',                'Muon'),
   ('TrigMuonEFInfoContainer#HLT_MuTagIMO_EF',                           'BS ESD AODFULL AODSLIM',                'Muon'),
   ('TrigMuonEFIsolationContainer#HLT_MuonEFIsolation',                  'BS ESD AODFULL AODSLIM',                'Muon'),
    #    ('CaloShowerContainer#HLT',                                           'BS ESD',                                'Calo'),       # Egamma?  
   ('egammaContainer#HLT_egamma_Electrons',                              '',                                      'Egamma'),
   ('egammaContainer#HLT_egamma',                                        '',                                      'Egamma'),
   ('egammaContainer#HLT_egamma_Photons',                                '',                                      'Egamma'),
   ('TrigMissingET#HLT_TrigEFMissingET',                                 'BS ESD AODFULL AODSLIM',                'MET'),
   ('TrigMissingET#HLT_TrigEFMissingET_noiseSupp',                       '',                                      'MET'),
   ('TrigMissingET#HLT_TrigEFMissingET_FEB',                             'BS ESD AODFULL AODSLIM',                'MET'),
   ('TrigMissingET#HLT_TrigEFMissingET_topocl',                          'BS ESD AODFULL AODSLIM',                'MET'),
   ('TrigMissingET#HLT_TrigEFMissingET_topocl_PS',                       'BS ESD AODFULL AODSLIM',                'MET'),
   ('TrigMissingET#HLT_EFJetEtSum',                                      'BS ESD AODFULL AODSLIM',                'MET'),
   ('ElectronMuonTopoInfo#HLT_EF_PhotonMuonTopoFEX',                     '',               			  'Combined'),    #   Combined?
   ('ElectronMuonTopoInfo#HLT_EF_EgMuTopoFEX',                           '',             		          'Combined'),     #  Combined?
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Bjet_EFID',             '',           'Bjet'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Bphysics_EFID',         '',           'Bphys'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreationIOTRT_CosmicsN_EFID',    '',           'Cosmics'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_CosmicsN_EFID',         '',           'Cosmics'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Electron_EFID',         '',           'Egamma'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreationCombined_Electron_EFID', '',           'Egamma'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_FullScan_EFID',         '',           'Bphys'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_minBias_EFID',          '',           'MinBias'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Muon_EFID',             '',           'Muon'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Photon_EFID',           '',           'Egamma'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Tau_EFID',              '',           'Tau'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Tau_IDOnly_EFID',       '',           'Tau'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_Tau_SiTrk_EFID',        '',           'Tau'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreationTRTOnly_Tau_EFID',       '',           'Tau'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreationTRTOnly_Electron_EFID',  '',           'Egamma'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreationTRTOnly_Muon_EFID',      '',           'Muon'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreationTRTOnly_FullScan_EFID',  '',           'Bphys'),
   ('Rec::TrackParticleContainer#HLT_InDetTrigParticleCreation_MuonIso_EFID',          '',           'Muon'),

    # == EF List
    ('Analysis::TauDetailsContainer#HLT_TrigTauDetailsMerged',            '',  'Tau'),
    ('Analysis::TauDetailsContainer#HLT_TrigTauExtraDetailsMerged',       '',  'Tau'),
    ('Analysis::TauDetailsContainer#HLT_TrigTauDetailsCalo',              '',  'Tau'),
    
    # == EF List
    ('TrigTrackCounts#HLT',                                               '',                        'MinBias'),
    ('TrigTrackCounts#HLT_trackcounts',                                   'BS ESD AODFULL AODSLIM',  'MinBias'), #? this is the CMS trigger obj
    ('TrigVertexCounts#HLT_vertexcounts',                                 'BS ESD AODFULL AODSLIM',  'MinBias'),
    ('TrigEFBjetContainer#HLT_EFBjetFex',                                 'BS ESD AODFULL',          'Bjet'),
    ('Analysis::TauJetContainer#HLT_TrigTauRecMerged',                    '',  'Tau'),
    ('Analysis::TauJetContainer#HLT_TrigTauRecCalo',                      '',  'Tau'),
    ('JetCollection#HLT',                                                 '',                        'Jet'),
    ('JetKeyDescriptor#HLT_TrigJetKeyMap',                                '',                        'Jet'),        # ESD AODFULL AODSLIM'),
    ('JetMomentMap#HLT_TrigJetRecMomentMap',                              '',                        'Jet'),        #  # ESD AODFULL AODSLIM'),
    ('JetCollection#HLT_TrigJetRec',                                      '',                        'Jet'),
    ('JetCollection#HLT_TrigCosmicJetRec',                                '',                        'Cosmics'),
    ('JetCollection#HLT_TrigTauJet',                                      '',                        'Tau'),
   #    ('CaloTowerContainer#HLT_TrigCaloTowerMaker',                         '',                        'Calo'),       # Egamma?  
    ('egDetailContainer#HLT_egamma_Electrons',                            '',                        'Egamma'),
    ('egDetailContainer#HLT_egamma',                                      '',                        'Egamma'),
    ('egDetailContainer#HLT_egamma_Photons',                              '',                        'Egamma'),
    ('TrackCollection#TrigFastTrackFinder_Tau',                           '', 'Tau'), 
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_Bjet_EFID',               'ESD',                     'Bjet'),
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_Bphysics_EFID',           'ESD',                     'Bphys'),
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_CosmicsN_EFID',           'ESD',                     'Cosmics'),
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_Electron_EFID',           'ESD',                     'Egamma'),
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_FullScan_EFID',           'ESD',                     'Bphys'),
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_Muon_EFID',               'ESD',                     'Muon'),
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_Photon_EFID',             'ESD',                     'Egamma'),
    ('TrackCollection#HLT_InDetTrigTrackSlimmer_Tau_EFID',                'ESD',                     'Tau'),
    ('TrigVertexCollection#HLT_EFHistoPrmVtx',                            '',                        'Bjet'),        # category?
    ('VxContainer#HLT_PrimVx',                                            '',                        'Bjet'),
    ('VxContainer#HLT_ConversionVx',                                      '',                        'Bjet'),
    ('TrigOperationalInfo#HLT_OPI_EF',                                    'BS',                      'Steer'),
    ('TrigOperationalInfo#HLT_EXPRESS_OPI_EF',                            'BS',                      'Steer'),
    ('TrigMonConfig#HLT_OPI_EF_monitoring_config',                        'BS',                      'Steer'),
    ('TrigMonEvent#HLT_OPI_EF_monitoring_event',                          'BS',                      'Steer'),
    ('CaloCellContainer#HLT_TrigCaloCellMaker_eGamma',                    'BS ESD',                  'Calo'), 
    ('CaloCellContainer#HLT_TrigCaloCellMaker_tau',                       'BS ESD',                  'Calo'),  
    ('CaloCellContainer#HLT_TrigCaloCellLoF',                             'BS ESD',                  'Calo'),        # category?  
    #('CaloCellContainer#HLT_TrigCaloCellMaker', 			  'BS ESD', 		     'Calo'),
    # == end of L2+EF lists


]

TriggerResultsList=[
    ('TrigDec::TrigDecision#TrigDecision' ,           '', 'Steer'),#still needed for Run2?
    ('xAOD::TrigDecision#xTrigDecision' ,             'ESD AODFULL AODSLIM', 'Steer'),
    ('xAOD::TrigDecisionAuxInfo#xTrigDecisionAux.',   'ESD AODFULL AODSLIM', 'Steer'),
    ('xAOD::TrigConfKeys#TrigConfKeys' ,              'ESD AODFULL AODSLIM', 'Steer'),
    #    ('HLT::HLTResult#HLTResult_L2',                    'ESD AODFULL AODSLIM', 'Steer'),#still needed for Run2?
    #    ('HLT::HLTResult#HLTResult_EF' ,                   'ESD AODFULL AODSLIM', 'Steer'),#still needed for Run2?
    ('HLT::HLTResult#HLTResult_HLT',                  'ESD', 'Steer'),
    ('xAOD::TrigNavigation#TrigNavigation' ,          'ESD AODFULL AODSLIM', 'Steer'),
    ('xAOD::TrigNavigationAuxInfo#TrigNavigationAux.','ESD AODFULL AODSLIM', 'Steer')
    ]


TriggerLvl1List=[
    ('xAOD::MuonRoIContainer#LVL1MuonRoIs' ,          'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::MuonRoIAuxContainer#LVL1MuonRoIsAux.' ,   'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::EmTauRoIContainer#LVL1EmTauRoIs' ,        'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::EmTauRoIAuxContainer#LVL1EmTauRoIsAux.' , 'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::JetRoIContainer#LVL1JetRoIs' ,            'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::JetRoIAuxContainer#LVL1JetRoIsAux.' ,     'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::JetEtRoI#LVL1JetEtRoI' ,                  'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::JetEtRoIAuxInfo#LVL1JetEtRoIAux.' ,       'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::EnergySumRoI#LVL1EnergySumRoI' ,          'ESD AODFULL AODSLIM', 'L1'),
    ('xAOD::EnergySumRoIAuxInfo#LVL1EnergySumRoIAux.','ESD AODFULL AODSLIM', 'L1'),

    ('xAOD::L1TopoRawDataContainer#L1TopoRawData'       , 'ESD AODFULL', 'L1'),
    ('xAOD::L1TopoRawDataAuxContainer#L1TopoRawDataAux.', 'ESD AODFULL', 'L1'),

    ('DataVector<LVL1::JetElement>#JetElements' ,        'ESD', 'L1'),
    ('DataVector<LVL1::JetElement>#JetElementsOverlap' , 'ESD', 'L1'),
    ('DataVector<LVL1::CPMTower>#CPMTowers' ,            'ESD', 'L1'),
    ('DataVector<LVL1::CPMTower>#CPMTowersOverlap' ,     'ESD', 'L1'),
    ('DataVector<LVL1::CPMHits>#CPMHits' ,               'ESD', 'L1'),
    ('DataVector<LVL1::CPMRoI>#CPMRoIs' ,                'ESD', 'L1'),
    ('DataVector<LVL1::CMMCPHits>#CMMCPHits' ,           'ESD', 'L1'),
    ('DataVector<LVL1::CMMJetHits>#CMMJetHits' ,         'ESD', 'L1'),
    ('DataVector<LVL1::CMMEtSums>#CMMEtSums' ,           'ESD', 'L1'),
    ('DataVector<LVL1::JEMHits>#JEMHits' ,               'ESD', 'L1'),
    ('DataVector<LVL1::JEMRoI>#JEMRoIs' ,                'ESD', 'L1'),
    ('DataVector<LVL1::JEMEtSums>#JEMEtSums' ,           'ESD', 'L1'),
    ('DataVector<LVL1::RODHeader>#RODHeaders' ,          'ESD', 'L1'),
    ('LVL1::CMMRoI#CMMRoIs' ,                            'ESD', 'L1'),
    # =========================================================================
    ('CTP_RDO#CTP_RDO' ,                                 'ESD', 'Steer'),
    ('MuCTPI_RDO#MUCTPI_RDO' ,                           'ESD', 'Unknown'),
    ('ROIB::RoIBResult#RoIBResult' ,                     'ESD', 'Steer'),

    # from LVL1 bytestream decoders: ReadLVL1CaloBSRun2_jobOptions.py
    # ============================================================================
    # xAOD
    # ============================================================================
    ('xAOD::TriggerTowerContainer#xAODTriggerTowers' ,          'ESD', 'L1'),
    ('xAOD::TriggerTowerAuxContainer#xAODTriggerTowersAux.' ,   'ESD', 'L1'),
    ('xAOD::TriggerTowerContainer#xAODTriggerTowersMuon' ,       'ESD', 'L1'),
    ('xAOD::TriggerTowerAuxContainer#xAODTriggerTowersMuonAux.' ,   'ESD', 'L1'),

    ('xAOD::CPMTowerContainer#CPMTowers' ,                      'ESD', 'L1'),
    ('xAOD::CPMTowerAuxContainer#CPMTowersAux.' ,               'ESD', 'L1'),
    ('xAOD::CPMTowerContainer#CPMTowersOverlap' ,               'ESD', 'L1'),
    ('xAOD::CPMTowerAuxContainer#CPMTowersOverlapAux.' ,        'ESD', 'L1'),
   
    ('xAOD::CMXCPTobContainer#CMXCPTobs',                       'ESD', 'L1'),
    ('xAOD::CMXCPTobAuxContainer#CMXCPTobsAux.',                'ESD', 'L1'),
    
    ('xAOD::CMXCPHitsContainer#CMXCPHits',                      'ESD', 'L1'),
    ('xAOD::CMXCPHitsAuxContainer#CMXCPHitsAux.',               'ESD', 'L1'),
    
    ('xAOD::CMXJetTobContainer#CMXJetTobs',                     'ESD', 'L1'),
    ('xAOD::CMXJetTobAuxContainer#CMXJetTobsAux.',              'ESD', 'L1'),
    
    ('xAOD::CMXJetHitsContainer#CMXJetHits',                    'ESD', 'L1'),
    ('xAOD::CMXJetHitsAuxContainer#CMXJetHitsAux.',             'ESD', 'L1'),
    
    ('xAOD::CMXEtSumsContainer#CMXEtSums',                      'ESD', 'L1'),
    ('xAOD::CMXEtSumsAuxContainer#CMXEtSumsAux.',               'ESD', 'L1'),
    
    ('xAOD::JEMEtSumsContainer#JEMEtSums',                      'ESD', 'L1'),
    ('xAOD::JEMEtSumsAuxContainer#JEMEtSumsAux.',               'ESD', 'L1'),
    
    ('xAOD::CPMTobRoIContainer#CPMTobRoIs',                     'ESD', 'L1'),
    ('xAOD::CPMTobRoIAuxContainer#CPMTobRoIsAux.',              'ESD', 'L1'),
    
    ('xAOD::CPMTobRoIContainer#CPMTobRoIsRoIB',                 'ESD', 'L1'),
    ('xAOD::CPMTobRoIAuxContainer#CPMTobRoIsRoIBAux.',          'ESD', 'L1'),
    
    ('xAOD::JEMTobRoIContainer#JEMTobRoIs',                     'ESD', 'L1'),
    ('xAOD::JEMTobRoIAuxContainer#JEMTobRoIsAux.',              'ESD', 'L1'),
    
    ('xAOD::JEMTobRoIContainer#JEMTobRoIsRoIB',                 'ESD', 'L1'),
    ('xAOD::JEMTobRoIAuxContainer#JEMTobRoIsRoIBAux.',          'ESD', 'L1'),
    
    ('xAOD::JetElementContainer#JetElements' ,                  'ESD', 'L1'),
    ('xAOD::JetElementAuxContainer#JetElementsAux.' ,           'ESD', 'L1'),
    
    ('xAOD::JetElementContainer#JetElementsOverlap' ,           'ESD', 'L1'),
    ('xAOD::JetElementAuxContainer#JetElementsOverlapAux.' ,    'ESD', 'L1'),
    
    ('xAOD::RODHeaderContainer#RODHeaders' ,                    'ESD', 'L1'),
    ('xAOD::RODHeaderAuxContainer#RODHeadersAux.' ,             'ESD', 'L1'),

    ('xAOD::CMXRoIContainer#CMXRoIs' ,                          'ESD', 'L1'),
    ('xAOD::CMXRoIAuxContainer#CMXRoIsAux.' ,                   'ESD', 'L1'),
    # =========================================================================

    ]


TriggerIDTruth= [
    ('TrigInDetTrackTruthMap#TrigInDetTrackTruthMap', 'ESD AODFULL AODSLIM', 'Unknown')
    ]


TriggerRoiList = [
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_secondaryRoI_L2" , "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_secondaryRoI_L2Aux.", 
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_secondaryRoI_EF" , "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_secondaryRoI_EFAux.",
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_secondaryRoI_HLT", "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_secondaryRoI_HLTAux.",
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_initialRoI" ,      "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_initialRoIAux." ,    
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_ReverseRoI" ,      "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_ReverseRoIAux." ,    
    #    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore" ,          
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_forID" ,           "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_forIDAux." ,         
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_forID1" ,          "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_forID1Aux." ,         
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_forID2" ,          "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_forID2Aux." ,         
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_forID3" ,          "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_forID3Aux." ,         
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_forMS" ,           "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_forMSAux." ,         
    #   "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_TrigT2CaloJet",
    #   "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_TrigT2CosmicJet",
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_T2TauFinal" ,      "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_T2TauFinalAux." ,       
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_TrigT2CaloTau" ,   "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_TrigT2CaloTauAux." ,    
    #   "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_TrigJetRec" ,   
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_SplitJet",         "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_SplitJetAux." ,         
    "xAOD::RoiDescriptorStore#HLT_xAOD__RoiDescriptorStore_SuperRoi",         "xAOD::RoiDescriptorStoreAuxInfo#HLT_xAOD__RoiDescriptorStore_SuperRoiAux."          
    ]
 

# below go details of Trigger EDM

EDMDetails = {}

EDMDetails[ "TrigRoiDescriptor" ]         = {'persistent':"TrigRoiDescriptorCollection_p3",     'typealias':'Roi', 'collection':'TrigRoiDescriptorCollection' }
#EDMDetails[ "TrigRoiDescriptor" ]         = {'persistent':"TrigRoiDescriptorCollection_tlp1",     'typealias':'Roi', 'collection':'TrigRoiDescriptorCollection' }
EDMDetails[ "TrigOperationalInfo" ]       = {'persistent':"TrigOperationalInfoCollection_tlp1",   'typealias':'', 'collection':'TrigOperationalInfoCollection' }    
EDMDetails[ "TrigMonConfig" ]             = {'persistent':"TrigMonConfigCollection_tlp1",         'typealias':'', 'collection':'TrigMonConfigCollection' }
EDMDetails[ "TrigMonEvent" ]              = {'persistent':"TrigMonEventCollection_tlp1",          'typealias':'', 'collection':'TrigMonEventCollection' }
EDMDetails[ "TrigInDetTrackCollection" ]  = {'persistent':"TrigInDetTrackCollection_tlp2",        'typealias':'InDetTrk' }
#EDMDetails[ "MuonFeature" ]               = {'persistent':"MuonFeatureContainer_tlp1",            'typealias':'Mu', 'collection':'MuonFeatureContainer'}
EDMDetails[ "MuonFeature" ]               = {'persistent':"MuonFeatureContainer_p3",            'typealias':'Mu', 'collection':'MuonFeatureContainer'}
#EDMDetails[ "MuonFeatureDetails" ]        = {'persistent':"MuonFeatureDetailsContainer_tlp1",     'typealias':'MuDetails', 'collection':'MuonFeatureDetailsContainer'}
EDMDetails[ "MuonFeatureDetails" ]        = {'persistent':"MuonFeatureDetailsContainer_p2",       'typealias':'MuDetails', 'collection':'MuonFeatureDetailsContainer'}
#EDMDetails[ "IsoMuonFeature" ]            = {'persistent':"IsoMuonFeatureContainer_tlp1",         'typealias':'IsoMu', 'collection':'IsoMuonFeatureContainer'}
#EDMDetails[ "IsoMuonFeature" ]            = {'persistent':"IsoMuonFeatureContainer_p2",           'typealias':'IsoMu', 'collection':'IsoMuonFeatureContainer'}
EDMDetails[ "IsoMuonFeature" ]            = {'persistent':"IsoMuonFeatureContainer_p3",           'typealias':'IsoMu', 'collection':'IsoMuonFeatureContainer'}
EDMDetails[ "TrigMuonClusterFeature" ]    = {'persistent':"TrigMuonClusterFeatureContainer_tlp1", 'typealias':'','collection':'TrigMuonClusterFeatureContainer'}
#EDMDetails[ "CombinedMuonFeature" ]       = {'persistent':"CombinedMuonFeatureContainer_tlp1",    'typealias':'CombMu', 'collection':'CombinedMuonFeatureContainer'}
#EDMDetails[ "CombinedMuonFeature" ]       = {'persistent':"CombinedMuonFeatureContainer_p2",      'typealias':'CombMu', 'collection':'CombinedMuonFeatureContainer'}
#EDMDetails[ "CombinedMuonFeature" ]       = {'persistent':"CombinedMuonFeatureContainer_p3",      'typealias':'CombMu', 'collection':'CombinedMuonFeatureContainer'}
EDMDetails[ "CombinedMuonFeature" ]       = {'persistent':"CombinedMuonFeatureContainer_p4",      'typealias':'CombMu', 'collection':'CombinedMuonFeatureContainer'}
#EDMDetails[ "TileMuFeature" ]             = {'persistent':"TileMuFeatureContainer_tlp1",          'typealias':'TileMu', 'collection':'TileMuFeatureContainer'}
EDMDetails[ "TileMuFeature" ]             = {'persistent':"TileMuFeatureContainer_p2",            'typealias':'TileMu', 'collection':'TileMuFeatureContainer'}
#EDMDetails[ "TileTrackMuFeature" ]        = {'persistent':"TileTrackMuFeatureContainer_tlp1",     'typealias':'TileTrackMu', 'collection':'TileTrackMuFeatureContainer'}
EDMDetails[ "TileTrackMuFeature" ]        = {'persistent':"TileTrackMuFeatureContainer_p3",       'typealias':'TileTrackMu', 'collection':'TileTrackMuFeatureContainer'}
#EDMDetails[ "TrigEMCluster" ]             = {'persistent':"TrigEMClusterContainer_tlp1",          'typealias':'EMCl', 'collection':"TrigEMClusterContainer"}
#EDMDetails[ "TrigEMCluster" ]             = {'persistent':"TrigEMClusterContainer_tlp2",          'typealias':'EMCl', 'collection':"TrigEMClusterContainer"}
#EDMDetails[ "TrigEMCluster" ]             = {'persistent':"TrigEMClusterContainer_p3",          'typealias':'EMCl', 'collection':"TrigEMClusterContainer"}
EDMDetails[ "TrigEMCluster" ]             = {'persistent':"TrigEMClusterContainer_p4",          'typealias':'EMCl', 'collection':"TrigEMClusterContainer"}
EDMDetails[ "TrigCaloCluster" ]            = {'persistent':"TrigCaloClusterContainer_p3",          'typealias':'CaloCl', 'collection':"TrigCaloClusterContainer"}
#EDMDetails[ "RingerRings" ]               = {'persistent':"RingerRingsContainer_tlp1",            'typealias':'RiRi', 'collection':"RingerRingsContainer"}
EDMDetails[ "RingerRings" ]               = {'persistent':"RingerRingsContainer_p2",            'typealias':'RiRi', 'collection':"RingerRingsContainer"}
#EDMDetails[ "TrigRNNOutput" ]             = {'persistent':"TrigRNNOutputContainer_tlp1",          'typealias':'RNNOut', 'collection':"TrigRNNOutputContainer"}
EDMDetails[ "TrigRNNOutput" ]             = {'persistent':"TrigRNNOutputContainer_p2",            'typealias':'RNNOut', 'collection':"TrigRNNOutputContainer"}
#EDMDetails[ "TrigTauCluster" ]            = {'persistent':"TrigTauClusterContainer_tlp1",         'typealias':'TauCl', 'collection': 'TrigTauClusterContainer' }
#EDMDetails[ "TrigTauCluster" ]            = {'persistent':"TrigTauClusterContainer_p3",         'typealias':'TauCl', 'collection': 'TrigTauClusterContainer' }
EDMDetails[ "TrigTauCluster" ]            = {'persistent':"TrigTauClusterContainer_p5",         'typealias':'TauCl', 'collection': 'TrigTauClusterContainer' }
#EDMDetails[ "TrigTauClusterDetails" ]     = {'persistent':"TrigTauClusterDetailsContainer_tlp1",  'typealias':'TauDetails', 'collection': 'TrigTauClusterDetailsContainer' }
EDMDetails[ "TrigTauClusterDetails" ]     = {'persistent':"TrigTauClusterDetailsContainer_p2",  'typealias':'TauDetails', 'collection': 'TrigTauClusterDetailsContainer' }
#EDMDetails[ "TrigTauTracksInfo" ]         = {'persistent':"TrigTauTracksInfoCollection_tlp1",     'typealias':'TauTrInfo', 'collection':'TrigTauTracksInfoCollection'}
EDMDetails[ "TrigTauTracksInfo" ]         = {'persistent':"TrigTauTracksInfoCollection_p2",     'typealias':'TauTrInfo', 'collection':'TrigTauTracksInfoCollection'}
#EDMDetails[ "TrigT2Jet" ]                 = {'persistent':"TrigT2JetContainer_tlp1",              'typealias':'T2Jet', 'collection':'TrigT2JetContainer'}
EDMDetails[ "TrigT2Jet" ]                 = {'persistent':"TrigT2JetContainer_p3",              'typealias':'T2Jet', 'collection':'TrigT2JetContainer'}
#EDMDetails[ "TrigElectronContainer" ]     = {'persistent':"TrigElectronContainer_tlp2",           'typealias':'' }
EDMDetails[ "TrigElectronContainer" ]     = {'persistent':"TrigElectronContainer_p3",           'typealias':'AOD' }
#EDMDetails[ "TrigPhotonContainer" ]       = {'persistent':"TrigPhotonContainer_tlp2",             'typealias':'' }
EDMDetails[ "TrigPhotonContainer" ]       = {'persistent':"TrigPhotonContainer_p3",             'typealias':'AOD' }
EDMDetails[ "TrigTau" ]                   = {'persistent':"TrigTauContainer_p3",                'typealias':'tau', 'collection':'TrigTauContainer'}
#EDMDetails[ "TrigL2BjetContainer" ]       = {'persistent':"TrigL2BjetContainer_tlp2",             'typealias':'' }
EDMDetails[ "TrigL2BjetContainer" ]       = {'persistent':"TrigL2BjetContainer_p3",             'typealias':'AOD' }
#EDMDetails[ "TrigMissingET" ]             = {'persistent':"TrigMissingETContainer_tlp1",          'typealias':'MET', 'collection':'TrigMissingETContainer'}
EDMDetails[ "TrigMissingET" ]             = {'persistent':"TrigMissingETContainer_p3",          'typealias':'MET', 'collection':'TrigMissingETContainer'}
#EDMDetails[ "TrigT2MbtsBits" ]            = {'persistent':"TrigT2MbtsBitsContainer_tlp1",         'typealias':'', 'collection':'TrigT2MbtsBitsContainer'} 
EDMDetails[ "TrigT2MbtsBits" ]            = {'persistent':"TrigT2MbtsBitsContainer_p3",         'typealias':'AOD', 'collection':'TrigT2MbtsBitsContainer'} 
#EDMDetails[ "TrigSpacePointCounts" ]      = {'persistent':"TrigSpacePointCountsCollection_tlp1",  'typealias':'', 'collection':'TrigSpacePointCountsCollection'}
EDMDetails[ "TrigSpacePointCounts" ]      = {'persistent':"TrigSpacePointCountsCollection_p4",  'typealias':'AOD', 'collection':'TrigSpacePointCountsCollection'}
#EDMDetails[ "TrigTrtHitCounts" ]          = {'persistent':"TrigTrtHitCountsCollection_tlp1",      'typealias':'', 'collection':'TrigTrtHitCountsCollection'}
EDMDetails[ "TrigTrtHitCounts" ]          = {'persistent':"TrigTrtHitCountsCollection_p2",      'typealias':'AOD', 'collection':'TrigTrtHitCountsCollection'}
EDMDetails[ "CosmicMuonCollection" ]      = {'persistent':"CosmicMuonCollection_tlp1",            'typealias':'', 'collection': 'CosmicMuonCollection'}
#EDMDetails[ "MdtTrackSegmentCollection" ] = {'persistent':"MdtTrackSegmentCollection_tlp1",       'typealias':'', 'collection': 'MdtTrackSegmentCollection'}
EDMDetails[ "MdtTrackSegmentCollection" ] = {'persistent':"MdtTrackSegmentCollection_p2",         'typealias':'', 'collection': 'MdtTrackSegmentCollection'}
EDMDetails[ "TrigVertexCollection" ]      = {'persistent':"TrigVertexCollection_tlp1",            'typealias':'AOD'}
EDMDetails[ "TrigL2BphysContainer" ]      = {'persistent':"TrigL2BphysContainer_tlp1",            'typealias':'L2Bphys', 'collection':'TrigL2BphysContainer'}
EDMDetails[ "TrigEFBphysContainer" ]      = {'persistent':"TrigEFBphysContainer_tlp1",            'typealias':'AOD', 'collection':'TrigEFBphysContainer'}
EDMDetails[ "TrigEFBjetContainer" ]       = {'persistent':"TrigEFBjetContainer_tlp2",             'typealias':'AOD' }

#EDMDetails[ "JetCollection" ]             = {'persistent':"JetCollection_tlp2",                   'typealias':'JetColl' }
EDMDetails[ "JetKeyDescriptor" ]          = {'persistent':"JetKeyDescriptorCollection_p1",        'typealias':'AOD', 'collection':'JetKeyDescriptorCollection' }
EDMDetails[ "JetMomentMap" ]              = {'persistent':"JetMomentMapCollection_p6",            'typealias':'AOD', 'collection':'JetMomentMapCollection' }
EDMDetails[ "JetCollection" ]             = {'persistent':"JetCollection_tlp6",                   'typealias':'JetColl' }
#EDMDetails[ "JetCollection" ]             = {'persistent':"JetCollection_tlp5",                   'typealias':'JetColl' }

#EDMDetails[ "CaloClusterCellLinkContainer"] = {'persistent':"CaloClusterCellLinkContainer",       'typealias':''}
EDMDetails[ "CaloClusterContainer" ]      = {'persistent':"CaloClusterContainer_p4",              'typealias':'CaloCl' }
EDMDetails[ "CaloCellLinkContainer" ]     = {'persistent':"CaloCellLinkContainer_p2",             'typealias':'CaloLk' }
EDMDetails[ "CaloShowerContainer" ]       = {'persistent':"CaloShowerContainer_p2",               'typealias':'CaloSh' }
EDMDetails[ "CaloTowerContainer" ]        = {'persistent':"CaloTowerContainer_p1",                'typealias':'' }  
#EDMDetails[ "TrigMuonEFContainer" ]       = {'persistent':"TrigMuonEFContainer_tlp1",             'typealias':'' }
EDMDetails[ "TrigMuonEFContainer" ]       = {'persistent':"TrigMuonEFContainer_p2",              'typealias':'' }
#EDMDetails[ "TrigMuonEFIsolationContainer" ]   = {'persistent':"TrigMuonEFIsolationContainer_p1",'typealias':'' }
EDMDetails[ "TrigMuonEFIsolationContainer" ]   = {'persistent':"TrigMuonEFIsolationContainer_p2",'typealias':'' }
EDMDetails[ "TrigMuonEFInfoContainer" ]   = {'persistent':"TrigMuonEFInfoContainer_tlp2",         'typealias':'AOD' }

EDMDetails[ "egDetailContainer" ]         = {'persistent':"egDetailContainer_p2",                 'typealias':'egDet' }
EDMDetails[ "egammaContainer" ]           = {'persistent':"egammaContainer_p2",                   'typealias':'eg' }
#EDMDetails[ "Analysis::TauJetContainer" ] = {'persistent':"TauJetContainer_p3",                   'typealias':'' }
EDMDetails[ "Analysis::TauJetContainer" ] = {'persistent':"TauJetContainer_p4",                   'typealias':'AOD' }
#EDMDetails[ "Analysis::TauJetContainer" ] = {'persistent':"TauJetContainer_p5",                   'typealias':'' }

EDMDetails[ "TrigTrackCounts" ]           = {'persistent':"TrigTrackCountsCollection_tlp1",       'typealias':'TrkCount', 'collection':'TrigTrackCountsCollection'}
EDMDetails[ "TrigVertexCounts" ]           = {'persistent':"TrigVertexCountsCollection_tlp1",     'typealias':'VtxCount', 'collection':'TrigVertexCountsCollection'}
EDMDetails[ "TrackCollection" ]           = { 'persistent':"Trk::TrackCollection_tlp2",           'typealias':'AOD' }
EDMDetails[ "Rec::TrackParticleContainer" ]   = {'persistent':"Rec::TrackParticleContainer_tlp1", 'typealias':'' }
#EDMDetails[ "Rec::TrackParticleContainer" ]   = {'persistent':"Rec::TrackParticleContainer_tlp3", 'typealias':'' }
EDMDetails[ "TrigT2ZdcSignals" ]        = {'persistent':"TrigT2ZdcSignals_p1",                    'typealias':'AOD', 'collection':'TrigT2ZdcSignalsContainer'}

EDMDetails[ "Analysis::TauDetailsContainer" ] ={'persistent':"TauDetailsContainer_tlp2",          'typealias':'AOD' }
#EDMDetails[ "Analysis::TauDetailsContainer" ] ={'persistent':"TauDetailsContainer_tlp4",          'typealias':'' }
#EDMDetails[ "VxContainer" ]               = {'persistent':"",                                     'typealias':'' }
EDMDetails[ "VxContainer" ]               = {'persistent':"Trk::VxContainer_tlp2",                'typealias':'' }
EDMDetails[ "CaloCellContainer" ]         = {'persistent':"CaloCompactCellContainer" }
EDMDetails[ "TrigPassBits" ]              = {'persistent':"TrigPassBitsCollection_p1",            'typealias':'AOD', 'collection':'TrigPassBitsCollection' }
EDMDetails[ "TrigPassFlags" ]             = {'persistent':"TrigPassFlagsCollection_p1",           'typealias':'', 'collection':'TrigPassFlagsCollection' }
EDMDetails[ "ElectronMuonTopoInfo" ]      = {'persistent':"ElectronMuonTopoInfoContainer_p1",     'typealias':'AOD', 'collection':"ElectronMuonTopoInfoContainer"}
EDMDetails[ "TrigComposite" ]             = {'persistent':"TrigCompositeContainer_p1",            'typealias':'Comp', 'collection':'TrigCompositeContainer'}
EDMDetails["TrigDec::TrigDecision"]               = {'persistent':"",               'typealias':'' }
#EDMDetails["HLT::HLTResult"]                      = {'persistent':"",               'typealias':'' }
EDMDetails["HLT::HLTResult"]                      = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::TriggerTower>"]      = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::JetElement>"]        = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::CPMTower>"]          = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::CPMHits>"]           = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::CPMRoI>"]            = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::CMMCPHits>"]         = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::CMMJetHits>"]        = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::CMMEtSums>"]         = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::JEMHits>"]           = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::JEMRoI>"]            = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::JEMEtSums>"]         = {'persistent':"",               'typealias':'' }
EDMDetails["DataVector<LVL1::RODHeader>"]         = {'persistent':"",               'typealias':'' }
EDMDetails["LVL1::CMMRoI"]                        = {'persistent':"",               'typealias':'' }
EDMDetails["CTP_RDO"]                             = {'persistent':"",               'typealias':'' }
EDMDetails["MuCTPI_RDO"]                          = {'persistent':"",               'typealias':'' }
EDMDetails["ROIB::RoIBResult"]                    = {'persistent':"",               'typealias':'' }
EDMDetails["TrigInDetTrackTruthMap"]              = {'persistent':"",               'typealias':'' }

#xAOD details. 

EDMDetails[ "xAOD::TrigPassBits" ]             = {'persistent':"", 'typealias':'', 'collection':'xAOD::TrigPassBitsContainer'}
EDMDetails[ "xAOD::TrigPassBitsAuxContainer" ] = {'persistent':"", 'typealias':'', 'parent': 'xAOD::TrigPassBitsContainer' }

EDMDetails["xAOD::TrigEMCluster"]             = {'persistent':"", 'typealias':'', 'collection': 'xAOD::TrigEMClusterContainer' } 
EDMDetails["xAOD::TrigEMClusterContainer"]    = {'persistent':"", 'typealias':''} 
EDMDetails["xAOD::TrigEMClusterAuxContainer"] = {'persistent':"", 'typealias':'', 'parent': 'xAOD::TrigEMClusterContainer'} 

EDMDetails["xAOD::TrigRingerRings"]             = {'persistent':"", 'typealias':'Rings', 'collection': 'xAOD::TrigRingerRingsContainer' }
EDMDetails["xAOD::TrigRingerRingsContainer"]    = {'persistent':"", 'typealias':'Rings'}
EDMDetails["xAOD::TrigRingerRingsAuxContainer"] = {'persistent':"", 'typealias':'Rings', 'parent': 'xAOD::TrigRingerRingsContainer'}

EDMDetails["xAOD::TrigRNNOutput"]             = {'persistent':"", 'typealias':'', 'collection': 'xAOD::TrigRNNOutputContainer' }
EDMDetails["xAOD::TrigRNNOutputContainer"]    = {'persistent':"", 'typealias':''}
EDMDetails["xAOD::TrigRNNOutputAuxContainer"] = {'persistent':"", 'typealias':'', 'parent': 'xAOD::TrigRNNOutputContainer'}

EDMDetails["xAOD::CaloClusterContainer" ]      = {'persistent':"",              'typealias':'Rings' }
EDMDetails["xAOD::CaloClusterTrigAuxContainer" ]   = {'persistent':"",              'typealias':'Rings', 'parent': 'xAOD::CaloClusterContainer'}

EDMDetails[ "xAOD::L2StandAloneMuonContainer" ]     = {'persistent':"",         'typealias':'' }
EDMDetails[ "xAOD::L2StandAloneMuonAuxContainer" ]  = {'persistent':"",         'typealias':'', 'parent': 'xAOD::L2StandAloneMuonContainer'}
EDMDetails[ "xAOD::L2CombinedMuonContainer" ]       = {'persistent':"",         'typealias':'' } #fp
EDMDetails[ "xAOD::L2CombinedMuonAuxContainer" ]    = {'persistent':"",         'typealias':'', 'parent': 'xAOD::L2CombinedMuonContainer'} #fp
EDMDetails[ "xAOD::L2IsoMuonContainer" ]            = {'persistent':"",         'typealias':'' } #fp
EDMDetails[ "xAOD::L2IsoMuonAuxContainer" ]         = {'persistent':"",         'typealias':'', 'parent': 'xAOD::L2IsoMuonContainer'} #fp

EDMDetails["xAOD::MuonContainer" ]             = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::MuonAuxContainer" ]          = {'persistent':"",              'typealias':'', 'parent': 'xAOD::MuonContainer'}

EDMDetails["xAOD::TrackParticleContainer" ]    = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TrackParticleAuxContainer" ] = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrackParticleContainer'}

EDMDetails["xAOD::TauJetContainer" ]           = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TauJetAuxContainer" ]        = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TauJetContainer'}

EDMDetails["xAOD::TauTrackContainer" ]           = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TauTrackAuxContainer" ]        = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TauTrackContainer'}

#EDMDetails["xAOD::TrackParticleContainer" ]    = {'persistent':"",              'typealias':'' }
#EDMDetails["xAOD::TrackParticleAuxContainer" ] = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrackParticleContainer'}

EDMDetails["xAOD::VertexContainer" ]           = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::VertexAuxContainer" ]        = {'persistent':"",              'typealias':'', 'parent': 'xAOD::VertexContainer'}

EDMDetails["xAOD::HIEventShapeContainer"]    = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::HIEventShapeAuxContainer"] = {'persistent':"",              'typealias':'', 'parent': 'xAOD::HIEventShapeContainer'}

EDMDetails["xAOD::TrigCompositeContainer"]     = {'persistent':'', 		'typealias':''} 
EDMDetails["xAOD::TrigCompositeAuxContainer"]  = {'persistent':'', 		'typealias':'', 'parent': 'xAOD::TrigCompositeContainer'} 
EDMDetails["xAOD::MuonRoIContainer"]           = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::MuonRoIAuxContainer"]        = {'persistent':"",              'typealias':'', 'parent': 'xAOD::MuonRoIContainer'}
EDMDetails["xAOD::EmTauRoIContainer"]          = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::EmTauRoIAuxContainer"]       = {'persistent':"",              'typealias':'', 'parent': 'xAOD::EmTauRoIContainer'}
EDMDetails["xAOD::JetRoIContainer"]            = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::JetRoIAuxContainer"]         = {'persistent':"",              'typealias':'', 'parent': 'xAOD::JetRoIContainer'}

EDMDetails["xAOD::JetEtRoI"]                   = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::JetEtRoIAuxInfo"]            = {'persistent':"",              'typealias':'', 'parent': 'xAOD::JetEtRoI'}
EDMDetails["xAOD::EnergySumRoI"]               = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::EnergySumRoIAuxInfo"]        = {'persistent':"",              'typealias':'', 'parent': 'xAOD::EnergySumRoI'}

EDMDetails['xAOD::L1TopoRawDataContainer']     = {'persistent':"",              'typealias':'' }
EDMDetails['xAOD::L1TopoRawDataAuxContainer']  = {'persistent':"",              'typealias':'', 'parent': 'xAOD::L1TopoRawDataContainer'}

EDMDetails["xAOD::ElectronContainer"]          = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::ElectronTrigAuxContainer"]   = {'persistent':"",              'typealias':'', 'parent': 'xAOD::ElectronContainer'}
EDMDetails["xAOD::PhotonContainer"]            = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::PhotonTrigAuxContainer"]     = {'persistent':"",              'typealias':'', 'parent': 'xAOD::PhotonContainer'}

EDMDetails["xAOD::TrigBphysContainer" ]        = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TrigBphysAuxContainer" ]     = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrigBphysContainer'}

EDMDetails["xAOD::TrigT2MbtsBits" ]                       = {'persistent':"",   'typealias':'', 'collection':"xAOD::TrigT2MbtsBitsContainer"}
EDMDetails["xAOD::TrigT2MbtsBitsAuxContainer" ]           = {'persistent':"",   'typealias':'', 'parent': 'xAOD::TrigT2MbtsBitsContainer'}
EDMDetails["xAOD::TrigSpacePointCounts" ]                 = {'persistent':"",   'typealias':'', 'collection':"xAOD::TrigSpacePointCountsContainer" }
EDMDetails["xAOD::TrigSpacePointCountsAuxContainer" ]     = {'persistent':"",   'typealias':'', 'parent': 'xAOD::TrigSpacePointCountsContainer'}
EDMDetails["xAOD::TrigVertexCounts" ]                     = {'persistent':"",   'typealias':'', 'collection':"xAOD::TrigVertexCountsContainer" }
EDMDetails["xAOD::TrigVertexCountsAuxContainer" ]         = {'persistent':"",   'typealias':'', 'parent': 'xAOD::TrigVertexCountsContainer'}
EDMDetails["xAOD::TrigTrackCounts" ]                      = {'persistent':"",   'typealias':'', 'collection':"xAOD::TrigTrackCountsContainer" }
EDMDetails["xAOD::TrigTrackCountsAuxContainer" ]          = {'persistent':"",   'typealias':'', 'parent': 'xAOD::TrigTrackCountsContainer'}
EDMDetails["xAOD::TrigT2ZdcSignals" ]                     = {'persistent':"",   'typealias':'', 'collection':"xAOD::TrigT2ZdcSignalsContainer"}
EDMDetails["xAOD::TrigT2ZdcSignalsAuxContainer" ]         = {'persistent':"",   'typealias':'', 'parent': 'xAOD::TrigT2ZdcSignalsContainer'}

EDMDetails["xAOD::TrigMissingET"]              = {'persistent':"",              'typealias':'', 'collection':"xAOD::TrigMissingETContainer"}
EDMDetails["xAOD::TrigMissingETContainer"]     = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TrigMissingETAuxContainer"]  = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrigMissingETContainer'}

EDMDetails["xAOD::TrigPhotonContainer"]       = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TrigPhotonAuxContainer"]    = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrigPhotonContainer'}
EDMDetails["xAOD::TrigElectronContainer"]     = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TrigElectronAuxContainer"]  = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrigElectronContainer'}


EDMDetails["xAOD::JetContainer"]               = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::JetTrigAuxContainer"]        = {'persistent':"",              'typealias':'', 'parent': 'xAOD::JetContainer'}

EDMDetails["xAOD::TrigDecision"]               = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::TrigDecisionAuxInfo"]        = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrigDecision'}
EDMDetails["xAOD::TrigConfKeys"]               = {'persistent':"",              'typealias':'' }

EDMDetails["xAOD::TrigNavigation"]          = {'persistent':"",              'typealias':'' } 
EDMDetails["xAOD::TrigNavigationAuxInfo"]   = {'persistent':"",              'typealias':'', 'parent': 'xAOD::TrigNavigation'}

EDMDetails["xAOD::BTaggingContainer"]          = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::BTaggingTrigAuxContainer"]   = {'persistent':"",              'typealias':'', 'parent': 'xAOD::BTaggingContainer'}
EDMDetails["xAOD::BTagVertexContainer"]        = {'persistent':"",              'typealias':'' }
EDMDetails["xAOD::BTagVertexAuxContainer"]     = {'persistent':"",              'typealias':'', 'parent': 'xAOD::BTagVertexContainer'}

# objects from xAODTrigL1CaloEvent package
# =============================================================================
EDMDetails["xAOD::TriggerTowerContainer"]                 = {'persistent':"", 'typealias':'' }                                
EDMDetails["xAOD::TriggerTowerAuxContainer"]              = {'persistent':"", 'typealias':'', 'parent': 'xAOD::TriggerTowerContainer'}

EDMDetails["xAOD::CPMTowerContainer"]                     = {'persistent':"", 'typealias':''}                                                            
EDMDetails["xAOD::CPMTowerAuxContainer"]                  = {'persistent':"", 'typealias':'', 'parent': 'xAOD::CPMTowerContainer'}

EDMDetails['xAOD::CMXCPTobContainer']                     = {'persistent': "", 'typealias':'' }                          
EDMDetails['xAOD::CMXCPTobAuxContainer']                  = {'persistent': "", 'typealias':'', 'parent': 'xAOD::CMXCPTobContainer' }    

EDMDetails['xAOD::CMXCPHitsContainer']                    = {'persistent': "",  'typealias':'' }                        
EDMDetails['xAOD::CMXCPHitsAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::CMXCPHitsContainer'  }  

EDMDetails['xAOD::CMXJetTobContainer']                    = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::CMXJetTobAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::CMXJetTobContainer'  }  

EDMDetails['xAOD::CMXJetHitsContainer']                   = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::CMXJetHitsAuxContainer']                = {'persistent': "", 'typealias':'', 'parent': 'xAOD::CMXJetHitsContainer'  }  

EDMDetails['xAOD::CMXEtSumsContainer']                    = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::CMXEtSumsAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::CMXEtSumsContainer'  }  

EDMDetails['xAOD::JEMEtSumsContainer']                    = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::JEMEtSumsAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::JEMEtSumsContainer'  }  

EDMDetails['xAOD::CPMTobRoIContainer']                    = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::CPMTobRoIAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::CPMTobRoIContainer'  }  

EDMDetails['xAOD::JEMTobRoIContainer']                    = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::JEMTobRoIAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::JEMTobRoIContainer'  }  

EDMDetails['xAOD::JEMTobRoIAuxContainer']                 = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::JEMTobRoIAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::JEMTobRoIAuxContainer'  }  

EDMDetails['xAOD::JetElementContainer']                   = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::JetElementAuxContainer']                = {'persistent': "", 'typealias':'', 'parent': 'xAOD::JetElementContainer'  }  

EDMDetails['xAOD::RODHeaderContainer']                    = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::RODHeaderAuxContainer']                 = {'persistent': "", 'typealias':'', 'parent': 'xAOD::RODHeaderContainer' }   

EDMDetails['xAOD::CMXRoIContainer']                       = {'persistent': "", 'typealias':'' }                                
EDMDetails['xAOD::CMXRoIAuxContainer']                    = {'persistent': "", 'typealias':'', 'parent': 'xAOD::CMXRoIContainer' }  
# =============================================================================

#
# Rustem Ospanov: move list of EDM libraries from HLTTriggerGetter.py to TriggerEDM.py
#
EDMLibraries = [ 'TrigSteeringEvent', 'TrigMuonEvent',
                 'TrigBphysicsEvent', 'TrigCaloEvent',
                 'TrigInDetEvent',
                 'TrigParticle',      'TrigMissingEtEvent',
                 'TrigDecisionEvent', 'TrigMonitoringEvent',
		 'TrigTopoEvent' , 'TrigCombinedEvent', 
	         'TrigMonitoringEventTPCnv',   'RecTPCnv',
                 'TrigCaloEventTPCnv',
                 'TrigDecisionEventTPCnv',     'TrigInDetEventTPCnv',
                 'TrigMissingEtEventTPCnv',    'TrigMuonEventTPCnv',
                 'TrigParticleTPCnv',          'TrigSteeringEventTPCnv',
		 'TrigTopoEventTPCnv',         'TrigCombinedEventTPCnv', 
		 'tauEventTPCnvDict',          'RecTPCnvDict',
		 'TrigMonitoringEventTPCnvDict',
		 'OLD_RecTPCnvDict',
                 'TrigCosmicEvent',
#
		 #'TrigCombinedEventTPCnv1Dict',     'TrigCaloEventTPCnv1Dict',          
                 #'TrigDecisionEventTPCnv1Dict',     'TrigInDetEventTPCnv1Dict',
                 #'TrigMissingEtEventTPCnv1Dict',    'TrigMuonEventTPCnv1Dict',
                 #'TrigParticleTPCnv1Dict',          'TrigSteeringEventTPCnv1Dict',
                 #'OLD_TrigDecisionEventTPCnv1Dict', 'OLD_TrigMuonEventTPCnv1Dict',
                 #'OLD_TrigParticleTPCnv1Dict',      'OLD_TrigSteeringEventTPCnv1Dict',
#
                 'xAODCaloEvent', 'xAODEgamma', 'xAODTracking', 'xAODMuon', 'xAODTau', 'xAODJet',
                 'xAODTrigBphys', 'xAODTrigMissingET', 'xAODTrigEgamma', 'xAODTrigMuon', 'xAODTrigCalo',
                 'xAODTrigMinBias', 'xAODBTagging', 'xAODTrigRinger', 
		 'xAODHIEvent',
                 'xAODCaloEventDict', 'xAODEgammaDict', 'xAODTrackingDict', 'xAODMuonDict', 'xAODTauDict', 'xAODJetDict',
                 'xAODTrigBphysDict', 'xAODTrigMissingETDict', 'xAODTrigEgammaDict', 'xAODTrigMuonDict', 'xAODTrigCaloDict',
                 'xAODTrigMinBiasDict', 'xAODBTaggingDict',
		 'xAODHIEventDict'
	         #'ARA_TrigMonitoringEventTPCnvDict',
                 #'ARA_TrigCaloEventTPCnv1Dict',
                 #'ARA_TrigDecisionEventTPCnv1Dict',  #'ARA_TrigInDetEventTPCnv1Dict',
                 #'ARA_TrigMissingEtEventTPCnv1Dict', 'ARA_TrigMuonEventTPCnv1Dict',
                 #'ARA_TrigParticleTPCnv1Dict',
                 #'ARA_TrigSteeringEventTPCnv1Dict',
		 #'ARA_RecTPCnvDict', 'ARA_tauEventTPCnvDict'
         ]

def getCategory(s):
    """ From name of object in AOD/ESD found by checkFileTrigSize.py, return category """ 
    
    """ Clean up object name """
    s = s.strip()
                 
    # To-do
    # seperate the first part of the string at the fisrt '_'
    # search in EDMDetails for the key corresponding to the persistent value
    # if a key is found, use this as the first part of the original string
    # put the string back together
                 
    if s.startswith('HLT_xAOD__') or s.startswith('HLT_Rec__') or s.startswith('HLT_Analysis__') :
        s = s[s.index('__')+2:]
        s = s[s.index('_')+1:]
        if s.count('Dyn') : s = s[:s.index('Dyn')]
        if s.count('.') : s = s[:s.index('.')]
        s = "HLT_"+s
    elif s.startswith('HLT_'):
        if s.count('_'): s = s[s.index('_')+1:]
        if s.count('_'): s = s[s.index('_')+1:]
        s = "HLT_"+s
    if s.count('.') : s = s[:s.index('.')] 
        
                 
    if s.count('::'): s = s[s.index(':')+2:]
    if s.count('<'):  s = s[s.index('<')+1:]
    if s.count('>'):  s = s[:s.index('>')]

        # need to make unique list?
    TriggerList = TriggerL2List + TriggerEFList + TriggerResultsList + TriggerResultsRun1List + TriggerLvl1List + TriggerIDTruth + TriggerHLTList

    category = '' 
    bestMatch = ''

    """ Loop over all objects already defined in lists (and hopefully categorized!!) """
    for item in TriggerList:
        t,k = getTypeAndKey(item[0])
    
        """ Clean up type name """
        if t.count('::'): t = t[t.index(':')+2:]
        if t.count('<'):  t = t[t.index('<')+1:]
        if t.count('>'):  t = t[:t.index('>')]
            
        if (s.startswith(t) and s.endswith(k)) and (len(t) > len(bestMatch)):
            bestMatch = t
            category = item[2]

        if k.count('.'): k = k[:k.index('.')]  
        if (s == k):
            bestMatch = k
            category = item[2]
                 
    if category == '': return 'NOTFOUND'
    return category



def getTypeAndKey(s):
    """ From the strings containing type and key of trigger EDM extract type and key
    """
    return s[:s.index('#')], s[s.index('#')+1:]

def keyToLabel(key):
    """ The key is usually HLT_blablah, this function returns second part of it or empty string
    """
    if '_' not in key:
        return ''
    else:
        return key[key.index('_'):].lstrip('_')

def getTriggerObjList(destination, lst):
    """
    Gives back the Python dictionary  with the content of ESD/AOD (dst) which can be inserted in OKS.
    """
    dset = set(destination.split())
    
    toadd = {}
    import itertools

    for item in itertools.chain(*lst):
        if item[1] == '':
            continue
        confset = set(item[1].split())
        if dset & confset: # intersection of the sets
            t,k = getTypeAndKey(item[0])
            colltype = t
            if EDMDetails[t].has_key('collection'):
                colltype = EDMDetails[t]['collection']
            if toadd.has_key(colltype):
                if k not in toadd[colltype]:
                    toadd[colltype] += [k]
            else:
                    toadd[colltype] = [k]
    return InsertContainerNameForHLT(toadd)



def getTrigIDTruthList(dst):
    """
    Gives back the Python dictionary  with the truth trigger content of ESD/AOD (dst) which can be inserted in OKS.
    """
    return getTriggerObjList(dst,[TriggerIDTruth])
    
def getLvl1ESDList():
    """
    Gives back the Python dictionary  with the lvl1 trigger result content of ESD which can be inserted in OKS.
    """
    return getTriggerObjList('ESD',[TriggerLvl1List])
 
def getLvl1AODList():
    """
    Gives back the Python dictionary  with the lvl1 trigger result content of AOD which can be inserted in OKS.
    """
    return getTriggerObjList('AODFULL',[TriggerLvl1List])
 

def getTriggerEDMList(key, runVersion):
    """
    List (Literally Python dict) of trigger objects to be placed with flags:
    key can be" 'ESD', 'AODFULL', 'DS'
    run can be: '1 (Run1)', '2 (Run2)'
    """
    if runVersion == 2:
        return getTriggerObjList(key,[TriggerHLTList, TriggerResultsList])
    else:
        return getTriggerObjList(key,[TriggerL2List,TriggerEFList, TriggerResultsRun1List])
        
####################################################
    # These are useless, but still used in some places: they can replaced by the above:
    #see: atlas/Reconstruction/RecJobTransforms/share/skeleton.RDOtoRDOtrigger.py
    #     atlas/PhysicsAnalysis/D3PDMaker/TriggerD3PDMaker/python/TrigNavSlimming.py
## def getESDList(key='ESD'):
##     """
##     List (Literally Python dict) of trigger objects to be placed in ESD
##     """
##     return getTriggerObjList(key,[TriggerL2List,TriggerEFList,TriggerHLTList, TriggerResultsList])

## def getAODList(tag='AODFULL'):
##     """
##     List (Literally Python dict) of trigger objects to be placed in AOD
##     """
##     return getTriggerObjList(tag, [TriggerL2List, TriggerEFList, TriggerHLTList, TriggerResultsList])

## def getDSList(tag='DS'):
##     """
##     List (Literally Python dict) of trigger objects to be placed in DS
##     """
##     return getTriggerObjList(tag, [TriggerL2List,TriggerEFList,TriggerHLTList, TriggerResultsList])

####################################################
    
def getL2PreregistrationList():
    """
    List (Literally Python list) of trigger objects to be preregistered i.e. this objects we want in every event for L2
    """
    l = []
    for item in TriggerL2List:
        if len (item[1]) == 0: continue
        t,k = getTypeAndKey(item[0])
        if('Aux' in t):
            continue #we don't wat to preregister Aux containers
        l += [t+"#"+keyToLabel(k)]
    return l
    
def getEFPreregistrationList():
    """
    List (Literally Python list) of trigger objects to be preregistered i.e. this objects we want in every event for EF
    """
    l = []
    for item in TriggerEFList:
        if len (item[1]) == 0: continue
        t,k = getTypeAndKey(item[0])
        if('Aux' in t):
            continue #we don't wat to preregister Aux containers
        l += [t+"#"+keyToLabel(k)]
    return l

def getHLTPreregistrationList():
    """
    List (Literally Python list) of trigger objects to be preregistered i.e. this objects we want in every event for merged L2/EF in addition to default L2 and EF
    """
    l = []
    for item in TriggerHLTList:
        if len (item[1]) == 0: continue
        t,k = getTypeAndKey(item[0])
        if('Aux' in t):
            continue #we don't wat to preregister Aux containers
        l += [t+"#"+keyToLabel(k)]
    return l


def getPreregistrationList(version=2):
    """
    List (Literally Python list) of trigger objects to be preregistered i.e. this objects we want for all levels
    version can be: '1 (Run1)', '2 (Run2)'    
    """
    
    l=[]
    if version==2:
        l = getHLTPreregistrationList()
    else:
        l=list(set(getL2PreregistrationList()+getEFPreregistrationList()+getHLTPreregistrationList()))   
    return l



def getEFDSList():
    """
    List (Literally Python list) of trigger objects to be placed in RAW data. i.e. BS after EF
    """
    l = []
    for item in TriggerEFList:
        if 'DS' in item[1].split():
            t,k = getTypeAndKey(item[0])
            l += [t+"#"+keyToLabel(k)]   
    return l

def getHLTDSList():
    """
    List (Literally Python list) of trigger objects to be placed in RAW data. i.e. BS after merged L2EF
    """
    l = []
    for item in TriggerHLTList:
        if 'DS' in item[1].split():
            t,k = getTypeAndKey(item[0])
            l += [t+"#"+keyToLabel(k)]   
    return l

def getL2BSList():
    """
    List (Literally Python list) of L2 trigger objects to be placed in output BS
    """
    l = []
    for item in TriggerL2List:
        if 'BS' in item[1]:
            t,k = getTypeAndKey(item[0])
            l += [t+"#"+keyToLabel(k)]   
    return l

def getEFBSList():
    """
    List (Literally Python list) of EF trigger objects to be placed in output BS
    """
    l = []
    for item in TriggerEFList:
        if 'BS' in item[1]:
            t,k = getTypeAndKey(item[0])
            l += [t+"#"+keyToLabel(k)]   
    return l

def getHLTBSList():
    """
    List (Literally Python list) of merged HLT trigger objects to be placed in output BS
    """
    l = []
    for item in TriggerHLTList:
        if 'BS' in item[1]:
            t,k = getTypeAndKey(item[0])
            l += [t+"#"+keyToLabel(k)]   
    return l

def getL2BSTypeList():
    """ List of L2 types to be read from BS, used by the TP
    """
    l = []
    for item in TriggerL2List:
        t,k = getTypeAndKey(item[0])
        ctype = t
        if EDMDetails[t].has_key('collection'):
            ctype = EDMDetails[t]['collection']
        l += [ctype]
    return l

def getEFBSTypeList():
    """ List of EF types to be read from BS, used by the TP 
    """
    l = []
    for item in TriggerEFList:
        t,k = getTypeAndKey(item[0])
        ctype = t
        if EDMDetails[t].has_key('collection'):
            ctype = EDMDetails[t]['collection']
        l += [ctype]
    return l

def getHLTBSTypeList():
    """ List of HLT types to be read from BS, used by the TP 
    """
    l = []
    for item in TriggerHLTList:
        t,k = getTypeAndKey(item[0])
        ctype = t
        if EDMDetails[t].has_key('collection'):
            ctype = EDMDetails[t]['collection']
        l += [ctype]
    return l

def getEFDSTypeList():
    """ List of types to be placed in BS after EF
    """
    l = []
    for item in TriggerEFList:
        if 'DS' in item[1].split():
            t,k = getTypeAndKey(item[0])
            ctype = t
            if EDMDetails[t].has_key('collection'):
                ctype = EDMDetails[t]['collection']
            l += [ctype]
    return l

def getHLTDSTypeList():
    """ List of types to be placed in BS after L2EF
    """
    l = []
    for item in TriggerHLTList:
        if 'DS' in item[1].split():
            t,k = getTypeAndKey(item[0])
            ctype = t
            if EDMDetails[t].has_key('collection'):
                ctype = EDMDetails[t]['collection']
            l += [ctype]
    return l


def getTPList(version=2):
    """
    Mapping  of Transient objects to Peristent during serialization (BS creation)
    version can be: '1 (Run1)', '2 (Run2)'
    """
    l = {}
    if version==2:
        bslist = getHLTBSTypeList()
    else:
        bslist = list(set(getL2BSTypeList() + getEFBSTypeList()))
        
    for t,d in EDMDetails.iteritems():                
        colltype = t
        if d.has_key('collection'):
            colltype = EDMDetails[t]['collection']
        if colltype in bslist: 
            l[colltype] = d['persistent']
    return l

## def getTPList():
##     """
##     Mapping  of Transient objects to Peristent during serialization (BS creation)
##     """
##     l = {}
##     bslist = getL2BSTypeList() + getEFBSTypeList()+getHLTBSTypeList()
##     for t,d in EDMDetails.iteritems():                
##         colltype = t
##         if d.has_key('collection'):
##             colltype = EDMDetails[t]['collection']
##         if colltype in bslist: 
##             l[colltype] = d['persistent']
##     return l

#FPP: how to change this for the merged HLT in view of splitting?
def getARATypesRenaming():
    """
    Defines how to rename collecion keys in ARA when two types have the same key.
    i.e. TrigRoiDescriptorCollection#HLT
    and TrigTau#HLT
    After the remapping they will be named HLT_tau and HLT_roi so are distinct.
    """
    edm = set(TriggerL2List + TriggerEFList + TriggerHLTList)
    keys = [ getTypeAndKey(i[0])[1] for i in edm]
    # find repeating keys
    tmp = [ i for i in keys if keys.count(i) > 1 ]
    nonunique = {}
    for i in tmp:
        nonunique[i] = 1
    # nonunique = nonunique.keys()

    # define remens for all object of which key appeared in nonunique
    renames = {}
    for entry in edm:
        t, key = getTypeAndKey(entry[0])
        if nonunique.has_key(key): # potential problem we have to do something
            
            if not EDMDetails[t].has_key('typealias') or EDMDetails[t]['typealias'] == '':
                if nonunique[key] == 1:
                    # First time's ok.
                    nonunique[key] = t
                elif nonunique[key] == t:
                    # Duplicate entry; ok.
                    continue
                else:
                    print "ERROR types/keys will catch ", t, " ", key
                continue
            else:
                obj = t                    
                if EDMDetails[t].has_key('collection'):
                    obj = EDMDetails[t]['collection']

                # form the branch name
                bname = key+'_'+EDMDetails[t]['typealias']
                        
                #print 'type/key : ',key, '/', obj, ' to be aliased to: ', bname
                renames[(key, obj)] = bname
            
    return renames

def getEDMLibraries():
    return EDMLibraries

def InsertContainerNameForHLT(typedict):
    import re    
    output = {}
    for k,v in typedict.iteritems():
        newnames = []
        for el in v:
            if el.startswith('HLT_') or el == 'HLT':
                prefixAndLabel = el.split('_',1) #only split on first underscore
                containername = k if not 'Aux' in k else EDMDetails[k]['parent'] #we want the type in the Aux SG key to be the parent type #104811
                #maybe this is not needed anymore since we are now versionless with the CLIDs but it's not hurting either
                containername = re.sub('::','__',re.sub('_v[0-9]+$','',containername))
                newnames+=['_'.join([prefixAndLabel[0],containername]+([prefixAndLabel[1]] if len(prefixAndLabel) > 1 else []))]
            else:
                newnames+=[el]
            output[k] = newnames
    return output
