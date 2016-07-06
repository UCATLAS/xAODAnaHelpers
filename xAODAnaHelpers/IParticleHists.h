#ifndef xAODAnaHelpers_IParticleHists_H
#define xAODAnaHelpers_IParticleHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/Particle.h"

#include <xAODEventInfo/EventInfo.h>
#include <xAODBase/IParticleContainer.h>

class IParticleHists : public HistogramManager
{
  public:

    IParticleHists(std::string name, std::string detailStr, std::string prefix="", std::string title="");
    virtual ~IParticleHists() ;

    bool m_debug;
    virtual StatusCode initialize();
    StatusCode execute( const xAOD::IParticleContainer* particles, float eventWeight, const xAOD::EventInfo* eventInfo = 0 );
    virtual StatusCode execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* eventInfo = 0 );
    virtual StatusCode execute( const xAH::Particle* particle, float eventWeight);
    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  protected:

    // holds bools that control which histograms are filled
    HelperClasses::IParticleInfoSwitch* m_infoSwitch;

  private:
    std::string m_prefix;
    std::string m_title;

    //basic
    TH1F* m_Pt_l;                //!
    TH1F* m_Pt;                  //!
    TH1F* m_Pt_m;                //!
    TH1F* m_Pt_s;                //!
    TH1F* m_Eta;                 //!
    TH1F* m_Phi;                 //!
    TH1F* m_M;                   //!
    TH1F* m_E;                   //!
    TH1F* m_Rapidity;            //!

    // kinematic
    TH1F* m_Px;                  //!
    TH1F* m_Py;                  //!
    TH1F* m_Pz;                  //!

    //NLeadingParticles
    std::vector< TH1F* > m_NPt;       //!
    std::vector< TH1F* > m_NEta;      //!
    std::vector< TH1F* > m_NPhi;      //!
    std::vector< TH1F* > m_NM;        //!
    std::vector< TH1F* > m_NE;        //!
    std::vector< TH1F* > m_NRapidity; //!

};

#endif
