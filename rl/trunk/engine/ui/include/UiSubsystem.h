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

#ifndef __UiSubsystem_H__
#define __UiSubsystem_H__

#include "UiPrerequisites.h"
#include "World.h"

#include <OgreSingleton.h>

#include "CharacterController.h"
#include "GameTask.h"

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
        protected Ogre::Singleton<UiSubsystem>, protected GameTask
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

		void useDefaultAction(GameObject* obj, Creature* actor);
		void usePickedObjectDefaultActions();
		void toggleObjectPicking();

		CEGUI::OgreCEGUIRenderer* getGUIRenderer();
		Person* getActiveCharacter() const;
		void setActiveCharacter(Person* person);
        CharacterController* getCharacterController() const;
		CharacterController::ControllerType getCharacterControllerType() const;
		
		/**
		 * Aendert den CharacterController, wird sofort durchgefuehrt
		 * kann Probleme bei Ausfuehrung innerhalb eines Controllers geben,
		 * dann ist @see requestCharacterControllerSwitch empfohlen
		*/
		void setCharacterController(CharacterController::ControllerType type);
		
		/**
		 * Aendert den CharacterController nach dem naechsten Frame
		*/
		void requestCharacterControllerSwitch(CharacterController::ControllerType type);

		void update();

        /// from SceneChangeListener
        void onBeforeClearScene();

		void run(Ogre::Real elapsedTime);

    private:
        CharacterController* mCharacterController;
		CharacterController::ControllerType mCharacterControllerType;
        GameActor* mHero;
        Person* mCharacter;

        // Singletons
		InputManager* mInputManager;
		WindowFactory* mWindowFactory;
		WindowManager* mWindowManager;

        CEGUI::OgreCEGUIRenderer* mGuiRenderer;
        CEGUI::OgreCEGUIResourceProvider* mGuiResourceProvider;
        CEGUI::System* mGuiSystem;

        void initializeUiSubsystem();
        void runTest();
    };
}

#endif
