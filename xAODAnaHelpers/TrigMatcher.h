/**
 * @file TrigMatcher.h
 * @author Karol Krizka <kkrizka@cern.ch>
 * @brief |xAH| algorithm to match reconstructed physics objects to trigger objects using the Trig::MatchingTool.
 */

#ifndef xAODAnaHelpers_TrigMatcher_H
#define xAODAnaHelpers_TrigMatcher_H

// algorithm wrapper
#include <xAODAnaHelpers/Algorithm.h>

#include <AsgTools/AnaToolHandle.h>
#include <TrigDecisionTool/TrigDecisionTool.h>
#include <TriggerMatchingTool/IMatchingTool.h>

#include <TH1D.h>

/**
   @brief A wrapper of the trigger matching tool in the ASG [TriggerMatchingTool](https://twiki.cern.ch/twiki/bin/view/Atlas/XAODMatchingTool) package.
   @rst

      The idea behind this algorithm is to decorate inputs from upstream |xAH| algorithms based on user's configuration, handling also the case where systematics on the input physics objects are taken into account. The list of trigger chains that contain a trigger object matched to a reconstructed object are saved in the :cpp:any:`trigMatched` decoration as :cpp:any:`std::vector<std::string>.

      Here follows a usage example for photon matching.

      Assuming the typical |xAH| analysis configuration through :py:class:`~xAH_config`, the analysis workflow could look like the following::

          c = xAH_config()
          # ...
          c.algorithm("PhotonSelector", PhotonSelectorDict)
          # ...
          c.algorithm("TrigMatcher", TrigMatcherDict)
          # ...

      where each algorithm has the following I/O systematics configuration (via python dictionaries)::

          PhotonSelectorDict = {
              # ...
              "m_inputAlgo" : "PhotonCalibrator_Syst",
	      "m_outContainerName" : "PhotonSelector_Out",
              "m_outputAlgo" : "PhotonSelector_Syst",
              # ...
         }

         TrigMatcherDict = {
              # ...
	      "m_inContainerName" : "PhotonSelector_Out",
	      "m_systNames" : "PhotonSelector_Syst",  # leave empty when not considering systematics
	      "m_trigChains" : "HLT_g120_loose,HLT_g140_loose"
              # ...
         }

      In this way the trigger matching algorithm will be able to correctly work out all the combinatorics, assigning decoration to input objects in each input systematics combination to be subsequently used downstream according to the user's needs.

   @endrst
*/
class TrigMatcher : public xAH::Algorithm
{
public:
  /* configuration variables */

  /** @brief Input container name */
  std::string    m_inContainerName = "";
  /** @brief Input systematics list container name */
  std::string    m_systNames = "";

  /** @brief Comma-separated list of trigger chains
      @rst
        A comma-separated string w/ all the HLT trigger chains for which you want to perform the matching. If left empty (as it is by default), no trigger matching will be attempted at all
      @endrst
  */
  std::string    m_trigChains = "";

private:

  /* tools */
  asg::AnaToolHandle<Trig::TrigDecisionTool> m_trigDecTool_handle  {"Trig::TrigDecisionTool/TrigDecisionTool"             }; //!
  asg::AnaToolHandle<Trig::IMatchingTool>    m_trigMatchTool_handle{"Trig::MatchingTool/MatchingTool"        , this}; //!

  std::vector<std::string> m_trigChainsList; //!  /* contains all the HLT trigger chains tokens extracted from m_trigChains */

public:

  /* this is a standard constructor */

  TrigMatcher ();

  ~TrigMatcher();

  /* these are the functions inherited from Algorithm */

  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();

  /* these are the functions not inherited from Algorithm */
  EL::StatusCode executeMatching( const xAOD::IParticleContainer* inParticles );

  /// @cond
  ClassDef(TrigMatcher, 1);
  /// @endcond

};


#endif //#ifndef xAODAnaHelpers_TrigMatcher_H
