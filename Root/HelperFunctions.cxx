#include "xAODAnaHelpers/HelperFunctions.h"
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include "xAODBase/IParticleContainer.h"

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
  Warning("HelperFunctions::getPrimaryVertexLocation()","No primary vertex location was found! Returning -1");
  return -1;
}

bool HelperFunctions::applyPrimaryVertexSelection( const xAOD::JetContainer* jets, const xAOD::VertexContainer* vertices )
{

  if(jets->empty()) { return true; }

  int pvLocation = HelperFunctions::getPrimaryVertexLocation(vertices);
  if ( pvLocation < 0 ) { return false; }
  const xAOD::Vertex* vertex = vertices->at( pvLocation );

  // check if the PV compatible Ghost Matched tracks are already here
  static SG::AuxElement::ConstAccessor< std::vector<ElementLink< xAOD::IParticleContainer > > >ghostTrackPVAcc ("GhostTrackPV");
  if( ghostTrackPVAcc.isAvailable( *(jets->at(0)) ) ) { return true; }

  // get the originals and apply selection
  static SG::AuxElement::ConstAccessor< std::vector<ElementLink< xAOD::IParticleContainer > > >ghostTrack ("GhostTrack");
  for( auto jet_itr : *jets ) {

    if ( !ghostTrack.isAvailable( *jet_itr ) ) { continue; }
    std::vector<ElementLink<xAOD::IParticleContainer> > trackLinks = ghostTrack( *jet_itr );

    // store the selected tracks
    std::vector<ElementLink< xAOD::IParticleContainer > > selectedTrackHolder;

    int originalIndex(-1);
    for ( auto link_itr : trackLinks ) {
      originalIndex++;

      if( !link_itr.isValid() ) { continue; }
      const xAOD::TrackParticle* track = dynamic_cast<const xAOD::TrackParticle*>( *link_itr );

      if( track->pt() < 500 )                     { continue; } // pT cut
      if( track->vertex() != vertex ) {                        // if not in PV vertex fit
        if( track->vertex() != 0 )                { continue; } // make sure in no vertex fits
        if( fabs((track->z0()+track->vz()-vertex->z())*sin(track->theta())) > 3.0 ) { continue; } // make sure close to PV in z
      }

      selectedTrackHolder.push_back( link_itr );

    } // loop over tracks

    jet_itr->auxdecor< std::vector< ElementLink< xAOD::IParticleContainer > > > ("GhostTrackPV") = selectedTrackHolder;

  } // loop over jets


  return true;
}

// compatible with starting with: 2015-PreRecomm-13TeV-MC12-CDI_August3-v1.root
//https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#MV2c20_tagger_AntiKt4EMTopoJets
float HelperFunctions::GetBTagMV2c20_Cut( int efficiency ) {
  if     ( efficiency == 85 ) { return -0.7887; }
  else if( efficiency == 77 ) { return -0.4434; }
  else if( efficiency == 70 ) { return -0.0436; }
  else if( efficiency == 60 ) { return  0.4496; }
  else { std::cout << "WARNING!! UNKNOWN BTAG EFFICIENCY POINT " << efficiency << std::endl; }
  return -1; // no cut
}

std::string HelperFunctions::GetBTagMV2c20_CutStr( int efficiency ) {
  float value = HelperFunctions::GetBTagMV2c20_Cut( efficiency );
  std::string valueStr = std::to_string(value);
  valueStr.replace(valueStr.find('.'),1,"_"); // replace period with underscore
  // 7 characters long if start with a - and 6 otherwise
  if( valueStr.find('-') != std::string::npos ) {
    valueStr.resize(7,'0'); // cut to 7 or pad with 0s
  } else {
    valueStr.resize(6,'0'); // cut to 6 or pad with 0s
  }
  return valueStr;
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

std::vector<TString> HelperFunctions::SplitString(TString& orig, const char separator)
{
    // 'splitV' with the primitive strings
    std::vector<TString> splitV;
    TString splitOpt(orig);
    splitOpt.ReplaceAll("\n"," ");
    splitOpt = splitOpt.Strip(TString::kBoth,separator);

    while (splitOpt.Length()>0) {
      if ( !splitOpt.Contains(separator) ) {
        splitOpt.ReplaceAll(" ",""); // clear empty spaces
        splitV.push_back(splitOpt);
        break;
      }
      else {
        TString toSave = splitOpt(0,splitOpt.First(separator));
        splitV.push_back(toSave);
        splitOpt = splitOpt(splitOpt.First(separator),splitOpt.Length());
      }
      splitOpt = splitOpt.Strip(TString::kLeading,separator);
    }

    return splitV;
}

StatusCode HelperFunctions::isAvailableMetaData(TTree* metaData){
    if ( !metaData ) {
      Info("HelperFunctions::isAvailableMetaData()", "MetaData tree missing from input file. Aborting ");
      return StatusCode::FAILURE;
    }
    return StatusCode::SUCCESS;
}

bool HelperFunctions::isFilePrimaryxAOD(TFile* inputFile) {
    TTree* metaData = dynamic_cast<TTree*> (inputFile->Get("MetaData"));

    /* check that MetaData tree exists */
    RETURN_CHECK("HelperFunctions::isFilePrimaryxAOD", isAvailableMetaData(metaData), "" );

    metaData->LoadTree(0);
    TObjArray* ar = metaData->GetListOfBranches();
    for (int i = 0; i < ar->GetEntries(); ++i) {
      TBranch* b = (TBranch*) ar->At(i);
      std::string name = std::string(b->GetName());
      if (name == "StreamAOD")
        return true;
    }

    return false;
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

const xAOD::Vertex* HelperFunctions::getPrimaryVertex(const xAOD::VertexContainer* vertexContainer, bool quiet)
{
  // vertex types are listed on L328 of
  // https://svnweb.cern.ch/trac/atlasoff/browser/Event/xAOD/xAODTracking/trunk/xAODTracking/TrackingPrimitives.h
  for( auto vtx_itr : *vertexContainer )
  {
    if(vtx_itr->vertexType() != xAOD::VxType::VertexType::PriVtx) { continue; }
    return vtx_itr;
  }

  if(!quiet)
    Warning("HelperFunctions::getPrimaryVertex()","No primary vertex was found! Returning nullptr");

  return 0;
}

float HelperFunctions::getPrimaryVertexZ(const xAOD::Vertex* pvx)
{
  float pvx_z = 0;
  if(pvx) pvx_z = pvx->z();
  return pvx_z;
}


bool HelperFunctions::sort_pt(xAOD::IParticle* partA, xAOD::IParticle* partB){
  return partA->pt() > partB->pt();
}

// Get the subset of systematics to consider
// can also return full set if systName = "All"
//
// CP::make_systematics_vector(recSysts); has some similar functionality but does not
// prune down to 1 systematic if only request that one.  It does however include the
// nominal case as a null SystematicSet
std::vector< CP::SystematicSet > HelperFunctions::getListofSystematics(const CP::SystematicSet inSysts, std::string systName, float systVal, bool debug ) {

  std::vector< CP::SystematicSet > outSystList;

  if ( debug ) { Info("HelperFunctions::getListofSystematics()","systName %s", (systName).c_str()); }

  // loop over input set
  //
  for ( const auto syst : inSysts ) {

    if ( debug ) { Info("HelperFunctions::getListofSystematics()","  %s", (syst.name()).c_str()); }

    // 1.
    // A match with input systName is found in the list:
    // add that systematic only to the output list
    //
    if ( systName == syst.basename() ) {

      if ( debug ) { Info("HelperFunctions::getListofSystematics()","Found match! Adding systematic %s", syst.name().c_str()); }

      // continuous systematics - can choose at what sigma to evaluate
      //
      if ( syst == CP::SystematicVariation (syst.basename(), CP::SystematicVariation::CONTINUOUS) ) {

        outSystList.push_back(CP::SystematicSet());

        if ( systVal == 0 ) {
          Error("HelperFunctions::getListofSystematics()","Setting continuous systematic to 0 is nominal! Please check!");
          RCU_THROW_MSG("Failure");
        }

        outSystList.back().insert(CP::SystematicVariation (syst.basename(), systVal));

      } else {
      // not a continuous system

        outSystList.push_back(CP::SystematicSet());
        outSystList.back().insert(syst);

      }
    }
    // 2.
    // input systName contains "All":
    // add all systematics to the output list
    //
    else if ( systName.find("All") != std::string::npos ) {

      if ( debug ) { Info("HelperFunctions::getListofSystematics()","Adding systematic %s", syst.name().c_str()); }

      // continuous systematics - can choose at what sigma to evaluate
      // add +1 and -1 for when running all
      //
      if ( syst == CP::SystematicVariation (syst.basename(), CP::SystematicVariation::CONTINUOUS) ) {

      if ( systVal == 0 ) {
        Error("HelperFunctions::getListofSystematics()","Setting continuous systematic to 0 is nominal! Please check!");
        RCU_THROW_MSG("Failure");
      }

      outSystList.push_back(CP::SystematicSet());
      outSystList.back().insert(CP::SystematicVariation (syst.basename(),  fabs(systVal)));
      outSystList.push_back(CP::SystematicSet());
      outSystList.back().insert(CP::SystematicVariation (syst.basename(), -1.0*fabs(systVal)));

      } else {
      // not a continuous systematic

        outSystList.push_back(CP::SystematicSet());
        outSystList.back().insert(syst);

      }

    }

  } // loop over recommended systematics

  // Add an empty CP::SystematicVariation at the top of output list to account for the nominal case
  // when running on all systematics or on nominal only
  //
  if ( systName.find("Nominal") != std::string::npos || systName.find("All") != std::string::npos || systName.empty() ) {
    outSystList.insert( outSystList.begin(), CP::SystematicSet() );
    const CP::SystematicVariation nullVar = CP::SystematicVariation("");
    outSystList.back().insert(nullVar);
  }

  return outSystList;

}


float HelperFunctions::dPhi(float phi1, float phi2)
{
  float dPhi = phi1 - phi2;
  if(dPhi > 3.14)  dPhi -= 2*3.14;
  if(dPhi < -3.14) dPhi += 2*3.14;
  return dPhi;
}


std::size_t HelperFunctions::string_pos( const std::string& inputstr, const char& occurence, int n_occurencies )
{

  std::string read("");
  std::string cache(inputstr);

  for ( int i(1); i < n_occurencies+1; ++i ) {
    std::size_t found = cache.rfind(occurence);
    read += cache.substr(found);
    cache.erase(found);
    if ( i == n_occurencies ) { return ( inputstr.size() - read.size() ) + 1; }
  }
  return std::string::npos;
}


std::string HelperFunctions::parse_wp( const std::string& type, const std::string& config_name )
{

  std::string wp("");
  
  std::size_t init;
  std::size_t end;
  
  if ( type.compare("ISO") == 0 ) {
    
    std::size_t found_iso = config_name.find("_isol");
    
    // Return empty string if no isolation in config name
    
    if ( found_iso == std::string::npos ) { return wp; }
    
    init = found_iso + 5; 
    end  = config_name.find(".root");
  
  } else if ( type.compare("ID") == 0 ) {
 
    std::size_t found_ID = config_name.find("LLH");
    
    // Return empty string if no LLH in config name
    
    if ( found_ID == std::string::npos ) { return wp; }
       
    init = string_pos( config_name, '.', 2 );
    end  = found_ID;
    
  } else if ( type.compare("TRIG") == 0 ) {
 
    std::size_t found_trigger = config_name.find("trigger");
    
    // Return empty string if no LLH in config name
    
    if ( found_trigger == std::string::npos ) { return wp; }
       
    init = string_pos( config_name, '.', 3 );
    end  = string_pos( config_name, '.', 2 ) - 1;
    
  } else {
  
    Warning("HelperFunctions::parse_wp()","WP type can be either 'ISO' or 'ID'. Please check passed parameters of this function. Returning empty WP.");
    return wp;
  
  }
  
  wp = config_name.substr( init, (end - init) );
  
  if ( type.compare("ID") == 0 ) { wp += "LLH"; }

  return wp;
}

bool HelperFunctions::has_exact(const std::string input, const std::string flag) 
{ 
  std::set<std::string> inputSet;
  
  // parse and split by space
  std::string token;
  std::istringstream ss(input);
  while ( std::getline(ss, token, ' ') )
    inputSet.insert(token);


  return inputSet.find(flag) != inputSet.end(); 
}
