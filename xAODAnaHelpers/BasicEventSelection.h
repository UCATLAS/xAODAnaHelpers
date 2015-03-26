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
namespace CP{
  class PileupReweightingTool;
}

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
    CP::PileupReweightingTool*   m_pileuptool; //!
#endif // not __CINT__

    int m_eventCounter;     //!

    // variables read in through configuration file
    int m_debug;            //!
    bool m_truthLevelOnly;  //!
    // GRL
    std::string m_GRLxml;   //!
    //PU Reweighting
    bool m_doPUreweighting; //!
    
    // primary vertex
    std::string m_vertexContainerName; //!
    int m_PVNTrack;                //!
    
    // read from MetaData
    TH1D* m_histEventCount;  //!    
    int m_MD_initialNevents; //!
    int m_MD_finalNevents;   //!
    float m_MD_initialSumW;    //!
    float m_MD_finalSumW;	     //!
    
    // cutflow
    TH1D* m_cutflowHist;    //!
    TH1D* m_cutflowHistW;   //!
    int m_cutflow_all;      //!
    int m_cutflow_grl;      //!
    int m_cutflow_lar;      //!
    int m_cutflow_tile;     //!
    int m_cutflow_core;     //!
    int m_cutflow_npv;      //!

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
