/******************************************
 *
 * Interface to CP Electron selection tool(s).
 *
 * M. Milesi (marco.milesi@cern.ch)
 * Jan 28 15:36 AEST 2015
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
#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODTracking/VertexContainer.h"

// package include(s):
#include "xAODAnaHelpers/ElectronSelector.h"
#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):
#include "ElectronPhotonSelectorTools/AsgElectronIsEMSelector.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "ElectronIsolationSelection/ElectronIsolationSelectionTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ElectronPhotonSelectorTools
// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ElectronIsolationSelectionTool

using HelperClasses::ContainerType;

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronSelector)


ElectronSelector :: ElectronSelector () {
}

ElectronSelector :: ElectronSelector (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_type(ContainerType::UNKNOWN),
  m_cutflowHist(0),
  m_cutflowHistW(0),
  m_asgElectronIsEMSelector(0),
  m_asgElectronLikelihoodTool(0),
  m_electronIsolationSelectionTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("ElectronSelector()", "Calling constructor \n");
}

ElectronSelector::~ElectronSelector() {}

EL::StatusCode  ElectronSelector :: configure ()
{
  Info("configure()", "Configuing ElectronSelector Interface. User configuration read from : %s \n", m_configName.c_str());

  m_type = ContainerType::UNKNOWN;

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "ElectronSelector::configure()", m_configName);

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
  m_pass_max                = config->GetValue("PassMax", -1);
  m_pass_min                = config->GetValue("PassMin", -1);
  m_pT_max                  = config->GetValue("pTMax",  1e8);
  m_pT_min                  = config->GetValue("pTMin",  1e8);
  m_eta_max                 = config->GetValue("etaMax", 1e8);
  m_vetoCrack               = config->GetValue("VetoCrack", true);
  m_d0sig_max     	    = config->GetValue("d0sigMax", 4.0);
  m_z0sintheta_max     	    = config->GetValue("z0sinthetaMax", 10.0);

  m_likelihoodPID  = config->GetValue("LikelihoodPID", "Loose"); // electron PID as defined by LikeEnum enum {VeryLoose, Loose, Medium, Tight, VeryTight, LooseRelaxed} (default is 1 - loose).
  if( m_likelihoodPID != "VeryLoose" &&
      m_likelihoodPID != "Loose"     &&
      m_likelihoodPID != "Medium"    &&
      m_likelihoodPID != "Tight"     &&
      m_likelihoodPID != "VeryTight" &&
      m_likelihoodPID != "LooseRelaxed" ) {
    Error("configure()", "Unknown electron likelihood PID requested %s!",m_likelihoodPID.Data());
    return EL::StatusCode::FAILURE;
  }

  // isolation stuff
  m_useRelativeIso          = config->GetValue("UseRelativeIso"   , true      );
  m_CaloBasedIsoType        = config->GetValue("CaloBasedIsoType" ,	"etcone20");
  m_CaloBasedIsoCut         = config->GetValue("CaloBasedIsoCut"  , 0.05      );
  m_TrackBasedIsoType       = config->GetValue("TrackBasedIsoType",	"ptcone20");
  m_TrackBasedIsoCut        = config->GetValue("TrackBasedIsoCut" , 0.05      );

  m_passAuxDecorKeys         = config->GetValue("PassDecorKeys", "");

  TObjArray* passKeysStrings = m_passAuxDecorKeys.Tokenize(",");
  for(int i = 0; i<passKeysStrings->GetEntries(); ++i) {
    TObjString* passKeyObj = (TObjString*)passKeysStrings->At(i);
    m_passKeys.push_back(passKeyObj->GetString());
  }

  m_failAuxDecorKeys        = config->GetValue("FailDecorKeys", "");
  TObjArray* failKeysStrings = m_failAuxDecorKeys.Tokenize(",");
  for(int i = 0; i<failKeysStrings->GetEntries(); ++i) {
    TObjString* failKeyObj = (TObjString*)failKeysStrings->At(i);
    m_failKeys.push_back(failKeyObj->GetString());
  }


  if( m_inContainerName.Length() == 0 ) {
    Error("configure()", "InputContainer is empty!");
    return EL::StatusCode::FAILURE;
  }

  config->Print();
  Info("configure()", "ElectronSelector Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode ElectronSelector :: setupJob (EL::Job& job)
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
  xAOD::Init( "ElectronSelector" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: histInitialize ()
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



EL::StatusCode ElectronSelector :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "Calling fileExecute \n");


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "Calling changeInput \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing ElectronSelector Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  //std::cout << m_name << " Number of events = " << m_event->getEntries() << std::endl;

  m_numEvent      = 0;
  m_numObject     = 0;
  m_numEventPass  = 0;
  m_weightNumEventPass  = 0;
  m_numObjectPass = 0;

  // initialise AsgElectronIsEMSelector
  m_asgElectronIsEMSelector = new AsgElectronIsEMSelector("AsgElectronIsEMSelector");
  RETURN_CHECK( "ElectronSelector::initialize()", m_asgElectronIsEMSelector->initialize(), "Failed to properly initialize AsgElectronIsEMSelector." );

  // initialise AsgElectronLikelihoodTool
  m_asgElectronLikelihoodTool = new AsgElectronLikelihoodTool("AsgElectronLikelihoodTool");
  m_asgElectronLikelihoodTool->setProperty("primaryVertexContainer", "PrimaryVertices");
  m_asgElectronLikelihoodTool->setProperty("inputPDFFileName", "ElectronPhotonSelectorTools/v1/ElectronLikelihoodPdfs.root");
  HelperClasses::EnumParser<LikeEnum::Menu> likelihoodPIDParser;
  m_asgElectronLikelihoodTool->setProperty("OperatingPoint", static_cast<unsigned int>( likelihoodPIDParser.parseEnum(m_likelihoodPID) ) );
  RETURN_CHECK( "ElectronSelector::initialize()", m_asgElectronLikelihoodTool->initialize(), "Failed to properly initialize AsgElectronLikelihoodTool." );

  // initialise ElectronIsolationSelectionTool
  m_electronIsolationSelectionTool = new CP::ElectronIsolationSelectionTool( "ElectronIsolationSelectionTool" );
  m_electronIsolationSelectionTool->msg().setLevel( MSG::VERBOSE); // ERROR, VERBOSE, DEBUG, INFO
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ElectronIsolationSelectionTool
  HelperClasses::EnumParser<xAOD::Iso::IsolationType> isoParser;
  m_electronIsolationSelectionTool->configureCutBasedIsolation( isoParser.parseEnum(m_CaloBasedIsoType),   static_cast<double>(m_CaloBasedIsoCut),  m_useRelativeIso );
  m_electronIsolationSelectionTool->configureCutBasedIsolation( isoParser.parseEnum(m_TrackBasedIsoType),  static_cast<double>(m_TrackBasedIsoCut), m_useRelativeIso );
  RETURN_CHECK( "ElectronSelector::initialize()", m_electronIsolationSelectionTool->initialize(), "Failed to properly initialize ElectronIsolationSelectionTool." );

  Info("initialize()", "ElectronSelector Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Electron Selection... \n");

  // retrieve mc event weight (PU contribution multiplied in BaseEventSelection)
  const xAOD::EventInfo* eventInfo = 0;
  RETURN_CHECK( "ElectronSelector::execute()", m_event->retrieve(eventInfo, "EventInfo"), "");

  float mcEvtWeight(1.0);
  if (eventInfo->isAvailable< float >( "mcEventWeight" )){
    mcEvtWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  } else {
    Error("execute()  ", "mcEventWeight is not available as decoration! Aborting" );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent++;

  // this will be the collection processed - no matter what!!
  const xAOD::ElectronContainer* inElectrons = 0;

  // if type is not defined then we need to define it
  //  1 = get from TStore
  //  2 = get from TEvent
  if( m_type == ContainerType::UNKNOWN ) {
    if ( m_store->contains< ConstDataVector<xAOD::ElectronContainer> >(m_inContainerName.Data())){
      m_type = ContainerType::CONSTDV;
    }
    else if ( m_event->contains<const xAOD::ElectronContainer>(m_inContainerName.Data())){
      m_type = ContainerType::CONSTCONT;
    }
    else {
      Error("execute()  ", "Failed to retrieve %s container from File or Store. Exiting.", m_inContainerName.Data() );
      m_store->print();
      return EL::StatusCode::FAILURE;
    }
  }

  // Can retrieve collection from input file ( const )
  //           or collection from tstore ( ConstDataVector which then gives a const collection )
  // decide which on first pass
  if ( m_type == ContainerType::CONSTDV ) {        // get ConstDataVector from TStore
    ConstDataVector<xAOD::ElectronContainer>* inElectronsCDV = 0;
    if ( !m_store->retrieve( inElectronsCDV, m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container from Store. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
    inElectrons = inElectronsCDV->asDataVector();
  }
  else if ( m_type == ContainerType::CONSTCONT ) {   // get const container from TEvent
    if ( !m_event->retrieve( inElectrons , m_inContainerName.Data() ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container from File. Exiting.", m_inContainerName.Data() );
      return EL::StatusCode::FAILURE;
    }
  }

  return executeConst( inElectrons, mcEvtWeight );

}

EL::StatusCode ElectronSelector :: executeConst ( const xAOD::ElectronContainer* inElectrons, float mcEvtWeight )
{

  // create output container (if requested)
  ConstDataVector<xAOD::ElectronContainer>* selectedElectrons = 0;
  if(m_createSelectedContainer) {
    selectedElectrons = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
  }

  const xAOD::VertexContainer* vertices = 0;
  RETURN_CHECK( "ElectronSelector::execute()", m_event->retrieve( vertices, "PrimaryVertices" ), "");
  const xAOD::Vertex *pvx = HelperFunctions::getPrimaryVertex(vertices);

  int nPass(0); int nObj(0);
  for( auto el_itr : *inElectrons ) { // duplicated of basic loop

    // if only looking at a subset of electrons make sure all are decorated
    if( m_nToProcess > 0 && nObj >= m_nToProcess ) {
      if(m_decorateSelectedObjects) {
        el_itr->auxdecor< char >( "passSel" ) = -1;
      } else {
        break;
      }
      continue;
    }

    nObj++;
    bool passSel = this->PassCuts( el_itr, pvx );
    if(m_decorateSelectedObjects) {
      el_itr->auxdecor< char >( "passSel" ) = passSel;
    }

    if(passSel) {
      nPass++;
      if(m_createSelectedContainer) {
        selectedElectrons->push_back( el_itr );
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
    RETURN_CHECK( "ElectronSelector::execute()", m_store->record( selectedElectrons, m_outContainerName.Data() ), "Failed to store const data container");
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronSelector :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: finalize ()
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

  if(m_asgElectronIsEMSelector){
    delete m_asgElectronIsEMSelector;
    m_asgElectronIsEMSelector = 0;
  }
  if(m_asgElectronLikelihoodTool){
    delete m_asgElectronLikelihoodTool;
    m_asgElectronLikelihoodTool = 0;
  }
  if(m_electronIsolationSelectionTool){
    delete m_electronIsolationSelectionTool;
    m_electronIsolationSelectionTool = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronSelector :: histFinalize ()
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

int ElectronSelector :: PassCuts( const xAOD::Electron* electron, const xAOD::Vertex *primaryVertex ) {

  int author       = static_cast<int>( electron->author() );
  float et         = static_cast<float>( (electron->caloCluster()->e()) ) / static_cast<float>( cosh(electron->trackParticle()->eta()) );
  float eta        = static_cast<float>( electron->caloCluster()->eta() );
  int oq           = static_cast<int>( electron->auxdata<uint32_t>("OQ") & 1446 );
  float z0sintheta = (static_cast<float>( electron->trackParticle()->z0() ) + static_cast<float>( electron->trackParticle()->vz() ) - static_cast<float>( primaryVertex->z() )) * sin( electron->trackParticle()->theta() );


  // author cut
  if (!(author == 1 || author ==3)) {
      if (m_debug) std::cout << "Electron failed author cut." << std::endl;
      return 0;
  }
  // Object Quality cut
  if (!(oq == 0)) {
      if (m_debug) std::cout << "Electron failed Object Quality cut." << std::endl;
      return 0;
  }
  // pT max
  if( m_pT_max != 1e8 ) {
    if( et > m_pT_max ) {
      if (m_debug) std::cout << "Electron failed pT max cut." << std::endl;
      return 0;
    }
  }
  // pT min
  if( m_pT_min != 1e8 ) {
    if( et < m_pT_min ) {
      if (m_debug) std::cout << "Electron failed pT min cut." << std::endl;
      return 0;
    }
  }
  // |eta| max
  if( m_eta_max != 1e8 ) {
    if( fabs(eta) > m_eta_max ) {
      if (m_debug) std::cout << "Electron failed |eta| max cut." << std::endl;
      return 0;
    }
  }
  // |eta| crack veto
  if( m_vetoCrack ) {
    if( fabs(eta) > 1.37 && fabs(eta) < 1.52 ) {
      if (m_debug) std::cout << "Electron failed |eta| crack veto cut." << std::endl;
      return 0;
    }
  }
  // z0*sin(theta) cut
  if (!(fabs(z0sintheta) < m_z0sintheta_max)) {
      if (m_debug) std::cout << "Electron failed z0*sin(theta) cut." << std::endl;
      return 0;
  }
  // likelihood PID
  if ( ! m_asgElectronLikelihoodTool->accept( *electron ) ){
      if (m_debug) std::cout << "Electron failed likelihood PID cut." << std::endl;
      return 0;
  }
  // isolation
  if ( ! m_electronIsolationSelectionTool->accept( *electron ) ){
    if (m_debug) std::cout << "Electron failed isolation cut." << std::endl;
    return 0;
  }

  return 1;
}


