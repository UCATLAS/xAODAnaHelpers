#ifndef xAODAnaHelpers_MinixAOD_H
#define xAODAnaHelpers_MinixAOD_H

// algorithm wrapper
#include "xAODAnaHelpers/Algorithm.h"

//MetaData
#include <xAODMetaDataCnv/FileMetaDataTool.h>
#include <xAODTriggerCnv/TriggerMenuMetaDataTool.h>

#include <xAODCutFlow/CutBookkeeper.h>
#include <xAODCutFlow/CutBookkeeperContainer.h>
#include <xAODCutFlow/CutBookkeeperAuxContainer.h>

/**
  @brief Information for copying a container
*/
class MinixAODCopyInfo
{
public:
  /// Name of input container
  std::string inputContainerName;

  /// Name of output conainer
  std::string outputContainerName;

  /// Name of the systematics list (for particle copies only)
  std::string systAlgo;

  /// shallowIO mode (for shallw copies only)
  bool shallowIO;

public:
  MinixAODCopyInfo() {}

  /**
     @rst
      Parses a container copy string in the format
       inputContainer*>outputContainer|systAlgo
       
       inputContainer - name of the container to copy
       outputContainer - name for the container in the output file (optional, default is to use the same name)
       systAlgo - name of the systematics list container (m_particleCopy only)
       * - for shallow copy, set shallowIO=false
     @endrst

     @param copyString copy description string
  */
  MinixAODCopyInfo(const std::string& copyString);
};

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

      * make a deep-copy of the ConstDataVector and :code:`TEvent::record()`. The problem is that we point to local memory that will not persist when making the CDV.

    The trickiest case is with shallow copies because those could be our systematics -- and you might want to copy the original container, and only copy over systematics via true shallow copies to conserve memory and space. It is up to the user to handle this correctly.

  @endrst

 */
class MinixAOD : public xAH::Algorithm
{
public:
  /// XRootD path to copy file to (leave empty to save to disk)
  std::string m_outputXRootD;

  /// name of the output file to use for xAOD dumping
  std::string m_outputFileName;

  /// enable to create the output file for xAOD dumping
  bool m_createOutputFile;

  /// copy the file metadata over
  bool m_copyFileMetaData;

  /// copy the trigger containers and meta data over
  bool m_copyTriggerInfo;

  /// copy the cutbookkeeper data over
  bool m_copyCutBookkeeper;

  /**
    @brief names of containers to copy from the input file

    @rst
      Container names should be space-delimited::

        "m_simpleCopyKeys": "EventInfo AntiKt4EMTopoJets"

    @endrst
   */
  std::string m_simpleCopyKeys;

  /**
    @brief names of deep-copy containers in the TStore to copy over

    @rst

      .. note:: This option is appropriate for existing deep-copied containers.

      Container names should be space-delimited. To specify a new name for the output container, use ``>NewContainerName`` syntax.

        "m_storeCopyKeys": "BrandNewJetContainer ReclusteredJets>NewContainerName"

    @endrst
   */
  std::string m_storeCopyKeys;

  /**
    @brief names of containers that have been shallow-copied

    @rst

      .. note:: This option is appropriate for shallow-copied containers.

      Shallow-copied containers have parent containers. However, there are two options depending on the :code:`setShallowIO` option

        True
          If this is set to true, you will want to exclicitely save the parent container

            "m_shallowCopyKeys": "SCAntiKt4EMTopoJets"

        False
          If this is set to false, you do not need to save the parent container. Instead add a * at the end to enable deep-copy.

            "m_shallowCopyKeys": "SCAntiKt4EMTopoJets* SCMuons*>NewContainerName"

      .. warning:: Please note that the :code:`shallowIO` option is what determines how the memory is managed. If you run into issues with shallow-copied containers here, make sure you know whether this option was enabled or not before asking for help.

      Always specify your string in a space-delimited format, adding a * for deep copy operations ``shallow container name``. To rename a container in output, use the ``>NewContainerName`` syntax.

    @endrst
   */
  std::string m_shallowCopyKeys;

  /**
    @brief names of containers that have been shallow-copied

    @rst

      .. note:: This option is appropriate for view-only containers such as :code:`ConstDataVector`.

      Here, we will do the deep-copying for you, so that the containers can be correctly recorded into the output. Due to the way view-only containers work, we can't figure out whether the memory points to a specific parent container we can copy, or to a non-persistable, local (stack) memory. The best option is to just deep-copy and allocate new memory instead::

          "m_deepCopyKeys": "AntiKt4EMTopoJets"

      Always specify your string in a space-delimited format ``input container name``.

    @endrst
   */
  std::string m_deepCopyKeys;

  /**
    @brief names of IParticle containers that should be copied, along with the systematics containers

    @rst

      Here, we will automatically determine the best method to copy a container. The heuristics applied are
       1. If it is a shallow copy, result is saved with ShallowIO=True
       2. If it is an already existing deep copy in TStore, result is copied over
       3. If it is is a ConstDataVector, a new deep-copy is saved.
       4. Rest is directly copied over from TEvent.

      Always specify your string in a space-delimited format, with each container in the syntax ``inputContainer*>outputContainer|systAlgo``, where
       
       inputContainer - name of the container to copy
       outputContainer - name for the container in the output file (optional, default is to use the same name)
       systAlgo - name of the systematics list container
       * - for shallow copy, set shallowIO=false

    @endrst
   */
  std::string m_particleCopyKeys;

private:
  /// A vector of containers that are in TEvent that just need to be written to the output
  std::vector<MinixAODCopyInfo> m_simpleCopyKeys_vec; //!
  /// A vector of (container name, shallowIO) pairs for shallow-copied objects
  std::vector<MinixAODCopyInfo> m_shallowCopyKeys_vec; //!
  /// A vector of containers that need to be deep-copied first before moving to TEvent
  std::vector<MinixAODCopyInfo> m_deepCopyKeys_vec; //!
  /// A vector of containers that can be directly copied from TStore
  std::vector<MinixAODCopyInfo> m_storeCopyKeys_vec; //!
  /// A vector of container for which mode should be determine automatically.
  std::vector<MinixAODCopyInfo> m_particleCopyKeys_vec; //!

  /// Pointer for the File MetaData Tool
  xAODMaker::FileMetaDataTool          *m_fileMetaDataTool;    //!
  /// Pointer for the TriggerMenu MetaData Tool
  xAODMaker::TriggerMenuMetaDataTool   *m_trigMetaDataTool; //!

  /// Pointer for our CutBookkeeper
  xAOD::CutBookkeeperContainer    *m_outputCBKContainer;       //!
  xAOD::CutBookkeeperAuxContainer *m_outputCBKContainer_aux;   //!
  xAOD::CutBookkeeperContainer    *m_outputInCBKContainer;     //!
  xAOD::CutBookkeeperAuxContainer *m_outputInCBKContainer_aux; //!
  xAOD::CutBookkeeper             *m_outputCBK;                //!

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

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(MinixAOD, 1);
  /// @endcond

};

#endif
