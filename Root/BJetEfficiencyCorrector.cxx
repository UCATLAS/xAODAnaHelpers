/******************************************
 *
 * Interface to CP BJet Efficiency Correction Tool.
 *
 * John Alison (john.alison@cern.ch)
 *
 *
 ******************************************/

// c++ include(s):
#include <iostream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/BJetEfficiencyCorrector.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(BJetEfficiencyCorrector)


BJetEfficiencyCorrector :: BJetEfficiencyCorrector () {
}

BJetEfficiencyCorrector :: BJetEfficiencyCorrector (std::string name, std::string configName ) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_BJetEffSFTool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("BJetEfficiencyCorrector()", "Calling constructor");

}


EL::StatusCode  BJetEfficiencyCorrector :: configure ()
{
  if(!m_configName.empty()){
    Info("configure()", "Configuing BJetEfficiencyCorrector Interface. User configuration read from : %s ", m_configName.c_str());

    m_configName = gSystem->ExpandPathName( m_configName.c_str() );
    RETURN_CHECK_CONFIG( "BJetEfficiencyCorrector::configure()", m_configName);

    TEnv* config = new TEnv(m_configName.c_str());

    // read debug flag from .config file
    m_debug                   = config->GetValue("Debug" , false );
    // input container to be read from TEvent or TStore
    m_inContainerName         = config->GetValue("InputContainer",  "");
    m_outContainerName        = config->GetValue("OutputContainer", "");
    m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

    // Systematics stuff
    m_runAllSyst              = config->GetValue("RunAllSyst" ,      false ); // default: false
    m_systName		          = config->GetValue("SystName" ,       "" );      // default: no syst
    m_outputSystName          = config->GetValue("OutputSystName",  "BJetEfficiency_Algo");

    // file(s) containing corrections
    m_corrFileName           = config->GetValue("CorrectionFileName", "2014-Winter-8TeV-MC12-CDI.root" );
    m_jetAuthor              = config->GetValue("JetAuthor",          "AntiKt4TopoEMJVF0_5" );
    m_taggerName             = config->GetValue("TaggerName",         "MV1" );
    m_useDevelopmentFile     = config->GetValue("UseDevelopmentFile", true );
    m_coneFlavourLabel       = config->GetValue("ConeFlavourLabel",   true );
    // Btag quality
    m_btag_veryloose          = config->GetValue("BTagVeryLoose",   false);
    m_btag_loose              = config->GetValue("BTagLoose",       false);
    m_btag_medium             = config->GetValue("BTagMedium",      false);
    m_btag_tight              = config->GetValue("BTagTight",       false);

    m_decor                   = config->GetValue("DecorationName", "BTag_SF");

    config->Print();
    Info("configure()", "BJetEfficiencyCorrector Interface succesfully configured! ");

    delete config;
  }

  m_isEMjet = m_inContainerName.find("EMTopoJets") != std::string::npos;
  m_isLCjet = m_inContainerName.find("LCTopoJets") != std::string::npos;

  if( m_isEMjet ) {
    if( m_btag_veryloose ) { m_operatingPt = "0_1644"; m_decor += "_BTagVeryLoose";  }
    if( m_btag_loose     ) { m_operatingPt = "0_3900"; m_decor += "_BTagLoose";      }
    if( m_btag_medium    ) { m_operatingPt = "0_8119"; m_decor += "_BTagMedium";     }
    if( m_btag_tight     ) { m_operatingPt = "0_9867"; m_decor += "_BTagTight";      }


  } else if ( m_isLCjet ) {
    if( m_btag_veryloose ) { m_operatingPt = "0_1340"; m_decor += "_BTagVeryLoose";  }
    if( m_btag_loose     ) { m_operatingPt = "0_3511"; m_decor += "_BTagLoose";      }
    if( m_btag_medium    ) { m_operatingPt = "0_7892"; m_decor += "_BTagMedium";     }
    if( m_btag_tight     ) { m_operatingPt = "0_9827"; m_decor += "_BTagTight";      }
  }

  if( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetEfficiencyCorrector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "BJetEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing BJetEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  // initialize the BJetEfficiencyCorrectionTool
  std::string sf_tool_name = std::string("BJetEfficiencyCorrectionTool_") + m_name;
  m_BJetEffSFTool = new BTaggingEfficiencyTool( "BTagTest" );
  m_BJetEffSFTool->msg().setLevel( MSG::INFO ); // DEBUG, VERBOSE, INFO, ERROR

  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("TaggerName",          m_taggerName),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("OperatingPoint",      m_operatingPt),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("JetAuthor",           m_jetAuthor),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("ScaleFactorFileName", m_corrFileName),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("UseDevelopmentFile",  m_useDevelopmentFile), "Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("ConeFlavourLabel",    m_coneFlavourLabel), "Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->initialize(), "Failed to properly initialize the BJetEfficiencyCorrectionTool");

  std::cout << "-----------------------------------------------------" << std::endl;
  const std::map<CP::SystematicVariation, std::vector<std::string> > allowed_variations = m_BJetEffSFTool->listSystematics();
  std::cout << "Allowed systematics variations for tool " << m_BJetEffSFTool->name() << ":" << std::endl;
  for (auto var : allowed_variations) {
    std::cout << std::setw(40) << std::left << var.first.name() << ":";
    for (auto flv : var.second) std::cout << " " << flv;
    std::cout << std::endl;
  }
  std::cout << "-----------------------------------------------------" << std::endl;


  // Get a list of affecting systematics
  CP::SystematicSet affectSysts = m_BJetEffSFTool->affectingSystematics();
  // Convert into a simple list
  std::vector<CP::SystematicSet> affectSystList = CP::make_systematics_vector(affectSysts);
  for ( const auto& syst_it : affectSystList ){
      Info("initialize()"," tool can be affected by systematic: %s", (syst_it.name()).c_str());
  }

  // Get a list of recommended systematics
  CP::SystematicSet recSysts = m_BJetEffSFTool->recommendedSystematics();
  // Convert into a simple list
  m_systList = CP::make_systematics_vector(recSysts);
  for ( const auto& syst_it : m_systList ){
      Info("initialize()"," available recommended systematic: %s", (syst_it.name()).c_str());
  }

  if( m_systName.empty() && !m_runAllSyst ){
    Info("initialize()"," Running w/ nominal configuration!");
  }

  Info("initialize()", "BJetEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetEfficiencyCorrector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying BJet Efficiency Correction... ");

  // get the collection from TEvent or TStore
  const xAOD::JetContainer* correctedJets(nullptr);
  RETURN_CHECK("BJetEfficiencyCorrector::execute()", HelperFunctions::retrieve(correctedJets, m_inContainerName, m_event, m_store, m_debug) ,"");

  // create ConstDataVector to be eventually stored in TStore
  ConstDataVector<xAOD::JetContainer>* correctedJetsCDV = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  correctedJetsCDV->reserve( correctedJets->size() );

  if(m_debug){
    unsigned int idx(0);
    for( auto jet_itr : *(correctedJets) ) {
      Info( "execute", "Input Jet %i, pt = %.2f GeV ", idx, (jet_itr->pt() * 1e-3) );
      ++idx;
    }
  }

  //
  // loop over available systematics
  //
  std::vector< std::string >* sysVariationNames = new std::vector< std::string >;
  for(const auto& syst_it : m_systList){

    //
    // if not running systematics, only compulte weight for specified systematic (m_systName)
    //    default is nominal (i.e., "")
    //
    if(!m_runAllSyst){
      if( syst_it.name() != m_systName ) {
	if(m_debug) Info("execute()","Not running systematics only apply nominal SF");
	continue;
      }
    }

    //
    // Creat the name of the weight
    //   template:  SYSNAME_BTag_SF
    //
    std::string sfName = m_decor;
    if( !syst_it.name().empty() ){
       std::string prepend = syst_it.name() + "_";
       sfName.insert( 0, prepend );
    }
    if(m_debug) Info("execute()", "SF decoration name is: %s", m_decor.c_str());
    sysVariationNames->push_back(sfName);

    //
    // configure tool with syst variation
    //
    if (m_BJetEffSFTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok) {
      Error("initialize()", "Failed to configure BJetEfficiencyCorrections for systematic %s", syst_it.name().c_str());
      return EL::StatusCode::FAILURE;
    }
    if(m_debug) Info("execute()", "Successfully applied systematic: %s", syst_it.name().c_str());

    //
    // and now apply data-driven efficiency and efficiency SF!
    //
    for( auto jet_itr : *(correctedJets)) {

      //
      //  If btagging vector doesnt exist create it
      //
      SG::AuxElement::Decorator< std::vector<float> > sfVec( m_outputSystName );
      if(!sfVec.isAvailable(*jet_itr)){
	sfVec(*jet_itr) = std::vector<float>();
      }

      //
      // obtain efficiency SF
      //
      float SF(0.0);
      if( m_BJetEffSFTool->getScaleFactor( *jet_itr, SF ) != CP::CorrectionCode::Ok){
	Error( "execute()", "Problem in getEfficiencyScaleFactor");
	return EL::StatusCode::FAILURE;
      }
      if(m_debug) Info( "execute()", "\t efficiency SF = %g", SF );

      //
      // Add it to vector
      //
      sfVec(*jet_itr).push_back(SF);

      if(m_debug){
	//
	// directly obtain reco efficiency
	//
	float eff(0.0);
	if( m_BJetEffSFTool->getEfficiency( *jet_itr, eff ) != CP::CorrectionCode::Ok){
	  Error( "execute()", "Problem in getRecoEfficiency");
	  return EL::StatusCode::FAILURE;
	}
	Info( "execute()", "\t reco efficiency = %g", eff );
      }

      if(m_debug){
	Info( "execute", "===>>> Resulting SF (%s) (from tool) %f, (from object) %f",  sfName.c_str(), SF, sfVec(*jet_itr).back());
      }

    } // close jet loop

  } // close loop on systematics

  //
  // save pointers in ConstDataVector
  //
  RETURN_CHECK( "BJetEfficiencyCorrections::execute()", HelperFunctions::makeSubsetCont(correctedJets, correctedJetsCDV, "", ToolName::CORRECTOR), "");

  //
  // add container to TStore
  //
  RETURN_CHECK( "BJetEfficiencyCorrector::execute()", m_store->record( correctedJetsCDV,  m_outContainerName ), "Failed to store container.");

  //
  // add list of sys names to TStore
  //
  RETURN_CHECK( "BJetEfficiencyCorrector::execute()", m_store->record( sysVariationNames, m_outputSystName), "Failed to record vector of systematic names.");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetEfficiencyCorrector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  Info("finalize()", "Deleting tool instances...");

  if(m_BJetEffSFTool){
    delete m_BJetEffSFTool; m_BJetEffSFTool = nullptr;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.

  Info("histFinalize()", "Calling histFinalize");

  return EL::StatusCode::SUCCESS;
}

