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

#include "Landmark.h"
#include "CoreSubsystem.h"
#include "LineSetPrimitive.h"
#include "World.h"

namespace rl
{
	Landmark::Landmark(const Ogre::String &name, const Ogre::Vector3 &position)
		: mName(name), mPosition(position)
	{
	}

    const Ogre::String& Landmark::getName() const
    {
        return mName;
    }

    const Ogre::Vector3& Landmark::getPosition() const
    {
        return mPosition;
    }

    DebugVisualisableFlag Landmark::getFlag() const
    {
        return DVF_WAYPOINT;
    }
    void Landmark::updatePrimitive()
    {
        if (mSceneNode->getParent() == NULL)
        {
		    CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->
			    getRootSceneNode()->addChild(mSceneNode);
		    //mCharacterActor->_getSceneNode()->addChild(mSceneNode);
        }

        LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);

	    lineSet->clear();
		// draw the waypoint itself
        lineSet->addLine(mPosition, mPosition + Ogre::Vector3(0,2,0), Ogre::ColourValue::Green);
    }

    void Landmark::doCreatePrimitive()
    {
	    mPrimitive = new LineSetPrimitive();
    }
}
