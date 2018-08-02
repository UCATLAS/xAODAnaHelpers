/******************************************
 *
 * This class gets HLT jets from the TDT and can be expanded to get other features
 *
 * Merlin Davies (merlin.davies@cern.ch)
 * Caterina Doglioni (caterina.doglioni@cern.ch)
 * John Alison (john.alison@cern.ch)
 *
 *
 ******************************************/

#ifndef xAODAnaHelpers_HLTJetGetter_H
#define xAODAnaHelpers_HLTJetGetter_H


// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"


namespace TrigConf {
    class xAODConfigTool;
}

namespace Trig {
    class TrigDecisionTool;
}


class HLTJetGetter : public xAH::Algorithm
{

public:

  /* configuration variables */
  /// @brief List of triggers whose features will be extracted from TDT
  std::string m_triggerList = ".*";
  /// @brief input container name, WITHOUT the HLT_xAOD__JetContainer_ prefix
  std::string m_inContainerName = "";
  /// @brief output container name
  std::string m_outContainerName = "";

private:

  //Trig::TrigDecisionTool*        m_trigDecTool = nullptr;   //!
  asg::AnaToolHandle<Trig::TrigDecisionTool>              m_trigDecTool_handle{"Trig::TrigDecisionTool/TrigDecisionTool"    }; //! 
  TrigConf::xAODConfigTool*      m_trigConfTool = nullptr;   //!
  /// @brief flag to own TDT and TCT
  bool                           m_ownTDTAndTCT = false;   //!

public:

  // this is a standard constructor
  HLTJetGetter ();

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

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(HLTJetGetter, 1);
  /// @endcond

};

#endif
