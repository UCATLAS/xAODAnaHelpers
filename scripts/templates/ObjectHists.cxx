#include <${package}/${name}.h>

${name} :: ${name} (std::string name, std::string detailStr, std::string delimiter) :
  HistogramManager(name, detailStr, delimiter)
{
}

${name} :: ~${name} () {}

EL::StatusCode ${name}::initialize() {

  // These plots are always made
  m_${obj_lc}Pt          = book(m_name, "${obj_lc}Pt",  "${obj_lc} p_{T} [GeV]", 120, 0, 600);

  m_fillPosition = false;
  if( m_detailStr.find("position") != std::string::npos ) {
    m_${obj_lc}Eta         = book(m_name, "${obj_lc}Eta", "${obj_lc} #eta", 80, -4, 4);
    m_${obj_lc}Phi         = book(m_name, "${obj_lc}Phi", "${obj_lc} Phi",120, -TMath::Pi(), TMath::Pi() );
    m_${obj_lc}Rapidity    = book(m_name, "${obj_lc}Rapidity", "${obj_lc} Rapidity",120, -10, 10);
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}::execute( const xAOD::${obj}Container* ${obj_lc}s, float eventWeight ) {
  xAOD::${obj}Container::const_iterator ${obj_lc}_itr = ${obj_lc}s->begin();
  xAOD::${obj}Container::const_iterator ${obj_lc}_end = ${obj_lc}s->end();
  for( ; ${obj_lc}_itr != ${obj_lc}_end; ++${obj_lc}_itr ) {
    this->execute( (*${obj_lc}_itr), eventWeight );
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ${name}::execute( const xAOD::${obj}* ${obj_lc}, float eventWeight ) { 

  //basic
  m_${obj_lc}Pt ->        Fill( ${obj_lc}->pt()/1e3,    eventWeight );

  if( m_fillPosition ){
    m_${obj_lc}Eta->        Fill( ${obj_lc}->eta(),       eventWeight );
    m_${obj_lc}Phi->        Fill( ${obj_lc}->phi(),       eventWeight );
    m_${obj_lc}Rapidity->   Fill( ${obj_lc}->rapidity(),  eventWeight );
  } // fillPosition

  return EL::StatusCode::SUCCESS;
}

