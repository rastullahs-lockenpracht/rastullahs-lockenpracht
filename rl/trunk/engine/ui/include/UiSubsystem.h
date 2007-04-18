/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include <OgreSingleton.h>

#include "CharacterController.h"

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

    class _RlUiExport UiSubsystem : public SceneChangeListener,
        protected Ogre::Singleton<UiSubsystem>
    {
    public:
        static const char* CEGUI_ROOT;

        /** Default Constructor */
        UiSubsystem();
        /** Default Deconstructor */
        virtual ~UiSubsystem();

        /** Returns the Singleton */
        static UiSubsystem & getSingleton(void);
        static UiSubsystem * getSingletonPtr(void);

        CEGUI::OgreCEGUIRenderer* getGUIRenderer();
        Person* getActiveCharacter() const;
        void setActiveCharacter(Person* person);

        /// from SceneChangeListener
        virtual void onBeforeClearScene();

        void initializeSubsystem();

        /// this method is here because it is easier to include it in swig here (actually really defined in InputManager)
        void linkKeyToRubyCommand(const CeGuiString &key, const CeGuiString &command);

    private:
        Person* mCharacter;

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
