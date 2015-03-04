/******************************************
 *
 * BJet selector.
 *
 * G. Facini, M. Milesi (marco.milesi@cern.ch)
 * Jan 28 16:07 AEST 2015
 *
 ******************************************/

// c++ include(s):
#include <iostream>
#include <typeinfo>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODBTagging/BTagging.h"

// package include(s):
#include "xAODAnaHelpers/BJetSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(BJetSelector)

BJetSelector :: BJetSelector () {
}

BJetSelector :: BJetSelector (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_cutflowHist(0),
  m_cutflowHistW(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  Info("BJetSelector()", "Calling constructor \n");
}

EL::StatusCode  BJetSelector :: configure ()
{
  Info("configure()", "Configuing BJetSelector Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "BJetSelector::configure()", m_configName);

  TEnv* config = new TEnv(m_configName.c_str());

  // read debug flag from .config file
  m_debug         = config->GetValue("Debug" ,      false );
  m_useCutFlow    = config->GetValue("UseCutFlow",  true);

  // input container to be read from TEvent or TStore
  m_inContainerName  = config->GetValue("InputContainer",  "");

  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects = config->GetValue("DecorateSelectedObjects", true);
  // additional functionality : create output container of selected objects
  //                            using the SG::VIEW_ELEMENTS option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainer = config->GetValue("CreateSelectedContainer", false);
  // if requested, a new container is made using the SG::VIEW_ELEMENTS option
  m_outContainerName        = config->GetValue("OutputContainer", "");
  // if only want to look at a subset of object
  m_nToProcess              = config->GetValue("NToProcess", -1);
  // sort before running selection

  m_isEMjet = m_inContainerName.find("EMTopoJets") != std::string::npos;
  m_isLCjet = m_inContainerName.find("LCTopoJets") != std::string::npos;

  // cuts
  m_cleanJets               = config->GetValue("CleanJets", true);
  m_pass_max                = config->GetValue("PassMax", -1);
  m_pass_min                = config->GetValue("PassMin", -1);
  m_pT_max                  = config->GetValue("pTMax",       1e8);
  m_pT_min                  = config->GetValue("pTMin",       1e8);
  m_eta_max                 = config->GetValue("etaMax",      1e8);
  m_eta_min                 = config->GetValue("etaMin",      1e8);
  m_mass_max                = config->GetValue("massMax",     1e8);
  m_mass_min                = config->GetValue("massMin",     1e8);
  m_rapidity_max            = config->GetValue("rapidityMax", 1e8);
  // quality
  m_veryloose               = config->GetValue("VeryLoose",   false);
  m_loose                   = config->GetValue("Loose",       false);
  m_medium                  = config->GetValue("Medium",      false);
  m_tight                   = config->GetValue("Tight",       false);

  m_btagCut = -1;
  m_decor   = "passSel";
  if( m_isEMjet ) {
    if( m_veryloose ) { m_btagCut = 0.1340; m_decor += "VeryLoose";  }
    if( m_loose     ) { m_btagCut = 0.3511; m_decor += "Loose";      }
    if( m_medium    ) { m_btagCut = 0.7892; m_decor += "Medium";     }
    if( m_tight     ) { m_btagCut = 0.9827; m_decor += "Tight";      }
  } else if ( m_isLCjet ) {
    if( m_veryloose ) { m_btagCut = 0.1644; m_decor += "VeryLoose";  }
    if( m_loose     ) { m_btagCut = 0.3900; m_decor += "Loose";      }
    if( m_medium    ) { m_btagCut = 0.8119; m_decor += "Medium";     }
    if( m_tight     ) { m_btagCut = 0.9867; m_decor += "Tight";      }
  }
  if(m_decorateSelectedObjects) {
    Info(m_name.c_str()," Decorate Jets with %s", m_decor.c_str());
  }

  if( m_inContainerName.empty() ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "BJetSelector Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode BJetSelector :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Calling setupJob \n");

  job.useXAOD ();
  xAOD::Init( "BJetSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetSelector :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "Calling histInitialize \n");
  if(m_useCutFlow) {
    TFile *file = wk()->getOutputFile ("cutflow");
    m_cutflowHist  = (TH1D*)file->Get("cutflow");
    m_cutflowHistW = (TH1D*)file->Get("cutflow_weighted");
    m_cutflow_bin  = m_cutflowHist->GetXaxis()->FindBin(m_name.c_str());
    m_cutflowHistW->GetXaxis()->FindBin(m_name.c_str());
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  //std::cout << m_name << " Number of events = " << m_event->getEntries() << std::endl;

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_numObjectPass = 0;

  Info("initialize()", "BJetSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Jet Selection... \n");

  // mc event weight (PU contribution multiplied in BaseEventSelection)
  const xAOD::EventInfo* eventInfo = HelperFunctions::getContainer<xAOD::EventInfo>("EventInfo", m_event, m_store);;

  float mcEvtWeight(1.0);
  if (eventInfo->isAvailable< float >( "mcEventWeight" )){
    mcEvtWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  } else {
    Error("execute()  ", "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent++;

  // this will be the collection processed - no matter what!!
  const xAOD::JetContainer* inJets = HelperFunctions::getContainer<xAOD::JetContainer>(m_inContainerName, m_event, m_store);

  return executeConst( inJets, mcEvtWeight );
}


EL::StatusCode BJetSelector :: executeConst ( const xAOD::JetContainer* inJets, float mcEvtWeight )
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Jet Selection... \n");

  // create output container (if requested)
  ConstDataVector<xAOD::JetContainer>* selectedJets = 0;
  if(m_createSelectedContainer) {
    selectedJets = new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
  }

  int nPass(0); int nObj(0);
  for( auto jet_itr : *inJets ){

    // if only looking at a subset of jets make sure all are decorrated
    if( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if(m_decorateSelectedObjects) {
        jet_itr->auxdecor< char >( m_decor ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    int passSel = this->PassCuts( jet_itr );

    // only b-tag jets passing the kinematic selection
    if( m_btagCut >=0 && passSel > 0 ) {
      const xAOD::BTagging *myBTag = jet_itr->btagging();
      if( myBTag->MV1_discriminant() > m_btagCut ) {
        // get efficiency scale factor for jet
      } else {
        // get inefficiency scale factor for jet
        passSel = 0;
      }
    }

    if(m_decorateSelectedObjects) {
      jet_itr->auxdecor< char >( m_decor ) = passSel;
    }

    if(passSel) {
      nPass++;
      if(m_createSelectedContainer) {
        selectedJets->push_back( jet_itr );
      }
    }
  }

  m_numObject     += nObj;
  m_numObjectPass += nPass;

  // apply event selection based on minimal/maximal requirements on the number of objects per event passing cuts
  if( m_pass_min > 0 && nPass < m_pass_min ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }
  if( m_pass_max > 0 && nPass > m_pass_max ) {
    wk()->skipEvent();
    return EL::StatusCode::SUCCESS;
  }

  // add output container to TStore
  if( m_createSelectedContainer ) {
    RETURN_CHECK( "BJetSelector::execute()", m_store->record( selectedJets, m_outContainerName ), "Failed to store container.");
  }

  m_numEventPass++;
  if(m_useCutFlow) {
    m_cutflowHist ->Fill( m_cutflow_bin, 1 );
    m_cutflowHistW->Fill( m_cutflow_bin, mcEvtWeight);
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode BJetSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetSelector :: finalize ()
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

  Info("finalize()", "Deleting tool instances... \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode BJetSelector :: histFinalize ()
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
  return EL::StatusCode::SUCCESS;
}

int BJetSelector :: PassCuts( const xAOD::Jet* jet ) {

  // clean jets
  if( m_cleanJets ) {
    if( jet->isAvailable< char >( "cleanJet" ) ) {
      if( !jet->auxdata< char >( "cleanJet" ) ) { return 0; }
    }
  }

  // pT
  if( m_pT_max != 1e8 ) {
    if( jet->pt() > m_pT_max ) { return 0; }
  }
  if( m_pT_min != 1e8 ) {
    if( jet->pt() < m_pT_min ) { return 0; }
  }

  // eta
  if( m_eta_max != 1e8 ) {
    if( jet->eta() > m_eta_max ) { return 0; }
  }
  if( m_eta_min != 1e8 ) {
    if( jet->eta() < m_eta_min ) { return 0; }
  }

  // mass
  if( m_mass_max != 1e8 ) {
    if( jet->m() > m_mass_max ) { return 0; }
  }
  if( m_mass_min != 1e8 ) {
    if( jet->m() < m_mass_min ) { return 0; }
  }

  // rapidity
  if( m_rapidity_max != 1e8 ) {
    if( fabs(jet->rapidity()) > m_rapidity_max ) { return 0; }
  }

  return 1;
}


