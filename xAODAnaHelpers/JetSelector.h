/**
 * @file   JetSelector.h
 *
 * @author Gabriel Facini <gabriel.facini@cern.ch>
 * @author Jeff Dandoy  <jeff.dandoy@cern.ch>
 * @author Marco Milesi <marco.milesi@cern.ch>
 * @author John Alison <john.alison@cern.ch>
 *
 * @brief  Select jets and apply JVT corrections.
 *
 */

#ifndef xAODAnaHelpers_JetSelector_H
#define xAODAnaHelpers_JetSelector_H

// EDM include(s):
#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"

// ROOT include(s):
#include "TH1D.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

// external tools include(s):
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"
#include "JetJvtEfficiency/JetJvtEfficiency.h"
#include "AsgTools/AnaToolHandle.h"

class JetSelector : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  bool m_useCutFlow;

  // configuration variables
  std::string m_inContainerName;   // input container name
  std::string m_outContainerName;  // output container name
  std::string m_inputAlgo;         // input type - from xAOD or from xAODAnaHelper Algo output
  std::string m_outputAlgo;        // output type - this is how the vector<string> w/ syst names will be saved in TStore
  std::string m_jetScaleType;    // Type of Scale Momementum
  std::string m_decor;            // The decoration key written to passing objects
  bool m_decorateSelectedObjects; // decorate selected objects? defaul passSel
  bool m_createSelectedContainer; // fill using SG::VIEW_ELEMENTS to be light weight
  int m_nToProcess;               // look at n objects
  bool m_cleanJets;               // require cleanJet decoration to not be set and false
  int m_cleanEvtLeadJets;         // kill event if any of the N leading jets are not clean
  /** @brief Kill event if any passing jets are not clean */
  bool m_cleanEvent;
  int m_pass_min;                 // minimum number of objects passing cuts
  int m_pass_max;                 // maximum number of objects passing cuts
  float m_pT_max;                 // require pT < pt_max
  float m_pT_min;                 // require pT > pt_max
  float m_eta_max;                // require eta < eta_max
  float m_eta_min;                // require eta > eta_max
  float m_detEta_max;             // require detEta < detEta_max
  float m_detEta_min;             // require detEta > detEta_max
  float m_mass_max;               // require mass < mass_max
  float m_mass_min;               // require mass > mass_max
  float m_rapidity_max;           // require rapidity < rapidity_max
  float m_rapidity_min;           // require rapidity > rapidity_min
  int   m_truthLabel;             // require truth level on truth jets
  bool  m_useHadronConeExcl;      // use HadronConeExclTruthLabelID for truth match (default)

  bool m_doJVF;                   // check JVF
  float m_pt_max_JVF;             // max pT (JVF is a pileup cut)
  float m_eta_max_JVF;            // detector eta cut
  float m_JVFCut;                 // cut value
  bool m_doJVT;                   // check JVT
  float m_pt_max_JVT;             // max pT (JVT is a pileup cut)
  float m_eta_max_JVT;            // detector eta cut

    /**
        @brief Minimum value of JVT for selecting jets.

        @rst
            .. warning:: If set to a non-negative value (default is -1.0), it will override any set value for :cpp:member:`JetSelector::m_WorkingPointJVT`
        @endrst
    */
  float m_JVTCut;

        /**
        @rst
            Available working points for JVT cut from the ``CP::IJetJvtEfficiency`` tool.

            The corresponding data/MC SF will be saved as a ``std::vector<float>`` decoration (for MC only), for nominal WP and the available systematics.

            ======== ================= =============
            Value    JVT Cut           Efficiency
            ======== ================= =============
            "Medium"  (Default) 0.59    92%
            "Loose"   0.11              97%
            "Tight"   0.91              85%
            ======== ================= =============

        @endrst
        */
  std::string m_WorkingPointJVT;
  std::string m_outputSystNamesJVT;

  float         m_systValJVT;
  std::string   m_systNameJVT;

  // for BTaggingSelectionTool -- doubles are needed or will crash
  bool  m_doBTagCut;              // Flag to apply btagging cut, if false just decorate decisions
  std::string m_corrFileName;
  std::string m_jetAuthor;
  std::string m_taggerName;
  std::string m_operatingPt;
  double m_b_eta_max;
  double m_b_pt_min;

  bool m_doHLTBTagCut;
  std::string m_HLTBTagTaggerName;
  float m_HLTBTagCutValue;
  bool  m_requireHLTVtx;
  bool  m_requireNoHLTVtx;

  std::string              m_passAuxDecorKeys;
  std::string              m_failAuxDecorKeys;

private:
  int m_numEvent;         //!
  int m_numObject;        //!
  int m_numEventPass;     //!
  int m_weightNumEventPass; //!
  int m_numObjectPass;    //!
  int m_pvLocation;       //!

  bool m_isEMjet;                //!
  bool m_isLCjet;                //!

  // cutflow
  TH1D* m_cutflowHist;          //!
  TH1D* m_cutflowHistW;         //!
  int   m_cutflow_bin;          //!

  std::vector<std::string> m_passKeys;  //!
  std::vector<std::string> m_failKeys;  //!

  /* object-level cutflow */

  TH1D* m_jet_cutflowHist_1;  //!

  int   m_jet_cutflow_all;           //!
  int   m_jet_cutflow_cleaning_cut;  //!
  int   m_jet_cutflow_ptmax_cut;     //!
  int   m_jet_cutflow_ptmin_cut;     //!
  int   m_jet_cutflow_eta_cut;       //!
  int   m_jet_cutflow_jvt_cut;       //!
  int   m_jet_cutflow_btag_cut;      //!

  std::vector<CP::SystematicSet> m_systListJVT; //!

  BTaggingSelectionTool   *m_BJetSelectTool;    //!

  std::string m_JVT_tool_name;                                 //!
  asg::AnaToolHandle<CP::IJetJvtEfficiency> m_JVT_tool_handle; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  JetSelector (std::string className = "JetSelector");

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

  // these are the functions not inherited from Algorithm
  virtual bool executeSelection( const xAOD::JetContainer* inJets, float mcEvtWeight, bool count, std::string outContainerName );

  // added functions not from Algorithm
  // why does this need to be virtual?
  virtual int PassCuts( const xAOD::Jet* jet );

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(JetSelector, 1);
  /// @endcond

};

#endif
