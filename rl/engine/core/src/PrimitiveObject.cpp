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

#include "CoreSubsystem.h"
#include "PrimitiveObject.h"
#include "World.h"

using namespace Ogre;

namespace rl
{

    PrimitiveObject::PrimitiveObject(const Ogre::String& name, const Ogre::String& materialName)
        : mMaterialName(materialName)
    {
        World* world = CoreSubsystem::getSingleton().getWorld();
        String manualObjectName = name == Ogre::BLANKSTRING ? world->getUniqueName() : name;
        mMovableObject = world->getSceneManager()->createManualObject(manualObjectName);

        if (mMaterialName == Ogre::BLANKSTRING)
        {
            mMaterialName = "DefaultDebugPrimitiveMaterial";
        }
    }

    PrimitiveObject::~PrimitiveObject()
    {
        CoreSubsystem::getSingleton().getWorld()->getSceneManager()->destroyManualObject(getManualObject());
    }

    void PrimitiveObject::setMaterial(const Ogre::String& materialName)
    {
        mMaterialName = materialName;
        updatePrimitive();
    }

    Ogre::String PrimitiveObject::getMaterial()
    {
        return mMaterialName;
    }

    Ogre::AxisAlignedBox PrimitiveObject::getDefaultSize() const
    {
        return getManualObject()->getBoundingBox();
    }

    Ogre::ManualObject* PrimitiveObject::getManualObject() const
    {
        return static_cast<ManualObject*>(mMovableObject);
    }
}
