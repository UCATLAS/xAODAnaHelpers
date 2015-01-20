#ifndef xAODAnaHelpers_AnalysisLoop_H
#define xAODAnaHelpers_AnalysisLoop_H

// C++ include(s):
#include <vector>

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// EDM include(s):
// custom include(s):
// ROOT include(s):
#include "TH1D.h"
#include "TEnv.h"

// Forward declarations
class GoodRunsListSelectionTool;

class AnalysisLoop : public EL::Algorithm
{

  // cannot be accessed by derived class
  private:

  protected:

    xAOD::TEvent*                m_event;     //!
    xAOD::TStore*                m_store;     //!

#ifndef __CINT__
    GoodRunsListSelectionTool*   m_grl;       //!
#endif // not __CINT__

    int m_eventCounter;     //!
    int m_numCleanEvents;   //!
    int m_numPassEvents;    //!

    std::string m_configFile;

    TH1* m_histEventCount;  //!

    // primary vertex
    std::string m_vertexContainerName;

    // muons
    std::string m_muonContainerName;

    // electrons
    std::string m_electronContainerName;

    // jets
    std::string m_jetContainerName;

    // fat jets
    std::string m_fatJetContainerName; 

    // put your configuration variables here as public variables.
    // that way they can be set directly from CINT and python.
  public:
    // float cutValue;

    bool m_debug;
    bool m_isMC;

    int m_maxEvent;


    // variables that don't get filled at submission time should be
    // protected from being send from the submission node to the worker
    // node (done by the //!)
  public:
    //TTree *outTree; //!
    // TH1 *myHist; //!



    // this is a standard constructor
    AnalysisLoop ();

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

    // these are functions not inherited from Algorithm
    // these are functions not inherited from Algorithm
    void setConfigFile(std::string configFile) { m_configFile = configFile; }
    virtual EL::StatusCode configure ();

    // FOR USERS: A few functions users can add to the derived class inorder to customize
    virtual EL::StatusCode setupJobUser (EL::Job& job) = 0;
    virtual EL::StatusCode initializeUser()   = 0;
    virtual EL::StatusCode histInitializeUser()   = 0;
    virtual EL::StatusCode passSelection()    = 0;
    virtual EL::StatusCode postExecuteUser()  = 0;
    virtual EL::StatusCode finalizeUser()     = 0;


    // this is needed to distribute the algorithm to the workers
    ClassDef(AnalysisLoop, 1);
};

#endif
