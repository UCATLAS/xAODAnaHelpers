import ROOT

def config_BasicEventSelection(Job, **kw):
    basicEventSel = ROOT.BasicEventSelection()
    basicEventSel.m_debug                 = kw.get('debug'      ,  False)
    basicEventSel.m_applyGRLCut           = kw.get('applyGRLCut',  False)
    basicEventSel.m_name                  = kw.get('name',         "baseEventSel")
    basicEventSel.m_GRLxml                = kw.get('GRLxml', "$ROOTCOREBIN/data/xAODAnaHelpers/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml")
    basicEventSel.m_doPUreweighting       = kw.get('doPUreweighting',      False)
    basicEventSel.m_vertexContainerName   = kw.get('vertexContainerName',  "PrimaryVertices")
    basicEventSel.m_PVNTrack              = kw.get('PVNTrack',             2)
    basicEventSel.m_truthLevelOnly        = kw.get('truthLevelOnly',       False)
    basicEventSel.m_applyPrimaryVertexCut = kw.get('applyPrimaryVertexCut',     True)
    basicEventSel.m_derivationName        = kw.get('derivationName',   "")
    basicEventSel.m_lumiCalcFileNames     = kw.get('lumiCalcFileNames', "$ROOTCOREBIN/data/xAODAnaHelpers/ilumicalc_histograms_None_266904-267639_periodA+B1_DetStatus-v62-pro18_DQDefects-00-01-02_PHYS_StandardGRL_All_Good.root")
    basicEventSel.m_PRWFileNames          = kw.get('PRWFileNames',"$ROOTCOREBIN/data/xAODAnaHelpers/JZ_QStar_PRW.root")
    basicEventSel.m_triggerSelection      = kw.get('triggerSelection', "")
    basicEventSel.m_storeTrigDecisions    = kw.get('storeTrigDecisions',True)
    basicEventSel.m_useMetaData           = kw.get('useMetaData',False)
    basicEventSel.m_applyEventCleaningCut = kw.get('applyEventCleaningCut',True)
    basicEventSel.m_applyTriggerCut       = kw.get('applyTriggerCut', False)
    Job.algsAdd(basicEventSel)

def config_JetCalibrator(Job, name, **kw):
    jetCalib = ROOT.JetCalibrator()
    jetCalib.m_name                      = name
    jetCalib.m_systName                  = kw.get('systName'           ,      "")
    jetCalib.m_systVal                   = kw.get('systVal'            ,      1)
    jetCalib.m_inContainerName           = kw.get('inContainerName'    ,      "")
    jetCalib.m_outContainerName          = kw.get('outContainerName'   ,      "")
    jetCalib.m_sort                      = kw.get('sort'               ,      True)
    jetCalib.m_jetAlgo                   = kw.get('jetAlgo'            ,      "")
    jetCalib.m_outputAlgoSystNames       = kw.get('outputAlgoSystNames',      "")
    jetCalib.m_calibSequence             = kw.get('calibSequence'      ,      "")
    jetCalib.m_calibConfigFullSim        = kw.get('calibConfigFullSim' ,      "")
    jetCalib.m_calibConfigData           = kw.get('calibConfigData'    ,      "")
    jetCalib.m_calibConfigAFII           = kw.get('calibConfigAFII'    ,      "")
    jetCalib.m_jetCleanCutLevel          = kw.get('jetCleanCutLevel'   ,      "LooseBad")
    jetCalib.m_JESUncertConfig           = kw.get('JESUncertConfig'    ,      "$ROOTCOREBIN/data/JetUncertainties/JES_2015/Prerec/PrerecJES2015_3NP_Scenario1_50ns.config")
    jetCalib.m_JESUncertMCType           = kw.get('JESUncertMCType'    ,      "MC15")
    jetCalib.m_verbose                   = kw.get('verbose'            ,      False)
    jetCalib.m_debug                     = kw.get('debug'              ,      False)
    jetCalib.m_redoJVT                   = kw.get('redoJVT'            ,      False)
    jetCalib.m_jetCleanUgly              = kw.get('jetCleanUgly'       ,      False)
    jetCalib.m_cleanParent               = kw.get('cleanParent'        ,      False)
    Job.algsAdd(jetCalib)

def config_muonCalib(Job,name, **kw):
    muonCalib = ROOT.MuonCalibrator()
    muonCalib.m_name                 = name
    muonCalib.m_inContainerName      = kw.get('inContainerName'    , "")
    muonCalib.m_outContainerName     = kw.get('outContainerName'   , "")
    muonCalib.m_outputAlgoSystNames  = kw.get('outputAlgoSystNames', "")
    Job.algsAdd(muonCalib)


def config_JetHistsAlgo(Job, name, **kw):
    jetHists_alg = ROOT.JetHistsAlgo()
    jetHists_alg.m_name            = name
    jetHists_alg.m_inContainerName = kw.get('inContainerName', "")
    jetHists_alg.m_detailStr       = kw.get('detailStr', "kinematic clean energy flavorTag")
    Job.algsAdd(jetHists_alg)


def config_JetSelector(Job, name, **kw):
    jetSelector = ROOT.JetSelector()
    jetSelector.m_name                     = name
    jetSelector.m_inContainerName          = kw.get('inContainerName'           ,    "")
    jetSelector.m_decorateSelectedObjects  = kw.get('decorateSelectedObjects'   ,    False)
    jetSelector.m_createSelectedContainer  = kw.get('createSelectedContainer'   ,    True)
    jetSelector.m_outContainerName         = kw.get('outContainerName'          ,    "")
    jetSelector.m_pT_min                   = kw.get('pT_min'                    ,    0)
    jetSelector.m_eta_min                  = kw.get('eta_min'                   ,    -10)
    jetSelector.m_eta_max                  = kw.get('eta_max'                   ,    10)
    jetSelector.m_debug                    = kw.get('debug'                     ,    False)
    jetSelector.m_cleanJets                = kw.get('cleanJets'                 ,    True)
    jetSelector.m_nToProcess               = kw.get('nToProcess'                ,    -1)
    jetSelector.m_inputAlgo                = kw.get('inputAlgo'                 ,    "")
    jetSelector.m_doBTagCut                = kw.get('doBTagCut'                 ,    False)
    jetSelector.m_taggerName               = kw.get('taggerName'                ,    "")
    jetSelector.m_operatingPt              = kw.get('operatingPt'               ,    "")
    Job.algsAdd(jetSelector)

