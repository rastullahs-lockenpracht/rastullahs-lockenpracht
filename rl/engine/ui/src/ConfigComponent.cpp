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

#include <CEGUI/WindowManager.h>
#include <iostream>

#include "ConfigComponent.h"

namespace rl
{
    ConfigComponent::ConfigComponent(const CEGUI::String& xmlfile, const CEGUI::String& drivername)
    {
        // Set layout file for component
        mXmlFile = xmlfile;

        // Set user readable name for driver
        mName = drivername;
    }

    ConfigComponent::~ConfigComponent()
    {
        // Free components root window and its child windows
        if (mWindow != NULL)
        {
            mWindow->getParent()->removeChild(mWindow);
            CEGUI::WindowManager::getSingleton().destroyWindow(mWindow);
            CEGUI::WindowManager::getSingleton().cleanDeadPool();
        }
    }

    void ConfigComponent::addTo(CEGUI::Window* parent)
    {
        mWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(mXmlFile);
        parent->addChild(mWindow);
    }

    void ConfigComponent::setVisible(bool visible)
    {
        mWindow->setVisible(visible);
    }

    const CEGUI::String ConfigComponent::getDriverName() const
    {
        return mName;
    }
}
