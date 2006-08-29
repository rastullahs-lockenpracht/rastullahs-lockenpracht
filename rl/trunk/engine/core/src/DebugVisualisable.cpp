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

#include "DebugVisualisable.h"
#include "DebugVisualsManager.h"
#include "PrimitiveObject.h"
#include "CoreSubsystem.h"
#include "World.h"

using namespace Ogre;

namespace rl {

    DebugVisualisable::DebugVisualisable()
        : mSceneNode(NULL),
          mPrimitive(NULL),
          mIsShown(false)
    {
        DebugVisualsManager::getSingleton().registerDebugVisualisable(this);
    }

    DebugVisualisable::~DebugVisualisable()
    {
        DebugVisualsManager::getSingleton().unregisterDebugVisualisable(this);

        if (mSceneNode != NULL)
        {
            mPrimitive->_detachSceneNode(mSceneNode);
            CoreSubsystem::getSingleton().getWorld()->
                getSceneManager()->destroySceneNode(mSceneNode->getName());
        }

        delete mPrimitive;
    }

    void DebugVisualisable::show()
    {
        if (mSceneNode == NULL)
        {
            mSceneNode = CoreSubsystem::getSingleton().getWorld()->
                getSceneManager()->createSceneNode();
            doCreatePrimitive();
            mPrimitive->_attachSceneNode(mSceneNode);
        }
        else
        {
            mSceneNode->setVisible(true);
        }
        mIsShown = true;
    }

    void DebugVisualisable::hide()
    {
        if (mSceneNode != NULL)
        {
            mSceneNode->setVisible(false);
        }
        mIsShown = false;
    }

    bool DebugVisualisable::isShown() const
    {
        return mIsShown;
    }
}
