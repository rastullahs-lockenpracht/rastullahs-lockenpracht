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

#include "ScriptWrapper.h"

using namespace Ogre;

template <> rl::ScriptWrapper* Singleton<rl::ScriptWrapper>::msSingleton = 0;

namespace rl
{

    ScriptWrapper::ScriptWrapper()
        : m_ScriptWrapperInstance(NULL)
    {
    }

    ScriptWrapper::~ScriptWrapper()
    {
    }

    void ScriptWrapper::setScriptWrapperInstance(ScriptWrapperInstance* list)
    {
        m_ScriptWrapperInstance = list;
    }

    void ScriptWrapper::deleted(void* ptr)
    {
        if (m_ScriptWrapperInstance != NULL)
        {
            m_ScriptWrapperInstance->deleted(ptr);
        }
    }

    void ScriptWrapper::owned(void* ptr)
    {
        if (m_ScriptWrapperInstance != NULL)
        {
            m_ScriptWrapperInstance->owned(ptr);
        }
    }

    void ScriptWrapper::disowned(void* ptr)
    {
        if (m_ScriptWrapperInstance != NULL)
        {
            m_ScriptWrapperInstance->disowned(ptr);
        }
    }
}
