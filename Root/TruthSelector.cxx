/************************************
 *
 * Truth selector tool
 *
 * J.Alison (john.alison@cern.ch)
 *
 ************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"

// package include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/TruthSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

// external tools include(s):

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(TruthSelector)

TruthSelector :: TruthSelector () :
    Algorithm("TruthSelector")
{
}

EL::StatusCode TruthSelector :: setupJob (EL::Job& job)
{
  ANA_MSG_INFO( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "TruthSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TruthSelector :: histInitialize ()
{
  ANA_MSG_INFO( "Calling histInitialize");
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TruthSelector :: fileExecute ()
{
  ANA_MSG_INFO( "Calling fileExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TruthSelector :: changeInput (bool /*firstFile*/)
{
  ANA_MSG_INFO( "Calling changeInput");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TruthSelector :: initialize ()
{
  ANA_MSG_INFO( "Calling initialize");

  if ( m_useCutFlow ) {

   // retrieve the file in which the cutflow hists are stored
    //
    TFile *file     = wk()->getOutputFile ("cutflow");

    // retrieve the event cutflows
    //
    m_cutflowHist  = (TH1D*)file->Get("cutflow");
    m_cutflowHistW = (TH1D*)file->Get("cutflow_weighted");
    m_cutflow_bin  = m_cutflowHist->GetXaxis()->FindBin(m_name.c_str());
    m_cutflowHistW->GetXaxis()->FindBin(m_name.c_str());

    // retrieve the object cutflow
    //
    m_truth_cutflowHist_1 = (TH1D*)file->Get("cutflow_truths_1");

    m_truth_cutflow_all             = m_truth_cutflowHist_1->GetXaxis()->FindBin("all");
    m_truth_cutflow_ptmax_cut       = m_truth_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");
    m_truth_cutflow_ptmin_cut       = m_truth_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");
    m_truth_cutflow_eta_cut         = m_truth_cutflowHist_1->GetXaxis()->FindBin("eta_cut");

  }

  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_decor   = "passSel";

  if ( m_decorateSelectedObjects ) {
    ANA_MSG_INFO(" Decorate Jets with " << m_decor);
  }


  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  ANA_MSG_INFO( "TruthSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TruthSelector :: execute ()
{
  ANA_MSG_DEBUG( "Applying Jet Selection... ");

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // MC event weight
  float mcEvtWeight(1.0);
  static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
  if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
    ANA_MSG_ERROR( "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }
  mcEvtWeight = mcEvtWeightAcc( *eventInfo );

  m_numEvent++;

  // did any collection pass the cuts?
  bool pass(false);
  bool count(true); // count for the 1st collection in the container - could be better as
                    // shoudl only count for the nominal
  const xAOD::TruthParticleContainer* inTruthParts(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it

  // this will be the collection processed - no matter what!!
  ANA_CHECK( HelperFunctions::retrieve(inTruthParts, m_inContainerName, m_event, m_store, msg()) );

  pass = executeSelection( inTruthParts, mcEvtWeight, count, m_outContainerName);

  // look what we have in TStore
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  if ( !pass ) {
    wk()->skipEvent();
  }

  return EL::StatusCode::SUCCESS;

}

bool TruthSelector :: executeSelection ( const xAOD::TruthParticleContainer* inTruthParts,
					 float mcEvtWeight,
					 bool count,
					 std::string outContainerName
					 )
{

  // create output container (if requested)
  ConstDataVector<xAOD::TruthParticleContainer>* selectedTruthParts(nullptr);
  if ( m_createSelectedContainer ) {
    selectedTruthParts = new ConstDataVector<xAOD::TruthParticleContainer>(SG::VIEW_ELEMENTS);
  }

  int nPass(0); int nObj(0);

  static SG::AuxElement::Decorator< char > passSelDecor( m_decor );

  for ( auto truth_itr : *inTruthParts ) { // duplicated of basic loop

    // if only looking at a subset of jets make sure all are decorated
    if ( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if ( m_decorateSelectedObjects ) {
        passSelDecor( *truth_itr ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    int passSel = this->PassCuts( truth_itr );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *truth_itr ) = passSel;
    }

    if ( passSel ) {
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedTruthParts->push_back( truth_itr );
      }
    }
  }

  if ( count ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  // add ConstDataVector to TStore
  if ( m_createSelectedContainer ) {
    ANA_CHECK( m_store->record( selectedTruthParts, outContainerName ));
  }

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  if ( m_pass_min > 0 && nPass < m_pass_min ) {
    return false;
  }
  if ( m_pass_max > 0 && nPass > m_pass_max ) {
    return false;
  }

  if ( count ) {
    m_numEventPass++;
    m_weightNumEventPass += mcEvtWeight;
  }

  return true;
}


EL::StatusCode TruthSelector :: postExecute ()
{
  ANA_MSG_DEBUG( "Calling postExecute");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TruthSelector :: finalize ()
{
  ANA_MSG_INFO( m_name );

  if ( m_useCutFlow ) {
    ANA_MSG_INFO( "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TruthSelector :: histFinalize ()
{
  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}

int TruthSelector :: PassCuts( const xAOD::TruthParticle* truthPart ) {

  // fill cutflow bin 'all' before any cut
  if(m_useCutFlow) m_truth_cutflowHist_1->Fill( m_truth_cutflow_all, 1 );

  // pT
  if ( m_pT_max != 1e8 ) {
    if ( truthPart->pt() > m_pT_max ) { return 0; }
  }
  if(m_useCutFlow) m_truth_cutflowHist_1->Fill( m_truth_cutflow_ptmax_cut, 1 );

  if ( m_pT_min != 1e8 ) {
    if ( truthPart->pt() < m_pT_min ) { return 0; }
  }
  if(m_useCutFlow) m_truth_cutflowHist_1->Fill( m_truth_cutflow_ptmin_cut, 1 );

  // eta
  if ( m_eta_max != 1e8 ) {
    if ( fabs(truthPart->eta()) > m_eta_max ) { return 0; }
  }
  if ( m_eta_min != 1e8 ) {
    if ( fabs(truthPart->eta()) < m_eta_min ) { return 0; }
  }
  if(m_useCutFlow) m_truth_cutflowHist_1->Fill( m_truth_cutflow_eta_cut, 1 );

  // mass
  if ( m_mass_max != 1e8 ) {
    if ( truthPart->m() > m_mass_max ) { return 0; }
  }
  if ( m_mass_min != 1e8 ) {
    if ( truthPart->m() < m_mass_min ) { return 0; }
  }

  // rapidity
  if ( m_rapidity_max != 1e8 ) {
    if ( truthPart->rapidity() > m_rapidity_max ) { return 0; }
  }

  if ( m_rapidity_min != 1e8 ) {
    if ( truthPart->rapidity() < m_rapidity_min ) { return 0; }
  }

  return 1;
}


