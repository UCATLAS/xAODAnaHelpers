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

//EDM
#include "xAODJet/JetAuxContainer.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/BJetEfficiencyCorrector.h"

#include <AsgTools/MessageCheck.h>

// tools
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"

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

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  // several lists of systematics could be configured
  // this is the case when MET sys should be added
  // to the OR ones

  // parse and split by comma
  std::string token;
  std::istringstream ss(m_inputAlgo);
  while (std::getline(ss, token, ',')) {
    m_inputAlgoList.push_back(token);
  }

  m_decorSF = m_decor + "_SF";

  bool opOK(false),taggerOK(false);
  m_getScaleFactors = false;
  // not calibrated yet
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTagCalib2017
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarksRelease21

  // All working points are calibrated (but not all taggers, see next check)
  if (m_operatingPt == "FixedCutBEff_60") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_70") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_77") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_85") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "HybBEff_60")  { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "HybBEff_70")  { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "HybBEff_77")  { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "HybBEff_85")  { opOK = true; m_getScaleFactors =  true; }

  // Only DL1 and MV2c10 are calibrated
  if (m_taggerName == "MV2c10")    { taggerOK = true; m_getScaleFactors =  true; }
  if (m_taggerName == "MV2c10rnn") { taggerOK = true; m_getScaleFactors =  false; }
  if (m_taggerName == "MV2c10mu")  { taggerOK = true; m_getScaleFactors =  false; }
  if (m_taggerName == "DL1")       { taggerOK = true; m_getScaleFactors =  true; }
  if (m_taggerName == "DL1rnn")    { taggerOK = true; m_getScaleFactors =  false; }
  if (m_taggerName == "DL1mu")     { taggerOK = true; m_getScaleFactors =  false; }

  if( !opOK || !taggerOK ) {
    ANA_MSG_ERROR( "Requested tagger/operating point is not known to xAH. Arrow v Indian? " << m_operatingPt);
    return EL::StatusCode::FAILURE;
  }

  // make unique name
  m_decor           += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorSF         += "_" + m_taggerName + "_" + m_operatingPt;
  m_outputSystName  += "_" + m_taggerName + "_" + m_operatingPt;

  ANA_MSG_INFO( "Decision Decoration Name     : " << m_decor);
  ANA_MSG_INFO( "Scale Factor Decoration Name : " << m_decorSF);

  // now take this name and convert it to the cut value for the CDI file
  // if using the fixed efficiency points
  if(m_operatingPtCDI.empty()) m_operatingPtCDI = m_operatingPt;
  ANA_MSG_INFO("Using Standard OperatingPoint for CDI BTag Efficiency of " << m_operatingPtCDI);

  m_runAllSyst = (m_systName.find("All") != std::string::npos);

  if( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  if ( !isMC() ) {
    ANA_MSG_WARNING( "Attempting to run BTagging Jet Scale Factors on data.  Turning off scale factors." );
    m_getScaleFactors = false;
  }

  // initialize the BJetSelectionTool
  // A few which are not configurable as of yet....
  // is there a reason to have this configurable here??...I think no (GF to self)
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("MaxEta",2.5));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("MinPt",20000.));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("FlvTagCutDefinitionsFileName",m_corrFileName.c_str()));
  // configurable parameters
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("TaggerName",          m_taggerName));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("OperatingPoint",      m_operatingPt));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("JetAuthor",           m_jetAuthor));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("OutputLevel", msg().level() ));
  ANA_CHECK( m_BJetSelectTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_BJetSelectTool_handle);

  //  Configure the BJetEfficiencyCorrectionTool
  if( m_getScaleFactors ) {

    // initialize the BJetEfficiencyCorrectionTool
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("TaggerName",          m_taggerName));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("SystematicsStrategy", m_systematicsStrategy ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("OperatingPoint",      m_operatingPtCDI));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("JetAuthor",           m_jetAuthor));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("ScaleFactorFileName", m_corrFileName));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("UseDevelopmentFile",  m_useDevelopmentFile));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("ConeFlavourLabel",    m_coneFlavourLabel));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("OutputLevel", msg().level() ));
    ANA_CHECK( m_BJetEffSFTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_BJetEffSFTool_handle);

  } else {
    ANA_MSG_WARNING( "Input operating point is not calibrated - no SFs will be obtained");
  }

  //
  // Print out
  //
  if( !m_systName.empty() && m_getScaleFactors ) {
    ANA_MSG_DEBUG("-----------------------------------------------------");
    const std::map<CP::SystematicVariation, std::vector<std::string> > allowed_variations = m_BJetEffSFTool_handle->listSystematics();
    ANA_MSG_DEBUG("Allowed systematics variations for tool " << m_BJetEffSFTool_handle.typeAndName() << ":");
    for (auto var : allowed_variations) {
      ANA_MSG_DEBUG(std::setw(40) << std::left << var.first.name() << ":");
      for (auto flv : var.second) ANA_MSG_DEBUG("\t" << flv);
    }
    ANA_MSG_DEBUG("-----------------------------------------------------");
  }


  // Get a list of affecting systematics
  if( m_getScaleFactors ) {
    CP::SystematicSet affectSysts = m_BJetEffSFTool_handle->affectingSystematics();
    // Convert into a simple list
    std::vector<CP::SystematicSet> affectSystList = CP::make_systematics_vector(affectSysts);
    if( !m_systName.empty() ) {
      for ( const auto& syst_it : affectSystList ){
        ANA_MSG_DEBUG(" tool can be affected by systematic: " << syst_it.name());
      }
    }

    if( m_systName.empty() )
      ANA_MSG_INFO(" Running w/ nominal configuration!");

    // Get a list of recommended systematics
    CP::SystematicSet recSyst = m_BJetEffSFTool_handle->recommendedSystematics();
    m_systList = HelperFunctions::getListofSystematics( recSyst, m_systName, m_systVal, msg() );
  }

  if( m_runAllSyst ){
    ANA_MSG_INFO(" Running w/ All systematics");
  }

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systList, m_outputSystName, fileMD);
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

  // if m_inputAlgo == "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  std::vector<std::string>* systNames_ptr(nullptr);
  if ( !m_inputAlgo.empty() ) ANA_CHECK( HelperFunctions::retrieve(systNames_ptr, m_inputAlgo, 0, m_store, msg()) );

  std::vector<std::string> systNames{""};
  if (systNames_ptr) systNames = *systNames_ptr;

  // loop over systematic sets available
  for ( auto systName : systNames ) {

    bool doNominal = (systName == "");

    // input jets
    const xAOD::JetContainer* inJets(nullptr);

    // some systematics might have rejected the event
    if ( m_store->contains<xAOD::JetContainer>( m_inContainerName+systName ) ) {
      // Check the existence of the container
      ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName+systName, m_event, m_store, msg()) );

      executeEfficiencyCorrection( inJets, eventInfo, doNominal );
    }

  }

  ANA_MSG_DEBUG( "Leave Efficency Selection... ");

  return EL::StatusCode::SUCCESS;

}




EL::StatusCode BJetEfficiencyCorrector :: executeEfficiencyCorrection(const xAOD::JetContainer* inJets,
								      const xAOD::EventInfo* /*eventInfo*/,
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
  // Add decorator for btag decision
  //
  for( auto jet_itr : *(inJets)) {
    SG::AuxElement::Decorator< char > isBTag( m_decor );
    if( m_BJetSelectTool_handle->accept( *jet_itr ) ) {
      isBTag( *jet_itr ) = 1;
    }
    else {
      isBTag( *jet_itr ) = 0;
    }
  }

  std::vector< std::string >* sysVariationNames = new std::vector< std::string >;

  //
  // loop over available systematics
  //
  for(const auto& syst_it : m_systList){

    //
    //  If not nominal input jet collection, dont calculate systematics
    //
    if ( !doNominal ) {
      if( syst_it.name() != "" ) { // if not nominal btag decision
        ANA_MSG_DEBUG("Not running B-tag systematics when doing JES systematics");
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

      if (m_BJetEffSFTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok) {
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
      // retrieve btag decision
      //
      SG::AuxElement::Decorator< char > isBTag( m_decor );
      tagged = isBTag(*jet_itr);

      float SF(1.0);
      // if only decorator with decision because OP is not calibrated, set SF to 1
      if ( m_getScaleFactors && fabs(jet_itr->eta()) < 2.5 ) {

        CP::CorrectionCode BJetEffCode;
        // if passes cut take the efficiency scale factor
        // if failed cut take the inefficiency scale factor
        if( tagged ) {
          BJetEffCode = m_BJetEffSFTool_handle->getScaleFactor( *jet_itr, SF );
        } else {
          BJetEffCode = m_BJetEffSFTool_handle->getInefficiencyScaleFactor( *jet_itr, SF );
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

      /*
      if( m_getScaleFactors){
        //
        // directly obtain reco efficiency
        //
        float eff(0.0);
        if( (fabs(jet_itr->eta()) < 2.5) &&
            m_BJetEffSFTool_handle->getEfficiency( *jet_itr, eff ) != CP::CorrectionCode::Ok){
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

  } // close loop on systematics

  //
  // add list of sys names to TStore
  //
  if(doNominal){
    ANA_CHECK( m_store->record( sysVariationNames, m_outputSystName));

    ANA_MSG_DEBUG("Size is " << sysVariationNames->size());
    for(auto sysName : *sysVariationNames) ANA_MSG_DEBUG(sysName);
  } else {
    delete sysVariationNames;
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
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetEfficiencyCorrector :: histFinalize ()
{
  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());

  return EL::StatusCode::SUCCESS;
}
