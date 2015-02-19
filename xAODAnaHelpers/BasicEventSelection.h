#ifndef xAODAnaHelpers_BasicEventSelection_H
#define xAODAnaHelpers_BasicEventSelection_H

// EL include(s):
#include <EventLoop/StatusCode.h>
#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

// EDM include(s):
// custom include(s):

// ROOT include(s):
#include "TH1D.h"

// Forward declarations
class GoodRunsListSelectionTool;


class BasicEventSelection : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
  public:
    // float cutValue;

    std::string m_name;
    std::string m_configName;

    xAOD::TEvent*                m_event;     //!
    xAOD::TStore*                m_store;     //!

#ifndef __CINT__
    GoodRunsListSelectionTool*   m_grl;       //!
#endif // not __CINT__

    int m_eventCounter;     //!
    int m_cutflow_all;      //!
    int m_cutflow_grl;      //!
    int m_cutflow_lar;      //!
    int m_cutflow_tile;     //!
    int m_cutflow_core;     //!
    int m_cutflow_npv;      //!

    TH1D* m_histEventCount;  //!

    // variables read in through configuration file
    bool m_cleanTStore;     //!
    int m_debug;            //!
    // GRL
    std::string m_GRLxml;   //!
    // primary vertex
    TString m_vertexContainerName; //!
    int m_PVNTrack;                //!
    // cutflow
    TH1D* m_cutflowHist;           //!
    TH1D* m_cutflowHistW;          //!


    // variables that don't get filled at submission time should be
    // protected from being send from the submission node to the worker
    // node (done by the //!)
  public:
    // Tree *myTree; //!
    // TH1 *myHist; //!
    //

    // this is a standard constructor
    BasicEventSelection ();
    BasicEventSelection (std::string name, std::string configName);

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
    virtual EL::StatusCode configure ();

    // this is needed to distribute the algorithm to the workers
    ClassDef(BasicEventSelection, 1);
};

#endif
