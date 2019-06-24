#ifndef xAODAnaHelpers_SecondaryVertexSelector_H
#define xAODAnaHelpers_SecondaryVertexSelector_H

/**
 *@file SecondaryVertexSelector.h
 *
 *@author Colleen Treado <colleen.jennifer.treado@cern.ch>
 *@author Jackson Bruzynski <jackson.carl.burzynski@cern.ch>
 *
 *@brief Selects secondary vertices based on the VsiBonsai tool
 *
 **/

#include <string>
#include "TH3S.h"

#include <xAODTracking/Vertex.h>

#include <xAODAnaHelpers/Algorithm.h>
#include "xAODAnaHelpers/VsiBonsaiTool.h"


class SecondaryVertexSelector : public xAH::Algorithm
{
  // configuration variables
  // public so they can be seen directly from CINT + python
 public:
  // float cutValue;

  // do cutflow

  std::string m_inContainerName  = "VrtSecInclusive_SecondaryVertices";
  std::string m_outContainerName = "FilteredSecondaryVertices";

  bool m_decorateSelectedObjects = true;
  bool m_createSelectedContainer = true; // false
  bool m_do_trackTrimming = true;
  bool m_do_matMapVeto = true;

  std::string m_mapInFile;
  std::string m_mapInName;
  std::string m_mapOutFile;
  std::string m_mapOutName;
  std::string m_matrixInName;

  TH3S* m_materialMap_v3_Outer;
  TH3S* m_materialMap_v32_Inner;
  TMatrixT<double>* m_materialMap_v32_Matrix;

  // vertex cuts...
  VsiBonsai::Configurator m_bonsaiCfg;
  double      prop_chi2Cut;              
  int         prop_hitPatternReq;        
  int         prop_doDropAssociated;     
  int         prop_fillTracks;           
  double      prop_trackStorePrescale;   
  double      prop_d0_wrtSVCut;          
  double      prop_z0_wrtSVCut;          
  double      prop_errd0_wrtSVCut;       
  double      prop_errz0_wrtSVCut;       
  double      prop_d0signif_wrtSVCut;    
  double      prop_z0signif_wrtSVCut;    
  double      prop_chi2_toSVCut;         


  // variables not filled at submission time
  // protected from being sent from submission to worker node
 public:
  // TTree *myTree; //!
  // TH1 *myHist; //!
  

  // standard constructor
  SecondaryVertexSelector ();

  // functions inherited from Algorithm
  virtual EL::StatusCode setupJob ( EL::Job& job );
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput ( bool firstFile );
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  virtual int PassCuts ( const xAOD::Vertex* vtx, const xAOD::Vertex* primaryVtx ); // may need pv too

  //needed to distribute algorithm to workers
  ClassDef ( SecondaryVertexSelector, 1 );
};

#endif // xAODAnaHelpers_SecondaryVertexSelector_H
