/**
 * @file   JetTriggerEfficiencies.h
 *
 * @author Will Kalderon <william.kalderon@cern.ch>
 *
 * @brief  Make jet trigger efficiency histograms using the trigger decision tool and emulation
 *
 */

#ifndef xAODAnaHelpers_JetTriggerEfficiencies_H
#define xAODAnaHelpers_JetTriggerEfficiencies_H

// EDM include(s):
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

// ROOT include(s):
#include "TH1D.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// for JetInfo
#include "xAODAnaHelpers/TreeReader.h"

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h"
#include "JetJvtEfficiency/IJetJvtEfficiency.h"
#include "JetInterface/IJetModifier.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

#include <string.h>
#include <regex>
std::vector<std::string> splitString(std::string parentString, std::string sep);
std::vector<std::string> splitListString(std::string parentString);
static inline void ReplaceAll(std::string &str, const std::string& from, const std::string& to);

class RulebookEntry {
 public:
  float lumiPoint;
  float rate;
  std::string comment;
};


class TriggerDecision {
 public:
  bool passedTrigger = false;
  bool isDisabled = false;
  bool L1_isPassedBeforePrescale = false;
  bool L1_isPassedAfterPrescale = false;
  bool L1_isPassedAfterVeto = false;
  bool HLT_isPrescaledOut = true;

  void fillFromBits(unsigned int triggerBits) {
    L1_isPassedBeforePrescale = triggerBits & TrigDefs::L1_isPassedBeforePrescale;
    L1_isPassedAfterPrescale  = triggerBits & TrigDefs::L1_isPassedAfterPrescale;
    L1_isPassedAfterVeto      = triggerBits & TrigDefs::L1_isPassedAfterVeto;
    HLT_isPrescaledOut        = triggerBits & TrigDefs::EF_prescaled;
  }

};


class JetTriggerInfo {
 public:
  std::string chainName;
  std::string L1;
  std::string HLTjetContainer;
  std::string HLTjetContainerPreselection;
  std::string topoclusterFormation;
  std::string clusters;
  std::string clustering;
  std::vector<std::string> calibrationSteps;
  std::string comment;
  std::vector<RulebookEntry> rulebookEntry;
  std::vector< std::pair<std::string, std::pair<float, float> > > HLTselection;
  std::vector< std::pair<std::string, std::pair<float, float> > > L1selection;
  std::vector< std::pair<std::string, std::pair<float, float> > > offlineSelectionRecommended;
  std::vector< std::pair<std::string, std::pair<float, float> > > offlineSelection;
  std::string offlineSelectionString;

  // pull out relevant part of infoString
  std::string find_and_strip(std::string infoString, std::string key) {
    std::vector<std::string> splitInfoString = splitString(infoString, "\n");
    std::string resultString = "";
    for(auto part : splitInfoString) {
      // check whether part starts with key
      if( strncmp(part.c_str(), key.c_str(), key.size()) == 0 ) {
        resultString = part.substr(key.size(), part.size());
        break;
      }
    }

    // strip whitespace
    return std::regex_replace(resultString, std::regex("^ +| +$|( ) +"), "$1");
  }
  
  // translate the python dictionary like selection string into the c++ equivalent
  std::vector< std::pair<std::string, std::pair<float, float> > > decodeSelection(std::string selectionString) {
    std::vector< std::pair<std::string, std::pair<float, float> > > selection;
    
    // if empty (eg RD0_FILLED) then return empty selection vector
    if(selectionString == "{}")
      return selection;

    // trim { and }
    selectionString = selectionString.substr(1, selectionString.size()-2);

    // remove all whitespace
    selectionString.erase(remove(selectionString.begin(), selectionString.end(), ' '), selectionString.end());

    // split on ", '" since keys are strings and selections are floats or ints
    std::vector<std::string> vecString = splitString(selectionString, ",'");

    // now have:
    //  "multiplicity': 1"
    //  "eta': [0.0, 3.1]"
    //  "ET': 100"
    for (auto part : vecString ) {
      // get key
      std::string key = splitString(part, "':")[0];
      // remove stray ' characters
      key.erase(remove(key.begin(), key.end(), '\''), key.end());


      // get value part
      std::string val_str = splitString(part, "':")[1];
      std::pair<float, float> vals_pair;
      // if it's a single number
      if (val_str.find(",") == std::string::npos) {
        float val_float = std::stof(val_str);
        vals_pair.first = val_float;
        vals_pair.second = val_float;
      }
      // if it's a range
      else {
        // remove [ and ]
        val_str = val_str.substr(1,val_str.size()-2);
        vals_pair.first = std::stof(splitString(val_str, ",")[0]);
        vals_pair.second = std::stof(splitString(val_str, ",")[1]);
      }

      std::pair<std::string, std::pair<float, float> > thisSelection;
      thisSelection.first = key;
      thisSelection.second = vals_pair;
      selection.push_back(thisSelection);
    }
    
    return selection;
  }


  void fillInfo(std::string infoString, std::string offlineSelectionStr) {
    
    std::vector<std::string> splitInfoString = splitString(infoString, "\n");
    if(splitInfoString.size() == 0) {
      std::cout << "something went wrong" << std::endl;
      return;
    }

    // get chain name. Later: build protection for the case it's not in the menu
    chainName = splitInfoString.at(0);

    // easy ones - just strings
    L1                          = find_and_strip(infoString, "   L1:");
    HLTjetContainer             = find_and_strip(infoString, "   HLT jet container:");
    HLTjetContainerPreselection = find_and_strip(infoString, "   HLT jet container preselection:");
    topoclusterFormation        = find_and_strip(infoString, "   topocluster formation:");
    clusters                    = find_and_strip(infoString, "   clusters:");
    clustering                  = find_and_strip(infoString, "   clustering:");
    comment                     = find_and_strip(infoString, "   comment:");

    // get calibration steps: python list of strings -> vector of strings
    calibrationSteps = splitListString( find_and_strip(infoString, "   calibration steps:") );

    // get L1, HLT, offline selections: dictionary -> vector of map<variable, map<low,high> >
    L1selection      = decodeSelection( find_and_strip(infoString, "   L1 selection:") );
    HLTselection     = decodeSelection( find_and_strip(infoString, "   HLT selection:") );
    offlineSelectionRecommended = decodeSelection( find_and_strip(infoString, "   offline selection recommended:") );

    // set offline selection to use - "auto" takes recommended, otherwise decode as above
    if(offlineSelectionStr == "auto") {
      offlineSelection = offlineSelectionRecommended;
      offlineSelectionString = find_and_strip(infoString, "   offline selection recommended:");
    }
    else if (offlineSelectionStr.find("auto+") != std::string::npos) {
      // auto part
      std::vector< std::pair<std::string, std::pair<float, float> > > autoSelection = offlineSelectionRecommended;
      std::string autoSelectionString = find_and_strip(infoString, "   offline selection recommended:");

      // extra part
      std::string extraPart = splitString(offlineSelectionStr,"auto+")[1];
      std::vector< std::pair<std::string, std::pair<float, float> > > extraSelection = decodeSelection(extraPart);

      // merge
      offlineSelection = extraSelection;
      // get keys of extraSelection
      std::vector<std::string> extraSelectionKeys;
      for(auto selection : extraSelection) {
        extraSelectionKeys.push_back(selection.first);
      }
      // add bits of autoSelection that aren't in extraSelection
      for(auto selection : autoSelection) {
        // don't overwrite anything in extraSelection
        if (std::find(extraSelectionKeys.begin(), extraSelectionKeys.end(), selection.first) != extraSelectionKeys.end()) {
          continue;
        }
        else {
          offlineSelection.push_back(selection);
        }
      }
      // lazy definition of string, because faff
      offlineSelectionString = autoSelectionString + " + " + extraPart;
      std::cout << "new string " << offlineSelectionString << std::endl;
    }
    else {
      offlineSelection = decodeSelection( offlineSelectionStr );
      offlineSelectionString = offlineSelectionStr;
    }


    // get rulebook
    // do this later...
    /*
    class RulebookEntry {
    public:
      float lumiPoint;
      float rate;
      std::string comment;
      };*/
    /* rulebook entry             {1000: {'comment': 'Primary 1.7e34, Express ATR-17782', 'PS': 1.0, 'ESRate': 0.2}, 24001: {'comment': 'Disabled', 'PS': -1.0}} */
        
  }

  // look for 'multiplicity' in offlineSelection
  int getMultiplicity() {
    int mult = -1;
    for (auto selection : offlineSelection) {
      if(selection.first == "multiplicity") {
        mult = int(selection.second.first);
        break;
      }
    }
    return mult;
  }

  bool setMultiplicity(int mult) {
    bool found = false;
    for (auto selection : offlineSelection) {
      if(selection.first == "multiplicity") {
        found = true;
        // only increase multiplicity, don't decrease
        int origMult = int(selection.second.first);
        if (mult > origMult) {
          selection.second.first = mult;
          selection.second.second = mult;
        }
      }
    }
    return found;
  }

};



class JetTriggerEfficiencies : public xAH::Algorithm
{

  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  /** @brief trigDecTool name for configurability if name is not default.  If empty, use the default name. If not empty, change the name. */
  std::string m_trigDecTool_name{"TDT:JetTriggerEfficiencies"};
  

  // configuration variables
  
  /// @brief 
  bool m_fromNTUP = false;
  
  /// @brief container to use as offline reference (ie efficiencies are made as a function of this)
  std::string m_offlineContainerName = "";
  
  /// @brief string defining turnons to make
  std::string m_turnonString = "";
  std::string m_selectionString = "auto";
  std::string m_variableString = "pt";
  bool m_orderByVariable = false; // eg for mass turnons plot nth mass not mass of nth pt

  bool m_plotSelectionVars = false;


  /// @brief emulate the turnon?
  bool m_TDT = true;
  bool m_emulate = false;
  bool m_skipCompare = false; // unless true, will compare TDT and emulation and complain if they are not the same. Emulation not implemented for everything, though, so set true if konw this to be the case to avoid drowning in warnings.

  // modified from HistogramManager
  /// @brief generically the main name assigned to all histograms
  std::string m_mainHistName = "effHists";

  /// @brief split by selection on this variable
  std::string m_splitBy = "";
  std::string m_splitValuesStr = "";


  /// @ brief path where jet trigger info script lives - needs to be part of this code so grid knows about it
  std::string m_jetTriggerInfoPath = "xAODAnaHelpers/menu";

  /// @ brief menu set to use
  std::string m_jetTriggerMenuSet = "2018";

  /// @ brief require probe triggers to be an exact match to a menu item
  bool m_requireTriggerInfoMatch = true;

  /// @brief use of Good Runs List
  bool m_applyGRLCut = false;
  std::string m_GRLxml = "xAODAnaHelpers/data17_13TeV.periodAllYear_DetStatus-v97-pro21-13_Unknown_PHYS_StandardGRL_All_Good_25ns_JetHLT_Normal2017.xml";


private:
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

  /** @brief Trigger decision tool.

    If you need to use a TDT that was previously created before this algorithm with a different name, set the name in m_trigDecTool_name.
  */
  asg::AnaToolHandle<Trig::TrigDecisionTool> m_trigDecTool_handle{"Trig::TrigDecisionTool/TrigDecisionTool"}; //!
  asg::AnaToolHandle<Trig::IMatchingTool>    m_trigJetMatchTool_handle{"Trig::MatchingTool/MatchingTool", this}; //!

  /// @brief GRL tool
  asg::AnaToolHandle<IGoodRunsListSelectionTool> m_grl_handle{"GoodRunsListSelectionTool", this}; //!

  /// @brief vectors reference and probe trigger names, decoded from m_turnonString
  std::vector<std::string> m_referenceTriggers; //!
  std::vector<std::string> m_probeTriggers; //!

  /// @brief vectors of selection and variable to plot, one for each turnon
  std::vector<std::string> m_selections; //!
  std::vector<std::string> m_variables; //!
  std::vector<std::string> m_variables_var; //!
  std::vector<int> m_variables_index; //!

  /// @brief vector of the split values to use
  std::vector<float> m_splitValues; //!

  /// @brief vector of histograms - one for each turnon - that will be numerators (ie pass ref and probe) and denominators (ie pass ref) in the efficiencies
  std::vector<TH1F*> m_numeratorHistsTDT; //!
  std::vector<TH1F*> m_numeratorHistsEmulated; //!
  std::vector<TH1F*> m_denominatorHistsTDT; //!
  std::vector<TH1F*> m_denominatorHistsEmulated; //!

  std::vector<JetTriggerInfo> m_referenceTriggerInfo; //!
  std::vector<JetTriggerInfo> m_probeTriggerInfo; //!

  TH1F* m_splitVarHist; //!

  std::vector< std::vector<TH1F*> > m_preSelHistsSelections; //!
  std::vector< std::vector<TH1F*> > m_postSelHistsSelections; //!
  std::vector< std::vector<TH1F*> > m_numeratorHistsSelectionsTDT; //!
  std::vector< std::vector<TH1F*> > m_denominatorHistsSelectionsTDT; //!
  std::vector< std::vector<TH1F*> > m_numeratorHistsSelectionsEmulated; //!
  std::vector< std::vector<TH1F*> > m_denominatorHistsSelectionsEmulated; //!
  


public:

  // this is a standard constructor
  JetTriggerEfficiencies ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // stolen ish from xAH::HistogramManager
  TH1F* book(std::string name, std::string title, std::string xlabel,
             int xbins, double xlow, double xhigh, EL::Worker* wk);


  // these are the functions not inherited from Algorithm
  /* virtual bool executeSelection( const xAOD::JetContainer* inJets, float mcEvtWeight, bool count, std::string outContainerName, bool isNominal ); */


  // added functions not from Algorithm
  // why does this need to be virtual?
  /* virtual int PassCuts( const xAOD::Jet* jet ); */

  virtual EL::StatusCode emulateTriggerDecision(JetTriggerInfo &triggerInfo, TriggerDecision &triggerDecision);
  virtual EL::StatusCode applySelections(bool &passSelections, std::vector< std::pair<std::string, std::pair<float, float> > > selections, const xAOD::JetContainer* jets, JetInfo &jetsInfo, unsigned int multiplicity_required, std::vector<int> &good_indices, bool isHLTpresel=false);
  virtual EL::StatusCode applySelection(std::vector<int> &passed_indices, std::pair<std::string, std::pair<float, float> > selection, const xAOD::JetContainer* jets, JetInfo &jetsInfo, std::vector<int> good_indices, bool isHLTpresel);

  virtual EL::StatusCode FillSelectionVarHists(JetTriggerInfo &probeTriggerInfo, const xAOD::JetContainer* jets, JetInfo &jetsInfo, std::vector<TH1F*> &histsVec, int jet_index);

  virtual EL::StatusCode retrieveJetInfo(JetInfo &jetInfo, std::string jetCollectionName);

  // gets a variable by its string name, from the relevant one of xAOD jet container and NTUP's JetInfo
  virtual EL::StatusCode get_variable(std::vector<float> &var_vec, std::string varName, const xAOD::JetContainer* jets, JetInfo &jetsInfo, bool fromNTUP);

  virtual EL::StatusCode get_event_variable(float &var, std::string varName, const xAOD::EventInfo* eventInfo_xAOD, EventInfo &eventInfo_ntup, bool fromNTUP);

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetTriggerEfficiencies, 1);
  /// @endcond
  
  
};




#endif
