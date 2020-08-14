#!/usr/bin/env python
# PYTHON_ARGCOMPLETE_OK
# @file:    xAH_run.py
# @purpose: run the analysis
# @author:  Giordon Stark <gstark@cern.ch>
# @date:    April 2015
#
# @example:
# @code
# xAH_run.py --help
# @endcode
#

from __future__ import print_function
#TODO: move into __main__

import argparse
try: import argcomplete
except: pass
import os
import subprocess
import sys
import datetime
import time

try:
    import configparser
except ImportError: # Python 2.x fallback
    import ConfigParser as configparser

try:
    import xAODAnaHelpers
    import xAODAnaHelpers.cli_options as xAH_cli_options
    import xAODAnaHelpers.utils as xAH_utils

# this is the situation when you're running xAH_run.py without having installed xAODAnaHelpers
# mostly needed to build documentation
except ImportError:
    import python as xAODAnaHelpers
    import python.cli_options as xAH_cli_options
    import python.utils as xAH_utils

#
# Load default options configuration
config = configparser.ConfigParser()
config.optionxform = str
config.read(os.path.expanduser("~/.xah"))

# Apply the configuration defaults
if config.has_section('general'  ): xAH_utils.update_clioption_defaults(xAH_cli_options.standard         , dict(config.items('general'  )))
if config.has_section('direct'   ): xAH_utils.update_clioption_defaults(xAH_cli_options.drivers_direct   , dict(config.items('direct'   )))
if config.has_section('prooflite'): xAH_utils.update_clioption_defaults(xAH_cli_options.drivers_prooflite, dict(config.items('prooflite')))
if config.has_section('prun'     ): xAH_utils.update_clioption_defaults(xAH_cli_options.drivers_prun     , dict(config.items('prun'     )))
if config.has_section('condor'   ): xAH_utils.update_clioption_defaults(xAH_cli_options.drivers_condor   , dict(config.items('condor'   )))
if config.has_section('lsf'      ): xAH_utils.update_clioption_defaults(xAH_cli_options.drivers_lsf      , dict(config.items('lsf'      )))
if config.has_section('slurm'    ): xAH_utils.update_clioption_defaults(xAH_cli_options.drivers_slurm    , dict(config.items('slurm'    )))

#
# if we want multiple custom formatters, use inheriting
class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter):
  pass

class _HelpAction(argparse.Action):
  def __call__(self, parser, namespace, values, option_string=None):
    if not values:
      parser.print_help()
    else:
      available_groups = [group.title for group in parser._action_groups]
      if values in available_groups:
        action_group = parser._action_groups[available_groups.index(values)]
        formatter = parser._get_formatter()
        formatter.start_section(action_group.title)
        formatter.add_text(action_group.description)
        formatter.add_arguments(action_group._group_actions)
        formatter.end_section()
        parser._print_message(formatter.format_help())
      else:
        print("That is not a valid subsection. Chose from {{{0:s}}}".format(','.join(available_groups)))
    parser.exit()

# this is a useful env variable set on our docker images
__version__ = os.getenv('xAODAnaHelpers_VERSION')
# if None, we're probably not using Docker, see if it's just a git clone
if __version__ is None:
  try:
    __version__ = subprocess.check_output(["git", "describe", "--always"], cwd=os.path.dirname(os.path.realpath(__file__)), stderr=subprocess.STDOUT).strip()
  except:
    __version__ = "private"

baseUsageStr = """xAH_run.py --files ... file [file ...]
                  --config path/to/file.json
                  [options]
                  {0:s} [{0:s} options]
"""
parser = argparse.ArgumentParser(add_help=False, description='Spin up an analysis instantly!',
                                 usage=baseUsageStr.format('driver'),
                                 formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))

# there are really no positional arguments in the automatic group
parser_requiredNamed = parser.add_argument_group('required named arguments')

# add custom help
parser.add_argument('-h', '--help', metavar='subsection', nargs='?', action=_HelpAction, help='show this help message and exit. You can also pass in the name of a subsection.')

# http://stackoverflow.com/a/16981688
parser._positionals.title = "required"
parser._optionals.title = "optional"

# positional argument, require the first argument to be the input filename
parser_requiredNamed.add_argument('--files', dest='input_filename', metavar='file', type=str, nargs='+', required=True, help='input file(s) to read. This gives all the input files for the script to use. Depending on the other options specified, these could be rucio sample names, local paths, or text files containing a list of newline separated paths/filenames.')
parser_requiredNamed.add_argument('--config', metavar='', type=str, required=True, help='configuration for the algorithms. This tells the script which algorithms to load, configure, run, and in which order. Without it, it becomes a headless chicken.')

parser.add_argument('--version', action='version', version='xAH_run.py {version}'.format(version=__version__), help='{version}'.format(version=__version__))
xAH_utils.register_on_parser(xAH_cli_options.standard, parser)

# drivers we provide support for
drivers_parser = parser.add_subparsers(prog='xAH_run.py', title='drivers', dest='driver', description='specify where to run jobs')
direct = drivers_parser.add_parser('direct',
                                   help='Run your jobs locally.',
                                   usage=baseUsageStr.format('direct'),
                                   formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))
xAH_utils.register_on_parser(xAH_cli_options.drivers_direct, direct)

prooflite = drivers_parser.add_parser('prooflite',
                                      help='Run your jobs using ProofLite',
                                      usage=baseUsageStr.format('prooflite'),
                                      formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))
xAH_utils.register_on_parser(xAH_cli_options.drivers_prooflite, prooflite)

prun = drivers_parser.add_parser('prun',
                                 help='Run your jobs on the grid using prun. Use prun --help for descriptions of the options.',
                                 usage=baseUsageStr.format('prun'),
                                 formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))
xAH_utils.register_on_parser(xAH_cli_options.drivers_prun, prun)

condor = drivers_parser.add_parser('condor',
                                   help='Flock your jobs to condor',
                                   usage=baseUsageStr.format('condor'),
                                   formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))
xAH_utils.register_on_parser(xAH_cli_options.drivers_condor, condor)

lsf = drivers_parser.add_parser('lsf',
                                help='Flock your jobs to lsf',
                                usage=baseUsageStr.format('lsf'),
                                formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))
xAH_utils.register_on_parser(xAH_cli_options.drivers_lsf, lsf)

slurm = drivers_parser.add_parser('slurm',
                                   help='Flock your jobs to SLURM',
                                   usage=baseUsageStr.format('slurm'),
                                   formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))
xAH_utils.register_on_parser(xAH_cli_options.drivers_slurm , slurm)

local = drivers_parser.add_parser('local',
                                  help='Run using the LocalDriver',
                                  usage=baseUsageStr.format('local'),
                                  formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30))


# start the script
if __name__ == "__main__":
  SCRIPT_START_TIME = datetime.datetime.now()

  try: argcomplete.autocomplete(parser)
  except: pass
  # parse the arguments, throw errors if missing any
  args = parser.parse_args()

  import logging
  xAH_logger = logging.getLogger("xAH.run")

  # set verbosity for python printing
  numeric_log_level = getattr(logging, args.log_level.upper(), None)
  if not isinstance(numeric_log_level, int):
    raise ValueError('Invalid log level: {0:s}'.format(args.log_level))
  xAH_logger.setLevel(numeric_log_level)

  ## Determine which ASG framework using env var for CMAKE setup
  ASG_framework_list = ['Base', 'Top']
  ASG_framework_type = xAH_utils.findFrameworkTypeFromList(ASG_framework_list)
  if( ASG_framework_type == None ):
    arch = os.environ.get('CMTCONFIG', os.environ.get('BINARY_TYPE', '<arch>'))
    raise OSError("It doesn't seem like the CMake environment is setup correctly. (Hint: source 'build/{0:s}/setup.sh)".format(arch))

  # architecture used for CMake
  arch = os.environ.get('Analysis'+ASG_framework_type+'_PLATFORM')

  try:
    import xAODAnaHelpers.timing

    # check environment variables for various options first before trying to do anything else
    if args.driver == 'prun':
      required_environment_variables = ['PATHENA_GRID_SETUP_SH', 'PANDA_CONFIG_ROOT', 'ATLAS_LOCAL_PANDACLIENT_PATH', 'PANDA_SYS', 'ATLAS_LOCAL_PANDACLI_VERSION']
      for env_var in required_environment_variables:
        if os.getenv(env_var) is None:
          raise EnvironmentError('Panda client is not setup. Run lsetup panda.')

      isSLC7 = True
      # check that we're not submitting to grid from CC7 machines
      try:
        xAH_logger.debug('Running lsb_release -d.')
        lsb_release = subprocess.check_output(["lsb_release", "-d"], cwd=os.path.dirname(os.path.realpath(__file__)), stderr=subprocess.STDOUT).strip()
        xAH_logger.debug('  - command output: {}'.format(lsb_release))
        slc7_release_names = ['CentOS Linux release 7', 'Scientific Linux release 7']
        if not any(name in lsb_release for name in slc7_release_names):
          xAH_logger.debug('  - did not find SLC7 in output.')
          isSLC7 = False
        else:
          xAH_logger.debug('  - found SLC7 in output.')

      except:
        xAH_logger.debug('Previous command could not run correctly. Searching through env. variables.')
        # there was a problem trying to get lsb_release, search through env variables
        lsb_release = None
        env_vars = [(k,v) for k,v in os.environ.items() if k.endswith('_PLATFORM') and k != 'Analysis'+ASG_framework_type+'_PLATFORM']
        xAH_logger.debug('Relevant environment variables found:')
        for (k,v) in env_vars: xAH_logger.debug('  - {0} = {1}'.format(k, v))
        if any(['slc6' in v for (k,v) in env_vars]):
          xAH_logger.debug('Found SLC6 in environment variable, cannot submit from this machine.')
          isSLC7 = False

      if not isSLC7:
        raise EnvironmentError('We think you\'re not running on SLC7. Grid jobs cannot be submitted from this machine. See https://xaodanahelpers.readthedocs.io/en/master/FAQs.html#slc6-vs-slc7 for more information.')

    # check submission directory
    if args.force_overwrite:
      xAH_logger.info("removing {0:s}".format(args.submit_dir))
      import shutil
      shutil.rmtree(args.submit_dir, True)
    else:
      # check if directory exists
      if os.path.exists(args.submit_dir):
        raise OSError('Output directory {0:s} already exists. Either re-run with -f/--force, choose a different --submitDir, or rm -rf it yourself. Just deal with it, dang it.'.format(args.submit_dir))

    use_scanEOS = (args.use_scanEOS)

    # singleTask is only supported with rucio and input filelists
    singleTask = (args.driver=='prun') and (args.singleTask)
    if singleTask and (not args.use_inputFileList and not args.use_scanRucio):
      xAH_logger.warning("--singleTask requires both --inputList and --inputRucio to have an effect")


    # at this point, we should import ROOT and do stuff
    import ROOT
    # Set up the job for xAOD access:
    ROOT.xAOD.Init("xAH_run").ignore()

    # load the standard algorithm since pyroot delays quickly
    xAH_logger.info("Loading up your analysis dictionaries now, give us a second.")
    ROOT.EL.Algorithm()
    # load this for the MSG::level values. See https://its.cern.ch/jira/browse/ATLASG-270
    ROOT.asg.ToolStore()
    xAH_logger.info("All dictionaries loaded and good to go. Have a wonderful day :)")

    # check that we have appropriate drivers
    if args.driver == 'prun':
      if getattr(ROOT.EL, 'PrunDriver') is None:
          raise KeyError('Cannot load the Prun driver from EventLoop. Did you not compile it?')
    elif args.driver == 'condor':
      if getattr(ROOT.EL, 'CondorDriver') is None:
        raise KeyError('Cannot load the Condor driver from EventLoop. Did you not compile it?')
    elif args.driver == 'lsf':
      if getattr(ROOT.EL, 'LSFDriver') is None:
        raise KeyError('Cannot load the LSF driver from EventLoop. Did you not compile it?')
    elif args.driver == 'slurm':
      if getattr(ROOT.EL, 'SlurmDriver') is None:
        raise KeyError('Cannot load the SLURM driver from EventLoop. Did you not compile it?')
    elif args.driver == 'local':
      if getattr(ROOT.EL, 'LocalDriver') is None:
        raise KeyError('Cannot load the Local driver from EventLoop. Did you not compile it?')

    # create a new sample handler to describe the data files we use
    xAH_logger.info("creating new sample handler")
    sh_all = ROOT.SH.SampleHandler()

    # this portion is just to output for verbosity
    if args.use_SH:
      xAH_logger.info("\t\tReading in file(s) using SH::SampleHandler::load(dir)")
    elif args.use_inputFileList:
      xAH_logger.info("\t\tReading in file(s) containing list of files")
      if args.use_scanRucio:
        xAH_logger.info("\t\tAdding samples using scanRucio")
      elif use_scanEOS:
        xAH_logger.info("\t\tAdding samples using scanEOS")
      else:
        xAH_logger.info("\t\tAdding using readFileList")
    else:
      if args.use_scanRucio:
        xAH_logger.info("\t\tAdding samples using scanRucio")
      elif use_scanEOS:
        xAH_logger.info("\t\tAdding samples using scanEOS")
      else:
        xAH_logger.info("\t\tAdding samples using scanDir")

    for fname in args.input_filename:
      if args.use_SH:
        sh_all.load(fname)
      elif args.use_inputFileList:
        # Read the filelist
        filelist=[]
        with open(fname, 'r') as f:
          for line in f:
            if line.startswith('#') : continue
            if not line.strip()     : continue
            line = line.strip()
            filelist.append(line)
        # Iterate over the filelist and add each item to the SampleHandler
        if use_scanEOS or args.use_scanXRD or (args.use_scanRucio and not singleTask):
          for line in filelist:
            if args.use_scanRucio:
              ROOT.SH.scanRucio(sh_all, line)
            elif use_scanEOS:
              base = os.path.basename(line)
              eosDataSet = os.path.dirname(line)
              ROOT.SH.ScanDir().sampleDepth(0).samplePattern(eosDataSet).scanEOS(sh_all,base)
            elif args.use_scanXRD:
              # assume format like root://someserver//path/to/files/*pattern*.root
              server, path = line.replace('root://', '').split('//')
              sh_list = ROOT.SH.DiskListXRD(server, os.path.join('/', path), True)
              ROOT.SH.ScanDir().scan(sh_all, sh_list)
            else:
              raise Exception("What just happened?")
        elif args.use_scanRucio and singleTask:
          ROOT.xAH.addRucio(sh_all,os.path.basename(fname),
                            ','.join(filelist))
        else:
          # Sample name
          sname='.'.join(os.path.basename(fname).split('.')[:-1]) # input filelist name without extension
          # Read settings
          fcname=os.path.dirname(fname)+'/'+sname+'.config' # replace .txt with .config
          config={}
          if os.path.exists(fcname): # load configuration if it exists
            with open(fcname, 'r') as f:
              for line in f:
                line=line.strip()
                parts=line.split('=')
                if len(parts)!=2: continue
                config[parts[0].strip()]=parts[1].strip()

          ROOT.SH.readFileList(sh_all, sname, fname)
          if 'xsec'    in config: sh_all.get(sname).meta().setDouble(ROOT.SH.MetaFields.crossSection    ,float(config['xsec'   ]))
          if 'filteff' in config: sh_all.get(sname).meta().setDouble(ROOT.SH.MetaFields.filterEfficiency,float(config['filteff']))
          if 'nEvents' in config: sh_all.get(sname).meta().setDouble(ROOT.SH.MetaFields.numEvents       ,float(config['nEvents']))
      else:

        if args.use_scanRucio:
          ROOT.SH.scanRucio(sh_all, fname)
        elif use_scanEOS:
          tag=args.inputTag
          if ( tag == "" ):
            tag="*"
          print("Running on EOS directory "+fname+" with tag "+tag)
          ROOT.SH.ScanDir().filePattern(tag).scanEOS(sh_all,fname)
        elif args.use_scanXRD:
          # assume format like root://someserver//path/to/files/*pattern*.root
          server, path = fname.replace('root://', '').split('//')
          sh_list = ROOT.SH.DiskListXRD(server, os.path.join(path, ''), True)
          ROOT.SH.ScanDir().scan(sh_all, sh_list)
        else:
          # need to parse and split it up
          fname_base = os.path.basename(fname)
          sample_dir = os.path.dirname(os.path.abspath(fname))
          mother_dir = os.path.dirname(sample_dir)
          sh_list = ROOT.SH.DiskListLocal(mother_dir)
          ROOT.SH.scanDir(sh_all, sh_list, fname_base, os.path.basename(sample_dir))

    # do we need to overwrite sample names?
    if args.sample_names:
      if len(args.sample_names) != len(sh_all):
        raise ValueError("You specified to override the names of {0:d} samples, but we found {1:d} samples".format(len(args.sample_names), len(sh_all)))
      else:
        for sampleName, sample in zip(args.sample_names, sh_all):
          MBJ_logger.info(" - changing sample name from {0:s} to {1:s}".format(sample.meta().getString(ROOT.SH.MetaFields.sampleName), sampleName))
          sample.meta().setString(ROOT.SH.MetaFields.sampleName, sampleName)

    # print out the samples we found
    xAH_logger.info("\t%d different dataset(s) found", len(sh_all))
        #if not args.use_scanRucio:
        #for dataset in sh_all:
        #xAH_logger.info("\t\t%d files in %s", dataset.numFiles(), dataset.name())
    sh_all.printContent()

    if len(sh_all) == 0:
      xAH_logger.info("No datasets found. Exiting.")
      sys.exit(0)

    if args.optEventsPerWorker is not None:
      xAH_logger.info("Splitting up events onto each worker. optEventsPerWorker was set!")
      ROOT.SH.scanNEvents(sh_all)

    # set the name of the tree in our files (should be configurable)
    sh_all.setMetaString( "nc_tree", args.treeName)
    #sh_all.setMetaString( "nc_excludeSite", "ANALY_RAL_SL6");
    sh_all.setMetaString( "nc_grid_filter", "*");

    # This is a fix for running on the grid with release 21.2.X
    if int(os.environ.get('ROOTCORE_RELEASE_SERIES', 0)) >= 25:
      xAH_logger.info("Setting nc_cmtConfig to {0:s}".format(os.getenv('Analysis'+ASG_framework_type+'_PLATFORM')))
      sh_all.setMetaString("nc_cmtConfig", os.getenv('Analysis'+ASG_framework_type+'_PLATFORM'))

    # read susy meta data (should be configurable)
    path_metadata=ROOT.PathResolver.FindCalibDirectory("xAODAnaHelpers/metadata")
    xAH_logger.info("reading all metadata in {0}".format(path_metadata))
    ROOT.SH.readSusyMetaDir(sh_all,path_metadata)

    # this is the basic description of our job
    xAH_logger.info("creating new job")
    job = ROOT.EL.Job()
    job.sampleHandler(sh_all)

    if args.num_events > 0:
      xAH_logger.info("\tprocessing only %d events", args.num_events)
      job.options().setDouble(ROOT.EL.Job.optMaxEvents, args.num_events)

    if args.skip_events > 0:
      xAH_logger.info("\tskipping first %d events", args.skip_events)
      job.options().setDouble(ROOT.EL.Job.optSkipEvents, args.skip_events)

    # should be configurable
    job.options().setDouble(ROOT.EL.Job.optCacheSize, 50*1024*1024)
    job.options().setDouble(ROOT.EL.Job.optCacheLearnEntries, 50)

    if args.variable_stats:
      xAH_logger.info("\tprinting variable statistics")
      job.options().setDouble(ROOT.EL.Job.optXAODPerfStats, 1)
      job.options().setDouble(ROOT.EL.Job.optPrintPerFileStats, 1)

    # access mode branch
    if args.access_mode == 'branch':
      xAH_logger.info("\tusing branch access mode: ROOT.EL.Job.optXaodAccessMode_branch")
      job.options().setString( ROOT.EL.Job.optXaodAccessMode, ROOT.EL.Job.optXaodAccessMode_branch )
    elif args.access_mode == 'athena':
      xAH_logger.info("\tusing branch access mode: ROOT.EL.Job.optXaodAccessMode_athena")
      job.options().setString (ROOT.EL.Job.optXaodAccessMode, ROOT.EL.Job.optXaodAccessMode_athena)
    else:
      xAH_logger.info("\tusing class access mode: ROOT.EL.Job.optXaodAccessMode_class")
      job.options().setString( ROOT.EL.Job.optXaodAccessMode, ROOT.EL.Job.optXaodAccessMode_class )

    # formatted string
    algorithmConfiguration_string = []

    from xAODAnaHelpers import Config
    configurator = None

    if ".json" in args.config:
      # parse_json is json.load + stripping comments
      xAH_logger.debug("Loading json files")
      algConfigs = xAH_utils.parse_json(args.config)
      xAH_logger.debug("loaded the json configurations")
      # add our algorithm to the job
      configurator = Config()
      map(lambda x: configurator.algorithm(x['class'], x['configs']), algConfigs)

    else:
      #  Executing the python
      #   (configGlobals and configLocals are used to pass vars
      configGlobals, configLocals = {}, {'args': args}
      execfile(args.config, configGlobals, configLocals)
      # Find the created xAODAnaHelpers.config.Config object and add its _algorithms to the Job
      for k,v in configLocals.items():
        if isinstance(v, Config):
          configurator = v
          break

    # setting sample metadata
    for pattern, metadata in configurator._samples.items():
      found_matching_sample = False
      xAH_logger.debug("Looking for sample(s) that matches pattern {0}".format(pattern))
      for sample in sh_all:
        if pattern in sample.name():
          found_matching_sample = True
          xAH_logger.info("Setting sample metadata for {0:s}".format(sample.name()))
          for k,t,v in ((k, type(v), v) for k,v in metadata.items()):
            if t in [float]:
              setter = 'setDouble'
            elif t in [int]:
              setter = 'setInteger'
            elif t in [bool]:
              setter = 'setBool'
            else:
              setter = 'setString'
            getattr(sample.meta(), setter)(k, v)
            xAH_logger.info("\t - sample.meta().{0:s}({1:s}, {2})".format(setter, k, v))
        if not found_matching_sample:
          xAH_logger.warning("No matching sample found for pattern {0}".format(pattern))

    for output in configurator._outputs:
      xAH_logger.info('Creating output stream "{}"'.format(output))
      job.outputAdd(ROOT.EL.OutputStream(output))

    # If we wish to add an NTupleSvc, make sure an output stream (NB: must have the same name of the service itself!)
    # is created and added to the job *before* the service
    if hasattr(ROOT.EL, 'NTupleSvc'):
      for alg in configurator._algorithms:
        if isinstance(alg, ROOT.EL.NTupleSvc) and not job.outputHas(alg.GetName()):
          job.outputAdd(ROOT.EL.OutputStream(alg.GetName()))

    # Add the algorithms to the job
    map(job.algsAdd, configurator._algorithms)

    for configLog in configurator._log:
      # this is when we have just the algorithm name
      if len(configLog) == 2:
        xAH_logger.info("creating algorithm %s with name %s", configLog[0], configLog[1])
        algorithmConfiguration_string.append("{0}: {1} options".format(*configLog))
      elif len(configLog) == 3:
        printStr = "\tsetting {0: >20}.{1:<30} = {2}"
        xAH_logger.debug("\t%s", printStr.format(*configLog))
        algorithmConfiguration_string.append(printStr.format(*configLog))
      else:
        raise Exception("Something weird happened with the logging. Tell someone important.")

    # make the driver we want to use:
    xAH_logger.info("creating driver")
    xAH_logger.info("\trunning on {0:s}".format(args.driver))
    driver = None
    if (args.driver == "direct"):
      driver = ROOT.EL.DirectDriver()

    elif (args.driver == "prooflite"):
      driver = ROOT.EL.ProofDriver()
      for opt, t in map(lambda x: (x.dest, x.type), prooflite._actions):
        if getattr(args, opt) is None: continue  # skip if not set
        if opt in ['help', 'optBatchWait', 'optBatchShellInit']: continue  # skip some options
        if t in [float]:
          setter = 'setDouble'
        elif t in [int]:
          setter = 'setInteger'
        elif t in [bool]:
          setter = 'setBool'
        else:
          setter = 'setString'
        getattr(driver.options(), setter)(getattr(ROOT.EL.Job, opt), getattr(args, opt))
        xAH_logger.info("\t - driver.options().{0:s}({1:s}, {2})".format(setter, getattr(ROOT.EL.Job, opt), getattr(args, opt)))

    elif (args.driver == "prun"):
      driver = ROOT.EL.PrunDriver()
      if args.optGridNGBPerJob is None:
        xAH_logger.warning("optGridNGBPerJob is not set. This will let the scout jobs figure out a limit for your jobs but may not be optimal. If you find your jobs are exhausted, increase the limit. A sensible limit is somewhere between 4GB and 12GB.")
      for opt, t in map(lambda x: (x.dest, x.type), prun._actions):
        if getattr(args, opt) is None: continue  # skip if not set
        if opt in ['help', 'optGridOutputSampleName', 'singleTask', 'optBatchWait', 'optBatchShellInit']: continue  # skip some options
        if t in [float]:
          setter = 'setDouble'
        elif t in [int]:
          setter = 'setInteger'
        elif t in [bool]:
          setter = 'setBool'
        else:
          setter = 'setString'
        getattr(driver.options(), setter)(getattr(ROOT.EL.Job, opt), getattr(args, opt))
        xAH_logger.info("\t - driver.options().{0:s}({1:s}, {2})".format(setter, getattr(ROOT.EL.Job, opt), getattr(args, opt)))
      nc_outputSampleNameStr = args.optGridOutputSampleName
      driver.options().setString("nc_outputSampleName", nc_outputSampleNameStr)
      xAH_logger.info("\t - driver.options().setString(nc_outputSampleName, {0:s})".format(nc_outputSampleNameStr))

    elif (args.driver == "condor"):
      driver = ROOT.EL.CondorDriver()
      driver.shellInit = args.optBatchShellInit
      for opt, t in map(lambda x: (x.dest, x.type), condor._actions):
        if getattr(args, opt) is None: continue  # skip if not set
        if opt in ['help', 'optBatchWait', 'optBatchShellInit']: continue  # skip some options
        if t in [float]:
          setter = 'setDouble'
        elif t in [int]:
          setter = 'setInteger'
        elif t in [bool]:
          setter = 'setBool'
        else:
          setter = 'setString'
        getattr(driver.options(), setter)(getattr(ROOT.EL.Job, opt), getattr(args, opt))
        xAH_logger.info("\t - driver.options().{0:s}({1:s}, {2})".format(setter, getattr(ROOT.EL.Job, opt), getattr(args, opt)))

    elif (args.driver == "lsf"):
      driver = ROOT.EL.LSFDriver()
      driver.shellInit = args.optBatchShellInit
      for opt, t in map(lambda x: (x.dest, x.type), lsf._actions):
        if getattr(args, opt) is None: continue  # skip if not set
        if opt in ['help', 'optBatchWait', 'optBatchShellInit']: continue  # skip some options
        if t in [float]:
          setter = 'setDouble'
        elif t in [int]:
          setter = 'setInteger'
        elif t in [bool]:
          setter = 'setBool'
        else:
          setter = 'setString'
        getattr(driver.options(), setter)(getattr(ROOT.EL.Job, opt), getattr(args, opt))
        xAH_logger.info("\t - driver.options().{0:s}({1:s}, {2})".format(setter, getattr(ROOT.EL.Job, opt), getattr(args, opt)))

    elif (args.driver == "slurm"):
      driver = ROOT.EL.SlurmDriver()
      driver.shellInit = args.optBatchShellInit
      driver.SetJobName         (os.path.basename(args.submit_dir))
      driver.SetAccount         (args.optSlurmAccount             )
      driver.SetPartition       (args.optSlurmPartition           )
      driver.SetRunTime         (args.optSlurmRunTime             )
      if args.optSlurmMemory:    driver.SetMemory          (args.optSlurmMemory              )
      if args.optSlurmConstrain: driver.SetConstrain       (args.optSlurmConstrain           )
      for opt, t in map(lambda x: (x.dest, x.type), slurm._actions):
        if getattr(args, opt) is None: continue  # skip if not set
        if opt in ['help', 'optBatchWait', 'optBatchShellInit', 'optSlurmAccount', 'optSlurmPartition', 'optSlurmRunTime', 'optSlurmMemory', 'optSlurmConstrain']: continue  # skip some options
        if t in [float]:
          setter = 'setDouble'
        elif t in [int]:
          setter = 'setInteger'
        elif t in [bool]:
          setter = 'setBool'
        else:
          setter = 'setString'
        jobopt=opt.replace('Slurm','BatchSlurm')
        getattr(driver.options(), setter)(getattr(ROOT.EL.Job, jobopt), getattr(args, opt))
        xAH_logger.info("\t - driver.options().{0:s}({1:s}, {2})".format(setter, getattr(ROOT.EL.Job, jobopt), getattr(args, opt)))

    elif (args.driver == "local"):
      driver = ROOT.EL.LocalDriver()
      for opt, t in map(lambda x: (x.dest, x.type), local._actions):
        if getattr(args, opt) is None: continue  # skip if not set
        if opt in ['help', 'optBatchWait', 'optBatchShellInit']: continue  # skip some options
        if t in [float]:
          setter = 'setDouble'
        elif t in [int]:
          setter = 'setInteger'
        elif t in [bool]:
          setter = 'setBool'
        else:
          setter = 'setString'
        getattr(driver.options(), setter)(getattr(ROOT.EL.Job, opt), getattr(args, opt))
        xAH_logger.info("\t - driver.options().{0:s}({1:s}, {2})".format(setter, getattr(ROOT.EL.Job, opt), getattr(args, opt)))

    xAH_logger.info("\tsubmit job")
    if args.driver in ["prun","condor","lsf","slurm","local"] and not args.optBatchWait:
      driver.submitOnly(job, args.submit_dir)
    else:
      driver.submit(job, args.submit_dir)

    SCRIPT_END_TIME = datetime.datetime.now()

    with open(os.path.join(args.submit_dir, 'xAH_run.log'), 'w+') as f:
      f.write(' '.join(['[{0}]'.format(__version__), os.path.basename(sys.argv[0])] + sys.argv[1:]))
      f.write('\n')
      if __version__ != "private":
        f.write('Code:  https://github.com/UCATLAS/xAODAnaHelpers/tree/{0}\n'.format(__version__))
      f.write('Start: {0}\nStop:  {1}\nDelta: {2}\n\n'.format(SCRIPT_START_TIME.strftime("%b %d %Y %H:%M:%S"), SCRIPT_END_TIME.strftime("%b %d %Y %H:%M:%S"), SCRIPT_END_TIME - SCRIPT_START_TIME))
      f.write('job runner options\n')
      for opt in ['input_filename', 'submit_dir', 'num_events', 'skip_events', 'force_overwrite', 'use_inputFileList', 'use_scanRucio', 'use_scanEOS', 'use_scanXRD', 'log_level', 'driver']:
        f.write('\t{0: <51} = {1}\n'.format(opt, getattr(args, opt)))
      for algConfig_str in algorithmConfiguration_string:
        f.write('{0}\n'.format(algConfig_str))

  except Exception as e:
    # we crashed
    xAH_logger.exception("{0}\nAn exception was caught!".format("-"*20))
    sys.exit(1)
