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


#include <SampleHandler/MetaFields.h>

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


BJetEfficiencyCorrector :: BJetEfficiencyCorrector (const std::string& name, ISvcLocator *pSvcLocator) :
    Algorithm(name, pSvcLocator, "BJetEfficiencyCorrector")
{
    declareProperty("inContainerName", m_inContainerName);
    declareProperty("inputAlgo", m_inputAlgo);
    declareProperty("systName", m_systName);
    declareProperty("outputSystName", m_outputSystName);
    declareProperty("writeSystToMetadata", m_writeSystToMetadata);
    declareProperty("corrFileName", m_corrFileName);
    declareProperty("jetAuthor", m_jetAuthor);
    declareProperty("minPt", m_minPt);
    declareProperty("taggerName", m_taggerName);
    declareProperty("useDevelopmentFile", m_useDevelopmentFile);
    declareProperty("coneFlavourLabel", m_coneFlavourLabel);
    declareProperty("systematicsStrategy", m_systematicsStrategy);
    declareProperty("errorOnTagWeightFailure", m_errorOnTagWeightFailure);
    declareProperty("operatingPt", m_operatingPt);
    declareProperty("operatingPtCDI", m_operatingPtCDI);
    declareProperty("getScaleFactors", m_getScaleFactors);
    declareProperty("useContinuous", m_useContinuous);
    declareProperty("decor", m_decor);
    declareProperty("orBJetPtUpperThres", m_orBJetPtUpperThres);
    declareProperty("EfficiencyCalibration", m_EfficiencyCalibration);
}


StatusCode BJetEfficiencyCorrector :: histInitialize ()
{
  ANA_CHECK( xAH::Algorithm::algInitialize());

  m_corrFileName = PathResolverFindCalibFile(m_corrFileName);

  return StatusCode::SUCCESS;
}



StatusCode BJetEfficiencyCorrector :: fileExecute ()
{
  return StatusCode::SUCCESS;
}



StatusCode BJetEfficiencyCorrector :: changeInput (bool /*firstFile*/)
{
  return StatusCode::SUCCESS;
}

StatusCode BJetEfficiencyCorrector :: initialize ()
{
  ANA_MSG_INFO( "Initializing BJetEfficiencyCorrector Interface... ");



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
  m_decorWeight   = m_decor + "_Weight";
  m_decorQuantile = m_decor + "_Quantile";
  m_decorInefficiencySF  = m_decor + "_InefficiencySF";

  bool opOK(false),taggerOK(false);
  m_getScaleFactors = false;
  m_useContinuous   = false;
  // not calibrated yet
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTagCalib2017
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarksRelease21

  // All working points are calibrated (but not all taggers, see next check)
  if (m_operatingPt == "FixedCutBEff_60") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_70") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_77") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_85") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "Continuous"     ) { opOK = true; m_getScaleFactors =  true;
    m_useContinuous = true;
    ANA_MSG_DEBUG(" Using continuous b-tagging"); }

  // Only DL1, DL1r, DLr1mu and MV2c10 are calibrated
  if (m_taggerName == "MV2c10") { taggerOK = true; m_getScaleFactors =  true; }
  if (m_taggerName == "DL1")    { taggerOK = true; m_getScaleFactors =  true; }
  if (m_taggerName == "DL1r")   { taggerOK = true; m_getScaleFactors =  true; }
  if (m_taggerName == "DL1rmu") { taggerOK = true; m_getScaleFactors =  true; }

  if( !opOK || !taggerOK ) {
    ANA_MSG_ERROR( "Requested tagger/operating point is not known to xAH. Arrow v Indian? " << m_taggerName << "/" << m_operatingPt);
    return StatusCode::FAILURE;
  }

  // make unique name
  m_decor               += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorSF             += "_" + m_taggerName + "_" + m_operatingPt;
  m_outputSystName      += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorWeight         += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorQuantile       += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorInefficiencySF += "_" + m_taggerName + "_" + m_operatingPt;

  if(!m_useContinuous){
    ANA_MSG_INFO( "Decision Decoration Name     : " << m_decor);
    ANA_MSG_INFO( "Scale Factor Decoration Name : " << m_decorSF);
  } else {
    ANA_MSG_INFO( "Weight Decoration Name: "        << m_decorWeight);
    ANA_MSG_INFO( "Quantile Decoration Name: "      << m_decorQuantile);
    ANA_MSG_INFO( "Scale Factor Decoration Name : " << m_decorSF);
    ANA_MSG_INFO( "Inefficiency Scale Factor Decoration Name : " << m_decorInefficiencySF);
  }

  // now take this name and convert it to the cut value for the CDI file
  // if using the fixed efficiency points
  if(m_operatingPtCDI.empty()) m_operatingPtCDI = m_operatingPt;
  ANA_MSG_INFO("Using Standard OperatingPoint for CDI BTag Efficiency of " << m_operatingPtCDI);

  m_runAllSyst = (m_systName.find("All") != std::string::npos);

  if( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return StatusCode::FAILURE;
  }

  if ( !isMC() && m_getScaleFactors ) {
    ANA_MSG_WARNING( "Attempting to run BTagging Jet Scale Factors on data.  Turning off scale factors." );
    m_getScaleFactors = false;
  }

  // initialize the BJetSelectionTool
  // A few which are not configurable as of yet....
  // is there a reason to have this configurable here??...I think no (GF to self)
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("FlvTagCutDefinitionsFileName",m_corrFileName.c_str()));
  // configurable parameters
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("TaggerName",          m_taggerName));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("OperatingPoint",      m_operatingPt));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("JetAuthor",           m_jetAuthor));
  if(m_minPt!=-1) ANA_CHECK( m_BJetSelectTool_handle.setProperty("MinPt", m_minPt));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("ErrorOnTagWeightFailure", m_errorOnTagWeightFailure));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("OutputLevel", msg().level() ));
  ANA_CHECK( m_BJetSelectTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_BJetSelectTool_handle);

  //  Configure the BJetEfficiencyCorrectionTool
  if( m_getScaleFactors ) {

    // initialize the BJetEfficiencyCorrectionTool
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("TaggerName",          m_taggerName         ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("SystematicsStrategy", m_systematicsStrategy));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("OperatingPoint",      m_operatingPtCDI     ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("JetAuthor",           m_jetAuthor          ));
    if(m_minPt!=-1) ANA_CHECK( m_BJetEffSFTool_handle.setProperty("MinPt", m_minPt            ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("ScaleFactorFileName", m_corrFileName       ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("UseDevelopmentFile",  m_useDevelopmentFile ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("ConeFlavourLabel",    m_coneFlavourLabel   ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("OutputLevel", msg().level() ));

    if(isMC() && !m_EfficiencyCalibration.empty()) {
      std::string calibration=m_EfficiencyCalibration;
      if(m_EfficiencyCalibration=="auto") {
          std::string gridName("");
          ANA_CHECK(inputMetaStore()->retrieve(gridName, SH::MetaFields::gridName));

          HelperFunctions::ShowerType sampleShowerType=HelperFunctions::Unknown;
          if(!gridName.empty()) { // Do not trust sample name on the grid, in case there are multiple samples per job
              std::stringstream ss(gridName);
              std::string sampleName;
              while(std::getline(ss,sampleName,',')) {
                  HelperFunctions::ShowerType mySampleShowerType=HelperFunctions::getMCShowerType(sampleName);
                  if(mySampleShowerType!=sampleShowerType && sampleShowerType!=HelperFunctions::Unknown){
                    ANA_MSG_ERROR("Cannot have different shower types per grid task.");
                  }
                  sampleShowerType=mySampleShowerType;
              }
          } else { // Use sample name when running locally
              ANA_CHECK(inputMetaStore()->retrieve(gridName, SH::MetaFields::sampleName));
              sampleShowerType=HelperFunctions::getMCShowerType(gridName);
          }

          switch(sampleShowerType) {
              case HelperFunctions::Pythia8:
                  calibration="410470";
              break;
              case HelperFunctions::Herwig7:
                  calibration="410558";
              break;
              case HelperFunctions::Sherpa21:
                  calibration="426131";
              break;
              case HelperFunctions::Sherpa22:
                  calibration="410250";
              break;
              case HelperFunctions::Unknown:
                  ANA_MSG_ERROR("Cannot determine MC shower type for sample " << gridName << ".");
                  return StatusCode::FAILURE;
              break;
          }
      }
      ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyBCalibrations"    ,  calibration));
      ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyCCalibrations"    ,  calibration));
      ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyTCalibrations"    ,  calibration));
      ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyLightCalibrations",  calibration));
  }

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
    writeSystematicsListHist(m_systList, m_outputSystName);
  }

  ANA_MSG_INFO( "BJetEfficiencyCorrector Interface succesfully initialized!" );

  return StatusCode::SUCCESS;
}

StatusCode BJetEfficiencyCorrector :: execute ()
{
  ANA_MSG_DEBUG( "Applying BJetEfficiencyCorrector for " << m_taggerName << " tagger... ");

  //
  // retrieve event
  //
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( evtStore()->retrieve(eventInfo, m_eventInfoContainerName) );
  ANA_MSG_DEBUG("\n\n eventNumber: " << eventInfo->eventNumber() << std::endl );

  // if m_inputAlgo == "" --> input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  std::vector<std::string>* systNames_ptr(nullptr);
  if ( !m_inputAlgo.empty() ) ANA_CHECK( evtStore()->retrieve(systNames_ptr, m_inputAlgo) );

  std::vector<std::string> systNames{""};
  if (systNames_ptr) systNames = *systNames_ptr;

  // loop over systematic sets available
  for ( const std::string& systName : systNames ) {

    bool doNominal = (systName == "");

    // input jets
    const xAOD::JetContainer* inJets(nullptr);

    // some systematics might have rejected the event
    if ( evtStore()->contains<xAOD::JetContainer>( m_inContainerName+systName ) ) {
      // Check the existence of the container
      ANA_CHECK( evtStore()->retrieve(inJets, m_inContainerName+systName) );

      ANA_CHECK( executeEfficiencyCorrection( inJets, eventInfo, doNominal ) );
    }

  }

  ANA_MSG_DEBUG( "Leave Efficency Selection... ");

  return StatusCode::SUCCESS;

}




StatusCode BJetEfficiencyCorrector :: executeEfficiencyCorrection(const xAOD::JetContainer* inJets,
								      const xAOD::EventInfo* /*eventInfo*/,
								      bool doNominal)
{
  ANA_MSG_DEBUG("Applying BJet Cuts and Efficiency Correction (when applicable...) ");

  SG::AuxElement::Decorator< char > dec_isBTag( m_decor );
  SG::AuxElement::Decorator< std::vector<float> > dec_sfBTag( m_decorSF );

  SG::AuxElement::Decorator< char > dec_isBTagOR( m_decor+"OR" );

  // for continuous b-tagging only
  SG::AuxElement::Decorator< float > dec_Weight( m_decorWeight );
  SG::AuxElement::Decorator< int > dec_Quantile( m_decorQuantile );
  SG::AuxElement::Decorator< std::vector<float> > dec_ineffsfBTag( m_decorInefficiencySF );

  //
  // run the btagging decision or get weight and quantile if running continuous
  //
  for( const xAOD::Jet* jet_itr : *(inJets))
    {

      if(!m_useContinuous)
	{ // get tagging decision
	  ANA_MSG_DEBUG(" Getting tagging decision ");

	  // Add decorator for decision
	  if( m_BJetSelectTool_handle->accept( *jet_itr ) )
	    dec_isBTag( *jet_itr ) = 1;
	  else
	    dec_isBTag( *jet_itr ) = 0;

	  // Add pT-dependent b-tag decision decorator (intended for use in OR)
	  if ((m_orBJetPtUpperThres < 0 || m_orBJetPtUpperThres > (*jet_itr).pt()/1000.) // passes pT criteria
	      && m_BJetSelectTool_handle->accept( *jet_itr ) )
	    dec_isBTagOR( *jet_itr ) = 1;
	  else
	    dec_isBTagOR( *jet_itr ) = 0;
	}
      else
	{ // continuous b-tagging

	  ANA_MSG_DEBUG(" Getting TaggerWeight and Quantile");

	  double tagWeight;
	  if( m_BJetSelectTool_handle->getTaggerWeight( *jet_itr, tagWeight)!=CP::CorrectionCode::Ok )
	    {
	      ANA_MSG_ERROR(" Error retrieving b-tagger weight ");
	      return StatusCode::FAILURE;
	    }
	  int quantile = m_BJetSelectTool_handle->getQuantile(*jet_itr);
	  ANA_MSG_DEBUG( "tagWeight: " << tagWeight );
	  ANA_MSG_DEBUG( "quantile: " << quantile );
	  dec_Weight( *jet_itr)    = tagWeight;
	  dec_Quantile( *jet_itr ) = quantile;
	}
    }

  //
  // get the scale factors for all jets
  //
  if(m_getScaleFactors)
    { // loop over available systematics
      for(const CP::SystematicSet& syst_it : m_systList)
	{
	  //  If not nominal input jet collection, dont calculate systematics
	  if ( !doNominal )
	    {
	      if( syst_it.name() != "" )
		{ // if not nominal btag decision
		  ANA_MSG_DEBUG("Not running B-tag systematics when doing JES systematics");
		  continue;
		}
	    }

	  // configure tool with syst variation
	  if (m_BJetEffSFTool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok)
	    {
	      ANA_MSG_ERROR( "Failed to configure BJetEfficiencyCorrections for systematic " << syst_it.name());
	      return StatusCode::FAILURE;
	    }
	  ANA_MSG_DEBUG("Successfully configured BJetEfficiencyCorrections for systematic: " << syst_it.name());

	  for( const xAOD::Jet* jet_itr : *(inJets))
	    {
	      // get the scale factor
	      float SF(-1.0);
	      float inefficiencySF(-1.0); // only for continuous b-tagging
	      CP::CorrectionCode BJetEffCode;
	      CP::CorrectionCode BJetIneEffCode = CP::CorrectionCode::Ok;
	      // if passes cut take the efficiency scale factor
	      // if failed cut take the inefficiency scale factor
	      // for continuous b-tagging save both
	      if(m_useContinuous)
		{
		  BJetEffCode = m_BJetEffSFTool_handle->getScaleFactor( *jet_itr, SF );
		  BJetIneEffCode = m_BJetEffSFTool_handle->getInefficiencyScaleFactor( *jet_itr, inefficiencySF );
		}
	      else
		{
		  if( dec_isBTag( *jet_itr ) )
		    BJetEffCode = m_BJetEffSFTool_handle->getScaleFactor( *jet_itr, SF );
		  else
		    BJetEffCode = m_BJetEffSFTool_handle->getInefficiencyScaleFactor( *jet_itr, SF );
		}

	      if (BJetEffCode == CP::CorrectionCode::Error || BJetIneEffCode == CP::CorrectionCode::Error)
		{
		  ANA_MSG_ERROR( "Error in getEfficiencyScaleFactor");
		  return StatusCode::FAILURE;
		}
	      else if (BJetEffCode == CP::CorrectionCode::OutOfValidityRange)
		{
		  ANA_MSG_DEBUG( "Jet is out of validity range");
		}

	      // Save it to the vector
	      if(                   !dec_sfBTag     .isAvailable( *jet_itr ))
		dec_sfBTag     ( *jet_itr ) = std::vector<float>();
	      if(m_useContinuous && !dec_ineffsfBTag.isAvailable( *jet_itr ))
		dec_ineffsfBTag( *jet_itr ) = std::vector<float>();


	      dec_sfBTag( *jet_itr ).push_back(SF);
	      if(m_useContinuous) dec_ineffsfBTag( *jet_itr ).push_back(inefficiencySF);
      }
    }
  }
  else
    {
      for( const xAOD::Jet* jet_itr : *(inJets))
	{
	  if(                   !dec_sfBTag     .isAvailable( *jet_itr ))
	    dec_sfBTag     ( *jet_itr ) = std::vector<float>({1.});
	  if(m_useContinuous && !dec_ineffsfBTag.isAvailable( *jet_itr ))
	    dec_ineffsfBTag( *jet_itr ) = std::vector<float>({1.});
	}
    }

  //
  // Store list of available systematics
  //
  if(doNominal){
    auto sysVariationNames = std::make_unique< std::vector< std::string > >();
    if(m_getScaleFactors) {
      for(const auto& syst_it : m_systList)
	sysVariationNames->push_back(syst_it.name());
    }
    else {
      sysVariationNames->push_back("");
    }

    ANA_MSG_DEBUG("Size is " << sysVariationNames->size());
    for(auto sysName : *sysVariationNames) ANA_MSG_DEBUG(sysName);

    ANA_CHECK( evtStore()->record( std::move(sysVariationNames), m_outputSystName));
  }

  return StatusCode::SUCCESS;
}


StatusCode BJetEfficiencyCorrector :: finalize ()
{
  return StatusCode::SUCCESS;
}



StatusCode BJetEfficiencyCorrector :: histFinalize ()
{
  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());

  return StatusCode::SUCCESS;
}
