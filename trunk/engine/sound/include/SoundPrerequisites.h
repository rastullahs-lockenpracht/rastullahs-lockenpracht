#ifndef __SoundPrerequisites_H__
#define __SoundPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLSOUND_EXPORTS )
#       define _RlSoundExport __declspec( dllexport )
#   else
#       define _RlSoundExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlSoundExport
#endif

//#include "OgreNoMemoryMacros.h"
#include "Exception.h"
extern "C" {
#include "AL/al.h"
#include "AL/alu.h"
#include "AL/alut.h"
#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
}


#endif
