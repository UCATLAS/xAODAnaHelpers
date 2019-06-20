#ifndef EJsAnalysis_VertexContainer_H
#define EJsAnalysis_VertexContainer_H

/* VertexContainer is the base class for handling vertex container
   information to be used in making ntuples;
   based on xAODAnaHelpers's ParticleContainer class, 
   VertexContainer provides functions to add, clear, and fill branches
   with variables common to all types of vertices (i.e. n) */

#include <vector>
#include <string>
#include <cstdint>
#include <limits>

#include <TTree.h>

#include "EJsAnalysis/EJsHelperClasses.h"

namespace EJs {

  template < class T_INFOSWITCH >
  class VertexContainer
  {
  public:
    VertexContainer ( const std::string& name, const std::string& detailStr = "", float units = 1e3, bool mc = false )
      : m_name       ( name      ),
        m_infoSwitch ( detailStr ),
        m_units      ( units     ),
        m_mc         ( mc        ),
        m_debug      ( false     )
    {
      m_n = 0;
    }

    virtual ~VertexContainer() {};
    
    virtual void setTree ( TTree* tree )
    {
      std::string counterName  = "n" + m_name;
      tree->SetBranchStatus  ( counterName.c_str(), 1 );
      tree->SetBranchAddress ( counterName.c_str(), &m_n );  
    }
  
    virtual void setBranches ( TTree* tree )
    {
      std::string counterName  = "n" + m_name;
      tree->Branch( counterName.c_str(), &m_n, (counterName+"/I").c_str() );
    }
 
    virtual void clear ()
    {
      m_n = 0;
    }
   
    virtual void FillVertex ()
    {
      m_n++;
    }

    // updateEntry, at_nonConst, at, operator, size ... do we need these ??

    
  protected:
    std::string branchName ( const std::string& varName )
    {
      std::string name  = m_name + "_" + varName;
      return name;
    }
    
    template <typename T_BR> void connectBranch ( TTree* tree, const std::string& branch, std::vector<T_BR> **variable )
    {
      std::string name = branchName( branch );  
      if ( *variable ) { delete (*variable); (*variable) = 0; }
      if ( tree->GetBranch( name.c_str() ) ) {
	(*variable) = new std::vector<T_BR>();
	tree->SetBranchStatus  ( name.c_str(), 1 );
	tree->SetBranchAddress ( name.c_str(), variable );
      }
    }

    template <typename T> void setBranch ( TTree* tree, std::string varName, std::vector<T>* localVectorPtr )
    {
      std::string name = branchName( varName );
      tree->Branch( name.c_str(), localVectorPtr );
    }

    // safe*Fill, updateParticle ... may want to add later...
 
    std::string m_name;

    
  public:
    T_INFOSWITCH m_infoSwitch; 
    float    m_units;
    bool     m_mc;
    bool     m_debug;   
    int      m_n;

  };

  #define AUXDYN( obj, type, varname ) \
  ( obj->isAvailable<type>(varname) ? obj->auxdataConst<type>(varname) : std::numeric_limits<type>::quiet_NaN() )

  #define AUXDYNVEC( obj, type, varname ) \
    ( obj->isAvailable<std::vector<type>>(varname) ? obj->auxdataConst<std::vector<type>>(varname) : std::vector<type>() )

} // EJs

#endif // EJsAnalysis_VertexContainer_H
