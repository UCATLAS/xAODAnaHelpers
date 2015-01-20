#ifndef xAODAnaHelpers_${name}_H
#define xAODAnaHelpers_${name}_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAOD${obj}/${obj}Container.h>

class ${name} : public HistogramManager
{
  public:
    ${name}(std::string name, std::string detailStr, std::string delimiter);
    ~${name}();

    EL::StatusCode initialize();
    EL::StatusCode execute( const xAOD::${obj}Container* ${obj_lc}s, float eventWeight );
    EL::StatusCode execute( const xAOD::${obj}* ${obj_lc}, float eventWeight );
    using HistogramManager::book; // make other overloaded version of book() to show up in subclass
    using HistogramManager::execute; // overload

  protected:
    // bools to control which histograms are filled
    bool m_fillPosition;     //!

  private:
    //basic
    TH1F* m_${obj_lc}Pt;                  //!
    // position
    TH1F* m_${obj_lc}Eta;                 //!
    TH1F* m_${obj_lc}Phi;                 //!
    TH1F* m_${obj_lc}Rapidity;            //!
};

#endif
