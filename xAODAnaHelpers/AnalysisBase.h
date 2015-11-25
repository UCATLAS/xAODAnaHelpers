#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/DiskListLocal.h"
#include "SampleHandler/DiskListEOS.h"
#include "SampleHandler/ToolsDiscovery.h"
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/stat.h>

#include "EventLoop/DirectDriver.h"
#include "EventLoop/CondorDriver.h"
#include "EventLoopGrid/PrunDriver.h"

#include "TEnv.h"
#include "TSystem.h"

int setupJob(int argc, char* argv[],  EL::Job& job,
	     std::string& configName, std::string& submitDir, std::string& systName,
	     float& systVal, bool& f_grid, bool& f_production, std::string& outputName,   std::vector< std::string >& outputContainerNames){


  /////////// Retrieve job arguments //////////////////////////
  std::vector< std::string> options;
  for(int ii=1; ii < argc; ++ii){
    options.push_back( std::string(argv[ii]) );
  }


  if (argc > 1 && options.at(0).compare("-h") == 0) {
    std::cout << std::endl
	      << " job submission" << std::endl
	      << std::endl
	      << " Optional arguments:" << std::endl
	      << "  -h               Prints this menu" << std::endl
	      << "  -inFile          Path to a folder, root file, or text file" << std::endl
	      << "  -inputTag        A wildcarded file name to run on" << std::endl
	      << "  -outputTag       Version string to be appended to job name" << std::endl
	      << "  -submitDir       Name of output directory" << std::endl
	      << "  -configName      Path to config file" << std::endl
	      << "  -syst            Name AND value for systematic" << std::endl
	      << "  -production      Group name for the production role" << std::endl
	      << std::endl;
    exit(1);
  }

  std::string samplePath = ".";
  std::string outputTag  = "";
  std::string inputTag   = "";
  std::string productionName = "";

  int iArg = 0;
  while(iArg < argc-1) {

    if (options.at(iArg).compare("-h") == 0) {
       // Ignore if not first argument
       ++iArg;

    } else if (options.at(iArg).compare("-inFile") == 0) {
       char tmpChar = options.at(iArg+1)[0];
       if (iArg+1 == argc || tmpChar == '-' ) {
         std::cout << " -inFile should be followed by a file or folder" << std::endl;
         return 1;
       } else {
         samplePath = options.at(iArg+1);
         iArg += 2;
       }

    } else if (options.at(iArg).compare("-inputTag") == 0) {
      char tmpChar = options.at(iArg+1)[0];
      if (iArg+1 == argc || tmpChar == '-' ) {
	std::cout << " -inputTag is a wildcarded file name to run on" << std::endl;
	return 1;
      } else {
	inputTag = options.at(iArg+1);
	iArg += 2;
      }

    } else if (options.at(iArg).compare("-outputTag") == 0) {
       char tmpChar = options.at(iArg+1)[0];
       if (iArg+1 == argc || tmpChar == '-' ) {
         std::cout << " -outputTag should be followed by a job version string" << std::endl;
         return 1;
       } else {
         outputTag = options.at(iArg+1);
         iArg += 2;
       }

    } else if (options.at(iArg).compare("-submitDir") == 0) {
       char tmpChar = options.at(iArg+1)[0];
       if (iArg+1 == argc || tmpChar == '-' ) {
         std::cout << " -submitDir should be followed by a folder name" << std::endl;
         return 1;
       } else {
         submitDir = options.at(iArg+1);
         iArg += 2;
       }

    } else if (options.at(iArg).compare("-configName") == 0) {
       char tmpChar = options.at(iArg+1)[0];
       if (iArg+1 == argc || tmpChar == '-' ) {
         std::cout << " -configName should be followed by a config file" << std::endl;
         return 1;
       } else {
         configName = options.at(iArg+1);
         iArg += 2;
       }

    } else if (options.at(iArg).compare("-production") == 0) {
      char tmpChar = options.at(iArg+1)[0];
      if (iArg+1 == argc-1 || tmpChar == '-' ) {
	std::cout << " -production should be followed by the production role (i.e. phys-exotics)" << std::endl;
	return 1;
      } else {
	productionName = options.at(iArg+1);
	iArg += 2;
      }

    } else if (options.at(iArg).compare("-syst") == 0) {
       char tmpChar = options.at(iArg+1)[0];
       if (iArg+1 == argc || iArg+2 == argc || tmpChar == '-' ) {
         std::cout << " -syst should be followed by a systematic string and an integer" << std::endl;
         return 1;
       } else {
         std::stringstream ss;
         systName = options.at(iArg+1);
         ss << options.at(iArg+2);
         ss >> systVal;
         ss.str("");
         if(systVal == 0 && options.at(iArg+2)[0] != '0'){
           std::cout << " -inFile should be followed by a systematic string and an INTEGER" << std::endl;
           return 1;
         }
         iArg += 3;
       }

    }else{
      std::cout << "Couldn't understand argument " << options.at(iArg) << std::endl;
      return 1;
    }

  }//while arguments

  if (productionName.size() > 0)
    f_production = true;

  //if grid job

  // Set up the job for xAOD access:
  xAOD::Init().ignore();


  // Construct the samples to run on:
  SH::SampleHandler sh;
  std::string containerName;
  std::string userName = getlogin();

  if (f_production)
    userName = productionName;

  //Check if input is a directory or a file
  struct stat buf;
  stat(samplePath.c_str(), &buf);
  if( samplePath.substr(0, 4).find("eos") != std::string::npos){
    SH::DiskListEOS list(samplePath.c_str());
    std::cout << "Running on EOS directory " << samplePath << " with tag " << inputTag << std::endl;
    if (inputTag.size() > 0){
      SH::scanDir (sh, list, inputTag); //Run on all files within dir containing inputTag
    }else{
      SH::scanDir (sh, list); //Run on all files within dir
    }
  }else if( S_ISDIR(buf.st_mode) ){ //if it is a local directory
    SH::DiskListLocal list (samplePath);
    SH::scanDir (sh, list); // Run on all files within dir
    std::cout << "Running Locally on directory  " << samplePath << std::endl;

  } else {  //if it is a file
    if( samplePath.substr( samplePath.size()-4 ).find(".txt") != std::string::npos){ //It is a text file of samples
      if( samplePath.find("grid") != std::string::npos ) //It is samples for the grid
        f_grid = true;

      std::ifstream inFile( samplePath );
      while(std::getline(inFile, containerName) ){
        if (containerName.size() > 1 && containerName.find("#") != 0 ){
          std::cout << "Adding container " << containerName << std::endl;
          if(f_grid){
            SH::scanDQ2( sh, containerName);
            //Add output container name to file of containers
            //follows grid format: "user."+userName+".%in:name[1]%.%in:name[2]%.%in:name[3]%"+outputTag
            int namePosition = 0;
            namePosition = containerName.find_first_of(".", namePosition)+1;
            namePosition = containerName.find_first_of(".", namePosition)+1;
            namePosition = containerName.find_first_of(".", namePosition)+1;
            outputContainerNames.push_back( ("user."+userName+"."+containerName.substr(0, namePosition)+outputTag+"/") );
          }else{
            //Get full path of file
            char fullPath[300];
            realpath( containerName.c_str(), fullPath );
	    std::string thisPath = fullPath;
            //split into fileName and directory two levels above file
	    std::string fileName = thisPath.substr(containerName.find_last_of("/")+1);
            thisPath = thisPath.substr(0, thisPath.find_last_of("/"));
            thisPath = thisPath.substr(0, thisPath.find_last_of("/"));
            std::cout << "path and filename are " << thisPath << " and " << fileName << std::endl;

            SH::DiskListLocal list (thisPath);
            //SH::SampleHandler sh_tmp;
            //SH::scanDir (sh_tmp, list);
            //sh.add( sh_tmp.findByName, ("*"+fileName).c_str() );
            SH::scanDir (sh, list, fileName); // specifying one particular file for testing
          }
        }
      }

    }else{ //It is a single root file to run on
    //Get full path of file
    char fullPath[300];
    realpath( samplePath.c_str(), fullPath );
    std::string thisPath = fullPath;
    //split into fileName and directory two levels above file
    std::string fileName = thisPath.substr(thisPath.find_last_of("/")+1);
    thisPath = thisPath.substr(0, thisPath.find_last_of("/"));
    thisPath = thisPath.substr(0, thisPath.find_last_of("/"));

    std::cout << "path and file " << thisPath << " and " << fileName << std::endl;
    SH::DiskListLocal list (thisPath);
    SH::scanDir (sh, list, fileName); // specifying one particular file for testing

    }
  }//it's a file

  ///////// Set output container name //////////////
  if( outputTag.size() > 0)
    outputTag = "."+outputTag+"/";
  else
    outputTag = "/";

  if(f_grid){
    if (f_production){
      outputName = "group."+userName+".%in:name[1]%.%in:name[2]%"+outputTag;
      //outputName = "group."+userName+".%in:name[1]%.%in:name[2]%.%in:name[3]%"+outputTag;
    }else
      outputName = "user."+userName+".%in:name[3]%.%in:name[4]%.%in:name[5]%"+outputTag;
  }else
    outputName = "%in:name%"+outputTag;

  std::string replaceStr = "Charybdis2Pythia8EvtGen_A14CTEQ6L1_BH2_n6";
  size_t index = outputName.find(replaceStr);
  if (index != std::string::npos){
    /* Make the replacement. */
    std::cout << "Changing output Name from " << outputName;
    outputName.replace(index, replaceStr.length(), "BH_n6");
    std::cout << " to " << outputName;
    std::cout << std::endl;
  }else{
    std::cout << "No Match " << outputName << std::endl;
  }


  // Set the name of the input TTree. It's always "CollectionTree" for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );
  sh.setMetaString( "nc_excludeSite", "ANALY_RAL_SL6");
  sh.setMetaString( "nc_grid_filter", "*");
  sh.print();

  job.sampleHandler( sh );
  // To automatically delete submitDir
  job.options()->setDouble(EL::Job::optRemoveSubmitDir, 1);

  //
  //  Set the number of events
  //
  TEnv* config = new TEnv(gSystem->ExpandPathName( configName.c_str() ));
  int nEvents = config->GetValue("MaxEvent",       -1);
  if(nEvents > 0)
    job.options()->setDouble(EL::Job::optMaxEvents, nEvents);



  return 0;
}

void submitJob(EL::Job& job, std::string outputName, std::string submitDir, bool f_grid, bool f_production, std::vector<std::string> outputContainerNames, bool f_condor = false){

  if(!f_grid && !f_condor){
    // Run the job using the local/direct driver:
    EL::DirectDriver driver;
    driver.options()->setString("nc_outputSampleName", outputName);
    driver.submit( job, submitDir );
  }else if(f_condor){
    //std::cout << "Submitting in condor"<<std::endl;
    //EL::CondorDriver driver;
    //driver.options()->setBool(EL::Job::optBatchSharedFileSystem, false);
    //driver.submit(job, submitDir);
  }else{
    EL::PrunDriver driver;
    driver.options()->setString("nc_outputSampleName", outputName);
    driver.options()->setString(EL::Job::optGridMergeOutput, "true");

    if (f_production)
      driver.options()->setString(EL::Job::optSubmitFlags, "--official");


    //driver.options()->setDouble(EL::Job::optGridNFilesPerJob, 2);
    //driver.options()->setString(EL::Job::optRootVer, "5.34.25");
    //driver.options()->setString(EL::Job::optCmtConfig, "x86_64-slc6-gcc48-opt");
    ////driver.options()->setDouble("nc_nGBPerJob", 1);
    //driver.options()->setString(EL::Job::optGridMergeOutput, "false");
    //driver.options()->setDouble(EL::Job::optGridMemory,10240); // 10 GB

    //driver.submit(job, submitDir); // with monitoring
    driver.submitOnly(job, submitDir); //without monitoring
  }

  ///// For grid, save list of ouput containers to the submission directory /////
  std::ofstream fileList((submitDir+"/outputContainers.txt"), std::ios_base::out);
  for( unsigned int iCont=0; iCont < outputContainerNames.size(); ++iCont){
    fileList << outputContainerNames.at(iCont)+"\n";
  }
  fileList.close();

  return;
}
