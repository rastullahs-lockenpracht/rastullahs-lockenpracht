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


#include "ControlState.h"
#include "MessagePump.h"
#include "Effect.h"

// Gar nicht schˆn, aber ansonsten gibt es unnˆtige Abh‰ngigkeiten,
// wenn man die Header hier inkludiert.
namespace CEGUI {
    class OgreRenderer;
    class System;
}

namespace rl {

    class Creature;
    class GameActor;
    class GameObject;
    class InputManager;
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

        CEGUI::OgreRenderer* getGUIRenderer();

        void initializeSubsystem();

    private:
        Creature* mCharacter;
        int mCharacterId;

	    MessagePump::ScopedConnection mSceneClearingConnection;
        MessagePump::ScopedConnection mGameObjectsLoadedConnection;
        MessagePump::ScopedConnection mBeforeLoadingGameObjectsConnection;
        MessagePump::ScopedConnection mActiveCharacterChangedConnection;
        MessagePump::ScopedConnection mAllPlayerCharactersDiedConnection;

        bool onBeforeClearScene();
        bool onBeforeGameObjectsLoaded();
        bool onGameObjectsLoaded();
        ///@todo function feels misplaced here,
        bool onActiveCharacterChanged(Creature* oldActive, Creature* newActive);
        bool onAllPlayerCharactersDied();
        

        // Singletons
        InputManager* mInputManager;
        WindowFactory* mWindowFactory;
        WindowManager* mWindowManager;

        CEGUI::OgreRenderer* mGuiRenderer;
        CEGUI::ResourceProvider* mGuiResourceProvider;
        CEGUI::System* mGuiSystem;
        
    };
}

#endif
