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
#include "LineSetPrimitive.h"
#include "World.h"

using namespace Ogre;

namespace rl
{

    LineSetPrimitive::LineSetPrimitive(const String& name)
        : PrimitiveObject(name, "AmbientVertexColour")
        , mLines()
    {
    }

    void LineSetPrimitive::clear()
    {
        mLines.clear();
        updatePrimitive();
    }

    void LineSetPrimitive::addLine(const Vector3& from, const Vector3& to, const ColourValue& colour)
    {
        mLines.push_back(Line(from, to, colour));
        updatePrimitive();
    }

    void LineSetPrimitive::updatePrimitive()
    {
        ManualObject* mo = getManualObject();
        mo->clear();
        if (!mLines.empty())
        {
            mo->begin(mMaterialName, RenderOperation::OT_LINE_LIST);
            for (unsigned short i = 0; i < mLines.size(); ++i)
            {
                mo->position(mLines[i].from);
                mo->colour(mLines[i].colour);
                mo->position(mLines[i].to);
                mo->colour(mLines[i].colour);
            }
            mo->end();
        }
    }
}
