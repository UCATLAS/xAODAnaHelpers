/**************************************************
 *
 * Interface to CP BJet Efficiency Correction Tool.
 *
 * John Alison (john.alison@cern.ch)
 * Gabriel Facini (gabriel.facini@cern.ch)
 *
 *
 **************************************************/

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

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(BJetEfficiencyCorrector)


BJetEfficiencyCorrector :: BJetEfficiencyCorrector () :
    Algorithm("BJetEfficiencyCorrector")
{
}


EL::StatusCode BJetEfficiencyCorrector :: setupJob (EL::Job& job)
{
  ANA_MSG_INFO( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "BJetEfficiencyCorrector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: histInitialize ()
{
  ANA_CHECK( xAH::Algorithm::algInitialize());

  m_corrFileName = PathResolverFindCalibFile(m_corrFileName);

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
  ANA_MSG_INFO( "Initializing BJetEfficiencyCorrector Interface... ");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  m_decorSF = m_decor + "_SF";

  bool allOK(false);
  m_getScaleFactors = false;
  // not calibrated yet
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTagCalib2015#Pre_Recommendation_August_2015
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger_AntiKt4EMTopoJets
  if (m_operatingPt == "FixedCutBEff_30") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_50") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_80") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_90") { allOK = true; }
  // these are the only calibrated working points
  if (m_operatingPt == "FixedCutBEff_60") { allOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_70") { allOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_77") { allOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_85") { allOK = true; m_getScaleFactors =  true; }
  //
  if (m_operatingPt == "FlatBEff_30") { allOK = true; }
  if (m_operatingPt == "FlatBEff_40") { allOK = true; }
  if (m_operatingPt == "FlatBEff_50") { allOK = true; }
  if (m_operatingPt == "FlatBEff_60") { allOK = true; }
  if (m_operatingPt == "FlatBEff_70") { allOK = true; }
  if (m_operatingPt == "FlatBEff_77") { allOK = true; }
  if (m_operatingPt == "FlatBEff_85") { allOK = true; }

  if( !allOK ) {
    ANA_MSG_ERROR( "Requested operating point is not known to xAH. Arrow v Indian? " << m_operatingPt);
    return EL::StatusCode::FAILURE;
  }

  // make unique name
  m_decor           += "_" + m_operatingPt;
  m_decorSF         += "_" + m_operatingPt;
  m_outputSystName  += "_" + m_operatingPt;

  ANA_MSG_INFO( "Decision Decoration Name     : " << m_decor);
  ANA_MSG_INFO( "Scale Factor Decoration Name : " << m_decorSF);

  // now take this name and convert it to the cut value for the CDI file
  // if using the fixed efficiency points
  if(m_operatingPtCDI.empty()) m_operatingPtCDI = m_operatingPt;
  std::cout << "Using Standard OperatingPoint for CDI BTag Efficiency of " << m_operatingPtCDI << std::endl;

  // Outdated code for translating user friendly Btag WP to cut value
  // Code now accepts the user friendly Btag WP
  /*
  if( m_operatingPtCDI.find("FixedCutBEff") != std::string::npos) {
    m_operatingPtCDI.erase(0,13); // remove FixedCutBEff_
    std::cout << "Get OperatingPoint for CDI BTag Efficiency using eff = " << m_operatingPtCDI << std::endl;
    m_operatingPtCDI = HelperFunctions::GetBTagMV2c20_CutStr( atoi( m_operatingPtCDI.c_str() ) );
  }
  */

  m_runAllSyst = (m_systName.find("All") != std::string::npos);

  if( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  if ( !m_isMC ) {
    ANA_MSG_WARNING( "Attempting to run BTagging Jet Scale Factors on data.  Turning off scale factors." );
    m_getScaleFactors = false;
  }

  //
  // initialize the BJetSelectionTool
  //
  std::string sel_tool_name = std::string("BJetSelectionTool_") + m_name;
  if ( asg::ToolStore::contains<BTaggingSelectionTool>( sel_tool_name ) ) {
    m_BJetSelectTool = asg::ToolStore::get<BTaggingSelectionTool>( sel_tool_name );
  } else {
    m_BJetSelectTool = new BTaggingSelectionTool( sel_tool_name );
  }
  m_BJetSelectTool->msg().setLevel( MSG::INFO ); // DEBUG, VERBOSE, INFO, ERROR

  //
  //  Configure the BJetSelectionTool
  //
  // A few which are not configurable as of yet....
  // is there a reason to have this configurable here??...I think no (GF to self)
  ANA_CHECK( m_BJetSelectTool->setProperty("MaxEta",2.5));
  ANA_CHECK( m_BJetSelectTool->setProperty("MinPt",20000.));
  ANA_CHECK( m_BJetSelectTool->setProperty("FlvTagCutDefinitionsFileName",m_corrFileName.c_str()));
  // configurable parameters
  ANA_CHECK( m_BJetSelectTool->setProperty("TaggerName",          m_taggerName));
  ANA_CHECK( m_BJetSelectTool->setProperty("OperatingPoint",      m_operatingPt));
  ANA_CHECK( m_BJetSelectTool->setProperty("JetAuthor",           m_jetAuthor));
  ANA_CHECK( m_BJetSelectTool->initialize());
  ANA_MSG_INFO( "BTaggingSelectionTool initialized : " << m_BJetSelectTool->name() );

  //
  //  Configure the BJetEfficiencyCorrectionTool
  //
  if( m_getScaleFactors ) {

    //
    // initialize the BJetEfficiencyCorrectionTool
    //
    std::string sf_tool_name = std::string("BJetEfficiencyCorrectionTool_") + m_name;
    if ( asg::ToolStore::contains<BTaggingEfficiencyTool>( sf_tool_name ) ) {
      m_BJetEffSFTool = asg::ToolStore::get<BTaggingEfficiencyTool>( sf_tool_name );
    } else {
      m_BJetEffSFTool = new BTaggingEfficiencyTool( sf_tool_name );
    }
    m_BJetEffSFTool->msg().setLevel( MSG::INFO ); // DEBUG, VERBOSE, INFO, ERROR

    ANA_CHECK( m_BJetEffSFTool->setProperty("TaggerName",          m_taggerName));
    ANA_CHECK( m_BJetEffSFTool->setProperty("SystematicsStrategy", m_systematicsStrategy ));
    ANA_CHECK( m_BJetEffSFTool->setProperty("OperatingPoint",      m_operatingPtCDI));
    ANA_CHECK( m_BJetEffSFTool->setProperty("JetAuthor",           m_jetAuthor));
    ANA_CHECK( m_BJetEffSFTool->setProperty("ScaleFactorFileName", m_corrFileName));
    ANA_CHECK( m_BJetEffSFTool->setProperty("UseDevelopmentFile",  m_useDevelopmentFile));
    ANA_CHECK( m_BJetEffSFTool->setProperty("ConeFlavourLabel",    m_coneFlavourLabel));
    ANA_CHECK( m_BJetEffSFTool->initialize());
    ANA_MSG_INFO( "BTaggingEfficiencyTool initialized : " << m_BJetEffSFTool->name() );
  } else {
    ANA_MSG_WARNING( "Input operating point is not calibrated - no SFs will be obtained");
  }

  //
  // Print out
  //
  if( !m_systName.empty() && m_getScaleFactors ) {
    ANA_MSG_DEBUG("-----------------------------------------------------");
    const std::map<CP::SystematicVariation, std::vector<std::string> > allowed_variations = m_BJetEffSFTool->listSystematics();
    ANA_MSG_DEBUG("Allowed systematics variations for tool " << m_BJetEffSFTool->name() << ":");
    for (auto var : allowed_variations) {
      ANA_MSG_DEBUG(std::setw(40) << std::left << var.first.name() << ":");
      for (auto flv : var.second) ANA_MSG_DEBUG("\t" << flv);
    }
    ANA_MSG_DEBUG("-----------------------------------------------------");
  }


  // Get a list of affecting systematics
  if( m_getScaleFactors ) {
    CP::SystematicSet affectSysts = m_BJetEffSFTool->affectingSystematics();
    // Convert into a simple list
    std::vector<CP::SystematicSet> affectSystList = CP::make_systematics_vector(affectSysts);
    if( !m_systName.empty() ) {
      for ( const auto& syst_it : affectSystList ){
        ANA_MSG_DEBUG(" tool can be affected by systematic: " << syst_it.name());
      }
    }

    // Get a list of recommended systematics
    CP::SystematicSet recSysts = m_BJetEffSFTool->recommendedSystematics();
    // Convert into a simple list -- nominal is included already here!!
    m_systList = CP::make_systematics_vector(recSysts);
    if( !m_systName.empty() ) {
      for ( const auto& syst_it : m_systList ){
        ANA_MSG_DEBUG(" available recommended systematic: " << syst_it.name());
      }
    } else { // remove all but the nominal
      std::vector<CP::SystematicSet>::iterator syst_it = m_systList.begin();
      while( syst_it != m_systList.end() ) {
        if( syst_it->name().empty() ) { syst_it++; }
        syst_it = m_systList.erase(syst_it);
      }
    }

    if( m_systName.empty() ){
      ANA_MSG_INFO(" Running w/ nominal configuration!");
    }

  } else {
    // need the nominal to get the decisions
    CP::SystematicSet recSysts; // empty
    m_systList = CP::make_systematics_vector(recSysts); // comes back with 1 entry for nominal
  }

  if( m_runAllSyst ){
    ANA_MSG_INFO(" Running w/ All systematics");
  }

  ANA_MSG_INFO( "BJetEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode BJetEfficiencyCorrector :: execute ()
{
  ANA_MSG_DEBUG( "Applying BJet Efficency Corrector... ");

  //
  // retrieve event
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  ANA_MSG_DEBUG("\n\n eventNumber: " << eventInfo->eventNumber() << std::endl );

  //
  //  input jets
  //
  const xAOD::JetContainer* inJets(nullptr);

  //
  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  //
  if ( m_inputAlgo.empty() ) {

    // this will be the collection processed - no matter what!!
    ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, msg()) );

    executeEfficiencyCorrection( inJets, eventInfo, true);

  }
  //
  // get the list of systematics to run over
  //
  else {

    //
    // get vector of string giving the names
    //
    std::vector<std::string>* systNames(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, msg()) );

    //
    // loop over systematics
    //
    for ( auto systName : *systNames ) {

      bool doNominal = (systName == "");

      ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName+systName, m_event, m_store, msg()) );

      executeEfficiencyCorrection( inJets, eventInfo, doNominal );

    }

  }

  ANA_MSG_DEBUG( "Leave Efficency Selection... ");

  return EL::StatusCode::SUCCESS;

}




EL::StatusCode BJetEfficiencyCorrector :: executeEfficiencyCorrection(const xAOD::JetContainer* inJets,
								      const xAOD::EventInfo* eventInfo,
								      bool doNominal)
{
  ANA_MSG_DEBUG("Applying BJet Cuts and Efficiency Correction (when applicable...) ");

  //
  // Create Scale Factor aux for all jets
  //
  SG::AuxElement::Decorator< std::vector<float> > sfVec( m_decorSF );
  for( auto jet_itr : *(inJets)) {
    sfVec(*jet_itr) = std::vector<float>();
  }

  //
  // Define also an *event* weight, which is the product of all the BTag eff. SFs for each object in the event
  //
  std::string SF_NAME_GLOBAL = m_decorSF + "_GLOBAL";
  SG::AuxElement::Decorator< std::vector<float> > sfVec_GLOBAL ( SF_NAME_GLOBAL );

  std::vector< std::string >* sysVariationNames = new std::vector< std::string >;

  //
  // loop over available systematics
  //
  for(const auto& syst_it : m_systList){

    //
    // Initialise product of SFs for *this* systematic
    //
    float SF_GLOBAL(1.0);

    //
    //  If not nominal jets, dont calculate systematics
    //
    if ( !doNominal ) {
      if( syst_it.name() != "" ) {
        ANA_MSG_DEBUG("Not running B-tag systematics when doing JES systematics");
        continue;
      }
    }

    //
    // if not running systematics, only compulte weight for specified systematic (m_systName)
    //    default is nominal (i.e., "")
    //
    if ( !m_runAllSyst ) {
      if( syst_it.name() != m_systName ) {
        ANA_MSG_DEBUG("Not running systematics only apply nominal SF");
        continue;
      }
    }

    //
    // Create the name of the weight
    //   template:  SYSNAME
    //
    ANA_MSG_DEBUG("systematic variation name is: " << syst_it.name());
    sysVariationNames->push_back(syst_it.name());

    //
    // configure tool with syst variation
    //
    if (m_getScaleFactors ) {

      if (m_BJetEffSFTool->applySystematicVariation(syst_it) != CP::SystematicCode::Ok) {
        ANA_MSG_ERROR( "Failed to configure BJetEfficiencyCorrections for systematic " << syst_it.name());
        return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG("Successfully applied systematic: " << syst_it.name());
    }

    bool tagged(false);
    //
    // and now apply data-driven efficiency and efficiency SF!
    //
    unsigned int idx(0);
    for( auto jet_itr : *(inJets)) {

      //
      // Add decorator for decision
      //
      SG::AuxElement::Decorator< char > isBTag( m_decor );
      if( m_BJetSelectTool->accept( *jet_itr ) ) {
        isBTag( *jet_itr ) = 1;
        tagged = true;
      }
      else {
        isBTag( *jet_itr ) = 0;
        tagged = false;
      }

      float SF(1.0);
      // if only decorator with decision because OP is not calibrated, set SF to 1
      if ( m_getScaleFactors && fabs(jet_itr->eta()) < 2.5 ) {

        CP::CorrectionCode BJetEffCode;
        // if passes cut take the efficiency scale factor
        // if failed cut take the inefficiency scale factor
        if( tagged ) {
          BJetEffCode = m_BJetEffSFTool->getScaleFactor( *jet_itr, SF );
        } else {
          BJetEffCode = m_BJetEffSFTool->getInefficiencyScaleFactor( *jet_itr, SF );
        }
        if (BJetEffCode == CP::CorrectionCode::Error){
          ANA_MSG_WARNING( "Error in getEfficiencyScaleFactor");
          SF = -2;
          //return EL::StatusCode::FAILURE;
        }
        // if it is out of validity range (jet pt > 1200 GeV), the tools just applies the SF at 200 GeV
        //if (BJetEffCode == CP::CorrectionCode::OutOfValidityRange)
      } //m_getScaleFactors && eta < 2.5

      // Add it to vector
      sfVec(*jet_itr).push_back(SF);

      if(doNominal) SF_GLOBAL *= SF;

      /*
      if( m_getScaleFactors){
        //
        // directly obtain reco efficiency
        //
        float eff(0.0);
        if( (fabs(jet_itr->eta()) < 2.5) &&
            m_BJetEffSFTool->getEfficiency( *jet_itr, eff ) != CP::CorrectionCode::Ok){
          ANA_MSG_ERROR( "Problem in getRecoEfficiency");
          //return EL::StatusCode::FAILURE;
        }
        ANA_MSG_INFO( "\t reco efficiency = %g", eff );
      }
      */

       ANA_MSG_DEBUG( "===>>>");
       ANA_MSG_DEBUG( " ");
       ANA_MSG_DEBUG( "Jet " << idx << " pt = " << jet_itr->pt()*1e-3 << " GeV , eta = " << jet_itr->eta() );
       ANA_MSG_DEBUG( " ");
       ANA_MSG_DEBUG( "BTag SF decoration: " << m_decorSF );
       ANA_MSG_DEBUG( " ");
       ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
       ANA_MSG_DEBUG( " ");
       ANA_MSG_DEBUG( "BTag SF:");
       ANA_MSG_DEBUG( "\t from tool = " << SF << ", from object = " << sfVec(*jet_itr).back());
       ANA_MSG_DEBUG( "--------------------------------------");
      ++idx;

    } // close jet loop

    // For *this* systematic, store the global SF weight for the event
    ANA_MSG_DEBUG( "--------------------------------------");
    ANA_MSG_DEBUG( "GLOBAL BTag SF for event:");
    ANA_MSG_DEBUG( "\t " << SF_GLOBAL );
    ANA_MSG_DEBUG( "--------------------------------------");

    //
    //  Add the SF only if doing nominal Jets
    //
    if(doNominal) sfVec_GLOBAL( *eventInfo ).push_back( SF_GLOBAL );


  } // close loop on systematics

  //
  // add list of sys names to TStore
  //
  if(doNominal){
    ANA_CHECK( m_store->record( sysVariationNames, m_outputSystName));

    ANA_MSG_DEBUG("Size is " << sysVariationNames->size());
    for(auto sysName : *sysVariationNames) ANA_MSG_DEBUG(sysName);
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetEfficiencyCorrector :: postExecute ()
{
  ANA_MSG_DEBUG("Calling postExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: finalize ()
{
  ANA_MSG_INFO( "Deleting tool instances...");

  if ( m_BJetSelectTool ) { delete m_BJetSelectTool; m_BJetSelectTool = nullptr;  }
  if ( m_BJetEffSFTool )  { delete m_BJetEffSFTool; m_BJetEffSFTool = nullptr; }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: histFinalize ()
{
  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());

  return EL::StatusCode::SUCCESS;
}

