#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODAnaHelpers/JetSelector.h"
#include "xAODAnaHelpers/HelperFunctions.h"

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetSelector)


JetSelector :: JetSelector () {
}

JetSelector :: JetSelector (std::string name, std::string configName) :
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
  Info("JetSelector()", "Calling constructor \n");
}

EL::StatusCode  JetSelector :: configure ()
{
  Info("configure()", "Configuing JetSelector Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  // check if file exists
  /* https://root.cern.ch/root/roottalk/roottalk02/5332.html */
  FileStat_t fStats;
  int fSuccess = gSystem->GetPathInfo(m_configName.c_str(), fStats);
  if(fSuccess != 0){
    Error("configure()", "Could not find the configuration file");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Found configuration file");
  
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
  m_sort                    = config->GetValue("Sort",          false);

  m_isEMjet = ( static_cast<bool>(m_inContainerName.Contains("EMTopoJets",TString::kIgnoreCase)) ) ? true : false;
  m_isLCjet = ( static_cast<bool>(m_inContainerName.Contains("LCTopoJets",TString::kIgnoreCase)) ) ? true : false;

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
  m_rapidity_min            = config->GetValue("rapidityMin", 1e8);

  m_doJVF 		    = config->GetValue("DoJVF", false);
  m_pt_max_JVF 		    = config->GetValue("pTMaxJVF",       1e8);
  m_eta_max_JVF 	    = config->GetValue("etaMaxJVF",      1e8);
  m_JVFCut 		    = config->GetValue("JVFCut", 0.5);

  if( m_inContainerName.Length() == 0 ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "JetSelector Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
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

  Info("setupJob()", "Calling setupJob \n");

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



EL::StatusCode JetSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput \n");

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

  Info("initialize()", "JetSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Jet Selection... \n");

  float mcEvtWeight(1); // FIXME - set to something from eventInfo

  m_numEvent++;

  // get the collection from TEvent or TStore (NB: if retrieving original xAOD container, must be const!!)
  xAOD::JetContainer* inJets = 0;
  if ( !m_event->retrieve( inJets , m_inContainerName.Data() ).isSuccess() ){
    if ( !m_store->retrieve( inJets , m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  if(m_sort) {
    std::sort( inJets->begin(), inJets->end(), HelperFunctions::sort_pt );
  }

  // create output container (if requested) - deep copy

  xAOD::JetContainer* selectedJets = 0;
  if(m_createSelectedContainer) {
    selectedJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);
  }

  xAOD::JetContainer::iterator jet_itr = inJets->begin();
  xAOD::JetContainer::iterator jet_end = inJets->end();
  int nPass(0); int nObj(0);
  for( ; jet_itr != jet_end; ++jet_itr ){

    // if only looking at a subset of jets make sure all are decorrated
    if( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if(m_decorateSelectedObjects) {
        (*jet_itr)->auxdecor< int >( "passSel" ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    int passSel = this->PassCuts( (*jet_itr) );
    if(m_decorateSelectedObjects) {
      (*jet_itr)->auxdecor< int >( "passSel" ) = passSel;
    }

    if(passSel) {
      nPass++;
      if(m_createSelectedContainer) {
        selectedJets->push_back( *jet_itr );
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
    if( !m_store->record( selectedJets, m_outContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  m_numEventPass++;
  if(m_useCutFlow) {
    m_cutflowHist ->Fill( m_cutflow_bin, 1 );
    m_cutflowHistW->Fill( m_cutflow_bin, mcEvtWeight);
  }

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

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

  Info("finalize()", "Deleting tool instances... \n");

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
  return EL::StatusCode::SUCCESS;
}

int JetSelector :: PassCuts( const xAOD::Jet* jet ) {

  // clean jets
  if( m_cleanJets ) {
    if( jet->isAvailable< int >( "cleanJet" ) ) {
      if( !jet->auxdata< int >( "cleanJet" ) ) { return 0; }
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
    if( jet->rapidity() > m_rapidity_max ) { return 0; }
  }
  if( m_rapidity_min != 1e8 ) {
    if( jet->rapidity() < m_rapidity_min ) { return 0; }
  }

  // JVF pileup cut
  if( m_doJVF ){
    if( jet->pt() < m_pt_max_JVF &&  jet->eta() < m_eta_max_JVF ){
       std::vector<float> jvf_vec = jet->getAttribute< std::vector<float> >(xAOD::JetAttribute::JVF);
       // find maximum JVF
       float JVFMax = -1.0;
       for (auto jvf_itr = jvf_vec.begin(); jvf_itr != jvf_vec.end(); ++jvf_itr){
         if( *jvf_itr > JVFMax ) JVFMax = *jvf_itr;
       }
       if ( fabs(JVFMax) < m_JVFCut ) { return 0; }
    }
  }

  return 1;
}


