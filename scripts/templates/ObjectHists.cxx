#include <${package}/${name}Hists.h>

${name}Hists :: ${name}Hists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr)
{
}

${name}Hists :: ~${name}Hists () {}

EL::StatusCode ${name}Hists::initialize() {

  // These plots are always made
  m_jetPt          = book(m_name, "jetPt",  "jet p_{T} [GeV]", 120, 0, 600);

  m_fillPosition = false;
  if( m_detailStr.find("position") != std::string::npos ) {
    m_jetEta         = book(m_name, "jetEta", "jet Eta", 80, -4, 4);
    m_jetPhi         = book(m_name, "jetPhi", "jet Phi",120, -TMath::Pi(), TMath::Pi() );
    m_jetRapidity    = book(m_name, "jetRapidity", "jet Rapidity",120, -10, 10);
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}Hists::execute( const xAOD::JetContainer* jets, float eventWeight ) {
  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();
  for( ; jet_itr != jet_end; ++jet_itr ) {
    this->execute( (*jet_itr), eventWeight );
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}Hists::execute( const xAOD::Jet* jet, float eventWeight ) { 

  //basic
  m_jetPt ->        Fill( jet->pt()/1e3,    eventWeight );

  if( m_fillPosition ){
    m_jetEta->        Fill( jet->eta(),       eventWeight );
    m_jetPhi->        Fill( jet->phi(),       eventWeight );
    m_jetRapidity->   Fill( jet->rapidity(),  eventWeight );
  } // fillPosition

  return EL::StatusCode::SUCCESS;
}

