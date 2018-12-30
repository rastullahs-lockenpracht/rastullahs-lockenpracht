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

#ifndef __RL_BOX_PRIMITIVE_H__
#define __RL_BOX_PRIMITIVE_H__

#include "CorePrerequisites.h"
#include "PrimitiveObject.h"

namespace rl
{

    /// Simple box primitive. Size is determined by an axis aligned box.
    class _RlCoreExport BoxPrimitive : public PrimitiveObject
    {
    public:
        /// Name has to be unique among all ActorControlledObject names.
        /// By default the name is generated.
        BoxPrimitive(
            const Ogre::AxisAlignedBox& size, const Ogre::String& name = "", const Ogre::String& materialName = "");

        virtual ~BoxPrimitive()
        {
        }

        /// Sets new size of the box and redraws it.
        void setSize(const Ogre::AxisAlignedBox& size);

        /// Get the size of the box.
        Ogre::AxisAlignedBox getSize();

        /// Returns "BoxPrimitive" for this class.
        virtual Ogre::String getObjectType() const
        {
            return "BoxPrimitive";
        }

    protected:
        virtual void updatePrimitive();

    private:
        Ogre::AxisAlignedBox mSize;
        void drawBox();
    };
}
#endif
