#ifndef xAODAnaHelpers_PhotonHists_H
#define xAODAnaHelpers_PhotonHists_H

#include "xAODAnaHelpers/IParticleHists.h"
#include <xAODEgamma/PhotonContainer.h>

#include <AthContainers/DataVector.h>

class PhotonHists : public IParticleHists
{
  public:


    PhotonHists(std::string name, std::string detailStr);
    virtual ~PhotonHists() ;

    virtual StatusCode initialize();
    virtual StatusCode execute( const xAOD::Photon* photon, float eventWeight);
    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using IParticleHists::execute; // overload

  protected:

    // holds bools that control which histograms are filled
    HelperClasses::PhotonInfoSwitch* m_infoSwitch;

};

#endif
