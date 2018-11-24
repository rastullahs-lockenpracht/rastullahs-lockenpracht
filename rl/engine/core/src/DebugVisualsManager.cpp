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

#include "DebugVisualisable.h"
#include "DebugVisualsManager.h"
#include "Exception.h"

using namespace Ogre;

template <> rl::DebugVisualsManager* Singleton<rl::DebugVisualsManager>::msSingleton = 0;

namespace rl
{
    DebugVisualsManager::DebugVisualsManager()
        : mVisualisables()
    {
    }

    DebugVisualsManager::~DebugVisualsManager()
    {
    }

    void DebugVisualsManager::show(unsigned long flags)
    {
        for (VisualisableSet::iterator it = mVisualisables.begin(), end = mVisualisables.end(); it != end; ++it)
        {
            if ((*it)->getFlag() & flags)
            {
                (*it)->show();
            }
        }
    }

    void DebugVisualsManager::hide(unsigned long flags)
    {
        for (VisualisableSet::iterator it = mVisualisables.begin(), end = mVisualisables.end(); it != end; ++it)
        {
            if ((*it)->getFlag() & flags)
            {
                (*it)->hide();
            }
        }
    }

    void DebugVisualsManager::showAll()
    {
        for (VisualisableSet::iterator it = mVisualisables.begin(), end = mVisualisables.end(); it != end; ++it)
        {
            (*it)->show();
        }
    }

    void DebugVisualsManager::hideAll()
    {
        for (VisualisableSet::iterator it = mVisualisables.begin(), end = mVisualisables.end(); it != end; ++it)
        {
            (*it)->hide();
        }
    }

    void DebugVisualsManager::toggleAll()
    {
        for (VisualisableSet::iterator it = mVisualisables.begin(), end = mVisualisables.end(); it != end; ++it)
        {
            if ((*it)->isShown())
            {
                (*it)->hide();
            }
            else
            {
                (*it)->show();
            }
        }
    }

    void DebugVisualsManager::registerDebugVisualisable(DebugVisualisable* visualisable)
    {
        VisualisableSet::const_iterator it = mVisualisables.find(visualisable);
        if (it == mVisualisables.end())
        {
            mVisualisables.insert(visualisable);
        }
        else
        {
            Throw(IllegalArgumentException, "DebugVisualisable already registered.");
        }
    }

    void DebugVisualsManager::unregisterDebugVisualisable(DebugVisualisable* visualisable)
    {
        VisualisableSet::iterator it = mVisualisables.find(visualisable);
        if (it != mVisualisables.end())
        {
            mVisualisables.erase(it);
        }
        else
        {
            Throw(IllegalArgumentException, "DebugVisualisable not registered.");
        }
    }

    void DebugVisualsManager::run(Real elapsedTime)
    {
        for (VisualisableSet::iterator it = mVisualisables.begin(), end = mVisualisables.end(); it != end; ++it)
        {
            if ((*it)->isShown())
            {
                (*it)->updatePrimitive();
            }
        }
    }

    const Ogre::String& DebugVisualsManager::getName() const
    {
        static String NAME = "DebugVisualsManager";

        return NAME;
    }
}
