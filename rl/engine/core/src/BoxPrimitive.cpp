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

#include "BoxPrimitive.h"
#include "World.h"
#include "CoreSubsystem.h"

using namespace Ogre;
using namespace std;

namespace rl {

    BoxPrimitive::BoxPrimitive(const AxisAlignedBox& size,
        const String& name, const String& materialName)
        : PrimitiveObject(name, materialName),
          mSize(size)
    {
        drawBox();
    }

    void BoxPrimitive::setSize(const Ogre::AxisAlignedBox& size)
    {
        mSize = size;
        updatePrimitive();
    }

    Ogre::AxisAlignedBox BoxPrimitive::getSize()
    {
        return mSize;
    }

    void BoxPrimitive::updatePrimitive()
    {
        drawBox();
    }

    void BoxPrimitive::drawBox()
    {
        ManualObject* manualObject = getManualObject();
        manualObject->clear();
		std::vector<Vector3> corners;
        corners.reserve(8);

        Vector3 vmin = mSize.getMinimum();
        Vector3 vmax = mSize.getMaximum();

        manualObject->begin(mMaterialName);
            manualObject->position(vmin);                               // 0
            manualObject->position(Vector3(vmax.x, vmin.y, vmin.z));    // 1
            manualObject->position(Vector3(vmax.x, vmax.y, vmin.z));    // 2
            manualObject->position(Vector3(vmin.x, vmax.y, vmin.z));    // 3
            manualObject->position(Vector3(vmax.x, vmin.y, vmax.z));    // 4
            manualObject->position(Vector3(vmin.x, vmin.y, vmax.z));    // 5
            manualObject->position(Vector3(vmin.x, vmax.y, vmax.z));    // 6
            manualObject->position(vmax);                               // 7

            manualObject->quad(0, 3, 2, 1);
            manualObject->quad(0, 5, 6, 3);
            manualObject->quad(0, 1, 4, 5);
            manualObject->quad(7, 2, 3, 6);
            manualObject->quad(7, 4, 1, 2);
            manualObject->quad(7, 6, 5, 4);
        manualObject->end();
    }
}
