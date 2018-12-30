#ifndef __MeshCombinerPrerequisites_H__
#define __MeshCombinerPrerequisites_H__

#include <OgrePlatform.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( MESHCOMBINER_EXPORTS )
#       define _MCExport __declspec( dllexport )
#   else
#       define _MCExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _MCExport
#endif

#endif // __MeshCombinerPrerequisites_H__
