#ifndef __CommonPrerequisites_H__
#define __CommonPrerequisites_H__

#include "RastullahPrerequisites.h"

#undef _assert

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLCOMMON_EXPORTS )
#       define _RlCommonExport __declspec( dllexport )
#   else
#       define _RlCommonExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlCommonExport
#endif

#endif
