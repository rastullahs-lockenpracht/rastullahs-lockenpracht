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

#include "CameraObject.h"
#include "Actor.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "Exception.h"

using namespace Ogre;

namespace rl {
    CameraObject::CameraObject(const String& name)
    {
        Camera* camera = CoreSubsystem::getSingleton().getWorld()->
            getSceneManager()->createCamera(name);

        camera->setNearClipDistance(0.1);
        camera->setFarClipDistance(10000);

        mMovableObject = camera;
    }

    CameraObject::~CameraObject()
    {
        CoreSubsystem::getSingleton().getWorld()->getSceneManager()->destroyCamera(getCamera());
    }

    Camera* CameraObject::getCamera() const
    {
        return static_cast<Camera*>(mMovableObject);
    }

    String CameraObject::getObjectType()
    {
        return "CameraObject";
    }

    Vector3 CameraObject::getDefaultSize() const
    {
        return Vector3(getCamera()->getNearClipDistance() * 3.0f, 0, 0);
    }

    Entity* CameraObject::getEntity() const
    {
        Throw(rl::OperationNotSupportedException, "CameraObject::getEntity not implemented.");
    }

    Vector3 CameraObject::getPoseSize(const String&)
    {
        Throw(OperationNotSupportedException,
            "CameraObject::calculateSizeFromPose not implemented.");
    }
}
