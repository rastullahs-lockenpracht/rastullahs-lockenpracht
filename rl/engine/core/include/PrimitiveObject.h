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

#ifndef __RL_PRIMITIVE_OBJECT_H__
#define __RL_PRIMITIVE_OBJECT_H__

#include "CorePrerequisites.h"

#include "PhysicalObject.h"

namespace rl
{

    /// Abstract super class for graphical primitives like boxes, spheres, etc.
    class _RlCoreExport PrimitiveObject : public PhysicalObject
    {
    public:
        /// Name has to be unique among all ActorControlledObject names.
        /// By default the name is generated.
        PrimitiveObject(const Ogre::String& name = "", const Ogre::String& materialName = "");

        virtual ~PrimitiveObject();

        void setMaterial(const Ogre::String& materialName);
        Ogre::String getMaterial();

        virtual Ogre::AxisAlignedBox getDefaultSize() const;

        Ogre::ManualObject* getManualObject() const;

    protected:
        Ogre::String mMaterialName;

        /// Called to notify subclasses to changes.
        virtual void updatePrimitive() = 0;
    };
}
#endif
