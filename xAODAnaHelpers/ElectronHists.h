#ifndef xAODAnaHelpers_ElectronHists_H
#define xAODAnaHelpers_ElectronHists_H

#include "xAODAnaHelpers/IParticleHists.h"
#include <xAODEgamma/ElectronContainer.h>
#include "xAODAnaHelpers/Electron.h"
#include "xAODAnaHelpers/EventInfo.h"
#include <AthContainers/DataVector.h>

class ElectronHists : public IParticleHists
{
  public:


    ElectronHists(std::string name, std::string detailStr);
    virtual ~ElectronHists() ;

    virtual StatusCode initialize();
    virtual StatusCode execute( const xAOD::Electron* electron, float eventWeight, const xAOD::EventInfo* eventInfo = 0);
    virtual StatusCode execute( const xAH::Electron*  electron, float eventWeight, const xAH::EventInfo*  eventInfo = 0);
    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using IParticleHists::execute; // overload

  protected:

    virtual StatusCode execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* eventInfo = 0 );
    virtual StatusCode execute( const xAH::Particle* particle,   float eventWeight, const xAH::EventInfo*  eventInfo = 0 );

    // holds bools that control which histograms are filled
    HelperClasses::ElectronInfoSwitch* m_infoSwitch;

  private:

    // Isolation
    TH1F* m_isIsolated_LooseTrackOnly              ; //!
    TH1F* m_isIsolated_Loose			   ; //!
    TH1F* m_isIsolated_Tight			   ; //!
    TH1F* m_isIsolated_Gradient			   ; //!
    TH1F* m_isIsolated_GradientLoose		   ; //!
    TH1F* m_isIsolated_GradientT1		   ; //!
    TH1F* m_isIsolated_GradientT2		   ; //!
    TH1F* m_isIsolated_MU0p06			   ; //!
    TH1F* m_isIsolated_FixedCutLoose		   ; //!
    TH1F* m_isIsolated_FixedCutTight		   ; //!
    TH1F* m_isIsolated_FixedCutTightTrackOnly	   ; //!
    TH1F* m_isIsolated_UserDefinedFixEfficiency	   ; //!
    TH1F* m_isIsolated_UserDefinedCut		   ; //!

    //quality
    //TH1F* m_LHVeryLoose   ; //!
    TH1F* m_LHLoose       ; //!    
    TH1F* m_LHMedium      ; //!   
    TH1F* m_LHTight       ; //!     

    
    // clean
    TH1F* m_ptcone20;                //!
    TH1F* m_ptcone30;                //!
    TH1F* m_ptcone40;                //!
    TH1F* m_ptvarcone20;             //!
    TH1F* m_ptvarcone30;             //!
    TH1F* m_ptvarcone40;             //!
    TH1F* m_topoetcone20;            //!
    TH1F* m_topoetcone30;            //!
    TH1F* m_topoetcone40;            //!

    // rel
    TH1F* m_ptcone20_rel;            //!
    TH1F* m_ptcone30_rel;            //!
    TH1F* m_ptcone40_rel;            //!
    TH1F* m_ptvarcone20_rel;         //!
    TH1F* m_ptvarcone30_rel;         //!
    TH1F* m_ptvarcone40_rel;         //!
    TH1F* m_topoetcone20_rel;        //!
    TH1F* m_topoetcone30_rel;        //!
    TH1F* m_topoetcone40_rel;        //!

    

};

#endif
