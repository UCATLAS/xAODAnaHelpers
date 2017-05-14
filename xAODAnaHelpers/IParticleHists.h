#ifndef xAODAnaHelpers_IParticleHists_H
#define xAODAnaHelpers_IParticleHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/ParticleContainer.h"

#include <xAODAnaHelpers/Jet.h>
#include <xAODAnaHelpers/EventInfo.h>
#include <xAODBase/IParticleContainer.h>
#include <AsgTools/MessageCheck.h>

ANA_MSG_HEADER(msgIParticleHists)

class IParticleHists : public HistogramManager
{
  public:

    IParticleHists(std::string name, std::string detailStr, std::string prefix="", std::string title="");
    virtual ~IParticleHists() ;

    bool m_debug;
    virtual StatusCode initialize();
    StatusCode execute( const xAOD::IParticleContainer* particles, float eventWeight, const xAOD::EventInfo* eventInfo = 0 );
    virtual StatusCode execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* eventInfo = 0 );

    template <class T_PARTICLE, class T_INFOSWITCH>
      StatusCode execute( const xAH::ParticleContainer<T_PARTICLE, T_INFOSWITCH>* particles, float eventWeight, const xAH::EventInfo* eventInfo = 0);

    //StatusCode execute( const xAH::ParticleContainer* particles, float eventWeight, const xAH::EventInfo* eventInfo = 0 );
    virtual StatusCode execute( const xAH::Particle* particle, float eventWeight, const xAH::EventInfo* eventInfo = 0);
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
    TH1F* m_Et;                  //!
    TH1F* m_Et_m;                //!
    TH1F* m_Et_s;                //!

    //NLeadingParticles
    std::vector< TH1F* > m_NPt_l;       //!
    std::vector< TH1F* > m_NPt;       //!
    std::vector< TH1F* > m_NPt_m;       //!
    std::vector< TH1F* > m_NPt_s;       //!
    std::vector< TH1F* > m_NEta;      //!
    std::vector< TH1F* > m_NPhi;      //!
    std::vector< TH1F* > m_NM;        //!
    std::vector< TH1F* > m_NE;        //!
    std::vector< TH1F* > m_NRapidity; //!

    // kinematic
    std::vector< TH1F* > m_NEt;       //!
    std::vector< TH1F* > m_NEt_m;       //!
    std::vector< TH1F* > m_NEt_s;       //!
};


template <class T_PARTICLE, class T_INFOSWITCH>
StatusCode IParticleHists::execute( const xAH::ParticleContainer<T_PARTICLE, T_INFOSWITCH>* particles, float eventWeight, const xAH::EventInfo* eventInfo) {
  using namespace msgIParticleHists;
  unsigned int nPart = particles->size();
  for(unsigned int i = 0;  i<nPart; ++i){
    ANA_CHECK( this->execute( static_cast<const xAH::Particle*>(&particles->at(i)), eventWeight, eventInfo) );

  }

  if( m_infoSwitch->m_numLeading > 0){
    int numParticles = std::min( m_infoSwitch->m_numLeading, (int)particles->size() );
    for(int iParticle=0; iParticle < numParticles; ++iParticle){
      const TLorentzVector& partP4 = particles->at(iParticle).p4;

      m_NPt_l.at(iParticle)->        Fill( partP4.Pt(),   eventWeight);
      m_NPt.at(iParticle)->        Fill( partP4.Pt(),   eventWeight);
      m_NPt_m.at(iParticle)->        Fill( partP4.Pt(),   eventWeight);
      m_NPt_s.at(iParticle)->        Fill( partP4.Pt(),   eventWeight);
      m_NEta.at(iParticle)->       Fill( partP4.Eta(),      eventWeight);
      m_NPhi.at(iParticle)->       Fill( partP4.Phi(),      eventWeight);
      m_NM.at(iParticle)->         Fill( partP4.M(),    eventWeight);
      m_NE.at(iParticle)->         Fill( partP4.E(),    eventWeight);
      m_NRapidity.at(iParticle)->  Fill( partP4.Rapidity(), eventWeight);

      if(m_infoSwitch->m_kinematic){
	m_NEt  .at(iParticle)->        Fill( partP4.Et(),   eventWeight);
	m_NEt_m.at(iParticle)->        Fill( partP4.Et(),   eventWeight);
	m_NEt_s.at(iParticle)->        Fill( partP4.Et(),   eventWeight);
      }

    }
  }

  return StatusCode::SUCCESS;
}


#endif
