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

// external tools include(s):
#include "AsgTools/AnaToolHandle.h"
#include "JetJvtEfficiency/IJetJvtEfficiency.h"
#include "JetInterface/IJetModifier.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

#include <string.h>
std::vector<std::string> splitString(std::string parentString, std::string sep);

class RulebookEntry {
 public:
  float lumiPoint;
  float rate;
  std::string comment;
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
  int multiplicity;

  void fillInfo(std::string infoString) {
    std::vector<std::string> splitInfoString = splitString(infoString, "\n");
    /* later: build protection for the case it's not in the menu */
    if(splitInfoString.size() == 0) {
      std::cout << "something went wrong" << std::endl;
      return;
    }
    chainName = splitInfoString.at(0);
    std::cout << "sadly I haven't implemented this yet" << std::endl;
    /* std::string offlSelStr = "   offline selection recommended"; */
    /* for(auto part : splitInfoString) { */
      /* std::cout << "===" << part << "===" << std::endl; */
      /* if( strncmp(part.c_str(), offlSelStr.c_str(), offlSelStr.size()) == 0 ) { */
        //    offline selection recommended  {'multiplicity': 1, 'eta': [0, 2.8000000000000003]}
        /* std::cout << "  yahoo! " << std::endl; */
        /* std::string selectionString = splitString(part, "{").at(1); //.pop_back(); */
        /* selectionString = splitString(selectionString, "}").at(0); */
        // 'multiplicity': 1, 'eta': [0, 2.8000000000000003]
        /* std::cout << selectionString << std::endl; */
        /* std::vector<std::string> selectionStringsVec = splitString(selectionString, ","); */
        // this isn't going to work because vec has comma
        // make / steal a better python list parser
        // can't do this now :-(
        /* for(auto selStr : selectionStringsVec) { */
          /* if( strncmp(selStr.c_str(), std::string("'multiplicity': ").c_str(), std::string("'multiplicity': ").size()) == 0 ) { */
            /* multiplicity = std::atoi(splitString(selStr, ":").at(1)); */
          /* } */
          /* if( strncmp(selStr.c_str(), std::string("'eta': ").c_str(), std::string("'eta': ").size()) == 0 ) { */
            /* float etaLow = std::atoi( splitString(selStr, ":").at(1) ); */
          /* } */
          
        /* } */
      /* } */
      /* else { */
        /* std::cout << "  booo" << std::endl; */
      /* } */


    /* } */
    /* later: build protection for the case it's not in the menu */
    /* HLT_j225_gsc420_boffperf_split */
      /* signature                  Jet */
      /* L1                         L1_J100 */
      /* HLT jet container preselection  HLT_xAOD__JetContainer_a4tcemsubjesISFS */
      /* HLT jet container          HLT_xAOD__JetContainer_GSCJet */
      /* topocluster formation      full scan */
      /* clusters                   em topoclusters */
      /* clustering                 radius 0.4 anti-kt */
      /* calibration steps          ['pileup subtraction', 'Jet Energy Scale correction', 'eta and JES in-situ corrections', 'calorimeter parts of GSC', 'ID track parts of GSC'] */
      /* comment                    Standard gsc central jet triggers (primaries) */
      /* rulebook entry             {1000: {'comment': 'Primary 1.7e34, Express ATR-17782', 'PS': 1.0, 'ESRate': 0.2}, 24001: {'comment': 'Disabled', 'PS': -1.0}} */
      /* HLT selection              {'multiplicity': 1, 'ET_preselection': 225, 'eta': [0.0, 3.2], 'ET': 420} */
      /* L1 selection               {'multiplicity': 1, 'eta': [0.0, 3.1], 'ET': 100} */
    
    
  }

};

void fillJetTriggerInfo(JetTriggerInfo jetTriggerInfo, std::string infoString);


class JetTriggerEfficiencies : public xAH::Algorithm
{

  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  /** @brief trigDecTool name for configurability if name is not default.  If empty, use the default name. If not empty, change the name. */
  std::string m_trigDecTool_name{"TDT:JetTriggerEfficiencies"};
  
  // configuration variables
  /// @brief input container name
  std::string m_inContainerName = "";
  // also need the HLT containers if going to emulate
  // for L1, HLT_presel and HLT
  
  /// @brief string defining turnons to make
  std::string m_turnonString = "";

  /// @brief emulate the turnon?
  bool m_emulate = false;

  // modified from HistogramManager
  /// @brief generically the main name assigned to all histograms
  std::string m_mainHistName = "effHists";

  /// @ brief path where jet trigger info script lives
  std::string m_jetTriggerInfoPath = "";

private:
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)

  /** @brief Trigger decision tool.

    If you need to use a TDT that was previously created before this algorithm with a different name, set the name in m_trigDecTool_name.
  */
  asg::AnaToolHandle<Trig::TrigDecisionTool> m_trigDecTool_handle{"Trig::TrigDecisionTool/TrigDecisionTool"}; //!
  asg::AnaToolHandle<Trig::IMatchingTool>    m_trigJetMatchTool_handle{"Trig::MatchingTool/MatchingTool", this}; //!

  /// @brief vectors reference and probe trigger names, decoded from m_turnonString
  std::vector<std::string> m_referenceTriggers; //!
  std::vector<std::string> m_probeTriggers; //!

  /// @brief vector of histograms - one for each turnon - that will be numerators (ie pass ref and probe) and denominators (ie pass ref) in the efficiencies
  std::vector<TH1F*> m_numeratorHists; //!
  std::vector<TH1F*> m_denominatorHists; //!

  std::vector<JetTriggerInfo> m_referenceTriggerInfo; //!
  std::vector<JetTriggerInfo> m_probeTriggerInfo; //!


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

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetTriggerEfficiencies, 1);
  /// @endcond
  
  
};


#endif
