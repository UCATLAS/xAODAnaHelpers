#ifndef xAODAnaHelpers_JERShifter_H
#define xAODAnaHelpers_JERShifter_H

// external tools include(s):
#include "JetResolution/JERTool.h"
#include "JetResolution/JERSmearingTool.h"

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

class JERShifter : public xAH::Algorithm
{

  public:
    // configuration variables
    std::string m_inContainerName;      // input container name
    std::string m_outContainerName;     // output container name
    std::string m_outAuxContainerName;
    std::string m_jetAlgo;

  private:
    int m_numEvent;         //!

    JERTool         * m_JERTool;     //!
    JERSmearingTool * m_JERSmearing; //!

  public:
    // this is a standard constructor
    JERShifter ();

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
