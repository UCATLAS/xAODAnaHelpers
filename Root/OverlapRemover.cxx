/******************************************
 *
 * Interface to OverlapRemoval tool (applying recommendations from Harmonisation TF).
 *
 * M. Milesi (marco.milesi@cern.ch)
 * Jan 28 15:48 AEST 2015
 *
 ******************************************/

// c++ include(s):
#include <iostream>
#include <sstream>

// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

// EDM include(s):
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/Photon.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/Muon.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/Jet.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODTau/TauJet.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODCore/ShallowCopy.h"

// package include(s):
#include "xAODAnaHelpers/OverlapRemover.h"
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/HelperClasses.h"

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

// external tools include(s):
#include "AssociationUtils/OverlapRemovalTool.h"

// ROOT include(s):
#include "TEnv.h"
#include "TSystem.h"

using HelperClasses::ToolName;

// this is needed to distribute the algorithm to the workers
ClassImp(OverlapRemover)


OverlapRemover :: OverlapRemover () {
}

OverlapRemover :: OverlapRemover (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_overlapRemovalTool(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("OverlapRemover()", "Calling constructor \n");

}

EL::StatusCode  OverlapRemover :: configure ()
{
  Info("configure()", "Configuing OverlapRemover Interface. User configuration read from : %s \n", m_configName.c_str());

  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "OverlapRemover::configure()", m_configName);

  TEnv* config = new TEnv(m_configName.c_str());

  // read debug flag from .config file
  m_debug         = config->GetValue("Debug" , false );
  // input container(s) to be read from TEvent or TStore

  /* Muons */
  m_inContainerName_Muons         	     = config->GetValue("InputContainerMuons",  "");
  /* Electrons */
  m_inContainerName_Electrons                = config->GetValue("InputContainerElectrons",  "");
  /* Photons */
  m_inContainerName_Photons                  = config->GetValue("InputContainerPhotons",  "");
  /* Taus */
  m_inContainerName_Taus                     = config->GetValue("InputContainerTaus",  "");
  /* Jets */
  m_inContainerName_Jets                     = config->GetValue("InputContainerJets",  "");

  if( m_inContainerName_Muons.empty() ) {
    Error("configure()", "InputContainerMuons is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  if( m_inContainerName_Electrons.empty() ) {
    Error("configure()", "InputContainerElectrons is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  if( m_inContainerName_Jets.empty() ) {
    Error("configure()", "InputContainerJets is empty! Must have it to perform Overlap Removal! Exiting.");
    return EL::StatusCode::FAILURE;
  }


  // decorate selected objects that pass the cuts
  m_decorateSelectedObjects = config->GetValue("DecorateSelectedObjects", true);
  // additional functionality : create output container of selected objects
  //                            using the SG::View_Element option
  //                            decorating and output container should not be mutually exclusive
  m_createSelectedContainers = config->GetValue("CreateSelectedContainers", false);

  m_useSelected = config->GetValue("UseSelected", false);

  m_outContainerName_Electrons        = config->GetValue("OutputContainerElectrons", "");
  m_outAuxContainerName_Electrons     = m_outContainerName_Electrons + "Aux."; // the period is very important!

  m_outContainerName_Muons        = config->GetValue("OutputContainerMuons", "");
  m_outAuxContainerName_Muons     = m_outContainerName_Muons + "Aux."; // the period is very important!

  m_outContainerName_Jets        = config->GetValue("OutputContainerJets", "");
  m_outAuxContainerName_Jets     = m_outContainerName_Jets + "Aux."; // the period is very important!

  m_outContainerName_Photons        = config->GetValue("OutputContainerPhotons", "");
  m_outAuxContainerName_Photons     = m_outContainerName_Photons + "Aux."; // the period is very important!

  m_outContainerName_Taus        = config->GetValue("OutputContainerTaus", "");
  m_outAuxContainerName_Taus     = m_outContainerName_Taus + "Aux."; // the period is very important!

  config->Print();
  Info("configure()", "OverlapRemover Interface succesfully configured! \n");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: setupJob (EL::Job& job)
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
  xAOD::Init( "OverlapRemover" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initializing OverlapRemover Interface... \n");

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events: %lld ", m_event->getEntries() );

  if ( configure() == EL::StatusCode::FAILURE ) {
    Error("initialize()", "Failed to properly configure. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_numEvent      = 0;
  m_numObject     = 0;

  // initialize overlap removal tool
  m_overlapRemovalTool = new OverlapRemovalTool("OverlapRemovalTool");
  m_overlapRemovalTool->msg().setLevel( MSG::DEBUG ); // VERBOSE, INFO, DEBUG

  // set object decoration
  const std::string selected_label = (m_useSelected) ? "passSel" : "";  // set with decoration flag you use for selected objects if want to consider only selected objects in OR, otherwise it will perform OR on all objects
  m_overlapRemovalTool->setProperty("InputLabel",  selected_label);
  m_overlapRemovalTool->setProperty("OverlapLabel", "overlaps"); // tool will decorate objects with 'overlaps' boolean if they overlap (not possible to customise name atm!)
  RETURN_CHECK( "OverlapRemover::initialize()", m_overlapRemovalTool->initialize(), "Failed to properly initialize the OverlapRemovalTool.");

  Info("initialize()", "OverlapRemover Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if(m_debug) Info("execute()", "Applying Overlap Removal... \n");

  m_numEvent++;

  // get the collections from TEvent or TStore

  const xAOD::MuonContainer* inMuons            = 0;
  const xAOD::ElectronContainer* inElectrons    = 0;
  const xAOD::JetContainer* inJets              = 0;
  const xAOD::PhotonContainer* inPhotons        = 0;
  const xAOD::TauJetContainer* inTaus           = 0;
  inMuons	     = HelperClasses::getContainer<xAOD::MuonContainer>(     m_inContainerName_Muons,	     m_event, m_store);
  inElectrons	     = HelperClasses::getContainer<xAOD::ElectronContainer>( m_inContainerName_Electrons,    m_event, m_store);
  inJets	     = HelperClasses::getContainer<xAOD::JetContainer>(      m_inContainerName_Jets,	     m_event, m_store);
  if( !m_inContainerName_Photons.empty() ) inPhotons	     = HelperClasses::getContainer<xAOD::PhotonContainer>(   m_inContainerName_Photons,	   m_event, m_store);
  if( !m_inContainerName_Taus.empty() )    inTaus	     = HelperClasses::getContainer<xAOD::TauJetContainer>(   m_inContainerName_Taus,	   m_event, m_store);

  // remove overlaps
  /*
  =======================================================
  https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/AnalysisCommon/AssociationUtils/trunk/doc/README.rst
  ========================================================
  */

  if(m_debug){
    m_store->print();
    Info("execute()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size());
  }

  m_overlapRemovalTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons);

  return executeConst( inElectrons, inMuons, inJets, inTaus, inPhotons );
}


EL::StatusCode OverlapRemover :: executeConst ( const xAOD::ElectronContainer* inElectrons,
						const xAOD::MuonContainer* inMuons,
						const xAOD::JetContainer* inJets,
						const xAOD::TauJetContainer* inTaus,
						const xAOD::PhotonContainer* inPhotons
					      )
{

  // remove overlaps
  /*
  =======================================================
  https://svnweb.cern.ch/trac/atlasoff/browser/PhysicsAnalysis/AnalysisCommon/AssociationUtils/trunk/doc/README.rst
  ========================================================
  */
  /*
  if(m_debug){
    Info("executeConst()",  "inElectrons : %lu, inMuons : %lu, inJets : %lu", inElectrons->size(), inMuons->size(),  inJets->size());
  }

  m_overlapRemovalTool->removeOverlaps(inElectrons, inMuons, inJets, inTaus, inPhotons);
  */
  // debug : check that something has been done
  if(m_debug){
    // electrons
    Info("executeConst()", "Now dumping the electrons");
    printOverlapInfo("electron", inElectrons, "passSel", "overlaps");
    //for(auto electron : *inElectrons) printOverlapInfo("electron", electron, "passSel", "overlaps");
    // muons
    Info("executeConst()", "Now dumping the muons");
    printOverlapInfo("muon", inMuons, "passSel", "overlaps");
    //for(auto muon : *inMuons) printOverlapInfo("muon", muon, "passSel", "overlaps");
    // jets
    Info("executeConst()",  "Now dumping the jets");
    printOverlapInfo("jet", inJets, "passSel", "overlaps");
    //for(auto jet : *inJets) printOverlapInfo("jet", jet, "passSel", "overlaps");
  }

  ///*
  // instantiate output container(s)
  ConstDataVector<xAOD::ElectronContainer> *selectedElectrons   = 0;
  ConstDataVector<xAOD::MuonContainer>     *selectedMuons	= 0;
  ConstDataVector<xAOD::JetContainer>      *selectedJets	= 0;
  ConstDataVector<xAOD::PhotonContainer>   *selectedPhotons	= 0;
  ConstDataVector<xAOD::TauJetContainer>   *selectedTaus	= 0;

  // make a copy of input container(s) with selected objects
  if(m_createSelectedContainers) {
    selectedElectrons   = new ConstDataVector<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
    selectedMuons	= new ConstDataVector<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
    selectedJets	= new ConstDataVector<xAOD::JetContainer>(SG::VIEW_ELEMENTS);
    selectedPhotons	= new ConstDataVector<xAOD::PhotonContainer>(SG::VIEW_ELEMENTS);
    selectedTaus	= new ConstDataVector<xAOD::TauJetContainer>(SG::VIEW_ELEMENTS);
  }

  RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, "overlaps", ToolName::OVERLAPREMOVER), "");
  RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inMuons, selectedMuons, "overlaps", ToolName::OVERLAPREMOVER), "");
  RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inJets, selectedJets, "overlaps", ToolName::OVERLAPREMOVER), "");
  if( !m_inContainerName_Photons.empty() )
  {
    RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, "overlaps", ToolName::OVERLAPREMOVER), "");
  }
  if( !m_inContainerName_Taus.empty() )
  {
    RETURN_CHECK( "OverlapRemover::execute()", HelperFunctions::makeSubsetCont(inTaus, selectedTaus, "overlaps", ToolName::OVERLAPREMOVER), "");
  }

  // add ConstDataVector to TStore
  if(m_createSelectedContainers) {
    RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedElectrons, m_outContainerName_Electrons ), "Failed to store const data container");
    RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedMuons, m_outContainerName_Muons ), "Failed to store const data container");
    RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedJets, m_outContainerName_Jets ), "Failed to store const data container");
    if( !m_inContainerName_Photons.empty() ){
      RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedPhotons, m_outContainerName_Photons ), "Failed to store const data container");
    }
    if( !m_inContainerName_Taus.empty() ){
      RETURN_CHECK( "OverlapRemover::execute()", m_store->record( selectedTaus, m_outContainerName_Taus ), "Failed to store const data container");
    }
  }

  //*******************************************************
  /*
  // make a copy of input container(s) with selected objects
  xAOD::ElectronContainer *selectedElectrons = 0;
  xAOD::MuonContainer	  *selectedMuons     = 0;
  xAOD::JetContainer	  *selectedJets      = 0;
  xAOD::PhotonContainer   *selectedPhotons   = 0;
  xAOD::TauJetContainer   *selectedTaus      = 0;
  if(m_createSelectedContainers) {

    // instantiate output container(s)
    selectedElectrons = new xAOD::ElectronContainer(SG::VIEW_ELEMENTS);
    selectedMuons     = new xAOD::MuonContainer(SG::VIEW_ELEMENTS);
    selectedJets      = new xAOD::JetContainer(SG::VIEW_ELEMENTS);
    selectedPhotons   = new xAOD::PhotonContainer(SG::VIEW_ELEMENTS);
    selectedTaus      = new xAOD::TauJetContainer(SG::VIEW_ELEMENTS);

    if( ! HelperFunctions::makeSubsetCont(inElectrons, selectedElectrons, "overlaps") ){
        Error("execute()  ", "Failed to make subset container of %s. Exiting.", m_inContainerName_Electrons.c_str() );
        return EL::StatusCode::FAILURE;
    }
    if( ! HelperFunctions::makeSubsetCont(inMuons, selectedMuons, "overlaps") ){
        Error("execute()  ", "Failed to make subset container of %s. Exiting.", m_inContainerName_Muons.c_str() );
        return EL::StatusCode::FAILURE;
    }
    if( ! HelperFunctions::makeSubsetCont(inJets, selectedJets, "overlaps") ){
        Error("execute()  ", "Failed to make subset container of %s. Exiting.", m_inContainerName_Jets.c_str() );
        return EL::StatusCode::FAILURE;
    }
    if( m_inContainerName_Photons.empty() )
    {
      if( ! HelperFunctions::makeSubsetCont(inPhotons, selectedPhotons, "overlaps") ){
        Error("execute()  ", "Failed to make subset container of %s. Exiting.", m_inContainerName_Photons.c_str() );
        return EL::StatusCode::FAILURE;
      }
    }
    if( m_inContainerName_Taus.empty() )
    {
      if( ! HelperFunctions::makeSubsetCont(inTaus, selectedTaus, "overlaps") ){
        Error("execute()  ", "Failed to make subset container of %s. Exiting.", m_inContainerName_Taus.c_str() );
        return EL::StatusCode::FAILURE;
      }
    }

    if( !m_store->record( selectedElectrons, m_outContainerName_Electrons ).isSuccess() ){
      Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName_Electrons.c_str() );
      return EL::StatusCode::FAILURE;
    }

    if( !m_store->record( selectedMuons, m_outContainerName_Muons ).isSuccess() ){
      Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName_Muons.c_str() );
      return EL::StatusCode::FAILURE;
    }

    if( !m_store->record( selectedJets, m_outContainerName_Jets ).isSuccess() ){
      Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName_Jets.c_str() );
      return EL::StatusCode::FAILURE;
    }

    if( !m_inContainerName_Photons.empty() && !m_outContainerName_Photons.empty()  )
    {
      if( !m_store->record( selectedPhotons, m_outContainerName_Photons ).isSuccess() ){
        Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName_Photons.c_str() );
        return EL::StatusCode::FAILURE;
      }
    }

    if( !m_inContainerName_Taus.empty() && !m_outContainerName_Taus.empty() )
    {
      if( !m_store->record( selectedTaus, m_outContainerName_Taus ).isSuccess() ){
        Error("execute()  ", "Failed to store container %s. Exiting.", m_outContainerName_Taus.c_str() );
        return EL::StatusCode::FAILURE;
      }
    }

  }
  */

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode OverlapRemover :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  if(m_debug) Info("postExecute()", "Calling postExecute \n");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: finalize ()
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

  if(m_overlapRemovalTool){
    delete m_overlapRemovalTool;
    m_overlapRemovalTool = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode OverlapRemover :: histFinalize ()
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


// Print object information

EL::StatusCode OverlapRemover :: printOverlapInfo (const char* type, const xAOD::IParticleContainer* objCont, const std::string& selectFlag, const std::string& overlapFlag)
{
  static SG::AuxElement::ConstAccessor<char> selectAcc(selectFlag);
  static SG::AuxElement::ConstAccessor<char> overlapAcc(overlapFlag);
  static SG::AuxElement::ConstAccessor< ElementLink<xAOD::IParticleContainer> > objLinkAcc("overlapObject");

  for( auto obj_itr = objCont->begin(); obj_itr != objCont->end(); ++obj_itr ) {

    // Safety check
    if(!overlapAcc.isAvailable(*(*obj_itr))){
      Error("printObj()", "Overlap decoration missing for object of type %s ", type );
      return EL::StatusCode::FAILURE;
    }
    if(selectAcc.isAvailable(*(*obj_itr))){
      Info("printObj()", "  %s pt %6.2f eta %5.2f phi %5.2f selected %i overlaps %i \n", type, (*obj_itr)->pt()/1000., (*obj_itr)->eta(), (*obj_itr)->phi(), selectAcc(*(*obj_itr)), overlapAcc(*(*obj_itr)));
    } else {
      Info("printObj()", "  %s pt %6.2f eta %5.2f phi %5.2f overlaps %i \n", type, (*obj_itr)->pt()/1000., (*obj_itr)->eta(), (*obj_itr)->phi(), overlapAcc(*(*obj_itr)));
    }
    // Check for overlap object link
    if(objLinkAcc.isAvailable(*(*obj_itr)) && objLinkAcc(*(*obj_itr)).isValid()){
      const xAOD::IParticle* overlapObj = *objLinkAcc(*(*obj_itr));
      std::stringstream ss_or; ss_or << overlapObj->type();
      Info("printObj()", "    Overlap: type %s pt %6.2f", (ss_or.str()).c_str(), overlapObj->pt()/1e3);
    }
  }

  return EL::StatusCode::SUCCESS;

}


EL::StatusCode OverlapRemover :: printOverlapInfo (const char* type, xAOD::IParticle* obj, const std::string& selectFlag, const std::string& overlapFlag)
{
  static SG::AuxElement::ConstAccessor<char> selectAcc(selectFlag);
  static SG::AuxElement::ConstAccessor<char> overlapAcc(overlapFlag);
  static SG::AuxElement::ConstAccessor< ElementLink<xAOD::IParticleContainer> > objLinkAcc("overlapObject");

  // Safety check
  if(!overlapAcc.isAvailable(*obj)){
    Error("printObj()", "Overlap decoration missing for object of type %s ", type );
    return EL::StatusCode::FAILURE;
  }
  if(selectAcc.isAvailable(*obj)){
    Info("printObj()", "  %s pt %6.2f eta %5.2f phi %5.2f selected %i overlaps %i \n", type, obj->pt()/1000., obj->eta(), obj->phi(), selectAcc(*obj), overlapAcc(*obj));
  } else {
    Info("printObj()", "  %s pt %6.2f eta %5.2f phi %5.2f overlaps %i \n", type, obj->pt()/1000., obj->eta(), obj->phi(), overlapAcc(*obj));
  }
  // Check for overlap object link
  if(objLinkAcc.isAvailable(*obj) && objLinkAcc(*obj).isValid()){
    const xAOD::IParticle* overlapObj = *objLinkAcc(*obj);
    std::stringstream ss_or; ss_or << overlapObj->type();
    Info("printObj()", "    Overlap: type %s pt %6.2f", (ss_or.str()).c_str(), overlapObj->pt()/1e3);
  }

  return EL::StatusCode::SUCCESS;

}
