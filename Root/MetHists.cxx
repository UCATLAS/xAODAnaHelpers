/********************************************************
 *
 * Met Histogramming
 *
 * J. Alison (john.alison@cern.ch)
 *
 *******************************************************/


#include <xAODAnaHelpers/MetHists.h>
#include <sstream>

#include <AsgTools/MessageCheck.h>

using std::vector;

MetHists :: MetHists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr),
  m_infoSwitch(new HelperClasses::METInfoSwitch(m_detailStr))
{
  m_debug = false;
}

MetHists :: ~MetHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode MetHists::initialize() {

  m_metFinalClus      = book(m_name,  "metFinalClus",      "metFinalClus",      100,     0,    200);
  m_metFinalClusPx    = book(m_name,  "metFinalClusPx",    "metFinalClusPx",    100,  -200,    200);
  m_metFinalClusPy    = book(m_name,  "metFinalClusPy",    "metFinalClusPy",    100,  -200,    200);
  m_metFinalClusSumEt = book(m_name,  "metFinalClusSumEt", "metFinalClusSumEt", 100,     0,   2000);
  m_metFinalClusPhi   = book(m_name,  "metFinalClusPhi",   "metFinalClusPhi",   100,    -3.2,    3.2);

  m_metFinalTrk       = book(m_name, "metFinalTrk",       "metFinalTrk",       100,     0,    200);
  m_metFinalTrkPx     = book(m_name, "metFinalTrkPx",     "metFinalTrkPx",     100,  -200,    200);
  m_metFinalTrkPy     = book(m_name, "metFinalTrkPy",     "metFinalTrkPy",     100,  -200,    200);
  m_metFinalTrkSumEt  = book(m_name, "metFinalTrkSumEt",  "metFinalTrkSumEt",  100,     0,   2000);
  m_metFinalTrkPhi    = book(m_name, "metFinalTrkPhi",    "metFinalTrkPhi",    100,    -3.2,    3.2);

  return StatusCode::SUCCESS;
}

StatusCode MetHists::execute( const xAOD::MissingETContainer* met, float eventWeight ) {

  if(m_debug) std::cout << "MetHists: in execute " <<std::endl;

  //
  // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
  //
  const xAOD::MissingET* final_clus = *met->find("FinalClus");
  m_metFinalClus      -> Fill( final_clus->met()   / 1e3, eventWeight);
  m_metFinalClusPx    -> Fill( final_clus->mpx()   / 1e3, eventWeight);
  m_metFinalClusPy    -> Fill( final_clus->mpy()   / 1e3, eventWeight);
  m_metFinalClusSumEt -> Fill( final_clus->sumet() / 1e3, eventWeight);
  m_metFinalClusPhi   -> Fill( final_clus->phi()        , eventWeight);

  //
  // ("FinalClus" uses the calocluster-based soft terms, "FinalTrk" uses the track-based ones)
  //
  const xAOD::MissingET* final_trk = *met->find("FinalTrk");
  m_metFinalTrk       -> Fill( final_trk->met()   / 1e3,  eventWeight);
  m_metFinalTrkPx     -> Fill( final_trk->mpx()   / 1e3,  eventWeight);
  m_metFinalTrkPy     -> Fill( final_trk->mpy()   / 1e3,  eventWeight);
  m_metFinalTrkSumEt  -> Fill( final_trk->sumet() / 1e3,  eventWeight);
  m_metFinalTrkPhi    -> Fill( final_trk->phi()        ,  eventWeight);

  return StatusCode::SUCCESS;
}

