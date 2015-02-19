#ifndef xAODAnaHelpers_JERShifter_H
#define xAODAnaHelpers_JERShifter_H

// EL include(s)
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

class JERTool;
class JERSmearingTool;

class JERShifter : public EL::Algorithm
{

  public:

    xAOD::TEvent *m_event;  //!
    xAOD::TStore *m_store;  //!
    int m_numEvent;         //!

    std::string m_name;
    std::string m_configName;

  private:

    // configuration variables
    TString m_inContainerName;      // input container name
    TString m_outContainerName;     // output container name
    TString m_outAuxContainerName;  
    TString m_jetAlgo;

#ifndef __CINT__  
    JERTool         * m_JERTool;     //!
    JERSmearingTool * m_JERSmearing; //!
#endif // not __CINT__

  public:

    // this is a standard constructor
    JERShifter ();
    JERShifter (std::string name, std::string configName);

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

    // this is needed to distribute the algorithm to the workers
    ClassDef(JERShifter, 1);
};

#endif
