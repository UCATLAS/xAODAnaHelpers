#include "xAODAnaHelpers/HelperFunctions.h"

// jet reclustering
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>

// jet trimming
#include <fastjet/tools/Filter.hh>
#include <JetEDM/JetConstituentFiller.h>

// Get Number of Vertices with at least Ntracks
bool HelperFunctions::passPrimaryVertexSelection(const xAOD::VertexContainer* vertexContainer, int Ntracks)
{
  const xAOD::Vertex* primaryVertex = getPrimaryVertex( vertexContainer );
  if(!primaryVertex){ return false; }
  if((int)(primaryVertex)->nTrackParticles() < Ntracks ) {
    return false;
  }
  return true;
}

int HelperFunctions::countPrimaryVertices(const xAOD::VertexContainer* vertexContainer, int Ntracks)
{

  int NPV = 0;

  // Loop over vertices in the container
  for( auto vtx_itr : *vertexContainer )
  {
    if((int)vtx_itr->nTrackParticles() < Ntracks ) { continue; }
    NPV++;
  }

  return NPV;

}

int HelperFunctions::getPrimaryVertexLocation(const xAOD::VertexContainer* vertexContainer)
{
  int location(0);
  for( auto vtx_itr : *vertexContainer )
  {
    if(vtx_itr->vertexType() == xAOD::VxType::VertexType::PriVtx) {
      return location;
    }
    location++;
  }
  return -1;
}

std::string HelperFunctions::replaceString(std::string subject, const std::string& search, const std::string& replace)
{
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
  return subject;
}


std::vector<TLorentzVector> HelperFunctions::jetReclustering(
  const xAOD::JetContainer* jets,
  double radius,
  double fcut,
  fastjet::JetAlgorithm rc_alg
){

  //1. Need to convert the vector of jets to a vector of pseudojets
  //    only need p4() since we're using them as inputs
  std::vector<fastjet::PseudoJet> input_jets;

  for(auto jet : *jets){
    const TLorentzVector jet_p4 = jet->p4();
    input_jets.push_back(
      fastjet::PseudoJet(
        jet_p4.Px()/1000.,
        jet_p4.Py()/1000.,
        jet_p4.Pz()/1000.,
        jet_p4.E ()/1000.
      )
    );
  }

  //2. Build up the new jet definitions using input configurations
  //    - jet algorithm
  //    - radius
  fastjet::JetDefinition jet_def(rc_alg, radius);

  //3. Run the Cluster Sequence on pseudojets with the right jet definition above
  //    cs = clustersequence
  fastjet::ClusterSequence cs(input_jets, jet_def);

  // 4. Grab the reclustered jets, sorted by pt()
  //    rc_jets == reclustered jets
  std::vector<fastjet::PseudoJet> rc_jets = fastjet::sorted_by_pt(cs.inclusive_jets());

  //5.  Apply trimming on PJ.constituents() using fcut
  //    rc_t_jets == reclustered, trimmed jets
  std::vector<TLorentzVector> rc_t_jets;

  for(auto rc_jet : rc_jets){
    TLorentzVector rc_t_jet = TLorentzVector();
    // loop over subjets
    for(auto rc_jet_subjet : rc_jet.constituents()){
      TLorentzVector subjet = TLorentzVector();
      subjet.SetPtEtaPhiE(
        rc_jet_subjet.pt(),
        rc_jet_subjet.eta(),
        rc_jet_subjet.phi(),
        rc_jet_subjet.e()
      );
      if(subjet.Pt() > fcut*rc_jet.pt()) rc_t_jet += subjet;
    }
    rc_t_jets.push_back(rc_t_jet);
  }

  // notes: rc_t_jets is not sorted by pt due to trimming applied
  struct sort_by_pt
  {
      inline bool operator() (const TLorentzVector lhs, const TLorentzVector rhs)
      {
        return (lhs.Pt() > rhs.Pt());
      }
  };
  std::sort(rc_t_jets.begin(), rc_t_jets.end(), sort_by_pt());

  return rc_t_jets;
}

std::vector<TLorentzVector> HelperFunctions::jetTrimming(
  const xAOD::JetContainer* jets,
  double radius,
  double fcut,
  fastjet::JetAlgorithm s_alg
){

  std::vector<TLorentzVector> t_jets;
  for(const auto jet: *jets){
    t_jets.push_back( jetTrimming(jet, radius, fcut, s_alg) );
  }

  // notes: t_jets is not sorted by pt due to trimming applied
  struct sort_by_pt
  {
      inline bool operator() (const TLorentzVector lhs, const TLorentzVector rhs)
      {
        return (lhs.Pt() > rhs.Pt());
      }
  };
  std::sort(t_jets.begin(), t_jets.end(), sort_by_pt());

  return t_jets;
}

TLorentzVector HelperFunctions::jetTrimming(
  const xAOD::Jet* jet,
  double radius,
  double fcut,
  fastjet::JetAlgorithm s_alg
){

  //1. Create the trimmer
  fastjet::Filter trimmer(fastjet::JetDefinition(s_alg, radius), fastjet::SelectorPtFractionMin(fcut));

  //2.  Apply the trimmer to the jet, this requires the JetEDM
  //        convert xAOD::Jet to PseudoJet with constituents
  //        apply trimmer on the PseudoJet
  TLorentzVector t_jet = TLorentzVector();
  std::vector<fastjet::PseudoJet> constit_pseudojets = jet::JetConstituentFiller::constituentPseudoJets(*jet);

  //3. Need to use ClusterSequence to recluster jet again once we found constituents
  fastjet::ClusterSequence cs(constit_pseudojets, fastjet::JetDefinition( (fastjet::JetAlgorithm) jet->getAlgorithmType(), jet->getSizeParameter()));

  fastjet::PseudoJet t_pjet = trimmer(fastjet::join(cs.inclusive_jets()));

  t_jet.SetPtEtaPhiE(
    t_pjet.pt(),
    t_pjet.eta(),
    t_pjet.phi(),
    t_pjet.e()
  );

  return t_jet;

}

const xAOD::Vertex* HelperFunctions::getPrimaryVertex(const xAOD::VertexContainer* vertexContainer)
{
  // vertex types are listed on L328 of
  // https://svnweb.cern.ch/trac/atlasoff/browser/Event/xAOD/xAODTracking/trunk/xAODTracking/TrackingPrimitives.h
  for( auto vtx_itr : *vertexContainer )
  {
    if(vtx_itr->vertexType() != xAOD::VxType::VertexType::PriVtx) { continue; }
    return vtx_itr;
  }

  return 0;
}

bool HelperFunctions::sort_pt(xAOD::IParticle* partA, xAOD::IParticle* partB){
  return partA->pt() > partB->pt();
}

// Get the subset of systematics to consider
// can also retun full set if systName = "All"
std::vector< CP::SystematicSet > HelperFunctions::getListofSystematics( CP::SystematicSet recSysts, 
    std::string systName, 
    float systVal ) {
  std::vector< CP::SystematicSet > systList;
  // loop over recommended systematics
  for( auto syst : recSysts ) {
    Info("getListofSystematics()","  %s", (syst.basename()).c_str());
    if( systName == syst.basename() ) {
      Info("getListofSystematics()","Found match! Adding systematic %s", syst.basename().c_str());
      // continuous systematics - can choose at what sigma to evaluate
      if (syst == CP::SystematicVariation (syst.basename(), CP::SystematicVariation::CONTINUOUS)) {
        systList.push_back(CP::SystematicSet());
        if ( systVal == 0 ) { 
          Error("getListofSystematics()","Setting continuous systematic to 0 is nominal! Please check!");
          RCU_THROW_MSG("Failure");
        }
        systList.back().insert(CP::SystematicVariation (syst.basename(), systVal));
      } 
      // not a continuous system
      else {
        systList.push_back(CP::SystematicSet());
        systList.back().insert(syst);
      }
    } // found match!
    else if ( systName == "All" ) {
      Info("initialize()","Adding systematic %s", syst.basename().c_str());
      // continuous systematics - can choose at what sigma to evaluate
      // add +1 and -1 for when running all
      if (syst == CP::SystematicVariation (syst.basename(), CP::SystematicVariation::CONTINUOUS)) {
        systList.push_back(CP::SystematicSet());
        systList.back().insert(CP::SystematicVariation (syst.basename(),  1.0));
        systList.push_back(CP::SystematicSet());
        systList.back().insert(CP::SystematicVariation (syst.basename(), -1.0));
      } 
      // not a continuous systematic
      else {
        systList.push_back(CP::SystematicSet());
        systList.back().insert(syst);
      }
    } // running all
  } // loop over recommended systematics
  return systList;
}
