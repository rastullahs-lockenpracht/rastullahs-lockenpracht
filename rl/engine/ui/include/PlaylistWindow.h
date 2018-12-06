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

#ifndef __PlaylistWindow_H__
#define __PlaylistWindow_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"

namespace rl
{

    class Person;

    class _RlUiExport PlaylistWindow : public AbstractWindow
    {
    public:
        PlaylistWindow();
        ~PlaylistWindow();

        /**
         * Aktualisiert die Tabellenelemente
         */
        void update();

    private:
        bool handleQuit();
        bool handlePlay();

        Person* mCharacter;

        CEGUI::Window* mSheet;
        CEGUI::MultiColumnList* mPlaylistTable;
    };
}

#endif
