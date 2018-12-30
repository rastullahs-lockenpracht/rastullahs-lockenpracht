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

#ifndef __RL_LINESET_PRIMITIVE_H__
#define __RL_LINESET_PRIMITIVE_H__

#include "CorePrerequisites.h"
#include "PrimitiveObject.h"

#include <vector>

namespace rl
{

    /// Primitive class that encapsules a set of lines. Lines can have a colour,
    /// which is set as vertex colour, rather than a material colour, in order
    /// to allow for it to use a single render op.
    class _RlCoreExport LineSetPrimitive : public PrimitiveObject
    {
    public:
        /// Name has to be unique among all ActorControlledObject names.
        /// By default the name is generated.
        LineSetPrimitive(const Ogre::String& name = "");

        virtual ~LineSetPrimitive()
        {
        }

        /// Clears this line set and thus removes all lines from it.
        void clear();

        /// Adds a new line to the set.
        void addLine(const Ogre::Vector3& from, const Ogre::Vector3& to, const Ogre::ColourValue& colour);

        /// Returns "LineSetPrimitive" for this class.
        virtual Ogre::String getObjectType() const
        {
            return "LineSetPrimitive";
        }

    protected:
        virtual void updatePrimitive();

    private:
        struct Line
        {
            Line(const Ogre::Vector3& f, const Ogre::Vector3& t, const Ogre::ColourValue& c)
                : from(f)
                , to(t)
                , colour(c)
            {
            }
            Ogre::Vector3 from;
            Ogre::Vector3 to;
            Ogre::ColourValue colour;
        };
        std::vector<Line> mLines;
    };
}
#endif
