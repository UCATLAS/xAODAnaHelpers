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

#include <SampleHandler/MetaFields.h>

#include "PathResolver/PathResolver.h"

//EDM
#include "xAODJet/JetAuxContainer.h"

// package include(s):
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/BJetEfficiencyCorrector.h"

#include <AsgMessaging/MessageCheck.h>

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
  m_decorWeight   = m_decor + "_Weight";
  m_decorQuantile = m_decor + "_Quantile";

  bool opOK(false),taggerOK(false);
  m_getScaleFactors = false;
  m_useContinuous   = false;
  // not calibrated yet
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTagCalib2017
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BTaggingBenchmarksRelease21

  // All working points are calibrated (but not all taggers, see next check)
  if (m_operatingPt == "FixedCutBEff_60" and (m_taggerName != "GN2v01")) { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_65") {
    if(m_taggerName == "GN2v01"){
        opOK = true; m_getScaleFactors =  true;
    }
  }
  if (m_operatingPt == "FixedCutBEff_70") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_77") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_85") { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "FixedCutBEff_90" and (m_taggerName == "GN2v01")) { opOK = true; m_getScaleFactors =  true; }
  if (m_operatingPt == "Continuous"     ) { opOK = true; m_getScaleFactors =  true;
    m_useContinuous = true;
    ANA_MSG_DEBUG(" Using continuous b-tagging"); }

  // No official calibrations in rel22 yet
  if (m_taggerName == "DL1r")   { taggerOK = true; m_getScaleFactors =  false; }
  if (m_taggerName == "DL1dv00")   { taggerOK = true; m_getScaleFactors = false; }
  if (m_taggerName == "DL1dv01")   { taggerOK = true; m_getScaleFactors =  true; }
  if (m_taggerName == "GN120220509")   { taggerOK = true; m_getScaleFactors =  false; }
  if (m_taggerName == "GN2v00LegacyWP")   { taggerOK = true; m_getScaleFactors =  false; }
  if (m_taggerName == "GN2v00NewAliasWP")   { taggerOK = true; m_getScaleFactors =  false; }
  if (m_taggerName == "GN2v01")   { taggerOK = true; m_getScaleFactors =  true; }

  if( !opOK || !taggerOK ) {
    ANA_MSG_ERROR( "Requested tagger/operating point is not known to xAH. Arrow v Indian? " << m_taggerName << "/" << m_operatingPt);
    return EL::StatusCode::FAILURE;
  }

  // make unique name
  m_decor               += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorSF             += "_" + m_taggerName + "_" + m_operatingPt;
  m_outputSystName      += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorWeight         += "_" + m_taggerName + "_" + m_operatingPt;
  m_decorQuantile       += "_" + m_taggerName + "_" + m_operatingPt;

  if(!m_useContinuous){
    ANA_MSG_INFO( "Decision Decoration Name     : " << m_decor);
    ANA_MSG_INFO( "Scale Factor Decoration Name : " << m_decorSF);
  } else {
    ANA_MSG_INFO( "Weight Decoration Name: "        << m_decorWeight);
    ANA_MSG_INFO( "Quantile Decoration Name: "      << m_decorQuantile);
    ANA_MSG_INFO( "Scale Factor Decoration Name : " << m_decorSF);
  }

  // now take this name and convert it to the cut value for the CDI file
  // if using the fixed efficiency points
  if(m_operatingPtCDI.empty()) m_operatingPtCDI = m_operatingPt;
  ANA_MSG_INFO("Using Standard OperatingPoint for CDI BTag Efficiency of " << m_operatingPtCDI);

  m_runAllSyst = (m_systName.find("All") != std::string::npos);

  if( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  if ( !isMC() && m_getScaleFactors ) {
    ANA_MSG_WARNING( "Attempting to run BTagging Jet Scale Factors on data.  Turning off scale factors." );
    m_getScaleFactors = false;
  }
  if ( m_tagDecisionOnly && m_getScaleFactors ) {
    ANA_MSG_WARNING( "BTagging scale factors have been manually disabled. This is not recommended!" );
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
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("MinPt", m_minPt));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("ErrorOnTagWeightFailure", m_errorOnTagWeightFailure));
  ANA_CHECK( m_BJetSelectTool_handle.setProperty("readFromBTaggingObject", m_readFromBTaggingObject));
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
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("MinPt", m_minPt            ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("ScaleFactorFileName", m_corrFileName       ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("UseDevelopmentFile",  m_useDevelopmentFile ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("ConeFlavourLabel",    m_coneFlavourLabel   ));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("readFromBTaggingObject", m_readFromBTaggingObject));
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("OutputLevel", msg().level() ));

    if(!m_EfficiencyCalibration.empty()){
	std::string calibration=m_EfficiencyCalibration;
	if(m_EfficiencyCalibration=="auto")
	  {
	    std::string gridName=wk()->metaData()->castString(SH::MetaFields::gridName);

	    HelperFunctions::ShowerType sampleShowerType=HelperFunctions::Unknown;
	    if(!gridName.empty())
	      { // Do not trust sample name on the grid, in case there are multiple samples per job
		std::stringstream ss(gridName);
		std::string sampleName;
		while(std::getline(ss,sampleName,','))
		  {
		    HelperFunctions::ShowerType mySampleShowerType=HelperFunctions::getMCShowerType(sampleName,m_taggerName);
		    if(mySampleShowerType!=sampleShowerType && sampleShowerType!=HelperFunctions::Unknown)
		      ANA_MSG_ERROR("Cannot have different shower types per grid task.");
		    sampleShowerType=mySampleShowerType;
		  }
	      }
	    else // Use sample name when running locally
	      {
		gridName=wk()->metaData()->castString(SH::MetaFields::sampleName);
		sampleShowerType=HelperFunctions::getMCShowerType(gridName,m_taggerName);
	      }
       
        if(m_taggerName=="DL1dv01"){
            if(m_isRun3){
                switch(sampleShowerType)
                {
                    case HelperFunctions::Pythia8:
                        calibration="601229";
                        break;
                    case HelperFunctions::Herwig7p2:
                        calibration="601414";
                        break;
                    case HelperFunctions::Sherpa2212:
                        calibration="700660";
                        break;
                    default:
                        if (m_allowCalibrationFallback) {
                          ANA_MSG_WARNING("Cannot determine MC shower type for sample " << gridName << ", falling back to 'default'.");
                          ANA_MSG_WARNING("Please double-check if this is appropriate for your sample, otherwise you have specify the MC-to-MC calibration manually!");
                          calibration="default";
                          break;
                        }
                        else {
                          ANA_MSG_ERROR("Cannot determine MC shower type for sample " << gridName << ".");
                          return EL::StatusCode::FAILURE;
                           break;
                        }
                }
            } else {

    	        switch(sampleShowerType)
	            {
	                case HelperFunctions::Pythia8:
		                calibration="410470";
		                break;
    	            case HelperFunctions::Herwig7p1:
	    	            calibration="411233";
                        break; 
                    case HelperFunctions::Herwig7p2:
                        calibration="600666";
		                break;
    	            case HelperFunctions::Sherpa221:
	    	            calibration="410250";
		                break;
	                case HelperFunctions::Sherpa2210:
		                calibration="700122";
		                break;
                    case HelperFunctions::Sherpa2212:
                        calibration="700660";
                        break;
                    case HelperFunctions::AmcPy8:
                        calibration="410464";
                        break;
                    case HelperFunctions::AmcH7:
                        calibration="412116";
                        break;
	                default:
                        if (m_allowCalibrationFallback) {
                          ANA_MSG_WARNING("Cannot determine MC shower type for sample " << gridName << ", falling back to 'default'.");
                          ANA_MSG_WARNING("Please double-check if this is appropriate for your sample, otherwise you have specify the MC-to-MC calibration manually!");
                          calibration="default";
                          break;
                        }
                        else {
		                  ANA_MSG_ERROR("Cannot determine MC shower type for sample " << gridName << ".");
		                  return EL::StatusCode::FAILURE;
		                  break;
                        }
	            }
            }
        } else if(m_taggerName=="GN2v01"){
            if(m_isRun3){
                switch(sampleShowerType)
                {
                    case HelperFunctions::Pythia8:
                        calibration="default";
                        break;
                    case HelperFunctions::Herwig7p2:
                        calibration="601414";
                        break;
                    case HelperFunctions::Sherpa2214:
                        calibration="700808";
                        break;
                    case HelperFunctions::Sherpa_Unknown:
                        ANA_MSG_WARNING("Unknown Sherpa version MC shower type for sample " << gridName << ", falling back to MC-MC SFs for Sherpa 2.2.11-2.2.16.");
                        ANA_MSG_WARNING("Please double-check if this is appropriate for your sample, otherwise you have specify the MC-to-MC calibration manually!");
                        calibration="700808";
                        break;
                    default:
                        if (m_allowCalibrationFallback) {
                          ANA_MSG_WARNING("Cannot determine MC shower type for sample " << gridName << ", falling back to 'default'.");
                          ANA_MSG_WARNING("Please double-check if this is appropriate for your sample, otherwise you have specify the MC-to-MC calibration manually!");
                          calibration="default";
                          break;
                        }
                        else {
                          ANA_MSG_ERROR("Cannot determine MC shower type for sample " << gridName << ".");
                          return EL::StatusCode::FAILURE;
                           break;
                        }
                }
            } else {

                switch(sampleShowerType)
                {
                    case HelperFunctions::Pythia8:
                        calibration="default";
                        break;
                    case HelperFunctions::Herwig7p1:
                        calibration="411233";
                        break;
                    case HelperFunctions::Herwig7p2:
                        calibration="600666";
                        break;
                    case HelperFunctions::Sherpa2214:
                        calibration="700660";
                        break;
                    case HelperFunctions::Sherpa_Unknown:
                        ANA_MSG_WARNING("Unknown Sherpa version MC shower type for sample " << gridName << ", falling back to MC-MC SFs for Sherpa 2.2.11-2.2.16.");
                        ANA_MSG_WARNING("Please double-check if this is appropriate for your sample, otherwise you have specify the MC-to-MC calibration manually!");
                        calibration="700660";
                        break;
                    default:
                      if (m_allowCalibrationFallback) {
                        ANA_MSG_WARNING("Cannot determine MC shower type for sample " << gridName << ", falling back to 'default'.");
                        ANA_MSG_WARNING("Please double-check if this is appropriate for your sample, otherwise you have specify the MC-to-MC calibration manually!");
                        calibration="default";
                        break;
                      }
                      else {
                        ANA_MSG_ERROR("Cannot determine MC shower type for sample " << gridName << ".");
                        return EL::StatusCode::FAILURE;
                        break;
                     }
                }
            }
        } else {
            if (m_allowCalibrationFallback) {
                ANA_MSG_WARNING("Cannot determine MC shower type for tagger " << m_taggerName << ", only GN2v01 and DL1dv01 are supported. Falling back to 'default'.");
                ANA_MSG_WARNING("Please double-check if this is appropriate for your sample and tagger, otherwise you have specify the MC-to-MC calibration manually!");
                calibration="default";
            } else {
                ANA_MSG_ERROR("Cannot determine MC shower type for tagger " << m_taggerName << ".");
                return EL::StatusCode::FAILURE;
            }
        }
	  } else { makeMCIndexMap(m_EfficiencyCalibration); }
	ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyBCalibrations"    ,  calibration));
	ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyCCalibrations"    ,  calibration));
	ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyTCalibrations"    ,  calibration));
	ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EfficiencyLightCalibrations",  calibration));
      }

    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EigenvectorReductionB"        ,  m_EigenvectorReductionB) );
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EigenvectorReductionC"        ,  m_EigenvectorReductionC) );
    ANA_CHECK( m_BJetEffSFTool_handle.setProperty("EigenvectorReductionLight"    ,  m_EigenvectorReductionLight) );

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

  // Get DSID to Generator map (if needed)
  if (m_setMapIndex){
    ANA_MSG_DEBUG("Fill DSID->Generator map");
    // open input file
    std::ifstream file;
    std::string fn = PathResolverFindCalibFile(m_DSIDtoGenerator_filename);
    //file.open(gSystem->ExpandPathName(m_DSIDtoGenerator_filename.c_str()));
    file.open(fn.c_str());
    if (!file.good()) {
      ANA_MSG_ERROR("Can't open file " << m_DSIDtoGenerator_filename.c_str());
      return EL::StatusCode::FAILURE;
    }
    // process file
    while (!file.eof()) {
      // read line
      std::string lineString;
      getline(file, lineString);
      // store in map
      std::stringstream line(lineString);
      int dsid;
      std::string gen;
      line >> dsid >> gen;
      if (m_DSIDtoGenerator.count(dsid) != 0) {
        ANA_MSG_WARNING("Skipping duplicated DSID for line " << lineString.c_str());
        continue;
      }
      m_DSIDtoGenerator[dsid] = gen;
    }
    file.close();
  }

  ANA_MSG_INFO( "BJetEfficiencyCorrector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode BJetEfficiencyCorrector :: execute ()
{
  ANA_MSG_DEBUG( "Applying BJetEfficiencyCorrector for " << m_taggerName << " tagger... ");

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
  for ( const std::string& systName : systNames ) {

    bool doNominal = (systName == "");

    // input jets
    const xAOD::JetContainer* inJets(nullptr);

    // some systematics might have rejected the event
    if ( m_store->contains<xAOD::JetContainer>( m_inContainerName+systName ) ) {
      // Check the existence of the container
      ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName+systName, m_event, m_store, msg()) );

      ANA_CHECK( executeEfficiencyCorrection( inJets, eventInfo, doNominal ) );
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

  SG::AuxElement::Decorator< char > dec_isBTag( m_decor );
  SG::AuxElement::Decorator< std::vector<float> > dec_sfBTag( m_decorSF );

  SG::AuxElement::Decorator< char > dec_isBTagOR( m_decor+"OR" );

  // for continuous b-tagging only
  SG::AuxElement::Decorator< float > dec_Weight( m_decorWeight );
  SG::AuxElement::Decorator< int > dec_Quantile( m_decorQuantile );

  //
  // run the btagging decision or get weight and quantile if running continuous
  //
  for( const xAOD::Jet* jet_itr : *(inJets)){

    if(abs(jet_itr->eta()) > 2.5){
      if(!m_useContinuous){
        dec_isBTag( *jet_itr ) = 0;
        dec_isBTagOR( *jet_itr ) = 0;
      }else{
        dec_Quantile( *jet_itr ) = -1;
      }
      if(m_useContinuous || m_alwaysGetTagWeight) dec_Weight( *jet_itr) = 1;
      continue;
    }

    if(!m_useContinuous){
      // get tagging decision
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
    else{
      ANA_MSG_DEBUG(" Getting Quantile");
      int quantile = m_BJetSelectTool_handle->getQuantile(*jet_itr);
      ANA_MSG_DEBUG( "quantile: " << quantile );
      dec_Quantile( *jet_itr ) = quantile;
    }
    if(m_useContinuous || m_alwaysGetTagWeight){
      ANA_MSG_DEBUG(" Getting TaggerWeight");
      
      double tagWeight;
      if( m_BJetSelectTool_handle->getTaggerWeight( *jet_itr, tagWeight)!=CP::CorrectionCode::Ok ){
        ANA_MSG_ERROR(" Error retrieving b-tagger weight ");
        return EL::StatusCode::FAILURE;
      }
      ANA_MSG_DEBUG( "tagWeight: " << tagWeight );
	    dec_Weight( *jet_itr)    = tagWeight;
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
	  if (m_BJetEffSFTool_handle->applySystematicVariation(syst_it) != EL::StatusCode::SUCCESS)
	    {
	      ANA_MSG_ERROR( "Failed to configure BJetEfficiencyCorrections for systematic " << syst_it.name());
	      return EL::StatusCode::FAILURE;
	    }
	  ANA_MSG_DEBUG("Successfully configured BJetEfficiencyCorrections for systematic: " << syst_it.name());

          for( const xAOD::Jet* jet_itr : *(inJets))
	    {
              if(abs(jet_itr->eta()) > 2.5){
                if(!dec_sfBTag.isAvailable( *jet_itr ))
	                dec_sfBTag     ( *jet_itr ) = std::vector<float>({1.});
                continue;
              }
              if(m_setMapIndex){ // select an efficiency map for use in MC/MC and inefficiency scale factors, based on user specified selection of efficiency maps
                auto FlavLabel = getFlavorLabel(*jet_itr);
                auto DSID      = eventInfo->mcChannelNumber();
                auto MCindex   = getMCIndex(DSID);
                ANA_CHECK( m_BJetEffSFTool_handle->setMapIndex(FlavLabel,MCindex));
              }
	      // get the scale factor
	      float SF(-1.0);
	      CP::CorrectionCode BJetEffCode;
	      // if passes cut take the efficiency scale factor
	      // if failed cut take the inefficiency scale factor
	      // for continuous take efficiency, as inefficiency should not be used
	      if(m_useContinuous)
		{
		  BJetEffCode = m_BJetEffSFTool_handle->getScaleFactor( *jet_itr, SF );
		}
	      else
		{
		  if( dec_isBTag( *jet_itr ) )
		    BJetEffCode = m_BJetEffSFTool_handle->getScaleFactor( *jet_itr, SF );
		  else
		    BJetEffCode = m_BJetEffSFTool_handle->getInefficiencyScaleFactor( *jet_itr, SF );
		}

	      if (BJetEffCode == CP::CorrectionCode::Error)
		{
		  ANA_MSG_ERROR( "Error in getEfficiencyScaleFactor");
		  return EL::StatusCode::FAILURE;
		}
	      else if (BJetEffCode == CP::CorrectionCode::OutOfValidityRange)
		{
		  ANA_MSG_DEBUG( "Jet is out of validity range");
		}

	      // Save it to the vector
	      if(                   !dec_sfBTag     .isAvailable( *jet_itr ))
		dec_sfBTag     ( *jet_itr ) = std::vector<float>();


	      dec_sfBTag( *jet_itr ).push_back(SF);
      }
    }
  }
  else
    {
      for( const xAOD::Jet* jet_itr : *(inJets))
	{
	  if(                   !dec_sfBTag     .isAvailable( *jet_itr ))
	    dec_sfBTag     ( *jet_itr ) = std::vector<float>({1.});
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

    ANA_CHECK( m_store->record( std::move(sysVariationNames), m_outputSystName));
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

void BJetEfficiencyCorrector :: makeMCIndexMap (std::string effCalib)
{
  ANA_MSG_DEBUG( "Calling makeMCIndexMap()");

  // Interprete m_EfficiencyCalibration string to identify the corresponding indexes
  char delim = ';';
  std::vector<std::string> samples;
  std::stringstream ss(effCalib);
  std::string item;
  while (std::getline(ss, item, delim)) {
    samples.push_back(item);
  }

  // Find index for Pythia8, Sherpa 2.2, Herwig7 and aMC@NLO+Pythia8
  for (unsigned int i = 0; i < samples.size(); ++i){
    if (samples.at(i) == "410470") m_MCIndexes["Pythia8"]    = i;
    if (samples.at(i) == "700122") m_MCIndexes["Sherpa2210"] = i;
    if (samples.at(i) == "410250") m_MCIndexes["Sherpa22"]   = i;
    if (samples.at(i) == "600666") m_MCIndexes["Herwig721"]  = i;
    if (samples.at(i) == "410558") m_MCIndexes["Herwig7"]    = i;
    if (samples.at(i) == "410464") m_MCIndexes["aMC@NLO"]    = i;
  }

}

unsigned int BJetEfficiencyCorrector :: getMCIndex (int dsid)
{
  ANA_MSG_DEBUG( "Calling getMCIndex");

  auto name = m_DSIDtoGenerator[dsid];

  if(m_EfficiencyCalibration=="auto") {
    return 0; // there is only one index when m_EfficiencyCalibration == 'auto'
  } else {
    // Pythia 8
    if ((name.find("Pythia8_") != std::string::npos) ||
        (name.find("Pythia8B_") != std::string::npos) ||
        (name.find("MGPy8EG_") != std::string::npos) ||
        (name.find("MadGraphPythia8_") != std::string::npos) ||
        (name.find("MadGraphPythia8EvtGen_") != std::string::npos) ||
        (name.find("PowhegPythia8EvtGen_CT10_") != std::string::npos) ||
        (name.find("PowhegPythia8EvtGen_") != std::string::npos) ||
        (name.find("Pythia8EvtGen_") != std::string::npos) ||
        (name.find("Pythia8EG_") != std::string::npos) ||
        (name.find("PwPy8EG_CT10_") != std::string::npos) ||
        (name.find("PowhegPythia8_") != std::string::npos) ||
        (name.find("PowhegPythia8EG_") != std::string::npos) ||
        (name.find("PhPy8EG_CT10_") != std::string::npos) ||
        (name.find("Py8EG_") != std::string::npos) ||
        (name.find("Py8") != std::string::npos)) { return m_MCIndexes["Pythia8"]; }

    // Sherpa 2.2.10
    if ((name.find("Sh_2210") != std::string::npos)){ return m_MCIndexes["Sherpa2210"]; }

    // Sherpa 2.2.11
    if ((name.find("Sh_2211") != std::string::npos)){ return m_MCIndexes["Sherpa2210"]; } // MC-MC SFs from Sherpa 2.2.10 is the best we can use for Sherpa 2.2.11 (for now)

    // Sherpa 2.2
    if ((name.find("Sh_22") != std::string::npos) ||
        (name.find("Sherpa_NNPDF30NNLO") != std::string::npos) ||
        (name.find("Sherpa_221_NNPDF30NNLO") != std::string::npos)){ return m_MCIndexes["Sherpa22"]; }

    // Herwig 7.2.1
    if ( (name.find("Herwig721") != std::string::npos) ){ return m_MCIndexes["Herwig721"]; }

    // Herwig 7
    if ( (name.find("Herwig") != std::string::npos) ){ return m_MCIndexes["Herwig"]; }

    // aMC@NLO+Pythia8
    if ( (name.find("aMC@NLO+Pythia8") != std::string::npos) ){ return m_MCIndexes["aMC@NLO"]; }

    // for all other samples, use the default map (pythia8).
    ANA_MSG_WARNING( "MC index not recognized, using default (0)." );
    return 0;
  }
}

std::string BJetEfficiencyCorrector :: getFlavorLabel (const xAOD::Jet &jet) const
{
  int label;
  bool status = jet.getAttribute<int>( "HadronConeExclTruthLabelID", label );
  if(status){
    if ((label == 5) || (label == -5)){          return "B";
    } else if ((label == 4) || (label == -4)){   return "C";
    } else if ((label == 15) || (label == -15)){ return "T";
    } else { return "Light";}
  } else {
    ANA_MSG_WARNING( "HadronConeExclTruthLabelID was not found, using Light as jet flavour for retrieving b-tagging SF." );
    return "Light";
  }
}

