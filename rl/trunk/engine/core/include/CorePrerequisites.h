/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#ifndef __CorePrerequisites_H__
#define __CorePrerequisites_H__

#include "CommonPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLCORE_EXPORTS )
#       define _RlCoreExport __declspec( dllexport )
#   else
#       define _RlCoreExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlCoreExport
#	if defined ( RL_USE_PCH )
#		include "stdinc.h"
#	endif
#endif

#endif
