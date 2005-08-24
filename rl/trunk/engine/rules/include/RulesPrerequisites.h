/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __RulesPrerequisites_H__
#define __RulesPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLRULES_EXPORTS )
#       define _RlRulesExport __declspec( dllexport )
#   else
#       define _RlRulesExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlRulesExport
#endif

#include <map>
#include <vector>

#define _t(T) CeGuiString(T)

#include <OgreLogManager.h>

#define log_error(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_CRITICAL);
#define log_info(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_NORMAL);
#define log_debug(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_TRIVIAL);

#undef min
#include <OgreNoMemoryMacros.h>
#pragma warning (push)
#pragma warning( disable : 4267 ) // Konvertierungswarnung in CEGUIString
#include <CEGUIString.h>
#pragma warning (pop)
#include <OgreMemoryMacros.h>

namespace rl
{
    class _RlRulesExport Action;
    class _RlRulesExport GameObject;

	typedef std::vector<Action*> ActionVector;
	typedef CEGUI::String CeGuiString;
    typedef std::vector<CeGuiString> CeGuiStringVector;

	ActionVector::iterator findAction(ActionVector::iterator& begin, ActionVector::iterator& end, const CeGuiString& actionName);
}
#endif
