from Campaigns.Utils import Campaign
from AthenaConfiguration.Enums import LHCPeriod
from AnalysisAlgorithmsConfig.ConfigAccumulator import DataType

_campaigns_AMITag = {
    # NOTE this is a "fallback" approach to read campaign based on standard r-tag with pile-up for
    # MC campaigns. For non-standard reconstruction, extra r-tags have to be added here to be recognized.
    # Recommended approach is to read mc_campaign from FileMetaData (seems to require reasonably recent p-tags)
    Campaign.MC20a: ['r13167', 'r14859'],
    Campaign.MC20d: ['r13144', 'r14860'],
    Campaign.MC20e: ['r13145', 'r14861'],
    Campaign.MC21a: ['r13829'],
    Campaign.MC23a: ['r14622'],
    Campaign.MC23c: ['r14799'],
}

_years_runNumbers = {
    # for each year (dict key), provide list of [min runNumber, max runNumber)
    # i.e. we check min runNumber <= runNumber < max runNumber 
    2015: [0, 290000],
    2016: [290000, 320000],
    2017: [320000, 342000],
    2018: [342000, 400000],
    2022: [410000, 450000],
    2023: [450000, 999999]
}

_campaigns_R2 = [
    Campaign.MC20a, Campaign.MC20d, Campaign.MC20e
]

_campaigns_R3 = [
    Campaign.MC21a, Campaign.MC23a, Campaign.MC23c,
]


def populate_config_flags(flags, metadata):
    """
    Populate additional information in the AllConfigFlags from FileMetaData
    """
    flags.addFlag('Input.AMITag', metadata.get('AMITag', ''))
    if len(flags.Input.RunNumber) != 1:
        print('WARNING (metaConfig.populate_config_flags): FileMetaData reports RunNumber list '
              f'with not exactly 1 entry: {flags.Input.RunNumber}')
    flags.addFlag('Input.RunNumberAsInt', int(flags.Input.RunNumber[0]))
    flags.addFlag('Input.DataType', get_data_type)
    is_data = (flags.Input.DataType is DataType.Data)
    if not is_data:
        # try a fallback solution to determine MC campaign
        # this is for samples, that don't include the MCCampaign entry in FileMetaData
        # this problem should be fixed in p58XX tags
        if flags.Input.MCCampaign == Campaign.Unknown:
            flags.Input.MCCampaign = get_campaign_fallback
    flags.addFlag('Input.LHCPeriod', get_LHCgeometry)
    flags.addFlag('Input.isRun3', isRun3)
    flags.addFlag('Input.isPHYSLITE', isPhysLite)


def get_data_type(flags):
    if not flags.Input.isMC:
        return DataType.Data
    else:
        if flags.Sim.ISF.Simulator.isFullSim():
            return DataType.FullSim
        elif flags.Sim.ISF.Simulator.usesFastCaloSim():
            return DataType.FastSim
        else:
            raise Exception('Could not determine data_type, '
                            'perhaps SimulationFlavour metadata is missing')


def get_campaign_fallback(flags):
    """
    In case MC Campaign is not stored in FileMetaData, we can try to figure it out from AMI tag.
    """
    amiTags = flags.Input.AMITag
    if amiTags == '':
        print('WARNING (metaConfig.get_campaign): AMITag entry in FileMetaData '
                'appears to be empty or does not exist')

    for (cmp, tagsList) in _campaigns_AMITag.items():
        for tag in tagsList:
            if tag in amiTags:
                return cmp
    raise Exception(f'AMITag {amiTags} in FileMetaData does not correspond to any implemented campaign')


def get_data_year(flags):
    """
    Try to determine the year of data-taking based on runNumber.
    """
    if flags.Input.RunNumberAsInt == 0:
        print('ERROR (metaConfig.get_data_year): runNumber == 0, we cannot determine data year reliably.')
    for (year, runRange) in _years_runNumbers.items():
        if flags.Input.RunNumberAsInt >= runRange[0] and flags.Input.RunNumberAsInt < runRange[1]:
            return year
    print(f'WARNING (metaConfig.get_data_year): runNumber {flags.Input.RunNumberAsInt} '
          'does not correspond to any of the defined years of data taking!')
    return 0

def isPhysLite(flags):
    """
    Check whether the derivation format is PHYSLITE.
    """
    if flags.Input.ProcessingTags is not None:
        return 'StreamDAOD_PHYSLITE' in flags.Input.ProcessingTags
    else:
        print('WARNING Could not find any information about the sample being PHYSLITE '
              'in the metadata. Will assume that it was regular PHYS.')
    return False

def isRun3(flags):
    if flags.Input.DataType is DataType.Data:
        year = get_data_year(flags)
        return (year >= 2022)
    else:
        cmp = flags.Input.MCCampaign
        return (cmp in _campaigns_R3)


def get_LHCgeometry(flags):
    if isRun3(flags):
        return LHCPeriod.Run3
    else:
        return LHCPeriod.Run2


def get_grl(flags):
    year = get_data_year(flags)
    try:
        return _year_GRL[year]
    except KeyError:
        raise Exception(f'Unrecognized year for GRL {year}')


def pretty_print(flags):
    """
    Print all the relevant flags we have set up, both from the
    metadata and from our fall-back options.
    """
    print("="*73)
    print("="*19, "xAODAnaHelpers FLAG CONFIGURATION", "="*19)
    print("="*73)
    print(" "*2, "DataType:       ", flags.Input.DataType)
    print(" "*2, "LHCPeriod:      ", flags.Input.LHCPeriod)
    print(" "*2, "RunNumber:      ", flags.Input.RunNumber)
    print(" "*2, "MCChannelNumber:", flags.Input.MCChannelNumber)
    print(" "*2, "RunNumberAsInt: ", flags.Input.RunNumberAsInt)
    print(" "*2, "AMITag:         ", flags.Input.AMITag)
    print(" "*2, "isRun3:         ", flags.Input.isRun3)
    print(" "*2, "isPHYSLITE:     ", flags.Input.isPHYSLITE)
    print(" "*2, "MCCampaign:     ", flags.Input.MCCampaign)
    print(" "*2, "GeneratorInfo:  ", flags.Input.GeneratorsInfo)
    print("="*73)
