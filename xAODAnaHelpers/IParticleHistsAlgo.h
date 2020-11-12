#ifndef xAODAnaHelpers_IParticleHistsAlgo_H
#define xAODAnaHelpers_IParticleHistsAlgo_H

#include <SampleHandler/MetaObject.h>
#include <SampleHandler/MetaFields.h>

// algorithm wrapper
#include <xAODAnaHelpers/Algorithm.h>
#include <xAODAnaHelpers/IParticleHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>

class IParticleHistsAlgo : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  /** input container */
  std::string m_inContainerName = "";
  /** which plots will be turned on */
  std::string m_detailStr = "";
  /** name of algo input container comes from - only if */
  std::string m_inputAlgo = "";
  /** Histogram name prefix when using IParticleHistsAlgo directly */
  std::string m_histPrefix;
  /** Histogram xaxis title when using IParticleHistsAlgo directly */
  std::string m_histTitle;

private:
  std::map< std::string, IParticleHists* > m_plots;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the )
public:
  // Tree *myTree;
  // TH1 *myHist;

  // this is a standard constructor
  IParticleHistsAlgo (const std::string& name, ISvcLocator *pSvcLocator, const std::string& className="IParticleHistsAlgo");

  // these are the functions inherited from Algorithm
  virtual StatusCode setupJob (EL::Job& job);
  virtual StatusCode fileExecute ();
  virtual StatusCode histInitialize ();
  virtual StatusCode changeInput (bool firstFile);
  virtual StatusCode initialize ();
  /**
      @brief Calls execute<IParticleContainer>
   */
  virtual StatusCode execute ();
  virtual StatusCode postExecute ();
  virtual StatusCode finalize ();
  virtual StatusCode histFinalize ();

  /**
      @brief Fill histograms with particles in a container
      @rst
          Templated (container type) function that loops over all systematics (or nominal only)
	  and fills the corresponding histogram objects.

	  The event weight, in case of Monte Carlo samples, is
	   mcEventWeight*crossSection*filterEfficiency*kfactor
	  where the sample-weights are taken from SampleHandler and set to 1 by default.
      @endrst
  */
  template<class HIST_T, class CONT_T> StatusCode execute ()
  {
    static SG::AuxElement::Accessor< float > mcEvtWeightAcc("mcEventWeight");

    const xAOD::EventInfo* eventInfo(nullptr);
    ANA_CHECK( HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, msg()) );

    float eventWeight(1);
    if ( mcEvtWeightAcc.isAvailable( *eventInfo ) ) {
      eventWeight = mcEvtWeightAcc( *eventInfo );
    }
    // if(isMC())
    //   {
    // 	double xs     =wk()->metaData()->castDouble(SH::MetaFields::crossSection    ,1);
    // 	double eff    =wk()->metaData()->castDouble(SH::MetaFields::filterEfficiency,1);
    // 	double kfac   =wk()->metaData()->castDouble(SH::MetaFields::kfactor         ,1);
    // 	eventWeight *= xs * eff * kfac;
    //   }

    // this will hold the collection processed
    const CONT_T* inParticles = 0;

    // if input comes from xAOD, or just running one collection,
    // then get the one collection and be done with it
    if( m_inputAlgo.empty() ) {
      ANA_CHECK( HelperFunctions::retrieve(inParticles, m_inContainerName, m_event, m_store, msg()) );

      // pass the photon collection
      ANA_CHECK( static_cast<HIST_T*>(m_plots[""])->execute( inParticles, eventWeight, eventInfo ));
    }
    else { // get the list of systematics to run over

      // get vector of string giving the names
      std::vector<std::string>* systNames(nullptr);
      ANA_CHECK( HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, msg()) );

      // loop over systematics
      for( auto systName : *systNames ) {
	ANA_CHECK( HelperFunctions::retrieve(inParticles, m_inContainerName+systName, m_event, m_store, msg()) );
	if( m_plots.find( systName ) == m_plots.end() ) { this->AddHists( systName ); }
	ANA_CHECK( static_cast<HIST_T*>(m_plots[systName])->execute( inParticles, eventWeight, eventInfo ));
      }
    }

    return StatusCode::SUCCESS;
  }

  // these are the functions not inherited from Algorithm
  /**
      @brief Calls AddHists<IParticleHists>
      @param name Name of the systematic
   */
  virtual StatusCode AddHists( std::string name);

  /**
      @brief Create histograms
      @param name Name of the systematic

      @rst
          Tempalated (histogram colllection class) function that creates all necessary histogram
	  objects for a given systematic. The class chosen for HIST_T template must inherit from
	  IParticleHists.
      @endrst
  */
  template<class HIST_T> StatusCode AddHists( std::string name ) {
    std::string fullname(m_name);
    fullname += name; // add systematic
    HIST_T* particleHists = new HIST_T( fullname, m_detailStr ); // add systematic
    particleHists->m_debug = msgLvl(MSG::DEBUG);
    ANA_CHECK( particleHists->initialize());
    particleHists->record( wk() );
    m_plots[name] = particleHists;

    return StatusCode::SUCCESS;
  }

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(IParticleHistsAlgo, 1);
  /// @endcond

};

#endif
