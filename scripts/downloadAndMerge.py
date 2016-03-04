#!/usr/bin/env python

##******************************************
#downloadAndMerge.py
#download and merge output datasets produced by dijet analysis code on the GRID

#NOTE before starting, set the variables below
#NOTE just run the script without any parameter to get instruction
#HOW TO time python -u downloadAndMerge.py dataset
#NOTE 'dataset' can have wildcards, the matching datasets will be treated independently
#EXAMPLE time python -u downloadAndMerge.py user.gfacini.*.v06_*.root/
##******************************************

#------------------------------------------
#import
import os, sys, subprocess, glob, shutil
import argparse
parser = argparse.ArgumentParser(description="%prog [options]", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("--container", dest='container', default="None",
     help="Name of dataset to be downloaded, may include wildcards")
parser.add_argument("--file", dest='fileOfContainers', default="None",
     help="A text file containing names of datasets to be downloaded")
parser.add_argument("--types", dest='types', default="tree",
     help="Comma separated list of types of datasets to be downloaded")
parser.add_argument("--outPath", dest='outPath', default="./gridOutput/",
     help="Output path")
parser.add_argument("--mergeRawDatasets", dest='mergeRawDatasets', default="True",
     help="Merge raw datasets (hadd)")
parser.add_argument("--doFax", dest='doFax', default=False, action="store_true", help="Use get-fax")
parser.add_argument("--renameRawDatasets", dest='renameRawDatasets', default="False",
     help="Rename raw datasets")
parser.add_argument("--maxSize", dest='maxSize', default=-1,
     help="Attempted max size (in GB) of output files. Files larger than this will not be merged. -1 for no size limit ")
args = parser.parse_args()

def main():
  ##******************************************
  #NOTE before starting, set the variables
  ##******************************************
  outputPath = args.outPath #= '/tmp/doglioni/gridOutput/'
#  datasetVariants = ['cutflow', 'hist-output', 'metadata', 'tree']#, 'log']
  datasetVariants = args.types.split(',')
  mergeRawDatasets = args.mergeRawDatasets
  renameRawDatasets = args.renameRawDatasets

  ##******************************************
  # check that hadd exists
  devnull = open(os.devnull)
  try :
    subprocess.call(["hadd"], stdout=devnull, stderr=devnull)
  except OSError as e:
    print "Error, hadd not available"
    exit(1)
  #------------------------------------------
  #get current directory
  currentDir = os.getcwd()

  containers = []

  #### Get wildcarded containers to check
  #------------------------------------------
  #if given a wildcarded set of containers
  if not args.container == 'None':
    rawDataset = args.container.rstrip('/').rstrip('*')+'*/'
    containers.append( rawDataset )
  #------------------------------------------
  #get containers from a text file
  else:
    #if given a text file of containers
    if not args.fileOfContainers == 'None':
      textFileList = [args.fileOfContainers]
    #otherwise search for containers in gridOutput/
    else:
      textFileList = glob.glob("gridOutput/gridJobs/*/outputContainers.txt") #Get lists of the output containers

    for iFile, file in enumerate(textFileList):
      with open(file, 'r') as containerFile:
        for rawline in containerFile:
          line = rawline.rstrip()
          if not ( line.find("#") == 0 or len(line) < 1 or not line ):
            rawDataset = line.rstrip().rstrip('/').rstrip('*')+'*/'
            containers.append( rawDataset )

  datasetList = []
  for container in containers:
    print container
    try:
      (stdout, stderr) = subprocess.Popen(['rucio','list-dids','--short',container], stdout=subprocess.PIPE).communicate()
    except:
      print "Error, could not run rucio.  Please setup it up with setupATLAS + localSetupRucioClients"
      exit(1)
    datasetList += [container for container in stdout.splitlines() if not container.split('.')[-1].isdigit()]


  if len(datasetList) == 0:
    print 'ERROR: No datasets matched'
    exit(1)

  datasetList.sort()
  ## choose only file types given by arguement types ##
  datasetList = [ dataset for dataset in datasetList if any(variant in dataset for variant in datasetVariants)]
  print '\nmatching datasets:'
  for dataset in datasetList:
    print ' %s'%dataset

  numDownloads  = str(len(datasetList))

  #------------------------------------------
  #prepare output directory for raw datasets
  if not os.path.exists(outputPath):
    os.mkdir(outputPath)
  if not os.path.exists(outputPath+'/rawDownload/'):
    os.mkdir(outputPath+'/rawDownload/')
  try:
    os.chdir(outputPath+'/rawDownload/')
  except:
    raise SystemExit('\n***EXIT*** couldn\'t change directory')

  #download datasets
  print '\n******************************************\ndownloading datasets'
  for idataset, dataset in enumerate(datasetList):
    print '\n ---------------------- downloading dataset ('+str(idataset)+'/'+numDownloads+'): %s'%dataset

    if args.doFax:
      print "fax-get "+dataset
      os.system("fax-get "+dataset)
    else:
      print "rucio download "+dataset+" --ndownloader 5"
      os.system("rucio download "+dataset+" --ndownloader 5")

    #remove scope from dataset name (i.e. user.x:)
    datasetList[idataset] = dataset.split(':')[1]

  #prepare output directories for merged files
  os.chdir('../')
  for variant in datasetVariants:
    directory = variant+'/'
    if not os.path.exists(directory):
      os.mkdir(directory)

  #merge datasets

  if mergeRawDatasets == "True" or renameRawDatasets == "True" :
    print '\n******************************************\nmerging files'
    for dataset in datasetList:
      if '.log/' in dataset:
        continue
      print '\n dataset: %s'%dataset

      for datasetVariant in datasetVariants:
        if datasetVariant in dataset:
          variant = datasetVariant
          break
      print '  variant: %s'%variant

      inputFilesNameWildCard = 'rawDownload/'+dataset.rstrip('/')+'*/*.root*'
      outputFileName = variant+'/'+dataset.rstrip('/')

      print '   outputFileName: %s'%outputFileName

      inputFilesName = glob.glob(inputFilesNameWildCard)

      if (mergeRawDatasets=="True") :
        print '   hadding inputFilesName: %s'%inputFilesNameWildCard
        if outputFileName.endswith('.root'):
          outputFileName = outputFileName[:-5] #strip .root

        if args.maxSize <= 0:
          os.system('hadd '+outputFileName+'.root '+inputFilesNameWildCard)
        else:
          ## Get file sizes
          fileSizes = []
          for iFile,theFile in enumerate(inputFilesName) :
            fileSizes.append( os.path.getsize(theFile)/1E9 ) #save as GB
          ## Select combinations of files
          filesToMerge = [ [] ]
          fileSizesToMerge = [ [] ]
          while len( inputFilesName ) > 0:
            if len(fileSizesToMerge[-1])==0 or float(sum( fileSizesToMerge[-1] )+fileSizes[0]) < float(args.maxSize):
              #If current list is empty or new size will be below threshold,
              #Add to current merge list and stay on this list
              filesToMerge[-1].append( inputFilesName[0] )
              fileSizesToMerge[-1].append( fileSizes[0] )

            else: #combined size is too large
              ## Move on to next merge list then add file
              filesToMerge.append( [] )
              fileSizesToMerge.append( [] )
              filesToMerge[-1].append( inputFilesName[0] )
              fileSizesToMerge[-1].append( fileSizes[0] )


            inputFilesName = inputFilesName[1:] #remove first element
            fileSizes = fileSizes[1:] #remove first element

          #for iMerge, theseFilesToMerge in enumerate( filesToMerge ):
          #  print theseFilesToMerge
          #  print fileSizesToMerge[iMerge]
          #  print sum(fileSizesToMerge[iMerge])

          ## Combine
          for iMerge, theseFilesToMerge in enumerate( filesToMerge ):
            if len( filesToMerge) == 1: #Only one output file
              os.system('hadd '+outputFileName+'.root '+' '.join(theseFilesToMerge) )
            elif len(theseFilesToMerge) == 1:
              os.system("mv "+theseFilesToMerge[0]+" "+outputFileName+"."+str(iMerge)+".root")
            else:
              os.system('hadd '+outputFileName+'.'+str(iMerge)+'.root '+' '.join(theseFilesToMerge) )


      elif (renameRawDatasets=="True") :
        print 'renaming ', inputFilesName
        for iFile,theFile in enumerate(inputFilesName) :
          os.system("mv "+theFile+" "+outputFileName+"."+str(iFile)+".root")

  #------------------------------------------
  #at last, go back to original directory
  os.chdir(currentDir)
  ##shutil.rmtree(outputPath+'/rawDownload/')


if __name__ == "__main__":
  main()
  print '\n\ndone'
