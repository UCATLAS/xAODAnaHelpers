Brian Tuan's Tutorial
=====================

Introduction
------------

This tutorial will introduce a new user to analysis of xAOD's on Tier 3 using the RootCore framework. We will first produce a slimmed tree from Monte Carlo on the GRID, then we will retrieve the file to the local disk and produce a weighted histogram of pT from the slimmed tree. The `full source code of the tutorial is here <http://faxbox.usatlas.org/user/btuan/Tutorial-00-00-02.zip>`_, for those looking to get a quick start.

Setup
-----

.. highlight:: none

First, connect to the UC Tier 3 server with X-11 window forwarding
enabled (so that you may use ROOT interactively later)::

    ssh -Y btuan@uct3.uchicago.edu

Make sure that the following lines are in your ~/.bash\_profile file, which is a script that runs each time you log into the shell. These lines set up the ATLAS software environment where RootCore, among other tools, is located and depends upon::

    # Setup ATLAS Environment
    export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
    export ALRB_localConfigDir=$HOME/localConfig
    source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh

Now, setup the RootCore environment and build it. We will use 2.3.21, which, at time of writing is the latest supported release by xAODAnaHelpers. You'll need to perform this step each time you login to the shell::

    cd ~/work
    rcSetup Base,2.3.21

To see other available versions of RootCore, type::

    rcSetup -r

Make a work directory and checkout the xAODAnaHelpers package. This package contains a few useful tools for any analysis: e.g. event selection, jet cleaning, jet calibration etc. After checking out all the packages, return to the directory that has the "RootCore" folder -- probably your home directory -- and recompile, just to double check that everything works. This may take a while, be patient.

In general, always check to make sure that your code compiles properly after any changes with ``rc compile``. Any time package dependencies are updated, be sure to run ``rc find_packages`` as well. In addition, if compilation for one of your packages fails, and all code errors seem to be fixed, but RootCore still refuses to compile your package, try running ``rc clean`` then ``rc find_packages && rc compile``. This will clean out all of the old files that may have been improperly compiled.

It is good practice to repeat this procedure any time you change versions of any packages, RootCore included (though recompiling everything will of course take a while)::

    mkdir work && cd work
    git clone https://github.com/UCATLAS/xAODAnaHelpers.git
    python xAODAnaHelpers/scripts/checkoutASGtags.py 2.3.21
    rc find_packages && rc compile

RootCore comes with a script that allows us to easily create a skeleton for your analysis. Do so::

    . $ROOTCOREDIR/scripts/make_skeleton.sh Tutorial

Make a directory called run. This is where your script will be located::

    cd work/Tutorial && mkdir run && cd run
    touch makeSlimmedTree.cxx

Code and Data
-------------

.. highlight:: c++

.. warning:: setConfig and getConfig are eliminated as ``ROOT::TEnv`` support is now deprecated.

Place the following code in your **``makeSlimmedTree.cxx``**. Skim through it to familiarize yourself with the sequence of the analysis::

    void makeSlimmedTree (const std::string& submitDir)
    {
        //===========================================
        // FOR ROOT6 WE DO NOT PUT THIS LINE
        // (ROOT6 uses Cling instead of CINT)
        // Load the libraries for all packages
        // gROOT->Macro("$ROOTCOREDIR/scripts/load_packages.C");
        // Instead on command line do:
        // > root -l -b -q '$ROOTCOREDIR/scripts/load_packages.C' 'makeSlimmedTree.cxx ("submitDir")'
        // The above works for ROOT6 and ROOT5
        //==========================================

        bool f_grid = false;    // decide if we use the GRID to run our analysis. default false.

        // Set up the job for xAOD access:
        xAOD::Init().ignore();


        /*#####################################################
        ############# SampleHandler Configuation ##############
        #####################################################*/

        // create a new sample handler to describe the data files we use.
        SH::SampleHandler sh;

        // Use only one of the following three methods to scan for files with SampleHandler

        // (1) use SampleHandler with DQ2 to obtain the desired dataset
        // SH::scanDQ2 (sh, "data15_13TeV.00267638.physics_Main.merge.AOD.r6818_p2358/");

        // (2) use SampleHandler with a dataset list to obtain the desired dataset
        const std::string inputFilePath = gSystem->ExpandPathName("$ROOTCOREBIN/data/Tutorial/inDSShort.txt");
        SH::readFileList (sh, "sample", inputFilePath );

        // (3) use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
        // const char* inputFilePath = gSystem->ExpandPathName ("/export/t3data3/fizisist/");
        // SH::DiskListLocal list (inputFilePath);
        // SH::scanDir(sh, list);

        // set the name of the tree in our files. in the xAOD the TTree containing the EDM containers is "CollectionTree"
        sh.setMetaString ("nc_tree", "CollectionTree");
        sh.setMetaString("nc_grid_filter", "*");    //Data files on grid to not end in .root

        // print out the samples we found
        sh.print ();


        /*#####################################################
        ################## Job Configuration ##################
        #####################################################*/

        // this is the basic description of our job
        EL::Job job;
        job.sampleHandler (sh); // use SampleHandler in this job

        // job.options()->setDouble (EL::Job::optMaxEvents, 5000); // for testing purposes, limit to run over the first 500 events only!

        // To automatically delete submitDir
        job.options()->setDouble(EL::Job::optRemoveSubmitDir, 1);

        // For Trigger
        job.options()->setString( EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_branch );

        // Use TTreeCache to precache data files to speed up analysis
        job.options()->setDouble (EL::Job::optCacheSize, 10*1024*1024);
        job.options()->setDouble (EL::Job::optCacheLearnEntries, 20);


     /*#####################################################
        ############### Output Configuration #################
        #####################################################*/

        std::string outputName;
        std::string userName = "btuan";

        // if running on GRID, make sure no two runs have the same output name. tag as necessary
        std::string outputTag = ".v1/";

        if(f_grid) // follow GRID naming conventions
            outputName = "user."+userName+".%in:name[1]%.%in:name[2]%.%in:name[3]%"+outputTag;
        else
            outputName = "%in:name%"+outputTag;


        /*#####################################################
        ############## Algorithm Configuration ################
        #####################################################*/

        // basic event selection : GRL, event cleaning, NPV
        BasicEventSelection* baseEventSel = new BasicEventSelection();
        baseEventSel->setName("baseEventSel")->setConfig( "$ROOTCOREBIN/data/Tutorial/baseEventSel.config" );

        // jet calibrator
        std::string systName = "None";
        float systVal = 0;
        JetCalibrator* jetCalib = new JetCalibrator();
        jetCalib->setName( "jetCalib" )->setConfig( "$ROOTCOREBIN/data/Tutorial/jetCalib_AntiKt4EMTopo.config")->setSyst( systName, systVal );

        // jet selector
        JetSelector* jetSelect = new JetSelector();
        jetSelect->setName( "jetSelect" )->setConfig( "$ROOTCOREBIN/data/Tutorial/jetSelect.config" );

        // tree output
        TreeAlgo* outTree = new TreeAlgo();
        outTree->setName( "outTree" )->setConfig( "$ROOTCOREBIN/data/Tutorial/outTree.config" );

        // add algorithms to analysis
        job.algsAdd (baseEventSel);
        job.algsAdd (jetCalib);
        job.algsAdd (jetSelect);
        job.algsAdd (outTree);


        /*#####################################################
        ################$ Initialize Driver #####$$$###########
        #####################################################*/

        // here, we choose which driver to use with the boolean set earlier
        if (f_grid){ // run using the GRID driver
            EL::PrunDriver driver;

            driver.options()->setString("nc_outputSampleName", outputName);
            driver.options()->setDouble(EL::Job::optGridNFilesPerJob, 2);
            // driver.options()->setDouble(EL::Job::optGridMemory, 10240); //10 GB

            driver.submitOnly(job, submitDir); // submitOnly runs job without opening monitoring loop
        }
        else { // run using a direct driver
            EL::DirectDriver driver;
            driver.options()->setString("nc_outputSampleName", outputName);
            driver.submit (job, submitDir);
        }

    }

.. highlight:: none

Update the package dependencies on the line **``PACKAGE_DEP``** in ``cmt/Makefile.RootCore`` to include xAODAnaHelpers::

    PACKAGE_DEP = xAODAnaHelpers

Later on, in more driven analyses, you may find yourself adding the EventLoop and EventLoopGrid packages to the dependencies. The xAODAnaHelpers package takes care of all of the event looping for you in this case, so the only dependency is upon that package.

Since we use the DQ2 SampleHandler to obtain the datasets, you will need to set up a valid VOMS proxy (which you will need anyways to submit the job to the grid) and a DQ2 client if you want to run the job locally.  You can also use the XRootD protocol with FAX to obtain the samples. The code for this is commented out in the **``makeSlimmedTree.cxx``** code.  The gist of this is the following (on the command line)::

    voms-proxy-init -voms altas
    localSetupFAX
    fax-get-gLFNs data15_13TeV.00267638.physics_Main.merge.AOD.r6818_p2358 > inDS.txt
    localSetupPandaClient

Make a directory **``Tutorial/data``**. This will be where we put all of the data and configuration files for our package, and for xAODAnaHelpers. Once you run find\_packages and compile with RootCore, you will be able to refer to this data directory with the **``$ROOTCOREBIN``** path variable, which is particularly useful when you have to generalize your code to run on batch machines, grid, etc::

    mkdir ~/work/Tutorial/data/
    mv inDS.txt ~/work/Tutorial/data/

Configuration of xAODAnaHelpers Algorithms
------------------------------------------

As mentioned earlier, xAODAnaHelpers provides a series of algorithms that are chained in sequence to provide the desired output. The input and output containers for each of the algorithms in sequence are configured by .config files -- one for each algorithm. Create the following configuration files (as set in the ROOT macro in the run directory) in the data directory::

    touch ~/work/Tutorial/data/baseEventSel.config
    touch ~/work/Tutorial/data/jetCalib_AntiKt4EMTopo.config
    touch ~/work/Tutorial/data/jetSelect.config
    touch ~/work/Tutorial/data/outTree.config

Each of these configuration files will set the options for a separate part of the analysis. Include the following in each file. At present, there is no centralized documentation for all of xAODAnaHelpers -- there is some on the GitHub wiki -- but to view the availability of configuration options for each xAODAnaHelpers algorithm, view the header file and source code.

baseEventSel.config::

        Debug                     False
        ApplyGRL                  False
        GRL                       $ROOTCOREBIN/data/Tutorial/data15_13TeV.periodAllYear_DetStatus-v63-pro18-01_DQDefects-00-01-02_PHYS_StandardGRL_All_Good.xml
        DoPileupReweighting       False
        VertexContainer           PrimaryVertices
        NTrackForPrimaryVertex    2
        TruthLevelOnly            False
        #Trigger                   L1_RD0_FILLED
        #Trigger                   L1_.*
        #Trigger                   L1_MBTS_1_1
        #Trigger                   .*
        Trigger                   .*_MBTS_1_1|.*_RD0_FILLED|L1_J[0-9]*|HLT_j[0-9]*|HLT_noalg_j[0-9]*|L1_XE[0-9]*|HLT_XE[0-9]*|HLT_noalg_XE[0-9]*
        StoreTrigDecision         True
        CutOnTrigger              False
        StorePassAny              True
        StorePassL1               True
        StorePassHLT              True
        StoreTrigKeys             True
        UseMetaData               False
        ## last option must be followed by a new line ##

jetCalib\_AntiKt4EMTopo.config::

        Debug             False
        InputContainer    AntiKt4EMTopoJets
        JetAlgorithm      AntiKt4EMTopo
        #
        SaveAllCleanDecisions True
        #
        OutputContainer   Jets_Calib
        OutputAlgo        Jets_Calib_Algo
        configNameAFII    JES_Full2012dataset_AFII_January2014.config
        configNameFullSim JES_MC15Prerecommendation_April2015.config
        configNameData    JES_MC15Prerecommendation_April2015.config
        #configNameData    JES_Full2012dataset_May2014.config
        CalibSequence     JetArea_Residual_Origin_EtaJES_GSC
        #
        ## last option must be followed by a new line ##

jetSelect.config::

        Debug                   False
        InputContainer          Jets_Calib
        InputAlgo               Jets_Calib_Algo
        OutputContainer         SignalJets
        OutputAlgo              SignalJets_Algo
        DecorateSelectedObjects False
        CreateSelectedContainer True
        # save multiple cleaning decisions instead of applying the cleaning
        CleanJets               False
        #
        pTMin                   20e3
        PassMin                 1
        Sort                    True
        UseCutFlow              True
        # pT cut is > JVF recommended pT cut - to be added ... or JVT?
        DoJVF                                     False
        pTMaxJVF                        50e3
        etaMaxJVF                       2.4
        JVFCut                                  0.5
        ## last option must be followed by a new line ##

outTree.config::

        Debug                               False
        EventDetailStr              "pileup"
        TrigDetailStr               True
        JetDetailStr                "kinematic substructure rapidity energy scales truth LeadingJets"
        #JetDetailStr               "kinematic"
        JetContainerName    SignalJets
        SameHistsOutDir             False
        ## last option must be followed by a new line ##

Almost there! All that's left to do is copy the requisite files into the locations specified by our makeSlimmedTrees.cxx script.

The atlasstyle package is located here. Download and unzip the package, then place it in the run/ directory. Full support for `ATLAS Style <https://twiki.cern.ch/twiki/pub/AtlasProtected/PubComTemplates/atlasstyle-00-03-05.tar.gz>`_ will be incorporated soon.

Copy the desired GRL to the data/ folder. The Good Runs List is used to specify which events will be kept and which events will be discarded, based on LHC and ATLAS operations (e.g. bad luminosity block, etc.). The minutiae are located `here <https://twiki.cern.ch/twiki/bin/view/AtlasProtected/GoodRunListsForAnalysisRun2>`_.

.. note::
    Always use the most updated GRL, and use the same GRL for your luminosity calculations as you do your event selections.  This tutorial uses the following `GRL <https://atlasdqm.web.cern.ch/atlasdqm/grlgen/All_Good/data15_13TeV.periodAllYear_DetStatus-v63-pro18-01_DQDefects-00-01-02_PHYS_StandardGRL_All_Good.xml>`_.

Plotting
--------

.. highlight:: c++

Here is a "quick and dirty" plotting macro to be placed in the **``run/``** folder for a plot. An example better integrating AtlasStyle is in the works and should be updated soon::

    /**
        * Plotter.cxx -- simple plotter for slimmed trees
        *
        * @author   Brian Tuan
        * @contact  brian.tuan@cern.ch
        * @date     21 July 2015
        *
        * Run on the command line by:
        *   root -l '$ROOTCOREDIR/scripts/load_packages.C' '$ROOTCOREBIN/data/Tutorial/atlasstyle/AtlasStyle.C' 'Plotter.cxx( filePath )'
        *           If no argument indicated, Plotter will default to $PWD/submitDir/data-tree/sample.root
        **/

        #include "atlasstyle/AtlasUtils.h"
        #include "atlasstyle/AtlasLabels.h"
        #include "atlasstyle/AtlasStyle.h"

        #include "atlasstyle/AtlasUtils.C"
        #include "atlasstyle/AtlasLabels.C"

        #include "TCanvas.h"
        #include "TFile.h"
        #include "TROOT.h"
        #include "TH1F.h"
        #include "TRandom.h"
        #include "TGraphErrors.h"

        void Plotter (const std::string filePath = "submitDir/data-tree/sample.root"){

            SetAtlasStyle();

            // TFile* f_input = new TFile(filePath.c_str(), "READ", "file", 1);
            TFile* f_input = new TFile("/afs/cern.ch/user/b/btuan/work/Tutorial/run/submitDir/data-tree/sample.root", "READ", "file", 1);
            if( !f_input ){ std::cout<<"File not found! Exiting..."<<std::endl; return; }

            TTree* t_tree = (TTree*)f_input->Get("outTree"); // argument must be exact name of tree

            // Create a TTreeReader named "MyTree" from the given TDirectory.
            // The TTreeReader gives access to the TTree to the TTreeReaderValue and
            // TTreeReaderArray objects. It knows the current entry number and knows
            // how to iterate through the TTree.
            TTreeReader reader("outTree", f_input);

            // Read a single float value in each tree entry:
            TTreeReaderValue<int> evNum(reader, "eventNumber");
            TTreeReaderValue<float> weight(reader, "mcEventWeight"); // weight defaults to 1 if data

            // Read a vector from in each of the tree entries:
            TTreeReaderValue<std::vector<float>> jetPt(reader, "jet_pt");
            TTreeReaderValue<std::vector<float>> jetEMPt(reader, "jet_emScalePt");
            TTreeReaderValue<std::vector<float>> jetPUPt(reader, "jet_pileupScalePt");
            TTreeReaderValue<std::vector<float>> jetPhi(reader, "jet_phi");
            TTreeReaderValue<std::vector<float>> jetEta(reader, "jet_eta");
            TTreeReaderValue<std::vector<float>> jetWidth(reader, "jet_Width");

            // Now iterate through the TTree entries and fill a histogram.
            TH1F* h_jetPt = new TH1F("h_jetPt", "pt", 100, 0., 250.);
            h_jetPt->SetTitle("AntiKt4 Pt");
            h_jetPt->SetXTitle("Pt (GeV)");
            h_jetPt->SetYTitle("nEvents");

            while( reader.Next() ) { // dummy iterator just to keep count!
                if (reader.GetEntryStatus() != TTreeReader::kEntryValid ){
                    switch (reader.GetEntryStatus()) {
                        case TTreeReader::kEntryValid:
                            // All good! Nothing to worry about.
                            break;
                        case TTreeReader::kEntryNotLoaded:
                            std::cerr << "Error: TTreeReader has not loaded any data yet!\n";
                            break;
                        case TTreeReader::kEntryNoTree:
                            std::cerr << "Error: TTreeReader cannot find a tree named \"outTree\"!\n";
                            break;
                        case TTreeReader::kEntryNotFound:
                            // Can't really happen as TTreeReader::Next() knows when to stop.
                            std::cerr << "Error: The entry number doe not exist\n";
                            break;
                        case TTreeReader::kEntryChainSetupError:
                            std::cerr << "Error: TTreeReader cannot access a chain element, e.g. file without the tree\n";
                            break;
                        case TTreeReader::kEntryChainFileError:
                            std::cerr << "Error: TTreeReader cannot open a chain element, e.g. missing file\n";
                            break;
                        case TTreeReader::kEntryDictionaryError:
                            std::cerr << "Error: TTreeReader cannot find the dictionary for some data\n";
                            break;
                    }
                    return false;
                }

                // Access the jetPt as an array, whether the TTree stores this as
                // a std::vector, std::list, TClonesArray or Jet* C-style array, with
                // fixed or variable array size.
                if ((*jetPt).size() < 2 || (*jetPt)[0] < 100) //at least two jets, leading jet > 100 GeV
                    continue;

                // Access the array of taus.
                float currentWeight = *weight;
                for (int iJets = 0, nJets = (*jetPt).size(); iJets < nJets; ++iJets)
                    h_jetPt->Fill( (*jetPt)[iJets] , currentWeight);
            }


            TCanvas* c1 = new TCanvas("c1","AntiKt4EMTopoJets pT",50,50,600,600);
            TPad* thePad = (TPad*)c1->cd();

            myText(       0.3,  0.85, 1, "#sqrt{s}= 14 TeV");
            myText(       0.57, 0.85, 1, "|#eta_{jet}|<0.5");
            myMarkerText( 0.55, 0.75, 1, 20, "Data 2009",1.3);
            myBoxText(    0.55, 0.67, 0.05, 5, "NLO QCD");

            ATLASLabel(0.2,0.2,"Preliminary");

            h_jetPt->Draw();

            c1->Print("Output.eps");
            c1->Print("Output.png");
            c1->Print("Output.pdf");
        }

Tips & Tricks
-------------

Here are a few tips and tricks that should help you avoid most errors, and prove as good practice for any analysis with AnaHelpers.

Maintaining xAODAnaHelpers
~~~~~~~~~~~~~~~~~~~~~~~~~~

-  xAODAnaHelpers is now hosted on `GitHub <https://github.com/UCATLAS/xAODAnaHelpers>`__! This means two things: first, there is a basic documentation available (:ref:`MainPage`) as reference. The documentation is still in progress, but what's already there should help you figure out what's going on with the package. Second, the development page (:ref:`Versions`) will contain information about the latest analysis base release that xAH has been tested to be compatible with.

-  Should you find any errors with xAODAnaHelpers code -- which should be a very rare occurrence, but programmers are still human -- you can immediately report the issue to the entire xAH team in `GitHub issues <https://github.com/UCATLAS/xAODAnaHelpers/issues>`_. Issues are tracked publicly, so you can stay posted about the resolution of your issue.

-  Updating the framework should be as simple as calling ``git pull !https://github.com/xAODAnaHelpers`` from within the ``xAODAnaHelpers`` directory. Then, to make sure all the latest Good Runs Lists (GRLs) and configuration information are updated as well, run ``python xAODAnaHelpers/scripts/checkoutASGtags.py $ABver`` where $ABver is the version of your analysis base release, in this case ``2.3.21``. The following lines of code should accomplish the same result automatically:

.. code-block:: none

    if [ -d $ROOTCOREBIN/../xAODAnaHelpers ]
        then cd $ROOTCOREBIN/../ python xAODAnaHelpers/scripts/checkoutASGtags.py $(echo $ROOTCOREDIR \| sed 's/\\/cvmfs\\/atlas\\.cern\\.ch\\/repo\\/sw\\/ASG\\/AnalysisBase\\/\\([0-9]\*[.][0-9]\*[.][0-9]\*\\).\*/\\1 /');
    fi

This framework will automatically scale everything in to the GeV range for you, but the xAOD format lists all energies in MeV.

Monitoring loop with pbook show() retry() kill() bigpanda / loadpackages::

  EL::Driver::wait()

Debug True gives a verbose mode.
