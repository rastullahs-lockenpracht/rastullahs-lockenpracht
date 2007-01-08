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

#ifndef __RL_CEGUI_HELPER_H__
#define __RL_CEGUI_HELPER_H__

#include "UiPrerequisites.h"
namespace rl
{
    class CeGuiHelper
    {
    public:
        static CEGUI::UVector2 asAbsolute(const CEGUI::Vector2& position)
        {
            return CEGUI::UVector2(CEGUI::UDim(0, position.d_x), CEGUI::UDim(0, position.d_y));
        }

        static CEGUI::UVector2 asRelative(const CEGUI::Vector2& position)
        {
            return CEGUI::UVector2(CEGUI::UDim(position.d_x, 0), CEGUI::UDim(position.d_y, 0));
        }
    };
}

#endif
