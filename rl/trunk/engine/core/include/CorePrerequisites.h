#ifndef __CorePrerequisites_H__
#define __CorePrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLCORE_EXPORTS )
#       define _RlCoreExport __declspec( dllexport )
#   else
#       define _RlCoreExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlCoreExport
#endif

#endif
