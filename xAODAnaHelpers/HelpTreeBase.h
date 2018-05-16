/********************************************************
 * HelpTreeBase:
 *
 * This class is meant to help the user write out a tree.
 * Some branches are included by default while others
 * need to be added by the user
 *
 * John Alison (john.alison@cern.ch)
 * Gabriel Facini (gabriel.facini@cern.ch)
 * Marco Milesi (marco.milesi@cern.ch)
 * Jeff Dandoy (jeff.dandoy@cern.ch)
 *
 ********************************************************/

// Dear emacs, this is -*-c++-*-
#ifndef xAODAnaHelpers_HelpTreeBase_H
#define xAODAnaHelpers_HelpTreeBase_H


#include "xAODEventInfo/EventInfo.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODCaloEvent/CaloClusterContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODTrigger/JetRoIContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTau/TauJetContainer.h"
#include "xAODMissingET/MissingETContainer.h"
#include "xAODTracking/TrackParticleContainer.h"

#include "xAODAnaHelpers/HelperClasses.h"
#include "xAODAnaHelpers/EventInfo.h"
#include "xAODAnaHelpers/MetContainer.h"
#include "xAODAnaHelpers/JetContainer.h"
#include "xAODAnaHelpers/ElectronContainer.h"
#include "xAODAnaHelpers/PhotonContainer.h"
#include "xAODAnaHelpers/ClusterContainer.h"
#include "xAODAnaHelpers/FatJetContainer.h"
#include "xAODAnaHelpers/TruthContainer.h"
#include "xAODAnaHelpers/TrackContainer.h"
#include "xAODAnaHelpers/MuonContainer.h"
#include "xAODAnaHelpers/TauContainer.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"


#include <map>

// root includes
#include "TTree.h"
#include "TFile.h"

namespace TrigConf {
  class xAODConfigTool;
}

namespace Trig {
  class TrigDecisionTool;
}

typedef SG::AuxElement::Accessor< std::vector< float > > floatAccessor ;

class HelpTreeBase {

public:

  HelpTreeBase(xAOD::TEvent *event, TTree* tree, TFile* file, const float units = 1e3, bool debug = false, xAOD::TStore* store = nullptr );
  HelpTreeBase(TTree* tree, TFile* file, xAOD::TEvent *event = nullptr, xAOD::TStore* store = nullptr, const float units = 1e3, bool debug = false );
  virtual ~HelpTreeBase();

  void AddEvent       (const std::string detailStr = "");
  void AddTrigger     (const std::string detailStr = "");
  void AddJetTrigger  (const std::string detailStr = "");
  void AddMuons       (const std::string detailStr = "", const std::string muonName = "muon");
  void AddElectrons   (const std::string detailStr = "", const std::string elecName = "el");
  void AddPhotons     (const std::string detailStr = "", const std::string photonName = "ph");
  void AddClusters    (const std::string detailStr = "", const std::string clusterName = "cl");
  void AddJets        (const std::string detailStr = "", const std::string jetName = "jet");
  void AddL1Jets      ();
  void AddTruthParts  (const std::string truthName,      const std::string detailStr = "");
  void AddTrackParts  (const std::string trackName,	 const std::string detailStr = "");

  /**
   *  @brief  Declare a new collection of fatjets to be written to the output tree.
   *  @param  detailStr   A (space-separated) list of detail options. These keywords specify
   *                      exactly which information about each jet is written out. Current
   *                      influential options are: `kinematic` `substructure` `constituent`
   *                      `constituentAll`
   *  @param  fatjetName  The (prefix) name of the container. Default: `fatjet`.
   *  @param  subjetDetailStr List of detail options to pass to the subjet container. See :cpp:member:`HelpTreeBase::AddJets` for list of supported values.
   **/
  void AddFatJets     (const std::string detailStr = "", const std::string fatjetName = "fatjet", const std::string subjetDetailStr="", const std::string suffix="");
  void AddTruthFatJets(const std::string detailStr = "", const std::string truthFatJetName = "truth_fatjet");

  void AddTaus        (const std::string detailStr = "", const std::string tauName = "tau");
  void AddMET         (const std::string detailStr = "", const std::string metName = "met");

  /**
   *  @brief  Helper function to lookup each fatjet container name/suffix combo in the internal map
   *          of vectors for vectors. You probably don't need this but it might be useful if you're
   *          implementing `[Add/Fill/Clear]FatJetsUser()`.
   *  @param  fatjetName  The (prefix) name of the container.
   *  @param  suffix      The container branch suffix.
   *
   *  @return a string that uniquely identifies the collection name/suffix in the lookup map.
   **/
  static std::string FatJetCollectionName(const std::string fatjetName = "fatjet", const std::string suffix = "");

  xAOD::TEvent* m_event;
  xAOD::TStore* m_store;

  // control which branches are filled
  HelperClasses::TriggerInfoSwitch*    m_trigInfoSwitch;

  std::string                  m_triggerSelection;
  TrigConf::xAODConfigTool*    m_trigConfTool;
  Trig::TrigDecisionTool*      m_trigDecTool;

  void FillEvent( const xAOD::EventInfo* eventInfo, xAOD::TEvent* event = nullptr );

  void FillTrigger( const xAOD::EventInfo* eventInfo );
  void FillJetTrigger();

  void FillMuons( const xAOD::MuonContainer* muons, const xAOD::Vertex* primaryVertex, const std::string muonName = "muon" );
  void FillMuon( const xAOD::Muon* muon, const xAOD::Vertex* primaryVertex, const std::string muonName = "muon" );

  void FillElectrons( const xAOD::ElectronContainer* electrons, const xAOD::Vertex* primaryVertex, const std::string elecName = "el" );
  void FillElectron ( const xAOD::Electron* elec, const xAOD::Vertex* primaryVertex, const std::string elecName = "el" );

  void FillPhotons( const xAOD::PhotonContainer* photons, const std::string photonName = "ph" );
  void FillPhoton ( const xAOD::Photon*          photon,  const std::string photonName = "ph" );

  void FillClusters( const xAOD::CaloClusterContainer* clusters, const std::string clusterName = "cl" );
  void FillCluster ( const xAOD::CaloCluster*          cluster,  const std::string clusterName = "cl" );

  void FillJets( const xAOD::JetContainer* jets, int pvLocation = -1, const std::string jetName = "jet" );
  void FillJet( const xAOD::Jet* jet_itr, const xAOD::Vertex* pv, int pvLocation, const std::string jetName = "jet" );
  void FillL1Jets( const xAOD::JetRoIContainer* jets, bool sortL1Jets = false );

  void FillTruth( const std::string truthName, const xAOD::TruthParticleContainer* truth);
  void FillTruth( const xAOD::TruthParticle* truthPart, const std::string truthName );

  void FillTracks( const std::string trackName, const xAOD::TrackParticleContainer* tracks);
  void FillTrack( const xAOD::TrackParticle* trackPart, const std::string trackName );

  /**
   *  @brief  Write a container of jets to the specified container name (and optionally suffix). The
   *          container name and suffix should be declared beforehand using `AddFatJets()`.
   *          This clears the current branch state for the collection so it only makes sense to
   *          call once per call to `Fill()`.
   *  @param  fatJets     A container of jets to be written out.
   *  @param  fatjetName  The name of the output collection to write to.
   *  @param  suffix      The suffix of the output collection to write to.
   */
  void FillFatJets( const xAOD::JetContainer* fatJets , const std::string fatjetName = "fatjet", const std::string suffix = "");
  void FillFatJet ( const xAOD::Jet* fatjet_itr,        const std::string fatjetName = "fatjet", const std::string suffix = "");

  void FillTruthFatJets( const xAOD::JetContainer* truthFatJets,     const std::string truthFatJetName="truth_fatjet" );
  void FillTruthFatJet ( const xAOD::Jet*          truth_fatjet_itr, const std::string truthFatJetName="truth_fatjet" );

  void FillTaus( const xAOD::TauJetContainer* taus, const std::string tauName = "tau" );
  void FillTau ( const xAOD::TauJet* tau,           const std::string tauName = "tau" );
  void FillMET( const xAOD::MissingETContainer* met, const std::string metName = "met" );

  void Fill();
  void ClearEvent();
  void ClearTrigger();
  void ClearJetTrigger();
  void ClearMuons       (const std::string jetName = "muon");
  void ClearElectrons   (const std::string elecName = "el");
  void ClearPhotons     (const std::string photonName = "ph");
  void ClearClusters    (const std::string clusterName = "cl");
  void ClearJets        (const std::string jetName = "jet");
  void ClearL1Jets      ();
  void ClearTruth       (const std::string truthName);
  void ClearTracks	(const std::string trackName);
  void ClearFatJets     (const std::string fatjetName, const std::string suffix="");
  void ClearTruthFatJets(const std::string truthFatJetName = "truth_fatjet");
  void ClearTaus        (const std::string tauName = "tau" );
  void ClearMET         (const std::string metName = "met");

  bool writeTo( TFile *file );

  virtual void AddEventUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddEventUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddTriggerUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddTriggerUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddJetTriggerUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddJetTriggerUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddMuonsUser(const std::string detailStr = "")      {
    if(m_debug) Info("AddMuonsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddElectronsUser(const std::string detailStr = "")  {
    if(m_debug) Info("AddElectronsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddPhotonsUser(const std::string detailStr = "")  {
    if(m_debug) Info("AddPhotonsUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddClustersUser(const std::string detailStr = "")  {
    if(m_debug) Info("AddClustersUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddJetsUser(const std::string detailStr = "", const std::string jetName = "jet")       {
    if(m_debug) Info("AddJetsUser","Empty function called from HelpTreeBase %s %s",detailStr.c_str(), jetName.c_str());
    return;
  };

  virtual void AddTruthUser(const std::string truthName, const std::string detailStr = "")       {
    if(m_debug) Info("AddTruthUser","Empty function called from HelpTreeBase %s %s",truthName.c_str(), detailStr.c_str());
    return;
  };

  virtual void AddTracksUser(const std::string trackName, const std::string detailStr = "")       {
    if(m_debug) Info("AddTracksUser","Empty function called from HelpTreeBase %s %s",trackName.c_str(), detailStr.c_str());
    return;
  };

  /**
   *  @brief  Declare a new fat jet collection. Automatically called once per call to `AddFatJets()`;
   *          override this if you want to provide your own additional branches for fatjets.
   *  @param  detailStr   The space-separated list of detail requested by the called.
   *  @param  fatjetName  The (prefix) name of the output collection.
   *  @param  suffix      A suffix to be appeneded to the end of the output branch name(s).
   */
  virtual void AddFatJetsUser(const std::string detailStr = "", const std::string fatjetName = "", const std::string suffix = "")       {
    if(m_debug) Info("AddFatJetsUser","Empty function called from HelpTreeBase %s for %s with suffix %s", detailStr.c_str(), fatjetName.c_str(), suffix.c_str());
    return;
  };

  virtual void AddTruthFatJetsUser(const std::string detailStr = "", const std::string truthFatJetName = "truth_fatjet")       {
    if(m_debug) Info("AddTruthFatJetsUser","Empty function called from HelpTreeBase %s for %s", detailStr.c_str(), truthFatJetName.c_str());
    return;
  };

  virtual void AddTausUser(const std::string detailStr = "")       {
    if(m_debug) Info("AddTausUser","Empty function called from HelpTreeBase %s",detailStr.c_str());
    return;
  };

  virtual void AddMETUser(const std::string detailStr = "", const std::string metName = "met")       {
    if(m_debug) Info("AddMETUser","Empty function called from HelpTreeBase %s for %s",detailStr.c_str(), metName.c_str());
    return;
  };

  virtual void ClearEventUser       ()     { return; };
  virtual void ClearTriggerUser     ()   { return; };
  virtual void ClearMuonsUser       (const std::string /*muonName = muon"*/)     { return; };
  virtual void ClearElectronsUser   (const std::string /*elecName = "el"*/) { return; };
  virtual void ClearPhotonsUser     (const std::string /*photonName = "ph"*/) { return; };
  virtual void ClearClustersUser    (const std::string /*clusterName = "cl"*/) { return; };
  virtual void ClearTruthUser       (const std::string /*truthName*/) 	    { return; };
  virtual void ClearTracksUser      (const std::string /*trackName*/)       { return; };
  virtual void ClearJetsUser        (const std::string /*jetName = "jet"*/ ) 	    { return; };
  virtual void ClearFatJetsUser     (const std::string /*fatjetName = "fatjet"*/, const std::string /*suffix = ""*/)   { return; };
  virtual void ClearTruthFatJetsUser(const std::string /*truthFatJetName = "truth_fatjet"*/)   { return; };
  virtual void ClearTausUser        (const std::string /*tauName = "tau"*/) 	    { return; };
  virtual void ClearMETUser         (const std::string /*metName = "met"*/)       { return; };

  virtual void FillEventUser    ( const xAOD::EventInfo*  )        { return; };
  virtual void FillMuonsUser    ( const xAOD::Muon*,        const std::string /*muonName = "muon"*/  )             { return; };
  virtual void FillElectronsUser( const xAOD::Electron*,    const std::string /*elecName = "el"*/ )     { return; };
  virtual void FillPhotonsUser  ( const xAOD::Photon*,      const std::string /*photonName = "ph"*/ )     { return; };
  virtual void FillClustersUser ( const xAOD::CaloCluster*, const std::string /*clusterName = "cl"*/ )     { return; };
  virtual void FillJetsUser     ( const xAOD::Jet*,         const std::string /*jetName = "jet"*/  )               { return; };
  virtual void FillTruthUser    ( const std::string /*truthName*/, const xAOD::TruthParticle*  )               { return; };
  virtual void FillTracksUser   ( const std::string /*trackName*/, const xAOD::TrackParticle*  )               { return; };
  /**
   *  @brief  Called once per call to `FillFatJets()`.Ooverride this if you want to any additional
   *          information to your jet collection.
   *  @param  jet         a pointer to the current xAOD::Jet object that should be written to the
   *                      output branch(s).
   *  @param  fatjetName  the (prefix) name of the output collection
   *  @param  suffix      the suffix to append to output branches.
   */
  virtual void FillFatJetsUser( const xAOD::Jet* /*jet*/, const std::string /*fatjetName = "fatjet"*/, const std::string /*suffix = ""*/) { return; };
  virtual void FillTruthFatJetsUser( const xAOD::Jet* /*jet*/, const std::string /*fatjetName = "truth_fatjet"*/   )            { return; };
  virtual void FillTausUser( const xAOD::TauJet*,           const std::string /*tauName = "tau"*/  )            { return; };
  virtual void FillMETUser( const xAOD::MissingETContainer*, const std::string /*metName = "met"*/ ) { return; };
  virtual void FillTriggerUser( const xAOD::EventInfo*  )      { return; };
  virtual void FillJetTriggerUser()                            { return; };

 protected:

  template<typename T, typename U, typename V>
    void safeFill(const V* xAODObj, SG::AuxElement::ConstAccessor<T>& accessor, std::vector<U>& destination, U defaultValue, int m_units = 1);

  template<typename T, typename U, typename V>
    void safeVecFill(const V* xAODObj, SG::AuxElement::ConstAccessor<std::vector<T> >& accessor, std::vector<std::vector<U> >& destination, int m_units = 1);

  template<typename T>
    void setBranch(std::string prefix, std::string varName, std::vector<T>* localVectorPtr);

protected:

  TTree* m_tree;

  int m_units; //For MeV to GeV conversion in output

  bool m_debug;
  bool m_isMC;
  bool m_nominalTree;

  // event
  xAH::EventInfo*      m_eventInfo;

  // trigger
  int m_passL1;
  int m_passHLT;
  unsigned int m_masterKey;
  unsigned int m_L1PSKey;
  unsigned int m_HLTPSKey;
  std::vector<std::string>  m_elTrigForMatching;   /* each event can have a list of electron trigger chains to which each electron could be matched.
  						   / This list is created when configuring ElectronSelector.cxx, where the electron trigger matching is actually performed
						   */

  // jet trigger
  std::vector<std::string> m_passedTriggers;
  std::vector<std::string> m_disabledTriggers;
  std::vector<float> m_triggerPrescales;
  std::vector<float> m_triggerPrescalesLumi;
  std::vector<std::string>  m_isPassBitsNames;
  std::vector<unsigned int> m_isPassBits;

  //
  //  Jets
  //
  std::map<std::string, xAH::JetContainer*> m_jets;

  //
  // L1 Jets
  //
  int m_nL1Jet;
  std::vector<float> m_l1Jet_et8x8;
  std::vector<float> m_l1Jet_eta;
  std::vector<float> m_l1Jet_phi;

  //
  // Truth
  //
  std::map<std::string, xAH::TruthContainer*> m_truth;

  //
  // Tracks
  //
  std::map<std::string, xAH::TrackContainer*> m_tracks;

  //
  // fat jets
  //
  std::map<std::string, xAH::FatJetContainer*> m_fatjets;

  //
  // truth fat jets
  //
  std::map<std::string, xAH::FatJetContainer*> m_truth_fatjets;

  //
  // muons
  //
  std::map<std::string, xAH::MuonContainer*> m_muons;
  std::map<std::string, std::vector<std::string> > m_MuonRecoEff_SF_sysNames;
  std::map<std::string, std::vector<std::string> > m_MuonIsoEff_SF_sysNames;
  std::map<std::string, std::vector<std::string> > m_MuonTrigEff_SF_sysNames;
  std::vector<std::string>  m_MuonTTVAEff_SF_sysNames;

  //
  // electrons
  //
  std::map<std::string, xAH::ElectronContainer*> m_elecs;

  //
  // photons
  //
  std::map<std::string, xAH::PhotonContainer*> m_photons;

  //
  // clusters
  //
  std::map<std::string, xAH::ClusterContainer*> m_clusters;


  //
  // taus
  //
  std::map<std::string, xAH::TauContainer*> m_taus;

  //
  // met
  //
  std::map<std::string, xAH::MetContainer* > m_met;

};


template<typename T, typename U, typename V>
void HelpTreeBase::safeFill(const V* xAODObj, SG::AuxElement::ConstAccessor<T>& accessor, std::vector<U>& destination, U defaultValue, int m_units){
  if ( accessor.isAvailable( *xAODObj ) ) {
    destination.push_back( accessor( *xAODObj ) / m_units );
  } else {
    destination.push_back( defaultValue );
  }
  return;
}

template<typename T, typename U, typename V>
void HelpTreeBase::safeVecFill(const V* xAODObj, SG::AuxElement::ConstAccessor<std::vector<T> >& accessor, std::vector<std::vector<U> >& destination, int m_units){
  destination.push_back( std::vector<U>() );

  if ( accessor.isAvailable( *xAODObj ) ) {
    for(U itemInVec : accessor(*xAODObj))        destination.back().push_back(itemInVec / m_units);
  }
  return;
}


template<typename T>
void HelpTreeBase::setBranch(std::string prefix, std::string varName, std::vector<T>* localVectorPtr){
  m_tree->Branch((prefix+"_"+varName).c_str(),        localVectorPtr);
}


#endif
