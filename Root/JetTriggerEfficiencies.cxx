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
  if( isalnum( m_mainHistName.back() ) && !ispunct( m_mainHistName.back() ) ) {
    m_mainHistName += "/";
  }

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


    // set the index in m_variables[i_turnon], adjust multiplicity reuirement if necessary
    if (m_variables[i_turnon].find("[") == std::string::npos) {
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

  std::cout << "histInitialise is done!" << std::endl;

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
    m_event = NULL; // set NULL for xAH::HelperFunctions::retrieve()
  }

  ANA_MSG_INFO( "Have store");
  
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
  ANA_MSG_DEBUG("retrieved event with eventNumber " << eventNumber);

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
  else if (m_splitBy == "avgIntPerX") {
    ANA_CHECK (this->get_event_variable(splitVal, m_splitBy, eventInfo, global_eventInfo, m_fromNTUP) );
  }
  else {
    ANA_MSG_ERROR("I don't know what to do with m_splitBy = " << m_splitBy);
  }
  
  std::cout << "the value of " << m_splitBy << " is " << splitVal << std::endl;
  
  
  
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
    if(!passedRefTrigger)
      continue;

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
      }
      else {
        probeDecision.passedTrigger = m_trigDecTool_handle->isPassed(probeChainName);
        const unsigned int probeBits = m_trigDecTool_handle->isPassedBits(probeChainName);
        probeDecision.fillFromBits(probeBits);
      }
    }

    // emulate trigger
    TriggerDecision probeDecisionEmulated;
    if(m_emulate) {
      ANA_MSG_DEBUG("emulating trigger decisions");
      this->emulateTriggerDecision(m_probeTriggerInfo[i_turnon], probeDecisionEmulated);
      ANA_MSG_DEBUG("emulation done");
    }



    if(m_TDT) {
      if(probeDecisionEmulated.passedTrigger != probeDecision.passedTrigger) {
        // if TDT failed, maybe it's because of prescales. Only worry about it if it is not - ie the trigger fails but was not prescaled out at L1 or HLT
        if( probeDecision.passedTrigger || (!probeDecision.passedTrigger && probeDecision.L1_isPassedAfterVeto && !probeDecision.HLT_isPrescaledOut)) {

          ANA_MSG_WARNING("emulation and TDT disagree for " + m_probeTriggers[i_turnon] + " in event " << eventNumber);
          std::cout << "  emulated passed? " << probeDecisionEmulated.passedTrigger << std::endl;
          std::cout << "  TDT passed?      " << probeDecision.passedTrigger << std::endl;
        }
      }
      else {
        ANA_MSG_DEBUG("they agree for " << m_probeTriggers[i_turnon] << " in event " << eventNumber << " - both " << probeDecision.passedTrigger);
      }
    }
    

    // only fill denominator hist if the L1 of the HLT passed and it was not prescaled out
    // need to do something different if it's an L1 turnon
    if(!m_emulate) {
      if(!probeDecision.L1_isPassedAfterVeto || probeDecision.HLT_isPrescaledOut)
        continue;
    }


    // apply selections
    ANA_MSG_DEBUG("applying offline selections");
    int multiplicity_required = m_probeTriggerInfo[i_turnon].getMultiplicity();
    std::vector<int> good_indices;
    bool passSelections;
    ANA_CHECK( this->applySelections(passSelections, m_probeTriggerInfo[i_turnon].offlineSelection, offlineJets, offlineJetsInfo, multiplicity_required, good_indices) );
    if (!passSelections)
      continue;


    // get the relevant variable
    int index = m_variables_index[i_turnon];
    std::string variable = m_variables_var[i_turnon];
    std::vector<float> var_vec;
    ANA_CHECK (this->get_variable(var_vec, variable, offlineJets, offlineJetsInfo, m_fromNTUP) );
    float var_to_fill = var_vec[index];
    

    
    // iterate over selections
    for (unsigned int i_split = 0; i_split < m_splitValues.size()-1; i_split++) {

      int histNum = i_split*m_probeTriggers.size() + i_turnon;

      if(splitVal < m_splitValues[i_split] || splitVal >= m_splitValues[i_split+1])
        continue;

      // fill hists
      // TDT needs to account for probe L1 and prescale
      if(m_TDT) {
        if(probeDecision.L1_isPassedAfterVeto && !probeDecision.HLT_isPrescaledOut) {
          m_denominatorHistsTDT.at(histNum)->Fill(var_to_fill);
          if(probeDecision.passedTrigger){
            m_numeratorHistsTDT.at(histNum)->Fill(var_to_fill);
          }
        }
      }
      // emulated doesn't need to worry about probe L1 or prescale
      // currently not emulating L1 in order to compare, add as option?
      if(m_emulate) {
        m_denominatorHistsEmulated.at(histNum)->Fill(var_to_fill);
        if(probeDecisionEmulated.passedTrigger){
          m_numeratorHistsEmulated.at(histNum)->Fill(var_to_fill);
        }
      }
    }
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
  else if(selection.first == "m" || selection.first == "pt" || selection.first == "ET" || selection.first == "ET_preselection") {
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
  if(m_fromNTUP) {
    ANA_CHECK( JetTriggerEfficiencies::retrieveJetInfo(HLTjetsInfo, triggerInfo.HLTjetContainer) );
    // ANA_MSG_DEBUG("lead ntup jet 4-vec and ET: (" << HLTjetsInfo.pt->at(0) << ", " << HLTjetsInfo.eta->at(0) << ", " << HLTjetsInfo.phi->at(0) << ", " << HLTjetsInfo.E->at(0) << ") " << HLTjetsInfo.tlv.at(0).Et() << "now tlv pt = "  << HLTjetsInfo.tlv.at(0).Pt() << " new tlv " << HLTjetsInfo.TLV(0).Et());
    ANA_MSG_DEBUG("lead ntup jet 4-vec and ET: (" << HLTjetsInfo.pt->at(0) << ", " << HLTjetsInfo.eta->at(0) << ", " << HLTjetsInfo.phi->at(0) << ", " << HLTjetsInfo.E->at(0) << ") " << HLTjetsInfo.TLV(0).Et() );
    ANA_MSG_DEBUG("em const pu etaJES gsc insit " << HLTjetsInfo.emScalePt->at(0) << ", " << HLTjetsInfo.constScalePt->at(0) << ", " << HLTjetsInfo.pileupScalePt->at(0) << ", " << HLTjetsInfo.etaJESScalePt->at(0) << ", " << HLTjetsInfo.gscScalePt->at(0) << ", " << HLTjetsInfo.insituScalePt->at(0) ); 
  }
  else {
    ANA_CHECK( HelperFunctions::retrieve(HLTjets, triggerInfo.HLTjetContainer, m_event, m_store, msg()) );
    ANA_MSG_DEBUG("lead xaod jet 4-vec and ET: (" << HLTjets->at(0)->pt()/1000. << ", " << HLTjets->at(0)->eta() << ", " << HLTjets->at(0)->phi() << ", " << HLTjets->at(0)->p4().E()/1000. << ") " << HLTjets->at(0)->p4().Et()/1000. );
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

  ANA_MSG_DEBUG("getting variable " << varName);

  unsigned int nJets = fromNTUP ? jetsInfo.pt->size() : jets->size();

  for( unsigned int index = 0; index < nJets; index++ ) {
    if(varName == "eta") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.eta->at(index));
      else
        var_vec.push_back(jets->at(index)->eta());
    }

    else if(varName == "pt") {
      if(fromNTUP)
        var_vec.push_back(jetsInfo.pt->at(index));
      else
        var_vec.push_back(jets->at(index)->pt()/1000.);
    }

    else if(varName == "m") {
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

    else {
      ANA_MSG_ERROR("do not recognise variable " + varName);
      return EL::StatusCode::FAILURE;
    }
 
  }

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
  
  else {
    ANA_MSG_ERROR("do not recognise variable " + varName);
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;

}
