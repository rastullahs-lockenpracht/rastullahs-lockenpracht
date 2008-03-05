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

#ifndef __UiSubsystem_H__
#define __UiSubsystem_H__

#include "UiPrerequisites.h"
#include "World.h"


#include "ControlState.h"
#include "MessagePump.h"

// Gar nicht schön, aber ansonsten gibt es unnötige Abhängigkeiten,
// wenn man die Header hier inkludiert.
namespace CEGUI {
    class OgreCEGUIRenderer;
    class OgreCEGUIResourceProvider;
    class System;
}

namespace rl {

    class Creature;
    class GameActor;
    class GameObject;
    class InputManager;
    class Person;
    class RBCombat;
    class RTCombat;
    class WindowFactory;
    class WindowManager;

    class _RlUiExport UiSubsystem : public Ogre::Singleton<UiSubsystem>
    {
    public:
        static const char* CEGUI_ROOT;

        /** Default Constructor */
        UiSubsystem();
        /** Default Deconstructor */
        virtual ~UiSubsystem();

        CEGUI::OgreCEGUIRenderer* getGUIRenderer();
        Person* getActiveCharacter() const;
        ///@todo function feels misplaced here,
        void setActiveCharacter(Person* person);

        virtual bool onBeforeClearScene();
        virtual bool onBeforeGameObjectsLoaded();
        virtual bool onGameObjectsLoaded();

        void initializeSubsystem();

    private:
        Person* mCharacter;
        int mCharacterId;

	    MessagePump::ScopedConnection mSceneClearingConnection;
        MessagePump::ScopedConnection mGameObjectsLoadedConnection;
        MessagePump::ScopedConnection mBeforeLoadingGameObjectsConnection;

        // Singletons
        InputManager* mInputManager;
        WindowFactory* mWindowFactory;
        WindowManager* mWindowManager;

        CEGUI::OgreCEGUIRenderer* mGuiRenderer;
        CEGUI::ResourceProvider* mGuiResourceProvider;
        CEGUI::System* mGuiSystem;
    };
}

#endif
