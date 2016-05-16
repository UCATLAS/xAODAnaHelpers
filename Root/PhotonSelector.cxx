/********************************************
 *
 * Interface to CP Photon selection tool(s).
 *
 *******************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include <xAODEventInfo/EventInfo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include <xAODAnaHelpers/PhotonSelector.h>
#include <xAODEgamma/EgammaDefs.h>
#include <xAODEgamma/EgammaxAODHelpers.h>


#include <IsolationSelection/IsolationSelectionTool.h>
#include <TrigDecisionTool/TrigDecisionTool.h>

// ROOT include(s):
#include <TFile.h>
#include <TObjArray.h>
#include <TObjString.h>


// this is needed to distribute the algorithm to the workers
ClassImp(PhotonSelector)

PhotonSelector :: PhotonSelector (std::string className) :
    Algorithm(className),
    m_cutflowHist(nullptr),
    m_cutflowHistW(nullptr),
    m_ph_cutflowHist_1(nullptr),
    m_IsolationSelectionTool(nullptr),
    m_trigDecTool(nullptr)
//m_match_Tool(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("PhotonSelector()", "Calling constructor");

  m_debug                   = false;
  m_useCutFlow              = true;

  // input container to be read from TEvent or TStore
  //
  m_inContainerName         = "";

  // Systematics stuff
  //
  m_inputAlgoSystNames      = "";
  m_outputAlgoSystNames     = "PhotonSelector_Syst";


  // decorate selected objects that pass the cuts
  //
  m_decorateSelectedObjects = true;
  // additional functionality : create output container of selected objects
  //                            using the SG::VIEW_ELEMENTS option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainer = true;
  // if requested, a new container is made using the SG::VIEW_ELEMENTS option
  m_outContainerName        = "";

  // if only want to look at a subset of object
  //
  m_nToProcess              = -1;

  // configurable cuts
  //
  m_pass_max                = -1;
  m_pass_min                = -1;
  m_pT_max                  = 1e8;
  m_pT_min                  = 1e8;
  m_eta_max                 = 1e8;
  m_vetoCrack               = true;
  m_doAuthorCut             = true;
  m_doOQCut                 = true;

  // PID
  m_photonIdCut             = "None";

  // isolation stuff
  //
  m_MinIsoWPCut             = "";
  m_IsoWPList		    = "FixedCutTightCaloOnly,FixedCutTight,FixedCutLoose";

  // trigger matching stuff
  //
  m_PhTrigChains            = "";

}

PhotonSelector::~PhotonSelector() {}

EL::StatusCode PhotonSelector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "PhotonSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "Calling histInitialize");
  RETURN_CHECK("xAH::Algorithm::algInitialize()", xAH::Algorithm::algInitialize(), "");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PhotonSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing PhotonSelector Interface... ");

  // Let's see if the algorithm has been already used before:
  // if yes, will write object cutflow in a different histogram!
  //
  // This is the case when the selector algorithm is used for
  // preselecting objects, and then again for the final selection
  //
  Info("initialize()", "Algorithm name: '%s' - of type '%s' ", (this->m_name).c_str(), (this->m_className).c_str() );

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
    m_ph_cutflowHist_1 = (TH1D*)file->Get("cutflow_photons_1");

    m_ph_cutflow_all             = m_ph_cutflowHist_1->GetXaxis()->FindBin("all");
    m_ph_cutflow_author_cut      = m_ph_cutflowHist_1->GetXaxis()->FindBin("author_cut");
    m_ph_cutflow_OQ_cut          = m_ph_cutflowHist_1->GetXaxis()->FindBin("OQ_cut");
    m_ph_cutflow_PID_cut         = m_ph_cutflowHist_1->GetXaxis()->FindBin("PID_cut");
    m_ph_cutflow_ptmax_cut       = m_ph_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");
    m_ph_cutflow_ptmin_cut       = m_ph_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");
    m_ph_cutflow_eta_cut         = m_ph_cutflowHist_1->GetXaxis()->FindBin("eta_cut"); // including crack veto, if applied
    m_ph_cutflow_iso_cut         = m_ph_cutflowHist_1->GetXaxis()->FindBin("iso_cut");


  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events in file: %lld ", m_event->getEntries() );

  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

  // Parse input isolation WP list, split by comma, and put into a vector for later use
  // Make sure it's not empty!
  //
  std::string token;
  std::istringstream ss(m_IsoWPList);
  while ( std::getline(ss, token, ',') ) {
    m_IsoKeys.push_back(token);
  }

  if ( m_inContainerName.empty() ) {
    Error("initialize()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }


  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  // *************************************
  //
  // Initialise CP::IsolationSelectionTool
  //
  // *************************************
  const std::string isoToolName = m_name+"_PhotonIso";
  if ( asg::ToolStore::contains<CP::IsolationSelectionTool>(isoToolName.c_str()) ) {
    m_IsolationSelectionTool = asg::ToolStore::get<CP::IsolationSelectionTool>(isoToolName.c_str());
  } else {
    m_IsolationSelectionTool = new CP::IsolationSelectionTool(isoToolName.c_str());
  }
  if ( m_debug ) { Info("initialize()", "Adding isolation WP %s to IsolationSelectionTool", (m_IsoKeys.at(0)).c_str() ); }
  RETURN_CHECK( "PhotonSelector::initialize()", m_IsolationSelectionTool->setProperty("PhotonWP", (m_IsoKeys.at(0)).c_str()), "Failed to configure base WP" );
  RETURN_CHECK( "PhotonSelector::initialize()", m_IsolationSelectionTool->initialize(), "Failed to properly initialize IsolationSelectionTool." );

  for ( auto WP_itr = std::next(m_IsoKeys.begin()); WP_itr != m_IsoKeys.end(); ++WP_itr ) {
    if ( m_debug ) { Info("initialize()", "Adding extra isolation WP %s to IsolationSelectionTool", (*WP_itr).c_str() ); }
    RETURN_CHECK( "PhotonSelector::initialize()", m_IsolationSelectionTool->addPhotonWP( (*WP_itr).c_str() ), "Failed to add isolation WP" );
  }

  // ***************************************
  //
  // Initialise Trig::TrigEgammaMatchingTool
  //
  // ***************************************
  if( !m_PhTrigChains.empty()) {
    std::string trig;
    std::istringstream ss(m_PhTrigChains);

    while ( std::getline(ss, trig, ',') ) {
      m_PhTrigChainsList.push_back(trig);
    }

    if ( asg::ToolStore::contains<Trig::TrigDecisionTool>( "TrigDecisionTool" ) ) {
      m_trigDecTool = asg::ToolStore::get<Trig::TrigDecisionTool>("TrigDecisionTool");
    } else {
      Error("Initialize()", "the Trigger Decision Tool is not initialized.. [%s]", m_name.c_str());
      return EL::StatusCode::FAILURE;
    }

    ToolHandle<Trig::TrigDecisionTool> trigDecHandle( m_trigDecTool );

    //  everything went fine, let's initialise the tool!
    //
    const std::string MatchingToolName = m_name+"_TrigEgammaMatchingTool";
    //    if ( asg::ToolStore::contains<Trig::TrigEgammaMatchingTool>(MatchingToolName.c_str()) ) {
      //m_match_Tool = asg::ToolStore::get<Trig::TrigEgammaMatchingTool>(MatchingToolName.c_str());
    //} else {
      //m_match_Tool = new Trig::TrigEgammaMatchingTool(MatchingToolName.c_str());
      //RETURN_CHECK( "PhotonSelector::initialize()", m_match_Tool->setProperty( "TriggerTool", trigDecHandle ), "Failed to configure TrigDecisionTool" );
      //RETURN_CHECK( "PhotonSelector::initialize()", m_match_Tool->initialize(), "Failed to properly initialize TrigMuonMatching." );
    //}
  }

  // **********************************************************************************************

  Info("initialize()", "PhotonSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode PhotonSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Applying Photon Selection... "); }

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("PhotonSelector::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,
	       Form("Failed in retrieving %s in %s", m_eventInfoContainerName.c_str(), m_name.c_str() ));

  // MC event weight
  //
  float mcEvtWeight(1.0);
  static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
  if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
    Error("execute()  ", "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }
  mcEvtWeight = mcEvtWeightAcc( *eventInfo );

  m_numEvent++;

  bool eventPass(false);
  bool countPass(true); // for cutflow: count for the 1st collection in the syst container - could be better as should only count for the nominal
  const xAOD::PhotonContainer* inPhotons(nullptr);

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  //
  if ( m_inputAlgoSystNames.empty() ) {

    // this will be the collection processed - no matter what!!
    //
    RETURN_CHECK("PhotonSelector::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName, m_event, m_store, m_verbose) , "");

    // create output container (if requested)
    ConstDataVector<xAOD::PhotonContainer>* selectedPhotons(nullptr);
    if ( m_createSelectedContainer ) { selectedPhotons = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS); }

    // find the selected photons, and return if event passes object selection
    //
    eventPass = executeSelection(inPhotons, mcEvtWeight, countPass, selectedPhotons );

    if ( m_createSelectedContainer) {
      if ( eventPass ) {
        // add ConstDataVector to TStore
	//
        RETURN_CHECK( "PhotonSelector::execute()", m_store->record( selectedPhotons, m_outContainerName ), "Failed to store const data container");
      } else {
        // if the event does not pass the selection, CDV won't be ever recorded to TStore, so we have to delete it!
        delete selectedPhotons; selectedPhotons = nullptr;
      }
    }

  } else { // get the list of systematics to run over

    // get vector of string giving the syst names of the upstream algo from TStore (rememeber: 1st element is a blank string: nominal case!)
    //
    std::vector< std::string >* systNames(nullptr);
    RETURN_CHECK("PhotonSelector::execute()", HelperFunctions::retrieve(systNames, m_inputAlgoSystNames, 0, m_store, m_verbose) ,"");

    // prepare a vector of the names of CDV containers for usage by downstream algos
    // must be a pointer to be recorded in TStore
    //
    std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;
    if ( m_debug ) { Info("execute()", " input list of syst size: %i ", static_cast<int>(systNames->size()) ); }

    // loop over systematic sets
    //
    bool eventPassThisSyst(false);
    for ( auto systName : *systNames) {

      if ( m_debug ) { Info("execute()", " syst name: %s  input container name: %s ", systName.c_str(), (m_inContainerName+systName).c_str() ); }

      RETURN_CHECK("PhotonSelector::execute()", HelperFunctions::retrieve(inPhotons, m_inContainerName + systName, m_event, m_store, m_verbose), "");

      // create output container (if requested) - one for each systematic
      //
      ConstDataVector<xAOD::PhotonContainer>* selectedPhotons(nullptr);
      if ( m_createSelectedContainer ) { selectedPhotons = new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS); }

      // find the selected photons, and return if event passes object selection
      //
      eventPassThisSyst = executeSelection( inPhotons, mcEvtWeight, countPass, selectedPhotons );

      if ( countPass ) { countPass = false; } // only count objects/events for 1st syst collection in iteration (i.e., nominal)

      if ( eventPassThisSyst ) {
	// save the string of syst set under question if event is passing the selection
	//
	vecOutContainerNames->push_back( systName );
      }

      // if for at least one syst set the event passes selection, this will remain true!
      //
      eventPass = ( eventPass || eventPassThisSyst );

      if ( m_debug ) { Info("execute()", " syst name: %s  output container name: %s ", systName.c_str(), (m_outContainerName+systName).c_str() ); }

      if ( m_createSelectedContainer ) {
        if ( eventPassThisSyst ) {
          // add ConstDataVector to TStore
	  //
          RETURN_CHECK( "PhotonSelector::execute()", m_store->record( selectedPhotons, m_outContainerName+systName ), "Failed to store const data container");
        } else {
          // if the event does not pass the selection for this syst, CDV won't be ever recorded to TStore, so we have to delete it!
	  //
          delete selectedPhotons; selectedPhotons = nullptr;
        }
      }
    }

    if ( m_debug ) {  Info("execute()", " output list of syst size: %i ", static_cast<int>(vecOutContainerNames->size()) ); }

    // record in TStore the list of systematics names that should be considered down stream
    //
    RETURN_CHECK( "PhotonSelector::execute()", m_store->record( vecOutContainerNames, m_outputAlgoSystNames), "Failed to record vector of output container names.");
  }

  // look what we have in TStore
  //
  if ( m_verbose ) { m_store->print(); }

  if( !eventPass ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  return EL::StatusCode::SUCCESS;
}

bool PhotonSelector :: executeSelection ( const xAOD::PhotonContainer* inPhotons,
					  float mcEvtWeight, bool countPass,
					  ConstDataVector<xAOD::PhotonContainer>* selectedPhotons )
{
  int nPass(0); int nObj(0);
  static SG::AuxElement::Decorator< char > passSelDecor( "passSel" );

  for ( auto ph_itr : *inPhotons ) { // duplicated of basic loop

    // if only looking at a subset of photons make sure all are decorated
    //
    if ( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if ( m_decorateSelectedObjects ) {
        passSelDecor( *ph_itr ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;

    bool passSel = this->passCuts( ph_itr );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *ph_itr ) = passSel;
    }

    if ( passSel ) {
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedPhotons->push_back( ph_itr );
      }
    }
  }

  // for cutflow: make sure to count passed objects only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  //
  if ( m_pass_min > 0 && nPass < m_pass_min ) {
    return false;
  }
  if ( m_pass_max > 0 && nPass > m_pass_max ) {
    return false;
  }

  // for cutflow: make sure to count passed events only once (i.e., this flag will be true only for nominal)
  //
  if ( countPass ){
    m_numEventPass++;
    m_weightNumEventPass += mcEvtWeight;
  }

  // Perform trigger matching on the "good" (selected) photons
  //
  // NB: this part will be skipped if:
  //
  //  1. the user didn't pass any trigger chains to the algo (see initialize(): in that case, the tool is not even initialised!)
  //  2. there are no selected photons in the event
  //
//  if ( m_match_Tool && selectedPhotons ) {
//
//    unsigned int nSelectedPhotons = selectedPhotons->size();
//
//    if ( nSelectedPhotons > 0 ) {
//
//      if ( m_debug ) { Info("executeSelection()", "Now doing photon trigger matching..."); }
//
//      for ( auto const &chain : m_PhTrigChainsList ) {
//
//	if ( m_debug ) { Info("executeSelection()", "\t checking trigger chain %s", chain.c_str()); }
//
//	for ( auto const photon : *selectedPhotons ) {
//
//	  SG::AuxElement::Decorator< std::map<std::string,bool> > isTrigMatchedMapPhDecor( "isTrigMatchedMapPh" );
//	  if ( !isTrigMatchedMapPhDecor.isAvailable( *photon ) ) {
//	    isTrigMatchedMapPhDecor( *photon ) = std::map<std::string,bool>();
//	  }
//
//	  bool matched = false;
//
//	  static const bool DO_TEMPORAL_MATCHING = true;
//
//	  if (!DO_TEMPORAL_MATCHING)  { // the current matching tool does not work for the photon
//	    matched = ( m_match_Tool->matchHLT( photon, chain ) );
//
//	    if ( m_debug ) { Info("executeSelection()", "\t\t is photon trigger matched? %s for %s", (matched ? "Y" : "N"), chain.c_str()); }
//	  } else {
//
//	    if(m_event->contains<xAOD::PhotonContainer>("HLT_xAOD__PhotonContainer_egamma_Photons")){ // check if trigger photon info exist
//	      // get trigger list from config file
//	      Trig::FeatureContainer fc = m_trigDecTool->features(chain);
//	      const auto vec = fc.containerFeature<xAOD::PhotonContainer>("egamma_Photons");
//	      if ( m_debug ) { Info("executeSelection()", "%d trigger objects are found and to be used for the matching", (int) vec.size() ); }
//	      double deltaR = 0.;
//	      for(auto feat : vec){
//		const xAOD::PhotonContainer *cont = feat.cptr();
//		for(const auto& phTrig : *cont){
//		  double eta1 = photon->eta();
//		  double phi1 = photon->phi();
//		  double eta2 = phTrig->eta();
//		  double phi2 = phTrig->phi();
//		  double deta = fabs(eta1 - eta2);
//		  double dphi = fabs(phi1 - phi2) < TMath::Pi() ? fabs(phi1 - phi2) : 2*TMath::Pi() - fabs(phi1 - phi2);
//		  deltaR = sqrt(deta*deta + dphi*dphi);
//		  if(deltaR < 0.07) matched = true;
//		}
//		if ( m_debug ) { Info("executeSelection()", "\t\t is photon trigger matched? %s for %s", (matched ? "Y" : "N"), chain.c_str()); }
//	      } // loop over trigger list
//	    } else {
//	      if ( m_debug ) { Info("executeSelection()", "HLT_xAOD__PhotonContainer_egamma_Photons is not available in this datafile"); }
//	      matched = false;
//	    }
//	  }
//
//	  ( isTrigMatchedMapPhDecor( *photon ) )[chain] = matched;
//	}
//      }
//    }
//  }

  return true;
}

bool PhotonSelector :: passCuts( const xAOD::Photon* photon )
{
  float et    = photon->pt();

  // all the eta cuts are done using the measurement of the cluster position with the 2nd layer cluster,
  // as for Egamma CP  recommendation
  //
  float eta   = ( photon->caloCluster() ) ? photon->caloCluster()->etaBE(2) : -999.0;

  float reta = photon->showerShapeValue(xAOD::EgammaParameters::Reta);
  float rphi = photon->showerShapeValue(xAOD::EgammaParameters::Rphi);

  uint32_t oq= photon->auxdata<uint32_t>("OQ");

  // photon ID key name set
  std::string photonIDKeyName = "PhotonID_"+m_photonIdCut;
  if (  (!(photon->isAvailable< bool >( photonIDKeyName ) )) and (m_photonIdCut != "None") ) {
    Error("passCuts()", Form("Please call PhotonCalibrator before calling PhotonSelector, or check the quality requirement (should be either of Tight/Medium/Loose) [%s %s]",m_name.c_str(), photonIDKeyName.c_str()) );
  }

  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_all, 1 );

  // *********************************************************************************************************************************************************************
  //
  // author cut
  //
  if( m_doAuthorCut ) {
    if( ! ( (photon->author() & xAOD::EgammaParameters::AuthorPhoton) || (photon->author() & xAOD::EgammaParameters::AuthorAmbiguous) ) ) {
      if ( m_debug ) { Info("PassCuts()", "Photon failed author kinematic cut." ); }
      return false;
    }
  }
  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_author_cut, 1 );

  // *********************************************************************************************************************************************************************
  //
  // Object Quality cut
  //
  if ( m_doOQCut ) {
    if ( (oq & 134217728) != 0 && (reta > 0.98 || rphi > 1.0 || (oq & 67108864) != 0) ) {
      if ( m_debug ) { Info("PassCuts()", "Electron failed Object Quality cut." ); }
      return 0;
    }
  }
  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_OQ_cut, 1 );

  // *********************************************************************************************************************************************************************
  //
  // ID cut
  //
  if ( m_photonIdCut != "None" ) {
    // it crashes in case the "PhotonID_X" is not stored on purpose
    if ( ! photon->auxdecor< bool >( photonIDKeyName ) ) {
      if ( m_debug ) { Info("PassCuts()", "Photon failed ID cut." ); }
      return false;
    }
  }
  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_PID_cut, 1 );

  // *********************************************************************************************************************************************************************
  //
  // pT max cut
  //
  if ( m_pT_max != 1e8 ) {
    if ( et > m_pT_max ) {
      if ( m_debug ) { Info("PassCuts()", "Photon failed pT max cut." ); }
      return false;
    }
  }
  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_ptmax_cut, 1 );

  // *********************************************************************************************************************************************************************
  //
  // pT min cut
  //
  if ( m_pT_min != 1e8 ) {
    if ( et < m_pT_min ) {
      if ( m_debug ) { Info("PassCuts()", "Photon failed pT min cut." ); }
      return false;
    }
  }
  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_ptmin_cut, 1 );

  // *********************************************************************************************************************************************************************
  //
  // eta cuts
  //

  // |eta| max cut
  //
  if ( m_eta_max != 1e8 ) {
    if ( fabs(eta) > m_eta_max ) {
      if ( m_debug ) { Info("PassCuts()", "Photon failed |eta| max cut." ); }
      return false;
    }
  }
  // |eta| crack veto
  //
  if ( m_vetoCrack ) {
    if ( fabs( eta ) > 1.37 && fabs( eta ) < 1.52 ) {
      if ( m_debug ) { Info("PassCuts()", "Photon failed |eta| crack veto cut." ); }
      return false;
    }
  }
  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_eta_cut, 1 );

  // *********************************************************************************************************************************************************************
  //
  // isolation cut
  //

  // Get the "list" of input WPs with the accept() decision from the tool
  //
  Root::TAccept accept_list = m_IsolationSelectionTool->accept( *photon );


  // Decorate w/ decision for all input WPs
  //
  const std::string base_decor("isIsolated");
  for ( auto WP_itr : m_IsoKeys ) {

    std::string decorWP = base_decor + "_" + WP_itr;

    if ( m_debug ) { Info("PassCuts()", "Decorate photon with %s - accept() ? %i", decorWP.c_str(), accept_list.getCutResult( WP_itr.c_str()) ); }
    photon->auxdecor<char>(decorWP) = static_cast<char>( accept_list.getCutResult( WP_itr.c_str() ) );

  }

  // Apply the cut if needed
  //
  if ( !m_MinIsoWPCut.empty() && !accept_list.getCutResult( m_MinIsoWPCut.c_str() ) ) {
    if ( m_debug ) { Info("PassCuts()", "Photon failed isolation cut %s ",  m_MinIsoWPCut.c_str() ); }
    return false;
  }
  if(m_useCutFlow) m_ph_cutflowHist_1->Fill( m_ph_cutflow_iso_cut, 1 );

  return true;
}


EL::StatusCode PhotonSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if ( m_debug ) { Info("postExecute()", "Calling postExecute"); }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonSelector :: finalize ()
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

  Info("finalize()", "Deleting tool instances...");

  if ( m_useCutFlow ) {
    Info("finalize()", "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

  if (m_debug) { Info("finalize()", "Cutflow filled"); }

  if (m_IsolationSelectionTool) {
    delete m_IsolationSelectionTool;
    m_IsolationSelectionTool = nullptr;
  }

  if (m_debug) { Info("finalize()", "Isolation Tool deleted"); }

//  if (m_match_Tool) {
//    delete m_match_Tool;
//    m_match_Tool = nullptr;
//  }

  if (m_debug) { Info("finalize()", "Matching Tool deleted"); }

  Info("finalize()", "Finalization done.");
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode PhotonSelector :: histFinalize ()
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

  Info("histFinalize()", "Calling histFinalize");
  RETURN_CHECK("xAH::Algorithm::algFinalize()", xAH::Algorithm::algFinalize(), "");

  return EL::StatusCode::SUCCESS;
}


