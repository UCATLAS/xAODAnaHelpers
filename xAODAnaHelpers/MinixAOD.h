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
  xAODMaker::TriggerMenuMetaDataTool   *m_triggerMetaDataTool; //!

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
