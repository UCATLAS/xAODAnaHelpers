/*************************************************
 *
 * Interface to TauAnalysisTools::TauSelectionTool
 *
 * F. Scutti (federico.scutti@cern.ch)
 * M. Milesi (marco.milesi@cern.ch)
 *
 ************************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <map>

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
// tool includes
#include "TauAnalysisTools/TauSelectionTool.h"  
#include "TriggerMatchingTool/MatchingTool.h"

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

  if ( this->numInstances() > 1 ) {
    m_isUsedBefore = true;
    ANA_MSG_INFO( "\t An algorithm of the same type has been already used " << numInstances() << " times" );
  }

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

  // IMPORTANT: if no working point is specified the one in this configuration will be used
  ANA_CHECK( m_tauSelTool_handle.setProperty("ConfigPath",PathResolverFindDataFile(m_ConfigPath).c_str()));
  if (!m_JetIDWP.empty()) {
    
    std::map <std::string, int> jetid_wp_map;
    
    jetid_wp_map["JETIDNONE"] = int(TauAnalysisTools::JETIDNONE);
    jetid_wp_map["JETIDBDTLOOSE"] = int(TauAnalysisTools::JETIDBDTLOOSE);
    jetid_wp_map["JETIDBDTMEDIUM"] = int(TauAnalysisTools::JETIDBDTMEDIUM);
    jetid_wp_map["JETIDBDTTIGHT"] = int(TauAnalysisTools::JETIDBDTTIGHT);
    jetid_wp_map["JETIDBDTLOOSENOTTIGHT"] = int(TauAnalysisTools::JETIDBDTLOOSENOTTIGHT);
    jetid_wp_map["JETIDBDTLOOSENOTMEDIUM"] = int(TauAnalysisTools::JETIDBDTLOOSENOTMEDIUM);
    jetid_wp_map["JETIDBDTMEDIUMNOTTIGHT"] = int(TauAnalysisTools::JETIDBDTMEDIUMNOTTIGHT);
    jetid_wp_map["JETIDBDTNOTLOOSE"] = int(TauAnalysisTools::JETIDBDTNOTLOOSE);
    jetid_wp_map["JETIDBDTVERYLOOSE"] = int(TauAnalysisTools::JETIDBDTVERYLOOSE);
    
    if (jetid_wp_map.count(m_JetIDWP) != 0 ) {
      ANA_CHECK( m_tauSelTool_handle.setProperty("JetIDWP", jetid_wp_map[m_JetIDWP]));
    } else {
      ANA_MSG_ERROR( "Unknown requested tau JetIDWP " << m_JetIDWP);
      return EL::StatusCode::FAILURE; 
    }
  }

  if (!m_EleBDTWP.empty()) {
    
    std::map <std::string, int> elebdt_wp_map;
    
    elebdt_wp_map["ELEIDNONE"] = int(TauAnalysisTools::ELEIDNONE);
    elebdt_wp_map["ELEIDBDTLOOSE"] = int(TauAnalysisTools::ELEIDBDTLOOSE);
    elebdt_wp_map["ELEIDBDTMEDIUM"] = int(TauAnalysisTools::ELEIDBDTMEDIUM);
    elebdt_wp_map["ELEIDBDTTIGHT"] = int(TauAnalysisTools::ELEIDBDTTIGHT);
    
    if (elebdt_wp_map.count(m_EleBDTWP) != 0 ) {
      ANA_CHECK( m_tauSelTool_handle.setProperty("EleBDTWP", elebdt_wp_map[m_EleBDTWP]));
    } else {
      ANA_MSG_ERROR( "Unknown requested tau EleBDTWP " << m_EleBDTWP);
      return EL::StatusCode::FAILURE; 
    }
  }

  ANA_CHECK( m_tauSelTool_handle.setProperty("EleOLR", m_EleOLR));

  ANA_CHECK(m_tauSelTool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_tauSelTool_handle);

  // **************************************
  //
  // Initialise Trig::MatchingTool
  //
  // **************************************
  if( !( m_singleTauTrigChains.empty() && m_diTauTrigChains.empty() ) ) {
    // Grab the TrigDecTool from the ToolStore
    if(!m_trigDecTool_handle.isUserConfigured()){
      ANA_MSG_FATAL("A configured " << m_trigDecTool_handle.typeAndName() << " must have been previously created! Are you creating one in xAH::BasicEventSelection?" );
      return EL::StatusCode::FAILURE;
    }
    ANA_CHECK( m_trigDecTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecTool_handle);

    //  everything went fine, let's initialise the tool!
    ANA_CHECK( m_trigTauMatchTool_handle.setProperty( "TrigDecisionTool", m_trigDecTool_handle ));
    ANA_CHECK( m_trigTauMatchTool_handle.setProperty("OutputLevel", msg().level() ));
    ANA_CHECK( m_trigTauMatchTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigTauMatchTool_handle);

  } else {

    m_doTrigMatch = false;

    ANA_MSG_WARNING("***********************************************************");
    ANA_MSG_WARNING( "Will not perform any tau trigger matching at this stage b/c :");
    ANA_MSG_WARNING("\t -) could not find the TrigDecisionTool in asg::ToolStore");
    ANA_MSG_WARNING("\t AND/OR");
    ANA_MSG_WARNING("\t -) all input HLT trigger chain lists are empty");
    ANA_MSG_WARNING("However, if you really didn't want to do the matching now, it's all good!");
    ANA_MSG_WARNING("***********************************************************");
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

  // QUESTION: why this must be done in execute(), and does not work in initialize()?
  //
  if ( m_numEvent == 1 && m_trigDecTool_handle.isInitialized() ) {

    // parse input tau trigger chain list, split by comma and fill vector
    //
    std::string singletau_trig;
    std::istringstream ss_singletau_trig(m_singleTauTrigChains);

    while ( std::getline(ss_singletau_trig, singletau_trig, ',') ) {
   	m_singleTauTrigChainsList.push_back(singletau_trig);
    }

    std::string ditau_trig;
    std::istringstream ss_ditau_trig(m_diTauTrigChains);

    while ( std::getline(ss_ditau_trig, ditau_trig, ',') ) {
   	m_diTauTrigChainsList.push_back(ditau_trig);
    }

    ANA_MSG_INFO( "Input single tau trigger chains that will be considered for matching:\n");
    for ( auto const &chain : m_singleTauTrigChainsList ) { ANA_MSG_INFO( "\t " << chain); }
    ANA_MSG_INFO( "\n");

    ANA_MSG_INFO( "Input di-tau trigger chains that will be considered for matching:\n");
    for ( auto const &chain : m_diTauTrigChainsList ) { ANA_MSG_INFO( "\t " << chain); }
    ANA_MSG_INFO( "\n");

  }

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


  // Perform trigger matching on the "good" (selected) taus
  //
  // NB: this part will be skipped if:
  //
  //  1. the user didn't pass any trigger chains to the algo (see initialize(): in that case, the tool is not even initialised!)
  //  2. there are no selected taus in the event
  //
  if ( m_doTrigMatch && selectedTaus ) {

    unsigned int nSelectedTaus = selectedTaus->size();

    static SG::AuxElement::Decorator< std::map<std::string,char> > isTrigMatchedMapTauDecor( "isTrigMatchedMapTau" );

    if ( nSelectedTaus > 0 ) {

      ANA_MSG_DEBUG( "Doing single tau trigger matching...");

      for ( auto const &chain : m_singleTauTrigChainsList ) {

        ANA_MSG_DEBUG( "\t checking trigger chain " << chain);

        for ( auto const tau : *selectedTaus ) {

          //  For each tau, decorate w/ a map<string,char> with the 'isMatched' info associated
          //  to each trigger chain in the input list.
          //  If decoration map doesn't exist for this tau yet, create it (will be done only for the 1st iteration on the chain names)
          //
          if ( !isTrigMatchedMapTauDecor.isAvailable( *tau ) ) {
            isTrigMatchedMapTauDecor( *tau ) = std::map<std::string,char>();
          }

          // check whether the tau is matched (NOTE: no DR is required for taus)
          //
          char matched = ( m_trigTauMatchTool_handle->match( *tau, chain ) );

          ANA_MSG_DEBUG( "\t\t is tau trigger matched? " << matched);

          ( isTrigMatchedMapTauDecor( *tau ) )[chain] = matched;
        }
      }

    }

    // If checking dilepton trigger, form lepton pairs and test matching for each one.
    // Save a:
    //
    // multimap< chain, pair< pair<idx_i, idx_j>, ismatched > >
    //
    // as *event* decoration to store which
    // pairs are matched (to a given chain) and which aren't.
    // A multimap is used b/c a given key (i.e., a chain) can be associated to more than one pair. This is the case for e.g., trilepton events.
    //
    // By retrieving this map later on, user can decide what to do with the event
    // (Generally one could just loop over the map and save a flag if there's at least one pair that matches a given chain)

    if ( nSelectedTaus > 1 && !m_diTauTrigChains.empty() ) {

      ANA_MSG_DEBUG( "Doing di-tau trigger matching...");

      const xAOD::EventInfo* eventInfo(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

      typedef std::pair< std::pair<unsigned int,unsigned int>, char> ditau_trigmatch_pair;
      typedef std::multimap< std::string, ditau_trigmatch_pair >    ditau_trigmatch_pair_map;
      static SG::AuxElement::Decorator< ditau_trigmatch_pair_map >  diTauTrigMatchPairMapDecor( "diTauTrigMatchPairMap" );

      for ( auto const &chain : m_diTauTrigChainsList ) {

      	ANA_MSG_DEBUG( "\t checking trigger chain " << chain);

      	//  If decoration map doesn't exist for this event yet, create it (will be done only for the 1st iteration on the chain names)
      	//
      	if ( !diTauTrigMatchPairMapDecor.isAvailable( *eventInfo ) ) {
          diTauTrigMatchPairMapDecor( *eventInfo ) = ditau_trigmatch_pair_map();
      	}

      	std::vector<const xAOD::IParticle*> myTaus;

      	for ( unsigned int itau = 0; itau < selectedTaus->size()-1; ++itau ) {

      	  for ( unsigned int jtau = itau+1; jtau < selectedTaus->size(); ++jtau ) {

            // test a new pair
            //
      	    myTaus.clear();
      	    myTaus.push_back( selectedTaus->at(itau) );
      	    myTaus.push_back( selectedTaus->at(jtau) );

            // check whether the pair is matched (NOTE: no DR is required for taus)
            //
      	    char matched = m_trigTauMatchTool_handle->match( myTaus, chain  );

      	    ANA_MSG_DEBUG( "\t\t is the tau pair ("<<itau<<","<<jtau<<") trigger matched? " << matched);

      	    std::pair <unsigned int, unsigned int>  chain_idxs = std::make_pair(itau,jtau);
            ditau_trigmatch_pair  chain_decision = std::make_pair(chain_idxs,matched);
            diTauTrigMatchPairMapDecor( *eventInfo ).insert( std::pair< std::string, ditau_trigmatch_pair >(chain,chain_decision) );

      	  }
      	}
      } //for m_diTauTrigChainsList
    } //if nSelectedTaus > 1 && !m_diTauTrigChains.empty()
  } //if m_doTrigMatch && selectedTaus

  ANA_MSG_DEBUG( "Left  executeSelection..." );

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
  if(!m_isUsedBefore && m_useCutFlow) m_tau_cutflowHist_1->Fill( m_tau_cutflow_all, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_tau_cutflowHist_2->Fill( m_tau_cutflow_all, 1 ); }

  // **********************************************************************************************************
  //
  // TauSelectorTool cut
  //

  // JetBDTSigID decoration
  // ----------------------
  static SG::AuxElement::Decorator< int > isJetBDTSigVeryLoose("isJetBDTSigVeryLoose");
  static SG::AuxElement::Decorator< int > isJetBDTSigLoose("isJetBDTSigLoose");
  static SG::AuxElement::Decorator< int > isJetBDTSigMedium("isJetBDTSigMedium");
  static SG::AuxElement::Decorator< int > isJetBDTSigTight("isJetBDTSigTight");

  static SG::AuxElement::Decorator< float > JetBDTScore("JetBDTScore");
  static SG::AuxElement::Decorator< float > JetBDTScoreSigTrans("JetBDTScoreSigTrans");
  

  isJetBDTSigVeryLoose( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::JetBDTSigVeryLoose));
  isJetBDTSigLoose( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::JetBDTSigLoose));
  isJetBDTSigMedium( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::JetBDTSigMedium));
  isJetBDTSigTight( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::JetBDTSigTight));

  JetBDTScore( *tau ) = static_cast<float>(tau->discriminant(xAOD::TauJetParameters::BDTJetScore));
  JetBDTScoreSigTrans( *tau ) = static_cast<float>(tau->discriminant(xAOD::TauJetParameters::BDTJetScoreSigTrans));

  // EleBDT decoration
  // -----------------
  static SG::AuxElement::Decorator< int > isEleBDTLoose("isEleBDTLoose");
  static SG::AuxElement::Decorator< int > isEleBDTMedium("isEleBDTMedium");
  static SG::AuxElement::Decorator< int > isEleBDTTight("isEleBDTTight");

  static SG::AuxElement::Decorator< float > EleBDTScore("EleBDTScore");
  

  isEleBDTLoose( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::EleBDTLoose));
  isEleBDTMedium( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::EleBDTMedium));
  isEleBDTTight( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::EleBDTTight));

  EleBDTScore( *tau ) = static_cast<float>(tau->discriminant(xAOD::TauJetParameters::BDTEleScore));


  // EleOLR decoration
  // -----------------
  static SG::AuxElement::Decorator< int > passEleOLR("passEleOLR");
  
  passEleOLR( *tau ) = static_cast<int>(tau->isTau(xAOD::TauJetParameters::PassEleOLR));


  ANA_MSG_DEBUG( "Got decoration values" );


  if ( tau->pt() <= m_minPtDAOD ) {
    ANA_MSG_DEBUG( "Tau failed minimal pT requirement for usage with derivations");
    return 0;
  }

  if ( ! m_tauSelTool_handle->accept( *tau ) ) {
    ANA_MSG_DEBUG( "Tau failed requirements of TauSelectionTool");
    return 0;
  }

  if( !m_isUsedBefore && m_useCutFlow) m_tau_cutflowHist_1->Fill( m_tau_cutflow_selected, 1 );
  if ( m_isUsedBefore && m_useCutFlow ) { m_tau_cutflowHist_2->Fill( m_tau_cutflow_selected, 1 ); }

  return 1;
}

