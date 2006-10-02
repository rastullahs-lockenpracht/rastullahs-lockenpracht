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

#ifndef __Fmod4DriverPrerequisites_H__
#define __Fmod4DriverPrerequisites_H__

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLFMOD4DRIVER_EXPORTS )
#       define _RlFmod4DriverExport __declspec( dllexport )
#   else
#       define _RlFmod4DriverExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlFmod4DriverExport
#endif

#include <OgreNoMemoryMacros.h>
#   undef min
#   pragma warning (push)
#   pragma warning (disable : 4267)
#   include <CEGUIString.h>
#   pragma warning (pop)
#include <OgreMemoryMacros.h>

namespace rl
{
    typedef CEGUI::String CeGuiString;
    typedef std::vector<CeGuiString> CeGuiStringVector;
    typedef std::list<Ogre::String> StringList;
}


#endif //__Fmod4DriverPrerequisites_H__
