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
parser_requiredNamed.add_argument('--files', dest='input_filename', metavar='file', type=str, nargs='+', required=True, help='input file(s) to read. This gives all the input files for the script to use. Depending on the other options specified, these could be DQ2 sample names, local paths, or text files containing a list of filenames/paths.')
parser_requiredNamed.add_argument('--config', metavar='', type=str, required=True, help='configuration for the algorithms. This tells the script which algorithms to load, configure, run, and in which order. Without it, it becomes a headless chicken.')
parser.add_argument('--submitDir', dest='submit_dir', metavar='<directory>', type=str, required=False, help='Output directory to store the output.', default='submitDir')
parser.add_argument('--nevents', dest='num_events', metavar='<n>', type=int, help='Number of events to process for all datasets. (0 = no limit)', default=0)
parser.add_argument('--skip', dest='skip_events', metavar='<n>', type=int, help='Number of events to skip at start for all datasets. (0 = no limit)', default=0)
parser.add_argument('-f', '--force', dest='force_overwrite', action='store_true', help='Overwrite previous directory if it exists.')

parser.add_argument('--version', action='version', version='xAH_run.py {version}'.format(version=__version__), help='{version}'.format(version=__version__))
parser.add_argument('--mode', dest='access_mode', type=str, metavar='{class, branch, athena}', choices=['class', 'branch', 'athena'], default='class', help='run using class access mode, branch access mode, or athena access mode')
parser.add_argument('--treeName', dest="treeName",     default="CollectionTree", help="Tree Name to run on")
parser.add_argument('--isMC',     action="store_true", dest="is_MC",    default=False, help="Running MC")
parser.add_argument('--isAFII',   action="store_true", dest="is_AFII",  default=False, help="Running on AFII")
parser.add_argument('--extraOptions', dest="extra_options", metavar="[param=val]", type=str, required=False, help='Pass in extra options straight into the python config file. These can be accessed by using argparse: `parser.parse_args(shlex.split(args.extra_options))`.', default='')

parser.add_argument('--inputList', dest='use_inputFileList', action='store_true', help='If enabled, will read in a text file containing a list of paths/filenames.')
parser.add_argument('--inputTag', dest='inputTag', default="", help='A wildcarded name of input files to run on.')
parser.add_argument('--inputDQ2', dest='use_scanDQ2', action='store_true', help='[DEPRECATION] Use inputRucio instead.')
parser.add_argument('--inputRucio', dest='use_scanRucio', action='store_true', help='If enabled, will search using Rucio. Can be combined with `--inputList`.')
parser.add_argument('--inputEOS', action='store_true', dest='use_scanEOS', default=False, help='If enabled, will search using EOS. Can be combined with `--inputList and inputTag`.')
parser.add_argument('--scanXRD', action='store_true', dest='use_scanXRD', default=False, help='If enabled, will search the xrootd server for the given pattern')
parser.add_argument('-l', '--log-level', type=str, default='info', help='Logging level. See https://docs.python.org/3/howto/logging.html for more info.')
parser.add_argument('--stats', action='store_true', dest='variable_stats', default=False, help='If enabled, will variable usage statistics.')

# first is the driver common arguments
drivers_common = argparse.ArgumentParser(add_help=False, description='Common Driver Arguments')
drivers_common.add_argument('--optSubmitFlags', metavar='', type=str, required=False, default=None, help='the name of the option for supplying extra submit parameters to batch systems')
drivers_common.add_argument('--optEventsPerWorker', metavar='', type=float, required=False, default=None, help='the name of the option for selecting the number of events per batch job.  (only BatchDriver and derived drivers). warning: this option will be ignored unless you have called SH::scanNEvents first.')  # TODO: add a check so we can run SH::scanNEvents to spread workload more evenly
drivers_common.add_argument('--optFilesPerWorker', metavar='', type=float, required=False, default=None, help='the name of the option for selecting the number of files per batch job.  (only BatchDriver and derived drivers).')
drivers_common.add_argument('--optDisableMetrics', metavar='', type=int, required=False, default=None, help='the option to turn off collection of performance data')
drivers_common.add_argument('--optPrintPerFileStats', metavar='', type=int, required=False, default=None, help='the option to turn on printing of i/o statistics at the end of each file. warning: this is not supported for all drivers.')
drivers_common.add_argument('--optRemoveSubmitDir', metavar='', type=int, required=False, default=None, help='the name of the option for overwriting the submission directory.  if you set this to a non-zero value it will remove any existing submit-directory before tryingto create a new one. You can also use -f/--force as well in xAH_run.py.')
drivers_common.add_argument('--optBatchSharedFileSystem', type=bool, required=False, default=False, help='enable to signify whether your batch driver is running on a shared filesystem')
drivers_common.add_argument('--optBatchWait', action='store_true', required=False, help='submit using the submit() command. This causes the code to wait until all jobs are finished and then merge all of the outputs automatically')
drivers_common.add_argument('--optBatchShellInit', metavar='', type=str, required=False, default='', help='extra code to execute on each batch node before starting EventLoop')

# These are handled by xAH_run.py at the top level instead of down by drivers
#.add_argument('--optMaxEvents', type=str, required=False, default=None)
#.add_argument('--optSkipEvents', type=str, required=False, default=None)
#.add_argument('--optXaodAccessMode', type=str, required=False, default=None)
#.add_argument('--optXaodAccessMode_branch', type=str, required=False, default=None)
#.add_argument('--optXaodAccessMode_class', type=str, required=False, default=None)

# standard options for other drivers -- not used because they're only for performance-tuning
#.add_argument('--optCacheLearnEntries', type=str, required=False, default=None)
#.add_argument('--optCacheSize', type=str, required=False, default=None)
#.add_argument('--optXAODPerfStats', type=str, required=False, default=None)
#.add_argument('--optXAODReadStats', type=str, required=False, default=None)

# then the drivers we provide support for
drivers_parser = parser.add_subparsers(prog='xAH_run.py', title='drivers', dest='driver', description='specify where to run jobs')
direct = drivers_parser.add_parser('direct',
                                   help='Run your jobs locally.',
                                   usage=baseUsageStr.format('direct'),
                                   formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30),
                                   parents=[drivers_common])

prooflite = drivers_parser.add_parser('prooflite',
                                      help='Run your jobs using ProofLite',
                                      usage=baseUsageStr.format('prooflite'),
                                      formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30),
                                      parents=[drivers_common])

prun = drivers_parser.add_parser('prun',
                                 help='Run your jobs on the grid using prun. Use prun --help for descriptions of the options.',
                                 usage=baseUsageStr.format('prun'),
                                 formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30),
                                 parents=[drivers_common])

condor = drivers_parser.add_parser('condor',
                                   help='Flock your jobs to condor',
                                   usage=baseUsageStr.format('condor'),
                                   formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30),
                                   parents=[drivers_common])

lsf = drivers_parser.add_parser('lsf',
                                help='Flock your jobs to lsf',
                                usage=baseUsageStr.format('lsf'),
                                formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30),
                                parents=[drivers_common])

slurm = drivers_parser.add_parser('slurm',
                                   help='Flock your jobs to SLURM',
                                   usage=baseUsageStr.format('slurm'),
                                   formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30),
                                   parents=[drivers_common])

local = drivers_parser.add_parser('local',
                                  help='Run using the LocalDriver',
                                  usage=baseUsageStr.format('local'),
                                  formatter_class=lambda prog: CustomFormatter(prog, max_help_position=30),
                                  parents=[drivers_common])

# define arguments for prooflite driver
prooflite.add_argument('--optPerfTree',          metavar='', type=int, required=False, default=None, help='the option to turn on the performance tree in PROOF.  if this is set to 1, it will write out the tree')
prooflite.add_argument('--optBackgroundProcess', metavar='', type=int, required=False, default=None, help='the option to do processing in a background process in PROOF')

# define arguments for prun driver
prun.add_argument('--optGridDestSE',           metavar='', type=str, required=False, default=None)
prun.add_argument('--optGridSite',             metavar='', type=str, required=False, default=None)
prun.add_argument('--optGridCloud',            metavar='', type=str, required=False, default=None)
prun.add_argument('--optGridExcludedSite',     metavar='', type=str, required=False, default=None)
prun.add_argument('--optGridNGBPerJob',        metavar='', type=str, required=False, default=2)
prun.add_argument('--optGridMemory',           metavar='', type=int, required=False, default=None)
prun.add_argument('--optGridMaxCpuCount',      metavar='', type=int, required=False, default=None)
prun.add_argument('--optGridNFiles',           metavar='', type=float, required=False, default=None)
prun.add_argument('--optGridNFilesPerJob',     metavar='', type=float, required=False, default=None)
prun.add_argument('--optGridNJobs',            metavar='', type=int, required=False, default=None)
prun.add_argument('--optGridMaxFileSize',      metavar='', type=int, required=False, default=None)
prun.add_argument('--optGridMaxNFilesPerJob',  metavar='', type=int, required=False, default=None)
prun.add_argument('--optGridUseChirpServer',   metavar='', type=int, required=False, default=None)
prun.add_argument('--optGridExpress',          metavar='', type=str, required=False, default=None)
prun.add_argument('--optGridNoSubmit',         metavar='', type=int, required=False, default=None)
prun.add_argument('--optGridMergeOutput',      metavar='', type=float, required=False, default=None)
prun.add_argument('--optTmpDir',               metavar='', type=str, required=False, default=None)
prun.add_argument('--optRootVer',              metavar='', type=str, required=False, default=None)
prun.add_argument('--optCmtConfig',            metavar='', type=str, required=False, default=None)
prun.add_argument('--optGridDisableAutoRetry', metavar='', type=int, required=False, default=None)
prun.add_argument('--optOfficial',             metavar='', type=int, required=False, default=None)
prun.add_argument('--optVoms',                 metavar='', type=int, required=False, default=None)
# the following is not technically supported by Job.h but it is a valid option for prun, emailed pathelp about it
prun.add_argument('--optGridOutputSampleName', metavar='', type=str, required=False, help='Define output grid sample name', default='user.%nickname%.%in:name[2]%.%in:name[3]%.%in:name[6]%.%in:name[7]%_xAH')

# define arguments for condor driver
condor.add_argument('--optCondorConf', metavar='', type=str, required=False, default='stream_output = true', help='the name of the option for supplying extra parameters for condor systems')

# define arguments for lsf driver
lsf.add_argument('--optResetShell', metavar='', type=bool, required=False, default=False, help='the option to reset the shell on the worker nodes')

# define arguments for slurm driver
slurm.add_argument('--optSlurmAccount'         , metavar='', type=str, required=False, default='atlas'      , help='the name of the account to use')
slurm.add_argument('--optSlurmPartition'       , metavar='', type=str, required=False, default='shared-chos', help='the name of the partition to use')
slurm.add_argument('--optSlurmRunTime'         , metavar='', type=str, required=False, default='24:00:00'   , help='the maximum runtime')
slurm.add_argument('--optSlurmMemory'          , metavar='', type=str, required=False, default='1800'       , help='the maximum memory usage of the job (MB)')
slurm.add_argument('--optSlurmConstrain'       , metavar='', type=str, required=False, default=''           , help='the extra constrains on the nodes')
slurm.add_argument('--optSlurmExtraConfigLines', metavar='', type=str, required=False, default=''           , help='the extra config lines to pass to SLURM')
slurm.add_argument('--optSlurmWrapperExec'     , metavar='', type=str, required=False, default=''           , help='the wrapper around the run script')

# start the script
if __name__ == "__main__":
  SCRIPT_START_TIME = datetime.datetime.now()

  try: argcomplete.autocomplete(parser)
  except: pass
  # parse the arguments, throw errors if missing any
  args = parser.parse_args()

  import xAODAnaHelpers
  import logging
  xAH_logger = logging.getLogger("xAH.run")

  # set verbosity for python printing
  numeric_log_level = getattr(logging, args.log_level.upper(), None)
  if not isinstance(numeric_log_level, int):
    raise ValueError('Invalid log level: {0:s}'.format(args.log_level))
  xAH_logger.setLevel(numeric_log_level)

  try:
    import xAODAnaHelpers.timing

    # check environment variables for various options first before trying to do anything else
    if args.driver == 'prun':
      required_environment_variables = ['PATHENA_GRID_SETUP_SH', 'PANDA_CONFIG_ROOT', 'ATLAS_LOCAL_PANDACLIENT_PATH', 'PANDA_SYS', 'ATLAS_LOCAL_PANDACLI_VERSION']
      for env_var in required_environment_variables:
        if os.getenv(env_var) is None:
          raise EnvironmentError('Panda client is not setup. Run localSetupPandaClient.')

    # check submission directory
    if args.force_overwrite:
      xAH_logger.info("removing {0:s}".format(args.submit_dir))
      import shutil
      shutil.rmtree(args.submit_dir, True)
    else:
      # check if directory exists
      if os.path.exists(args.submit_dir):
        raise OSError('Output directory {0:s} already exists. Either re-run with -f/--force, choose a different --submitDir, or rm -rf it yourself. Just deal with it, dang it.'.format(args.submit_dir))

    # they will need it to get it working for rel 20
    needXRD = ( args.use_scanDQ2|args.use_scanRucio ) & (args.driver!='prun')
    if needXRD:
      if os.getenv('XRDSYS') is None and os.getenv('RUCIO_HOME') is None:
        raise EnvironmentError('xrootd client is not setup. Run localSetupFAX or equivalent.')

    use_scanEOS = (args.use_scanEOS)


    # at this point, we should import ROOT and do stuff
    import ROOT
    if xAODAnaHelpers.utils.is_release20():
      xAH_logger.info("loading packages")
      ROOT.gROOT.Macro("$ROOTCOREDIR/scripts/load_packages.C")
    else:
      ## Determine which ASG framework using env var for CMAKE setup
      ASG_framework_list = ['Base', 'Top']
      ASG_framework_type = xAODAnaHelpers.utils.findFrameworkTypeFromList(ASG_framework_list)
      if( ASG_framework_type == None ):
        arch = os.environ.get('CMTCONFIG', os.environ.get('BINARY_TYPE', '<arch>'))
        raise OSError("It doesn't seem like the CMake environment is setup correctly. (Hint: source 'build/{0:s}/setup.sh)".format(arch))

      # architecture used for CMake
      arch = os.environ.get('Analysis'+ASG_framework_type+'_PLATFORM')

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
      if not os.path.isfile(os.path.expandvars('$ROOTCOREBIN/../RootCore.par')) and xAODAnaHelpers.utils.is_release20():
        raise IOError('I cannot find RootCore.par. Make sure you run `rc make_par` before running xAH_run.py.')
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
    if args.use_inputFileList:
      xAH_logger.info("\t\tReading in file(s) containing list of files")
      if args.use_scanDQ2:
        xAH_logger.info("\t\tAdding samples using scanDQ2")
        xAH_logger.warning("\033[5m\t\tUse inputRucio instead\033[0m")
      elif args.use_scanRucio:
        xAH_logger.info("\t\tAdding samples using scanRucio")
      elif use_scanEOS:
        xAH_logger.info("\t\tAdding samples using scanEOS")
      else:
        xAH_logger.info("\t\tAdding using readFileList")
    else:
      if args.use_scanDQ2:
        xAH_logger.info("\t\tAdding samples using scanDQ2")
        xAH_logger.warning("\033[5m\t\tUse inputRucio instead\033[0m")
      if args.use_scanRucio:
        xAH_logger.info("\t\tAdding samples using scanRucio")
      elif use_scanEOS:
        xAH_logger.info("\t\tAdding samples using scanEOS")
      else:
        xAH_logger.info("\t\tAdding samples using scanDir")

    for fname in args.input_filename:
      if args.use_inputFileList:
        if (args.use_scanDQ2 or use_scanEOS or args.use_scanXRD or args.use_scanRucio):
          with open(fname, 'r') as f:
            for line in f:
              if line.startswith('#') : continue
              if not line.strip()     : continue
              line = line.strip()
              if args.use_scanDQ2:
                ROOT.SH.scanDQ2(sh_all, line)
              elif args.use_scanRucio:
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

        if args.use_scanDQ2:
          ROOT.SH.scanDQ2(sh_all, fname)
        elif args.use_scanRucio:
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

    # print out the samples we found
    xAH_logger.info("\t%d different dataset(s) found", len(sh_all))
        #if not args.use_scanDQ2:
        #for dataset in sh_all:
        #xAH_logger.info("\t\t%d files in %s", dataset.numFiles(), dataset.name())
    sh_all.printContent()

    if len(sh_all) == 0:
      xAH_logger.info("No datasets found. Exiting.")
      sys.exit(0)

    # set the name of the tree in our files (should be configurable)
    sh_all.setMetaString( "nc_tree", args.treeName)
    #sh_all.setMetaString( "nc_excludeSite", "ANALY_RAL_SL6");
    sh_all.setMetaString( "nc_grid_filter", "*");

    # This is a fix for running on the grid with release 21.2.X
    if int(os.environ.get('ROOTCORE_RELEASE_SERIES', 0)) >= 25:
      xAH_logger.info("Setting nc_cmtConfig to {0:s}".format(os.getenv('Analysis'+ASG_framework_type+'_PLATFORM')))
      sh_all.setMetaString("nc_cmtConfig", os.getenv('Analysis'+ASG_framework_type+'_PLATFORM'))

    # read susy meta data (should be configurable)
    path_metadata=ROOT.PathResolverFindCalibDirectory("xAODAnaHelpers/metadata")
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
      from xAODAnaHelpers.utils import parse_json
      xAH_logger.debug("Loading json files")
      algConfigs = parse_json(args.config)
      xAH_logger.debug("loaded the json configurations")
      # add our algorithm to the job
      configurator = Config()
      map(lambda x: configurator.setalg(x['class'], x['configs']), algConfigs)

    else:
      #  Executing the python
      #   (configGlobals and configLocals are used to pass vars
      configGlobals, configLocals = {}, {'args': args}
      execfile(args.config, configGlobals, configLocals)
      # Find the created xAODAnaHelpers.config.Config object and add its _algorithms to the Job
      for k,v in configLocals.iteritems():
        if isinstance(v, Config):
          configurator = v
          break

    # setting sample metadata
    for pattern, metadata in configurator._samples.iteritems():
      found_matching_sample = False
      xAH_logger.debug("Looking for sample(s) that matches pattern {0}".format(pattern))
      for sample in sh_all:
        if pattern in sample.name():
          found_matching_sample = True
          xAH_logger.info("Setting sample metadata for {0:s}".format(sample.name()))
          for k,t,v in ((k, type(v), v) for k,v in metadata.iteritems()):
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
      for opt, t in map(lambda x: (x.dest, x.type), prun._actions):
        if getattr(args, opt) is None: continue  # skip if not set
        if opt in ['help', 'optGridOutputSampleName', 'optBatchWait', 'optBatchShellInit']: continue  # skip some options
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
      driver.SetMemory          (args.optSlurmMemory              )
      driver.SetConstrain       (args.optSlurmConstrain           )
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
      for opt in ['input_filename', 'submit_dir', 'num_events', 'skip_events', 'force_overwrite', 'use_inputFileList', 'use_scanDQ2', 'use_scanRucio', 'use_scanEOS', 'use_scanXRD', 'log_level', 'driver']:
        f.write('\t{0: <51} = {1}\n'.format(opt, getattr(args, opt)))
      for algConfig_str in algorithmConfiguration_string:
        f.write('{0}\n'.format(algConfig_str))

  except Exception as e:
    # we crashed
    xAH_logger.exception("{0}\nAn exception was caught!".format("-"*20))
    sys.exit(1)
