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

  job.useXAOD ();
  xAOD::Init( "JetTriggerEfficiencies" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetTriggerEfficiencies :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_DEBUG( "Calling histInitialize");
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
  for ( int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
    std::cout << "  " << m_probeTriggers[i_turnon] << " / " << m_referenceTriggers[i_turnon] << " with selection " << m_selections[i_turnon] << " and variable " << m_variables[i_turnon] << std::endl;
  }


  std::string jetTriggerInfoPath = "/afs/cern.ch/user/c/ckaldero/trigger/useful-scripts/menu";
  for ( int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
    std::cout << "getting info for " << m_probeTriggers[i_turnon] << std::endl;

    // get trigger info from python script
    std::string command = "python " + jetTriggerInfoPath + "/get_trigger_info.py --trigger " + m_probeTriggers[i_turnon] + " --verbosity 0";
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


  
  // create the histograms

  for ( int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {
    std::string histname = m_probeTriggers[i_turnon]+"-"+m_referenceTriggers[i_turnon]+"_"+m_variables[i_turnon];
    std::string xaxistitle = m_inContainerName + " " + m_variables[i_turnon] + " " + m_probeTriggerInfo[i_turnon].offlineSelectionString;

    m_numeratorHists.push_back( book(m_mainHistName, histname + "_num", xaxistitle, 1000, 0, 1000, wk()) );
    m_denominatorHists.push_back( book(m_mainHistName, histname + "_denom", xaxistitle, 1000, 0, 1000, wk()) );
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

  ANA_MSG_DEBUG( "Calling initialize");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  
  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  // Grab the TrigDecTool from the ToolStore
  if(!m_trigDecTool_handle.isUserConfigured()){
    ANA_MSG_FATAL("A configured " << m_trigDecTool_handle.typeAndName() << " must have been previously created! Double-check the name of the tool." );
    return EL::StatusCode::FAILURE;
  }
  ANA_CHECK( m_trigDecTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);
  
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

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // MC event weight
  float mcEvtWeight(1.0);
  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
    static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
    if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
      ANA_MSG_ERROR( "mcEventWeight is not available as decoration! Aborting" );
      return EL::StatusCode::FAILURE;
    }
    mcEvtWeight = mcEvtWeightAcc( *eventInfo );
  }

  // get offline jet collection on which to do event selection and whih is going to be source of the quantity the efficiency is plotted as a function of
  const xAOD::JetContainer* inJets(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, msg()) );


  // iterate over turnons
  for ( int i_turnon = 0; i_turnon < m_probeTriggers.size(); i_turnon++) {

    // get the reference trigger info
    std::string refChainName = m_referenceTriggers[i_turnon];
    bool passedRefTrigger = m_trigDecTool_handle->isPassed(refChainName);
    // no point doing anything else if the reference trigger failed
    if(!passedRefTrigger)
      continue;

    // get the probe trigger info
    std::string probeChainName = m_probeTriggers[i_turnon];
    bool passedProbeTrigger = m_trigDecTool_handle->isPassed(probeChainName);
    const unsigned int probeBits = m_trigDecTool_handle->isPassedBits(probeChainName);
    bool L1_isPassedBeforePrescale = probeBits & TrigDefs::L1_isPassedBeforePrescale;
    bool L1_isPassedAfterPrescale  = probeBits & TrigDefs::L1_isPassedAfterPrescale;
    bool L1_isPassedAfterVeto      = probeBits & TrigDefs::L1_isPassedAfterVeto;
    bool isPrescaledOut = probeBits & TrigDefs::EF_prescaled;
    if(m_emulate) {
      // need to define separate function? Or tool?
      // read in stuff from the megaScript-filled class
      continue;
    }

    // only fill denominator hist if the L1 of the HLT passed and it was not prescaled out
    if(!L1_isPassedAfterVeto || isPrescaledOut)
      continue;

    // apply selections
    int mult = m_probeTriggerInfo[i_turnon].getMultiplicity();
    if(inJets->size() < mult)
      continue;


    std::vector<int> good_indices;
    for(int i=0; i<inJets->size(); i++){
      good_indices.push_back(i);
    }

    for(auto selection : m_probeTriggerInfo[i_turnon].offlineSelection) {
      // if already failed, don't waste time calculating things
      if(good_indices.size() < mult)
        break;

      std::vector<int> passed_indices;

      // ignore multiplicity
      if(selection.first == "multiplicity") {
        continue;
      }

      // eta
      else if(selection.first == "eta") {
        for(auto index : good_indices) {
          if( fabs(inJets->at(index)->eta()) > selection.second.first && fabs(inJets->at(index)->eta()) < selection.second.second )
            passed_indices.push_back(index);
        }
      }

      // else complain
      else {
        ANA_MSG_ERROR("do not recognise selection " + selection.first);
        return EL::StatusCode::FAILURE;
      }

      // carry forward only the good ones
      good_indices = passed_indices;
    }
    
    if(good_indices.size() < mult)
      continue;


    // get the relevant variable
    int index = m_variables_index[i_turnon];
    std::string variable = m_variables_var[i_turnon];
    float var_to_fill = -1;
    if(variable=="pt") {
      var_to_fill = inJets->at(good_indices[index])->pt() / 1000.;
    }
    else {
      ANA_MSG_ERROR("don't know how to interpret the variable "+ variable);
      return EL::StatusCode::FAILURE;
    }


    // fill hists
    m_denominatorHists.at(i_turnon)->Fill(var_to_fill);
    if(passedProbeTrigger){
      m_numeratorHists.at(i_turnon)->Fill(var_to_fill);
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
