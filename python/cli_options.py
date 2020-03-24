#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging
import copy

logger = logging.getLogger("xAH.cli_options")


standard = {
    "submitDir": {
        "dest": "submit_dir",
        "metavar": "<directory>",
        "type": str,
        "required": False,
        "help": "Output directory to store the output.",
        "default": "submitDir",
    },
    "nevents": {
        "dest": "num_events",
        "metavar": "<n>",
        "type": int,
        "help": "Number of events to process for all datasets. (0 = no limit)",
        "default": 0,
    },
    "skip": {
        "dest": "skip_events",
        "metavar": "<n>",
        "type": int,
        "help": "Number of events to skip at start for all datasets. (0 = no limit)",
        "default": 0,
    },
    "force": {
        "flags": ["-f", "--force"],
        "dest": "force_overwrite",
        "action": "store_true",
        "help": "Overwrite previous directory if it exists.",
    },
    "mode": {
        "dest": "access_mode",
        "type": str,
        "metavar": "{class, branch, athena}",
        "choices": ["class", "branch", "athena"],
        "default": "class",
        "help": "run using class access mode, branch access mode, or athena access mode",
    },
    "treeName": {
        "dest": "treeName",
        "default": "CollectionTree",
        "help": "Tree Name to run on",
    },
    "isMC": {
        "action": "store_true",
        "dest": "is_MC",
        "default": False,
        "help": "Running MC",
    },
    "isAFII": {
        "action": "store_true",
        "dest": "is_AFII",
        "default": False,
        "help": "Running on AFII",
    },
    "extraOptions": {
        "dest": "extra_options",
        "metavar": "[param=val]",
        "type": str,
        "required": False,
        "help": "Pass in extra options straight into the python config file. These can be accessed by using argparse: `parser.parse_args(shlex.split(args.extra_options))`.",
        "default": "",
    },
    "inputList": {
        "dest": "use_inputFileList",
        "action": "store_true",
        "help": "If enabled, will read in a text file containing a list of newline separated paths/filenames.",
    },
    "inputTag": {
        "dest": "inputTag",
        "default": "",
        "help": "A wildcarded name of input files to run on.",
    },
    "inputRucio": {
        "dest": "use_scanRucio",
        "action": "store_true",
        "help": "If enabled, will search using Rucio. Can be combined with `--inputList`.",
    },
    "inputEOS": {
        "action": "store_true",
        "dest": "use_scanEOS",
        "default": False,
        "help": "If enabled, will search using EOS. Can be combined with `--inputList and inputTag`.",
    },
    "inputSH": {
        "action": "store_true",
        "dest": "use_SH",
        "default": False,
        "help": "If enabled, will assume the input file is a directory of ROOT files of saved SH instances to use. Call SH::SampleHandler::load() on it.",
    },
    "scanXRD": {
        "action": "store_true",
        "dest": "use_scanXRD",
        "default": False,
        "help": "If enabled, will search the xrootd server for the given pattern",
    },
    "log-level": {
        "flags": ["-l", "--log-level"],
        "type": str,
        "default": "info",
        "help": "Logging level. See https://docs.python.org/3/howto/logging.html for more info.",
    },
    "stats": {
        "action": "store_true",
        "dest": "variable_stats",
        "default": False,
        "help": "If enabled, will variable usage statistics.",
    },
    "sample-names": {
        "help": "Specify the sample names for the input files if you need to change them from the default.",
        "type": str,
        "nargs": "+",
        "default": [],
    },
}

# These are handled by xAH_run.py at the top level instead of down by drivers
# .add_argument('--optMaxEvents', type=str, required=False, default=None)
# .add_argument('--optSkipEvents', type=str, required=False, default=None)
# .add_argument('--optXaodAccessMode', type=str, required=False, default=None)
# .add_argument('--optXaodAccessMode_branch', type=str, required=False, default=None)
# .add_argument('--optXaodAccessMode_class', type=str, required=False, default=None)

# standard options for other drivers -- not used because they're only for performance-tuning
# .add_argument('--optCacheLearnEntries', type=str, required=False, default=None)
# .add_argument('--optCacheSize', type=str, required=False, default=None)
# .add_argument('--optXAODPerfStats', type=str, required=False, default=None)
# .add_argument('--optXAODReadStats', type=str, required=False, default=None)

drivers_common = {
    "optSubmitFlags": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": None,
        "help": "the name of the option for supplying extra submit parameters to batch systems",
    },
    "optEventsPerWorker": {
        "metavar": "",
        "type": float,
        "required": False,
        "default": None,
        "help": "the name of the option for selecting the number of events per batch job.  (only BatchDriver and derived drivers). warning: this option will be ignored unless you have called SH::scanNEvents first.",
    },
    "optFilesPerWorker": {
        "metavar": "",
        "type": float,
        "required": False,
        "default": None,
        "help": "the name of the option for selecting the number of files per batch job.  (only BatchDriver and derived drivers).",
    },
    "optDisableMetrics": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
        "help": "the option to turn off collection of performance data",
    },
    "optPrintPerFileStats": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
        "help": "the option to turn on printing of i/o statistics at the end of each file. warning: this is not supported for all drivers.",
    },
    "optRemoveSubmitDir": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
        "help": "the name of the option for overwriting the submission directory.  if you set this to a non-zero value it will remove any existing submit-directory before tryingto create a new one. You can also use -f/--force as well in xAH_run.py.",
    },
    "optBatchSharedFileSystem": {
        "type": bool,
        "required": False,
        "default": False,
        "help": "enable to signify whether your batch driver is running on a shared filesystem",
    },
    "optBatchWait": {
        "action": "store_true",
        "required": False,
        "help": "submit using the submit() command. This causes the code to wait until all jobs are finished and then merge all of the outputs automatically",
    },
    "optBatchShellInit": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "",
        "help": "extra code to execute on each batch node before starting EventLoop",
    },
}

# define arguments for direct driver
drivers_direct = {}
drivers_direct.update(copy.deepcopy(drivers_common))

# define arguments for prooflite driver
drivers_prooflite = {}
drivers_prooflite.update(copy.deepcopy(drivers_common))
drivers_prooflite.update({
    "optPerfTree": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
        "help": "the option to turn on the performance tree in PROOF.  if this is set to 1, it will write out the tree",
    },
    "optBackgroundProcess": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
        "help": "the option to do processing in a background process in PROOF",
    },
})

# define arguments for prun driver
drivers_prun = {}
drivers_prun.update(copy.deepcopy(drivers_common))
drivers_prun.update({
    "optGridDestSE": {"metavar": "", "type": str, "required": False, "default": None},
    "optGridSite": {"metavar": "", "type": str, "required": False, "default": None},
    "optGridCloud": {"metavar": "", "type": str, "required": False, "default": None},
    "optGridExcludedSite": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": None,
    },
    "optGridNGBPerJob": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": None,
    },
    "optGridMemory": {"metavar": "", "type": int, "required": False, "default": None},
    "optGridMaxCpuCount": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
    },
    "optGridNFiles": {"metavar": "", "type": float, "required": False, "default": None},
    "optGridNFilesPerJob": {
        "metavar": "",
        "type": float,
        "required": False,
        "default": None,
    },
    "optGridNJobs": {"metavar": "", "type": int, "required": False, "default": None},
    "optGridMaxFileSize": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
    },
    "optGridMaxNFilesPerJob": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
    },
    "optGridUseChirpServer": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
    },
    "optGridExpress": {"metavar": "", "type": str, "required": False, "default": None},
    "optGridNoSubmit": {"metavar": "", "type": int, "required": False, "default": None},
    "optGridMergeOutput": {
        "metavar": "",
        "type": float,
        "required": False,
        "default": None,
    },
    "optTmpDir": {"metavar": "", "type": str, "required": False, "default": None},
    "optRootVer": {"metavar": "", "type": str, "required": False, "default": None},
    "optCmtConfig": {"metavar": "", "type": str, "required": False, "default": None},
    "optGridDisableAutoRetry": {
        "metavar": "",
        "type": int,
        "required": False,
        "default": None,
    },
    "optOfficial": {"metavar": "", "type": int, "required": False, "default": None},
    "optVoms": {"metavar": "", "type": int, "required": False, "default": None},
    # the following is not technically supported by Job.h but it is a valid option for prun, emailed pathelp about it
    "optGridOutputSampleName": {
        "metavar": "",
        "type": str,
        "required": False,
        "help": "Define output grid sample name",
        "default": "user.%nickname%.%in:name[2]%.%in:name[3]%.%in:name[6]%.%in:name[7]%_xAH",
    },
    "singleTask": {
        "action": "store_true",
        "required": False,
        "default": False,
        "help": "Submit all input datasets under a single task.",
    },
})

drivers_condor = {}
drivers_condor.update(copy.deepcopy(drivers_common))
drivers_condor.update({
    "optCondorConf": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "stream_output = true",
        "help": "the name of the option for supplying extra parameters for condor systems",
    }
})

drivers_lsf = {}
drivers_lsf.update(copy.deepcopy(drivers_common))
drivers_lsf.update({
    "optResetShell": {
        "metavar": "",
        "type": bool,
        "required": False,
        "default": False,
        "help": "the option to reset the shell on the worker nodes",
    }
})

drivers_slurm = {}
drivers_slurm.update(copy.deepcopy(drivers_common))
drivers_slurm.update({
    "optSlurmAccount": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "atlas",
        "help": "the name of the account to use",
    },
    "optSlurmPartition": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "",
        "help": "the name of the partition to use",
    },
    "optSlurmRunTime": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "24:00:00",
        "help": "the maximum runtime",
    },
    "optSlurmMemory": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "",
        "help": "the maximum memory usage of the job (MB)",
    },
    "optSlurmConstrain": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "",
        "help": "the extra constrains on the nodes",
    },
    "optSlurmExtraConfigLines": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "",
        "help": "the extra config lines to pass to SLURM",
    },
    "optSlurmWrapperExec": {
        "metavar": "",
        "type": str,
        "required": False,
        "default": "",
        "help": "the wrapper around the run script",
    },
})
