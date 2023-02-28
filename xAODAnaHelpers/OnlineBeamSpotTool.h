#ifndef xAODAnaHelpers_OnlineBeamSpotTool_H
#define xAODAnaHelpers_OnlineBeamSpotTool_H

#include "xAODEventInfo/EventInfo.h"
#include "xAODAnaHelpers/EventInfo.h"

#include <vector>
#include <map>

namespace xAH {

  class OnlineBeamSpotTool
  {

  public:
    enum BSData{
      BSx,
      BSy,
      BSz
    };

  private:
    struct LBData {
      int m_LBStart;
      int m_LBEnd;
      float m_BSx;
      float m_BSy;
      float m_BSz;

      LBData(int LBStart, int LBEnd, float BSx, float BSy, float BSz){
	m_LBStart = LBStart;
	m_LBEnd   = LBEnd;
	m_BSx     = BSx;
	m_BSy     = BSy;
	m_BSz     = BSz;
      }
    };

    typedef std::vector<LBData>    RunInfo;
    typedef std::map<int, RunInfo> RunToLBDataMap;
    typedef std::map<int, RunInfo>::iterator RunToLBDataMapItr;

  public:

    OnlineBeamSpotTool();
    ~OnlineBeamSpotTool();

    float getOnlineBSInfo(const xAOD::EventInfo* eventInfo, BSData datakey);
    float getOnlineBSInfo(const xAH::EventInfo* eventInfo, BSData datakey);
    float getOnlineBSInfo(int runNumber, int lumiBlock, bool isMC, BSData datakey);

  private:

    const LBData*  getLBData(int runNumber, int lumiBlock, bool isMC);
    const LBData*  getLBData(int lumiBlock);

    void setRunInfo(int runNumber);

    void readFiles();
    void readFile(std::string rootFileName);

    RunToLBDataMap m_runList;

    int m_cachedRunNum;
    int m_cachedLB;
    RunInfo* m_cachedRunInfo;
    LBData*  m_cachedLBData;
    LBData*  m_mcLBData;

    bool m_files_loaded;
  };

}//xAH
#endif // xAODAnaHelpers_OnlineBeamSpotTool_H
