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
  @brief Produce xAOD outputs
  @rst

    .. warning:: Care must be taken when managing memory and using copies. You need to think about how copies point to each other and whether you can use shallow copies or deep copies or both.

    I can think up the following cases when a user is doing an EL Algorithm:

      * input containers in TEvent (simple)
      * deep-copied containers in TStore (deep-copy)
      * shallow-copied containers in TStore (shallow)
      * CDV containers in TStore (cdv)

    For the above use-cases, we might produce outputs like so:

      * write the input container to the output. This uses ``TEvent::copy()``.
      * write the deep-copied containers to the output. This calls ``TStore::retrieve()`` and then ``TEvent::record()``.
      * two options when we have shallow-copies:

        #. ``shallowIO=false``: write to the output as a deep-copy like in the previous option
        #. ``shallowIO=true``: write to the output as a shallow-copy, but make sure the original container is also written to the output

      * make a deep-copy of the ConstDataVector and then move from ``TStore`` to ``TEvent``. The problem is that we point to local memory that will not persist when making the CDV.

    The trickiest case is with shallow copies because those could be our systematics -- and you might want to copy the original container, and only copy over systematics via true shallow copies to conserve memory and space.

  @endrst

 */
class MinixAOD : public xAH::Algorithm
{
public:
  /// @brief name of the output file to use for xAOD dumping
  std::string m_outputFileName = "out_miniXAOD";

  /// @brief enable to create the output file for xAOD dumping
  bool m_createOutputFile = true;

  /// @brief copy the file metadata over
  bool m_copyFileMetaData = false;

  /// @brief copy the trigger containers and meta data over
  bool m_copyTriggerInfo = false;

  /// @brief copy the cutbookkeeper data over
  bool m_copyCutBookkeeper = false;

  /**
    @brief names of containers to copy from the input file

    @rst
      Container names should be space-delimited::

        "m_simpleCopyKeys": "EventInfo AntiKt4EMTopoJets"

    @endrst
   */
  std::string m_simpleCopyKeys = "";

  /**
    @brief names of containers in the TStore to copy over

    @rst

      .. note:: This option is appropriate for deep-copied containers.

      Container names should be space-delimited::

        "m_storeCopyKeys": "BrandNewJetContainer ReclusteredJets"

    @endrst
   */
  std::string m_storeCopyKeys = "";

  /**
    @brief names of containers that have been shallow-copied

    @rst

      .. note:: This option is appropriate for shallow-copied containers.

      This option is a little different because shallow-copied containers have parent containers. However, there are two options depending on the ``setShallowIO`` option

        True
          If this is set to true, you will want to specify the parent container so that we copy it over as well (it is assumed that the parent container is in TStore or TEvent)::

            "m_shallowCopyKeys": "SCAntiKt4EMTopoJets|AntiKt4EMTopoJets SCMuons|Muons_Presel"

        False
          If this is set to false, you will not want to specify the parent container

            "m_shallowCopyKeys": "SCAntiKt4EMTopoJets| SCMuons|"

      .. warning:: Please note that the ``shallowIO`` option is what determines how the memory is managed. If you run into issues with shallow-copied containers here, make sure you know whether this option was enabled or not before asking for help.

      Always specify your string in a space-delimited format where pairs are split up by ``shallow container name|parent container name``.

    @endrst
   */
  std::string m_shallowCopyKeys = "";

  /**
    @brief names of containers that have been shallow-copied

    @rst

      .. note:: This option is appropriate for view-only containers such as ``ConstDataVector``.

      Here, we will do the deep-copying for you, so that the containers can be correctly recorded into the output. Due to the way view-only containers work, we can't figure out whether the memory points to a specific parent container we can copy, or to a non-persistable, local (stack) memory. The best option is to just deep-copy and allocate new memory instead::

          "m_deepCopyKeys": "AntiKt4EMTopoJets|DeepCopyAntiKt4Jets Muons|DeepCopyMuons"

      Always specify your string in a space-delimited format where pairs are split up by ``input container name|output container name``.

    @endrst
   */
  std::string m_deepCopyKeys = "";

  /**
    @brief names of vectors that have container names for its contents

    @rst

      .. note:: This option is appropriate for groups shallow-copied containers such as when you are dealing with systematics.

      Here, we will do the copying for you by retrieving the vector of container names and copy each one over. See how :cpp:member:`MinixAOD::m_shallowCopyKeys` works.

      Always specify your string in a space-delimited format where pairs are split up by ``vector name|parent container name``.
    @endrst
   */
  std::string m_vectorCopyKeys = "";

private:
  /// A vector of containers that are in TEvent that just need to be written to the output
  std::vector<std::string> m_simpleCopyKeys_vec; //!
  /// A vector of (container name, parent name) pairs for shallow-copied objects -- if parent is empty, deep-copy it
  std::vector<std::pair<std::string, std::string>> m_shallowCopyKeys_vec; //!
  /// A vector of (in container, output container) that need to be deep-copied first before moving to TStore
  std::vector<std::pair<std::string, std::string>> m_deepCopyKeys_vec; //!
  /// A vector of (name of vector of container names, parent name) pairs for shallow-copied objects (like systematics) -- if parent is empty, deep-copy it
  std::vector<std::pair<std::string, std::string>> m_vectorCopyKeys_vec; //!

  /// A vector of containers (and aux-pairs) in TStore to record in TEvent
  std::vector<std::string> m_copyFromStoreToEventKeys_vec; //!

  /// Pointer for the File MetaData Tool
  xAODMaker::FileMetaDataTool          *m_fileMetaDataTool = nullptr;    //!
  /// Pointer for the TriggerMenu MetaData Tool
  xAODMaker::TriggerMenuMetaDataTool   *m_trigMetaDataTool = nullptr; //!

  /// Pointer for our CutBookkeeper
  xAOD::CutBookkeeperContainer    *m_outputCBKContainer = nullptr;       //!
  xAOD::CutBookkeeperAuxContainer *m_outputCBKContainer_aux = nullptr;   //!
  xAOD::CutBookkeeperContainer    *m_outputInCBKContainer = nullptr;     //!
  xAOD::CutBookkeeperAuxContainer *m_outputInCBKContainer_aux = nullptr; //!
  xAOD::CutBookkeeper             *m_outputCBK = nullptr;                //!

public:
  // this is a standard constructor
  MinixAOD ();

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
