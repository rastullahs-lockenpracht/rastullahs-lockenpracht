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
#ifndef __CharacterSelectionWindow_H__
#define __CharacterSelectionWindow_H__

#include "UiPrerequisites.h"

#include "AbstractWindow.h"
#include "MessagePump.h"
#include "ObjectStateChangeListener.h"

namespace rl
{
    class Creature;

    class CharacterSelectionWindow : public AbstractWindow
    {
    public:
        CharacterSelectionWindow();
        virtual ~CharacterSelectionWindow();

        bool update();

    private:
        class Element : public AbstractWindow, public ObjectStateChangeListener
        {
        public:
            Element(CharacterSelectionWindow* parent);
            virtual ~Element();

            void setCreature(Creature* creature);
            void objectStateChanged(ObjectStateChangeEvent* evt);

        private:
            CharacterSelectionWindow* mParent;
            Creature* mCreature;
            CEGUI::Window* mTextName;
            CEGUI::Window* mPortrait;
            CEGUI::ProgressBar* mLP;

            void update();
        };

        std::vector<Element*> mCharacterWindows;
        CEGUI::Window* mCharacterWindow;

        bool handleClickOnCharacter(size_t index);
        CEGUI::Window* createCharacterWindow();
        void removeCharacterWindow(CEGUI::Window* window);

        MessagePump::ScopedConnection mCharacterAddedConnection;
        MessagePump::ScopedConnection mCharacterRemovedConnection;
    };
}

#endif // __CharacterSelectionWindow_H__
