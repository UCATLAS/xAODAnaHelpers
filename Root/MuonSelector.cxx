/******************************************
 *
 * Interface to CP Muon selection tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 * Jan 28 15:48 AEST 2015
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
#include "xAODCore/ShallowCopy.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/VertexContainer.h"

// package include(s):
#include "xAODAnaHelpers/MuonSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):
#include "MuonSelectorTools/MuonSelectionTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

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
  RETURN_CHECK_CONFIG( "MuonSelector::configure()", m_configName);

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
  /* initialize set to check for appropriate values, much faster in long run */
  std::set<std::string> muonQualitySet;
  muonQualitySet.insert("Tight");
  muonQualitySet.insert("Medium");
  muonQualitySet.insert("Loose");
  muonQualitySet.insert("VeryLoose");
  if( muonQualitySet.find(m_muonQuality) == muonQualitySet.end()){
    Error("configure()", "Unknown muon quality requested %s!",m_muonQuality.c_str());
    return EL::StatusCode::FAILURE;
  }
  m_muonType                = config->GetValue("MuonType", ""); // muon type as defined by xAOD::Muon::MuonType enum (0: Combined, 1:MuonStandAlone ,2:SegmentTagged, 3:CaloTagged, 4:SiliconAssociatedForwardMuon  - default is empty string = no type).
  std::set<std::string> muonTypeSet;
  muonTypeSet.insert("");
  muonTypeSet.insert("Combined");
  muonTypeSet.insert("MuonStandAlone");
  muonTypeSet.insert("SegmentTagged");
  muonTypeSet.insert("CaloTagged");
  muonTypeSet.insert("SiliconAssociatedForwardMuon");

  if( muonTypeSet.find(m_muonType) == muonTypeSet.end()){
    Error("configure()", "Unknown muon type requested %s!",m_muonType.c_str());
    return EL::StatusCode::FAILURE;
  }
  m_pass_max                = config->GetValue("PassMax", -1);
  m_pass_min                = config->GetValue("PassMin", -1);
  m_pT_max                  = config->GetValue("pTMax",  1e8);
  m_pT_min                  = config->GetValue("pTMin",  1e8);
  m_eta_max                 = config->GetValue("etaMax", 1e8);
  m_d0sig_max     	        = config->GetValue("d0sigMax", 4.0);
  m_z0sintheta_max     	    = config->GetValue("z0sinthetaMax", 1.5);

  // isolation stuff
  m_CaloBasedIsoType        = config->GetValue("CaloBasedIsoType" ,	"etcone20");
  m_CaloBasedIsoCut         = config->GetValue("CaloBasedIsoCut"  , 0.05      );
  m_TrackBasedIsoType       = config->GetValue("TrackBasedIsoType",	"ptcone20");
  m_TrackBasedIsoCut        = config->GetValue("TrackBasedIsoCut" , 0.05      );


  // parse and split by comma
  std::string token;

  m_passAuxDecorKeys        = config->GetValue("PassDecorKeys", "");
  std::istringstream ss(m_passAuxDecorKeys);
  while(std::getline(ss, token, ',')){
    m_passKeys.push_back(token);
  }

  m_failAuxDecorKeys        = config->GetValue("FailDecorKeys", "");
  ss.str(m_failAuxDecorKeys);
  while(std::getline(ss, token, ',')){
    m_failKeys.push_back(token);
  }

  if( m_inContainerName.empty() ){
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
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  if(m_debug){
    std::cout << " Parameters to MuonSelector() : "  << "\n"
	  << "\t m_inContainerName : "         << m_inContainerName.c_str()   <<  " of type " <<  typeid(m_inContainerName).name() << "\n"
	  << "\t m_decorateSelectedObjects : " << m_decorateSelectedObjects  <<  " of type " <<  typeid(m_decorateSelectedObjects).name() << "\n"
	  << "\t m_createSelectedContainer : " << m_createSelectedContainer  <<  " of type " <<  typeid(m_createSelectedContainer).name() <<  "\n"
	  << "\t m_outContainerName: "         << m_outContainerName.c_str()  <<  " of type " <<  typeid(m_outContainerName).name() << "\n"
	  << "\t m_debug: "		       << m_debug	             <<  " of type " <<  typeid(m_debug).name() <<  "\n"
	  << "\t m_nToProcess: "	       << m_nToProcess  	     <<  " of type " <<  typeid(m_nToProcess).name() << "\n"
	  << "\t m_muonQuality  : "	       << m_muonQuality.c_str() 	     <<  " of type " <<  typeid(m_muonQuality).name() <<  "\n"
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

  RETURN_CHECK("MuonSelector::initialize()", m_muonSelectionTool->initialize(), "Failed to properly initialize the Muon Selection Tool");

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

  // mc event weight (PU contribution multiplied in BaseEventSelection)
  const xAOD::EventInfo* eventInfo = HelperFunctions::getContainer<xAOD::EventInfo>( "EventInfo", m_event, m_store);

  float mcEvtWeight(1.0);
  if (eventInfo->isAvailable< float >( "mcEventWeight" )){
    mcEvtWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  } else {
    Error("execute()  ", "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent++;

  // this will be the collection processed - no matter what!!
  const xAOD::MuonContainer* inMuons = HelperFunctions::getContainer<xAOD::MuonContainer>(m_inContainerName, m_event, m_store);

  return executeConst( inMuons, mcEvtWeight );

}

EL::StatusCode MuonSelector :: executeConst ( const xAOD::MuonContainer* inMuons, float mcEvtWeight )
{

  // create output container (if requested)
  ConstDataVector<xAOD::MuonContainer>* selectedMuons = 0;
  if(m_createSelectedContainer) {
    selectedMuons = new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
  }

  // get primary vertex
  const xAOD::VertexContainer *vertices = HelperFunctions::getContainer<xAOD::VertexContainer>("PrimaryVertices", m_event, m_store);
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);


  int nPass(0); int nObj(0);
  for( auto mu_itr : *inMuons ) { // duplicated of basic loop

    // if only looking at a subset of muons make sure all are decorrated
    if( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if(m_decorateSelectedObjects) {
        mu_itr->auxdecor< char >( "passSel" ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    int passSel = this->PassCuts( mu_itr, pvx );
    if(m_decorateSelectedObjects) {
      mu_itr->auxdecor< char >( "passSel" ) = passSel;
    }

    if(passSel) {
      nPass++;
      if(m_createSelectedContainer) {
        selectedMuons->push_back( mu_itr );
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

  m_numEventPass++;
  m_weightNumEventPass += mcEvtWeight;

  // add ConstDataVector to TStore
  if(m_createSelectedContainer) {
    RETURN_CHECK("MuonSelector::execute()", m_store->record( selectedMuons, m_outContainerName ), "Failed to store const data container");
  }

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
  if(m_useCutFlow) {
    Info("histFinalize()", "Filling cutflow");
    m_cutflowHist ->SetBinContent( m_cutflow_bin, m_numEventPass        );
    m_cutflowHistW->SetBinContent( m_cutflow_bin, m_weightNumEventPass  );
  }
  return EL::StatusCode::SUCCESS;
}

int MuonSelector :: PassCuts( const xAOD::Muon* muon, const xAOD::Vertex *primaryVertex  ) {

  //compute sigma(d0)
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackingDC14
  // maybe need this
  const xAOD::TrackParticle* tp  = const_cast<xAOD::TrackParticle*>(muon->primaryTrackParticle());

  int type = static_cast<int>(muon->muonType());

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

  // do not cut on impact parameter if muon is Standalone
  if ( type != xAOD::Muon::MuonType::MuonStandAlone ){
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
  }

  // muon quality
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
  if( !m_muonType.empty() ){
    if( muon->muonType() != static_cast<int>(muTypeParser.parseEnum(m_muonType))) {
      if (m_debug) std::cout << "Muon type: " << muon->muonType() <<" - required: " << m_muonType.c_str() << " . Failed" << std::endl;
      return 0;
    }
  }

  // isolation
  HelperClasses::EnumParser<xAOD::Iso::IsolationType> isoParser;
  float ptcone_dr = -999., etcone_dr = -999.;
  if ( muon->isolation(ptcone_dr, isoParser.parseEnum(m_TrackBasedIsoType)) &&  muon->isolation(etcone_dr,isoParser.parseEnum(m_CaloBasedIsoType)) ){
    bool isTrackIso = ( ptcone_dr / (muon->pt()) > 0.0 && ptcone_dr / (muon->pt()) <  m_TrackBasedIsoCut) ? true : false;
    bool isCaloIso  = ( etcone_dr / (muon->pt()) > 0.0 && etcone_dr / (muon->pt()) <  m_CaloBasedIsoCut) ? true : false;
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

