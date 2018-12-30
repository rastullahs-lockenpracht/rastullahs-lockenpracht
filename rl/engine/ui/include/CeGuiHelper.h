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

#ifndef __RL_CEGUI_HELPER_H__
#define __RL_CEGUI_HELPER_H__

#include "UiPrerequisites.h"

#include <CEGUI/Size.h>
#include <CEGUI/Vector.h>

namespace rl
{
    class CeGuiHelper
    {
    public:
        static CEGUI::UVector2 asAbsolute(int x, int y)
        {
            return CEGUI::UVector2(CEGUI::UDim(0, x), CEGUI::UDim(0, y));
        }

        static CEGUI::UVector2 asAbsolute(const CEGUI::UVector2& position, const CEGUI::Sizef& size)
        {
            return CEGUI::UVector2(
                CEGUI::UDim(0, static_cast<int>(position.d_x.d_scale * size.d_width + position.d_x.d_offset)),
                CEGUI::UDim(0, static_cast<int>(position.d_y.d_scale * size.d_height + position.d_y.d_offset)));
        }

        static CEGUI::UVector2 asRelative(float x, float y)
        {
            return CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0));
        }

        static CEGUI::UVector2 asRelative(const CEGUI::UVector2& position, const CEGUI::Sizef& size)
        {
            auto w = position.d_x.d_scale * size.d_width + position.d_x.d_offset;
            auto h = position.d_y.d_scale * size.d_height + position.d_y.d_offset;

            return CEGUI::UVector2(CEGUI::UDim(size.d_width / w, 0), CEGUI::UDim(size.d_height / h, 0));
        }

        template <typename T> static CEGUI::Vector2<T> toPosition(const CEGUI::Size<T>& size)
        {
            return CEGUI::Vector2<T>(size.d_width, size.d_height);
        }
    };
}

#endif
