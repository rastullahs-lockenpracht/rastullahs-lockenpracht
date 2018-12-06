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

#ifndef __UiPrerequisites_H__
#define __UiPrerequisites_H__

#undef _assert

#include <vector>
#include "FixRubyHeaders.h"

#include "CommonPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLUI_EXPORTS )
#       define _RlUiExport __declspec( dllexport )
#   else
#       define _RlUiExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlUiExport
#	if defined ( RL_USE_PCH )
#		include "stdinc.h"
#	endif
#endif

// Important types. Probably will have to be relocated, though it is used throughout RlUi.
namespace rl
{
    enum ControlStateType 
    {
	    CST_NONE = 1,
	    CST_FREEFLIGHT,
	    CST_MOVEMENT,
	    CST_DIALOG,
	    CST_CUTSCENE,
	    CST_COMBAT,
    };
}
#endif
