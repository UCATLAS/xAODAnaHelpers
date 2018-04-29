#ifndef xAODAnaHelpers_ParticleContainer_H
#define xAODAnaHelpers_ParticleContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/HelperFunctions.h>

#include <xAODAnaHelpers/Particle.h>
#include <xAODBase/IParticle.h>

namespace xAH {

    template <class T_PARTICLE, class T_INFOSWITCH>
    class ParticleContainer
    {
    public:
    ParticleContainer(const std::string& name,
		      const std::string& detailStr="",
		      float units = 1e3,
		      bool mc = false,
		      bool useMass=false,
          bool storeSystSFs = true,
		      const std::string& suffix="")
      : m_name(name),
	m_infoSwitch(detailStr),
	m_mc(mc),
	m_debug(false),
	m_units(units),
  m_storeSystSFs(storeSystSFs),
	m_useMass(useMass),
	m_suffix(suffix)
      {
	m_n = 0;

        // kinematic
        m_pt  =new std::vector<float>();
        m_eta =new std::vector<float>();
        m_phi =new std::vector<float>();
        m_E   =new std::vector<float>();
        m_M   =new std::vector<float>();
      }

      virtual ~ParticleContainer()
      {
        // kinematic
        if(m_infoSwitch.m_kinematic){
	  delete m_pt;
	  delete m_eta;
	  delete m_phi;
	  delete m_E;
	  delete m_M;
	}
      }

      virtual void setTree(TTree *tree)
      {

	std::string                   counterName = "n"+m_name;
	if (!m_suffix.empty())        counterName += "_" + m_suffix;
	if (m_infoSwitch.m_useTheS) { counterName += "s";
	  std::cerr << "WARNING! The useTheS option is depricated in ParticleContainer." << std::endl;
	}

	tree->SetBranchStatus  (counterName.c_str() , 1);
	tree->SetBranchAddress (counterName.c_str() , &m_n);

        if(m_infoSwitch.m_kinematic)
          {
	    // Determine whether mass or energy is saved
	    std::string mname = branchName("m");
	    m_useMass=tree->GetBranch(mname.c_str())!=0;

	    connectBranch<float>(tree,"pt" ,&m_pt);
	    connectBranch<float>(tree,"eta",&m_eta);
	    connectBranch<float>(tree,"phi",&m_phi);
	    if(m_useMass) connectBranch<float>(tree,"m"  ,&m_M);
	    else          connectBranch<float>(tree,"E"  ,&m_E);
          }
      }

      virtual void setBranches(TTree *tree)
      {

	std::string              counterName = "n"+m_name;
	if (!m_suffix.empty()) { counterName += "_" + m_suffix; }

	tree->Branch(counterName.c_str(),    &m_n, (counterName+"/I").c_str());

        if(m_infoSwitch.m_kinematic) {
	  if(m_useMass)  setBranch<float>(tree,"m",                        m_M                );
	  else           setBranch<float>(tree,"E",                        m_E                );
	  setBranch<float>(tree,"pt",                       m_pt               );
	  setBranch<float>(tree,"phi",                      m_phi              );
	  setBranch<float>(tree,"eta",                      m_eta              );
	}
      }

      virtual void clear()
      {
	m_n = 0;

        if(m_infoSwitch.m_kinematic) {
	  if(m_useMass)  m_M->clear();
	  else           m_E->clear();
	  m_pt  ->clear();
	  m_phi ->clear();
	  m_eta ->clear();
	}
      }

      virtual void FillParticle(const xAOD::IParticle* particle)
      {
	m_n++;

	if( m_infoSwitch.m_kinematic ){
	  m_pt  -> push_back ( particle->pt() / m_units );
	  m_eta -> push_back( particle->eta() );
	  m_phi -> push_back( particle->phi() );
	  if(m_useMass) m_M->push_back  ( particle->m() / m_units );
	  else          m_E->push_back  ( particle->e() / m_units );
	}
      }

      void updateEntry()
      {
        m_particles.clear();

        for(int i=0;i<m_n;i++)
          {
	    T_PARTICLE particle;
	    updateParticle(i,particle);
	    m_particles.push_back(particle);
          }
      }

      T_PARTICLE& at_nonConst(uint idx)
	{ return m_particles[idx]; }

      const T_PARTICLE& at(uint idx) const
      { return m_particles[idx]; }

      const T_PARTICLE& operator[](uint idx) const
      { return m_particles[idx]; }

      uint size() const
      { return m_particles.size(); }


    protected:
      std::string branchName(const std::string& varName)
      {
	std::string name = m_name + "_" + varName;
	if (! m_suffix.empty()) { name += "_" + m_suffix; }
	return name;
      }

      template <typename T_BR> void connectBranch(TTree *tree, const std::string& branch, std::vector<T_BR> **variable)
      {
	std::string name = branchName(branch);
	if(*variable) { delete (*variable); (*variable)=0; }
	if(tree->GetBranch(name.c_str()))
	  {
	    (*variable)=new std::vector<T_BR>();
	    tree->SetBranchStatus  (name.c_str()  , 1);
	    tree->SetBranchAddress (name.c_str()  , variable);
	  }
      }

      template<typename T> void setBranch(TTree* tree, std::string varName, std::vector<T>* localVectorPtr){
	std::string name = branchName(varName);
	tree->Branch(name.c_str(),        localVectorPtr);
      }

      template<typename T, typename U, typename V> void safeFill(const V* xAODObj, SG::AuxElement::ConstAccessor<T>& accessor, std::vector<U>* destination, U defaultValue, int units = 1){
	if ( accessor.isAvailable( *xAODObj ) ) {
	  destination->push_back( accessor( *xAODObj ) / units );
	} else {
	  destination->push_back( defaultValue );
	}
      }


      template<typename T, typename U, typename V> void safeVecFill(const V* xAODObj, SG::AuxElement::ConstAccessor<std::vector<T> >& accessor, std::vector<std::vector<U> >* destination, int units = 1){
	destination->push_back( std::vector<U>() );

	if ( accessor.isAvailable( *xAODObj ) ) {
	  for(U itemInVec : accessor(*xAODObj))        destination->back().push_back(itemInVec / units);
	}
	return;
      }

      template<typename T, typename V> void safeSFVecFill(const V* xAODObj, SG::AuxElement::ConstAccessor<std::vector<T> >& accessor, std::vector<std::vector<T> >* destination, const std::vector<T> &defaultValue) {
        if ( accessor.isAvailable( *xAODObj ) ) {
          if ( m_storeSystSFs ) {
            destination->push_back( accessor(*xAODObj) );
          } else {
            destination->push_back( std::vector< float > ({accessor(*xAODObj)[0]}) );
            //std::cout << "NUMBER: " << std::vector< float > ({accessor(*xAODObj)[0]}) << std::endl;
          }
        } else {
          destination->push_back( defaultValue );
        }
      }

      virtual void updateParticle(uint idx, T_PARTICLE& particle)
      {
        if(m_infoSwitch.m_kinematic)
          {
	    if(m_useMass){
	      particle.p4.SetPtEtaPhiM(m_pt ->at(idx),
				       m_eta->at(idx),
				       m_phi->at(idx),
				       m_M  ->at(idx));

	    } else{
	      particle.p4.SetPtEtaPhiE(m_pt ->at(idx),
				       m_eta->at(idx),
				       m_phi->at(idx),
				       m_E  ->at(idx));
	    }
	  }
      }

      std::string m_name;

      std::vector<T_PARTICLE> m_particles;

    public:
      T_INFOSWITCH m_infoSwitch;
      bool m_mc;
      bool m_debug;
      float m_units;
      bool m_storeSystSFs;

      int m_n;


    private:
      bool        m_useMass;
      std::string m_suffix;

      //
      // Vector branches

      // kinematic
      std::vector<float> *m_pt;
      std::vector<float> *m_eta;
      std::vector<float> *m_phi;
      std::vector<float> *m_E;
      std::vector<float> *m_M;
    };

}//xAH
#endif // xAODAnaHelpers_ParticleContainer_H
