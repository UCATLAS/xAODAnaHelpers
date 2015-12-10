#ifndef xAODAnaHelpers_MinixAOD_H
#define xAODAnaHelpers_MinixAOD_H

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

//MetaData
#include "xAODMetaDataCnv/FileMetaDataTool.h"
#include "xAODTriggerCnv/TriggerMenuMetaDataTool.h"

// EDM includes
#include "xAODBase/IParticleContainer.h"
#include "xAODCore/AuxContainerBase.h"
/**
  @brief Produce xAOD outputs
  @rst

    .. warning:: Care must be taken when managing memory and using copies. You need to think about how copies point to each other and whether you can use shallow copies or deep copies or both.

    I can think up the following cases when a user is doing an EL Algorithm:

      * input containers in TEvent (simple)
      * deep-copied containers in TStore (deep-copy)
      * shallow-copied containers in TStore (shallow)
      * CDV containers in TStore (cdv)

    For the above use-cases, we might produce outputs like so:

      * write the input container to the output. This uses :code:`TEvent::copy()`.
      * write the deep-copied containers to the output. This calls :code:`TStore::retrieve()` and then :code:`TEvent::record()`.
      * two options when we have shallow-copies:

        #. :code:`shallowIO=false`: write to the output as a deep-copy like in the previous option
        #. :code:`shallowIO=true`: write to the output as a shallow-copy, but make sure the original container is also written to the output

      * make a deep-copy of the ConstDataVector and then move from :code:`TStore` to :code:`TEvent`. The problem is that we point to local memory that will not persist when making the CDV.

    The trickiest case is with shallow copies because those could be our systematics -- and you might want to copy the original container, and only copy over systematics via true shallow copies to conserve memory and space.

  @endrst

 */
class MinixAOD : public xAH::Algorithm
{
public:
  /// name of the output file to use for xAOD dumping
  std::string m_outputFileName; //!

  /// enable to create the output file for xAOD dumping
  bool m_createOutputFile;      //!

  /// copy the file metadata over
  bool m_copyFileMetaData; //!

  /// copy the trigger containers and meta data over
  bool m_copyTriggerInfo; //!

  /// names of containers to copy from input
  std::string m_simpleCopyKeys; //!

  /// names of containers to shallow copy
  std::string m_shallowCopyKeys; //!

  /// names of containers to deep copy
  std::string m_deepCopyKeys; //!

private:
  std::vector<std::string> m_simpleCopyKeys_vec;      //!
  std::vector<std::string> m_shallowCopyKeys_vec;     //!
  std::vector<std::string> m_deepCopyKeys_vec;        //!

  xAODMaker::FileMetaDataTool          *m_fileMetaDataTool;    //!
  xAODMaker::TriggerMenuMetaDataTool   *m_trigMetaDataTool; //!

public:
  // this is a standard constructor
  MinixAOD (std::string className = "MinixAOD");

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // these are the functions not inherited from Algorithm
  virtual EL::StatusCode configure ();

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MinixAOD, 1);
  /// @endcond

};

#endif
