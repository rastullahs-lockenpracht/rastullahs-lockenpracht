#ifndef __DialogPrerequisites_H__
#define __DialogPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLDIALOG_EXPORTS )
#       define _RlDialogExport __declspec( dllexport )
#   else
#       define _RlDialogExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlDialogExport
#endif

#pragma warning (disable : 4251)

#endif

