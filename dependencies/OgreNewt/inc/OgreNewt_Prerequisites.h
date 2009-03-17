/* 
    OgreNewt Library

    Ogre implementation of Newton Game Dynamics SDK

    OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

        by Walaber
        some changes by melven

*/


#ifndef __INCLUDE_OGRENEWT_PREREQ__
#define __INCLUDE_OGRENEWT_PREREQ__

#ifdef __APPLE__
#    include <Carbon/Carbon.h>
#   include <Ogre/OgreVector3.h>
#   include <Ogre/OgreQuaternion.h>
#   include <Ogre/OgreMovableObject.h>
#   include <Ogre/OgreRenderable.h>
#   include <Ogre/OgreNode.h>
#   include <Ogre/OgreFrameListener.h>
#else
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreMovableObject.h>
#include <OgreRenderable.h>
#include <OgreNode.h>
#include <OgreFrameListener.h>
#endif

#include <Newton.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   define _CDECL _cdecl
#   if defined( _OGRENEWT_EXPORTS ) && defined( _OGRENEWT_DYNAMIC )
#       define _OgreNewtExport __declspec( dllexport )
#   elif defined( _OGRENEWT_DYNAMIC )
#       define _OgreNewtExport __declspec( dllimport )
#   else
#       define _OgreNewtExport
#   endif
#else // Linux / Mac OSX etc
#   define _OgreNewtExport
#   define _CDECL
#endif

#endif 

