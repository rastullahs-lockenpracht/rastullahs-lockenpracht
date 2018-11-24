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

#include "CameraObject.h"

#include "Actor.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "World.h"

using namespace Ogre;

namespace rl
{
    CameraObject::CameraObject(const String& name)
    {
        Camera* camera = CoreSubsystem::getSingleton().getWorld()->getSceneManager()->createCamera(name);

        camera->setNearClipDistance(0.1);
        camera->setFarClipDistance(5000);
        camera->setOrientation(Quaternion::IDENTITY);
        camera->setPosition(Vector3::ZERO);
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

    String CameraObject::getObjectType() const
    {
        return "CameraObject";
    }

    AxisAlignedBox CameraObject::getDefaultSize() const
    {
        Real r = getCamera()->getNearClipDistance();
        return AxisAlignedBox(2.0 * Vector3(-r, -r, -r), 2.0 * Vector3(r, r, r));
    }

    Vector3 CameraObject::getPointOnScreen(const Ogre::Vector3& worldCoords) const
    {
        const Camera* camera = getCamera();
        const Matrix4& viewMatrix = camera->getViewMatrix(true);
        const Matrix4& projMatrix = camera->getProjectionMatrix();

        Vector3 eyeSpacePos = viewMatrix * worldCoords;
        Vector3 screenSpacePos = projMatrix * eyeSpacePos;
        screenSpacePos.z = eyeSpacePos.z;

        return screenSpacePos;
    }

    Vector3 CameraObject::getPointOnCeGuiScreen(const Ogre::Vector3& worldCoords) const
    {
        // see Ogre::Camera::getCameraToViewportRay(Real, Real)
        //		Real nx = (2.0f * screenX) - 1.0f;
        //		Real ny = 1.0f - (2.0f * screenY);
        // -> screenX = (nx + 1.0f) / 2.0f
        // -> screenY = (1.0f - ny) / 2.0f

        Vector3 screenSpacePos = getPointOnScreen(worldCoords);
        screenSpacePos.x = (screenSpacePos.x + 1.0f) / 2.0f;
        screenSpacePos.y = (1.0f - screenSpacePos.y) / 2.0f;
        return screenSpacePos;
    }

    CameraObject::PixelPos CameraObject::getPixelPosOnScreen(float x, float y) const
    {
        int width = getCamera()->getViewport()->getActualWidth();
        int height = getCamera()->getViewport()->getActualHeight();

        int sx = (x + 1.0f) * width / 2.0f;
        int sy = (-y + 1.0f) * height / 2.0f;

        return PixelPos(sx, sy);
    }

    Ogre::Rect CameraObject::getPixelRectOnScreen(const Ogre::FloatRect& rect) const
    {
        PixelPos leftTop = getPixelPosOnScreen(rect.left, rect.top);
        PixelPos rightBottom = getPixelPosOnScreen(rect.right, rect.bottom);

        return Ogre::Rect(leftTop.first, leftTop.second, rightBottom.first, rightBottom.second);
    }

    Vector3 CameraObject::getDirectionFromScreenPosition(const Real& x, const Real& y) const
    {
        Ray ray = getCamera()->getCameraToViewportRay(x, y);
        return ray.getDirection();
    }

    Ray CameraObject::getCameraToViewportRay(const Real& x, const Real& y) const
    {
        return getCamera()->getCameraToViewportRay(x, y);
    }
}
