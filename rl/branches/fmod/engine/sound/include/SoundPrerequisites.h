/* SoundPrerequisites.h - Verschiedene Einstellung für RlSound.
 * (C) 2003-2005. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __SoundPrerequisites_H__
#define __SoundPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLSOUND_EXPORTS )
#       define _RlSoundExport __declspec( dllexport )
#   else
#       define _RlSoundExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlSoundExport
#endif

#include "Exception.h"

extern "C" {
#include <fmod.h>
#include <fmod_errors.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
}


#endif
