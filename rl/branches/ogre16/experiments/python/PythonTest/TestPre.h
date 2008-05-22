#ifndef __TestPre_H__
#define __TestPre_H__

// Das macht Ogre auch
#pragma warning(disable : 4251)

// Für benutzen aus anderen DLLs sinnvoll...
#ifdef WIN32
// Export control
#   if defined( PYTEST_EXPORTS )
#       define _PyTestExport __declspec( dllexport )
#   else
#       define _PyTestExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlCoreExport
#endif

#endif