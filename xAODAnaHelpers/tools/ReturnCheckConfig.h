// Dear emacs, this is -*- c++ -*-
#ifndef xAODAnaHelpers_TOOLS_RETURNCHECKCONFIG_H
#define xAODAnaHelpers_TOOLS_RETURNCHECKCONFIG_H

// ROOT include(s):
#include <TError.h>
#include <TSystem.h>

#include <xAODAnaHelpers/tools/Message.h>
#include <EventLoop/StatusCode.h>

/// Helper macro for checking return codes in a compact form in the code
///
/// This is pretty much a rip-off of the (ATH_)CHECK macros of the offline
/// code. It is used in the package in functions that return a TReturnCode,
/// and themselves call functions returning TReturnCode.
///
/// @param CONTEXT A context string to print an error message on failure
/// @param CONFIG A std::string config name, cannot be TString!
///
#define RETURN_CHECK_CONFIG( CONTEXT, CONFIG )                             \
   do {                                                                    \
      /* check if file exists                                              \
       https://root.cern.ch/root/roottalk/roottalk02/5332.html */          \
      FileStat_t fStats;                                                   \
      int fSuccess = gSystem->GetPathInfo(CONFIG.c_str(), fStats);         \
      if(fSuccess != 0){                                                   \
        ::Error( CONTEXT, XAOD_MESSAGE( "Could not find the "              \
              "configuration file: %s."), CONFIG.c_str());                 \
        return EL::StatusCode::FAILURE;                                    \
      } else {                                                             \
        ::Info( CONTEXT, "Found configuration file: %s", CONFIG.c_str());  \
      }                                                                    \
   } while( false )

#endif // xAODAnaHelpers_TOOLS_RETURNCHECKCONFIG_H


