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

#ifndef __CommonPrerequisites_H__
#define __CommonPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#if defined(RLCOMMON_EXPORTS)
#define _RlCommonExport __declspec(dllexport)
#else
#define _RlCommonExport __declspec(dllimport)
#endif
#else // Linux / Mac OSX etc
#define _RlCommonExport
#if defined(RL_USE_PCH)
#include "stdinc.h"
#endif
#endif

#undef _assert

#undef max
#undef min
#pragma warning(disable : 4267) // Konvertierungswarnung in CEGUIString

#ifdef __APPLE__
#include <CEGUI/CEGUIString.h>
#else
#include <CEGUI/String.h>
#endif

namespace rl
{
    typedef std::list<Ogre::String> StringList;
    typedef CEGUI::String CeGuiString;
    typedef std::vector<CeGuiString> CeGuiStringVector;
    typedef RL_LONGLONG Time;
    typedef std::pair<int, int> IntPair;
}

#include "Logger.h"

#endif
