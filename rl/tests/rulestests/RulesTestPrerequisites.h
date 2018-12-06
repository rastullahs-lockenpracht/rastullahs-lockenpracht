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

#ifndef __RulesTestPrerequisites_H__
#define __RulesTestPrerequisites_H__

#include "CommonPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLRULESTEST_EXPORTS )
#       define _RlRulesTestExport __declspec( dllexport )
#   else
#       define _RlRulesTestExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlRulesTestExport
#endif

#include <map>
#include <vector>

#define _t(T) CeGuiString(T)

#include <OgreLogManager.h>

#define log_error(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_CRITICAL);
#define log_info(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_NORMAL);
#define log_debug(msg) Ogre::LogManager::getSingleton().logMessage(msg, LML_TRIVIAL);

#undef min

#endif
