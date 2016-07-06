#ifndef xAODAnaHelpers_ParticleContainer_H
#define xAODAnaHelpers_ParticleContainer_H

#include <TTree.h>
#include <TLorentzVector.h>

#include <vector>
#include <string>

#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/Particle.h>

namespace xAH {

    template <class T_PARTICLE, class T_INFOSWITCH>
    class ParticleContainer
    {
    public:
    ParticleContainer(const std::string& name, const std::string& detailStr="", bool useMass=false)
      : m_name(name), m_infoSwitch(detailStr), m_mc(false), m_useMass(useMass)
      {
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
        delete m_pt;
        delete m_eta;
        delete m_phi;
        delete m_E;
        delete m_M;
      }
    
      virtual void setTree(TTree *tree)
      {
        if(m_infoSwitch.m_kinematic)
          {
    	connectBranch<float>(tree,"pt" ,&m_pt);
    	connectBranch<float>(tree,"eta",&m_eta);
    	connectBranch<float>(tree,"phi",&m_phi);
    	if(m_useMass) connectBranch<float>(tree,"m"  ,&m_M);
	else          connectBranch<float>(tree,"E"  ,&m_E);
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
      template <typename T_BR> void connectBranch(TTree *tree, const std::string& branch, std::vector<T_BR> **variable)
      {
        tree->SetBranchStatus  ((m_name+"_"+branch).c_str()  , 1);
        tree->SetBranchAddress ((m_name+"_"+branch).c_str()  , variable);
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
    
      int m_n;
    
    public:
      T_INFOSWITCH m_infoSwitch;
      bool m_mc;
    
    private:
      bool m_useMass;

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
