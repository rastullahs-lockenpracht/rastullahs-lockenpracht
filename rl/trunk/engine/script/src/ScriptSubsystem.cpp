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

#include "ScriptSubsystem.h"
#include "ScriptObjectMarker.h"
#include "Logger.h"

using Ogre::Singleton;

template<> rl::ScriptSubsystem* Singleton<rl::ScriptSubsystem>::ms_Singleton = 0;

namespace rl {

    ScriptSubsystem& ScriptSubsystem::getSingleton()
    {
        return Singleton<ScriptSubsystem>::getSingleton();
    }

    ScriptSubsystem* ScriptSubsystem::getSingletonPtr()
    {
        return Singleton<ScriptSubsystem>::getSingletonPtr();
    }

    ScriptSubsystem::ScriptSubsystem() : mScriptObjectMarker(0)
    {
        mScriptObjectMarker = new ScriptObjectMarker();	
    }

    ScriptSubsystem::~ScriptSubsystem()
    {
        delete mScriptObjectMarker;
    }

    void ScriptSubsystem::log(const rl::CeGuiString &message)
    {
        Logger::getSingleton().log(Logger::SCRIPT, Ogre::LML_NORMAL, message);
    }

}
