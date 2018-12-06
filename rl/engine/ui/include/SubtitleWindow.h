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
#ifndef __SubtitleWindow_H__
#define __SubtitleWindow_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"

namespace rl
{
    class GameObject;

    class _RlUiExport SubtitleWindow : public AbstractWindow
    {
    public:
        SubtitleWindow();
        virtual ~SubtitleWindow();

        void show(const CeGuiString& text);
        const CeGuiString& getText() const;

    private:
        CEGUI::MultiLineEditbox* mSubtitle;
    };

} // namespace rl

#endif // __SubtitleWindow_H__
