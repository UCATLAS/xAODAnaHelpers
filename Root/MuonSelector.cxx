#include <iostream>
#include <typeinfo>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODCore/ShallowCopy.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/Vertex.h"
#include "xAODAnaHelpers/MuonSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include "MuonSelectorTools/MuonSelectionTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(MuonSelector)


MuonSelector :: MuonSelector () {
}

MuonSelector :: MuonSelector (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_cutflowHist(0),
  m_cutflowHistW(0),
  m_muonSelectionTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MuonSelector()", "Calling constructor \n");
}

MuonSelector::~MuonSelector() {}

EL::StatusCode  MuonSelector :: configure ()
{
  Info("configure()", "Configuing MuonSelector Interface. User configuration read from : %s \n", m_configName.c_str());

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
  //                            using the SG::View_Element option
  //                            decorrating and output container should not be mutually exclusive
  m_createSelectedContainer = config->GetValue("CreateSelectedContainer", false);
  // if requested, a new container is made using the SG::View_Element option
  m_outContainerName        = config->GetValue("OutputContainer", "");
  m_outAuxContainerName     = m_outContainerName + "Aux."; // the period is very important!

  // if only want to look at a subset of object
  m_nToProcess              = config->GetValue("NToProcess", -1);
  // sort before running selection
  m_sort                    = config->GetValue("Sort",          false);

  // configurable cuts
  m_muonQuality             = config->GetValue("MuonQuality", "Medium"); // muon quality as defined by xAOD::MuonQuality enum {Tight, Medium, Loose, VeryLoose}, corresponding to 0, 1, 2 and 3 (default is 1 - medium quality).
  if( m_muonQuality != "Tight"  &&
      m_muonQuality != "Medium" &&
      m_muonQuality != "Loose"  &&
      m_muonQuality != "VeryLoose" ) {
    Error("configure()", "Unknown muon quality requested %s!",m_muonQuality.Data());
    return EL::StatusCode::FAILURE;
  }
  m_muonType             = config->GetValue("MuonType", ""); // muon type as defined by xAOD::Muon::MuonType enum (0: Combined, 1:MuonStandAlone ,2:SegmentTagged, 3:CaloTagged, 4:SiliconAssociatedForwardMuon  - default is empty string = no type).
  if( m_muonType != ""               &&
      m_muonType != "Combined"       &&
      m_muonType != "MuonStandAlone" &&
      m_muonType != "SegmentTagged"  &&
      m_muonType != "CaloTagged"     &&
      m_muonType != "SiliconAssociatedForwardMuon" ) {
    Error("configure()", "Unknown muon type requested %s!",m_muonType.Data());
    return EL::StatusCode::FAILURE;
  }
  m_pass_max                = config->GetValue("PassMax", -1);
  m_pass_min                = config->GetValue("PassMin", -1);
  m_pT_max                  = config->GetValue("pTMax",  1e8);
  m_pT_min                  = config->GetValue("pTMin",  1e8);
  m_eta_max                 = config->GetValue("etaMax", 1e8);
  m_d0sig_max     	    = config->GetValue("d0sigMax", 4.0);
  m_z0sintheta_max     	    = config->GetValue("z0sinthetaMax", 1.5);

  if( m_inContainerName.Length() == 0 ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "MuonSelector Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonSelector :: setupJob (EL::Job& job)
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
  xAOD::Init( "MuonSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: histInitialize ()
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



EL::StatusCode MuonSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute \n");


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing MuonSelector Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }


  std::cout << m_name << " Number of events = " << m_event->getEntries() << std::endl;

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_numObjectPass = 0;

  if(m_debug){
    std::cout << " Parameters to MuonSelector() : "  << "\n"
	  << "\t m_inContainerName : "         << m_inContainerName.Data()   <<  " of type " <<  typeid(m_inContainerName).name() << "\n"
	  << "\t m_decorateSelectedObjects : " << m_decorateSelectedObjects  <<  " of type " <<  typeid(m_decorateSelectedObjects).name() << "\n"
	  << "\t m_createSelectedContainer : " << m_createSelectedContainer  <<  " of type " <<  typeid(m_createSelectedContainer).name() <<  "\n"
	  << "\t m_outContainerName: "         << m_outContainerName.Data()  <<  " of type " <<  typeid(m_outContainerName).name() << "\n"
	  << "\t m_debug: "		       << m_debug	             <<  " of type " <<  typeid(m_debug).name() <<  "\n"
	  << "\t m_nToProcess: "	       << m_nToProcess  	     <<  " of type " <<  typeid(m_nToProcess).name() << "\n"
	  << "\t m_muonQuality  : "	       << m_muonQuality.Data() 	     <<  " of type " <<  typeid(m_muonQuality).name() <<  "\n"
	  << "\t m_pass_max	: "	       << m_pass_max		     <<  " of type " <<  typeid(m_pass_max).name() << "\n"
	  << "\t m_pass_min	: "	       << m_pass_min		     <<  " of type " <<  typeid(m_pass_min).name() << "\n"
	  << "\t m_pT_max	: "	       << m_pT_max		     <<  " of type " <<  typeid(m_pT_max).name() << "\n"
	  << "\t m_pT_min	: "	       << m_pT_min		     <<  " of type " <<  typeid(m_pT_min).name() << "\n"
	  << "\t m_eta_max	: "	       << m_eta_max		     <<  " of type " <<  typeid(m_eta_max).name() << "\n"
          << std::endl;
  }

  // initialise  muon Selector Tool
  m_muonSelectionTool = new CP::MuonSelectionTool("MuonSelection");
  m_muonSelectionTool->msg().setLevel( MSG::ERROR); // VERBOSE

  HelperClasses::EnumParser<xAOD::Muon::Quality> muQualityParser;

  // set eta and quality requirements in order to accept the muon - ID tracks required by default
  m_muonSelectionTool->setProperty("MaxEta",    static_cast<double>(m_eta_max) ); // default 2.5
  m_muonSelectionTool->setProperty("MuQuality", static_cast<int>(muQualityParser.parseEnum(m_muonQuality))   ); // why is not ok to pass the enum??

  if (! m_muonSelectionTool->initialize().isSuccess() ){
    Error("initialize()", "Failed to properly initialize the Muon Selection Tool. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  Info("initialize()", "MuonSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Muon Selection... \n");

  float mcEvtWeight(1); // FIXME - set to something from eventInfo

  m_numEvent++;

  // get the collection from TEvent or TStore
  xAOD::MuonContainer* inMuons = 0;
  if ( !m_event->retrieve( inMuons , m_inContainerName.Data() ).isSuccess() ){
    if ( !m_store->retrieve( inMuons , m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  // get primary vertex
  const xAOD::VertexContainer *vertices = 0;
  if (!m_event->retrieve(vertices, "PrimaryVertices").isSuccess()) {
      Error("execute()", "Failed to retrieve PrimaryVertices. Exiting.");
      return EL::StatusCode::FAILURE;
  }
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);

  if(m_sort) {
    std::sort( inMuons->begin(), inMuons->end(), HelperFunctions::sort_pt );
  }

  // create output container (if requested) - deep copy
  xAOD::MuonContainer* selectedMuons = 0;
  if(m_createSelectedContainer) {
    selectedMuons = new xAOD::MuonContainer(SG::VIEW_ELEMENTS);
  }

  xAOD::MuonContainer::iterator muon_itr = inMuons->begin();
  xAOD::MuonContainer::iterator muon_end = inMuons->end();
  int nPass(0); int nObj(0);
  for( ; muon_itr != muon_end; ++muon_itr ){

    // if only looking at a subset of muons make sure all are decorrated
    if( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if(m_decorateSelectedObjects) {
        (*muon_itr)->auxdecor< int >( "passSel" ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    int passSel = this->PassCuts( (*muon_itr), pvx );
    if(m_decorateSelectedObjects) {
      (*muon_itr)->auxdecor< int >( "passSel" ) = passSel;
    }
    if(passSel) {
      nPass++;
      if(m_createSelectedContainer) {
        selectedMuons->push_back( *muon_itr );
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
    if( !m_store->record( selectedMuons, m_outContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  m_numEventPass++;
  if(m_useCutFlow) {
    m_cutflowHist ->Fill( m_cutflow_bin, 1 );
    m_cutflowHistW->Fill( m_cutflow_bin, mcEvtWeight);
  }

  // shall we delete new container passed to TStore?
  //delete selectedMuons;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MuonSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: finalize ()
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

  if(m_muonSelectionTool){
    delete m_muonSelectionTool;
    m_muonSelectionTool = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MuonSelector :: histFinalize ()
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

  Info("histFinalize()", "Calling histFinalize \n");

  return EL::StatusCode::SUCCESS;
}

int MuonSelector :: PassCuts( const xAOD::Muon* muon, const xAOD::Vertex *primaryVertex  ) {

  //compute sigma(d0)
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackingDC14
  // maybe need this
  const xAOD::TrackParticle* tp  = const_cast<xAOD::TrackParticle*>(muon->primaryTrackParticle());

  float d0_significance = fabs(tp->d0()) / sqrt(tp->definingParametersCovMatrix()(0,0));
  float z0sintheta      = (static_cast<float>( tp->z0() ) + static_cast<float>( tp->vz() ) - static_cast<float>( primaryVertex->z() )) * sin( tp->theta() );

  // pT max
  if( m_pT_max != 1e8 ) {
    if(  muon->pt() > m_pT_max ) {
      if (m_debug) std::cout << "Muon failed pT max cut." << std::endl;
      return 0;
    }
  }
  // pT min
  if( m_pT_min != 1e8 ) {
    if( muon->pt() < m_pT_min ) {
      if (m_debug) std::cout << "Muon failed pT min cut." << std::endl;
      return 0;
    }
  }
  // |eta| max
  if( m_eta_max != 1e8 ) {
    if( fabs(muon->eta()) > m_eta_max ) {
      if (m_debug) std::cout << "Muon failed |eta| max cut." << std::endl;
      return 0;
    }
  }
  // d0sig cut
  if (!( d0_significance < m_d0sig_max ) ) {
      if (m_debug) std::cout << "Muon failed d0 significance cut." << std::endl;
      return 0;
  }
  // z0*sin(theta) cut
  if (!(fabs(z0sintheta) < m_z0sintheta_max)) {
      if (m_debug) std::cout << "Muon failed z0*sin(theta) cut." << std::endl;
      return 0;
  }

  // retireve muon quality
  xAOD::Muon::Quality my_quality = m_muonSelectionTool->getQuality( *muon );
  if(m_debug) std::cout << "Muon quality " << static_cast<int>(my_quality) << std::endl;
  /*
  if(my_quality <= xAOD::Muon::VeryLoose)
    verylooseMuons++;
  if(my_quality <= xAOD::Muon::Loose)
    looseMuons++;
  if(my_quality <= xAOD::Muon::Medium)
    mediumMuons++;
  if(my_quality <= xAOD::Muon::Tight)
    tightMuons++;
  if(my_quality <= xAOD::Muon::VeryLoose && !(my_quality <= xAOD::Muon::Loose))
    badMuons++;
  */

  // if specified, cut on muon quality
  HelperClasses::EnumParser<xAOD::Muon::MuonType> muTypeParser;
  //static_cast<int>(muTypeParser.parseEnum(m_muonType))
  if( m_muonType.Length() != 0){
    if( muon->muonType() != static_cast<int>(muTypeParser.parseEnum(m_muonType))) {
      if (m_debug) std::cout << "Muon type: " << muon->muonType() <<" - required: " << m_muonType << " . Failed" << std::endl;
      return 0;
    }
  }

  // isolation MAKE CONFIGURABLE
  float ptcone20 = -999., etcone20 = -999.;
  if ( muon->isolation(ptcone20, xAOD::Iso::ptcone20) &&  muon->isolation(etcone20, xAOD::Iso::etcone20) ){
    bool isTrackIso = ( ptcone20 / (muon->pt()) > 0.0 && ptcone20 / (muon->pt()) < 0.1 ) ? true : false;
    bool isCaloIso  = ( etcone20 / (muon->pt()) > 0.0 && etcone20 / (muon->pt()) < 0.1 ) ? true : false;
    if( !( isTrackIso && isCaloIso ) ){
      if (m_debug) std::cout << "Muon failed isolation cut " << std::endl;
      return 0;
    }
  }

  // this will accept the muon based on the settings at initialization
  if ( ! m_muonSelectionTool->accept( *muon ) ){
    if (m_debug) std::cout << "Muon failed requirements of MuonSelectionTool." << std::endl;
    return 0;
  }

  return 1;
}


