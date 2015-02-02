#!/usr/bin/env python

################################################################################
#                                                                              #
#       Author: Giordon Stark                                                  #
#       Date: 01-20-2015                                                       #
#                                                                              #
#       A python helper to manage scaffolding code for xAODAnaHelpers and      #
#       manage appropriate package dependencies and corrects both the Makefile #
#       and the LinkDef if needed                                              #
#                                                                              #
################################################################################

# for configuration
from imp import load_source

import argparse
from glob import glob
from os import environ, path
import re
from string import Template
import sys

# for fixing the Makefile.RootCore
from tempfile import mkstemp
from shutil import move
from os import remove, close

def check_environ():
  if environ.get('ROOTCOREBIN') is None:
    print "RootCore is not set up!"
    sys.exit(1)
  return True

def get_rc_wd():
  ''' This gets the working directory of the rootcore installation '''
  check_environ()
  return path.dirname(environ['ROOTCOREBIN'])

def get_script_wd():
  ''' This gets the working directory of where the script is executed/located '''
  return path.dirname(path.realpath(__file__))

def get_rc_packages():
  '''  This gets the list of available packages in the user's rootcore
  directory by looking for the Makefile.RootCore in the appropriate package
  directory'''
  rc_wd = get_rc_wd()
  p = re.compile(r'{0}/(\w+)/cmt/Makefile.RootCore'.format(rc_wd))
  return [p.match(package).groups()[0] for package in glob('{0}/*/cmt/Makefile.RootCore'.format(rc_wd))]

def parse_template(filename, **kwargs):
  ''' Manage template parsing automatically without remembering how to do it '''
  return Template( open(filename).read() ).substitute(**kwargs)

def check_makefile(args):
  if args.pkg == 'xAODAnaHelpers':
    # do not do this for xAODAnaHelpers
    return True

  # first make temp file to write the new Makefile.RootCore in case
  temp_fh, temp_abspath = mkstemp()
  temp_file = open(temp_abspath, 'w')

  # get the path to the Makefile
  #  we know the Makefile exists because of `get_rc_packages()`
  pkg_makefile_path = path.join(get_rc_wd(), args.pkg, 'cmt', 'Makefile.RootCore')

  # set up fixDependencies to determine what the user does
  fixDependencies = False
  # open and read line by line
  with open(pkg_makefile_path, 'r') as f:
    for line in f:
      if line.startswith('PACKAGE_DEP'):
        if not 'xAODAnaHelpers' in line:
          # ask user if we want to fix up their makefile
          print "We have determined that {0} does not list xAODAnaHelpers as a dependency.".format(args.pkg)
          fixDependencies = bool(query_yes_no("Do you want us to fix this?") == "yes")
          if fixDependencies:
            # they want us to, we'll read it in, grab all packages, add xAODAnaHelpers, and then sort
            package_deps = sorted(line.strip().split('=')[1].strip().split(' ') + ['xAODAnaHelpers'])
            # join and write to temp file
            temp_file.write('PACKAGE_DEP \t = {0}\n'.format(' '.join(package_deps)))
          else:
            temp_file.write(line)
        else:
          temp_file.write(line)
      else:
        temp_file.write(line)
  # close the temp file
  temp_file.close()
  close(temp_fh)
  # they want us to fix it up
  if fixDependencies:
    # remove original file
    remove(pkg_makefile_path)
    # move temp file
    move(temp_abspath, pkg_makefile_path)
  else:
    # remove the temp file, since we're not moving it
    remove(temp_abspath)
  return True

def check_linkdef(args, className):
  # check the linkdef
  temp_fg, temp_abspath = mkstemp()
  temp_file = open(temp_abspath, 'w')
  # get the path to the LinkDef
  pkg_linkdef_path = path.join(get_rc_wd(), args.pkg, 'Root', 'LinkDef.h')
  # read through file and determine if needs to be included
  foundLinkDef = False
  with open(pkg_linkdef_path, 'r') as f:
    for line in f:
      if line.startswith('#include'):
        if className in line:
          foundLinkDef = True
          answered = True
      # copy lines over to temp file always
      temp_file.write(line)
  # finished reading in the file
  additionalString = """
/*This was automatically inserted by xAH_scaffold.py*/
#include <{0}/{1}.h>
#ifdef __CINT__
#pragma link C++ class {1}+;
#endif
/*This was automatically inserted by xAH_scaffold.py*/""".format(args.pkg, className)
  temp_file.write(additionalString)
  # close the temp_file
  temp_file.close()
  close(temp_fg)

  # ask user if we want to fix up their linkdef
  if not foundLinkDef:
    print "We have determined that your LinkDef does not contain a pragma link for `{0}` yet."
    if bool(query_yes_no("Do you want us to fix this?") == "yes"):
      # they want us to fix it up
      # remove original file
      remove(pkg_linkdef_path)
      # move temp file
      move(temp_abspath, pkg_linkdef_path)
    else:
      remove(temp_abspath)
  else:
    remove(temp_abspath)
  return True

# http://code.activestate.com/recipes/577058/
def query_yes_no(question, default="yes"):
  """Ask a yes/no question via raw_input() and return their answer.
  
  "question" is a string that is presented to the user.
  "default" is the presumed answer if the user just hits <Enter>.
      It must be "yes" (the default), "no" or None (meaning
      an answer is required of the user).

  The "answer" return value is one of "yes" or "no".
  """
  valid = {"yes":"yes",   "y":"yes",  "ye":"yes",
           "no":"no",     "n":"no"}
  if default == None:
    prompt = " [y/n] "
  elif default == "yes":
    prompt = " [Y/n] "
  elif default == "no":
    prompt = " [y/N] "
  else:
    raise ValueError("invalid default answer: '%s'" % default)

  while 1:
    sys.stdout.write(question + prompt)
    choice = raw_input().lower()
    if default is not None and choice == '':
      return default
    elif choice in valid.keys():
      return valid[choice]
    else:
      sys.stdout.write("Please respond with 'yes' or 'no' "\
                       "(or 'y' or 'n').\n")


if __name__ == "__main__":
  # script working directory, and rootcore working directory
  script_wd = get_script_wd()
  rc_wd = get_rc_wd()

  # load in the configuration file which contains the templates
  if not path.isfile(path.join(script_wd, 'config')):
    print "Missing configuration file! This is problematic."
    sys.exit(1)

  try:
    templates = getattr(load_source('config',path.join(script_wd, 'config')), "templates")
  except AttributeError:
    print "Missing template configuration variable `templates` inside `config`. This is problematic."
    sys.exit(1)

  # parse all arguments appropriately
  parser = argparse.ArgumentParser(description='Scaffolding for xAOD Analysis Helpers')
  parser.add_argument('pkg',  help='what package to scaffold into, e.g. xAODAnaHelpers',  choices=get_rc_packages())
  parser.add_argument('name',     help='the name of the file & class, e.g. Jet or Track. We automatically append appropriate names to the end to be consistent with xAODAnaHelpers naming scheme.')
  parser.add_argument('type',     help='what kind of file to scaffold, e.g. plots',           choices=templates.keys())

  args = parser.parse_args()
  template_arguments = {'name': args.name, 'package': args.pkg}

  # build up the absolute paths and relative paths for the template we want to
  # use. This can be looped over multiple times to do multiple checks which is
  # convenient
  fileExists = 0
  missingTemplateFile = 0
  for file_to_scaffold in templates[args.type]:
    if file_to_scaffold['type'] == 'pair':
      # build relative paths to output files, relative to package
      file_to_scaffold['output_headerFile_relpath'] = path.join(args.pkg, args.pkg, '{0}{1}.h'.format(args.name, file_to_scaffold['append']))
      file_to_scaffold['output_sourceFile_relpath'] = path.join(args.pkg, 'Root', '{0}{1}.cxx'.format(args.name, file_to_scaffold['append']))
      # build absolute paths to output files
      file_to_scaffold['output_headerFile_abspath'] = path.join(rc_wd, file_to_scaffold['output_headerFile_relpath'])
      file_to_scaffold['output_sourceFile_abspath'] = path.join(rc_wd, file_to_scaffold['output_sourceFile_relpath'])
      # check if the file(s) exist already in the package being used
      if path.isfile(file_to_scaffold['output_headerFile_abspath']):
        print "Header file already exists at {0}".format(file_to_scaffold['output_headerFile_relpath'])
        fileExists += 1
      if path.isfile(file_to_scaffold['output_sourceFile_abspath']):
        print "Source file already exists at {0}".format(file_to_scaffold['output_sourceFile_relpath'])
        fileExists += 1
      # check that the template files exist as well
      # make sure this exists first... just in case some idiot doesn't run `python config` for checking
      if not path.isfile(path.join(script_wd, 'templates', file_to_scaffold['header'])):
        print "Missing header template file! Please inform someone: {0}".format(file_to_scaffold['header'])
        missingTemplateFile += 1
      if not path.isfile(path.join(script_wd, 'templates', file_to_scaffold['source'])):
        print "Missing source template file! Please inform someone: {0}".format(file_to_scaffold['source'])
        missingTemplateFile += 1

    elif file_to_scaffold['type'] == 'util':
      # build relative paths to output files, relative to package
      file_to_scaffold['output_sourceFile_relpath'] = path.join(args.pkg, 'util', '{0}{1}.cxx'.format(args.name, file_to_scaffold['append']))
      # build absolute paths to output files
      file_to_scaffold['output_sourceFile_abspath'] = path.join(rc_wd, file_to_scaffold['output_sourceFile_relpath'])
      # check if the file(s) exist already in the package being used
      if path.isfile(file_to_scaffold['output_sourceFile_abspath']):
        print "Source file already exists at {0}".format(file_to_scaffold['output_sourceFile_relpath'])
        fileExists += 1
      #check that the template files exist as well
      # make sure this exists first... just in case some idiot doesn't run `python config` for checking
      if not path.isfile(path.join(script_wd, 'templates', file_to_scaffold['source'])):
        print "Missing source template file! Please inform someone: {0}".format(file_to_scaffold['source'])
        missingTemplateFile += 1

  if fileExists > 0 or missingTemplateFile > 0:
    sys.exit(1)

  # now, let's alert the user to what files we are going to be creating and ask
  # for confirmation
  numFilesCreated = 0
  print "Files created will be in {0} at:".format(args.pkg)
  for file_to_scaffold in templates[args.type]:
    print "\tType: {0}".format(file_to_scaffold['append'])

    if file_to_scaffold['type'] == 'pair':
      print "\t\tHeader: {0}".format(file_to_scaffold['output_headerFile_relpath'])
      print "\t\tSource: {0}".format(file_to_scaffold['output_sourceFile_relpath'])
      numFilesCreated += 2

    elif file_to_scaffold['type'] == 'util':
      print "\t\tSource: {0}".format(file_to_scaffold['output_sourceFile_relpath'])
      numFilesCreated += 1

  if query_yes_no("{0:d} files will be created. Is this correct?".format(numFilesCreated), "no") == "no":
    sys.exit(0)

  # a flag to update linkdef
  linkDef = []
  # files don't exist and user is ok with list of files to be made
  for file_to_scaffold in templates[args.type]:
    if file_to_scaffold['type'] == 'pair':
      # parse the template files and replace the contents with substitution
      output_headerFile_contents = parse_template(path.join(script_wd, 'templates', file_to_scaffold['header']), **template_arguments)
      output_sourceFile_contents = parse_template(path.join(script_wd, 'templates', file_to_scaffold['source']), **template_arguments)
      with open(file_to_scaffold['output_headerFile_abspath'], 'w') as f:
        f.write(output_headerFile_contents)
      with open(file_to_scaffold['output_sourceFile_abspath'], 'w') as f:
        f.write(output_sourceFile_contents)

    elif file_to_scaffold['type'] == 'util':
      # parse the template files and replace the contents with substitution
      output_sourceFile_contents = parse_template(path.join(script_wd, 'templates', file_to_scaffold['source']), **template_arguments)
      with open(file_to_scaffold['output_sourceFile_abspath'], 'w') as f:
        f.write(output_sourceFile_contents)

    # deal with linkdef flag
    if file_to_scaffold.get('linkdef', False):
      linkDef.append("{0}{1}".format(args.name, file_to_scaffold['append']))

  # always check the makefile
  check_makefile(args)
  # always check the linkDef
  for className in linkDef:
    check_linkdef(args, className)
