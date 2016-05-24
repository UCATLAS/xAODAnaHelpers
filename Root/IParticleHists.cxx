#include <xAODAnaHelpers/IParticleHists.h>
#include <sstream>

#include "xAODAnaHelpers/tools/ReturnCheck.h"

using std::vector;

IParticleHists :: IParticleHists (std::string name, std::string detailStr, std::string prefix, std::string title) :
  HistogramManager(name, detailStr),
  m_infoSwitch(new HelperClasses::IParticleInfoSwitch(m_detailStr)),
  m_prefix(prefix),m_title(title)
{
  m_debug = false;
}

IParticleHists :: ~IParticleHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode IParticleHists::initialize() {

  // These plots are always made
  m_Pt_l        = book(m_name, m_prefix+"Pt_l",     m_title+" p_{T} [GeV]", 120, 0, 3000.);
  m_Pt          = book(m_name, m_prefix+"Pt",       m_title+" p_{T} [GeV]", 100, 0, 1000.);
  m_Pt_m        = book(m_name, m_prefix+"Pt_m",     m_title+" p_{T} [GeV]", 100, 0,  500.);
  m_Pt_s        = book(m_name, m_prefix+"Pt_s",     m_title+" p_{T} [GeV]", 100, 0,  100.);
  m_Eta         = book(m_name, m_prefix+"Eta",      m_title+" #eta",         80, -4, 4);
  m_Phi         = book(m_name, m_prefix+"Phi",      m_title+" Phi",         120, -TMath::Pi(), TMath::Pi() );
  m_M           = book(m_name, m_prefix+"Mass",     m_title+" Mass [GeV]",  120, 0, 400);
  m_E           = book(m_name, m_prefix+"Energy",   m_title+" Energy [GeV]",120, 0, 4000.);
  m_Rapidity    = book(m_name, m_prefix+"Rapidity", m_title+" Rapidity",    120, -10, 10);

  if(m_debug) Info("IParticleHists::initialize()", m_name.c_str());
  // details of the particle kinematics
  if( m_infoSwitch->m_kinematic ) {
    if(m_debug) Info("IParticleHists::initialize()", "adding kinematic plots");
    m_Px     = book(m_name, m_prefix+"Px",     m_title+" Px [GeV]",     120, 0, 1000);
    m_Py     = book(m_name, m_prefix+"Py",     m_title+" Py [GeV]",     120, 0, 1000);
    m_Pz     = book(m_name, m_prefix+"Pz",     m_title+" Pz [GeV]",     120, 0, 4000);
  }

  // N leading jets
  if( m_infoSwitch->m_numLeading > 0 ){
    std::stringstream pNum;
    std::stringstream pTitle;
    for(int iParticle=0; iParticle < m_infoSwitch->m_numLeading; ++iParticle){
      pNum << iParticle;

      pTitle << iParticle+1;
      switch(iParticle)
	{
	case 0:
	  pTitle << "^{st}";
	  break;
	case 1:
	  pTitle << "^{nd}";
	  break;
	case 2:
	  pTitle << "^{rd}";
	  break;
	default:
	  pTitle << "^{th}";
	  break;
	}

      m_NPt.push_back(       book(m_name, (m_prefix+"Pt_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,120,            0,       3000. ) );
      m_NEta.push_back(      book(m_name, (m_prefix+"Eta_"+pNum.str()),      pTitle.str()+" "+m_title+" #eta"        , 80,           -4,           4 ) );
      m_NPhi.push_back(      book(m_name, (m_prefix+"Phi_"+pNum.str()),      pTitle.str()+" "+m_title+" Phi"         ,120, -TMath::Pi(), TMath::Pi() ) );
      m_NM.push_back(        book(m_name, (m_prefix+"Mass_"+pNum.str()),     pTitle.str()+" "+m_title+" Mass [GeV]"  ,120,            0,         400 ) );
      m_NE.push_back(        book(m_name, (m_prefix+"Energy_"+pNum.str()),   pTitle.str()+" "+m_title+" Energy [GeV]",120,            0,       4000. ) );
      m_NRapidity.push_back( book(m_name, (m_prefix+"Rapidity_"+pNum.str()), pTitle.str()+" "+m_title+" Rapidity"    ,120,          -10,          10 ) );
      pNum.str("");
      pTitle.str("");
    }//for iParticle
  }

  return StatusCode::SUCCESS;
}

StatusCode IParticleHists::execute( const xAOD::IParticleContainer* particles, float eventWeight, const xAOD::EventInfo* eventInfo) {
  for( auto particle_itr : *particles ) {
    RETURN_CHECK("IParticleHists::execute()", this->execute( particle_itr, eventWeight, eventInfo), "");
  }

  if( m_infoSwitch->m_numLeading > 0){
    int numParticles = std::min( m_infoSwitch->m_numLeading, (int)particles->size() );
    for(int iParticle=0; iParticle < numParticles; ++iParticle){
      m_NPt.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NEta.at(iParticle)->       Fill( particles->at(iParticle)->eta(),      eventWeight);
      m_NPhi.at(iParticle)->       Fill( particles->at(iParticle)->phi(),      eventWeight);
      m_NM.at(iParticle)->         Fill( particles->at(iParticle)->m()/1e3,    eventWeight);
      m_NE.at(iParticle)->         Fill( particles->at(iParticle)->e()/1e3,    eventWeight);
      m_NRapidity.at(iParticle)->  Fill( particles->at(iParticle)->rapidity(), eventWeight);
    }
  }

  return StatusCode::SUCCESS;
}

StatusCode IParticleHists::execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* /*eventInfo*/ ) {

  if(m_debug) std::cout << "IParticleHists: in execute " <<std::endl;

  //basic
  m_Pt_l ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Pt ->        Fill( particle->pt()/1e3,    eventWeight );
  m_Pt_m ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Pt_s ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Eta->        Fill( particle->eta(),       eventWeight );
  m_Phi->        Fill( particle->phi(),       eventWeight );
  m_M->          Fill( particle->m()/1e3,     eventWeight );
  m_E->          Fill( particle->e()/1e3,     eventWeight );
  m_Rapidity->   Fill( particle->rapidity(),  eventWeight );

  // kinematic
  if( m_infoSwitch->m_kinematic ) {
    m_Px->  Fill( particle->p4().Px()/1e3,  eventWeight );
    m_Py->  Fill( particle->p4().Py()/1e3,  eventWeight );
    m_Pz->  Fill( particle->p4().Pz()/1e3,  eventWeight );
  } // fillKinematic

  return StatusCode::SUCCESS;
}

