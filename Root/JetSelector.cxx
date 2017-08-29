// c++ include(s):
#include <iostream>
#include <typeinfo>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODJet/JetContainer.h"
#include <xAODJet/JetAuxContainer.h>
#include <xAODCore/AuxContainerBase.h>
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicCode.h"

// package include(s):
#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/JetSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

// external tools include(s):
#include "JetJvtEfficiency/JetJvtEfficiency.h"
#include "JetMomentTools/JetForwardJvtTool.h"
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"

// ROOT include(s):
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetSelector)


JetSelector :: JetSelector () :
    Algorithm("JetSelector")
{
}

EL::StatusCode JetSelector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  ANA_MSG_DEBUG( "Calling setupJob");

  job.useXAOD ();
  xAOD::Init( "JetSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  ANA_MSG_DEBUG( "Calling histInitialize");
  ANA_CHECK( xAH::Algorithm::algInitialize());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  ANA_MSG_DEBUG( "Calling fileExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  ANA_MSG_DEBUG( "Calling changeInput");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  ANA_MSG_DEBUG( "Calling initialize");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );
  m_isMC = ( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );

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
    m_jet_cutflowHist_1 = (TH1D*)file->Get("cutflow_jets_1");

    m_jet_cutflow_all             = m_jet_cutflowHist_1->GetXaxis()->FindBin("all");
    m_jet_cutflow_ptmax_cut       = m_jet_cutflowHist_1->GetXaxis()->FindBin("ptmax_cut");
    m_jet_cutflow_ptmin_cut       = m_jet_cutflowHist_1->GetXaxis()->FindBin("ptmin_cut");
    m_jet_cutflow_eta_cut         = m_jet_cutflowHist_1->GetXaxis()->FindBin("eta_cut");
    m_jet_cutflow_jvt_cut         = m_jet_cutflowHist_1->GetXaxis()->FindBin("JVT_cut");
    m_jet_cutflow_btag_cut        = m_jet_cutflowHist_1->GetXaxis()->FindBin("BTag_cut");
    m_jet_cutflow_cleaning_cut    = m_jet_cutflowHist_1->GetXaxis()->FindBin("cleaning_cut");

  }

  //If not set, find default from input container name
  if (m_jetScaleType.size() == 0){
    if( m_inContainerName.find("EMTopo") != std::string::npos){
      m_jetScaleType = "JetEMScaleMomentum";
    }else{
      m_jetScaleType = "JetConstitScaleMomentum";
    }
  }


  if ( m_outputAlgo.empty() ) {
    m_outputAlgo = m_inputAlgo + "_JetSelect";
  }

  m_isEMjet = m_inContainerName.find("EMTopoJets") != std::string::npos;
  m_isLCjet = m_inContainerName.find("LCTopoJets") != std::string::npos;

  // parse and split by comma
  std::string token;

  std::istringstream ss(m_passAuxDecorKeys);
  while ( std::getline(ss, token, ',') ) {
    m_passKeys.push_back(token);
  }

  ss.clear();
  ss.str(m_failAuxDecorKeys);
  while ( std::getline(ss, token, ',') ) {
    ANA_MSG_INFO(token);
    m_failKeys.push_back(token);
  }

  if ( m_inContainerName.empty() ) {
    ANA_MSG_ERROR( "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  bool allOK(true);
  if (!m_operatingPt.empty() || m_doBTagCut ) { allOK = false; }
  if (m_operatingPt == "FixedCutBEff_30") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_50") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_60") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_70") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_77") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_80") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_85") { allOK = true; }
  if (m_operatingPt == "FixedCutBEff_90") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_30") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_40") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_50") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_60") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_70") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_77") { allOK = true; }
  if (m_operatingPt == "FlatCutBEff_85") { allOK = true; }

  if( !allOK ) {
    ANA_MSG_ERROR( "Requested operating point is not known to xAH. Arrow v Indian? " << m_operatingPt);
    return EL::StatusCode::FAILURE;
  }

  if ( m_decorateSelectedObjects ) {
    ANA_MSG_INFO(" Decorate Jets with " << m_decor);
  }

  // if applying cut on nr. bjets, configure it
  //
  if ( m_doBTagCut ) {

    // initialize the BJetSelectionTool
    setToolName(m_BJetSelectTool_handle);
    // A few which are not configurable as of yet....
    // is there a reason to have this configurable here??...I think no (GF to self)
    ANA_CHECK( m_BJetSelectTool_handle.setProperty("MaxEta",m_b_eta_max));
    ANA_CHECK( m_BJetSelectTool_handle.setProperty("MinPt",m_b_pt_min));
    ANA_CHECK( m_BJetSelectTool_handle.setProperty("FlvTagCutDefinitionsFileName", m_corrFileName));
    // configurable parameters
    ANA_CHECK( m_BJetSelectTool_handle.setProperty("TaggerName",	      m_taggerName));
    ANA_CHECK( m_BJetSelectTool_handle.setProperty("OperatingPoint",      m_operatingPt));
    ANA_CHECK( m_BJetSelectTool_handle.setProperty("JetAuthor",	      m_jetAuthor));
    ANA_CHECK( m_BJetSelectTool_handle.setProperty("OutputLevel",  msg().level()));
    ANA_CHECK( m_BJetSelectTool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_BJetSelectTool_handle);

  }

  //init fJVT
  if (m_dofJVT) {
    // initialize the CP::JetJvtEfficiency Tool for fJVT
    setToolName(m_fJVT_eff_tool_handle, "fJVT_eff_tool");
    ANA_CHECK( ASG_MAKE_ANA_TOOL(m_fJVT_eff_tool_handle, CP::JetJvtEfficiency));
    ANA_CHECK( m_fJVT_eff_tool_handle.setProperty("WorkingPoint", m_WorkingPointfJVT ));
    ANA_CHECK( m_fJVT_eff_tool_handle.setProperty("SFFile",       m_SFFilefJVT ));
    ANA_CHECK( m_fJVT_eff_tool_handle.setProperty("ScaleFactorDecorationName", "fJVTSF"));
    ANA_CHECK( m_fJVT_eff_tool_handle.setProperty("OutputLevel",  msg().level()));
    ANA_CHECK( m_fJVT_eff_tool_handle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_fJVT_eff_tool_handle);

    //  Add the chosen WP to the string labelling the vector<SF> decoration
    m_outputSystNamesfJVT = m_outputSystNamesfJVT + "_fJVT_" + m_WorkingPointfJVT;
    //  Create a passed label for JVT cut
    m_outputfJVTPassed = m_outputfJVTPassed + "_" + m_WorkingPointfJVT;

    CP::SystematicSet affectSystsfJVT = m_fJVT_eff_tool_handle->affectingSystematics();
    for ( const auto& syst_it : affectSystsfJVT ) { ANA_MSG_DEBUG("JetJvtEfficiency tool can be affected by fJVT efficiency systematic: " << syst_it.name()); }
    //
    // Make a list of systematics to be used, based on configuration input
    // Use HelperFunctions::getListofSystematics() for this!
    //
    const CP::SystematicSet recSystsfJVT = m_fJVT_eff_tool_handle->recommendedSystematics();
    m_systListfJVT = HelperFunctions::getListofSystematics( recSystsfJVT, m_systNamefJVT, m_systValfJVT, msg() );

    ANA_MSG_INFO("Will be using JetJvtEfficiency tool fJVT efficiency systematic:");
    for ( const auto& syst_it : m_systListfJVT ) {
      if ( m_systNamefJVT.empty() ) {
        ANA_MSG_INFO("\t Running w/ nominal configuration only!");
        break;
      }
      ANA_MSG_INFO("\t " << syst_it.name());
    }
  }

  // initialize the CP::JetJvtEfficiency Tool for JVT
  setToolName(m_JVT_tool_handle, "JVT_eff_tool");
  ANA_CHECK( ASG_MAKE_ANA_TOOL(m_JVT_tool_handle, CP::JetJvtEfficiency));
  ANA_CHECK( m_JVT_tool_handle.setProperty("WorkingPoint", m_WorkingPointJVT ));
  ANA_CHECK( m_JVT_tool_handle.setProperty("SFFile",       m_SFFileJVT ));
  ANA_CHECK( m_JVT_tool_handle.setProperty("OutputLevel",  msg().level()));
  ANA_CHECK( m_JVT_tool_handle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_JVT_tool_handle);

  //  Add the chosen WP to the string labelling the vector<SF> decoration
  m_outputSystNamesJVT = m_outputSystNamesJVT + "_JVT_" + m_WorkingPointJVT;
  //  Create a passed label for JVT cut
  m_outputJVTPassed = m_outputJVTPassed + "_" + m_WorkingPointJVT;

  CP::SystematicSet affectSystsJVT = m_JVT_tool_handle->affectingSystematics();
  for ( const auto& syst_it : affectSystsJVT ) { ANA_MSG_DEBUG("JetJvtEfficiency tool can be affected by JVT efficiency systematic: " << syst_it.name()); }
  //
  // Make a list of systematics to be used, based on configuration input
  // Use HelperFunctions::getListofSystematics() for this!
  //
  const CP::SystematicSet recSystsJVT = m_JVT_tool_handle->recommendedSystematics();
  m_systListJVT = HelperFunctions::getListofSystematics( recSystsJVT, m_systNameJVT, m_systValJVT, msg() );

  ANA_MSG_INFO("Will be using JetJvtEfficiency tool JVT efficiency systematic:");
  for ( const auto& syst_it : m_systListJVT ) {
    if ( m_systNameJVT.empty() ) {
      ANA_MSG_INFO("\t Running w/ nominal configuration only!");
      break;
    }
    ANA_MSG_INFO("\t " << syst_it.name());
  }

  // Write output sys names
  if ( m_writeSystToMetadata ) {
    TFile *fileMD = wk()->getOutputFile ("metadata");
    HelperFunctions::writeSystematicsListHist(m_systListJVT, m_outputSystNamesJVT, fileMD);
    HelperFunctions::writeSystematicsListHist(m_systListfJVT, m_outputSystNamesfJVT, fileMD);
  }

  ANA_MSG_DEBUG( "Number of events in file: " << m_event->getEntries() );

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  ANA_MSG_DEBUG( "JetSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  ANA_MSG_DEBUG( "Applying Jet Selection... " << m_name);

  // retrieve event
  const xAOD::EventInfo* eventInfo(nullptr);
  ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

  // MC event weight
  float mcEvtWeight(1.0);
  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
    static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");
    if ( ! mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
      ANA_MSG_ERROR( "mcEventWeight is not available as decoration! Aborting" );
      return EL::StatusCode::FAILURE;
    }
    mcEvtWeight = mcEvtWeightAcc( *eventInfo );
  }

  m_numEvent++;

  // did any collection pass the cuts?
  bool pass(false);
  bool count(true); // count for the 1st collection in the container - could be better as
                    // shoudl only count for the nominal
  const xAOD::JetContainer* inJets(nullptr);

  const xAOD::JetContainer *truthJets = nullptr;
  if ( m_isMC && m_doJVT ) ANA_CHECK( HelperFunctions::retrieve(truthJets, m_truthJetContainer, m_event, m_store, msg()) );

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  if ( m_inputAlgo.empty() ) {

    // this will be the collection processed - no matter what!!
    ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, msg()) );

    // decorate inJets with truth info
    if ( m_isMC && m_doJVT ) {
      static SG::AuxElement::Decorator<char>  isHS("isJvtHS");
      static SG::AuxElement::Decorator<char>  isPU("isJvtPU");
      for(const auto& jet : *inJets) {
        bool ishs = false;
        bool ispu = true;
        for(const auto& tjet : *truthJets) {
          if (tjet->p4().DeltaR(jet->p4())<0.3 && tjet->pt()>10e3) ishs = true;
          if (tjet->p4().DeltaR(jet->p4())<0.6) ispu = false;
        }
        isHS(*jet)=ishs;
        isPU(*jet)=ispu;
      }
    }

    pass = executeSelection( inJets, mcEvtWeight, count, m_outContainerName, true );

  }  else { // get the list of systematics to run over

    // get vector of string giving the names
    std::vector<std::string>* systNames(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, msg()) );

    // loop over systematics
    std::vector< std::string >* vecOutContainerNames = new std::vector< std::string >;
    bool passOne(false);
    for ( auto systName : *systNames ) {

      ANA_CHECK( HelperFunctions::retrieve(inJets, m_inContainerName+systName, m_event, m_store, msg()) );

      // decorate inJets with truth info
      if ( m_isMC && m_doJVT ) {
        static SG::AuxElement::Decorator<char>  isHS("isJvtHS");
        static SG::AuxElement::Decorator<char>  isPU("isJvtPU");
        for(const auto& jet : *inJets) {
          bool ishs = false;
          bool ispu = true;
          for(const auto& tjet : *truthJets) {
            if (tjet->p4().DeltaR(jet->p4())<0.3 && tjet->pt()>10e3) ishs = true;
            if (tjet->p4().DeltaR(jet->p4())<0.6) ispu = false;
          }
          isHS(*jet)=ishs;
          isPU(*jet)=ispu;
        }
      }

      passOne = executeSelection( inJets, mcEvtWeight, count, m_outContainerName+systName, systName.empty() );
      if ( count ) { count = false; } // only count for 1 collection
      // save the string if passing the selection
      if ( passOne ) {
        vecOutContainerNames->push_back( systName );
      }
      // the final decision - if at least one passes keep going!
      pass = pass || passOne;
    }

    // save list of systs that should be considered down stream
    ANA_CHECK( m_store->record( vecOutContainerNames, m_outputAlgo));
    //delete vecOutContainerNames;

  }

  // look what we have in TStore
  if(msgLvl(MSG::VERBOSE)) m_store->print();

  if ( !pass ) {
    wk()->skipEvent();
  }

  ANA_MSG_DEBUG( "Leave Jet Selection... ");

  return EL::StatusCode::SUCCESS;

}

bool JetSelector :: executeSelection ( const xAOD::JetContainer* inJets,
    float mcEvtWeight,
    bool count,
    std::string outContainerName,
    bool isNominal
    )
{
  ANA_MSG_DEBUG("in executeSelection... " << m_name);

  // create output container (if requested)
  ConstDataVector<xAOD::JetContainer>* selectedJets(nullptr);
  if ( m_createSelectedContainer ) {
    selectedJets = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  }

  // if doing JVF or JVT get PV location
  if ( m_doJVF ) {
    const xAOD::VertexContainer* vertices(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, msg()) );
    m_pvLocation = HelperFunctions::getPrimaryVertexLocation( vertices, msg() );
  }

  int nPass(0); int nObj(0);
  bool passEventClean(true);

  static SG::AuxElement::Accessor< char > isCleanAcc("cleanJet");

  //
  // This cannot be static as multiple instance of Jet Selector would
  //   then share the same passSelDecor, including the m_decor name
  //
  SG::AuxElement::Decorator< char > passSelDecor( m_decor );

  for ( auto jet_itr : *inJets ) { // duplicated of basic loop

    // if only looking at a subset of jets make sure all are decorated
    if ( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if ( m_decorateSelectedObjects ) {
        passSelDecor( *jet_itr ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    // All selections but Cleaning
    int passSel = this->PassCuts( jet_itr );
    if ( m_decorateSelectedObjects ) {
      passSelDecor( *jet_itr ) = passSel;
    }

    // Cleaning Selection must come after kinematic and JVT selections
    if ( m_cleanJets && passSel && isCleanAcc.isAvailable( *jet_itr ) ) {
      if( !isCleanAcc( *jet_itr ) ) {
        passSel = false;
        if ( m_decorateSelectedObjects )
          passSelDecor( *jet_itr ) = passSel;

        // If any of the passing jets fail the recommendation is to remove the jet (and MET is wrong)
        // If any of the N leading jets are not clean the event should be removed
        if( m_markCleanEvent || m_cleanEvent || nObj <= m_cleanEvtLeadJets ){
          passEventClean = false;
          ANA_MSG_DEBUG("Remove event due to bad jet with pt " << jet_itr->pt() );
        }// if cleaning the event

      }// if jet is not clean
    }// if jet clean aux missing
    if( m_useCutFlow && passSel )
      m_jet_cutflowHist_1->Fill( m_jet_cutflow_cleaning_cut, 1 );



    if ( passSel ) {
      ANA_MSG_DEBUG("passSel");
      nPass++;
      if ( m_createSelectedContainer ) {
        selectedJets->push_back( jet_itr );
      }
    }
  }

  // Loop over selected jets and decorate with JVT efficiency SF
  // Do it only for MC
  //
  if ( m_isMC && m_doJVT ) {

    std::vector< std::string >* sysVariationNamesJVT  = new std::vector< std::string >;

    // Do it only if a tool with *this* name hasn't already been used
    //
    if ( m_JVT_tool_handle.isInitialized() ) {

      for ( const auto& syst_it : m_systListJVT ) {

        // we do not need all SF for non-nominal trees
        if ( !syst_it.name().empty() && !isNominal )
            continue;

        // Create the name of the SF weight to be recorded
        //   template:  SYSNAME_JVTEff_SF
        //
        std::string sfName = "JVTEff_SF_" + m_WorkingPointJVT;
        if ( !syst_it.name().empty() ) {
           std::string prepend = syst_it.name() + "_";
           sfName.insert( 0, prepend );
        }
        ANA_MSG_DEBUG("JVT SF sys name (to be recorded in xAOD::TStore) is: " << sfName);
        sysVariationNamesJVT->push_back(sfName);

        // apply syst
        //
        if ( m_JVT_tool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
          ANA_MSG_ERROR( "Failed to configure CP::JetJvtEfficiency for systematic " << syst_it.name());
          return EL::StatusCode::FAILURE;
        }
        ANA_MSG_DEBUG("Successfully applied systematic: " << syst_it.name());

        // and now apply JVT SF!
        //
        unsigned int idx(0);
        for ( auto jet : *(selectedJets) ) {

          ANA_MSG_DEBUG("Applying JVT SF" );

          // create passed JVT decorator
          static const SG::AuxElement::Decorator<char> passedJVT( m_outputJVTPassed );
          if ( syst_it.name().empty() ) {
            passedJVT( *jet ) = 1; // passes by default
          }

          // obtain JVT SF as a float (to be stored away separately)
          //
          //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* jet)
          //
          static const SG::AuxElement::Decorator< std::vector<float> > sfVecJVT( m_outputSystNamesJVT );
          if ( !sfVecJVT.isAvailable( *jet ) ) {
            sfVecJVT( *jet ) = std::vector<float>();
          }

          float jvtSF(1.0);
          if ( m_JVT_tool_handle->isInRange(*jet) ) {
            // If we do not enforce JVT veto and the jet hasn't passed the JVT cut, we need to calculate the inefficiency scale factor for it
            if ( m_noJVTVeto && !m_JVT_tool_handle->passesJvtCut(*jet) ) {
              if ( syst_it.name().empty() ) {
                passedJVT( *jet ) = 0; // mark as not passed
              }
              if ( m_JVT_tool_handle->getInefficiencyScaleFactor( *jet, jvtSF ) != CP::CorrectionCode::Ok ) {
                ANA_MSG_WARNING( "Problem in JVT Tool getInefficiencyScaleFactor");
                jvtSF = 1.0;
              }
            } else { // otherwise classic efficiency scale factor
              if ( syst_it.name().empty() ) {
                passedJVT( *jet ) = 1;
              }
              if ( m_JVT_tool_handle->getEfficiencyScaleFactor( *jet, jvtSF ) != CP::CorrectionCode::Ok ) {
                ANA_MSG_WARNING( "Problem in JVT Tool getEfficiencyScaleFactor");
                jvtSF = 1.0;
              }
            }
          }
          //
          // Add it to decoration vector
          //
          sfVecJVT( *jet ).push_back( jvtSF );

          ANA_MSG_DEBUG( "===>>>");
          ANA_MSG_DEBUG( "Jet " << idx << ", pt = " << jet->pt()*1e-3 << " GeV, |eta| = " << std::fabs(jet->eta()) );
          ANA_MSG_DEBUG( "JVT SF decoration: " << m_outputSystNamesJVT );
          ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
          ANA_MSG_DEBUG( "JVT SF:");
          ANA_MSG_DEBUG( "\t " << jvtSF << " (from getEfficiencyScaleFactor())" );
          ANA_MSG_DEBUG( "--------------------------------------");

          ++idx;
        }
      }
    }

    // Add list of JVT systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSelection() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesJVT) ) { ANA_CHECK( m_store->record( sysVariationNamesJVT, m_outputSystNamesJVT)); }
  } else if ( !m_isMC && m_doJVT ) {
    // Loop over selected jets and decorate with JVT passed status
    for ( auto jet : *(selectedJets) ) {
      // create passed JVT decorator
      static const SG::AuxElement::Decorator<char> passedJVT( m_outputJVTPassed );
      passedJVT( *jet ) = 1; // passes by default

      if ( m_JVT_tool_handle->isInRange(*jet) ) {
        if ( m_noJVTVeto && !m_JVT_tool_handle->passesJvtCut(*jet) ) {
          passedJVT( *jet ) = 0; // mark as not passed
        } else {
          passedJVT( *jet ) = 1;
        }
      }
    }
  }
  
  // Loop over selected jets and decorate with fJVT efficiency SF
  // Do it only for MC
  //
  if ( m_isMC && m_dofJVT ) {

    std::vector< std::string >* sysVariationNamesfJVT  = new std::vector< std::string >;

    // Do it only if a tool with *this* name hasn't already been used
    //
    if ( m_fJVT_eff_tool_handle.isInitialized() ) {

      for ( const auto& syst_it : m_systListfJVT ) {

        // we do not need all SF for non-nominal trees
        if ( !syst_it.name().empty() && !isNominal )
            continue;

        // Create the name of the SF weight to be recorded
        //   template:  SYSNAME_fJVTEff_SF
        //
        std::string sfName = "fJVTEff_SF";
        if ( !syst_it.name().empty() ) {
           std::string prepend = syst_it.name() + "_";
           sfName.insert( 0, prepend );
        }
        ANA_MSG_DEBUG("fJVT SF sys name (to be recorded in xAOD::TStore) is: " << sfName);
        sysVariationNamesfJVT->push_back(sfName);

        // apply syst
        //
        if ( m_fJVT_eff_tool_handle->applySystematicVariation(syst_it) != CP::SystematicCode::Ok ) {
          ANA_MSG_ERROR( "Failed to configure CP::JetJvtEfficiency for systematic " << syst_it.name());
          return EL::StatusCode::FAILURE;
        }
        ANA_MSG_DEBUG("Successfully applied systematic: " << syst_it.name());

        // and now apply fJVT SF!
        //
        unsigned int idx(0);
        for ( auto jet : *(selectedJets) ) {

          ANA_MSG_DEBUG("Applying fJVT SF" );
          
          static const SG::AuxElement::Decorator<char> passedfJVT( m_outputfJVTPassed );
          if ( syst_it.name().empty() ) {
            passedfJVT( *jet ) = jet->auxdata<char>("passFJVT");
          }

          // obtain fJVT SF as a float (to be stored away separately)
          //
          //  If SF decoration vector doesn't exist, create it (will be done only for the 1st systematic for *this* jet)
          //
          static const SG::AuxElement::Decorator< std::vector<float> > sfVecfJVT( m_outputSystNamesfJVT );
          if ( !sfVecfJVT.isAvailable( *jet ) ) {
            sfVecfJVT( *jet ) = std::vector<float>();
          }

          float fjvtSF(1.0);
          if ( m_fJVT_eff_tool_handle->isInRange(*jet) ) {
            // If we do not enforce JVT veto and the jet hasn't passed the JVT cut, we need to calculate the inefficiency scale factor for it
            if ( !m_dofJVTVeto && jet->auxdata<char>("passFJVT") != 1 ) {
              if ( m_fJVT_eff_tool_handle->getInefficiencyScaleFactor( *jet, fjvtSF ) != CP::CorrectionCode::Ok ) {
                ANA_MSG_WARNING( "Problem in fJVT Tool getInefficiencyScaleFactor");
                fjvtSF = 1.0;
              }
            } else { // otherwise classic efficiency scale factor
              if ( m_fJVT_eff_tool_handle->getEfficiencyScaleFactor( *jet, fjvtSF ) != CP::CorrectionCode::Ok ) {
                ANA_MSG_WARNING( "Problem in fJVT Tool getEfficiencyScaleFactor");
                fjvtSF = 1.0;
              }
            }
          }
          //
          // Add it to decoration vector
          //
          sfVecfJVT( *jet ).push_back( fjvtSF );

          ANA_MSG_DEBUG( "===>>>");
          ANA_MSG_DEBUG( "Jet " << idx << ", pt = " << jet->pt()*1e-3 << " GeV, |eta| = " << std::fabs(jet->eta()) );
          ANA_MSG_DEBUG( "fJVT SF decoration: " << m_outputSystNamesfJVT );
          ANA_MSG_DEBUG( "Systematic: " << syst_it.name() );
          ANA_MSG_DEBUG( "fJVT SF:");
          ANA_MSG_DEBUG( "\t " << fjvtSF << " (from getEfficiencyScaleFactor())" );
          ANA_MSG_DEBUG( "--------------------------------------");

          ++idx;
        }
      }
    }

    // Add list of fJVT systematics names to TStore
    //
    // NB: we need to make sure that this is not pushed more than once in TStore!
    // This will be the case when this executeSelection() function gets called for every syst varied input container,
    // e.g. the different SC containers w/ calibration systematics upstream.
    //
    if ( !m_store->contains<std::vector<std::string> >(m_outputSystNamesfJVT) ) { ANA_CHECK( m_store->record( sysVariationNamesfJVT, m_outputSystNamesfJVT)); }
  } else if ( !m_isMC && m_dofJVT ) {
    // Loop over selected jets and decorate with fJVT passed status
    for ( auto jet : *(selectedJets) ) {
      // create passed fJVT decorator
      static const SG::AuxElement::Decorator<char> passedfJVT( m_outputfJVTPassed );
      passedfJVT( *jet ) = jet->auxdata<char>("passFJVT");
    }
  }

  // add ConstDataVector to TStore
  if ( m_createSelectedContainer ) {
    ANA_CHECK( m_store->record( selectedJets, outContainerName ));
  }

  if ( count ) {
    m_numObject     += nObj;
    m_numObjectPass += nPass;
  }

  //
  //  Mark Event as pass/fail cleaning for nominal selection
  //
  if ( m_markCleanEvent && count ) {
    ANA_MSG_DEBUG("Marking Clean");
    // Decorator
    SG::AuxElement::Decorator< char > isCleanEventDecor( "cleanEvent_"+m_name );
    const xAOD::EventInfo* eventInfo(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

    isCleanEventDecor(*eventInfo) = passEventClean;
  }


  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  if ( m_cleanEvent && !passEventClean ) { return false; }
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

  ANA_MSG_DEBUG("leave executeSelection... ");
  return true;
}


EL::StatusCode JetSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  ANA_MSG_DEBUG("Calling postExecute");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: finalize ()
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

  ANA_MSG_DEBUG( m_name );

  if ( m_useCutFlow ) {
    ANA_MSG_DEBUG( "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: histFinalize ()
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

  ANA_MSG_DEBUG( "Calling histFinalize");
  ANA_CHECK( xAH::Algorithm::algFinalize());
  return EL::StatusCode::SUCCESS;
}

int JetSelector :: PassCuts( const xAOD::Jet* jet ) {
  ANA_MSG_DEBUG("In pass cuts");

  // fill cutflow bin 'all' before any cut
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_all, 1 );

  // pT
  if ( m_pT_max != 1e8 ) {
    if ( jet->pt() > m_pT_max ) { return 0; }
  }
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_ptmax_cut, 1 );

  if ( m_pT_min != 1e8 ) {
    if ( jet->pt() < m_pT_min ) { return 0; }
  }
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_ptmin_cut, 1 );

  // eta
  if ( m_eta_max != 1e8 ) {
    if ( fabs(jet->eta()) > m_eta_max ) { return 0; }
  }
  if ( m_eta_min != 1e8 ) {
    if ( fabs(jet->eta()) < m_eta_min ) { return 0; }
  }
  if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_eta_cut, 1 );

  // detEta
  if ( m_detEta_max != 1e8 ) {
    if ( fabs( ( jet->getAttribute<xAOD::JetFourMom_t>(m_jetScaleType.c_str()) ).eta() ) > m_detEta_max ) { return 0; }
  }
  if ( m_detEta_min != 1e8 ) {
    if ( fabs( ( jet->getAttribute<xAOD::JetFourMom_t>(m_jetScaleType.c_str()) ).eta() ) < m_detEta_min ) { return 0; }
  }

  // mass
  if ( m_mass_max != 1e8 ) {
    if ( jet->m() > m_mass_max ) { return 0; }
  }
  if ( m_mass_min != 1e8 ) {
    if ( jet->m() < m_mass_min ) { return 0; }
  }

  // rapidity
  if ( m_rapidity_max != 1e8 ) {
    if ( jet->rapidity() > m_rapidity_max ) { return 0; }
  }
  if ( m_rapidity_min != 1e8 ) {
    if ( jet->rapidity() < m_rapidity_min ) { return 0; }
  }

  // JVF pileup cut
  if ( m_doJVF ){
    ANA_MSG_DEBUG("Doing JVF");
    ANA_MSG_DEBUG("Jet Pt " << jet->pt());
    if ( jet->pt() < m_pt_max_JVF ) {
      xAOD::JetFourMom_t jetScaleP4 = jet->getAttribute< xAOD::JetFourMom_t >( m_jetScaleType.c_str() );
      if ( fabs(jetScaleP4.eta()) < m_eta_max_JVF ){
        if ( jet->getAttribute< std::vector<float> >( "JVF" ).at( m_pvLocation ) < m_JVFCut ) {
          return 0;
        }
      }
    }
  } // m_doJVF

  if(m_dofJVT){
    if(jet->auxdata<char>("passFJVT")!=1){
      ANA_MSG_DEBUG("jet pt = "<<jet->pt()<<",eta = "<<jet->eta()<<",phi = "<<jet->phi());
      ANA_MSG_DEBUG("Failed forward JVT");
      if(m_dofJVTVeto)return 0;
    }
    else if (TMath::Abs(jet->eta()>2.5) ) {
      ANA_MSG_DEBUG("jet pt = " << jet->pt() << ",eta = "<<jet->eta()<<",phi = "<<jet->phi());
      ANA_MSG_DEBUG("Passed forward JVT");
    }

  }//do forward JVT

  // JVT pileup cut
  //
  if ( m_doJVT ) {
    ANA_MSG_DEBUG("Checking JVT cut...");
    if ( m_JVTCut > 0 && jet->getAttribute< float >( "Jvt" ) < m_JVTCut ) { ANA_MSG_DEBUG( " pt/eta = "<<jet->pt()<<"/" << jet->eta() ); }

    // Old usage: check manually whether this jet passes JVT cut
    //
    if ( m_JVTCut > 0 ) {
      if ( jet->pt() < m_pt_max_JVT ) {
        ANA_MSG_DEBUG("Pass JVT-pT Cut");
        xAOD::JetFourMom_t jetScaleP4 = jet->getAttribute< xAOD::JetFourMom_t >( m_jetScaleType.c_str() );
        if ( fabs(jetScaleP4.eta()) < m_eta_max_JVT ){
          ANA_MSG_DEBUG(" Pass JVT-Eta Cut " );
          ANA_MSG_DEBUG(" JVT = " << jet->getAttribute< float >( "Jvt" ) );
          if ( jet->getAttribute< float >( "Jvt" ) < m_JVTCut ) {
            ANA_MSG_DEBUG(" upper JVTCut is " << m_JVTCut << " - cutting this jet!!" );
            return 0;
          } else {
            ANA_MSG_DEBUG(" upper JVTCut is " << m_JVTCut << " - jet passes JVT ");
          }
        }
      }
    } else {
      if ( !m_noJVTVeto && !m_JVT_tool_handle->passesJvtCut(*jet) ) { return 0; }
    }
  } // m_doJVT
  if ( m_useCutFlow ) m_jet_cutflowHist_1->Fill( m_jet_cutflow_jvt_cut, 1 );

  //
  //  BTagging
  //
  if ( m_doBTagCut ) {
    ANA_MSG_DEBUG("Doing BTagging");
    if ( m_BJetSelectTool_handle->accept( jet ) ) {
      if(m_useCutFlow) m_jet_cutflowHist_1->Fill( m_jet_cutflow_btag_cut, 1 );
    } else {
      return 0;
    }
  }


  //
  //  HLT BTagging
  //
  if ( m_doHLTBTagCut ) {
    const xAOD::BTagging *btag_info = jet->auxdata< const xAOD::BTagging* >("HLTBTag");

    double tagValue = -99;
    if(m_HLTBTagTaggerName=="MV2c20"){
      btag_info->MVx_discriminant("MV2c20", tagValue);
    }

    if(m_HLTBTagTaggerName=="COMB"){
      float wIP3D = btag_info->IP3D_loglikelihoodratio();
      float wSV1  = btag_info->SV1_loglikelihoodratio();
      tagValue = wIP3D + wSV1;
    }

    if(tagValue < m_HLTBTagCutValue){return 0;}
  }

  //
  // HLT Valid Vtx
  //
  if ( m_requireHLTVtx ) {
    const xAOD::Vertex *online_pvx   = jet->auxdata<const xAOD::Vertex*>("HLTBJetTracks_vtx");
    if(!online_pvx) {return 0;}
  }

  if ( m_requireNoHLTVtx ) {
    const xAOD::Vertex *online_pvx   = jet->auxdata<const xAOD::Vertex*>("HLTBJetTracks_vtx");
    if(online_pvx) {return 0;}
  }


  //
  //  Pass Keys
  //
  for ( auto& passKey : m_passKeys ) {
    if ( !(jet->auxdata< char >(passKey) == '1') ) { return 0;}
  }

  //
  //  Fail Keys
  //
  for ( auto& failKey : m_failKeys ){
    if ( !(jet->auxdata< char >(failKey) == '0') ) { return 0;}
  }

  //
  //  Truth Label
  //
  if ( m_truthLabel != -1 ) {
    ANA_MSG_DEBUG("Doing Truth Label");
    int this_TruthLabel = 0;

    static SG::AuxElement::ConstAccessor<int> HadronConeExclTruthLabelID ("HadronConeExclTruthLabelID");
    static SG::AuxElement::ConstAccessor<int> TruthLabelID ("TruthLabelID");
    static SG::AuxElement::ConstAccessor<int> PartonTruthLabelID ("PartonTruthLabelID");

    if( m_useHadronConeExcl && HadronConeExclTruthLabelID.isAvailable( *jet) ){
      this_TruthLabel = HadronConeExclTruthLabelID(( *jet) );
    } else if ( TruthLabelID.isAvailable( *jet) ) {
      this_TruthLabel = TruthLabelID( *jet );
      if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
    } else {
      this_TruthLabel = PartonTruthLabelID( *jet );
      if (this_TruthLabel == 21 || this_TruthLabel<4) this_TruthLabel = 0;
    }

    if ( this_TruthLabel == -1 ) {return 0;}
    if ( (m_truthLabel == 5) && this_TruthLabel != 5 ) { return 0;}
    if ( (m_truthLabel == 4) && this_TruthLabel != 4 ) { return 0;}
    if ( (m_truthLabel == 0) && this_TruthLabel != 0 ) { return 0;}

  }

  ANA_MSG_DEBUG("Passed Cuts");
  return 1;
}
