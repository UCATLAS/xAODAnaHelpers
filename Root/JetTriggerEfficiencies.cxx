// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include <xAODJet/JetContainer.h>
#include <xAODJet/JetAuxContainer.h>
#include <xAODCore/AuxContainerBase.h>
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"

// package include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/JetTriggerEfficiencies.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/TreeReader.h"

// external tools include(s):
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "JetJvtEfficiency/JetJvtEfficiency.h"
#include "JetMomentTools/JetForwardJvtTool.h"
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"
#include "TriggerMatchingTool/MatchingTool.h"

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"


// for reading in output of shell commands
#include <cstdio>
// #include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}



// this is needed to distribute the algorithm to the workers
ClassImp(JetTriggerEfficiencies)


JetTriggerEfficiencies :: JetTriggerEfficiencies () :
    Algorithm("JetTriggerEfficiencies")
{
}

EL::StatusCode JetTriggerEfficiencies :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_DEBUG( "Calling setupJob");

  if(!m_fromNTUP) {
    job.useXAOD ();
    xAOD::Init( "JetTriggerEfficiencies" ).ignore(); // call before opening first file
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_INFO( "Calling histInitialize");
  std::cout << "this is " << m_name << std::endl;
  ANA_CHECK( xAH::Algorithm::algInitialize());

  // if last character of name is a alphanumeric add a / so that
  // in the output file, a TDirectory is created with the histograms inside
  // if( isalnum( m_mainHistName.back() ) && !ispunct( m_mainHistName.back() ) ) {
    // m_mainHistName += "/";
  // }
  m_mainHistName = m_mainHistName + "_" + m_name + "/";

  // decode the turnon string
  // each turnon is of the form "probeTrigger/refTrigger" and they are split by "|"

  if(m_turnonString == "") {
    std::cout << "no turnons requested" << std::endl;
    return EL::StatusCode::FAILURE;
  }

  // strip out whitespace
  m_turnonString.erase(remove(m_turnonString.begin(), m_turnonString.end(), ' '), m_turnonString.end());

  // split by | into turnons
  std::vector<std::string> turnons = splitString(m_turnonString, "|");

  for (auto turnon : turnons) {
    // split by "/" to get the probe and reference
    std::vector<std::string> turnonParts = splitString(turnon, "/");

    // check there are exactly two entries
    if (turnonParts.size() != 2) {
      std::cout << "turnon string part " << turnon << " was not specified correctly - it should have a single '/' separating a reference and probe" << std::endl;
      for (auto part : turnonParts) {
        std::cout << "  " << part << std::endl;
      }
      return EL::StatusCode::FAILURE;
    }

    // fill the probe and reference vectors
    m_probeTriggers.push_back(turnonParts[0]);
    m_referenceTriggers.push_back(turnonParts[1]);
  }

  // set selections
  m_selectionString.erase(remove(m_selectionString.begin(), m_selectionString.end(), ' '), m_selectionString.end());
  // if only one, set the same for every trigger
  if (m_selectionString.find("|") == std::string::npos) {
    for(auto turnon : turnons) {
      m_selections.push_back(m_selectionString);
    }
  }
  else {
    m_selections = splitString(m_selectionString, "|");
  }
  if(m_selections.size() != m_probeTriggers.size()) {
    ANA_MSG_ERROR( "m_selections is a different length to m_probeTriggers! Examine m_selectionString and m_turnonString");
    return EL::StatusCode::FAILURE;
  }


  // set variables to plot on x axes of turnons
  m_variableString.erase(remove(m_variableString.begin(), m_variableString.end(), ' '), m_variableString.end());
  // if only one, set the same for every trigger
  if (m_variableString.find("|") == std::string::npos) {
    for(auto turnon : turnons) {
      m_variables.push_back(m_variableString);
    }
  }
  else {
    m_variables = splitString(m_variableString, "|");
  }
  if(m_variables.size() != m_probeTriggers.size()) {
    ANA_MSG_ERROR( "m_variables is a different length to m_probeTriggers! Examine m_variableString and m_turnonString");
    return EL::StatusCode::FAILURE;
  }
  

  // print the findings
  std::cout << "I am going to make " << m_probeTriggers.size() << " turnons:" << std::endl;
  for ( unsigned int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
    std::cout << "  " << m_probeTriggers[i_turnon] << " / " << m_referenceTriggers[i_turnon] << " with selection " << m_selections[i_turnon] << " and variable " << m_variables[i_turnon] << std::endl;
  }


  for ( unsigned int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
    std::cout << "getting info for " << m_probeTriggers[i_turnon] << " (" << m_referenceTriggers[i_turnon] << ")" << std::endl;

    // get trigger info from python script
    std::string fullJetTriggerInfoPath = PathResolverFindCalibDirectory(m_jetTriggerInfoPath);
    std::string command = "python " + fullJetTriggerInfoPath + "/get_trigger_info.py --trigger " + m_probeTriggers[i_turnon] + " --verbosity 0 --menuSet "+m_jetTriggerMenuSet;
    std::string result = exec(command.c_str());
    std::cout << result << std::endl;

    // parse the python string output into the JetTriggerInfo class
    JetTriggerInfo thisJetTriggerInfo;
    thisJetTriggerInfo.fillInfo(result, m_selections[i_turnon]);

    if(m_probeTriggers[i_turnon] != thisJetTriggerInfo.chainName) {
      if(m_requireTriggerInfoMatch) {
        ANA_MSG_ERROR("Asked for '" << m_probeTriggers[i_turnon] << "' but found '" << thisJetTriggerInfo.chainName << "'");
        return EL::StatusCode::FAILURE;
      }
      else {
        ANA_MSG_WARNING("Asked for '" << m_probeTriggers[i_turnon] << "' but found '" << thisJetTriggerInfo.chainName << "'");
        ANA_MSG_INFO("continuing anyway, things might go wrong");
      }
    }

    
    // set the index in m_variables[i_turnon], adjust multiplicity requirement if necessary
    if (m_variables[i_turnon]=="ht") {
      // don't do anything - there isn't a multiplicity involved
      m_variables_var.push_back(m_variables[i_turnon]);
    }
    else if (m_variables[i_turnon].find("[") == std::string::npos) {
      int mult = thisJetTriggerInfo.getMultiplicity();
      if(mult==-1) {
        ANA_MSG_ERROR( "failed to get multiplicity for " + m_probeTriggers[i_turnon] );
        return EL::StatusCode::FAILURE;
      }
      m_variables_var.push_back(m_variables[i_turnon]);
      m_variables_index.push_back(mult-1);
      m_variables[i_turnon] = m_variables[i_turnon] + "[" + std::to_string(mult-1) + "]";
    }
    else {
      int mult = std::stoi( splitString( splitString(m_variables[i_turnon],"[")[1], "]")[0] ) + 1;
      m_variables_var.push_back(splitString(m_variables[i_turnon],"[")[0]);
      m_variables_index.push_back(mult-1);
      bool succeeded = thisJetTriggerInfo.setMultiplicity(mult);
      if(!succeeded) {
        ANA_MSG_ERROR( "failed to set multiplicity for " + m_probeTriggers[i_turnon] );
        return EL::StatusCode::FAILURE;
      }
    }

    std::cout << "variable set to " << m_variables[i_turnon] << std::endl;

    m_probeTriggerInfo.push_back(thisJetTriggerInfo);
  }

  
  // parse the split selection info
  std::vector<std::string> splitValuesVec;
  if(m_splitBy != "") {
    ANA_MSG_INFO("decoding splitValuesStr " << m_splitValuesStr);
    // this should be in a format like "["1","2","3"]"
    splitValuesVec = splitListString(m_splitValuesStr);
    for(auto valStr : splitValuesVec ) {
      m_splitValues.push_back(std::stof(valStr));
    }
  }
  else {
    m_splitValues.push_back(-999);
    m_splitValues.push_back(999);
  }

  
  // create the histograms  
  for (unsigned int i_split = 0; i_split < m_splitValues.size()-1; i_split++) {

    std::string thisHistNameSplit = "";
    std::string thisHistXaxisSplit = "";
    if(m_splitBy != "") {
      thisHistNameSplit = splitValuesVec[i_split] + m_splitBy + splitValuesVec[i_split+1] + "/";
      thisHistXaxisSplit = ", " + splitValuesVec[i_split] + " #leq " + m_splitBy + " < " + splitValuesVec[i_split+1];
    }

    for ( unsigned int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
      std::string histname = m_probeTriggers[i_turnon]+"-"+m_referenceTriggers[i_turnon]+"_"+m_variables[i_turnon];
      std::string xaxistitle = m_offlineContainerName + " " + m_variables[i_turnon] + " " + m_probeTriggerInfo[i_turnon].offlineSelectionString + thisHistXaxisSplit;

      if(m_TDT) {
        m_numeratorHistsTDT.push_back( book(m_mainHistName+thisHistNameSplit, histname + "_numTDT", xaxistitle, 1000, 0, 1000, wk()) );
        m_denominatorHistsTDT.push_back( book(m_mainHistName+thisHistNameSplit, histname + "_denomTDT", xaxistitle, 1000, 0, 1000, wk()) );
      }
      if(m_emulate) {
        m_numeratorHistsEmulated.push_back( book(m_mainHistName+thisHistNameSplit, histname + "_numEmulated", xaxistitle, 1000, 0, 1000, wk()) );
        m_denominatorHistsEmulated.push_back( book(m_mainHistName+thisHistNameSplit, histname + "_denomEmulated", xaxistitle, 1000, 0, 1000, wk()) );
      }
    }
  }

  // have a set with no splitting, nosel
  if(m_splitBy != "") {
    for ( unsigned int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
      std::string histname = m_probeTriggers[i_turnon]+"-"+m_referenceTriggers[i_turnon]+"_"+m_variables[i_turnon];
      std::string xaxistitle = m_offlineContainerName + " " + m_variables[i_turnon] + " " + m_probeTriggerInfo[i_turnon].offlineSelectionString;

      if(m_TDT) {
        m_numeratorHistsTDT.push_back( book(m_mainHistName, histname + "_numTDT", xaxistitle, 1000, 0, 1000, wk()) );
        m_denominatorHistsTDT.push_back( book(m_mainHistName, histname + "_denomTDT", xaxistitle, 1000, 0, 1000, wk()) );
      }
      if(m_emulate) {
        m_numeratorHistsEmulated.push_back( book(m_mainHistName, histname + "_numEmulated", xaxistitle, 1000, 0, 1000, wk()) );
        m_denominatorHistsEmulated.push_back( book(m_mainHistName, histname + "_denomEmulated", xaxistitle, 1000, 0, 1000, wk()) );
      }
    }
    
  }
  
  
  // create histograms of the offline variable I'm splitting on
  if(m_splitBy != "") {
    std::string tdirname = "splitVar/";
    std::string hname = m_name+"_"+m_splitBy;
    std::string xaxistitle = m_splitBy;
    int nBins = 100;
    float xMin = 0;
    float xMax = 100;

    // if(m_split == "something") {
      // nBins = 0;
      // xMin = 0;
      // xMax = 0;
    // }

    m_splitVarHist = book(tdirname, hname , xaxistitle, nBins, xMin, xMax, wk());
  }


  // create histograms of the offline variables I'm selecting on
  if(m_plotSelectionVars) {
    ANA_MSG_INFO("defining histograms of selection variables");
    for ( unsigned int i_turnon = 0; i_turnon < m_probeTriggerInfo.size(); i_turnon++) {
      ANA_MSG_INFO("turnon " << i_turnon << " - " << m_probeTriggerInfo[i_turnon].chainName);

      std::vector<TH1F*> this_preSelHistsSelections;
      std::vector<TH1F*> this_postSelHistsSelections;
      std::vector<TH1F*> this_numeratorHistsSelectionsTDT;
      std::vector<TH1F*> this_denominatorHistsSelectionsTDT;
      std::vector<TH1F*> this_numeratorHistsSelectionsEmulated;
      std::vector<TH1F*> this_denominatorHistsSelectionsEmulated;

      // std::vector< std::pair<std::string, std::pair<float, float> > > extraSelection = decodeSelection(extraPart);
      for( unsigned int i_sel = 0; i_sel < m_probeTriggerInfo[i_turnon].offlineSelection.size(); i_sel++) {
        std::string sel_name = m_probeTriggerInfo[i_turnon].offlineSelection[i_sel].first;

        if(sel_name == "multiplicity")
          continue;

        float sel_low = m_probeTriggerInfo[i_turnon].offlineSelection[i_sel].second.first;
        float sel_high = m_probeTriggerInfo[i_turnon].offlineSelection[i_sel].second.second;

        ANA_MSG_INFO("  selection " << i_sel << " on " << sel_name << ", " << sel_low << " - " << sel_high);

        std::string tdirname = "selectionHists_" + m_name + "/";

        std::string sel_name_sanitised = sel_name;
        ReplaceAll(sel_name_sanitised, std::string("/"), std::string("-"));

        std::string histname = m_probeTriggers[i_turnon]+"-"+m_referenceTriggers[i_turnon]+"_"+m_variables[i_turnon]+"_"+sel_name_sanitised;
        std::string xaxistitle = m_offlineContainerName + " " + sel_name + " " + std::to_string(sel_low)+" - "+std::to_string(sel_high);

        int nbins = 1000;
        float low_x = 0.0;
        float high_x = 1000.0;
        if(sel_name == "eta") {
          nbins = 100;
          low_x = -5.0;
          high_x = 5.0;
        }
        else if(sel_name == "pt") {
          ;
        }
        else if(sel_name == "m") {
          nbins = 100;
          low_x = 0;
          high_x = 100;
        }
        else if(sel_name == "m/pt") {
          nbins = 200;
          low_x = 0;
          high_x = 2;
        }
        else {
          ANA_MSG_ERROR("Don't recgnise selection hist variable, you need to define histogram binning");
          return EL::StatusCode::FAILURE;
        }

        this_preSelHistsSelections.push_back( book(tdirname, histname + "_preSel", xaxistitle, nbins, low_x, high_x, wk()) );
        this_postSelHistsSelections.push_back( book(tdirname, histname + "_postSel", xaxistitle, nbins, low_x, high_x, wk()) );
        if(m_TDT) {
          this_numeratorHistsSelectionsTDT.push_back( book(tdirname, histname + "_numTDT", xaxistitle, nbins, low_x, high_x, wk()) );
          this_denominatorHistsSelectionsTDT.push_back( book(tdirname, histname + "_denomTDT", xaxistitle, nbins, low_x, high_x, wk()) );
        }
        if(m_emulate) {
          this_numeratorHistsSelectionsEmulated.push_back( book(tdirname, histname + "_numEmulated", xaxistitle, nbins, low_x, high_x, wk()) );
          this_denominatorHistsSelectionsEmulated.push_back( book(tdirname, histname + "_denomEmulated", xaxistitle, nbins, low_x, high_x, wk()) );
        }
      }

      m_preSelHistsSelections.push_back(this_preSelHistsSelections);
      m_postSelHistsSelections.push_back(this_postSelHistsSelections);
      m_numeratorHistsSelectionsTDT.push_back(this_numeratorHistsSelectionsTDT);
      m_denominatorHistsSelectionsTDT.push_back(this_denominatorHistsSelectionsTDT);
      m_numeratorHistsSelectionsEmulated.push_back(this_numeratorHistsSelectionsEmulated);
      m_denominatorHistsSelectionsEmulated.push_back(this_denominatorHistsSelectionsEmulated);
    }
  }

  ANA_MSG_INFO("histInitialise is done!");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  ANA_MSG_DEBUG( "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  ANA_MSG_DEBUG( "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_INFO( "Calling initialize");

  // don't have xaodEvent in NTUP
  if(!m_fromNTUP) {
    m_event = wk()->xaodEvent();
    m_store = wk()->xaodStore();
  }
  else {
    m_event = NULL; // set NULL for xAH::HelperFunctions::retrieve() (not that I use that currently)
  }
  
  if ( m_offlineContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  if(!m_fromNTUP){
    // Grab the TrigDecTool from the ToolStore
    if(!m_trigDecTool_handle.isUserConfigured()){
      ANA_MSG_FATAL("A configured " << m_trigDecTool_handle.typeAndName() << " must have been previously created! Double-check the name of the tool." );
      return EL::StatusCode::FAILURE;
    }
    ANA_CHECK( m_trigDecTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);
  }
  
  // get GRL tool
  if(m_applyGRLCut){
    std::vector<std::string> vecStringGRL;

    std::string grl;
    std::istringstream ss(m_GRLxml);
    while ( std::getline(ss, grl, ',') ) {
        std::string file = PathResolverFindCalibFile(grl);
        ANA_MSG_DEBUG("Found GRL: " << file);
        vecStringGRL.push_back(file);
    }

    ANA_CHECK( m_grl_handle.setProperty("GoodRunsListVec", vecStringGRL));
    ANA_CHECK( m_grl_handle.setProperty("PassThrough", false));
    ANA_CHECK( m_grl_handle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK( m_grl_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_grl_handle);
  }


  ANA_MSG_DEBUG( "JetTriggerEfficiencies Interface succesfully initialized!" );
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG( "Getting jet trigger efficiencies... " << m_name);
  
  

  // event info and MC weight
  const xAOD::EventInfo* eventInfo(nullptr);
  float mcEvtWeight(1.0);
  if(m_fromNTUP) {
    // global_eventInfo already defined
    // MC event weight
    // to implement
    ;
  }
  else {
    // retrieve event
    ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
    // MC event weight
    if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
      static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
      if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
        ANA_MSG_ERROR( "mcEventWeight is not available as decoration! Aborting" );
        return EL::StatusCode::FAILURE;
      }
      mcEvtWeight = mcEvtWeightAcc( *eventInfo );
    }
  }

  Long64_t eventNumber = m_fromNTUP ? global_eventInfo.eventNumber : eventInfo->eventNumber();
  int lumiBlock = m_fromNTUP ? global_eventInfo.lumiBlock : eventInfo->lumiBlock();
  int runNumber = m_fromNTUP ? global_eventInfo.runNumber : eventInfo->runNumber();
  ANA_MSG_DEBUG("retrieved event with eventNumber " << eventNumber << " from lumi block " << lumiBlock);


  // GRL
  if ( m_applyGRLCut ) {
    ANA_MSG_DEBUG("applying GRL");
    if ( !m_grl_handle->passRunLB( runNumber, lumiBlock ) ) {
      ANA_MSG_DEBUG("  failed GRL");
      return EL::StatusCode::SUCCESS;
    }
    else {
      ANA_MSG_DEBUG("  passed GRL");
    }
  }


  // offline jets
  const xAOD::JetContainer* offlineJets(nullptr);
  JetInfo offlineJetsInfo;

  if(m_fromNTUP) {
    ANA_CHECK( JetTriggerEfficiencies::retrieveJetInfo(offlineJetsInfo, m_offlineContainerName) );
  }
  else {
    ANA_CHECK( HelperFunctions::retrieve(offlineJets, m_offlineContainerName, m_event, m_store, msg()) );
  }

  ANA_MSG_DEBUG("got offline jets");


  // get split value
  float splitVal;
  if(m_splitBy == "") {
    // m_splitVals is {-999,999}
    splitVal = 0;
  }
  else if (m_splitBy == "avgIntPerX" || m_splitBy == "actIntPerX") {
    ANA_CHECK (this->get_event_variable(splitVal, m_splitBy, eventInfo, global_eventInfo, m_fromNTUP) );
  }
  else if (m_splitBy == "phi") {
    std::vector<float> thisvar_vec;
    ANA_CHECK (this->get_variable(thisvar_vec, "phi", offlineJets, offlineJetsInfo, m_fromNTUP) );
    if(thisvar_vec.size() == 0) 
      splitVal = -9;
    else
      splitVal = thisvar_vec.at(0);
  }
  else {
    ANA_MSG_ERROR("I don't know what to do with m_splitBy = " << m_splitBy);
  }

  if(m_splitBy != "")
    m_splitVarHist->Fill(splitVal);
  
  ANA_MSG_DEBUG("the value of " << m_splitBy << " is " << splitVal);
  

  // get list of disabled triggers from AOD
  std::vector<std::string> disabledTriggers;
  if(!m_fromNTUP) {
    static SG::AuxElement::ConstAccessor< std::vector< std::string > > acc_disabledTriggers("disabledTriggers");
    if( acc_disabledTriggers.isAvailable( *eventInfo ) ) {
      disabledTriggers = acc_disabledTriggers( *eventInfo );
    }
    else {
      ANA_MSG_WARNING("cannot access disabledTriggers in eventInfo, need this for accurate trigger efficiencies");
    }
  }
  
  
  // iterate over turnons
  for ( unsigned int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
    ANA_MSG_DEBUG("turnon " << i_turnon);

    // get the reference trigger info
    std::string refChainName = m_referenceTriggers[i_turnon];
    bool passedRefTrigger;
    if(m_fromNTUP) {
      passedRefTrigger = (std::find(global_eventInfo.passedTriggers->begin(), global_eventInfo.passedTriggers->end(), refChainName) != global_eventInfo.passedTriggers->end());
    }
    else {
      passedRefTrigger = m_trigDecTool_handle->isPassed(refChainName);
    }

    ANA_MSG_DEBUG("got ref trigger info for " << refChainName);

    // no point doing anything else if the reference trigger failed
    if(!passedRefTrigger) {
      ANA_MSG_DEBUG("failed ref trigger " << refChainName);
      continue;
    }

    ANA_MSG_DEBUG("passed ref trigger " << refChainName);
    
    // get the probe trigger info
    TriggerDecision probeDecision;
    if(m_TDT) {
      std::string probeChainName = m_probeTriggers[i_turnon];
      ANA_MSG_DEBUG("getting TDT probe info for " << probeChainName);
      if(m_fromNTUP) {
        probeDecision.passedTrigger = (std::find(global_eventInfo.passedTriggers->begin(), global_eventInfo.passedTriggers->end(), probeChainName) != global_eventInfo.passedTriggers->end());
        auto index = std::find(global_eventInfo.isPassBitsNames->begin(), global_eventInfo.isPassBitsNames->end(), probeChainName);
        if (index == global_eventInfo.isPassBitsNames->end()) {
          ANA_MSG_ERROR("failed to find trigger bits for " + probeChainName);
          return EL::StatusCode::FAILURE;
        }
        int atIndex = index - global_eventInfo.isPassBitsNames->begin();
        probeDecision.fillFromBits(global_eventInfo.isPassBits->at(atIndex));

        if(global_eventInfo.disabledTriggers == NULL) {
          if(m_acceptNoDisabled) {
            probeDecision.isDisabled = false;
          }
          else {
            ANA_MSG_ERROR("failed to find trigger bits for " + probeChainName);
            return EL::StatusCode::FAILURE;            
          }
        }
        else {
          probeDecision.isDisabled = (std::find(global_eventInfo.disabledTriggers->begin(), global_eventInfo.disabledTriggers->end(), probeChainName) != global_eventInfo.disabledTriggers->end());
        }
      }
      else {
        probeDecision.passedTrigger = m_trigDecTool_handle->isPassed(probeChainName);        

        const unsigned int probeBits = m_trigDecTool_handle->isPassedBits(probeChainName);
        probeDecision.fillFromBits(probeBits);

        probeDecision.isDisabled = (std::find(disabledTriggers.begin(), disabledTriggers.end(), probeChainName) != disabledTriggers.end());
        
      }
    }

    // emulate trigger
    TriggerDecision probeDecisionEmulated;
    if(m_emulate) {
      ANA_MSG_DEBUG("emulating trigger decisions");
      this->emulateTriggerDecision(m_probeTriggerInfo[i_turnon], probeDecisionEmulated);
      ANA_MSG_DEBUG("emulation done");
    }
    
    // if do them both, check agreement
    if(m_TDT && m_emulate && !m_skipCompare) {
      ANA_MSG_DEBUG("comparing emulation and trigger decision tool");
      if(probeDecisionEmulated.passedTrigger != probeDecision.passedTrigger) {
        // if TDT failed, maybe it's because of prescales. Only worry about it if it is not - ie the trigger fails but was not prescaled out at L1 or HLT, and was not disabled
        if( probeDecision.passedTrigger || (!probeDecision.passedTrigger && probeDecision.L1_isPassedAfterVeto && !probeDecision.HLT_isPrescaledOut && !probeDecision.isDisabled)) {
          ANA_MSG_WARNING("emulation and TDT disagree for " + m_probeTriggers[i_turnon] + " in event " << eventNumber << " from lumi block " << lumiBlock);
          std::cout << "  emulated passed? " << probeDecisionEmulated.passedTrigger << std::endl;
          std::cout << "  TDT passed?      " << probeDecision.passedTrigger << std::endl;
        }
      }
      else {
        ANA_MSG_DEBUG("they agree for " << m_probeTriggers[i_turnon] << " in event " << eventNumber << " from lumi block " << lumiBlock << " - both " << probeDecision.passedTrigger);
      }
    }
    

    // only fill denominator hist if the L1 of the HLT passed and it was not prescaled out
    // need to do something different if it's an L1 turnon
    if(!m_emulate) {
      if(!probeDecision.L1_isPassedAfterVeto || probeDecision.HLT_isPrescaledOut)
        continue;
    }

    int jet_index = -1;
    if(m_plotSelectionVars) {
      jet_index = m_variables_index[i_turnon];
      ANA_CHECK( this->FillSelectionVarHists(m_probeTriggerInfo[i_turnon], offlineJets, offlineJetsInfo, m_preSelHistsSelections[i_turnon], jet_index) );
    }

    // apply selections
    ANA_MSG_DEBUG("applying offline selections");
    int multiplicity_required = m_probeTriggerInfo[i_turnon].getMultiplicity();
    std::vector<int> good_indices;
    bool passSelections;
    ANA_CHECK( this->applySelections(passSelections, m_probeTriggerInfo[i_turnon].offlineSelection, offlineJets, offlineJetsInfo, multiplicity_required, good_indices) );


    if (!passSelections) {
      ANA_MSG_DEBUG("failed selection");
      continue;
    }
    ANA_MSG_DEBUG("passed selection");

    if(good_indices.size() <= m_variables_index[i_turnon]) {
      ANA_MSG_ERROR("I am looking for index " << m_variables_index[i_turnon] << " of good_indices with size " << good_indices.size());
      ANA_MSG_ERROR("This shouldn't be possible now I've passed selections");
      return EL::StatusCode::FAILURE;
    }
    jet_index = good_indices[m_variables_index[i_turnon]];    

    if(m_plotSelectionVars) {
      ANA_CHECK( this->FillSelectionVarHists(m_probeTriggerInfo[i_turnon], offlineJets, offlineJetsInfo, m_postSelHistsSelections[i_turnon], jet_index) );
    }

    // get the relevant variable
    ANA_MSG_DEBUG("getting variable to fill turnon with");
    std::string variable = m_variables_var[i_turnon];
    std::vector<float> var_vec;
    ANA_CHECK (this->get_variable(var_vec, variable, offlineJets, offlineJetsInfo, m_fromNTUP) );

    // order by variable - eg for mass
    if(m_orderByVariable) {
      std::sort(var_vec.begin(), var_vec.end());
    }

    float var_to_fill = var_vec[jet_index];



    ANA_MSG_DEBUG("set variable to fill turnon with");

    
    // iterate over split selection
    ANA_MSG_DEBUG("iterating over split selections");
    for (unsigned int i_split = 0; i_split < m_splitValues.size()-1; i_split++) {

      ANA_MSG_DEBUG("  this is split " << i_split);

      // used to be:
      // int histNum = i_split*m_probeTriggers.size() + i_turnon;
      // then I added an extra one on the end for each probeTrigger
      // this is like i_split = m_splitVaues().size() - 1

      int histNum = i_split*m_probeTriggers.size() + i_turnon;
      int noSelHistNum = (m_splitValues.size()-1)*m_probeTriggers.size() + i_turnon;
      
      if(splitVal < m_splitValues[i_split] || splitVal >= m_splitValues[i_split+1]) {
        ANA_MSG_DEBUG("  failed split " << i_split);
        continue;
      }
      ANA_MSG_DEBUG("  passed split " << i_split);

      // fill hists
      // TDT needs to account for probe L1 and prescale, and whether trigger is disabled
      if(m_TDT) {
        bool goodForDenominator = false;
        bool goodForNumerator = false;
        if(m_probeTriggerInfo[i_turnon].isL1) {
          goodForDenominator = passedRefTrigger;
          goodForNumerator = probeDecision.L1_isPassedBeforePrescale;
        }
        else {
          goodForDenominator = passedRefTrigger && probeDecision.L1_isPassedAfterVeto && !probeDecision.HLT_isPrescaledOut && !probeDecision.isDisabled;
          goodForNumerator = probeDecision.passedTrigger;
        }


        if(goodForDenominator) {
          m_denominatorHistsTDT.at(histNum)->Fill(var_to_fill);
          if(m_plotSelectionVars) {
            ANA_MSG_DEBUG("about to fill selection hist (passed ref and relevant prescale / disabled) for TDT");
            ANA_CHECK( this->FillSelectionVarHists(m_probeTriggerInfo[i_turnon], offlineJets, offlineJetsInfo, m_denominatorHistsSelectionsTDT[i_turnon], jet_index) );
          }
          if(m_splitBy != "") {
            m_denominatorHistsTDT.at(noSelHistNum)->Fill(var_to_fill);
          }
          ANA_MSG_DEBUG("filling reference for " << m_probeTriggerInfo[i_turnon].chainName << ", is it disabled? " << probeDecision.isDisabled);
          if(goodForNumerator){
            // std::cout << m_probeTriggerInfo[i_turnon].chainName << " passed with ref " << refChainName << std::endl;
            // std::cout << "passedTrigger? " << probeDecision.passedTrigger << std::endl;
            ANA_MSG_DEBUG("   it passed!");
            m_numeratorHistsTDT.at(histNum)->Fill(var_to_fill);
            if(m_plotSelectionVars) {
              ANA_MSG_DEBUG("about to fill selection hist (passed ref and probe) for TDT");
              ANA_CHECK( this->FillSelectionVarHists(m_probeTriggerInfo[i_turnon], offlineJets, offlineJetsInfo, m_numeratorHistsSelectionsTDT[i_turnon], jet_index) );
            }
            if(m_splitBy != "") {
              m_numeratorHistsTDT.at(noSelHistNum)->Fill(var_to_fill);
            }
          }
        }
      }
      // emulated doesn't need to worry about probe L1 or prescale
      // currently not emulating L1 in order to compare, add as option?
      if(m_emulate) {
        m_denominatorHistsEmulated.at(histNum)->Fill(var_to_fill);
        if(m_plotSelectionVars) {
          ANA_MSG_DEBUG("about to fill selection hist (passed ref and relevant prescale / disabled) for emulation");
          ANA_CHECK( this->FillSelectionVarHists(m_probeTriggerInfo[i_turnon], offlineJets, offlineJetsInfo, m_denominatorHistsSelectionsEmulated[i_turnon], jet_index) );
        }
        if(m_splitBy != "") {
          m_denominatorHistsEmulated.at(noSelHistNum)->Fill(var_to_fill);
        }
        if(probeDecisionEmulated.passedTrigger){
          m_numeratorHistsEmulated.at(histNum)->Fill(var_to_fill);
          if(m_plotSelectionVars) {
            ANA_MSG_DEBUG("about to fill selection hist (passed ref and probe) for emulation");
            ANA_CHECK( this->FillSelectionVarHists(m_probeTriggerInfo[i_turnon], offlineJets, offlineJetsInfo, m_numeratorHistsSelectionsEmulated[i_turnon], jet_index) );
          }
          if(m_splitBy != "") {
            m_numeratorHistsEmulated.at(noSelHistNum)->Fill(var_to_fill);
          }
        }
      }
      ANA_MSG_DEBUG("  done with split " << i_split);
    }
    ANA_MSG_DEBUG("finished turnon " << i_turnon);
  }

  ANA_MSG_DEBUG( "Leaving jet trigger efficiencies... ");

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode JetTriggerEfficiencies :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: finalize ()
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

  ANA_MSG_DEBUG( m_name );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: histFinalize ()
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

  ANA_MSG_DEBUG( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}


// the following borrowed and modified from xAH::HistogramManager because I am bad at c++ and have no idea how to use it otherwise

TH1F* JetTriggerEfficiencies::book(std::string name, std::string title, std::string xlabel,
                                   int xbins, double xlow, double xhigh, EL::Worker* wk)
{
  TH1F* tmp = new TH1F( (name + title).c_str(), (title+";"+xlabel).c_str(), xbins, xlow, xhigh);
  tmp->Sumw2();
  wk->addOutput(tmp);
  return tmp;
}





EL::StatusCode JetTriggerEfficiencies :: applySelections(bool &passSelections, std::vector< std::pair<std::string, std::pair<float, float> > > selections, const xAOD::JetContainer* jets, JetInfo &jetsInfo, unsigned int multiplicity_required, std::vector<int> &good_indices, bool isHLTpresel) {
  
  passSelections = false;

  unsigned int nJets = m_fromNTUP ? jetsInfo.pt->size() : jets->size();
  for(unsigned int i=0; i<nJets; i++){
    good_indices.push_back(i);
  }
  ANA_MSG_DEBUG("applying selections on " << nJets << " jets");
  
  for(auto selection : selections) {
    // if already failed, don't waste time calculating things
    if(good_indices.size() < multiplicity_required)
      return EL::StatusCode::SUCCESS;
    
    // get the indices that pass this selection
    std::vector<int> passed_indices;
    ANA_MSG_DEBUG("about to apply selection on " << selection.first);
    ANA_CHECK( this->applySelection(passed_indices, selection, jets, jetsInfo, good_indices, isHLTpresel) );
    ANA_MSG_DEBUG("applied selection");

    // carry forward only the good ones
    good_indices = passed_indices;
  }
  
  if(good_indices.size() < multiplicity_required)
    return EL::StatusCode::SUCCESS;
  
  passSelections = true;
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetTriggerEfficiencies :: applySelection(std::vector<int> &passed_indices, std::pair<std::string, std::pair<float, float> > selection, const xAOD::JetContainer* jets, JetInfo &jetsInfo, std::vector<int> good_indices, bool isHLTpresel) {

  // ignore multiplicity
  if(selection.first == "multiplicity") {
    passed_indices = good_indices;
    return EL::StatusCode::SUCCESS;
  }

  // if preselection, ignore ET, if not ignore ET_preselection
  if(isHLTpresel) {
    if(selection.first == "ET") {
      passed_indices = good_indices;
      return EL::StatusCode::SUCCESS;
    }
  }
  else {
    if(selection.first == "ET_preselection") {
      passed_indices = good_indices;
      return EL::StatusCode::SUCCESS;
    }
  }

  ANA_MSG_DEBUG("about to apply an actual selection");

  // get variable
  std::vector<float> thisvar_vec;
  ANA_CHECK (this->get_variable(thisvar_vec, selection.first, jets, jetsInfo, m_fromNTUP) );
 
 // eta
  if(selection.first == "eta") {
    for(auto index : good_indices) {
      if( fabs(thisvar_vec.at(index)) > selection.second.first && fabs(thisvar_vec.at(index)) < selection.second.second )
        passed_indices.push_back(index);
    }
  }
  
  // m, pt, ET
  else if(selection.first == "m" || selection.first == "singlejetmass" || selection.first == "pt" || selection.first == "ET" || selection.first == "ET_preselection" || selection.first == "m/pt") {
    for(auto index : good_indices) {
      if( thisvar_vec.at(index) > selection.second.first)
        passed_indices.push_back(index);
    }
  }
  
  // else complain
  else {
    ANA_MSG_ERROR("do not recognise selection " + selection.first + " - not applying");
    passed_indices = good_indices;
    return EL::StatusCode::SUCCESS;
  }
  
  return EL::StatusCode::SUCCESS;
  
}



EL::StatusCode JetTriggerEfficiencies :: emulateTriggerDecision(JetTriggerInfo &triggerInfo, TriggerDecision &triggerDecision) {

  int multiplicity_required = triggerInfo.getMultiplicity();
  bool passed = true;

  // check for preselection
  if(triggerInfo.HLTjetContainerPreselection != "") {

    // define empty things
    std::vector<int> good_indices;
    bool passSelections = false;
    const xAOD::JetContainer* HLTpreselJets(nullptr);
    JetInfo HLTpreselJetsInfo;

    // if from NTUP, get right JetsInfo from global vector
    if(m_fromNTUP) {
      ANA_CHECK( JetTriggerEfficiencies::retrieveJetInfo(HLTpreselJetsInfo, triggerInfo.HLTjetContainerPreselection) );
    }
    // if not, retrieve xAOD jet container
    else {
      ANA_CHECK( HelperFunctions::retrieve(HLTpreselJets, triggerInfo.HLTjetContainerPreselection, m_event, m_store, msg()) );
    }

    ANA_MSG_DEBUG("about to apply emulation preselections");
    ANA_CHECK( this->applySelections(passSelections, triggerInfo.HLTselection, HLTpreselJets, HLTpreselJetsInfo, multiplicity_required, good_indices, true) ); // isHLTpresel=true

    if (!passSelections)
      passed = false;
  }

  // main HLT selection
  const xAOD::JetContainer* HLTjets(nullptr);
  JetInfo HLTjetsInfo;
  ANA_MSG_DEBUG("about to retrieve HLT jet collection");
  if(m_fromNTUP) {
    ANA_CHECK( JetTriggerEfficiencies::retrieveJetInfo(HLTjetsInfo, triggerInfo.HLTjetContainer) );
    if(HLTjetsInfo.pt->size() > 0) {
      ANA_MSG_DEBUG("lead ntup jet 4-vec and ET: (" << HLTjetsInfo.pt->at(0) << ", " << HLTjetsInfo.eta->at(0) << ", " << HLTjetsInfo.phi->at(0) << ", " << HLTjetsInfo.E->at(0) << ") " << HLTjetsInfo.TLV(0).Et() );
    }
  }
  else {
    ANA_CHECK( HelperFunctions::retrieve(HLTjets, triggerInfo.HLTjetContainer, m_event, m_store, msg()) );
    if(HLTjets->size() > 0) {
      ANA_MSG_DEBUG("lead xaod jet 4-vec and ET: (" << HLTjets->at(0)->pt()/1000. << ", " << HLTjets->at(0)->eta() << ", " << HLTjets->at(0)->phi() << ", " << HLTjets->at(0)->p4().E()/1000. << ") " << HLTjets->at(0)->p4().Et()/1000. );
    }
  }
  
  std::vector<int> good_indices;
  bool passSelections = false;
  ANA_MSG_DEBUG("about to apply emulation selections");
  ANA_CHECK( this->applySelections(passSelections, triggerInfo.HLTselection, HLTjets, HLTjetsInfo, multiplicity_required, good_indices, false) ); // isHLTpresel=false
  if (!passSelections)
    passed = false;
  

  triggerDecision.passedTrigger = passed;

  return EL::StatusCode::SUCCESS;

}


EL::StatusCode JetTriggerEfficiencies::retrieveJetInfo(JetInfo &jetInfo, std::string jetCollectionName) {
  bool found = false;
  for(auto jetCollectionInfo : global_jetCollectionInfos) {
    if(jetCollectionInfo.xAODname == jetCollectionName) {
      jetInfo = jetCollectionInfo;
      found = true;
      break;
    }
  }
  if(!found) {
    ANA_MSG_ERROR("do not have necessary jet collection, xAODname " << jetCollectionName);
    return EL::StatusCode::FAILURE;
  }
  else {
    ANA_MSG_DEBUG("retrieved " << jetCollectionName << " from ntup");
    return EL::StatusCode::SUCCESS;
  }
}



EL::StatusCode JetTriggerEfficiencies::get_variable(std::vector<float> &var_vec, std::string varName, const xAOD::JetContainer* jets, JetInfo &jetsInfo, bool fromNTUP) {

  ANA_MSG_DEBUG("get_variable: getting variable " << varName);

  unsigned int nJets = fromNTUP ? jetsInfo.pt->size() : jets->size();

  for( unsigned int index = 0; index < nJets; index++ ) {
    if(varName == "pt") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.pt->at(index));
      else
        var_vec.push_back(jets->at(index)->pt()/1000.);
    }

    else if(varName == "eta") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.eta->at(index));
      else
        var_vec.push_back(jets->at(index)->eta());
    }

    else if(varName == "phi") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.phi->at(index));
      else
        var_vec.push_back(jets->at(index)->phi());
    }

    else if(varName == "m" || varName == "singlejetmass") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.TLV(index).M());
      else
        var_vec.push_back(jets->at(index)->m()/1000.);
    }

    else if(varName == "ET" || varName == "ET_preselection") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.TLV(index).Et());
      else
        var_vec.push_back(jets->at(index)->p4().Et()/1000.);
    }

    else if(varName == "m/pt") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.TLV(index).M() / jetsInfo.pt->at(index));
      else
        var_vec.push_back(jets->at(index)->m() / jets->at(index)->pt());
    }

    else {
      ANA_MSG_ERROR("do not recognise variable " + varName);
      return EL::StatusCode::FAILURE;
    }
 
  }

  ANA_MSG_DEBUG("  successfully got");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies::get_event_variable(float &var, std::string varName, const xAOD::EventInfo* eventInfo_xAOD, EventInfo &eventInfo_ntup, bool fromNTUP) {

  ANA_MSG_DEBUG("getting event variable " << varName);

  if(varName == "avgIntPerX") {
    if(fromNTUP)
      var = eventInfo_ntup.avgIntPerX;
    else
      var = eventInfo_xAOD->averageInteractionsPerCrossing();
  }

  else if(varName == "actIntPerX") {
    if(fromNTUP)
      var = eventInfo_ntup.actIntPerX;
    else
      var = eventInfo_xAOD->actualInteractionsPerCrossing();
  }
  
  else {
    ANA_MSG_ERROR("do not recognise variable " + varName);
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;

}



EL::StatusCode JetTriggerEfficiencies::FillSelectionVarHists(JetTriggerInfo &probeTriggerInfo, const xAOD::JetContainer* jets, JetInfo &jetsInfo, std::vector<TH1F*> &histsVec, int jet_index) {

  ANA_MSG_DEBUG("FillSelectionVars: filling hists for " << probeTriggerInfo.chainName);
  
  int offset = 0;
  for(unsigned int i_sel = 0; i_sel < probeTriggerInfo.offlineSelection.size(); i_sel++){

    std::string variable = probeTriggerInfo.offlineSelection[i_sel].first;
    
    if(variable == "multiplicity") {
      offset += 1;
      continue;
    }

    ANA_MSG_DEBUG("  selection " << i_sel << " on " << variable);

    std::vector<float> var_vec;
    ANA_CHECK (this->get_variable(var_vec, variable, jets, jetsInfo, m_fromNTUP) );

    if(var_vec.size() <= jet_index) {
      ANA_MSG_DEBUG("  FillSelectionVars: require jet index " << jet_index << " but only have " << var_vec.size());
      continue;
    }

    float var_to_fill = var_vec[jet_index];

    ANA_MSG_DEBUG("  nth = " << jet_index << " jet has value " << var_to_fill);

    if(i_sel-offset >= histsVec.size()) {
      ANA_MSG_ERROR("  histsVec has size " << histsVec.size() << " but trying to fill index " << i_sel-offset);
      return EL::StatusCode::FAILURE;
    }
    
    ANA_MSG_DEBUG("  about to fill hist " << variable << "(number " << i_sel - offset << " of " << histsVec.size() << ") with " << var_to_fill);
    histsVec[i_sel-offset]->Fill(var_to_fill);
    ANA_MSG_DEBUG("  successfully filled hist for " << variable);
  }

  ANA_MSG_DEBUG("leaving FillSelectionVarHists successfully");
  return EL::StatusCode::SUCCESS;
}





std::vector<std::string> splitString(std::string parentString, std::string sep) {
  std::size_t start = 0, end = 0;
  std::vector<std::string> splitVec;
  while ((end = parentString.find(sep, start)) != std::string::npos) {
    splitVec.push_back(parentString.substr(start, end - start));
    start = end + sep.size();
  }
  splitVec.push_back(parentString.substr(start));
  return splitVec;
}


// decodes a python list
std::vector<std::string> splitListString(std::string parentString) {
  // step 1: strip whitespace
  std::string tempString = regex_replace(parentString, std::regex("^ +| +$|( ) +"), "$1");

  // return if empty
  if(tempString.size() == 0) {
    std::vector<std::string> splitVec;
    return splitVec;
  }

  // step 2: remove [ and ] (first and last, given whitespace stripping)
  tempString = tempString.substr(1,tempString.size()-2);
  
  // step 3: split into vector of strings
  std::vector<std::string> splitVec = splitString(tempString, ",");

  // step 4: remove quotation marks and whitespace
  for(unsigned int i = 0; i<splitVec.size(); i++) {
    splitVec[i] = regex_replace(splitVec[i], std::regex("^ +| +$|( ) +"), "$1");
    splitVec[i] = splitVec[i].substr(1,splitVec[i].size()-2);
  }

  return splitVec;
}


// https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
static inline void ReplaceAll(std::string &str, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }
}
