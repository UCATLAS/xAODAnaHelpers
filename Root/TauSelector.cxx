/*************************************************
 *
 * Interface to TauAnalysisTools::TauSelectionTool
 *
 * M. Milesi (marco.milesi@cern.ch)
 *
 ************************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODTau/TauJetContainer.h"

// package include(s):
#include "xAODAnaHelpers/TauSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include "PATCore/TAccept.h"

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(TauSelector)


TauSelector :: TauSelector () :
    Algorithm("TauSelector")
{
}

TauSelector::~TauSelector() {}

EL::StatusCode TauSelector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_INFO( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "TauSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_INFO( "Calling histInitialize");
  ANA_CHECK( xAH::Algorithm::algInitialize());
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  ANA_MSG_INFO( "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  ANA_MSG_INFO( "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_INFO( "Initializing TauSelector Interface... ");

  // Let's see if the algorithm has been already used before:
  // if yes, will write object cutflow in a different histogram!
  //
  // This is the case when the selector algorithm is used for
  // preselecting objects, and then again for the final selection
  //
  ANA_MSG_INFO( "Algorithm name: " << m_name << " - of type " << m_className );
  if ( this->numInstances() > 0 ) {
    m_isUsedBefore = true;
    ANA_MSG_INFO( "\t An algorithm of the same type has been already used " << numInstances() << " times" );
  }

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
    m_tau_cutflowHist_1  = (TH1D*)file->Get("cutflow_taus_1");

    m_tau_cutflow_all                  = m_tau_cutflowHist_1->GetXaxis()->FindBin("all");
    m_tau_cutflow_selected             = m_tau_cutflowHist_1->GetXaxis()->FindBin("selected");

    if ( m_isUsedBefore ) {
      m_tau_cutflowHist_2 = (TH1D*)file->Get("cutflow_taus_2");

      m_tau_cutflow_all                  = m_tau_cutflowHist_2->GetXaxis()->FindBin("all");
      m_tau_cutflow_selected             = m_tau_cutflowHist_2->GetXaxis()->FindBin("selected");
    }

  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO( "Number of events in file: " << m_event->getEntries() );


  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

  if ( m_inContainerName.empty() ){
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  // ********************************
  //
  // Initialise TauSelectionTool
  //
  // ********************************

  std::string sel_tool_name = std::string("TauSelectionTool_") + m_name;
  m_TauSelTool = new TauAnalysisTools::TauSelectionTool( sel_tool_name );
  m_TauSelTool->msg().setLevel( MSG::INFO ); // VERBOSE, INFO, DEBUG

  ANA_CHECK( m_TauSelTool->setProperty("ConfigPath",PathResolverFindDataFile(m_ConfigPath).c_str()));
  if ( !m_EleOLRFilePath.empty() ) {
    ANA_CHECK( m_TauSelTool->setProperty("EleOLRFilePath",PathResolverFindDataFile(m_EleOLRFilePath).c_str()));
  }
  ANA_CHECK( m_TauSelTool->initialize());

  if ( m_setTauOverlappingEleLLHDecor ) {
    std::string eleOLR_tool_name = std::string("TauOverlappingElectronLLHDecorator_") + m_name;
    m_TOELLHDecorator = new TauAnalysisTools::TauOverlappingElectronLLHDecorator( eleOLR_tool_name );
    m_TOELLHDecorator->msg().setLevel( MSG::INFO ); // VERBOSE, INFO, DEBUG
    ANA_CHECK( m_TOELLHDecorator->initialize());
  }

  ANA_MSG_INFO( "TauSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TauSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG( "Applying Tau Selection..." );

  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // MC event weight
  //
  float mcEvtWeight(1.0);
  static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
  if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
    ANA_MSG_ERROR( "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }
  mcEvtWeight = mcEvtWeightAcc( *eventInfo );

  m_numEvent++;

  // did any collection pass the cuts?
  //
  bool eventPass(false);
  bool countPass(true); // for cutflow: count for the 1st collection in the syst container - could be better as should only count for the nominal
  const xAOD::TauJetContainer* inTaus(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  //
  if ( m_inputAlgoSystNames.empty() ) {

    // this will be the collection processed - no matter what!!
    //
    ANA_CHECK( HelperFunctions::retrieve(inTaus, m_inContainerName, m_event, m_store, msg()) );

    // create output container (if requested)
    //
    ConstDataVector<xAOD::TauJetContainer>* selectedTaus(nullptr);
    if ( m_createSelectedContainer ) { selectedTaus = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS); }

    // find the selected taus, and return if event passes object selection
    //
    eventPass = executeSelection( inTaus, mcEvtWeight, countPass, selectedTaus );

    if ( m_createSelectedContainer ) {
      if ( eventPass ) {
        // add ConstDataVector to TStore
	//
        ANA_CHECK( m_store->record( selectedTaus, m_outContainerName ));
      } else {
        // if the event does not pass the selection, CDV won't be ever recorded to TStore, so we have to delete it!
	//
        delete selectedTaus; selectedTaus = nullptr;
      }
    }

  } else { // get the list of systematics to run over

    // get vector of string giving the syst names of the upstream algo from TStore (rememeber: 1st element is a blank string: nominal case!)
    //
    std::vector< std::string >* systNames(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, msg()) );

    // prepare a vector of the names of CDV containers for usage by downstream algos
    // must be a pointer to be recorded in TStore
    //
    std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;
    ANA_MSG_DEBUG( " input list of syst size: " << static_cast<int>(systNames->size()) );

    // loop over systematic sets
    //
    bool eventPassThisSyst(false);
    for ( auto systName : *systNames ) {

      ANA_MSG_DEBUG( " syst name: " << systName << "  input container name: " << m_inContainerName+systName );

      ANA_CHECK( HelperFunctions::retrieve(inTaus, m_inContainerName + systName, m_event, m_store, msg()) );

      // create output container (if requested) - one for each systematic
      //
      ConstDataVector<xAOD::TauJetContainer>* selectedTaus(nullptr);
      if ( m_createSelectedContainer ) { selectedTaus = new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS); }

      // find the selected Taus, and return if event passes object selection
      //
      eventPassThisSyst = executeSelection( inTaus, mcEvtWeight, countPass, selectedTaus );

      if ( countPass ) { countPass = false; } // only count objects/events for 1st syst collection in iteration (i.e., nominal)

      if ( eventPassThisSyst ) {
	// save the string of syst set under question if event is passing the selection
	//
	vecOutContainerNames->push_back( systName );
      }

      // if for at least one syst set the event passes selection, this will remain true!
      //
      eventPass = ( eventPass || eventPassThisSyst );

      ANA_MSG_DEBUG( " syst name: " << systName << "  output container name: " << m_outContainerName+systName );

      if ( m_createSelectedContainer ) {
        if ( eventPassThisSyst ) {
          // add ConstDataVector to TStore
	  //
          ANA_CHECK( m_store->record( selectedTaus, m_outContainerName+systName ));
        } else {
          // if the event does not pass the selection for this syst, CDV won't be ever recorded to TStore, so we have to delete it!
          delete selectedTaus; selectedTaus = nullptr;
        }
      }

    } // close loop over syst sets

    ANA_MSG_DEBUG(" output list of syst size: " << static_cast<int>(vecOutContainerNames->size()) );

    // record in TStore the list of systematics names that should be considered down stream
    //
    ANA_CHECK( m_store->record( vecOutContainerNames, m_outputAlgoSystNames));

  }

  // look what we have in TStore
  //
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  if( !eventPass ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  return EL::StatusCode::SUCCESS;

}

bool TauSelector :: executeSelection ( const xAOD::TauJetContainer* inTaus, float mcEvtWeight, bool countPass,
				       ConstDataVector<xAOD::TauJetContainer>* selectedTaus )
{

  int nPass(0); int nObj(0);
  static SG::AuxElement::Decorator< char > passSelDecor( "passSel" );

  ANA_MSG_DEBUG( "Initial Taus: " << static_cast<uint32_t>(inTaus->size()) );

  for ( auto tau_itr : *inTaus ) { // duplicated of basic loop

    // if only looking at a subset of Taus make sure all are decorated
    //
    if ( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if ( m_decorateSelectedObjects ) {
        passSelDecor( *tau_itr ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    bool passSel = this->passCuts( tau_itr );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *tau_itr ) = passSel;
    }

    if ( passSel ) {
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedTaus->push_back( tau_itr );
      }
    }
  }

  // for cutflow: make sure to count passed objects only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  ANA_MSG_DEBUG( "Initial Taus (count obj): " << nObj << " - Selected Taus: " << nPass );

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  //
  if ( m_pass_min > 0 && nPass < m_pass_min ) {
    ANA_MSG_DEBUG( "Reject event: nSelectedTaus ("<<nPass<<") < nPassMin ("<<m_pass_min<<")" );
    return false;
  }
  if ( m_pass_max > 0 && nPass > m_pass_max ) {
    ANA_MSG_DEBUG( "Reject event: nSelectedTaus ("<<nPass<<") > nPassMax ("<<m_pass_max<<")" );
    return false;
  }

  // for cutflow: make sure to count passed events only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ){
    m_numEventPass++;
    m_weightNumEventPass += mcEvtWeight;
  }

  return true;
}


EL::StatusCode TauSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG( "Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauSelector :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  ANA_MSG_INFO( "Deleting tool instances...");

  if ( m_TauSelTool )      { m_TauSelTool = nullptr;      delete m_TauSelTool; }
  if ( m_TOELLHDecorator ) { m_TOELLHDecorator = nullptr; delete m_TOELLHDecorator; }

  if ( m_useCutFlow ) {
    ANA_MSG_INFO( "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode TauSelector :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.

  ANA_MSG_INFO( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}

int TauSelector :: passCuts( const xAOD::TauJet* tau ) {

  // fill cutflow bin 'all' before any cut
  if(m_useCutFlow) m_tau_cutflowHist_1->Fill( m_tau_cutflow_all, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_tau_cutflowHist_2->Fill( m_tau_cutflow_all, 1 ); }

  // **********************************************************************************************************
  //
  // TauSelectorTool cut
  //

  if ( tau->pt() <= m_minPtDAOD ) {
    ANA_MSG_DEBUG( "Tau failed minimal pT requirement for usage with derivations");
    return 0;
  }

  if ( m_setTauOverlappingEleLLHDecor ) { m_TOELLHDecorator->decorate( *tau ); }

  if ( ! m_TauSelTool->accept( *tau ) ) {
    ANA_MSG_DEBUG( "Tau failed requirements of TauSelectionTool");
    return 0;
  }

  if(m_useCutFlow) m_tau_cutflowHist_1->Fill( m_tau_cutflow_selected, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_tau_cutflowHist_2->Fill( m_tau_cutflow_selected, 1 ); }

  return 1;
}

