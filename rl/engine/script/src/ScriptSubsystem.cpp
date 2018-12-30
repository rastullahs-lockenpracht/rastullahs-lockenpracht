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
#include "stdinc.h" //precompiled header

#include "ScriptSubsystem.h"

#include "Logger.h"
#include "SceneManager.h"
#include "ScriptObjectMarker.h"

#include "SceneManager.h"

using Ogre::Singleton;

template <> rl::ScriptSubsystem* Singleton<rl::ScriptSubsystem>::msSingleton = NULL;

namespace rl
{
    ScriptSubsystem::ScriptSubsystem()
        : mScriptObjectMarker(NULL)
        , mTriggerFactory(NULL)
    {
        mScriptObjectMarker = new ScriptObjectMarker();
        new SceneManager();
    }

    ScriptSubsystem::~ScriptSubsystem()
    {
        delete SceneManager::getSingletonPtr();
        delete mScriptObjectMarker;
    }

    void ScriptSubsystem::log(const CeGuiString& message)
    {
        LOG_MESSAGE(Logger::SCRIPT, message);
    }

    void ScriptSubsystem::logError(const CeGuiString& message)
    {
        LOG_ERROR(Logger::SCRIPT, message);
    }

    TriggerFactory* ScriptSubsystem::getTriggerFactory() const
    {
        return mTriggerFactory;
    }

    void ScriptSubsystem::setTriggerFactory(TriggerFactory* factory)
    {
        mTriggerFactory = factory;
    }
}
