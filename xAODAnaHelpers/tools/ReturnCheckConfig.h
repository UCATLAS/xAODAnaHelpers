#warning Please use instead ANA_CHECK in AsgTools/MessageCheck.h

#ifndef xAODAnaHelpers_TOOLS_RETURNCHECKCONFIG_H
#define xAODAnaHelpers_TOOLS_RETURNCHECKCONFIG_H

#include "AsgTools/MessageCheck.h"

#define RETURN_CHECK_CONFIG( CONTEXT, CONFIG )                             \
     do {                                                                    \
             /* check if file exists                                              \
                       https://root.cern.ch/root/roottalk/roottalk02/5332.html */          \
             FileStat_t fStats;                                                   \
             int fSuccess = gSystem->GetPathInfo(CONFIG.c_str(), fStats);         \
             if(fSuccess != 0){                                                   \
                       ANA_MSG_ERROR( "Could not find the "                       \
                                           "configuration file:" << CONFIG.c_str());      \
                       return StatusCode::FAILURE;                                    \
                     } else {                                                             \
                               ANA_MSG_INFO( "Found configuration file: " << CONFIG.c_str());  \
                             }                                                                    \
          } while( false )

#endif // xAODAnaHelpers_TOOLS_RETURNCHECKCONFIG_H
