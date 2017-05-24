#warning Please use instead ANA_CHECK in AsgTools/MessageCheck.h

#include "AsgTools/MessageCheck.h"

#ifndef xAODAnaHelpers_TOOLS_RETURNCHECK_H
#define xAODAnaHelpers_TOOLS_RETURNCHECK_H

#define RETURN_CHECK( CONTEXT, EXP, INFO )  ANA_CHECK(EXP)
#define EL_RETURN_CHECK( CONTEXT, EXP )     ANA_CHECK(EXP)

#endif // xAODAnaHelpers_TOOLS_RETURNCHECK_H
