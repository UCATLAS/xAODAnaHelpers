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

    //
    // read flags set from .config file
    //
    m_debug                   = config->GetValue("Debug" , false );
    m_inContainerName         = config->GetValue("InputContainer",  "");
    m_systName                = config->GetValue("SystName" ,       "" );      // default: no syst
    m_outputSystName          = config->GetValue("OutputSystName",  "BJetEfficiency_Algo");

    //
    // configuration of the bjet eff tool
    //
    m_corrFileName           = config->GetValue("CorrectionFileName", "2014-Winter-8TeV-MC12-CDI.root" );
    m_jetAuthor              = config->GetValue("JetAuthor",          "AntiKt4TopoEMJVF0_5" );
    m_taggerName             = config->GetValue("TaggerName",         "MV1" );
    m_useDevelopmentFile     = config->GetValue("UseDevelopmentFile", true );
    m_coneFlavourLabel       = config->GetValue("ConeFlavourLabel",   true );

    //
    // Btag quality
    //
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

  m_runAllSyst = (m_systName.find("All") != std::string::npos);

  if( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetEfficiencyCorrector :: setupJob (EL::Job& job)
{
  Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "BJetEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: histInitialize ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode BJetEfficiencyCorrector :: initialize ()
{
  Info("initialize()", "Initializing BJetEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  //
  // initialize the BJetEfficiencyCorrectionTool
  //
  std::string sf_tool_name = std::string("BJetEfficiencyCorrectionTool_") + m_name;
  m_BJetEffSFTool = new BTaggingEfficiencyTool( "BTagTest" );
  m_BJetEffSFTool->msg().setLevel( MSG::INFO ); // DEBUG, VERBOSE, INFO, ERROR

  //
  //  Configure the BJetEfficiencyCorrectionTool
  //
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("TaggerName",          m_taggerName),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("OperatingPoint",      m_operatingPt),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("JetAuthor",           m_jetAuthor),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("ScaleFactorFileName", m_corrFileName),"Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("UseDevelopmentFile",  m_useDevelopmentFile), "Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->setProperty("ConeFlavourLabel",    m_coneFlavourLabel), "Failed to set property");
  RETURN_CHECK( "BJetEfficiencyCorrector::initialize()", m_BJetEffSFTool->initialize(), "Failed to properly initialize the BJetEfficiencyCorrectionTool");

  //
  // Print out
  //
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

  if( m_systName.empty() ){
    Info("initialize()"," Running w/ nominal configuration!");
  }

  if( m_runAllSyst ){
    Info("initialize()"," Running w/ All systematics");
  }

  Info("initialize()", "BJetEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetEfficiencyCorrector :: execute ()
{
  if(m_debug) Info("execute()", "Applying BJet Efficiency Correction... ");

  // get the collection from TEvent or TStore
  const xAOD::JetContainer* correctedJets(nullptr);
  RETURN_CHECK("BJetEfficiencyCorrector::execute()", HelperFunctions::retrieve(correctedJets, m_inContainerName, m_event, m_store, m_debug) ,"");

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
    // Create the name of the weight
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
  // add list of sys names to TStore
  //
  RETURN_CHECK( "BJetEfficiencyCorrector::execute()", m_store->record( sysVariationNames, m_outputSystName), "Failed to record vector of systematic names.");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetEfficiencyCorrector :: postExecute ()
{
  if(m_debug) Info("postExecute()", "Calling postExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: finalize ()
{
  Info("finalize()", "Deleting tool instances...");
  if(m_BJetEffSFTool){
    delete m_BJetEffSFTool; m_BJetEffSFTool = nullptr;
  }
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: histFinalize ()
{
  Info("histFinalize()", "Calling histFinalize");
  return EL::StatusCode::SUCCESS;
}

