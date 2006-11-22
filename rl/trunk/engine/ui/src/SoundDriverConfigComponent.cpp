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

#include "SoundDriverConfigComponent.h"

#include "SoundDriver.h"
#include <CEGUIWindowManager.h>

namespace rl {
	SoundDriverConfigComponent::SoundDriverConfigComponent(const CeGuiString& xmlfile)
        : mXmlFile(xmlfile)
	{
	}

    SoundDriverConfigComponent::~SoundDriverConfigComponent()
    {
        if (mWindow != NULL)
        {
            mWindow->getParent()->removeChildWindow(mWindow);
            CEGUI::WindowManager::getSingleton().destroyWindow(mWindow);
            CEGUI::WindowManager::getSingleton().cleanDeadPool();
        }
    }

	const CeGuiString SoundDriverConfigComponent::getDriverName() const
	{
		return getDriver()->getName();
	}

    void SoundDriverConfigComponent::addTo(CEGUI::Window* parent)
    {
        mWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(mXmlFile);
        parent->addChildWindow(mWindow);
        initialize();
    }

    void SoundDriverConfigComponent::setVisible(bool visible)
    {
        mWindow->setVisible(visible);
    }
}
